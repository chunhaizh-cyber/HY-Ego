#pragma once

#include "合同.动态结构.h"

namespace 海中鱼巣 {

class 状态类数据服务;
class 特征类数据服务;
class 特征值类数据服务;

class 动态类数据服务 final : public 动态结构只读提供者 {
public:
    动态类数据服务() = delete;
    动态类数据服务(const 动态类数据服务&) = delete;
    动态类数据服务& operator=(const 动态类数据服务&) = delete;
    动态类数据服务(动态类数据服务&&) = delete;
    动态类数据服务& operator=(动态类数据服务&&) = delete;

    动态类数据服务(L1事实基座服务&,
        L1所有者范围写端口&&,
        const 动态结构交付&,
        const 存在结构身份只读提供者&,
        const 状态使用绑定只读提供者&,
        const 状态类数据服务&,
        const 特征类数据服务&,
        const 特征值类数据服务&);

    bool 绑定于(const L1事实基座服务&) const noexcept override;

    动态单项结果 创建原子动态(const 动态原子创建请求&);
    动态单项结果 创建组合动态(const 动态组合创建请求&);
    动态单项结果 读取当前动态(
        const 动态当前读取请求&) const override;
    动态组结果 按主体查询动态(
        const 动态按主体查询请求&) const override;
    动态组结果 按绑定反查原子动态(
        const 动态按绑定查询请求&) const override;
    动态组结果 按子动态反查父动态(
        const 动态按子动态查询请求&) const override;
    动态展开结果 展开动态来源(
        const 动态来源展开请求&) const override;
    动态操作结果 确认当前动态结构身份(
        const 动态当前身份请求&) const override;
    动态单项结果 退出动态(const 动态退出请求&);

private:
    bool 布局浅层有效() const noexcept;
    bool 当前布局有效() const;
    static 动态操作结果 失败头(动态数据状态) noexcept;
    static 动态单项结果 单项失败(动态数据状态) noexcept;
    static 动态组结果 组失败(动态数据状态) noexcept;
    static 动态展开结果 展开失败(动态数据状态) noexcept;

    L1事实基座服务& l1_;
    L1所有者范围写端口 port_;
    L1结构所有者身份 owner_{};
    动态结构交付 layout_;
    const 存在结构身份只读提供者& existence_;
    const 状态使用绑定只读提供者& binding_;
    const 状态类数据服务& state_;
    const 特征类数据服务& feature_;
    const 特征值类数据服务& value_;
};

} // namespace 海中鱼巣
