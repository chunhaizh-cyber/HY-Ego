module;

#include <algorithm>
#include <cstdint>
#include <limits>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

export module 海中鱼巣.业务.应用服务.特征概念类;

import 海中鱼巣.领域.服务.L2概念结构;
import 海中鱼巣.领域.数据服务.存在类;

export namespace 海中鱼巣 {

struct 特征概念匹配请求 final {
    std::uint32_t 合同版本 = 1;
    L2结构请求头 请求头;
    L2新存在引用 宿主;
    L2新特征类型引用 特征类型;
    特征概念形成规则 规则;
    特征概念材料 原始材料;
    std::uint64_t 候选预算 = 0;
    friend bool operator==(const 特征概念匹配请求&, const 特征概念匹配请求&) = default;
};

struct 特征概念生成请求 final {
    特征概念匹配请求 匹配;
    L2结构幂等身份 概念幂等身份;
    std::vector<不可变材料身份> 来源材料组;
    friend bool operator==(const 特征概念生成请求&, const 特征概念生成请求&) = default;
};

struct 特征概念匹配结果 final {
    L2特征概念状态 状态 = L2特征概念状态::入口拒绝;
    std::uint32_t 合同版本 = 1;
    std::uint64_t 事实截止 = 0;
    std::vector<L2特征概念事实> 候选;
    bool 成功() const noexcept {
        if (状态 != L2特征概念状态::已读取 || 合同版本 != 1 || !事实截止)
            return false;
        for (const auto& x : 候选)
            if (!特征概念载荷完整(x) || !L2普通概念事实完整(x.普通概念, 事实截止))
                return false;
        return 候选.size() <= 1;
    }
};

struct 特征概念实例关联请求 final {
    std::uint32_t 合同版本 = 1;
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2新特征值引用 精确值;
    L2新存在引用 宿主;
    L2新特征类型引用 特征类型;
    L2概念身份 概念;
    std::uint64_t Gbind = 0;
    稳定编码 宿主成员关系{};
    std::optional<稳定编码> 原关系稳定编码;
    friend bool operator==(const 特征概念实例关联请求&, const 特征概念实例关联请求&) = default;
};

using 特征概念观测请求 = L2特征观测登记请求;

struct 特征概念实例读取请求 final {
    std::uint32_t 合同版本 = 1;
    L2结构请求头 请求头;
    L2新特征实例引用 实例;
};

struct 特征概念实例投影 final {
    特征类结点 实例;
    L2特征名称字段事实 名称字段;
    L2特征概念事实 概念;
};

struct 特征概念实例结果 final {
    L2特征概念状态 状态 = L2特征概念状态::入口拒绝;
    std::uint32_t 合同版本 = 1;
    std::uint64_t 事实截止 = 0;
    std::optional<特征概念实例投影> 实例;
    std::optional<std::variant<L2特征名称字段绑定请求, L2特征名称字段替换请求>> 原始写入材料;
    bool 成功() const noexcept {
        return (状态 == L2特征概念状态::已读取 || 状态 == L2特征概念状态::已绑定
                || 状态 == L2特征概念状态::精确重复)
               && 合同版本 == 1 && 事实截止 && 实例 && 实例->实例.结点 == 实例->名称字段.实例.值
               && 实例->实例.特征类型 == 实例->名称字段.特征类型.值
               && 实例->名称字段.概念 == 实例->概念.普通概念.概念 && 特征概念载荷完整(实例->名称字段)
               && 特征概念载荷完整(实例->概念);
    }
};

struct 特征概念成熟评估请求 final {
    std::uint32_t 合同版本 = 1;
    L2结构请求头 请求头;
    L2概念身份 概念;
    特征概念成熟规则 规则;
    L2结构幂等身份 成熟写幂等身份;
    std::uint64_t 最大观测数 = 0;
    friend bool operator==(const 特征概念成熟评估请求&, const 特征概念成熟评估请求&) = default;
};

struct 特征概念细分请求 final {
    std::uint32_t 合同版本 = 1;
    L2结构请求头 请求头;
    L2新存在引用 宿主;
    L2新特征类型引用 特征类型;
    特征概念形成规则 规则;
    std::uint64_t 最大概念数 = 0;
    std::uint64_t 最大观测数 = 0;
};

struct 特征概念细分候选 final {
    特征概念值域 值域;
    std::vector<稳定编码> 观测身份组;
    L2中性时间_v1 首次时间{};
    L2中性时间_v1 末次时间{};
    std::uint64_t 独立观测数 = 0;
    bool 存在重叠 = false;
};

struct 特征概念细分结果 final {
    L2特征概念状态 状态 = L2特征概念状态::入口拒绝;
    std::uint32_t 合同版本 = 1;
    std::uint64_t 事实截止 = 0;
    std::vector<特征概念细分候选> 候选;
};

using 特征概念结构写请求 =
    std::variant<L2特征概念建立请求, L2特征名称字段绑定请求, L2特征名称字段替换请求, L2特征观测登记请求,
                 L2特征概念成熟请求, L2概念治理生命周期迁移请求, L2特征实例引用释放请求>;

using 特征概念写入准备请求 =
    std::variant<特征概念生成请求, 特征概念实例关联请求, 特征概念观测请求, 特征概念成熟评估请求,
                 L2概念治理生命周期迁移请求, L2特征实例引用释放请求>;

// 只校验保存的生成输入和结构请求的纯值一致性；真实根和世界引用由 owner 读回。
inline bool 特征概念生成材料一致(const 特征概念生成请求& original, const L2特征概念建立请求& request,
                                 std::uint64_t h) noexcept {
    const auto& m = original.匹配;
    if (m.合同版本 != 1 || m.请求头.合同版本 != 1 || !h || m.请求头.期望事实代次 != h || !m.候选预算
        || request.合同版本 != 1 || request.请求头 != m.请求头 || !有效(m.宿主.值) || !有效(m.特征类型.值)
        || request.宿主 != m.宿主 || request.特征类型 != m.特征类型 || !特征概念形成规则有效(m.规则)
        || request.规则 != m.规则 || !original.概念幂等身份.值 || request.幂等身份 != original.概念幂等身份
        || original.来源材料组.empty() || request.来源材料组 != original.来源材料组
        || !有效(request.本体根.值) || request.直接上位组.size() != 1
        || request.直接上位组.front() != request.本体根 || !特征概念值域有效(request.值域))
        return false;
    for (const auto& source : original.来源材料组)
        if (!有效(source.值))
            return false;
    if (!std::visit(
            [](const auto& value) {
                if constexpr (std::is_same_v<std::decay_t<decltype(value)>, std::int64_t>)
                    return true;
                else
                    return !value.empty();
            },
            m.原始材料))
        return false;
    const auto* scalar = std::get_if<std::int64_t>(&m.原始材料);
    if (!m.规则.分区.empty() && !scalar)
        return false;
    const 特征概念I64区间* selected = nullptr;
    for (const auto& range : m.规则.分区) {
        if (*scalar < range.下界 || *scalar > range.上界)
            continue;
        if (selected)
            return false;
        selected = &range;
    }
    if (!selected)
        return request.值域.精确值 && *request.值域.精确值 == m.原始材料;
    if (selected->下界 == selected->上界)
        return request.值域.精确值 && std::get_if<std::int64_t>(&*request.值域.精确值)
               && std::get<std::int64_t>(*request.值域.精确值) == selected->下界;
    return request.值域.区间 && *request.值域.区间 == *selected;
}

struct 特征概念写入准备结果 final {
    L2特征概念状态 状态 = L2特征概念状态::入口拒绝;
    std::uint32_t 合同版本 = 1;
    std::uint64_t 原事实截止 = 0;
    std::optional<特征概念结构写请求> 完整请求;
    std::optional<L2特征概念事实> 已有概念;
    std::vector<L2特征概念事实> 歧义概念组;
    std::vector<特征概念值域> 歧义值域组;
    std::optional<特征概念生成请求> 原生成请求;
    bool 成功() const noexcept {
        if (完整请求 && std::holds_alternative<L2特征概念建立请求>(*完整请求)) {
            if (!原生成请求
                || !特征概念生成材料一致(*原生成请求, std::get<L2特征概念建立请求>(*完整请求), 原事实截止))
                return false;
        } else if (原生成请求)
            return false;
        return 歧义概念组.empty() && 歧义值域组.empty() && 状态 == L2特征概念状态::已读取 && 合同版本 == 1
               && 原事实截止 && (完整请求.has_value() != 已有概念.has_value())
               && (!已有概念 || 特征概念载荷完整(*已有概念));
    }
};

struct 特征概念写入结果 final {
    L2特征概念状态 状态 = L2特征概念状态::入口拒绝;
    std::uint32_t 合同版本 = 1;
    std::uint64_t 事实截止 = 0;
    std::optional<特征概念结构写请求> 原始写入材料;
    std::optional<L2特征概念事实> 概念;
    std::optional<L2特征名称字段事实> 名称字段;
    std::optional<L2特征观测事实> 观测;
    std::optional<L2概念治理生命周期事实> 生命周期;
    std::optional<L2特征实例引用释放结果> 引用释放;
    std::optional<std::uint64_t> 发布截止;
    std::vector<L2特征概念事实> 歧义概念组;
    std::vector<特征概念值域> 歧义值域组;
    std::optional<特征概念生成请求> 原生成请求;
    bool 成功() const noexcept {
        if (合同版本 != 1 || !事实截止)
            return false;
        if (原始写入材料 && std::holds_alternative<L2特征概念建立请求>(*原始写入材料)) {
            const auto& request = std::get<L2特征概念建立请求>(*原始写入材料);
            if (!原生成请求 || !特征概念生成材料一致(*原生成请求, request, request.请求头.期望事实代次))
                return false;
        } else if (原生成请求)
            return false;
        const auto count = static_cast<unsigned>(概念.has_value()) + 名称字段.has_value() + 观测.has_value()
                           + 生命周期.has_value() + 引用释放.has_value();
        if (count != 1)
            return false;
        if (概念
            && (!特征概念载荷完整(*概念) || !L2普通概念事实完整(概念->普通概念, 事实截止)
                || (概念->已成熟 && 概念->评估截止 > 事实截止)))
            return false;
        if (名称字段 && (!特征概念载荷完整(*名称字段) || 名称字段->创建代次 > 事实截止 || 名称字段->退出代次))
            return false;
        if (观测 && (!特征概念载荷完整(*观测) || 观测->创建代次 > 事实截止))
            return false;
        if (生命周期
            && (!L2概念治理生命周期事实截止投影完整(*生命周期, 事实截止) || 生命周期->生命周期.退出事实代次))
            return false;
        if (引用释放 && (!引用释放->成功() || 引用释放->事实截止 != 事实截止))
            return false;
        if (状态 == L2特征概念状态::已读取)
            return 概念 && !原始写入材料;
        if (!原始写入材料)
            return false;
        return std::visit(
            [&](const auto& r) {
                using T = std::decay_t<decltype(r)>;
                const auto repeat = 状态 == L2特征概念状态::精确重复;
                if constexpr (std::is_same_v<T, L2特征概念建立请求>) {
                    return 概念 && (repeat || 状态 == L2特征概念状态::已创建) && 概念->宿主 == r.宿主
                           && 概念->特征类型 == r.特征类型 && 概念->规则 == r.规则 && 概念->值域 == r.值域;
                } else if constexpr (std::is_same_v<T, L2特征概念成熟请求>) {
                    return 概念 && (repeat || 状态 == L2特征概念状态::已成熟) && 概念->普通概念.概念 == r.概念
                           && 概念->已成熟 && 概念->成熟规则 == r.规则 && 概念->评估截止 == r.观测证据截止
                           && 概念->成熟证据身份组 == r.观测记录身份组;
                } else if constexpr (std::is_same_v<T, L2特征名称字段绑定请求>
                                     || std::is_same_v<T, L2特征名称字段替换请求>) {
                    return 名称字段 && (repeat || 状态 == L2特征概念状态::已绑定) && 名称字段->实例 == r.实例
                           && 名称字段->宿主 == r.宿主 && 名称字段->特征类型 == r.特征类型
                           && 名称字段->概念 == r.概念
                           && (repeat
                               || (名称字段->Gbind == r.Gbind && 名称字段->宿主成员关系 == r.宿主成员关系));
                } else if constexpr (std::is_same_v<T, L2特征观测登记请求>) {
                    return 观测 && (repeat || 状态 == L2特征概念状态::已记录)
                           && 观测->观测稳定编码 == r.观测稳定编码 && 观测->概念 == r.概念
                           && 观测->宿主 == r.宿主 && 观测->特征类型 == r.特征类型 && 观测->精确值 == r.精确值
                           && 观测->观测时间 == r.观测时间 && 观测->来源材料 == r.来源材料
                           && 观测->Gobs == r.Gobs && 观测->宿主成员关系 == r.宿主成员关系;
                } else if constexpr (std::is_same_v<T, L2特征实例引用释放请求>) {
                    return 引用释放 && 引用释放->状态 == 状态 && 引用释放->释放前清单 == r.期望清单;
                } else {
                    return 生命周期 && (repeat || 状态 == L2特征概念状态::已迁移) && 生命周期->概念 == r.概念
                           && 生命周期->状态 == r.目标状态
                           && 生命周期->事实稳定编码 != r.当前生命周期事实稳定编码;
                }
            },
            *原始写入材料);
    }
};

using 特征概念生成结果 = 特征概念写入结果;
using 特征概念成熟结果 = 特征概念写入结果;

class 特征概念应用服务 final {
    using S = L2特征概念状态;

  public:
    特征概念应用服务(L2概念结构服务& conceptService, const 特征类数据服务& feature,
                     const 存在类数据服务& existence, const 特征值类数据服务& value)
        : 概念服务_(conceptService), 特征服务_(feature), 存在服务_(existence), 值服务_(value) {
        if (!conceptService.特征概念依赖匹配(feature, existence, value))
            throw std::invalid_argument("feature concept providers must share configured instances");
    }
    特征概念应用服务(const 特征概念应用服务&) = delete;
    特征概念应用服务& operator=(const 特征概念应用服务&) = delete;

    特征概念匹配结果 查找匹配概念(const 特征概念匹配请求& r) const {
        特征概念匹配结果 out;
        out.事实截止 = r.请求头.期望事实代次;
        try {
            if (r.合同版本 != 1 || r.请求头.合同版本 != 1 || !r.请求头.期望事实代次 || !有效(r.宿主.值)
                || !有效(r.特征类型.值) || !r.候选预算 || !特征概念形成规则有效(r.规则)
                || !特征概念值域有效(单点(r.原始材料)))
                return out;
            const auto type = 特征服务_.确认特征类型({1, r.请求头.期望事实代次, r.特征类型.值});
            if (!type.成功()) {
                out.状态 = 历史状态(type.状态);
                return out;
            }
            const auto representation = std::visit(
                [](const auto& v) {
                    using T = std::decay_t<decltype(v)>;
                    if constexpr (std::is_same_v<T, std::int64_t>)
                        return L1所有者范围值表示种类::I64;
                    else if constexpr (std::is_same_v<T, std::vector<std::int64_t>>)
                        return L1所有者范围值表示种类::I64组;
                    else
                        return L1所有者范围值表示种类::U64组;
                },
                r.原始材料);
            if (*type.表示 != representation && *type.表示 != L1所有者范围值表示种类::独立材料引用) {
                out.状态 = S::引用冲突;
                return out;
            }
            if (!r.规则.分区.empty() && representation != L1所有者范围值表示种类::I64) {
                out.状态 = S::规则不支持;
                return out;
            }
            auto rows = 概念服务_.按宿主和类型读取特征概念(
                {1, r.请求头, r.宿主, r.特征类型, r.规则.规则身份, r.规则.规则版本, r.候选预算});
            if (!rows.成功()) {
                out.状态 = rows.状态;
                return out;
            }
            for (const auto& x : rows.概念组) {
                if (x.规则 != r.规则) {
                    out.状态 = S::幂等冲突;
                    out.候选.clear();
                    return out;
                }
                if (x.普通概念.治理生命周期.状态 == L2概念治理生命周期状态::活跃 && 包含(x.值域, r.原始材料))
                    out.候选.push_back(x);
            }
            out.状态 = out.候选.size() > 1 ? S::匹配歧义 : S::已读取;
            return out;
        } catch (const std::bad_alloc&) {
            out.状态 = S::资源失败;
        } catch (...) {
            out.状态 = S::内部不一致;
        }
        out.候选.clear();
        return out;
    }

  private:
    void 核验固定写入业务(const 特征概念结构写请求& input) const {
        const auto g = 当前截止();
        std::visit(
            [&](const auto& r) {
                using T = std::decay_t<decltype(r)>;
                const auto h = r.请求头.期望事实代次;
                if (!h || h > g)
                    throw 业务失败{S::事实代次漂移};
                if constexpr (std::is_same_v<T, L2特征名称字段绑定请求>
                              || std::is_same_v<T, L2特征名称字段替换请求>) {
                    auto f = 特征服务_.按实例读取特征历史事实({1, g, r.Gbind, r.实例.值, 1});
                    if (!f.成功())
                        throw 业务失败{历史状态(f.状态)};
                    const auto definition = 读取概念(r.概念, g, h);
                    if (f.特征->特征类型 != r.特征类型.值 || definition.宿主 != r.宿主
                        || definition.特征类型 != r.特征类型
                        || definition.普通概念.治理生命周期.状态 != L2概念治理生命周期状态::活跃
                        || !包含(definition.值域, 解析特征值(*f.特征, g, r.Gbind)))
                        throw 业务失败{S::引用冲突};
                } else if constexpr (std::is_same_v<T, L2特征观测登记请求>) {
                    auto f = 特征服务_.读取特征历史事实({1, g, r.Gobs, r.精确值.实例.值, r.精确值.值事实});
                    if (!f.成功())
                        throw 业务失败{历史状态(f.状态)};
                    const auto definition = 读取概念(r.概念, g, h);
                    if (definition.宿主 != r.宿主 || definition.特征类型 != r.特征类型
                        || definition.普通概念.治理生命周期.状态 != L2概念治理生命周期状态::活跃
                        || !包含(definition.值域, 解析特征值(*f.特征, g, r.Gobs)))
                        throw 业务失败{S::引用冲突};
                } else if constexpr (std::is_same_v<T, L2特征概念成熟请求>) {
                    if (!特征概念成熟规则有效(r.规则) || !r.观测证据截止 || r.观测证据截止 > h)
                        throw 业务失败{S::入口拒绝};
                    const auto definition = 读取概念(r.概念, g, h);
                    auto rows = 概念服务_.读取特征概念观测({1,
                                                            {1, g},
                                                            r.概念,
                                                            L2读取类别::历史,
                                                            r.观测证据截止,
                                                            (std::numeric_limits<std::uint64_t>::max)()});
                    if (!rows.成功())
                        throw 业务失败{rows.状态};
                    std::vector<L2特征观测事实> counted;
                    for (const auto& obs : rows.观测组) {
                        auto f = 特征服务_.读取特征历史事实(
                            {1, g, obs.Gobs, obs.精确值.实例.值, obs.精确值.值事实});
                        if (!f.成功())
                            throw 业务失败{历史状态(f.状态)};
                        if (!包含(definition.值域, 解析特征值(*f.特征, g, obs.Gobs)))
                            throw 业务失败{S::引用冲突};
                        if (counted.empty()
                            || static_cast<std::uint64_t>(obs.观测时间.纳秒)
                                       - static_cast<std::uint64_t>(counted.back().观测时间.纳秒)
                                   >= static_cast<std::uint64_t>(r.规则.最小观测间隔.纳秒))
                            counted.push_back(obs);
                    }
                    if (counted.size() >= 2)
                        counted.back() = rows.观测组.back();
                    if (counted.size() < r.规则.最少独立观测数
                        || static_cast<std::uint64_t>(counted.back().观测时间.纳秒)
                                   - static_cast<std::uint64_t>(counted.front().观测时间.纳秒)
                               < static_cast<std::uint64_t>(r.规则.最短观察跨度.纳秒))
                        throw 业务失败{S::证据不足};
                    std::vector<稳定编码> ids;
                    for (const auto& obs : counted)
                        ids.push_back(obs.记录节点);
                    if (ids != r.观测记录身份组)
                        throw 业务失败{S::引用冲突};
                } else if constexpr (std::is_same_v<T, L2概念治理生命周期迁移请求>) {
                    static_cast<void>(读取概念(r.概念, g, h));
                }
            },
            input);
        if (当前截止() != g)
            throw 业务失败{S::事实代次漂移};
    }
    static L2结构状态 旧结构状态(S state) noexcept {
        switch (state) {
        case S::入口拒绝:
            return L2结构状态::入口拒绝;
        case S::未找到:
            return L2结构状态::未找到;
        case S::目标已退出:
            return L2结构状态::已退出;
        case S::结构未配置:
            return L2结构状态::未实现;
        case S::事实代次漂移:
            return L2结构状态::事实代次漂移;
        case S::幂等冲突:
            return L2结构状态::幂等冲突;
        case S::引用冲突:
            return L2结构状态::引用冲突;
        case S::数量预算不足:
            return L2结构状态::数量预算不足;
        case S::资源失败:
            return L2结构状态::资源失败;
        default:
            return L2结构状态::内部不一致;
        }
    }
    static S 结构状态(L2结构状态 s) noexcept {
        switch (s) {
        case L2结构状态::已提交:
            return S::已迁移;
        case L2结构状态::精确重复:
            return S::精确重复;
        case L2结构状态::已读取:
            return S::已读取;
        case L2结构状态::入口拒绝:
            return S::入口拒绝;
        case L2结构状态::未找到:
            return S::未找到;
        case L2结构状态::已退出:
            return S::目标已退出;
        case L2结构状态::未实现:
            return S::结构未配置;
        case L2结构状态::事实代次漂移:
            return S::事实代次漂移;
        case L2结构状态::幂等冲突:
            return S::幂等冲突;
        case L2结构状态::引用冲突:
            return S::引用冲突;
        case L2结构状态::数量预算不足:
            return S::数量预算不足;
        case L2结构状态::资源失败:
            return S::资源失败;
        default:
            return S::内部不一致;
        }
    }
    std::uint64_t 当前截止() const {
        const auto roots = 概念服务_.读取全部当前概念本体根({{1, 0}});
        if (!roots.成功())
            throw 业务失败{结构状态(roots.结果头.状态)};
        return roots.结果头.事实截止代次;
    }
    特征概念材料 解析特征值(const 特征类结点& f, std::uint64_t g, std::uint64_t h) const {
        return std::visit(
            [&](const auto& value) -> 特征概念材料 {
                using T = std::decay_t<decltype(value)>;
                if constexpr (std::is_same_v<T, 特征直接值>) {
                    return std::visit([](const auto& x) -> 特征概念材料 { return x; }, value);
                } else {
                    const auto result = 值服务_.按结点读取不可变材料历史事实({1, g, h, value.结点, 1});
                    if (!result.成功())
                        throw 业务失败{历史状态(result.状态)};
                    return std::visit(
                        [](const auto& x) -> 特征概念材料 {
                            if constexpr (std::is_same_v<std::decay_t<decltype(x)>, L1所有者范围独立材料引用>)
                                throw 业务失败{S::规则不支持};
                            else
                                return x;
                        },
                        result.材料->特征值);
                }
            },
            f.特征值);
    }
    L2特征概念事实 读取概念(L2概念身份 id, std::uint64_t g, std::uint64_t h) const {
        const auto result = 概念服务_.读取特征概念({1, {1, g}, id, L2读取类别::历史, h});
        if (!result.成功())
            throw 业务失败{result.状态};
        return *result.概念;
    }
    特征类结点 核验关联(const 特征概念实例关联请求& r) const {
        const auto g = r.请求头.期望事实代次;
        if (r.合同版本 != 1 || r.请求头.合同版本 != 1 || !g || !r.幂等身份.值 || !r.Gbind || r.Gbind > g
            || !有效(r.宿主.值) || !有效(r.特征类型.值) || !有效(r.概念.值) || !有效(r.宿主成员关系))
            throw 业务失败{S::入口拒绝};
        if (!有效(r.精确值.实例.值) || !有效(r.精确值.值事实))
            throw 业务失败{S::材料不足};
        const auto feature = 特征服务_.读取特征历史事实({1, g, r.Gbind, r.精确值.实例.值, r.精确值.值事实});
        if (!feature.成功())
            throw 业务失败{历史状态(feature.状态)};
        if (feature.特征->特征类型 != r.特征类型.值)
            throw 业务失败{S::引用冲突};
        const auto member =
            存在服务_.读取特征成员历史事实({1, g, r.Gbind, r.宿主.值, r.宿主成员关系, r.精确值.实例.值});
        if (!member.成功())
            throw 业务失败{历史状态(member.状态)};
        const auto current = 特征服务_.按实例读取特征历史事实({1, g, g, r.精确值.实例.值, 1});
        if (!current.成功())
            throw 业务失败{历史状态(current.状态)};
        const auto definition = 读取概念(r.概念, g, g);
        if (definition.宿主 != r.宿主 || definition.特征类型 != r.特征类型
            || definition.普通概念.治理生命周期.状态 != L2概念治理生命周期状态::活跃
            || !包含(definition.值域, 解析特征值(*feature.特征, g, r.Gbind)))
            throw 业务失败{S::引用冲突};
        return *feature.特征;
    }
    特征概念写入准备结果 准备生成(const 特征概念生成请求& r) const {
        特征概念写入准备结果 out;
        out.原事实截止 = r.匹配.请求头.期望事实代次;
        if (!r.概念幂等身份.值 || r.来源材料组.empty())
            return out;
        const auto matches = 查找匹配概念(r.匹配);
        if (!matches.成功()) {
            out.状态 = matches.状态;
            if (matches.状态 == S::匹配歧义)
                out.歧义概念组 = matches.候选;
            return out;
        }
        if (!matches.候选.empty()) {
            out.状态 = S::已读取;
            out.已有概念 = matches.候选.front();
            return out;
        }
        auto domain = 单点(r.匹配.原始材料);
        std::optional<特征概念值域> selected;
        if (!r.匹配.规则.分区.empty()) {
            const auto* scalar = std::get_if<std::int64_t>(&r.匹配.原始材料);
            if (!scalar) {
                out.状态 = S::规则不支持;
                return out;
            }
            for (const auto& interval : r.匹配.规则.分区) {
                if (*scalar < interval.下界 || *scalar > interval.上界)
                    continue;
                if (out.歧义值域组.size() == r.匹配.候选预算) {
                    out.状态 = S::数量预算不足;
                    out.歧义值域组.clear();
                    return out;
                }
                out.歧义值域组.push_back(区间域(interval));
                selected = 区间域(interval);
            }
            if (out.歧义值域组.size() > 1) {
                out.状态 = S::匹配歧义;
                return out;
            }
            out.歧义值域组.clear();
            if (selected)
                domain = *selected;
        }
        const auto root = 概念服务_.按角色读取当前概念本体根({r.匹配.请求头, L2概念本体根角色::特征});
        if (!root.成功()) {
            out.状态 = 结构状态(root.结果头.状态);
            return out;
        }
        L2特征概念建立请求 request;
        request.请求头 = r.匹配.请求头;
        request.幂等身份 = r.概念幂等身份;
        request.本体根 = root.本体根->根概念;
        request.宿主 = r.匹配.宿主;
        request.特征类型 = r.匹配.特征类型;
        request.规则 = r.匹配.规则;
        request.值域 = std::move(domain);
        request.来源材料组 = r.来源材料组;
        request.直接上位组 = {request.本体根};
        out.完整请求 = std::move(request);
        out.原生成请求 = r;
        out.状态 = S::已读取;
        return out;
    }
    L2特征概念成熟请求 准备成熟(const 特征概念成熟评估请求& r) const {
        const auto g = r.请求头.期望事实代次;
        if (r.合同版本 != 1 || r.请求头.合同版本 != 1 || !g || !有效(r.概念.值) || !r.成熟写幂等身份.值
            || !r.最大观测数 || !特征概念成熟规则有效(r.规则))
            throw 业务失败{S::入口拒绝};
        const auto definition = 读取概念(r.概念, g, g);
        auto observations =
            概念服务_.读取特征概念观测({1, r.请求头, r.概念, L2读取类别::当前, 0, r.最大观测数});
        if (!observations.成功())
            throw 业务失败{observations.状态};
        std::vector<L2特征观测事实> counted;
        for (const auto& obs : observations.观测组) {
            const auto f =
                特征服务_.读取特征历史事实({1, g, obs.Gobs, obs.精确值.实例.值, obs.精确值.值事实});
            if (!f.成功())
                throw 业务失败{历史状态(f.状态)};
            if (!包含(definition.值域, 解析特征值(*f.特征, g, obs.Gobs)))
                throw 业务失败{S::引用冲突};
            if (counted.empty()
                || static_cast<std::uint64_t>(obs.观测时间.纳秒)
                           - static_cast<std::uint64_t>(counted.back().观测时间.纳秒)
                       >= static_cast<std::uint64_t>(r.规则.最小观测间隔.纳秒))
                counted.push_back(obs);
        }
        if (counted.size() >= 2)
            counted.back() = observations.观测组.back();
        if (counted.size() < r.规则.最少独立观测数
            || static_cast<std::uint64_t>(counted.back().观测时间.纳秒)
                       - static_cast<std::uint64_t>(counted.front().观测时间.纳秒)
                   < static_cast<std::uint64_t>(r.规则.最短观察跨度.纳秒))
            throw 业务失败{S::证据不足};
        L2特征概念成熟请求 request;
        request.请求头 = r.请求头;
        request.幂等身份 = r.成熟写幂等身份;
        request.概念 = r.概念;
        request.原程度值事实 = definition.程度值事实;
        request.规则 = r.规则;
        request.观测证据截止 = g;
        for (const auto& obs : counted)
            request.观测记录身份组.push_back(obs.记录节点);
        return request;
    }

  public:
    特征概念写入准备结果 准备特征概念写入(const 特征概念写入准备请求& input) const {
        特征概念写入准备结果 out;
        try {
            std::visit(
                [&](const auto& r) {
                    using T = std::decay_t<decltype(r)>;
                    if constexpr (std::is_same_v<T, 特征概念生成请求>) {
                        out = 准备生成(r);
                    } else {
                        out.原事实截止 = r.请求头.期望事实代次;
                        if constexpr (std::is_same_v<T, 特征概念实例关联请求>) {
                            static_cast<void>(核验关联(r));
                            L2特征名称字段绑定请求 base{
                                1,          r.请求头, r.幂等身份, r.精确值.实例, r.宿主,
                                r.特征类型, r.概念,   r.Gbind,    r.宿主成员关系};
                            if (r.原关系稳定编码) {
                                L2特征名称字段替换请求 replace;
                                replace.请求头 = base.请求头;
                                replace.幂等身份 = base.幂等身份;
                                replace.实例 = base.实例;
                                replace.宿主 = base.宿主;
                                replace.特征类型 = base.特征类型;
                                replace.概念 = base.概念;
                                replace.Gbind = base.Gbind;
                                replace.宿主成员关系 = base.宿主成员关系;
                                replace.原关系稳定编码 = *r.原关系稳定编码;
                                out.完整请求 = replace;
                            } else
                                out.完整请求 = base;
                        } else if constexpr (std::is_same_v<T, 特征概念观测请求>) {
                            特征概念实例关联请求 check{r.合同版本, r.请求头, r.幂等身份,
                                                       r.精确值,   r.宿主,   r.特征类型,
                                                       r.概念,     r.Gobs,   r.宿主成员关系};
                            static_cast<void>(核验关联(check));
                            out.完整请求 = r;
                        } else if constexpr (std::is_same_v<T, 特征概念成熟评估请求>) {
                            out.完整请求 = 准备成熟(r);
                        } else if constexpr (std::is_same_v<T, L2概念治理生命周期迁移请求>) {
                            if (!L2概念治理生命周期迁移请求有效(r))
                                throw 业务失败{S::入口拒绝};
                            const auto conceptFact = 读取概念(r.概念, out.原事实截止, out.原事实截止);
                            if (conceptFact.普通概念.治理生命周期.事实稳定编码 != r.当前生命周期事实稳定编码
                                || conceptFact.普通概念.治理生命周期.状态 != r.当前状态)
                                throw 业务失败{S::引用冲突};
                            out.完整请求 = r;
                        } else {
                            const auto list = 概念服务_.读取特征实例当前引用清单(
                                {r.请求头, 1, r.期望清单.实例, r.期望清单.实例见证截止, r.最大引用数});
                            if (!list.成功())
                                throw 业务失败{list.状态};
                            if (*list.清单 != r.期望清单)
                                throw 业务失败{S::引用冲突};
                            out.完整请求 = r;
                        }
                        out.状态 = S::已读取;
                    }
                },
                input);
            return out;
        } catch (const 业务失败& e) {
            out.状态 = e.状态;
        } catch (const std::bad_alloc&) {
            out.状态 = S::资源失败;
        } catch (...) {
            out.状态 = S::内部不一致;
        }
        out.完整请求.reset();
        out.已有概念.reset();
        return out;
    }
    特征概念写入结果 重放特征概念写入(const 特征概念写入准备结果& prepared) {
        特征概念写入结果 out;
        out.事实截止 = prepared.原事实截止;
        out.原始写入材料 = prepared.完整请求;
        out.原生成请求 = prepared.原生成请求;
        try {
            if (prepared.完整请求 && std::holds_alternative<L2特征概念建立请求>(*prepared.完整请求)
                && prepared.原生成请求) {
                const auto& original = *prepared.原生成请求;
                const auto& m = original.匹配;
                if (特征概念形成规则有效(m.规则) && !m.规则.分区.empty()) {
                    const auto* scalar = std::get_if<std::int64_t>(&m.原始材料);
                    if (!scalar)
                        throw 业务失败{S::规则不支持};
                    for (const auto& interval : m.规则.分区) {
                        if (*scalar < interval.下界 || *scalar > interval.上界)
                            continue;
                        if (out.歧义值域组.size() == m.候选预算) {
                            out.歧义值域组.clear();
                            throw 业务失败{S::数量预算不足};
                        }
                        out.歧义值域组.push_back(区间域(interval));
                    }
                    if (out.歧义值域组.size() > 1)
                        throw 业务失败{S::匹配歧义};
                    out.歧义值域组.clear();
                }
                if (!特征概念生成材料一致(original, std::get<L2特征概念建立请求>(*prepared.完整请求),
                                          prepared.原事实截止))
                    throw 业务失败{S::引用冲突};
            }
            if (!prepared.成功())
                return out;
            if (prepared.已有概念) {
                const auto id = prepared.已有概念->普通概念.概念;
                const auto actual = 读取概念(id, 当前截止(), prepared.原事实截止);
                if (actual != *prepared.已有概念)
                    throw 业务失败{S::引用冲突};
                out.概念 = actual;
                out.状态 = S::已读取;
                return out;
            }
            out.原始写入材料 = prepared.完整请求;
            if (std::holds_alternative<L2特征概念建立请求>(*prepared.完整请求)) {
                const auto g = 当前截止();
                if (g < prepared.原事实截止)
                    throw 业务失败{S::事实代次漂移};
                if (g == prepared.原事实截止) {
                    const auto verified = 准备生成(*prepared.原生成请求);
                    if (!verified.完整请求) {
                        out.状态 = verified.状态;
                        out.歧义概念组 = verified.歧义概念组;
                        out.歧义值域组 = verified.歧义值域组;
                        // A forged creation request does not become a successful reuse write.
                        if (verified.已有概念)
                            out.状态 = S::引用冲突;
                        return out;
                    }
                    if (*verified.完整请求 != *prepared.完整请求)
                        throw 业务失败{S::引用冲突};
                }
            }
            核验固定写入业务(*prepared.完整请求);
            std::visit(
                [&](const auto& r) {
                    using T = std::decay_t<decltype(r)>;
                    if (r.请求头.期望事实代次 != prepared.原事实截止)
                        throw 业务失败{S::入口拒绝};
                    if constexpr (std::is_same_v<T, L2特征概念建立请求>
                                  || std::is_same_v<T, L2特征概念成熟请求>) {
                        const auto result = [&] {
                            if constexpr (std::is_same_v<T, L2特征概念建立请求>)
                                return 概念服务_.建立特征概念(r);
                            else
                                return 概念服务_.发布特征概念成熟(r);
                        }();
                        out.状态 = result.状态;
                        out.事实截止 = result.事实截止;
                        out.发布截止 = result.发布截止;
                        if (result.成功())
                            out.概念 = result.概念;
                    } else if constexpr (std::is_same_v<T, L2特征名称字段绑定请求>
                                         || std::is_same_v<T, L2特征名称字段替换请求>) {
                        const auto result = [&] {
                            if constexpr (std::is_same_v<T, L2特征名称字段绑定请求>)
                                return 概念服务_.绑定特征名称字段节点(r);
                            else
                                return 概念服务_.替换特征名称字段节点(r);
                        }();
                        out.状态 = result.状态;
                        out.事实截止 = result.事实截止;
                        out.发布截止 = result.发布截止;
                        if (result.成功())
                            out.名称字段 = result.名称字段;
                    } else if constexpr (std::is_same_v<T, L2特征观测登记请求>) {
                        const auto result = 概念服务_.登记特征概念观测(r);
                        out.状态 = result.状态;
                        out.事实截止 = result.事实截止;
                        out.发布截止 = result.发布截止;
                        if (result.成功())
                            out.观测 = result.观测;
                    } else if constexpr (std::is_same_v<T, L2特征实例引用释放请求>) {
                        const auto result = 概念服务_.释放特征实例当前引用(r);
                        out.状态 = result.状态;
                        out.事实截止 = result.事实截止;
                        out.发布截止 = result.发布截止;
                        if (result.成功())
                            out.引用释放 = result;
                    } else {
                        static_cast<void>(读取概念(r.概念, 当前截止(), r.请求头.期望事实代次));
                        const auto result = 概念服务_.迁移概念治理生命周期(r);
                        out.状态 = 结构状态(result.结果头.状态);
                        out.事实截止 = result.结果头.事实截止代次;
                        if (result.成功())
                            out.生命周期 = result.当前生命周期;
                        else if (result.结果头.状态 == L2结构状态::资源失败
                                 || result.结果头.状态 == L2结构状态::内部不一致)
                            out.状态 = S::已可能发布;
                    }
                },
                *prepared.完整请求);
            if ((out.概念 || out.名称字段 || out.观测 || out.生命周期 || out.引用释放) && !out.成功())
                throw 业务失败{S::已可能发布};
            return out;
        } catch (const 业务失败& e) {
            out.状态 = e.状态;
        } catch (...) {
            out.状态 = S::已可能发布;
        }
        out.概念.reset();
        out.名称字段.reset();
        out.观测.reset();
        out.生命周期.reset();
        out.引用释放.reset();
        return out;
    }
    特征概念生成结果 取得或生成概念(const 特征概念生成请求& r) {
        const auto prepared = 准备特征概念写入(r);
        if (!prepared.成功()) {
            特征概念生成结果 out;
            out.状态 = prepared.状态;
            out.事实截止 = prepared.原事实截止;
            out.歧义概念组 = prepared.歧义概念组;
            out.歧义值域组 = prepared.歧义值域组;
            return out;
        }
        auto result = 重放特征概念写入(prepared);
        if (result.状态 == S::引用冲突 && prepared.完整请求) {
            try {
                auto fresh = r.匹配;
                fresh.请求头.期望事实代次 = 当前截止();
                auto matches = 查找匹配概念(fresh);
                const auto& expected = std::get<L2特征概念建立请求>(*prepared.完整请求);
                if (matches.成功() && matches.候选.size() == 1 && matches.候选.front().值域 == expected.值域
                    && matches.候选.front().规则 == expected.规则) {
                    result = {};
                    result.状态 = S::已读取;
                    result.事实截止 = matches.事实截止;
                    result.概念 = matches.候选.front();
                }
            } catch (...) {
            }
        }
        return result;
    }
    特征概念成熟结果 评估并发布成熟(const 特征概念成熟评估请求& r) {
        const auto prepared = 准备特征概念写入(r);
        if (!prepared.成功()) {
            特征概念成熟结果 out;
            out.状态 = prepared.状态;
            out.事实截止 = prepared.原事实截止;
            return out;
        }
        return 重放特征概念写入(prepared);
    }
    特征概念实例结果 读取特征实例(const 特征概念实例读取请求& r) const {
        特征概念实例结果 out;
        out.事实截止 = r.请求头.期望事实代次;
        try {
            if (r.合同版本 != 1 || r.请求头.合同版本 != 1 || !out.事实截止 || !有效(r.实例.值))
                return out;
            const auto g = out.事实截止;
            auto feature = 特征服务_.按实例读取特征历史事实({1, g, g, r.实例.值, 1});
            if (!feature.成功())
                throw 业务失败{历史状态(feature.状态)};
            auto name = 概念服务_.读取特征名称字段节点({1, r.请求头, r.实例});
            if (!name.成功())
                throw 业务失败{name.状态};
            auto definition = 读取概念(name.名称字段->概念, g, g);
            out.实例 = 特征概念实例投影{*feature.特征, *name.名称字段, std::move(definition)};
            out.状态 = S::已读取;
            return out;
        } catch (const 业务失败& e) {
            out.状态 = e.状态;
        } catch (const std::bad_alloc&) {
            out.状态 = S::资源失败;
        } catch (...) {
            out.状态 = S::内部不一致;
        }
        out.实例.reset();
        return out;
    }
    特征概念实例结果 关联已发布特征实例(const 特征概念实例关联请求& r) {
        特征概念实例结果 out;
        out.事实截止 = r.请求头.期望事实代次;
        const auto prepared = 准备特征概念写入(r);
        if (!prepared.成功()) {
            out.状态 = prepared.状态;
            return out;
        }
        const auto written = 重放特征概念写入(prepared);
        std::visit(
            [&](const auto& request) {
                using T = std::decay_t<decltype(request)>;
                if constexpr (std::is_same_v<T, L2特征名称字段绑定请求>
                              || std::is_same_v<T, L2特征名称字段替换请求>)
                    out.原始写入材料 = request;
            },
            *prepared.完整请求);
        out.状态 = written.状态;
        out.事实截止 = written.事实截止;
        if (!written.成功())
            return out;
        try {
            const auto g = 当前截止();
            const auto h = written.事实截止;
            auto feature = 特征服务_.按实例读取特征历史事实({1, g, h, r.精确值.实例.值, 1});
            if (!feature.成功())
                throw 业务失败{历史状态(feature.状态)};
            out.实例 = 特征概念实例投影{*feature.特征, *written.名称字段, 读取概念(r.概念, g, h)};
            if (!out.成功())
                throw 业务失败{S::内部不一致};
        } catch (...) {
            out.状态 = S::已可能发布;
            out.实例.reset();
        }
        return out;
    }
    L2特征观测结果 登记独立观测(const 特征概念观测请求& r) {
        const auto prepared = 准备特征概念写入(r);
        if (!prepared.成功()) {
            L2特征观测结果 out;
            out.状态 = prepared.状态;
            out.事实截止 = prepared.原事实截止;
            return out;
        }
        return 概念服务_.登记特征概念观测(std::get<L2特征观测登记请求>(*prepared.完整请求));
    }
    L2概念治理生命周期写入结果 迁移概念生命周期(const L2概念治理生命周期迁移请求& r) {
        const auto prepared = 准备特征概念写入(r);
        if (!prepared.成功()) {
            L2概念治理生命周期写入结果 out;
            out.结果头 = {1, 旧结构状态(prepared.状态), prepared.原事实截止, std::nullopt};
            return out;
        }
        return 概念服务_.迁移概念治理生命周期(r);
    }
    L2特征实例引用释放结果 释放实例当前概念引用(const L2特征实例引用释放请求& r) {
        return 概念服务_.释放特征实例当前引用(r);
    }
    特征概念细分结果 生成值域细分候选(const 特征概念细分请求& r) const {
        特征概念细分结果 out;
        const auto g = r.请求头.期望事实代次;
        out.事实截止 = g;
        try {
            if (r.合同版本 != 1 || r.请求头.合同版本 != 1 || !g || !r.最大概念数 || !r.最大观测数
                || !特征概念形成规则有效(r.规则))
                return out;
            const auto concepts = 概念服务_.按宿主和类型读取全部特征概念(
                {1, r.请求头, r.宿主, r.特征类型, r.规则, r.最大概念数});
            if (!concepts.成功())
                throw 业务失败{concepts.状态};
            if (concepts.宿主 != r.宿主 || concepts.特征类型 != r.特征类型 || concepts.已验证规则 != r.规则
                || concepts.事实截止 != g)
                throw 业务失败{S::内部不一致};
            std::vector<std::pair<L2特征观测事实, 特征概念材料>> observations;
            for (const auto& definition : concepts.概念组) {
                const auto rows = 概念服务_.读取特征概念观测(
                    {1, r.请求头, definition.普通概念.概念, L2读取类别::当前, 0, r.最大观测数});
                if (!rows.成功())
                    throw 业务失败{rows.状态};
                for (const auto& obs : rows.观测组) {
                    if (obs.宿主 != r.宿主 || obs.特征类型 != r.特征类型)
                        throw 业务失败{S::内部不一致};
                    bool duplicate = false;
                    for (const auto& previous : observations) {
                        if (previous.first.观测稳定编码 == obs.观测稳定编码) {
                            if (previous.first != obs)
                                throw 业务失败{S::幂等冲突};
                            duplicate = true;
                        } else if (previous.first.来源材料 == obs.来源材料
                                   && previous.first.观测时间 == obs.观测时间)
                            throw 业务失败{S::引用冲突};
                    }
                    if (duplicate)
                        continue;
                    if (observations.size() == r.最大观测数)
                        throw 业务失败{S::数量预算不足};
                    const auto f =
                        特征服务_.读取特征历史事实({1, g, obs.Gobs, obs.精确值.实例.值, obs.精确值.值事实});
                    if (!f.成功())
                        throw 业务失败{历史状态(f.状态)};
                    observations.emplace_back(obs, 解析特征值(*f.特征, g, obs.Gobs));
                }
            }
            std::sort(observations.begin(), observations.end(), [](const auto& a, const auto& b) {
                if (a.first.观测时间.纳秒 != b.first.观测时间.纳秒)
                    return a.first.观测时间.纳秒 < b.first.观测时间.纳秒;
                return a.first.观测稳定编码 < b.first.观测稳定编码;
            });
            const auto fill = [&](特征概念值域 domain) {
                特征概念细分候选 candidate;
                candidate.值域 = std::move(domain);
                for (const auto& [obs, value] : observations) {
                    if (!包含(candidate.值域, value))
                        continue;
                    if (candidate.观测身份组.empty())
                        candidate.首次时间 = obs.观测时间;
                    candidate.末次时间 = obs.观测时间;
                    candidate.观测身份组.push_back(obs.记录节点);
                }
                candidate.独立观测数 = candidate.观测身份组.size();
                if (candidate.独立观测数)
                    out.候选.push_back(std::move(candidate));
            };
            if (!r.规则.分区.empty()) {
                for (const auto& [obs, value] : observations)
                    if (!std::holds_alternative<std::int64_t>(value))
                        throw 业务失败{S::规则不支持};
                for (const auto interval : r.规则.分区)
                    fill(区间域(interval));
            } else {
                std::vector<特征概念材料> unique;
                for (const auto& [obs, value] : observations)
                    unique.push_back(value);
                std::sort(unique.begin(), unique.end());
                unique.erase(std::unique(unique.begin(), unique.end()), unique.end());
                for (const auto& value : unique)
                    fill(单点(value));
            }
            for (std::size_t i = 0; i < out.候选.size(); ++i)
                for (std::size_t j = 0; j < i; ++j)
                    for (const auto id : out.候选[i].观测身份组)
                        if (std::find(out.候选[j].观测身份组.begin(), out.候选[j].观测身份组.end(), id)
                            != out.候选[j].观测身份组.end())
                            out.候选[i].存在重叠 = out.候选[j].存在重叠 = true;
            if (当前截止() != g)
                throw 业务失败{S::事实代次漂移};
            out.状态 = S::已读取;
            return out;
        } catch (const 业务失败& e) {
            out.状态 = e.状态;
        } catch (const std::bad_alloc&) {
            out.状态 = S::资源失败;
        } catch (...) {
            out.状态 = S::内部不一致;
        }
        out.候选.clear();
        return out;
    }

  private:
    struct 业务失败 {
        S 状态;
    };
    L2概念结构服务& 概念服务_;
    const 特征类数据服务& 特征服务_;
    const 存在类数据服务& 存在服务_;
    const 特征值类数据服务& 值服务_;

    static bool 包含(const 特征概念值域& d, const 特征概念材料& v) {
        if (!特征概念值域有效(d))
            return false;
        if (d.精确值)
            return *d.精确值 == v;
        auto i = std::get_if<std::int64_t>(&v);
        return i && *i >= d.区间->下界 && *i <= d.区间->上界;
    }
    static 特征概念值域 单点(const 特征概念材料& v) {
        return {特征概念值域类别::精确值, v, std::nullopt};
    }
    static 特征概念值域 区间域(特征概念I64区间 x) {
        if (x.下界 == x.上界)
            return 单点(特征概念材料{x.下界});
        return {特征概念值域类别::I64闭区间, std::nullopt, x};
    }
    static S 历史状态(特征引用读取状态 s) noexcept {
        switch (s) {
        case 特征引用读取状态::入口拒绝:
            return S::入口拒绝;
        case 特征引用读取状态::未找到:
            return S::未找到;
        case 特征引用读取状态::目标已退出:
            return S::目标已退出;
        case 特征引用读取状态::事实代次漂移:
            return S::事实代次漂移;
        case 特征引用读取状态::引用冲突:
            return S::引用冲突;
        case 特征引用读取状态::历史材料不可用:
            return S::历史材料不可用;
        case 特征引用读取状态::资源失败:
            return S::资源失败;
        case 特征引用读取状态::数量预算不足:
            return S::数量预算不足;
        default:
            return S::内部不一致;
        }
    }
};

} // namespace 海中鱼巣
