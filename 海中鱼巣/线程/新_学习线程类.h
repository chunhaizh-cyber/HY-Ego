#pragma once
#include <cstdint>
#include <functional>
#include <future>
#include <memory>
#include <optional>
#include "../业务/模拟学习.服务.h"
#include "../业务/方法问题学习.服务.h"

namespace 海中鱼巣 {
enum class 新学习线程状态 : std::uint8_t {
    未启动, 等待工作, 正在学习, 停止中, 已停止, 内部错误
};
enum class 新学习操作状态 : std::uint8_t {
    已完成=1, 无变化, 入口拒绝, 尚未启动, 已停止接收,
    精确重复, 等待超时, 不能等待自身, 资源失败, 内部错误
};
struct 新学习接收结果 final {
    新学习操作状态 状态=新学习操作状态::入口拒绝;
    std::shared_future<新学习结束通知> 处理结果;
};
struct 新学习线程快照 final {
    新学习线程状态 状态=新学习线程状态::未启动;
    std::uint64_t 待处理数量=0;
    std::optional<稳定编码> 当前方法;
    bool 停止已请求=false;
};

// 服务覆盖线程寿命；已接收工作保留至接收者确认，重复方法共用同一future。
class 新_学习线程类 final {
public:
    新_学习线程类(模拟学习服务&, std::function<void()> 完成通知={}) noexcept;
    新_学习线程类(模拟学习服务&, 方法问题学习服务&,
        std::function<void()> 完成通知={}) noexcept;
    ~新_学习线程类() noexcept;
    新_学习线程类(const 新_学习线程类&)=delete;
    新_学习线程类& operator=(const 新_学习线程类&)=delete;
    新学习操作状态 启动(std::uint64_t 等待毫秒) noexcept;
    新学习接收结果 提交学习(稳定编码 方法节点) noexcept;
    新学习接收结果 提交方法问题学习(const 新方法问题学习消息&) noexcept;
    新学习操作状态 确认方法问题结果已接收(稳定编码 归档节点) noexcept;
    std::optional<新方法问题学习收集结果> 读取方法问题收集结果(
        稳定编码 归档节点) const noexcept;
    新学习操作状态 确认结果已接收(稳定编码 方法节点) noexcept;
    新学习操作状态 请求停止() noexcept;
    新学习操作状态 等待停止(std::uint64_t 等待毫秒) noexcept;
    新学习线程快照 读取快照() const noexcept;
private:
    struct 实现;
    std::unique_ptr<实现> 实现_;
};
}
