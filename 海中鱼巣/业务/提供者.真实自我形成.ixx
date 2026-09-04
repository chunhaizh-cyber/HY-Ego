module;

#include <optional>

export module 海中鱼巣.业务.提供者.真实自我形成;

import 海中鱼巣.业务.合同.自我世界树根消费;
import 海中鱼巣.领域.服务.L2存在结构;
import 海中鱼巣.领域.服务.L2场景结构;
import 海中鱼巣.领域.服务.L2结构聚合;

export {
#include "真实自我形成.数据.h"
}

export namespace 海中鱼巣 {

bool 真实自我形成结果::成功() const noexcept {
  return 已形成自我 &&
      (状态 == 真实自我形成状态::已形成 ||
       状态 == 真实自我形成状态::精确重复);
}

class 真实自我形成提供者 final {
public:
  explicit 真实自我形成提供者(L2结构聚合服务& 聚合服务) noexcept
      : 聚合服务_(聚合服务) {}

  真实自我形成提供者() = delete;
  真实自我形成提供者(const 真实自我形成提供者&) = delete;
  真实自我形成提供者& operator=(const 真实自我形成提供者&) = delete;
  真实自我形成提供者(真实自我形成提供者&&) = delete;
  真实自我形成提供者& operator=(真实自我形成提供者&&) = delete;

  真实自我形成结果 形成(const 真实自我形成请求& 请求) noexcept {
    (void)请求;
    (void)聚合服务_;
    return {真实自我形成状态::请求无效};
  }

private:
  L2结构聚合服务& 聚合服务_;
};

} // namespace 海中鱼巣
