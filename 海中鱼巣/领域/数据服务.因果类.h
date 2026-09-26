#pragma once

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



#include "合同.因果结构.h"

namespace 海中鱼巣 {

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
        !布局材料有效())
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
          concepts_.核验因果定义概念({1, r.G0, definition, r.预算});
      const auto conceptStatus =
          核验概念回显(checked, definition, r.预算, r.G0);
      if (conceptStatus != 因果数据状态::已读取)
        return 单项失败(conceptStatus, r.G0);
      auto remaining = r.预算;
      if (!扣减概念用量(remaining, checked))
        return 单项失败(因果数据状态::内部不一致, r.G0);
      for (const auto &source : definition.来源组)
        if (const auto *q = std::get_if<因果信息身份>(&source.目标)) {
          const auto prior = 读取当前({1, r.G0, *q, remaining}, &remaining);
          if (!prior.成功())
            return 单项失败(映射来源(prior.操作.状态), prior.操作.Gread, r.G0);
        }
      const auto duplicate = 查询同义(r.G0, definition, remaining, &remaining);
      if (!duplicate.成功())
        return 单项失败(duplicate.操作.状态, duplicate.操作.Gread, r.G0);
      if (duplicate.因果组.size() > 1)
        return 单项失败(因果数据状态::内部不一致, r.G0, r.G0);
      if (duplicate.因果组.size() == 1)
        return {{因果数据状态::已复用, 1, r.G0, std::nullopt,
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
      auto out = 读取当前({1, after.Gread, {*id}, r.预算});
      if (!out.成功() || out.内容->信息.定义 != definition)
        return 提交后失败(saved.事实代次, out.操作.Gread);
      return {{status, 1, after.Gread, saved.事实代次,
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
      auto out = 读取当前({r.合同版本, r.G0, r.身份, r.预算});
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

  因果组结果 查询精确定义(const 因果精确定义查询请求 &r) const override {
    if (!查询请求有效(r))
      return 组失败(因果数据状态::入口拒绝);
    try {
      auto d = r.定义;
      const auto s = 规范化并核验(d);
      if (s != 因果数据状态::已读取)
        return 组失败(s, r.Gread);
      const auto definitionItems = 定义项总数(d);
      if (!definitionItems || *definitionItems > r.预算.最大定义项数)
        return 组失败(因果数据状态::数量预算不足, r.Gread);
      auto out = 查询同义(r.Gread, d, r.预算);
      const auto after = 读取当前代次();
      if (out.成功() &&
          (after.状态 != 因果数据状态::已读取 || after.Gread != r.Gread))
        return 组失败(after.状态 == 因果数据状态::已读取
                          ? 因果数据状态::事实代次漂移
                          : after.状态,
                      after.Gread);
      return out;
    } catch (const std::bad_alloc &) {
      return 组失败(因果数据状态::资源失败);
    } catch (...) {
      return 组失败(因果数据状态::内部不一致);
    }
  }

  因果组结果 按特征概念查询因果(const 因果按特征查询请求 &r) const override {
    if (r.合同版本 != 1 || !r.Gread ||
        !有效(r.特征概念) || !预算有效(r.预算) ||
        (r.方向 != 因果特征召回方向::条件 && r.方向 != 因果特征召回方向::结果))
      return 组失败(因果数据状态::入口拒绝);
    try {
      const auto guard = 读取当前代次();
      if (guard.状态 != 因果数据状态::已读取)
        return 组失败(guard.状态, guard.Gread);
      if (guard.Gread != r.Gread)
        return 组失败(因果数据状态::事实代次漂移, guard.Gread);
      const auto type = r.方向 == 因果特征召回方向::条件
                            ? layout_.条件FC关系类型
                            : layout_.结果FC关系类型;
      const auto q = l1_.读取所有者范围当前目标关系组(
          {L1所有者范围CRUD合同版本,r.特征概念.编码,type});
      if (q.状态 != L1所有者范围读取状态::成功)
        return 组失败(映射读取(q.状态), q.读取事实代次);
      if (q.读取事实代次 != r.Gread)
        return 组失败(因果数据状态::事实代次漂移, q.读取事实代次);
      std::vector<因果信息身份> ids;
      std::map<std::uint64_t, std::set<std::uint64_t>> alreadyCountedByQ;
      for (const auto &e : q.关系组) {
        if (e.写入所有者 != owner_ || e.目标节点 != r.特征概念.编码 ||
            e.关系类型节点 != type)
          return 组失败(因果数据状态::内部不一致, r.Gread);
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
        return 组失败(因果数据状态::数量预算不足, r.Gread);
      remaining.最大候选数 -= ids.size();
      remaining.最大关系数 -= q.关系组.size();
      remaining.最大来源材料数 -= q.关系组.size();
      因果组结果 out{读取头(r.Gread), {}};
      for (auto id : ids) {
        auto one = 读取当前({1, r.Gread, id, remaining}, &remaining, false,
                           &alreadyCountedByQ.at(id.编码.值),
                           type, r.特征概念.编码);
        if (!one.成功())
          return 组失败(one.操作.状态, one.操作.Gread);
        out.因果组.push_back(*one.内容);
      }
      const auto after = 读取当前代次();
      if (after.状态 != 因果数据状态::已读取 || after.Gread != r.Gread)
        return 组失败(after.状态 == 因果数据状态::已读取
                          ? 因果数据状态::事实代次漂移
                          : after.状态,
                      after.Gread);
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
      const auto q = 读取当前({1, r.G0, r.因果, remaining}, &remaining);
      if (!q.成功())
        return 证据失败(q.操作.状态, q.操作.Gread);
      if (q.内容->信息.定义.类型 == 因果类型::主动)
        return 证据失败(因果数据状态::动作来源未实现, r.G0);
      if (r.证据.动作来源)
        return 证据失败(因果数据状态::入口拒绝, r.G0);
      const auto checked = concepts_.核验因果定义概念(
          {1, r.G0, q.内容->信息.定义, remaining});
      const auto cs = 核验概念回显(checked, q.内容->信息.定义, remaining, r.G0);
      if (cs != 因果数据状态::已读取)
        return 证据失败(cs, r.G0);
      if (!扣减概念用量(remaining, checked))
        return 证据失败(因果数据状态::内部不一致, r.G0);
      证据索引 index;
      const auto is = 读取证据索引(r.因果, r.G0, index);
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
          auto fact = 投影证据事实(r.因果, *existing, r.G0);
          return {{因果数据状态::已复用, 1, r.G0, std::nullopt,
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
      auto read = 读取因果证据({1, after.Gread, r.因果,
                                 normalized.发生锚点, r.预算});
      if (!read.成功() || read.证据组.size() != 1)
        return 证据提交后失败(saved.事实代次, read.操作.Gread);
      read.操作 = {status, 1, after.Gread, saved.事实代次,
                    因果发布阶段::已读回};
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
    if (r.合同版本 != 1 || !r.Gread || !有效(r.因果) ||
        !预算有效(r.预算) || (r.发生锚点 && !有效(*r.发生锚点)))
      return 证据失败(因果数据状态::入口拒绝);
    try {
      const auto guard = 读取当前代次();
      if (guard.状态 != 因果数据状态::已读取)
        return 证据失败(guard.状态, guard.Gread);
      if (guard.Gread != r.Gread)
        return 证据失败(因果数据状态::事实代次漂移, guard.Gread);
      auto remaining = r.预算;
      const auto q = 读取当前({1, r.Gread, r.因果, remaining}, &remaining);
      if (!q.成功())
        return 证据失败(q.操作.状态, q.操作.Gread);
      证据索引 index;
      const auto s = 读取证据索引(r.因果, r.Gread, index);
      if (s != 因果数据状态::已读取)
        return 证据失败(s, r.Gread);
      因果证据结果 out{读取头(r.Gread), {}};
      来源读取缓存 sourceCache;
      for (const auto &v : index.项目组) {
        if (r.发生锚点 && v.内容.发生锚点 != *r.发生锚点)
          continue;
        const auto valid = 核验证据来源(q.内容->信息.定义, v.内容, {}, r.Gread,
                                      remaining, sourceCache);
        if (valid != 因果数据状态::已读取)
          return 证据失败(valid, r.Gread);
        out.证据组.push_back(投影证据事实(r.因果, v, r.Gread));
      }
      if (r.发生锚点 && out.证据组.empty())
        return 证据失败(因果数据状态::未找到, r.Gread);
      const auto after = 读取当前代次();
      if (after.状态 != 因果数据状态::已读取 || after.Gread != r.Gread)
        return 证据失败(after.状态 == 因果数据状态::已读取
                            ? 因果数据状态::事实代次漂移
                            : after.状态,
                        after.Gread);
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
      const auto s = 读取证据索引(r.定位.因果, r.G0, index);
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
      const auto status = 映射写入(saved.状态, 因果数据状态::已删除);
      if (status != 因果数据状态::已删除 && status != 因果数据状态::精确重复)
        return 证据提交失败(status, saved.事实代次);
      if (!写入头完整(saved, r.幂等身份, r.G0))
        return 证据提交后失败(saved.事实代次);
      const auto after = 读取当前代次();
      if (after.状态 != 因果数据状态::已读取)
        return 证据提交后失败(saved.事实代次, after.Gread);
      证据索引 confirmed;
      if (读取证据索引(r.定位.因果, after.Gread, confirmed) !=
          因果数据状态::已读取)
        return 证据提交后失败(saved.事实代次, after.Gread);
      const auto confirmedItem =
          std::find_if(confirmed.项目组.begin(), confirmed.项目组.end(),
                       [&](const auto &v) {
                         return v.内容.发生锚点 == r.定位.发生锚点;
                       });
      if (confirmedItem != confirmed.项目组.end())
        return 证据提交后失败(saved.事实代次, after.Gread);
      return {{status, 1, after.Gread, saved.事实代次,
               因果发布阶段::已读回}, {}};
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
      const auto current = 读取当前({1, r.G0, r.身份, remaining}, &remaining);
      if (!current.成功())
        return current;
      证据索引 index;
      const auto is = 读取证据索引(r.身份, r.G0, index);
      if (is != 因果数据状态::已读取)
        return 单项失败(is, r.G0);
      if (!index.项目组.empty())
        return 单项失败(因果数据状态::引用冲突, r.G0);
      auto ws = 形成因果退出写集(r, *current.内容, index);
      const auto saved = port_.提交所有者范围中性写集(ws);
      const auto status = 映射写入(saved.状态, 因果数据状态::已删除);
      if (status != 因果数据状态::已删除 && status != 因果数据状态::精确重复)
        return 提交失败(status, saved.事实代次);
      if (!写入头完整(saved, r.幂等身份, r.G0))
        return 提交后失败(saved.事实代次);
      const auto after = 读取当前代次();
      if (after.状态 != 因果数据状态::已读取)
        return 提交后失败(saved.事实代次, after.Gread);
      auto read = 读取当前({1, after.Gread, r.身份, r.预算});
      if (read.操作.状态 != 因果数据状态::未找到 || read.内容)
        return 提交后失败(saved.事实代次, read.操作.Gread);
      return {{status, 1, after.Gread, saved.事实代次,
               因果发布阶段::已读回}, std::nullopt};
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
      auto h = 读取身份当前(r);
      const auto after = 读取当前代次();
      if (h.状态 == 因果数据状态::已读取 &&
          (after.状态 != 因果数据状态::已读取 || after.Gread != r.G0))
        return 失败头(after.状态 == 因果数据状态::已读取
                          ? 因果数据状态::事实代次漂移
                          : after.状态,
                      after.Gread);
      return h;
    } catch (const std::bad_alloc &) {
      return 失败头(因果数据状态::资源失败);
    } catch (...) {
      return 失败头(因果数据状态::内部不一致);
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
  struct 当前读取参数 {
    std::uint32_t 合同版本 = 1;
    std::uint64_t Gread = 0;
    因果信息身份 身份;
    因果读取预算 预算;
  };
  struct 证据索引项 {
    因果证据提交 内容;
    std::uint64_t 创建事实代次 = 0;
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
      const std::uint64_t fixed = 5 + (item.内容.动作来源 ? 1 : 0);
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
  static bool 查询请求有效(const 因果精确定义查询请求 &r) noexcept {
    return r.合同版本 == 1 && r.Gread && 预算有效(r.预算);
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
            预算有效(r.预算);
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
                              std::uint64_t = 0) noexcept {
    return {s, 1, g, std::nullopt, 因果发布阶段::无写入};
  }
  static 因果操作结果 读取头(std::uint64_t g) noexcept {
    return {因果数据状态::已读取, 1, g, std::nullopt, 因果发布阶段::无写入};
  }
  static 因果单项结果 单项失败(因果数据状态 s, std::uint64_t g = 0,
                                std::uint64_t = 0) noexcept {
    return {失败头(s, g), std::nullopt};
  }
  static 因果组结果 组失败(因果数据状态 s, std::uint64_t g = 0,
                            std::uint64_t = 0) noexcept {
    return {失败头(s, g), {}};
  }
  static 因果证据结果 证据失败(因果数据状态 s, std::uint64_t g = 0,
                                std::uint64_t = 0) noexcept {
    return {失败头(s, g), {}};
  }
  static 因果单项结果 提交失败(因果数据状态 s, std::uint64_t g) noexcept {
    if (s == 因果数据状态::已可能发布)
      return {{s, 1, g, g ? std::optional<std::uint64_t>{g} : std::nullopt,
               因果发布阶段::未知},
              std::nullopt};
    return 单项失败(s, g);
  }
  static 因果单项结果 提交后失败(std::uint64_t g,
                                 std::uint64_t read = 0) noexcept {
    return {
        {因果数据状态::内部不一致, 1, read, g, 因果发布阶段::已提交待读回},
        std::nullopt};
  }
  static 因果证据结果 证据提交失败(因果数据状态 s, std::uint64_t g) noexcept {
    if (s == 因果数据状态::已可能发布)
      return {{s, 1, g, g ? std::optional<std::uint64_t>{g} : std::nullopt,
               因果发布阶段::未知},
              {}};
    return 证据失败(s, g);
  }
  static 因果证据结果 证据提交后失败(std::uint64_t g,
                                     std::uint64_t read = 0) noexcept {
    return {
        {因果数据状态::内部不一致, 1, read, g, 因果发布阶段::已提交待读回},
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
           n->属性类型表示 == repr;
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
              e.目标节点 != expected[i])
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
    if (s == 因果数据状态::未找到)
      return 因果数据状态::来源不足;
    return s;
  }
  static 因果数据状态 映射动态(动态数据状态 s) noexcept {
    switch (s) {
    case 动态数据状态::依赖未实现:
      return 因果数据状态::依赖未实现;
    case 动态数据状态::未找到:
    case 动态数据状态::来源未找到:
      return 因果数据状态::来源不足;
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
    case 状态使用绑定数据状态::场景未找到:
    case 状态使用绑定数据状态::存在未找到:
    case 状态使用绑定数据状态::状态未找到:
      return 因果数据状态::来源不足;
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
  因果组结果 查询同义(std::uint64_t, const 因果定义 &,
                       const 因果读取预算 &, 因果读取预算 * = nullptr) const;
  因果单项结果 读取当前(const 当前读取参数 &, 因果读取预算 * = nullptr,
                        bool = false, const std::set<std::uint64_t> * = nullptr,
                        std::optional<稳定编码> = std::nullopt,
                        std::optional<稳定编码> = std::nullopt) const;
  因果操作结果 读取身份当前(const 因果当前身份请求 &) const;
  L1所有者范围写集请求 形成发布写集(const 因果发布请求 &,
                                    const 编码结果 &) const;
  std::optional<因果单项结果> 重放发布(const 因果发布请求 &, const 因果定义 &);
  std::optional<因果单项结果> 重放退出(const 因果退出请求 &);
  std::optional<因果证据结果> 重放证据关联(const 因果证据关联请求 &);
  std::optional<因果证据结果> 重放证据退出(const 因果证据退出请求 &);
  因果数据状态 读取证据索引(因果信息身份, std::uint64_t, 证据索引 &) const;
  static void 规范化证据(因果证据提交 &);
  static bool 添加证据索引项(证据索引 &, const 因果证据提交 &);
  因果数据状态 核验证据来源(const 因果定义 &, const 因果证据提交 &,
                             const 因果概念核验结果 &, std::uint64_t,
                             因果读取预算 &, 来源读取缓存 &) const;
  L1所有者范围写集请求 形成证据关联写集(const 因果证据关联请求 &,
                                        const 证据索引 &) const;
  L1所有者范围写集请求 形成证据退出写集(const 因果证据退出请求 &,
                                        const 证据索引 &,
                                        const 证据索引项 &) const;
  L1所有者范围写集请求 形成因果退出写集(const 因果退出请求 &,
                                        const 因果内容事实 &,
                                        const 证据索引 &) const;
  static 因果证据事实 投影证据事实(因果信息身份, const 证据索引项 &,
                                   std::uint64_t);

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
