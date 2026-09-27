#pragma once

#include "应用服务.特征概念类.h"
#include "应用服务.自我形成.h"
#include "../领域/数据服务.需求类.h"

#include <cstdint>
#include <optional>

namespace 海中鱼巣 {

struct 本能根运行单根锚点 final {
  稳定编码 需求{}, 列表项{}, 目标合同{};
  特征信息身份 实际特征;
  bool 完整() const noexcept;
  friend bool operator==(const 本能根运行单根锚点&,
                         const 本能根运行单根锚点&) = default;
};

struct 本能根运行锚点 final {
  稳定编码 自我{};
  本能根运行单根锚点 安全根, 服务根;
  bool 完整() const noexcept;
  friend bool operator==(const 本能根运行锚点&,
                         const 本能根运行锚点&) = default;
};

struct 本能根运行初始化请求 final {
  稳定编码 唯一自我{};
  friend bool operator==(const 本能根运行初始化请求&,
                         const 本能根运行初始化请求&) = default;
};

enum class 本能根运行初始化状态 : std::uint8_t {
  已形成 = 1,
  入口拒绝 = 3,
  幂等冲突 = 5,
  引用冲突 = 6,
  材料未闭合 = 7,
  已可能发布 = 9,
  资源失败 = 10,
  内部不一致 = 11,
  未实现 = 12
};

struct 本能根运行初始化结果 final {
  本能根运行初始化状态 状态 = 本能根运行初始化状态::入口拒绝;
  本能根运行初始化请求 原请求;
  std::optional<本能根运行锚点> 锚点;
  bool 成功() const noexcept;
  friend bool operator==(const 本能根运行初始化结果&,
                         const 本能根运行初始化结果&) = default;
};

class 本能根运行初始化提供者 final {
public:
  本能根运行初始化提供者(
      特征概念应用服务&, const 存在类数据服务&,
      需求类数据服务&, const 真实自我读取请求&,
      const 真实自我形成结果&);

  本能根运行初始化提供者() = delete;
  本能根运行初始化提供者(const 本能根运行初始化提供者&) = delete;
  本能根运行初始化提供者& operator=(
      const 本能根运行初始化提供者&) = delete;
  本能根运行初始化提供者(本能根运行初始化提供者&&) = delete;
  本能根运行初始化提供者& operator=(
      本能根运行初始化提供者&&) = delete;

  本能根运行初始化结果 初始化(
      const 本能根运行初始化请求&) noexcept;

private:
  特征概念应用服务& 特征概念_;
  const 存在类数据服务& 存在_;
  需求类数据服务& 需求_;
  真实自我投影 自我投影_;
  bool 已调用_ = false;
};

} // namespace 海中鱼巣
