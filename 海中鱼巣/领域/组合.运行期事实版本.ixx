module;

#include <cstdint>
#include <new>
#include <optional>
#include <stdexcept>

export module 海中鱼巣.领域.组合.运行期事实版本;

import 海中鱼巣.领域.服务.L2结构聚合;
import 海中鱼巣.领域.服务.L2存在结构;
import 海中鱼巣.领域.合同.L2结构公共;

export namespace 海中鱼巣 {

inline constexpr std::uint32_t 共享事实截止读取合同版本 = 1;

struct 运行期事实版本服务配置 final {
  std::uint64_t 运行代次 = 0;
  std::uint32_t 事实范围版本 = 0;
};

struct 共享事实截止读取请求 final {
  std::uint32_t 合同版本 = 共享事实截止读取合同版本;
  std::uint64_t 运行代次 = 0;
  std::uint32_t 事实范围版本 = 0;
};

enum class 共享事实截止读取状态 : std::uint8_t {
  已读取 = 1,
  请求拒绝 = 2,
  资源失败 = 3,
  内部不一致 = 4
};

struct 已发布共享事实截止 final {
  std::uint32_t 合同版本 = 共享事实截止读取合同版本;
  std::uint64_t 运行代次 = 0;
  std::uint32_t 事实范围版本 = 0;
  std::uint64_t 事实截止代次 = 0;
};

struct 共享事实截止读取结果 final {
  共享事实截止读取状态 状态 = 共享事实截止读取状态::请求拒绝;
  std::optional<已发布共享事实截止> 截止;
};

class 运行期事实版本服务 final {
public:
  运行期事实版本服务(const L2结构聚合服务& 结构聚合服务,
                       运行期事实版本服务配置 配置) noexcept
      : 结构聚合服务_(结构聚合服务), 配置_(配置) {}

  运行期事实版本服务(const 运行期事实版本服务&) = delete;
  运行期事实版本服务& operator=(const 运行期事实版本服务&) = delete;
  运行期事实版本服务(运行期事实版本服务&&) = delete;
  运行期事实版本服务& operator=(运行期事实版本服务&&) = delete;

  共享事实截止读取结果 读取当前已发布共享事实截止(
      const 共享事实截止读取请求& 请求) const noexcept {
    if (配置_.运行代次 == 0 || 配置_.事实范围版本 == 0 ||
        请求.合同版本 != 共享事实截止读取合同版本 ||
        请求.运行代次 == 0 || 请求.事实范围版本 == 0 ||
        请求.运行代次 != 配置_.运行代次 ||
        请求.事实范围版本 != 配置_.事实范围版本) {
      return {共享事实截止读取状态::请求拒绝, std::nullopt};
    }

    try {
      const auto& 存在服务 = 结构聚合服务_.取得L2存在结构服务();
      const auto 代次结果 = 存在服务.读取当前事实代次();
      if (代次结果.状态 == L2结构状态::资源失败) {
        return {共享事实截止读取状态::资源失败, std::nullopt};
      }
      if (代次结果.合同版本 != L2结构合同版本 ||
          代次结果.状态 != L2结构状态::已读取 ||
          代次结果.事实截止代次 == 0 ||
          代次结果.变更事实代次.has_value()) {
        return {共享事实截止读取状态::内部不一致, std::nullopt};
      }

      已发布共享事实截止 截止;
      截止.运行代次 = 配置_.运行代次;
      截止.事实范围版本 = 配置_.事实范围版本;
      截止.事实截止代次 = 代次结果.事实截止代次;
      return {共享事实截止读取状态::已读取, 截止};
    } catch (const std::bad_alloc&) {
      return {共享事实截止读取状态::资源失败, std::nullopt};
    } catch (const std::length_error&) {
      return {共享事实截止读取状态::资源失败, std::nullopt};
    } catch (...) {
      return {共享事实截止读取状态::内部不一致, std::nullopt};
    }
  }

private:
  const L2结构聚合服务& 结构聚合服务_;
  const 运行期事实版本服务配置 配置_;
};

} // namespace 海中鱼巣
