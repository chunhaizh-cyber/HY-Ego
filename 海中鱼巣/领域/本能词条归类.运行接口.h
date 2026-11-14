#pragma once

#include "新_本能函数集.h"
#include "概念_存在类.h"

namespace 海中鱼巣 {

struct 新词条归类动作结果 final {
    新词条归类运行状态 状态 = 新词条归类运行状态::条件未具备;
    std::optional<稳定编码> 动作;
    std::optional<新本能函数本次条件> 实际条件;
    std::optional<新词条归类本能输入> 实际输入;
    std::vector<稳定编码> 变化依赖节点组;
};

struct 新词条归类提交结果 final {
    新词条归类运行状态 状态 = 新词条归类运行状态::条件未具备;
    std::optional<新本能词条归类回执> 回执;
    std::vector<稳定编码> 变化依赖节点组;
};

// 真实调用和隔离调用各自持有实现，函数只使用传入场景的能力。
class 新本能词条归类运行接口 {
public:
    virtual ~新本能词条归类运行接口() = default;
    virtual 新词条归类动作结果 开始或取得归类动作(const 方法参数场景&) = 0;
    virtual 新词条归类提交结果 提交或取得归类结果(const 方法参数场景&) = 0;
};

struct 新词条归类登记配置 final {
    稳定编码 词条全域, 通用存在范围, 正式概念全域, 挂接特征类型;
    新词典归类信息概念组 归类范围;
};

class 新词条归类登记端口 {
public:
    virtual ~新词条归类登记端口() = default;
    virtual std::optional<新词条归类登记配置> 读取登记材料() const noexcept = 0;
    virtual 新方法建立结果 确保方法登记(const 新方法知识建立请求&) noexcept = 0;
};

struct 新本能词条归类任务参数请求 final {
    稳定编码 方法节点, 任务节点, 执行实例节点;
    std::vector<新方法参数角色绑定> 参数绑定组;
    std::optional<稳定编码> 执行分支节点;
    std::optional<新词典归类需求来源> 来源;
};
struct 新词条归类任务参数场景结果 final {
    新词条归类运行状态 状态 = 新词条归类运行状态::条件未具备;
    std::shared_ptr<const 方法参数场景> 参数场景;
    std::vector<稳定编码> 变化依赖节点组;
};
class 新本能词条归类任务参数工厂 {
public:
    virtual ~新本能词条归类任务参数工厂() = default;
    virtual bool 支持任务方法(稳定编码) const noexcept = 0;
    virtual 新词条归类任务参数场景结果 建立任务参数场景(
        const 新本能词条归类任务参数请求&,const 新内部治理对象读取接口*,
        std::optional<新方法任务成员绑定>) = 0;
    virtual 新词条归类任务参数场景结果 取得任务续接参数场景(稳定编码) = 0;
};

} // namespace 海中鱼巣
