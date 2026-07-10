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

enum class 任务线程壳状态 : std::uint32_t {
    未启动 = 0,
    运行中 = 1,
    正在停止 = 2,
    已停止 = 3,
    启动失败 = 4
};

enum class 任务线程壳拒绝原因 : std::uint32_t {
    无 = 0,
    重复启动 = 1,
    队列容量为零 = 2,
    未启动 = 3,
    停止消息失败 = 4,
    非任务执行请求 = 5,
    请求入队失败 = 6,
    回执入队失败 = 7,
    无请求材料 = 8
};

enum class 缓存事件线程壳状态 : std::uint32_t {
    未启动 = 0,
    运行中 = 1,
    正在停止 = 2,
    已停止 = 3,
    启动失败 = 4
};

enum class 缓存事件线程壳拒绝原因 : std::uint32_t {
    无 = 0,
    重复启动 = 1,
    队列容量为零 = 2,
    未启动 = 3,
    停止消息失败 = 4,
    非缓存刷新消息 = 5,
    非事件日志消息 = 6,
    缓存消息入队失败 = 7,
    事件消息入队失败 = 8,
    无缓存刷新材料 = 9,
    无事件日志材料 = 10
};

enum class 外设采样材料线程壳状态 : std::uint32_t {
    未启动 = 0,
    运行中 = 1,
    正在停止 = 2,
    已停止 = 3,
    启动失败 = 4
};

enum class 外设采样材料线程壳拒绝原因 : std::uint32_t {
    无 = 0,
    重复启动 = 1,
    队列容量为零 = 2,
    未启动 = 3,
    停止消息失败 = 4,
    非外部材料消息 = 5,
    非外设采样材料来源 = 6,
    材料入队失败 = 7,
    无外部材料 = 8,
    非过期材料 = 9
};

struct 运行宿主操作结果 {
    bool 成功 = false;
    bool 局部失败 = true;
    bool 写业务事实 = false;
    运行宿主拒绝原因 拒绝原因 = 运行宿主拒绝原因::无;
};

struct 任务线程壳操作结果 {
    bool 成功 = false;
    bool 局部失败 = true;
    bool 写业务事实 = false;
    任务线程壳拒绝原因 拒绝原因 = 任务线程壳拒绝原因::无;
};

struct 任务线程壳消费结果 {
    bool 成功 = false;
    bool 局部失败 = true;
    bool 写业务事实 = false;
    bool 裸写仓库 = false;
    bool 裁决任务完成 = false;
    bool 裁决需求满足 = false;
    bool 裁决方法成功 = false;
    bool 需要领域服务复核 = true;
    任务线程壳拒绝原因 拒绝原因 = 任务线程壳拒绝原因::无;
    运行消息 请求消息;
    运行消息 回执消息;
};

struct 缓存事件线程壳操作结果 {
    bool 成功 = false;
    bool 局部失败 = true;
    bool 写业务事实 = false;
    缓存事件线程壳拒绝原因 拒绝原因 = 缓存事件线程壳拒绝原因::无;
};

struct 缓存事件线程壳消费结果 {
    bool 成功 = false;
    bool 局部失败 = true;
    bool 写业务事实 = false;
    bool 裸写仓库 = false;
    bool 裁决业务事实 = false;
    bool 修复结构 = false;
    bool 恢复运行期结构 = false;
    bool 只产生审计或缓存材料 = false;
    缓存事件线程壳拒绝原因 拒绝原因 = 缓存事件线程壳拒绝原因::无;
    运行消息 消费消息;
};

struct 外设采样材料线程壳操作结果 {
    bool 成功 = false;
    bool 局部失败 = true;
    bool 写业务事实 = false;
    bool 写世界事实 = false;
    bool 接入真实外设 = false;
    bool 接入D455 = false;
    bool 接入体素 = false;
    bool 触发外设动作 = false;
    外设采样材料线程壳拒绝原因 拒绝原因 = 外设采样材料线程壳拒绝原因::无;
};

struct 外设采样材料线程壳消费结果 {
    bool 成功 = false;
    bool 局部失败 = true;
    bool 写业务事实 = false;
    bool 写世界事实 = false;
    bool 裸写仓库 = false;
    bool 裁决机器事实 = false;
    bool 接入真实外设 = false;
    bool 接入D455 = false;
    bool 接入体素 = false;
    bool 触发外设动作 = false;
    bool 丢弃过期材料 = false;
    bool 只产生外部材料 = false;
    外设采样材料线程壳拒绝原因 拒绝原因 = 外设采样材料线程壳拒绝原因::无;
    运行消息 材料消息;
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

class 外设采样材料线程壳 {
public:
    explicit 外设采样材料线程壳(std::uint64_t 材料队列容量)
        : 外部材料队列(材料队列容量) {}

    ~外设采样材料线程壳() {
        请求停止.store(true);
        if (采样材料线程.joinable()) {
            采样材料线程.join();
        }
    }

    外设采样材料线程壳(const 外设采样材料线程壳&) = delete;
    外设采样材料线程壳& operator=(const 外设采样材料线程壳&) = delete;

    外设采样材料线程壳状态 当前状态() const {
        return 状态.load();
    }

    bool 采样材料线程已进入() const {
        return 线程已进入.load();
    }

    bool 线程可收口() const {
        return !采样材料线程.joinable();
    }

    const 有界运行消息队列& 只读外部材料队列() const {
        return 外部材料队列;
    }

    外设采样材料线程壳操作结果 启动() {
        if (状态.load() == 外设采样材料线程壳状态::运行中) {
            return 拒绝(外设采样材料线程壳拒绝原因::重复启动);
        }
        if (外部材料队列.容量() == 0) {
            状态.store(外设采样材料线程壳状态::启动失败);
            return 拒绝(外设采样材料线程壳拒绝原因::队列容量为零);
        }

        请求停止.store(false);
        线程已进入.store(false);
        状态.store(外设采样材料线程壳状态::运行中);
        采样材料线程 = std::thread([this]() {
            线程已进入.store(true);
            while (!请求停止.load()) {
                std::this_thread::yield();
            }
        });
        return {true, false, false, false, false, false, false, false, 外设采样材料线程壳拒绝原因::无};
    }

    外设采样材料线程壳操作结果 发送外部材料消息(const 运行消息& 消息) {
        if (状态.load() != 外设采样材料线程壳状态::运行中) {
            return 拒绝(外设采样材料线程壳拒绝原因::未启动);
        }
        if (消息.类型 != 运行消息类型::外部材料) {
            return 拒绝(外设采样材料线程壳拒绝原因::非外部材料消息);
        }
        if (消息.来源角色 != 运行线程角色::外设采样材料) {
            return 拒绝(外设采样材料线程壳拒绝原因::非外设采样材料来源);
        }
        const auto 入队 = 外部材料队列.入队(消息);
        if (!入队.已入队) {
            return 拒绝(外设采样材料线程壳拒绝原因::材料入队失败);
        }
        return {true, false, false, false, false, false, false, false, 外设采样材料线程壳拒绝原因::无};
    }

    外设采样材料线程壳消费结果 消费外部材料一次() {
        if (状态.load() != 外设采样材料线程壳状态::运行中) {
            return 消费拒绝(外设采样材料线程壳拒绝原因::未启动);
        }

        const auto 出队 = 外部材料队列.出队();
        if (!出队.有消息) {
            return 消费拒绝(外设采样材料线程壳拒绝原因::无外部材料);
        }
        if (出队.消息.类型 != 运行消息类型::外部材料) {
            return 消费拒绝(外设采样材料线程壳拒绝原因::非外部材料消息);
        }

        return {
            true,
            false,
            false,
            false,
            false,
            false,
            false,
            false,
            false,
            false,
            false,
            true,
            外设采样材料线程壳拒绝原因::无,
            出队.消息
        };
    }

    外设采样材料线程壳消费结果 丢弃过期外部材料一次(std::uint64_t 当前时间戳) {
        if (状态.load() != 外设采样材料线程壳状态::运行中) {
            return 消费拒绝(外设采样材料线程壳拒绝原因::未启动);
        }

        const auto 出队 = 外部材料队列.出队();
        if (!出队.有消息) {
            return 消费拒绝(外设采样材料线程壳拒绝原因::无外部材料);
        }
        if (出队.消息.类型 != 运行消息类型::外部材料) {
            return 消费拒绝(外设采样材料线程壳拒绝原因::非外部材料消息);
        }
        if (出队.消息.发生时间戳 >= 当前时间戳) {
            return {
                false,
                true,
                false,
                false,
                false,
                false,
                false,
                false,
                false,
                false,
                false,
                true,
                外设采样材料线程壳拒绝原因::非过期材料,
                出队.消息
            };
        }

        return {
            true,
            false,
            false,
            false,
            false,
            false,
            false,
            false,
            false,
            false,
            true,
            true,
            外设采样材料线程壳拒绝原因::无,
            出队.消息
        };
    }

    外设采样材料线程壳操作结果 请求停止并发送停止消息(std::uint64_t 停止消息编号, std::uint64_t 停止时间戳) {
        if (状态.load() == 外设采样材料线程壳状态::未启动) {
            return 拒绝(外设采样材料线程壳拒绝原因::未启动);
        }

        运行消息 停止消息;
        停止消息.消息编号 = 停止消息编号;
        停止消息.类型 = 运行消息类型::停止;
        停止消息.来源角色 = 运行线程角色::运行宿主;
        停止消息.目标角色 = 运行线程角色::外设采样材料;
        停止消息.发生时间戳 = 停止时间戳;
        停止消息.优先级 = 运行消息优先级::停止;
        停止消息.幂等键 = 停止消息编号;
        停止消息.材料摘要 = 停止消息编号;
        停止消息.只读材料说明 = true;
        停止消息.承载机器事实 = false;

        const auto 入队 = 外部材料队列.入队(停止消息);
        if (!入队.已入队) {
            return 拒绝(外设采样材料线程壳拒绝原因::停止消息失败);
        }

        状态.store(外设采样材料线程壳状态::正在停止);
        请求停止.store(true);
        return {true, false, false, false, false, false, false, false, 外设采样材料线程壳拒绝原因::无};
    }

    外设采样材料线程壳操作结果 收口等待() {
        if (采样材料线程.joinable()) {
            采样材料线程.join();
        }
        if (状态.load() == 外设采样材料线程壳状态::正在停止 || 状态.load() == 外设采样材料线程壳状态::运行中) {
            状态.store(外设采样材料线程壳状态::已停止);
        }
        return {true, false, false, false, false, false, false, false, 外设采样材料线程壳拒绝原因::无};
    }

private:
    有界运行消息队列 外部材料队列;
    std::atomic<外设采样材料线程壳状态> 状态 = 外设采样材料线程壳状态::未启动;
    std::atomic<bool> 请求停止 = false;
    std::atomic<bool> 线程已进入 = false;
    std::thread 采样材料线程;

    外设采样材料线程壳操作结果 拒绝(外设采样材料线程壳拒绝原因 原因) const {
        return {false, true, false, false, false, false, false, false, 原因};
    }

    外设采样材料线程壳消费结果 消费拒绝(外设采样材料线程壳拒绝原因 原因) const {
        return {false, true, false, false, false, false, false, false, false, false, false, false, 原因, {}};
    }
};

class 缓存事件线程壳 {
public:
    缓存事件线程壳(std::uint64_t 缓存队列容量, std::uint64_t 事件队列容量)
        : 缓存刷新队列(缓存队列容量), 事件日志队列(事件队列容量) {}

    ~缓存事件线程壳() {
        请求停止.store(true);
        if (缓存统计线程.joinable()) {
            缓存统计线程.join();
        }
        if (事件日志线程.joinable()) {
            事件日志线程.join();
        }
    }

    缓存事件线程壳(const 缓存事件线程壳&) = delete;
    缓存事件线程壳& operator=(const 缓存事件线程壳&) = delete;

    缓存事件线程壳状态 当前状态() const {
        return 状态.load();
    }

    bool 缓存统计线程已进入() const {
        return 缓存线程已进入.load();
    }

    bool 事件日志线程已进入() const {
        return 事件线程已进入.load();
    }

    bool 线程可收口() const {
        return !缓存统计线程.joinable() && !事件日志线程.joinable();
    }

    const 有界运行消息队列& 只读缓存刷新队列() const {
        return 缓存刷新队列;
    }

    const 有界运行消息队列& 只读事件日志队列() const {
        return 事件日志队列;
    }

    缓存事件线程壳操作结果 启动() {
        if (状态.load() == 缓存事件线程壳状态::运行中) {
            return 拒绝(缓存事件线程壳拒绝原因::重复启动);
        }
        if (缓存刷新队列.容量() == 0 || 事件日志队列.容量() == 0) {
            状态.store(缓存事件线程壳状态::启动失败);
            return 拒绝(缓存事件线程壳拒绝原因::队列容量为零);
        }

        请求停止.store(false);
        缓存线程已进入.store(false);
        事件线程已进入.store(false);
        状态.store(缓存事件线程壳状态::运行中);
        缓存统计线程 = std::thread([this]() {
            缓存线程已进入.store(true);
            while (!请求停止.load()) {
                std::this_thread::yield();
            }
        });
        事件日志线程 = std::thread([this]() {
            事件线程已进入.store(true);
            while (!请求停止.load()) {
                std::this_thread::yield();
            }
        });
        return {true, false, false, 缓存事件线程壳拒绝原因::无};
    }

    缓存事件线程壳操作结果 发送缓存刷新消息(const 运行消息& 消息) {
        if (状态.load() != 缓存事件线程壳状态::运行中) {
            return 拒绝(缓存事件线程壳拒绝原因::未启动);
        }
        if (消息.类型 != 运行消息类型::缓存刷新) {
            return 拒绝(缓存事件线程壳拒绝原因::非缓存刷新消息);
        }
        const auto 入队 = 缓存刷新队列.入队(消息);
        if (!入队.已入队) {
            return 拒绝(缓存事件线程壳拒绝原因::缓存消息入队失败);
        }
        return {true, false, false, 缓存事件线程壳拒绝原因::无};
    }

    缓存事件线程壳操作结果 发送事件日志消息(const 运行消息& 消息) {
        if (状态.load() != 缓存事件线程壳状态::运行中) {
            return 拒绝(缓存事件线程壳拒绝原因::未启动);
        }
        if (消息.类型 != 运行消息类型::事件日志) {
            return 拒绝(缓存事件线程壳拒绝原因::非事件日志消息);
        }
        const auto 入队 = 事件日志队列.入队(消息);
        if (!入队.已入队) {
            return 拒绝(缓存事件线程壳拒绝原因::事件消息入队失败);
        }
        return {true, false, false, 缓存事件线程壳拒绝原因::无};
    }

    缓存事件线程壳消费结果 缓存统计线程消费一次() {
        if (状态.load() != 缓存事件线程壳状态::运行中) {
            return 消费拒绝(缓存事件线程壳拒绝原因::未启动);
        }

        const auto 出队 = 缓存刷新队列.出队();
        if (!出队.有消息) {
            return 消费拒绝(缓存事件线程壳拒绝原因::无缓存刷新材料);
        }
        if (出队.消息.类型 != 运行消息类型::缓存刷新) {
            return 消费拒绝(缓存事件线程壳拒绝原因::非缓存刷新消息);
        }

        return {true, false, false, false, false, false, false, true, 缓存事件线程壳拒绝原因::无, 出队.消息};
    }

    缓存事件线程壳消费结果 事件日志线程消费一次(bool 模拟写入失败) {
        if (状态.load() != 缓存事件线程壳状态::运行中) {
            return 消费拒绝(缓存事件线程壳拒绝原因::未启动);
        }

        const auto 出队 = 事件日志队列.出队();
        if (!出队.有消息) {
            return 消费拒绝(缓存事件线程壳拒绝原因::无事件日志材料);
        }
        if (出队.消息.类型 != 运行消息类型::事件日志) {
            return 消费拒绝(缓存事件线程壳拒绝原因::非事件日志消息);
        }

        return {
            !模拟写入失败,
            模拟写入失败,
            false,
            false,
            false,
            false,
            false,
            true,
            缓存事件线程壳拒绝原因::无,
            出队.消息
        };
    }

    缓存事件线程壳操作结果 请求停止并发送停止消息(std::uint64_t 停止消息编号, std::uint64_t 停止时间戳) {
        if (状态.load() == 缓存事件线程壳状态::未启动) {
            return 拒绝(缓存事件线程壳拒绝原因::未启动);
        }

        运行消息 停止消息;
        停止消息.消息编号 = 停止消息编号;
        停止消息.类型 = 运行消息类型::停止;
        停止消息.来源角色 = 运行线程角色::运行宿主;
        停止消息.目标角色 = 运行线程角色::缓存统计;
        停止消息.发生时间戳 = 停止时间戳;
        停止消息.优先级 = 运行消息优先级::停止;
        停止消息.幂等键 = 停止消息编号;
        停止消息.材料摘要 = 停止消息编号;
        停止消息.只读材料说明 = true;
        停止消息.承载机器事实 = false;

        const auto 缓存停止 = 缓存刷新队列.入队(停止消息);
        停止消息.消息编号 = 停止消息编号 + 1;
        停止消息.目标角色 = 运行线程角色::事件日志;
        停止消息.幂等键 = 停止消息.消息编号;
        停止消息.材料摘要 = 停止消息.消息编号;
        const auto 事件停止 = 事件日志队列.入队(停止消息);
        if (!缓存停止.已入队 || !事件停止.已入队) {
            return 拒绝(缓存事件线程壳拒绝原因::停止消息失败);
        }

        状态.store(缓存事件线程壳状态::正在停止);
        请求停止.store(true);
        return {true, false, false, 缓存事件线程壳拒绝原因::无};
    }

    缓存事件线程壳操作结果 收口等待() {
        if (缓存统计线程.joinable()) {
            缓存统计线程.join();
        }
        if (事件日志线程.joinable()) {
            事件日志线程.join();
        }
        if (状态.load() == 缓存事件线程壳状态::正在停止 || 状态.load() == 缓存事件线程壳状态::运行中) {
            状态.store(缓存事件线程壳状态::已停止);
        }
        return {true, false, false, 缓存事件线程壳拒绝原因::无};
    }

private:
    有界运行消息队列 缓存刷新队列;
    有界运行消息队列 事件日志队列;
    std::atomic<缓存事件线程壳状态> 状态 = 缓存事件线程壳状态::未启动;
    std::atomic<bool> 请求停止 = false;
    std::atomic<bool> 缓存线程已进入 = false;
    std::atomic<bool> 事件线程已进入 = false;
    std::thread 缓存统计线程;
    std::thread 事件日志线程;

    缓存事件线程壳操作结果 拒绝(缓存事件线程壳拒绝原因 原因) const {
        return {false, true, false, 原因};
    }

    缓存事件线程壳消费结果 消费拒绝(缓存事件线程壳拒绝原因 原因) const {
        return {false, true, false, false, false, false, false, false, 原因, {}};
    }
};

class 任务线程壳 {
public:
    任务线程壳(std::uint64_t 调度队列容量, std::uint64_t 回执队列容量)
        : 任务调度队列(调度队列容量), 任务回执队列(回执队列容量) {}

    ~任务线程壳() {
        请求停止.store(true);
        if (任务管理线程.joinable()) {
            任务管理线程.join();
        }
        if (任务工作线程.joinable()) {
            任务工作线程.join();
        }
    }

    任务线程壳(const 任务线程壳&) = delete;
    任务线程壳& operator=(const 任务线程壳&) = delete;

    任务线程壳状态 当前状态() const {
        return 状态.load();
    }

    bool 任务管理线程已进入() const {
        return 管理线程已进入.load();
    }

    bool 任务工作线程已进入() const {
        return 工作线程已进入.load();
    }

    bool 线程可收口() const {
        return !任务管理线程.joinable() && !任务工作线程.joinable();
    }

    const 有界运行消息队列& 只读任务调度队列() const {
        return 任务调度队列;
    }

    const 有界运行消息队列& 只读任务回执队列() const {
        return 任务回执队列;
    }

    任务线程壳操作结果 启动() {
        if (状态.load() == 任务线程壳状态::运行中) {
            return 拒绝(任务线程壳拒绝原因::重复启动);
        }
        if (任务调度队列.容量() == 0 || 任务回执队列.容量() == 0) {
            状态.store(任务线程壳状态::启动失败);
            return 拒绝(任务线程壳拒绝原因::队列容量为零);
        }

        请求停止.store(false);
        管理线程已进入.store(false);
        工作线程已进入.store(false);
        状态.store(任务线程壳状态::运行中);
        任务管理线程 = std::thread([this]() {
            管理线程已进入.store(true);
            while (!请求停止.load()) {
                std::this_thread::yield();
            }
        });
        任务工作线程 = std::thread([this]() {
            工作线程已进入.store(true);
            while (!请求停止.load()) {
                std::this_thread::yield();
            }
        });
        return {true, false, false, 任务线程壳拒绝原因::无};
    }

    任务线程壳操作结果 发送任务执行请求(const 运行消息& 请求消息) {
        if (状态.load() != 任务线程壳状态::运行中) {
            return 拒绝(任务线程壳拒绝原因::未启动);
        }
        if (请求消息.类型 != 运行消息类型::任务执行请求) {
            return 拒绝(任务线程壳拒绝原因::非任务执行请求);
        }
        const auto 入队 = 任务调度队列.入队(请求消息);
        if (!入队.已入队) {
            return 拒绝(任务线程壳拒绝原因::请求入队失败);
        }
        return {true, false, false, 任务线程壳拒绝原因::无};
    }

    任务线程壳消费结果 任务工作线程消费一次并生成回执(std::uint64_t 回执消息编号, std::uint64_t 回执时间戳) {
        if (状态.load() != 任务线程壳状态::运行中) {
            return 消费拒绝(任务线程壳拒绝原因::未启动);
        }

        const auto 出队 = 任务调度队列.出队();
        if (!出队.有消息) {
            return 消费拒绝(任务线程壳拒绝原因::无请求材料);
        }
        if (出队.消息.类型 != 运行消息类型::任务执行请求) {
            return 消费拒绝(任务线程壳拒绝原因::非任务执行请求);
        }

        运行消息 回执消息;
        回执消息.消息编号 = 回执消息编号;
        回执消息.类型 = 运行消息类型::结果回执;
        回执消息.来源角色 = 运行线程角色::任务工作;
        回执消息.目标角色 = 运行线程角色::任务管理;
        回执消息.目标任务 = 出队.消息.目标任务;
        回执消息.目标材料 = 出队.消息.目标材料;
        回执消息.句柄版本 = 出队.消息.句柄版本;
        回执消息.当前句柄版本 = 出队.消息.当前句柄版本;
        回执消息.发生时间戳 = 回执时间戳;
        回执消息.优先级 = 运行消息优先级::高;
        回执消息.幂等键 = 回执消息编号;
        回执消息.批次号 = 出队.消息.批次号;
        回执消息.材料摘要 = 回执消息编号 + 出队.消息.材料摘要;
        回执消息.任务编号 = 出队.消息.任务编号;
        回执消息.任务序号 = 出队.消息.任务序号;
        回执消息.只读材料说明 = true;
        回执消息.承载机器事实 = false;

        const auto 入队 = 任务回执队列.入队(回执消息);
        if (!入队.已入队) {
            return 消费拒绝(任务线程壳拒绝原因::回执入队失败);
        }

        return {
            true,
            false,
            false,
            false,
            false,
            false,
            false,
            true,
            任务线程壳拒绝原因::无,
            出队.消息,
            回执消息
        };
    }

    任务线程壳操作结果 请求停止并发送停止消息(std::uint64_t 停止消息编号, std::uint64_t 停止时间戳) {
        if (状态.load() == 任务线程壳状态::未启动) {
            return 拒绝(任务线程壳拒绝原因::未启动);
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

        const auto 调度停止 = 任务调度队列.入队(停止消息);
        停止消息.消息编号 = 停止消息编号 + 1;
        停止消息.目标角色 = 运行线程角色::任务工作;
        停止消息.幂等键 = 停止消息.消息编号;
        停止消息.材料摘要 = 停止消息.消息编号;
        const auto 回执停止 = 任务回执队列.入队(停止消息);
        if (!调度停止.已入队 || !回执停止.已入队) {
            return 拒绝(任务线程壳拒绝原因::停止消息失败);
        }

        状态.store(任务线程壳状态::正在停止);
        请求停止.store(true);
        return {true, false, false, 任务线程壳拒绝原因::无};
    }

    任务线程壳操作结果 收口等待() {
        if (任务管理线程.joinable()) {
            任务管理线程.join();
        }
        if (任务工作线程.joinable()) {
            任务工作线程.join();
        }
        if (状态.load() == 任务线程壳状态::正在停止 || 状态.load() == 任务线程壳状态::运行中) {
            状态.store(任务线程壳状态::已停止);
        }
        return {true, false, false, 任务线程壳拒绝原因::无};
    }

private:
    有界运行消息队列 任务调度队列;
    有界运行消息队列 任务回执队列;
    std::atomic<任务线程壳状态> 状态 = 任务线程壳状态::未启动;
    std::atomic<bool> 请求停止 = false;
    std::atomic<bool> 管理线程已进入 = false;
    std::atomic<bool> 工作线程已进入 = false;
    std::thread 任务管理线程;
    std::thread 任务工作线程;

    任务线程壳操作结果 拒绝(任务线程壳拒绝原因 原因) const {
        return {false, true, false, 原因};
    }

    任务线程壳消费结果 消费拒绝(任务线程壳拒绝原因 原因) const {
        return {false, true, false, false, false, false, false, true, 原因, {}, {}};
    }
};

}
