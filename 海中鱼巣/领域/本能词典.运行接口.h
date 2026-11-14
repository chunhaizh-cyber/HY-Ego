#pragma once

#include "新_本能函数集.h"

namespace 海中鱼巣 {

// 仅声明调用期能力；真实任务和隔离样例各自提供，不持有任务/模拟实现。
struct 新词典查阅动作结果 final {
    新词典运行状态 状态 = 新词典运行状态::输入拒绝;
    std::optional<稳定编码> 动作;
    std::optional<新本能函数本次条件> 实际条件;
    std::optional<新词典本能输入> 实际输入;
    std::vector<新词典学习缺口> 缺口组;
    std::vector<稳定编码> 变化依赖节点组;
};

struct 新词典查阅推进结果 final {
    新词典运行状态 状态 = 新词典运行状态::输入拒绝;
    std::optional<稳定编码> 动作;
    std::optional<新本能词典读取回执> 回执;
    std::vector<新词典学习缺口> 缺口组;
    std::vector<稳定编码> 变化依赖节点组;
    std::optional<新词典推进收束原因> 收束原因;
};

class 新本能词典运行接口 {
public:
    virtual ~新本能词典运行接口() = default;
    virtual 新词典本能输入读取结果 读取查阅输入(const 方法参数场景&) const = 0;
    virtual 新词典查阅动作结果 开始或取得查阅动作(const 方法参数场景&) = 0;
    virtual 新词典查阅推进结果 推进本次查阅(const 方法参数场景&) = 0;
    virtual bool 已请求停止() const noexcept = 0;
};

struct 新本能词典任务参数请求 final {
    稳定编码 方法节点, 执行实例节点;
    std::vector<新方法参数角色绑定> 参数绑定组;
    std::optional<稳定编码> 执行分支节点;
};

struct 新词典任务参数场景结果 final {
    新词典运行状态 状态 = 新词典运行状态::输入拒绝;
    std::shared_ptr<const 方法参数场景> 参数场景;
    std::vector<新词典学习缺口> 缺口组;
    std::vector<稳定编码> 变化依赖节点组;
};

struct 新词典学习来源 final {
    稳定编码 当前场景, 来源存在, 批次特征, 定位特征;
    std::string 可信批次标识;
    std::uint64_t 来源场景标识 = 0;
};
struct 新词典学习来源读取结果 final {
    新词典运行状态 状态 = 新词典运行状态::输入拒绝;
    std::optional<新词典学习来源> 来源;
    std::vector<新词典学习缺口> 缺口组;
    std::vector<稳定编码> 变化依赖节点组;
};

class 新本能词典任务参数工厂 {
public:
    virtual ~新本能词典任务参数工厂() = default;
    virtual bool 支持任务方法(稳定编码) const noexcept = 0;
    virtual 新词典学习来源读取结果 读取定向学习来源(稳定编码) const noexcept = 0;
    virtual 新词典任务参数场景结果 建立任务参数场景(
        const 新本能词典任务参数请求&, const 新内部治理对象读取接口*,
        std::optional<新方法任务成员绑定>) = 0;
    virtual 新词典任务参数场景结果 取得任务续接参数场景(稳定编码) = 0;
    virtual 新词典推进收束读取结果 读取任务推进收束(稳定编码) const noexcept = 0;
};

struct 新词典登记配置 final {
    稳定编码 词条全域, 词典材料全域, 通用存在范围;
    稳定编码 材料特征类型, 来源包特征类型, 外设场景标识类型;
};

// 只供空参数场景登记；执行必须取得参数场景自己的运行能力。
class 新词典登记端口 {
public:
    virtual ~新词典登记端口() = default;
    virtual std::optional<新词典登记配置> 读取登记材料() const noexcept = 0;
    virtual 新方法建立结果 确保方法登记(const 新方法知识建立请求&) noexcept = 0;
};

} // namespace 海中鱼巣
