// 文件规则：有界运行消息队列只管理消息材料的有界排队和拒绝；不得定义线程类、承载机器事实或写业务结构。
module;

#include <cstdint>
#include <deque>
#include <unordered_map>

export module 海中鱼巣.线程.有界运行消息队列;

import 海中鱼巣.线程.运行消息协议;

export namespace 海中鱼巣 {

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
