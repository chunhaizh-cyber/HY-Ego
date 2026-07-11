// 文件规则：关系变更必须通过本仓库入口；不得裸改关系记录。
#pragma once

#include "句柄.h"
#include "节点仓库.h"

#include <cstdint>
#include <limits>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <unordered_map>
#include <vector>

namespace 海中鱼巣 {

struct 关系记录 {
    std::uint64_t 关系编号 = 0;
    关系类型 类型 = 关系类型::普通父子;
    节点句柄 源节点;
    节点句柄 目标节点;
    std::int64_t 顺序号 = 0;
    std::uint32_t 版本号 = 0;
    记录状态 状态 = 记录状态::无效;
};

enum class 关系状态变更状态 : std::uint32_t {
    未定义 = 0,
    已变更 = 1,
    已在目标状态 = 2,
    无效句柄 = 3,
    状态转换不允许 = 4,
    版本已耗尽 = 5,
    内部不一致 = 6
};

struct 关系状态变更材料 {
    关系状态变更状态 状态 = 关系状态变更状态::未定义;
    关系句柄 原关系;
    关系句柄 当前关系;
    关系类型 类型 = 关系类型::普通父子;
    节点句柄 源节点;
    节点句柄 目标节点;
    记录状态 原状态 = 记录状态::无效;
    记录状态 当前状态 = 记录状态::无效;

    bool 完整() const {
        if ((状态 != 关系状态变更状态::已变更
                && 状态 != 关系状态变更状态::已在目标状态)
            || !句柄有效(原关系)
            || !句柄有效(当前关系)
            || !句柄有效(源节点)
            || !句柄有效(目标节点)
            || 原关系.仓库编号 != 当前关系.仓库编号
            || 原关系.关系编号 != 当前关系.关系编号
            || 当前状态 != 记录状态::已失效) {
            return false;
        }
        if (状态 == 关系状态变更状态::已变更) {
            return 原状态 == 记录状态::有效
                && 原关系.版本号 != std::numeric_limits<std::uint32_t>::max()
                && 当前关系.版本号 == 原关系.版本号 + 1;
        }
        return (原状态 == 记录状态::有效
                && 原关系.版本号 != std::numeric_limits<std::uint32_t>::max()
                && 当前关系.版本号 == 原关系.版本号 + 1)
            || (原状态 == 记录状态::已失效
                && 当前关系 == 原关系);
    }
};

enum class 节点挂载结果状态 : std::uint32_t {
    已创建 = 1,
    已重挂 = 2,
    已在目标父 = 3,
    无效端点 = 4,
    拒绝自身 = 5,
    拒绝成环 = 6,
    内部不一致 = 7
};

struct 节点挂载结果 {
    节点挂载结果状态 状态 = 节点挂载结果状态::无效端点;
    节点句柄 节点;
    std::optional<节点句柄> 旧父节点;
    节点句柄 新父节点;
    std::optional<关系句柄> 父关系;

    bool 关系已确定() const {
        if (!句柄有效(节点) || !句柄有效(新父节点) || 节点 == 新父节点 || !父关系.has_value()) {
            return false;
        }
        if (状态 == 节点挂载结果状态::已创建) {
            return !旧父节点.has_value();
        }
        if (状态 == 节点挂载结果状态::已重挂) {
            return 旧父节点.has_value() && !(旧父节点.value() == 新父节点);
        }
        return 状态 == 节点挂载结果状态::已在目标父
            && 旧父节点 == std::optional<节点句柄>{新父节点};
    }
};

class 关系仓库 {
public:
    explicit 关系仓库(const 节点仓库& 节点, std::uint64_t 仓库编号 = 1);

    关系句柄 创建关系(关系类型 类型, 节点句柄 源节点, 节点句柄 目标节点, std::int64_t 顺序号 = 0);
    std::optional<关系记录> 读取关系(关系句柄 关系) const;
    std::optional<关系记录> 读取关系审计(关系句柄 当前关系) const;
    std::vector<关系记录> 获取关系审计记录组(节点句柄 源节点, 关系类型 类型) const;
    关系状态变更材料 失效关系(关系句柄 关系);
    bool 删除关系(关系句柄 关系);
    bool 重挂关系(关系句柄 关系, 节点句柄 新源节点, 节点句柄 新目标节点);
    std::optional<关系句柄> 重挂关系并返回新句柄(
        关系句柄 关系,
        节点句柄 新源节点,
        节点句柄 新目标节点);
    bool 重挂节点(节点句柄 节点, 节点句柄 新父节点);
    节点挂载结果 挂载或重挂节点(节点句柄 节点, 节点句柄 新父节点);
    std::vector<节点句柄> 获取子节点(节点句柄 父节点) const;
    std::optional<节点句柄> 获取父节点(节点句柄 子节点) const;
    bool 节点在父链中(节点句柄 起点, 节点句柄 目标) const;
    std::optional<节点句柄> 获取目标节点(节点句柄 源节点, 关系类型 类型) const;
    std::vector<节点句柄> 获取目标节点组(节点句柄 源节点, 关系类型 类型) const;
    std::vector<节点句柄> 获取目标节点组(节点句柄 源节点, 关系类型 类型, std::int64_t 顺序号) const;
    std::vector<关系记录> 获取关系记录组(节点句柄 源节点, 关系类型 类型) const;
    std::vector<节点句柄> 获取来源节点组(节点句柄 目标节点, 关系类型 类型) const;
    std::vector<节点句柄> 获取来源节点组(节点句柄 目标节点, 关系类型 类型, std::int64_t 顺序号) const;
    std::vector<关系记录> 获取来源关系记录组(节点句柄 目标节点, 关系类型 类型) const;
    std::vector<关系记录> 获取节点相关关系记录组(节点句柄 节点) const;
    bool 存在关系(关系类型 类型, 节点句柄 源节点, 节点句柄 目标节点) const;
    bool 存在目标关系(关系类型 类型, 节点句柄 目标节点) const;
    std::uint64_t 有效关系数量() const;

private:
    struct 普通父关系复核材料 {
        std::optional<std::uint64_t> 关系编号;
        std::optional<节点句柄> 父节点;
        bool 结构不一致 = false;
    };

    enum class 父链复核结果 : std::uint32_t {
        未包含目标 = 1,
        包含目标 = 2,
        结构不一致 = 3
    };

    普通父关系复核材料 复核普通父关系_已加锁(节点句柄 子节点) const;
    父链复核结果 复核父链_已加锁(
        节点句柄 起点,
        节点句柄 目标,
        std::vector<节点句柄>* 父链节点组 = nullptr) const;

    const 节点仓库& 节点_;
    std::uint64_t 仓库编号_ = 1;
    std::uint64_t 下个关系编号_ = 1;
    mutable std::shared_mutex 仓库锁_;
    std::unordered_map<std::uint64_t, 关系记录> 关系表_;
};

}
