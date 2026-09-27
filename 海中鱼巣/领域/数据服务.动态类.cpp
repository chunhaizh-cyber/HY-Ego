#include "数据服务.动态类.h"

#include <algorithm>
#include <array>
#include <set>
#include <stdexcept>
#include <utility>

#include "数据服务.状态类.h"
#include "数据服务.特征类.h"
#include "数据服务.特征值类.h"

namespace 海中鱼巣 {
namespace {

const L1所有者范围一致节点读取结果项* 节点(
    const L1所有者范围一致当前读取结果& r, 稳定编码 id) noexcept {
    const auto it = std::find_if(r.节点.begin(), r.节点.end(),
        [&](const auto& x) { return x.查询编码 == id; });
    return it == r.节点.end() ? nullptr : &*it;
}

const L1所有者范围一致属性值读取结果项* 属性(
    const L1所有者范围一致当前读取结果& r,
    稳定编码 id, 稳定编码 type) noexcept {
    const auto it = std::find_if(r.属性值.begin(), r.属性值.end(),
        [&](const auto& x) { return x.节点 == id && x.属性类型 == type; });
    return it == r.属性值.end() ? nullptr : &*it;
}

const L1所有者范围一致源关系组读取结果项* 源关系(
    const L1所有者范围一致当前读取结果& r,
    稳定编码 id, 稳定编码 type) noexcept {
    const auto it = std::find_if(r.源关系组.begin(), r.源关系组.end(),
        [&](const auto& x) { return x.源节点 == id && x.关系类型节点 == type; });
    return it == r.源关系组.end() ? nullptr : &*it;
}

bool 当前普通节点(const L1所有者范围一致节点读取结果项* item,
    稳定编码 id, L1结构所有者身份 owner) noexcept {
    return item && item->状态 == L1所有者范围一致当前读取项目状态::成功
        && item->事实 && item->事实->编码 == id
        && item->事实->写入所有者 == owner
        && item->事实->种类 == 节点种类::普通
        && !item->事实->属性类型表示;
}

bool 当前属性类型节点(const L1所有者范围一致节点读取结果项* item,
    稳定编码 id, L1结构所有者身份 owner,
    L1所有者范围值表示种类 repr) noexcept {
    return item && item->状态 == L1所有者范围一致当前读取项目状态::成功
        && item->事实 && item->事实->编码 == id
        && item->事实->写入所有者 == owner
        && item->事实->种类 == 节点种类::属性类型
        && item->事实->属性类型表示 == repr;
}

bool 唯一关系(const L1所有者范围一致源关系组读取结果项* item,
    稳定编码 source, 稳定编码 type, L1结构所有者身份 owner,
    std::int64_t role, std::optional<稳定编码> target = std::nullopt) noexcept {
    if (!item || item->成员.size() != 1) return false;
    const auto& edge = item->成员.front().关系;
    return edge.写入所有者 == owner && edge.源节点 == source
        && edge.关系类型节点 == type && edge.角色或顺序 == role
        && (!target || edge.目标节点 == *target);
}

动态数据状态 映射一致读取(L1所有者范围一致当前读取状态 s) noexcept {
    if (s == L1所有者范围一致当前读取状态::资源失败)
        return 动态数据状态::资源失败;
    if (s == L1所有者范围一致当前读取状态::入口拒绝)
        return 动态数据状态::入口拒绝;
    return 动态数据状态::内部不一致;
}

} // namespace

动态类数据服务::动态类数据服务(L1事实基座服务& l1,
    L1所有者范围写端口&& port, const 动态结构交付& layout,
    const 存在结构身份只读提供者& existence,
    const 状态使用绑定只读提供者& binding,
    const 状态类数据服务& state, const 特征类数据服务& feature,
    const 特征值类数据服务& value)
    : l1_(l1), port_(std::move(port)), owner_(port_.所有者身份()),
      layout_(layout), existence_(existence), binding_(binding), state_(state),
      feature_(feature), value_(value) {
    if (!绑定于(l1_) || !有效(owner_) || !布局浅层有效() || !当前布局有效())
        throw std::invalid_argument("invalid dynamic configuration");
}

bool 动态类数据服务::绑定于(const L1事实基座服务& s) const noexcept {
    return &s == &l1_ && port_.有效() && port_.绑定于(s)
        && existence_.绑定于(s) && binding_.绑定于(s)
        && state_.绑定于(s) && feature_.绑定于(s) && value_.绑定于(s);
}

bool 动态类数据服务::布局浅层有效() const noexcept {
    const 稳定编码 ids[]{layout_.格式锚点, layout_.字段登记关系类型,
        layout_.动态族锚点, layout_.族归属关系类型, layout_.主体关系类型,
        layout_.前绑定关系类型, layout_.后绑定关系类型,
        layout_.同主体成员关系类型, layout_.首次形成UTC属性类型};
    for (std::size_t i = 0; i < std::size(ids); ++i) {
        if (!有效(ids[i])) return false;
        for (std::size_t j = 0; j < i; ++j)
            if (ids[i] == ids[j]) return false;
    }
    return true;
}

bool 动态类数据服务::当前布局有效() const {
    L1所有者范围一致当前读取请求 q;
    q.所有者 = {owner_};
    q.节点 = {layout_.格式锚点, layout_.字段登记关系类型,
        layout_.动态族锚点, layout_.族归属关系类型, layout_.主体关系类型,
        layout_.前绑定关系类型, layout_.后绑定关系类型,
        layout_.同主体成员关系类型, layout_.首次形成UTC属性类型};
    q.源关系组 = {{layout_.格式锚点, layout_.字段登记关系类型}};
    const auto r = l1_.尝试读取所有者范围一致当前投影(q);
    if (r.状态 != L1所有者范围一致当前读取状态::成功
        || r.所有者.size() != 1 || r.所有者.front().查询所有者 != owner_
        || r.所有者.front().状态 != L1所有者范围一致当前读取项目状态::成功
        || !r.所有者.front().所有者事实
        || r.所有者.front().所有者事实->范围种类
            != L1所有者范围种类::独占结构范围)
        return false;
    for (const auto id : q.节点) {
        if (id == layout_.首次形成UTC属性类型) {
            if (!当前属性类型节点(节点(r, id), id, owner_,
                    L1所有者范围值表示种类::I64))
                return false;
        } else if (!当前普通节点(节点(r, id), id, owner_)) return false;
    }
    const auto fields = 源关系(r, layout_.格式锚点, layout_.字段登记关系类型);
    if (!fields || fields->成员.size() != 7) return false;
    const 稳定编码 expected[]{layout_.动态族锚点, layout_.族归属关系类型,
        layout_.主体关系类型, layout_.前绑定关系类型,
        layout_.后绑定关系类型, layout_.同主体成员关系类型,
        layout_.首次形成UTC属性类型};
    for (std::size_t i = 0; i < std::size(expected); ++i) {
        const auto count = std::count_if(fields->成员.begin(), fields->成员.end(),
            [&](const auto& item) {
                const auto& edge = item.关系;
                return edge.写入所有者 == owner_
                    && edge.源节点 == layout_.格式锚点
                    && edge.关系类型节点 == layout_.字段登记关系类型
                    && edge.目标节点 == expected[i]
                    && edge.角色或顺序 == static_cast<std::int64_t>(i + 1);
            });
        if (count != 1) return false;
    }
    return true;
}

动态操作结果 动态类数据服务::失败头(动态数据状态 s) noexcept {
    return {s, 动态发布阶段::无写入};
}
动态单项结果 动态类数据服务::单项失败(动态数据状态 s) noexcept {
    return {失败头(s), std::nullopt};
}
动态组结果 动态类数据服务::组失败(动态数据状态 s) noexcept {
    return {失败头(s), {}};
}
动态展开结果 动态类数据服务::展开失败(动态数据状态 s) noexcept {
    return {失败头(s), std::nullopt};
}

动态单项结果 动态类数据服务::创建原子动态(const 动态原子创建请求& q) {
    if (!有效(q.幂等身份) || !有效(q.主体存在)
        || !有效(q.变化.前绑定) || !有效(q.变化.后绑定)
        || q.变化.前绑定 == q.变化.后绑定)
        return 单项失败(动态数据状态::入口拒绝);
    // 需要将存在、前后 C/E/S 绑定、状态内容与比较依据放在共同快照中核验。
    return 单项失败(动态数据状态::依赖未实现);
}

动态单项结果 动态类数据服务::创建组合动态(const 动态组合创建请求& q) {
    if (!有效(q.幂等身份) || !有效(q.主体存在) || q.变化.成员组.empty())
        return 单项失败(动态数据状态::入口拒绝);
    std::set<std::uint64_t> seen;
    for (std::size_t i = 0; i < q.变化.成员组.size(); ++i) {
        const auto& item = q.变化.成员组[i];
        if (item.顺序 != i + 1 || item.角色 != 动态成员角色::同主体
            || !有效(item.子动态) || !seen.insert(item.子动态.编码.值).second)
            return 单项失败(item.顺序 != i + 1 || !有效(item.子动态)
                ? 动态数据状态::入口拒绝 : 动态数据状态::重复成员);
    }
    // 成员共同主体、无环和写入必须由一次锁内组合能力闭合。
    return 单项失败(动态数据状态::依赖未实现);
}

动态单项结果 动态类数据服务::读取当前动态(
    const 动态当前读取请求& q) const {
    if (!有效(q.身份)) return 单项失败(动态数据状态::入口拒绝);
    try {
        L1所有者范围一致当前读取请求 request;
        request.所有者 = {owner_};
        request.节点 = {q.身份.编码};
        request.属性值 = {{q.身份.编码, layout_.首次形成UTC属性类型}};
        request.源关系组 = {
            {q.身份.编码, layout_.族归属关系类型},
            {q.身份.编码, layout_.主体关系类型},
            {q.身份.编码, layout_.前绑定关系类型},
            {q.身份.编码, layout_.后绑定关系类型},
            {q.身份.编码, layout_.同主体成员关系类型}};
        const auto r = l1_.尝试读取所有者范围一致当前投影(request);
        if (r.状态 != L1所有者范围一致当前读取状态::成功)
            return 单项失败(映射一致读取(r.状态));
        const auto n = 节点(r, q.身份.编码);
        if (n && n->状态 == L1所有者范围一致当前读取项目状态::未找到)
            return 单项失败(动态数据状态::未找到);
        if (!当前普通节点(n, q.身份.编码, owner_))
            return 单项失败(动态数据状态::内部不一致);
        const auto family = 源关系(r, q.身份.编码, layout_.族归属关系类型);
        const auto subject = 源关系(r, q.身份.编码, layout_.主体关系类型);
        const auto before = 源关系(r, q.身份.编码, layout_.前绑定关系类型);
        const auto after = 源关系(r, q.身份.编码, layout_.后绑定关系类型);
        const auto members = 源关系(r, q.身份.编码, layout_.同主体成员关系类型);
        const auto utc = 属性(r, q.身份.编码, layout_.首次形成UTC属性类型);
        if (!唯一关系(family, q.身份.编码, layout_.族归属关系类型,
                owner_, 1, layout_.动态族锚点)
            || !唯一关系(subject, q.身份.编码, layout_.主体关系类型,
                owner_, 1)
            || !utc || utc->状态 != L1所有者范围一致当前读取项目状态::成功
            || !utc->投影)
            return 单项失败(动态数据状态::内部不一致);
        const auto* time = std::get_if<std::int64_t>(&utc->投影->当前值事实.材料);
        if (!time || *time < 0) return 单项失败(动态数据状态::旧格式不支持);

        动态变化内容 change;
        std::vector<稳定编码> changeEdges;
        if (before && after && members
            && before->成员.size() == 1 && after->成员.size() == 1
            && members->成员.empty()
            && before->成员.front().关系.目标节点
                != after->成员.front().关系.目标节点) {
            const auto& a = before->成员.front().关系;
            const auto& b = after->成员.front().关系;
            if (a.写入所有者 != owner_ || b.写入所有者 != owner_
                || a.角色或顺序 != 1 || b.角色或顺序 != 1)
                return 单项失败(动态数据状态::内部不一致);
            change = 动态原子内容{{a.目标节点}, {b.目标节点}};
            changeEdges = {a.编码, b.编码};
        } else if (before && after && members
            && before->成员.empty() && after->成员.empty()
            && !members->成员.empty()) {
            auto sorted = members->成员;
            std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
                return a.关系.角色或顺序 < b.关系.角色或顺序;
            });
            动态组合内容 composite;
            std::set<std::uint64_t> seen;
            for (std::size_t i = 0; i < sorted.size(); ++i) {
                const auto& edge = sorted[i].关系;
                if (edge.写入所有者 != owner_
                    || edge.角色或顺序 != static_cast<std::int64_t>(i + 1)
                    || !seen.insert(edge.目标节点.值).second)
                    return 单项失败(动态数据状态::内部不一致);
                composite.成员组.push_back({static_cast<std::uint32_t>(i + 1),
                    动态成员角色::同主体, {edge.目标节点}});
                changeEdges.push_back(edge.编码);
            }
            change = std::move(composite);
        } else return 单项失败(动态数据状态::内部不一致);

        动态内容事实 content;
        content.信息 = {q.身份,
            subject->成员.front().关系.目标节点, std::move(change)};
        content.自有事实 = {family->成员.front().关系.编码,
            subject->成员.front().关系.编码,
            utc->投影->当前值事实.编码, std::move(changeEdges)};
        content.首次形成UTC纳秒 = *time;
        return {{动态数据状态::已读取, 动态发布阶段::无写入},
            std::move(content)};
    } catch (const std::bad_alloc&) {
        return 单项失败(动态数据状态::资源失败);
    } catch (const std::length_error&) {
        return 单项失败(动态数据状态::资源失败);
    } catch (...) {
        return 单项失败(动态数据状态::内部不一致);
    }
}

动态组结果 动态类数据服务::按主体查询动态(
    const 动态按主体查询请求& q) const {
    if (!有效(q.主体存在)) return 组失败(动态数据状态::入口拒绝);
    // 反向关系候选与每个动态完整内容当前尚无一次联合投影。
    return 组失败(动态数据状态::依赖未实现);
}

动态组结果 动态类数据服务::按绑定反查原子动态(
    const 动态按绑定查询请求& q) const {
    if (!有效(q.绑定)) return 组失败(动态数据状态::入口拒绝);
    return 组失败(动态数据状态::依赖未实现);
}

动态组结果 动态类数据服务::按子动态反查父动态(
    const 动态按子动态查询请求& q) const {
    if (!有效(q.子动态)) return 组失败(动态数据状态::入口拒绝);
    return 组失败(动态数据状态::依赖未实现);
}

动态展开结果 动态类数据服务::展开动态来源(
    const 动态来源展开请求& q) const {
    if (!有效(q.身份)
        || (q.方式 != 动态展开方式::直接 && q.方式 != 动态展开方式::递归))
        return 展开失败(动态数据状态::入口拒绝);
    // 展开还需要动态、C/E/S 绑定和状态事实的共同快照。
    return 展开失败(动态数据状态::依赖未实现);
}

动态操作结果 动态类数据服务::确认当前动态结构身份(
    const 动态当前身份请求& q) const {
    const auto read = 读取当前动态({q.身份});
    return read.操作.状态 == 动态数据状态::已读取 && read.内容
        ? 动态操作结果{动态数据状态::已读取, 动态发布阶段::无写入}
        : read.操作;
}

动态单项结果 动态类数据服务::退出动态(const 动态退出请求& q) {
    if (!有效(q.幂等身份) || !有效(q.身份))
        return 单项失败(动态数据状态::入口拒绝);
    // current-only L1 尚无“核对完整动态事实并原子退出”的条件写入口。
    return 单项失败(动态数据状态::依赖未实现);
}

} // namespace 海中鱼巣
