#pragma once

#include "数据服务.绑定存在.h"
#include "数据服务.定位特征.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <limits>
#include <mutex>
#include <new>
#include <optional>
#include <stdexcept>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

#include "合同.场景角色组织.h"
#include "合同.世界树根.h"

namespace 海中鱼巣 {

class 场景类数据服务 final : public 状态使用绑定只读提供者,
                             public 场景动态组织只读提供者,
                             public 场景直接包含只读提供者, public 原子I64特征组织参与者,
                             public 绑定存在场景参与者, public 世界树根场景参与者 {
public:
  场景类数据服务() = delete;
  场景类数据服务(const 场景类数据服务 &) = delete;
  场景类数据服务 &operator=(const 场景类数据服务 &) = delete;
  场景类数据服务(场景类数据服务 &&) = delete;
  场景类数据服务 &operator=(场景类数据服务 &&) = delete;

  场景类数据服务(const L1事实基座服务 &l1, L1所有者范围写端口 &&port,
                 const 场景角色结构交付 &layout,
                 const 存在结构身份只读提供者 &existence,
                 const 状态类数据服务 &state,
                 const 场景特征组织扩展结构交付 &featureLayout,
                 const 场景直接包含扩展结构交付 &includeLayout)
      : l1_(l1), port_(std::move(port)), owner_(port_.所有者身份()),
        layout_(layout), featureLayout_(featureLayout),
        includeLayout_(includeLayout), existence_(existence),
        existenceStructure_(dynamic_cast<const 存在组成结构只读提供者 &>(existence)),
        state_(state) {
    if (!布局形状有效(layout_) || !有效(owner_) || !port_.有效() ||
        !port_.绑定于(l1_) || !有效(featureLayout_.特征组织关系类型) ||
        !包含布局形状有效(includeLayout_, layout_, featureLayout_) ||
        !existence_.绑定于(l1_) || !state_.绑定于(l1_) || !登记材料匹配() ||
        !扩展登记材料匹配() || !包含扩展登记材料匹配())
      throw std::invalid_argument("场景结构交付无效");
  }

  bool 绑定于(const L1事实基座服务 &x) const noexcept override {
    return &l1_ == &x && port_.绑定于(x) && existence_.绑定于(x) &&
           existenceStructure_.绑定于(x) && state_.绑定于(x);
  }

  const L1事实基座服务 &世界树根底座() const noexcept override { return l1_; }
  L1结构所有者身份 世界树根所有者() const noexcept override {
    return port_.所有者身份();
  }
  bool 世界树根幂等键可用(L1所有者范围写入幂等身份 k) const noexcept override {
    return k.值 && k.值 != 1 &&
           k != 场景特征组织扩展登记固定幂等身份 &&
           k != 场景直接包含扩展登记固定幂等身份 &&
           (k.值 >> 48) != 0x4E43;
  }
  bool 世界树根结构已就绪() const noexcept override { return 场景结构登记已就绪(); }
  世界树根场景片段
  准备世界树根场景片段(const 世界树根初始化请求 &r) const override {
    世界树根场景片段 out;
    out.写集 = {{2}, owner_, {L1所有者范围CRUD合同版本, r.G0, r.场景幂等身份}};
    const L1有限N分区原子事实引用_v3 root{{1}, {1}};
    for (std::uint32_t i = 1; i <= 4; ++i)
      out.写集.写集.节点.push_back({{i}, 节点种类::普通, std::nullopt});
    out.写集.写集.关系.push_back({{5}, root, layout_.绑定结构.场景族锚点,
                                  layout_.绑定结构.场景族归属关系类型, 1});
    for (std::uint32_t i = 1; i <= 4; ++i)
      out.写集.写集.关系.push_back({{i + 5}, root,
                                    L1所有者范围写集本地键{i},
                                    layout_.根绑定关系类型,
                                    static_cast<std::int64_t>(i)});
    out.写集.写集.关系.push_back({{10}, root, includeLayout_.树登记锚点,
                                  includeLayout_.根标记关系类型, 1});
    return out;
  }
  L1所有者范围首次写入读取结果 读取世界树根场景首次材料(
      L1所有者范围写入幂等身份 k) const override {
    return port_.读取首次写入材料({L1所有者范围首次写入读取合同版本, k});
  }
  世界树根组读取结果
  读取当前世界树根组(const 世界树根组读取请求 &r) const override {
    世界树根组读取结果 out{世界树根状态::入口拒绝, r.G0, {}};
    if (r.版本 != 世界树根合同版本 || !r.G0 || !r.最大关系数量 ||
        r.最大关系数量 > 4096)
      return out;
    try {
      auto q = 查询关系(r.G0, r.G0, L1所有者范围关系端点方向::目标,
                        includeLayout_.树登记锚点,
                        includeLayout_.根标记关系类型);
      if (!q.成功()) {
        out.Gread = q.Gread;
        switch (q.状态) {
        case 场景角色数据状态::事实代次漂移: out.状态 = 世界树根状态::事实代次漂移; break;
        case 场景角色数据状态::历史材料已清理: out.状态 = 世界树根状态::历史材料已清理; break;
        case 场景角色数据状态::资源失败: out.状态 = 世界树根状态::资源失败; break;
        default: out.状态 = 世界树根状态::内部不一致; break;
        }
        return out;
      }
      if (q.关系.size() > r.最大关系数量) {
        out.状态 = 世界树根状态::数量预算不足;
        return out;
      }
      for (const auto &e : q.关系) {
        if (!有效(e.源节点) || e.目标节点 != includeLayout_.树登记锚点 ||
            e.关系类型节点 != includeLayout_.根标记关系类型 ||
            e.角色或顺序 != 1 || e.写入所有者 != owner_ ||
            !e.创建事实代次 || e.退出事实代次) {
          out.状态 = 世界树根状态::内部不一致;
          out.根组.clear();
          return out;
        }
        out.根组.push_back(e.源节点);
      }
      std::sort(out.根组.begin(), out.根组.end(),
                [](auto a, auto b) { return a.值 < b.值; });
      out.状态 = 世界树根状态::精确重复;
    } catch (const std::bad_alloc &) {
      out.状态 = 世界树根状态::资源失败;
    } catch (...) {
      out.状态 = 世界树根状态::内部不一致;
    }
    return out;
  }

  static 场景特征组织扩展登记结果
  登记特征组织扩展(const L1事实基座服务 &l1, L1所有者范围写端口 &port,
                   const 场景特征组织扩展登记请求 &r);
  static 场景直接包含扩展登记结果
  登记直接包含扩展(const L1事实基座服务 &, L1所有者范围写端口 &,
                   const 场景直接包含扩展登记请求 &);

  static 场景结构登记结果 登记结构类型(const L1事实基座服务 &l1,
                                       L1所有者范围写端口 &port,
                                       const 场景结构登记请求 &r) {
    场景结构登记结果 out;
    if (r.版本 != 2 || !r.G0 || r.G0 == UINT64_MAX || r.幂等身份.值 != 1 ||
        !port.绑定于(l1))
      return out;
    bool commitStarted = false;
    std::uint64_t published = 0, knownFirst = 0;
    try {
      L1所有者范围写集请求 ws{L1所有者范围CRUD合同版本, r.G0, r.幂等身份};
      for (std::uint32_t i = 1; i <= 7; ++i)
        ws.节点.push_back({{i}, 节点种类::普通, std::nullopt});
      const auto first =
          port.读取首次写入材料({L1所有者范围首次写入读取合同版本, r.幂等身份});
      const bool wasReplay = first.状态 == L1所有者范围读取状态::成功;
      if (first.合同版本 != L1所有者范围首次写入读取合同版本 ||
          first.所有者 != port.所有者身份() ||
          first.写入幂等身份 != r.幂等身份) {
        out.状态 = 场景角色数据状态::内部不一致;
        out.Gread = first.读取事实代次;
        return out;
      }
      if (first.状态 == L1所有者范围读取状态::成功) {
        if (first.首次规范化写集 &&
            first.首次规范化写集->节点.size() == 6 &&
            first.首次规范化写集->关系.empty() &&
            first.首次规范化写集->值.empty() &&
            first.首次规范化写集->属性槽变更.empty() &&
            first.首次规范化写集->退出事实.empty()) {
          out.状态 = 场景角色数据状态::旧版本迁移拒绝;
          out.Gread = first.读取事实代次;
          return out;
        }
        if (!first.首次规范化写集 || !写集相同(*first.首次规范化写集, ws)) {
          out.状态 = 场景角色数据状态::幂等冲突;
          out.Gread = first.读取事实代次;
          return out;
        }
        const bool firstHeadComplete =
            first.首次写入结果 &&
            first.首次写入结果->状态 == L1所有者范围写入状态::成功 &&
            first.首次写入结果->合同版本 == L1所有者范围CRUD合同版本 &&
            first.首次写入结果->所有者 == port.所有者身份() &&
            first.首次写入结果->写入幂等身份 == r.幂等身份 &&
            first.首次写入结果->事实代次 == r.G0 + 1 &&
            first.首次写入结果->是否形成内存权威发布 &&
            first.首次写入结果->重试边界 == L1所有者范围重试边界::不适用 &&
            写入映射完整(*first.首次写入结果, ws);
        if (!firstHeadComplete) {
          const bool publishedBefore =
              first.首次写入结果 && first.首次写入结果->是否形成内存权威发布;
          out.状态 = publishedBefore ? 场景角色数据状态::已可能发布
                                     : 场景角色数据状态::内部不一致;
          out.Gread = first.读取事实代次;
          return out;
        }
        knownFirst = first.首次写入结果->事实代次;
      } else if (first.状态 == L1所有者范围读取状态::未找到) {
        if (first.读取事实代次 != r.G0 || first.首次规范化写集 ||
            first.首次写入结果) {
          out.状态 = first.读取事实代次 != r.G0 ? 场景角色数据状态::事实代次漂移
                                                : 场景角色数据状态::内部不一致;
          out.Gread = first.读取事实代次;
          return out;
        }
      } else {
        out.状态 = first.状态 == L1所有者范围读取状态::资源失败
                       ? 场景角色数据状态::资源失败
                       : 场景角色数据状态::内部不一致;
        out.Gread = first.读取事实代次;
        return out;
      }
      commitStarted = true;
      const auto saved = port.提交所有者范围中性写集(ws);
      published = saved.事实代次;
      if (wasReplay && saved.状态 != L1所有者范围写入状态::精确重复) {
        out.状态 = 映射场景写入状态(saved, 场景角色数据状态::已登记,
                                    port.所有者身份(), ws);
        if (out.状态 == 场景角色数据状态::已登记)
          out.状态 = saved.是否形成内存权威发布 ? 场景角色数据状态::已可能发布
                                                : 场景角色数据状态::内部不一致;
        out.Gread = saved.事实代次;
        out.首次发布代次 =
            out.状态 == 场景角色数据状态::已可能发布 ? saved.事实代次 : 0;
        return out;
      }
      if (!wasReplay && saved.状态 == L1所有者范围写入状态::精确重复) {
        const auto won = port.读取首次写入材料(
            {L1所有者范围首次写入读取合同版本, r.幂等身份});
        const bool wonFirstComplete =
            won.状态 == L1所有者范围读取状态::成功 &&
            won.合同版本 == L1所有者范围首次写入读取合同版本 &&
            won.所有者 == port.所有者身份() && won.写入幂等身份 == r.幂等身份 &&
            won.首次规范化写集 && 写集相同(*won.首次规范化写集, ws) &&
            won.首次写入结果 &&
            won.首次写入结果->状态 == L1所有者范围写入状态::成功 &&
            won.首次写入结果->合同版本 == L1所有者范围CRUD合同版本 &&
            won.首次写入结果->所有者 == port.所有者身份() &&
            won.首次写入结果->写入幂等身份 == r.幂等身份 &&
            won.首次写入结果->事实代次 == r.G0 + 1 &&
            won.首次写入结果->是否形成内存权威发布 &&
            won.首次写入结果->重试边界 == L1所有者范围重试边界::不适用 &&
            写入映射完整(*won.首次写入结果, ws);
        if (wonFirstComplete)
          knownFirst = won.首次写入结果->事实代次;
        if (!wonFirstComplete ||
            won.首次写入结果->新编码映射 != saved.新编码映射) {
          out.状态 = 场景角色数据状态::已可能发布;
          out.Gread = won.读取事实代次;
          out.首次发布代次 = knownFirst;
          return out;
        }
      }
      if (!wasReplay && saved.状态 != L1所有者范围写入状态::成功 &&
          saved.状态 != L1所有者范围写入状态::精确重复) {
        out.状态 = 映射场景写入状态(saved, 场景角色数据状态::已登记,
                                    port.所有者身份(), ws);
        out.Gread = saved.事实代次;
        out.首次发布代次 =
            out.状态 == 场景角色数据状态::已可能发布 ? saved.事实代次 : 0;
        return out;
      }
      if (wasReplay && (!first.首次写入结果 ||
                        saved.新编码映射 != first.首次写入结果->新编码映射)) {
        out.状态 = 场景角色数据状态::已可能发布;
        out.Gread = saved.事实代次;
        out.首次发布代次 =
            first.首次写入结果 ? first.首次写入结果->事实代次 : saved.事实代次;
        return out;
      }
      const auto firstHead = saved.状态 == L1所有者范围写入状态::成功 &&
                             saved.是否形成内存权威发布 &&
                             saved.重试边界 == L1所有者范围重试边界::不适用;
      const auto replayHead =
          saved.状态 == L1所有者范围写入状态::精确重复 &&
          !saved.是否形成内存权威发布 &&
          saved.重试边界 == L1所有者范围重试边界::原幂等身份读回收敛;
      if (saved.合同版本 != L1所有者范围CRUD合同版本 ||
          saved.所有者 != port.所有者身份() ||
          saved.写入幂等身份 != r.幂等身份 || saved.事实代次 != r.G0 + 1 ||
          (!firstHead && !replayHead)) {
        const bool priorOrPublished =
            wasReplay || saved.状态 == L1所有者范围写入状态::精确重复 ||
            saved.是否形成内存权威发布;
        out.状态 = priorOrPublished ? 场景角色数据状态::已可能发布
                                    : 场景角色数据状态::内部不一致;
        out.首次发布代次 = priorOrPublished ? knownFirst : 0;
        return out;
      }
      std::array<稳定编码, 7> ids{};
      if (saved.新编码映射.size() != 7) {
        out.状态 = 场景角色数据状态::已可能发布;
        return out;
      }
      for (const auto &m : saved.新编码映射) {
        if (m.first.值 < 1 || m.first.值 > 7 || 有效(ids[m.first.值 - 1])) {
          out.状态 = 场景角色数据状态::已可能发布;
          return out;
        }
        ids[m.first.值 - 1] = m.second;
      }
      if (std::ranges::any_of(ids, [](auto x) { return !有效(x); })) {
        out.状态 = 场景角色数据状态::已可能发布;
        return out;
      }
      out.交付 =
          场景角色结构交付{{ids[0], ids[1], ids[2]}, ids[3], ids[4], ids[5],
                           ids[6]};
      out.首次发布代次 = saved.事实代次;
      const auto guard = l1.读取中性当前事实代次({L1中性CRUD合同版本});
      if (guard.状态 != L1中性读取状态::成功 ||
          guard.合同版本 != L1中性CRUD合同版本 || !guard.事实代次 ||
          guard.事实代次 < saved.事实代次) {
        out.状态 = 场景角色数据状态::已可能发布;
        out.Gread = guard.事实代次;
        return out;
      }
      out.Gread = guard.事实代次;
      for (const auto id : ids) {
        const auto q =
            l1.读取所有者范围历史事实({L1所有者范围CRUD合同版本, id});
        if (q.状态 != L1所有者范围读取状态::成功 ||
            q.合同版本 != L1所有者范围CRUD合同版本 || q.查询编码 != id ||
            q.读取事实代次 != out.Gread || !q.事实 || q.物理清理事实代次 ||
            q.物理清理墓碑) {
          out.状态 = 场景角色数据状态::已可能发布;
          return out;
        }
        const auto *n = std::get_if<L1所有者范围节点事实>(&*q.事实);
        if (!n || n->写入所有者 != port.所有者身份() ||
            n->种类 != 节点种类::普通 || n->属性类型表示 ||
            !n->当前属性.empty() || n->退出事实代次) {
          out.状态 = 场景角色数据状态::已可能发布;
          return out;
        }
      }
      const auto finalGuard = l1.读取中性当前事实代次({L1中性CRUD合同版本});
      if (finalGuard.状态 != L1中性读取状态::成功 ||
          finalGuard.合同版本 != L1中性CRUD合同版本 ||
          finalGuard.事实代次 != out.Gread) {
        out.状态 = 场景角色数据状态::已可能发布;
        out.Gread = finalGuard.事实代次;
        return out;
      }
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


  场景当前身份结果 确认当前场景角色(const 场景当前身份请求 &r) const override;
  场景角色历史结果 读取场景角色历史(const 场景历史身份请求 &r) const override;
  场景角色写结果 启用场景角色(const 场景角色启用请求 &r,
                              const 直接归属联合只读提供者 &);
  场景角色写结果 退出场景角色(const 场景角色退出请求 &r);
  场景父语境读取结果
  读取当前父场景语境(const 场景父语境读取请求 &r) const override;

  状态使用绑定创建结果 创建状态使用绑定(const 状态使用绑定创建请求 &r);
  状态使用绑定读取结果
  读取当前状态使用绑定(const 状态使用绑定当前读取请求 &) const override;
  状态使用绑定读取结果
  读取状态使用绑定历史(const 状态使用绑定历史读取请求 &) const override;
  状态使用绑定组查询结果 按发生场景查询当前状态使用绑定组(
      const 状态使用绑定按发生场景查询请求 &) const;
  状态使用绑定组查询结果 按被描述存在查询当前状态使用绑定组(
      const 状态使用绑定按被描述存在查询请求 &) const;
  状态使用绑定组查询结果
  按状态查询当前状态使用绑定组(const 状态使用绑定按状态查询请求 &) const;
  状态使用绑定退出结果 退出状态使用绑定(const 状态使用绑定退出请求 &);

  场景组织写结果_v2 组织状态实例(const 场景状态组织请求 &);
  场景组织写结果_v2 组织动态实例(const 场景动态组织请求 &,
                                 const 动态结构只读提供者 &);
  场景组织历史结果 读取实例组织历史(const 场景组织历史请求 &) const;
  场景动态组织历史结果
  读取动态场景组织历史(const 场景动态组织历史请求 &) const override;
  场景特征组织写结果_v1 组织特征实例(const 场景特征组织请求 &);
  场景特征组织历史结果 读取特征组织历史(const 场景特征组织历史请求 &) const;

  bool 使用存在提供者(const 存在结构身份只读提供者 &provider) const noexcept {
    return &provider == &existence_;
  }

  已发布概念绑定创建结果 添加场景节点(
      const 已发布概念绑定创建请求 &r, 绑定存在数据服务 &binding,
      已发布概念引用参与者 &concepts) noexcept {
    if(r.绑定.种类!=存在初始绑定种类::直接子场景||
       !binding.使用场景提供者(*this)) return {};
    return binding.创建绑定存在并引用概念(r,concepts);
  }

  场景直接包含组结果
  读取当前场景包含父组(const 场景直接包含反向读取请求 &) const override;
  场景直接包含组结果
  读取当前场景包含子组(const 场景直接包含组读取请求 &) const override;
  场景直接包含当前完整读取结果_v2
  读取当前场景包含父组_v2(
      const 场景直接包含父组当前完整读取请求_v2 &) const override;
  场景直接包含当前完整读取结果_v2
  读取当前场景包含子组_v2(
      const 场景直接包含子组当前完整读取请求_v2 &) const override;
  直接归属场景角色读取结果
  读取当前场景角色位置(const 直接归属场景角色读取请求 &) const override;
  场景树角色写结果 启用并建立场景树根(const 场景树根启用请求 &,
                                      const 直接归属联合只读提供者 &);
  场景树角色写结果 启用并接纳直接子场景(const 场景直接子场景启用请求 &,
                                        const 直接归属联合只读提供者 &);
  场景直接包含单项结果 新增直接存在成员(const 场景直接包含写请求 &,
                                        const 直接归属联合只读提供者 &);
  场景直接包含单项结果 退出直接存在成员(const 场景直接包含写请求 &,
                                        const 直接归属联合只读提供者 &);
  场景直接包含迁移结果 迁移直接存在成员(const 场景直接包含迁移请求 &,
                                             const 直接归属联合只读提供者 &);
  场景直接包含组结果
  读取当前直接存在成员组(const 场景直接包含组读取请求 &) const;
  场景直接包含单项结果
  读取当前直接存在成员父(const 场景直接包含反向读取请求 &) const;
  场景直接包含单项结果
  读取直接存在成员历史(const 场景直接包含历史读取请求 &) const;
  场景直接包含迁移结果 迁移直接子场景(const 场景直接包含迁移请求 &,
                                           const 直接归属联合只读提供者 &);
  场景直接包含组结果
  读取当前直接子场景组(const 场景直接包含组读取请求 &) const;
  场景直接包含单项结果
  读取当前直接子场景父(const 场景直接包含反向读取请求 &) const;
  场景直接包含单项结果
  读取直接子场景历史(const 场景直接包含历史读取请求 &) const;
  场景直接包含单项结果 读取直接包含历史(const 场景直接包含历史读取请求 &) const;
  场景树当前结果 读取当前场景树(const 场景树当前读取请求 &,
                                const 直接归属联合只读提供者 &) const;

private:
  friend class ::海中鱼巣::世界树根数据服务;
  L1所有者范围写端口 &世界树根协调端口() noexcept { return port_; }
  struct 关系读取 {
    场景角色数据状态 状态 = 场景角色数据状态::内部不一致;
    std::uint64_t Gread = 0;
    std::vector<L1所有者范围关系事实> 关系;
    bool 成功() const noexcept { return 状态 == 场景角色数据状态::已读取; }
  };
  struct 代次读取 {
    场景角色数据状态 状态 = 场景角色数据状态::内部不一致;
    std::uint64_t Gread = 0;
    bool 成功() const noexcept { return 状态 == 场景角色数据状态::已读取; }
  };
  struct 节点读取 {
    场景角色数据状态 状态 = 场景角色数据状态::内部不一致;
    std::uint64_t Gread = 0;
    std::optional<L1所有者范围节点事实> 事实;
    bool 成功() const noexcept {
      return 状态 == 场景角色数据状态::已读取 && 事实.has_value();
    }
  };
  struct 关系事实读取 {
    场景角色数据状态 状态 = 场景角色数据状态::内部不一致;
    std::uint64_t Gread = 0;
    std::optional<L1所有者范围关系事实> 事实;
    bool 成功() const noexcept {
      return 状态 == 场景角色数据状态::已读取 && 事实.has_value();
    }
  };
  static bool 布局形状有效(const 场景角色结构交付 &) noexcept;
  static bool 包含布局形状有效(const 场景直接包含扩展结构交付 &,
                               const 场景角色结构交付 &,
                               const 场景特征组织扩展结构交付 &) noexcept;
  bool 登记材料匹配() const;
  bool 扩展登记材料匹配() const;
  bool 包含扩展登记材料匹配() const;
  关系读取 查询关系(std::uint64_t, std::uint64_t, L1所有者范围关系端点方向,
                    稳定编码, 稳定编码) const;
  节点读取 读节点(稳定编码, std::uint64_t) const;
  关系事实读取 读关系(稳定编码, std::uint64_t) const;
  场景角色历史结果 读角色(std::uint64_t, std::uint64_t, 稳定编码) const;
  状态使用绑定读取结果 读B(std::uint64_t, std::uint64_t,
                           状态使用绑定身份) const;
  状态使用绑定组查询结果 查B组(std::uint32_t, std::uint64_t, 稳定编码,
                               std::int64_t, std::uint64_t) const;
  场景组织历史结果 读路径(std::uint64_t, std::uint64_t, 场景根角色, 稳定编码,
                          std::uint64_t) const;
  场景特征组织历史结果 读特征路径(std::uint64_t, std::uint64_t, 特征信息身份,
                                  std::uint64_t,bool* =nullptr) const;
  场景角色数据状态 核验当前已知准确特征(const 场景特征组织请求 &,
                                        std::uint64_t &) const;
  场景角色数据状态 核验特征组织父(const 场景特征组织请求 &, 稳定编码,
                                  std::uint64_t &) const;
  场景角色数据状态 核验特征未组织(const 场景特征组织请求 &,
                                  std::uint64_t &) const;
  场景角色数据状态 核验特征组织前置(const 场景特征组织请求 &,
                                    std::uint64_t &) const;
  场景特征组织写结果_v1 读回特征组织(const 场景特征组织请求 &, 场景角色数据状态,
                                     const L1所有者范围写入结果 &) const;
  场景组织写结果_v2 组织(std::uint32_t, std::uint64_t, L1所有者范围写入幂等身份,
                         稳定编码, 稳定编码, 稳定编码, std::uint64_t,
                         场景根角色);
  static 场景节点见证 转节点(const L1所有者范围节点事实 &n) {
    return {n.编码, {n.创建事实代次, n.退出事实代次}};
  }
  static 场景组织边见证 转边(const L1所有者范围关系事实 &e) {
    return {e.编码,
            e.源节点,
            e.目标节点,
            e.关系类型节点,
            static_cast<std::uint64_t>(e.角色或顺序),
            {e.创建事实代次, e.退出事实代次}};
  }
  static 状态使用绑定数据状态 场景到B(场景角色数据状态) noexcept;
  static 状态使用绑定数据状态 存在到B(存在结构身份只读状态) noexcept;
  static 状态使用绑定数据状态 状态到B(状态类数据状态) noexcept;
  static 场景角色数据状态 状态到场景(状态类数据状态) noexcept;
  static 场景角色数据状态 动态到场景(动态数据状态) noexcept;
  static 场景角色数据状态 存在特征到场景(存在已知准确特征只读状态) noexcept;
  static 场景角色数据状态 存在到场景(存在结构身份只读状态) noexcept;
  static 场景角色数据状态
  映射场景写入状态(const L1所有者范围写入结果 &, 场景角色数据状态,
                   L1结构所有者身份, const L1所有者范围写集请求 &) noexcept;
  static 状态使用绑定数据状态
  映射绑定写入状态(const L1所有者范围写入结果 &, 状态使用绑定数据状态,
                   L1结构所有者身份, const L1所有者范围写集请求 &) noexcept;
  代次读取 读取当前代次() const;
  代次读取 核验代次(std::uint64_t) const;
  bool 写入头完整(const L1所有者范围写入结果 &, L1所有者范围写入幂等身份,
                  std::uint64_t) const noexcept;
  static bool 写入失败头完整(const L1所有者范围写入结果 &, L1结构所有者身份,
                             const L1所有者范围写集请求 &) noexcept;
  static bool 写入映射完整(const L1所有者范围写入结果 &,
                           const L1所有者范围写集请求 &) noexcept;
  bool 竞争精确重复材料完整(const L1所有者范围写入结果 &,
                            const L1所有者范围写集请求 &) const;
  static bool 写集相同(const L1所有者范围写集请求 &,
                       const L1所有者范围写集请求 &) noexcept;
  static std::optional<稳定编码> 映射编码(const L1所有者范围写入结果 &,
                                          L1所有者范围写集本地键);
  L1所有者范围写入结果 串行提交(const L1所有者范围写集请求 &);
  L1所有者范围写集请求 形成启用写集(std::uint64_t,
                                        L1所有者范围写入幂等身份,
                                        稳定编码) const;
  L1所有者范围写集请求 形成绑定写集(const 状态使用绑定创建请求 &) const;
  L1所有者范围写集请求 形成组织写集(std::uint64_t, L1所有者范围写入幂等身份,
                                    稳定编码, 稳定编码, 场景根角色) const;
  L1所有者范围写集请求 形成特征组织写集(std::uint64_t, L1所有者范围写入幂等身份,
                                        稳定编码, 特征信息身份) const;
  static bool 普通幂等身份有效(L1所有者范围写入幂等身份) noexcept;
  场景直接包含组结果
  读取当前直接包含组(const 场景直接包含组读取请求 &,
                       场景直接包含种类) const;
  场景直接包含单项结果
  读取当前直接包含父(const 场景直接包含反向读取请求 &,
                       场景直接包含种类) const;
  场景直接包含单项结果
  读取直接包含历史(const 场景直接包含历史读取请求 &,
                     场景直接包含种类) const;
  场景直接包含状态 校验联合闭包(std::uint64_t, 稳定编码, std::uint64_t,
                                 std::uint64_t, const 直接归属联合只读提供者 &,
                                 std::optional<稳定编码>) const;
  std::optional<场景树角色写结果>
  尝试重放树角色(std::uint64_t, L1所有者范围写入幂等身份, 稳定编码,
                   std::optional<稳定编码>, std::optional<稳定编码>, bool,
                   std::uint64_t, std::uint64_t,
                   const 直接归属联合只读提供者 &) const;
  场景树角色写结果 启用树角色核心(std::uint64_t, L1所有者范围写入幂等身份,
                                  稳定编码, std::optional<稳定编码>,
                                  std::optional<稳定编码>,
                                  std::optional<直接归属联合事实>,
                                  std::optional<稳定编码>, bool, std::uint64_t,
                                  std::uint64_t,
                                  const 直接归属联合只读提供者 &);
  场景直接包含单项结果 变更直接存在成员核心(const 场景直接包含写请求 &,
                                            const 直接归属联合只读提供者 &,
                                            bool);
  场景直接包含迁移结果 迁移直接包含核心(const 场景直接包含迁移请求 &,
                                        const 直接归属联合只读提供者 &,
                                        场景直接包含种类);

  using 绑定S = 绑定存在创建状态;
  const L1事实基座服务 &绑定存在底座() const noexcept override { return l1_; }
  const L1所有者范围写端口 &绑定存在端口() const noexcept override {
    return port_;
  }
  bool 绑定存在结构已就绪() const noexcept override {
    return 场景结构登记已就绪();
  }
  bool 绑定存在幂等键可用(L1所有者范围写入幂等身份 k) const noexcept override {
    return k.值 && k.值 != 1 && k != 场景特征组织扩展登记固定幂等身份 &&
           k != 场景直接包含扩展登记固定幂等身份 && (k.值 >> 48) != 0x4E43;
  }
  const 场景直接包含只读提供者 &绑定存在场景提供者() const noexcept override {
    return *this;
  }
  static 绑定S 绑定映射(直接归属联合只读状态 s) noexcept {
    using X = 直接归属联合只读状态;
    switch (s) {
    case X::成员未找到:
      return 绑定S::绑定未找到;
    case X::成员已退出:
      return 绑定S::绑定已退出;
    case X::事实代次漂移:
      return 绑定S::事实代次漂移;
    case X::入口拒绝:
      return 绑定S::绑定类型不符;
    case X::数量预算不足:
      return 绑定S::数量预算不足;
    case X::历史材料已清理:
      return 绑定S::历史材料已清理;
    case X::资源失败:
      return 绑定S::资源失败;
    default:
      return 绑定S::内部不一致;
    }
  }
  static 绑定S 绑定映射(场景直接包含状态 state) noexcept {
    using X = 场景直接包含状态;
    switch (state) {
    case X::场景未找到:
    case X::成员未找到:
      return 绑定S::绑定未找到;
    case X::场景已退出:
    case X::成员已退出:
      return 绑定S::绑定已退出;
    case X::入口拒绝:
    case X::引用冲突:
      return 绑定S::绑定类型不符;
    case X::成员已归属:
    case X::成员多重归属:
      return 绑定S::包含冲突;
    case X::形成场景环:
      return 绑定S::成环;
    case X::事实代次漂移:
      return 绑定S::事实代次漂移;
    case X::数量预算不足:
      return 绑定S::数量预算不足;
    case X::历史材料已清理:
      return 绑定S::历史材料已清理;
    case X::资源失败:
      return 绑定S::资源失败;
    default:
      return 绑定S::内部不一致;
    }
  }
  static 绑定S 绑定映射(场景角色数据状态 state) noexcept {
    using X = 场景角色数据状态;
    switch (state) {
    case X::未找到:
      return 绑定S::绑定未找到;
    case X::目标已退出:
      return 绑定S::绑定已退出;
    case X::入口拒绝:
    case X::场景角色未启用:
    case X::引用冲突:
      return 绑定S::绑定类型不符;
    case X::事实代次漂移:
      return 绑定S::事实代次漂移;
    case X::数量预算不足:
      return 绑定S::数量预算不足;
    case X::历史材料已清理:
      return 绑定S::历史材料已清理;
    case X::资源失败:
      return 绑定S::资源失败;
    default:
      return 绑定S::内部不一致;
    }
  }
  template <class T, class F>
  绑定存在参与者结果<T> 绑定保护(std::uint64_t g, std::uint64_t h,
                                 F &&fn) const {
    绑定存在参与者结果<T> out;
    out.Gread = g;
    out.H = h;
    try {
      auto before = 读取当前代次();
      if (!before.成功())
        throw 绑定映射(before.状态);
      if (before.Gread != g)
        throw 绑定S::事实代次漂移;
      out.数据 = fn();
      const auto after = 读取当前代次();
      if (!after.成功())
        throw 绑定映射(after.状态);
      if (after.Gread != g)
        throw 绑定S::事实代次漂移;
      out.状态 = 绑定S::精确重复;
    } catch (绑定S e) {
      out.状态 = e;
      out.数据.reset();
    } catch (const std::bad_alloc &) {
      out.状态 = 绑定S::资源失败;
      out.数据.reset();
    } catch (const std::length_error &) {
      out.状态 = 绑定S::资源失败;
      out.数据.reset();
    } catch (...) {
      out.状态 = 绑定S::内部不一致;
      out.数据.reset();
    }
    return out;
  }
  绑定存在参与者结果<L1所有者范围首次写入读取结果>
  读取场景绑定首次材料(L1所有者范围写入幂等身份 k) const override {
    auto f = port_.读取首次写入材料({L1所有者范围首次写入读取合同版本, k});
    return {绑定S::精确重复, f.读取事实代次, f.读取事实代次, f};
  }
  绑定存在参与者结果<绑定现实树见证>
  核验绑定现实树(std::uint64_t g, const 存在初始绑定 &b, 稳定编码 root,
                 const 绑定存在创建预算 &budget,
                 const 直接归属联合只读提供者 &joint) const override {
    return 绑定保护<绑定现实树见证>(g, g, [&] {
      if (!joint.绑定于(l1_))
        throw 绑定S::入口拒绝;
      auto rp = joint.读取当前场景角色位置({1, g, root});
      if (!rp.成功({1, g, root}))
        throw 绑定映射(rp.状态);
      if (!rp.角色 || rp.角色->位置 != 直接归属场景位置::场景树根 ||
          rp.角色->树根 != root)
        throw 绑定S::绑定不在现实树;
      auto bp = joint.读取当前场景角色位置({1, g, b.绑定节点});
      if (!bp.成功({1, g, b.绑定节点}))
        throw 绑定映射(bp.状态);
      if (b.种类 != 存在初始绑定种类::父存在组成 &&
          (!bp.角色 || bp.角色->位置 == 直接归属场景位置::未纳入场景树))
        throw 绑定S::绑定类型不符;
      绑定现实树见证 out{g, b.绑定节点, root, *rp.角色, {}};
      std::unordered_set<std::uint64_t> seen;
      auto cursor = b.绑定节点;
      std::uint64_t scenes = 0;
      for (;;) {
        if (!seen.insert(cursor.值).second)
          throw 绑定S::成环;
        auto role = joint.读取当前场景角色位置({1, g, cursor});
        if (!role.成功({1, g, cursor}))
          throw 绑定映射(role.状态);
        if (role.角色) {
          if (++scenes > budget.最大场景数量)
            throw 绑定S::数量预算不足;
          if (role.角色->树根 != root)
            throw 绑定S::绑定不在现实树;
        }
        auto parent = joint.读取当前联合父({1, g, cursor, budget.最大关系数量});
        if (!parent.父读取成功({1, g, cursor, budget.最大关系数量}))
          throw 绑定映射(parent.状态);
        if (cursor == root) {
          if (parent.父)
            throw 绑定S::包含冲突;
          break;
        }
        if (!parent.父)
          throw 绑定S::绑定不在现实树;
        if (out.上行路径.size() >= budget.最大祖先数量 ||
            out.上行路径.size() >= budget.最大关系数量)
          throw 绑定S::数量预算不足;
        out.上行路径.push_back(*parent.父);
        cursor = parent.父->父;
      }
      return out;
    });
  }
  绑定存在参与者结果<L1有限N分区原子参与者写集_v3>
  准备场景绑定片段(const 绑定存在创建请求 &r, std::uint64_t g) const override {
    return 绑定保护<L1有限N分区原子参与者写集_v3>(g, r.G0, [&] {
      auto parent = 读角色(g, r.G0, r.绑定.绑定节点);
      if (!parent.成功({2, g, r.G0, r.绑定.绑定节点}))
        throw 绑定映射(parent.状态);
      auto key = std::get<存在场景绑定创建键>(r.幂等键).场景幂等身份;
      L1有限N分区原子参与者写集_v3 p{
          {2}, owner_, {L1所有者范围CRUD合同版本, r.G0, key}};
      const L1有限N分区原子事实引用_v3 newborn{{1}, {1}};
      if (r.绑定.种类 == 存在初始绑定种类::场景成员) {
        p.写集.关系.push_back({{1},
                               r.绑定.绑定节点,
                               newborn,
                               includeLayout_.直接存在成员关系类型,
                               1});
        return p;
      }
      if (r.绑定.种类 != 存在初始绑定种类::直接子场景)
        throw 绑定S::入口拒绝;
      for (std::uint32_t i = 1; i <= 4; ++i)
        p.写集.节点.push_back({{i}, 节点种类::普通, std::nullopt});
      p.写集.关系.push_back({{5},
                             newborn,
                             layout_.绑定结构.场景族锚点,
                             layout_.绑定结构.场景族归属关系类型,
                             1});
      for (std::uint32_t i = 1; i <= 4; ++i)
        p.写集.关系.push_back({{i + 5},
                               newborn,
                               L1所有者范围写集本地键{i},
                               layout_.根绑定关系类型,
                               static_cast<std::int64_t>(i)});
      auto markers = 查询关系(g, r.G0, L1所有者范围关系端点方向::源,
                              r.绑定.绑定节点, includeLayout_.根标记关系类型);
      auto membership =
          查询关系(g, r.G0, L1所有者范围关系端点方向::源, r.绑定.绑定节点,
                   includeLayout_.树归属关系类型);
      if (!markers.成功())
        throw 绑定映射(markers.状态);
      if (!membership.成功())
        throw 绑定映射(membership.状态);
      if (markers.关系.size() + membership.关系.size() != 1)
        throw 绑定S::内部不一致;
      const auto actualRoot = markers.关系.empty()
                                  ? membership.关系.front().目标节点
                                  : r.绑定.绑定节点;
      if (actualRoot != r.期望现实树根)
        throw 绑定S::绑定不在现实树;
      p.写集.关系.push_back(
          {{10}, newborn, actualRoot, includeLayout_.树归属关系类型, 1});
      p.写集.关系.push_back({{11},
                             r.绑定.绑定节点,
                             newborn,
                             includeLayout_.直接子场景关系类型,
                             1});
      p.写集.关系.push_back({{12}, newborn, r.绑定.绑定节点,
                             layout_.父场景语境关系类型, 1});
      return p;
    });
  }
  绑定存在参与者结果<场景绑定出生见证>
  读取场景绑定出生(std::uint64_t g, std::uint64_t h, 稳定编码 e,
                   const 存在初始绑定 &b) const override {
    return 绑定保护<场景绑定出生见证>(g, h, [&] {
      auto type = b.种类 == 存在初始绑定种类::场景成员
                      ? includeLayout_.直接存在成员关系类型
                      : includeLayout_.直接子场景关系类型;
      auto edges = 查询关系(g, h, L1所有者范围关系端点方向::目标, e, type);
      if (!edges.成功())
        throw 绑定映射(edges.状态);
      if (edges.关系.empty())
        throw 绑定S::绑定未找到;
      if (edges.关系.size() != 1)
        throw 绑定S::内部不一致;
      const auto &x = edges.关系.front();
      if (x.源节点 != b.绑定节点 || x.目标节点 != e || x.角色或顺序 != 1)
        throw 绑定S::内部不一致;
      auto bound = 读取直接包含历史({1, g, h, x.编码});
      if (!bound.成功())
        throw 绑定映射(bound.结果头.状态);
      if (!bound.包含)
        throw 绑定S::内部不一致;
      场景绑定出生见证 out{g, h, b.种类, b.绑定节点, e, *bound.包含, {}};
      if (b.种类 == 存在初始绑定种类::直接子场景) {
        auto role = 读角色(g, h, e);
        if (!role.成功({2, g, h, e}))
          throw 绑定映射(role.状态);
        auto proof = 查询关系(g, h, L1所有者范围关系端点方向::源, e,
                              includeLayout_.树归属关系类型);
        if (!proof.成功())
          throw 绑定映射(proof.状态);
        if (proof.关系.size() != 1)
          throw 绑定S::内部不一致;
        const auto &edge = proof.关系.front();
        场景树节点当前事实 node;
        node.场景角色 = *role.角色;
        node.树证明 = {g,
                       h,
                       场景树证明种类::树归属,
                       edge.编码,
                       e,
                       edge.目标节点,
                       {edge.编码,
                        e,
                        edge.目标节点,
                        edge.关系类型节点,
                        1,
                        {edge.创建事实代次, {}}}};
        node.直接父 =
            直接归属联合事实{g,        h, 直接归属来源::直接子场景, x.编码,
                             x.源节点, e, x.创建事实代次,           {}};
        node.从上游场景到本场景路径.push_back(*node.直接父);
        const auto contextEdges=查询关系(g,h,L1所有者范围关系端点方向::源,e,
                                           layout_.父场景语境关系类型);
        if(!contextEdges.成功()||contextEdges.关系.size()!=1)
          throw 绑定S::内部不一致;
        const auto &contextEdge=contextEdges.关系.front();
        if(contextEdge.源节点!=e||contextEdge.目标节点!=b.绑定节点||
           contextEdge.关系类型节点!=layout_.父场景语境关系类型||
           contextEdge.角色或顺序!=1)
          throw 绑定S::内部不一致;
        node.父语境投影=场景父语境投影事实{
            g,h,e,b.绑定节点,*node.直接父,转边(contextEdge)};
        out.子场景结构 = std::move(node);
      }
      return out;
    });
  }
  bool 场景结构登记已就绪() const noexcept {
    try {
      std::lock_guard<std::mutex> lock(write_);
      const auto g=l1_.读取中性当前事实代次({L1中性CRUD合同版本});
      if(g.状态!=L1中性读取状态::成功||!g.事实代次||!绑定于(l1_)||!port_.有效()
          ||!布局形状有效(layout_)||!包含布局形状有效(includeLayout_,layout_,featureLayout_)
          ||!登记材料匹配()||!扩展登记材料匹配()||!包含扩展登记材料匹配())return false;
      const auto tail=l1_.读取中性当前事实代次({L1中性CRUD合同版本});
      return tail.状态==L1中性读取状态::成功&&tail.事实代次==g.事实代次;
    }catch(...){return false;}
  }
  const L1事实基座服务& 原子I64底座() const noexcept override { return l1_; }
  L1所有者范围写端口& 原子I64端口() noexcept override { return port_; }
  bool 原子I64结构已就绪() const noexcept override { return 场景结构登记已就绪(); }
  原子I64特征参与结果<L1有限N分区原子参与者写集_v3>
  准备原子I64出生片段(const 原子I64特征出生请求& r,std::uint64_t g) const override {
    原子I64特征参与结果<L1有限N分区原子参与者写集_v3> out;
    out.Gread=g;out.H=r.G0;
    try {
      if(!r.G0||g!=r.G0||!有效(r.位置.场景)||!有效(r.位置.组织父)||
         !r.组织读取预算.最大路径长度||r.组织读取预算.最大路径长度>4096||!r.键.组织.值)
        throw 原子I64特征出生状态::入口拒绝;
      const auto map=[](场景角色数据状态 s) {
        using X=场景角色数据状态; using B=原子I64特征出生状态;
        switch(s) {case X::入口拒绝:return B::入口拒绝;case X::未找到:return B::未找到;
          case X::目标已退出:return B::目标已退出;case X::事实代次漂移:return B::事实代次漂移;
          case X::数量预算不足:return B::预算不足;case X::历史材料已清理:return B::历史材料不可用;
          case X::资源失败:return B::资源失败;case X::实例未组织:return B::位置冲突;
          default:return B::内部不一致;}
      };
      const auto role=读角色(g,r.G0,r.位置.场景);
      if(!role.成功({2,g,r.G0,r.位置.场景}))throw map(role.状态);
      const auto root=role.角色->四根[0].根.编码;
      std::size_t pathLength=0;
      if(r.位置.组织父!=root) {
        bool missingKnown=false;
        const auto path=读特征路径(g,r.G0,{r.位置.组织父},r.组织读取预算.最大路径长度,&missingKnown);
        if(missingKnown)throw 原子I64特征出生状态::位置冲突;
        if(!path.成功({1,g,r.G0,{r.位置.组织父},r.组织读取预算.最大路径长度}))throw map(path.状态);
        if(path.组织->场景角色.场景!=r.位置.场景||path.组织->根!=root)throw 原子I64特征出生状态::位置冲突;
        pathLength=path.组织->路径边.size();
      }
      if(pathLength>=r.组织读取预算.最大路径长度)throw 原子I64特征出生状态::预算不足;
      L1有限N分区原子参与者写集_v3 part;
      part.参与者={3};part.所有者=owner_;
      part.写集={L1所有者范围CRUD合同版本,r.G0,r.键.组织};
      part.写集.关系.push_back({{1},r.位置.组织父,
          L1三分区原子事实引用_v2{{1},{1}},featureLayout_.特征组织关系类型,1});
      out.数据=std::move(part);out.状态=原子I64特征出生状态::已创建;
    } catch(原子I64特征出生状态 s) { out.状态=s;out.数据.reset(); }
      catch(const std::bad_alloc&) { out.状态=原子I64特征出生状态::资源失败;out.数据.reset(); }
      catch(const std::length_error&) { out.状态=原子I64特征出生状态::资源失败;out.数据.reset(); }
      catch(...) { out.状态=原子I64特征出生状态::内部不一致;out.数据.reset(); }
    return out;
  }
  原子I64特征窄读取结果<原子I64特征组织事实> 读取原子I64组织(const 原子I64特征组织读取请求& r) const override {
    原子I64特征窄读取结果<原子I64特征组织事实> out;out.Gread=r.Gread;out.H=r.H;
    auto map=[](L1所有者范围读取状态 s) {using X=原子I64特征窄读取状态;switch(s) {
      case L1所有者范围读取状态::未找到:return X::未找到;case L1所有者范围读取状态::已退出:return X::目标已退出;
      case L1所有者范围读取状态::事实代次漂移:return X::事实代次漂移;case L1所有者范围读取状态::历史材料已清理:return X::历史材料不可用;
      case L1所有者范围读取状态::资源失败:return X::资源失败;default:return X::内部不一致;}};
    try {if(r.版本!=1||!r.Gread||!r.H||r.H>r.Gread||!有效(r.C)||!有效(r.P)||!有效(r.F)||!r.预算.最大路径长度)return out;
      const auto guard=[&]{const auto x=l1_.读取中性当前事实代次({L1中性CRUD合同版本});if(x.状态!=L1中性读取状态::成功||x.事实代次!=r.Gread)throw 原子I64特征窄读取状态::事实代次漂移;};guard();
      std::vector<原子I64特征场景路径边事实> reverse;std::unordered_set<std::uint64_t> seen;auto child=r.F;
      for(std::uint64_t depth=0;;++depth){if(!seen.insert(child.值).second)throw 原子I64特征窄读取状态::内部不一致;
        const auto roots=l1_.读取所有者范围历史关系组({L1所有者范围CRUD合同版本,L1所有者范围关系端点方向::目标,child,layout_.根绑定关系类型,r.H});
        if(roots.读取事实代次!=r.Gread)throw 原子I64特征窄读取状态::事实代次漂移;
        if(roots.状态!=L1所有者范围读取状态::成功)throw map(roots.状态);
        if(!roots.关系组.empty()){if(roots.关系组.size()!=1)throw 原子I64特征窄读取状态::内部不一致;const auto& e=roots.关系组.front();if(e.写入所有者!=owner_||e.源节点!=r.C||e.目标节点!=child||e.角色或顺序!=static_cast<std::int64_t>(场景根角色::特征)||e.创建事实代次>r.H||e.退出事实代次)throw 原子I64特征窄读取状态::位置冲突;break;}
        if(depth>=r.预算.最大路径长度)throw 原子I64特征窄读取状态::预算不足;const auto rows=l1_.读取所有者范围历史关系组({L1所有者范围CRUD合同版本,L1所有者范围关系端点方向::目标,child,featureLayout_.特征组织关系类型,r.H});
        if(rows.读取事实代次!=r.Gread)throw 原子I64特征窄读取状态::事实代次漂移;
        if(rows.状态!=L1所有者范围读取状态::成功)throw map(rows.状态);
        if(rows.关系组.size()!=1)throw 原子I64特征窄读取状态::位置冲突;const auto& e=rows.关系组.front();if(e.写入所有者!=owner_||e.目标节点!=child||e.关系类型节点!=featureLayout_.特征组织关系类型||e.角色或顺序!=1||e.创建事实代次>r.H||e.退出事实代次)throw 原子I64特征窄读取状态::内部不一致;reverse.push_back({e.编码,e.源节点,e.目标节点,e.关系类型节点,e.角色或顺序,e.创建事实代次});child=e.源节点;
      }
      std::reverse(reverse.begin(),reverse.end());if(reverse.empty()||reverse.back().源!=r.P||reverse.back().目标!=r.F)throw 原子I64特征窄读取状态::位置冲突;guard();
      const auto& last=reverse.back();out.事实=原子I64特征组织事实{r.C,r.P,r.F,last.关系,last.关系类型,std::move(reverse),last.创建H};out.状态=原子I64特征窄读取状态::已读取;
    }catch(原子I64特征窄读取状态 s){out.状态=s;out.事实.reset();}catch(...){out.状态=原子I64特征窄读取状态::内部不一致;out.事实.reset();}return out;
  }
  const L1事实基座服务 &l1_;
  L1所有者范围写端口 port_;
  L1结构所有者身份 owner_;
  场景角色结构交付 layout_;
  场景特征组织扩展结构交付 featureLayout_;
  场景直接包含扩展结构交付 includeLayout_;
  const 存在结构身份只读提供者 &existence_;
  const 存在组成结构只读提供者 &existenceStructure_;
  const 状态类数据服务 &state_;
  mutable std::mutex write_;
};


} // namespace 海中鱼巣
