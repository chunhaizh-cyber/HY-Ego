module;

#include <algorithm>
#include <cstdint>
#include <new>
#include <optional>
#include <stdexcept>
#include <utility>
#include <variant>
#include <vector>

export module 海中鱼巣.领域.数据服务.世界树根;

export import 海中鱼巣.领域.合同.世界树根;
export import 海中鱼巣.领域.数据服务.存在类;
export import 海中鱼巣.领域.数据服务.场景类;

export namespace 海中鱼巣 {

class 世界树根数据服务 final {
public:
  世界树根数据服务() = delete;
  世界树根数据服务(const 世界树根数据服务 &) = delete;
  世界树根数据服务 &operator=(const 世界树根数据服务 &) = delete;
  世界树根数据服务(世界树根数据服务 &&) = delete;
  世界树根数据服务 &operator=(世界树根数据服务 &&) = delete;

  世界树根数据服务(存在类数据服务 &e, 场景类数据服务 &s)
      : existence_(e), scene_(s), joint_(e, s) {
    if (&e.世界树根底座() != &s.世界树根底座() ||
        !e.世界树根结构已就绪() || !s.世界树根结构已就绪() ||
        !joint_.绑定于(e.世界树根底座()) ||
        e.世界树根所有者() == s.世界树根所有者())
      throw std::invalid_argument("world root participants");
  }

  世界树根初始化结果 初始化世界树根(const 世界树根初始化请求 &r) noexcept {
    世界树根初始化结果 out;
    out.原请求 = r;
    bool dispatched = false;
    try {
      if (!请求有效(r))
        return out;
      auto generation = 当前代次();
      if (!generation || *generation < r.G0) {
        out.状态 = 世界树根状态::事实代次漂移;
        out.Gread = generation.value_or(0);
        return out;
      }
      out.Gread = *generation;
      const 世界树根组读取请求 rootsRequest{
          世界树根合同版本, *generation, r.最大关系数量};
      const auto roots = scene_.读取当前世界树根组(rootsRequest);
      if (!roots.成功(rootsRequest)) {
        out.状态 = roots.状态;
        out.Gread = roots.Gread;
        return out;
      }
      if (roots.根组.size() > 1) {
        out.状态 = 世界树根状态::内部不一致;
        return out;
      }

      const auto ef = existence_.读取世界树根存在首次材料(r.存在幂等身份);
      const auto sf = scene_.读取世界树根场景首次材料(r.场景幂等身份);
      const auto materialState = [](const auto &x) {
        return x.状态 == L1所有者范围读取状态::成功 ? 1
             : x.状态 == L1所有者范围读取状态::未找到 ? 0
                                                               : -1;
      };
      const int em = materialState(ef), sm = materialState(sf);
      if (em < 0 || sm < 0) {
        out.状态 =
            ef.状态 == L1所有者范围读取状态::资源失败 ||
                    sf.状态 == L1所有者范围读取状态::资源失败
                ? 世界树根状态::资源失败
                : 世界树根状态::内部不一致;
        return out;
      }
      if (em != sm) {
        out.状态 = 世界树根状态::内部不一致;
        return out;
      }
      const bool replay = em == 1;
      if ((!roots.根组.empty() && !replay) || (roots.根组.empty() && replay)) {
        out.状态 = roots.根组.empty() ? 世界树根状态::内部不一致
                                     : 世界树根状态::已有不同根;
        return out;
      }
      if (!replay && *generation != r.G0) {
        out.状态 = 世界树根状态::事实代次漂移;
        return out;
      }

      auto ep = existence_.准备世界树根存在片段(r).写集;
      auto sp = scene_.准备世界树根场景片段(r).写集;
      if (ep.参与者.值 != 1 || sp.参与者.值 != 2 ||
          ep.所有者 != existence_.世界树根所有者() ||
          sp.所有者 != scene_.世界树根所有者()) {
        out.状态 = 世界树根状态::内部不一致;
        return out;
      }
      L1有限N分区原子事务请求_v3 request{
          L1有限N分区原子事务合同版本_v3, r.G0, r.组合幂等身份, {ep, sp}};
      std::vector<const L1所有者范围写端口 *> others{&scene_.世界树根协调端口()};
      dispatched = true;
      const auto saved =
          existence_.世界树根协调端口().提交有限N分区原子事务_v3(request, others);
      using LS = L1有限N分区原子事务状态_v3;
      if (saved.状态 != LS::已提交 && saved.状态 != LS::精确重复) {
        if (saved.状态 == LS::已可能发布 || saved.是否已确认形成内存权威发布 ||
            saved.重试边界 == L1所有者范围重试边界::原幂等身份读回收敛) {
          out.状态 = 世界树根状态::已可能发布;
          return out;
        }
        dispatched = false;
        switch (saved.状态) {
        case LS::事实代次漂移: out.状态 = 世界树根状态::事实代次漂移; break;
        case LS::幂等冲突: out.状态 = 世界树根状态::幂等冲突; break;
        case LS::资源失败: out.状态 = 世界树根状态::资源失败; break;
        case LS::入口拒绝: out.状态 = 世界树根状态::入口拒绝; break;
        default: out.状态 = 世界树根状态::内部不一致; break;
        }
        return out;
      }
      dispatched = false;
      if (saved.共同事实代次 != r.G0 + 1 || saved.参与者结果组.size() != 2) {
        out.状态 = 世界树根状态::内部不一致;
        return out;
      }
      out.首次发布H = saved.共同事实代次;
      const auto existenceFirst =
          existence_.读取世界树根存在首次材料(r.存在幂等身份);
      const auto sceneFirst = scene_.读取世界树根场景首次材料(r.场景幂等身份);
      const auto firstComplete = [&](const auto &first,
                                     const auto &participant) {
        const auto expected = 规范化根写集(participant, saved.参与者结果组);
        return first.状态 == L1所有者范围读取状态::成功 &&
               first.写入幂等身份 == participant.写集.写入幂等身份 &&
               first.首次规范化写集 && first.首次写入结果 &&
               expected && 根写集相同(*first.首次规范化写集, *expected) &&
               first.首次写入结果->状态 == L1所有者范围写入状态::成功 &&
               first.首次写入结果->事实代次 == saved.共同事实代次;
      };
      if (!firstComplete(existenceFirst, ep) || !firstComplete(sceneFirst, sp)) {
        out.状态 = 世界树根状态::已可能发布;
        return out;
      }
      稳定编码 root{};
      for (const auto &part : saved.参与者结果组) {
        if (part.参与者.值 != 1)
          continue;
        for (const auto &[key, id] : part.新编码映射)
          if (key.值 == 1)
            root = id;
      }
      if (!有效(root)) {
        out.状态 = 世界树根状态::内部不一致;
        return out;
      }
      auto current = 当前代次();
      if (!current) {
        out.状态 = 世界树根状态::内部不一致;
        return out;
      }
      out.Gread = *current;
      const auto source =
          existence_.读取世界树根存在来源(*current, *out.首次发布H, root);
      if (!source) {
        out.状态 = 世界树根状态::已可能发布;
        return out;
      }
      const 场景树当前读取请求 treeRequest{
          1, *current, root, r.最大场景数量, r.最大关系数量};
      const auto tree = scene_.读取当前场景树(treeRequest, joint_);
      if (!tree.成功(treeRequest) || !tree.树 || tree.树->场景组.size() != 1 ||
          tree.树->场景组.front().场景角色.场景 != root) {
        out.状态 = 世界树根状态::已可能发布;
        return out;
      }
      const 直接归属联合父读取请求 parentRequest{
          1, *current, root, r.最大关系数量};
      const auto parent = joint_.读取当前联合父(parentRequest);
      if (!parent.父读取成功(parentRequest) || parent.父) {
        out.状态 = 世界树根状态::已可能发布;
        return out;
      }
      out.事实 = 世界树根事实{
          root, *out.首次发布H, *current, tree.树->场景组.front()};
      out.状态 = saved.状态 == LS::精确重复 ? 世界树根状态::精确重复
                                            : 世界树根状态::已建立;
      if (!out.成功(r)) {
        out.事实.reset();
        out.状态 = 世界树根状态::已可能发布;
      }
    } catch (const std::bad_alloc &) {
      out.状态 = dispatched ? 世界树根状态::已可能发布 : 世界树根状态::资源失败;
    } catch (const std::length_error &) {
      out.状态 = dispatched ? 世界树根状态::已可能发布 : 世界树根状态::资源失败;
    } catch (...) {
      out.状态 = dispatched ? 世界树根状态::已可能发布
                             : 世界树根状态::内部不一致;
    }
    return out;
  }

private:
  static std::optional<L1所有者范围写集请求> 规范化根写集(
      const L1有限N分区原子参与者写集_v3 &participant,
      const std::vector<L1有限N分区原子参与者结果_v3> &results) {
    const auto convert = [&](const L1有限N分区原子事实引用值_v3 &ref)
        -> std::optional<L1所有者范围事实引用> {
      if (const auto *stable = std::get_if<稳定编码>(&ref))
        return *stable;
      if (const auto *local =
              std::get_if<L1所有者范围写集本地键>(&ref))
        return *local;
      const auto &cross = std::get<L1有限N分区原子事实引用_v3>(ref);
      if (!cross.参与者.值)
        return std::nullopt;
      const auto found = std::find_if(
          results.begin(), results.end(), [&](const auto &x) {
            return x.参与者 == cross.参与者;
          });
      if (found == results.end())
        return std::nullopt;
      const auto mapped = std::find_if(
          found->新编码映射.begin(), found->新编码映射.end(),
          [&](const auto &x) { return x.first == cross.本地键; });
      if (mapped == found->新编码映射.end())
        return std::nullopt;
      return mapped->second;
    };
    L1所有者范围写集请求 out{participant.写集.合同版本,
                                participant.写集.期望事实代次,
                                participant.写集.写入幂等身份};
    if (!participant.写集.值.empty() ||
        !participant.写集.属性槽变更.empty())
      return std::nullopt;
    out.退出事实 = participant.写集.退出事实;
    for (const auto &node : participant.写集.节点)
      out.节点.push_back({node.本地键, node.种类, node.属性类型表示});
    for (const auto &relation : participant.写集.关系) {
      const auto source = convert(relation.源节点);
      const auto target = convert(relation.目标节点);
      const auto type = convert(relation.关系类型节点);
      if (!source || !target || !type)
        return std::nullopt;
      out.关系.push_back(
          {relation.本地键, *source, *target, *type, relation.角色或顺序});
    }
    return out;
  }

  static bool 根写集相同(const L1所有者范围写集请求 &a,
                         const L1所有者范围写集请求 &b) noexcept {
    const auto refEqual = [](const L1所有者范围事实引用 &x,
                             const L1所有者范围事实引用 &y) noexcept {
      if (x.index() != y.index())
        return false;
      if (const auto *stable = std::get_if<稳定编码>(&x)) {
        const auto *other = std::get_if<稳定编码>(&y);
        return other && *stable == *other;
      }
      const auto *local = std::get_if<L1所有者范围写集本地键>(&x);
      const auto *other = std::get_if<L1所有者范围写集本地键>(&y);
      return local && other && *local == *other;
    };
    if (a.合同版本 != b.合同版本 || a.期望事实代次 != b.期望事实代次 ||
        a.写入幂等身份 != b.写入幂等身份 ||
        a.节点.size() != b.节点.size() || a.关系.size() != b.关系.size() ||
        !a.值.empty() || !b.值.empty() || !a.属性槽变更.empty() ||
        !b.属性槽变更.empty() || a.退出事实 != b.退出事实)
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
          !refEqual(x.源节点, y.源节点) ||
          !refEqual(x.目标节点, y.目标节点) ||
          !refEqual(x.关系类型节点, y.关系类型节点))
        return false;
    }
    return true;
  }

  bool 请求有效(const 世界树根初始化请求 &r) const noexcept {
    return r.版本 == 世界树根合同版本 && r.G0 && r.G0 < UINT64_MAX &&
           r.组合幂等身份.值 && r.存在幂等身份.值 &&
           r.场景幂等身份.值 && r.组合幂等身份 != r.存在幂等身份 &&
           r.组合幂等身份 != r.场景幂等身份 &&
           r.存在幂等身份 != r.场景幂等身份 && r.最大关系数量 >= 1 &&
           r.最大关系数量 <= 4096 && r.最大场景数量 >= 1 &&
           r.最大场景数量 <= 4096 && existence_.世界树根结构已就绪() &&
           scene_.世界树根结构已就绪() &&
           existence_.世界树根幂等键可用(r.存在幂等身份) &&
           scene_.世界树根幂等键可用(r.场景幂等身份) &&
           (r.组合幂等身份.值 >> 48) != 0x4E43;
  }
  std::optional<std::uint64_t> 当前代次() const noexcept {
    const auto r = existence_.世界树根底座().读取中性当前事实代次(
        {L1中性CRUD合同版本});
    if (r.状态 != L1中性读取状态::成功 || !r.事实代次)
      return std::nullopt;
    return r.事实代次;
  }

  存在类数据服务 &existence_;
  场景类数据服务 &scene_;
  直接归属联合只读组合器 joint_;
};

} // namespace 海中鱼巣

