// 文件规则：自我任务后继决议协议只承载不可变定位；任务管理必须独立读回正式决议。
module;

#include <cstdint>

export module 海中鱼巣.线程.协议.自我任务后继决议;

import 海中鱼巣.领域.服务.L2自我治理结构;

export namespace 海中鱼巣 {

inline constexpr std::uint32_t 自我任务后继决议消息版本 = 1;

struct 自我任务后继决议定位消息 final {
    std::uint32_t 消息版本 = 自我任务后继决议消息版本;
    稳定编码 决议身份;
    L2任务身份 任务;
    L2任务轮次身份 已收束任务轮次;
    std::uint64_t 路由序号 = 0;
    friend bool operator==(const 自我任务后继决议定位消息&,
        const 自我任务后继决议定位消息&) = default;
};

inline bool 自我任务后继决议定位消息完整(
    const 自我任务后继决议定位消息& 消息) noexcept {
    return 消息.消息版本 == 自我任务后继决议消息版本
        && 有效(消息.决议身份) && 有效(消息.任务.值)
        && 有效(消息.已收束任务轮次.值) && 消息.路由序号 != 0;
}

} // namespace 海中鱼巣
