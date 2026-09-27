#include "数据服务.特征类.h"

#include <algorithm>
#include <bit>
#include <limits>
#include <utility>

namespace 海中鱼巣 {
namespace {

using E = 特征数据错误;
constexpr L1所有者范围写入幂等身份 I64结构键{0x4645'4154'4936'3442ULL};
constexpr L1所有者范围写入幂等身份 R结构键{0x4654'554C'4553'0001ULL};
constexpr L1所有者范围写入幂等身份 来源结构键{0x4654'534F'5552'4345ULL};

void 要求(bool v, E e = E::内部不一致) { if (!v) throw e; }

E 映射(L1所有者范围读取状态 s) noexcept {
    if (s == L1所有者范围读取状态::未找到) return E::未找到;
    if (s == L1所有者范围读取状态::入口拒绝) return E::入口拒绝;
    if (s == L1所有者范围读取状态::资源失败) return E::资源失败;
    return E::内部不一致;
}
E 映射(L1所有者范围写入状态 s) noexcept {
    if (s == L1所有者范围写入状态::未找到) return E::未找到;
    if (s == L1所有者范围写入状态::入口拒绝) return E::入口拒绝;
    if (s == L1所有者范围写入状态::引用冲突) return E::引用冲突;
    if (s == L1所有者范围写入状态::幂等冲突) return E::幂等冲突;
    if (s == L1所有者范围写入状态::资源失败) return E::资源失败;
    return E::内部不一致;
}

template<class T, class F>
特征数据结果<T> 保护(F&& f) {
    try { return std::forward<F>(f)(); }
    catch (E e) { return e; }
    catch (const std::bad_alloc&) { return E::资源失败; }
    catch (const std::length_error&) { return E::资源失败; }
    catch (...) { return E::内部不一致; }
}

L1所有者范围写集本地键 新键(const L1所有者范围写集请求& w) {
    std::uint32_t n = 0;
    for (const auto& x : w.节点) n = std::max(n, x.本地键.值);
    for (const auto& x : w.关系) n = std::max(n, x.本地键.值);
    for (const auto& x : w.值) n = std::max(n, x.本地键.值);
    要求(n != std::numeric_limits<std::uint32_t>::max(), E::算术不可表示);
    return {n + 1};
}
L1所有者范围写集本地键 加节点(L1所有者范围写集请求& w,
    std::optional<L1所有者范围值表示种类> r = std::nullopt) {
    const auto k = 新键(w);
    w.节点.push_back({k, r ? 节点种类::属性类型 : 节点种类::普通, r});
    return k;
}
L1所有者范围写集本地键 加关系(L1所有者范围写集请求& w,
    L1所有者范围事实引用 a, L1所有者范围事实引用 b,
    L1所有者范围事实引用 t, std::int64_t role = 1) {
    const auto k = 新键(w);
    w.关系.push_back({k, std::move(a), std::move(b), std::move(t), role});
    return k;
}
L1所有者范围写集本地键 加值(L1所有者范围写集请求& w,
    L1所有者范围事实引用 n, L1所有者范围事实引用 t,
    L1所有者范围原始值材料 v, L1所有者范围事实引用 source) {
    const auto k = 新键(w);
    w.值.push_back({k, n, t, std::move(v), std::move(source)});
    w.属性槽变更.push_back({std::move(n), std::move(t), k});
    return k;
}
std::optional<稳定编码> 编码(const L1所有者范围写入结果& r,
    L1所有者范围写集本地键 k) noexcept {
    std::optional<稳定编码> out;
    for (const auto& x : r.新编码映射) if (x.first == k) {
        if (out || !有效(x.second)) return std::nullopt;
        out = x.second;
    }
    return out;
}
bool 写成功(L1所有者范围写入状态 s) noexcept {
    return s == L1所有者范围写入状态::成功
        || s == L1所有者范围写入状态::精确重复;
}
L1所有者范围写集请求 结构写集(L1所有者范围写入幂等身份 id,
    const std::vector<std::optional<L1所有者范围值表示种类>>& kinds) {
    L1所有者范围写集请求 w; w.写入幂等身份 = id;
    for (auto k : kinds) (void)加节点(w, k);
    return w;
}
L1所有者范围写入结果 建立结构(L1所有者范围写端口& p,
    const L1所有者范围写集请求& expected) {
    const auto first = p.读取首次写入材料({expected.写入幂等身份});
    if (first.状态 == L1所有者范围读取状态::成功) {
        要求(first.所有者 == p.所有者身份() && first.首次规范化写集
            && first.首次写入结果, E::发布结果未确认);
        要求(*first.首次规范化写集 == expected, E::旧格式不支持);
        return *first.首次写入结果;
    }
    if (first.状态 != L1所有者范围读取状态::未找到) throw 映射(first.状态);
    auto r = p.提交所有者范围中性写集(expected);
    if (!写成功(r.状态)) throw 映射(r.状态);
    return r;
}
template<std::size_t N>
void 接受结构(const L1所有者范围写入结果& r, std::array<稳定编码, N>& out) {
    要求(写成功(r.状态) && r.是否形成内存权威发布
        && r.新编码映射.size() == N, E::发布结果未确认);
    for (std::size_t i = 0; i < N; ++i) {
        const auto id = 编码(r, {static_cast<std::uint32_t>(i + 1)});
        要求(id.has_value()); out[i] = *id;
    }
}

特征规范I64域 规范域(特征规范I64域 d) {
    要求(!d.区间.empty(), E::入口拒绝);
    for (auto x : d.区间) 要求(浅层结构有效(x), E::入口拒绝);
    std::sort(d.区间.begin(), d.区间.end(), [](auto a, auto b) {
        return a.下界 != b.下界 ? a.下界 < b.下界 : a.上界 < b.上界;
    });
    std::vector<特征I64闭区间> out;
    for (auto x : d.区间) {
        if (!out.empty() && (x.下界 <= out.back().上界
            || (out.back().上界 != std::numeric_limits<std::int64_t>::max()
                && x.下界 == out.back().上界 + 1)))
            out.back().上界 = std::max(out.back().上界, x.上界);
        else out.push_back(x);
    }
    return {std::move(out)};
}
bool 包含(const 特征规范I64域& outer, const 特征规范I64域& inner) noexcept {
    std::size_t i = 0;
    for (auto x : inner.区间) {
        while (i < outer.区间.size() && outer.区间[i].上界 < x.下界) ++i;
        if (i == outer.区间.size() || outer.区间[i].下界 > x.下界
            || outer.区间[i].上界 < x.上界) return false;
    }
    return true;
}
void 检查规格(const I64基础特征类型形成规格& s) {
    要求((s.来源 == 特征类型来源::外设能够获取 || s.来源 == 特征类型来源::先天定义)
        && s.缩放分子 && s.缩放分母 && std::gcd(s.缩放分子, s.缩放分母) == 1
        && !s.允许集合.empty(), E::入口拒绝);
    if (s.来源 == 特征类型来源::外设能够获取)
        要求(s.外设提供者 && 有效(*s.外设提供者), E::入口拒绝);
    else 要求(!s.外设提供者, E::入口拒绝);
    if (s.单位绑定 == I64基础特征单位绑定::既有稳定单位)
        要求(s.既有单位 && 有效(*s.既有单位), E::入口拒绝);
    else 要求(s.单位绑定 == I64基础特征单位绑定::新FT自身 && !s.既有单位, E::入口拒绝);
    for (auto x : s.允许集合) 要求(浅层结构有效(x), E::入口拒绝);
    if (s.域形成) 要求(s.域形成->允许误差 >= 0 && 有效(s.域形成->参数来源), E::入口拒绝);
}
template<class T>
const L1所有者范围一致属性值读取结果项* 属性(
    const T& r, 稳定编码 n, 稳定编码 t) noexcept {
    const auto i = std::find_if(r.属性值.begin(), r.属性值.end(),
        [&](const auto& x) { return x.节点 == n && x.属性类型 == t; });
    return i == r.属性值.end() ? nullptr : &*i;
}
template<class T>
const L1所有者范围一致源关系组读取结果项* 源关系(
    const T& r, 稳定编码 n, 稳定编码 t) noexcept {
    const auto i = std::find_if(r.源关系组.begin(), r.源关系组.end(),
        [&](const auto& x) { return x.源节点 == n && x.关系类型节点 == t; });
    return i == r.源关系组.end() ? nullptr : &*i;
}
std::int64_t 整数(const 准确特征读取事实& f) {
    if (const auto* x = std::get_if<std::int64_t>(&f.完整值)) return *x;
    const auto* x = std::get_if<std::int64_t>(
        &std::get<特征值信息>(f.完整值).值内容);
    要求(x != nullptr, E::能力未提供); return *x;
}
特征R区间材料 单点(std::int64_t v) {
    return {1, 特征R材料类别::I64闭区间,
        {std::bit_cast<std::uint64_t>(v), std::bit_cast<std::uint64_t>(v)}};
}
bool 解单点(const 特征R区间材料& m, std::int64_t& v) noexcept {
    if (m.格式版本 != 1 || m.类别 != 特征R材料类别::I64闭区间
        || m.规范化U64组.size() != 2) return false;
    const auto a = std::bit_cast<std::int64_t>(m.规范化U64组[0]);
    const auto b = std::bit_cast<std::int64_t>(m.规范化U64组[1]);
    if (a != b) return false; v = a; return true;
}

} // namespace

特征类数据服务::特征类数据服务(const L1事实基座服务& l1,
    L1所有者范围写端口&& definitions, L1所有者范围写端口&& information,
    const 特征值类数据服务& values, 稳定编码 producer)
    : l1_(l1), definitions_(std::move(definitions)), information_(std::move(information)),
      values_(values), producer_(producer) {
    if (!绑定于(l1) || !有效(producer_)) throw std::invalid_argument("特征数据端口绑定");
}
bool 特征类数据服务::绑定于(const L1事实基座服务& l1) const noexcept {
    return &l1 == &l1_ && definitions_.有效() && information_.有效()
        && definitions_.绑定于(l1) && information_.绑定于(l1) && values_.绑定于(l1)
        && definitions_.所有者身份() != information_.所有者身份();
}
bool 特征类数据服务::与特征服务同底座(const 特征类数据服务& x) const noexcept {
    return 绑定于(l1_) && x.绑定于(l1_);
}

特征数据结果<std::monostate> 特征类数据服务::初始化特征定义结构() {
    return 保护<std::monostate>([&] {
        std::lock_guard<std::mutex> lock(mutex_);
        接受结构(建立结构(definitions_, 结构写集({1},
            {{}, {}, L1所有者范围值表示种类::U64组, L1所有者范围值表示种类::I64,
             {}, {}, {}, {}, L1所有者范围值表示种类::I64,
             L1所有者范围值表示种类::U64组, {}, {}, {},
             L1所有者范围值表示种类::U64组, L1所有者范围值表示种类::I64组, {}})), d_);
        接受结构(建立结构(definitions_, 结构写集(I64结构键,
            {{}, {}, L1所有者范围值表示种类::U64组,
             L1所有者范围值表示种类::I64组})), k_);
        接受结构(建立结构(definitions_, 结构写集(R结构键,
            {{}, L1所有者范围值表示种类::I64,
             L1所有者范围值表示种类::U64组})), r_);
        接受结构(建立结构(definitions_, 结构写集(来源结构键,
            {L1所有者范围值表示种类::I64})), source_);
        definition_ready_ = true; return std::monostate{};
    });
}
特征数据结果<std::monostate> 特征类数据服务::初始化准确特征结构() {
    return 保护<std::monostate>([&] {
        std::lock_guard<std::mutex> lock(mutex_);
        要求(definition_ready_, E::未设置);
        接受结构(建立结构(information_, 结构写集({1},
            {{}, {}, L1所有者范围值表示种类::I64,
             L1所有者范围值表示种类::I64, {}})), f_);
        information_ready_ = true; return std::monostate{};
    });
}
特征数据结果<std::monostate> 特征类数据服务::收敛待确认写入() {
    return std::monostate{};
}

特征数据结果<I64基础特征类型信息> 特征类数据服务::读取I64基础特征类型(
    特征类型身份 id) const {
    return 保护<I64基础特征类型信息>([&] {
        std::lock_guard<std::mutex> lock(mutex_);
        要求(definition_ready_ && 有效(id), E::入口拒绝);
        L1所有者范围一致关系类型闭包读取请求 q;
        q.所有者 = {definitions_.所有者身份()}; q.节点 = {id.编码};
        q.属性值 = {{id.编码, d_[类型规格属性]}, {id.编码, source_[类型来源属性]}};
        q.源关系组 = {{id.编码, d_[定义归属]}, {id.编码, d_[外设来源关系]},
            {id.编码, d_[单位关系]}, {id.编码, d_[域规则关系]}};
        L1所有者范围一致关系类型闭包选择项 ruleSelection;
        ruleSelection.入口关系类型节点 = d_[域规则关系];
        ruleSelection.目标节点属性类型 = {d_[规则误差属性]};
        ruleSelection.目标节点源关系类型 = {d_[参数来源关系]};
        q.关系类型闭包.push_back(std::move(ruleSelection));
        const auto r = l1_.尝试读取所有者范围一致关系类型闭包投影(q);
        要求(r.状态 == L1所有者范围一致当前读取状态::成功, E::资源失败);
        const auto n = std::find_if(r.节点.begin(), r.节点.end(),
            [&](const auto& x) { return x.查询编码 == id.编码; });
        要求(n != r.节点.end()); if (n->状态 == L1所有者范围一致当前读取项目状态::未找到) throw E::未找到;
        要求(n->状态 == L1所有者范围一致当前读取项目状态::成功 && n->事实
            && n->事实->写入所有者 == definitions_.所有者身份()
            && n->事实->种类 == 节点种类::属性类型
            && n->事实->属性类型表示 == L1所有者范围值表示种类::I64, E::类型不相容);
        const auto owner = 源关系(r, id.编码, d_[定义归属]);
        const auto unit = 源关系(r, id.编码, d_[单位关系]);
        const auto device = 源关系(r, id.编码, d_[外设来源关系]);
        const auto rule = 源关系(r, id.编码, d_[域规则关系]);
        要求(owner && owner->成员.size() == 1
            && owner->成员.front().关系.目标节点 == d_[定义锚点]
            && unit && unit->成员.size() == 1, E::旧格式不支持);
        const auto spec = 属性(r, id.编码, d_[类型规格属性]);
        const auto source = 属性(r, id.编码, source_[类型来源属性]);
        要求(spec && spec->状态 == L1所有者范围一致当前读取项目状态::成功 && spec->投影
            && source && source->状态 == L1所有者范围一致当前读取项目状态::成功
            && source->投影, E::旧格式不支持);
        const auto* bytes = std::get_if<std::vector<std::uint64_t>>(&spec->投影->当前值事实.材料);
        const auto* kind = std::get_if<std::int64_t>(&source->投影->当前值事实.材料);
        要求(bytes && kind && bytes->size() >= 5 && (*bytes)[2]
            && bytes->size() == 3 + 2 * (*bytes)[2], E::旧格式不支持);
        I64基础特征类型信息 out; out.身份 = id;
        out.规格.来源 = static_cast<特征类型来源>(*kind);
        out.规格.单位 = unit->成员.front().关系.目标节点;
        out.规格.缩放分子 = (*bytes)[0]; out.规格.缩放分母 = (*bytes)[1];
        for (std::size_t i = 3; i < bytes->size(); i += 2)
            out.规格.允许集合.push_back({std::bit_cast<std::int64_t>((*bytes)[i]),
                std::bit_cast<std::int64_t>((*bytes)[i + 1])});
        if (out.规格.来源 == 特征类型来源::外设能够获取) {
            要求(device && device->成员.size() == 1, E::引用冲突);
            out.规格.外设提供者 = device->成员.front().关系.目标节点;
        } else 要求(!device || device->成员.empty(), E::引用冲突);
        if (rule && !rule->成员.empty()) {
            要求(rule->成员.size() == 1, E::引用冲突);
            const auto closure = std::find_if(r.关系类型闭包.begin(), r.关系类型闭包.end(),
                [&](const auto& x) { return x.入口关系类型节点 == d_[域规则关系]; });
            要求(closure != r.关系类型闭包.end()
                && closure->状态 == L1所有者范围一致当前读取项目状态::成功
                && closure->成员.size() == 1, E::内部不一致);
            const auto& member = closure->成员.front();
            要求(member.关系.源节点 == id.编码
                && member.关系.目标节点 == rule->成员.front().关系.目标节点
                && member.目标节点属性值.size() == 1
                && member.目标节点源关系组.size() == 1, E::内部不一致);
            const auto& errorItem = member.目标节点属性值.front();
            const auto& parameterItem = member.目标节点源关系组.front();
            要求(errorItem.状态 == L1所有者范围一致当前读取项目状态::成功
                && errorItem.投影 && parameterItem.状态 == L1所有者范围一致当前读取项目状态::成功
                && parameterItem.成员.size() == 1, E::旧格式不支持);
            const auto* error = std::get_if<std::int64_t>(&errorItem.投影->当前值事实.材料);
            要求(error && *error >= 0, E::旧格式不支持);
            out.规则 = 特征比较规则身份{member.目标节点.编码};
            out.规格.域形成 = I64特征域形成参数{
                *error, parameterItem.成员.front().关系.目标节点};
        }
        (void)规范域({out.规格.允许集合}); return out;
    });
}

I64基础特征类型定义结果 特征类数据服务::形成或读取I64基础特征类型(
    const I64基础特征类型定义请求& q) noexcept {
    I64基础特征类型定义结果 out; out.原请求 = q;
    try {
        if (!definition_ready_ || !有效(q.幂等身份)) return out;
        检查规格(q.规格);
        L1所有者范围写集请求 w; w.写入幂等身份 = q.幂等身份;
        const auto ft = 加节点(w, L1所有者范围值表示种类::I64);
        (void)加关系(w, ft, d_[定义锚点], d_[定义归属]);
        if (q.规格.外设提供者) (void)加关系(w, ft, *q.规格.外设提供者, d_[外设来源关系]);
        const L1所有者范围事实引用 unit = q.规格.单位绑定 == I64基础特征单位绑定::新FT自身
            ? L1所有者范围事实引用{ft} : L1所有者范围事实引用{*q.规格.既有单位};
        (void)加关系(w, ft, unit, d_[单位关系]);
        std::vector<std::uint64_t> data{q.规格.缩放分子, q.规格.缩放分母, q.规格.允许集合.size()};
        for (auto x : q.规格.允许集合) {
            data.push_back(std::bit_cast<std::uint64_t>(x.下界));
            data.push_back(std::bit_cast<std::uint64_t>(x.上界));
        }
        (void)加值(w, ft, d_[类型规格属性], std::move(data), producer_);
        (void)加值(w, ft, source_[类型来源属性], static_cast<std::int64_t>(q.规格.来源), producer_);
        if (q.规格.域形成) {
            const auto r = 加节点(w); (void)加关系(w, ft, r, d_[域规则关系]);
            (void)加关系(w, r, q.规格.域形成->参数来源, d_[参数来源关系]);
            (void)加值(w, r, d_[规则误差属性], q.规格.域形成->允许误差, producer_);
        }
        const auto addR = [&](std::int64_t use) {
            const auto r = 加节点(w); (void)加关系(w, r, d_[定义锚点], d_[定义归属]);
            (void)加关系(w, ft, r, r_[R规则归属关系], use);
            (void)加值(w, r, r_[R规则版本属性], std::int64_t{1}, producer_);
            (void)加值(w, r, r_[R规则参数属性], std::vector<std::uint64_t>{1}, producer_);
        };
        addR(1); addR(2); if (q.规格.域形成) addR(3);
        bool restored = false; L1所有者范围写入结果 receipt;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            const auto first = definitions_.读取首次写入材料({q.幂等身份});
            if (first.状态 == L1所有者范围读取状态::成功) {
                if (!first.首次规范化写集 || !first.首次写入结果 || *first.首次规范化写集 != w) {
                    out.状态 = I64基础特征类型定义状态::幂等冲突; return out;
                }
                receipt = *first.首次写入结果; restored = true;
            } else if (first.状态 == L1所有者范围读取状态::未找到)
                receipt = definitions_.提交所有者范围中性写集(w);
            else {
                out.状态 = first.状态 == L1所有者范围读取状态::资源失败
                    ? I64基础特征类型定义状态::资源失败 : I64基础特征类型定义状态::已可能发布;
                return out;
            }
        }
        if (!写成功(receipt.状态) || !receipt.是否形成内存权威发布) {
            out.状态 = receipt.状态 == L1所有者范围写入状态::幂等冲突
                ? I64基础特征类型定义状态::幂等冲突
                : receipt.状态 == L1所有者范围写入状态::资源失败
                    ? I64基础特征类型定义状态::资源失败 : I64基础特征类型定义状态::已可能发布;
            return out;
        }
        const auto id = 编码(receipt, ft); 要求(id.has_value(), E::发布结果未确认);
        out.首次写入回执 = receipt;
        auto read = 读取I64基础特征类型({*id});
        if (const auto* e = std::get_if<E>(&read)) throw *e;
        out.事实 = 特征截止事实<I64基础特征类型信息>{
            std::get<I64基础特征类型信息>(std::move(read))};
        out.状态 = restored ? I64基础特征类型定义状态::已恢复 : I64基础特征类型定义状态::已形成;
    } catch (E e) {
        out.状态 = e == E::入口拒绝 ? I64基础特征类型定义状态::入口拒绝
            : e == E::幂等冲突 ? I64基础特征类型定义状态::幂等冲突
            : e == E::资源失败 ? I64基础特征类型定义状态::资源失败
            : e == E::引用冲突 || e == E::类型不相容 ? I64基础特征类型定义状态::引用冲突
            : I64基础特征类型定义状态::内部不一致;
    } catch (const std::bad_alloc&) { out.状态 = I64基础特征类型定义状态::资源失败; }
    catch (...) { out.状态 = I64基础特征类型定义状态::内部不一致; }
    return out;
}

特征数据结果<准确特征读取事实> 特征类数据服务::读取准确特征事实(
    const 准确特征读取请求& q) const {
    return 保护<准确特征读取事实>([&] {
        std::lock_guard<std::mutex> lock(mutex_);
        要求(information_ready_ && 有效(q.身份), E::入口拒绝);
        L1所有者范围一致当前读取请求 request;
        request.所有者 = {information_.所有者身份()}; request.节点 = {q.身份.编码};
        request.属性值 = {{q.身份.编码, f_[准确内联属性]}, {q.身份.编码, f_[准确引用属性]}};
        request.源关系组 = {{q.身份.编码, f_[信息归属]}, {q.身份.编码, f_[准确类型关系]}};
        const auto r = l1_.尝试读取所有者范围一致当前投影(request);
        要求(r.状态 == L1所有者范围一致当前读取状态::成功, E::资源失败);
        const auto n = std::find_if(r.节点.begin(), r.节点.end(),
            [&](const auto& x) { return x.查询编码 == q.身份.编码; });
        要求(n != r.节点.end()); if (n->状态 == L1所有者范围一致当前读取项目状态::未找到) throw E::未找到;
        要求(n->状态 == L1所有者范围一致当前读取项目状态::成功 && n->事实
            && n->事实->写入所有者 == information_.所有者身份()
            && n->事实->种类 == 节点种类::普通, E::旧格式不支持);
        const auto owner = 源关系(r, q.身份.编码, f_[信息归属]);
        const auto type = 源关系(r, q.身份.编码, f_[准确类型关系]);
        要求(owner && owner->成员.size() == 1 && owner->成员.front().关系.目标节点 == f_[信息锚点]
            && type && type->成员.size() == 1, E::旧格式不支持);
        const auto direct = 属性(r, q.身份.编码, f_[准确内联属性]);
        const auto ref = 属性(r, q.身份.编码, f_[准确引用属性]);
        const bool a = direct && direct->状态 == L1所有者范围一致当前读取项目状态::成功;
        const bool b = ref && ref->状态 == L1所有者范围一致当前读取项目状态::成功;
        要求(a != b, E::旧格式不支持);
        const auto& p = *(a ? direct->投影 : ref->投影);
        const auto* scalar = std::get_if<std::int64_t>(&p.当前值事实.材料);
        要求(scalar, E::旧格式不支持);
        准确特征读取事实 out;
        out.信息 = {q.身份, {type->成员.front().关系.目标节点}, *scalar};
        out.类型关系 = type->成员.front().关系.编码; out.准确值事实 = p.当前值事实.编码;
        if (a) out.完整值 = *scalar;
        else {
            const 特征值身份 valueId{p.当前值事实.编码};
            const auto value = values_.获取特征值(valueId);
            const auto* full = std::get_if<特征值信息>(&value);
            要求(full, E::能力未提供);
            const auto* fullScalar = std::get_if<std::int64_t>(&full->值内容);
            要求(fullScalar && *fullScalar == *scalar);
            out.信息.准确值 = valueId; out.完整值 = *full;
        }
        return out;
    });
}
特征数据结果<特征信息> 特征类数据服务::读取准确特征(特征信息身份 id) const {
    auto r = 读取准确特征事实({id}); if (const auto* e = std::get_if<S>(&r)) return *e;
    return std::get<准确特征读取事实>(std::move(r)).信息;
}
特征数据结果<特征类型身份> 特征类数据服务::读取准确特征类型(特征信息身份 id) const {
    auto r = 读取准确特征(id); if (const auto* e = std::get_if<S>(&r)) return *e;
    return std::get<特征信息>(r).类型;
}
特征数据结果<特征准确值> 特征类数据服务::读取准确特征值(特征信息身份 id) const {
    auto r = 读取准确特征(id); if (const auto* e = std::get_if<S>(&r)) return *e;
    return std::get<特征信息>(std::move(r)).准确值;
}
特征数据结果<std::vector<特征信息>> 特征类数据服务::查询准确特征(
    特征类型身份 type, const 特征准确值& value) const {
    return 保护<std::vector<特征信息>>([&] {
        要求(information_ready_ && 有效(type) && 浅层结构有效(value), E::入口拒绝);
        const auto edges = l1_.读取所有者范围当前目标关系组({type.编码, f_[准确类型关系]});
        if (edges.状态 != L1所有者范围读取状态::成功) throw 映射(edges.状态);
        std::vector<特征信息> out;
        for (const auto& edge : edges.关系组) {
            auto one = 读取准确特征({edge.源节点});
            if (const auto* e = std::get_if<S>(&one)) throw *e;
            auto info = std::get<特征信息>(std::move(one));
            if (info.类型 == type && info.准确值 == value) out.push_back(std::move(info));
        }
        std::sort(out.begin(), out.end(), [](const auto& a, const auto& b) { return a.身份.编码 < b.身份.编码; });
        return out;
    });
}
特征数据结果<std::monostate> 特征类数据服务::删除准确特征(特征信息身份) {
    return S::能力未提供;
}
有界准确特征读取结果 特征类数据服务::读取有界准确特征事实(
    const 有界准确特征读取请求& q) const noexcept {
    有界准确特征读取结果 out; out.原请求 = q;
    try {
        const auto r = 读取准确特征事实({q.身份});
        if (const auto* e = std::get_if<S>(&r)) {
            out.状态 = *e == S::未找到 ? 特征类标量状态::未找到
                : *e == S::资源失败 ? 特征类标量状态::资源失败
                : *e == S::能力未提供 ? 特征类标量状态::算法不支持
                : *e == S::入口拒绝 ? 特征类标量状态::入口拒绝 : 特征类标量状态::内部不一致;
        } else { out.事实 = std::get<准确特征读取事实>(r); out.状态 = 特征类标量状态::已读取; }
    } catch (...) { out.状态 = 特征类标量状态::内部不一致; }
    return out;
}

特征类型准确值核验结果 特征类数据服务::核验正式特征类型准确值(
    const 特征类型准确值核验请求& q) const {
    特征类型准确值核验结果 out;
    try {
        if (!有效(q.正式特征类型) || !浅层结构有效(q.准确值)) return out;
        const auto t = 读取I64基础特征类型(q.正式特征类型);
        if (const auto* e = std::get_if<S>(&t)) {
            out.状态 = *e == S::未找到 ? 特征类型准确值核验状态::正式特征类型未找到
                : *e == S::资源失败 ? 特征类型准确值核验状态::资源失败
                : 特征类型准确值核验状态::内部不一致; return out;
        }
        std::int64_t v{};
        if (const auto* p = std::get_if<std::int64_t>(&q.准确值)) v = *p;
        else {
            const auto x = values_.获取特征值(std::get<特征值身份>(q.准确值));
            const auto* info = std::get_if<特征值信息>(&x);
            if (!info) { out.状态 = 特征类型准确值核验状态::准确值未找到; return out; }
            const auto* stored = std::get_if<std::int64_t>(&info->值内容);
            if (!stored) { out.状态 = 特征类型准确值核验状态::准确值不相容; return out; }
            v = *stored;
        }
        const auto& spec = std::get<I64基础特征类型信息>(t).规格;
        if (!包含(规范域({spec.允许集合}), {{{v, v}}})) {
            out.状态 = 特征类型准确值核验状态::准确值不相容; return out;
        }
        out.事实 = 特征类型准确值核验事实{q.正式特征类型, q.准确值};
        out.状态 = 特征类型准确值核验状态::已核验;
    } catch (const std::bad_alloc&) { out.状态 = 特征类型准确值核验状态::资源失败; }
    catch (...) { out.状态 = 特征类型准确值核验状态::内部不一致; }
    return out;
}
特征正式准确I64解析结果_v2 特征类数据服务::解析正式特征类型准确I64_v2(
    const 特征正式准确I64解析请求_v2& q) const noexcept {
    特征正式准确I64解析结果_v2 out;
    try {
        const auto checked = 核验正式特征类型准确值({q.正式特征类型, q.准确值});
        if (!checked.成功()) {
            out.状态 = checked.状态 == 特征类型准确值核验状态::正式特征类型未找到
                ? 特征正式准确I64解析状态_v2::正式特征类型未找到
                : checked.状态 == 特征类型准确值核验状态::准确值未找到
                    ? 特征正式准确I64解析状态_v2::准确值未找到
                : checked.状态 == 特征类型准确值核验状态::准确值不相容
                    ? 特征正式准确I64解析状态_v2::准确值不相容
                : checked.状态 == 特征类型准确值核验状态::资源失败
                    ? 特征正式准确I64解析状态_v2::资源失败
                    : 特征正式准确I64解析状态_v2::内部不一致;
            return out;
        }
        std::int64_t v{};
        if (const auto* p = std::get_if<std::int64_t>(&q.准确值)) v = *p;
        else {
            const auto x = values_.获取特征值(std::get<特征值身份>(q.准确值));
            const auto* info = std::get_if<特征值信息>(&x);
            const auto* stored = info ? std::get_if<std::int64_t>(&info->值内容) : nullptr;
            if (!stored) { out.状态 = 特征正式准确I64解析状态_v2::非I64; return out; } v = *stored;
        }
        out.事实 = 特征正式准确I64解析事实_v2{q.正式特征类型, q.准确值, v};
        out.状态 = 特征正式准确I64解析状态_v2::已解析;
    } catch (...) { out.状态 = 特征正式准确I64解析状态_v2::内部不一致; }
    return out;
}

特征数据结果<特征截止事实<I64基础特征类型信息>>
特征类数据服务::读取I64基础特征类型事实(const 特征类型截止请求& q) const {
    auto r = 读取I64基础特征类型(q.类型); if (const auto* e = std::get_if<S>(&r)) return *e;
    return 特征截止事实<I64基础特征类型信息>{std::get<I64基础特征类型信息>(std::move(r))};
}
特征数据结果<特征截止事实<特征规范I64域>>
特征类数据服务::读取I64类型完整域(const 特征类型截止请求& q) const {
    auto r = 读取I64基础特征类型(q.类型); if (const auto* e = std::get_if<S>(&r)) return *e;
    return 特征截止事实<特征规范I64域>{规范域({std::get<I64基础特征类型信息>(r).规格.允许集合})};
}
特征数据结果<特征域形成事实> 特征类数据服务::形成I64特征域(
    const 准确特征读取请求& q) const {
    auto fr = 读取准确特征事实(q); if (const auto* e = std::get_if<S>(&fr)) return *e;
    const auto& f = std::get<准确特征读取事实>(fr);
    auto tr = 读取I64基础特征类型(f.信息.类型); if (const auto* e = std::get_if<S>(&tr)) return *e;
    const auto& t = std::get<I64基础特征类型信息>(tr);
    if (!t.规格.域形成 || !t.规则) return S::规则缺失;
    const auto v = 整数(f), d = t.规格.域形成->允许误差;
    if ((d > 0 && v < std::numeric_limits<std::int64_t>::min() + d)
        || (d > 0 && v > std::numeric_limits<std::int64_t>::max() - d)) return S::算术不可表示;
    return 特征域形成事实{t.身份, {{{v - d, v + d}}}, q.身份, *t.规则};
}
特征数据结果<特征截止事实<特征规范I64域>>
特征类数据服务::规范化I64特征域(const 特征I64域判定请求& q) const {
    auto full = 读取I64类型完整域(q.类型); if (const auto* e = std::get_if<S>(&full)) return *e;
    auto d = 规范域(q.域);
    if (!包含(std::get<特征截止事实<特征规范I64域>>(full).数据, d)) return S::类型不相容;
    return 特征截止事实<特征规范I64域>{std::move(d)};
}
特征数据结果<特征截止事实<bool>> 特征类数据服务::判定准确特征命中域(
    const 准确特征域命中请求& q) const {
    auto fr = 读取准确特征事实(q.特征); if (const auto* e = std::get_if<S>(&fr)) return *e;
    const auto& f = std::get<准确特征读取事实>(fr);
    auto d = 规范化I64特征域({{f.信息.类型}, q.域}); if (const auto* e = std::get_if<S>(&d)) return *e;
    const auto v = 整数(f);
    return 特征截止事实<bool>{包含(std::get<特征截止事实<特征规范I64域>>(d).数据, {{{v, v}}})};
}
特征数据结果<特征截止事实<bool>> 特征类数据服务::判定I64域包含(
    const 特征I64域包含请求& q) const {
    auto a = 规范化I64特征域({q.类型, q.外}); if (const auto* e = std::get_if<S>(&a)) return *e;
    auto b = 规范化I64特征域({q.类型, q.内}); if (const auto* e = std::get_if<S>(&b)) return *e;
    return 特征截止事实<bool>{包含(std::get<特征截止事实<特征规范I64域>>(a).数据,
        std::get<特征截止事实<特征规范I64域>>(b).数据)};
}

特征R归组规则结果 特征类数据服务::归组特征R(const 特征R归组规则请求& q) const noexcept {
    特征R归组规则结果 out;
    try {
        const auto x = 解析正式特征类型准确I64_v2({q.FT, q.候选值});
        if (!x.成功({q.FT, q.候选值})) {
            out.状态 = x.状态 == 特征正式准确I64解析状态_v2::资源失败
                ? 特征R规则状态::资源失败 : 特征R规则状态::候选值不可读; return out;
        }
        const auto material = 单点(x.事实->I64);
        for (const auto& item : q.当前R项) if (item.材料 == material) {
            if (out.命中R || !有效(item.R)) { out.状态 = 特征R规则状态::内部不一致; return out; }
            out.命中R = item.R;
        }
        out.规范化材料 = material;
        out.状态 = out.命中R ? 特征R规则状态::唯一命中 : 特征R规则状态::形成新R;
    } catch (...) { out.状态 = 特征R规则状态::内部不一致; }
    return out;
}
特征R代表值结果 特征类数据服务::读取特征R代表值(const 特征R代表值请求& q) const noexcept {
    特征R代表值结果 out; std::int64_t v{};
    if (!有效(q.FT)) return out;
    if (!解单点(q.材料, v)) { out.状态 = 特征R规则状态::材料格式不支持; return out; }
    out.代表值 = 特征准确值{v}; out.状态 = 特征R规则状态::已取得代表值; return out;
}
特征R概念归并结果 特征类数据服务::归并特征R概念(const 特征R概念归并请求& q) const noexcept {
    特征R概念归并结果 out; out.R集合 = q.R集合版本.R集合; out.版本 = q.R集合版本.版本;
    if (!有效(q.FT) || !有效(out.R集合) || !有效(out.版本) || q.R集合版本.R项.empty()) return out;
    out.状态 = 特征R规则状态::已归并零输出; return out;
}
特征数据结果<补齐I64默认R规则结果> 特征类数据服务::补齐I64默认R规则(
    const 补齐I64默认R规则请求&) { return S::能力未提供; }

特征I64比较绑定结果 特征类数据服务::建立I64比较绑定(const 特征I64比较绑定建立请求& q) {
    特征I64比较绑定结果 out; out.建立原请求 = q; return out;
}
特征I64比较绑定结果 特征类数据服务::退出I64比较绑定(const 特征I64比较绑定退出请求&) {
    特征I64比较绑定结果 out; out.操作 = 特征I64比较绑定操作::退出; return out;
}
特征I64比较绑定读取结果_v2 特征类数据服务::读取I64比较绑定_v2(
    const 特征I64比较绑定读取请求_v2&) const noexcept { return {}; }
特征I64比较绑定读取结果_v2 特征类数据服务::读取当前I64比较绑定_v2(
    const 特征I64当前比较绑定读取请求_v2&) const noexcept { return {}; }

bool 特征类标量派生批量读取结果::成功() const noexcept { return false; }
特征类标量派生批量读取结果 特征类数据服务::批量读取标量派生定义(
    const 特征类标量派生批量读取请求& q) const {
    特征类标量派生批量读取结果 out; out.原请求 = q; out.状态 = 特征类标量状态::算法不支持; return out;
}
特征类标量派生读取结果 特征类数据服务::读取标量派生定义(
    const 特征类标量派生读取请求&) const {
    特征类标量派生读取结果 out; out.状态 = 特征类标量状态::算法不支持; return out;
}
特征类标量派生写结果 特征类数据服务::建立标量派生定义(
    const 特征类标量派生建立请求&) {
    特征类标量派生写结果 out; out.状态 = 特征类标量状态::算法不支持; return out;
}
特征类标量派生写结果 特征类数据服务::退出标量派生定义(
    const 特征类标量派生退出请求&) {
    特征类标量派生写结果 out; out.状态 = 特征类标量状态::算法不支持; return out;
}

const L1事实基座服务& 特征类数据服务::原子I64底座() const noexcept { return l1_; }
L1所有者范围写端口& 特征类数据服务::原子I64端口() noexcept { return information_; }
bool 特征类数据服务::原子I64结构已就绪() const noexcept { return information_ready_; }
原子I64特征候选查询结果 特征类数据服务::查询原子I64内容候选(
    const 原子I64特征候选查询请求& q) const {
    原子I64特征候选查询结果 out; out.原请求 = q;
    try {
        if (!有效(q.正式特征类型)) return out;
        const auto edges = l1_.读取所有者范围当前目标关系组({q.正式特征类型, f_[准确类型关系]});
        if (edges.状态 != L1所有者范围读取状态::成功) {
            out.状态 = edges.状态 == L1所有者范围读取状态::资源失败
                ? 原子I64特征候选查询状态::资源失败 : 原子I64特征候选查询状态::内部不一致; return out;
        }
        for (const auto& edge : edges.关系组) {
            const auto r = 读取准确特征事实({{edge.源节点}});
            const auto* f = std::get_if<准确特征读取事实>(&r);
            if (!f) throw 原子I64特征候选查询状态::内部不一致;
            if (整数(*f) == q.准确I64) out.候选.push_back({f->信息.身份.编码});
        }
        out.状态 = 原子I64特征候选查询状态::已读取;
    } catch (原子I64特征候选查询状态 s) { out.状态 = s; out.候选.clear(); }
    catch (...) { out.状态 = 原子I64特征候选查询状态::内部不一致; out.候选.clear(); }
    return out;
}
原子I64特征参与结果<L1有限N分区原子参与者写集>
特征类数据服务::准备原子I64出生片段(const 原子I64特征出生请求& q) const {
    原子I64特征参与结果<L1有限N分区原子参与者写集> out;
    try {
        if (!information_ready_ || !有效(q.正式特征类型) || !有效(q.键.内容)) return out;
        if (!核验正式特征类型准确值({q.正式特征类型, 特征准确值{q.准确I64}}).成功()) return out;
        L1有限N分区原子参与者写集 p; p.参与者 = {1}; p.所有者 = information_.所有者身份();
        p.写集.写入幂等身份 = q.键.内容; const L1所有者范围写集本地键 F{1};
        p.写集.节点.push_back({F, 节点种类::普通, {}});
        p.写集.关系.push_back({{2}, F, f_[信息锚点], f_[信息归属], 1});
        p.写集.关系.push_back({{3}, F, q.正式特征类型, f_[准确类型关系], 1});
        p.写集.值.push_back({{4}, F, f_[准确内联属性], q.准确I64, producer_});
        p.写集.属性槽变更.push_back({F, f_[准确内联属性], L1所有者范围写集本地键{4}});
        out.数据 = std::move(p); out.状态 = 原子I64特征出生状态::已创建;
    } catch (...) { out.状态 = 原子I64特征出生状态::内部不一致; }
    return out;
}
原子I64特征窄读取结果<原子I64特征内容事实>
特征类数据服务::读取原子I64内容(const 原子I64特征内容读取请求& q) const {
    原子I64特征窄读取结果<原子I64特征内容事实> out;
    try {
        const auto r = 读取准确特征事实({{q.F}});
        if (const auto* e = std::get_if<S>(&r)) {
            out.状态 = *e == S::未找到 ? 原子I64特征窄读取状态::未找到
                : *e == S::资源失败 ? 原子I64特征窄读取状态::资源失败
                : *e == S::入口拒绝 ? 原子I64特征窄读取状态::入口拒绝
                : 原子I64特征窄读取状态::内部不一致; return out;
        }
        const auto& f = std::get<准确特征读取事实>(r);
        if (!f.准确值事实) return out;
        out.事实 = 原子I64特征内容事实{q.F, f.信息.类型.编码, f.类型关系, *f.准确值事实, 整数(f)};
        out.状态 = 原子I64特征窄读取状态::已读取;
    } catch (...) { out.状态 = 原子I64特征窄读取状态::内部不一致; }
    return out;
}

} // namespace 海中鱼巣
