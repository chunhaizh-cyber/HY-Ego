#include "初始化.二次关系概念结构.h"

#include <algorithm>
#include <array>
#include <set>
#include <stdexcept>

namespace 海中鱼巣 {
namespace {

using S = 二次关系数据状态;
using Key = L1所有者范围写集本地键;
using Ref = L1所有者范围事实引用;

struct 初始化失败 final { S 状态; };

L1所有者范围当前读取结果 读取任一当前事实(
    const L1事实基座服务& l1,稳定编码 id) {
  const L1所有者范围事实读取请求 request{L1所有者范围CRUD合同版本,id};
  const std::array reads{l1.读取所有者范围当前节点(request),
      l1.读取所有者范围当前关系(request),l1.读取所有者范围当前值(request)};
  std::optional<L1所有者范围当前读取结果> hit;
  for(const auto&read:reads){
    if(read.状态==L1所有者范围读取状态::成功){if(hit)return {};
      hit=read;
    }else if(read.状态!=L1所有者范围读取状态::未找到)return read;
  }
  return hit?*hit:reads.front();
}

bool 已发布纯概念结构(const L1事实基座服务 &l1,
                      const L1所有者范围写端口 &port,
                      const 纯概念结构交付_v2 &pure,
                      std::uint64_t g) {
  if (pure.版本 != 2 || !g || !port.有效() || !port.绑定于(l1))
    return false;
  const auto &t = pure.类型;
  const std::array<稳定编码, 15> ids{
      pure.格式锚点, pure.概念族锚点, t.类型登记, t.概念族成员,
      t.格式版本, t.概念类别, t.定义成员, t.定义种类, t.定义特征类型,
      t.定义模板, t.I64域, t.通用规则, t.直接上位, t.生命周期,
      t.存在概念使用};
  std::set<std::uint64_t> unique;
  for (std::size_t i = 0; i != ids.size(); ++i) {
    if (!有效(ids[i]) || !unique.insert(ids[i].值).second) return false;
    const auto read = 读取任一当前事实(l1,ids[i]);
    if (read.状态 != L1所有者范围读取状态::成功 ||
        read.读取事实代次 != g || !read.事实)
      return false;
    const auto *node = std::get_if<L1所有者范围节点事实>(&*read.事实);
    std::optional<L1所有者范围值表示种类> representation;
    const auto position = i + 1;
    if (position == 5 || position == 6 || position == 8 ||
        position == 12 || position == 14)
      representation = L1所有者范围值表示种类::I64;
    else if (position == 11)
      representation = L1所有者范围值表示种类::U64组;
    if (!node || node->写入所有者 != port.所有者身份() ||
        node->种类 !=
            (representation ? 节点种类::属性类型 : 节点种类::普通) ||
        node->属性类型表示 != representation)
      return false;
  }
  const auto registrations = l1.读取所有者范围当前源关系组(
      {L1所有者范围CRUD合同版本, pure.格式锚点, t.类型登记});
  if (registrations.状态 != L1所有者范围读取状态::成功 ||
      registrations.读取事实代次 != g ||
      registrations.关系组.size() < 13)
    return false;
  std::array<bool, 13> seen{};
  for (const auto &edge : registrations.关系组) {
    if (edge.角色或顺序 < 1 || edge.角色或顺序 > 13) continue;
    const auto slot = static_cast<std::size_t>(edge.角色或顺序 - 1);
    if (seen[slot] || edge.写入所有者 != port.所有者身份() ||
        edge.源节点 != pure.格式锚点 ||
        edge.目标节点 != ids[slot + 2] ||
        edge.关系类型节点 != t.类型登记)
      return false;
    seen[slot] = true;
  }
  if (std::ranges::any_of(seen, [](bool value) { return !value; }))
    return false;
  const L1所有者范围所属节点当前完整值组读取请求_v2 valuesRequest{
      L1所有者范围所属节点当前完整值组读取合同版本_v2,
      port.所有者身份(), pure.格式锚点, g};
  const auto values = l1.读取所有者范围所属节点当前完整值组(valuesRequest);
  if (values.状态 != L1所有者范围所属节点当前完整值组读取状态_v2::成功 ||
      values.合同版本 != valuesRequest.合同版本 ||
      values.所有者 != valuesRequest.所有者 ||
      values.所属节点 != valuesRequest.所属节点 ||
      values.期望事实代次 != g || values.读取事实代次 != g)
    return false;
  std::size_t formatCount = 0;
  for (const auto &value : values.载荷) {
    if (value.属性类型节点 != t.格式版本) continue;
    ++formatCount;
    if (value.写入所有者 != port.所有者身份() ||
        value.所属节点 != pure.格式锚点 ||
        value.来源节点 != pure.格式锚点 ||
        !std::holds_alternative<std::int64_t>(value.材料) ||
        std::get<std::int64_t>(value.材料) != 2)
      return false;
  }
  return formatCount == 1;
}

S 映射读取(L1所有者范围读取状态 v) noexcept {
  switch (v) {
  case L1所有者范围读取状态::未找到: return S::未找到;
  case L1所有者范围读取状态::事实代次漂移: return S::事实代次漂移;
  case L1所有者范围读取状态::资源失败: return S::资源失败;
  case L1所有者范围读取状态::入口拒绝: return S::入口拒绝;
  default: return S::内部不一致;
  }
}

S 映射写入(L1所有者范围写入状态 v) noexcept {
  switch (v) {
  case L1所有者范围写入状态::事实代次漂移: return S::事实代次漂移;
  case L1所有者范围写入状态::幂等冲突: return S::幂等冲突;
  case L1所有者范围写入状态::引用冲突:
  case L1所有者范围写入状态::未找到: return S::引用冲突;
  case L1所有者范围写入状态::资源失败: return S::资源失败;
  case L1所有者范围写入状态::入口拒绝: return S::入口拒绝;
  default: return S::发布未知;
  }
}

std::array<稳定编码 *, 22> 类型指针(二次关系结构类型 &t) noexcept {
  return {&t.规范化规则归属, &t.规则版本, &t.定义种类, &t.定义格式,
          &t.域掩码, &t.输出角色, &t.固定K, &t.约束成员, &t.约束FC,
          &t.约束EC, &t.合取成员, &t.子RC, &t.来源成员, &t.来源F,
          &t.来源B, &t.来源概念, &t.用途成员, &t.用途目标,
          &t.用途业务依据, &t.用途业务标识, &t.用途角色, &t.用途时间};
}

std::array<稳定编码, 22> 类型值(const 二次关系结构类型 &t) noexcept {
  return {t.规范化规则归属, t.规则版本, t.定义种类, t.定义格式,
          t.域掩码, t.输出角色, t.固定K, t.约束成员, t.约束FC,
          t.约束EC, t.合取成员, t.子RC, t.来源成员, t.来源F, t.来源B,
          t.来源概念, t.用途成员, t.用途目标,
          t.用途业务依据, t.用途业务标识, t.用途角色, t.用途时间};
}

bool 属性类型位置(std::size_t i,
                    L1所有者范围值表示种类 &kind) noexcept {
  switch (i + 1) {
  case 2: case 3: case 4: case 5: case 6: case 21: case 22:
    kind = L1所有者范围值表示种类::I64; return true;
  case 20:
    kind = L1所有者范围值表示种类::U64组; return true;
  default: return false;
  }
}

L1所有者范围写集请求 形成写集(const 二次关系初始化请求 &r) {
  const auto &p = r.纯概念结构.类型;
  L1所有者范围写集请求 w{L1所有者范围CRUD合同版本, r.G0,
                              r.幂等身份};
  w.节点.push_back({Key{1}, 节点种类::普通, std::nullopt});
  for (std::size_t i = 0; i != 22; ++i) {
    L1所有者范围值表示种类 kind{};
    const bool attribute = 属性类型位置(i, kind);
    w.节点.push_back({Key{static_cast<std::uint32_t>(i + 2)},
                      attribute ? 节点种类::属性类型 : 节点种类::普通,
                      attribute ? std::optional{kind} : std::nullopt});
  }
  w.节点.push_back({Key{24}, 节点种类::普通, std::nullopt});
  w.值.push_back({Key{25}, Ref{Key{1}}, Ref{p.格式版本}, std::int64_t{1},
                    Ref{Key{1}}});
  w.值.push_back({Key{26}, Ref{Key{24}}, Ref{p.格式版本}, std::int64_t{1},
                    Ref{Key{24}}});
  w.值.push_back({Key{27}, Ref{Key{24}}, Ref{Key{3}}, std::int64_t{1},
                    Ref{Key{24}}});
  w.属性槽变更.push_back({Ref{Key{1}}, Ref{p.格式版本}, Key{25}});
  w.属性槽变更.push_back({Ref{Key{24}}, Ref{p.格式版本}, Key{26}});
  w.属性槽变更.push_back({Ref{Key{24}}, Ref{Key{3}}, Key{27}});
  w.关系.push_back({Key{28}, Ref{r.纯概念结构.格式锚点}, Ref{Key{1}},
                      Ref{p.类型登记}, 4242});
  for (std::uint32_t i = 0; i != 22; ++i)
    w.关系.push_back({Key{29 + i}, Ref{Key{1}}, Ref{Key{2 + i}},
                      Ref{p.类型登记}, static_cast<std::int64_t>(i + 1)});
  w.关系.push_back({Key{51}, Ref{Key{1}}, Ref{Key{24}}, Ref{Key{2}}, 1});
  return w;
}

L1所有者范围事实副本 读事实(const L1事实基座服务 &l1, 稳定编码 id,
                                  std::uint64_t g) {
  const auto r = 读取任一当前事实(l1,id);
  if (r.状态 != L1所有者范围读取状态::成功 || r.读取事实代次 != g ||
      !r.事实)
    throw 初始化失败{映射读取(r.状态)};
  return *r.事实;
}

std::vector<L1所有者范围关系事实> 读关系(
    const L1事实基座服务 &l1, L1结构所有者身份 owner,
    稳定编码 source, 稳定编码 type,
    std::uint64_t g) {
  const auto r = l1.读取所有者范围当前源关系组(
      {L1所有者范围CRUD合同版本, source, type});
  if (r.状态 != L1所有者范围读取状态::成功 || r.读取事实代次 != g)
    throw 初始化失败{映射读取(r.状态)};
  for (const auto &edge : r.关系组)
    if (edge.写入所有者 != owner)
      throw 初始化失败{S::内部不一致};
  return r.关系组;
}

std::vector<L1所有者范围值事实> 读属性(
    const L1事实基座服务 &l1, L1结构所有者身份 owner,
    稳定编码 node, std::uint64_t g) {
  const L1所有者范围所属节点当前完整值组读取请求_v2 request{
      L1所有者范围所属节点当前完整值组读取合同版本_v2, owner, node, g};
  const auto r = l1.读取所有者范围所属节点当前完整值组(request);
  if (r.状态 != L1所有者范围所属节点当前完整值组读取状态_v2::成功 ||
      r.合同版本 != request.合同版本 || r.所有者 != owner ||
      r.所属节点 != node || r.期望事实代次 != g || r.读取事实代次 != g)
    throw 初始化失败{S::内部不一致};
  return r.载荷;
}

二次关系结构交付 核验并形成交付(
    const L1事实基座服务 &l1, const L1所有者范围写端口 &port,
    const 纯概念结构交付_v2 &pure, 稳定编码 anchor, std::uint64_t g) {
  if (!有效(anchor)) throw 初始化失败{S::内部不一致};
  const auto owner = port.所有者身份();
  const auto anchorRaw = 读事实(l1, anchor, g);
  const auto *anchorNode = std::get_if<L1所有者范围节点事实>(&anchorRaw);
  if (!anchorNode || anchorNode->写入所有者 != owner ||
      anchorNode->种类 != 节点种类::普通 || anchorNode->属性类型表示)
    throw 初始化失败{S::内部不一致};

  二次关系结构交付 out;
  out.锚点 = anchor;
  auto edges = 读关系(l1, owner, anchor, pure.类型.类型登记, g);
  std::array<bool, 22> seen{};
  auto ptrs = 类型指针(out.类型);
  for (const auto &e : edges) {
    if (e.角色或顺序 < 1 || e.角色或顺序 > 22)
      throw 初始化失败{S::内部不一致};
    const auto i = static_cast<std::size_t>(e.角色或顺序 - 1);
    if (seen[i] || e.写入所有者 != owner || e.源节点 != anchor ||
        e.关系类型节点 != pure.类型.类型登记)
      throw 初始化失败{S::内部不一致};
    seen[i] = true;
    *ptrs[i] = e.目标节点;
  }
  if (edges.size() != 22) throw 初始化失败{S::内部不一致};

  std::set<std::uint64_t> unique{anchor.值};
  const auto ids = 类型值(out.类型);
  for (std::size_t i = 0; i != ids.size(); ++i) {
    if (!有效(ids[i]) || !unique.insert(ids[i].值).second)
      throw 初始化失败{S::内部不一致};
    const auto raw = 读事实(l1, ids[i], g);
    const auto *node = std::get_if<L1所有者范围节点事实>(&raw);
    L1所有者范围值表示种类 kind{};
    const bool attribute = 属性类型位置(i, kind);
    if (!node || node->写入所有者 != owner ||
        node->种类 != (attribute ? 节点种类::属性类型 : 节点种类::普通) ||
        node->属性类型表示 !=
            (attribute ? std::optional{kind} : std::nullopt))
      throw 初始化失败{S::内部不一致};
  }

  const auto ruleEdges = 读关系(l1, owner, anchor, out.类型.规范化规则归属, g);
  if (ruleEdges.size() != 1 || ruleEdges[0].写入所有者 != owner ||
      ruleEdges[0].源节点 != anchor || ruleEdges[0].角色或顺序 != 1)
    throw 初始化失败{S::内部不一致};
  out.规范化规则 = 概念树规则身份{ruleEdges[0].目标节点};
  if (!有效(out.规范化规则.值) ||
      !unique.insert(out.规范化规则.值.值).second)
    throw 初始化失败{S::内部不一致};
  const auto ruleRaw = 读事实(l1, out.规范化规则.值, g);
  const auto *rule = std::get_if<L1所有者范围节点事实>(&ruleRaw);
  if (!rule || rule->写入所有者 != owner ||
      rule->种类 != 节点种类::普通 || rule->属性类型表示)
    throw 初始化失败{S::内部不一致};

  const auto checkI64 = [&](稳定编码 node, 稳定编码 type) {
    const auto values = 读属性(l1, owner, node, g);
    std::size_t count = 0;
    for (const auto &v : values)
      if (v.属性类型节点 == type) {
        ++count;
        if (v.写入所有者 != owner || v.所属节点 != node ||
            v.来源节点 != node ||
            !std::holds_alternative<std::int64_t>(v.材料) ||
            std::get<std::int64_t>(v.材料) != 1)
          throw 初始化失败{S::内部不一致};
      }
    if (count != 1) throw 初始化失败{S::内部不一致};
  };
  checkI64(anchor, pure.类型.格式版本);
  checkI64(out.规范化规则.值, pure.类型.格式版本);
  checkI64(out.规范化规则.值, out.类型.规则版本);
  return out;
}

std::optional<二次关系结构交付> 定位既有(
    const L1事实基座服务 &l1, const L1所有者范围写端口 &port,
    const 纯概念结构交付_v2 &pure, std::uint64_t g) {
  const auto edges = 读关系(l1, port.所有者身份(), pure.格式锚点,
                            pure.类型.类型登记, g);
  std::optional<稳定编码> anchor;
  for (const auto &e : edges) {
    if (e.角色或顺序 != 4242) continue;
    if (e.写入所有者 != port.所有者身份() ||
        e.源节点 != pure.格式锚点 ||
        e.关系类型节点 != pure.类型.类型登记 ||
        !有效(e.目标节点))
      throw 初始化失败{S::内部不一致};
    if (anchor) throw 初始化失败{S::内部不一致};
    anchor = e.目标节点;
  }
  if (!anchor) return std::nullopt;
  return 核验并形成交付(l1, port, pure, *anchor, g);
}

二次关系发布见证 形成见证(const L1所有者范围写入结果 &r,
                              bool replay) {
  二次关系发布见证 out;
  out.幂等身份 = r.写入幂等身份.值;
  out.发布H = r.事实代次;
  out.已确认发布 = r.状态 == L1所有者范围写入状态::成功 ||
                       r.状态 == L1所有者范围写入状态::精确重复;
  out.精确重放 = replay;
  out.新编码映射.reserve(r.新编码映射.size());
  for (const auto &[key, id] : r.新编码映射)
    out.新编码映射.emplace_back(key.值, id);
  std::sort(out.新编码映射.begin(), out.新编码映射.end(),
            [](const auto &a, const auto &b) { return a.first < b.first; });
  return out;
}

} // namespace

二次关系初始化结果 初始化二次关系结构(
    const L1事实基座服务 &l1, L1所有者范围写端口 &port,
    const 二次关系初始化请求 &r) noexcept {
  二次关系初始化结果 out;
  bool dispatched = false;
  try {
    if (r.版本 != 1 || !r.G0 || r.G0 == UINT64_MAX ||
        !有效(r.幂等身份) || r.最大首次材料项数 < 51 ||
        !port.有效() || !port.绑定于(l1) ||
        r.纯概念结构.版本 != 2 || !有效(r.纯概念结构.格式锚点) ||
        !有效(r.纯概念结构.类型.类型登记) ||
        !有效(r.纯概念结构.类型.格式版本))
      throw 初始化失败{S::入口拒绝};

    const auto first = port.读取首次写入材料(
        {L1所有者范围首次写入读取合同版本, r.幂等身份});
    out.Gread = first.读取事实代次;
    if (first.合同版本 != L1所有者范围首次写入读取合同版本 ||
        first.所有者 != port.所有者身份() ||
        first.写入幂等身份 != r.幂等身份)
      throw 初始化失败{S::内部不一致};
    const auto expected = 形成写集(r);
    const bool replay = first.状态 == L1所有者范围读取状态::成功;
    const auto pureGeneration = replay ? first.读取事实代次 : r.G0;
    if (!已发布纯概念结构(l1, port, r.纯概念结构, pureGeneration))
      throw 初始化失败{S::格式不支持};
    if (replay) {
      if (!first.首次规范化写集 || *first.首次规范化写集 != expected)
        throw 初始化失败{S::幂等冲突};
      if (!first.首次写入结果 ||
          first.首次写入结果->状态 != L1所有者范围写入状态::成功 ||
          !first.首次写入结果->是否形成内存权威发布 ||
          first.首次写入结果->事实代次 != r.G0 + 1)
        throw 初始化失败{S::发布未知};
    } else if (first.状态 == L1所有者范围读取状态::未找到) {
      if (first.读取事实代次 != r.G0 || first.首次规范化写集 ||
          first.首次写入结果)
        throw 初始化失败{first.读取事实代次 != r.G0
                               ? S::事实代次漂移
                               : S::内部不一致};
      if (auto existing = 定位既有(l1, port, r.纯概念结构, r.G0)) {
        out.状态 = S::已复用;
        out.Gread = r.G0;
        out.交付 = *existing;
        return out;
      }
    } else {
      throw 初始化失败{映射读取(first.状态)};
    }

    dispatched = true;
    const auto saved = port.提交所有者范围中性写集(expected);
    out.Gread = saved.事实代次;
    const auto &published = replay ? *first.首次写入结果 : saved;
    out.正式回执 = 形成见证(published, replay);
    const auto wanted = replay ? L1所有者范围写入状态::精确重复
                               : L1所有者范围写入状态::成功;
    if (saved.状态 != wanted || saved.合同版本 != L1所有者范围CRUD合同版本 ||
        saved.所有者 != port.所有者身份() ||
        saved.写入幂等身份 != r.幂等身份 || saved.事实代次 != r.G0 + 1 ||
        (!replay && !saved.是否形成内存权威发布))
      throw 初始化失败{映射写入(saved.状态)};
    if (published.新编码映射.size() != 51)
      throw 初始化失败{S::发布未知};
    稳定编码 anchor{};
    for (const auto &[key, id] : published.新编码映射)
      if (key.值 == 1) {
        if (有效(anchor)) throw 初始化失败{S::内部不一致};
        anchor = id;
      }
    const auto current = l1.读取中性当前事实代次({L1中性CRUD合同版本});
    if (current.状态 != L1中性读取状态::成功 ||
        current.事实代次 < published.事实代次)
      throw 初始化失败{S::发布未知};
    out.Gread = current.事实代次;
    const auto located = 定位既有(l1, port, r.纯概念结构, out.Gread);
    if (!located || located->锚点 != anchor)
      throw 初始化失败{S::内部不一致};
    out.交付 = *located;
    out.状态 = replay ? S::精确重放 : S::已创建;
    if (!out.成功()) throw 初始化失败{S::内部不一致};
  } catch (const 初始化失败 &e) {
    out.状态 = dispatched && e.状态 != S::事实代次漂移 &&
                           e.状态 != S::幂等冲突
                       ? S::发布未知
                       : e.状态;
    out.交付.reset();
    if (out.状态 != S::发布未知) {
      out.正式回执.reset();
    }
  } catch (const std::bad_alloc &) {
    out.状态 = dispatched ? S::发布未知 : S::资源失败;
    out.交付.reset();
    if (!dispatched) out.正式回执.reset();
  } catch (...) {
    out.状态 = dispatched ? S::发布未知 : S::内部不一致;
    out.交付.reset();
    if (!dispatched) out.正式回执.reset();
  }
  return out;
}

} // namespace 海中鱼巣
