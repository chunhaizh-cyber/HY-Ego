#pragma once

#include <memory>
#include <optional>

#include "../领域/进入场景.本能合同.h"

namespace 海中鱼巣 {

class 新_世界树类;
class 新_方法类;

// 通用本能“进入场景”的运行服务（6220 §2、R94/R95）。
// 非空间分支：动作只写自我“所在场景”，随后经世界树 owner 迁入自我并维护场景“参与自我”。
// 空间分支（场景带坐标范围特征）当前未支持；现行代码尚无坐标范围特征类型。
class 进入场景本能运行服务 final : public 新进入场景本能端口 {
public:
    进入场景本能运行服务(新_世界树类&, 新_方法类&) noexcept;
    ~进入场景本能运行服务() override;
    进入场景本能运行服务(const 进入场景本能运行服务&) = delete;
    进入场景本能运行服务& operator=(const 进入场景本能运行服务&) = delete;

    bool 初始化(const 新进入场景登记配置&) noexcept;
    std::optional<稳定编码> 已登记方法节点() const noexcept;

    std::optional<新进入场景登记配置> 读取登记配置() const noexcept override;
    新方法建立结果 确保方法登记(const 新方法知识建立请求&) noexcept override;
    新本能函数本次条件结果对 执行任务(
        const 方法参数场景&, 稳定编码 执行实例节点) noexcept override;
private:
    struct 实现;
    std::unique_ptr<实现> 实现_;
};

} // namespace 海中鱼巣
