module;

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <memory>
#include <mutex>
#include <new>
#include <optional>
#include <type_traits>
#include <utility>
#include <vector>

export module 海中鱼巣.业务.提供者.四本体根生产初始化;

import 海中鱼巣.领域.服务.L2概念结构;
import 海中鱼巣.领域.服务.L2概念结构聚合;

export {
#include "四本体根生产初始化.数据.h"
}

#include "初始化.四本体根.inl"

export namespace 海中鱼巣 {

bool 四本体根生产初始化结果::成功() const noexcept {
  return 四本体根生产初始化内部::结果证据完整(*this);
}

class 四本体根生产初始化提供者 final {
public:
  explicit 四本体根生产初始化提供者(L2概念结构聚合服务 &聚合服务) noexcept
      : 聚合服务_(聚合服务) {}

  四本体根生产初始化提供者() = delete;
  四本体根生产初始化提供者(const 四本体根生产初始化提供者 &) = delete;
  四本体根生产初始化提供者 &
  operator=(const 四本体根生产初始化提供者 &) = delete;
  四本体根生产初始化提供者(四本体根生产初始化提供者 &&) = delete;
  四本体根生产初始化提供者 &operator=(四本体根生产初始化提供者 &&) = delete;

  四本体根生产初始化结果 初始化(const 四本体根生产初始化请求 &请求) noexcept {
    return 四本体根生产初始化内部::编排四本体根生产初始化(
        发布锁_, 锁定选择_, 存在保存请求_, 存在请求待重放_, 特征保存请求_,
        特征请求待重放_, 动态保存请求_, 动态请求待重放_, 因果链保存请求_,
        因果链请求待重放_, 已发布材料_, 请求,
        [this]() noexcept -> L2概念结构服务 & {
          return 聚合服务_.取得L2概念结构服务();
        });
  }

  std::optional<四本体根发布材料> 读取已发布材料() const {
    std::lock_guard<std::mutex> 锁(发布锁_);
    return 已发布材料_;
  }

private:
  L2概念结构聚合服务 &聚合服务_;
  mutable std::mutex 发布锁_;
  std::optional<四本体根生产初始化请求> 锁定选择_;
  std::optional<L2概念本体根建立请求> 存在保存请求_;
  bool 存在请求待重放_ = false;
  std::optional<L2概念本体根建立请求> 特征保存请求_;
  bool 特征请求待重放_ = false;
  std::optional<L2概念本体根建立请求> 动态保存请求_;
  bool 动态请求待重放_ = false;
  std::optional<L2概念本体根建立请求> 因果链保存请求_;
  bool 因果链请求待重放_ = false;
  std::optional<四本体根发布材料> 已发布材料_;
};

} // namespace 海中鱼巣
