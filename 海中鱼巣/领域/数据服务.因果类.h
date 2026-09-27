#pragma once

#include "合同.因果结构.h"

namespace 海中鱼巣 {

class 状态类数据服务;

class 因果类数据服务 final : public 因果结构只读提供者 {
public:
  因果类数据服务() = delete;
  因果类数据服务(const 因果类数据服务 &) = delete;
  因果类数据服务 &operator=(const 因果类数据服务 &) = delete;
  因果类数据服务(因果类数据服务 &&) = delete;
  因果类数据服务 &operator=(因果类数据服务 &&) = delete;

  因果类数据服务(L1事实基座服务 &,
                   L1所有者范围写端口 &&,
                   const 因果结构交付 &,
                   const 因果概念核验提供者 &,
                   const 动态结构只读提供者 &,
                   const 状态使用绑定只读提供者 &,
                   const 状态类数据服务 &);

  bool 绑定于(const L1事实基座服务 &) const noexcept override;

  因果单项结果 发布或复用因果(const 因果发布请求 &);
  因果单项结果 读取当前因果(
      const 因果当前读取请求 &) const override;
  因果组结果 查询精确定义(
      const 因果精确定义查询请求 &) const override;
  因果组结果 按特征概念查询因果(
      const 因果按特征查询请求 &) const override;
  因果证据结果 关联因果证据(const 因果证据关联请求 &);
  因果证据结果 读取因果证据(
      const 因果证据读取请求 &) const override;
  因果证据结果 退出因果证据(const 因果证据退出请求 &);
  因果单项结果 退出因果(const 因果退出请求 &);
  因果操作结果 确认当前因果结构身份(
      const 因果当前身份请求 &) const override;

private:
  bool 布局浅层有效() const noexcept;
  bool 当前布局有效() const;

  static 因果操作结果 失败头(因果数据状态) noexcept;
  static 因果单项结果 单项失败(因果数据状态) noexcept;
  static 因果组结果 组失败(因果数据状态) noexcept;
  static 因果证据结果 证据失败(因果数据状态) noexcept;

  L1事实基座服务 &l1_;
  L1所有者范围写端口 port_;
  L1结构所有者身份 owner_{};
  因果结构交付 layout_;
  const 因果概念核验提供者 &concepts_;
  const 动态结构只读提供者 &dynamics_;
  const 状态使用绑定只读提供者 &bindings_;
  const 状态类数据服务 &states_;
};

} // namespace 海中鱼巣
