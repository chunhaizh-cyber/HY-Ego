#pragma once

#include <optional>

#include "新_方法公共类型.h"
#include "新_本能函数集.h"

namespace 海中鱼巣 {

class 方法参数场景;

struct 新连接登记配置 final {
    稳定编码 自我存在, 自我概念, 通用存在概念;
    稳定编码 交互对象类型, 接收消息类型, 发送消息类型;
    稳定编码 消息正文范围;
    稳定编码 输出队列类型;
    稳定编码 接收数量类型;
    // v4第6参数：交互窗口副本持有的外设场景标识特征类型（R91、R92）。
    稳定编码 外设场景标识类型;
    friend bool operator==(const 新连接登记配置&, const 新连接登记配置&) = default;
};

class 新连接本能端口 {
public:
    virtual ~新连接本能端口() = default;
    virtual std::optional<新连接登记配置> 读取登记配置() const noexcept = 0;
    virtual 新方法建立结果 确保方法登记(const 新方法知识建立请求&) noexcept = 0;
    virtual 新本能函数本次条件结果对 执行任务(
        const 方法参数场景&, 稳定编码 执行实例节点) noexcept = 0;
    virtual 新本能函数本次条件结果对 推进本次(
        const 方法参数场景&, 稳定编码 调用身份) noexcept = 0;
};

} // namespace 海中鱼巣
