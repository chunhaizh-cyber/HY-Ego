export module 需求环境模块;

import <mutex>;
import <string>;
import <unordered_map>;
import <vector>;

import 主信息定义模块;
import 需求模块;

export inline 需求类 需求集{};

export class 需求状态管理器类 {
public:
    void 初始化_扫描需求链(const std::string& 调用点 = "需求状态管理器::初始化_扫描需求链") {
        {
            std::lock_guard<std::mutex> lk(锁_);
            状态缓存_.clear();
        }

        for (auto* 根需求 : 需求集.获取全部需求(调用点)) {
            登记需求及子树(根需求, 调用点 + "/登记子树");
        }
    }

    void 登记需求及子树(
        需求节点类* 根需求,
        const std::string& 调用点 = "需求状态管理器::登记需求及子树")
    {
        if (!根需求) return;

        std::vector<需求节点类*> 栈;
        栈.push_back(根需求);
        while (!栈.empty()) {
            auto* n = 栈.back();
            栈.pop_back();
            if (!n || !n->主信息) continue;

            (void)刷新需求状态(n, 调用点 + "/刷新");

            if (n->子) {
                auto* 起 = n->子;
                auto* it = 起;
                do {
                    栈.push_back(it);
                    it = it->下;
                } while (it && it != 起);
            }
        }
    }

    枚举_需求运行状态 刷新需求状态(
        需求节点类* 需求,
        const std::string& 调用点 = "需求状态管理器::刷新需求状态")
    {
        const auto 新状态 = 私有_计算需求状态(需求, 调用点);
        if (!需求) return 新状态;

        std::lock_guard<std::mutex> lk(锁_);
        状态缓存_[需求] = 新状态;
        return 新状态;
    }

    void 刷新需求及父链状态(
        需求节点类* 需求,
        const std::string& 调用点 = "需求状态管理器::刷新需求及父链状态")
    {
        auto* cur = 需求;
        while (cur) {
            (void)刷新需求状态(cur, 调用点);
            cur = static_cast<需求节点类*>(cur->父);
        }
    }

    void 标记需求状态(
        需求节点类* 需求,
        枚举_需求运行状态 状态,
        const std::string& 调用点 = "需求状态管理器::标记需求状态")
    {
        (void)调用点;
        if (!需求) return;

        std::lock_guard<std::mutex> lk(锁_);
        状态缓存_[需求] = 状态;
    }

    void 处理任务返回消息(
        const 结构_任务返回消息& 消息,
        const std::string& 调用点 = "需求状态管理器::处理任务返回消息")
    {
        auto 刷新任务头需求 = [&](任务节点类* 任务, const std::string& 标签) {
            auto* 头信息 = (任务 && 任务->主信息) ? dynamic_cast<任务头结点信息*>(任务->主信息) : nullptr;
            if (头信息 && 头信息->需求) {
                刷新需求及父链状态(头信息->需求, 调用点 + "/" + 标签);
            }
        };

        if (消息.对应需求) {
            刷新需求及父链状态(消息.对应需求, 调用点 + "/对应需求");
        }
        刷新任务头需求(消息.来源任务, "来源任务");
        刷新任务头需求(消息.相关子任务, "相关子任务");
    }

    枚举_需求运行状态 查询需求状态(需求节点类* 需求) const {
        if (!需求) return 枚举_需求运行状态::未知;
        {
            std::lock_guard<std::mutex> lk(锁_);
            if (auto it = 状态缓存_.find(需求); it != 状态缓存_.end()) {
                return it->second;
            }
        }
        return const_cast<需求状态管理器类*>(this)->刷新需求状态(
            需求,
            "需求状态管理器::查询需求状态/即时刷新");
    }

private:
    枚举_需求运行状态 私有_计算需求状态(
        需求节点类* 需求,
        const std::string& 调用点) const
    {
        if (!需求 || !需求->主信息) return 枚举_需求运行状态::未知;
        const auto 判定 = 需求集.判断需求状态(需求, 调用点);
        return 判定.已满足 ? 枚举_需求运行状态::已满足 : 枚举_需求运行状态::待执行;
    }

private:
    mutable std::mutex 锁_{};
    std::unordered_map<需求节点类*, 枚举_需求运行状态> 状态缓存_{};
};

export inline 需求状态管理器类 需求状态管理器{};
