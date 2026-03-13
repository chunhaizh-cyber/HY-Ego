// 任务模块.ixx
// ============================================================
// 任务模块（当前实现）
// - 负责任务节点创建、任务虚拟存在写回、头结点需求/场景维护
// - 负责步骤/结果/子任务挂接所需的最小结构操作
// - 不负责具体步骤执行与跨任务调度策略（见 任务执行模块）
// ============================================================

export module 任务模块;

import <cstdint>;
import <algorithm>;
import <vector>;
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

    inline const 词性节点类* 名称_场景_任务头条件_() {
        return 语素集.添加词性词("场景_任务头条件", "名词");
    }

    inline const 词性节点类* 名称_任务_默认_() {
        return 语素集.添加词性词("任务", "名词");
    }

    inline const 词性节点类* 类型_任务_需求驱动_() {
        return 语素集.添加词性词("需求驱动任务", "名词");
    }

    inline const 词性节点类* 特征_任务状态_() {
        return 语素集.添加词性词("任务_状态", "名词");
    }

    inline const 词性节点类* 特征_任务基准优先级_() {
        return 语素集.添加词性词("任务_基准优先级", "名词");
    }

    inline const 词性节点类* 特征_任务局部优先级偏移_() {
        return 语素集.添加词性词("任务_局部优先级偏移", "名词");
    }

    inline const 词性节点类* 特征_任务调度优先级_() {
        return 语素集.添加词性词("任务_调度优先级", "名词");
    }

    inline const 词性节点类* 特征_任务当前步骤ID_() {
        return 语素集.添加词性词("任务_当前步骤ID", "名词");
    }

    inline const 词性节点类* 特征_任务当前子任务头指针_() {
        return 语素集.添加词性词("任务_当前子任务头指针", "名词");
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

    inline bool 快照相等_(const std::optional<特征快照值>& oldValue, const 特征快照值& newValue) noexcept {
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

    inline 存在节点类* 确保任务虚拟存在_(任务信息基类* mi, 时间戳 now, const std::string& 调用点) {
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

    inline 场景节点类* 取或创建_任务内部状态场景_(任务信息基类* mi, 时间戳 now, const std::string& 调用点) {
        auto* ve = 确保任务虚拟存在_(mi, now, 调用点);
        if (!ve) return nullptr;
        return 世界树.取或创建子场景_按名称(ve, 名称_场景_任务内部状态_(), now, 调用点);
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

    inline void 初始化任务虚拟存在信息_(任务信息基类* mi, 时间戳 now, const std::string& 调用点) {
        if (!mi) return;

        auto* ve = 确保任务虚拟存在_(mi, now, 调用点);
        auto* 状态场景 = 取或创建_任务内部状态场景_(mi, now, 调用点);
        if (!ve) return;

        初始化I64特征_(状态场景, ve, 特征_任务状态_(), static_cast<I64>(mi->状态), now, 调用点);
        初始化I64特征_(状态场景, ve, 特征_任务基准优先级_(), static_cast<I64>(mi->基准优先级), now, 调用点);
        初始化I64特征_(状态场景, ve, 特征_任务局部优先级偏移_(), static_cast<I64>(mi->局部优先级偏移), now, 调用点);
        初始化I64特征_(状态场景, ve, 特征_任务调度优先级_(), static_cast<I64>(mi->调度优先级), now, 调用点);
        初始化I64特征_(状态场景, ve, 特征_任务当前步骤ID_(), 0, now, 调用点);
        初始化指针特征_(状态场景, ve, 特征_任务当前子任务头指针_(), 0, now, 调用点);
        初始化I64特征_(状态场景, ve, 特征_任务当前分支动作_(), 0, now, 调用点);
        初始化I64特征_(状态场景, ve, 特征_任务已重试次数_(), 0, now, 调用点);
        初始化I64特征_(状态场景, ve, 特征_任务允许重试次数_(), 0, now, 调用点);
        初始化I64特征_(状态场景, ve, 特征_任务进度_Q10000_(), 0, now, 调用点);
        初始化I64特征_(状态场景, ve, 特征_任务最近调度时间_(), 0, now, 调用点);
        初始化指针特征_(状态场景, ve, 特征_任务最近结果节点指针_(), 0, now, 调用点);
        初始化指针特征_(状态场景, ve, 特征_任务绑定全局容器指针_(), 0, now, 调用点);
    }

    inline void 写任务内部状态_(任务信息基类* mi, 枚举_存在状态事件 事件, bool 是否变化, 时间戳 now, const std::string& 调用点) {
        if (!mi) return;

        auto* ve = 确保任务虚拟存在_(mi, now, 调用点);
        if (!ve) return;
        auto* 状态场景 = 取或创建_任务内部状态场景_(mi, now, 调用点);

        const I64 v = static_cast<I64>(mi->状态);
        const 特征快照值 新值{ v };
        const auto 旧值 = 世界树.读取特征快照(ve, 特征_任务状态_(), 调用点);
        const bool 值发生变化 = !快照相等_(旧值, 新值);

        世界树.写入特征_I64(ve, 特征_任务状态_(), v, {}, 调用点);
        if (!状态场景 && !mi->场景) return;
        (void)记录内部特征状态_(状态场景 ? 状态场景 : mi->场景, ve, 特征_任务状态_(), 新值, 事件, 值发生变化, now, 调用点);
    }

    inline 任务头结点信息* 取头结点信息_(任务节点类* 节点) noexcept {
        return (节点 && 节点->主信息) ? dynamic_cast<任务头结点信息*>(节点->主信息) : nullptr;
    }

    inline const 任务头结点信息* 取头结点信息_(const 任务节点类* 节点) noexcept {
        return (节点 && 节点->主信息) ? dynamic_cast<const 任务头结点信息*>(节点->主信息) : nullptr;
    }

    inline 任务步骤节点信息* 取步骤结点信息_(任务节点类* 节点) noexcept {
        return (节点 && 节点->主信息) ? dynamic_cast<任务步骤节点信息*>(节点->主信息) : nullptr;
    }

    inline const 任务步骤节点信息* 取步骤结点信息_(const 任务节点类* 节点) noexcept {
        return (节点 && 节点->主信息) ? dynamic_cast<const 任务步骤节点信息*>(节点->主信息) : nullptr;
    }

    inline 任务结果节点信息* 取结果结点信息_(任务节点类* 节点) noexcept {
        return (节点 && 节点->主信息) ? dynamic_cast<任务结果节点信息*>(节点->主信息) : nullptr;
    }

    inline const 任务结果节点信息* 取结果结点信息_(const 任务节点类* 节点) noexcept {
        return (节点 && 节点->主信息) ? dynamic_cast<const 任务结果节点信息*>(节点->主信息) : nullptr;
    }

    inline 任务节点类* 推导所属任务头结点_(任务节点类* 节点) noexcept {
        if (!节点 || !节点->主信息) return nullptr;
        if (取头结点信息_(节点)) return 节点;
        if (auto* step = 取步骤结点信息_(节点)) return step->所属任务头结点;
        if (auto* result = 取结果结点信息_(节点)) return result->所属任务头结点;
        return nullptr;
    }

    inline 任务节点类* 推导主任务头结点_(任务节点类* 节点) noexcept {
        auto* head = 推导所属任务头结点_(节点);
        while (head) {
            auto* hmi = 取头结点信息_(head);
            if (!hmi || !hmi->父任务头结点 || hmi->父任务头结点 == head) break;
            head = hmi->父任务头结点;
        }
        return head;
    }

    inline std::int64_t 推导主任务基准优先级_(任务节点类* 节点, std::int64_t fallback = 0) noexcept {
        if (auto* root = 推导主任务头结点_(节点)) {
            if (root->主信息) return root->主信息->基准优先级;
        }
        if (auto* head = 推导所属任务头结点_(节点)) {
            if (head->主信息) return head->主信息->基准优先级;
        }
        return fallback;
    }

    inline 场景节点类* 确保任务头结点条件场景_(任务头结点信息* mi, 时间戳 now, const std::string& 调用点) {
        if (!mi) return nullptr;
        if (mi->场景) return mi->场景;

        auto* parent = mi->任务虚拟存在
            ? static_cast<基础信息节点类*>(mi->任务虚拟存在)
            : static_cast<基础信息节点类*>(世界树.取内部世界());
        if (!parent) return nullptr;

        auto* scene = 世界树.取或创建子场景_按名称(parent, 名称_场景_任务头条件_(), now, 调用点);
        mi->场景 = scene;
        return scene;
    }

} // namespace 任务模块_detail

export class 任务类 {
public:
    struct 结构_创建任务头参数 {
        const 词性节点类* 名称 = nullptr;
        const 自然句节点类* 描述 = nullptr;
        const 词性节点类* 类型 = nullptr;
        场景节点类* 初始场景 = nullptr;
        时间戳 创建时间 = 0;
        std::int64_t 基准优先级 = 0;
        std::int64_t 局部优先级偏移 = 0;
        枚举_任务状态 初始状态 = 枚举_任务状态::就绪;
        枚举_任务树类型 任务树类型 = 枚举_任务树类型::编排任务;
        任务节点类* 父任务头结点 = nullptr;
        任务节点类* 来源父结果节点 = nullptr;
        任务节点类* 来源父步骤节点 = nullptr;
        bool 自动关联到需求 = true;
        bool 自动进入筹办阶段 = false;
    };

    struct 结构_创建步骤参数 {
        const 词性节点类* 名称 = nullptr;
        const 自然句节点类* 描述 = nullptr;
        const 词性节点类* 类型 = nullptr;
        任务节点类* 所属任务头结点 = nullptr;
        场景节点类* 步骤场景 = nullptr;
        std::vector<方法节点类*> 可用方法首节点列表{};
        方法节点类* 当前选中方法首节点 = nullptr;
        时间戳 创建时间 = 0;
        std::int64_t 局部优先级偏移 = 0;
        枚举_任务状态 初始状态 = 枚举_任务状态::就绪;
        std::int32_t 步骤序号 = 0;
        std::int32_t 允许重试次数 = 0;
        时间戳 超时截止时间 = 0;
        bool 允许切换方法 = true;
    };

    struct 结构_创建结果参数 {
        const 词性节点类* 名称 = nullptr;
        const 自然句节点类* 描述 = nullptr;
        const 词性节点类* 类型 = nullptr;
        任务节点类* 所属任务头结点 = nullptr;
        场景节点类* 结果场景 = nullptr;
        枚举_任务结果角色 结果角色 = 枚举_任务结果角色::未定义;
        方法节点类* 来源方法首节点 = nullptr;
        方法节点类* 对应方法结果节点 = nullptr;
        任务节点类* 命中预测结果节点 = nullptr;
        枚举_分支选择动作 命中后动作 = 枚举_分支选择动作::未定义;
        bool 是否预测外结果 = false;
        std::int64_t 置信度_Q10000 = 0;
        std::int64_t 错误码 = 0;
        时间戳 创建时间 = 0;
        std::int64_t 局部优先级偏移 = 0;
        枚举_任务状态 初始状态 = 枚举_任务状态::就绪;
    };

public:
    static 任务节点类* 新建任务节点(
        任务信息基类* 主信息,
        任务节点类* 父节点 = nullptr,
        const std::string& 调用点 = "任务管理类::新建任务节点")
    {
        if (!主信息) return nullptr;

        const 时间戳 now = 任务模块_detail::规范化时间_(主信息->创建时间);
        if (主信息->创建时间 == 0) 主信息->创建时间 = now;

        任务模块_detail::初始化任务虚拟存在信息_(主信息, now, 调用点);

        任务节点类* created = nullptr;
        {
            锁调度器守卫 锁({
                锁请求::写(任务链.链表锁, 枚举_锁域::任务链, "任务链", 调用点)
            });

            if (父节点) created = static_cast<任务节点类*>(任务链.添加子节点_已加锁(父节点, 主信息));
            else created = static_cast<任务节点类*>(任务链.添加节点_已加锁(nullptr, 主信息));
        }

        任务模块_detail::写任务内部状态_(主信息, 枚举_存在状态事件::创建, false, now, 调用点);
        return created;
    }

    static 任务节点类* 创建任务头结点(
        需求节点类* 需求,
        const 结构_创建任务头参数& 参数 = {},
        任务节点类* 父节点 = nullptr,
        const std::string& 调用点 = "任务管理类::创建任务头结点")
    {
        if (!需求 || !需求->主信息) return nullptr;

        auto* 需求信息 = 需求->主信息;
        auto* info = new 任务头结点信息();
        info->名称 = 参数.名称 ? 参数.名称 : (需求信息->类型 ? 需求信息->类型 : 任务模块_detail::名称_任务_默认_());
        info->描述信息 = 参数.描述 ? 参数.描述 : 需求信息->描述信息;
        info->类型 = 参数.类型 ? 参数.类型 : (需求信息->类型 ? 需求信息->类型 : 任务模块_detail::类型_任务_需求驱动_());
        info->节点种类 = 枚举_任务节点种类::头结点;
        info->状态 = 参数.初始状态 == 枚举_任务状态::未定义 ? 枚举_任务状态::就绪 : 参数.初始状态;
        info->创建时间 = 任务模块_detail::规范化时间_(参数.创建时间);
        info->场景 = 参数.初始场景;
        info->任务虚拟存在 = nullptr;
        info->任务树类型 = 参数.任务树类型 == 枚举_任务树类型::未定义 ? 枚举_任务树类型::编排任务 : 参数.任务树类型;
        info->需求 = 需求;
        info->目标维度签名 = 需求信息->需求签名;
        info->父任务头结点 = 参数.父任务头结点 ? 参数.父任务头结点 : (父节点 ? 任务模块_detail::推导所属任务头结点_(父节点) : nullptr);
        info->来源父结果节点 = 参数.来源父结果节点 ? 参数.来源父结果节点 : ((父节点 && 任务模块_detail::取结果结点信息_(父节点)) ? 父节点 : nullptr);
        info->来源父步骤节点 = 参数.来源父步骤节点 ? 参数.来源父步骤节点 : ((父节点 && 任务模块_detail::取步骤结点信息_(父节点)) ? 父节点 : nullptr);
        auto* 主任务头结点 = info->父任务头结点 ? 任务模块_detail::推导主任务头结点_(info->父任务头结点) : nullptr;
        info->基准优先级 = (主任务头结点 && 主任务头结点->主信息)
            ? 主任务头结点->主信息->基准优先级
            : ((参数.基准优先级 != 0) ? 参数.基准优先级 : 需求信息->权重);
        info->局部优先级偏移 = 参数.局部优先级偏移;
        info->调度优先级 = info->基准优先级 + info->局部优先级偏移;

        auto* created = 新建任务节点(info, 父节点, 调用点);
        if (!created) return nullptr;

        if (auto* 父头信息 = 任务模块_detail::取头结点信息_(info->父任务头结点)) {
            if (std::find(父头信息->直属子任务头结点列表.begin(), 父头信息->直属子任务头结点列表.end(), created) == 父头信息->直属子任务头结点列表.end()) {
                父头信息->直属子任务头结点列表.push_back(created);
            }
            父头信息->当前子任务头结点 = created;
        }
        if (auto* 来源结果信息 = 任务模块_detail::取结果结点信息_(info->来源父结果节点)) {
            if (std::find(来源结果信息->派生子任务头结点列表.begin(), 来源结果信息->派生子任务头结点列表.end(), created) == 来源结果信息->派生子任务头结点列表.end()) {
                来源结果信息->派生子任务头结点列表.push_back(created);
            }
        }

        if (参数.自动关联到需求 && 需求->主信息) {
            锁调度器守卫 锁({
                锁请求::写(需求链.链表锁, 枚举_锁域::需求链, "需求链", 调用点)
            });
            if (需求->主信息) 需求->主信息->相关任务 = created;
        }

        if (参数.自动进入筹办阶段) {
            (void)进入筹办阶段(created, 参数.初始场景, 调用点 + "/自动进入筹办");
        }
        return created;
    }

    static 任务节点类* 创建步骤节点(
        任务节点类* 父节点,
        const 结构_创建步骤参数& 参数 = {},
        const std::string& 调用点 = "任务管理类::创建步骤节点")
    {
        if (!父节点 || !父节点->主信息) return nullptr;

        auto* info = new 任务步骤节点信息();
        info->名称 = 参数.名称 ? 参数.名称 : (父节点->主信息->名称 ? 父节点->主信息->名称 : 任务模块_detail::名称_任务_默认_());
        info->描述信息 = 参数.描述;
        info->类型 = 参数.类型 ? 参数.类型 : 父节点->主信息->类型;
        info->节点种类 = 枚举_任务节点种类::步骤节点;
        info->状态 = 参数.初始状态 == 枚举_任务状态::未定义 ? 枚举_任务状态::就绪 : 参数.初始状态;
        info->创建时间 = 任务模块_detail::规范化时间_(参数.创建时间);
        info->场景 = 参数.步骤场景 ? 参数.步骤场景 : 父节点->主信息->场景;
        info->任务虚拟存在 = nullptr;
        info->任务树类型 = 父节点->主信息->任务树类型;
        info->所属任务头结点 = 参数.所属任务头结点 ? 参数.所属任务头结点 : 任务模块_detail::推导所属任务头结点_(父节点);
        info->基准优先级 = 任务模块_detail::推导主任务基准优先级_(info->所属任务头结点, 父节点->主信息->基准优先级);
        info->局部优先级偏移 = 参数.局部优先级偏移;
        info->调度优先级 = info->基准优先级 + info->局部优先级偏移;
        info->可用方法首节点列表 = 参数.可用方法首节点列表;
        info->当前选中方法首节点 = 参数.当前选中方法首节点 ? 参数.当前选中方法首节点 : (info->可用方法首节点列表.empty() ? nullptr : info->可用方法首节点列表.front());
        info->步骤序号 = 参数.步骤序号;
        info->允许重试次数 = 参数.允许重试次数;
        info->超时截止时间 = 参数.超时截止时间;
        info->允许切换方法 = 参数.允许切换方法;

        auto* created = 新建任务节点(info, 父节点, 调用点);
        if (!created) return nullptr;

        if (info->所属任务头结点 && info->所属任务头结点->主信息) {
            auto* hmi = 任务模块_detail::取头结点信息_(info->所属任务头结点);
            if (hmi && !hmi->当前步骤节点) {
                hmi->当前步骤节点 = created;
                (void)设置任务当前步骤(info->所属任务头结点->主信息, created, info->创建时间, 调用点 + "/设置默认当前步骤");
            }
        }
        return created;
    }

    static 任务节点类* 创建结果节点(
        任务节点类* 步骤节点,
        const 结构_创建结果参数& 参数 = {},
        const std::string& 调用点 = "任务管理类::创建结果节点")
    {
        auto* stepInfo = 任务模块_detail::取步骤结点信息_(步骤节点);
        if (!stepInfo || !步骤节点->主信息) return nullptr;

        auto* info = new 任务结果节点信息();
        info->名称 = 参数.名称 ? 参数.名称 : (步骤节点->主信息->名称 ? 步骤节点->主信息->名称 : 任务模块_detail::名称_任务_默认_());
        info->描述信息 = 参数.描述;
        info->类型 = 参数.类型 ? 参数.类型 : 步骤节点->主信息->类型;
        info->节点种类 = 枚举_任务节点种类::结果节点;
        info->状态 = 参数.初始状态 == 枚举_任务状态::未定义 ? 枚举_任务状态::就绪 : 参数.初始状态;
        info->创建时间 = 任务模块_detail::规范化时间_(参数.创建时间);
        info->场景 = 参数.结果场景 ? 参数.结果场景 : 步骤节点->主信息->场景;
        info->任务虚拟存在 = nullptr;
        info->任务树类型 = 步骤节点->主信息->任务树类型;
        info->基准优先级 = 任务模块_detail::推导主任务基准优先级_(stepInfo->所属任务头结点, 步骤节点->主信息->基准优先级);
        info->局部优先级偏移 = 参数.局部优先级偏移;
        info->调度优先级 = info->基准优先级 + info->局部优先级偏移;
        info->结果角色 = 参数.结果角色;
        info->所属任务头结点 = 参数.所属任务头结点 ? 参数.所属任务头结点 : stepInfo->所属任务头结点;
        info->来源步骤节点 = 步骤节点;
        info->来源方法首节点 = 参数.来源方法首节点 ? 参数.来源方法首节点 : stepInfo->当前选中方法首节点;
        info->对应方法结果节点 = 参数.对应方法结果节点;
        info->命中预测结果节点 = 参数.命中预测结果节点;
        info->命中后动作 = 参数.命中后动作;
        info->是否预测外结果 = 参数.是否预测外结果;
        info->置信度_Q10000 = 参数.置信度_Q10000;
        info->错误码 = 参数.错误码;
        info->评估摘要存在 = nullptr;

        if (info->命中后动作 == 枚举_分支选择动作::未定义 && info->命中预测结果节点) {
            if (auto* predicted = 任务模块_detail::取结果结点信息_(info->命中预测结果节点)) {
                info->命中后动作 = predicted->命中后动作;
            }
        }

        auto* created = 新建任务节点(info, 步骤节点, 调用点);
        if (!created) return nullptr;

        if (info->结果角色 == 枚举_任务结果角色::实际结果 && info->所属任务头结点 && info->所属任务头结点->主信息) {
            auto* hmi = 任务模块_detail::取头结点信息_(info->所属任务头结点);
            if (hmi) hmi->最近实际结果节点 = created;
            (void)设置任务最近结果节点(info->所属任务头结点->主信息, created, info->创建时间, 调用点 + "/设置最近实际结果");
        }
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

    static 状态节点类* 设置任务基准优先级(
        任务信息基类* 主信息,
        std::int64_t 基准优先级,
        时间戳 now = 0,
        const std::string& 调用点 = "任务管理类::设置任务基准优先级")
    {
        if (!主信息) return nullptr;
        主信息->基准优先级 = 基准优先级;
        return 写任务虚拟存在_I64特征(主信息, 任务模块_detail::特征_任务基准优先级_(), static_cast<I64>(基准优先级), now, 调用点);
    }

    static 状态节点类* 设置任务局部优先级偏移(
        任务信息基类* 主信息,
        std::int64_t 局部优先级偏移,
        时间戳 now = 0,
        const std::string& 调用点 = "任务管理类::设置任务局部优先级偏移")
    {
        if (!主信息) return nullptr;
        主信息->局部优先级偏移 = 局部优先级偏移;
        return 写任务虚拟存在_I64特征(主信息, 任务模块_detail::特征_任务局部优先级偏移_(), static_cast<I64>(局部优先级偏移), now, 调用点);
    }

    static 状态节点类* 设置任务调度优先级(
        任务信息基类* 主信息,
        std::int64_t 调度优先级,
        时间戳 now = 0,
        const std::string& 调用点 = "任务管理类::设置任务调度优先级")
    {
        if (!主信息) return nullptr;
        主信息->调度优先级 = 调度优先级;
        return 写任务虚拟存在_I64特征(主信息, 任务模块_detail::特征_任务调度优先级_(), static_cast<I64>(调度优先级), now, 调用点);
    }

    static 状态节点类* 设置任务当前步骤(
        任务信息基类* 主信息,
        任务节点类* 步骤节点,
        时间戳 now = 0,
        const std::string& 调用点 = "任务管理类::设置任务当前步骤")
    {
        if (auto* hmi = dynamic_cast<任务头结点信息*>(主信息)) {
            hmi->当前步骤节点 = 步骤节点;
        }
        const auto stepId = static_cast<I64>(reinterpret_cast<std::uintptr_t>(步骤节点));
        return 写任务虚拟存在_I64特征(主信息, 任务模块_detail::特征_任务当前步骤ID_(), stepId, now, 调用点);
    }

    static 状态节点类* 设置任务当前子任务(
        任务信息基类* 主信息,
        任务节点类* 子任务头结点,
        时间戳 now = 0,
        const std::string& 调用点 = "任务管理类::设置任务当前子任务")
    {
        if (auto* hmi = dynamic_cast<任务头结点信息*>(主信息)) {
            hmi->当前子任务头结点 = 子任务头结点;
        }
        if (auto* smi = dynamic_cast<任务步骤节点信息*>(主信息)) {
            smi->当前子任务头结点 = 子任务头结点;
        }
        return 写任务虚拟存在_指针特征(主信息, 任务模块_detail::特征_任务当前子任务头指针_(), reinterpret_cast<std::uintptr_t>(子任务头结点), now, 调用点);
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
        if (auto* hmi = dynamic_cast<任务头结点信息*>(主信息)) {
            hmi->最近实际结果节点 = 结果节点;
        }
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

    static bool 绑定步骤并发子任务(
        任务节点类* 步骤节点,
        任务节点类* 子任务头结点,
        bool 设为当前 = true,
        const std::string& 调用点 = "任务管理类::绑定步骤并发子任务")
    {
        auto* smi = 任务模块_detail::取步骤结点信息_(步骤节点);
        auto* childInfo = 任务模块_detail::取头结点信息_(子任务头结点);
        if (!smi || !childInfo) return false;

        bool added = false;
        {
            锁调度器守卫 锁({
                锁请求::写(任务链.链表锁, 枚举_锁域::任务链, "任务链", 调用点)
            });

            if (std::find(smi->并发子任务头结点列表.begin(), smi->并发子任务头结点列表.end(), 子任务头结点) == smi->并发子任务头结点列表.end()) {
                smi->并发子任务头结点列表.push_back(子任务头结点);
                added = true;
            }
            if (设为当前) smi->当前子任务头结点 = 子任务头结点;
            if (!childInfo->父任务头结点) childInfo->父任务头结点 = smi->所属任务头结点;
            if (!childInfo->来源父步骤节点) childInfo->来源父步骤节点 = 步骤节点;

            if (auto* hmi = 任务模块_detail::取头结点信息_(smi->所属任务头结点)) {
                if (std::find(hmi->直属子任务头结点列表.begin(), hmi->直属子任务头结点列表.end(), 子任务头结点) == hmi->直属子任务头结点列表.end()) {
                    hmi->直属子任务头结点列表.push_back(子任务头结点);
                }
                if (设为当前) hmi->当前子任务头结点 = 子任务头结点;
            }
        }

        if (设为当前) {
            (void)设置任务当前子任务(步骤节点->主信息, 子任务头结点, 结构体_时间戳::当前_微秒(), 调用点 + "/步骤当前子任务");
            if (smi->所属任务头结点 && smi->所属任务头结点->主信息) {
                (void)设置任务当前子任务(smi->所属任务头结点->主信息, 子任务头结点, 结构体_时间戳::当前_微秒(), 调用点 + "/头结点当前子任务");
            }
        }
        return added || 设为当前;
    }

    static bool 绑定结果派生子任务(
        任务节点类* 结果节点,
        任务节点类* 子任务头结点,
        bool 同步绑定到来源步骤 = true,
        bool 设为当前 = false,
        const std::string& 调用点 = "任务管理类::绑定结果派生子任务")
    {
        auto* rmi = 任务模块_detail::取结果结点信息_(结果节点);
        auto* childInfo = 任务模块_detail::取头结点信息_(子任务头结点);
        if (!rmi || !childInfo) return false;

        bool added = false;
        {
            锁调度器守卫 锁({
                锁请求::写(任务链.链表锁, 枚举_锁域::任务链, "任务链", 调用点)
            });

            if (std::find(rmi->派生子任务头结点列表.begin(), rmi->派生子任务头结点列表.end(), 子任务头结点) == rmi->派生子任务头结点列表.end()) {
                rmi->派生子任务头结点列表.push_back(子任务头结点);
                added = true;
            }
            if (!childInfo->父任务头结点) childInfo->父任务头结点 = rmi->所属任务头结点;
            if (!childInfo->来源父结果节点) childInfo->来源父结果节点 = 结果节点;
        }

        if (同步绑定到来源步骤 && rmi->来源步骤节点) {
            (void)绑定步骤并发子任务(rmi->来源步骤节点, 子任务头结点, 设为当前, 调用点 + "/同步到步骤");
        }
        return added || 同步绑定到来源步骤;
    }

    static bool 更新任务头结点需求(
        任务节点类* 头结点,
        需求节点类* 需求,
        bool 自动关联到需求 = true,
        const std::string& 调用点 = "任务管理类::更新任务头结点需求")
    {
        if (!头结点 || !需求 || !需求->主信息) return false;

        const auto now = 结构体_时间戳::当前_微秒();
        bool ok = false;
        {
            锁调度器守卫 锁({
                锁请求::写(任务链.链表锁, 枚举_锁域::任务链, "任务链", 调用点)
            });

            auto* hmi = 任务模块_detail::取头结点信息_(头结点);
            if (!hmi) return false;

            hmi->需求 = 需求;
            hmi->目标维度签名 = 需求->主信息->需求签名;
            if (!hmi->描述信息) hmi->描述信息 = 需求->主信息->描述信息;
            if (!hmi->类型) hmi->类型 = 需求->主信息->类型;
            ok = true;
        }

        if (ok && 自动关联到需求 && 需求->主信息) {
            锁调度器守卫 锁({
                锁请求::写(需求链.链表锁, 枚举_锁域::需求链, "需求链", 调用点)
            });
            if (需求->主信息) 需求->主信息->相关任务 = 头结点;
        }

        if (ok && 头结点->主信息) {
            (void)标记任务最近调度(头结点->主信息, now, 调用点 + "/同步最近调度");
        }
        return ok;
    }

    static 场景节点类* 确保任务头结点场景(
        任务节点类* 头结点,
        时间戳 now = 0,
        const std::string& 调用点 = "任务管理类::确保任务头结点场景")
    {
        auto* hmi = 任务模块_detail::取头结点信息_(头结点);
        if (!hmi) return nullptr;
        if (now == 0) now = 结构体_时间戳::当前_微秒();

        auto* scene = 任务模块_detail::确保任务头结点条件场景_(hmi, now, 调用点);
        if (scene) {
            (void)绑定任务全局容器(hmi, scene, now, 调用点 + "/绑定全局容器");
        }
        return scene;
    }

    static bool 更新任务头结点场景(
        任务节点类* 头结点,
        场景节点类* 条件场景,
        时间戳 now = 0,
        const std::string& 调用点 = "任务管理类::更新任务头结点场景")
    {
        if (!头结点 || !条件场景) return false;
        if (now == 0) now = 结构体_时间戳::当前_微秒();

        bool ok = false;
        任务头结点信息* hmi = nullptr;
        {
            锁调度器守卫 锁({
                锁请求::写(任务链.链表锁, 枚举_锁域::任务链, "任务链", 调用点)
            });

            hmi = 任务模块_detail::取头结点信息_(头结点);
            if (!hmi) return false;
            hmi->场景 = 条件场景;
            ok = true;
        }

        if (ok && hmi) {
            (void)绑定任务全局容器(hmi, 条件场景, now, 调用点 + "/绑定全局容器");
        }
        return ok;
    }

    static bool 进入筹办阶段(
        任务节点类* 头结点,
        场景节点类* 条件场景 = nullptr,
        const std::string& 调用点 = "任务管理类::进入筹办阶段")
    {
        auto* hmi = 任务模块_detail::取头结点信息_(头结点);
        if (!hmi || !hmi->需求 || !hmi->需求->主信息) return false;

        const auto now = 结构体_时间戳::当前_微秒();
        (void)更新任务头结点需求(头结点, hmi->需求, true, 调用点 + "/同步需求");

        auto* scene = 条件场景;
        if (!scene) scene = hmi->场景;
        if (!scene) scene = 确保任务头结点场景(头结点, now, 调用点 + "/确保条件场景");
        else (void)更新任务头结点场景(头结点, scene, now, 调用点 + "/更新条件场景");

        if (!scene) return false;
        设置任务状态(头结点, 枚举_任务状态::筹办中, 调用点);
        return true;
    }

    static void 设置任务状态(
        任务节点类* 节点,
        枚举_任务状态 新状态,
        const std::string& 调用点 = "任务管理类::设置任务状态")
    {
        if (!节点 || !节点->主信息) return;

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

    static bool 删除任务节点(
        任务节点类* 节点,
        const std::string& 调用点 = "任务管理类::删除任务节点")
    {
        if (!节点) return false;

        需求节点类* 需求 = nullptr;
        {
            锁调度器守卫 锁({
                锁请求::读(任务链.链表锁, 枚举_锁域::任务链, "任务链", 调用点)
            });
            if (auto* hmi = 任务模块_detail::取头结点信息_(节点)) {
                需求 = hmi->需求;
            }
        }

        if (需求 && 需求->主信息) {
            锁调度器守卫 锁({
                锁请求::写(需求链.链表锁, 枚举_锁域::需求链, "需求链", 调用点)
            });
            if (需求->主信息 && 需求->主信息->相关任务 == 节点) {
                需求->主信息->相关任务 = nullptr;
            }
        }

        锁调度器守卫 锁({
            锁请求::写(任务链.链表锁, 枚举_锁域::任务链, "任务链", 调用点)
        });
        任务链.删除节点_已加锁(节点);
        return true;
    }

    static void 逻辑取消任务(
        任务节点类* 节点,
        const std::string& 调用点 = "任务管理类::逻辑取消任务")
    {
        设置任务状态(节点, 枚举_任务状态::取消, 调用点);
    }

    static 任务信息基类* 取任务主信息指针_不加锁(任务节点类* 节点) noexcept {
        return 节点 ? 节点->主信息 : nullptr;
    }
};

