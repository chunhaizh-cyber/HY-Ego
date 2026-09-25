#include "装配.普通应用.h"

#include "业务/应用服务.世界树类.h"
#include "业务/应用服务.自我形成.h"
#include "业务/应用服务.自我线程正式上下文.h"
#include "业务/应用服务.特征概念类.h"
#include "业务/应用服务.场景成员概念类.h"
#include "业务/初始化.二次关系概念结构.h"
#include "核心/日志系统.h"

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <algorithm>
#include <climits>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <mutex>
#include <new>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "领域/数据服务.世界树根.h"
#include "领域/数据服务.特征值域比较类.h"
#include "领域/数据服务.需求类.h"
#include "领域/任务治理.本能根任务核心.h"

namespace 海中鱼巣 {
namespace 普通应用装配内部 {

struct 概念结构异常 final { 纯概念状态 原因; };
struct 角色结构异常 final { 存在单例角色状态 原因; };
struct 本能先天特征概念异常 final { 本能先天特征概念初始化状态 原因; };
struct 本能双根二次关系概念异常 final {
  本能双根二次关系概念初始化状态 原因;
};
struct 持久恢复异常 final { L1事实基座持久恢复结果_v1 结果; };

const wchar_t* 实例特征结构异常文本(实例特征结构异常 reason) noexcept {
  switch (reason) {
  case 实例特征结构异常::半结构: return L"半结构";
  case 实例特征结构异常::空R项: return L"空R项";
  case 实例特征结构异常::缺材料: return L"缺材料";
  case 实例特征结构异常::重复R项: return L"重复R项";
  case 实例特征结构异常::多重命中: return L"多重命中";
  }
  return L"未知结构异常";
}

void 报告实例特征结构异常(存在信息身份 e, 特征类型身份 ft,
                        实例特征结构异常 reason, std::uint64_t gread,
                        std::uint64_t g0) noexcept {
  constexpr auto module = L"普通应用装配";
  constexpr auto entry = L"实例特征结构诊断";
  constexpr auto title = L"海中鱼巣实例特征结构异常";
  constexpr auto fallback = L"实例特征结构诊断未能构造完整内容。";
  try {
    const std::wstring content =
        std::wstring{L"实例特征结构异常："} + 实例特征结构异常文本(reason) +
        L"\nE=" + std::to_wstring(e.编码.值) +
        L"\nFT=" + std::to_wstring(ft.编码.值) +
        L"\nGread=" + std::to_wstring(gread) +
        L"\nG0=" + std::to_wstring(g0);
    try { (void)记录逻辑错误日志(module, entry, content); } catch (...) {}
    try { (void)MessageBoxW(nullptr, content.c_str(), title, MB_OK | MB_ICONERROR); }
    catch (...) {}
  } catch (...) {
    try { (void)记录逻辑错误日志(module, entry, fallback); } catch (...) {}
    try { (void)MessageBoxW(nullptr, fallback, title, MB_OK | MB_ICONERROR); }
    catch (...) {}
  }
}

// 装配结果仍以既有纯概念状态承载失败原因；两组定义状态只在此失败边界做归类。
// 成功状态不会进入此函数。
纯概念状态 映射两组定义失败(存在概念两组状态_v3 state) noexcept {
  switch (state) {
  case 存在概念两组状态_v3::入口拒绝: return 纯概念状态::入口拒绝;
  case 存在概念两组状态_v3::目标已退出: return 纯概念状态::目标已退出;
  case 存在概念两组状态_v3::概念已退役: return 纯概念状态::概念已退役;
  case 存在概念两组状态_v3::类别冲突: return 纯概念状态::类别冲突;
  case 存在概念两组状态_v3::定义不相容: return 纯概念状态::定义不相容;
  case 存在概念两组状态_v3::规则缺失: return 纯概念状态::定义不支持;
  case 存在概念两组状态_v3::事实代次漂移: return 纯概念状态::事实代次漂移;
  case 存在概念两组状态_v3::幂等冲突: return 纯概念状态::幂等冲突;
  case 存在概念两组状态_v3::数量预算不足: return 纯概念状态::数量预算不足;
  case 存在概念两组状态_v3::历史材料不可用: return 纯概念状态::历史材料不可用;
  case 存在概念两组状态_v3::资源失败: return 纯概念状态::资源失败;
  case 存在概念两组状态_v3::已可能发布: return 纯概念状态::已可能发布;
  case 存在概念两组状态_v3::旧格式不支持: return 纯概念状态::旧格式不支持;
  default: return 纯概念状态::内部不一致;
  }
}

纯概念状态 映射二次关系结构失败(二次关系数据状态 state) noexcept {
  switch (state) {
  case 二次关系数据状态::入口拒绝: return 纯概念状态::入口拒绝;
  case 二次关系数据状态::目标已退出: return 纯概念状态::目标已退出;
  case 二次关系数据状态::引用冲突: return 纯概念状态::引用冲突;
  case 二次关系数据状态::事实代次漂移: return 纯概念状态::事实代次漂移;
  case 二次关系数据状态::幂等冲突: return 纯概念状态::幂等冲突;
  case 二次关系数据状态::旧预算不足: return 纯概念状态::旧格式不支持;
  case 二次关系数据状态::历史材料不可用: return 纯概念状态::历史材料不可用;
  case 二次关系数据状态::资源失败: return 纯概念状态::资源失败;
  case 二次关系数据状态::发布未知: return 纯概念状态::已可能发布;
  case 二次关系数据状态::格式不支持: return 纯概念状态::旧格式不支持;
  case 二次关系数据状态::规则不支持: return 纯概念状态::定义不支持;
  default: return 纯概念状态::内部不一致;
  }
}

using 元节点规格 =
    std::pair<节点种类, std::optional<L1所有者范围值表示种类>>;

std::uint64_t 当前代次(const L1事实基座服务 &l1) {
  const auto result =
      l1.读取中性当前事实代次({L1中性CRUD合同版本});
  if (result.状态 != L1中性读取状态::成功 || !result.事实代次)
    throw 普通应用装配状态::内部不一致;
  return result.事实代次;
}

L1所有者范围交付 建立所有者(L1所有者范围签发器 &issuer,
                              const L1事实基座服务&l1,std::uint64_t key) {
  const L1所有者范围建立请求 request{
      L1所有者范围CRUD合同版本,{key},L1所有者范围种类::独占结构范围};
  auto result=issuer.建立所有者范围(request);
  const auto state=result.建立结果.状态;
  if ((state!=L1所有者范围管理状态::成功&&
       state!=L1所有者范围管理状态::精确重复)||
      result.建立结果.合同版本!=request.合同版本||
      result.建立结果.建立幂等身份!=request.建立幂等身份||
      !result.建立结果.所有者事实||
      result.建立结果.所有者事实->范围种类!=L1所有者范围种类::独占结构范围||
      result.建立结果.所有者事实->退出事实代次||
      (state==L1所有者范围管理状态::成功)!=
          result.建立结果.是否形成内存权威发布||
      !result.写入端口||!result.写入端口->有效()||
      !result.写入端口->绑定于(l1)||
      result.写入端口->所有者身份()!=result.建立结果.所有者事实->所有者)
    throw 普通应用装配状态::所有者建立失败;
  const auto current=l1.读取当前结构所有者(
      {L1所有者范围CRUD合同版本,result.写入端口->所有者身份()});
  if(current.状态!=L1所有者范围读取状态::成功||!current.所有者事实||
     *current.所有者事实!=*result.建立结果.所有者事实)
    throw 普通应用装配状态::所有者建立失败;
  return result;
}

struct 首次定位 final {
  bool 已有=false;std::uint64_t G0=0,H=0,Gread=0;
  std::optional<L1所有者范围首次写入读取结果> 材料;
};

首次定位 定位首次(L1所有者范围写端口&port,const L1事实基座服务&l1,
                    std::uint64_t key) {
  const auto before=当前代次(l1);
  auto first=port.读取首次写入材料(
      {L1所有者范围首次写入读取合同版本,{key}});
  const auto after=当前代次(l1);
  if(before!=after||first.合同版本!=L1所有者范围首次写入读取合同版本||
     first.所有者!=port.所有者身份()||first.写入幂等身份.值!=key||
     first.读取事实代次!=after)
    throw 普通应用装配状态::元结构建立失败;
  if(first.状态==L1所有者范围读取状态::未找到) {
    if(first.首次规范化写集||first.首次写入结果)
      throw 普通应用装配状态::元结构建立失败;
    return {false,after,0,after,std::move(first)};
  }
  if(first.状态!=L1所有者范围读取状态::成功||
     !first.首次规范化写集||!first.首次写入结果) {
    if(first.状态==L1所有者范围读取状态::资源失败)
      throw 普通应用装配状态::资源失败;
    throw 普通应用装配状态::元结构建立失败;
  }
  const auto&ws=*first.首次规范化写集;const auto&saved=*first.首次写入结果;
  if(ws.合同版本!=L1所有者范围CRUD合同版本||ws.写入幂等身份.值!=key||
     !ws.期望事实代次||saved.合同版本!=L1所有者范围CRUD合同版本||
     saved.状态!=L1所有者范围写入状态::成功||
     saved.所有者!=port.所有者身份()||saved.写入幂等身份.值!=key||
     !saved.是否形成内存权威发布||
     saved.重试边界!=L1所有者范围重试边界::不适用||
     saved.事实代次!=ws.期望事实代次+1||saved.事实代次>after)
    throw 普通应用装配状态::元结构建立失败;
  std::vector<std::uint64_t> stable;
  for(const auto&[local,id]:saved.新编码映射) {
    if(!local.值||!有效(id))throw 普通应用装配状态::元结构建立失败;
    stable.push_back(id.值);
  }
  std::sort(stable.begin(),stable.end());
  if(std::adjacent_find(stable.begin(),stable.end())!=stable.end())
    throw 普通应用装配状态::元结构建立失败;
  return {true,ws.期望事实代次,saved.事实代次,after,std::move(first)};
}

std::vector<稳定编码>
建立并读回元节点(L1所有者范围写端口 &port,
                  const L1事实基座服务 &l1, std::uint64_t key,
                  const std::vector<元节点规格> &specs) {
  if (specs.empty())
    throw 普通应用装配状态::元结构建立失败;
  auto located=定位首次(port,l1,key);
  L1所有者范围写集请求 request{
      L1所有者范围CRUD合同版本,located.G0,{key}};
  request.节点.reserve(specs.size());
  for (std::uint32_t i = 0; i < specs.size(); ++i)
    request.节点.push_back(
        {{static_cast<std::uint64_t>(i) + 1}, specs[i].first,
         specs[i].second});

  const auto saved = port.提交所有者范围中性写集(request);
  auto expectedState=L1所有者范围写入状态::成功;
  if(located.已有)expectedState=L1所有者范围写入状态::精确重复;
  if (saved.状态 != expectedState ||
      saved.所有者 != port.所有者身份() ||
      saved.写入幂等身份 != request.写入幂等身份 ||
      saved.新编码映射.size() != specs.size() ||saved.事实代次!=located.G0+1||
      (saved.是否形成内存权威发布!=!located.已有))
    throw 普通应用装配状态::元结构建立失败;

  const auto first = port.读取首次写入材料(
      {L1所有者范围首次写入读取合同版本, request.写入幂等身份});
  if (first.状态 != L1所有者范围读取状态::成功 ||
      first.所有者 != port.所有者身份() ||
      first.写入幂等身份 != request.写入幂等身份 ||
      first.读取事实代次 != 当前代次(l1) ||
      !first.首次规范化写集 || !first.首次写入结果 ||
      *first.首次规范化写集 != request ||
      first.首次写入结果->状态 != L1所有者范围写入状态::成功 ||
      first.首次写入结果->事实代次 != located.G0+1 ||
      first.首次写入结果->新编码映射 != saved.新编码映射)
    throw 普通应用装配状态::元结构建立失败;

  std::vector<稳定编码> ids(specs.size());
  for (const auto &[local, id] : saved.新编码映射) {
    if (!local.值 || local.值 > ids.size() || !有效(id) ||
        有效(ids[local.值 - 1]))
      throw 普通应用装配状态::元结构建立失败;
    ids[local.值 - 1] = id;
  }
  const auto gread=当前代次(l1);
  for (std::size_t i = 0; i < ids.size(); ++i) {
    const auto read = l1.读取所有者范围当前节点(
        {L1所有者范围CRUD合同版本, ids[i]});
    const auto *node = read.事实
                           ? std::get_if<L1所有者范围节点事实>(&*read.事实)
                           : nullptr;
    if (read.状态 != L1所有者范围读取状态::成功 ||
        read.读取事实代次 != gread || !node ||
        node->编码 != ids[i] || node->种类 != specs[i].first ||
        node->属性类型表示 != specs[i].second ||
        node->写入所有者 != port.所有者身份() ||
        node->创建事实代次 != located.G0+1 || node->退出事实代次)
      throw 普通应用装配状态::元结构建立失败;
  }
  if(当前代次(l1)!=gread)throw 普通应用装配状态::元结构建立失败;
  return ids;
}

struct 普通应用上下文 final {
  std::unique_ptr<L1事实基座运行包> 运行包;
  L1事实基座持久恢复结果_v1 持久恢复;
  L1所有者范围交付 特征定义所有者;
  L1所有者范围交付 特征值所有者;
  L1所有者范围交付 特征信息所有者;
  L1所有者范围交付 存在所有者;
  L1所有者范围交付 场景所有者;
  L1所有者范围交付 状态所有者;
  L1所有者范围交付 概念所有者;
  L1所有者范围交付 需求所有者;
  L1所有者范围交付 本能根任务核心所有者;
  L1所有者范围交付 方法登记根初始化结构所有者;
  std::unique_ptr<特征值类数据服务> 特征值;
  std::unique_ptr<特征类数据服务> 特征;
  std::unique_ptr<存在类数据服务> 存在;
  std::unique_ptr<需求类数据服务> 需求;
  std::unique_ptr<本能根任务核心服务_v1> 本能根任务核心;
  std::unique_ptr<状态类数据服务> 状态;
  std::unique_ptr<场景类数据服务> 场景;
  std::unique_ptr<概念树类数据服务> 概念;
  本能先天特征概念初始化结果 本能先天特征概念初始化;
  本能双根二次关系概念初始化结果 本能双根二次关系概念初始化;
  std::unique_ptr<特征值域比较数据服务> 特征值域比较;
  std::unique_ptr<有序I64特征比较提供者> 二次关系I64比较;
  std::unique_ptr<二次关系求值应用服务> 二次关系求值;
  std::unique_ptr<自我根需求复核提供者> 自我根需求复核;
  std::unique_ptr<原子I64特征出生数据服务> 原子I64特征出生;
  std::unique_ptr<特征概念应用服务> 特征概念;
  std::unique_ptr<场景成员概念应用服务> 场景成员概念;
  std::unique_ptr<世界树根数据服务> 根服务;
  std::unique_ptr<世界树应用服务> 世界树;
  std::unique_ptr<真实自我形成服务> 自我;
  std::unique_ptr<L2方法结构聚合服务> 方法结构聚合;
  std::unique_ptr<方法登记根生产初始化提供者> 方法登记根初始化;
  存在单例角色结构交付 角色结构;
  std::optional<std::uint64_t> 根原G0;
  std::optional<本能根运行初始化结果_v1> 本能根运行初始化尝试;
  std::optional<本能根运行锚点_v1> 本能根运行锚点;
  std::optional<真实自我投影> 本能根运行自我投影;
  std::optional<稳定编码> 本能根运行根场景;
  std::unique_ptr<自我线程正式上下文提供者> 自我线程正式上下文;
  自我线程 自我线程对象;
};

std::mutex 上下文锁;
std::unique_ptr<普通应用上下文> 上下文;
std::optional<普通应用配置> 已选配置;

std::unique_ptr<普通应用上下文> 建立上下文(
    const 普通应用配置&config,
    std::optional<L1事实基座持久恢复结果_v1>&recovery) {
  auto result = std::make_unique<普通应用上下文>();
  auto persistent=建立L1事实基座持久运行包_v1(config.L1事实基座持久存储);
  recovery=persistent.恢复;
  result->持久恢复=persistent.恢复;
  if(!persistent.成功()||!persistent.运行包)
    throw 持久恢复异常{persistent.恢复};
  result->运行包=std::move(persistent.运行包);
  auto &l1 = result->运行包->读取服务();
  auto &issuer = result->运行包->所有者范围签发器();

  result->特征值所有者 = 建立所有者(issuer,l1,0x1000);
  result->特征定义所有者 = 建立所有者(issuer,l1,0x1001);
  result->特征信息所有者 = 建立所有者(issuer,l1,0x1002);
  result->存在所有者 = 建立所有者(issuer,l1,0x1003);
  result->场景所有者 = 建立所有者(issuer,l1,0x1004);
  result->状态所有者 = 建立所有者(issuer,l1,0x1005);
  result->概念所有者 = 建立所有者(issuer,l1,0x1006);
  result->需求所有者 = 建立所有者(issuer,l1,0x1007);
  result->本能根任务核心所有者 = 建立所有者(issuer,l1,0x1008);

  const auto producers = 建立并读回元节点(
      *result->特征定义所有者.写入端口, l1, 0x1101,
      {{节点种类::普通, std::nullopt},
       {节点种类::普通, std::nullopt}});

  std::vector<元节点规格> definition(
      16, {节点种类::普通, std::nullopt});
  for (auto i : {2, 9, 13})
    definition[i] =
        {节点种类::属性类型, L1所有者范围值表示种类::U64组};
  for (auto i : {3, 8})
    definition[i] =
        {节点种类::属性类型, L1所有者范围值表示种类::I64};
  definition[14] =
      {节点种类::属性类型, L1所有者范围值表示种类::I64组};
  (void)建立并读回元节点(*result->特征定义所有者.写入端口, l1, 1,
                           definition);

  (void)建立并读回元节点(
      *result->特征信息所有者.写入端口, l1, 1,
      {{节点种类::普通, std::nullopt},
       {节点种类::普通, std::nullopt},
       {节点种类::属性类型, L1所有者范围值表示种类::I64},
       {节点种类::属性类型, L1所有者范围值表示种类::I64},
       {节点种类::普通, std::nullopt}});

  const auto existenceLayout = 建立并读回元节点(
      *result->存在所有者.写入端口, l1, 0x1201,
      {{节点种类::普通, std::nullopt},
       {节点种类::普通, std::nullopt},
       {节点种类::普通, std::nullopt}});
  (void)建立并读回元节点(
      *result->存在所有者.写入端口, l1, 0x455849535446414DULL,
      {{节点种类::普通, std::nullopt},
       {节点种类::普通, std::nullopt}});

  const auto stateLayout = 建立并读回元节点(
      *result->状态所有者.写入端口, l1, 0x1301,
      {{节点种类::普通, std::nullopt},
       {节点种类::普通, std::nullopt},
       {节点种类::普通, std::nullopt},
       {节点种类::属性类型, L1所有者范围值表示种类::I64},
       {节点种类::属性类型, L1所有者范围值表示种类::U64组},
       {节点种类::属性类型, L1所有者范围值表示种类::I64},
       {节点种类::属性类型, L1所有者范围值表示种类::I64},
       {节点种类::属性类型, L1所有者范围值表示种类::I64}});

  const auto rootFirst=[&](L1所有者范围写端口&port,std::uint64_t key){
    try {return 定位首次(port,l1,key);}
    catch(普通应用装配状态 s) {
      if(s==普通应用装配状态::资源失败)throw;
      throw 普通应用装配状态::世界树根失败;
    }
  };
  const auto existenceRoot=rootFirst(*result->存在所有者.写入端口,0x2002);
  const auto sceneRoot=rootFirst(*result->场景所有者.写入端口,0x2003);
  if(existenceRoot.Gread!=sceneRoot.Gread||existenceRoot.已有!=sceneRoot.已有)
    throw 普通应用装配状态::世界树根失败;
  if(existenceRoot.已有) {
    if(existenceRoot.G0!=sceneRoot.G0||existenceRoot.H!=sceneRoot.H||
       !existenceRoot.材料||!sceneRoot.材料||
       existenceRoot.材料->首次写入结果->新编码映射.size()!=2||
       sceneRoot.材料->首次写入结果->新编码映射.size()!=10)
      throw 普通应用装配状态::世界树根失败;
    result->根原G0=existenceRoot.G0;
  }

  const 特征值U64组结构登记请求_B2 valueLayoutRequest{
      1, 定位首次(*result->特征值所有者.写入端口,l1,0x1010).G0,
      {0x1010}};
  const auto valueLayout = 特征值类数据服务::登记U64组结构_B2(
      l1, *result->特征值所有者.写入端口, valueLayoutRequest);
  if (!valueLayout.成功(valueLayoutRequest) || !valueLayout.交付)
    throw 普通应用装配状态::元结构建立失败;
  result->特征值 = std::make_unique<特征值类数据服务>(
      l1, std::move(*result->特征值所有者.写入端口), *valueLayout.交付);
  result->特征 = std::make_unique<特征类数据服务>(
      l1, std::move(*result->特征定义所有者.写入端口),
      std::move(*result->特征信息所有者.写入端口), *result->特征值,
      producers.front());
  if (!std::holds_alternative<std::monostate>(
          result->特征->初始化特征定义结构()) ||
      !std::holds_alternative<std::monostate>(
          result->特征->初始化准确特征结构()))
    throw 普通应用装配状态::元结构建立失败;
  const auto roleG0=定位首次(*result->存在所有者.写入端口,l1,0x1202).G0;
  const 存在单例角色结构登记请求 roleRequest{1,roleG0,{0x1202}};
  const auto role=存在类数据服务::登记单例角色结构(
      l1,*result->存在所有者.写入端口,roleRequest);
  if(!role.成功(roleRequest)||!role.交付)throw 角色结构异常{role.状态};
  result->角色结构=*role.交付;
  const auto instanceFeatureG0=当前代次(l1);
  const 实例特征结构登记请求 instanceFeatureRequest{
      2,instanceFeatureG0,{0x4946525354525632ULL}};
  const auto instanceFeature=存在类数据服务::登记实例特征结构(
      l1,*result->存在所有者.写入端口,instanceFeatureRequest);
  if(!instanceFeature.成功(instanceFeatureRequest)||!instanceFeature.交付)
    throw 普通应用装配状态::元结构建立失败;
  result->存在 = std::make_unique<存在类数据服务>(
      l1, *result->特征, std::move(*result->存在所有者.写入端口),
      existenceLayout[0], existenceLayout[1],
      存在当前采用结构交付{existenceLayout[2]},*instanceFeature.交付,result->角色结构);
  const 需求结构登记请求 demandRequest{需求结构登记合同版本,
      当前代次(l1)};
  const auto demandRegistration = 需求类数据服务::登记需求结构(
      l1, *result->需求所有者.写入端口, demandRequest);
  if (!demandRegistration.成功(demandRequest) || !demandRegistration.交付)
    throw 普通应用装配状态::服务建立失败;
  result->需求 = std::make_unique<需求类数据服务>(l1, *result->特征,
      *result->存在, std::move(*result->需求所有者.写入端口),
      *demandRegistration.交付);
  const 本能根任务核心结构登记请求_v1 taskOwnerRequest{
      1, 当前代次(l1), {0x544F000000000001ULL}};
  const auto taskOwnerRegistration = 本能根任务核心服务_v1::登记结构(
      l1, *result->本能根任务核心所有者.写入端口, taskOwnerRequest);
  if (!taskOwnerRegistration.成功(taskOwnerRequest) || !taskOwnerRegistration.交付)
    throw 普通应用装配状态::服务建立失败;
  result->本能根任务核心 = std::make_unique<本能根任务核心服务_v1>(
      l1, *result->需求, *result->存在, *result->特征,
      std::move(*result->本能根任务核心所有者.写入端口),
      *taskOwnerRegistration.交付, result->角色结构.项目角色);
  result->状态 = std::make_unique<状态类数据服务>(
      l1, *result->特征, std::move(*result->状态所有者.写入端口),
      状态类结构交付{stateLayout[0], stateLayout[1], stateLayout[2],
                     stateLayout[3], stateLayout[4], stateLayout[5],
                     stateLayout[6], stateLayout[7]});

  场景结构登记请求 baseRequest{2,
      定位首次(*result->场景所有者.写入端口,l1,1).G0,{1}};
  auto base = 场景类数据服务::登记结构类型(
      l1, *result->场景所有者.写入端口, baseRequest);
  if(!base.成功(baseRequest)||!base.交付)
    throw 普通应用装配状态::元结构建立失败;
  场景特征组织扩展登记请求 featureRequest{1,
      定位首次(*result->场景所有者.写入端口,l1,
        场景特征组织扩展登记固定幂等身份.值).G0};
  auto feature = 场景类数据服务::登记特征组织扩展(
      l1, *result->场景所有者.写入端口, featureRequest);
  if(!feature.成功(featureRequest)||!feature.交付)
    throw 普通应用装配状态::元结构建立失败;
  场景直接包含扩展登记请求 includeRequest{1,
      定位首次(*result->场景所有者.写入端口,l1,
        场景直接包含扩展登记固定幂等身份.值).G0};
  auto include = 场景类数据服务::登记直接包含扩展(
      l1, *result->场景所有者.写入端口, includeRequest);
  if(!include.成功(includeRequest)||!include.交付)
    throw 普通应用装配状态::元结构建立失败;

  result->场景 = std::make_unique<场景类数据服务>(
      l1, std::move(*result->场景所有者.写入端口), *base.交付,
      *result->存在, *result->状态, *feature.交付, *include.交付);
  const 纯概念结构登记请求_v2 conceptRequest{2,
      定位首次(*result->概念所有者.写入端口,l1,0x1401).G0,{0x1401},1048576};
  const auto conceptRegistration=概念树类数据服务::登记纯概念结构_v2(
      l1,*result->概念所有者.写入端口,conceptRequest);
  if(!conceptRegistration.成功(conceptRequest)||!conceptRegistration.交付)
    throw 概念结构异常{conceptRegistration.状态};
  const 特征概念出生使用结构登记请求 featureBirthRequest{
      1, 定位首次(*result->概念所有者.写入端口,l1,0x1402).G0, {0x1402},
      *conceptRegistration.交付, 4};
  const auto featureBirthRegistration=概念树类数据服务::登记特征概念出生使用结构(
      l1,*result->概念所有者.写入端口,featureBirthRequest);
  if(!featureBirthRegistration.成功(featureBirthRequest)||!featureBirthRegistration.交付)
    throw 概念结构异常{featureBirthRegistration.状态};
  const 存在概念两组结构登记请求_v1 completeDefinitionRequest{
      1, 定位首次(*result->概念所有者.写入端口,l1,0x1403).G0, {0x1403},
      *conceptRegistration.交付, 18};
  const auto completeDefinitionRegistration=概念树类数据服务::登记存在概念两组结构_v1(
      l1,*result->概念所有者.写入端口,completeDefinitionRequest);
  if(!completeDefinitionRegistration.成功(completeDefinitionRequest)||!completeDefinitionRegistration.交付)
    throw 概念结构异常{映射两组定义失败(completeDefinitionRegistration.状态)};
  const 二次关系初始化请求 secondaryRelationRequest{
      1, 定位首次(*result->概念所有者.写入端口,l1,0x1404).G0,
      {0x1404}, *conceptRegistration.交付, 53};
  const auto secondaryRelationRegistration=初始化二次关系结构(
      l1,*result->概念所有者.写入端口,secondaryRelationRequest);
  if(!secondaryRelationRegistration.成功()||!secondaryRelationRegistration.交付)
    throw 概念结构异常{映射二次关系结构失败(secondaryRelationRegistration.状态)};
  result->概念=std::make_unique<概念树类数据服务>(
      l1,*result->特征,*result->存在,*result->特征值,*result->场景,
      std::move(*result->概念所有者.写入端口),*conceptRegistration.交付,
      *featureBirthRegistration.交付,*completeDefinitionRegistration.交付,
      *secondaryRelationRegistration.交付,*result->场景);
  本能先天特征概念初始化提供者 instinctInitialization(*result->特征,*result->概念);
  result->本能先天特征概念初始化=instinctInitialization.初始化(
      {1,当前代次(l1),{64,256,0,0,0,64,0,0}});
  if(!result->本能先天特征概念初始化.成功())
    throw 本能先天特征概念异常{result->本能先天特征概念初始化.状态};
  本能双根二次关系概念初始化提供者 rootRelationInitialization(
      *result->特征, *result->概念);
  result->本能双根二次关系概念初始化 = rootRelationInitialization.初始化(
      {本能双根二次关系概念初始化合同版本,
       当前代次(l1), result->本能先天特征概念初始化});
  if(!result->本能双根二次关系概念初始化.成功())
    throw 本能双根二次关系概念异常{
        result->本能双根二次关系概念初始化.状态};
  result->特征值域比较=std::make_unique<特征值域比较数据服务>(
      *result->概念,*result->特征,*result->特征值);
  result->二次关系I64比较=std::make_unique<有序I64特征比较提供者>();
  result->二次关系求值=std::make_unique<二次关系求值应用服务>(
      l1,*result->概念,*result->场景,*result->状态,*result->存在,
      *result->特征,*result->特征值,*result->需求,
      *result->二次关系I64比较);
  result->自我根需求复核=std::make_unique<自我根需求复核提供者>(
      l1,*result->需求,*result->存在,*result->二次关系求值,
      result->本能双根二次关系概念初始化);
  result->原子I64特征出生=std::make_unique<原子I64特征出生数据服务>(
      *result->特征,*result->存在,*result->场景,*result->概念);
  result->特征概念=std::make_unique<特征概念应用服务>(
      *result->特征,*result->概念,*result->原子I64特征出生,*result->存在,
      result->本能先天特征概念初始化,报告实例特征结构异常);
  result->场景成员概念=std::make_unique<场景成员概念应用服务>(
      *result->特征概念);
  result->根服务 =
      std::make_unique<世界树根数据服务>(*result->存在, *result->场景);
  result->方法登记根初始化结构所有者 = 建立所有者(
      issuer, l1, 方法登记根初始化所有者建立键);
  result->方法结构聚合 = std::make_unique<L2方法结构聚合服务>(l1,
      std::move(*result->方法登记根初始化结构所有者.写入端口));
  result->方法登记根初始化 = std::make_unique<方法登记根生产初始化提供者>(
      *result->方法结构聚合);
  return result;
}

普通应用配置 规范化配置(普通应用配置 config) {
  auto&storage=config.L1事实基座持久存储;
  if(config.版本!=2||storage.合同版本!=L1事实基座持久恢复合同版本_v1||
     !config.自我线程邮箱容量||!config.自我线程进入停门等待毫秒||
     !config.自我线程停止回收诊断等待毫秒||
     storage.受控根.empty()||!storage.受控根.is_absolute())
    throw 普通应用装配状态::入口拒绝;
  const auto original=storage.受控根.native();
  if(original.find(L'\0')!=std::wstring::npos)
    throw 普通应用装配状态::入口拒绝;
  for(const auto&part:storage.受控根)
    if(part==L"."||part==L"..")throw 普通应用装配状态::入口拒绝;
  auto path=storage.受控根.lexically_normal();path.make_preferred();
  auto native=path.native();const auto root=path.root_path().native();
  while(native.size()>root.size()&&(native.back()==L'\\'||native.back()==L'/'))
    native.pop_back();
  storage.受控根=std::filesystem::path(std::move(native));
  if(storage.受控根.empty()||!storage.受控根.is_absolute())
    throw 普通应用装配状态::入口拒绝;
  return config;
}

bool 相同配置(const 普通应用配置&a,const 普通应用配置&b) noexcept {
  if(a.版本!=b.版本||
     a.L1事实基座持久存储.合同版本!=b.L1事实基座持久存储.合同版本||
     a.自我线程邮箱容量!=b.自我线程邮箱容量||
     a.自我线程进入停门等待毫秒!=b.自我线程进入停门等待毫秒||
     a.自我线程停止回收诊断等待毫秒!=b.自我线程停止回收诊断等待毫秒)
    return false;
  const auto&x=a.L1事实基座持久存储.受控根.native();
  const auto&y=b.L1事实基座持久存储.受控根.native();
  if(x.size()>INT_MAX||y.size()>INT_MAX)return false;
  return CompareStringOrdinal(x.data(),static_cast<int>(x.size()),
      y.data(),static_cast<int>(y.size()),TRUE)==CSTR_EQUAL;
}

普通应用配置 默认配置() {
  SetLastError(ERROR_SUCCESS);
  const auto required=GetEnvironmentVariableW(L"LOCALAPPDATA",nullptr,0);
  if(!required) {
    const auto error=GetLastError();
    if(error==ERROR_SUCCESS||error==ERROR_ENVVAR_NOT_FOUND)
      throw 普通应用装配状态::入口拒绝;
    throw 普通应用装配状态::资源失败;
  }
  std::vector<wchar_t> buffer(required);
  SetLastError(ERROR_SUCCESS);
  const auto written=GetEnvironmentVariableW(L"LOCALAPPDATA",buffer.data(),required);
  if(!written) {
    const auto error=GetLastError();
    if(error==ERROR_SUCCESS||error==ERROR_ENVVAR_NOT_FOUND)
      throw 普通应用装配状态::入口拒绝;
    throw 普通应用装配状态::资源失败;
  }
  if(written>=required)throw 普通应用装配状态::入口拒绝;
  std::filesystem::path root(std::wstring(buffer.data(),written));
  if(root.empty()||!root.is_absolute())throw 普通应用装配状态::入口拒绝;
  root/=L"海中鱼巣";root/=L"数据";root/=L"L1事实基座";
  return 规范化配置({2,{L1事实基座持久恢复合同版本_v1,std::move(root)},
      普通应用默认自我线程邮箱容量,
      普通应用默认自我线程进入停门等待毫秒,
      普通应用默认自我线程停止回收诊断等待毫秒});
}

constexpr 世界树读取预算 本能根运行世界树读取预算{64,256};
constexpr 世界树绑定创建预算 本能根运行自我读取预算{256,64,64};
constexpr 概念树预算 本能根运行自我概念读取预算{
    256,256,256,64,64,64};

本能根运行初始化结果_v1 本能根运行失败(
    const 本能根运行初始化请求_v1&request,
    const 本能根运行初始化状态_v1 status) noexcept {
  return {本能根运行初始化合同版本_v1,status,request,std::nullopt};
}

本能根运行初始化状态_v1 映射世界树读取失败(
    const 世界树操作状态 status) noexcept {
  if(status==世界树操作状态::事实代次漂移)
    return 本能根运行初始化状态_v1::当前性漂移;
  if(status==世界树操作状态::资源失败)
    return 本能根运行初始化状态_v1::资源失败;
  return 本能根运行初始化状态_v1::内部不一致;
}

本能根运行初始化状态_v1 映射自我读取失败(
    const 真实自我形成状态 status) noexcept {
  if(status==真实自我形成状态::读取未完成)
    return 本能根运行初始化状态_v1::当前性漂移;
  if(status==真实自我形成状态::资源失败)
    return 本能根运行初始化状态_v1::资源失败;
  return 本能根运行初始化状态_v1::内部不一致;
}

自我线程操作结果_v1 安全停止并回收自我线程(
    普通应用上下文&context,const 普通应用配置&config) noexcept {
  const auto stop=context.自我线程对象.请求停止();
  const auto joined=context.自我线程对象.等待停止(
      config.自我线程停止回收诊断等待毫秒);
  if(joined.成功())return joined;
  if(stop.状态!=自我线程操作状态::成功&&
     stop.状态!=自我线程操作状态::精确重复&&
     joined.状态==自我线程操作状态::入口拒绝)
    return stop;
  return joined;
}

自我线程创建结果_v1 回收失败结果(
    const 自我线程操作结果_v1&result) noexcept {
  return {result.状态,result.生命周期,std::nullopt,result.写业务事实};
}

} // namespace 普通应用装配内部

namespace 普通应用装配内部 {
普通应用装配结果 构造普通应用上下文实现(
    std::optional<普通应用配置> supplied) noexcept {
  std::lock_guard lock(上下文锁);
  普通应用装配结果 out;
  try {
    普通应用配置 selected;
    if(supplied)selected=规范化配置(std::move(*supplied));
    else if(已选配置)selected=*已选配置;
    else selected=默认配置();
    if(已选配置&&!相同配置(*已选配置,selected))return out;
    if(!已选配置)已选配置=std::move(selected);

    if(上下文) {
      out.持久恢复=上下文->持久恢复;
      if(!上下文->本能先天特征概念初始化.成功()) {
        out.状态=普通应用装配状态::本能先天特征概念初始化失败;
        out.本能先天特征概念原因=上下文->本能先天特征概念初始化.状态;
        return out;
      }
      if(!上下文->本能双根二次关系概念初始化.成功()) {
        out.状态=普通应用装配状态::本能双根二次关系概念初始化失败;
        out.本能双根二次关系概念原因=
            上下文->本能双根二次关系概念初始化.状态;
        return out;
      }
      if(!上下文->自我) {
        try {上下文->自我=std::make_unique<真实自我形成服务>(
            *上下文->世界树,*上下文->存在,上下文->角色结构.项目角色);}
        catch(...) {out.状态=普通应用装配状态::自我服务建立失败;return out;}
      }
      if(!上下文->自我线程正式上下文)
        上下文->自我线程正式上下文=
            std::make_unique<自我线程正式上下文提供者>(
                *上下文->世界树,*上下文->自我,*上下文->需求,
                上下文->方法结构聚合->取得L2方法结构服务(),
                上下文->角色结构.项目角色);
      const 世界树读取预算 rootBudget{64,256};
      const auto root=上下文->世界树->读取当前现实世界根(rootBudget);
      if(!root.成功(rootBudget)) {
        out.状态=普通应用装配状态::内部不一致;
        return out;
      }
      out.状态=普通应用装配状态::已装配;
      out.根事实代次=root.结果头.Gread;
      return out;
    }

    auto candidate=建立上下文(*已选配置,out.持久恢复);
    auto&l1=candidate->运行包->读取服务();
    const 世界树根初始化请求 rootRequest{
        世界树根合同版本,candidate->根原G0.value_or(当前代次(l1)),
        {0x2001},{0x2002},{0x2003},256,64};
    const auto initialized=candidate->根服务->初始化世界树根(rootRequest);
    if(!initialized.成功(rootRequest)) {
      out.状态=普通应用装配状态::世界树根失败;
      return out;
    }

    const 世界树根验证请求 verifyRequest{2,initialized.Gread,{64,256}};
    auto world=建立世界树应用服务(*candidate->场景,*candidate->存在,
                                    *candidate->概念,verifyRequest);
    const bool worldEstablished=world.成功();
    if(!world.服务) {
      out.状态=普通应用装配状态::世界树应用失败;
      return out;
    }
    candidate->世界树=std::move(world.服务);
    上下文=std::move(candidate);
    if(!worldEstablished||
       !上下文->世界树->验证现实世界根(verifyRequest).成功(verifyRequest)) {
      out.状态=普通应用装配状态::世界树应用失败;
      return out;
    }
    try {
      上下文->自我=std::make_unique<真实自我形成服务>(
          *上下文->世界树,*上下文->存在,上下文->角色结构.项目角色);
    } catch(...) {
      out.状态=普通应用装配状态::自我服务建立失败;
      return out;
    }
    上下文->自我线程正式上下文=
        std::make_unique<自我线程正式上下文提供者>(
            *上下文->世界树,*上下文->自我,*上下文->需求,
            上下文->方法结构聚合->取得L2方法结构服务(),
            上下文->角色结构.项目角色);
    const 世界树读取预算 rootBudget{64,256};
    const auto root=上下文->世界树->读取当前现实世界根(rootBudget);
    if(!root.成功(rootBudget)) {
      out.状态=普通应用装配状态::内部不一致;
      return out;
    }
    out.状态=普通应用装配状态::已装配;
    out.根事实代次=root.结果头.Gread;
  } catch(const 持久恢复异常&e) {
    out.状态=普通应用装配状态::L1事实基座持久恢复失败;
    out.持久恢复=e.结果;
  } catch(普通应用装配状态 status) {
    out.状态=status;
  } catch(const 概念结构异常&e) {
    out.状态=普通应用装配状态::概念结构失败;out.概念原因=e.原因;
  } catch(const 角色结构异常&e) {
    out.状态=普通应用装配状态::单例角色结构失败;out.角色原因=e.原因;
  } catch(const 本能先天特征概念异常&e) {
    out.状态=普通应用装配状态::本能先天特征概念初始化失败;
    out.本能先天特征概念原因=e.原因;
  } catch(const 本能双根二次关系概念异常&e) {
    out.状态=普通应用装配状态::本能双根二次关系概念初始化失败;
    out.本能双根二次关系概念原因=e.原因;
  } catch(const std::bad_alloc&) {
    out.状态=普通应用装配状态::资源失败;
  } catch(const std::length_error&) {
    out.状态=普通应用装配状态::资源失败;
  } catch(...) {
    out.状态=普通应用装配状态::内部不一致;
  }
  return out;
}
} // namespace 普通应用装配内部

普通应用装配结果 构造普通应用上下文(
    const 普通应用配置&config) noexcept {
  try {
    return 普通应用装配内部::构造普通应用上下文实现(config);
  } catch(const std::bad_alloc&) {
    普通应用装配结果 out;out.状态=普通应用装配状态::资源失败;return out;
  } catch(const std::length_error&) {
    普通应用装配结果 out;out.状态=普通应用装配状态::资源失败;return out;
  } catch(...) {
    普通应用装配结果 out;out.状态=普通应用装配状态::内部不一致;return out;
  }
}
普通应用装配结果 构造普通应用上下文() noexcept {
  return 普通应用装配内部::构造普通应用上下文实现(std::nullopt);
}
真实自我形成结果 初始化普通应用自我() noexcept {
  using namespace 普通应用装配内部;
  std::lock_guard lock(上下文锁);
  if(!上下文||!上下文->世界树)return {};
  try {
    if(!上下文->自我)上下文->自我=std::make_unique<真实自我形成服务>(
        *上下文->世界树,*上下文->存在,上下文->角色结构.项目角色);
    const 世界树读取预算 rootBudget{64,256};
    const auto root=上下文->世界树->读取当前现实世界根(rootBudget);
    if(!root.成功(rootBudget))return {};
    const 真实自我形成请求 request{1,root.树->根场景,上下文->角色结构.项目角色,
        {0x3001},{{0x3002},{0x3003},{0x3004},{0x3005}},
        {256,64,64},{256,256,256,64,64,64}};
    return 上下文->自我->形成(request);
  } catch(const std::bad_alloc&){真实自我形成结果 o;o.状态=真实自我形成状态::资源失败;return o;}
    catch(...){真实自我形成结果 o;o.状态=真实自我形成状态::内部不一致;return o;}
}

真实自我形成结果 读取普通应用自我() noexcept {
  using namespace 普通应用装配内部;
  std::lock_guard lock(上下文锁);
  真实自我形成结果 out;
  if(!上下文||!上下文->自我)return out;
  const 世界树读取预算 rootBudget{64,256};
  const auto root=上下文->世界树->读取当前现实世界根(rootBudget);
  if(!root.成功(rootBudget)){out.状态=真实自我形成状态::读取未完成;return out;}
  return 上下文->自我->读取当前自我({1,root.结果头.Gread,root.树->根场景,
      上下文->角色结构.项目角色,{256,64,64},{256,256,256,64,64,64}});
}

方法登记根生产初始化结果 初始化普通应用方法登记根() noexcept {
  using namespace 普通应用装配内部;
  std::lock_guard lock(上下文锁);
  方法登记根生产初始化结果 out;
  if (!上下文 || !上下文->世界树 || !上下文->自我 ||
      !上下文->方法登记根初始化) return out;
  try {
    const 世界树读取预算 rootBudget{64,256};
    const auto root = 上下文->世界树->读取当前现实世界根(rootBudget);
    if (!root.成功(rootBudget)) return out;
    const 真实自我读取请求 selfRequest{1, root.结果头.Gread,
        root.树->根场景, 上下文->角色结构.项目角色, {256,64,64},
        {256,256,256,64,64,64}};
    const auto self = 上下文->自我->读取当前自我(selfRequest);
    if (!self.成功(selfRequest)) return out;
    return 上下文->方法登记根初始化->初始化({1});
  } catch (const std::bad_alloc &) {
    out.状态 = 方法登记根生产初始化状态::资源失败;
  } catch (...) {
    out.状态 = 方法登记根生产初始化状态::内部不一致;
  }
  return out;
}

本能根运行初始化结果_v1 初始化普通应用本能根运行锚点(
    const 方法登记根生产初始化结果&method) noexcept {
  using namespace 普通应用装配内部;
  std::lock_guard lock(上下文锁);
  本能根运行初始化请求_v1 request{};
  const auto 清除候选缓存=[](普通应用上下文&context) noexcept {
    context.本能根运行锚点.reset();
    context.本能根运行自我投影.reset();
    context.本能根运行根场景.reset();
  };
  if(!上下文||!上下文->世界树||!上下文->自我||
     !上下文->特征概念||!上下文->存在||!上下文->需求||
     !method.成功()||!method.最终Gread) {
    if(上下文)清除候选缓存(*上下文);
    auto out=本能根运行失败(request,
        本能根运行初始化状态_v1::入口拒绝);
    if(上下文)上下文->本能根运行初始化尝试=out;
    return out;
  }
  if(上下文->自我线程对象.读取诊断快照().生命周期!=
      自我线程生命周期状态::未创建) {
    清除候选缓存(*上下文);
    auto out=本能根运行失败(request,
        本能根运行初始化状态_v1::入口拒绝);
    上下文->本能根运行初始化尝试=out;
    return out;
  }
  清除候选缓存(*上下文);
  try {
    const 世界树根验证请求 rootRequest{
        2,method.最终Gread,本能根运行世界树读取预算};
    const auto root=上下文->世界树->验证现实世界根(rootRequest);
    if(!root.成功(rootRequest)||!root.树) {
      auto out=本能根运行失败(request,
          映射世界树读取失败(root.结果头.状态));
      上下文->本能根运行初始化尝试=out;
      return out;
    }
    const 真实自我读取请求 selfRequest{
        1,method.最终Gread,root.树->根场景,
        上下文->角色结构.项目角色,本能根运行自我读取预算,
        本能根运行自我概念读取预算};
    const auto self=上下文->自我->读取当前自我(selfRequest);
    if(!self.成功(selfRequest)||!self.投影) {
      auto out=本能根运行失败(request,
          映射自我读取失败(self.状态));
      上下文->本能根运行初始化尝试=out;
      return out;
    }
    const auto&projection=*self.投影;
    if(!有效(projection.E)||projection.Gread!=method.最终Gread||
       projection.H!=method.最终Gread||
       projection.世界根!=root.树->根场景||
       !projection.位置.直接结构父||
       projection.位置.直接结构父->来源!=直接归属来源::直接子场景||
       projection.位置.直接结构父->父!=root.树->根场景||
       projection.位置.直接结构父->成员!=projection.E) {
      auto out=本能根运行失败(request,
          本能根运行初始化状态_v1::内部不一致);
      上下文->本能根运行初始化尝试=out;
      return out;
    }
    request.唯一自我={projection.E};
    本能根运行初始化提供者 provider(*上下文->特征概念,
        *上下文->存在,*上下文->需求,selfRequest,self);
    auto out=provider.初始化(request);
    上下文->本能根运行初始化尝试=out;
    if(!out.成功())return out;
    上下文->本能根运行锚点=*out.锚点;
    上下文->本能根运行自我投影=projection;
    上下文->本能根运行根场景=root.树->根场景;
    return out;
  } catch(const std::bad_alloc&) {
    清除候选缓存(*上下文);
    auto out=本能根运行失败(request,
        本能根运行初始化状态_v1::资源失败);
    上下文->本能根运行初始化尝试=out;
    return out;
  } catch(const std::length_error&) {
    清除候选缓存(*上下文);
    auto out=本能根运行失败(request,
        本能根运行初始化状态_v1::资源失败);
    上下文->本能根运行初始化尝试=out;
    return out;
  } catch(...) {
    清除候选缓存(*上下文);
    auto out=本能根运行失败(request,
        本能根运行初始化状态_v1::内部不一致);
    上下文->本能根运行初始化尝试=out;
    return out;
  }
}

自我线程创建结果_v1 创建并停门普通应用自我线程() noexcept {
  using namespace 普通应用装配内部;
  std::lock_guard lock(上下文锁);
  if(!上下文||!已选配置||!上下文->世界树||!上下文->自我||
     !上下文->自我线程正式上下文||
     !上下文->本能根运行锚点||!上下文->本能根运行自我投影||
     !上下文->本能根运行根场景)
    return {};
  const auto 收口=[&](自我线程创建结果_v1 original) noexcept {
    const auto snapshot=上下文->自我线程对象.读取诊断快照();
    if(snapshot.生命周期==自我线程生命周期状态::未创建)
      return original;
    const auto cleanup=安全停止并回收自我线程(*上下文,*已选配置);
    return cleanup.成功()?original:回收失败结果(cleanup);
  };
  try {
    const auto&anchor=*上下文->本能根运行锚点;
    const 世界树根验证请求 rootRequest{
        2,anchor.事实截止代次,本能根运行世界树读取预算};
    const auto root=上下文->世界树->验证现实世界根(rootRequest);
    if(!root.成功(rootRequest)||!root.树||
       root.树->根场景!=*上下文->本能根运行根场景)
      return 收口({});
    const 真实自我读取请求 selfRequest{
        1,anchor.事实截止代次,root.树->根场景,
        上下文->角色结构.项目角色,本能根运行自我读取预算,
        本能根运行自我概念读取预算};
    const auto self=上下文->自我->读取当前自我(selfRequest);
    if(!self.成功(selfRequest)||!self.投影)return 收口({});
    const auto&projection=*self.投影;
    if(projection.E!=anchor.自我.编码||
       projection.世界根!=root.树->根场景||
       !projection.位置.直接结构父||
       projection.位置.直接结构父->来源!=直接归属来源::直接子场景||
       projection.位置.直接结构父->父!=root.树->根场景||
       projection.位置.直接结构父->成员!=projection.E)
      return 收口({});

    自我线程创建请求_v1 request;
    request.邮箱容量=已选配置->自我线程邮箱容量;
    request.世界={{root.树->根场景.值},{root.树->根场景.值},
        root.结果头.Gread};
    request.自我={{projection.E.值},{projection.世界根.值},
        {projection.位置.直接结构父->父.值},projection.Gread};
    request.本能根.自我={anchor.自我.编码.值};
    request.本能根.安全根={{anchor.安全根.需求.值},
        {anchor.安全根.列表项.值},{anchor.安全根.实际特征.编码.值},
        {anchor.安全根.目标合同.值}};
    request.本能根.服务根={{anchor.服务根.需求.值},
        {anchor.服务根.列表项.值},{anchor.服务根.实际特征.编码.值},
        {anchor.服务根.目标合同.值}};
    request.本能根.Gread=anchor.事实截止代次;
    if(!request.完整())
      return 收口({自我线程操作状态::内部错误,
          自我线程生命周期状态::未创建,std::nullopt,false});

    auto created=上下文->自我线程对象.创建并停在治理运行门(
        request,*上下文->自我线程正式上下文,
        已选配置->自我线程进入停门等待毫秒);
    const auto snapshot=上下文->自我线程对象.读取诊断快照();
    const bool postcondition=created.成功()&&created.见证&&
        created.见证->完整()&&
        snapshot.生命周期==自我线程生命周期状态::已停门&&
        !snapshot.治理运行门开启&&snapshot.线程已进入&&
        !snapshot.线程已完成&&snapshot.邮箱容量==request.邮箱容量&&
        snapshot.邮箱数量==0&&!snapshot.当前消息&&
        snapshot.已冻结批次数量==0&&!snapshot.内部错误锁存;
    if(postcondition)return created;
    if(created.成功())
      created={自我线程操作状态::内部错误,snapshot.生命周期,
          std::nullopt,false};
    return 收口(created);
  } catch(const std::bad_alloc&) {
    return 收口({自我线程操作状态::资源失败,
        自我线程生命周期状态::启动失败,std::nullopt,false});
  } catch(const std::length_error&) {
    return 收口({自我线程操作状态::资源失败,
        自我线程生命周期状态::启动失败,std::nullopt,false});
  } catch(...) {
    return 收口({自我线程操作状态::内部错误,
        自我线程生命周期状态::内部错误,std::nullopt,false});
  }
}

自我线程操作结果_v1 停止并回收普通应用自我线程() noexcept {
  using namespace 普通应用装配内部;
  std::lock_guard lock(上下文锁);
  if(!上下文||!已选配置)return {};
  return 安全停止并回收自我线程(*上下文,*已选配置);
}

std::optional<本能先天特征概念初始化结果>
读取普通应用本能先天特征概念初始化() noexcept {
  using namespace 普通应用装配内部;
  std::lock_guard lock(上下文锁);
  if(!上下文||!上下文->本能先天特征概念初始化.成功())return std::nullopt;
  return 上下文->本能先天特征概念初始化;
}

std::optional<本能双根二次关系概念初始化结果>
读取普通应用本能双根二次关系概念初始化() noexcept {
  using namespace 普通应用装配内部;
  std::lock_guard lock(上下文锁);
  if(!上下文||!上下文->本能双根二次关系概念初始化.成功())
    return std::nullopt;
  return 上下文->本能双根二次关系概念初始化;
}

二次关系求值应用服务* 读取普通应用二次关系求值服务() noexcept {
  using namespace 普通应用装配内部;
  std::lock_guard lock(上下文锁);
  return 上下文 && 上下文->二次关系求值
      ? 上下文->二次关系求值.get() : nullptr;
}

自我根需求复核提供者* 读取普通应用自我根需求复核服务() noexcept {
  using namespace 普通应用装配内部;
  std::lock_guard lock(上下文锁);
  return 上下文 && 上下文->自我根需求复核
      ? 上下文->自我根需求复核.get() : nullptr;
}

需求类数据服务* 读取普通应用需求服务() noexcept {
  using namespace 普通应用装配内部;
  std::lock_guard lock(上下文锁);
  return 上下文 && 上下文->需求 ? 上下文->需求.get() : nullptr;
}

本能根任务核心端口_v1& 本能根任务核心() noexcept {
  using namespace 普通应用装配内部;
  std::lock_guard lock(上下文锁);
  if (!上下文 || !上下文->本能根任务核心) std::terminate();
  return *上下文->本能根任务核心;
}

} // namespace 海中鱼巣
