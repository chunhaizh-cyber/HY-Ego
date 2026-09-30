#include "新_状态类.h"

#include <algorithm>
#include <array>
#include <limits>
#include <mutex>
#include <utility>

namespace 海中鱼巣 {
namespace {

std::recursive_mutex 新状态互斥;

稳定编码 状态节点类型字段{};
稳定编码 状态节点类型{};
稳定编码 状态被描述存在字段{};
稳定编码 状态特征类型字段{};
稳定编码 状态固定准确值字段{};
稳定编码 状态强时间字段{};
稳定编码 状态被引用次数字段{};

bool 节点仍存在(稳定编码 节点) noexcept {
    return 有效(节点) && 全局基础数据集.查询节点(节点).has_value();
}

bool 结构已初始化() noexcept {
    return 有效(状态节点类型字段)
        || 有效(状态节点类型)
        || 有效(状态被描述存在字段)
        || 有效(状态特征类型字段)
        || 有效(状态固定准确值字段)
        || 有效(状态强时间字段)
        || 有效(状态被引用次数字段);
}

bool 结构仍存在() noexcept {
    return 节点仍存在(状态节点类型字段)
        && 节点仍存在(状态节点类型)
        && 节点仍存在(状态被描述存在字段)
        && 节点仍存在(状态特征类型字段)
        && 节点仍存在(状态固定准确值字段)
        && 节点仍存在(状态强时间字段)
        && 节点仍存在(状态被引用次数字段);
}

bool 强时间有效(const 新状态强时间& 时间) noexcept {
    return (时间.语义 == 新状态时间语义::实例绝对UTC
            || 时间.语义 == 新状态时间语义::抽象相对)
        && 时间.纳秒 >= 0;
}

基础值 编码强时间(const 新状态强时间& 时间) {
    return 基础值{std::vector<std::int64_t>{
        static_cast<std::int64_t>(时间.语义), 时间.纳秒}};
}

std::optional<新状态强时间> 解码强时间(const 基础字段关系& 字段) {
    const auto* 值 = std::get_if<基础值>(&字段.内容);
    if (!值) return std::nullopt;
    const auto* 分量 = std::get_if<std::vector<std::int64_t>>(&值->材料);
    if (!分量 || 分量->size() != 2) return std::nullopt;
    const 新状态强时间 时间{
        static_cast<新状态时间语义>((*分量)[0]), (*分量)[1]};
    return 强时间有效(时间)
        ? std::optional<新状态强时间>{时间} : std::nullopt;
}

std::optional<稳定编码> 读取唯一节点字段(
    稳定编码 节点, 稳定编码 字段节点) {
    const auto 字段组 = 全局基础数据集.查询字段(节点, 字段节点);
    if (字段组.size() != 1) return std::nullopt;
    const auto* 目标 = std::get_if<稳定编码>(&字段组.front().内容);
    return 目标 && 节点仍存在(*目标)
        ? std::optional<稳定编码>{*目标} : std::nullopt;
}

std::optional<新特征准确值> 读取唯一准确值字段(
    稳定编码 节点) {
    const auto 字段组 = 全局基础数据集.查询字段(
        节点, 状态固定准确值字段);
    if (字段组.size() != 1) return std::nullopt;
    if (const auto* 目标 = std::get_if<稳定编码>(&字段组.front().内容)) {
        return 节点仍存在(*目标)
            ? std::optional<新特征准确值>{新特征准确值{*目标}}
            : std::nullopt;
    }
    const auto* 值 = std::get_if<基础值>(&字段组.front().内容);
    if (!值) return std::nullopt;
    const auto* I64 = std::get_if<std::int64_t>(&值->材料);
    return I64
        ? std::optional<新特征准确值>{新特征准确值{*I64}}
        : std::nullopt;
}

std::optional<新状态强时间> 读取唯一强时间字段(稳定编码 节点) {
    const auto 字段组 = 全局基础数据集.查询字段(节点, 状态强时间字段);
    return 字段组.size() == 1 ? 解码强时间(字段组.front()) : std::nullopt;
}

std::optional<std::pair<稳定编码, std::int64_t>> 读取唯一I64字段(
    稳定编码 节点, 稳定编码 字段节点) {
    const auto 字段组 = 全局基础数据集.查询字段(节点, 字段节点);
    if (字段组.size() != 1) return std::nullopt;
    const auto* 值 = std::get_if<基础值>(&字段组.front().内容);
    if (!值) return std::nullopt;
    const auto* I64 = std::get_if<std::int64_t>(&值->材料);
    return I64
        ? std::optional<std::pair<稳定编码, std::int64_t>>{
            std::pair{字段组.front().编码, *I64}}
        : std::nullopt;
}

稳定编码 写入准确值字段(
    稳定编码 状态节点, const 新特征准确值& 值) noexcept {
    if (const auto* I64 = std::get_if<std::int64_t>(&值)) {
        return 全局基础数据集.添加字段值(
            状态节点, 状态固定准确值字段, 基础值{*I64});
    }
    const auto* 值节点 = std::get_if<稳定编码>(&值);
    return 值节点 && 节点仍存在(*值节点)
        ? 全局基础数据集.添加字段节点(
            状态节点, 状态固定准确值字段, *值节点)
        : 稳定编码{};
}

void 回滚状态节点(
    稳定编码 状态节点, const std::vector<稳定编码>& 已写字段) noexcept {
    for (auto 位置 = 已写字段.rbegin(); 位置 != 已写字段.rend(); ++位置) {
        (void)全局基础数据集.删除字段(*位置);
    }
    (void)全局基础数据集.删除节点(状态节点);
}

std::vector<稳定编码> 查询全部状态节点() {
    std::vector<稳定编码> 结果;
    for (const auto& 字段 : 全局基础数据集.查询目标字段(
        状态节点类型, 状态节点类型字段)) {
        if (节点仍存在(字段.所属节点)) 结果.push_back(字段.所属节点);
    }
    std::sort(结果.begin(), 结果.end());
    结果.erase(std::unique(结果.begin(), 结果.end()), 结果.end());
    return 结果;
}

template<class 判定>
std::vector<稳定编码> 筛选状态(判定&& 接受) {
    std::vector<稳定编码> 结果;
    for (const auto 状态节点 : 查询全部状态节点()) {
        if (接受(状态节点)) 结果.push_back(状态节点);
    }
    return 结果;
}

bool 恢复状态字段(const 新状态信息& 信息) noexcept {
    std::vector<稳定编码> 已恢复;
    auto 记录 = [&](稳定编码 字段) {
        if (!有效(字段)) return false;
        已恢复.push_back(字段);
        return true;
    };
    if (记录(全局基础数据集.添加字段节点(
            信息.节点, 状态节点类型字段, 状态节点类型))
        && 记录(全局基础数据集.添加字段节点(
            信息.节点, 状态被描述存在字段, 信息.被描述存在节点))
        && 记录(全局基础数据集.添加字段节点(
            信息.节点, 状态特征类型字段, 信息.特征类型概念节点))
        && 记录(写入准确值字段(信息.节点, 信息.固定准确值))
        && 记录(全局基础数据集.添加字段值(
            信息.节点, 状态强时间字段, 编码强时间(信息.强时间)))
        && 记录(全局基础数据集.添加字段值(
            信息.节点, 状态被引用次数字段,
            基础值{信息.被引用次数}))) {
        return true;
    }
    for (auto 位置 = 已恢复.rbegin(); 位置 != 已恢复.rend(); ++位置) {
        (void)全局基础数据集.删除字段(*位置);
    }
    return false;
}

} // namespace

新_状态类::新_状态类(新_特征类& 特征服务) noexcept
    : 特征服务_(特征服务) {}

bool 新_状态类::初始化() noexcept {
    try {
        std::lock_guard 锁(新状态互斥);
        if (!特征服务_.初始化()) return false;
        if (结构已初始化()) return 结构仍存在();
        auto 建立 = [](稳定编码& 节点) {
            if (有效(节点)) return 节点仍存在(节点);
            节点 = 全局基础数据集.新建节点();
            return 有效(节点);
        };
        return 建立(状态节点类型字段)
            && 建立(状态节点类型)
            && 建立(状态被描述存在字段)
            && 建立(状态特征类型字段)
            && 建立(状态固定准确值字段)
            && 建立(状态强时间字段)
            && 建立(状态被引用次数字段);
    } catch (...) {
        return false;
    }
}

新状态建立结果 新_状态类::建立状态(
    稳定编码 被描述存在节点,
    稳定编码 特征节点,
    const 新状态强时间& 强时间) noexcept {
    新状态建立结果 结果;
    try {
        std::lock_guard 锁(新状态互斥);
        if (!结构仍存在() || !有效(被描述存在节点)
            || !有效(特征节点)) {
            return 结果;
        }
        if (!节点仍存在(被描述存在节点)) {
            结果.状态 = 新状态操作状态::存在不存在;
            return 结果;
        }
        if (!强时间有效(强时间)) {
            结果.状态 = 新状态操作状态::时间不合法;
            return 结果;
        }
        const auto 特征 = 特征服务_.获取特征(特征节点);
        if (!特征) {
            结果.状态 = 新状态操作状态::特征不存在;
            return 结果;
        }
        if (!特征->当前值) {
            结果.状态 = 新状态操作状态::特征当前值不存在;
            return 结果;
        }

        const auto 状态节点 = 全局基础数据集.新建节点();
        if (!有效(状态节点)) {
            结果.状态 = 新状态操作状态::资源失败;
            return 结果;
        }
        std::vector<稳定编码> 已写字段;
        auto 记录 = [&](稳定编码 字段) {
            if (!有效(字段)) return false;
            已写字段.push_back(字段);
            return true;
        };
        const bool 已写完整 =
            记录(全局基础数据集.添加字段节点(
                状态节点, 状态节点类型字段, 状态节点类型))
            && 记录(全局基础数据集.添加字段节点(
                状态节点, 状态被描述存在字段, 被描述存在节点))
            && 记录(全局基础数据集.添加字段节点(
                状态节点, 状态特征类型字段, 特征->特征概念节点))
            && 记录(写入准确值字段(状态节点, *特征->当前值))
            && 记录(全局基础数据集.添加字段值(
                状态节点, 状态强时间字段, 编码强时间(强时间)))
            && 记录(全局基础数据集.添加字段值(
                状态节点, 状态被引用次数字段, 基础值{std::int64_t{0}}));
        if (!已写完整) {
            回滚状态节点(状态节点, 已写字段);
            结果.状态 = 新状态操作状态::资源失败;
            return 结果;
        }
        const auto 读回 = 获取状态(状态节点);
        if (!读回
            || 读回->被描述存在节点 != 被描述存在节点
            || 读回->特征类型概念节点 != 特征->特征概念节点
            || 读回->固定准确值 != *特征->当前值
            || 读回->强时间 != 强时间
            || 读回->被引用次数 != 0) {
            回滚状态节点(状态节点, 已写字段);
            结果.状态 = 新状态操作状态::结构不一致;
            return 结果;
        }
        结果.状态 = 新状态操作状态::已完成;
        结果.状态节点 = 状态节点;
        结果.状态信息 = *读回;
        return 结果;
    } catch (...) {
        结果.状态 = 新状态操作状态::资源失败;
        结果.状态节点.reset();
        结果.状态信息.reset();
        return 结果;
    }
}

std::optional<新状态信息> 新_状态类::获取状态(
    稳定编码 状态节点) const noexcept {
    try {
        std::lock_guard 锁(新状态互斥);
        if (!是状态节点(状态节点)) return std::nullopt;
        const auto 存在 = 读取唯一节点字段(
            状态节点, 状态被描述存在字段);
        const auto 特征类型 = 读取唯一节点字段(
            状态节点, 状态特征类型字段);
        const auto 准确值 = 读取唯一准确值字段(状态节点);
        const auto 时间 = 读取唯一强时间字段(状态节点);
        const auto 引用次数 = 读取唯一I64字段(
            状态节点, 状态被引用次数字段);
        if (!存在 || !特征类型 || !准确值 || !时间
            || !引用次数 || 引用次数->second < 0) return std::nullopt;
        return 新状态信息{
            状态节点, *存在, *特征类型, *准确值, *时间,
            引用次数->second};
    } catch (...) {
        return std::nullopt;
    }
}

bool 新_状态类::是状态节点(稳定编码 节点) const noexcept {
    try {
        std::lock_guard 锁(新状态互斥);
        if (!结构仍存在() || !节点仍存在(节点)) return false;
        const auto 类型组 = 全局基础数据集.查询字段(
            节点, 状态节点类型字段);
        const auto* 类型 = 类型组.size() == 1
            ? std::get_if<稳定编码>(&类型组.front().内容) : nullptr;
        return 类型 && *类型 == 状态节点类型;
    } catch (...) {
        return false;
    }
}

std::vector<稳定编码> 新_状态类::按存在查询状态(
    稳定编码 被描述存在节点) const noexcept {
    try {
        std::lock_guard 锁(新状态互斥);
        if (!节点仍存在(被描述存在节点)) return {};
        return 筛选状态([&](稳定编码 节点) {
            const auto 信息 = 获取状态(节点);
            return 信息 && 信息->被描述存在节点 == 被描述存在节点;
        });
    } catch (...) {
        return {};
    }
}

std::vector<稳定编码> 新_状态类::按特征类型查询状态(
    稳定编码 特征类型概念节点) const noexcept {
    try {
        std::lock_guard 锁(新状态互斥);
        if (!节点仍存在(特征类型概念节点)) return {};
        return 筛选状态([&](稳定编码 节点) {
            const auto 信息 = 获取状态(节点);
            return 信息 && 信息->特征类型概念节点 == 特征类型概念节点;
        });
    } catch (...) {
        return {};
    }
}

std::vector<稳定编码> 新_状态类::按准确值查询状态(
    const 新特征准确值& 固定准确值) const noexcept {
    try {
        std::lock_guard 锁(新状态互斥);
        if (const auto* 节点 = std::get_if<稳定编码>(&固定准确值);
            节点 && !节点仍存在(*节点)) return {};
        return 筛选状态([&](稳定编码 节点) {
            const auto 信息 = 获取状态(节点);
            return 信息 && 信息->固定准确值 == 固定准确值;
        });
    } catch (...) {
        return {};
    }
}

std::vector<稳定编码> 新_状态类::按强时间查询状态(
    const 新状态强时间& 强时间) const noexcept {
    try {
        std::lock_guard 锁(新状态互斥);
        if (!强时间有效(强时间)) return {};
        return 筛选状态([&](稳定编码 节点) {
            const auto 信息 = 获取状态(节点);
            return 信息 && 信息->强时间 == 强时间;
        });
    } catch (...) {
        return {};
    }
}

std::vector<稳定编码> 新_状态类::查找完全相同状态(
    稳定编码 被描述存在节点,
    稳定编码 特征类型概念节点,
    const 新特征准确值& 固定准确值,
    const 新状态强时间& 强时间) const noexcept {
    try {
        std::lock_guard 锁(新状态互斥);
        if (!节点仍存在(被描述存在节点)
            || !节点仍存在(特征类型概念节点)
            || !强时间有效(强时间)) return {};
        return 筛选状态([&](稳定编码 节点) {
            const auto 信息 = 获取状态(节点);
            return 信息
                && 信息->被描述存在节点 == 被描述存在节点
                && 信息->特征类型概念节点 == 特征类型概念节点
                && 信息->固定准确值 == 固定准确值
                && 信息->强时间 == 强时间;
        });
    } catch (...) {
        return {};
    }
}

新状态操作状态 新_状态类::增加引用(稳定编码 状态节点) noexcept {
    try {
        std::lock_guard 锁(新状态互斥);
        const auto 引用字段 = 读取唯一I64字段(
            状态节点, 状态被引用次数字段);
        if (!引用字段 || 引用字段->second < 0) {
            return 节点仍存在(状态节点)
                ? 新状态操作状态::结构不一致
                : 新状态操作状态::状态不存在;
        }
        if (引用字段->second == (std::numeric_limits<std::int64_t>::max)()) {
            return 新状态操作状态::引用计数溢出;
        }
        const auto 新计数 = 引用字段->second + 1;
        if (!全局基础数据集.修改字段值(
            引用字段->first, 基础值{新计数})) {
            return 新状态操作状态::资源失败;
        }
        const auto 读回 = 获取状态(状态节点);
        if (读回 && 读回->被引用次数 == 新计数) {
            return 新状态操作状态::已完成;
        }
        (void)全局基础数据集.修改字段值(
            引用字段->first, 基础值{引用字段->second});
        return 新状态操作状态::结构不一致;
    } catch (...) {
        return 新状态操作状态::资源失败;
    }
}

新状态操作状态 新_状态类::减少引用(稳定编码 状态节点) noexcept {
    try {
        std::lock_guard 锁(新状态互斥);
        const auto 引用字段 = 读取唯一I64字段(
            状态节点, 状态被引用次数字段);
        if (!引用字段) {
            return 节点仍存在(状态节点)
                ? 新状态操作状态::结构不一致
                : 新状态操作状态::状态不存在;
        }
        if (引用字段->second <= 0) return 新状态操作状态::结构不一致;
        const auto 新计数 = 引用字段->second - 1;
        if (!全局基础数据集.修改字段值(
            引用字段->first, 基础值{新计数})) {
            return 新状态操作状态::资源失败;
        }
        const auto 读回 = 获取状态(状态节点);
        if (!读回 || 读回->被引用次数 != 新计数) {
            (void)全局基础数据集.修改字段值(
                引用字段->first, 基础值{引用字段->second});
            return 新状态操作状态::结构不一致;
        }
        if (新计数 != 0) return 新状态操作状态::已完成;

        const auto 删除结果 = 删除状态(状态节点);
        if (删除结果 == 新状态操作状态::已完成) return 删除结果;
        const auto 恢复字段 = 读取唯一I64字段(
            状态节点, 状态被引用次数字段);
        if (!恢复字段 || !全局基础数据集.修改字段值(
            恢复字段->first, 基础值{引用字段->second})) {
            return 新状态操作状态::结构不一致;
        }
        const auto 恢复读回 = 获取状态(状态节点);
        return 恢复读回 && 恢复读回->被引用次数 == 引用字段->second
            ? 删除结果
            : 新状态操作状态::结构不一致;
    } catch (...) {
        return 新状态操作状态::资源失败;
    }
}

新状态操作状态 新_状态类::删除状态(稳定编码 状态节点) noexcept {
    try {
        std::lock_guard 锁(新状态互斥);
        const auto 信息 = 获取状态(状态节点);
        if (!信息) {
            return 节点仍存在(状态节点)
                ? 新状态操作状态::结构不一致
                : 新状态操作状态::状态不存在;
        }
        if (信息->被引用次数 != 0) {
            return 新状态操作状态::状态仍被引用;
        }
        std::vector<稳定编码> 字段关系组;
        for (const auto 字段节点 : std::array{
            状态节点类型字段, 状态被描述存在字段, 状态特征类型字段,
            状态固定准确值字段, 状态强时间字段,
            状态被引用次数字段}) {
            const auto 字段组 = 全局基础数据集.查询字段(状态节点, 字段节点);
            if (字段组.size() != 1) return 新状态操作状态::结构不一致;
            字段关系组.push_back(字段组.front().编码);
        }
        for (const auto 字段关系 : 字段关系组) {
            if (!全局基础数据集.删除字段(字段关系)) {
                return 新状态操作状态::资源失败;
            }
        }
        if (全局基础数据集.删除节点(状态节点)) {
            return 新状态操作状态::已完成;
        }
        return 恢复状态字段(*信息)
            ? 新状态操作状态::状态仍被引用
            : 新状态操作状态::结构不一致;
    } catch (...) {
        return 新状态操作状态::资源失败;
    }
}

} // namespace 海中鱼巣
