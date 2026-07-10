// 文件规则：运行宿主只管理工程生命周期和调度材料；不得成为动作来源、写业务结构或启动业务线程。
#pragma once

#include "运行消息队列.h"

#include <atomic>
#include <cstdint>
#include <thread>

namespace 海中鱼巣 {

enum class 运行宿主状态 : std::uint32_t {
    未启动 = 0,
    运行中 = 1,
    正在停止 = 2,
    已停止 = 3,
    启动失败 = 4
};

enum class 运行宿主拒绝原因 : std::uint32_t {
    无 = 0,
    重复启动 = 1,
    队列容量为零 = 2,
    队列已停止 = 3,
    停止消息失败 = 4,
    未启动 = 5,
    已经收口 = 6
};

struct 运行宿主操作结果 {
    bool 成功 = false;
    bool 局部失败 = true;
    bool 写业务事实 = false;
    运行宿主拒绝原因 拒绝原因 = 运行宿主拒绝原因::无;
};

class 运行宿主 {
public:
    explicit 运行宿主(std::uint64_t 队列容量)
        : 消息队列(队列容量) {}

    ~运行宿主() {
        if (宿主线程.joinable()) {
            请求停止.store(true);
            宿主线程.join();
        }
    }

    运行宿主(const 运行宿主&) = delete;
    运行宿主& operator=(const 运行宿主&) = delete;

    运行宿主状态 当前状态() const {
        return 状态.load();
    }

    bool 线程可收口() const {
        return !宿主线程.joinable();
    }

    const 有界运行消息队列& 只读消息队列() const {
        return 消息队列;
    }

    运行宿主操作结果 启动(std::uint64_t 调度消息编号, std::uint64_t 调度时间戳) {
        if (状态.load() == 运行宿主状态::运行中) {
            return 拒绝(运行宿主拒绝原因::重复启动);
        }
        if (消息队列.容量() == 0) {
            状态.store(运行宿主状态::启动失败);
            return 拒绝(运行宿主拒绝原因::队列容量为零);
        }
        if (消息队列.已停止()) {
            状态.store(运行宿主状态::启动失败);
            return 拒绝(运行宿主拒绝原因::队列已停止);
        }

        运行消息 调度消息;
        调度消息.消息编号 = 调度消息编号;
        调度消息.类型 = 运行消息类型::调度周期;
        调度消息.来源角色 = 运行线程角色::运行宿主;
        调度消息.目标角色 = 运行线程角色::任务管理;
        调度消息.发生时间戳 = 调度时间戳;
        调度消息.优先级 = 运行消息优先级::普通;
        调度消息.幂等键 = 调度消息编号;
        调度消息.材料摘要 = 调度消息编号;
        调度消息.只读材料说明 = true;
        调度消息.承载机器事实 = false;

        const auto 入队 = 消息队列.入队(调度消息);
        if (!入队.已入队) {
            状态.store(运行宿主状态::启动失败);
            return 拒绝(运行宿主拒绝原因::队列已停止);
        }

        请求停止.store(false);
        线程已进入.store(false);
        状态.store(运行宿主状态::运行中);
        宿主线程 = std::thread([this]() {
            线程已进入.store(true);
            while (!请求停止.load()) {
                std::this_thread::yield();
            }
        });
        return {true, false, false, 运行宿主拒绝原因::无};
    }

    运行宿主操作结果 请求停止并发送停止消息(std::uint64_t 停止消息编号, std::uint64_t 停止时间戳) {
        if (状态.load() == 运行宿主状态::未启动) {
            return 拒绝(运行宿主拒绝原因::未启动);
        }
        if (状态.load() == 运行宿主状态::已停止) {
            return 拒绝(运行宿主拒绝原因::已经收口);
        }

        运行消息 停止消息;
        停止消息.消息编号 = 停止消息编号;
        停止消息.类型 = 运行消息类型::停止;
        停止消息.来源角色 = 运行线程角色::运行宿主;
        停止消息.目标角色 = 运行线程角色::运行宿主;
        停止消息.发生时间戳 = 停止时间戳;
        停止消息.优先级 = 运行消息优先级::停止;
        停止消息.幂等键 = 停止消息编号;
        停止消息.材料摘要 = 停止消息编号;
        停止消息.只读材料说明 = true;
        停止消息.承载机器事实 = false;

        const auto 入队 = 消息队列.入队(停止消息);
        if (!入队.已入队) {
            return 拒绝(运行宿主拒绝原因::停止消息失败);
        }

        状态.store(运行宿主状态::正在停止);
        请求停止.store(true);
        return {true, false, false, 运行宿主拒绝原因::无};
    }

    运行宿主操作结果 收口等待() {
        if (宿主线程.joinable()) {
            宿主线程.join();
        }
        if (状态.load() == 运行宿主状态::正在停止 || 状态.load() == 运行宿主状态::运行中) {
            状态.store(运行宿主状态::已停止);
        }
        return {true, false, false, 运行宿主拒绝原因::无};
    }

    bool 宿主线程已进入() const {
        return 线程已进入.load();
    }

private:
    有界运行消息队列 消息队列;
    std::atomic<运行宿主状态> 状态 = 运行宿主状态::未启动;
    std::atomic<bool> 请求停止 = false;
    std::atomic<bool> 线程已进入 = false;
    std::thread 宿主线程;

    运行宿主操作结果 拒绝(运行宿主拒绝原因 原因) const {
        return {false, true, false, 原因};
    }
};

}
