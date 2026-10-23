#pragma once
#include <memory>
#include "../领域/新_方法类.h"
#include "../领域/服务要求.材料合同.h"

namespace 海中鱼巣 {
class 新_世界树类;
class 新_任务类;
class 新_动态类;
class 新_自我线程类;
class 新_任务管理线程类;
class 服务对象需求发布服务;
class 连接本能运行服务;

// owner及材料读取接口必须覆盖本服务、参数场景和所有在途调用的生命期。
// 宿主须在依赖销毁前显式关闭端口并停止、join工作线程；析构只收束自身，不注销注册表。
// 初始化调用者提供正式范围概念；六个参数按有序规格定位，不再使用材料角色占位。
class 服务需求本能运行服务 final : public 新服务需求获取本能端口,
    public std::enable_shared_from_this<服务需求本能运行服务> {
public:
    服务需求本能运行服务(新_世界树类&, 新_方法类&, 新_任务类&, 新_需求类&, 新_动态类&,
        服务对象需求发布服务&, 连接本能运行服务&, const 新服务要求材料读取接口&,
        const 新部分服务要求读取接口&,
        新_自我线程类&, 新_任务管理线程类&,
        新服务需求工程报告函数) noexcept;
    ~服务需求本能运行服务();
    服务需求本能运行服务(const 服务需求本能运行服务&) = delete;
    服务需求本能运行服务& operator=(const 服务需求本能运行服务&) = delete;
    bool 初始化(const 新服务需求获取配置&) noexcept;
    // 关闭拒绝新调用，在途调用由共享租约保活；关闭后的owner不能重新初始化。
    bool 关闭() noexcept;
    // 独立调用包装；通用W3已持有守卫时直接调用方法owner，不嵌套本入口。
    新方法本能调用结果 执行(稳定编码 方法节点,
        const 方法参数场景&, 稳定编码 执行实例) noexcept;
    std::optional<新服务需求获取配置> 读取登记材料() const noexcept override;
    新方法建立结果 确保方法登记(新服务需求获取方式,
        const 新方法知识建立请求&) noexcept override;
    新本能函数本次条件结果对 执行任务(新服务需求获取方式,
        const 方法参数场景&, 稳定编码 执行实例) noexcept override;
    void 报告工程故障(std::string_view) noexcept override;
private:
    新服务需求执行回执 执行服务需求采用(新服务需求获取方式,
        const 方法参数场景&, 稳定编码 动作节点, 稳定编码 对方需求节点) noexcept;
    struct 实现;
    std::unique_ptr<实现> 实现_;
};
} // namespace 海中鱼巣
