#include "计算.二次特征.h"

#include <algorithm>
#include <map>
#include <set>
#include <type_traits>

namespace 海中鱼巣::二次计算内部 {

节点键 键(const 二次计算节点身份& id) {
    return std::visit([](const auto& value) -> 节点键 {
        if constexpr (std::is_same_v<std::decay_t<decltype(value)>, 二次本次节点身份>) {
            return {true, value.局部编号};
        } else {
            return {false, value.定义.结点.值};
        }
    }, id);
}

二次计算节点身份 身份(节点键 key) {
    if (key.first) return 二次本次节点身份{key.second};
    return 二次已保存定义节点身份{{稳定编码{key.second}}};
}

二次计算来源 来源(const 二次根输出& root) {
    return std::visit([](const auto& source) -> 二次计算来源 { return source; }, root.来源);
}

节点键 输出键(const 二次计算来源& source) {
    if (const auto* local = std::get_if<二次本次输出来源>(&source))
        return {true, local->局部编号};
    return {false, std::get<二次已保存定义输出来源>(source).定义.结点.值};
}

特征类标量结果角色 输出角色(const 二次计算来源& source) {
    if (const auto* local = std::get_if<二次本次输出来源>(&source)) return local->输出角色;
    return std::get<二次已保存定义输出来源>(source).输出角色;
}

unsigned 角色位(特征类标量结果角色 role) noexcept {
    const auto value = static_cast<unsigned>(role);
    return value >= 1 && value <= 3 ? 1U << (value - 1) : 0;
}

unsigned 上下文位(const 二次计算上下文& context) noexcept {
    return (context.参与者A ? 1U : 0U) | (context.参与者B ? 2U : 0U) |
        (context.左时间 ? 4U : 0U) | (context.右时间 ? 8U : 0U) |
        (context.参照 ? 16U : 0U);
}

bool 上下文有效(const 二次计算上下文& context) noexcept {
    return (!context.参与者A || 有效(*context.参与者A)) &&
        (!context.参与者B || 有效(*context.参与者B)) &&
        (!context.参照 || 有效(*context.参照));
}

bool 来源有效(const 二次计算来源& source) noexcept {
    if (source.valueless_by_exception()) return false;
    if (const auto* exact = std::get_if<二次准确F来源>(&source)) return 有效(exact->F);
    if (const auto* local = std::get_if<二次本次输出来源>(&source))
        return local->局部编号 && 角色位(local->输出角色);
    const auto& saved = std::get<二次已保存定义输出来源>(source);
    return 有效(saved.定义.结点) && 角色位(saved.输出角色);
}

std::optional<std::int64_t> 整数(const 准确特征读取事实& fact) noexcept {
    if (const auto* value = std::get_if<std::int64_t>(&fact.完整值)) return *value;
    if (const auto* material = std::get_if<特征值信息>(&fact.完整值))
        if (const auto* value = std::get_if<std::int64_t>(&material->值内容)) return *value;
    return {};
}

bool 准确来源完整(const 准确特征读取事实& fact) noexcept {
    if (!浅层结构有效(fact.信息) || !有效(fact.类型关系) ||
        (fact.准确值事实 && !有效(*fact.准确值事实))) return false;
    if (const auto* direct = std::get_if<std::int64_t>(&fact.信息.准确值)) {
        const auto* value = std::get_if<std::int64_t>(&fact.完整值);
        return value && *value == *direct;
    }
    const auto* identity = std::get_if<特征值身份>(&fact.信息.准确值);
    const auto* material = std::get_if<特征值信息>(&fact.完整值);
    return identity && material && fact.准确值事实 &&
        material->值身份 == *identity && material->值身份.编码 == *fact.准确值事实 &&
        std::holds_alternative<std::int64_t>(material->值内容);
}

bool 快照完整(const 有序I64比较合同快照& snapshot) {
    if (!有效(snapshot.K) ||
        (snapshot.来源 != 特征比较合同来源::当前独立绑定 &&
         snapshot.来源 != 特征比较合同来源::已保存定义固定K)) return false;
    特征I64比较绑定定义 definition{snapshot.输入FT, snapshot.用途,
        snapshot.算法族, snapshot.算法版本, snapshot.左角色, snapshot.右角色,
        snapshot.上下文要求位, snapshot.输入量化, snapshot.误差合同版本,
        snapshot.误差预算, snapshot.相等容差, snapshot.关系编码, {}};
    for (const auto& output : snapshot.输出组) {
        if (!有效(output.输出关系)) return false;
        definition.输出组.push_back({output.输出, output.输出FT});
    }
    return I64绑定定义完整(definition);
}

bool 同量化(const 特征类标量量化合同& left,
            const 特征类标量量化合同& right) noexcept {
    return left == right;
}

有序I64比较合同快照 快照(const 特征I64比较绑定事实& fact) {
    const auto& d = fact.定义;
    return {特征比较合同来源::当前独立绑定, fact.身份, d.输入FT, d.用途,
        d.算法族, d.算法版本, d.左角色, d.右角色, d.上下文要求位,
        d.输入量化, d.误差合同版本, d.误差预算, d.相等容差,
        d.关系编码, fact.输出组};
}

bool 固定相容(const 有序I64比较合同快照& left,
              const 有序I64比较合同快照& right) noexcept {
    return left.输入FT == right.输入FT && left.用途 == right.用途 &&
        left.算法族 == right.算法族 && left.算法版本 == right.算法版本 &&
        left.左角色 == right.左角色 && left.右角色 == right.右角色 &&
        left.上下文要求位 == right.上下文要求位 &&
        left.输入量化 == right.输入量化 &&
        left.误差合同版本 == right.误差合同版本 &&
        left.误差预算 == right.误差预算 && left.相等容差 == right.相等容差 &&
        left.关系编码 == right.关系编码 && left.输出组 == right.输出组;
}

} // namespace 海中鱼巣::二次计算内部

namespace 海中鱼巣 {

bool 二次准确计算结果::成功() const noexcept {
    using namespace 二次计算内部;
    if (状态 != 二次计算状态::已计算 || !请求身份 || !上下文有效(上下文) ||
        根输出组.empty() || 结果组.size() != 根输出组.size() ||
        基础叶组.empty() || 计算项组.empty()) return false;
    for (const auto& leaf : 基础叶组)
        if (!准确来源完整(leaf.事实) || !整数(leaf.事实)) return false;
    for (const auto& item : 计算项组)
        if (!快照完整(item.K) || item.输出组.empty() || item.真实阶次 <= 1)
            return false;
    return true;
}

二次特征计算应用服务::二次特征计算应用服务(
    const 特征类数据服务& feature,
    const 有序I64特征比较提供者& provider) noexcept
    : feature_(feature), provider_(provider) {}

bool 二次特征计算应用服务::与特征服务同底座(
    const 特征类数据服务& feature) const noexcept {
    return feature_.与特征服务同底座(feature);
}

二次准确计算结果 二次特征计算应用服务::计算(
    const 二次计算请求& request) const noexcept {
    二次准确计算结果 result;
    result.请求身份 = request.请求身份;
    result.上下文 = request.上下文;
    if (!request.请求身份 || request.根输出组.empty() ||
        !二次计算内部::上下文有效(request.上下文)) return result;
    for (const auto& item : request.图项组) {
        if (!item.局部编号 || !有效(item.输入FT) ||
            item.要求结果位 < 1 || item.要求结果位 > 7 ||
            (item.预期K && !有效(*item.预期K))) return result;
        for (std::size_t i = 0; i < item.输入.size(); ++i)
            if (item.输入[i].顺序 != i + 1 ||
                !二次计算内部::来源有效(item.输入[i].来源)) return result;
    }

    // 待实现：准确叶、保存定义和当前K尚无一次current-only组合读取。
    // 纯I64方向和值计算仍由有序I64比较提供者承担，本入口不伪造共同快照。
    result.状态 = 二次计算状态::未实现;
    return result;
}

} // namespace 海中鱼巣
