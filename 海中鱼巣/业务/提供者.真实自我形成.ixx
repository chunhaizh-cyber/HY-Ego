module;

#include <cstdint>
#include <mutex>
#include <new>
#include <optional>
#include <utility>

export module 海中鱼巣.业务.提供者.真实自我形成;

// 预冻结导入：SELF-FORM 最终 ABI 须机械匹配 DATA 发布。
import 海中鱼巣.领域.服务.L2存在结构;
import 海中鱼巣.领域.服务.L2场景结构;
import 海中鱼巣.领域.服务.L2结构聚合;
import 海中鱼巣.装配.普通应用;

export {
#include "真实自我形成.数据.h"
}

export namespace 海中鱼巣 {

bool 真实自我形成结果::成功() const noexcept {
  // 完整完整性函数待实现；当前桩只检查基本成功形状。
  if (已形成自我) {
    if (状态 == 真实自我形成状态::已形成 ||
        状态 == 真实自我形成状态::精确重复)
      return true;
  }
  return false;
}

class 真实自我形成提供者 final {
public:
  explicit 真实自我形成提供者(
      L2结构聚合服务 &聚合服务) noexcept
      : 聚合服务_(聚合服务) {}

  真实自我形成提供者() = delete;
  真实自我形成提供者(const 真实自我形成提供者 &) = delete;
  真实自我形成提供者 &
  operator=(const 真实自我形成提供者 &) = delete;
  真实自我形成提供者(真实自我形成提供者 &&) = delete;
  真实自我形成提供者 &
  operator=(真实自我形成提供者 &&) = delete;

  真实自我形成结果
  形成(const 真实自我形成请求 &请求) noexcept {
    // 桩实现：五步状态机待实现。
    真实自我形成结果 结果;
    结果.状态 = 真实自我形成状态::内部不一致;
    return 结果;
  }

  std::optional<真实自我事实> 读取已发布自我() const {
    std::lock_guard<std::mutex> 锁(发布锁_);
    return 已发布自我_;
  }

private:
  L2结构聚合服务 &聚合服务_;
  mutable std::mutex 发布锁_;
  std::optional<真实自我事实> 已发布自我_;
};

} // namespace 海中鱼巣
