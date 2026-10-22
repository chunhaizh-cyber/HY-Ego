#pragma once

#include <cstdint>
#include <memory>
#include <optional>

#include "../领域/连接.本能合同.h"

namespace 海中鱼巣 {

class 新_世界树类;
class 新_外设世界树类;
class 新_方法类;
class 新_任务类;
class 新_存在类;
class 新_特征类;
class 新_特征值类;
class 新_状态类;
class 新_动态类;
class 概念_存在类;
class 概念_特征类;

enum class 新连接状态 : std::uint8_t {
    待发送完成 = 1, 待接收, 待关系发布, 已确认关系, 已取消,
    输入拒绝, 条件未具备, 页面正被使用, 传输未完成, 来源失效, 前提已变化
};
struct 新连接请求 final {
    稳定编码 自我存在, 目标存在, 交互对象特征;
    稳定编码 现实场景, 外设窗口;
    std::optional<稳定编码> 本次起点接收状态;
};
struct 新连接返回 final {
    新连接状态 状态 = 新连接状态::输入拒绝;
    std::optional<稳定编码> 调用身份, 动作节点;
    std::optional<新本能函数本次条件结果对> 运行材料;
};

class 连接本能运行服务 final : public 新连接本能端口,
    public std::enable_shared_from_this<连接本能运行服务> {
public:
    连接本能运行服务(新_世界树类&, 新_外设世界树类&, 新_方法类&,
        新_存在类&, 新_特征类&, 新_特征值类&, 新_状态类&,
        新_动态类&, 概念_存在类&, 概念_特征类&, 新_任务类&) noexcept;
    ~连接本能运行服务() override;
    连接本能运行服务(const 连接本能运行服务&) = delete;
    连接本能运行服务& operator=(const 连接本能运行服务&) = delete;

    bool 初始化(const 新连接登记配置&) noexcept;
    新连接返回 开始(const 新连接请求&) noexcept;
    新连接返回 推进(稳定编码 调用身份) noexcept;
    新连接返回 取消(稳定编码 调用身份) noexcept;
    void 关闭() noexcept;
    std::optional<新连接登记配置> 读取登记配置() const noexcept override;
    新方法建立结果 确保方法登记(const 新方法知识建立请求&) noexcept override;
    新本能函数本次条件结果对 执行任务(
        const 方法参数场景&, 稳定编码 执行实例节点) noexcept override;
    新本能函数本次条件结果对 推进本次(
        const 方法参数场景&, 稳定编码 调用身份) noexcept override;
private:
    新连接返回 开始内部(const 新连接请求&, const 方法参数场景*,
        std::optional<稳定编码>) noexcept;
    struct 实现;
    std::unique_ptr<实现> 实现_;
};

} // namespace 海中鱼巣
