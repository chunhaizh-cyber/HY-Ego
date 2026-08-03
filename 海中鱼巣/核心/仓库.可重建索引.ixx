// 文件规则：本模块只承载可丢弃、可重建的索引物理定位；命中后必须回读权威节点或关系。
module;

#include "句柄.h"
#include "节点直接结构合同.数据.h"

#include <algorithm>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <mutex>
#include <new>
#include <optional>
#include <shared_mutex>
#include <unordered_map>
#include <utility>
#include <vector>

export module 海中鱼巣.核心.仓库.可重建索引;

import 海中鱼巣.核心.仓库.节点直接身份;
import 海中鱼巣.核心.仓库.正式关系;

export namespace 海中鱼巣 {

enum class 索引目标种类 : std::uint8_t {
    节点 = 1,
    关系 = 2
};

struct 可重建索引记录 {
    索引物理键 物理键;
    索引目标种类 目标种类 = 索引目标种类::节点;
    节点句柄 节点;
    关系句柄 关系;
};

inline bool 可重建索引记录完整(const 可重建索引记录& 记录) noexcept {
    if (!索引物理键完整(记录.物理键)) return false;
    if (记录.目标种类 == 索引目标种类::节点) {
        return 句柄有效(记录.节点) && !句柄有效(记录.关系);
    }
    return 记录.目标种类 == 索引目标种类::关系
        && 句柄有效(记录.关系) && !句柄有效(记录.节点);
}

enum class 可重建索引操作状态 : std::uint32_t {
    已创建候选 = 1,
    幂等读回 = 2,
    已确认待发布 = 3,
    已撤销 = 4,
    已发布 = 5,
    入口拒绝_无效事务 = 6,
    入口拒绝_无效物理键 = 7,
    入口拒绝_无效目标 = 8,
    入口拒绝_物理键冲突 = 9,
    内部不一致 = 10
};

enum class 可重建索引候选阶段 : std::uint8_t {
    持有 = 1,
    已确认待发布 = 2,
    已撤销 = 3,
    已发布 = 4,
    已移动 = 5
};

class 可重建索引仓库;

class 可重建索引候选 {
public:
    可重建索引候选() = delete;
    可重建索引候选(const 可重建索引候选&) = delete;
    可重建索引候选& operator=(const 可重建索引候选&) = delete;
    可重建索引候选(可重建索引候选&& 其它) noexcept
        : 事务序号_(其它.事务序号_), 记录_(其它.记录_), 是移除_(其它.是移除_), 阶段_(其它.阶段_) {
        其它.事务序号_ = 0;
        其它.阶段_ = 可重建索引候选阶段::已移动;
    }
    可重建索引候选& operator=(可重建索引候选&&) = delete;

    std::uint64_t 读取事务序号() const noexcept { return 事务序号_; }
    const 可重建索引记录& 读取记录() const noexcept { return 记录_; }
    可重建索引候选阶段 读取阶段() const noexcept { return 阶段_; }

private:
    friend class 可重建索引仓库;
    可重建索引候选(std::uint64_t 事务序号, 可重建索引记录 记录, bool 是移除 = false) noexcept
        : 事务序号_(事务序号), 记录_(std::move(记录)), 是移除_(是移除) {
    }
    std::uint64_t 事务序号_ = 0;
    可重建索引记录 记录_;
    bool 是移除_ = false;
    可重建索引候选阶段 阶段_ = 可重建索引候选阶段::持有;
};

struct 可重建索引写入结果 {
    可重建索引操作状态 状态 = 可重建索引操作状态::入口拒绝_无效物理键;
    std::optional<可重建索引记录> 当前记录;
    std::optional<可重建索引候选> 候选;

    bool 成功() const noexcept {
        return 状态 == 可重建索引操作状态::已创建候选
            || 状态 == 可重建索引操作状态::幂等读回;
    }
};

struct 可重建索引权威材料 {
    std::vector<可重建索引记录> 记录组;
};

enum class 可重建索引精确读取状态 : std::uint8_t {
    已读取 = 1,
    未找到 = 2,
    入口拒绝 = 3,
    资源失败 = 4,
    内部不一致 = 5
};

struct 可重建索引精确读取结果 {
    可重建索引精确读取状态 状态 = 可重建索引精确读取状态::入口拒绝;
    std::optional<可重建索引记录> 当前记录;
};

#if defined(HY_EGO_ENABLE_STRUCTURE_COMMIT_FAULT_SELF_TEST)
struct 可重建索引精确读取测试异常 {};

enum class 可重建索引精确读取内部损坏测试种类 : std::uint8_t {
    已发布候选事务序号非零 = 1,
    记录键错配 = 2,
    反向绑定缺失 = 3,
    反向绑定重复 = 4,
    目标权威不可读 = 5
};
#endif

class 可重建索引恢复候选 final {
public:
    可重建索引恢复候选(const 可重建索引恢复候选&) = delete;
    可重建索引恢复候选& operator=(const 可重建索引恢复候选&) = delete;
    可重建索引恢复候选(可重建索引恢复候选&& 其它) noexcept
        : 仓库_(其它.仓库_), 事务序号_(其它.事务序号_), 材料_(std::move(其它.材料_)), 阶段_(其它.阶段_) {
        其它.仓库_ = nullptr; 其它.事务序号_ = 0; 其它.阶段_ = 节点直接仓候选阶段::已移动;
    }
    可重建索引恢复候选& operator=(可重建索引恢复候选&&) = delete;
    节点直接仓候选阶段 读取阶段() const noexcept { return 阶段_; }
private:
    friend class 可重建索引仓库;
    可重建索引恢复候选(可重建索引仓库* 仓库, std::uint64_t 事务序号, 可重建索引权威材料 材料)
        : 仓库_(仓库), 事务序号_(事务序号), 材料_(std::move(材料)) {}
    可重建索引仓库* 仓库_ = nullptr;
    std::uint64_t 事务序号_ = 0;
    可重建索引权威材料 材料_;
    节点直接仓候选阶段 阶段_ = 节点直接仓候选阶段::持有;
};

struct 可重建索引恢复候选结果 {
    节点直接恢复仓操作状态 状态 = 节点直接恢复仓操作状态::入口拒绝;
    std::optional<可重建索引恢复候选> 候选;
};

class 可重建索引仓库 {
public:
    可重建索引仓库(
        const 节点直接身份仓库& 节点,
        const 正式关系仓库& 关系)
        : 节点_(节点), 关系_(关系) {
    }

    可重建索引写入结果 结构化创建索引候选(
        可重建索引记录 记录,
        std::uint64_t 事务序号) {
        可重建索引写入结果 结果;
        if (事务序号 == 0) {
            结果.状态 = 可重建索引操作状态::入口拒绝_无效事务;
            return 结果;
        }
        if (!可重建索引记录完整(记录)) {
            结果.状态 = 可重建索引操作状态::入口拒绝_无效物理键;
            return 结果;
        }
        if (!目标权威可读_(记录, 事务序号)) {
            结果.状态 = 可重建索引操作状态::入口拒绝_无效目标;
            return 结果;
        }
        std::unique_lock 锁(仓库锁_);
        const auto 位置 = 正向绑定_.find(记录.物理键);
        if (位置 != 正向绑定_.end()) {
            if (位置->second.已发布 && 记录相同_(位置->second.记录, 记录)) {
                结果.状态 = 可重建索引操作状态::幂等读回;
                结果.当前记录 = 位置->second.记录;
            } else {
                结果.状态 = 可重建索引操作状态::入口拒绝_物理键冲突;
            }
            return 结果;
        }
        try {
            const auto 目标 = 形成目标标识_(记录);
            auto& 键组 = 反向绑定_[目标];
            键组.push_back(记录.物理键);
            try {
                const auto [插入位置, 已插入] = 正向绑定_.emplace(记录.物理键, 索引条目{记录, false, 事务序号});
                if (!已插入) {
                    键组.pop_back();
                    结果.状态 = 可重建索引操作状态::内部不一致;
                    return 结果;
                }
            } catch (...) {
                键组.pop_back();
                throw;
            }
        } catch (...) {
            结果.状态 = 可重建索引操作状态::内部不一致;
            return 结果;
        }
        结果.状态 = 可重建索引操作状态::已创建候选;
        结果.当前记录 = 记录;
        结果.候选.emplace(可重建索引候选{事务序号, 记录});
        return 结果;
    }

    可重建索引写入结果 结构化移除索引候选(
        const 索引物理键& 键,
        const 节点稳定身份见证& 预期目标,
        std::uint64_t 事务序号) {
        可重建索引写入结果 结果;
        if (事务序号 == 0) {
            结果.状态 = 可重建索引操作状态::入口拒绝_无效事务;
            return 结果;
        }
        if (!索引物理键完整(键) || !节点稳定身份见证完整(预期目标)) return 结果;
        const auto 节点读取 = 节点_.读取稳定主键当前身份(预期目标.稳定主键, 事务序号);
        if (节点读取.状态 != 稳定主键当前身份状态::当前有效 || !节点读取.当前身份) {
            结果.状态 = 可重建索引操作状态::入口拒绝_无效目标;
            return 结果;
        }
        const auto 节点记录 = 节点_.读取节点(*节点读取.当前身份, 事务序号);
        if (!节点记录 || 节点记录->类型 != 预期目标.类型
            || 节点记录->版本号 != 预期目标.身份版本) {
            结果.状态 = 可重建索引操作状态::入口拒绝_无效目标;
            return 结果;
        }
        std::shared_lock 锁(仓库锁_);
        const auto 位置 = 正向绑定_.find(键);
        if (位置 == 正向绑定_.end() || !位置->second.已发布
            || 位置->second.记录.目标种类 != 索引目标种类::节点
            || 位置->second.记录.节点 != *节点读取.当前身份) {
            结果.状态 = 可重建索引操作状态::入口拒绝_物理键冲突;
            return 结果;
        }
        结果.状态 = 可重建索引操作状态::已创建候选;
        结果.当前记录 = 位置->second.记录;
        结果.候选.emplace(可重建索引候选{事务序号, 位置->second.记录, true});
        return 结果;
    }

    可重建索引精确读取结果 读取索引物理键结构化(
        const 索引物理键& 键) const {
        if (!索引物理键完整(键)) return {};
        try {
#if defined(HY_EGO_ENABLE_STRUCTURE_COMMIT_FAULT_SELF_TEST)
            const auto 故障 = 测试_下次结构化精确读取故障_.exchange(
                0, std::memory_order_acq_rel);
            if (故障 == 1) throw std::bad_alloc{};
            if (故障 == 2) throw 可重建索引精确读取测试异常{};
#endif
            可重建索引记录 记录;
            {
                std::shared_lock 锁(仓库锁_);
                const auto 位置 = 正向绑定_.find(键);
                if (位置 == 正向绑定_.end() || !位置->second.已发布) {
                    return {可重建索引精确读取状态::未找到, std::nullopt};
                }
                if (
#if defined(HY_EGO_ENABLE_STRUCTURE_COMMIT_FAULT_SELF_TEST)
                    故障 == 3 ||
#endif
                    位置->second.候选事务序号 != 0
                    || !可重建索引记录完整(位置->second.记录)
#if defined(HY_EGO_ENABLE_STRUCTURE_COMMIT_FAULT_SELF_TEST)
                    || 故障 == 4
#endif
                    || !(位置->second.记录.物理键 == 键)) {
                    return {可重建索引精确读取状态::内部不一致, std::nullopt};
                }
                const auto 目标 = 形成目标标识_(位置->second.记录);
                const auto 反向位置 = 反向绑定_.find(目标);
                if (
#if defined(HY_EGO_ENABLE_STRUCTURE_COMMIT_FAULT_SELF_TEST)
                    故障 == 5 ||
#endif
                    反向位置 == 反向绑定_.end()
#if defined(HY_EGO_ENABLE_STRUCTURE_COMMIT_FAULT_SELF_TEST)
                    || 故障 == 6
#endif
                    || std::count(反向位置->second.begin(), 反向位置->second.end(), 键) != 1) {
                    return {可重建索引精确读取状态::内部不一致, std::nullopt};
                }
                记录 = 位置->second.记录;
            }
            if (
#if defined(HY_EGO_ENABLE_STRUCTURE_COMMIT_FAULT_SELF_TEST)
                故障 == 7 ||
#endif
                !目标权威可读_(记录, std::nullopt)) {
                return {可重建索引精确读取状态::内部不一致, std::nullopt};
            }
            return {可重建索引精确读取状态::已读取, 记录};
        } catch (const std::bad_alloc&) {
            return {可重建索引精确读取状态::资源失败, std::nullopt};
        } catch (...) {
            return {可重建索引精确读取状态::内部不一致, std::nullopt};
        }
    }

    std::optional<可重建索引记录> 读取索引物理键(const 索引物理键& 键) const {
        auto 结果 = 读取索引物理键结构化(键);
        return 结果.状态 == 可重建索引精确读取状态::已读取
            ? std::move(结果.当前记录) : std::nullopt;
    }

#if defined(HY_EGO_ENABLE_STRUCTURE_COMMIT_FAULT_SELF_TEST)
    void 测试_令下次结构化精确读取抛出资源异常() noexcept {
        测试_下次结构化精确读取故障_.store(1, std::memory_order_release);
    }

    void 测试_令下次结构化精确读取抛出其它异常() noexcept {
        测试_下次结构化精确读取故障_.store(2, std::memory_order_release);
    }

    bool 测试_令下次结构化精确读取模拟内部损坏(
        可重建索引精确读取内部损坏测试种类 种类) noexcept {
        const auto 值 = static_cast<std::uint8_t>(种类);
        if (值 < 1 || 值 > 5) return false;
        测试_下次结构化精确读取故障_.store(
            static_cast<std::uint8_t>(2 + 值), std::memory_order_release);
        return true;
    }
#endif

    std::optional<可重建索引记录> 读取索引物理键(
        const 索引物理键& 键,
        std::uint64_t 事务序号) const {
        auto 记录 = 读取索引物理键核心_(键, 事务序号);
        return 记录.has_value() && 目标权威可读_(记录.value(), 事务序号)
            ? 记录 : std::nullopt;
    }

    std::vector<索引物理键> 读取目标索引物理键组(const 可重建索引记录& 目标) const {
        std::vector<索引物理键> 键组;
        {
            std::shared_lock 锁(仓库锁_);
            const auto 位置 = 反向绑定_.find(形成目标标识_(目标));
            if (位置 != 反向绑定_.end()) 键组 = 位置->second;
        }
        键组.erase(std::remove_if(键组.begin(), 键组.end(), [this] (const auto& 键) {
            return !读取索引物理键(键).has_value();
        }), 键组.end());
        return 键组;
    }

    可重建索引操作状态 确认候选(可重建索引候选& 候选, std::uint64_t 事务序号) {
        std::unique_lock 锁(仓库锁_);
        if (!候选匹配_(候选, 事务序号, 可重建索引候选阶段::持有)) {
            return 可重建索引操作状态::入口拒绝_无效事务;
        }
        const auto 位置 = 正向绑定_.find(候选.记录_.物理键);
        if (位置 == 正向绑定_.end() || !记录相同_(位置->second.记录, 候选.记录_)
            || (候选.是移除_ ? !位置->second.已发布
                              : (位置->second.已发布 || 位置->second.候选事务序号 != 事务序号))) {
            return 可重建索引操作状态::内部不一致;
        }
        候选.阶段_ = 可重建索引候选阶段::已确认待发布;
        return 可重建索引操作状态::已确认待发布;
    }

    可重建索引操作状态 撤销候选(可重建索引候选& 候选, std::uint64_t 事务序号) {
        std::unique_lock 锁(仓库锁_);
        if (!候选匹配_(候选, 事务序号, 候选.阶段_)
            || (候选.阶段_ != 可重建索引候选阶段::持有
                && 候选.阶段_ != 可重建索引候选阶段::已确认待发布)) {
            return 可重建索引操作状态::入口拒绝_无效事务;
        }
        if (候选.是移除_) {
            候选.阶段_ = 可重建索引候选阶段::已撤销;
            return 可重建索引操作状态::已撤销;
        }
        const auto 位置 = 正向绑定_.find(候选.记录_.物理键);
        if (位置 == 正向绑定_.end() || 位置->second.已发布
            || 位置->second.候选事务序号 != 事务序号) {
            return 可重建索引操作状态::内部不一致;
        }
        const auto 目标 = 形成目标标识_(位置->second.记录);
        const auto 反向位置 = 反向绑定_.find(目标);
        if (反向位置 == 反向绑定_.end()) return 可重建索引操作状态::内部不一致;
        const auto 键位置 = std::find(反向位置->second.begin(), 反向位置->second.end(), 候选.记录_.物理键);
        if (键位置 == 反向位置->second.end()) return 可重建索引操作状态::内部不一致;
        反向位置->second.erase(键位置);
        if (反向位置->second.empty()) 反向绑定_.erase(反向位置);
        正向绑定_.erase(位置);
        候选.阶段_ = 可重建索引候选阶段::已撤销;
        return 可重建索引操作状态::已撤销;
    }

    void 完成发布(可重建索引候选& 候选, std::uint64_t 事务序号) noexcept {
        if (!候选匹配_(候选, 事务序号, 可重建索引候选阶段::已确认待发布)) return;
        std::unique_lock 锁(仓库锁_);
        const auto 位置 = 正向绑定_.find(候选.记录_.物理键);
        if (候选.是移除_) {
            if (位置 == 正向绑定_.end() || !位置->second.已发布
                || !记录相同_(位置->second.记录, 候选.记录_)) return;
            const auto 目标 = 形成目标标识_(位置->second.记录);
            const auto 反向位置 = 反向绑定_.find(目标);
            if (反向位置 == 反向绑定_.end()) return;
            const auto 键位置 = std::find(反向位置->second.begin(), 反向位置->second.end(), 候选.记录_.物理键);
            if (键位置 == 反向位置->second.end()) return;
            反向位置->second.erase(键位置);
            if (反向位置->second.empty()) 反向绑定_.erase(反向位置);
            正向绑定_.erase(位置);
            候选.阶段_ = 可重建索引候选阶段::已发布;
            return;
        }
        if (位置 == 正向绑定_.end() || 位置->second.已发布
            || 位置->second.候选事务序号 != 事务序号) return;
        位置->second.已发布 = true;
        位置->second.候选事务序号 = 0;
        候选.阶段_ = 可重建索引候选阶段::已发布;
    }

    bool 清空可重建索引(std::uint64_t 事务序号) {
        if (事务序号 == 0) return false;
        std::unique_lock 锁(仓库锁_);
        for (const auto& [键, 条目] : 正向绑定_) if (!条目.已发布) return false;
        正向绑定_.clear();
        反向绑定_.clear();
        return true;
    }

    bool 从权威记录重建(
        const std::vector<可重建索引记录>& 记录组,
        std::uint64_t 事务序号) {
        if (事务序号 == 0) return false;
        for (const auto& 记录 : 记录组) {
            if (!可重建索引记录完整(记录) || !目标权威可读_(记录, 事务序号)) return false;
        }
        decltype(正向绑定_) 新正向;
        decltype(反向绑定_) 新反向;
        try {
            for (const auto& 记录 : 记录组) {
                if (!新正向.emplace(记录.物理键, 索引条目{记录, true, 0}).second) return false;
                新反向[形成目标标识_(记录)].push_back(记录.物理键);
            }
        } catch (...) {
            return false;
        }
        std::unique_lock 锁(仓库锁_);
        for (const auto& [键, 条目] : 正向绑定_) if (!条目.已发布) return false;
        正向绑定_.swap(新正向);
        反向绑定_.swap(新反向);
        return true;
    }

    可重建索引恢复候选结果 结构化建立恢复未发布候选(
        可重建索引权威材料 材料, std::uint64_t 事务序号) {
        可重建索引恢复候选结果 结果;
        std::sort(材料.记录组.begin(), 材料.记录组.end(), [](const auto& 左, const auto& 右) {
            if (左.物理键.所有者身份 != 右.物理键.所有者身份) return 左.物理键.所有者身份 < 右.物理键.所有者身份;
            if (左.物理键.命名域 != 右.物理键.命名域) return 左.物理键.命名域 < 右.物理键.命名域;
            if (左.物理键.键格式版本 != 右.物理键.键格式版本) return 左.物理键.键格式版本 < 右.物理键.键格式版本;
            if (左.物理键.探测规则版本 != 右.物理键.探测规则版本) return 左.物理键.探测规则版本 < 右.物理键.探测规则版本;
            return 左.物理键.键值 < 右.物理键.键值;
        });
        if (事务序号 == 0 || std::any_of(材料.记录组.begin(), 材料.记录组.end(),
                [](const auto& 记录) { return !可重建索引记录完整(记录); })) {
            结果.状态 = 节点直接恢复仓操作状态::材料不完整;
            return 结果;
        }
        for (std::size_t 序号 = 1; 序号 < 材料.记录组.size(); ++序号) {
            if (材料.记录组[序号 - 1].物理键 == 材料.记录组[序号].物理键) {
                结果.状态 = 节点直接恢复仓操作状态::身份冲突;
                return 结果;
            }
        }
        std::unique_lock 锁(仓库锁_);
        if (恢复事务序号_ != 0) { 结果.状态 = 节点直接恢复仓操作状态::身份冲突; return 结果; }
        if (!正向绑定_.empty() || !反向绑定_.empty()) {
            结果.状态 = 节点直接恢复仓操作状态::版本漂移;
            return 结果;
        }
        try {
            恢复事务序号_ = 事务序号;
            结果.候选.emplace(可重建索引恢复候选{this, 事务序号, std::move(材料)});
            结果.状态 = 节点直接恢复仓操作状态::已形成候选;
        } catch (...) { 恢复事务序号_ = 0; 结果.状态 = 节点直接恢复仓操作状态::资源失败; }
        return 结果;
    }

    节点直接恢复仓操作状态 确认恢复候选(
        可重建索引恢复候选& 候选, std::uint64_t 事务序号) {
        std::unique_lock 锁(仓库锁_);
        if (!恢复候选匹配_(候选, 事务序号, 节点直接仓候选阶段::持有))
            return 节点直接恢复仓操作状态::入口拒绝;
        候选.阶段_ = 节点直接仓候选阶段::已确认待发布;
        return 节点直接恢复仓操作状态::已确认待发布;
    }

    节点直接恢复仓操作状态 撤销恢复候选(
        可重建索引恢复候选& 候选, std::uint64_t 事务序号) noexcept {
        try {
            std::unique_lock 锁(仓库锁_);
            if (候选.仓库_ != this || 候选.事务序号_ != 事务序号 || 恢复事务序号_ != 事务序号
                || 事务序号 == 0 || (候选.阶段_ != 节点直接仓候选阶段::持有
                    && 候选.阶段_ != 节点直接仓候选阶段::已确认待发布))
                return 节点直接恢复仓操作状态::入口拒绝;
            恢复事务序号_ = 0;
            候选.阶段_ = 节点直接仓候选阶段::已撤销;
            return 节点直接恢复仓操作状态::已撤销;
        } catch (...) { return 节点直接恢复仓操作状态::内部不一致; }
    }

    节点直接恢复仓操作状态 完成发布恢复候选(
        可重建索引恢复候选& 候选, std::uint64_t 事务序号) noexcept {
        try {
            decltype(正向绑定_) 新正向;
            decltype(反向绑定_) 新反向;
            for (const auto& 记录 : 候选.材料_.记录组) {
                if (!目标权威可读_(记录, std::nullopt)) return 节点直接恢复仓操作状态::关系矛盾;
                if (!新正向.emplace(记录.物理键, 索引条目{记录, true, 0}).second)
                    return 节点直接恢复仓操作状态::身份冲突;
                新反向[形成目标标识_(记录)].push_back(记录.物理键);
            }
            std::unique_lock 锁(仓库锁_);
            if (!恢复候选匹配_(候选, 事务序号, 节点直接仓候选阶段::已确认待发布)
                || !正向绑定_.empty() || !反向绑定_.empty())
                return 节点直接恢复仓操作状态::内部不一致;
            正向绑定_.swap(新正向);
            反向绑定_.swap(新反向);
            恢复事务序号_ = 0;
            候选.阶段_ = 节点直接仓候选阶段::已发布;
            return 节点直接恢复仓操作状态::已发布;
        } catch (...) { return 节点直接恢复仓操作状态::资源失败; }
    }

    可重建索引权威材料 导出可丢弃加速材料() const {
        可重建索引权威材料 材料;
        std::shared_lock 锁(仓库锁_);
        for (const auto& [键, 条目] : 正向绑定_) if (条目.已发布) 材料.记录组.push_back(条目.记录);
        std::sort(材料.记录组.begin(), 材料.记录组.end(), [] (const auto& 左, const auto& 右) {
            if (左.物理键.所有者身份 != 右.物理键.所有者身份) return 左.物理键.所有者身份 < 右.物理键.所有者身份;
            if (左.物理键.命名域 != 右.物理键.命名域) return 左.物理键.命名域 < 右.物理键.命名域;
            return 左.物理键.键值 < 右.物理键.键值;
        });
        return 材料;
    }

private:
    struct 索引物理键哈希 {
        std::size_t operator()(const 索引物理键& 键) const noexcept {
            std::size_t 结果 = static_cast<std::size_t>(键.所有者身份 ^ (键.键值 << 1));
            结果 ^= static_cast<std::size_t>(键.命名域) << 7;
            结果 ^= static_cast<std::size_t>(键.键格式版本) << 17;
            结果 ^= static_cast<std::size_t>(键.探测规则版本) << 25;
            return 结果;
        }
    };

    struct 索引目标标识 {
        索引目标种类 种类 = 索引目标种类::节点;
        std::uint64_t 仓库编号 = 0;
        std::uint64_t 本地编号 = 0;
        std::uint32_t 版本号 = 0;

        bool operator==(const 索引目标标识& 其它) const noexcept {
            return 种类 == 其它.种类 && 仓库编号 == 其它.仓库编号
                && 本地编号 == 其它.本地编号 && 版本号 == 其它.版本号;
        }
    };

    struct 索引目标哈希 {
        std::size_t operator()(const 索引目标标识& 目标) const noexcept {
            return static_cast<std::size_t>(目标.本地编号 ^ (目标.仓库编号 << 1)
                ^ (static_cast<std::uint64_t>(目标.版本号) << 33)
                ^ (static_cast<std::uint64_t>(目标.种类) << 61));
        }
    };

    struct 索引条目 {
        可重建索引记录 记录;
        bool 已发布 = false;
        std::uint64_t 候选事务序号 = 0;
    };

    static 索引目标标识 形成目标标识_(const 可重建索引记录& 记录) noexcept {
        if (记录.目标种类 == 索引目标种类::节点) {
            return {索引目标种类::节点, 记录.节点.仓库编号, 记录.节点.节点编号, 记录.节点.版本号};
        }
        return {索引目标种类::关系, 记录.关系.仓库编号, 记录.关系.关系编号, 记录.关系.版本号};
    }

    bool 目标权威可读_(const 可重建索引记录& 记录, std::optional<std::uint64_t> 事务序号) const {
        if (记录.目标种类 == 索引目标种类::节点) {
            return 事务序号.has_value() ? 节点_.读取节点(记录.节点, 事务序号.value()).has_value()
                : 节点_.读取节点(记录.节点).has_value();
        }
        return 事务序号.has_value() ? 关系_.读取关系(记录.关系, 事务序号.value()).has_value()
            : 关系_.读取关系(记录.关系).has_value();
    }

    std::optional<可重建索引记录> 读取索引物理键核心_(
        const 索引物理键& 键,
        std::optional<std::uint64_t> 事务序号) const {
        if (!索引物理键完整(键)) return std::nullopt;
        std::shared_lock 锁(仓库锁_);
        const auto 位置 = 正向绑定_.find(键);
        if (位置 == 正向绑定_.end()) return std::nullopt;
        if (!位置->second.已发布
            && (!事务序号.has_value() || 位置->second.候选事务序号 != 事务序号.value())) return std::nullopt;
        const auto 反向位置 = 反向绑定_.find(形成目标标识_(位置->second.记录));
        if (反向位置 == 反向绑定_.end()
            || std::count(反向位置->second.begin(), 反向位置->second.end(), 键) != 1) return std::nullopt;
        return 位置->second.记录;
    }

    static bool 记录相同_(const 可重建索引记录& 左, const 可重建索引记录& 右) noexcept {
        return 左.物理键 == 右.物理键 && 左.目标种类 == 右.目标种类
            && 左.节点 == 右.节点 && 左.关系 == 右.关系;
    }

    static bool 候选匹配_(
        const 可重建索引候选& 候选,
        std::uint64_t 事务序号,
        可重建索引候选阶段 阶段) noexcept {
        return 事务序号 != 0 && 候选.事务序号_ == 事务序号 && 候选.阶段_ == 阶段;
    }

    bool 恢复候选匹配_(const 可重建索引恢复候选& 候选,
        std::uint64_t 事务序号, 节点直接仓候选阶段 阶段) const noexcept {
        return 事务序号 != 0 && 恢复事务序号_ == 事务序号
            && 候选.仓库_ == this && 候选.事务序号_ == 事务序号 && 候选.阶段_ == 阶段;
    }

    const 节点直接身份仓库& 节点_;
    const 正式关系仓库& 关系_;
    mutable std::shared_mutex 仓库锁_;
    std::unordered_map<索引物理键, 索引条目, 索引物理键哈希> 正向绑定_;
    std::unordered_map<索引目标标识, std::vector<索引物理键>, 索引目标哈希> 反向绑定_;
    std::uint64_t 恢复事务序号_ = 0;
#if defined(HY_EGO_ENABLE_STRUCTURE_COMMIT_FAULT_SELF_TEST)
    mutable std::atomic<std::uint8_t> 测试_下次结构化精确读取故障_{0};
#endif
};

}
