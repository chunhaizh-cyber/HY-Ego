module;

#include <algorithm>
#include <cstdint>
#include <map>
#include <optional>
#include <set>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

export module 海中鱼巣.业务.应用服务.动态概念树类;
export import 海中鱼巣.领域.数据服务.概念树类;

export namespace 海中鱼巣 {
struct 动态概念应用预算 final {
    概念树预算 数据;
    std::uint64_t 最大场景关系数 = 0, 最大存在关系数 = 0, 最大状态数 = 0, 最大变化特征数 = 0,
                  最大快照特征总数 = 0, 最大继续尝试数 = 0;
    friend bool operator==(const 动态概念应用预算 &, const 动态概念应用预算 &) = default;
};
struct 动态概念特征见证 final {
    概念树概念身份 模板;
    稳定编码 原快照特征{}, 变化目标特征{};
    friend bool operator==(const 动态概念特征见证 &, const 动态概念特征见证 &) = default;
};
struct 动态概念槽见证 final {
    稳定编码 状态{};
    std::vector<动态概念特征见证> 约束;
    friend bool operator==(const 动态概念槽见证 &, const 动态概念槽见证 &) = default;
};
struct 动态概念形成请求 final {
    std::uint32_t 合同版本 = 1;
    std::uint64_t Gread = 0, H = 0;
    概念树场景引用 场景;
    概念树存在引用 主体;
    概念树动态引用 动态;
    std::vector<动态概念槽见证> 槽见证;
    std::uint64_t 创建键 = 0, 支持键 = 0;
    动态概念应用预算 预算;
    friend bool operator==(const 动态概念形成请求 &, const 动态概念形成请求 &) = default;
};
struct 动态概念上位请求 final {
    std::uint32_t 合同版本 = 1;
    std::uint64_t Gread = 0;
    概念树概念身份 下位;
    std::vector<std::vector<概念树概念身份>> 保留槽模板;
    std::uint64_t 创建键 = 0, 接边键 = 0;
    动态概念应用预算 预算;
    friend bool operator==(const 动态概念上位请求 &, const 动态概念上位请求 &) = default;
};
using 动态概念原业务请求 = std::variant<动态概念形成请求, 动态概念上位请求>;
struct 动态概念继续材料 final {
    std::uint32_t 合同版本 = 1;
    动态概念原业务请求 原请求;
    std::uint64_t 首次读取G = 0;
    概念树动态定义 冻结定义;
    std::optional<概念树概念身份> 已定位概念;
    std::optional<概念树动态建立请求> 建立原请求;
    std::optional<概念树支持建立请求> 支持原请求;
    std::optional<概念树上位操作请求> 接边原请求;
    std::vector<概念树业务写请求> 已确认未绑定尝试;
    std::vector<概念树写入结果> 历次回执提示;
    friend bool operator==(const 动态概念继续材料 &, const 动态概念继续材料 &) = default;
};
struct 动态概念继续请求 final {
    std::uint32_t 合同版本 = 1;
    std::uint64_t Gread = 0;
    动态概念继续材料 材料;
    动态概念应用预算 本次预算;
};
enum class 动态概念应用状态 : std::uint8_t {
    已完成 = 1,
    待继续,
    需显式治理,
    入口拒绝,
    见证不匹配,
    目标未找到,
    目标已退出,
    事实代次漂移,
    数量预算不足,
    历史证据不可用,
    幂等冲突,
    可能已发布,
    资源失败,
    内部不一致
};
enum class 动态概念阶段 : std::uint8_t {
    未形成 = 1,
    概念已确定,
    支持已确认,
    上位已确定,
    接边已确认,
    治理已确认,
    退出已确认,
    引用已释放,
    先前阶段待核验
};
struct 动态概念应用结果 final {
    std::uint32_t 合同版本 = 1;
    动态概念应用状态 状态 = 动态概念应用状态::入口拒绝;
    动态概念阶段 阶段 = 动态概念阶段::未形成;
    std::uint64_t Gread = 0;
    std::optional<概念树概念事实> 概念;
    std::optional<概念树支持事实> 支持;
    std::vector<概念树直接上位事实> 直接边;
    std::optional<动态概念继续材料> 继续;
    std::vector<概念树写入结果> 步骤回执;
    bool 成功() const noexcept;
};

class 动态概念树应用服务 final {
    using A = 动态概念应用状态;
    using Stage = 动态概念阶段;
    using D = 概念树数据状态;
    struct 失败 {
        A 状态;
    };
    概念树类数据服务 &concepts_;
    const 场景类数据服务 &scenes_;
    const 存在类数据服务 &existences_;
    const 动态类数据服务 &dynamics_;
    概念树概念身份 root_;

  public:
    动态概念树应用服务(概念树类数据服务 &c, const 场景类数据服务 &s, const 存在类数据服务 &e,
                       const 动态类数据服务 &d, 概念树概念身份 root)
        : concepts_(c), scenes_(s), existences_(e), dynamics_(d), root_(root) {
        if (!有效(root.值) || !c.使用动态世界服务(s, e, d) || c.动态根引用() != root)
            throw std::invalid_argument("dynamic concept application binding mismatch");
    }
    动态概念树应用服务() = delete;
    动态概念树应用服务(const 动态概念树应用服务 &) = delete;
    动态概念树应用服务 &operator=(const 动态概念树应用服务 &) = delete;
    动态概念树应用服务(动态概念树应用服务 &&) = delete;
    动态概念树应用服务 &operator=(动态概念树应用服务 &&) = delete;
    动态概念应用结果 形成或复用动态概念(const 动态概念形成请求 &);
    动态概念应用结果 形成上位动态概念(const 动态概念上位请求 &);
    动态概念应用结果 继续动态概念操作(const 动态概念继续请求 &);
    概念树图结果 读取动态概念树(const 概念树图读取请求 &) const;
    概念树写入结果 迁移动态概念生命周期(const 概念树生命周期迁移请求 &);
    概念树写入结果 退出动态概念(const 概念树概念退出请求 &);
    概念树写入结果 退出动态支持(const 概念树支持退出请求 &);
    概念树写入结果 释放动态形成依据(const 概念树动态依据释放请求 &);
    概念树写入结果 释放动态上下文引用(const 概念树形成引用释放请求 &);

  private:
    std::uint64_t 选择读取G(std::uint64_t) const;
    概念树动态定义 校验形成见证(const 动态概念形成请求 &, std::uint64_t) const;
    std::optional<概念树动态定义> 构造上位定义(const 概念树概念事实 &,
                                               const std::vector<std::vector<概念树概念身份>> &) const;
    动态概念应用结果 推进形成(动态概念继续材料, std::uint64_t, const 动态概念应用预算 &);
    动态概念应用结果 推进上位(动态概念继续材料, std::uint64_t, const 动态概念应用预算 &);
    void 核验最终结果(动态概念应用结果 &, const 动态概念继续材料 &, std::uint64_t,
                      const 动态概念应用预算 &) const;
    void 核验继续材料(const 动态概念继续材料 &, std::uint64_t, const 动态概念应用预算 &) const;
    概念树概念事实 概念(概念树概念身份, std::uint64_t, std::uint64_t, const 概念树预算 &,
                        bool 普通 = true) const;
    void 根有效(std::uint64_t, const 概念树预算 &) const;
    std::optional<概念树概念事实> 查找(const 概念树动态定义 &, std::uint64_t, const 概念树预算 &) const;
    static void 预算有效(const 动态概念应用预算 &);
    static bool 值域匹配(const 概念树特征值域 &, const 特征类值 &);
    static A 映射(D);
    static A 映射(特征引用读取状态);
    static A 映射(动态类数据状态);
    static A 映射(场景类数据状态);
    static D 数据状态(A);
    static std::uint64_t 关系数(const 存在类结点 &e) {
        return e.子存在组.size() + e.特征组.size() + e.状态组.size() + e.动态组.size();
    }
    template <class T> static bool 包含(const std::vector<T> &group, 稳定编码 id) {
        return std::count_if(group.begin(), group.end(), [&](const auto &x) { return x.目标结点 == id; }) ==
               1;
    }
    static bool 身份小于(概念树概念身份 a, 概念树概念身份 b) {
        return a.值.值 < b.值.值;
    }
    static void 保留失败(动态概念应用结果 &, 动态概念继续材料 &&, A);

    static std::uint64_t 已保存请求数(const 动态概念继续材料 &m) noexcept {
        return m.已确认未绑定尝试.size() + m.建立原请求.has_value() + m.支持原请求.has_value() +
               m.接边原请求.has_value();
    }
    static void 新准备预算(const 动态概念继续材料 &m, const 动态概念应用预算 &b) {
        if (已保存请求数(m) >= b.最大继续尝试数)
            throw 失败{A::数量预算不足};
    }
    template <class R>
    概念树写入结果 执行阶段(std::optional<R> &saved, 动态概念继续材料 &m, 动态概念应用结果 &out,
                            const 动态概念应用预算 &b, bool 可重新准备) {
        auto invoke = [&]() {
            auto request = *saved;
            request.预算 = b.数据;
            概念树写入结果 result;
            if constexpr (std::is_same_v<R, 概念树动态建立请求>)
                result = concepts_.创建动态概念(request);
            else if constexpr (std::is_same_v<R, 概念树支持建立请求>)
                result = concepts_.建立世界支持(request);
            else
                result = concepts_.操作直接上位(request);
            out.步骤回执.push_back(result);
            m.历次回执提示.push_back(result);
            return result;
        };
        auto result = invoke();
        if (!result.成功() && 可重新准备 && result.状态 == D::事实代次漂移 &&
            result.发布状态 == 概念树发布状态::未派发 && !result.发布截止) {
            新准备预算(m, b);
            m.已确认未绑定尝试.push_back(*saved);
            const auto g = 选择读取G(0);
            核验继续材料(m, g, b);
            if constexpr (std::is_same_v<R, 概念树动态建立请求>)
                saved->头.期望事实代次 = g;
            else
                saved->写入头.期望事实代次 = g;
            result = invoke();
        }
        if (!result.成功())
            throw 失败{映射(result.状态)};
        return result;
    }
    template <class R> 概念树写入结果 治理(const R &);
};

动态概念应用状态 动态概念树应用服务::映射(D s) {
    switch (s) {
    case D::入口拒绝:
    case D::根不可退出:
        return A::入口拒绝;
    case D::引用冲突:
    case D::形成环:
        return A::见证不匹配;
    case D::未找到:
        return A::目标未找到;
    case D::目标已退出:
        return A::目标已退出;
    case D::事实代次漂移:
        return A::事实代次漂移;
    case D::数量预算不足:
        return A::数量预算不足;
    case D::历史材料不可用:
        return A::历史证据不可用;
    case D::幂等冲突:
        return A::幂等冲突;
    case D::资源失败:
        return A::资源失败;
    case D::已可能发布:
        return A::可能已发布;
    default:
        return A::内部不一致;
    }
}
动态概念应用状态 动态概念树应用服务::映射(特征引用读取状态 s) {
    switch (s) {
    case 特征引用读取状态::入口拒绝:
        return A::入口拒绝;
    case 特征引用读取状态::引用冲突:
        return A::见证不匹配;
    case 特征引用读取状态::未找到:
        return A::目标未找到;
    case 特征引用读取状态::目标已退出:
        return A::目标已退出;
    case 特征引用读取状态::事实代次漂移:
        return A::事实代次漂移;
    case 特征引用读取状态::数量预算不足:
        return A::数量预算不足;
    case 特征引用读取状态::历史材料不可用:
        return A::历史证据不可用;
    case 特征引用读取状态::资源失败:
        return A::资源失败;
    default:
        return A::内部不一致;
    }
}
动态概念应用状态 动态概念树应用服务::映射(动态类数据状态 s) {
    switch (s) {
    case 动态类数据状态::入口拒绝:
        return A::入口拒绝;
    case 动态类数据状态::未找到:
        return A::目标未找到;
    case 动态类数据状态::目标已退出:
        return A::目标已退出;
    case 动态类数据状态::事实代次漂移:
        return A::事实代次漂移;
    case 动态类数据状态::数量预算不足:
        return A::数量预算不足;
    case 动态类数据状态::历史材料已清理:
        return A::历史证据不可用;
    case 动态类数据状态::资源失败:
        return A::资源失败;
    case 动态类数据状态::引用冲突:
    case 动态类数据状态::成员时间无序:
        return A::见证不匹配;
    default:
        return A::内部不一致;
    }
}
动态概念应用状态 动态概念树应用服务::映射(场景类数据状态 s) {
    switch (s) {
    case 场景类数据状态::入口拒绝:
        return A::入口拒绝;
    case 场景类数据状态::未找到:
        return A::目标未找到;
    case 场景类数据状态::目标已退出:
        return A::目标已退出;
    case 场景类数据状态::事实代次漂移:
        return A::事实代次漂移;
    case 场景类数据状态::数量预算不足:
        return A::数量预算不足;
    case 场景类数据状态::历史材料已清理:
        return A::历史证据不可用;
    case 场景类数据状态::资源失败:
        return A::资源失败;
    case 场景类数据状态::引用冲突:
        return A::见证不匹配;
    default:
        return A::内部不一致;
    }
}
概念树数据状态 动态概念树应用服务::数据状态(A s) {
    switch (s) {
    case A::入口拒绝:
    case A::需显式治理:
        return D::入口拒绝;
    case A::见证不匹配:
        return D::引用冲突;
    case A::目标未找到:
        return D::未找到;
    case A::目标已退出:
        return D::目标已退出;
    case A::事实代次漂移:
        return D::事实代次漂移;
    case A::数量预算不足:
        return D::数量预算不足;
    case A::历史证据不可用:
        return D::历史材料不可用;
    case A::幂等冲突:
        return D::幂等冲突;
    case A::资源失败:
        return D::资源失败;
    case A::可能已发布:
        return D::已可能发布;
    default:
        return D::内部不一致;
    }
}
void 动态概念树应用服务::预算有效(const 动态概念应用预算 &b) {
    const auto &d = b.数据;
    if (!d.最大概念数 || !d.最大关系数 || !d.最大来源数 || !d.最大支持数 || !d.最大世界成员数 ||
        !d.最大特征属性数 || !d.最大动态槽数 || !d.最大动态模板数 || !b.最大场景关系数 || !b.最大存在关系数 ||
        !b.最大状态数 || !b.最大变化特征数 || !b.最大快照特征总数 || !b.最大继续尝试数)
        throw 失败{A::入口拒绝};
}
std::uint64_t 动态概念树应用服务::选择读取G(std::uint64_t expected) const {
    const auto r = concepts_.读取当前事实代次();
    if (!r.成功())
        throw 失败{映射(r.状态)};
    if (expected && r.Gread != expected)
        throw 失败{A::事实代次漂移};
    return r.Gread;
}
概念树概念事实 动态概念树应用服务::概念(概念树概念身份 id, std::uint64_t g, std::uint64_t h,
                                        const 概念树预算 &b, bool ordinary) const {
    const auto r = concepts_.读取概念({{1, g, h}, id, b});
    if (!r.成功())
        throw 失败{映射(r.状态)};
    if (r.Gread != g || r.H != h || r.概念->概念 != id)
        throw 失败{A::内部不一致};
    if (r.概念->根角色 != 概念树根角色::动态 || r.概念->本体根 != root_ || (ordinary && r.概念->是本体根))
        throw 失败{A::见证不匹配};
    return *r.概念;
}
void 动态概念树应用服务::根有效(std::uint64_t g, const 概念树预算 &b) const {
    if (!概念(root_, g, g, b, false).是本体根)
        throw 失败{A::内部不一致};
}
std::optional<概念树概念事实> 动态概念树应用服务::查找(const 概念树动态定义 &d, std::uint64_t g,
                                                       const 概念树预算 &b) const {
    const auto r = concepts_.查找完整定义({{1, g, g}, d, b});
    if (!r.成功())
        throw 失败{映射(r.状态)};
    if (r.Gread != g || r.H != g || r.概念组.size() > 1)
        throw 失败{A::内部不一致};
    if (r.概念组.empty())
        return std::nullopt;
    const auto &f = r.概念组.front();
    if (f.是本体根 || f.根角色 != 概念树根角色::动态 || f.本体根 != root_ || !f.定义 ||
        !std::holds_alternative<概念树动态定义>(*f.定义) || std::get<概念树动态定义>(*f.定义) != d)
        throw 失败{A::内部不一致};
    return f;
}
bool 动态概念树应用服务::值域匹配(const 概念树特征值域 &domain, const 特征类值 &value) {
    const auto *direct = std::get_if<特征直接值>(&value);
    if (!direct || direct->valueless_by_exception() || domain.valueless_by_exception())
        return false;
    if (const auto *interval = std::get_if<概念树I64区间>(&domain)) {
        const auto *v = std::get_if<std::int64_t>(direct);
        return v && interval->下界 <= *v && *v <= interval->上界;
    }
    const auto &exact = std::get<概念树精确值>(domain);
    if (exact.valueless_by_exception())
        return false;
    return std::visit(
        [&](const auto &v) {
            const auto *x = std::get_if<std::decay_t<decltype(v)>>(direct);
            return x && *x == v;
        },
        exact);
}
概念树动态定义 动态概念树应用服务::校验形成见证(const 动态概念形成请求 &r, std::uint64_t g) const {
    预算有效(r.预算);
    if (r.合同版本 != 1 || !r.H || r.H > g || !有效(r.场景.值) || !有效(r.主体.值) || !有效(r.动态.值) ||
        !r.创建键 || !r.支持键 || r.创建键 == r.支持键 || r.槽见证.size() < 2)
        throw 失败{A::入口拒绝};
    const auto &b = r.预算;
    if (r.槽见证.size() > b.最大状态数 || r.槽见证.size() > b.数据.最大动态槽数)
        throw 失败{A::数量预算不足};
    auto scene = scenes_.读取场景历史事实({1, g, r.H, r.场景.值, b.最大场景关系数});
    if (!scene.成功())
        throw 失败{映射(scene.状态)};
    if (scene.Gread != g || scene.H != r.H || scene.场景结点->结点 != r.场景.值)
        throw 失败{A::内部不一致};
    auto dynamic = dynamics_.读取动态历史事实(
        {1, g, r.H, r.动态.值, b.最大状态数, b.最大变化特征数, b.最大快照特征总数});
    if (!dynamic.成功())
        throw 失败{映射(dynamic.状态)};
    if (dynamic.Gread != g || dynamic.H != r.H || dynamic.动态结点->结点 != r.动态.值)
        throw 失败{A::内部不一致};
    std::map<std::uint64_t, 存在类结点> hosts;
    std::uint64_t remaining = b.最大存在关系数;
    auto host = [&](std::uint64_t h) -> const 存在类结点 & {
        auto it = hosts.find(h);
        if (it != hosts.end())
            return it->second;
        if (!remaining)
            throw 失败{A::数量预算不足};
        auto e = existences_.读取存在历史事实({1, g, h, r.主体.值, remaining});
        if (!e.成功())
            throw 失败{映射(e.状态)};
        if (e.Gread != g || e.H != h || e.存在->结点 != r.主体.值)
            throw 失败{A::内部不一致};
        const auto count = 关系数(*e.存在);
        if (count > remaining)
            throw 失败{A::内部不一致};
        remaining -= count;
        return hosts.emplace(h, std::move(*e.存在)).first->second;
    };
    const auto &e = host(r.H);
    const auto &sc = *scene.场景结点;
    if (!包含(sc.存在组, r.主体.值) || !包含(sc.动态组, r.动态.值) || !包含(e.动态组, r.动态.值) ||
        dynamic.状态组.size() != r.槽见证.size())
        throw 失败{A::见证不匹配};
    std::map<std::uint64_t, 概念树动态模板事实> templates;
    std::uint64_t used = 0;
    概念树动态定义 def{r.场景, r.主体, {}};
    const auto start = dynamic.状态组.front().时间;
    for (std::size_t i = 0; i < r.槽见证.size(); ++i) {
        const auto &q = dynamic.状态组[i];
        const auto &witness = r.槽见证[i];
        if (witness.状态 != q.结点 || !包含(sc.状态组, q.结点) || !包含(e.状态组, q.结点) ||
            witness.约束.empty())
            throw 失败{A::见证不匹配};
        if (witness.约束.size() > b.数据.最大动态模板数 - used)
            throw 失败{A::数量预算不足};
        used += witness.约束.size();
        if (q.时间 < start || q.时间 <= 0)
            throw 失败{A::见证不匹配};
        概念树动态时间槽 slot{q.时间 - start, {}};
        const auto &origin = host(q.创建事实代次);
        std::set<std::uint64_t> ids;
        for (const auto &w : witness.约束) {
            if (!有效(w.模板.值) || !有效(w.原快照特征) || !有效(w.变化目标特征) ||
                !ids.insert(w.模板.值.值).second)
                throw 失败{A::入口拒绝};
            const auto f = std::find_if(q.特征值快照组.begin(), q.特征值快照组.end(),
                                        [&](const auto &x) { return x.结点 == w.原快照特征; });
            const auto changed = std::find_if(dynamic.变化特征组.begin(), dynamic.变化特征组.end(),
                                              [&](const auto &x) { return x.结点 == w.变化目标特征; });
            if (f == q.特征值快照组.end() || changed == dynamic.变化特征组.end() ||
                !包含(origin.特征组, w.原快照特征) || !包含(e.特征组, w.变化目标特征))
                throw 失败{A::见证不匹配};
            auto it = templates.find(w.模板.值.值);
            if (it == templates.end()) {
                const auto t = concepts_.读取动态特征模板({{1, g, g}, w.模板, b.数据});
                if (!t.成功())
                    throw 失败{映射(t.状态)};
                if (t.Gread != g || t.H != g || t.模板->模板 != w.模板)
                    throw 失败{A::内部不一致};
                it = templates.emplace(w.模板.值.值, *t.模板).first;
            }
            const auto &t = it->second;
            if (t.治理状态 == 概念树生命周期状态::退役)
                throw 失败{A::需显式治理};
            if (t.定义.形成宿主 != r.主体 || f->特征类型 != changed->特征类型 ||
                f->特征类型 != t.定义.特征类型.值 || !值域匹配(t.定义.值域, f->特征值))
                throw 失败{A::见证不匹配};
            slot.模板组.push_back(w.模板);
        }
        std::sort(slot.模板组.begin(), slot.模板组.end(), 身份小于);
        def.时间槽组.push_back(std::move(slot));
    }
    (void)选择读取G(g);
    return def;
}
std::optional<概念树动态定义> 动态概念树应用服务::构造上位定义(
    const 概念树概念事实 &f, const std::vector<std::vector<概念树概念身份>> &selected) const {
    if (f.是本体根 || f.根角色 != 概念树根角色::动态 || !f.定义 ||
        !std::holds_alternative<概念树动态定义>(*f.定义))
        throw 失败{A::见证不匹配};
    auto out = std::get<概念树动态定义>(*f.定义);
    if (selected.size() != out.时间槽组.size())
        throw 失败{A::入口拒绝};
    if (std::all_of(selected.begin(), selected.end(), [](const auto &q) { return q.empty(); }))
        return std::nullopt;
    bool strict = false;
    for (std::size_t i = 0; i < selected.size(); ++i) {
        auto ids = selected[i];
        std::sort(ids.begin(), ids.end(), 身份小于);
        const auto &original = out.时间槽组[i].模板组;
        if (ids.empty() || std::adjacent_find(ids.begin(), ids.end()) != ids.end())
            throw 失败{A::入口拒绝};
        for (auto id : ids)
            if (!有效(id.值) || std::find(original.begin(), original.end(), id) == original.end())
                throw 失败{A::入口拒绝};
        if (ids.size() < original.size())
            strict = true;
        out.时间槽组[i].模板组 = std::move(ids);
    }
    if (!strict)
        throw 失败{A::入口拒绝};
    return out;
}

void 动态概念树应用服务::核验继续材料(const 动态概念继续材料 &m, std::uint64_t g,
                                      const 动态概念应用预算 &b) const {
    预算有效(b);
    if (m.合同版本 != 1 || m.原请求.valueless_by_exception() || !m.首次读取G || m.首次读取G > g ||
        (m.已定位概念 && !有效(m.已定位概念->值)))
        throw 失败{A::入口拒绝};
    const auto count = 已保存请求数(m);
    if (count > b.最大继续尝试数)
        throw 失败{A::数量预算不足};
    根有效(g, b.数据);
    概念树动态定义 target;
    std::vector<概念树动态依据项> sources;
    std::uint64_t createKey = 0;
    std::optional<概念树概念身份> child;
    bool rootMode = false;
    if (const auto *r = std::get_if<动态概念形成请求>(&m.原请求)) {
        if (m.接边原请求 || (r->Gread && r->Gread != m.首次读取G))
            throw 失败{A::入口拒绝};
        auto check = *r;
        check.预算 = b;
        target = 校验形成见证(check, g);
        if (target != m.冻结定义)
            throw 失败{A::见证不匹配};
        sources = {{r->动态, r->H}};
        createKey = r->创建键;
    } else {
        const auto &upperRequest = std::get<动态概念上位请求>(m.原请求);
        if (upperRequest.合同版本 != 1 || !upperRequest.创建键 || !upperRequest.接边键 ||
            upperRequest.创建键 == upperRequest.接边键 || m.支持原请求 ||
            (upperRequest.Gread && upperRequest.Gread != m.首次读取G))
            throw 失败{A::入口拒绝};
        const auto original = 概念(upperRequest.下位, g, m.首次读取G, b.数据);
        if (!original.定义 || std::get<概念树动态定义>(*original.定义) != m.冻结定义)
            throw 失败{A::见证不匹配};
        const auto parent = 构造上位定义(original, upperRequest.保留槽模板);
        rootMode = !parent;
        if (parent)
            target = *parent;
        if (rootMode && m.建立原请求)
            throw 失败{A::入口拒绝};
        sources = {{upperRequest.下位, m.首次读取G}};
        createKey = upperRequest.创建键;
        child = upperRequest.下位;
    }
    auto checkHeader = [&](const 概念树写入头 &h, std::uint64_t key) {
        if (h.合同版本 != 1 || h.幂等身份 != key || h.期望事实代次 < m.首次读取G || h.期望事实代次 > g)
            throw 失败{A::入口拒绝};
    };
    auto checkCreate = [&](const 概念树动态建立请求 &r) {
        checkHeader(r.头, createKey);
        if (rootMode || r.定义 != target || r.依据 != sources ||
            r.直接上位 != std::vector<概念树概念身份>{root_})
            throw 失败{A::见证不匹配};
    };
    auto checkSupport = [&](const 概念树支持建立请求 &q) {
        const auto *r = std::get_if<动态概念形成请求>(&m.原请求);
        if (!r || !m.已定位概念)
            throw 失败{A::入口拒绝};
        checkHeader(q.写入头, r->支持键);
        if (q.目标概念 != *m.已定位概念 || q.世界引用 != 概念树世界引用{r->动态} || q.证据H != r->H)
            throw 失败{A::见证不匹配};
        const auto f = 概念(q.目标概念, g, q.写入头.期望事实代次, b.数据);
        if (!f.定义 || std::get<概念树动态定义>(*f.定义) != target)
            throw 失败{A::见证不匹配};
    };
    auto checkEdge = [&](const 概念树上位操作请求 &q) {
        const auto *r = std::get_if<动态概念上位请求>(&m.原请求);
        if (!r || !m.已定位概念)
            throw 失败{A::入口拒绝};
        checkHeader(q.写入头, r->接边键);
        if (q.下位 != r->下位 || q.新上位 != m.已定位概念)
            throw 失败{A::见证不匹配};
        const auto lower = 概念(q.下位, g, q.写入头.期望事实代次, b.数据);
        if (!lower.定义 || std::get<概念树动态定义>(*lower.定义) != m.冻结定义)
            throw 失败{A::见证不匹配};
        if (rootMode) {
            if (*q.新上位 != root_)
                throw 失败{A::见证不匹配};
        } else {
            const auto upper = 概念(*q.新上位, g, q.写入头.期望事实代次, b.数据);
            if (!upper.定义 || std::get<概念树动态定义>(*upper.定义) != target)
                throw 失败{A::见证不匹配};
        }
        const auto rootEdge = std::find_if(lower.直接上位组.begin(), lower.直接上位组.end(),
                                           [&](const auto &e) { return e.上位 == root_; });
        const bool replace = !rootMode && rootEdge != lower.直接上位组.end();
        if (replace) {
            if (q.操作 != 概念树上位操作::替换 || q.旧上位 != root_ || q.旧关系 != rootEdge->关系)
                throw 失败{A::见证不匹配};
        } else if (q.操作 != 概念树上位操作::新增 || q.旧上位 || q.旧关系)
            throw 失败{A::见证不匹配};
    };
    if (m.建立原请求)
        checkCreate(*m.建立原请求);
    if (m.支持原请求)
        checkSupport(*m.支持原请求);
    if (m.接边原请求)
        checkEdge(*m.接边原请求);
    // 保存的未绑定尝试也只能承载本业务，不能借预算或历史回执引入新写类型。
    for (const auto &old : m.已确认未绑定尝试)
        std::visit(
            [&](const auto &r) {
                using T = std::decay_t<decltype(r)>;
                if constexpr (std::is_same_v<T, 概念树动态建立请求>)
                    checkCreate(r);
                else if constexpr (std::is_same_v<T, 概念树支持建立请求>)
                    checkSupport(r);
                else if constexpr (std::is_same_v<T, 概念树上位操作请求>)
                    checkEdge(r);
                else
                    throw 失败{A::入口拒绝};
            },
            old);
    (void)选择读取G(g);
}
void 动态概念树应用服务::保留失败(动态概念应用结果 &out, 动态概念继续材料 &&m, A status) {
    out.状态 = status;
    if ((out.阶段 == Stage::支持已确认 || out.阶段 == Stage::接边已确认) &&
        (status == A::事实代次漂移 || status == A::目标未找到 || status == A::目标已退出 ||
         status == A::历史证据不可用))
        out.状态 = A::待继续;
    auto verified = [&](const auto &saved) {
        if (!saved)
            return true;
        return std::any_of(out.步骤回执.begin(), out.步骤回执.end(), [&](const auto &receipt) {
            if (!receipt.原请求)
                return false;
            const auto *actual =
                std::get_if<typename std::decay_t<decltype(saved)>::value_type>(&*receipt.原请求);
            if (!actual)
                return false;
            const auto same = [&] {
                using T = typename std::decay_t<decltype(saved)>::value_type;
                if constexpr (std::is_same_v<T, 概念树动态建立请求>)
                    return saved->头 == actual->头 && saved->定义 == actual->定义 &&
                           saved->直接上位 == actual->直接上位 && saved->依据 == actual->依据;
                else if constexpr (std::is_same_v<T, 概念树支持建立请求>)
                    return saved->写入头 == actual->写入头 && saved->目标概念 == actual->目标概念 &&
                           saved->世界引用 == actual->世界引用 && saved->证据H == actual->证据H;
                else
                    return saved->写入头 == actual->写入头 && saved->操作 == actual->操作 &&
                           saved->下位 == actual->下位 && saved->旧上位 == actual->旧上位 &&
                           saved->新上位 == actual->新上位 && saved->旧关系 == actual->旧关系;
            };
            return same() &&
                   (receipt.成功() || (receipt.发布状态 == 概念树发布状态::未派发 && !receipt.发布截止));
        });
    };
    if (!verified(m.建立原请求) || !verified(m.支持原请求) || !verified(m.接边原请求)) {
        out.状态 = A::可能已发布;
        if (!out.概念 && !out.支持)
            out.阶段 = Stage::先前阶段待核验;
    }
    out.继续 = std::move(m);
}
动态概念应用结果 动态概念树应用服务::形成或复用动态概念(const 动态概念形成请求 &r) {
    动态概念应用结果 out;
    动态概念继续材料 m;
    try {
        m.原请求 = r;
        const auto g = 选择读取G(r.Gread);
        out.Gread = g;
        m.首次读取G = g;
        根有效(g, r.预算.数据);
        m.冻结定义 = 校验形成见证(r, g);
        return 推进形成(std::move(m), g, r.预算);
    } catch (const 失败 &e) {
        保留失败(out, std::move(m), e.状态);
    } catch (const std::bad_alloc &) {
        保留失败(out, std::move(m), A::资源失败);
    } catch (const std::length_error &) {
        保留失败(out, std::move(m), A::资源失败);
    } catch (...) {
        保留失败(out, std::move(m), A::内部不一致);
    }
    return out;
}
动态概念应用结果 动态概念树应用服务::形成上位动态概念(const 动态概念上位请求 &r) {
    动态概念应用结果 out;
    动态概念继续材料 m;
    try {
        m.原请求 = r;
        预算有效(r.预算);
        if (r.合同版本 != 1 || !r.创建键 || !r.接边键 || r.创建键 == r.接边键)
            throw 失败{A::入口拒绝};
        const auto g = 选择读取G(r.Gread);
        out.Gread = g;
        m.首次读取G = g;
        根有效(g, r.预算.数据);
        const auto f = 概念(r.下位, g, g, r.预算.数据);
        m.冻结定义 = std::get<概念树动态定义>(*f.定义);
        (void)构造上位定义(f, r.保留槽模板);
        return 推进上位(std::move(m), g, r.预算);
    } catch (const 失败 &e) {
        保留失败(out, std::move(m), e.状态);
    } catch (const std::bad_alloc &) {
        保留失败(out, std::move(m), A::资源失败);
    } catch (const std::length_error &) {
        保留失败(out, std::move(m), A::资源失败);
    } catch (...) {
        保留失败(out, std::move(m), A::内部不一致);
    }
    return out;
}
动态概念应用结果 动态概念树应用服务::继续动态概念操作(const 动态概念继续请求 &r) {
    动态概念应用结果 out;
    动态概念继续材料 m;
    try {
        m = r.材料;
        if (r.合同版本 != 1)
            throw 失败{A::入口拒绝};
        const auto g = 选择读取G(r.Gread);
        out.Gread = g;
        核验继续材料(m, g, r.本次预算);
        if (std::holds_alternative<动态概念形成请求>(m.原请求))
            return 推进形成(std::move(m), g, r.本次预算);
        return 推进上位(std::move(m), g, r.本次预算);
    } catch (const 失败 &e) {
        保留失败(out, std::move(m), e.状态);
    } catch (const std::bad_alloc &) {
        保留失败(out, std::move(m), A::资源失败);
    } catch (const std::length_error &) {
        保留失败(out, std::move(m), A::资源失败);
    } catch (...) {
        保留失败(out, std::move(m), A::内部不一致);
    }
    return out;
}
动态概念应用结果 动态概念树应用服务::推进形成(动态概念继续材料 m, std::uint64_t g,
                                              const 动态概念应用预算 &b) {
    动态概念应用结果 out;
    out.Gread = g;
    try {
        核验继续材料(m, g, b);
        const auto &original = std::get<动态概念形成请求>(m.原请求);
        const bool hadCreate = m.建立原请求.has_value(), hadSupport = m.支持原请求.has_value();
        if (m.建立原请求) {
            const auto r = 执行阶段(m.建立原请求, m, out, b, hadCreate);
            if (!r.概念)
                throw 失败{A::内部不一致};
            if (m.已定位概念 && *m.已定位概念 != r.概念->概念)
                throw 失败{A::见证不匹配};
            m.已定位概念 = r.概念->概念;
        } else if (!m.已定位概念) {
            auto found = 查找(m.冻结定义, g, b.数据);
            if (found)
                m.已定位概念 = found->概念;
            else {
                新准备预算(m, b);
                m.建立原请求 = 概念树动态建立请求{
                    {1, g, original.创建键}, m.冻结定义, {root_}, {{original.动态, original.H}}, b.数据};
                const auto r = 执行阶段(m.建立原请求, m, out, b, false);
                if (!r.概念)
                    throw 失败{A::内部不一致};
                m.已定位概念 = r.概念->概念;
            }
        }
        g = 选择读取G(0);
        out.Gread = g;
        auto fact = 概念(*m.已定位概念, g, g, b.数据);
        if (!fact.定义 || std::get<概念树动态定义>(*fact.定义) != m.冻结定义)
            throw 失败{A::见证不匹配};
        out.概念 = fact;
        out.阶段 = Stage::概念已确定;
        if (fact.治理状态 == 概念树生命周期状态::退役)
            throw 失败{A::需显式治理};
        auto check = original;
        check.预算 = b;
        if (校验形成见证(check, g) != m.冻结定义)
            throw 失败{A::见证不匹配};
        // 当前端点准入与原 H 证据是不同读取，不能以旧见证跳过新关系准入。
        const auto current = dynamics_.读取动态历史事实(
            {1, g, g, original.动态.值, b.最大状态数, b.最大变化特征数, b.最大快照特征总数});
        if (!current.成功())
            throw 失败{映射(current.状态)};
        if (current.Gread != g || current.H != g || current.动态结点->结点 != original.动态.值)
            throw 失败{A::内部不一致};
        if (m.支持原请求) {
            const auto r = 执行阶段(m.支持原请求, m, out, b, hadSupport);
            if (!r.支持)
                throw 失败{A::内部不一致};
            out.支持 = r.支持;
        } else {
            const auto byConcept = concepts_.按概念读取支持({{1, g, g}, *m.已定位概念, b.数据});
            const auto byWorld = concepts_.按世界读取支持({{1, g, g}, original.动态, b.数据});
            if (!byConcept.成功())
                throw 失败{映射(byConcept.状态)};
            if (!byWorld.成功())
                throw 失败{映射(byWorld.状态)};
            if (byConcept.Gread != g || byConcept.H != g || byWorld.Gread != g || byWorld.H != g)
                throw 失败{A::内部不一致};
            for (const auto &s : byConcept.支持组)
                if (s.世界引用 == 概念树世界引用{original.动态}) {
                    if (out.支持 || std::count(byWorld.支持组.begin(), byWorld.支持组.end(), s) != 1)
                        throw 失败{A::内部不一致};
                    out.支持 = s;
                }
            if (!out.支持) {
                新准备预算(m, b);
                m.支持原请求 = 概念树支持建立请求{
                    {1, g, original.支持键}, *m.已定位概念, original.动态, original.H, b.数据};
                const auto r = 执行阶段(m.支持原请求, m, out, b, false);
                if (!r.支持)
                    throw 失败{A::内部不一致};
                out.支持 = r.支持;
            }
        }
        out.阶段 = Stage::支持已确认;
        g = 选择读取G(0);
        核验最终结果(out, m, g, b);
        out.继续 = m;
        if (!out.成功())
            throw 失败{A::内部不一致};
        return out;
    } catch (const 失败 &e) {
        保留失败(out, std::move(m), e.状态);
    } catch (const std::bad_alloc &) {
        保留失败(out, std::move(m), A::资源失败);
    } catch (const std::length_error &) {
        保留失败(out, std::move(m), A::资源失败);
    } catch (...) {
        保留失败(out, std::move(m), A::内部不一致);
    }
    return out;
}
动态概念应用结果 动态概念树应用服务::推进上位(动态概念继续材料 m, std::uint64_t g,
                                              const 动态概念应用预算 &b) {
    动态概念应用结果 out;
    out.Gread = g;
    try {
        核验继续材料(m, g, b);
        const auto &original = std::get<动态概念上位请求>(m.原请求);
        const bool hadCreate = m.建立原请求.has_value(), hadEdge = m.接边原请求.has_value();
        auto lower = 概念(original.下位, g, g, b.数据);
        if (std::get<概念树动态定义>(*lower.定义) != m.冻结定义)
            throw 失败{A::见证不匹配};
        if (lower.治理状态 == 概念树生命周期状态::退役)
            throw 失败{A::需显式治理};
        const auto target = 构造上位定义(lower, original.保留槽模板);
        if (!target)
            m.已定位概念 = root_;
        else if (m.建立原请求) {
            const auto r = 执行阶段(m.建立原请求, m, out, b, hadCreate);
            if (!r.概念)
                throw 失败{A::内部不一致};
            if (m.已定位概念 && *m.已定位概念 != r.概念->概念)
                throw 失败{A::见证不匹配};
            m.已定位概念 = r.概念->概念;
        } else if (!m.已定位概念) {
            const auto found = 查找(*target, g, b.数据);
            if (found)
                m.已定位概念 = found->概念;
            else {
                新准备预算(m, b);
                m.建立原请求 = 概念树动态建立请求{
                    {1, g, original.创建键}, *target, {root_}, {{original.下位, m.首次读取G}}, b.数据};
                const auto r = 执行阶段(m.建立原请求, m, out, b, false);
                if (!r.概念)
                    throw 失败{A::内部不一致};
                m.已定位概念 = r.概念->概念;
            }
        }
        g = 选择读取G(0);
        out.Gread = g;
        auto upper = 概念(*m.已定位概念, g, g, b.数据, target.has_value());
        if (target && (!upper.定义 || std::get<概念树动态定义>(*upper.定义) != *target))
            throw 失败{A::见证不匹配};
        out.概念 = upper;
        out.阶段 = Stage::上位已确定;
        if (upper.治理状态 == 概念树生命周期状态::退役)
            throw 失败{A::需显式治理};
        lower = 概念(original.下位, g, g, b.数据);
        if (lower.治理状态 == 概念树生命周期状态::退役)
            throw 失败{A::需显式治理};
        if (构造上位定义(lower, original.保留槽模板) != target)
            throw 失败{A::见证不匹配};
        if (m.接边原请求)
            (void)执行阶段(m.接边原请求, m, out, b, hadEdge);
        else {
            const auto existing = std::find_if(lower.直接上位组.begin(), lower.直接上位组.end(),
                                               [&](const auto &e) { return e.上位 == *m.已定位概念; });
            if (existing == lower.直接上位组.end()) {
                const auto old = std::find_if(lower.直接上位组.begin(), lower.直接上位组.end(),
                                              [&](const auto &e) { return e.上位 == root_; });
                概念树上位操作请求 edge{{1, g, original.接边键},
                                        概念树上位操作::新增,
                                        original.下位,
                                        std::nullopt,
                                        m.已定位概念,
                                        std::nullopt,
                                        b.数据};
                if (target && old != lower.直接上位组.end()) {
                    edge.操作 = 概念树上位操作::替换;
                    edge.旧上位 = root_;
                    edge.旧关系 = old->关系;
                }
                新准备预算(m, b);
                m.接边原请求 = edge;
                (void)执行阶段(m.接边原请求, m, out, b, false);
            }
        }
        out.阶段 = Stage::接边已确认;
        g = 选择读取G(0);
        核验最终结果(out, m, g, b);
        out.继续 = m;
        if (!out.成功())
            throw 失败{A::内部不一致};
        return out;
    } catch (const 失败 &e) {
        保留失败(out, std::move(m), e.状态);
    } catch (const std::bad_alloc &) {
        保留失败(out, std::move(m), A::资源失败);
    } catch (const std::length_error &) {
        保留失败(out, std::move(m), A::资源失败);
    } catch (...) {
        保留失败(out, std::move(m), A::内部不一致);
    }
    return out;
}

void 动态概念树应用服务::核验最终结果(动态概念应用结果 &out, const 动态概念继续材料 &m, std::uint64_t g,
                                      const 动态概念应用预算 &b) const {
    if (!m.已定位概念)
        throw 失败{A::内部不一致};
    根有效(g, b.数据);
    const auto graph = concepts_.读取概念图({{1, g, g}, 概念树根角色::动态, true, b.数据});
    if (!graph.成功())
        throw 失败{映射(graph.状态)};
    if (graph.Gread != g || graph.H != g || graph.根身份 != root_ || !graph.支持组)
        throw 失败{A::内部不一致};
    const bool formation = std::holds_alternative<动态概念形成请求>(m.原请求);
    auto target = 概念(*m.已定位概念, g, g, b.数据, formation || *m.已定位概念 != root_);
    if (std::count(graph.概念组.begin(), graph.概念组.end(), target) != 1)
        throw 失败{A::内部不一致};
    if (target.治理状态 == 概念树生命周期状态::退役)
        throw 失败{A::需显式治理};
    out.直接边.clear();
    if (formation) {
        const auto &original = std::get<动态概念形成请求>(m.原请求);
        if (!target.定义 || std::get<概念树动态定义>(*target.定义) != m.冻结定义)
            throw 失败{A::见证不匹配};
        const auto byC = concepts_.按概念读取支持({{1, g, g}, target.概念, b.数据});
        const auto byD = concepts_.按世界读取支持({{1, g, g}, original.动态, b.数据});
        if (!byC.成功())
            throw 失败{映射(byC.状态)};
        if (!byD.成功())
            throw 失败{映射(byD.状态)};
        if (byC.Gread != g || byD.Gread != g || byC.H != g || byD.H != g)
            throw 失败{A::内部不一致};
        std::optional<概念树支持事实> support;
        for (const auto &s : byC.支持组)
            if (s.世界引用 == 概念树世界引用{original.动态}) {
                if (support || std::count(byD.支持组.begin(), byD.支持组.end(), s) != 1 ||
                    std::count(graph.支持组->begin(), graph.支持组->end(), s) != 1)
                    throw 失败{A::内部不一致};
                support = s;
            }
        if (!support || !out.支持)
            throw 失败{A::目标未找到};
        auto previous = *out.支持;
        previous.动态证据 = 概念树动态证据状态::未展开;
        if (previous != *support)
            throw 失败{A::见证不匹配};
        const auto audit = concepts_.核验动态支持依据({{1, g, g}, support->记录, b.数据});
        if (!audit.成功())
            throw 失败{映射(audit.状态)};
        if (audit.Gread != g || audit.H != g || !audit.支持)
            throw 失败{A::内部不一致};
        auto structure = *audit.支持;
        structure.动态证据 = 概念树动态证据状态::未展开;
        if (structure != *support)
            throw 失败{A::内部不一致};
        if (audit.支持->动态证据 == 概念树动态证据状态::历史已清理)
            throw 失败{A::历史证据不可用};
        if (audit.支持->动态证据 != 概念树动态证据状态::已核验)
            throw 失败{A::内部不一致};
        out.支持 = audit.支持;
        out.直接边 = target.直接上位组;
        out.阶段 = Stage::支持已确认;
    } else {
        const auto &original = std::get<动态概念上位请求>(m.原请求);
        const auto lower = 概念(original.下位, g, g, b.数据);
        if (lower.治理状态 == 概念树生命周期状态::退役)
            throw 失败{A::需显式治理};
        if (!lower.定义 || std::get<概念树动态定义>(*lower.定义) != m.冻结定义)
            throw 失败{A::见证不匹配};
        const auto expected = 构造上位定义(lower, original.保留槽模板);
        if (expected) {
            if (!target.定义 || std::get<概念树动态定义>(*target.定义) != *expected)
                throw 失败{A::见证不匹配};
        } else if (!target.是本体根 || target.概念 != root_)
            throw 失败{A::见证不匹配};
        if (std::count_if(lower.直接上位组.begin(), lower.直接上位组.end(),
                          [&](const auto &e) { return e.上位 == target.概念; }) != 1)
            throw 失败{A::目标未找到};
        if (m.接边原请求) {
            const auto &request = *m.接边原请求;
            const auto before = 概念(original.下位, g, request.写入头.期望事实代次, b.数据);
            for (const auto &edge : before.直接上位组) {
                if (request.操作 == 概念树上位操作::替换 && request.旧关系 == edge.关系)
                    continue;
                if (std::count(lower.直接上位组.begin(), lower.直接上位组.end(), edge) != 1)
                    throw 失败{A::见证不匹配};
            }
            if (request.操作 == 概念树上位操作::替换) {
                if (!request.旧关系 ||
                    std::any_of(lower.直接上位组.begin(), lower.直接上位组.end(),
                                [&](const auto &e) { return e.关系 == *request.旧关系 || e.上位 == root_; }))
                    throw 失败{A::见证不匹配};
                const bool proved =
                    std::any_of(out.步骤回执.begin(), out.步骤回执.end(), [&](const auto &receipt) {
                        if (!receipt.成功() || !receipt.发布截止 || *receipt.发布截止 > g || !receipt.原请求)
                            return false;
                        const auto *r = std::get_if<概念树上位操作请求>(&*receipt.原请求);
                        if (!r)
                            return false;
                        auto normalized = request;
                        normalized.预算 = r->预算;
                        return normalized == *r && std::count(receipt.已退出事实.begin(),
                                                              receipt.已退出事实.end(), *request.旧关系) == 1;
                    });
                if (!proved)
                    throw 失败{A::内部不一致};
            }
        }
        out.直接边 = lower.直接上位组;
        out.支持.reset();
        out.阶段 = Stage::接边已确认;
    }
    for (const auto &edge : out.直接边)
        if (std::count(graph.直接边组.begin(), graph.直接边组.end(), edge) != 1)
            throw 失败{A::内部不一致};
    (void)选择读取G(g);
    out.概念 = std::move(target);
    out.Gread = g;
    out.状态 = A::已完成;
}

template <class R> 概念树写入结果 动态概念树应用服务::治理(const R &r) {
    概念树写入结果 out;
    try {
        out.原请求 = r;
        const auto &head = [&]() -> const 概念树写入头 & {
            if constexpr (std::is_same_v<R, 概念树动态依据释放请求>)
                return r.头;
            else
                return r.写入头;
        }();
        const auto g = 选择读取G(0);
        out.事实代次 = g;
        if (head.合同版本 != 1 || !head.期望事实代次 || head.期望事实代次 > g || !head.幂等身份)
            throw 失败{A::入口拒绝};
        const auto h = head.期望事实代次;
        根有效(g, r.预算);
        if constexpr (std::is_same_v<R, 概念树生命周期迁移请求> || std::is_same_v<R, 概念树概念退出请求>) {
            (void)概念(r.概念, g, h, r.预算);
        } else if constexpr (std::is_same_v<R, 概念树支持退出请求>) {
            if (!std::holds_alternative<概念树动态引用>(r.预期支持.世界引用))
                throw 失败{A::入口拒绝};
            (void)概念(r.预期支持.目标概念, g, h, r.预算);
            const auto c = concepts_.按概念读取支持({{1, g, h}, r.预期支持.目标概念, r.预算});
            const auto d = concepts_.按世界读取支持({{1, g, h}, r.预期支持.世界引用, r.预算});
            if (!c.成功())
                throw 失败{映射(c.状态)};
            if (!d.成功())
                throw 失败{映射(d.状态)};
            if (c.Gread != g || d.Gread != g || c.H != h || d.H != h)
                throw 失败{A::内部不一致};
            auto expected = r.预期支持;
            expected.动态证据 = 概念树动态证据状态::未展开;
            if (std::count(c.支持组.begin(), c.支持组.end(), expected) != 1 ||
                std::count(d.支持组.begin(), d.支持组.end(), expected) != 1)
                throw 失败{A::见证不匹配};
        } else if constexpr (std::is_same_v<R, 概念树动态依据释放请求>) {
            const auto f = 概念(r.所属概念, g, h, r.预算);
            std::vector<概念树动态依据事实> current;
            for (const auto &x : f.动态依据组)
                if (!x.当前引用已释放)
                    current.push_back(x);
            auto expected = r.预期当前依据;
            auto order = [](const auto &a, const auto &b) { return a.引用关系.值 < b.引用关系.值; };
            std::sort(current.begin(), current.end(), order);
            std::sort(expected.begin(), expected.end(), order);
            if (current != expected)
                throw 失败{A::见证不匹配};
        } else {
            if (r.世界引用.valueless_by_exception() || (!std::holds_alternative<概念树场景引用>(r.世界引用) &&
                                                        !std::holds_alternative<概念树存在引用>(r.世界引用)))
                throw 失败{A::入口拒绝};
            if (r.预期当前引用.size() > r.预算.最大关系数)
                throw 失败{A::数量预算不足};
            for (const auto &x : r.预期当前引用) {
                const auto f = 概念(x.所属概念, g, h, r.预算);
                const auto &def = std::get<概念树动态定义>(*f.定义);
                if (x.记录 != f.定义记录 || x.世界引用 != r.世界引用 ||
                    (x.世界引用 != 概念树形成世界引用{def.场景} &&
                     x.世界引用 != 概念树形成世界引用{def.主体}))
                    throw 失败{A::见证不匹配};
            }
        }
        (void)选择读取G(g);
        if constexpr (std::is_same_v<R, 概念树生命周期迁移请求>)
            return concepts_.迁移概念生命周期(r);
        else if constexpr (std::is_same_v<R, 概念树概念退出请求>)
            return concepts_.退出概念(r);
        else if constexpr (std::is_same_v<R, 概念树支持退出请求>)
            return concepts_.退出世界支持(r);
        else if constexpr (std::is_same_v<R, 概念树动态依据释放请求>)
            return concepts_.释放动态形成依据(r);
        else
            return concepts_.释放形成来源当前引用(r);
    } catch (const 失败 &e) {
        out.状态 = 数据状态(e.状态);
    } catch (const std::bad_alloc &) {
        out.状态 = D::资源失败;
    } catch (const std::length_error &) {
        out.状态 = D::资源失败;
    } catch (...) {
        out.状态 = D::内部不一致;
    }
    // 前置读取失败不作原 key 从未发布的证明；保持全部原请求交调用者处理。
    if (out.状态 == D::资源失败 || out.状态 == D::内部不一致 || out.状态 == D::历史材料不可用 ||
        out.状态 == D::事实代次漂移)
        out.发布状态 = 概念树发布状态::可能已发布;
    return out;
}
概念树写入结果 动态概念树应用服务::迁移动态概念生命周期(const 概念树生命周期迁移请求 &r) {
    return 治理(r);
}
概念树写入结果 动态概念树应用服务::退出动态概念(const 概念树概念退出请求 &r) {
    return 治理(r);
}
概念树写入结果 动态概念树应用服务::退出动态支持(const 概念树支持退出请求 &r) {
    return 治理(r);
}
概念树写入结果 动态概念树应用服务::释放动态形成依据(const 概念树动态依据释放请求 &r) {
    return 治理(r);
}
概念树写入结果 动态概念树应用服务::释放动态上下文引用(const 概念树形成引用释放请求 &r) {
    return 治理(r);
}
概念树图结果 动态概念树应用服务::读取动态概念树(const 概念树图读取请求 &r) const {
    if (r.根角色 != 概念树根角色::动态)
        return {};
    return concepts_.读取概念图(r);
}
bool 动态概念应用结果::成功() const noexcept {
    if (合同版本 != 1 || 状态 != 动态概念应用状态::已完成 || !Gread || !概念 || !继续 ||
        继续->合同版本 != 1 || 继续->原请求.valueless_by_exception() || 继续->已定位概念 != 概念->概念 ||
        !继续->首次读取G || 继续->首次读取G > Gread)
        return false;
    const auto live = [&](const 概念树生命周期 &l) {
        return l.创建事实代次 && l.创建事实代次 <= Gread && (!l.退出事实代次 || *l.退出事实代次 > Gread);
    };
    const auto &c = *概念;
    if (!有效(c.概念.值) || !有效(c.本体根.值) || c.根角色 != 概念树根角色::动态 || !live(c.生命周期) ||
        (c.治理状态 != 概念树生命周期状态::活跃 && c.治理状态 != 概念树生命周期状态::冷却))
        return false;
    if (c.是本体根) {
        if (c.概念 != c.本体根 || c.定义 || 有效(c.定义记录))
            return false;
    } else if (!c.定义 || !std::holds_alternative<概念树动态定义>(*c.定义) || !有效(c.规则.值) ||
               !有效(c.定义记录) || !有效(c.生命周期值事实) || c.动态依据组.empty() || c.直接上位组.empty())
        return false;
    if (!c.是本体根) {
        const auto &definition = std::get<概念树动态定义>(*c.定义);
        if (!有效(definition.场景.值) || !有效(definition.主体.值) || definition.时间槽组.size() < 2 ||
            definition.时间槽组.front().相对纳秒 != 0)
            return false;
        for (std::size_t i = 0; i < definition.时间槽组.size(); ++i) {
            const auto &slot = definition.时间槽组[i];
            if (slot.相对纳秒 < 0 || (i && slot.相对纳秒 < definition.时间槽组[i - 1].相对纳秒) ||
                slot.模板组.empty())
                return false;
            for (std::size_t j = 0; j < slot.模板组.size(); ++j)
                if (!有效(slot.模板组[j].值) || (j && slot.模板组[j - 1].值.值 >= slot.模板组[j].值.值))
                    return false;
        }
        for (const auto &source : c.动态依据组)
            if (!有效(source.记录) || !有效(source.成员关系) || !有效(source.引用关系) ||
                !有效(source.截止值事实) || source.依据.目标.valueless_by_exception() || !source.依据.H ||
                source.依据.H >= source.生命周期.创建事实代次 || !live(source.生命周期))
                return false;
    }
    for (std::size_t i = 0; i < 直接边.size(); ++i) {
        const auto &e = 直接边[i];
        if (!有效(e.关系) || !有效(e.上位.值) || !有效(e.下位.值) || e.上位 == e.下位 || !live(e.生命周期))
            return false;
        for (std::size_t j = 0; j < i; ++j)
            if (直接边[j].关系 == e.关系)
                return false;
    }
    if (const auto *r = std::get_if<动态概念形成请求>(&继续->原请求)) {
        if (阶段 != 动态概念阶段::支持已确认 || c.是本体根 ||
            std::get<概念树动态定义>(*c.定义) != 继续->冻结定义 || !支持)
            return false;
        if (r->合同版本 != 1 || !r->创建键 || !r->支持键 || r->创建键 == r->支持键 || !r->H ||
            r->H > 继续->首次读取G || r->场景 != 继续->冻结定义.场景 || r->主体 != 继续->冻结定义.主体 ||
            r->槽见证.size() != 继续->冻结定义.时间槽组.size())
            return false;
        for (std::size_t i = 0; i < r->槽见证.size(); ++i) {
            const auto &witnesses = r->槽见证[i];
            const auto &templates = 继续->冻结定义.时间槽组[i].模板组;
            if (!有效(witnesses.状态) || witnesses.约束.size() != templates.size())
                return false;
            for (const auto &w : witnesses.约束)
                if (!有效(w.原快照特征) || !有效(w.变化目标特征) ||
                    std::count(templates.begin(), templates.end(), w.模板) != 1 ||
                    std::count_if(witnesses.约束.begin(), witnesses.约束.end(),
                                  [&](const auto &x) { return x.模板 == w.模板; }) != 1)
                    return false;
        }
        const auto &s = *支持;
        if (!有效(s.记录) || !有效(s.归属关系) || !有效(s.概念关系) || !有效(s.世界关系) ||
            !有效(s.截止值事实) || s.目标概念 != c.概念 || s.世界引用 != 概念树世界引用{r->动态} ||
            !s.证据H || s.证据H >= s.记录生命周期.创建事实代次 || !live(s.记录生命周期) ||
            s.动态证据 != 概念树动态证据状态::已核验 || 直接边 != c.直接上位组)
            return false;
    } else {
        if (阶段 != 动态概念阶段::接边已确认 || 支持)
            return false;
        const auto &upperRequest = std::get<动态概念上位请求>(继续->原请求);
        const auto &original = 继续->冻结定义;
        if (upperRequest.合同版本 != 1 || !upperRequest.创建键 || !upperRequest.接边键 ||
            upperRequest.创建键 == upperRequest.接边键 ||
            upperRequest.保留槽模板.size() != original.时间槽组.size() || original.时间槽组.size() < 2)
            return false;
        const bool rootMode = std::all_of(upperRequest.保留槽模板.begin(), upperRequest.保留槽模板.end(),
                                          [](const auto &a) { return a.empty(); });
        if (rootMode != c.是本体根)
            return false;
        if (!rootMode) {
            const auto &actual = std::get<概念树动态定义>(*c.定义);
            if (actual.场景 != original.场景 || actual.主体 != original.主体 ||
                actual.时间槽组.size() != original.时间槽组.size())
                return false;
            bool strict = false;
            for (std::size_t i = 0; i < actual.时间槽组.size(); ++i) {
                const auto &wanted = upperRequest.保留槽模板[i];
                const auto &base = original.时间槽组[i];
                const auto &slot = actual.时间槽组[i];
                if (wanted.empty() || wanted.size() > base.模板组.size() ||
                    wanted.size() != slot.模板组.size() || slot.相对纳秒 != base.相对纳秒)
                    return false;
                strict = strict || wanted.size() < base.模板组.size();
                for (auto id : wanted)
                    if (std::count(wanted.begin(), wanted.end(), id) != 1 ||
                        std::count(base.模板组.begin(), base.模板组.end(), id) != 1 ||
                        std::count(slot.模板组.begin(), slot.模板组.end(), id) != 1)
                        return false;
            }
            if (!strict)
                return false;
        }

        if (std::count_if(直接边.begin(), 直接边.end(), [&](const auto &e) {
                return e.上位 == c.概念 && e.下位 == upperRequest.下位;
            }) != 1)
            return false;
        if (std::any_of(直接边.begin(), 直接边.end(),
                        [&](const auto &e) { return e.下位 != upperRequest.下位; }))
            return false;
    }
    return true;
}
} // namespace 海中鱼巣
