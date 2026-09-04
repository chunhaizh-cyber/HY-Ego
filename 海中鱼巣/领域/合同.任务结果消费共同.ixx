module;

#include <cstdint>
#include <optional>
#include <vector>

export module 海中鱼巣.领域.合同.任务结果消费共同;

export import 海中鱼巣.领域.合同.任务结果消费身份;
export import 海中鱼巣.领域.服务.L2状态结构;
export import 海中鱼巣.领域.服务.L2任务结构;
export import 海中鱼巣.领域.服务.L2需求结构;

export namespace 海中鱼巣 {

struct L2自我结果争用优先级批次身份 final {
    稳定编码 值;
    explicit L2自我结果争用优先级批次身份(稳定编码 编码 = {}) noexcept
        : 值(编码) {}
    friend bool operator==(const L2自我结果争用优先级批次身份&,
        const L2自我结果争用优先级批次身份&) = default;
};

struct L2自我结果争用成员优先级身份 final {
    稳定编码 值;
    explicit L2自我结果争用成员优先级身份(稳定编码 编码 = {}) noexcept
        : 值(编码) {}
    friend bool operator==(const L2自我结果争用成员优先级身份&,
        const L2自我结果争用成员优先级身份&) = default;
};

enum class L2任务结果消费形态 : std::uint8_t {
    可重复引用 = 1,
    可分割单位 = 2,
    不可分割资格 = 3
};

enum class L2结果消费扣减维度 : std::uint8_t {
    数量 = 1,
    范围 = 2,
    所有权 = 3,
    有效性 = 4
};

struct L2任务结果消费容量合同 final {
    稳定编码 合同身份;
    std::uint32_t 合同版本 = 1;
    L2任务结果消费形态 消费形态 = L2任务结果消费形态::可重复引用;
    std::uint64_t 初始可用单位 = 0;
    std::vector<L2结果消费扣减维度> 扣减维度组;
    稳定编码 范围合同身份;
    稳定编码 所有权合同身份;
    稳定编码 有效性合同身份;
    friend bool operator==(const L2任务结果消费容量合同&,
        const L2任务结果消费容量合同&) = default;
};

struct L2成员消费资格材料 final {
    L2任务来源需求关系事实 成员;
    L2目标状态合同身份 目标合同;
    稳定编码 资格证据身份;
    std::uint32_t 资格规则版本 = 1;
    稳定编码 范围合同身份;
    稳定编码 所有权合同身份;
    稳定编码 有效性合同身份;
    std::uint64_t 最低合法分配单位 = 0;
    std::uint64_t 期望分配单位 = 0;
    friend bool operator==(const L2成员消费资格材料&,
        const L2成员消费资格材料&) = default;
};

inline bool L2成员消费资格材料有效(
    const L2成员消费资格材料& 材料) noexcept {
    return 有效(材料.成员.关系稳定编码) && 有效(材料.成员.任务.值)
        && 有效(材料.成员.来源需求.值) && 材料.成员.来源顺序 != 0
        && L2生命周期完整(材料.成员.生命周期)
        && !材料.成员.生命周期.退出事实代次
        && 有效(材料.目标合同.值) && 有效(材料.资格证据身份)
        && 材料.资格规则版本 != 0;
}

inline bool L2任务结果消费容量合同有效(
    const L2任务结果消费容量合同& 合同) noexcept {
    if (!有效(合同.合同身份) || 合同.合同版本 != 1) return false;
    if (合同.消费形态 == L2任务结果消费形态::可重复引用)
        return 合同.初始可用单位 == 0 && 合同.扣减维度组.empty();
    return 合同.初始可用单位 != 0 && 合同.扣减维度组.size() == 1;
}

}
