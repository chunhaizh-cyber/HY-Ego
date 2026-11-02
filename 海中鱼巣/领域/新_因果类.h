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
    稳定编码 确保先天因果(const 新先天因果定义&);
    新先天因果概念组 确保先天因果概念(稳定编码 治理定义);
    std::optional<新先天因果信息> 读取先天因果(稳定编码) const;
    std::vector<稳定编码> 查询先天结果(稳定编码 结果特征类型) const;
    新关系先天元数据 确保关系先天元数据(const 新关系先天元数据请求&);
    新关系先天因果信息 确保关系先天因果(const 新关系先天因果定义&);
    std::optional<新关系先天因果信息> 读取关系先天因果(稳定编码) const;
    std::vector<新关系先天因果信息> 查询关系先天结果(稳定编码 特征类型,
        std::optional<新关系先天结果种类> 种类=std::nullopt) const;
    // 独立来源前驱族；只发布规则知识，不产生采用、任务或动作发生事实。
    // 工程错误向上送出，读取返回的是含三阶段判定及完整保护的正式定义。
    新服务取得来源前驱元数据 确保服务取得来源前驱元数据(const 新服务取得来源前驱配置&);
    新来源前驱元数据读取结果 读取服务取得来源前驱元数据(const 新服务取得来源前驱配置&) const;
    新来源前驱规则读取结果 读取来源前驱规则保护(稳定编码) const;
    新来源前驱规则查询结果 查询来源前驱规则保护(
        稳定编码 服务任务类型, 新来源前驱关系结果) const;
    新来源前驱规则信息 确保来源前驱规则(const 新来源前驱规则定义&);
    std::optional<新来源前驱规则信息> 读取来源前驱规则(稳定编码) const;
    std::vector<新来源前驱规则信息> 查询来源前驱规则(
        稳定编码 服务任务类型, 新来源前驱关系结果) const;
    新_因果类(新_状态类 &, 新_动态类 &, 新_存在类 &, 概念_存在类 &, 概念_特征类 &, const 概念_动态类 &,
              const 新_特征值类 &, 新_场景类 &,
              const 新因果动作证据提供者 * = nullptr,
              const 新治理参与者读取接口 * = nullptr) noexcept;
    新因果共同抽象结果 抽象共同因果(const std::vector<稳定编码> &) noexcept;
    新因果共同抽象结果 获取概念因果(稳定编码) const noexcept;
    // 完整具体规律全集；不包含概念因果，不形成规律或更新统计。
    std::vector<稳定编码> 查询具体因果() const noexcept;
    std::vector<稳定编码> 查询概念因果() const noexcept;
    新因果世界范围 查询当前场景树因果() const noexcept;

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
    std::optional<新来源前驱规则信息> 读取来源前驱规则可抛内部(
        稳定编码, bool 只读变化可返回, std::vector<基础联合节点快照>* 完整保护输出) const;
    std::vector<新来源前驱规则信息> 查询来源前驱规则可抛内部(
        稳定编码 服务任务类型, 新来源前驱关系结果, bool 只读变化可返回,
        std::vector<基础联合节点快照>* 完整保护输出) const;
    void 核验先天概念引用(const 新先天因果定义&) const;
    void 尝试共同抽象(稳定编码) noexcept;
    新因果处理结果 处理发生(const 新因果发生记录 &发生, 新因果形成来源 来源,
                            const std::optional<新因果动作定义> &动作) noexcept;

    新_状态类 &状态服务_;
    新_动态类 &动态服务_;
    const 新因果动作证据提供者 *动作证据提供者_ = nullptr;
    新_存在类 *存在服务_ = nullptr;
    概念_存在类 *存在概念服务_ = nullptr;
    概念_特征类 *特征概念服务_ = nullptr;
    const 概念_动态类 *动态概念服务_ = nullptr;
    const 新_特征值类 *特征值服务_ = nullptr;
    新_场景类 *场景服务_ = nullptr;
    const 新治理参与者读取接口 *治理参与者_ = nullptr;
};

} // namespace 海中鱼巣
