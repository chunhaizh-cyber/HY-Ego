module;

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <map>
#include <mutex>
#include <new>
#include <optional>
#include <stdexcept>
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
              L1_, 写入端口_)) {}

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
                    || 合同.自我 != 请求.自我
                    || !合同引用闭合(成员, 合同, G0))
                    return 失败(服务合同事实权威读取状态_v1::引用冲突);
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

    服务进展完整集合读取结果_v2
    读取当前服务合同关联进展完整集合_v2(
        const 服务进展完整集合读取请求_v2& 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
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
                        p->当前值事实, 成员.目标节点.编码, f)
                    || f.自我 != 请求.自我)
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

    服务准备完整集合读取结果_v2 读取当前服务准备完整集合_v2(
        const 服务准备完整集合读取请求_v2& 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
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
                        p->当前值事实, 成员.目标节点.编码, f)
                    || f.自我 != 请求.自我)
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

#if defined(ARCH_INSTINCT_SERVICE_CONTRACT_FACT_AUTHORITY_VALIDATION)
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

    void ARCH_注入读中漂移一次() noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        ARCH_读中漂移一次_ = true;
    }

    void ARCH_注入资源失败一次() noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        ARCH_资源失败一次_ = true;
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

    bool 读前守卫(std::uint64_t G0) const noexcept { return 读后守卫(G0); }
    bool 读后守卫(std::uint64_t G0) const noexcept {
        const auto 当前 = L1_.读取中性当前事实代次({L1中性CRUD合同版本});
        return 当前.状态 == L1中性读取状态::成功 && 当前.事实代次 == G0;
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
    mutable std::uint64_t ARCH_漂移序号_ = 1;
    std::uint64_t ARCH_v2损坏序号_ = 1;
#endif
};

} // namespace 海中鱼巣
