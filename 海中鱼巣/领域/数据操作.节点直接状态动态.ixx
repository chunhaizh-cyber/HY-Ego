// 文件规则：数据操作只编排关系 19/20、类型化记录、索引候选和结构会话；不向调用方暴露仓库能力。
module;

#include "状态动态类型化记录.数据.h"

#include <optional>

export module 海中鱼巣.领域.数据操作.节点直接状态动态;

import 海中鱼巣.领域.参与者.状态动态类型化记录;

export namespace 海中鱼巣 {

struct 状态动态结构候选 final {
    std::optional<状态域记录> 状态记录;
    std::optional<动态域记录> 动态记录;
    std::uint64_t 结构版本 = 0;

    bool 有效() const noexcept {
        return 结构版本 != 0
            && (!状态记录 || 状态记录->值域有效())
            && (!动态记录 || 动态记录->值域有效())
            && (状态记录.has_value() || 动态记录.has_value());
    }
};

class 节点直接状态动态数据操作 final {
public:
    bool 登记状态记录(状态动态类型化记录事务参与者& 参与者,
        const 状态域记录& 记录) const noexcept {
        return 参与者.登记状态记录(记录);
    }

    bool 登记动态记录(状态动态类型化记录事务参与者& 参与者,
        const 动态域记录& 记录) const noexcept {
        return 参与者.登记动态记录(记录);
    }

    bool 形成状态关系候选(const 完整实例状态请求& 请求) const noexcept {
        return 状态请求字段有效(请求);
    }

    bool 形成动态关系候选(const 完整动态请求& 请求) const noexcept {
        return 动态请求字段有效(请求);
    }

    状态动态结构候选 形成结构候选(
        const std::optional<状态域记录>& 状态记录,
        const std::optional<动态域记录>& 动态记录,
        std::uint64_t 结构版本) const {
        状态动态结构候选 候选;
        候选.状态记录 = 状态记录;
        候选.动态记录 = 动态记录;
        候选.结构版本 = 结构版本;
        return 候选;
    }

private:
    static bool 状态请求字段有效(const 完整实例状态请求& 请求) noexcept {
        return 句柄有效(请求.主体) && 句柄有效(请求.场景)
            && 句柄有效(请求.特征) && 句柄有效(请求.特征值)
            && 句柄有效(请求.来源存在) && 请求.发生时间 != 0
            && (!请求.接收时间 || *请求.接收时间 >= 请求.发生时间)
            && static_cast<std::uint32_t>(请求.记录模式) >= 1
            && static_cast<std::uint32_t>(请求.记录模式) <= 3;
    }

    static bool 动态请求字段有效(const 完整动态请求& 请求) noexcept {
        const auto 种类值 = static_cast<std::uint32_t>(请求.种类);
        const bool 基础有效 = 句柄有效(请求.主体) && 句柄有效(请求.场景)
            && 句柄有效(请求.被改变目标) && 句柄有效(请求.前状态)
            && 句柄有效(请求.后状态) && 句柄有效(请求.来源存在)
            && 请求.发生时间 != 0 && 种类值 >= 1 && 种类值 <= 5;
        if (!基础有效) return false;
        if (请求.种类 == 动态种类::聚合动态) {
            return 请求.聚合规则版本 != 0
                && !请求.来源动作.has_value()
                && !请求.同源状态迁移动能.has_value()
                && !请求.来源低层动态组.empty();
        }
        if (请求.聚合规则版本 != 0 || !请求.来源低层动态组.empty()) return false;
        if (请求.种类 == 动态种类::动作致变) {
            return 请求.来源动作.has_value()
                && 句柄有效(*请求.来源动作)
                && 请求.同源状态迁移动能.has_value()
                && 句柄有效(*请求.同源状态迁移动能);
        }
        return !请求.来源动作.has_value()
            && !请求.同源状态迁移动能.has_value();
    }
};

}
