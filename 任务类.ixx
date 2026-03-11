// 任务模块.ixx
// ============================================================
// 任务模块（重构版，最小职责）
//
// ✅ 本模块负责：
// 1) 进展Δ计算（方向决定在任务侧）
// 2) 渐进门控（允许执行：连续无进展/背离）
// 3) 任务节点的最小管理（创建/状态写回），不掺杂执行与跨任务调度
//
// ❌ 不在本模块做：
// - 步骤执行（见 任务执行模块）
// - 方法运行/动作线程（见 执行器/动作线程）
// - 跨任务影响与重评估（见 任务调度器模块）
// 任务要么执行（有可用方法），要么分解（生成子任务），子任务沿同样规则递归，直到叶子任务执行并产出结果，父任务用结果聚合推进分支与下一步。
// ============================================================

export module 任务模块;

import <cstdint>;
import <limits>;
import <algorithm>;
import <string>;
import <optional>;

import 主信息定义模块;
import 基础数据类型模块;
import 数据仓库模块;
import 日志模块;
import 通用函数模块;
import 世界树环境模块;
import 状态模块;
import 二次特征模块;
import 语素环境模块;

using namespace 数据仓库模块;
namespace 任务模块_detail {

    // 避免 abs(INT64_MIN) 溢出：返回非负值（用 INT64_MAX 代替极小值的绝对值）。
    inline std::int64_t 安全绝对值(std::int64_t x) noexcept {
        if (x == (std::numeric_limits<std::int64_t>::min)()) {
            return (std::numeric_limits<std::int64_t>::max)();
        }
        return (x >= 0) ? x : -x;
    }

    inline std::int64_t 距离到点(std::int64_t v, std::int64_t 目标点) noexcept {
        return 安全绝对值(v - 目标点);
    }

    // 距离到区间：
    // - 若 lo<=hi：区间有效，返回到区间的距离（区间内为0）
    // - 若 lo>hi：区间无效，按点距离退化（以 lo 作为点）
    inline std::int64_t 距离到区间(std::int64_t v, std::int64_t lo, std::int64_t hi) noexcept {
        if (lo <= hi) {
            if (v < lo) return (lo - v);
            if (v > hi) return (v - hi);
            return 0;
        }
        return 距离到点(v, lo);
    }

    // 超出下限的“违规量”：希望 v <= 下限。
    inline std::int64_t 超出下限(std::int64_t v, std::int64_t 下限) noexcept {
        return (v > 下限) ? (v - 下限) : 0;
    }

    // 低于上限的“缺口量”：希望 v >= 上限。
    inline std::int64_t 低于上限(std::int64_t v, std::int64_t 上限) noexcept {
        return (v < 上限) ? (上限 - v) : 0;
    }

    inline std::int64_t 夹紧置信(std::int64_t 置信) noexcept {
        // 你工程里置信度通常是 i64，语义上更像 0~100 或 0~1000。
        // 这里不强行改尺度，只保证非负。
        return (置信 < 0) ? 0 : 置信;
    }

    inline 时间戳 规范化时间_(时间戳 now) noexcept {
        return now != 0 ? now : 结构体_时间戳::当前_微秒();
    }

    inline const 词性节点类* 类型_存在_任务虚拟存在_() {
        return 语素集.添加词性词("存在_任务虚拟存在", "名词");
    }

    inline const 词性节点类* 名称_存在_任务虚拟存在_() {
        return 语素集.添加词性词("任务虚拟存在", "名词");
    }

    inline const 词性节点类* 名称_场景_任务内部状态_() {
        return 语素集.添加词性词("场景_任务内部状态", "名词");
    }

    inline const 词性节点类* 特征_任务状态_() {
        return 语素集.添加词性词("任务_状态", "名词");
    }

    inline const 词性节点类* 特征_任务当前步骤ID_() {
        return 语素集.添加词性词("任务_当前步骤ID", "名词");
    }

    inline const 词性节点类* 特征_任务当前分支动作_() {
        return 语素集.添加词性词("任务_当前分支动作", "名词");
    }

    inline const 词性节点类* 特征_任务已重试次数_() {
        return 语素集.添加词性词("任务_已重试次数", "名词");
    }

    inline const 词性节点类* 特征_任务允许重试次数_() {
        return 语素集.添加词性词("任务_允许重试次数", "名词");
    }

    inline const 词性节点类* 特征_任务进度_Q10000_() {
        return 语素集.添加词性词("任务_进度_Q10000", "名词");
    }

    inline const 词性节点类* 特征_任务最近调度时间_() {
        return 语素集.添加词性词("任务_最近调度时间", "名词");
    }

    inline const 词性节点类* 特征_任务最近结果节点指针_() {
        return 语素集.添加词性词("任务_最近结果节点指针", "名词");
    }

    inline const 词性节点类* 特征_任务绑定全局容器指针_() {
        return 语素集.添加词性词("任务_绑定全局容器指针", "名词");
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

    inline 存在节点类* 确保任务虚拟存在_(任务信息基类* mi, 时间戳 now, const std::string& 调用点)
    {
        if (!mi) return nullptr;
        if (mi->任务虚拟存在) return mi->任务虚拟存在;
        auto* 内部世界 = 世界树.取内部世界();
        if (!内部世界) return nullptr;

        auto* veMi = new 存在节点主信息类();
        veMi->类型 = const_cast<词性节点类*>(类型_存在_任务虚拟存在_());
        veMi->名称 = const_cast<词性节点类*>(mi->名称 ? mi->名称 : 名称_存在_任务虚拟存在_());
        mi->任务虚拟存在 = 世界树.添加子存在(内部世界, veMi, now, 调用点);
        return mi->任务虚拟存在;
    }

    inline 场景节点类* 取或创建_任务内部状态场景_(任务信息基类* mi, 时间戳 now, const std::string& 调用点)
    {
        auto* ve = 确保任务虚拟存在_(mi, now, 调用点);
        if (!ve) return nullptr;
        return 世界树.取或创建子场景_按名称(ve, 名称_场景_任务内部状态_(), now, 调用点);
    }

    inline void 确保特征槽_(基础信息节点类* 主体, const 词性节点类* 特征类型, const std::string& 调用点)
    {
        if (!主体 || !特征类型) return;
        (void)世界树.确保特征(主体, 特征类型, 特征类型, 调用点);
    }

    inline void 初始化I64特征_(
        场景节点类* 场景,
        基础信息节点类* 主体,
        const 词性节点类* 特征类型,
        I64 值,
        时间戳 now,
        const std::string& 调用点)
    {
        if (!主体 || !特征类型) return;

        const 特征快照值 新值{ 值 };
        const auto 旧值 = 世界树.读取特征快照(主体, 特征类型, 调用点);
        const bool 值发生变化 = !快照相等_(旧值, 新值);
        世界树.写入特征_I64(主体, 特征类型, 值, {}, 调用点);
        if (!场景) return;
        (void)记录内部特征状态_(
            场景,
            主体,
            特征类型,
            新值,
            旧值.has_value() ? 枚举_存在状态事件::变化 : 枚举_存在状态事件::创建,
            值发生变化,
            now,
            调用点);
    }

    inline void 初始化指针特征_(
        场景节点类* 场景,
        基础信息节点类* 主体,
        const 词性节点类* 特征类型,
        std::uintptr_t ptr,
        时间戳 now,
        const std::string& 调用点)
    {
        if (!主体 || !特征类型) return;

        const 特征快照值 新值{ 指针句柄{ ptr } };
        const auto 旧值 = 世界树.读取特征快照(主体, 特征类型, 调用点);
        const bool 值发生变化 = !快照相等_(旧值, 新值);
        世界树.写入特征_指针(主体, 特征类型, ptr, {}, 调用点);
        if (!场景) return;
        (void)记录内部特征状态_(
            场景,
            主体,
            特征类型,
            新值,
            旧值.has_value() ? 枚举_存在状态事件::变化 : 枚举_存在状态事件::创建,
            值发生变化,
            now,
            调用点);
    }

    inline 状态节点类* 记录任务虚拟存在特征_(
        任务信息基类* mi,
        const 词性节点类* 特征类型,
        const 特征快照值& 值,
        时间戳 now,
        const std::string& 调用点)
    {
        if (!mi || !特征类型) return nullptr;
        if (now == 0) now = 结构体_时间戳::当前_微秒();

        auto* ve = 确保任务虚拟存在_(mi, now, 调用点);
        auto* 状态场景 = 取或创建_任务内部状态场景_(mi, now, 调用点);
        if (!ve) return nullptr;

        const auto 旧值 = 世界树.读取特征快照(ve, 特征类型, 调用点);
        const bool 值发生变化 = !快照相等_(旧值, 值);

        auto* feat = 世界树.确保特征(ve, 特征类型, 特征类型, 调用点);
        if (!feat || !feat->主信息) return nullptr;
        if (auto* fmi = dynamic_cast<特征节点主信息类*>(feat->主信息)) {
            fmi->当前快照 = 值;
        }

        if (!状态场景) return nullptr;
        return 记录内部特征状态_(
            状态场景,
            ve,
            特征类型,
            值,
            旧值.has_value() ? 枚举_存在状态事件::变化 : 枚举_存在状态事件::创建,
            值发生变化,
            now,
            调用点);
    }
    inline void 初始化任务虚拟存在信息_(任务信息基类* mi, 时间戳 now, const std::string& 调用点)
    {
        if (!mi) return;

        auto* ve = 确保任务虚拟存在_(mi, now, 调用点);
        auto* 状态场景 = 取或创建_任务内部状态场景_(mi, now, 调用点);
        if (!ve) return;

        初始化I64特征_(状态场景, ve, 特征_任务状态_(), static_cast<I64>(mi->状态), now, 调用点);
        初始化I64特征_(状态场景, ve, 特征_任务当前步骤ID_(), 0, now, 调用点);
        初始化I64特征_(状态场景, ve, 特征_任务当前分支动作_(), 0, now, 调用点);
        初始化I64特征_(状态场景, ve, 特征_任务已重试次数_(), 0, now, 调用点);
        初始化I64特征_(状态场景, ve, 特征_任务允许重试次数_(), 0, now, 调用点);
        初始化I64特征_(状态场景, ve, 特征_任务进度_Q10000_(), 0, now, 调用点);
        初始化I64特征_(状态场景, ve, 特征_任务最近调度时间_(), 0, now, 调用点);
        初始化指针特征_(状态场景, ve, 特征_任务最近结果节点指针_(), 0, now, 调用点);
        初始化指针特征_(状态场景, ve, 特征_任务绑定全局容器指针_(), 0, now, 调用点);
    }

    inline void 写任务内部状态_(任务信息基类* mi, 枚举_存在状态事件 事件, bool 是否变化, 时间戳 now, const std::string& 调用点)
    {
        if (!mi) return;

        auto* ve = 确保任务虚拟存在_(mi, now, 调用点);
        if (!ve) return;
        auto* 状态场景 = 取或创建_任务内部状态场景_(mi, now, 调用点);

        const I64 v = static_cast<I64>(mi->状态);
        const 特征快照值 新值{ v };
        const auto 旧值 = 世界树.读取特征快照(ve, 特征_任务状态_(), 调用点);
        const bool 值发生变化 = !快照相等_(旧值, 新值);

        世界树.写入特征_I64(ve, 特征_任务状态_(), v, {}, 调用点);

        // 特征状态以“存在上的特征值”为主存；只有值变化（或创建）时才补一条状态。
        if (!状态场景 && !mi->场景) return;
        (void)记录内部特征状态_(状态场景 ? 状态场景 : mi->场景, ve, 特征_任务状态_(), 新值, 事件, 值发生变化, now, 调用点);
    }

} // namespace 任务模块_detail


// ============================================================
// 任务类：只保留“方向/进展/门控”这些与任务本体强耦合的规则
// ============================================================
export class 任务类 {

public:
    // ============================================================
    // 1) 增：创建任务节点（挂到父节点下，或挂到根层）
    // ============================================================
    static 任务节点类* 新建任务节点(
        任务信息基类* 主信息,
        任务节点类* 父节点 = nullptr,
        const std::string& 调用点 = "任务管理类::新建任务节点")
    {
        if (!主信息) return nullptr;

        const 时间戳 now = 任务模块_detail::规范化时间_(主信息->创建时间);
        if (主信息->创建时间 == 0) 主信息->创建时间 = now;

        // 任务虚拟存在：用于挂载任务自身统计/解释等特征（避免继续向主信息加字段）
        任务模块_detail::初始化任务虚拟存在信息_(主信息, now, 调用点);

        任务节点类* created = nullptr;
        {
            锁调度器守卫 锁({
                锁请求::写(任务链.链表锁, 枚举_锁域::任务链, "任务链", 调用点)
                });

            if (父节点) {
                created = static_cast<任务节点类*>(任务链.添加子节点_已加锁(父节点, 主信息));
            }
            else {
                // 根层：插到根后（位置=nullptr 会落到 根指针）
                created = static_cast<任务节点类*>(任务链.添加节点_已加锁(nullptr, 主信息));
            }
        }

        任务模块_detail::写任务内部状态_(主信息, 枚举_存在状态事件::创建, false, now, 调用点);
        return created;
    }

    static 状态节点类* 记录任务虚拟存在特征(
        任务信息基类* 主信息,
        const 词性节点类* 特征类型,
        const 特征快照值& 值,
        时间戳 now = 0,
        const std::string& 调用点 = "任务管理类::记录任务虚拟存在特征")
    {
        return 任务模块_detail::记录任务虚拟存在特征_(主信息, 特征类型, 值, now, 调用点);
    }

    static 状态节点类* 写任务虚拟存在_I64特征(
        任务信息基类* 主信息,
        const 词性节点类* 特征类型,
        I64 值,
        时间戳 now = 0,
        const std::string& 调用点 = "任务管理类::写任务虚拟存在_I64特征")
    {
        return 任务模块_detail::记录任务虚拟存在特征_(主信息, 特征类型, 特征快照值{ 值 }, now, 调用点);
    }

    static 状态节点类* 写任务虚拟存在_指针特征(
        任务信息基类* 主信息,
        const 词性节点类* 特征类型,
        std::uintptr_t ptr,
        时间戳 now = 0,
        const std::string& 调用点 = "任务管理类::写任务虚拟存在_指针特征")
    {
        return 任务模块_detail::记录任务虚拟存在特征_(主信息, 特征类型, 特征快照值{ 指针句柄{ ptr } }, now, 调用点);
    }
    static 状态节点类* 标记任务最近调度(
        任务信息基类* 主信息,
        时间戳 now = 0,
        const std::string& 调用点 = "任务管理类::标记任务最近调度")
    {
        if (now == 0) now = 结构体_时间戳::当前_微秒();
        return 写任务虚拟存在_I64特征(主信息, 任务模块_detail::特征_任务最近调度时间_(), static_cast<I64>(now), now, 调用点);
    }

    static 状态节点类* 设置任务当前步骤(
        任务信息基类* 主信息,
        任务节点类* 步骤节点,
        时间戳 now = 0,
        const std::string& 调用点 = "任务管理类::设置任务当前步骤")
    {
        const auto stepId = static_cast<I64>(reinterpret_cast<std::uintptr_t>(步骤节点));
        return 写任务虚拟存在_I64特征(主信息, 任务模块_detail::特征_任务当前步骤ID_(), stepId, now, 调用点);
    }

    static 状态节点类* 设置任务当前分支动作(
        任务信息基类* 主信息,
        枚举_分支选择动作 动作,
        时间戳 now = 0,
        const std::string& 调用点 = "任务管理类::设置任务当前分支动作")
    {
        return 写任务虚拟存在_I64特征(主信息, 任务模块_detail::特征_任务当前分支动作_(), static_cast<I64>(动作), now, 调用点);
    }

    static 状态节点类* 设置任务最近结果节点(
        任务信息基类* 主信息,
        任务节点类* 结果节点,
        时间戳 now = 0,
        const std::string& 调用点 = "任务管理类::设置任务最近结果节点")
    {
        return 写任务虚拟存在_指针特征(主信息, 任务模块_detail::特征_任务最近结果节点指针_(), reinterpret_cast<std::uintptr_t>(结果节点), now, 调用点);
    }

    static 状态节点类* 绑定任务全局容器(
        任务信息基类* 主信息,
        基础信息节点类* 全局容器,
        时间戳 now = 0,
        const std::string& 调用点 = "任务管理类::绑定任务全局容器")
    {
        return 写任务虚拟存在_指针特征(主信息, 任务模块_detail::特征_任务绑定全局容器指针_(), reinterpret_cast<std::uintptr_t>(全局容器), now, 调用点);
    }
    // ============================================================
    // 2) 状态写回：只改主信息，不做节点删除（避免悬空指针）
    // ============================================================
    static void 设置任务状态(
        任务节点类* 节点,
        枚举_任务状态 新状态,
        const std::string& 调用点 = "任务管理类::设置任务状态")
    {
        if (!节点 || !节点->主信息) return;

        using namespace 数据仓库模块;

        bool changed = false;
        任务信息基类* mi = nullptr;
        const 时间戳 now = 结构体_时间戳::当前_微秒();

        {
            锁调度器守卫 锁({
                锁请求::写(任务链.链表锁, 枚举_锁域::任务链, "任务链", 调用点)
                });

            mi = 节点->主信息;
            if (!mi) return;
            changed = (mi->状态 != 新状态);
            mi->状态 = 新状态;
        }

        任务模块_detail::写任务内部状态_(mi, 枚举_存在状态事件::变化, changed, now, 调用点);
    }

    // ============================================================
    // 3) 逻辑取消：不 delete 节点，只把状态置为取消
    // ============================================================
    static void 逻辑取消任务(
        任务节点类* 节点,
        const std::string& 调用点 = "任务管理类::逻辑取消任务")
    {
        设置任务状态(节点, 枚举_任务状态::取消, 调用点);
    }

    // ============================================================
    // 4) 取主信息（只读，不加锁：调用者自己保证并发安全）
    // ============================================================
    static 任务信息基类* 取任务主信息指针_不加锁(任务节点类* 节点) noexcept {
        return 节点 ? 节点->主信息 : nullptr;
    }
};



