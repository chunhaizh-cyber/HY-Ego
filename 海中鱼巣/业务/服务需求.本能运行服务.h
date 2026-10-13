#pragma once
#include <memory>
#include "../领域/新_方法类.h"
#include "../领域/服务要求.材料合同.h"

namespace 海中鱼巣 {
class 新_世界树类;
class 新_任务类;
class 新_动态类;

// owner及材料读取接口必须覆盖本服务、参数场景和所有在途调用的生命期。
// 初始化调用者须提供语义已定义的信息包复合概念及五个角色概念。
class 服务需求本能运行服务 final : public 新服务需求获取本能端口,
    public std::enable_shared_from_this<服务需求本能运行服务> {
public:
    服务需求本能运行服务(新_世界树类&, 新_方法类&, 新_任务类&, 新_动态类&,
        概念_存在类&, 概念_特征类&, const 新服务要求材料读取接口&,
        新服务需求工程报告函数) noexcept;
    ~服务需求本能运行服务();
    服务需求本能运行服务(const 服务需求本能运行服务&) = delete;
    服务需求本能运行服务& operator=(const 服务需求本能运行服务&) = delete;
    bool 初始化(const 新服务需求获取配置&) noexcept;
    // 关闭拒绝新调用，在途调用由共享租约保活；关闭后的owner不能重新初始化。
    bool 关闭() noexcept;
    新方法本能调用结果 执行(稳定编码 方法节点,
        const 方法参数场景&, 稳定编码 执行实例) noexcept;
    std::optional<新服务需求获取配置> 读取登记材料() const noexcept override;
    新方法建立结果 确保方法登记(新服务需求获取方式,
        const 新方法知识建立请求&) noexcept override;
    bool 核验执行(新服务需求获取方式, const 方法参数场景&) const noexcept override;
    新服务需求获取动作回执 建立动作(新服务需求获取方式,
        const 方法参数场景&, 稳定编码 来源人类节点) noexcept override;
    void 报告工程故障(std::string_view) noexcept override;
private:
    struct 实现;
    std::unique_ptr<实现> 实现_;
};
} // namespace 海中鱼巣
