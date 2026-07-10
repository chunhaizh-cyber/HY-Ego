// 文件规则：缓存统计线程只消费非权威缓存刷新材料；不得裁决业务事实、写权威结构或定义其他线程角色。
module;

#include <atomic>
#include <cstdint>
#include <thread>

export module 海中鱼巣.线程.缓存统计线程;

import 海中鱼巣.线程.运行消息协议;
import 海中鱼巣.线程.有界运行消息队列;

export namespace 海中鱼巣 {

enum class 缓存统计线程状态 : std::uint32_t {
    未启动 = 0,
    运行中 = 1,
    正在停止 = 2,
    已停止 = 3,
    启动失败 = 4
};

enum class 缓存统计线程拒绝原因 : std::uint32_t {
    无 = 0,
    重复启动 = 1,
    队列容量为零 = 2,
    未启动 = 3,
    停止消息失败 = 4,
    非缓存刷新消息 = 5,
    缓存消息入队失败 = 6,
    无缓存刷新材料 = 7
};

struct 缓存统计线程操作结果 {
    bool 成功 = false;
    bool 局部失败 = true;
    bool 写业务事实 = false;
    缓存统计线程拒绝原因 拒绝原因 = 缓存统计线程拒绝原因::无;
};

struct 缓存统计线程消费结果 {
    bool 成功 = false;
    bool 局部失败 = true;
    bool 写业务事实 = false;
    bool 裸写仓库 = false;
    bool 裁决业务事实 = false;
    bool 修复结构 = false;
    bool 恢复运行期结构 = false;
    bool 只产生审计或缓存材料 = false;
    缓存统计线程拒绝原因 拒绝原因 = 缓存统计线程拒绝原因::无;
    运行消息 消费消息;
};

class 缓存统计线程 {
public:
    explicit 缓存统计线程(std::uint64_t 队列容量)
        : 缓存刷新队列(队列容量) {}

    ~缓存统计线程() {
        请求停止.store(true);
        if (线程.joinable()) {
            线程.join();
        }
    }

    缓存统计线程(const 缓存统计线程&) = delete;
    缓存统计线程& operator=(const 缓存统计线程&) = delete;

    缓存统计线程状态 当前状态() const {
        return 状态.load();
    }

    bool 线程已进入() const {
        return 已进入.load();
    }

    bool 线程可收口() const {
        return !线程.joinable();
    }

    const 有界运行消息队列& 只读缓存刷新队列() const {
        return 缓存刷新队列;
    }

    缓存统计线程操作结果 启动() {
        if (状态.load() == 缓存统计线程状态::运行中) {
            return 拒绝(缓存统计线程拒绝原因::重复启动);
        }
        if (缓存刷新队列.容量() == 0) {
            状态.store(缓存统计线程状态::启动失败);
            return 拒绝(缓存统计线程拒绝原因::队列容量为零);
        }

        请求停止.store(false);
        已进入.store(false);
        状态.store(缓存统计线程状态::运行中);
        线程 = std::thread([this]() {
            已进入.store(true);
            while (!请求停止.load()) {
                std::this_thread::yield();
            }
        });
        return {true, false, false, 缓存统计线程拒绝原因::无};
    }

    缓存统计线程操作结果 发送缓存刷新消息(const 运行消息& 消息) {
        if (状态.load() != 缓存统计线程状态::运行中) {
            return 拒绝(缓存统计线程拒绝原因::未启动);
        }
        if (消息.类型 != 运行消息类型::缓存刷新) {
            return 拒绝(缓存统计线程拒绝原因::非缓存刷新消息);
        }
        const auto 入队 = 缓存刷新队列.入队(消息);
        if (!入队.已入队) {
            return 拒绝(缓存统计线程拒绝原因::缓存消息入队失败);
        }
        return {true, false, false, 缓存统计线程拒绝原因::无};
    }

    缓存统计线程消费结果 消费一次() {
        if (状态.load() != 缓存统计线程状态::运行中) {
            return 消费拒绝(缓存统计线程拒绝原因::未启动);
        }

        const auto 出队 = 缓存刷新队列.出队();
        if (!出队.有消息) {
            return 消费拒绝(缓存统计线程拒绝原因::无缓存刷新材料);
        }
        if (出队.消息.类型 != 运行消息类型::缓存刷新) {
            return 消费拒绝(缓存统计线程拒绝原因::非缓存刷新消息);
        }

        return {true, false, false, false, false, false, false, true,
            缓存统计线程拒绝原因::无, 出队.消息};
    }

    缓存统计线程操作结果 请求停止并发送停止消息(
        std::uint64_t 停止消息编号, std::uint64_t 停止时间戳) {
        if (状态.load() == 缓存统计线程状态::未启动) {
            return 拒绝(缓存统计线程拒绝原因::未启动);
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

        const auto 入队 = 缓存刷新队列.入队(停止消息);
        if (!入队.已入队) {
            return 拒绝(缓存统计线程拒绝原因::停止消息失败);
        }

        状态.store(缓存统计线程状态::正在停止);
        请求停止.store(true);
        return {true, false, false, 缓存统计线程拒绝原因::无};
    }

    缓存统计线程操作结果 收口等待() {
        if (线程.joinable()) {
            线程.join();
        }
        if (状态.load() == 缓存统计线程状态::正在停止
            || 状态.load() == 缓存统计线程状态::运行中) {
            状态.store(缓存统计线程状态::已停止);
        }
        return {true, false, false, 缓存统计线程拒绝原因::无};
    }

private:
    有界运行消息队列 缓存刷新队列;
    std::atomic<缓存统计线程状态> 状态 = 缓存统计线程状态::未启动;
    std::atomic<bool> 请求停止 = false;
    std::atomic<bool> 已进入 = false;
    std::thread 线程;

    缓存统计线程操作结果 拒绝(缓存统计线程拒绝原因 原因) const {
        return {false, true, false, 原因};
    }

    缓存统计线程消费结果 消费拒绝(缓存统计线程拒绝原因 原因) const {
        return {false, true, false, false, false, false, false, false, 原因, {}};
    }
};

}
