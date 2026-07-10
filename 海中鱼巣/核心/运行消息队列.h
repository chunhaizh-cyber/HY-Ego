// 文件规则：运行消息队列只承载请求、材料、回执和调度信号；不得承载机器事实、启动真实线程或写业务结构。
#pragma once

#include <cstdint>
#include <deque>
#include <optional>
#include <unordered_map>

namespace 海中鱼巣 {

enum class 运行线程角色 : std::uint32_t {
    未知 = 0,
    运行宿主 = 1,
    任务管理 = 2,
    任务工作 = 3,
    缓存统计 = 4,
    事件日志 = 5,
    外设采样材料 = 6
};

enum class 运行消息类型 : std::uint32_t {
    未知 = 0,
    调度周期 = 1,
    停止 = 2,
    任务执行请求 = 3,
    任务拒绝回执 = 4,
    结果回执 = 5,
    缓存刷新 = 6,
    事件日志 = 7,
    外部材料 = 8
};

enum class 运行消息优先级 : std::uint32_t {
    低 = 0,
    普通 = 1,
    高 = 2,
    停止 = 3
};

enum class 入队拒绝原因 : std::uint32_t {
    无 = 0,
    容量为零 = 1,
    消息编号无效 = 2,
    队列已满 = 3,
    队列已停止 = 4,
    消息承载事实 = 5,
    幂等键冲突 = 6,
    句柄版本过期 = 7,
    同一任务序号倒退 = 8
};

struct 运行消息 {
    std::uint64_t 消息编号 = 0;
    运行消息类型 类型 = 运行消息类型::未知;
    运行线程角色 来源角色 = 运行线程角色::未知;
    运行线程角色 目标角色 = 运行线程角色::未知;
    std::uint64_t 目标任务 = 0;
    std::uint64_t 目标材料 = 0;
    std::uint64_t 句柄版本 = 0;
    std::uint64_t 当前句柄版本 = 0;
    std::uint64_t 发生时间戳 = 0;
    运行消息优先级 优先级 = 运行消息优先级::普通;
    std::uint64_t 幂等键 = 0;
    std::uint64_t 批次号 = 0;
    std::uint64_t 材料摘要 = 0;
    std::uint64_t 任务编号 = 0;
    std::uint64_t 任务序号 = 0;
    bool 只读材料说明 = true;
    bool 承载机器事实 = false;
};

struct 入队结果 {
    bool 已入队 = false;
    bool 局部失败 = true;
    bool 写业务事实 = false;
    入队拒绝原因 拒绝原因 = 入队拒绝原因::无;
};

struct 出队结果 {
    bool 有消息 = false;
    bool 队列已停止 = false;
    运行消息 消息;
};

class 有界运行消息队列 {
public:
    explicit 有界运行消息队列(std::uint64_t 容量) : 容量上限(容量) {}

    std::uint64_t 容量() const {
        return 容量上限;
    }

    std::uint64_t 数量() const {
        return static_cast<std::uint64_t>(队列.size());
    }

    bool 已停止() const {
        return 停止态;
    }

    入队结果 入队(const 运行消息& 原始消息) {
        if (容量上限 == 0) {
            return 拒绝(入队拒绝原因::容量为零);
        }

        运行消息 消息 = 原始消息;
        if (消息.消息编号 == 0) {
            return 拒绝(入队拒绝原因::消息编号无效);
        }
        if (消息.承载机器事实) {
            return 拒绝(入队拒绝原因::消息承载事实);
        }
        if (消息.当前句柄版本 != 0 && 消息.句柄版本 != 消息.当前句柄版本) {
            return 拒绝(入队拒绝原因::句柄版本过期);
        }
        if (消息.幂等键 != 0) {
            const auto 已有材料 = 幂等材料摘要.find(消息.幂等键);
            if (已有材料 != 幂等材料摘要.end() && 已有材料->second != 消息.材料摘要) {
                return 拒绝(入队拒绝原因::幂等键冲突);
            }
        }
        if (消息.任务编号 != 0 && 消息.任务序号 != 0) {
            const auto 已有序号 = 任务最新序号.find(消息.任务编号);
            if (已有序号 != 任务最新序号.end() && 消息.任务序号 < 已有序号->second) {
                return 拒绝(入队拒绝原因::同一任务序号倒退);
            }
        }

        const bool 是停止消息 = 消息.类型 == 运行消息类型::停止;
        if (停止态 && !是停止消息) {
            return 拒绝(入队拒绝原因::队列已停止);
        }

        if (是停止消息) {
            消息.优先级 = 运行消息优先级::停止;
            停止态 = true;
            while (队列.size() >= 容量上限 && !队列.empty()) {
                队列.pop_back();
            }
        } else if (队列.size() >= 容量上限) {
            return 拒绝(入队拒绝原因::队列已满);
        }

        记录材料索引(消息);
        插入消息(消息);
        return {true, false, false, 入队拒绝原因::无};
    }

    出队结果 出队() {
        if (队列.empty()) {
            return {false, 停止态, {}};
        }

        const auto 消息 = 队列.front();
        队列.pop_front();
        return {true, 停止态, 消息};
    }

private:
    std::uint64_t 容量上限 = 0;
    bool 停止态 = false;
    std::deque<运行消息> 队列;
    std::unordered_map<std::uint64_t, std::uint64_t> 幂等材料摘要;
    std::unordered_map<std::uint64_t, std::uint64_t> 任务最新序号;

    入队结果 拒绝(入队拒绝原因 原因) const {
        return {false, true, false, 原因};
    }

    void 记录材料索引(const 运行消息& 消息) {
        if (消息.幂等键 != 0) {
            幂等材料摘要.emplace(消息.幂等键, 消息.材料摘要);
        }
        if (消息.任务编号 != 0 && 消息.任务序号 != 0) {
            auto& 最新序号 = 任务最新序号[消息.任务编号];
            if (消息.任务序号 > 最新序号) {
                最新序号 = 消息.任务序号;
            }
        }
    }

    void 插入消息(const 运行消息& 消息) {
        const auto 消息优先级值 = static_cast<std::uint32_t>(消息.优先级);
        auto 位置 = 队列.begin();
        while (位置 != 队列.end()
            && static_cast<std::uint32_t>(位置->优先级) >= 消息优先级值) {
            ++位置;
        }
        队列.insert(位置, 消息);
    }
};

}
