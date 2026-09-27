#pragma once

#include "应用服务.自我线程正式上下文.h"
#include "计算.二次关系求值.h"

#include <cstdint>
#include <optional>

namespace 海中鱼巣 {

enum class 自我根复核触发根 : std::uint8_t { 双根 = 1, 安全根 = 2, 服务根 = 3 };
enum class 自我线程根复核业务结果 : std::uint8_t { 当前满足 = 1, 正差距 = 2 };

enum class 自我线程根需求复核状态 : std::uint8_t {
  已复核 = 1, 入口拒绝 = 2, 材料不足 = 3, 规则不支持 = 4,
  // 5 对应已经退出的旧技术状态，稳定数值不复用。
  运算溢出 = 7, 资源失败 = 8, 内部不一致 = 9, 退役不可用 = 10,
  类型不相容 = 11, 单位量化不相容 = 12, 参照不相容 = 13, 未实现 = 14
};

enum class 自我线程单根复核技术状态 : std::uint8_t {
  未复核 = 1, 已复核 = 2, 入口拒绝 = 3, 材料不足 = 4, 规则不支持 = 5,
  // 6 对应已经退出的旧技术状态，稳定数值不复用。
  运算溢出 = 8, 资源失败 = 9, 内部不一致 = 10, 退役不可用 = 11,
  类型不相容 = 12, 单位量化不相容 = 13, 参照不相容 = 14, 未实现 = 15
};

struct 自我根复核锚点 final {
  特征类型身份 FT;
  特征I64比较绑定身份 K;
  概念树概念身份 RC;
  bool 完整() const noexcept;
  friend bool operator==(const 自我根复核锚点&, const 自我根复核锚点&) = default;
};

struct 自我线程根需求复核请求 final {
  自我线程正式上下文投影 上下文;
  自我根复核触发根 触发根 = 自我根复核触发根::双根;
  稳定编码 正式需求定位{}, 来源消息{}, 原请求{};
  bool 完整() const noexcept;
  friend bool operator==(const 自我线程根需求复核请求&,
                         const 自我线程根需求复核请求&) = default;
};

struct 自我线程单根复核投影 final {
  本能根角色 角色 = 本能根角色::安全;
  本能根材料 根材料;
  特征信息身份 当前实际特征;
  特征类型身份 FT;
  特征I64比较绑定身份 K;
  概念树概念身份 RC;
  std::int64_t 当前I64 = 0, 目标I64 = 0, 准确差异 = 0;
  自我线程根复核业务结果 业务结果 = 自我线程根复核业务结果::当前满足;
  二次关系判断结果 求值见证;
  bool 完整() const noexcept;
};

struct 自我线程根需求复核结果 final {
  自我线程根需求复核状态 状态 = 自我线程根需求复核状态::入口拒绝;
  自我线程根需求复核请求 原请求;
  自我线程单根复核技术状态 安全根技术状态 = 自我线程单根复核技术状态::未复核;
  自我线程单根复核技术状态 服务根技术状态 = 自我线程单根复核技术状态::未复核;
  std::optional<自我线程单根复核投影> 安全根, 服务根;
  bool 写业务事实 = false;
  bool 成功(const 自我线程根需求复核请求&) const noexcept;
};

class 自我线程根需求复核端口 {
public:
  virtual ~自我线程根需求复核端口() = default;
  virtual 自我线程根需求复核结果 复核双根当前需求(
      const 自我线程根需求复核请求&) const noexcept = 0;
};

class 自我根需求复核提供者 final : public 自我线程根需求复核端口 {
public:
  自我根需求复核提供者(const 需求类数据服务&, const 存在类数据服务&,
                       const 二次关系求值应用服务&, 自我根复核锚点,
                       自我根复核锚点);
  自我线程根需求复核结果 复核双根当前需求(
      const 自我线程根需求复核请求&) const noexcept override;
  bool 使用需求服务(const 需求类数据服务&) const noexcept;
  bool 使用存在服务(const 存在类数据服务&) const noexcept;
  bool 使用求值服务(const 二次关系求值应用服务&) const noexcept;

  自我根需求复核提供者() = delete;
  自我根需求复核提供者(const 自我根需求复核提供者&) = delete;
  自我根需求复核提供者& operator=(const 自我根需求复核提供者&) = delete;
  自我根需求复核提供者(自我根需求复核提供者&&) = delete;
  自我根需求复核提供者& operator=(自我根需求复核提供者&&) = delete;

private:
  const 需求类数据服务& 需求_;
  const 存在类数据服务& 存在_;
  const 二次关系求值应用服务& 求值_;
  自我根复核锚点 安全根_, 服务根_;
};

} // namespace 海中鱼巣
