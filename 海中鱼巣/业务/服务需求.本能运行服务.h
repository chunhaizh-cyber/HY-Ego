#pragma once
#include <memory>
#include <string_view>
#include "../领域/新_方法类.h"
#include "../领域/服务要求.材料合同.h"
#include "../领域/本能需求.运行接口.h"

namespace 海中鱼巣 {
#ifdef HY_EGO_DEBUG_LOG_NEW_DEMAND_STARTUP
// 诊断暂存不做I/O；装配层释放上下文锁后统一输出，不参与业务裁决。
void 暂存新需求启动观察(std::wstring_view 模块, std::wstring_view 入口,
    std::wstring_view 内容) noexcept;
void 输出新需求启动观察() noexcept;
#endif
class 新_世界树类;
class 新_动态类;
class 新_任务类;
class 服务对象需求发布服务;
class 新本能需求运行接口;

// 只落实参数中的既有需求D；不建立D、不承接自我需求，不建立任务。
class 服务对象新需求本能运行服务 final : public 新服务对象新需求本能端口,
    public 新本能需求任务参数工厂,
    public std::enable_shared_from_this<服务对象新需求本能运行服务> {
public:
    服务对象新需求本能运行服务(新_世界树类&, 新_方法类&, 新_需求类&,
        新_动态类&, 服务对象需求发布服务&, const 新服务要求材料读取接口&, 新_任务类&) noexcept;
    ~服务对象新需求本能运行服务();
    服务对象新需求本能运行服务(const 服务对象新需求本能运行服务&) = delete;
    服务对象新需求本能运行服务& operator=(const 服务对象新需求本能运行服务&) = delete;
    bool 初始化(const 新服务对象新需求配置&) noexcept;
    bool 关闭() noexcept;
    std::optional<新服务对象新需求配置> 读取登记材料() const noexcept override;
    新方法建立结果 确保方法登记(const 新方法知识建立请求&) noexcept override;
    std::shared_ptr<新本能需求运行接口> 建立参数操作提供者(
        稳定编码 方法节点, const std::vector<新方法参数角色绑定>& 参数,
        稳定编码 动作场景, 稳定编码 动作主体, 稳定编码 动作来源);
    新方法参数场景结果 建立执行参数场景(
        稳定编码 方法节点, std::vector<新方法参数角色绑定> 参数,
        稳定编码 动作场景, 稳定编码 动作主体, 稳定编码 动作来源);
    bool 支持任务方法(稳定编码 方法节点) const noexcept override;
    新本能已发布需求核验结果 核验任务新需求结果(
        稳定编码,const 新本能已发布需求结果&) const noexcept override;
    std::shared_ptr<新本能需求运行接口> 建立任务参数操作提供者(
        const 新本能需求任务参数请求&) override;
    新方法参数场景结果 建立任务参数场景(
        const 新本能需求任务参数请求&,
        const 新内部治理对象读取接口*,
        std::optional<新方法任务成员绑定>) override;
    // 外层工厂固定K/X记录来源；执行只消费场景能力及未完成发布的临时恢复材料。
    新本能函数本次条件结果对 执行(const 方法参数场景&) noexcept override;
private:
    struct 实现;
    std::unique_ptr<实现> 实现_;
};
} // namespace 海中鱼巣
