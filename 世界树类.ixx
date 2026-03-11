module;

#include <cstdint>
#include <string>
#include <vector>
#include <optional>
#include <algorithm>
#include <bit>

export module 世界树模块;

import 主信息定义模块;
import 数据仓库模块;
import 基础数据类型模块;
import 通用函数模块;
import 特征值模块;
import 特征模块;
import 二次特征模块;
import 概念树模块;

// ============================================================
// 世界树模块（Facade / Single Entry）
// ------------------------------------------------------------
// 设计目标（按你最新共识收敛）：
// 1) 世界树类是“唯一入口”：应用层不再直接操作 特征值/特征/存在/场景/二次特征 的底层链表。
// 2) 存在类、场景类只做 CRUD；真正的写入、比较、二次特征生成在世界树里编排。
// 3) Int 不进入特征值链：I64 快照/统计内联在 特征节点主信息。
// 4) VecIU64 进入证据池：特征值模块负责等值去重，特征节点持 VecU句柄。
// 5) 二次特征只存“纯结果标量”：方向性由需求/条件解释（本模块只产出 state）。
// ============================================================

using namespace 数据仓库模块;
using namespace 通用函数模块;
namespace 世界树模块_detail {

    inline std::vector<基础信息节点类*> 枚举子节点_已加锁(基础信息节点类* parent) {
        std::vector<基础信息节点类*> out;
        if (!parent || !parent->子) return out;
        auto* first = static_cast<基础信息节点类*>(parent->子);
        auto* it = first;
        do {
            out.push_back(it);
            it = static_cast<基础信息节点类*>(it->下);
        } while (it && it != first);
        return out;
    }

    template<class TMi>
    inline bool 是主信息(const 基础信息节点类* n) noexcept {
        return n && n->主信息 && dynamic_cast<const TMi*>(n->主信息) != nullptr;
    }

    inline I64 abs_i64_saturate(I64 x) noexcept {
        if (x == I64_MIN) return I64_MAX;
        return x < 0 ? -x : x;
    }



    inline I64 hamming_rate_q10000(const VecIU64& A, const VecIU64& B) noexcept {
        // 以 bit 为单位的差异率 * 10000
        const std::size_t n = std::max(A.size(), B.size());
        if (n == 0) return 0;

        std::uint64_t diffBits = 0;
        std::uint64_t totalBits = (std::uint64_t)n * 64ull;

        const std::size_t m = std::min(A.size(), B.size());
        for (std::size_t i = 0; i < m; ++i) {
            diffBits += (std::uint64_t)std::popcount(A[i] ^ B[i]);
        }
        // 长度不等：把多出来的块视为全不同
        if (A.size() > m) diffBits += (std::uint64_t)(A.size() - m) * 64ull;
        if (B.size() > m) diffBits += (std::uint64_t)(B.size() - m) * 64ull;

        // Q10000
        return (I64)((diffBits * 10000ull) / (totalBits ? totalBits : 1ull));
    }

    inline std::string 主键或空(const 词性节点类* x) {
        return x ? x->获取主键() : std::string{};
    }

} // namespace 世界树模块_detail

export class 世界树类 {
public:
    世界树类() {
        // 默认差异度：
        // - I64：|a-b|
        // - VecU：bit 差异率 Q10000
        特征服务_.设置差异度函数([this](const 词性节点类* /*type*/, const 特征快照值& a, const 特征快照值& b) -> I64 {
            return this->默认差异度(a, b);
            });
    }
    // ============================================================
    //  定义
    // ============================================================
    场景节点类* 现实世界 = nullptr;
    场景节点类* 文本世界 = nullptr;
    场景节点类* 想象世界 = nullptr;
    场景节点类* 记忆世界 = nullptr;
    场景节点类* 推理世界 = nullptr;
    场景节点类* 虚拟世界 = nullptr;
    存在节点类* 自我指针 = nullptr;
    场景节点类* 自我所在场景 = nullptr;
    // ============================================================
    // 0) 初始化
    // ============================================================
    void 初始化(const std::string& 调用点 = "世界树类::初始化") {
        // 证据池缓存重建（可选）
        值池_.初始化();
        // 二次特征概念树根：懒创建即可，这里不强制。
        (void)调用点;
    }

    void 初始化默认世界() {}
    // 世界树类.ixx 里建议加

    void 初始化_概念域根_挂世界根(时间戳 now)
    {
        概念树类 概念树;
        // 世界根
        auto* root = 世界链.根指针;   // 你的世界根指针          



        概念树.获取或创建_概念域根(枚举_概念域::特征, now);
        概念树.获取或创建_概念域根(枚举_概念域::存在, now);
        概念树.获取或创建_概念域根(枚举_概念域::状态, now);
        概念树.获取或创建_概念域根(枚举_概念域::二次特征, now);
        概念树.获取或创建_概念域根(枚举_概念域::因果, now);
        概念树.获取或创建_概念域根(枚举_概念域::自然语言, now);
    }
    // ============================================================
    // 1) 世界根
    // ============================================================
    基础信息节点类* 世界根() const noexcept { return 世界链.根指针; }

    // ============================================================
    // 2) 场景 CRUD
    // ============================================================
    场景节点类* 创建场景(基础信息节点类* parent, 场景节点主信息类* mi, const std::string& 调用点 = "世界树类::创建场景") {
        if (!mi) return nullptr;
        锁调度器守卫 锁({ 锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点) });
        auto* p = parent ? parent : 世界链.根指针;
        return static_cast<场景节点类*>(世界链.添加子节点_已加锁(p, static_cast<基础信息基类*>(mi)));
    }

    bool 删除节点(基础信息节点类* node, const std::string& 调用点 = "世界树类::删除节点") {
        if (!node) return false;
        锁调度器守卫 锁({ 锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点) });
        世界链.删除节点_已加锁(node);
        return true;
    }

    bool 更新主信息(基础信息节点类* node, 基础信息基类* newMi, const std::string& 调用点 = "世界树类::更新主信息") {
        if (!node || !newMi) return false;
        锁调度器守卫 锁({ 锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点) });
        if (node->主信息 == newMi) return true;
        delete node->主信息;
        node->主信息 = newMi;
        return true;
    }

    // 枚举子场景/子存在/子特征
    std::vector<场景节点类*> 获取子场景(基础信息节点类* parent, const std::string& 调用点 = "世界树类::获取子场景") const {
        std::vector<场景节点类*> out;
        if (!parent || !parent->子) return out;
        锁调度器守卫 锁({ 锁请求::读(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点) });
        auto kids = 世界树模块_detail::枚举子节点_已加锁(parent);
        for (auto* n : kids) if (世界树模块_detail::是主信息<场景节点主信息类>(n)) out.push_back(static_cast<场景节点类*>(n));
        return out;
    }

    std::vector<存在节点类*> 获取子存在(基础信息节点类* parent, const std::string& 调用点 = "世界树类::获取子存在") const {
        std::vector<存在节点类*> out;
        if (!parent || !parent->子) return out;
        锁调度器守卫 锁({ 锁请求::读(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点) });
        auto kids = 世界树模块_detail::枚举子节点_已加锁(parent);
        for (auto* n : kids) if (世界树模块_detail::是主信息<存在节点主信息类>(n)) out.push_back(static_cast<存在节点类*>(n));
        return out;
    }

    std::vector<特征节点类*> 获取子特征(基础信息节点类* parent, const std::string& 调用点 = "世界树类::获取子特征") const {
        std::vector<特征节点类*> out;
        if (!parent || !parent->子) return out;
        锁调度器守卫 锁({ 锁请求::读(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点) });
        auto kids = 世界树模块_detail::枚举子节点_已加锁(parent);
        for (auto* n : kids) if (世界树模块_detail::是主信息<特征节点主信息类>(n)) out.push_back(static_cast<特征节点类*>(n));
        return out;
    }

    // ============================================================
    // 3) 存在 CRUD
    // ============================================================
    存在节点类* 创建存在(基础信息节点类* parent, 存在节点主信息类* mi, const std::string& 调用点 = "世界树类::创建存在") {
        if (!mi) return nullptr;
        锁调度器守卫 锁({ 锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点) });
        auto* p = parent ? parent : 世界链.根指针;
        return static_cast<存在节点类*>(世界链.添加子节点_已加锁(p, static_cast<基础信息基类*>(mi)));

    }
        // 兼容/便捷：添加子存在（别名）
        // - 说明：部分实现模块使用“添加子存在”命名；底层仍走 创建存在
        存在节点类* 添加子存在(基础信息节点类 * parent, 存在节点主信息类 * mi, 时间戳 now = 0, const std::string & 调用点 = "世界树类::添加子存在") {
            (void)now; // 当前不写入观测时间，必要时可在 mi->创建时间/最后观测时间 上更新
            return 创建存在(parent, mi, 调用点);
        }

        // 兼容/便捷：写入名称（写主信息.名称）
        bool 写入名称(基础信息节点类 * node, const 词性节点类 * 名称词, 时间戳 now = 0, const std::string & 调用点 = "世界树类::写入名称") {
            if (!node || !名称词) return false;
            锁调度器守卫 锁({ 锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点) });
            auto* bi = node->主信息 ? dynamic_cast<基础信息基类*>(node->主信息) : nullptr;
            if (!bi) return false;
            bi->名称 = const_cast<词性节点类*>(名称词);
            (void)now;
            return true;
        }
    

    // ============================================================
   // 3.5) 元函数：创建/取或创建“干净节点”（不注入内置坐标单位等）
   // ------------------------------------------------------------
   // 说明：
   // - 这里的“取或创建”只在同一父节点下按 主信息.名称/类型 做线性查找；
   // - 序列化暂不考虑：若后续需要跨重启稳定定位，请把引用主键落到虚拟存在中。
   // ============================================================

    场景节点类* 取或创建子场景_按名称(
        基础信息节点类* parent,
        const 词性节点类* 场景名称,
        时间戳 now = 0,
        const std::string& 调用点 = "世界树类::取或创建子场景_按名称")
    {
        if (!场景名称) return nullptr;

        锁调度器守卫 锁({ 锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点) });
        auto* p = parent ? parent : 世界链.根指针;

        const std::string key = 场景名称->获取主键();
        if (p->子) {
            auto* first = static_cast<基础信息节点类*>(p->子);
            auto* it = first;
            do {
                auto* smi = it ? dynamic_cast<场景节点主信息类*>(it->主信息) : nullptr;
                if (smi && smi->名称 && smi->名称->获取主键() == key) {
                    return static_cast<场景节点类*>(it);
                }
                it = static_cast<基础信息节点类*>(it->下);
            } while (it && it != first);
        }

        auto* mi = new 场景节点主信息类();
        mi->名称 = const_cast<词性节点类*>(场景名称);
        // mi->记录观测(now);
        return static_cast<场景节点类*>(世界链.添加子节点_已加锁(p, static_cast<基础信息基类*>(mi)));
    }

    存在节点类* 取或创建子存在_按类型(
        基础信息节点类* parent,
        const 词性节点类* 存在类型,
        时间戳 now = 0,
        const std::string& 调用点 = "世界树类::取或创建子存在_按类型")
    {
        if (!存在类型) return nullptr;

        锁调度器守卫 锁({ 锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点) });
        auto* p = parent ? parent : 世界链.根指针;

        const std::string key = 存在类型->获取主键();
        if (p->子) {
            auto* first = static_cast<基础信息节点类*>(p->子);
            auto* it = first;
            do {
                auto* emi = it ? dynamic_cast<存在节点主信息类*>(it->主信息) : nullptr;
                if (emi && emi->类型 && emi->类型->获取主键() == key) {
                    return static_cast<存在节点类*>(it);
                }
                it = static_cast<基础信息节点类*>(it->下);
            } while (it && it != first);
        }

        auto* mi = new 存在节点主信息类();
        mi->类型 = const_cast<词性节点类*>(存在类型);
        // mi->记录观测(now);
        return static_cast<存在节点类*>(世界链.添加子节点_已加锁(p, static_cast<基础信息基类*>(mi)));
    }



    // ============================================================
    // 3.6) 元函数：取或创建子存在（按 类型 + 特征I64 唯一键）
    // ------------------------------------------------------------
    // 说明：
    // - 解决：同一父节点下存在类型相同但需要按设备/实例区分的问题
    // - 特征值用于唯一键（例如：设备ID哈希）
    // - 只支持 I64 作为唯一键（与你当前特征快照载体一致）
    // ============================================================
    存在节点类* 取或创建子存在_按类型并按特征I64(
        基础信息节点类* parent,
        const 词性节点类* 存在类型,
        const 词性节点类* 唯一键特征类型,
        I64 唯一键特征值,
        时间戳 now = 0,
        const std::string& 调用点 = "世界树类::取或创建子存在_按类型并按特征I64")
    {
        if (!存在类型 || !唯一键特征类型) return nullptr;

        锁调度器守卫 锁({ 锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点) });
        auto* p = parent ? parent : 世界链.根指针;

        const std::string typeKey = 存在类型->获取主键();
        const std::string featKey = 唯一键特征类型->获取主键();

        auto 取子特征I64_不加锁 = [&](基础信息节点类* host) -> std::optional<I64> {
            if (!host || !host->子) return std::nullopt;
            auto* first = static_cast<基础信息节点类*>(host->子);
            auto* it = first;
            do {
                auto* fmi = it ? dynamic_cast<特征节点主信息类*>(it->主信息) : nullptr;
                if (fmi && fmi->类型 && fmi->类型->获取主键() == featKey) {
                    if (std::holds_alternative<I64>(fmi->当前快照)) return std::get<I64>(fmi->当前快照);
                    return std::nullopt;
                }
                it = static_cast<基础信息节点类*>(it->下);
            } while (it && it != first);
            return std::nullopt;
            };

        // 先查找：类型相同且唯一键特征相等
        if (p->子) {
            auto* first = static_cast<基础信息节点类*>(p->子);
            auto* it = first;
            do {
                auto* emi = it ? dynamic_cast<存在节点主信息类*>(it->主信息) : nullptr;
                if (emi && emi->类型 && emi->类型->获取主键() == typeKey) {
                    auto key = 取子特征I64_不加锁(it);
                    if (key && *key == 唯一键特征值) return static_cast<存在节点类*>(it);
                }
                it = static_cast<基础信息节点类*>(it->下);
            } while (it && it != first);
        }

        // 未命中：创建新存在，并写入唯一键特征（初始化快照即可）
        auto* mi = new 存在节点主信息类();
        mi->类型 = const_cast<词性节点类*>(存在类型);
        auto* node = static_cast<存在节点类*>(世界链.添加子节点_已加锁(p, static_cast<基础信息基类*>(mi)));
        if (!node) return nullptr;

        // 写唯一键特征（初始化快照即可；后续写入仍走特征服务）
        auto* f = 确保特征_已加锁(node, 唯一键特征类型);
        if (f) {
            auto* fmi = dynamic_cast<特征节点主信息类*>(f->主信息);
            if (fmi) fmi->当前快照 = 特征快照值{ 唯一键特征值 };
        }
        return node;
    }

    // ============================================================
        // 4) 特征 CRUD + 写入
        // ============================================================
    特征节点类* 查找子特征_按类型(基础信息节点类* host, const 词性节点类* 特征类型, const std::string& 调用点 = "世界树类::查找子特征_按类型") const {
        if (!host || !特征类型 || !host->子) return nullptr;
        锁调度器守卫 锁({ 锁请求::读(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点) });
        auto* first = static_cast<基础信息节点类*>(host->子);
        auto* it = first;
        do {
            auto* mi = it ? dynamic_cast<特征节点主信息类*>(it->主信息) : nullptr;
            if (mi && mi->类型 == 特征类型) return static_cast<特征节点类*>(it);
            it = it ? static_cast<基础信息节点类*>(it->下) : nullptr;
        } while (it && it != first);
        return nullptr;
    }

    特征节点类* 确保特征(基础信息节点类* host, const 词性节点类* 特征类型, const 词性节点类* 特征名称 = nullptr, const std::string& 调用点 = "世界树类::确保特征") {
        if (!host || !特征类型) return nullptr;

        // 先读锁查找
        if (auto* hit = 查找子特征_按类型(host, 特征类型, 调用点)) return hit;

        // 再写锁创建（允许竞态：若别人已创建，则返回已有）
        锁调度器守卫 锁({ 锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点) });
        // 双检
        if (host->子) {
            auto* first = static_cast<基础信息节点类*>(host->子);
            auto* it = first;
            do {
                auto* mi = it ? dynamic_cast<特征节点主信息类*>(it->主信息) : nullptr;
                if (mi && mi->类型 == 特征类型) return static_cast<特征节点类*>(it);
                it = it ? static_cast<基础信息节点类*>(it->下) : nullptr;
            } while (it && it != first);
        }

        auto* mi = new 特征节点主信息类();
        mi->类型 = const_cast<词性节点类*>(特征类型);
        mi->名称 = const_cast<词性节点类*>(特征名称);

        return static_cast<特征节点类*>(世界链.添加子节点_已加锁(host, static_cast<基础信息基类*>(mi)));
    }

    // 写入：I64 内联
    特征写入结果 写入特征_I64(基础信息节点类* host, const 词性节点类* 特征类型, I64 v, const 写入参数& p = {}, const std::string& 调用点 = "世界树类::写入特征_I64") {
        锁调度器守卫 锁({
            锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点)
            });

        auto* f = 确保特征_已加锁(host, 特征类型);
        if (!f) return {};
        return 特征服务_.写入特征值(f, 特征快照值{ v }, p);
    }

    // 写入：VecIU64 证据池 + 句柄
    特征写入结果 写入特征_VecU(基础信息节点类* host, const 词性节点类* 特征类型, const VecIU64& xs, const 写入参数& p = {}, const std::string& 调用点 = "世界树类::写入特征_VecU") {
        // 锁顺序：世界链 -> 特征值链（在锁内调用值池即可）
        锁调度器守卫 锁({
            锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点)
            });

        auto* f = 确保特征_已加锁(host, 特征类型);
        if (!f) return {};

        const VecU句柄 h = 值池_.获取或创建VecU(xs);
        return 特征服务_.写入特征值(f, 特征快照值{ h }, p);
    }

    特征写入结果 写入特征_指针(基础信息节点类* host, const 词性节点类* 特征类型, std::uintptr_t ptr, const 写入参数& p = {}, const std::string& 调用点 = "世界树类::写入特征_指针") {
        锁调度器守卫 锁({
            锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点)
            });

        auto* f = 确保特征_已加锁(host, 特征类型);
        if (!f) return {};
        return 特征服务_.写入特征值(f, 特征快照值{ 指针句柄{ ptr } }, p);
    }



    // ============================================================
    // 4.1) 特征读取（只读快照）
    // ------------------------------------------------------------
    // 说明：
    // - 返回特征节点“当前快照”（I64 或 VecU句柄）
    // - 读取不产生副作用；不写入候选窗口与稳态统计
    // ============================================================
    std::optional<特征快照值> 读取特征快照(
        基础信息节点类* host,
        const 词性节点类* 特征类型,
        const std::string& 调用点 = "世界树类::读取特征快照") const
    {
        if (!host || !特征类型) return std::nullopt;

        // 读锁：世界链
        锁调度器守卫 锁({ 锁请求::读(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点) });

        auto* f = 查找子特征_按类型(host, 特征类型, 调用点);
        if (!f) return std::nullopt;
        auto* fmi = dynamic_cast<特征节点主信息类*>(f->主信息);
        if (!fmi) return std::nullopt;
        return fmi->当前快照;
    }

    std::optional<I64> 读取特征_I64(
        基础信息节点类* host,
        const 词性节点类* 特征类型,
        const std::string& 调用点 = "世界树类::读取特征_I64") const
    {
        auto v = 读取特征快照(host, 特征类型, 调用点);
        if (!v) return std::nullopt;
        if (!std::holds_alternative<I64>(*v)) return std::nullopt;
        return std::get<I64>(*v);
    }

    // ============================================================
        // 5) 存在比较（特征集合聚合）
        // ============================================================
    存在比较结果 比较存在(存在节点类* A, 存在节点类* B, const 集合比较参数& p = {}, const std::string& 调用点 = "世界树类::比较存在") const {
        if (!A || !B) return {};
        锁调度器守卫 锁({
            锁请求::读(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点)
            });
        auto fa = 获取子特征_已加锁(A);
        auto fb = 获取子特征_已加锁(B);
        return 特征服务_.比较特征集合(fa, fb, p);
    }

    // ============================================================
    // 6) 由存在比较结果生成二次特征 state（纯结果）
    // ============================================================
    struct 二次特征生成参数 {
        bool 输出明细 = true;
        bool 输出权重 = false;
        bool 输出缺失码 = true;
        bool 输出覆盖率 = true;
    };

    std::vector<二次特征节点类*> 由存在比较结果生成二次特征(
        存在节点类* A,
        存在节点类* B,
        const 存在比较结果& cmp,
        场景节点类* 归属场景 = nullptr,
        const 二次特征生成参数& gp = {},
        const std::string& 调用点 = "世界树类::由存在比较结果生成二次特征")
    {
        std::vector<二次特征节点类*> out;
        if (!A || !B) return out;

        // 统一在世界链写锁内创建 state 节点
        锁调度器守卫 锁({
            锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点)
            });

        auto attachParent = 归属场景 ? static_cast<基础信息节点类*>(归属场景) : 世界链.根指针;

        auto make_state = [&](const std::string& 链键, 枚举_二次特征种类 kind, I64 val, 基础信息节点类* 左对象, 基础信息节点类* 右对象) -> 二次特征节点类* {
            // 概念命中（读锁在二次特征模块内部处理；这里在写锁内，允许调用它的写锁函数会死锁）
            // 解决：只在这里创建 state 节点，不做命中；命中可在锁外完成或由调用方补。
            // —— 为保持最小可运行，这里仅填链键与标量。

            auto* mi = new 二次特征主信息类();
            mi->形态 = 二次特征主信息类::枚举_二次特征形态::实例_state;
            mi->种类 = kind;
            mi->度量签名_链键 = 链键;
            mi->所属场景 = 归属场景;
            mi->主体 = A;
            mi->客体 = B;
            mi->左对象 = 左对象;
            mi->右对象 = 右对象;
            mi->标量值 = val;

            return static_cast<二次特征节点类*>(世界链.添加子节点_已加锁(attachParent, static_cast<基础信息基类*>(mi)));
            };

        // ---- 存在级聚合 ----
        {
            make_state("存在|总差异度", 枚举_二次特征种类::存在比较, cmp.总差异度, A, B);
            make_state("存在|归一化差异度Q", 枚举_二次特征种类::存在比较, cmp.归一化差异度, A, B);
            const I64 simQ = 10000 - std::min<I64>(10000, std::max<I64>(0, cmp.归一化差异度));
            make_state("存在|总体相似度Q", 枚举_二次特征种类::存在比较, simQ, A, B);
            make_state("存在|命中特征数", 枚举_二次特征种类::存在比较, (I64)cmp.命中特征数, A, B);
            make_state("存在|比较特征数", 枚举_二次特征种类::存在比较, (I64)cmp.明细.size(), A, B);

            if (gp.输出覆盖率) {
                I64 presentBoth = 0;
                for (const auto& d : cmp.明细) if (!d.缺失A && !d.缺失B) presentBoth++;
                const I64 denom = (I64)(cmp.明细.empty() ? 1 : cmp.明细.size());
                const I64 covQ = (presentBoth * 10000) / denom;
                make_state("存在|覆盖率Q", 枚举_二次特征种类::存在比较, covQ, A, B);
            }
        }

        if (!gp.输出明细) return out;

        // ---- 特征级明细 ----
        for (const auto& d : cmp.明细) {
            if (!d.类型) continue;
            const std::string tkey = d.类型->获取主键();

            const std::string kDiff = std::string("特征|差异度Q|T=") + tkey;
            make_state(kDiff, 枚举_二次特征种类::特征比较, d.差异度, A, B);

            if (gp.输出权重) {
                const std::string kW = std::string("特征|权重Q|T=") + tkey;
                make_state(kW, 枚举_二次特征种类::特征比较, d.权重, A, B);
            }

            if (gp.输出缺失码) {
                const int miss = (d.缺失A ? 1 : 0) | (d.缺失B ? 2 : 0);
                const std::string kM = std::string("特征|缺失码|T=") + tkey;
                make_state(kM, 枚举_二次特征种类::特征比较, (I64)miss, A, B);
            }
        }

        return out;
    }

    // ============================================================
    // 7) 二次特征概念命中（锁外调用）
    //    - 用于把 state 的 标量值 映射到概念刻度叶子
    // ============================================================
    二次特征节点类* 为state命中概念模板(二次特征节点类* stateNode, const std::string& 调用点 = "世界树类::为state命中概念模板") {
        if (!stateNode) return nullptr;
        auto* mi = dynamic_cast<二次特征主信息类*>(stateNode->主信息);
        if (!mi) return nullptr;

        // 1) 确保链根
        auto* 链根 = 二次特征类::获取或创建_度量链根(mi->度量签名_链键, mi->种类, 调用点);
        if (!链根) return nullptr;

        // 2) 命中概念节点
        auto* hit = 二次特征类::按差值命中概念节点(链根, mi->标量值, 调用点);
        if (!hit) return nullptr;

        // 3) 记录到 state（写锁）
        {
            锁调度器守卫 锁({
                锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点)
                });
            mi->概念模板 = hit;
        }

        return hit;
    }

    // ============================================================
    // 8) 访问底层服务（必要时）
    // ============================================================
    特征值类& 值池() noexcept { return 值池_; }
    const 特征值类& 值池() const noexcept { return 值池_; }

    特征类& 特征服务() noexcept { return 特征服务_; }
    const 特征类& 特征服务() const noexcept { return 特征服务_; }

private:
    // 内部：在“已持有世界链写锁”的前提下确保特征
    特征节点类* 确保特征_已加锁(基础信息节点类* host, const 词性节点类* 特征类型) {
        if (!host || !特征类型) return nullptr;

        if (host->子) {
            auto* first = static_cast<基础信息节点类*>(host->子);
            auto* it = first;
            do {
                auto* mi = it ? dynamic_cast<特征节点主信息类*>(it->主信息) : nullptr;
                if (mi && mi->类型 == 特征类型) return static_cast<特征节点类*>(it);
                it = it ? static_cast<基础信息节点类*>(it->下) : nullptr;
            } while (it && it != first);
        }

        auto* mi = new 特征节点主信息类();
        mi->类型 = const_cast<词性节点类*>(特征类型);
        return static_cast<特征节点类*>(世界链.添加子节点_已加锁(host, static_cast<基础信息基类*>(mi)));
    }

    // 内部：在“已持有世界链读锁”的前提下获取子特征
    std::vector<特征节点类*> 获取子特征_已加锁(基础信息节点类* parent) const {
        std::vector<特征节点类*> out;
        if (!parent || !parent->子) return out;
        auto* first = static_cast<基础信息节点类*>(parent->子);
        auto* it = first;
        do {
            if (it && it->主信息 && dynamic_cast<特征节点主信息类*>(it->主信息)) out.push_back(static_cast<特征节点类*>(it));
            it = it ? static_cast<基础信息节点类*>(it->下) : nullptr;
        } while (it && it != first);
        return out;
    }

    // 默认差异度（用于没有模板注册表时的最小可运行）
    I64 默认差异度(const 特征快照值& a, const 特征快照值& b) {
        if (a.index() != b.index()) return 10000;
        if (auto* pa = std::get_if<I64>(&a)) {
            const I64 pb = std::get<I64>(b);
            return 世界树模块_detail::abs_i64_saturate(饱和减(*pa, pb));
        }
        if (auto* ha = std::get_if<VecU句柄>(&a)) {
            const auto hb = std::get<VecU句柄>(b);
            const VecIU64* va = 值池_.取VecU只读指针(*ha);
            const VecIU64* vb = 值池_.取VecU只读指针(hb);
            if (!va || !vb) return 10000;
            return 世界树模块_detail::hamming_rate_q10000(*va, *vb);
        }
        return 10000;
    }

public:
    场景节点类* 取内部世界() const noexcept { return 虚拟世界; }

    struct 状态关系结果 {
        bool 同主体 = false;
        bool 同特征 = false;
        bool 同状态域 = false;
        bool 同事件 = false;
        bool 值类型一致 = false;
        bool 值相等 = false;
        bool 值可比较 = false;
        bool 有值变化量 = false;
        I64 值变化量 = 0;
        I64 值变化绝对量 = 0;
        I64 值变化方向 = 0;
        I64 值变化速度_每秒 = 0;
        I64 发生时间差_微秒 = 0;
        I64 收到时间差_微秒 = 0;
        I64 左事件值 = 0;
        I64 右事件值 = 0;
    };

    struct 状态二次特征候选 {
        std::string 度量签名_链键{};
        枚举_二次特征种类 种类 = 枚举_二次特征种类::状态比较;
        I64 标量值 = 0;
        二次特征主信息类::枚举_时间窗 时间窗 = 二次特征主信息类::枚举_时间窗::短;
        基础信息节点类* 左对象 = nullptr;
        基础信息节点类* 右对象 = nullptr;
    };

    struct 状态影响结果候选 {
        状态节点类* 条件状态 = nullptr;
        状态节点类* 结果状态 = nullptr;
        状态关系结果 关系{};
        std::vector<状态二次特征候选> 证据特征;
        std::vector<std::string> 影响度量签名;
        I64 影响评分Q = 0;
        I64 结果偏移方向 = 0;
    };

    状态关系结果 计算状态关系(状态节点类* 左状态, 状态节点类* 右状态) const {
        状态关系结果 out{};
        auto* 左mi = (左状态 && 左状态->主信息) ? dynamic_cast<状态节点主信息类*>(左状态->主信息) : nullptr;
        auto* 右mi = (右状态 && 右状态->主信息) ? dynamic_cast<状态节点主信息类*>(右状态->主信息) : nullptr;
        if (!左mi || !右mi) return out;

        out.同主体 = 左mi->状态主体 == 右mi->状态主体;
        out.同特征 = 左mi->状态特征 == 右mi->状态特征;
        out.同状态域 = 左mi->状态域 == 右mi->状态域;
        out.左事件值 = 左mi->是否变化 ? 1 : 0;
        out.右事件值 = 右mi->是否变化 ? 1 : 0;
        out.同事件 = out.左事件值 == out.右事件值;
        out.值类型一致 = 左mi->状态值.index() == 右mi->状态值.index();
        out.值相等 = 左mi->状态值 == 右mi->状态值;
        out.发生时间差_微秒 = 左mi->发生时间 >= 右mi->发生时间 ? static_cast<I64>(左mi->发生时间 - 右mi->发生时间) : -static_cast<I64>(右mi->发生时间 - 左mi->发生时间);
        out.收到时间差_微秒 = 左mi->收到时间 >= 右mi->收到时间 ? static_cast<I64>(左mi->收到时间 - 右mi->收到时间) : -static_cast<I64>(右mi->收到时间 - 左mi->收到时间);

        if (std::holds_alternative<I64>(左mi->状态值) && std::holds_alternative<I64>(右mi->状态值)) {
            const I64 左值 = std::get<I64>(左mi->状态值);
            const I64 右值 = std::get<I64>(右mi->状态值);
            out.值可比较 = true;
            out.有值变化量 = true;
            out.值变化量 = 饱和减(左值, 右值);
            out.值变化绝对量 = 世界树模块_detail::abs_i64_saturate(out.值变化量);
            out.值变化方向 = out.值变化量 > 0 ? 1 : (out.值变化量 < 0 ? -1 : 0);
            const auto dt = 世界树模块_detail::abs_i64_saturate(out.发生时间差_微秒);
            out.值变化速度_每秒 = dt > 0 ? (out.值变化量 * 1000000) / dt : 0;
        }
        return out;
    }

    std::vector<状态二次特征候选> 尝试计算状态二次特征(
        状态节点类* 左状态,
        状态节点类* 右状态,
        const std::string& 调用点 = "世界树类::尝试计算状态二次特征") const
    {
        (void)调用点;
        std::vector<状态二次特征候选> out;
        auto* 左mi = (左状态 && 左状态->主信息) ? dynamic_cast<状态节点主信息类*>(左状态->主信息) : nullptr;
        auto* 右mi = (右状态 && 右状态->主信息) ? dynamic_cast<状态节点主信息类*>(右状态->主信息) : nullptr;
        if (!左mi || !右mi) return out;

        const auto 关系 = 计算状态关系(左状态, 右状态);
        const auto 时间窗 = 世界树模块_detail::abs_i64_saturate(关系.发生时间差_微秒) <= 1000000 ? 二次特征主信息类::枚举_时间窗::短 : (世界树模块_detail::abs_i64_saturate(关系.发生时间差_微秒) <= 60000000 ? 二次特征主信息类::枚举_时间窗::中 : 二次特征主信息类::枚举_时间窗::长);
        auto push = [&](const std::string& key, I64 value) {
            状态二次特征候选 x{};
            x.度量签名_链键 = key;
            x.标量值 = value;
            x.时间窗 = 时间窗;
            x.左对象 = 左状态;
            x.右对象 = 右状态;
            out.push_back(std::move(x));
        };

        push("状态|同状态域", 关系.同状态域 ? 1 : 0);
        push("状态|同主体", 关系.同主体 ? 1 : 0);
        push("状态|同特征", 关系.同特征 ? 1 : 0);
        push("状态|同事件", 关系.同事件 ? 1 : 0);
        push("状态|值类型一致", 关系.值类型一致 ? 1 : 0);
        push("状态|值相等", 关系.值相等 ? 1 : 0);
        push("状态|发生时间差_微秒", 关系.发生时间差_微秒);
        push("状态|收到时间差_微秒", 关系.收到时间差_微秒);
        push("状态|发生先后", 关系.发生时间差_微秒 > 0 ? 1 : (关系.发生时间差_微秒 < 0 ? -1 : 0));
        push("状态|收到先后", 关系.收到时间差_微秒 > 0 ? 1 : (关系.收到时间差_微秒 < 0 ? -1 : 0));
        push("状态|左事件", 关系.左事件值);
        push("状态|右事件", 关系.右事件值);
        push("状态|左状态域", static_cast<I64>(左mi->状态域));
        push("状态|右状态域", static_cast<I64>(右mi->状态域));
        push("状态|值可比较", 关系.值可比较 ? 1 : 0);
        if (关系.有值变化量) {
            push("状态|值变化量", 关系.值变化量);
            push("状态|值变化绝对量", 关系.值变化绝对量);
            push("状态|值变化方向", 关系.值变化方向);
            push("状态|值变化速度_每秒", 关系.值变化速度_每秒);
        }
        return out;
    }

    状态影响结果候选 尝试分析条件影响结果(
        状态节点类* 条件状态,
        状态节点类* 结果状态,
        const std::string& 调用点 = "世界树类::尝试分析条件影响结果") const
    {
        (void)调用点;
        状态影响结果候选 out{};
        out.条件状态 = 条件状态;
        out.结果状态 = 结果状态;
        out.关系 = 计算状态关系(结果状态, 条件状态);
        out.证据特征 = 尝试计算状态二次特征(结果状态, 条件状态, 调用点);
        out.影响度量签名.reserve(out.证据特征.size());
        for (const auto& x : out.证据特征) out.影响度量签名.push_back(x.度量签名_链键);

        I64 score = 0;
        if (out.关系.发生时间差_微秒 >= 0) score += 3000;
        if (out.关系.同主体) score += 1800;
        if (out.关系.同特征) score += 2200;
        if (out.关系.同状态域) score += 600;
        if (out.关系.值类型一致) score += 500;
        if (out.关系.值可比较) score += 700;
        const I64 dt = 世界树模块_detail::abs_i64_saturate(out.关系.发生时间差_微秒);
        if (dt <= 1000000) score += 1200;
        else if (dt <= 60000000) score += 600;
        if (out.关系.有值变化量) score += std::min<I64>(1000, out.关系.值变化绝对量 * 50);
        if (score < 0) score = 0;
        if (score > 10000) score = 10000;
        out.影响评分Q = score;
        out.结果偏移方向 = out.关系.值变化方向;
        return out;
    }

    std::vector<状态影响结果候选> 批量分析条件影响结果(
        const std::vector<状态节点类*>& 条件状态列表,
        状态节点类* 结果状态,
        const std::string& 调用点 = "世界树类::批量分析条件影响结果") const
    {
        std::vector<状态影响结果候选> out;
        for (auto* 条件 : 条件状态列表) {
            if (!条件) continue;
            out.push_back(尝试分析条件影响结果(条件, 结果状态, 调用点));
        }
        std::sort(out.begin(), out.end(), [](const auto& a, const auto& b) {
            return a.影响评分Q > b.影响评分Q;
        });
        return out;
    }
    // ============================================================
   // 9) 语义函数
   // ============================================================
public:
    场景节点类* 获取自我所在场景() {
        return 自我所在场景;
    }

private:
    特征值类 值池_{};
    特征类 特征服务_{};
};



