#include "数据服务.场景类.h"
#include <system_error>
#include <unordered_map>

namespace 海中鱼巣 {

namespace {
bool L1当前引用读取成功(const L1节点当前完整引用读取结果_v2& 结果,
                       const L1节点当前完整引用读取请求_v2& 请求) noexcept {
  return 结果.状态 == L1节点当前完整引用读取状态_v2::成功
      && 结果.合同版本 == L1节点当前完整引用读取合同版本
      && 结果.节点 == 请求.节点
      && 结果.期望事实代次 == 请求.期望事实代次
      && 结果.读取事实代次 == 请求.期望事实代次;
}
}

场景特征组织扩展登记结果 场景类数据服务::登记特征组织扩展(const L1事实基座服务 &l1,
                                 L1所有者范围写端口 &port,
                                 const 场景特征组织扩展登记请求 &r) {
  场景特征组织扩展登记结果 out;
  if (r.版本 != 1 || !r.G0 || r.G0 == UINT64_MAX || !port.绑定于(l1))
    return out;
  bool commitStarted = false;
  std::uint64_t published = 0;
  const auto key = 场景特征组织扩展登记固定幂等身份;
  try {
    L1所有者范围写集请求 ws{L1所有者范围CRUD合同版本, r.G0, key};
    ws.节点 = {{{1}, 节点种类::普通, std::nullopt}};
    auto first = port.读取首次写入材料({L1所有者范围首次写入读取合同版本, key});
    if (first.合同版本 != L1所有者范围首次写入读取合同版本 ||
        first.所有者 != port.所有者身份() || first.写入幂等身份 != key) {
      out.状态 = 场景角色数据状态::内部不一致;
      out.Gread = first.读取事实代次;
      return out;
    }
    const bool replay = first.状态 == L1所有者范围读取状态::成功;
    if (replay) {
      if (!first.首次规范化写集 || !写集相同(*first.首次规范化写集, ws) ||
          !first.首次写入结果 ||
          first.首次写入结果->状态 != L1所有者范围写入状态::成功 ||
          first.首次写入结果->合同版本 != L1所有者范围CRUD合同版本 ||
          first.首次写入结果->所有者 != port.所有者身份() ||
          first.首次写入结果->写入幂等身份 != key ||
          first.首次写入结果->事实代次 != r.G0 + 1 ||
          !first.首次写入结果->是否形成内存权威发布 ||
          first.首次写入结果->重试边界 != L1所有者范围重试边界::不适用 ||
          !写入映射完整(*first.首次写入结果, ws)) {
        out.状态 = first.首次规范化写集 && !写集相同(*first.首次规范化写集, ws)
                       ? 场景角色数据状态::幂等冲突
                       : 场景角色数据状态::内部不一致;
        out.Gread = first.读取事实代次;
        return out;
      }
    } else if (first.状态 != L1所有者范围读取状态::未找到) {
      out.状态 = first.状态 == L1所有者范围读取状态::资源失败
                     ? 场景角色数据状态::资源失败
                     : 场景角色数据状态::内部不一致;
      out.Gread = first.读取事实代次;
      return out;
    } else if (first.读取事实代次 != r.G0 || first.首次规范化写集 ||
               first.首次写入结果) {
      out.状态 = first.读取事实代次 != r.G0 ? 场景角色数据状态::事实代次漂移
                                            : 场景角色数据状态::内部不一致;
      out.Gread = first.读取事实代次;
      return out;
    }
    commitStarted = true;
    const auto saved = port.提交所有者范围中性写集(ws);
    published = saved.事实代次;
    if (saved.状态 != L1所有者范围写入状态::成功 &&
        saved.状态 != L1所有者范围写入状态::精确重复) {
      out.状态 = 映射场景写入状态(saved, 场景角色数据状态::已登记,
                                  port.所有者身份(), ws);
      out.Gread = saved.事实代次;
      out.首次发布代次 =
          out.状态 == 场景角色数据状态::已可能发布 ? saved.事实代次 : 0;
      return out;
    }
    if (saved.合同版本 != L1所有者范围CRUD合同版本 ||
        saved.所有者 != port.所有者身份() || saved.写入幂等身份 != key ||
        saved.事实代次 != r.G0 + 1 || !写入映射完整(saved, ws)) {
      out.状态 = 场景角色数据状态::已可能发布;
      out.Gread = saved.事实代次;
      out.首次发布代次 = saved.事实代次;
      return out;
    }
    if (saved.状态 == L1所有者范围写入状态::精确重复) {
      const auto won =
          port.读取首次写入材料({L1所有者范围首次写入读取合同版本, key});
      if (won.状态 != L1所有者范围读取状态::成功 || !won.首次规范化写集 ||
          !写集相同(*won.首次规范化写集, ws) || !won.首次写入结果 ||
          won.首次写入结果->状态 != L1所有者范围写入状态::成功 ||
          won.首次写入结果->新编码映射 != saved.新编码映射) {
        out.状态 = 场景角色数据状态::已可能发布;
        out.Gread = won.读取事实代次;
        out.首次发布代次 = saved.事实代次;
        return out;
      }
    }
    const auto id = 映射编码(saved, {1});
    if (!id) {
      out.状态 = 场景角色数据状态::已可能发布;
      out.首次发布代次 = saved.事实代次;
      return out;
    }
    const auto guard = l1.读取中性当前事实代次({L1中性CRUD合同版本});
    out.Gread = guard.事实代次;
    out.首次发布代次 = saved.事实代次;
    if (guard.状态 != L1中性读取状态::成功 ||
        guard.合同版本 != L1中性CRUD合同版本 ||
        guard.事实代次 < saved.事实代次) {
      out.状态 = 场景角色数据状态::已可能发布;
      return out;
    }
    const auto node = l1.读取所有者范围当前事实(
        {L1所有者范围当前事实读取合同版本_v2,port.所有者身份(),*id,out.Gread});
    const auto *n =
        node.载荷 ? std::get_if<L1所有者范围节点事实>(&*node.载荷) : nullptr;
    if (node.状态 != L1所有者范围当前事实读取状态_v2::成功 ||
        node.合同版本 != L1所有者范围当前事实读取合同版本_v2 ||
        node.读取事实代次 != out.Gread || node.事实编码 != *id || !n ||
        n->写入所有者 != port.所有者身份() || n->种类 != 节点种类::普通 ||
        n->属性类型表示 || !n->当前属性.empty() ||
        n->创建事实代次 != saved.事实代次) {
      out.状态 = 场景角色数据状态::已可能发布;
      return out;
    }
    const auto last = l1.读取中性当前事实代次({L1中性CRUD合同版本});
    if (last.状态 != L1中性读取状态::成功 ||
        last.合同版本 != L1中性CRUD合同版本 || last.事实代次 != out.Gread) {
      out.状态 = 场景角色数据状态::已可能发布;
      out.Gread = last.事实代次;
      return out;
    }
    out.交付 = 场景特征组织扩展结构交付{*id};
    out.状态 = saved.状态 == L1所有者范围写入状态::成功
                   ? 场景角色数据状态::已登记
                   : 场景角色数据状态::精确重复;
    return out;
  } catch (const std::bad_alloc &) {
    out.状态 = commitStarted ? 场景角色数据状态::已可能发布
                             : 场景角色数据状态::资源失败;
    out.首次发布代次 = commitStarted ? published : 0;
    return out;
  } catch (...) {
    out.状态 = commitStarted ? 场景角色数据状态::已可能发布
                             : 场景角色数据状态::内部不一致;
    out.首次发布代次 = commitStarted ? published : 0;
    return out;
  }
}

场景特征组织写结果_v1 场景类数据服务::组织特征实例(const 场景特征组织请求 &r) {
  if (r.版本 != 1 || !r.G0 || r.G0 == UINT64_MAX ||
      !普通幂等身份有效(r.幂等身份) || !有效(r.场景) || !有效(r.组织父) ||
      !有效(r.特征) || r.场景 == r.组织父 || r.场景 == r.特征.编码 ||
      r.组织父 == r.特征.编码 || !r.最大路径长度 || r.最大路径长度 > 4096)
    return {};
  bool commitStarted = false;
  std::uint64_t published = 0, confirmed = 0;
  try {
    const auto expected = 形成特征组织写集(r.G0, r.幂等身份, r.组织父, r.特征);
    const auto first =
        port_.读取首次写入材料({L1所有者范围首次写入读取合同版本, r.幂等身份});
    if (first.合同版本 != L1所有者范围首次写入读取合同版本 ||
        first.所有者 != owner_ || first.写入幂等身份 != r.幂等身份)
      return {1, 场景角色数据状态::内部不一致, first.读取事实代次, 0,
              std::nullopt};
    L1所有者范围写入结果 saved;
    场景角色数据状态 status = 场景角色数据状态::内部不一致;
    if (first.状态 == L1所有者范围读取状态::成功) {
      if (!first.首次规范化写集 || !写集相同(*first.首次规范化写集, expected))
        return {1, 场景角色数据状态::幂等冲突, first.读取事实代次, 0,
                std::nullopt};
      if (!first.首次写入结果 ||
          first.首次写入结果->状态 != L1所有者范围写入状态::成功 ||
          !写入映射完整(*first.首次写入结果, expected))
        return {1, 场景角色数据状态::内部不一致, first.读取事实代次, 0,
                std::nullopt};
      commitStarted = true;
      saved = 串行提交(*first.首次规范化写集);
      published = saved.事实代次;
      if (saved.状态 != L1所有者范围写入状态::精确重复 ||
          !写入头完整(saved, r.幂等身份, r.G0) ||
          !写入映射完整(saved, expected) ||
          saved.新编码映射 != first.首次写入结果->新编码映射)
        return {1, 场景角色数据状态::已可能发布, saved.事实代次, saved.事实代次,
                std::nullopt};
      status = 场景角色数据状态::精确重复;
    } else {
      if (first.状态 != L1所有者范围读取状态::未找到)
        return {1,
                first.状态 == L1所有者范围读取状态::资源失败
                    ? 场景角色数据状态::资源失败
                    : 场景角色数据状态::内部不一致,
                first.读取事实代次, 0, std::nullopt};
      if (first.读取事实代次 != r.G0)
        return {1, 场景角色数据状态::事实代次漂移, first.读取事实代次, 0,
                std::nullopt};
      if (first.首次规范化写集 || first.首次写入结果)
        return {1, 场景角色数据状态::内部不一致, first.读取事实代次, 0,
                std::nullopt};
      std::uint64_t preflightGread = 0;
      const auto preflight = 核验特征组织前置(r, preflightGread);
      if (preflight != 场景角色数据状态::已读取)
        return {1, preflight, preflightGread, 0, std::nullopt};
      commitStarted = true;
      saved = 串行提交(expected);
      published = saved.事实代次;
      status =
          映射场景写入状态(saved, 场景角色数据状态::已组织, owner_, expected);
      if (status == 场景角色数据状态::精确重复 &&
          !竞争精确重复材料完整(saved, expected))
        return {1, 场景角色数据状态::已可能发布, saved.事实代次, saved.事实代次,
                std::nullopt};
      if (status != 场景角色数据状态::已组织 &&
          status != 场景角色数据状态::精确重复)
        return {1, status, saved.事实代次,
                status == 场景角色数据状态::已可能发布 ? saved.事实代次 : 0,
                std::nullopt};
      if (!写入头完整(saved, r.幂等身份, r.G0) ||
          !写入映射完整(saved, expected))
        return {1,
                saved.是否形成内存权威发布 ||
                        saved.状态 == L1所有者范围写入状态::精确重复
                    ? 场景角色数据状态::已可能发布
                    : 场景角色数据状态::内部不一致,
                saved.事实代次, saved.事实代次, std::nullopt};
    }
    auto result = 读回特征组织(r, status, saved);
    confirmed = result.Gread;
    return result;
  } catch (const std::bad_alloc &) {
    return {1,
            commitStarted ? 场景角色数据状态::已可能发布
                          : 场景角色数据状态::资源失败,
            confirmed ? confirmed : published, commitStarted ? published : 0,
            std::nullopt};
  } catch (...) {
    return {1,
            commitStarted ? 场景角色数据状态::已可能发布
                          : 场景角色数据状态::内部不一致,
            confirmed ? confirmed : published, commitStarted ? published : 0,
            std::nullopt};
  }
}

场景特征组织当前读取结果
场景类数据服务::读取当前特征组织(const 场景特征组织当前读取请求 &r) const {
  if (r.版本 != 1 || !r.G0 || !有效(r.特征) ||
      !r.最大路径长度 || r.最大路径长度 > 4096)
    return {};
  std::uint64_t confirmed = 0;
  try {
    auto first = 核验代次(r.G0);
    if (!first.成功())
      return {1, first.状态, first.Gread, std::nullopt};
    confirmed = first.Gread;
    auto result = 读特征路径(r.G0, r.特征, r.最大路径长度);
    auto last = 核验代次(r.G0);
    if (!last.成功())
      return {1, last.状态, last.Gread, std::nullopt};
    return result;
  } catch (const std::bad_alloc &) {
    return {1, 场景角色数据状态::资源失败, confirmed, std::nullopt};
  } catch (...) {
    return {1, 场景角色数据状态::内部不一致, confirmed, std::nullopt};
  }
}

bool 场景类数据服务::扩展登记材料匹配() const {
  const auto key = 场景特征组织扩展登记固定幂等身份;
  const auto f =
      port_.读取首次写入材料({L1所有者范围首次写入读取合同版本, key});
  if (f.状态 != L1所有者范围读取状态::成功 ||
      f.合同版本 != L1所有者范围首次写入读取合同版本 || f.所有者 != owner_ ||
      f.写入幂等身份 != key || !f.首次规范化写集 || !f.首次写入结果)
    return false;
  const auto &ws = *f.首次规范化写集;
  if (ws.合同版本 != L1所有者范围CRUD合同版本 || ws.写入幂等身份 != key ||
      !ws.期望事实代次 || ws.期望事实代次 == UINT64_MAX ||
      ws.节点.size() != 1 || !ws.关系.empty() || !ws.值.empty() ||
      !ws.属性槽变更.empty() || !ws.退出事实.empty())
    return false;
  const auto &n = ws.节点.front();
  if (n.本地键.值 != 1 || n.种类 != 节点种类::普通 || n.属性类型表示)
    return false;
  const auto &first = *f.首次写入结果;
  if (first.状态 != L1所有者范围写入状态::成功 ||
      first.合同版本 != L1所有者范围CRUD合同版本 || first.所有者 != owner_ ||
      first.写入幂等身份 != key || first.事实代次 != ws.期望事实代次 + 1 ||
      !first.是否形成内存权威发布 ||
      first.重试边界 != L1所有者范围重试边界::不适用 ||
      first.新编码映射.size() != 1)
    return false;
  const auto id = 映射编码(first, {1});
  if (!id || *id != featureLayout_.特征组织关系类型)
    return false;
  const auto guard = 读取当前代次();
  if (!guard.成功() || guard.Gread < first.事实代次)
    return false;
  const auto node = 读节点(*id, guard.Gread);
  return node.成功() && node.事实->写入所有者 == owner_ &&
         node.事实->种类 == 节点种类::普通 && !node.事实->属性类型表示 &&
         node.事实->当前属性.empty() &&
         node.事实->创建事实代次 == first.事实代次;
}

场景特征组织当前读取结果 场景类数据服务::读特征路径(std::uint64_t g,
                                                 特征信息身份 feature,
                                                 std::uint64_t budget,bool* missingKnown) const {
  if(missingKnown)*missingKnown=false;
  场景特征组织事实 fact;
  fact.Gread = g;
  fact.角色 = 场景根角色::特征;
  fact.实例 = feature.编码;
  std::vector<L1所有者范围关系事实> reverse;
  std::unordered_set<std::uint64_t> visited;
  auto child = feature.编码;
  for (std::uint64_t depth = 0;; ++depth) {
    if (!visited.insert(child.值).second)
      return {1, 场景角色数据状态::内部不一致, g, std::nullopt};
    auto roots = 查询关系(g, 关系端点方向::目标, child,
                          layout_.根绑定关系类型);
    if (!roots.成功())
      return {1, roots.状态, roots.Gread, std::nullopt};
    if (roots.关系.size() > 4096)
      return {1, 场景角色数据状态::数量预算不足, g, std::nullopt};
    if (!roots.关系.empty()) {
      if (roots.关系.size() != 1 || roots.关系.front().写入所有者 != owner_ ||
          roots.关系.front().目标节点 != child ||
          roots.关系.front().关系类型节点 != layout_.根绑定关系类型 ||
          roots.关系.front().角色或顺序 !=
              static_cast<std::int64_t>(场景根角色::特征))
        return {1, 场景角色数据状态::内部不一致, g, std::nullopt};
      if (reverse.empty())
        return {1, 场景角色数据状态::实例未组织, g, std::nullopt};
      const auto c = roots.关系.front().源节点;
      auto scene = 读角色(g, c);
      if (!scene.成功({2, g, c}))
        return {1, scene.状态, scene.Gread, std::nullopt};
      if (scene.角色->四根[0].根.编码 != child)
        return {1, 场景角色数据状态::内部不一致, g, std::nullopt};
      fact.场景角色 = *scene.角色;
      fact.根 = child;
      break;
    }
    if (depth >= budget)
      return {1, 场景角色数据状态::数量预算不足, g, std::nullopt};
    auto incoming = 查询关系(g, 关系端点方向::目标, child,
                             featureLayout_.特征组织关系类型);
    if (!incoming.成功())
      return {1, incoming.状态, incoming.Gread, std::nullopt};
    if (incoming.关系.size() > 4096)
      return {1, 场景角色数据状态::数量预算不足, g, std::nullopt};
    if (incoming.关系.empty())
      return {1,
              reverse.empty() ? 场景角色数据状态::实例未组织
                              : 场景角色数据状态::内部不一致,
              g, std::nullopt};
    if (incoming.关系.size() != 1)
      return {1, 场景角色数据状态::内部不一致, g, std::nullopt};
    const auto &e = incoming.关系.front();
    if (e.写入所有者 != owner_ ||
        e.关系类型节点 != featureLayout_.特征组织关系类型 ||
        e.角色或顺序 != 1 || e.目标节点 != child || e.创建事实代次 > g)
      return {1, 场景角色数据状态::内部不一致, g, std::nullopt};
    reverse.push_back(e);
    child = e.源节点;
  }
  std::reverse(reverse.begin(), reverse.end());
  auto rootNode = 读节点(fact.根, g);
  if (!rootNode.成功())
    return {1,
            rootNode.状态 == 场景角色数据状态::未找到
                ? 场景角色数据状态::内部不一致
                : rootNode.状态,
            rootNode.Gread, std::nullopt};
  if (rootNode.事实->写入所有者 != owner_ ||
      rootNode.事实->种类 != 节点种类::普通 || rootNode.事实->属性类型表示 ||
      !rootNode.事实->当前属性.empty() || rootNode.事实->创建事实代次 > g)
    return {1, 场景角色数据状态::内部不一致, g, std::nullopt};
  fact.路径节点.push_back(转节点(*rootNode.事实));
  for (const auto &e : reverse) {
    fact.路径边.push_back(转边(e));
    const 存在已知准确特征当前请求 request{1, g, fact.场景角色.场景,
                                           e.目标节点};
    auto f = existence_.确认当前已知准确特征(request);
    if (!f.当前成功(request)) {
      if(missingKnown&&f.状态==存在已知准确特征只读状态::未找到)*missingKnown=true;
      return {1, 存在特征到场景(f.状态), f.Gread, std::nullopt};
    }
    fact.路径节点.push_back(
        {e.目标节点, {f.见证->特征创建事实代次}});
  }
  场景特征组织当前读取结果 result{1, 场景角色数据状态::已读取, g,
                              std::move(fact)};
  return result.成功({1, g, feature, budget})
             ? std::move(result)
             : 场景特征组织当前读取结果{1, 场景角色数据状态::内部不一致, g,
                                    std::nullopt};
}

场景角色数据状态
场景类数据服务::核验当前已知准确特征(const 场景特征组织请求 &r,
                                     std::uint64_t &gread) const {
  const 存在已知准确特征当前请求 request{1, r.G0, r.场景, r.特征.编码};
  auto known = existence_.确认当前已知准确特征(request);
  gread = known.Gread;
  return known.当前成功(request) ? 场景角色数据状态::已读取
                                 : 存在特征到场景(known.状态);
}

场景角色数据状态 场景类数据服务::核验特征组织父(const 场景特征组织请求 &r,
                                                稳定编码 root,
                                                std::uint64_t &gread) const {
  if (r.组织父 != root) {
    auto parentPath = 读特征路径(r.G0, {r.组织父}, r.最大路径长度);
    gread = parentPath.Gread;
    const 场景特征组织当前读取请求 parentPathRequest{
        1, r.G0, {r.组织父}, r.最大路径长度};
    if (!parentPath.成功(parentPathRequest))
      return parentPath.状态;
    if (parentPath.组织->场景角色.场景 != r.场景)
      return 场景角色数据状态::引用冲突;
    if (parentPath.组织->路径边.size() >= r.最大路径长度)
      return 场景角色数据状态::数量预算不足;
  }
  return 场景角色数据状态::已读取;
}

场景角色数据状态 场景类数据服务::核验特征未组织(const 场景特征组织请求 &r,
                                                std::uint64_t &gread) const {
  auto incoming = 查询关系(r.G0, 关系端点方向::目标,
                           r.特征.编码, featureLayout_.特征组织关系类型);
  gread = incoming.Gread;
  if (!incoming.成功())
    return incoming.状态;
  if (incoming.关系.size() > 4096)
    return 场景角色数据状态::数量预算不足;
  if (!incoming.关系.empty())
    return 场景角色数据状态::实例已组织;
  auto outgoing = 查询关系(r.G0, 关系端点方向::源,
                           r.特征.编码, featureLayout_.特征组织关系类型);
  gread = outgoing.Gread;
  if (!outgoing.成功())
    return outgoing.状态;
  if (outgoing.关系.size() > 4096)
    return 场景角色数据状态::数量预算不足;
  if (!outgoing.关系.empty())
    return 场景角色数据状态::引用冲突;
  return 场景角色数据状态::已读取;
}

场景角色数据状态 场景类数据服务::核验特征组织前置(const 场景特征组织请求 &r,
                                                  std::uint64_t &gread) const {
  auto scene = 读角色(r.G0, r.场景);
  gread = scene.Gread;
  if (!scene.成功({2, r.G0, r.场景}))
    return scene.状态;
  auto status = 核验当前已知准确特征(r, gread);
  if (status != 场景角色数据状态::已读取)
    return status;
  status = 核验特征组织父(r, scene.角色->四根[0].根.编码, gread);
  if (status != 场景角色数据状态::已读取)
    return status;
  return 核验特征未组织(r, gread);
}

场景特征组织写结果_v1
场景类数据服务::读回特征组织(const 场景特征组织请求 &r, 场景角色数据状态 status,
                             const L1所有者范围写入结果 &saved) const {
  const auto guard = 读取当前代次();
  if (!guard.成功())
    return {1, 场景角色数据状态::已可能发布, guard.Gread, saved.事实代次,
            std::nullopt};
  const 存在已知准确特征当前请求 knownRequest{1, guard.Gread,
                                              r.场景, r.特征.编码};
  auto known = existence_.确认当前已知准确特征(knownRequest);
  if (!known.当前成功(knownRequest))
    return {1, 场景角色数据状态::已可能发布, known.Gread, saved.事实代次,
            std::nullopt};
  auto path = 读特征路径(guard.Gread, r.特征, r.最大路径长度);
  const 场景特征组织当前读取请求 pathRequest{1, guard.Gread, r.特征,
                                         r.最大路径长度};
  if (!path.成功(pathRequest) || path.组织->场景角色.场景 != r.场景 ||
      path.组织->路径边.back().源 != r.组织父)
    return {1, 场景角色数据状态::已可能发布, path.Gread, saved.事实代次,
            std::nullopt};
  const auto finalGuard = 核验代次(guard.Gread);
  if (!finalGuard.成功())
    return {1, 场景角色数据状态::已可能发布, finalGuard.Gread, saved.事实代次,
            std::nullopt};
  场景实例组织回执_v2 receipt{guard.Gread, r.场景,
                              path.组织->根,
                              r.特征.编码,
                              场景根角色::特征,
                              path.组织->路径边.back()};
  return {1, status, guard.Gread, saved.事实代次, std::move(receipt)};
}

场景角色数据状态
场景类数据服务::存在特征到场景(存在已知准确特征只读状态 s) noexcept {
  switch (s) {
  case 存在已知准确特征只读状态::未找到:
    return 场景角色数据状态::未找到;
  case 存在已知准确特征只读状态::事实代次漂移:
    return 场景角色数据状态::事实代次漂移;
  case 存在已知准确特征只读状态::数量预算不足:
    return 场景角色数据状态::数量预算不足;
  case 存在已知准确特征只读状态::资源失败:
    return 场景角色数据状态::资源失败;
  case 存在已知准确特征只读状态::入口拒绝:
    return 场景角色数据状态::入口拒绝;
  default:
    return 场景角色数据状态::内部不一致;
  }
}

L1所有者范围写集请求
场景类数据服务::形成特征组织写集(std::uint64_t g, L1所有者范围写入幂等身份 key,
                                 稳定编码 parent, 特征信息身份 feature) const {
  L1所有者范围写集请求 ws{L1所有者范围CRUD合同版本, g, key};
  ws.关系 = {{{1}, parent, feature.编码, featureLayout_.特征组织关系类型, 1}};
  return ws;
}

bool 场景类数据服务::普通幂等身份有效(L1所有者范围写入幂等身份 key) noexcept {
  return key.值 > 1 && key != 场景特征组织扩展登记固定幂等身份 &&
         key != 场景直接包含扩展登记固定幂等身份 &&
         (key.值 >> 48) != 0x4E43;
}

} // namespace 海中鱼巣
namespace 海中鱼巣 {

场景角色数据状态 场景类数据服务::状态到场景(状态类数据状态 s) noexcept {
  switch (s) {
  case 状态类数据状态::未找到:
    return 场景角色数据状态::未找到;
  case 状态类数据状态::正式特征类型未找到:
    return 场景角色数据状态::引用冲突;
  case 状态类数据状态::事实代次漂移:
    return 场景角色数据状态::事实代次漂移;
  case 状态类数据状态::数量预算不足:
    return 场景角色数据状态::数量预算不足;
  case 状态类数据状态::资源失败:
    return 场景角色数据状态::资源失败;
  case 状态类数据状态::入口拒绝:
  case 状态类数据状态::准确值不相容:
  case 状态类数据状态::旧格式不支持:
    return 场景角色数据状态::入口拒绝;
  case 状态类数据状态::引用冲突:
  default:
    return 场景角色数据状态::内部不一致;
  }
}
场景角色数据状态 场景类数据服务::动态到场景(动态数据状态 s) noexcept {
  switch (s) {
  case 动态数据状态::未找到:
    return 场景角色数据状态::未找到;
  case 动态数据状态::来源未找到:
  case 动态数据状态::引用冲突:
    return 场景角色数据状态::引用冲突;
  case 动态数据状态::数量预算不足:
    return 场景角色数据状态::数量预算不足;
  case 动态数据状态::事实代次漂移:
    return 场景角色数据状态::事实代次漂移;
  case 动态数据状态::资源失败:
    return 场景角色数据状态::资源失败;
  case 动态数据状态::入口拒绝:
    return 场景角色数据状态::入口拒绝;
  default:
    return 场景角色数据状态::内部不一致;
  }
}
场景角色数据状态
场景类数据服务::存在到场景(存在结构身份只读状态 s) noexcept {
  switch (s) {
  case 存在结构身份只读状态::未找到:
    return 场景角色数据状态::未找到;
  case 存在结构身份只读状态::入口拒绝:
    return 场景角色数据状态::入口拒绝;
  case 存在结构身份只读状态::事实代次漂移:
    return 场景角色数据状态::事实代次漂移;
  case 存在结构身份只读状态::资源失败:
    return 场景角色数据状态::资源失败;
  default:
    return 场景角色数据状态::内部不一致;
  }
}

场景角色数据状态 场景类数据服务::映射场景写入状态(
    const L1所有者范围写入结果 &r, 场景角色数据状态 success,
    L1结构所有者身份 owner, const L1所有者范围写集请求 &ws) noexcept {
  if (r.是否形成内存权威发布 && r.状态 != L1所有者范围写入状态::成功)
    return 场景角色数据状态::已可能发布;
  if (r.状态 != L1所有者范围写入状态::成功 &&
      r.状态 != L1所有者范围写入状态::精确重复 && !写入失败头完整(r, owner, ws))
    return 场景角色数据状态::内部不一致;
  switch (r.状态) {
  case L1所有者范围写入状态::成功:
    return success;
  case L1所有者范围写入状态::精确重复:
    return 场景角色数据状态::精确重复;
  case L1所有者范围写入状态::事实代次漂移:
    return 场景角色数据状态::事实代次漂移;
  case L1所有者范围写入状态::幂等冲突:
    return 场景角色数据状态::幂等冲突;
  case L1所有者范围写入状态::引用冲突:
    return 场景角色数据状态::引用冲突;
  case L1所有者范围写入状态::资源失败:
    return 场景角色数据状态::资源失败;
  default:
    return 场景角色数据状态::内部不一致;
  }
}

状态使用绑定数据状态 场景类数据服务::映射绑定写入状态(
    const L1所有者范围写入结果 &r, 状态使用绑定数据状态 success,
    L1结构所有者身份 owner, const L1所有者范围写集请求 &ws) noexcept {
  if (r.是否形成内存权威发布 && r.状态 != L1所有者范围写入状态::成功)
    return 状态使用绑定数据状态::已可能发布;
  if (r.状态 != L1所有者范围写入状态::成功 &&
      r.状态 != L1所有者范围写入状态::精确重复 && !写入失败头完整(r, owner, ws))
    return 状态使用绑定数据状态::内部不一致;
  switch (r.状态) {
  case L1所有者范围写入状态::成功:
    return success;
  case L1所有者范围写入状态::精确重复:
    return 状态使用绑定数据状态::精确重复;
  case L1所有者范围写入状态::事实代次漂移:
    return 状态使用绑定数据状态::事实代次漂移;
  case L1所有者范围写入状态::幂等冲突:
    return 状态使用绑定数据状态::幂等冲突;
  case L1所有者范围写入状态::引用冲突:
    return 状态使用绑定数据状态::引用冲突;
  case L1所有者范围写入状态::资源失败:
    return 状态使用绑定数据状态::资源失败;
  default:
    return 状态使用绑定数据状态::内部不一致;
  }
}

场景类数据服务::代次读取 场景类数据服务::读取当前代次() const {
  const auto q = l1_.读取中性当前事实代次({L1中性CRUD合同版本});
  if (q.状态 == L1中性读取状态::资源失败)
    return {场景角色数据状态::资源失败, q.事实代次};
  if (q.状态 != L1中性读取状态::成功 || q.合同版本 != L1中性CRUD合同版本 ||
      !q.事实代次)
    return {场景角色数据状态::内部不一致, q.事实代次};
  return {场景角色数据状态::已读取, q.事实代次};
}

场景类数据服务::代次读取
场景类数据服务::核验代次(std::uint64_t g) const {
  L1所有者范围一致当前读取请求 request;
  request.合同版本 = L1所有者范围一致当前读取合同版本;
  request.期望事实代次 = g;
  request.所有者 = {owner_};
  const auto q = l1_.尝试读取所有者范围一致当前投影(request);
  if (q.状态 == L1所有者范围一致当前读取状态::事实代次漂移)
    return {场景角色数据状态::事实代次漂移, q.读取事实代次};
  if (q.状态 == L1所有者范围一致当前读取状态::资源失败)
    return {场景角色数据状态::资源失败, q.读取事实代次};
  if (q.状态 != L1所有者范围一致当前读取状态::成功 ||
      q.合同版本 != L1所有者范围一致当前读取合同版本 || q.期望事实代次 != g ||
      q.读取事实代次 != g || q.所有者.size() != 1 ||
      q.所有者.front().查询所有者 != owner_ ||
      q.所有者.front().状态 != L1所有者范围一致当前读取项目状态::成功 ||
      !q.所有者.front().所有者事实 ||
      q.所有者.front().所有者事实->所有者 != owner_)
    return {场景角色数据状态::内部不一致, q.读取事实代次};
  return {场景角色数据状态::已读取, g};
}

bool 场景类数据服务::写入头完整(const L1所有者范围写入结果 &r,
                                       L1所有者范围写入幂等身份 key,
                                       std::uint64_t expected) const noexcept {
  const bool first = r.状态 == L1所有者范围写入状态::成功 &&
                     r.是否形成内存权威发布 &&
                     r.重试边界 == L1所有者范围重试边界::不适用;
  const bool replay = r.状态 == L1所有者范围写入状态::精确重复 &&
                      !r.是否形成内存权威发布 &&
                      r.重试边界 == L1所有者范围重试边界::原幂等身份读回收敛;
  return expected != UINT64_MAX && r.合同版本 == L1所有者范围CRUD合同版本 &&
         r.所有者 == owner_ && r.写入幂等身份 == key &&
         r.事实代次 == expected + 1 && (first || replay);
}

bool
场景类数据服务::写入失败头完整(const L1所有者范围写入结果 &r,
                               L1结构所有者身份 owner,
                               const L1所有者范围写集请求 &ws) noexcept {
  if (r.合同版本 != L1所有者范围CRUD合同版本 || r.所有者 != owner ||
      r.写入幂等身份 != ws.写入幂等身份 || r.是否形成内存权威发布 ||
      !r.新编码映射.empty())
    return false;
  switch (r.状态) {
  case L1所有者范围写入状态::事实代次漂移:
    return r.事实代次 && r.事实代次 != ws.期望事实代次 &&
           r.重试边界 == L1所有者范围重试边界::原请求可重试;
  case L1所有者范围写入状态::资源失败:
    return r.重试边界 == L1所有者范围重试边界::原请求可重试;
  case L1所有者范围写入状态::内部不一致:
    return r.重试边界 == L1所有者范围重试边界::原幂等身份读回收敛;
  case L1所有者范围写入状态::幂等冲突:
    return r.事实代次 && r.重试边界 == L1所有者范围重试边界::修正请求后可重试;
  case L1所有者范围写入状态::引用冲突:
    return r.事实代次 == ws.期望事实代次 &&
           r.重试边界 == L1所有者范围重试边界::修正请求后可重试;
  case L1所有者范围写入状态::入口拒绝:
  case L1所有者范围写入状态::许可拒绝:
  case L1所有者范围写入状态::未找到:
    return r.重试边界 == L1所有者范围重试边界::修正请求后可重试;
  default:
    return false;
  }
}

bool
场景类数据服务::写入映射完整(const L1所有者范围写入结果 &r,
                             const L1所有者范围写集请求 &ws) noexcept {
  std::vector<std::uint64_t> expected;
  expected.reserve(ws.节点.size() + ws.关系.size() + ws.值.size());
  for (const auto &x : ws.节点)
    expected.push_back(x.本地键.值);
  for (const auto &x : ws.关系)
    expected.push_back(x.本地键.值);
  for (const auto &x : ws.值)
    expected.push_back(x.本地键.值);
  std::sort(expected.begin(), expected.end());
  if (std::adjacent_find(expected.begin(), expected.end()) != expected.end() ||
      r.新编码映射.size() != expected.size())
    return false;
  std::vector<std::uint64_t> actual;
  actual.reserve(r.新编码映射.size());
  for (const auto &[key, id] : r.新编码映射) {
    if (!有效(id))
      return false;
    actual.push_back(key.值);
  }
  std::sort(actual.begin(), actual.end());
  return actual == expected;
}

bool
场景类数据服务::竞争精确重复材料完整(const L1所有者范围写入结果 &r,
                                     const L1所有者范围写集请求 &ws) const {
  if (r.状态 != L1所有者范围写入状态::精确重复 ||
      !写入头完整(r, ws.写入幂等身份, ws.期望事实代次) || !写入映射完整(r, ws))
    return false;
  const auto first = port_.读取首次写入材料(
      {L1所有者范围首次写入读取合同版本, ws.写入幂等身份});
  return first.状态 == L1所有者范围读取状态::成功 &&
         first.合同版本 == L1所有者范围首次写入读取合同版本 &&
         first.所有者 == owner_ && first.写入幂等身份 == ws.写入幂等身份 &&
         first.首次规范化写集 && 写集相同(*first.首次规范化写集, ws) &&
         first.首次写入结果 &&
         first.首次写入结果->状态 == L1所有者范围写入状态::成功 &&
         写入头完整(*first.首次写入结果, ws.写入幂等身份, ws.期望事实代次) &&
         写入映射完整(*first.首次写入结果, ws) &&
         first.首次写入结果->新编码映射 == r.新编码映射;
}

bool 场景类数据服务::写集相同(const L1所有者范围写集请求 &a,
                                     const L1所有者范围写集请求 &b) noexcept {
  const auto refEqual = [](const L1所有者范围事实引用 &x,
                           const L1所有者范围事实引用 &y) noexcept {
    if (x.index() != y.index())
      return false;
    if (const auto *p = std::get_if<稳定编码>(&x)) {
      const auto *q = std::get_if<稳定编码>(&y);
      return q && *p == *q;
    }
    const auto *p = std::get_if<L1所有者范围写集本地键>(&x);
    const auto *q = std::get_if<L1所有者范围写集本地键>(&y);
    return p && q && *p == *q;
  };
  if (a.合同版本 != b.合同版本 || a.期望事实代次 != b.期望事实代次 ||
      a.写入幂等身份 != b.写入幂等身份 || a.节点.size() != b.节点.size() ||
      a.关系.size() != b.关系.size() || !a.值.empty() || !b.值.empty() ||
      !a.属性槽变更.empty() || !b.属性槽变更.empty() ||
      a.退出事实 != b.退出事实)
    return false;
  for (std::size_t i = 0; i < a.节点.size(); ++i) {
    const auto &x = a.节点[i];
    const auto &y = b.节点[i];
    if (x.本地键 != y.本地键 || x.种类 != y.种类 ||
        x.属性类型表示 != y.属性类型表示)
      return false;
  }
  for (std::size_t i = 0; i < a.关系.size(); ++i) {
    const auto &x = a.关系[i];
    const auto &y = b.关系[i];
    if (x.本地键 != y.本地键 || x.角色或顺序 != y.角色或顺序 ||
        !refEqual(x.源节点, y.源节点) || !refEqual(x.目标节点, y.目标节点) ||
        !refEqual(x.关系类型节点, y.关系类型节点))
      return false;
  }
  return true;
}

std::optional<稳定编码>
场景类数据服务::映射编码(const L1所有者范围写入结果 &r,
                         L1所有者范围写集本地键 key) {
  std::optional<稳定编码> out;
  for (const auto &[k, id] : r.新编码映射)
    if (k == key) {
      if (out || !有效(id))
        return std::nullopt;
      out = id;
    }
  return out;
}
L1所有者范围写入结果
场景类数据服务::串行提交(const L1所有者范围写集请求 &ws) {
  std::lock_guard lock(write_);
  return port_.提交所有者范围中性写集(ws);
}

L1所有者范围写集请求 场景类数据服务::形成启用写集(
    std::uint64_t g, L1所有者范围写入幂等身份 key, 稳定编码 object) const {
  L1所有者范围写集请求 ws{L1所有者范围CRUD合同版本, g, key};
  ws.关系.push_back({{1},
                     object,
                     layout_.绑定结构.场景族锚点,
                     layout_.绑定结构.场景族归属关系类型,
                     1});
  for (std::uint32_t i = 0; i < 4; ++i) {
    ws.节点.push_back({{i + 2}, 节点种类::普通, std::nullopt});
    ws.关系.push_back({{i + 6},
                       object,
                       L1所有者范围写集本地键{i + 2},
                       layout_.根绑定关系类型,
                       static_cast<std::int64_t>(i + 1)});
  }
  return ws;
}

L1所有者范围写集请求
场景类数据服务::形成绑定写集(const 状态使用绑定创建请求 &r) const {
  L1所有者范围写集请求 ws{L1所有者范围CRUD合同版本, r.G0, r.幂等身份};
  ws.节点 = {{{1}, 节点种类::普通, std::nullopt}};
  ws.关系 = {{{2},
              L1所有者范围写集本地键{1},
              r.发生场景,
              layout_.绑定结构.状态使用绑定成员关系类型,
              1},
             {{3},
              L1所有者范围写集本地键{1},
              r.被描述存在,
              layout_.绑定结构.状态使用绑定成员关系类型,
              2},
             {{4},
              L1所有者范围写集本地键{1},
              r.状态.编码,
              layout_.绑定结构.状态使用绑定成员关系类型,
              3}};
  return ws;
}

L1所有者范围写集请求
场景类数据服务::形成组织写集(std::uint64_t g, L1所有者范围写入幂等身份 key,
                             稳定编码 parent, 稳定编码 instance,
                             场景根角色 role) const {
  L1所有者范围写集请求 ws{L1所有者范围CRUD合同版本, g, key};
  ws.关系 = {{{1},
              parent,
              instance,
              role == 场景根角色::状态 ? layout_.状态组织关系类型
                                       : layout_.动态组织关系类型,
              1}};
  return ws;
}

场景组织当前读取结果 场景类数据服务::读路径(std::uint64_t g,
                                                场景根角色 role,
                                                稳定编码 instance,
                                                std::uint64_t budget) const {
  const auto type = role == 场景根角色::状态 ? layout_.状态组织关系类型
                                             : layout_.动态组织关系类型;
  场景实例组织事实 fact;
  fact.Gread = g;
  fact.角色 = role;
  fact.实例 = instance;
  std::vector<L1所有者范围关系事实> reverse;
  std::unordered_set<std::uint64_t> visited;
  auto child = instance;
  for (std::uint64_t depth = 0;; ++depth) {
    if (!visited.insert(child.值).second)
      return {2, 场景角色数据状态::内部不一致, g, std::nullopt};
    auto roots = 查询关系(g, 关系端点方向::目标, child,
                          layout_.根绑定关系类型);
    if (!roots.成功())
      return {2, roots.状态, roots.Gread, std::nullopt};
    if (roots.关系.size() > 4096)
      return {2, 场景角色数据状态::数量预算不足, g, std::nullopt};
    if (!roots.关系.empty()) {
      if (roots.关系.size() != 1 || roots.关系.front().写入所有者 != owner_ ||
          roots.关系.front().目标节点 != child ||
          roots.关系.front().关系类型节点 != layout_.根绑定关系类型 ||
          roots.关系.front().角色或顺序 != static_cast<std::int64_t>(role))
        return {2, 场景角色数据状态::内部不一致, g, std::nullopt};
      if (reverse.empty())
        return {2, 场景角色数据状态::实例未组织, g, std::nullopt};
      auto c = roots.关系.front().源节点;
      auto scene = 读角色(g, c);
      if (!scene.成功({2, g, c}))
        return {2, scene.状态, scene.Gread, std::nullopt};
      const auto index =
          static_cast<std::size_t>(static_cast<unsigned>(role) - 1);
      if (index >= scene.角色->四根.size() ||
          scene.角色->四根[index].根.编码 != child)
        return {2, 场景角色数据状态::内部不一致, g, std::nullopt};
      fact.场景角色 = *scene.角色;
      fact.根 = child;
      break;
    }
    if (depth >= budget)
      return {2, 场景角色数据状态::数量预算不足, g, std::nullopt};
    auto q = 查询关系(g, 关系端点方向::目标, child, type);
    if (!q.成功())
      return {2, q.状态, q.Gread, std::nullopt};
    if (q.关系.size() > 4096)
      return {2, 场景角色数据状态::数量预算不足, g, std::nullopt};
    if (q.关系.empty())
      return {2,
              reverse.empty() ? 场景角色数据状态::实例未组织
                              : 场景角色数据状态::内部不一致,
              g, std::nullopt};
    if (q.关系.size() != 1)
      return {2, 场景角色数据状态::内部不一致, g, std::nullopt};
    const auto &e = q.关系.front();
    if (e.写入所有者 != owner_ || e.关系类型节点 != type || e.角色或顺序 != 1 ||
        e.目标节点 != child || e.创建事实代次 > g)
      return {2, 场景角色数据状态::内部不一致, g, std::nullopt};
    reverse.push_back(e);
    child = e.源节点;
  }
  std::reverse(reverse.begin(), reverse.end());
  const 稳定编码 forbidden[]{fact.场景角色.场景,
                             fact.场景角色.四根[0].根.编码,
                             fact.场景角色.四根[1].根.编码,
                             fact.场景角色.四根[2].根.编码,
                             fact.场景角色.四根[3].根.编码,
                             layout_.绑定结构.场景族锚点,
                             layout_.绑定结构.场景族归属关系类型,
                             layout_.绑定结构.状态使用绑定成员关系类型,
                             layout_.根绑定关系类型,
                             layout_.状态组织关系类型,
                             layout_.动态组织关系类型};
  for (const auto &e : reverse) {
    for (const auto id : forbidden)
      if (e.目标节点 == id)
        return {2, 场景角色数据状态::内部不一致, g, std::nullopt};
    auto b = 查询关系(g, 关系端点方向::源, e.目标节点,
                      layout_.绑定结构.状态使用绑定成员关系类型);
    if (!b.成功())
      return {2, b.状态, b.Gread, std::nullopt};
    if (b.关系.size() > 4096)
      return {2, 场景角色数据状态::数量预算不足, g, std::nullopt};
    if (!b.关系.empty())
      return {2, 场景角色数据状态::内部不一致, g, std::nullopt};
  }
  auto rootNode = 读节点(fact.根, g);
  if (!rootNode.成功())
    return {2,
            rootNode.状态 == 场景角色数据状态::未找到
                ? 场景角色数据状态::内部不一致
                : rootNode.状态,
            rootNode.Gread, std::nullopt};
  if (rootNode.事实->写入所有者 != owner_ ||
      rootNode.事实->种类 != 节点种类::普通 || rootNode.事实->属性类型表示 ||
      rootNode.事实->创建事实代次 > g)
    return {2, 场景角色数据状态::内部不一致, g, std::nullopt};
  fact.路径节点.push_back(转节点(*rootNode.事实));
  for (const auto &e : reverse) {
    fact.路径边.push_back(转边(e));
    auto n = 读节点(e.目标节点, g);
    if (!n.成功())
      return {2,
              n.状态 == 场景角色数据状态::未找到 ? 场景角色数据状态::内部不一致
                                                 : n.状态,
              n.Gread, std::nullopt};
    if (n.事实->创建事实代次 > g)
      return {2, 场景角色数据状态::内部不一致, g, std::nullopt};
    fact.路径节点.push_back(转节点(*n.事实));
  }
  场景组织当前读取结果 result{2, 场景角色数据状态::已读取, g, std::move(fact)};
  return result.成功({2, g, role, instance, budget})
             ? std::move(result)
             : 场景组织当前读取结果{2, 场景角色数据状态::内部不一致, g,
                                std::nullopt};
}
场景组织当前读取结果
场景类数据服务::读取当前实例组织(const 场景组织当前读取请求 &r) const {
  if (r.版本 != 2 || !r.G0 || !有效(r.实例) ||
      !r.最大路径长度 || r.最大路径长度 > 4096 ||
      (r.角色 != 场景根角色::状态 && r.角色 != 场景根角色::动态))
    return {};
  std::uint64_t confirmed = 0;
  try {
    auto first = 核验代次(r.G0);
    if (!first.成功())
      return {2, first.状态, first.Gread, std::nullopt};
    confirmed = first.Gread;
    auto result = 读路径(r.G0, r.角色, r.实例, r.最大路径长度);
    auto last = 核验代次(r.G0);
    if (!last.成功())
      return {2, last.状态, last.Gread, std::nullopt};
    return result;
  } catch (const std::bad_alloc &) {
    return {2, 场景角色数据状态::资源失败, confirmed, std::nullopt};
  } catch (...) {
    return {2, 场景角色数据状态::内部不一致, confirmed, std::nullopt};
  }
}
场景动态组织当前读取结果
场景类数据服务::读取当前动态场景组织(const 场景动态组织当前读取请求 &r) const {
  auto x = 读取当前实例组织(
      {r.版本, r.G0, 场景根角色::动态, r.动态.编码, r.最大路径长度});
  return {x.版本, x.状态, x.Gread, std::move(x.组织)};
}

场景组织写结果_v2
场景类数据服务::组织(std::uint32_t v, std::uint64_t g,
                     L1所有者范围写入幂等身份 key, 稳定编码 c, 稳定编码 parent,
                     稳定编码 instance, std::uint64_t budget, 场景根角色 role) {
  if (v != 2 || !g || g == UINT64_MAX || !普通幂等身份有效(key) || !有效(c) ||
      !有效(parent) || !有效(instance) || !budget || budget > 4096 ||
      parent == instance ||
      (role != 场景根角色::状态 && role != 场景根角色::动态))
    return {};
  bool commitStarted = false;
  std::uint64_t published = 0, confirmed = 0;
  try {
    const auto expected = 形成组织写集(g, key, parent, instance, role);
    const auto first =
        port_.读取首次写入材料({L1所有者范围首次写入读取合同版本, key});
    if (first.合同版本 != L1所有者范围首次写入读取合同版本 ||
        first.所有者 != owner_ || first.写入幂等身份 != key)
      return {2, 场景角色数据状态::内部不一致, first.读取事实代次, 0,
              std::nullopt};
    if (first.状态 == L1所有者范围读取状态::成功) {
      if (!first.首次规范化写集 || !写集相同(*first.首次规范化写集, expected))
        return {2, 场景角色数据状态::幂等冲突, first.读取事实代次, 0,
                std::nullopt};
      if (!first.首次写入结果 ||
          first.首次写入结果->状态 != L1所有者范围写入状态::成功 ||
          !写入头完整(*first.首次写入结果, key, g) ||
          !写入映射完整(*first.首次写入结果, expected))
        return {2, 场景角色数据状态::内部不一致, first.读取事实代次, 0,
                std::nullopt};
      commitStarted = true;
      const auto saved = 串行提交(*first.首次规范化写集);
      published = saved.事实代次;
      if (saved.状态 != L1所有者范围写入状态::精确重复) {
        auto status =
            映射场景写入状态(saved, 场景角色数据状态::已组织, owner_, expected);
        if (status == 场景角色数据状态::已组织)
          status = saved.是否形成内存权威发布 ? 场景角色数据状态::已可能发布
                                              : 场景角色数据状态::内部不一致;
        return {2, status, saved.事实代次,
                status == 场景角色数据状态::已可能发布 ? saved.事实代次 : 0,
                std::nullopt};
      }
      if (!写入头完整(saved, key, g) || !写入映射完整(saved, expected) ||
          saved.新编码映射 != first.首次写入结果->新编码映射)
        return {2, 场景角色数据状态::已可能发布, saved.事实代次, saved.事实代次,
                std::nullopt};
      const auto guard = 读取当前代次();
      if (!guard.成功())
        return {2, 场景角色数据状态::已可能发布, guard.Gread, saved.事实代次,
                std::nullopt};
      confirmed = guard.Gread;
      auto path = 读路径(guard.Gread, role, instance, budget);
      if (!path.成功(
              {2, guard.Gread, role, instance, budget}) ||
          path.组织->场景角色.场景 != c ||
          path.组织->路径边.back().源 != parent)
        return {2, 场景角色数据状态::已可能发布, path.Gread, saved.事实代次,
                std::nullopt};
      const auto finalGuard = 核验代次(guard.Gread);
      if (!finalGuard.成功())
        return {2, 场景角色数据状态::已可能发布, finalGuard.Gread,
                saved.事实代次, std::nullopt};
      场景实例组织回执_v2 receipt{
          guard.Gread, c, path.组织->根, instance, role,
          path.组织->路径边.back()};
      return {2, 场景角色数据状态::精确重复, guard.Gread, saved.事实代次,
              std::move(receipt)};
    }
    if (first.状态 != L1所有者范围读取状态::未找到)
      return {2,
              first.状态 == L1所有者范围读取状态::资源失败
                  ? 场景角色数据状态::资源失败
                  : 场景角色数据状态::内部不一致,
              first.读取事实代次, 0, std::nullopt};
    if (first.读取事实代次 != g)
      return {2, 场景角色数据状态::事实代次漂移, first.读取事实代次, 0,
              std::nullopt};
    if (first.首次规范化写集 || first.首次写入结果)
      return {2, 场景角色数据状态::内部不一致, first.读取事实代次, 0,
              std::nullopt};
    auto scene = 读角色(g, c);
    if (!scene.成功({2, g, c}))
      return {2, scene.状态, scene.Gread, 0, std::nullopt};
    auto idx = static_cast<std::size_t>(static_cast<unsigned>(role) - 1);
    auto root = scene.角色->四根[idx].根.编码;
    const 稳定编码 forbidden[]{c,
                               scene.角色->四根[0].根.编码,
                               scene.角色->四根[1].根.编码,
                               scene.角色->四根[2].根.编码,
                               scene.角色->四根[3].根.编码,
                               layout_.绑定结构.场景族锚点,
                               layout_.绑定结构.场景族归属关系类型,
                               layout_.绑定结构.状态使用绑定成员关系类型,
                               layout_.根绑定关系类型,
                               layout_.状态组织关系类型,
                               layout_.动态组织关系类型};
    for (auto id : forbidden)
      if (instance == id)
        return {2, 场景角色数据状态::引用冲突, g, 0, std::nullopt};
    auto bmark = 查询关系(g, 关系端点方向::源, instance,
                          layout_.绑定结构.状态使用绑定成员关系类型);
    if (!bmark.成功())
      return {2, bmark.状态, bmark.Gread, 0, std::nullopt};
    if (!bmark.关系.empty())
      return {2, 场景角色数据状态::引用冲突, g, 0, std::nullopt};
    std::vector<稳定编码> prefix{root};
    if (parent != root) {
      auto p = 读路径(g, role, parent, budget);
      if (!p.成功({2, g, role, parent, budget}) ||
          p.组织->场景角色.场景 != c)
        return {2,
                p.成功({2, g, role, parent, budget})
                    ? 场景角色数据状态::引用冲突
                    : p.状态,
                p.Gread, 0, std::nullopt};
      if (p.组织->路径边.size() >= budget)
        return {2, 场景角色数据状态::数量预算不足, g, 0, std::nullopt};
      prefix.clear();
      for (const auto &n : p.组织->路径节点)
        prefix.push_back(n.编码);
    }
    if (std::find(prefix.begin(), prefix.end(), instance) != prefix.end())
      return {2, 场景角色数据状态::引用冲突, g, 0, std::nullopt};
    auto existing =
        查询关系(g, 关系端点方向::目标, instance,
                 role == 场景根角色::状态 ? layout_.状态组织关系类型
                                          : layout_.动态组织关系类型);
    if (!existing.成功())
      return {2, existing.状态, existing.Gread, 0, std::nullopt};
    if (existing.关系.size() > 4096)
      return {2, 场景角色数据状态::数量预算不足, g, 0, std::nullopt};
    if (!existing.关系.empty())
      return {2, 场景角色数据状态::实例已组织, existing.Gread, 0, std::nullopt};
    commitStarted = true;
    auto saved = 串行提交(expected);
    published = saved.事实代次;
    auto status =
        映射场景写入状态(saved, 场景角色数据状态::已组织, owner_, expected);
    if (status == 场景角色数据状态::精确重复 &&
        !竞争精确重复材料完整(saved, expected))
      return {2, 场景角色数据状态::已可能发布, saved.事实代次, saved.事实代次,
              std::nullopt};
    if (status != 场景角色数据状态::已组织 &&
        status != 场景角色数据状态::精确重复)
      return {2, status, saved.事实代次,
              status == 场景角色数据状态::已可能发布 ? saved.事实代次 : 0,
              std::nullopt};
    if (!写入头完整(saved, key, g) || !写入映射完整(saved, expected))
      return {2,
              saved.是否形成内存权威发布 ||
                      saved.状态 == L1所有者范围写入状态::精确重复
                  ? 场景角色数据状态::已可能发布
                  : 场景角色数据状态::内部不一致,
              saved.事实代次,
              saved.是否形成内存权威发布 ||
                      saved.状态 == L1所有者范围写入状态::精确重复
                  ? saved.事实代次
                  : 0,
              std::nullopt};
    const auto guard = 读取当前代次();
    if (!guard.成功())
      return {2, 场景角色数据状态::已可能发布, guard.Gread, saved.事实代次,
              std::nullopt};
    confirmed = guard.Gread;
    auto path = 读路径(guard.Gread, role, instance, budget);
    if (!path.成功({2, guard.Gread, role, instance, budget}) ||
        path.组织->场景角色.场景 != c || path.组织->路径边.back().源 != parent)
      return {2, 场景角色数据状态::已可能发布, path.Gread, saved.事实代次,
              std::nullopt};
    const auto finalGuard = 核验代次(guard.Gread);
    if (!finalGuard.成功())
      return {2, 场景角色数据状态::已可能发布, finalGuard.Gread, saved.事实代次,
              std::nullopt};
    场景实例组织回执_v2 receipt{
        guard.Gread, c, path.组织->根, instance, role,
        path.组织->路径边.back()};
    return {2, status, guard.Gread, saved.事实代次, std::move(receipt)};
  } catch (const std::bad_alloc &) {
    return {2,
            commitStarted ? 场景角色数据状态::已可能发布
                          : 场景角色数据状态::资源失败,
            confirmed ? confirmed : published, commitStarted ? published : 0,
            std::nullopt};
  } catch (...) {
    return {2,
            commitStarted ? 场景角色数据状态::已可能发布
                          : 场景角色数据状态::内部不一致,
            confirmed ? confirmed : published, commitStarted ? published : 0,
            std::nullopt};
  }
}
场景组织写结果_v2
场景类数据服务::组织状态实例(const 场景状态组织请求 &r) {
  if (r.版本 != 2 || !r.G0 || r.G0 == UINT64_MAX ||
      !普通幂等身份有效(r.幂等身份) || !有效(r.场景) || !有效(r.组织父) ||
      !有效(r.状态) || !r.最大路径长度 || r.最大路径长度 > 4096)
    return {};
  try {
    const auto first =
        port_.读取首次写入材料({L1所有者范围首次写入读取合同版本, r.幂等身份});
    if (first.状态 == L1所有者范围读取状态::成功)
      return 组织(r.版本, r.G0, r.幂等身份, r.场景, r.组织父, r.状态.编码,
                  r.最大路径长度, 场景根角色::状态);
    if (first.状态 != L1所有者范围读取状态::未找到)
      return {2,
              first.状态 == L1所有者范围读取状态::资源失败
                  ? 场景角色数据状态::资源失败
                  : 场景角色数据状态::内部不一致,
              first.读取事实代次, 0, std::nullopt};
    auto s = state_.读取当前状态({2, r.G0, r.状态});
    if (s.结果头.状态 != 状态类数据状态::已读取 || !s.内容)
      return {2, 状态到场景(s.结果头.状态), s.结果头.事实截止代次, 0,
              std::nullopt};
    if (s.结果头.事实截止代次 != r.G0 || s.内容->Gread != r.G0 ||
        s.内容->信息.身份 != r.状态)
      return {2, 场景角色数据状态::内部不一致, r.G0, 0, std::nullopt};
    return 组织(r.版本, r.G0, r.幂等身份, r.场景, r.组织父, r.状态.编码,
                r.最大路径长度, 场景根角色::状态);
  } catch (const std::bad_alloc &) {
    return {2, 场景角色数据状态::资源失败};
  } catch (...) {
    return {2, 场景角色数据状态::内部不一致};
  }
}
场景组织写结果_v2 场景类数据服务::组织动态实例(const 场景动态组织请求 &r,
                                               const 动态结构只读提供者 &d) {
  if (r.版本 != 2 || !r.G0 || r.G0 == UINT64_MAX ||
      !普通幂等身份有效(r.幂等身份) || !有效(r.场景) || !有效(r.组织父) ||
      !有效(r.动态) || !r.最大路径长度 || r.最大路径长度 > 4096)
    return {};
  if (!d.绑定于(l1_))
    return {2, 场景角色数据状态::内部不一致};
  try {
    const auto first =
        port_.读取首次写入材料({L1所有者范围首次写入读取合同版本, r.幂等身份});
    if (first.状态 == L1所有者范围读取状态::成功)
      return 组织(r.版本, r.G0, r.幂等身份, r.场景, r.组织父, r.动态.编码,
                  r.最大路径长度, 场景根角色::动态);
    if (first.状态 != L1所有者范围读取状态::未找到)
      return {2,
              first.状态 == L1所有者范围读取状态::资源失败
                  ? 场景角色数据状态::资源失败
                  : 场景角色数据状态::内部不一致,
              first.读取事实代次, 0, std::nullopt};
    动态读取预算 b{4096, 4096, 16384, 4096, 65536};
    auto one = d.读取当前动态({1, r.G0, r.动态, b});
    if (one.操作.状态 != 动态数据状态::已读取
        || one.操作.合同版本 != 1
        || one.操作.Gread != r.G0
        || one.操作.发布代次
        || one.操作.阶段 != 动态发布阶段::无写入
        || !one.内容)
      return {2, 动态到场景(one.操作.状态), one.操作.Gread, 0, std::nullopt};
    if (one.内容->Gread != r.G0 || one.内容->信息.身份 != r.动态)
      return {2, 场景角色数据状态::内部不一致, one.操作.Gread, 0, std::nullopt};
    return 组织(r.版本, r.G0, r.幂等身份, r.场景, r.组织父, r.动态.编码,
                r.最大路径长度, 场景根角色::动态);
  } catch (const std::bad_alloc &) {
    return {2, 场景角色数据状态::资源失败};
  } catch (...) {
    return {2, 场景角色数据状态::内部不一致};
  }
}

} // namespace 海中鱼巣
namespace 海中鱼巣 {

场景角色写结果 场景类数据服务::启用场景角色(
    const 场景角色启用请求 &r, const 直接归属联合只读提供者 &joint) {
  if (r.版本 == 3)
    return {2, 场景角色数据状态::旧版本迁移拒绝, r.G0};
  if (r.版本 != 4 || !r.G0 || r.G0 == UINT64_MAX ||
      !普通幂等身份有效(r.幂等身份) || !有效(r.对象存在) ||
      !有效(r.父场景语境) || !有效(r.预期结构父关系) ||
      r.对象存在 == r.父场景语境 || !r.最大关系数量 ||
      r.最大关系数量 > 4096 || !r.最大祖先数量 ||
      r.最大祖先数量 > 4096 || !joint.绑定于(l1_))
    return {};
  const auto first =
      port_.读取首次写入材料({L1所有者范围首次写入读取合同版本, r.幂等身份});
  if (first.状态 == L1所有者范围读取状态::成功 &&
      first.首次规范化写集 && first.首次规范化写集->节点.size() == 4 &&
      first.首次规范化写集->关系.size() == 10 &&
      first.首次规范化写集->值.empty() &&
      first.首次规范化写集->属性槽变更.empty() &&
      first.首次规范化写集->退出事实.empty())
    return {2, 场景角色数据状态::旧版本迁移拒绝,
            first.读取事实代次};
  const 直接归属联合父读取请求 parentRequest{
      1, r.G0, r.对象存在, r.最大关系数量};
  const auto parent = joint.读取当前联合父(parentRequest);
  if (!parent.父读取成功(parentRequest) || !parent.父 ||
      parent.父->关系 != r.预期结构父关系 ||
      parent.父->父 != r.父场景语境 || parent.父->成员 != r.对象存在)
    return {};
  const 直接归属场景角色读取请求 upstreamRequest{1, r.G0, r.父场景语境};
  const auto upstream = joint.读取当前场景角色位置(upstreamRequest);
  if (!upstream.成功(upstreamRequest) || !upstream.角色 ||
      !upstream.角色->树根)
    return {};
  const auto tree = 启用树角色核心(
      r.G0, r.幂等身份, r.对象存在, std::nullopt, r.父场景语境,
      parent.父, upstream.角色->树根, false, r.最大关系数量,
      r.最大祖先数量, joint);
  const auto map = [](场景直接包含状态 status) noexcept {
    switch (status) {
    case 场景直接包含状态::已启用并纳入场景树:
      return 场景角色数据状态::已启用;
    case 场景直接包含状态::精确重复:
      return 场景角色数据状态::精确重复;
    case 场景直接包含状态::入口拒绝:
      return 场景角色数据状态::入口拒绝;
    case 场景直接包含状态::场景未找到:
    case 场景直接包含状态::成员未找到:
      return 场景角色数据状态::未找到;
    case 场景直接包含状态::成员已归属:
      return 场景角色数据状态::场景角色已存在;
    case 场景直接包含状态::引用冲突:
      return 场景角色数据状态::引用冲突;
    case 场景直接包含状态::事实代次漂移:
      return 场景角色数据状态::事实代次漂移;
    case 场景直接包含状态::幂等冲突:
      return 场景角色数据状态::幂等冲突;
    case 场景直接包含状态::数量预算不足:
      return 场景角色数据状态::数量预算不足;
    case 场景直接包含状态::资源失败:
      return 场景角色数据状态::资源失败;
    case 场景直接包含状态::已可能发布:
      return 场景角色数据状态::已可能发布;
    default:
      return 场景角色数据状态::内部不一致;
    }
  };
  场景角色写结果 out{2, map(tree.结果头.状态), tree.结果头.Gread,
                        tree.结果头.首次发布H.value_or(0), tree.场景角色,
                        tree.既有直接父, tree.父语境投影};
  if ((out.状态 == 场景角色数据状态::已启用 ||
       out.状态 == 场景角色数据状态::精确重复) && !out.启用成功(r)) {
    out.状态 = 场景角色数据状态::已可能发布;
    out.角色.reset();
    out.结构父.reset();
    out.父语境投影.reset();
  }
  return out;
}


场景角色写结果 场景类数据服务::退出场景角色(const 场景角色退出请求 &r) {
  if (r.版本 == 3)
    return {2, 场景角色数据状态::旧版本迁移拒绝, r.G0};
  if (r.版本 != 4 || !r.G0 || r.G0 == UINT64_MAX ||
      !普通幂等身份有效(r.幂等身份) || !有效(r.场景) ||
      !有效(r.父场景语境) || !有效(r.预期结构父关系) ||
      r.场景 == r.父场景语境 || !r.最大关系数量 ||
      r.最大关系数量 > 4096)
    return {};
  bool commitStarted = false;
  std::uint64_t published = 0, confirmed = 0;
  try {
    const auto first =
        port_.读取首次写入材料({L1所有者范围首次写入读取合同版本, r.幂等身份});
    if (first.合同版本 != L1所有者范围首次写入读取合同版本 ||
        first.所有者 != owner_ || first.写入幂等身份 != r.幂等身份)
      return {2, 场景角色数据状态::内部不一致, first.读取事实代次, 0,
              std::nullopt};
    if (first.状态 == L1所有者范围读取状态::成功) {
      if (!first.首次规范化写集 || !first.首次写入结果)
        return {2, 场景角色数据状态::内部不一致, first.读取事实代次, 0,
                std::nullopt};
      const auto &ws = *first.首次规范化写集;
      for (const auto id : ws.退出事实) {
        const auto legacy = 读关系(id, first.读取事实代次);
        if (legacy.成功() &&
            legacy.事实->关系类型节点 ==
                includeLayout_.直接子场景关系类型 &&
            legacy.事实->目标节点 == r.场景) {
          return {2, 场景角色数据状态::旧版本迁移拒绝,
                  first.读取事实代次, 0, std::nullopt, std::nullopt};
        }
      }
      if (ws.期望事实代次 != r.G0 || ws.写入幂等身份 != r.幂等身份 ||
          !ws.节点.empty() || !ws.关系.empty() || !ws.值.empty() ||
          !ws.属性槽变更.empty() || ws.退出事实.size() != 10 ||
          !std::is_sorted(ws.退出事实.begin(), ws.退出事实.end()) ||
          first.首次写入结果->状态 != L1所有者范围写入状态::成功 ||
          !写入头完整(*first.首次写入结果, r.幂等身份, r.G0))
        return {2, 场景角色数据状态::幂等冲突, first.读取事实代次, 0,
                std::nullopt};
      commitStarted = true;
      const auto saved = 串行提交(ws);
      published = saved.事实代次;
      if (saved.状态 != L1所有者范围写入状态::精确重复) {
        auto status =
            映射场景写入状态(saved, 场景角色数据状态::已删除, owner_, ws);
        if (status == 场景角色数据状态::已删除)
          status = saved.是否形成内存权威发布 ? 场景角色数据状态::已可能发布
                                              : 场景角色数据状态::内部不一致;
        return {2, status, saved.事实代次,
                status == 场景角色数据状态::已可能发布 ? saved.事实代次 : 0,
                std::nullopt};
      }
      if (!写入头完整(saved, r.幂等身份, r.G0) || !写入映射完整(saved, ws) ||
          saved.新编码映射 != first.首次写入结果->新编码映射)
        return {2, 场景角色数据状态::已可能发布, saved.事实代次, saved.事实代次,
                std::nullopt};
      const auto guard = 读取当前代次();
      if (!guard.成功())
        return {2, 场景角色数据状态::已可能发布, guard.Gread, saved.事实代次,
                std::nullopt};
      confirmed = guard.Gread;
      const auto current = 读角色(guard.Gread, r.场景);
      if (current.状态 != 场景角色数据状态::场景角色未启用)
        return {2, 场景角色数据状态::已可能发布, current.Gread,
                saved.事实代次, std::nullopt};
      const auto finalGuard = 核验代次(guard.Gread);
      if (!finalGuard.成功())
        return {2, 场景角色数据状态::已可能发布, finalGuard.Gread,
                saved.事实代次, std::nullopt};
      return {2, 场景角色数据状态::精确重复, guard.Gread,
              saved.事实代次, std::nullopt, std::nullopt, std::nullopt};
    }
    if (first.状态 != L1所有者范围读取状态::未找到)
      return {2,
              first.状态 == L1所有者范围读取状态::资源失败
                  ? 场景角色数据状态::资源失败
                  : 场景角色数据状态::内部不一致,
              first.读取事实代次, 0, std::nullopt};
    if (first.读取事实代次 != r.G0)
      return {2, 场景角色数据状态::事实代次漂移, first.读取事实代次, 0,
              std::nullopt};
    if (first.首次规范化写集 || first.首次写入结果)
      return {2, 场景角色数据状态::内部不一致, first.读取事实代次, 0,
              std::nullopt};
    auto old = 读角色(r.G0, r.场景);
    if (!old.成功({2, r.G0, r.场景}))
      return {2, old.状态, old.Gread, 0, std::nullopt};
    直接归属联合只读组合器 joint(existenceStructure_, *this);
    const 直接归属联合父读取请求 parentRequest{
        1, r.G0, r.场景, r.最大关系数量};
    const auto parent = joint.读取当前联合父(parentRequest);
    if (!parent.父读取成功(parentRequest) || !parent.父 ||
        parent.父->关系 != r.预期结构父关系 ||
        parent.父->父 != r.父场景语境 || parent.父->成员 != r.场景)
      return {2, 场景角色数据状态::引用冲突, r.G0, 0, std::nullopt};
    const 场景父语境读取请求 contextRequest{
        1, r.G0, r.场景, r.最大关系数量};
    const auto context = 读取当前父场景语境(contextRequest);
    if (!context.成功(contextRequest) || !context.投影 ||
        context.投影->结构父.关系 != r.预期结构父关系 ||
        context.投影->父场景语境 != r.父场景语境)
      return {2, 场景角色数据状态::引用冲突, r.G0, 0, std::nullopt};
    const auto position = 读取当前场景角色位置({1, r.G0, r.场景});
    if (!position.成功({1, r.G0, r.场景}) || !position.角色 ||
        position.角色->位置 == 直接归属场景位置::场景树根 ||
        !position.角色->树证明关系)
      return {2, 场景角色数据状态::引用冲突, r.G0, 0, std::nullopt};
    const 稳定编码 roleDependencies[]{
        includeLayout_.根标记关系类型, includeLayout_.直接存在成员关系类型,
        includeLayout_.直接子场景关系类型};
    for (const auto type : roleDependencies) {
      auto q = 查询关系(r.G0, 关系端点方向::源, r.场景, type);
      if (!q.成功())
        return {2, q.状态, q.Gread, 0, std::nullopt};
      if (!q.关系.empty())
        return {2, 场景角色数据状态::引用冲突, r.G0, 0, std::nullopt};
    }
    auto bs = 查B组(1, r.G0, r.场景, 1, 65536);
    if (!bs.成功()) {
      const auto s = bs.结果头.状态 == 状态使用绑定数据状态::事实代次漂移
                         ? 场景角色数据状态::事实代次漂移
                     : bs.结果头.状态 == 状态使用绑定数据状态::数量预算不足
                         ? 场景角色数据状态::数量预算不足
                     : bs.结果头.状态 == 状态使用绑定数据状态::资源失败
                         ? 场景角色数据状态::资源失败
                         : 场景角色数据状态::内部不一致;
      return {2, s, bs.结果头.Gread, 0, std::nullopt};
    }
    if (!bs.绑定组.empty())
      return {2, 场景角色数据状态::引用冲突, r.G0, 0, std::nullopt};
    auto featureEdges =
        查询关系(r.G0, 关系端点方向::源,
                 old.角色->四根[0].根.编码, featureLayout_.特征组织关系类型);
    if (!featureEdges.成功() || !featureEdges.关系.empty())
      return {2,
              featureEdges.成功() ? 场景角色数据状态::引用冲突
                                  : featureEdges.状态,
              featureEdges.Gread, 0, std::nullopt};
    for (std::size_t i = 1; i <= 2; ++i) {
      auto type = i == 1 ? layout_.状态组织关系类型 : layout_.动态组织关系类型;
      auto q = 查询关系(r.G0, 关系端点方向::源,
                        old.角色->四根[i].根.编码, type);
      if (!q.成功() || !q.关系.empty())
        return {2, q.成功() ? 场景角色数据状态::引用冲突 : q.状态, q.Gread, 0,
                std::nullopt};
    }
    L1所有者范围写集请求 ws{L1所有者范围CRUD合同版本, r.G0, r.幂等身份};
    for (const auto &x : old.角色->四根) {
      ws.退出事实.push_back(x.根.编码);
      ws.退出事实.push_back(x.绑定.编码);
    }
    ws.退出事实.push_back(context.投影->投影边.编码);
    ws.退出事实.push_back(*position.角色->树证明关系);
    std::sort(ws.退出事实.begin(), ws.退出事实.end());
    commitStarted = true;
    auto saved = 串行提交(ws);
    published = saved.事实代次;
    auto status = 映射场景写入状态(saved, 场景角色数据状态::已删除, owner_, ws);
    if (status == 场景角色数据状态::精确重复 &&
        !竞争精确重复材料完整(saved, ws))
      return {2, 场景角色数据状态::已可能发布, saved.事实代次, saved.事实代次,
              std::nullopt};
    if (status != 场景角色数据状态::已删除 &&
        status != 场景角色数据状态::精确重复)
      return {2, status, saved.事实代次,
              status == 场景角色数据状态::已可能发布 ? saved.事实代次 : 0,
              std::nullopt};
    if (!写入头完整(saved, r.幂等身份, r.G0) || !写入映射完整(saved, ws))
      return {2,
              saved.是否形成内存权威发布 ||
                      saved.状态 == L1所有者范围写入状态::精确重复
                  ? 场景角色数据状态::已可能发布
                  : 场景角色数据状态::内部不一致,
              saved.事实代次,
              saved.是否形成内存权威发布 ||
                      saved.状态 == L1所有者范围写入状态::精确重复
                  ? saved.事实代次
                  : 0,
              std::nullopt};
    const auto guard = 读取当前代次();
    if (!guard.成功())
      return {2, 场景角色数据状态::已可能发布, guard.Gread, saved.事实代次,
              std::nullopt};
    confirmed = guard.Gread;
    const auto read = 读角色(guard.Gread, r.场景);
    if (read.状态 != 场景角色数据状态::场景角色未启用)
      return {2, 场景角色数据状态::已可能发布, read.Gread,
              saved.事实代次, std::nullopt};
    const auto finalGuard = 核验代次(guard.Gread);
    if (!finalGuard.成功())
      return {2, 场景角色数据状态::已可能发布, finalGuard.Gread, saved.事实代次,
              std::nullopt};
    return {2, status, guard.Gread, saved.事实代次, std::move(old.角色),
            *parent.父, std::move(context.投影)};
  } catch (const std::bad_alloc &) {
    return {2,
            commitStarted ? 场景角色数据状态::已可能发布
                          : 场景角色数据状态::资源失败,
            confirmed ? confirmed : published, commitStarted ? published : 0,
            std::nullopt};
  } catch (...) {
    return {2,
            commitStarted ? 场景角色数据状态::已可能发布
                          : 场景角色数据状态::内部不一致,
            confirmed ? confirmed : published, commitStarted ? published : 0,
            std::nullopt};
  }
}

状态使用绑定创建结果
场景类数据服务::创建状态使用绑定(const 状态使用绑定创建请求 &r) {
  if (r.合同版本 != 1 || !r.G0 || r.G0 == UINT64_MAX ||
      !普通幂等身份有效(r.幂等身份) || !有效(r.发生场景) ||
      !有效(r.被描述存在) || !有效(r.状态))
    return {};
  bool commitStarted = false;
  std::uint64_t published = 0, confirmed = 0;
  try {
    const auto expected = 形成绑定写集(r);
    const auto first =
        port_.读取首次写入材料({L1所有者范围首次写入读取合同版本, r.幂等身份});
    if (first.合同版本 != L1所有者范围首次写入读取合同版本 ||
        first.所有者 != owner_ || first.写入幂等身份 != r.幂等身份)
      return {{状态使用绑定数据状态::内部不一致, 1, first.读取事实代次,
               std::nullopt},
              std::nullopt};
    if (first.状态 == L1所有者范围读取状态::成功) {
      if (!first.首次规范化写集 || !写集相同(*first.首次规范化写集, expected))
        return {{状态使用绑定数据状态::幂等冲突, 1, first.读取事实代次,
                 std::nullopt},
                std::nullopt};
      if (!first.首次写入结果 ||
          first.首次写入结果->状态 != L1所有者范围写入状态::成功 ||
          !写入头完整(*first.首次写入结果, r.幂等身份, r.G0) ||
          !写入映射完整(*first.首次写入结果, expected))
        return {{状态使用绑定数据状态::内部不一致, 1, first.读取事实代次,
                 std::nullopt},
                std::nullopt};
      commitStarted = true;
      const auto saved = 串行提交(*first.首次规范化写集);
      published = saved.事实代次;
      if (saved.状态 != L1所有者范围写入状态::精确重复) {
        auto status = 映射绑定写入状态(saved, 状态使用绑定数据状态::已创建,
                                       owner_, expected);
        if (status == 状态使用绑定数据状态::已创建)
          status = saved.是否形成内存权威发布
                       ? 状态使用绑定数据状态::已可能发布
                       : 状态使用绑定数据状态::内部不一致;
        return {{status, 1, saved.事实代次,
                 status == 状态使用绑定数据状态::已可能发布
                     ? std::optional<std::uint64_t>{saved.事实代次}
                     : std::nullopt},
                std::nullopt};
      }
      if (!写入头完整(saved, r.幂等身份, r.G0) ||
          !写入映射完整(saved, expected) ||
          saved.新编码映射 != first.首次写入结果->新编码映射)
        return {{状态使用绑定数据状态::已可能发布, 1, saved.事实代次,
                 saved.事实代次},
                std::nullopt};
      const auto bid = 映射编码(saved, {1});
      if (!bid)
        return {{状态使用绑定数据状态::已可能发布, 1, saved.事实代次,
                 saved.事实代次},
                std::nullopt};
      const auto guard = 读取当前代次();
      if (!guard.成功())
        return {
            {状态使用绑定数据状态::已可能发布, 1, guard.Gread, saved.事实代次},
            std::nullopt};
      confirmed = guard.Gread;
      auto read = 读B(guard.Gread, {*bid});
      if (!read.成功())
        return {
            {状态使用绑定数据状态::已可能发布, 1, guard.Gread, saved.事实代次},
            std::nullopt};
      const auto finalGuard = 核验代次(guard.Gread);
      if (!finalGuard.成功())
        return {{状态使用绑定数据状态::已可能发布, 1, finalGuard.Gread,
                 saved.事实代次},
                std::nullopt};
      read.结果头 = {状态使用绑定数据状态::精确重复, 1, guard.Gread,
                     saved.事实代次};
      return {read.结果头, std::move(read.绑定)};
    }
    if (first.状态 != L1所有者范围读取状态::未找到)
      return {{first.状态 == L1所有者范围读取状态::资源失败
                   ? 状态使用绑定数据状态::资源失败
                   : 状态使用绑定数据状态::内部不一致,
               1, first.读取事实代次, std::nullopt},
              std::nullopt};
    if (first.读取事实代次 != r.G0)
      return {{状态使用绑定数据状态::事实代次漂移, 1, first.读取事实代次,
               std::nullopt},
              std::nullopt};
    if (first.首次规范化写集 || first.首次写入结果)
      return {{状态使用绑定数据状态::内部不一致, 1, first.读取事实代次,
               std::nullopt},
              std::nullopt};
    auto c = 确认当前场景角色({2, r.G0, r.发生场景});
    if (!c.成功({2, r.G0, r.发生场景}))
      return {{场景到B(c.状态), 1, c.Gread, std::nullopt}, std::nullopt};
    auto e = existence_.确认当前存在结构身份(r.G0, r.被描述存在);
    if (!e.成功(r.G0))
      return {{存在到B(e.状态), 1, e.Gread, std::nullopt}, std::nullopt};
    auto s = state_.读取当前状态({2, r.G0, r.状态});
    if (s.结果头.状态 != 状态类数据状态::已读取 || !s.内容)
      return {{状态到B(s.结果头.状态), 1, s.结果头.事实截止代次, std::nullopt},
              std::nullopt};
    if (s.结果头.事实截止代次 != r.G0 || s.内容->Gread != r.G0 ||
        s.内容->信息.身份 != r.状态)
      return {{状态使用绑定数据状态::内部不一致, 1, r.G0, std::nullopt},
              std::nullopt};
    commitStarted = true;
    auto saved = 串行提交(expected);
    published = saved.事实代次;
    auto status =
        映射绑定写入状态(saved, 状态使用绑定数据状态::已创建, owner_, expected);
    if (status == 状态使用绑定数据状态::精确重复 &&
        !竞争精确重复材料完整(saved, expected))
      return {
          {状态使用绑定数据状态::已可能发布, 1, saved.事实代次, saved.事实代次},
          std::nullopt};
    if (status != 状态使用绑定数据状态::已创建 &&
        status != 状态使用绑定数据状态::精确重复)
      return {{status, 1, saved.事实代次,
               status == 状态使用绑定数据状态::已可能发布
                   ? std::optional<std::uint64_t>{saved.事实代次}
                   : std::nullopt},
              std::nullopt};
    if (!写入头完整(saved, r.幂等身份, r.G0) || !写入映射完整(saved, expected))
      return {{saved.是否形成内存权威发布 ||
                       saved.状态 == L1所有者范围写入状态::精确重复
                   ? 状态使用绑定数据状态::已可能发布
                   : 状态使用绑定数据状态::内部不一致,
               1, saved.事实代次,
               saved.是否形成内存权威发布 ||
                       saved.状态 == L1所有者范围写入状态::精确重复
                   ? std::optional<std::uint64_t>{saved.事实代次}
                   : std::nullopt},
              std::nullopt};
    const auto bid = 映射编码(saved, {1});
    if (!bid)
      return {
          {状态使用绑定数据状态::已可能发布, 1, saved.事实代次, saved.事实代次},
          std::nullopt};
    const auto guard = 读取当前代次();
    if (!guard.成功())
      return {
          {状态使用绑定数据状态::已可能发布, 1, guard.Gread, saved.事实代次},
          std::nullopt};
    confirmed = guard.Gread;
    auto read = 读B(guard.Gread, {*bid});
    if (!read.成功())
      return {
          {状态使用绑定数据状态::已可能发布, 1, guard.Gread, saved.事实代次},
          std::nullopt};
    const auto finalGuard = 核验代次(guard.Gread);
    if (!finalGuard.成功())
      return {{状态使用绑定数据状态::已可能发布, 1, finalGuard.Gread,
               saved.事实代次},
              std::nullopt};
    read.结果头 = {status, 1, guard.Gread, saved.事实代次};
    return {read.结果头, std::move(read.绑定)};
  } catch (const std::bad_alloc &) {
    return {{commitStarted ? 状态使用绑定数据状态::已可能发布
                           : 状态使用绑定数据状态::资源失败,
             1, confirmed ? confirmed : published,
             commitStarted ? std::optional<std::uint64_t>{published}
                           : std::nullopt},
            std::nullopt};
  } catch (...) {
    return {{commitStarted ? 状态使用绑定数据状态::已可能发布
                           : 状态使用绑定数据状态::内部不一致,
             1, confirmed ? confirmed : published,
             commitStarted ? std::optional<std::uint64_t>{published}
                           : std::nullopt},
            std::nullopt};
  }
}

状态使用绑定退出结果
场景类数据服务::退出状态使用绑定(const 状态使用绑定退出请求 &r) {
  if (r.合同版本 != 1 || !r.G0 || r.G0 == UINT64_MAX ||
      !普通幂等身份有效(r.幂等身份) || !有效(r.身份))
    return {};
  bool commitStarted = false;
  std::uint64_t published = 0, confirmed = 0;
  try {
    const auto first =
        port_.读取首次写入材料({L1所有者范围首次写入读取合同版本, r.幂等身份});
    if (first.合同版本 != L1所有者范围首次写入读取合同版本 ||
        first.所有者 != owner_ || first.写入幂等身份 != r.幂等身份)
      return {{状态使用绑定数据状态::内部不一致, 1, first.读取事实代次,
               std::nullopt}};
    if (first.状态 == L1所有者范围读取状态::成功) {
      if (!first.首次规范化写集 || !first.首次写入结果)
        return {{状态使用绑定数据状态::内部不一致, 1, first.读取事实代次,
                 std::nullopt}};
      const auto &ws = *first.首次规范化写集;
      if (ws.期望事实代次 != r.G0 || ws.写入幂等身份 != r.幂等身份 ||
          !ws.节点.empty() || !ws.关系.empty() || !ws.值.empty() ||
          !ws.属性槽变更.empty() || ws.退出事实.size() != 4 ||
          !std::is_sorted(ws.退出事实.begin(), ws.退出事实.end()) ||
          std::find(ws.退出事实.begin(), ws.退出事实.end(), r.身份.编码) ==
              ws.退出事实.end() ||
          first.首次写入结果->状态 != L1所有者范围写入状态::成功 ||
          !写入头完整(*first.首次写入结果, r.幂等身份, r.G0) ||
          !写入映射完整(*first.首次写入结果, ws))
        return {{状态使用绑定数据状态::幂等冲突, 1, first.读取事实代次,
                 std::nullopt}};
      commitStarted = true;
      const auto saved = 串行提交(ws);
      published = saved.事实代次;
      if (saved.状态 != L1所有者范围写入状态::精确重复) {
        auto status =
            映射绑定写入状态(saved, 状态使用绑定数据状态::已删除, owner_, ws);
        if (status == 状态使用绑定数据状态::已删除)
          status = saved.是否形成内存权威发布
                       ? 状态使用绑定数据状态::已可能发布
                       : 状态使用绑定数据状态::内部不一致;
        return {{status, 1, saved.事实代次,
                  status == 状态使用绑定数据状态::已可能发布
                      ? std::optional<std::uint64_t>{saved.事实代次}
                      : std::nullopt}};
      }
      if (!写入头完整(saved, r.幂等身份, r.G0) || !写入映射完整(saved, ws) ||
          saved.新编码映射 != first.首次写入结果->新编码映射)
        return {{状态使用绑定数据状态::已可能发布, 1, saved.事实代次,
                 saved.事实代次}};
      const auto guard = 读取当前代次();
      if (!guard.成功())
        return {
            {状态使用绑定数据状态::已可能发布, 1, guard.Gread, saved.事实代次}};
      confirmed = guard.Gread;
      auto read = 读B(guard.Gread, r.身份);
      if (read.结果头.状态 != 状态使用绑定数据状态::未找到)
        return {
            {状态使用绑定数据状态::已可能发布, 1, guard.Gread, saved.事实代次}};
      const auto finalGuard = 核验代次(guard.Gread);
      if (!finalGuard.成功())
        return {{状态使用绑定数据状态::已可能发布, 1, finalGuard.Gread,
                 saved.事实代次}};
      return {
          {状态使用绑定数据状态::精确重复, 1, saved.事实代次, saved.事实代次}};
    }
    if (first.状态 != L1所有者范围读取状态::未找到)
      return {{first.状态 == L1所有者范围读取状态::资源失败
                    ? 状态使用绑定数据状态::资源失败
                    : 状态使用绑定数据状态::内部不一致,
                1, first.读取事实代次, std::nullopt}};
    if (first.读取事实代次 != r.G0)
      return {{状态使用绑定数据状态::事实代次漂移, 1, first.读取事实代次,
               std::nullopt}};
    if (first.首次规范化写集 || first.首次写入结果)
      return {{状态使用绑定数据状态::内部不一致, 1, first.读取事实代次,
               std::nullopt}};
    auto old = 读B(r.G0, r.身份);
    if (!old.成功())
      return {old.结果头};
    L1所有者范围写集请求 ws{L1所有者范围CRUD合同版本, r.G0, r.幂等身份};
    ws.退出事实 = {old.绑定->信息.身份.编码, old.绑定->场景成员关系,
                   old.绑定->存在成员关系, old.绑定->状态成员关系};
    std::sort(ws.退出事实.begin(), ws.退出事实.end());
    commitStarted = true;
    auto saved = 串行提交(ws);
    published = saved.事实代次;
    auto status =
        映射绑定写入状态(saved, 状态使用绑定数据状态::已删除, owner_, ws);
    if (status == 状态使用绑定数据状态::精确重复 &&
        !竞争精确重复材料完整(saved, ws))
      return {
          {状态使用绑定数据状态::已可能发布, 1, saved.事实代次, saved.事实代次}};
    if (status != 状态使用绑定数据状态::已删除 &&
        status != 状态使用绑定数据状态::精确重复)
      return {{status, 1, saved.事实代次,
               status == 状态使用绑定数据状态::已可能发布
                   ? std::optional<std::uint64_t>{saved.事实代次}
                   : std::nullopt}};
    if (!写入头完整(saved, r.幂等身份, r.G0) || !写入映射完整(saved, ws))
      return {{saved.是否形成内存权威发布 ||
                       saved.状态 == L1所有者范围写入状态::精确重复
                   ? 状态使用绑定数据状态::已可能发布
                   : 状态使用绑定数据状态::内部不一致,
               1, saved.事实代次,
               saved.是否形成内存权威发布 ||
                       saved.状态 == L1所有者范围写入状态::精确重复
                   ? std::optional<std::uint64_t>{saved.事实代次}
                   : std::nullopt}};
    const auto guard = 读取当前代次();
    if (!guard.成功())
      return {
          {状态使用绑定数据状态::已可能发布, 1, guard.Gread, saved.事实代次}};
    confirmed = guard.Gread;
    auto read = 读B(guard.Gread, r.身份);
    if (read.结果头.状态 != 状态使用绑定数据状态::未找到)
      return {
          {状态使用绑定数据状态::已可能发布, 1, guard.Gread, saved.事实代次}};
    const auto finalGuard = 核验代次(guard.Gread);
    if (!finalGuard.成功())
      return {{状态使用绑定数据状态::已可能发布, 1, finalGuard.Gread,
               saved.事实代次}};
    return {{status, 1, saved.事实代次, saved.事实代次}};
  } catch (const std::bad_alloc &) {
    return {{commitStarted ? 状态使用绑定数据状态::已可能发布
                           : 状态使用绑定数据状态::资源失败,
             1, confirmed ? confirmed : published,
             commitStarted ? std::optional<std::uint64_t>{published}
                           : std::nullopt}};
  } catch (...) {
    return {{commitStarted ? 状态使用绑定数据状态::已可能发布
                           : 状态使用绑定数据状态::内部不一致,
             1, confirmed ? confirmed : published,
             commitStarted ? std::optional<std::uint64_t>{published}
                           : std::nullopt}};
  }
}

} // namespace 海中鱼巣
namespace 海中鱼巣 {

状态使用绑定读取结果 场景类数据服务::读B(std::uint64_t g,
                                                 状态使用绑定身份 bid) const {
  auto n = 读节点(bid.编码, g);
  if (!n.成功())
    return {{n.状态 == 场景角色数据状态::未找到 ? 状态使用绑定数据状态::未找到
                                                : 场景到B(n.状态),
             1, n.Gread, std::nullopt},
            std::nullopt};
  if (n.事实->创建事实代次 > g)
    return {{状态使用绑定数据状态::未找到, 1, g, std::nullopt}, std::nullopt};
  auto q = 查询关系(g, 关系端点方向::源, bid.编码,
                    layout_.绑定结构.状态使用绑定成员关系类型);
  if (!q.成功())
    return {{场景到B(q.状态), 1, q.Gread, std::nullopt}, std::nullopt};
  if (q.关系.empty())
    return {{状态使用绑定数据状态::入口拒绝, 1, g, std::nullopt}, std::nullopt};
  if (q.关系.size() > 4096)
    return {{状态使用绑定数据状态::数量预算不足, 1, g, std::nullopt},
            std::nullopt};
  if (n.事实->写入所有者 != owner_ || n.事实->种类 != 节点种类::普通 ||
      n.事实->属性类型表示 || !n.事实->当前属性.empty() || q.关系.size() != 3)
    return {{状态使用绑定数据状态::内部不一致, 1, g, std::nullopt},
            std::nullopt};
  std::array<std::optional<L1所有者范围关系事实>, 3> edges;
  for (const auto &e : q.关系) {
    if (e.角色或顺序 < 1 || e.角色或顺序 > 3 || e.写入所有者 != owner_ ||
        e.创建事实代次 != n.事实->创建事实代次)
      return {{状态使用绑定数据状态::内部不一致, 1, g, std::nullopt},
              std::nullopt};
    auto &i = edges[static_cast<std::size_t>(e.角色或顺序 - 1)];
    if (i)
      return {{状态使用绑定数据状态::内部不一致, 1, g, std::nullopt},
              std::nullopt};
    i = e;
  }
  if (!edges[0] || !edges[1] || !edges[2])
    return {{状态使用绑定数据状态::内部不一致, 1, g, std::nullopt},
            std::nullopt};
  const auto C = edges[0]->目标节点, E = edges[1]->目标节点,
             S = edges[2]->目标节点;
  auto c = 读角色(g, C);
  if (!c.成功({2, g, C}))
    return {{场景到B(c.状态), 1, c.Gread, std::nullopt}, std::nullopt};
  auto e = existence_.读取当前存在身份来源见证(g, E);
  if (!e.成功(g, E))
    return {{存在到B(e.状态), 1, e.Gread, std::nullopt}, std::nullopt};
  auto s = state_.读取当前状态({2, g, {S}});
  if (s.结果头.状态 != 状态类数据状态::已读取 || !s.内容)
    return {{状态到B(s.结果头.状态), 1, s.结果头.事实截止代次, std::nullopt},
            std::nullopt};
  if (s.结果头.事实截止代次 != g || s.内容->Gread != g ||
      s.内容->信息.身份.编码 != S)
    return {{状态使用绑定数据状态::内部不一致, 1, g, std::nullopt},
            std::nullopt};
  状态使用绑定事实 fact{g,
                         {bid, C, E, {S}},
                         edges[0]->编码,
                         edges[1]->编码,
                         edges[2]->编码,
                         n.事实->创建事实代次};
  if (!状态使用绑定事实完整(fact))
    return {{状态使用绑定数据状态::内部不一致, 1, g, std::nullopt},
            std::nullopt};
  return {{状态使用绑定数据状态::已读取, 1, g, std::nullopt}, std::move(fact)};
}
状态使用绑定读取结果
场景类数据服务::读取当前状态使用绑定(const 状态使用绑定当前读取请求 &r) const {
  if (r.合同版本 != 1 || !r.G0 || !有效(r.身份))
    return {};
  std::uint64_t confirmed = 0;
  try {
    auto first = 核验代次(r.G0);
    if (!first.成功())
      return {{场景到B(first.状态), 1, first.Gread, std::nullopt},
              std::nullopt};
    confirmed = first.Gread;
    auto result = 读B(r.G0, r.身份);
    auto last = 核验代次(r.G0);
    if (!last.成功())
      return {{场景到B(last.状态), 1, last.Gread, std::nullopt}, std::nullopt};
    return result;
  } catch (const std::bad_alloc &) {
    return {{状态使用绑定数据状态::资源失败, 1, confirmed, std::nullopt},
            std::nullopt};
  } catch (...) {
    return {{状态使用绑定数据状态::内部不一致, 1, confirmed, std::nullopt},
            std::nullopt};
  }
}
状态使用绑定组查询结果
场景类数据服务::查B组(std::uint32_t v, std::uint64_t g, 稳定编码 target,
                      std::int64_t role, std::uint64_t budget) const {
  if (v != 1 || !g || !有效(target) || !budget || budget > 65536)
    return {};
  std::uint64_t confirmed = 0;
  try {
    auto first = 核验代次(g);
    if (!first.成功())
      return {{场景到B(first.状态), 1, first.Gread, std::nullopt}, {}};
    confirmed = first.Gread;
    auto q = 查询关系(g, 关系端点方向::目标, target,
                      layout_.绑定结构.状态使用绑定成员关系类型);
    if (!q.成功())
      return {{场景到B(q.状态), 1, q.Gread, std::nullopt}, {}};
    if (q.关系.size() > budget)
      return {{状态使用绑定数据状态::数量预算不足, 1, g, std::nullopt}, {}};
    std::vector<稳定编码> ids;
    for (const auto &e : q.关系)
      if (e.角色或顺序 == role)
        ids.push_back(e.源节点);
    std::sort(ids.begin(), ids.end());
    ids.erase(std::unique(ids.begin(), ids.end()), ids.end());
    状态使用绑定组查询结果 out{
        {状态使用绑定数据状态::已读取, 1, g, std::nullopt}, {}};
    for (auto id : ids) {
      auto b = 读B(g, {id});
      if (!b.成功())
        return {b.结果头, {}};
      out.绑定组.push_back(*b.绑定);
    }
    auto last = 核验代次(g);
    if (!last.成功())
      return {{场景到B(last.状态), 1, last.Gread, std::nullopt}, {}};
    return out;
  } catch (const std::bad_alloc &) {
    return {{状态使用绑定数据状态::资源失败, 1, confirmed, std::nullopt}, {}};
  } catch (...) {
    return {{状态使用绑定数据状态::内部不一致, 1, confirmed, std::nullopt}, {}};
  }
}
状态使用绑定组查询结果 场景类数据服务::按发生场景查询当前状态使用绑定组(
    const 状态使用绑定按发生场景查询请求 &r) const {
  return 查B组(r.合同版本, r.G0, r.发生场景, 1, r.最大候选数量);
}
状态使用绑定组查询结果
场景类数据服务::按被描述存在查询当前状态使用绑定组(
    const 状态使用绑定按被描述存在查询请求 &r) const {
  return 查B组(r.合同版本, r.G0, r.被描述存在, 2, r.最大候选数量);
}
状态使用绑定组查询结果 场景类数据服务::按状态查询当前状态使用绑定组(
    const 状态使用绑定按状态查询请求 &r) const {
  return 查B组(r.合同版本, r.G0, r.状态.编码, 3, r.最大候选数量);
}

} // namespace 海中鱼巣
namespace 海中鱼巣 {

场景直接包含扩展登记结果
场景类数据服务::登记直接包含扩展(const L1事实基座服务 &l1,
                                 L1所有者范围写端口 &port,
                                 const 场景直接包含扩展登记请求 &r) {
  场景直接包含扩展登记结果 out;
  out.Gread = r.G0;
  if (r.版本 != 1 || !r.G0 || r.G0 == UINT64_MAX || !port.绑定于(l1))
    return out;
  try {
    L1所有者范围写集请求 ws{L1所有者范围CRUD合同版本, r.G0,
                            场景直接包含扩展登记固定幂等身份};
    for (std::uint32_t i = 1; i <= 5; ++i)
      ws.节点.push_back({{i}, 节点种类::普通, std::nullopt});
    const auto first = port.读取首次写入材料(
        {L1所有者范围首次写入读取合同版本, 场景直接包含扩展登记固定幂等身份});
    if (first.状态 == L1所有者范围读取状态::成功) {
      if (!first.首次规范化写集 || !写集相同(*first.首次规范化写集, ws) ||
          !first.首次写入结果) {
        out.状态 = 场景直接包含状态::幂等冲突;
        out.Gread = first.读取事实代次;
        return out;
      }
    } else if (first.状态 != L1所有者范围读取状态::未找到) {
      out.状态 = first.状态 == L1所有者范围读取状态::资源失败
                     ? 场景直接包含状态::资源失败
                     : 场景直接包含状态::内部不一致;
      out.Gread = first.读取事实代次;
      return out;
    } else if (first.读取事实代次 != r.G0) {
      out.状态 = 场景直接包含状态::事实代次漂移;
      out.Gread = first.读取事实代次;
      return out;
    }
    const auto saved = port.提交所有者范围中性写集(ws);
    out.Gread = saved.事实代次;
    if (saved.状态 != L1所有者范围写入状态::成功 &&
        saved.状态 != L1所有者范围写入状态::精确重复) {
      switch (saved.状态) {
      case L1所有者范围写入状态::事实代次漂移:
        out.状态 = 场景直接包含状态::事实代次漂移;
        break;
      case L1所有者范围写入状态::幂等冲突:
        out.状态 = 场景直接包含状态::幂等冲突;
        break;
      case L1所有者范围写入状态::资源失败:
        out.状态 = 场景直接包含状态::资源失败;
        break;
      default:
        out.状态 = saved.是否形成内存权威发布 ? 场景直接包含状态::已可能发布
                                              : 场景直接包含状态::内部不一致;
        break;
      }
      return out;
    }
    if (saved.新编码映射.size() != 5) {
      out.状态 = 场景直接包含状态::已可能发布;
      return out;
    }
    std::array<稳定编码, 5> ids{};
    for (const auto &[key, id] : saved.新编码映射) {
      if (key.值 < 1 || key.值 > 5 || 有效(ids[key.值 - 1]) || !有效(id)) {
        out.状态 = 场景直接包含状态::已可能发布;
        return out;
      }
      ids[key.值 - 1] = id;
    }
    out.交付 = 场景直接包含扩展结构交付{ids[0], ids[1], ids[2], ids[3], ids[4]};
    out.首次发布H = r.G0 + 1;
    const auto guard = l1.读取中性当前事实代次({L1中性CRUD合同版本});
    if (guard.状态 != L1中性读取状态::成功 || guard.事实代次 < out.首次发布H) {
      out.状态 = 场景直接包含状态::已可能发布;
      out.Gread = guard.事实代次;
      return out;
    }
    out.Gread = guard.事实代次;
    out.状态 = saved.状态 == L1所有者范围写入状态::成功
                   ? 场景直接包含状态::已登记
                   : 场景直接包含状态::精确重复;
  } catch (const std::bad_alloc &) {
    out.状态 = 场景直接包含状态::资源失败;
  } catch (...) {
    out.状态 = 场景直接包含状态::内部不一致;
  }
  return out;
}

bool 场景类数据服务::布局形状有效(const 场景角色结构交付 &v) noexcept {
  const 稳定编码 ids[]{v.绑定结构.场景族锚点,
                       v.绑定结构.场景族归属关系类型,
                       v.绑定结构.状态使用绑定成员关系类型,
                       v.根绑定关系类型,
                       v.状态组织关系类型,
                       v.动态组织关系类型,
                       v.父场景语境关系类型};
  for (std::size_t i = 0; i < 7; ++i) {
    if (!有效(ids[i]))
      return false;
    for (std::size_t j = 0; j < i; ++j)
      if (ids[i] == ids[j])
        return false;
  }
  return true;
}

bool 场景类数据服务::包含布局形状有效(
    const 场景直接包含扩展结构交付 &v, const 场景角色结构交付 &base,
    const 场景特征组织扩展结构交付 &feature) noexcept {
  const 稳定编码 ids[]{v.树登记锚点, v.根标记关系类型, v.树归属关系类型,
                       v.直接存在成员关系类型, v.直接子场景关系类型};
  for (std::size_t i = 0; i < 5; ++i) {
    if (!有效(ids[i]))
      return false;
    for (std::size_t j = 0; j < i; ++j)
      if (ids[i] == ids[j])
        return false;
  }
  const 稳定编码 occupied[]{base.绑定结构.场景族锚点,
                            base.绑定结构.场景族归属关系类型,
                            base.绑定结构.状态使用绑定成员关系类型,
                            base.根绑定关系类型,
                            base.状态组织关系类型,
                             base.动态组织关系类型,
                             base.父场景语境关系类型,
                            feature.特征组织关系类型};
  for (const auto id : ids)
    for (const auto old : occupied)
      if (id == old)
        return false;
  return true;
}

bool 场景类数据服务::包含扩展登记材料匹配() const {
  const auto f = port_.读取首次写入材料(
      {L1所有者范围首次写入读取合同版本, 场景直接包含扩展登记固定幂等身份});
  if (f.状态 != L1所有者范围读取状态::成功 || !f.首次规范化写集 ||
      !f.首次写入结果 || f.所有者 != owner_)
    return false;
  const auto &ws = *f.首次规范化写集;
  if (ws.节点.size() != 5 || !ws.关系.empty() || !ws.值.empty() ||
      !ws.属性槽变更.empty() || !ws.退出事实.empty())
    return false;
  std::array<稳定编码, 5> ids{};
  for (const auto &[key, id] : f.首次写入结果->新编码映射) {
    if (key.值 < 1 || key.值 > 5 || 有效(ids[key.值 - 1]) || !有效(id))
      return false;
    ids[key.值 - 1] = id;
  }
  return ids == std::array<稳定编码, 5>{includeLayout_.树登记锚点,
                                        includeLayout_.根标记关系类型,
                                        includeLayout_.树归属关系类型,
                                        includeLayout_.直接存在成员关系类型,
                                        includeLayout_.直接子场景关系类型};
}

bool 场景类数据服务::登记材料匹配() const {
  const auto f =
      port_.读取首次写入材料({L1所有者范围首次写入读取合同版本, {1}});
  if (f.状态 != L1所有者范围读取状态::成功 ||
      f.合同版本 != L1所有者范围首次写入读取合同版本 || f.所有者 != owner_ ||
      f.写入幂等身份.值 != 1 || !f.首次规范化写集 || !f.首次写入结果)
    return false;
  const auto &ws = *f.首次规范化写集;
  if (ws.合同版本 != L1所有者范围CRUD合同版本 || ws.写入幂等身份.值 != 1 ||
      !ws.期望事实代次 || ws.期望事实代次 == UINT64_MAX ||
       ws.节点.size() != 7 || !ws.关系.empty() || !ws.值.empty() ||
      !ws.属性槽变更.empty() || !ws.退出事实.empty())
    return false;
  std::array<bool, 7> keys{};
  for (const auto &n : ws.节点) {
    if (n.本地键.值 < 1 || n.本地键.值 > 7 || keys[n.本地键.值 - 1] ||
        n.种类 != 节点种类::普通 || n.属性类型表示)
      return false;
    keys[n.本地键.值 - 1] = true;
  }
  const auto &first = *f.首次写入结果;
  if (first.状态 != L1所有者范围写入状态::成功 ||
      first.合同版本 != L1所有者范围CRUD合同版本 || first.所有者 != owner_ ||
      first.写入幂等身份.值 != 1 || first.事实代次 != ws.期望事实代次 + 1 ||
      !first.是否形成内存权威发布 ||
      first.重试边界 != L1所有者范围重试边界::不适用 ||
       first.新编码映射.size() != 7)
    return false;
  std::array<稳定编码, 7> ids{};
  for (const auto &m : first.新编码映射) {
    if (m.first.值 < 1 || m.first.值 > 7 || 有效(ids[m.first.值 - 1]) ||
        !有效(m.second))
      return false;
    ids[m.first.值 - 1] = m.second;
  }
  if (ids !=
      std::array<稳定编码, 7>{
          layout_.绑定结构.场景族锚点, layout_.绑定结构.场景族归属关系类型,
          layout_.绑定结构.状态使用绑定成员关系类型, layout_.根绑定关系类型,
          layout_.状态组织关系类型, layout_.动态组织关系类型,
          layout_.父场景语境关系类型})
    return false;
  const auto guard = 读取当前代次();
  if (!guard.成功() || guard.Gread < first.事实代次)
    return false;
  for (const auto id : ids) {
    const auto n = 读节点(id, guard.Gread);
    if (!n.成功() || n.事实->写入所有者 != owner_ ||
        n.事实->种类 != 节点种类::普通 || n.事实->属性类型表示 ||
        !n.事实->当前属性.empty() || n.事实->创建事实代次 != first.事实代次)
      return false;
  }
  return true;
}

场景类数据服务::关系读取
场景类数据服务::查询关系(std::uint64_t g, 关系端点方向 d, 稳定编码 endpoint,
                         稳定编码 type) const {
  if (d == 关系端点方向::源) {
    const auto q = l1_.读取所有者范围当前源关系组(
        {L1所有者范围CRUD合同版本, endpoint, type});
    if (q.合同版本 != L1所有者范围CRUD合同版本 || q.源节点 != endpoint ||
        q.关系类型节点 != type)
      return {场景角色数据状态::内部不一致, q.读取事实代次, {}};
    if (q.状态 == L1所有者范围读取状态::资源失败)
      return {场景角色数据状态::资源失败, q.读取事实代次, {}};
    if (q.读取事实代次 != g)
      return {场景角色数据状态::事实代次漂移, q.读取事实代次, {}};
    if (q.状态 != L1所有者范围读取状态::成功)
      return {场景角色数据状态::内部不一致, g, {}};
    return {场景角色数据状态::已读取, g, q.关系组};
  }
  const auto q = l1_.读取所有者范围当前目标关系组(
      {L1所有者范围CRUD合同版本, endpoint, type});
  if (q.合同版本 != L1所有者范围CRUD合同版本 || q.目标节点 != endpoint ||
      q.关系类型节点 != type)
    return {场景角色数据状态::内部不一致, q.读取事实代次, {}};
  if (q.状态 == L1所有者范围读取状态::资源失败)
    return {场景角色数据状态::资源失败, q.读取事实代次, {}};
  if (q.读取事实代次 != g)
    return {场景角色数据状态::事实代次漂移, q.读取事实代次, {}};
  if (q.状态 != L1所有者范围读取状态::成功)
    return {场景角色数据状态::内部不一致, g, {}};
  return {场景角色数据状态::已读取, g, q.关系组};
}

场景类数据服务::节点读取 场景类数据服务::读节点(稳定编码 id,
                                                       std::uint64_t g) const {
  auto q = l1_.读取所有者范围当前节点({L1所有者范围CRUD合同版本, id});
  if (q.状态 == L1所有者范围读取状态::资源失败)
    return {场景角色数据状态::资源失败, q.读取事实代次, std::nullopt};
  if (q.读取事实代次 != g)
    return {场景角色数据状态::事实代次漂移, q.读取事实代次, std::nullopt};
  if (q.状态 == L1所有者范围读取状态::未找到)
    return {场景角色数据状态::未找到, g, std::nullopt};
  if (q.状态 != L1所有者范围读取状态::成功 ||
      q.合同版本 != L1所有者范围CRUD合同版本 || q.查询编码 != id || !q.事实)
    return {场景角色数据状态::内部不一致, g, std::nullopt};
  if (const auto *node = std::get_if<L1所有者范围节点事实>(&*q.事实))
    return {场景角色数据状态::已读取, g, *node};
  return {场景角色数据状态::内部不一致, g, std::nullopt};
}
场景类数据服务::关系事实读取
场景类数据服务::读关系(稳定编码 id, std::uint64_t g) const {
  auto q = l1_.读取所有者范围当前关系({L1所有者范围CRUD合同版本, id});
  if (q.状态 == L1所有者范围读取状态::资源失败)
    return {场景角色数据状态::资源失败, q.读取事实代次, std::nullopt};
  if (q.读取事实代次 != g)
    return {场景角色数据状态::事实代次漂移, q.读取事实代次, std::nullopt};
  if (q.状态 == L1所有者范围读取状态::未找到)
    return {场景角色数据状态::未找到, g, std::nullopt};
  if (q.状态 != L1所有者范围读取状态::成功 ||
      q.合同版本 != L1所有者范围CRUD合同版本 || q.查询编码 != id || !q.事实)
    return {场景角色数据状态::内部不一致, g, std::nullopt};
  if (const auto *edge = std::get_if<L1所有者范围关系事实>(&*q.事实))
    return {场景角色数据状态::已读取, g, *edge};
  return {场景角色数据状态::内部不一致, g, std::nullopt};
}

场景角色当前读取结果 场景类数据服务::读角色(std::uint64_t g,
                                               稳定编码 c) const {
  return 读当前完整角色_v2(g, c);
}
场景角色当前读取结果
场景类数据服务::读当前完整角色_v2(std::uint64_t g, 稳定编码 c) const {
  auto 失败 = [&](场景角色数据状态 s, std::uint64_t gr) {
    return 场景角色当前读取结果{2, s, gr, std::nullopt};
  };
  if (!g || !有效(c))
    return 失败(场景角色数据状态::入口拒绝, g);
  const auto ew = existence_.读取当前存在身份来源见证(g, c);
  if (!ew.成功(g, c))
    return 失败(存在到场景(ew.状态), ew.Gread);

  const L1节点当前完整引用读取请求_v2 refRequest{
      L1节点当前完整引用读取合同版本, c, g};
  const auto refs = l1_.读取节点全部当前引用_v2(refRequest);
  if (!L1当前引用读取成功(refs, refRequest)) {
    if (refs.状态 == L1节点当前完整引用读取状态_v2::事实代次漂移)
      return 失败(场景角色数据状态::事实代次漂移, refs.读取事实代次);
    if (refs.状态 == L1节点当前完整引用读取状态_v2::资源失败)
      return 失败(场景角色数据状态::资源失败, refs.读取事实代次);
    return 失败(场景角色数据状态::内部不一致, refs.读取事实代次);
  }
  std::vector<L1所有者范围关系事实> registrations;
  std::vector<L1所有者范围关系事实> currentRoots;
  for (const auto &ref : refs.引用) {
    const auto *edge = std::get_if<L1所有者范围关系事实>(&ref);
    if (!edge || edge->源节点 != c)
      continue;
    const bool registration =
        edge->关系类型节点 == layout_.绑定结构.场景族归属关系类型;
    const bool root = edge->关系类型节点 == layout_.根绑定关系类型;
    if (!registration && !root)
      continue;
    if (edge->写入所有者 != owner_ || !有效(edge->编码) ||
        !有效(edge->目标节点) || !edge->创建事实代次 ||
        edge->创建事实代次 > g ||
        (registration &&
         (edge->目标节点 != layout_.绑定结构.场景族锚点 ||
          edge->角色或顺序 != 1)) ||
        (root && (edge->角色或顺序 < 1 || edge->角色或顺序 > 4)))
      return 失败(场景角色数据状态::内部不一致, g);
    (registration ? registrations : currentRoots).push_back(*edge);
  }
  if (registrations.empty() && currentRoots.empty())
    return 失败(场景角色数据状态::场景角色未启用, g);
  if (registrations.size() != 1)
    return 失败(场景角色数据状态::内部不一致, g);
  const auto &registration = registrations.front();
  const auto gc = registration.创建事实代次;

  if (currentRoots.empty())
    return 失败(场景角色数据状态::场景角色未启用, g);
  std::vector<L1所有者范围关系事实> roots = currentRoots;
  if (roots.size() != 4 || (!currentRoots.empty() && currentRoots.size() != 4))
    return 失败(场景角色数据状态::内部不一致, g);

  场景角色当前事实 fact;
  fact.Gread = g;
  fact.场景 = c;
  fact.对象存在来源 = *ew.见证;
  fact.场景角色登记边 = 转边(registration);
  const std::array<稳定编码, 3> metas{
      layout_.绑定结构.场景族锚点,
      layout_.绑定结构.场景族归属关系类型,
      layout_.根绑定关系类型};

  std::array<bool, 4> seen{};
  std::array<L1所有者范围关系事实, 4> rootEdges{};
  std::array<稳定编码, 4> rootIds{};
  for (const auto &edge : roots) {
    if (edge.源节点 != c || edge.关系类型节点 != layout_.根绑定关系类型 ||
        edge.写入所有者 != owner_ || edge.角色或顺序 < 1 ||
        edge.角色或顺序 > 4 || !有效(edge.编码) ||
        !有效(edge.目标节点) || edge.创建事实代次 != gc)
      return 失败(场景角色数据状态::内部不一致, g);
    const auto index = static_cast<std::size_t>(edge.角色或顺序 - 1);
    if (seen[index])
      return 失败(场景角色数据状态::内部不一致, g);
    seen[index] = true;
    rootEdges[index] = edge;
    rootIds[index] = edge.目标节点;
  }

  L1所有者范围一致当前读取请求 projectionRequest;
  projectionRequest.合同版本 = L1所有者范围一致当前读取合同版本;
  projectionRequest.期望事实代次 = g;
  projectionRequest.所有者 = {owner_};
  projectionRequest.节点.assign(metas.begin(), metas.end());
  projectionRequest.节点.insert(projectionRequest.节点.end(), rootIds.begin(),
                                rootIds.end());
  projectionRequest.关系 = {registration.编码};
  for (const auto &edge : rootEdges)
    projectionRequest.关系.push_back(edge.编码);
  const auto projection = l1_.尝试读取所有者范围一致当前投影(projectionRequest);
  if (projection.状态 == L1所有者范围一致当前读取状态::事实代次漂移)
    return 失败(场景角色数据状态::事实代次漂移, projection.读取事实代次);
  if (projection.状态 == L1所有者范围一致当前读取状态::资源失败)
    return 失败(场景角色数据状态::资源失败, projection.读取事实代次);
  if (projection.状态 != L1所有者范围一致当前读取状态::成功 ||
      projection.合同版本 != L1所有者范围一致当前读取合同版本 ||
      projection.期望事实代次 != g || projection.读取事实代次 != g ||
      projection.节点.size() != projectionRequest.节点.size() ||
      projection.关系.size() != projectionRequest.关系.size())
    return 失败(场景角色数据状态::内部不一致, projection.读取事实代次);

  auto findNode = [&](稳定编码 id) -> const L1所有者范围节点事实 * {
    const auto it = std::find_if(projection.节点.begin(), projection.节点.end(),
                                 [&](const auto &x) { return x.查询编码 == id; });
    return it != projection.节点.end() &&
                   it->状态 == L1所有者范围一致当前读取项目状态::成功 &&
                   it->事实
               ? &*it->事实
               : nullptr;
  };
  auto findEdge = [&](稳定编码 id) -> const L1所有者范围关系事实 * {
    const auto it = std::find_if(projection.关系.begin(), projection.关系.end(),
                                 [&](const auto &x) { return x.查询编码 == id; });
    return it != projection.关系.end() &&
                   it->状态 == L1所有者范围一致当前读取项目状态::成功 &&
                   it->事实
               ? &*it->事实
               : nullptr;
  };
  const auto *projectedRegistration = findEdge(registration.编码);
  if (!projectedRegistration || *projectedRegistration != registration)
    return 失败(场景角色数据状态::内部不一致, g);
  std::array<场景节点见证 *, 3> metaWitnesses{
      &fact.场景族锚点, &fact.场景族归属类型, &fact.根绑定类型};
  for (std::size_t i = 0; i < metas.size(); ++i) {
    const auto *node = findNode(metas[i]);
    if (!node || node->写入所有者 != owner_ || node->种类 != 节点种类::普通 ||
        node->属性类型表示 || !node->当前属性.empty() ||
        !node->创建事实代次 || node->创建事实代次 > gc ||
        false)
      return 失败(场景角色数据状态::内部不一致, g);
    *metaWitnesses[i] = 转节点(*node);
  }
  for (std::size_t i = 0; i < 4; ++i) {
    const auto *edge = findEdge(rootEdges[i].编码);
    const auto *node = findNode(rootIds[i]);
    if (!edge || !node || *edge != rootEdges[i] ||
        node->写入所有者 != owner_ || node->种类 != 节点种类::普通 ||
        node->属性类型表示 || !node->当前属性.empty() ||
        node->创建事实代次 != gc || rootIds[i] == c)
      return 失败(场景角色数据状态::内部不一致, g);
    for (std::size_t j = 0; j < i; ++j)
      if (rootIds[i] == rootIds[j])
        return 失败(场景角色数据状态::内部不一致, g);
    if (std::ranges::find(metas, rootIds[i]) != metas.end())
      return 失败(场景角色数据状态::内部不一致, g);

    const L1节点当前完整引用读取请求_v2 inboundRequest{
        L1节点当前完整引用读取合同版本, rootIds[i], g};
    const auto inbound = l1_.读取节点全部当前引用_v2(inboundRequest);
    if (!L1当前引用读取成功(inbound, inboundRequest)) {
      if (inbound.状态 == L1节点当前完整引用读取状态_v2::事实代次漂移)
        return 失败(场景角色数据状态::事实代次漂移, inbound.读取事实代次);
      if (inbound.状态 == L1节点当前完整引用读取状态_v2::资源失败)
        return 失败(场景角色数据状态::资源失败, inbound.读取事实代次);
      return 失败(场景角色数据状态::内部不一致, inbound.读取事实代次);
    }
    std::size_t count = 0;
    for (const auto &ref : inbound.引用) {
      const auto *candidate = std::get_if<L1所有者范围关系事实>(&ref);
      if (!candidate || candidate->目标节点 != rootIds[i] ||
          candidate->关系类型节点 != layout_.根绑定关系类型)
        continue;
      if (*candidate != rootEdges[i])
        return 失败(场景角色数据状态::内部不一致, g);
      ++count;
    }
    if (count != 1)
      return 失败(场景角色数据状态::内部不一致, g);
    fact.四根[i] = {static_cast<场景根角色>(i + 1), 转节点(*node), 转边(*edge)};
  }
  场景角色当前读取结果 result{2, 场景角色数据状态::已读取, g,
                            std::move(fact)};
  return result.成功({2, g, c})
             ? result
             : 失败(场景角色数据状态::内部不一致, g);
}

场景当前身份结果
场景类数据服务::确认当前场景角色(const 场景当前身份请求 &r) const {
  if (r.版本 != 2 || !r.G0 || !有效(r.场景))
    return {};
  std::uint64_t confirmed = 0;
  try {
    auto first = 核验代次(r.G0);
    if (!first.成功())
      return {2, first.状态, first.Gread};
    confirmed = first.Gread;
    auto x = 读当前完整角色_v2(r.G0, r.场景);
    auto last = 核验代次(r.G0);
    if (!last.成功())
      return {2, last.状态, last.Gread};
    return {2, x.状态, x.Gread};
  } catch (const std::bad_alloc &) {
    return {2, 场景角色数据状态::资源失败, confirmed};
  } catch (...) {
    return {2, 场景角色数据状态::内部不一致, confirmed};
  }
}
场景角色当前读取结果
场景类数据服务::读取当前场景角色(const 场景角色当前读取请求 &r) const {
  if (r.版本 != 2 || !r.G0 || !有效(r.场景))
    return {};
  std::uint64_t confirmed = 0;
  try {
    auto first = 核验代次(r.G0);
    if (!first.成功())
      return {2, first.状态, first.Gread, std::nullopt};
    confirmed = first.Gread;
    auto result = 读当前完整角色_v2(r.G0, r.场景);
    auto last = 核验代次(r.G0);
    if (!last.成功())
      return {2, last.状态, last.Gread, std::nullopt};
    return result;
  } catch (const std::bad_alloc &) {
    return {2, 场景角色数据状态::资源失败, confirmed, std::nullopt};
  } catch (...) {
    return {2, 场景角色数据状态::内部不一致, confirmed, std::nullopt};
  }
}

场景父语境读取结果
场景类数据服务::读取当前父场景语境(const 场景父语境读取请求 &r) const {
  场景父语境读取结果 out{1, 场景角色数据状态::入口拒绝, r.G0, {}};
  if (r.版本 != 1 || !r.G0 || !有效(r.场景) ||
      r.最大关系数量 < 1 || r.最大关系数量 > 4096)
    return out;
  try {
    const auto edges = 查询关系(r.G0,
                                  关系端点方向::源, r.场景,
                                  layout_.父场景语境关系类型);
    out.Gread = edges.Gread;
    if (!edges.成功()) {
      out.状态 = edges.状态;
      return out;
    }
    if (edges.关系.size() > r.最大关系数量) {
      out.状态 = 场景角色数据状态::数量预算不足;
      return out;
    }
    if (edges.关系.empty()) {
      out.状态 = 场景角色数据状态::未找到;
      return out;
    }
    if (edges.关系.size() != 1) {
      out.状态 = 场景角色数据状态::内部不一致;
      return out;
    }
    const auto &edge = edges.关系.front();
    if (edge.源节点 != r.场景 || !有效(edge.目标节点) ||
        edge.关系类型节点 != layout_.父场景语境关系类型 ||
        edge.角色或顺序 != 1 || edge.写入所有者 != owner_ ||
        false) {
      out.状态 = 场景角色数据状态::内部不一致;
      return out;
    }
    直接归属联合只读组合器 joint(existenceStructure_, *this);
    const 直接归属联合父读取请求 parentRequest{
        1, r.G0, r.场景, r.最大关系数量};
    const auto parent = joint.读取当前联合父(parentRequest);
    if (!parent.父读取成功(parentRequest) || !parent.父 ||
        parent.父->成员 != r.场景 || parent.父->父 != edge.目标节点) {
      out.状态 = 场景角色数据状态::内部不一致;
      return out;
    }
    out.投影 = 场景父语境投影事实{r.G0, r.场景, edge.目标节点,
                                         *parent.父, 转边(edge)};
    out.状态 = 场景角色数据状态::已读取;
    if (!out.成功(r)) {
      out.投影.reset();
      out.状态 = 场景角色数据状态::内部不一致;
    }
  } catch (const std::bad_alloc &) {
    out.状态 = 场景角色数据状态::资源失败;
  } catch (const std::length_error &) {
    out.状态 = 场景角色数据状态::资源失败;
  } catch (...) {
    out.状态 = 场景角色数据状态::内部不一致;
  }
  return out;
}

bool 场景类数据服务::当前父场景语境私有结果_v2::成功(
    std::uint64_t g, 稳定编码 scene,
    const 直接归属联合事实 &parent) const noexcept {
  const auto same = [](const 直接归属联合事实 &a,
                       const 直接归属联合事实 &b) noexcept {
    return a.Gread == b.Gread && a.来源 == b.来源 &&
           a.关系 == b.关系 && a.父 == b.父 && a.成员 == b.成员 &&
           a.创建事实代次 == b.创建事实代次;
  };
  return g && 有效(scene) && 状态 == 场景角色数据状态::已读取 &&
         Gread == g && 投影 && 投影->Gread == g &&
         投影->场景 == scene && 投影->父场景语境 == parent.父 &&
         same(投影->结构父, parent) && 有效(投影->投影边.编码) &&
         投影->投影边.源 == scene && 投影->投影边.目标 == parent.父 &&
         投影->投影边.角色或顺序 == 1 &&
         投影->投影边.生命周期.创建事实代次 &&
         投影->投影边.生命周期.创建事实代次 <= g;
}

bool 场景类数据服务::当前场景树证明私有结果_v2::成功(
    std::uint64_t g, 稳定编码 scene, 稳定编码 root,
    const 直接归属场景角色事实 &role) const noexcept {
  if (!g || !有效(scene) || !有效(root) ||
      状态 != 场景角色数据状态::已读取 || Gread != g || !证明 ||
      role.Gread != g || role.场景 != scene ||
      !role.树根 || *role.树根 != root || !role.树证明关系 ||
      证明->Gread != g || 证明->关系 != *role.树证明关系 ||
      证明->场景 != scene || 证明->树根 != root ||
      证明->见证.编码 != 证明->关系 || 证明->见证.源 != scene ||
      证明->见证.角色或顺序 != 1 ||
      !证明->见证.生命周期.创建事实代次 ||
      证明->见证.生命周期.创建事实代次 > g)
    return false;
  if (role.位置 == 直接归属场景位置::场景树根)
    return scene == root && 证明->种类 == 场景树证明种类::根标记;
  return role.位置 == 直接归属场景位置::场景树非根 && scene != root &&
         证明->种类 == 场景树证明种类::树归属;
}

场景类数据服务::当前父场景语境私有结果_v2
场景类数据服务::读当前父场景语境_v2(
    std::uint64_t g, 稳定编码 scene,
    const 直接归属联合事实 &parent) const {
  当前父场景语境私有结果_v2 out;
  out.Gread = g;
  if (!g || !有效(scene) || !有效(parent.关系) || !有效(parent.父) ||
      parent.成员 != scene || parent.Gread != g ||
      !parent.创建事实代次 || parent.创建事实代次 > g)
    return out;
  try {
    const L1节点当前完整引用读取请求_v2 request{
        L1节点当前完整引用读取合同版本, scene, g};
    const auto refs = l1_.读取节点全部当前引用_v2(request);
    out.Gread = refs.读取事实代次;
    if (!L1当前引用读取成功(refs, request)) {
      out.状态 = refs.状态 == L1节点当前完整引用读取状态_v2::事实代次漂移
                       ? 场景角色数据状态::事实代次漂移
                   : refs.状态 == L1节点当前完整引用读取状态_v2::资源失败
                       ? 场景角色数据状态::资源失败
                       : 场景角色数据状态::内部不一致;
      return out;
    }
    std::vector<L1所有者范围关系事实> edges;
    for (const auto &ref : refs.引用) {
      const auto *edge = std::get_if<L1所有者范围关系事实>(&ref);
      if (!edge || edge->源节点 != scene ||
          edge->关系类型节点 != layout_.父场景语境关系类型)
        continue;
      if (edge->写入所有者 != owner_ || !有效(edge->编码) ||
          !有效(edge->目标节点) || edge->目标节点 == scene ||
          edge->角色或顺序 != 1 || !edge->创建事实代次 ||
          edge->创建事实代次 > g) {
        out.状态 = 场景角色数据状态::内部不一致;
        return out;
      }
      edges.push_back(*edge);
    }
    if (edges.empty()) {
      out.状态 = 场景角色数据状态::未找到;
      return out;
    }
    if (edges.size() != 1 || edges.front().目标节点 != parent.父) {
      out.状态 = 场景角色数据状态::内部不一致;
      return out;
    }
    out.投影 = 场景父语境投影事实{g, scene, parent.父, parent,
                                         转边(edges.front())};
    out.状态 = 场景角色数据状态::已读取;
    if (!out.成功(g, scene, parent)) {
      out.投影.reset();
      out.状态 = 场景角色数据状态::内部不一致;
    }
  } catch (const std::bad_alloc &) {
    out.状态 = 场景角色数据状态::资源失败;
  } catch (const std::length_error &) {
    out.状态 = 场景角色数据状态::资源失败;
  } catch (const std::system_error &) {
    out.状态 = 场景角色数据状态::资源失败;
  } catch (...) {
    out.状态 = 场景角色数据状态::内部不一致;
  }
  if (out.状态 != 场景角色数据状态::已读取)
    out.投影.reset();
  return out;
}

场景类数据服务::当前场景树证明私有结果_v2
场景类数据服务::读当前场景树证明_v2(
    std::uint64_t g, 稳定编码 scene, 稳定编码 root,
    const 直接归属场景角色事实 &role) const {
  当前场景树证明私有结果_v2 out;
  out.Gread = g;
  if (!g || !有效(scene) || !有效(root) || role.Gread != g ||
      role.场景 != scene || !role.树根 || *role.树根 != root ||
      !role.树证明关系 ||
      (role.位置 != 直接归属场景位置::场景树根 &&
       role.位置 != 直接归属场景位置::场景树非根))
    return out;
  try {
    const auto read = 读关系(*role.树证明关系, g);
    out.Gread = read.Gread;
    if (!read.成功()) {
      out.状态 = read.状态 == 场景角色数据状态::事实代次漂移 ||
                           read.状态 == 场景角色数据状态::资源失败
                       ? read.状态
                       : 场景角色数据状态::内部不一致;
      return out;
    }
    const bool isRoot = role.位置 == 直接归属场景位置::场景树根;
    const auto &edge = *read.事实;
    if (edge.源节点 != scene || edge.写入所有者 != owner_ ||
        edge.角色或顺序 != 1 || !edge.创建事实代次 ||
        edge.创建事实代次 > g ||
        edge.关系类型节点 !=
            (isRoot ? includeLayout_.根标记关系类型
                    : includeLayout_.树归属关系类型) ||
        edge.目标节点 != (isRoot ? includeLayout_.树登记锚点 : root)) {
      out.状态 = 场景角色数据状态::内部不一致;
      return out;
    }
    out.证明 = 场景树证明事实{g,
                                  isRoot ? 场景树证明种类::根标记
                                         : 场景树证明种类::树归属,
                                  edge.编码, scene, root, 转边(edge)};
    out.状态 = 场景角色数据状态::已读取;
    if (!out.成功(g, scene, root, role)) {
      out.证明.reset();
      out.状态 = 场景角色数据状态::内部不一致;
    }
  } catch (const std::bad_alloc &) {
    out.状态 = 场景角色数据状态::资源失败;
  } catch (const std::length_error &) {
    out.状态 = 场景角色数据状态::资源失败;
  } catch (const std::system_error &) {
    out.状态 = 场景角色数据状态::资源失败;
  } catch (...) {
    out.状态 = 场景角色数据状态::内部不一致;
  }
  if (out.状态 != 场景角色数据状态::已读取)
    out.证明.reset();
  return out;
}

场景直接包含状态 包含读取状态(场景角色数据状态 s) noexcept {
  switch (s) {
  case 场景角色数据状态::已读取:
    return 场景直接包含状态::已读取;
  case 场景角色数据状态::未找到:
    return 场景直接包含状态::成员未找到;
  case 场景角色数据状态::事实代次漂移:
    return 场景直接包含状态::事实代次漂移;
  case 场景角色数据状态::资源失败:
    return 场景直接包含状态::资源失败;
  case 场景角色数据状态::入口拒绝:
    return 场景直接包含状态::入口拒绝;
  default:
    return 场景直接包含状态::内部不一致;
  }
}
直接归属联合只读状态 联合读取状态(存在结构身份只读状态 s) noexcept {
  switch (s) {
  case 存在结构身份只读状态::事实代次漂移:
    return 直接归属联合只读状态::事实代次漂移;
  case 存在结构身份只读状态::资源失败:
    return 直接归属联合只读状态::资源失败;
  case 存在结构身份只读状态::入口拒绝:
    return 直接归属联合只读状态::入口拒绝;
  case 存在结构身份只读状态::未找到:
    return 直接归属联合只读状态::成员未找到;
  default:
    return 直接归属联合只读状态::内部不一致;
  }
}
直接归属联合只读状态 联合读取状态(场景角色数据状态 s) noexcept {
  switch (s) {
  case 场景角色数据状态::事实代次漂移:
    return 直接归属联合只读状态::事实代次漂移;
  case 场景角色数据状态::资源失败:
    return 直接归属联合只读状态::资源失败;
  case 场景角色数据状态::入口拒绝:
    return 直接归属联合只读状态::入口拒绝;
  default:
    return 直接归属联合只读状态::内部不一致;
  }
}
场景直接包含状态 联合到包含(直接归属联合只读状态 s) noexcept {
  switch (s) {
  case 直接归属联合只读状态::事实代次漂移:
    return 场景直接包含状态::事实代次漂移;
  case 直接归属联合只读状态::数量预算不足:
    return 场景直接包含状态::数量预算不足;
  case 直接归属联合只读状态::资源失败:
    return 场景直接包含状态::资源失败;
  case 直接归属联合只读状态::入口拒绝:
    return 场景直接包含状态::入口拒绝;
  default:
    return 场景直接包含状态::内部不一致;
  }
}
场景直接包含状态 存在到包含(存在结构身份只读状态 s) noexcept {
  switch (s) {
  case 存在结构身份只读状态::事实代次漂移:
    return 场景直接包含状态::事实代次漂移;
  case 存在结构身份只读状态::资源失败:
    return 场景直接包含状态::资源失败;
  case 存在结构身份只读状态::未找到:
    return 场景直接包含状态::成员未找到;
  default:
    return 场景直接包含状态::内部不一致;
  }
}

场景直接包含组结果
场景类数据服务::读取当前场景包含父组(const 场景直接包含反向读取请求 &r) const {
  场景直接包含组结果 out{
      {场景直接包含状态::入口拒绝, 1, r.G0, std::nullopt}, {}};
  if (r.版本 != 1 || !r.G0 || !有效(r.成员) || r.最大关系数量 < 1 ||
      r.最大关系数量 > 4096)
    return out;
  try {
    for (const auto [kind, type] :
         std::array<std::pair<场景直接包含种类, 稳定编码>, 2>{
             {{场景直接包含种类::存在成员, includeLayout_.直接存在成员关系类型},
              {场景直接包含种类::子场景, includeLayout_.直接子场景关系类型}}}) {
      auto q =
          查询关系(r.G0, 关系端点方向::目标, r.成员, type);
      if (!q.成功()) {
        out.结果头.状态 = 包含读取状态(q.状态);
        out.结果头.Gread = q.Gread;
        out.包含组.clear();
        return out;
      }
      for (const auto &e : q.关系) {
        if (e.写入所有者 != owner_ || e.目标节点 != r.成员 ||
            e.关系类型节点 != type || e.角色或顺序 != 1) {
          out.结果头.状态 = 场景直接包含状态::内部不一致;
          out.包含组.clear();
          return out;
        }
        out.包含组.push_back({r.G0, kind, e.源节点, e.目标节点, 转边(e)});
      }
    }
    if (out.包含组.size() > r.最大关系数量) {
      out.结果头.状态 = 场景直接包含状态::数量预算不足;
      out.包含组.clear();
      return out;
    }
    std::sort(out.包含组.begin(), out.包含组.end(),
              [](const auto &a, const auto &b) {
                return a.关系.编码.值 < b.关系.编码.值;
              });
    out.结果头.状态 = 场景直接包含状态::已读取;
  } catch (const std::bad_alloc &) {
    out.结果头.状态 = 场景直接包含状态::资源失败;
    out.包含组.clear();
  } catch (...) {
    out.结果头.状态 = 场景直接包含状态::内部不一致;
    out.包含组.clear();
  }
  return out;
}

场景直接包含组结果
场景类数据服务::读取当前场景包含子组(const 场景直接包含组读取请求 &r) const {
  场景直接包含组结果 out{
      {场景直接包含状态::入口拒绝, 1, r.G0, std::nullopt}, {}};
  if (r.版本 != 1 || !r.G0 || !有效(r.父场景) || r.最大关系数量 < 1 ||
      r.最大关系数量 > 4096)
    return out;
  try {
    for (const auto [kind, type] :
         std::array<std::pair<场景直接包含种类, 稳定编码>, 2>{
             {{场景直接包含种类::存在成员, includeLayout_.直接存在成员关系类型},
              {场景直接包含种类::子场景, includeLayout_.直接子场景关系类型}}}) {
      auto q =
          查询关系(r.G0, 关系端点方向::源, r.父场景, type);
      if (!q.成功()) {
        out.结果头.状态 = 包含读取状态(q.状态);
        out.结果头.Gread = q.Gread;
        out.包含组.clear();
        return out;
      }
      for (const auto &e : q.关系) {
        if (e.写入所有者 != owner_ || e.源节点 != r.父场景 ||
            e.关系类型节点 != type || e.角色或顺序 != 1) {
          out.结果头.状态 = 场景直接包含状态::内部不一致;
          out.包含组.clear();
          return out;
        }
        out.包含组.push_back({r.G0, kind, e.源节点, e.目标节点, 转边(e)});
      }
    }
    if (out.包含组.size() > r.最大关系数量) {
      out.结果头.状态 = 场景直接包含状态::数量预算不足;
      out.包含组.clear();
      return out;
    }
    std::sort(out.包含组.begin(), out.包含组.end(),
              [](const auto &a, const auto &b) {
                return a.关系.编码.值 < b.关系.编码.值;
              });
    out.结果头.状态 = 场景直接包含状态::已读取;
  } catch (const std::bad_alloc &) {
    out.结果头.状态 = 场景直接包含状态::资源失败;
    out.包含组.clear();
  } catch (...) {
    out.结果头.状态 = 场景直接包含状态::内部不一致;
    out.包含组.clear();
  }
  return out;
}

场景直接包含当前完整读取结果_v2
场景类数据服务::读取当前场景包含父组_v2(
    const 场景直接包含父组当前完整读取请求_v2 &r) const {
  场景直接包含当前完整读取结果_v2 out;
  out.Gread = r.G0;
  if (r.版本 != 场景直接包含当前完整读取合同版本_v2 || !r.G0 ||
      !有效(r.成员))
    return out;
  try {
    const L1节点当前完整引用读取请求_v2 请求{
        L1节点当前完整引用读取合同版本, r.成员, r.G0};
    const auto 读取 = l1_.读取节点全部当前引用_v2(请求);
    out.Gread = 读取.读取事实代次;
    if (读取.状态 != L1节点当前完整引用读取状态_v2::成功
        || 读取.合同版本 != L1节点当前完整引用读取合同版本
        || 读取.节点 != 请求.节点
        || 读取.期望事实代次 != 请求.期望事实代次
        || 读取.读取事实代次 != 请求.期望事实代次) {
      switch (读取.状态) {
      case L1节点当前完整引用读取状态_v2::未找到:
        out.状态 = 场景直接包含当前完整读取状态_v2::未找到;
        break;
      case L1节点当前完整引用读取状态_v2::事实代次漂移:
        out.状态 = 场景直接包含当前完整读取状态_v2::事实代次漂移;
        break;
      case L1节点当前完整引用读取状态_v2::资源失败:
        out.状态 = 场景直接包含当前完整读取状态_v2::资源失败;
        break;
      default:
        out.状态 = 场景直接包含当前完整读取状态_v2::内部不一致;
        break;
      }
      return out;
    }
    std::unordered_map<std::uint64_t, 场景直接包含种类> 成员种类;
    for (const auto &引用 : 读取.引用) {
      const auto *关系 = std::get_if<L1所有者范围关系事实>(&引用);
      if (!关系)
        continue;
      场景直接包含种类 种类{};
      if (关系->关系类型节点 == includeLayout_.直接存在成员关系类型)
        种类 = 场景直接包含种类::存在成员;
      else if (关系->关系类型节点 == includeLayout_.直接子场景关系类型)
        种类 = 场景直接包含种类::子场景;
      else
        continue;
      if (关系->目标节点 != r.成员)
        continue;
      if (关系->写入所有者 != owner_ || 关系->角色或顺序 != 1 ||
          !有效(关系->编码) || !有效(关系->源节点) ||
          关系->源节点 == r.成员 || !关系->创建事实代次 ||
          关系->创建事实代次 > out.Gread) {
        out.状态 = 场景直接包含当前完整读取状态_v2::内部不一致;
        out.包含组.clear();
        return out;
      }
      const auto [既有, 新成员] = 成员种类.emplace(关系->目标节点.值, 种类);
      if (!新成员 && 既有->second != 种类) {
        out.状态 = 场景直接包含当前完整读取状态_v2::内部不一致;
        out.包含组.clear();
        return out;
      }
      out.包含组.push_back(
          {out.Gread, 种类, 关系->源节点, 关系->目标节点, 转边(*关系)});
    }
    std::sort(out.包含组.begin(), out.包含组.end(), [](const auto &a, const auto &b) {
      return a.关系.编码.值 < b.关系.编码.值;
    });
    if (out.包含组.size() > 1) {
      out.状态 = 场景直接包含当前完整读取状态_v2::内部不一致;
      out.包含组.clear();
      return out;
    }
    out.状态 = 场景直接包含当前完整读取状态_v2::已读取;
  } catch (const std::bad_alloc &) {
    out.状态 = 场景直接包含当前完整读取状态_v2::资源失败;
  } catch (const std::length_error &) {
    out.状态 = 场景直接包含当前完整读取状态_v2::资源失败;
  } catch (const std::system_error &) {
    out.状态 = 场景直接包含当前完整读取状态_v2::资源失败;
  } catch (...) {
    out.状态 = 场景直接包含当前完整读取状态_v2::内部不一致;
  }
  if (out.状态 != 场景直接包含当前完整读取状态_v2::已读取)
    out.包含组.clear();
  return out;
}

场景直接包含当前完整读取结果_v2
场景类数据服务::读取当前场景包含子组_v2(
    const 场景直接包含子组当前完整读取请求_v2 &r) const {
  场景直接包含当前完整读取结果_v2 out;
  out.Gread = r.G0;
  if (r.版本 != 场景直接包含当前完整读取合同版本_v2 || !r.G0 ||
      !有效(r.父))
    return out;
  try {
    const L1节点当前完整引用读取请求_v2 请求{
        L1节点当前完整引用读取合同版本, r.父, r.G0};
    const auto 读取 = l1_.读取节点全部当前引用_v2(请求);
    out.Gread = 读取.读取事实代次;
    if (读取.状态 != L1节点当前完整引用读取状态_v2::成功
        || 读取.合同版本 != L1节点当前完整引用读取合同版本
        || 读取.节点 != 请求.节点
        || 读取.期望事实代次 != 请求.期望事实代次
        || 读取.读取事实代次 != 请求.期望事实代次) {
      switch (读取.状态) {
      case L1节点当前完整引用读取状态_v2::未找到:
        out.状态 = 场景直接包含当前完整读取状态_v2::未找到;
        break;
      case L1节点当前完整引用读取状态_v2::事实代次漂移:
        out.状态 = 场景直接包含当前完整读取状态_v2::事实代次漂移;
        break;
      case L1节点当前完整引用读取状态_v2::资源失败:
        out.状态 = 场景直接包含当前完整读取状态_v2::资源失败;
        break;
      default:
        out.状态 = 场景直接包含当前完整读取状态_v2::内部不一致;
        break;
      }
      return out;
    }
    std::unordered_map<std::uint64_t, 场景直接包含种类> 子成员种类;
    for (const auto &引用 : 读取.引用) {
      const auto *关系 = std::get_if<L1所有者范围关系事实>(&引用);
      if (!关系)
        continue;
      场景直接包含种类 种类{};
      if (关系->关系类型节点 == includeLayout_.直接存在成员关系类型)
        种类 = 场景直接包含种类::存在成员;
      else if (关系->关系类型节点 == includeLayout_.直接子场景关系类型)
        种类 = 场景直接包含种类::子场景;
      else
        continue;
      if (关系->源节点 != r.父)
        continue;
      if (关系->写入所有者 != owner_ || 关系->角色或顺序 != 1 ||
          !有效(关系->编码) || !有效(关系->目标节点) ||
          关系->目标节点 == r.父 || !关系->创建事实代次 ||
          关系->创建事实代次 > out.Gread) {
        out.状态 = 场景直接包含当前完整读取状态_v2::内部不一致;
        out.包含组.clear();
        return out;
      }
      const auto [既有, 新成员] = 子成员种类.emplace(关系->目标节点.值, 种类);
      if (!新成员 && 既有->second != 种类) {
        out.状态 = 场景直接包含当前完整读取状态_v2::内部不一致;
        out.包含组.clear();
        return out;
      }
      out.包含组.push_back(
          {out.Gread, 种类, 关系->源节点, 关系->目标节点, 转边(*关系)});
    }
    std::sort(out.包含组.begin(), out.包含组.end(), [](const auto &a, const auto &b) {
      return a.关系.编码.值 < b.关系.编码.值;
    });
    for (std::size_t i = 1; i < out.包含组.size(); ++i) {
      if (out.包含组[i - 1].关系.编码 == out.包含组[i].关系.编码) {
        out.状态 = 场景直接包含当前完整读取状态_v2::内部不一致;
        out.包含组.clear();
        return out;
      }
    }
    out.状态 = 场景直接包含当前完整读取状态_v2::已读取;
  } catch (const std::bad_alloc &) {
    out.状态 = 场景直接包含当前完整读取状态_v2::资源失败;
  } catch (const std::length_error &) {
    out.状态 = 场景直接包含当前完整读取状态_v2::资源失败;
  } catch (const std::system_error &) {
    out.状态 = 场景直接包含当前完整读取状态_v2::资源失败;
  } catch (...) {
    out.状态 = 场景直接包含当前完整读取状态_v2::内部不一致;
  }
  if (out.状态 != 场景直接包含当前完整读取状态_v2::已读取)
    out.包含组.clear();
  return out;
}

场景直接包含组结果 场景类数据服务::读取当前直接包含组(
    const 场景直接包含组读取请求 &r, 场景直接包含种类 kind) const {
  场景直接包含组结果 out{
      {场景直接包含状态::入口拒绝, 1, r.G0, std::nullopt}, {}};
  if (r.版本 != 1 || !r.G0 || !有效(r.父场景) || r.最大关系数量 < 1 ||
      r.最大关系数量 > 4096)
    return out;
  const auto type = kind == 场景直接包含种类::存在成员
                        ? includeLayout_.直接存在成员关系类型
                        : includeLayout_.直接子场景关系类型;
  try {
    auto q =
        查询关系(r.G0, 关系端点方向::源, r.父场景, type);
    if (!q.成功()) {
      out.结果头.状态 = 包含读取状态(q.状态);
      out.结果头.Gread = q.Gread;
      return out;
    }
    if (q.关系.size() > r.最大关系数量) {
      out.结果头.状态 = 场景直接包含状态::数量预算不足;
      return out;
    }
    for (const auto &e : q.关系) {
      if (e.写入所有者 != owner_ || e.源节点 != r.父场景 ||
          e.关系类型节点 != type || e.角色或顺序 != 1) {
        out.结果头.状态 = 场景直接包含状态::内部不一致;
        out.包含组.clear();
        return out;
      }
      out.包含组.push_back(
          {r.G0, kind, e.源节点, e.目标节点, 转边(e)});
    }
    std::sort(out.包含组.begin(), out.包含组.end(),
              [](const auto &a, const auto &b) {
                return a.关系.编码.值 < b.关系.编码.值;
              });
    out.结果头.状态 = 场景直接包含状态::已读取;
  } catch (const std::bad_alloc &) {
    out.结果头.状态 = 场景直接包含状态::资源失败;
    out.包含组.clear();
  } catch (...) {
    out.结果头.状态 = 场景直接包含状态::内部不一致;
    out.包含组.clear();
  }
  return out;
}

场景直接包含单项结果 场景类数据服务::读取当前直接包含父(
    const 场景直接包含反向读取请求 &r, 场景直接包含种类 kind) const {
  场景直接包含单项结果 out{
      {场景直接包含状态::入口拒绝, 1, r.G0, std::nullopt},
      std::nullopt};
  if (r.版本 != 1 || !r.G0 || !有效(r.成员) || r.最大关系数量 < 1 ||
      r.最大关系数量 > 4096)
    return out;
  const auto type = kind == 场景直接包含种类::存在成员
                        ? includeLayout_.直接存在成员关系类型
                        : includeLayout_.直接子场景关系类型;
  try {
    auto q =
        查询关系(r.G0, 关系端点方向::目标, r.成员, type);
    if (!q.成功()) {
      out.结果头.状态 = 包含读取状态(q.状态);
      out.结果头.Gread = q.Gread;
      return out;
    }
    if (q.关系.size() > r.最大关系数量) {
      out.结果头.状态 = 场景直接包含状态::数量预算不足;
      return out;
    }
    if (q.关系.empty()) {
      out.结果头.状态 = 场景直接包含状态::成员未归属;
      return out;
    }
    if (q.关系.size() != 1) {
      out.结果头.状态 = 场景直接包含状态::成员多重归属;
      return out;
    }
    const auto &e = q.关系.front();
    if (e.写入所有者 != owner_ || e.目标节点 != r.成员 ||
        e.关系类型节点 != type || e.角色或顺序 != 1) {
      out.结果头.状态 = 场景直接包含状态::内部不一致;
      return out;
    }
    out.包含 = {r.G0, kind, e.源节点, e.目标节点, 转边(e)};
    out.结果头.状态 = 场景直接包含状态::已读取;
  } catch (const std::bad_alloc &) {
    out.结果头.状态 = 场景直接包含状态::资源失败;
    out.包含.reset();
  } catch (...) {
    out.结果头.状态 = 场景直接包含状态::内部不一致;
    out.包含.reset();
  }
  return out;
}

场景直接包含组结果 场景类数据服务::读取当前直接存在成员组(
    const 场景直接包含组读取请求 &r) const {
  return 读取当前直接包含组(r, 场景直接包含种类::存在成员);
}
场景直接包含单项结果 场景类数据服务::读取当前直接存在成员父(
    const 场景直接包含反向读取请求 &r) const {
  return 读取当前直接包含父(r, 场景直接包含种类::存在成员);
}
场景直接包含组结果 场景类数据服务::读取当前直接子场景组(
    const 场景直接包含组读取请求 &r) const {
  return 读取当前直接包含组(r, 场景直接包含种类::子场景);
}
场景直接包含单项结果 场景类数据服务::读取当前直接子场景父(
    const 场景直接包含反向读取请求 &r) const {
  return 读取当前直接包含父(r, 场景直接包含种类::子场景);
}

场景直接包含单项结果 场景类数据服务::读取当前直接包含(
    const 场景直接包含当前读取请求 &r, 场景直接包含种类 kind) const {
  auto out = 读取当前直接包含(r);
  if (out.结果头.状态 == 场景直接包含状态::已读取 && out.包含 &&
      out.包含->种类 != kind) {
    out.结果头.状态 = 场景直接包含状态::内部不一致;
    out.包含.reset();
  }
  return out;
}
场景直接包含单项结果 场景类数据服务::读取当前直接存在成员(
    const 场景直接包含当前读取请求 &r) const {
  return 读取当前直接包含(r, 场景直接包含种类::存在成员);
}
场景直接包含单项结果 场景类数据服务::读取当前直接子场景(
    const 场景直接包含当前读取请求 &r) const {
  return 读取当前直接包含(r, 场景直接包含种类::子场景);
}

直接归属场景角色读取结果
场景类数据服务::读取当前场景角色位置(const 直接归属场景角色读取请求 &r) const {
  直接归属场景角色读取结果 out;
  out.Gread = r.G0;
  if (r.版本 != 1 || !r.G0 || !有效(r.对象))
    return out;
  try {
    const auto ew = existence_.确认当前存在结构身份(r.G0, r.对象);
    if (!ew.成功(r.G0)) {
      out.Gread = ew.Gread;
      out.状态 = ew.状态 == 存在结构身份只读状态::已读取
                       ? 直接归属联合只读状态::内部不一致
                       : 联合读取状态(ew.状态);
      return out;
    }
    auto role = 读角色(r.G0, r.对象);
    if (role.状态 == 场景角色数据状态::场景角色未启用) {
      out.状态 = 直接归属联合只读状态::已读取;
      return out;
    }
    if (!role.成功({2, r.G0, r.对象})) {
      out.Gread = role.Gread;
      out.状态 = 联合读取状态(role.状态);
      return out;
    }
    auto root = 查询关系(r.G0, 关系端点方向::源, r.对象,
                         includeLayout_.根标记关系类型);
    auto parent = 查询关系(r.G0, 关系端点方向::源, r.对象,
                           includeLayout_.树归属关系类型);
    if (!root.成功() || !parent.成功()) {
      if (!root.成功()) {
        out.Gread = root.Gread;
        out.状态 = 联合读取状态(root.状态);
      } else {
        out.Gread = parent.Gread;
        out.状态 = 联合读取状态(parent.状态);
      }
      return out;
    }
    if (root.关系.size() > 1 || parent.关系.size() > 1 ||
        (!root.关系.empty() && !parent.关系.empty())) {
      out.状态 = 直接归属联合只读状态::内部不一致;
      return out;
    }
    直接归属场景角色事实 fact{r.G0,         r.对象,
                              直接归属场景位置::未纳入场景树,
                              std::nullopt, std::nullopt};
    if (!root.关系.empty()) {
      const auto &e = root.关系.front();
      if (e.目标节点 != includeLayout_.树登记锚点 || e.写入所有者 != owner_ ||
          e.角色或顺序 != 1) {
        out.状态 = 直接归属联合只读状态::内部不一致;
        return out;
      }
      fact.位置 = 直接归属场景位置::场景树根;
      fact.树根 = r.对象;
      fact.树证明关系 = e.编码;
    } else if (!parent.关系.empty()) {
      const auto &e = parent.关系.front();
      if (!有效(e.目标节点) || e.写入所有者 != owner_ || e.角色或顺序 != 1) {
        out.状态 = 直接归属联合只读状态::内部不一致;
        return out;
      }
      fact.位置 = 直接归属场景位置::场景树非根;
      fact.树根 = e.目标节点;
      fact.树证明关系 = e.编码;
    }
    out.角色 = fact;
    out.状态 = 直接归属联合只读状态::已读取;
  } catch (const std::bad_alloc &) {
    out.状态 = 直接归属联合只读状态::资源失败;
  } catch (...) {
    out.状态 = 直接归属联合只读状态::内部不一致;
  }
  return out;
}

直接归属场景角色当前完整读取结果_v2
场景类数据服务::读取当前场景角色位置_v2(
    const 直接归属场景角色当前完整读取请求_v2 &r) const {
  直接归属场景角色当前完整读取结果_v2 out;
  out.Gread = r.G0;
  if (r.版本 != 直接归属场景角色当前完整读取合同版本_v2 || !r.G0 ||
      !有效(r.对象))
    return out;
  try {
    const auto existence = existence_.确认当前存在结构身份(r.G0, r.对象);
    out.Gread = existence.Gread;
    if (!existence.成功(r.G0)) {
      using E = 存在结构身份只读状态;
      switch (existence.状态) {
      case E::未找到:
        out.状态 = 直接归属场景角色当前完整读取状态_v2::未找到;
        break;
      case E::事实代次漂移:
        out.状态 = 直接归属场景角色当前完整读取状态_v2::事实代次漂移;
        break;
      case E::资源失败:
        out.状态 = 直接归属场景角色当前完整读取状态_v2::资源失败;
        break;
      default:
        out.状态 = 直接归属场景角色当前完整读取状态_v2::内部不一致;
        break;
      }
      return out;
    }

    const auto role = 读当前完整角色_v2(r.G0, r.对象);
    const bool noRole = role.状态 == 场景角色数据状态::场景角色未启用;
    if (!noRole && !role.成功({2, r.G0, r.对象})) {
      out.Gread = role.Gread;
      out.状态 = role.状态 == 场景角色数据状态::事实代次漂移
                       ? 直接归属场景角色当前完整读取状态_v2::事实代次漂移
                   : role.状态 == 场景角色数据状态::资源失败
                       ? 直接归属场景角色当前完整读取状态_v2::资源失败
                       : 直接归属场景角色当前完整读取状态_v2::内部不一致;
      return out;
    }

    const L1节点当前完整引用读取请求_v2 request{
        L1节点当前完整引用读取合同版本, r.对象, r.G0};
    const auto refs = l1_.读取节点全部当前引用_v2(request);
    out.Gread = refs.读取事实代次;
    if (!L1当前引用读取成功(refs, request)) {
      out.状态 = refs.状态 == L1节点当前完整引用读取状态_v2::事实代次漂移
                       ? 直接归属场景角色当前完整读取状态_v2::事实代次漂移
                   : refs.状态 == L1节点当前完整引用读取状态_v2::资源失败
                       ? 直接归属场景角色当前完整读取状态_v2::资源失败
                       : 直接归属场景角色当前完整读取状态_v2::内部不一致;
      return out;
    }
    std::vector<L1所有者范围关系事实> roots, memberships, contexts;
    for (const auto &ref : refs.引用) {
      const auto *edge = std::get_if<L1所有者范围关系事实>(&ref);
      if (!edge || edge->源节点 != r.对象)
        continue;
      std::vector<L1所有者范围关系事实> *selected = nullptr;
      if (edge->关系类型节点 == includeLayout_.根标记关系类型)
        selected = &roots;
      else if (edge->关系类型节点 == includeLayout_.树归属关系类型)
        selected = &memberships;
      else if (edge->关系类型节点 == layout_.父场景语境关系类型)
        selected = &contexts;
      else
        continue;
      if (edge->写入所有者 != owner_ || !有效(edge->编码) ||
          !有效(edge->目标节点) || edge->角色或顺序 != 1 ||
          !edge->创建事实代次 || edge->创建事实代次 > r.G0) {
        out.状态 = 直接归属场景角色当前完整读取状态_v2::内部不一致;
        return out;
      }
      selected->push_back(*edge);
    }
    if (roots.size() > 1 || memberships.size() > 1 || contexts.size() > 1 ||
        (!roots.empty() && !memberships.empty())) {
      out.状态 = 直接归属场景角色当前完整读取状态_v2::内部不一致;
      return out;
    }
    if (noRole) {
      if (!roots.empty() || !memberships.empty() || !contexts.empty()) {
        out.状态 = 直接归属场景角色当前完整读取状态_v2::内部不一致;
        return out;
      }
      out.状态 = 直接归属场景角色当前完整读取状态_v2::已读取;
      return out;
    }

    直接归属场景角色事实 fact{r.G0, r.对象,
                                  直接归属场景位置::未纳入场景树,
                                  std::nullopt, std::nullopt};
    if (!roots.empty()) {
      if (roots.front().目标节点 != includeLayout_.树登记锚点 ||
          !contexts.empty()) {
        out.状态 = 直接归属场景角色当前完整读取状态_v2::内部不一致;
        return out;
      }
      fact.位置 = 直接归属场景位置::场景树根;
      fact.树根 = r.对象;
      fact.树证明关系 = roots.front().编码;
    } else if (!memberships.empty()) {
      if (memberships.front().目标节点 == r.对象 || contexts.size() != 1) {
        out.状态 = 直接归属场景角色当前完整读取状态_v2::内部不一致;
        return out;
      }
      fact.位置 = 直接归属场景位置::场景树非根;
      fact.树根 = memberships.front().目标节点;
      fact.树证明关系 = memberships.front().编码;
    } else if (!contexts.empty()) {
      out.状态 = 直接归属场景角色当前完整读取状态_v2::内部不一致;
      return out;
    }
    out.角色 = fact;
    out.状态 = 直接归属场景角色当前完整读取状态_v2::已读取;
    if (!out.成功(r)) {
      out.角色.reset();
      out.状态 = 直接归属场景角色当前完整读取状态_v2::内部不一致;
    }
  } catch (const std::bad_alloc &) {
    out.状态 = 直接归属场景角色当前完整读取状态_v2::资源失败;
  } catch (const std::length_error &) {
    out.状态 = 直接归属场景角色当前完整读取状态_v2::资源失败;
  } catch (const std::system_error &) {
    out.状态 = 直接归属场景角色当前完整读取状态_v2::资源失败;
  } catch (...) {
    out.状态 = 直接归属场景角色当前完整读取状态_v2::内部不一致;
  }
  if (out.状态 != 直接归属场景角色当前完整读取状态_v2::已读取)
    out.角色.reset();
  return out;
}

场景直接包含状态
场景类数据服务::校验联合闭包(std::uint64_t g, 稳定编码 start,
                              std::uint64_t relationBudget,
                              std::uint64_t sceneBudget,
                              const 直接归属联合只读提供者 &joint,
                              std::optional<稳定编码> requiredRoot) const {
  std::vector<稳定编码> pending{start};
  std::unordered_set<std::uint64_t> seen;
  std::unordered_set<std::uint64_t> relations;
  std::uint64_t sceneCount = 0;
  for (std::size_t i = 0; i < pending.size(); ++i) {
    const auto e = pending[i];
    if (!seen.insert(e.值).second)
      return 场景直接包含状态::形成场景环;
    const 直接归属场景角色读取请求 rr{1, g, e};
    const auto role = joint.读取当前场景角色位置(rr);
    if (!role.成功(rr))
      return 联合到包含(role.状态);
    if (role.角色) {
      ++sceneCount;
      if (sceneCount > sceneBudget)
        return 场景直接包含状态::数量预算不足;
      if (!role.角色->树证明关系 ||
          !relations.insert(role.角色->树证明关系->值).second)
        return 场景直接包含状态::内部不一致;
      if (relations.size() > relationBudget)
        return 场景直接包含状态::数量预算不足;
      if (!requiredRoot || !role.角色->树根 ||
          *role.角色->树根 != *requiredRoot)
        return 场景直接包含状态::引用冲突;
    }
    const 直接归属联合子组读取请求 cr{1, g, e, relationBudget};
    const auto children = joint.读取当前联合子组(cr);
    if (!children.子组读取成功(cr))
      return 联合到包含(children.状态);
    for (const auto &x : children.子组) {
      if (!relations.insert(x.关系.值).second)
        return 场景直接包含状态::内部不一致;
      if (relations.size() > relationBudget)
        return 场景直接包含状态::数量预算不足;
      pending.push_back(x.成员);
    }
  }
  return 场景直接包含状态::已读取;
}

场景直接包含单项结果
场景类数据服务::变更直接存在成员核心(const 场景直接包含写请求 &r,
                                     const 直接归属联合只读提供者 &joint,
                                     bool adding) {
  场景直接包含单项结果 out{
      {场景直接包含状态::入口拒绝, 1, r.G0, std::nullopt}, std::nullopt};
  if (r.版本 != 1 || !r.G0 || r.G0 == UINT64_MAX ||
      !普通幂等身份有效(r.幂等身份) || !有效(r.父场景) || !有效(r.成员) ||
      r.最大关系数量 < 1 || r.最大关系数量 > 4096 || r.最大场景数量 < 1 ||
      r.最大场景数量 > 4096 || !joint.绑定于(l1_))
    return out;
  bool started = false;
  try {
    L1所有者范围写集请求 expected{L1所有者范围CRUD合同版本, r.G0,
                                    r.幂等身份};
    if (adding)
      expected.关系 = {
          {{1}, r.父场景, r.成员, includeLayout_.直接存在成员关系类型, 1}};
    const auto first =
        port_.读取首次写入材料({L1所有者范围首次写入读取合同版本, r.幂等身份});
    if (first.状态 == L1所有者范围读取状态::成功) {
      if (!first.首次规范化写集 || !first.首次写入结果 ||
          !写入头完整(*first.首次写入结果, r.幂等身份, r.G0)) {
        out.结果头.状态 = 场景直接包含状态::幂等冲突;
        return out;
      }
      const auto &firstSet = *first.首次规范化写集;
      稳定编码 relation{};
      if (adding) {
        if (!写集相同(firstSet, expected)) {
          out.结果头.状态 = 场景直接包含状态::幂等冲突;
          return out;
        }
        const auto mapped = 映射编码(*first.首次写入结果, {1});
        if (!mapped) {
          out.结果头.状态 = 场景直接包含状态::内部不一致;
          return out;
        }
        relation = *mapped;
      } else {
        if (firstSet.合同版本 != L1所有者范围CRUD合同版本 ||
            firstSet.期望事实代次 != r.G0 ||
            firstSet.写入幂等身份 != r.幂等身份 || !firstSet.节点.empty() ||
            !firstSet.关系.empty() || !firstSet.值.empty() ||
            !firstSet.属性槽变更.empty() || firstSet.退出事实.size() != 1) {
          out.结果头.状态 = 场景直接包含状态::幂等冲突;
          return out;
        }
        relation = firstSet.退出事实.front();
      }
      std::optional<场景直接包含事实> replayFact;
      场景直接包含状态 replayFailure = 场景直接包含状态::内部不一致;
      if (adding) {
        const auto historical = 读取当前直接包含(
            {1, first.读取事实代次, relation});
        if (historical.成功())
          replayFact = historical.包含;
        else
          replayFailure = historical.结果头.状态;
      } else {
        const auto current = 读关系(relation, first.读取事实代次);
        if (current.状态 != 场景角色数据状态::未找到) {
          out.结果头.状态 = current.成功()
                                   ? 场景直接包含状态::内部不一致
                                   : 包含读取状态(current.状态);
          out.结果头.Gread = first.读取事实代次;
          return out;
        }
        out.结果头 = {场景直接包含状态::精确重复, 1,
                      first.读取事实代次,
                      first.首次写入结果->事实代次};
        return out;
      }
      if (!replayFact || replayFact->种类 != 场景直接包含种类::存在成员 ||
          replayFact->父场景 != r.父场景 || replayFact->成员 != r.成员) {
        out.结果头.状态 = replayFact ? 场景直接包含状态::内部不一致
                                     : replayFailure;
        out.结果头.Gread = first.读取事实代次;
        return out;
      }
      out.结果头 = {场景直接包含状态::精确重复, 1,
                    first.读取事实代次,
                    first.首次写入结果->事实代次};
      out.包含 = replayFact;
      return out;
    }
    if (first.状态 != L1所有者范围读取状态::未找到 ||
        first.读取事实代次 != r.G0) {
      if (first.读取事实代次 != r.G0)
        out.结果头.状态 = 场景直接包含状态::事实代次漂移;
      else if (first.状态 == L1所有者范围读取状态::资源失败)
        out.结果头.状态 = 场景直接包含状态::资源失败;
      else
        out.结果头.状态 = 场景直接包含状态::内部不一致;
      out.结果头.Gread = first.读取事实代次;
      return out;
    }
    const auto c = 读取当前场景角色位置({1, r.G0, r.父场景});
    if (!c.成功({1, r.G0, r.父场景}) || !c.角色 ||
        c.角色->位置 == 直接归属场景位置::未纳入场景树) {
      out.结果头.状态 = 场景直接包含状态::场景未找到;
      return out;
    }
    const auto e = existence_.确认当前存在结构身份(r.G0, r.成员);
    if (e.状态 != 存在结构身份只读状态::已读取) {
      out.结果头.状态 = 存在到包含(e.状态);
      out.结果头.Gread = e.Gread;
      return out;
    }
    const 直接归属联合父读取请求 pr{1, r.G0, r.成员, r.最大关系数量};
    const auto parent = joint.读取当前联合父(pr);
    if (!parent.父读取成功(pr)) {
      out.结果头.状态 = 联合到包含(parent.状态);
      out.结果头.Gread = parent.Gread;
      return out;
    }
    if (adding && parent.父) {
      out.结果头.状态 = 场景直接包含状态::成员已归属;
      return out;
    }
    if (!adding && (!parent.父 || parent.父->来源 != 直接归属来源::场景成员 ||
                    parent.父->父 != r.父场景)) {
      out.结果头.状态 = parent.父 ? 场景直接包含状态::原父不匹配
                                  : 场景直接包含状态::成员未归属;
      return out;
    }
    const auto closure = 校验联合闭包(r.G0, r.成员, r.最大关系数量,
                                      r.最大场景数量, joint, std::nullopt);
    if (closure != 场景直接包含状态::已读取) {
      out.结果头.状态 = closure;
      return out;
    }
    auto ws = expected;
    if (!adding)
      ws.退出事实 = {parent.父->关系};
    started = true;
    const auto saved = 串行提交(ws);
    out.结果头.Gread = saved.事实代次;
    if (saved.状态 != L1所有者范围写入状态::成功 &&
        saved.状态 != L1所有者范围写入状态::精确重复) {
      if (saved.状态 == L1所有者范围写入状态::事实代次漂移)
        out.结果头.状态 = 场景直接包含状态::事实代次漂移;
      else if (saved.状态 == L1所有者范围写入状态::幂等冲突)
        out.结果头.状态 = 场景直接包含状态::幂等冲突;
      else if (saved.是否形成内存权威发布)
        out.结果头.状态 = 场景直接包含状态::已可能发布;
      else if (saved.状态 == L1所有者范围写入状态::资源失败)
        out.结果头.状态 = 场景直接包含状态::资源失败;
      else
        out.结果头.状态 = 场景直接包含状态::内部不一致;
      if (saved.是否形成内存权威发布)
        out.结果头.首次发布H = saved.事实代次;
      return out;
    }
    out.结果头.首次发布H = saved.事实代次;
    if (adding) {
      auto now =
          读取当前场景包含父组({1, saved.事实代次, r.成员, r.最大关系数量});
      for (const auto &x : now.包含组)
        if (x.父场景 == r.父场景 && x.种类 == 场景直接包含种类::存在成员) {
          out.包含 = x;
          break;
        }
    } else {
      const auto current = 读关系(parent.父->关系, saved.事实代次);
      if (current.状态 != 场景角色数据状态::未找到) {
        out.结果头.状态 = 场景直接包含状态::已可能发布;
        return out;
      }
    }
    if (adding && !out.包含) {
      out.结果头.状态 = 场景直接包含状态::已可能发布;
      return out;
    }
    if (saved.状态 == L1所有者范围写入状态::精确重复)
      out.结果头.状态 = 场景直接包含状态::精确重复;
    else
      out.结果头.状态 =
          adding ? 场景直接包含状态::已新增 : 场景直接包含状态::已删除;
  } catch (const std::bad_alloc &) {
    out.结果头.状态 =
        started ? 场景直接包含状态::已可能发布 : 场景直接包含状态::资源失败;
  } catch (...) {
    out.结果头.状态 =
        started ? 场景直接包含状态::已可能发布 : 场景直接包含状态::内部不一致;
  }
  return out;
}

场景直接包含单项结果
场景类数据服务::新增直接存在成员(const 场景直接包含写请求 &r,
                                 const 直接归属联合只读提供者 &j) {
  return 变更直接存在成员核心(r, j, true);
}
场景直接包含单项结果
场景类数据服务::退出直接存在成员(const 场景直接包含写请求 &r,
                                 const 直接归属联合只读提供者 &j) {
  return 变更直接存在成员核心(r, j, false);
}

场景直接包含迁移结果
场景类数据服务::迁移直接包含核心(const 场景直接包含迁移请求 &r,
                                 const 直接归属联合只读提供者 &joint,
                                 场景直接包含种类 kind) {
  场景直接包含迁移结果 out{
      {场景直接包含状态::入口拒绝, 1, r.G0, std::nullopt},
      std::nullopt,
      std::nullopt};
  if (r.版本 != 1 || !r.G0 || r.G0 == UINT64_MAX ||
      !普通幂等身份有效(r.幂等身份) || !有效(r.原父场景) ||
      !有效(r.目标父场景) || r.原父场景 == r.目标父场景 || !有效(r.成员) ||
      r.最大关系数量 < 1 || r.最大关系数量 > 4096 || r.最大场景数量 < 1 ||
      r.最大场景数量 > 4096 || !joint.绑定于(l1_)) {
    if (r.原父场景 == r.目标父场景 && 有效(r.原父场景))
      out.结果头.状态 = 场景直接包含状态::目标父相同;
    return out;
  }
  bool started = false;
  try {
    const auto type = kind == 场景直接包含种类::存在成员
                          ? includeLayout_.直接存在成员关系类型
                          : includeLayout_.直接子场景关系类型;
    const auto first =
        port_.读取首次写入材料({L1所有者范围首次写入读取合同版本, r.幂等身份});
    if (first.状态 == L1所有者范围读取状态::成功) {
      if (!first.首次规范化写集 || !first.首次写入结果 ||
          !写入头完整(*first.首次写入结果, r.幂等身份, r.G0)) {
        out.结果头.状态 = 场景直接包含状态::幂等冲突;
        return out;
      }
      const auto &ws = *first.首次规范化写集;
      const bool sceneKind = kind == 场景直接包含种类::子场景;
      const bool shape =
          ws.合同版本 == L1所有者范围CRUD合同版本 &&
          ws.期望事实代次 == r.G0 && ws.写入幂等身份 == r.幂等身份 &&
          ws.节点.empty() && ws.值.empty() && ws.属性槽变更.empty() &&
          ws.关系.size() == (sceneKind ? 2 : 1) &&
          ws.退出事实.size() == (sceneKind ? 2 : 1) &&
          std::is_sorted(ws.退出事实.begin(), ws.退出事实.end()) &&
          ws.关系.front() == L1所有者范围关系新建项{
                                     {1}, r.目标父场景, r.成员, type, 1} &&
          (!sceneKind ||
           ws.关系[1] == L1所有者范围关系新建项{
                                  {2}, r.成员, r.目标父场景,
                                  layout_.父场景语境关系类型, 1});
      const auto newId = 映射编码(*first.首次写入结果, {1});
      const auto newEdge = newId ? 读关系(*newId, first.读取事实代次)
                                 : 关系事实读取{};
      if (!shape) {
        out.结果头.状态 = 场景直接包含状态::幂等冲突;
        return out;
      }
      if (!newId || !newEdge.成功() ||
          newEdge.事实->源节点 != r.目标父场景 ||
          newEdge.事实->目标节点 != r.成员 ||
          newEdge.事实->关系类型节点 != type) {
        out.结果头.状态 = 场景直接包含状态::内部不一致;
        out.结果头.Gread = first.读取事实代次;
        return out;
      }
      out.结果头 = {场景直接包含状态::精确重复, 1,
                    first.读取事实代次,
                    first.首次写入结果->事实代次};
      out.已建立新包含 = 场景直接包含事实{
          first.读取事实代次, kind, r.目标父场景, r.成员,
          转边(*newEdge.事实)};
      if (sceneKind) {
        const auto newContextId = 映射编码(*first.首次写入结果, {2});
        const 场景父语境读取请求 contextRequest{
            1, first.读取事实代次, r.成员, r.最大关系数量};
        const auto currentContext = 读取当前父场景语境(contextRequest);
        if (!newContextId || !currentContext.成功(contextRequest) ||
            !currentContext.投影 ||
            currentContext.投影->投影边.编码 != *newContextId ||
            currentContext.投影->父场景语境 != r.目标父场景) {
          out.已建立新包含.reset();
          out.结果头.状态 = 场景直接包含状态::已可能发布;
          return out;
        }
        out.已建立新父语境 = currentContext.投影;
      }
      return out;
    }
    if (first.状态 != L1所有者范围读取状态::未找到 ||
        first.读取事实代次 != r.G0) {
      out.结果头.Gread = first.读取事实代次;
      out.结果头.状态 = first.读取事实代次 != r.G0
                              ? 场景直接包含状态::事实代次漂移
                          : first.状态 == L1所有者范围读取状态::资源失败
                              ? 场景直接包含状态::资源失败
                              : 场景直接包含状态::内部不一致;
      return out;
    }
    const auto oldRole = 读取当前场景角色位置({1, r.G0, r.原父场景});
    const auto newRole = 读取当前场景角色位置({1, r.G0, r.目标父场景});
    if (!oldRole.成功({1, r.G0, r.原父场景}) ||
        !newRole.成功({1, r.G0, r.目标父场景}) || !oldRole.角色 ||
        !newRole.角色 || !oldRole.角色->树根 || !newRole.角色->树根) {
      out.结果头.状态 = 场景直接包含状态::场景未找到;
      return out;
    }
    if (*oldRole.角色->树根 != *newRole.角色->树根) {
      out.结果头.状态 = 场景直接包含状态::引用冲突;
      return out;
    }
    const 直接归属联合父读取请求 pr{1, r.G0, r.成员, r.最大关系数量};
    const auto parent = joint.读取当前联合父(pr);
    if (!parent.父读取成功(pr)) {
      out.结果头.状态 = 联合到包含(parent.状态);
      return out;
    }
    const auto expectedSource = kind == 场景直接包含种类::存在成员
                                    ? 直接归属来源::场景成员
                                    : 直接归属来源::直接子场景;
    if (!parent.父 || parent.父->来源 != expectedSource ||
        parent.父->父 != r.原父场景) {
      out.结果头.状态 = parent.父 ? 场景直接包含状态::原父不匹配
                                  : 场景直接包含状态::成员未归属;
      return out;
    }
    const auto closure = 校验联合闭包(r.G0, r.成员, r.最大关系数量,
                                      r.最大场景数量, joint,
                                      *oldRole.角色->树根);
    if (closure != 场景直接包含状态::已读取) {
      out.结果头.状态 = closure;
      return out;
    }
    std::optional<场景父语境投影事实> oldContext;
    if (kind == 场景直接包含种类::子场景) {
      const 场景父语境读取请求 contextRequest{
          1, r.G0, r.成员, r.最大关系数量};
      const auto context = 读取当前父场景语境(contextRequest);
      if (!context.成功(contextRequest) || !context.投影 ||
          context.投影->父场景语境 != r.原父场景 ||
          context.投影->结构父.关系 != parent.父->关系) {
        out.结果头.状态 = context.状态 == 场景角色数据状态::数量预算不足
                                  ? 场景直接包含状态::数量预算不足
                                  : 场景直接包含状态::引用冲突;
        return out;
      }
      oldContext = context.投影;
    }
    L1所有者范围写集请求 ws{L1所有者范围CRUD合同版本, r.G0, r.幂等身份};
    ws.关系 = {{{1}, r.目标父场景, r.成员, type, 1}};
    ws.退出事实 = {parent.父->关系};
    if (oldContext) {
      ws.关系.push_back({{2}, r.成员, r.目标父场景,
                         layout_.父场景语境关系类型, 1});
      ws.退出事实.push_back(oldContext->投影边.编码);
      std::sort(ws.退出事实.begin(), ws.退出事实.end());
    }
    started = true;
    const auto saved = 串行提交(ws);
    out.结果头.Gread = saved.事实代次;
    if (saved.状态 != L1所有者范围写入状态::成功 &&
        saved.状态 != L1所有者范围写入状态::精确重复) {
      if (saved.状态 == L1所有者范围写入状态::事实代次漂移)
        out.结果头.状态 = 场景直接包含状态::事实代次漂移;
      else if (saved.状态 == L1所有者范围写入状态::幂等冲突)
        out.结果头.状态 = 场景直接包含状态::幂等冲突;
      else if (saved.是否形成内存权威发布)
        out.结果头.状态 = 场景直接包含状态::已可能发布;
      else if (saved.状态 == L1所有者范围写入状态::资源失败)
        out.结果头.状态 = 场景直接包含状态::资源失败;
      else
        out.结果头.状态 = 场景直接包含状态::内部不一致;
      if (saved.是否形成内存权威发布)
        out.结果头.首次发布H = saved.事实代次;
      return out;
    }
    out.结果头.首次发布H = saved.事实代次;
    const auto newid = 映射编码(saved, {1});
    if (!newid) {
      out.结果头.状态 = 场景直接包含状态::已可能发布;
      return out;
    }
    const auto ne = 读关系(*newid, saved.事实代次);
    if (!ne.成功()) {
      out.结果头.状态 = 场景直接包含状态::已可能发布;
      return out;
    }
    out.已建立新包含 =
        场景直接包含事实{saved.事实代次, kind,
                         r.目标父场景,   r.成员,         转边(*ne.事实)};
    if (kind == 场景直接包含种类::子场景) {
      const auto newContextId = 映射编码(saved, {2});
      const auto newContextEdge =
          newContextId ? 读关系(*newContextId, saved.事实代次)
                       : 关系事实读取{};
      const 直接归属联合父读取请求 currentParentRequest{
          1, saved.事实代次, r.成员, r.最大关系数量};
      const auto currentParent = joint.读取当前联合父(currentParentRequest);
      const 场景父语境读取请求 currentContextRequest{
          1, saved.事实代次, r.成员, r.最大关系数量};
      const auto currentContext = 读取当前父场景语境(currentContextRequest);
      const 场景树当前读取请求 treeRequest{
          1, saved.事实代次, *oldRole.角色->树根, r.最大场景数量,
          r.最大关系数量};
      const auto tree = 读取当前场景树(treeRequest, joint);
      if (!newContextId || !newContextEdge.成功() ||
          newContextEdge.事实->源节点 != r.成员 ||
          newContextEdge.事实->目标节点 != r.目标父场景 ||
          newContextEdge.事实->关系类型节点 != layout_.父场景语境关系类型 ||
          newContextEdge.事实->角色或顺序 != 1 ||
          !currentParent.父读取成功(currentParentRequest) ||
          !currentParent.父 || currentParent.父->关系 != *newid ||
          !currentContext.成功(currentContextRequest) || !currentContext.投影 ||
          currentContext.投影->投影边.编码 != *newContextId ||
          currentContext.投影->结构父.关系 != *newid ||
          !tree.成功(treeRequest)) {
        out.已建立新包含.reset();
        out.结果头.状态 = 场景直接包含状态::已可能发布;
        return out;
      }
      out.已建立新父语境 = currentContext.投影;
    }
    out.结果头.状态 = saved.状态 == L1所有者范围写入状态::成功
                          ? 场景直接包含状态::已迁移
                          : 场景直接包含状态::精确重复;
  } catch (const std::bad_alloc &) {
    out.结果头.状态 =
        started ? 场景直接包含状态::已可能发布 : 场景直接包含状态::资源失败;
  } catch (...) {
    out.结果头.状态 =
        started ? 场景直接包含状态::已可能发布 : 场景直接包含状态::内部不一致;
  }
  return out;
}

场景直接包含迁移结果
场景类数据服务::迁移直接存在成员(const 场景直接包含迁移请求 &r,
                                 const 直接归属联合只读提供者 &j) {
  return 迁移直接包含核心(r, j, 场景直接包含种类::存在成员);
}
场景直接包含迁移结果
场景类数据服务::迁移直接子场景(const 场景直接包含迁移请求 &r,
                               const 直接归属联合只读提供者 &j) {
  return 迁移直接包含核心(r, j, 场景直接包含种类::子场景);
}

std::optional<场景树角色写结果> 场景类数据服务::尝试重放树角色(
    std::uint64_t g, L1所有者范围写入幂等身份 key, 稳定编码 object,
    std::optional<稳定编码> parentScene,
    std::optional<稳定编码> requestedExistingParent, bool rootMarker,
    std::uint64_t relationBudget, std::uint64_t ancestorBudget,
    const 直接归属联合只读提供者 &joint) const {
  const auto first = port_.读取首次写入材料(
      {L1所有者范围首次写入读取合同版本, key});
  场景树角色写结果 out{{场景直接包含状态::内部不一致, 1,
                           first.读取事实代次, std::nullopt},
                          std::nullopt, std::nullopt, std::nullopt,
                          std::nullopt, std::nullopt};
  if (first.状态 == L1所有者范围读取状态::未找到) {
    if (first.合同版本 != L1所有者范围首次写入读取合同版本 ||
        first.所有者 != owner_ || first.写入幂等身份 != key ||
        first.首次规范化写集 || first.首次写入结果)
      return out;
    if (first.读取事实代次 != g) {
      out.结果头.状态 = 场景直接包含状态::事实代次漂移;
      return out;
    }
    return std::nullopt;
  }
  if (first.状态 == L1所有者范围读取状态::资源失败) {
    out.结果头.状态 = 场景直接包含状态::资源失败;
    return out;
  }
  if (first.状态 != L1所有者范围读取状态::成功 ||
      first.合同版本 != L1所有者范围首次写入读取合同版本 ||
      first.所有者 != owner_ || first.写入幂等身份 != key ||
      !first.首次规范化写集 || !first.首次写入结果) {
    return out;
  }

  const auto &firstSet = *first.首次规范化写集;
  const auto &firstSaved = *first.首次写入结果;
  if (!写入头完整(firstSaved, key, g) ||
      firstSaved.状态 != L1所有者范围写入状态::成功 ||
      !写入映射完整(firstSaved, firstSet)) {
    out.结果头.状态 = firstSaved.是否形成内存权威发布
                            ? 场景直接包含状态::已可能发布
                            : 场景直接包含状态::内部不一致;
    if (firstSaved.是否形成内存权威发布)
      out.结果头.首次发布H = firstSaved.事实代次;
    return out;
  }
  const auto H = firstSaved.事实代次;

  std::optional<稳定编码> recordedProofTarget;
  for (const auto &edge : firstSet.关系) {
    if (edge.本地键 == L1所有者范围写集本地键{10}) {
      const auto *target = std::get_if<稳定编码>(&edge.目标节点);
      if (!target || recordedProofTarget) {
        out.结果头.状态 = 场景直接包含状态::幂等冲突;
        return out;
      }
      recordedProofTarget = *target;
    }
  }
  if (!recordedProofTarget || !有效(*recordedProofTarget) ||
      (rootMarker && *recordedProofTarget != includeLayout_.树登记锚点)) {
    out.结果头.状态 = 场景直接包含状态::幂等冲突;
    return out;
  }
  const auto recordedRoot = rootMarker ? object : *recordedProofTarget;
  auto expected = 形成启用写集(g, key, object);
  expected.关系.push_back({{10}, object,
                           rootMarker ? includeLayout_.树登记锚点 : recordedRoot,
                           rootMarker ? includeLayout_.根标记关系类型
                                      : includeLayout_.树归属关系类型,
                           1});
  if (parentScene)
    expected.关系.push_back(
        {{11}, *parentScene, object, includeLayout_.直接子场景关系类型, 1});
  if (parentScene)
    expected.关系.push_back(
        {{12}, object, *parentScene, layout_.父场景语境关系类型, 1});
  else if (requestedExistingParent)
    expected.关系.push_back({{11}, object, *requestedExistingParent,
                             layout_.父场景语境关系类型, 1});
  if (!写集相同(firstSet, expected)) {
    out.结果头.状态 = 场景直接包含状态::幂等冲突;
    return out;
  }

  const auto guard = 读取当前代次();
  if (!guard.成功() || guard.Gread < H) {
    out.结果头.状态 = 场景直接包含状态::已可能发布;
    out.结果头.Gread = guard.Gread;
    out.结果头.首次发布H = H;
    return out;
  }
  out.结果头.Gread = guard.Gread;
  const auto role = 读角色(guard.Gread, object);
  const auto proofId = 映射编码(firstSaved, {10});
  if (!role.成功({2, guard.Gread, object}) || !proofId) {
    out.结果头.状态 = 场景直接包含状态::已可能发布;
    out.结果头.首次发布H = H;
    return out;
  }
  std::unordered_set<std::uint64_t> verifiedRelations;
  std::unordered_set<std::uint64_t> visitedAncestors;
  auto failBudget = [&]() {
    out.结果头.状态 = 场景直接包含状态::数量预算不足;
    out.结果头.首次发布H.reset();
    out.场景角色.reset();
    out.树证明.reset();
    out.最近上游场景.reset();
    out.既有直接父.reset();
    out.新直接包含.reset();
    return out;
  };
  auto rememberRelation = [&](稳定编码 relation) {
    return 有效(relation) && verifiedRelations.insert(relation.值).second;
  };
  if (!rememberRelation(role.角色->场景角色登记边.编码)) {
    out.结果头.状态 = 场景直接包含状态::已可能发布;
    out.结果头.首次发布H = H;
    return out;
  }
  for (const auto &root : role.角色->四根) {
    if (!rememberRelation(root.绑定.编码)) {
      out.结果头.状态 = 场景直接包含状态::已可能发布;
      out.结果头.首次发布H = H;
      return out;
    }
  }
  if (!rememberRelation(*proofId)) {
    out.结果头.状态 = 场景直接包含状态::已可能发布;
    out.结果头.首次发布H = H;
    return out;
  }
  if (verifiedRelations.size() > relationBudget)
    return failBudget();
  const auto proof = 读关系(*proofId, guard.Gread);
  if (!proof.成功() || proof.事实->源节点 != object ||
      proof.事实->目标节点 !=
          (rootMarker ? includeLayout_.树登记锚点 : recordedRoot) ||
      proof.事实->关系类型节点 !=
          (rootMarker ? includeLayout_.根标记关系类型
                      : includeLayout_.树归属关系类型) ||
      proof.事实->角色或顺序 != 1 || proof.事实->创建事实代次 != H) {
    out.结果头.状态 = 场景直接包含状态::已可能发布;
    out.结果头.首次发布H = H;
    return out;
  }
  auto proofWitness = 转边(*proof.事实);
  out.场景角色 = role.角色;
  out.树证明 = 场景树证明事实{guard.Gread,
                              rootMarker ? 场景树证明种类::根标记
                                         : 场景树证明种类::树归属,
                              *proofId,
                              object,
                               recordedRoot,
                              proofWitness};

  if (parentScene) {
    const auto edgeId = 映射编码(firstSaved, {11});
    if (!edgeId) {
      out.结果头.状态 = 场景直接包含状态::已可能发布;
      out.结果头.首次发布H = H;
      out.场景角色.reset();
      out.树证明.reset();
      return out;
    }
    if (!rememberRelation(*edgeId)) {
      out.结果头.状态 = 场景直接包含状态::已可能发布;
      out.结果头.首次发布H = H;
      out.场景角色.reset();
      out.树证明.reset();
      return out;
    }
    if (verifiedRelations.size() > relationBudget)
      return failBudget();
    const auto edge = 读关系(*edgeId, guard.Gread);
    if (!edge.成功() || edge.事实->源节点 != *parentScene ||
        edge.事实->目标节点 != object ||
        edge.事实->关系类型节点 != includeLayout_.直接子场景关系类型 ||
        edge.事实->角色或顺序 != 1 || edge.事实->创建事实代次 != H) {
      out.结果头.状态 = 场景直接包含状态::已可能发布;
      out.结果头.首次发布H = H;
      out.场景角色.reset();
      out.树证明.reset();
      return out;
    }
    auto witness = 转边(*edge.事实);
    out.最近上游场景 = parentScene;
    out.新直接包含 =
        场景直接包含事实{guard.Gread, 场景直接包含种类::子场景,
                           *parentScene, object, witness};
    out.既有直接父 = 直接归属联合事实{
        guard.Gread, 直接归属来源::直接子场景, *edgeId, *parentScene,
        object, edge.事实->创建事实代次};
  } else if (requestedExistingParent) {
    const 直接归属联合父读取请求 directRequest{
        1, guard.Gread, object, relationBudget};
    const auto direct = joint.读取当前联合父(directRequest);
    if (!direct.父读取成功(directRequest) || !direct.父 ||
        direct.父->父 != *requestedExistingParent ||
        direct.父->创建事实代次 > H) {
      out.结果头.状态 = 场景直接包含状态::幂等冲突;
      out.场景角色.reset();
      out.树证明.reset();
      return out;
    }
    if (!rememberRelation(direct.父->关系)) {
      out.结果头.状态 = 场景直接包含状态::已可能发布;
      out.结果头.首次发布H = H;
      out.场景角色.reset();
      out.树证明.reset();
      return out;
    }
    if (verifiedRelations.size() > relationBudget)
      return failBudget();
    out.既有直接父 = *direct.父;

    稳定编码 cursor = *requestedExistingParent;
    for (;;) {
      if (!visitedAncestors.insert(cursor.值).second) {
        out.结果头.状态 = 场景直接包含状态::已可能发布;
        out.结果头.首次发布H = H;
        out.场景角色.reset();
        out.树证明.reset();
        out.既有直接父.reset();
        return out;
      }
      if (visitedAncestors.size() > ancestorBudget)
        return failBudget();
      const auto upstreamRole =
          joint.读取当前场景角色位置({1, guard.Gread, cursor});
      if (!upstreamRole.成功({1, guard.Gread, cursor})) {
        out.结果头.状态 = 场景直接包含状态::已可能发布;
        out.结果头.首次发布H = H;
        out.场景角色.reset();
        out.树证明.reset();
        out.既有直接父.reset();
        return out;
      }
      if (upstreamRole.角色) {
        if (!upstreamRole.角色->树证明关系 ||
            !rememberRelation(*upstreamRole.角色->树证明关系)) {
          out.结果头.状态 = 场景直接包含状态::已可能发布;
          out.结果头.首次发布H = H;
          out.场景角色.reset();
          out.树证明.reset();
          out.既有直接父.reset();
          return out;
        }
        if (verifiedRelations.size() > relationBudget)
          return failBudget();
        if (upstreamRole.角色->树根 == recordedRoot) {
          out.最近上游场景 = cursor;
          break;
        }
        out.结果头.状态 = 场景直接包含状态::已可能发布;
        out.结果头.首次发布H = H;
        out.场景角色.reset();
        out.树证明.reset();
        out.既有直接父.reset();
        return out;
      }
      if (visitedAncestors.size() == ancestorBudget)
        return failBudget();
      if (verifiedRelations.size() >= relationBudget)
        return failBudget();
      const auto remaining = relationBudget - verifiedRelations.size();
      const 直接归属联合父读取请求 upRequest{
          1, guard.Gread, cursor, remaining};
      const auto up = joint.读取当前联合父(upRequest);
      if (!up.父读取成功(upRequest) || !up.父 ||
          !rememberRelation(up.父->关系)) {
        out.结果头.状态 = 场景直接包含状态::已可能发布;
        out.结果头.首次发布H = H;
        out.场景角色.reset();
        out.树证明.reset();
        out.既有直接父.reset();
        return out;
      }
      if (verifiedRelations.size() > relationBudget)
        return failBudget();
      cursor = up.父->父;
    }
  }
  if (!rootMarker) {
    const 场景父语境读取请求 contextRequest{
        1, guard.Gread, object, relationBudget};
    auto context = 读取当前父场景语境(contextRequest);
    if (!context.成功(contextRequest) || !context.投影 ||
        !out.既有直接父 ||
        context.投影->结构父.关系 != out.既有直接父->关系) {
      out.结果头.状态 = 场景直接包含状态::已可能发布;
      return out;
    }
    out.父语境投影 = std::move(context.投影);
  }
  out.结果头.首次发布H = H;
  out.结果头.状态 = 场景直接包含状态::精确重复;
  return out;
}

场景树角色写结果 场景类数据服务::启用树角色核心(
    std::uint64_t g, L1所有者范围写入幂等身份 key, 稳定编码 object,
    std::optional<稳定编码> parentScene,
    std::optional<稳定编码> nearestUpstream,
    std::optional<直接归属联合事实> existingParent,
    std::optional<稳定编码> treeRoot, bool rootMarker,
    std::uint64_t relationBudget, std::uint64_t ancestorBudget,
    const 直接归属联合只读提供者 &joint) {
  场景树角色写结果 out{{场景直接包含状态::入口拒绝, 1, g, std::nullopt},
                        std::nullopt,
                        std::nullopt,
                        std::nullopt,
                        std::nullopt,
                        std::nullopt};
  if (!g || g == UINT64_MAX || !普通幂等身份有效(key) || !有效(object) ||
      !treeRoot || !有效(*treeRoot) || !joint.绑定于(l1_))
    return out;
  if (auto replay = 尝试重放树角色(g, key, object, parentScene,
                                    existingParent
                                        ? std::optional<稳定编码>{existingParent->父}
                                        : std::nullopt,
                                    rootMarker, relationBudget, ancestorBudget, joint))
    return *replay;
  bool started = false;
  try {
    const auto e = existence_.确认当前存在结构身份(g, object);
    if (e.状态 != 存在结构身份只读状态::已读取) {
      out.结果头.状态 = 存在到包含(e.状态);
      out.结果头.Gread = e.Gread;
      return out;
    }
    const auto old = 读取当前场景角色位置({1, g, object});
    if (!old.成功({1, g, object}) || old.角色) {
      out.结果头.状态 = old.角色 ? 场景直接包含状态::成员已归属
                                 : 场景直接包含状态::内部不一致;
      return out;
    }
    auto ws = 形成启用写集(g, key, object);
    ws.关系.push_back({{10},
                       object,
                       rootMarker ? includeLayout_.树登记锚点 : *treeRoot,
                       rootMarker ? includeLayout_.根标记关系类型
                                  : includeLayout_.树归属关系类型,
                       1});
    if (parentScene)
      ws.关系.push_back(
          {{11}, *parentScene, object, includeLayout_.直接子场景关系类型, 1});
    if (parentScene)
      ws.关系.push_back(
          {{12}, object, *parentScene, layout_.父场景语境关系类型, 1});
    else if (existingParent)
      ws.关系.push_back({{11}, object, existingParent->父,
                         layout_.父场景语境关系类型, 1});
    started = true;
    const auto saved = 串行提交(ws);
    out.结果头.Gread = saved.事实代次;
    if (saved.状态 != L1所有者范围写入状态::成功 &&
        saved.状态 != L1所有者范围写入状态::精确重复) {
      if (saved.状态 == L1所有者范围写入状态::事实代次漂移)
        out.结果头.状态 = 场景直接包含状态::事实代次漂移;
      else if (saved.状态 == L1所有者范围写入状态::幂等冲突)
        out.结果头.状态 = 场景直接包含状态::幂等冲突;
      else if (saved.是否形成内存权威发布)
        out.结果头.状态 = 场景直接包含状态::已可能发布;
      else if (saved.状态 == L1所有者范围写入状态::资源失败)
        out.结果头.状态 = 场景直接包含状态::资源失败;
      else
        out.结果头.状态 = 场景直接包含状态::内部不一致;
      if (saved.是否形成内存权威发布)
        out.结果头.首次发布H = saved.事实代次;
      return out;
    }
    if (saved.状态 == L1所有者范围写入状态::精确重复) {
      if (auto replay = 尝试重放树角色(
              g, key, object, parentScene,
              existingParent ? std::optional<稳定编码>{existingParent->父}
                             : std::nullopt,
              rootMarker, relationBudget, ancestorBudget, joint))
        return *replay;
      out.结果头.状态 = 场景直接包含状态::已可能发布;
      return out;
    }
    out.结果头.首次发布H = saved.事实代次;
    const auto role = 读角色(saved.事实代次, object);
    const auto pos = 读取当前场景角色位置({1, saved.事实代次, object});
    if (!role.成功({2, saved.事实代次, object}) ||
        !pos.成功({1, saved.事实代次, object}) || !pos.角色 ||
        !pos.角色->树证明关系) {
      out.结果头.状态 = 场景直接包含状态::已可能发布;
      return out;
    }
    const auto proofEdge = 读关系(*pos.角色->树证明关系, saved.事实代次);
    if (!proofEdge.成功()) {
      out.结果头.状态 = 场景直接包含状态::已可能发布;
      return out;
    }
    if (existingParent && !parentScene) {
      const 直接归属联合父读取请求 currentParentRequest{
          1, saved.事实代次, object, 4096};
      const auto currentParent = joint.读取当前联合父(currentParentRequest);
      if (!currentParent.父读取成功(currentParentRequest) || !currentParent.父 ||
          currentParent.父->父 != existingParent->父 ||
          currentParent.父->来源 != existingParent->来源 ||
          currentParent.父->关系 != existingParent->关系) {
        out.结果头.状态 = 场景直接包含状态::已可能发布;
        return out;
      }
      existingParent = currentParent.父;
    }
    out.场景角色 = role.角色;
    out.树证明 = 场景树证明事实{saved.事实代次,
                                rootMarker ? 场景树证明种类::根标记
                                           : 场景树证明种类::树归属,
                                proofEdge.事实->编码,
                                object,
                                 *treeRoot,
                                 转边(*proofEdge.事实)};
    out.最近上游场景 = nearestUpstream;
    out.既有直接父 = existingParent;
    if (parentScene) {
      auto group = 读取当前场景包含父组({1, saved.事实代次, object, 4096});
      for (const auto &x : group.包含组)
        if (x.种类 == 场景直接包含种类::子场景 && x.父场景 == *parentScene) {
          out.新直接包含 = x;
          break;
        }
      if (!out.新直接包含) {
        out.结果头.状态 = 场景直接包含状态::已可能发布;
        return out;
      }
      out.既有直接父 =
          直接归属联合事实{saved.事实代次,
                           直接归属来源::直接子场景,
                           out.新直接包含->关系.编码,
                           *parentScene,
                           object,
                           out.新直接包含->关系.生命周期.创建事实代次};
    }
    if (!rootMarker) {
      const 场景父语境读取请求 contextRequest{
          1, saved.事实代次, object, relationBudget};
      const auto context = 读取当前父场景语境(contextRequest);
      if (!context.成功(contextRequest) || !context.投影 ||
          !out.既有直接父 ||
          context.投影->结构父.关系 != out.既有直接父->关系) {
        out.结果头.状态 = 场景直接包含状态::已可能发布;
        return out;
      }
      out.父语境投影 = context.投影;
    }
    if (saved.状态 == L1所有者范围写入状态::精确重复)
      out.结果头.状态 = 场景直接包含状态::精确重复;
    else
      out.结果头.状态 = rootMarker ? 场景直接包含状态::已建立场景树根
                                   : 场景直接包含状态::已启用并纳入场景树;
  } catch (const std::bad_alloc &) {
    out.结果头.状态 =
        started ? 场景直接包含状态::已可能发布 : 场景直接包含状态::资源失败;
  } catch (...) {
    out.结果头.状态 =
        started ? 场景直接包含状态::已可能发布 : 场景直接包含状态::内部不一致;
  }
  return out;
}

场景树角色写结果
场景类数据服务::启用并建立场景树根(const 场景树根启用请求 &r,
                                   const 直接归属联合只读提供者 &joint) {
  if (r.版本 != 1 || r.最大关系数量 < 1 || r.最大关系数量 > 4096 ||
      r.最大祖先数量 < 1 || r.最大祖先数量 > 4096 || !joint.绑定于(l1_))
    return {};
  if (auto replay = 尝试重放树角色(
          r.G0, r.幂等身份, r.对象存在, std::nullopt, std::nullopt, true,
          r.最大关系数量, r.最大祖先数量, joint))
    return *replay;
  const 直接归属联合父读取请求 pr{1, r.G0, r.对象存在, r.最大关系数量};
  const auto p = joint.读取当前联合父(pr);
  if (!p.父读取成功(pr) || p.父)
    return {};
  return 启用树角色核心(r.G0, r.幂等身份, r.对象存在, std::nullopt,
                         std::nullopt, std::nullopt, r.对象存在, true,
                         r.最大关系数量, r.最大祖先数量, joint);
}

场景树角色写结果
场景类数据服务::启用并接纳直接子场景(const 场景直接子场景启用请求 &r,
                                     const 直接归属联合只读提供者 &joint) {
  if (r.版本 != 1 || r.最大关系数量 < 1 || r.最大关系数量 > 4096 ||
      r.最大祖先数量 < 1 || r.最大祖先数量 > 4096 || !joint.绑定于(l1_))
    return {};
  if (auto replay = 尝试重放树角色(
          r.G0, r.幂等身份, r.对象存在, r.父场景, std::nullopt, false,
          r.最大关系数量, r.最大祖先数量, joint))
    return *replay;
  const auto parent = 读取当前场景角色位置({1, r.G0, r.父场景});
  if (!parent.成功({1, r.G0, r.父场景}) || !parent.角色 || !parent.角色->树根)
    return {};
  const 直接归属联合父读取请求 pr{1, r.G0, r.对象存在, r.最大关系数量};
  const auto p = joint.读取当前联合父(pr);
  if (!p.父读取成功(pr) || p.父)
    return {};
  return 启用树角色核心(r.G0, r.幂等身份, r.对象存在, r.父场景, r.父场景,
                         std::nullopt, parent.角色->树根, false,
                         r.最大关系数量, r.最大祖先数量, joint);
}


场景直接包含单项结果
场景类数据服务::读取当前直接包含(const 场景直接包含当前读取请求 &r) const {
  场景直接包含单项结果 out{
      {场景直接包含状态::入口拒绝, 1, r.G0, std::nullopt},
      std::nullopt};
  if (r.版本 != 1 || !r.G0 || !有效(r.关系))
    return out;
  try {
    const auto rel = 读关系(r.关系, r.G0);
    if (!rel.成功()) {
      out.结果头.状态 = 包含读取状态(rel.状态);
      out.结果头.Gread = rel.Gread;
      return out;
    }
    const auto &e = *rel.事实;
    场景直接包含种类 kind;
    if (e.关系类型节点 == includeLayout_.直接存在成员关系类型)
      kind = 场景直接包含种类::存在成员;
    else if (e.关系类型节点 == includeLayout_.直接子场景关系类型)
      kind = 场景直接包含种类::子场景;
    else {
      out.结果头.状态 = 场景直接包含状态::内部不一致;
      return out;
    }
    if (e.创建事实代次 > r.G0) {
      out.结果头.状态 = 场景直接包含状态::成员未找到;
      return out;
    }
    auto witness = 转边(e);
    out.包含 = 场景直接包含事实{r.G0, kind, e.源节点, e.目标节点,
                                 witness};
    out.结果头.状态 = 场景直接包含状态::已读取;
  } catch (const std::bad_alloc &) {
    out.结果头.状态 = 场景直接包含状态::资源失败;
  } catch (...) {
    out.结果头.状态 = 场景直接包含状态::内部不一致;
  }
  return out;
}

场景树当前结果
场景类数据服务::读取当前场景树(const 场景树当前读取请求 &r,
                               const 直接归属联合只读提供者 &joint) const {
  场景树当前结果 out{{场景直接包含状态::入口拒绝, 1, r.G0, std::nullopt},
                     std::nullopt};
  if (r.版本 != 1 || !r.G0 || !有效(r.根场景) || r.最大场景数量 < 1 ||
      r.最大场景数量 > 4096 || r.最大关系数量 < 1 || r.最大关系数量 > 4096 ||
      !joint.绑定于(l1_))
    return out;
  struct 待查 {
    稳定编码 E;
    std::optional<直接归属联合事实> parent;
    std::vector<直接归属联合事实> path;
  };
  try {
    const auto rootPos = 读取当前场景角色位置({1, r.G0, r.根场景});
    if (!rootPos.成功({1, r.G0, r.根场景})) {
      out.结果头.状态 = 联合到包含(rootPos.状态);
      out.结果头.Gread = rootPos.Gread;
      return out;
    }
    if (!rootPos.角色 || rootPos.角色->位置 != 直接归属场景位置::场景树根 ||
        !rootPos.角色->树根 || *rootPos.角色->树根 != r.根场景) {
      out.结果头.状态 = 场景直接包含状态::场景未找到;
      return out;
    }
    const 直接归属联合父读取请求 rootParentRequest{
        1, r.G0, r.根场景, r.最大关系数量};
    const auto rootParent = joint.读取当前联合父(rootParentRequest);
    if (!rootParent.父读取成功(rootParentRequest)) {
      out.结果头.状态 = 联合到包含(rootParent.状态);
      out.结果头.Gread = rootParent.Gread;
      return out;
    }
    if (rootParent.父) {
      out.结果头.状态 = 场景直接包含状态::内部不一致;
      return out;
    }
    std::vector<待查> pending{{r.根场景, std::nullopt, {}}};
    std::unordered_set<std::uint64_t> seen;
    std::unordered_set<std::uint64_t> relationSeen;
    场景树当前事实 tree{r.G0, r.根场景, {}};
    std::uint64_t edgeCount = 0;
    for (std::size_t i = 0; i < pending.size(); ++i) {
      auto item = std::move(pending[i]);
      if (!seen.insert(item.E.值).second) {
        out.结果头.状态 = 场景直接包含状态::形成场景环;
        return out;
      }
      const auto rolePos = joint.读取当前场景角色位置({1, r.G0, item.E});
      if (!rolePos.成功({1, r.G0, item.E})) {
        out.结果头.状态 = 联合到包含(rolePos.状态);
        return out;
      }
      std::optional<稳定编码> upstream;
      if (rolePos.角色) {
        if (!rolePos.角色->树根 || *rolePos.角色->树根 != r.根场景 ||
            !rolePos.角色->树证明关系) {
          out.结果头.状态 = 场景直接包含状态::引用冲突;
          return out;
        }
        const auto role = 读角色(r.G0, item.E);
        const auto proof = 读关系(*rolePos.角色->树证明关系, r.G0);
        if (!role.成功({2, r.G0, item.E}) || !proof.成功()) {
          out.结果头.状态 = 场景直接包含状态::内部不一致;
          return out;
        }
        relationSeen.insert(rolePos.角色->树证明关系->值);
        if (relationSeen.size() > r.最大关系数量) {
          out.结果头.状态 = 场景直接包含状态::数量预算不足;
          return out;
        }
        场景树节点当前事实 node;
        node.场景角色 = *role.角色;
        node.树证明 = {r.G0,
                       rolePos.角色->位置 == 直接归属场景位置::场景树根
                           ? 场景树证明种类::根标记
                           : 场景树证明种类::树归属,
                       proof.事实->编码,
                       item.E,
                       r.根场景,
                       转边(*proof.事实)};
        node.直接父 = item.parent;
        node.从上游场景到本场景路径 = item.path;
        if (item.parent) {
          const 场景父语境读取请求 contextRequest{
              1, r.G0, item.E, r.最大关系数量};
          const auto context = 读取当前父场景语境(contextRequest);
          if (!context.成功(contextRequest) || !context.投影 ||
              context.投影->结构父.关系 != item.parent->关系) {
            out.结果头.状态 = 场景直接包含状态::内部不一致;
            return out;
          }
          node.父语境投影 = context.投影;
          relationSeen.insert(context.投影->投影边.编码.值);
          if (relationSeen.size() > r.最大关系数量) {
            out.结果头.状态 = 场景直接包含状态::数量预算不足;
            return out;
          }
        }
        const 场景直接包含组读取请求 gr{1, r.G0, item.E, r.最大关系数量};
        const auto direct = 读取当前场景包含子组(gr);
        if (!direct.子组读取成功(gr)) {
          out.结果头 = direct.结果头;
          return out;
        }
        for (const auto &x : direct.包含组) {
          if (x.种类 == 场景直接包含种类::存在成员)
            node.直接存在成员组.push_back(x);
          else
            node.直接子场景组.push_back(x);
        }
        tree.场景组.push_back(std::move(node));
        if (tree.场景组.size() > r.最大场景数量) {
          out.结果头.状态 = 场景直接包含状态::数量预算不足;
          return out;
        }
        upstream = item.E;
      }
      const 直接归属联合子组读取请求 cr{1, r.G0, item.E, r.最大关系数量};
      const auto children = joint.读取当前联合子组(cr);
      if (!children.子组读取成功(cr)) {
        out.结果头.状态 = 联合到包含(children.状态);
        return out;
      }
      for (const auto &e : children.子组) {
        relationSeen.insert(e.关系.值);
        edgeCount = relationSeen.size();
        if (edgeCount > r.最大关系数量) {
          out.结果头.状态 = 场景直接包含状态::数量预算不足;
          return out;
        }
        待查 next;
        next.E = e.成员;
        next.parent = e;
        if (upstream) {
          next.path = {e};
        } else {
          next.path = item.path;
          next.path.push_back(e);
        }
        pending.push_back(std::move(next));
      }
    }
    std::sort(tree.场景组.begin(), tree.场景组.end(),
              [](const auto &a, const auto &b) {
                return a.场景角色.场景.值 < b.场景角色.场景.值;
              });
    if (tree.场景组.empty() || tree.场景组.front().场景角色.场景 != r.根场景) {
      const auto it = std::find_if(
          tree.场景组.begin(), tree.场景组.end(),
          [&](const auto &n) { return n.场景角色.场景 == r.根场景; });
      if (it == tree.场景组.end()) {
        out.结果头.状态 = 场景直接包含状态::内部不一致;
        return out;
      }
    }
    out.树 = std::move(tree);
    out.结果头.状态 = 场景直接包含状态::已读取;
  } catch (const std::bad_alloc &) {
    out.结果头.状态 = 场景直接包含状态::资源失败;
  } catch (...) {
    out.结果头.状态 = 场景直接包含状态::内部不一致;
  }
  return out;
}

场景树当前完整读取结果_v2
场景类数据服务::读取当前场景树_v2(
    const 场景树当前完整读取请求_v2 &r,
    const 直接归属联合只读提供者 &joint) const {
  场景树当前完整读取结果_v2 out;
  out.Gread = r.G0;
  if (r.版本 != 场景树当前完整读取合同版本_v2 || !r.G0 ||
      !有效(r.根场景) || !joint.绑定于(l1_))
    return out;

  const auto same = [](const 直接归属联合事实 &a,
                       const 直接归属联合事实 &b) noexcept {
    return a.Gread == b.Gread && a.来源 == b.来源 &&
           a.关系 == b.关系 && a.父 == b.父 && a.成员 == b.成员 &&
           a.创建事实代次 == b.创建事实代次;
  };
  auto mapRole = [](直接归属场景角色当前完整读取状态_v2 status,
                    bool root) noexcept {
    using S = 直接归属场景角色当前完整读取状态_v2;
    switch (status) {
    case S::未找到:
      return root ? 场景树当前完整读取状态_v2::根未找到
                  : 场景树当前完整读取状态_v2::内部不一致;
    case S::事实代次漂移:
      return 场景树当前完整读取状态_v2::事实代次漂移;
    case S::资源失败:
      return 场景树当前完整读取状态_v2::资源失败;
    default:
      return 场景树当前完整读取状态_v2::内部不一致;
    }
  };
  auto mapJoint = [](直接归属联合当前完整读取状态_v2 status) noexcept {
    using S = 直接归属联合当前完整读取状态_v2;
    switch (status) {
    case S::事实代次漂移:
      return 场景树当前完整读取状态_v2::事实代次漂移;
    case S::资源失败:
      return 场景树当前完整读取状态_v2::资源失败;
    default:
      return 场景树当前完整读取状态_v2::内部不一致;
    }
  };
  auto mapScene = [](场景直接包含当前完整读取状态_v2 status) noexcept {
    using S = 场景直接包含当前完整读取状态_v2;
    switch (status) {
    case S::事实代次漂移:
      return 场景树当前完整读取状态_v2::事实代次漂移;
    case S::资源失败:
      return 场景树当前完整读取状态_v2::资源失败;
    default:
      return 场景树当前完整读取状态_v2::内部不一致;
    }
  };
  auto mapPrivate = [](场景角色数据状态 status) noexcept {
    switch (status) {
    case 场景角色数据状态::事实代次漂移:
      return 场景树当前完整读取状态_v2::事实代次漂移;
    case 场景角色数据状态::资源失败:
      return 场景树当前完整读取状态_v2::资源失败;
    default:
      return 场景树当前完整读取状态_v2::内部不一致;
    }
  };

  struct 待查_v2 final {
    稳定编码 节点{};
    std::optional<直接归属联合事实> 直接父;
    std::vector<直接归属联合事实> 最近上游场景路径;
    std::vector<稳定编码> 完整祖先链;
  };
  try {
    const 直接归属场景角色当前完整读取请求_v2 rootRoleRequest{
        直接归属场景角色当前完整读取合同版本_v2, r.G0, r.根场景};
    const auto rootRole = 读取当前场景角色位置_v2(rootRoleRequest);
    if (!rootRole.成功(rootRoleRequest)) {
      out.Gread = rootRole.Gread;
      out.状态 = mapRole(rootRole.状态, true);
      return out;
    }
    if (!rootRole.角色 ||
        rootRole.角色->位置 != 直接归属场景位置::场景树根 ||
        !rootRole.角色->树根 || *rootRole.角色->树根 != r.根场景) {
      out.状态 = 场景树当前完整读取状态_v2::根角色未启用;
      return out;
    }
    const 直接归属联合父当前完整读取请求_v2 rootParentRequest{
        直接归属联合当前完整读取合同版本_v2, r.G0, r.根场景};
    const auto rootParent = joint.读取当前联合父_v2(rootParentRequest);
    if (!rootParent.父读取成功(rootParentRequest)) {
      out.Gread = rootParent.Gread;
      out.状态 = mapJoint(rootParent.状态);
      return out;
    }
    if (rootParent.父) {
      out.状态 = 场景树当前完整读取状态_v2::内部不一致;
      return out;
    }

    std::vector<待查_v2> pending{
        {r.根场景, std::nullopt, {}, {r.根场景}}};
    std::unordered_set<std::uint64_t> queued{r.根场景.值};
    std::unordered_set<std::uint64_t> completed;
    std::unordered_set<std::uint64_t> authorityRelationIds;
    std::unordered_map<std::uint64_t, std::uint64_t> memberParent;
    std::unordered_set<std::uint64_t> proofEdges;
    std::unordered_set<std::uint64_t> contextEdges;
    场景树当前事实 tree{r.G0, r.根场景, {}};

    for (std::size_t cursor = 0; cursor < pending.size(); ++cursor) {
      auto item = std::move(pending[cursor]);
      queued.erase(item.节点.值);
      if (!completed.insert(item.节点.值).second) {
        out.状态 = 场景树当前完整读取状态_v2::引用冲突;
        return out;
      }
      if (item.直接父) {
        const 直接归属联合父当前完整读取请求_v2 parentRequest{
            直接归属联合当前完整读取合同版本_v2, r.G0, item.节点};
        const auto parent = joint.读取当前联合父_v2(parentRequest);
        if (!parent.父读取成功(parentRequest)) {
          out.Gread = parent.Gread;
          out.状态 = mapJoint(parent.状态);
          return out;
        }
        if (!parent.父 || !same(*parent.父, *item.直接父)) {
          out.状态 = 场景树当前完整读取状态_v2::引用冲突;
          return out;
        }
      }

      const 直接归属场景角色当前完整读取请求_v2 roleRequest{
          直接归属场景角色当前完整读取合同版本_v2, r.G0, item.节点};
      const auto rolePosition = joint.读取当前场景角色位置_v2(roleRequest);
      if (!rolePosition.成功(roleRequest)) {
        out.Gread = rolePosition.Gread;
        out.状态 = mapRole(rolePosition.状态, false);
        return out;
      }
      const bool isScene = rolePosition.角色.has_value();
      if (isScene &&
          (rolePosition.角色->位置 == 直接归属场景位置::未纳入场景树 ||
           !rolePosition.角色->树根 ||
           *rolePosition.角色->树根 != r.根场景 ||
           (item.节点 == r.根场景) !=
               (rolePosition.角色->位置 == 直接归属场景位置::场景树根))) {
        out.状态 = 场景树当前完整读取状态_v2::引用冲突;
        return out;
      }

      std::optional<std::size_t> sceneIndex;
      if (isScene) {
        const auto fullRole = 读当前完整角色_v2(r.G0, item.节点);
        if (!fullRole.成功({2, r.G0, item.节点})) {
          out.Gread = fullRole.Gread;
          out.状态 = mapPrivate(fullRole.状态);
          return out;
        }
        const auto proof = 读当前场景树证明_v2(
            r.G0, item.节点, r.根场景, *rolePosition.角色);
        if (!proof.成功(r.G0, item.节点, r.根场景,
                        *rolePosition.角色)) {
          out.Gread = proof.Gread;
          out.状态 = mapPrivate(proof.状态);
          return out;
        }
        if (!proofEdges.insert(proof.证明->关系.值).second ||
            contextEdges.contains(proof.证明->关系.值) ||
            authorityRelationIds.contains(proof.证明->关系.值)) {
          out.状态 = 场景树当前完整读取状态_v2::内部不一致;
          return out;
        }
        场景树节点当前事实 node;
        node.场景角色 = *fullRole.角色;
        node.树证明 = *proof.证明;
        node.直接父 = item.直接父;
        node.从上游场景到本场景路径 = item.最近上游场景路径;
        if (item.直接父) {
          const auto context =
              读当前父场景语境_v2(r.G0, item.节点, *item.直接父);
          if (!context.成功(r.G0, item.节点, *item.直接父)) {
            out.Gread = context.Gread;
            out.状态 = mapPrivate(context.状态);
            return out;
          }
          if (!contextEdges.insert(context.投影->投影边.编码.值).second ||
              proofEdges.contains(context.投影->投影边.编码.值) ||
              authorityRelationIds.contains(context.投影->投影边.编码.值)) {
            out.状态 = 场景树当前完整读取状态_v2::内部不一致;
            return out;
          }
          node.父语境投影 = context.投影;
        }
        const 场景直接包含子组当前完整读取请求_v2 directRequest{
            场景直接包含当前完整读取合同版本_v2, r.G0, item.节点};
        const auto direct = 读取当前场景包含子组_v2(directRequest);
        if (!direct.子组读取成功(directRequest)) {
          out.Gread = direct.Gread;
          out.状态 = mapScene(direct.状态);
          return out;
        }
        for (const auto &entry : direct.包含组)
          (entry.种类 == 场景直接包含种类::存在成员
               ? node.直接存在成员组
               : node.直接子场景组)
              .push_back(entry);
        tree.场景组.push_back(std::move(node));
        sceneIndex = tree.场景组.size() - 1;
      }

      const 直接归属联合子组当前完整读取请求_v2 childRequest{
          直接归属联合当前完整读取合同版本_v2, r.G0, item.节点};
      const auto children = joint.读取当前联合子组_v2(childRequest);
      if (!children.子组读取成功(childRequest)) {
        out.Gread = children.Gread;
        out.状态 = mapJoint(children.状态);
        return out;
      }
      std::unordered_set<std::uint64_t> localMembers;
      for (const auto &edge : children.子组) {
        if (std::ranges::find(item.完整祖先链, edge.成员) !=
            item.完整祖先链.end()) {
          out.状态 = 场景树当前完整读取状态_v2::形成场景环;
          return out;
        }
        if (!localMembers.insert(edge.成员.值).second || edge.父 != item.节点 ||
            proofEdges.contains(edge.关系.值) || contextEdges.contains(edge.关系.值) ||
            authorityRelationIds.contains(edge.关系.值) ||
            memberParent.contains(edge.成员.值)) {
          out.状态 = 场景树当前完整读取状态_v2::引用冲突;
          return out;
        }
        authorityRelationIds.insert(edge.关系.值);
        const auto [it, inserted] = memberParent.emplace(edge.成员.值, edge.父.值);
        if (!inserted && it->second != edge.父.值) {
          out.状态 = 场景树当前完整读取状态_v2::引用冲突;
          return out;
        }
        if (completed.contains(edge.成员.值) || queued.contains(edge.成员.值)) {
          out.状态 = 场景树当前完整读取状态_v2::引用冲突;
          return out;
        }
        待查_v2 next;
        next.节点 = edge.成员;
        next.直接父 = edge;
        next.完整祖先链 = item.完整祖先链;
        next.完整祖先链.push_back(edge.成员);
        if (isScene)
          next.最近上游场景路径 = {edge};
        else {
          next.最近上游场景路径 = item.最近上游场景路径;
          next.最近上游场景路径.push_back(edge);
        }
        queued.insert(edge.成员.值);
        pending.push_back(std::move(next));
      }
      if (sceneIndex) {
        const auto &sceneNode = tree.场景组[*sceneIndex];
        auto projected = [&](const 场景直接包含事实 &entry,
                             const 直接归属联合事实 &edge) noexcept {
          const auto source =
              entry.种类 == 场景直接包含种类::存在成员
                  ? 直接归属来源::场景成员
                  : 直接归属来源::直接子场景;
          return edge.来源 == source && edge.Gread == entry.Gread &&
                 edge.关系 == entry.关系.编码 &&
                 edge.父 == entry.父场景 && edge.成员 == entry.成员 &&
                 edge.创建事实代次 == entry.关系.生命周期.创建事实代次;
        };
        auto requireProjection = [&](const 场景直接包含事实 &entry) {
          return std::ranges::count_if(children.子组, [&](const auto &edge) {
                   return projected(entry, edge);
                 }) == 1;
        };
        if (!std::ranges::all_of(sceneNode.直接存在成员组, requireProjection) ||
            !std::ranges::all_of(sceneNode.直接子场景组, requireProjection)) {
          out.状态 = 场景树当前完整读取状态_v2::引用冲突;
          return out;
        }
        for (const auto &edge : children.子组) {
          if (edge.来源 != 直接归属来源::场景成员 &&
              edge.来源 != 直接归属来源::直接子场景)
            continue;
          const auto &group = edge.来源 == 直接归属来源::场景成员
                                  ? sceneNode.直接存在成员组
                                  : sceneNode.直接子场景组;
          if (std::ranges::count_if(group, [&](const auto &entry) {
                return projected(entry, edge);
              }) != 1) {
            out.状态 = 场景树当前完整读取状态_v2::引用冲突;
            return out;
          }
        }
      }
    }
    for (const auto &sceneNode : tree.场景组)
      for (const auto &child : sceneNode.直接子场景组)
        if (std::ranges::none_of(tree.场景组, [&](const auto &candidate) {
              return candidate.场景角色.场景 == child.成员;
            })) {
          out.状态 = 场景树当前完整读取状态_v2::引用冲突;
          return out;
        }
    std::sort(tree.场景组.begin(), tree.场景组.end(),
              [](const auto &a, const auto &b) {
                return a.场景角色.场景.值 < b.场景角色.场景.值;
              });
    out.树 = std::move(tree);
    out.状态 = 场景树当前完整读取状态_v2::已读取;
    if (!out.成功(r)) {
      out.树.reset();
      out.状态 = 场景树当前完整读取状态_v2::内部不一致;
    }
  } catch (const std::bad_alloc &) {
    out.状态 = 场景树当前完整读取状态_v2::资源失败;
  } catch (const std::length_error &) {
    out.状态 = 场景树当前完整读取状态_v2::资源失败;
  } catch (const std::system_error &) {
    out.状态 = 场景树当前完整读取状态_v2::资源失败;
  } catch (...) {
    out.状态 = 场景树当前完整读取状态_v2::内部不一致;
  }
  if (out.状态 != 场景树当前完整读取状态_v2::已读取)
    out.树.reset();
  return out;
}

状态使用绑定数据状态
场景类数据服务::场景到B(场景角色数据状态 s) noexcept {
  switch (s) {
  case 场景角色数据状态::已读取:
    return 状态使用绑定数据状态::已读取;
  case 场景角色数据状态::场景角色未启用:
  case 场景角色数据状态::未找到:
    return 状态使用绑定数据状态::场景未找到;
  case 场景角色数据状态::事实代次漂移:
    return 状态使用绑定数据状态::事实代次漂移;
  case 场景角色数据状态::资源失败:
    return 状态使用绑定数据状态::资源失败;
  case 场景角色数据状态::入口拒绝:
    return 状态使用绑定数据状态::入口拒绝;
  default:
    return 状态使用绑定数据状态::内部不一致;
  }
}
状态使用绑定数据状态
场景类数据服务::存在到B(存在结构身份只读状态 s) noexcept {
  switch (s) {
  case 存在结构身份只读状态::未找到:
    return 状态使用绑定数据状态::存在未找到;
  case 存在结构身份只读状态::入口拒绝:
    return 状态使用绑定数据状态::入口拒绝;
  case 存在结构身份只读状态::事实代次漂移:
    return 状态使用绑定数据状态::事实代次漂移;
  case 存在结构身份只读状态::资源失败:
    return 状态使用绑定数据状态::资源失败;
  default:
    return 状态使用绑定数据状态::内部不一致;
  }
}
状态使用绑定数据状态 场景类数据服务::状态到B(状态类数据状态 s) noexcept {
  switch (s) {
  case 状态类数据状态::未找到:
    return 状态使用绑定数据状态::状态未找到;
  case 状态类数据状态::正式特征类型未找到:
    return 状态使用绑定数据状态::引用冲突;
  case 状态类数据状态::准确值不相容:
  case 状态类数据状态::旧格式不支持:
  case 状态类数据状态::入口拒绝:
    return 状态使用绑定数据状态::入口拒绝;
  case 状态类数据状态::事实代次漂移:
    return 状态使用绑定数据状态::事实代次漂移;
  case 状态类数据状态::数量预算不足:
    return 状态使用绑定数据状态::数量预算不足;
  case 状态类数据状态::资源失败:
    return 状态使用绑定数据状态::资源失败;
  default:
    return 状态使用绑定数据状态::内部不一致;
  }
}

} // namespace 海中鱼巣
