#include "应用服务.本能根运行初始化.h"
#include "应用服务.本能根运行初始化.内部.h"

#include <algorithm>
#include <array>
#include <limits>
#include <new>
#include <stdexcept>
#include <utility>

namespace 海中鱼巣::本能根运行初始化内部 {
namespace {

using 状态 = 本能根运行初始化状态_v1;

struct 根固定参数 final {
  本能值角色 特征角色;
  本能根角色 需求角色;
  L1所有者范围写入幂等身份 IFR键;
  L1所有者范围写入幂等身份 FCv概念键;
  L1所有者范围写入幂等身份 当前采用键;
  原子I64特征出生键 F出生键;
};

constexpr 根固定参数 安全参数{
    本能值角色::安全值,
    本能根角色::安全,
    {0x4952415341460001ULL},
    {0x4952415341460002ULL},
    {0x4952415341460003ULL},
    {{0x4952415341460011ULL}, {0x4952415341460012ULL},
     {0x4952415341460013ULL}, {0x4952415341460014ULL},
     {0x4952415341460015ULL}}};

constexpr 根固定参数 服务参数{
    本能值角色::服务值,
    本能根角色::服务,
    {0x4952415352560001ULL},
    {0x4952415352560002ULL},
    {0x4952415352560003ULL},
    {{0x4952415352560011ULL}, {0x4952415352560012ULL},
     {0x4952415352560013ULL}, {0x4952415352560014ULL},
     {0x4952415352560015ULL}}};

constexpr 实例特征IFR读取预算 IFR预算{8, 8, 32, 8, 64};
constexpr 特征R规则读取预算 R规则预算{16, 64, 16, 8, 8, 64, 64};
constexpr 原子I64特征候选读取预算 候选预算{8};
constexpr 原子I64特征组织读取预算 组织预算{8};
constexpr 原子I64特征概念读取预算 概念读取预算{16, 64, 16};
constexpr 概念树预算 概念预算{64, 256, 64, 64, 64, 64, 64, 64};
constexpr std::uint64_t 当前采用关系预算 = 16;

本能根运行初始化结果_v1 失败(const 本能根运行初始化请求_v1 &请求,
                                状态 s) noexcept {
  return {本能根运行初始化合同版本_v1, s, 请求, std::nullopt};
}

int 优先级(状态 s) noexcept {
  switch (s) {
  case 状态::已可能发布: return 8;
  case 状态::幂等冲突: return 7;
  case 状态::当前性漂移: return 6;
  case 状态::资源失败: return 5;
  case 状态::引用冲突: return 4;
  case 状态::材料未闭合: return 3;
  case 状态::入口拒绝: return 2;
  default: return 1;
  }
}

状态 合并(状态 a, 状态 b) noexcept {
  return 优先级(b) > 优先级(a) ? b : a;
}

状态 映射(需求类数据状态 s) noexcept {
  switch (s) {
  case 需求类数据状态::入口拒绝: return 状态::入口拒绝;
  case 需求类数据状态::事实代次漂移: return 状态::当前性漂移;
  case 需求类数据状态::资源失败: return 状态::资源失败;
  default: return 状态::内部不一致;
  }
}

状态 映射(本能根材料状态 s) noexcept {
  switch (s) {
  case 本能根材料状态::入口拒绝: return 状态::入口拒绝;
  case 本能根材料状态::事实代次漂移: return 状态::当前性漂移;
  case 本能根材料状态::幂等冲突: return 状态::幂等冲突;
  case 本能根材料状态::引用冲突:
  case 本能根材料状态::实际特征未找到:
  case 本能根材料状态::实际特征类型不匹配:
    return 状态::引用冲突;
  case 本能根材料状态::根材料未闭合: return 状态::材料未闭合;
  case 本能根材料状态::已可能发布: return 状态::已可能发布;
  case 本能根材料状态::资源失败: return 状态::资源失败;
  default: return 状态::内部不一致;
  }
}

状态 映射(存在类数据状态 s) noexcept {
  switch (s) {
  case 存在类数据状态::入口拒绝: return 状态::入口拒绝;
  case 存在类数据状态::事实代次漂移: return 状态::当前性漂移;
  case 存在类数据状态::幂等冲突: return 状态::幂等冲突;
  case 存在类数据状态::引用冲突:
    return 状态::引用冲突;
  case 存在类数据状态::未找到:
  case 存在类数据状态::结构类型未找到:
  case 存在类数据状态::格式不支持:
  case 存在类数据状态::数量预算不足:
    return 状态::材料未闭合;
  case 存在类数据状态::已可能发布: return 状态::已可能发布;
  case 存在类数据状态::资源失败: return 状态::资源失败;
  default: return 状态::内部不一致;
  }
}

状态 映射(实例特征IFR状态 s) noexcept {
  switch (s) {
  case 实例特征IFR状态::入口拒绝: return 状态::入口拒绝;
  case 实例特征IFR状态::未找到:
  case 实例特征IFR状态::数量预算不足:
    return 状态::材料未闭合;
  case 实例特征IFR状态::引用冲突:
    return 状态::引用冲突;
  case 实例特征IFR状态::事实代次漂移: return 状态::当前性漂移;
  case 实例特征IFR状态::幂等冲突: return 状态::幂等冲突;
  case 实例特征IFR状态::已可能发布: return 状态::已可能发布;
  case 实例特征IFR状态::资源失败: return 状态::资源失败;
  default: return 状态::内部不一致;
  }
}

状态 映射(I64单值出生概念确保状态 s) noexcept {
  switch (s) {
  case I64单值出生概念确保状态::入口拒绝: return 状态::入口拒绝;
  case I64单值出生概念确保状态::规则缺失:
    return 状态::材料未闭合;
  case I64单值出生概念确保状态::类型不相容: return 状态::引用冲突;
  case I64单值出生概念确保状态::事实代次漂移: return 状态::当前性漂移;
  case I64单值出生概念确保状态::幂等冲突: return 状态::幂等冲突;
  case I64单值出生概念确保状态::已可能发布: return 状态::已可能发布;
  case I64单值出生概念确保状态::资源失败: return 状态::资源失败;
  default: return 状态::内部不一致;
  }
}

状态 映射(原子I64特征候选查询状态 s) noexcept {
  switch (s) {
  case 原子I64特征候选查询状态::入口拒绝: return 状态::入口拒绝;
  case 原子I64特征候选查询状态::位置冲突:
    return 状态::引用冲突;
  case 原子I64特征候选查询状态::预算不足:
    return 状态::材料未闭合;
  case 原子I64特征候选查询状态::事实代次漂移: return 状态::当前性漂移;
  case 原子I64特征候选查询状态::资源失败: return 状态::资源失败;
  default: return 状态::内部不一致;
  }
}

状态 映射(原子I64特征窄读取状态 s) noexcept {
  switch (s) {
  case 原子I64特征窄读取状态::入口拒绝: return 状态::入口拒绝;
  case 原子I64特征窄读取状态::位置冲突:
    return 状态::引用冲突;
  case 原子I64特征窄读取状态::未找到:
  case 原子I64特征窄读取状态::预算不足:
    return 状态::材料未闭合;
  case 原子I64特征窄读取状态::事实代次漂移: return 状态::当前性漂移;
  case 原子I64特征窄读取状态::资源失败: return 状态::资源失败;
  default: return 状态::内部不一致;
  }
}

状态 映射(原子I64特征出生状态 s) noexcept {
  switch (s) {
  case 原子I64特征出生状态::入口拒绝: return 状态::入口拒绝;
  case 原子I64特征出生状态::位置冲突:
  case 原子I64特征出生状态::概念不适配:
    return 状态::引用冲突;
  case 原子I64特征出生状态::未找到:
  case 原子I64特征出生状态::存量未定位:
  case 原子I64特征出生状态::预算不足:
    return 状态::材料未闭合;
  case 原子I64特征出生状态::事实代次漂移: return 状态::当前性漂移;
  case 原子I64特征出生状态::幂等冲突: return 状态::幂等冲突;
  case 原子I64特征出生状态::已可能发布: return 状态::已可能发布;
  case 原子I64特征出生状态::资源失败: return 状态::资源失败;
  default: return 状态::内部不一致;
  }
}

状态 映射(const I64原子准确特征出生应用结果 &r) noexcept {
  switch (r.状态) {
  case I64原子准确特征出生应用状态::入口拒绝: return 状态::入口拒绝;
  case I64原子准确特征出生应用状态::需选择F: return 状态::引用冲突;
  case I64原子准确特征出生应用状态::FCv确保失败:
    return r.FCv确保 ? 映射(r.FCv确保->状态) : 状态::内部不一致;
  case I64原子准确特征出生应用状态::候选查询失败:
    return r.候选查询 ? 映射(r.候选查询->状态) : 状态::内部不一致;
  case I64原子准确特征出生应用状态::候选读取失败: {
    if (r.候选读取组.empty())
      return 状态::内部不一致;
    状态 out = 状态::内部不一致;
    for (const auto &x : r.候选读取组)
      out = 合并(out, 映射(x.状态));
    return out;
  }
  case I64原子准确特征出生应用状态::出生失败:
    return r.出生 ? 映射(r.出生->状态) : 状态::内部不一致;
  default: return 状态::内部不一致;
  }
}

状态 映射(const 实例特征R观察结果 &r) noexcept {
  switch (r.状态) {
  case 实例特征R观察状态::入口拒绝: return 状态::入口拒绝;
  case 实例特征R观察状态::读取失败:
    return r.IFR ? 映射(r.IFR->状态) : 状态::内部不一致;
  case 实例特征R观察状态::比较未启用:
  case 实例特征R观察状态::归组失败:
    return 状态::材料未闭合;
  case 实例特征R观察状态::结构异常: return 状态::内部不一致;
  case 实例特征R观察状态::F出生失败:
    return r.F出生 ? 映射(*r.F出生) : 状态::内部不一致;
  case 实例特征R观察状态::IFR写入失败:
    return r.IFR ? 映射(r.IFR->状态) : 状态::内部不一致;
  default: return 状态::内部不一致;
  }
}

状态 映射(const 本能根I64实际F结果 &r) noexcept {
  switch (r.状态) {
  case 本能根I64实际F状态::入口拒绝: return 状态::入口拒绝;
  case 本能根I64实际F状态::初始化交付不完整: return 状态::材料未闭合;
  case 本能根I64实际F状态::事实代次漂移: return 状态::当前性漂移;
  case 本能根I64实际F状态::引用冲突: return 状态::引用冲突;
  case 本能根I64实际F状态::资源失败: return 状态::资源失败;
  case 本能根I64实际F状态::当前采用读取失败:
  case 本能根I64实际F状态::内部不一致:
    return 状态::内部不一致;
  case 本能根I64实际F状态::IFR失败:
    return r.IFR ? 映射(*r.IFR) : 状态::内部不一致;
  case 本能根I64实际F状态::F出生失败:
    return r.IFR && r.IFR->F出生 ? 映射(*r.IFR->F出生)
                                     : 状态::内部不一致;
  case 本能根I64实际F状态::当前采用写入失败:
    return r.当前采用 ? 映射(r.当前采用->状态) : 状态::内部不一致;
  default: return 状态::内部不一致;
  }
}

std::optional<std::uint64_t>
提取根F成功截止(const 本能根I64实际F结果 &r) noexcept {
  if (!r.成功() || !r.初始化交付 || !r.IFR读回 || !r.当前采用 ||
      !r.当前采用->采用 || !r.准确F || !r.原子读回 ||
      !r.原子读回->事实 || !r.单值概念读回 || !r.实际F)
    return std::nullopt;
  const auto g = r.当前采用->Gread;
  if (!g || r.IFR读回->Gread != g || r.准确F->Gread != g ||
      r.原子读回->Gread != g || r.单值概念读回->Gread != g ||
      r.初始化交付->Gread == 0 || r.初始化交付->Gread > g ||
      *r.实际F != r.当前采用->采用->F ||
      *r.实际F != r.准确F->信息.身份 ||
      r.原子读回->事实->F != r.实际F->编码)
    return std::nullopt;
  return g;
}

std::optional<稳定编码> 查找唯一特征根(const 真实自我投影 &投影) noexcept {
  std::optional<稳定编码> out;
  for (const auto &根 : 投影.场景.场景角色.四根) {
    if (根.角色 != 场景根角色::特征)
      continue;
    if (out || !有效(根.根.编码))
      return std::nullopt;
    out = 根.根.编码;
  }
  return out;
}

本能根I64实际F请求 构造F请求(std::uint64_t g, 存在信息身份 e,
                              稳定编码 特征根,
                              const 根固定参数 &p) {
  return {1, g, p.特征角色, e, {e.编码, 特征根}, p.IFR键,
          p.FCv概念键, p.当前采用键, p.F出生键, IFR预算, R规则预算,
          候选预算, 组织预算, 概念读取预算, 概念预算,
          当前采用关系预算};
}

bool 根材料匹配(const 本能根材料结果 &r, const 本能根材料请求 &请求,
                本能根材料状态 最小状态) noexcept {
  if (!r.成功(请求) || !r.材料 || r.材料->读取事实代次 != r.事实代次)
    return false;
  if (最小状态 == 本能根材料状态::已形成)
    return true;
  return r.状态 == 本能根材料状态::已恢复 ||
         r.状态 == 本能根材料状态::已读取;
}

状态 核验守卫(本能根运行初始化调用端口 &端口,
              std::uint64_t g) noexcept {
  const auto r = 端口.核验当前事实代次(g);
  return r.成功() ? 状态::已恢复 : 映射(r.状态);
}

} // namespace

本能根运行初始化结果_v1
执行组合(本能根运行初始化调用端口 &端口, const 真实自我投影 &投影,
         const 本能根运行初始化请求_v1 &请求) noexcept {
  try {
    const 存在信息身份 自我{投影.E};
    const auto 特征根 = 查找唯一特征根(投影);
    if (请求.合同版本 != 本能根运行初始化合同版本_v1 || !有效(自我) ||
        请求.唯一自我 != 自我 || !投影.Gread ||
        !特征根)
      return 失败(请求, 状态::入口拒绝);

    auto g = 投影.Gread;
    auto guard = 核验守卫(端口, g);
    if (guard != 状态::已恢复)
      return 失败(请求, guard);

    bool 本次发布 = false;
    auto 安全F请求 = 构造F请求(g, 自我, *特征根, 安全参数);
    auto 安全F = 端口.形成或读取实际F(安全F请求);
    if (!安全F.成功())
      return 失败(请求, 映射(安全F));
    auto 安全G = 提取根F成功截止(安全F);
    if (!安全G)
      return 失败(请求, 状态::内部不一致);
    本次发布 = 安全F.状态 == 本能根I64实际F状态::已形成;
    g = *安全G;
    guard = 核验守卫(端口, g);
    if (guard != 状态::已恢复)
      return 失败(请求, guard);

    const 本能根材料请求 安全根请求{1, g, 本能根角色::安全,
                                            安全F.实际F->编码};
    auto 安全根 = 端口.建立或读取根材料(安全根请求);
    if (!根材料匹配(安全根, 安全根请求, 本能根材料状态::已形成))
      return 失败(请求, 安全根.成功(安全根请求) ? 状态::内部不一致
                                                  : 映射(安全根.状态));
    本次发布 = 本次发布 || 安全根.状态 == 本能根材料状态::已形成;
    g = 安全根.事实代次;
    guard = 核验守卫(端口, g);
    if (guard != 状态::已恢复)
      return 失败(请求, guard);

    auto 服务F请求 = 构造F请求(g, 自我, *特征根, 服务参数);
    auto 服务F = 端口.形成或读取实际F(服务F请求);
    if (!服务F.成功())
      return 失败(请求, 映射(服务F));
    auto 服务G = 提取根F成功截止(服务F);
    if (!服务G)
      return 失败(请求, 状态::内部不一致);
    if (安全F.实际F == 服务F.实际F ||
        安全F.初始化交付->类型.身份 == 服务F.初始化交付->类型.身份 ||
        安全F.初始化交付->完整域概念.概念 ==
            服务F.初始化交付->完整域概念.概念)
      return 失败(请求, 状态::引用冲突);
    本次发布 = 本次发布 || 服务F.状态 == 本能根I64实际F状态::已形成;
    g = *服务G;
    guard = 核验守卫(端口, g);
    if (guard != 状态::已恢复)
      return 失败(请求, guard);

    const 本能根材料请求 服务根请求{1, g, 本能根角色::服务,
                                            服务F.实际F->编码};
    auto 服务根 = 端口.建立或读取根材料(服务根请求);
    if (!根材料匹配(服务根, 服务根请求, 本能根材料状态::已形成))
      return 失败(请求, 服务根.成功(服务根请求) ? 状态::内部不一致
                                                  : 映射(服务根.状态));
    本次发布 = 本次发布 || 服务根.状态 == 本能根材料状态::已形成;
    const auto gfinal = 服务根.事实代次;
    guard = 核验守卫(端口, gfinal);
    if (guard != 状态::已恢复)
      return 失败(请求, guard);

    安全F请求.G0 = gfinal;
    auto 安全F重读 = 端口.形成或读取实际F(安全F请求);
    const auto 安全重读G = 提取根F成功截止(安全F重读);
    if (!安全重读G || *安全重读G != gfinal ||
        安全F重读.状态 != 本能根I64实际F状态::已读取 ||
        安全F重读.实际F != 安全F.实际F)
      return 失败(请求, 安全F重读.成功() ? 状态::内部不一致
                                          : 映射(安全F重读));
    guard = 核验守卫(端口, gfinal);
    if (guard != 状态::已恢复)
      return 失败(请求, guard);

    服务F请求.G0 = gfinal;
    auto 服务F重读 = 端口.形成或读取实际F(服务F请求);
    const auto 服务重读G = 提取根F成功截止(服务F重读);
    if (!服务重读G || *服务重读G != gfinal ||
        服务F重读.状态 != 本能根I64实际F状态::已读取 ||
        服务F重读.实际F != 服务F.实际F)
      return 失败(请求, 服务F重读.成功() ? 状态::内部不一致
                                          : 映射(服务F重读));
    guard = 核验守卫(端口, gfinal);
    if (guard != 状态::已恢复)
      return 失败(请求, guard);

    const 本能根材料请求 安全根重读请求{
        1, gfinal, 本能根角色::安全, 安全F.实际F->编码};
    auto 安全根重读 = 端口.建立或读取根材料(安全根重读请求);
    if (!根材料匹配(安全根重读, 安全根重读请求,
                    本能根材料状态::已恢复))
      return 失败(请求, 安全根重读.成功(安全根重读请求)
                              ? 状态::内部不一致
                              : 映射(安全根重读.状态));
    guard = 核验守卫(端口, gfinal);
    if (guard != 状态::已恢复)
      return 失败(请求, guard);

    const 本能根材料请求 服务根重读请求{
        1, gfinal, 本能根角色::服务, 服务F.实际F->编码};
    auto 服务根重读 = 端口.建立或读取根材料(服务根重读请求);
    if (!根材料匹配(服务根重读, 服务根重读请求,
                    本能根材料状态::已恢复))
      return 失败(请求, 服务根重读.成功(服务根重读请求)
                              ? 状态::内部不一致
                              : 映射(服务根重读.状态));
    guard = 核验守卫(端口, gfinal);
    if (guard != 状态::已恢复)
      return 失败(请求, guard);

    if (!安全根重读.材料 || !服务根重读.材料)
      return 失败(请求, 状态::内部不一致);
    const auto &a = *安全根重读.材料;
    const auto &s = *服务根重读.材料;
    if (a.目标I64值 != std::numeric_limits<std::int64_t>::max() ||
        s.目标I64值 != std::numeric_limits<std::int64_t>::max())
      return 失败(请求, 状态::引用冲突);

    本能根运行锚点_v1 锚点{
        自我,
        {a.根需求, a.根列表项, 特征信息身份{a.实际特征}, a.根目标合同},
        {s.根需求, s.根列表项, 特征信息身份{s.实际特征}, s.根目标合同},
        gfinal};
    if (!锚点.完整())
      return 失败(请求, 状态::引用冲突);
    return {本能根运行初始化合同版本_v1,
            本次发布 ? 状态::已形成 : 状态::已恢复, 请求,
            std::move(锚点)};
  } catch (const std::bad_alloc &) {
    return 失败(请求, 状态::资源失败);
  } catch (const std::length_error &) {
    return 失败(请求, 状态::资源失败);
  } catch (...) {
    return 失败(请求, 状态::内部不一致);
  }
}

} // namespace 海中鱼巣::本能根运行初始化内部

namespace 海中鱼巣 {

本能根运行初始化提供者::本能根运行初始化提供者(
    特征概念应用服务 &特征概念, const 存在类数据服务 &存在,
    需求类数据服务 &需求, const 真实自我读取请求 &自我读取请求,
    const 真实自我形成结果 &自我读取结果)
    : 特征概念_(特征概念), 存在_(存在), 需求_(需求),
      自我投影_(自我读取结果.投影.value_or(真实自我投影{})) {
  if (!特征概念_.使用存在服务(存在_) ||
      !需求_.与存在服务同底座(存在_) ||
      !自我读取结果.成功(自我读取请求) || !自我读取结果.投影 ||
      !本能根运行初始化内部::查找唯一特征根(自我投影_))
    throw std::invalid_argument("invalid instinct root runtime dependencies");
}

本能根运行初始化结果_v1 本能根运行初始化提供者::初始化(
    const 本能根运行初始化请求_v1 &请求) noexcept {
  if (已调用_)
    return {本能根运行初始化合同版本_v1,
            本能根运行初始化状态_v1::入口拒绝, 请求, std::nullopt};
  已调用_ = true;

  class 适配 final : public 本能根运行初始化内部::本能根运行初始化调用端口 {
  public:
    适配(特征概念应用服务 &f, 需求类数据服务 &d) : f_(f), d_(d) {}
    本能根I64实际F结果 形成或读取实际F(
        const 本能根I64实际F请求 &r) noexcept override {
      return f_.形成或读取本能根I64实际F(r);
    }
    本能根材料结果
    建立或读取根材料(const 本能根材料请求 &r) override {
      return d_.建立或读取本能根材料(r);
    }
    需求当前事实代次核验结果
    核验当前事实代次(std::uint64_t g) const noexcept override {
      return d_.核验当前事实代次(g);
    }
  private:
    特征概念应用服务 &f_;
    需求类数据服务 &d_;
  } 端口{特征概念_, 需求_};
  return 本能根运行初始化内部::执行组合(端口, 自我投影_, 请求);
}

} // namespace 海中鱼巣
