#pragma once

#include "../领域/新_场景类.h"
#include "../领域/新_因果类.h"
#include "因果.反向查询服务.h"

namespace 海中鱼巣 {

// 仅组织治理实例；任务和方法仍是其虚拟存在及特征的唯一写入者。
class 新_内部治理世界类 final : public 新任务治理世界协作接口,
    public 新方法治理世界协作接口,
    public 新治理参与者读取接口, public 新因果动作证据提供者 {
public:
    新_内部治理世界类(新_特征值类&, 概念_特征类&, 新_特征类&,
        概念_存在类&, const 概念_动态类&, 新_状态类&, 新_存在类&,
        新_动态类&, const 新_场景类&) noexcept;
    新_内部治理世界类(const 新_内部治理世界类&) = delete;
    新_内部治理世界类& operator=(const 新_内部治理世界类&) = delete;
    bool 装配任务读取(const 新内部治理对象读取接口&) noexcept;
    bool 装配方法读取(const 新方法治理对象读取接口&) noexcept;
    bool 初始化() noexcept;
    std::optional<稳定编码> 获取根场景() const noexcept;
    新方法治理场景发布结果 发布新方法治理场景组(
        const std::vector<新方法虚拟存在引用>&, const 基础封存追加片段&,
        新特征I64更新准备* 联动特征准备 = nullptr) noexcept override;
    void 迁移已有方法治理场景(const std::vector<新方法虚拟存在引用>&) noexcept override;
    std::optional<新方法治理场景信息> 读取方法治理场景(
        const 新方法虚拟存在引用&) const noexcept override;
    新治理方法特征变化结果 完成方法特征变化(
        const 新方法虚拟存在引用&, 稳定编码, 稳定编码) noexcept override;
    std::optional<新治理方法特征变化结果> 读取方法特征变化(
        const 新方法虚拟存在引用&, 稳定编码, 稳定编码) const noexcept override;
    void 回收未消费方法治理场景组(
        const std::vector<新方法虚拟存在引用>&) noexcept override;
    新任务治理场景发布结果 发布新任务治理场景(
        const 新任务虚拟存在引用&, const 基础封存追加片段&) noexcept override;
    void 迁移已有任务治理场景(const std::vector<新任务虚拟存在引用>&) noexcept override;
    std::optional<新任务治理场景信息> 读取任务治理场景(
        const 新任务虚拟存在引用&) const noexcept override;
    新治理动作登记结果 建立任务状态动作(const 新治理任务动作请求&) noexcept override;
    新治理任务变化结果 完成任务状态变化(稳定编码, 稳定编码, 稳定编码) noexcept override;
    std::optional<新治理任务变化结果> 读取任务状态变化(
        const 新任务虚拟存在引用&, 稳定编码) const noexcept override;
    新治理任务特征变化结果 完成任务特征变化(
        const 新任务虚拟存在特征引用&, 稳定编码, 稳定编码) noexcept override;
    std::optional<新治理任务特征变化结果> 读取任务特征变化(
        const 新任务虚拟存在特征引用&, 稳定编码, 稳定编码) const noexcept override;
    新治理动作登记结果 建立任务特征动作(稳定编码) noexcept override;
    新治理任务特征动作变化结果 完成任务特征动作(
        稳定编码, 稳定编码) noexcept override;
    std::optional<新治理任务特征动作变化结果> 读取任务特征动作(
        稳定编码) const noexcept override;
    std::optional<新治理场景参与者> 读取治理参与者(
        稳定编码, 稳定编码) const noexcept override;
    std::optional<新因果动作定义> 读取自我动作事实(稳定编码) const noexcept override;
    std::optional<新因果动作定义> 推断外部动作(稳定编码) const noexcept override;
    反向因果查询结果 查询因果链(const 反向因果查询请求&) const noexcept;
private:
    std::optional<新治理任务变化结果> 读取场景内任务状态变化(
        const 新任务虚拟存在引用&, 稳定编码 来源普通动作, 稳定编码 场景) const;
    std::optional<新治理场景参与者> 读取范围治理参与者(稳定编码, 稳定编码) const;
    void 交付实际发生(const 新治理任务变化结果&) noexcept;
    稳定编码 根场景节点_{};
    概念_特征类& 特征概念服务_;
    新_特征类& 特征服务_;
    新_状态类& 状态服务_;
    新_动态类& 动态服务_;
    const 新_场景类& 现实场景_;
    const 新内部治理对象读取接口* 任务读取_ = nullptr;
    const 新方法治理对象读取接口* 方法读取_ = nullptr;
    新_场景类 治理场景_;
    新_因果类 因果服务_;
    因果反向查询服务 反查服务_;
};
} // namespace 海中鱼巣
