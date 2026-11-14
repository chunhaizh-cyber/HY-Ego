#pragma once

#include <cstdint>

#include "../核心/基础数据集.h"

namespace 海中鱼巣 {

// 本轮组合发布的引用增减材料；计数仍由被引用节点的所有者维护。
struct 新引用计数变化 final {
    稳定编码 节点;
    std::int64_t 增量 = 0;
    friend bool operator==(const 新引用计数变化&, const 新引用计数变化&) = default;
};

} // namespace 海中鱼巣
