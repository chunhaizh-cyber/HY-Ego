// 文件规则：索引只能作为查找入口，不能替代节点、关系或特征值事实。
#include "索引仓库.h"

namespace 海中鱼巣 {

索引仓库::索引仓库(const 节点仓库& 节点)
    : 节点_(节点) {
}

bool 索引仓库::绑定主键(std::uint64_t 主键, 节点句柄 节点) {
    if (主键 == 0 || !节点_.节点是否有效(节点)) {
        return false;
    }
    std::unique_lock<std::shared_mutex> 锁(仓库锁_);
    const auto 现有 = 主键索引_.find(主键);
    if (现有 != 主键索引_.end()) {
        return 现有->second == 节点;
    }
    主键索引_.emplace(主键, 节点);
    return true;
}

std::optional<节点句柄> 索引仓库::按主键查节点(std::uint64_t 主键) const {
    if (主键 == 0) {
        return std::nullopt;
    }
    std::shared_lock<std::shared_mutex> 锁(仓库锁_);
    const auto 位置 = 主键索引_.find(主键);
    if (位置 == 主键索引_.end() || !节点_.节点是否有效(位置->second)) {
        return std::nullopt;
    }
    return 位置->second;
}

bool 索引仓库::删除主键(std::uint64_t 主键) {
    if (主键 == 0) {
        return false;
    }
    std::unique_lock<std::shared_mutex> 锁(仓库锁_);
    return 主键索引_.erase(主键) != 0;
}

std::uint64_t 索引仓库::有效主键数量() const {
    std::shared_lock<std::shared_mutex> 锁(仓库锁_);
    std::uint64_t 数量 = 0;
    for (const auto& 条目 : 主键索引_) {
        if (节点_.节点是否有效(条目.second)) {
            ++数量;
        }
    }
    return 数量;
}

}
