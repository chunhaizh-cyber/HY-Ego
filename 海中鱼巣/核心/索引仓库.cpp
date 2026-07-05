// 文件规则：索引只能作为查找入口，不能替代节点、关系或特征值事实。
#include "索引仓库.h"

namespace 海中鱼巣 {

bool 索引仓库::绑定主键(std::uint64_t 主键, 节点句柄 节点) {
    if (主键 == 0 || !句柄有效(节点)) {
        return false;
    }
    主键索引_[主键] = 节点;
    return true;
}

std::optional<节点句柄> 索引仓库::按主键查节点(std::uint64_t 主键) const {
    const auto 位置 = 主键索引_.find(主键);
    if (位置 == 主键索引_.end()) {
        return std::nullopt;
    }
    return 位置->second;
}

bool 索引仓库::删除主键(std::uint64_t 主键) {
    return 主键索引_.erase(主键) != 0;
}

}
