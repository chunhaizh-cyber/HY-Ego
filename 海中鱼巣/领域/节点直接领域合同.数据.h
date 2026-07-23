// 文件规则：本头只定义节点直接领域间共享的值式结果形状，不承载仓库、事务、关系端点或可写能力。
#pragma once

#include <cstdint>
#include <optional>
#include <vector>

namespace 海中鱼巣 {

enum class 合同结果状态 : std::uint32_t {
    成功 = 1,
    幂等读回 = 2,
    合法未找到 = 3,
    合法空组 = 4,
    入口拒绝 = 5,
    许可竞争 = 6,
    写前漂移 = 7,
    读取版本漂移 = 8,
    内部错误 = 9
};

enum class 合同结果原因 : std::uint32_t {
    无 = 0,
    无效身份 = 1,
    错命名域 = 2,
    错类型 = 3,
    错版本 = 4,
    未知规则版本 = 5,
    关系角色不合法 = 6,
    基数不满足 = 7,
    同键异义 = 8,
    值归属不一致 = 9,
    时间不合法 = 10,
    自我结构不合法 = 11,
    并发许可失败 = 12,
    读取期间版本变化 = 13,
    权威读回不一致 = 14,
    事务阶段不一致 = 15
};

inline bool 合同状态携带单项材料(合同结果状态 状态) noexcept {
    return 状态 == 合同结果状态::成功 || 状态 == 合同结果状态::幂等读回;
}

inline bool 合同状态携带具名组材料(合同结果状态 状态) noexcept {
    return 状态 == 合同结果状态::成功 || 状态 == 合同结果状态::合法空组;
}

struct 合同无值结果 {
    合同结果状态 状态 = 合同结果状态::入口拒绝;
    合同结果原因 原因 = 合同结果原因::无效身份;

    bool 成功() const noexcept { return 合同状态携带单项材料(状态); }
};

template <typename 材料类型>
struct 合同单项结果 {
    合同结果状态 状态 = 合同结果状态::入口拒绝;
    合同结果原因 原因 = 合同结果原因::无效身份;
    std::optional<材料类型> 材料;

    bool 自洽() const noexcept {
        return 材料.has_value() == 合同状态携带单项材料(状态);
    }

    bool 成功() const noexcept {
        return 合同状态携带单项材料(状态) && 材料.has_value();
    }
};

template <typename 项目类型>
struct 合同组结果 {
    合同结果状态 状态 = 合同结果状态::入口拒绝;
    合同结果原因 原因 = 合同结果原因::无效身份;
    std::vector<项目类型> 项目组;
    std::uint64_t 读取结构版本 = 0;
};

template <typename 组材料类型>
struct 合同具名组结果 {
    合同结果状态 状态 = 合同结果状态::入口拒绝;
    合同结果原因 原因 = 合同结果原因::无效身份;
    std::optional<组材料类型> 材料;

    bool 自洽() const noexcept {
        return 材料.has_value() == 合同状态携带具名组材料(状态);
    }

    bool 成功() const noexcept {
        return 合同状态携带具名组材料(状态) && 材料.has_value();
    }
};

template <typename 材料类型>
using 带值读取结果 = 合同单项结果<材料类型>;

template <typename 材料类型>
struct 带值结构写入结果 {
    合同结果状态 状态 = 合同结果状态::入口拒绝;
    合同结果原因 原因 = 合同结果原因::无效身份;
    std::optional<材料类型> 材料;
    std::uint64_t 结构版本 = 0;

    bool 自洽() const noexcept {
        const bool 可带材料 = 合同状态携带单项材料(状态);
        return 材料.has_value() == 可带材料 && (可带材料 ? 结构版本 != 0 : 结构版本 == 0);
    }

    bool 成功() const noexcept {
        return 合同状态携带单项材料(状态) && 材料.has_value() && 结构版本 != 0;
    }
};

}
