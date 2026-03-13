module;
#include <afx.h>

// 需求模块（重写版）
// 目标：
//  1) 只做“需求链”的增删改查 + 扫描去重/合并权重 + 选择下一需求
//  2) 不引入特征值细节；签名只依赖主键（类型/对象/状态）
//  3) 不做兼容：旧的多余接口不再提供

export module 需求模块;

import 数据仓库模块;
import 主信息定义模块;
import 基础数据类型模块;
import 通用函数模块;
import 世界树环境模块;
import 状态模块;
import 二次特征模块;
import 语素环境模块;

import <string>;
import <string_view>;
import <vector>;
import <unordered_map>;
import <optional>;
import <algorithm>;
import <limits>;
import <cstdint>;

using namespace 通用函数模块;
using namespace 数据仓库模块;

namespace 需求模块_detail {
    inline 时间戳 规范化时间_(时间戳 now) noexcept {
        return now != 0 ? now : 结构体_时间戳::当前_微秒();
    }

    inline const 词性节点类* 类型_存在_需求虚拟存在_() {
        return 语素集.添加词性词("存在_需求虚拟存在", "名词");
    }

    inline const 词性节点类* 名称_存在_需求虚拟存在_() {
        return 语素集.添加词性词("需求虚拟存在", "名词");
    }

    inline const 词性节点类* 特征_需求权重_() {
        return 语素集.添加词性词("需求_权重", "名词");
    }

    inline const 词性节点类* 特征_二次特征标量值_() {
        return 语素集.添加词性词("二次特征_标量值", "名词");
    }

    inline bool 快照相等_(const std::optional<特征快照值>& oldValue, const 特征快照值& newValue) noexcept
    {
        return oldValue.has_value() && *oldValue == newValue;
    }

    inline 状态节点类* 记录内部特征状态_(
        场景节点类* 场景,
        基础信息节点类* 主体,
        const 词性节点类* 特征类型,
        const 特征快照值& 值,
        枚举_存在状态事件 事件,
        bool 是否变化,
        时间戳 now,
        const std::string& 调用点)
    {
        if (!场景 || !主体 || !特征类型) return nullptr;
        if (事件 != 枚举_存在状态事件::创建 && !是否变化) return nullptr;

        auto* feat = 世界树.确保特征(主体, 特征类型, 特征类型, 调用点);
        return 状态集.记录内部特征状态(
            场景,
            主体,
            feat,
            值,
            事件,
            是否变化,
            now,
            [](场景节点类* s, 状态节点类* n, 时间戳 ts, const std::string& cp) {
                二次特征类::状态记录后刷新二次特征(s, n, ts, cp);
            },
            调用点);
    }
    inline 存在节点类* 确保需求虚拟存在_(需求主信息类* mi, 时间戳 now, const std::string& 调用点)
    {
        if (!mi) return nullptr;
        if (mi->需求虚拟存在) return mi->需求虚拟存在;
        auto* 内部世界 = 世界树.取内部世界();
        if (!内部世界) return nullptr;

        auto* veMi = new 存在节点主信息类();
        veMi->类型 = const_cast<词性节点类*>(类型_存在_需求虚拟存在_());
        veMi->名称 = const_cast<词性节点类*>(mi->类型 ? mi->类型 : 名称_存在_需求虚拟存在_());
        mi->需求虚拟存在 = 世界树.添加子存在(内部世界, veMi, now, 调用点);
        return mi->需求虚拟存在;
    }

    inline void 写需求内部状态_(需求主信息类* mi, 枚举_存在状态事件 事件, bool 是否变化, 时间戳 now, const std::string& 调用点)
    {
        if (!mi) return;

        auto* ve = 确保需求虚拟存在_(mi, now, 调用点);
        if (!ve) return;

        const I64 v = static_cast<I64>(mi->权重);
        const 特征快照值 新值{ v };
        const auto 旧值 = 世界树.读取特征快照(ve, 特征_需求权重_(), 调用点);
        const bool 值发生变化 = !快照相等_(旧值, 新值);

        世界树.写入特征_I64(ve, 特征_需求权重_(), v, {}, 调用点);

        // 特征状态优先直接存进需求虚拟存在；只有值变化（或创建）时再补一份状态快照。
        if (!mi->需求场景) return;
        (void)记录内部特征状态_(mi->需求场景, ve, 特征_需求权重_(), 新值, 事件, 值发生变化, now, 调用点);
    }
    inline 状态节点主信息类* 取状态主信息_(状态节点类* 状态) noexcept
    {
        return (状态 && 状态->主信息) ? dynamic_cast<状态节点主信息类*>(状态->主信息) : nullptr;
    }

    inline std::optional<特征快照值> 读取需求现状快照_(const 需求主信息类* mi, const 状态节点主信息类* 目标状态, const std::string& 调用点)
    {
        if (!mi || !目标状态) return std::nullopt;

        auto* 主体 = 目标状态->状态主体 ? 目标状态->状态主体 : static_cast<基础信息节点类*>(mi->被需求存在);
        auto* 特征 = 目标状态->状态特征;
        if (!主体 || !特征 || !特征->主信息) return std::nullopt;

        auto* 特征主信息 = dynamic_cast<特征节点主信息类*>(特征->主信息);
        if (!特征主信息 || !特征主信息->类型) return std::nullopt;

        return 世界树.读取特征快照(主体, 特征主信息->类型, 调用点);
    }

}

export class 需求类 {
public:
    using I64 = std::int64_t;

    struct 结构_创建参数 {
        const 自然句节点类* 描述 = nullptr;
        const 词性节点类* 类型 = nullptr;
        场景节点类* 发生场景 = nullptr;
        时间戳 生成时间 = 0;       // 0 表示自动填充
        时间戳 有效截止 = 0;       // 0 表示不设置
        I64 权重 = 0;
    };

    struct 结构_扫描统计 {
        std::size_t 总数 = 0;
        std::size_t 合并数 = 0;
        std::size_t 删除数 = 0;
        std::size_t 补签名数 = 0;
    };


    struct 结构_状态判断结果 {
        需求节点类* 需求 = nullptr;
        状态节点类* 目标状态 = nullptr;
        基础信息节点类* 状态主体 = nullptr;
        特征节点类* 状态特征 = nullptr;
        枚举_状态域 状态域 = 枚举_状态域::未定义;
        特征快照值 现状值{};
        特征快照值 目标值{};
        bool 有现状 = false;
        bool 有目标 = false;
        bool 值可比较 = false;
        bool 已满足 = false;
        I64 目标差值 = 0;
        I64 目标差值绝对量 = 0;
        I64 调整方向 = 0;
    };

    // ============================================================
    // 1) CRUD
    // ============================================================

    // 创建一个需求（顶层节点）。
    //  - 主体/被需求存在/被需求状态 是“对象绑定”；
    //  - p.类型 为空也允许（抽象需求），但会导致签名退化。
    需求节点类* 创建需求(
        存在节点类* 需求主体,
        存在节点类* 被需求存在,
        状态节点类* 被需求状态,
        const 结构_创建参数& p = {},
        const std::string& 调用点 = "需求类::创建需求",
        需求节点类* 父需求 = nullptr)
    {
        using namespace 数据仓库模块;

        auto* mi = new 需求主信息类();
        mi->描述信息 = p.描述;
        mi->类型 = p.类型;
        mi->需求主体 = 需求主体;
        mi->需求场景 = p.发生场景;
        mi->被需求存在 = 被需求存在;
        mi->被需求状态 = 被需求状态;
        mi->权重 = p.权重;
        mi->需求生成时间 = 需求模块_detail::规范化时间_(p.生成时间);
        mi->需求有效截止 = p.有效截止;

        // 生成签名（只依赖主键，不触碰特征值）
        mi->需求签名.clear();
        mi->需求签名.push_back(私有_生成维度签名_已加锁(*mi));

        需求节点类* created = nullptr;
        {
            锁调度器守卫 锁({
                锁请求::写(需求链.链表锁, 枚举_锁域::需求链, "需求链", 调用点)
                });

            if (父需求) created = 需求链.添加子节点_已加锁(父需求, mi);
            else created = 需求链.添加节点_已加锁(nullptr, mi);
        }

        (void)需求模块_detail::确保需求虚拟存在_(mi, mi->需求生成时间, 调用点);
        需求模块_detail::写需求内部状态_(mi, 枚举_存在状态事件::创建, false, mi->需求生成时间, 调用点);
        return created;
    }

    // 删除一个需求节点（会递归删除子链，如果你未来在需求下挂子节点）。
    bool 删除需求(需求节点类* n, const std::string& 调用点 = "需求类::删除需求") {
        using namespace 数据仓库模块;
        if (!n) return false;

        锁调度器守卫 锁({
            锁请求::写(需求链.链表锁, 枚举_锁域::需求链, "需求链", 调用点)
            });

        需求链.删除节点_已加锁(n);
        return true;
    }

    // 更新权重（饱和加法）
    bool 调整权重(需求节点类* n, I64 delta, const std::string& 调用点 = "需求类::调整权重") {
        using namespace 数据仓库模块;
        if (!n || !n->主信息) return false;

        需求主信息类* mi = nullptr;
        bool changed = false;
        const 时间戳 now = 结构体_时间戳::当前_微秒();

        {
            锁调度器守卫 锁({
                锁请求::写(需求链.链表锁, 枚举_锁域::需求链, "需求链", 调用点)
                });

            mi = n->主信息;
            if (!mi) return false;
            const I64 old = mi->权重;
            mi->权重 = 饱和加(mi->权重, delta);
            changed = (old != mi->权重);
        }

        需求模块_detail::写需求内部状态_(mi, 枚举_存在状态事件::变化, changed, now, 调用点);
        return true;
    }

    // 重新生成签名（当你改变了类型/对象/状态绑定时调用）
    bool 刷新签名(需求节点类* n, const std::string& 调用点 = "需求类::刷新签名") {
        using namespace 数据仓库模块;
        if (!n || !n->主信息) return false;

        锁调度器守卫 锁({
            锁请求::写(需求链.链表锁, 枚举_锁域::需求链, "需求链", 调用点)
            });

        auto* mi = n->主信息;
        mi->需求签名.clear();
        mi->需求签名.push_back(私有_生成维度签名_已加锁(*mi));
        return true;
    }

    结构_状态判断结果 判断需求状态(
        需求节点类* n,
        const std::string& 调用点 = "需求类::判断需求状态") const
    {
        结构_状态判断结果 out{};
        out.需求 = n;
        if (!n || !n->主信息) return out;

        auto* mi = n->主信息;
        auto* 目标状态 = 需求模块_detail::取状态主信息_(mi->被需求状态);
        if (!目标状态) return out;

        out.目标状态 = mi->被需求状态;
        out.状态主体 = 目标状态->状态主体 ? 目标状态->状态主体 : static_cast<基础信息节点类*>(mi->被需求存在);
        out.状态特征 = 目标状态->状态特征;
        out.状态域 = 目标状态->状态域;
        out.目标值 = 目标状态->状态值;
        out.有目标 = 特征快照有值(out.目标值);

        auto 现状 = 需求模块_detail::读取需求现状快照_(mi, 目标状态, 调用点);
        if (现状.has_value()) {
            out.现状值 = *现状;
            out.有现状 = 特征快照有值(out.现状值);
        }

        if (!out.有目标 || !out.有现状) return out;

        if (std::holds_alternative<I64>(out.现状值) && std::holds_alternative<I64>(out.目标值)) {
            const I64 现状值 = std::get<I64>(out.现状值);
            const I64 目标值 = std::get<I64>(out.目标值);
            out.值可比较 = true;
            out.目标差值 = 目标值 - 现状值;
            out.目标差值绝对量 = out.目标差值 < 0 ? -out.目标差值 : out.目标差值;
            out.调整方向 = (out.目标差值 > 0) ? 1 : ((out.目标差值 < 0) ? -1 : 0);
            out.已满足 = (out.目标差值 == 0);
            return out;
        }

        if (out.现状值.index() == out.目标值.index() && out.现状值.index() != 0) {
            out.值可比较 = true;
            out.已满足 = (out.现状值 == out.目标值);
        }

        return out;
    }

    bool 需求已满足(
        需求节点类* n,
        const std::string& 调用点 = "需求类::需求已满足") const
    {
        return 判断需求状态(n, 调用点).已满足;
    }

    // 获取所有需求节点（只返回顶层链，不展开子链）
    std::vector<需求节点类*> 获取全部需求(const std::string& 调用点 = "需求类::获取全部需求") const {
        using namespace 数据仓库模块;
        std::vector<需求节点类*> out;

        锁调度器守卫 锁({
            锁请求::读(需求链.链表锁, 枚举_锁域::需求链, "需求链", 调用点)
            });

        if (!需求链.根指针) return out;
        auto* root = 需求链.根指针;
        for (auto* it = root->链下; it && it != root; it = it->链下) {
            out.push_back(static_cast<需求节点类*>(it));
        }
        return out;
    }

    // ============================================================
    // 2) 扫描：补签名 + 去重合并权重
    // ============================================================
    结构_扫描统计 扫描并合并重复需求(const std::string& 调用点 = "需求类::扫描并合并重复需求") {
        using namespace 数据仓库模块;
        结构_扫描统计 st{};

        锁调度器守卫 锁({
            锁请求::写(需求链.链表锁, 枚举_锁域::需求链, "需求链", 调用点)
            });

        if (!需求链.根指针) return st;

        std::unordered_map<std::string, 需求节点类*> map;
        map.reserve(256);

        auto* root = 需求链.根指针;

        for (auto* it = root->链下; it && it != root;) {
            auto* cur = static_cast<需求节点类*>(it);
            it = it->链下; // 先取 next，避免删除后悬空

            if (!cur || !cur->主信息) continue;
            ++st.总数;

            auto* mi = cur->主信息;

            // 补签名
            if (mi->需求签名.empty() || mi->需求签名.front().empty()) {
                mi->需求签名.clear();
                mi->需求签名.push_back(私有_生成维度签名_已加锁(*mi));
                ++st.补签名数;
            }

            const std::string& key = mi->需求签名.front();
            if (key.empty()) {
                // 仍为空：不做去重
                continue;
            }

            auto itMap = map.find(key);
            if (itMap == map.end()) {
                map.emplace(key, cur);
                continue;
            }

            // 重复：合并到 keep
            auto* keep = itMap->second;
            if (!keep || !keep->主信息) {
                itMap->second = cur;
                continue;
            }

            auto* kmi = keep->主信息;

            // 合并权重（饱和加）
            kmi->权重 = 饱和加(kmi->权重, mi->权重);

            // 命中次数累加（用于“重复出现的需求更重要”）
            kmi->命中次数 += (mi->命中次数 + 1);

            // 生成时间：保留更早的（更像“长期未满足”），你也可以改成保留最新
            if (kmi->需求生成时间 == 0 || (mi->需求生成时间 != 0 && mi->需求生成时间 < kmi->需求生成时间)) {
                kmi->需求生成时间 = mi->需求生成时间;
            }

            ++st.合并数;

            // 删除重复节点
            需求链.删除节点_已加锁(cur);
            ++st.删除数;
        }

        return st;
    }

    // ============================================================
    // 3) 选择下一需求
    //    规则（最小稳定版）：
    //      先比 权重（大优先）
    //      权重相同：选更早生成的（更像“欠账久”）
    // ============================================================
    需求节点类* 选择下一需求(const std::string& 调用点 = "需求类::选择下一需求") const {
        using namespace 数据仓库模块;

        锁调度器守卫 锁({
            锁请求::读(需求链.链表锁, 枚举_锁域::需求链, "需求链", 调用点)
            });

        if (!需求链.根指针) return nullptr;

        需求节点类* best = nullptr;
        I64 bestW = std::numeric_limits<I64>::min();
        时间戳 bestT = 0;

        auto* root = 需求链.根指针;
        for (auto* it = root->链下; it && it != root; it = it->链下) {
            auto* n = static_cast<需求节点类*>(it);
            if (!n || !n->主信息) continue;

            const auto* mi = n->主信息;
            const I64 w = mi->权重;
            const 时间戳 t = mi->需求生成时间;

            if (!best) {
                best = n; bestW = w; bestT = t;
                continue;
            }

            if (w > bestW) {
                best = n; bestW = w; bestT = t;
                continue;
            }
            if (w == bestW) {
                // t=0 视为“未知”，排后
                if (bestT == 0 && t != 0) { best = n; bestT = t; continue; }
                if (t != 0 && bestT != 0 && t < bestT) { best = n; bestT = t; continue; }
            }
        }

        return best;
    }

private:
  

    // 维度签名：
    //  - 只使用主键（类型/对象/状态）
    //  - 不触碰特征值，不做深扫描
    //  - 结果用于“重复需求合并/快速索引”
    static std::string 私有_生成维度签名_已加锁(const 需求主信息类& x) {
        std::string s;
        s.reserve(256);

        // 需求类型
        s += "T=";
        s += x.类型 ? x.类型->获取主键() : "NULL";

        // 被需求存在
        s += "|O=";
        s += x.被需求存在 ? x.被需求存在->获取主键() : "NULL";

        // 被需求状态
        s += "|S=";
        s += x.被需求状态 ? x.被需求状态->获取主键() : "NULL";

        // 发生场景（可选，默认不纳入去重；需要按场景去重时可取消注释）
        // s += "|C=";
        // s += 私有_节点主键(x.需求场景);

        return s;
    }

  
};







