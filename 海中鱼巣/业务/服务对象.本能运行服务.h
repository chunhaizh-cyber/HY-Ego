#pragma once
#include <string>
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

enum class 新窗口服务观察状态 : std::uint8_t {
    等待正式确认, 等待对端, 概念缺判别定义, 对端归属材料不足, 明确非人类, 人类待权限登记, 前提已变化, 合格待正式恢复
};
struct 新窗口服务观察 final {
    稳定编码 外设场景;
    std::optional<稳定编码> 窗口, 确认, 外设对端, 内部场景, 内部对端, 人类概念;
    新窗口服务观察状态 状态 = 新窗口服务观察状态::等待正式确认;
    bool 已由新窗触发 = false;
    std::string 内部会话;
    std::optional<稳定编码> 最近材料;
    std::int64_t 材料时刻下限 = 0;
    std::uint64_t 材料续判次数 = 0;
    bool 来源已关闭 = false;
    std::optional<稳定编码> 登记认知来源, 登记认知存在;
    std::uint64_t 续办次数 = 0;
};

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
    新窗口服务观察 处理新外设窗口(稳定编码 外设场景) const noexcept;
    std::optional<新窗口服务观察> 确认并登记本窗服务对象(稳定编码 外设场景) const noexcept;
    bool 绑定正式服务对象特征(稳定编码) noexcept;
    std::optional<新窗口服务观察> 续判同窗正式材料(const 新窗口服务观察&, 稳定编码 状态节点) const noexcept;
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
    void 重读窗口资格(新窗口服务观察&) const;
    struct 实现;
    std::unique_ptr<实现> 实现_;
};
} // namespace 海中鱼巣
