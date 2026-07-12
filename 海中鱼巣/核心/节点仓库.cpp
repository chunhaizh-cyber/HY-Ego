// 文件规则：节点删除只让旧句柄失效，不在此直接改写业务事实。
#include "节点仓库.h"

#include <stdexcept>

namespace 海中鱼巣 {

namespace {

bool 节点类型已定义(节点类型 类型) {
    return 类型 >= 节点类型::未分类 && 类型 <= 节点类型::二次特征;
}

bool 接线一致(const 结构事务接线& 左, const 结构事务接线& 右) {
    if (!左.已接域() && !右.已接域()) return true;
    return 左.已接域() && 右.已接域()
        && 左.域编号 == 右.域编号 && 左.运行期纪元 == 右.运行期纪元
        && 左.运行期状态 == 右.运行期状态;
}

bool 验证共享令牌(const 结构事务接线& 接线, const 结构事务令牌& 令牌) {
    return 接线.已接域() && 接线.验证共享路径令牌(接线.运行期状态, 令牌);
}

bool 验证独占令牌(const 结构事务接线& 接线, const 结构事务令牌& 令牌) {
    return 接线.已接域() && 接线.验证独占令牌(接线.运行期状态, 令牌);
}

}

节点仓库::节点仓库(const 主信息仓库& 主信息, std::uint64_t 仓库编号, 结构事务接线 接线)
    : 主信息_(主信息), 仓库编号_(仓库编号 == 0 ? 1 : 仓库编号), 事务接线_(std::move(接线)) {
    if (!接线一致(事务接线_, 主信息_.事务接线_)) {
        throw std::invalid_argument("节点仓库与主信息仓库必须同时未接域或使用同一结构事务域");
    }
}

节点句柄 节点仓库::创建节点(节点类型 类型, 主信息句柄 主信息) {
    if (事务接线_.已接域()) {
        auto 许可 = 事务接线_.取得共享许可(事务接线_.运行期状态);
        return 许可.有效() ? 创建节点(类型, 主信息, 许可.读取令牌()) : 节点句柄{};
    }
    if (!主信息_.主信息是否有效(主信息) || !节点类型已定义(类型)) {
        return {};
    }
    std::unique_lock<std::shared_mutex> 锁(仓库锁_);
    const std::uint64_t 编号 = 下个节点编号_++;
    节点记录 记录;
    记录.节点编号 = 编号;
    记录.类型 = 类型;
    记录.主信息 = 主信息;
    记录.版本号 = 1;
    记录.状态 = 记录状态::有效;
    记录.创建序号 = 下个创建序号_++;
    节点表_.emplace(编号, 记录);
    return 节点句柄{仓库编号_, 编号, 记录.版本号};
}

节点句柄 节点仓库::创建节点(节点类型 类型, 主信息句柄 主信息, const 结构事务令牌& 令牌) {
    if (!验证共享令牌(事务接线_, 令牌) || !主信息_.主信息是否有效(主信息, 令牌) || !节点类型已定义(类型)) return {};
    std::unique_lock<std::shared_mutex> 锁(仓库锁_);
    const std::uint64_t 编号 = 下个节点编号_++;
    节点记录 记录{编号, 类型, 主信息, 1, 记录状态::有效, 下个创建序号_++};
    节点表_.emplace(编号, 记录);
    return {仓库编号_, 编号, 记录.版本号};
}

std::optional<节点记录> 节点仓库::读取节点(节点句柄 节点) const {
    if (事务接线_.已接域()) {
        auto 许可 = 事务接线_.取得共享许可(事务接线_.运行期状态);
        return 许可.有效() ? 读取节点(节点, 许可.读取令牌()) : std::nullopt;
    }
    std::shared_lock<std::shared_mutex> 锁(仓库锁_);
    const auto 位置 = 节点表_.find(节点.节点编号);
    if (位置 == 节点表_.end()) {
        return std::nullopt;
    }
    const 节点记录& 记录 = 位置->second;
    if (节点.仓库编号 != 仓库编号_ || 节点.版本号 != 记录.版本号 || 记录.状态 != 记录状态::有效) {
        return std::nullopt;
    }
    return 记录;
}

std::optional<节点记录> 节点仓库::读取节点(节点句柄 节点, const 结构事务令牌& 令牌) const {
    if (!验证共享令牌(事务接线_, 令牌)) return std::nullopt;
    std::shared_lock<std::shared_mutex> 锁(仓库锁_);
    const auto 位置 = 节点表_.find(节点.节点编号);
    if (位置 == 节点表_.end() || 节点.仓库编号 != 仓库编号_ || 节点.版本号 != 位置->second.版本号 || 位置->second.状态 != 记录状态::有效) return std::nullopt;
    return 位置->second;
}

bool 节点仓库::删除节点(节点句柄 节点) {
    if (事务接线_.已接域()) {
        auto 许可 = 事务接线_.取得独占许可(事务接线_.运行期状态);
        return 许可.有效() && 删除节点(节点, 许可.读取令牌());
    }
    std::unique_lock<std::shared_mutex> 锁(仓库锁_);
    auto 位置 = 节点表_.find(节点.节点编号);
    if (位置 == 节点表_.end()) {
        return false;
    }
    节点记录& 记录 = 位置->second;
    if (节点.仓库编号 != 仓库编号_ || 节点.版本号 != 记录.版本号 || 记录.状态 != 记录状态::有效) {
        return false;
    }
    记录.状态 = 记录状态::已删除;
    ++记录.版本号;
    return true;
}

bool 节点仓库::删除节点(节点句柄 节点, const 结构事务令牌& 令牌) {
    if (!验证独占令牌(事务接线_, 令牌)) return false;
    std::unique_lock<std::shared_mutex> 锁(仓库锁_);
    auto 位置 = 节点表_.find(节点.节点编号);
    if (位置 == 节点表_.end()) return false;
    节点记录& 记录 = 位置->second;
    if (节点.仓库编号 != 仓库编号_ || 节点.版本号 != 记录.版本号 || 记录.状态 != 记录状态::有效) return false;
    记录.状态 = 记录状态::已删除;
    ++记录.版本号;
    return true;
}

bool 节点仓库::节点是否有效(节点句柄 节点) const {
    if (事务接线_.已接域()) {
        auto 许可 = 事务接线_.取得共享许可(事务接线_.运行期状态);
        return 许可.有效() && 节点是否有效(节点, 许可.读取令牌());
    }
    return 读取节点(节点).has_value();
}

bool 节点仓库::节点是否有效(节点句柄 节点, const 结构事务令牌& 令牌) const {
    return 读取节点(节点, 令牌).has_value();
}

std::uint64_t 节点仓库::有效节点数量() const {
    if (事务接线_.已接域()) {
        auto 许可 = 事务接线_.取得共享许可(事务接线_.运行期状态);
        return 许可.有效() ? 有效节点数量(许可.读取令牌()) : 0;
    }
    std::shared_lock<std::shared_mutex> 锁(仓库锁_);
    std::uint64_t 数量 = 0;
    for (const auto& 条目 : 节点表_) {
        if (条目.second.状态 == 记录状态::有效) {
            ++数量;
        }
    }
    return 数量;
}

std::uint64_t 节点仓库::有效节点数量(const 结构事务令牌& 令牌) const {
    if (!验证共享令牌(事务接线_, 令牌)) return 0;
    std::shared_lock<std::shared_mutex> 锁(仓库锁_);
    std::uint64_t 数量 = 0;
    for (const auto& 条目 : 节点表_) if (条目.second.状态 == 记录状态::有效) ++数量;
    return 数量;
}

std::uint64_t 节点仓库::仓库编号() const {
    std::shared_lock<std::shared_mutex> 锁(仓库锁_);
    return 仓库编号_;
}

}
