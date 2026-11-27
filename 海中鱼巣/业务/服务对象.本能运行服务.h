#pragma once
#include <string_view>

#include "../领域/新_方法类.h"
#include "../领域/服务对象.材料合同.h"

namespace 海中鱼巣
{
class 新_世界树类;
class 新_外设世界树类;
struct 新外设网页临时场景来源结果;
class 新_任务类;
class 新_动态类;

// 所有借用服务须保活至调用结束；普通应用先归并自我，再关闭端口。
class 服务对象本能运行服务 final : public 新服务对象获取本能端口,
                                   public std::enable_shared_from_this<服务对象本能运行服务>
{
  public:
    服务对象本能运行服务(新_世界树类 &, 新_外设世界树类 &, 新_方法类 &, 新_任务类 &, 新_存在类 &, 新_特征类 &,
                         新_动态类 &) noexcept;
    ~服务对象本能运行服务();
    服务对象本能运行服务(const 服务对象本能运行服务 &) = delete;
    服务对象本能运行服务 &operator=(const 服务对象本能运行服务 &) = delete;
    bool 初始化(const 新服务对象获取配置 &) noexcept;
    // 正式按窗取材。自我核验确认时读取，返回外设所有者原结果。
    新外设网页临时场景来源结果 获取已确认窗口交互事实(
        std::string_view 会话, std::string_view 对话框) const noexcept;
    bool 关闭() noexcept;
    std::optional<稳定编码> 已登记方法节点() const noexcept;
    std::optional<新服务对象获取配置> 读取登记材料() const noexcept override;
    std::optional<稳定编码> 读取初始连接关系概念() const noexcept override;
    新方法建立结果 确保方法登记(const 新方法知识建立请求 &) noexcept override;
    新本能函数本次条件结果对 执行任务(const 方法参数场景 &, 稳定编码 执行实例节点) noexcept override;
    新方法建立结果 确保交互者方法登记(const 新方法知识建立请求 &) noexcept override;
    新本能函数本次条件结果对 执行交互者任务(const 方法参数场景 &, 稳定编码 执行实例节点) noexcept override;

  private:
    新方法建立结果 确保族方法登记(const 新方法知识建立请求 &, bool 交互者) noexcept;
    std::optional<std::vector<稳定编码>> 读取族参数(const 方法参数场景 &, 稳定编码, bool) const;
    bool 复核族条件(const 方法参数场景 &, 稳定编码, bool, const 新服务对象获取配置 &, 稳定编码,
        const std::vector<稳定编码> &) const;
    稳定编码 记录族动作(const 方法参数场景 &, 稳定编码, 稳定编码, 稳定编码, 稳定编码);
    bool 写族结果(新本能函数本次条件结果对 &, 稳定编码, 稳定编码, 稳定编码, 稳定编码, 稳定编码);
    struct 实现;
    std::unique_ptr<实现> 实现_;
};
} // namespace 海中鱼巣
