// 文件规则：索引仓库只维护主键到节点句柄的查找关系，不裁决机器事实。
#pragma once

#include "句柄.h"
#include "节点仓库.h"

#include <cstdint>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <unordered_map>

namespace 海中鱼巣 {

class 索引仓库 {
public:
    explicit 索引仓库(const 节点仓库& 节点, 结构事务接线 接线 = {});

    bool 绑定主键(std::uint64_t 主键, 节点句柄 节点);
    bool 绑定主键(std::uint64_t 主键, 节点句柄 节点, const 结构事务令牌& 令牌);
    std::optional<节点句柄> 按主键查节点(std::uint64_t 主键) const;
    std::optional<节点句柄> 按主键查节点(std::uint64_t 主键, const 结构事务令牌& 令牌) const;
    bool 删除主键(std::uint64_t 主键);
    bool 删除主键(std::uint64_t 主键, const 结构事务令牌& 令牌);
    std::uint64_t 有效主键数量() const;
    std::uint64_t 有效主键数量(const 结构事务令牌& 令牌) const;

private:
    const 节点仓库& 节点_;
    结构事务接线 事务接线_;
    mutable std::shared_mutex 仓库锁_;
    std::unordered_map<std::uint64_t, 节点句柄> 主键索引_;
};

}
