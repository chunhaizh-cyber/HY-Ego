#pragma once

#include "../领域/新_方法类.h"
#include "../领域/服务对象.材料合同.h"

namespace 海中鱼巣
{
class 新_世界树类;
class 新_任务类;
class 新_动态类;
class 连接本能运行服务;

// 所有非拥有owner及来源回调必须覆盖全部调用；应用先join，再关闭端口。
class 服务对象本能运行服务 final : public 新服务对象获取本能端口,
                                   public std::enable_shared_from_this<服务对象本能运行服务>
{
  public:
    服务对象本能运行服务(新_世界树类 &, 新_方法类 &, 新_任务类 &, 新_存在类 &, 新_特征类 &, 新_动态类 &,
                         连接本能运行服务 &) noexcept;
    ~服务对象本能运行服务();
    服务对象本能运行服务(const 服务对象本能运行服务 &) = delete;
    服务对象本能运行服务 &operator=(const 服务对象本能运行服务 &) = delete;
    bool 初始化(const 新服务对象获取配置 &) noexcept;
    bool 关闭() noexcept;
    std::optional<稳定编码> 已登记方法节点() const noexcept;
    std::optional<新服务对象获取配置> 读取登记材料() const noexcept override;
    新方法建立结果 确保方法登记(const 新方法知识建立请求 &) noexcept override;
    新本能函数本次条件结果对 执行任务(const 方法参数场景 &, 稳定编码 执行实例节点) noexcept override;

  private:
    struct 实现;
    std::unique_ptr<实现> 实现_;
};
} // namespace 海中鱼巣
