#pragma once

#include <optional>
#include <vector>

#include "新_动态类.h"
#include "新_因果公共类型.h"

namespace 海中鱼巣
{
class 新_场景类;

// 全局基础数据集中的独立因果信息组织根。
extern 稳定编码 新因果信息图根节点;

class 新_因果类 final
{
  public:
    新_因果类(新_状态类 &状态服务, 新_动态类 &动态服务) noexcept;
    新_因果类(新_状态类 &状态服务, 新_动态类 &动态服务,
              const 新因果动作证据提供者 &动作证据提供者) noexcept;

    bool 初始化() noexcept;
    新_因果类(新_状态类 &, 新_动态类 &, 新_存在类 &, 概念_存在类 &, 概念_特征类 &, 概念_动态类 &,
              const 新_特征值类 &, 新_场景类 &) noexcept;
    新因果共同抽象结果 抽象共同因果(const std::vector<稳定编码> &) noexcept;
    新因果共同抽象结果 获取概念因果(稳定编码) const noexcept;
    // 完整具体规律全集；不包含概念因果，不形成规律或更新统计。
    std::vector<稳定编码> 查询具体因果() const noexcept;
    std::vector<稳定编码> 查询概念因果() const noexcept;

    新因果处理结果 处理结果状态(const 新因果发生记录 &发生) noexcept;
    新因果处理结果 处理结果动态(const 新因果发生记录 &发生) noexcept;
    新因果处理结果 处理动作事实(const 新因果发生记录 &发生) noexcept;
    新因果处理结果 处理外部动作动态(const 新因果发生记录 &发生) noexcept;

    std::optional<新因果信息> 获取因果(稳定编码 因果节点) const noexcept;
    bool 是因果节点(稳定编码 节点) const noexcept;

    新因果查询结果 根据结果状态查询(const 新因果状态模式 &结果状态) const noexcept;
    新因果查询结果 根据条件状态查询(const 新因果状态模式 &条件状态) const noexcept;
    新因果查询结果 根据动作概念查询(稳定编码 动作概念节点) const noexcept;
    std::vector<新因果方法参考> 为目标状态查询方法参考(
        const 新因果状态模式 &目标状态) const noexcept;

    // 普通观察和方法执行都必须传入统一发生身份，以保证跨路径只计一次。
    新因果操作状态 登记因果发生结果(稳定编码 因果节点, 稳定编码 发生身份, bool 符合) noexcept;
    新因果操作状态 登记方法执行结果(稳定编码 因果节点, 稳定编码 发生身份, bool 符合) noexcept;

  private:
    void 尝试共同抽象(稳定编码) noexcept;
    新因果处理结果 处理发生(const 新因果发生记录 &发生, 新因果形成来源 来源,
                            const std::optional<新因果动作定义> &动作) noexcept;

    新_状态类 &状态服务_;
    新_动态类 &动态服务_;
    const 新因果动作证据提供者 *动作证据提供者_ = nullptr;
    新_存在类 *存在服务_ = nullptr;
    概念_存在类 *存在概念服务_ = nullptr;
    概念_特征类 *特征概念服务_ = nullptr;
    概念_动态类 *动态概念服务_ = nullptr;
    const 新_特征值类 *特征值服务_ = nullptr;
    新_场景类 *场景服务_ = nullptr;
};

} // namespace 海中鱼巣
