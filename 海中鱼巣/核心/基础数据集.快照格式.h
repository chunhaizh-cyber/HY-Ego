#pragma once

#include "基础数据集.h"
#include "基础数据集.持久存储.h"

#include <array>
#include <span>

namespace 海中鱼巣 {

struct 基础快照映像 final {
    std::array<std::uint8_t, 16> 数据集身份{};
    std::uint64_t 下一个编码 = 0;
    std::vector<基础节点> 节点组;
    std::vector<基础外部关系> 外部关系组;
    std::vector<基础字段关系> 字段关系组;
    std::vector<std::uint8_t> 技术材料;
    friend bool operator==(const 基础快照映像&, const 基础快照映像&) = default;
};

std::pair<基础持久状态, std::vector<std::uint8_t>> 编码基础快照(const 基础快照映像& 映像);
std::pair<基础持久状态, std::optional<基础快照映像>> 解码基础快照(std::span<const std::uint8_t> 字节);

} // namespace 海中鱼巣
