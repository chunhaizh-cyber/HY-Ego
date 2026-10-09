#pragma once

#include <cstdint>
#include <functional>
#include <future>
#include <memory>
#include <vector>

#include "../领域/新_需求类.h"
#include "../领域/新_任务类.h"

namespace 海中鱼巣 {

enum class 新自我工作生命周期 : std::uint8_t {
    未启动, 启动中, 等待工作, 处理工作, 停止中, 已停止, 启动失败, 内部错误
};

enum class 新自我工作操作状态 : std::uint8_t {
    已完成 = 1, 无变化, 入口拒绝, 已在运行, 尚未启动,
    等待超时, 不能等待自身, 资源失败, 内部错误
};

struct 新自我工作操作结果 final {
    新自我工作操作状态 状态 = 新自我工作操作状态::入口拒绝;
    新自我工作生命周期 生命周期 = 新自我工作生命周期::未启动;
};

enum class 新自我工作种类 : std::uint8_t {
    需求任务数量核对 = 1,
    需求树全局扫描 = 2
};

struct 新自我需求树扫描请求 final {
    std::uint64_t 工作编号 = 0;
    新自我工作种类 种类 = 新自我工作种类::需求树全局扫描;
    friend bool operator==(const 新自我需求树扫描请求&,
        const 新自我需求树扫描请求&) = default;
};

enum class 新自我需求树扫描状态 : std::uint8_t {
    已完成 = 1,
    需求服务未就绪,
    任务服务未就绪,
    需求树结构不一致,
    任务结构不一致,
    停止取消,
    资源失败,
    内部错误
};

// 本结果只是一次current-only完整读取形成的有界运行材料，不是第二需求账
// 或任务账。需求和任务的正式事实仍分别由需求owner和任务owner持有。
struct 新自我需求树扫描结果 final {
    新自我需求树扫描请求 原请求;
    新自我需求树扫描状态 状态 = 新自我需求树扫描状态::内部错误;
    std::uint64_t 需求数量 = 0;
    std::uint64_t 任务数量 = 0;
    std::vector<稳定编码> 需求节点组;
    std::vector<稳定编码> 缺少任务的需求节点组;
    std::vector<稳定编码> 多任务需求节点组;
    std::vector<稳定编码> 无需求任务节点组;
};

enum class 新自我工作接收状态 : std::uint8_t {
    已接收 = 1, 精确重复, 身份冲突, 已有扫描, 尚未启动,
    已停止接收, 入口拒绝, 资源失败, 内部错误
};

struct 新自我工作接收结果 final {
    新自我工作接收状态 状态 = 新自我工作接收状态::入口拒绝;
    std::shared_future<新自我需求树扫描结果> 处理结果;
};

struct 新自我工作快照 final {
    新自我工作生命周期 生命周期 = 新自我工作生命周期::未启动;
    bool 有活动扫描 = false;
    bool 停止已请求 = false;
    bool 线程已完成 = false;
};

// self消息线程只提交工作和消费结果；本线程串行执行完整读取与比对，
// 不建立任务、不调用manager，也不发送业务消息。
class 新_自我工作线程类 final {
public:
    新_自我工作线程类(
        新_需求类& 需求服务,
        新_任务类& 任务服务,
        std::function<void()> 完成通知 = {}) noexcept;
    ~新_自我工作线程类() noexcept;
    新_自我工作线程类(const 新_自我工作线程类&) = delete;
    新_自我工作线程类& operator=(const 新_自我工作线程类&) = delete;

    新自我工作操作结果 启动(std::uint64_t 等待毫秒) noexcept;
    新自我工作接收结果 提交工作(
        const 新自我需求树扫描请求& 请求) noexcept;
    新自我工作操作状态 确认结果已接收(std::uint64_t 工作编号) noexcept;
    新自我工作操作结果 请求停止() noexcept;
    新自我工作操作结果 等待停止(std::uint64_t 等待毫秒) noexcept;
    新自我工作快照 读取快照() const noexcept;

private:
    struct 实现;
    std::unique_ptr<实现> 实现_;
};

} // namespace 海中鱼巣
