#pragma once

#include <optional>
#include <vector>

#include "新_因果公共类型.h"
#include "新_动态类.h"

namespace 海中鱼巣 {

// 全局基础数据集中的独立因果信息组织根。
extern 稳定编码 新因果信息图根节点;

class 新_因果类 final {
public:
    新_因果类(
        新_状态类& 状态服务,
        新_动态类& 动态服务) noexcept;
    新_因果类(
        新_状态类& 状态服务,
        新_动态类& 动态服务,
        const 新因果动作证据提供者& 动作证据提供者) noexcept;

    bool 初始化() noexcept;

    新因果处理结果 处理结果状态(
        const 新因果发生记录& 发生) noexcept;
    新因果处理结果 处理结果动态(
        const 新因果发生记录& 发生) noexcept;
    新因果处理结果 处理动作事实(
        const 新因果发生记录& 发生) noexcept;
    新因果处理结果 处理外部动作动态(
        const 新因果发生记录& 发生) noexcept;

    std::optional<新因果信息> 获取因果(
        稳定编码 因果节点) const noexcept;
    bool 是因果节点(稳定编码 节点) const noexcept;

    新因果查询结果 根据结果状态查询(
        const 新因果状态模式& 结果状态) const noexcept;
    新因果查询结果 根据条件状态查询(
        const 新因果状态模式& 条件状态) const noexcept;
    新因果查询结果 根据动作概念查询(
        稳定编码 动作概念节点) const noexcept;
    std::vector<新因果方法参考> 为目标状态查询方法参考(
        const 新因果状态模式& 目标状态) const noexcept;

    // 普通观察和方法执行都必须传入统一发生身份，以保证跨路径只计一次。
    新因果操作状态 登记因果发生结果(
        稳定编码 因果节点,
        稳定编码 发生身份,
        bool 符合) noexcept;
    新因果操作状态 登记方法执行结果(
        稳定编码 因果节点,
        稳定编码 发生身份,
        bool 符合) noexcept;

private:
    新因果处理结果 处理发生(
        const 新因果发生记录& 发生,
        新因果形成来源 来源,
        const std::optional<新因果动作定义>& 动作) noexcept;

    新_状态类& 状态服务_;
    新_动态类& 动态服务_;
    const 新因果动作证据提供者* 动作证据提供者_ = nullptr;
};

} // namespace 海中鱼巣
