#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

#include "../领域/连接.本能合同.h"
#include "../领域/本能交互.运行接口.h"

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

struct 新连接请求 final {
    稳定编码 自我存在, 目标存在, 交互对象特征;
    稳定编码 现实场景, 外设窗口;
};
struct 新连接返回 final {
    新连接状态 状态 = 新连接状态::输入拒绝;
    std::optional<稳定编码> 调用身份, 动作节点;
    std::optional<新本能函数本次条件结果对> 运行材料;
};

// 本次查询读回的过程证据，不表示永久连接，也不授予后继动作执行许可。

class 连接本能运行服务 final : public 新连接本能端口,
    public 新方法场景事实构成读取接口, public 新方法场景事实读取接口,
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
    std::vector<新连接事实> 读取已确认连接事实(
        稳定编码 现实场景, 稳定编码 自我存在,
        稳定编码 目标存在, 稳定编码 交互者特征) const noexcept;
    void 关闭() noexcept;
    新场景事实构成读取结果 读取场景事实构成(稳定编码) const override;
    新交互求值状态 读取场景事实(const 新方法初始场景事实限定&,
        const std::vector<新方法参数角色绑定>&) const override;
    新交互求值状态 读取本次结果场景事实(const 新方法本次场景事实引用&,
        const std::vector<新方法参数角色绑定>&, 稳定编码, 稳定编码,
        新交互连接过程&) const override;
    std::optional<新连接登记配置> 读取登记配置() const noexcept override;
    新方法建立结果 确保方法登记(const 新方法知识建立请求&) noexcept override;
    新本能函数本次条件结果对 执行任务(
        const 方法参数场景&, 稳定编码 执行实例节点) noexcept override;
    新本能函数本次条件结果对 推进本次(
        const 方法参数场景&, 稳定编码 调用身份) noexcept override;
private:
    新交互求值状态 投影连接过程证据(const 新连接事实&,新交互连接过程&) const;
    新连接返回 开始内部(const 新连接请求&, const 方法参数场景*,
        std::optional<稳定编码>) noexcept;
    struct 实现;
    std::unique_ptr<实现> 实现_;
};

} // namespace 海中鱼巣
