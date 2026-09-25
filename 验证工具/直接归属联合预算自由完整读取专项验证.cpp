#include "../海中鱼巣/领域/数据服务.场景类.h"
#include "../海中鱼巣/领域/数据服务.存在类.h"

#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

namespace {
using namespace 海中鱼巣;

int 通过数量 = 0;

void 检查(bool 条件, std::string_view 名称) {
  if (!条件) {
    std::cerr << "FAIL " << 名称 << '\n';
    throw std::runtime_error("direct ownership union v2 probe failed");
  }
  ++通过数量;
  std::cout << "PASS " << 名称 << '\n';
}

std::uint64_t 当前代次(const L1事实基座服务 &服务) {
  const auto 结果 = 服务.读取中性当前事实代次({L1中性CRUD合同版本});
  检查(结果.状态 == L1中性读取状态::成功 && 结果.事实代次 != 0,
       "current-generation-read");
  return 结果.事实代次;
}

L1所有者范围交付 建立所有者(L1所有者范围签发器 &签发器, std::uint64_t 键) {
  auto 结果 = 签发器.建立所有者范围(
      {L1所有者范围CRUD合同版本, {键}, L1所有者范围种类::独占结构范围});
  检查(结果.建立结果.状态 == L1所有者范围管理状态::成功 &&
           结果.写入端口 && 结果.写入端口->有效(),
       "owner-created");
  return 结果;
}

using 节点规格 =
    std::pair<节点种类, std::optional<L1所有者范围值表示种类>>;

std::vector<稳定编码> 建立元数据(L1所有者范围写端口 &端口,
                                  const L1事实基座服务 &服务,
                                  std::uint64_t 键,
                                  const std::vector<节点规格> &规格组) {
  L1所有者范围写集请求 请求{L1所有者范围CRUD合同版本, 当前代次(服务), {键}};
  for (std::uint32_t i = 0; i < 规格组.size(); ++i)
    请求.节点.push_back({{i + 1}, 规格组[i].first, 规格组[i].second});
  const auto 写入 = 端口.提交所有者范围中性写集(请求);
  检查(写入.状态 == L1所有者范围写入状态::成功 &&
           写入.新编码映射.size() == 规格组.size(),
       "metadata-published");
  std::vector<稳定编码> 编码组(规格组.size());
  for (const auto &[本地键, 编码] : 写入.新编码映射)
    if (本地键.值 && 本地键.值 <= 编码组.size())
      编码组[本地键.值 - 1] = 编码;
  for (const auto 编码 : 编码组)
    检查(有效(编码), "metadata-id-valid");
  return 编码组;
}

std::map<std::uint64_t, 稳定编码>
提交写集(L1所有者范围写端口 &端口, const L1事实基座服务 &服务,
         std::uint64_t 键, L1所有者范围写集请求 请求) {
  请求.合同版本 = L1所有者范围CRUD合同版本;
  请求.期望事实代次 = 当前代次(服务);
  请求.写入幂等身份 = {键};
  const auto 写入 = 端口.提交所有者范围中性写集(请求);
  检查(写入.状态 == L1所有者范围写入状态::成功 &&
           写入.是否形成内存权威发布,
       "fact-set-published");
  std::map<std::uint64_t, 稳定编码> 结果;
  for (const auto &[本地键, 编码] : 写入.新编码映射)
    结果.emplace(本地键.值, 编码);
  return 结果;
}

void 退出事实(L1所有者范围写端口 &端口, const L1事实基座服务 &服务,
              std::uint64_t 键, std::vector<稳定编码> 编码组) {
  L1所有者范围写集请求 请求{L1所有者范围CRUD合同版本, 当前代次(服务), {键}};
  请求.退出事实 = std::move(编码组);
  const auto 写入 = 端口.提交所有者范围中性写集(请求);
  检查(写入.状态 == L1所有者范围写入状态::成功,
       "facts-exited");
}

struct 夹具 {
  std::unique_ptr<L1事实基座运行包> 运行包;
  L1所有者范围交付 特征定义所有者, 特征信息所有者, 存在所有者, 场景所有者,
      状态所有者, 异主所有者;
  std::unique_ptr<特征值类数据服务> 特征值;
  std::unique_ptr<特征类数据服务> 特征;
  std::unique_ptr<存在类数据服务> 存在;
  std::unique_ptr<状态类数据服务> 状态;
  std::unique_ptr<场景类数据服务> 场景;
  std::unique_ptr<直接归属联合只读组合器> 联合;
  std::vector<稳定编码> E;
  std::map<std::uint64_t, 稳定编码> 场景事实;
  场景直接包含扩展结构交付 场景包含布局;
  稳定编码 存在组成类型{};

  夹具() {
    运行包 = std::make_unique<L1事实基座运行包>(建立L1事实基座运行包());
    auto &服务 = 运行包->读取服务();
    auto &签发器 = 运行包->所有者范围签发器();
    特征定义所有者 = 建立所有者(签发器, 0xD101);
    特征信息所有者 = 建立所有者(签发器, 0xD102);
    存在所有者 = 建立所有者(签发器, 0xD103);
    场景所有者 = 建立所有者(签发器, 0xD104);
    状态所有者 = 建立所有者(签发器, 0xD105);
    异主所有者 = 建立所有者(签发器, 0xD106);

    const auto 生产者 = 建立元数据(
        *特征定义所有者.写入端口, 服务, 0xD111,
        {{节点种类::普通, std::nullopt}, {节点种类::普通, std::nullopt}});
    std::vector<节点规格> 定义结构(16, {节点种类::普通, std::nullopt});
    for (const auto i : {2, 9, 13})
      定义结构[i] = {节点种类::属性类型, L1所有者范围值表示种类::U64组};
    for (const auto i : {3, 8})
      定义结构[i] = {节点种类::属性类型, L1所有者范围值表示种类::I64};
    定义结构[14] = {节点种类::属性类型, L1所有者范围值表示种类::I64组};
    (void)建立元数据(*特征定义所有者.写入端口, 服务, 1, 定义结构);
    (void)建立元数据(
        *特征信息所有者.写入端口, 服务, 1,
        {{节点种类::普通, std::nullopt}, {节点种类::普通, std::nullopt},
         {节点种类::属性类型, L1所有者范围值表示种类::I64},
         {节点种类::属性类型, L1所有者范围值表示种类::I64},
         {节点种类::普通, std::nullopt}});
    特征值 = std::make_unique<特征值类数据服务>(服务);
    特征 = std::make_unique<特征类数据服务>(
        服务, std::move(*特征定义所有者.写入端口),
        std::move(*特征信息所有者.写入端口), *特征值, 生产者.front());

    const auto 存在布局 = 建立元数据(
        *存在所有者.写入端口, 服务, 0xD121,
        {{节点种类::普通, std::nullopt}, {节点种类::普通, std::nullopt},
         {节点种类::普通, std::nullopt}});
    存在组成类型 = 存在布局[0];
    const auto 存在族 = 建立元数据(
        *存在所有者.写入端口, 服务, 0x455849535446414DULL,
        {{节点种类::普通, std::nullopt}, {节点种类::普通, std::nullopt}});
    const 存在单例角色结构登记请求 角色请求{1, 当前代次(服务), {0x1202}};
    const auto 角色 = 存在类数据服务::登记单例角色结构(
        服务, *存在所有者.写入端口, 角色请求);
    检查(角色.成功(角色请求) && 角色.交付, "existence-role-layout");
    const 实例特征结构登记请求 实例特征请求{
        2, 当前代次(服务), {0x4946525354525632ULL}};
    const auto 实例特征 = 存在类数据服务::登记实例特征结构(
        服务, *存在所有者.写入端口, 实例特征请求);
    检查(实例特征.成功(实例特征请求) && 实例特征.交付,
         "instance-feature-layout");

    L1所有者范围写集请求 存在写集;
    for (std::uint32_t i = 1; i <= 18; ++i)
      存在写集.节点.push_back({{i}, 节点种类::普通, std::nullopt});
    for (std::uint32_t i = 1; i <= 18; ++i)
      存在写集.关系.push_back(
          {{100 + i}, L1所有者范围写集本地键{i}, 存在族[0], 存在族[1], 1});
    const auto 加组成 = [&](std::uint32_t 键, std::uint32_t 父,
                            std::uint32_t 子, std::int64_t 角色值 = 1,
                            std::optional<稳定编码> 类型 = std::nullopt) {
      存在写集.关系.push_back(
          {{键}, L1所有者范围写集本地键{父}, L1所有者范围写集本地键{子},
           类型.value_or(存在布局[0]), 角色值});
    };
    加组成(201, 1, 2);
    加组成(202, 2, 3);
    加组成(203, 1, 5);
    加组成(204, 2, 5);
    加组成(205, 18, 8, 2);
    加组成(206, 9, 9);
    加组成(207, 1, 11);
    加组成(208, 1, 12);
    加组成(209, 15, 16);
    加组成(210, 1, 4, 1, 存在布局[1]);
    const auto 存在映射 = 提交写集(*存在所有者.写入端口, 服务, 0xD130,
                                   std::move(存在写集));
    E.resize(18);
    for (std::uint32_t i = 1; i <= 18; ++i)
      E[i - 1] = 存在映射.at(i);
    退出事实(*存在所有者.写入端口, 服务, 0xD131,
             {存在映射.at(106), E[5]});

    const auto 状态布局 = 建立元数据(
        *状态所有者.写入端口, 服务, 0xD141,
        {{节点种类::普通, std::nullopt}, {节点种类::普通, std::nullopt},
         {节点种类::普通, std::nullopt},
         {节点种类::属性类型, L1所有者范围值表示种类::I64},
         {节点种类::属性类型, L1所有者范围值表示种类::U64组},
         {节点种类::属性类型, L1所有者范围值表示种类::I64},
         {节点种类::属性类型, L1所有者范围值表示种类::I64},
         {节点种类::属性类型, L1所有者范围值表示种类::I64}});
    状态 = std::make_unique<状态类数据服务>(
        服务, *特征, std::move(*状态所有者.写入端口),
        状态类结构交付{状态布局[0], 状态布局[1], 状态布局[2], 状态布局[3],
                       状态布局[4], 状态布局[5], 状态布局[6], 状态布局[7]});

    场景结构登记请求 场景请求{2, 当前代次(服务), {1}};
    const auto 场景结构 = 场景类数据服务::登记结构类型(
        服务, *场景所有者.写入端口, 场景请求);
    场景特征组织扩展登记请求 特征组织请求{1, 当前代次(服务)};
    const auto 特征组织 = 场景类数据服务::登记特征组织扩展(
        服务, *场景所有者.写入端口, 特征组织请求);
    场景直接包含扩展登记请求 包含请求{1, 当前代次(服务)};
    const auto 包含 = 场景类数据服务::登记直接包含扩展(
        服务, *场景所有者.写入端口, 包含请求);
    检查(场景结构.成功(场景请求) && 场景结构.交付 &&
             特征组织.成功(特征组织请求) && 特征组织.交付 &&
             包含.成功(包含请求) && 包含.交付,
         "scene-layouts");
    场景包含布局 = *包含.交付;

    L1所有者范围写集请求 场景写集;
    constexpr std::uint32_t 场景节点数量 = 4116;
    for (std::uint32_t i = 1; i <= 场景节点数量; ++i)
      场景写集.节点.push_back({{i}, 节点种类::普通, std::nullopt});
    const auto 加场景边 = [&](std::uint32_t 键, auto 父, auto 子, 稳定编码 类型,
                              std::int64_t 角色值 = 1) {
      场景写集.关系.push_back({{键}, 父, 子, 类型, 角色值});
    };
    加场景边(5001, L1所有者范围写集本地键{1}, L1所有者范围写集本地键{2},
             场景包含布局.直接存在成员关系类型);
    加场景边(5002, L1所有者范围写集本地键{2}, L1所有者范围写集本地键{3},
             场景包含布局.直接子场景关系类型);
    加场景边(5003, L1所有者范围写集本地键{1}, L1所有者范围写集本地键{5},
             场景包含布局.直接存在成员关系类型);
    加场景边(5004, L1所有者范围写集本地键{2}, L1所有者范围写集本地键{5},
             场景包含布局.直接子场景关系类型);
    加场景边(5005, L1所有者范围写集本地键{1}, L1所有者范围写集本地键{6},
             场景包含布局.直接存在成员关系类型, 2);
    加场景边(5006, L1所有者范围写集本地键{7}, L1所有者范围写集本地键{7},
             场景包含布局.直接存在成员关系类型);
    加场景边(5007, L1所有者范围写集本地键{1}, L1所有者范围写集本地键{11},
             场景包含布局.直接存在成员关系类型);
    加场景边(5008, L1所有者范围写集本地键{1}, L1所有者范围写集本地键{11},
             场景包含布局.直接子场景关系类型);
    for (std::uint32_t i = 0; i < 4097; ++i)
      加场景边(6000 + i, L1所有者范围写集本地键{10},
               L1所有者范围写集本地键{20 + i},
               场景包含布局.直接存在成员关系类型);
    加场景边(11001, E[13], E[9], 场景包含布局.直接存在成员关系类型);
    加场景边(11002, E[13], E[10], 场景包含布局.直接子场景关系类型);
    加场景边(11003, E[0], E[12], 场景包含布局.直接存在成员关系类型);
    加场景边(11004, E[0], E[11], 场景包含布局.直接子场景关系类型);
    加场景边(11005, E[14], E[16], 场景包含布局.直接存在成员关系类型);
    场景事实 = 提交写集(*场景所有者.写入端口, 服务, 0xD150,
                         std::move(场景写集));
    退出事实(*场景所有者.写入端口, 服务, 0xD151, {场景事实.at(9)});

    L1所有者范围写集请求 异主写集;
    异主写集.关系.push_back(
        {{1}, E[17], E[6], 存在组成类型, 1});
    异主写集.关系.push_back(
        {{2}, 场景事实.at(1), 场景事实.at(8),
         场景包含布局.直接存在成员关系类型, 1});
    异主写集.合同版本 = L1所有者范围CRUD合同版本;
    异主写集.期望事实代次 = 当前代次(服务);
    异主写集.写入幂等身份 = {0xD160};
    const auto 异主写入 =
        异主所有者.写入端口->提交所有者范围中性写集(异主写集);
    检查(异主写入.状态 != L1所有者范围写入状态::成功 &&
             !异主写入.是否形成内存权威发布,
         "l1-rejects-cross-owner-relationship-shapes");

    存在 = std::make_unique<存在类数据服务>(
        服务, *特征, std::move(*存在所有者.写入端口), 存在布局[0], 存在布局[1],
        存在当前采用结构交付{存在布局[2]}, *实例特征.交付, *角色.交付);
    场景 = std::make_unique<场景类数据服务>(
        服务, std::move(*场景所有者.写入端口), *场景结构.交付, *存在, *状态,
        *特征组织.交付, *包含.交付);
    联合 = std::make_unique<直接归属联合只读组合器>(*存在, *场景);
  }

  std::uint64_t G() const { return 当前代次(运行包->读取服务()); }
};

void 验证请求与状态(夹具 &x) {
  const auto 存在版本拒绝 = x.存在->读取当前组成父_v2({1, x.G(), x.E[1]});
  检查(存在版本拒绝.状态 == 存在组成当前完整读取状态_v2::入口拒绝 &&
             !存在版本拒绝.父 && 存在版本拒绝.子组.empty() &&
             !存在版本拒绝.父读取成功({1, x.G(), x.E[1]}),
       "existence-v2-version-rejected");
  const auto 存在零代次 = x.存在->读取当前组成父_v2({2, 0, x.E[1]});
  检查(存在零代次.状态 == 存在组成当前完整读取状态_v2::入口拒绝 &&
             !存在零代次.父 && 存在零代次.子组.empty(),
       "existence-zero-generation-rejected");
  const auto 场景零端点 = x.场景->读取当前场景包含子组_v2({2, x.G(), {}});
  检查(场景零端点.状态 == 场景直接包含当前完整读取状态_v2::入口拒绝 &&
             场景零端点.包含组.empty(),
       "scene-zero-endpoint-rejected");
  const auto 联合未找到 =
      x.联合->读取当前联合父_v2({2, x.G(), {0xFFFF'FFFF'FFFF'FF01ULL}});
  检查(联合未找到.状态 == 直接归属联合当前完整读取状态_v2::未找到 &&
             !联合未找到.父 && 联合未找到.子组.empty(),
       "union-missing-endpoint");
  const auto 存在已退出 = x.存在->读取当前组成父_v2({2, x.G(), x.E[5]});
  检查(存在已退出.状态 == 存在组成当前完整读取状态_v2::目标已退出 &&
             !存在已退出.父 && 存在已退出.子组.empty(),
       "existence-exited-endpoint");
  const auto 场景已退出 =
      x.场景->读取当前场景包含父组_v2({2, x.G(), x.场景事实.at(9)});
  检查(场景已退出.状态 == 场景直接包含当前完整读取状态_v2::目标已退出 &&
             场景已退出.包含组.empty(),
       "scene-exited-endpoint");
  const auto 联合漂移 = x.联合->读取当前联合父_v2({2, x.G() - 1, x.E[1]});
  检查(联合漂移.状态 == 直接归属联合当前完整读取状态_v2::事实代次漂移 &&
             !联合漂移.父 && 联合漂移.子组.empty(),
       "union-generation-drift");
}

void 验证组成方向与结构(夹具 &x) {
  const auto g = x.G();
  const 存在组成父当前完整读取请求_v2 父请求{2, g, x.E[1]};
  const auto 父 = x.存在->读取当前组成父_v2(父请求);
  检查(父.父读取成功(父请求) && 父.父 && 父.父->父存在 == x.E[0],
       "existence-parent-ignores-legal-outgoing");
  const 存在组成子组当前完整读取请求_v2 子请求{2, g, x.E[1]};
  const auto 子 = x.存在->读取当前组成子组_v2(子请求);
  检查(子.子组读取成功(子请求) && 子.子组.size() == 2,
       "existence-children-ignore-legal-incoming");
  const 存在组成父当前完整读取请求_v2 仅反向父请求{2, g, x.E[0]};
  const auto 仅反向父 = x.存在->读取当前组成父_v2(仅反向父请求);
  检查(仅反向父.父读取成功(仅反向父请求) && !仅反向父.父,
       "existence-parent-only-reverse-edge-is-empty");
  const 存在组成子组当前完整读取请求_v2 仅反向子请求{2, g, x.E[2]};
  const auto 仅反向子 = x.存在->读取当前组成子组_v2(仅反向子请求);
  检查(仅反向子.子组读取成功(仅反向子请求) && 仅反向子.子组.empty(),
       "existence-children-only-reverse-edge-is-empty");
  const 存在组成父当前完整读取请求_v2 空父请求{2, g, x.E[3]};
  检查(x.存在->读取当前组成父_v2(空父请求).父读取成功(空父请求),
       "existence-wrong-type-ignored");
  检查(x.存在->读取当前组成父_v2({2, g, x.E[4]}).状态 ==
             存在组成当前完整读取状态_v2::内部不一致,
       "existence-multiple-parents-conflict");
  const auto 错角色父 = x.存在->读取当前组成父_v2({2, g, x.E[7]});
  检查(错角色父.状态 == 存在组成当前完整读取状态_v2::内部不一致 &&
             !错角色父.父 && 错角色父.子组.empty(),
       "existence-selected-wrong-role");
  const auto 自环父 = x.存在->读取当前组成父_v2({2, g, x.E[8]});
  检查(自环父.状态 == 存在组成当前完整读取状态_v2::内部不一致 &&
             !自环父.父 && 自环父.子组.empty(),
       "existence-self-loop-conflict");
  const auto 错角色子 = x.存在->读取当前组成子组_v2({2, g, x.E[17]});
  检查(错角色子.状态 == 存在组成当前完整读取状态_v2::内部不一致 &&
             !错角色子.父 && 错角色子.子组.empty(),
       "existence-child-selected-wrong-role");
  const auto 自环子 = x.存在->读取当前组成子组_v2({2, g, x.E[8]});
  检查(自环子.状态 == 存在组成当前完整读取状态_v2::内部不一致 &&
             !自环子.父 && 自环子.子组.empty(),
       "existence-child-self-loop-conflict");
}

void 验证场景方向与完整组(夹具 &x) {
  const auto g = x.G();
  const 场景直接包含父组当前完整读取请求_v2 父请求{2, g, x.场景事实.at(2)};
  const auto 父 = x.场景->读取当前场景包含父组_v2(父请求);
  检查(父.父组读取成功(父请求) && 父.包含组.size() == 1 &&
             父.包含组.front().父场景 == x.场景事实.at(1),
       "scene-parent-ignores-legal-outgoing");
  const 场景直接包含子组当前完整读取请求_v2 子请求{2, g, x.场景事实.at(2)};
  const auto 子 = x.场景->读取当前场景包含子组_v2(子请求);
  检查(子.子组读取成功(子请求) && 子.包含组.size() == 2,
       "scene-children-ignore-legal-incoming");
  const 场景直接包含父组当前完整读取请求_v2 仅反向父请求{
      2, g, x.场景事实.at(1)};
  const auto 仅反向父 = x.场景->读取当前场景包含父组_v2(仅反向父请求);
  检查(仅反向父.父组读取成功(仅反向父请求) &&
             仅反向父.包含组.empty(),
       "scene-parent-only-reverse-edge-is-empty");
  const 场景直接包含子组当前完整读取请求_v2 仅反向子请求{
      2, g, x.场景事实.at(3)};
  const auto 仅反向子 = x.场景->读取当前场景包含子组_v2(仅反向子请求);
  检查(仅反向子.子组读取成功(仅反向子请求) &&
             仅反向子.包含组.empty(),
       "scene-children-only-reverse-edge-is-empty");
  const 场景直接包含父组当前完整读取请求_v2 空请求{2, g, x.场景事实.at(4)};
  检查(x.场景->读取当前场景包含父组_v2(空请求).父组读取成功(空请求),
       "scene-legal-empty-parent");
  const 场景直接包含父组当前完整读取请求_v2 类型端点{
      2, g, x.场景包含布局.直接存在成员关系类型};
  检查(x.场景->读取当前场景包含父组_v2(类型端点).父组读取成功(类型端点),
       "scene-type-node-reference-ignored");
  检查(x.场景->读取当前场景包含父组_v2({2, g, x.场景事实.at(5)}).状态 ==
             场景直接包含当前完整读取状态_v2::内部不一致,
       "scene-multiple-parents-conflict");
  const auto 错角色父 =
      x.场景->读取当前场景包含父组_v2({2, g, x.场景事实.at(6)});
  检查(错角色父.状态 == 场景直接包含当前完整读取状态_v2::内部不一致 &&
             错角色父.包含组.empty(),
       "scene-selected-wrong-role");
  const auto 自环父 =
      x.场景->读取当前场景包含父组_v2({2, g, x.场景事实.at(7)});
  检查(自环父.状态 == 场景直接包含当前完整读取状态_v2::内部不一致 &&
             自环父.包含组.empty(),
       "scene-self-loop-conflict");
  const auto 错角色子 =
      x.场景->读取当前场景包含子组_v2({2, g, x.场景事实.at(1)});
  检查(错角色子.状态 == 场景直接包含当前完整读取状态_v2::内部不一致 &&
             错角色子.包含组.empty(),
       "scene-child-selected-wrong-role");
  const auto 自环子 =
      x.场景->读取当前场景包含子组_v2({2, g, x.场景事实.at(7)});
  检查(自环子.状态 == 场景直接包含当前完整读取状态_v2::内部不一致 &&
             自环子.包含组.empty(),
       "scene-child-self-loop-conflict");
  const 场景直接包含子组当前完整读取请求_v2 大组请求{2, g, x.场景事实.at(10)};
  const auto 大组 = x.场景->读取当前场景包含子组_v2(大组请求);
  检查(大组.子组读取成功(大组请求) && 大组.包含组.size() == 4097,
       "scene-more-than-4096-complete");
}

void 验证联合(夹具 &x) {
  const auto g = x.G();
  const 直接归属联合父当前完整读取请求_v2 组成父请求{2, g, x.E[1]};
  const auto 组成父 = x.联合->读取当前联合父_v2(组成父请求);
  检查(组成父.父读取成功(组成父请求) && 组成父.父 &&
             组成父.父->来源 == 直接归属来源::存在组成,
       "union-existence-parent");
  const 直接归属联合父当前完整读取请求_v2 场景父请求{2, g, x.E[9]};
  const auto 场景父 = x.联合->读取当前联合父_v2(场景父请求);
  检查(场景父.父读取成功(场景父请求) && 场景父.父 &&
             场景父.父->来源 == 直接归属来源::场景成员,
       "union-scene-parent");
  const 直接归属联合父当前完整读取请求_v2 空父请求{2, g, x.E[3]};
  const auto 空父 = x.联合->读取当前联合父_v2(空父请求);
  检查(空父.父读取成功(空父请求) && !空父.父,
       "union-empty-parent");
  检查(x.联合->读取当前联合父_v2({2, g, x.E[10]}).状态 ==
             直接归属联合当前完整读取状态_v2::内部不一致,
       "union-cross-family-parent-conflict");
  const 直接归属联合子组当前完整读取请求_v2 合并请求{2, g, x.E[14]};
  const auto 合并 = x.联合->读取当前联合子组_v2(合并请求);
  检查(合并.子组读取成功(合并请求) && 合并.子组.size() == 2 &&
             合并.子组[0].成员.值 < 合并.子组[1].成员.值,
       "union-cross-family-complete-sorted-children");
  检查(x.联合->读取当前联合子组_v2({2, g, x.E[0]}).状态 ==
             直接归属联合当前完整读取状态_v2::内部不一致,
       "union-duplicate-member-conflict");
}

void 验证谓词零载荷与自环防御(夹具 &x) {
  存在组成当前完整读取结果_v2 组成;
  组成.状态 = 存在组成当前完整读取状态_v2::资源失败;
  组成.Gread = 9;
  组成.父 = 存在组成关系事实{};
  检查(!组成.父读取成功({2, 9, {1}}),
       "existence-non-success-payload-rejected");
  场景直接包含当前完整读取结果_v2 场景;
  场景.状态 = 场景直接包含当前完整读取状态_v2::资源失败;
  场景.Gread = 9;
  场景.包含组.push_back({});
  检查(!场景.子组读取成功({2, 9, {1}}),
       "scene-non-success-payload-rejected");
  直接归属联合当前完整读取结果_v2 联合;
  联合.状态 = 直接归属联合当前完整读取状态_v2::资源失败;
  联合.Gread = 9;
  联合.子组.push_back({});
  检查(!联合.子组读取成功({2, 9, {1}}),
       "union-non-success-payload-rejected");

  const auto g = x.G();
  const 存在组成父当前完整读取请求_v2 存在父请求{2, g, x.E[1]};
  auto 存在父 = x.存在->读取当前组成父_v2(存在父请求);
  检查(存在父.父读取成功(存在父请求) && 存在父.父,
       "existence-parent-predicate-baseline");
  存在父.父->父存在 = 存在父.父->子存在;
  检查(!存在父.父读取成功(存在父请求),
       "existence-parent-predicate-rejects-self-loop");

  const 存在组成子组当前完整读取请求_v2 存在子请求{2, g, x.E[1]};
  auto 存在子 = x.存在->读取当前组成子组_v2(存在子请求);
  检查(存在子.子组读取成功(存在子请求) && !存在子.子组.empty(),
       "existence-children-predicate-baseline");
  存在子.子组.front().子存在 = 存在子.子组.front().父存在;
  检查(!存在子.子组读取成功(存在子请求),
       "existence-children-predicate-rejects-self-loop");

  const 场景直接包含父组当前完整读取请求_v2 场景父请求{
      2, g, x.场景事实.at(2)};
  auto 场景父 = x.场景->读取当前场景包含父组_v2(场景父请求);
  检查(场景父.父组读取成功(场景父请求) && !场景父.包含组.empty(),
       "scene-parent-predicate-baseline");
  场景父.包含组.front().父场景 = 场景父.包含组.front().成员;
  场景父.包含组.front().关系.源 = 场景父.包含组.front().成员;
  检查(!场景父.父组读取成功(场景父请求),
       "scene-parent-predicate-rejects-self-loop");

  const 场景直接包含子组当前完整读取请求_v2 场景子请求{
      2, g, x.场景事实.at(2)};
  auto 场景子 = x.场景->读取当前场景包含子组_v2(场景子请求);
  检查(场景子.子组读取成功(场景子请求) && !场景子.包含组.empty(),
       "scene-children-predicate-baseline");
  场景子.包含组.front().成员 = 场景子.包含组.front().父场景;
  场景子.包含组.front().关系.目标 = 场景子.包含组.front().父场景;
  检查(!场景子.子组读取成功(场景子请求),
       "scene-children-predicate-rejects-self-loop");

  const 直接归属联合父当前完整读取请求_v2 联合父请求{2, g, x.E[1]};
  auto 联合父 = x.联合->读取当前联合父_v2(联合父请求);
  检查(联合父.父读取成功(联合父请求) && 联合父.父,
       "union-parent-predicate-baseline");
  联合父.父->父 = 联合父.父->成员;
  检查(!联合父.父读取成功(联合父请求),
       "union-parent-predicate-rejects-self-loop");

  const 直接归属联合子组当前完整读取请求_v2 联合子请求{2, g, x.E[14]};
  auto 联合子 = x.联合->读取当前联合子组_v2(联合子请求);
  检查(联合子.子组读取成功(联合子请求) && !联合子.子组.empty(),
       "union-children-predicate-baseline");
  联合子.子组.front().成员 = 联合子.子组.front().父;
  检查(!联合子.子组读取成功(联合子请求),
       "union-children-predicate-rejects-self-loop");
}
} // namespace

int main() {
  try {
    std::cout << std::unitbuf;
    夹具 数据;
    验证请求与状态(数据);
    验证组成方向与结构(数据);
    验证场景方向与完整组(数据);
    验证联合(数据);
    验证谓词零载荷与自环防御(数据);
    std::cout << "PASS total=" << 通过数量 << '\n';
    return 0;
  } catch (const std::exception &错误) {
    std::cerr << "FAIL " << 错误.what() << '\n';
    return 1;
  } catch (...) {
    std::cerr << "FAIL unexpected exception\n";
    return 2;
  }
}
