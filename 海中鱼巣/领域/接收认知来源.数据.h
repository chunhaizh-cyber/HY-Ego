#pragma once

#include "../核心/基础数据集.h"
#include <cstdint>
#include <vector>

namespace 海中鱼巣 {

struct 新接收认知来源键 final {
    稳定编码 接收消息节点;
    std::uint64_t 原文起点=0, 原文终点=0;
    std::vector<std::int64_t> 根出现路径;
    friend bool operator==(const 新接收认知来源键&, const 新接收认知来源键&) = default;
};
struct 新接收认知来源 final {
    新接收认知来源键 键;
    稳定编码 采用含义节点;
    friend bool operator==(const 新接收认知来源&, const 新接收认知来源&) = default;
};

struct 新行动请求承接来源 final {
    新接收认知来源 需求来源;
    稳定编码 被认知需求节点, 来源人类节点, 受请求者节点;
    friend bool operator==(const 新行动请求承接来源&, const 新行动请求承接来源&) = default;
};

} // namespace 海中鱼巣
