module;

#include <algorithm>
#include <cstdint>
#include <mutex>
#include <new>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#define L2_CONCEPT_STRUCTURE_NO_INCLUDES
#define L2_ORDINARY_CONCEPT_STRUCTURE_NO_INCLUDES

export module 海中鱼巣.领域.服务.L2概念结构;

export import 海中鱼巣.领域.合同.L2结构公共;
import 海中鱼巣.核心.服务.L1事实基座;
import 海中鱼巣.领域.服务.不可变材料;
import 海中鱼巣.领域.服务.L2场景结构;
import 海中鱼巣.领域.服务.L2存在结构;
import 海中鱼巣.领域.服务.L2特征结构;
import 海中鱼巣.领域.服务.L2状态结构;
import 海中鱼巣.领域.服务.L2动态结构;
import 海中鱼巣.领域.服务.L2因果结构;

export {
#include "L2概念结构.数据.h"
#include "L2普通概念结构.数据.h"
}

export namespace 海中鱼巣 {

inline constexpr L1所有者范围建立幂等身份 概念所有者建立身份{
    0x4C32'434F'4E43'0001ULL};

class L2概念结构服务;

class L2概念所有者交付 final {
public:
    L2概念所有者交付() = delete;
    L2概念所有者交付(const L2概念所有者交付&) = delete;
    L2概念所有者交付& operator=(const L2概念所有者交付&) = delete;
    L2概念所有者交付(L2概念所有者交付&&) noexcept = default;
    L2概念所有者交付& operator=(L2概念所有者交付&&) = delete;

private:
    explicit L2概念所有者交付(L1所有者范围写端口&& 端口) noexcept
        : 写入端口_(std::move(端口)) {}

    friend std::optional<L2概念所有者交付> 尝试形成L2概念所有者交付(
        const L1事实基座服务&, L1所有者范围交付&&) noexcept;
    friend class L2概念结构服务;
    L1所有者范围写端口 写入端口_;
};

std::optional<L2概念所有者交付> 尝试形成L2概念所有者交付(
    const L1事实基座服务&, L1所有者范围交付&&) noexcept;

}

namespace 海中鱼巣::L2概念结构内部 {

inline constexpr L1所有者范围写入幂等身份 概念身份来源登记幂等身份{
    0x4C32'4944'5352'430AULL};
inline constexpr L1所有者范围写入幂等身份 概念本体结构登记幂等身份{
    0x4C32'434F'5247'0001ULL};
inline constexpr L1所有者范围写入幂等身份 普通概念结构登记幂等身份{
    0x4C32'434F'5247'0002ULL};
inline constexpr L1所有者范围写入幂等身份 概念世界事实支持结构登记幂等身份{
    0x4C32'434F'5247'0003ULL};

inline constexpr std::uint32_t 概念族锚点本地键值 = 0x020A'6001U;
inline constexpr std::uint32_t 概念族归属关系类型本地键值 = 0x020A'6002U;
inline constexpr std::uint32_t 本体根角色锚点本地键值 = 0x020A'6101U;
inline constexpr std::uint32_t 本体根角色关系类型本地键值 = 0x020A'6102U;
inline constexpr std::uint32_t 直接上位关系类型本地键值 = 0x020A'6103U;
inline constexpr std::uint32_t 首次请求见证属性类型本地键值 = 0x020A'6104U;

inline constexpr std::uint32_t 本体根归属关系类型本地键值 = 0x020A'6201U;
inline constexpr std::uint32_t 签名规则关系类型本地键值 = 0x020A'6202U;
inline constexpr std::uint32_t 签名值式项成员关系类型本地键值 = 0x020A'6203U;
inline constexpr std::uint32_t 定义项成员关系类型本地键值 = 0x020A'6204U;
inline constexpr std::uint32_t 来源证据关系类型本地键值 = 0x020A'6205U;
inline constexpr std::uint32_t 普通概念首次请求见证属性类型本地键值 = 0x020A'6211U;
inline constexpr std::uint32_t 项键属性类型本地键值 = 0x020A'6212U;
inline constexpr std::uint32_t 签名I64材料属性类型本地键值 = 0x020A'6213U;
inline constexpr std::uint32_t 签名I64组材料属性类型本地键值 = 0x020A'6214U;
inline constexpr std::uint32_t 签名U64组材料属性类型本地键值 = 0x020A'6215U;
inline constexpr std::uint32_t 签名独立材料引用属性类型本地键值 = 0x020A'6216U;
inline constexpr std::uint32_t 定义目标编码属性类型本地键值 = 0x020A'6217U;
inline constexpr std::uint32_t 概念治理生命周期属性类型本地键值 = 0x020A'6218U;

inline constexpr std::uint32_t 支持登记根节点本地键值 = 0x020A'6301U;
inline constexpr std::uint32_t 支持登记成员关系类型本地键值 = 0x020A'6302U;
inline constexpr std::uint32_t 世界事实支持关系类型本地键值 = 0x020A'6303U;
inline constexpr std::uint32_t 世界事实引用编码属性类型本地键值 = 0x020A'6311U;
inline constexpr std::uint32_t 支持建立依据材料属性类型本地键值 = 0x020A'6312U;
inline constexpr std::uint32_t 支持发布证据材料组属性类型本地键值 = 0x020A'6313U;

inline constexpr std::uint32_t 根概念节点本地键值 = 0x020A'1001U;
inline constexpr std::uint32_t 概念族归属关系本地键值 = 0x020A'1002U;
inline constexpr std::uint32_t 本体根角色关系本地键值 = 0x020A'1003U;
inline constexpr std::uint32_t 首次请求见证值本地键值 = 0x020A'1004U;
inline constexpr std::uint32_t 普通概念节点本地键值 = 0x020A'2001U;
inline constexpr std::uint32_t 普通概念族归属关系本地键值 = 0x020A'2002U;
inline constexpr std::uint32_t 普通概念本体根归属关系本地键值 = 0x020A'2003U;
inline constexpr std::uint32_t 普通概念签名规则关系本地键值 = 0x020A'2004U;
inline constexpr std::uint32_t 普通概念首次请求见证值本地键值 = 0x020A'2005U;
inline constexpr std::uint32_t 普通概念治理生命周期值本地键值 = 0x020A'2006U;
inline constexpr std::uint32_t 支持事实节点本地键值 = 0x020A'4001U;
inline constexpr std::uint32_t 支持登记成员关系本地键值 = 0x020A'4002U;
inline constexpr std::uint32_t 世界事实支持关系本地键值 = 0x020A'4003U;
inline constexpr std::uint32_t 世界事实引用编码值本地键值 = 0x020A'4004U;
inline constexpr std::uint32_t 支持建立依据材料值本地键值 = 0x020A'4005U;
inline constexpr std::uint32_t 支持发布证据材料组值本地键值 = 0x020A'4006U;
inline constexpr std::uint32_t 概念治理生命周期迁移值本地键值 = 0x020A'5001U;
inline constexpr std::int64_t 概念族归属角色值 = 1;

struct 当前代次读取值 final {
    L1所有者范围读取状态 状态 = L1所有者范围读取状态::入口拒绝;
    std::uint64_t 事实代次 = 0;
};

struct 概念身份来源定位 final {
    L1结构所有者身份 所有者;
    稳定编码 族锚点;
    稳定编码 族归属关系类型;
    std::uint64_t 建立事实代次 = 0;
};

struct 概念本体结构定位 final {
    L1结构所有者身份 所有者;
    稳定编码 角色锚点;
    稳定编码 角色关系类型;
    稳定编码 直接上位关系类型;
    稳定编码 首次见证属性类型;
    std::uint64_t 建立事实代次 = 0;
};

struct 普通概念结构定位 final {
    稳定编码 本体根归属关系类型;
    稳定编码 签名规则关系类型;
    稳定编码 签名值式项成员关系类型;
    稳定编码 定义项成员关系类型;
    稳定编码 来源证据关系类型;
    稳定编码 首次见证属性类型;
    稳定编码 项键属性类型;
    稳定编码 签名I64材料属性类型;
    稳定编码 签名I64组材料属性类型;
    稳定编码 签名U64组材料属性类型;
    稳定编码 签名独立材料引用属性类型;
    稳定编码 定义目标编码属性类型;
    稳定编码 治理生命周期属性类型;
    std::uint64_t 建立事实代次 = 0;
};

struct 概念世界事实支持结构定位 final {
    稳定编码 登记根;
    稳定编码 登记成员关系类型;
    稳定编码 支持关系类型;
    稳定编码 世界事实编码属性类型;
    稳定编码 建立依据材料属性类型;
    稳定编码 发布证据材料组属性类型;
    std::uint64_t 建立事实代次 = 0;
};

// 诊断责任：无适用错误分支；把 L1 当前事实代次读取映射为 owner 读取状态。
当前代次读取值 读取当前事实代次(const L1事实基座服务& 第一层服务) noexcept {
    const auto 读取 = 第一层服务.读取中性当前事实代次({L1中性CRUD合同版本});
    L1所有者范围读取状态 状态 = L1所有者范围读取状态::内部不一致;
    switch (读取.状态) {
    case L1中性读取状态::成功:
        状态 = L1所有者范围读取状态::成功;
        break;
    case L1中性读取状态::入口拒绝:
        状态 = L1所有者范围读取状态::入口拒绝;
        break;
    case L1中性读取状态::未找到:
        状态 = L1所有者范围读取状态::未找到;
        break;
    case L1中性读取状态::已退出:
        状态 = L1所有者范围读取状态::已退出;
        break;
    case L1中性读取状态::属性未设置:
        状态 = L1所有者范围读取状态::属性未设置;
        break;
    case L1中性读取状态::资源失败:
        状态 = L1所有者范围读取状态::资源失败;
        break;
    case L1中性读取状态::内部不一致:
        break;
    }
    if (读取.合同版本 != L1中性CRUD合同版本)
        状态 = L1所有者范围读取状态::内部不一致;
    return {状态, 读取.事实代次};
}

// 诊断责任：无适用错误分支；判断登记首次或精确重复回显是否完整。
bool 登记写入结果完整(const L1所有者范围写入结果& 写入,
    L1结构所有者身份 所有者, L1所有者范围写入幂等身份 幂等身份,
    std::size_t 映射数量) noexcept {
    const bool 首次完整 = 写入.状态 == L1所有者范围写入状态::成功
        && 写入.是否形成内存权威发布
        && 写入.重试边界 == L1所有者范围重试边界::不适用;
    const bool 重复完整 = 写入.状态 == L1所有者范围写入状态::精确重复
        && !写入.是否形成内存权威发布
        && 写入.重试边界 == L1所有者范围重试边界::原幂等身份读回收敛;
    return 写入.合同版本 == L1所有者范围CRUD合同版本
        && 写入.所有者 == 所有者 && 写入.写入幂等身份 == 幂等身份
        && 写入.事实代次 != 0 && (首次完整 || 重复完整)
        && 写入.新编码映射.size() == 映射数量;
}

// 诊断责任：向上送出；未知、重复或零编码映射令构造失败。
稳定编码 取得唯一登记编码(const L1所有者范围写入结果& 写入,
    std::uint32_t 本地键值) {
    std::optional<稳定编码> 找到;
    for (const auto& [键, 编码] : 写入.新编码映射) {
        if (键.值 != 本地键值) continue;
        if (找到 || !有效(编码))
            throw std::runtime_error("L2 concept registration mapping is invalid");
        找到 = 编码;
    }
    if (!找到)
        throw std::runtime_error("L2 concept registration mapping is missing");
    return *找到;
}

// 诊断责任：向上送出；登记节点历史读回不完整令构造失败。
L1所有者范围节点事实 读取登记节点(const L1事实基座服务& 第一层服务,
    L1结构所有者身份 所有者, 稳定编码 编码, 节点种类 种类,
    std::optional<L1所有者范围值表示种类> 表示,
    std::uint64_t 建立事实代次) {
    const auto 读取 = 第一层服务.读取所有者范围历史事实(
        {L1所有者范围CRUD合同版本, 编码});
    const auto* 节点 = 读取.事实
        ? std::get_if<L1所有者范围节点事实>(&*读取.事实) : nullptr;
    if (读取.状态 != L1所有者范围读取状态::成功
        || 读取.合同版本 != L1所有者范围CRUD合同版本
        || 读取.查询编码 != 编码 || 读取.读取事实代次 < 建立事实代次
        || !节点 || 节点->编码 != 编码 || 节点->写入所有者 != 所有者
        || 节点->种类 != 种类 || 节点->属性类型表示 != 表示
        || 节点->创建事实代次 != 建立事实代次
        || 节点->退出事实代次 || !节点->当前属性.empty())
        throw std::runtime_error("L2 concept registration readback is invalid");
    return *节点;
}

// 诊断责任：向上送出；概念 owner 或族来源登记不完整令服务构造失败。
概念身份来源定位 初始化概念身份来源(
    const L1事实基座服务& 第一层服务, L1所有者范围写端口& 写入端口) {
    const auto 所有者 = 写入端口.所有者身份();
    const auto 所有者读取 = 第一层服务.读取当前结构所有者(
        {L1所有者范围CRUD合同版本, 所有者});
    if (所有者读取.状态 != L1所有者范围读取状态::成功
        || 所有者读取.合同版本 != L1所有者范围CRUD合同版本
        || 所有者读取.查询所有者 != 所有者 || !所有者读取.所有者事实
        || 所有者读取.所有者事实->所有者 != 所有者
        || 所有者读取.所有者事实->范围种类
            != L1所有者范围种类::独占结构范围
        || 所有者读取.所有者事实->创建事实代次 == 0
        || 所有者读取.所有者事实->退出事实代次)
        throw std::runtime_error("L2 concept owner fact is invalid");

    const auto 当前 = 读取当前事实代次(第一层服务);
    if (当前.状态 != L1所有者范围读取状态::成功 || 当前.事实代次 == 0)
        throw std::runtime_error("L2 concept identity registration generation failed");
    L1所有者范围写集请求 写集;
    写集.合同版本 = L1所有者范围CRUD合同版本;
    写集.期望事实代次 = 当前.事实代次;
    写集.写入幂等身份 = 概念身份来源登记幂等身份;
    写集.节点 = {
        {{概念族锚点本地键值}, 节点种类::普通, std::nullopt},
        {{概念族归属关系类型本地键值}, 节点种类::普通, std::nullopt}};
    const auto 写入 = 写入端口.提交所有者范围中性写集(写集);
    if (!登记写入结果完整(写入, 所有者,
            概念身份来源登记幂等身份, 2))
        throw std::runtime_error("L2 concept identity registration failed");
    const auto 锚点 = 取得唯一登记编码(写入, 概念族锚点本地键值);
    const auto 类型 = 取得唯一登记编码(写入, 概念族归属关系类型本地键值);
    if (锚点 == 类型)
        throw std::runtime_error("L2 concept identity registration aliases facts");
    (void)读取登记节点(第一层服务, 所有者, 锚点, 节点种类::普通,
        std::nullopt, 写入.事实代次);
    (void)读取登记节点(第一层服务, 所有者, 类型, 节点种类::普通,
        std::nullopt, 写入.事实代次);
    return {所有者, 锚点, 类型, 写入.事实代次};
}

// 诊断责任：向上送出；四种本体内部类型必须同 owner 原子登记并正式读回。
概念本体结构定位 初始化概念本体结构(
    const L1事实基座服务& 第一层服务, L1所有者范围写端口& 写入端口) {
    const auto 当前 = 读取当前事实代次(第一层服务);
    if (当前.状态 != L1所有者范围读取状态::成功 || 当前.事实代次 == 0)
        throw std::runtime_error("L2 concept ontology registration generation failed");
    const auto 所有者 = 写入端口.所有者身份();
    L1所有者范围写集请求 写集;
    写集.合同版本 = L1所有者范围CRUD合同版本;
    写集.期望事实代次 = 当前.事实代次;
    写集.写入幂等身份 = 概念本体结构登记幂等身份;
    写集.节点 = {
        {{本体根角色锚点本地键值}, 节点种类::普通, std::nullopt},
        {{本体根角色关系类型本地键值}, 节点种类::普通, std::nullopt},
        {{直接上位关系类型本地键值}, 节点种类::普通, std::nullopt},
        {{首次请求见证属性类型本地键值}, 节点种类::属性类型,
            L1所有者范围值表示种类::U64组}};
    const auto 写入 = 写入端口.提交所有者范围中性写集(写集);
    if (!登记写入结果完整(写入, 所有者,
            概念本体结构登记幂等身份, 4))
        throw std::runtime_error("L2 concept ontology registration failed");
    const auto 角色锚点 = 取得唯一登记编码(写入, 本体根角色锚点本地键值);
    const auto 角色类型 = 取得唯一登记编码(写入, 本体根角色关系类型本地键值);
    const auto 上位类型 = 取得唯一登记编码(写入, 直接上位关系类型本地键值);
    const auto 见证类型 = 取得唯一登记编码(写入, 首次请求见证属性类型本地键值);
    const std::vector<std::uint64_t> 编码组{
        角色锚点.值, 角色类型.值, 上位类型.值, 见证类型.值};
    auto 排序编码 = 编码组;
    std::sort(排序编码.begin(), 排序编码.end());
    if (std::adjacent_find(排序编码.begin(), 排序编码.end()) != 排序编码.end())
        throw std::runtime_error("L2 concept ontology registration aliases facts");
    (void)读取登记节点(第一层服务, 所有者, 角色锚点,
        节点种类::普通, std::nullopt, 写入.事实代次);
    (void)读取登记节点(第一层服务, 所有者, 角色类型,
        节点种类::普通, std::nullopt, 写入.事实代次);
    (void)读取登记节点(第一层服务, 所有者, 上位类型,
        节点种类::普通, std::nullopt, 写入.事实代次);
    (void)读取登记节点(第一层服务, 所有者, 见证类型,
        节点种类::属性类型, L1所有者范围值表示种类::U64组,
        写入.事实代次);
    return {所有者, 角色锚点, 角色类型, 上位类型, 见证类型,
        写入.事实代次};
}

// 诊断责任：向上送出；普通概念私有类型必须同 owner 原子登记并逐项读回。
普通概念结构定位 初始化普通概念结构(
    const L1事实基座服务& 第一层服务, L1所有者范围写端口& 写入端口,
    const 概念身份来源定位& 来源定位,
    const 概念本体结构定位& 本体定位) {
    const auto 当前 = 读取当前事实代次(第一层服务);
    if (当前.状态 != L1所有者范围读取状态::成功 || 当前.事实代次 == 0)
        throw std::runtime_error("L2 ordinary concept registration generation failed");
    const auto 所有者 = 写入端口.所有者身份();
    L1所有者范围写集请求 写集;
    写集.合同版本 = L1所有者范围CRUD合同版本;
    写集.期望事实代次 = 当前.事实代次;
    写集.写入幂等身份 = 普通概念结构登记幂等身份;
    写集.节点 = {
        {{本体根归属关系类型本地键值}, 节点种类::普通, std::nullopt},
        {{签名规则关系类型本地键值}, 节点种类::普通, std::nullopt},
        {{签名值式项成员关系类型本地键值}, 节点种类::普通, std::nullopt},
        {{定义项成员关系类型本地键值}, 节点种类::普通, std::nullopt},
        {{来源证据关系类型本地键值}, 节点种类::普通, std::nullopt},
        {{普通概念首次请求见证属性类型本地键值}, 节点种类::属性类型,
            L1所有者范围值表示种类::U64组},
        {{项键属性类型本地键值}, 节点种类::属性类型,
            L1所有者范围值表示种类::U64组},
        {{签名I64材料属性类型本地键值}, 节点种类::属性类型,
            L1所有者范围值表示种类::I64},
        {{签名I64组材料属性类型本地键值}, 节点种类::属性类型,
            L1所有者范围值表示种类::I64组},
        {{签名U64组材料属性类型本地键值}, 节点种类::属性类型,
            L1所有者范围值表示种类::U64组},
        {{签名独立材料引用属性类型本地键值}, 节点种类::属性类型,
            L1所有者范围值表示种类::独立材料引用},
        {{定义目标编码属性类型本地键值}, 节点种类::属性类型,
            L1所有者范围值表示种类::U64组},
        {{概念治理生命周期属性类型本地键值}, 节点种类::属性类型,
            L1所有者范围值表示种类::I64}};
    const auto 写入 = 写入端口.提交所有者范围中性写集(写集);
    if (!登记写入结果完整(写入, 所有者,
            普通概念结构登记幂等身份, 13))
        throw std::runtime_error("L2 ordinary concept registration failed");

    普通概念结构定位 定位{
        取得唯一登记编码(写入, 本体根归属关系类型本地键值),
        取得唯一登记编码(写入, 签名规则关系类型本地键值),
        取得唯一登记编码(写入, 签名值式项成员关系类型本地键值),
        取得唯一登记编码(写入, 定义项成员关系类型本地键值),
        取得唯一登记编码(写入, 来源证据关系类型本地键值),
        取得唯一登记编码(写入, 普通概念首次请求见证属性类型本地键值),
        取得唯一登记编码(写入, 项键属性类型本地键值),
        取得唯一登记编码(写入, 签名I64材料属性类型本地键值),
        取得唯一登记编码(写入, 签名I64组材料属性类型本地键值),
        取得唯一登记编码(写入, 签名U64组材料属性类型本地键值),
        取得唯一登记编码(写入, 签名独立材料引用属性类型本地键值),
        取得唯一登记编码(写入, 定义目标编码属性类型本地键值),
        取得唯一登记编码(写入, 概念治理生命周期属性类型本地键值),
        写入.事实代次};
    const std::vector<std::pair<稳定编码,
        std::optional<L1所有者范围值表示种类>>> 登记{
        {定位.本体根归属关系类型, std::nullopt},
        {定位.签名规则关系类型, std::nullopt},
        {定位.签名值式项成员关系类型, std::nullopt},
        {定位.定义项成员关系类型, std::nullopt},
        {定位.来源证据关系类型, std::nullopt},
        {定位.首次见证属性类型, L1所有者范围值表示种类::U64组},
        {定位.项键属性类型, L1所有者范围值表示种类::U64组},
        {定位.签名I64材料属性类型, L1所有者范围值表示种类::I64},
        {定位.签名I64组材料属性类型, L1所有者范围值表示种类::I64组},
        {定位.签名U64组材料属性类型, L1所有者范围值表示种类::U64组},
        {定位.签名独立材料引用属性类型,
            L1所有者范围值表示种类::独立材料引用},
        {定位.定义目标编码属性类型, L1所有者范围值表示种类::U64组},
        {定位.治理生命周期属性类型, L1所有者范围值表示种类::I64}};
    std::vector<std::uint64_t> 编码组{
        来源定位.族锚点.值,
        来源定位.族归属关系类型.值,
        本体定位.角色锚点.值,
        本体定位.角色关系类型.值,
        本体定位.直接上位关系类型.值,
        本体定位.首次见证属性类型.值};
    编码组.reserve(登记.size() + 编码组.size());
    for (const auto& [编码, 表示] : 登记) {
        编码组.push_back(编码.值);
        (void)读取登记节点(第一层服务, 所有者, 编码,
            表示 ? 节点种类::属性类型 : 节点种类::普通,
            表示, 写入.事实代次);
    }
    std::sort(编码组.begin(), 编码组.end());
    if (std::adjacent_find(编码组.begin(), 编码组.end()) != 编码组.end())
        throw std::runtime_error("L2 ordinary concept registration aliases facts");
    return 定位;
}

// 诊断责任：向上送出；支持登记根、两种关系类型和三个属性类型必须独立原子登记。
概念世界事实支持结构定位 初始化概念世界事实支持结构(
    const L1事实基座服务& 第一层服务, L1所有者范围写端口& 写入端口,
    const 概念身份来源定位& 来源定位,
    const 概念本体结构定位& 本体定位,
    const 普通概念结构定位& 普通定位) {
    const auto 当前 = 读取当前事实代次(第一层服务);
    if (当前.状态 != L1所有者范围读取状态::成功 || 当前.事实代次 == 0)
        throw std::runtime_error("L2 concept support registration generation failed");
    const auto 所有者 = 写入端口.所有者身份();
    L1所有者范围写集请求 写集;
    写集.合同版本 = L1所有者范围CRUD合同版本;
    写集.期望事实代次 = 当前.事实代次;
    写集.写入幂等身份 = 概念世界事实支持结构登记幂等身份;
    写集.节点 = {
        {{支持登记根节点本地键值}, 节点种类::普通, std::nullopt},
        {{支持登记成员关系类型本地键值}, 节点种类::普通, std::nullopt},
        {{世界事实支持关系类型本地键值}, 节点种类::普通, std::nullopt},
        {{世界事实引用编码属性类型本地键值}, 节点种类::属性类型,
            L1所有者范围值表示种类::U64组},
        {{支持建立依据材料属性类型本地键值}, 节点种类::属性类型,
            L1所有者范围值表示种类::独立材料引用},
        {{支持发布证据材料组属性类型本地键值}, 节点种类::属性类型,
            L1所有者范围值表示种类::U64组}};
    const auto 写入 = 写入端口.提交所有者范围中性写集(写集);
    if (!登记写入结果完整(写入, 所有者,
            概念世界事实支持结构登记幂等身份, 6))
        throw std::runtime_error("L2 concept support registration failed");
    概念世界事实支持结构定位 定位{
        取得唯一登记编码(写入, 支持登记根节点本地键值),
        取得唯一登记编码(写入, 支持登记成员关系类型本地键值),
        取得唯一登记编码(写入, 世界事实支持关系类型本地键值),
        取得唯一登记编码(写入, 世界事实引用编码属性类型本地键值),
        取得唯一登记编码(写入, 支持建立依据材料属性类型本地键值),
        取得唯一登记编码(写入, 支持发布证据材料组属性类型本地键值),
        写入.事实代次};
    const std::vector<std::pair<稳定编码,
        std::optional<L1所有者范围值表示种类>>> 登记{
        {定位.登记根, std::nullopt},
        {定位.登记成员关系类型, std::nullopt},
        {定位.支持关系类型, std::nullopt},
        {定位.世界事实编码属性类型, L1所有者范围值表示种类::U64组},
        {定位.建立依据材料属性类型,
            L1所有者范围值表示种类::独立材料引用},
        {定位.发布证据材料组属性类型, L1所有者范围值表示种类::U64组}};
    std::vector<std::uint64_t> 编码组{
        来源定位.族锚点.值, 来源定位.族归属关系类型.值,
        本体定位.角色锚点.值, 本体定位.角色关系类型.值,
        本体定位.直接上位关系类型.值, 本体定位.首次见证属性类型.值,
        普通定位.本体根归属关系类型.值,
        普通定位.签名规则关系类型.值};
    编码组.reserve(编码组.size() + 登记.size());
    for (const auto& [编码, 表示] : 登记) {
        编码组.push_back(编码.值);
        (void)读取登记节点(第一层服务, 所有者, 编码,
            表示 ? 节点种类::属性类型 : 节点种类::普通,
            表示, 写入.事实代次);
    }
    std::sort(编码组.begin(), 编码组.end());
    if (std::adjacent_find(编码组.begin(), 编码组.end()) != 编码组.end())
        throw std::runtime_error("L2 concept support registration aliases facts");
    return 定位;
}

// 诊断责任：无适用错误分支；纯值映射覆盖全部 L1 一致读取状态。
L2结构状态 映射一致读取状态(L1所有者范围一致当前读取状态 状态) noexcept {
    switch (状态) {
    case L1所有者范围一致当前读取状态::成功:
        return L2结构状态::已读取;
    case L1所有者范围一致当前读取状态::入口拒绝:
        return L2结构状态::入口拒绝;
    case L1所有者范围一致当前读取状态::许可拒绝:
        return L2结构状态::许可拒绝;
    case L1所有者范围一致当前读取状态::事实代次漂移:
        return L2结构状态::事实代次漂移;
    case L1所有者范围一致当前读取状态::资源失败:
        return L2结构状态::资源失败;
    case L1所有者范围一致当前读取状态::内部不一致:
        return L2结构状态::内部不一致;
    }
    return L2结构状态::内部不一致;
}

// 诊断责任：无适用错误分支；纯值映射覆盖全部 owner 写入状态。
L2结构状态 映射写入状态(L1所有者范围写入状态 状态) noexcept {
    switch (状态) {
    case L1所有者范围写入状态::成功:
        return L2结构状态::已提交;
    case L1所有者范围写入状态::精确重复:
        return L2结构状态::精确重复;
    case L1所有者范围写入状态::入口拒绝:
        return L2结构状态::入口拒绝;
    case L1所有者范围写入状态::许可拒绝:
        return L2结构状态::许可拒绝;
    case L1所有者范围写入状态::事实代次漂移:
        return L2结构状态::事实代次漂移;
    case L1所有者范围写入状态::幂等冲突:
        return L2结构状态::幂等冲突;
    case L1所有者范围写入状态::引用冲突:
        return L2结构状态::引用冲突;
    case L1所有者范围写入状态::资源失败:
        return L2结构状态::资源失败;
    case L1所有者范围写入状态::未找到:
    case L1所有者范围写入状态::已退出:
    case L1所有者范围写入状态::内部不一致:
        return L2结构状态::内部不一致;
    }
    return L2结构状态::内部不一致;
}

// 诊断责任：无适用错误分支；所有非成功根组结果保持空载荷和零变更。
L2当前概念本体根组读取结果 形成根组失败(
    L2结构状态 状态, std::uint64_t 事实截止代次 = 0) noexcept {
    return {{L2结构合同版本, 状态, 事实截止代次, std::nullopt}, {}};
}

// 诊断责任：无适用错误分支；登记节点在当前闭包中的固定形状只作纯值校验。
bool 登记节点当前完整(const L1所有者范围节点事实& 节点,
    L1结构所有者身份 所有者, 稳定编码 编码, 节点种类 种类,
    std::optional<L1所有者范围值表示种类> 表示,
    std::uint64_t 建立事实代次) noexcept {
    return 节点.编码 == 编码 && 节点.写入所有者 == 所有者
        && 节点.种类 == 种类 && 节点.属性类型表示 == 表示
        && 节点.创建事实代次 == 建立事实代次
        && !节点.退出事实代次 && 节点.当前属性.empty();
}

// 诊断责任：向上送出；一次 L1 闭包许可内形成所有本体根的同截止正式负载。
L2当前概念本体根组读取结果 读取全部当前概念本体根核心(
    const L1事实基座服务& 第一层服务,
    const 概念身份来源定位& 来源定位,
    const 概念本体结构定位& 本体定位,
    const L2当前概念本体根组读取请求& 请求) {
    L1所有者范围一致关系类型闭包读取请求 L1请求;
    L1请求.合同版本 = L1所有者范围一致关系类型闭包读取合同版本;
    L1请求.期望事实代次 = 请求.请求头.期望事实代次;
    L1请求.所有者 = {来源定位.所有者};
    L1请求.关系类型闭包 = {{本体定位.角色关系类型,
        {本体定位.首次见证属性类型}, {来源定位.族归属关系类型},
        {本体定位.直接上位关系类型}, {}, {}, {}}};
    const auto 读取 = 第一层服务.尝试读取所有者范围一致关系类型闭包投影(
        L1请求);
    const bool 顶层空载荷 = 读取.所有者.empty() && 读取.节点.empty()
        && 读取.关系.empty() && 读取.值.empty() && 读取.属性值.empty()
        && 读取.源关系组.empty() && 读取.目标关系组.empty()
        && 读取.关系类型闭包.empty();
    if (读取.状态 != L1所有者范围一致当前读取状态::成功) {
        const bool 头完整 = 读取.合同版本
                == L1所有者范围一致关系类型闭包读取合同版本
            && 读取.期望事实代次 == 请求.请求头.期望事实代次;
        return 形成根组失败(头完整 && 顶层空载荷
                ? 映射一致读取状态(读取.状态)
                : L2结构状态::内部不一致,
            读取.读取事实代次);
    }
    if (读取.合同版本 != L1所有者范围一致关系类型闭包读取合同版本
        || 读取.期望事实代次 != 请求.请求头.期望事实代次
        || 读取.读取事实代次 == 0
        || (请求.请求头.期望事实代次 != 0
            && 读取.读取事实代次 != 请求.请求头.期望事实代次)
        || 读取.所有者.size() != 1 || !读取.节点.empty()
        || !读取.关系.empty() || !读取.值.empty() || !读取.属性值.empty()
        || !读取.源关系组.empty() || !读取.目标关系组.empty()
        || 读取.关系类型闭包.size() != 1)
        return 形成根组失败(L2结构状态::内部不一致, 读取.读取事实代次);

    const auto 截止 = 读取.读取事实代次;
    const auto& 所有者项 = 读取.所有者.front();
    if (所有者项.查询所有者 != 来源定位.所有者
        || 所有者项.状态 != L1所有者范围一致当前读取项目状态::成功
        || !所有者项.所有者事实
        || 所有者项.所有者事实->所有者 != 来源定位.所有者
        || 所有者项.所有者事实->范围种类
            != L1所有者范围种类::独占结构范围
        || 所有者项.所有者事实->创建事实代次 == 0
        || 所有者项.所有者事实->创建事实代次 > 截止
        || 所有者项.所有者事实->退出事实代次)
        return 形成根组失败(L2结构状态::内部不一致, 截止);

    const auto& 闭包 = 读取.关系类型闭包.front();
    if (闭包.入口关系类型节点 != 本体定位.角色关系类型
        || 闭包.状态 != L1所有者范围一致当前读取项目状态::成功
        || !闭包.关系类型事实
        || !登记节点当前完整(*闭包.关系类型事实, 来源定位.所有者,
            本体定位.角色关系类型, 节点种类::普通, std::nullopt,
            本体定位.建立事实代次))
        return 形成根组失败(L2结构状态::内部不一致, 截止);

    std::vector<L2概念本体根事实> 根组;
    根组.reserve(闭包.成员.size());
    for (const auto& 成员 : 闭包.成员) {
        if (成员.关系.写入所有者 != 来源定位.所有者
            || 成员.关系.关系类型节点 != 本体定位.角色关系类型
            || 成员.关系.源节点 != 成员.源节点.编码
            || 成员.关系.目标节点 != 成员.目标节点.编码
            || 成员.关系.目标节点 != 本体定位.角色锚点
            || 成员.关系.退出事实代次
            || 成员.源节点.写入所有者 != 来源定位.所有者
            || 成员.源节点.种类 != 节点种类::普通
            || 成员.源节点.属性类型表示 || 成员.源节点.退出事实代次
            || 成员.源节点.创建事实代次 != 成员.关系.创建事实代次
            || !登记节点当前完整(成员.目标节点, 来源定位.所有者,
                本体定位.角色锚点, 节点种类::普通, std::nullopt,
                本体定位.建立事实代次)
            || 成员.源节点属性值.size() != 1
            || 成员.源节点源关系组.size() != 1
            || 成员.源节点目标关系组.size() != 1
            || !成员.目标节点属性值.empty()
            || !成员.目标节点源关系组.empty()
            || !成员.目标节点目标关系组.empty())
            return 形成根组失败(L2结构状态::内部不一致, 截止);

        const auto 角色值 = 成员.关系.角色或顺序;
        if (角色值 < 1 || 角色值 > 4)
            return 形成根组失败(L2结构状态::内部不一致, 截止);
        const auto 角色 = static_cast<L2概念本体根角色>(角色值);
        if (!L2概念本体根角色有效(角色))
            return 形成根组失败(L2结构状态::内部不一致, 截止);

        const auto& 属性项 = 成员.源节点属性值.front();
        if (属性项.节点 != 成员.源节点.编码
            || 属性项.属性类型 != 本体定位.首次见证属性类型
            || 属性项.状态 != L1所有者范围一致当前读取项目状态::成功
            || !属性项.投影)
            return 形成根组失败(L2结构状态::内部不一致, 截止);
        const auto& 属性槽 = 属性项.投影->属性槽;
        const auto& 见证值 = 属性项.投影->当前值事实;
        const auto* 见证材料 = std::get_if<std::vector<std::uint64_t>>(
            &见证值.材料);
        if (成员.源节点.当前属性.size() != 1
            || 成员.源节点.当前属性.front() != 属性槽
            || 属性槽.属性类型节点 != 本体定位.首次见证属性类型
            || 属性槽.当前值 != 见证值.编码
            || 见证值.写入所有者 != 来源定位.所有者
            || 见证值.所属节点 != 成员.源节点.编码
            || 见证值.属性类型节点 != 本体定位.首次见证属性类型
            || 见证值.来源节点 != 成员.源节点.编码
            || 见证值.创建事实代次 != 成员.源节点.创建事实代次
            || 见证值.退出事实代次 || !见证材料
            || 见证材料->size() != 2 || (*见证材料)[0] == 0
            || (*见证材料)[1] == 0)
            return 形成根组失败(L2结构状态::内部不一致, 截止);

        const auto& 族组 = 成员.源节点源关系组.front();
        if (族组.端点节点 != 成员.源节点.编码
            || 族组.关系类型节点 != 来源定位.族归属关系类型
            || 族组.状态 != L1所有者范围一致当前读取项目状态::成功
            || !族组.关系类型事实
            || !登记节点当前完整(*族组.关系类型事实,
                来源定位.所有者, 来源定位.族归属关系类型,
                节点种类::普通, std::nullopt, 来源定位.建立事实代次)
            || 族组.成员.size() != 1)
            return 形成根组失败(L2结构状态::内部不一致, 截止);
        const auto& 族投影 = 族组.成员.front();
        if (族投影.关系.写入所有者 != 来源定位.所有者
            || 族投影.关系.源节点 != 成员.源节点.编码
            || 族投影.关系.目标节点 != 来源定位.族锚点
            || 族投影.关系.关系类型节点 != 来源定位.族归属关系类型
            || 族投影.关系.角色或顺序 != 概念族归属角色值
            || 族投影.关系.创建事实代次 != 成员.源节点.创建事实代次
            || 族投影.关系.退出事实代次
            || !登记节点当前完整(族投影.对端节点, 来源定位.所有者,
                来源定位.族锚点, 节点种类::普通, std::nullopt,
                来源定位.建立事实代次))
            return 形成根组失败(L2结构状态::内部不一致, 截止);

        const auto& 上位组 = 成员.源节点目标关系组.front();
        if (上位组.端点节点 != 成员.源节点.编码
            || 上位组.关系类型节点 != 本体定位.直接上位关系类型
            || 上位组.状态 != L1所有者范围一致当前读取项目状态::成功
            || !上位组.关系类型事实
            || !登记节点当前完整(*上位组.关系类型事实,
                来源定位.所有者, 本体定位.直接上位关系类型,
                节点种类::普通, std::nullopt, 本体定位.建立事实代次)
            || !上位组.成员.empty())
            return 形成根组失败(L2结构状态::内部不一致, 截止);

        L2节点族身份来源事实 身份来源{
            L2节点族::概念, 成员.源节点.编码, 来源定位.所有者,
            来源定位.族锚点, 来源定位.族归属关系类型,
            族投影.关系.编码, 族投影.关系.角色或顺序,
            {所有者项.所有者事实->创建事实代次,
                所有者项.所有者事实->退出事实代次},
            {成员.源节点.创建事实代次, 成员.源节点.退出事实代次},
            {族投影.对端节点.创建事实代次,
                族投影.对端节点.退出事实代次},
            {族组.关系类型事实->创建事实代次,
                族组.关系类型事实->退出事实代次},
            {族投影.关系.创建事实代次, 族投影.关系.退出事实代次}};
        L2概念本体根事实 根{
            L2概念身份{成员.源节点.编码}, std::move(身份来源),
            {角色, L2概念身份{成员.源节点.编码}, 成员.关系.编码,
                {成员.关系.创建事实代次, 成员.关系.退出事实代次}},
            {(*见证材料)[0]}, (*见证材料)[1], {}};
        if (!L2概念本体根事实完整(根, 截止))
            return 形成根组失败(L2结构状态::内部不一致, 截止);
        根组.push_back(std::move(根));
    }

    std::sort(根组.begin(), 根组.end(), [](const auto& 左, const auto& 右) {
        return static_cast<std::uint8_t>(左.根角色.角色)
            < static_cast<std::uint8_t>(右.根角色.角色);
    });
    for (std::size_t 索引 = 0; 索引 < 根组.size(); ++索引) {
        for (std::size_t 已有 = 0; 已有 < 索引; ++已有) {
            if (根组[已有].根角色.角色 == 根组[索引].根角色.角色
                || 根组[已有].根概念 == 根组[索引].根概念
                || 根组[已有].首次幂等身份 == 根组[索引].首次幂等身份)
                return 形成根组失败(L2结构状态::内部不一致, 截止);
        }
    }
    L2当前概念本体根组读取结果 结果{
        {L2结构合同版本, L2结构状态::已读取, 截止, std::nullopt},
        std::move(根组)};
    return 结果.成功() ? 结果
        : 形成根组失败(L2结构状态::内部不一致, 截止);
}

// 诊断责任：无适用错误分支；四项私有登记身份不进入公开幂等域。
bool 是概念保留登记幂等身份(L2结构幂等身份 身份) noexcept {
    return 身份.值 == 概念身份来源登记幂等身份.值
        || 身份.值 == 概念本体结构登记幂等身份.值
        || 身份.值 == 普通概念结构登记幂等身份.值
        || 身份.值 == 概念世界事实支持结构登记幂等身份.值;
}

// 诊断责任：无适用错误分支；公开幂等身份逐值无损映射到唯一 owner 域。
L1所有者范围写入幂等身份 映射概念幂等身份(L2结构幂等身份 身份) noexcept {
    return {身份.值};
}

// 诊断责任：无适用错误分支；普通概念非成功结果始终为空负载、零变更。
L2普通概念写入结果 形成普通概念写入失败(
    L2结构状态 状态, std::uint64_t 截止 = 0) noexcept {
    if (状态 == L2结构状态::已提交 || 状态 == L2结构状态::精确重复
        || 状态 == L2结构状态::已读取) 状态 = L2结构状态::内部不一致;
    return {{L2结构合同版本, 状态, 截止, std::nullopt}, std::nullopt};
}

// 诊断责任：无适用错误分支；普通概念读取失败保留请求类别但不泄漏部分负载。
L2普通概念读取结果 形成普通概念读取失败(
    const L2普通概念读取请求& 请求, L2结构状态 状态,
    std::uint64_t 截止 = 0) noexcept {
    if (状态 == L2结构状态::已提交 || 状态 == L2结构状态::精确重复
        || 状态 == L2结构状态::已读取) 状态 = L2结构状态::内部不一致;
    return {{L2结构合同版本, 状态, 截止, std::nullopt}, 请求.读取类别,
        请求.读取类别 == L2读取类别::历史
            ? 请求.历史截止事实代次 : 0,
        std::nullopt};
}

// 诊断责任：无适用错误分支；L1 历史读取状态无损映射到共同结构状态。
L2结构状态 映射历史读取状态(L1所有者范围读取状态 状态) noexcept {
    switch (状态) {
    case L1所有者范围读取状态::成功: return L2结构状态::已读取;
    case L1所有者范围读取状态::入口拒绝: return L2结构状态::入口拒绝;
    case L1所有者范围读取状态::许可拒绝: return L2结构状态::许可拒绝;
    case L1所有者范围读取状态::未找到: return L2结构状态::未找到;
    case L1所有者范围读取状态::已退出: return L2结构状态::已退出;
    case L1所有者范围读取状态::属性未设置:
        return L2结构状态::内部不一致;
    case L1所有者范围读取状态::事实代次漂移:
        return L2结构状态::事实代次漂移;
    case L1所有者范围读取状态::资源失败: return L2结构状态::资源失败;
    case L1所有者范围读取状态::内部不一致:
        return L2结构状态::内部不一致;
    }
    return L2结构状态::内部不一致;
}

// 诊断责任：无适用错误分支；闭合定义目标编码不依赖运行期类型表。
std::vector<std::uint64_t> 编码概念定义目标(const L2概念定义目标& 目标) {
    return std::visit([](const auto& 值) -> std::vector<std::uint64_t> {
        using 类型 = std::decay_t<decltype(值)>;
        if constexpr (std::is_same_v<类型, L2概念身份>) return {1, 值.值.值};
        else if constexpr (std::is_same_v<类型, L2场景身份>) return {2, 值.值.值};
        else if constexpr (std::is_same_v<类型, L2存在身份>) return {3, 值.值.值};
        else if constexpr (std::is_same_v<类型, L2特征定义身份>) return {4, 值.值.值};
        else if constexpr (std::is_same_v<类型, L2特征实例身份>) return {5, 值.值.值};
        else if constexpr (std::is_same_v<类型, L2概念特征值引用>)
            return {6, 值.特征实例.值.值, 值.值稳定编码.值};
        else if constexpr (std::is_same_v<类型, L2状态身份>) return {7, 值.值.值};
        else if constexpr (std::is_same_v<类型, L2动态身份>) return {8, 值.值.值};
        else if constexpr (std::is_same_v<类型, L2因果身份>) return {9, 值.值.值};
        else {
            const auto 端点 = [](const L2概念世界关系端点& 端)
                -> std::pair<std::uint64_t, std::uint64_t> {
                return std::visit([](const auto& 项) {
                    using 端类型 = std::decay_t<decltype(项)>;
                    if constexpr (std::is_same_v<端类型, L2场景身份>)
                        return std::pair<std::uint64_t, std::uint64_t>{1, 项.值.值};
                    else if constexpr (std::is_same_v<端类型, L2存在身份>)
                        return std::pair<std::uint64_t, std::uint64_t>{2, 项.值.值};
                    else if constexpr (std::is_same_v<端类型, L2特征定义身份>)
                        return std::pair<std::uint64_t, std::uint64_t>{3, 项.值.值};
                    else if constexpr (std::is_same_v<端类型, L2特征实例身份>)
                        return std::pair<std::uint64_t, std::uint64_t>{4, 项.值.值};
                    else if constexpr (std::is_same_v<端类型, L2状态身份>)
                        return std::pair<std::uint64_t, std::uint64_t>{5, 项.值.值};
                    else if constexpr (std::is_same_v<端类型, L2动态身份>)
                        return std::pair<std::uint64_t, std::uint64_t>{6, 项.值.值};
                    else if constexpr (std::is_same_v<端类型, L2因果身份>)
                        return std::pair<std::uint64_t, std::uint64_t>{7, 项.值.值};
                    else return std::pair<std::uint64_t, std::uint64_t>{8, 项.值.值};
                }, 端);
            };
            const auto 源 = 端点(值.源);
            const auto 目标端 = 端点(值.目标);
            return {10, static_cast<std::uint64_t>(值.类别),
                值.关系稳定编码.值, 源.first, 源.second,
                目标端.first, 目标端.second};
        }
    }, 目标);
}

// 诊断责任：无适用错误分支；只接受本合同定义的十类闭合目标编码。
std::optional<L2概念定义目标> 解码概念定义目标(
    const std::vector<std::uint64_t>& 编码) noexcept {
    if (编码.size() < 2 || 编码[0] == 0) return std::nullopt;
    const auto 稳定 = [](std::uint64_t 值) { return 稳定编码{值}; };
    if (编码[0] >= 1 && 编码[0] <= 5 && 编码.size() == 2 && 编码[1] != 0) {
        switch (编码[0]) {
        case 1: return L2概念身份{稳定(编码[1])};
        case 2: return L2场景身份{稳定(编码[1])};
        case 3: return L2存在身份{稳定(编码[1])};
        case 4: return L2特征定义身份{稳定(编码[1])};
        case 5: return L2特征实例身份{稳定(编码[1])};
        }
    }
    if (编码[0] == 6 && 编码.size() == 3 && 编码[1] != 0 && 编码[2] != 0)
        return L2概念特征值引用{L2特征实例身份{稳定(编码[1])},
            稳定(编码[2])};
    if (编码[0] >= 7 && 编码[0] <= 9 && 编码.size() == 2 && 编码[1] != 0) {
        if (编码[0] == 7) return L2状态身份{稳定(编码[1])};
        if (编码[0] == 8) return L2动态身份{稳定(编码[1])};
        return L2因果身份{稳定(编码[1])};
    }
    if (编码[0] != 10 || 编码.size() != 7 || 编码[1] < 1 || 编码[1] > 13
        || 编码[2] == 0 || 编码[3] < 1 || 编码[3] > 8 || 编码[4] == 0
        || 编码[5] < 1 || 编码[5] > 8 || 编码[6] == 0) return std::nullopt;
    const auto 端点 = [&](std::uint64_t 标签, std::uint64_t 值)
        -> L2概念世界关系端点 {
        switch (标签) {
        case 1: return L2场景身份{稳定(值)};
        case 2: return L2存在身份{稳定(值)};
        case 3: return L2特征定义身份{稳定(值)};
        case 4: return L2特征实例身份{稳定(值)};
        case 5: return L2状态身份{稳定(值)};
        case 6: return L2动态身份{稳定(值)};
        case 7: return L2因果身份{稳定(值)};
        default: return L2概念动作引用{稳定(值)};
        }
    };
    L2概念世界关系引用 引用{static_cast<L2概念世界关系类别>(编码[1]),
        稳定(编码[2]), 端点(编码[3], 编码[4]), 端点(编码[5], 编码[6])};
    if (!L2概念世界关系引用有效(引用)) return std::nullopt;
    return 引用;
}

// 诊断责任：无适用错误分支；世界事实复用闭合定义编码但从类型上排除概念身份。
std::vector<std::uint64_t> 编码世界事实引用(const L2世界事实引用& 引用) {
    return std::visit([](const auto& 值) {
        return 编码概念定义目标(L2概念定义目标{值});
    }, 引用);
}

// 诊断责任：无适用错误分支；概念分支或坏编码不得进入世界事实闭集。
std::optional<L2世界事实引用> 解码世界事实引用(
    const std::vector<std::uint64_t>& 编码) noexcept {
    const auto 目标 = 解码概念定义目标(编码);
    if (!目标 || std::holds_alternative<L2概念身份>(*目标)) return std::nullopt;
    return std::visit([](const auto& 值) -> std::optional<L2世界事实引用> {
        using 类型 = std::decay_t<decltype(值)>;
        if constexpr (std::is_same_v<类型, L2概念身份>) return std::nullopt;
        else return L2世界事实引用{值};
    }, *目标);
}

// 诊断责任：无适用错误分支；普通值材料与 L1 owner 写集表示逐分支无损转换。
L1所有者范围原始值材料 映射普通概念材料到L1(const L2原始值材料& 材料) {
    return std::visit([](const auto& 值) -> L1所有者范围原始值材料 {
        using 类型 = std::decay_t<decltype(值)>;
        if constexpr (std::is_same_v<类型, L2独立材料引用>)
            return L1所有者范围独立材料引用{值.编码};
        else return 值;
    }, 材料);
}

std::optional<L2原始值材料> 映射L1材料到普通概念(
    const L1所有者范围原始值材料& 材料) noexcept {
    return std::visit([](const auto& 值) -> std::optional<L2原始值材料> {
        using 类型 = std::decay_t<decltype(值)>;
        if constexpr (std::is_same_v<类型, L1所有者范围独立材料引用>)
            return L2独立材料引用{值.编码};
        else return L2原始值材料{值};
    }, 材料);
}

// 诊断责任：无适用错误分支；固定高位分区必须容纳全部确定排序项。
bool 普通概念写集数量可表示(const L2普通概念建立请求& 请求) noexcept {
    constexpr std::size_t 分区容量 = 0x1000'0000ULL;
    return 请求.直接上位.size() <= 分区容量
        && 请求.来源证据材料.size() <= 分区容量
        && 请求.签名值式项.size() <= 分区容量
        && 请求.初始定义.size() <= 分区容量;
}

// 诊断责任：无适用错误分支；首次结果必须逐键覆盖本请求的完整确定写集且编码互异。
std::optional<稳定编码> 读取普通概念节点映射(
    const L1所有者范围写入结果& 写入,
    const L2普通概念建立请求& 请求) {
    std::vector<std::uint32_t> 期望键{
        普通概念节点本地键值,
        普通概念族归属关系本地键值,
        普通概念本体根归属关系本地键值,
        普通概念签名规则关系本地键值,
        普通概念首次请求见证值本地键值,
        普通概念治理生命周期值本地键值};
    const auto 追加分区 = [&](std::uint32_t 分区, std::size_t 数量) {
        for (std::size_t 索引 = 0; 索引 < 数量; ++索引)
            期望键.push_back(分区 + static_cast<std::uint32_t>(索引));
    };
    追加分区(0x1000'0000U, 请求.直接上位.size());
    追加分区(0x2000'0000U, 请求.来源证据材料.size());
    追加分区(0x3000'0000U, 请求.签名值式项.size());
    追加分区(0x4000'0000U, 请求.签名值式项.size());
    追加分区(0x5000'0000U, 请求.签名值式项.size());
    追加分区(0x6000'0000U, 请求.签名值式项.size());
    追加分区(0x7000'0000U, 请求.初始定义.size());
    追加分区(0x8000'0000U, 请求.初始定义.size());
    追加分区(0x9000'0000U, 请求.初始定义.size());
    追加分区(0xA000'0000U, 请求.初始定义.size());
    if (写入.新编码映射.size() != 期望键.size()) return std::nullopt;

    std::vector<std::uint32_t> 实际键;
    std::vector<std::uint64_t> 实际编码;
    实际键.reserve(写入.新编码映射.size());
    实际编码.reserve(写入.新编码映射.size());
    std::optional<稳定编码> 概念编码;
    for (const auto& [键, 编码] : 写入.新编码映射) {
        if (!有效(编码)) return std::nullopt;
        实际键.push_back(键.值);
        实际编码.push_back(编码.值);
        if (键.值 == 普通概念节点本地键值) {
            if (概念编码) return std::nullopt;
            概念编码 = 编码;
        }
    }
    std::sort(期望键.begin(), 期望键.end());
    std::sort(实际键.begin(), 实际键.end());
    std::sort(实际编码.begin(), 实际编码.end());
    if (实际键 != 期望键 || !概念编码
        || std::adjacent_find(实际编码.begin(), 实际编码.end())
            != 实际编码.end()) return std::nullopt;
    return 概念编码;
}

// 诊断责任：无适用错误分支；形成可由 owner 域原子发布的完整普通概念写集。
L1所有者范围写集请求 形成普通概念写集(
    const L2普通概念建立请求& 请求,
    const 概念身份来源定位& 来源定位,
    const 概念本体结构定位& 本体定位,
    const 普通概念结构定位& 普通定位) {
    const auto 本地 = [](std::uint32_t 分区, std::size_t 索引) {
        return L1所有者范围写集本地键{
            static_cast<std::uint32_t>(分区 + static_cast<std::uint32_t>(索引))};
    };
    L1所有者范围写集请求 写集;
    写集.合同版本 = L1所有者范围CRUD合同版本;
    写集.期望事实代次 = 请求.请求头.期望事实代次;
    写集.写入幂等身份 = 映射概念幂等身份(请求.幂等身份);
    const L1所有者范围写集本地键 概念键{普通概念节点本地键值};
    写集.节点.push_back({概念键, 节点种类::普通, std::nullopt});
    写集.关系 = {
        {{普通概念族归属关系本地键值}, 概念键, 来源定位.族锚点,
            来源定位.族归属关系类型, 概念族归属角色值},
        {{普通概念本体根归属关系本地键值}, 概念键, 请求.本体根.值,
            普通定位.本体根归属关系类型, 1},
        {{普通概念签名规则关系本地键值}, 概念键,
            请求.签名规则材料.值, 普通定位.签名规则关系类型, 1}};
    写集.值 = {
        {{普通概念首次请求见证值本地键值}, 概念键,
            普通定位.首次见证属性类型,
            std::vector<std::uint64_t>{请求.幂等身份.值,
                请求.请求头.期望事实代次, L2普通概念结构合同版本},
            概念键},
        {{普通概念治理生命周期值本地键值}, 概念键,
            普通定位.治理生命周期属性类型,
            static_cast<std::int64_t>(L2概念治理生命周期状态::活跃),
            概念键}};
    写集.属性槽变更 = {
        {概念键, 普通定位.首次见证属性类型,
            {普通概念首次请求见证值本地键值}},
        {概念键, 普通定位.治理生命周期属性类型,
            {普通概念治理生命周期值本地键值}}};

    for (std::size_t 索引 = 0; 索引 < 请求.直接上位.size(); ++索引)
        写集.关系.push_back({本地(0x1000'0000U, 索引),
            请求.直接上位[索引].值, 概念键, 本体定位.直接上位关系类型,
            static_cast<std::int64_t>(索引 + 1)});
    for (std::size_t 索引 = 0; 索引 < 请求.来源证据材料.size(); ++索引)
        写集.关系.push_back({本地(0x2000'0000U, 索引), 概念键,
            请求.来源证据材料[索引].值, 普通定位.来源证据关系类型,
            static_cast<std::int64_t>(索引 + 1)});

    for (std::size_t 索引 = 0; 索引 < 请求.签名值式项.size(); ++索引) {
        const auto& 项 = 请求.签名值式项[索引];
        const auto 节点键 = 本地(0x3000'0000U, 索引);
        const auto 关系键 = 本地(0x4000'0000U, 索引);
        const auto 键值键 = 本地(0x5000'0000U, 索引);
        const auto 材料值键 = 本地(0x6000'0000U, 索引);
        写集.节点.push_back({节点键, 节点种类::普通, std::nullopt});
        写集.关系.push_back({关系键, 概念键, 节点键,
            普通定位.签名值式项成员关系类型,
            static_cast<std::int64_t>(索引 + 1)});
        写集.值.push_back({键值键, 节点键, 普通定位.项键属性类型,
            std::vector<std::uint64_t>{项.角色, 项.顺序}, 概念键});
        const auto [属性类型, 来源] = std::visit([&](const auto& 值) {
            using 类型 = std::decay_t<decltype(值)>;
            if constexpr (std::is_same_v<类型, std::int64_t>)
                return std::pair{普通定位.签名I64材料属性类型,
                    L1所有者范围事实引用{概念键}};
            else if constexpr (std::is_same_v<类型, std::vector<std::int64_t>>)
                return std::pair{普通定位.签名I64组材料属性类型,
                    L1所有者范围事实引用{概念键}};
            else if constexpr (std::is_same_v<类型, std::vector<std::uint64_t>>)
                return std::pair{普通定位.签名U64组材料属性类型,
                    L1所有者范围事实引用{概念键}};
            else return std::pair{普通定位.签名独立材料引用属性类型,
                L1所有者范围事实引用{值.编码}};
        }, 项.值);
        写集.值.push_back({材料值键, 节点键, 属性类型,
            映射普通概念材料到L1(项.值), 来源});
        写集.属性槽变更.push_back({节点键, 普通定位.项键属性类型, 键值键});
        写集.属性槽变更.push_back({节点键, 属性类型, 材料值键});
    }
    for (std::size_t 索引 = 0; 索引 < 请求.初始定义.size(); ++索引) {
        const auto& 项 = 请求.初始定义[索引];
        const auto 节点键 = 本地(0x7000'0000U, 索引);
        const auto 关系键 = 本地(0x8000'0000U, 索引);
        const auto 键值键 = 本地(0x9000'0000U, 索引);
        const auto 目标值键 = 本地(0xA000'0000U, 索引);
        写集.节点.push_back({节点键, 节点种类::普通, std::nullopt});
        写集.关系.push_back({关系键, 概念键, 节点键,
            普通定位.定义项成员关系类型,
            static_cast<std::int64_t>(索引 + 1)});
        写集.值.push_back({键值键, 节点键, 普通定位.项键属性类型,
            std::vector<std::uint64_t>{项.角色, 项.顺序}, 概念键});
        写集.值.push_back({目标值键, 节点键,
            普通定位.定义目标编码属性类型, 编码概念定义目标(项.目标), 概念键});
        写集.属性槽变更.push_back({节点键, 普通定位.项键属性类型, 键值键});
        写集.属性槽变更.push_back({节点键,
            普通定位.定义目标编码属性类型, 目标值键});
    }
    return 写集;
}

struct 普通概念重建结果 final {
    L2结构状态 状态 = L2结构状态::内部不一致;
    std::uint64_t 截止 = 0;
    std::optional<L2普通概念事实> 事实;
    std::optional<L2普通概念建立请求> 首次请求;
};

// 诊断责任：无适用错误分支；只校验历史组读头与调用时许可代次。
bool 历史关系组头完整(const L1所有者范围历史关系组读取结果& 读取,
    L1所有者范围关系端点方向 方向, 稳定编码 端点, 稳定编码 类型,
    std::uint64_t 历史截止, std::uint64_t 调用代次) noexcept {
    return 读取.状态 == L1所有者范围读取状态::成功
        && 读取.合同版本 == L1所有者范围CRUD合同版本
        && 读取.方向 == 方向 && 读取.端点节点 == 端点
        && 读取.关系类型节点 == 类型
        && 读取.历史截止事实代次 == 历史截止
        && 读取.读取事实代次 == 调用代次;
}

// 诊断责任：无适用错误分支；从 owner 历史投影重建一份普通概念自有结构。
普通概念重建结果 重建普通概念自有事实(
    const L1事实基座服务& 第一层服务,
    const 概念身份来源定位& 来源定位,
    const 概念本体结构定位& 本体定位,
    const 普通概念结构定位& 普通定位,
    const L2普通概念读取请求& 请求) {
    const auto 调用代次 = 请求.请求头.期望事实代次;
    const auto 投影截止 = 请求.读取类别 == L2读取类别::历史
        ? 请求.历史截止事实代次 : 调用代次;
    const auto 投影退出 = [投影截止](std::optional<std::uint64_t> 退出) {
        return 退出 && *退出 <= 投影截止 ? 退出 : std::nullopt;
    };
    const auto 投影生命周期 = [&](std::uint64_t 创建,
        std::optional<std::uint64_t> 退出) {
        return L2生命周期{创建, 投影退出(退出)};
    };
    const auto 节点读 = 第一层服务.读取所有者范围历史事实(
        {L1所有者范围CRUD合同版本, 请求.概念.值});
    if (节点读.状态 != L1所有者范围读取状态::成功)
        return {映射历史读取状态(节点读.状态), 投影截止};
    if (节点读.读取事实代次 != 调用代次)
        return {L2结构状态::事实代次漂移, 节点读.读取事实代次};
    const auto* 概念节点 = 节点读.事实
        ? std::get_if<L1所有者范围节点事实>(&*节点读.事实) : nullptr;
    if (节点读.合同版本 != L1所有者范围CRUD合同版本
        || 节点读.查询编码 != 请求.概念.值
        || !概念节点
        || 概念节点->编码 != 请求.概念.值
        || 概念节点->写入所有者 != 来源定位.所有者
        || 概念节点->种类 != 节点种类::普通 || 概念节点->属性类型表示)
        return {L2结构状态::内部不一致, 投影截止};
    if (概念节点->创建事实代次 > 投影截止)
        return {L2结构状态::未找到, 投影截止};
    if (请求.读取类别 == L2读取类别::当前
        && 概念节点->退出事实代次
        && *概念节点->退出事实代次 <= 投影截止)
        return {L2结构状态::已退出, 投影截止};
    const auto 结构截止 = 概念节点->退出事实代次
            && *概念节点->退出事实代次 <= 投影截止
        ? *概念节点->退出事实代次 - 1 : 投影截止;

    const auto 读关系组 = [&](L1所有者范围关系端点方向 方向,
        稳定编码 端点, 稳定编码 类型) {
        return 第一层服务.读取所有者范围历史关系组(
            {L1所有者范围CRUD合同版本, 方向, 端点, 类型, 结构截止});
    };
    const auto 族组 = 读关系组(L1所有者范围关系端点方向::源,
        请求.概念.值, 来源定位.族归属关系类型);
    const auto 根组 = 读关系组(L1所有者范围关系端点方向::源,
        请求.概念.值, 普通定位.本体根归属关系类型);
    const auto 规则组 = 读关系组(L1所有者范围关系端点方向::源,
        请求.概念.值, 普通定位.签名规则关系类型);
    const auto 签名组 = 读关系组(L1所有者范围关系端点方向::源,
        请求.概念.值, 普通定位.签名值式项成员关系类型);
    const auto 定义组 = 读关系组(L1所有者范围关系端点方向::源,
        请求.概念.值, 普通定位.定义项成员关系类型);
    const auto 证据组 = 读关系组(L1所有者范围关系端点方向::源,
        请求.概念.值, 普通定位.来源证据关系类型);
    const auto 上位组 = 读关系组(L1所有者范围关系端点方向::目标,
        请求.概念.值, 本体定位.直接上位关系类型);
    for (const auto* 组 : {&族组, &根组, &规则组, &签名组,
            &定义组, &证据组, &上位组}) {
        if (组->状态 != L1所有者范围读取状态::成功)
            return {映射历史读取状态(组->状态), 投影截止};
        if (组->读取事实代次 != 调用代次)
            return {L2结构状态::事实代次漂移, 组->读取事实代次};
    }
    if (!历史关系组头完整(族组, L1所有者范围关系端点方向::源,
            请求.概念.值, 来源定位.族归属关系类型, 结构截止, 调用代次)
        || !历史关系组头完整(根组, L1所有者范围关系端点方向::源,
            请求.概念.值, 普通定位.本体根归属关系类型, 结构截止, 调用代次)
        || !历史关系组头完整(规则组, L1所有者范围关系端点方向::源,
            请求.概念.值, 普通定位.签名规则关系类型, 结构截止, 调用代次)
        || !历史关系组头完整(签名组, L1所有者范围关系端点方向::源,
            请求.概念.值, 普通定位.签名值式项成员关系类型, 结构截止, 调用代次)
        || !历史关系组头完整(定义组, L1所有者范围关系端点方向::源,
            请求.概念.值, 普通定位.定义项成员关系类型, 结构截止, 调用代次)
        || !历史关系组头完整(证据组, L1所有者范围关系端点方向::源,
            请求.概念.值, 普通定位.来源证据关系类型, 结构截止, 调用代次)
        || !历史关系组头完整(上位组, L1所有者范围关系端点方向::目标,
            请求.概念.值, 本体定位.直接上位关系类型, 结构截止, 调用代次)
        )
        return {L2结构状态::内部不一致, 投影截止};
    if (根组.关系组.empty())
        return {L2结构状态::未找到, 投影截止};
    if (族组.关系组.size() != 1 || 根组.关系组.size() != 1
        || 规则组.关系组.size() != 1 || 上位组.关系组.empty()
        || 证据组.关系组.empty())
        return {L2结构状态::内部不一致, 投影截止};

    const auto 关系完整 = [&](const L1所有者范围关系事实& 关系,
        稳定编码 源, 稳定编码 目标, 稳定编码 类型) {
        return 关系.写入所有者 == 来源定位.所有者
            && 关系.源节点 == 源 && 关系.目标节点 == 目标
            && 关系.关系类型节点 == 类型 && 关系.角色或顺序 > 0
            && 关系.创建事实代次 <= 结构截止
            && (!关系.退出事实代次 || *关系.退出事实代次 > 结构截止);
    };
    const auto& 族关系 = 族组.关系组.front();
    const auto& 根关系 = 根组.关系组.front();
    const auto& 规则关系 = 规则组.关系组.front();
    if (!关系完整(族关系, 请求.概念.值, 来源定位.族锚点,
            来源定位.族归属关系类型)
        || 族关系.角色或顺序 != 概念族归属角色值
        || 族关系.创建事实代次 != 概念节点->创建事实代次
        || !关系完整(根关系, 请求.概念.值, 根关系.目标节点,
            普通定位.本体根归属关系类型)
        || 根关系.角色或顺序 != 1
        || 根关系.创建事实代次 != 概念节点->创建事实代次
        || !关系完整(规则关系, 请求.概念.值, 规则关系.目标节点,
            普通定位.签名规则关系类型)
        || 规则关系.角色或顺序 != 1
        || 规则关系.创建事实代次 != 概念节点->创建事实代次)
        return {L2结构状态::内部不一致, 投影截止};

    const auto 所有者读 = 第一层服务.读取历史结构所有者(
        {L1所有者范围CRUD合同版本, 来源定位.所有者});
    if (所有者读.状态 != L1所有者范围读取状态::成功)
        return {所有者读.状态 == L1所有者范围读取状态::资源失败
                ? L2结构状态::资源失败 : L2结构状态::内部不一致,
            所有者读.读取事实代次};
    if (所有者读.读取事实代次 != 调用代次)
        return {L2结构状态::事实代次漂移, 所有者读.读取事实代次};
    struct 私有节点读取结果 final {
        L2结构状态 状态 = L2结构状态::内部不一致;
        std::uint64_t 截止 = 0;
        std::optional<L1所有者范围节点事实> 事实;
    };
    const auto 读节点 = [&](稳定编码 编码) -> 私有节点读取结果 {
        const auto 读取 = 第一层服务.读取所有者范围历史事实(
            {L1所有者范围CRUD合同版本, 编码});
        const auto* 节点 = 读取.事实
            ? std::get_if<L1所有者范围节点事实>(&*读取.事实) : nullptr;
        if (读取.状态 != L1所有者范围读取状态::成功)
            return {读取.状态 == L1所有者范围读取状态::资源失败
                    ? L2结构状态::资源失败 : L2结构状态::内部不一致,
                读取.读取事实代次, std::nullopt};
        if (读取.读取事实代次 != 调用代次)
            return {L2结构状态::事实代次漂移,
                读取.读取事实代次, std::nullopt};
        if (读取.合同版本 != L1所有者范围CRUD合同版本
            || 读取.查询编码 != 编码 || !节点 || 节点->编码 != 编码)
            return {L2结构状态::内部不一致,
                读取.读取事实代次, std::nullopt};
        return {L2结构状态::已读取, 读取.读取事实代次, *节点};
    };
    const auto 族锚点读 = 读节点(来源定位.族锚点);
    const auto 族类型读 = 读节点(来源定位.族归属关系类型);
    if (族锚点读.状态 != L2结构状态::已读取)
        return {族锚点读.状态, 族锚点读.截止};
    if (族类型读.状态 != L2结构状态::已读取)
        return {族类型读.状态, 族类型读.截止};
    const auto& 族锚点 = *族锚点读.事实;
    const auto& 族类型 = *族类型读.事实;
    if (所有者读.合同版本 != L1所有者范围CRUD合同版本
        || 所有者读.查询所有者 != 来源定位.所有者
        || !所有者读.所有者事实
        || 所有者读.所有者事实->所有者 != 来源定位.所有者
        || 所有者读.所有者事实->范围种类
            != L1所有者范围种类::独占结构范围
        || 所有者读.所有者事实->创建事实代次 > 投影截止
        || (所有者读.所有者事实->退出事实代次
            && *所有者读.所有者事实->退出事实代次 <= 投影截止)
        || 族锚点.编码 != 来源定位.族锚点
        || 族锚点.写入所有者 != 来源定位.所有者
        || 族锚点.种类 != 节点种类::普通 || 族锚点.属性类型表示
        || 族锚点.退出事实代次
        || 族类型.编码 != 来源定位.族归属关系类型
        || 族类型.写入所有者 != 来源定位.所有者
        || 族类型.种类 != 节点种类::普通 || 族类型.属性类型表示
        || 族类型.退出事实代次)
        return {L2结构状态::内部不一致, 投影截止};

    const auto 属性组 = 第一层服务.读取所有者范围历史属性值组(
        {L1所有者范围CRUD合同版本, 请求.概念.值, 结构截止});
    if (属性组.状态 != L1所有者范围读取状态::成功)
        return {属性组.状态 == L1所有者范围读取状态::资源失败
                ? L2结构状态::资源失败 : L2结构状态::内部不一致,
            属性组.读取事实代次};
    if (属性组.读取事实代次 != 调用代次)
        return {L2结构状态::事实代次漂移, 属性组.读取事实代次};
    if (属性组.合同版本 != L1所有者范围CRUD合同版本
        || 属性组.所属节点 != 请求.概念.值
        || 属性组.历史截止事实代次 != 结构截止
        || 属性组.属性值组.size() != 2)
        return {L2结构状态::内部不一致, 投影截止};
    const L1所有者范围值事实* 见证值 = nullptr;
    const L1所有者范围值事实* 生命周期值 = nullptr;
    for (const auto& 值 : 属性组.属性值组) {
        if (值.属性类型节点 == 普通定位.首次见证属性类型) 见证值 = &值;
        else if (值.属性类型节点 == 普通定位.治理生命周期属性类型)
            生命周期值 = &值;
        else return {L2结构状态::内部不一致, 投影截止};
    }
    const auto* 见证 = 见证值
        ? std::get_if<std::vector<std::uint64_t>>(&见证值->材料) : nullptr;
    const auto* 生命周期材料 = 生命周期值
        ? std::get_if<std::int64_t>(&生命周期值->材料) : nullptr;
    const auto 值在结构截止完整 = [&](const L1所有者范围值事实& 值) {
        return 有效(值.编码) && 值.写入所有者 == 来源定位.所有者
            && 值.创建事实代次 != 0 && 值.创建事实代次 <= 结构截止
            && (!值.退出事实代次 || *值.退出事实代次 > 结构截止);
    };
    if (!见证值 || !生命周期值 || !见证 || 见证->size() != 3
        || (*见证)[0] == 0 || (*见证)[1] == 0
        || (*见证)[1] >= 概念节点->创建事实代次
        || (*见证)[2] != L2普通概念结构合同版本 || !生命周期材料
        || *生命周期材料 < 1 || *生命周期材料 > 3
        || !值在结构截止完整(*见证值) || !值在结构截止完整(*生命周期值)
        || 见证值->创建事实代次 != 概念节点->创建事实代次
        || 见证值->所属节点 != 请求.概念.值
        || 见证值->来源节点 != 请求.概念.值
        || 生命周期值->所属节点 != 请求.概念.值
        || 生命周期值->来源节点 != 请求.概念.值)
        return {L2结构状态::内部不一致, 投影截止};

    L2普通概念事实 事实;
    事实.概念 = 请求.概念;
    事实.身份来源 = {L2节点族::概念, 请求.概念.值, 来源定位.所有者,
        来源定位.族锚点, 来源定位.族归属关系类型, 族关系.编码,
        族关系.角色或顺序,
        投影生命周期(所有者读.所有者事实->创建事实代次,
            所有者读.所有者事实->退出事实代次),
        投影生命周期(概念节点->创建事实代次, 概念节点->退出事实代次),
        投影生命周期(族锚点.创建事实代次, 族锚点.退出事实代次),
        投影生命周期(族类型.创建事实代次, 族类型.退出事实代次),
        投影生命周期(族关系.创建事实代次, 族关系.退出事实代次)};
    事实.本体根归属 = {L2概念身份{根关系.目标节点}, 请求.概念,
        根关系.编码, 投影生命周期(
            根关系.创建事实代次, 根关系.退出事实代次)};
    事实.签名规则 = {请求.概念, 不可变材料身份{规则关系.目标节点},
        规则关系.编码, 投影生命周期(
            规则关系.创建事实代次, 规则关系.退出事实代次)};
    事实.治理生命周期 = {请求.概念,
        static_cast<L2概念治理生命周期状态>(*生命周期材料),
        生命周期值->编码, 投影生命周期(
            生命周期值->创建事实代次, 生命周期值->退出事实代次)};

    std::uint64_t 读取项失败截止 = 投影截止;
    const auto 读取项 = [&](const L1所有者范围关系事实& 成员,
        bool 是定义) -> L2结构状态 {
        const auto 类型 = 是定义 ? 普通定位.定义项成员关系类型
            : 普通定位.签名值式项成员关系类型;
        if (!关系完整(成员, 请求.概念.值, 成员.目标节点, 类型)
            || 成员.创建事实代次 != 概念节点->创建事实代次)
            return L2结构状态::内部不一致;
        const auto 项节点读 = 读节点(成员.目标节点);
        if (项节点读.状态 != L2结构状态::已读取) {
            读取项失败截止 = 项节点读.截止;
            return 项节点读.状态;
        }
        const auto& 项节点 = *项节点读.事实;
        if (项节点.写入所有者 != 来源定位.所有者
            || 项节点.种类 != 节点种类::普通 || 项节点.属性类型表示
            || 项节点.创建事实代次 != 成员.创建事实代次
            || (项节点.退出事实代次
                && *项节点.退出事实代次 <= 结构截止))
            return L2结构状态::内部不一致;
        const auto 项属性 = 第一层服务.读取所有者范围历史属性值组(
            {L1所有者范围CRUD合同版本, 成员.目标节点, 结构截止});
        if (项属性.状态 != L1所有者范围读取状态::成功) {
            读取项失败截止 = 项属性.读取事实代次;
            return 项属性.状态 == L1所有者范围读取状态::资源失败
                ? L2结构状态::资源失败 : L2结构状态::内部不一致;
        }
        if (项属性.读取事实代次 != 调用代次) {
            读取项失败截止 = 项属性.读取事实代次;
            return L2结构状态::事实代次漂移;
        }
        if (项属性.合同版本 != L1所有者范围CRUD合同版本
            || 项属性.所属节点 != 成员.目标节点
            || 项属性.历史截止事实代次 != 结构截止
            || 项属性.属性值组.size() != 2)
            return L2结构状态::内部不一致;
        const L1所有者范围值事实* 键值 = nullptr;
        const L1所有者范围值事实* 内容值 = nullptr;
        for (const auto& 值 : 项属性.属性值组) {
            if (值.属性类型节点 == 普通定位.项键属性类型) 键值 = &值;
            else 内容值 = &值;
        }
        const auto* 键 = 键值
            ? std::get_if<std::vector<std::uint64_t>>(&键值->材料) : nullptr;
        if (!键值 || !内容值 || !键 || 键->size() != 2
            || (*键)[0] == 0 || (*键)[1] == 0
            || !值在结构截止完整(*键值) || !值在结构截止完整(*内容值)
            || 键值->创建事实代次 != 成员.创建事实代次
            || 内容值->创建事实代次 != 成员.创建事实代次
            || 键值->所属节点 != 成员.目标节点
            || 键值->来源节点 != 请求.概念.值
            || 内容值->所属节点 != 成员.目标节点)
            return L2结构状态::内部不一致;
        if (是定义) {
            if (内容值->属性类型节点 != 普通定位.定义目标编码属性类型)
                return L2结构状态::内部不一致;
            const auto* 编码 = std::get_if<std::vector<std::uint64_t>>(&内容值->材料);
            const auto 目标 = 编码 ? 解码概念定义目标(*编码) : std::nullopt;
            if (!目标 || 内容值->来源节点 != 请求.概念.值)
                return L2结构状态::内部不一致;
            事实.定义关系.push_back({请求.概念, {(*键)[0], (*键)[1], *目标},
                成员.编码, 投影生命周期(
                    成员.创建事实代次, 成员.退出事实代次)});
        } else {
            const bool 类型匹配 = [&]() {
                if (内容值->属性类型节点 == 普通定位.签名I64材料属性类型)
                    return std::holds_alternative<std::int64_t>(内容值->材料)
                        && 内容值->来源节点 == 请求.概念.值;
                if (内容值->属性类型节点 == 普通定位.签名I64组材料属性类型)
                    return std::holds_alternative<std::vector<std::int64_t>>(
                            内容值->材料)
                        && 内容值->来源节点 == 请求.概念.值;
                if (内容值->属性类型节点 == 普通定位.签名U64组材料属性类型)
                    return std::holds_alternative<std::vector<std::uint64_t>>(
                            内容值->材料)
                        && 内容值->来源节点 == 请求.概念.值;
                if (内容值->属性类型节点
                        != 普通定位.签名独立材料引用属性类型)
                    return false;
                const auto* 引用 = std::get_if<L1所有者范围独立材料引用>(
                    &内容值->材料);
                return 引用 && 有效(引用->编码)
                    && 内容值->来源节点 == 引用->编码;
            }();
            const auto 材料 = 映射L1材料到普通概念(内容值->材料);
            if (!类型匹配 || !材料) return L2结构状态::内部不一致;
            事实.签名值式项.push_back({请求.概念,
                {(*键)[0], (*键)[1], *材料}, 内容值->编码,
                投影生命周期(成员.创建事实代次, 成员.退出事实代次)});
        }
        return L2结构状态::已读取;
    };
    for (const auto& 成员 : 签名组.关系组) {
        const auto 状态 = 读取项(成员, false);
        if (状态 != L2结构状态::已读取) return {状态, 读取项失败截止};
    }
    for (const auto& 成员 : 定义组.关系组) {
        const auto 状态 = 读取项(成员, true);
        if (状态 != L2结构状态::已读取) return {状态, 读取项失败截止};
    }
    for (const auto& 关系 : 上位组.关系组) {
        if (!关系完整(关系, 关系.源节点, 请求.概念.值,
                本体定位.直接上位关系类型))
            return {L2结构状态::内部不一致, 投影截止};
        const auto 投影退出 = 关系.退出事实代次
                && *关系.退出事实代次 <= 投影截止
            ? 关系.退出事实代次 : std::nullopt;
        事实.直接上位.push_back({L2概念身份{关系.源节点}, 请求.概念,
            关系.编码, 投影生命周期(
                关系.创建事实代次, 投影退出)});
    }
    for (const auto& 关系 : 证据组.关系组) {
        if (!关系完整(关系, 请求.概念.值, 关系.目标节点,
                普通定位.来源证据关系类型)
            || 关系.创建事实代次 != 概念节点->创建事实代次)
            return {L2结构状态::内部不一致, 投影截止};
        事实.来源证据.push_back({请求.概念,
            不可变材料身份{关系.目标节点}, 关系.编码,
            投影生命周期(关系.创建事实代次, 关系.退出事实代次)});
    }
    const auto 键小于 = [](const auto& 左, const auto& 右) {
        return 左.项.角色 < 右.项.角色
            || (左.项.角色 == 右.项.角色 && 左.项.顺序 < 右.项.顺序);
    };
    std::sort(事实.签名值式项.begin(), 事实.签名值式项.end(), 键小于);
    std::sort(事实.定义关系.begin(), 事实.定义关系.end(), 键小于);
    std::sort(事实.直接上位.begin(), 事实.直接上位.end(), [](const auto& 左,
        const auto& 右) { return 左.上位概念.值.值 < 右.上位概念.值.值; });
    std::sort(事实.来源证据.begin(), 事实.来源证据.end(), [](const auto& 左,
        const auto& 右) { return 左.证据材料.值.值 < 右.证据材料.值.值; });
    if (!L2普通概念事实完整(事实, 投影截止))
        return {L2结构状态::内部不一致, 投影截止};
    if (投影截止 != 概念节点->创建事实代次) {
        const auto 首次重建 = 重建普通概念自有事实(第一层服务,
            来源定位, 本体定位, 普通定位,
            {{请求.请求头.合同版本, 请求.请求头.期望事实代次},
                请求.普通概念合同版本, L2读取类别::历史,
                请求.概念, 概念节点->创建事实代次});
        if (首次重建.状态 != L2结构状态::已读取
            || !首次重建.首次请求 || !首次重建.事实
            || 首次重建.截止 != 概念节点->创建事实代次)
            return {首次重建.状态 == L2结构状态::已读取
                    ? L2结构状态::内部不一致 : 首次重建.状态,
                首次重建.截止};
        return {L2结构状态::已读取, 投影截止, std::move(事实),
            *首次重建.首次请求};
    }
    L2普通概念建立请求 首次{{L2结构合同版本, (*见证)[1]},
        static_cast<std::uint32_t>((*见证)[2]), {(*见证)[0]},
        事实.本体根归属.本体根, 事实.签名规则.规则材料};
    for (const auto& 项 : 事实.签名值式项) 首次.签名值式项.push_back(项.项);
    for (const auto& 项 : 事实.定义关系) 首次.初始定义.push_back(项.项);
    for (const auto& 项 : 事实.直接上位) 首次.直接上位.push_back(项.上位概念);
    for (const auto& 项 : 事实.来源证据) 首次.来源证据材料.push_back(项.证据材料);
    return {L2结构状态::已读取, 投影截止, std::move(事实), std::move(首次)};
}

// 诊断责任：无适用错误分支；生命周期写失败固定为空负载和零变更。
L2概念治理生命周期写入结果 形成概念生命周期写入失败(
    L2结构状态 状态, std::uint64_t 截止 = 0) noexcept {
    if (状态 == L2结构状态::已提交 || 状态 == L2结构状态::精确重复
        || 状态 == L2结构状态::已读取) 状态 = L2结构状态::内部不一致;
    return {{L2结构合同版本, 状态, 截止, std::nullopt},
        std::nullopt, std::nullopt};
}

// 诊断责任：无适用错误分支；读取失败保留读取类别但不泄漏部分生命周期。
L2概念治理生命周期读取结果 形成概念生命周期读取失败(
    const L2概念治理生命周期读取请求& 请求,
    L2结构状态 状态, std::uint64_t 截止 = 0) noexcept {
    if (状态 == L2结构状态::已提交 || 状态 == L2结构状态::精确重复
        || 状态 == L2结构状态::已读取) 状态 = L2结构状态::内部不一致;
    return {{L2结构合同版本, 状态, 截止, std::nullopt}, 请求.读取类别,
        请求.读取类别 == L2读取类别::历史
            ? 请求.历史截止事实代次 : 0,
        std::nullopt};
}

// 诊断责任：无适用错误分支；规范化写集只建新值、退旧值并切换同一属性槽。
L1所有者范围写集请求 形成概念治理生命周期迁移写集(
    const L2概念治理生命周期迁移请求& 请求,
    const 普通概念结构定位& 普通定位) {
    L1所有者范围写集请求 写集;
    写集.合同版本 = L1所有者范围CRUD合同版本;
    写集.期望事实代次 = 请求.请求头.期望事实代次;
    写集.写入幂等身份 = 映射概念幂等身份(请求.幂等身份);
    const L1所有者范围写集本地键 新值键{
        概念治理生命周期迁移值本地键值};
    写集.值.push_back({新值键, 请求.概念.值,
        普通定位.治理生命周期属性类型,
        static_cast<std::int64_t>(请求.目标状态), 请求.概念.值});
    写集.属性槽变更.push_back({请求.概念.值,
        普通定位.治理生命周期属性类型, 新值键});
    写集.退出事实.push_back(请求.当前生命周期事实稳定编码);
    return 写集;
}

struct 概念生命周期值读取值 final {
    L2结构状态 状态 = L2结构状态::内部不一致;
    std::uint64_t 截止 = 0;
    std::optional<L1所有者范围值事实> 值;
};

// 诊断责任：无适用错误分支；按稳定编码读取概念 owner 自有的治理生命周期值。
概念生命周期值读取值 读取概念治理生命周期值事实(
    const L1事实基座服务& 第一层服务,
    const 概念身份来源定位& 来源定位,
    const 普通概念结构定位& 普通定位,
    L2概念身份 概念, 稳定编码 值编码,
    std::uint64_t 调用代次) noexcept {
    const auto 读取 = 第一层服务.读取所有者范围历史事实(
        {L1所有者范围CRUD合同版本, 值编码});
    if (读取.状态 != L1所有者范围读取状态::成功)
        return {映射历史读取状态(读取.状态), 读取.读取事实代次};
    if (读取.读取事实代次 != 调用代次)
        return {L2结构状态::事实代次漂移, 读取.读取事实代次};
    const auto* 值 = 读取.事实
        ? std::get_if<L1所有者范围值事实>(&*读取.事实) : nullptr;
    const auto* 状态材料 = 值
        ? std::get_if<std::int64_t>(&值->材料) : nullptr;
    if (读取.合同版本 != L1所有者范围CRUD合同版本
        || 读取.查询编码 != 值编码 || !值 || 值->编码 != 值编码
        || 值->所属节点 != 概念.值
        || 值->属性类型节点 != 普通定位.治理生命周期属性类型
        || 值->来源节点 != 概念.值 || 值->写入所有者 != 来源定位.所有者
        || 值->创建事实代次 == 0 || !状态材料
        || *状态材料 < static_cast<std::int64_t>(L2概念治理生命周期状态::活跃)
        || *状态材料 > static_cast<std::int64_t>(L2概念治理生命周期状态::退役))
        return {L2结构状态::内部不一致, 读取.读取事实代次};
    return {L2结构状态::已读取, 读取.读取事实代次, *值};
}

// 诊断责任：无适用错误分支；首次写入只允许新生命周期值一个本地编码映射。
std::optional<稳定编码> 读取概念治理生命周期新值映射(
    const L1所有者范围写入结果& 首次结果) noexcept {
    if (首次结果.新编码映射.size() != 1
        || 首次结果.新编码映射.front().first
            != L1所有者范围写集本地键{概念治理生命周期迁移值本地键值}
        || !有效(首次结果.新编码映射.front().second)) return std::nullopt;
    return 首次结果.新编码映射.front().second;
}

// 诊断责任：无适用错误分支；在首次截止重建同一次迁移的新旧生命周期事实。
L2概念治理生命周期写入结果 重建概念治理生命周期首次结果(
    const L1事实基座服务& 第一层服务,
    const 概念身份来源定位& 来源定位,
    const 概念本体结构定位& 本体定位,
    const 普通概念结构定位& 普通定位,
    const L1所有者范围写集请求& 写集,
    const L1所有者范围写入结果& 首次结果,
    L2结构状态 返回状态,
    const L2概念治理生命周期迁移请求& 请求,
    std::uint64_t 调用代次) noexcept {
    const auto 截止 = 首次结果.事实代次;
    const bool 首次完整 = 首次结果.状态 == L1所有者范围写入状态::成功
        && 首次结果.合同版本 == L1所有者范围CRUD合同版本
        && 首次结果.所有者 == 来源定位.所有者
        && 首次结果.写入幂等身份 == 写集.写入幂等身份
        && 截止 != 0 && 截止 > 请求.请求头.期望事实代次
        && 截止 <= 调用代次 && 首次结果.是否形成内存权威发布
        && 首次结果.重试边界 == L1所有者范围重试边界::不适用;
    const auto 新值编码 = 读取概念治理生命周期新值映射(首次结果);
    if (!首次完整 || !新值编码
        || *新值编码 == 请求.当前生命周期事实稳定编码)
        return 形成概念生命周期写入失败(
            L2结构状态::内部不一致, 截止);

    const auto 新值 = 读取概念治理生命周期值事实(第一层服务,
        来源定位, 普通定位, 请求.概念, *新值编码, 调用代次);
    const auto 旧值 = 读取概念治理生命周期值事实(第一层服务,
        来源定位, 普通定位, 请求.概念,
        请求.当前生命周期事实稳定编码, 调用代次);
    if (新值.状态 != L2结构状态::已读取 || !新值.值
        || 旧值.状态 != L2结构状态::已读取 || !旧值.值) {
        const auto 失败状态 = 新值.状态 == L2结构状态::资源失败
                || 旧值.状态 == L2结构状态::资源失败
            ? L2结构状态::资源失败 : L2结构状态::内部不一致;
        return 形成概念生命周期写入失败(失败状态,
            新值.状态 != L2结构状态::已读取 ? 新值.截止 : 旧值.截止);
    }
    const auto* 新状态 = std::get_if<std::int64_t>(&新值.值->材料);
    const auto* 旧状态 = std::get_if<std::int64_t>(&旧值.值->材料);
    if (!新状态 || !旧状态
        || *新状态 != static_cast<std::int64_t>(请求.目标状态)
        || *旧状态 != static_cast<std::int64_t>(请求.当前状态)
        || 新值.值->创建事实代次 != 截止 || 新值.值->退出事实代次
        || 旧值.值->创建事实代次 >= 截止
        || 旧值.值->退出事实代次 != 截止)
        return 形成概念生命周期写入失败(
            L2结构状态::内部不一致, 截止);

    const auto 概念重建 = 重建普通概念自有事实(第一层服务,
        来源定位, 本体定位, 普通定位,
        {{L2结构合同版本, 调用代次}, L2普通概念结构合同版本,
            L2读取类别::历史, 请求.概念, 截止});
    if (概念重建.状态 != L2结构状态::已读取 || !概念重建.事实
        || 概念重建.事实->治理生命周期.事实稳定编码 != *新值编码
        || 概念重建.事实->治理生命周期.状态 != 请求.目标状态
        || 概念重建.事实->治理生命周期.生命周期.创建事实代次 != 截止
        || 概念重建.事实->治理生命周期.生命周期.退出事实代次)
        return 形成概念生命周期写入失败(
            概念重建.状态 == L2结构状态::资源失败
                ? L2结构状态::资源失败 : L2结构状态::内部不一致,
            概念重建.截止);

    L2概念治理生命周期事实 已退出{请求.概念, 请求.当前状态,
        请求.当前生命周期事实稳定编码,
        {旧值.值->创建事实代次, 旧值.值->退出事实代次}};
    L2概念治理生命周期写入结果 结果{{L2结构合同版本, 返回状态,
        截止, 截止}, 概念重建.事实->治理生命周期, std::move(已退出)};
    return 结果.成功() ? 结果
        : 形成概念生命周期写入失败(
            L2结构状态::内部不一致, 截止);
}

struct 普通概念公开依赖 final {
    const 不可变材料服务* 材料 = nullptr;
    const L2场景结构服务* 场景 = nullptr;
    const L2存在结构服务* 存在 = nullptr;
    const L2特征结构服务* 特征 = nullptr;
    const L2状态结构服务* 状态 = nullptr;
    const L2动态结构服务* 动态 = nullptr;
    const L2因果结构服务* 因果 = nullptr;

    bool 完整() const noexcept {
        return 材料 && 场景 && 存在 && 特征 && 状态 && 动态 && 因果;
    }
};

// 诊断责任：无适用错误分支；所有跨 owner 引用只经具名公开服务在同一截止验证。
template <class 世界事实校验器>
L2结构状态 校验普通概念公开引用(
    const L1事实基座服务& 第一层服务,
    const 概念身份来源定位& 来源定位,
    const 概念本体结构定位& 本体定位,
    const 普通概念结构定位& 普通定位,
    const 普通概念公开依赖& 依赖,
    const 世界事实校验器& 校验世界事实,
    const L2普通概念事实& 事实,
    const L2普通概念读取请求& 请求) {
    if (!依赖.完整()) return L2结构状态::未实现;
    const auto 历史截止 = 请求.读取类别 == L2读取类别::历史
        ? 请求.历史截止事实代次 : 0;
    const auto 投影截止 = 历史截止 != 0
        ? 历史截止 : 请求.请求头.期望事实代次;
    const auto 结果成功 = [&](const L2结构结果头& 头) {
        return 头.合同版本 == L2结构合同版本
            && 头.状态 == L2结构状态::已读取
            && 头.事实截止代次 == 投影截止 && !头.变更事实代次;
    };
    const auto 读取头形状结论 = [&](const L2结构结果头& 头, bool 负载完整) {
        if (头.状态 != L2结构状态::已读取) return 头.状态;
        return 结果成功(头) && 负载完整
            ? L2结构状态::已读取 : L2结构状态::内部不一致;
    };
    const auto 完整读取形状结论 = [&](const auto& 读取, bool 负载完整) {
        if (读取.结果头.状态 != L2结构状态::已读取) return 读取.结果头.状态;
        return 结果成功(读取.结果头)
            && 读取.读取类别 == 请求.读取类别
            && 读取.历史截止事实代次 == 历史截止
            && 负载完整
            ? L2结构状态::已读取 : L2结构状态::内部不一致;
    };
    const auto 读材料 = [&](不可变材料身份 身份) -> L2结构状态 {
        const auto 结果 = 依赖.材料->读取不可变材料(
            {请求.请求头, 请求.读取类别, 身份, 历史截止});
        if (结果.结果头.状态 != L2结构状态::已读取)
            return 结果.结果头.状态;
        const bool 材料完整 = 结果.材料
            && 有效(结果.材料->身份.值) && 有效(结果.材料->格式身份.值)
            && (结果.材料->格式角色 == 不可变材料格式角色::严格UTF8
                || 结果.材料->格式角色 == 不可变材料格式角色::通用字节)
            && 结果.材料->字节长度 != 0 && 有效(结果.材料->来源节点)
            && 结果.材料->物理定位
            && !结果.材料->物理定位->摘要对象绝对路径.empty()
            && L2生命周期完整(结果.材料->生命周期)
            && 结果.材料->生命周期.创建事实代次 <= 投影截止
            && (!结果.材料->生命周期.退出事实代次
                || *结果.材料->生命周期.退出事实代次 <= 投影截止)
            && (请求.读取类别 == L2读取类别::历史
                || !结果.材料->生命周期.退出事实代次);
        if (!结果成功(结果.结果头) || 结果.读取类别 != 请求.读取类别
            || 结果.历史截止事实代次 != 历史截止 || !结果.材料
            || 结果.材料->身份 != 身份 || !材料完整)
            return L2结构状态::内部不一致;
        return L2结构状态::已读取;
    };
    auto 状态 = 读材料(事实.签名规则.规则材料);
    if (状态 != L2结构状态::已读取) return 状态;
    for (const auto& 证据 : 事实.来源证据) {
        状态 = 读材料(证据.证据材料);
        if (状态 != L2结构状态::已读取) return 状态;
    }
    for (const auto& 项 : 事实.签名值式项) {
        const auto* 引用 = std::get_if<L2独立材料引用>(&项.项.值);
        if (!引用) continue;
        状态 = 读材料(不可变材料身份{引用->编码});
        if (状态 != L2结构状态::已读取) return 状态;
    }

    const auto 根组 = 读取全部当前概念本体根核心(第一层服务,
        来源定位, 本体定位, {请求.请求头});
    if (!根组.成功()) return 根组.结果头.状态;
    const auto 是根 = [&](L2概念身份 身份) {
        return std::any_of(根组.本体根.begin(), 根组.本体根.end(),
            [&](const auto& 根) { return 根.根概念 == 身份
                && 根.根角色.生命周期.创建事实代次 <= 投影截止; });
    };
    if (!是根(事实.本体根归属.本体根)) return L2结构状态::引用冲突;
    for (const auto& 上位 : 事实.直接上位) {
        if (是根(上位.上位概念)) continue;
        const auto 父 = 重建普通概念自有事实(第一层服务, 来源定位,
            本体定位, 普通定位,
            {请求.请求头, L2普通概念结构合同版本, 请求.读取类别,
                上位.上位概念, 历史截止});
        if (父.状态 != L2结构状态::已读取) return 父.状态;
        if (!父.事实 || 父.事实->本体根归属.本体根
                != 事实.本体根归属.本体根)
            return L2结构状态::引用冲突;
    }

    const auto 读场景 = [&](L2场景身份 身份) {
        return 依赖.场景->读取完整场景(
            {请求.请求头, 请求.读取类别, 身份, 历史截止});
    };
    const auto 读存在 = [&](L2存在身份 身份) {
        return 依赖.存在->读取完整存在(
            {请求.请求头, 请求.读取类别, 身份, 历史截止});
    };
    const auto 读特征定义 = [&](L2特征定义身份 身份) {
        return 依赖.特征->读取完整统一特征定义(
            {请求.请求头, 请求.读取类别, 身份, 历史截止});
    };
    const auto 读特征实例 = [&](L2特征实例身份 身份) {
        return 依赖.特征->读取完整特征实例(
            {请求.请求头, 请求.读取类别, 身份, 历史截止});
    };
    const auto 读状态 = [&](L2状态身份 身份) {
        return 依赖.状态->读取完整状态(
            {请求.请求头, 请求.读取类别, 身份, 历史截止});
    };
    const auto 读动态 = [&](L2动态身份 身份) {
        return 依赖.动态->读取完整动态(
            {请求.请求头, 请求.读取类别, 身份, 历史截止});
    };
    const auto 读因果 = [&](L2因果身份 身份) {
        return 依赖.因果->读取完整因果(
            {请求.请求头, 请求.读取类别, 身份, 历史截止});
    };
    const auto 生命周期投影完整 = [&](const L2生命周期& 生命周期) {
        return L2生命周期完整(生命周期)
            && 生命周期.创建事实代次 <= 投影截止
            && (!生命周期.退出事实代次
                || *生命周期.退出事实代次 <= 投影截止)
            && (请求.读取类别 == L2读取类别::历史
                || !生命周期.退出事实代次);
    };
    const auto 状态公开形状完整 = [&](const L2状态事实& 状态事实,
        const L2状态自有关系引用& 关系) {
        return 有效(状态事实.身份.值) && 有效(状态事实.主体存在.值)
            && 有效(状态事实.特征实例.值) && 有效(状态事实.来源稳定编码)
            && 生命周期投影完整(状态事实.生命周期)
            && L2属性事实截止投影完整(状态事实.精确值, 投影截止)
            && L2属性事实截止投影完整(状态事实.时间值, 投影截止)
            && (请求.读取类别 == L2读取类别::历史
                || (!状态事实.精确值.退出事实代次
                    && !状态事实.时间值.退出事实代次))
            && 状态事实.精确值.值稳定编码 != 状态事实.时间值.值稳定编码
            && 状态事实.精确值.来源稳定编码 == 状态事实.来源稳定编码
            && 状态事实.时间值.来源稳定编码 == 状态事实.来源稳定编码
            && 有效(关系.族归属关系稳定编码)
            && 有效(关系.主体关系稳定编码)
            && 有效(关系.特征实例关系稳定编码)
            && 关系.族归属关系稳定编码 != 关系.主体关系稳定编码
            && 关系.族归属关系稳定编码 != 关系.特征实例关系稳定编码
            && 关系.主体关系稳定编码 != 关系.特征实例关系稳定编码;
    };
    const auto 动态公开形状完整 = [&](const L2动态事实& 动态事实,
        const L2动态自有关系引用& 关系) {
        return 有效(动态事实.身份.值) && 有效(动态事实.主体存在.值)
            && 有效(动态事实.前状态.值) && 有效(动态事实.后状态.值)
            && 有效(动态事实.来源稳定编码)
            && 生命周期投影完整(动态事实.生命周期)
            && L2属性事实截止投影完整(动态事实.起始时间值, 投影截止)
            && L2属性事实截止投影完整(动态事实.结束时间值, 投影截止)
            && (请求.读取类别 == L2读取类别::历史
                || (!动态事实.起始时间值.退出事实代次
                    && !动态事实.结束时间值.退出事实代次))
            && 动态事实.起始时间值.值稳定编码
                != 动态事实.结束时间值.值稳定编码
            && 动态事实.起始时间值.来源稳定编码 == 动态事实.来源稳定编码
            && 动态事实.结束时间值.来源稳定编码 == 动态事实.来源稳定编码
            && 有效(关系.族归属关系稳定编码)
            && 有效(关系.主体关系稳定编码)
            && 有效(关系.前状态关系稳定编码)
            && 有效(关系.后状态关系稳定编码)
            && 关系.族归属关系稳定编码 != 关系.主体关系稳定编码
            && 关系.族归属关系稳定编码 != 关系.前状态关系稳定编码
            && 关系.族归属关系稳定编码 != 关系.后状态关系稳定编码
            && 关系.主体关系稳定编码 != 关系.前状态关系稳定编码
            && 关系.主体关系稳定编码 != 关系.后状态关系稳定编码
            && 关系.前状态关系稳定编码 != 关系.后状态关系稳定编码;
    };
    for (const auto& 项 : 事实.定义关系) {
        状态 = std::visit([&](const auto& 目标) -> L2结构状态 {
            using 类型 = std::decay_t<decltype(目标)>;
            if constexpr (!std::is_same_v<类型, L2概念身份>)
                return 校验世界事实(L2世界事实引用{目标},
                    请求.读取类别, 历史截止,
                    请求.请求头.期望事实代次);
            if constexpr (std::is_same_v<类型, L2概念身份>) {
                if (是根(目标)) return L2结构状态::已读取;
                const auto 读取 = 重建普通概念自有事实(第一层服务,
                    来源定位, 本体定位, 普通定位,
                    {请求.请求头, L2普通概念结构合同版本,
                        请求.读取类别, 目标, 历史截止});
                return 读取.状态;
            } else if constexpr (std::is_same_v<类型, L2场景身份>) {
                const auto 读取 = 读场景(目标);
                return 完整读取形状结论(读取,
                    读取.场景 && 读取.场景->身份 == 目标
                        && L2场景事实完整(*读取.场景, 投影截止)
                        && 生命周期投影完整(读取.场景->生命周期));
            } else if constexpr (std::is_same_v<类型, L2存在身份>) {
                const auto 读取 = 读存在(目标);
                return 完整读取形状结论(读取,
                    读取.存在 && 读取.存在->身份 == 目标
                        && L2存在事实完整(*读取.存在, 投影截止)
                        && 生命周期投影完整(读取.存在->生命周期));
            } else if constexpr (std::is_same_v<类型, L2特征定义身份>) {
                const auto 读取 = 读特征定义(目标);
                return 完整读取形状结论(读取,
                    读取.特征定义 && 读取.特征定义->身份 == 目标
                        && L2统一特征定义事实完整(
                            *读取.特征定义, 投影截止)
                        && 生命周期投影完整(读取.特征定义->生命周期));
            } else if constexpr (std::is_same_v<类型, L2特征实例身份>) {
                const auto 读取 = 读特征实例(目标);
                return 完整读取形状结论(读取,
                    读取.特征实例 && 读取.特征实例->身份 == 目标
                        && 读取.自有关系
                        && L2特征实例事实完整(
                            *读取.特征实例, 投影截止)
                        && 生命周期投影完整(读取.特征实例->生命周期));
            } else if constexpr (std::is_same_v<类型, L2概念特征值引用>) {
                const auto 实例 = 读特征实例(目标.特征实例);
                const auto 实例结论 = 完整读取形状结论(实例,
                    实例.特征实例 && 实例.特征实例->身份 == 目标.特征实例);
                if (实例结论 != L2结构状态::已读取) return 实例结论;
                if (请求.读取类别 == L2读取类别::当前) {
                    const auto 值 = 依赖.特征->读取特征当前值(
                        {请求.请求头, 目标.特征实例});
                    return 读取头形状结论(值.结果头, 值.成功() && 值.当前值
                        && 值.当前值->特征实例 == 目标.特征实例
                        && 值.当前值->值稳定编码 == 目标.值稳定编码);
                }
                const auto 值 = 依赖.特征->读取特征历史值(
                    {请求.请求头, 目标.特征实例, 目标.值稳定编码});
                if (值.结果头.状态 != L2结构状态::已读取)
                    return 值.结果头.状态;
                return 值.结果头.合同版本 == L2结构合同版本
                    && 值.结果头.事实截止代次 == 请求.请求头.期望事实代次
                    && !值.结果头.变更事实代次 && 值.特征值
                    && 值.特征值->特征实例 == 目标.特征实例
                    && 值.特征值->值稳定编码 == 目标.值稳定编码
                    && 有效(值.特征值->来源稳定编码)
                    && L2特征当前值材料有效(值.特征值->类型化不可变材料)
                    && L2特征值事实截止投影完整(
                        *值.特征值, 投影截止)
                    ? L2结构状态::已读取 : L2结构状态::内部不一致;
            } else if constexpr (std::is_same_v<类型, L2状态身份>) {
                const auto 读取 = 读状态(目标);
                return 完整读取形状结论(读取,
                    读取.状态 && 读取.状态->身份 == 目标
                        && 读取.自有关系
                        && 状态公开形状完整(*读取.状态, *读取.自有关系));
            } else if constexpr (std::is_same_v<类型, L2动态身份>) {
                const auto 读取 = 读动态(目标);
                return 完整读取形状结论(读取,
                    读取.动态 && 读取.动态->身份 == 目标
                        && 读取.自有关系
                        && 动态公开形状完整(*读取.动态, *读取.自有关系));
            } else if constexpr (std::is_same_v<类型, L2因果身份>) {
                const auto 读取 = 读因果(目标);
                return 完整读取形状结论(读取,
                    读取.因果 && 读取.因果->身份 == 目标
                        && 读取.自有事实
                        && L2因果事实完整(*读取.因果, 投影截止)
                        && 生命周期投影完整(读取.因果->生命周期));
            } else {
                const auto& 引用 = 目标;
                switch (引用.类别) {
                case L2概念世界关系类别::场景父子: {
                    const auto 源 = std::get<L2场景身份>(引用.源);
                    const auto 目标场景 = std::get<L2场景身份>(引用.目标);
                    const auto 读取 = 读场景(源);
                    const auto 结论 = 完整读取形状结论(读取,
                        读取.场景 && 读取.场景->身份 == 源
                            && L2场景事实完整(*读取.场景, 投影截止)
                            && 生命周期投影完整(读取.场景->生命周期));
                    if (结论 != L2结构状态::已读取) return 结论;
                    const bool 命中 = std::any_of(读取.场景->直接子场景.begin(),
                        读取.场景->直接子场景.end(), [&](const auto& 关系) {
                            return 关系.关系稳定编码 == 引用.关系稳定编码
                                && 关系.父场景 == 源 && 关系.子场景 == 目标场景; });
                    return 命中 ? L2结构状态::已读取 : L2结构状态::引用冲突;
                }
                case L2概念世界关系类别::场景成员: {
                    const auto 源 = std::get<L2场景身份>(引用.源);
                    const auto 目标存在 = std::get<L2存在身份>(引用.目标);
                    const auto 读取 = 读场景(源);
                    const auto 结论 = 完整读取形状结论(读取,
                        读取.场景 && 读取.场景->身份 == 源
                            && L2场景事实完整(*读取.场景, 投影截止)
                            && 生命周期投影完整(读取.场景->生命周期));
                    if (结论 != L2结构状态::已读取) return 结论;
                    const bool 命中 = std::any_of(读取.场景->直接成员.begin(),
                        读取.场景->直接成员.end(), [&](const auto& 关系) {
                            return 关系.关系稳定编码 == 引用.关系稳定编码
                                && 关系.场景 == 源 && 关系.存在 == 目标存在; });
                    return 命中 ? L2结构状态::已读取 : L2结构状态::引用冲突;
                }
                case L2概念世界关系类别::场景宿主存在: {
                    const auto 源 = std::get<L2场景身份>(引用.源);
                    const auto 目标存在 = std::get<L2存在身份>(引用.目标);
                    const auto 读取 = 读场景(源);
                    const auto 结论 = 完整读取形状结论(读取,
                        读取.场景 && 读取.场景->身份 == 源
                            && L2场景事实完整(*读取.场景, 投影截止)
                            && 生命周期投影完整(读取.场景->生命周期));
                    if (结论 != L2结构状态::已读取) return 结论;
                    return 读取.场景->当前宿主关系
                        && 读取.场景->当前宿主关系->关系稳定编码
                            == 引用.关系稳定编码
                        && 读取.场景->当前宿主关系->场景 == 源
                        && 读取.场景->当前宿主关系->宿主存在 == 目标存在
                        ? L2结构状态::已读取 : L2结构状态::引用冲突;
                }
                case L2概念世界关系类别::特征宿主:
                case L2概念世界关系类别::特征定义: {
                    const auto 源 = std::get<L2特征实例身份>(引用.源);
                    const auto 读取 = 读特征实例(源);
                    const auto 结论 = 完整读取形状结论(读取,
                        读取.特征实例 && 读取.特征实例->身份 == 源
                            && 读取.自有关系
                            && L2特征实例事实完整(
                                *读取.特征实例, 投影截止)
                            && 生命周期投影完整(读取.特征实例->生命周期));
                    if (结论 != L2结构状态::已读取) return 结论;
                    const bool 宿主 = 引用.类别 == L2概念世界关系类别::特征宿主;
                    const bool 端点 = 宿主
                        ? 读取.特征实例->宿主存在 == std::get<L2存在身份>(引用.目标)
                        : 读取.特征实例->特征定义 == std::get<L2特征定义身份>(引用.目标);
                    const auto 编码 = 宿主 ? 读取.自有关系->宿主关系稳定编码
                        : 读取.自有关系->定义关系稳定编码;
                    return 端点 && 编码 == 引用.关系稳定编码
                        ? L2结构状态::已读取 : L2结构状态::引用冲突;
                }
                case L2概念世界关系类别::状态主体:
                case L2概念世界关系类别::状态特征实例: {
                    const auto 源 = std::get<L2状态身份>(引用.源);
                    const auto 读取 = 读状态(源);
                    const auto 结论 = 完整读取形状结论(读取,
                        读取.状态 && 读取.状态->身份 == 源
                            && 读取.自有关系
                            && 状态公开形状完整(*读取.状态, *读取.自有关系));
                    if (结论 != L2结构状态::已读取) return 结论;
                    const bool 主体 = 引用.类别 == L2概念世界关系类别::状态主体;
                    const bool 端点 = 主体
                        ? 读取.状态->主体存在 == std::get<L2存在身份>(引用.目标)
                        : 读取.状态->特征实例 == std::get<L2特征实例身份>(引用.目标);
                    const auto 编码 = 主体 ? 读取.自有关系->主体关系稳定编码
                        : 读取.自有关系->特征实例关系稳定编码;
                    return 端点 && 编码 == 引用.关系稳定编码
                        ? L2结构状态::已读取 : L2结构状态::引用冲突;
                }
                case L2概念世界关系类别::动态主体:
                case L2概念世界关系类别::动态前状态:
                case L2概念世界关系类别::动态后状态: {
                    const auto 源 = std::get<L2动态身份>(引用.源);
                    const auto 读取 = 读动态(源);
                    const auto 结论 = 完整读取形状结论(读取,
                        读取.动态 && 读取.动态->身份 == 源
                            && 读取.自有关系
                            && 动态公开形状完整(*读取.动态, *读取.自有关系));
                    if (结论 != L2结构状态::已读取) return 结论;
                    bool 端点 = false; 稳定编码 编码;
                    if (引用.类别 == L2概念世界关系类别::动态主体) {
                        端点 = 读取.动态->主体存在 == std::get<L2存在身份>(引用.目标);
                        编码 = 读取.自有关系->主体关系稳定编码;
                    } else if (引用.类别 == L2概念世界关系类别::动态前状态) {
                        端点 = 读取.动态->前状态 == std::get<L2状态身份>(引用.目标);
                        编码 = 读取.自有关系->前状态关系稳定编码;
                    } else {
                        端点 = 读取.动态->后状态 == std::get<L2状态身份>(引用.目标);
                        编码 = 读取.自有关系->后状态关系稳定编码;
                    }
                    return 端点 && 编码 == 引用.关系稳定编码
                        ? L2结构状态::已读取 : L2结构状态::引用冲突;
                }
                default: {
                    const auto 源 = std::get<L2因果身份>(引用.源);
                    const auto 读取 = 读因果(源);
                    const auto 结论 = 完整读取形状结论(读取,
                        读取.因果 && 读取.因果->身份 == 源
                            && 读取.自有事实
                            && L2因果事实完整(*读取.因果, 投影截止)
                            && 生命周期投影完整(读取.因果->生命周期));
                    if (结论 != L2结构状态::已读取) return 结论;
                    if (引用.类别 == L2概念世界关系类别::因果动作引用) {
                        const auto 目标动作 = std::get<L2概念动作引用>(引用.目标);
                        return 读取.因果->动作 && 读取.自有事实->动作关系稳定编码
                            && *读取.因果->动作 == 目标动作.值
                            && *读取.自有事实->动作关系稳定编码
                                == 引用.关系稳定编码
                            ? L2结构状态::已读取 : L2结构状态::引用冲突;
                    }
                    const auto 目标状态 = std::get<L2状态身份>(引用.目标);
                    const bool 条件 = 引用.类别
                        == L2概念世界关系类别::因果条件状态;
                    const auto& 端点组 = 条件 ? 读取.因果->条件状态
                        : 读取.因果->结果状态;
                    const auto& 编码组 = 条件 ? 读取.自有事实->条件关系稳定编码
                        : 读取.自有事实->结果关系稳定编码;
                    for (std::size_t i = 0; i < 端点组.size() && i < 编码组.size(); ++i)
                        if (端点组[i].目标 == 目标状态.值
                            && 编码组[i] == 引用.关系稳定编码)
                            return L2结构状态::已读取;
                    return L2结构状态::引用冲突;
                }
                }
            }
        }, 项.项.目标);
        if (状态 != L2结构状态::已读取) return 状态;
    }
    return L2结构状态::已读取;
}

struct 当前普通概念身份组 final {
    L2结构状态 状态 = L2结构状态::内部不一致;
    std::uint64_t 截止 = 0;
    std::vector<L2概念身份> 身份;
};

// 诊断责任：无适用错误分支；一次 L1 闭包许可只枚举当前根归属关系的源概念。
当前普通概念身份组 读取全部当前普通概念身份(
    const L1事实基座服务& 第一层服务,
    const 概念身份来源定位& 来源定位,
    const 普通概念结构定位& 普通定位,
    std::uint64_t 期望事实代次) {
    L1所有者范围一致关系类型闭包读取请求 请求;
    请求.合同版本 = L1所有者范围一致关系类型闭包读取合同版本;
    请求.期望事实代次 = 期望事实代次;
    请求.所有者 = {来源定位.所有者};
    请求.关系类型闭包 = {{普通定位.本体根归属关系类型,
        {}, {}, {}, {}, {}, {}}};
    const auto 读取 = 第一层服务.尝试读取所有者范围一致关系类型闭包投影(请求);
    if (读取.状态 != L1所有者范围一致当前读取状态::成功)
        return {映射一致读取状态(读取.状态), 读取.读取事实代次, {}};
    if (读取.合同版本 != L1所有者范围一致关系类型闭包读取合同版本
        || 读取.期望事实代次 != 期望事实代次
        || 读取.读取事实代次 != 期望事实代次
        || 读取.所有者.size() != 1 || !读取.节点.empty()
        || !读取.关系.empty() || !读取.值.empty() || !读取.属性值.empty()
        || !读取.源关系组.empty() || !读取.目标关系组.empty()
        || 读取.关系类型闭包.size() != 1)
        return {L2结构状态::内部不一致, 读取.读取事实代次, {}};
    const auto& 闭包 = 读取.关系类型闭包.front();
    if (闭包.入口关系类型节点 != 普通定位.本体根归属关系类型
        || 闭包.状态 != L1所有者范围一致当前读取项目状态::成功
        || !闭包.关系类型事实
        || 闭包.关系类型事实->编码 != 普通定位.本体根归属关系类型
        || 闭包.关系类型事实->写入所有者 != 来源定位.所有者
        || 闭包.关系类型事实->退出事实代次)
        return {L2结构状态::内部不一致, 读取.读取事实代次, {}};
    当前普通概念身份组 结果{L2结构状态::已读取, 读取.读取事实代次, {}};
    结果.身份.reserve(闭包.成员.size());
    for (const auto& 成员 : 闭包.成员) {
        if (成员.关系.写入所有者 != 来源定位.所有者
            || 成员.关系.关系类型节点 != 普通定位.本体根归属关系类型
            || 成员.关系.源节点 != 成员.源节点.编码
            || 成员.关系.目标节点 != 成员.目标节点.编码
            || 成员.关系.角色或顺序 != 1 || 成员.关系.退出事实代次
            || 成员.源节点.写入所有者 != 来源定位.所有者
            || 成员.源节点.种类 != 节点种类::普通
            || 成员.源节点.属性类型表示 || 成员.源节点.退出事实代次)
            return {L2结构状态::内部不一致, 读取.读取事实代次, {}};
        结果.身份.push_back(L2概念身份{成员.源节点.编码});
    }
    std::sort(结果.身份.begin(), 结果.身份.end(), [](const auto& 左,
        const auto& 右) { return 左.值.值 < 右.值.值; });
    if (std::adjacent_find(结果.身份.begin(), 结果.身份.end()) != 结果.身份.end())
        return {L2结构状态::内部不一致, 读取.读取事实代次, {}};
    return 结果;
}

// 诊断责任：无适用错误分支；概念唯一键不含父项与来源证据。
bool 普通概念签名相同(const L2普通概念建立请求& 左,
    const L2普通概念建立请求& 右) noexcept {
    return 左.本体根 == 右.本体根
        && 左.签名规则材料 == 右.签名规则材料
        && 左.签名值式项 == 右.签名值式项
        && 左.初始定义 == 右.初始定义;
}

// 诊断责任：无适用错误分支；形成一根固定、可重放的 owner-scoped 写集。
L1所有者范围写集请求 形成本体根写集(
    const L2概念本体根建立请求& 请求,
    const 概念身份来源定位& 来源定位,
    const 概念本体结构定位& 本体定位) {
    L1所有者范围写集请求 写集;
    写集.合同版本 = L1所有者范围CRUD合同版本;
    写集.期望事实代次 = 请求.请求头.期望事实代次;
    写集.写入幂等身份 = 映射概念幂等身份(请求.幂等身份);
    写集.节点 = {{{根概念节点本地键值}, 节点种类::普通, std::nullopt}};
    写集.关系 = {
        {{概念族归属关系本地键值}, L1所有者范围写集本地键{根概念节点本地键值},
            来源定位.族锚点, 来源定位.族归属关系类型,
            概念族归属角色值},
        {{本体根角色关系本地键值}, L1所有者范围写集本地键{根概念节点本地键值},
            本体定位.角色锚点, 本体定位.角色关系类型,
            static_cast<std::int64_t>(请求.角色)}};
    写集.值 = {{{首次请求见证值本地键值},
        L1所有者范围写集本地键{根概念节点本地键值},
        本体定位.首次见证属性类型,
        std::vector<std::uint64_t>{请求.幂等身份.值,
            请求.请求头.期望事实代次},
        L1所有者范围写集本地键{根概念节点本地键值}}};
    写集.属性槽变更 = {{L1所有者范围写集本地键{根概念节点本地键值},
        本体定位.首次见证属性类型,
        {首次请求见证值本地键值}}};
    return 写集;
}

// 诊断责任：无适用错误分支；写失败保持空根和零变更形状。
L2概念本体根写入结果 形成根写入失败(
    L2结构状态 状态, std::uint64_t 截止 = 0) noexcept {
    return {{L2结构合同版本, 状态, 截止, std::nullopt}, std::nullopt};
}

struct 根写入映射 final {
    稳定编码 根节点;
    稳定编码 族归属关系;
    稳定编码 角色关系;
    稳定编码 见证值;
};

// 诊断责任：向上送出；首次写入的四项映射必须完整、互异且零额外项。
std::optional<根写入映射> 读取根写入映射(
    const L1所有者范围写入结果& 写入) {
    if (写入.新编码映射.size() != 4) return std::nullopt;
    根写入映射 映射;
    for (const auto& [键, 编码] : 写入.新编码映射) {
        if (!有效(编码)) return std::nullopt;
        稳定编码* 目标 = nullptr;
        switch (键.值) {
        case 根概念节点本地键值: 目标 = &映射.根节点; break;
        case 概念族归属关系本地键值: 目标 = &映射.族归属关系; break;
        case 本体根角色关系本地键值: 目标 = &映射.角色关系; break;
        case 首次请求见证值本地键值: 目标 = &映射.见证值; break;
        default: return std::nullopt;
        }
        if (有效(*目标)) return std::nullopt;
        *目标 = 编码;
    }
    if (!有效(映射.根节点) || !有效(映射.族归属关系)
        || !有效(映射.角色关系) || !有效(映射.见证值))
        return std::nullopt;
    std::vector<std::uint64_t> 编码组{映射.根节点.值, 映射.族归属关系.值,
        映射.角色关系.值, 映射.见证值.值};
    std::sort(编码组.begin(), 编码组.end());
    if (std::adjacent_find(编码组.begin(), 编码组.end()) != 编码组.end())
        return std::nullopt;
    return 映射;
}

// 诊断责任：向上送出；首次规范化写集与首次结果必须由唯一写端口正式回显。
L2概念本体根写入结果 重建首次本体根(
    const L1事实基座服务& 第一层服务,
    L1所有者范围写端口& 写入端口,
    const 概念身份来源定位& 来源定位,
    const 概念本体结构定位& 本体定位,
    const L2概念本体根建立请求& 请求,
    const L1所有者范围写集请求& 固定写集,
    const L1所有者范围写入结果& 提交结果) {
    const auto 首次读取 = 写入端口.读取首次写入材料(
        {L1所有者范围首次写入读取合同版本, 固定写集.写入幂等身份});
    if (首次读取.状态 != L1所有者范围读取状态::成功) {
        return 形成根写入失败(
            首次读取.状态 == L1所有者范围读取状态::资源失败
                ? L2结构状态::资源失败 : L2结构状态::内部不一致,
            首次读取.读取事实代次);
    }
    if (首次读取.合同版本 != L1所有者范围首次写入读取合同版本
        || 首次读取.所有者 != 来源定位.所有者
        || 首次读取.写入幂等身份 != 固定写集.写入幂等身份
        || 首次读取.读取事实代次 == 0
        || !首次读取.首次规范化写集 || !首次读取.首次写入结果
        || *首次读取.首次规范化写集 != 固定写集)
        return 形成根写入失败(L2结构状态::内部不一致,
            首次读取.读取事实代次);

    const auto& 首次结果 = *首次读取.首次写入结果;
    const bool 首次结果完整 = 首次结果.状态 == L1所有者范围写入状态::成功
        && 首次结果.合同版本 == L1所有者范围CRUD合同版本
        && 首次结果.所有者 == 来源定位.所有者
        && 首次结果.写入幂等身份 == 固定写集.写入幂等身份
        && 首次结果.事实代次 != 0
        && 首次结果.事实代次 <= 首次读取.读取事实代次
        && 首次结果.是否形成内存权威发布
        && 首次结果.重试边界 == L1所有者范围重试边界::不适用;
    const bool 提交首次完整 = 提交结果.状态 == L1所有者范围写入状态::成功
        && 提交结果.是否形成内存权威发布
        && 提交结果.重试边界 == L1所有者范围重试边界::不适用;
    const bool 提交重复完整 = 提交结果.状态
            == L1所有者范围写入状态::精确重复
        && !提交结果.是否形成内存权威发布
        && 提交结果.重试边界
            == L1所有者范围重试边界::原幂等身份读回收敛;
    if (!首次结果完整 || (!提交首次完整 && !提交重复完整)
        || 提交结果.合同版本 != L1所有者范围CRUD合同版本
        || 提交结果.所有者 != 首次结果.所有者
        || 提交结果.写入幂等身份 != 首次结果.写入幂等身份
        || 提交结果.事实代次 != 首次结果.事实代次
        || 提交结果.新编码映射 != 首次结果.新编码映射)
        return 形成根写入失败(L2结构状态::内部不一致,
            首次结果.事实代次);
    const auto 映射 = 读取根写入映射(首次结果);
    if (!映射)
        return 形成根写入失败(L2结构状态::内部不一致,
            首次结果.事实代次);
    const auto 截止 = 首次结果.事实代次;

    const auto 所有者读 = 第一层服务.读取历史结构所有者(
        {L1所有者范围CRUD合同版本, 来源定位.所有者});
    if (所有者读.状态 != L1所有者范围读取状态::成功
        || 所有者读.合同版本 != L1所有者范围CRUD合同版本
        || 所有者读.查询所有者 != 来源定位.所有者
        || 所有者读.读取事实代次 < 截止 || !所有者读.所有者事实
        || 所有者读.所有者事实->所有者 != 来源定位.所有者
        || 所有者读.所有者事实->范围种类
            != L1所有者范围种类::独占结构范围
        || 所有者读.所有者事实->创建事实代次 == 0
        || 所有者读.所有者事实->创建事实代次 > 截止
        || 所有者读.所有者事实->退出事实代次)
        return 形成根写入失败(L2结构状态::内部不一致, 截止);

    const auto 读节点 = [&](稳定编码 编码)
        -> std::optional<L1所有者范围节点事实> {
        const auto 读取 = 第一层服务.读取所有者范围历史事实(
            {L1所有者范围CRUD合同版本, 编码});
        const auto* 节点 = 读取.事实
            ? std::get_if<L1所有者范围节点事实>(&*读取.事实) : nullptr;
        if (读取.状态 != L1所有者范围读取状态::成功
            || 读取.合同版本 != L1所有者范围CRUD合同版本
            || 读取.查询编码 != 编码 || 读取.读取事实代次 < 截止
            || !节点 || 节点->编码 != 编码
            || 节点->写入所有者 != 来源定位.所有者
            || 节点->创建事实代次 > 截止 || 节点->退出事实代次)
            return std::nullopt;
        return *节点;
    };
    const auto 读关系 = [&](稳定编码 编码)
        -> std::optional<L1所有者范围关系事实> {
        const auto 读取 = 第一层服务.读取所有者范围历史事实(
            {L1所有者范围CRUD合同版本, 编码});
        const auto* 关系 = 读取.事实
            ? std::get_if<L1所有者范围关系事实>(&*读取.事实) : nullptr;
        if (读取.状态 != L1所有者范围读取状态::成功
            || 读取.合同版本 != L1所有者范围CRUD合同版本
            || 读取.查询编码 != 编码 || 读取.读取事实代次 < 截止
            || !关系 || 关系->编码 != 编码
            || 关系->写入所有者 != 来源定位.所有者
            || 关系->创建事实代次 > 截止 || 关系->退出事实代次)
            return std::nullopt;
        return *关系;
    };
    const auto 读值 = [&](稳定编码 编码)
        -> std::optional<L1所有者范围值事实> {
        const auto 读取 = 第一层服务.读取所有者范围历史事实(
            {L1所有者范围CRUD合同版本, 编码});
        const auto* 值 = 读取.事实
            ? std::get_if<L1所有者范围值事实>(&*读取.事实) : nullptr;
        if (读取.状态 != L1所有者范围读取状态::成功
            || 读取.合同版本 != L1所有者范围CRUD合同版本
            || 读取.查询编码 != 编码 || 读取.读取事实代次 < 截止
            || !值 || 值->编码 != 编码
            || 值->写入所有者 != 来源定位.所有者
            || 值->创建事实代次 > 截止 || 值->退出事实代次)
            return std::nullopt;
        return *值;
    };

    const auto 根节点 = 读节点(映射->根节点);
    const auto 族锚点 = 读节点(来源定位.族锚点);
    const auto 族类型 = 读节点(来源定位.族归属关系类型);
    const auto 族关系 = 读关系(映射->族归属关系);
    const auto 角色锚点 = 读节点(本体定位.角色锚点);
    const auto 角色类型 = 读节点(本体定位.角色关系类型);
    const auto 角色关系 = 读关系(映射->角色关系);
    const auto 上位类型 = 读节点(本体定位.直接上位关系类型);
    const auto 见证类型 = 读节点(本体定位.首次见证属性类型);
    const auto 见证值 = 读值(映射->见证值);
    if (!根节点 || !族锚点 || !族类型 || !族关系 || !角色锚点
        || !角色类型 || !角色关系 || !上位类型 || !见证类型 || !见证值)
        return 形成根写入失败(L2结构状态::内部不一致, 截止);

    const auto* 见证材料 = std::get_if<std::vector<std::uint64_t>>(
        &见证值->材料);
    if (根节点->种类 != 节点种类::普通 || 根节点->属性类型表示
        || 根节点->创建事实代次 != 截止 || 根节点->当前属性.size() != 1
        || 根节点->当前属性.front().属性类型节点
            != 本体定位.首次见证属性类型
        || 根节点->当前属性.front().当前值 != 映射->见证值
        || !登记节点当前完整(*族锚点, 来源定位.所有者,
            来源定位.族锚点, 节点种类::普通, std::nullopt,
            来源定位.建立事实代次)
        || !登记节点当前完整(*族类型, 来源定位.所有者,
            来源定位.族归属关系类型, 节点种类::普通,
            std::nullopt, 来源定位.建立事实代次)
        || 族关系->源节点 != 映射->根节点
        || 族关系->目标节点 != 来源定位.族锚点
        || 族关系->关系类型节点 != 来源定位.族归属关系类型
        || 族关系->角色或顺序 != 概念族归属角色值
        || 族关系->创建事实代次 != 截止
        || !登记节点当前完整(*角色锚点, 来源定位.所有者,
            本体定位.角色锚点, 节点种类::普通, std::nullopt,
            本体定位.建立事实代次)
        || !登记节点当前完整(*角色类型, 来源定位.所有者,
            本体定位.角色关系类型, 节点种类::普通,
            std::nullopt, 本体定位.建立事实代次)
        || 角色关系->源节点 != 映射->根节点
        || 角色关系->目标节点 != 本体定位.角色锚点
        || 角色关系->关系类型节点 != 本体定位.角色关系类型
        || 角色关系->角色或顺序 != static_cast<std::int64_t>(请求.角色)
        || 角色关系->创建事实代次 != 截止
        || !登记节点当前完整(*上位类型, 来源定位.所有者,
            本体定位.直接上位关系类型, 节点种类::普通,
            std::nullopt, 本体定位.建立事实代次)
        || !登记节点当前完整(*见证类型, 来源定位.所有者,
            本体定位.首次见证属性类型, 节点种类::属性类型,
            L1所有者范围值表示种类::U64组, 本体定位.建立事实代次)
        || 见证值->所属节点 != 映射->根节点
        || 见证值->属性类型节点 != 本体定位.首次见证属性类型
        || 见证值->来源节点 != 映射->根节点
        || 见证值->创建事实代次 != 截止 || !见证材料
        || *见证材料 != std::vector<std::uint64_t>{请求.幂等身份.值,
            请求.请求头.期望事实代次})
        return 形成根写入失败(L2结构状态::内部不一致, 截止);

    const auto 上位组 = 第一层服务.读取所有者范围历史关系组({
        L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::目标,
        映射->根节点, 本体定位.直接上位关系类型, 截止});
    if (上位组.状态 != L1所有者范围读取状态::成功
        || 上位组.合同版本 != L1所有者范围CRUD合同版本
        || 上位组.方向 != L1所有者范围关系端点方向::目标
        || 上位组.端点节点 != 映射->根节点
        || 上位组.关系类型节点 != 本体定位.直接上位关系类型
        || 上位组.历史截止事实代次 != 截止
        || 上位组.读取事实代次 < 截止 || !上位组.关系组.empty())
        return 形成根写入失败(L2结构状态::内部不一致, 截止);

    L2节点族身份来源事实 身份来源{
        L2节点族::概念, 映射->根节点, 来源定位.所有者,
        来源定位.族锚点, 来源定位.族归属关系类型,
        映射->族归属关系, 概念族归属角色值,
        {所有者读.所有者事实->创建事实代次,
            所有者读.所有者事实->退出事实代次},
        {根节点->创建事实代次, 根节点->退出事实代次},
        {族锚点->创建事实代次, 族锚点->退出事实代次},
        {族类型->创建事实代次, 族类型->退出事实代次},
        {族关系->创建事实代次, 族关系->退出事实代次}};
    L2概念本体根事实 根{
        L2概念身份{映射->根节点}, std::move(身份来源),
        {请求.角色, L2概念身份{映射->根节点}, 映射->角色关系,
            {角色关系->创建事实代次, 角色关系->退出事实代次}},
        请求.幂等身份, 请求.请求头.期望事实代次, {}};
    const auto 状态 = 提交结果.状态 == L1所有者范围写入状态::成功
        ? L2结构状态::已提交 : L2结构状态::精确重复;
    L2概念本体根写入结果 结果{
        {L2结构合同版本, 状态, 截止, 截止}, std::move(根)};
    return 结果.成功() ? 结果
        : 形成根写入失败(L2结构状态::内部不一致, 截止);
}

inline constexpr std::uint32_t 直接上位关系写入本地键值 = 0x020A'3001U;

struct 直接关系期望 final {
    L2概念身份 上位;
    L2概念身份 下位;
};

struct 直接关系事实读取值 final {
    L2结构状态 状态 = L2结构状态::入口拒绝;
    std::uint64_t 读取事实代次 = 0;
    std::optional<L1所有者范围关系事实> 关系;
};

// 诊断责任：无适用错误分支；关系写失败固定为空载荷和零变更。
L2概念直接上位写入结果 形成概念关系写入失败(
    L2结构状态 状态, std::uint64_t 截止 = 0) noexcept {
    return {{L2结构合同版本, 状态, 截止, std::nullopt},
        std::nullopt, std::nullopt};
}

// 诊断责任：无适用错误分支；关系组失败固定为空组和零变更。
L2概念直接上位组读取结果 形成概念关系组失败(
    L2结构状态 状态, L2读取类别 类别,
    std::uint64_t 历史截止, std::uint64_t 截止 = 0) noexcept {
    return {{L2结构合同版本, 状态, 截止, std::nullopt},
        类别, 类别 == L2读取类别::历史 ? 历史截止 : 0, {}};
}

// 诊断责任：无适用错误分支；按当前优先、历史回退读取一条 owner 关系事实。
直接关系事实读取值 读取直接关系事实(
    const L1事实基座服务& 第一层服务,
    const 概念身份来源定位& 来源定位,
    const 概念本体结构定位& 本体定位,
    稳定编码 关系编码) noexcept {
    const auto 当前 = 第一层服务.读取所有者范围当前关系(
        {L1所有者范围CRUD合同版本, 关系编码});
    const auto 映射 = [&](const L1所有者范围当前读取结果& 读取)
        -> 直接关系事实读取值 {
        if (读取.状态 != L1所有者范围读取状态::成功 || !读取.事实)
            return {映射历史读取状态(读取.状态), 读取.读取事实代次};
        const auto* 关系 = std::get_if<L1所有者范围关系事实>(&*读取.事实);
        if (!关系 || 读取.合同版本 != L1所有者范围CRUD合同版本
            || 读取.查询编码 != 关系编码
            || 关系->编码 != 关系编码 || 关系->写入所有者 != 来源定位.所有者
            || 关系->关系类型节点 != 本体定位.直接上位关系类型)
            return {L2结构状态::内部不一致, 读取.读取事实代次};
        return {L2结构状态::已读取, 读取.读取事实代次, *关系};
    };
    if (当前.状态 == L1所有者范围读取状态::成功) return 映射(当前);
    if (当前.状态 != L1所有者范围读取状态::已退出
        && 当前.状态 != L1所有者范围读取状态::未找到)
        return {映射历史读取状态(当前.状态), 当前.读取事实代次};
    const auto 历史 = 第一层服务.读取所有者范围历史事实(
        {L1所有者范围CRUD合同版本, 关系编码});
    if (历史.状态 != L1所有者范围读取状态::成功 || !历史.事实)
        return {映射历史读取状态(历史.状态), 历史.读取事实代次};
    const auto* 关系 = std::get_if<L1所有者范围关系事实>(&*历史.事实);
    if (!关系 || 历史.合同版本 != L1所有者范围CRUD合同版本
        || 历史.查询编码 != 关系编码
        || 关系->编码 != 关系编码 || 关系->写入所有者 != 来源定位.所有者
        || 关系->关系类型节点 != 本体定位.直接上位关系类型)
        return {L2结构状态::内部不一致, 历史.读取事实代次};
    return {L2结构状态::已读取, 历史.读取事实代次, *关系};
}

// 诊断责任：无适用错误分支；形成新增、替换或退出的确定 owner 写集。
L1所有者范围写集请求 形成直接关系写集(
    std::uint64_t 期望代次, L2结构幂等身份 幂等身份,
    const 概念本体结构定位& 本体定位,
    std::optional<直接关系期望> 新关系,
    std::optional<稳定编码> 退出关系) {
    L1所有者范围写集请求 写集;
    写集.合同版本 = L1所有者范围CRUD合同版本;
    写集.期望事实代次 = 期望代次;
    写集.写入幂等身份 = {幂等身份.值};
    if (新关系) {
        写集.关系.push_back({{直接上位关系写入本地键值},
            新关系->上位.值, 新关系->下位.值,
            本体定位.直接上位关系类型, 1});
    }
    if (退出关系) 写集.退出事实.push_back(*退出关系);
    return 写集;
}

// 诊断责任：无适用错误分支；首次结果只映射关系本地键，退出编码不猜测。
L2概念直接上位写入结果 重建直接关系首次结果(
    const L1事实基座服务& 第一层服务,
    const 概念身份来源定位& 来源定位,
    const 概念本体结构定位& 本体定位,
    const L1所有者范围写集请求& 写集,
    const L1所有者范围写入结果& 首次结果,
    L2结构状态 返回状态,
    std::optional<直接关系期望> 新关系,
    std::optional<std::pair<稳定编码, 直接关系期望>> 旧关系) noexcept {
    const std::uint64_t 截止 = 首次结果.事实代次;
    const bool 首次完整 = 首次结果.状态 == L1所有者范围写入状态::成功
        && 首次结果.合同版本 == L1所有者范围CRUD合同版本
        && 首次结果.所有者 == 来源定位.所有者
        && 首次结果.写入幂等身份 == 写集.写入幂等身份
        && 截止 != 0 && 首次结果.是否形成内存权威发布
        && 首次结果.重试边界 == L1所有者范围重试边界::不适用;
    if (!首次完整) return 形成概念关系写入失败(
        L2结构状态::内部不一致, 截止);

    std::optional<L2概念直接上位事实> 当前关系;
    std::optional<L2概念直接上位事实> 已退出关系;
    if (新关系) {
        if (首次结果.新编码映射.size() != 1
            || 首次结果.新编码映射.front().first
                != L1所有者范围写集本地键{直接上位关系写入本地键值})
            return 形成概念关系写入失败(L2结构状态::内部不一致, 截止);
        const auto 编码 = 首次结果.新编码映射.front().second;
        const auto 读取 = 读取直接关系事实(
            第一层服务, 来源定位, 本体定位, 编码);
        if (读取.状态 != L2结构状态::已读取 || !读取.关系
            || 读取.关系->源节点 != 新关系->上位.值
            || 读取.关系->目标节点 != 新关系->下位.值
            || 读取.关系->创建事实代次 != 截止
            || (读取.关系->退出事实代次
                && *读取.关系->退出事实代次 <= 截止))
            return 形成概念关系写入失败(
                读取.状态 == L2结构状态::已读取
                    ? L2结构状态::内部不一致 : 读取.状态,
                读取.读取事实代次);
        当前关系 = L2概念直接上位事实{新关系->上位, 新关系->下位,
            编码, {读取.关系->创建事实代次, std::nullopt}};
    } else if (!首次结果.新编码映射.empty()) {
        return 形成概念关系写入失败(L2结构状态::内部不一致, 截止);
    }
    if (旧关系) {
        const auto 读取 = 读取直接关系事实(
            第一层服务, 来源定位, 本体定位, 旧关系->first);
        if (读取.状态 != L2结构状态::已读取 || !读取.关系
            || 读取.关系->源节点 != 旧关系->second.上位.值
            || 读取.关系->目标节点 != 旧关系->second.下位.值
            || 读取.关系->退出事实代次 != 截止)
            return 形成概念关系写入失败(
                读取.状态 == L2结构状态::已读取
                    ? L2结构状态::内部不一致 : 读取.状态,
                读取.读取事实代次);
        已退出关系 = L2概念直接上位事实{旧关系->second.上位,
            旧关系->second.下位, 旧关系->first,
            {读取.关系->创建事实代次, 截止}};
    }
    L2概念直接上位写入结果 结果{{L2结构合同版本, 返回状态,
        截止, 截止}, std::move(当前关系), std::move(已退出关系)};
    return 结果.成功() ? 结果
        : 形成概念关系写入失败(L2结构状态::内部不一致, 截止);
}

// 诊断责任：无适用错误分支；只读取一个端点方向的当前或历史活动关系组。
L2概念直接上位组读取结果 读取直接关系组核心(
    const L1事实基座服务& 第一层服务,
    const 概念身份来源定位& 来源定位,
    const 概念本体结构定位& 本体定位,
    L1所有者范围关系端点方向 方向, L2概念身份 端点,
    L2读取类别 类别, std::uint64_t 历史截止,
    std::uint64_t 期望代次, std::size_t 数量预算) {
    std::vector<L1所有者范围关系事实> 原始组;
    std::uint64_t 读取代次 = 0;
    if (类别 == L2读取类别::当前) {
        if (方向 == L1所有者范围关系端点方向::源) {
            const auto 读取 = 第一层服务.读取所有者范围当前源关系组({
                L1所有者范围CRUD合同版本, 端点.值,
                本体定位.直接上位关系类型});
            读取代次 = 读取.读取事实代次;
            if (读取.状态 != L1所有者范围读取状态::成功)
                return 形成概念关系组失败(
                    映射历史读取状态(读取.状态), 类别, 0, 读取代次);
            if (读取.合同版本 != L1所有者范围CRUD合同版本
                || 读取.源节点 != 端点.值
                || 读取.关系类型节点 != 本体定位.直接上位关系类型)
                return 形成概念关系组失败(
                    L2结构状态::内部不一致, 类别, 0, 读取代次);
            原始组 = 读取.关系组;
        } else {
            const auto 读取 = 第一层服务.读取所有者范围当前目标关系组({
                L1所有者范围CRUD合同版本, 端点.值,
                本体定位.直接上位关系类型});
            读取代次 = 读取.读取事实代次;
            if (读取.状态 != L1所有者范围读取状态::成功)
                return 形成概念关系组失败(
                    映射历史读取状态(读取.状态), 类别, 0, 读取代次);
            if (读取.合同版本 != L1所有者范围CRUD合同版本
                || 读取.目标节点 != 端点.值
                || 读取.关系类型节点 != 本体定位.直接上位关系类型)
                return 形成概念关系组失败(
                    L2结构状态::内部不一致, 类别, 0, 读取代次);
            原始组 = 读取.关系组;
        }
    } else {
        const auto 读取 = 第一层服务.读取所有者范围历史关系组({
            L1所有者范围CRUD合同版本, 方向, 端点.值,
            本体定位.直接上位关系类型, 历史截止});
        读取代次 = 读取.读取事实代次;
        if (读取.状态 != L1所有者范围读取状态::成功)
            return 形成概念关系组失败(
                映射历史读取状态(读取.状态), 类别, 历史截止, 读取代次);
        if (读取.合同版本 != L1所有者范围CRUD合同版本
            || 读取.方向 != 方向 || 读取.端点节点 != 端点.值
            || 读取.关系类型节点 != 本体定位.直接上位关系类型
            || 读取.历史截止事实代次 != 历史截止)
            return 形成概念关系组失败(
                L2结构状态::内部不一致, 类别, 历史截止, 读取代次);
        原始组 = 读取.关系组;
    }
    if (读取代次 != 期望代次)
        return 形成概念关系组失败(
            L2结构状态::事实代次漂移, 类别, 历史截止, 读取代次);
    if (原始组.size() > 数量预算)
        return 形成概念关系组失败(
            L2结构状态::数量预算不足, 类别, 历史截止, 读取代次);

    const std::uint64_t 投影截止 = 类别 == L2读取类别::历史
        ? 历史截止 : 期望代次;
    std::vector<L2概念直接上位事实> 关系组;
    关系组.reserve(原始组.size());
    for (const auto& 关系 : 原始组) {
        if (!有效(关系.编码) || 关系.写入所有者 != 来源定位.所有者
            || 关系.关系类型节点 != 本体定位.直接上位关系类型
            || 关系.源节点 == 关系.目标节点 || 关系.角色或顺序 <= 0
            || 关系.创建事实代次 == 0 || 关系.创建事实代次 > 投影截止
            || (方向 == L1所有者范围关系端点方向::源
                ? 关系.源节点 != 端点.值 : 关系.目标节点 != 端点.值)
            || (类别 == L2读取类别::当前 && 关系.退出事实代次)
            || (类别 == L2读取类别::历史 && 关系.退出事实代次
                && *关系.退出事实代次 <= 历史截止))
            return 形成概念关系组失败(
                L2结构状态::内部不一致, 类别, 历史截止, 读取代次);
        关系组.push_back({L2概念身份{关系.源节点},
            L2概念身份{关系.目标节点}, 关系.编码,
            {关系.创建事实代次, std::nullopt}});
    }
    std::sort(关系组.begin(), 关系组.end(), [](const auto& 左, const auto& 右) {
        return 左.上位概念.值 < 右.上位概念.值
            || (左.上位概念 == 右.上位概念
                && (左.下位概念.值 < 右.下位概念.值
                    || (左.下位概念 == 右.下位概念
                        && 左.关系稳定编码 < 右.关系稳定编码)));
    });
    L2概念直接上位组读取结果 结果{{L2结构合同版本,
        L2结构状态::已读取, 投影截止, std::nullopt}, 类别,
        类别 == L2读取类别::历史 ? 历史截止 : 0, std::move(关系组)};
    return 结果.成功() ? 结果
        : 形成概念关系组失败(
            L2结构状态::内部不一致, 类别, 历史截止, 读取代次);
}

struct 概念世界事实支持内部事实 final {
    L2概念世界事实支持关系事实 公开事实;
    稳定编码 支持节点;
    稳定编码 登记关系;
    稳定编码 世界事实值;
    稳定编码 建立依据值;
    稳定编码 发布证据值;
};

struct 概念世界事实支持单项读取值 final {
    L2结构状态 状态 = L2结构状态::内部不一致;
    std::uint64_t 截止 = 0;
    std::optional<概念世界事实支持内部事实> 事实;
};

// 诊断责任：无适用错误分支；支持写失败固定为空载荷与零变更。
L2概念世界事实支持写入结果 形成概念支持写入失败(
    L2结构状态 状态, std::uint64_t 截止 = 0) noexcept {
    if (状态 == L2结构状态::已提交 || 状态 == L2结构状态::精确重复
        || 状态 == L2结构状态::已读取) 状态 = L2结构状态::内部不一致;
    return {{L2结构合同版本, 状态, 截止, std::nullopt},
        std::nullopt, std::nullopt};
}

// 诊断责任：无适用错误分支；支持组失败保留读取类别且固定为空组与零变更。
L2概念世界事实支持组读取结果 形成概念支持组失败(
    L2结构状态 状态, L2读取类别 类别,
    std::uint64_t 历史截止, std::uint64_t 截止 = 0) noexcept {
    if (状态 == L2结构状态::已提交 || 状态 == L2结构状态::精确重复
        || 状态 == L2结构状态::已读取) 状态 = L2结构状态::内部不一致;
    return {{L2结构合同版本, 状态, 截止, std::nullopt}, 类别,
        类别 == L2读取类别::历史 ? 历史截止 : 0, {}};
}

// 诊断责任：无适用错误分支；在一个当前守卫下重建指定支持内部节点的完整投影。
概念世界事实支持单项读取值 读取概念支持单项(
    const L1事实基座服务& 第一层服务,
    const 概念身份来源定位& 来源定位,
    const 概念世界事实支持结构定位& 支持定位,
    L2读取类别 类别, std::uint64_t 历史截止,
    std::uint64_t 期望代次, 稳定编码 支持节点,
    std::optional<稳定编码> 期望登记关系 = std::nullopt,
    std::optional<稳定编码> 期望支持关系 = std::nullopt) {
    const std::uint64_t 投影截止 = 类别 == L2读取类别::历史
        ? 历史截止 : 期望代次;
    if (!有效(支持节点) || 投影截止 == 0 || 期望代次 == 0)
        return {L2结构状态::内部不一致, 期望代次};
    const auto 活动 = [投影截止, 类别](std::uint64_t 创建,
        const std::optional<std::uint64_t>& 退出) noexcept {
        return 创建 != 0 && 创建 <= 投影截止
            && (!退出 || *退出 > 投影截止)
            && (类别 == L2读取类别::历史 || !退出);
    };
    L1所有者范围节点事实 节点;
    bool 有节点 = false;
    const auto 解释节点 = [&](const auto& 读取) -> L2结构状态 {
        if (读取.读取事实代次 != 期望代次)
            return L2结构状态::事实代次漂移;
        if (读取.状态 != L1所有者范围读取状态::成功 || !读取.事实)
            return 映射历史读取状态(读取.状态);
        const auto* 候选 = std::get_if<L1所有者范围节点事实>(&*读取.事实);
        if (读取.合同版本 != L1所有者范围CRUD合同版本
            || 读取.查询编码 != 支持节点 || !候选
            || 候选->编码 != 支持节点
            || 候选->写入所有者 != 来源定位.所有者
            || 候选->种类 != 节点种类::普通 || 候选->属性类型表示
            || !活动(候选->创建事实代次, 候选->退出事实代次))
            return L2结构状态::内部不一致;
        节点 = *候选;
        有节点 = true;
        return L2结构状态::已读取;
    };
    L2结构状态 状态;
    if (类别 == L2读取类别::当前)
        状态 = 解释节点(第一层服务.读取所有者范围当前节点(
            {L1所有者范围CRUD合同版本, 支持节点}));
    else
        状态 = 解释节点(第一层服务.读取所有者范围历史事实(
            {L1所有者范围CRUD合同版本, 支持节点}));
    if (状态 != L2结构状态::已读取 || !有节点)
        return {状态, 期望代次};

    const auto 读取关系组 = [&](L1所有者范围关系端点方向 方向,
        稳定编码 端点, 稳定编码 类型)
        -> std::pair<L2结构状态, std::vector<L1所有者范围关系事实>> {
        std::vector<L1所有者范围关系事实> 关系组;
        if (类别 == L2读取类别::当前) {
            if (方向 == L1所有者范围关系端点方向::源) {
                const auto 读取 = 第一层服务.读取所有者范围当前源关系组(
                    {L1所有者范围CRUD合同版本, 端点, 类型});
                if (读取.读取事实代次 != 期望代次)
                    return {L2结构状态::事实代次漂移, {}};
                if (读取.状态 != L1所有者范围读取状态::成功)
                    return {映射历史读取状态(读取.状态), {}};
                if (读取.合同版本 != L1所有者范围CRUD合同版本
                    || 读取.源节点 != 端点 || 读取.关系类型节点 != 类型)
                    return {L2结构状态::内部不一致, {}};
                关系组 = 读取.关系组;
            } else {
                const auto 读取 = 第一层服务.读取所有者范围当前目标关系组(
                    {L1所有者范围CRUD合同版本, 端点, 类型});
                if (读取.读取事实代次 != 期望代次)
                    return {L2结构状态::事实代次漂移, {}};
                if (读取.状态 != L1所有者范围读取状态::成功)
                    return {映射历史读取状态(读取.状态), {}};
                if (读取.合同版本 != L1所有者范围CRUD合同版本
                    || 读取.目标节点 != 端点 || 读取.关系类型节点 != 类型)
                    return {L2结构状态::内部不一致, {}};
                关系组 = 读取.关系组;
            }
        } else {
            const auto 读取 = 第一层服务.读取所有者范围历史关系组({
                L1所有者范围CRUD合同版本, 方向, 端点, 类型, 历史截止});
            if (读取.读取事实代次 != 期望代次)
                return {L2结构状态::事实代次漂移, {}};
            if (读取.状态 != L1所有者范围读取状态::成功)
                return {映射历史读取状态(读取.状态), {}};
            if (读取.合同版本 != L1所有者范围CRUD合同版本
                || 读取.方向 != 方向 || 读取.端点节点 != 端点
                || 读取.关系类型节点 != 类型
                || 读取.历史截止事实代次 != 历史截止)
                return {L2结构状态::内部不一致, {}};
            关系组 = 读取.关系组;
        }
        return {L2结构状态::已读取, std::move(关系组)};
    };
    auto 登记组 = 读取关系组(L1所有者范围关系端点方向::目标,
        支持节点, 支持定位.登记成员关系类型);
    auto 支持组 = 读取关系组(L1所有者范围关系端点方向::源,
        支持节点, 支持定位.支持关系类型);
    if (登记组.first != L2结构状态::已读取)
        return {登记组.first, 期望代次};
    if (支持组.first != L2结构状态::已读取)
        return {支持组.first, 期望代次};
    if (登记组.second.size() != 1 || 支持组.second.size() != 1)
        return {L2结构状态::内部不一致, 期望代次};
    const auto& 登记关系 = 登记组.second.front();
    const auto& 支持关系 = 支持组.second.front();
    const auto 关系完整 = [&](const L1所有者范围关系事实& 关系,
        稳定编码 类型) {
        return 有效(关系.编码) && 关系.写入所有者 == 来源定位.所有者
            && 关系.关系类型节点 == 类型 && 关系.角色或顺序 == 1
            && 活动(关系.创建事实代次, 关系.退出事实代次)
            && 关系.创建事实代次 == 节点.创建事实代次;
    };
    if (!关系完整(登记关系, 支持定位.登记成员关系类型)
        || 登记关系.源节点 != 支持定位.登记根
        || 登记关系.目标节点 != 支持节点
        || !关系完整(支持关系, 支持定位.支持关系类型)
        || 支持关系.源节点 != 支持节点
        || !有效(支持关系.目标节点)
        || (期望登记关系 && 登记关系.编码 != *期望登记关系)
        || (期望支持关系 && 支持关系.编码 != *期望支持关系))
        return {L2结构状态::内部不一致, 期望代次};

    std::vector<L1所有者范围值事实> 值组;
    if (类别 == L2读取类别::当前) {
        if (节点.当前属性.size() != 3)
            return {L2结构状态::内部不一致, 期望代次};
        for (const auto& 槽 : 节点.当前属性) {
            const auto 读取 = 第一层服务.读取所有者范围当前值(
                {L1所有者范围CRUD合同版本, 槽.当前值});
            if (读取.读取事实代次 != 期望代次)
                return {L2结构状态::事实代次漂移, 读取.读取事实代次};
            if (读取.状态 != L1所有者范围读取状态::成功 || !读取.事实)
                return {映射历史读取状态(读取.状态), 读取.读取事实代次};
            const auto* 值 = std::get_if<L1所有者范围值事实>(&*读取.事实);
            if (读取.合同版本 != L1所有者范围CRUD合同版本
                || 读取.查询编码 != 槽.当前值 || !值
                || 值->属性类型节点 != 槽.属性类型节点)
                return {L2结构状态::内部不一致, 读取.读取事实代次};
            值组.push_back(*值);
        }
    } else {
        const auto 读取 = 第一层服务.读取所有者范围历史属性值组({
            L1所有者范围CRUD合同版本, 支持节点, 历史截止});
        if (读取.读取事实代次 != 期望代次)
            return {L2结构状态::事实代次漂移, 读取.读取事实代次};
        if (读取.状态 != L1所有者范围读取状态::成功)
            return {映射历史读取状态(读取.状态), 读取.读取事实代次};
        if (读取.合同版本 != L1所有者范围CRUD合同版本
            || 读取.所属节点 != 支持节点
            || 读取.历史截止事实代次 != 历史截止)
            return {L2结构状态::内部不一致, 读取.读取事实代次};
        值组 = 读取.属性值组;
    }
    if (值组.size() != 3)
        return {L2结构状态::内部不一致, 期望代次};
    const L1所有者范围值事实* 世界值 = nullptr;
    const L1所有者范围值事实* 依据值 = nullptr;
    const L1所有者范围值事实* 证据值 = nullptr;
    for (const auto& 值 : 值组) {
        if (!有效(值.编码) || 值.写入所有者 != 来源定位.所有者
            || 值.所属节点 != 支持节点
            || 值.创建事实代次 != 节点.创建事实代次
            || !活动(值.创建事实代次, 值.退出事实代次))
            return {L2结构状态::内部不一致, 期望代次};
        const L1所有者范围值事实** 目标 = nullptr;
        if (值.属性类型节点 == 支持定位.世界事实编码属性类型) 目标 = &世界值;
        else if (值.属性类型节点 == 支持定位.建立依据材料属性类型) 目标 = &依据值;
        else if (值.属性类型节点 == 支持定位.发布证据材料组属性类型) 目标 = &证据值;
        else return {L2结构状态::内部不一致, 期望代次};
        if (*目标) return {L2结构状态::内部不一致, 期望代次};
        *目标 = &值;
    }
    if (!世界值 || !依据值 || !证据值)
        return {L2结构状态::内部不一致, 期望代次};
    const auto* 世界编码 = std::get_if<std::vector<std::uint64_t>>(&世界值->材料);
    const auto* 依据材料 = std::get_if<L1所有者范围独立材料引用>(&依据值->材料);
    const auto* 证据材料 = std::get_if<std::vector<std::uint64_t>>(&证据值->材料);
    const auto 世界事实 = 世界编码 ? 解码世界事实引用(*世界编码) : std::nullopt;
    if (!世界事实 || !依据材料 || !有效(依据材料->编码) || !证据材料
        || 世界值->来源节点 != 支持节点
        || 依据值->来源节点 != 依据材料->编码
        || 证据值->来源节点 != 支持节点)
        return {L2结构状态::内部不一致, 期望代次};
    std::vector<不可变材料身份> 发布证据;
    发布证据.reserve(证据材料->size());
    for (const auto 编码 : *证据材料) 发布证据.push_back({稳定编码{编码}});
    L2概念世界事实支持关系事实 公开事实{*世界事实,
        L2概念身份{支持关系.目标节点}, 不可变材料身份{依据材料->编码},
        std::move(发布证据), 支持关系.编码,
        {支持关系.创建事实代次, std::nullopt}};
    if (!L2概念世界事实支持关系事实截止投影完整(公开事实, 投影截止))
        return {L2结构状态::内部不一致, 期望代次};
    return {L2结构状态::已读取, 投影截止,
        概念世界事实支持内部事实{std::move(公开事实), 支持节点,
            登记关系.编码, 世界值->编码, 依据值->编码, 证据值->编码}};
}

// 诊断责任：无适用错误分支；按概念或按登记根枚举支持项并形成稳定排序组。
L2概念世界事实支持组读取结果 读取概念支持关系组核心(
    const L1事实基座服务& 第一层服务,
    const 概念身份来源定位& 来源定位,
    const 概念世界事实支持结构定位& 支持定位,
    L2读取类别 类别, std::uint64_t 历史截止,
    std::uint64_t 期望代次, std::optional<L2概念身份> 概念,
    std::optional<L2世界事实引用> 世界事实,
    std::size_t 扫描预算, std::size_t 数量预算) {
    std::vector<L1所有者范围关系事实> 枚举关系;
    const auto 方向 = 概念 ? L1所有者范围关系端点方向::目标
        : L1所有者范围关系端点方向::源;
    const auto 端点 = 概念 ? 概念->值 : 支持定位.登记根;
    const auto 类型 = 概念 ? 支持定位.支持关系类型
        : 支持定位.登记成员关系类型;
    std::uint64_t 读取代次 = 0;
    if (类别 == L2读取类别::当前) {
        if (概念) {
            const auto 读取 = 第一层服务.读取所有者范围当前目标关系组(
                {L1所有者范围CRUD合同版本, 端点, 类型});
            读取代次 = 读取.读取事实代次;
            if (读取.状态 != L1所有者范围读取状态::成功)
                return 形成概念支持组失败(
                    映射历史读取状态(读取.状态), 类别, 0, 读取代次);
            if (读取.合同版本 != L1所有者范围CRUD合同版本
                || 读取.目标节点 != 端点 || 读取.关系类型节点 != 类型)
                return 形成概念支持组失败(
                    L2结构状态::内部不一致, 类别, 0, 读取代次);
            枚举关系 = 读取.关系组;
        } else {
            const auto 读取 = 第一层服务.读取所有者范围当前源关系组(
                {L1所有者范围CRUD合同版本, 端点, 类型});
            读取代次 = 读取.读取事实代次;
            if (读取.状态 != L1所有者范围读取状态::成功)
                return 形成概念支持组失败(
                    映射历史读取状态(读取.状态), 类别, 0, 读取代次);
            if (读取.合同版本 != L1所有者范围CRUD合同版本
                || 读取.源节点 != 端点 || 读取.关系类型节点 != 类型)
                return 形成概念支持组失败(
                    L2结构状态::内部不一致, 类别, 0, 读取代次);
            枚举关系 = 读取.关系组;
        }
    } else {
        const auto 读取 = 第一层服务.读取所有者范围历史关系组({
            L1所有者范围CRUD合同版本, 方向, 端点, 类型, 历史截止});
        读取代次 = 读取.读取事实代次;
        if (读取.状态 != L1所有者范围读取状态::成功)
            return 形成概念支持组失败(
                映射历史读取状态(读取.状态), 类别, 历史截止, 读取代次);
        if (读取.合同版本 != L1所有者范围CRUD合同版本
            || 读取.方向 != 方向 || 读取.端点节点 != 端点
            || 读取.关系类型节点 != 类型
            || 读取.历史截止事实代次 != 历史截止)
            return 形成概念支持组失败(
                L2结构状态::内部不一致, 类别, 历史截止, 读取代次);
        枚举关系 = 读取.关系组;
    }
    if (读取代次 != 期望代次)
        return 形成概念支持组失败(
            L2结构状态::事实代次漂移, 类别, 历史截止, 读取代次);
    if ((!概念 && 枚举关系.size() > 扫描预算)
        || (概念 && 枚举关系.size() > 数量预算))
        return 形成概念支持组失败(
            L2结构状态::数量预算不足, 类别, 历史截止, 读取代次);
    std::vector<L2概念世界事实支持关系事实> 关系组;
    for (const auto& 枚举项 : 枚举关系) {
        const auto 支持节点 = 概念 ? 枚举项.源节点 : 枚举项.目标节点;
        const auto 单项 = 读取概念支持单项(第一层服务, 来源定位,
            支持定位, 类别, 历史截止, 期望代次, 支持节点,
            概念 ? std::nullopt : std::optional<稳定编码>{枚举项.编码},
            概念 ? std::optional<稳定编码>{枚举项.编码} : std::nullopt);
        if (单项.状态 != L2结构状态::已读取 || !单项.事实)
            return 形成概念支持组失败(
                单项.状态 == L2结构状态::已读取
                    ? L2结构状态::内部不一致 : 单项.状态,
                类别, 历史截止, 期望代次);
        if (概念 && 单项.事实->公开事实.概念 != *概念)
            return 形成概念支持组失败(
                L2结构状态::内部不一致, 类别, 历史截止, 期望代次);
        if (世界事实 && 单项.事实->公开事实.世界事实 != *世界事实) continue;
        关系组.push_back(std::move(单项.事实->公开事实));
        if (关系组.size() > 数量预算)
            return 形成概念支持组失败(
                L2结构状态::数量预算不足, 类别, 历史截止, 期望代次);
    }
    std::sort(关系组.begin(), 关系组.end(), [](const auto& 左, const auto& 右) {
        const auto 左键 = L2普通概念数据内部::世界事实引用排序键(左.世界事实);
        const auto 右键 = L2普通概念数据内部::世界事实引用排序键(右.世界事实);
        return 左键 < 右键 || (左键 == 右键
            && (左.概念.值 < 右.概念.值 || (左.概念 == 右.概念
                && 左.关系稳定编码 < 右.关系稳定编码)));
    });
    for (std::size_t 索引 = 0; 索引 < 关系组.size(); ++索引)
        for (std::size_t 前项 = 0; 前项 < 索引; ++前项)
            if (关系组[前项].关系稳定编码 == 关系组[索引].关系稳定编码
                || (关系组[前项].世界事实 == 关系组[索引].世界事实
                    && 关系组[前项].概念 == 关系组[索引].概念))
                return 形成概念支持组失败(
                    L2结构状态::内部不一致, 类别, 历史截止, 期望代次);
    const auto 尾代次 = 读取当前事实代次(第一层服务);
    if (尾代次.状态 != L1所有者范围读取状态::成功)
        return 形成概念支持组失败(
            尾代次.状态 == L1所有者范围读取状态::资源失败
                ? L2结构状态::资源失败 : L2结构状态::内部不一致,
            类别, 历史截止, 尾代次.事实代次);
    if (尾代次.事实代次 != 期望代次)
        return 形成概念支持组失败(
            L2结构状态::事实代次漂移, 类别, 历史截止,
            尾代次.事实代次);
    const auto 投影截止 = 类别 == L2读取类别::历史
        ? 历史截止 : 期望代次;
    L2概念世界事实支持组读取结果 结果{{L2结构合同版本,
        L2结构状态::已读取, 投影截止, std::nullopt}, 类别,
        类别 == L2读取类别::历史 ? 历史截止 : 0, std::move(关系组)};
    return 结果.成功() ? 结果 : 形成概念支持组失败(
        L2结构状态::内部不一致, 类别, 历史截止, 投影截止);
}

// 诊断责任：无适用错误分支；新增与替换建立完整支持项，退出关闭旧项全部当前事实。
L1所有者范围写集请求 形成概念支持写集(
    std::uint64_t 期望代次, L2结构幂等身份 幂等身份,
    const 概念世界事实支持结构定位& 支持定位,
    const std::optional<L2概念世界事实支持关系事实>& 新关系,
    const std::optional<概念世界事实支持内部事实>& 旧关系) {
    L1所有者范围写集请求 写集;
    写集.合同版本 = L1所有者范围CRUD合同版本;
    写集.期望事实代次 = 期望代次;
    写集.写入幂等身份 = {幂等身份.值};
    if (新关系) {
        const L1所有者范围写集本地键 节点键{支持事实节点本地键值};
        写集.节点.push_back({节点键, 节点种类::普通, std::nullopt});
        写集.关系 = {
            {{支持登记成员关系本地键值}, 支持定位.登记根, 节点键,
                支持定位.登记成员关系类型, 1},
            {{世界事实支持关系本地键值}, 节点键, 新关系->概念.值,
                支持定位.支持关系类型, 1}};
        std::vector<std::uint64_t> 证据;
        证据.reserve(新关系->发布证据材料.size());
        for (const auto 身份 : 新关系->发布证据材料)
            证据.push_back(身份.值.值);
        写集.值 = {
            {{世界事实引用编码值本地键值}, 节点键,
                支持定位.世界事实编码属性类型,
                编码世界事实引用(新关系->世界事实), 节点键},
            {{支持建立依据材料值本地键值}, 节点键,
                支持定位.建立依据材料属性类型,
                L1所有者范围独立材料引用{新关系->建立依据材料.值},
                新关系->建立依据材料.值},
            {{支持发布证据材料组值本地键值}, 节点键,
                支持定位.发布证据材料组属性类型, std::move(证据), 节点键}};
        写集.属性槽变更 = {
            {节点键, 支持定位.世界事实编码属性类型,
                {世界事实引用编码值本地键值}},
            {节点键, 支持定位.建立依据材料属性类型,
                {支持建立依据材料值本地键值}},
            {节点键, 支持定位.发布证据材料组属性类型,
                {支持发布证据材料组值本地键值}}};
    }
    if (旧关系) {
        写集.退出事实 = {旧关系->世界事实值, 旧关系->建立依据值,
            旧关系->发布证据值, 旧关系->登记关系,
            旧关系->公开事实.关系稳定编码, 旧关系->支持节点};
    }
    return 写集;
}

struct 概念支持写入映射 final {
    稳定编码 节点;
    稳定编码 登记关系;
    稳定编码 支持关系;
    稳定编码 世界值;
    稳定编码 依据值;
    稳定编码 证据值;
};

// 诊断责任：无适用错误分支；首次结果必须恰好覆盖新支持项六个确定本地键。
std::optional<概念支持写入映射> 读取概念支持写入映射(
    const L1所有者范围写入结果& 结果, bool 有新关系) noexcept {
    if (!有新关系) return 结果.新编码映射.empty()
        ? std::optional<概念支持写入映射>{概念支持写入映射{}}
        : std::nullopt;
    if (结果.新编码映射.size() != 6) return std::nullopt;
    概念支持写入映射 映射;
    for (const auto& [键, 编码] : 结果.新编码映射) {
        if (!有效(编码)) return std::nullopt;
        稳定编码* 目标 = nullptr;
        switch (键.值) {
        case 支持事实节点本地键值: 目标 = &映射.节点; break;
        case 支持登记成员关系本地键值: 目标 = &映射.登记关系; break;
        case 世界事实支持关系本地键值: 目标 = &映射.支持关系; break;
        case 世界事实引用编码值本地键值: 目标 = &映射.世界值; break;
        case 支持建立依据材料值本地键值: 目标 = &映射.依据值; break;
        case 支持发布证据材料组值本地键值: 目标 = &映射.证据值; break;
        default: return std::nullopt;
        }
        if (有效(*目标)) return std::nullopt;
        *目标 = 编码;
    }
    std::vector<std::uint64_t> 编码{映射.节点.值, 映射.登记关系.值,
        映射.支持关系.值, 映射.世界值.值, 映射.依据值.值, 映射.证据值.值};
    std::sort(编码.begin(), 编码.end());
    if (编码.front() == 0
        || std::adjacent_find(编码.begin(), 编码.end()) != 编码.end())
        return std::nullopt;
    return 映射;
}

// 诊断责任：无适用错误分支；当前公开支持关系编码必须唯一定位完整内部支持项。
概念世界事实支持单项读取值 读取当前概念支持内部事实(
    const L1事实基座服务& 第一层服务,
    const 概念身份来源定位& 来源定位,
    const 概念世界事实支持结构定位& 支持定位,
    std::uint64_t 期望代次, 稳定编码 关系编码) {
    const auto 读取 = 第一层服务.读取所有者范围当前关系(
        {L1所有者范围CRUD合同版本, 关系编码});
    if (读取.读取事实代次 != 期望代次)
        return {L2结构状态::事实代次漂移, 读取.读取事实代次};
    if (读取.状态 != L1所有者范围读取状态::成功 || !读取.事实)
        return {映射历史读取状态(读取.状态), 读取.读取事实代次};
    const auto* 关系 = std::get_if<L1所有者范围关系事实>(&*读取.事实);
    if (读取.合同版本 != L1所有者范围CRUD合同版本
        || 读取.查询编码 != 关系编码 || !关系
        || 关系->编码 != 关系编码 || 关系->写入所有者 != 来源定位.所有者
        || 关系->关系类型节点 != 支持定位.支持关系类型
        || 关系->退出事实代次)
        return {L2结构状态::内部不一致, 读取.读取事实代次};
    return 读取概念支持单项(第一层服务, 来源定位, 支持定位,
        L2读取类别::当前, 0, 期望代次, 关系->源节点,
        std::nullopt, 关系编码);
}

// 诊断责任：无适用错误分支；退出后的六项内部事实必须同代关闭且保留历史。
std::optional<L2概念世界事实支持关系事实> 重建已退出概念支持事实(
    const L1事实基座服务& 第一层服务,
    const 概念身份来源定位& 来源定位,
    const 概念世界事实支持内部事实& 旧事实,
    std::uint64_t 截止, std::uint64_t 期望当前代次) noexcept {
    const std::vector<稳定编码> 编码{
        旧事实.支持节点, 旧事实.登记关系, 旧事实.公开事实.关系稳定编码,
        旧事实.世界事实值, 旧事实.建立依据值, 旧事实.发布证据值};
    for (const auto 项 : 编码) {
        const auto 读取 = 第一层服务.读取所有者范围历史事实(
            {L1所有者范围CRUD合同版本, 项});
        if (读取.状态 != L1所有者范围读取状态::成功
            || 读取.合同版本 != L1所有者范围CRUD合同版本
            || 读取.查询编码 != 项 || 读取.读取事实代次 != 期望当前代次
            || !读取.事实) return std::nullopt;
        const bool 完整 = std::visit([&](const auto& 事实) {
            return 事实.编码 == 项 && 事实.写入所有者 == 来源定位.所有者
                && 事实.创建事实代次 != 0
                && 事实.退出事实代次 == 截止;
        }, *读取.事实);
        if (!完整) return std::nullopt;
    }
    auto 公开 = 旧事实.公开事实;
    公开.生命周期.退出事实代次 = 截止;
    return 公开;
}

// 诊断责任：无适用错误分支；首次材料按首次截止历史重建新项并按退出见证重建旧项。
L2概念世界事实支持写入结果 重建概念支持首次结果(
    const L1事实基座服务& 第一层服务,
    const 概念身份来源定位& 来源定位,
    const 概念世界事实支持结构定位& 支持定位,
    const L1所有者范围写集请求& 写集,
    const L1所有者范围写入结果& 首次结果,
    L2结构状态 返回状态,
    const std::optional<L2概念世界事实支持关系事实>& 新关系,
    const std::optional<概念世界事实支持内部事实>& 旧关系,
    std::uint64_t 当前代次) {
    const auto 截止 = 首次结果.事实代次;
    if (首次结果.状态 != L1所有者范围写入状态::成功
        || 首次结果.合同版本 != L1所有者范围CRUD合同版本
        || 首次结果.所有者 != 来源定位.所有者
        || 首次结果.写入幂等身份 != 写集.写入幂等身份
        || 截止 == 0 || !首次结果.是否形成内存权威发布
        || 首次结果.重试边界 != L1所有者范围重试边界::不适用)
        return 形成概念支持写入失败(L2结构状态::内部不一致, 截止);
    const auto 映射 = 读取概念支持写入映射(首次结果, 新关系.has_value());
    if (!映射) return 形成概念支持写入失败(
        L2结构状态::内部不一致, 截止);
    std::optional<L2概念世界事实支持关系事实> 当前关系;
    std::optional<L2概念世界事实支持关系事实> 已退出关系;
    if (新关系) {
        const auto 读取 = 读取概念支持单项(第一层服务, 来源定位,
            支持定位, L2读取类别::历史, 截止, 当前代次, 映射->节点,
            映射->登记关系, 映射->支持关系);
        if (读取.状态 != L2结构状态::已读取 || !读取.事实
            || 读取.事实->公开事实.世界事实 != 新关系->世界事实
            || 读取.事实->公开事实.概念 != 新关系->概念
            || 读取.事实->公开事实.建立依据材料 != 新关系->建立依据材料
            || 读取.事实->公开事实.发布证据材料 != 新关系->发布证据材料)
            return 形成概念支持写入失败(
                读取.状态 == L2结构状态::已读取
                    ? L2结构状态::内部不一致 : 读取.状态,
                读取.截止);
        当前关系 = std::move(读取.事实->公开事实);
    }
    if (旧关系) {
        已退出关系 = 重建已退出概念支持事实(
            第一层服务, 来源定位, *旧关系, 截止, 当前代次);
        if (!已退出关系)
            return 形成概念支持写入失败(
                L2结构状态::内部不一致, 截止);
    }
    L2概念世界事实支持写入结果 结果{{L2结构合同版本,
        返回状态, 截止, 截止}, std::move(当前关系), std::move(已退出关系)};
    return 结果.成功() ? 结果
        : 形成概念支持写入失败(L2结构状态::内部不一致, 截止);
}

struct 当前概念图节点 final {
    L2概念身份 概念;
    L2概念身份 本体根;
};

struct 当前概念图边 final {
    稳定编码 关系;
    L2概念身份 上位;
    L2概念身份 下位;
};

struct 当前概念图 final {
    std::vector<L2概念身份> 本体根;
    std::vector<当前概念图节点> 普通概念;
    std::vector<当前概念图边> 边;
};

struct 当前概念图读取值 final {
    L2结构状态 状态 = L2结构状态::入口拒绝;
    std::uint64_t 截止 = 0;
    std::optional<当前概念图> 图;
};

struct 概念端点读取值 final {
    L2结构状态 状态 = L2结构状态::入口拒绝;
    std::uint64_t 截止 = 0;
    std::optional<L2概念身份> 本体根;
    bool 是本体根 = false;
};

// 诊断责任：无适用错误分支；只校验值式图的同根、多父、无环和唯一根可达。
bool 当前概念图完整(const 当前概念图& 图, bool 允许一个候选零编码) {
    const auto 根索引 = [&图](L2概念身份 身份) -> std::optional<std::size_t> {
        for (std::size_t 索引 = 0; 索引 < 图.本体根.size(); ++索引)
            if (图.本体根[索引] == 身份) return 索引;
        return std::nullopt;
    };
    const auto 节点索引 = [&图](L2概念身份 身份) -> std::optional<std::size_t> {
        for (std::size_t 索引 = 0; 索引 < 图.普通概念.size(); ++索引)
            if (图.普通概念[索引].概念 == 身份) return 索引;
        return std::nullopt;
    };
    for (std::size_t 索引 = 0; 索引 < 图.本体根.size(); ++索引) {
        if (!有效(图.本体根[索引].值)) return false;
        for (std::size_t 前项 = 0; 前项 < 索引; ++前项)
            if (图.本体根[前项] == 图.本体根[索引]) return false;
    }
    for (std::size_t 索引 = 0; 索引 < 图.普通概念.size(); ++索引) {
        const auto& 节点 = 图.普通概念[索引];
        if (!有效(节点.概念.值) || !有效(节点.本体根.值)
            || 节点.概念 == 节点.本体根 || !根索引(节点.本体根)) return false;
        for (std::size_t 前项 = 0; 前项 < 索引; ++前项)
            if (图.普通概念[前项].概念 == 节点.概念) return false;
    }
    std::size_t 零编码数 = 0;
    for (std::size_t 索引 = 0; 索引 < 图.边.size(); ++索引) {
        const auto& 边 = 图.边[索引];
        if (!有效(边.上位.值) || !有效(边.下位.值) || 边.上位 == 边.下位
            || !节点索引(边.下位)) return false;
        if (!有效(边.关系)) {
            ++零编码数;
            if (!允许一个候选零编码 || 零编码数 > 1) return false;
        }
        for (std::size_t 前项 = 0; 前项 < 索引; ++前项) {
            const auto& 已有 = 图.边[前项];
            if ((有效(边.关系) && 已有.关系 == 边.关系)
                || (已有.上位 == 边.上位 && 已有.下位 == 边.下位)) return false;
        }
        const auto 下位索引 = *节点索引(边.下位);
        if (const auto 上位索引 = 节点索引(边.上位)) {
            if (图.普通概念[*上位索引].本体根
                != 图.普通概念[下位索引].本体根) return false;
        } else if (!根索引(边.上位)
            || 边.上位 != 图.普通概念[下位索引].本体根) return false;
    }
    for (const auto& 节点 : 图.普通概念) {
        bool 有父 = false;
        for (const auto& 边 : 图.边) if (边.下位 == 节点.概念) {
            有父 = true;
            break;
        }
        if (!有父) return false;
    }
    std::vector<std::uint8_t> 访问状态(图.普通概念.size(), 0);
    const auto 访问 = [&](auto&& 自身, std::size_t 索引) -> bool {
        if (访问状态[索引] == 1) return false;
        if (访问状态[索引] == 2) return true;
        访问状态[索引] = 1;
        const auto& 节点 = 图.普通概念[索引];
        for (const auto& 边 : 图.边) {
            if (边.下位 != 节点.概念) continue;
            if (const auto 上位索引 = 节点索引(边.上位)) {
                if (!自身(自身, *上位索引)) return false;
            } else if (边.上位 != 节点.本体根) {
                return false;
            }
        }
        访问状态[索引] = 2;
        return true;
    };
    for (std::size_t 索引 = 0; 索引 < 图.普通概念.size(); ++索引)
        if (!访问(访问, 索引)) return false;
    return true;
}

} // namespace 海中鱼巣::L2概念结构内部

export namespace 海中鱼巣 {

// 诊断责任：向上送出；交付、端口或实例绑定不成立时返回空 optional。
std::optional<L2概念所有者交付> 尝试形成L2概念所有者交付(
    const L1事实基座服务& 第一层读取服务,
    L1所有者范围交付&& 原始交付) noexcept {
    try {
        const auto& 建立 = 原始交付.建立结果;
        const bool 首次完整 = 建立.状态 == L1所有者范围管理状态::成功
            && 建立.是否形成内存权威发布
            && 建立.重试边界 == L1所有者范围重试边界::不适用;
        const bool 重复完整 = 建立.状态 == L1所有者范围管理状态::精确重复
            && !建立.是否形成内存权威发布
            && 建立.重试边界
                == L1所有者范围重试边界::原幂等身份读回收敛;
        if (建立.合同版本 != L1所有者范围CRUD合同版本
            || 建立.建立幂等身份 != 概念所有者建立身份
            || 建立.事实代次 == 0 || (!首次完整 && !重复完整)
            || !建立.所有者事实 || !原始交付.写入端口
            || 建立.所有者事实->范围种类
                != L1所有者范围种类::独占结构范围
            || 建立.所有者事实->创建事实代次 == 0
            || 建立.所有者事实->退出事实代次
            || 原始交付.写入端口->所有者身份()
                != 建立.所有者事实->所有者
            || !原始交付.写入端口->有效()
            || !原始交付.写入端口->绑定于(第一层读取服务))
            return std::nullopt;
        const auto 当前 = 第一层读取服务.读取当前结构所有者(
            {L1所有者范围CRUD合同版本, 建立.所有者事实->所有者});
        if (当前.状态 != L1所有者范围读取状态::成功
            || 当前.合同版本 != L1所有者范围CRUD合同版本
            || 当前.查询所有者 != 建立.所有者事实->所有者
            || !当前.所有者事实 || *当前.所有者事实 != *建立.所有者事实)
            return std::nullopt;
        return L2概念所有者交付{std::move(*原始交付.写入端口)};
    } catch (...) {
        return std::nullopt;
    }
}

class L2概念结构服务 final {
public:
    // 诊断责任：向上送出；交付无效或三份私有登记失败时抛出构造异常。
    explicit L2概念结构服务(
        const L1事实基座服务& 第一层服务,
        L2概念所有者交付&& 概念所有者交付)
        : 第一层服务_(第一层服务),
          第一层写入端口_(验证并移动交付(第一层服务, 概念所有者交付)),
          身份来源定位_(L2概念结构内部::初始化概念身份来源(
              第一层服务_, 第一层写入端口_)),
          本体结构定位_(L2概念结构内部::初始化概念本体结构(
              第一层服务_, 第一层写入端口_)),
          普通结构定位_(L2概念结构内部::初始化普通概念结构(
              第一层服务_, 第一层写入端口_, 身份来源定位_,
              本体结构定位_)) {}

    // 诊断责任：向上送出；完整构造只借入七份同生命周期只读服务引用。
    L2概念结构服务(
        const L1事实基座服务& 第一层服务,
        const 不可变材料服务& 材料服务,
        const L2场景结构服务& 场景服务,
        const L2存在结构服务& 存在服务,
        const L2特征结构服务& 特征服务,
        const L2状态结构服务& 状态服务,
        const L2动态结构服务& 动态服务,
        const L2因果结构服务& 因果服务,
        L2概念所有者交付&& 概念所有者交付)
        : L2概念结构服务(第一层服务, std::move(概念所有者交付)) {
        普通概念依赖_ = {&材料服务, &场景服务, &存在服务, &特征服务,
            &状态服务, &动态服务, &因果服务};
        支持结构定位_ = L2概念结构内部::初始化概念世界事实支持结构(
            第一层服务_, 第一层写入端口_, 身份来源定位_,
            本体结构定位_, 普通结构定位_);
    }

    L2概念结构服务() = delete;
    L2概念结构服务(const L2概念结构服务&) = delete;
    L2概念结构服务& operator=(const L2概念结构服务&) = delete;
    L2概念结构服务(L2概念结构服务&&) = delete;
    L2概念结构服务& operator=(L2概念结构服务&&) = delete;

    // 诊断责任：向上送出；全部失败只经结构化结果返回，发布未知保留原请求重放。
    L2概念本体根写入结果 建立概念本体根(
        const L2概念本体根建立请求& 请求) {
        if (!L2概念本体根建立请求有效(请求)
            || L2概念结构内部::是概念保留登记幂等身份(请求.幂等身份))
            return L2概念结构内部::形成根写入失败(L2结构状态::入口拒绝);
        try {
            std::lock_guard<std::mutex> 锁(概念写入锁_);
            const auto 预读 = L2概念结构内部::读取全部当前概念本体根核心(
                第一层服务_, 身份来源定位_, 本体结构定位_,
                {请求.请求头});
            if (预读.成功()) {
                const L2概念本体根事实* 目标角色根 = nullptr;
                for (const auto& 根 : 预读.本体根) {
                    if (根.首次幂等身份 == 请求.幂等身份) {
                        if (根.根角色.角色 != 请求.角色
                            || 根.首次期望事实代次
                                != 请求.请求头.期望事实代次)
                            return L2概念结构内部::形成根写入失败(
                                L2结构状态::幂等冲突,
                                预读.结果头.事实截止代次);
                    }
                    if (根.根角色.角色 == 请求.角色) 目标角色根 = &根;
                }
                if (目标角色根) {
                    if (目标角色根->首次幂等身份 != 请求.幂等身份)
                        return L2概念结构内部::形成根写入失败(
                            L2结构状态::引用冲突,
                            预读.结果头.事实截止代次);
                    const auto 首次截止 =
                        目标角色根->根角色.生命周期.创建事实代次;
                    L2概念本体根写入结果 结果{{L2结构合同版本,
                        L2结构状态::精确重复, 首次截止, 首次截止},
                        *目标角色根};
                    return 结果.成功() ? 结果
                        : L2概念结构内部::形成根写入失败(
                            L2结构状态::内部不一致, 首次截止);
                }
            } else if (预读.结果头.状态 != L2结构状态::事实代次漂移) {
                return L2概念结构内部::形成根写入失败(
                    预读.结果头.状态, 预读.结果头.事实截止代次);
            }

            const auto 写集 = L2概念结构内部::形成本体根写集(
                请求, 身份来源定位_, 本体结构定位_);
            const auto 写入 = 第一层写入端口_.提交所有者范围中性写集(写集);
            if (写入.合同版本 != L1所有者范围CRUD合同版本
                || 写入.所有者 != 第一层写入端口_.所有者身份()
                || 写入.写入幂等身份 != 写集.写入幂等身份)
                return L2概念结构内部::形成根写入失败(
                    L2结构状态::内部不一致, 写入.事实代次);
            const auto 状态 = L2概念结构内部::映射写入状态(写入.状态);
            if (状态 != L2结构状态::已提交
                && 状态 != L2结构状态::精确重复) {
                if (!写入.新编码映射.empty() || 写入.是否形成内存权威发布)
                    return L2概念结构内部::形成根写入失败(
                        L2结构状态::内部不一致, 写入.事实代次);
                return L2概念结构内部::形成根写入失败(状态, 写入.事实代次);
            }
            return L2概念结构内部::重建首次本体根(
                第一层服务_, 第一层写入端口_, 身份来源定位_,
                本体结构定位_, 请求, 写集, 写入);
        } catch (const std::bad_alloc&) {
            return L2概念结构内部::形成根写入失败(L2结构状态::资源失败);
        } catch (const std::length_error&) {
            return L2概念结构内部::形成根写入失败(L2结构状态::资源失败);
        } catch (...) {
            return L2概念结构内部::形成根写入失败(
                L2结构状态::内部不一致);
        }
    }

    // 诊断责任：向上送出；单根读取复用一次全组许可且同代缺失为内部不一致。
    L2当前概念本体根读取结果 按角色读取当前概念本体根(
        const L2当前概念本体根读取请求& 请求) const {
        const auto 失败 = [](L2结构状态 状态, std::uint64_t 截止 = 0) {
            return L2当前概念本体根读取结果{
                {L2结构合同版本, 状态, 截止, std::nullopt}, std::nullopt};
        };
        if (!L2当前概念本体根读取请求有效(请求))
            return 失败(L2结构状态::入口拒绝);
        try {
            const auto 全组 = L2概念结构内部::读取全部当前概念本体根核心(
                第一层服务_, 身份来源定位_, 本体结构定位_,
                {请求.请求头});
            if (!全组.成功())
                return 失败(全组.结果头.状态, 全组.结果头.事实截止代次);
            const L2概念本体根事实* 找到 = nullptr;
            for (const auto& 根 : 全组.本体根) {
                if (根.根角色.角色 != 请求.角色) continue;
                if (找到)
                    return 失败(L2结构状态::内部不一致,
                        全组.结果头.事实截止代次);
                找到 = &根;
            }
            if (!找到)
                return 失败(L2结构状态::内部不一致,
                    全组.结果头.事实截止代次);
            L2当前概念本体根读取结果 结果{全组.结果头, *找到};
            return 结果.成功() && 结果.本体根->根角色.角色 == 请求.角色
                ? 结果 : 失败(L2结构状态::内部不一致,
                    全组.结果头.事实截止代次);
        } catch (const std::bad_alloc&) {
            return 失败(L2结构状态::资源失败);
        } catch (const std::length_error&) {
            return 失败(L2结构状态::资源失败);
        } catch (...) {
            return 失败(L2结构状态::内部不一致);
        }
    }

    // 诊断责任：向上送出；只返回一次 L1 同许可形成的当前根组结构化结果。
    L2当前概念本体根组读取结果 读取全部当前概念本体根(
        const L2当前概念本体根组读取请求& 请求) const {
        if (!L2当前概念本体根组读取请求有效(请求))
            return L2概念结构内部::形成根组失败(L2结构状态::入口拒绝);
        try {
            return L2概念结构内部::读取全部当前概念本体根核心(
                第一层服务_, 身份来源定位_, 本体结构定位_, 请求);
        } catch (const std::bad_alloc&) {
            return L2概念结构内部::形成根组失败(L2结构状态::资源失败);
        } catch (const std::length_error&) {
            return L2概念结构内部::形成根组失败(L2结构状态::资源失败);
        } catch (...) {
            return L2概念结构内部::形成根组失败(L2结构状态::内部不一致);
        }
    }

    // 诊断责任：向上送出；新增建立一条完整世界事实到概念支持关系。
    L2概念世界事实支持写入结果 新增概念世界事实支持关系(
        const L2概念世界事实支持新增请求& 请求) {
        using namespace L2概念结构内部;
        if (!L2概念世界事实支持新增请求有效(请求)
            || 是概念保留登记幂等身份(请求.幂等身份))
            return 形成概念支持写入失败(L2结构状态::入口拒绝);
        L2概念世界事实支持关系事实 新关系{请求.世界事实, 请求.概念,
            请求.建立依据材料, 请求.发布证据材料, {}, {}};
        return 执行概念世界事实支持写入(请求.请求头.期望事实代次,
            请求.幂等身份, std::move(新关系), std::nullopt);
    }

    // 诊断责任：向上送出；替换保持概念端点并在同一写集中退旧建新。
    L2概念世界事实支持写入结果 替换概念世界事实支持关系(
        const L2概念世界事实支持替换请求& 请求) {
        using namespace L2概念结构内部;
        if (!L2概念世界事实支持替换请求有效(请求)
            || 是概念保留登记幂等身份(请求.幂等身份))
            return 形成概念支持写入失败(L2结构状态::入口拒绝);
        L2概念世界事实支持关系事实 新关系{请求.新世界事实, 请求.概念,
            请求.新建立依据材料, 请求.新发布证据材料, {}, {}};
        L2概念世界事实支持关系事实 旧关系{请求.旧世界事实, 请求.概念,
            {}, {}, 请求.旧关系稳定编码, {}};
        return 执行概念世界事实支持写入(请求.请求头.期望事实代次,
            请求.幂等身份, std::move(新关系), std::move(旧关系));
    }

    // 诊断责任：向上送出；退出关闭支持项全部内部当前事实但不修改端点与材料。
    L2概念世界事实支持写入结果 退出概念世界事实支持关系(
        const L2概念世界事实支持退出请求& 请求) {
        using namespace L2概念结构内部;
        if (!L2概念世界事实支持退出请求有效(请求)
            || 是概念保留登记幂等身份(请求.幂等身份))
            return 形成概念支持写入失败(L2结构状态::入口拒绝);
        L2概念世界事实支持关系事实 旧关系{请求.世界事实, 请求.概念,
            {}, {}, 请求.关系稳定编码, {}};
        return 执行概念世界事实支持写入(请求.请求头.期望事实代次,
            请求.幂等身份, std::nullopt, std::move(旧关系));
    }

    // 诊断责任：向上送出；按概念读取同截止直接支持世界事实组。
    L2概念世界事实支持组读取结果 按概念读取世界事实支持关系组(
        const L2按概念世界事实支持组读取请求& 请求) const {
        using namespace L2概念结构内部;
        if (!L2按概念世界事实支持组读取请求有效(请求))
            return 形成概念支持组失败(L2结构状态::入口拒绝,
                请求.读取类别, 请求.历史截止事实代次);
        if (!支持结构定位_ || !普通概念依赖_.完整())
            return 形成概念支持组失败(L2结构状态::未实现,
                请求.读取类别, 请求.历史截止事实代次);
        try {
            const auto 端点 = 读取概念端点归属(请求.概念, 请求.读取类别,
                请求.历史截止事实代次, 请求.请求头.期望事实代次);
            if (端点.状态 != L2结构状态::已读取 || !端点.本体根)
                return 形成概念支持组失败(端点.状态,
                    请求.读取类别, 请求.历史截止事实代次, 端点.截止);
            auto 结果 = 读取概念支持关系组核心(第一层服务_,
                身份来源定位_, *支持结构定位_, 请求.读取类别,
                请求.历史截止事实代次, 请求.请求头.期望事实代次,
                请求.概念, std::nullopt, 0, 请求.数量预算);
            if (!结果.成功()) return 结果;
            for (const auto& 关系 : 结果.关系组) {
                const auto 状态 = 校验概念支持公开引用(关系,
                    请求.读取类别, 请求.历史截止事实代次,
                    请求.请求头.期望事实代次);
                if (状态 != L2结构状态::已读取)
                    return 形成概念支持组失败(状态, 请求.读取类别,
                        请求.历史截止事实代次, 结果.结果头.事实截止代次);
            }
            return 结果;
        } catch (const std::bad_alloc&) {
            return 形成概念支持组失败(L2结构状态::资源失败,
                请求.读取类别, 请求.历史截止事实代次);
        } catch (const std::length_error&) {
            return 形成概念支持组失败(L2结构状态::资源失败,
                请求.读取类别, 请求.历史截止事实代次);
        } catch (...) {
            return 形成概念支持组失败(L2结构状态::内部不一致,
                请求.读取类别, 请求.历史截止事实代次);
        }
    }

    // 诊断责任：向上送出；按世界事实有界扫描登记根并返回同截止概念组。
    L2概念世界事实支持组读取结果 按世界事实读取概念支持关系组(
        const L2按世界事实支持概念组读取请求& 请求) const {
        using namespace L2概念结构内部;
        if (!L2按世界事实支持概念组读取请求有效(请求))
            return 形成概念支持组失败(L2结构状态::入口拒绝,
                请求.读取类别, 请求.历史截止事实代次);
        if (!支持结构定位_ || !普通概念依赖_.完整())
            return 形成概念支持组失败(L2结构状态::未实现,
                请求.读取类别, 请求.历史截止事实代次);
        try {
            const auto 世界状态 = 校验世界事实公开引用(请求.世界事实,
                请求.读取类别, 请求.历史截止事实代次,
                请求.请求头.期望事实代次);
            if (世界状态 != L2结构状态::已读取)
                return 形成概念支持组失败(世界状态, 请求.读取类别,
                    请求.历史截止事实代次,
                    请求.读取类别 == L2读取类别::历史
                        ? 请求.历史截止事实代次
                        : 请求.请求头.期望事实代次);
            auto 结果 = 读取概念支持关系组核心(第一层服务_,
                身份来源定位_, *支持结构定位_, 请求.读取类别,
                请求.历史截止事实代次, 请求.请求头.期望事实代次,
                std::nullopt, 请求.世界事实, 请求.扫描预算, 请求.数量预算);
            if (!结果.成功()) return 结果;
            for (const auto& 关系 : 结果.关系组) {
                const auto 状态 = 校验概念支持公开引用(关系,
                    请求.读取类别, 请求.历史截止事实代次,
                    请求.请求头.期望事实代次);
                if (状态 != L2结构状态::已读取)
                    return 形成概念支持组失败(状态, 请求.读取类别,
                        请求.历史截止事实代次, 结果.结果头.事实截止代次);
            }
            return 结果;
        } catch (const std::bad_alloc&) {
            return 形成概念支持组失败(L2结构状态::资源失败,
                请求.读取类别, 请求.历史截止事实代次);
        } catch (const std::length_error&) {
            return 形成概念支持组失败(L2结构状态::资源失败,
                请求.读取类别, 请求.历史截止事实代次);
        } catch (...) {
            return 形成概念支持组失败(L2结构状态::内部不一致,
                请求.读取类别, 请求.历史截止事实代次);
        }
    }

    // 诊断责任：向上送出；合法迁移复用唯一 owner、概念写锁和原请求幂等收敛。
    L2概念治理生命周期写入结果 迁移概念治理生命周期(
        const L2概念治理生命周期迁移请求& 请求) {
        using namespace L2概念结构内部;
        if (!L2概念治理生命周期迁移请求有效(请求)
            || 是概念保留登记幂等身份(请求.幂等身份))
            return 形成概念生命周期写入失败(L2结构状态::入口拒绝);
        try {
            std::lock_guard<std::mutex> 锁(概念写入锁_);
            const auto 写集 = 形成概念治理生命周期迁移写集(
                请求, 普通结构定位_);
            const auto 既有幂等 = 第一层写入端口_.读取首次写入材料(
                {L1所有者范围首次写入读取合同版本, 写集.写入幂等身份});
            const bool 幂等头完整 = 既有幂等.合同版本
                    == L1所有者范围首次写入读取合同版本
                && 既有幂等.所有者 == 第一层写入端口_.所有者身份()
                && 既有幂等.写入幂等身份 == 写集.写入幂等身份
                && 既有幂等.读取事实代次 != 0;
            if (!幂等头完整)
                return 形成概念生命周期写入失败(
                    L2结构状态::内部不一致, 既有幂等.读取事实代次);
            if (既有幂等.状态 == L1所有者范围读取状态::成功) {
                if (!既有幂等.首次规范化写集 || !既有幂等.首次写入结果)
                    return 形成概念生命周期写入失败(
                        L2结构状态::内部不一致, 既有幂等.读取事实代次);
                if (*既有幂等.首次规范化写集 != 写集)
                    return 形成概念生命周期写入失败(
                        L2结构状态::幂等冲突, 既有幂等.读取事实代次);
                const auto& 首次结果 = *既有幂等.首次写入结果;
                if (首次结果.事实代次 == 0
                    || 首次结果.事实代次 <= 请求.请求头.期望事实代次
                    || 首次结果.事实代次 > 既有幂等.读取事实代次)
                    return 形成概念生命周期写入失败(
                        L2结构状态::内部不一致, 既有幂等.读取事实代次);
                return 重建概念治理生命周期首次结果(第一层服务_,
                    身份来源定位_, 本体结构定位_, 普通结构定位_, 写集,
                    首次结果, L2结构状态::精确重复, 请求,
                    既有幂等.读取事实代次);
            }
            if (既有幂等.首次规范化写集 || 既有幂等.首次写入结果)
                return 形成概念生命周期写入失败(
                    L2结构状态::内部不一致, 既有幂等.读取事实代次);
            if (既有幂等.状态 != L1所有者范围读取状态::未找到)
                return 形成概念生命周期写入失败(
                    映射历史读取状态(既有幂等.状态),
                    既有幂等.读取事实代次);

            if (既有幂等.读取事实代次 == 请求.请求头.期望事实代次) {
                const auto 当前 = 读取概念治理生命周期({请求.请求头,
                    L2概念治理生命周期合同版本, L2读取类别::当前,
                    请求.概念, 0});
                if (!当前.成功() || !当前.生命周期)
                    return 形成概念生命周期写入失败(
                        当前.结果头.状态 == L2结构状态::已读取
                            ? L2结构状态::内部不一致 : 当前.结果头.状态,
                        当前.结果头.事实截止代次);
                if (当前.生命周期->事实稳定编码
                        != 请求.当前生命周期事实稳定编码
                    || 当前.生命周期->状态 != 请求.当前状态)
                    return 形成概念生命周期写入失败(
                        L2结构状态::引用冲突,
                        当前.结果头.事实截止代次);
            }

            const auto 写入 = 第一层写入端口_.提交所有者范围中性写集(写集);
            if (写入.合同版本 != L1所有者范围CRUD合同版本
                || 写入.所有者 != 第一层写入端口_.所有者身份()
                || 写入.写入幂等身份 != 写集.写入幂等身份)
                return 形成概念生命周期写入失败(
                    L2结构状态::内部不一致, 写入.事实代次);
            const auto 写入状态 = 映射写入状态(写入.状态);
            if (写入状态 != L2结构状态::已提交
                && 写入状态 != L2结构状态::精确重复) {
                if (!写入.新编码映射.empty() || 写入.是否形成内存权威发布)
                    return 形成概念生命周期写入失败(
                        L2结构状态::内部不一致, 写入.事实代次);
                return 形成概念生命周期写入失败(
                    写入状态, 写入.事实代次);
            }
            const bool 提交首次完整 = 写入.状态 == L1所有者范围写入状态::成功
                && 写入.是否形成内存权威发布
                && 写入.重试边界 == L1所有者范围重试边界::不适用;
            const bool 提交重复完整 = 写入.状态
                    == L1所有者范围写入状态::精确重复
                && !写入.是否形成内存权威发布
                && 写入.重试边界
                    == L1所有者范围重试边界::原幂等身份读回收敛;
            if (写入.事实代次 == 0
                || 写入.事实代次 <= 请求.请求头.期望事实代次
                || (!提交首次完整 && !提交重复完整))
                return 形成概念生命周期写入失败(
                    L2结构状态::内部不一致, 写入.事实代次);

            const auto 首次读取 = 第一层写入端口_.读取首次写入材料(
                {L1所有者范围首次写入读取合同版本, 写集.写入幂等身份});
            if (首次读取.状态 != L1所有者范围读取状态::成功)
                return 形成概念生命周期写入失败(
                    首次读取.状态 == L1所有者范围读取状态::资源失败
                        ? L2结构状态::资源失败 : L2结构状态::内部不一致,
                    首次读取.读取事实代次);
            if (首次读取.合同版本 != L1所有者范围首次写入读取合同版本
                || 首次读取.所有者 != 第一层写入端口_.所有者身份()
                || 首次读取.写入幂等身份 != 写集.写入幂等身份
                || 首次读取.读取事实代次 == 0
                || !首次读取.首次规范化写集 || !首次读取.首次写入结果
                || *首次读取.首次规范化写集 != 写集)
                return 形成概念生命周期写入失败(
                    L2结构状态::内部不一致, 首次读取.读取事实代次);
            const auto& 首次结果 = *首次读取.首次写入结果;
            if (首次结果.状态 != L1所有者范围写入状态::成功
                || 首次结果.合同版本 != L1所有者范围CRUD合同版本
                || 首次结果.所有者 != 第一层写入端口_.所有者身份()
                || 首次结果.写入幂等身份 != 写集.写入幂等身份
                || 首次结果.事实代次 == 0
                || 首次结果.事实代次 <= 请求.请求头.期望事实代次
                || 首次结果.事实代次 > 首次读取.读取事实代次
                || !首次结果.是否形成内存权威发布
                || 首次结果.重试边界 != L1所有者范围重试边界::不适用
                || 写入.事实代次 != 首次结果.事实代次
                || 写入.新编码映射 != 首次结果.新编码映射)
                return 形成概念生命周期写入失败(
                    L2结构状态::内部不一致, 首次结果.事实代次);
            return 重建概念治理生命周期首次结果(第一层服务_,
                身份来源定位_, 本体结构定位_, 普通结构定位_, 写集,
                首次结果, 写入状态, 请求, 首次读取.读取事实代次);
        } catch (const std::bad_alloc&) {
            return 形成概念生命周期写入失败(L2结构状态::资源失败);
        } catch (const std::length_error&) {
            return 形成概念生命周期写入失败(L2结构状态::资源失败);
        } catch (...) {
            return 形成概念生命周期写入失败(L2结构状态::内部不一致);
        }
    }

    // 诊断责任：向上送出；只从概念 owner 自有事实读取当前或历史生命周期。
    L2概念治理生命周期读取结果 读取概念治理生命周期(
        const L2概念治理生命周期读取请求& 请求) const {
        using namespace L2概念结构内部;
        if (!L2概念治理生命周期读取请求有效(请求))
            return 形成概念生命周期读取失败(
                请求, L2结构状态::入口拒绝);
        try {
            const auto 根组 = 读取全部当前概念本体根核心(
                第一层服务_, 身份来源定位_, 本体结构定位_, {请求.请求头});
            if (!根组.成功())
                return 形成概念生命周期读取失败(
                    请求, 根组.结果头.状态, 根组.结果头.事实截止代次);
            for (const auto& 根 : 根组.本体根)
                if (根.根概念 == 请求.概念)
                    return 形成概念生命周期读取失败(
                        请求, L2结构状态::入口拒绝,
                        根组.结果头.事实截止代次);

            const auto 重建 = 重建普通概念自有事实(第一层服务_,
                身份来源定位_, 本体结构定位_, 普通结构定位_,
                {请求.请求头, L2普通概念结构合同版本, 请求.读取类别,
                    请求.概念, 请求.读取类别 == L2读取类别::历史
                        ? 请求.历史截止事实代次 : 0});
            if (重建.状态 != L2结构状态::已读取 || !重建.事实)
                return 形成概念生命周期读取失败(
                    请求, 重建.状态 == L2结构状态::已读取
                        ? L2结构状态::内部不一致 : 重建.状态,
                    重建.截止);
            const auto 投影截止 = 请求.读取类别 == L2读取类别::历史
                ? 请求.历史截止事实代次 : 请求.请求头.期望事实代次;
            if (!L2概念治理生命周期事实截止投影完整(
                    重建.事实->治理生命周期, 投影截止)
                || 重建.事实->治理生命周期.概念 != 请求.概念
                || 重建.事实->治理生命周期.生命周期.退出事实代次)
                return 形成概念生命周期读取失败(
                    请求, L2结构状态::内部不一致, 投影截止);
            const auto 尾代次 = 读取当前事实代次(第一层服务_);
            if (尾代次.状态 != L1所有者范围读取状态::成功)
                return 形成概念生命周期读取失败(
                    请求, 尾代次.状态 == L1所有者范围读取状态::资源失败
                        ? L2结构状态::资源失败 : L2结构状态::内部不一致,
                    尾代次.事实代次);
            if (尾代次.事实代次 != 请求.请求头.期望事实代次)
                return 形成概念生命周期读取失败(
                    请求, L2结构状态::事实代次漂移, 尾代次.事实代次);
            L2概念治理生命周期读取结果 结果{{L2结构合同版本,
                L2结构状态::已读取, 投影截止, std::nullopt}, 请求.读取类别,
                请求.读取类别 == L2读取类别::历史
                    ? 请求.历史截止事实代次 : 0,
               重建.事实->治理生命周期};
            return 结果.成功() ? 结果
                : 形成概念生命周期读取失败(
                    请求, L2结构状态::内部不一致, 投影截止);
        } catch (const std::bad_alloc&) {
            return 形成概念生命周期读取失败(
                请求, L2结构状态::资源失败);
        } catch (const std::length_error&) {
            return 形成概念生命周期读取失败(
                请求, L2结构状态::资源失败);
        } catch (...) {
            return 形成概念生命周期读取失败(
                请求, L2结构状态::内部不一致);
        }
    }

    // 诊断责任：向上送出；当前与历史都先重建 owner 自有结构，再经公开依赖同截止互证。
    L2普通概念读取结果 读取普通概念(
        const L2普通概念读取请求& 请求) const {
        if (!L2普通概念读取请求有效(请求))
            return L2概念结构内部::形成普通概念读取失败(
                请求, L2结构状态::入口拒绝);
        if (!普通概念依赖_.完整())
            return L2概念结构内部::形成普通概念读取失败(
                请求, L2结构状态::未实现);
        try {
            const auto 重建 = L2概念结构内部::重建普通概念自有事实(
                第一层服务_, 身份来源定位_, 本体结构定位_,
                普通结构定位_, 请求);
            if (重建.状态 != L2结构状态::已读取 || !重建.事实)
                return L2概念结构内部::形成普通概念读取失败(
                    请求, 重建.状态, 重建.截止);
            const auto 依赖状态 = L2概念结构内部::校验普通概念公开引用(
                第一层服务_, 身份来源定位_, 本体结构定位_,
                普通结构定位_, 普通概念依赖_,
                [this](const L2世界事实引用& 引用, L2读取类别 类别,
                    std::uint64_t 历史截止, std::uint64_t 期望代次) {
                    return 校验世界事实公开引用(
                        引用, 类别, 历史截止, 期望代次);
                }, *重建.事实, 请求);
            if (依赖状态 != L2结构状态::已读取)
                return L2概念结构内部::形成普通概念读取失败(
                    请求, 依赖状态, 重建.截止);
            const auto 尾代次 = L2概念结构内部::读取当前事实代次(第一层服务_);
            if (尾代次.状态 != L1所有者范围读取状态::成功)
                return L2概念结构内部::形成普通概念读取失败(
                    请求, 尾代次.状态 == L1所有者范围读取状态::资源失败
                        ? L2结构状态::资源失败 : L2结构状态::内部不一致,
                    尾代次.事实代次);
            if (尾代次.事实代次 != 请求.请求头.期望事实代次)
                return L2概念结构内部::形成普通概念读取失败(
                    请求, L2结构状态::事实代次漂移, 尾代次.事实代次);
            const auto 截止 = 请求.读取类别 == L2读取类别::历史
                ? 请求.历史截止事实代次 : 请求.请求头.期望事实代次;
            L2普通概念读取结果 结果{{L2结构合同版本,
                L2结构状态::已读取, 截止, std::nullopt}, 请求.读取类别,
                请求.读取类别 == L2读取类别::历史
                    ? 请求.历史截止事实代次 : 0,
                *重建.事实};
            return 结果.成功() ? 结果
                : L2概念结构内部::形成普通概念读取失败(
                    请求, L2结构状态::内部不一致, 截止);
        } catch (const std::bad_alloc&) {
            return L2概念结构内部::形成普通概念读取失败(
                请求, L2结构状态::资源失败);
        } catch (const std::length_error&) {
            return L2概念结构内部::形成普通概念读取失败(
                请求, L2结构状态::资源失败);
        } catch (...) {
            return L2概念结构内部::形成普通概念读取失败(
                请求, L2结构状态::内部不一致);
        }
    }

    // 诊断责任：向上送出；新增只接受完整强类型端点并保留调用方幂等身份。
    L2概念直接上位写入结果 新增概念直接上位关系(
        const L2概念直接上位新增请求& 请求) {
        using namespace L2概念结构内部;
        if (!L2概念直接上位新增请求有效(请求)
            || 是概念保留登记幂等身份(请求.幂等身份))
            return 形成概念关系写入失败(L2结构状态::入口拒绝);
        return 执行概念直接上位关系写入(请求.请求头.期望事实代次,
            请求.幂等身份, 直接关系期望{请求.上位概念, 请求.下位概念},
            std::nullopt);
    }

    // 诊断责任：向上送出；替换在同一 owner 写集中退出旧边并建立新边。
    L2概念直接上位写入结果 替换概念直接上位关系(
        const L2概念直接上位替换请求& 请求) {
        using namespace L2概念结构内部;
        if (!L2概念直接上位替换请求有效(请求)
            || 是概念保留登记幂等身份(请求.幂等身份))
            return 形成概念关系写入失败(L2结构状态::入口拒绝);
        return 执行概念直接上位关系写入(请求.请求头.期望事实代次,
            请求.幂等身份, 直接关系期望{请求.新上位概念, 请求.下位概念},
            std::optional<std::pair<稳定编码, 直接关系期望>>{
                std::in_place, 请求.旧关系稳定编码,
                直接关系期望{请求.旧上位概念, 请求.下位概念}});
    }

    // 诊断责任：向上送出；退出只接受具名旧边且不得令普通概念失去全部父边。
    L2概念直接上位写入结果 退出概念直接上位关系(
        const L2概念直接上位退出请求& 请求) {
        using namespace L2概念结构内部;
        if (!L2概念直接上位退出请求有效(请求)
            || 是概念保留登记幂等身份(请求.幂等身份))
            return 形成概念关系写入失败(L2结构状态::入口拒绝);
        return 执行概念直接上位关系写入(请求.请求头.期望事实代次,
            请求.幂等身份, std::nullopt,
            std::optional<std::pair<稳定编码, 直接关系期望>>{
                std::in_place, 请求.关系稳定编码,
                直接关系期望{请求.上位概念, 请求.下位概念}});
    }

    // 诊断责任：向上送出；按下位端点返回同截止、稳定排序的直接上位关系组。
    L2概念直接上位组读取结果 读取概念直接上位关系组(
        const L2概念直接上位组读取请求& 请求) const {
        using namespace L2概念结构内部;
        if (!L2概念直接上位组读取请求有效(请求))
            return 形成概念关系组失败(
                L2结构状态::入口拒绝, 请求.读取类别,
                请求.历史截止事实代次);
        if (!普通概念依赖_.完整())
            return 形成概念关系组失败(
                L2结构状态::未实现, 请求.读取类别,
                请求.历史截止事实代次);
        try {
            const auto 下位 = 读取概念端点归属(请求.下位概念,
                请求.读取类别, 请求.历史截止事实代次,
                请求.请求头.期望事实代次);
            if (下位.状态 != L2结构状态::已读取 || !下位.本体根)
                return 形成概念关系组失败(
                    下位.状态, 请求.读取类别,
                    请求.历史截止事实代次, 下位.截止);
            auto 结果 = 读取直接关系组核心(第一层服务_, 身份来源定位_,
                本体结构定位_, L1所有者范围关系端点方向::目标,
                请求.下位概念, 请求.读取类别,
                请求.历史截止事实代次, 请求.请求头.期望事实代次,
                请求.数量预算);
            if (!结果.成功()) return 结果;
            if (下位.是本体根 && !结果.关系组.empty())
                return 形成概念关系组失败(L2结构状态::内部不一致,
                    请求.读取类别, 请求.历史截止事实代次,
                    结果.结果头.事实截止代次);
            for (const auto& 关系 : 结果.关系组) {
                if (关系.下位概念 != 请求.下位概念)
                    return 形成概念关系组失败(L2结构状态::内部不一致,
                        请求.读取类别, 请求.历史截止事实代次,
                        结果.结果头.事实截止代次);
                const auto 上位 = 读取概念端点归属(关系.上位概念,
                    请求.读取类别, 请求.历史截止事实代次,
                    请求.请求头.期望事实代次);
                if (上位.状态 != L2结构状态::已读取 || !上位.本体根)
                    return 形成概念关系组失败(
                        上位.状态 == L2结构状态::已读取
                            ? L2结构状态::内部不一致 : 上位.状态,
                        请求.读取类别, 请求.历史截止事实代次, 上位.截止);
                if (*上位.本体根 != *下位.本体根)
                    return 形成概念关系组失败(L2结构状态::内部不一致,
                        请求.读取类别, 请求.历史截止事实代次,
                        结果.结果头.事实截止代次);
            }
            const auto 尾代次 = 读取当前事实代次(第一层服务_);
            if (尾代次.状态 != L1所有者范围读取状态::成功)
                return 形成概念关系组失败(
                    尾代次.状态 == L1所有者范围读取状态::资源失败
                        ? L2结构状态::资源失败 : L2结构状态::内部不一致,
                    请求.读取类别, 请求.历史截止事实代次,
                    尾代次.事实代次);
            if (尾代次.事实代次 != 请求.请求头.期望事实代次)
                return 形成概念关系组失败(L2结构状态::事实代次漂移,
                    请求.读取类别, 请求.历史截止事实代次,
                    尾代次.事实代次);
            return 结果;
        } catch (const std::bad_alloc&) {
            return 形成概念关系组失败(L2结构状态::资源失败,
                请求.读取类别, 请求.历史截止事实代次);
        } catch (const std::length_error&) {
            return 形成概念关系组失败(L2结构状态::资源失败,
                请求.读取类别, 请求.历史截止事实代次);
        } catch (...) {
            return 形成概念关系组失败(L2结构状态::内部不一致,
                请求.读取类别, 请求.历史截止事实代次);
        }
    }

    // 诊断责任：向上送出；按上位端点返回同截止、稳定排序的直接下位关系组。
    L2概念直接下位组读取结果 读取概念直接下位关系组(
        const L2概念直接下位组读取请求& 请求) const {
        using namespace L2概念结构内部;
        if (!L2概念直接下位组读取请求有效(请求))
            return 形成概念关系组失败(
                L2结构状态::入口拒绝, 请求.读取类别,
                请求.历史截止事实代次);
        if (!普通概念依赖_.完整())
            return 形成概念关系组失败(
                L2结构状态::未实现, 请求.读取类别,
                请求.历史截止事实代次);
        try {
            const auto 上位 = 读取概念端点归属(请求.上位概念,
                请求.读取类别, 请求.历史截止事实代次,
                请求.请求头.期望事实代次);
            if (上位.状态 != L2结构状态::已读取 || !上位.本体根)
                return 形成概念关系组失败(
                    上位.状态, 请求.读取类别,
                    请求.历史截止事实代次, 上位.截止);
            auto 结果 = 读取直接关系组核心(第一层服务_, 身份来源定位_,
                本体结构定位_, L1所有者范围关系端点方向::源,
                请求.上位概念, 请求.读取类别,
                请求.历史截止事实代次, 请求.请求头.期望事实代次,
                请求.数量预算);
            if (!结果.成功()) return 结果;
            for (const auto& 关系 : 结果.关系组) {
                if (关系.上位概念 != 请求.上位概念)
                    return 形成概念关系组失败(L2结构状态::内部不一致,
                        请求.读取类别, 请求.历史截止事实代次,
                        结果.结果头.事实截止代次);
                const auto 下位 = 读取概念端点归属(关系.下位概念,
                    请求.读取类别, 请求.历史截止事实代次,
                    请求.请求头.期望事实代次);
                if (下位.状态 != L2结构状态::已读取 || !下位.本体根
                    || 下位.是本体根)
                    return 形成概念关系组失败(
                        下位.状态 == L2结构状态::已读取
                            ? L2结构状态::内部不一致 : 下位.状态,
                        请求.读取类别, 请求.历史截止事实代次, 下位.截止);
                if (*上位.本体根 != *下位.本体根)
                    return 形成概念关系组失败(L2结构状态::内部不一致,
                        请求.读取类别, 请求.历史截止事实代次,
                        结果.结果头.事实截止代次);
            }
            const auto 尾代次 = 读取当前事实代次(第一层服务_);
            if (尾代次.状态 != L1所有者范围读取状态::成功)
                return 形成概念关系组失败(
                    尾代次.状态 == L1所有者范围读取状态::资源失败
                        ? L2结构状态::资源失败 : L2结构状态::内部不一致,
                    请求.读取类别, 请求.历史截止事实代次,
                    尾代次.事实代次);
            if (尾代次.事实代次 != 请求.请求头.期望事实代次)
                return 形成概念关系组失败(L2结构状态::事实代次漂移,
                    请求.读取类别, 请求.历史截止事实代次,
                    尾代次.事实代次);
            return 结果;
        } catch (const std::bad_alloc&) {
            return 形成概念关系组失败(L2结构状态::资源失败,
                请求.读取类别, 请求.历史截止事实代次);
        } catch (const std::length_error&) {
            return 形成概念关系组失败(L2结构状态::资源失败,
                请求.读取类别, 请求.历史截止事实代次);
        } catch (...) {
            return 形成概念关系组失败(L2结构状态::内部不一致,
                请求.读取类别, 请求.历史截止事实代次);
        }
    }

    // 诊断责任：向上送出；唯一性预读、owner 原子提交和首次历史读回共用同一写锁。
    L2普通概念写入结果 建立普通概念(
        const L2普通概念建立请求& 请求) {
        using namespace L2概念结构内部;
        if (!L2普通概念建立请求有效(请求)
            || 是概念保留登记幂等身份(请求.幂等身份))
            return 形成普通概念写入失败(L2结构状态::入口拒绝);
        if (!普通概念依赖_.完整())
            return 形成普通概念写入失败(L2结构状态::未实现);
        if (!普通概念写集数量可表示(请求))
            return 形成普通概念写入失败(L2结构状态::数量预算不足);
        try {
            std::lock_guard<std::mutex> 锁(概念写入锁_);
            const auto 写集 = 形成普通概念写集(请求, 身份来源定位_,
                本体结构定位_, 普通结构定位_);
            const auto 既有幂等 = 第一层写入端口_.读取首次写入材料(
                {L1所有者范围首次写入读取合同版本, 写集.写入幂等身份});
            const bool 幂等头完整 = 既有幂等.合同版本
                    == L1所有者范围首次写入读取合同版本
                && 既有幂等.所有者 == 第一层写入端口_.所有者身份()
                && 既有幂等.写入幂等身份 == 写集.写入幂等身份
                && 既有幂等.读取事实代次 != 0;
            if (!幂等头完整)
                return 形成普通概念写入失败(
                    L2结构状态::内部不一致, 既有幂等.读取事实代次);
            if (既有幂等.状态 == L1所有者范围读取状态::成功) {
                if (!既有幂等.首次规范化写集 || !既有幂等.首次写入结果)
                    return 形成普通概念写入失败(
                        L2结构状态::内部不一致, 既有幂等.读取事实代次);
                if (*既有幂等.首次规范化写集 != 写集)
                    return 形成普通概念写入失败(
                        L2结构状态::幂等冲突, 既有幂等.读取事实代次);
                const auto& 首次结果 = *既有幂等.首次写入结果;
                const bool 首次形状完整 = 首次结果.状态
                        == L1所有者范围写入状态::成功
                    && 首次结果.合同版本 == L1所有者范围CRUD合同版本
                    && 首次结果.所有者 == 第一层写入端口_.所有者身份()
                    && 首次结果.写入幂等身份 == 写集.写入幂等身份
                    && 首次结果.事实代次 != 0
                    && 首次结果.事实代次 <= 既有幂等.读取事实代次
                    && 首次结果.是否形成内存权威发布
                    && 首次结果.重试边界 == L1所有者范围重试边界::不适用;
                const auto 概念编码 = 读取普通概念节点映射(首次结果, 请求);
                if (!首次形状完整 || !概念编码)
                    return 形成普通概念写入失败(
                        L2结构状态::内部不一致, 既有幂等.读取事实代次);
                const auto 首次读 = 读取普通概念({
                    {L2结构合同版本, 既有幂等.读取事实代次},
                    L2普通概念结构合同版本, L2读取类别::历史,
                    L2概念身份{*概念编码}, 首次结果.事实代次});
                if (!首次读.成功() || !首次读.概念)
                    return 形成普通概念写入失败(
                        首次读.结果头.状态, 首次读.结果头.事实截止代次);
                L2普通概念写入结果 结果{{L2结构合同版本,
                    L2结构状态::精确重复, 首次结果.事实代次,
                    首次结果.事实代次}, *首次读.概念};
                return 结果.成功() ? 结果
                    : 形成普通概念写入失败(
                        L2结构状态::内部不一致, 首次结果.事实代次);
            }
            if (既有幂等.首次规范化写集 || 既有幂等.首次写入结果)
                return 形成普通概念写入失败(
                    L2结构状态::内部不一致, 既有幂等.读取事实代次);
            if (既有幂等.状态 != L1所有者范围读取状态::未找到)
                return 形成普通概念写入失败(
                    映射历史读取状态(既有幂等.状态), 既有幂等.读取事实代次);
            if (既有幂等.读取事实代次 != 请求.请求头.期望事实代次)
                return 形成普通概念写入失败(
                    L2结构状态::事实代次漂移, 既有幂等.读取事实代次);

            const L2概念身份 空概念{稳定编码{}};
            L2普通概念事实 候选{空概念, {},
                {请求.本体根, 空概念, {}, {}},
                {空概念, 请求.签名规则材料, {}, {}},
                {}, {}, {}, {},
                {空概念, L2概念治理生命周期状态::活跃, {}, {}}};
            for (const auto& 项 : 请求.签名值式项)
                候选.签名值式项.push_back({空概念, 项, {}, {}});
            for (const auto& 项 : 请求.初始定义)
                候选.定义关系.push_back({空概念, 项, {}, {}});
            for (const auto& 上位 : 请求.直接上位)
                候选.直接上位.push_back({上位, 空概念, {}, {}});
            for (const auto& 证据 : 请求.来源证据材料)
                候选.来源证据.push_back({空概念, 证据, {}, {}});
            const L2普通概念读取请求 验证请求{请求.请求头,
                L2普通概念结构合同版本, L2读取类别::当前, 空概念, 0};
            const auto 引用状态 = 校验普通概念公开引用(第一层服务_,
                身份来源定位_, 本体结构定位_, 普通结构定位_,
                普通概念依赖_,
                [this](const L2世界事实引用& 引用, L2读取类别 类别,
                    std::uint64_t 历史截止, std::uint64_t 期望代次) {
                    return 校验世界事实公开引用(
                        引用, 类别, 历史截止, 期望代次);
                }, 候选, 验证请求);
            if (引用状态 != L2结构状态::已读取)
                return 形成普通概念写入失败(引用状态,
                    请求.请求头.期望事实代次);

            const auto 身份组 = 读取全部当前普通概念身份(第一层服务_,
                身份来源定位_, 普通结构定位_, 请求.请求头.期望事实代次);
            if (身份组.状态 != L2结构状态::已读取)
                return 形成普通概念写入失败(身份组.状态, 身份组.截止);
            for (const auto& 身份 : 身份组.身份) {
                const auto 现有 = 重建普通概念自有事实(第一层服务_,
                    身份来源定位_, 本体结构定位_, 普通结构定位_,
                    {请求.请求头, L2普通概念结构合同版本,
                        L2读取类别::当前, 身份, 0});
                if (现有.状态 != L2结构状态::已读取 || !现有.首次请求
                    || !现有.事实)
                    return 形成普通概念写入失败(
                        现有.状态 == L2结构状态::已读取
                            ? L2结构状态::内部不一致 : 现有.状态,
                        现有.截止);
                if (现有.首次请求->幂等身份 == 请求.幂等身份) {
                    return 形成普通概念写入失败(
                        L2结构状态::内部不一致, 现有.截止);
                }
                const L2普通概念读取请求 现有验证请求{请求.请求头,
                    L2普通概念结构合同版本, L2读取类别::当前, 身份, 0};
                const auto 现有引用状态 = 校验普通概念公开引用(
                    第一层服务_, 身份来源定位_, 本体结构定位_,
                    普通结构定位_, 普通概念依赖_,
                    [this](const L2世界事实引用& 引用, L2读取类别 类别,
                        std::uint64_t 历史截止, std::uint64_t 期望代次) {
                        return 校验世界事实公开引用(
                            引用, 类别, 历史截止, 期望代次);
                    }, *现有.事实, 现有验证请求);
                if (现有引用状态 != L2结构状态::已读取)
                    return 形成普通概念写入失败(
                        现有引用状态, 请求.请求头.期望事实代次);
                if (普通概念签名相同(*现有.首次请求, 请求))
                    return 形成普通概念写入失败(L2结构状态::引用冲突,
                        请求.请求头.期望事实代次);
            }

            const auto 写入 = 第一层写入端口_.提交所有者范围中性写集(写集);
            if (写入.合同版本 != L1所有者范围CRUD合同版本
                || 写入.所有者 != 第一层写入端口_.所有者身份()
                || 写入.写入幂等身份 != 写集.写入幂等身份)
                return 形成普通概念写入失败(
                    L2结构状态::内部不一致, 写入.事实代次);
            const auto 写入状态 = 映射写入状态(写入.状态);
            if (写入状态 != L2结构状态::已提交
                && 写入状态 != L2结构状态::精确重复) {
                if (!写入.新编码映射.empty() || 写入.是否形成内存权威发布)
                    return 形成普通概念写入失败(
                        L2结构状态::内部不一致, 写入.事实代次);
                return 形成普通概念写入失败(写入状态, 写入.事实代次);
            }
            const bool 提交首次完整 = 写入.状态 == L1所有者范围写入状态::成功
                && 写入.是否形成内存权威发布
                && 写入.重试边界 == L1所有者范围重试边界::不适用;
            const bool 提交重复完整 = 写入.状态
                    == L1所有者范围写入状态::精确重复
                && !写入.是否形成内存权威发布
                && 写入.重试边界
                    == L1所有者范围重试边界::原幂等身份读回收敛;
            const auto 提交概念编码 = 读取普通概念节点映射(写入, 请求);
            if (写入.事实代次 == 0 || (!提交首次完整 && !提交重复完整)
                || !提交概念编码)
                return 形成普通概念写入失败(
                    L2结构状态::内部不一致, 写入.事实代次);
            const auto 首次读取 = 第一层写入端口_.读取首次写入材料(
                {L1所有者范围首次写入读取合同版本, 写集.写入幂等身份});
            if (首次读取.状态 != L1所有者范围读取状态::成功)
                return 形成普通概念写入失败(
                    首次读取.状态 == L1所有者范围读取状态::资源失败
                        ? L2结构状态::资源失败 : L2结构状态::内部不一致,
                    首次读取.读取事实代次);
            if (首次读取.合同版本 != L1所有者范围首次写入读取合同版本
                || 首次读取.所有者 != 第一层写入端口_.所有者身份()
                || 首次读取.写入幂等身份 != 写集.写入幂等身份
                || 首次读取.读取事实代次 == 0
                || !首次读取.首次规范化写集 || !首次读取.首次写入结果
                || *首次读取.首次规范化写集 != 写集)
                return 形成普通概念写入失败(
                    L2结构状态::内部不一致, 首次读取.读取事实代次);
            const auto& 首次结果 = *首次读取.首次写入结果;
            if (首次结果.状态 != L1所有者范围写入状态::成功
                || 首次结果.合同版本 != L1所有者范围CRUD合同版本
                || 首次结果.所有者 != 第一层写入端口_.所有者身份()
                || 首次结果.写入幂等身份 != 写集.写入幂等身份
                || 首次结果.事实代次 == 0 || !首次结果.是否形成内存权威发布
                || 首次结果.重试边界 != L1所有者范围重试边界::不适用
                || 首次结果.事实代次 > 首次读取.读取事实代次
                || 写入.事实代次 != 首次结果.事实代次
                || 写入.新编码映射 != 首次结果.新编码映射)
                return 形成普通概念写入失败(
                    L2结构状态::内部不一致, 首次结果.事实代次);
            const auto 首次概念编码 = 读取普通概念节点映射(首次结果, 请求);
            if (!首次概念编码 || *首次概念编码 != *提交概念编码)
                return 形成普通概念写入失败(
                    L2结构状态::内部不一致, 首次结果.事实代次);
            const auto 首次读 = 读取普通概念({
                {L2结构合同版本, 首次读取.读取事实代次},
                L2普通概念结构合同版本, L2读取类别::历史,
                L2概念身份{*首次概念编码}, 首次结果.事实代次});
            if (!首次读.成功() || !首次读.概念)
                return 形成普通概念写入失败(
                    首次读.结果头.状态, 首次读.结果头.事实截止代次);
            L2普通概念写入结果 结果{{L2结构合同版本,
                写入状态, 首次结果.事实代次, 首次结果.事实代次},
                *首次读.概念};
            return 结果.成功() ? 结果
                : 形成普通概念写入失败(
                    L2结构状态::内部不一致, 首次结果.事实代次);
        } catch (const std::bad_alloc&) {
            return 形成普通概念写入失败(L2结构状态::资源失败);
        } catch (const std::length_error&) {
            return 形成普通概念写入失败(L2结构状态::资源失败);
        } catch (...) {
            return 形成普通概念写入失败(L2结构状态::内部不一致);
        }
    }

private:
    // 诊断责任：向上送出；单个世界事实只经其唯一公开结构服务在同一截止读回。
    L2结构状态 校验世界事实公开引用(
        const L2世界事实引用& 引用, L2读取类别 类别,
        std::uint64_t 历史截止, std::uint64_t 期望代次) const {
        using namespace L2概念结构内部;
        if (!普通概念依赖_.完整()) return L2结构状态::未实现;
        const auto 头 = L2结构请求头{L2结构合同版本, 期望代次};
        const auto 读截止 = 类别 == L2读取类别::历史 ? 历史截止 : 0;
        const auto 结果状态 = [](const L2结构结果头& 结果头) {
            return 结果头.状态 == L2结构状态::已读取
                ? L2结构状态::内部不一致 : 结果头.状态;
        };
        const auto 投影截止 = 类别 == L2读取类别::历史
            ? 历史截止 : 期望代次;
        const auto 完整读取成功 = [&](const auto& 读取) {
            return 读取.结果头.合同版本 == L2结构合同版本
                && 读取.结果头.状态 == L2结构状态::已读取
                && 读取.结果头.事实截止代次 == 投影截止
                && !读取.结果头.变更事实代次
                && 读取.读取类别 == 类别
                && 读取.历史截止事实代次 == 读截止;
        };
        const auto 生命周期投影完整 = [&](const L2生命周期& 生命周期) {
            return L2生命周期完整(生命周期)
                && 生命周期.创建事实代次 <= 投影截止
                && (!生命周期.退出事实代次
                    || *生命周期.退出事实代次 <= 投影截止)
                && (类别 == L2读取类别::历史
                    || !生命周期.退出事实代次);
        };
        const auto 状态公开形状完整 = [&](const L2状态事实& 状态事实,
            const L2状态自有关系引用& 关系) {
            return 有效(状态事实.身份.值) && 有效(状态事实.主体存在.值)
                && 有效(状态事实.特征实例.值) && 有效(状态事实.来源稳定编码)
                && 生命周期投影完整(状态事实.生命周期)
                && L2属性事实截止投影完整(状态事实.精确值, 投影截止)
                && L2属性事实截止投影完整(状态事实.时间值, 投影截止)
                && (类别 == L2读取类别::历史
                    || (!状态事实.精确值.退出事实代次
                        && !状态事实.时间值.退出事实代次))
                && 状态事实.精确值.值稳定编码 != 状态事实.时间值.值稳定编码
                && 状态事实.精确值.来源稳定编码 == 状态事实.来源稳定编码
                && 状态事实.时间值.来源稳定编码 == 状态事实.来源稳定编码
                && 有效(关系.族归属关系稳定编码)
                && 有效(关系.主体关系稳定编码)
                && 有效(关系.特征实例关系稳定编码)
                && 关系.族归属关系稳定编码 != 关系.主体关系稳定编码
                && 关系.族归属关系稳定编码 != 关系.特征实例关系稳定编码
                && 关系.主体关系稳定编码 != 关系.特征实例关系稳定编码;
        };
        const auto 动态公开形状完整 = [&](const L2动态事实& 动态事实,
            const L2动态自有关系引用& 关系) {
            return 有效(动态事实.身份.值) && 有效(动态事实.主体存在.值)
                && 有效(动态事实.前状态.值) && 有效(动态事实.后状态.值)
                && 有效(动态事实.来源稳定编码)
                && 生命周期投影完整(动态事实.生命周期)
                && L2属性事实截止投影完整(动态事实.起始时间值, 投影截止)
                && L2属性事实截止投影完整(动态事实.结束时间值, 投影截止)
                && (类别 == L2读取类别::历史
                    || (!动态事实.起始时间值.退出事实代次
                        && !动态事实.结束时间值.退出事实代次))
                && 动态事实.起始时间值.值稳定编码
                    != 动态事实.结束时间值.值稳定编码
                && 动态事实.起始时间值.来源稳定编码 == 动态事实.来源稳定编码
                && 动态事实.结束时间值.来源稳定编码 == 动态事实.来源稳定编码
                && 有效(关系.族归属关系稳定编码)
                && 有效(关系.主体关系稳定编码)
                && 有效(关系.前状态关系稳定编码)
                && 有效(关系.后状态关系稳定编码)
                && 关系.族归属关系稳定编码 != 关系.主体关系稳定编码
                && 关系.族归属关系稳定编码 != 关系.前状态关系稳定编码
                && 关系.族归属关系稳定编码 != 关系.后状态关系稳定编码
                && 关系.主体关系稳定编码 != 关系.前状态关系稳定编码
                && 关系.主体关系稳定编码 != 关系.后状态关系稳定编码
                && 关系.前状态关系稳定编码 != 关系.后状态关系稳定编码;
        };
        return std::visit([&](const auto& 值) -> L2结构状态 {
            using 类型 = std::decay_t<decltype(值)>;
            if constexpr (std::is_same_v<类型, L2场景身份>) {
                const auto 读取 = 普通概念依赖_.场景->读取完整场景(
                    {头, 类别, 值, 读截止});
                if (!完整读取成功(读取)) return 结果状态(读取.结果头);
                return 读取.场景 && 读取.场景->身份 == 值
                    && L2场景事实完整(*读取.场景, 投影截止)
                    && 生命周期投影完整(读取.场景->生命周期)
                    ? L2结构状态::已读取 : L2结构状态::内部不一致;
            } else if constexpr (std::is_same_v<类型, L2存在身份>) {
                const auto 读取 = 普通概念依赖_.存在->读取完整存在(
                    {头, 类别, 值, 读截止});
                if (!完整读取成功(读取)) return 结果状态(读取.结果头);
                return 读取.存在 && 读取.存在->身份 == 值
                    && L2存在事实完整(*读取.存在, 投影截止)
                    && 生命周期投影完整(读取.存在->生命周期)
                    ? L2结构状态::已读取 : L2结构状态::内部不一致;
            } else if constexpr (std::is_same_v<类型, L2特征定义身份>) {
                const auto 读取 = 普通概念依赖_.特征->读取完整统一特征定义(
                    {头, 类别, 值, 读截止});
                if (!完整读取成功(读取)) return 结果状态(读取.结果头);
                return 读取.特征定义 && 读取.特征定义->身份 == 值
                    && L2统一特征定义事实完整(*读取.特征定义, 投影截止)
                    && 生命周期投影完整(读取.特征定义->生命周期)
                    ? L2结构状态::已读取 : L2结构状态::内部不一致;
            } else if constexpr (std::is_same_v<类型, L2特征实例身份>) {
                const auto 读取 = 普通概念依赖_.特征->读取完整特征实例(
                    {头, 类别, 值, 读截止});
                if (!完整读取成功(读取)) return 结果状态(读取.结果头);
                return 读取.特征实例 && 读取.特征实例->身份 == 值
                    && 读取.自有关系
                    && L2特征实例事实完整(*读取.特征实例, 投影截止)
                    && 生命周期投影完整(读取.特征实例->生命周期)
                    ? L2结构状态::已读取 : L2结构状态::内部不一致;
            } else if constexpr (std::is_same_v<类型, L2概念特征值引用>) {
                const auto 实例 = 普通概念依赖_.特征->读取完整特征实例(
                    {头, 类别, 值.特征实例, 读截止});
                if (!完整读取成功(实例) || !实例.特征实例
                    || 实例.特征实例->身份 != 值.特征实例
                    || !实例.自有关系
                    || !L2特征实例事实完整(*实例.特征实例, 投影截止)
                    || !生命周期投影完整(实例.特征实例->生命周期))
                    return 结果状态(实例.结果头);
                if (类别 == L2读取类别::当前) {
                    const auto 读取 = 普通概念依赖_.特征->读取特征当前值(
                        {头, 值.特征实例});
                    if (!读取.成功()) return 结果状态(读取.结果头);
                    return 读取.当前值
                        && 读取.当前值->特征实例 == 值.特征实例
                        && 读取.当前值->值稳定编码 == 值.值稳定编码
                        ? L2结构状态::已读取 : L2结构状态::引用冲突;
                }
                const auto 读取 = 普通概念依赖_.特征->读取特征历史值(
                    {头, 值.特征实例, 值.值稳定编码});
                if (读取.结果头.状态 != L2结构状态::已读取)
                    return 读取.结果头.状态;
                return 读取.结果头.合同版本 == L2结构合同版本
                    && 读取.结果头.事实截止代次 == 投影截止
                    && !读取.结果头.变更事实代次 && 读取.特征值
                    && 读取.特征值->特征实例 == 值.特征实例
                    && 读取.特征值->值稳定编码 == 值.值稳定编码
                    && 有效(读取.特征值->来源稳定编码)
                    && L2特征当前值材料有效(读取.特征值->类型化不可变材料)
                    && L2特征值事实截止投影完整(*读取.特征值, 投影截止)
                    ? L2结构状态::已读取 : L2结构状态::内部不一致;
            } else if constexpr (std::is_same_v<类型, L2状态身份>) {
                const auto 读取 = 普通概念依赖_.状态->读取完整状态(
                    {头, 类别, 值, 读截止});
                if (!完整读取成功(读取)) return 结果状态(读取.结果头);
                return 读取.状态 && 读取.状态->身份 == 值
                    && 读取.自有关系
                    && 状态公开形状完整(*读取.状态, *读取.自有关系)
                    ? L2结构状态::已读取 : L2结构状态::内部不一致;
            } else if constexpr (std::is_same_v<类型, L2动态身份>) {
                const auto 读取 = 普通概念依赖_.动态->读取完整动态(
                    {头, 类别, 值, 读截止});
                if (!完整读取成功(读取)) return 结果状态(读取.结果头);
                return 读取.动态 && 读取.动态->身份 == 值
                    && 读取.自有关系
                    && 动态公开形状完整(*读取.动态, *读取.自有关系)
                    ? L2结构状态::已读取 : L2结构状态::内部不一致;
            } else if constexpr (std::is_same_v<类型, L2因果身份>) {
                const auto 读取 = 普通概念依赖_.因果->读取完整因果(
                    {头, 类别, 值, 读截止});
                if (!完整读取成功(读取)) return 结果状态(读取.结果头);
                return 读取.因果 && 读取.因果->身份 == 值
                    && 读取.自有事实
                    && L2因果事实完整(*读取.因果, 投影截止)
                    && 生命周期投影完整(读取.因果->生命周期)
                    ? L2结构状态::已读取 : L2结构状态::内部不一致;
            } else {
                const auto& 关系 = 值;
                switch (关系.类别) {
                case L2概念世界关系类别::场景父子:
                case L2概念世界关系类别::场景成员:
                case L2概念世界关系类别::场景宿主存在: {
                    const auto 源 = std::get<L2场景身份>(关系.源);
                    const auto 读取 = 普通概念依赖_.场景->读取完整场景(
                        {头, 类别, 源, 读截止});
                    if (!完整读取成功(读取) || !读取.场景
                        || 读取.场景->身份 != 源
                        || !L2场景事实完整(*读取.场景, 投影截止)
                        || !生命周期投影完整(读取.场景->生命周期))
                        return 结果状态(读取.结果头);
                    if (关系.类别 == L2概念世界关系类别::场景父子) {
                        const auto 目标 = std::get<L2场景身份>(关系.目标);
                        for (const auto& 项 : 读取.场景->直接子场景)
                            if (项.关系稳定编码 == 关系.关系稳定编码
                                && 项.父场景 == 源 && 项.子场景 == 目标)
                                return L2结构状态::已读取;
                    } else if (关系.类别 == L2概念世界关系类别::场景成员) {
                        const auto 目标 = std::get<L2存在身份>(关系.目标);
                        for (const auto& 项 : 读取.场景->直接成员)
                            if (项.关系稳定编码 == 关系.关系稳定编码
                                && 项.场景 == 源 && 项.存在 == 目标)
                                return L2结构状态::已读取;
                    } else {
                        const auto 目标 = std::get<L2存在身份>(关系.目标);
                        if (读取.场景->当前宿主关系
                            && 读取.场景->当前宿主关系->关系稳定编码
                                == 关系.关系稳定编码
                            && 读取.场景->当前宿主关系->场景 == 源
                            && 读取.场景->当前宿主关系->宿主存在 == 目标)
                            return L2结构状态::已读取;
                    }
                    return L2结构状态::引用冲突;
                }
                case L2概念世界关系类别::特征宿主:
                case L2概念世界关系类别::特征定义: {
                    const auto 源 = std::get<L2特征实例身份>(关系.源);
                    const auto 读取 = 普通概念依赖_.特征->读取完整特征实例(
                        {头, 类别, 源, 读截止});
                    if (!完整读取成功(读取) || !读取.特征实例
                        || 读取.特征实例->身份 != 源 || !读取.自有关系
                        || !L2特征实例事实完整(*读取.特征实例, 投影截止)
                        || !生命周期投影完整(读取.特征实例->生命周期))
                        return 结果状态(读取.结果头);
                    const bool 宿主 = 关系.类别 == L2概念世界关系类别::特征宿主;
                    const bool 端点 = 宿主
                        ? 读取.特征实例->宿主存在 == std::get<L2存在身份>(关系.目标)
                        : 读取.特征实例->特征定义 == std::get<L2特征定义身份>(关系.目标);
                    const auto 编码 = 宿主 ? 读取.自有关系->宿主关系稳定编码
                        : 读取.自有关系->定义关系稳定编码;
                    return 端点 && 编码 == 关系.关系稳定编码
                        ? L2结构状态::已读取 : L2结构状态::引用冲突;
                }
                case L2概念世界关系类别::状态主体:
                case L2概念世界关系类别::状态特征实例: {
                    const auto 源 = std::get<L2状态身份>(关系.源);
                    const auto 读取 = 普通概念依赖_.状态->读取完整状态(
                        {头, 类别, 源, 读截止});
                    if (!完整读取成功(读取) || !读取.状态
                        || 读取.状态->身份 != 源 || !读取.自有关系
                        || !状态公开形状完整(*读取.状态, *读取.自有关系))
                        return 结果状态(读取.结果头);
                    const bool 主体 = 关系.类别 == L2概念世界关系类别::状态主体;
                    const bool 端点 = 主体
                        ? 读取.状态->主体存在 == std::get<L2存在身份>(关系.目标)
                        : 读取.状态->特征实例 == std::get<L2特征实例身份>(关系.目标);
                    const auto 编码 = 主体 ? 读取.自有关系->主体关系稳定编码
                        : 读取.自有关系->特征实例关系稳定编码;
                    return 端点 && 编码 == 关系.关系稳定编码
                        ? L2结构状态::已读取 : L2结构状态::引用冲突;
                }
                case L2概念世界关系类别::动态主体:
                case L2概念世界关系类别::动态前状态:
                case L2概念世界关系类别::动态后状态: {
                    const auto 源 = std::get<L2动态身份>(关系.源);
                    const auto 读取 = 普通概念依赖_.动态->读取完整动态(
                        {头, 类别, 源, 读截止});
                    if (!完整读取成功(读取) || !读取.动态
                        || 读取.动态->身份 != 源 || !读取.自有关系
                        || !动态公开形状完整(*读取.动态, *读取.自有关系))
                        return 结果状态(读取.结果头);
                    bool 端点 = false; 稳定编码 编码;
                    if (关系.类别 == L2概念世界关系类别::动态主体) {
                        端点 = 读取.动态->主体存在 == std::get<L2存在身份>(关系.目标);
                        编码 = 读取.自有关系->主体关系稳定编码;
                    } else if (关系.类别 == L2概念世界关系类别::动态前状态) {
                        端点 = 读取.动态->前状态 == std::get<L2状态身份>(关系.目标);
                        编码 = 读取.自有关系->前状态关系稳定编码;
                    } else {
                        端点 = 读取.动态->后状态 == std::get<L2状态身份>(关系.目标);
                        编码 = 读取.自有关系->后状态关系稳定编码;
                    }
                    return 端点 && 编码 == 关系.关系稳定编码
                        ? L2结构状态::已读取 : L2结构状态::引用冲突;
                }
                default: {
                    const auto 源 = std::get<L2因果身份>(关系.源);
                    const auto 读取 = 普通概念依赖_.因果->读取完整因果(
                        {头, 类别, 源, 读截止});
                    if (!完整读取成功(读取) || !读取.因果
                        || 读取.因果->身份 != 源 || !读取.自有事实
                        || !L2因果事实完整(*读取.因果, 投影截止)
                        || !生命周期投影完整(读取.因果->生命周期))
                        return 结果状态(读取.结果头);
                    if (关系.类别 == L2概念世界关系类别::因果动作引用) {
                        const auto 目标 = std::get<L2概念动作引用>(关系.目标);
                        return 读取.因果->动作 && 读取.自有事实->动作关系稳定编码
                            && *读取.因果->动作 == 目标.值
                            && *读取.自有事实->动作关系稳定编码
                                == 关系.关系稳定编码
                            ? L2结构状态::已读取 : L2结构状态::引用冲突;
                    }
                    const auto 目标 = std::get<L2状态身份>(关系.目标);
                    const bool 条件 = 关系.类别
                        == L2概念世界关系类别::因果条件状态;
                    const auto& 端点组 = 条件 ? 读取.因果->条件状态
                        : 读取.因果->结果状态;
                    const auto& 编码组 = 条件 ? 读取.自有事实->条件关系稳定编码
                        : 读取.自有事实->结果关系稳定编码;
                    for (std::size_t 索引 = 0;
                        索引 < 端点组.size() && 索引 < 编码组.size(); ++索引)
                        if (端点组[索引].目标 == 目标.值
                            && 编码组[索引] == 关系.关系稳定编码)
                            return L2结构状态::已读取;
                    return L2结构状态::引用冲突;
                }
                }
            }
        }, 引用);
    }

    // 诊断责任：向上送出；支持事实的概念、世界事实和全部材料在同一截止互证。
    L2结构状态 校验概念支持公开引用(
        const L2概念世界事实支持关系事实& 关系, L2读取类别 类别,
        std::uint64_t 历史截止, std::uint64_t 期望代次) const {
        const auto 概念 = 读取概念端点归属(
            关系.概念, 类别, 历史截止, 期望代次);
        if (概念.状态 != L2结构状态::已读取 || !概念.本体根)
            return 概念.状态 == L2结构状态::已读取
                ? L2结构状态::内部不一致 : 概念.状态;
        const auto 世界 = 校验世界事实公开引用(
            关系.世界事实, 类别, 历史截止, 期望代次);
        if (世界 != L2结构状态::已读取) return 世界;
        const auto 读材料 = [&](不可变材料身份 身份) {
            const auto 读取 = 普通概念依赖_.材料->读取不可变材料({
                {L2结构合同版本, 期望代次}, 类别, 身份,
                类别 == L2读取类别::历史 ? 历史截止 : 0});
            if (读取.结果头.状态 != L2结构状态::已读取)
                return 读取.结果头.状态;
            const auto 投影截止 = 类别 == L2读取类别::历史
                ? 历史截止 : 期望代次;
            return 读取.结果头.合同版本 == L2结构合同版本
                && 读取.结果头.事实截止代次 == 投影截止
                && !读取.结果头.变更事实代次 && 读取.材料
                && 读取.材料->身份 == 身份
                && L2生命周期完整(读取.材料->生命周期)
                && 读取.材料->生命周期.创建事实代次 <= 投影截止
                && (!读取.材料->生命周期.退出事实代次
                    || *读取.材料->生命周期.退出事实代次 <= 投影截止)
                && (类别 == L2读取类别::历史
                    || !读取.材料->生命周期.退出事实代次)
                ? L2结构状态::已读取 : L2结构状态::内部不一致;
        };
        auto 状态 = 读材料(关系.建立依据材料);
        if (状态 != L2结构状态::已读取) return 状态;
        for (const auto 身份 : 关系.发布证据材料) {
            状态 = 读材料(身份);
            if (状态 != L2结构状态::已读取) return 状态;
        }
        return L2结构状态::已读取;
    }

    // 诊断责任：向上送出；三种支持关系写共用首次材料、当前唯一性和原子提交。
    L2概念世界事实支持写入结果 执行概念世界事实支持写入(
        std::uint64_t 期望代次, L2结构幂等身份 幂等身份,
        std::optional<L2概念世界事实支持关系事实> 新关系,
        std::optional<L2概念世界事实支持关系事实> 旧关系期望) {
        using namespace L2概念结构内部;
        if (!支持结构定位_ || !普通概念依赖_.完整())
            return 形成概念支持写入失败(L2结构状态::未实现);
        try {
            std::lock_guard<std::mutex> 锁(概念写入锁_);
            const auto 映射身份 = 映射概念幂等身份(幂等身份);
            const auto 首次读取 = 第一层写入端口_.读取首次写入材料(
                {L1所有者范围首次写入读取合同版本, 映射身份});
            const bool 首次头完整 = 首次读取.合同版本
                    == L1所有者范围首次写入读取合同版本
                && 首次读取.所有者 == 第一层写入端口_.所有者身份()
                && 首次读取.写入幂等身份 == 映射身份
                && 首次读取.读取事实代次 != 0;
            if (!首次头完整)
                return 形成概念支持写入失败(
                    L2结构状态::内部不一致, 首次读取.读取事实代次);
            if (首次读取.状态 == L1所有者范围读取状态::成功) {
                if (!首次读取.首次规范化写集 || !首次读取.首次写入结果)
                    return 形成概念支持写入失败(
                        L2结构状态::内部不一致, 首次读取.读取事实代次);
                const auto& 首次结果 = *首次读取.首次写入结果;
                if (首次结果.事实代次 == 0
                    || 首次结果.事实代次 > 首次读取.读取事实代次)
                    return 形成概念支持写入失败(
                        L2结构状态::内部不一致, 首次读取.读取事实代次);
                std::optional<概念世界事实支持内部事实> 旧内部;
                if (旧关系期望) {
                    if (首次结果.事实代次 <= 1)
                        return 形成概念支持写入失败(
                            L2结构状态::内部不一致, 首次结果.事实代次);
                    const auto 历史关系 = 第一层服务_.读取所有者范围历史事实(
                        {L1所有者范围CRUD合同版本,
                            旧关系期望->关系稳定编码});
                    const auto* 关系 = 历史关系.事实
                        ? std::get_if<L1所有者范围关系事实>(&*历史关系.事实)
                        : nullptr;
                    if (历史关系.状态 != L1所有者范围读取状态::成功
                        || 历史关系.读取事实代次 != 首次读取.读取事实代次
                        || !关系 || 关系->写入所有者 != 身份来源定位_.所有者
                        || 关系->关系类型节点 != 支持结构定位_->支持关系类型
                        || 关系->退出事实代次 != 首次结果.事实代次)
                        return 形成概念支持写入失败(
                            L2结构状态::幂等冲突, 首次读取.读取事实代次);
                    const auto 旧读 = 读取概念支持单项(第一层服务_,
                        身份来源定位_, *支持结构定位_, L2读取类别::历史,
                        首次结果.事实代次 - 1, 首次读取.读取事实代次,
                        关系->源节点, std::nullopt, 关系->编码);
                    if (旧读.状态 != L2结构状态::已读取 || !旧读.事实)
                        return 形成概念支持写入失败(
                            L2结构状态::幂等冲突, 首次读取.读取事实代次);
                    旧内部 = *旧读.事实;
                }
                const auto 期望写集 = 形成概念支持写集(期望代次, 幂等身份,
                    *支持结构定位_, 新关系, 旧内部);
                if (*首次读取.首次规范化写集 != 期望写集)
                    return 形成概念支持写入失败(
                        L2结构状态::幂等冲突, 首次读取.读取事实代次);
                auto 结果 = 重建概念支持首次结果(第一层服务_,
                    身份来源定位_, *支持结构定位_, 期望写集, 首次结果,
                    L2结构状态::精确重复, 新关系, 旧内部,
                    首次读取.读取事实代次);
                if (!结果.成功()) return 结果;
                if (旧关系期望 && (!结果.已退出关系
                    || 结果.已退出关系->关系稳定编码
                        != 旧关系期望->关系稳定编码
                    || 结果.已退出关系->世界事实 != 旧关系期望->世界事实
                    || 结果.已退出关系->概念 != 旧关系期望->概念))
                    return 形成概念支持写入失败(
                        L2结构状态::幂等冲突, 首次读取.读取事实代次);
                return 结果;
            }
            if (首次读取.首次规范化写集 || 首次读取.首次写入结果)
                return 形成概念支持写入失败(
                    L2结构状态::内部不一致, 首次读取.读取事实代次);
            if (首次读取.状态 != L1所有者范围读取状态::未找到)
                return 形成概念支持写入失败(
                    映射历史读取状态(首次读取.状态),
                    首次读取.读取事实代次);
            if (首次读取.读取事实代次 != 期望代次)
                return 形成概念支持写入失败(
                    L2结构状态::事实代次漂移, 首次读取.读取事实代次);

            constexpr std::size_t 最大预算 = static_cast<std::size_t>(-1);
            const auto 当前组 = 读取概念支持关系组核心(第一层服务_,
                身份来源定位_, *支持结构定位_, L2读取类别::当前,
                0, 期望代次, std::nullopt, std::nullopt,
                最大预算, 最大预算);
            if (!当前组.成功())
                return 形成概念支持写入失败(
                    当前组.结果头.状态, 当前组.结果头.事实截止代次);
            std::optional<L2概念世界事实支持关系事实> 旧公开;
            bool 旧编码命中 = false;
            bool 旧端点命中 = false;
            if (旧关系期望) {
                for (const auto& 关系 : 当前组.关系组) {
                    const bool 编码命中 = 关系.关系稳定编码
                        == 旧关系期望->关系稳定编码;
                    const bool 端点命中 = 关系.世界事实 == 旧关系期望->世界事实
                        && 关系.概念 == 旧关系期望->概念;
                    旧编码命中 = 旧编码命中 || 编码命中;
                    旧端点命中 = 旧端点命中 || 端点命中;
                    if (编码命中 && 端点命中) 旧公开 = 关系;
                }
                if (!旧公开) return 形成概念支持写入失败(
                    旧编码命中 || 旧端点命中
                        ? L2结构状态::引用冲突 : L2结构状态::未找到,
                    期望代次);
            }
            if (新关系) {
                for (const auto& 关系 : 当前组.关系组) {
                    if (旧公开
                        && 关系.关系稳定编码 == 旧公开->关系稳定编码) continue;
                    if (关系.世界事实 == 新关系->世界事实
                        && 关系.概念 == 新关系->概念)
                        return 形成概念支持写入失败(
                            L2结构状态::引用冲突, 期望代次);
                }
                const auto 新状态 = 校验概念支持公开引用(
                    *新关系, L2读取类别::当前, 0, 期望代次);
                if (新状态 != L2结构状态::已读取)
                    return 形成概念支持写入失败(新状态, 期望代次);
            }
            std::optional<概念世界事实支持内部事实> 旧内部;
            if (旧公开) {
                const auto 旧状态 = 校验概念支持公开引用(
                    *旧公开, L2读取类别::当前, 0, 期望代次);
                if (旧状态 != L2结构状态::已读取)
                    return 形成概念支持写入失败(旧状态, 期望代次);
                const auto 旧读 = 读取当前概念支持内部事实(第一层服务_,
                    身份来源定位_, *支持结构定位_, 期望代次,
                    旧公开->关系稳定编码);
                if (旧读.状态 != L2结构状态::已读取 || !旧读.事实)
                    return 形成概念支持写入失败(
                        旧读.状态 == L2结构状态::已读取
                            ? L2结构状态::内部不一致 : 旧读.状态,
                        旧读.截止);
                旧内部 = *旧读.事实;
            }
            const auto 写集 = 形成概念支持写集(期望代次, 幂等身份,
                *支持结构定位_, 新关系, 旧内部);
            const auto 写入 = 第一层写入端口_.提交所有者范围中性写集(写集);
            if (写入.合同版本 != L1所有者范围CRUD合同版本
                || 写入.所有者 != 第一层写入端口_.所有者身份()
                || 写入.写入幂等身份 != 写集.写入幂等身份)
                return 形成概念支持写入失败(
                    L2结构状态::内部不一致, 写入.事实代次);
            const auto 写入状态 = 映射写入状态(写入.状态);
            if (写入状态 != L2结构状态::已提交
                && 写入状态 != L2结构状态::精确重复) {
                if (!写入.新编码映射.empty() || 写入.是否形成内存权威发布)
                    return 形成概念支持写入失败(
                        L2结构状态::内部不一致, 写入.事实代次);
                return 形成概念支持写入失败(写入状态, 写入.事实代次);
            }
            const bool 首次提交完整 = 写入.状态 == L1所有者范围写入状态::成功
                && 写入.是否形成内存权威发布
                && 写入.重试边界 == L1所有者范围重试边界::不适用;
            const bool 重复提交完整 = 写入.状态
                    == L1所有者范围写入状态::精确重复
                && !写入.是否形成内存权威发布
                && 写入.重试边界
                    == L1所有者范围重试边界::原幂等身份读回收敛;
            if (写入.事实代次 == 0 || 写入.事实代次 <= 期望代次
                || (!首次提交完整 && !重复提交完整))
                return 形成概念支持写入失败(
                    L2结构状态::内部不一致, 写入.事实代次);
            const auto 首次确认 = 第一层写入端口_.读取首次写入材料(
                {L1所有者范围首次写入读取合同版本, 写集.写入幂等身份});
            if (首次确认.状态 != L1所有者范围读取状态::成功)
                return 形成概念支持写入失败(
                    首次确认.状态 == L1所有者范围读取状态::资源失败
                        ? L2结构状态::资源失败 : L2结构状态::内部不一致,
                    首次确认.读取事实代次);
            if (首次确认.合同版本 != L1所有者范围首次写入读取合同版本
                || 首次确认.所有者 != 第一层写入端口_.所有者身份()
                || 首次确认.写入幂等身份 != 写集.写入幂等身份
                || 首次确认.读取事实代次 == 0
                || !首次确认.首次规范化写集 || !首次确认.首次写入结果
                || *首次确认.首次规范化写集 != 写集)
                return 形成概念支持写入失败(
                    L2结构状态::内部不一致, 首次确认.读取事实代次);
            const auto& 首次结果 = *首次确认.首次写入结果;
            if (首次结果.状态 != L1所有者范围写入状态::成功
                || 首次结果.事实代次 != 写入.事实代次
                || 首次结果.新编码映射 != 写入.新编码映射)
                return 形成概念支持写入失败(
                    L2结构状态::内部不一致, 首次结果.事实代次);
            return 重建概念支持首次结果(第一层服务_, 身份来源定位_,
                *支持结构定位_, 写集, 首次结果, 写入状态,
                新关系, 旧内部, 首次确认.读取事实代次);
        } catch (const std::bad_alloc&) {
            return 形成概念支持写入失败(L2结构状态::资源失败);
        } catch (const std::length_error&) {
            return 形成概念支持写入失败(L2结构状态::资源失败);
        } catch (...) {
            return 形成概念支持写入失败(L2结构状态::内部不一致);
        }
    }

    // 诊断责任：向上送出；三种直接关系写共用首次材料、值式图裁决和原子提交。
    L2概念直接上位写入结果 执行概念直接上位关系写入(
        std::uint64_t 期望代次, L2结构幂等身份 幂等身份,
        std::optional<L2概念结构内部::直接关系期望> 新关系,
        std::optional<std::pair<稳定编码,
            L2概念结构内部::直接关系期望>> 旧关系) {
        using namespace L2概念结构内部;
        if (!普通概念依赖_.完整())
            return 形成概念关系写入失败(L2结构状态::未实现);
        try {
            std::lock_guard<std::mutex> 锁(概念写入锁_);
            const auto 写集 = 形成直接关系写集(期望代次, 幂等身份,
                本体结构定位_, 新关系,
                旧关系 ? std::optional<稳定编码>{旧关系->first} : std::nullopt);
            const auto 既有幂等 = 第一层写入端口_.读取首次写入材料(
                {L1所有者范围首次写入读取合同版本, 写集.写入幂等身份});
            const bool 幂等头完整 = 既有幂等.合同版本
                    == L1所有者范围首次写入读取合同版本
                && 既有幂等.所有者 == 第一层写入端口_.所有者身份()
                && 既有幂等.写入幂等身份 == 写集.写入幂等身份
                && 既有幂等.读取事实代次 != 0;
            if (!幂等头完整)
                return 形成概念关系写入失败(
                    L2结构状态::内部不一致, 既有幂等.读取事实代次);
            if (既有幂等.状态 == L1所有者范围读取状态::成功) {
                if (!既有幂等.首次规范化写集 || !既有幂等.首次写入结果)
                    return 形成概念关系写入失败(
                        L2结构状态::内部不一致, 既有幂等.读取事实代次);
                if (*既有幂等.首次规范化写集 != 写集)
                    return 形成概念关系写入失败(
                        L2结构状态::幂等冲突, 既有幂等.读取事实代次);
                const auto& 首次结果 = *既有幂等.首次写入结果;
                if (首次结果.事实代次 == 0
                    || 首次结果.事实代次 <= 期望代次
                    || 首次结果.事实代次 > 既有幂等.读取事实代次)
                    return 形成概念关系写入失败(
                        L2结构状态::内部不一致, 既有幂等.读取事实代次);
                return 重建直接关系首次结果(第一层服务_, 身份来源定位_,
                    本体结构定位_, 写集, 首次结果, L2结构状态::精确重复,
                    新关系, 旧关系);
            }
            if (既有幂等.首次规范化写集 || 既有幂等.首次写入结果)
                return 形成概念关系写入失败(
                    L2结构状态::内部不一致, 既有幂等.读取事实代次);
            if (既有幂等.状态 != L1所有者范围读取状态::未找到)
                return 形成概念关系写入失败(
                    映射历史读取状态(既有幂等.状态), 既有幂等.读取事实代次);
            if (既有幂等.读取事实代次 != 期望代次)
                return 形成概念关系写入失败(
                    L2结构状态::事实代次漂移, 既有幂等.读取事实代次);

            auto 图读取 = 读取当前概念图(期望代次);
            if (图读取.状态 != L2结构状态::已读取 || !图读取.图)
                return 形成概念关系写入失败(
                    图读取.状态 == L2结构状态::已读取
                        ? L2结构状态::内部不一致 : 图读取.状态,
                    图读取.截止);
            auto& 图 = *图读取.图;
            const auto 查端点 = [&图](L2概念身份 概念)
                -> std::optional<std::pair<L2概念身份, bool>> {
                for (const auto& 根 : 图.本体根)
                    if (根 == 概念) return std::pair{根, true};
                for (const auto& 节点 : 图.普通概念)
                    if (节点.概念 == 概念) return std::pair{节点.本体根, false};
                return std::nullopt;
            };
            if (新关系) {
                const auto 上位 = 查端点(新关系->上位);
                const auto 下位 = 查端点(新关系->下位);
                if (!上位 || !下位)
                    return 形成概念关系写入失败(
                        L2结构状态::未找到, 期望代次);
                if (下位->second || 上位->first != 下位->first)
                    return 形成概念关系写入失败(
                        L2结构状态::入口拒绝, 期望代次);
            }
            if (旧关系) {
                const auto 上位 = 查端点(旧关系->second.上位);
                const auto 下位 = 查端点(旧关系->second.下位);
                if (!上位 || !下位)
                    return 形成概念关系写入失败(
                        L2结构状态::未找到, 期望代次);
                if (下位->second || 上位->first != 下位->first)
                    return 形成概念关系写入失败(
                        L2结构状态::入口拒绝, 期望代次);
            }

            std::optional<std::size_t> 旧边索引;
            bool 旧编码命中 = false;
            bool 旧端点命中 = false;
            if (旧关系) {
                for (std::size_t 索引 = 0; 索引 < 图.边.size(); ++索引) {
                    const auto& 边 = 图.边[索引];
                    const bool 编码相同 = 边.关系 == 旧关系->first;
                    const bool 端点相同 = 边.上位 == 旧关系->second.上位
                        && 边.下位 == 旧关系->second.下位;
                    旧编码命中 = 旧编码命中 || 编码相同;
                    旧端点命中 = 旧端点命中 || 端点相同;
                    if (编码相同 && 端点相同) 旧边索引 = 索引;
                }
                if (!旧边索引) {
                    const auto 状态 = 旧编码命中 || 旧端点命中
                        ? L2结构状态::引用冲突 : L2结构状态::未找到;
                    return 形成概念关系写入失败(状态, 期望代次);
                }
            }
            if (新关系) {
                for (std::size_t 索引 = 0; 索引 < 图.边.size(); ++索引) {
                    if (旧边索引 && 索引 == *旧边索引) continue;
                    const auto& 边 = 图.边[索引];
                    if (边.上位 == 新关系->上位 && 边.下位 == 新关系->下位)
                        return 形成概念关系写入失败(
                            L2结构状态::引用冲突, 期望代次);
                }
            }
            if (旧边索引) 图.边.erase(图.边.begin() + *旧边索引);
            if (新关系) 图.边.push_back(
                {稳定编码{}, 新关系->上位, 新关系->下位});
            if (!当前概念图完整(图, 新关系.has_value()))
                return 形成概念关系写入失败(
                    L2结构状态::引用冲突, 期望代次);

            const auto 写入 = 第一层写入端口_.提交所有者范围中性写集(写集);
            if (写入.合同版本 != L1所有者范围CRUD合同版本
                || 写入.所有者 != 第一层写入端口_.所有者身份()
                || 写入.写入幂等身份 != 写集.写入幂等身份)
                return 形成概念关系写入失败(
                    L2结构状态::内部不一致, 写入.事实代次);
            const auto 写入状态 = 映射写入状态(写入.状态);
            if (写入状态 != L2结构状态::已提交
                && 写入状态 != L2结构状态::精确重复) {
                if (!写入.新编码映射.empty() || 写入.是否形成内存权威发布)
                    return 形成概念关系写入失败(
                        L2结构状态::内部不一致, 写入.事实代次);
                return 形成概念关系写入失败(写入状态, 写入.事实代次);
            }
            const bool 提交首次完整 = 写入.状态 == L1所有者范围写入状态::成功
                && 写入.是否形成内存权威发布
                && 写入.重试边界 == L1所有者范围重试边界::不适用;
            const bool 提交重复完整 = 写入.状态
                    == L1所有者范围写入状态::精确重复
                && !写入.是否形成内存权威发布
                && 写入.重试边界
                    == L1所有者范围重试边界::原幂等身份读回收敛;
            if (写入.事实代次 == 0 || 写入.事实代次 <= 期望代次
                || (!提交首次完整 && !提交重复完整))
                return 形成概念关系写入失败(
                    L2结构状态::内部不一致, 写入.事实代次);
            const auto 首次读取 = 第一层写入端口_.读取首次写入材料(
                {L1所有者范围首次写入读取合同版本, 写集.写入幂等身份});
            if (首次读取.状态 != L1所有者范围读取状态::成功)
                return 形成概念关系写入失败(
                    首次读取.状态 == L1所有者范围读取状态::资源失败
                        ? L2结构状态::资源失败 : L2结构状态::内部不一致,
                    首次读取.读取事实代次);
            if (首次读取.合同版本 != L1所有者范围首次写入读取合同版本
                || 首次读取.所有者 != 第一层写入端口_.所有者身份()
                || 首次读取.写入幂等身份 != 写集.写入幂等身份
                || 首次读取.读取事实代次 == 0
                || !首次读取.首次规范化写集 || !首次读取.首次写入结果
                || *首次读取.首次规范化写集 != 写集)
                return 形成概念关系写入失败(
                    L2结构状态::内部不一致, 首次读取.读取事实代次);
            const auto& 首次结果 = *首次读取.首次写入结果;
            if (首次结果.状态 != L1所有者范围写入状态::成功
                || 首次结果.合同版本 != L1所有者范围CRUD合同版本
                || 首次结果.所有者 != 第一层写入端口_.所有者身份()
                || 首次结果.写入幂等身份 != 写集.写入幂等身份
                || 首次结果.事实代次 == 0 || 首次结果.事实代次 <= 期望代次
                || 首次结果.事实代次 > 首次读取.读取事实代次
                || !首次结果.是否形成内存权威发布
                || 首次结果.重试边界 != L1所有者范围重试边界::不适用
                || 写入.事实代次 != 首次结果.事实代次
                || 写入.新编码映射 != 首次结果.新编码映射)
                return 形成概念关系写入失败(
                    L2结构状态::内部不一致, 首次结果.事实代次);
            return 重建直接关系首次结果(第一层服务_, 身份来源定位_,
                本体结构定位_, 写集, 首次结果, 写入状态,
                新关系, 旧关系);
        } catch (const std::bad_alloc&) {
            return 形成概念关系写入失败(L2结构状态::资源失败);
        } catch (const std::length_error&) {
            return 形成概念关系写入失败(L2结构状态::资源失败);
        } catch (...) {
            return 形成概念关系写入失败(L2结构状态::内部不一致);
        }
    }

    // 诊断责任：向上送出；根或普通概念端点只经同一服务公开读回确定所属根。
    L2概念结构内部::概念端点读取值 读取概念端点归属(
        L2概念身份 概念, L2读取类别 类别,
        std::uint64_t 历史截止, std::uint64_t 期望代次) const {
        using namespace L2概念结构内部;
        const std::uint64_t 投影截止 = 类别 == L2读取类别::历史
            ? 历史截止 : 期望代次;
        const auto 根组 = 读取全部当前概念本体根(
            {{L2结构合同版本, 期望代次}});
        if (!根组.成功())
            return {根组.结果头.状态, 根组.结果头.事实截止代次};
        for (const auto& 根 : 根组.本体根) {
            if (根.根概念 != 概念) continue;
            if (根.根角色.生命周期.创建事实代次 > 投影截止)
                return {L2结构状态::未找到, 投影截止};
            return {L2结构状态::已读取, 投影截止, 根.根概念, true};
        }
        const auto 读取 = 读取普通概念({
            {L2结构合同版本, 期望代次}, L2普通概念结构合同版本,
            类别, 概念, 类别 == L2读取类别::历史 ? 历史截止 : 0});
        if (!读取.成功() || !读取.概念)
            return {读取.结果头.状态, 读取.结果头.事实截止代次};
        return {L2结构状态::已读取, 投影截止,
            读取.概念->本体根归属.本体根, false};
    }

    // 诊断责任：向上送出；在一个非零守卫上重建全部当前普通概念及直接边。
    L2概念结构内部::当前概念图读取值 读取当前概念图(
        std::uint64_t 期望代次) const {
        using namespace L2概念结构内部;
        const auto 根组 = 读取全部当前概念本体根(
            {{L2结构合同版本, 期望代次}});
        if (!根组.成功())
            return {根组.结果头.状态, 根组.结果头.事实截止代次};
        当前概念图 图;
        for (const auto& 根 : 根组.本体根) 图.本体根.push_back(根.根概念);
        const auto 身份组 = 读取全部当前普通概念身份(
            第一层服务_, 身份来源定位_, 普通结构定位_, 期望代次);
        if (身份组.状态 != L2结构状态::已读取)
            return {身份组.状态, 身份组.截止};
        for (const auto 身份 : 身份组.身份) {
            const auto 读取 = 读取普通概念({
                {L2结构合同版本, 期望代次}, L2普通概念结构合同版本,
                L2读取类别::当前, 身份, 0});
            if (!读取.成功() || !读取.概念)
                return {读取.结果头.状态 == L2结构状态::已读取
                        ? L2结构状态::内部不一致 : 读取.结果头.状态,
                    读取.结果头.事实截止代次};
            图.普通概念.push_back(
                {身份, 读取.概念->本体根归属.本体根});
            for (const auto& 关系 : 读取.概念->直接上位)
                图.边.push_back({关系.关系稳定编码,
                    关系.上位概念, 关系.下位概念});
        }
        if (!当前概念图完整(图, false))
            return {L2结构状态::内部不一致, 期望代次};
        const auto 尾代次 = 读取当前事实代次(第一层服务_);
        if (尾代次.状态 != L1所有者范围读取状态::成功)
            return {尾代次.状态 == L1所有者范围读取状态::资源失败
                    ? L2结构状态::资源失败 : L2结构状态::内部不一致,
                尾代次.事实代次};
        if (尾代次.事实代次 != 期望代次)
            return {L2结构状态::事实代次漂移, 尾代次.事实代次};
        return {L2结构状态::已读取, 期望代次, std::move(图)};
    }

    // 诊断责任：向上送出；交付必须有效且绑定同一 L1 实例。
    static L1所有者范围写端口&& 验证并移动交付(
        const L1事实基座服务& 第一层服务, L2概念所有者交付& 交付) {
        if (!交付.写入端口_.有效() || !交付.写入端口_.绑定于(第一层服务))
            throw std::invalid_argument("L2 concept owner delivery is invalid");
        return std::move(交付.写入端口_);
    }

    const L1事实基座服务& 第一层服务_;
    L1所有者范围写端口 第一层写入端口_;
    L2概念结构内部::概念身份来源定位 身份来源定位_;
    L2概念结构内部::概念本体结构定位 本体结构定位_;
    L2概念结构内部::普通概念结构定位 普通结构定位_;
    std::optional<L2概念结构内部::概念世界事实支持结构定位>
        支持结构定位_;
    L2概念结构内部::普通概念公开依赖 普通概念依赖_;
    std::mutex 概念写入锁_;
};

} // namespace 海中鱼巣
