// 文件规则：概念活动状态只保存版本 1 的值式角色、规范根签名、关系身份和重建视图；不得承载仓库、令牌或发布能力。
#pragma once

#include "../核心/句柄.h"
#include "概念图算法.h"
#include "系统角色清单.数据.h"

#include <array>
#include <cstddef>
#include <cstdint>

namespace 海中鱼巣 {

inline constexpr std::uint32_t 概念活动材料当前版本 = 1;
inline constexpr std::uint32_t 概念活动初始化参数当前版本 = 1;
inline constexpr std::uint64_t 概念活动初始版本 = 1;

enum class 运行期概念生命周期阶段 : std::uint32_t {
    未定义 = 0,
    活跃 = 1,
    冷却 = 2,
    退役 = 3
};

enum class 概念活动业务状态 : std::uint8_t {
    已提交 = 1,
    幂等读回 = 2,
    入口拒绝 = 3,
    幂等冲突 = 4,
    许可拒绝 = 5,
    版本漂移 = 6,
    内部不一致 = 7
};

struct 概念活动初始化参数 {
    std::uint32_t 参数版本 = 概念活动初始化参数当前版本;
    std::uint64_t 活跃状态稳定主键 = 0;
    std::uint64_t 冷却状态稳定主键 = 0;
    std::uint64_t 退役状态稳定主键 = 0;

    std::array<std::uint64_t, 3> 读取稳定键组() const noexcept {
        return {活跃状态稳定主键, 冷却状态稳定主键, 退役状态稳定主键};
    }

    bool 有效() const noexcept {
        const auto 键组 = 读取稳定键组();
        return 参数版本 == 概念活动初始化参数当前版本
            && 键组[0] != 0 && 键组[1] != 0 && 键组[2] != 0
            && 键组[0] != 键组[1] && 键组[0] != 键组[2] && 键组[1] != 键组[2];
    }

    bool operator==(const 概念活动初始化参数&) const = default;
};

struct 概念活动状态角色材料 {
    运行期概念生命周期阶段 阶段 = 运行期概念生命周期阶段::未定义;
    std::uint64_t 稳定主键 = 0;
    节点句柄 状态节点;
    主信息句柄 主信息;
    std::int64_t 状态值 = 0;

    bool 完整() const noexcept {
        return 阶段 >= 运行期概念生命周期阶段::活跃
            && 阶段 <= 运行期概念生命周期阶段::退役
            && 稳定主键 != 0 && 句柄有效(状态节点) && 句柄有效(主信息)
            && 状态节点.仓库编号 == 主信息.仓库编号
            && 状态值 == static_cast<std::int64_t>(阶段);
    }

    bool operator==(const 概念活动状态角色材料&) const = default;
};

inline 系统角色稳定键用途 概念根对应系统角色用途(概念根类别 类别) noexcept {
    switch (类别) {
    case 概念根类别::存在: return 系统角色稳定键用途::概念存在根;
    case 概念根类别::动态: return 系统角色稳定键用途::概念动态根;
    case 概念根类别::关系: return 系统角色稳定键用途::概念关系根;
    case 概念根类别::因果: return 系统角色稳定键用途::概念因果根;
    default: return 系统角色稳定键用途::未定义;
    }
}

inline 节点类型 概念根对应节点类型(概念根类别 类别) noexcept {
    switch (类别) {
    case 概念根类别::存在: return 节点类型::存在;
    case 概念根类别::动态: return 节点类型::动态;
    case 概念根类别::关系: return 节点类型::二次特征;
    case 概念根类别::因果: return 节点类型::因果引用;
    default: return 节点类型::未分类;
    }
}

inline bool 是规范概念根签名(const 概念签名材料& 签名, 概念根类别 类别) noexcept {
    return 类别 >= 概念根类别::存在 && 类别 <= 概念根类别::因果
        && 签名.类别 == 类别 && 签名.签名版本 == 1 && 签名.约束组.empty();
}

struct 概念活动根材料 {
    概念根类别 类别 = 概念根类别::未定义;
    系统角色身份材料 根身份;
    概念签名材料 签名;
    系统角色关系材料 活跃关系;

    bool 完整(const 概念活动状态角色材料& 活跃角色) const noexcept {
        return 根身份.完整() && 活跃角色.完整()
            && 根身份.用途 == 概念根对应系统角色用途(类别)
            && 根身份.类型 == 概念根对应节点类型(类别)
            && 是规范概念根签名(签名, 类别)
            && 活跃关系.完整() && 活跃关系.类型 == 关系类型::概念生命周期
            && 活跃关系.源节点 == 根身份.节点
            && 活跃关系.目标节点 == 活跃角色.状态节点
            && 活跃关系.顺序号 == static_cast<std::int64_t>(类别) - 1;
    }

    bool 等于(const 概念活动根材料& 右) const noexcept {
        return 类别 == 右.类别 && 根身份 == 右.根身份
            && 是规范概念根签名(签名, 类别)
            && 是规范概念根签名(右.签名, 右.类别)
            && 活跃关系 == 右.活跃关系;
    }
};

struct 概念活动重建视图 {
    std::array<概念活动状态角色材料, 3> 状态角色组{};
    std::array<概念活动根材料, 4> 根组{};

    bool 完整() const noexcept {
        if (!状态角色组[0].完整() || !状态角色组[1].完整()
            || !状态角色组[2].完整()
            || 状态角色组[0].阶段 != 运行期概念生命周期阶段::活跃
            || 状态角色组[1].阶段 != 运行期概念生命周期阶段::冷却
            || 状态角色组[2].阶段 != 运行期概念生命周期阶段::退役) {
            return false;
        }
        for (std::size_t 索引 = 0; 索引 < 根组.size(); ++索引) {
            const auto 预期类别 = static_cast<概念根类别>(索引 + 1);
            if (根组[索引].类别 != 预期类别
                || !根组[索引].完整(状态角色组[0])) return false;
        }
        return true;
    }

    bool 等于(const 概念活动重建视图& 右) const noexcept {
        if (状态角色组 != 右.状态角色组) return false;
        for (std::size_t 索引 = 0; 索引 < 根组.size(); ++索引) {
            if (!根组[索引].等于(右.根组[索引])) return false;
        }
        return true;
    }
};

struct 概念活动材料 {
    std::uint32_t 材料版本 = 0;
    std::uint64_t 活动版本 = 0;
    概念活动初始化参数 初始化参数;
    std::array<概念活动状态角色材料, 3> 状态角色组{};
    std::array<概念活动根材料, 4> 根组{};
    概念活动重建视图 重建视图;

    bool 完整() const noexcept {
        if (材料版本 != 概念活动材料当前版本
            || 活动版本 == 0 || !初始化参数.有效() || !重建视图.完整()
            || 状态角色组 != 重建视图.状态角色组) return false;
        const auto 键组 = 初始化参数.读取稳定键组();
        for (std::size_t 索引 = 0; 索引 < 状态角色组.size(); ++索引) {
            if (状态角色组[索引].稳定主键 != 键组[索引]) return false;
        }
        for (std::size_t 索引 = 0; 索引 < 根组.size(); ++索引) {
            if (!根组[索引].等于(重建视图.根组[索引])) return false;
        }
        const auto 仓库编号 = 状态角色组[0].状态节点.仓库编号;
        for (const auto& 角色 : 状态角色组) {
            if (!角色.完整() || 角色.状态节点.仓库编号 != 仓库编号) return false;
        }
        for (const auto& 根 : 根组) {
            if (根.根身份.节点.仓库编号 != 仓库编号
                || 根.活跃关系.关系.仓库编号 != 仓库编号) return false;
        }
        return true;
    }

    bool 匹配参数(const 概念活动初始化参数& 参数) const noexcept {
        return 完整() && 参数.有效() && 初始化参数 == 参数;
    }

    bool 等于(const 概念活动材料& 右) const noexcept {
        if (材料版本 != 右.材料版本 || 活动版本 != 右.活动版本
            || !(初始化参数 == 右.初始化参数) || 状态角色组 != 右.状态角色组
            || !重建视图.等于(右.重建视图)) return false;
        for (std::size_t 索引 = 0; 索引 < 根组.size(); ++索引) {
            if (!根组[索引].等于(右.根组[索引])) return false;
        }
        return true;
    }
};

struct 概念活动重建结果 {
    概念活动业务状态 状态 = 概念活动业务状态::入口拒绝;
    概念活动重建视图 视图;

    bool 成功() const noexcept {
        return 状态 == 概念活动业务状态::幂等读回 && 视图.完整();
    }
};

struct 概念活动初始化结果 {
    概念活动业务状态 状态 = 概念活动业务状态::入口拒绝;
    概念活动材料 材料;

    bool 成功() const noexcept {
        return (状态 == 概念活动业务状态::已提交
                || 状态 == 概念活动业务状态::幂等读回)
            && 材料.完整();
    }

    bool 改变了结构() const noexcept { return 状态 == 概念活动业务状态::已提交; }
};

}
