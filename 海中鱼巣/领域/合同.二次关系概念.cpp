#include "合同.二次关系概念.h"

#include <algorithm>
#include <set>

namespace 海中鱼巣 {
namespace {

bool 约束完整(const 二次关系概念约束& value) noexcept {
  const auto role = static_cast<unsigned>(value.角色);
  return role >= 1 && role <= 3 && 有效(value.概念.值);
}

bool 约束小于(const 二次关系概念约束& left,
              const 二次关系概念约束& right) noexcept {
  if (left.角色 != right.角色)
    return static_cast<unsigned>(left.角色) < static_cast<unsigned>(right.角色);
  return left.概念.值.值 < right.概念.值.值;
}

bool 约束组完整(const 二次关系约束组& group) noexcept {
  const auto valid = [](const auto& values) noexcept {
    if (!std::all_of(values.begin(), values.end(), 约束完整)) return false;
    for (std::size_t i = 1; i < values.size(); ++i)
      if (!约束小于(values[i - 1], values[i])) return false;
    return true;
  };
  return valid(group.FC) && valid(group.EC);
}

bool 原子完整(const 二次关系原子定义& atom) noexcept {
  return atom.D.掩码 >= 1 && atom.D.掩码 <= 7 &&
      约束组完整(atom.约束) && 有效(atom.K) &&
      atom.输出 == 特征类标量结果角色::差异;
}

bool 定义完整(const 二次关系定义& definition) noexcept {
  if (const auto* atom = std::get_if<二次关系原子定义>(&definition))
    return 原子完整(*atom);
  const auto* all = std::get_if<二次关系合取定义>(&definition);
  if (!all || !约束组完整(all->共同约束) || all->条件组.empty()) return false;
  std::uint64_t prior = 0;
  for (const auto& item : all->条件组) {
    if (!有效(item.子RC.值) || !约束组完整(item.附加约束) ||
        item.子RC.值.值 <= prior) return false;
    prior = item.子RC.值.值;
  }
  return true;
}

bool 规范形完整(const 二次关系规范形& form) noexcept {
  if (form.版本 != 1 || form.原子组.empty()) return false;
  for (const auto& atom : form.原子组)
    if (!原子完整(atom)) return false;
  return true;
}

bool 结构类型完整(const 二次关系结构类型& types) noexcept {
  const 稳定编码 values[] = {
      types.规范化规则归属, types.规则版本, types.定义种类, types.定义格式,
      types.域掩码, types.输出角色, types.固定K, types.约束成员, types.约束FC,
      types.约束EC, types.合取成员, types.子RC, types.来源成员, types.来源F,
      types.来源B, types.来源概念, types.用途成员, types.用途目标,
      types.用途业务依据, types.用途业务标识, types.用途角色, types.用途时间};
  std::set<std::uint64_t> unique;
  for (const auto value : values)
    if (!有效(value) || !unique.insert(value.值).second) return false;
  return true;
}

bool 结构交付完整(const 二次关系结构交付& layout) noexcept {
  return 有效(layout.锚点) && 有效(layout.规范化规则.值) &&
      结构类型完整(layout.类型);
}

bool 发布见证完整(const 二次关系发布见证& witness) noexcept {
  return 有效(witness.幂等身份) && witness.已确认发布;
}

bool 事实完整(const 二次关系概念事实& fact) noexcept {
  if (!有效(fact.身份.值) || fact.类别 != 相关概念类别::特征 ||
      !有效(fact.规则.值) || !定义完整(fact.定义) ||
      !规范形完整(fact.规范形)) return false;
  if (fact.治理状态 != 概念树生命周期状态::活跃 &&
      fact.治理状态 != 概念树生命周期状态::冷却 &&
      fact.治理状态 != 概念树生命周期状态::退役) return false;
  return true;
}

bool 来源完整(const 二次关系来源事实& fact) noexcept {
  return 有效(fact.记录) && 有效(fact.成员关系) && 有效(fact.目标关系) &&
      有效(fact.RC.值) && !fact.来源.来源.valueless_by_exception();
}

bool 用途完整(const 二次关系用途事实& fact) noexcept {
  return 有效(fact.记录) && 有效(fact.成员关系) && 有效(fact.概念关系) &&
      有效(fact.依据关系) && 有效(fact.RC.值) && fact.业务标识 &&
      fact.用途角色 && 有效(fact.业务依据);
}

bool 边完整(const 二次关系关系见证& edge) noexcept {
  return 有效(edge.编码) && 有效(edge.源) && 有效(edge.目标) && 有效(edge.类型);
}

} // namespace

bool 二次关系初始化结果::成功() const noexcept {
  return (状态 == 二次关系数据状态::已创建 ||
          状态 == 二次关系数据状态::已复用 ||
          状态 == 二次关系数据状态::精确重放) &&
      正式回执 && 发布见证完整(*正式回执) && 交付 && 结构交付完整(*交付);
}

bool 二次关系定义核验结果::成功() const noexcept {
  return 状态 == 二次关系数据状态::已读取 && 规范形 && 规范形完整(*规范形);
}

bool 二次关系概念读取结果::成功() const noexcept {
  return 状态 == 二次关系数据状态::已读取 && 事实 && 事实完整(*事实);
}

bool 二次关系概念写入结果::成功() const noexcept {
  return (状态 == 二次关系数据状态::已创建 ||
          状态 == 二次关系数据状态::已复用 ||
          状态 == 二次关系数据状态::精确重放) &&
      正式回执 && 发布见证完整(*正式回执) && 事实 && 事实完整(*事实) &&
      建立原请求 && 定义完整(建立原请求->定义);
}

bool 二次关系关联结果::成功() const noexcept {
  if (状态 != 二次关系数据状态::已读取) return false;
  return std::all_of(来源组.begin(), 来源组.end(), 来源完整) &&
      std::all_of(用途组.begin(), 用途组.end(), 用途完整);
}

bool 二次关系图结果::成功() const noexcept {
  if (状态 != 二次关系数据状态::已读取 || 类别 != 相关概念类别::特征)
    return false;
  return std::all_of(RC组.begin(), RC组.end(), 事实完整) &&
      std::all_of(直接边.begin(), 直接边.end(), 边完整);
}

bool 二次关系治理结果::成功() const noexcept {
  if (状态 != 二次关系数据状态::已变更 || !正式回执 ||
      !发布见证完整(*正式回执) || !事实 || !事实完整(*事实)) return false;
  return std::all_of(新直接边.begin(), 新直接边.end(), 边完整);
}

} // namespace 海中鱼巣
