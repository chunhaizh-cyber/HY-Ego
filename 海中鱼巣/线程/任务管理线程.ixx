// 文件规则：任务管理线程只接收和排队任务请求材料；不得执行方法、裁决任务结果、写业务结构或定义其他线程角色。
module;

#include <atomic>
#include <cstdint>
#include <thread>

export module 海中鱼巣.线程.任务管理线程;

import 海中鱼巣.线程.运行消息协议;
import 海中鱼巣.线程.有界运行消息队列;

export namespace 海中鱼巣 {

enum class 任务管理线程状态 : std::uint32_t {
    未启动 = 0,
    运行中 = 1,
    正在停止 = 2,
    已停止 = 3,
    启动失败 = 4
};

enum class 任务管理线程拒绝原因 : std::uint32_t {
    无 = 0,
    重复启动 = 1,
    队列容量为零 = 2,
    未启动 = 3,
    停止消息失败 = 4,
    非任务执行请求 = 5,
    请求入队失败 = 6
};

struct 任务管理线程操作结果 {
    bool 成功 = false;
    bool 局部失败 = true;
    bool 写业务事实 = false;
    任务管理线程拒绝原因 拒绝原因 = 任务管理线程拒绝原因::无;
};

class 任务管理线程 {
public:
    explicit 任务管理线程(std::uint64_t 调度队列容量)
        : 任务调度队列(调度队列容量) {}

    ~任务管理线程() {
        请求停止.store(true);
        if (线程.joinable()) {
            线程.join();
        }
    }

    任务管理线程(const 任务管理线程&) = delete;
    任务管理线程& operator=(const 任务管理线程&) = delete;

    任务管理线程状态 当前状态() const {
        return 状态.load();
    }

    bool 线程已进入() const {
        return 已进入.load();
    }

    bool 线程可收口() const {
        return !线程.joinable();
    }

    const 有界运行消息队列& 只读任务调度队列() const {
        return 任务调度队列;
    }

    任务管理线程操作结果 启动() {
        if (状态.load() == 任务管理线程状态::运行中) {
            return 拒绝(任务管理线程拒绝原因::重复启动);
        }
        if (任务调度队列.容量() == 0) {
            状态.store(任务管理线程状态::启动失败);
            return 拒绝(任务管理线程拒绝原因::队列容量为零);
        }

        请求停止.store(false);
        已进入.store(false);
        状态.store(任务管理线程状态::运行中);
        线程 = std::thread([this]() {
            已进入.store(true);
            while (!请求停止.load()) {
                std::this_thread::yield();
            }
        });
        return {true, false, false, 任务管理线程拒绝原因::无};
    }

    任务管理线程操作结果 发送任务执行请求(const 运行消息& 请求消息) {
        if (状态.load() != 任务管理线程状态::运行中) {
            return 拒绝(任务管理线程拒绝原因::未启动);
        }
        if (请求消息.类型 != 运行消息类型::任务执行请求) {
            return 拒绝(任务管理线程拒绝原因::非任务执行请求);
        }
        const auto 入队 = 任务调度队列.入队(请求消息);
        if (!入队.已入队) {
            return 拒绝(任务管理线程拒绝原因::请求入队失败);
        }
        return {true, false, false, 任务管理线程拒绝原因::无};
    }

    出队结果 取出任务执行请求() {
        if (状态.load() != 任务管理线程状态::运行中) {
            return {};
        }
        return 任务调度队列.出队();
    }

    任务管理线程操作结果 请求停止并发送停止消息(
        std::uint64_t 停止消息编号, std::uint64_t 停止时间戳) {
        if (状态.load() == 任务管理线程状态::未启动) {
            return 拒绝(任务管理线程拒绝原因::未启动);
        }

        运行消息 停止消息;
        停止消息.消息编号 = 停止消息编号;
        停止消息.类型 = 运行消息类型::停止;
        停止消息.来源角色 = 运行线程角色::运行宿主;
        停止消息.目标角色 = 运行线程角色::任务管理;
        停止消息.发生时间戳 = 停止时间戳;
        停止消息.优先级 = 运行消息优先级::停止;
        停止消息.幂等键 = 停止消息编号;
        停止消息.材料摘要 = 停止消息编号;
        停止消息.只读材料说明 = true;
        停止消息.承载机器事实 = false;

        const auto 入队 = 任务调度队列.入队(停止消息);
        if (!入队.已入队) {
            return 拒绝(任务管理线程拒绝原因::停止消息失败);
        }

        状态.store(任务管理线程状态::正在停止);
        请求停止.store(true);
        return {true, false, false, 任务管理线程拒绝原因::无};
    }

    任务管理线程操作结果 收口等待() {
        if (线程.joinable()) {
            线程.join();
        }
        if (状态.load() == 任务管理线程状态::正在停止
            || 状态.load() == 任务管理线程状态::运行中) {
            状态.store(任务管理线程状态::已停止);
        }
        return {true, false, false, 任务管理线程拒绝原因::无};
    }

private:
    有界运行消息队列 任务调度队列;
    std::atomic<任务管理线程状态> 状态 = 任务管理线程状态::未启动;
    std::atomic<bool> 请求停止 = false;
    std::atomic<bool> 已进入 = false;
    std::thread 线程;

    任务管理线程操作结果 拒绝(任务管理线程拒绝原因 原因) const {
        return {false, true, false, 原因};
    }
};

}
