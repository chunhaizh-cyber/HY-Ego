export module 概念_特征树模块;

// ============================================================================
// 概念_特征树模块
//
// 本模块现在包含两部分：
// A) 概念_特征树类：二次特征“概念刻度(schema)”树（原有实现，保留）
// B) 概念_聚合特征树类：聚合特征概念（坐标/尺寸/姿态等）注册与读取
//
// 说明：
// - 聚合特征概念：多维连续量一律拆成多个 I64 分量特征存储；概念本体只提供“分量定义”。
// - 概念节点/关系边：统一复用 概念树模块 的存在节点语义。
// ============================================================================

import <cstdint>;
import <string>;
import <string_view>;
import <vector>;
import <limits>;
import <algorithm>;
import <stdexcept>;
import <utility>;

import 数据仓库模块;
import 主信息定义模块;
import 基础数据类型模块;
import 度量模板注册表模块;
import 通用函数模块;
import 语素环境模块;
import 概念树模块;

using namespace 通用函数模块;
using namespace 数据仓库模块;

// ============================================================================
// A) 二次特征：概念刻度(schema)树（原有实现）
// ============================================================================

export class 概念_特征树类 {
public:
    using I64 = std::int64_t;

    static const std::string& 概念树根标识() {
        static const std::string k = "__二次特征概念树根__";
        return k;
    }

    static 二次特征节点类* 获取或创建_概念树根(
        const std::string& 调用点 = "概念_特征树类::获取或创建_概念树根")
    {
        锁调度器守卫 锁({
            锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点)
            });

        if (世界链.根指针) {
            for (auto* it = 世界链.根指针->链下; it && it != 世界链.根指针; it = it->链下) {
                auto* n = static_cast<二次特征节点类*>(it);
                auto* m = MI(n);
                if (!m) continue;
                if (m->形态 != 二次特征主信息类::枚举_二次特征形态::概念模板_schema) continue;
                if (m->度量签名_链键 == 概念树根标识()) return n;
            }
        }

        auto* m = 新建_schema主信息(
            概念树根标识(),
            I64区间{ I64_MIN, I64_MAX },
            二次特征主信息类::枚举_区间层级::未定义,
            nullptr,
            枚举_二次特征种类::未定义);

        return static_cast<二次特征节点类*>(世界链.添加节点_已加锁(nullptr, static_cast<基础信息基类*>(m)));
    }

    static 二次特征节点类* 获取或创建_度量链根(
        const std::string& 链键,
        枚举_二次特征种类 种类 = 枚举_二次特征种类::未定义,
        const std::string& 调用点 = "概念_特征树类::获取或创建_度量链根")
    {
        if (链键.empty()) return nullptr;

        锁调度器守卫 锁({
            锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点)
            });

        auto* 根 = 获取或创建_概念树根(调用点);
        if (!根) return nullptr;

        if (根->子) {
            auto* first = static_cast<二次特征节点类*>(根->子);
            auto* it = first;
            do {
                auto* m = MI(it);
                if (m && m->形态 == 二次特征主信息类::枚举_二次特征形态::概念模板_schema && m->度量签名_链键 == 链键) {
                    私有_确保默认大区间_已加锁(it, 调用点);
                    return it;
                }
                it = static_cast<二次特征节点类*>(it->下);
            } while (it && it != first);
        }

        auto* m = 新建_schema主信息(
            链键,
            I64区间{ I64_MIN, I64_MAX },
            二次特征主信息类::枚举_区间层级::未定义,
            nullptr,
            种类);

        auto* 链根 = static_cast<二次特征节点类*>(世界链.添加子节点_已加锁(根, static_cast<基础信息基类*>(m)));
        私有_确保默认大区间_已加锁(链根, 调用点);
        return 链根;
    }

    static 二次特征节点类* 注册_小区间(
        二次特征节点类* 链根,
        const I64区间& r,
        const 词性节点类* 概念名,
        const std::string& 调用点 = "概念_特征树类::注册_小区间")
    {
        if (!链根) return nullptr;
        if (!r.有效()) return nullptr;

        锁调度器守卫 锁({
            锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点)
            });

        if (链根->子) {
            auto* first = static_cast<二次特征节点类*>(链根->子);
            auto* it = first;
            do {
                auto* m = MI(it);
                if (m && m->形态 == 二次特征主信息类::枚举_二次特征形态::概念模板_schema) {
                    if (m->区间层级 == 二次特征主信息类::枚举_区间层级::小区间 && m->标量区间.低值 == r.低值 && m->标量区间.高值 == r.高值) {
                        if (概念名 == nullptr || 概念名匹配(m, 概念名)) return it;
                    }
                }
                it = static_cast<二次特征节点类*>(it->下);
            } while (it && it != first);
        }

        const auto* 链根MI = MI(链根);
        const std::string 链键 = 链根MI ? 链根MI->度量签名_链键 : std::string();
        auto* m = 新建_schema主信息(
            链键,
            r,
            二次特征主信息类::枚举_区间层级::小区间,
            概念名,
            链根MI ? 链根MI->种类 : 枚举_二次特征种类::未定义);

        return static_cast<二次特征节点类*>(世界链.添加子节点_已加锁(链根, static_cast<基础信息基类*>(m)));
    }

    static 二次特征节点类* 命中概念节点(
        二次特征节点类* 链根,
        I64 标量值,
        const std::string& 调用点 = "概念_特征树类::命中概念节点")
    {
        if (!链根 || !链根->子) return nullptr;

        锁调度器守卫 锁({
            锁请求::读(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点)
            });

        auto* best = (二次特征节点类*)nullptr;
        I64 bestW = I64_MAX;
        auto bestLv = 二次特征主信息类::枚举_区间层级::未定义;

        auto* first = static_cast<二次特征节点类*>(链根->子);
        auto* it = first;
        do {
            auto* m = MI(it);
            if (m && m->形态 == 二次特征主信息类::枚举_二次特征形态::概念模板_schema) {
                if (区间包含(m->标量区间, 标量值)) {
                    const I64 w = 区间宽度(m->标量区间);
                    const auto lv = m->区间层级;

                    bool better = false;
                    if (w < bestW) better = true;
                    else if (w == bestW) {
                        if (bestLv != 二次特征主信息类::枚举_区间层级::小区间 && lv == 二次特征主信息类::枚举_区间层级::小区间) better = true;
                    }

                    if (better) {
                        best = it;
                        bestW = w;
                        bestLv = lv;
                    }
                }
            }
            it = static_cast<二次特征节点类*>(it->下);
        } while (it && it != first);

        return best;
    }

private:
    static 二次特征主信息类* MI(二次特征节点类* n) noexcept {
        return n ? dynamic_cast<二次特征主信息类*>(n->主信息) : nullptr;
    }

    static bool 区间包含(const I64区间& r, I64 v) noexcept {
        return v >= r.低值 && v <= r.高值;
    }

    static I64 区间宽度(const I64区间& r) noexcept {
        return 安全减(r.高值, r.低值);
    }

    static bool 概念名匹配(const 二次特征主信息类* m, const 词性节点类* 名) {
        if (!m || !名) return false;
        if (m->概念名称 == 名) return true;
        if (m->概念名称 && !m->概念名称->获取主键().empty() && m->概念名称->获取主键() == 名->获取主键()) return true;
        return false;
    }

    static 二次特征主信息类* 新建_schema主信息(
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

        m->概念模板 = nullptr;
        m->所属场景 = nullptr;
        m->主体 = nullptr;
        m->客体 = nullptr;
        m->左对象 = nullptr;
        m->右对象 = nullptr;

        m->标量值 = 0;
        m->是否满足 = false;
        m->置信度 = 0;
        m->观察 = {};
        return m;
    }

    static std::string 私有_提取特征类型主键(std::string_view 链键) {
        constexpr std::string_view tag = "|T=";
        auto pos = 链键.find(tag);
        if (pos == std::string_view::npos) return {};
        pos += tag.size();
        if (pos >= 链键.size()) return {};
        return std::string(链键.substr(pos));
    }

    static void 私有_确保默认大区间_已加锁(二次特征节点类* 链根, const std::string&) {
        if (!链根) return;

        const auto* 链根MI = MI(链根);
        const std::string 链键 = 链根MI ? 链根MI->度量签名_链键 : std::string();
        if (链键.empty()) return;

        auto& reg = 度量模板注册表类::实例();
        const std::string typeKey = 私有_提取特征类型主键(链键);

        auto cuts = reg.默认刻度_由链键(链键, typeKey);

        if (cuts.empty()) {
            cuts.push_back({ I64区间{0, 3333}, "低", false });
            cuts.push_back({ I64区间{3334, 6666}, "中", false });
            cuts.push_back({ I64区间{6667, I64_MAX}, "高", false });
        }

        for (const auto& c : cuts) {
            const I64区间 r = c.区间;
            if (!r.有效()) continue;

            bool exists = false;
            if (链根->子) {
                auto* first = static_cast<二次特征节点类*>(链根->子);
                auto* it = first;
                do {
                    auto* m = MI(it);
                    if (m && m->形态 == 二次特征主信息类::枚举_二次特征形态::概念模板_schema) {
                        if (m->区间层级 == 二次特征主信息类::枚举_区间层级::大区间 &&
                            m->标量区间.低值 == r.低值 && m->标量区间.高值 == r.高值) {
                            exists = true;
                            break;
                        }
                    }
                    it = static_cast<二次特征节点类*>(it->下);
                } while (it && it != first);
            }
            if (exists) continue;

            auto* mi = 新建_schema主信息(
                链键,
                r,
                二次特征主信息类::枚举_区间层级::大区间,
                nullptr,
                链根MI ? 链根MI->种类 : 枚举_二次特征种类::未定义);

            世界链.添加子节点_已加锁(链根, static_cast<基础信息基类*>(mi));
        }
    }
};

// ============================================================================
// B) 聚合特征概念：坐标/尺寸/姿态...
// ============================================================================

export class 概念_聚合特征树类 {
public:
    using I64 = std::int64_t;

    // 聚合分量序号（挂在“关系边节点”下的特征，I64）
    static const 词性节点类* 特征类型_分量序号() {
        static const 词性节点类* t = nullptr;
        if (!t) t = 语素集.添加词性词("特征_聚合分量序号", "名词");
        return t;
    }

    // 维度 N（可选，挂在“概念节点”下的特征）
    static const 词性节点类* 特征类型_维度N() {
        static const 词性节点类* t = nullptr;
        if (!t) t = 语素集.添加词性词("特征_聚合维度N", "名词");
        return t;
    }

    // ============================================================
    // 1) 注册一个聚合特征概念
    //    - 概念节点位于 概念域_特征
    //    - 用 relation(part_of) 边表示分量
    //    - 每条边下写 特征_聚合分量序号 = i
    // ============================================================
    static 存在节点类* 注册_聚合特征概念(
        const 词性节点类* 概念名,
        const std::vector<const 词性节点类*>& 分量特征类型,
        const std::string& 调用点 = "概念_聚合特征树类::注册_聚合特征概念")
    {
        if (!概念名) return nullptr;
        if (分量特征类型.empty()) return nullptr;

        // 概念节点（同域：特征）
        auto* 新概念节点 = 概念树类::获取或创建_概念(枚举_概念域::特征, 概念名,结构体_时间戳::当前_微秒(),调用点);
        if (!新概念节点) return nullptr;

        // 注册分量边 + 序号
        for (std::size_t i = 0; i < 分量特征类型.size(); ++i) {
            auto* ct = 分量特征类型[i];
            if (!ct) continue;

            auto* edge = 概念树类::注册关系_同域(新概念节点, 枚举_概念关系::part_of, ct, 调用点);
            if (!edge) continue;
            私有_写边序号(edge, (I64)i, 调用点);
        }

        // 写维度N（可选）
        私有_写概念维度N(新概念节点, (I64)分量特征类型.size(), 调用点);

        return 新概念节点;
    }

    // 词面重载
    static 存在节点类* 注册_聚合特征概念(
        const std::string& 概念词面,
        const std::vector<const 词性节点类*>& 分量特征类型,
        const std::string& 词性标签 = "名词",
        const std::string& 调用点 = "概念_聚合特征树类::注册_聚合特征概念(词面)")
    {
        if (概念词面.empty()) return nullptr;
        const 词性节点类* name = 语素集.添加词性词(概念词面, 词性标签);
        return 注册_聚合特征概念(name, 分量特征类型, 调用点);
    }

    // ============================================================
    // 2) 读取聚合定义：返回按序号排序后的分量特征类型列表
    // ============================================================
    static std::vector<const 词性节点类*> 取分量列表(
        存在节点类* 聚合概念节点,
        const std::string& 调用点 = "概念_聚合特征树类::取分量列表")
    {
        std::vector<const 词性节点类*> out;
        if (!聚合概念节点) return out;

        const 词性节点类* rel = 概念树类::取关系词(枚举_概念关系::part_of);
        if (!rel) return out;

        std::vector<std::pair<I64, const 词性节点类*>> tmp;

        锁调度器守卫 锁({
            锁请求::读(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点)
            });

        if (!聚合概念节点->子) return out;
        auto* first = static_cast<基础信息节点类*>(聚合概念节点->子);
        auto* it = first;
        do {
            auto* mi = it ? dynamic_cast<存在节点主信息类*>(it->主信息) : nullptr;
            if (mi && mi->类型 == rel && mi->名称) {
                const I64 idx = 私有_读边序号_已加锁(static_cast<存在节点类*>(it));
                tmp.push_back({ idx, mi->名称 });
            }
            it = it ? static_cast<基础信息节点类*>(it->下) : nullptr;
        } while (it && it != first);

        std::sort(tmp.begin(), tmp.end(), [](const auto& a, const auto& b) {
            return a.first < b.first;
            });

        for (auto& kv : tmp) out.push_back(kv.second);
        return out;
    }

    // ============================================================
    // 3) 运行时读取：从实例存在上按定义读取 I64 分量值
    // ============================================================
    static bool 读聚合_I64N(
        存在节点类* 实例存在,
        存在节点类* 聚合概念节点,
        std::vector<I64>& out,
        bool 要求全部分量 = true,
        const std::string& 调用点 = "概念_聚合特征树类::读聚合_I64N")
    {
        out.clear();
        if (!实例存在 || !聚合概念节点) return false;

        auto dims = 取分量列表(聚合概念节点, 调用点);
        if (dims.empty()) return false;
        out.resize(dims.size(), 0);

        bool ok = true;

        锁调度器守卫 锁({
            锁请求::读(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点)
            });

        for (std::size_t i = 0; i < dims.size(); ++i) {
            const 词性节点类* t = dims[i];
            I64 v = 0;
            if (!私有_读实例特征I64_已加锁(实例存在, t, v)) {
                if (要求全部分量) ok = false;
            }
            out[i] = v;
        }
        return ok;
    }

private:
    static void 私有_写概念维度N(存在节点类* 概念节点, I64 n, const std::string& 调用点)
    {
        if (!概念节点) return;
        auto* ft = 特征类型_维度N();
        if (!ft) return;

        锁调度器守卫 锁({
            锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点)
            });

        auto* f = 私有_确保子特征_已加锁(概念节点, ft);
        if (!f) return;
        auto* mi = dynamic_cast<特征节点主信息类*>(f->主信息);
        if (!mi) return;
        mi->当前快照 = 特征快照值{ n };
    }

    static void 私有_写边序号(存在节点类* edge, I64 idx, const std::string& 调用点)
    {
        if (!edge) return;
        auto* ft = 特征类型_分量序号();
        if (!ft) return;

        锁调度器守卫 锁({
            锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点)
            });

        auto* f = 私有_确保子特征_已加锁(edge, ft);
        if (!f) return;
        auto* mi = dynamic_cast<特征节点主信息类*>(f->主信息);
        if (!mi) return;
        mi->当前快照 = 特征快照值{ idx };
    }

    static I64 私有_读边序号_已加锁(存在节点类* edge)
    {
        if (!edge || !edge->子) return (I64)0;
        const 词性节点类* ft = 特征类型_分量序号();
        if (!ft) return (I64)0;

        auto* first = static_cast<基础信息节点类*>(edge->子);
        auto* it = first;
        do {
            auto* mi = it ? dynamic_cast<特征节点主信息类*>(it->主信息) : nullptr;
            if (mi && mi->类型 == ft) {
                if (auto* p = std::get_if<I64>(&mi->当前快照)) return *p;
                return (I64)0;
            }
            it = it ? static_cast<基础信息节点类*>(it->下) : nullptr;
        } while (it && it != first);

        return (I64)0;
    }

    static 特征节点类* 私有_确保子特征_已加锁(基础信息节点类* host, const 词性节点类* type)
    {
        if (!host || !type) return nullptr;

        if (host->子) {
            auto* first = static_cast<基础信息节点类*>(host->子);
            auto* it = first;
            do {
                auto* mi = it ? dynamic_cast<特征节点主信息类*>(it->主信息) : nullptr;
                if (mi && mi->类型 == type) return static_cast<特征节点类*>(it);
                it = it ? static_cast<基础信息节点类*>(it->下) : nullptr;
            } while (it && it != first);
        }

        auto* mi = new 特征节点主信息类(type);
        return static_cast<特征节点类*>(世界链.添加子节点_已加锁(host, static_cast<基础信息基类*>(mi)));
    }

    static bool 私有_读实例特征I64_已加锁(存在节点类* inst, const 词性节点类* type, I64& out)
    {
        out = 0;
        if (!inst || !type || !inst->子) return false;

        auto* first = static_cast<基础信息节点类*>(inst->子);
        auto* it = first;
        do {
            auto* mi = it ? dynamic_cast<特征节点主信息类*>(it->主信息) : nullptr;
            if (mi && mi->类型 == type) {
                if (auto* p = std::get_if<I64>(&mi->当前快照)) {
                    out = *p;
                    return true;
                }
                return false;
            }
            it = it ? static_cast<基础信息节点类*>(it->下) : nullptr;
        } while (it && it != first);

        return false;
    }
};
