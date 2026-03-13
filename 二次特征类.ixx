export module 二次特征模块;

// ============================================================================
// 二次特征模块（与最新 二次特征主信息类 对齐）
//
// 关键约束：
// 1) 节点主键必须由系统自动生成的唯一键，绝不承载业务语义。
// 2) “度量链”的业务定位使用 二次特征主信息类::度量签名_链键（业务二级键）。
// 3) 二次特征概念树只存 schema（概念刻度）：链根 + 三大区间 + 可选小区间。
//    结构约束（重要）：三大域与小域处于“同一链”（同层子节点），不做“大域->小域”父子层级。
//    命中规则：在链根同层所有域节点中，取“覆盖差值且区间最窄”的那个；
//            若宽度相同，优先小区间，其次大区间。
// ============================================================================

import <cstdint>;
import <string>;
import <vector>;
import <limits>;
import <algorithm>;
import 语素环境模块;
import 日志模块;
import 状态模块;
import <optional>;

import 主信息定义模块;
import 动态模块;
import 因果信息模块;
import 数据仓库模块;
import 基础数据类型模块;
import 通用函数模块;
import 度量模板注册表模块;

using namespace 通用函数模块;
namespace 二次特征模块_detail {


    // ============================================================
    // ChainKey（度量签名_链键）生成（可读、可解析；用于业务二级键）
    // 注意：节点主键仍由系统生成，链键不进入主键。
    // ============================================================
//    static std::string 生成度量签名_链键(
//        const std::string& 度量模板ID,
//        const std::string& 值模式,      // "Value" / "Delta" / "Trend"
//        const std::string& 参照签名,    // "主体" / "相机" / "地面" / "存在:<key>" ...（尽量枚举化）
//        const std::string& 目标域,      // "存在" / "特征" / "状态" / ...
//        const std::string& 单位)        // "mm" / "px" / "ratio" / "bool" ...
//    {
//        std::string s;
//        s.reserve(128);
//        s += 度量模板ID;
//        s += "|";
//        s += 值模式;
//        s += "|R=";
//        s += 参照签名;
//        s += "|D=";
//        s += 目标域;
//        s += "|U=";
//        s += 单位;
//        return s;
//    }

    inline bool 区间包含(const I64区间& r, I64 v) noexcept {
        return v >= r.低值 && v <= r.高值;
    }

    inline I64 区间宽度(const I64区间& r) noexcept {
        return 安全减(r.高值, r.低值);
    }

    template<class Pred>
    inline 二次特征节点类* 同层查找(二次特征节点类* first, Pred pred) {
        if (!first) return nullptr;
        auto* it = first;
        do {
            if (pred(it)) return it;
            it = static_cast<二次特征节点类*>(it->下);
        } while (it && it != first);
        return nullptr;
    }

    inline std::vector<二次特征节点类*> 枚举子节点(二次特征节点类* parent) {
        std::vector<二次特征节点类*> out;
        if (!parent || !parent->子) return out;
        auto* first = static_cast<二次特征节点类*>(parent->子);
        auto* it = first;
        do {
            out.push_back(it);
            it = static_cast<二次特征节点类*>(it->下);
        } while (it && it != first);
        return out;
    }

    inline 二次特征主信息类* MI(二次特征节点类* n) noexcept {
        return n ? dynamic_cast<二次特征主信息类*>(n->主信息) : nullptr;
    }

    inline const 二次特征主信息类* MI(const 二次特征节点类* n) noexcept {
        return n ? dynamic_cast<const 二次特征主信息类*>(n->主信息) : nullptr;
    }

    // 概念树根的业务标识（不是节点主键）。
    inline const std::string& 概念树根标识() {
        static const std::string k = "__二次特征概念树根__";
        return k;
    }

    // 默认刻度（大区间）是“概念刻度模板”，与节点主键无关。
    // 说明：
    // - Delta/变化类：负/零/正 三段（[-inf,-1],[0,0],[1,+inf]）
    // - 相似度类：低/中/高 三段（[0,3333],[3334,6666],[6667,10000]）
    // - 差异/距离类：低/中/高 三段（[0,3333],[3334,6666],[6667,+inf]）
    // - bool/包含类：否/是/其他 三段（[0,0],[1,1],[2,+inf]）
    // 更细刻度统一用“小区间”注册。

    enum class 枚举_默认刻度模式 : std::uint8_t {
        未定义 = 0,
        符号三段 = 1,
        相似度三段 = 2,
        差异度三段 = 3,
        布尔三段 = 4,
    };

    inline bool 包含子串(const std::string& s, const char* sub) {
        return sub && *sub && (s.find(sub) != std::string::npos);
    }

    inline 枚举_默认刻度模式 推断默认刻度模式(const std::string& 链键) {
        if (链键.empty()) return 枚举_默认刻度模式::差异度三段;

        // bool / 包含 / 缺失码
        if (包含子串(链键, "Contain") || 包含子串(链键, "包含") || 包含子串(链键, "bool") || 包含子串(链键, "缺失")) {
            return 枚举_默认刻度模式::布尔三段;
        }
        // 相似度
        if (包含子串(链键, "Sim") || 包含子串(链键, "相似")) {
            return 枚举_默认刻度模式::相似度三段;
        }
        // Delta/变化
        if (包含子串(链键, "Delta") || 包含子串(链键, "变化")) {
            return 枚举_默认刻度模式::符号三段;
        }
        // 差异/距离/误差/代价
        if (包含子串(链键, "Diff") || 包含子串(链键, "差异") || 包含子串(链键, "距离") || 包含子串(链键, "误差") || 包含子串(链键, "代价")) {
            return 枚举_默认刻度模式::差异度三段;
        }
        // 默认：按差异度处理（更符合“纯结果”）
        return 枚举_默认刻度模式::差异度三段;
    }

    inline std::vector<I64区间> 默认大区间集合(枚举_默认刻度模式 mode) {
        switch (mode) {
        case 枚举_默认刻度模式::符号三段:
            return { { I64_MIN, -1 }, { 0, 0 }, { 1, I64_MAX } };
        case 枚举_默认刻度模式::相似度三段:
            return { { 0, 3333 }, { 3334, 6666 }, { 6667, 10000 } };
        case 枚举_默认刻度模式::布尔三段:
            return { { 0, 0 }, { 1, 1 }, { 2, I64_MAX } };
        case 枚举_默认刻度模式::差异度三段:
        default:
            return { { 0, 3333 }, { 3334, 6666 }, { 6667, I64_MAX } };
        }
    }

    inline 二次特征主信息类* 新建_schema主信息(
        const std::string& 链键,
        const I64区间& r,
        二次特征主信息类::枚举_区间层级 层级,
        const 词性节点类* 概念名,
        枚举_二次特征种类 种类)
    {
        auto* m = new 二次特征主信息类();
        m->形态 = 二次特征主信息类::枚举_二次特征形态::概念模板_schema;
        m->种类 = 种类;
        m->度量签名_链键 = 链键;
        m->标量区间 = r;
        m->区间层级 = 层级;
        m->概念名称 = 概念名;

        // schema：不绑定实例
        m->概念模板 = nullptr;
        m->所属场景 = nullptr;
        m->主体 = nullptr;
        m->客体 = nullptr;
        m->左对象 = nullptr;
        m->右对象 = nullptr;        // schema：不存具体值
        m->标量值 = 0;
        m->是否满足 = false;
        m->置信度 = 0;
        m->观察 = {};
        return m;
    }

    inline bool 概念名匹配(const 二次特征主信息类* m, const 词性节点类* 名) {
        if (!m || !名) return false;
        if (m->概念名称 == 名) return true;
        // 允许“不同指针但主键相同”的情况
        if (m->概念名称 && !m->概念名称->获取主键().empty() && m->概念名称->获取主键() == 名->获取主键()) return true;
        return false;
    }


    inline const 词性节点类* 特征_二次特征标量值_() {
        return 语素集.添加词性词("二次特征_标量值", "名词");
    }

    inline I64 abs_i64_saturate(I64 x) noexcept {
        if (x == I64_MIN) return I64_MAX;
        return x < 0 ? -x : x;
    }

    inline I64 时间差_有符号_饱和(时间戳 左, 时间戳 右) noexcept {
        if (左 >= 右) {
            const auto d = 左 - 右;
            return d > static_cast<时间戳>(I64_MAX) ? I64_MAX : static_cast<I64>(d);
        }
        const auto d = 右 - 左;
        if (d > static_cast<时间戳>(I64_MAX)) return I64_MIN;
        return -static_cast<I64>(d);
    }

    inline 二次特征主信息类::枚举_时间窗 推断时间窗(I64 绝对时间差_微秒) noexcept {
        if (绝对时间差_微秒 <= 1000000) return 二次特征主信息类::枚举_时间窗::短;
        if (绝对时间差_微秒 <= 60000000) return 二次特征主信息类::枚举_时间窗::中;
        return 二次特征主信息类::枚举_时间窗::长;
    }

    inline 特征节点类* 确保二次特征标量特征(二次特征节点类* host) {
        if (!host) return nullptr;
        const auto* 类型 = 特征_二次特征标量值_();
        const std::string key = 类型->获取主键();
        if (host->子) {
            auto* first = static_cast<基础信息节点类*>(host->子);
            auto* it = first;
            do {
                auto* fmi = it ? dynamic_cast<特征节点主信息类*>(it->主信息) : nullptr;
                if (fmi && fmi->类型 && fmi->类型->获取主键() == key) {
                    return static_cast<特征节点类*>(it);
                }
                it = static_cast<基础信息节点类*>(it->下);
            } while (it && it != first);
        }

        auto* n = new 特征节点类{};
        n->父 = host;
        n->子 = nullptr;
        n->根 = nullptr;
        n->链上 = nullptr;
        n->链下 = nullptr;
        n->子节点数量 = 0;

        auto* mi = new 特征节点主信息类();
        mi->类型 = const_cast<词性节点类*>(类型);
        mi->名称 = const_cast<词性节点类*>(类型);
        n->主信息 = mi;

        if (!host->子) {
            host->子 = n;
            n->上 = n;
            n->下 = n;
            return n;
        }

        auto* first = static_cast<基础信息节点类*>(host->子);
        auto* last = static_cast<基础信息节点类*>(first->上 ? first->上 : first);
        n->下 = first;
        n->上 = last;
        first->上 = n;
        last->下 = n;
        return n;
    }

    inline 二次特征节点类* 查找场景二次特征(
        场景节点类* 场景,
        const std::string& 链键,
        基础信息节点类* 左对象,
        基础信息节点类* 右对象) noexcept
    {
        if (!场景 || !场景->主信息) return nullptr;
        auto* smi = dynamic_cast<场景节点主信息类*>(场景->主信息);
        if (!smi) return nullptr;
        for (auto* n : smi->关系列表) {
            auto* mi = (n && n->主信息) ? dynamic_cast<二次特征主信息类*>(n->主信息) : nullptr;
            if (!mi) continue;
            if (mi->度量签名_链键 != 链键) continue;
            if (mi->左对象 != 左对象 || mi->右对象 != 右对象) continue;
            return n;
        }
        return nullptr;
    }} // namespace 二次特征模块_detail


export class 二次特征类 {
public:
    ~二次特征类() = default;
    using I64 = std::int64_t;

    // ============================================================
    // 1) 获取（或创建）二次特征概念树根
    //    - 不依赖节点主键，依赖主信息里的业务标识：度量签名_链键 == "__二次特征概念树根__"
    // ============================================================
    static 二次特征节点类* 获取或创建_二次特征概念树根(
        const std::string& 调用点 = "二次特征类::获取或创建_二次特征概念树根")
    {
        using namespace 数据仓库模块;
        using namespace 二次特征模块_detail;

        锁调度器守卫 锁({
            锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点)
            });

        // 扫描世界链全链：找主信息类型=二次特征_修饰 且 链键为根标识
        if (世界链.根指针) {
            for (auto* it = 世界链.根指针->链下; it && it != 世界链.根指针; it = it->链下) {
                auto* n = dynamic_cast<二次特征节点类*>(it);
                if (!n) continue;
                auto* m = MI(n);
                if (!m) continue;
                if (m->形态 != 二次特征主信息类::枚举_二次特征形态::概念模板_schema) continue;
                if (m->度量签名_链键 == 概念树根标识()) return n;
            }
        }

        // 不存在则创建一个新节点挂到世界链根下
        auto* m = 新建_schema主信息(
            概念树根标识(),
            I64区间{ I64_MIN, I64_MAX },
            二次特征主信息类::枚举_区间层级::未定义,
            nullptr,
            枚举_二次特征种类::未定义);

        auto* n = static_cast<二次特征节点类*>(世界链.添加节点_已加锁(nullptr, static_cast<基础信息基类*>(m)));
        return n;
    }

    // ============================================================
    // 2) 获取或创建“度量链根”
    //    - 链根本身也是 schema 节点，度量签名_链键=ChainKey
    // ============================================================
    static 二次特征节点类* 获取或创建_度量链根(
        const std::string& 链键,
        枚举_二次特征种类 种类 = 枚举_二次特征种类::未定义,
        const std::string& 调用点 = "二次特征类::获取或创建_度量链根")
    {
        using namespace 数据仓库模块;
        using namespace 二次特征模块_detail;
        if (链键.empty()) return nullptr;

        锁调度器守卫 锁({
            锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点)
            });

        auto* 根 = 获取或创建_二次特征概念树根(调用点);
        if (!根) return nullptr;

        // 同层查找：度量签名_链键匹配且层级未定义（链根）
        auto* found = 二次特征模块_detail::同层查找(static_cast<二次特征节点类*>(根->子), [&](二次特征节点类* n) {
            auto* m = MI(n);
            if (!m) return false;
            if (m->形态 != 二次特征主信息类::枚举_二次特征形态::概念模板_schema) return false;
            if (m->度量签名_链键 != 链键) return false;
            if (m->区间层级 != 二次特征主信息类::枚举_区间层级::未定义) return false;
            return true;
            });

        if (found) {
            // 补齐三大区间（若缺）
            确保三大区间_已加锁(found, 种类);
            return found;
        }

        auto* m = 新建_schema主信息(
            链键,
            I64区间{ I64_MIN, I64_MAX },
            二次特征主信息类::枚举_区间层级::未定义,
            nullptr,
            种类);

        auto* 链根 = static_cast<二次特征节点类*>(世界链.添加子节点_已加锁(根, static_cast<基础信息基类*>(m)));
        if (!链根) return nullptr;

        确保三大区间_已加锁(链根, 种类);
        return 链根;
    }

    // ============================================================
    // 3) 注册一个小区间（细分域）
    //    - 结构约束：小区间与三大区间处于“同一链”（同层子节点），不挂在大区间节点下面。
    //    - 兼容：允许传入大区间节点/旧小区间节点，本函数会自动向上回溯到链根再注册。
    // ============================================================
    static 二次特征节点类* 注册小区间(
        二次特征节点类* 任意域节点或链根,
        const I64区间& 区间,
        const 词性节点类* 概念名,
        const std::string& 调用点 = "二次特征类::注册小区间")
    {
        using namespace 数据仓库模块;
        using namespace 二次特征模块_detail;
        if (!任意域节点或链根) return nullptr;

        锁调度器守卫 锁({
            锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点)
            });

        // 向上回溯到“链根”（区间层级==未定义）
        二次特征节点类* 链根 = 任意域节点或链根;
        {
            auto* cur = 链根;
            while (cur) {
                auto* mi = MI(cur);
                if (!mi) break;
                if (mi->形态 != 二次特征主信息类::枚举_二次特征形态::概念模板_schema) break;
                if (mi->区间层级 == 二次特征主信息类::枚举_区间层级::未定义) { 链根 = cur; break; }
                cur = static_cast<二次特征节点类*>(cur->父);
            }
        }

        auto* mRoot = MI(链根);
        if (!mRoot) return nullptr;
        if (mRoot->形态 != 二次特征主信息类::枚举_二次特征形态::概念模板_schema) return nullptr;
        if (mRoot->区间层级 != 二次特征主信息类::枚举_区间层级::未定义) return nullptr;

        // 保障：链根下必须存在三大区间（作为兜底域）
        确保三大区间_已加锁(链根, mRoot->种类);

        // 去重：同区间 + 同概念名（按主键匹配）
        auto* found = 二次特征模块_detail::同层查找(static_cast<二次特征节点类*>(链根->子), [&](二次特征节点类* n) {
            auto* m = MI(n);
            if (!m) return false;
            if (m->区间层级 != 二次特征主信息类::枚举_区间层级::小区间) return false;
            if (m->标量区间.低值 != 区间.低值 || m->标量区间.高值 != 区间.高值) return false;
            if (!概念名) return m->概念名称 == nullptr;
            return 概念名匹配(m, 概念名);
            });
        if (found) return found;

        auto* m = 新建_schema主信息(
            mRoot->度量签名_链键,
            区间,
            二次特征主信息类::枚举_区间层级::小区间,
            概念名,
            mRoot->种类);

        // ✅ 同层挂载：链根 -> 小区间
        return static_cast<二次特征节点类*>(世界链.添加子节点_已加锁(链根, static_cast<基础信息基类*>(m)));
    }

    // ============================================================
    // 4) 按差值命中（优先小区间，否则大区间）
    // ============================================================
    static 二次特征节点类* 按差值命中概念节点(
        二次特征节点类* 链根,
        I64 差值,
        const std::string& 调用点 = "二次特征类::按差值命中概念节点")
    {
        using namespace 数据仓库模块;
        using namespace 二次特征模块_detail;
        if (!链根) return nullptr;

        锁调度器守卫 锁({
            锁请求::读(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点)
            });

        // 同层扫描：三大域 + 小域（兼容：若历史数据仍存在“挂在大域下面的小域”，也一并考虑）
        二次特征节点类* best = nullptr;
        I64 bestW = I64_MAX;
        bool bestIsSmall = false;

        auto consider = [&](二次特征节点类* n) {
            auto* m = MI(n);
            if (!m) return;
            if (m->形态 != 二次特征主信息类::枚举_二次特征形态::概念模板_schema) return;
            if (m->区间层级 != 二次特征主信息类::枚举_区间层级::大区间 &&
                m->区间层级 != 二次特征主信息类::枚举_区间层级::小区间) return;
            if (!区间包含(m->标量区间, 差值)) return;

            const I64 w = 区间宽度(m->标量区间);
            const bool isSmall = (m->区间层级 == 二次特征主信息类::枚举_区间层级::小区间);

            if (!best) {
                best = n; bestW = w; bestIsSmall = isSmall; return;
            }
            if (w < bestW) {
                best = n; bestW = w; bestIsSmall = isSmall; return;
            }
            if (w == bestW) {
                // 同宽度：优先小区间
                if (isSmall && !bestIsSmall) { best = n; bestIsSmall = true; return; }
            }
            };

        auto domains = 枚举子节点(链根);
        for (auto* d : domains) {
            consider(d);

            // 兼容：旧结构“大域->小域”
            auto* md = MI(d);
            if (md && md->区间层级 == 二次特征主信息类::枚举_区间层级::大区间 && d->子) {
                auto subs = 枚举子节点(d);
                for (auto* s : subs) consider(s);
            }
        }

        return best;
    }

    // ============================================================
    // 5) 用概念名称反查（优先小区间，否则大区间）
    // ============================================================
    static 二次特征节点类* 用概念名称反查节点(
        二次特征节点类* 链根,
        const 词性节点类* 概念名,
        const std::string& 调用点 = "二次特征类::用概念名称反查节点")
    {
        using namespace 数据仓库模块;
        using namespace 二次特征模块_detail;
        if (!链根 || !概念名) return nullptr;

        锁调度器守卫 锁({
            锁请求::读(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点)
            });

        // 同层扫：小区间优先，其次大区间（兼容：旧结构大域子节点）
        auto domains = 枚举子节点(链根);

        // 1) 先找小区间（同层）
        for (auto* d : domains) {
            auto* md = MI(d);
            if (!md) continue;

            if (md->区间层级 == 二次特征主信息类::枚举_区间层级::小区间) {
                if (概念名匹配(md, 概念名)) return d;
            }

            // 兼容：旧结构大域->小域
            if (md->区间层级 == 二次特征主信息类::枚举_区间层级::大区间 && d->子) {
                auto subs = 枚举子节点(d);
                for (auto* s : subs) {
                    auto* ms = MI(s);
                    if (!ms || ms->区间层级 != 二次特征主信息类::枚举_区间层级::小区间) continue;
                    if (概念名匹配(ms, 概念名)) return s;
                }
            }
        }

        // 2) 再找大区间（同层）
        for (auto* d : domains) {
            auto* md = MI(d);
            if (!md || md->区间层级 != 二次特征主信息类::枚举_区间层级::大区间) continue;
            if (概念名匹配(md, 概念名)) return d;
        }

        return nullptr;
    }

    // ============================================================
    // 6) 饱和差值（state 计算可复用）
    // ============================================================
    static bool 链键是方向型_(const std::string& 链键) {
        return 链键.find("方向") != std::string::npos
            || 链键.find("先后") != std::string::npos
            || 链键.find("趋势") != std::string::npos
            || 链键.find("Trend") != std::string::npos;
    }

    static void 确保方向概念刻度_(const std::string& 链键, 枚举_二次特征种类 种类, const std::string& 调用点) {
        if (链键.empty() || !链键是方向型_(链键)) return;

        auto* 链根 = 获取或创建_度量链根(链键, 种类, 调用点 + "/方向链根");
        if (!链根) return;

        // 方向概念先不依赖名称：仅按区间（负/零/正）稳定复用。
        (void)注册小区间(链根, I64区间{ I64_MIN, -1 }, nullptr, 调用点 + "/方向负");
        (void)注册小区间(链根, I64区间{ 0, 0 }, nullptr, 调用点 + "/方向零");
        (void)注册小区间(链根, I64区间{ 1, I64_MAX }, nullptr, 调用点 + "/方向正");
    }

    static void 尝试为关系节点命中概念_(二次特征节点类* 关系节点, const std::string& 调用点) {
        auto* mi = 关系节点 && 关系节点->主信息 ? dynamic_cast<二次特征主信息类*>(关系节点->主信息) : nullptr;
        if (!mi) return;

        确保方向概念刻度_(mi->度量签名_链键, mi->种类, 调用点);

        auto* 链根 = 获取或创建_度量链根(mi->度量签名_链键, mi->种类, 调用点 + "/链根");
        if (!链根) return;

        auto* 命中节点 = 按差值命中概念节点(链根, mi->标量值, 调用点 + "/按值命中");
        if (!命中节点) return;

        mi->概念模板 = 命中节点;
    }

    static 动态节点主信息类* 取动态主信息(const 动态节点类* d) noexcept {
        return (d && d->主信息) ? dynamic_cast<动态节点主信息类*>(d->主信息) : nullptr;
    }
    static void 刷新新动态的二次特征(
        场景节点类* 场景,
        动态节点类* 新动态,
        时间戳 now = 0,
        const std::string& 调用点 = "二次特征类::刷新新动态的二次特征")
    {
        using namespace 二次特征模块_detail;
        if (!场景 || !新动态 || !场景->主信息 || !新动态->主信息) return;
        auto* smi = dynamic_cast<场景节点主信息类*>(场景->主信息);
        auto* 新mi = 取动态主信息(新动态);
        if (!smi || !新mi) return;
        smi->清理空指针();
        const std::size_t 关系数量起始 = smi->关系列表.size();
        if (now == 0) now = 新mi->结束时间 ? 新mi->结束时间 : 结构体_时间戳::当前_微秒();

        const I64 新时长 = 时间差_有符号_饱和(新mi->结束时间, 新mi->开始时间);
        const I64 新路径长度 = static_cast<I64>(新mi->状态路径列表.empty() ? 新mi->事件列表.size() : 新mi->状态路径列表.size());

        for (auto* 旧动态 : smi->动态列表) {
            if (!旧动态 || 旧动态 == 新动态 || !旧动态->主信息) continue;
            auto* 旧mi = 取动态主信息(旧动态);
            if (!旧mi) continue;
            if (!新mi->动态主体 || !旧mi->动态主体) continue;
            if (新mi->动态主体 != 旧mi->动态主体) continue;

            const I64 旧时长 = 时间差_有符号_饱和(旧mi->结束时间, 旧mi->开始时间);
            const I64 新旧开始差 = 时间差_有符号_饱和(新mi->开始时间, 旧mi->开始时间);
            const I64 新旧结束差 = 时间差_有符号_饱和(新mi->结束时间, 旧mi->结束时间);
            const I64 路径长度差 = 通用函数模块::安全减(新路径长度, static_cast<I64>(旧mi->状态路径列表.empty() ? 旧mi->事件列表.size() : 旧mi->状态路径列表.size()));
            const auto 时间窗 = 推断时间窗(abs_i64_saturate(新旧结束差));

            auto 记录候选 = [&](const std::string& 链键, I64 标量值) {
                auto* 关系节点 = 查找场景二次特征(场景, 链键, 新动态, 旧动态);
                auto* 关系mi = 关系节点 ? dynamic_cast<二次特征主信息类*>(关系节点->主信息) : nullptr;
                const bool 已有 = (关系mi != nullptr);
                const I64 旧值 = 已有 ? 关系mi->标量值 : 0;
                if (已有 && 旧值 == 标量值) return;

                if (!关系节点) {
                    关系节点 = new 二次特征节点类{};
                    关系节点->父 = nullptr;
                    关系节点->子 = nullptr;
                    关系节点->上 = nullptr;
                    关系节点->下 = nullptr;
                    关系节点->根 = nullptr;
                    关系节点->链上 = nullptr;
                    关系节点->链下 = nullptr;
                    关系节点->子节点数量 = 0;

                    关系mi = new 二次特征主信息类();
                    关系mi->形态 = 二次特征主信息类::枚举_二次特征形态::实例_state;
                    关系mi->种类 = 枚举_二次特征种类::动态比较;
                    关系mi->度量签名_链键 = 链键;
                    关系mi->所属场景 = 场景;
                    关系mi->左对象 = 新动态;
                    关系mi->右对象 = 旧动态;
                    关系mi->主体 = 新动态;
                    关系mi->客体 = 旧动态;
                    关系节点->主信息 = 关系mi;
                    smi->关系列表.push_back(关系节点);
                }

                关系mi->时间窗 = 时间窗;
                关系mi->标量值 = 标量值;

                auto* 标量特征 = 确保二次特征标量特征(关系节点);
                auto* 标量mi = 标量特征 ? dynamic_cast<特征节点主信息类*>(标量特征->主信息) : nullptr;
                if (标量mi) 标量mi->当前快照 = 特征快照值{ 标量值 };

                尝试为关系节点命中概念_(关系节点, 调用点 + "/关系概念");
                状态集.记录内部特征状态(
                    场景,
                    关系节点,
                    标量特征,
                    特征快照值{ 标量值 },
                    已有 ? 枚举_存在状态事件::变化 : 枚举_存在状态事件::创建,
                    !已有 || 旧值 != 标量值,
                    now,
                    {},
                    调用点);
            };

            记录候选("动态|同主体", 新mi->动态主体 == 旧mi->动态主体 ? 1 : 0);
            记录候选("动态|同特征", 新mi->动态特征 == 旧mi->动态特征 ? 1 : 0);
            记录候选("动态|路径签名相同", 新mi->动态路径签名 != 0 && 新mi->动态路径签名 == 旧mi->动态路径签名 ? 1 : 0);
            记录候选("动态|路径长度", 新路径长度);
            记录候选("动态|路径长度差", 路径长度差);
            记录候选("动态|时长_微秒", 新时长);
            记录候选("动态|时长差_微秒", 通用函数模块::安全减(新时长, 旧时长));
            记录候选("动态|开始时间差_微秒", 新旧开始差);
            记录候选("动态|结束时间差_微秒", 新旧结束差);
            记录候选("动态|结束先后", 新旧结束差 > 0 ? 1 : (新旧结束差 < 0 ? -1 : 0));
        }
        const bool 验收关注 = (调用点.find("安全值") != std::string::npos) || (调用点.find("记录安全状态") != std::string::npos);
        if (验收关注) {
            const std::size_t 关系数量结束 = smi->关系列表.size();
            if (关系数量结束 > 关系数量起始) {
                日志::运行f("[验收安全链路] 二次特征已生成(动态): 场景={}, 新增关系={}",
                    static_cast<void*>(场景),
                    (关系数量结束 - 关系数量起始));
            }
        }
    }
    static I64 计算差值(I64 左值, I64 右值) noexcept {
        return 通用函数模块::安全减(左值, 右值);
    }

    static void 状态记录后刷新二次特征(
        场景节点类* 场景,
        状态节点类* 新状态,
        时间戳 now = 0,
        const std::string& 调用点 = "二次特征类::状态记录后刷新二次特征")
    {
        if (!场景 || !新状态) return;
        if (auto* 新动态 = 动态集.状态记录后刷新动态(场景, 新状态, now, 调用点 + "/动态")) {
            const bool 验收关注 = (调用点.find("安全值") != std::string::npos) || (调用点.find("记录安全状态") != std::string::npos);
            if (验收关注) {
                日志::运行f("[验收安全链路] 动态已生成(状态触发): 场景={}, 动态={}, 来源状态={}",
                    static_cast<void*>(场景),
                    static_cast<void*>(新动态),
                    static_cast<void*>(新状态));
            }
            因果集.动态记录后刷新因果(场景, 新动态, now, 调用点 + "/因果");
            刷新新动态的二次特征(场景, 新动态, now, 调用点 + "/动态二次特征");
            return;
        }
        刷新新状态的二次特征(场景, 新状态, now, 调用点 + "/状态兼容");
    }
    static void 刷新新状态的二次特征(
        场景节点类* 场景,
        状态节点类* 新状态,
        时间戳 now = 0,
        const std::string& 调用点 = "二次特征类::刷新新状态的二次特征")
    {
        using namespace 二次特征模块_detail;
        if (!场景 || !新状态 || !新状态->主信息 || !场景->主信息) return;
        auto* 新mi = dynamic_cast<状态节点主信息类*>(新状态->主信息);
        auto* smi = dynamic_cast<场景节点主信息类*>(场景->主信息);
        if (!新mi || !smi || !新mi->状态特征) return;
        smi->清理空指针();
        const std::size_t 关系数量起始 = smi->关系列表.size();
        if (now == 0) now = 新mi->发生时间 ? 新mi->发生时间 : 结构体_时间戳::当前_微秒();

        for (auto* 旧状态 : smi->状态列表) {
            if (!旧状态 || 旧状态 == 新状态 || !旧状态->主信息) continue;
            auto* 旧mi = dynamic_cast<状态节点主信息类*>(旧状态->主信息);
            if (!旧mi) continue;
            if (旧mi->状态特征 != 新mi->状态特征) continue;
            if (旧mi->状态主体 != 新mi->状态主体) continue;

            const auto 发生时间差 = 时间差_有符号_饱和(新mi->发生时间, 旧mi->发生时间);
            const auto 收到时间差 = 时间差_有符号_饱和(新mi->收到时间, 旧mi->收到时间);
            const auto 发生时间差绝对值 = abs_i64_saturate(发生时间差);
            const auto 收到时间差绝对值 = abs_i64_saturate(收到时间差);
            const auto 时间窗 = 推断时间窗(发生时间差绝对值);
            const auto 新事件 = 取状态事件(新状态);
            const auto 旧事件 = 取状态事件(旧状态);

            auto 记录候选 = [&](const std::string& 链键, I64 标量值) {
                auto* 关系节点 = 查找场景二次特征(场景, 链键, 新状态, 旧状态);
                auto* 关系mi = 关系节点 ? dynamic_cast<二次特征主信息类*>(关系节点->主信息) : nullptr;
                const bool 已有 = (关系mi != nullptr);
                const I64 旧值 = 已有 ? 关系mi->标量值 : 0;
                if (已有 && 旧值 == 标量值) return;

                if (!关系节点) {
                    关系节点 = new 二次特征节点类{};
                    关系节点->父 = nullptr;
                    关系节点->子 = nullptr;
                    关系节点->上 = nullptr;
                    关系节点->下 = nullptr;
                    关系节点->根 = nullptr;
                    关系节点->链上 = nullptr;
                    关系节点->链下 = nullptr;
                    关系节点->子节点数量 = 0;

                    关系mi = new 二次特征主信息类();
                    关系mi->形态 = 二次特征主信息类::枚举_二次特征形态::实例_state;
                    关系mi->种类 = 枚举_二次特征种类::状态比较;
                    关系mi->度量签名_链键 = 链键;
                    关系mi->所属场景 = 场景;
                    关系mi->左对象 = 新状态;
                    关系mi->右对象 = 旧状态;
                    关系mi->主体 = 新状态;
                    关系mi->客体 = 旧状态;
                    关系节点->主信息 = 关系mi;
                    smi->关系列表.push_back(关系节点);
                }

                关系mi->时间窗 = 时间窗;
                关系mi->标量值 = 标量值;

                auto* 标量特征 = 确保二次特征标量特征(关系节点);
                auto* 标量mi = 标量特征 ? dynamic_cast<特征节点主信息类*>(标量特征->主信息) : nullptr;
                if (标量mi) 标量mi->当前快照 = 特征快照值{ 标量值 };

                尝试为关系节点命中概念_(关系节点, 调用点 + "/关系概念");
                状态集.记录内部特征状态(
                    场景,
                    关系节点,
                    标量特征,
                    特征快照值{ 标量值 },
                    已有 ? 枚举_存在状态事件::变化 : 枚举_存在状态事件::创建,
                    !已有 || 旧值 != 标量值,
                    now,
                    {},
                    调用点);
            };

            记录候选("状态|同状态域", 新mi->状态域 == 旧mi->状态域 ? 1 : 0);
            记录候选("状态|同主体", 新mi->状态主体 == 旧mi->状态主体 ? 1 : 0);
            记录候选("状态|同特征", 新mi->状态特征 == 旧mi->状态特征 ? 1 : 0);
            记录候选("状态|同事件", (新事件 != 枚举_存在状态事件::未定义 && 新事件 == 旧事件) ? 1 : 0);
            记录候选("状态|值类型一致", 新mi->状态值.index() == 旧mi->状态值.index() ? 1 : 0);
            记录候选("状态|值相等", 新mi->状态值 == 旧mi->状态值 ? 1 : 0);
            记录候选("状态|发生时间差_微秒", 发生时间差);
            记录候选("状态|发生时间差绝对值_微秒", 发生时间差绝对值);
            记录候选("状态|发生先后", 发生时间差 > 0 ? 1 : (发生时间差 < 0 ? -1 : 0));
            记录候选("状态|收到时间差_微秒", 收到时间差);
            记录候选("状态|收到时间差绝对值_微秒", 收到时间差绝对值);
            记录候选("状态|收到先后", 收到时间差 > 0 ? 1 : (收到时间差 < 0 ? -1 : 0));
            记录候选("状态|左事件", static_cast<I64>(新事件));
            记录候选("状态|右事件", static_cast<I64>(旧事件));
            记录候选("状态|左状态域", static_cast<I64>(新mi->状态域));
            记录候选("状态|右状态域", static_cast<I64>(旧mi->状态域));

            if (std::holds_alternative<I64>(新mi->状态值) && std::holds_alternative<I64>(旧mi->状态值)) {
                const I64 新值 = std::get<I64>(新mi->状态值);
                const I64 旧值 = std::get<I64>(旧mi->状态值);
                const I64 变化量 = 安全减(新值, 旧值);
                const I64 变化绝对量 = abs_i64_saturate(变化量);
                记录候选("状态|值可比较", 1);
                记录候选("状态|值变化量", 变化量);
                记录候选("状态|值变化绝对量", 变化绝对量);
                记录候选("状态|值变化方向", 变化量 > 0 ? 1 : (变化量 < 0 ? -1 : 0));
                记录候选("状态|值变化速度_每秒", 发生时间差绝对值 > 0 ? (变化量 * 1000000) / 发生时间差绝对值 : 0);
            }
            else {
                记录候选("状态|值可比较", 新mi->状态值.index() == 旧mi->状态值.index() && 新mi->状态值.index() != 0 ? 1 : 0);
            }
        }
        const bool 验收关注 = (调用点.find("安全值") != std::string::npos) || (调用点.find("记录安全状态") != std::string::npos);
        if (验收关注) {
            const std::size_t 关系数量结束 = smi->关系列表.size();
            if (关系数量结束 > 关系数量起始) {
                日志::运行f("[验收安全链路] 二次特征已生成(状态): 场景={}, 新增关系={}",
                    static_cast<void*>(场景),
                    (关系数量结束 - 关系数量起始));
            }
        }
    }
private:
    static void 确保三大区间_已加锁(二次特征节点类* 链根, 枚举_二次特征种类 种类) {
        using namespace 数据仓库模块;
        using namespace 二次特征模块_detail;
        if (!链根) return;

        auto hasDomain = [&](const I64区间& r) {
            return 同层查找(static_cast<二次特征节点类*>(链根->子), [&](二次特征节点类* n) {
                auto* m = MI(n);
                if (!m) return false;
                if (m->区间层级 != 二次特征主信息类::枚举_区间层级::大区间) return false;
                return m->标量区间.低值 == r.低值 && m->标量区间.高值 == r.高值;
                }) != nullptr;
            };

        auto ensureDomain = [&](const I64区间& r) {
            if (hasDomain(r)) return;
            auto* m = 新建_schema主信息(
                MI(链根) ? MI(链根)->度量签名_链键 : std::string{},
                r,
                二次特征主信息类::枚举_区间层级::大区间,
                nullptr,
                种类);
            世界链.添加子节点_已加锁(链根, static_cast<基础信息基类*>(m));
            };

        const std::string chainKey = (MI(链根) ? MI(链根)->度量签名_链键 : std::string{});
        const auto mode = 推断默认刻度模式(chainKey);
        const auto ranges = 默认大区间集合(mode);
        for (const auto& r : ranges) ensureDomain(r);
    }
};












