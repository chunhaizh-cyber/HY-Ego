#include "初始化.本能双根二次关系概念.h"

namespace 海中鱼巣 {
namespace {

bool 正差距定义完整(const 二次关系定义& definition,
                    const 特征I64比较绑定身份 K) noexcept {
  const auto* atom = std::get_if<二次关系原子定义>(&definition);
  return atom && atom->D.掩码 == 4 && atom->K == K &&
      atom->输出 == 特征类标量结果角色::差异;
}

} // namespace

bool 本能根二次关系概念交付::完整() const noexcept {
  return 先天.角色 == 角色 && 有效(先天.类型.身份) &&
      有效(先天.完整域概念.概念.值) &&
      I64绑定事实完整(目标判断K) &&
      目标判断K.定义.用途 == 特征I64比较用途::目标判断 &&
      有效(通用存在概念.概念.值) &&
      std::holds_alternative<通用存在概念定义>(通用存在概念.定义) &&
      std::get<通用存在概念定义>(通用存在概念.定义).规则 ==
          通用存在定义规则::不预设特征 &&
      有效(正差距RC.值) &&
      正差距定义完整(正差距定义, 目标判断K.身份);
}

bool 本能双根二次关系概念初始化结果::成功() const noexcept {
  return (状态 == 本能双根二次关系概念初始化状态::已形成 ||
          状态 == 本能双根二次关系概念初始化状态::已恢复) &&
      原请求.先天交付.成功() && 安全根 && 服务根 &&
      安全根->角色 == 本能值角色::安全值 &&
      服务根->角色 == 本能值角色::服务值 &&
      安全根->完整() && 服务根->完整();
}

本能双根二次关系概念初始化提供者::
    本能双根二次关系概念初始化提供者(
        特征类数据服务& features,
        概念树类数据服务& concepts) noexcept
    : 特征服务_(features), 概念服务_(concepts) {}

本能双根二次关系概念初始化结果
本能双根二次关系概念初始化提供者::初始化(
    const 本能双根二次关系概念初始化请求& request) noexcept {
  本能双根二次关系概念初始化结果 result;
  result.原请求 = request;
  if (!request.先天交付.成功() ||
      !特征服务_.与特征服务同底座(特征服务_) ||
      !概念服务_.使用特征服务(特征服务_)) return result;

  // 待实现：现行概念树没有RC定义的current-only查询、建立和读回入口。
  // 不恢复旧纯概念、K或RC入口，也不把D_POS身份伪造成已发布RC。
  result.状态 = 本能双根二次关系概念初始化状态::未实现;
  return result;
}

} // namespace 海中鱼巣
