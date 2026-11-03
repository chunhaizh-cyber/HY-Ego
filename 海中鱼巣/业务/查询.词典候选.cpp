#include "查询.词典候选.h"

#include <utility>

namespace 海中鱼巣 {

词典查询服务::词典查询服务(std::shared_ptr<const 离线词典数据源> 数据源) noexcept
    : 数据源_(std::move(数据源)) {}

词典精确查询结果 词典查询服务::查询词义(
    const 词典精确查询请求& 请求) const noexcept {
    if (数据源_) return 数据源_->精确查询(请求);
    词典精确查询结果 结果;
    结果.状态 = 词典操作状态::未配置;
    return 结果;
}

词典前缀查询结果 词典查询服务::查询原文词面候选(
    const 词典前缀查询请求& 请求) const noexcept {
    if (数据源_) return 数据源_->查询词面前缀(请求);
    词典前缀查询结果 结果;
    结果.状态 = 词典操作状态::未配置;
    return 结果;
}

} // namespace 海中鱼巣
