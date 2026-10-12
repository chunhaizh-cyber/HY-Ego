#pragma once
#include <map>
#include <mutex>
#include "模拟学习.服务.h"
#include "../领域/方法问题反馈.数据.h"

namespace 海中鱼巣 {
struct 新方法问题学习收集结果 final {
    新方法问题学习消息 消息;
    std::shared_ptr<const 新方法问题反馈材料> 原始材料;
    bool 材料完整 = false;
};

// 只收集不可变的原始问题样本；不修改方法知识、命中或任务。
class 方法问题学习服务 final {
public:
    explicit 方法问题学习服务(const 新方法问题归档接口&) noexcept;
    新学习结束通知 学习(const 新方法问题学习消息&,
        const std::function<bool()>& 停止检查 = {});
    std::optional<新方法问题学习收集结果> 读取收集结果(
        稳定编码 归档节点) const noexcept;
private:
    const 新方法问题归档接口& 归档_;
    mutable std::mutex 互斥_;
    std::map<稳定编码, 新方法问题学习收集结果> 收集组_;
};
}
