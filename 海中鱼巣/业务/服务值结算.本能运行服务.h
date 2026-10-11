#pragma once
#include <memory>
#include "../领域/服务值结算.本能合同.h"

namespace 海中鱼巣 {
class 新_世界树类;
class 新_方法类;
class 新_任务类;
class 新_场景类;
class 新_动态类;
class 服务值结算本能运行服务 final : public 新服务值结算本能端口, public 新服务值结算确认接口 {
public:
    服务值结算本能运行服务(新_世界树类&, 新_方法类&, 新_任务类&, 新_需求类&,
        新_场景类&, 新_动态类&, 新_特征类&, 概念_特征类&, 概念_二次特征类&,
        新服务值结算工程报告函数) noexcept;
    ~服务值结算本能运行服务();
    服务值结算本能运行服务(const 服务值结算本能运行服务&) = delete;
    服务值结算本能运行服务& operator=(const 服务值结算本能运行服务&) = delete;
    服务值结算本能运行服务(服务值结算本能运行服务&&) = delete;
    服务值结算本能运行服务& operator=(服务值结算本能运行服务&&) = delete;
    新服务值结算登记结果 初始化(稳定编码, 稳定编码, 稳定编码, 稳定编码) noexcept override;
    std::optional<新服务值结算登记材料> 读取登记材料() const noexcept override;
    新方法建立结果 确保方法登记(const 新方法知识建立请求&) noexcept override;
    新需求建立结果 确保首子需求(稳定编码) noexcept override;
    新服务值结算动作回执 建立普通动作(稳定编码, const 新任务虚拟存在引用&,
        稳定编码, 稳定编码, 稳定编码, std::int64_t) noexcept override;
    新服务值结算变化回执 更新普通结果(稳定编码, 稳定编码, 稳定编码,
        稳定编码, std::int64_t) noexcept override;
    新服务值结算确认结果 确认并收尾(稳定编码, 稳定编码, 稳定编码) noexcept override;
    void 报告并停止(std::string_view) noexcept override;
    void 报告待处理工程异常() noexcept override;
private:
    struct 实现;
    std::unique_ptr<实现> 实现_;
};
} // namespace 海中鱼巣
