#pragma once

#ifndef L2_ORDINARY_CONCEPT_STRUCTURE_NO_INCLUDES
#include <cstddef>
#include <cstdint>
#include <optional>
#include <type_traits>
#include <variant>
#include <vector>
#include "L2结构公共.数据.h"
#include "不可变材料.数据.h"
#include "L2状态结构.数据.h"
#endif

namespace 海中鱼巣 {

inline constexpr std::uint32_t L2普通概念结构合同版本 = 2;
inline constexpr std::uint32_t L2概念定义合同版本 = 1;
inline constexpr std::uint32_t L2普通概念退出合同版本 = 1;
inline constexpr std::uint32_t L2概念直接上下位关系合同版本 = 1;
inline constexpr std::uint32_t L2概念世界事实支持关系合同版本 = 1;
inline constexpr std::uint32_t L2概念治理生命周期合同版本 = 1;

enum class L2概念治理生命周期状态 : std::uint8_t {
    活跃 = 1,
    冷却 = 2,
    退役 = 3
};

struct L2概念特征值引用 final {
    L2特征实例身份 特征实例;
    稳定编码 值稳定编码;
    friend bool operator==(const L2概念特征值引用&,
        const L2概念特征值引用&) = default;
};

struct L2概念动作引用 final {
    稳定编码 值;
    friend bool operator==(const L2概念动作引用&,
        const L2概念动作引用&) = default;
};

using L2概念世界关系端点 = std::variant<L2场景身份, L2存在身份,
    L2特征定义身份, L2特征实例身份, L2状态身份, L2动态身份,
    L2因果身份, L2概念动作引用>;

enum class L2概念世界关系类别 : std::uint8_t {
    场景父子 = 1,
    场景成员 = 2,
    场景宿主存在 = 3,
    特征宿主 = 4,
    特征定义 = 5,
    状态主体 = 6,
    状态特征实例 = 7,
    动态主体 = 8,
    动态前状态 = 9,
    动态后状态 = 10,
    因果条件状态 = 11,
    因果动作引用 = 12,
    因果结果状态 = 13
};

struct L2概念世界关系引用 final {
    L2概念世界关系类别 类别 = L2概念世界关系类别::场景父子;
    稳定编码 关系稳定编码;
    L2概念世界关系端点 源;
    L2概念世界关系端点 目标;
    friend bool operator==(const L2概念世界关系引用&,
        const L2概念世界关系引用&) = default;
};

struct L2新存在引用 final {
    稳定编码 值{};
    friend bool operator==(const L2新存在引用&, const L2新存在引用&) = default;
};
struct L2新特征类型引用 final {
    稳定编码 值{};
    friend bool operator==(const L2新特征类型引用&, const L2新特征类型引用&) = default;
};
struct L2新特征实例引用 final {
    稳定编码 值{};
    friend bool operator==(const L2新特征实例引用&, const L2新特征实例引用&) = default;
};
struct L2新特征值引用 final {
    L2新特征实例引用 实例;
    稳定编码 值事实{};
    friend bool operator==(const L2新特征值引用&, const L2新特征值引用&) = default;
};

using L2概念定义目标 = std::variant<L2概念身份, L2场景身份, L2存在身份, L2特征定义身份, L2特征实例身份,
                                    L2概念特征值引用, L2状态身份, L2动态身份, L2因果身份, L2概念世界关系引用,
                                    L2新存在引用, L2新特征类型引用, L2新特征实例引用, L2新特征值引用>;

using L2世界事实引用 = std::variant<L2场景身份, L2存在身份, L2特征定义身份, L2特征实例身份, L2概念特征值引用,
                                    L2状态身份, L2动态身份, L2因果身份, L2概念世界关系引用, L2新存在引用,
                                    L2新特征类型引用, L2新特征实例引用, L2新特征值引用>;

struct L2概念签名值式项 final {
    std::uint64_t 角色 = 0;
    std::uint64_t 顺序 = 0;
    L2原始值材料 值;
    friend bool operator==(const L2概念签名值式项&,
        const L2概念签名值式项&) = default;
};

struct L2概念定义引用项 final {
    std::uint64_t 角色 = 0;
    std::uint64_t 顺序 = 0;
    L2概念定义目标 目标;
    friend bool operator==(const L2概念定义引用项&,
        const L2概念定义引用项&) = default;
};

struct L2概念定义角色合同 final {
    std::uint64_t 角色 = 0;
    std::size_t 最小数量 = 0;
    std::size_t 最大数量 = 0;
    friend bool operator==(const L2概念定义角色合同&,
        const L2概念定义角色合同&) = default;
};

struct L2概念本体根归属事实 final {
    L2概念身份 本体根;
    L2概念身份 概念;
    稳定编码 关系稳定编码;
    L2生命周期 生命周期;
    friend bool operator==(const L2概念本体根归属事实&,
        const L2概念本体根归属事实&) = default;
};

struct L2概念签名规则事实 final {
    L2概念身份 概念;
    不可变材料身份 规则材料;
    稳定编码 关系稳定编码;
    L2生命周期 生命周期;
    friend bool operator==(const L2概念签名规则事实&,
        const L2概念签名规则事实&) = default;
};

struct L2概念签名值式事实 final {
    L2概念身份 概念;
    L2概念签名值式项 项;
    稳定编码 值稳定编码;
    L2生命周期 生命周期;
    friend bool operator==(const L2概念签名值式事实&,
        const L2概念签名值式事实&) = default;
};

struct L2概念定义关系事实 final {
    L2概念身份 概念;
    L2概念定义引用项 项;
    稳定编码 关系稳定编码;
    L2生命周期 生命周期;
    friend bool operator==(const L2概念定义关系事实&,
        const L2概念定义关系事实&) = default;
};

struct L2概念定义角色合同事实 final {
    L2概念身份 概念;
    L2概念定义角色合同 合同;
    稳定编码 成员关系稳定编码;
    稳定编码 角色值稳定编码;
    稳定编码 最小数量值稳定编码;
    稳定编码 最大数量值稳定编码;
    L2生命周期 生命周期;
    friend bool operator==(const L2概念定义角色合同事实&,
        const L2概念定义角色合同事实&) = default;
};

struct L2概念世界事实支持关系事实 final {
    L2世界事实引用 世界事实;
    L2概念身份 概念;
    不可变材料身份 建立依据材料;
    std::vector<不可变材料身份> 发布证据材料;
    稳定编码 关系稳定编码;
    L2生命周期 生命周期;
    friend bool operator==(const L2概念世界事实支持关系事实&,
        const L2概念世界事实支持关系事实&) = default;
};

struct L2概念直接上位事实 final {
    L2概念身份 上位概念;
    L2概念身份 下位概念;
    稳定编码 关系稳定编码;
    L2生命周期 生命周期;
    friend bool operator==(const L2概念直接上位事实&,
        const L2概念直接上位事实&) = default;
};

struct L2概念来源证据事实 final {
    L2概念身份 概念;
    不可变材料身份 证据材料;
    稳定编码 关系稳定编码;
    L2生命周期 生命周期;
    friend bool operator==(const L2概念来源证据事实&,
        const L2概念来源证据事实&) = default;
};

struct L2概念治理生命周期事实 final {
    L2概念身份 概念;
    L2概念治理生命周期状态 状态 = L2概念治理生命周期状态::活跃;
    稳定编码 事实稳定编码;
    L2生命周期 生命周期;
    friend bool operator==(const L2概念治理生命周期事实&,
        const L2概念治理生命周期事实&) = default;
};

struct L2普通概念事实 final {
    L2概念身份 概念;
    L2节点族身份来源事实 身份来源;
    L2概念本体根归属事实 本体根归属;
    L2概念签名规则事实 签名规则;
    std::vector<L2概念签名值式事实> 签名值式项;
    std::vector<L2概念定义角色合同事实> 定义角色合同;
    std::vector<L2概念定义关系事实> 定义关系;
    std::vector<L2概念直接上位事实> 直接上位;
    std::vector<L2概念来源证据事实> 来源证据;
    L2概念治理生命周期事实 治理生命周期;
    friend bool operator==(const L2普通概念事实&,
        const L2普通概念事实&) = default;
};

struct L2普通概念建立请求 final {
    L2结构请求头 请求头;
    std::uint32_t 普通概念合同版本 = L2普通概念结构合同版本;
    L2结构幂等身份 幂等身份;
    L2概念身份 本体根;
    不可变材料身份 签名规则材料;
    std::vector<L2概念签名值式项> 签名值式项;
    std::vector<L2概念定义角色合同> 定义角色合同;
    std::vector<L2概念定义引用项> 初始定义;
    std::vector<L2概念身份> 直接上位;
    std::vector<不可变材料身份> 来源证据材料;
    friend bool operator==(const L2普通概念建立请求&,
        const L2普通概念建立请求&) = default;
};

struct L2普通概念写入结果 final {
    L2结构结果头 结果头;
    std::optional<L2普通概念事实> 概念;
    bool 成功() const noexcept;
    friend bool operator==(const L2普通概念写入结果&,
        const L2普通概念写入结果&) = default;
};

struct L2普通概念读取请求 final {
    L2结构请求头 请求头;
    std::uint32_t 普通概念合同版本 = L2普通概念结构合同版本;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2概念身份 概念;
    std::uint64_t 历史截止事实代次 = 0;
    friend bool operator==(const L2普通概念读取请求&,
        const L2普通概念读取请求&) = default;
};

struct L2普通概念读取结果 final {
    L2结构结果头 结果头;
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止事实代次 = 0;
    std::optional<L2普通概念事实> 概念;
    bool 成功() const noexcept;
    friend bool operator==(const L2普通概念读取结果&,
        const L2普通概念读取结果&) = default;
};

struct L2按概念定义组读取请求 final {
    L2结构请求头 请求头;
    std::uint32_t 定义合同版本 = L2概念定义合同版本;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2概念身份 概念;
    std::uint64_t 历史截止事实代次 = 0;
    std::size_t 数量预算 = 0;
    friend bool operator==(const L2按概念定义组读取请求&,
        const L2按概念定义组读取请求&) = default;
};

struct L2按定义目标概念组读取请求 final {
    L2结构请求头 请求头;
    std::uint32_t 定义合同版本 = L2概念定义合同版本;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2概念定义目标 目标;
    std::uint64_t 历史截止事实代次 = 0;
    std::size_t 扫描预算 = 0;
    std::size_t 数量预算 = 0;
    friend bool operator==(const L2按定义目标概念组读取请求&,
        const L2按定义目标概念组读取请求&) = default;
};

struct L2概念定义组读取结果 final {
    L2结构结果头 结果头;
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止事实代次 = 0;
    std::vector<L2概念定义角色合同事实> 角色合同组;
    std::vector<L2概念定义关系事实> 定义关系组;
    bool 成功() const noexcept;
    friend bool operator==(const L2概念定义组读取结果&,
        const L2概念定义组读取结果&) = default;
};

struct L2普通概念退出请求 final {
    L2结构请求头 请求头;
    std::uint32_t 退出合同版本 = L2普通概念退出合同版本;
    L2结构幂等身份 幂等身份;
    L2概念身份 概念;
    稳定编码 当前退役生命周期事实稳定编码;
    std::size_t 图扫描预算 = 0;
    std::size_t 关系数量预算 = 0;
    friend bool operator==(const L2普通概念退出请求&,
        const L2普通概念退出请求&) = default;
};

struct L2普通概念退出结果 final {
    L2结构结果头 结果头;
    std::optional<L2普通概念事实> 已退出概念;
    std::vector<L2概念直接上位事实> 已退出直接关系;
    std::vector<L2概念直接上位事实> 新增替代关系;
    bool 成功() const noexcept;
    friend bool operator==(const L2普通概念退出结果&,
        const L2普通概念退出结果&) = default;
};

struct L2概念治理生命周期迁移请求 final {
    L2结构请求头 请求头;
    std::uint32_t 生命周期合同版本 = L2概念治理生命周期合同版本;
    L2结构幂等身份 幂等身份;
    L2概念身份 概念;
    稳定编码 当前生命周期事实稳定编码;
    L2概念治理生命周期状态 当前状态 = L2概念治理生命周期状态::活跃;
    L2概念治理生命周期状态 目标状态 = L2概念治理生命周期状态::活跃;
    friend bool operator==(const L2概念治理生命周期迁移请求&,
        const L2概念治理生命周期迁移请求&) = default;
};

struct L2概念治理生命周期写入结果 final {
    L2结构结果头 结果头;
    std::optional<L2概念治理生命周期事实> 当前生命周期;
    std::optional<L2概念治理生命周期事实> 已退出生命周期;
    bool 成功() const noexcept;
    friend bool operator==(const L2概念治理生命周期写入结果&,
        const L2概念治理生命周期写入结果&) = default;
};

struct L2概念治理生命周期读取请求 final {
    L2结构请求头 请求头;
    std::uint32_t 生命周期合同版本 = L2概念治理生命周期合同版本;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2概念身份 概念;
    std::uint64_t 历史截止事实代次 = 0;
    friend bool operator==(const L2概念治理生命周期读取请求&,
        const L2概念治理生命周期读取请求&) = default;
};

struct L2概念治理生命周期读取结果 final {
    L2结构结果头 结果头;
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止事实代次 = 0;
    std::optional<L2概念治理生命周期事实> 生命周期;
    bool 成功() const noexcept;
    friend bool operator==(const L2概念治理生命周期读取结果&,
        const L2概念治理生命周期读取结果&) = default;
};

struct L2概念直接上位新增请求 final {
    L2结构请求头 请求头;
    std::uint32_t 关系合同版本 = L2概念直接上下位关系合同版本;
    L2结构幂等身份 幂等身份;
    L2概念身份 上位概念;
    L2概念身份 下位概念;
    friend bool operator==(const L2概念直接上位新增请求&,
        const L2概念直接上位新增请求&) = default;
};

struct L2概念直接上位替换请求 final {
    L2结构请求头 请求头;
    std::uint32_t 关系合同版本 = L2概念直接上下位关系合同版本;
    L2结构幂等身份 幂等身份;
    稳定编码 旧关系稳定编码;
    L2概念身份 旧上位概念;
    L2概念身份 下位概念;
    L2概念身份 新上位概念;
    friend bool operator==(const L2概念直接上位替换请求&,
        const L2概念直接上位替换请求&) = default;
};

struct L2概念直接上位退出请求 final {
    L2结构请求头 请求头;
    std::uint32_t 关系合同版本 = L2概念直接上下位关系合同版本;
    L2结构幂等身份 幂等身份;
    稳定编码 关系稳定编码;
    L2概念身份 上位概念;
    L2概念身份 下位概念;
    friend bool operator==(const L2概念直接上位退出请求&,
        const L2概念直接上位退出请求&) = default;
};

struct L2概念直接上位写入结果 final {
    L2结构结果头 结果头;
    std::optional<L2概念直接上位事实> 当前关系;
    std::optional<L2概念直接上位事实> 已退出关系;
    bool 成功() const noexcept;
    friend bool operator==(const L2概念直接上位写入结果&,
        const L2概念直接上位写入结果&) = default;
};

struct L2概念直接上位组读取请求 final {
    L2结构请求头 请求头;
    std::uint32_t 关系合同版本 = L2概念直接上下位关系合同版本;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2概念身份 下位概念;
    std::uint64_t 历史截止事实代次 = 0;
    std::size_t 数量预算 = 0;
    friend bool operator==(const L2概念直接上位组读取请求&,
        const L2概念直接上位组读取请求&) = default;
};

struct L2概念直接下位组读取请求 final {
    L2结构请求头 请求头;
    std::uint32_t 关系合同版本 = L2概念直接上下位关系合同版本;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2概念身份 上位概念;
    std::uint64_t 历史截止事实代次 = 0;
    std::size_t 数量预算 = 0;
    friend bool operator==(const L2概念直接下位组读取请求&,
        const L2概念直接下位组读取请求&) = default;
};

struct L2概念直接上位组读取结果 final {
    L2结构结果头 结果头;
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止事实代次 = 0;
    std::vector<L2概念直接上位事实> 关系组;
    bool 成功() const noexcept;
    friend bool operator==(const L2概念直接上位组读取结果&,
        const L2概念直接上位组读取结果&) = default;
};

using L2概念直接下位组读取结果 = L2概念直接上位组读取结果;

struct L2概念世界事实支持新增请求 final {
    L2结构请求头 请求头;
    std::uint32_t 关系合同版本 = L2概念世界事实支持关系合同版本;
    L2结构幂等身份 幂等身份;
    L2世界事实引用 世界事实;
    L2概念身份 概念;
    不可变材料身份 建立依据材料;
    std::vector<不可变材料身份> 发布证据材料;
    friend bool operator==(const L2概念世界事实支持新增请求&,
        const L2概念世界事实支持新增请求&) = default;
};

struct L2概念世界事实支持替换请求 final {
    L2结构请求头 请求头;
    std::uint32_t 关系合同版本 = L2概念世界事实支持关系合同版本;
    L2结构幂等身份 幂等身份;
    稳定编码 旧关系稳定编码;
    L2世界事实引用 旧世界事实;
    L2概念身份 概念;
    L2世界事实引用 新世界事实;
    不可变材料身份 新建立依据材料;
    std::vector<不可变材料身份> 新发布证据材料;
    friend bool operator==(const L2概念世界事实支持替换请求&,
        const L2概念世界事实支持替换请求&) = default;
};

struct L2概念世界事实支持退出请求 final {
    L2结构请求头 请求头;
    std::uint32_t 关系合同版本 = L2概念世界事实支持关系合同版本;
    L2结构幂等身份 幂等身份;
    稳定编码 关系稳定编码;
    L2世界事实引用 世界事实;
    L2概念身份 概念;
    friend bool operator==(const L2概念世界事实支持退出请求&,
        const L2概念世界事实支持退出请求&) = default;
};

struct L2概念世界事实支持写入结果 final {
    L2结构结果头 结果头;
    std::optional<L2概念世界事实支持关系事实> 当前关系;
    std::optional<L2概念世界事实支持关系事实> 已退出关系;
    bool 成功() const noexcept;
    friend bool operator==(const L2概念世界事实支持写入结果&,
        const L2概念世界事实支持写入结果&) = default;
};

struct L2按概念世界事实支持组读取请求 final {
    L2结构请求头 请求头;
    std::uint32_t 关系合同版本 = L2概念世界事实支持关系合同版本;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2概念身份 概念;
    std::uint64_t 历史截止事实代次 = 0;
    std::size_t 数量预算 = 0;
    friend bool operator==(const L2按概念世界事实支持组读取请求&,
        const L2按概念世界事实支持组读取请求&) = default;
};

struct L2按世界事实支持概念组读取请求 final {
    L2结构请求头 请求头;
    std::uint32_t 关系合同版本 = L2概念世界事实支持关系合同版本;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2世界事实引用 世界事实;
    std::uint64_t 历史截止事实代次 = 0;
    std::size_t 扫描预算 = 0;
    std::size_t 数量预算 = 0;
    friend bool operator==(const L2按世界事实支持概念组读取请求&,
        const L2按世界事实支持概念组读取请求&) = default;
};

struct L2概念世界事实支持组读取结果 final {
    L2结构结果头 结果头;
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止事实代次 = 0;
    std::vector<L2概念世界事实支持关系事实> 关系组;
    bool 成功() const noexcept;
    friend bool operator==(const L2概念世界事实支持组读取结果&,
        const L2概念世界事实支持组读取结果&) = default;
};

// 诊断责任：无适用错误分支；只校验闭合关系类别、非零关系身份与强类型端点方向。
inline bool L2概念世界关系引用有效(
    const L2概念世界关系引用& 引用) noexcept {
    if (!有效(引用.关系稳定编码)) return false;
    const auto 场景到场景 = [&引用]() noexcept {
        const auto* 源 = std::get_if<L2场景身份>(&引用.源);
        const auto* 目标 = std::get_if<L2场景身份>(&引用.目标);
        return 源 && 目标 && 有效(源->值) && 有效(目标->值) && *源 != *目标;
    };
    const auto 场景到存在 = [&引用]() noexcept {
        const auto* 源 = std::get_if<L2场景身份>(&引用.源);
        const auto* 目标 = std::get_if<L2存在身份>(&引用.目标);
        return 源 && 目标 && 有效(源->值) && 有效(目标->值);
    };
    const auto 特征实例到存在 = [&引用]() noexcept {
        const auto* 源 = std::get_if<L2特征实例身份>(&引用.源);
        const auto* 目标 = std::get_if<L2存在身份>(&引用.目标);
        return 源 && 目标 && 有效(源->值) && 有效(目标->值);
    };
    const auto 特征实例到定义 = [&引用]() noexcept {
        const auto* 源 = std::get_if<L2特征实例身份>(&引用.源);
        const auto* 目标 = std::get_if<L2特征定义身份>(&引用.目标);
        return 源 && 目标 && 有效(源->值) && 有效(目标->值);
    };
    const auto 状态到存在 = [&引用]() noexcept {
        const auto* 源 = std::get_if<L2状态身份>(&引用.源);
        const auto* 目标 = std::get_if<L2存在身份>(&引用.目标);
        return 源 && 目标 && 有效(源->值) && 有效(目标->值);
    };
    const auto 状态到特征实例 = [&引用]() noexcept {
        const auto* 源 = std::get_if<L2状态身份>(&引用.源);
        const auto* 目标 = std::get_if<L2特征实例身份>(&引用.目标);
        return 源 && 目标 && 有效(源->值) && 有效(目标->值);
    };
    const auto 动态到存在 = [&引用]() noexcept {
        const auto* 源 = std::get_if<L2动态身份>(&引用.源);
        const auto* 目标 = std::get_if<L2存在身份>(&引用.目标);
        return 源 && 目标 && 有效(源->值) && 有效(目标->值);
    };
    const auto 动态到状态 = [&引用]() noexcept {
        const auto* 源 = std::get_if<L2动态身份>(&引用.源);
        const auto* 目标 = std::get_if<L2状态身份>(&引用.目标);
        return 源 && 目标 && 有效(源->值) && 有效(目标->值);
    };
    const auto 因果到状态 = [&引用]() noexcept {
        const auto* 源 = std::get_if<L2因果身份>(&引用.源);
        const auto* 目标 = std::get_if<L2状态身份>(&引用.目标);
        return 源 && 目标 && 有效(源->值) && 有效(目标->值);
    };
    const auto 因果到动作 = [&引用]() noexcept {
        const auto* 源 = std::get_if<L2因果身份>(&引用.源);
        const auto* 目标 = std::get_if<L2概念动作引用>(&引用.目标);
        return 源 && 目标 && 有效(源->值) && 有效(目标->值);
    };
    switch (引用.类别) {
    case L2概念世界关系类别::场景父子: return 场景到场景();
    case L2概念世界关系类别::场景成员:
    case L2概念世界关系类别::场景宿主存在: return 场景到存在();
    case L2概念世界关系类别::特征宿主: return 特征实例到存在();
    case L2概念世界关系类别::特征定义: return 特征实例到定义();
    case L2概念世界关系类别::状态主体: return 状态到存在();
    case L2概念世界关系类别::状态特征实例: return 状态到特征实例();
    case L2概念世界关系类别::动态主体: return 动态到存在();
    case L2概念世界关系类别::动态前状态:
    case L2概念世界关系类别::动态后状态: return 动态到状态();
    case L2概念世界关系类别::因果条件状态:
    case L2概念世界关系类别::因果结果状态: return 因果到状态();
    case L2概念世界关系类别::因果动作引用: return 因果到动作();
    default: return false;
    }
}

// 诊断责任：无适用错误分支；只校验定义目标的强类型值式形状，不读取目标事实。
inline bool L2概念定义目标有效(const L2概念定义目标& 目标) noexcept {
    return std::visit([](const auto& 值) noexcept {
        using 类型 = std::decay_t<decltype(值)>;
        if constexpr (std::is_same_v<类型, L2概念世界关系引用>)
            return L2概念世界关系引用有效(值);
        else if constexpr (std::is_same_v<类型, L2概念特征值引用>)
            return 有效(值.特征实例.值) && 有效(值.值稳定编码);
        else if constexpr (std::is_same_v<类型, L2新特征值引用>)
            return 有效(值.实例.值) && 有效(值.值事实);
        else
            return 有效(值.值);
    }, 目标);
}

// 诊断责任：无适用错误分支；世界事实闭集排除概念身份并复用既有强类型形状。
inline bool L2世界事实引用有效(const L2世界事实引用& 引用) noexcept {
    return std::visit([](const auto& 值) noexcept {
        using 类型 = std::decay_t<decltype(值)>;
        if constexpr (std::is_same_v<类型, L2概念世界关系引用>)
            return L2概念世界关系引用有效(值);
        else if constexpr (std::is_same_v<类型, L2概念特征值引用>)
            return 有效(值.特征实例.值) && 有效(值.值稳定编码);
        else if constexpr (std::is_same_v<类型, L2新特征值引用>)
            return 有效(值.实例.值) && 有效(值.值事实);
        else
            return 有效(值.值);
    }, 引用);
}

namespace L2普通概念数据内部 {

inline bool 发布证据材料组有效(
    const std::vector<不可变材料身份>& 材料组) noexcept {
    if (材料组.empty()) return false;
    for (std::size_t 索引 = 0; 索引 < 材料组.size(); ++索引)
        if (!有效(材料组[索引].值)
            || (索引 != 0 && !(材料组[索引 - 1].值 < 材料组[索引].值)))
            return false;
    return true;
}

inline std::vector<std::uint64_t> 世界事实引用排序键(
    const L2世界事实引用& 引用) {
    return std::visit([](const auto& 值) -> std::vector<std::uint64_t> {
        using 类型 = std::decay_t<decltype(值)>;
        if constexpr (std::is_same_v<类型, L2场景身份>) return {1, 值.值.值};
        else if constexpr (std::is_same_v<类型, L2存在身份>) return {2, 值.值.值};
        else if constexpr (std::is_same_v<类型, L2特征定义身份>)
            return {3, 值.值.值};
        else if constexpr (std::is_same_v<类型, L2特征实例身份>)
            return {4, 值.值.值};
        else if constexpr (std::is_same_v<类型, L2概念特征值引用>)
            return {5, 值.特征实例.值.值, 值.值稳定编码.值};
        else if constexpr (std::is_same_v<类型, L2状态身份>) return {6, 值.值.值};
        else if constexpr (std::is_same_v<类型, L2动态身份>) return {7, 值.值.值};
        else if constexpr (std::is_same_v<类型, L2因果身份>) return {8, 值.值.值};
        else if constexpr (std::is_same_v<类型, L2新存在引用>)
            return {10, 值.值.值};
        else if constexpr (std::is_same_v<类型, L2新特征类型引用>)
            return {11, 值.值.值};
        else if constexpr (std::is_same_v<类型, L2新特征实例引用>)
            return {12, 值.值.值};
        else if constexpr (std::is_same_v<类型, L2新特征值引用>)
            return {13, 值.实例.值.值, 值.值事实.值};
        else {
            const auto 端点键 = [](const L2概念世界关系端点& 端点) {
                return std::visit([](const auto& 项) {
                    using 端点类型 = std::decay_t<decltype(项)>;
                    if constexpr (std::is_same_v<端点类型, L2场景身份>)
                        return std::pair<std::uint64_t, std::uint64_t>{1, 项.值.值};
                    else if constexpr (std::is_same_v<端点类型, L2存在身份>)
                        return std::pair<std::uint64_t, std::uint64_t>{2, 项.值.值};
                    else if constexpr (std::is_same_v<端点类型, L2特征定义身份>)
                        return std::pair<std::uint64_t, std::uint64_t>{3, 项.值.值};
                    else if constexpr (std::is_same_v<端点类型, L2特征实例身份>)
                        return std::pair<std::uint64_t, std::uint64_t>{4, 项.值.值};
                    else if constexpr (std::is_same_v<端点类型, L2状态身份>)
                        return std::pair<std::uint64_t, std::uint64_t>{5, 项.值.值};
                    else if constexpr (std::is_same_v<端点类型, L2动态身份>)
                        return std::pair<std::uint64_t, std::uint64_t>{6, 项.值.值};
                    else if constexpr (std::is_same_v<端点类型, L2因果身份>)
                        return std::pair<std::uint64_t, std::uint64_t>{7, 项.值.值};
                    else return std::pair<std::uint64_t, std::uint64_t>{8, 项.值.值};
                }, 端点);
            };
            const auto 源 = 端点键(值.源);
            const auto 目标 = 端点键(值.目标);
            return {9, static_cast<std::uint64_t>(值.类别),
                值.关系稳定编码.值, 源.first, 源.second,
                目标.first, 目标.second};
        }
    }, 引用);
}

inline bool 读取类别字段有效(L2读取类别 类别,
    std::uint64_t 历史截止, std::uint64_t 期望代次) noexcept {
    if (类别 == L2读取类别::当前) return 历史截止 == 0;
    return 类别 == L2读取类别::历史 && 历史截止 != 0
        && 历史截止 <= 期望代次;
}

} // namespace L2普通概念数据内部

// 诊断责任：无适用错误分支；只接受正式治理生命周期三状态闭集。
inline bool L2概念治理生命周期状态有效(
    L2概念治理生命周期状态 状态) noexcept {
    return 状态 == L2概念治理生命周期状态::活跃
        || 状态 == L2概念治理生命周期状态::冷却
        || 状态 == L2概念治理生命周期状态::退役;
}

// 诊断责任：无适用错误分支；迁移矩阵不包含同状态和两条禁止跃迁。
inline bool L2概念治理生命周期迁移有效(
    L2概念治理生命周期状态 当前状态,
    L2概念治理生命周期状态 目标状态) noexcept {
    return (当前状态 == L2概念治理生命周期状态::活跃
            && 目标状态 == L2概念治理生命周期状态::冷却)
        || (当前状态 == L2概念治理生命周期状态::冷却
            && (目标状态 == L2概念治理生命周期状态::活跃
                || 目标状态 == L2概念治理生命周期状态::退役))
        || (当前状态 == L2概念治理生命周期状态::退役
            && 目标状态 == L2概念治理生命周期状态::活跃);
}

// 诊断责任：无适用错误分支；只校验迁移请求的公开纯值合同。
inline bool L2概念治理生命周期迁移请求有效(
    const L2概念治理生命周期迁移请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && 请求.生命周期合同版本 == L2概念治理生命周期合同版本
        && L2结构幂等身份有效(请求.幂等身份)
        && 有效(请求.概念.值) && 有效(请求.当前生命周期事实稳定编码)
        && L2概念治理生命周期迁移有效(请求.当前状态, 请求.目标状态);
}

// 诊断责任：无适用错误分支；当前与历史生命周期读取参数按类别互斥。
inline bool L2概念治理生命周期读取请求有效(
    const L2概念治理生命周期读取请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && 请求.生命周期合同版本 == L2概念治理生命周期合同版本
        && 有效(请求.概念.值)
        && L2普通概念数据内部::读取类别字段有效(
            请求.读取类别, 请求.历史截止事实代次,
            请求.请求头.期望事实代次);
}

// 诊断责任：无适用错误分支；只判断治理生命周期在具名截止的值式投影。
inline bool L2概念治理生命周期事实截止投影完整(
    const L2概念治理生命周期事实& 事实,
    std::uint64_t 截止) noexcept {
    return 截止 != 0 && 有效(事实.概念.值)
        && L2概念治理生命周期状态有效(事实.状态)
        && 有效(事实.事实稳定编码) && L2生命周期完整(事实.生命周期)
        && 事实.生命周期.创建事实代次 <= 截止
        && (!事实.生命周期.退出事实代次
            || *事实.生命周期.退出事实代次 <= 截止);
}

// 诊断责任：无适用错误分支；只判断生命周期迁移首次或精确重复成功形状。
inline bool L2概念治理生命周期写入结果::成功() const noexcept {
    const bool 成功状态 = 结果头.状态 == L2结构状态::已提交
        || 结果头.状态 == L2结构状态::精确重复;
    if (!成功状态 || 结果头.合同版本 != L2结构合同版本
        || 结果头.事实截止代次 == 0 || !结果头.变更事实代次
        || *结果头.变更事实代次 != 结果头.事实截止代次
        || !当前生命周期 || !已退出生命周期
        || !L2概念治理生命周期事实截止投影完整(
            *当前生命周期, 结果头.事实截止代次)
        || !L2概念治理生命周期事实截止投影完整(
            *已退出生命周期, 结果头.事实截止代次)) return false;
    return 当前生命周期->概念 == 已退出生命周期->概念
        && 当前生命周期->事实稳定编码 != 已退出生命周期->事实稳定编码
        && 当前生命周期->生命周期.创建事实代次
            == 结果头.事实截止代次
        && !当前生命周期->生命周期.退出事实代次
        && 已退出生命周期->生命周期.创建事实代次
            < 结果头.事实截止代次
        && 已退出生命周期->生命周期.退出事实代次
            == 结果头.事实截止代次
        && L2概念治理生命周期迁移有效(
            已退出生命周期->状态, 当前生命周期->状态);
}

// 诊断责任：无适用错误分支；只判断当前或历史生命周期的完整已读取形状。
inline bool L2概念治理生命周期读取结果::成功() const noexcept {
    if (结果头.状态 != L2结构状态::已读取
        || 结果头.合同版本 != L2结构合同版本
        || 结果头.事实截止代次 == 0 || 结果头.变更事实代次
        || !生命周期 || !L2概念治理生命周期事实截止投影完整(
            *生命周期, 结果头.事实截止代次)
        || 生命周期->生命周期.退出事实代次) return false;
    if (读取类别 == L2读取类别::当前)
        return 历史截止事实代次 == 0;
    return 读取类别 == L2读取类别::历史
        && 历史截止事实代次 != 0
        && 结果头.事实截止代次 == 历史截止事实代次;
}

// 诊断责任：无适用错误分支；只校验支持新增请求的值式形状。
inline bool L2概念世界事实支持新增请求有效(
    const L2概念世界事实支持新增请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && 请求.关系合同版本 == L2概念世界事实支持关系合同版本
        && L2结构幂等身份有效(请求.幂等身份)
        && L2世界事实引用有效(请求.世界事实)
        && 有效(请求.概念.值) && 有效(请求.建立依据材料.值)
        && L2普通概念数据内部::发布证据材料组有效(请求.发布证据材料);
}

// 诊断责任：无适用错误分支；只校验支持替换请求的值式形状。
inline bool L2概念世界事实支持替换请求有效(
    const L2概念世界事实支持替换请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && 请求.关系合同版本 == L2概念世界事实支持关系合同版本
        && L2结构幂等身份有效(请求.幂等身份)
        && 有效(请求.旧关系稳定编码)
        && L2世界事实引用有效(请求.旧世界事实)
        && L2世界事实引用有效(请求.新世界事实)
        && 有效(请求.概念.值) && 有效(请求.新建立依据材料.值)
        && L2普通概念数据内部::发布证据材料组有效(
            请求.新发布证据材料);
}

// 诊断责任：无适用错误分支；只校验支持退出请求的值式形状。
inline bool L2概念世界事实支持退出请求有效(
    const L2概念世界事实支持退出请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && 请求.关系合同版本 == L2概念世界事实支持关系合同版本
        && L2结构幂等身份有效(请求.幂等身份)
        && 有效(请求.关系稳定编码)
        && L2世界事实引用有效(请求.世界事实)
        && 有效(请求.概念.值);
}

// 诊断责任：无适用错误分支；按概念组读的当前与历史字段互斥。
inline bool L2按概念世界事实支持组读取请求有效(
    const L2按概念世界事实支持组读取请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && 请求.关系合同版本 == L2概念世界事实支持关系合同版本
        && 有效(请求.概念.值) && 请求.数量预算 != 0
        && L2普通概念数据内部::读取类别字段有效(
            请求.读取类别, 请求.历史截止事实代次,
            请求.请求头.期望事实代次);
}

// 诊断责任：无适用错误分支；按世界事实组读还要求非零扫描预算。
inline bool L2按世界事实支持概念组读取请求有效(
    const L2按世界事实支持概念组读取请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && 请求.关系合同版本 == L2概念世界事实支持关系合同版本
        && L2世界事实引用有效(请求.世界事实)
        && 请求.扫描预算 != 0 && 请求.数量预算 != 0
        && L2普通概念数据内部::读取类别字段有效(
            请求.读取类别, 请求.历史截止事实代次,
            请求.请求头.期望事实代次);
}

// 诊断责任：无适用错误分支；只校验支持事实的值式闭合与截止投影。
inline bool L2概念世界事实支持关系事实截止投影完整(
    const L2概念世界事实支持关系事实& 事实,
    std::uint64_t 截止) noexcept {
    return 截止 != 0 && L2世界事实引用有效(事实.世界事实)
        && 有效(事实.概念.值) && 有效(事实.建立依据材料.值)
        && L2普通概念数据内部::发布证据材料组有效(事实.发布证据材料)
        && 有效(事实.关系稳定编码) && L2生命周期完整(事实.生命周期)
        && 事实.生命周期.创建事实代次 <= 截止
        && (!事实.生命周期.退出事实代次
            || *事实.生命周期.退出事实代次 <= 截止);
}

// 诊断责任：无适用错误分支；只判断支持关系新增、替换或退出的首次结果形状。
inline bool L2概念世界事实支持写入结果::成功() const noexcept {
    const bool 成功状态 = 结果头.状态 == L2结构状态::已提交
        || 结果头.状态 == L2结构状态::精确重复;
    if (!成功状态 || 结果头.合同版本 != L2结构合同版本
        || 结果头.事实截止代次 == 0 || !结果头.变更事实代次
        || *结果头.变更事实代次 != 结果头.事实截止代次
        || (!当前关系 && !已退出关系)) return false;
    if (当前关系
        && (!L2概念世界事实支持关系事实截止投影完整(
                *当前关系, 结果头.事实截止代次)
            || 当前关系->生命周期.退出事实代次)) return false;
    if (已退出关系
        && (!L2概念世界事实支持关系事实截止投影完整(
                *已退出关系, 结果头.事实截止代次)
            || 已退出关系->生命周期.退出事实代次
                != 结果头.事实截止代次)) return false;
    return !当前关系 || !已退出关系
        || 当前关系->关系稳定编码 != 已退出关系->关系稳定编码;
}

// 诊断责任：无适用错误分支；只判断双向当前或历史支持关系组的完整已读取形状。
inline bool L2概念世界事实支持组读取结果::成功() const noexcept {
    if (结果头.状态 != L2结构状态::已读取
        || 结果头.合同版本 != L2结构合同版本
        || 结果头.事实截止代次 == 0 || 结果头.变更事实代次) return false;
    if (读取类别 == L2读取类别::当前) {
        if (历史截止事实代次 != 0) return false;
    } else if (读取类别 == L2读取类别::历史) {
        if (历史截止事实代次 == 0
            || 结果头.事实截止代次 != 历史截止事实代次) return false;
    } else {
        return false;
    }
    for (std::size_t 索引 = 0; 索引 < 关系组.size(); ++索引) {
        const auto& 关系 = 关系组[索引];
        if (!L2概念世界事实支持关系事实截止投影完整(
                关系, 结果头.事实截止代次)
            || 关系.生命周期.退出事实代次) return false;
        if (索引 == 0) continue;
        const auto& 前项 = 关系组[索引 - 1];
        const auto 前世界键 = L2普通概念数据内部::世界事实引用排序键(
            前项.世界事实);
        const auto 当前世界键 = L2普通概念数据内部::世界事实引用排序键(
            关系.世界事实);
        const bool 严格升序 = 前世界键 < 当前世界键
            || (前世界键 == 当前世界键
                && (前项.概念.值 < 关系.概念.值
                    || (前项.概念 == 关系.概念
                        && 前项.关系稳定编码 < 关系.关系稳定编码)));
        if (!严格升序) return false;
    }
    return true;
}

// 诊断责任：无适用错误分支；角色合同必须有限、非零且上下界有序。
inline bool L2概念定义角色合同有效(
    const L2概念定义角色合同& 合同) noexcept {
    return 合同.角色 != 0 && 合同.最大数量 != 0
        && 合同.最小数量 <= 合同.最大数量;
}

// 诊断责任：无适用错误分支；只判断角色合同事实在具名截止的完整投影。
inline bool L2概念定义角色合同事实截止投影完整(
    const L2概念定义角色合同事实& 事实, std::uint64_t 截止) noexcept {
    return 截止 != 0 && 有效(事实.概念.值)
        && L2概念定义角色合同有效(事实.合同)
        && 有效(事实.成员关系稳定编码)
        && 有效(事实.角色值稳定编码)
        && 有效(事实.最小数量值稳定编码)
        && 有效(事实.最大数量值稳定编码)
        && L2生命周期完整(事实.生命周期)
        && 事实.生命周期.创建事实代次 <= 截止
        && (!事实.生命周期.退出事实代次
            || *事实.生命周期.退出事实代次 <= 截止);
}

// 诊断责任：无适用错误分支；只校验普通概念建立请求的值式合同、排序和闭合强类型引用。
inline bool L2普通概念建立请求有效(
    const L2普通概念建立请求& 请求) noexcept {
    if (!L2结构请求头合同有效(请求.请求头)
        || 请求.普通概念合同版本 != L2普通概念结构合同版本
        || 请求.请求头.期望事实代次 == 0
        || !L2结构幂等身份有效(请求.幂等身份)
        || !有效(请求.本体根.值) || !有效(请求.签名规则材料.值)
        || 请求.直接上位.empty() || 请求.来源证据材料.empty()) return false;
    const auto 材料有效 = [](const L2原始值材料& 材料) noexcept {
        return std::visit([](const auto& 值) noexcept {
            using 类型 = std::decay_t<decltype(值)>;
            if constexpr (std::is_same_v<类型, std::int64_t>) return true;
            else if constexpr (std::is_same_v<类型, L2独立材料引用>)
                return 有效(值.编码);
            else return !值.empty();
        }, 材料);
    };
    const auto 键小于 = [](std::uint64_t 左角色, std::uint64_t 左顺序,
        std::uint64_t 右角色, std::uint64_t 右顺序) noexcept {
        return 左角色 < 右角色 || (左角色 == 右角色 && 左顺序 < 右顺序);
    };
    for (std::size_t 索引 = 0; 索引 < 请求.签名值式项.size(); ++索引) {
        const auto& 项 = 请求.签名值式项[索引];
        if (项.角色 == 0 || 项.顺序 == 0 || !材料有效(项.值)) return false;
        if (索引 != 0 && !键小于(请求.签名值式项[索引 - 1].角色,
                请求.签名值式项[索引 - 1].顺序, 项.角色, 项.顺序)) return false;
    }
    for (std::size_t 索引 = 0; 索引 < 请求.定义角色合同.size(); ++索引) {
        const auto& 合同 = 请求.定义角色合同[索引];
        if (!L2概念定义角色合同有效(合同)
            || (索引 != 0
                && 请求.定义角色合同[索引 - 1].角色 >= 合同.角色))
            return false;
    }
    for (std::size_t 索引 = 0; 索引 < 请求.初始定义.size(); ++索引) {
        const auto& 项 = 请求.初始定义[索引];
        if (项.角色 == 0 || 项.顺序 == 0 || !L2概念定义目标有效(项.目标))
            return false;
        if (索引 != 0 && !键小于(请求.初始定义[索引 - 1].角色,
                请求.初始定义[索引 - 1].顺序, 项.角色, 项.顺序)) return false;
    }
    for (const auto& 值式项 : 请求.签名值式项)
        for (const auto& 定义项 : 请求.初始定义)
            if (值式项.角色 == 定义项.角色 && 值式项.顺序 == 定义项.顺序)
                return false;
    for (const auto& 定义项 : 请求.初始定义) {
        const L2概念定义角色合同* 合同 = nullptr;
        for (const auto& 候选 : 请求.定义角色合同)
            if (候选.角色 == 定义项.角色) 合同 = &候选;
        if (!合同 || 定义项.顺序 > 合同->最大数量) return false;
    }
    for (const auto& 合同 : 请求.定义角色合同) {
        std::size_t 数量 = 0;
        std::uint64_t 期望顺序 = 1;
        for (const auto& 定义项 : 请求.初始定义) {
            if (定义项.角色 != 合同.角色) continue;
            if (定义项.顺序 != 期望顺序++) return false;
            ++数量;
        }
        if (数量 < 合同.最小数量 || 数量 > 合同.最大数量) return false;
    }
    for (std::size_t 索引 = 0; 索引 < 请求.直接上位.size(); ++索引) {
        if (!有效(请求.直接上位[索引].值)
            || (索引 != 0
                && !(请求.直接上位[索引 - 1].值 < 请求.直接上位[索引].值)))
            return false;
    }
    for (std::size_t 索引 = 0; 索引 < 请求.来源证据材料.size(); ++索引) {
        if (!有效(请求.来源证据材料[索引].值)
            || (索引 != 0
                && !(请求.来源证据材料[索引 - 1].值
                    < 请求.来源证据材料[索引].值))) return false;
    }
    return true;
}

// 诊断责任：无适用错误分支；正向定义组读要求非零守卫、身份和数量预算。
inline bool L2按概念定义组读取请求有效(
    const L2按概念定义组读取请求& 请求) noexcept {
    if (!L2结构请求头合同有效(请求.请求头)
        || 请求.请求头.期望事实代次 == 0
        || 请求.定义合同版本 != L2概念定义合同版本
        || !有效(请求.概念.值) || 请求.数量预算 == 0) return false;
    return L2普通概念数据内部::读取类别字段有效(
        请求.读取类别, 请求.历史截止事实代次,
        请求.请求头.期望事实代次);
}

// 诊断责任：无适用错误分支；反向定义组读还要求闭合目标和非零扫描预算。
inline bool L2按定义目标概念组读取请求有效(
    const L2按定义目标概念组读取请求& 请求) noexcept {
    if (!L2结构请求头合同有效(请求.请求头)
        || 请求.请求头.期望事实代次 == 0
        || 请求.定义合同版本 != L2概念定义合同版本
        || !L2概念定义目标有效(请求.目标)
        || 请求.扫描预算 == 0 || 请求.数量预算 == 0) return false;
    return L2普通概念数据内部::读取类别字段有效(
        请求.读取类别, 请求.历史截止事实代次,
        请求.请求头.期望事实代次);
}

// 诊断责任：无适用错误分支；退出请求要求精确退役见证和两个有界预算。
inline bool L2普通概念退出请求有效(
    const L2普通概念退出请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && 请求.退出合同版本 == L2普通概念退出合同版本
        && L2结构幂等身份有效(请求.幂等身份)
        && 有效(请求.概念.值)
        && 有效(请求.当前退役生命周期事实稳定编码)
        && 请求.图扫描预算 != 0 && 请求.关系数量预算 != 0;
}

// 诊断责任：无适用错误分支；当前与历史读取参数按类别互斥。
inline bool L2普通概念读取请求有效(
    const L2普通概念读取请求& 请求) noexcept {
    if (!L2结构请求头合同有效(请求.请求头)
        || 请求.普通概念合同版本 != L2普通概念结构合同版本
        || 请求.请求头.期望事实代次 == 0 || !有效(请求.概念.值)) return false;
    if (请求.读取类别 == L2读取类别::当前)
        return 请求.历史截止事实代次 == 0;
    return 请求.读取类别 == L2读取类别::历史
        && 请求.历史截止事实代次 != 0
        && 请求.历史截止事实代次 <= 请求.请求头.期望事实代次;
}

// 诊断责任：无适用错误分支；只校验直接上位新增请求的值式形状。
inline bool L2概念直接上位新增请求有效(
    const L2概念直接上位新增请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && 请求.关系合同版本 == L2概念直接上下位关系合同版本
        && L2结构幂等身份有效(请求.幂等身份)
        && 有效(请求.上位概念.值) && 有效(请求.下位概念.值)
        && 请求.上位概念 != 请求.下位概念;
}

// 诊断责任：无适用错误分支；只校验直接上位替换请求的值式形状。
inline bool L2概念直接上位替换请求有效(
    const L2概念直接上位替换请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && 请求.关系合同版本 == L2概念直接上下位关系合同版本
        && L2结构幂等身份有效(请求.幂等身份)
        && 有效(请求.旧关系稳定编码) && 有效(请求.旧上位概念.值)
        && 有效(请求.下位概念.值) && 有效(请求.新上位概念.值)
        && 请求.旧上位概念 != 请求.下位概念
        && 请求.新上位概念 != 请求.下位概念
        && 请求.旧上位概念 != 请求.新上位概念;
}

// 诊断责任：无适用错误分支；只校验直接上位退出请求的值式形状。
inline bool L2概念直接上位退出请求有效(
    const L2概念直接上位退出请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && 请求.关系合同版本 == L2概念直接上下位关系合同版本
        && L2结构幂等身份有效(请求.幂等身份)
        && 有效(请求.关系稳定编码) && 有效(请求.上位概念.值)
        && 有效(请求.下位概念.值)
        && 请求.上位概念 != 请求.下位概念;
}

// 诊断责任：无适用错误分支；当前与历史直接上位组读参数按类别互斥。
inline bool L2概念直接上位组读取请求有效(
    const L2概念直接上位组读取请求& 请求) noexcept {
    if (!L2结构请求头合同有效(请求.请求头)
        || 请求.请求头.期望事实代次 == 0
        || 请求.关系合同版本 != L2概念直接上下位关系合同版本
        || !有效(请求.下位概念.值) || 请求.数量预算 == 0) return false;
    if (请求.读取类别 == L2读取类别::当前)
        return 请求.历史截止事实代次 == 0;
    return 请求.读取类别 == L2读取类别::历史
        && 请求.历史截止事实代次 != 0
        && 请求.历史截止事实代次 <= 请求.请求头.期望事实代次;
}

// 诊断责任：无适用错误分支；当前与历史直接下位组读参数按类别互斥。
inline bool L2概念直接下位组读取请求有效(
    const L2概念直接下位组读取请求& 请求) noexcept {
    if (!L2结构请求头合同有效(请求.请求头)
        || 请求.请求头.期望事实代次 == 0
        || 请求.关系合同版本 != L2概念直接上下位关系合同版本
        || !有效(请求.上位概念.值) || 请求.数量预算 == 0) return false;
    if (请求.读取类别 == L2读取类别::当前)
        return 请求.历史截止事实代次 == 0;
    return 请求.读取类别 == L2读取类别::历史
        && 请求.历史截止事实代次 != 0
        && 请求.历史截止事实代次 <= 请求.请求头.期望事实代次;
}

// 诊断责任：无适用错误分支；只判断直接上位关系在具名截止的值式投影。
inline bool L2概念直接上位事实截止投影完整(
    const L2概念直接上位事实& 关系, std::uint64_t 截止) noexcept {
    return 截止 != 0 && 有效(关系.上位概念.值) && 有效(关系.下位概念.值)
        && 关系.上位概念 != 关系.下位概念 && 有效(关系.关系稳定编码)
        && L2生命周期完整(关系.生命周期)
        && 关系.生命周期.创建事实代次 <= 截止
        && (!关系.生命周期.退出事实代次
            || *关系.生命周期.退出事实代次 <= 截止);
}

// 诊断责任：无适用错误分支；只判断普通概念负载在具名截止的值式完整性。
inline bool L2普通概念事实完整(
    const L2普通概念事实& 事实, std::uint64_t 截止) noexcept {
    const auto 生命周期完整 = [截止](const L2生命周期& 生命周期) noexcept {
        return L2生命周期完整(生命周期)
            && 生命周期.创建事实代次 <= 截止
            && (!生命周期.退出事实代次
                || *生命周期.退出事实代次 <= 截止);
    };
    const auto 材料有效 = [](const L2原始值材料& 材料) noexcept {
        return std::visit([](const auto& 值) noexcept {
            using 类型 = std::decay_t<decltype(值)>;
            if constexpr (std::is_same_v<类型, std::int64_t>) return true;
            else if constexpr (std::is_same_v<类型, L2独立材料引用>)
                return 有效(值.编码);
            else return !值.empty();
        }, 材料);
    };
    const auto 键小于 = [](std::uint64_t 左角色, std::uint64_t 左顺序,
        std::uint64_t 右角色, std::uint64_t 右顺序) noexcept {
        return 左角色 < 右角色 || (左角色 == 右角色 && 左顺序 < 右顺序);
    };
    if (截止 == 0 || !有效(事实.概念.值)
        || !L2节点族身份来源事实完整(事实.身份来源, 截止)
        || 事实.身份来源.节点族 != L2节点族::概念
        || 事实.身份来源.节点稳定编码 != 事实.概念.值
        || !有效(事实.本体根归属.本体根.值)
        || 事实.本体根归属.本体根 == 事实.概念
        || 事实.本体根归属.概念 != 事实.概念
        || !有效(事实.本体根归属.关系稳定编码)
        || !生命周期完整(事实.本体根归属.生命周期)
        || 事实.签名规则.概念 != 事实.概念
        || !有效(事实.签名规则.规则材料.值)
        || !有效(事实.签名规则.关系稳定编码)
        || !生命周期完整(事实.签名规则.生命周期)
        || 事实.直接上位.empty() || 事实.来源证据.empty()
        || 事实.治理生命周期.概念 != 事实.概念
        || !L2概念治理生命周期事实截止投影完整(
            事实.治理生命周期, 截止)) return false;
    for (std::size_t 索引 = 0; 索引 < 事实.签名值式项.size(); ++索引) {
        const auto& 项 = 事实.签名值式项[索引];
        if (项.概念 != 事实.概念 || 项.项.角色 == 0 || 项.项.顺序 == 0
            || !材料有效(项.项.值) || !有效(项.值稳定编码)
            || !生命周期完整(项.生命周期)) return false;
        if (索引 != 0 && !键小于(事实.签名值式项[索引 - 1].项.角色,
                事实.签名值式项[索引 - 1].项.顺序,
                项.项.角色, 项.项.顺序)) return false;
    }
    for (std::size_t 索引 = 0; 索引 < 事实.定义角色合同.size(); ++索引) {
        const auto& 合同 = 事实.定义角色合同[索引];
        if (合同.概念 != 事实.概念
            || !L2概念定义角色合同事实截止投影完整(合同, 截止)
            || (索引 != 0
                && 事实.定义角色合同[索引 - 1].合同.角色
                    >= 合同.合同.角色))
            return false;
    }
    for (std::size_t 索引 = 0; 索引 < 事实.定义关系.size(); ++索引) {
        const auto& 项 = 事实.定义关系[索引];
        if (项.概念 != 事实.概念 || 项.项.角色 == 0 || 项.项.顺序 == 0
            || !L2概念定义目标有效(项.项.目标)
            || !有效(项.关系稳定编码) || !生命周期完整(项.生命周期))
            return false;
        if (索引 != 0 && !键小于(事实.定义关系[索引 - 1].项.角色,
                事实.定义关系[索引 - 1].项.顺序,
                项.项.角色, 项.项.顺序)) return false;
    }
    for (const auto& 值式项 : 事实.签名值式项)
        for (const auto& 定义项 : 事实.定义关系)
            if (值式项.项.角色 == 定义项.项.角色
                && 值式项.项.顺序 == 定义项.项.顺序) return false;
    for (const auto& 定义项 : 事实.定义关系) {
        const L2概念定义角色合同事实* 合同 = nullptr;
        for (const auto& 候选 : 事实.定义角色合同)
            if (候选.合同.角色 == 定义项.项.角色) 合同 = &候选;
        if (!合同 || 定义项.项.顺序 > 合同->合同.最大数量) return false;
    }
    for (const auto& 合同 : 事实.定义角色合同) {
        std::size_t 数量 = 0;
        std::uint64_t 期望顺序 = 1;
        for (const auto& 定义项 : 事实.定义关系) {
            if (定义项.项.角色 != 合同.合同.角色) continue;
            if (定义项.项.顺序 != 期望顺序++) return false;
            ++数量;
        }
        if (数量 < 合同.合同.最小数量
            || 数量 > 合同.合同.最大数量) return false;
    }
    for (std::size_t 索引 = 0; 索引 < 事实.直接上位.size(); ++索引) {
        const auto& 上位 = 事实.直接上位[索引];
        if (!有效(上位.上位概念.值) || 上位.上位概念 == 事实.概念
            || 上位.下位概念 != 事实.概念 || !有效(上位.关系稳定编码)
            || !生命周期完整(上位.生命周期)
            || (索引 != 0
                && !(事实.直接上位[索引 - 1].上位概念.值 < 上位.上位概念.值)))
            return false;
    }
    for (std::size_t 索引 = 0; 索引 < 事实.来源证据.size(); ++索引) {
        const auto& 证据 = 事实.来源证据[索引];
        if (证据.概念 != 事实.概念 || !有效(证据.证据材料.值)
            || !有效(证据.关系稳定编码) || !生命周期完整(证据.生命周期)
            || (索引 != 0
                && !(事实.来源证据[索引 - 1].证据材料.值 < 证据.证据材料.值)))
            return false;
    }
    return true;
}

// 诊断责任：无适用错误分支；只判断关系新增、替换或退出的首次结果形状。
inline bool L2概念直接上位写入结果::成功() const noexcept {
    const bool 成功状态 = 结果头.状态 == L2结构状态::已提交
        || 结果头.状态 == L2结构状态::精确重复;
    if (!成功状态 || 结果头.合同版本 != L2结构合同版本
        || 结果头.事实截止代次 == 0 || !结果头.变更事实代次
        || *结果头.变更事实代次 != 结果头.事实截止代次
        || (!当前关系 && !已退出关系)) return false;
    if (当前关系
        && (!L2概念直接上位事实截止投影完整(
                *当前关系, 结果头.事实截止代次)
            || 当前关系->生命周期.退出事实代次)) return false;
    if (已退出关系
        && (!L2概念直接上位事实截止投影完整(
                *已退出关系, 结果头.事实截止代次)
            || 已退出关系->生命周期.退出事实代次
                != 结果头.事实截止代次)) return false;
    return !当前关系 || !已退出关系
        || 当前关系->关系稳定编码 != 已退出关系->关系稳定编码;
}

// 诊断责任：无适用错误分支；只判断双向当前或历史关系组的完整已读取形状。
inline bool L2概念直接上位组读取结果::成功() const noexcept {
    if (结果头.状态 != L2结构状态::已读取
        || 结果头.合同版本 != L2结构合同版本
        || 结果头.事实截止代次 == 0 || 结果头.变更事实代次) return false;
    if (读取类别 == L2读取类别::当前) {
        if (历史截止事实代次 != 0) return false;
    } else if (读取类别 == L2读取类别::历史) {
        if (历史截止事实代次 == 0
            || 结果头.事实截止代次 != 历史截止事实代次) return false;
    } else {
        return false;
    }
    for (std::size_t 索引 = 0; 索引 < 关系组.size(); ++索引) {
        const auto& 关系 = 关系组[索引];
        if (!L2概念直接上位事实截止投影完整(
                关系, 结果头.事实截止代次)
            || 关系.生命周期.退出事实代次) return false;
        if (索引 == 0) continue;
        const auto& 前项 = 关系组[索引 - 1];
        const bool 严格升序 = 前项.上位概念.值 < 关系.上位概念.值
            || (前项.上位概念 == 关系.上位概念
                && (前项.下位概念.值 < 关系.下位概念.值
                    || (前项.下位概念 == 关系.下位概念
                        && 前项.关系稳定编码 < 关系.关系稳定编码)));
        if (!严格升序) return false;
    }
    return true;
}

// 诊断责任：无适用错误分支；只判断写结果是否为完整的首次提交或原结果精确重复形状。
inline bool L2普通概念写入结果::成功() const noexcept {
    const bool 成功状态 = 结果头.状态 == L2结构状态::已提交
        || 结果头.状态 == L2结构状态::精确重复;
    if (!成功状态 || 结果头.合同版本 != L2结构合同版本
        || 结果头.事实截止代次 == 0 || !结果头.变更事实代次
        || *结果头.变更事实代次 != 结果头.事实截止代次 || !概念
        || !L2普通概念事实完整(*概念, 结果头.事实截止代次)
        || 概念->治理生命周期.状态 != L2概念治理生命周期状态::活跃)
        return false;
    const auto 身份来源当前 = [](const L2节点族身份来源事实& 来源) noexcept {
        return !来源.节点生命周期.退出事实代次
            && !来源.族锚点生命周期.退出事实代次
            && !来源.关系类型生命周期.退出事实代次
            && !来源.归属关系生命周期.退出事实代次;
    };
    if (!身份来源当前(概念->身份来源)
        || 概念->本体根归属.生命周期.退出事实代次
        || 概念->签名规则.生命周期.退出事实代次
        || 概念->治理生命周期.生命周期.退出事实代次) return false;
    for (const auto& 项 : 概念->签名值式项)
        if (项.生命周期.退出事实代次) return false;
    for (const auto& 项 : 概念->定义角色合同)
        if (项.生命周期.退出事实代次) return false;
    for (const auto& 项 : 概念->定义关系)
        if (项.生命周期.退出事实代次) return false;
    for (const auto& 项 : 概念->直接上位)
        if (项.生命周期.退出事实代次) return false;
    for (const auto& 项 : 概念->来源证据)
        if (项.生命周期.退出事实代次) return false;
    return true;
}

// 诊断责任：无适用错误分支；只判断当前或历史读取结果的完整已读取形状。
inline bool L2普通概念读取结果::成功() const noexcept {
    const std::uint64_t 投影截止 = 读取类别 == L2读取类别::历史
        ? 历史截止事实代次 : 结果头.事实截止代次;
    if (结果头.状态 != L2结构状态::已读取
        || 结果头.合同版本 != L2结构合同版本
        || 结果头.事实截止代次 == 0 || 结果头.变更事实代次 || !概念
        || !L2普通概念事实完整(*概念, 投影截止)) return false;
    if (读取类别 == L2读取类别::历史)
        return 历史截止事实代次 != 0
            && 结果头.事实截止代次 == 历史截止事实代次;
    if (读取类别 != L2读取类别::当前 || 历史截止事实代次 != 0)
        return false;
    const auto 身份来源当前 = [](const L2节点族身份来源事实& 来源) noexcept {
        return !来源.节点生命周期.退出事实代次
            && !来源.族锚点生命周期.退出事实代次
            && !来源.关系类型生命周期.退出事实代次
            && !来源.归属关系生命周期.退出事实代次;
    };
    if (!身份来源当前(概念->身份来源)
        || 概念->本体根归属.生命周期.退出事实代次
        || 概念->签名规则.生命周期.退出事实代次
        || 概念->治理生命周期.生命周期.退出事实代次) return false;
    for (const auto& 项 : 概念->签名值式项)
        if (项.生命周期.退出事实代次) return false;
    for (const auto& 项 : 概念->定义角色合同)
        if (项.生命周期.退出事实代次) return false;
    for (const auto& 项 : 概念->定义关系)
        if (项.生命周期.退出事实代次) return false;
    for (const auto& 项 : 概念->直接上位)
        if (项.生命周期.退出事实代次) return false;
    for (const auto& 项 : 概念->来源证据)
        if (项.生命周期.退出事实代次) return false;
    return true;
}

// 诊断责任：无适用错误分支；定义组读必须同截止、稳定排序且全量值式完整。
inline bool L2概念定义组读取结果::成功() const noexcept {
    if (结果头.状态 != L2结构状态::已读取
        || 结果头.合同版本 != L2结构合同版本
        || 结果头.事实截止代次 == 0 || 结果头.变更事实代次) return false;
    if (读取类别 == L2读取类别::当前) {
        if (历史截止事实代次 != 0) return false;
    } else if (读取类别 == L2读取类别::历史) {
        if (历史截止事实代次 == 0
            || 结果头.事实截止代次 != 历史截止事实代次) return false;
    } else return false;
    const auto 生命周期适合类别 = [&](const L2生命周期& 生命周期) noexcept {
        if (读取类别 == L2读取类别::当前)
            return !生命周期.退出事实代次;
        return !生命周期.退出事实代次
            || *生命周期.退出事实代次 <= 结果头.事实截止代次;
    };
    for (std::size_t 索引 = 0; 索引 < 角色合同组.size(); ++索引) {
        const auto& 当前 = 角色合同组[索引];
        if (!L2概念定义角色合同事实截止投影完整(
                当前, 结果头.事实截止代次)
            || !生命周期适合类别(当前.生命周期)) return false;
        if (索引 == 0) continue;
        const auto& 前 = 角色合同组[索引 - 1];
        if (!(前.概念.值 < 当前.概念.值
                || (前.概念 == 当前.概念
                    && 前.合同.角色 < 当前.合同.角色))) return false;
    }
    for (std::size_t 索引 = 0; 索引 < 定义关系组.size(); ++索引) {
        const auto& 当前 = 定义关系组[索引];
        if (!有效(当前.概念.值) || 当前.项.角色 == 0
            || 当前.项.顺序 == 0 || !L2概念定义目标有效(当前.项.目标)
            || !有效(当前.关系稳定编码)
            || !L2生命周期完整(当前.生命周期)
            || 当前.生命周期.创建事实代次 > 结果头.事实截止代次
            || !生命周期适合类别(当前.生命周期)) return false;
        if (索引 == 0) continue;
        const auto& 前 = 定义关系组[索引 - 1];
        if (!(前.概念.值 < 当前.概念.值
                || (前.概念 == 当前.概念
                    && (前.项.角色 < 当前.项.角色
                        || (前.项.角色 == 当前.项.角色
                            && (前.项.顺序 < 当前.项.顺序
                                || (前.项.顺序 == 当前.项.顺序
                                    && 前.关系稳定编码
                                        < 当前.关系稳定编码))))))) return false;
    }
    return true;
}

// 诊断责任：无适用错误分支；退出成功必须同代关闭概念和旧边并值式返回替代边。
inline bool L2普通概念退出结果::成功() const noexcept {
    const bool 成功状态 = 结果头.状态 == L2结构状态::已提交
        || 结果头.状态 == L2结构状态::精确重复;
    if (!成功状态 || 结果头.合同版本 != L2结构合同版本
        || 结果头.事实截止代次 == 0 || !结果头.变更事实代次
        || *结果头.变更事实代次 != 结果头.事实截止代次
        || !已退出概念
        || !L2普通概念事实完整(*已退出概念, 结果头.事实截止代次))
        return false;
    const auto 截止 = 结果头.事实截止代次;
    const auto 同截止退出 = [截止](const L2生命周期& 生命周期) noexcept {
        return 生命周期.退出事实代次 == 截止;
    };
    if (!同截止退出(已退出概念->身份来源.节点生命周期)
        || !同截止退出(已退出概念->身份来源.归属关系生命周期)
        || !同截止退出(已退出概念->本体根归属.生命周期)
        || !同截止退出(已退出概念->签名规则.生命周期)
        || !同截止退出(已退出概念->治理生命周期.生命周期)) return false;
    for (const auto& 项 : 已退出概念->签名值式项)
        if (!同截止退出(项.生命周期)) return false;
    for (const auto& 项 : 已退出概念->定义角色合同)
        if (!同截止退出(项.生命周期)) return false;
    for (const auto& 项 : 已退出概念->定义关系)
        if (!同截止退出(项.生命周期)) return false;
    for (const auto& 项 : 已退出概念->来源证据)
        if (!同截止退出(项.生命周期)) return false;
    for (const auto& 关系 : 已退出直接关系)
        if (!L2概念直接上位事实截止投影完整(关系, 截止)
            || 关系.生命周期.退出事实代次 != 截止) return false;
    for (const auto& 关系 : 新增替代关系)
        if (!L2概念直接上位事实截止投影完整(关系, 截止)
            || 关系.生命周期.创建事实代次 != 截止
            || 关系.生命周期.退出事实代次) return false;
    return true;
}

} // namespace 海中鱼巣

namespace 海中鱼巣 {

// FEATURE-CONCEPT-APPLICATION：具名特征概念结构合同；业务算法由应用执行。
inline constexpr std::uint32_t L2特征概念合同版本 = 1;
enum class L2特征概念状态 : std::uint8_t {
    已创建 = 1,
    精确重复 = 2,
    已读取 = 3,
    已绑定 = 4,
    已记录 = 5,
    已成熟 = 6,
    已迁移 = 7,
    入口拒绝 = 8,
    未找到 = 9,
    目标已退出 = 10,
    结构未配置 = 11,
    事实代次漂移 = 12,
    幂等冲突 = 13,
    引用冲突 = 14,
    匹配歧义 = 15,
    证据不足 = 16,
    数量预算不足 = 17,
    资源失败 = 18,
    内部不一致 = 19,
    已可能发布 = 20,
    规则不支持 = 21,
    历史材料不可用 = 22,
    已退出 = 23,
    材料不足 = 24,
    无须释放 = 25,
    已释放 = 26
};
using 特征概念材料 = std::variant<std::int64_t, std::vector<std::int64_t>, std::vector<std::uint64_t>>;
struct 特征概念I64区间 final {
    std::int64_t 下界 = 0;
    std::int64_t 上界 = 0;
    friend bool operator==(const 特征概念I64区间&, const 特征概念I64区间&) = default;
};
enum class 特征概念值域类别 : std::uint8_t { 精确值 = 1, I64闭区间 = 2 };
struct 特征概念值域 final {
    特征概念值域类别 类别 = 特征概念值域类别::精确值;
    std::optional<特征概念材料> 精确值;
    std::optional<特征概念I64区间> 区间;
    friend bool operator==(const 特征概念值域&, const 特征概念值域&) = default;
};
struct 特征概念形成规则 final {
    稳定编码 规则身份{};
    std::uint64_t 规则版本 = 0;
    不可变材料身份 规则材料;
    std::vector<特征概念I64区间> 分区;
    friend bool operator==(const 特征概念形成规则&, const 特征概念形成规则&) = default;
};
struct 特征概念成熟规则 final {
    稳定编码 规则身份{};
    std::uint64_t 规则版本 = 0;
    不可变材料身份 规则材料;
    std::uint64_t 最少独立观测数 = 0;
    L2中性时间_v1 最短观察跨度{}, 最小观测间隔{};
    friend bool operator==(const 特征概念成熟规则&, const 特征概念成熟规则&) = default;
};
inline bool 特征概念值域有效(const 特征概念值域& d) noexcept {
    if (d.类别 == 特征概念值域类别::I64闭区间)
        return !d.精确值 && d.区间 && d.区间->下界 < d.区间->上界;
    if (d.类别 != 特征概念值域类别::精确值 || !d.精确值 || d.区间)
        return false;
    return std::visit(
        [](const auto& v) {
            if constexpr (std::is_same_v<std::decay_t<decltype(v)>, std::int64_t>)
                return true;
            else
                return !v.empty();
        },
        *d.精确值);
}
inline bool 特征概念形成规则有效(const 特征概念形成规则& r) noexcept {
    if (!有效(r.规则身份) || !r.规则版本 || !有效(r.规则材料.值) || r.规则身份 != r.规则材料.值)
        return false;
    for (std::size_t i = 0; i < r.分区.size(); ++i) {
        auto v = r.分区[i];
        if (v.下界 > v.上界)
            return false;
        if (i
            && (r.分区[i - 1].下界 > v.下界
                || (r.分区[i - 1].下界 == v.下界 && r.分区[i - 1].上界 >= v.上界)))
            return false;
    }
    return true;
}
inline bool 特征概念成熟规则有效(const 特征概念成熟规则& r) noexcept {
    return 有效(r.规则身份) && r.规则版本 && 有效(r.规则材料.值) && r.规则身份 == r.规则材料.值
           && r.最少独立观测数 >= 2 && r.最短观察跨度.语义 == L2中性时间语义_v1::非负相对纳秒
           && r.最短观察跨度.纳秒 > 0 && r.最小观测间隔.语义 == L2中性时间语义_v1::非负相对纳秒
           && r.最小观测间隔.纳秒 > 0;
}
struct L2特征概念建立请求 final {
    std::uint32_t 合同版本 = L2特征概念合同版本;
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2概念身份 本体根;
    L2新存在引用 宿主;
    L2新特征类型引用 特征类型;
    特征概念形成规则 规则;
    特征概念值域 值域;
    std::vector<不可变材料身份> 来源材料组;
    std::vector<L2概念身份> 直接上位组;
    friend bool operator==(const L2特征概念建立请求&, const L2特征概念建立请求&) = default;
};
struct L2特征概念读取请求 final {
    std::uint32_t 合同版本 = L2特征概念合同版本;
    L2结构请求头 请求头;
    L2概念身份 概念;
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止 = 0;
    friend bool operator==(const L2特征概念读取请求&, const L2特征概念读取请求&) = default;
};
struct L2特征概念组请求 final {
    std::uint32_t 合同版本 = L2特征概念合同版本;
    L2结构请求头 请求头;
    L2新存在引用 宿主;
    L2新特征类型引用 特征类型;
    稳定编码 规则身份{};
    std::uint64_t 规则版本 = 0;
    std::uint64_t 最大候选数 = 0;
    friend bool operator==(const L2特征概念组请求&, const L2特征概念组请求&) = default;
};
struct L2特征名称字段绑定请求 final {
    std::uint32_t 合同版本 = L2特征概念合同版本;
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2新特征实例引用 实例;
    L2新存在引用 宿主;
    L2新特征类型引用 特征类型;
    L2概念身份 概念;
    std::uint64_t Gbind = 0;
    稳定编码 宿主成员关系{};

    friend bool operator==(const L2特征名称字段绑定请求&, const L2特征名称字段绑定请求&) = default;
};
struct L2特征名称字段替换请求 final {
    std::uint32_t 合同版本 = L2特征概念合同版本;
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2新特征实例引用 实例;
    L2新存在引用 宿主;
    L2新特征类型引用 特征类型;
    L2概念身份 概念;
    std::uint64_t Gbind = 0;
    稳定编码 宿主成员关系{};
    稳定编码 原关系稳定编码{};
    friend bool operator==(const L2特征名称字段替换请求&, const L2特征名称字段替换请求&) = default;
};
struct L2特征名称字段读取请求 final {
    std::uint32_t 合同版本 = L2特征概念合同版本;
    L2结构请求头 请求头;
    L2新特征实例引用 实例;
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止 = 0;
    friend bool operator==(const L2特征名称字段读取请求&, const L2特征名称字段读取请求&) = default;
};
struct L2特征名称字段退出请求 final {
    std::uint32_t 合同版本 = L2特征概念合同版本;
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2新特征实例引用 实例;
    稳定编码 原关系稳定编码{};
    friend bool operator==(const L2特征名称字段退出请求&, const L2特征名称字段退出请求&) = default;
};
struct L2特征观测登记请求 final {
    std::uint32_t 合同版本 = L2特征概念合同版本;
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    稳定编码 观测稳定编码{};
    L2新存在引用 宿主;
    L2新特征类型引用 特征类型;
    L2新特征值引用 精确值;
    L2概念身份 概念;
    L2中性时间_v1 观测时间{};
    不可变材料身份 来源材料;
    std::uint64_t Gobs = 0;
    稳定编码 宿主成员关系{};

    friend bool operator==(const L2特征观测登记请求&, const L2特征观测登记请求&) = default;
};
struct L2特征观测组请求 final {
    std::uint32_t 合同版本 = L2特征概念合同版本;
    L2结构请求头 请求头;
    L2概念身份 概念;
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止 = 0;
    std::uint64_t 最大观测数 = 0;
    friend bool operator==(const L2特征观测组请求&, const L2特征观测组请求&) = default;
};
struct L2特征概念成熟请求 final {
    std::uint32_t 合同版本 = L2特征概念合同版本;
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2概念身份 概念;
    稳定编码 原程度值事实{};
    特征概念成熟规则 规则;
    std::uint64_t 观测证据截止 = 0;
    std::vector<稳定编码> 观测记录身份组;
    friend bool operator==(const L2特征概念成熟请求&, const L2特征概念成熟请求&) = default;
};
struct L2特征概念事实 final {
    L2普通概念事实 普通概念;
    L2新存在引用 宿主;
    L2新特征类型引用 特征类型;
    特征概念形成规则 规则;
    特征概念值域 值域;
    bool 已成熟 = false;
    稳定编码 程度值事实{};
    std::optional<特征概念成熟规则> 成熟规则;
    std::vector<稳定编码> 成熟证据身份组;
    std::uint64_t 评估截止 = 0;
    friend bool operator==(const L2特征概念事实&, const L2特征概念事实&) = default;
};
struct L2特征名称字段事实 final {
    稳定编码 关系身份{};
    L2新特征实例引用 实例;
    L2新存在引用 宿主;
    L2新特征类型引用 特征类型;
    L2概念身份 概念;
    std::uint64_t Gbind = 0;
    稳定编码 宿主成员关系{};
    std::uint64_t 创建代次 = 0;
    std::optional<std::uint64_t> 退出代次;
    friend bool operator==(const L2特征名称字段事实&, const L2特征名称字段事实&) = default;
};
struct L2特征观测事实 final {
    稳定编码 记录节点{};
    稳定编码 观测稳定编码{};
    L2新存在引用 宿主;
    L2新特征类型引用 特征类型;
    L2新特征值引用 精确值;
    L2概念身份 概念;
    L2中性时间_v1 观测时间{};
    不可变材料身份 来源材料;
    std::uint64_t Gobs = 0;
    稳定编码 宿主成员关系{};
    std::uint64_t 创建代次 = 0;
    friend bool operator==(const L2特征观测事实&, const L2特征观测事实&) = default;
};

inline bool 特征概念载荷完整(const L2特征概念事实& f) noexcept {
    for (std::size_t i = 0; i < f.成熟证据身份组.size(); ++i) {
        if (!有效(f.成熟证据身份组[i]))
            return false;
        for (std::size_t j = 0; j < i; ++j)
            if (f.成熟证据身份组[j] == f.成熟证据身份组[i])
                return false;
    }
    return 有效(f.普通概念.概念.值) && 有效(f.宿主.值) && 有效(f.特征类型.值) && 特征概念形成规则有效(f.规则)
           && 特征概念值域有效(f.值域) && 有效(f.程度值事实)
           && (f.已成熟 ? (f.成熟规则 && 特征概念成熟规则有效(*f.成熟规则) && !f.成熟证据身份组.empty()
                           && f.评估截止)
                        : (!f.成熟规则 && f.成熟证据身份组.empty() && !f.评估截止));
}
inline bool 特征概念载荷完整(const L2特征名称字段事实& f) noexcept {
    return 有效(f.关系身份) && 有效(f.实例.值) && 有效(f.宿主.值) && 有效(f.特征类型.值) && 有效(f.概念.值)
           && f.Gbind && 有效(f.宿主成员关系) && f.创建代次 && f.Gbind < f.创建代次
           && (!f.退出代次 || *f.退出代次 > f.创建代次);
}
inline bool 特征概念载荷完整(const L2特征观测事实& f) noexcept {
    return 有效(f.记录节点) && 有效(f.观测稳定编码) && 有效(f.宿主.值) && 有效(f.特征类型.值)
           && 有效(f.精确值.实例.值) && 有效(f.精确值.值事实) && 有效(f.概念.值)
           && f.观测时间.语义 == L2中性时间语义_v1::绝对UTC纳秒 && f.观测时间.纳秒 >= 0 && 有效(f.来源材料.值)
           && f.Gobs && 有效(f.宿主成员关系) && f.创建代次 > f.Gobs;
}
struct L2特征概念结果 final {
    L2特征概念状态 状态 = L2特征概念状态::入口拒绝;
    std::uint32_t 合同版本 = 1;
    std::uint64_t 事实截止 = 0;
    std::optional<std::uint64_t> 发布截止;
    std::optional<L2特征概念事实> 概念;
    bool 成功() const noexcept {
        return (状态 == L2特征概念状态::已创建 || 状态 == L2特征概念状态::精确重复
                || 状态 == L2特征概念状态::已读取 || 状态 == L2特征概念状态::已成熟)
               && 合同版本 == 1 && 事实截止 && 概念 && 特征概念载荷完整(*概念)
               && L2普通概念事实完整(概念->普通概念, 事实截止)
               && (!概念->已成熟 || 概念->评估截止 <= 事实截止);
    }
};
struct L2特征名称字段结果 final {
    L2特征概念状态 状态 = L2特征概念状态::入口拒绝;
    std::uint32_t 合同版本 = 1;
    std::uint64_t 事实截止 = 0;
    std::optional<std::uint64_t> 发布截止;
    std::optional<L2特征名称字段事实> 名称字段;
    bool 成功() const noexcept {
        return (状态 == L2特征概念状态::已绑定 || 状态 == L2特征概念状态::精确重复
                || 状态 == L2特征概念状态::已读取 || 状态 == L2特征概念状态::已退出)
               && 合同版本 == 1 && 事实截止 && 名称字段 && 特征概念载荷完整(*名称字段)
               && 名称字段->创建代次 <= 事实截止
               && (状态 == L2特征概念状态::已退出 ? 名称字段->退出代次 == 事实截止 : !名称字段->退出代次);
    }
};
struct L2特征观测结果 final {
    L2特征概念状态 状态 = L2特征概念状态::入口拒绝;
    std::uint32_t 合同版本 = 1;
    std::uint64_t 事实截止 = 0;
    std::optional<std::uint64_t> 发布截止;
    std::optional<L2特征观测事实> 观测;
    bool 成功() const noexcept {
        return (状态 == L2特征概念状态::已记录 || 状态 == L2特征概念状态::精确重复
                || 状态 == L2特征概念状态::已读取)
               && 合同版本 == 1 && 事实截止 && 观测 && 特征概念载荷完整(*观测) && 观测->创建代次 <= 事实截止;
    }
};
struct L2特征概念组结果 final {
    L2特征概念状态 状态 = L2特征概念状态::入口拒绝;
    std::uint32_t 合同版本 = 1;
    std::uint64_t 事实截止 = 0;
    L2新存在引用 宿主;
    L2新特征类型引用 特征类型;
    稳定编码 规则身份{};
    std::uint64_t 规则版本 = 0;
    std::vector<L2特征概念事实> 概念组;
    bool 成功() const noexcept {
        if (状态 != L2特征概念状态::已读取 || 合同版本 != 1 || !事实截止)
            return false;
        if (!有效(宿主.值) || !有效(特征类型.值) || !有效(规则身份) || !规则版本)
            return false;
        稳定编码 previous{};
        for (const auto& x : 概念组) {
            if (!特征概念载荷完整(x) || !L2普通概念事实完整(x.普通概念, 事实截止) || x.宿主 != 宿主
                || x.特征类型 != 特征类型 || x.规则.规则身份 != 规则身份 || x.规则.规则版本 != 规则版本
                || (有效(previous) && !(previous < x.普通概念.概念.值)))
                return false;
            previous = x.普通概念.概念.值;
        }
        return true;
    }
};
struct L2特征观测组结果 final {
    L2特征概念状态 状态 = L2特征概念状态::入口拒绝;
    std::uint32_t 合同版本 = 1;
    std::uint64_t 事实截止 = 0;
    L2概念身份 概念;
    std::vector<L2特征观测事实> 观测组;
    bool 成功() const noexcept {
        if (状态 != L2特征概念状态::已读取 || 合同版本 != 1 || !事实截止)
            return false;
        if (!有效(概念.值))
            return false;
        for (std::size_t i = 0; i < 观测组.size(); ++i) {
            const auto& x = 观测组[i];
            if (!特征概念载荷完整(x) || x.概念 != 概念 || x.创建代次 > 事实截止)
                return false;
            for (std::size_t j = 0; j < i; ++j)
                if (观测组[j].记录节点 == x.记录节点 || 观测组[j].观测稳定编码 == x.观测稳定编码)
                    return false;
            if (i
                && (观测组[i - 1].观测时间.纳秒 > x.观测时间.纳秒
                    || (观测组[i - 1].观测时间 == x.观测时间
                        && !(观测组[i - 1].观测稳定编码 < x.观测稳定编码))))
                return false;
        }
        return true;
    }
};

} // namespace 海中鱼巣

namespace 海中鱼巣 {
struct L2特征观测实例引用项 final {
    稳定编码 观测记录{};
    稳定编码 实例关系{};
    friend bool operator==(const L2特征观测实例引用项&, const L2特征观测实例引用项&) = default;
};
struct L2特征实例当前引用清单 final {
    L2新特征实例引用 实例;
    std::uint64_t 实例见证截止 = 0;
    std::optional<稳定编码> 名称关系;
    std::vector<L2特征观测实例引用项> 观测实例关系;
    friend bool operator==(const L2特征实例当前引用清单&, const L2特征实例当前引用清单&) = default;
};
inline bool 特征实例引用清单完整(const L2特征实例当前引用清单& x, std::uint64_t g) noexcept {
    if (!有效(x.实例.值) || !x.实例见证截止 || x.实例见证截止 > g || (x.名称关系 && !有效(*x.名称关系)))
        return false;
    for (std::size_t i = 0; i < x.观测实例关系.size(); ++i) {
        const auto& item = x.观测实例关系[i];
        if (!有效(item.观测记录) || !有效(item.实例关系) || (x.名称关系 && *x.名称关系 == item.实例关系)
            || (i && !(x.观测实例关系[i - 1].实例关系 < item.实例关系)))
            return false;
        for (std::size_t j = 0; j < i; ++j)
            if (x.观测实例关系[j].观测记录 == item.观测记录)
                return false;
    }
    return true;
}
struct L2特征实例引用清单请求 final {
    L2结构请求头 请求头;
    std::uint32_t 合同版本 = 1;
    L2新特征实例引用 实例;
    std::uint64_t 实例见证截止 = 0;
    std::uint64_t 最大引用数 = 0;
};
struct L2特征实例引用清单结果 final {
    L2特征概念状态 状态 = L2特征概念状态::入口拒绝;
    std::uint32_t 合同版本 = 1;
    std::uint64_t 事实截止 = 0;
    std::optional<L2特征实例当前引用清单> 清单;
    bool 成功() const noexcept {
        return 状态 == L2特征概念状态::已读取 && 合同版本 == 1 && 事实截止 && 清单
               && 特征实例引用清单完整(*清单, 事实截止);
    }
};
struct L2特征实例引用释放请求 final {
    L2结构请求头 请求头;
    std::uint32_t 合同版本 = 1;
    L2结构幂等身份 幂等身份;
    L2特征实例当前引用清单 期望清单;
    std::uint64_t 最大引用数 = 0;
    friend bool operator==(const L2特征实例引用释放请求&, const L2特征实例引用释放请求&) = default;
};
struct L2特征实例引用释放结果 final {
    L2特征概念状态 状态 = L2特征概念状态::入口拒绝;
    std::uint32_t 合同版本 = 1;
    std::uint64_t 事实截止 = 0;
    std::optional<std::uint64_t> 发布截止;
    std::optional<L2特征实例当前引用清单> 释放前清单;
    std::vector<稳定编码> 已退出关系;
    std::vector<稳定编码> 保留观测记录;
    bool 成功() const noexcept {
        if (合同版本 != 1 || !事实截止 || !释放前清单 || !特征实例引用清单完整(*释放前清单, 事实截止))
            return false;
        const auto& x = *释放前清单;
        const auto count = x.观测实例关系.size() + (x.名称关系 ? 1 : 0);
        if (状态 == L2特征概念状态::无须释放)
            return !count && 已退出关系.empty() && 保留观测记录.empty() && !发布截止;
        if ((状态 != L2特征概念状态::已释放 && 状态 != L2特征概念状态::精确重复) || 发布截止 != 事实截止
            || !count || 已退出关系.size() != count || 保留观测记录.size() != x.观测实例关系.size())
            return false;
        for (std::size_t i = 0; i < 已退出关系.size(); ++i) {
            if (!有效(已退出关系[i]) || (i && !(已退出关系[i - 1] < 已退出关系[i])))
                return false;
            bool found = x.名称关系 && *x.名称关系 == 已退出关系[i];
            for (const auto& item : x.观测实例关系)
                found |= item.实例关系 == 已退出关系[i];
            if (!found)
                return false;
        }
        for (std::size_t i = 0; i < 保留观测记录.size(); ++i) {
            if (!有效(保留观测记录[i]) || (i && !(保留观测记录[i - 1] < 保留观测记录[i])))
                return false;
            bool found = false;
            for (const auto& item : x.观测实例关系)
                found |= item.观测记录 == 保留观测记录[i];
            if (!found)
                return false;
        }
        return true;
    }
};
} // namespace 海中鱼巣

namespace 海中鱼巣 {
struct L2特征概念范围请求 final {
    std::uint32_t 合同版本 = 1;
    L2结构请求头 请求头;
    L2新存在引用 宿主;
    L2新特征类型引用 特征类型;
    特征概念形成规则 本次形成规则;
    std::uint64_t 最大概念数 = 0;
};
struct L2特征概念范围结果 final {
    L2特征概念状态 状态 = L2特征概念状态::入口拒绝;
    std::uint32_t 合同版本 = 1;
    std::uint64_t 事实截止 = 0;
    L2新存在引用 宿主;
    L2新特征类型引用 特征类型;
    std::optional<特征概念形成规则> 已验证规则;
    std::vector<L2特征概念事实> 概念组;
    bool 成功() const noexcept {
        if (状态 != L2特征概念状态::已读取 || 合同版本 != 1 || !事实截止 || !有效(宿主.值)
            || !有效(特征类型.值) || !已验证规则 || !特征概念形成规则有效(*已验证规则))
            return false;
        for (std::size_t i = 0; i < 概念组.size(); ++i) {
            const auto& x = 概念组[i];
            if (!特征概念载荷完整(x) || !L2普通概念事实完整(x.普通概念, 事实截止)
                || (x.已成熟 && x.评估截止 > 事实截止) || x.宿主 != 宿主 || x.特征类型 != 特征类型
                || (i && !(概念组[i - 1].普通概念.概念.值 < x.普通概念.概念.值)))
                return false;
        }
        return true;
    }
};
} // namespace 海中鱼巣
