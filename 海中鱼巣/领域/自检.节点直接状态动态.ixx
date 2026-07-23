// 文件规则：专项自检只验证 P2B 候选的字段、角色和所有权边界，不进入生产依赖和统一运行器。
module;

#include "状态动态类型化记录.数据.h"

export module 海中鱼巣.领域.自检.节点直接状态动态;

export namespace 海中鱼巣 {

struct 节点直接状态动态自检 final {
    static bool 状态记录字段检查() noexcept {
        状态域记录 记录;
        return !记录.值域有效();
    }

    static bool 动态记录字段检查() noexcept {
        动态域记录 记录;
        return !记录.值域有效();
    }

    static bool 状态请求角色检查(
        const 完整实例状态请求& 请求) noexcept {
        return 句柄有效(请求.主体) && 句柄有效(请求.场景)
            && 句柄有效(请求.特征) && 句柄有效(请求.特征值)
            && 句柄有效(请求.来源存在);
    }

    static bool 动态请求角色检查(
        const 完整动态请求& 请求) noexcept {
        const auto 种类值 = static_cast<std::uint32_t>(请求.种类);
        return 种类值 >= 1 && 种类值 <= 5
            && 句柄有效(请求.主体) && 句柄有效(请求.场景)
            && 句柄有效(请求.被改变目标) && 句柄有效(请求.前状态)
            && 句柄有效(请求.后状态) && 句柄有效(请求.来源存在);
    }

    static bool 退役只读边界检查(
        const 状态退役规格& 状态, const 动态退役规格& 动态) noexcept {
        return 句柄有效(状态.目标) && 状态.预期版本 != 0
            && 句柄有效(动态.目标) && 动态.预期版本 != 0;
    }
};

}
