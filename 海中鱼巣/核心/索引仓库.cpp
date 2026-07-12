// 文件规则：索引只能作为查找入口，不能替代节点、关系或特征值事实。
#include "索引仓库.h"

#include <stdexcept>
#include <vector>

namespace 海中鱼巣 {

namespace {

bool 接线一致(const 结构事务接线& 左, const 结构事务接线& 右) {
    if (!左.已接域() && !右.已接域()) return true;
    return 左.已接域() && 右.已接域() && 左.域编号 == 右.域编号
        && 左.运行期纪元 == 右.运行期纪元 && 左.运行期状态 == 右.运行期状态;
}

bool 验证共享令牌(const 结构事务接线& 接线, const 结构事务令牌& 令牌) {
    return 接线.已接域() && 接线.验证共享路径令牌(接线.运行期状态, 令牌);
}

}

索引仓库::索引仓库(const 节点仓库& 节点, 结构事务接线 接线)
    : 节点_(节点), 事务接线_(std::move(接线)) {
    if (!接线一致(事务接线_, 节点_.事务接线_)) {
        throw std::invalid_argument("索引仓库与节点仓库必须同时未接域或使用同一结构事务域");
    }
}

bool 索引仓库::绑定主键(std::uint64_t 主键, 节点句柄 节点) {
    if (事务接线_.已接域()) {
        auto 许可 = 事务接线_.取得共享许可(事务接线_.运行期状态);
        return 许可.有效() && 绑定主键(主键, 节点, 许可.读取令牌());
    }
    if (主键 == 0 || !节点_.节点是否有效(节点)) return false;
    std::unique_lock<std::shared_mutex> 锁(仓库锁_);
    const auto 现有 = 主键索引_.find(主键);
    if (现有 != 主键索引_.end()) return 现有->second == 节点;
    主键索引_.emplace(主键, 节点);
    return true;
}

bool 索引仓库::绑定主键(std::uint64_t 主键, 节点句柄 节点, const 结构事务令牌& 令牌) {
    if (!验证共享令牌(事务接线_, 令牌) || 主键 == 0 || !节点_.节点是否有效(节点, 令牌)) return false;
    std::unique_lock<std::shared_mutex> 锁(仓库锁_);
    const auto 现有 = 主键索引_.find(主键);
    if (现有 != 主键索引_.end()) return 现有->second == 节点;
    主键索引_.emplace(主键, 节点);
    return true;
}

std::optional<节点句柄> 索引仓库::按主键查节点(std::uint64_t 主键) const {
    if (事务接线_.已接域()) {
        auto 许可 = 事务接线_.取得共享许可(事务接线_.运行期状态);
        return 许可.有效() ? 按主键查节点(主键, 许可.读取令牌()) : std::nullopt;
    }
    if (主键 == 0) return std::nullopt;
    节点句柄 候选;
    {
        std::shared_lock<std::shared_mutex> 锁(仓库锁_);
        const auto 位置 = 主键索引_.find(主键);
        if (位置 == 主键索引_.end()) return std::nullopt;
        候选 = 位置->second;
    }
    return 节点_.节点是否有效(候选) ? std::optional<节点句柄>{候选} : std::nullopt;
}

std::optional<节点句柄> 索引仓库::按主键查节点(std::uint64_t 主键, const 结构事务令牌& 令牌) const {
    if (!验证共享令牌(事务接线_, 令牌) || 主键 == 0) return std::nullopt;
    节点句柄 候选;
    {
        std::shared_lock<std::shared_mutex> 锁(仓库锁_);
        const auto 位置 = 主键索引_.find(主键);
        if (位置 == 主键索引_.end()) return std::nullopt;
        候选 = 位置->second;
    }
    return 节点_.节点是否有效(候选, 令牌) ? std::optional<节点句柄>{候选} : std::nullopt;
}

bool 索引仓库::删除主键(std::uint64_t 主键) {
    if (事务接线_.已接域()) {
        auto 许可 = 事务接线_.取得共享许可(事务接线_.运行期状态);
        return 许可.有效() && 删除主键(主键, 许可.读取令牌());
    }
    if (主键 == 0) return false;
    std::unique_lock<std::shared_mutex> 锁(仓库锁_);
    return 主键索引_.erase(主键) != 0;
}

bool 索引仓库::删除主键(std::uint64_t 主键, const 结构事务令牌& 令牌) {
    if (!验证共享令牌(事务接线_, 令牌) || 主键 == 0) return false;
    std::unique_lock<std::shared_mutex> 锁(仓库锁_);
    return 主键索引_.erase(主键) != 0;
}

std::uint64_t 索引仓库::有效主键数量() const {
    if (事务接线_.已接域()) {
        auto 许可 = 事务接线_.取得共享许可(事务接线_.运行期状态);
        return 许可.有效() ? 有效主键数量(许可.读取令牌()) : 0;
    }
    std::vector<节点句柄> 候选组;
    {
        std::shared_lock<std::shared_mutex> 锁(仓库锁_);
        候选组.reserve(主键索引_.size());
        for (const auto& 条目 : 主键索引_) 候选组.push_back(条目.second);
    }
    std::uint64_t 数量 = 0;
    for (const auto& 候选 : 候选组) if (节点_.节点是否有效(候选)) ++数量;
    return 数量;
}

std::uint64_t 索引仓库::有效主键数量(const 结构事务令牌& 令牌) const {
    if (!验证共享令牌(事务接线_, 令牌)) return 0;
    std::vector<节点句柄> 候选组;
    {
        std::shared_lock<std::shared_mutex> 锁(仓库锁_);
        候选组.reserve(主键索引_.size());
        for (const auto& 条目 : 主键索引_) 候选组.push_back(条目.second);
    }
    std::uint64_t 数量 = 0;
    for (const auto& 候选 : 候选组) if (节点_.节点是否有效(候选, 令牌)) ++数量;
    return 数量;
}

}
