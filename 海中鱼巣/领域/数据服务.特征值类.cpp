#include "数据服务.特征值类.h"

namespace 海中鱼巣 {
namespace {

constexpr L1所有者范围写集本地键 承载键{1};
constexpr L1所有者范围写集本地键 U64类型键{2};
constexpr L1所有者范围写集本地键 值键{1};

template<class T> bool 是成功写入(T s) noexcept {
    return s == T::成功 || s == T::精确重复;
}

特征值U64组保存状态_B2 映射写入(L1所有者范围写入状态 s) noexcept {
    switch (s) {
    case L1所有者范围写入状态::许可拒绝: return 特征值U64组保存状态_B2::许可拒绝;
    case L1所有者范围写入状态::未找到: return 特征值U64组保存状态_B2::未找到;
    case L1所有者范围写入状态::事实代次漂移: return 特征值U64组保存状态_B2::事实代次漂移;
    case L1所有者范围写入状态::幂等冲突: return 特征值U64组保存状态_B2::幂等冲突;
    case L1所有者范围写入状态::引用冲突: return 特征值U64组保存状态_B2::引用冲突;
    case L1所有者范围写入状态::资源失败: return 特征值U64组保存状态_B2::资源失败;
    case L1所有者范围写入状态::内部不一致: return 特征值U64组保存状态_B2::内部不一致;
    default: return 特征值U64组保存状态_B2::入口拒绝;
    }
}

特征值U64组保存状态_B2 映射读取(L1所有者范围属性类型当前完整值组读取状态_v2 s) noexcept {
    switch (s) {
    case L1所有者范围属性类型当前完整值组读取状态_v2::未找到: return 特征值U64组保存状态_B2::未找到;
    case L1所有者范围属性类型当前完整值组读取状态_v2::事实代次漂移: return 特征值U64组保存状态_B2::事实代次漂移;
    case L1所有者范围属性类型当前完整值组读取状态_v2::资源失败: return 特征值U64组保存状态_B2::资源失败;
    default: return 特征值U64组保存状态_B2::内部不一致;
    }
}

特征值U64组结构登记状态_B2 映射登记写入(L1所有者范围写入状态 s) noexcept {
    switch (s) {
    case L1所有者范围写入状态::许可拒绝: return 特征值U64组结构登记状态_B2::许可拒绝;
    case L1所有者范围写入状态::未找到: return 特征值U64组结构登记状态_B2::未找到;
    case L1所有者范围写入状态::事实代次漂移: return 特征值U64组结构登记状态_B2::事实代次漂移;
    case L1所有者范围写入状态::幂等冲突: return 特征值U64组结构登记状态_B2::幂等冲突;
    case L1所有者范围写入状态::引用冲突: return 特征值U64组结构登记状态_B2::引用冲突;
    case L1所有者范围写入状态::资源失败: return 特征值U64组结构登记状态_B2::资源失败;
    default: return 特征值U64组结构登记状态_B2::内部不一致;
    }
}

std::optional<稳定编码> 映射(const L1所有者范围写入结果& r,
                              L1所有者范围写集本地键 key) noexcept {
    std::optional<稳定编码> out;
    for (const auto& [local, id] : r.新编码映射) {
        if (local == key) {
            if (out || !有效(id)) return std::nullopt;
            out = id;
        }
    }
    return out;
}

bool U64节点结构当前(const L1事实基座服务& l1, 稳定编码 node,
                     L1结构所有者身份 owner, 节点种类 kind,
                     std::optional<L1所有者范围值表示种类> representation,
                     std::uint64_t g) noexcept {
    const auto raw = l1.读取所有者范围当前节点({L1所有者范围CRUD合同版本, node});
    const auto* fact = raw.事实 ? std::get_if<L1所有者范围节点事实>(&*raw.事实) : nullptr;
    return raw.状态 == L1所有者范围读取状态::成功 && raw.读取事实代次 == g && fact
        && fact->编码 == node && fact->写入所有者 == owner && fact->种类 == kind
        && fact->属性类型表示 == representation;
}

}

bool 特征值U64组保存结果_B2::成功(const 特征值U64组保存请求_B2& r) const noexcept {
    return r.版本 == 1 && r.G0 != 0 && !r.内容.empty() && 有效(r.幂等)
        && 状态 == 特征值U64组保存状态_B2::已复用 && Gread == r.G0 && 值 && 有效(*值)
        || (r.版本 == 1 && r.G0 != 0 && !r.内容.empty() && 有效(r.幂等)
            && 状态 == 特征值U64组保存状态_B2::已保存 && Gread > r.G0 && 值 && 有效(*值));
}

bool 特征值U64组结构登记结果_B2::成功(const 特征值U64组结构登记请求_B2& r) const noexcept {
    return r.版本 == 1 && r.G0 != 0 && 有效(r.幂等)
        && 状态 == 特征值U64组结构登记状态_B2::已登记 && Gread >= r.G0 && 交付
        && 有效(交付->所有者) && 有效(交付->承载节点)
        && 有效(交付->U64组属性类型节点) && 交付->来源节点 == 交付->承载节点;
}

特征值类数据服务::特征值类数据服务(const L1事实基座服务& l1,
    L1所有者范围写端口&& port, const 特征值U64组结构交付_B2& layout) noexcept
    : 第一层服务_(l1), 材料服务_(nullptr), 写端口_(std::move(port)), U64组结构_(layout) {
    if (!写端口_ || !写端口_->绑定于(l1) || !有效(layout.所有者)
        || 写端口_->所有者身份() != layout.所有者 || !有效(layout.承载节点)
        || !有效(layout.U64组属性类型节点) || layout.来源节点 != layout.承载节点)
        U64组结构_.reset();
}

特征值U64组结构登记结果_B2 特征值类数据服务::登记U64组结构_B2(
    const L1事实基座服务& l1, L1所有者范围写端口& port,
    const 特征值U64组结构登记请求_B2& r) noexcept {
    特征值U64组结构登记结果_B2 out;
    if (r.版本 != 1 || !r.G0 || !有效(r.幂等) || !port.绑定于(l1)) return out;
    try {
        L1所有者范围写集请求 ws{L1所有者范围CRUD合同版本, r.G0, r.幂等};
        ws.节点 = {{承载键, 节点种类::普通, std::nullopt},
            {U64类型键, 节点种类::属性类型, L1所有者范围值表示种类::U64组}};
        const auto saved = port.提交所有者范围中性写集(ws);
        out.Gread = saved.事实代次;
        if (!是成功写入(saved.状态)) { out.状态 = 映射登记写入(saved.状态); return out; }
        const auto carrier = 映射(saved, 承载键); const auto type = 映射(saved, U64类型键);
        if (!carrier || !type || *carrier == *type || saved.新编码映射.size() != 2) {
            out.状态 = 特征值U64组结构登记状态_B2::已可能发布; return out;
        }
        const auto tail = l1.读取中性当前事实代次({L1中性CRUD合同版本});
        if (tail.状态 != L1中性读取状态::成功 || !tail.事实代次
            || saved.事实代次 != r.G0 + 1 || tail.事实代次 < saved.事实代次
            || (saved.状态 == L1所有者范围写入状态::成功
                && tail.事实代次 != saved.事实代次)) {
            out.状态 = 特征值U64组结构登记状态_B2::已可能发布; return out;
        }
        const auto g = tail.事实代次;
        if (!U64节点结构当前(l1, *carrier, port.所有者身份(), 节点种类::普通, std::nullopt, g)
            || !U64节点结构当前(l1, *type, port.所有者身份(), 节点种类::属性类型,
                L1所有者范围值表示种类::U64组, g)) {
            out.状态 = 特征值U64组结构登记状态_B2::已可能发布; return out;
        }
        out.Gread = g; out.交付 = {port.所有者身份(), *carrier, *type, *carrier};
        out.状态 = 特征值U64组结构登记状态_B2::已登记;
    } catch (const std::bad_alloc&) { out.状态 = 特征值U64组结构登记状态_B2::资源失败;
    } catch (...) { out.状态 = 特征值U64组结构登记状态_B2::内部不一致; }
    return out;
}

特征值完整读取结果_B1 特征值类数据服务::读取完整U64组_B2(
    const 特征值完整读取请求_B1& r) const noexcept {
    auto out = 获取完整值(r);
    if (out.状态 == 特征值读取状态_B1::已读取 && (!out.值 || out.值->表示 != 特征值表示类型::U64组)) {
        out.状态 = 特征值读取状态_B1::类型不相容; out.值.reset(); out.材料.reset(); out.用量 = {};
    }
    return out;
}

特征值U64组保存结果_B2 特征值类数据服务::保存U64组(
    const 特征值U64组保存请求_B2& r) noexcept {
    特征值U64组保存结果_B2 out;
    if (r.版本 != 1 || !r.G0 || !有效(r.幂等) || r.内容.empty() || !世界结构预算有效(r.预算)) return out;
    if (!写端口_ || !U64组结构_) { out.状态 = 特征值U64组保存状态_B2::结构未就绪; return out; }
    try {
        const auto& layout = *U64组结构_;
        const auto all = 第一层服务_.读取所有者范围属性类型当前完整值组(
            {L1所有者范围属性类型当前完整值组读取合同版本, layout.所有者,
             layout.U64组属性类型节点, r.G0});
        out.Gread = all.读取事实代次; out.用量.最大值数 = 1;
        if (all.状态 != L1所有者范围属性类型当前完整值组读取状态_v2::成功) {
            out.状态 = 映射读取(all.状态); return out;
        }
        if (all.读取事实代次 != r.G0 || all.所有者 != layout.所有者
            || all.属性类型节点 != layout.U64组属性类型节点) { out.状态 = 特征值U64组保存状态_B2::内部不一致; return out; }
        std::optional<特征值身份> hit;
        for (const auto& value : all.当前值) {
            const auto* bytes = std::get_if<std::vector<std::uint64_t>>(&value.材料);
            if (value.写入所有者 != layout.所有者 || value.所属节点 != layout.承载节点
                || value.属性类型节点 != layout.U64组属性类型节点 || value.来源节点 != layout.来源节点
                || !bytes) { out.状态 = 特征值U64组保存状态_B2::内部不一致; return out; }
            if (*bytes == r.内容) { if (hit) { out.状态 = 特征值U64组保存状态_B2::内部不一致; return out; } hit = {value.编码}; }
        }
        if (hit) { out.状态 = 特征值U64组保存状态_B2::已复用; out.值 = hit; return out; }
        L1所有者范围写集请求 ws{L1所有者范围CRUD合同版本, r.G0, r.幂等};
        ws.值.push_back({值键, layout.承载节点, layout.U64组属性类型节点, r.内容, layout.来源节点});
        const auto saved = 写端口_->提交所有者范围中性写集(ws);
        out.Gread = saved.事实代次;
        if (!是成功写入(saved.状态)) { out.状态 = 映射写入(saved.状态); return out; }
        const auto id = 映射(saved, 值键);
        if (!id || saved.新编码映射.size() != 1) { out.状态 = 特征值U64组保存状态_B2::已可能发布; return out; }
        const auto tail = 第一层服务_.读取中性当前事实代次({L1中性CRUD合同版本});
        if (tail.状态 != L1中性读取状态::成功 || !tail.事实代次
            || (saved.状态 == L1所有者范围写入状态::成功 && tail.事实代次 != r.G0 + 1)
            || (saved.状态 == L1所有者范围写入状态::精确重复 && tail.事实代次 != r.G0)) {
            out.状态 = 特征值U64组保存状态_B2::已可能发布; return out;
        }
        auto readBudget = r.预算;
        if (readBudget.最大值元素数 < r.内容.size()) { out.状态 = 特征值U64组保存状态_B2::数量预算不足; return out; }
        auto read = 读取完整U64组_B2({1, tail.事实代次, {*id}, readBudget});
        if (!read.成功({1, tail.事实代次, {*id}, readBudget})
            || !read.值 || std::get<std::vector<std::uint64_t>>(read.值->内容) != r.内容) {
            out.状态 = 特征值U64组保存状态_B2::已可能发布; return out;
        }
        out.Gread = tail.事实代次; out.值 = {*id}; out.状态 = 特征值U64组保存状态_B2::已保存;
    } catch (const std::bad_alloc&) { out.状态 = 特征值U64组保存状态_B2::资源失败;
    } catch (...) { out.状态 = 特征值U64组保存状态_B2::内部不一致; }
    return out;
}

} // namespace 海中鱼巣
