#pragma once

namespace 海中鱼巣 {

inline constexpr std::uint32_t 系统世界树根初始化合同版本 = 1;
inline constexpr L2结构幂等身份 海中鱼巣现实世界树根建立幂等身份{
    0x5359'5357'5452'0001ULL};

struct 系统世界树根初始化请求 final {
    std::uint32_t 合同版本 = 系统世界树根初始化合同版本;
    L2结构幂等身份 根建立幂等身份 = 海中鱼巣现实世界树根建立幂等身份;
};

enum class 系统世界树根初始化状态 : std::uint8_t {
    已发布 = 1,
    精确重复 = 2,
    请求无效 = 3,
    已发布选择冲突 = 4,
    树结构登记读取失败 = 5,
    根建立失败 = 6,
    根正式读回失败 = 7,
    根读回不一致 = 8,
    资源失败 = 9,
    内部不一致 = 10
};

enum class 系统世界树根发布来源 : std::uint8_t {
    首次建立 = 1,
    待收敛精确重复 = 2,
    已发布重复消费 = 3
};

struct 系统世界树根读回 final {
    系统世界树根初始化请求 选择;
    L2场景树根建立请求 首次建立请求;
    L2场景树身份 树;
    L2场景身份 根场景;
    L2场景树事实 正式树读回;
    std::uint64_t 事实截止代次 = 0;
};

struct 系统世界树根初始化结果 final {
    系统世界树根初始化状态 状态 = 系统世界树根初始化状态::请求无效;
    std::optional<系统世界树根发布来源> 来源;
    std::optional<L2场景树结构登记结果> 根建立前登记读取;
    std::optional<L2场景树节点写入结果> 根建立;
    std::optional<L2场景树结构登记结果> 根读回前登记读取;
    std::optional<L2场景树读取结果> 根读取;
    std::optional<系统世界树根读回> 已发布根;

    bool 成功() const noexcept;
};

}
