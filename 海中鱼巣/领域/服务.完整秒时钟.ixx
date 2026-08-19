module;

#include <chrono>
#include <cstdint>
#include <optional>

#define 运行期Provider_NO_INCLUDES
export module 海中鱼巣.领域.服务.完整秒时钟;
#undef 运行期Provider_NO_INCLUDES

export {
#include "运行期Provider.数据.h"
}

// 对齐 BIZ-L3-002-01-02 v0.1
// 完整秒时钟服务：把可信单调时间换算为当前运行纪元内已完整结束的秒区间
// 使用 std::chrono::steady_clock 作为单调时钟适配器（L4-002-01-02-01 最小实现）
export namespace 海中鱼巣 {

class 完整秒时钟服务 final {
public:
  完整秒时钟服务(std::uint64_t 运行代次, 时间纪元身份 纪元身份,
                  std::uint32_t 时间源版本) noexcept
      : 运行代次_(运行代次), 纪元身份_(纪元身份),
        时间源版本_(时间源版本) {}

  完整秒时钟服务(const 完整秒时钟服务&) = delete;
  完整秒时钟服务& operator=(const 完整秒时钟服务&) = delete;

  // BIZ-L3-002-01-02: 读取当前完整秒边界
  完整秒边界读取结果 读取当前完整秒边界(
      const 完整秒边界读取请求& 请求) const noexcept {
    完整秒边界读取结果 结果;

    // N00: 服务配置非零代次、非零纪元、固定源版本
    if (运行代次_ == 0 || 纪元身份_.值 == 0 || 时间源版本_ == 0) {
      结果.状态 = 完整秒状态::内部不一致;
      return 结果;
    }

    // N01: 请求合同、非零字段和非负上一边界有效
    if (请求.合同版本 != 完整秒时钟合同版本 ||
        请求.运行代次 == 0 || 请求.纪元身份.值 == 0) {
      结果.状态 = 完整秒状态::请求拒绝;
      return 结果;
    }

    // N12: 请求代次和纪元匹配服务配置
    if (请求.运行代次 != 运行代次_ ||
        请求.纪元身份 != 纪元身份_) {
      结果.状态 = 完整秒状态::纪元错配;
      return 结果;
    }

    // N13: 期望源版本匹配服务固定源版本
    if (请求.期望时间源版本 != 时间源版本_) {
      结果.状态 = 完整秒状态::时间源版本漂移;
      return 结果;
    }

    // N02: 读取可信单调时间证据（L4 最小实现：steady_clock）
    const auto 现在 = std::chrono::steady_clock::now();
    const auto 纳秒数 = static_cast<std::uint64_t>(
        std::chrono::duration_cast<std::chrono::nanoseconds>(
            现在.time_since_epoch())
            .count());

    // 换算为完整秒边界
    const auto 当前完整秒 = 纳秒数 / 1000000000ULL;
    const auto 纳秒偏移 = 纳秒数 % 1000000000ULL;

    // N16: 时间倒退检查
    if (当前完整秒 < 请求.上一已消费完整秒边界) {
      结果.状态 = 完整秒状态::时间倒退;
      return 结果;
    }

    结果.观察.完整秒边界 = 当前完整秒;
    结果.观察.纳秒偏移 = 纳秒偏移;

    // 无新完整秒：与上一边界相同
    if (当前完整秒 == 请求.上一已消费完整秒边界) {
      结果.状态 = 完整秒状态::无新完整秒;
      return 结果;
    }

    // 已读取：有新完整秒，形成区间
    结果.状态 = 完整秒状态::已读取;
    结果.区间 = 完整秒区间{请求.上一已消费完整秒边界, 当前完整秒};
    return 结果;
  }

  std::uint64_t 运行代次() const noexcept { return 运行代次_; }
  时间纪元身份 纪元身份() const noexcept { return 纪元身份_; }
  std::uint32_t 时间源版本() const noexcept { return 时间源版本_; }

private:
  std::uint64_t 运行代次_;
  时间纪元身份 纪元身份_;
  std::uint32_t 时间源版本_;
};

} // namespace 海中鱼巣
