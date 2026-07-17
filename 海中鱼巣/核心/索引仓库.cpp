// 文件规则：索引只能作为查找入口，不能替代节点、关系或特征值事实。
#include "索引仓库.h"

#include <algorithm>
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

bool 验证独占令牌(const 结构事务接线& 接线, const 结构事务令牌& 令牌) {
    return 接线.已接域() && 接线.验证独占令牌(接线.运行期状态, 令牌);
}

bool 绑定主键_已加锁(
    std::unordered_map<std::uint64_t, 节点句柄>& 主键索引,
    std::unordered_map<std::uint64_t, std::vector<std::uint64_t>>& 节点主键组,
    std::uint64_t 主键,
    节点句柄 节点) {
    const auto 现有 = 主键索引.find(主键);
    if (现有 != 主键索引.end()) return 现有->second == 节点;
    auto [反向位置, 新建反向项] = 节点主键组.try_emplace(节点.节点编号);
    try {
        反向位置->second.push_back(主键);
        const auto [位置, 已插入] = 主键索引.emplace(主键, 节点);
        if (!已插入) {
            反向位置->second.pop_back();
            if (新建反向项 && 反向位置->second.empty()) 节点主键组.erase(反向位置);
            return 位置->second == 节点;
        }
    } catch (...) {
        if (!反向位置->second.empty() && 反向位置->second.back() == 主键) 反向位置->second.pop_back();
        if (新建反向项 && 反向位置->second.empty()) 节点主键组.erase(反向位置);
        throw;
    }
    return true;
}

bool 删除主键_已加锁(
    std::unordered_map<std::uint64_t, 节点句柄>& 主键索引,
    std::unordered_map<std::uint64_t, std::vector<std::uint64_t>>& 节点主键组,
    std::uint64_t 主键) {
    const auto 位置 = 主键索引.find(主键);
    if (位置 == 主键索引.end()) return false;
    const auto 节点编号 = 位置->second.节点编号;
    const auto 反向位置 = 节点主键组.find(节点编号);
    if (反向位置 == 节点主键组.end()) {
        throw std::logic_error("主键索引缺少反向节点主键组");
    }
    auto& 主键组 = 反向位置->second;
    const auto 键位置 = std::find(主键组.begin(), 主键组.end(), 主键);
    if (键位置 == 主键组.end()) {
        throw std::logic_error("主键索引与反向节点主键组不对称");
    }
    主键索引.erase(位置);
    主键组.erase(键位置);
    if (主键组.empty()) 节点主键组.erase(反向位置);
    return true;
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
    return 绑定主键_已加锁(主键索引_, 节点主键组_, 主键, 节点);
}

bool 索引仓库::绑定主键(std::uint64_t 主键, 节点句柄 节点, const 结构事务令牌& 令牌) {
    if (!验证共享令牌(事务接线_, 令牌) || 主键 == 0 || !节点_.节点是否有效(节点, 令牌)) return false;
    std::unique_lock<std::shared_mutex> 锁(仓库锁_);
    return 绑定主键_已加锁(主键索引_, 节点主键组_, 主键, 节点);
}

结构写入结果 索引仓库::结构化绑定主键(
    std::uint64_t 主键,
    节点句柄 节点,
    const 结构事务令牌& 令牌) {
    结构写入结果 结果{结构写入状态::入口拒绝, 主键, 节点.版本号, 0};
    if (!验证独占令牌(事务接线_, 令牌)) {
        结果.状态 = 结构写入状态::许可拒绝;
        return 结果;
    }
    if (主键 == 0 || !句柄有效(节点) || !节点_.节点是否有效(节点, 令牌)) {
        return 结果;
    }
    std::unique_lock<std::shared_mutex> 锁(仓库锁_);
    const auto 现有 = 主键索引_.find(主键);
    if (现有 != 主键索引_.end()) {
        结果.当前版本 = 现有->second.版本号;
        if (现有->second != 节点) {
            const bool 同一身份 = 现有->second.仓库编号 == 节点.仓库编号
                && 现有->second.节点编号 == 节点.节点编号;
            结果.状态 = 同一身份 ? 结构写入状态::版本漂移 : 结构写入状态::入口拒绝;
            return 结果;
        }
        const auto 反向位置 = 节点主键组_.find(节点.节点编号);
        if (反向位置 == 节点主键组_.end()
            || std::find(反向位置->second.begin(), 反向位置->second.end(), 主键)
                == 反向位置->second.end()) {
            结果.状态 = 结构写入状态::内部不一致;
            return 结果;
        }
        结果.状态 = 结构写入状态::幂等读回;
        return 结果;
    }
    if (!绑定主键_已加锁(主键索引_, 节点主键组_, 主键, 节点)) {
        结果.状态 = 结构写入状态::内部不一致;
        return 结果;
    }
    结果.状态 = 结构写入状态::已提交;
    结果.当前版本 = 节点.版本号;
    return 结果;
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

std::vector<std::uint64_t> 索引仓库::读取节点主键组(节点句柄 节点) const {
    if (事务接线_.已接域()) {
        auto 许可 = 事务接线_.取得共享许可(事务接线_.运行期状态);
        return 许可.有效() ? 读取节点主键组(节点, 许可.读取令牌()) : std::vector<std::uint64_t>{};
    }
    if (!节点_.节点是否有效(节点)) return {};
    std::shared_lock<std::shared_mutex> 锁(仓库锁_);
    const auto 位置 = 节点主键组_.find(节点.节点编号);
    if (位置 == 节点主键组_.end()) return {};
    auto 结果 = 位置->second;
    std::sort(结果.begin(), 结果.end());
    return 结果;
}

std::vector<std::uint64_t> 索引仓库::读取节点主键组(
    节点句柄 节点,
    const 结构事务令牌& 令牌) const {
    if (!验证共享令牌(事务接线_, 令牌) || !节点_.节点是否有效(节点, 令牌)) return {};
    std::shared_lock<std::shared_mutex> 锁(仓库锁_);
    const auto 位置 = 节点主键组_.find(节点.节点编号);
    if (位置 == 节点主键组_.end()) return {};
    auto 结果 = 位置->second;
    std::sort(结果.begin(), 结果.end());
    return 结果;
}

std::optional<节点主键删除准备包> 索引仓库::准备节点主键删除包(
    节点句柄 节点,
    const std::vector<std::uint64_t>& 精确主键组,
    const 结构事务令牌& 令牌) const {
    if (!验证独占令牌(事务接线_, 令牌)
        || !节点_.节点是否有效(节点, 令牌)
        || 令牌.许可序号 == 0) return std::nullopt;
    std::vector<std::uint64_t> 当前主键组;
    {
        std::shared_lock<std::shared_mutex> 锁(仓库锁_);
        const auto 位置 = 节点主键组_.find(节点.节点编号);
        if (位置 != 节点主键组_.end()) 当前主键组 = 位置->second;
    }
    auto 预期主键组 = 精确主键组;
    std::sort(当前主键组.begin(), 当前主键组.end());
    std::sort(预期主键组.begin(), 预期主键组.end());
    if (当前主键组 != 预期主键组) return std::nullopt;
    节点主键删除准备包 包{节点, std::move(当前主键组), 令牌.许可序号};
    return 包.完整() ? std::optional<节点主键删除准备包>{std::move(包)} : std::nullopt;
}

void 索引仓库::提交节点主键删除包(
    const 节点主键删除准备包& 包,
    const 结构事务令牌& 令牌,
    const 概念安全删除提交会话& 会话) {
    if (!验证独占令牌(事务接线_, 令牌)
        || !包.完整()
        || !会话.有效()
        || 会话.读取运行期状态() != 事务接线_.运行期状态
        || 会话.读取目标() != 包.目标
        || 会话.读取写集身份() != 包.写集身份
        || 包.写集身份 != 令牌.许可序号
        || 会话.读取阶段() != 概念安全删除提交阶段::索引) {
        throw std::logic_error("概念安全删除索引提交能力不匹配");
    }
    std::unique_lock<std::shared_mutex> 锁(仓库锁_);
    const auto 反向位置 = 节点主键组_.find(包.目标.节点编号);
    const std::size_t 当前数量 = 反向位置 == 节点主键组_.end() ? 0 : 反向位置->second.size();
    if (当前数量 != 包.主键组.size()) {
        throw std::logic_error("概念安全删除索引提交前反向主键漂移");
    }
    for (const auto 主键 : 包.主键组) {
        if (反向位置 == 节点主键组_.end()
            || std::find(反向位置->second.begin(), 反向位置->second.end(), 主键)
                == 反向位置->second.end()) {
            throw std::logic_error("概念安全删除索引提交前反向主键漂移");
        }
        const auto 位置 = 主键索引_.find(主键);
        if (位置 == 主键索引_.end() || 位置->second != 包.目标) {
            throw std::logic_error("概念安全删除索引提交前主键漂移");
        }
    }
    for (const auto 主键 : 包.主键组) 主键索引_.erase(主键);
    if (反向位置 != 节点主键组_.end()) 节点主键组_.erase(反向位置);
}

bool 索引仓库::删除主键(std::uint64_t 主键) {
    if (事务接线_.已接域()) {
        auto 许可 = 事务接线_.取得共享许可(事务接线_.运行期状态);
        return 许可.有效() && 删除主键(主键, 许可.读取令牌());
    }
    if (主键 == 0) return false;
    std::unique_lock<std::shared_mutex> 锁(仓库锁_);
    return 删除主键_已加锁(主键索引_, 节点主键组_, 主键);
}

bool 索引仓库::删除主键(std::uint64_t 主键, const 结构事务令牌& 令牌) {
    if (!验证共享令牌(事务接线_, 令牌) || 主键 == 0) return false;
    std::unique_lock<std::shared_mutex> 锁(仓库锁_);
    return 删除主键_已加锁(主键索引_, 节点主键组_, 主键);
}

std::optional<主键绑定记录> 索引仓库::读取主键绑定记录(
    std::uint64_t 主键,
    const 结构事务令牌& 令牌) const {
    if (!验证共享令牌(事务接线_, 令牌) || 主键 == 0) return std::nullopt;
    std::shared_lock<std::shared_mutex> 锁(仓库锁_);
    const auto 位置 = 主键索引_.find(主键);
    if (位置 == 主键索引_.end()) return std::nullopt;
    return 主键绑定记录{主键, 位置->second};
}

std::vector<主键绑定记录> 索引仓库::读取全部主键绑定组(const 结构事务令牌& 令牌) const {
    if (!验证共享令牌(事务接线_, 令牌)) return {};
    std::vector<主键绑定记录> 结果;
    std::shared_lock<std::shared_mutex> 锁(仓库锁_);
    try {
        结果.reserve(主键索引_.size());
        for (const auto& [主键, 节点] : 主键索引_) 结果.push_back({主键, 节点});
    } catch (...) {
        return {};
    }
    std::sort(结果.begin(), 结果.end(), [](const 主键绑定记录& 左, const 主键绑定记录& 右) {
        return 左.主键 < 右.主键;
    });
    return 结果;
}

结构写入结果 索引仓库::严格删除主键(
    std::uint64_t 主键,
    节点句柄 预期节点,
    const 结构事务令牌& 令牌) {
    结构写入结果 结果{结构写入状态::入口拒绝, 主键, 预期节点.版本号, 0};
    if (!验证独占令牌(事务接线_, 令牌)) {
        结果.状态 = 结构写入状态::许可拒绝;
        return 结果;
    }
    if (主键 == 0 || !句柄有效(预期节点)) return 结果;
    const bool 预期节点有效 = 节点_.节点是否有效(预期节点, 令牌);

    std::unique_lock<std::shared_mutex> 锁(仓库锁_);
    const auto 位置 = 主键索引_.find(主键);
    if (位置 == 主键索引_.end()) {
        结果.状态 = 结构写入状态::幂等读回;
        return 结果;
    }
    结果.当前版本 = 位置->second.版本号;
    if (位置->second != 预期节点) {
        const bool 同一身份 = 位置->second.仓库编号 == 预期节点.仓库编号
            && 位置->second.节点编号 == 预期节点.节点编号;
        结果.状态 = 同一身份 ? 结构写入状态::版本漂移 : 结构写入状态::入口拒绝;
        return 结果;
    }
    if (!预期节点有效) {
        结果.状态 = 结构写入状态::内部不一致;
        return 结果;
    }
    const auto 反向位置 = 节点主键组_.find(预期节点.节点编号);
    if (反向位置 == 节点主键组_.end()
        || std::find(反向位置->second.begin(), 反向位置->second.end(), 主键)
            == 反向位置->second.end()) {
        结果.状态 = 结构写入状态::内部不一致;
        return 结果;
    }
    if (!删除主键_已加锁(主键索引_, 节点主键组_, 主键)) {
        结果.状态 = 结构写入状态::内部不一致;
        return 结果;
    }
    结果.状态 = 结构写入状态::已提交;
    return 结果;
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
