#pragma once
#include <memory>
#include "../领域/服务值结算.本能合同.h"

namespace 海中鱼巣 {
class 新_世界树类;
class 新_方法类;
class 新_任务类;
class 新_场景类;
class 新_动态类;
class 服务值结算本能运行服务 final : public 新服务值结算确认接口 {
public:
    服务值结算本能运行服务(新_世界树类&, 新_方法类&, 新_任务类&, 新_需求类&,
        新_场景类&, 新_动态类&, 新_特征类&, 概念_特征类&, 概念_二次特征类&,
        新任务治理世界协作接口&, 新服务值结算工程报告函数) noexcept;
    ~服务值结算本能运行服务();
    服务值结算本能运行服务(const 服务值结算本能运行服务&) = delete;
    服务值结算本能运行服务& operator=(const 服务值结算本能运行服务&) = delete;
    服务值结算本能运行服务(服务值结算本能运行服务&&) = delete;
    服务值结算本能运行服务& operator=(服务值结算本能运行服务&&) = delete;
    新服务值治理初始化结果 初始化治理(稳定编码, 稳定编码, 稳定编码, 稳定编码) noexcept override;
    新服务值最终消费结果 执行最终来源工作(
        const 新服务值最终来源定位&) noexcept override;
    新本能函数本次条件结果对 执行贡献治理工作(
        const 新任务贡献反馈定位&) noexcept override;
    新服务值最终消费结果 读取最终来源结果(
        const 新服务值最终来源定位&) const noexcept override;
    void 报告并停止(std::string_view) noexcept override;
    void 报告待处理工程异常() noexcept override;
private:
    std::optional<新服务值结算登记材料> 读取登记材料() const noexcept;
    新服务值最终消费结果 消费正式来源(const 新服务值最终来源定位&) noexcept;
    std::optional<新服务值最终消费结果> 读取或收尾既有最终消费(
        const 新服务值最终来源定位&) noexcept;
    struct 实现;
    std::unique_ptr<实现> 实现_;
};
} // namespace 海中鱼巣
