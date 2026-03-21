module;
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <functional>
#include <optional>
#include <variant>
#include <stdexcept>
#include <limits>
#include <algorithm>

export module 特征模块;

// 说明：
// - 本模块只负责：特征的写入(动态观测)、稳态原型、特征集合比较(存在比较的输入)
// - 特征值不暴露：I64 内联在 特征快照值；VecU 用 VecU句柄 引用特征值链中的 VecIU64（由特征值模块去重共享）。
// - 差异度统一为 I64：
//     0 表示“完全一致/命中”，越大越不同。
// - 连续测量误差阈值（仅 I64 特征）：用于消除设备抖动
//     d' = max(0, |Δ| - eps)


import 基础数据类型模块;
import 主信息定义模块;
import 通用函数模块;

using namespace 通用函数模块;
// ============================================================
// 1) 比较输出：特征明细 / 存在聚合
// ============================================================

export struct 特征比较明细
{
    const 词性节点类* 类型 = nullptr;
    I64 差异度 = 0;     // 0 表示完全一致；越大越不同
    I64 权重 = 10000;   // Q10000，默认 1.0
    bool 缺失A = false;
    bool 缺失B = false;
};

export struct 存在比较结果
{
    I64 总差异度 = 0;        // 聚合差异（加权和或均值，见 compare 参数）
    I64 归一化差异度 = 0;    // 建议理解为“加权平均差异度”
    int 命中特征数 = 0;      // diff==0 的条目数（且 A/B 都不缺失）
    std::vector<特征比较明细> 明细{};
};

// ============================================================
// 2) 写入与稳态参数
// ============================================================

export struct 稳态策略
{
    // 连续 N 帧通过候选门才允许参与稳态判定
    int 连续N = 5;

    // diff <= 阈值_判稳 视为“无变化”
    I64 阈值_判稳 = 0;

    // diff <= 阈值_命中原型 视为命中某个稳态原型
    I64 阈值_命中原型 = 0;

    // 点原型上限（<=0 不限制）
    int Kmax_points = 32;

    // 淘汰超时（微秒），0 不淘汰
    时间戳 淘汰_超时 = 0;
};

export struct 写入参数
{
    时间戳 ts = 0;
    bool 仅更新当前值 = false;

    // 动态观察开关
    bool 允许创建稳态 = true;
    bool 允许淘汰 = true;

    bool 覆盖默认值 = false;        // true：本次实测强制覆盖 默认I64
    bool 若默认为空则写默认 = true;  // true：默认I64为空时，首帧实测写入默认

    稳态策略 稳态{};
};

export struct 特征写入结果
{
    特征节点类* 特征 = nullptr;
    bool 通过候选门 = false;
    bool 稳态发生变化 = false;
};

// ============================================================
// 3) 特征集合比较参数
// ============================================================

export struct 集合比较参数
{
    bool 输出明细 = true;

    // 缺失惩罚（I64）
    I64 缺失惩罚 = 10000;

    // 是否输出“归一化差异度”（加权平均）
    bool 归一化 = true;

    // 若权重全为 10000，则归一化差异度 = sum(diff)/count
    // 若权重不一，则归一化差异度 = sum(diff*weight)/sum(weight)
};

// ============================================================
// 4) 特征类
// ============================================================

export class 特征类
{
public:
    using 差异度函数 = std::function<I64(const 词性节点类* type, const 特征快照值& a, const 特征快照值& b)>;

    特征类() = default;
    explicit 特征类(差异度函数 fn) : diff_(std::move(fn)) {}

    // 兼容世界树：允许在构造后注入差异度函数
    inline void 设置差异度函数(差异度函数 fn) { diff_ = std::move(fn); }

    // ========================================================
    // CRUD 辅助：主信息
    // ========================================================
    static inline 特征节点主信息类* 取特征主信息(特征节点类* n) noexcept {
        return n ? dynamic_cast<特征节点主信息类*>(n->主信息) : nullptr;
    }
    static inline const 特征节点主信息类* 取特征主信息(const 特征节点类* n) noexcept {
        return n ? dynamic_cast<const 特征节点主信息类*>(n->主信息) : nullptr;
    }

    // ========================================================
    // 4.1) 写入（动态观测：环形缓冲 + 稳态原型）
    // ========================================================
    特征写入结果 写入特征值(特征节点类* 特征节点, const 特征快照值& 新值, const 写入参数& p = {});

    // ========================================================
    // 4.2) 存在比较（特征集合聚合）
    // ========================================================
    using 特征集合 = std::vector<特征节点类*>;
    存在比较结果 比较特征集合(const 特征集合& A, const 特征集合& B, const 集合比较参数& p = {}) const;

    // ========================================================
    // 4.3) 度量签名（ChainKey）
    // ========================================================
    std::string 生成度量签名(特征节点类* 特征节点) const;

private:
    // ========================================================
    // 连续测量差异（仅 I64 特征应用误差阈值）
    // ========================================================
 // ===== [REPLACE] 连续测量差异：仅 I64 使用 误差区间 =====
  //  static inline I64 私有_夹断_I128到I64(__int128 x) noexcept {
  //      if (x > (__int128)std::numeric_limits<I64>::max()) return (std::numeric_limits<I64>::max)();
  //      if (x < (__int128)std::numeric_limits<I64>::min()) return (std::numeric_limits<I64>::min)();
  //      return (I64)x;
  //  }

    // 计算“点 x 到闭区间 [lo,hi]”的距离（>=0）
    static inline I64 私有_点到区间距离(I64 x, const I64区间& r) noexcept {
        if (!r.有效()) return 0;
        if (x < r.低值) return r.低值 - x;
        if (x > r.高值) return x - r.高值;
        return 0;
    }

    // 连续测量差异：a=当前/新值，b=历史/参照
    inline I64 连续测量差异(const 特征节点主信息类& fmi, const 词性节点类* type, const 特征快照值& a, const 特征快照值& b, I64 缺省不同 = 10000) const
    {
        // 1) 先得到 raw diff（仍然允许用 diff_，但 I64 我们可自行算 Δ）
        I64 d = 缺省不同;
        if (diff_) d = diff_(type, a, b);
        else {
            if (a.index() == b.index()) {
                if (auto* pa = std::get_if<I64>(&a)) d = (*pa == std::get<I64>(b)) ? 0 : 缺省不同;
                else if (auto* ha = std::get_if<VecU句柄>(&a)) d = (ha->主信息指针 == std::get<VecU句柄>(b).主信息指针) ? 0 : 缺省不同;
                else if (auto* hp = std::get_if<指针句柄>(&a)) d = (hp->指针 == std::get<指针句柄>(b).指针) ? 0 : 缺省不同;
                else d = 0;
            }
            else d = 缺省不同;
        }

        // 2) 仅对 I64：用“误差区间”把小幅变化视为 0
        const auto* ia = std::get_if<I64>(&a);
        const auto* ib = std::get_if<I64>(&b);
        if (ia && ib && fmi.连续测量误差区间.有效()) {
            // Δ = a - b （连续测量变化量，有符号）           
            I64 delta = 饱和减(*ia, *ib);

            // 若 Δ 落在误差区间内，则 d=0；否则 d=到区间的距离（>=0）
            const I64 dist = 私有_点到区间距离(delta, fmi.连续测量误差区间);
            d = dist; // 这里让“差异度”直接等于超出误差区间的幅度
        }

        return d;
    }
    inline bool 通过候选门_连续N(const 特征节点主信息类& fmi, const 词性节点类* type, const 特征快照值& v, const 稳态策略& pol) const
    {
        const int N = std::max(2, pol.连续N);
        if ((int)fmi.近帧候选.size < (N - 1)) return false;

        // v 与最近 N-1 条历史都足够近
        for (int i = 0; i < N - 1; ++i) {
            const auto* pv = fmi.近帧候选.get_from_latest((std::size_t)i);
            if (!pv) return false;
            const I64 d = 连续测量差异(fmi, type, v, *pv);
            if (d > pol.阈值_判稳) return false;
        }
        return true;
    }

    inline int 匹配稳态点原型(const 特征节点主信息类& fmi, const 词性节点类* type, const 特征快照值& v, const 稳态策略& pol) const
    {
        int best = -1;
        I64 bestD = (std::numeric_limits<I64>::max)();

        for (int i = 0; i < (int)fmi.稳态表.size(); ++i) {
            const auto& proto = fmi.稳态表[(std::size_t)i];
            if (!std::holds_alternative<稳态点原型>(proto)) continue;
            const auto& pp = std::get<稳态点原型>(proto);
            if (!pp.有效()) continue;

            const I64 d = 连续测量差异(fmi, type, v, pp.点);
            if (d <= pol.阈值_命中原型 && d < bestD) { bestD = d; best = i; }
        }
        return best;
    }

    inline void 修剪点原型容量(特征节点主信息类& fmi, const 稳态策略& pol)
    {
        if (pol.Kmax_points <= 0) return;

        auto count_points = [&]() {
            int n = 0;
            for (auto& proto : fmi.稳态表) if (std::holds_alternative<稳态点原型>(proto)) ++n;
            return n;
            };

        while (count_points() > pol.Kmax_points) {
            int evict = -1;
            std::uint64_t bestHit = (std::numeric_limits<std::uint64_t>::max)();
            时间戳 bestLast = (std::numeric_limits<时间戳>::max)();

            for (int i = 0; i < (int)fmi.稳态表.size(); ++i) {
                if (!std::holds_alternative<稳态点原型>(fmi.稳态表[(std::size_t)i])) continue;
                if (i == fmi.当前稳态索引) continue;

                const auto& p = std::get<稳态点原型>(fmi.稳态表[(std::size_t)i]);
                if (!p.有效()) { evict = i; break; }

                if (p.命中次数 < bestHit || (p.命中次数 == bestHit && p.最后命中 < bestLast)) {
                    bestHit = p.命中次数;
                    bestLast = p.最后命中;
                    evict = i;
                }
            }

            if (evict < 0) break;
            fmi.稳态表.erase(fmi.稳态表.begin() + evict);
            if (fmi.当前稳态索引 == evict) fmi.当前稳态索引 = -1;
            else if (fmi.当前稳态索引 > evict) fmi.当前稳态索引 -= 1;
        }
    }

    inline void 淘汰超时(特征节点主信息类& fmi, 时间戳 now, const 稳态策略& pol)
    {
        if (pol.淘汰_超时 == 0) return;
        auto expired = [&](时间戳 last) {
            return (now > last) && ((now - last) > pol.淘汰_超时);
            };

        for (int i = (int)fmi.稳态表.size() - 1; i >= 0; --i) {
            bool drop = false;
            auto& proto = fmi.稳态表[(std::size_t)i];
            if (std::holds_alternative<稳态点原型>(proto)) {
                const auto& p = std::get<稳态点原型>(proto);
                drop = !p.有效() || expired(p.最后命中);
            }
            else {
                const auto& iv = std::get<稳态多维区间原型>(proto);
                drop = !iv.有效() || expired(iv.最后命中);
            }
            if (!drop) continue;

            fmi.稳态表.erase(fmi.稳态表.begin() + i);
            if (fmi.当前稳态索引 == i) fmi.当前稳态索引 = -1;
            else if (fmi.当前稳态索引 > i) fmi.当前稳态索引 -= 1;
        }

        if (fmi.当前稳态索引 < 0 || fmi.当前稳态索引 >= (int)fmi.稳态表.size()) fmi.当前稳态索引 = -1;
    }

private:
    差异度函数 diff_{};
};

// ============================================================
// 5) 度量链键生成（特征式）
//    目标：稳定、短、不爆炸。
// ============================================================

namespace 特征模块_detail {
    inline std::string 主键或空(const 词性节点类* x) {
        return x ? x->获取主键() : std::string{};
    }

    // 仅生成“度量定义”，不携带阈值/实例
    inline std::string 生成_特征式度量链键(
        const 词性节点类* 类型,
        const 词性节点类* 名称,
        std::int64_t 维度索引,
        std::int64_t 维度数,
        std::string_view 值模式,
        std::string_view 参照签名,
        std::string_view 目标域,
        std::string_view 单位)
    {
        // 必含：模式 + 类型
        std::string out;
        out.reserve(64);

        out.append("特征|");
        out.append(值模式);

        const std::string t = 主键或空(类型);
        if (!t.empty()) {
            out.append("|T=");
            out.append(t);
        }

        // 名称（可选）：如不想拆分同类型不同名，直接注释掉这段
        const std::string n = 主键或空(名称);
        if (!n.empty()) {
            out.append("|N=");
            out.append(n);
        }

        // 维度信息：仅多维时写入
        if (维度数 > 1) {
            out.append("|D=");
            out.append(std::to_string(维度索引));
            out.push_back('/');
            out.append(std::to_string(维度数));
        }

        // 常见默认值省略：参照=无 域=特征 单位=i64
        if (!参照签名.empty() && 参照签名 != "无") {
            out.append("|Ref=");
            out.append(参照签名);
        }
        if (!目标域.empty() && 目标域 != "特征") {
            out.append("|Domain=");
            out.append(目标域);
        }
        if (!单位.empty() && 单位 != "i64") {
            out.append("|U=");
            out.append(单位);
        }

        return out;
    }
} // namespace 特征模块_detail

// ============================================================
// 6) 实现：写入 / 比较 / 链键
// ============================================================

inline 特征写入结果 特征类::写入特征值(特征节点类* 特征节点, const 特征快照值& 新值, const 写入参数& p)
{
    特征写入结果 out{};
    out.特征 = 特征节点;

    if (!特征节点 || !特征快照有值(新值)) return out;

    auto* fmi = 取特征主信息(特征节点);
    if (!fmi) return out;

    const 时间戳 now = (p.ts != 0) ? p.ts : 结构体_时间戳::当前_微秒();

    // 1) 更新当前快照 + 环形缓冲
    fmi->当前快照 = 新值;
    fmi->命中次数 += 1;
    fmi->近帧候选.push(新值);
    fmi->近帧时间.push(now);

    if (p.仅更新当前值) return out;

    // 2) 候选门（连续稳定）
    out.通过候选门 = 通过候选门_连续N(*fmi, fmi->类型, 新值, p.稳态);
    if (!out.通过候选门) return out;

    // 3) 稳态命中/创建
    const int oldIdx = fmi->当前稳态索引;
    const int hitIdx = 匹配稳态点原型(*fmi, fmi->类型, 新值, p.稳态);

    if (hitIdx >= 0) {
        auto& proto = fmi->稳态表[(std::size_t)hitIdx];
        auto& pp = std::get<稳态点原型>(proto);
        pp.命中次数 += 1;
        pp.最后命中 = now;

        fmi->当前稳态索引 = hitIdx;
        out.稳态发生变化 = (oldIdx != hitIdx);
    }
    else if (p.允许创建稳态) {
        稳态点原型 pp{};
        pp.点 = 新值;
        pp.命中次数 = 1;
        pp.最后命中 = now;

        fmi->稳态表.push_back(pp);
        fmi->当前稳态索引 = (int)fmi->稳态表.size() - 1;
        out.稳态发生变化 = true;
    }

    // 4) 点容量控制/淘汰
    修剪点原型容量(*fmi, p.稳态);
    if (p.允许淘汰) 淘汰超时(*fmi, now, p.稳态);

    return out;
}

inline 存在比较结果 特征类::比较特征集合(const 特征集合& A, const 特征集合& B, const 集合比较参数& p) const
{
    存在比较结果 out{};
    if (!p.输出明细) out.明细.clear();

    const I64 最大I64 = (std::numeric_limits<I64>::max)();
    const I64 最小I64 = (std::numeric_limits<I64>::min)();

    std::unordered_map<const 词性节点类*, 特征节点类*> ma;
    std::unordered_map<const 词性节点类*, 特征节点类*> mb;

    auto build = [](const 特征集合& in, std::unordered_map<const 词性节点类*, 特征节点类*>& m) {
        m.clear();
        for (auto* n : in) {
            if (!n || !n->主信息) continue;
            auto* mi = dynamic_cast<特征节点主信息类*>(n->主信息);
            if (!mi || !mi->类型) continue;
            if (m.find(mi->类型) == m.end()) {
                m.emplace(mi->类型, n); // 同类型取第一个
            }
        }
        };

    I64 sumW = 0;
    I64 sumDW = 0;
    int hit = 0;

    // 非负权重饱和乘法：用于 差异度 * 权重
    auto 饱和乘_非负权重 = [&](I64 值, I64 非负权重) -> I64 {
        if (值 == 0 || 非负权重 <= 0) return 0;

        if (值 > 0) {
            if (值 > 最大I64 / 非负权重) return 最大I64;
        }
        else { // 值 < 0
            if (值 < 最小I64 / 非负权重) return 最小I64;
        }

        return 值 * 非负权重;
        };

    // 统一聚合当前项
    auto 聚合当前项 = [&](const 特征比较明细& d) {
        sumW = 通用函数模块::饱和加(sumW, d.权重);
        sumDW = 通用函数模块::饱和加(sumDW, 饱和乘_非负权重(d.差异度, d.权重));
        if (p.输出明细) out.明细.push_back(d);
        };

    build(A, ma);
    build(B, mb);

    std::unordered_map<const 词性节点类*, int> all;
    for (auto& kv : ma) all[kv.first] = 1;
    for (auto& kv : mb) all[kv.first] = 1;

    for (auto& kv : all) {
        const 词性节点类* type = kv.first;
        auto ita = ma.find(type);
        auto itb = mb.find(type);

        特征比较明细 d{};
        d.类型 = type;
        d.权重 = 10000;

        if (ita == ma.end()) {
            d.缺失A = true;
            d.差异度 = p.缺失惩罚;
            聚合当前项(d);
            continue;
        }

        if (itb == mb.end()) {
            d.缺失B = true;
            d.差异度 = p.缺失惩罚;
            聚合当前项(d);
            continue;
        }

        const auto* a = 取特征主信息(ita->second);
        const auto* b = 取特征主信息(itb->second);

        if (!a || !b || !特征快照有值(a->当前快照) || !特征快照有值(b->当前快照)) {
            d.差异度 = p.缺失惩罚;
            聚合当前项(d);
            continue;
        }

        // 正常差异度（存在比较不使用“连续测量误差阈值”，它只对同一特征的连续观测生效）
        if (diff_) {
            d.差异度 = diff_(type, a->当前快照, b->当前快照);
        }
        else {
            // 退化：严格相等 -> 0，否则缺失惩罚
            if (a->当前快照.index() == b->当前快照.index()) {
                if (auto* pa = std::get_if<I64>(&a->当前快照)) {
                    d.差异度 = (*pa == std::get<I64>(b->当前快照)) ? 0 : p.缺失惩罚;
                }
                else if (auto* ha = std::get_if<VecU句柄>(&a->当前快照)) {
                    d.差异度 = (ha->主信息指针 == std::get<VecU句柄>(b->当前快照).主信息指针) ? 0 : p.缺失惩罚;
                }
                else if (auto* hp = std::get_if<指针句柄>(&a->当前快照)) {
                    d.差异度 = (hp->指针 == std::get<指针句柄>(b->当前快照).指针) ? 0 : p.缺失惩罚;
                }
                else {
                    d.差异度 = 0;
                }
            }
            else {
                d.差异度 = p.缺失惩罚;
            }
        }

        if (d.差异度 == 0) ++hit;

        聚合当前项(d);
    }

    out.命中特征数 = hit;

    // 总差异度：用“加权和/10000”作为总量
    if (sumW > 0) {
        out.总差异度 = sumDW / 10000;
    }
    else {
        out.总差异度 = 0;
    }

    if (!p.归一化) {
        out.归一化差异度 = out.总差异度;
        return out;
    }

    // 归一化差异度：加权平均 diff（sum(diff*weight)/sum(weight)）
    if (sumW <= 0) {
        out.归一化差异度 = out.总差异度;
    }
    else {
        out.归一化差异度 = sumDW / sumW;
    }

    return out;
}

inline std::string 特征类::生成度量签名(特征节点类* 特征节点) const
{
    const auto* fmi = 取特征主信息(特征节点);
    const 词性节点类* 类型 = fmi ? fmi->类型 : nullptr;
    const 词性节点类* 名称 = fmi ? fmi->名称 : nullptr;

    return 特征模块_detail::生成_特征式度量链键(
        类型,
        名称,
        /*维度索引*/ 0,
        /*维度数*/ 1,
        /*值模式*/ "Value",
        /*参照签名*/ "无",
        /*目标域*/ "特征",
        /*单位*/ "i64");
}

