module;

#include <algorithm>
#include <bit>
#include <cstdint>
#include <limits>
#include <map>
#include <new>
#include <optional>
#include <set>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

export module 海中鱼巣.领域.数据服务.因果类;

export import 海中鱼巣.领域.合同.因果结构;

export namespace 海中鱼巣 {

class 因果类数据服务 final : public 因果结构只读提供者 {
public:
  因果类数据服务() = delete;
  因果类数据服务(const 因果类数据服务 &) = delete;
  因果类数据服务 &operator=(const 因果类数据服务 &) = delete;
  因果类数据服务(因果类数据服务 &&) = delete;
  因果类数据服务 &operator=(因果类数据服务 &&) = delete;

  因果类数据服务(L1事实基座服务 &l1, L1所有者范围写端口 &&port,
                 const 因果结构交付 &layout, const 因果概念核验提供者 &concepts,
                 const 动态结构只读提供者 &dynamics,
                 const 状态使用绑定只读提供者 &bindings,
                 const 状态类数据服务 &states)
      : l1_(l1), port_(std::move(port)), owner_(port_.所有者身份()),
        layout_(layout), concepts_(concepts), dynamics_(dynamics),
        bindings_(bindings), states_(states) {
    if (!绑定于(l1_) || !有效(owner_) || !布局浅层有效())
      throw std::invalid_argument("invalid causal configuration");
    const auto owner =
        l1_.读取当前结构所有者({L1所有者范围CRUD合同版本, owner_});
    if (owner.状态 != L1所有者范围读取状态::成功 || !owner.所有者事实 ||
        owner.所有者事实->所有者 != owner_ ||
        owner.所有者事实->范围种类 != L1所有者范围种类::独占结构范围 ||
        owner.所有者事实->退出事实代次 || !布局材料有效())
      throw std::invalid_argument("invalid causal structure");
  }

  bool 绑定于(const L1事实基座服务 &s) const noexcept override {
    return &s == &l1_ && port_.绑定于(s) && concepts_.绑定于(s) &&
           dynamics_.绑定于(s) && bindings_.绑定于(s) && states_.绑定于(s);
  }

  因果单项结果 发布或复用因果(const 因果发布请求 &r) {
    if (!发布请求有效(r))
      return 单项失败(因果数据状态::入口拒绝);
    try {
      因果定义 definition = r.定义;
      const auto normalized = 规范化并核验(definition);
      if (normalized != 因果数据状态::已读取)
        return 单项失败(normalized, r.G0);
      const auto definitionItems = 定义项总数(definition);
      if (!definitionItems || *definitionItems > r.预算.最大定义项数)
        return 单项失败(因果数据状态::数量预算不足, r.G0);
      if (auto replay = 重放发布(r, definition))
        return *replay;
      const auto guard = 读取当前代次();
      if (guard.状态 != 因果数据状态::已读取)
        return 单项失败(guard.状态, guard.Gread);
      if (guard.Gread != r.G0)
        return 单项失败(因果数据状态::事实代次漂移, guard.Gread);
      const auto checked =
          concepts_.核验因果定义概念({1, r.G0, r.G0, definition, r.预算});
      const auto conceptStatus =
          核验概念回显(checked, definition, r.预算, r.G0);
      if (conceptStatus != 因果数据状态::已读取)
        return 单项失败(conceptStatus, r.G0);
      auto remaining = r.预算;
      if (!扣减概念用量(remaining, checked))
        return 单项失败(因果数据状态::内部不一致, r.G0);
      for (const auto &source : definition.来源组)
        if (const auto *q = std::get_if<因果信息身份>(&source.目标)) {
          const auto prior = 读取历史({1, r.G0, r.G0, *q, remaining}, std::nullopt,
                                    &remaining);
          if (!prior.成功())
            return 单项失败(映射来源(prior.操作.状态), prior.操作.Gread, r.G0);
        }
      const auto duplicate = 查询同义(r.G0, r.G0, definition, remaining, &remaining);
      if (!duplicate.成功())
        return 单项失败(duplicate.操作.状态, duplicate.操作.Gread, r.G0);
      if (duplicate.因果组.size() > 1)
        return 单项失败(因果数据状态::内部不一致, r.G0, r.G0);
      if (duplicate.因果组.size() == 1)
        return {{因果数据状态::已复用, 1, r.G0, r.G0, std::nullopt,
                 因果发布阶段::无写入},
                duplicate.因果组.front()};
      编码结果 encoded;
      编码定义(definition, encoded);
      if (encoded.引用组.size() >
              static_cast<std::size_t>(UINT32_MAX - 4u) ||
          encoded.值.size() + 4 > remaining.最大值元素数 ||
          encoded.引用组.size() + 1 > remaining.最大关系数 ||
          encoded.引用组.size() + 4 > remaining.最大来源材料数)
        return 单项失败(因果数据状态::数量预算不足, r.G0);
      auto ws = 形成发布写集(r, encoded);
      const auto saved = port_.提交所有者范围中性写集(ws);
      const auto status = 映射写入(saved.状态, 因果数据状态::已创建);
      if (status != 因果数据状态::已创建 && status != 因果数据状态::精确重复)
        return 提交失败(status, saved.事实代次);
      if (!写入头完整(saved, r.幂等身份, r.G0))
        return 提交后失败(saved.事实代次);
      const auto id = 映射编码(saved, 节点键);
      if (!id)
        return 提交后失败(saved.事实代次);
      const auto after = 读取当前代次();
      if (after.状态 != 因果数据状态::已读取)
        return 提交后失败(saved.事实代次, after.Gread);
      auto out = 读取历史({1, after.Gread, saved.事实代次, {*id}, r.预算},
                          std::nullopt);
      if (!out.成功() || out.内容->信息.定义 != definition)
        return 提交后失败(saved.事实代次, out.操作.Gread);
      return {{status, 1, after.Gread, saved.事实代次, saved.事实代次,
               因果发布阶段::已读回},
              std::move(out.内容)};
    } catch (const std::bad_alloc &) {
      return 单项失败(因果数据状态::资源失败);
    } catch (const std::length_error &) {
      return 单项失败(因果数据状态::资源失败);
    } catch (...) {
      return 单项失败(因果数据状态::内部不一致);
    }
  }

  因果单项结果 读取当前因果(const 因果当前读取请求 &r) const override {
    if (r.合同版本 != 1 || !r.G0 || !有效(r.身份) || !预算有效(r.预算))
      return 单项失败(因果数据状态::入口拒绝);
    try {
      const auto guard = 读取当前代次();
      if (guard.状态 != 因果数据状态::已读取)
        return 单项失败(guard.状态, guard.Gread);
      if (guard.Gread != r.G0)
        return 单项失败(因果数据状态::事实代次漂移, guard.Gread);
      auto out = 读取历史({1, r.G0, r.G0, r.身份, r.预算}, std::nullopt);
      const auto after = 读取当前代次();
      if (out.成功() &&
          (after.状态 != 因果数据状态::已读取 || after.Gread != r.G0))
        return 单项失败(after.状态 == 因果数据状态::已读取
                            ? 因果数据状态::事实代次漂移
                            : after.状态,
                        after.Gread);
      return out;
    } catch (const std::bad_alloc &) {
      return 单项失败(因果数据状态::资源失败);
    } catch (...) {
      return 单项失败(因果数据状态::内部不一致);
    }
  }

  因果单项结果 读取因果历史(const 因果历史读取请求 &r) const override {
    if (!历史请求有效(r))
      return 单项失败(因果数据状态::入口拒绝);
    try {
      const auto guard = 读取当前代次();
      if (guard.状态 != 因果数据状态::已读取)
        return 单项失败(guard.状态, guard.Gread, r.H);
      if (guard.Gread != r.Gread)
        return 单项失败(因果数据状态::事实代次漂移, guard.Gread, r.H);
      auto out = 读取历史(r, std::nullopt);
      const auto after = 读取当前代次();
      if (out.成功() &&
          (after.状态 != 因果数据状态::已读取 || after.Gread != r.Gread))
        return 单项失败(after.状态 == 因果数据状态::已读取
                            ? 因果数据状态::事实代次漂移
                            : after.状态,
                        after.Gread, r.H);
      return out;
    } catch (const std::bad_alloc &) {
      return 单项失败(因果数据状态::资源失败);
    } catch (...) {
      return 单项失败(因果数据状态::内部不一致);
    }
  }

  因果组结果 查询精确定义(const 因果精确定义查询请求 &r) const override {
    if (!查询请求有效(r))
      return 组失败(因果数据状态::入口拒绝);
    try {
      auto d = r.定义;
      const auto s = 规范化并核验(d);
      if (s != 因果数据状态::已读取)
        return 组失败(s, r.Gread, r.H);
      const auto definitionItems = 定义项总数(d);
      if (!definitionItems || *definitionItems > r.预算.最大定义项数)
        return 组失败(因果数据状态::数量预算不足, r.Gread, r.H);
      auto out = 查询同义(r.Gread, r.H, d, r.预算);
      const auto after = 读取当前代次();
      if (out.成功() &&
          (after.状态 != 因果数据状态::已读取 || after.Gread != r.Gread))
        return 组失败(after.状态 == 因果数据状态::已读取
                          ? 因果数据状态::事实代次漂移
                          : after.状态,
                      after.Gread, r.H);
      return out;
    } catch (const std::bad_alloc &) {
      return 组失败(因果数据状态::资源失败);
    } catch (...) {
      return 组失败(因果数据状态::内部不一致);
    }
  }

  因果组结果 按特征概念查询因果(const 因果按特征查询请求 &r) const override {
    if (r.合同版本 != 1 || !r.Gread || !r.H || r.H > r.Gread ||
        !有效(r.特征概念) || !预算有效(r.预算) ||
        (r.方向 != 因果特征召回方向::条件 && r.方向 != 因果特征召回方向::结果))
      return 组失败(因果数据状态::入口拒绝);
    try {
      const auto guard = 读取当前代次();
      if (guard.状态 != 因果数据状态::已读取)
        return 组失败(guard.状态, guard.Gread, r.H);
      if (guard.Gread != r.Gread)
        return 组失败(因果数据状态::事实代次漂移, guard.Gread, r.H);
      const auto type = r.方向 == 因果特征召回方向::条件
                            ? layout_.条件FC关系类型
                            : layout_.结果FC关系类型;
      const auto q = l1_.读取所有者范围历史关系组(
          {L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::目标,
           r.特征概念.编码, type, r.H});
      if (q.状态 != L1所有者范围读取状态::成功)
        return 组失败(映射读取(q.状态), q.读取事实代次, r.H);
      if (q.读取事实代次 != r.Gread)
        return 组失败(因果数据状态::事实代次漂移, q.读取事实代次, r.H);
      std::vector<因果信息身份> ids;
      std::map<std::uint64_t, std::set<std::uint64_t>> alreadyCountedByQ;
      for (const auto &e : q.关系组) {
        if (e.写入所有者 != owner_ || e.目标节点 != r.特征概念.编码 ||
            e.关系类型节点 != type)
          return 组失败(因果数据状态::内部不一致, r.Gread, r.H);
        ids.push_back({e.源节点});
        alreadyCountedByQ[e.源节点.值].insert(e.编码.值);
      }
      std::sort(ids.begin(), ids.end(),
                [](auto a, auto b) { return a.编码 < b.编码; });
      ids.erase(std::unique(ids.begin(), ids.end()), ids.end());
      auto remaining = r.预算;
      if (ids.size() > remaining.最大候选数 ||
          q.关系组.size() > remaining.最大关系数 ||
          q.关系组.size() > remaining.最大来源材料数)
        return 组失败(因果数据状态::数量预算不足, r.Gread, r.H);
      remaining.最大候选数 -= ids.size();
      remaining.最大关系数 -= q.关系组.size();
      remaining.最大来源材料数 -= q.关系组.size();
      因果组结果 out{读取头(r.Gread, r.H), {}};
      for (auto id : ids) {
        auto one = 读取历史({1, r.Gread, r.H, id, remaining}, std::nullopt,
                           &remaining, false, &alreadyCountedByQ.at(id.编码.值),
                           type, r.特征概念.编码);
        if (!one.成功())
          return 组失败(one.操作.状态, one.操作.Gread, r.H);
        out.因果组.push_back(*one.内容);
      }
      const auto after = 读取当前代次();
      if (after.状态 != 因果数据状态::已读取 || after.Gread != r.Gread)
        return 组失败(after.状态 == 因果数据状态::已读取
                          ? 因果数据状态::事实代次漂移
                          : after.状态,
                      after.Gread, r.H);
      return out;
    } catch (const std::bad_alloc &) {
      return 组失败(因果数据状态::资源失败);
    } catch (...) {
      return 组失败(因果数据状态::内部不一致);
    }
  }

  因果证据结果 关联因果证据(const 因果证据关联请求 &r) {
    if (!证据关联请求有效(r))
      return 证据失败(因果数据状态::入口拒绝);
    try {
      if (auto replay = 重放证据关联(r))
        return *replay;
      const auto guard = 读取当前代次();
      if (guard.状态 != 因果数据状态::已读取)
        return 证据失败(guard.状态, guard.Gread);
      if (guard.Gread != r.G0)
        return 证据失败(因果数据状态::事实代次漂移, guard.Gread);
      auto remaining = r.预算;
      const auto q =
          读取历史({1, r.G0, r.G0, r.因果, remaining}, std::nullopt, &remaining);
      if (!q.成功())
        return 证据失败(q.操作.状态, q.操作.Gread);
      if (q.内容->信息.定义.类型 == 因果类型::主动)
        return 证据失败(因果数据状态::动作来源未实现, r.G0);
      if (r.证据.动作来源)
        return 证据失败(因果数据状态::入口拒绝, r.G0);
      const auto checked = concepts_.核验因果定义概念(
          {1, r.G0, r.G0, q.内容->信息.定义, remaining});
      const auto cs = 核验概念回显(checked, q.内容->信息.定义, remaining, r.G0);
      if (cs != 因果数据状态::已读取)
        return 证据失败(cs, r.G0);
      if (!扣减概念用量(remaining, checked))
        return 证据失败(因果数据状态::内部不一致, r.G0);
      证据索引 index;
      const auto is = 读取证据索引(r.因果, r.G0, r.G0, index);
      if (is != 因果数据状态::已读取)
        return 证据失败(is, r.G0);
      auto normalized = r.证据;
      规范化证据(normalized);
      auto existing = std::lower_bound(index.项目组.begin(), index.项目组.end(),
                                       normalized.发生锚点.编码,
                                       [](const auto &a, const auto &b) {
                                         return a.内容.发生锚点.编码 < b;
                                       });
      if (existing != index.项目组.end() &&
          existing->内容.发生锚点 == normalized.发生锚点) {
        if (existing->内容 == normalized) {
          auto fact = 投影证据事实(r.因果, *existing, r.G0, r.G0);
          return {{因果数据状态::已复用, 1, r.G0, r.G0, std::nullopt,
                   因果发布阶段::无写入},
                  {std::move(fact)}};
        }
        return 证据失败(因果数据状态::引用冲突, r.G0);
      }
      来源读取缓存 sourceCache;
      const auto vs = 核验证据来源(q.内容->信息.定义, normalized, checked, r.G0,
                                  remaining, sourceCache);
      if (vs != 因果数据状态::已读取)
        return 证据失败(vs, r.G0);
      if (!添加证据索引项(index, normalized))
        return 证据失败(因果数据状态::数量预算不足, r.G0);
      const auto indexValues = 证据索引值元素数(index);
      const std::uint64_t newRelations =
          1 + normalized.角色绑定组.size() + normalized.状态证据组.size() * 2;
      if (!indexValues || newRelations > UINT32_MAX - 1u ||
          newRelations > remaining.最大关系数 ||
          *indexValues > remaining.最大值元素数 ||
          newRelations + 1 > remaining.最大来源材料数)
        return 证据失败(因果数据状态::数量预算不足, r.G0);
      auto ws = 形成证据关联写集(r, index);
      const auto saved = port_.提交所有者范围中性写集(ws);
      const auto status = 映射写入(saved.状态, 因果数据状态::已关联);
      if (status != 因果数据状态::已关联 && status != 因果数据状态::精确重复)
        return 证据提交失败(status, saved.事实代次);
      if (!写入头完整(saved, r.幂等身份, r.G0))
        return 证据提交后失败(saved.事实代次);
      const auto after = 读取当前代次();
      if (after.状态 != 因果数据状态::已读取)
        return 证据提交后失败(saved.事实代次, after.Gread);
      auto read = 读取因果证据({1, after.Gread, saved.事实代次, r.因果,
                                normalized.发生锚点, r.预算});
      if (!read.成功() || read.证据组.size() != 1)
        return 证据提交后失败(saved.事实代次, read.操作.Gread);
      read.操作 = {status,         1,
                   after.Gread,    saved.事实代次,
                   saved.事实代次, 因果发布阶段::已读回};
      return read;
    } catch (const std::bad_alloc &) {
      return 证据失败(因果数据状态::资源失败);
    } catch (const std::length_error &) {
      return 证据失败(因果数据状态::资源失败);
    } catch (...) {
      return 证据失败(因果数据状态::内部不一致);
    }
  }

  因果证据结果 读取因果证据(const 因果证据读取请求 &r) const override {
    if (r.合同版本 != 1 || !r.Gread || !r.H || r.H > r.Gread || !有效(r.因果) ||
        !预算有效(r.预算) || (r.发生锚点 && !有效(*r.发生锚点)))
      return 证据失败(因果数据状态::入口拒绝);
    try {
      const auto guard = 读取当前代次();
      if (guard.状态 != 因果数据状态::已读取)
        return 证据失败(guard.状态, guard.Gread, r.H);
      if (guard.Gread != r.Gread)
        return 证据失败(因果数据状态::事实代次漂移, guard.Gread, r.H);
      auto remaining = r.预算;
      const auto q = 读取历史({1, r.Gread, r.H, r.因果, remaining}, std::nullopt,
                             &remaining);
      if (!q.成功())
        return 证据失败(q.操作.状态, q.操作.Gread, r.H);
      证据索引 index;
      const auto s = 读取证据索引(r.因果, r.Gread, r.H, index);
      if (s != 因果数据状态::已读取)
        return 证据失败(s, r.Gread, r.H);
      因果证据结果 out{读取头(r.Gread, r.H), {}};
      来源读取缓存 sourceCache;
      for (const auto &v : index.项目组) {
        if (r.发生锚点 && v.内容.发生锚点 != *r.发生锚点)
          continue;
        const auto valid = 核验证据来源(q.内容->信息.定义, v.内容, {}, r.Gread,
                                      remaining, sourceCache, false);
        if (valid != 因果数据状态::已读取)
          return 证据失败(valid, r.Gread, r.H);
        out.证据组.push_back(投影证据事实(r.因果, v, r.Gread, r.H));
      }
      if (r.发生锚点 && out.证据组.empty())
        return 证据失败(因果数据状态::未找到, r.Gread, r.H);
      const auto after = 读取当前代次();
      if (after.状态 != 因果数据状态::已读取 || after.Gread != r.Gread)
        return 证据失败(after.状态 == 因果数据状态::已读取
                            ? 因果数据状态::事实代次漂移
                            : after.状态,
                        after.Gread, r.H);
      return out;
    } catch (const std::bad_alloc &) {
      return 证据失败(因果数据状态::资源失败);
    } catch (...) {
      return 证据失败(因果数据状态::内部不一致);
    }
  }

  因果证据结果 退出因果证据(const 因果证据退出请求 &r) {
    if (r.合同版本 != 1 || !r.G0 ||
        r.G0 == std::numeric_limits<std::uint64_t>::max() ||
        !有效(r.幂等身份) || !有效(r.定位.因果) || !有效(r.定位.发生锚点) ||
        !预算有效(r.预算))
      return 证据失败(因果数据状态::入口拒绝);
    try {
      if (auto replay = 重放证据退出(r))
        return *replay;
      const auto guard = 读取当前代次();
      if (guard.状态 != 因果数据状态::已读取)
        return 证据失败(guard.状态, guard.Gread);
      if (guard.Gread != r.G0)
        return 证据失败(因果数据状态::事实代次漂移, guard.Gread);
      证据索引 index;
      const auto s = 读取证据索引(r.定位.因果, r.G0, r.G0, index);
      if (s != 因果数据状态::已读取)
        return 证据失败(s, r.G0);
      auto remaining = r.预算;
      if (!扣减证据索引预算(remaining, index))
        return 证据失败(因果数据状态::数量预算不足, r.G0);
      auto it = std::lower_bound(index.项目组.begin(), index.项目组.end(),
                                 r.定位.发生锚点.编码,
                                 [](const auto &a, const auto &b) {
                                   return a.内容.发生锚点.编码 < b;
                                 });
      if (it == index.项目组.end() || it->内容.发生锚点 != r.定位.发生锚点)
        return 证据失败(因果数据状态::未找到, r.G0);
      const auto old = *it;
      index.项目组.erase(it);
      const auto indexValues = 证据索引值元素数(index);
      if (!indexValues || *indexValues > remaining.最大值元素数 ||
          remaining.最大来源材料数 < 1)
        return 证据失败(因果数据状态::数量预算不足, r.G0);
      auto ws = 形成证据退出写集(r, index, old);
      const auto saved = port_.提交所有者范围中性写集(ws);
      const auto status = 映射写入(saved.状态, 因果数据状态::已退出);
      if (status != 因果数据状态::已退出 && status != 因果数据状态::精确重复)
        return 证据提交失败(status, saved.事实代次);
      if (!写入头完整(saved, r.幂等身份, r.G0))
        return 证据提交后失败(saved.事实代次);
      const auto after = 读取当前代次();
      if (after.状态 != 因果数据状态::已读取)
        return 证据提交后失败(saved.事实代次, after.Gread);
      证据索引 confirmed;
      if (读取证据索引(r.定位.因果, after.Gread, saved.事实代次 - 1,
                       confirmed) != 因果数据状态::已读取)
        return 证据提交后失败(saved.事实代次, after.Gread);
      const auto confirmedItem =
          std::find_if(confirmed.项目组.begin(), confirmed.项目组.end(),
                       [&](const auto &v) {
                         return v.内容.发生锚点 == r.定位.发生锚点;
                       });
      if (confirmedItem == confirmed.项目组.end() ||
          confirmedItem->内容 != old.内容 ||
          confirmedItem->退出事实代次 != saved.事实代次)
        return 证据提交后失败(saved.事实代次, after.Gread);
      auto fact = 投影证据事实(r.定位.因果, *confirmedItem, after.Gread,
                               saved.事实代次 - 1, saved.事实代次);
      return {{status, 1, after.Gread, saved.事实代次 - 1, saved.事实代次,
               因果发布阶段::已读回},
              {std::move(fact)}};
    } catch (const std::bad_alloc &) {
      return 证据失败(因果数据状态::资源失败);
    } catch (...) {
      return 证据失败(因果数据状态::内部不一致);
    }
  }

  因果单项结果 退出因果(const 因果退出请求 &r) {
    if (r.合同版本 != 1 || !r.G0 ||
        r.G0 == std::numeric_limits<std::uint64_t>::max() ||
        !有效(r.幂等身份) || !有效(r.身份) || !预算有效(r.预算))
      return 单项失败(因果数据状态::入口拒绝);
    try {
      if (auto replay = 重放退出(r))
        return *replay;
      const auto guard = 读取当前代次();
      if (guard.状态 != 因果数据状态::已读取)
        return 单项失败(guard.状态, guard.Gread);
      if (guard.Gread != r.G0)
        return 单项失败(因果数据状态::事实代次漂移, guard.Gread);
      auto remaining = r.预算;
      const auto current = 读取历史({1, r.G0, r.G0, r.身份, remaining},
                                  std::nullopt, &remaining);
      if (!current.成功())
        return current;
      证据索引 index;
      const auto is = 读取证据索引(r.身份, r.G0, r.G0, index);
      if (is != 因果数据状态::已读取)
        return 单项失败(is, r.G0);
      if (!index.项目组.empty())
        return 单项失败(因果数据状态::引用冲突, r.G0);
      auto ws = 形成因果退出写集(r, *current.内容, index);
      const auto saved = port_.提交所有者范围中性写集(ws);
      const auto status = 映射写入(saved.状态, 因果数据状态::已退出);
      if (status != 因果数据状态::已退出 && status != 因果数据状态::精确重复)
        return 提交失败(status, saved.事实代次);
      if (!写入头完整(saved, r.幂等身份, r.G0))
        return 提交后失败(saved.事实代次);
      const auto after = 读取当前代次();
      if (after.状态 != 因果数据状态::已读取)
        return 提交后失败(saved.事实代次, after.Gread);
      auto read =
          读取历史({1, after.Gread, saved.事实代次 - 1, r.身份, r.预算},
                   saved.事实代次);
      if (!read.成功())
        return 提交后失败(saved.事实代次, read.操作.Gread);
      return {{status, 1, after.Gread, saved.事实代次 - 1, saved.事实代次,
               因果发布阶段::已读回},
              std::move(read.内容)};
    } catch (const std::bad_alloc &) {
      return 单项失败(因果数据状态::资源失败);
    } catch (...) {
      return 单项失败(因果数据状态::内部不一致);
    }
  }

  因果操作结果 确认当前因果结构身份(const 因果当前身份请求 &r) const override {
    if (r.合同版本 != 1 || !r.G0 || !有效(r.身份))
      return 失败头(因果数据状态::入口拒绝);
    try {
      const auto guard = 读取当前代次();
      if (guard.状态 != 因果数据状态::已读取)
        return 失败头(guard.状态, guard.Gread);
      if (guard.Gread != r.G0)
        return 失败头(因果数据状态::事实代次漂移, guard.Gread);
      auto h = 读取身份({1, r.G0, r.G0, r.身份});
      const auto after = 读取当前代次();
      if (h.成功() &&
          (after.状态 != 因果数据状态::已读取 || after.Gread != r.G0))
        return 失败头(after.状态 == 因果数据状态::已读取
                          ? 因果数据状态::事实代次漂移
                          : after.状态,
                      after.Gread);
      return h.成功() ? 读取头(r.G0, r.G0) : h.操作;
    } catch (const std::bad_alloc &) {
      return 失败头(因果数据状态::资源失败);
    } catch (...) {
      return 失败头(因果数据状态::内部不一致);
    }
  }

  因果身份历史结果 读取因果身份历史(const 因果历史身份请求 &r) const override {
    if (r.合同版本 != 1 || !r.Gread || !r.H || r.H > r.Gread || !有效(r.身份))
      return 身份失败(因果数据状态::入口拒绝);
    try {
      const auto guard = 读取当前代次();
      if (guard.状态 != 因果数据状态::已读取)
        return 身份失败(guard.状态, guard.Gread, r.H);
      if (guard.Gread != r.Gread)
        return 身份失败(因果数据状态::事实代次漂移, guard.Gread, r.H);
      auto out = 读取身份(r);
      const auto after = 读取当前代次();
      if (out.成功() &&
          (after.状态 != 因果数据状态::已读取 || after.Gread != r.Gread))
        return 身份失败(after.状态 == 因果数据状态::已读取
                            ? 因果数据状态::事实代次漂移
                            : after.状态,
                        after.Gread, r.H);
      return out;
    } catch (const std::bad_alloc &) {
      return 身份失败(因果数据状态::资源失败);
    } catch (...) {
      return 身份失败(因果数据状态::内部不一致);
    }
  }

private:
  inline static constexpr L1所有者范围写集本地键 节点键{1}, 族关系键{2},
      定义值键{3}, 证据索引值键{4};
  struct 代次结果 {
    因果数据状态 状态 = 因果数据状态::内部不一致;
    std::uint64_t Gread = 0;
  };
  struct 引用槽 {
    稳定编码 目标{};
    稳定编码 类型{};
  };
  struct 编码结果 {
    std::vector<std::uint64_t> 值;
    std::vector<引用槽> 引用组;
  };
  struct 证据索引项 {
    因果证据提交 内容;
    std::uint64_t 创建事实代次 = 0;
    std::optional<std::uint64_t> 退出事实代次;
    std::vector<std::pair<std::uint64_t, 稳定编码>> 引用关系组;
  };
  struct 证据索引 {
    std::uint64_t 最大序号 = 0;
    稳定编码 当前值{};
    std::vector<证据索引项> 项目组;
  };
  struct 来源读取缓存 {
    using 键 = std::tuple<std::uint64_t, std::uint64_t, std::uint64_t>;
    using 展开键 = std::tuple<std::uint64_t, std::uint64_t, std::uint64_t, unsigned>;
    std::map<展开键, 动态展开结果> 动态展开;
    std::map<键, 状态使用绑定读取结果> 绑定读取;
    std::map<键, 状态读取结果> 状态读取;
    std::map<键, 动态操作结果> 动态当前身份;
    std::set<std::uint64_t> 已计节点, 已计关系, 已计值;
  };

  static bool 预算有效(const 因果读取预算 &v) noexcept {
    return v.最大候选数 && v.最大定义项数 && v.最大关系数 && v.最大值元素数 &&
           v.最大来源材料数 && v.最大展开深度;
  }
  static std::optional<std::uint64_t> 定义项总数(const 因果定义 &d) noexcept {
    const auto add = [](std::uint64_t &sum, std::size_t n) noexcept {
      if (n > std::numeric_limits<std::uint64_t>::max() - sum)
        return false;
      sum += static_cast<std::uint64_t>(n);
      return true;
    };
    std::uint64_t sum = 0;
    if (!add(sum, d.参与者.角色组.size()) ||
        !add(sum, d.参与者.同异约束组.size()) ||
        !add(sum, d.条件组.size()) || !add(sum, d.条件组.size()) ||
        !add(sum, d.结果组.size()) || !add(sum, d.结果组.size()) ||
        !add(sum, d.时序组.size()) || !add(sum, d.来源组.size()))
      return std::nullopt;
    if (d.动作) {
      if (!add(sum, 1) || !add(sum, d.动作->参数组.size()) ||
          !add(sum, d.动作->参数组.size()))
        return std::nullopt;
    }
    return sum;
  }
  bool 扣减内容预算(因果读取预算 &remaining, const 因果内容事实 &content,
                     bool 族关系已计量 = false,
                     std::uint64_t 已计定义关系数 = 0) const {
    const auto items = 定义项总数(content.信息.定义);
    编码结果 encoded;
    编码定义(content.信息.定义, encoded);
    if (已计定义关系数 > content.定义引用关系组.size())
      return false;
    const std::uint64_t relations =
        static_cast<std::uint64_t>(content.定义引用关系组.size()) -
        已计定义关系数 + (族关系已计量 ? 0 : 1);
    const std::uint64_t materials = relations + 2;
    if (!items || *items > remaining.最大定义项数 ||
        relations > remaining.最大关系数 ||
        encoded.值.size() > remaining.最大值元素数 ||
        materials > remaining.最大来源材料数)
      return false;
    remaining.最大定义项数 -= *items;
    remaining.最大关系数 -= relations;
    remaining.最大值元素数 -= encoded.值.size();
    remaining.最大来源材料数 -= materials;
    return true;
  }
  static bool 扣减概念用量(因果读取预算 &remaining,
                           const 因果概念核验结果 &result) noexcept {
    if (result.已读取关系数 > remaining.最大关系数 ||
        result.已读取值元素数 > remaining.最大值元素数 ||
        result.已读取材料数 > remaining.最大来源材料数)
      return false;
    remaining.最大关系数 -= result.已读取关系数;
    remaining.最大值元素数 -= result.已读取值元素数;
    remaining.最大来源材料数 -= result.已读取材料数;
    return true;
  }
  static bool 可预留公开材料(const 因果读取预算 &remaining,
                           std::uint64_t relations, std::uint64_t values,
                           std::uint64_t materials) noexcept {
    return relations <= remaining.最大关系数 && values <= remaining.最大值元素数 &&
           materials <= remaining.最大来源材料数;
  }
  static bool 登记公开材料(因果读取预算 &remaining, 来源读取缓存 &cache,
                         const std::set<std::uint64_t> &nodes,
                         const std::set<std::uint64_t> &relations,
                         const std::set<std::uint64_t> &values) {
    std::vector<std::uint64_t> newNodes, newRelations, newValues;
    for (auto v : nodes)
      if (!cache.已计节点.contains(v))
        newNodes.push_back(v);
    for (auto v : relations)
      if (!cache.已计关系.contains(v))
        newRelations.push_back(v);
    for (auto v : values)
      if (!cache.已计值.contains(v))
        newValues.push_back(v);
    const auto materials = newNodes.size() + newRelations.size() + newValues.size();
    if (newRelations.size() > remaining.最大关系数 ||
        newValues.size() > remaining.最大值元素数 ||
        materials > remaining.最大来源材料数)
      return false;
    remaining.最大关系数 -= newRelations.size();
    remaining.最大值元素数 -= newValues.size();
    remaining.最大来源材料数 -= materials;
    cache.已计节点.insert(newNodes.begin(), newNodes.end());
    cache.已计关系.insert(newRelations.begin(), newRelations.end());
    cache.已计值.insert(newValues.begin(), newValues.end());
    return true;
  }
  static std::optional<std::uint64_t>
  证据索引值元素数(const 证据索引 &index) noexcept {
    std::uint64_t values = 4;
    for (const auto &item : index.项目组) {
      const auto r = static_cast<std::uint64_t>(item.内容.角色绑定组.size());
      const auto s = static_cast<std::uint64_t>(item.内容.状态证据组.size());
      const auto n = static_cast<std::uint64_t>(item.引用关系组.size());
      const std::uint64_t fixed = 6 + (item.内容.动作来源 ? 1 : 0);
      if (r > (std::numeric_limits<std::uint64_t>::max() - fixed) / 2 ||
          s > (std::numeric_limits<std::uint64_t>::max() - fixed - 2 * r) / 4 ||
          n > std::numeric_limits<std::uint64_t>::max() - fixed - 2 * r - 4 * s ||
          fixed + 2 * r + 4 * s + n >
              std::numeric_limits<std::uint64_t>::max() - values)
        return std::nullopt;
      values += fixed + 2 * r + 4 * s + n;
    }
    return values;
  }
  static bool 扣减证据索引预算(因果读取预算 &remaining,
                               const 证据索引 &index) noexcept {
    std::uint64_t relations = 0;
    for (const auto &item : index.项目组) {
      const auto n = static_cast<std::uint64_t>(item.引用关系组.size());
      if (n > std::numeric_limits<std::uint64_t>::max() - relations)
        return false;
      relations += n;
    }
    const auto values = 证据索引值元素数(index);
    if (!values)
      return false;
    if (relations == std::numeric_limits<std::uint64_t>::max() ||
        relations > remaining.最大关系数 || *values > remaining.最大值元素数 ||
        relations + 1 > remaining.最大来源材料数)
      return false;
    remaining.最大关系数 -= relations;
    remaining.最大值元素数 -= *values;
    remaining.最大来源材料数 -= relations + 1;
    return true;
  }
  static bool 历史请求有效(const 因果历史读取请求 &r) noexcept {
    return r.合同版本 == 1 && r.Gread && r.H && r.H <= r.Gread &&
           有效(r.身份) && 预算有效(r.预算);
  }
  static bool 查询请求有效(const 因果精确定义查询请求 &r) noexcept {
    return r.合同版本 == 1 && r.Gread && r.H && r.H <= r.Gread &&
           预算有效(r.预算);
  }
  static bool 发布请求有效(const 因果发布请求 &r) noexcept {
    return r.合同版本 == 1 && r.G0 &&
           r.G0 != std::numeric_limits<std::uint64_t>::max() &&
           有效(r.幂等身份) && 预算有效(r.预算);
  }
  static bool 证据关联请求有效(const 因果证据关联请求 &r) noexcept {
    return r.合同版本 == 1 && r.G0 &&
           r.G0 != std::numeric_limits<std::uint64_t>::max() &&
           有效(r.幂等身份) && 有效(r.因果) && 有效(r.证据.发生锚点) &&
           r.证据.证据截止 && r.证据.证据截止 <= r.G0 && 预算有效(r.预算);
  }
  代次结果 读取当前代次() const {
    const auto q = l1_.读取中性当前事实代次({L1中性CRUD合同版本});
    if (q.状态 == L1中性读取状态::资源失败)
      return {因果数据状态::资源失败, q.事实代次};
    if (q.状态 != L1中性读取状态::成功 || q.合同版本 != L1中性CRUD合同版本 ||
        !q.事实代次)
      return {因果数据状态::内部不一致, q.事实代次};
    return {因果数据状态::已读取, q.事实代次};
  }

  static 因果操作结果 失败头(因果数据状态 s, std::uint64_t g = 0,
                             std::uint64_t h = 0) noexcept {
    return {s, 1, g, h, std::nullopt, 因果发布阶段::无写入};
  }
  static 因果操作结果 读取头(std::uint64_t g, std::uint64_t h) noexcept {
    return {因果数据状态::已读取, 1, g, h, std::nullopt, 因果发布阶段::无写入};
  }
  static 因果单项结果 单项失败(因果数据状态 s, std::uint64_t g = 0,
                               std::uint64_t h = 0) noexcept {
    return {失败头(s, g, h), std::nullopt};
  }
  static 因果组结果 组失败(因果数据状态 s, std::uint64_t g = 0,
                           std::uint64_t h = 0) noexcept {
    return {失败头(s, g, h), {}};
  }
  static 因果证据结果 证据失败(因果数据状态 s, std::uint64_t g = 0,
                               std::uint64_t h = 0) noexcept {
    return {失败头(s, g, h), {}};
  }
  static 因果身份历史结果 身份失败(因果数据状态 s, std::uint64_t g = 0,
                                   std::uint64_t h = 0) noexcept {
    return {失败头(s, g, h), std::nullopt};
  }
  static 因果单项结果 提交失败(因果数据状态 s, std::uint64_t g) noexcept {
    if (s == 因果数据状态::已可能发布)
      return {{s, 1, g, 0, g ? std::optional<std::uint64_t>{g} : std::nullopt,
               因果发布阶段::未知},
              std::nullopt};
    return 单项失败(s, g);
  }
  static 因果单项结果 提交后失败(std::uint64_t g,
                                 std::uint64_t read = 0) noexcept {
    return {
        {因果数据状态::内部不一致, 1, read, 0, g, 因果发布阶段::已提交待读回},
        std::nullopt};
  }
  static 因果证据结果 证据提交失败(因果数据状态 s, std::uint64_t g) noexcept {
    if (s == 因果数据状态::已可能发布)
      return {{s, 1, g, 0, g ? std::optional<std::uint64_t>{g} : std::nullopt,
               因果发布阶段::未知},
              {}};
    return 证据失败(s, g);
  }
  static 因果证据结果 证据提交后失败(std::uint64_t g,
                                     std::uint64_t read = 0) noexcept {
    return {
        {因果数据状态::内部不一致, 1, read, 0, g, 因果发布阶段::已提交待读回},
        {}};
  }

  bool 布局浅层有效() const noexcept {
    const 稳定编码 ids[]{layout_.格式锚点,         layout_.字段登记关系类型,
                         layout_.因果族锚点,       layout_.族归属关系类型,
                         layout_.定义属性类型,     layout_.参与者EC关系类型,
                         layout_.条件FC关系类型,   layout_.结果FC关系类型,
                         layout_.参数FC关系类型,   layout_.约束RC关系类型,
                         layout_.动作DC关系类型,   layout_.原因果关系类型,
                         layout_.来源链关系类型,   layout_.证据索引属性类型,
                         layout_.证据锚点关系类型, layout_.证据存在关系类型,
                         layout_.证据绑定关系类型};
    for (std::size_t i = 0; i < 17; ++i) {
      if (!有效(ids[i]))
        return false;
      for (std::size_t j = 0; j < i; ++j)
        if (ids[i] == ids[j])
          return false;
    }
    return true;
  }
  bool 是交付元节点(稳定编码 id) const noexcept {
    const 稳定编码 ids[]{layout_.格式锚点,         layout_.字段登记关系类型,
                         layout_.因果族锚点,       layout_.族归属关系类型,
                         layout_.定义属性类型,     layout_.参与者EC关系类型,
                         layout_.条件FC关系类型,   layout_.结果FC关系类型,
                         layout_.参数FC关系类型,   layout_.约束RC关系类型,
                         layout_.动作DC关系类型,   layout_.原因果关系类型,
                         layout_.来源链关系类型,   layout_.证据索引属性类型,
                         layout_.证据锚点关系类型, layout_.证据存在关系类型,
                         layout_.证据绑定关系类型};
    return std::find(std::begin(ids), std::end(ids), id) != std::end(ids);
  }
  bool 节点类型有效(稳定编码 id, 节点种类 kind,
                    std::optional<L1所有者范围值表示种类> repr) const {
    const auto q = l1_.读取所有者范围当前节点({L1所有者范围CRUD合同版本, id});
    const auto *n =
        q.事实 ? std::get_if<L1所有者范围节点事实>(&*q.事实) : nullptr;
    return q.状态 == L1所有者范围读取状态::成功 && n && n->编码 == id &&
           n->写入所有者 == owner_ && n->种类 == kind &&
           n->属性类型表示 == repr && !n->退出事实代次;
  }
  bool 布局材料有效() const {
    const 稳定编码 plain[]{layout_.格式锚点,         layout_.字段登记关系类型,
                           layout_.因果族锚点,       layout_.族归属关系类型,
                           layout_.参与者EC关系类型, layout_.条件FC关系类型,
                           layout_.结果FC关系类型,   layout_.参数FC关系类型,
                           layout_.约束RC关系类型,   layout_.动作DC关系类型,
                           layout_.原因果关系类型,   layout_.来源链关系类型,
                           layout_.证据锚点关系类型, layout_.证据存在关系类型,
                           layout_.证据绑定关系类型};
    for (auto id : plain)
      if (!节点类型有效(id, 节点种类::普通, std::nullopt))
        return false;
    if (!节点类型有效(layout_.定义属性类型, 节点种类::属性类型,
                      L1所有者范围值表示种类::U64组) ||
        !节点类型有效(layout_.证据索引属性类型, 节点种类::属性类型,
                      L1所有者范围值表示种类::U64组))
      return false;
    const auto q = l1_.读取所有者范围当前源关系组(
        {L1所有者范围CRUD合同版本, layout_.格式锚点, layout_.字段登记关系类型});
    if (q.状态 != L1所有者范围读取状态::成功 || q.关系组.size() != 15)
      return false;
    const 稳定编码 expected[]{
        layout_.因果族锚点,       layout_.族归属关系类型,
        layout_.定义属性类型,     layout_.参与者EC关系类型,
        layout_.条件FC关系类型,   layout_.结果FC关系类型,
        layout_.参数FC关系类型,   layout_.约束RC关系类型,
        layout_.动作DC关系类型,   layout_.原因果关系类型,
        layout_.来源链关系类型,   layout_.证据索引属性类型,
        layout_.证据锚点关系类型, layout_.证据存在关系类型,
        layout_.证据绑定关系类型};
    for (std::size_t i = 0; i < 15; ++i) {
      std::size_t count = 0;
      for (const auto &e : q.关系组)
        if (e.角色或顺序 == static_cast<std::int64_t>(i + 1)) {
          if (e.写入所有者 != owner_ || e.源节点 != layout_.格式锚点 ||
              e.关系类型节点 != layout_.字段登记关系类型 ||
              e.目标节点 != expected[i] || e.退出事实代次)
            return false;
          ++count;
        }
      if (count != 1)
        return false;
    }
    return true;
  }

  static std::optional<稳定编码> 映射编码(const L1所有者范围写入结果 &r,
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
  bool 写入头完整(const L1所有者范围写入结果 &r, L1所有者范围写入幂等身份 key,
                  std::uint64_t expected) const noexcept {
    const bool first = r.状态 == L1所有者范围写入状态::成功 &&
                       r.是否形成内存权威发布 &&
                       r.重试边界 == L1所有者范围重试边界::不适用;
    const bool replay = r.状态 == L1所有者范围写入状态::精确重复 &&
                        !r.是否形成内存权威发布 &&
                        r.重试边界 == L1所有者范围重试边界::原幂等身份读回收敛;
    return expected != std::numeric_limits<std::uint64_t>::max() &&
           r.合同版本 == L1所有者范围CRUD合同版本 && r.所有者 == owner_ &&
           r.写入幂等身份 == key && r.事实代次 == expected + 1 &&
           (first || replay);
  }

  static 因果数据状态 映射读取(L1所有者范围读取状态 s) noexcept {
    switch (s) {
    case L1所有者范围读取状态::未找到:
      return 因果数据状态::未找到;
    case L1所有者范围读取状态::已退出:
      return 因果数据状态::目标已退出;
    case L1所有者范围读取状态::历史材料已清理:
      return 因果数据状态::历史材料已清理;
    case L1所有者范围读取状态::事实代次漂移:
      return 因果数据状态::事实代次漂移;
    case L1所有者范围读取状态::资源失败:
      return 因果数据状态::资源失败;
    default:
      return 因果数据状态::内部不一致;
    }
  }
  static 因果数据状态 映射写入(L1所有者范围写入状态 s,
                               因果数据状态 ok) noexcept {
    switch (s) {
    case L1所有者范围写入状态::成功:
      return ok;
    case L1所有者范围写入状态::精确重复:
      return 因果数据状态::精确重复;
    case L1所有者范围写入状态::事实代次漂移:
      return 因果数据状态::事实代次漂移;
    case L1所有者范围写入状态::幂等冲突:
      return 因果数据状态::幂等冲突;
    case L1所有者范围写入状态::引用冲突:
      return 因果数据状态::引用冲突;
    case L1所有者范围写入状态::资源失败:
      return 因果数据状态::资源失败;
    case L1所有者范围写入状态::内部不一致:
      return 因果数据状态::已可能发布;
    default:
      return 因果数据状态::内部不一致;
    }
  }
  static 因果数据状态 映射来源(因果数据状态 s) noexcept {
    if (s == 因果数据状态::未找到 || s == 因果数据状态::目标已退出)
      return 因果数据状态::来源不足;
    return s;
  }
  static 因果数据状态 映射动态(动态数据状态 s) noexcept {
    switch (s) {
    case 动态数据状态::依赖未实现:
      return 因果数据状态::依赖未实现;
    case 动态数据状态::未找到:
    case 动态数据状态::目标已退出:
    case 动态数据状态::来源未找到:
    case 动态数据状态::来源已退出:
      return 因果数据状态::来源不足;
    case 动态数据状态::历史材料已清理:
      return 因果数据状态::历史材料已清理;
    case 动态数据状态::事实代次漂移:
      return 因果数据状态::事实代次漂移;
    case 动态数据状态::数量预算不足:
      return 因果数据状态::数量预算不足;
    case 动态数据状态::资源失败:
      return 因果数据状态::资源失败;
    default:
      return 因果数据状态::内部不一致;
    }
  }
  static 因果数据状态 映射绑定(状态使用绑定数据状态 s) noexcept {
    switch (s) {
    case 状态使用绑定数据状态::未实现:
      return 因果数据状态::依赖未实现;
    case 状态使用绑定数据状态::未找到:
    case 状态使用绑定数据状态::目标已退出:
    case 状态使用绑定数据状态::场景未找到:
    case 状态使用绑定数据状态::场景已退出:
    case 状态使用绑定数据状态::存在未找到:
    case 状态使用绑定数据状态::存在已退出:
    case 状态使用绑定数据状态::状态未找到:
    case 状态使用绑定数据状态::状态已退出:
      return 因果数据状态::来源不足;
    case 状态使用绑定数据状态::历史材料已清理:
      return 因果数据状态::历史材料已清理;
    case 状态使用绑定数据状态::事实代次漂移:
      return 因果数据状态::事实代次漂移;
    case 状态使用绑定数据状态::数量预算不足:
      return 因果数据状态::数量预算不足;
    case 状态使用绑定数据状态::资源失败:
      return 因果数据状态::资源失败;
    default:
      return 因果数据状态::内部不一致;
    }
  }
  static 因果数据状态 映射概念(因果数据状态 s) noexcept {
    switch (s) {
    case 因果数据状态::依赖未实现:
      return s;
    case 因果数据状态::概念不相容:
      return s;
    case 因果数据状态::约束不支持:
      return s;
    case 因果数据状态::历史材料已清理:
      return s;
    case 因果数据状态::事实代次漂移:
      return s;
    case 因果数据状态::数量预算不足:
      return s;
    case 因果数据状态::资源失败:
      return s;
    default:
      return 因果数据状态::内部不一致;
    }
  }

  // 后续私有实现保持在同一模块，所有失败都向公开服务边界送出结构化状态。
  static 因果数据状态 规范化并核验(因果定义 &);
  void 编码定义(const 因果定义 &, 编码结果 &) const;
  bool 解码定义(const std::vector<std::uint64_t> &,
                const std::map<std::uint64_t, 引用槽> &, 因果定义 &,
                因果数据状态 *) const;
  因果数据状态 核验概念回显(const 因果概念核验结果 &, const 因果定义 &,
                            const 因果读取预算 &, std::uint64_t) const;
  因果组结果 查询同义(std::uint64_t, std::uint64_t, const 因果定义 &,
                       const 因果读取预算 &, 因果读取预算 * = nullptr) const;
  因果单项结果 读取历史(const 因果历史读取请求 &,
                       std::optional<std::uint64_t>, 因果读取预算 * = nullptr,
                       bool = false, const std::set<std::uint64_t> * = nullptr,
                       std::optional<稳定编码> = std::nullopt,
                       std::optional<稳定编码> = std::nullopt) const;
  因果身份历史结果 读取身份(const 因果历史身份请求 &) const;
  L1所有者范围写集请求 形成发布写集(const 因果发布请求 &,
                                    const 编码结果 &) const;
  std::optional<因果单项结果> 重放发布(const 因果发布请求 &, const 因果定义 &);
  std::optional<因果单项结果> 重放退出(const 因果退出请求 &);
  std::optional<因果证据结果> 重放证据关联(const 因果证据关联请求 &);
  std::optional<因果证据结果> 重放证据退出(const 因果证据退出请求 &);
  因果数据状态 读取证据索引(因果信息身份, std::uint64_t, std::uint64_t,
                            证据索引 &) const;
  static void 规范化证据(因果证据提交 &);
  static bool 添加证据索引项(证据索引 &, const 因果证据提交 &);
  因果数据状态 核验证据来源(const 因果定义 &, const 因果证据提交 &,
                             const 因果概念核验结果 &, std::uint64_t,
                             因果读取预算 &, 来源读取缓存 &, bool = true) const;
  L1所有者范围写集请求 形成证据关联写集(const 因果证据关联请求 &,
                                        const 证据索引 &) const;
  L1所有者范围写集请求 形成证据退出写集(const 因果证据退出请求 &,
                                        const 证据索引 &,
                                        const 证据索引项 &) const;
  L1所有者范围写集请求 形成因果退出写集(const 因果退出请求 &,
                                        const 因果内容事实 &,
                                        const 证据索引 &) const;
  static 因果证据事实 投影证据事实(因果信息身份, const 证据索引项 &,
                                   std::uint64_t, std::uint64_t,
                                   std::optional<std::uint64_t> = {});

  L1事实基座服务 &l1_;
  L1所有者范围写端口 port_;
  L1结构所有者身份 owner_{};
  因果结构交付 layout_;
  const 因果概念核验提供者 &concepts_;
  const 动态结构只读提供者 &dynamics_;
  const 状态使用绑定只读提供者 &bindings_;
  const 状态类数据服务 &states_;
};

} // namespace 海中鱼巣

namespace 海中鱼巣 {
namespace 因果实现细节 {
template <class E> bool enum有效(E v, std::uint64_t max) noexcept {
  const auto x = static_cast<std::uint64_t>(v);
  return x && x <= max;
}
struct 并查集 {
  std::vector<std::size_t> p;
  explicit 并查集(std::size_t n) : p(n + 1) {
    for (std::size_t i = 0; i < p.size(); ++i)
      p[i] = i;
  }
  std::size_t 根(std::size_t x) { return p[x] == x ? x : p[x] = 根(p[x]); }
  void 合并(std::size_t a, std::size_t b) { p[根(a)] = 根(b); }
};
inline std::uint64_t i64位(std::int64_t v) noexcept {
  return std::bit_cast<std::uint64_t>(v);
}
inline std::int64_t 位i64(std::uint64_t v) noexcept {
  return std::bit_cast<std::int64_t>(v);
}
} // namespace 因果实现细节

因果数据状态 因果类数据服务::规范化并核验(因果定义 &d) {
  using namespace 因果实现细节;
  if (!enum有效(d.类型, 2) || d.参与者.角色组.empty() || d.条件组.empty() ||
      d.结果组.empty() || d.参与者.角色组.size() > UINT32_MAX ||
      d.条件组.size() > UINT32_MAX || d.结果组.size() > UINT32_MAX)
    return 因果数据状态::入口拒绝;
  if (d.类型 == 因果类型::主动) {
    if (!d.动作)
      return 因果数据状态::入口拒绝;
  } else if (d.动作)
    return 因果数据状态::入口拒绝;
  std::sort(d.参与者.角色组.begin(), d.参与者.角色组.end(),
            [](const auto &a, const auto &b) { return a.角色 < b.角色; });
  for (std::size_t i = 0; i < d.参与者.角色组.size(); ++i)
    if (d.参与者.角色组[i].角色 != i + 1 || !有效(d.参与者.角色组[i].存在概念))
      return 因果数据状态::角色冲突;
  const auto R = d.参与者.角色组.size();
  并查集 uf(R);
  for (auto &v : d.参与者.同异约束组) {
    if (!v.左角色 || !v.右角色 || v.左角色 > R || v.右角色 > R ||
        v.左角色 == v.右角色 || !enum有效(v.关系, 2))
      return 因果数据状态::角色冲突;
    if (v.左角色 > v.右角色)
      std::swap(v.左角色, v.右角色);
    if (v.关系 == 因果角色同异::同一)
      uf.合并(v.左角色, v.右角色);
  }
  std::sort(d.参与者.同异约束组.begin(), d.参与者.同异约束组.end(),
            [](const auto &a, const auto &b) {
              return std::tie(a.左角色, a.右角色, a.关系) <
                     std::tie(b.左角色, b.右角色, b.关系);
            });
  d.参与者.同异约束组.erase(
      std::unique(d.参与者.同异约束组.begin(), d.参与者.同异约束组.end()),
      d.参与者.同异约束组.end());
  for (const auto &v : d.参与者.同异约束组)
    if (v.关系 == 因果角色同异::不同 && uf.根(v.左角色) == uf.根(v.右角色))
      return 因果数据状态::角色冲突;
  auto constraint = [&](const 因果条件约束 &v, std::uint32_t role,
                        bool parameter) -> 因果数据状态 {
    if (v.valueless_by_exception())
      return 因果数据状态::入口拒绝;
    if (const auto *x = std::get_if<因果准确值约束>(&v)) {
      if (!浅层结构有效(x->值))
        return 因果数据状态::入口拒绝;
      if (std::holds_alternative<特征值身份>(x->值))
        return 因果数据状态::约束不支持;
    } else if (const auto *x = std::get_if<因果I64区间约束>(&v)) {
      if (x->下界 > x->上界)
        return 因果数据状态::入口拒绝;
    } else if (const auto *x = std::get_if<因果二元关系约束>(&v)) {
      if (parameter)
        return 因果数据状态::约束不支持;
      if (!有效(x->关系概念) || x->被描述角色A != role || !x->参照角色B ||
          x->参照角色B > R)
        return 因果数据状态::角色冲突;
    }
    return 因果数据状态::已读取;
  };
  std::sort(d.条件组.begin(), d.条件组.end(),
            [](const auto &a, const auto &b) { return a.序号 < b.序号; });
  for (std::size_t i = 0; i < d.条件组.size(); ++i) {
    const auto &v = d.条件组[i];
    if (v.序号 != i + 1 || !v.参与者角色 || v.参与者角色 > R ||
        !有效(v.特征概念))
      return 因果数据状态::角色冲突;
    const auto s = constraint(v.约束, v.参与者角色, false);
    if (s != 因果数据状态::已读取)
      return s;
    for (std::size_t j = 0; j < i; ++j)
      if (v.参与者角色 == d.条件组[j].参与者角色 &&
          v.特征概念 == d.条件组[j].特征概念 &&
          v.约束 == d.条件组[j].约束)
        return 因果数据状态::入口拒绝;
  }
  if (d.动作) {
    auto &a = *d.动作;
    if (!有效(a.动作概念) || !a.执行者角色 || a.执行者角色 > R ||
        (a.作用对象角色 && (!*a.作用对象角色 || *a.作用对象角色 > R)))
      return 因果数据状态::角色冲突;
    std::sort(
        a.参数组.begin(), a.参数组.end(),
        [](const auto &x, const auto &y) { return x.参数角色 < y.参数角色; });
    for (std::size_t i = 0; i < a.参数组.size(); ++i) {
      const auto &v = a.参数组[i];
      if (!v.参数角色 || !有效(v.特征概念) ||
          (i && a.参数组[i - 1].参数角色 == v.参数角色))
        return 因果数据状态::角色冲突;
      const auto s = constraint(v.约束, v.参数角色, true);
      if (s != 因果数据状态::已读取)
        return s;
    }
  }
  std::sort(d.结果组.begin(), d.结果组.end(),
            [](const auto &a, const auto &b) { return a.序号 < b.序号; });
  for (std::size_t i = 0; i < d.结果组.size(); ++i) {
    const auto &v = d.结果组[i];
    if (v.序号 != i + 1 || !v.承担者角色 || v.承担者角色 > R ||
        !有效(v.特征概念) || v.约束.valueless_by_exception())
      return 因果数据状态::角色冲突;
    if (const auto *x = std::get_if<因果准确值约束>(&v.约束)) {
      if (!浅层结构有效(x->值))
        return 因果数据状态::入口拒绝;
      if (std::holds_alternative<特征值身份>(x->值))
        return 因果数据状态::约束不支持;
    } else if (const auto *x = std::get_if<因果I64区间约束>(&v.约束)) {
      if (x->下界 > x->上界)
        return 因果数据状态::入口拒绝;
    } else if (const auto *x = std::get_if<因果相对结果约束>(&v.约束)) {
      if (!x->基准条件序号 || x->基准条件序号 > d.条件组.size() ||
          !有效(x->变化关系概念))
        return 因果数据状态::概念不相容;
    }
    if (v.时限 && v.时限->最迟形成纳秒 < 0)
      return 因果数据状态::时序冲突;
    for (std::size_t j = 0; j < i; ++j)
      if (v.承担者角色 == d.结果组[j].承担者角色 &&
          v.特征概念 == d.结果组[j].特征概念 &&
          v.约束 == d.结果组[j].约束 && v.时限 == d.结果组[j].时限)
        return 因果数据状态::入口拒绝;
  }
  auto endpoint = [&](const 因果时序端 &e) -> bool {
    if (!enum有效(e.类别, 4) || !e.序号)
      return false;
    if (e.类别 == 因果时序端类别::条件)
      return e.序号 <= d.条件组.size();
    if (e.类别 == 因果时序端类别::结果)
      return e.序号 <= d.结果组.size();
    return d.类型 == 因果类型::主动 && e.序号 == 1;
  };
  std::vector<因果相对时序项> all = d.时序组;
  for (const auto &r : d.结果组)
    if (r.时限)
      all.push_back({r.时限->起点,
                     {因果时序端类别::结果, r.序号},
                     0,
                     r.时限->最迟形成纳秒});
  for (const auto &v : all)
    if (!endpoint(v.前端) || !endpoint(v.后端) || v.前端 == v.后端 ||
        v.最小间隔纳秒 < 0 || v.最大间隔纳秒 < v.最小间隔纳秒)
      return 因果数据状态::时序冲突;
  std::sort(d.时序组.begin(), d.时序组.end(), [](const auto &a, const auto &b) {
    return std::tie(a.前端.类别, a.前端.序号, a.后端.类别, a.后端.序号,
                    a.最小间隔纳秒, a.最大间隔纳秒) <
           std::tie(b.前端.类别, b.前端.序号, b.后端.类别, b.后端.序号,
                    b.最小间隔纳秒, b.最大间隔纳秒);
  });
  d.时序组.erase(std::unique(d.时序组.begin(), d.时序组.end()), d.时序组.end());
  std::map<std::pair<unsigned, unsigned>, std::size_t> nodes;
  auto node = [&](const 因果时序端 &e) {
    auto key = std::make_pair(static_cast<unsigned>(e.类别), e.序号);
    auto [it, _] = nodes.emplace(key, nodes.size());
    return it->second;
  };
  for (const auto &v : all) {
    node(v.前端);
    node(v.后端);
  }
  std::vector<std::tuple<std::size_t, std::size_t, std::int64_t>> edges;
  for (const auto &v : all) {
    const auto a = node(v.前端), b = node(v.后端);
    edges.push_back({a, b, v.最大间隔纳秒});
    edges.push_back({b, a, -v.最小间隔纳秒});
  }
  std::vector<std::int64_t> dist(nodes.size());
  for (std::size_t i = 0; i < nodes.size(); ++i) {
    bool changed = false;
    for (const auto &[a, b, w] : edges)
      if (dist[b] > dist[a] + w) {
        dist[b] = dist[a] + w;
        changed = true;
        if (i + 1 == nodes.size())
          return 因果数据状态::时序冲突;
      }
    if (!changed)
      break;
  }
  auto sourceKey = [](const 因果定义来源项 &v) {
    const auto tag = v.目标.index();
    const auto id = std::visit([](const auto &x) { return x.编码.值; }, v.目标);
    return std::tuple{static_cast<unsigned>(v.用途), tag, id};
  };
  for (const auto &v : d.来源组)
    if (!enum有效(v.用途, 2) || v.目标.valueless_by_exception() ||
        !std::visit([](const auto &x) { return 有效(x); }, v.目标))
      return 因果数据状态::入口拒绝;
  std::sort(d.来源组.begin(), d.来源组.end(),
            [&](const auto &a, const auto &b) {
              return sourceKey(a) < sourceKey(b);
            });
  d.来源组.erase(std::unique(d.来源组.begin(), d.来源组.end()), d.来源组.end());
  return 因果数据状态::已读取;
}

void 因果类数据服务::编码定义(const 因果定义 &d, 编码结果 &o) const {
  using namespace 因果实现细节;
  std::vector<std::uint64_t> b;
  auto u = [&](std::uint64_t x) { b.push_back(x); };
  auto i = [&](std::int64_t x) { b.push_back(i64位(x)); };
  auto ref = [&](稳定编码 x, 稳定编码 type) {
    o.引用组.push_back({x, type});
    u(o.引用组.size());
  };
  auto endpoint = [&](const 因果时序端 &x) {
    u(static_cast<unsigned>(x.类别));
    u(x.序号);
  };
  auto condition = [&](const 因果条件约束 &x) {
    u(x.index() + 1);
    std::visit(
        [&](const auto &v) {
          using T = std::decay_t<decltype(v)>;
          if constexpr (std::is_same_v<T, 因果准确值约束>) {
            if (const auto *p = std::get_if<std::int64_t>(&v.值)) {
              u(1);
              i(*p);
            } else {
              u(2);
              u(std::get<特征值身份>(v.值).编码.值);
            }
          } else if constexpr (std::is_same_v<T, 因果I64区间约束>) {
            i(v.下界);
            i(v.上界);
          } else if constexpr (std::is_same_v<T, 因果二元关系约束>) {
            ref(v.关系概念.编码, layout_.约束RC关系类型);
            u(v.被描述角色A);
            u(v.参照角色B);
          }
        },
        x);
  };
  auto result = [&](const 因果结果约束 &x) {
    u(x.index() + 1);
    std::visit(
        [&](const auto &v) {
          using T = std::decay_t<decltype(v)>;
          if constexpr (std::is_same_v<T, 因果准确值约束>) {
            if (const auto *p = std::get_if<std::int64_t>(&v.值)) {
              u(1);
              i(*p);
            } else {
              u(2);
              u(std::get<特征值身份>(v.值).编码.值);
            }
          } else if constexpr (std::is_same_v<T, 因果I64区间约束>) {
            i(v.下界);
            i(v.上界);
          } else if constexpr (std::is_same_v<T, 因果相对结果约束>) {
            u(v.基准条件序号);
            ref(v.变化关系概念.编码, layout_.约束RC关系类型);
          }
        },
        x);
  };
  u(static_cast<unsigned>(d.类型));
  u(d.参与者.角色组.size());
  for (const auto &v : d.参与者.角色组) {
    u(v.角色);
    ref(v.存在概念.编码, layout_.参与者EC关系类型);
  }
  u(d.参与者.同异约束组.size());
  for (const auto &v : d.参与者.同异约束组) {
    u(v.左角色);
    u(v.右角色);
    u(static_cast<unsigned>(v.关系));
  }
  u(d.条件组.size());
  for (const auto &v : d.条件组) {
    u(v.序号);
    u(v.参与者角色);
    ref(v.特征概念.编码, layout_.条件FC关系类型);
    condition(v.约束);
  }
  u(d.动作 ? 1 : 0);
  if (d.动作) {
    const auto &a = *d.动作;
    ref(a.动作概念.编码, layout_.动作DC关系类型);
    u(a.执行者角色);
    u(a.作用对象角色 ? 1 : 0);
    if (a.作用对象角色)
      u(*a.作用对象角色);
    u(a.参数组.size());
    for (const auto &v : a.参数组) {
      u(v.参数角色);
      ref(v.特征概念.编码, layout_.参数FC关系类型);
      condition(v.约束);
    }
  }
  u(d.结果组.size());
  for (const auto &v : d.结果组) {
    u(v.序号);
    u(v.承担者角色);
    ref(v.特征概念.编码, layout_.结果FC关系类型);
    result(v.约束);
    u(v.时限 ? 1 : 0);
    if (v.时限) {
      endpoint(v.时限->起点);
      i(v.时限->最迟形成纳秒);
    }
  }
  u(d.时序组.size());
  for (const auto &v : d.时序组) {
    endpoint(v.前端);
    endpoint(v.后端);
    i(v.最小间隔纳秒);
    i(v.最大间隔纳秒);
  }
  u(d.来源组.size());
  for (const auto &v : d.来源组) {
    u(static_cast<unsigned>(v.用途));
    u(v.目标.index() + 1);
    std::visit(
        [&](const auto &x) {
          using T = std::decay_t<decltype(x)>;
          if constexpr (std::is_same_v<T, 因果信息身份>)
            ref(x.编码, layout_.原因果关系类型);
          else
            ref(x.编码, layout_.来源链关系类型);
        },
        v.目标);
  }
  o.值 = {4340, 1, b.size()};
  o.值.insert(o.值.end(), b.begin(), b.end());
}

bool 因果类数据服务::解码定义(const std::vector<std::uint64_t> &v,
                               const std::map<std::uint64_t, 引用槽> &refs,
                               因果定义 &d, 因果数据状态 *failure) const {
  using namespace 因果实现细节;
  if (v.size() < 3 || v[0] != 4340 || v[1] != 1 || v[2] != v.size() - 3)
    return false;
  std::size_t p = 3;
  std::set<std::uint64_t> used;
  auto u = [&](std::uint64_t &x) {
    if (p >= v.size())
      return false;
    x = v[p++];
    return true;
  };
  auto u32 = [&](std::uint32_t &x) {
    std::uint64_t z;
    if (!u(z) || z > UINT32_MAX)
      return false;
    x = static_cast<std::uint32_t>(z);
    return true;
  };
  const auto room = [&](std::uint64_t count, std::size_t minimum) {
    return minimum && count <= (v.size() - p) / minimum;
  };
  auto i = [&](std::int64_t &x) {
    std::uint64_t z;
    if (!u(z))
      return false;
    x = 位i64(z);
    return true;
  };
  auto ref = [&](稳定编码 &typeTarget, 稳定编码 type) {
    std::uint64_t seq;
    if (!u(seq))
      return false;
    auto it = refs.find(seq);
    if (it == refs.end() || it->second.类型 != type || !used.insert(seq).second)
      return false;
    typeTarget = it->second.目标;
    return true;
  };
  auto endpoint = [&](因果时序端 &x) {
    std::uint64_t z;
    if (!u(z) || z < 1 || z > 4 || !u32(x.序号))
      return false;
    x.类别 = static_cast<因果时序端类别>(z);
    return true;
  };
  auto condition = [&](因果条件约束 &x) {
    std::uint64_t tag;
    if (!u(tag) || tag < 1 || tag > 4)
      return false;
    if (tag == 1)
      x = 因果概念域约束{};
    else if (tag == 2) {
      std::uint64_t kind;
      if (!u(kind))
        return false;
      因果准确值约束 a;
      if (kind == 1) {
        std::int64_t z;
        if (!i(z))
          return false;
        a.值 = z;
      } else if (kind == 2) {
        std::uint64_t z;
        if (!u(z))
          return false;
        a.值 = 特征值身份{{z}};
      } else
        return false;
      x = std::move(a);
    } else if (tag == 3) {
      因果I64区间约束 z;
      if (!i(z.下界) || !i(z.上界))
        return false;
      x = z;
    } else {
      因果二元关系约束 z;
      if (!ref(z.关系概念.编码, layout_.约束RC关系类型) ||
          !u32(z.被描述角色A) || !u32(z.参照角色B))
        return false;
      x = z;
    }
    return true;
  };
  auto result = [&](因果结果约束 &x) {
    std::uint64_t tag;
    if (!u(tag) || tag < 1 || tag > 4)
      return false;
    if (tag == 1)
      x = 因果概念域约束{};
    else if (tag == 2) {
      std::uint64_t kind;
      if (!u(kind))
        return false;
      因果准确值约束 a;
      if (kind == 1) {
        std::int64_t z;
        if (!i(z))
          return false;
        a.值 = z;
      } else if (kind == 2) {
        std::uint64_t z;
        if (!u(z))
          return false;
        a.值 = 特征值身份{{z}};
      } else
        return false;
      x = std::move(a);
    } else if (tag == 3) {
      因果I64区间约束 z;
      if (!i(z.下界) || !i(z.上界))
        return false;
      x = z;
    } else {
      因果相对结果约束 z;
      if (!u32(z.基准条件序号) ||
          !ref(z.变化关系概念.编码, layout_.约束RC关系类型))
        return false;
      x = z;
    }
    return true;
  };
  std::uint64_t z;
  if (!u(z) || z < 1 || z > 2)
    return false;
  d.类型 = static_cast<因果类型>(z);
  if (!u(z) || z > UINT32_MAX || !room(z, 2))
    return false;
  d.参与者.角色组.resize(static_cast<std::size_t>(z));
  for (auto &x : d.参与者.角色组)
    if (!u32(x.角色) || !ref(x.存在概念.编码, layout_.参与者EC关系类型))
      return false;
  if (!u(z) || z > UINT32_MAX || !room(z, 3))
    return false;
  d.参与者.同异约束组.resize(static_cast<std::size_t>(z));
  for (auto &x : d.参与者.同异约束组) {
    if (!u32(x.左角色) || !u32(x.右角色) || !u(z) || z < 1 || z > 2)
      return false;
    x.关系 = static_cast<因果角色同异>(z);
  }
  if (!u(z) || z > UINT32_MAX || !room(z, 4))
    return false;
  d.条件组.resize(static_cast<std::size_t>(z));
  for (auto &x : d.条件组)
    if (!u32(x.序号) || !u32(x.参与者角色) ||
        !ref(x.特征概念.编码, layout_.条件FC关系类型) || !condition(x.约束))
      return false;
  if (!u(z) || z > 1)
    return false;
  if (z) {
    d.动作.emplace();
    auto &a = *d.动作;
    if (!ref(a.动作概念.编码, layout_.动作DC关系类型) || !u32(a.执行者角色) ||
        !u(z) || z > 1)
      return false;
    if (z) {
      std::uint32_t role;
      if (!u32(role))
        return false;
      a.作用对象角色 = role;
    }
    if (!u(z) || z > UINT32_MAX || !room(z, 4))
      return false;
    a.参数组.resize(static_cast<std::size_t>(z));
    for (auto &x : a.参数组)
      if (!u32(x.参数角色) || !ref(x.特征概念.编码, layout_.参数FC关系类型) ||
          !condition(x.约束))
        return false;
  }
  if (!u(z) || z > UINT32_MAX || !room(z, 5))
    return false;
  d.结果组.resize(static_cast<std::size_t>(z));
  for (auto &x : d.结果组) {
    if (!u32(x.序号) || !u32(x.承担者角色) ||
        !ref(x.特征概念.编码, layout_.结果FC关系类型) || !result(x.约束) ||
        !u(z) || z > 1)
      return false;
    if (z) {
      x.时限.emplace();
      if (!endpoint(x.时限->起点) || !i(x.时限->最迟形成纳秒))
        return false;
    }
  }
  if (!u(z) || z > UINT32_MAX || !room(z, 6))
    return false;
  d.时序组.resize(static_cast<std::size_t>(z));
  for (auto &x : d.时序组)
    if (!endpoint(x.前端) || !endpoint(x.后端) || !i(x.最小间隔纳秒) ||
        !i(x.最大间隔纳秒))
      return false;
  if (!u(z) || z > UINT32_MAX || !room(z, 3))
    return false;
  d.来源组.resize(static_cast<std::size_t>(z));
  for (auto &x : d.来源组) {
    if (!u(z) || z < 1 || z > 2)
      return false;
    x.用途 = static_cast<因果来源用途>(z);
    if (!u(z) || z < 1 || z > 2)
      return false;
    if (z == 1) {
      因果信息身份 q;
      if (!ref(q.编码, layout_.原因果关系类型))
        return false;
      x.目标 = q;
    } else {
      因果链概念引用 c;
      if (!ref(c.编码, layout_.来源链关系类型))
        return false;
      x.目标 = c;
    }
  }
  if (p != v.size() || used.size() != refs.size())
    return false;
  const auto normalized = 规范化并核验(d);
  if (normalized == 因果数据状态::已读取)
    return true;
  if (failure && normalized == 因果数据状态::约束不支持) {
    const auto hidden = [](const auto &constraint) {
      const auto *exact = std::get_if<因果准确值约束>(&constraint);
      return exact && std::holds_alternative<特征值身份>(exact->值);
    };
    bool hasHidden = false;
    for (const auto &x : d.条件组)
      hasHidden = hasHidden || hidden(x.约束);
    if (d.动作)
      for (const auto &x : d.动作->参数组)
        hasHidden = hasHidden || hidden(x.约束);
    for (const auto &x : d.结果组)
      hasHidden = hasHidden || hidden(x.约束);
    if (hasHidden)
      *failure = 因果数据状态::旧格式不支持;
  }
  return false;
}

因果数据状态 因果类数据服务::核验概念回显(const 因果概念核验结果 &r,
                                          const 因果定义 &d,
                                          const 因果读取预算 &budget,
                                          std::uint64_t g) const {
  if (!r.成功()) {
    if (r.已核验定义 || !r.概念组.empty())
      return 因果数据状态::内部不一致;
    return 映射概念(r.操作.状态);
  }
  if (r.操作.Gread != g || r.操作.H != g || *r.已核验定义 != d ||
      r.已读取关系数 > budget.最大关系数 ||
      r.已读取值元素数 > budget.最大值元素数 ||
      r.已读取材料数 > budget.最大来源材料数)
    return 因果数据状态::内部不一致;
  编码结果 encoded;
  编码定义(d, encoded);
  std::vector<std::pair<稳定编码, 因果概念类别>> expected;
  for (const auto &slot : encoded.引用组) {
    if (slot.类型 == layout_.原因果关系类型)
      continue;
    因果概念类别 kind;
    if (slot.类型 == layout_.参与者EC关系类型)
      kind = 因果概念类别::存在;
    else if (slot.类型 == layout_.条件FC关系类型 ||
             slot.类型 == layout_.结果FC关系类型 ||
             slot.类型 == layout_.参数FC关系类型)
      kind = 因果概念类别::特征域;
    else if (slot.类型 == layout_.约束RC关系类型)
      kind = 因果概念类别::二次关系;
    else if (slot.类型 == layout_.动作DC关系类型)
      kind = 因果概念类别::动作;
    else if (slot.类型 == layout_.来源链关系类型)
      kind = 因果概念类别::因果链;
    else
      return 因果数据状态::内部不一致;
    expected.push_back({slot.目标, kind});
  }
  if (r.概念组.size() != expected.size())
    return 因果数据状态::内部不一致;
  for (std::size_t i = 0; i < expected.size(); ++i)
    if (r.概念组[i].引用序号 != i + 1 ||
        r.概念组[i].概念 != expected[i].first ||
        r.概念组[i].类别 != expected[i].second)
      return 因果数据状态::内部不一致;
  return 因果数据状态::已读取;
}

L1所有者范围写集请求 因果类数据服务::形成发布写集(const 因果发布请求 &r,
                                                  const 编码结果 &e) const {
  L1所有者范围写集请求 w{L1所有者范围CRUD合同版本, r.G0, r.幂等身份};
  w.节点 = {{节点键, 节点种类::普通, std::nullopt}};
  w.关系 = {{族关系键, 节点键, layout_.因果族锚点, layout_.族归属关系类型, 1}};
  for (std::size_t i = 0; i < e.引用组.size(); ++i)
    w.关系.push_back({{static_cast<std::uint32_t>(5 + i)},
                      节点键,
                      e.引用组[i].目标,
                      e.引用组[i].类型,
                      static_cast<std::int64_t>(i + 1)});
  w.值 = {{定义值键, 节点键, layout_.定义属性类型, e.值, 节点键},
          {证据索引值键, 节点键, layout_.证据索引属性类型,
           std::vector<std::uint64_t>{4341, 1, 0, 0}, 节点键}};
  w.属性槽变更 = {{节点键, layout_.定义属性类型, 定义值键},
                  {节点键, layout_.证据索引属性类型, 证据索引值键}};
  return w;
}

因果单项结果
因果类数据服务::读取历史(const 因果历史读取请求 &r,
                         std::optional<std::uint64_t> expectedExit,
                         因果读取预算 *remainingOut,
                         bool familyRelationAlreadyCounted,
                         const std::set<std::uint64_t> *countedDefinitionRelations,
                         std::optional<稳定编码> countedDefinitionType,
                         std::optional<稳定编码> countedDefinitionTarget) const {
  if (是交付元节点(r.身份.编码))
    return 单项失败(因果数据状态::未找到, r.Gread, r.H);
  const auto raw =
      l1_.读取所有者范围历史事实({L1所有者范围CRUD合同版本, r.身份.编码});
  if (raw.合同版本 != L1所有者范围CRUD合同版本 || raw.查询编码 != r.身份.编码)
    return 单项失败(因果数据状态::内部不一致, raw.读取事实代次, r.H);
  if (raw.状态 != L1所有者范围读取状态::成功)
    return 单项失败(映射读取(raw.状态), raw.读取事实代次, r.H);
  if (raw.读取事实代次 != r.Gread)
    return 单项失败(因果数据状态::事实代次漂移, raw.读取事实代次, r.H);
  const auto *n =
      raw.事实 ? std::get_if<L1所有者范围节点事实>(&*raw.事实) : nullptr;
  if (!n || n->编码 != r.身份.编码 || n->写入所有者 != owner_ ||
      n->种类 != 节点种类::普通 || n->属性类型表示)
    return 单项失败(因果数据状态::未找到, r.Gread, r.H);
  if (!n->创建事实代次)
    return 单项失败(因果数据状态::内部不一致, r.Gread, r.H);
  if (n->创建事实代次 > r.H)
    return 单项失败(因果数据状态::未找到, r.Gread, r.H);
  if (n->退出事实代次 && *n->退出事实代次 <= r.H)
    return 单项失败(因果数据状态::目标已退出, r.Gread, r.H);
  if (expectedExit && n->退出事实代次 != expectedExit)
    return 单项失败(因果数据状态::内部不一致, r.Gread, r.H);
  const auto fq = l1_.读取所有者范围历史关系组(
      {L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::源, r.身份.编码,
       layout_.族归属关系类型, r.H});
  if (fq.状态 != L1所有者范围读取状态::成功 || fq.读取事实代次 != r.Gread)
    return 单项失败(fq.读取事实代次 != r.Gread ? 因果数据状态::事实代次漂移
                                               : 映射读取(fq.状态),
                    fq.读取事实代次, r.H);
  std::vector<L1所有者范围关系事实> family;
  for (const auto &e : fq.关系组)
    if (e.写入所有者 == owner_)
      family.push_back(e);
    else
      return 单项失败(因果数据状态::内部不一致, r.Gread, r.H);
  if (family.empty())
    return 单项失败(因果数据状态::旧格式不支持, r.Gread, r.H);
  if (family.size() != 1 || family[0].目标节点 != layout_.因果族锚点 ||
      family[0].角色或顺序 != 1 || family[0].创建事实代次 != n->创建事实代次 ||
      family[0].退出事实代次 != n->退出事实代次)
    return 单项失败(因果数据状态::内部不一致, r.Gread, r.H);
  const auto attrs = l1_.读取所有者范围历史属性值组(
      {L1所有者范围CRUD合同版本, r.身份.编码, r.H});
  if (attrs.状态 != L1所有者范围读取状态::成功 || attrs.读取事实代次 != r.Gread)
    return 单项失败(attrs.读取事实代次 != r.Gread ? 因果数据状态::事实代次漂移
                                                  : 映射读取(attrs.状态),
                    attrs.读取事实代次, r.H);
  const L1所有者范围值事实 *definition = nullptr;
  const L1所有者范围值事实 *index = nullptr;
  for (const auto &v : attrs.属性值组) {
    if (v.写入所有者 != owner_)
      continue;
    if (v.属性类型节点 == layout_.定义属性类型) {
      if (definition)
        return 单项失败(因果数据状态::内部不一致, r.Gread, r.H);
      definition = &v;
    } else if (v.属性类型节点 == layout_.证据索引属性类型) {
      if (index)
        return 单项失败(因果数据状态::内部不一致, r.Gread, r.H);
      index = &v;
    } else
      return 单项失败(因果数据状态::内部不一致, r.Gread, r.H);
  }
  if (!definition || !index || definition->所属节点 != r.身份.编码 ||
      index->所属节点 != r.身份.编码 ||
      definition->创建事实代次 != n->创建事实代次 ||
      definition->退出事实代次 != n->退出事实代次 ||
      definition->来源节点 != r.身份.编码 || index->来源节点 != r.身份.编码)
    return 单项失败(因果数据状态::内部不一致, r.Gread, r.H);
  const auto *material =
      std::get_if<std::vector<std::uint64_t>>(&definition->材料);
  if (!material)
    return 单项失败(因果数据状态::内部不一致, r.Gread, r.H);
  const auto *indexMaterial =
      std::get_if<std::vector<std::uint64_t>>(&index->材料);
  if (!indexMaterial || indexMaterial->size() < 4 || (*indexMaterial)[0] != 4341 ||
      (*indexMaterial)[1] != 1)
    return 单项失败(因果数据状态::内部不一致, r.Gread, r.H);
  证据索引 parsedIndex;
  const auto indexStatus = 读取证据索引(r.身份, r.Gread, r.H, parsedIndex);
  if (indexStatus != 因果数据状态::已读取 || parsedIndex.当前值 != index->编码)
    return 单项失败(indexStatus == 因果数据状态::已读取
                          ? 因果数据状态::内部不一致
                          : indexStatus,
                      r.Gread, r.H);
  auto remaining = remainingOut ? *remainingOut : r.预算;
  if (!扣减证据索引预算(remaining, parsedIndex))
    return 单项失败(因果数据状态::数量预算不足, r.Gread, r.H);
  const 稳定编码 types[]{layout_.参与者EC关系类型, layout_.条件FC关系类型,
                         layout_.结果FC关系类型,   layout_.参数FC关系类型,
                         layout_.约束RC关系类型,   layout_.动作DC关系类型,
                         layout_.原因果关系类型,   layout_.来源链关系类型};
  std::map<std::uint64_t, 引用槽> refs;
  std::vector<std::pair<std::uint64_t, 稳定编码>> relationBySequence;
  std::map<std::uint64_t, std::pair<稳定编码, 稳定编码>> relationIdentityById;
  for (auto type : types) {
    const auto q = l1_.读取所有者范围历史关系组({L1所有者范围CRUD合同版本,
                                                 L1所有者范围关系端点方向::源,
                                                 r.身份.编码, type, r.H});
    if (q.状态 != L1所有者范围读取状态::成功 || q.读取事实代次 != r.Gread)
      return 单项失败(q.读取事实代次 != r.Gread ? 因果数据状态::事实代次漂移
                                                : 映射读取(q.状态),
                      q.读取事实代次, r.H);
    for (const auto &e : q.关系组) {
      if (e.写入所有者 != owner_ || e.源节点 != r.身份.编码 ||
          e.关系类型节点 != type || e.角色或顺序 <= 0 ||
          e.创建事实代次 != n->创建事实代次 ||
          e.退出事实代次 != n->退出事实代次 ||
          !refs.emplace(static_cast<std::uint64_t>(e.角色或顺序),
                        引用槽{e.目标节点, type})
               .second)
        return 单项失败(因果数据状态::内部不一致, r.Gread, r.H);
      relationBySequence.push_back(
          {static_cast<std::uint64_t>(e.角色或顺序), e.编码});
      if (!relationIdentityById.emplace(
               e.编码.值, std::pair{e.关系类型节点, e.目标节点})
               .second)
        return 单项失败(因果数据状态::内部不一致, r.Gread, r.H);
    }
  }
  const auto countedDefinitionCount =
      countedDefinitionRelations ? countedDefinitionRelations->size() : 0;
  if (countedDefinitionCount > relationBySequence.size())
    return 单项失败(因果数据状态::内部不一致, r.Gread, r.H);
  if (countedDefinitionRelations) {
    if (!countedDefinitionType || !countedDefinitionTarget)
      return 单项失败(因果数据状态::内部不一致, r.Gread, r.H);
    for (auto id : *countedDefinitionRelations) {
      const auto found = relationIdentityById.find(id);
      if (!id || found == relationIdentityById.end() ||
          found->second.first != *countedDefinitionType ||
          found->second.second != *countedDefinitionTarget)
        return 单项失败(因果数据状态::内部不一致, r.Gread, r.H);
    }
  } else if (countedDefinitionType || countedDefinitionTarget) {
    return 单项失败(因果数据状态::内部不一致, r.Gread, r.H);
  }
  const auto relationCount = refs.size() - countedDefinitionCount;
  if (relationCount > remaining.最大关系数 ||
      material->size() > remaining.最大值元素数 ||
      relationCount + 2 > remaining.最大来源材料数)
    return 单项失败(因果数据状态::数量预算不足, r.Gread, r.H);
  因果定义 d;
  auto decodeFailure = 因果数据状态::内部不一致;
  if (!解码定义(*material, refs, d, &decodeFailure))
    return 单项失败(decodeFailure, r.Gread, r.H);
  const auto definitionItems = 定义项总数(d);
  if (!definitionItems || *definitionItems > remaining.最大定义项数)
    return 单项失败(因果数据状态::数量预算不足, r.Gread, r.H);
  std::sort(relationBySequence.begin(), relationBySequence.end(),
            [](const auto &a, const auto &b) { return a.first < b.first; });
  std::vector<稳定编码> relationIds;
  relationIds.reserve(relationBySequence.size());
  for (const auto &[_, id] : relationBySequence)
    relationIds.push_back(id);
  因果内容事实 content{r.Gread,
                       r.H,
                       {r.身份, std::move(d)},
                       family[0].编码,
                       definition->编码,
                       std::move(relationIds),
                       n->创建事实代次,
                       n->退出事实代次};
  因果单项结果 out{读取头(r.Gread, r.H), std::move(content)};
  if (!out.成功())
    return 单项失败(因果数据状态::内部不一致, r.Gread, r.H);
  if (!扣减内容预算(remaining, *out.内容, familyRelationAlreadyCounted,
                  countedDefinitionCount))
    return 单项失败(因果数据状态::数量预算不足, r.Gread, r.H);
  if (remainingOut)
    *remainingOut = remaining;
  return out;
}

因果身份历史结果 因果类数据服务::读取身份(const 因果历史身份请求 &r) const {
  if (是交付元节点(r.身份.编码))
    return 身份失败(因果数据状态::未找到, r.Gread, r.H);
  const auto raw =
      l1_.读取所有者范围历史事实({L1所有者范围CRUD合同版本, r.身份.编码});
  if (raw.状态 != L1所有者范围读取状态::成功)
    return 身份失败(映射读取(raw.状态), raw.读取事实代次, r.H);
  if (raw.读取事实代次 != r.Gread)
    return 身份失败(因果数据状态::事实代次漂移, raw.读取事实代次, r.H);
  const auto *n =
      raw.事实 ? std::get_if<L1所有者范围节点事实>(&*raw.事实) : nullptr;
  if (!n || n->写入所有者 != owner_ || n->种类 != 节点种类::普通 ||
      n->属性类型表示)
    return 身份失败(因果数据状态::未找到, r.Gread, r.H);
  if (n->创建事实代次 > r.H)
    return 身份失败(因果数据状态::未找到, r.Gread, r.H);
  if (n->退出事实代次 && *n->退出事实代次 <= r.H)
    return 身份失败(因果数据状态::目标已退出, r.Gread, r.H);
  const auto q = l1_.读取所有者范围历史关系组(
      {L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::源, r.身份.编码,
       layout_.族归属关系类型, r.H});
  if (q.状态 != L1所有者范围读取状态::成功 || q.读取事实代次 != r.Gread)
    return 身份失败(q.读取事实代次 != r.Gread ? 因果数据状态::事实代次漂移
                                              : 映射读取(q.状态),
                    q.读取事实代次, r.H);
  if (q.关系组.empty())
    return 身份失败(因果数据状态::旧格式不支持, r.Gread, r.H);
  if (q.关系组.size() != 1)
    return 身份失败(因果数据状态::内部不一致, r.Gread, r.H);
  const auto &e = q.关系组.front();
  if (e.写入所有者 != owner_ || e.目标节点 != layout_.因果族锚点 ||
      e.角色或顺序 != 1 || e.创建事实代次 != n->创建事实代次 ||
      e.退出事实代次 != n->退出事实代次)
    return 身份失败(因果数据状态::内部不一致, r.Gread, r.H);
  因果身份历史结果 out{读取头(r.Gread, r.H),
                       因果身份历史事实{r.Gread, r.H, r.身份,
                                        layout_.因果族锚点,
                                        layout_.族归属关系类型, e.编码,
                                        n->创建事实代次, n->退出事实代次}};
  return out.成功() ? out : 身份失败(因果数据状态::内部不一致, r.Gread, r.H);
}

因果组结果 因果类数据服务::查询同义(std::uint64_t g, std::uint64_t h,
                                    const 因果定义 &d,
                                    const 因果读取预算 &budget,
                                    因果读取预算 *remainingOut) const {
  const auto guard = 读取当前代次();
  if (guard.状态 != 因果数据状态::已读取)
    return 组失败(guard.状态, guard.Gread, h);
  if (guard.Gread != g)
    return 组失败(因果数据状态::事实代次漂移, guard.Gread, h);
  const auto q = l1_.读取所有者范围历史关系组(
      {L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::目标,
       layout_.因果族锚点, layout_.族归属关系类型, h});
  if (q.状态 != L1所有者范围读取状态::成功)
    return 组失败(映射读取(q.状态), q.读取事实代次, h);
  if (q.读取事实代次 != g)
    return 组失败(因果数据状态::事实代次漂移, q.读取事实代次, h);
  auto remaining = budget;
  if (q.关系组.size() > remaining.最大候选数 ||
      q.关系组.size() > remaining.最大关系数 ||
      q.关系组.size() > remaining.最大来源材料数)
    return 组失败(因果数据状态::数量预算不足, g, h);
  remaining.最大候选数 -= q.关系组.size();
  remaining.最大关系数 -= q.关系组.size();
  remaining.最大来源材料数 -= q.关系组.size();
  因果组结果 out{读取头(g, h), {}};
  for (const auto &e : q.关系组) {
    if (e.写入所有者 != owner_ || e.目标节点 != layout_.因果族锚点 ||
        e.关系类型节点 != layout_.族归属关系类型)
      return 组失败(因果数据状态::内部不一致, g, h);
    auto one = 读取历史({1, g, h, {e.源节点}, remaining}, std::nullopt,
                       &remaining, true, nullptr);
    if (!one.成功())
      return 组失败(one.操作.状态, one.操作.Gread, h);
    if (one.内容->信息.定义 == d)
      out.因果组.push_back(*one.内容);
  }
  std::sort(out.因果组.begin(), out.因果组.end(),
            [](const auto &a, const auto &b) {
              return a.信息.身份.编码 < b.信息.身份.编码;
            });
  if (remainingOut)
    *remainingOut = remaining;
  return out;
}

std::optional<因果单项结果> 因果类数据服务::重放发布(const 因果发布请求 &r,
                                                     const 因果定义 &d) {
  const auto first =
      port_.读取首次写入材料({L1所有者范围首次写入读取合同版本, r.幂等身份});
  if (first.状态 == L1所有者范围读取状态::未找到) {
    if (!first.读取事实代次 || first.首次规范化写集 || first.首次写入结果)
      return 单项失败(因果数据状态::内部不一致, first.读取事实代次);
    return std::nullopt;
  }
  if (first.状态 != L1所有者范围读取状态::成功 || !first.首次规范化写集 ||
      !first.首次写入结果)
    return 单项失败(映射读取(first.状态), first.读取事实代次);
  编码结果 enc;
  编码定义(d, enc);
  const auto expected = 形成发布写集(r, enc);
  if (*first.首次规范化写集 != expected)
    return 单项失败(因果数据状态::幂等冲突, first.读取事实代次);
  const auto saved = port_.提交所有者范围中性写集(expected);
  if (saved.状态 != L1所有者范围写入状态::精确重复 ||
      !写入头完整(saved, r.幂等身份, r.G0))
    return 提交失败(映射写入(saved.状态, 因果数据状态::已创建), saved.事实代次);
  const auto id = 映射编码(saved, 节点键);
  if (!id)
    return 提交后失败(saved.事实代次);
  const auto guard = 读取当前代次();
  if (guard.状态 != 因果数据状态::已读取)
    return 提交后失败(saved.事实代次, guard.Gread);
  auto read =
      读取历史({1, guard.Gread, saved.事实代次, {*id}, r.预算}, std::nullopt);
  if (!read.成功())
    return 提交后失败(saved.事实代次, read.操作.Gread);
  return 因果单项结果{{因果数据状态::精确重复, 1, guard.Gread, saved.事实代次,
                       saved.事实代次, 因果发布阶段::已读回},
                      std::move(read.内容)};
}

void 因果类数据服务::规范化证据(因果证据提交 &v) {
  std::sort(v.角色绑定组.begin(), v.角色绑定组.end(),
            [](const auto &a, const auto &b) { return a.角色 < b.角色; });
  std::sort(v.状态证据组.begin(), v.状态证据组.end(),
            [](const auto &a, const auto &b) {
              return std::tie(a.类别, a.定义项序号) <
                     std::tie(b.类别, b.定义项序号);
            });
}

bool 因果类数据服务::添加证据索引项(证据索引 &index,
                                    const 因果证据提交 &value) {
  const std::uint64_t count =
      1 + value.角色绑定组.size() + value.状态证据组.size() * 2;
  if (count >
      static_cast<std::uint64_t>(std::numeric_limits<std::int64_t>::max()) -
          index.最大序号)
    return false;
  证据索引项 item;
  item.内容 = value;
  for (std::uint64_t i = 0; i < count; ++i)
    item.引用关系组.push_back({++index.最大序号, {}});
  index.项目组.push_back(std::move(item));
  std::sort(index.项目组.begin(), index.项目组.end(),
            [](const auto &a, const auto &b) {
              return a.内容.发生锚点.编码 < b.内容.发生锚点.编码;
            });
  return true;
}

因果数据状态 因果类数据服务::读取证据索引(因果信息身份 id, std::uint64_t g,
                                          std::uint64_t h,
                                          证据索引 &out) const {
  const auto attrs =
      l1_.读取所有者范围历史属性值组({L1所有者范围CRUD合同版本, id.编码, h});
  if (attrs.状态 != L1所有者范围读取状态::成功)
    return 映射读取(attrs.状态);
  if (attrs.读取事实代次 != g)
    return 因果数据状态::事实代次漂移;
  const L1所有者范围值事实 *indexValue = nullptr;
  for (const auto &v : attrs.属性值组) {
    if (v.写入所有者 == owner_ && v.属性类型节点 == layout_.证据索引属性类型) {
      if (indexValue)
        return 因果数据状态::内部不一致;
      indexValue = &v;
    }
  }
  if (!indexValue || indexValue->所属节点 != id.编码 ||
      indexValue->来源节点 != id.编码)
    return 因果数据状态::内部不一致;
  const auto *data = std::get_if<std::vector<std::uint64_t>>(&indexValue->材料);
  if (!data || data->size() < 4 || (*data)[0] != 4341 || (*data)[1] != 1)
    return 因果数据状态::内部不一致;
  struct R {
    稳定编码 target{}, type{}, relation{};
    std::uint64_t create = 0;
    std::optional<std::uint64_t> exit;
  };
  std::map<std::uint64_t, R> refs;
  const 稳定编码 types[]{layout_.证据锚点关系类型, layout_.证据存在关系类型,
                         layout_.证据绑定关系类型};
  for (auto type : types) {
    const auto q = l1_.读取所有者范围历史关系组({L1所有者范围CRUD合同版本,
                                                 L1所有者范围关系端点方向::源,
                                                 id.编码, type, h});
    if (q.状态 != L1所有者范围读取状态::成功)
      return 映射读取(q.状态);
    if (q.读取事实代次 != g)
      return 因果数据状态::事实代次漂移;
    for (const auto &e : q.关系组) {
      if (e.写入所有者 != owner_ || e.源节点 != id.编码 ||
          e.关系类型节点 != type || e.角色或顺序 <= 0 ||
          !refs.emplace(
                   static_cast<std::uint64_t>(e.角色或顺序),
                   R{e.目标节点, type, e.编码, e.创建事实代次, e.退出事实代次})
               .second)
        return 因果数据状态::内部不一致;
    }
  }
  std::size_t p = 2;
  auto u = [&](std::uint64_t &x) {
    if (p >= data->size())
      return false;
    x = (*data)[p++];
    return true;
  };
  std::uint64_t count = 0;
  if (!u(out.最大序号) || out.最大序号 >
                               static_cast<std::uint64_t>(
                                   std::numeric_limits<std::int64_t>::max()) ||
      !u(count) || count > UINT32_MAX || count > (data->size() - p) / 6)
    return 因果数据状态::内部不一致;
  out.当前值 = indexValue->编码;
  out.项目组.clear();
  out.项目组.reserve(static_cast<std::size_t>(count));
  std::set<std::uint64_t> used;
  for (std::uint64_t k = 0; k < count; ++k) {
    证据索引项 item;
    std::uint64_t seq = 0, n = 0, tag = 0;
    if (!u(seq) || !seq || seq > out.最大序号)
      return 因果数据状态::内部不一致;
    auto anchor = refs.find(seq);
    if (anchor == refs.end() ||
        anchor->second.type != layout_.证据锚点关系类型 ||
        !used.insert(seq).second)
      return 因果数据状态::内部不一致;
    item.内容.发生锚点 = {anchor->second.target};
    item.引用关系组.push_back({seq, anchor->second.relation});
    item.创建事实代次 = anchor->second.create;
    item.退出事实代次 = anchor->second.exit;
    if (!u(item.内容.证据截止) || !item.内容.证据截止 || !u(n) ||
        n > UINT32_MAX || n > (data->size() - p) / 2)
      return 因果数据状态::内部不一致;
    for (std::uint64_t j = 0; j < n; ++j) {
      因果证据角色绑定 x;
      std::uint64_t role;
      if (!u(role) || role > UINT32_MAX || !u(seq) || !seq ||
          seq > out.最大序号)
        return 因果数据状态::内部不一致;
      auto it = refs.find(seq);
      if (it == refs.end() || it->second.type != layout_.证据存在关系类型 ||
          !used.insert(seq).second || it->second.create != item.创建事实代次 ||
          it->second.exit != item.退出事实代次)
        return 因果数据状态::内部不一致;
      x.角色 = static_cast<std::uint32_t>(role);
      x.存在 = it->second.target;
      item.内容.角色绑定组.push_back(x);
      item.引用关系组.push_back({seq, it->second.relation});
    }
    if (!u(n) || n > UINT32_MAX || n > (data->size() - p) / 4)
      return 因果数据状态::内部不一致;
    for (std::uint64_t j = 0; j < n; ++j) {
      因果状态证据项 x;
      std::uint64_t category, index, dseq, bseq;
      if (!u(category) || category < 1 || category > 2 || !u(index) ||
          index > UINT32_MAX || !u(dseq) || !u(bseq) || !dseq || !bseq ||
          dseq > out.最大序号 || bseq > out.最大序号)
        return 因果数据状态::内部不一致;
      auto di = refs.find(dseq), bi = refs.find(bseq);
      if (di == refs.end() || bi == refs.end() ||
          di->second.type != layout_.证据锚点关系类型 ||
          bi->second.type != layout_.证据绑定关系类型 ||
          !used.insert(dseq).second || !used.insert(bseq).second ||
          di->second.create != item.创建事实代次 ||
          bi->second.create != item.创建事实代次 ||
          di->second.exit != item.退出事实代次 ||
          bi->second.exit != item.退出事实代次)
        return 因果数据状态::内部不一致;
      x.类别 = static_cast<因果证据项类别>(category);
      x.定义项序号 = static_cast<std::uint32_t>(index);
      x.原子来源 = {di->second.target};
      x.绑定 = {bi->second.target};
      item.内容.状态证据组.push_back(x);
      item.引用关系组.push_back({dseq, di->second.relation});
      item.引用关系组.push_back({bseq, bi->second.relation});
    }
    if (!u(tag) || tag > 1)
      return 因果数据状态::内部不一致;
    if (tag) {
      std::uint64_t relation;
      if (!u(relation) || !relation)
        return 因果数据状态::内部不一致;
      item.内容.动作来源 = 因果动作来源引用{{relation}};
    }
    if (!u(n) || n != item.引用关系组.size())
      return 因果数据状态::内部不一致;
    for (std::uint64_t j = 0; j < n; ++j) {
      std::uint64_t listed;
      if (!u(listed) || !listed || listed > out.最大序号 ||
          listed != item.引用关系组[j].first)
        return 因果数据状态::内部不一致;
    }
    out.项目组.push_back(std::move(item));
  }
  if (p != data->size() || used.size() != refs.size())
    return 因果数据状态::内部不一致;
  return 因果数据状态::已读取;
}

namespace 因果证据编码细节 {
template <class Index> std::vector<std::uint64_t> 编码(const Index &index) {
  std::vector<std::uint64_t> v{4341, 1, index.最大序号, index.项目组.size()};
  for (const auto &item : index.项目组) {
    std::size_t p = 0;
    auto next = [&]() { return item.引用关系组.at(p++).first; };
    v.push_back(next());
    v.push_back(item.内容.证据截止);
    v.push_back(item.内容.角色绑定组.size());
    for (const auto &r : item.内容.角色绑定组) {
      v.push_back(r.角色);
      v.push_back(next());
    }
    v.push_back(item.内容.状态证据组.size());
    for (const auto &s : item.内容.状态证据组) {
      v.push_back(static_cast<unsigned>(s.类别));
      v.push_back(s.定义项序号);
      v.push_back(next());
      v.push_back(next());
    }
    v.push_back(item.内容.动作来源 ? 1 : 0);
    if (item.内容.动作来源)
      v.push_back(item.内容.动作来源->关系身份.值);
    v.push_back(item.引用关系组.size());
    for (const auto &[seq, _] : item.引用关系组)
      v.push_back(seq);
  }
  return v;
}
template <class Item>
std::vector<std::pair<稳定编码, 稳定编码>> 目标(const Item &item,
                                                const 因果结构交付 &l) {
  std::vector<std::pair<稳定编码, 稳定编码>> v;
  v.push_back({item.内容.发生锚点.编码, l.证据锚点关系类型});
  for (const auto &r : item.内容.角色绑定组)
    v.push_back({r.存在, l.证据存在关系类型});
  for (const auto &s : item.内容.状态证据组) {
    v.push_back({s.原子来源.编码, l.证据锚点关系类型});
    v.push_back({s.绑定.编码, l.证据绑定关系类型});
  }
  return v;
}
} // namespace 因果证据编码细节

L1所有者范围写集请求
因果类数据服务::形成证据关联写集(const 因果证据关联请求 &r,
                                 const 证据索引 &index) const {
  L1所有者范围写集请求 w{L1所有者范围CRUD合同版本, r.G0, r.幂等身份};
  const L1所有者范围写集本地键 newValue{1};
  w.值 = {{newValue, r.因果.编码, layout_.证据索引属性类型,
           因果证据编码细节::编码(index), r.因果.编码}};
  w.属性槽变更 = {{r.因果.编码, layout_.证据索引属性类型, newValue}};
  w.退出事实 = {index.当前值};
  std::uint32_t nextLocalKey = 2;
  for (const auto &item : index.项目组) {
    const auto targets = 因果证据编码细节::目标(item, layout_);
    if (targets.size() != item.引用关系组.size())
      throw std::logic_error("invalid evidence references");
    for (std::size_t i = 0; i < targets.size(); ++i)
      if (!有效(item.引用关系组[i].second))
        w.关系.push_back(
            {{nextLocalKey++},
             r.因果.编码,
             targets[i].first,
             targets[i].second,
             static_cast<std::int64_t>(item.引用关系组[i].first)});
  }
  return w;
}

L1所有者范围写集请求
因果类数据服务::形成证据退出写集(const 因果证据退出请求 &r,
                                 const 证据索引 &index,
                                 const 证据索引项 &old) const {
  L1所有者范围写集请求 w{L1所有者范围CRUD合同版本, r.G0, r.幂等身份};
  const L1所有者范围写集本地键 newValue{1};
  w.值 = {{newValue, r.定位.因果.编码, layout_.证据索引属性类型,
           因果证据编码细节::编码(index), r.定位.因果.编码}};
  w.属性槽变更 = {{r.定位.因果.编码, layout_.证据索引属性类型, newValue}};
  w.退出事实 = {index.当前值};
  for (const auto &[_, id] : old.引用关系组)
    w.退出事实.push_back(id);
  std::sort(w.退出事实.begin(), w.退出事实.end());
  return w;
}

因果证据事实 因果类数据服务::投影证据事实(因果信息身份 q,
                                          const 证据索引项 &item,
                                          std::uint64_t g, std::uint64_t h,
                                          std::optional<std::uint64_t> exit) {
  std::vector<稳定编码> ids;
  for (const auto &[_, id] : item.引用关系组)
    ids.push_back(id);
  因果证据事实 out{g,
                   h,
                   q,
                   item.内容,
                   ids.front(),
                   {},
                   item.创建事实代次,
                   exit ? exit : item.退出事实代次};
  out.来源关系组.assign(ids.begin() + 1, ids.end());
  return out;
}

因果数据状态
因果类数据服务::核验证据来源(const 因果定义 &d, const 因果证据提交 &e,
                             const 因果概念核验结果 &concepts, std::uint64_t g,
                             因果读取预算 &budget, 来源读取缓存 &cache,
                             bool current) const {
  if (!有效(e.发生锚点) || !e.证据截止 || e.证据截止 > g)
    return 因果数据状态::入口拒绝;
  auto normalized = e;
  规范化证据(normalized);
  if (normalized != e)
    return 因果数据状态::入口拒绝;
  if (e.角色绑定组.size() != d.参与者.角色组.size() ||
      e.状态证据组.size() != d.条件组.size() + d.结果组.size())
    return 因果数据状态::来源不足;
  std::map<std::uint32_t, 稳定编码> roles;
  for (std::size_t i = 0; i < e.角色绑定组.size(); ++i) {
    if (e.角色绑定组[i].角色 != i + 1 || !有效(e.角色绑定组[i].存在) ||
        !roles.emplace(e.角色绑定组[i].角色, e.角色绑定组[i].存在).second)
      return 因果数据状态::角色冲突;
  }
  for (const auto &x : d.参与者.同异约束组) {
    const bool same = roles[x.左角色] == roles[x.右角色];
    if ((x.关系 == 因果角色同异::同一) != same)
      return 因果数据状态::角色冲突;
  }
  动态读取预算 db{budget.最大候选数, budget.最大来源材料数, budget.最大关系数,
                  budget.最大展开深度, budget.最大来源材料数};
  const 来源读取缓存::展开键 expandKey{
      g, e.证据截止, e.发生锚点.编码.值,
      static_cast<unsigned>(动态展开方式::递归)};
  auto expandIt = cache.动态展开.find(expandKey);
  if (expandIt == cache.动态展开.end())
    expandIt = cache.动态展开
                   .emplace(expandKey,
                            dynamics_.展开动态来源(
                                {1, g, e.证据截止, e.发生锚点,
                                 动态展开方式::递归, db}))
                   .first;
  const auto &expanded = expandIt->second;
  if (!expanded.成功()) {
    if (expanded.来源)
      return 因果数据状态::内部不一致;
    return 映射动态(expanded.操作.状态);
  }
  if (expanded.来源->Gread != g || expanded.来源->H != e.证据截止)
    return 因果数据状态::内部不一致;
  std::set<std::uint64_t> nodes, relations, values;
  const auto add = [](std::set<std::uint64_t> &set, 稳定编码 id) {
    return 有效(id) && set.insert(id.值).second;
  };
  for (const auto &v : expanded.来源->动态组) {
    add(nodes, v.信息.身份.编码);
    add(relations, v.自有事实.族归属关系);
    add(relations, v.自有事实.主体关系);
    for (auto id : v.自有事实.变化关系组)
      add(relations, id);
    add(values, v.自有事实.首次形成UTC值);
  }
  for (const auto &a : expanded.来源->原子来源组) {
    const 状态使用绑定事实 bindings[]{a.前绑定, a.后绑定};
    for (const auto &v : bindings) {
      add(nodes, v.信息.身份.编码);
      add(relations, v.场景成员关系);
      add(relations, v.存在成员关系);
      add(relations, v.状态成员关系);
    }
    const 状态内容事实 states[]{a.前状态, a.后状态};
    for (const auto &v : states) {
      add(nodes, v.信息.身份.编码);
      add(relations, v.族归属关系);
      add(relations, v.正式特征类型关系);
      add(values, v.准确值事实);
      add(values, v.强时间事实);
      add(values, v.首次形成UTC事实);
    }
  }
  if (!登记公开材料(budget, cache, nodes, relations, values))
    return 因果数据状态::数量预算不足;
  std::map<std::uint64_t, const 动态原子来源事实 *> atoms;
  for (const auto &a : expanded.来源->原子来源组)
    atoms.emplace(a.动态.编码.值, &a);
  std::map<std::uint64_t, 特征类型身份> ft;
  for (const auto &x : concepts.概念组)
    if (x.类别 == 因果概念类别::特征域 && x.正式特征类型)
      ft.emplace(x.概念.值, *x.正式特征类型);
  auto readBinding = [&](状态使用绑定身份 id, std::uint64_t h,
                         bool now) -> std::pair<因果数据状态,
                                                const 状态使用绑定事实 *> {
    const 来源读取缓存::键 key{g, h, id.编码.值};
    auto it = cache.绑定读取.find(key);
    if (it == cache.绑定读取.end()) {
      if (!可预留公开材料(budget, 3, 0, 4))
        return {因果数据状态::数量预算不足, nullptr};
      auto result = now ? bindings_.读取当前状态使用绑定({1, g, id})
                        : bindings_.读取状态使用绑定历史({1, g, h, id});
      it = cache.绑定读取.emplace(key, std::move(result)).first;
    }
    const auto &result = it->second;
    if (!result.成功()) {
      if (result.绑定)
        return {因果数据状态::内部不一致, nullptr};
      return {映射绑定(result.结果头.状态), nullptr};
    }
    if (!result.绑定 || result.绑定->Gread != g || result.绑定->H != h ||
        result.绑定->信息.身份 != id)
      return {因果数据状态::内部不一致, nullptr};
    const std::set<std::uint64_t> ns{result.绑定->信息.身份.编码.值};
    const std::set<std::uint64_t> rs{result.绑定->场景成员关系.值,
                                     result.绑定->存在成员关系.值,
                                     result.绑定->状态成员关系.值};
    if (!登记公开材料(budget, cache, ns, rs, {}))
      return {因果数据状态::数量预算不足, nullptr};
    return {因果数据状态::已读取, &*result.绑定};
  };
  auto readState = [&](状态信息身份 id,
                       std::uint64_t h) -> std::pair<因果数据状态,
                                                      const 状态内容事实 *> {
    const 来源读取缓存::键 key{g, h, id.编码.值};
    auto it = cache.状态读取.find(key);
    if (it == cache.状态读取.end()) {
      if (!可预留公开材料(budget, 2, 3, 6))
        return {因果数据状态::数量预算不足, nullptr};
      it = cache.状态读取
               .emplace(key, states_.读取状态历史内容({2, g, h, id}))
               .first;
    }
    const auto &result = it->second;
    if (!result.成功()) {
      if (result.内容)
        return {因果数据状态::内部不一致, nullptr};
      switch (result.结果头.状态) {
      case 状态类数据状态::未找到:
      case 状态类数据状态::目标已退出:
      case 状态类数据状态::正式特征类型未找到:
      case 状态类数据状态::正式特征类型已退出:
        return {因果数据状态::来源不足, nullptr};
      case 状态类数据状态::历史材料已清理:
        return {因果数据状态::历史材料已清理, nullptr};
      case 状态类数据状态::旧格式不支持:
        return {因果数据状态::旧格式不支持, nullptr};
      case 状态类数据状态::事实代次漂移:
        return {因果数据状态::事实代次漂移, nullptr};
      case 状态类数据状态::数量预算不足:
        return {因果数据状态::数量预算不足, nullptr};
      case 状态类数据状态::资源失败:
        return {因果数据状态::资源失败, nullptr};
      default:
        return {因果数据状态::内部不一致, nullptr};
      }
    }
    if (!result.内容 || result.内容->Gread != g || result.内容->H != h ||
        result.内容->信息.身份 != id)
      return {因果数据状态::内部不一致, nullptr};
    const std::set<std::uint64_t> ns{result.内容->信息.身份.编码.值};
    const std::set<std::uint64_t> rs{result.内容->族归属关系.值,
                                     result.内容->正式特征类型关系.值};
    const std::set<std::uint64_t> vs{result.内容->准确值事实.值,
                                     result.内容->强时间事实.值,
                                     result.内容->首次形成UTC事实.值};
    if (!登记公开材料(budget, cache, ns, rs, vs))
      return {因果数据状态::数量预算不足, nullptr};
    return {因果数据状态::已读取, &*result.内容};
  };
  auto confirmCurrentDynamic = [&](动态信息身份 id) -> 因果数据状态 {
    const 来源读取缓存::键 key{g, g, id.编码.值};
    auto it = cache.动态当前身份.find(key);
    if (it == cache.动态当前身份.end())
      it = cache.动态当前身份
               .emplace(key, dynamics_.确认当前动态结构身份({1, g, id}))
               .first;
    const auto &result = it->second;
    if (result.状态 != 动态数据状态::已读取)
      return 映射动态(result.状态);
    if (result.合同版本 != 1 || result.Gread != g || result.H != g ||
        result.发布代次 || result.阶段 != 动态发布阶段::无写入)
      return 因果数据状态::内部不一致;
    return 因果数据状态::已读取;
  };
  std::int64_t maxStart = std::numeric_limits<std::int64_t>::min(),
               minEnd = std::numeric_limits<std::int64_t>::max(),
               maxCondition = std::numeric_limits<std::int64_t>::min(),
               minResult = std::numeric_limits<std::int64_t>::max();
  for (std::size_t i = 0; i < e.状态证据组.size(); ++i) {
    const auto &x = e.状态证据组[i];
    const bool expectedCondition = i < d.条件组.size();
    const auto expectedIndex = static_cast<std::uint32_t>(
        expectedCondition ? i + 1 : i - d.条件组.size() + 1);
    if (x.类别 != (expectedCondition ? 因果证据项类别::条件
                                     : 因果证据项类别::结果) ||
        x.定义项序号 != expectedIndex)
      return 因果数据状态::来源不足;
    const bool cond = expectedCondition;
    const auto definitionCount = cond ? d.条件组.size() : d.结果组.size();
    if (!x.定义项序号 || x.定义项序号 > definitionCount)
      return 因果数据状态::来源不足;
    auto ai = atoms.find(x.原子来源.编码.值);
    if (ai == atoms.end())
      return 因果数据状态::来源不足;
    const auto &a = *ai->second;
    const auto &binding = cond ? a.前绑定 : a.后绑定;
    const auto &state = cond ? a.前状态 : a.后状态;
    if (binding.信息.身份 != x.绑定)
      return 因果数据状态::来源不足;
    const auto role = cond ? d.条件组[x.定义项序号 - 1].参与者角色
                           : d.结果组[x.定义项序号 - 1].承担者角色;
    if (binding.信息.被描述存在 != roles[role])
      return 因果数据状态::角色冲突;
    const auto fc = cond ? d.条件组[x.定义项序号 - 1].特征概念.编码
                         : d.结果组[x.定义项序号 - 1].特征概念.编码;
    if (!concepts.概念组.empty()) {
      auto fi = ft.find(fc.值);
      if (fi == ft.end() || fi->second != state.信息.正式特征类型)
        return 因果数据状态::概念不相容;
    }
    const auto start = a.前状态.信息.强时间.纳秒,
               end = a.后状态.信息.强时间.纳秒;
    if (a.前状态.信息.强时间.语义 != a.后状态.信息.强时间.语义 || start >= end)
      return 因果数据状态::来源不足;
    maxStart = std::max(maxStart, start);
    minEnd = std::min(minEnd, end);
    if (cond)
      maxCondition = std::max(maxCondition, start);
    else
      minResult = std::min(minResult, end);
    const auto bindingRead = readBinding(x.绑定, current ? g : e.证据截止, current);
    if (bindingRead.first != 因果数据状态::已读取)
      return bindingRead.first;
    if ((current && bindingRead.second->信息 != binding.信息) ||
        (!current && *bindingRead.second != binding))
      return 因果数据状态::内部不一致;
    if (current) {
      const auto ds = confirmCurrentDynamic(x.原子来源);
      if (ds != 因果数据状态::已读取)
        return ds;
    }
    const auto stateRead = readState(state.信息.身份, e.证据截止);
    if (stateRead.first != 因果数据状态::已读取)
      return stateRead.first;
    const auto &actualState = *stateRead.second;
    const bool stateEcho = actualState.Gread == state.Gread && actualState.H == state.H &&
                           actualState.信息.身份 == state.信息.身份 &&
                           actualState.信息.正式特征类型 == state.信息.正式特征类型 &&
                           actualState.信息.固定准确值 == state.信息.固定准确值 &&
                           actualState.信息.强时间.语义 == state.信息.强时间.语义 &&
                           actualState.信息.强时间.纳秒 == state.信息.强时间.纳秒 &&
                           actualState.族归属关系 == state.族归属关系 &&
                           actualState.正式特征类型关系 == state.正式特征类型关系 &&
                           actualState.准确值事实 == state.准确值事实 &&
                           actualState.强时间事实 == state.强时间事实 &&
                           actualState.首次形成UTC事实 == state.首次形成UTC事实 &&
                           actualState.首次形成UTC纳秒 == state.首次形成UTC纳秒 &&
                           actualState.创建事实代次 == state.创建事实代次 &&
                           actualState.退出事实代次 == state.退出事实代次;
    if (!stateEcho)
      return 因果数据状态::内部不一致;
  }
  if (maxStart >= minEnd || maxCondition >= minResult)
    return 因果数据状态::来源不足;
  if (current) {
    const auto root = confirmCurrentDynamic(e.发生锚点);
    if (root != 因果数据状态::已读取)
      return root;
  }
  return 因果数据状态::已读取;
}

L1所有者范围写集请求
因果类数据服务::形成因果退出写集(const 因果退出请求 &r, const 因果内容事实 &v,
                                 const 证据索引 &index) const {
  L1所有者范围写集请求 w{L1所有者范围CRUD合同版本, r.G0, r.幂等身份};
  w.退出事实 = {v.信息.身份.编码, v.族归属关系, v.定义值, index.当前值};
  w.退出事实.insert(w.退出事实.end(), v.定义引用关系组.begin(),
                    v.定义引用关系组.end());
  std::sort(w.退出事实.begin(), w.退出事实.end());
  return w;
}

std::optional<因果单项结果> 因果类数据服务::重放退出(const 因果退出请求 &r) {
  const auto first =
      port_.读取首次写入材料({L1所有者范围首次写入读取合同版本, r.幂等身份});
  if (first.状态 == L1所有者范围读取状态::未找到)
    return std::nullopt;
  if (first.状态 != L1所有者范围读取状态::成功 || !first.首次规范化写集 ||
      !first.首次写入结果)
    return 单项失败(映射读取(first.状态), first.读取事实代次);
  const auto &ws = *first.首次规范化写集;
  if (ws.期望事实代次 != r.G0 || !ws.节点.empty() || !ws.关系.empty() ||
      !ws.值.empty() || !ws.属性槽变更.empty() ||
      !std::binary_search(ws.退出事实.begin(), ws.退出事实.end(), r.身份.编码))
    return 单项失败(因果数据状态::幂等冲突, first.读取事实代次);
  const auto saved = port_.提交所有者范围中性写集(ws);
  if (saved.状态 != L1所有者范围写入状态::精确重复 ||
      !写入头完整(saved, r.幂等身份, r.G0))
    return 提交失败(映射写入(saved.状态, 因果数据状态::已退出), saved.事实代次);
  const auto guard = 读取当前代次();
  if (guard.状态 != 因果数据状态::已读取)
    return 提交后失败(saved.事实代次, guard.Gread);
  auto read = 读取历史({1, guard.Gread, saved.事实代次 - 1, r.身份, r.预算},
                       saved.事实代次);
  if (!read.成功())
    return 提交后失败(saved.事实代次, read.操作.Gread);
  return 因果单项结果{{因果数据状态::精确重复, 1, guard.Gread,
                       saved.事实代次 - 1, saved.事实代次,
                       因果发布阶段::已读回},
                      std::move(read.内容)};
}

std::optional<因果证据结果>
因果类数据服务::重放证据关联(const 因果证据关联请求 &r) {
  const auto first =
      port_.读取首次写入材料({L1所有者范围首次写入读取合同版本, r.幂等身份});
  if (first.状态 == L1所有者范围读取状态::未找到)
    return std::nullopt;
  if (first.状态 != L1所有者范围读取状态::成功 || !first.首次规范化写集 ||
      !first.首次写入结果)
    return 证据失败(映射读取(first.状态), first.读取事实代次);
  const auto &ws = *first.首次规范化写集;
  if (ws.期望事实代次 != r.G0 || ws.值.size() != 1 ||
      ws.属性槽变更.size() != 1 ||
      ws.属性槽变更[0].所属节点 != L1所有者范围事实引用{r.因果.编码})
    return 证据失败(因果数据状态::幂等冲突, first.读取事实代次);
  const auto originalG = first.首次写入结果->事实代次;
  const auto guard = 读取当前代次();
  if (guard.状态 != 因果数据状态::已读取)
    return 证据失败(guard.状态, guard.Gread, originalG);
  证据索引 index;
  if (读取证据索引(r.因果, guard.Gread, originalG, index) !=
      因果数据状态::已读取)
    return 证据失败(因果数据状态::内部不一致, guard.Gread, originalG);
  auto normalized = r.证据;
  规范化证据(normalized);
  const auto item = std::find_if(index.项目组.begin(), index.项目组.end(),
                                 [&](const auto &v) {
                                   return v.内容.发生锚点 == normalized.发生锚点;
                                 });
  if (item == index.项目组.end() || item->内容 != normalized ||
      item->创建事实代次 != originalG)
    return 证据失败(因果数据状态::幂等冲突, guard.Gread, originalG);
  const auto saved = port_.提交所有者范围中性写集(ws);
  if (saved.状态 != L1所有者范围写入状态::精确重复 ||
      !写入头完整(saved, r.幂等身份, r.G0))
    return 证据提交失败(映射写入(saved.状态, 因果数据状态::已关联),
                        saved.事实代次);
  const auto after = 读取当前代次();
  if (after.状态 != 因果数据状态::已读取)
    return 证据提交后失败(saved.事实代次, after.Gread);
  证据索引 confirmed;
  if (读取证据索引(r.因果, after.Gread, originalG, confirmed) !=
      因果数据状态::已读取)
    return 证据提交后失败(saved.事实代次, after.Gread);
  const auto confirmedItem =
      std::find_if(confirmed.项目组.begin(), confirmed.项目组.end(),
                   [&](const auto &v) {
                     return v.内容.发生锚点 == normalized.发生锚点;
                   });
  if (confirmedItem == confirmed.项目组.end() || confirmedItem->内容 != normalized ||
      confirmedItem->创建事实代次 != originalG)
    return 证据提交后失败(saved.事实代次, after.Gread);
  auto fact = 投影证据事实(r.因果, *confirmedItem, after.Gread, originalG);
  return 因果证据结果{{因果数据状态::精确重复, 1, after.Gread, originalG,
                        originalG, 因果发布阶段::已读回},
                       {std::move(fact)}};
}

std::optional<因果证据结果>
因果类数据服务::重放证据退出(const 因果证据退出请求 &r) {
  const auto first =
      port_.读取首次写入材料({L1所有者范围首次写入读取合同版本, r.幂等身份});
  if (first.状态 == L1所有者范围读取状态::未找到)
    return std::nullopt;
  if (first.状态 != L1所有者范围读取状态::成功 || !first.首次规范化写集 ||
      !first.首次写入结果)
    return 证据失败(映射读取(first.状态), first.读取事实代次);
  const auto &ws = *first.首次规范化写集;
  if (ws.期望事实代次 != r.G0 || ws.值.size() != 1 ||
      ws.属性槽变更.size() != 1 ||
      ws.属性槽变更[0].所属节点 != L1所有者范围事实引用{r.定位.因果.编码})
    return 证据失败(因果数据状态::幂等冲突, first.读取事实代次);
  const auto originalG = first.首次写入结果->事实代次;
  if (originalG <= 1)
    return 证据失败(因果数据状态::内部不一致, first.读取事实代次);
  const auto guard = 读取当前代次();
  if (guard.状态 != 因果数据状态::已读取)
    return 证据失败(guard.状态, guard.Gread, originalG - 1);
  证据索引 index;
  if (读取证据索引(r.定位.因果, guard.Gread, originalG - 1, index) !=
      因果数据状态::已读取)
    return 证据失败(因果数据状态::内部不一致, guard.Gread, originalG - 1);
  const auto item = std::find_if(index.项目组.begin(), index.项目组.end(),
                                 [&](const auto &v) {
                                   return v.内容.发生锚点 == r.定位.发生锚点;
                                 });
  if (item == index.项目组.end() || item->退出事实代次 != originalG)
    return 证据失败(因果数据状态::幂等冲突, guard.Gread, originalG - 1);
  const auto saved = port_.提交所有者范围中性写集(ws);
  if (saved.状态 != L1所有者范围写入状态::精确重复 ||
      !写入头完整(saved, r.幂等身份, r.G0))
    return 证据提交失败(映射写入(saved.状态, 因果数据状态::已退出),
                          saved.事实代次);
  const auto after = 读取当前代次();
  if (after.状态 != 因果数据状态::已读取)
    return 证据提交后失败(saved.事实代次, after.Gread);
  证据索引 confirmed;
  if (读取证据索引(r.定位.因果, after.Gread, originalG - 1, confirmed) !=
      因果数据状态::已读取)
    return 证据提交后失败(saved.事实代次, after.Gread);
  const auto confirmedItem =
      std::find_if(confirmed.项目组.begin(), confirmed.项目组.end(),
                   [&](const auto &v) {
                     return v.内容.发生锚点 == r.定位.发生锚点;
                   });
  if (confirmedItem == confirmed.项目组.end() ||
      confirmedItem->退出事实代次 != originalG)
    return 证据提交后失败(saved.事实代次, after.Gread);
  auto fact = 投影证据事实(r.定位.因果, *confirmedItem, after.Gread, originalG - 1,
                           originalG);
  return 因果证据结果{{因果数据状态::精确重复, 1, after.Gread,
                        originalG - 1, originalG, 因果发布阶段::已读回},
                       {std::move(fact)}};
}

} // namespace 海中鱼巣
