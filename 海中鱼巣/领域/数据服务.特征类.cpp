#include "数据服务.特征类.h"
namespace 海中鱼巣 {
特征数据错误 特征类数据服务::映射(L1所有者范围读取状态 s) {
    switch (s) {
    case L1所有者范围读取状态::未找到: return S::未找到;
    case L1所有者范围读取状态::已退出: return S::已退出;
    case L1所有者范围读取状态::入口拒绝: return S::入口拒绝;
    case L1所有者范围读取状态::事实代次漂移: return S::并发变化;
    case L1所有者范围读取状态::历史材料已清理: return S::历史材料不可用;
    case L1所有者范围读取状态::资源失败: return S::资源失败;
    default: return S::内部不一致;
    }
}
特征数据错误 特征类数据服务::映射(L1所有者范围写入状态 s) {
    switch (s) {
    case L1所有者范围写入状态::未找到: return S::未找到;
    case L1所有者范围写入状态::已退出: return S::已退出;
    case L1所有者范围写入状态::入口拒绝: return S::入口拒绝;
    case L1所有者范围写入状态::引用冲突: return S::引用冲突;
    case L1所有者范围写入状态::事实代次漂移: return S::并发变化;
    case L1所有者范围写入状态::幂等冲突: return S::幂等冲突;
    case L1所有者范围写入状态::资源失败: return S::资源失败;
    default: return S::内部不一致;
    }
}
std::uint64_t 特征类数据服务::当前G() const {
    要求(绑定于(l1_) && definitions_.有效() && information_.有效());
    auto r = l1_.读取中性当前事实代次({L1中性CRUD合同版本});
    if (r.状态 != L1中性读取状态::成功)
        throw r.状态 == L1中性读取状态::资源失败 ? S::资源失败 : S::内部不一致;
    要求(r.合同版本 == L1中性CRUD合同版本 && r.事实代次 != 0); return r.事实代次;
}
L1所有者范围事实副本 特征类数据服务::原始事实(稳定编码 id, std::uint64_t g, 读取计量* meter) const {
    要求(有效(id), S::入口拒绝);
    if (meter && meter->已读.contains(id)) return meter->已读.at(id);
    if (meter) 要求(meter->用量.材料总数<meter->上限.最大材料总数,S::数量预算不足);
    auto r = l1_.读取所有者范围历史事实({L1所有者范围CRUD合同版本, id});
    if (r.状态 != L1所有者范围读取状态::成功) throw 映射(r.状态);
    要求(r.读取事实代次 == g, S::并发变化);
    要求(r.合同版本 == L1所有者范围CRUD合同版本 && r.查询编码 == id && r.事实
        && !r.物理清理墓碑 && !r.物理清理事实代次);
    std::visit([&](const auto& f) { 要求(f.编码 == id); }, *r.事实); if (meter) meter->记(*r.事实); return *r.事实;
}
特征类数据服务::N 特征类数据服务::节点(稳定编码 id, std::uint64_t g,
    std::uint64_t h, std::optional<分区> p, 读取计量* meter) const {
    auto raw = 原始事实(id, g, meter); const auto* n = std::get_if<N>(&raw);
    要求(n != nullptr, S::类型不相容); 生命周期(*n, g, h);
    if (p) 要求(n->写入所有者 == 端口(*p).所有者身份(), S::引用冲突); return *n;
}
std::vector<特征类数据服务::E> 特征类数据服务::关系(稳定编码 id, 稳定编码 type,
    bool incoming, std::uint64_t g, std::uint64_t h, 分区 p, 读取计量* meter) const {
    const auto cacheKey=std::make_tuple(g,h,id,type,incoming);
    if(meter&&meter->已读关系组.contains(cacheKey)) {
        const auto& cached=meter->已读关系组.at(cacheKey);
        for(const auto& e:cached)要求(e.写入所有者==端口(p).所有者身份());
        return cached;
    }
    auto direction = incoming ? L1所有者范围关系端点方向::目标 : L1所有者范围关系端点方向::源;
    auto r = l1_.读取所有者范围历史关系组({L1所有者范围CRUD合同版本, direction, id, type, h});
    if (r.状态 != L1所有者范围读取状态::成功) throw 映射(r.状态);
    要求(r.读取事实代次 == g, S::并发变化);
    要求(r.合同版本 == L1所有者范围CRUD合同版本 && r.方向 == direction
        && r.端点节点 == id && r.关系类型节点 == type && r.历史截止事实代次 == h);
    std::set<std::uint64_t> seen;
    for (const auto& e : r.关系组) {
        if (meter) meter->记(L1所有者范围事实副本{e});
        生命周期(e, g, h);
        要求(有效(e.编码) && e.关系类型节点 == type && (incoming ? e.目标节点 : e.源节点) == id
            && e.写入所有者 == 端口(p).所有者身份() && seen.insert(e.编码.值).second);
    }
    std::sort(r.关系组.begin(), r.关系组.end(), [](const E& a, const E& b) {
        return a.角色或顺序 != b.角色或顺序 ? a.角色或顺序 < b.角色或顺序 : a.编码 < b.编码;
    });
    if(meter)meter->已读关系组.emplace(cacheKey,r.关系组);
    return r.关系组;
}
std::vector<特征类数据服务::V> 特征类数据服务::属性(稳定编码 id,
    std::uint64_t g, std::uint64_t h, 分区 p, 读取计量* meter) const {
    const auto cacheKey=std::make_tuple(g,h,id);
    if(meter&&meter->已读属性组.contains(cacheKey)) {
        const auto& cached=meter->已读属性组.at(cacheKey);
        for(const auto& v:cached)要求(v.写入所有者==端口(p).所有者身份());
        return cached;
    }
    auto r = l1_.读取所有者范围历史属性值组({L1所有者范围CRUD合同版本, id, h});
    if (r.状态 != L1所有者范围读取状态::成功) throw 映射(r.状态);
    要求(r.读取事实代次 == g, S::并发变化);
    要求(r.合同版本 == L1所有者范围CRUD合同版本 && r.所属节点 == id && r.历史截止事实代次 == h);
    std::set<std::uint64_t> slots;
    for (const auto& v : r.属性值组) {
        if (meter) meter->记(L1所有者范围事实副本{v});
        生命周期(v, g, h);
        要求(有效(v.编码) && v.所属节点 == id && 有效(v.来源节点)
            && v.写入所有者 == 端口(p).所有者身份() && slots.insert(v.属性类型节点.值).second
            && L1所有者范围原始材料完整(v.材料));
        (void)节点(v.来源节点, g, h, std::nullopt, meter);
    }
    if(meter)meter->已读属性组.emplace(cacheKey,r.属性值组);
    return r.属性值组;
}
特征类数据服务::E 特征类数据服务::唯一关系(稳定编码 id, 稳定编码 type,
    std::uint64_t g, std::uint64_t h, 分区 p, 读取计量* meter) const {
    auto es = 关系(id, type, false, g, h, p, meter);
    要求(es.size() == 1 && es.front().角色或顺序 == 1, S::旧格式不支持); return es.front();
}
const 特征类数据服务::V& 特征类数据服务::唯一属性(const std::vector<V>& values, 稳定编码 type) {
    auto it = std::find_if(values.begin(), values.end(), [&](const V& v) { return v.属性类型节点 == type; });
    要求(it != values.end(), S::旧格式不支持); return *it;
}
void 特征类数据服务::结构就绪(分区 p, std::uint64_t g, std::uint64_t h, 读取计量* meter) const {
    要求(definition_ready_ && (p == 分区::定义 || information_ready_), S::未设置);
    if (p == 分区::定义) { for (auto id : d_) (void)节点(id, g, h, p, meter); }
    else { for (auto id : f_) (void)节点(id, g, h, p, meter); }
}
特征类数据服务::E 特征类数据服务::核对归属(稳定编码 id, std::uint64_t g,
    std::uint64_t h, 分区 p, 读取计量* meter) const {
    结构就绪(p, g, h, meter);
    auto e = 唯一关系(id, p == 分区::定义 ? d_[定义归属] : f_[信息归属], g, h, p, meter);
    要求(e.目标节点 == (p == 分区::定义 ? d_[定义锚点] : f_[信息锚点])
        && e.源节点 != e.目标节点, S::旧格式不支持); return e;
}
特征规范I64域 特征类数据服务::规范域(特征规范I64域 domain) {
    要求(!domain.区间.empty(), S::入口拒绝);
    for (auto x : domain.区间) 要求(浅层结构有效(x), S::入口拒绝);
    std::sort(domain.区间.begin(), domain.区间.end(), [](auto a, auto b) {
        return a.下界 != b.下界 ? a.下界 < b.下界 : a.上界 < b.上界;
    });
    std::vector<特征I64闭区间> out;
    for (auto x : domain.区间) {
        if (!out.empty() && (x.下界 <= out.back().上界
            || (out.back().上界 != std::numeric_limits<std::int64_t>::max() && x.下界 == out.back().上界 + 1)))
            out.back().上界 = std::max(out.back().上界, x.上界);
        else out.push_back(x);
    } return {std::move(out)};
}
bool 特征类数据服务::包含(const 特征规范I64域& outer, const 特征规范I64域& inner) {
    std::size_t i = 0;
    for (auto x : inner.区间) {
        while (i < outer.区间.size() && outer.区间[i].上界 < x.下界) ++i;
        if (i == outer.区间.size() || outer.区间[i].下界 > x.下界 || outer.区间[i].上界 < x.上界) return false;
    } return true;
}
void 特征类数据服务::检查形成规格(const I64基础特征类型形成规格& s) {
    要求((s.来源 == 特征类型来源::外设能够获取
            || s.来源 == 特征类型来源::先天定义)
        && s.缩放分子 && s.缩放分母
        && std::gcd(s.缩放分子, s.缩放分母) == 1 && !s.允许集合.empty(), S::入口拒绝);
    if (s.来源 == 特征类型来源::外设能够获取)
        要求(s.外设提供者 && 有效(*s.外设提供者), S::入口拒绝);
    else 要求(!s.外设提供者, S::入口拒绝);
    if (s.单位绑定 == I64基础特征单位绑定::既有稳定单位)
        要求(s.既有单位 && 有效(*s.既有单位), S::入口拒绝);
    else if (s.单位绑定 == I64基础特征单位绑定::新FT自身)
        要求(!s.既有单位, S::入口拒绝);
    else throw S::入口拒绝;
    for (auto x : s.允许集合) 要求(浅层结构有效(x), S::入口拒绝);
    if (s.域形成) 要求(s.域形成->允许误差 >= 0 && 有效(s.域形成->参数来源), S::入口拒绝);
}
void 特征类数据服务::检查规格(const I64基础特征类型规格& s) {
    要求((s.来源 == 特征类型来源::外设能够获取
            || s.来源 == 特征类型来源::先天定义
            || s.来源 == 特征类型来源::后天派生)
        && 有效(s.单位) && s.缩放分子 && s.缩放分母
        && std::gcd(s.缩放分子, s.缩放分母) == 1 && !s.允许集合.empty(), S::入口拒绝);
    if (s.来源 == 特征类型来源::外设能够获取)
        要求(s.外设提供者 && 有效(*s.外设提供者), S::类型不相容);
    else 要求(!s.外设提供者, S::类型不相容);
    for (auto x : s.允许集合) 要求(浅层结构有效(x), S::入口拒绝);
    if (s.域形成) 要求(s.域形成->允许误差 >= 0 && 有效(s.域形成->参数来源), S::入口拒绝);
}
std::int64_t 特征类数据服务::完整整数(const 准确特征读取事实& f) {
    if (const auto* v = std::get_if<std::int64_t>(&f.完整值)) return *v;
    const auto* v = std::get_if<std::int64_t>(&std::get<特征值信息>(f.完整值).值内容);
    要求(v != nullptr, S::能力未提供); return *v;
}

I64基础特征类型信息 特征类数据服务::读类型(特征类型身份 id, std::uint64_t g, std::uint64_t h,
    标量读取上下文* count, 读取计量* meter) const {
    要求(有效(id), S::入口拒绝);
    const auto n = 节点(id.编码, g, h, 分区::定义, meter);
    const auto family = 核对归属(id.编码, g, h, 分区::定义, meter);
    if (count) 标量计数(*count, count->关系计数, family.编码, count->预算.最大关系数);
    要求(n.种类 == 节点种类::属性类型 && n.属性类型表示 == L1所有者范围值表示种类::I64, S::能力未提供);
    auto attrs = 属性(id.编码, g, h, 分区::定义, meter);
    if (count) for (const auto& value : attrs) 标量计数(*count, count->值计数, value.编码, count->预算.最大属性值数);
    const auto specifications = std::count_if(attrs.begin(), attrs.end(), [&](const V& value) {
        return value.属性类型节点 == d_[类型规格属性];
    });
    const auto sourcesValues = std::count_if(attrs.begin(), attrs.end(), [&](const V& value) {
        return value.属性类型节点 == source_[类型来源属性];
    });
    要求(specifications == 1 && sourcesValues <= 1
        && attrs.size() == specifications + sourcesValues, S::旧格式不支持);
    const auto& specification = 唯一属性(attrs, d_[类型规格属性]);
    const auto* data = std::get_if<std::vector<std::uint64_t>>(&specification.材料);
    要求(data && data->size() >= 5 && (*data)[2] && (*data)[2] <= (data->size() - 3) / 2
        && data->size() == 3 + 2 * (*data)[2], S::旧格式不支持);
    const auto sources = 关系(id.编码, d_[外设来源关系], false, g, h, 分区::定义, meter);
    const auto unit = 唯一关系(id.编码, d_[单位关系], g, h, 分区::定义, meter);
    if (count) {
        for (const auto& source : sources)
            标量计数(*count, count->关系计数, source.编码, count->预算.最大关系数);
        标量计数(*count, count->关系计数, unit.编码, count->预算.最大关系数);
    }
    特征类型来源 source = 特征类型来源::外设能够获取;
    if (sourcesValues == 1) {
        const auto& sourceValue = 唯一属性(attrs, source_[类型来源属性]);
        const auto* rawSource = std::get_if<std::int64_t>(&sourceValue.材料);
        要求(rawSource && (*rawSource == static_cast<std::int64_t>(特征类型来源::外设能够获取)
            || *rawSource == static_cast<std::int64_t>(特征类型来源::先天定义)
            || *rawSource == static_cast<std::int64_t>(特征类型来源::后天派生)), S::类型不相容);
        source = static_cast<特征类型来源>(*rawSource);
    } else {
        // 旧格式只在存在唯一合法外设关系时兼容为外设来源；零关系不得推断为先天定义。
        要求(sources.size() == 1, S::旧格式不支持);
    }
    if (source == 特征类型来源::外设能够获取) {
        要求(sources.size() == 1, S::引用冲突);
        (void)节点(sources.front().目标节点, g, h, std::nullopt, meter);
    } else 要求(sources.empty(), S::引用冲突);
    (void)节点(unit.目标节点, g, h, std::nullopt, meter);
    I64基础特征类型信息 out; out.身份 = id; out.规格.来源 = source;
    if (source == 特征类型来源::外设能够获取)
        out.规格.外设提供者 = sources.front().目标节点;
    out.规格.单位 = unit.目标节点;
    out.规格.缩放分子 = (*data)[0]; out.规格.缩放分母 = (*data)[1];
    for (std::size_t i = 3; i < data->size(); i += 2)
        out.规格.允许集合.push_back({std::bit_cast<std::int64_t>((*data)[i]), std::bit_cast<std::int64_t>((*data)[i + 1])});
    auto rules = 关系(id.编码, d_[域规则关系], false, g, h, 分区::定义, meter);
    if (count) for (const auto& edge : rules) 标量计数(*count, count->关系计数, edge.编码, count->预算.最大关系数);
    要求(rules.size() <= 1);
    if (!rules.empty()) {
        const auto& e = rules.front(); 要求(e.角色或顺序 == 1);
        const auto rule = 节点(e.目标节点, g, h, 分区::定义, meter);
        要求(rule.种类 == 节点种类::普通 && rule.创建事实代次 == n.创建事实代次);
        const auto values = 属性(e.目标节点, g, h, 分区::定义, meter);
        if (count) for (const auto& fact : values) 标量计数(*count, count->值计数, fact.编码, count->预算.最大属性值数);
        要求(values.size() == 1);
        const auto& value = 唯一属性(values, d_[规则误差属性]);
        const auto* error = std::get_if<std::int64_t>(&value.材料); 要求(error && *error >= 0);
        const auto parameter = 唯一关系(e.目标节点, d_[参数来源关系], g, h, 分区::定义, meter);
        if (count) 标量计数(*count, count->关系计数, parameter.编码, count->预算.最大关系数);
        (void)节点(parameter.目标节点, g, h, std::nullopt, meter);
        out.规格.域形成 = I64特征域形成参数{*error, parameter.目标节点};
        out.规则 = 特征比较规则身份{e.目标节点};
    }
    检查规格(out.规格);
    return out;
}
特征规范I64域 特征类数据服务::读完整域(特征类型身份 id, std::uint64_t g, std::uint64_t h) const {
    const auto n = 节点(id.编码, g, h, 分区::定义);
    const auto attrs = 属性(id.编码, g, h, 分区::定义);
    const auto marker = std::find_if(attrs.begin(), attrs.end(), [&](const V& v) { return v.属性类型节点 == id.编码; });
    if (marker == attrs.end()) return 规范域({读类型(id, g, h).规格.允许集合});
    const auto* value = std::get_if<std::int64_t>(&marker->材料);
    要求(value && *value == 标量格式标记 && attrs.size() == 1, S::旧格式不支持);
    try {
        标量读取上下文 c{g, h, 标量业务准入预算};
        const auto edges = 标量关系(id.编码, d_[输出FT归属关系], true, c);
        标量要求(edges.size() == 1 && edges.front().角色或顺序 >= 1 && edges.front().角色或顺序 <= 3);
        标量展开(edges.front().源节点, true, c);
        const auto& output = 标量输出(c.定义.at(edges.front().源节点), static_cast<unsigned>(edges.front().角色或顺序));
        要求(output.特征类型 == id.编码 && output.格式标记值事实 == marker->编码);
        return 特征规范I64域{{{output.声明.量化.下界, output.声明.量化.上界}}};
    } catch (const 标量失败& e) {
        switch (e.状态) {
        case SS::预算不足: throw S::数量预算不足;
        case SS::资源失败: throw S::资源失败;
        case SS::事实代次漂移: throw S::并发变化;
        case SS::历史材料不可用: throw S::历史材料不可用;
        case SS::未找到: throw S::未找到;
        case SS::已退出: throw S::已退出;
        case SS::格式不支持: throw S::旧格式不支持;
        case SS::类型不匹配: case SS::单位量化不匹配: throw S::类型不相容;
        case SS::算法不支持: throw S::能力未提供;
        default: throw S::内部不一致;
        }
    }
}
std::int64_t 特征类数据服务::解析输入(const 特征准确值& input, std::uint64_t g, std::uint64_t h,读取计量* meter) const {
    要求(浅层结构有效(input), S::入口拒绝);
    if (const auto* v = std::get_if<std::int64_t>(&input)) return *v;
    auto id = std::get<特征值身份>(input);
    auto raw = 原始事实(id.编码, g,meter); const auto* fact = std::get_if<V>(&raw);
    要求(fact != nullptr, S::类型不相容); 生命周期(*fact, g, h);
    auto result = values_.获取特征值(id);
    if (const auto* error = std::get_if<特征值读取错误>(&result)) {
        switch (*error) {
        case 特征值读取错误::入口拒绝: throw S::入口拒绝;
        case 特征值读取错误::未找到: throw S::未找到;
        case 特征值读取错误::材料已清理: throw S::历史材料不可用;
        case 特征值读取错误::能力未提供: throw S::能力未提供;
        case 特征值读取错误::资源失败: throw S::资源失败;
        default: throw S::内部不一致;
        }
    }
    auto& full = std::get<特征值信息>(result);
    const auto* value = std::get_if<std::int64_t>(&full.值内容);
    const auto* stored = std::get_if<std::int64_t>(&fact->材料);
    要求(value && stored, S::能力未提供);
    要求(full.值身份 == id && *stored == *value); 守卫(g); return *value;
}
准确特征读取事实 特征类数据服务::读准确(特征信息身份 id, std::uint64_t g, std::uint64_t h,
    标量读取上下文* count, 读取计量* meter) const {
    const auto n = 节点(id.编码, g, h, 分区::信息, meter);
    要求(n.种类 == 节点种类::普通, S::旧格式不支持);
    const auto family = 核对归属(id.编码, g, h, 分区::信息, meter);
    const auto type = 唯一关系(id.编码, f_[准确类型关系], g, h, 分区::信息, meter);
    if (count) {
        标量计数(*count, count->关系计数, family.编码, count->预算.最大关系数);
        标量计数(*count, count->关系计数, type.编码, count->预算.最大关系数);
    }
    const auto ft = 读类型({type.目标节点}, g, h, count, meter);
    const auto values = 属性(id.编码, g, h, 分区::信息, meter);
    if (count) for (const auto& value : values) 标量计数(*count, count->值计数, value.编码, count->预算.最大属性值数);
    要求(values.size() == 1, S::旧格式不支持);
    const auto& v = values.front(); const auto* scalar = std::get_if<std::int64_t>(&v.材料);
    要求(scalar && (v.属性类型节点 == f_[准确内联属性] || v.属性类型节点 == f_[准确引用属性]), S::旧格式不支持);
    要求(v.创建事实代次 == n.创建事实代次 && type.创建事实代次 == n.创建事实代次
        && family.创建事实代次 == n.创建事实代次 && v.退出事实代次 == n.退出事实代次
        && type.退出事实代次 == n.退出事实代次 && family.退出事实代次 == n.退出事实代次);
    要求(包含(规范域({ft.规格.允许集合}), 特征规范I64域{{{*scalar, *scalar}}}), S::类型不相容);
    准确特征读取事实 out;
    out.Gread = g; out.H = h; out.信息.身份 = id; out.信息.类型 = ft.身份;
    out.类型关系 = type.编码; out.准确值事实 = v.编码;
    out.创建G = n.创建事实代次; out.退出G = n.退出事实代次;
    if (v.属性类型节点 == f_[准确内联属性]) { out.信息.准确值 = *scalar; out.完整值 = *scalar; }
    else {
        特征值身份 vid{v.编码};
        // 引用分支仍由特征值服务完整核验；同编码事实不重复计入本次用量。
        要求(解析输入(特征准确值{vid}, g, h,meter) == *scalar);
        out.信息.准确值 = vid; out.完整值 = 特征值信息{vid, 特征值内容{*scalar}};
    } return out;
}
特征类数据服务::Key 特征类数据服务::新键(const WS& ws) {
    std::uint32_t max = 0;
    for (const auto& x : ws.节点) max = std::max(max, x.本地键.值);
    for (const auto& x : ws.关系) max = std::max(max, x.本地键.值);
    for (const auto& x : ws.值) max = std::max(max, x.本地键.值);
    要求(max != std::numeric_limits<std::uint32_t>::max(), S::数量预算不足); return {max + 1};
}
特征类数据服务::Key 特征类数据服务::加节点(WS& ws, std::optional<L1所有者范围值表示种类> repr) {
    auto k = 新键(ws); ws.节点.push_back({k, repr ? 节点种类::属性类型 : 节点种类::普通, repr}); return k;
}
特征类数据服务::Key 特征类数据服务::加关系(WS& ws, Ref a, Ref b, Ref type, std::int64_t role) {
    auto k = 新键(ws); ws.关系.push_back({k, std::move(a), std::move(b), std::move(type), role}); return k;
}
特征类数据服务::Key 特征类数据服务::加值(WS& ws, Ref node, Ref type,
    L1所有者范围原始值材料 content, std::optional<Ref> source) const {
    auto k = 新键(ws); ws.值.push_back({k, node, type, std::move(content), source ? *source : Ref{producer_}});
    ws.属性槽变更.push_back({std::move(node), std::move(type), k}); return k;
}
特征类数据服务::WS 特征类数据服务::新写集(分区 p, std::uint64_t g) const {
    要求(!pending_&&!scalar_pending_&&!binding_pending_, S::前次写入待收敛);
    要求(g && g < std::numeric_limits<std::uint64_t>::max(), S::算术不可表示);
    auto key = g + 1;
    for (;;) {
        if ((key >> 48) == 0x4E43) key = 0x4E44'0000'0000'0000;
        auto first = 端口(p).读取首次写入材料({L1所有者范围首次写入读取合同版本, {key}});
        要求(first.读取事实代次 == g, S::并发变化);
        if (first.状态 == L1所有者范围读取状态::未找到) break;
        if (first.状态 != L1所有者范围读取状态::成功) throw 映射(first.状态);
        要求(key != std::numeric_limits<std::uint64_t>::max(), S::算术不可表示); ++key;
    }
    WS ws; ws.期望事实代次 = g; ws.写入幂等身份 = {key}; return ws;
}
void 特征类数据服务::规范化写集(WS& ws) {
    const auto by_key = [](const auto& a, const auto& b) { return a.本地键 < b.本地键; };
    std::sort(ws.节点.begin(), ws.节点.end(), by_key); std::sort(ws.关系.begin(), ws.关系.end(), by_key);
    std::sort(ws.值.begin(), ws.值.end(), by_key);
    const auto order = [](const Ref& ref) {
        if (const auto* id = std::get_if<稳定编码>(&ref)) return id->值;
        return (std::uint64_t{1} << 63) | std::get<Key>(ref).值;
    };
    std::sort(ws.属性槽变更.begin(), ws.属性槽变更.end(), [&](const auto& a, const auto& b) {
        if (order(a.所属节点) != order(b.所属节点)) return order(a.所属节点) < order(b.所属节点);
        if (order(a.属性类型节点) != order(b.属性类型节点)) return order(a.属性类型节点) < order(b.属性类型节点);
        return a.新当前值 < b.新当前值;
    });
    std::sort(ws.退出事实.begin(), ws.退出事实.end());
    要求(std::adjacent_find(ws.退出事实.begin(), ws.退出事实.end()) == ws.退出事实.end());
}
稳定编码 特征类数据服务::映射编码(const L1所有者范围写入结果& r, Key k) {
    auto it = std::find_if(r.新编码映射.begin(), r.新编码映射.end(), [&](const auto& x) { return x.first == k; });
    要求(it != r.新编码映射.end() && 有效(it->second)); return it->second;
}
稳定编码 特征类数据服务::解析引用(const L1所有者范围写入结果& r, const Ref& ref) {
    if (const auto* id = std::get_if<稳定编码>(&ref)) return *id; return 映射编码(r, std::get<Key>(ref));
}
特征类数据服务::WS 特征类数据服务::初始化写集(分区 p, std::uint64_t g) {
    WS ws; ws.期望事实代次 = g; ws.写入幂等身份 = {1};
    const auto count = p == 分区::定义 ? std::size_t{定义角色数} : std::size_t{信息角色数};
    for (std::size_t i = 0; i < count; ++i) {
        std::optional<L1所有者范围值表示种类> repr;
        if (p == 分区::定义) {
            if (i == 类型规格属性 || i == 注册U64属性 || i == 派生规则属性) repr = L1所有者范围值表示种类::U64组;
            if (i == 规则误差属性 || i == 阶次属性) repr = L1所有者范围值表示种类::I64;
            if (i == 注册I64属性) repr = L1所有者范围值表示种类::I64组;
        } else if (i == 准确内联属性 || i == 准确引用属性) repr = L1所有者范围值表示种类::I64;
        (void)加节点(ws, repr);
    } return ws;
}
void 特征类数据服务::接受初始化(分区 p, const L1所有者范围写入结果& result) {
    if (p == 分区::定义) {
        for (std::size_t i = 0; i < d_.size(); ++i) d_[i] = 映射编码(result, {static_cast<std::uint32_t>(i + 1)});
        definition_ready_ = true;
    } else {
        for (std::size_t i = 0; i < f_.size(); ++i) f_[i] = 映射编码(result, {static_cast<std::uint32_t>(i + 1)});
        information_ready_ = true;
    }
}

void 特征类数据服务::确认发布(分区 p, const WS& ws, const L1所有者范围写入结果& r) const {
    要求(r.状态 == L1所有者范围写入状态::成功 && r.合同版本 == L1所有者范围CRUD合同版本
        && r.所有者 == 端口(p).所有者身份() && r.写入幂等身份 == ws.写入幂等身份
        && r.是否形成内存权威发布 && r.事实代次 && r.事实代次 == ws.期望事实代次 + 1
        && r.新编码映射.size() == ws.节点.size() + ws.关系.size() + ws.值.size());
    std::set<std::uint32_t> keys; std::set<std::uint64_t> ids;
    for (const auto& [k, id] : r.新编码映射)
        要求(keys.insert(k.值).second && ids.insert(id.值).second && 有效(id));
    const auto g = 当前G(), h = r.事实代次; 要求(g >= h);
    for (const auto& x : ws.节点) {
        auto raw = 原始事实(映射编码(r, x.本地键), g); const auto* n = std::get_if<N>(&raw);
        要求(n && n->种类 == x.种类 && n->属性类型表示 == x.属性类型表示
            && n->创建事实代次 == h && n->写入所有者 == 端口(p).所有者身份());
    }
    for (const auto& x : ws.关系) {
        auto raw = 原始事实(映射编码(r, x.本地键), g); const auto* e = std::get_if<E>(&raw);
        要求(e && e->源节点 == 解析引用(r, x.源节点) && e->目标节点 == 解析引用(r, x.目标节点)
            && e->关系类型节点 == 解析引用(r, x.关系类型节点) && e->角色或顺序 == x.角色或顺序
            && e->写入所有者 == 端口(p).所有者身份() && e->创建事实代次 == h);
    }
    for (const auto& x : ws.值) {
        auto raw = 原始事实(映射编码(r, x.本地键), g); const auto* v = std::get_if<V>(&raw);
        要求(v && v->所属节点 == 解析引用(r, x.所属节点) && v->属性类型节点 == 解析引用(r, x.属性类型节点)
            && v->来源节点 == 解析引用(r, x.来源节点) && v->材料 == x.材料
            && v->写入所有者 == 端口(p).所有者身份() && v->创建事实代次 == h);
    }
    for (const auto& x : ws.属性槽变更) {
        const auto values = 属性(解析引用(r, x.所属节点), g, h, p);
        要求(唯一属性(values, 解析引用(r, x.属性类型节点)).编码 == 映射编码(r, x.新当前值));
    }
    for (auto id : ws.退出事实) {
        const auto raw = 原始事实(id, g);
        std::visit([&](const auto& f) { 要求(f.写入所有者 == 端口(p).所有者身份() && f.退出事实代次 == h); }, raw);
    } 守卫(g);
}
L1所有者范围写入结果 特征类数据服务::收敛原请求() {
    要求(pending_.has_value());
    try {
        const auto p = pending_->区; const auto& ws = pending_->请求;
        auto first = 端口(p).读取首次写入材料({L1所有者范围首次写入读取合同版本, ws.写入幂等身份});
        if (first.状态 == L1所有者范围读取状态::未找到) {
            const auto replay = 端口(p).提交所有者范围中性写集(ws);
            if (replay.状态 != L1所有者范围写入状态::成功 && replay.状态 != L1所有者范围写入状态::精确重复) {
                if (!replay.是否形成内存权威发布
                    && (replay.状态 == L1所有者范围写入状态::入口拒绝 || replay.状态 == L1所有者范围写入状态::引用冲突
                        || replay.状态 == L1所有者范围写入状态::未找到 || replay.状态 == L1所有者范围写入状态::已退出
                        || replay.状态 == L1所有者范围写入状态::事实代次漂移 || replay.状态 == L1所有者范围写入状态::幂等冲突)) {
                    const auto s = 映射(replay.状态); pending_.reset(); throw s;
                } throw S::发布结果未确认;
            }
            first = 端口(p).读取首次写入材料({L1所有者范围首次写入读取合同版本, ws.写入幂等身份});
        }
        要求(first.状态 == L1所有者范围读取状态::成功 && first.首次规范化写集 && first.首次写入结果, S::发布结果未确认);
        if(first.所有者 != 端口(p).所有者身份() || first.写入幂等身份 != ws.写入幂等身份
            || *first.首次规范化写集 != ws) {
            pending_.reset(); throw S::幂等冲突;
        }
        确认发布(p, ws, *first.首次写入结果);
        if (pending_->初始化) 接受初始化(p, *first.首次写入结果);
        if (pending_->I64扩展初始化) 接受I64扩展(*first.首次写入结果);
        if (pending_->R规则扩展初始化) 接受R规则扩展(*first.首次写入结果);
        auto result = std::move(*first.首次写入结果); pending_.reset(); return result;
    } catch (...) { if (pending_) throw S::发布结果未确认; throw; }
}
L1所有者范围写入结果 特征类数据服务::提交(分区 p, WS ws, bool init) {
    要求(!pending_&&!scalar_pending_&&!binding_pending_, S::前次写入待收敛); 规范化写集(ws);
    pending_.emplace(待确认写入{p, std::move(ws), init});
    try { (void)端口(p).提交所有者范围中性写集(pending_->请求); return 收敛原请求(); }
    catch (...) { if (pending_) throw S::发布结果未确认; throw; }
}
void 特征类数据服务::初始化(分区 p) {
    要求(!pending_&&!scalar_pending_&&!binding_pending_, S::前次写入待收敛);
    const auto g = 当前G(); (void)节点(producer_, g, g);
    if (p == 分区::信息) 结构就绪(分区::定义, g, g);
    const auto first = 端口(p).读取首次写入材料({L1所有者范围首次写入读取合同版本, {1}});
    要求(first.读取事实代次 == g, S::并发变化);
    if (first.状态 == L1所有者范围读取状态::成功) {
        要求(first.首次规范化写集 && first.首次写入结果);
        const auto expected = 初始化写集(p, first.首次规范化写集->期望事实代次);
        要求(*first.首次规范化写集 == expected, S::旧格式不支持);
        确认发布(p, expected, *first.首次写入结果); 接受初始化(p, *first.首次写入结果);
    } else if (first.状态 == L1所有者范围读取状态::未找到) (void)提交(p, 初始化写集(p, g), true);
    else throw 映射(first.状态);
}
void 特征类数据服务::初始化R规则扩展() {
    要求(!pending_ && !scalar_pending_ && !binding_pending_, S::前次写入待收敛);
    const auto g = 当前G();
    const auto first = definitions_.读取首次写入材料(
        {L1所有者范围首次写入读取合同版本, R规则结构扩展初始化幂等身份});
    要求(first.读取事实代次 == g, S::并发变化);
    if (first.状态 == L1所有者范围读取状态::成功) {
        要求(first.首次规范化写集 && first.首次写入结果, S::旧格式不支持);
        const auto ws = R规则扩展写集(first.首次规范化写集->期望事实代次);
        要求(*first.首次规范化写集 == ws, S::旧格式不支持);
        要求(first.所有者 == definitions_.所有者身份()
            && first.写入幂等身份 == ws.写入幂等身份, S::幂等冲突);
        确认发布(分区::定义, ws, *first.首次写入结果);
        接受R规则扩展(*first.首次写入结果);
    } else if (first.状态 == L1所有者范围读取状态::未找到) {
        auto ws = R规则扩展写集(g);
        pending_.emplace(待确认写入{分区::定义, std::move(ws), false, {}, false, true});
        (void)收敛原请求();
    } else throw 映射(first.状态);
}
void 特征类数据服务::初始化类型来源扩展() {
    要求(!pending_ && !scalar_pending_ && !binding_pending_, S::前次写入待收敛);
    const auto g = 当前G();
    const auto first = definitions_.读取首次写入材料(
        {L1所有者范围首次写入读取合同版本, 类型来源结构扩展初始化幂等身份});
    要求(first.读取事实代次 == g, S::并发变化);
    if (first.状态 == L1所有者范围读取状态::成功) {
        要求(first.首次规范化写集 && first.首次写入结果, S::旧格式不支持);
        const auto ws = 类型来源扩展写集(first.首次规范化写集->期望事实代次);
        要求(*first.首次规范化写集 == ws && first.所有者 == definitions_.所有者身份()
            && first.写入幂等身份 == ws.写入幂等身份, S::幂等冲突);
        确认发布(分区::定义, ws, *first.首次写入结果); 接受类型来源扩展(*first.首次写入结果);
    } else if (first.状态 == L1所有者范围读取状态::未找到) {
        auto ws = 类型来源扩展写集(g);
        pending_.emplace(待确认写入{分区::定义, std::move(ws), false});
        const auto receipt = 收敛原请求(); 接受类型来源扩展(receipt);
    } else throw 映射(first.状态);
}
void 特征类数据服务::添加I64默认R规则(WS& ws, Ref ft, bool hasDomainFormation) const {
    R规则就绪();
    const auto append = [&](std::int64_t usage) {
        const auto rule = 加节点(ws);
        (void)加关系(ws, rule, d_[定义锚点], d_[定义归属]);
        (void)加关系(ws, ft, rule, r_[R规则归属关系], usage);
        (void)加值(ws, rule, r_[R规则版本属性], std::int64_t{1});
        (void)加值(ws, rule, r_[R规则参数属性], std::vector<std::uint64_t>{1});
    };
    append(1); append(2); if (hasDomainFormation) append(3);
}
std::optional<稳定编码> 特征类数据服务::读取R规则(
    特征类型身份 ft, std::int64_t usage, std::uint64_t g, std::uint64_t h,
    const 特征R规则读取预算& budget) const {
    R规则就绪();
    要求(usage >= 1 && usage <= 3, S::入口拒绝);
    const auto all = 关系(ft.编码, r_[R规则归属关系], false, g, h, 分区::定义);
    // 本次规则读取会读取全部 FT→规则关系、一个规则锚点、其定义归属关系及两个属性值。
    // 先按实际返回组守卫，不能把预算仅当作非零标记。
    要求(all.size() + 1 <= budget.最大规则关系数 && budget.最大规则节点数 >= 1
        && budget.最大规则值数 >= 2, S::数量预算不足);
    std::vector<E> matches;
    for (const auto& edge : all) {
        要求(edge.源节点 == ft.编码 && edge.关系类型节点 == r_[R规则归属关系], S::旧格式不支持);
        if (edge.角色或顺序 == usage) matches.push_back(edge);
        else 要求(edge.角色或顺序 >= 1 && edge.角色或顺序 <= 3, S::旧格式不支持);
    }
    if (matches.empty()) return std::nullopt;
    要求(matches.size() == 1, S::内部不一致);
    const auto anchor = 节点(matches.front().目标节点, g, h, 分区::定义);
    要求(anchor.种类 == 节点种类::普通 && !anchor.属性类型表示, S::旧格式不支持);
    要求(matches.front().创建事实代次 == anchor.创建事实代次
        && matches.front().退出事实代次 == anchor.退出事实代次, S::旧格式不支持);
    const auto owner = 唯一关系(anchor.编码, d_[定义归属], g, h, 分区::定义);
    要求(owner.目标节点 == d_[定义锚点] && owner.源节点 == anchor.编码
        && owner.创建事实代次 == anchor.创建事实代次
        && owner.退出事实代次 == anchor.退出事实代次, S::旧格式不支持);
    const auto values = 属性(anchor.编码, g, h, 分区::定义);
    要求(values.size() == 2, S::旧格式不支持);
    const auto& version = 唯一属性(values, r_[R规则版本属性]);
    const auto& parameter = 唯一属性(values, r_[R规则参数属性]);
    const auto* v = std::get_if<std::int64_t>(&version.材料);
    const auto* p = std::get_if<std::vector<std::uint64_t>>(&parameter.材料);
    要求(v && *v == 1 && p && *p == std::vector<std::uint64_t>{1}, S::内部不一致);
    for (const auto* x : {&version, &parameter})
        要求(x->所属节点 == anchor.编码 && x->创建事实代次 == anchor.创建事实代次
            && x->退出事实代次 == anchor.退出事实代次, S::旧格式不支持);
    return anchor.编码;
}
特征数据结果<std::monostate> 特征类数据服务::初始化特征定义结构() {
    return 保护<std::monostate>([&] {
        初始化(分区::定义); 初始化I64扩展(); 初始化R规则扩展(); 初始化类型来源扩展();
        return std::monostate{};
    });
}
特征数据结果<std::monostate> 特征类数据服务::初始化准确特征结构() {
    return 保护<std::monostate>([&] { 初始化(分区::信息); return std::monostate{}; });
}
特征数据结果<std::monostate> 特征类数据服务::收敛待确认写入() {
    return 保护<std::monostate>([&] {
        要求(!binding_pending_&&!scalar_pending_&&(!pending_ || !pending_->旧治理), S::前次写入待收敛);
        if (pending_) (void)收敛原请求(); return std::monostate{};
    });
}
特征类数据服务::WS 特征类数据服务::I64基础类型写集(
    std::uint64_t g, const I64基础特征类型定义请求& request) const {
    类型来源就绪(); R规则就绪();
    WS ws; ws.期望事实代次 = g; ws.写入幂等身份 = request.幂等身份;
    const auto ft = 加节点(ws, L1所有者范围值表示种类::I64);
    (void)加关系(ws, ft, d_[定义锚点], d_[定义归属]);
    if (request.规格.外设提供者)
        (void)加关系(ws, ft, *request.规格.外设提供者, d_[外设来源关系]);
    const Ref unit = request.规格.单位绑定 == I64基础特征单位绑定::新FT自身
        ? Ref{ft} : Ref{*request.规格.既有单位};
    (void)加关系(ws, ft, unit, d_[单位关系]);
    std::vector<std::uint64_t> data{request.规格.缩放分子, request.规格.缩放分母,
        request.规格.允许集合.size()};
    for (const auto x : request.规格.允许集合) {
        data.push_back(std::bit_cast<std::uint64_t>(x.下界));
        data.push_back(std::bit_cast<std::uint64_t>(x.上界));
    }
    (void)加值(ws, ft, d_[类型规格属性], std::move(data));
    (void)加值(ws, ft, source_[类型来源属性], static_cast<std::int64_t>(request.规格.来源));
    if (request.规格.域形成) {
        const auto rule = 加节点(ws);
        (void)加关系(ws, ft, rule, d_[域规则关系]);
        (void)加关系(ws, rule, request.规格.域形成->参数来源, d_[参数来源关系]);
        (void)加值(ws, rule, d_[规则误差属性], request.规格.域形成->允许误差);
    }
    添加I64默认R规则(ws, ft, request.规格.域形成.has_value());
    return ws;
}

特征截止事实<I64基础特征类型信息> 特征类数据服务::读取I64基础类型定义事实(
    const L1所有者范围写入结果& receipt, std::uint64_t g) const {
    const auto ft = 特征类型身份{映射编码(receipt, {1})};
    auto info = 读类型(ft, g, receipt.事实代次);
    守卫(g);
    return {g, receipt.事实代次, std::move(info)};
}

I64基础特征类型定义结果 特征类数据服务::形成或读取I64基础特征类型(
    const I64基础特征类型定义请求& request) noexcept {
    I64基础特征类型定义结果 out; out.原请求 = request;
    using V = I64基础特征类型定义状态;
    try {
        std::lock_guard<std::mutex> lock(mutex_);
        if (request.版本 != 1 || !request.G0 || !有效(request.幂等身份)) return out;
        检查形成规格(request.规格);
        if (当前G() != request.G0) { out.状态 = V::当前性漂移; return out; }
        结构就绪(分区::定义, request.G0, request.G0); 类型来源就绪(); R规则就绪();
        (void)节点(producer_, request.G0, request.G0);
        if (request.规格.外设提供者) (void)节点(*request.规格.外设提供者, request.G0, request.G0);
        if (request.规格.既有单位) (void)节点(*request.规格.既有单位, request.G0, request.G0);
        if (request.规格.域形成) (void)节点(request.规格.域形成->参数来源, request.G0, request.G0);
        auto first = definitions_.读取首次写入材料(
            {L1所有者范围首次写入读取合同版本, request.幂等身份});
        if (first.读取事实代次 != request.G0) { out.状态 = V::当前性漂移; return out; }
        if (first.状态 == L1所有者范围读取状态::未找到) {
            const auto receipt = 提交(分区::定义, I64基础类型写集(request.G0, request));
            out.首次写入回执 = receipt;
            const auto g = 当前G(); out.事实 = 读取I64基础类型定义事实(receipt, g);
            out.状态 = V::已形成;
        } else if (first.状态 == L1所有者范围读取状态::成功
            && first.首次规范化写集 && first.首次写入结果) {
            const auto expected = I64基础类型写集(first.首次规范化写集->期望事实代次, request);
            if (first.所有者 != definitions_.所有者身份()
                || first.写入幂等身份 != request.幂等身份
                || *first.首次规范化写集 != expected) {
                out.状态 = V::幂等冲突; return out;
            }
            确认发布(分区::定义, expected, *first.首次写入结果);
            out.首次写入回执 = *first.首次写入结果;
            const auto g = 当前G(); out.事实 = 读取I64基础类型定义事实(*first.首次写入结果, g);
            out.状态 = V::已恢复;
        } else {
            out.状态 = first.状态 == L1所有者范围读取状态::资源失败
                ? V::资源失败 : V::已可能发布;
        }
        if (out.成功()) {
            const auto& actual = out.事实->数据;
            const auto& specified = request.规格;
            const bool unitMatches = specified.单位绑定 == I64基础特征单位绑定::新FT自身
                ? actual.规格.单位 == actual.身份.编码
                : specified.既有单位 && actual.规格.单位 == *specified.既有单位;
            if (actual.规格.来源 != specified.来源
                || actual.规格.外设提供者 != specified.外设提供者
                || !unitMatches || actual.规格.缩放分子 != specified.缩放分子
                || actual.规格.缩放分母 != specified.缩放分母
                || actual.规格.允许集合 != specified.允许集合
                || actual.规格.域形成 != specified.域形成) throw S::引用冲突;
        }
    } catch (S s) {
        out.状态 = s == S::入口拒绝 ? V::入口拒绝
            : s == S::并发变化 ? V::当前性漂移
            : s == S::幂等冲突 ? V::幂等冲突
            : s == S::旧格式不支持 ? V::首次材料不一致
            : s == S::已退出 ? V::类型已退出
            : s == S::资源失败 ? V::资源失败
            : s == S::发布结果未确认 ? V::已可能发布
            : s == S::引用冲突 || s == S::类型不相容 ? V::引用冲突
            : V::内部不一致;
    } catch (const std::bad_alloc&) { out.状态 = V::资源失败; }
      catch (const std::length_error&) { out.状态 = V::资源失败; }
      catch (...) { out.状态 = V::内部不一致; }
    if (!out.成功()) {
        out.事实.reset();
        if (out.状态 != V::已可能发布) out.首次写入回执.reset();
    }
    return out;
}

有界准确特征读取结果 特征类数据服务::读取有界准确特征事实(const 有界准确特征读取请求& r) const noexcept {
    有界准确特征读取结果 out; out.原请求=r;
    读取计量 meter; meter.上限=r.预算;
    auto fail=[&](SS state) { out.状态=state; out.事实.reset(); out.用量=meter.用量; };
    try {
        std::lock_guard<std::mutex> lock(mutex_);
        截止有效(r.版本,r.Gread,r.H); 要求(有效(r.身份)&&r.预算.有效(),S::入口拒绝);
        守卫(r.Gread); out.事实=读准确(r.身份,r.Gread,r.H,nullptr,&meter);
        out.用量=meter.用量; out.状态=SS::已读取; 守卫(r.Gread);
        要求(out.成功());
    } catch(S e) { fail(标量映射(e)); }
    catch(const std::bad_alloc&) { fail(SS::资源失败); }
    catch(const std::length_error&) { fail(SS::资源失败); }
    catch(...) { fail(SS::内部不一致); }
    return out;
}

特征数据结果<准确特征读取事实> 特征类数据服务::读取准确特征事实(const 准确特征读取请求& r) const {
    return 保护<准确特征读取事实>([&] {
        截止有效(r.合同版本, r.Gread, r.H); 守卫(r.Gread);
        auto out = 读准确(r.身份, r.Gread, r.H); 守卫(r.Gread); return out;
    });
}
inline 特征类型准确值核验结果
特征类数据服务::核验正式特征类型准确值(
    const 特征类型准确值核验请求& r) const {
    using V = 特征类型准确值核验状态;
    特征类型准确值核验结果 out;
    const auto fail = [&](V state) { out.状态 = state; out.事实.reset(); };
    const auto common = [](S state) noexcept {
        switch (state) {
        case S::入口拒绝: return V::入口拒绝;
        case S::历史材料不可用: return V::历史材料不可用;
        case S::并发变化: return V::事实代次漂移;
        case S::资源失败: return V::资源失败;
        default: return V::内部不一致;
        }
    };
    try {
        std::lock_guard<std::mutex> lock(mutex_);
        截止有效(r.合同版本, r.Gread, r.H);
        要求(有效(r.正式特征类型) && 浅层结构有效(r.准确值), S::入口拒绝);
        守卫(r.Gread);
        I64基础特征类型信息 type;
        try { type = 读类型(r.正式特征类型, r.Gread, r.H); }
        catch (S state) {
            if (state == S::未找到) fail(V::正式特征类型未找到);
            else if (state == S::已退出) fail(V::正式特征类型已退出);
            else if (state == S::入口拒绝) fail(V::内部不一致);
            else fail(common(state));
            return out;
        }
        std::int64_t value = 0;
        try { value = 解析输入(r.准确值, r.Gread, r.H); }
        catch (S state) {
            if (state == S::未找到) fail(V::准确值未找到);
            else if (state == S::已退出) fail(V::准确值已退出);
            else if (state == S::类型不相容 || state == S::能力未提供)
                fail(V::准确值不相容);
            else if (state == S::入口拒绝) fail(V::内部不一致);
            else fail(common(state));
            return out;
        }
        if (!包含(规范域({type.规格.允许集合}), 特征规范I64域{{{value, value}}})) {
            fail(V::准确值不相容); return out;
        }
        守卫(r.Gread);
        out.状态 = V::已核验;
        out.事实 = 特征类型准确值核验事实{
            r.Gread, r.H, r.正式特征类型, r.准确值};
        if (!out.成功() || out.事实->Gread != r.Gread || out.事实->H != r.H
            || out.事实->正式特征类型 != r.正式特征类型
            || out.事实->准确值 != r.准确值)
            fail(V::内部不一致);
    } catch (S state) { fail(common(state)); }
    catch (const std::bad_alloc&) { fail(V::资源失败); }
    catch (const std::length_error&) { fail(V::资源失败); }
    catch (...) { fail(V::内部不一致); }
    return out;
}
特征数据结果<特征信息> 特征类数据服务::读取准确特征(特征信息身份 id) const {
    return 保护<特征信息>([&] { const auto g = 当前G(); auto out = 读准确(id, g, g).信息; 守卫(g); return out; });
}
特征数据结果<特征类型身份> 特征类数据服务::读取准确特征类型(特征信息身份 id) const {
    return 保护<特征类型身份>([&] { const auto g = 当前G(); auto out = 读准确(id, g, g).信息.类型; 守卫(g); return out; });
}
特征数据结果<特征准确值> 特征类数据服务::读取准确特征值(特征信息身份 id) const {
    return 保护<特征准确值>([&] { const auto g = 当前G(); auto out = 读准确(id, g, g).信息.准确值; 守卫(g); return out; });
}
特征数据结果<std::vector<特征信息>> 特征类数据服务::查询准确特征(特征类型身份 type, const 特征准确值& input) const {
    return 保护<std::vector<特征信息>>([&] {
        const auto g = 当前G(); 结构就绪(分区::信息, g, g);
        (void)读类型(type, g, g); const auto value = 解析输入(input, g, g);
        const auto es = 关系(type.编码, f_[准确类型关系], true, g, g, 分区::信息);
        std::vector<特征信息> out;
        for (const auto& e : es) {
            要求(e.角色或顺序 == 1); auto f = 读准确({e.源节点}, g, g);
            if (完整整数(f) == value) out.push_back(std::move(f.信息));
        }
        std::sort(out.begin(), out.end(), [](const auto& a, const auto& b) { return a.身份.编码 < b.身份.编码; });
        守卫(g); return out;
    });
}
特征数据结果<std::monostate> 特征类数据服务::删除准确特征(特征信息身份 id) {
    return 保护<std::monostate>([&] {
        const auto g = 当前G(); const auto f = 读准确(id, g, g); auto ws = 新写集(分区::信息, g);
        ws.退出事实 = {id.编码, f.类型关系, 核对归属(id.编码, g, g, 分区::信息).编码};
        for (const auto& v : 属性(id.编码, g, g, 分区::信息)) ws.退出事实.push_back(v.编码);
        (void)提交(分区::信息, std::move(ws)); return std::monostate{};
    });
}
特征数据结果<I64基础特征类型信息> 特征类数据服务::读取I64基础特征类型(
    特征类型身份 id) const {
    return 保护<I64基础特征类型信息>([&] {
        const auto g = 当前G(); auto out = 读类型(id, g, g); 守卫(g); return out;
    });
}
特征数据结果<特征截止事实<I64基础特征类型信息>>
特征类数据服务::读取I64基础特征类型事实(const 特征类型截止请求& r) const {
    return 保护<特征截止事实<I64基础特征类型信息>>([&] {
        截止有效(r.版本, r.Gread, r.H); 守卫(r.Gread); auto ft = 读类型(r.类型, r.Gread, r.H); 守卫(r.Gread);
        return 特征截止事实<I64基础特征类型信息>{r.Gread, r.H, std::move(ft)};
    });
}
特征数据结果<特征截止事实<特征规范I64域>> 特征类数据服务::读取I64类型完整域(const 特征类型截止请求& r) const {
    return 保护<特征截止事实<特征规范I64域>>([&] {
        截止有效(r.版本, r.Gread, r.H); 守卫(r.Gread);
        auto domain = 读完整域(r.类型, r.Gread, r.H); 守卫(r.Gread);
        return 特征截止事实<特征规范I64域>{r.Gread, r.H, std::move(domain)};
    });
}
特征数据结果<特征域形成事实> 特征类数据服务::形成I64特征域(const 准确特征读取请求& r) const {
    return 保护<特征域形成事实>([&] { return 形成I64特征域已持锁(r); });
}

特征域形成事实 特征类数据服务::形成I64特征域已持锁(const 准确特征读取请求& r) const {
    截止有效(r.合同版本, r.Gread, r.H); 守卫(r.Gread);
    const auto f = 读准确(r.身份, r.Gread, r.H); const auto ft = 读类型(f.信息.类型, r.Gread, r.H);
    要求(ft.规格.域形成 && ft.规则, S::规则缺失);
    const auto v = 完整整数(f), error = ft.规格.域形成->允许误差;
    要求(v >= std::numeric_limits<std::int64_t>::min() + error
        && v <= std::numeric_limits<std::int64_t>::max() - error, S::算术不可表示);
    特征规范I64域 domain{{{v - error, v + error}}};
    要求(包含(规范域({ft.规格.允许集合}), domain), S::类型不相容); 守卫(r.Gread);
    return {ft.身份, std::move(domain), r.身份, *ft.规则, r.Gread, r.H};
}
特征数据结果<特征截止事实<特征规范I64域>> 特征类数据服务::规范化I64特征域(const 特征I64域判定请求& r) const {
    return 保护<特征截止事实<特征规范I64域>>([&] {
        截止有效(r.类型.版本, r.类型.Gread, r.类型.H); 守卫(r.类型.Gread);
        const auto full = 读完整域(r.类型.类型, r.类型.Gread, r.类型.H);
        auto domain = 规范域(r.域); 要求(包含(full, domain), S::类型不相容); 守卫(r.类型.Gread);
        return 特征截止事实<特征规范I64域>{r.类型.Gread, r.类型.H, std::move(domain)};
    });
}
特征数据结果<特征截止事实<bool>> 特征类数据服务::判定准确特征命中域(const 准确特征域命中请求& r) const {
    return 保护<特征截止事实<bool>>([&] {
        截止有效(r.特征.合同版本, r.特征.Gread, r.特征.H); 守卫(r.特征.Gread);
        const auto f = 读准确(r.特征.身份, r.特征.Gread, r.特征.H);
        const auto full = 规范域({读类型(f.信息.类型, r.特征.Gread, r.特征.H).规格.允许集合});
        const auto domain = 规范域(r.域); 要求(包含(full, domain), S::类型不相容);
        const auto value = 完整整数(f); const auto hit = 包含(domain, 特征规范I64域{{{value, value}}}); 守卫(r.特征.Gread);
        return 特征截止事实<bool>{r.特征.Gread, r.特征.H, hit};
    });
}
特征数据结果<特征截止事实<bool>> 特征类数据服务::判定I64域包含(const 特征I64域包含请求& r) const {
    return 保护<特征截止事实<bool>>([&] {
        截止有效(r.类型.版本, r.类型.Gread, r.类型.H); 守卫(r.类型.Gread);
        const auto full = 读完整域(r.类型.类型, r.类型.Gread, r.类型.H);
        const auto outer = 规范域(r.外), inner = 规范域(r.内);
        要求(包含(full, outer) && 包含(full, inner), S::类型不相容);
        const auto hit = 包含(outer, inner); 守卫(r.类型.Gread);
        return 特征截止事实<bool>{r.类型.Gread, r.类型.H, hit};
    });
}

特征类数据服务::分区 特征类数据服务::旧路由(std::uint64_t g, std::uint64_t h) const {
    标量要求(legacy_.has_value(), SS::格式不支持);
    const auto& t = *legacy_;
    const std::array<稳定编码, 7> ids{t.实际阶次属性类型, t.派生规则属性类型,
        t.直接来源关系类型, t.宿主关系类型, t.比较注册归属关系类型,
        t.比较注册U64属性类型, t.比较注册I64属性类型};
    标量要求(t.格式 == 1, SS::入口拒绝);
    std::set<稳定编码> seen;
    std::optional<L1结构所有者身份> owner;
    for (std::size_t i = 0; i < ids.size(); ++i) {
        标量要求(有效(ids[i]) && seen.insert(ids[i]).second, SS::入口拒绝);
        const auto raw = 原始事实(ids[i], g); const auto* n = std::get_if<N>(&raw);
        标量要求(n && n->编码 == ids[i], SS::格式不支持);
        const auto rep = i == 0 ? std::optional{L1所有者范围值表示种类::I64}
            : i == 1 || i == 5 ? std::optional{L1所有者范围值表示种类::U64组}
            : i == 6 ? std::optional{L1所有者范围值表示种类::I64组} : std::nullopt;
        标量要求(n->种类 == (rep ? 节点种类::属性类型 : 节点种类::普通)
            && n->属性类型表示 == rep && n->当前属性.empty(), SS::格式不支持);
        if (h) 生命周期(*n, g, h);
        if (!owner) owner = n->写入所有者;
        标量要求(n->写入所有者 == *owner, SS::来源不匹配);
    }
    const bool a = *owner == definitions_.所有者身份(), b = *owner == information_.所有者身份();
    标量要求(a != b && 绑定于(l1_), SS::格式不支持);
    return a ? 分区::定义 : 分区::信息;
}
特征类数据服务::旧治理布局 特征类数据服务::旧读取闭包(
    特征类定义身份 root, std::uint64_t g, std::uint64_t h, 分区 p,
    const 特征类派生读取预算& budget) const {
    标量要求(budget.有效() && 有效(root.结点) && h && g >= h, SS::入口拒绝);
    标量要求(旧路由(g, h) == p, SS::来源不匹配);
    const auto& t = *legacy_;
    std::set<稳定编码> rels, vals, leaves, defs;
    auto rawnode = [&](稳定编码 id, bool own) {
        const auto raw = 原始事实(id, g); const auto* n = std::get_if<N>(&raw);
        标量要求(n && n->编码 == id, SS::格式不支持);
        if (own) 标量要求(n->写入所有者 == 端口(p).所有者身份(), SS::来源不匹配);
        生命周期(*n, g, h); return *n;
    };
    auto readattrs = [&](const N& n) {
        const auto result = l1_.读取所有者范围历史属性值组({L1所有者范围CRUD合同版本, n.编码, h});
        标量要求(result.读取事实代次 == g, SS::事实代次漂移);
        标量要求(result.状态 == L1所有者范围读取状态::成功, 标量映射(result.状态));
        标量要求(result.合同版本 == L1所有者范围CRUD合同版本 && result.所属节点 == n.编码
            && result.历史截止事实代次 == h);
        auto vs = result.属性值组; std::set<稳定编码> slots;
        for (const auto& v : vs) {
            标量计数(vals, v.编码, budget.最大属性值数);
            生命周期(v, g, h);
            标量要求(有效(v.编码) && v.写入所有者 == n.写入所有者 && v.所属节点 == n.编码
                && slots.insert(v.属性类型节点).second && L1所有者范围原始材料完整(v.材料)
                && v.来源节点 == n.编码 && v.创建事实代次 == n.创建事实代次
                && v.退出事实代次 == n.退出事实代次, SS::来源不匹配);
        }
        return vs;
    };
    auto readrels = [&](const N& n, 稳定编码 type) {
        const auto result = l1_.读取所有者范围历史关系组(
            {L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::源, n.编码, type, h});
        标量要求(result.读取事实代次 == g, SS::事实代次漂移);
        标量要求(result.状态 == L1所有者范围读取状态::成功, 标量映射(result.状态));
        标量要求(result.合同版本 == L1所有者范围CRUD合同版本 && result.方向 == L1所有者范围关系端点方向::源
            && result.端点节点 == n.编码 && result.关系类型节点 == type && result.历史截止事实代次 == h);
        auto es = result.关系组; std::set<稳定编码> unique;
        for (const auto& e : es) {
            标量计数(rels, e.编码, budget.最大关系数);
            生命周期(e, g, h);
            标量要求(有效(e.编码) && unique.insert(e.编码).second && e.写入所有者 == n.写入所有者
                && e.源节点 == n.编码 && e.关系类型节点 == type && 有效(e.目标节点)
                && e.创建事实代次 == n.创建事实代次
                && e.退出事实代次 == n.退出事实代次, SS::来源不匹配);
        }
        std::sort(es.begin(), es.end(), [](const auto& a, const auto& b) {
            return a.角色或顺序 != b.角色或顺序 ? a.角色或顺序 < b.角色或顺序 : a.编码 < b.编码;
        });
        return es;
    };
    auto parse = [&](稳定编码 id) {
        旧治理布局 o;
        const auto n = rawnode(id, id == root.结点);
        标量要求(n.种类 == 节点种类::普通 && !n.属性类型表示, SS::格式不支持);
        auto vs = readattrs(n);
        auto sources = readrels(n, t.直接来源关系类型), host = readrels(n, t.宿主关系类型),
            reg = readrels(n, t.比较注册归属关系类型);
        o.事实.身份 = {id}; o.事实.创建G = n.创建事实代次; o.事实.退出G = n.退出事实代次;
        const bool any = std::any_of(vs.begin(), vs.end(), [&](const auto& v) {
            return v.属性类型节点 == t.实际阶次属性类型 || v.属性类型节点 == t.派生规则属性类型;
        }) || !sources.empty() || !host.empty() || !reg.empty();
        // 根只能是旧派生定义；注册节点、基础叶和其它退出集合成员不是同一业务目标。
        if(id==root.结点)标量要求(any
            &&std::count_if(vs.begin(),vs.end(),[&](const auto& v){return v.属性类型节点==t.实际阶次属性类型;})==1
            &&std::count_if(vs.begin(),vs.end(),[&](const auto& v){return v.属性类型节点==t.派生规则属性类型;})==1,
            SS::格式不支持);
        if (!any) {
            标量计数(leaves, id, budget.最大叶数);
            标量要求(vs.size() == 1, SS::格式不支持);
            const auto ft = rawnode(vs.front().属性类型节点, false);
            const auto rep = std::visit([](const auto& value) {
                using T = std::decay_t<decltype(value)>;
                if constexpr (std::is_same_v<T, std::int64_t>) return L1所有者范围值表示种类::I64;
                else if constexpr (std::is_same_v<T, std::vector<std::int64_t>>) return L1所有者范围值表示种类::I64组;
                else if constexpr (std::is_same_v<T, std::vector<std::uint64_t>>) return L1所有者范围值表示种类::U64组;
                else return L1所有者范围值表示种类::独立材料引用;
            }, vs.front().材料);
            标量要求(ft.种类 == 节点种类::属性类型 && ft.属性类型表示
                && ft.创建事实代次 <= n.创建事实代次
                && *ft.属性类型表示 == rep, SS::格式不支持);
            o.事实.实际阶次 = 1; return o; // 仅旧布局的治理证明，不产生新准确 F 或计算值。
        }
        标量计数(defs, id, budget.最大定义数);
        标量要求(vs.size() == 2 && sources.size() == 2 && host.size() <= 1 && reg.size() == 1, SS::格式不支持);
        const auto& order = 唯一属性(vs, t.实际阶次属性类型);
        const auto& rule = 唯一属性(vs, t.派生规则属性类型);
        const auto* k = std::get_if<std::int64_t>(&order.材料);
        const auto* rulebits = std::get_if<std::vector<std::uint64_t>>(&rule.材料);
        标量要求(k && *k > 1 && static_cast<std::uint64_t>(*k) <= UINT32_MAX
            && rulebits && rulebits->size() == 2 && (*rulebits)[0] && (*rulebits)[1]
            && (*rulebits)[1] <= UINT32_MAX, SS::格式不支持);
        o.事实.实际阶次 = static_cast<std::uint32_t>(*k);
        (void)rawnode({(*rulebits)[0]}, false);
        标量要求(reg.front().角色或顺序 == 1, SS::来源不匹配);
        const auto rn = rawnode(reg.front().目标节点, false);
        标量要求(rn.种类 == 节点种类::普通 && !rn.属性类型表示
            && rn.写入所有者 == n.写入所有者
            && rn.创建事实代次 == n.创建事实代次 && rn.退出事实代次 == n.退出事实代次, SS::来源不匹配);
        const auto rvs = readattrs(rn);
        标量要求(rvs.size() == 2, SS::格式不支持);
        const auto& uv = 唯一属性(rvs, t.比较注册U64属性类型);
        const auto& iv = 唯一属性(rvs, t.比较注册I64属性类型);
        const auto* u = std::get_if<std::vector<std::uint64_t>>(&uv.材料);
        const auto* v = std::get_if<std::vector<std::int64_t>>(&iv.材料);
        标量要求(u && v && u->size() == 16 && v->size() == 4, SS::格式不支持);
        标量要求((*u)[0] == 1 && (*u)[2] == 2 && (*u)[4] == 1 && (*u)[7] == 1
            && (*u)[12] == 1 && (*u)[13] == 1 && (*u)[1] >= 1 && (*u)[1] <= 2
            && (*u)[3] && (*u)[3] <= UINT32_MAX && (*u)[3] == (*rulebits)[1]
            && (*u)[8] && (*u)[9] && (*u)[10] && (*u)[11] >= 1 && (*u)[11] <= 7
            && (*u)[14] && (*u)[14] <= UINT32_MAX && (*u)[15] == (*rulebits)[0], SS::格式不支持);
        for (std::size_t i : {std::size_t{0}, std::size_t{2}})
            标量要求(((*v)[i] == 0 && (*v)[i + 1] == 0)
                || ((*v)[i] == 1 && (*v)[i + 1] >= 0), SS::格式不支持);
        const bool goal = (*u)[1] == 1;
        标量要求((*u)[5] == (goal ? 1 : 3) && (*u)[6] == (goal ? 2 : 4), SS::来源不匹配);
        o.事实.用途 = static_cast<特征类比较用途>((*u)[1]);
        o.事实.左角色 = static_cast<特征类比较角色>((*u)[5]);
        o.事实.右角色 = static_cast<特征类比较角色>((*u)[6]);
        for (std::size_t i : {std::size_t{8}, std::size_t{9}, std::size_t{10}}) (void)rawnode({(*u)[i]}, false);
        o.自有 = {id, rn.编码, order.编码, rule.编码, uv.编码, iv.编码, reg.front().编码};
        for (std::size_t i = 0; i < sources.size(); ++i) {
            const auto& e = sources[i];
            const auto encoded = static_cast<std::uint64_t>(e.角色或顺序);
            标量要求((encoded >> 32) == i + 1 && static_cast<std::uint32_t>(encoded) == (*u)[5 + i],
                SS::来源不匹配);
            o.来源.push_back(e.目标节点); o.自有.push_back(e.编码);
        }
        标量要求(o.来源[0] != o.来源[1], SS::来源不匹配);
        if (!host.empty()) {
            标量要求(host.front().角色或顺序 == 1, SS::来源不匹配);
            (void)rawnode(host.front().目标节点, false);
            o.事实.宿主 = host.front().目标节点; o.自有.push_back(host.front().编码);
        }
        std::sort(o.自有.begin(), o.自有.end());
        标量要求(o.自有.size() == 9 + host.size()
            && std::adjacent_find(o.自有.begin(), o.自有.end()) == o.自有.end());
        return o;
    };
    struct Frame { 稳定编码 id; std::uint64_t depth; bool after; };
    std::vector<Frame> stack{{root.结点, 1, false}};
    std::map<稳定编码, 旧治理布局> nodes; std::set<稳定编码> active, done;
    std::map<稳定编码, std::uint64_t> heights;
    while (!stack.empty()) {
        const auto f = stack.back(); stack.pop_back();
        标量要求(f.depth <= budget.最大深度, SS::预算不足);
        if (done.contains(f.id)) {
            标量要求(heights.at(f.id) <= budget.最大深度 - f.depth + 1, SS::预算不足);
            continue;
        }
        if (f.after) {
            auto& n = nodes.at(f.id); std::uint32_t max = 0; std::uint64_t height = 1;
            for (auto src : n.来源) {
                标量要求(done.contains(src), SS::内部不一致);
                max = std::max(max, nodes.at(src).事实.实际阶次);
                标量要求(heights.at(src) < budget.最大深度, SS::预算不足);
                height = std::max(height, heights.at(src) + 1);
            }
            if (!n.来源.empty()) 标量要求(max != UINT32_MAX && n.事实.实际阶次 == max + 1, SS::来源不匹配);
            标量要求(height <= budget.最大深度 - f.depth + 1, SS::预算不足);
            heights.emplace(f.id, height);
            active.erase(f.id); done.insert(f.id); continue;
        }
        标量要求(!active.contains(f.id), SS::来源不匹配);
        auto [it, inserted] = nodes.emplace(f.id, parse(f.id)); 标量要求(inserted);
        active.insert(f.id); stack.push_back({f.id, f.depth, true});
        if (!it->second.来源.empty()) 标量要求(f.depth < budget.最大深度, SS::预算不足);
        for (auto i = it->second.来源.rbegin(); i != it->second.来源.rend(); ++i)
            stack.push_back({*i, f.depth + 1, false});
    }
    auto out = nodes.at(root.结点); 标量要求(out.事实.实际阶次 > 1, SS::格式不支持);
    标量守卫(g); return out;
}
旧派生治理读取结果 特征类数据服务::读取旧派生治理事实(
    const 特征类标量派生读取请求& r) const {
    旧派生治理读取结果 o; o.Gread = r.Gread; o.H = r.H;
    try {
        std::lock_guard<std::mutex> lock(mutex_);
        标量要求(r.版本 == 2 && r.H && r.H <= r.Gread && 有效(r.定义身份.结点) && r.预算.有效(), SS::入口拒绝);
        标量守卫(r.Gread);
        o.事实 = 旧读取闭包(r.定义身份, r.Gread, r.H, 旧路由(r.Gread, r.H), r.预算).事实;
        标量守卫(r.Gread); o.状态 = SS::已读取; 标量要求(o.成功()); return o;
    } catch (const 标量失败& e) { o.状态 = e.状态; }
    catch (S e) { o.状态 = 标量映射(e); }
    catch (const std::bad_alloc&) { o.状态 = SS::资源失败; }
    catch (const std::length_error&) { o.状态 = SS::资源失败; }
    catch (...) { o.状态 = SS::内部不一致; }
    o.事实.reset(); return o;
}
特征类标量派生写结果 特征类数据服务::退出旧派生治理定义(
    const 特征类标量派生退出请求& r) {
    特征类标量派生写结果 o;bool dispatched=false,prior=false,keyUnknown=false,ownsRecovery=false;
    std::lock_guard<std::mutex> lock(mutex_);
    try {
        标量要求(r.版本 == 2 && r.G && 有效(r.定义身份.结点) && 有效(r.幂等身份)
            && r.预算.有效(), SS::入口拒绝);
        o.旧治理原请求 = r; o.Gread = 标量当前G();
        const auto p = 旧路由(o.Gread, 0);
        标量要求(!binding_pending_&&(!pending_ || (pending_->旧治理 && *pending_->旧治理 == r && pending_->区 == p)),
            SS::已可能发布);
        if(scalar_pending_) {
            const auto* original=std::get_if<特征类标量派生退出请求>(&scalar_pending_->原请求);
            标量要求(scalar_pending_->旧治理&&original&&*original==r,SS::已可能发布);
        } else scalar_pending_.emplace(待确认标量业务{true,r});
        ownsRecovery=true;keyUnknown=true;
        const auto first = 端口(p).读取首次写入材料({L1所有者范围首次写入读取合同版本, r.幂等身份});
        标量要求(first.读取事实代次 == o.Gread, SS::事实代次漂移);
        标量要求(first.合同版本 == L1所有者范围首次写入读取合同版本
            && first.所有者 == 端口(p).所有者身份() && first.写入幂等身份 == r.幂等身份);
        WS ws; ws.合同版本 = L1所有者范围CRUD合同版本; ws.期望事实代次 = r.G; ws.写入幂等身份 = r.幂等身份;
        if (first.状态 == L1所有者范围读取状态::成功) {
            prior=true;keyUnknown=false;
            标量要求(first.首次规范化写集 && first.首次写入结果);
            const auto& recorded = *first.首次规范化写集;
            标量要求(recorded.合同版本 == L1所有者范围CRUD合同版本 && recorded.期望事实代次 == r.G
                && recorded.写入幂等身份 == r.幂等身份 && recorded.节点.empty() && recorded.关系.empty()
                && recorded.值.empty() && recorded.属性槽变更.empty()
                && (recorded.退出事实.size() == 9 || recorded.退出事实.size() == 10)
                && std::count(recorded.退出事实.begin(), recorded.退出事实.end(), r.定义身份.结点) == 1,
                SS::幂等冲突);
            const auto& saved = *first.首次写入结果;
            o.正式回执 = saved; if (saved.事实代次) o.首次发布H = saved.事实代次;
            标量要求(saved.合同版本 == L1所有者范围CRUD合同版本 && saved.所有者 == 端口(p).所有者身份()
                && saved.写入幂等身份 == r.幂等身份 && saved.状态 == L1所有者范围写入状态::成功
                && saved.是否形成内存权威发布 && saved.事实代次 > r.G && saved.事实代次 <= o.Gread);
            try {
                ws.退出事实 = 旧读取闭包(r.定义身份, o.Gread, r.G, p, r.预算).自有;
            } catch (const 标量失败& e) {
                // 退出集合内的注册等节点不是原目标；已绑定键的确定结构不符就是键冲突。
                if (e.状态 == SS::格式不支持 || e.状态 == SS::来源不匹配
                    || e.状态 == SS::未找到 || e.状态 == SS::已退出) throw 标量失败{SS::幂等冲突};
                throw;
            }
            标量要求(ws == *first.首次规范化写集 && saved.新编码映射.empty(), SS::幂等冲突);
        } else if (first.状态 == L1所有者范围读取状态::未找到) {
            标量要求(!first.首次规范化写集&&!first.首次写入结果);
            keyUnknown=false;
            标量守卫(r.G);
            ws.退出事实 = 旧读取闭包(r.定义身份, r.G, r.G, p, r.预算).自有;
        } else throw 标量失败{标量映射(first.状态)};
        if (pending_) 标量要求(pending_->请求 == ws, SS::幂等冲突);
        else pending_.emplace(待确认写入{p, ws, false, r});
        dispatched = true;
        const auto receipt = 端口(p).提交所有者范围中性写集(ws); o.正式回执 = receipt;
        const bool ok = receipt.状态 == L1所有者范围写入状态::成功
            || receipt.状态 == L1所有者范围写入状态::精确重复;
        if (!ok) {
            if (!prior && !receipt.是否形成内存权威发布
                && (receipt.状态 == L1所有者范围写入状态::事实代次漂移
                    || receipt.状态 == L1所有者范围写入状态::引用冲突
                    || receipt.状态 == L1所有者范围写入状态::入口拒绝)) {
                pending_.reset(); dispatched = false; o.发布确定性 = SP::确认未发布;
                throw 标量失败{标量映射(映射(receipt.状态))};
            }
            throw 标量失败{SS::已可能发布};
        }
        o.首次发布H = receipt.事实代次;
        标量要求(receipt.合同版本 == L1所有者范围CRUD合同版本 && receipt.所有者 == 端口(p).所有者身份()
            && receipt.写入幂等身份 == r.幂等身份 && receipt.事实代次 > r.G
            && receipt.新编码映射.empty() && (!prior || receipt.状态 == L1所有者范围写入状态::精确重复));
        o.Gread = 标量当前G();
        const auto after = 旧读取闭包(r.定义身份, o.Gread, r.G, p, r.预算);
        标量要求(after.自有 == ws.退出事实 && after.事实.退出G == o.首次发布H);
        for (auto id : ws.退出事实) {
            const auto fact = 原始事实(id, o.Gread);
            std::visit([&](const auto& f) {
                标量要求(f.写入所有者 == 端口(p).所有者身份() && f.退出事实代次 == o.首次发布H);
            }, fact);
        }
        标量守卫(o.Gread);
        o.旧治理退出 = 旧派生治理退出事实{after.事实, ws.退出事实};
        o.发布确定性 = SP::确认已发布;
        o.状态 = receipt.状态 == L1所有者范围写入状态::成功 ? SS::已退出 : SS::精确重复;
        标量要求(o.成功()); pending_.reset();scalar_pending_.reset();return o;
    } catch (const 标量失败& e) { o.状态 = e.状态; }
    catch (S e) { o.状态 = 标量映射(e); }
    catch (const std::bad_alloc&) { o.状态 = SS::资源失败; }
    catch (const std::length_error&) { o.状态 = SS::资源失败; }
    catch (...) { o.状态 = SS::内部不一致; }
    if(o.状态==SS::幂等冲突) {
        o.发布确定性=SP::确认未发布;
        if(ownsRecovery){pending_.reset();scalar_pending_.reset();}
    } else if (dispatched || prior || keyUnknown) {
        o.状态=SS::已可能发布;o.发布确定性=SP::可能已发布;
    } else if(ownsRecovery){pending_.reset();scalar_pending_.reset();}
    o.旧治理退出.reset(); o.定义事实.reset(); return o;
}


template<class BindingRequest> 特征I64比较绑定结果 特征类数据服务::绑定执行写(const BindingRequest& r) {
    constexpr bool create=std::is_same_v<BindingRequest,特征I64比较绑定建立请求>;
    特征I64比较绑定结果 out;out.操作=create ? 特征I64比较绑定操作::建立 : 特征I64比较绑定操作::退出;
    std::lock_guard<std::mutex> lock(mutex_);
    bool dispatched=false,knownFirst=false,ownsPending=false;
    auto fail=[&](KS state){
        out.事实.reset();
        if(state==KS::幂等冲突||state==KS::注册不唯一||state==KS::目标已退出){
            out.状态=state;out.首次发布H.reset();out.正式回执.reset();out.发布确定性=SP::确认未发布;
            if(ownsPending)binding_pending_.reset();
        }else if(dispatched||knownFirst||state==KS::已可能发布){
            out.状态=KS::已可能发布;out.发布确定性=SP::可能已发布;
        }else{
            out.状态=state;out.首次发布H.reset();out.正式回执.reset();out.发布确定性=SP::确认未发布;
            if(ownsPending)binding_pending_.reset();
        }
        return std::move(out);
    };
    try{
        if constexpr(create)out.建立原请求=r;
        绑定要求(r.版本==1&&r.G&&r.G<UINT64_MAX&&有效(r.幂等身份),KS::入口拒绝);
        if constexpr(create)绑定要求(I64绑定定义完整(r.定义),KS::入口拒绝);
        else 绑定要求(有效(r.身份),KS::入口拒绝);
        绑定就绪();
        绑定要求(!pending_&&!scalar_pending_,KS::已可能发布);
        if(binding_pending_){
            const auto* original=std::get_if<BindingRequest>(&binding_pending_->原请求);
            if(!original||original->幂等身份!=r.幂等身份)throw 绑定失败{KS::已可能发布};
            绑定要求(*original==r,KS::幂等冲突);
            ownsPending=true;dispatched=true;
        }
        out.Gread=当前G();结构就绪(分区::定义,out.Gread,out.Gread);
        auto first=definitions_.读取首次写入材料({L1所有者范围首次写入读取合同版本,r.幂等身份});
        WS w;
        if constexpr(create)w=绑定建立写集(r);
        auto firstHead=[&]{
            绑定要求(first.合同版本==L1所有者范围首次写入读取合同版本
                &&first.所有者==definitions_.所有者身份()&&first.写入幂等身份==r.幂等身份);
        };
        auto recover=[&](KS status)->特征I64比较绑定结果{
            firstHead();
            绑定要求(first.状态==L1所有者范围读取状态::成功&&first.首次规范化写集&&first.首次写入结果);
            if(!binding_pending_){
                binding_pending_.emplace(待确认I64绑定业务{r,*first.首次规范化写集});ownsPending=true;
            }
            knownFirst=true;
            const auto& saved=*first.首次写入结果;
            绑定要求(first.首次规范化写集->期望事实代次==r.G,KS::幂等冲突);
            out.Gread=当前G();绑定要求(first.读取事实代次<=out.Gread&&first.读取事实代次!=0);
            读取计量 meter;
            if constexpr(!create)w=绑定退出写集(r,out.Gread,meter);
            绑定要求(w==*first.首次规范化写集,KS::幂等冲突);
            if(binding_pending_)绑定要求(binding_pending_->写集==w,KS::幂等冲突);
            绑定要求(写入结果头完整(saved,r.幂等身份)&&saved.状态==L1所有者范围写入状态::成功
                &&saved.事实代次>r.G&&saved.事实代次<=out.Gread);
            out.首次发布H=out.H=saved.事实代次;out.正式回执=saved;
            确认发布(分区::定义,w,saved);
            out.Gread=当前G();
            if constexpr(create){
                const auto identity=标量查找编码(saved,Key{1});绑定要求(identity.has_value());
                const auto raw=原始事实(*identity,out.Gread,&meter);const auto* node=std::get_if<N>(&raw);
                绑定要求(node&&node->创建事实代次==out.H);
                绑定要求(!node->退出事实代次,KS::目标已退出);
                out.事实=读绑定({*identity},out.Gread,out.Gread,meter);
                绑定要求(out.事实->定义==r.定义&&out.事实->创建G==out.H);
                for(const auto& output:out.事实->输出组)
                    绑定要求(标量查找编码(saved,Key{0x200+static_cast<std::uint32_t>(output.输出.角色)})==output.输出关系);
            }else{
                绑定要求(saved.新编码映射.empty());
                out.事实=读绑定(r.身份,out.Gread,r.G,meter);
                绑定要求(out.事实->退出G==out.H);
            }
            守卫(out.Gread);out.状态=status;out.发布确定性=SP::确认已发布;
            绑定要求(out.成功());if(ownsPending)binding_pending_.reset();return std::move(out);
        };
        if(first.状态==L1所有者范围读取状态::成功)return recover(KS::精确重复);
        if(first.状态!=L1所有者范围读取状态::未找到)throw 绑定失败{绑定映射(映射(first.状态))};
        firstHead();绑定要求(!first.首次规范化写集&&!first.首次写入结果);dispatched=false;
        守卫(r.G);out.Gread=r.G;读取计量 meter;
        if constexpr(create){
            const auto route=读当前绑定({1,r.G,r.定义.输入FT,r.定义.用途,UINT64_MAX,UINT64_MAX});
            if(route.状态==KS::已读取){
                绑定要求(route.成功());
                throw 绑定失败{route.事实->定义==r.定义 ? KS::幂等冲突 : KS::注册不唯一};
            }
            if(route.状态!=KS::未找到)throw 绑定失败{route.状态};
            核验绑定FT(r.定义.输入FT,r.定义.输入量化,r.G,r.G,meter);
            for(const auto& output:r.定义.输出组)核验绑定FT(output.输出FT,output.输出.量化,r.G,r.G,meter);
        }else w=绑定退出写集(r,r.G,meter);
        守卫(r.G);
        if(binding_pending_)绑定要求(binding_pending_->写集==w,KS::幂等冲突);
        else{binding_pending_.emplace(待确认I64绑定业务{r,w});ownsPending=true;}
        dispatched=true;
        const auto committed=definitions_.提交所有者范围中性写集(w);
        first=definitions_.读取首次写入材料({L1所有者范围首次写入读取合同版本,r.幂等身份});
        if(first.状态==L1所有者范围读取状态::成功)
            return recover(committed.状态==L1所有者范围写入状态::成功 ? (create ? KS::已创建 : KS::已退出) : KS::精确重复);
        if(first.状态!=L1所有者范围读取状态::未找到)throw 绑定失败{KS::已可能发布};
        firstHead();绑定要求(!first.首次规范化写集&&!first.首次写入结果);
        const bool zero=!committed.是否形成内存权威发布
            &&(committed.状态==L1所有者范围写入状态::入口拒绝||committed.状态==L1所有者范围写入状态::未找到
                ||committed.状态==L1所有者范围写入状态::已退出||committed.状态==L1所有者范围写入状态::引用冲突
                ||committed.状态==L1所有者范围写入状态::事实代次漂移||committed.状态==L1所有者范围写入状态::幂等冲突);
        if(!zero)throw 绑定失败{KS::已可能发布};
        dispatched=false;const auto original=绑定映射(映射(committed.状态));
        if constexpr(create){
            // 不同原键竞争只进行一次最新 G 的唯一路由重扫，绝不换键再提交。
            const auto current=当前G();out.Gread=current;
            const auto route=读当前绑定({1,current,r.定义.输入FT,r.定义.用途,UINT64_MAX,UINT64_MAX});
            if(route.状态==KS::已读取){
                绑定要求(route.成功());
                throw 绑定失败{route.事实->定义==r.定义 ? KS::幂等冲突 : KS::注册不唯一};
            }
            if(route.状态!=KS::未找到)throw 绑定失败{route.状态};
        }
        throw 绑定失败{original};
    }catch(const 绑定失败& e){return fail(e.状态);}
    catch(S e){return fail(绑定映射(e));}
    catch(const 标量失败& e){return fail(绑定标量映射(e.状态));}
    catch(const std::bad_alloc&){return fail(KS::资源失败);}
    catch(const std::length_error&){return fail(KS::资源失败);}
    catch(...){return fail(KS::内部不一致);}
}
特征I64比较绑定结果 特征类数据服务::建立I64比较绑定(const 特征I64比较绑定建立请求& r){return 绑定执行写(r);}
特征I64比较绑定结果 特征类数据服务::退出I64比较绑定(const 特征I64比较绑定退出请求& r){return 绑定执行写(r);}


bool 特征类标量派生批量读取结果::成功() const noexcept {
    try{
        const auto& r=原请求;const auto& b=r.预算;const auto& u=用量;
        if(版本!=1||r.版本!=1||状态!=特征类标量状态::已读取||!H||H>Gread
            ||Gread!=r.Gread||H!=r.H||!b.有效()||r.根定义组.empty()||根回执组.size()!=r.根定义组.size()
            ||完整定义组.empty()||基础叶组.empty()||u.定义数!=完整定义组.size()||u.叶数!=基础叶组.size()
            ||!u.关系数||!u.属性值数||!u.最大深度||!u.材料总数
            ||u.定义数>b.最大定义数||u.关系数>b.最大关系数||u.叶数>b.最大叶数
            ||u.属性值数>b.最大属性值数||u.最大深度>b.最大深度||u.材料总数>b.最大材料总数
            ||u.定义数>UINT64_MAX-u.关系数||u.定义数+u.关系数>UINT64_MAX-u.叶数
            ||u.定义数+u.关系数+u.叶数>UINT64_MAX-u.属性值数
            ||u.材料总数<u.定义数+u.关系数+u.叶数+u.属性值数)return false;
        std::map<稳定编码,const 特征类标量派生事实*> definitions;
        std::map<稳定编码,const 特征类标量叶回执*> leaves;
        std::vector<const 特征类标量派生事实*> order;
        稳定编码 previous{};
        for(const auto& d:完整定义组){
            if(!d.完整(H)||(有效(previous)&&!(previous<d.定义身份.结点)))return false;
            previous=d.定义身份.结点;definitions.emplace(previous,&d);order.push_back(&d);
        }
        previous={};
        for(const auto& f:基础叶组){
            if(!标量叶完整(f,Gread,H)||(有效(previous)&&!(previous<f.F))||definitions.contains(f.F))return false;
            previous=f.F;leaves.emplace(f.F,&f);
        }
        std::sort(order.begin(),order.end(),[](const auto* a,const auto* z){return a->真实阶次<z->真实阶次;});
        std::map<稳定编码,std::set<稳定编码>> closure,descendants;
        std::map<稳定编码,std::uint64_t> height;
        for(const auto& [id,leaf]:leaves){closure[id]={id};height[id]=1;}
        for(const auto* d:order){
            auto& accumulated=closure[d->定义身份.结点];auto& children=descendants[d->定义身份.结点];
            children.insert(d->定义身份.结点);std::uint64_t maximumDepth=0;std::uint32_t maximumOrder=1;
            for(const auto& s:d->来源组){
                const auto id=标量来源编码(s.内容.来源);
                if(const auto* derived=std::get_if<特征类标量派生来源>(&s.内容.来源)){
                    const auto it=definitions.find(id);
                    if(it==definitions.end()||it->second->真实阶次>=d->真实阶次
                        ||std::none_of(it->second->输出组.begin(),it->second->输出组.end(),
                            [&](const auto& output){return output.声明.角色==derived->上游输出角色;}))return false;
                    maximumOrder=std::max(maximumOrder,it->second->真实阶次);
                    const auto& seen=descendants.at(id);children.insert(seen.begin(),seen.end());
                }else if(!leaves.contains(id))return false;
                const auto found=closure.find(id);if(found==closure.end())return false;
                accumulated.insert(found->second.begin(),found->second.end());
                maximumDepth=std::max(maximumDepth,height.at(id));
            }
            if(maximumOrder==UINT32_MAX||d->真实阶次!=maximumOrder+1||maximumDepth==UINT64_MAX)return false;
            height[d->定义身份.结点]=maximumDepth+1;
        }
        previous={};std::set<稳定编码> allLeaves,allDefinitions;std::uint64_t maximumDepth=0;
        for(std::size_t i=0;i<r.根定义组.size();++i){
            const auto id=r.根定义组[i].结点;const auto& root=根回执组[i];
            if(!有效(id)||(有效(previous)&&!(previous<id))||root.定义身份!=r.根定义组[i]
                ||!definitions.contains(id))return false;
            previous=id;const auto& d=*definitions.at(id);
            for(unsigned side=0;side<2;++side){
                const auto& actual=side ? root.右叶组 : root.左叶组;
                const auto& expected=closure.at(标量来源编码(d.来源组[side].内容.来源));
                if(actual.empty()||actual.size()!=expected.size()||!std::equal(actual.begin(),actual.end(),expected.begin()))return false;
                allLeaves.insert(actual.begin(),actual.end());
            }
            const auto& seen=descendants.at(id);allDefinitions.insert(seen.begin(),seen.end());
            maximumDepth=std::max(maximumDepth,height.at(id));
        }
        return allLeaves.size()==leaves.size()&&allDefinitions.size()==definitions.size()&&maximumDepth==u.最大深度;
    }catch(...){return false;}
}
特征类标量派生批量读取结果 特征类数据服务::批量读取标量派生定义(const 特征类标量派生批量读取请求& r) const {
    特征类标量派生批量读取结果 out;out.Gread=r.Gread;out.H=r.H;
    标量读取上下文 c{r.Gread,r.H,r.预算};
    auto sync=[&]{out.用量={c.定义计数.size(),c.计量->用量.关系数,c.叶计数.size(),
        c.计量->用量.属性值数,c.最大实际深度,c.计量->用量.材料总数};};
    auto fail=[&](SS state){
        sync();out.状态=state;out.根回执组.clear();out.完整定义组.clear();out.基础叶组.clear();return std::move(out);
    };
    try{
        out.原请求=r;
        std::lock_guard<std::mutex> lock(mutex_);
        标量要求(r.版本==1&&r.H&&r.Gread>=r.H&&r.预算.有效()&&!r.根定义组.empty(),SS::入口拒绝);
        稳定编码 previous{};
        for(auto id:r.根定义组){
            标量要求(有效(id.结点)&&(!有效(previous)||previous<id.结点),SS::入口拒绝);previous=id.结点;
        }
        标量守卫(r.Gread);
        for(auto id:r.根定义组)标量展开(id.结点,true,c);
        for(auto id:r.根定义组){
            const auto& d=c.定义.at(id.结点);
            const auto& left=c.闭包.at(标量来源编码(d.来源组[0].内容.来源));
            const auto& right=c.闭包.at(标量来源编码(d.来源组[1].内容.来源));
            out.根回执组.push_back({id,{left.begin(),left.end()},{right.begin(),right.end()}});
        }
        for(const auto& [id,d]:c.定义)out.完整定义组.push_back(d);
        for(const auto& [id,f]:c.叶)out.基础叶组.push_back(f);
        sync();标量守卫(r.Gread);out.状态=SS::已读取;标量要求(out.成功());return out;
    }catch(const 标量失败& e){return fail(e.状态);}
    catch(S e){return fail(标量映射(e));}
    catch(const std::bad_alloc&){return fail(SS::资源失败);}
    catch(const std::length_error&){return fail(SS::资源失败);}
    catch(...){return fail(SS::内部不一致);}
}

namespace {
特征R规则状态 映射R错误(特征数据错误 e) noexcept {
    using S = 特征数据错误;
    using R = 特征R规则状态;
    switch (e) {
    case S::未找到: return R::特征类型未找到;
    case S::已退出: return R::特征类型已退出;
    case S::数量预算不足: return R::数量预算不足;
    case S::并发变化: return R::事实代次漂移;
    case S::资源失败: return R::资源失败;
    default: return R::内部不一致;
    }
}
bool R材料I64单点(const 特征R区间材料& material, std::int64_t& value) noexcept {
    if (material.格式版本 != 1 || material.类别 != 特征R材料类别::I64闭区间
        || material.规范化U64组.size() != 2 || material.规范化U64组[0] != material.规范化U64组[1]) return false;
    value = std::bit_cast<std::int64_t>(material.规范化U64组[0]);
    return true;
}
特征R区间材料 I64单点R材料(std::int64_t value) {
    return {1, 特征R材料类别::I64闭区间,
        {std::bit_cast<std::uint64_t>(value), std::bit_cast<std::uint64_t>(value)}};
}
}

特征R归组规则结果 特征类数据服务::归组特征R(const 特征R归组规则请求& request) const noexcept {
    特征R归组规则结果 out; out.Gread = request.Gread; out.H = request.H;
    try {
        const auto& b = request.预算;
        if (request.合同版本 != 1 || !request.Gread || !request.H || request.H > request.Gread
            || !有效(request.FT) || !浅层结构有效(request.候选值)
            || !b.最大规则节点数 || !b.最大规则关系数 || !b.最大规则值数
            || !b.最大候选值元素数) return out;
        const bool hasItems = !request.当前R项.empty();
        bool hasMembers = false, hasMaterialElements = false;
        for (const auto& item : request.当前R项) {
            hasMembers = hasMembers || !item.形成成员.empty();
            hasMaterialElements = hasMaterialElements || !item.材料.规范化U64组.empty();
        }
        if ((hasItems != (b.最大R项数 != 0))
            || (hasMembers != (b.最大R成员数 != 0))
            || (hasMaterialElements != (b.最大材料U64项数 != 0))) return out;
        std::lock_guard<std::mutex> lock(mutex_);
        截止有效(1, request.Gread, request.H); 守卫(request.Gread);
        try { (void)读类型(request.FT, request.Gread, request.H); }
        catch (S e) { out.状态 = e == S::能力未提供 ? 特征R规则状态::规则未启用 : 映射R错误(e); return out; }
        if (!读取R规则(request.FT, 1, request.Gread, request.H, b)) { out.状态 = 特征R规则状态::规则未启用; return out; }
        const auto* candidate = std::get_if<std::int64_t>(&request.候选值);
        if (!candidate) { out.状态 = 特征R规则状态::候选值不可读; return out; }
        if (request.当前R项.size() > b.最大R项数) { out.状态 = 特征R规则状态::数量预算不足; return out; }
        const auto material = I64单点R材料(*candidate);
        std::optional<稳定编码> matched; 稳定编码 previousR{}; std::uint64_t members = 0;
        for (const auto& item : request.当前R项) {
            if (!有效(item.R) || (有效(previousR) && !(previousR < item.R))
                || item.材料.规范化U64组.size() > b.最大材料U64项数 || item.形成成员.empty()) {
                out.状态 = 特征R规则状态::内部不一致; return out;
            }
            previousR = item.R; std::int64_t point{};
            if (!R材料I64单点(item.材料, point)) { out.状态 = 特征R规则状态::材料格式不支持; return out; }
            稳定编码 previousF{};
            for (const auto& member : item.形成成员) {
                if (!有效(member.F) || !有效(member.FCv) || (有效(previousF) && !(previousF < member.F.编码))
                    || ++members > b.最大R成员数) { out.状态 = members > b.最大R成员数 ? 特征R规则状态::数量预算不足 : 特征R规则状态::内部不一致; return out; }
                previousF = member.F.编码;
                const auto f = 读准确(member.F, request.Gread, request.H);
                if (f.信息.类型 != request.FT || 完整整数(f) != point) { out.状态 = 特征R规则状态::内部不一致; return out; }
            }
            if (item.材料 == material) {
                if (matched) { out.状态 = 特征R规则状态::内部不一致; return out; }
                matched = item.R;
            }
        }
        守卫(request.Gread); out.规范化材料 = material;
        if (matched) { out.状态 = 特征R规则状态::唯一命中; out.命中R = matched; }
        else out.状态 = 特征R规则状态::形成新R;
    } catch (S e) { out.状态 = 映射R错误(e); out.命中R.reset(); out.规范化材料.reset(); }
    catch (const std::bad_alloc&) { out.状态 = 特征R规则状态::资源失败; out.命中R.reset(); out.规范化材料.reset(); }
    catch (...) { out.状态 = 特征R规则状态::内部不一致; out.命中R.reset(); out.规范化材料.reset(); }
    return out;
}

特征R代表值结果 特征类数据服务::读取特征R代表值(const 特征R代表值请求& request) const noexcept {
    特征R代表值结果 out; out.Gread = request.Gread; out.H = request.H;
    try {
        const auto& b = request.预算;
        if (request.合同版本 != 1 || !request.Gread || !request.H || request.H > request.Gread || !有效(request.FT)
            || !b.最大规则节点数 || !b.最大规则关系数 || !b.最大规则值数 || !b.最大材料U64项数
            || b.最大R项数 || b.最大R成员数 || b.最大候选值元素数) return out;
        std::lock_guard<std::mutex> lock(mutex_); 截止有效(1, request.Gread, request.H); 守卫(request.Gread);
        try { (void)读类型(request.FT, request.Gread, request.H); }
        catch (S e) { out.状态 = e == S::能力未提供 ? 特征R规则状态::规则未启用 : 映射R错误(e); return out; }
        if (!读取R规则(request.FT, 2, request.Gread, request.H, b)) { out.状态 = 特征R规则状态::规则未启用; return out; }
        if (request.材料.规范化U64组.size() > b.最大材料U64项数) { out.状态 = 特征R规则状态::数量预算不足; return out; }
        std::int64_t value{}; if (!R材料I64单点(request.材料, value)) { out.状态 = 特征R规则状态::材料格式不支持; return out; }
        守卫(request.Gread); out.状态 = 特征R规则状态::已取得代表值; out.代表值 = 特征准确值{value};
    } catch (S e) { out.状态 = 映射R错误(e); out.代表值.reset(); }
    catch (const std::bad_alloc&) { out.状态 = 特征R规则状态::资源失败; out.代表值.reset(); }
    catch (...) { out.状态 = 特征R规则状态::内部不一致; out.代表值.reset(); }
    return out;
}

特征R概念归并结果 特征类数据服务::归并特征R概念(const 特征R概念归并请求& request) const noexcept {
    特征R概念归并结果 out; out.Gread = request.Gread; out.H = request.H;
    try {
        const auto& b = request.预算;
        const auto& version = request.R集合版本;
        if (request.合同版本 != 2 || !request.Gread || !request.H || request.H > request.Gread || !有效(request.FT)
            || !有效(version.R集合) || !有效(version.版本) || version.R项.empty() || b.最大候选值元素数) return out;
        std::lock_guard<std::mutex> lock(mutex_); 截止有效(1, request.Gread, request.H); 守卫(request.Gread);
        if (version.R项.size() > b.最大R项数) { out.状态 = 特征R规则状态::数量预算不足; return out; }
        std::uint64_t memberCount = 0, materialCount = 0;
        std::set<稳定编码> seenFeatures;
        稳定编码 previousR{};
        struct 归并候选 final { const 特征R规则项投影* R项; 特征信息身份 首个成员; std::vector<稳定编码> FCv下位; };
        std::vector<归并候选> candidates;
        candidates.reserve(version.R项.size());
        for (const auto& item : version.R项) {
            if (!有效(item.R) || (有效(previousR) && !(previousR < item.R)) || item.形成成员.empty()) {
                out.状态 = 特征R规则状态::内部不一致; return out;
            }
            previousR = item.R;
            materialCount += item.材料.规范化U64组.size();
            if (materialCount > b.最大材料U64项数) { out.状态 = 特征R规则状态::数量预算不足; return out; }
            std::vector<稳定编码> concepts;
            稳定编码 previousF{};
            特征信息身份 first{};
            for (const auto& member : item.形成成员) {
                if (!有效(member.F) || !有效(member.FCv) || (有效(previousF) && !(previousF < member.F.编码))
                    || !seenFeatures.insert(member.F.编码).second) { out.状态 = 特征R规则状态::内部不一致; return out; }
                previousF = member.F.编码;
                if (++memberCount > b.最大R成员数) { out.状态 = 特征R规则状态::数量预算不足; return out; }
                const auto f = 读准确(member.F, request.Gread, request.H);
                if (f.信息.类型 != request.FT) { out.状态 = 特征R规则状态::内部不一致; return out; }
                if (!有效(first)) first = member.F;
                concepts.push_back(member.FCv);
            }
            std::sort(concepts.begin(), concepts.end());
            concepts.erase(std::unique(concepts.begin(), concepts.end()), concepts.end());
            candidates.push_back({&item, first, std::move(concepts)});
        }
        I64基础特征类型信息 type;
        try { type = 读类型(request.FT, request.Gread, request.H); }
        catch (S e) {
            if (e != S::能力未提供) throw;
            守卫(request.Gread); out.状态 = 特征R规则状态::已归并零输出;
            out.R集合 = version.R集合; out.版本 = version.版本; return out;
        }
        if (!type.规格.域形成 || !读取R规则(request.FT, 3, request.Gread, request.H, b)) {
            守卫(request.Gread); out.状态 = 特征R规则状态::已归并零输出;
            out.R集合 = version.R集合; out.版本 = version.版本; return out;
        }
        std::vector<特征RI64概念归并项> normalized;
        normalized.reserve(candidates.size());
        for (const auto& candidate : candidates) {
            std::int64_t point{};
            if (!R材料I64单点(candidate.R项->材料, point)) { out.状态 = 特征R规则状态::材料格式不支持; return out; }
            for (const auto& member : candidate.R项->形成成员) {
                const auto f = 读准确(member.F, request.Gread, request.H);
                if (完整整数(f) != point) { out.状态 = 特征R规则状态::内部不一致; return out; }
            }
            normalized.push_back({形成I64特征域已持锁({1, request.Gread, request.H, candidate.首个成员}).域, candidate.FCv下位});
        }
        auto lessDomain = [](const 特征规范I64域& left, const 特征规范I64域& right) {
            const auto count = std::min(left.区间.size(), right.区间.size());
            for (std::size_t index = 0; index < count; ++index) {
                if (left.区间[index].下界 != right.区间[index].下界) return left.区间[index].下界 < right.区间[index].下界;
                if (left.区间[index].上界 != right.区间[index].上界) return left.区间[index].上界 < right.区间[index].上界;
            }
            return left.区间.size() < right.区间.size();
        };
        std::sort(normalized.begin(), normalized.end(), [&](const auto& left, const auto& right) {
            if (lessDomain(left.域, right.域)) return true;
            if (lessDomain(right.域, left.域)) return false;
            return left.FCv下位 < right.FCv下位;
        });
        normalized.erase(std::unique(normalized.begin(), normalized.end()), normalized.end());
        守卫(request.Gread); out.状态 = normalized.empty() ? 特征R规则状态::已归并零输出 : 特征R规则状态::已归并输出;
        out.R集合 = version.R集合; out.版本 = version.版本; out.项 = std::move(normalized);
    } catch (S e) { out.状态 = e == S::能力未提供 ? 特征R规则状态::规则未启用 : 映射R错误(e); out.项.clear(); }
    catch (const std::bad_alloc&) { out.状态 = 特征R规则状态::资源失败; out.项.clear(); }
    catch (...) { out.状态 = 特征R规则状态::内部不一致; out.项.clear(); }
    return out;
}

特征数据结果<补齐I64默认R规则结果> 特征类数据服务::补齐I64默认R规则(const 补齐I64默认R规则请求& request) {
    return 保护<补齐I64默认R规则结果>([&] {
        补齐I64默认R规则结果 out;
        要求(request.合同版本 == 1 && request.G && 有效(request.FT) && 有效(request.幂等身份), S::入口拒绝);
        const auto g = 当前G(); out.Gread = g; 要求(g == request.G, S::并发变化);
        结构就绪(分区::定义, g, g); R规则就绪();
        const auto type = 读类型(request.FT, g, g);
        std::array<bool, 3> present{}; const auto required = type.规格.域形成 ? 3U : 2U;
        const 特征R规则读取预算 unbounded{
            std::numeric_limits<std::uint64_t>::max(), std::numeric_limits<std::uint64_t>::max(),
            std::numeric_limits<std::uint64_t>::max(), 0, 0, 0, 0};
        for (unsigned i = 0; i < required; ++i)
            present[i] = 读取R规则(request.FT, static_cast<std::int64_t>(i + 1), g, g, unbounded).has_value();
        const auto count = static_cast<unsigned>(present[0]) + static_cast<unsigned>(present[1]) + static_cast<unsigned>(present[2]);
        if (count == required) { out.状态 = 特征R规则状态::已归并零输出; out.规则已补齐 = true; return out; }
        要求(count == 0, S::内部不一致);
        auto ws = 新写集(分区::定义, g); ws.写入幂等身份 = request.幂等身份;
        添加I64默认R规则(ws, request.FT.编码, type.规格.域形成.has_value());
        (void)提交(分区::定义, std::move(ws));
        out.Gread = 当前G(); out.状态 = 特征R规则状态::已归并零输出; out.规则已补齐 = true; return out;
    });
}

} // namespace 海中鱼巣
