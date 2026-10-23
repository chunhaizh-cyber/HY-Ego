#pragma once
#include "../领域/因果反向查询.数据.h"
#include "../领域/因果筹办.只读合同.h"

namespace 海中鱼巣
{
class 新_因果类;
class 新_特征类;
class 概念_存在类;
class 概念_动态类;
class 新_动态类;
class 新_存在类;
class 新_场景类;

class 因果反向查询服务 final
{
  public:
    因果反向查询服务(const 新_因果类 &, const 新_特征类 &, const 概念_特征类 &, const 概念_存在类 &,
                     const 概念_动态类 &, const 新_动态类 &, const 新_存在类 &,
                     const 新_场景类 &, const 新治理参与者读取接口 * = nullptr) noexcept;
    反向因果查询结果 查询(const 反向因果查询请求 &) const noexcept;
    新因果筹办读回 查询筹办(const 新因果筹办请求 &) const;

  private:
    const 新_因果类 &因果服务_;
    const 新_特征类 &特征服务_;
    const 概念_特征类 &特征概念服务_;
    const 概念_存在类 &存在概念服务_;
    const 概念_动态类 &动态概念服务_;
    const 新_动态类 &动态服务_;
    const 新_存在类 &存在服务_;
    const 新_场景类 &场景服务_;
    const 新治理参与者读取接口 *治理参与者_ = nullptr;
};
} // namespace 海中鱼巣
