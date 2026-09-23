#pragma once

#include "../领域/数据服务.存在类.h"

#include <cstdint>
#include <optional>

namespace 海中鱼巣 {

inline constexpr std::uint32_t 本能根运行初始化合同版本_v1 = 1;

struct 本能根运行单根锚点_v1 final {
  稳定编码 需求{};
  稳定编码 列表项{};
  特征信息身份 实际特征{};
  稳定编码 目标合同{};

  [[nodiscard]] bool 完整() const noexcept {
    const 稳定编码 值[]{需求, 列表项, 实际特征.编码, 目标合同};
    for (std::size_t i = 0; i < 4; ++i) {
      if (!有效(值[i]))
        return false;
      for (std::size_t j = 0; j < i; ++j)
        if (值[i] == 值[j])
          return false;
    }
    return true;
  }

  friend bool operator==(const 本能根运行单根锚点_v1 &,
                         const 本能根运行单根锚点_v1 &) = default;
};

struct 本能根运行锚点_v1 final {
  存在信息身份 自我{};
  本能根运行单根锚点_v1 安全根{};
  本能根运行单根锚点_v1 服务根{};
  std::uint64_t 事实截止代次 = 0;

  [[nodiscard]] bool 完整() const noexcept {
    if (!有效(自我) || !事实截止代次 || !安全根.完整() ||
        !服务根.完整())
      return false;
    const 稳定编码 值[]{
        安全根.需求, 安全根.列表项, 安全根.实际特征.编码,
        安全根.目标合同, 服务根.需求, 服务根.列表项,
        服务根.实际特征.编码, 服务根.目标合同};
    for (std::size_t i = 0; i < 8; ++i) {
      if (值[i] == 自我.编码)
        return false;
      for (std::size_t j = 0; j < i; ++j)
        if (值[i] == 值[j])
          return false;
    }
    return true;
  }

  friend bool operator==(const 本能根运行锚点_v1 &,
                         const 本能根运行锚点_v1 &) = default;
};

struct 本能根运行初始化请求_v1 final {
  std::uint32_t 合同版本 = 本能根运行初始化合同版本_v1;
  存在信息身份 唯一自我{};

  friend bool operator==(const 本能根运行初始化请求_v1 &,
                         const 本能根运行初始化请求_v1 &) = default;
};

enum class 本能根运行初始化状态_v1 : std::uint8_t {
  已形成 = 1,
  已恢复 = 2,
  入口拒绝 = 3,
  当前性漂移 = 4,
  幂等冲突 = 5,
  引用冲突 = 6,
  材料未闭合 = 7,
  根材料已退出 = 8,
  已可能发布 = 9,
  资源失败 = 10,
  内部不一致 = 11
};

struct 本能根运行初始化结果_v1 final {
  std::uint32_t 合同版本 = 本能根运行初始化合同版本_v1;
  本能根运行初始化状态_v1 状态 = 本能根运行初始化状态_v1::入口拒绝;
  本能根运行初始化请求_v1 原请求{};
  std::optional<本能根运行锚点_v1> 锚点;

  [[nodiscard]] bool 成功() const noexcept {
    return 合同版本 == 本能根运行初始化合同版本_v1 &&
           (状态 == 本能根运行初始化状态_v1::已形成 ||
            状态 == 本能根运行初始化状态_v1::已恢复) &&
           锚点 && 锚点->完整() && 锚点->自我 == 原请求.唯一自我;
  }

  friend bool operator==(const 本能根运行初始化结果_v1 &,
                         const 本能根运行初始化结果_v1 &) = default;
};

} // namespace 海中鱼巣
