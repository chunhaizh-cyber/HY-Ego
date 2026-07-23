// 文件规则：动态服务只承担动态种类和关系 20 角色准入、值式读写合同和退役材料形成。
module;

#include "状态动态类型化记录.数据.h"

export module 海中鱼巣.领域.服务.节点直接动态;

export namespace 海中鱼巣 {

class 节点直接动态服务 final {
public:
    完整动态写入结果 记录状态迁移动能(
        const 完整动态请求& 请求);
    完整动态写入结果 记录动作致变动态(
        const 完整动态请求& 请求);
    完整动态写入结果 记录聚合动态(
        const 完整动态请求& 请求);

    完整动态读取结果 读取完整动态事实(节点句柄 动态) const;
    主体动态事实组读取结果 读取主体动态事实组(节点句柄 主体) const;

    动态退役规格结果 形成动态退役规格(
        节点句柄 动态, std::uint64_t 预期版本) const;

    带值读取结果<动态退役材料> 读取动态退役材料(
        节点句柄 动态) const;

private:
    static bool 动态请求基础可准入(const 完整动态请求& 请求) noexcept {
        const auto 种类值 = static_cast<std::uint32_t>(请求.种类);
        return 句柄有效(请求.主体) && 句柄有效(请求.场景)
            && 句柄有效(请求.被改变目标) && 句柄有效(请求.前状态)
            && 句柄有效(请求.后状态) && 句柄有效(请求.来源存在)
            && 请求.发生时间 != 0 && 种类值 >= 1 && 种类值 <= 5;
    }

    static bool 动态种类角色可准入(const 完整动态请求& 请求) noexcept {
        if (!动态请求基础可准入(请求)) return false;
        if (请求.种类 == 动态种类::聚合动态) {
            return 请求.聚合规则版本 != 0
                && 请求.来源动作 == std::nullopt
                && 请求.同源状态迁移动能 == std::nullopt
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

    static bool 退役规格可准入(
        节点句柄 动态, std::uint64_t 预期版本) noexcept {
        return 句柄有效(动态) && 预期版本 != 0;
    }
};

}
