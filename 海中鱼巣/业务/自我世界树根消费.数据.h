#pragma once

#include "系统世界树根初始化.数据.h"

namespace 海中鱼巣 {

inline constexpr std::uint32_t 自我世界树根消费合同版本 = 1;

struct 自我世界树根消费请求 final {
    std::uint32_t 合同版本 = 自我世界树根消费合同版本;
    系统世界树根初始化请求 项目树选择;
    L2场景树根建立请求 首次根建立请求;
    L2场景树身份 树;
    L2场景身份 根场景;
    L2场景树归属事实 首次根标记;
};

enum class 自我世界树根消费状态 : std::uint8_t {
    已发布 = 1,
    精确重复 = 2,
    请求无效 = 3,
    已发布选择冲突 = 4,
    树结构登记读取失败 = 5,
    根正式读回失败 = 6,
    根读回不一致 = 7,
    资源失败 = 8,
    内部不一致 = 9
};

enum class 自我世界树根消费来源 : std::uint8_t {
    首次消费 = 1,
    已发布重复消费 = 2
};

struct 自我世界树根消费材料 final {
    自我世界树根消费请求 请求;
    L2场景树结构登记 当前结构登记;
    L2场景树事实 正式树读回;
    std::uint64_t 本次事实截止代次 = 0;
};

struct 自我世界树根消费结果 final {
    自我世界树根消费状态 状态 = 自我世界树根消费状态::请求无效;
    std::optional<自我世界树根消费来源> 来源;
    std::optional<L2场景树结构登记结果> 结构登记读取;
    std::optional<L2场景树读取结果> 根读取;
    std::optional<自我世界树根消费材料> 已发布材料;
    bool 成功() const noexcept;
};

}
