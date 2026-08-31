module;

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <map>
#include <mutex>
#include <new>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#define INSTINCT_SERVICE_CONTRACT_FACT_AUTHORITY_NO_INCLUDES

export module 海中鱼巣.领域.服务.服务合同事实权威;

export import 海中鱼巣.领域.服务.L2状态结构;
export import 海中鱼巣.领域.服务.L2任务结构;
import 海中鱼巣.核心.服务.L1事实基座;

export {
#include "服务合同事实权威.数据.h"
}

export namespace 海中鱼巣 {

class 服务合同事实权威服务;

class 服务合同事实权威所有者交付_v1 final {
public:
    服务合同事实权威所有者交付_v1() = delete;
    服务合同事实权威所有者交付_v1(
        const 服务合同事实权威所有者交付_v1&) = delete;
    服务合同事实权威所有者交付_v1& operator=(
        const 服务合同事实权威所有者交付_v1&) = delete;
    服务合同事实权威所有者交付_v1(
        服务合同事实权威所有者交付_v1&&) noexcept = default;
    服务合同事实权威所有者交付_v1& operator=(
        服务合同事实权威所有者交付_v1&&) = delete;

    bool 有效() const noexcept { return 写入端口_.有效(); }
    bool 绑定于(const L1事实基座服务& 服务) const noexcept {
        return 写入端口_.绑定于(服务);
    }
    L1结构所有者身份 所有者身份() const noexcept {
        return 写入端口_.所有者身份();
    }

private:
    explicit 服务合同事实权威所有者交付_v1(
        L1所有者范围写端口&& 端口) noexcept : 写入端口_(std::move(端口)) {}
    friend std::optional<服务合同事实权威所有者交付_v1>
    尝试形成服务合同事实权威所有者交付_v1(
        const L1事实基座服务&, L1所有者范围交付&&) noexcept;
    friend class 服务合同事实权威服务;
    L1所有者范围写端口 写入端口_;
};

std::optional<服务合同事实权威所有者交付_v1>
尝试形成服务合同事实权威所有者交付_v1(
    const L1事实基座服务&, L1所有者范围交付&&) noexcept;

} // namespace 海中鱼巣

namespace 海中鱼巣::服务合同事实权威内部 {

inline constexpr L1所有者范围写入幂等身份 结构登记幂等身份{
    0x494E'5354'5343'5247ULL};
inline constexpr L1所有者范围写入幂等身份 进展扩展登记幂等身份{
    0x494E'5354'5350'5247ULL};
inline constexpr L1所有者范围写入幂等身份 准备扩展登记幂等身份{
    0x494E'5354'5350'5250ULL};
inline constexpr L1所有者范围写入幂等身份 进展v2登记幂等身份{
    0x494E'5354'5350'5632ULL};
inline constexpr L1所有者范围写入幂等身份 准备v2登记幂等身份{
    0x494E'5354'5050'5632ULL};
inline constexpr L1所有者范围写入幂等身份 历史账登记幂等身份{
    0x494E'5354'5348'5631ULL};
inline constexpr L1所有者范围写入幂等身份 历史覆盖登记幂等身份_v2{
    0x494E'5354'5348'5632ULL};
inline constexpr std::uint32_t 服务维护历史覆盖结构登记版本_v2 = 2;

inline constexpr std::uint32_t 登记锚点键 = 0x0307'3001U;
inline constexpr std::uint32_t 当前合同索引根键 = 0x0307'3002U;
inline constexpr std::uint32_t 到期事件索引根键 = 0x0307'3003U;
inline constexpr std::uint32_t 当前合同成员关系键 = 0x0307'3004U;
inline constexpr std::uint32_t 合同当前状态关系键 = 0x0307'3005U;
inline constexpr std::uint32_t 合同自我关系键 = 0x0307'3006U;
inline constexpr std::uint32_t 合同提出者关系键 = 0x0307'3007U;
inline constexpr std::uint32_t 合同需求关系键 = 0x0307'3008U;
inline constexpr std::uint32_t 合同目标宿主关系键 = 0x0307'3009U;
inline constexpr std::uint32_t 合同目标特征关系键 = 0x0307'300AU;
inline constexpr std::uint32_t 合同目标状态合同关系键 = 0x0307'300BU;
inline constexpr std::uint32_t 合同载荷键 = 0x0307'300CU;
inline constexpr std::uint32_t 合同状态载荷键 = 0x0307'300DU;
inline constexpr std::uint32_t 到期事件成员关系键 = 0x0307'300EU;
inline constexpr std::uint32_t 到期事件合同关系键 = 0x0307'300FU;
inline constexpr std::uint32_t 到期事件未满足事实关系键 = 0x0307'3010U;
inline constexpr std::uint32_t 到期事件载荷键 = 0x0307'3011U;
inline constexpr std::uint32_t 登记版本载荷键 = 0x0307'3012U;
inline constexpr std::uint32_t 登记版本值键 = 0x0307'3013U;
inline constexpr std::uint32_t 进展扩展登记锚点键 = 0x0307'3014U;
inline constexpr std::uint32_t 当前服务进展索引根键 = 0x0307'3015U;
inline constexpr std::uint32_t 当前服务进展成员关系键 = 0x0307'3016U;
inline constexpr std::uint32_t 进展服务合同关系键 = 0x0307'3017U;
inline constexpr std::uint32_t 进展自我关系键 = 0x0307'3018U;
inline constexpr std::uint32_t 进展需求关系键 = 0x0307'3019U;
inline constexpr std::uint32_t 进展任务关系键 = 0x0307'301AU;
inline constexpr std::uint32_t 进展方法关系键 = 0x0307'301BU;
inline constexpr std::uint32_t 进展状态引用关系键 = 0x0307'301CU;
inline constexpr std::uint32_t 进展动态引用关系键 = 0x0307'301DU;
inline constexpr std::uint32_t 进展载荷键 = 0x0307'301EU;
inline constexpr std::uint32_t 进展扩展登记版本载荷键 = 0x0307'301FU;
inline constexpr std::uint32_t 进展扩展登记版本值本地键 = 0xF000'301FU;
inline constexpr std::uint32_t 准备扩展登记锚点键 = 0x0307'3020U;
inline constexpr std::uint32_t 当前服务准备索引根键 = 0x0307'3021U;
inline constexpr std::uint32_t 当前服务准备成员关系键 = 0x0307'3022U;
inline constexpr std::uint32_t 准备自我关系键 = 0x0307'3023U;
inline constexpr std::uint32_t 准备来源需求关系键 = 0x0307'3024U;
inline constexpr std::uint32_t 准备来源能力缺口关系键 = 0x0307'3025U;
inline constexpr std::uint32_t 准备任务关系键 = 0x0307'3026U;
inline constexpr std::uint32_t 准备方法关系键 = 0x0307'3027U;
inline constexpr std::uint32_t 准备状态引用关系键 = 0x0307'3028U;
inline constexpr std::uint32_t 准备动态引用关系键 = 0x0307'3029U;
inline constexpr std::uint32_t 准备结果引用关系键 = 0x0307'302AU;
inline constexpr std::uint32_t 准备验证引用关系键 = 0x0307'302BU;
inline constexpr std::uint32_t 准备载荷键 = 0x0307'302CU;
inline constexpr std::uint32_t 准备扩展登记版本载荷键 = 0x0307'302DU;
inline constexpr std::uint32_t 准备扩展登记版本值本地键 = 0xF000'302DU;
inline constexpr std::uint32_t 进展v2登记锚点键 = 0x0307'3030U;
inline constexpr std::uint32_t 进展v2登记版本载荷键 = 0x0307'3031U;
inline constexpr std::uint32_t 进展v2当前索引根键 = 0x0307'3032U;
inline constexpr std::uint32_t 进展v2当前成员关系键 = 0x0307'3033U;
inline constexpr std::uint32_t 进展v2载荷键 = 0x0307'3034U;
inline constexpr std::uint32_t 进展v2服务合同关系键 = 0x0307'3035U;
inline constexpr std::uint32_t 进展v2自我关系键 = 0x0307'3036U;
inline constexpr std::uint32_t 进展v2需求关系键 = 0x0307'3037U;
inline constexpr std::uint32_t 进展v2任务关系键 = 0x0307'3038U;
inline constexpr std::uint32_t 进展v2方法关系键 = 0x0307'3039U;
inline constexpr std::uint32_t 进展v2正式选择关系键 = 0x0307'303AU;
inline constexpr std::uint32_t 进展v2冻结材料关系键 = 0x0307'303BU;
inline constexpr std::uint32_t 进展v2实例方法关系键 = 0x0307'303CU;
inline constexpr std::uint32_t 进展v2状态引用关系键 = 0x0307'303DU;
inline constexpr std::uint32_t 进展v2动态引用关系键 = 0x0307'303EU;
inline constexpr std::uint32_t 进展v2登记版本值本地键 = 0xF000'3031U;
inline constexpr std::uint32_t 准备v2登记锚点键 = 0x0307'3040U;
inline constexpr std::uint32_t 准备v2登记版本载荷键 = 0x0307'3041U;
inline constexpr std::uint32_t 准备v2当前索引根键 = 0x0307'3042U;
inline constexpr std::uint32_t 准备v2当前成员关系键 = 0x0307'3043U;
inline constexpr std::uint32_t 准备v2载荷键 = 0x0307'3044U;
inline constexpr std::uint32_t 准备v2来源关系键 = 0x0307'3045U;
inline constexpr std::uint32_t 准备v2自我关系键 = 0x0307'3046U;
inline constexpr std::uint32_t 准备v2任务关系键 = 0x0307'3047U;
inline constexpr std::uint32_t 准备v2方法关系键 = 0x0307'3048U;
inline constexpr std::uint32_t 准备v2正式选择关系键 = 0x0307'3049U;
inline constexpr std::uint32_t 准备v2冻结材料关系键 = 0x0307'304AU;
inline constexpr std::uint32_t 准备v2实例方法关系键 = 0x0307'304BU;
inline constexpr std::uint32_t 准备v2状态引用关系键 = 0x0307'304CU;
inline constexpr std::uint32_t 准备v2动态引用关系键 = 0x0307'304DU;
inline constexpr std::uint32_t 准备v2结果引用关系键 = 0x0307'304EU;
inline constexpr std::uint32_t 准备v2验证引用关系键 = 0x0307'304FU;
inline constexpr std::uint32_t 准备v2登记版本值本地键 = 0xF000'3041U;
inline constexpr std::uint32_t 历史账登记锚点键 = 0x0307'3050U;
inline constexpr std::uint32_t 历史账登记版本载荷键 = 0x0307'3051U;
inline constexpr std::uint32_t 合同历史索引根键 = 0x0307'3052U;
inline constexpr std::uint32_t 合同历史成员关系键 = 0x0307'3053U;
inline constexpr std::uint32_t 合同状态历史索引根键 = 0x0307'3054U;
inline constexpr std::uint32_t 合同状态历史成员关系键 = 0x0307'3055U;
inline constexpr std::uint32_t 到期事件历史索引根键 = 0x0307'3056U;
inline constexpr std::uint32_t 到期事件历史成员关系键 = 0x0307'3057U;
inline constexpr std::uint32_t 进展v2历史索引根键 = 0x0307'3058U;
inline constexpr std::uint32_t 进展v2历史成员关系键 = 0x0307'3059U;
inline constexpr std::uint32_t 准备v2历史索引根键 = 0x0307'305AU;
inline constexpr std::uint32_t 准备v2历史成员关系键 = 0x0307'305BU;
inline constexpr std::uint32_t 历史账登记版本值本地键 = 0xF000'3051U;
inline constexpr std::uint32_t 历史覆盖登记锚点键_v2 = 0x0307'3060U;
inline constexpr std::uint32_t 历史覆盖登记版本载荷键_v2 = 0x0307'3061U;
inline constexpr std::uint32_t 合同历史索引根键_v2 = 0x0307'3062U;
inline constexpr std::uint32_t 合同历史成员关系键_v2 = 0x0307'3063U;
inline constexpr std::uint32_t 合同状态历史索引根键_v2 = 0x0307'3064U;
inline constexpr std::uint32_t 合同状态历史成员关系键_v2 = 0x0307'3065U;
inline constexpr std::uint32_t 到期事件历史索引根键_v2 = 0x0307'3066U;
inline constexpr std::uint32_t 到期事件历史成员关系键_v2 = 0x0307'3067U;
inline constexpr std::uint32_t 进展历史索引根键_v2 = 0x0307'3068U;
inline constexpr std::uint32_t 进展历史成员关系键_v2 = 0x0307'3069U;
inline constexpr std::uint32_t 准备历史索引根键_v2 = 0x0307'306AU;
inline constexpr std::uint32_t 准备历史成员关系键_v2 = 0x0307'306BU;
inline constexpr std::uint32_t 历史覆盖登记版本值本地键_v2 = 0xF000'3061U;
inline constexpr std::uint32_t 历史覆盖种子关系本地键起点_v2 = 0xE100'0000U;

struct 结构登记定位 final {
    稳定编码 登记锚点{};
    稳定编码 当前合同索引根{};
    稳定编码 到期事件索引根{};
    稳定编码 当前合同成员关系{};
    稳定编码 合同当前状态关系{};
    稳定编码 合同自我关系{};
    稳定编码 合同提出者关系{};
    稳定编码 合同需求关系{};
    稳定编码 合同目标宿主关系{};
    稳定编码 合同目标特征关系{};
    稳定编码 合同目标状态合同关系{};
    稳定编码 合同载荷{};
    稳定编码 合同状态载荷{};
    稳定编码 到期事件成员关系{};
    稳定编码 到期事件合同关系{};
    稳定编码 到期事件未满足事实关系{};
    稳定编码 到期事件载荷{};
    稳定编码 登记版本载荷{};

    std::vector<稳定编码> 全部节点() const {
        return {登记锚点, 当前合同索引根, 到期事件索引根,
            当前合同成员关系, 合同当前状态关系, 合同自我关系,
            合同提出者关系, 合同需求关系, 合同目标宿主关系,
            合同目标特征关系, 合同目标状态合同关系, 合同载荷,
            合同状态载荷, 到期事件成员关系, 到期事件合同关系,
            到期事件未满足事实关系, 到期事件载荷, 登记版本载荷};
    }
};

struct 进展结构登记定位 final {
    稳定编码 登记锚点{};
    稳定编码 当前进展索引根{};
    稳定编码 当前进展成员关系{};
    稳定编码 进展服务合同关系{};
    稳定编码 进展自我关系{};
    稳定编码 进展需求关系{};
    稳定编码 进展任务关系{};
    稳定编码 进展方法关系{};
    稳定编码 进展状态引用关系{};
    稳定编码 进展动态引用关系{};
    稳定编码 进展载荷{};
    稳定编码 登记版本载荷{};

    std::vector<稳定编码> 全部节点() const {
        return {登记锚点, 当前进展索引根, 当前进展成员关系,
            进展服务合同关系, 进展自我关系, 进展需求关系,
            进展任务关系, 进展方法关系, 进展状态引用关系,
            进展动态引用关系, 进展载荷, 登记版本载荷};
    }
};

struct 准备结构登记定位 final {
    稳定编码 登记锚点{}, 当前准备索引根{}, 当前准备成员关系{};
    稳定编码 准备自我关系{}, 准备来源需求关系{}, 准备来源能力缺口关系{};
    稳定编码 准备任务关系{}, 准备方法关系{}, 准备状态引用关系{};
    稳定编码 准备动态引用关系{}, 准备结果引用关系{}, 准备验证引用关系{};
    稳定编码 准备载荷{}, 登记版本载荷{};
    std::vector<稳定编码> 全部节点() const {
        return {登记锚点, 当前准备索引根, 当前准备成员关系,
            准备自我关系, 准备来源需求关系, 准备来源能力缺口关系,
            准备任务关系, 准备方法关系, 准备状态引用关系,
            准备动态引用关系, 准备结果引用关系, 准备验证引用关系,
            准备载荷, 登记版本载荷};
    }
};

struct 进展结构登记定位_v2 final {
    稳定编码 登记锚点{}, 登记版本载荷{}, 当前索引根{}, 当前成员关系{};
    稳定编码 载荷{}, 服务合同关系{}, 自我关系{}, 需求关系{};
    稳定编码 任务关系{}, 方法关系{}, 正式选择关系{}, 冻结材料关系{};
    稳定编码 实例方法关系{}, 状态引用关系{}, 动态引用关系{};
    std::vector<稳定编码> 全部节点() const {
        return {登记锚点, 登记版本载荷, 当前索引根, 当前成员关系,
            载荷, 服务合同关系, 自我关系, 需求关系, 任务关系, 方法关系,
            正式选择关系, 冻结材料关系, 实例方法关系, 状态引用关系,
            动态引用关系};
    }
};

struct 准备结构登记定位_v2 final {
    稳定编码 登记锚点{}, 登记版本载荷{}, 当前索引根{}, 当前成员关系{};
    稳定编码 载荷{}, 来源关系{}, 自我关系{}, 任务关系{}, 方法关系{};
    稳定编码 正式选择关系{}, 冻结材料关系{}, 实例方法关系{};
    稳定编码 状态引用关系{}, 动态引用关系{}, 结果引用关系{}, 验证引用关系{};
    std::vector<稳定编码> 全部节点() const {
        return {登记锚点, 登记版本载荷, 当前索引根, 当前成员关系,
            载荷, 来源关系, 自我关系, 任务关系, 方法关系, 正式选择关系,
            冻结材料关系, 实例方法关系, 状态引用关系, 动态引用关系,
            结果引用关系, 验证引用关系};
    }
};

struct 服务维护历史账登记定位_v1 final {
    稳定编码 登记锚点{}, 登记版本载荷{};
    稳定编码 合同历史索引根{}, 合同历史成员关系{};
    稳定编码 合同状态历史索引根{}, 合同状态历史成员关系{};
    稳定编码 到期事件历史索引根{}, 到期事件历史成员关系{};
    稳定编码 进展v2历史索引根{}, 进展v2历史成员关系{};
    稳定编码 准备v2历史索引根{}, 准备v2历史成员关系{};
    std::uint64_t 登记事实代次 = 0;
    std::vector<稳定编码> 全部节点() const {
        return {登记锚点, 登记版本载荷, 合同历史索引根,
            合同历史成员关系, 合同状态历史索引根,
            合同状态历史成员关系, 到期事件历史索引根,
            到期事件历史成员关系, 进展v2历史索引根,
            进展v2历史成员关系, 准备v2历史索引根,
            准备v2历史成员关系};
    }
};

struct 服务维护历史覆盖登记定位_v2 final {
    稳定编码 登记锚点{}, 登记版本载荷{};
    稳定编码 合同历史索引根{}, 合同历史成员关系{};
    稳定编码 合同状态历史索引根{}, 合同状态历史成员关系{};
    稳定编码 到期事件历史索引根{}, 到期事件历史成员关系{};
    稳定编码 进展历史索引根{}, 进展历史成员关系{};
    稳定编码 准备历史索引根{}, 准备历史成员关系{};
    std::uint64_t 登记事实代次 = 0;
    std::vector<稳定编码> 全部节点() const {
        return {登记锚点, 登记版本载荷, 合同历史索引根,
            合同历史成员关系, 合同状态历史索引根,
            合同状态历史成员关系, 到期事件历史索引根,
            到期事件历史成员关系, 进展历史索引根,
            进展历史成员关系, 准备历史索引根,
            准备历史成员关系};
    }
};

struct 服务维护历史覆盖成员_v2 final {
    服务维护历史事实类别_v1 类别 =
        服务维护历史事实类别_v1::服务合同;
    稳定编码 目标事实节点{};
    稳定编码 事实身份{};
    std::uint64_t 事实版本 = 0;
    std::uint64_t 形成事实代次 = 0;
    friend bool operator==(const 服务维护历史覆盖成员_v2&,
        const 服务维护历史覆盖成员_v2&) = default;
};

inline bool 历史覆盖成员小于_v2(
    const 服务维护历史覆盖成员_v2& a,
    const 服务维护历史覆盖成员_v2& b) noexcept {
    if (a.类别 != b.类别)
        return static_cast<std::uint8_t>(a.类别)
            < static_cast<std::uint8_t>(b.类别);
    if (a.事实身份.值 != b.事实身份.值)
        return a.事实身份.值 < b.事实身份.值;
    if (a.事实版本 != b.事实版本)
        return a.事实版本 < b.事实版本;
    if (a.形成事实代次 != b.形成事实代次)
        return a.形成事实代次 < b.形成事实代次;
    return a.目标事实节点.值 < b.目标事实节点.值;
}

inline L1所有者范围写集本地键 键(std::uint32_t 值) noexcept { return {值}; }

inline std::optional<稳定编码> 查找映射(
    const std::vector<std::pair<L1所有者范围写集本地键, 稳定编码>>& 映射,
    std::uint32_t 本地键) noexcept {
    std::optional<稳定编码> 结果;
    for (const auto& [键值, 编码] : 映射) {
        if (键值.值 != 本地键) continue;
        if (结果) return std::nullopt;
        结果 = 编码;
    }
    return 结果;
}

inline const L1所有者范围一致属性值投影* 查找属性投影(
    const std::vector<L1所有者范围一致属性值读取结果项>& 组,
    稳定编码 节点, 稳定编码 属性类型) noexcept {
    const L1所有者范围一致属性值投影* 结果 = nullptr;
    for (const auto& 项 : 组) {
        if (项.节点 != 节点 || 项.属性类型 != 属性类型) continue;
        if (结果 || 项.状态 != L1所有者范围一致当前读取项目状态::成功
            || !项.投影)
            return nullptr;
        结果 = &*项.投影;
    }
    return 结果;
}

inline const std::vector<std::uint64_t>* U64载荷(
    const L1所有者范围值事实& 值) noexcept {
    return std::get_if<std::vector<std::uint64_t>>(&值.材料);
}

inline bool 活动于(const L1所有者范围节点事实& 事实,
    std::uint64_t 截止) noexcept {
    return 事实.创建事实代次 != 0 && 事实.创建事实代次 <= 截止
        && (!事实.退出事实代次 || *事实.退出事实代次 > 截止);
}

inline bool 活动于(const L1所有者范围关系事实& 事实,
    std::uint64_t 截止) noexcept {
    return 事实.创建事实代次 != 0 && 事实.创建事实代次 <= 截止
        && (!事实.退出事实代次 || *事实.退出事实代次 > 截止);
}

inline bool 活动于(const L1所有者范围值事实& 事实,
    std::uint64_t 截止) noexcept {
    return 事实.创建事实代次 != 0 && 事实.创建事实代次 <= 截止
        && (!事实.退出事实代次 || *事实.退出事实代次 > 截止);
}

inline L1所有者范围写集请求 形成结构登记写集(std::uint64_t 截止) {
    L1所有者范围写集请求 写集;
    写集.期望事实代次 = 截止;
    写集.写入幂等身份 = 结构登记幂等身份;
    const auto 普通 = [&](std::uint32_t 本地键) {
        写集.节点.push_back({键(本地键), 节点种类::普通, std::nullopt});
    };
    const auto 属性 = [&](std::uint32_t 本地键) {
        写集.节点.push_back({键(本地键), 节点种类::属性类型,
            L1所有者范围值表示种类::U64组});
    };
    普通(登记锚点键);
    普通(当前合同索引根键);
    普通(到期事件索引根键);
    普通(当前合同成员关系键);
    普通(合同当前状态关系键);
    普通(合同自我关系键);
    普通(合同提出者关系键);
    普通(合同需求关系键);
    普通(合同目标宿主关系键);
    普通(合同目标特征关系键);
    普通(合同目标状态合同关系键);
    属性(合同载荷键);
    属性(合同状态载荷键);
    普通(到期事件成员关系键);
    普通(到期事件合同关系键);
    普通(到期事件未满足事实关系键);
    属性(到期事件载荷键);
    属性(登记版本载荷键);
    写集.值.push_back({键(登记版本值键), 键(登记锚点键),
        键(登记版本载荷键),
        std::vector<std::uint64_t>{服务合同事实权威结构登记版本_v1},
        键(登记锚点键)});
    写集.属性槽变更.push_back(
        {键(登记锚点键), 键(登记版本载荷键), 键(登记版本值键)});
    return 写集;
}

inline 结构登记定位 从映射形成定位(
    const std::vector<std::pair<L1所有者范围写集本地键, 稳定编码>>& 映射) {
    const auto 取 = [&](std::uint32_t 本地键) -> 稳定编码 {
        const auto 值 = 查找映射(映射, 本地键);
        if (!值) throw std::runtime_error("service contract registration mapping missing");
        return *值;
    };
    return {取(登记锚点键), 取(当前合同索引根键), 取(到期事件索引根键),
        取(当前合同成员关系键), 取(合同当前状态关系键),
        取(合同自我关系键), 取(合同提出者关系键), 取(合同需求关系键),
        取(合同目标宿主关系键), 取(合同目标特征关系键),
        取(合同目标状态合同关系键), 取(合同载荷键),
        取(合同状态载荷键), 取(到期事件成员关系键),
        取(到期事件合同关系键), 取(到期事件未满足事实关系键),
        取(到期事件载荷键), 取(登记版本载荷键)};
}

inline bool 验证登记当前投影(const L1事实基座服务& L1,
    L1结构所有者身份 所有者, const 结构登记定位& 登记,
    std::uint64_t 截止) {
    L1所有者范围一致当前读取请求 请求;
    请求.期望事实代次 = 截止;
    请求.所有者 = {所有者};
    请求.节点 = 登记.全部节点();
    请求.属性值 = {{登记.登记锚点, 登记.登记版本载荷}};
    const auto 读取 = L1.尝试读取所有者范围一致当前投影(请求);
    if (读取.状态 != L1所有者范围一致当前读取状态::成功
        || 读取.读取事实代次 != 截止 || 读取.所有者.size() != 1
        || 读取.所有者.front().状态
            != L1所有者范围一致当前读取项目状态::成功
        || !读取.所有者.front().所有者事实
        || 读取.所有者.front().所有者事实->所有者 != 所有者
        || 读取.节点.size() != 登记.全部节点().size())
        return false;
    for (const auto& 项 : 读取.节点) {
        if (项.状态 != L1所有者范围一致当前读取项目状态::成功
            || !项.事实 || 项.事实->写入所有者 != 所有者
            || !活动于(*项.事实, 截止))
            return false;
    }
    const auto* 版本 = 查找属性投影(
        读取.属性值, 登记.登记锚点, 登记.登记版本载荷);
    if (!版本 || 版本->当前值事实.写入所有者 != 所有者
        || !活动于(版本->当前值事实, 截止))
        return false;
    const auto* 载荷 = U64载荷(版本->当前值事实);
    return 载荷 && *载荷
        == std::vector<std::uint64_t>{服务合同事实权威结构登记版本_v1};
}

inline 结构登记定位 初始化结构登记(
    const L1事实基座服务& L1, L1所有者范围写端口& 写入端口) {
    L1所有者范围写集请求 写集;
    const auto 首次 = 写入端口.读取首次写入材料(
        {L1所有者范围首次写入读取合同版本, 结构登记幂等身份});
    if (首次.状态 == L1所有者范围读取状态::成功) {
        if (!首次.首次规范化写集)
            throw std::runtime_error("service contract registration first write damaged");
        写集 = *首次.首次规范化写集;
    } else if (首次.状态 == L1所有者范围读取状态::未找到) {
        const auto 当前 = L1.读取中性当前事实代次({L1中性CRUD合同版本});
        if (当前.状态 != L1中性读取状态::成功 || 当前.事实代次 == 0)
            throw std::runtime_error("service contract registration cutoff unavailable");
        写集 = 形成结构登记写集(当前.事实代次);
    } else {
        throw std::runtime_error("service contract registration first write unavailable");
    }
    const auto 写入 = 写入端口.提交所有者范围中性写集(写集);
    if ((写入.状态 != L1所有者范围写入状态::成功
            && 写入.状态 != L1所有者范围写入状态::精确重复)
        || 写入.事实代次 == 0)
        throw std::runtime_error("service contract registration failed");
    const auto 登记 = 从映射形成定位(写入.新编码映射);
    const auto 当前 = L1.读取中性当前事实代次({L1中性CRUD合同版本});
    if (当前.状态 != L1中性读取状态::成功 || 当前.事实代次 == 0
        || !验证登记当前投影(
            L1, 写入端口.所有者身份(), 登记, 当前.事实代次))
        throw std::runtime_error("service contract registration readback failed");
    return 登记;
}

inline L1所有者范围写集请求 形成进展扩展登记写集(std::uint64_t 截止) {
    L1所有者范围写集请求 写集;
    写集.期望事实代次 = 截止;
    写集.写入幂等身份 = 进展扩展登记幂等身份;
    const auto 普通 = [&](std::uint32_t 本地键) {
        写集.节点.push_back({键(本地键), 节点种类::普通, std::nullopt});
    };
    const auto 属性 = [&](std::uint32_t 本地键) {
        写集.节点.push_back({键(本地键), 节点种类::属性类型,
            L1所有者范围值表示种类::U64组});
    };
    普通(进展扩展登记锚点键);
    普通(当前服务进展索引根键);
    普通(当前服务进展成员关系键);
    普通(进展服务合同关系键);
    普通(进展自我关系键);
    普通(进展需求关系键);
    普通(进展任务关系键);
    普通(进展方法关系键);
    普通(进展状态引用关系键);
    普通(进展动态引用关系键);
    属性(进展载荷键);
    属性(进展扩展登记版本载荷键);
    写集.值.push_back({键(进展扩展登记版本值本地键),
        键(进展扩展登记锚点键), 键(进展扩展登记版本载荷键),
        std::vector<std::uint64_t>{服务进展事实结构登记版本_v1},
        键(进展扩展登记锚点键)});
    写集.属性槽变更.push_back({键(进展扩展登记锚点键),
        键(进展扩展登记版本载荷键), 键(进展扩展登记版本值本地键)});
    return 写集;
}

inline 进展结构登记定位 从映射形成进展定位(
    const std::vector<std::pair<L1所有者范围写集本地键, 稳定编码>>& 映射) {
    const auto 取 = [&](std::uint32_t 本地键) -> 稳定编码 {
        const auto 值 = 查找映射(映射, 本地键);
        if (!值) throw std::runtime_error("service progress registration mapping missing");
        return *值;
    };
    return {取(进展扩展登记锚点键), 取(当前服务进展索引根键),
        取(当前服务进展成员关系键), 取(进展服务合同关系键),
        取(进展自我关系键), 取(进展需求关系键), 取(进展任务关系键),
        取(进展方法关系键), 取(进展状态引用关系键),
        取(进展动态引用关系键), 取(进展载荷键),
        取(进展扩展登记版本载荷键)};
}

inline bool 验证进展登记当前投影(const L1事实基座服务& L1,
    L1结构所有者身份 所有者, const 进展结构登记定位& 登记,
    std::uint64_t 截止) {
    L1所有者范围一致当前读取请求 请求;
    请求.期望事实代次 = 截止;
    请求.所有者 = {所有者};
    请求.节点 = 登记.全部节点();
    请求.属性值 = {{登记.登记锚点, 登记.登记版本载荷}};
    const auto 读取 = L1.尝试读取所有者范围一致当前投影(请求);
    if (读取.状态 != L1所有者范围一致当前读取状态::成功
        || 读取.读取事实代次 != 截止 || 读取.所有者.size() != 1
        || 读取.所有者.front().状态
            != L1所有者范围一致当前读取项目状态::成功
        || !读取.所有者.front().所有者事实
        || 读取.所有者.front().所有者事实->所有者 != 所有者
        || 读取.节点.size() != 登记.全部节点().size())
        return false;
    for (const auto& 项 : 读取.节点) {
        if (项.状态 != L1所有者范围一致当前读取项目状态::成功
            || !项.事实 || 项.事实->写入所有者 != 所有者
            || !活动于(*项.事实, 截止))
            return false;
    }
    const auto* 版本 = 查找属性投影(
        读取.属性值, 登记.登记锚点, 登记.登记版本载荷);
    if (!版本 || 版本->当前值事实.写入所有者 != 所有者
        || !活动于(版本->当前值事实, 截止))
        return false;
    const auto* 载荷 = U64载荷(版本->当前值事实);
    return 载荷 && *载荷
        == std::vector<std::uint64_t>{服务进展事实结构登记版本_v1};
}

inline 进展结构登记定位 初始化进展结构登记(
    const L1事实基座服务& L1, L1所有者范围写端口& 写入端口) {
    L1所有者范围写集请求 写集;
    const auto 首次 = 写入端口.读取首次写入材料(
        {L1所有者范围首次写入读取合同版本, 进展扩展登记幂等身份});
    if (首次.状态 == L1所有者范围读取状态::成功) {
        if (!首次.首次规范化写集)
            throw std::runtime_error("service progress registration first write damaged");
        写集 = *首次.首次规范化写集;
    } else if (首次.状态 == L1所有者范围读取状态::未找到) {
        const auto 当前 = L1.读取中性当前事实代次({L1中性CRUD合同版本});
        if (当前.状态 != L1中性读取状态::成功 || 当前.事实代次 == 0)
            throw std::runtime_error("service progress registration cutoff unavailable");
        写集 = 形成进展扩展登记写集(当前.事实代次);
    } else {
        throw std::runtime_error("service progress registration first write unavailable");
    }
    const auto 写入 = 写入端口.提交所有者范围中性写集(写集);
    if ((写入.状态 != L1所有者范围写入状态::成功
            && 写入.状态 != L1所有者范围写入状态::精确重复)
        || 写入.事实代次 == 0)
        throw std::runtime_error("service progress registration failed");
    const auto 登记 = 从映射形成进展定位(写入.新编码映射);
    const auto 当前 = L1.读取中性当前事实代次({L1中性CRUD合同版本});
    if (当前.状态 != L1中性读取状态::成功 || 当前.事实代次 == 0
        || !验证进展登记当前投影(
            L1, 写入端口.所有者身份(), 登记, 当前.事实代次))
        throw std::runtime_error("service progress registration readback failed");
    return 登记;
}

inline L1所有者范围写集请求 形成准备扩展登记写集(std::uint64_t 截止) {
    L1所有者范围写集请求 写集; 写集.期望事实代次 = 截止;
    写集.写入幂等身份 = 准备扩展登记幂等身份;
    const auto 普通 = [&](std::uint32_t k) { 写集.节点.push_back(
        {键(k), 节点种类::普通, std::nullopt}); };
    const auto 属性 = [&](std::uint32_t k) { 写集.节点.push_back(
        {键(k), 节点种类::属性类型, L1所有者范围值表示种类::U64组}); };
    for (const auto k : {准备扩展登记锚点键, 当前服务准备索引根键,
        当前服务准备成员关系键, 准备自我关系键, 准备来源需求关系键,
        准备来源能力缺口关系键, 准备任务关系键, 准备方法关系键,
        准备状态引用关系键, 准备动态引用关系键, 准备结果引用关系键,
        准备验证引用关系键}) 普通(k);
    属性(准备载荷键); 属性(准备扩展登记版本载荷键);
    写集.值.push_back({键(准备扩展登记版本值本地键), 键(准备扩展登记锚点键),
        键(准备扩展登记版本载荷键),
        std::vector<std::uint64_t>{服务准备事实结构登记版本_v1},
        键(准备扩展登记锚点键)});
    写集.属性槽变更.push_back({键(准备扩展登记锚点键),
        键(准备扩展登记版本载荷键), 键(准备扩展登记版本值本地键)});
    return 写集;
}

inline 准备结构登记定位 从映射形成准备定位(
    const std::vector<std::pair<L1所有者范围写集本地键, 稳定编码>>& 映射) {
    const auto 取 = [&](std::uint32_t k) { const auto v = 查找映射(映射, k);
        if (!v) throw std::runtime_error("service preparation registration mapping missing");
        return *v; };
    return {取(准备扩展登记锚点键), 取(当前服务准备索引根键),
        取(当前服务准备成员关系键), 取(准备自我关系键),
        取(准备来源需求关系键), 取(准备来源能力缺口关系键),
        取(准备任务关系键), 取(准备方法关系键), 取(准备状态引用关系键),
        取(准备动态引用关系键), 取(准备结果引用关系键),
        取(准备验证引用关系键), 取(准备载荷键),
        取(准备扩展登记版本载荷键)};
}

inline bool 验证准备登记当前投影(const L1事实基座服务& L1,
    L1结构所有者身份 所有者, const 准备结构登记定位& 登记,
    std::uint64_t 截止) {
    L1所有者范围一致当前读取请求 请求; 请求.期望事实代次 = 截止;
    请求.所有者 = {所有者}; 请求.节点 = 登记.全部节点();
    请求.属性值 = {{登记.登记锚点, 登记.登记版本载荷}};
    const auto 读 = L1.尝试读取所有者范围一致当前投影(请求);
    if (读.状态 != L1所有者范围一致当前读取状态::成功
        || 读.读取事实代次 != 截止 || 读.节点.size() != 登记.全部节点().size()) return false;
    for (const auto& 项 : 读.节点) if (项.状态
        != L1所有者范围一致当前读取项目状态::成功 || !项.事实
        || 项.事实->写入所有者 != 所有者 || !活动于(*项.事实, 截止)) return false;
    const auto* 版本 = 查找属性投影(读.属性值, 登记.登记锚点, 登记.登记版本载荷);
    const auto* 载荷 = 版本 ? U64载荷(版本->当前值事实) : nullptr;
    return 载荷 && *载荷 == std::vector<std::uint64_t>{服务准备事实结构登记版本_v1};
}

inline 准备结构登记定位 初始化准备结构登记(
    const L1事实基座服务& L1, L1所有者范围写端口& 端口) {
    L1所有者范围写集请求 写集;
    const auto 首次 = 端口.读取首次写入材料(
        {L1所有者范围首次写入读取合同版本, 准备扩展登记幂等身份});
    if (首次.状态 == L1所有者范围读取状态::成功 && 首次.首次规范化写集)
        写集 = *首次.首次规范化写集;
    else if (首次.状态 == L1所有者范围读取状态::未找到) {
        const auto 当前 = L1.读取中性当前事实代次({L1中性CRUD合同版本});
        if (当前.状态 != L1中性读取状态::成功 || 当前.事实代次 == 0)
            throw std::runtime_error("service preparation registration cutoff unavailable");
        写集 = 形成准备扩展登记写集(当前.事实代次);
    } else throw std::runtime_error("service preparation registration first write unavailable");
    const auto 写入 = 端口.提交所有者范围中性写集(写集);
    if ((写入.状态 != L1所有者范围写入状态::成功
        && 写入.状态 != L1所有者范围写入状态::精确重复) || 写入.事实代次 == 0)
        throw std::runtime_error("service preparation registration failed");
    const auto 登记 = 从映射形成准备定位(写入.新编码映射);
    const auto 当前 = L1.读取中性当前事实代次({L1中性CRUD合同版本});
    if (当前.状态 != L1中性读取状态::成功 || !验证准备登记当前投影(
        L1, 端口.所有者身份(), 登记, 当前.事实代次))
        throw std::runtime_error("service preparation registration readback failed");
    return 登记;
}

inline bool 验证v2登记当前投影(const L1事实基座服务& L1,
    L1结构所有者身份 所有者, const std::vector<稳定编码>& 节点组,
    稳定编码 锚点, 稳定编码 版本载荷, std::uint32_t 版本,
    std::uint64_t 截止) {
    L1所有者范围一致当前读取请求 请求;
    请求.期望事实代次 = 截止;
    请求.所有者 = {所有者};
    请求.节点 = 节点组;
    请求.属性值 = {{锚点, 版本载荷}};
    const auto 读 = L1.尝试读取所有者范围一致当前投影(请求);
    if (读.状态 != L1所有者范围一致当前读取状态::成功
        || 读.读取事实代次 != 截止 || 读.所有者.size() != 1
        || 读.节点.size() != 节点组.size()) return false;
    const auto& o = 读.所有者.front();
    if (o.状态 != L1所有者范围一致当前读取项目状态::成功
        || !o.所有者事实 || o.所有者事实->所有者 != 所有者
        || o.所有者事实->退出事实代次) return false;
    for (const auto& n : 读.节点)
        if (n.状态 != L1所有者范围一致当前读取项目状态::成功
            || !n.事实 || n.事实->写入所有者 != 所有者
            || !活动于(*n.事实, 截止)) return false;
    const auto* p = 查找属性投影(读.属性值, 锚点, 版本载荷);
    if (!p || p->当前值事实.写入所有者 != 所有者
        || !活动于(p->当前值事实, 截止)) return false;
    const auto* v = p ? U64载荷(p->当前值事实) : nullptr;
    return v && *v == std::vector<std::uint64_t>{版本};
}

inline L1所有者范围写集请求 形成进展v2登记写集(std::uint64_t 截止) {
    L1所有者范围写集请求 w; w.期望事实代次 = 截止;
    w.写入幂等身份 = 进展v2登记幂等身份;
    for (const auto k : {进展v2登记锚点键, 进展v2当前索引根键,
        进展v2当前成员关系键, 进展v2服务合同关系键, 进展v2自我关系键,
        进展v2需求关系键, 进展v2任务关系键, 进展v2方法关系键,
        进展v2正式选择关系键, 进展v2冻结材料关系键,
        进展v2实例方法关系键, 进展v2状态引用关系键,
        进展v2动态引用关系键})
        w.节点.push_back({键(k), 节点种类::普通, std::nullopt});
    for (const auto k : {进展v2登记版本载荷键, 进展v2载荷键})
        w.节点.push_back({键(k), 节点种类::属性类型,
            L1所有者范围值表示种类::U64组});
    w.值.push_back({键(进展v2登记版本值本地键), 键(进展v2登记锚点键),
        键(进展v2登记版本载荷键),
        std::vector<std::uint64_t>{服务进展事实结构登记版本_v2},
        键(进展v2登记锚点键)});
    w.属性槽变更.push_back({键(进展v2登记锚点键),
        键(进展v2登记版本载荷键), 键(进展v2登记版本值本地键)});
    return w;
}

inline 进展结构登记定位_v2 从映射形成进展v2定位(
    const std::vector<std::pair<L1所有者范围写集本地键, 稳定编码>>& m) {
    const auto 取 = [&](std::uint32_t k) { const auto v = 查找映射(m, k);
        if (!v) throw std::runtime_error("service progress v2 mapping missing"); return *v; };
    return {取(进展v2登记锚点键), 取(进展v2登记版本载荷键),
        取(进展v2当前索引根键), 取(进展v2当前成员关系键),
        取(进展v2载荷键), 取(进展v2服务合同关系键), 取(进展v2自我关系键),
        取(进展v2需求关系键), 取(进展v2任务关系键), 取(进展v2方法关系键),
        取(进展v2正式选择关系键), 取(进展v2冻结材料关系键),
        取(进展v2实例方法关系键), 取(进展v2状态引用关系键),
        取(进展v2动态引用关系键)};
}

inline 进展结构登记定位_v2 初始化进展v2结构登记(
    const L1事实基座服务& L1, L1所有者范围写端口& 端口) {
    L1所有者范围写集请求 w;
    const auto 首次 = 端口.读取首次写入材料(
        {L1所有者范围首次写入读取合同版本, 进展v2登记幂等身份});
    if (首次.状态 == L1所有者范围读取状态::成功 && 首次.首次规范化写集)
        w = *首次.首次规范化写集;
    else if (首次.状态 == L1所有者范围读取状态::未找到) {
        const auto 当前 = L1.读取中性当前事实代次({L1中性CRUD合同版本});
        if (当前.状态 != L1中性读取状态::成功 || 当前.事实代次 == 0)
            throw std::runtime_error("service progress v2 cutoff unavailable");
        w = 形成进展v2登记写集(当前.事实代次);
    } else throw std::runtime_error("service progress v2 first write unavailable");
    const auto 写 = 端口.提交所有者范围中性写集(w);
    if ((写.状态 != L1所有者范围写入状态::成功
        && 写.状态 != L1所有者范围写入状态::精确重复) || 写.事实代次 == 0)
        throw std::runtime_error("service progress v2 registration failed");
    const auto d = 从映射形成进展v2定位(写.新编码映射);
    const auto 当前 = L1.读取中性当前事实代次({L1中性CRUD合同版本});
    if (当前.状态 != L1中性读取状态::成功 || !验证v2登记当前投影(
        L1, 端口.所有者身份(), d.全部节点(), d.登记锚点,
        d.登记版本载荷, 服务进展事实结构登记版本_v2, 当前.事实代次))
        throw std::runtime_error("service progress v2 registration readback failed");
    return d;
}

inline L1所有者范围写集请求 形成准备v2登记写集(std::uint64_t 截止) {
    L1所有者范围写集请求 w; w.期望事实代次 = 截止;
    w.写入幂等身份 = 准备v2登记幂等身份;
    for (const auto k : {准备v2登记锚点键, 准备v2当前索引根键,
        准备v2当前成员关系键, 准备v2来源关系键, 准备v2自我关系键,
        准备v2任务关系键, 准备v2方法关系键, 准备v2正式选择关系键,
        准备v2冻结材料关系键, 准备v2实例方法关系键,
        准备v2状态引用关系键, 准备v2动态引用关系键,
        准备v2结果引用关系键, 准备v2验证引用关系键})
        w.节点.push_back({键(k), 节点种类::普通, std::nullopt});
    for (const auto k : {准备v2登记版本载荷键, 准备v2载荷键})
        w.节点.push_back({键(k), 节点种类::属性类型,
            L1所有者范围值表示种类::U64组});
    w.值.push_back({键(准备v2登记版本值本地键), 键(准备v2登记锚点键),
        键(准备v2登记版本载荷键),
        std::vector<std::uint64_t>{服务准备事实结构登记版本_v2},
        键(准备v2登记锚点键)});
    w.属性槽变更.push_back({键(准备v2登记锚点键),
        键(准备v2登记版本载荷键), 键(准备v2登记版本值本地键)});
    return w;
}

inline 准备结构登记定位_v2 从映射形成准备v2定位(
    const std::vector<std::pair<L1所有者范围写集本地键, 稳定编码>>& m) {
    const auto 取 = [&](std::uint32_t k) { const auto v = 查找映射(m, k);
        if (!v) throw std::runtime_error("service preparation v2 mapping missing"); return *v; };
    return {取(准备v2登记锚点键), 取(准备v2登记版本载荷键),
        取(准备v2当前索引根键), 取(准备v2当前成员关系键),
        取(准备v2载荷键), 取(准备v2来源关系键), 取(准备v2自我关系键),
        取(准备v2任务关系键), 取(准备v2方法关系键),
        取(准备v2正式选择关系键), 取(准备v2冻结材料关系键),
        取(准备v2实例方法关系键), 取(准备v2状态引用关系键),
        取(准备v2动态引用关系键), 取(准备v2结果引用关系键),
        取(准备v2验证引用关系键)};
}

inline 准备结构登记定位_v2 初始化准备v2结构登记(
    const L1事实基座服务& L1, L1所有者范围写端口& 端口) {
    L1所有者范围写集请求 w;
    const auto 首次 = 端口.读取首次写入材料(
        {L1所有者范围首次写入读取合同版本, 准备v2登记幂等身份});
    if (首次.状态 == L1所有者范围读取状态::成功 && 首次.首次规范化写集)
        w = *首次.首次规范化写集;
    else if (首次.状态 == L1所有者范围读取状态::未找到) {
        const auto 当前 = L1.读取中性当前事实代次({L1中性CRUD合同版本});
        if (当前.状态 != L1中性读取状态::成功 || 当前.事实代次 == 0)
            throw std::runtime_error("service preparation v2 cutoff unavailable");
        w = 形成准备v2登记写集(当前.事实代次);
    } else throw std::runtime_error("service preparation v2 first write unavailable");
    const auto 写 = 端口.提交所有者范围中性写集(w);
    if ((写.状态 != L1所有者范围写入状态::成功
        && 写.状态 != L1所有者范围写入状态::精确重复) || 写.事实代次 == 0)
        throw std::runtime_error("service preparation v2 registration failed");
    const auto d = 从映射形成准备v2定位(写.新编码映射);
    const auto 当前 = L1.读取中性当前事实代次({L1中性CRUD合同版本});
    if (当前.状态 != L1中性读取状态::成功 || !验证v2登记当前投影(
        L1, 端口.所有者身份(), d.全部节点(), d.登记锚点,
        d.登记版本载荷, 服务准备事实结构登记版本_v2, 当前.事实代次))
        throw std::runtime_error("service preparation v2 registration readback failed");
    return d;
}

inline L1所有者范围写集请求 形成历史账登记写集(std::uint64_t 截止) {
    L1所有者范围写集请求 w;
    w.期望事实代次 = 截止;
    w.写入幂等身份 = 历史账登记幂等身份;
    for (std::uint32_t k = 历史账登记锚点键;
        k <= 准备v2历史成员关系键; ++k) {
        if (k == 历史账登记版本载荷键)
            w.节点.push_back({键(k), 节点种类::属性类型,
                L1所有者范围值表示种类::U64组});
        else
            w.节点.push_back({键(k), 节点种类::普通, std::nullopt});
    }
    w.值.push_back({键(历史账登记版本值本地键), 键(历史账登记锚点键),
        键(历史账登记版本载荷键),
        std::vector<std::uint64_t>{服务维护历史事实账结构登记版本_v1},
        键(历史账登记锚点键)});
    w.属性槽变更.push_back({键(历史账登记锚点键),
        键(历史账登记版本载荷键), 键(历史账登记版本值本地键)});
    return w;
}

inline 服务维护历史账登记定位_v1 从映射形成历史账定位(
    const std::vector<std::pair<L1所有者范围写集本地键, 稳定编码>>& m,
    std::uint64_t 登记事实代次) {
    const auto 取 = [&](std::uint32_t k) {
        const auto v = 查找映射(m, k);
        if (!v) throw std::runtime_error("service history ledger mapping missing");
        return *v;
    };
    return {取(历史账登记锚点键), 取(历史账登记版本载荷键),
        取(合同历史索引根键), 取(合同历史成员关系键),
        取(合同状态历史索引根键), 取(合同状态历史成员关系键),
        取(到期事件历史索引根键), 取(到期事件历史成员关系键),
        取(进展v2历史索引根键), 取(进展v2历史成员关系键),
        取(准备v2历史索引根键), 取(准备v2历史成员关系键),
        登记事实代次};
}

inline 服务维护历史账登记定位_v1 初始化历史账结构登记(
    const L1事实基座服务& L1, L1所有者范围写端口& 端口) {
    L1所有者范围写集请求 w;
    auto 首次 = 端口.读取首次写入材料(
        {L1所有者范围首次写入读取合同版本, 历史账登记幂等身份});
    if (首次.状态 == L1所有者范围读取状态::成功
        && 首次.首次规范化写集 && 首次.首次写入结果)
        w = *首次.首次规范化写集;
    else if (首次.状态 == L1所有者范围读取状态::未找到) {
        const auto 当前 = L1.读取中性当前事实代次({L1中性CRUD合同版本});
        if (当前.状态 != L1中性读取状态::成功 || 当前.事实代次 == 0)
            throw std::runtime_error("service history ledger cutoff unavailable");
        w = 形成历史账登记写集(当前.事实代次);
    } else throw std::runtime_error("service history ledger first write unavailable");
    const auto 写 = 端口.提交所有者范围中性写集(w);
    if ((写.状态 != L1所有者范围写入状态::成功
        && 写.状态 != L1所有者范围写入状态::精确重复)
        || 写.事实代次 == 0)
        throw std::runtime_error("service history ledger registration failed");
    首次 = 端口.读取首次写入材料(
        {L1所有者范围首次写入读取合同版本, 历史账登记幂等身份});
    if (首次.状态 != L1所有者范围读取状态::成功
        || !首次.首次规范化写集 || !首次.首次写入结果
        || 首次.首次写入结果->事实代次 == 0
        || *首次.首次规范化写集 != 形成历史账登记写集(
            首次.首次规范化写集->期望事实代次))
        throw std::runtime_error("service history ledger first write drift");
    const auto d = 从映射形成历史账定位(
        首次.首次写入结果->新编码映射,
        首次.首次写入结果->事实代次);
    const auto 当前 = L1.读取中性当前事实代次({L1中性CRUD合同版本});
    if (当前.状态 != L1中性读取状态::成功 || !验证v2登记当前投影(
        L1, 端口.所有者身份(), d.全部节点(), d.登记锚点,
        d.登记版本载荷, 服务维护历史事实账结构登记版本_v1,
        当前.事实代次))
        throw std::runtime_error("service history ledger registration readback failed");
    return d;
}

inline std::vector<std::uint64_t> 编码合同(const 服务合同事实_v1& 事实) {
    return {事实.自我.值.值, 事实.提出者.值.值, 事实.需求.值.值,
        事实.来源提出事件.值, 事实.目标宿主.值.值,
        事实.目标特征.值.值, 事实.目标状态合同.值.值,
        事实.服务范围.值, 事实.提出运行代次, 事实.时间纪元身份.值,
        static_cast<std::uint64_t>(事实.提出完整秒边界),
        static_cast<std::uint64_t>(事实.有效开始完整秒边界),
        static_cast<std::uint64_t>(事实.冻结有效总秒),
        static_cast<std::uint64_t>(事实.冻结预算),
        static_cast<std::uint64_t>(事实.预支值),
        static_cast<std::uint64_t>(事实.完工余款), 事实.需求版本,
        事实.法规准入见证.值, 事实.法规规则版本, 事实.合同代次,
        事实.合同规则版本};
}

inline std::vector<std::uint64_t> 编码状态(const 服务合同状态事实_v1& 事实) {
    return {static_cast<std::uint64_t>(事实.状态),
        static_cast<std::uint64_t>(事实.已消费有效秒),
        static_cast<std::uint64_t>(事实.最后已结算完整秒边界),
        事实.状态版本, 事实.来源事实.值};
}

inline std::vector<std::uint64_t> 编码事件(
    const 服务到期未满足事件事实_v1& 事实) {
    return {事实.合同.值.值, 事实.需求.值.值, 事实.提出者.值.值,
        static_cast<std::uint64_t>(事实.冻结有效总秒),
        static_cast<std::uint64_t>(事实.实际已消费有效秒),
        static_cast<std::uint64_t>(事实.到期完整秒边界),
        事实.目标未满足事实.值, 事实.合同终态版本, 事实.规则版本};
}

inline std::vector<std::uint64_t> 编码进展(
    const 服务合同关联进展事实_v1& 事实) {
    return {事实.自我.值.值, 事实.服务合同.值.值, 事实.需求.值.值,
        事实.任务.值.值, 事实.方法.值.值, 事实.T到D关系稳定编码.值,
        事实.执行冻结见证.值.值, 事实.方法执行见证.值.值,
        事实.安全门禁见证.值.值,
        事实.进展状态 ? 事实.进展状态->值.值 : 0,
        事实.进展动态 ? 事实.进展动态->值.值 : 0,
        static_cast<std::uint64_t>(事实.运行状态), 事实.运行代次,
        static_cast<std::uint64_t>(事实.进展发生完整秒边界),
        static_cast<std::uint64_t>(事实.计量窗口开始完整秒边界),
        static_cast<std::uint64_t>(事实.计量窗口结束完整秒边界),
        事实.方法内容版本, 事实.方法规格版本, 事实.方法生命周期版本,
        事实.进展规则版本,
        事实.生命周期.退出事实代次 ? *事实.生命周期.退出事实代次 : 0};
}

inline std::vector<std::uint64_t> 编码准备(const 服务准备当前事实_v1& f) {
    const bool d = std::holds_alternative<L2需求身份>(f.来源);
    const auto s = d ? std::get<L2需求身份>(f.来源).值.值
        : std::get<服务能力缺口身份_v1>(f.来源).值.值;
    return {d ? 1ULL : 2ULL, s, f.自我.值.值, f.准备目标.值.值,
        f.适用服务范围.值, static_cast<std::uint64_t>(f.有效开始完整秒边界),
        static_cast<std::uint64_t>(f.有效结束完整秒边界), f.任务.值.值,
        f.方法.值.值, f.T到D关系稳定编码 ? f.T到D关系稳定编码->值 : 0,
        f.执行冻结见证.值.值, f.方法执行见证.值.值, f.安全门禁见证.值.值,
        f.进展状态 ? f.进展状态->值.值 : 0, f.进展动态 ? f.进展动态->值.值 : 0,
        f.准备结果 ? f.准备结果->值.值 : 0, f.完成验证 ? f.完成验证->值.值 : 0,
        static_cast<std::uint64_t>(f.运行状态), f.运行代次,
        static_cast<std::uint64_t>(f.进展发生完整秒边界),
        static_cast<std::uint64_t>(f.计量窗口开始完整秒边界),
        static_cast<std::uint64_t>(f.计量窗口结束完整秒边界),
        f.方法内容版本, f.方法规格版本, f.方法生命周期版本,
        f.准备规则版本, f.生命周期.退出事实代次 ? *f.生命周期.退出事实代次 : 0};
}

inline bool 解码合同(const L1所有者范围值事实& 值, 稳定编码 合同节点,
    服务合同事实_v1& 事实) noexcept {
    const auto* 载荷 = U64载荷(值);
    if (!载荷 || 载荷->size() != 21 || 值.所属节点 != 合同节点
        || 值.创建事实代次 == 0)
        return false;
    for (std::size_t i = 10; i <= 15; ++i)
        if ((*载荷)[i] > static_cast<std::uint64_t>(INT64_MAX)) return false;
    事实.身份 = {合同节点};
    事实.自我 = L2存在身份{稳定编码{(*载荷)[0]}};
    事实.提出者 = L2存在身份{稳定编码{(*载荷)[1]}};
    事实.需求 = L2需求身份{稳定编码{(*载荷)[2]}};
    事实.来源提出事件 = {(*载荷)[3]};
    事实.目标宿主 = L2存在身份{稳定编码{(*载荷)[4]}};
    事实.目标特征 = L2特征定义身份{稳定编码{(*载荷)[5]}};
    事实.目标状态合同 = L2目标状态合同身份{稳定编码{(*载荷)[6]}};
    事实.服务范围 = {(*载荷)[7]};
    事实.提出运行代次 = (*载荷)[8];
    事实.时间纪元身份 = {(*载荷)[9]};
    事实.提出完整秒边界 = static_cast<std::int64_t>((*载荷)[10]);
    事实.有效开始完整秒边界 = static_cast<std::int64_t>((*载荷)[11]);
    事实.冻结有效总秒 = static_cast<std::int64_t>((*载荷)[12]);
    事实.冻结预算 = static_cast<std::int64_t>((*载荷)[13]);
    事实.预支值 = static_cast<std::int64_t>((*载荷)[14]);
    事实.完工余款 = static_cast<std::int64_t>((*载荷)[15]);
    事实.需求版本 = (*载荷)[16];
    事实.法规准入见证 = {(*载荷)[17]};
    if ((*载荷)[18] > UINT32_MAX || (*载荷)[20] > UINT32_MAX) return false;
    事实.法规规则版本 = static_cast<std::uint32_t>((*载荷)[18]);
    事实.合同代次 = (*载荷)[19];
    事实.合同规则版本 = static_cast<std::uint32_t>((*载荷)[20]);
    事实.形成事实代次 = 值.创建事实代次;
    return 服务合同事实完整_v1(事实, 值.创建事实代次);
}

inline bool 解码状态(const L1所有者范围值事实& 值, 服务合同身份_v1 合同,
    服务合同状态事实_v1& 事实) noexcept {
    const auto* 载荷 = U64载荷(值);
    if (!载荷 || 载荷->size() != 5 || (*载荷)[0] > UINT8_MAX
        || (*载荷)[1] > static_cast<std::uint64_t>(INT64_MAX)
        || (*载荷)[2] > static_cast<std::uint64_t>(INT64_MAX))
        return false;
    事实.合同 = 合同;
    事实.状态 = static_cast<服务合同当前状态_v1>((*载荷)[0]);
    事实.已消费有效秒 = static_cast<std::int64_t>((*载荷)[1]);
    事实.最后已结算完整秒边界 = static_cast<std::int64_t>((*载荷)[2]);
    事实.状态版本 = (*载荷)[3];
    事实.来源事实 = {(*载荷)[4]};
    事实.形成事实代次 = 值.创建事实代次;
    return 服务合同状态事实完整_v1(事实, 值.创建事实代次);
}

inline bool 解码事件(const L1所有者范围值事实& 值, 稳定编码 事件节点,
    服务到期未满足事件事实_v1& 事实) noexcept {
    const auto* 载荷 = U64载荷(值);
    if (!载荷 || 载荷->size() != 9 || (*载荷)[3] > static_cast<std::uint64_t>(INT64_MAX)
        || (*载荷)[4] > static_cast<std::uint64_t>(INT64_MAX)
        || (*载荷)[5] > static_cast<std::uint64_t>(INT64_MAX)
        || (*载荷)[8] > UINT32_MAX)
        return false;
    事实.身份 = {事件节点};
    事实.合同 = {{(*载荷)[0]}};
    事实.需求 = L2需求身份{稳定编码{(*载荷)[1]}};
    事实.提出者 = L2存在身份{稳定编码{(*载荷)[2]}};
    事实.冻结有效总秒 = static_cast<std::int64_t>((*载荷)[3]);
    事实.实际已消费有效秒 = static_cast<std::int64_t>((*载荷)[4]);
    事实.到期完整秒边界 = static_cast<std::int64_t>((*载荷)[5]);
    事实.目标未满足事实 = {(*载荷)[6]};
    事实.合同终态版本 = (*载荷)[7];
    事实.规则版本 = static_cast<std::uint32_t>((*载荷)[8]);
    事实.形成事实代次 = 值.创建事实代次;
    return 服务到期未满足事件事实完整_v1(事实, 值.创建事实代次);
}

inline bool 解码进展(const L1所有者范围值事实& 值, 稳定编码 进展节点,
    服务合同关联进展事实_v1& 事实) noexcept {
    const auto* 载荷 = U64载荷(值);
    if (!载荷 || 载荷->size() != 21 || (*载荷)[11] > UINT8_MAX
        || (*载荷)[13] > static_cast<std::uint64_t>(INT64_MAX)
        || (*载荷)[14] > static_cast<std::uint64_t>(INT64_MAX)
        || (*载荷)[15] > static_cast<std::uint64_t>(INT64_MAX)
        || (*载荷)[19] > UINT32_MAX)
        return false;
    事实.身份 = {进展节点};
    事实.自我 = L2存在身份{稳定编码{(*载荷)[0]}};
    事实.服务合同 = {{(*载荷)[1]}};
    事实.需求 = L2需求身份{稳定编码{(*载荷)[2]}};
    事实.任务 = L2任务身份{稳定编码{(*载荷)[3]}};
    事实.方法 = L2方法身份{稳定编码{(*载荷)[4]}};
    事实.T到D关系稳定编码 = {(*载荷)[5]};
    事实.执行冻结见证 = {{(*载荷)[6]}};
    事实.方法执行见证 = {{(*载荷)[7]}};
    事实.安全门禁见证 = {{(*载荷)[8]}};
    if ((*载荷)[9] != 0)
        事实.进展状态 = L2状态身份{稳定编码{(*载荷)[9]}};
    if ((*载荷)[10] != 0)
        事实.进展动态 = L2动态身份{稳定编码{(*载荷)[10]}};
    事实.运行状态 = static_cast<服务进展运行状态_v1>((*载荷)[11]);
    事实.运行代次 = (*载荷)[12];
    事实.进展发生完整秒边界 = static_cast<std::int64_t>((*载荷)[13]);
    事实.计量窗口开始完整秒边界 = static_cast<std::int64_t>((*载荷)[14]);
    事实.计量窗口结束完整秒边界 = static_cast<std::int64_t>((*载荷)[15]);
    事实.方法内容版本 = (*载荷)[16];
    事实.方法规格版本 = (*载荷)[17];
    事实.方法生命周期版本 = (*载荷)[18];
    事实.进展规则版本 = static_cast<std::uint32_t>((*载荷)[19]);
    事实.形成事实代次 = 值.创建事实代次;
    事实.生命周期.创建事实代次 = 值.创建事实代次;
    if ((*载荷)[20] != 0) 事实.生命周期.退出事实代次 = (*载荷)[20];
    return 服务合同关联进展事实完整_v1(事实, 值.创建事实代次);
}

inline bool 解码准备(const L1所有者范围值事实& 值, 稳定编码 节点,
    服务准备当前事实_v1& f) noexcept {
    const auto* p = U64载荷(值);
    if (!p || 值.所属节点 != 节点 || 值.创建事实代次 == 0
        || p->size() != 27 || ((*p)[0] != 1 && (*p)[0] != 2)
        || (*p)[17] > UINT8_MAX || (*p)[25] > UINT32_MAX) return false;
    for (const auto i : {5U, 6U, 19U, 20U, 21U})
        if ((*p)[i] > static_cast<std::uint64_t>(INT64_MAX)) return false;
    f.身份 = {节点}; f.自我 = L2存在身份{{(*p)[2]}};
    f.来源 = (*p)[0] == 1 ? 服务准备来源身份_v1{L2需求身份{{(*p)[1]}}}
        : 服务准备来源身份_v1{服务能力缺口身份_v1{{(*p)[1]}}};
    f.准备目标 = {{(*p)[3]}}; f.适用服务范围 = {(*p)[4]};
    f.有效开始完整秒边界 = static_cast<std::int64_t>((*p)[5]);
    f.有效结束完整秒边界 = static_cast<std::int64_t>((*p)[6]);
    f.任务 = L2任务身份{{(*p)[7]}}; f.方法 = L2方法身份{{(*p)[8]}};
    if ((*p)[9]) f.T到D关系稳定编码 = 稳定编码{(*p)[9]};
    f.执行冻结见证 = {{{(*p)[10]}}}; f.方法执行见证 = {{{(*p)[11]}}};
    f.安全门禁见证 = {{{(*p)[12]}}};
    if ((*p)[13]) f.进展状态 = L2状态身份{{(*p)[13]}};
    if ((*p)[14]) f.进展动态 = L2动态身份{{(*p)[14]}};
    if ((*p)[15]) f.准备结果 = 服务准备结果身份_v1{{(*p)[15]}};
    if ((*p)[16]) f.完成验证 = 服务准备验证事实身份_v1{{(*p)[16]}};
    f.运行状态 = static_cast<服务准备运行状态_v1>((*p)[17]); f.运行代次 = (*p)[18];
    f.进展发生完整秒边界 = static_cast<std::int64_t>((*p)[19]);
    f.计量窗口开始完整秒边界 = static_cast<std::int64_t>((*p)[20]);
    f.计量窗口结束完整秒边界 = static_cast<std::int64_t>((*p)[21]);
    f.方法内容版本 = (*p)[22]; f.方法规格版本 = (*p)[23];
    f.方法生命周期版本 = (*p)[24]; f.准备规则版本 = static_cast<std::uint32_t>((*p)[25]);
    f.形成事实代次 = 值.创建事实代次;
    f.生命周期 = {值.创建事实代次, (*p)[26] ? std::optional<std::uint64_t>{(*p)[26]} : std::nullopt};
    return 服务准备当前事实完整_v1(f, 值.创建事实代次);
}

inline std::vector<std::uint64_t> 编码进展_v2(
    const 服务合同关联进展事实_v2& f) {
    return {f.自我.值.值, f.服务合同.值.值, f.需求.值.值,
        f.任务.值.值, f.方法.值.值, f.T到D关系稳定编码.值,
        f.执行绑定.正式选择.值.值, f.执行绑定.执行冻结材料.值.值,
        f.执行绑定.实例方法.值.值, f.执行绑定.筹办轮次,
        f.执行绑定.执行轮次,
        f.进展状态 ? f.进展状态->值.值 : 0,
        f.进展动态 ? f.进展动态->值.值 : 0,
        static_cast<std::uint64_t>(f.运行状态), f.运行代次,
        static_cast<std::uint64_t>(f.进展发生完整秒边界),
        static_cast<std::uint64_t>(f.计量窗口开始完整秒边界),
        static_cast<std::uint64_t>(f.计量窗口结束完整秒边界),
        f.方法内容版本, f.方法规格版本, f.方法生命周期版本,
        f.进展规则版本,
        f.生命周期.退出事实代次 ? *f.生命周期.退出事实代次 : 0};
}

inline bool 解码进展_v2(const L1所有者范围值事实& 值, 稳定编码 节点,
    服务合同关联进展事实_v2& f) noexcept {
    const auto* p = U64载荷(值);
    if (!p || p->size() != 23 || 值.所属节点 != 节点
        || 值.创建事实代次 == 0 || (*p)[13] > UINT8_MAX
        || (*p)[15] > static_cast<std::uint64_t>(INT64_MAX)
        || (*p)[16] > static_cast<std::uint64_t>(INT64_MAX)
        || (*p)[17] > static_cast<std::uint64_t>(INT64_MAX)
        || (*p)[21] > UINT32_MAX) return false;
    f.身份 = {节点}; f.自我 = L2存在身份{{(*p)[0]}};
    f.服务合同 = {{(*p)[1]}}; f.需求 = L2需求身份{{(*p)[2]}};
    f.任务 = L2任务身份{{(*p)[3]}}; f.方法 = L2方法身份{{(*p)[4]}};
    f.T到D关系稳定编码 = {(*p)[5]};
    f.执行绑定.正式选择 = L2任务方法选择记录身份{{(*p)[6]}};
    f.执行绑定.执行冻结材料 = L2任务执行绑定冻结材料身份{{(*p)[7]}};
    f.执行绑定.实例方法 = L2实例方法身份{{(*p)[8]}};
    f.执行绑定.筹办轮次 = (*p)[9]; f.执行绑定.执行轮次 = (*p)[10];
    if ((*p)[11]) f.进展状态 = L2状态身份{{(*p)[11]}};
    if ((*p)[12]) f.进展动态 = L2动态身份{{(*p)[12]}};
    f.运行状态 = static_cast<服务进展运行状态_v1>((*p)[13]);
    f.运行代次 = (*p)[14];
    f.进展发生完整秒边界 = static_cast<std::int64_t>((*p)[15]);
    f.计量窗口开始完整秒边界 = static_cast<std::int64_t>((*p)[16]);
    f.计量窗口结束完整秒边界 = static_cast<std::int64_t>((*p)[17]);
    f.方法内容版本 = (*p)[18]; f.方法规格版本 = (*p)[19];
    f.方法生命周期版本 = (*p)[20];
    f.进展规则版本 = static_cast<std::uint32_t>((*p)[21]);
    f.形成事实代次 = 值.创建事实代次;
    f.生命周期 = {值.创建事实代次,
        (*p)[22] ? std::optional<std::uint64_t>{(*p)[22]} : std::nullopt};
    return 服务合同关联进展事实完整_v2(f, 值.创建事实代次);
}

inline std::vector<std::uint64_t> 编码准备_v2(const 服务准备当前事实_v2& f) {
    const bool d = std::holds_alternative<L2需求身份>(f.来源);
    const auto s = d ? std::get<L2需求身份>(f.来源).值.值
        : std::get<服务能力缺口身份_v1>(f.来源).值.值;
    return {d ? 1ULL : 2ULL, s, f.自我.值.值, f.准备目标.值.值,
        f.适用服务范围.值, static_cast<std::uint64_t>(f.有效开始完整秒边界),
        static_cast<std::uint64_t>(f.有效结束完整秒边界), f.任务.值.值,
        f.方法.值.值, f.T到D关系稳定编码 ? f.T到D关系稳定编码->值 : 0,
        f.执行绑定.正式选择.值.值, f.执行绑定.执行冻结材料.值.值,
        f.执行绑定.实例方法.值.值, f.执行绑定.筹办轮次,
        f.执行绑定.执行轮次,
        f.进展状态 ? f.进展状态->值.值 : 0,
        f.进展动态 ? f.进展动态->值.值 : 0,
        f.准备结果 ? f.准备结果->值.值 : 0,
        f.完成验证 ? f.完成验证->值.值 : 0,
        static_cast<std::uint64_t>(f.运行状态), f.运行代次,
        static_cast<std::uint64_t>(f.进展发生完整秒边界),
        static_cast<std::uint64_t>(f.计量窗口开始完整秒边界),
        static_cast<std::uint64_t>(f.计量窗口结束完整秒边界),
        f.方法内容版本, f.方法规格版本, f.方法生命周期版本,
        f.准备规则版本,
        f.生命周期.退出事实代次 ? *f.生命周期.退出事实代次 : 0};
}

inline bool 解码准备_v2(const L1所有者范围值事实& 值, 稳定编码 节点,
    服务准备当前事实_v2& f) noexcept {
    const auto* p = U64载荷(值);
    if (!p || p->size() != 29 || 值.所属节点 != 节点
        || 值.创建事实代次 == 0 || ((*p)[0] != 1 && (*p)[0] != 2)
        || (*p)[19] > UINT8_MAX || (*p)[27] > UINT32_MAX) return false;
    for (const auto i : {5U, 6U, 21U, 22U, 23U})
        if ((*p)[i] > static_cast<std::uint64_t>(INT64_MAX)) return false;
    f.身份 = {节点}; f.自我 = L2存在身份{{(*p)[2]}};
    f.来源 = (*p)[0] == 1 ? 服务准备来源身份_v1{L2需求身份{{(*p)[1]}}}
        : 服务准备来源身份_v1{服务能力缺口身份_v1{{(*p)[1]}}};
    f.准备目标 = {{(*p)[3]}}; f.适用服务范围 = {(*p)[4]};
    f.有效开始完整秒边界 = static_cast<std::int64_t>((*p)[5]);
    f.有效结束完整秒边界 = static_cast<std::int64_t>((*p)[6]);
    f.任务 = L2任务身份{{(*p)[7]}}; f.方法 = L2方法身份{{(*p)[8]}};
    if ((*p)[9]) f.T到D关系稳定编码 = 稳定编码{(*p)[9]};
    f.执行绑定.正式选择 = L2任务方法选择记录身份{{(*p)[10]}};
    f.执行绑定.执行冻结材料 = L2任务执行绑定冻结材料身份{{(*p)[11]}};
    f.执行绑定.实例方法 = L2实例方法身份{{(*p)[12]}};
    f.执行绑定.筹办轮次 = (*p)[13]; f.执行绑定.执行轮次 = (*p)[14];
    if ((*p)[15]) f.进展状态 = L2状态身份{{(*p)[15]}};
    if ((*p)[16]) f.进展动态 = L2动态身份{{(*p)[16]}};
    if ((*p)[17]) f.准备结果 = 服务准备结果身份_v1{{(*p)[17]}};
    if ((*p)[18]) f.完成验证 = 服务准备验证事实身份_v1{{(*p)[18]}};
    f.运行状态 = static_cast<服务准备运行状态_v1>((*p)[19]);
    f.运行代次 = (*p)[20];
    f.进展发生完整秒边界 = static_cast<std::int64_t>((*p)[21]);
    f.计量窗口开始完整秒边界 = static_cast<std::int64_t>((*p)[22]);
    f.计量窗口结束完整秒边界 = static_cast<std::int64_t>((*p)[23]);
    f.方法内容版本 = (*p)[24]; f.方法规格版本 = (*p)[25];
    f.方法生命周期版本 = (*p)[26];
    f.准备规则版本 = static_cast<std::uint32_t>((*p)[27]);
    f.形成事实代次 = 值.创建事实代次;
    f.生命周期 = {值.创建事实代次,
        (*p)[28] ? std::optional<std::uint64_t>{(*p)[28]} : std::nullopt};
    return 服务准备当前事实完整_v2(f, 值.创建事实代次);
}

inline constexpr std::uint32_t 发布事实节点本地键 = 1U;
inline constexpr std::uint32_t 发布事实载荷本地键 = 2U;
inline constexpr std::uint32_t 发布当前成员关系本地键 = 3U;

inline 服务合同关联进展事实_v2 投影进展发布材料(
    const 服务进展事实发布材料_v2& m) noexcept {
    服务合同关联进展事实_v2 f;
    f.自我 = m.自我; f.服务合同 = m.服务合同; f.需求 = m.需求;
    f.任务 = m.任务; f.方法 = m.方法;
    f.T到D关系稳定编码 = m.T到D关系稳定编码;
    f.执行绑定 = m.执行绑定; f.进展状态 = m.进展状态;
    f.进展动态 = m.进展动态; f.运行状态 = m.运行状态;
    f.运行代次 = m.运行代次;
    f.计量窗口开始完整秒边界 = m.计量窗口开始完整秒边界;
    f.进展发生完整秒边界 = m.进展发生完整秒边界;
    f.计量窗口结束完整秒边界 = m.计量窗口结束完整秒边界;
    f.方法内容版本 = m.方法内容版本; f.方法规格版本 = m.方法规格版本;
    f.方法生命周期版本 = m.方法生命周期版本;
    f.进展规则版本 = m.进展规则版本;
    return f;
}

inline 服务准备当前事实_v2 投影准备发布材料(
    const 服务准备事实发布材料_v2& m) noexcept {
    服务准备当前事实_v2 f;
    f.自我 = m.自我; f.来源 = m.来源; f.准备目标 = m.准备目标;
    f.适用服务范围 = m.适用服务范围;
    f.有效开始完整秒边界 = m.有效开始完整秒边界;
    f.有效结束完整秒边界 = m.有效结束完整秒边界;
    f.任务 = m.任务; f.方法 = m.方法;
    f.T到D关系稳定编码 = m.T到D关系稳定编码;
    f.执行绑定 = m.执行绑定; f.进展状态 = m.进展状态;
    f.进展动态 = m.进展动态; f.准备结果 = m.准备结果;
    f.完成验证 = m.完成验证; f.运行状态 = m.运行状态;
    f.运行代次 = m.运行代次;
    f.计量窗口开始完整秒边界 = m.计量窗口开始完整秒边界;
    f.进展发生完整秒边界 = m.进展发生完整秒边界;
    f.计量窗口结束完整秒边界 = m.计量窗口结束完整秒边界;
    f.方法内容版本 = m.方法内容版本; f.方法规格版本 = m.方法规格版本;
    f.方法生命周期版本 = m.方法生命周期版本;
    f.准备规则版本 = m.准备规则版本;
    return f;
}

inline void 添加发布关系(L1所有者范围写集请求& w, std::uint32_t& k,
    L1所有者范围事实引用 源, L1所有者范围事实引用 目标,
    稳定编码 类型, std::int64_t 顺序 = 1) {
    w.关系.push_back({键(k++), std::move(源), std::move(目标), 类型, 顺序});
}

inline L1所有者范围写集请求 形成进展发布写集(
    const 发布服务进展事实请求_v2& 请求,
    const 进展结构登记定位_v2& d,
    const 服务维护历史账登记定位_v1& h,
    const 服务维护历史覆盖登记定位_v2& h2,
    std::vector<稳定编码> 退出) {
    L1所有者范围写集请求 w;
    w.期望事实代次 = 请求.请求头.期望事实代次;
    w.写入幂等身份 = {请求.幂等身份.值};
    w.节点.push_back({键(发布事实节点本地键), 节点种类::普通, std::nullopt});
    const auto f = 投影进展发布材料(请求.材料);
    w.值.push_back({键(发布事实载荷本地键), 键(发布事实节点本地键),
        d.载荷, 编码进展_v2(f), 键(发布事实节点本地键)});
    w.属性槽变更.push_back({键(发布事实节点本地键), d.载荷,
        键(发布事实载荷本地键)});
    std::uint32_t k = 发布当前成员关系本地键;
    添加发布关系(w, k, d.当前索引根, 键(发布事实节点本地键),
        d.当前成员关系);
    添加发布关系(w, k, h.进展v2历史索引根,
        键(发布事实节点本地键), h.进展v2历史成员关系);
    if (h2.登记事实代次 != 0)
        添加发布关系(w, k, h2.进展历史索引根,
            键(发布事实节点本地键), h2.进展历史成员关系);
#if !defined(ARCH_INSTINCT_SERVICE_CONTRACT_FACT_AUTHORITY_VALIDATION)
    else
        throw std::runtime_error("service history v2 progress ledger unavailable");
#endif
    添加发布关系(w, k, 键(发布事实节点本地键), f.服务合同.值, d.服务合同关系);
    添加发布关系(w, k, 键(发布事实节点本地键), f.自我.值, d.自我关系);
    添加发布关系(w, k, 键(发布事实节点本地键), f.需求.值, d.需求关系);
    添加发布关系(w, k, 键(发布事实节点本地键), f.任务.值, d.任务关系);
    添加发布关系(w, k, 键(发布事实节点本地键), f.方法.值, d.方法关系);
    添加发布关系(w, k, 键(发布事实节点本地键), f.执行绑定.正式选择.值,
        d.正式选择关系);
    添加发布关系(w, k, 键(发布事实节点本地键),
        f.执行绑定.执行冻结材料.值, d.冻结材料关系);
    添加发布关系(w, k, 键(发布事实节点本地键), f.执行绑定.实例方法.值,
        d.实例方法关系);
    if (f.进展状态) 添加发布关系(w, k, 键(发布事实节点本地键),
        f.进展状态->值, d.状态引用关系);
    if (f.进展动态) 添加发布关系(w, k, 键(发布事实节点本地键),
        f.进展动态->值, d.动态引用关系);
    w.退出事实 = std::move(退出);
    return w;
}

inline L1所有者范围写集请求 形成准备发布写集(
    const 发布服务准备事实请求_v2& 请求,
    const 准备结构登记定位_v2& d,
    const 服务维护历史账登记定位_v1& h,
    const 服务维护历史覆盖登记定位_v2& h2,
    std::vector<稳定编码> 退出) {
    L1所有者范围写集请求 w;
    w.期望事实代次 = 请求.请求头.期望事实代次;
    w.写入幂等身份 = {请求.幂等身份.值};
    w.节点.push_back({键(发布事实节点本地键), 节点种类::普通, std::nullopt});
    const auto f = 投影准备发布材料(请求.材料);
    w.值.push_back({键(发布事实载荷本地键), 键(发布事实节点本地键),
        d.载荷, 编码准备_v2(f), 键(发布事实节点本地键)});
    w.属性槽变更.push_back({键(发布事实节点本地键), d.载荷,
        键(发布事实载荷本地键)});
    std::uint32_t k = 发布当前成员关系本地键;
    添加发布关系(w, k, d.当前索引根, 键(发布事实节点本地键),
        d.当前成员关系);
    添加发布关系(w, k, h.准备v2历史索引根,
        键(发布事实节点本地键), h.准备v2历史成员关系);
    if (h2.登记事实代次 != 0)
        添加发布关系(w, k, h2.准备历史索引根,
            键(发布事实节点本地键), h2.准备历史成员关系);
#if !defined(ARCH_INSTINCT_SERVICE_CONTRACT_FACT_AUTHORITY_VALIDATION)
    else
        throw std::runtime_error("service history v2 preparation ledger unavailable");
#endif
    const auto 来源 = std::holds_alternative<L2需求身份>(f.来源)
        ? std::get<L2需求身份>(f.来源).值
        : std::get<服务能力缺口身份_v1>(f.来源).值;
    添加发布关系(w, k, 键(发布事实节点本地键), 来源, d.来源关系);
    添加发布关系(w, k, 键(发布事实节点本地键), f.自我.值, d.自我关系);
    添加发布关系(w, k, 键(发布事实节点本地键), f.任务.值, d.任务关系);
    添加发布关系(w, k, 键(发布事实节点本地键), f.方法.值, d.方法关系);
    添加发布关系(w, k, 键(发布事实节点本地键), f.执行绑定.正式选择.值,
        d.正式选择关系);
    添加发布关系(w, k, 键(发布事实节点本地键),
        f.执行绑定.执行冻结材料.值, d.冻结材料关系);
    添加发布关系(w, k, 键(发布事实节点本地键), f.执行绑定.实例方法.值,
        d.实例方法关系);
    if (f.进展状态) 添加发布关系(w, k, 键(发布事实节点本地键),
        f.进展状态->值, d.状态引用关系);
    if (f.进展动态) 添加发布关系(w, k, 键(发布事实节点本地键),
        f.进展动态->值, d.动态引用关系);
    if (f.准备结果) 添加发布关系(w, k, 键(发布事实节点本地键),
        f.准备结果->值, d.结果引用关系);
    if (f.完成验证) 添加发布关系(w, k, 键(发布事实节点本地键),
        f.完成验证->值, d.验证引用关系);
    w.退出事实 = std::move(退出);
    return w;
}

template<class 请求类型, class 事实类型, class 登记类型, class 编码函数>
inline bool 首次发布请求同义(
    const L1所有者范围首次写入读取结果& 首次, const 请求类型& 请求,
    const 登记类型& d, const 事实类型& 事实, 编码函数 编码) noexcept {
    if (首次.状态 != L1所有者范围读取状态::成功
        || !首次.首次规范化写集 || !首次.首次写入结果) return false;
    const auto& w = *首次.首次规范化写集;
    if (w.合同版本 != L1所有者范围CRUD合同版本
        || w.期望事实代次 != 请求.请求头.期望事实代次
        || w.写入幂等身份.值 != 请求.幂等身份.值 || w.值.size() != 1)
        return false;
    const auto& v = w.值.front();
    if (!std::holds_alternative<稳定编码>(v.属性类型节点)
        || std::get<稳定编码>(v.属性类型节点) != d.载荷
        || !std::holds_alternative<std::vector<std::uint64_t>>(v.材料)
        || std::get<std::vector<std::uint64_t>>(v.材料) != 编码(事实))
        return false;
    return true;
}

inline const L1所有者范围一致闭包端点关系组读取结果项* 查找关系组(
    const std::vector<L1所有者范围一致闭包端点关系组读取结果项>& 组,
    稳定编码 关系类型) noexcept {
    const L1所有者范围一致闭包端点关系组读取结果项* 结果 = nullptr;
    for (const auto& 项 : 组) {
        if (项.关系类型节点 != 关系类型) continue;
        if (结果) return nullptr;
        结果 = &项;
    }
    return 结果;
}

inline std::optional<稳定编码> 唯一关系目标(
    const std::vector<L1所有者范围一致闭包端点关系组读取结果项>& 组,
    稳定编码 关系类型, 稳定编码 源节点, std::uint64_t 截止) noexcept {
    const auto* 命中 = 查找关系组(组, 关系类型);
    if (!命中 || 命中->状态 != L1所有者范围一致当前读取项目状态::成功
        || !命中->关系类型事实 || 命中->成员.size() != 1)
        return std::nullopt;
    const auto& 成员 = 命中->成员.front();
    if (成员.关系.源节点 != 源节点
        || 成员.关系.关系类型节点 != 关系类型
        || 成员.关系.目标节点 != 成员.对端节点.编码
        || !活动于(成员.关系, 截止) || !活动于(成员.对端节点, 截止))
        return std::nullopt;
    return 成员.对端节点.编码;
}

inline bool 可选关系目标闭合_v2(
    const std::vector<L1所有者范围一致闭包端点关系组读取结果项>& 组,
    稳定编码 关系类型, 稳定编码 源节点,
    const std::optional<稳定编码>& 期望, std::uint64_t 截止) noexcept {
    const auto* 命中 = 查找关系组(组, 关系类型);
    if (!命中 || 命中->状态 != L1所有者范围一致当前读取项目状态::成功
        || !命中->关系类型事实)
        return false;
    if (!期望) return 命中->成员.empty();
    if (命中->成员.size() != 1) return false;
    const auto& 成员 = 命中->成员.front();
    return 成员.关系.源节点 == 源节点
        && 成员.关系.关系类型节点 == 关系类型
        && 成员.关系.目标节点 == 成员.对端节点.编码
        && 成员.对端节点.编码 == *期望
        && 活动于(成员.关系, 截止) && 活动于(成员.对端节点, 截止);
}

inline bool 服务进展当前闭包引用完整_v2(
    const L1所有者范围一致关系类型闭包成员& 成员,
    const 服务合同关联进展事实_v2& 事实,
    const 进展结构登记定位_v2& d, std::uint64_t G0) noexcept {
    const auto 状态 = 事实.进展状态
        ? std::optional<稳定编码>{事实.进展状态->值} : std::nullopt;
    const auto 动态 = 事实.进展动态
        ? std::optional<稳定编码>{事实.进展动态->值} : std::nullopt;
    const auto& 组 = 成员.目标节点源关系组;
    const auto 源 = 成员.目标节点.编码;
    return 唯一关系目标(组, d.服务合同关系, 源, G0)
            == std::optional<稳定编码>{事实.服务合同.值}
        && 唯一关系目标(组, d.自我关系, 源, G0)
            == std::optional<稳定编码>{事实.自我.值}
        && 唯一关系目标(组, d.需求关系, 源, G0)
            == std::optional<稳定编码>{事实.需求.值}
        && 唯一关系目标(组, d.任务关系, 源, G0)
            == std::optional<稳定编码>{事实.任务.值}
        && 唯一关系目标(组, d.方法关系, 源, G0)
            == std::optional<稳定编码>{事实.方法.值}
        && 唯一关系目标(组, d.正式选择关系, 源, G0)
            == std::optional<稳定编码>{事实.执行绑定.正式选择.值}
        && 唯一关系目标(组, d.冻结材料关系, 源, G0)
            == std::optional<稳定编码>{事实.执行绑定.执行冻结材料.值}
        && 唯一关系目标(组, d.实例方法关系, 源, G0)
            == std::optional<稳定编码>{事实.执行绑定.实例方法.值}
        && 可选关系目标闭合_v2(组, d.状态引用关系, 源, 状态, G0)
        && 可选关系目标闭合_v2(组, d.动态引用关系, 源, 动态, G0);
}

inline bool 服务准备当前闭包引用完整_v2(
    const L1所有者范围一致关系类型闭包成员& 成员,
    const 服务准备当前事实_v2& 事实,
    const 准备结构登记定位_v2& d, std::uint64_t G0) noexcept {
    const auto 来源 = std::holds_alternative<L2需求身份>(事实.来源)
        ? std::get<L2需求身份>(事实.来源).值
        : std::get<服务能力缺口身份_v1>(事实.来源).值;
    const auto 状态 = 事实.进展状态
        ? std::optional<稳定编码>{事实.进展状态->值} : std::nullopt;
    const auto 动态 = 事实.进展动态
        ? std::optional<稳定编码>{事实.进展动态->值} : std::nullopt;
    const auto 结果 = 事实.准备结果
        ? std::optional<稳定编码>{事实.准备结果->值} : std::nullopt;
    const auto 验证 = 事实.完成验证
        ? std::optional<稳定编码>{事实.完成验证->值} : std::nullopt;
    const auto& 组 = 成员.目标节点源关系组;
    const auto 源 = 成员.目标节点.编码;
    return 唯一关系目标(组, d.来源关系, 源, G0)
            == std::optional<稳定编码>{来源}
        && 唯一关系目标(组, d.自我关系, 源, G0)
            == std::optional<稳定编码>{事实.自我.值}
        && 唯一关系目标(组, d.任务关系, 源, G0)
            == std::optional<稳定编码>{事实.任务.值}
        && 唯一关系目标(组, d.方法关系, 源, G0)
            == std::optional<稳定编码>{事实.方法.值}
        && 唯一关系目标(组, d.正式选择关系, 源, G0)
            == std::optional<稳定编码>{事实.执行绑定.正式选择.值}
        && 唯一关系目标(组, d.冻结材料关系, 源, G0)
            == std::optional<稳定编码>{事实.执行绑定.执行冻结材料.值}
        && 唯一关系目标(组, d.实例方法关系, 源, G0)
            == std::optional<稳定编码>{事实.执行绑定.实例方法.值}
        && 可选关系目标闭合_v2(组, d.状态引用关系, 源, 状态, G0)
        && 可选关系目标闭合_v2(组, d.动态引用关系, 源, 动态, G0)
        && 可选关系目标闭合_v2(组, d.结果引用关系, 源, 结果, G0)
        && 可选关系目标闭合_v2(组, d.验证引用关系, 源, 验证, G0);
}

inline const L1所有者范围一致关系类型闭包读取结果项* 查找闭包投影(
    const L1所有者范围一致关系类型闭包读取结果& 读取,
    稳定编码 关系类型) noexcept {
    const L1所有者范围一致关系类型闭包读取结果项* 结果 = nullptr;
    for (const auto& 项 : 读取.关系类型闭包) {
        if (项.入口关系类型节点 != 关系类型) continue;
        if (结果 || 项.状态 != L1所有者范围一致当前读取项目状态::成功
            || !项.关系类型事实)
            return nullptr;
        结果 = &项;
    }
    return 结果;
}

template<class 登记类型>
inline bool 登记投影完整_v2(
    const L1所有者范围一致关系类型闭包读取结果& 读,
    L1结构所有者身份 所有者, const 登记类型& d,
    std::uint32_t 版本, std::uint64_t G0) noexcept {
    const auto 节点组 = d.全部节点();
    if (读.合同版本 != L1所有者范围一致关系类型闭包读取合同版本
        || 读.读取事实代次 != G0 || 读.所有者.size() != 1)
        return false;
    const auto& o = 读.所有者.front();
    if (o.查询所有者 != 所有者
        || o.状态 != L1所有者范围一致当前读取项目状态::成功
        || !o.所有者事实 || o.所有者事实->退出事实代次)
        return false;
    std::size_t 命中节点数 = 0;
    for (const auto& n : 读.节点) {
        if (std::find(节点组.begin(), 节点组.end(), n.查询编码)
                == 节点组.end())
            continue;
        if (n.状态 != L1所有者范围一致当前读取项目状态::成功
            || !n.事实 || n.事实->写入所有者 != 所有者
            || !活动于(*n.事实, G0))
            return false;
        ++命中节点数;
    }
    if (命中节点数 != 节点组.size()) return false;
    const auto* p = 查找属性投影(读.属性值, d.登记锚点, d.登记版本载荷);
    const auto* v = p ? U64载荷(p->当前值事实) : nullptr;
    return p && p->当前值事实.写入所有者 == 所有者
        && 活动于(p->当前值事实, G0) && v
        && *v == std::vector<std::uint64_t>{版本};
}

inline L1所有者范围一致关系类型闭包读取请求
形成历史覆盖种子读取请求_v2(std::uint64_t G0,
    L1结构所有者身份 所有者, const 结构登记定位& 基础,
    const 进展结构登记定位_v2& 进展,
    const 准备结构登记定位_v2& 准备) {
    L1所有者范围一致关系类型闭包读取请求 r;
    r.期望事实代次 = G0;
    r.所有者 = {所有者};
    const auto 加节点 = [&](const auto& d) {
        const auto n = d.全部节点();
        r.节点.insert(r.节点.end(), n.begin(), n.end());
        r.属性值.push_back({d.登记锚点, d.登记版本载荷});
    };
    加节点(基础);
    加节点(进展);
    加节点(准备);
    L1所有者范围一致关系类型闭包选择项 合同;
    合同.入口关系类型节点 = 基础.当前合同成员关系;
    合同.目标节点属性类型 = {基础.合同载荷};
    合同.目标节点源关系类型 = {基础.合同自我关系,
        基础.合同提出者关系, 基础.合同需求关系,
        基础.合同目标宿主关系, 基础.合同目标特征关系,
        基础.合同目标状态合同关系};
    L1所有者范围一致关系类型闭包选择项 状态;
    状态.入口关系类型节点 = 基础.合同当前状态关系;
    状态.源节点属性类型 = {基础.合同载荷};
    状态.目标节点属性类型 = {基础.合同状态载荷};
    L1所有者范围一致关系类型闭包选择项 进展项;
    进展项.入口关系类型节点 = 进展.当前成员关系;
    进展项.目标节点属性类型 = {进展.载荷};
    进展项.目标节点源关系类型 = {进展.服务合同关系,
        进展.自我关系, 进展.需求关系, 进展.任务关系,
        进展.方法关系, 进展.正式选择关系, 进展.冻结材料关系,
        进展.实例方法关系, 进展.状态引用关系, 进展.动态引用关系};
    L1所有者范围一致关系类型闭包选择项 准备项;
    准备项.入口关系类型节点 = 准备.当前成员关系;
    准备项.目标节点属性类型 = {准备.载荷};
    准备项.目标节点源关系类型 = {准备.来源关系,
        准备.自我关系, 准备.任务关系, 准备.方法关系,
        准备.正式选择关系, 准备.冻结材料关系,
        准备.实例方法关系, 准备.状态引用关系, 准备.动态引用关系,
        准备.结果引用关系, 准备.验证引用关系};
    r.关系类型闭包 = {std::move(合同), std::move(状态),
        std::move(进展项), std::move(准备项)};
    return r;
}

inline std::optional<std::vector<服务维护历史覆盖成员_v2>>
读取当前历史覆盖种子_v2(const L1事实基座服务& L1,
    L1结构所有者身份 所有者, const 结构登记定位& 基础,
    const 进展结构登记定位_v2& 进展,
    const 准备结构登记定位_v2& 准备, std::uint64_t G0) {
    const auto 读 = L1.尝试读取所有者范围一致关系类型闭包投影(
        形成历史覆盖种子读取请求_v2(G0, 所有者, 基础, 进展, 准备));
    if (!登记投影完整_v2(读, 所有者, 基础,
            服务合同事实权威结构登记版本_v1, G0)
        || !登记投影完整_v2(读, 所有者, 进展,
            服务进展事实结构登记版本_v2, G0)
        || !登记投影完整_v2(读, 所有者, 准备,
            服务准备事实结构登记版本_v2, G0)
        || 读.关系类型闭包.size() != 4)
        return std::nullopt;
    const auto* 合同组 = 查找闭包投影(读, 基础.当前合同成员关系);
    const auto* 状态组 = 查找闭包投影(读, 基础.合同当前状态关系);
    const auto* 进展组 = 查找闭包投影(读, 进展.当前成员关系);
    const auto* 准备组 = 查找闭包投影(读, 准备.当前成员关系);
    if (!合同组 || !状态组 || !进展组 || !准备组) return std::nullopt;

    std::map<std::uint64_t, std::pair<服务合同状态事实_v1, 稳定编码>> 状态表;
    for (const auto& m : 状态组->成员) {
        if (m.关系.写入所有者 != 所有者
            || m.关系.源节点 != m.源节点.编码
            || m.关系.目标节点 != m.目标节点.编码
            || m.关系.关系类型节点 != 基础.合同当前状态关系
            || m.关系.角色或顺序 != 1 || !活动于(m.关系, G0)
            || m.目标节点.写入所有者 != 所有者
            || !活动于(m.目标节点, G0))
            return std::nullopt;
        const auto* p = 查找属性投影(
            m.目标节点属性值, m.目标节点.编码, 基础.合同状态载荷);
        服务合同状态事实_v1 f;
        if (!p || !解码状态(p->当前值事实, {m.源节点.编码}, f)
            || f.形成事实代次 != m.目标节点.创建事实代次
            || !状态表.emplace(m.源节点.编码.值,
                std::pair{f, m.目标节点.编码}).second)
            return std::nullopt;
    }

    std::vector<服务维护历史覆盖成员_v2> 种子;
    for (const auto& m : 合同组->成员) {
        if (m.关系.写入所有者 != 所有者
            || m.关系.源节点 != 基础.当前合同索引根
            || m.关系.目标节点 != m.目标节点.编码
            || m.关系.关系类型节点 != 基础.当前合同成员关系
            || m.关系.角色或顺序 != 1 || !活动于(m.关系, G0)
            || m.目标节点.写入所有者 != 所有者
            || !活动于(m.目标节点, G0))
            return std::nullopt;
        const auto* p = 查找属性投影(
            m.目标节点属性值, m.目标节点.编码, 基础.合同载荷);
        服务合同事实_v1 f;
        if (!p || !解码合同(p->当前值事实, m.目标节点.编码, f)
            || f.形成事实代次 != m.目标节点.创建事实代次)
            return std::nullopt;
        const auto u = [&](稳定编码 t) {
            return 唯一关系目标(m.目标节点源关系组, t,
                m.目标节点.编码, G0);
        };
        if (u(基础.合同自我关系) != std::optional<稳定编码>{f.自我.值}
            || u(基础.合同提出者关系) != std::optional<稳定编码>{f.提出者.值}
            || u(基础.合同需求关系) != std::optional<稳定编码>{f.需求.值}
            || u(基础.合同目标宿主关系) != std::optional<稳定编码>{f.目标宿主.值}
            || u(基础.合同目标特征关系) != std::optional<稳定编码>{f.目标特征.值}
            || u(基础.合同目标状态合同关系)
                != std::optional<稳定编码>{f.目标状态合同.值})
            return std::nullopt;
        const auto s = 状态表.find(f.身份.值.值);
        if (s == 状态表.end()
            || s->second.first.状态 != 服务合同当前状态_v1::有效未满足
            || s->second.first.已消费有效秒 > f.冻结有效总秒)
            return std::nullopt;
        种子.push_back({服务维护历史事实类别_v1::服务合同,
            m.目标节点.编码, f.身份.值, f.合同代次, f.形成事实代次});
        种子.push_back({服务维护历史事实类别_v1::合同状态,
            s->second.second, f.身份.值, s->second.first.状态版本,
            s->second.first.形成事实代次});
        状态表.erase(s);
    }
    if (!状态表.empty()) return std::nullopt;

    for (const auto& m : 进展组->成员) {
        const auto* p = 查找属性投影(
            m.目标节点属性值, m.目标节点.编码, 进展.载荷);
        服务合同关联进展事实_v2 f;
        if (m.关系.写入所有者 != 所有者
            || m.关系.源节点 != 进展.当前索引根
            || m.关系.目标节点 != m.目标节点.编码
            || m.关系.关系类型节点 != 进展.当前成员关系
            || m.关系.角色或顺序 != 1 || !活动于(m.关系, G0)
            || m.目标节点.写入所有者 != 所有者
            || !活动于(m.目标节点, G0)
            || !p || !解码进展_v2(p->当前值事实, m.目标节点.编码, f)
            || f.形成事实代次 != m.目标节点.创建事实代次
            || !服务进展当前闭包引用完整_v2(m, f, 进展, G0))
            return std::nullopt;
        种子.push_back({服务维护历史事实类别_v1::服务进展,
            m.目标节点.编码, f.身份.值, f.形成事实代次,
            f.形成事实代次});
    }
    for (const auto& m : 准备组->成员) {
        const auto* p = 查找属性投影(
            m.目标节点属性值, m.目标节点.编码, 准备.载荷);
        服务准备当前事实_v2 f;
        if (m.关系.写入所有者 != 所有者
            || m.关系.源节点 != 准备.当前索引根
            || m.关系.目标节点 != m.目标节点.编码
            || m.关系.关系类型节点 != 准备.当前成员关系
            || m.关系.角色或顺序 != 1 || !活动于(m.关系, G0)
            || m.目标节点.写入所有者 != 所有者
            || !活动于(m.目标节点, G0)
            || !p || !解码准备_v2(p->当前值事实, m.目标节点.编码, f)
            || f.形成事实代次 != m.目标节点.创建事实代次
            || !服务准备当前闭包引用完整_v2(m, f, 准备, G0))
            return std::nullopt;
        种子.push_back({服务维护历史事实类别_v1::服务准备,
            m.目标节点.编码, f.身份.值, f.形成事实代次,
            f.形成事实代次});
    }
    std::sort(种子.begin(), 种子.end(), 历史覆盖成员小于_v2);
    if (std::adjacent_find(种子.begin(), 种子.end()) != 种子.end())
        return std::nullopt;
    return 种子;
}

inline bool 同一服务进展活动流事实_v2(
    const 服务合同关联进展事实_v2& a,
    const 服务合同关联进展事实_v2& b) noexcept {
    return a.自我 == b.自我 && a.服务合同 == b.服务合同
        && a.需求 == b.需求 && a.任务 == b.任务 && a.方法 == b.方法
        && a.T到D关系稳定编码 == b.T到D关系稳定编码
        && a.执行绑定 == b.执行绑定;
}

inline bool 同一服务准备活动流事实_v2(
    const 服务准备当前事实_v2& a,
    const 服务准备当前事实_v2& b) noexcept {
    return a.自我 == b.自我 && a.来源 == b.来源
        && a.准备目标 == b.准备目标
        && a.适用服务范围 == b.适用服务范围
        && a.任务 == b.任务 && a.方法 == b.方法
        && a.T到D关系稳定编码 == b.T到D关系稳定编码
        && a.执行绑定 == b.执行绑定;
}

template<class 投影类型, class 同流函数>
inline bool 历史当前版本链按流闭合_v2(
    const std::vector<投影类型>& 组, 同流函数 同流) noexcept {
    for (const auto& 当前 : 组) {
        std::size_t 同代同流数 = 0;
        std::size_t 前驱数 = 0;
        std::size_t 后继数 = 0;
        bool 有更早同流 = false;
        for (const auto& 候选 : 组) {
            if (!同流(当前.事实, 候选.事实)) continue;
            同代同流数 += 候选.事实.形成事实代次
                == 当前.事实.形成事实代次;
            有更早同流 = 有更早同流
                || 候选.事实.形成事实代次 < 当前.事实.形成事实代次;
            前驱数 += 候选.退出当前事实代次
                && *候选.退出当前事实代次 == 当前.事实.形成事实代次;
            后继数 += 当前.退出当前事实代次
                && 候选.事实.形成事实代次 == *当前.退出当前事实代次;
        }
        if (同代同流数 != 1
            || 前驱数 != (有更早同流 ? 1U : 0U)
            || 后继数 != (当前.退出当前事实代次 ? 1U : 0U))
            return false;
    }
    return true;
}

inline L1所有者范围写集请求 形成历史覆盖登记写集_v2(
    std::uint64_t G0, const std::vector<服务维护历史覆盖成员_v2>& 种子) {
    L1所有者范围写集请求 w;
    w.期望事实代次 = G0;
    w.写入幂等身份 = 历史覆盖登记幂等身份_v2;
    for (std::uint32_t k = 历史覆盖登记锚点键_v2;
        k <= 准备历史成员关系键_v2; ++k) {
        w.节点.push_back({键(k),
            k == 历史覆盖登记版本载荷键_v2 ? 节点种类::属性类型
                                               : 节点种类::普通,
            k == 历史覆盖登记版本载荷键_v2
                ? std::optional<L1所有者范围值表示种类>{
                    L1所有者范围值表示种类::U64组}
                : std::nullopt});
    }
    w.值.push_back({键(历史覆盖登记版本值本地键_v2),
        键(历史覆盖登记锚点键_v2), 键(历史覆盖登记版本载荷键_v2),
        std::vector<std::uint64_t>{服务维护历史覆盖结构登记版本_v2},
        键(历史覆盖登记锚点键_v2)});
    w.属性槽变更.push_back({键(历史覆盖登记锚点键_v2),
        键(历史覆盖登记版本载荷键_v2),
        键(历史覆盖登记版本值本地键_v2)});
    std::uint32_t rk = 历史覆盖种子关系本地键起点_v2;
    for (const auto& m : 种子) {
        std::uint32_t 根 = 0, 类型 = 0;
        switch (m.类别) {
        case 服务维护历史事实类别_v1::服务合同:
            根 = 合同历史索引根键_v2; 类型 = 合同历史成员关系键_v2; break;
        case 服务维护历史事实类别_v1::合同状态:
            根 = 合同状态历史索引根键_v2;
            类型 = 合同状态历史成员关系键_v2; break;
        case 服务维护历史事实类别_v1::服务进展:
            根 = 进展历史索引根键_v2; 类型 = 进展历史成员关系键_v2; break;
        case 服务维护历史事实类别_v1::服务准备:
            根 = 准备历史索引根键_v2; 类型 = 准备历史成员关系键_v2; break;
        default:
            throw std::runtime_error("invalid service history v2 seed category");
        }
        w.关系.push_back({键(rk++), 键(根), m.目标事实节点, 键(类型), 1});
    }
    return w;
}

inline 服务维护历史覆盖登记定位_v2 从映射形成历史覆盖定位_v2(
    const std::vector<std::pair<L1所有者范围写集本地键, 稳定编码>>& m,
    std::uint64_t Gseed) {
    const auto 取 = [&](std::uint32_t k) {
        const auto v = 查找映射(m, k);
        if (!v) throw std::runtime_error("service history v2 mapping missing");
        return *v;
    };
    return {取(历史覆盖登记锚点键_v2),
        取(历史覆盖登记版本载荷键_v2), 取(合同历史索引根键_v2),
        取(合同历史成员关系键_v2), 取(合同状态历史索引根键_v2),
        取(合同状态历史成员关系键_v2), 取(到期事件历史索引根键_v2),
        取(到期事件历史成员关系键_v2), 取(进展历史索引根键_v2),
        取(进展历史成员关系键_v2), 取(准备历史索引根键_v2),
        取(准备历史成员关系键_v2), Gseed};
}

inline bool 验证历史覆盖登记当前投影_v2(
    const L1事实基座服务& L1, L1结构所有者身份 所有者,
    const 服务维护历史覆盖登记定位_v2& d, std::uint64_t G0) {
    L1所有者范围一致当前读取请求 r;
    r.期望事实代次 = G0; r.所有者 = {所有者}; r.节点 = d.全部节点();
    r.属性值 = {{d.登记锚点, d.登记版本载荷}};
    const auto 读 = L1.尝试读取所有者范围一致当前投影(r);
    if (读.状态 != L1所有者范围一致当前读取状态::成功
        || 读.读取事实代次 != G0 || 读.所有者.size() != 1
        || 读.节点.size() != d.全部节点().size()) return false;
    for (const auto& n : 读.节点)
        if (n.状态 != L1所有者范围一致当前读取项目状态::成功
            || !n.事实 || n.事实->写入所有者 != 所有者
            || !活动于(*n.事实, G0)) return false;
    const auto* p = 查找属性投影(读.属性值, d.登记锚点, d.登记版本载荷);
    const auto* v = p ? U64载荷(p->当前值事实) : nullptr;
    return v && *v == std::vector<std::uint64_t>{
        服务维护历史覆盖结构登记版本_v2};
}

template<class 事实类型, class 解码函数>
inline std::optional<事实类型> 读取历史覆盖目标载荷_v2(
    const L1事实基座服务& L1, L1结构所有者身份 所有者,
    稳定编码 目标, 稳定编码 载荷类型, std::uint64_t Gread,
    解码函数 解码) {
    const auto n = L1.读取所有者范围历史事实(
        {L1所有者范围CRUD合同版本, 目标});
    if (n.状态 != L1所有者范围读取状态::成功
        || n.读取事实代次 != Gread || !n.事实
        || !std::holds_alternative<L1所有者范围节点事实>(*n.事实))
        return std::nullopt;
    const auto& 节点 = std::get<L1所有者范围节点事实>(*n.事实);
    if (节点.编码 != 目标 || 节点.写入所有者 != 所有者
        || 节点.创建事实代次 == 0 || 节点.退出事实代次)
        return std::nullopt;
    const auto values = L1.读取所有者范围历史属性值组(
        {L1所有者范围CRUD合同版本, 目标, 节点.创建事实代次});
    if (values.状态 != L1所有者范围读取状态::成功
        || values.读取事实代次 != Gread) return std::nullopt;
    const L1所有者范围值事实* value = nullptr;
    for (const auto& v : values.属性值组) {
        if (v.属性类型节点 != 载荷类型
            || v.创建事实代次 > 节点.创建事实代次
            || (v.退出事实代次 && *v.退出事实代次 <= 节点.创建事实代次))
            continue;
        if (value) return std::nullopt;
        value = &v;
    }
    事实类型 f;
    if (!value || !解码(*value, 目标, f)
        || f.形成事实代次 != 节点.创建事实代次)
        return std::nullopt;
    return f;
}

inline std::optional<L1所有者范围关系事实> 读取种子当前关系_v2(
    const L1事实基座服务& L1, L1结构所有者身份 所有者,
    稳定编码 目标, 稳定编码 类型, std::uint64_t Gseed前,
    std::uint64_t Gread) {
    const auto r = L1.读取所有者范围历史关系组({
        L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::目标,
        目标, 类型, Gseed前});
    if (r.状态 != L1所有者范围读取状态::成功
        || r.读取事实代次 != Gread || r.关系组.size() != 1)
        return std::nullopt;
    const auto& x = r.关系组.front();
    if (x.目标节点 != 目标 || x.关系类型节点 != 类型
        || x.写入所有者 != 所有者 || x.角色或顺序 != 1
        || !活动于(x, Gseed前)) return std::nullopt;
    return x;
}

inline std::optional<服务维护历史覆盖成员_v2>
从首次种子目标重建覆盖成员_v2(const L1事实基座服务& L1,
    L1结构所有者身份 所有者, 服务维护历史事实类别_v1 类别,
    稳定编码 目标, const 结构登记定位& 基础,
    const 进展结构登记定位_v2& 进展,
    const 准备结构登记定位_v2& 准备,
    std::uint64_t Gseed前, std::uint64_t Gread) {
    switch (类别) {
    case 服务维护历史事实类别_v1::服务合同: {
        const auto f = 读取历史覆盖目标载荷_v2<服务合同事实_v1>(
            L1, 所有者, 目标, 基础.合同载荷, Gread, 解码合同);
        const auto 当前 = 读取种子当前关系_v2(
            L1, 所有者, 目标, 基础.当前合同成员关系, Gseed前, Gread);
        if (!f || !当前 || 当前->源节点 != 基础.当前合同索引根)
            return std::nullopt;
        return 服务维护历史覆盖成员_v2{类别, 目标, f->身份.值,
            f->合同代次, f->形成事实代次};
    }
    case 服务维护历史事实类别_v1::合同状态: {
        const auto 当前 = 读取种子当前关系_v2(
            L1, 所有者, 目标, 基础.合同当前状态关系, Gseed前, Gread);
        if (!当前) return std::nullopt;
        const auto f = 读取历史覆盖目标载荷_v2<服务合同状态事实_v1>(
            L1, 所有者, 目标, 基础.合同状态载荷, Gread,
            [&](const auto& v, 稳定编码, auto& out) {
                return 解码状态(v, {当前->源节点}, out);
            });
        if (!f) return std::nullopt;
        return 服务维护历史覆盖成员_v2{类别, 目标, f->合同.值,
            f->状态版本, f->形成事实代次};
    }
    case 服务维护历史事实类别_v1::服务进展: {
        const auto f = 读取历史覆盖目标载荷_v2<服务合同关联进展事实_v2>(
            L1, 所有者, 目标, 进展.载荷, Gread, 解码进展_v2);
        const auto 当前 = 读取种子当前关系_v2(
            L1, 所有者, 目标, 进展.当前成员关系, Gseed前, Gread);
        if (!f || !当前 || 当前->源节点 != 进展.当前索引根)
            return std::nullopt;
        return 服务维护历史覆盖成员_v2{类别, 目标, f->身份.值,
            f->形成事实代次, f->形成事实代次};
    }
    case 服务维护历史事实类别_v1::服务准备: {
        const auto f = 读取历史覆盖目标载荷_v2<服务准备当前事实_v2>(
            L1, 所有者, 目标, 准备.载荷, Gread, 解码准备_v2);
        const auto 当前 = 读取种子当前关系_v2(
            L1, 所有者, 目标, 准备.当前成员关系, Gseed前, Gread);
        if (!f || !当前 || 当前->源节点 != 准备.当前索引根)
            return std::nullopt;
        return 服务维护历史覆盖成员_v2{类别, 目标, f->身份.值,
            f->形成事实代次, f->形成事实代次};
    }
    default:
        return std::nullopt;
    }
}

inline std::optional<std::vector<服务维护历史覆盖成员_v2>>
从首次写集重建覆盖种子_v2(const L1事实基座服务& L1,
    L1结构所有者身份 所有者, const L1所有者范围写集请求& w,
    const 结构登记定位& 基础, const 进展结构登记定位_v2& 进展,
    const 准备结构登记定位_v2& 准备, std::uint64_t Gseed,
    std::uint64_t Gread) {
    if (Gseed == 0 || w.期望事实代次 == 0
        || Gseed != w.期望事实代次 + 1
        || w.写入幂等身份 != 历史覆盖登记幂等身份_v2)
        return std::nullopt;
    std::vector<服务维护历史覆盖成员_v2> 结果;
    std::uint32_t 预期关系键 = 历史覆盖种子关系本地键起点_v2;
    for (const auto& r : w.关系) {
        const auto* 源 = std::get_if<L1所有者范围写集本地键>(&r.源节点);
        const auto* 目标 = std::get_if<稳定编码>(&r.目标节点);
        const auto* 类型 = std::get_if<L1所有者范围写集本地键>(&r.关系类型节点);
        if (!源 || !目标 || !类型 || r.本地键.值 != 预期关系键++
            || r.角色或顺序 != 1) return std::nullopt;
        服务维护历史事实类别_v1 类别;
        if (源->值 == 合同历史索引根键_v2
            && 类型->值 == 合同历史成员关系键_v2)
            类别 = 服务维护历史事实类别_v1::服务合同;
        else if (源->值 == 合同状态历史索引根键_v2
            && 类型->值 == 合同状态历史成员关系键_v2)
            类别 = 服务维护历史事实类别_v1::合同状态;
        else if (源->值 == 进展历史索引根键_v2
            && 类型->值 == 进展历史成员关系键_v2)
            类别 = 服务维护历史事实类别_v1::服务进展;
        else if (源->值 == 准备历史索引根键_v2
            && 类型->值 == 准备历史成员关系键_v2)
            类别 = 服务维护历史事实类别_v1::服务准备;
        else return std::nullopt;
        const auto m = 从首次种子目标重建覆盖成员_v2(
            L1, 所有者, 类别, *目标, 基础, 进展, 准备,
            w.期望事实代次, Gread);
        if (!m) return std::nullopt;
        结果.push_back(*m);
    }
    if (!std::is_sorted(结果.begin(), 结果.end(), 历史覆盖成员小于_v2)
        || std::adjacent_find(结果.begin(), 结果.end()) != 结果.end()
        || w != 形成历史覆盖登记写集_v2(w.期望事实代次, 结果))
        return std::nullopt;
    return 结果;
}

inline L1所有者范围一致关系类型闭包读取请求
形成历史覆盖恢复验证读取请求_v2(std::uint64_t Gread,
    L1结构所有者身份 所有者,
    const 服务维护历史覆盖登记定位_v2& d,
    const 结构登记定位& 基础, const 进展结构登记定位_v2& 进展,
    const 准备结构登记定位_v2& 准备) {
    L1所有者范围一致关系类型闭包读取请求 r;
    r.期望事实代次 = Gread;
    r.所有者 = {所有者};
    r.节点 = d.全部节点();
    r.属性值 = {{d.登记锚点, d.登记版本载荷}};
    const auto 加 = [&](稳定编码 入口, 稳定编码 载荷) {
        L1所有者范围一致关系类型闭包选择项 x;
        x.入口关系类型节点 = 入口;
        x.目标节点属性类型 = {载荷};
        r.关系类型闭包.push_back(std::move(x));
    };
    加(d.合同历史成员关系, 基础.合同载荷);
    加(d.合同状态历史成员关系, 基础.合同状态载荷);
    加(d.到期事件历史成员关系, 基础.到期事件载荷);
    加(d.进展历史成员关系, 进展.载荷);
    加(d.准备历史成员关系, 准备.载荷);
    return r;
}

inline std::optional<服务维护历史覆盖成员_v2>
解码历史覆盖闭包成员_v2(const L1事实基座服务& L1,
    L1结构所有者身份 所有者, 服务维护历史事实类别_v1 类别,
    const L1所有者范围一致关系类型闭包成员& m,
    稳定编码 根, 稳定编码 类型, const 结构登记定位& 基础,
    const 进展结构登记定位_v2& 进展,
    const 准备结构登记定位_v2& 准备,
    std::uint64_t Gseed, std::uint64_t Gread) {
    const auto G1 = m.目标节点.创建事实代次;
    const auto 预期关系代次 = G1 < Gseed ? Gseed : G1;
    if (G1 == 0 || G1 == Gseed
        || m.关系.源节点 != 根
        || m.关系.目标节点 != m.目标节点.编码
        || m.关系.关系类型节点 != 类型 || m.关系.角色或顺序 != 1
        || m.关系.写入所有者 != 所有者
        || m.目标节点.写入所有者 != 所有者
        || m.关系.创建事实代次 != 预期关系代次
        || m.关系.退出事实代次 || m.目标节点.退出事实代次
        || !活动于(m.关系, Gread) || !活动于(m.目标节点, Gread))
        return std::nullopt;
    switch (类别) {
    case 服务维护历史事实类别_v1::服务合同: {
        const auto* p = 查找属性投影(
            m.目标节点属性值, m.目标节点.编码, 基础.合同载荷);
        服务合同事实_v1 f;
        if (!p || !解码合同(p->当前值事实, m.目标节点.编码, f)
            || f.形成事实代次 != G1) return std::nullopt;
        return 服务维护历史覆盖成员_v2{
            类别, m.目标节点.编码, f.身份.值, f.合同代次, G1};
    }
    case 服务维护历史事实类别_v1::合同状态: {
        const auto 边界 = G1 < Gseed ? Gseed - 1 : G1;
        const auto 当前 = 读取种子当前关系_v2(
            L1, 所有者, m.目标节点.编码, 基础.合同当前状态关系,
            边界, Gread);
        const auto* p = 查找属性投影(
            m.目标节点属性值, m.目标节点.编码, 基础.合同状态载荷);
        服务合同状态事实_v1 f;
        if (!当前 || !p || !解码状态(
                p->当前值事实, {当前->源节点}, f)
            || f.形成事实代次 != G1) return std::nullopt;
        return 服务维护历史覆盖成员_v2{
            类别, m.目标节点.编码, f.合同.值, f.状态版本, G1};
    }
    case 服务维护历史事实类别_v1::到期未满足事件: {
        const auto* p = 查找属性投影(
            m.目标节点属性值, m.目标节点.编码, 基础.到期事件载荷);
        服务到期未满足事件事实_v1 f;
        if (!p || !解码事件(p->当前值事实, m.目标节点.编码, f)
            || f.形成事实代次 != G1) return std::nullopt;
        return 服务维护历史覆盖成员_v2{类别, m.目标节点.编码,
            f.身份.值, f.合同终态版本, G1};
    }
    case 服务维护历史事实类别_v1::服务进展: {
        const auto* p = 查找属性投影(
            m.目标节点属性值, m.目标节点.编码, 进展.载荷);
        服务合同关联进展事实_v2 f;
        if (!p || !解码进展_v2(p->当前值事实, m.目标节点.编码, f)
            || f.形成事实代次 != G1) return std::nullopt;
        return 服务维护历史覆盖成员_v2{
            类别, m.目标节点.编码, f.身份.值, G1, G1};
    }
    case 服务维护历史事实类别_v1::服务准备: {
        const auto* p = 查找属性投影(
            m.目标节点属性值, m.目标节点.编码, 准备.载荷);
        服务准备当前事实_v2 f;
        if (!p || !解码准备_v2(p->当前值事实, m.目标节点.编码, f)
            || f.形成事实代次 != G1) return std::nullopt;
        return 服务维护历史覆盖成员_v2{
            类别, m.目标节点.编码, f.身份.值, G1, G1};
    }
    default:
        return std::nullopt;
    }
}

inline bool 验证历史覆盖永久成员全集_v2(const L1事实基座服务& L1,
    L1结构所有者身份 所有者,
    const 服务维护历史覆盖登记定位_v2& d,
    const 结构登记定位& 基础, const 进展结构登记定位_v2& 进展,
    const 准备结构登记定位_v2& 准备,
    const std::vector<服务维护历史覆盖成员_v2>& 首次种子,
    std::uint64_t Gread) {
    const auto 读 = L1.尝试读取所有者范围一致关系类型闭包投影(
        形成历史覆盖恢复验证读取请求_v2(
            Gread, 所有者, d, 基础, 进展, 准备));
    if (!登记投影完整_v2(读, 所有者, d,
            服务维护历史覆盖结构登记版本_v2, Gread)
        || 读.关系类型闭包.size() != 5)
        return false;
    struct 闭包规格 final {
        服务维护历史事实类别_v1 类别;
        稳定编码 根;
        稳定编码 类型;
    };
    const std::array<闭包规格, 5> 规格{{
        {服务维护历史事实类别_v1::服务合同,
            d.合同历史索引根, d.合同历史成员关系},
        {服务维护历史事实类别_v1::合同状态,
            d.合同状态历史索引根, d.合同状态历史成员关系},
        {服务维护历史事实类别_v1::到期未满足事件,
            d.到期事件历史索引根, d.到期事件历史成员关系},
        {服务维护历史事实类别_v1::服务进展,
            d.进展历史索引根, d.进展历史成员关系},
        {服务维护历史事实类别_v1::服务准备,
            d.准备历史索引根, d.准备历史成员关系}}};
    std::vector<服务维护历史覆盖成员_v2> 全部;
    std::vector<服务维护历史覆盖成员_v2> Gseed子集;
    for (const auto& s : 规格) {
        const auto* 闭包 = 查找闭包投影(读, s.类型);
        if (!闭包) return false;
        for (const auto& m : 闭包->成员) {
            const auto x = 解码历史覆盖闭包成员_v2(
                L1, 所有者, s.类别, m, s.根, s.类型,
                基础, 进展, 准备, d.登记事实代次, Gread);
            if (!x) return false;
            全部.push_back(*x);
            if (x->形成事实代次 < d.登记事实代次)
                Gseed子集.push_back(*x);
        }
    }
    std::sort(全部.begin(), 全部.end(), 历史覆盖成员小于_v2);
    std::sort(Gseed子集.begin(), Gseed子集.end(), 历史覆盖成员小于_v2);
    return std::adjacent_find(全部.begin(), 全部.end()) == 全部.end()
        && Gseed子集 == 首次种子;
}

inline 服务维护历史覆盖登记定位_v2 初始化历史覆盖结构登记_v2(
    const L1事实基座服务& L1, L1所有者范围写端口& 端口,
    const 结构登记定位& 基础, const 进展结构登记定位_v2& 进展,
    const 准备结构登记定位_v2& 准备) {
    auto 首次 = 端口.读取首次写入材料(
        {L1所有者范围首次写入读取合同版本, 历史覆盖登记幂等身份_v2});
    L1所有者范围写集请求 w;
    if (首次.状态 == L1所有者范围读取状态::成功
        && 首次.首次规范化写集 && 首次.首次写入结果) {
        w = *首次.首次规范化写集;
    } else if (首次.状态 == L1所有者范围读取状态::未找到) {
        const auto 当前 = L1.读取中性当前事实代次({L1中性CRUD合同版本});
        if (当前.状态 != L1中性读取状态::成功 || 当前.事实代次 == 0)
            throw std::runtime_error("service history v2 cutoff unavailable");
        const auto 种子 = 读取当前历史覆盖种子_v2(
            L1, 端口.所有者身份(), 基础, 进展, 准备, 当前.事实代次);
        if (!种子) throw std::runtime_error("service history v2 seed unavailable");
        w = 形成历史覆盖登记写集_v2(当前.事实代次, *种子);
    } else {
        throw std::runtime_error("service history v2 first write unavailable");
    }
    const auto 写 = 端口.提交所有者范围中性写集(w);
    if ((写.状态 != L1所有者范围写入状态::成功
            && 写.状态 != L1所有者范围写入状态::精确重复)
        || 写.事实代次 == 0)
        throw std::runtime_error("service history v2 registration failed");
    首次 = 端口.读取首次写入材料(
        {L1所有者范围首次写入读取合同版本, 历史覆盖登记幂等身份_v2});
    if (首次.状态 != L1所有者范围读取状态::成功
        || !首次.首次规范化写集 || !首次.首次写入结果
        || 首次.首次写入结果->事实代次 != 写.事实代次
        || 首次.首次规范化写集->写入幂等身份
            != 历史覆盖登记幂等身份_v2
        || 首次.首次规范化写集->期望事实代次 == 0)
        throw std::runtime_error("service history v2 first write drift");
    const auto d = 从映射形成历史覆盖定位_v2(
        首次.首次写入结果->新编码映射, 首次.首次写入结果->事实代次);
    const auto 当前 = L1.读取中性当前事实代次({L1中性CRUD合同版本});
    const auto 重建种子 = 当前.状态 == L1中性读取状态::成功
        ? 从首次写集重建覆盖种子_v2(L1, 端口.所有者身份(),
            *首次.首次规范化写集, 基础, 进展, 准备,
            首次.首次写入结果->事实代次, 当前.事实代次)
        : std::nullopt;
    if (当前.状态 != L1中性读取状态::成功 || !重建种子
        || !验证历史覆盖登记当前投影_v2(
            L1, 端口.所有者身份(), d, 当前.事实代次)
        || !验证历史覆盖永久成员全集_v2(L1, 端口.所有者身份(),
            d, 基础, 进展, 准备, *重建种子, 当前.事实代次))
        throw std::runtime_error("service history v2 registration readback failed");
    return d;
}

#if defined(ARCH_INSTINCT_SERVICE_CONTRACT_FACT_AUTHORITY_VALIDATION)
inline bool ARCH_延迟下一实例历史覆盖登记_v2 = false;
#endif

inline 服务维护历史覆盖登记定位_v2 构造期初始化历史覆盖结构登记_v2(
    const L1事实基座服务& L1, L1所有者范围写端口& 端口,
    const 结构登记定位& 基础, const 进展结构登记定位_v2& 进展,
    const 准备结构登记定位_v2& 准备) {
#if defined(ARCH_INSTINCT_SERVICE_CONTRACT_FACT_AUTHORITY_VALIDATION)
    if (ARCH_延迟下一实例历史覆盖登记_v2) {
        ARCH_延迟下一实例历史覆盖登记_v2 = false;
        return {};
    }
#endif
    return 初始化历史覆盖结构登记_v2(L1, 端口, 基础, 进展, 准备);
}

inline 服务合同事实权威读取状态_v1 映射读取状态(
    L1所有者范围一致当前读取状态 状态) noexcept {
    switch (状态) {
    case L1所有者范围一致当前读取状态::许可拒绝:
        return 服务合同事实权威读取状态_v1::许可拒绝;
    case L1所有者范围一致当前读取状态::事实代次漂移:
        return 服务合同事实权威读取状态_v1::当前性漂移;
    case L1所有者范围一致当前读取状态::资源失败:
        return 服务合同事实权威读取状态_v1::资源失败;
    case L1所有者范围一致当前读取状态::入口拒绝:
        return 服务合同事实权威读取状态_v1::入口拒绝;
    default:
        return 服务合同事实权威读取状态_v1::内部错误;
    }
}

} // namespace 海中鱼巣::服务合同事实权威内部

export namespace 海中鱼巣 {

inline std::optional<服务合同事实权威所有者交付_v1>
尝试形成服务合同事实权威所有者交付_v1(
    const L1事实基座服务& 第一层服务,
    L1所有者范围交付&& 原始交付) noexcept {
    try {
        const auto& 建立 = 原始交付.建立结果;
        const bool 首次完整 = 建立.状态 == L1所有者范围管理状态::成功
            && 建立.是否形成内存权威发布
            && 建立.重试边界 == L1所有者范围重试边界::不适用;
        const bool 重复完整 = 建立.状态 == L1所有者范围管理状态::精确重复
            && !建立.是否形成内存权威发布
            && 建立.重试边界 == L1所有者范围重试边界::原幂等身份读回收敛;
        if (建立.合同版本 != L1所有者范围CRUD合同版本
            || 建立.建立幂等身份 != 服务合同事实权威所有者建立身份_v1
            || 建立.事实代次 == 0 || (!首次完整 && !重复完整)
            || !建立.所有者事实 || !原始交付.写入端口
            || 建立.所有者事实->范围种类 != L1所有者范围种类::独占结构范围
            || 建立.所有者事实->退出事实代次
            || 原始交付.写入端口->所有者身份() != 建立.所有者事实->所有者
            || !原始交付.写入端口->有效()
            || !原始交付.写入端口->绑定于(第一层服务))
            return std::nullopt;
        const auto 当前 = 第一层服务.读取当前结构所有者(
            {L1所有者范围CRUD合同版本, 建立.所有者事实->所有者});
        if (当前.状态 != L1所有者范围读取状态::成功
            || 当前.合同版本 != L1所有者范围CRUD合同版本
            || 当前.查询所有者 != 建立.所有者事实->所有者
            || !当前.所有者事实 || *当前.所有者事实 != *建立.所有者事实)
            return std::nullopt;
        return 服务合同事实权威所有者交付_v1{
            std::move(*原始交付.写入端口)};
    } catch (...) {
        return std::nullopt;
    }
}

#if defined(ARCH_INSTINCT_SERVICE_CONTRACT_FACT_AUTHORITY_VALIDATION)
struct ARCH_服务需求时间裁决验证配置_v1 final {
    std::optional<L2存在身份> 固定自我{};
    std::optional<std::uint64_t> 共同时间纪元身份{};
    std::int64_t 提出起始完整秒边界 = 10;
    std::int64_t 提出步长 = 0;
    std::int64_t 有效开始偏移 = 0;
    std::int64_t 最后已结算完整秒边界 = 10;
    std::int64_t 到期完整秒边界 = 110;
    std::uint32_t 合同规则版本 = 1;
    std::uint32_t 事件规则版本 = 1;
    bool 重复首个需求 = false;
    friend bool operator==(const ARCH_服务需求时间裁决验证配置_v1&,
        const ARCH_服务需求时间裁决验证配置_v1&) = default;
};
#endif

class 服务合同事实权威服务 final {
public:
    服务合同事实权威服务() = delete;
    服务合同事实权威服务(const 服务合同事实权威服务&) = delete;
    服务合同事实权威服务& operator=(const 服务合同事实权威服务&) = delete;

#if defined(ARCH_INSTINCT_SERVICE_CONTRACT_FACT_AUTHORITY_VALIDATION)
    static void ARCH_延迟下一实例v2历史覆盖登记() noexcept {
        服务合同事实权威内部::ARCH_延迟下一实例历史覆盖登记_v2 = true;
    }
#endif

    服务合同事实权威服务(const L1事实基座服务& 第一层服务,
        服务合同事实权威所有者交付_v1&& 交付)
        : L1_(第一层服务), 写入端口_(验证并移动交付(第一层服务, 交付)),
          登记_(服务合同事实权威内部::初始化结构登记(L1_, 写入端口_)),
          进展登记_(服务合同事实权威内部::初始化进展结构登记(
              L1_, 写入端口_)),
          准备登记_(服务合同事实权威内部::初始化准备结构登记(
              L1_, 写入端口_)),
          进展登记_v2_(服务合同事实权威内部::初始化进展v2结构登记(
              L1_, 写入端口_)),
          准备登记_v2_(服务合同事实权威内部::初始化准备v2结构登记(
              L1_, 写入端口_)),
          历史账登记_(服务合同事实权威内部::初始化历史账结构登记(
              L1_, 写入端口_)),
          历史覆盖登记_v2_(
              服务合同事实权威内部::构造期初始化历史覆盖结构登记_v2(
                  L1_, 写入端口_, 登记_, 进展登记_v2_, 准备登记_v2_)) {}

    服务合同完整集合读取结果_v1
    读取当前有效未满足服务合同完整集合_v1(
        const 服务合同完整集合读取请求_v1& 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 失败 = [](服务合同事实权威读取状态_v1 状态) {
            服务合同完整集合读取结果_v1 结果;
            结果.状态 = 状态;
            return 结果;
        };
        try {
            if (!请求有效(请求.合同版本, 请求.请求头, 请求.自我))
                return 失败(服务合同事实权威读取状态_v1::入口拒绝);
            const auto G0 = 请求.请求头.期望事实代次;
            if (!读前守卫(G0))
                return 失败(服务合同事实权威读取状态_v1::当前性漂移);
#if defined(ARCH_INSTINCT_SERVICE_CONTRACT_FACT_AUTHORITY_VALIDATION)
            if (ARCH_资源失败一次_) {
                ARCH_资源失败一次_ = false;
                return 失败(服务合同事实权威读取状态_v1::资源失败);
            }
            注入漂移_已锁定();
#endif
            auto L1请求 = 形成合同读取请求(G0);
            const auto 读取 = L1_.尝试读取所有者范围一致关系类型闭包投影(L1请求);
            if (读取.状态 != L1所有者范围一致当前读取状态::成功)
                return 失败(服务合同事实权威内部::映射读取状态(读取.状态));
            if (读取.读取事实代次 != G0)
                return 失败(服务合同事实权威读取状态_v1::当前性漂移);
            if (!验证共同登记(读取, G0))
                return 失败(服务合同事实权威读取状态_v1::结构未登记);
            if (读取.关系类型闭包.size() != 2)
                return 失败(服务合同事实权威读取状态_v1::集合不闭合);
            const auto* 成员闭包 = 查找闭包(读取, 登记_.当前合同成员关系);
            const auto* 状态闭包 = 查找闭包(读取, 登记_.合同当前状态关系);
            if (!成员闭包 || !状态闭包)
                return 失败(服务合同事实权威读取状态_v1::集合不闭合);

            std::map<std::uint64_t, 服务合同状态事实_v1> 状态表;
            for (const auto& 成员 : 状态闭包->成员) {
                if (成员.关系.关系类型节点 != 登记_.合同当前状态关系
                    || 成员.关系.源节点 != 成员.源节点.编码
                    || 成员.关系.目标节点 != 成员.目标节点.编码
                    || !服务合同事实权威内部::活动于(成员.关系, G0))
                    return 失败(服务合同事实权威读取状态_v1::引用冲突);
                const auto* 投影 = 服务合同事实权威内部::查找属性投影(
                    成员.目标节点属性值, 成员.目标节点.编码,
                    登记_.合同状态载荷);
                服务合同状态事实_v1 状态事实;
                if (!投影 || !服务合同事实权威内部::解码状态(
                        投影->当前值事实, {成员.源节点.编码}, 状态事实)
                    || !状态表.emplace(成员.源节点.编码.值, 状态事实).second)
                    return 失败(服务合同事实权威读取状态_v1::集合不闭合);
            }

            服务合同完整集合读取结果_v1 结果;
            结果.状态 = 服务合同事实权威读取状态_v1::已读取;
            for (const auto& 成员 : 成员闭包->成员) {
                if (成员.关系.源节点 != 登记_.当前合同索引根
                    || 成员.关系.关系类型节点 != 登记_.当前合同成员关系
                    || 成员.关系.目标节点 != 成员.目标节点.编码
                    || !服务合同事实权威内部::活动于(成员.关系, G0))
                    return 失败(服务合同事实权威读取状态_v1::引用冲突);
                const auto* 投影 = 服务合同事实权威内部::查找属性投影(
                    成员.目标节点属性值, 成员.目标节点.编码, 登记_.合同载荷);
                服务合同事实_v1 合同;
                if (!投影 || !服务合同事实权威内部::解码合同(
                        投影->当前值事实, 成员.目标节点.编码, 合同)
                    || !合同引用闭合(成员, 合同, G0))
                    return 失败(服务合同事实权威读取状态_v1::引用冲突);
                if (合同.自我 != 请求.自我)
                    continue;
                const auto 状态 = 状态表.find(合同.身份.值.值);
                if (状态 == 状态表.end()
                    || 状态->second.状态 != 服务合同当前状态_v1::有效未满足
                    || 状态->second.已消费有效秒 > 合同.冻结有效总秒)
                    return 失败(服务合同事实权威读取状态_v1::集合不闭合);
                结果.完整合同事实组.push_back(std::move(合同));
                结果.当前状态事实组.push_back(状态->second);
            }
            规范排序(结果.完整合同事实组, 结果.当前状态事实组);
            服务合同完整集合见证_v1 见证;
            见证.结构登记版本 = 服务合同事实权威结构登记版本_v1;
            见证.声明成员数 = 结果.完整合同事实组.size();
            见证.G0 = G0;
            for (const auto& 合同 : 结果.完整合同事实组)
                见证.规范成员身份组.push_back(合同.身份);
            结果.完整集合见证 = std::move(见证);
            结果.本次正式读回截止 = G0;
            if (!读后守卫(G0) || !结果.成功())
                return 失败(!读后守卫(G0)
                    ? 服务合同事实权威读取状态_v1::当前性漂移
                    : 服务合同事实权威读取状态_v1::集合不闭合);
            return 结果;
        } catch (const std::bad_alloc&) {
            return 失败(服务合同事实权威读取状态_v1::资源失败);
        } catch (const std::length_error&) {
            return 失败(服务合同事实权威读取状态_v1::资源失败);
        } catch (...) {
            return 失败(服务合同事实权威读取状态_v1::内部错误);
        }
    }

    服务到期事件完整集合读取结果_v1
    读取到期未满足事件完整集合_v1(
        const 服务到期事件完整集合读取请求_v1& 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 失败 = [](服务合同事实权威读取状态_v1 状态) {
            服务到期事件完整集合读取结果_v1 结果;
            结果.状态 = 状态;
            return 结果;
        };
        try {
            if (!请求有效(请求.合同版本, 请求.请求头, 请求.自我))
                return 失败(服务合同事实权威读取状态_v1::入口拒绝);
            const auto G0 = 请求.请求头.期望事实代次;
            if (!读前守卫(G0))
                return 失败(服务合同事实权威读取状态_v1::当前性漂移);
#if defined(ARCH_INSTINCT_SERVICE_CONTRACT_FACT_AUTHORITY_VALIDATION)
            if (ARCH_资源失败一次_) {
                ARCH_资源失败一次_ = false;
                return 失败(服务合同事实权威读取状态_v1::资源失败);
            }
            注入漂移_已锁定();
#endif
            const auto 读取 = L1_.尝试读取所有者范围一致关系类型闭包投影(
                形成事件读取请求(G0));
            if (读取.状态 != L1所有者范围一致当前读取状态::成功)
                return 失败(服务合同事实权威内部::映射读取状态(读取.状态));
            if (读取.读取事实代次 != G0)
                return 失败(服务合同事实权威读取状态_v1::当前性漂移);
            if (!验证共同登记(读取, G0))
                return 失败(服务合同事实权威读取状态_v1::结构未登记);
            if (读取.关系类型闭包.size() != 2)
                return 失败(服务合同事实权威读取状态_v1::集合不闭合);
            const auto* 成员闭包 = 查找闭包(读取, 登记_.到期事件成员关系);
            const auto* 合同闭包 = 查找闭包(读取, 登记_.到期事件合同关系);
            if (!成员闭包 || !合同闭包)
                return 失败(服务合同事实权威读取状态_v1::集合不闭合);
            std::map<std::uint64_t, 服务合同事实_v1> 事件合同;
            for (const auto& 成员 : 合同闭包->成员) {
                const auto* 投影 = 服务合同事实权威内部::查找属性投影(
                    成员.目标节点属性值, 成员.目标节点.编码, 登记_.合同载荷);
                服务合同事实_v1 合同;
                if (!投影 || !服务合同事实权威内部::解码合同(
                        投影->当前值事实, 成员.目标节点.编码, 合同)
                    || !事件合同.emplace(成员.源节点.编码.值, 合同).second)
                    return 失败(服务合同事实权威读取状态_v1::引用冲突);
            }
            服务到期事件完整集合读取结果_v1 结果;
            结果.状态 = 服务合同事实权威读取状态_v1::已读取;
            for (const auto& 成员 : 成员闭包->成员) {
                if (成员.关系.源节点 != 登记_.到期事件索引根
                    || 成员.关系.关系类型节点 != 登记_.到期事件成员关系
                    || 成员.关系.目标节点 != 成员.目标节点.编码
                    || !服务合同事实权威内部::活动于(成员.关系, G0))
                    return 失败(服务合同事实权威读取状态_v1::引用冲突);
                const auto* 投影 = 服务合同事实权威内部::查找属性投影(
                    成员.目标节点属性值, 成员.目标节点.编码,
                    登记_.到期事件载荷);
                服务到期未满足事件事实_v1 事件;
                if (!投影 || !服务合同事实权威内部::解码事件(
                        投影->当前值事实, 成员.目标节点.编码, 事件))
                    return 失败(服务合同事实权威读取状态_v1::引用冲突);
                const auto 合同 = 事件合同.find(事件.身份.值.值);
                if (合同 == 事件合同.end() || 合同->second.身份 != 事件.合同
                    || 合同->second.自我 != 请求.自我
                    || 合同->second.需求 != 事件.需求
                    || 合同->second.提出者 != 事件.提出者
                    || !事件引用闭合(成员, 事件, G0))
                    return 失败(服务合同事实权威读取状态_v1::引用冲突);
                结果.完整事件事实组.push_back(std::move(事件));
            }
            std::sort(结果.完整事件事实组.begin(), 结果.完整事件事实组.end(),
                [](const auto& 左, const auto& 右) {
                    return 左.身份.值.值 < 右.身份.值.值;
                });
            服务到期事件完整集合见证_v1 见证;
            见证.结构登记版本 = 服务合同事实权威结构登记版本_v1;
            见证.声明成员数 = 结果.完整事件事实组.size();
            见证.G0 = G0;
            for (const auto& 事件 : 结果.完整事件事实组)
                见证.规范成员身份组.push_back(事件.身份);
            结果.完整集合见证 = std::move(见证);
            结果.本次正式读回截止 = G0;
            if (!读后守卫(G0) || !结果.成功())
                return 失败(!读后守卫(G0)
                    ? 服务合同事实权威读取状态_v1::当前性漂移
                    : 服务合同事实权威读取状态_v1::集合不闭合);
            return 结果;
        } catch (const std::bad_alloc&) {
            return 失败(服务合同事实权威读取状态_v1::资源失败);
        } catch (const std::length_error&) {
            return 失败(服务合同事实权威读取状态_v1::资源失败);
        } catch (...) {
            return 失败(服务合同事实权威读取状态_v1::内部错误);
        }
    }

    服务到期事件合同终态完整集合读取结果_v1
    读取到期未满足事件合同终态完整集合_v1(
        const 服务到期事件完整集合读取请求_v1& 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 失败 = [](服务合同事实权威读取状态_v1 状态) {
            服务到期事件合同终态完整集合读取结果_v1 结果;
            结果.状态 = 状态;
            return 结果;
        };
        try {
            if (!请求有效(请求.合同版本, 请求.请求头, 请求.自我))
                return 失败(服务合同事实权威读取状态_v1::入口拒绝);
            const auto G0 = 请求.请求头.期望事实代次;
            if (!读前守卫(G0))
                return 失败(服务合同事实权威读取状态_v1::当前性漂移);
#if defined(ARCH_INSTINCT_SERVICE_CONTRACT_FACT_AUTHORITY_VALIDATION)
            if (ARCH_资源失败一次_) {
                ARCH_资源失败一次_ = false;
                return 失败(服务合同事实权威读取状态_v1::资源失败);
            }
            注入漂移_已锁定();
#endif
            const auto 读取 = L1_.尝试读取所有者范围一致关系类型闭包投影(
                形成事件合同终态读取请求(G0));
            if (读取.状态 != L1所有者范围一致当前读取状态::成功)
                return 失败(服务合同事实权威内部::映射读取状态(读取.状态));
            if (读取.读取事实代次 != G0)
                return 失败(服务合同事实权威读取状态_v1::当前性漂移);
            if (!验证共同登记(读取, G0))
                return 失败(服务合同事实权威读取状态_v1::结构未登记);
            if (读取.关系类型闭包.size() != 3)
                return 失败(服务合同事实权威读取状态_v1::集合不闭合);
            const auto* 成员闭包 = 查找闭包(读取, 登记_.到期事件成员关系);
            const auto* 合同闭包 = 查找闭包(读取, 登记_.到期事件合同关系);
            const auto* 状态闭包 = 查找闭包(读取, 登记_.合同当前状态关系);
            if (!成员闭包 || !合同闭包 || !状态闭包)
                return 失败(服务合同事实权威读取状态_v1::集合不闭合);

            std::map<std::uint64_t, 服务合同事实_v1> 事件合同;
            for (const auto& 成员 : 合同闭包->成员) {
                if (成员.关系.关系类型节点 != 登记_.到期事件合同关系
                    || 成员.关系.源节点 != 成员.源节点.编码
                    || 成员.关系.目标节点 != 成员.目标节点.编码
                    || !服务合同事实权威内部::活动于(成员.关系, G0))
                    return 失败(服务合同事实权威读取状态_v1::引用冲突);
                const auto* 投影 = 服务合同事实权威内部::查找属性投影(
                    成员.目标节点属性值, 成员.目标节点.编码, 登记_.合同载荷);
                服务合同事实_v1 合同;
                if (!投影 || !服务合同事实权威内部::解码合同(
                        投影->当前值事实, 成员.目标节点.编码, 合同)
                    || !合同引用闭合(成员, 合同, G0)
                    || !事件合同.emplace(成员.源节点.编码.值, 合同).second)
                    return 失败(服务合同事实权威读取状态_v1::引用冲突);
            }
            if (事件合同.size() != 成员闭包->成员.size())
                return 失败(服务合同事实权威读取状态_v1::集合不闭合);

            std::map<std::uint64_t, 服务合同状态事实_v1> 合同状态;
            for (const auto& 成员 : 状态闭包->成员) {
                if (成员.关系.关系类型节点 != 登记_.合同当前状态关系
                    || 成员.关系.源节点 != 成员.源节点.编码
                    || 成员.关系.目标节点 != 成员.目标节点.编码
                    || !服务合同事实权威内部::活动于(成员.关系, G0))
                    return 失败(服务合同事实权威读取状态_v1::引用冲突);
                const auto* 投影 = 服务合同事实权威内部::查找属性投影(
                    成员.目标节点属性值, 成员.目标节点.编码,
                    登记_.合同状态载荷);
                服务合同状态事实_v1 状态;
                if (!投影 || !服务合同事实权威内部::解码状态(
                        投影->当前值事实, {成员.源节点.编码}, 状态)
                    || !合同状态.emplace(成员.源节点.编码.值, 状态).second)
                    return 失败(服务合同事实权威读取状态_v1::集合不闭合);
            }

            服务到期事件合同终态完整集合读取结果_v1 结果;
            结果.状态 = 服务合同事实权威读取状态_v1::已读取;
            for (const auto& 成员 : 成员闭包->成员) {
                if (成员.关系.源节点 != 登记_.到期事件索引根
                    || 成员.关系.关系类型节点 != 登记_.到期事件成员关系
                    || 成员.关系.目标节点 != 成员.目标节点.编码
                    || !服务合同事实权威内部::活动于(成员.关系, G0))
                    return 失败(服务合同事实权威读取状态_v1::引用冲突);
                const auto* 投影 = 服务合同事实权威内部::查找属性投影(
                    成员.目标节点属性值, 成员.目标节点.编码,
                    登记_.到期事件载荷);
                服务到期未满足事件事实_v1 事件;
                if (!投影 || !服务合同事实权威内部::解码事件(
                        投影->当前值事实, 成员.目标节点.编码, 事件)
                    || !事件引用闭合(成员, 事件, G0))
                    return 失败(服务合同事实权威读取状态_v1::引用冲突);
                const auto 合同 = 事件合同.find(事件.身份.值.值);
                if (合同 == 事件合同.end())
                    return 失败(服务合同事实权威读取状态_v1::集合不闭合);
                const auto 状态 = 合同状态.find(合同->second.身份.值.值);
                if (状态 == 合同状态.end())
                    return 失败(服务合同事实权威读取状态_v1::集合不闭合);
                服务到期事件合同终态投影_v1 完整投影{
                    std::move(事件), 合同->second, 状态->second};
                if (完整投影.原合同.自我 != 请求.自我
                    || !服务到期事件合同终态投影完整_v1(完整投影, G0))
                    return 失败(服务合同事实权威读取状态_v1::引用冲突);
                结果.完整投影组.push_back(std::move(完整投影));
            }
            std::sort(结果.完整投影组.begin(), 结果.完整投影组.end(),
                [](const auto& 左, const auto& 右) {
                    return 左.事件.身份.值.值 < 右.事件.身份.值.值;
                });
            服务到期事件合同终态完整集合见证_v1 见证;
            见证.结构登记版本 = 服务合同事实权威结构登记版本_v1;
            见证.声明成员数 = 结果.完整投影组.size();
            见证.G0 = G0;
            for (const auto& 投影 : 结果.完整投影组)
                见证.规范成员身份组.push_back(投影.事件.身份);
            结果.完整集合见证 = std::move(见证);
            结果.本次正式读回截止 = G0;
            const bool 当前 = 读后守卫(G0);
            if (!当前 || !结果.成功())
                return 失败(!当前
                    ? 服务合同事实权威读取状态_v1::当前性漂移
                    : 服务合同事实权威读取状态_v1::集合不闭合);
            return 结果;
        } catch (const std::bad_alloc&) {
            return 失败(服务合同事实权威读取状态_v1::资源失败);
        } catch (const std::length_error&) {
            return 失败(服务合同事实权威读取状态_v1::资源失败);
        } catch (...) {
            return 失败(服务合同事实权威读取状态_v1::内部错误);
        }
    }

    服务进展完整集合读取结果_v1
    读取当前服务合同关联进展完整集合_v1(
        const 服务进展完整集合读取请求_v1& 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 失败 = [](服务合同事实权威读取状态_v1 状态) {
            服务进展完整集合读取结果_v1 结果;
            结果.状态 = 状态;
            return 结果;
        };
        try {
            if (!进展请求有效(请求))
                return 失败(服务合同事实权威读取状态_v1::入口拒绝);
            const auto G0 = 请求.请求头.期望事实代次;
            if (!读前守卫(G0))
                return 失败(服务合同事实权威读取状态_v1::当前性漂移);
#if defined(ARCH_INSTINCT_SERVICE_CONTRACT_FACT_AUTHORITY_VALIDATION)
            if (ARCH_资源失败一次_) {
                ARCH_资源失败一次_ = false;
                return 失败(服务合同事实权威读取状态_v1::资源失败);
            }
            注入漂移_已锁定();
#endif
            const auto 读取 = L1_.尝试读取所有者范围一致关系类型闭包投影(
                形成进展读取请求(G0));
            if (读取.状态 != L1所有者范围一致当前读取状态::成功)
                return 失败(服务合同事实权威内部::映射读取状态(读取.状态));
            if (读取.读取事实代次 != G0)
                return 失败(服务合同事实权威读取状态_v1::当前性漂移);
            if (!验证进展共同登记(读取, G0))
                return 失败(服务合同事实权威读取状态_v1::结构未登记);
            if (读取.关系类型闭包.size() != 2)
                return 失败(服务合同事实权威读取状态_v1::集合不闭合);
            const auto* 成员闭包 = 查找闭包(读取, 进展登记_.当前进展成员关系);
            const auto* 合同闭包 = 查找闭包(读取, 进展登记_.进展服务合同关系);
            if (!成员闭包 || !合同闭包)
                return 失败(服务合同事实权威读取状态_v1::集合不闭合);

            std::map<std::uint64_t, 服务合同事实_v1> 进展合同;
            for (const auto& 成员 : 合同闭包->成员) {
                const auto* 投影 = 服务合同事实权威内部::查找属性投影(
                    成员.目标节点属性值, 成员.目标节点.编码, 登记_.合同载荷);
                服务合同事实_v1 合同;
                if (!投影 || !服务合同事实权威内部::解码合同(
                        投影->当前值事实, 成员.目标节点.编码, 合同)
                    || !进展合同.emplace(成员.源节点.编码.值, 合同).second)
                    return 失败(服务合同事实权威读取状态_v1::引用冲突);
            }

            服务进展完整集合读取结果_v1 结果;
            结果.状态 = 服务合同事实权威读取状态_v1::已读取;
            for (const auto& 成员 : 成员闭包->成员) {
                if (成员.关系.源节点 != 进展登记_.当前进展索引根
                    || 成员.关系.关系类型节点 != 进展登记_.当前进展成员关系
                    || 成员.关系.目标节点 != 成员.目标节点.编码
                    || !服务合同事实权威内部::活动于(成员.关系, G0))
                    return 失败(服务合同事实权威读取状态_v1::引用冲突);
                const auto* 投影 = 服务合同事实权威内部::查找属性投影(
                    成员.目标节点属性值, 成员.目标节点.编码,
                    进展登记_.进展载荷);
                服务合同关联进展事实_v1 进展;
                if (!投影 || !服务合同事实权威内部::解码进展(
                        投影->当前值事实, 成员.目标节点.编码, 进展)
                    || 进展.自我 != 请求.自我
                    || !进展引用闭合(成员, 进展, G0))
                    return 失败(服务合同事实权威读取状态_v1::引用冲突);
                const auto 合同 = 进展合同.find(进展.身份.值.值);
                if (合同 == 进展合同.end() || 合同->second.身份 != 进展.服务合同
                    || 合同->second.自我 != 进展.自我
                    || 合同->second.需求 != 进展.需求)
                    return 失败(服务合同事实权威读取状态_v1::引用冲突);
                结果.完整进展事实组.push_back(std::move(进展));
            }
            std::sort(结果.完整进展事实组.begin(), 结果.完整进展事实组.end(),
                [](const auto& 左, const auto& 右) {
                    return 左.身份.值.值 < 右.身份.值.值;
                });
            服务进展完整集合见证_v1 见证;
            见证.结构登记版本 = 服务进展事实结构登记版本_v1;
            见证.声明成员数 = 结果.完整进展事实组.size();
            见证.G0 = G0;
            for (const auto& 进展 : 结果.完整进展事实组)
                见证.规范成员身份组.push_back(进展.身份);
            结果.完整集合见证 = std::move(见证);
            结果.本次正式读回截止 = G0;
            if (!读后守卫(G0) || !结果.成功())
                return 失败(!读后守卫(G0)
                    ? 服务合同事实权威读取状态_v1::当前性漂移
                    : 服务合同事实权威读取状态_v1::集合不闭合);
            return 结果;
        } catch (const std::bad_alloc&) {
            return 失败(服务合同事实权威读取状态_v1::资源失败);
        } catch (const std::length_error&) {
            return 失败(服务合同事实权威读取状态_v1::资源失败);
        } catch (...) {
            return 失败(服务合同事实权威读取状态_v1::内部错误);
        }
    }

    服务准备完整集合读取结果_v1 读取当前服务准备完整集合_v1(
        const 服务准备完整集合读取请求_v1& 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 失败 = [](服务合同事实权威读取状态_v1 s) {
            服务准备完整集合读取结果_v1 r; r.状态 = s; return r; };
        try {
            if (!准备请求有效(请求)) return 失败(服务合同事实权威读取状态_v1::入口拒绝);
            const auto G0 = 请求.请求头.期望事实代次;
            if (!读前守卫(G0)) return 失败(服务合同事实权威读取状态_v1::当前性漂移);
#if defined(ARCH_INSTINCT_SERVICE_CONTRACT_FACT_AUTHORITY_VALIDATION)
            if (ARCH_资源失败一次_) { ARCH_资源失败一次_ = false;
                return 失败(服务合同事实权威读取状态_v1::资源失败); }
            注入漂移_已锁定();
#endif
            const auto 读 = L1_.尝试读取所有者范围一致关系类型闭包投影(
                形成准备读取请求(G0));
            if (读.状态 != L1所有者范围一致当前读取状态::成功)
                return 失败(服务合同事实权威内部::映射读取状态(读.状态));
            if (读.读取事实代次 != G0) return 失败(服务合同事实权威读取状态_v1::当前性漂移);
            if (!验证准备共同登记(读, G0)) return 失败(服务合同事实权威读取状态_v1::结构未登记);
            if (读.关系类型闭包.size() != 1) return 失败(服务合同事实权威读取状态_v1::集合不闭合);
            const auto* 闭包 = 查找闭包(读, 准备登记_.当前准备成员关系);
            if (!闭包) return 失败(服务合同事实权威读取状态_v1::集合不闭合);
            服务准备完整集合读取结果_v1 结果;
            结果.状态 = 服务合同事实权威读取状态_v1::已读取;
            for (const auto& 成员 : 闭包->成员) {
                if (成员.关系.源节点 != 准备登记_.当前准备索引根
                    || 成员.关系.关系类型节点 != 准备登记_.当前准备成员关系
                    || 成员.关系.目标节点 != 成员.目标节点.编码
                    || !服务合同事实权威内部::活动于(成员.关系, G0))
                    return 失败(服务合同事实权威读取状态_v1::引用冲突);
                const auto* 投影 = 服务合同事实权威内部::查找属性投影(
                    成员.目标节点属性值, 成员.目标节点.编码, 准备登记_.准备载荷);
                服务准备当前事实_v1 f;
                if (!投影 || !服务合同事实权威内部::解码准备(
                    投影->当前值事实, 成员.目标节点.编码, f) || f.自我 != 请求.自我)
                    return 失败(服务合同事实权威读取状态_v1::引用冲突);
                const auto u = [&](稳定编码 t) { return 服务合同事实权威内部::唯一关系目标(
                    成员.目标节点源关系组, t, 成员.目标节点.编码, G0); };
                const auto sourceType = std::holds_alternative<L2需求身份>(f.来源)
                    ? 准备登记_.准备来源需求关系 : 准备登记_.准备来源能力缺口关系;
                const auto otherType = std::holds_alternative<L2需求身份>(f.来源)
                    ? 准备登记_.准备来源能力缺口关系 : 准备登记_.准备来源需求关系;
                const auto sourceValue = std::holds_alternative<L2需求身份>(f.来源)
                    ? std::get<L2需求身份>(f.来源).值
                    : std::get<服务能力缺口身份_v1>(f.来源).值;
                if (u(准备登记_.准备自我关系) != std::optional<稳定编码>{f.自我.值}
                    || u(sourceType) != std::optional<稳定编码>{sourceValue}
                    || !可选关系目标闭合(成员.目标节点源关系组,
                        otherType, 成员.目标节点.编码, std::nullopt, G0)
                    || u(准备登记_.准备任务关系) != std::optional<稳定编码>{f.任务.值}
                    || u(准备登记_.准备方法关系) != std::optional<稳定编码>{f.方法.值}
                    || !可选关系目标闭合(成员.目标节点源关系组,
                        准备登记_.准备状态引用关系, 成员.目标节点.编码,
                        f.进展状态 ? std::optional<稳定编码>{f.进展状态->值} : std::nullopt, G0)
                    || !可选关系目标闭合(成员.目标节点源关系组,
                        准备登记_.准备动态引用关系, 成员.目标节点.编码,
                        f.进展动态 ? std::optional<稳定编码>{f.进展动态->值} : std::nullopt, G0)
                    || !可选关系目标闭合(成员.目标节点源关系组,
                        准备登记_.准备结果引用关系, 成员.目标节点.编码,
                        f.准备结果 ? std::optional<稳定编码>{f.准备结果->值} : std::nullopt, G0)
                    || !可选关系目标闭合(成员.目标节点源关系组,
                        准备登记_.准备验证引用关系, 成员.目标节点.编码,
                        f.完成验证 ? std::optional<稳定编码>{f.完成验证->值} : std::nullopt, G0))
                    return 失败(服务合同事实权威读取状态_v1::引用冲突);
                结果.完整准备事实组.push_back(std::move(f));
            }
            std::sort(结果.完整准备事实组.begin(), 结果.完整准备事实组.end(),
                [](const auto& a, const auto& b) { return a.身份.值.值 < b.身份.值.值; });
            服务准备完整集合见证_v1 w{服务准备事实结构登记版本_v1,
                结果.完整准备事实组.size(), {}, G0};
            for (const auto& f : 结果.完整准备事实组) w.规范成员身份组.push_back(f.身份);
            结果.完整集合见证 = std::move(w); 结果.本次正式读回截止 = G0;
            const bool 当前 = 读后守卫(G0);
            if (!当前 || !结果.成功()) return 失败(
                !当前 ? 服务合同事实权威读取状态_v1::当前性漂移
                    : 服务合同事实权威读取状态_v1::集合不闭合);
            return 结果;
        } catch (const std::bad_alloc&) { return 失败(服务合同事实权威读取状态_v1::资源失败); }
        catch (const std::length_error&) { return 失败(服务合同事实权威读取状态_v1::资源失败); }
        catch (...) { return 失败(服务合同事实权威读取状态_v1::内部错误); }
    }

    服务进展事实发布结果_v2 发布服务进展事实_v2(
        const 发布服务进展事实请求_v2& 请求) noexcept {
        using namespace 服务合同事实权威内部;
        const auto 失败 = [&](服务活动事实发布状态_v2 s,
            std::uint64_t g1 = 0) {
            服务进展事实发布结果_v2 r; r.状态 = s;
            r.幂等身份 = 请求.幂等身份;
            if (s == 服务活动事实发布状态_v2::已可能发布)
                r.首次提交事实代次 = g1;
            return r;
        };
        if (请求.合同版本 != 服务活动事实发布合同版本_v2
            || 请求.请求头.合同版本 != L2结构合同版本
            || 请求.请求头.期望事实代次 == 0 || 请求.幂等身份.值 == 0
            || (请求.预期当前事实 && !有效(*请求.预期当前事实))
            || !服务进展事实发布材料完整_v2(请求.材料))
            return 失败(服务活动事实发布状态_v2::入口拒绝);
        std::unique_lock<std::mutex> 锁(互斥体_);
        try {
            const auto 首次 = 写入端口_.读取首次写入材料(
                {L1所有者范围首次写入读取合同版本,
                    {请求.幂等身份.值}});
            if (首次.状态 == L1所有者范围读取状态::成功) {
                const auto 投影 = 投影进展发布材料(请求.材料);
                if (!首次发布请求同义(首次, 请求, 进展登记_v2_, 投影,
                        编码进展_v2))
                    return 失败(服务活动事实发布状态_v2::幂等冲突);
                const auto 身份 = 首次.首次写入结果
                    ? 查找映射(首次.首次写入结果->新编码映射,
                        发布事实节点本地键) : std::nullopt;
                const auto G1 = 首次.首次写入结果
                    ? 首次.首次写入结果->事实代次 : 0;
                const auto Gread = 当前事实代次_已锁定();
                if (!身份 || G1 == 0 || !Gread || *Gread < G1)
                    return 失败(服务活动事实发布状态_v2::内部错误);
                if (!首次发布历史形状同义_已锁定(首次,
                        请求.预期当前事实, 进展登记_v2_,
                        历史账登记_.进展v2历史索引根,
                        历史账登记_.进展v2历史成员关系, G1, *Gread))
                    return 失败(服务活动事实发布状态_v2::幂等冲突);
                if (历史覆盖登记_v2_.登记事实代次 != 0
                    && !v2精确重复历史形状同义_已锁定(*身份, G1,
                        进展登记_v2_.当前索引根, 进展登记_v2_.当前成员关系,
                        历史覆盖登记_v2_.进展历史索引根,
                        历史覆盖登记_v2_.进展历史成员关系, *Gread))
                    return 失败(服务活动事实发布状态_v2::幂等冲突);
                auto f = 读取进展首次历史闭包_已锁定(*身份, G1, *Gread);
                if (!f || !读后守卫(*Gread))
                    return 失败(服务活动事实发布状态_v2::内部错误);
                服务进展事实发布结果_v2 r;
                r.状态 = 服务活动事实发布状态_v2::精确重复;
                r.幂等身份 = 请求.幂等身份; r.事实 = std::move(f);
                r.首次提交事实代次 = G1; r.本次正式读回截止 = *Gread;
                return r.成功() ? r : 失败(服务活动事实发布状态_v2::内部错误);
            }
            if (首次.状态 != L1所有者范围读取状态::未找到)
                return 失败(首次.状态 == L1所有者范围读取状态::许可拒绝
                    ? 服务活动事实发布状态_v2::许可拒绝
                    : 首次.状态 == L1所有者范围读取状态::资源失败
                        ? 服务活动事实发布状态_v2::资源失败
                        : 服务活动事实发布状态_v2::内部错误);

            锁.unlock();
            const auto 合同组 = 读取当前有效未满足服务合同完整集合_v1({
                服务合同事实权威合同版本_v1, 请求.请求头, 请求.材料.自我});
            锁.lock();
            if (!读前守卫(请求.请求头.期望事实代次))
                return 失败(服务活动事实发布状态_v2::当前性漂移);
            if (!合同组.成功())
                return 失败(合同组.状态 == 服务合同事实权威读取状态_v1::当前性漂移
                    ? 服务活动事实发布状态_v2::当前性漂移
                    : 合同组.状态 == 服务合同事实权威读取状态_v1::结构未登记
                        ? 服务活动事实发布状态_v2::结构未登记
                        : 合同组.状态 == 服务合同事实权威读取状态_v1::资源失败
                            ? 服务活动事实发布状态_v2::资源失败
                            : 服务活动事实发布状态_v2::引用冲突);
            const auto 合同 = std::find_if(合同组.完整合同事实组.begin(),
                合同组.完整合同事实组.end(), [&](const auto& x) {
                    return x.身份 == 请求.材料.服务合同; });
            if (合同 == 合同组.完整合同事实组.end()
                || 合同->自我 != 请求.材料.自我
                || 合同->需求 != 请求.材料.需求)
                return 失败(服务活动事实发布状态_v2::引用冲突);

            const auto 当前组 = 读取当前服务合同关联进展完整集合_v2_已锁定({
                服务进展事实扩展合同版本_v2, 请求.请求头, 请求.材料.自我});
            if (!当前组.成功())
                return 失败(映射v2读取到发布状态(当前组.状态));
            const 服务合同关联进展事实_v2* 旧 = nullptr;
            for (const auto& f : 当前组.完整进展事实组)
                if (同一服务进展活动流_v2(请求.材料, f)) {
                    if (旧) return 失败(服务活动事实发布状态_v2::集合不闭合);
                    旧 = &f;
                }
            if ((!旧 && 请求.预期当前事实)
                || (旧 && (!请求.预期当前事实 || 旧->身份 != *请求.预期当前事实)))
                return 失败(服务活动事实发布状态_v2::当前事实冲突);
            std::vector<稳定编码> 退出;
            if (旧) {
                auto x = 形成版本化退出组_已锁定(
                    旧->身份.值, 旧->形成事实代次, 进展登记_v2_,
                    历史账登记_.进展v2历史索引根,
                    历史账登记_.进展v2历史成员关系,
                    历史覆盖登记_v2_.进展历史索引根,
                    历史覆盖登记_v2_.进展历史成员关系,
                    请求.请求头.期望事实代次,
                    形成进展读取请求_v2(请求.请求头.期望事实代次));
                if (!x) return 失败(服务活动事实发布状态_v2::集合不闭合);
                退出 = std::move(*x);
            }
            const auto 写 = 写入端口_.提交所有者范围中性写集(
                形成进展发布写集(
                    请求, 进展登记_v2_, 历史账登记_, 历史覆盖登记_v2_,
                    std::move(退出)));
            if (写.状态 != L1所有者范围写入状态::成功
                && 写.状态 != L1所有者范围写入状态::精确重复)
                return 失败(映射写入到发布状态(写.状态));
#if defined(ARCH_INSTINCT_SERVICE_CONTRACT_FACT_AUTHORITY_VALIDATION)
            if (ARCH_发布后读回失败一次_) {
                ARCH_发布后读回失败一次_ = false;
                return 失败(
                    服务活动事实发布状态_v2::已可能发布, 写.事实代次);
            }
#endif
            const auto 身份 = 查找映射(写.新编码映射, 发布事实节点本地键);
            const auto Gread = 当前事实代次_已锁定();
            if (!身份 || 写.事实代次 == 0 || !Gread)
                return 失败(服务活动事实发布状态_v2::已可能发布, 写.事实代次);
            const auto 读回 = 读取当前服务合同关联进展完整集合_v2_已锁定({
                服务进展事实扩展合同版本_v2,
                {L2结构合同版本, *Gread}, 请求.材料.自我});
            if (!读回.成功())
                return 失败(服务活动事实发布状态_v2::已可能发布, 写.事实代次);
            const auto it = std::find_if(读回.完整进展事实组.begin(),
                读回.完整进展事实组.end(), [&](const auto& f) { return f.身份.值 == *身份; });
            if (it == 读回.完整进展事实组.end()
                || (旧 && std::any_of(读回.完整进展事实组.begin(),
                    读回.完整进展事实组.end(), [&](const auto& f) {
                        return f.身份 == 旧->身份; })))
                return 失败(服务活动事实发布状态_v2::已可能发布, 写.事实代次);
            服务进展事实发布结果_v2 r;
            r.状态 = 写.状态 == L1所有者范围写入状态::成功
                ? 服务活动事实发布状态_v2::已发布
                : 服务活动事实发布状态_v2::精确重复;
            r.幂等身份 = 请求.幂等身份; r.事实 = *it;
            r.首次提交事实代次 = 写.事实代次; r.本次正式读回截止 = *Gread;
            return r.成功() ? r
                : 失败(服务活动事实发布状态_v2::已可能发布, 写.事实代次);
        } catch (const std::bad_alloc&) {
            return 失败(服务活动事实发布状态_v2::资源失败);
        } catch (const std::length_error&) {
            return 失败(服务活动事实发布状态_v2::资源失败);
        } catch (...) {
            return 失败(服务活动事实发布状态_v2::内部错误);
        }
    }

    服务进展完整集合读取结果_v2
    读取当前服务合同关联进展完整集合_v2(
        const 服务进展完整集合读取请求_v2& 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        return 读取当前服务合同关联进展完整集合_v2_已锁定(请求);
    }

private:
    服务进展完整集合读取结果_v2
    读取当前服务合同关联进展完整集合_v2_已锁定(
        const 服务进展完整集合读取请求_v2& 请求) const noexcept {
        const auto 失败 = [](服务合同事实权威读取状态_v2 s) {
            服务进展完整集合读取结果_v2 r; r.状态 = s; return r; };
        try {
            if (!进展请求有效(请求))
                return 失败(服务合同事实权威读取状态_v2::入口拒绝);
            const auto G0 = 请求.请求头.期望事实代次;
            if (!读前守卫(G0))
                return 失败(服务合同事实权威读取状态_v2::当前性漂移);
#if defined(ARCH_INSTINCT_SERVICE_CONTRACT_FACT_AUTHORITY_VALIDATION)
            if (ARCH_资源失败一次_) { ARCH_资源失败一次_ = false;
                return 失败(服务合同事实权威读取状态_v2::资源失败); }
            注入漂移_已锁定();
#endif
            const auto 读 = L1_.尝试读取所有者范围一致关系类型闭包投影(
                形成进展读取请求_v2(G0));
            if (读.状态 != L1所有者范围一致当前读取状态::成功)
                return 失败(服务合同事实权威内部::映射读取状态(读.状态));
            if (读.读取事实代次 != G0)
                return 失败(服务合同事实权威读取状态_v2::当前性漂移);
            if (!验证v2共同登记(读, 进展登记_v2_,
                    服务进展事实结构登记版本_v2, G0))
                return 失败(服务合同事实权威读取状态_v2::结构未登记);
            if (读.关系类型闭包.size() != 1)
                return 失败(服务合同事实权威读取状态_v2::集合不闭合);
            const auto* 闭包 = 查找闭包(读, 进展登记_v2_.当前成员关系);
            if (!闭包) return 失败(服务合同事实权威读取状态_v2::集合不闭合);
            服务进展完整集合读取结果_v2 结果;
            结果.状态 = 服务合同事实权威读取状态_v2::已读取;
            for (const auto& 成员 : 闭包->成员) {
                if (成员.关系.源节点 != 进展登记_v2_.当前索引根
                    || 成员.关系.关系类型节点 != 进展登记_v2_.当前成员关系
                    || 成员.关系.目标节点 != 成员.目标节点.编码
                    || !服务合同事实权威内部::活动于(成员.关系, G0))
                    return 失败(服务合同事实权威读取状态_v2::引用冲突);
                const auto* p = 服务合同事实权威内部::查找属性投影(
                    成员.目标节点属性值, 成员.目标节点.编码, 进展登记_v2_.载荷);
                服务合同关联进展事实_v2 f;
                if (!p || !服务合同事实权威内部::解码进展_v2(
                        p->当前值事实, 成员.目标节点.编码, f))
                    return 失败(服务合同事实权威读取状态_v2::引用冲突);
                const auto u = [&](稳定编码 t) { return
                    服务合同事实权威内部::唯一关系目标(
                        成员.目标节点源关系组, t, 成员.目标节点.编码, G0); };
                if (u(进展登记_v2_.服务合同关系)
                        != std::optional<稳定编码>{f.服务合同.值}
                    || u(进展登记_v2_.自我关系)
                        != std::optional<稳定编码>{f.自我.值}
                    || u(进展登记_v2_.需求关系)
                        != std::optional<稳定编码>{f.需求.值}
                    || u(进展登记_v2_.任务关系)
                        != std::optional<稳定编码>{f.任务.值}
                    || u(进展登记_v2_.方法关系)
                        != std::optional<稳定编码>{f.方法.值}
                    || u(进展登记_v2_.正式选择关系)
                        != std::optional<稳定编码>{f.执行绑定.正式选择.值}
                    || u(进展登记_v2_.冻结材料关系)
                        != std::optional<稳定编码>{f.执行绑定.执行冻结材料.值}
                    || u(进展登记_v2_.实例方法关系)
                        != std::optional<稳定编码>{f.执行绑定.实例方法.值}
                    || !可选关系目标闭合(成员.目标节点源关系组,
                        进展登记_v2_.状态引用关系, 成员.目标节点.编码,
                        f.进展状态 ? std::optional<稳定编码>{f.进展状态->值}
                                    : std::nullopt, G0)
                    || !可选关系目标闭合(成员.目标节点源关系组,
                        进展登记_v2_.动态引用关系, 成员.目标节点.编码,
                        f.进展动态 ? std::optional<稳定编码>{f.进展动态->值}
                                    : std::nullopt, G0))
                    return 失败(服务合同事实权威读取状态_v2::引用冲突);
                if (f.自我 != 请求.自我)
                    continue;
                结果.完整进展事实组.push_back(std::move(f));
            }
            std::sort(结果.完整进展事实组.begin(), 结果.完整进展事实组.end(),
                [](const auto& a, const auto& b) { return a.身份.值.值 < b.身份.值.值; });
            服务进展完整集合见证_v2 w{服务进展事实结构登记版本_v2,
                结果.完整进展事实组.size(), {}, G0};
            for (const auto& f : 结果.完整进展事实组) w.规范成员身份组.push_back(f.身份);
            结果.完整集合见证 = std::move(w); 结果.本次正式读回截止 = G0;
            const bool 当前 = 读后守卫(G0);
            if (!当前 || !结果.成功()) return 失败(!当前
                ? 服务合同事实权威读取状态_v2::当前性漂移
                : 服务合同事实权威读取状态_v2::集合不闭合);
            return 结果;
        } catch (const std::bad_alloc&) { return 失败(服务合同事实权威读取状态_v2::资源失败); }
        catch (const std::length_error&) { return 失败(服务合同事实权威读取状态_v2::资源失败); }
        catch (...) { return 失败(服务合同事实权威读取状态_v2::内部错误); }
    }

public:
    服务准备事实发布结果_v2 发布服务准备事实_v2(
        const 发布服务准备事实请求_v2& 请求) noexcept {
        using namespace 服务合同事实权威内部;
        const auto 失败 = [&](服务活动事实发布状态_v2 s,
            std::uint64_t g1 = 0) {
            服务准备事实发布结果_v2 r; r.状态 = s;
            r.幂等身份 = 请求.幂等身份;
            if (s == 服务活动事实发布状态_v2::已可能发布)
                r.首次提交事实代次 = g1;
            return r;
        };
        if (请求.合同版本 != 服务活动事实发布合同版本_v2
            || 请求.请求头.合同版本 != L2结构合同版本
            || 请求.请求头.期望事实代次 == 0 || 请求.幂等身份.值 == 0
            || (请求.预期当前事实 && !有效(*请求.预期当前事实))
            || !服务准备事实发布材料完整_v2(请求.材料))
            return 失败(服务活动事实发布状态_v2::入口拒绝);
        std::lock_guard<std::mutex> 锁(互斥体_);
        try {
            const auto 首次 = 写入端口_.读取首次写入材料(
                {L1所有者范围首次写入读取合同版本,
                    {请求.幂等身份.值}});
            if (首次.状态 == L1所有者范围读取状态::成功) {
                const auto 投影 = 投影准备发布材料(请求.材料);
                if (!首次发布请求同义(首次, 请求, 准备登记_v2_, 投影,
                        编码准备_v2))
                    return 失败(服务活动事实发布状态_v2::幂等冲突);
                const auto 身份 = 首次.首次写入结果
                    ? 查找映射(首次.首次写入结果->新编码映射,
                        发布事实节点本地键) : std::nullopt;
                const auto G1 = 首次.首次写入结果
                    ? 首次.首次写入结果->事实代次 : 0;
                const auto Gread = 当前事实代次_已锁定();
                if (!身份 || G1 == 0 || !Gread || *Gread < G1)
                    return 失败(服务活动事实发布状态_v2::内部错误);
                if (!首次发布历史形状同义_已锁定(首次,
                        请求.预期当前事实, 准备登记_v2_,
                        历史账登记_.准备v2历史索引根,
                        历史账登记_.准备v2历史成员关系, G1, *Gread))
                    return 失败(服务活动事实发布状态_v2::幂等冲突);
                if (历史覆盖登记_v2_.登记事实代次 != 0
                    && !v2精确重复历史形状同义_已锁定(*身份, G1,
                        准备登记_v2_.当前索引根, 准备登记_v2_.当前成员关系,
                        历史覆盖登记_v2_.准备历史索引根,
                        历史覆盖登记_v2_.准备历史成员关系, *Gread))
                    return 失败(服务活动事实发布状态_v2::幂等冲突);
                auto f = 读取准备首次历史闭包_已锁定(*身份, G1, *Gread);
                if (!f || !读后守卫(*Gread))
                    return 失败(服务活动事实发布状态_v2::内部错误);
                服务准备事实发布结果_v2 r;
                r.状态 = 服务活动事实发布状态_v2::精确重复;
                r.幂等身份 = 请求.幂等身份; r.事实 = std::move(f);
                r.首次提交事实代次 = G1; r.本次正式读回截止 = *Gread;
                return r.成功() ? r : 失败(服务活动事实发布状态_v2::内部错误);
            }
            if (首次.状态 != L1所有者范围读取状态::未找到)
                return 失败(首次.状态 == L1所有者范围读取状态::许可拒绝
                    ? 服务活动事实发布状态_v2::许可拒绝
                    : 首次.状态 == L1所有者范围读取状态::资源失败
                        ? 服务活动事实发布状态_v2::资源失败
                        : 服务活动事实发布状态_v2::内部错误);
            if (!读前守卫(请求.请求头.期望事实代次))
                return 失败(服务活动事实发布状态_v2::当前性漂移);
            const auto 当前组 = 读取当前服务准备完整集合_v2_已锁定({
                服务准备事实扩展合同版本_v2, 请求.请求头, 请求.材料.自我});
            if (!当前组.成功())
                return 失败(映射v2读取到发布状态(当前组.状态));
            const 服务准备当前事实_v2* 旧 = nullptr;
            for (const auto& f : 当前组.完整准备事实组)
                if (同一服务准备活动流_v2(请求.材料, f)) {
                    if (旧) return 失败(服务活动事实发布状态_v2::集合不闭合);
                    旧 = &f;
                }
            if ((!旧 && 请求.预期当前事实)
                || (旧 && (!请求.预期当前事实 || 旧->身份 != *请求.预期当前事实)))
                return 失败(服务活动事实发布状态_v2::当前事实冲突);
            std::vector<稳定编码> 退出;
            if (旧) {
                auto x = 形成版本化退出组_已锁定(
                    旧->身份.值, 旧->形成事实代次, 准备登记_v2_,
                    历史账登记_.准备v2历史索引根,
                    历史账登记_.准备v2历史成员关系,
                    历史覆盖登记_v2_.准备历史索引根,
                    历史覆盖登记_v2_.准备历史成员关系,
                    请求.请求头.期望事实代次,
                    形成准备读取请求_v2(请求.请求头.期望事实代次));
                if (!x) return 失败(服务活动事实发布状态_v2::集合不闭合);
                退出 = std::move(*x);
            }
            const auto 写 = 写入端口_.提交所有者范围中性写集(
                形成准备发布写集(
                    请求, 准备登记_v2_, 历史账登记_, 历史覆盖登记_v2_,
                    std::move(退出)));
            if (写.状态 != L1所有者范围写入状态::成功
                && 写.状态 != L1所有者范围写入状态::精确重复)
                return 失败(映射写入到发布状态(写.状态));
#if defined(ARCH_INSTINCT_SERVICE_CONTRACT_FACT_AUTHORITY_VALIDATION)
            if (ARCH_发布后读回失败一次_) {
                ARCH_发布后读回失败一次_ = false;
                return 失败(
                    服务活动事实发布状态_v2::已可能发布, 写.事实代次);
            }
#endif
            const auto 身份 = 查找映射(写.新编码映射, 发布事实节点本地键);
            const auto Gread = 当前事实代次_已锁定();
            if (!身份 || 写.事实代次 == 0 || !Gread)
                return 失败(服务活动事实发布状态_v2::已可能发布, 写.事实代次);
            const auto 读回 = 读取当前服务准备完整集合_v2_已锁定({
                服务准备事实扩展合同版本_v2,
                {L2结构合同版本, *Gread}, 请求.材料.自我});
            if (!读回.成功())
                return 失败(服务活动事实发布状态_v2::已可能发布, 写.事实代次);
            const auto it = std::find_if(读回.完整准备事实组.begin(),
                读回.完整准备事实组.end(), [&](const auto& f) { return f.身份.值 == *身份; });
            if (it == 读回.完整准备事实组.end()
                || (旧 && std::any_of(读回.完整准备事实组.begin(),
                    读回.完整准备事实组.end(), [&](const auto& f) {
                        return f.身份 == 旧->身份; })))
                return 失败(服务活动事实发布状态_v2::已可能发布, 写.事实代次);
            服务准备事实发布结果_v2 r;
            r.状态 = 写.状态 == L1所有者范围写入状态::成功
                ? 服务活动事实发布状态_v2::已发布
                : 服务活动事实发布状态_v2::精确重复;
            r.幂等身份 = 请求.幂等身份; r.事实 = *it;
            r.首次提交事实代次 = 写.事实代次; r.本次正式读回截止 = *Gread;
            return r.成功() ? r
                : 失败(服务活动事实发布状态_v2::已可能发布, 写.事实代次);
        } catch (const std::bad_alloc&) {
            return 失败(服务活动事实发布状态_v2::资源失败);
        } catch (const std::length_error&) {
            return 失败(服务活动事实发布状态_v2::资源失败);
        } catch (...) {
            return 失败(服务活动事实发布状态_v2::内部错误);
        }
    }

    服务准备完整集合读取结果_v2 读取当前服务准备完整集合_v2(
        const 服务准备完整集合读取请求_v2& 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        return 读取当前服务准备完整集合_v2_已锁定(请求);
    }

    服务维护历史覆盖起点读取结果_v1
    读取服务维护历史事实账覆盖起点_v1(
        const 服务维护历史覆盖起点读取请求_v1& 请求) const noexcept {
        using 状态 = 服务维护历史覆盖起点读取状态_v1;
        using namespace 服务合同事实权威内部;
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 失败 = [](状态 s) {
            服务维护历史覆盖起点读取结果_v1 r;
            r.状态 = s;
            return r;
        };
        try {
            const auto G0 = 请求.请求头.期望事实代次;
            if (请求.合同版本 != 服务维护历史覆盖起点合同版本_v1
                || 请求.请求头.合同版本 != L2结构合同版本
                || G0 == 0 || !有效(请求.自我.值))
                return 失败(状态::入口拒绝);
            if (历史账登记_.登记事实代次 == 0)
                return 失败(状态::历史账未登记);
            if (历史账登记_.登记事实代次 > G0 || !读前守卫(G0))
                return 失败(状态::当前性漂移);
#if defined(ARCH_INSTINCT_SERVICE_CONTRACT_FACT_AUTHORITY_VALIDATION)
            if (ARCH_资源失败一次_) {
                ARCH_资源失败一次_ = false;
                return 失败(状态::资源失败);
            }
            注入漂移_已锁定();
#endif
            const auto 读 = L1_.尝试读取所有者范围一致关系类型闭包投影(
                形成历史账读取请求(G0));
            if (读.状态 != L1所有者范围一致当前读取状态::成功) {
                if (读.状态 == L1所有者范围一致当前读取状态::事实代次漂移)
                    return 失败(状态::当前性漂移);
                if (读.状态 == L1所有者范围一致当前读取状态::资源失败)
                    return 失败(状态::资源失败);
                return 失败(状态::内部错误);
            }
            if (读.读取事实代次 != G0)
                return 失败(状态::当前性漂移);
            if (!验证v2共同登记(读, 历史账登记_,
                    服务维护历史事实账结构登记版本_v1, G0))
                return 失败(状态::历史账未登记);
            if (!读后守卫(G0)) return 失败(状态::当前性漂移);
            服务维护历史覆盖起点读取结果_v1 r;
            r.状态 = 状态::已读取;
            r.快照 = 服务维护历史覆盖起点快照_v1{
                服务维护历史事实账结构登记版本_v1,
                历史账登记_.登记事实代次, 请求.自我, G0};
            r.本次正式读回截止 = G0;
            return r.成功() ? r : 失败(状态::引用冲突);
        } catch (const std::bad_alloc&) {
            return 失败(状态::资源失败);
        } catch (const std::length_error&) {
            return 失败(状态::资源失败);
        } catch (...) {
            return 失败(状态::内部错误);
        }
    }

    服务维护历史事实范围读取结果_v1
    按事实代次范围读取服务维护历史事实完整组_v1(
        const 服务维护历史事实范围读取请求_v1& 请求) const noexcept {
        using namespace 服务合同事实权威内部;
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 失败 = [](服务维护历史事实范围读取状态_v1 s) {
            服务维护历史事实范围读取结果_v1 r;
            r.状态 = s;
            return r;
        };
        try {
            const auto G0 = 请求.请求头.期望事实代次;
            if (请求.合同版本 != 服务维护历史事实账合同版本_v1
                || 请求.请求头.合同版本 != L2结构合同版本
                || G0 == 0 || !有效(请求.自我.值)
                || 请求.排除起始事实代次 == 0
                || 请求.排除起始事实代次 > G0 || 请求.数量预算 == 0)
                return 失败(服务维护历史事实范围读取状态_v1::入口拒绝);
            if (历史账登记_.登记事实代次 == 0)
                return 失败(服务维护历史事实范围读取状态_v1::历史账未登记);
            if (请求.排除起始事实代次 < 历史账登记_.登记事实代次)
                return 失败(服务维护历史事实范围读取状态_v1::覆盖边界不可用);
            if (!读前守卫(G0))
                return 失败(服务维护历史事实范围读取状态_v1::当前性漂移);
#if defined(ARCH_INSTINCT_SERVICE_CONTRACT_FACT_AUTHORITY_VALIDATION)
            if (ARCH_资源失败一次_) {
                ARCH_资源失败一次_ = false;
                return 失败(服务维护历史事实范围读取状态_v1::资源失败);
            }
            注入漂移_已锁定();
#endif
            const auto 读 = L1_.尝试读取所有者范围一致关系类型闭包投影(
                形成历史账读取请求(G0));
            if (读.状态 != L1所有者范围一致当前读取状态::成功) {
                if (读.状态 == L1所有者范围一致当前读取状态::事实代次漂移)
                    return 失败(服务维护历史事实范围读取状态_v1::当前性漂移);
                if (读.状态 == L1所有者范围一致当前读取状态::资源失败)
                    return 失败(服务维护历史事实范围读取状态_v1::资源失败);
                return 失败(服务维护历史事实范围读取状态_v1::内部错误);
            }
            if (读.读取事实代次 != G0)
                return 失败(服务维护历史事实范围读取状态_v1::当前性漂移);
            if (!验证v2共同登记(读, 历史账登记_,
                    服务维护历史事实账结构登记版本_v1, G0))
                return 失败(服务维护历史事实范围读取状态_v1::历史账未登记);
            if (读.关系类型闭包.size() != 7)
                return 失败(服务维护历史事实范围读取状态_v1::集合不闭合);
            const auto* 合同闭包 = 查找闭包(读, 历史账登记_.合同历史成员关系);
            const auto* 状态闭包 = 查找闭包(读, 历史账登记_.合同状态历史成员关系);
            const auto* 事件闭包 = 查找闭包(读, 历史账登记_.到期事件历史成员关系);
            const auto* 进展闭包 = 查找闭包(读, 历史账登记_.进展v2历史成员关系);
            const auto* 准备闭包 = 查找闭包(读, 历史账登记_.准备v2历史成员关系);
            const auto* 当前进展闭包 = 查找闭包(读, 进展登记_v2_.当前成员关系);
            const auto* 当前准备闭包 = 查找闭包(读, 准备登记_v2_.当前成员关系);
            if (!合同闭包 || !状态闭包 || !事件闭包 || !进展闭包
                || !准备闭包 || !当前进展闭包 || !当前准备闭包)
                return 失败(服务维护历史事实范围读取状态_v1::集合不闭合);

            const auto 成员基础完整 = [&](const auto& m, 稳定编码 根,
                稳定编码 类型) noexcept {
                return m.关系.源节点 == 根
                    && m.关系.目标节点 == m.目标节点.编码
                    && m.关系.关系类型节点 == 类型
                    && m.关系.角色或顺序 == 1
                    && m.关系.写入所有者 == 写入端口_.所有者身份()
                    && m.目标节点.写入所有者 == 写入端口_.所有者身份()
                    && m.关系.创建事实代次 == m.目标节点.创建事实代次
                    && m.关系.创建事实代次 >= 历史账登记_.登记事实代次
                    && !m.关系.退出事实代次
                    && 活动于(m.关系, G0) && 活动于(m.目标节点, G0);
            };
            std::map<std::uint64_t, L2存在身份> 合同自我表;
            服务维护历史事实范围读取结果_v1 结果;
            结果.状态 = 服务维护历史事实范围读取状态_v1::已读取;

            for (const auto& m : 合同闭包->成员) {
                if (!成员基础完整(m, 历史账登记_.合同历史索引根,
                        历史账登记_.合同历史成员关系))
                    return 失败(服务维护历史事实范围读取状态_v1::集合不闭合);
                const auto* p = 查找属性投影(
                    m.目标节点属性值, m.目标节点.编码, 登记_.合同载荷);
                服务合同事实_v1 f;
                if (!p || !解码合同(p->当前值事实, m.目标节点.编码, f)
                    || !合同历史引用闭合_已锁定(f, G0)
                    || !合同自我表.emplace(f.身份.值.值, f.自我).second)
                    return 失败(服务维护历史事实范围读取状态_v1::引用冲突);
                const auto 当前 = 历史成员关系_已锁定(f.身份.值,
                    登记_.当前合同索引根, 登记_.当前合同成员关系,
                    f.形成事实代次, G0);
                if (!当前 || (当前->退出事实代次
                        && (*当前->退出事实代次 <= f.形成事实代次
                            || *当前->退出事实代次 > G0)))
                    return 失败(服务维护历史事实范围读取状态_v1::集合不闭合);
                if (f.自我 == 请求.自我
                    && f.形成事实代次 > 请求.排除起始事实代次)
                    结果.合同变化组.push_back({f, 当前->退出事实代次});
            }
            for (const auto& m : 状态闭包->成员) {
                if (!成员基础完整(m, 历史账登记_.合同状态历史索引根,
                        历史账登记_.合同状态历史成员关系))
                    return 失败(服务维护历史事实范围读取状态_v1::集合不闭合);
                const auto G1 = m.目标节点.创建事实代次;
                const auto 当前 = 读取状态当前关系_已锁定(
                    m.目标节点.编码, G1, G0);
                if (!当前 || (当前->退出事实代次
                        && (*当前->退出事实代次 <= G1
                            || *当前->退出事实代次 > G0)))
                    return 失败(服务维护历史事实范围读取状态_v1::集合不闭合);
                const auto* p = 查找属性投影(m.目标节点属性值,
                    m.目标节点.编码, 登记_.合同状态载荷);
                服务合同状态事实_v1 f;
                if (!p || !解码状态(p->当前值事实,
                        {当前->源节点}, f))
                    return 失败(服务维护历史事实范围读取状态_v1::引用冲突);
                const auto self = 合同自我表.find(f.合同.值.值);
                if (self == 合同自我表.end())
                    return 失败(服务维护历史事实范围读取状态_v1::引用冲突);
                if (self->second == 请求.自我
                    && f.形成事实代次 > 请求.排除起始事实代次)
                    结果.合同状态变化组.push_back({f, 当前->退出事实代次});
            }
            for (const auto& m : 事件闭包->成员) {
                if (!成员基础完整(m, 历史账登记_.到期事件历史索引根,
                        历史账登记_.到期事件历史成员关系))
                    return 失败(服务维护历史事实范围读取状态_v1::集合不闭合);
                const auto* p = 查找属性投影(m.目标节点属性值,
                    m.目标节点.编码, 登记_.到期事件载荷);
                服务到期未满足事件事实_v1 f;
                if (!p || !解码事件(p->当前值事实, m.目标节点.编码, f)
                    || !到期事件历史引用闭合_已锁定(f, G0))
                    return 失败(服务维护历史事实范围读取状态_v1::引用冲突);
                const auto self = 合同自我表.find(f.合同.值.值);
                if (self == 合同自我表.end())
                    return 失败(服务维护历史事实范围读取状态_v1::引用冲突);
                if (self->second == 请求.自我
                    && f.形成事实代次 > 请求.排除起始事实代次)
                    结果.到期事件变化组.push_back({f, std::nullopt});
            }
            for (const auto& m : 进展闭包->成员) {
                if (!成员基础完整(m, 历史账登记_.进展v2历史索引根,
                        历史账登记_.进展v2历史成员关系))
                    return 失败(服务维护历史事实范围读取状态_v1::集合不闭合);
                const auto G1 = m.目标节点.创建事实代次;
                const auto f = 读取进展首次历史闭包_已锁定(
                    m.目标节点.编码, G1, G0);
                const auto 当前 = 历史成员关系_已锁定(m.目标节点.编码,
                    进展登记_v2_.当前索引根, 进展登记_v2_.当前成员关系,
                    G1, G0);
                if (!f || !当前 || (当前->退出事实代次
                        && (*当前->退出事实代次 <= G1
                            || *当前->退出事实代次 > G0)))
                    return 失败(服务维护历史事实范围读取状态_v1::引用冲突);
                if (f->自我 == 请求.自我 && G1 > 请求.排除起始事实代次)
                    结果.服务进展变化组.push_back({*f, 当前->退出事实代次});
            }
            for (const auto& m : 准备闭包->成员) {
                if (!成员基础完整(m, 历史账登记_.准备v2历史索引根,
                        历史账登记_.准备v2历史成员关系))
                    return 失败(服务维护历史事实范围读取状态_v1::集合不闭合);
                const auto G1 = m.目标节点.创建事实代次;
                const auto f = 读取准备首次历史闭包_已锁定(
                    m.目标节点.编码, G1, G0);
                const auto 当前 = 历史成员关系_已锁定(m.目标节点.编码,
                    准备登记_v2_.当前索引根, 准备登记_v2_.当前成员关系,
                    G1, G0);
                if (!f || !当前 || (当前->退出事实代次
                        && (*当前->退出事实代次 <= G1
                            || *当前->退出事实代次 > G0)))
                    return 失败(服务维护历史事实范围读取状态_v1::引用冲突);
                if (f->自我 == 请求.自我 && G1 > 请求.排除起始事实代次)
                    结果.服务准备变化组.push_back({*f, 当前->退出事实代次});
            }

            const auto 当前反查完整 = [&](const auto& 当前闭包,
                稳定编码 当前根, 稳定编码 当前类型,
                const auto& 历史闭包) noexcept {
                for (const auto& m : 当前闭包.成员) {
                    if (m.关系.源节点 != 当前根
                        || m.关系.目标节点 != m.目标节点.编码
                        || m.关系.关系类型节点 != 当前类型
                        || m.关系.角色或顺序 != 1
                        || m.关系.写入所有者 != 写入端口_.所有者身份()
                        || m.目标节点.写入所有者 != 写入端口_.所有者身份()
                        || m.关系.创建事实代次 != m.目标节点.创建事实代次
                        || m.关系.退出事实代次 || !活动于(m.关系, G0)
                        || !活动于(m.目标节点, G0))
                        return false;
                    if (m.目标节点.创建事实代次 < 历史账登记_.登记事实代次)
                        continue;
                    std::size_t 历史成员数 = 0;
                    for (const auto& h : 历史闭包.成员)
                        if (h.目标节点.编码 == m.目标节点.编码)
                            ++历史成员数;
                    if (历史成员数 != 1) return false;
                }
                return true;
            };
            if (!当前反查完整(*当前进展闭包, 进展登记_v2_.当前索引根,
                    进展登记_v2_.当前成员关系, *进展闭包)
                || !当前反查完整(*当前准备闭包, 准备登记_v2_.当前索引根,
                    准备登记_v2_.当前成员关系, *准备闭包))
                return 失败(服务维护历史事实范围读取状态_v1::集合不闭合);

            const auto 排序 = [](auto& 组, auto 身份, auto 版本) {
                std::sort(组.begin(), 组.end(), [&](const auto& a, const auto& b) {
                    if (a.事实.形成事实代次 != b.事实.形成事实代次)
                        return a.事实.形成事实代次 < b.事实.形成事实代次;
                    if (身份(a.事实).值 != 身份(b.事实).值)
                        return 身份(a.事实).值 < 身份(b.事实).值;
                    return 版本(a.事实) < 版本(b.事实);
                });
            };
            排序(结果.合同变化组,
                [](const auto& f) { return f.身份.值; },
                [](const auto& f) { return f.合同代次; });
            排序(结果.合同状态变化组,
                [](const auto& f) { return f.合同.值; },
                [](const auto& f) { return f.状态版本; });
            排序(结果.到期事件变化组,
                [](const auto& f) { return f.身份.值; },
                [](const auto& f) { return f.合同终态版本; });
            排序(结果.服务进展变化组,
                [](const auto& f) { return f.身份.值; },
                [](const auto& f) { return f.形成事实代次; });
            排序(结果.服务准备变化组,
                [](const auto& f) { return f.身份.值; },
                [](const auto& f) { return f.形成事实代次; });
            const auto 总数 = 结果.合同变化组.size()
                + 结果.合同状态变化组.size() + 结果.到期事件变化组.size()
                + 结果.服务进展变化组.size() + 结果.服务准备变化组.size();
            if (总数 > 请求.数量预算)
                return 失败(服务维护历史事实范围读取状态_v1::数量预算不足);
            服务维护历史事实完整集合见证_v1 w;
            w.结构登记版本 = 服务维护历史事实账结构登记版本_v1;
            w.历史账登记事实代次 = 历史账登记_.登记事实代次;
            w.自我 = 请求.自我;
            w.排除起始事实代次 = 请求.排除起始事实代次;
            w.包含结束事实代次 = G0;
            const auto 加见证 = [&](服务维护历史事实类别_v1 c,
                const auto& 组, auto 身份, auto 版本) {
                for (const auto& p : 组)
                    w.规范成员身份组.push_back({c, 身份(p.事实),
                        版本(p.事实), p.事实.形成事实代次});
            };
            加见证(服务维护历史事实类别_v1::服务合同, 结果.合同变化组,
                [](const auto& f) { return f.身份.值; },
                [](const auto& f) { return f.合同代次; });
            加见证(服务维护历史事实类别_v1::合同状态, 结果.合同状态变化组,
                [](const auto& f) { return f.合同.值; },
                [](const auto& f) { return f.状态版本; });
            加见证(服务维护历史事实类别_v1::到期未满足事件,
                结果.到期事件变化组,
                [](const auto& f) { return f.身份.值; },
                [](const auto& f) { return f.合同终态版本; });
            加见证(服务维护历史事实类别_v1::服务进展, 结果.服务进展变化组,
                [](const auto& f) { return f.身份.值; },
                [](const auto& f) { return f.形成事实代次; });
            加见证(服务维护历史事实类别_v1::服务准备, 结果.服务准备变化组,
                [](const auto& f) { return f.身份.值; },
                [](const auto& f) { return f.形成事实代次; });
            std::sort(w.规范成员身份组.begin(), w.规范成员身份组.end(),
                服务维护历史成员身份小于_v1);
            w.声明成员数 = w.规范成员身份组.size();
            w.G0 = G0;
            结果.完整集合见证 = std::move(w);
            结果.本次正式读回截止 = G0;
            if (!读后守卫(G0))
                return 失败(服务维护历史事实范围读取状态_v1::当前性漂移);
            if (!结果.成功())
                return 失败(服务维护历史事实范围读取状态_v1::集合不闭合);
            return 结果;
        } catch (const std::bad_alloc&) {
            return 失败(服务维护历史事实范围读取状态_v1::资源失败);
        } catch (const std::length_error&) {
            return 失败(服务维护历史事实范围读取状态_v1::资源失败);
        } catch (...) {
            return 失败(服务维护历史事实范围读取状态_v1::内部错误);
        }
    }

    服务维护历史边界读取结果_v2
    按事实代次边界读取服务维护历史完整快照_v2(
        const 服务维护历史边界读取请求_v2& 请求) const noexcept {
        using 状态 = 服务维护历史边界读取状态_v2;
        using namespace 服务合同事实权威内部;
        const auto 失败 = [](状态 s) {
            服务维护历史边界读取结果_v2 r;
            r.状态 = s;
            return r;
        };
        try {
            const auto G0 = 请求.请求头.期望事实代次;
            if (请求.合同版本 != 服务维护历史边界合同版本_v2
                || 请求.请求头.合同版本 != L2结构合同版本
                || G0 == 0 || !有效(请求.自我.值)
                || 请求.左边界事实代次 == 0
                || 请求.左边界事实代次 > G0
                || 请求.左边界数量预算 == 0 || 请求.变化数量预算 == 0)
                return 失败(状态::入口拒绝);

            std::unique_lock<std::mutex> 锁(互斥体_);
            const auto Gseed = 历史覆盖登记_v2_.登记事实代次;
            if (Gseed == 0) return 失败(状态::v2历史账未登记);
            if (请求.左边界事实代次 < Gseed)
                return 失败(状态::覆盖边界不可用);
            if (!读前守卫(G0)) return 失败(状态::当前性漂移);

            锁.unlock();
            const auto 当前合同 = 读取当前有效未满足服务合同完整集合_v1({
                服务合同事实权威合同版本_v1, 请求.请求头, 请求.自我});
            const auto 当前进展 = 读取当前服务合同关联进展完整集合_v2({
                服务进展事实扩展合同版本_v2, 请求.请求头, 请求.自我});
            const auto 当前准备 = 读取当前服务准备完整集合_v2({
                服务准备事实扩展合同版本_v2, 请求.请求头, 请求.自我});
            const auto 兼容变化 = 按事实代次范围读取服务维护历史事实完整组_v1({
                服务维护历史事实账合同版本_v1, 请求.请求头, 请求.自我,
                请求.左边界事实代次, 请求.变化数量预算});
            锁.lock();
            if (!读前守卫(G0)) return 失败(状态::当前性漂移);

            const auto 映射当前读取失败 = [&](auto s) noexcept {
                if (s == 服务合同事实权威读取状态_v1::当前性漂移)
                    return 状态::当前性漂移;
                if (s == 服务合同事实权威读取状态_v1::资源失败)
                    return 状态::资源失败;
                if (s == 服务合同事实权威读取状态_v1::结构未登记)
                    return 状态::版本漂移;
                if (s == 服务合同事实权威读取状态_v1::引用冲突)
                    return 状态::引用冲突;
                if (s == 服务合同事实权威读取状态_v1::集合不闭合)
                    return 状态::集合不闭合;
                return 状态::内部错误;
            };
            if (!当前合同.成功()) return 失败(映射当前读取失败(当前合同.状态));
            if (!当前进展.成功()) return 失败(映射当前读取失败(当前进展.状态));
            if (!当前准备.成功()) return 失败(映射当前读取失败(当前准备.状态));
            if (!兼容变化.成功()) {
                switch (兼容变化.状态) {
                case 服务维护历史事实范围读取状态_v1::当前性漂移:
                    return 失败(状态::当前性漂移);
                case 服务维护历史事实范围读取状态_v1::数量预算不足:
                    return 失败(状态::变化预算不足);
                case 服务维护历史事实范围读取状态_v1::资源失败:
                    return 失败(状态::资源失败);
                case 服务维护历史事实范围读取状态_v1::引用冲突:
                    return 失败(状态::引用冲突);
                case 服务维护历史事实范围读取状态_v1::覆盖边界不可用:
                case 服务维护历史事实范围读取状态_v1::历史账未登记:
                    return 失败(状态::版本漂移);
                case 服务维护历史事实范围读取状态_v1::集合不闭合:
                    return 失败(状态::集合不闭合);
                default:
                    return 失败(状态::内部错误);
                }
            }

            const auto 读 = L1_.尝试读取所有者范围一致关系类型闭包投影(
                形成历史覆盖账读取请求_v2(G0));
            if (读.状态 != L1所有者范围一致当前读取状态::成功) {
                if (读.状态 == L1所有者范围一致当前读取状态::事实代次漂移)
                    return 失败(状态::当前性漂移);
                if (读.状态 == L1所有者范围一致当前读取状态::资源失败)
                    return 失败(状态::资源失败);
                return 失败(状态::内部错误);
            }
            if (读.读取事实代次 != G0) return 失败(状态::当前性漂移);
            if (!验证v2共同登记(读, 历史覆盖登记_v2_,
                    服务维护历史覆盖结构登记版本_v2, G0))
                return 失败(状态::v2历史账未登记);
            if (读.关系类型闭包.size() != 5)
                return 失败(状态::集合不闭合);
            const auto* 合同闭包 = 查找闭包(
                读, 历史覆盖登记_v2_.合同历史成员关系);
            const auto* 状态闭包 = 查找闭包(
                读, 历史覆盖登记_v2_.合同状态历史成员关系);
            const auto* 事件闭包 = 查找闭包(
                读, 历史覆盖登记_v2_.到期事件历史成员关系);
            const auto* 进展闭包 = 查找闭包(
                读, 历史覆盖登记_v2_.进展历史成员关系);
            const auto* 准备闭包 = 查找闭包(
                读, 历史覆盖登记_v2_.准备历史成员关系);
            if (!合同闭包 || !状态闭包 || !事件闭包 || !进展闭包 || !准备闭包)
                return 失败(状态::集合不闭合);

            const auto 成员基础完整 = [&](const auto& m, 稳定编码 根,
                稳定编码 类型) noexcept {
                const auto 预期关系代次 = m.目标节点.创建事实代次 < Gseed
                    ? Gseed : m.目标节点.创建事实代次;
                return m.关系.源节点 == 根
                    && m.关系.目标节点 == m.目标节点.编码
                    && m.关系.关系类型节点 == 类型
                    && m.关系.角色或顺序 == 1
                    && m.关系.写入所有者 == 写入端口_.所有者身份()
                    && m.目标节点.写入所有者 == 写入端口_.所有者身份()
                    && m.目标节点.创建事实代次 != Gseed
                    && m.关系.创建事实代次 == 预期关系代次
                    && !m.关系.退出事实代次 && !m.目标节点.退出事实代次
                    && 活动于(m.关系, G0) && 活动于(m.目标节点, G0);
            };

            std::vector<服务合同历史投影_v1> 全合同;
            std::vector<服务合同状态历史投影_v1> 全状态;
            std::vector<服务到期事件历史投影_v1> 全事件;
            std::vector<服务进展历史投影_v1> 全进展;
            std::vector<服务准备历史投影_v1> 全准备;
            std::vector<服务合同状态历史投影_v1> 全局状态链;
            std::vector<服务进展历史投影_v1> 全局进展链;
            std::vector<服务准备历史投影_v1> 全局准备链;
            std::map<std::uint64_t, L2存在身份> 合同自我;

            for (const auto& m : 合同闭包->成员) {
                if (!成员基础完整(m, 历史覆盖登记_v2_.合同历史索引根,
                        历史覆盖登记_v2_.合同历史成员关系))
                    return 失败(状态::集合不闭合);
                const auto* p = 查找属性投影(
                    m.目标节点属性值, m.目标节点.编码, 登记_.合同载荷);
                服务合同事实_v1 f;
                if (!p || !解码合同(p->当前值事实, m.目标节点.编码, f)
                    || f.形成事实代次 != m.目标节点.创建事实代次
                    || !合同历史引用闭合_已锁定(f, G0))
                    return 失败(状态::引用冲突);
                const auto 当前 = 历史成员关系_已锁定(f.身份.值,
                    登记_.当前合同索引根, 登记_.当前合同成员关系,
                    f.形成事实代次, G0);
                if (!当前 || (当前->退出事实代次
                        && (*当前->退出事实代次 <= f.形成事实代次
                            || *当前->退出事实代次 > G0)))
                    return 失败(状态::集合不闭合);
                const auto [it, inserted] = 合同自我.emplace(f.身份.值.值, f.自我);
                if (!inserted && it->second != f.自我)
                    return 失败(状态::引用冲突);
                if (f.自我 == 请求.自我)
                    全合同.push_back({f, 当前->退出事实代次});
            }
            for (const auto& m : 状态闭包->成员) {
                if (!成员基础完整(m, 历史覆盖登记_v2_.合同状态历史索引根,
                        历史覆盖登记_v2_.合同状态历史成员关系))
                    return 失败(状态::集合不闭合);
                const auto G1 = m.目标节点.创建事实代次;
                const auto 当前 = 读取状态当前关系_已锁定(
                    m.目标节点.编码, G1, G0);
                const auto* p = 查找属性投影(
                    m.目标节点属性值, m.目标节点.编码, 登记_.合同状态载荷);
                服务合同状态事实_v1 f;
                if (!当前 || !p || !解码状态(p->当前值事实,
                        {当前->源节点}, f)
                    || f.形成事实代次 != G1)
                    return 失败(状态::引用冲突);
                if (当前->退出事实代次
                    && (*当前->退出事实代次 <= G1 || *当前->退出事实代次 > G0))
                    return 失败(状态::集合不闭合);
                const auto self = 合同自我.find(f.合同.值.值);
                if (self == 合同自我.end()) return 失败(状态::引用冲突);
                全局状态链.push_back({f, 当前->退出事实代次});
                if (self->second == 请求.自我)
                    全状态.push_back({f, 当前->退出事实代次});
            }
            for (const auto& m : 事件闭包->成员) {
                if (!成员基础完整(m, 历史覆盖登记_v2_.到期事件历史索引根,
                        历史覆盖登记_v2_.到期事件历史成员关系))
                    return 失败(状态::集合不闭合);
                const auto* p = 查找属性投影(m.目标节点属性值,
                    m.目标节点.编码, 登记_.到期事件载荷);
                服务到期未满足事件事实_v1 f;
                if (!p || !解码事件(p->当前值事实, m.目标节点.编码, f)
                    || f.形成事实代次 != m.目标节点.创建事实代次
                    || !到期事件历史引用闭合_已锁定(f, G0))
                    return 失败(状态::引用冲突);
                const auto self = 合同自我.find(f.合同.值.值);
                if (self == 合同自我.end()) return 失败(状态::引用冲突);
                if (self->second == 请求.自我) 全事件.push_back({f, std::nullopt});
            }
            for (const auto& m : 进展闭包->成员) {
                if (!成员基础完整(m, 历史覆盖登记_v2_.进展历史索引根,
                        历史覆盖登记_v2_.进展历史成员关系))
                    return 失败(状态::集合不闭合);
                const auto G1 = m.目标节点.创建事实代次;
                const auto f = 读取进展首次历史闭包_已锁定(
                    m.目标节点.编码, G1, G0);
                const auto 当前 = 历史成员关系_已锁定(m.目标节点.编码,
                    进展登记_v2_.当前索引根, 进展登记_v2_.当前成员关系,
                    G1, G0);
                if (!f || !当前 || (当前->退出事实代次
                        && (*当前->退出事实代次 <= G1 || *当前->退出事实代次 > G0)))
                    return 失败(状态::引用冲突);
                全局进展链.push_back({*f, 当前->退出事实代次});
                if (f->自我 == 请求.自我)
                    全进展.push_back({*f, 当前->退出事实代次});
            }
            for (const auto& m : 准备闭包->成员) {
                if (!成员基础完整(m, 历史覆盖登记_v2_.准备历史索引根,
                        历史覆盖登记_v2_.准备历史成员关系))
                    return 失败(状态::集合不闭合);
                const auto G1 = m.目标节点.创建事实代次;
                const auto f = 读取准备首次历史闭包_已锁定(
                    m.目标节点.编码, G1, G0);
                const auto 当前 = 历史成员关系_已锁定(m.目标节点.编码,
                    准备登记_v2_.当前索引根, 准备登记_v2_.当前成员关系,
                    G1, G0);
                if (!f || !当前 || (当前->退出事实代次
                        && (*当前->退出事实代次 <= G1 || *当前->退出事实代次 > G0)))
                    return 失败(状态::引用冲突);
                全局准备链.push_back({*f, 当前->退出事实代次});
                if (f->自我 == 请求.自我)
                    全准备.push_back({*f, 当前->退出事实代次});
            }

            const auto 排序 = [](auto& 组, auto 身份, auto 版本) {
                std::sort(组.begin(), 组.end(), [&](const auto& a, const auto& b) {
                    if (a.事实.形成事实代次 != b.事实.形成事实代次)
                        return a.事实.形成事实代次 < b.事实.形成事实代次;
                    if (身份(a.事实).值 != 身份(b.事实).值)
                        return 身份(a.事实).值 < 身份(b.事实).值;
                    return 版本(a.事实) < 版本(b.事实);
                });
            };
            排序(全合同, [](const auto& f) { return f.身份.值; },
                [](const auto& f) { return f.合同代次; });
            排序(全状态, [](const auto& f) { return f.合同.值; },
                [](const auto& f) { return f.状态版本; });
            排序(全事件, [](const auto& f) { return f.身份.值; },
                [](const auto& f) { return f.合同终态版本; });
            排序(全进展, [](const auto& f) { return f.身份.值; },
                [](const auto& f) { return f.形成事实代次; });
            排序(全准备, [](const auto& f) { return f.身份.值; },
                [](const auto& f) { return f.形成事实代次; });

            if (!历史当前版本链按流闭合_v2(全局状态链,
                    [](const auto& a, const auto& b) {
                        return a.合同 == b.合同;
                    })
                || !历史当前版本链按流闭合_v2(
                    全局进展链, 同一服务进展活动流事实_v2)
                || !历史当前版本链按流闭合_v2(
                    全局准备链, 同一服务准备活动流事实_v2))
                return 失败(状态::左边界不可恢复);

            服务维护历史边界读取结果_v2 结果;
            结果.状态 = 状态::已读取;
            服务维护历史边界完整范围快照_v2 s;
            s.v2覆盖登记事实代次 = Gseed;
            s.自我 = 请求.自我;
            s.左边界事实代次 = 请求.左边界事实代次;
            s.包含结束事实代次 = G0;

            std::map<std::uint64_t, 服务合同历史投影_v1> 左合同;
            std::map<std::uint64_t, 服务合同状态历史投影_v1> 左状态;
            const auto 在左边界当前 = [&](const auto& p) noexcept {
                return p.事实.形成事实代次 <= 请求.左边界事实代次
                    && (!p.退出当前事实代次
                        || *p.退出当前事实代次 > 请求.左边界事实代次);
            };
            for (const auto& p : 全合同) {
                if (在左边界当前(p)) {
                    if (!左合同.emplace(p.事实.身份.值.值, p).second)
                        return 失败(状态::左边界不可恢复);
                } else if (p.事实.形成事实代次 > 请求.左边界事实代次) {
                    s.合同变化组.push_back(p);
                }
            }
            for (const auto& p : 全状态) {
                if (在左边界当前(p)) {
                    if (左合同.contains(p.事实.合同.值.值)
                        && !左状态.emplace(p.事实.合同.值.值, p).second)
                        return 失败(状态::左边界不可恢复);
                } else if (p.事实.形成事实代次 > 请求.左边界事实代次) {
                    s.合同状态变化组.push_back(p);
                }
            }
            if (左合同.size() != 左状态.size())
                return 失败(状态::左边界不可恢复);
            for (const auto& [id, c] : 左合同) {
                const auto q = 左状态.find(id);
                if (q == 左状态.end()) return 失败(状态::左边界不可恢复);
                if (q->second.事实.状态 != 服务合同当前状态_v1::有效未满足
                    || q->second.事实.已消费有效秒 > c.事实.冻结有效总秒)
                    return 失败(状态::集合不闭合);
                s.左边界合同组.push_back({c, q->second});
            }
            std::sort(s.左边界合同组.begin(), s.左边界合同组.end(),
                [](const auto& a, const auto& b) {
                    const auto& x = a.合同.事实;
                    const auto& y = b.合同.事实;
                    if (x.形成事实代次 != y.形成事实代次)
                        return x.形成事实代次 < y.形成事实代次;
                    if (x.身份.值.值 != y.身份.值.值)
                        return x.身份.值.值 < y.身份.值.值;
                    return x.合同代次 < y.合同代次;
                });
            for (const auto& p : 全事件)
                if (p.事实.形成事实代次 > 请求.左边界事实代次)
                    s.到期事件变化组.push_back(p);
            for (const auto& p : 全进展) {
                if (在左边界当前(p)) s.左边界进展组.push_back(p);
                else if (p.事实.形成事实代次 > 请求.左边界事实代次)
                    s.服务进展变化组.push_back(p);
            }
            for (const auto& p : 全准备) {
                if (在左边界当前(p)) s.左边界准备组.push_back(p);
                else if (p.事实.形成事实代次 > 请求.左边界事实代次)
                    s.服务准备变化组.push_back(p);
            }

            if (s.合同变化组 != 兼容变化.合同变化组
                || s.合同状态变化组 != 兼容变化.合同状态变化组
                || s.到期事件变化组 != 兼容变化.到期事件变化组
                || s.服务进展变化组 != 兼容变化.服务进展变化组
                || s.服务准备变化组 != 兼容变化.服务准备变化组)
                return 失败(状态::集合不闭合);

            const auto 取当前 = [](const auto& 全组) {
                using 事实类型 = std::decay_t<decltype(全组.front().事实)>;
                std::vector<事实类型> r;
                for (const auto& p : 全组)
                    if (!p.退出当前事实代次) r.push_back(p.事实);
                return r;
            };
            auto G0合同 = 取当前(全合同);
            std::map<std::uint64_t, bool> G0合同身份;
            for (const auto& f : G0合同) G0合同身份.emplace(f.身份.值.值, true);
            std::vector<服务合同状态事实_v1> G0状态;
            for (const auto& p : 全状态)
                if (!p.退出当前事实代次
                    && G0合同身份.contains(p.事实.合同.值.值))
                    G0状态.push_back(p.事实);
            auto G0进展 = 取当前(全进展);
            auto G0准备 = 取当前(全准备);
            const auto 按编码排序 = [](auto& 组, auto 身份) {
                std::sort(组.begin(), 组.end(), [&](const auto& a, const auto& b) {
                    return 身份(a).值 < 身份(b).值;
                });
            };
            按编码排序(G0合同, [](const auto& f) { return f.身份.值; });
            按编码排序(G0状态, [](const auto& f) { return f.合同.值; });
            按编码排序(G0进展, [](const auto& f) { return f.身份.值; });
            按编码排序(G0准备, [](const auto& f) { return f.身份.值; });
            auto 权威合同 = 当前合同.完整合同事实组;
            auto 权威状态 = 当前合同.当前状态事实组;
            auto 权威进展 = 当前进展.完整进展事实组;
            auto 权威准备 = 当前准备.完整准备事实组;
#if defined(ARCH_INSTINCT_SERVICE_CONTRACT_FACT_AUTHORITY_VALIDATION)
            if (ARCH_G0终态错配一次_) {
                ARCH_G0终态错配一次_ = false;
                权威合同.push_back({});
            }
#endif
            按编码排序(权威合同, [](const auto& f) { return f.身份.值; });
            按编码排序(权威状态, [](const auto& f) { return f.合同.值; });
            按编码排序(权威进展, [](const auto& f) { return f.身份.值; });
            按编码排序(权威准备, [](const auto& f) { return f.身份.值; });
            if (G0合同 != 权威合同 || G0状态 != 权威状态
                || G0进展 != 权威进展 || G0准备 != 权威准备)
                return 失败(状态::集合不闭合);

            const auto 加成员 = [](auto& 组, 服务维护历史事实类别_v1 类别,
                const auto& 投影组, auto 身份, auto 版本) {
                for (const auto& p : 投影组)
                    组.push_back({类别, 身份(p.事实), 版本(p.事实),
                        p.事实.形成事实代次});
            };
            for (const auto& p : s.左边界合同组) {
                const auto& c = p.合同.事实;
                const auto& q = p.当前状态.事实;
                s.规范左边界成员身份组.push_back({
                    服务维护历史事实类别_v1::服务合同, c.身份.值,
                    c.合同代次, c.形成事实代次});
                s.规范左边界成员身份组.push_back({
                    服务维护历史事实类别_v1::合同状态, q.合同.值,
                    q.状态版本, q.形成事实代次});
            }
            加成员(s.规范左边界成员身份组,
                服务维护历史事实类别_v1::服务进展, s.左边界进展组,
                [](const auto& f) { return f.身份.值; },
                [](const auto& f) { return f.形成事实代次; });
            加成员(s.规范左边界成员身份组,
                服务维护历史事实类别_v1::服务准备, s.左边界准备组,
                [](const auto& f) { return f.身份.值; },
                [](const auto& f) { return f.形成事实代次; });
            加成员(s.规范变化成员身份组,
                服务维护历史事实类别_v1::服务合同, s.合同变化组,
                [](const auto& f) { return f.身份.值; },
                [](const auto& f) { return f.合同代次; });
            加成员(s.规范变化成员身份组,
                服务维护历史事实类别_v1::合同状态, s.合同状态变化组,
                [](const auto& f) { return f.合同.值; },
                [](const auto& f) { return f.状态版本; });
            加成员(s.规范变化成员身份组,
                服务维护历史事实类别_v1::到期未满足事件,
                s.到期事件变化组,
                [](const auto& f) { return f.身份.值; },
                [](const auto& f) { return f.合同终态版本; });
            加成员(s.规范变化成员身份组,
                服务维护历史事实类别_v1::服务进展, s.服务进展变化组,
                [](const auto& f) { return f.身份.值; },
                [](const auto& f) { return f.形成事实代次; });
            加成员(s.规范变化成员身份组,
                服务维护历史事实类别_v1::服务准备, s.服务准备变化组,
                [](const auto& f) { return f.身份.值; },
                [](const auto& f) { return f.形成事实代次; });
            std::sort(s.规范左边界成员身份组.begin(),
                s.规范左边界成员身份组.end(), 服务维护历史成员身份小于_v1);
            std::sort(s.规范变化成员身份组.begin(),
                s.规范变化成员身份组.end(), 服务维护历史成员身份小于_v1);
            s.声明左边界成员数 = s.规范左边界成员身份组.size();
            s.声明变化成员数 = s.规范变化成员身份组.size();
            if (s.声明左边界成员数 > 请求.左边界数量预算)
                return 失败(状态::左边界预算不足);
            if (s.声明变化成员数 > 请求.变化数量预算)
                return 失败(状态::变化预算不足);
            结果.快照 = std::move(s);
            结果.本次正式读回截止 = G0;
            if (!读后守卫(G0)) return 失败(状态::当前性漂移);
            if (!结果.成功()) return 失败(状态::集合不闭合);
            return 结果;
        } catch (const std::bad_alloc&) {
            return 失败(状态::资源失败);
        } catch (const std::length_error&) {
            return 失败(状态::资源失败);
        } catch (...) {
            return 失败(状态::内部错误);
        }
    }

private:
    服务准备完整集合读取结果_v2 读取当前服务准备完整集合_v2_已锁定(
        const 服务准备完整集合读取请求_v2& 请求) const noexcept {
        const auto 失败 = [](服务合同事实权威读取状态_v2 s) {
            服务准备完整集合读取结果_v2 r; r.状态 = s; return r; };
        try {
            if (!准备请求有效(请求))
                return 失败(服务合同事实权威读取状态_v2::入口拒绝);
            const auto G0 = 请求.请求头.期望事实代次;
            if (!读前守卫(G0))
                return 失败(服务合同事实权威读取状态_v2::当前性漂移);
#if defined(ARCH_INSTINCT_SERVICE_CONTRACT_FACT_AUTHORITY_VALIDATION)
            if (ARCH_资源失败一次_) { ARCH_资源失败一次_ = false;
                return 失败(服务合同事实权威读取状态_v2::资源失败); }
            注入漂移_已锁定();
#endif
            const auto 读 = L1_.尝试读取所有者范围一致关系类型闭包投影(
                形成准备读取请求_v2(G0));
            if (读.状态 != L1所有者范围一致当前读取状态::成功)
                return 失败(服务合同事实权威内部::映射读取状态(读.状态));
            if (读.读取事实代次 != G0)
                return 失败(服务合同事实权威读取状态_v2::当前性漂移);
            if (!验证v2共同登记(读, 准备登记_v2_,
                    服务准备事实结构登记版本_v2, G0))
                return 失败(服务合同事实权威读取状态_v2::结构未登记);
            if (读.关系类型闭包.size() != 1)
                return 失败(服务合同事实权威读取状态_v2::集合不闭合);
            const auto* 闭包 = 查找闭包(读, 准备登记_v2_.当前成员关系);
            if (!闭包) return 失败(服务合同事实权威读取状态_v2::集合不闭合);
            服务准备完整集合读取结果_v2 结果;
            结果.状态 = 服务合同事实权威读取状态_v2::已读取;
            for (const auto& 成员 : 闭包->成员) {
                if (成员.关系.源节点 != 准备登记_v2_.当前索引根
                    || 成员.关系.关系类型节点 != 准备登记_v2_.当前成员关系
                    || 成员.关系.目标节点 != 成员.目标节点.编码
                    || !服务合同事实权威内部::活动于(成员.关系, G0))
                    return 失败(服务合同事实权威读取状态_v2::引用冲突);
                const auto* p = 服务合同事实权威内部::查找属性投影(
                    成员.目标节点属性值, 成员.目标节点.编码, 准备登记_v2_.载荷);
                服务准备当前事实_v2 f;
                if (!p || !服务合同事实权威内部::解码准备_v2(
                        p->当前值事实, 成员.目标节点.编码, f))
                    return 失败(服务合同事实权威读取状态_v2::引用冲突);
                const auto u = [&](稳定编码 t) { return
                    服务合同事实权威内部::唯一关系目标(
                        成员.目标节点源关系组, t, 成员.目标节点.编码, G0); };
                const auto 来源 = std::holds_alternative<L2需求身份>(f.来源)
                    ? std::get<L2需求身份>(f.来源).值
                    : std::get<服务能力缺口身份_v1>(f.来源).值;
                if (u(准备登记_v2_.来源关系) != std::optional<稳定编码>{来源}
                    || u(准备登记_v2_.自我关系) != std::optional<稳定编码>{f.自我.值}
                    || u(准备登记_v2_.任务关系) != std::optional<稳定编码>{f.任务.值}
                    || u(准备登记_v2_.方法关系) != std::optional<稳定编码>{f.方法.值}
                    || u(准备登记_v2_.正式选择关系)
                        != std::optional<稳定编码>{f.执行绑定.正式选择.值}
                    || u(准备登记_v2_.冻结材料关系)
                        != std::optional<稳定编码>{f.执行绑定.执行冻结材料.值}
                    || u(准备登记_v2_.实例方法关系)
                        != std::optional<稳定编码>{f.执行绑定.实例方法.值}
                    || !可选关系目标闭合(成员.目标节点源关系组,
                        准备登记_v2_.状态引用关系, 成员.目标节点.编码,
                        f.进展状态 ? std::optional<稳定编码>{f.进展状态->值}
                                    : std::nullopt, G0)
                    || !可选关系目标闭合(成员.目标节点源关系组,
                        准备登记_v2_.动态引用关系, 成员.目标节点.编码,
                        f.进展动态 ? std::optional<稳定编码>{f.进展动态->值}
                                    : std::nullopt, G0)
                    || !可选关系目标闭合(成员.目标节点源关系组,
                        准备登记_v2_.结果引用关系, 成员.目标节点.编码,
                        f.准备结果 ? std::optional<稳定编码>{f.准备结果->值}
                                   : std::nullopt, G0)
                    || !可选关系目标闭合(成员.目标节点源关系组,
                        准备登记_v2_.验证引用关系, 成员.目标节点.编码,
                        f.完成验证 ? std::optional<稳定编码>{f.完成验证->值}
                                   : std::nullopt, G0))
                    return 失败(服务合同事实权威读取状态_v2::引用冲突);
                if (f.自我 != 请求.自我)
                    continue;
                结果.完整准备事实组.push_back(std::move(f));
            }
            std::sort(结果.完整准备事实组.begin(), 结果.完整准备事实组.end(),
                [](const auto& a, const auto& b) { return a.身份.值.值 < b.身份.值.值; });
            服务准备完整集合见证_v2 w{服务准备事实结构登记版本_v2,
                结果.完整准备事实组.size(), {}, G0};
            for (const auto& f : 结果.完整准备事实组) w.规范成员身份组.push_back(f.身份);
            结果.完整集合见证 = std::move(w); 结果.本次正式读回截止 = G0;
            const bool 当前 = 读后守卫(G0);
            if (!当前 || !结果.成功()) return 失败(!当前
                ? 服务合同事实权威读取状态_v2::当前性漂移
                : 服务合同事实权威读取状态_v2::集合不闭合);
            return 结果;
        } catch (const std::bad_alloc&) { return 失败(服务合同事实权威读取状态_v2::资源失败); }
        catch (const std::length_error&) { return 失败(服务合同事实权威读取状态_v2::资源失败); }
        catch (...) { return 失败(服务合同事实权威读取状态_v2::内部错误); }
    }

public:
#if defined(ARCH_INSTINCT_SERVICE_CONTRACT_FACT_AUTHORITY_VALIDATION)
    std::optional<std::uint64_t> ARCH_完成延迟v2历史覆盖登记() noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        try {
            if (历史覆盖登记_v2_.登记事实代次 != 0)
                return 历史覆盖登记_v2_.登记事实代次;
            历史覆盖登记_v2_ =
                服务合同事实权威内部::初始化历史覆盖结构登记_v2(
                    L1_, 写入端口_, 登记_, 进展登记_v2_, 准备登记_v2_);
            return 历史覆盖登记_v2_.登记事实代次 != 0
                ? std::optional<std::uint64_t>{历史覆盖登记_v2_.登记事实代次}
                : std::nullopt;
        } catch (...) {
            return std::nullopt;
        }
    }

    std::optional<L2存在身份> ARCH_建立历史到期事件样本_v1(
        std::uint64_t 幂等身份, bool 形成到期 = true,
        std::uint8_t 损坏载荷类别 = 0) noexcept {
        using namespace 服务合同事实权威内部;
        std::lock_guard<std::mutex> 锁(互斥体_);
        try {
            if (幂等身份 == 0 || 损坏载荷类别 > 3
                || (!形成到期 && 损坏载荷类别 == 3))
                return std::nullopt;
            const auto 当前 = 当前事实代次_已锁定();
            if (!当前) return std::nullopt;
            L1所有者范围写集请求 节点写;
            节点写.期望事实代次 = *当前;
            节点写.写入幂等身份 = {幂等身份};
            for (std::uint32_t k = 1; k <= 9; ++k)
                节点写.节点.push_back({键(k), 节点种类::普通, std::nullopt});
            const auto 节点结果 = 写入端口_.提交所有者范围中性写集(节点写);
            if (节点结果.状态 != L1所有者范围写入状态::成功
                || 节点结果.事实代次 == 0)
                return std::nullopt;
            const auto 取节点 = [&](std::uint32_t k) {
                return 查找映射(节点结果.新编码映射, k);
            };
            const auto 自我 = 取节点(1), 提出者 = 取节点(2);
            const auto 需求 = 取节点(3), 目标宿主 = 取节点(4);
            const auto 目标特征 = 取节点(5), 目标合同 = 取节点(6);
            const auto 初态来源 = 取节点(7), 未满足事实 = 取节点(8);
            const auto 终态来源 = 取节点(9);
            if (!自我 || !提出者 || !需求 || !目标宿主 || !目标特征
                || !目标合同 || !初态来源 || !未满足事实 || !终态来源)
                return std::nullopt;

            服务合同事实_v1 合同;
            合同.自我 = L2存在身份{*自我};
            合同.提出者 = L2存在身份{*提出者};
            合同.需求 = L2需求身份{*需求};
            合同.来源提出事件 = {0x5348'1000'0000'0001ULL};
            合同.目标宿主 = L2存在身份{*目标宿主};
            合同.目标特征 = L2特征定义身份{*目标特征};
            合同.目标状态合同 = L2目标状态合同身份{*目标合同};
            合同.服务范围 = {0x5348'1000'0000'0002ULL};
            合同.提出运行代次 = 1;
            合同.时间纪元身份 = {0x5348'1000'0000'0003ULL};
            合同.提出完整秒边界 = 10;
            合同.有效开始完整秒边界 = 10;
            合同.冻结有效总秒 = 100;
            合同.冻结预算 = 1000;
            合同.预支值 = 300;
            合同.完工余款 = 700;
            合同.需求版本 = 1;
            合同.法规准入见证 = {0x5348'1000'0000'0004ULL};
            合同.法规规则版本 = 1;
            合同.合同代次 = 1;
            合同.合同规则版本 = 1;
            L1所有者范围写集请求 合同写;
            合同写.期望事实代次 = 节点结果.事实代次;
            合同写.写入幂等身份 = {幂等身份 + 1};
            合同写.节点.push_back({键(10), 节点种类::普通, std::nullopt});
            auto 合同载荷 = 编码合同(合同);
            if (损坏载荷类别 == 1) 合同载荷.pop_back();
            合同写.值.push_back({键(50), 键(10), 登记_.合同载荷,
                std::move(合同载荷), 键(10)});
            合同写.属性槽变更.push_back({键(10), 登记_.合同载荷, 键(50)});
            const auto 合同关系 = [&](std::uint32_t k,
                L1所有者范围事实引用 源, L1所有者范围事实引用 目标,
                稳定编码 类型) {
                合同写.关系.push_back({键(k), std::move(源),
                    std::move(目标), 类型, 1});
            };
            合同关系(100, 键(10), *自我, 登记_.合同自我关系);
            合同关系(101, 键(10), *提出者, 登记_.合同提出者关系);
            合同关系(102, 键(10), *需求, 登记_.合同需求关系);
            合同关系(103, 键(10), *目标宿主, 登记_.合同目标宿主关系);
            合同关系(104, 键(10), *目标特征, 登记_.合同目标特征关系);
            合同关系(105, 键(10), *目标合同, 登记_.合同目标状态合同关系);
            合同关系(106, 登记_.当前合同索引根, 键(10),
                登记_.当前合同成员关系);
            合同关系(107, 历史账登记_.合同历史索引根, 键(10),
                历史账登记_.合同历史成员关系);
            if (历史覆盖登记_v2_.登记事实代次 != 0)
                合同关系(108, 历史覆盖登记_v2_.合同历史索引根, 键(10),
                    历史覆盖登记_v2_.合同历史成员关系);
            const auto 合同写结果 = 写入端口_.提交所有者范围中性写集(合同写);
            if (合同写结果.状态 != L1所有者范围写入状态::成功
                || 合同写结果.事实代次 == 0)
                return std::nullopt;
            const auto 合同节点 = 查找映射(合同写结果.新编码映射, 10);
            const auto 旧合同当前 = 查找映射(合同写结果.新编码映射, 106);
            if (!合同节点 || !旧合同当前) return std::nullopt;

            服务合同状态事实_v1 初态;
            初态.合同 = {*合同节点};
            初态.状态 = 服务合同当前状态_v1::有效未满足;
            初态.已消费有效秒 = 0;
            初态.最后已结算完整秒边界 = 10;
            初态.状态版本 = 1;
            初态.来源事实 = *初态来源;
            L1所有者范围写集请求 初态写;
            初态写.期望事实代次 = 合同写结果.事实代次;
            初态写.写入幂等身份 = {幂等身份 + 2};
            初态写.节点.push_back({键(10), 节点种类::普通, std::nullopt});
            auto 初态载荷 = 编码状态(初态);
            if (损坏载荷类别 == 2) 初态载荷.pop_back();
            初态写.值.push_back({键(50), 键(10), 登记_.合同状态载荷,
                std::move(初态载荷), 键(10)});
            初态写.属性槽变更.push_back({键(10), 登记_.合同状态载荷, 键(50)});
            初态写.关系.push_back({键(100), *合同节点, 键(10),
                登记_.合同当前状态关系, 1});
            初态写.关系.push_back({键(101), 历史账登记_.合同状态历史索引根,
                键(10), 历史账登记_.合同状态历史成员关系, 1});
            if (历史覆盖登记_v2_.登记事实代次 != 0)
                初态写.关系.push_back({键(102),
                    历史覆盖登记_v2_.合同状态历史索引根, 键(10),
                    历史覆盖登记_v2_.合同状态历史成员关系, 1});
            const auto 初态写结果 = 写入端口_.提交所有者范围中性写集(初态写);
            if (初态写结果.状态 != L1所有者范围写入状态::成功
                || 初态写结果.事实代次 == 0)
                return std::nullopt;
            const auto 旧状态当前 = 查找映射(初态写结果.新编码映射, 100);
            if (!旧状态当前) return std::nullopt;
            if (!形成到期) return L2存在身份{*自我};

            L1所有者范围写集请求 终态写;
            终态写.期望事实代次 = 初态写结果.事实代次;
            终态写.写入幂等身份 = {幂等身份 + 3};
            服务合同状态事实_v1 终态;
            终态.状态 = 服务合同当前状态_v1::已到期;
            终态.已消费有效秒 = 100;
            终态.最后已结算完整秒边界 = 110;
            终态.状态版本 = 2;
            终态.来源事实 = *终态来源;
            服务到期未满足事件事实_v1 事件;
            事件.合同 = {*合同节点};
            事件.需求 = L2需求身份{*需求};
            事件.提出者 = L2存在身份{*提出者};
            事件.冻结有效总秒 = 100;
            事件.实际已消费有效秒 = 100;
            事件.到期完整秒边界 = 110;
            事件.目标未满足事实 = *未满足事实;
            事件.合同终态版本 = 2;
            事件.规则版本 = 1;
            终态写.节点.push_back({键(10), 节点种类::普通, std::nullopt});
            终态写.节点.push_back({键(11), 节点种类::普通, std::nullopt});
            auto 终态载荷 = 编码状态(终态);
            auto 事件载荷 = 编码事件(事件);
            if (损坏载荷类别 == 3) 事件载荷.pop_back();
            终态写.值.push_back({键(50), 键(10), 登记_.合同状态载荷,
                std::move(终态载荷), 键(10)});
            终态写.值.push_back({键(51), 键(11), 登记_.到期事件载荷,
                std::move(事件载荷), 键(11)});
            终态写.属性槽变更.push_back({键(10),
                登记_.合同状态载荷, 键(50)});
            终态写.属性槽变更.push_back({键(11),
                登记_.到期事件载荷, 键(51)});
            std::uint32_t rk = 100;
            const auto r2 = [&](L1所有者范围事实引用 源,
                L1所有者范围事实引用 目标, 稳定编码 类型) {
                终态写.关系.push_back({键(rk++), std::move(源),
                    std::move(目标), 类型, 1});
            };
            r2(*合同节点, 键(10), 登记_.合同当前状态关系);
            r2(历史账登记_.合同状态历史索引根, 键(10),
                历史账登记_.合同状态历史成员关系);
            if (历史覆盖登记_v2_.登记事实代次 != 0)
                r2(历史覆盖登记_v2_.合同状态历史索引根, 键(10),
                    历史覆盖登记_v2_.合同状态历史成员关系);
            r2(登记_.到期事件索引根, 键(11), 登记_.到期事件成员关系);
            r2(键(11), *合同节点, 登记_.到期事件合同关系);
            r2(键(11), *未满足事实, 登记_.到期事件未满足事实关系);
            r2(历史账登记_.到期事件历史索引根, 键(11),
                历史账登记_.到期事件历史成员关系);
            if (历史覆盖登记_v2_.登记事实代次 != 0)
                r2(历史覆盖登记_v2_.到期事件历史索引根, 键(11),
                    历史覆盖登记_v2_.到期事件历史成员关系);
            终态写.退出事实 = {*旧合同当前, *旧状态当前};
            const auto 第二写 = 写入端口_.提交所有者范围中性写集(终态写);
            if (第二写.状态 != L1所有者范围写入状态::成功
                || 第二写.事实代次 == 0)
                return std::nullopt;
            return L2存在身份{*自我};
        } catch (...) {
            return std::nullopt;
        }
    }

    std::optional<L2存在身份> ARCH_建立验证样本_v1(
        std::uint64_t 幂等身份, std::size_t 合同数, std::size_t 事件数,
        bool 终态干扰 = false, bool 损坏载荷 = false,
        std::size_t 进展数 = 0, bool 损坏进展载荷 = false,
        std::size_t 准备数 = 0, bool 损坏准备载荷 = false,
        bool 损坏准备来源 = false, bool 损坏准备关系 = false,
        bool 事件合同到期终态 = false,
        bool 损坏事件终态 = false,
        bool 损坏事件终态版本 = false,
        bool 遗漏事件成员 = false,
        bool 重复事件成员 = false,
        bool 错误事件合同关系 = false,
        bool 遗漏事件终态关系 = false,
        bool 重复事件终态关系 = false,
        const ARCH_服务需求时间裁决验证配置_v1& 时间配置 = {}) noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        try {
            if (幂等身份 == 0 || 合同数 > 32 || 事件数 > 合同数
                || 进展数 > 合同数 || 准备数 > 32
                || (事件合同到期终态 && 终态干扰)
                || ((损坏事件终态 || 损坏事件终态版本
                        || 遗漏事件成员 || 重复事件成员
                        || 错误事件合同关系 || 遗漏事件终态关系
                        || 重复事件终态关系)
                    && (!事件合同到期终态 || 事件数 == 0))
                || (错误事件合同关系 && 合同数 < 2))
                return std::nullopt;
            if ((时间配置.固定自我 && !有效(时间配置.固定自我->值))
                || (时间配置.共同时间纪元身份
                    && *时间配置.共同时间纪元身份 == 0)
                || 时间配置.提出起始完整秒边界 < 0
                || 时间配置.提出步长 < 0 || 时间配置.有效开始偏移 < 0
                || 时间配置.最后已结算完整秒边界 < 0
                || 时间配置.到期完整秒边界 < 0
                || 时间配置.合同规则版本 == 0
                || 时间配置.事件规则版本 == 0
                || (时间配置.重复首个需求 && 合同数 < 2))
                return std::nullopt;
            if (const auto 已有 = ARCH_验证样本记录_.find(幂等身份);
                已有 != ARCH_验证样本记录_.end()) {
                const auto& 记录 = 已有->second;
                if (记录.合同数 != 合同数 || 记录.事件数 != 事件数
                    || 记录.终态干扰 != 终态干扰
                    || 记录.损坏载荷 != 损坏载荷 || 记录.进展数 != 进展数
                    || 记录.损坏进展载荷 != 损坏进展载荷
                    || 记录.准备数 != 准备数
                    || 记录.损坏准备载荷 != 损坏准备载荷
                    || 记录.损坏准备来源 != 损坏准备来源
                    || 记录.损坏准备关系 != 损坏准备关系
                    || 记录.事件合同到期终态 != 事件合同到期终态
                    || 记录.损坏事件终态 != 损坏事件终态
                    || 记录.损坏事件终态版本 != 损坏事件终态版本
                    || 记录.遗漏事件成员 != 遗漏事件成员
                    || 记录.重复事件成员 != 重复事件成员
                    || 记录.错误事件合同关系 != 错误事件合同关系
                    || 记录.遗漏事件终态关系 != 遗漏事件终态关系
                    || 记录.重复事件终态关系 != 重复事件终态关系
                    || 记录.时间配置 != 时间配置)
                    return std::nullopt;
                const auto 节点重放 = 写入端口_.提交所有者范围中性写集(
                    形成验证节点写集(
                        记录.起始事实代次, 幂等身份, 合同数, 事件数,
                        进展数, 准备数, 时间配置));
                if (节点重放.状态 != L1所有者范围写入状态::精确重复
                    || 节点重放.事实代次 != 记录.节点事实代次)
                    return std::nullopt;
                if (合同数 != 0 || 事件数 != 0 || 进展数 != 0 || 准备数 != 0) {
                    const auto 第二幂等身份 =
                        幂等身份 ^ 0x8000'0000'0000'0000ULL;
                    const auto 事实重放 = 写入端口_.提交所有者范围中性写集(
                        形成验证事实写集(记录.节点事实代次, 第二幂等身份,
                            合同数, 事件数, 终态干扰, 损坏载荷,
                            进展数, 损坏进展载荷, 准备数,
                            损坏准备载荷, 损坏准备来源, 损坏准备关系,
                            事件合同到期终态, 损坏事件终态,
                            损坏事件终态版本, 遗漏事件成员,
                            重复事件成员, 错误事件合同关系,
                            遗漏事件终态关系, 重复事件终态关系,
                            时间配置, 记录.映射));
                    if (事实重放.状态 != L1所有者范围写入状态::精确重复
                        || 事实重放.事实代次 != 记录.完成事实代次)
                        return std::nullopt;
                }
                return 记录.自我;
            }
            const auto 当前 = L1_.读取中性当前事实代次({L1中性CRUD合同版本});
            if (当前.状态 != L1中性读取状态::成功 || 当前.事实代次 == 0)
                return std::nullopt;
            const auto 第二幂等身份 = 幂等身份 ^ 0x8000'0000'0000'0000ULL;
            if (第二幂等身份 == 0) return std::nullopt;
            const auto 节点写入 = 写入端口_.提交所有者范围中性写集(
                形成验证节点写集(
                    当前.事实代次, 幂等身份, 合同数, 事件数,
                    进展数, 准备数, 时间配置));
            if ((节点写入.状态 != L1所有者范围写入状态::成功
                    && 节点写入.状态 != L1所有者范围写入状态::精确重复)
                || 节点写入.事实代次 == 0)
                return std::nullopt;
            const auto 自我 = 时间配置.固定自我
                ? std::optional<稳定编码>{时间配置.固定自我->值}
                : 服务合同事实权威内部::查找映射(
                    节点写入.新编码映射, 1U);
            if (!自我) return std::nullopt;
            if (合同数 == 0 && 事件数 == 0 && 进展数 == 0 && 准备数 == 0) {
                ARCH_验证样本记录 记录;
                记录.合同数 = 合同数; 记录.事件数 = 事件数;
                记录.终态干扰 = 终态干扰; 记录.损坏载荷 = 损坏载荷;
                记录.进展数 = 进展数; 记录.损坏进展载荷 = 损坏进展载荷;
                记录.准备数 = 准备数; 记录.损坏准备载荷 = 损坏准备载荷;
                记录.损坏准备来源 = 损坏准备来源;
                记录.损坏准备关系 = 损坏准备关系;
                记录.事件合同到期终态 = 事件合同到期终态;
                记录.损坏事件终态 = 损坏事件终态;
                记录.损坏事件终态版本 = 损坏事件终态版本;
                记录.遗漏事件成员 = 遗漏事件成员;
                记录.重复事件成员 = 重复事件成员;
                记录.错误事件合同关系 = 错误事件合同关系;
                记录.遗漏事件终态关系 = 遗漏事件终态关系;
                记录.重复事件终态关系 = 重复事件终态关系;
                记录.时间配置 = 时间配置;
                记录.起始事实代次 = 当前.事实代次;
                记录.节点事实代次 = 节点写入.事实代次;
                记录.完成事实代次 = 节点写入.事实代次;
                记录.映射 = 节点写入.新编码映射;
                记录.自我 = L2存在身份{*自我};
                ARCH_验证样本记录_.emplace(幂等身份, std::move(记录));
                return L2存在身份{*自我};
            }
            const auto 事实写入 = 写入端口_.提交所有者范围中性写集(
                形成验证事实写集(节点写入.事实代次, 第二幂等身份,
                    合同数, 事件数, 终态干扰, 损坏载荷,
                    进展数, 损坏进展载荷, 准备数,
                    损坏准备载荷, 损坏准备来源, 损坏准备关系,
                    事件合同到期终态, 损坏事件终态,
                    损坏事件终态版本, 遗漏事件成员,
                    重复事件成员, 错误事件合同关系,
                    遗漏事件终态关系, 重复事件终态关系,
                    时间配置, 节点写入.新编码映射));
            if ((事实写入.状态 != L1所有者范围写入状态::成功
                    && 事实写入.状态 != L1所有者范围写入状态::精确重复)
                || 事实写入.事实代次 == 0)
                return std::nullopt;
            ARCH_验证样本记录 记录;
            记录.合同数 = 合同数; 记录.事件数 = 事件数;
            记录.终态干扰 = 终态干扰; 记录.损坏载荷 = 损坏载荷;
            记录.进展数 = 进展数; 记录.损坏进展载荷 = 损坏进展载荷;
            记录.准备数 = 准备数; 记录.损坏准备载荷 = 损坏准备载荷;
            记录.损坏准备来源 = 损坏准备来源;
            记录.损坏准备关系 = 损坏准备关系;
            记录.事件合同到期终态 = 事件合同到期终态;
            记录.损坏事件终态 = 损坏事件终态;
            记录.损坏事件终态版本 = 损坏事件终态版本;
            记录.遗漏事件成员 = 遗漏事件成员;
            记录.重复事件成员 = 重复事件成员;
            记录.错误事件合同关系 = 错误事件合同关系;
            记录.遗漏事件终态关系 = 遗漏事件终态关系;
            记录.重复事件终态关系 = 重复事件终态关系;
            记录.时间配置 = 时间配置;
            记录.起始事实代次 = 当前.事实代次;
            记录.节点事实代次 = 节点写入.事实代次;
            记录.完成事实代次 = 事实写入.事实代次;
            记录.映射 = 节点写入.新编码映射;
            记录.自我 = L2存在身份{*自我};
            ARCH_验证样本记录_.emplace(幂等身份, std::move(记录));
            return L2存在身份{*自我};
        } catch (...) {
            return std::nullopt;
        }
    }

    std::optional<L2存在身份> ARCH_建立需求时间裁决验证样本_v1(
        std::uint64_t 幂等身份, std::size_t 合同数, std::size_t 事件数,
        bool 事件合同到期终态,
        const ARCH_服务需求时间裁决验证配置_v1& 时间配置) noexcept {
        return ARCH_建立验证样本_v1(幂等身份, 合同数, 事件数,
            false, false, 0, false, 0, false, false, false,
            事件合同到期终态, false, false, false, false, false,
            false, false, 时间配置);
    }

    std::optional<L2存在身份> ARCH_建立v2验证样本(
        std::uint64_t 幂等身份, std::size_t 进展数, std::size_t 准备数,
        bool 损坏绑定 = false, bool 损坏关系 = false,
        bool 损坏载荷 = false, bool 损坏集合 = false) noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        try {
            if (幂等身份 == 0 || 进展数 > 32 || 准备数 > 32
                || ((损坏绑定 || 损坏关系 || 损坏载荷 || 损坏集合)
                    && 进展数 == 0 && 准备数 == 0)) return std::nullopt;
            if (const auto it = ARCH_v2验证样本记录_.find(幂等身份);
                it != ARCH_v2验证样本记录_.end()) {
                const auto& x = it->second;
                if (x.进展数 != 进展数 || x.准备数 != 准备数
                    || x.损坏绑定 != 损坏绑定 || x.损坏关系 != 损坏关系
                    || x.损坏载荷 != 损坏载荷 || x.损坏集合 != 损坏集合)
                    return std::nullopt;
                const auto n = 写入端口_.提交所有者范围中性写集(
                    形成v2验证节点写集(x.起始事实代次, 幂等身份, 进展数, 准备数));
                if (n.状态 != L1所有者范围写入状态::精确重复
                    || n.事实代次 != x.节点事实代次) return std::nullopt;
                if (进展数 != 0 || 准备数 != 0) {
                    const auto f = 写入端口_.提交所有者范围中性写集(
                        形成v2验证事实写集(x.节点事实代次,
                            幂等身份 ^ 0x4000'0000'0000'0000ULL,
                            进展数, 准备数, 损坏绑定, 损坏关系,
                            损坏载荷, 损坏集合, x.映射));
                    if (f.状态 != L1所有者范围写入状态::精确重复
                        || f.事实代次 != x.完成事实代次) return std::nullopt;
                }
                return x.自我;
            }
            const auto 当前 = L1_.读取中性当前事实代次({L1中性CRUD合同版本});
            if (当前.状态 != L1中性读取状态::成功 || 当前.事实代次 == 0)
                return std::nullopt;
            const auto n = 写入端口_.提交所有者范围中性写集(
                形成v2验证节点写集(当前.事实代次, 幂等身份, 进展数, 准备数));
            if ((n.状态 != L1所有者范围写入状态::成功
                && n.状态 != L1所有者范围写入状态::精确重复)
                || n.事实代次 == 0) return std::nullopt;
            const auto s = 服务合同事实权威内部::查找映射(n.新编码映射, 1U);
            if (!s) return std::nullopt;
            std::uint64_t 完成 = n.事实代次;
            if (进展数 != 0 || 准备数 != 0) {
                const auto f = 写入端口_.提交所有者范围中性写集(
                    形成v2验证事实写集(n.事实代次,
                        幂等身份 ^ 0x4000'0000'0000'0000ULL,
                        进展数, 准备数, 损坏绑定, 损坏关系,
                        损坏载荷, 损坏集合, n.新编码映射));
                if ((f.状态 != L1所有者范围写入状态::成功
                    && f.状态 != L1所有者范围写入状态::精确重复)
                    || f.事实代次 == 0) return std::nullopt;
                完成 = f.事实代次;
            }
            ARCH_v2验证样本记录 x{进展数, 准备数, 损坏绑定, 损坏关系,
                损坏载荷, 损坏集合, 当前.事实代次, n.事实代次, 完成,
                n.新编码映射, L2存在身份{*s}};
            ARCH_v2验证样本记录_.emplace(幂等身份, x);
            return x.自我;
        } catch (...) { return std::nullopt; }
    }

    bool ARCH_损坏v2结构登记(bool 准备) noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        try {
            const auto 当前 = L1_.读取中性当前事实代次({L1中性CRUD合同版本});
            if (当前.状态 != L1中性读取状态::成功 || 当前.事实代次 == 0)
                return false;
            L1所有者范围写集请求 w; w.期望事实代次 = 当前.事实代次;
            w.写入幂等身份 = {0x494E'5354'5632'4200ULL + ARCH_v2损坏序号_};
            const auto k = 服务合同事实权威内部::键(
                static_cast<std::uint32_t>(0x76000000ULL + ARCH_v2损坏序号_++));
            const auto 锚点 = 准备 ? 准备登记_v2_.登记锚点 : 进展登记_v2_.登记锚点;
            const auto 属性 = 准备 ? 准备登记_v2_.登记版本载荷
                                   : 进展登记_v2_.登记版本载荷;
            w.值.push_back({k, 锚点, 属性, std::vector<std::uint64_t>{1}, 锚点});
            w.属性槽变更.push_back({锚点, 属性, k});
            const auto 写 = 写入端口_.提交所有者范围中性写集(w);
            return 写.状态 == L1所有者范围写入状态::成功 && 写.事实代次 != 0;
        } catch (...) { return false; }
    }

    bool ARCH_损坏当前v2事实引用关系(bool 准备, 稳定编码 事实,
        std::uint8_t 模式, 稳定编码 替代目标 = {}) noexcept {
        using namespace 服务合同事实权威内部;
        std::lock_guard<std::mutex> 锁(互斥体_);
        try {
            if (!有效(事实) || 模式 == 0 || 模式 > 3
                || ((模式 == 1 || 模式 == 3) && !有效(替代目标)))
                return false;
            const auto 当前 = L1_.读取中性当前事实代次({L1中性CRUD合同版本});
            if (当前.状态 != L1中性读取状态::成功 || 当前.事实代次 == 0)
                return false;
            const auto 所有者 = 写入端口_.所有者身份();
            const auto 当前闭包读取 = L1_.尝试读取所有者范围一致关系类型闭包投影(
                准备 ? 形成准备读取请求_v2(当前.事实代次)
                     : 形成进展读取请求_v2(当前.事实代次));
            const auto 当前索引根 = 准备
                ? 准备登记_v2_.当前索引根 : 进展登记_v2_.当前索引根;
            const auto 当前成员关系 = 准备
                ? 准备登记_v2_.当前成员关系 : 进展登记_v2_.当前成员关系;
            const auto 载荷类型 = 准备
                ? 准备登记_v2_.载荷 : 进展登记_v2_.载荷;
            if (当前闭包读取.状态 != L1所有者范围一致当前读取状态::成功
                || 当前闭包读取.读取事实代次 != 当前.事实代次
                || 当前闭包读取.关系类型闭包.size() != 1)
                return false;
            if (准备) {
                if (!验证v2共同登记(当前闭包读取, 准备登记_v2_,
                        服务准备事实结构登记版本_v2, 当前.事实代次))
                    return false;
            } else if (!验证v2共同登记(当前闭包读取, 进展登记_v2_,
                    服务进展事实结构登记版本_v2, 当前.事实代次)) {
                return false;
            }
            const auto* 当前闭包 = 查找闭包(
                当前闭包读取, 当前成员关系);
            if (!当前闭包) return false;
            const L1所有者范围一致关系类型闭包成员* 当前成员 = nullptr;
            for (const auto& 成员 : 当前闭包->成员) {
                if (成员.目标节点.编码 != 事实) continue;
                if (当前成员) return false;
                当前成员 = &成员;
            }
            if (!当前成员
                || 当前成员->关系.写入所有者 != 所有者
                || 当前成员->关系.源节点 != 当前索引根
                || 当前成员->关系.目标节点 != 事实
                || 当前成员->关系.关系类型节点 != 当前成员关系
                || 当前成员->关系.角色或顺序 != 1
                || !活动于(当前成员->关系, 当前.事实代次)
                || 当前成员->目标节点.写入所有者 != 所有者
                || !活动于(当前成员->目标节点, 当前.事实代次))
                return false;
            const auto* 载荷投影 = 查找属性投影(
                当前成员->目标节点属性值, 事实, 载荷类型);
            if (!载荷投影) return false;
            if (准备) {
                服务准备当前事实_v2 载荷;
                if (!解码准备_v2(载荷投影->当前值事实, 事实, 载荷)
                    || 载荷.形成事实代次 != 当前成员->目标节点.创建事实代次
                    || !服务准备当前闭包引用完整_v2(
                        *当前成员, 载荷, 准备登记_v2_, 当前.事实代次))
                    return false;
            } else {
                服务合同关联进展事实_v2 载荷;
                if (!解码进展_v2(载荷投影->当前值事实, 事实, 载荷)
                    || 载荷.形成事实代次 != 当前成员->目标节点.创建事实代次
                    || !服务进展当前闭包引用完整_v2(
                        *当前成员, 载荷, 进展登记_v2_, 当前.事实代次))
                    return false;
            }
            const auto 类型 = 准备
                ? 模式 == 1 ? 准备登记_v2_.正式选择关系
                    : 模式 == 2 ? 准备登记_v2_.状态引用关系
                                : 准备登记_v2_.动态引用关系
                : 模式 == 1 ? 进展登记_v2_.正式选择关系
                    : 模式 == 2 ? 进展登记_v2_.状态引用关系
                                : 进展登记_v2_.动态引用关系;
            const auto 关系组 = L1_.读取所有者范围历史关系组({
                L1所有者范围CRUD合同版本,
                L1所有者范围关系端点方向::源,
                事实, 类型, 当前.事实代次});
            if (关系组.状态 != L1所有者范围读取状态::成功
                || 关系组.读取事实代次 != 当前.事实代次)
                return false;
            for (const auto& 关系 : 关系组.关系组)
                if (关系.写入所有者 != 写入端口_.所有者身份()
                    || 关系.源节点 != 事实 || 关系.关系类型节点 != 类型
                    || !活动于(关系, 当前.事实代次))
                    return false;
            if (模式 == 3) {
                if (!关系组.关系组.empty()) return false;
            } else {
                if (关系组.关系组.size() != 1) return false;
                if (模式 == 1
                    && 关系组.关系组.front().目标节点 == 替代目标)
                    return false;
            }
            L1所有者范围写集请求 写集;
            写集.期望事实代次 = 当前.事实代次;
            写集.写入幂等身份 = {
                0x5348'5632'5246'0000ULL + ARCH_v2事实引用损坏序号_++};
            if (模式 != 3)
                写集.退出事实 = {关系组.关系组.front().编码};
            if (模式 != 2)
                写集.关系.push_back({键(1), 事实, 替代目标, 类型, 1});
            const auto 写 = 写入端口_.提交所有者范围中性写集(写集);
            return 写.状态 == L1所有者范围写入状态::成功
                && 写.事实代次 != 0;
        } catch (...) {
            return false;
        }
    }

    void ARCH_注入读中漂移一次() noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        ARCH_读中漂移一次_ = true;
    }

    void ARCH_注入资源失败一次() noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        ARCH_资源失败一次_ = true;
    }

    void ARCH_注入发布后读回失败一次() noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        ARCH_发布后读回失败一次_ = true;
    }

    void ARCH_注入G0终态错配一次() noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        ARCH_G0终态错配一次_ = true;
    }

    static bool ARCH_验证历史后继按业务流分组_v2() noexcept {
        using namespace 服务合同事实权威内部;
        服务进展历史投影_v1 a0, a1, b0, b1;
        a0.事实.自我 = a1.事实.自我 = b0.事实.自我 = b1.事实.自我
            = L2存在身份{稳定编码{1}};
        a0.事实.服务合同 = a1.事实.服务合同 = b0.事实.服务合同
            = b1.事实.服务合同 = 服务合同身份_v1{稳定编码{2}};
        a0.事实.需求 = a1.事实.需求 = b0.事实.需求 = b1.事实.需求
            = L2需求身份{稳定编码{3}};
        a0.事实.任务 = a1.事实.任务 = L2任务身份{稳定编码{4}};
        b0.事实.任务 = b1.事实.任务 = L2任务身份{稳定编码{5}};
        a0.事实.方法 = a1.事实.方法 = b0.事实.方法 = b1.事实.方法
            = L2方法身份{稳定编码{6}};
        a0.事实.T到D关系稳定编码 = a1.事实.T到D关系稳定编码
            = 稳定编码{7};
        b0.事实.T到D关系稳定编码 = b1.事实.T到D关系稳定编码
            = 稳定编码{8};
        a0.事实.形成事实代次 = 1;
        b0.事实.形成事实代次 = 2;
        a1.事实.形成事实代次 = b1.事实.形成事实代次 = 3;
        a0.退出当前事实代次 = b0.退出当前事实代次 = 3;
        const std::vector<服务进展历史投影_v1> 完整{a0, b0, a1, b1};
        const std::vector<服务进展历史投影_v1> 缺少A后继{a0, b0, b1};
        return 历史当前版本链按流闭合_v2(
                完整, 同一服务进展活动流事实_v2)
            && !历史当前版本链按流闭合_v2(
                缺少A后继, 同一服务进展活动流事实_v2);
    }

    bool ARCH_退出历史成员关系_v1(
        服务维护历史事实类别_v1 类别, 稳定编码 事实,
        std::uint64_t 形成事实代次, bool v2覆盖根 = false) noexcept {
        using namespace 服务合同事实权威内部;
        std::lock_guard<std::mutex> 锁(互斥体_);
        try {
            稳定编码 根{}, 类型{};
            switch (类别) {
            case 服务维护历史事实类别_v1::服务合同:
                根 = v2覆盖根 ? 历史覆盖登记_v2_.合同历史索引根
                              : 历史账登记_.合同历史索引根;
                类型 = v2覆盖根 ? 历史覆盖登记_v2_.合同历史成员关系
                                 : 历史账登记_.合同历史成员关系;
                break;
            case 服务维护历史事实类别_v1::合同状态:
                根 = v2覆盖根 ? 历史覆盖登记_v2_.合同状态历史索引根
                              : 历史账登记_.合同状态历史索引根;
                类型 = v2覆盖根 ? 历史覆盖登记_v2_.合同状态历史成员关系
                                 : 历史账登记_.合同状态历史成员关系;
                break;
            case 服务维护历史事实类别_v1::到期未满足事件:
                根 = v2覆盖根 ? 历史覆盖登记_v2_.到期事件历史索引根
                              : 历史账登记_.到期事件历史索引根;
                类型 = v2覆盖根 ? 历史覆盖登记_v2_.到期事件历史成员关系
                                 : 历史账登记_.到期事件历史成员关系;
                break;
            case 服务维护历史事实类别_v1::服务进展:
                根 = v2覆盖根 ? 历史覆盖登记_v2_.进展历史索引根
                              : 历史账登记_.进展v2历史索引根;
                类型 = v2覆盖根 ? 历史覆盖登记_v2_.进展历史成员关系
                                 : 历史账登记_.进展v2历史成员关系;
                break;
            case 服务维护历史事实类别_v1::服务准备:
                根 = v2覆盖根 ? 历史覆盖登记_v2_.准备历史索引根
                              : 历史账登记_.准备v2历史索引根;
                类型 = v2覆盖根 ? 历史覆盖登记_v2_.准备历史成员关系
                                 : 历史账登记_.准备v2历史成员关系;
                break;
            default:
                return false;
            }
            const auto G0 = 当前事实代次_已锁定();
            if (!G0 || !有效(事实) || 形成事实代次 == 0
                || 形成事实代次 > *G0)
                return false;
            const auto 关系 = v2覆盖根
                ? v2永久历史成员关系_已锁定(
                    事实, 形成事实代次, 根, 类型, *G0)
                : 历史成员关系_已锁定(
                    事实, 根, 类型, 形成事实代次, *G0);
            if (!关系 || 关系->退出事实代次) return false;
            L1所有者范围写集请求 写集;
            写集.期望事实代次 = *G0;
            写集.写入幂等身份 = {
                0x5348'444D'4700'0000ULL + ARCH_历史损坏序号_++};
            写集.退出事实 = {关系->编码};
            const auto 写 = 写入端口_.提交所有者范围中性写集(写集);
            return 写.状态 == L1所有者范围写入状态::成功
                && 写.事实代次 != 0;
        } catch (...) {
            return false;
        }
    }

    bool ARCH_追加重复进展历史成员关系_v1(稳定编码 事实) noexcept {
        using namespace 服务合同事实权威内部;
        std::lock_guard<std::mutex> 锁(互斥体_);
        try {
            const auto G0 = 当前事实代次_已锁定();
            if (!G0 || !有效(事实)) return false;
            L1所有者范围写集请求 写集;
            写集.期望事实代次 = *G0;
            写集.写入幂等身份 = {
                0x5348'444D'4700'0000ULL + ARCH_历史损坏序号_++};
            写集.关系.push_back({键(1), 历史账登记_.进展v2历史索引根,
                事实, 历史账登记_.进展v2历史成员关系, 1});
            const auto 写 = 写入端口_.提交所有者范围中性写集(写集);
            return 写.状态 == L1所有者范围写入状态::成功
                && 写.事实代次 != 0;
        } catch (...) {
            return false;
        }
    }

    bool ARCH_追加无载荷进展历史成员_v1() noexcept {
        using namespace 服务合同事实权威内部;
        std::lock_guard<std::mutex> 锁(互斥体_);
        try {
            const auto G0 = 当前事实代次_已锁定();
            if (!G0) return false;
            L1所有者范围写集请求 写集;
            写集.期望事实代次 = *G0;
            写集.写入幂等身份 = {
                0x5348'444D'4700'0000ULL + ARCH_历史损坏序号_++};
            写集.节点.push_back({键(1), 节点种类::普通, std::nullopt});
            写集.关系.push_back({键(2), 历史账登记_.进展v2历史索引根,
                键(1), 历史账登记_.进展v2历史成员关系, 1});
            const auto 写 = 写入端口_.提交所有者范围中性写集(写集);
            return 写.状态 == L1所有者范围写入状态::成功
                && 写.事实代次 != 0;
        } catch (...) {
            return false;
        }
    }
#endif

private:
    static L1所有者范围写端口&& 验证并移动交付(
        const L1事实基座服务& 服务,
        服务合同事实权威所有者交付_v1& 交付) {
        if (!交付.有效() || !交付.绑定于(服务))
            throw std::invalid_argument("invalid service contract owner delivery");
        return std::move(交付.写入端口_);
    }

    static bool 请求有效(std::uint32_t 合同版本,
        const L2结构请求头& 请求头, L2存在身份 自我) noexcept {
        return 合同版本 == 服务合同事实权威合同版本_v1
            && 请求头.合同版本 == L2结构合同版本
            && 请求头.期望事实代次 != 0 && 有效(自我.值);
    }

    static bool 进展请求有效(const 服务进展完整集合读取请求_v1& 请求) noexcept {
        return 请求.合同版本 == 服务进展事实扩展合同版本_v1
            && 请求.请求头.合同版本 == L2结构合同版本
            && 请求.请求头.期望事实代次 != 0 && 有效(请求.自我.值);
    }
    static bool 准备请求有效(const 服务准备完整集合读取请求_v1& 请求) noexcept {
        return 请求.合同版本 == 服务准备事实扩展合同版本_v1
            && 请求.请求头.合同版本 == L2结构合同版本
            && 请求.请求头.期望事实代次 != 0 && 有效(请求.自我.值);
    }
    static bool 进展请求有效(const 服务进展完整集合读取请求_v2& 请求) noexcept {
        return 请求.合同版本 == 服务进展事实扩展合同版本_v2
            && 请求.请求头.合同版本 == L2结构合同版本
            && 请求.请求头.期望事实代次 != 0 && 有效(请求.自我.值);
    }
    static bool 准备请求有效(const 服务准备完整集合读取请求_v2& 请求) noexcept {
        return 请求.合同版本 == 服务准备事实扩展合同版本_v2
            && 请求.请求头.合同版本 == L2结构合同版本
            && 请求.请求头.期望事实代次 != 0 && 有效(请求.自我.值);
    }

    static 服务活动事实发布状态_v2 映射写入到发布状态(
        L1所有者范围写入状态 状态) noexcept {
        switch (状态) {
        case L1所有者范围写入状态::成功:
            return 服务活动事实发布状态_v2::已发布;
        case L1所有者范围写入状态::精确重复:
            return 服务活动事实发布状态_v2::精确重复;
        case L1所有者范围写入状态::入口拒绝:
            return 服务活动事实发布状态_v2::入口拒绝;
        case L1所有者范围写入状态::许可拒绝:
            return 服务活动事实发布状态_v2::许可拒绝;
        case L1所有者范围写入状态::事实代次漂移:
            return 服务活动事实发布状态_v2::当前性漂移;
        case L1所有者范围写入状态::幂等冲突:
            return 服务活动事实发布状态_v2::幂等冲突;
        case L1所有者范围写入状态::引用冲突:
        case L1所有者范围写入状态::未找到:
        case L1所有者范围写入状态::已退出:
            return 服务活动事实发布状态_v2::引用冲突;
        case L1所有者范围写入状态::资源失败:
            return 服务活动事实发布状态_v2::资源失败;
        case L1所有者范围写入状态::内部不一致:
            return 服务活动事实发布状态_v2::内部错误;
        }
        return 服务活动事实发布状态_v2::内部错误;
    }

    static 服务活动事实发布状态_v2 映射v2读取到发布状态(
        服务合同事实权威读取状态_v2 状态) noexcept {
        switch (状态) {
        case 服务合同事实权威读取状态_v2::入口拒绝:
            return 服务活动事实发布状态_v2::入口拒绝;
        case 服务合同事实权威读取状态_v2::许可拒绝:
            return 服务活动事实发布状态_v2::许可拒绝;
        case 服务合同事实权威读取状态_v2::当前性漂移:
            return 服务活动事实发布状态_v2::当前性漂移;
        case 服务合同事实权威读取状态_v2::结构未登记:
            return 服务活动事实发布状态_v2::结构未登记;
        case 服务合同事实权威读取状态_v2::集合不闭合:
            return 服务活动事实发布状态_v2::集合不闭合;
        case 服务合同事实权威读取状态_v2::引用冲突:
            return 服务活动事实发布状态_v2::引用冲突;
        case 服务合同事实权威读取状态_v2::资源失败:
            return 服务活动事实发布状态_v2::资源失败;
        case 服务合同事实权威读取状态_v2::内部错误:
            return 服务活动事实发布状态_v2::内部错误;
        case 服务合同事实权威读取状态_v2::已读取:
            return 服务活动事实发布状态_v2::内部错误;
        }
        return 服务活动事实发布状态_v2::内部错误;
    }

    bool 读前守卫(std::uint64_t G0) const noexcept { return 读后守卫(G0); }
    bool 读后守卫(std::uint64_t G0) const noexcept {
        const auto 当前 = L1_.读取中性当前事实代次({L1中性CRUD合同版本});
        return 当前.状态 == L1中性读取状态::成功 && 当前.事实代次 == G0;
    }

    std::optional<std::uint64_t> 当前事实代次_已锁定() const noexcept {
        const auto 当前 = L1_.读取中性当前事实代次({L1中性CRUD合同版本});
        if (当前.状态 != L1中性读取状态::成功 || 当前.事实代次 == 0)
            return std::nullopt;
        return 当前.事实代次;
    }

    template<class 登记类型>
    std::optional<std::vector<稳定编码>> 收集退出闭包_已锁定(
        std::uint64_t G0, 稳定编码 事实身份, const 登记类型& d,
        L1所有者范围一致关系类型闭包读取请求 请求) const {
        using namespace 服务合同事实权威内部;
        const auto 读 = L1_.尝试读取所有者范围一致关系类型闭包投影(请求);
        if (读.状态 != L1所有者范围一致当前读取状态::成功
            || 读.读取事实代次 != G0 || !读后守卫(G0)) return std::nullopt;
        const auto* 闭包 = 查找闭包(读, d.当前成员关系);
        if (!闭包) return std::nullopt;
        const L1所有者范围一致关系类型闭包成员* 命中 = nullptr;
        for (const auto& 成员 : 闭包->成员) {
            if (成员.目标节点.编码 != 事实身份) continue;
            if (命中) return std::nullopt;
            命中 = &成员;
        }
        if (!命中 || 命中->关系.写入所有者 != 写入端口_.所有者身份()
            || 命中->目标节点.写入所有者 != 写入端口_.所有者身份())
            return std::nullopt;
        const auto* p = 查找属性投影(
            命中->目标节点属性值, 事实身份, d.载荷);
        if (!p || p->当前值事实.写入所有者 != 写入端口_.所有者身份())
            return std::nullopt;
        std::vector<稳定编码> 退出{事实身份, 命中->关系.编码,
            p->当前值事实.编码};
        for (const auto& 组 : 命中->目标节点源关系组) {
            if (组.状态 != L1所有者范围一致当前读取项目状态::成功
                || !组.关系类型事实) return std::nullopt;
            for (const auto& x : 组.成员) {
                if (x.关系.源节点 != 事实身份
                    || x.关系.写入所有者 != 写入端口_.所有者身份())
                    return std::nullopt;
                退出.push_back(x.关系.编码);
            }
        }
        std::sort(退出.begin() + 1, 退出.end(),
            [](稳定编码 a, 稳定编码 b) { return a.值 < b.值; });
        if (std::adjacent_find(退出.begin() + 1, 退出.end()) != 退出.end())
            return std::nullopt;
        return 退出;
    }

    std::optional<稳定编码> 历史唯一目标_已锁定(
        稳定编码 源, 稳定编码 类型, std::uint64_t G1,
        std::uint64_t Gread) const {
        const auto r = L1_.读取所有者范围历史关系组({
            L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::源,
            源, 类型, G1});
        if (r.状态 != L1所有者范围读取状态::成功
            || r.读取事实代次 != Gread || r.关系组.size() != 1)
            return std::nullopt;
        const auto& x = r.关系组.front();
        if (x.源节点 != 源 || x.关系类型节点 != 类型
            || x.创建事实代次 > G1
            || (x.退出事实代次 && *x.退出事实代次 <= G1))
            return std::nullopt;
        return x.目标节点;
    }

    bool 历史可选目标匹配_已锁定(稳定编码 源, 稳定编码 类型,
        const std::optional<稳定编码>& 期望, std::uint64_t G1,
        std::uint64_t Gread) const {
        const auto r = L1_.读取所有者范围历史关系组({
            L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::源,
            源, 类型, G1});
        if (r.状态 != L1所有者范围读取状态::成功
            || r.读取事实代次 != Gread) return false;
        if (!期望) return r.关系组.empty();
        return r.关系组.size() == 1 && r.关系组.front().目标节点 == *期望
            && r.关系组.front().创建事实代次 <= G1
            && (!r.关系组.front().退出事实代次
                || *r.关系组.front().退出事实代次 > G1);
    }

    bool 历史成员匹配_已锁定(稳定编码 事实, 稳定编码 索引根,
        稳定编码 类型, std::uint64_t G1, std::uint64_t Gread) const {
        const auto r = L1_.读取所有者范围历史关系组({
            L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::目标,
            事实, 类型, G1});
        return r.状态 == L1所有者范围读取状态::成功
            && r.读取事实代次 == Gread && r.关系组.size() == 1
            && r.关系组.front().源节点 == 索引根
            && r.关系组.front().目标节点 == 事实
            && r.关系组.front().创建事实代次 <= G1
            && (!r.关系组.front().退出事实代次
                || *r.关系组.front().退出事实代次 > G1);
    }

    std::optional<L1所有者范围关系事实> 历史成员关系_已锁定(
        稳定编码 事实, 稳定编码 索引根, 稳定编码 类型,
        std::uint64_t G1, std::uint64_t Gread) const {
        const auto r = L1_.读取所有者范围历史关系组({
            L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::目标,
            事实, 类型, G1});
        if (r.状态 != L1所有者范围读取状态::成功
            || r.读取事实代次 != Gread || r.关系组.size() != 1)
            return std::nullopt;
        const auto& x = r.关系组.front();
        if (x.源节点 != 索引根 || x.目标节点 != 事实
            || x.关系类型节点 != 类型 || x.角色或顺序 != 1
            || x.写入所有者 != 写入端口_.所有者身份()
            || x.创建事实代次 != G1
            || (x.退出事实代次 && *x.退出事实代次 <= G1))
            return std::nullopt;
        return x;
    }

    std::optional<L1所有者范围关系事实> v2永久历史成员关系_已锁定(
        稳定编码 事实, std::uint64_t 事实形成代次, 稳定编码 索引根,
        稳定编码 类型, std::uint64_t Gread) const {
        if (历史覆盖登记_v2_.登记事实代次 == 0
            || 事实形成代次 == 历史覆盖登记_v2_.登记事实代次)
            return std::nullopt;
        const auto r = L1_.读取所有者范围历史关系组({
            L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::目标,
            事实, 类型, Gread});
        if (r.状态 != L1所有者范围读取状态::成功
            || r.读取事实代次 != Gread || r.关系组.size() != 1)
            return std::nullopt;
        const auto& x = r.关系组.front();
        const auto 预期形成 = 事实形成代次 < 历史覆盖登记_v2_.登记事实代次
            ? 历史覆盖登记_v2_.登记事实代次 : 事实形成代次;
        if (x.源节点 != 索引根 || x.目标节点 != 事实
            || x.关系类型节点 != 类型 || x.角色或顺序 != 1
            || x.写入所有者 != 写入端口_.所有者身份()
            || x.创建事实代次 != 预期形成 || x.退出事实代次)
            return std::nullopt;
        return x;
    }

    bool v2精确重复历史形状同义_已锁定(
        稳定编码 事实, std::uint64_t 事实形成代次,
        稳定编码 当前索引根, 稳定编码 当前类型,
        稳定编码 历史索引根, 稳定编码 历史类型,
        std::uint64_t Gread) const {
        if (历史覆盖登记_v2_.登记事实代次 == 0
            || 事实形成代次 == 历史覆盖登记_v2_.登记事实代次)
            return false;
        if (事实形成代次 > 历史覆盖登记_v2_.登记事实代次)
            return v2永久历史成员关系_已锁定(
                事实, 事实形成代次, 历史索引根, 历史类型, Gread).has_value();
        const auto Gseed前 = 历史覆盖登记_v2_.登记事实代次 - 1;
        const auto 当前 = L1_.读取所有者范围历史关系组({
            L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::目标,
            事实, 当前类型, Gseed前});
        if (当前.状态 != L1所有者范围读取状态::成功
            || 当前.读取事实代次 != Gread || 当前.关系组.size() > 1)
            return false;
        const auto 历史 = L1_.读取所有者范围历史关系组({
            L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::目标,
            事实, 历史类型, Gread});
        if (历史.状态 != L1所有者范围读取状态::成功
            || 历史.读取事实代次 != Gread || 历史.关系组.size() > 1)
            return false;
        if (当前.关系组.empty()) return 历史.关系组.empty();
        const auto& c = 当前.关系组.front();
        if (c.源节点 != 当前索引根 || c.目标节点 != 事实
            || c.关系类型节点 != 当前类型 || c.角色或顺序 != 1
            || c.写入所有者 != 写入端口_.所有者身份()
            || c.创建事实代次 != 事实形成代次 || !服务合同事实权威内部::活动于(c, Gseed前))
            return false;
        const auto h = v2永久历史成员关系_已锁定(
            事实, 事实形成代次, 历史索引根, 历史类型, Gread);
        return h.has_value();
    }

    template<class 事实身份类型, class 登记类型>
    bool 首次发布历史形状同义_已锁定(
        const L1所有者范围首次写入读取结果& 首次,
        const std::optional<事实身份类型>& 预期当前事实,
        const 登记类型& d, 稳定编码 历史索引根,
        稳定编码 历史成员关系, std::uint64_t G1,
        std::uint64_t Gread) const {
        if (!首次.首次规范化写集) return false;
        const auto& w = *首次.首次规范化写集;
        if (G1 == 历史账登记_.登记事实代次) return false;
        if (G1 < 历史账登记_.登记事实代次) {
            if (!预期当前事实) return w.退出事实.empty();
            return !w.退出事实.empty()
                && w.退出事实.front() == 预期当前事实->值;
        }
        const auto 身份 = 首次.首次写入结果
            ? 服务合同事实权威内部::查找映射(
                首次.首次写入结果->新编码映射,
                服务合同事实权威内部::发布事实节点本地键)
            : std::nullopt;
        if (!身份) return false;
        const auto 历史 = 历史成员关系_已锁定(
            *身份, 历史索引根, 历史成员关系, G1, Gread);
        if (!历史 || 历史->退出事实代次) return false;
        if (!预期当前事实) return w.退出事实.empty();
        const auto 当前 = 历史成员关系_已锁定(
            预期当前事实->值, d.当前索引根, d.当前成员关系,
            G1, Gread);
        return 当前 && w.退出事实.size() == 1
            && w.退出事实.front() == 当前->编码;
    }

    template<class 登记类型>
    std::optional<std::vector<稳定编码>> 形成版本化退出组_已锁定(
        稳定编码 旧事实身份, std::uint64_t 旧事实形成代次,
        const 登记类型& d, 稳定编码 历史索引根,
        稳定编码 历史成员关系, 稳定编码 v2历史索引根,
        稳定编码 v2历史成员关系, std::uint64_t G0,
        L1所有者范围一致关系类型闭包读取请求 兼容读取请求) const {
        (void)兼容读取请求;
#if defined(ARCH_INSTINCT_SERVICE_CONTRACT_FACT_AUTHORITY_VALIDATION)
        if (历史覆盖登记_v2_.登记事实代次 == 0) {
            if (旧事实形成代次 == 历史账登记_.登记事实代次)
                return std::nullopt;
            if (旧事实形成代次 < 历史账登记_.登记事实代次)
                return 收集退出闭包_已锁定(
                    G0, 旧事实身份, d, std::move(兼容读取请求));
            const auto 当前 = 历史成员关系_已锁定(
                旧事实身份, d.当前索引根, d.当前成员关系,
                旧事实形成代次, G0);
            const auto 历史 = 历史成员关系_已锁定(
                旧事实身份, 历史索引根, 历史成员关系,
                旧事实形成代次, G0);
            if (!当前 || !历史 || 当前->退出事实代次 || 历史->退出事实代次)
                return std::nullopt;
            return std::vector<稳定编码>{当前->编码};
        }
#endif
        if (旧事实形成代次 == 历史账登记_.登记事实代次)
            return std::nullopt;
        const auto 当前 = 历史成员关系_已锁定(
            旧事实身份, d.当前索引根, d.当前成员关系,
            旧事实形成代次, G0);
        const auto v2历史 = v2永久历史成员关系_已锁定(
            旧事实身份, 旧事实形成代次,
            v2历史索引根, v2历史成员关系, G0);
        const auto v1历史 = 旧事实形成代次 < 历史账登记_.登记事实代次
            ? std::optional<L1所有者范围关系事实>{}
            : 历史成员关系_已锁定(旧事实身份, 历史索引根,
                历史成员关系, 旧事实形成代次, G0);
        if (!当前 || 当前->退出事实代次 || !v2历史
            || (旧事实形成代次 > 历史账登记_.登记事实代次 && !v1历史))
            return std::nullopt;
        return std::vector<稳定编码>{当前->编码};
    }

    template<class 事实类型, class 登记类型, class 解码函数>
    std::optional<事实类型> 读取首次历史载荷_已锁定(
        稳定编码 身份, std::uint64_t G1, std::uint64_t Gread,
        const 登记类型& d, 解码函数 解码) const {
        const auto n = L1_.读取所有者范围历史事实(
            {L1所有者范围CRUD合同版本, 身份});
        if (n.状态 != L1所有者范围读取状态::成功
            || n.读取事实代次 != Gread || !n.事实
            || !std::holds_alternative<L1所有者范围节点事实>(*n.事实))
            return std::nullopt;
        const auto& 节点 = std::get<L1所有者范围节点事实>(*n.事实);
        if (节点.编码 != 身份 || 节点.写入所有者 != 写入端口_.所有者身份()
            || 节点.创建事实代次 != G1) return std::nullopt;
        const auto values = L1_.读取所有者范围历史属性值组(
            {L1所有者范围CRUD合同版本, 身份, G1});
        if (values.状态 != L1所有者范围读取状态::成功
            || values.读取事实代次 != Gread) return std::nullopt;
        const L1所有者范围值事实* value = nullptr;
        for (const auto& v : values.属性值组) {
            if (v.属性类型节点 != d.载荷 || v.创建事实代次 > G1
                || (v.退出事实代次 && *v.退出事实代次 <= G1)) continue;
            if (value) return std::nullopt;
            value = &v;
        }
        事实类型 f;
        if (!value || !解码(*value, 身份, f)) return std::nullopt;
        f.生命周期.退出事实代次 = 节点.退出事实代次;
        return f;
    }


    std::optional<服务合同关联进展事实_v2> 读取进展首次历史闭包_已锁定(
        稳定编码 身份, std::uint64_t G1, std::uint64_t Gread) const {
        using namespace 服务合同事实权威内部;
        auto f = 读取首次历史载荷_已锁定<服务合同关联进展事实_v2>(
            身份, G1, Gread, 进展登记_v2_, 解码进展_v2);
        if (!f || !历史成员匹配_已锁定(身份, 进展登记_v2_.当前索引根,
                进展登记_v2_.当前成员关系, G1, Gread)
            || 历史唯一目标_已锁定(身份, 进展登记_v2_.服务合同关系, G1, Gread)
                != std::optional<稳定编码>{f->服务合同.值}
            || 历史唯一目标_已锁定(身份, 进展登记_v2_.自我关系, G1, Gread)
                != std::optional<稳定编码>{f->自我.值}
            || 历史唯一目标_已锁定(身份, 进展登记_v2_.需求关系, G1, Gread)
                != std::optional<稳定编码>{f->需求.值}
            || 历史唯一目标_已锁定(身份, 进展登记_v2_.任务关系, G1, Gread)
                != std::optional<稳定编码>{f->任务.值}
            || 历史唯一目标_已锁定(身份, 进展登记_v2_.方法关系, G1, Gread)
                != std::optional<稳定编码>{f->方法.值}
            || 历史唯一目标_已锁定(身份, 进展登记_v2_.正式选择关系, G1, Gread)
                != std::optional<稳定编码>{f->执行绑定.正式选择.值}
            || 历史唯一目标_已锁定(身份, 进展登记_v2_.冻结材料关系, G1, Gread)
                != std::optional<稳定编码>{f->执行绑定.执行冻结材料.值}
            || 历史唯一目标_已锁定(身份, 进展登记_v2_.实例方法关系, G1, Gread)
                != std::optional<稳定编码>{f->执行绑定.实例方法.值}
            || !历史可选目标匹配_已锁定(身份, 进展登记_v2_.状态引用关系,
                f->进展状态 ? std::optional<稳定编码>{f->进展状态->值}
                              : std::nullopt, G1, Gread)
            || !历史可选目标匹配_已锁定(身份, 进展登记_v2_.动态引用关系,
                f->进展动态 ? std::optional<稳定编码>{f->进展动态->值}
                              : std::nullopt, G1, Gread)) return std::nullopt;
        return f;
    }

    std::optional<服务准备当前事实_v2> 读取准备首次历史闭包_已锁定(
        稳定编码 身份, std::uint64_t G1, std::uint64_t Gread) const {
        using namespace 服务合同事实权威内部;
        auto f = 读取首次历史载荷_已锁定<服务准备当前事实_v2>(
            身份, G1, Gread, 准备登记_v2_, 解码准备_v2);
        if (!f) return std::nullopt;
        const auto 来源 = std::holds_alternative<L2需求身份>(f->来源)
            ? std::get<L2需求身份>(f->来源).值
            : std::get<服务能力缺口身份_v1>(f->来源).值;
        if (!历史成员匹配_已锁定(身份, 准备登记_v2_.当前索引根,
                准备登记_v2_.当前成员关系, G1, Gread)
            || 历史唯一目标_已锁定(身份, 准备登记_v2_.来源关系, G1, Gread)
                != std::optional<稳定编码>{来源}
            || 历史唯一目标_已锁定(身份, 准备登记_v2_.自我关系, G1, Gread)
                != std::optional<稳定编码>{f->自我.值}
            || 历史唯一目标_已锁定(身份, 准备登记_v2_.任务关系, G1, Gread)
                != std::optional<稳定编码>{f->任务.值}
            || 历史唯一目标_已锁定(身份, 准备登记_v2_.方法关系, G1, Gread)
                != std::optional<稳定编码>{f->方法.值}
            || 历史唯一目标_已锁定(身份, 准备登记_v2_.正式选择关系, G1, Gread)
                != std::optional<稳定编码>{f->执行绑定.正式选择.值}
            || 历史唯一目标_已锁定(身份, 准备登记_v2_.冻结材料关系, G1, Gread)
                != std::optional<稳定编码>{f->执行绑定.执行冻结材料.值}
            || 历史唯一目标_已锁定(身份, 准备登记_v2_.实例方法关系, G1, Gread)
                != std::optional<稳定编码>{f->执行绑定.实例方法.值}
            || !历史可选目标匹配_已锁定(身份, 准备登记_v2_.状态引用关系,
                f->进展状态 ? std::optional<稳定编码>{f->进展状态->值}
                              : std::nullopt, G1, Gread)
            || !历史可选目标匹配_已锁定(身份, 准备登记_v2_.动态引用关系,
                f->进展动态 ? std::optional<稳定编码>{f->进展动态->值}
                              : std::nullopt, G1, Gread)
            || !历史可选目标匹配_已锁定(身份, 准备登记_v2_.结果引用关系,
                f->准备结果 ? std::optional<稳定编码>{f->准备结果->值}
                              : std::nullopt, G1, Gread)
            || !历史可选目标匹配_已锁定(身份, 准备登记_v2_.验证引用关系,
                f->完成验证 ? std::optional<稳定编码>{f->完成验证->值}
                              : std::nullopt, G1, Gread)) return std::nullopt;
        return f;
    }

    std::optional<L1所有者范围关系事实> 读取状态当前关系_已锁定(
        稳定编码 状态身份, std::uint64_t G1,
        std::uint64_t Gread) const {
        const auto r = L1_.读取所有者范围历史关系组({
            L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::目标,
            状态身份, 登记_.合同当前状态关系, G1});
        if (r.状态 != L1所有者范围读取状态::成功
            || r.读取事实代次 != Gread || r.关系组.size() != 1)
            return std::nullopt;
        const auto& x = r.关系组.front();
        if (x.目标节点 != 状态身份
            || x.关系类型节点 != 登记_.合同当前状态关系
            || x.写入所有者 != 写入端口_.所有者身份()
            || x.角色或顺序 != 1 || x.创建事实代次 != G1
            || (x.退出事实代次 && *x.退出事实代次 <= G1))
            return std::nullopt;
        return x;
    }

    bool 合同历史引用闭合_已锁定(
        const 服务合同事实_v1& f, std::uint64_t Gread) const {
        const auto G1 = f.形成事实代次;
        return 历史唯一目标_已锁定(f.身份.值, 登记_.合同自我关系, G1, Gread)
                == std::optional<稳定编码>{f.自我.值}
            && 历史唯一目标_已锁定(f.身份.值, 登记_.合同提出者关系, G1, Gread)
                == std::optional<稳定编码>{f.提出者.值}
            && 历史唯一目标_已锁定(f.身份.值, 登记_.合同需求关系, G1, Gread)
                == std::optional<稳定编码>{f.需求.值}
            && 历史唯一目标_已锁定(f.身份.值, 登记_.合同目标宿主关系, G1, Gread)
                == std::optional<稳定编码>{f.目标宿主.值}
            && 历史唯一目标_已锁定(f.身份.值, 登记_.合同目标特征关系, G1, Gread)
                == std::optional<稳定编码>{f.目标特征.值}
            && 历史唯一目标_已锁定(f.身份.值,
                登记_.合同目标状态合同关系, G1, Gread)
                == std::optional<稳定编码>{f.目标状态合同.值};
    }

    bool 到期事件历史引用闭合_已锁定(
        const 服务到期未满足事件事实_v1& f,
        std::uint64_t Gread) const {
        const auto G1 = f.形成事实代次;
        return 历史唯一目标_已锁定(f.身份.值,
                登记_.到期事件合同关系, G1, Gread)
                == std::optional<稳定编码>{f.合同.值}
            && 历史唯一目标_已锁定(f.身份.值,
                登记_.到期事件未满足事实关系, G1, Gread)
                == std::optional<稳定编码>{f.目标未满足事实};
    }

    static std::optional<std::uint64_t> 投影退出当前边界(
        const L1所有者范围关系事实& 当前关系,
        std::uint64_t G0) noexcept {
        if (!当前关系.退出事实代次) return std::nullopt;
        if (*当前关系.退出事实代次 <= 当前关系.创建事实代次
            || *当前关系.退出事实代次 > G0)
            return std::nullopt;
        return 当前关系.退出事实代次;
    }

    void 填充共同读取请求(
        L1所有者范围一致关系类型闭包读取请求& 请求,
        std::uint64_t G0) const {
        请求.期望事实代次 = G0;
        请求.所有者 = {写入端口_.所有者身份()};
        请求.节点 = 登记_.全部节点();
        请求.属性值 = {{登记_.登记锚点, 登记_.登记版本载荷}};
    }

    L1所有者范围一致关系类型闭包读取请求
    形成合同读取请求(std::uint64_t G0) const {
        L1所有者范围一致关系类型闭包读取请求 请求;
        填充共同读取请求(请求, G0);
        L1所有者范围一致关系类型闭包选择项 成员;
        成员.入口关系类型节点 = 登记_.当前合同成员关系;
        成员.目标节点属性类型 = {登记_.合同载荷};
        成员.目标节点源关系类型 = {登记_.合同自我关系,
            登记_.合同提出者关系, 登记_.合同需求关系,
            登记_.合同目标宿主关系, 登记_.合同目标特征关系,
            登记_.合同目标状态合同关系};
        L1所有者范围一致关系类型闭包选择项 状态;
        状态.入口关系类型节点 = 登记_.合同当前状态关系;
        状态.源节点属性类型 = {登记_.合同载荷};
        状态.目标节点属性类型 = {登记_.合同状态载荷};
        请求.关系类型闭包 = {std::move(成员), std::move(状态)};
        return 请求;
    }

    L1所有者范围一致关系类型闭包读取请求
    形成事件读取请求(std::uint64_t G0) const {
        L1所有者范围一致关系类型闭包读取请求 请求;
        填充共同读取请求(请求, G0);
        L1所有者范围一致关系类型闭包选择项 成员;
        成员.入口关系类型节点 = 登记_.到期事件成员关系;
        成员.目标节点属性类型 = {登记_.到期事件载荷};
        成员.目标节点源关系类型 = {登记_.到期事件合同关系,
            登记_.到期事件未满足事实关系};
        L1所有者范围一致关系类型闭包选择项 合同;
        合同.入口关系类型节点 = 登记_.到期事件合同关系;
        合同.源节点属性类型 = {登记_.到期事件载荷};
        合同.目标节点属性类型 = {登记_.合同载荷};
        请求.关系类型闭包 = {std::move(成员), std::move(合同)};
        return 请求;
    }

    L1所有者范围一致关系类型闭包读取请求
    形成事件合同终态读取请求(std::uint64_t G0) const {
        auto 请求 = 形成事件读取请求(G0);
        auto& 合同 = 请求.关系类型闭包[1];
        合同.目标节点源关系类型 = {登记_.合同自我关系,
            登记_.合同提出者关系, 登记_.合同需求关系,
            登记_.合同目标宿主关系, 登记_.合同目标特征关系,
            登记_.合同目标状态合同关系};
        L1所有者范围一致关系类型闭包选择项 状态;
        状态.入口关系类型节点 = 登记_.合同当前状态关系;
        状态.源节点属性类型 = {登记_.合同载荷};
        状态.目标节点属性类型 = {登记_.合同状态载荷};
        请求.关系类型闭包.push_back(std::move(状态));
        return 请求;
    }

    L1所有者范围一致关系类型闭包读取请求
    形成进展读取请求(std::uint64_t G0) const {
        L1所有者范围一致关系类型闭包读取请求 请求;
        填充共同读取请求(请求, G0);
        const auto 进展节点 = 进展登记_.全部节点();
        请求.节点.insert(请求.节点.end(), 进展节点.begin(), 进展节点.end());
        请求.属性值.push_back(
            {进展登记_.登记锚点, 进展登记_.登记版本载荷});
        L1所有者范围一致关系类型闭包选择项 成员;
        成员.入口关系类型节点 = 进展登记_.当前进展成员关系;
        成员.目标节点属性类型 = {进展登记_.进展载荷};
        成员.目标节点源关系类型 = {进展登记_.进展服务合同关系,
            进展登记_.进展自我关系, 进展登记_.进展需求关系,
            进展登记_.进展任务关系, 进展登记_.进展方法关系,
            进展登记_.进展状态引用关系, 进展登记_.进展动态引用关系};
        L1所有者范围一致关系类型闭包选择项 合同;
        合同.入口关系类型节点 = 进展登记_.进展服务合同关系;
        合同.源节点属性类型 = {进展登记_.进展载荷};
        合同.目标节点属性类型 = {登记_.合同载荷};
        请求.关系类型闭包 = {std::move(成员), std::move(合同)};
        return 请求;
    }

    L1所有者范围一致关系类型闭包读取请求
    形成准备读取请求(std::uint64_t G0) const {
        L1所有者范围一致关系类型闭包读取请求 请求;
        填充共同读取请求(请求, G0);
        const auto 进展节点 = 进展登记_.全部节点();
        请求.节点.insert(
            请求.节点.end(), 进展节点.begin(), 进展节点.end());
        请求.属性值.push_back(
            {进展登记_.登记锚点, 进展登记_.登记版本载荷});
        const auto 准备节点 = 准备登记_.全部节点();
        请求.节点.insert(
            请求.节点.end(), 准备节点.begin(), 准备节点.end());
        请求.属性值.push_back({准备登记_.登记锚点, 准备登记_.登记版本载荷});
        L1所有者范围一致关系类型闭包选择项 成员;
        成员.入口关系类型节点 = 准备登记_.当前准备成员关系;
        成员.目标节点属性类型 = {准备登记_.准备载荷};
        成员.目标节点源关系类型 = {准备登记_.准备自我关系,
            准备登记_.准备来源需求关系, 准备登记_.准备来源能力缺口关系,
            准备登记_.准备任务关系, 准备登记_.准备方法关系,
            准备登记_.准备状态引用关系, 准备登记_.准备动态引用关系,
            准备登记_.准备结果引用关系, 准备登记_.准备验证引用关系};
        请求.关系类型闭包 = {std::move(成员)};
        return 请求;
    }

    L1所有者范围一致关系类型闭包读取请求
    形成进展读取请求_v2(std::uint64_t G0) const {
        L1所有者范围一致关系类型闭包读取请求 r;
        r.期望事实代次 = G0; r.所有者 = {写入端口_.所有者身份()};
        r.节点 = 进展登记_v2_.全部节点();
        r.属性值 = {{进展登记_v2_.登记锚点, 进展登记_v2_.登记版本载荷}};
        L1所有者范围一致关系类型闭包选择项 m;
        m.入口关系类型节点 = 进展登记_v2_.当前成员关系;
        m.目标节点属性类型 = {进展登记_v2_.载荷};
        m.目标节点源关系类型 = {进展登记_v2_.服务合同关系,
            进展登记_v2_.自我关系, 进展登记_v2_.需求关系,
            进展登记_v2_.任务关系, 进展登记_v2_.方法关系,
            进展登记_v2_.正式选择关系, 进展登记_v2_.冻结材料关系,
            进展登记_v2_.实例方法关系, 进展登记_v2_.状态引用关系,
            进展登记_v2_.动态引用关系};
        r.关系类型闭包 = {std::move(m)};
        return r;
    }

    L1所有者范围一致关系类型闭包读取请求
    形成准备读取请求_v2(std::uint64_t G0) const {
        L1所有者范围一致关系类型闭包读取请求 r;
        r.期望事实代次 = G0; r.所有者 = {写入端口_.所有者身份()};
        r.节点 = 准备登记_v2_.全部节点();
        r.属性值 = {{准备登记_v2_.登记锚点, 准备登记_v2_.登记版本载荷}};
        L1所有者范围一致关系类型闭包选择项 m;
        m.入口关系类型节点 = 准备登记_v2_.当前成员关系;
        m.目标节点属性类型 = {准备登记_v2_.载荷};
        m.目标节点源关系类型 = {准备登记_v2_.来源关系,
            准备登记_v2_.自我关系, 准备登记_v2_.任务关系,
            准备登记_v2_.方法关系, 准备登记_v2_.正式选择关系,
            准备登记_v2_.冻结材料关系, 准备登记_v2_.实例方法关系,
            准备登记_v2_.状态引用关系, 准备登记_v2_.动态引用关系,
            准备登记_v2_.结果引用关系, 准备登记_v2_.验证引用关系};
        r.关系类型闭包 = {std::move(m)};
        return r;
    }

    L1所有者范围一致关系类型闭包读取请求
    形成历史账读取请求(std::uint64_t G0) const {
        L1所有者范围一致关系类型闭包读取请求 r;
        r.期望事实代次 = G0;
        r.所有者 = {写入端口_.所有者身份()};
        r.节点 = 历史账登记_.全部节点();
        r.属性值 = {{历史账登记_.登记锚点,
            历史账登记_.登记版本载荷}};
        const auto 加 = [&](稳定编码 入口, 稳定编码 载荷) {
            L1所有者范围一致关系类型闭包选择项 x;
            x.入口关系类型节点 = 入口;
            x.目标节点属性类型 = {载荷};
            r.关系类型闭包.push_back(std::move(x));
        };
        加(历史账登记_.合同历史成员关系, 登记_.合同载荷);
        加(历史账登记_.合同状态历史成员关系, 登记_.合同状态载荷);
        加(历史账登记_.到期事件历史成员关系, 登记_.到期事件载荷);
        加(历史账登记_.进展v2历史成员关系, 进展登记_v2_.载荷);
        加(历史账登记_.准备v2历史成员关系, 准备登记_v2_.载荷);
        加(进展登记_v2_.当前成员关系, 进展登记_v2_.载荷);
        加(准备登记_v2_.当前成员关系, 准备登记_v2_.载荷);
        return r;
    }

    L1所有者范围一致关系类型闭包读取请求
    形成历史覆盖账读取请求_v2(std::uint64_t G0) const {
        L1所有者范围一致关系类型闭包读取请求 r;
        r.期望事实代次 = G0;
        r.所有者 = {写入端口_.所有者身份()};
        r.节点 = 历史覆盖登记_v2_.全部节点();
        r.属性值 = {{历史覆盖登记_v2_.登记锚点,
            历史覆盖登记_v2_.登记版本载荷}};
        const auto 加 = [&](稳定编码 入口, 稳定编码 载荷) {
            L1所有者范围一致关系类型闭包选择项 x;
            x.入口关系类型节点 = 入口;
            x.目标节点属性类型 = {载荷};
            r.关系类型闭包.push_back(std::move(x));
        };
        加(历史覆盖登记_v2_.合同历史成员关系, 登记_.合同载荷);
        加(历史覆盖登记_v2_.合同状态历史成员关系, 登记_.合同状态载荷);
        加(历史覆盖登记_v2_.到期事件历史成员关系, 登记_.到期事件载荷);
        加(历史覆盖登记_v2_.进展历史成员关系, 进展登记_v2_.载荷);
        加(历史覆盖登记_v2_.准备历史成员关系, 准备登记_v2_.载荷);
        return r;
    }

    template<class 登记类型>
    bool 验证v2共同登记(
        const L1所有者范围一致关系类型闭包读取结果& 读,
        const 登记类型& d, std::uint32_t 版本, std::uint64_t G0) const noexcept {
        const auto 节点组 = d.全部节点();
        if (读.合同版本 != L1所有者范围一致关系类型闭包读取合同版本
            || 读.所有者.size() != 1 || 读.节点.size() != 节点组.size())
            return false;
        const auto& o = 读.所有者.front();
        if (o.查询所有者 != 写入端口_.所有者身份()
            || o.状态 != L1所有者范围一致当前读取项目状态::成功
            || !o.所有者事实 || o.所有者事实->退出事实代次)
            return false;
        for (const auto& n : 读.节点)
            if (n.状态 != L1所有者范围一致当前读取项目状态::成功
                || !n.事实 || n.事实->写入所有者 != 写入端口_.所有者身份()
                || !服务合同事实权威内部::活动于(*n.事实, G0)) return false;
        const auto* p = 服务合同事实权威内部::查找属性投影(
            读.属性值, d.登记锚点, d.登记版本载荷);
        if (!p || p->当前值事实.写入所有者 != 写入端口_.所有者身份()
            || !服务合同事实权威内部::活动于(p->当前值事实, G0))
            return false;
        const auto* v = p ? 服务合同事实权威内部::U64载荷(p->当前值事实) : nullptr;
        return v && *v == std::vector<std::uint64_t>{版本};
    }

    bool 验证共同登记(
        const L1所有者范围一致关系类型闭包读取结果& 读取,
        std::uint64_t G0) const noexcept {
        if (读取.合同版本 != L1所有者范围一致关系类型闭包读取合同版本
            || 读取.所有者.size() != 1 || 读取.节点.size() != 登记_.全部节点().size())
            return false;
        const auto& 所有者 = 读取.所有者.front();
        if (所有者.查询所有者 != 写入端口_.所有者身份()
            || 所有者.状态 != L1所有者范围一致当前读取项目状态::成功
            || !所有者.所有者事实 || 所有者.所有者事实->退出事实代次)
            return false;
        for (const auto& 节点 : 读取.节点) {
            if (节点.状态 != L1所有者范围一致当前读取项目状态::成功
                || !节点.事实 || 节点.事实->写入所有者 != 写入端口_.所有者身份()
                || !服务合同事实权威内部::活动于(*节点.事实, G0))
                return false;
        }
        const auto* 版本 = 服务合同事实权威内部::查找属性投影(
            读取.属性值, 登记_.登记锚点, 登记_.登记版本载荷);
        if (!版本) return false;
        const auto* 载荷 = 服务合同事实权威内部::U64载荷(版本->当前值事实);
        return 载荷 && *载荷
            == std::vector<std::uint64_t>{服务合同事实权威结构登记版本_v1};
    }

    bool 验证进展共同登记(
        const L1所有者范围一致关系类型闭包读取结果& 读取,
        std::uint64_t G0) const noexcept {
        const auto 基础节点数 = 登记_.全部节点().size();
        const auto 进展节点数 = 进展登记_.全部节点().size();
        if (读取.合同版本 != L1所有者范围一致关系类型闭包读取合同版本
            || 读取.所有者.size() != 1
            || 读取.节点.size() != 基础节点数 + 进展节点数)
            return false;
        const auto& 所有者 = 读取.所有者.front();
        if (所有者.查询所有者 != 写入端口_.所有者身份()
            || 所有者.状态 != L1所有者范围一致当前读取项目状态::成功
            || !所有者.所有者事实 || 所有者.所有者事实->退出事实代次)
            return false;
        for (const auto& 节点 : 读取.节点) {
            if (节点.状态 != L1所有者范围一致当前读取项目状态::成功
                || !节点.事实 || 节点.事实->写入所有者 != 写入端口_.所有者身份()
                || !服务合同事实权威内部::活动于(*节点.事实, G0))
                return false;
        }
        const auto* 基础版本 = 服务合同事实权威内部::查找属性投影(
            读取.属性值, 登记_.登记锚点, 登记_.登记版本载荷);
        const auto* 进展版本 = 服务合同事实权威内部::查找属性投影(
            读取.属性值, 进展登记_.登记锚点, 进展登记_.登记版本载荷);
        if (!基础版本 || !进展版本) return false;
        const auto* 基础载荷 = 服务合同事实权威内部::U64载荷(
            基础版本->当前值事实);
        const auto* 进展载荷 = 服务合同事实权威内部::U64载荷(
            进展版本->当前值事实);
        return 基础载荷 && 进展载荷
            && *基础载荷
                == std::vector<std::uint64_t>{服务合同事实权威结构登记版本_v1}
            && *进展载荷
                == std::vector<std::uint64_t>{服务进展事实结构登记版本_v1};
    }

    bool 验证准备共同登记(
        const L1所有者范围一致关系类型闭包读取结果& 读,
        std::uint64_t G0) const noexcept {
        const auto 预期 = 登记_.全部节点().size()
            + 进展登记_.全部节点().size() + 准备登记_.全部节点().size();
        if (读.合同版本 != L1所有者范围一致关系类型闭包读取合同版本
            || 读.所有者.size() != 1 || 读.节点.size() != 预期) return false;
        const auto& 所有者 = 读.所有者.front();
        if (所有者.查询所有者 != 写入端口_.所有者身份()
            || 所有者.状态 != L1所有者范围一致当前读取项目状态::成功
            || !所有者.所有者事实 || 所有者.所有者事实->退出事实代次)
            return false;
        for (const auto& 节点 : 读.节点) if (节点.状态
            != L1所有者范围一致当前读取项目状态::成功 || !节点.事实
            || 节点.事实->写入所有者 != 写入端口_.所有者身份()
            || !服务合同事实权威内部::活动于(*节点.事实, G0)) return false;
        const auto* 基础 = 服务合同事实权威内部::查找属性投影(
            读.属性值, 登记_.登记锚点, 登记_.登记版本载荷);
        const auto* 进展 = 服务合同事实权威内部::查找属性投影(
            读.属性值, 进展登记_.登记锚点, 进展登记_.登记版本载荷);
        const auto* 准备 = 服务合同事实权威内部::查找属性投影(
            读.属性值, 准备登记_.登记锚点, 准备登记_.登记版本载荷);
        const auto* b = 基础 ? 服务合同事实权威内部::U64载荷(基础->当前值事实) : nullptr;
        const auto* g = 进展 ? 服务合同事实权威内部::U64载荷(进展->当前值事实) : nullptr;
        const auto* p = 准备 ? 服务合同事实权威内部::U64载荷(准备->当前值事实) : nullptr;
        return b && g && p
            && *b == std::vector<std::uint64_t>{服务合同事实权威结构登记版本_v1}
            && *g == std::vector<std::uint64_t>{服务进展事实结构登记版本_v1}
            && *p == std::vector<std::uint64_t>{服务准备事实结构登记版本_v1};
    }

    static const L1所有者范围一致关系类型闭包读取结果项* 查找闭包(
        const L1所有者范围一致关系类型闭包读取结果& 读取,
        稳定编码 关系类型) noexcept {
        const L1所有者范围一致关系类型闭包读取结果项* 结果 = nullptr;
        for (const auto& 项 : 读取.关系类型闭包) {
            if (项.入口关系类型节点 != 关系类型) continue;
            if (结果 || 项.状态 != L1所有者范围一致当前读取项目状态::成功
                || !项.关系类型事实)
                return nullptr;
            结果 = &项;
        }
        return 结果;
    }

    bool 合同引用闭合(
        const L1所有者范围一致关系类型闭包成员& 成员,
        const 服务合同事实_v1& 合同, std::uint64_t G0) const noexcept {
        using namespace 服务合同事实权威内部;
        return 唯一关系目标(成员.目标节点源关系组,
                   登记_.合同自我关系, 成员.目标节点.编码, G0)
                == std::optional<稳定编码>{合同.自我.值}
            && 唯一关系目标(成员.目标节点源关系组,
                   登记_.合同提出者关系, 成员.目标节点.编码, G0)
                == std::optional<稳定编码>{合同.提出者.值}
            && 唯一关系目标(成员.目标节点源关系组,
                   登记_.合同需求关系, 成员.目标节点.编码, G0)
                == std::optional<稳定编码>{合同.需求.值}
            && 唯一关系目标(成员.目标节点源关系组,
                   登记_.合同目标宿主关系, 成员.目标节点.编码, G0)
                == std::optional<稳定编码>{合同.目标宿主.值}
            && 唯一关系目标(成员.目标节点源关系组,
                   登记_.合同目标特征关系, 成员.目标节点.编码, G0)
                == std::optional<稳定编码>{合同.目标特征.值}
            && 唯一关系目标(成员.目标节点源关系组,
                   登记_.合同目标状态合同关系, 成员.目标节点.编码, G0)
                == std::optional<稳定编码>{合同.目标状态合同.值};
    }

    bool 事件引用闭合(
        const L1所有者范围一致关系类型闭包成员& 成员,
        const 服务到期未满足事件事实_v1& 事件,
        std::uint64_t G0) const noexcept {
        using namespace 服务合同事实权威内部;
        return 唯一关系目标(成员.目标节点源关系组,
                   登记_.到期事件合同关系, 成员.目标节点.编码, G0)
                == std::optional<稳定编码>{事件.合同.值}
            && 唯一关系目标(成员.目标节点源关系组,
                   登记_.到期事件未满足事实关系,
                   成员.目标节点.编码, G0)
                == std::optional<稳定编码>{事件.目标未满足事实};
    }

    static bool 可选关系目标闭合(
        const std::vector<L1所有者范围一致闭包端点关系组读取结果项>& 组,
        稳定编码 关系类型, 稳定编码 源节点,
        const std::optional<稳定编码>& 期望, std::uint64_t G0) noexcept {
        using namespace 服务合同事实权威内部;
        const auto* 命中 = 查找关系组(组, 关系类型);
        if (!命中 || 命中->状态 != L1所有者范围一致当前读取项目状态::成功
            || !命中->关系类型事实)
            return false;
        if (!期望) return 命中->成员.empty();
        if (命中->成员.size() != 1) return false;
        const auto& 成员 = 命中->成员.front();
        return 成员.关系.源节点 == 源节点
            && 成员.关系.关系类型节点 == 关系类型
            && 成员.关系.目标节点 == 成员.对端节点.编码
            && 成员.对端节点.编码 == *期望
            && 活动于(成员.关系, G0) && 活动于(成员.对端节点, G0);
    }

    bool 进展引用闭合(
        const L1所有者范围一致关系类型闭包成员& 成员,
        const 服务合同关联进展事实_v1& 进展,
        std::uint64_t G0) const noexcept {
        using namespace 服务合同事实权威内部;
        const auto 状态 = 进展.进展状态
            ? std::optional<稳定编码>{进展.进展状态->值} : std::nullopt;
        const auto 动态 = 进展.进展动态
            ? std::optional<稳定编码>{进展.进展动态->值} : std::nullopt;
        return 唯一关系目标(成员.目标节点源关系组,
                   进展登记_.进展服务合同关系, 成员.目标节点.编码, G0)
                == std::optional<稳定编码>{进展.服务合同.值}
            && 唯一关系目标(成员.目标节点源关系组,
                   进展登记_.进展自我关系, 成员.目标节点.编码, G0)
                == std::optional<稳定编码>{进展.自我.值}
            && 唯一关系目标(成员.目标节点源关系组,
                   进展登记_.进展需求关系, 成员.目标节点.编码, G0)
                == std::optional<稳定编码>{进展.需求.值}
            && 唯一关系目标(成员.目标节点源关系组,
                   进展登记_.进展任务关系, 成员.目标节点.编码, G0)
                == std::optional<稳定编码>{进展.任务.值}
            && 唯一关系目标(成员.目标节点源关系组,
                   进展登记_.进展方法关系, 成员.目标节点.编码, G0)
                == std::optional<稳定编码>{进展.方法.值}
            && 可选关系目标闭合(成员.目标节点源关系组,
                进展登记_.进展状态引用关系, 成员.目标节点.编码,
                状态, G0)
            && 可选关系目标闭合(成员.目标节点源关系组,
                进展登记_.进展动态引用关系, 成员.目标节点.编码,
                动态, G0);
    }

    static void 规范排序(std::vector<服务合同事实_v1>& 合同组,
        std::vector<服务合同状态事实_v1>& 状态组) {
        std::vector<std::size_t> 顺序(合同组.size());
        for (std::size_t i = 0; i < 顺序.size(); ++i) 顺序[i] = i;
        std::sort(顺序.begin(), 顺序.end(), [&](std::size_t 左, std::size_t 右) {
            return 合同组[左].身份.值.值 < 合同组[右].身份.值.值;
        });
        auto 旧合同 = std::move(合同组);
        auto 旧状态 = std::move(状态组);
        合同组.clear(); 状态组.clear();
        合同组.reserve(顺序.size()); 状态组.reserve(顺序.size());
        for (const auto i : 顺序) {
            合同组.push_back(std::move(旧合同[i]));
            状态组.push_back(std::move(旧状态[i]));
        }
    }

#if defined(ARCH_INSTINCT_SERVICE_CONTRACT_FACT_AUTHORITY_VALIDATION)
    L1所有者范围写集请求 形成v2验证节点写集(std::uint64_t G0,
        std::uint64_t 幂等身份, std::size_t 进展数, std::size_t 准备数) const {
        using namespace 服务合同事实权威内部;
        L1所有者范围写集请求 w; w.期望事实代次 = G0;
        w.写入幂等身份 = {幂等身份};
        w.节点.push_back({键(1), 节点种类::普通, std::nullopt});
        for (std::size_t i = 0; i < 进展数; ++i) {
            const auto b = static_cast<std::uint32_t>(100 + i * 20);
            for (std::uint32_t j = 0; j < 10; ++j)
                w.节点.push_back({键(b + j), 节点种类::普通, std::nullopt});
        }
        for (std::size_t i = 0; i < 准备数; ++i) {
            const auto b = static_cast<std::uint32_t>(1000 + i * 20);
            for (std::uint32_t j = 0; j < 11; ++j)
                w.节点.push_back({键(b + j), 节点种类::普通, std::nullopt});
        }
        return w;
    }

    L1所有者范围写集请求 形成v2验证事实写集(std::uint64_t G0,
        std::uint64_t 幂等身份, std::size_t 进展数, std::size_t 准备数,
        bool 损坏绑定, bool 损坏关系, bool 损坏载荷, bool 损坏集合,
        const std::vector<std::pair<L1所有者范围写集本地键, 稳定编码>>& m) const {
        using namespace 服务合同事实权威内部;
        L1所有者范围写集请求 w; w.期望事实代次 = G0;
        w.写入幂等身份 = {幂等身份};
        std::uint32_t next = 50000;
        const auto 取 = [&](std::uint32_t k) { const auto v = 查找映射(m, k);
            if (!v) throw std::runtime_error("service v2 validation mapping missing"); return *v; };
        const auto self = 取(1);
        const auto rel = [&](稳定编码 a, 稳定编码 b, 稳定编码 t,
            std::int64_t o = 1) {
            w.关系.push_back({键(next++), a, b, t, o});
        };
        for (std::size_t i = 0; i < 进展数; ++i) {
            const auto b = static_cast<std::uint32_t>(100 + i * 20);
            const auto node = 取(b), contract = 取(b + 1), demand = 取(b + 2);
            const auto task = 取(b + 3), method = 取(b + 4), selection = 取(b + 5);
            const auto freeze = 取(b + 6), instance = 取(b + 7);
            const auto state = 取(b + 8), dynamic = 取(b + 9);
            服务合同关联进展事实_v2 f;
            f.自我 = L2存在身份{self}; f.服务合同 = {contract};
            f.需求 = L2需求身份{demand}; f.任务 = L2任务身份{task};
            f.方法 = L2方法身份{method}; f.T到D关系稳定编码 = {0xFC00'0000ULL + i + 1};
            f.执行绑定 = {L2任务方法选择记录身份{selection},
                L2任务执行绑定冻结材料身份{freeze}, L2实例方法身份{instance},
                损坏绑定 && i == 0 ? 0ULL : i + 1, i + 1};
            if (i % 3 != 1) f.进展状态 = L2状态身份{state};
            if (i % 3 != 0) f.进展动态 = L2动态身份{dynamic};
            f.运行状态 = static_cast<服务进展运行状态_v1>(1 + i % 8);
            f.运行代次 = i + 1; f.计量窗口开始完整秒边界 = 10;
            f.进展发生完整秒边界 = 11; f.计量窗口结束完整秒边界 = 12;
            f.方法内容版本 = 1; f.方法规格版本 = 1;
            f.方法生命周期版本 = 1; f.进展规则版本 = 1;
            auto p = 编码进展_v2(f); if (损坏载荷 && i == 0) p.pop_back();
            const auto value = next++;
            w.值.push_back({键(value), node, 进展登记_v2_.载荷, std::move(p), node});
            w.属性槽变更.push_back({node, 进展登记_v2_.载荷, 键(value)});
            rel(进展登记_v2_.当前索引根, node, 进展登记_v2_.当前成员关系,
                static_cast<std::int64_t>(i + 1));
            if (损坏集合 && i == 0)
                rel(进展登记_v2_.当前索引根, node, 进展登记_v2_.当前成员关系,
                    static_cast<std::int64_t>(进展数 + 1));
            rel(node, contract, 进展登记_v2_.服务合同关系); rel(node, self, 进展登记_v2_.自我关系);
            rel(node, demand, 进展登记_v2_.需求关系); rel(node, task, 进展登记_v2_.任务关系);
            rel(node, method, 进展登记_v2_.方法关系);
            rel(node, 损坏关系 && i == 0 ? method : selection, 进展登记_v2_.正式选择关系);
            rel(node, freeze, 进展登记_v2_.冻结材料关系); rel(node, instance, 进展登记_v2_.实例方法关系);
            if (f.进展状态) rel(node, state, 进展登记_v2_.状态引用关系);
            if (f.进展动态) rel(node, dynamic, 进展登记_v2_.动态引用关系);
        }
        for (std::size_t i = 0; i < 准备数; ++i) {
            const auto b = static_cast<std::uint32_t>(1000 + i * 20);
            const auto node = 取(b), source = 取(b + 1), task = 取(b + 2);
            const auto method = 取(b + 3), selection = 取(b + 4), freeze = 取(b + 5);
            const auto instance = 取(b + 6), state = 取(b + 7), dynamic = 取(b + 8);
            const auto result = 取(b + 9), verify = 取(b + 10);
            const bool demandSource = i % 2 == 0;
            服务准备当前事实_v2 f; f.自我 = L2存在身份{self};
            f.来源 = demandSource ? 服务准备来源身份_v1{L2需求身份{source}}
                : 服务准备来源身份_v1{服务能力缺口身份_v1{source}};
            f.准备目标 = {{0xFD00'0000ULL + i + 1}};
            f.适用服务范围 = {0xFD10'0000ULL + i + 1};
            f.有效开始完整秒边界 = 10; f.有效结束完整秒边界 = 20;
            f.任务 = L2任务身份{task}; f.方法 = L2方法身份{method};
            if (demandSource) f.T到D关系稳定编码 = 稳定编码{0xFD20'0000ULL + i + 1};
            f.执行绑定 = {L2任务方法选择记录身份{selection},
                L2任务执行绑定冻结材料身份{freeze}, L2实例方法身份{instance},
                损坏绑定 && 进展数 == 0 && i == 0 ? 0ULL : i + 1, i + 1};
            if (i % 3 != 1) f.进展状态 = L2状态身份{state};
            if (i % 3 != 0) f.进展动态 = L2动态身份{dynamic};
            f.运行状态 = static_cast<服务准备运行状态_v1>(1 + i % 9);
            if (f.运行状态 == 服务准备运行状态_v1::已完整完成) {
                f.准备结果 = 服务准备结果身份_v1{result};
                f.完成验证 = 服务准备验证事实身份_v1{verify};
            }
            f.运行代次 = i + 1; f.计量窗口开始完整秒边界 = 10;
            f.进展发生完整秒边界 = 11; f.计量窗口结束完整秒边界 = 12;
            f.方法内容版本 = 1; f.方法规格版本 = 1;
            f.方法生命周期版本 = 1; f.准备规则版本 = 1;
            auto p = 编码准备_v2(f);
            if (损坏载荷 && 进展数 == 0 && i == 0) p.pop_back();
            const auto value = next++;
            w.值.push_back({键(value), node, 准备登记_v2_.载荷, std::move(p), node});
            w.属性槽变更.push_back({node, 准备登记_v2_.载荷, 键(value)});
            rel(准备登记_v2_.当前索引根, node, 准备登记_v2_.当前成员关系,
                static_cast<std::int64_t>(i + 1));
            if (损坏集合 && 进展数 == 0 && i == 0)
                rel(准备登记_v2_.当前索引根, node, 准备登记_v2_.当前成员关系,
                    static_cast<std::int64_t>(准备数 + 1));
            rel(node, source, 准备登记_v2_.来源关系); rel(node, self, 准备登记_v2_.自我关系);
            rel(node, task, 准备登记_v2_.任务关系); rel(node, method, 准备登记_v2_.方法关系);
            rel(node, 损坏关系 && 进展数 == 0 && i == 0 ? method : selection,
                准备登记_v2_.正式选择关系);
            rel(node, freeze, 准备登记_v2_.冻结材料关系); rel(node, instance, 准备登记_v2_.实例方法关系);
            if (f.进展状态) rel(node, state, 准备登记_v2_.状态引用关系);
            if (f.进展动态) rel(node, dynamic, 准备登记_v2_.动态引用关系);
            if (f.准备结果) rel(node, result, 准备登记_v2_.结果引用关系);
            if (f.完成验证) rel(node, verify, 准备登记_v2_.验证引用关系);
        }
        return w;
    }

    L1所有者范围写集请求 形成验证节点写集(std::uint64_t G0,
        std::uint64_t 幂等身份, std::size_t 合同数,
        std::size_t 事件数, std::size_t 进展数, std::size_t 准备数,
        const ARCH_服务需求时间裁决验证配置_v1& 时间配置) const {
        using namespace 服务合同事实权威内部;
        L1所有者范围写集请求 写集;
        写集.期望事实代次 = G0;
        写集.写入幂等身份 = {幂等身份};
        if (!时间配置.固定自我)
            写集.节点.push_back({键(1), 节点种类::普通, std::nullopt});
        for (std::size_t i = 0; i < 合同数; ++i) {
            const auto 基 = static_cast<std::uint32_t>(10 + i * 10);
            for (std::uint32_t 偏移 = 0; 偏移 < 7; ++偏移)
                写集.节点.push_back(
                    {键(基 + 偏移), 节点种类::普通, std::nullopt});
        }
        for (std::size_t i = 0; i < 事件数; ++i) {
            const auto 基 = static_cast<std::uint32_t>(1000 + i * 10);
            写集.节点.push_back({键(基), 节点种类::普通, std::nullopt});
            写集.节点.push_back({键(基 + 1), 节点种类::普通, std::nullopt});
        }
        for (std::size_t i = 0; i < 进展数; ++i) {
            const auto 基 = static_cast<std::uint32_t>(3000 + i * 10);
            for (std::uint32_t 偏移 = 0; 偏移 < 5; ++偏移)
                写集.节点.push_back(
                    {键(基 + 偏移), 节点种类::普通, std::nullopt});
        }
        for (std::size_t i = 0; i < 准备数; ++i) {
            const auto 基 = static_cast<std::uint32_t>(5000 + i * 10);
            for (std::uint32_t 偏移 = 0; 偏移 < 8; ++偏移)
                写集.节点.push_back(
                    {键(基 + 偏移), 节点种类::普通, std::nullopt});
        }
        return 写集;
    }

    L1所有者范围写集请求 形成验证事实写集(std::uint64_t G0,
        std::uint64_t 幂等身份, std::size_t 合同数, std::size_t 事件数,
        bool 终态干扰, bool 损坏载荷, std::size_t 进展数,
        bool 损坏进展载荷, std::size_t 准备数,
        bool 损坏准备载荷, bool 损坏准备来源, bool 损坏准备关系,
        bool 事件合同到期终态, bool 损坏事件终态,
        bool 损坏事件终态版本, bool 遗漏事件成员,
        bool 重复事件成员, bool 错误事件合同关系,
        bool 遗漏事件终态关系, bool 重复事件终态关系,
        const ARCH_服务需求时间裁决验证配置_v1& 时间配置,
        const std::vector<std::pair<L1所有者范围写集本地键, 稳定编码>>& 映射) const {
        using namespace 服务合同事实权威内部;
        L1所有者范围写集请求 写集;
        写集.期望事实代次 = G0;
        写集.写入幂等身份 = {幂等身份};
        std::uint32_t 下一个 = 2000;
        const auto 取 = [&](std::uint32_t 本地键) -> 稳定编码 {
            const auto 编码 = 查找映射(映射, 本地键);
            if (!编码) throw std::runtime_error("validation fixture mapping missing");
            return *编码;
        };
        const auto 自我 = 时间配置.固定自我
            ? 时间配置.固定自我->值 : 取(1);
        std::vector<稳定编码> 合同节点组;
        std::vector<服务合同事实_v1> 合同事实组;
        for (std::size_t i = 0; i < 合同数; ++i) {
            const auto 基 = static_cast<std::uint32_t>(10 + i * 10);
            const auto 合同节点 = 取(基);
            const auto 提出者 = 取(基 + 1);
            const auto 需求 = 取(基 + 2);
            const auto 目标宿主 = 取(基 + 3);
            const auto 目标特征 = 取(基 + 4);
            const auto 目标合同 = 取(基 + 5);
            const auto 状态节点 = 取(基 + 6);
            合同节点组.push_back(合同节点);
            服务合同事实_v1 合同;
            合同.自我 = L2存在身份{自我};
            合同.提出者 = L2存在身份{提出者};
            合同.需求 = 时间配置.重复首个需求 && i != 0
                ? 合同事实组.front().需求 : L2需求身份{需求};
            合同.来源提出事件 = {0xF1000000ULL + i + 1};
            合同.目标宿主 = L2存在身份{目标宿主};
            合同.目标特征 = L2特征定义身份{目标特征};
            合同.目标状态合同 = L2目标状态合同身份{目标合同};
            合同.服务范围 = {0xF2000000ULL + i + 1};
            合同.提出运行代次 = 1;
            合同.时间纪元身份 = {时间配置.共同时间纪元身份
                ? *时间配置.共同时间纪元身份
                : 0xF3000000ULL + i + 1};
            合同.提出完整秒边界 = 时间配置.提出起始完整秒边界
                + static_cast<std::int64_t>(i) * 时间配置.提出步长;
            合同.有效开始完整秒边界 = 合同.提出完整秒边界
                + 时间配置.有效开始偏移;
            合同.冻结有效总秒 = 100;
            合同.冻结预算 = 1000;
            合同.预支值 = 300;
            合同.完工余款 = 700;
            合同.需求版本 = 1;
            合同.法规准入见证 = {0xF4000000ULL + i + 1};
            合同.法规规则版本 = 1;
            合同.合同代次 = i + 1;
            合同.合同规则版本 = 时间配置.合同规则版本;
            合同事实组.push_back(合同);
            auto 合同载荷材料 = 编码合同(合同);
            if (损坏载荷 && i == 0) 合同载荷材料.pop_back();
            const auto 合同值 = 下一个++;
            写集.值.push_back({键(合同值), 合同节点, 登记_.合同载荷,
                std::move(合同载荷材料), 合同节点});
            写集.属性槽变更.push_back({合同节点, 登记_.合同载荷, 键(合同值)});
            服务合同状态事实_v1 状态;
            const bool 到期终态 = 事件合同到期终态 && i < 事件数;
            状态.状态 = 到期终态 ? 服务合同当前状态_v1::已到期
                : 终态干扰 && i == 0
                    ? 服务合同当前状态_v1::已完整完成
                    : 服务合同当前状态_v1::有效未满足;
            状态.已消费有效秒 = 到期终态
                ? (损坏事件终态 && i == 0 ? 99 : 100) : 0;
            状态.最后已结算完整秒边界 = 到期终态
                ? 时间配置.到期完整秒边界
                : 时间配置.最后已结算完整秒边界;
            状态.状态版本 = 到期终态
                ? (损坏事件终态版本 && i == 0 ? 3 : 2) : 1;
            状态.来源事实 = {0xF5000000ULL + i + 1};
            const auto 状态值 = 下一个++;
            写集.值.push_back({键(状态值), 状态节点, 登记_.合同状态载荷,
                编码状态(状态), 状态节点});
            写集.属性槽变更.push_back({状态节点, 登记_.合同状态载荷, 键(状态值)});
            const auto 关系 = [&](稳定编码 源, 稳定编码 目标,
                稳定编码 类型, std::int64_t 顺序 = 1) {
                写集.关系.push_back({键(下一个++), 源, 目标, 类型, 顺序});
            };
            关系(合同节点, 自我, 登记_.合同自我关系);
            关系(合同节点, 提出者, 登记_.合同提出者关系);
            关系(合同节点, 合同.需求.值, 登记_.合同需求关系);
            关系(合同节点, 目标宿主, 登记_.合同目标宿主关系);
            关系(合同节点, 目标特征, 登记_.合同目标特征关系);
            关系(合同节点, 目标合同, 登记_.合同目标状态合同关系);
            if (!(遗漏事件终态关系 && i == 0))
                关系(合同节点, 状态节点, 登记_.合同当前状态关系);
            if (重复事件终态关系 && i == 0)
                关系(合同节点, 状态节点, 登记_.合同当前状态关系, 2);
            if (!到期终态)
                关系(登记_.当前合同索引根, 合同节点,
                    登记_.当前合同成员关系, static_cast<std::int64_t>(i + 1));
        }
        for (std::size_t i = 0; i < 事件数; ++i) {
            const auto 基 = static_cast<std::uint32_t>(1000 + i * 10);
            const auto 事件节点 = 取(基);
            const auto 未满足事实 = 取(基 + 1);
            服务到期未满足事件事实_v1 事件;
            事件.合同 = {合同节点组[i]};
            事件.需求 = 合同事实组[i].需求;
            事件.提出者 = 合同事实组[i].提出者;
            事件.冻结有效总秒 = 100;
            事件.实际已消费有效秒 = 100;
            事件.到期完整秒边界 = 时间配置.到期完整秒边界;
            事件.目标未满足事实 = 未满足事实;
            事件.合同终态版本 = 2;
            事件.规则版本 = 时间配置.事件规则版本;
            const auto 事件值 = 下一个++;
            写集.值.push_back({键(事件值), 事件节点, 登记_.到期事件载荷,
                编码事件(事件), 事件节点});
            写集.属性槽变更.push_back({事件节点, 登记_.到期事件载荷, 键(事件值)});
            if (!(遗漏事件成员 && i == 0))
                写集.关系.push_back({键(下一个++), 登记_.到期事件索引根,
                    事件节点, 登记_.到期事件成员关系,
                    static_cast<std::int64_t>(i + 1)});
            if (重复事件成员 && i == 0)
                写集.关系.push_back({键(下一个++), 登记_.到期事件索引根,
                    事件节点, 登记_.到期事件成员关系,
                    static_cast<std::int64_t>(事件数 + 1)});
            写集.关系.push_back({键(下一个++), 事件节点,
                错误事件合同关系 && i == 0 ? 合同节点组[1] : 合同节点组[i],
                登记_.到期事件合同关系, 1});
            写集.关系.push_back({键(下一个++), 事件节点,
                未满足事实, 登记_.到期事件未满足事实关系, 1});
        }
        for (std::size_t i = 0; i < 进展数; ++i) {
            const auto 基 = static_cast<std::uint32_t>(3000 + i * 10);
            const auto 进展节点 = 取(基);
            const auto 任务 = 取(基 + 1);
            const auto 方法 = 取(基 + 2);
            const auto 状态 = 取(基 + 3);
            const auto 动态 = 取(基 + 4);
            服务合同关联进展事实_v1 进展;
            进展.自我 = L2存在身份{自我};
            进展.服务合同 = {合同节点组[i]};
            进展.需求 = 合同事实组[i].需求;
            进展.任务 = L2任务身份{任务};
            进展.方法 = L2方法身份{方法};
            进展.T到D关系稳定编码 = {0xFA00'0000ULL + i + 1};
            进展.执行冻结见证 = {{0xFA10'0000ULL + i + 1}};
            进展.方法执行见证 = {{0xFA20'0000ULL + i + 1}};
            进展.安全门禁见证 = {{0xFA30'0000ULL + i + 1}};
            if (i % 3 != 1) 进展.进展状态 = L2状态身份{状态};
            if (i % 3 != 0) 进展.进展动态 = L2动态身份{动态};
            进展.运行状态 = static_cast<服务进展运行状态_v1>(
                1 + (i % 8));
            进展.运行代次 = 1;
            进展.计量窗口开始完整秒边界 = 10;
            进展.进展发生完整秒边界 = 11;
            进展.计量窗口结束完整秒边界 = 12;
            进展.方法内容版本 = 1;
            进展.方法规格版本 = 1;
            进展.方法生命周期版本 = 1;
            进展.进展规则版本 = 1;
            auto 进展载荷材料 = 编码进展(进展);
            if (损坏进展载荷 && i == 0) 进展载荷材料.pop_back();
            const auto 进展值 = 下一个++;
            写集.值.push_back({键(进展值), 进展节点, 进展登记_.进展载荷,
                std::move(进展载荷材料), 进展节点});
            写集.属性槽变更.push_back(
                {进展节点, 进展登记_.进展载荷, 键(进展值)});
            const auto 关系 = [&](稳定编码 源, 稳定编码 目标,
                稳定编码 类型, std::int64_t 顺序 = 1) {
                写集.关系.push_back({键(下一个++), 源, 目标, 类型, 顺序});
            };
            关系(进展登记_.当前进展索引根, 进展节点,
                进展登记_.当前进展成员关系, static_cast<std::int64_t>(i + 1));
            关系(进展节点, 合同节点组[i], 进展登记_.进展服务合同关系);
            关系(进展节点, 自我, 进展登记_.进展自我关系);
            关系(进展节点, 合同事实组[i].需求.值, 进展登记_.进展需求关系);
            关系(进展节点, 任务, 进展登记_.进展任务关系);
            关系(进展节点, 方法, 进展登记_.进展方法关系);
            if (进展.进展状态)
                关系(进展节点, 状态, 进展登记_.进展状态引用关系);
            if (进展.进展动态)
                关系(进展节点, 动态, 进展登记_.进展动态引用关系);
        }
        for (std::size_t i = 0; i < 准备数; ++i) {
            const auto 基 = static_cast<std::uint32_t>(5000 + i * 10);
            const auto 准备节点 = 取(基);
            const auto 来源 = 取(基 + 1);
            const auto 任务 = 取(基 + 2);
            const auto 方法 = 取(基 + 3);
            const auto 状态 = 取(基 + 4);
            const auto 动态 = 取(基 + 5);
            const auto 结果 = 取(基 + 6);
            const auto 验证 = 取(基 + 7);
            const bool 需求来源 = i % 2 == 0;
            服务准备当前事实_v1 准备;
            准备.自我 = L2存在身份{自我};
            准备.来源 = 需求来源
                ? 服务准备来源身份_v1{L2需求身份{来源}}
                : 服务准备来源身份_v1{服务能力缺口身份_v1{来源}};
            准备.准备目标 = {{0xFB00'0000ULL + i + 1}};
            准备.适用服务范围 = {0xFB10'0000ULL + i + 1};
            准备.有效开始完整秒边界 = 10;
            准备.有效结束完整秒边界 = 20;
            准备.任务 = L2任务身份{任务};
            准备.方法 = L2方法身份{方法};
            if (需求来源)
                准备.T到D关系稳定编码 = 稳定编码{0xFB20'0000ULL + i + 1};
            准备.执行冻结见证 = {{0xFB30'0000ULL + i + 1}};
            准备.方法执行见证 = {{0xFB40'0000ULL + i + 1}};
            准备.安全门禁见证 = {{0xFB50'0000ULL + i + 1}};
            if (i % 3 != 1) 准备.进展状态 = L2状态身份{状态};
            if (i % 3 != 0) 准备.进展动态 = L2动态身份{动态};
            准备.运行状态 = static_cast<服务准备运行状态_v1>(1 + (i % 9));
            if (准备.运行状态 == 服务准备运行状态_v1::已完整完成) {
                准备.准备结果 = 服务准备结果身份_v1{结果};
                准备.完成验证 = 服务准备验证事实身份_v1{验证};
            }
            准备.运行代次 = i + 1;
            准备.进展发生完整秒边界 = 11;
            准备.计量窗口开始完整秒边界 = 10;
            准备.计量窗口结束完整秒边界 = 12;
            准备.方法内容版本 = 1;
            准备.方法规格版本 = 1;
            准备.方法生命周期版本 = 1;
            准备.准备规则版本 = 1;
            if (损坏准备来源 && i == 0)
                准备.T到D关系稳定编码.reset();
            auto 准备载荷材料 = 编码准备(准备);
            if (损坏准备载荷 && i == 0) 准备载荷材料.pop_back();
            const auto 准备值 = 下一个++;
            写集.值.push_back({键(准备值), 准备节点, 准备登记_.准备载荷,
                std::move(准备载荷材料), 准备节点});
            写集.属性槽变更.push_back(
                {准备节点, 准备登记_.准备载荷, 键(准备值)});
            const auto 关系 = [&](稳定编码 源, 稳定编码 目标,
                稳定编码 类型, std::int64_t 顺序 = 1) {
                写集.关系.push_back({键(下一个++), 源, 目标, 类型, 顺序});
            };
            关系(准备登记_.当前准备索引根, 准备节点,
                准备登记_.当前准备成员关系, static_cast<std::int64_t>(i + 1));
            关系(准备节点, 自我, 准备登记_.准备自我关系);
            关系(准备节点, 损坏准备关系 && i == 0 ? 方法 : 来源,
                需求来源 ? 准备登记_.准备来源需求关系
                           : 准备登记_.准备来源能力缺口关系);
            关系(准备节点, 任务, 准备登记_.准备任务关系);
            关系(准备节点, 方法, 准备登记_.准备方法关系);
            if (准备.进展状态)
                关系(准备节点, 状态, 准备登记_.准备状态引用关系);
            if (准备.进展动态)
                关系(准备节点, 动态, 准备登记_.准备动态引用关系);
            if (准备.准备结果)
                关系(准备节点, 结果, 准备登记_.准备结果引用关系);
            if (准备.完成验证)
                关系(准备节点, 验证, 准备登记_.准备验证引用关系);
        }
        return 写集;
    }

    void 注入漂移_已锁定() const {
        if (!ARCH_读中漂移一次_) return;
        ARCH_读中漂移一次_ = false;
        const auto 当前 = L1_.读取中性当前事实代次({L1中性CRUD合同版本});
        if (当前.状态 != L1中性读取状态::成功) return;
        L1所有者范围写集请求 写集;
        写集.期望事实代次 = 当前.事实代次;
        写集.写入幂等身份 = {0x494E'5354'5344'5246ULL + ARCH_漂移序号_++};
        const auto 本地键值 = static_cast<std::uint32_t>(
            0x70000000ULL + (ARCH_漂移序号_ & 0x0FFFFFFFULL));
        写集.节点 = {{服务合同事实权威内部::键(本地键值),
            节点种类::普通, std::nullopt}};
        (void)写入端口_.提交所有者范围中性写集(写集);
    }
#endif

    const L1事实基座服务& L1_;
    mutable L1所有者范围写端口 写入端口_;
    服务合同事实权威内部::结构登记定位 登记_;
    服务合同事实权威内部::进展结构登记定位 进展登记_;
    服务合同事实权威内部::准备结构登记定位 准备登记_;
    服务合同事实权威内部::进展结构登记定位_v2 进展登记_v2_;
    服务合同事实权威内部::准备结构登记定位_v2 准备登记_v2_;
    服务合同事实权威内部::服务维护历史账登记定位_v1 历史账登记_;
    服务合同事实权威内部::服务维护历史覆盖登记定位_v2 历史覆盖登记_v2_;
    mutable std::mutex 互斥体_;
#if defined(ARCH_INSTINCT_SERVICE_CONTRACT_FACT_AUTHORITY_VALIDATION)
    struct ARCH_v2验证样本记录 final {
        std::size_t 进展数 = 0;
        std::size_t 准备数 = 0;
        bool 损坏绑定 = false;
        bool 损坏关系 = false;
        bool 损坏载荷 = false;
        bool 损坏集合 = false;
        std::uint64_t 起始事实代次 = 0;
        std::uint64_t 节点事实代次 = 0;
        std::uint64_t 完成事实代次 = 0;
        std::vector<std::pair<L1所有者范围写集本地键, 稳定编码>> 映射;
        L2存在身份 自我{};
    };
    struct ARCH_验证样本记录 final {
        std::size_t 合同数 = 0;
        std::size_t 事件数 = 0;
        bool 终态干扰 = false;
        bool 损坏载荷 = false;
        std::size_t 进展数 = 0;
        bool 损坏进展载荷 = false;
        std::size_t 准备数 = 0;
        bool 损坏准备载荷 = false;
        bool 损坏准备来源 = false;
        bool 损坏准备关系 = false;
        bool 事件合同到期终态 = false;
        bool 损坏事件终态 = false;
        bool 损坏事件终态版本 = false;
        bool 遗漏事件成员 = false;
        bool 重复事件成员 = false;
        bool 错误事件合同关系 = false;
        bool 遗漏事件终态关系 = false;
        bool 重复事件终态关系 = false;
        ARCH_服务需求时间裁决验证配置_v1 时间配置{};
        std::uint64_t 起始事实代次 = 0;
        std::uint64_t 节点事实代次 = 0;
        std::uint64_t 完成事实代次 = 0;
        std::vector<std::pair<L1所有者范围写集本地键, 稳定编码>> 映射;
        L2存在身份 自我{};
    };
    std::map<std::uint64_t, ARCH_验证样本记录> ARCH_验证样本记录_;
    std::map<std::uint64_t, ARCH_v2验证样本记录> ARCH_v2验证样本记录_;
    mutable bool ARCH_读中漂移一次_ = false;
    mutable bool ARCH_资源失败一次_ = false;
    mutable bool ARCH_G0终态错配一次_ = false;
    bool ARCH_发布后读回失败一次_ = false;
    mutable std::uint64_t ARCH_漂移序号_ = 1;
    std::uint64_t ARCH_v2损坏序号_ = 1;
    std::uint64_t ARCH_历史损坏序号_ = 1;
    std::uint64_t ARCH_v2事实引用损坏序号_ = 1;
#endif
};

} // namespace 海中鱼巣
