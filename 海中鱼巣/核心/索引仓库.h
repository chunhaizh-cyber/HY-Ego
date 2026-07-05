// 文件规则：索引仓库只维护主键到节点句柄的查找关系，不裁决机器事实。
#pragma once

#include "句柄.h"

#include <cstdint>
#include <optional>
#include <unordered_map>

namespace 海中鱼巣 {

class 索引仓库 {
public:
    bool 绑定主键(std::uint64_t 主键, 节点句柄 节点);
    std::optional<节点句柄> 按主键查节点(std::uint64_t 主键) const;
    bool 删除主键(std::uint64_t 主键);

private:
    std::unordered_map<std::uint64_t, 节点句柄> 主键索引_;
};

}
