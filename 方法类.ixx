module;

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <shared_mutex>
#include <functional>
#include <atomic>
#include <algorithm>
#include <optional>
#include <stdexcept>
#include <sstream>

export module 方法模块;

import 主信息定义模块;
import 数据仓库模块;
import 语素环境模块;
import 基础数据类型模块;
import 特征模块;
import 概念_特征树模块;
import 度量模板注册表模块;
import 本能动作管理模块;
import 通用函数模块;
import 世界树环境模块;
import 状态模块;
import 二次特征模块;

using namespace 数据仓库模块;
using namespace 通用函数模块;
// ============================================================================
// 方法模块（更新版，2026-02-26）
//
// 设计定位：
//  1) 方法树维护（首/条件/结果） + 查找（动作名/影响签名/结果token）
//  2) 不直接暴露特征值存储细节；结果摘要只依赖稳定二级键（ChainKey + 概念名）
//  3) 不做兼容：旧的“基于特征值节点指针”的摘要提取逻辑已删除
//
// 依赖共识：
//  - ChainKey（度量签名_链键）是业务二级键
//  - 概念刻度命中由 概念_特征树类 提供
//  - 结果摘要索引（方法结果节点主信息::结果摘要索引）允许只存“轻量快照”
//    但其主信息指针必须由本模块的池托管，避免悬空
// ============================================================================

export struct 结构_方法上下文 {
    时间戳 now = 0;

    // 方法参数：当次输入
    场景节点类* 输入观测包场景 = nullptr;   // 必需：场景_观测包（只含特征值/句柄）
    场景节点类* 任务条件场景 = nullptr;   // 可选：锁定主键/ROI/上一帧引用等
    场景节点类* 调用覆盖场景 = nullptr;   // 可选：本次临时覆盖 policy（不沉淀）

    // 引用：首节点（长期状态不放 ctx）
    方法节点类* 方法首节点 = nullptr;       // 允许为空；本能方法第1步要补齐


    const 场景节点类* 当前场景 = nullptr;      // 可选：当前世界/条件场景
    const 场景节点类* 条件场景 = nullptr;      // 可选：方法条件模板或实例
};

export struct 结构_方法执行结果 {
    bool 成功 = false;
    场景节点类* 结果存在集场景 = nullptr;   // 场景_存在集_稳定
    场景节点类* 结果事件集场景 = nullptr;   // 可选：场景_动态事件集


    std::int64_t 成功码 = 0;
    std::int64_t 置信度 = 0;
    std::int64_t 质量_Q10000 = 0;
    std::int64_t 成本_Q10000 = 0;
    //    场景节点类* 结果场景 = nullptr;
    std::string 解释{};
};

// ----------------------------------------------------------------------


export struct 结构_方法候选 {
    方法节点类* 方法首节点 = nullptr;
    方法节点类* 匹配结果节点 = nullptr;

    std::int64_t 评分Q = 0;     // Q10000：命中率Q 或 overlap*Q
    std::uint32_t 命中数量 = 0;
    std::uint32_t 目标数量 = 0;

    std::vector<std::string> 命中Token; // 调试用途
};

export struct 结构_方法参数尝试候选 {
    方法节点类* 方法首节点 = nullptr;
    状态节点类* 条件状态 = nullptr;
    状态节点类* 目标结果状态 = nullptr;
    基础信息节点类* 参数主体 = nullptr;
    特征节点类* 参数特征 = nullptr;
    特征快照值 建议值{};
    I64区间 建议值域{ 0, -1 };
    I64区间 预期结果区间{ 0, -1 };
    std::vector<std::string> 证据签名;
    std::int64_t 评分Q = 0;
    std::int64_t 调整方向 = 0;
    bool 来自二次特征 = false;
    bool 需要随机探索 = false;
};

export struct 结构_尝试学习参数生成结果 {
    场景节点类* 参数场景 = nullptr;
    std::vector<结构_方法参数尝试候选> 候选列表;
};



namespace 方法模块_detail {

    inline std::string 动作句柄_键(const 结构体_动作句柄& h) {
        std::ostringstream oss;
        switch (h.类型) {
        case 枚举_动作句柄类型::本能函数ID:
            oss << "innate:" << h.本能ID_U64;
            break;
        case 枚举_动作句柄类型::外部实现主键:
            oss << "ext:" << h.外部实现主键;
            break;
        case 枚举_动作句柄类型::动作序列主键:
            oss << "seq:" << h.动作序列主键;
            break;
        default:
            oss << "none";
            break;
        }
        return oss.str();
    }

    inline const 方法首节点主信息类* MI首(const 方法节点类* n) {
        return n ? dynamic_cast<const 方法首节点主信息类*>(n->主信息) : nullptr;
    }
    inline 方法首节点主信息类* MI首(方法节点类* n) {
        return n ? dynamic_cast<方法首节点主信息类*>(n->主信息) : nullptr;
    }
    inline 方法结果节点主信息类* MI果(方法节点类* n) {
        return n ? dynamic_cast<方法结果节点主信息类*>(n->主信息) : nullptr;
    }
    inline const 方法结果节点主信息类* MI果(const 方法节点类* n) {
        return n ? dynamic_cast<const 方法结果节点主信息类*>(n->主信息) : nullptr;
    }
    inline 方法条件节点主信息类* MI条(方法节点类* n) {
        return n ? dynamic_cast<方法条件节点主信息类*>(n->主信息) : nullptr;
    }

    // 同层环遍历（node->下）
    template<class Node, class Fn>
    inline void 遍历同层(Node* start, Fn&& fn) {
        if (!start) return;
        Node* it = start;
        do {
            fn(it);
            it = static_cast<Node*>(it->下);
        } while (it && it != start);
    }

    // 遍历方法树下所有结果节点（结果可能挂在首节点或条件节点下）
    template<class Fn>
    inline void 遍历方法树_结果节点(方法节点类* 首节点, Fn&& fn) {
        if (!首节点 || !首节点->子) return;

        std::function<void(方法节点类*)> dfs = [&](方法节点类* node) {
            if (!node) return;
            if (dynamic_cast<方法结果节点主信息类*>(node->主信息)) fn(node);
            if (!node->子) return;
            遍历同层(static_cast<方法节点类*>(node->子), [&](方法节点类* ch) {
                dfs(ch);
                });
            };

        dfs(首节点);
    }

    // 二次特征摘要 token：只用稳定二级键，不用节点主键
    inline std::string 二次特征主信息_转Token(const 二次特征主信息类* mi) {
        if (!mi) return {};
        if (mi->度量签名_链键.empty()) return {};

        std::ostringstream oss;
        oss << mi->度量签名_链键;

        // 优先：命中概念模板 -> 使用模板的概念名称
        if (mi->概念模板) {
            const auto* tmi = dynamic_cast<const 二次特征主信息类*>(mi->概念模板->主信息);
            if (tmi && tmi->概念名称 && !tmi->概念名称->获取主键().empty()) {
                oss << "|概念=" << tmi->概念名称->获取主键();
                return oss.str();
            }
        }

        // 次选：自身概念名称
        if (mi->概念名称 && !mi->概念名称->获取主键().empty()) {
            oss << "|概念=" << mi->概念名称->获取主键();
            return oss.str();
        }

        // 兜底：只返回链键（粗召回）
        return oss.str();
    }

    inline std::string 二次特征节点_转Token(const 二次特征节点类& n) {
        return 二次特征主信息_转Token(dynamic_cast<const 二次特征主信息类*>(n.主信息));
    }

    inline std::size_t 签名重合数_(const std::vector<std::string>& A, const std::vector<std::string>& B) {
        if (A.empty() || B.empty()) return 0;
        std::unordered_set<std::string> setA;
        setA.reserve(A.size());
        for (const auto& s : A) if (!s.empty()) setA.insert(s);
        std::size_t hit = 0;
        for (const auto& s : B) {
            if (!s.empty() && setA.find(s) != setA.end()) ++hit;
        }
        return hit;
    }



    inline const 词性节点类* 特征_二次特征标量值_() {
        return 语素集.添加词性词("二次特征_标量值", "名词");
    }

    inline bool 快照相等_(const std::optional<特征快照值>& oldValue, const 特征快照值& newValue) noexcept
    {
        return oldValue.has_value() && *oldValue == newValue;
    }

    inline 特征节点主信息类* 取特征主信息_(基础信息节点类* n) noexcept {
        return (n && n->主信息) ? dynamic_cast<特征节点主信息类*>(n->主信息) : nullptr;
    }

    inline const 特征节点主信息类* 取特征主信息_(const 基础信息节点类* n) noexcept {
        return (n && n->主信息) ? dynamic_cast<const 特征节点主信息类*>(n->主信息) : nullptr;
    }

    inline 存在节点主信息类* 取存在主信息_(基础信息节点类* n) noexcept {
        return (n && n->主信息) ? dynamic_cast<存在节点主信息类*>(n->主信息) : nullptr;
    }

    inline const 存在节点主信息类* 取存在主信息_(const 基础信息节点类* n) noexcept {
        return (n && n->主信息) ? dynamic_cast<const 存在节点主信息类*>(n->主信息) : nullptr;
    }

    inline 场景节点主信息类* 取场景主信息_(基础信息节点类* n) noexcept {
        return (n && n->主信息) ? dynamic_cast<场景节点主信息类*>(n->主信息) : nullptr;
    }

    inline const 场景节点主信息类* 取场景主信息_(const 基础信息节点类* n) noexcept {
        return (n && n->主信息) ? dynamic_cast<const 场景节点主信息类*>(n->主信息) : nullptr;
    }

    inline void 挂子_仅同层环_(基础信息节点类* 父, 基础信息节点类* 子)
    {
        if (!父 || !子) return;
        子->父 = 父;
        子->根 = 父->根 ? 父->根 : 父;

        if (!父->子) {
            父->子 = 子;
            子->上 = 子;
            子->下 = 子;
            return;
        }

        auto* start = static_cast<基础信息节点类*>(父->子);
        auto* last = static_cast<基础信息节点类*>(start->上);
        last->下 = 子;
        子->上 = last;
        子->下 = start;
        start->上 = 子;
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
} // namespace 方法模块_detail

// ============================================================================
// 方法类
// ============================================================================
export class 方法类 {
public:
    using 本能函数 = std::function<结构_方法执行结果(结构_方法上下文& ctx, std::atomic_bool& cancel)>;

public:
    方法类() = default;

    // =============================================================
    // 注册/构建
    // =============================================================

    方法节点类* 注册方法首节点(
        const 词性节点类* 动作名,
        const 结构体_动作句柄& 动作句柄,
        枚举_方法沉淀来源 来源 = 枚举_方法沉淀来源::未定义,
        const std::vector<std::string>& 影响度量签名 = {},
        const std::string& 调用点 = "方法类::注册方法首节点")
    {
        using namespace 数据仓库模块;
        using namespace 方法模块_detail;

        方法节点类* result = nullptr;
        {
            锁调度器守卫 锁({
                锁请求::写(方法链.链表锁, 枚举_锁域::方法链, "方法链", 调用点)
                });

            if (!方法链.根指针) return nullptr;

            // 1) 先按句柄查找（更强）
            const std::string handleKey = 动作句柄_键(动作句柄);

            // 构建一次轻索引（在写锁内可直接扫）
            方法节点类* found = nullptr;
            if (方法链.根指针->子) {
                遍历同层(static_cast<方法节点类*>(方法链.根指针->子), [&](方法节点类* head) {
                    auto* hmi = MI首(head);
                    if (!hmi) return;
                    if (动作句柄.有效() && 动作句柄_键(hmi->动作句柄) == handleKey) {
                        found = head;
                        return;
                    }
                    });
            }

            // 2) 句柄不命中再按动作名主键
            if (!found && 动作名 && 方法链.根指针->子) {
                const std::string nameKey = 动作名->获取主键();
                遍历同层(static_cast<方法节点类*>(方法链.根指针->子), [&](方法节点类* head) {
                    auto* hmi = MI首(head);
                    if (!hmi || !hmi->动作名) return;
                    if (hmi->动作名->获取主键() == nameKey) {
                        found = head;
                        return;
                    }
                    });
            }

            if (found) {
                result = found;
            }
            else {
                // 创建新首节点
                auto* mi = new 方法首节点主信息类(动作名, 动作句柄);
                mi->来源 = 来源;
                mi->影响度量签名 = 影响度量签名;

                auto* head = 方法链.添加子节点_已加锁(方法链.根指针, static_cast<方法信息基类*>(mi));
                标记索引脏_已加锁();
                result = static_cast<方法节点类*>(head);
            }
        }

        if (result) {
            初始化方法虚拟存在信息(result, 结构体_时间戳::当前_微秒(), 调用点);
        }
        return result;
    }

    // =============================================================
    // 元函数：本能方法首节点/参数模板/方法虚拟存在
    // -------------------------------------------------------------
    // 约束（按你最新共识）：
    // 1) 结构_方法上下文 / 结构_方法执行结果 只承载“当次参数/返回”，不承载长期状态。
    // 2) 长期状态（policy/统计/学习指标）落到“方法虚拟存在”。
    // 3) 序列化暂不考虑：当前用内存缓存（方法首节点指针 -> 世界树节点指针）。
    // 4) 参数模板场景必须是“理想场景”：不自动插入坐标单位/变换等内置节点（因此用 世界链 直接创建）。
    // =============================================================

    方法节点类* 查找或创建_本能方法首节点(
        std::uint64_t 本能ID,
        时间戳 now = 0,
        const std::string& 调用点 = "方法类::查找或创建_本能方法首节点")
    {
        私有_确保元词_();

        结构体_动作句柄 h{};
        h.类型 = 枚举_动作句柄类型::本能函数ID;
        h.本能ID_U64 = 本能ID;

        const std::string name = std::string("本能_") + std::to_string(本能ID);
        const 词性节点类* 动作名 = 语素集.添加词性词(name, "动词");
        return 注册方法首节点(动作名, h, 枚举_方法沉淀来源::未定义, {}, 调用点);
    }

    // 如果你的工程里有 枚举_本能函数ID，可直接用这个重载
    方法节点类* 查找或创建_本能方法首节点(
        枚举_本能动作ID 本能ID,
        时间戳 now = 0,
        const std::string& 调用点 = "方法类::查找或创建_本能方法首节点")
    {
        return 查找或创建_本能方法首节点(static_cast<std::uint64_t>(本能ID), now, 调用点);
    }

    // 取或创建：方法虚拟存在（长期：policy/统计）
    存在节点类* 取或创建_方法虚拟存在(
        方法节点类* 方法首节点,
        时间戳 now = 0,
        const std::string& 调用点 = "方法类::取或创建_方法虚拟存在")
    {
        if (!方法首节点) return nullptr;

        // 先查缓存（不与世界锁交叉持有）
        {
            std::shared_lock lk(元缓存_mtx_);
            auto it = cache_首到虚拟存在_.find(方法首节点);
            if (it != cache_首到虚拟存在_.end()) return it->second;
        }

        私有_确保元词_();

        // 当前落到“内部世界”（暂以 世界树.虚拟世界 为实现根）
        auto* 内部世界 = 世界树.取内部世界();
        if (!内部世界) return nullptr;

        锁调度器守卫 锁({
            锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点)
            });

        auto* mi = new 存在节点主信息类();
        // 若主信息字段名不同，你可以在主信息定义模块统一；此处先按惯例写入 类型/名称
        mi->类型 = const_cast<词性节点类*>(类型_存在_方法虚拟存在_);
        mi->名称 = const_cast<词性节点类*>(名称_存在_方法虚拟存在_);

        // 若存在主信息提供 记录观测(now)，可启用（未定义就先忽略）
        // mi->记录观测(now);

        auto* node = static_cast<存在节点类*>(
            世界链.添加子节点_已加锁(内部世界, static_cast<基础信息基类*>(mi))
            );

        if (auto* base = dynamic_cast<方法信息基类*>(方法首节点->主信息)) base->方法虚拟存在根 = node;
        {
            std::unique_lock lk(元缓存_mtx_);
            cache_首到虚拟存在_[方法首节点] = node;
        }
        return node;
    }

    // 取或创建：方法参数模板场景（理想场景，仅用于“是否可调用”的结构约束）
    场景节点类* 取或创建_方法参数模板场景(
        方法节点类* 方法首节点,
        时间戳 now = 0,
        const std::string& 调用点 = "方法类::取或创建_方法参数模板场景")
    {
        if (!方法首节点) return nullptr;

        // 先查缓存
        {
            std::shared_lock lk(元缓存_mtx_);
            auto it = cache_首到参数模板场景_.find(方法首节点);
            if (it != cache_首到参数模板场景_.end()) return it->second;
        }

        私有_确保元词_();

        auto* v = 取或创建_方法虚拟存在(方法首节点, now, 调用点);
        if (!v) return nullptr;

        锁调度器守卫 锁({
            锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点)
            });

        场景节点类* found = nullptr;
        const std::string nameKey = 名称_场景_方法参数模板_->获取主键();

        // 查找 v 的子节点里是否已有同名模板场景
        if (v->子) {
            auto* first = static_cast<基础信息节点类*>(v->子);
            auto* it = first;
            do {
                auto* smi = it ? dynamic_cast<场景节点主信息类*>(it->主信息) : nullptr;
                if (smi && smi->名称 && smi->名称->获取主键() == nameKey) {
                    found = static_cast<场景节点类*>(it);
                    break;
                }
                it = static_cast<基础信息节点类*>(it->下);
            } while (it && it != first);
        }

        if (!found) {
            auto* mi = new 场景节点主信息类();
            mi->名称 = const_cast<词性节点类*>(名称_场景_方法参数模板_);
            // mi->记录观测(now);
            found = static_cast<场景节点类*>(
                世界链.添加子节点_已加锁(v, static_cast<基础信息基类*>(mi))
                );
        }

        {
            std::unique_lock lk(元缓存_mtx_);
            cache_首到参数模板场景_[方法首节点] = found;
        }
        return found;
    }
    场景节点类* 取或创建_方法内部状态场景(
        方法节点类* 方法首节点,
        时间戳 now = 0,
        const std::string& 调用点 = "方法类::取或创建_方法内部状态场景")
    {
        if (!方法首节点) return nullptr;

        {
            std::shared_lock lk(元缓存_mtx_);
            auto it = cache_首到内部状态场景_.find(方法首节点);
            if (it != cache_首到内部状态场景_.end()) return it->second;
        }

        私有_确保元词_();

        auto* v = 取或创建_方法虚拟存在(方法首节点, now, 调用点);
        if (!v) return nullptr;

        锁调度器守卫 锁({
            锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点)
            });

        场景节点类* found = nullptr;
        const std::string nameKey = 名称_场景_方法内部状态_->获取主键();

        if (v->子) {
            auto* first = static_cast<基础信息节点类*>(v->子);
            auto* it = first;
            do {
                auto* smi = it ? dynamic_cast<场景节点主信息类*>(it->主信息) : nullptr;
                if (smi && smi->名称 && smi->名称->获取主键() == nameKey) {
                    found = static_cast<场景节点类*>(it);
                    break;
                }
                it = static_cast<基础信息节点类*>(it->下);
            } while (it && it != first);
        }

        if (!found) {
            auto* mi = new 场景节点主信息类();
            mi->名称 = const_cast<词性节点类*>(名称_场景_方法内部状态_);
            found = static_cast<场景节点类*>(
                世界链.添加子节点_已加锁(v, static_cast<基础信息基类*>(mi))
                );
        }

        {
            std::unique_lock lk(元缓存_mtx_);
            cache_首到内部状态场景_[方法首节点] = found;
        }
        return found;
    }

    状态节点类* 记录方法虚拟存在特征(
        方法节点类* 方法首节点,
        const 词性节点类* 特征类型,
        const 特征快照值& 值,
        时间戳 now = 0,
        const std::string& 调用点 = "方法类::记录方法虚拟存在特征")
    {
        using namespace 方法模块_detail;
        if (!方法首节点 || !特征类型) return nullptr;
        if (now == 0) now = 结构体_时间戳::当前_微秒();

        auto* ve = 取或创建_方法虚拟存在(方法首节点, now, 调用点);
        auto* 状态场景 = 取或创建_方法内部状态场景(方法首节点, now, 调用点);
        if (!ve || !状态场景) return nullptr;

        const auto oldValue = 世界树.读取特征快照(ve, 特征类型, 调用点);
        const bool changed = !快照相等_(oldValue, 值);

        {
            锁调度器守卫 锁({
                锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点)
                });

            auto* f = 私有_取或创建_特征节点_按类型_(ve, 特征类型, now, 调用点);
            if (!f) return nullptr;

            auto* fmi = dynamic_cast<特征节点主信息类*>(f->主信息);
            if (!fmi) return nullptr;
            fmi->当前快照 = 值;
        }

        auto* feat = 世界树.确保特征(ve, 特征类型, 特征类型, 调用点);
        return 状态集.记录内部特征状态(
            状态场景,
            ve,
            feat,
            值,
            oldValue.has_value() ? 枚举_存在状态事件::变化 : 枚举_存在状态事件::创建,
            changed,
            now,
            [](场景节点类* s, 状态节点类* n, 时间戳 ts, const std::string& cp) {
                二次特征类::状态记录后刷新二次特征(s, n, ts, cp);
            },
            调用点);
    }

    void 同步场景特征到方法虚拟存在(
        方法节点类* 方法首节点,
        基础信息节点类* 根节点,
        时间戳 now = 0,
        const std::string& 调用点 = "方法类::同步场景特征到方法虚拟存在")
    {
        if (!方法首节点 || !根节点) return;
        if (now == 0) now = 结构体_时间戳::当前_微秒();

        std::function<void(基础信息节点类*)> walk = [&](基础信息节点类* n) {
            if (!n) return;

            if (auto* fmi = dynamic_cast<特征节点主信息类*>(n->主信息)) {
                if (fmi->类型 && !std::holds_alternative<std::monostate>(fmi->当前快照)) {
                    (void)记录方法虚拟存在特征(方法首节点, fmi->类型, fmi->当前快照, now, 调用点);
                }
            }

            if (!n->子) return;
            auto* start = static_cast<基础信息节点类*>(n->子);
            auto* cur = start;
            do {
                walk(cur);
                cur = cur ? static_cast<基础信息节点类*>(cur->下) : nullptr;
            } while (cur && cur != start);
        };

        walk(根节点);
    }

    void 初始化方法虚拟存在信息(
        方法节点类* 方法首节点,
        时间戳 now = 0,
        const std::string& 调用点 = "方法类::初始化方法虚拟存在信息")
    {
        if (!方法首节点 || !方法首节点->主信息) return;
        if (now == 0) now = 结构体_时间戳::当前_微秒();
        私有_确保元词_();

        auto* hmi = 方法模块_detail::MI首(方法首节点);
        if (!hmi) return;

        (void)取或创建_方法虚拟存在(方法首节点, now, 调用点);
        (void)取或创建_方法内部状态场景(方法首节点, now, 调用点);

        (void)记录方法虚拟存在特征(方法首节点, 特征_方法动作句柄类型_, 特征快照值{ (I64)hmi->动作句柄.类型 }, now, 调用点);
        (void)记录方法虚拟存在特征(方法首节点, 特征_方法沉淀来源_, 特征快照值{ (I64)hmi->来源 }, now, 调用点);
        (void)记录方法虚拟存在特征(方法首节点, 特征_方法影响签名数量_, 特征快照值{ (I64)hmi->影响度量签名.size() }, now, 调用点);
        if (hmi->动作句柄.类型 == 枚举_动作句柄类型::本能函数ID) {
            (void)记录方法虚拟存在特征(方法首节点, 特征_方法本能动作ID_, 特征快照值{ (I64)hmi->动作句柄.本能ID_U64 }, now, 调用点);
        }

        (void)记录方法虚拟存在特征(方法首节点, 特征_方法运行状态_, 特征快照值{ (I64)0 }, now, 调用点);
        (void)记录方法虚拟存在特征(方法首节点, 特征_运行次数_, 特征快照值{ (I64)0 }, now, 调用点);
        (void)记录方法虚拟存在特征(方法首节点, 特征_成功次数_, 特征快照值{ (I64)0 }, now, 调用点);
        (void)记录方法虚拟存在特征(方法首节点, 特征_失败次数_, 特征快照值{ (I64)0 }, now, 调用点);
        (void)记录方法虚拟存在特征(方法首节点, 特征_最近错误码_, 特征快照值{ (I64)0 }, now, 调用点);
        (void)记录方法虚拟存在特征(方法首节点, 特征_最近成功标记_, 特征快照值{ (I64)0 }, now, 调用点);
        (void)记录方法虚拟存在特征(方法首节点, 特征_方法最后调用时间_, 特征快照值{ (I64)0 }, now, 调用点);
    }

    void 标记方法调用开始(
        方法节点类* 方法首节点,
        场景节点类* 输入场景 = nullptr,
        时间戳 now = 0,
        const std::string& 调用点 = "方法类::标记方法调用开始")
    {
        if (!方法首节点) return;
        if (now == 0) now = 结构体_时间戳::当前_微秒();

        初始化方法虚拟存在信息(方法首节点, now, 调用点);
        if (输入场景) 同步场景特征到方法虚拟存在(方法首节点, 输入场景, now, 调用点 + "/输入");
        (void)记录方法虚拟存在特征(方法首节点, 特征_方法运行状态_, 特征快照值{ (I64)1 }, now, 调用点);
        (void)记录方法虚拟存在特征(方法首节点, 特征_方法最后调用时间_, 特征快照值{ (I64)now }, now, 调用点);
    }

    void 标记方法调用结束(
        方法节点类* 方法首节点,
        场景节点类* 输出场景,
        bool 成功,
        std::int64_t 错误码 = 0,
        时间戳 now = 0,
        const std::string& 调用点 = "方法类::标记方法调用结束",
        std::int64_t 耗时微秒 = 0)
    {
        if (!方法首节点) return;
        if (now == 0) now = 结构体_时间戳::当前_微秒();

        if (输出场景) 同步场景特征到方法虚拟存在(方法首节点, 输出场景, now, 调用点 + "/输出");
        (void)记录方法虚拟存在特征(方法首节点, 特征_方法运行状态_, 特征快照值{ 成功 ? (I64)2 : (I64)3 }, now, 调用点);
        (void)记录方法虚拟存在特征(方法首节点, 特征_方法最后调用时间_, 特征快照值{ (I64)now }, now, 调用点);
        写入_运行证据_到节点虚拟存在(方法首节点, nullptr, nullptr, 成功, 错误码, 耗时微秒, 0, now, 调用点);
    }
    场景节点类* 创建方法失败结果场景(
        方法节点类* 方法首节点,
        时间戳 now = 0,
        const std::string& 调用点 = "方法类::创建方法失败结果场景")
    {
        if (!方法首节点) return nullptr;
        if (now == 0) now = 结构体_时间戳::当前_微秒();

        auto* 方法存在 = 取或创建_方法虚拟存在(方法首节点, now, 调用点);
        auto* 内部世界 = 世界树.取内部世界();
        if (!方法存在 || !内部世界) return nullptr;

        auto* smi = new 场景节点主信息类();
        smi->名称 = 语素集.添加词性词(std::string("场景_方法失败结果_") + std::to_string(now), "名词");
        auto* 结果场景 = 世界树.创建场景(内部世界, smi, 调用点 + "/结果场景");
        if (!结果场景) return nullptr;

        auto* 快照类型 = 语素集.添加词性词("存在_方法失败结果快照", "名词");
        auto* 快照存在 = 世界树.取或创建子存在_按类型(结果场景, 快照类型, now, 调用点 + "/快照存在");
        if (!快照存在) return 结果场景;

        if (auto* vmi = dynamic_cast<存在节点主信息类*>(方法存在->主信息)) {
            if (vmi->名称) (void)世界树.写入名称(快照存在, vmi->名称, now, 调用点 + "/名称");
        }

        if (方法存在->子) {
            auto* first = static_cast<基础信息节点类*>(方法存在->子);
            auto* cur = first;
            do {
                auto* fmi = cur && cur->主信息 ? dynamic_cast<特征节点主信息类*>(cur->主信息) : nullptr;
                if (fmi && fmi->类型 && !std::holds_alternative<std::monostate>(fmi->当前快照)) {
                    if (auto* pv = std::get_if<I64>(&fmi->当前快照)) {
                        (void)世界树.写入特征_I64(快照存在, fmi->类型, *pv, {}, 调用点 + "/复制I64");
                    }
                    else if (auto* pp = std::get_if<指针句柄>(&fmi->当前快照)) {
                        (void)世界树.写入特征_指针(快照存在, fmi->类型, pp->指针, {}, 调用点 + "/复制指针");
                    }
                }
                cur = cur ? static_cast<基础信息节点类*>(cur->下) : nullptr;
            } while (cur && cur != first);
        }

        return 结果场景;
    }

    void 吸收结果节点摘要到影响签名(
        方法节点类* 方法首节点,
        方法节点类* 结果节点,
        时间戳 now = 0,
        const std::string& 调用点 = "方法类::吸收结果节点摘要到影响签名")
    {
        if (!方法首节点 || !结果节点 || !方法首节点->主信息) return;
        if (now == 0) now = 结构体_时间戳::当前_微秒();

        auto* hmi = 方法模块_detail::MI首(方法首节点);
        auto* rmi = 方法模块_detail::MI果(结果节点);
        if (!hmi || !rmi) return;

        std::unordered_set<std::string> seen;
        seen.reserve(hmi->影响度量签名.size() + rmi->结果摘要索引.size());
        for (const auto& s : hmi->影响度量签名) {
            if (!s.empty()) seen.insert(s);
        }

        bool changed = false;
        for (const auto& sec : rmi->结果摘要索引) {
            auto* mi = sec.主信息 ? dynamic_cast<二次特征主信息类*>(sec.主信息) : nullptr;
            if (!mi || mi->度量签名_链键.empty()) continue;
            if (seen.insert(mi->度量签名_链键).second) {
                hmi->影响度量签名.push_back(mi->度量签名_链键);
                changed = true;
            }
        }

        if (changed) {
            (void)记录方法虚拟存在特征(方法首节点, 特征_方法影响签名数量_, 特征快照值{ (I64)hmi->影响度量签名.size() }, now, 调用点);
        }
    }
    std::vector<结构_方法参数尝试候选> 生成参数尝试候选(
        方法节点类* 方法首节点,
        const std::vector<状态节点类*>& 条件状态列表,
        状态节点类* 目标结果状态,
        std::size_t 最多数量 = 8,
        const std::string& 调用点 = "方法类::生成参数尝试候选") const
    {
        using namespace 方法模块_detail;
        std::vector<结构_方法参数尝试候选> out;
        if (!方法首节点 || !目标结果状态) return out;

        auto* hmi = MI首(方法首节点);
        auto* 结果主信息 = (目标结果状态 && 目标结果状态->主信息) ? dynamic_cast<状态节点主信息类*>(目标结果状态->主信息) : nullptr;
        const auto 影响候选 = 世界树.批量分析条件影响结果(条件状态列表, 目标结果状态, 调用点);
        out.reserve(std::min<std::size_t>(最多数量, 影响候选.size() + 1));

        for (const auto& x : 影响候选) {
            if (!x.条件状态 || !x.条件状态->主信息) continue;
            auto* 条件主信息 = dynamic_cast<状态节点主信息类*>(x.条件状态->主信息);
            if (!条件主信息 || !条件主信息->状态特征) continue;

            结构_方法参数尝试候选 候选{};
            候选.方法首节点 = 方法首节点;
            候选.条件状态 = x.条件状态;
            候选.目标结果状态 = 目标结果状态;
            候选.参数主体 = 条件主信息->状态主体;
            候选.参数特征 = 条件主信息->状态特征;
            候选.建议值 = 条件主信息->状态值;
            候选.证据签名 = x.影响度量签名;
            候选.评分Q = x.影响评分Q;
            候选.调整方向 = x.结果偏移方向;
            候选.来自二次特征 = true;

            if (hmi && !hmi->影响度量签名.empty()) {
                const auto hit = 签名重合数_(hmi->影响度量签名, x.影响度量签名);
                候选.评分Q = std::min<std::int64_t>(10000, 候选.评分Q + static_cast<std::int64_t>(hit) * 800);
            }

            if (std::holds_alternative<I64>(条件主信息->状态值)) {
                const I64 center = std::get<I64>(条件主信息->状态值);
                I64 radius = 1 + std::max<I64>(0, (10000 - 候选.评分Q) / 1200);
                if (x.关系.有值变化量) radius += std::min<I64>(8, x.关系.值变化绝对量);
                候选.建议值域 = I64区间{ center - radius, center + radius };
            }

            if (结果主信息 && std::holds_alternative<I64>(结果主信息->状态值)) {
                const I64 target = std::get<I64>(结果主信息->状态值);
                const I64 radius = 1 + std::max<I64>(0, (10000 - 候选.评分Q) / 1500);
                候选.预期结果区间 = I64区间{ target - radius, target + radius };
            }

            out.push_back(std::move(候选));
            if (out.size() >= 最多数量) break;
        }

        if (out.empty()) {
            结构_方法参数尝试候选 候选{};
            候选.方法首节点 = 方法首节点;
            候选.目标结果状态 = 目标结果状态;
            候选.评分Q = 500;
            候选.需要随机探索 = true;
            out.push_back(std::move(候选));
        }

        return out;
    }

    存在节点类* 私有_创建尝试学习参数副本存在_(
        场景节点类* parent,
        const 结构_方法参数尝试候选& 候选,
        std::size_t index,
        时间戳 now,
        const std::string& 调用点)
    {
        if (!parent) return nullptr;

        const auto* base = 候选.参数主体 && 候选.参数主体->主信息
            ? dynamic_cast<存在节点主信息类*>(候选.参数主体->主信息) : nullptr;
        const std::string nameText = std::string("尝试参数副本_") + std::to_string(index);
        auto* fallbackName = 语素集.添加词性词(nameText, "名词");

        auto* mi = new 存在节点主信息类();
        mi->类型 = const_cast<词性节点类*>(base && base->类型 ? base->类型 : 类型_存在_方法桶虚拟存在_);
        mi->名称 = const_cast<词性节点类*>(base && base->名称 ? base->名称 : fallbackName);
        return 世界树.添加子存在(parent, mi, now, 调用点);
    }

    void 私有_写快照_到存在特征_(
        存在节点类* e,
        const 词性节点类* featureType,
        const 特征快照值& v,
        时间戳 now,
        const std::string& 调用点)
    {
        if (!e || !featureType) return;
        锁调度器守卫 锁({
            锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点)
            });

        auto* f = 私有_取或创建_特征节点_按类型_(e, featureType, now, 调用点);
        if (!f) return;

        auto* fmi = dynamic_cast<特征节点主信息类*>(f->主信息);
        if (!fmi) return;
        fmi->当前快照 = v;
    }

    结构_尝试学习参数生成结果 生成尝试学习参数(
        方法节点类* 方法首节点,
        const std::vector<状态节点类*>& 条件状态列表,
        状态节点类* 目标结果状态,
        std::size_t 最多数量 = 8,
        时间戳 now = 0,
        const std::string& 调用点 = "方法类::生成尝试学习参数")
    {
        结构_尝试学习参数生成结果 out{};
        if (!方法首节点 || !目标结果状态) return out;

        if (now == 0) now = 结构体_时间戳::当前_微秒();
        私有_确保元词_();

        out.候选列表 = 生成参数尝试候选(方法首节点, 条件状态列表, 目标结果状态, 最多数量, 调用点);

        auto* 模板场景 = 取或创建_方法参数模板场景(方法首节点, now, 调用点);
        if (!模板场景) return out;

        const std::string sceneName = std::string("场景_方法尝试学习参数_") + std::to_string(now);
        auto* sceneWord = 语素集.添加词性词(sceneName, "名词");

        {
            锁调度器守卫 锁({
                锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点)
                });

            auto* smi = new 场景节点主信息类();
            smi->名称 = const_cast<词性节点类*>(sceneWord);
            out.参数场景 = static_cast<场景节点类*>(世界链.添加子节点_已加锁(模板场景, static_cast<基础信息基类*>(smi)));
        }

        if (!out.参数场景) return out;

        auto 写候选到参数场景 = [&](const 结构_方法参数尝试候选& 候选, const 特征快照值& 参数值, std::size_t& index) -> bool {
            if (!候选.参数特征) return false;
            auto* 参数副本 = 私有_创建尝试学习参数副本存在_(out.参数场景, 候选, index++, now, 调用点);
            if (!参数副本) return false;

            auto* 参数特征主信息 = 候选.参数特征->主信息 ? dynamic_cast<特征节点主信息类*>(候选.参数特征->主信息) : nullptr;
            const auto* 参数类型 = 参数特征主信息 ? 参数特征主信息->类型 : nullptr;
            if (!参数类型) return false;

            私有_写快照_到存在特征_(参数副本, 参数类型, 参数值, now, 调用点);
            auto* feat = 世界树.确保特征(参数副本, 参数类型, 参数类型, 调用点);
            状态集.创建内部状态(out.参数场景, 参数副本, feat, 参数值, 枚举_存在状态事件::创建, true, now);
            return true;
        };

        std::size_t index = 0;
        bool 已写入参数 = false;
        for (const auto& 候选 : out.候选列表) {
            if (候选.需要随机探索 || !候选.参数特征) continue;
            已写入参数 = 写候选到参数场景(候选, 候选.建议值, index) || 已写入参数;
        }

        if (!已写入参数) {
            std::unordered_map<const 词性节点类*, std::vector<I64>> 同类值表{};
            std::unordered_map<const 词性节点类*, 特征节点类*> 同类特征表{};
            for (auto* 状态 : 条件状态列表) {
                auto* 状态主信息 = (状态 && 状态->主信息) ? dynamic_cast<状态节点主信息类*>(状态->主信息) : nullptr;
                auto* 特征主信息 = (状态主信息 && 状态主信息->状态特征 && 状态主信息->状态特征->主信息)
                    ? dynamic_cast<特征节点主信息类*>(状态主信息->状态特征->主信息) : nullptr;
                const auto* 特征类型 = 特征主信息 ? 特征主信息->类型 : nullptr;
                if (!特征类型) continue;
                同类特征表[特征类型] = 状态主信息->状态特征;
                if (std::holds_alternative<I64>(状态主信息->状态值)) {
                    同类值表[特征类型].push_back(std::get<I64>(状态主信息->状态值));
                }
            }

            for (const auto& kv : 同类特征表) {
                const auto* 特征类型 = kv.first;
                auto* 特征节点 = kv.second;
                if (!特征类型 || !特征节点) continue;

                auto& 值列 = 同类值表[特征类型];
                I64 最小值 = 0;
                I64 最大值 = 2;
                if (!值列.empty()) {
                    auto mm = std::minmax_element(值列.begin(), 值列.end());
                    最小值 = *mm.first;
                    最大值 = *mm.second;
                    if (最小值 == 最大值) {
                        最小值 -= 1;
                        最大值 += 1;
                    }
                }
                const I64 中间值 = 最小值 + (最大值 - 最小值) / 2;

                结构_方法参数尝试候选 基础候选{};
                基础候选.方法首节点 = 方法首节点;
                基础候选.目标结果状态 = 目标结果状态;
                基础候选.参数特征 = 特征节点;
                基础候选.评分Q = 300;

                已写入参数 = 写候选到参数场景(基础候选, 特征快照值{ 最小值 }, index) || 已写入参数;
                已写入参数 = 写候选到参数场景(基础候选, 特征快照值{ 中间值 }, index) || 已写入参数;
                已写入参数 = 写候选到参数场景(基础候选, 特征快照值{ 最大值 }, index) || 已写入参数;
            }
        }

        return out;
    }

    方法节点类* 创建条件节点(
        方法节点类* 首节点,
        const std::vector<二次特征节点类>& 条件判定索引 = {},
        std::string 条件模板主键 = {},
        时间戳 时间起 = 0,
        时间戳 时间止 = 0,
        const std::string& 调用点 = "方法类::创建条件节点",
        const 场景节点类* 条件场景模板 = nullptr)
    {
        using namespace 数据仓库模块;
        using namespace 方法模块_detail;
        if (!首节点) return nullptr;

        锁调度器守卫 锁({
            锁请求::写(方法链.链表锁, 枚举_锁域::方法链, "方法链", 调用点)
            });

        const std::string 融合键 = !条件模板主键.empty()
            ? 条件模板主键
            : (条件场景模板 ? 私有_编码条件Sig_(生成条件签名_基础(条件场景模板, 96, 调用点)) : std::string{});

        if (首节点->子) {
            方法节点类* found = nullptr;
            遍历同层(static_cast<方法节点类*>(首节点->子), [&](方法节点类* ch) {
                auto* cmi = MI条(ch);
                if (!cmi) return;

                bool 可融合 = false;
                if (条件场景模板) {
                    可融合 = 私有_条件场景可融合_(&cmi->场景根节点, 条件场景模板);
                }
                if (!可融合 && !条件判定索引.empty()) {
                    可融合 = 私有_条件判定索引可融合_(cmi, 条件判定索引, 调用点);
                }
                if (!可融合 && !融合键.empty()) {
                    std::uint64_t 左sig = 0;
                    std::uint64_t 右sig = 0;
                    if (私有_解析条件Sig_(cmi->条件场景模板主键, 左sig) && 私有_解析条件Sig_(融合键, 右sig)) {
                        可融合 = (左sig == 右sig);
                    }
                    else {
                        可融合 = (cmi->条件场景模板主键 == 融合键);
                    }
                }
                if (可融合) found = ch;
                });

            if (found) {
                if (auto* cmi = MI条(found)) {
                    if (!融合键.empty() && cmi->条件场景模板主键.empty()) cmi->条件场景模板主键 = 融合键;
                    私有_吸收条件判定索引_已加锁(cmi, 条件判定索引, 调用点);
                    if (条件场景模板) 私有_融合场景树_已加锁(&cmi->场景根节点, 条件场景模板);
                    if (时间起 != 0 && (cmi->时间起 == 0 || 时间起 < cmi->时间起)) cmi->时间起 = 时间起;
                    if (时间止 != 0 && 时间止 > cmi->时间止) cmi->时间止 = 时间止;
                }
                标记索引脏_已加锁();
                return found;
            }
        }

        auto* mi = new 方法条件节点主信息类();
        mi->条件场景模板主键 = 融合键.empty() ? std::move(条件模板主键) : 融合键;
        mi->时间起 = 时间起;
        mi->时间止 = 时间止;
        私有_吸收条件判定索引_已加锁(mi, 条件判定索引, 调用点);
        if (条件场景模板) 私有_融合场景树_已加锁(&mi->场景根节点, 条件场景模板);

        auto* node = 方法链.添加子节点_已加锁(首节点, static_cast<方法信息基类*>(mi));
        标记索引脏_已加锁();
        return static_cast<方法节点类*>(node);
    }

    方法节点类* 创建结果节点(
        方法节点类* 父节点,
        const 场景节点类* 结果场景模板 = nullptr,
        bool 自动生成结果摘要 = true,
        const std::string& 调用点 = "方法类::创建结果节点")
    {
        using namespace 数据仓库模块;
        using namespace 方法模块_detail;
        if (!父节点) return nullptr;

        锁调度器守卫 锁({
            锁请求::写(方法链.链表锁, 枚举_锁域::方法链, "方法链", 调用点)
            });

        if (结果场景模板 && 父节点->子) {
            const std::uint64_t 结果sig = 生成结果签名_基础(结果场景模板, 96, 调用点);
            方法节点类* found = nullptr;
            遍历同层(static_cast<方法节点类*>(父节点->子), [&](方法节点类* ch) {
                auto* rmi = MI果(ch);
                if (!rmi) return;
                if (私有_结果Sig_从摘要_(rmi) == 结果sig) found = ch;
                });
            if (found) {
                if (auto* rmi = MI果(found)) {
                    私有_融合场景树_已加锁(&rmi->场景根节点, 结果场景模板);
                    if (自动生成结果摘要) 私有_生成结果摘要索引_已加锁(found, &rmi->场景根节点);
                }
                标记索引脏_已加锁();
                return found;
            }
        }

        auto* mi = new 方法结果节点主信息类();
        if (结果场景模板) 私有_融合场景树_已加锁(&mi->场景根节点, 结果场景模板);

        auto* node = static_cast<方法节点类*>(方法链.添加子节点_已加锁(父节点, static_cast<方法信息基类*>(mi)));

        if (自动生成结果摘要 && 结果场景模板) {
            私有_生成结果摘要索引_已加锁(node, &mi->场景根节点);
        }

        标记索引脏_已加锁();
        return node;
    }


    // =============================================================
    // 桶化建树：基础签名 / 条件桶 / 结果桶 / 证据写入（自组织/尝试学习专用）
    // -------------------------------------------------------------
    // 目的：把一次“实际运行结果”归档进方法树（条件桶->结果桶），并把证据写入桶虚拟存在。
    // 不在这里判定“是否适用需求”；适用性由任务实践 + 二次特征评估给出。
    //
    // 约束：
    //  - 签名只使用稳定二级键（存在类型key + 特征链键 ChainKey）
    //  - 不修改主信息定义模块：条件签名编码到 条件场景模板主键（condSig=<u64>）
    //  - 结果签名从 结果摘要索引 推导（ChainKey集合哈希）
    // =============================================================

    // 基础条件签名：从 输入场景 提取（存在类型key + 特征链键）-> FNV1a64
    std::uint64_t 生成条件签名_基础(
        const 场景节点类* 输入场景,
        std::size_t maxKeys = 96,
        const std::string& 调用点 = "方法类::生成条件签名_基础") const
    {
        (void)调用点;
        if (!输入场景) return 0ull;

        std::vector<std::string> keys;
        keys.reserve(64);
        私有_收集场景_稳定键_递归(输入场景, keys, maxKeys);

        if (keys.empty()) return 0ull;
        std::sort(keys.begin(), keys.end());
        keys.erase(std::unique(keys.begin(), keys.end()), keys.end());

        std::uint64_t h = 1469598103934665603ull;
        for (const auto& s : keys) h = Hash64_混入字符串(h, s);
        return h;
    }

    // 基础结果签名：从 输出场景 提取（稳定键）-> FNV1a64
    std::uint64_t 生成结果签名_基础(
        const 场景节点类* 输出场景,
        std::size_t maxKeys = 96,
        const std::string& 调用点 = "方法类::生成结果签名_基础") const
    {
        (void)调用点;
        if (!输出场景) return 0ull;

        std::vector<std::string> keys;
        keys.reserve(64);
        私有_收集场景_稳定键_递归(输出场景, keys, maxKeys);

        if (keys.empty()) return 0ull;
        std::sort(keys.begin(), keys.end());
        keys.erase(std::unique(keys.begin(), keys.end()), keys.end());

        std::uint64_t h = 1469598103934665603ull;
        for (const auto& s : keys) h = Hash64_混入字符串(h, s);
        return h;
    }

    // 取或创建：条件桶（condSig）
    方法节点类* 取或创建_条件节点_按签名(
        方法节点类* 首节点,
        std::uint64_t condSig,
        时间戳 now = 0,
        std::size_t 条件桶上限 = 64,
        const std::string& 调用点 = "方法类::取或创建_条件节点_按签名")
    {
        using namespace 方法模块_detail;
        if (!首节点) return nullptr;
        if (条件桶上限 < 1) 条件桶上限 = 1;

        if (condSig != 0 && 私有_统计条件桶数量_(首节点) >= 条件桶上限) condSig = 0;

        if (首节点->子) {
            方法节点类* found = nullptr;
            遍历同层(static_cast<方法节点类*>(首节点->子), [&](方法节点类* ch) {
                auto* cmi = MI条(ch);
                if (!cmi) return;

                std::uint64_t s = 0;
                if (!私有_解析条件Sig_(cmi->条件场景模板主键, s)) return;
                if (s == condSig) found = ch;
                });
            if (found) return found;
        }

        const std::string key = 私有_编码条件Sig_(condSig);
        return 创建条件节点(首节点, {}, key, now, 0, 调用点);
    }

    方法节点类* 取或创建_条件节点_按场景(
        方法节点类* 首节点,
        const 场景节点类* 条件场景模板,
        时间戳 now = 0,
        std::size_t 条件桶上限 = 64,
        const std::vector<二次特征节点类>& 条件判定索引 = {},
        const std::string& 调用点 = "方法类::取或创建_条件节点_按场景")
    {
        if (!首节点) return nullptr;

        const std::uint64_t condSig = 条件场景模板
            ? 生成条件签名_基础(条件场景模板, 96, 调用点)
            : 0ull;

        auto* node = 取或创建_条件节点_按签名(首节点, condSig, now, 条件桶上限, 调用点);
        if (!node) return nullptr;
        if (!条件场景模板 && 条件判定索引.empty()) return node;

        锁调度器守卫 锁({
            锁请求::写(方法链.链表锁, 枚举_锁域::方法链, "方法链", 调用点)
            });

        auto* cmi = 方法模块_detail::MI条(node);
        if (!cmi) return node;

        if (条件场景模板) {
            私有_融合场景树_已加锁(&cmi->场景根节点, 条件场景模板);
            if (cmi->条件场景模板主键.empty()) {
                cmi->条件场景模板主键 = 私有_编码条件Sig_(condSig);
            }
        }
        私有_吸收条件判定索引_已加锁(cmi, 条件判定索引, 调用点);
        if (now != 0 && (cmi->时间起 == 0 || now < cmi->时间起)) cmi->时间起 = now;
        if (now != 0 && now > cmi->时间止) cmi->时间止 = now;
        标记索引脏_已加锁();
        return node;
    }

    // 取或创建：结果桶（resSig），挂在条件桶下
    // - 结果节点的“签名”通过其 结果摘要索引 推导（ChainKey 集合哈希），避免新增字段
    方法节点类* 取或创建_结果节点_按签名(
        方法节点类* 条件节点,
        std::uint64_t resSig,
        const 场景节点类* 结果场景模板 = nullptr,
        时间戳 now = 0,
        std::size_t 结果桶上限 = 64,
        const std::string& 调用点 = "方法类::取或创建_结果节点_按签名")
    {
        using namespace 方法模块_detail;
        if (!条件节点) return nullptr;
        if (结果桶上限 < 1) 结果桶上限 = 1;

        if (resSig != 0 && 私有_统计结果桶数量_(条件节点) >= 结果桶上限) resSig = 0;

        if (条件节点->子) {
            方法节点类* found = nullptr;
            遍历同层(static_cast<方法节点类*>(条件节点->子), [&](方法节点类* ch) {
                auto* rmi = MI果(ch);
                if (!rmi) return;
                const std::uint64_t s = 私有_结果Sig_从摘要_(rmi);
                if (s == resSig) found = ch;
                });
            if (found) {
                if (结果场景模板) {
                    锁调度器守卫 锁({
                        锁请求::写(方法链.链表锁, 枚举_锁域::方法链, "方法链", 调用点)
                        });
                    if (auto* rmi = MI果(found)) {
                        私有_融合场景树_已加锁(&rmi->场景根节点, 结果场景模板);
                        私有_生成结果摘要索引_已加锁(found, &rmi->场景根节点);
                        标记索引脏_已加锁();
                    }
                }
                return found;
            }
        }

        // 创建新结果节点；若传入模板则生成摘要索引（用于后续桶匹配与 token 召回）
        return 创建结果节点(条件节点, 结果场景模板, /*自动生成结果摘要*/ true, 调用点); 
    }

    // 写入：运行证据（首/条件/结果 三层桶虚拟存在）
    // - 证据写入世界树：方法虚拟存在 -> 条件桶虚拟存在 -> 结果桶虚拟存在
    void 写入_运行证据_到节点虚拟存在(
        方法节点类* 方法首节点,
        方法节点类* 条件节点,
        方法节点类* 结果节点,
        bool 成功,
        std::int64_t 错误码 = 0,
        std::int64_t 耗时微秒 = 0,
        std::uint64_t 样本键 = 0,
        时间戳 now = 0,
        const std::string& 调用点 = "方法类::写入_运行证据_到节点虚拟存在")
    {
        (void)样本键;

        if (!方法首节点) return;
        私有_确保元词_();

        // 1) 确保首节点虚拟存在
        auto* headVE = 取或创建_方法虚拟存在(方法首节点, now, 调用点);
        if (!headVE) return;

        auto* 状态场景 = 取或创建_方法内部状态场景(方法首节点, now, 调用点);
        auto 写I64并记录 = [&](存在节点类* host, const 词性节点类* featType, I64 value) {
            if (!host || !featType) return;
            const auto oldValue = 世界树.读取特征快照(host, featType, 调用点);
            const bool changed = !方法模块_detail::快照相等_(oldValue, 特征快照值{ value });
            私有_写I64_到存在特征_(host, featType, value, now, 调用点);
            if (!状态场景) return;
            auto* feat = 世界树.确保特征(host, featType, featType, 调用点);
            状态集.记录内部特征状态(
                状态场景,
                host,
                feat,
                特征快照值{ value },
                oldValue.has_value() ? 枚举_存在状态事件::变化 : 枚举_存在状态事件::创建,
                changed,
                now,
                [](场景节点类* s, 状态节点类* n, 时间戳 ts, const std::string& cp) {
                    二次特征类::状态记录后刷新二次特征(s, n, ts, cp);
                },
                调用点);
        };
        auto 写U64并记录 = [&](存在节点类* host, const 词性节点类* featType, U64 value) {
            const U64 vmax = (U64)INT64_MAX;
            写I64并记录(host, featType, (I64)((value > vmax) ? vmax : value));
        };
        auto 累加I64并记录 = [&](存在节点类* host, const 词性节点类* featType, I64 delta) {
            const I64 cur = 私有_读I64_存在特征_(host, featType);
            写I64并记录(host, featType, cur + delta);
        };

        // 2) 条件桶虚拟存在
        std::uint64_t condSig = 0;
        if (条件节点) {
            if (auto* cmi = 方法模块_detail::MI条(条件节点)) {
                (void)私有_解析条件Sig_(cmi->条件场景模板主键, condSig);
            }
        }
        auto* condVE = 私有_取或创建_子桶虚拟存在_(headVE, "条件桶_", condSig, now, 调用点);
        if (condVE) {
            写U64并记录(condVE, 特征_桶签名_U64_, condSig);
            累加I64并记录(condVE, 特征_运行次数_, 1);
            累加I64并记录(condVE, 特征_样本数_, 1);
            if (成功) {
                累加I64并记录(condVE, 特征_成功次数_, 1);
                写I64并记录(condVE, 特征_最近错误码_, 0);
            }
            else {
                累加I64并记录(condVE, 特征_失败次数_, 1);
                写I64并记录(condVE, 特征_最近错误码_, 错误码);
            }
        }

        // 3) 结果桶虚拟存在（挂在 condVE 下）
        std::uint64_t resSig = 0;
        if (结果节点) {
            if (auto* rmi = 方法模块_detail::MI果(结果节点)) resSig = 私有_结果Sig_从摘要_(rmi);
        }
        if (condVE) {
            auto* resVE = 私有_取或创建_子桶虚拟存在_(condVE, "结果桶_", resSig, now, 调用点);
            if (resVE) {
                写U64并记录(resVE, 特征_桶签名_U64_, resSig);
                累加I64并记录(resVE, 特征_运行次数_, 1);
                累加I64并记录(resVE, 特征_样本数_, 1);
                if (成功) {
                    累加I64并记录(resVE, 特征_成功次数_, 1);
                    写I64并记录(resVE, 特征_最近错误码_, 0);
                }
                else {
                    累加I64并记录(resVE, 特征_失败次数_, 1);
                    写I64并记录(resVE, 特征_最近错误码_, 错误码);
                }
            }
        }

        // 4) 首节点总计（方便全局统计）
        累加I64并记录(headVE, 特征_运行次数_, 1);
        if (成功) 累加I64并记录(headVE, 特征_成功次数_, 1);
        else     累加I64并记录(headVE, 特征_失败次数_, 1);
        写I64并记录(headVE, 特征_最近错误码_, 成功 ? 0 : 错误码);
        写I64并记录(headVE, 特征_最近成功标记_, 成功 ? 1 : 0);
        if (耗时微秒 > 0) {
            const I64 本次耗时毫秒 = static_cast<I64>(std::max<std::int64_t>(1, (耗时微秒 + 999) / 1000));
            const I64 历史EMA = 私有_读I64_存在特征_(headVE, 特征_平均耗时_ms_EMA_);
            const I64 新EMA = (历史EMA > 0)
                ? static_cast<I64>((历史EMA * 4 + 本次耗时毫秒 + 2) / 5)
                : 本次耗时毫秒;
            写I64并记录(headVE, 特征_最近耗时_ms_, 本次耗时毫秒);
            写I64并记录(headVE, 特征_平均耗时_ms_EMA_, 新EMA);
        }
    }

    // =============================================================
        // 本能函数注册/调用（方法执行协议最小骨架）
        // =============================================================
    void 注册本能函数(std::uint64_t 本能ID, 本能函数 fn) {
        if (本能ID == 0) return;
        std::lock_guard<std::mutex> g(本能_mtx_);
        本能表_[本能ID] = std::move(fn);
    }

    bool 有本能函数(std::uint64_t 本能ID) const {
        std::lock_guard<std::mutex> g(本能_mtx_);
        return 本能表_.find(本能ID) != 本能表_.end();
    }

    结构_方法执行结果 调用本能函数(std::uint64_t 本能ID, 结构_方法上下文& ctx, std::atomic_bool& cancel) {
        本能函数 fn;
        {
            std::lock_guard<std::mutex> g(本能_mtx_);
            auto it = 本能表_.find(本能ID);
            if (it == 本能表_.end()) return {};
            fn = it->second;
        }

        if (ctx.now == 0) ctx.now = 结构体_时间戳::当前_微秒();
        const 时间戳 t0 = 结构体_时间戳::当前_微秒();
        auto r = fn(ctx, cancel);
        const 时间戳 t1 = 结构体_时间戳::当前_微秒();
        const std::int64_t 耗时微秒 = (t1 >= t0) ? static_cast<std::int64_t>(t1 - t0) : 0;

        if (ctx.方法首节点) {
            写入_运行证据_到节点虚拟存在(
                ctx.方法首节点,
                nullptr,
                nullptr,
                r.成功,
                r.成功 ? 0 : r.成功码,
                耗时微秒,
                0,
                t1,
                "方法类::调用本能函数/写运行证据");
        }
        return r;
    }

    // =============================================================
    // 查找：动作名 / 影响度量签名 / 结果token
    // =============================================================

    方法节点类* 查找首节点_按动作名(const 词性节点类* 动作名,
        const std::string& 调用点 = "方法类::查找首节点_按动作名")
    {
        using namespace 数据仓库模块;
        using namespace 方法模块_detail;
        if (!动作名) return nullptr;

        // 快路径：索引
        重建索引_如需(调用点);
        {
            std::shared_lock lk(idx_mtx_);
            auto it = idx_动作名到首_.find(动作名->获取主键());
            if (it != idx_动作名到首_.end() && !it->second.empty()) return it->second.front();
        }

        // 慢路径：扫描
        锁调度器守卫 锁({
            锁请求::读(方法链.链表锁, 枚举_锁域::方法链, "方法链", 调用点)
            });

        auto* root = 方法链.根指针;
        if (!root || !root->子) return nullptr;

        方法节点类* found = nullptr;
        const std::string key = 动作名->获取主键();
        遍历同层(static_cast<方法节点类*>(root->子), [&](方法节点类* head) {
            auto* hmi = MI首(head);
            if (!hmi || !hmi->动作名) return;
            if (hmi->动作名->获取主键() == key) { found = head; }
            });
        return found;
    }

    // 影响度量签名召回（粗筛）：输入是 vector<string>（需求/目标的签名集合）
    std::vector<方法节点类*> 召回_按影响度量签名(
        const std::vector<std::string>& 目标签名,
        std::size_t topK = 20,
        const std::string& 调用点 = "方法类::召回_按影响度量签名")
    {
        (void)调用点;
        重建索引_如需(调用点);

        std::unordered_map<方法节点类*, std::uint32_t> score;
        {
            std::shared_lock lk(idx_mtx_);
            for (const auto& s : 目标签名) {
                if (s.empty()) continue;
                auto it = idx_影响签名到首_.find(s);
                if (it == idx_影响签名到首_.end()) continue;
                for (auto* head : it->second) score[head] += 1;
            }
        }

        std::vector<std::pair<方法节点类*, std::uint32_t>> items;
        items.reserve(score.size());
        for (auto& kv : score) items.push_back(kv);
        std::sort(items.begin(), items.end(), [](auto& a, auto& b) { return a.second > b.second; });

        std::vector<方法节点类*> out;
        for (std::size_t i = 0; i < items.size() && out.size() < topK; ++i) out.push_back(items[i].first);
        return out;
    }

    // 结果token模糊查找：输入 token 集合，倒排取候选结果节点并评分
    std::vector<结构_方法候选> 模糊查找_按结果Token(
        const std::vector<std::string>& 目标Token,
        std::size_t topK = 20,
        std::uint32_t minHit = 1,
        const std::string& 调用点 = "方法类::模糊查找_按结果Token")
    {
        using namespace 方法模块_detail;
        if (目标Token.empty()) return {};

        重建索引_如需(调用点);

        // 候选结果节点 -> 命中数
        std::unordered_map<方法节点类*, std::uint32_t> hit;
        std::unordered_map<方法节点类*, std::vector<std::string>> hitTokens;

        std::unordered_set<std::string> uniq;
        uniq.reserve(目标Token.size());
        for (const auto& t : 目标Token) if (!t.empty()) uniq.insert(t);

        {
            std::shared_lock lk(idx_mtx_);
            for (const auto& t : uniq) {
                auto it = idx_Token到结果_.find(t);
                if (it == idx_Token到结果_.end()) continue;
                for (auto* res : it->second) {
                    hit[res] += 1;
                    hitTokens[res].push_back(t);
                }
            }
        }

        std::vector<结构_方法候选> out;
        out.reserve(hit.size());

        for (auto& kv : hit) {
            if (kv.second < minHit) continue;
            方法节点类* resNode = kv.first;
            方法节点类* head = nullptr;
            {
                std::shared_lock lk(idx_mtx_);
                auto it = idx_结果到首_.find(resNode);
                if (it != idx_结果到首_.end()) head = it->second;
            }
            if (!head) continue;

            结构_方法候选 c{};
            c.方法首节点 = head;
            c.匹配结果节点 = resNode;
            c.命中数量 = kv.second;
            c.目标数量 = (std::uint32_t)uniq.size();
            c.命中Token = hitTokens[resNode];

            // 评分：命中率Q
            c.评分Q = (std::int64_t)c.命中数量 * 10000 / std::max<std::uint32_t>(1, c.目标数量);
            out.push_back(std::move(c));
        }

        std::sort(out.begin(), out.end(), [](const auto& a, const auto& b) {
            if (a.评分Q != b.评分Q) return a.评分Q > b.评分Q;
            return a.命中数量 > b.命中数量;
            });

        if (out.size() > topK) out.resize(topK);
        return out;
    }

    // =============================================================
    // 主信息指针 -> 节点指针（给任务/步骤持有主信息指针时用）
    // =============================================================
    方法节点类* 查找方法节点_按主信息指针(const 方法信息基类* 主信息,
        const std::string& 调用点 = "方法类::查找方法节点_按主信息指针")
    {
        if (!主信息) return nullptr;
        重建索引_如需(调用点);
        {
            std::shared_lock lk(idx_mtx_);
            auto it = idx_主信息到节点_.find(主信息);
            if (it != idx_主信息到节点_.end()) return it->second;
        }
        return nullptr;
    }

    // =============================================================
    // 索引维护
    // =============================================================
    void 标记索引脏() {
        std::unique_lock lk(idx_mtx_);
        idx_dirty_ = true;
    }

    void 重建索引_如需(const std::string& 调用点 = "方法类::重建索引_如需") {
        (void)调用点;
        std::unique_lock lk(idx_mtx_);
        if (!idx_dirty_) return;
        lk.unlock();
        重建索引(调用点);
    }

    void 重建索引(const std::string& 调用点 = "方法类::重建索引") {
        using namespace 数据仓库模块;
        using namespace 方法模块_detail;

        // 先用读锁扫描方法链构建临时索引，再一次性交换，避免长时间占用 idx_mtx_
        std::unordered_map<std::string, std::vector<方法节点类*>> new_动作;
        std::unordered_map<std::string, std::vector<方法节点类*>> new_影响;
        std::unordered_map<std::string, std::vector<方法节点类*>> new_tok;
        std::unordered_map<方法节点类*, 方法节点类*> new_res2head;
        std::unordered_map<const 方法信息基类*, 方法节点类*> new_mi2node;

        {
            锁调度器守卫 锁({
                锁请求::读(方法链.链表锁, 枚举_锁域::方法链, "方法链", 调用点)
                });

            auto* root = 方法链.根指针;
            if (root && root->子) {
                遍历同层(static_cast<方法节点类*>(root->子), [&](方法节点类* head) {
                    auto* hmi = MI首(head);
                    if (!hmi) return;

                    // 全量：主信息指针 -> 节点指针
                    std::function<void(方法节点类*)> dfsAll = [&](方法节点类* node) {
                        if (!node || !node->主信息) return;
                        if (new_mi2node.find(node->主信息) == new_mi2node.end()) new_mi2node.emplace(node->主信息, node);
                        if (!node->子) return;
                        遍历同层(static_cast<方法节点类*>(node->子), [&](方法节点类* ch) { dfsAll(ch); });
                        };
                    dfsAll(head);

                    // 动作名索引
                    if (hmi->动作名) new_动作[hmi->动作名->获取主键()].push_back(head);

                    // 影响签名倒排
                    if (!hmi->影响度量签名.empty()) {
                        std::unordered_set<std::string> dedup;
                        dedup.reserve(hmi->影响度量签名.size());
                        for (const auto& s : hmi->影响度量签名) {
                            if (s.empty()) continue;
                            if (!dedup.insert(s).second) continue;
                            new_影响[s].push_back(head);
                        }
                    }

                    // 结果token倒排
                    遍历方法树_结果节点(head, [&](方法节点类* resNode) {
                        auto* rmi = MI果(resNode);
                        if (!rmi) return;
                        new_res2head[resNode] = head;

                        // 结果摘要索引 => token
                        for (const auto& sec : rmi->结果摘要索引) {
                            const std::string tok = 二次特征节点_转Token(sec);
                            if (tok.empty()) continue;
                            new_tok[tok].push_back(resNode);
                        }
                        });
                    });
            }
        }

        // 交换写入
        {
            std::unique_lock lk(idx_mtx_);
            idx_动作名到首_.swap(new_动作);
            idx_影响签名到首_.swap(new_影响);
            idx_Token到结果_.swap(new_tok);
            idx_结果到首_.swap(new_res2head);
            idx_主信息到节点_.swap(new_mi2node);
            idx_dirty_ = false;
        }
    }

private:


    // =============================================================
    // 桶化辅助：签名编码/解析 + 稳定键提取 + 结果摘要签名
    // =============================================================
    static inline std::string 私有_编码条件Sig_(std::uint64_t sig) {
        return std::string("condSig=") + std::to_string(sig);
    }

    static inline bool 私有_解析条件Sig_(const std::string& s, std::uint64_t& outSig) noexcept {
        outSig = 0;
        if (s.empty()) return false;
        constexpr const char* k = "condSig=";
        if (s.rfind(k, 0) != 0) return false;
        try {
            outSig = (std::uint64_t)std::stoull(s.substr(std::char_traits<char>::length(k)));
            return true;
        }
        catch (...) {
            outSig = 0;
            return false;
        }
    }

    // 从 结果摘要索引 推导结果签名：只用 ChainKey，忽略概念名，保证稳定
    static inline std::uint64_t 私有_结果Sig_从摘要_(const 方法结果节点主信息类* rmi) {
        if (!rmi) return 0ull;
        if (rmi->结果摘要索引.empty()) return 0ull;

        std::vector<std::string> keys;
        keys.reserve(rmi->结果摘要索引.size());
        for (const auto& sec : rmi->结果摘要索引) {
            auto* mi = dynamic_cast<const 二次特征主信息类*>(sec.主信息);
            if (!mi) continue;
            if (mi->度量签名_链键.empty()) continue;
            keys.push_back(mi->度量签名_链键);
        }
        if (keys.empty()) return 0ull;
        std::sort(keys.begin(), keys.end());
        keys.erase(std::unique(keys.begin(), keys.end()), keys.end());

        std::uint64_t h = 1469598103934665603ull;
        for (const auto& s : keys) h = Hash64_混入字符串(h, s);
        return h;
    }

    struct 结构_可比较条件项 {
        std::string 键{};
        I64 低值 = 0;
        I64 高值 = 0;
        I64 代表值 = 0;
    };

    static inline bool 私有_提取可比较项_(const 特征节点主信息类* mi, 结构_可比较条件项& out) {
        if (!mi || !mi->类型) return false;
        out.键 = mi->类型->获取主键();
        if (out.键.empty()) return false;

        if (auto* p = std::get_if<I64>(&mi->当前快照)) {
            out.低值 = *p;
            out.高值 = *p;
            out.代表值 = *p;
            return true;
        }

        if (mi->区间摘要 && mi->区间摘要->有效() && mi->区间语义 == 枚举_区间语义::值域 && mi->区间摘要->维度.size() == 1) {
            const auto& d = mi->区间摘要->维度[0];
            out.低值 = d.低值;
            out.高值 = d.高值;
            if (mi->区间代表点 && std::holds_alternative<I64>(*mi->区间代表点)) {
                out.代表值 = std::get<I64>(*mi->区间代表点);
            }
            else {
                out.代表值 = d.低值 + (d.高值 - d.低值) / 2;
            }
            return true;
        }
        return false;
    }

    void 私有_收集场景可比较项_递归(const 基础信息节点类* 根, std::vector<结构_可比较条件项>& out) const
    {
        if (!根 || !根->子) return;
        auto* first = static_cast<const 基础信息节点类*>(根->子);
        auto* cur = first;
        do {
            if (auto* fmi = 方法模块_detail::取特征主信息_(cur)) {
                结构_可比较条件项 item{};
                if (私有_提取可比较项_(fmi, item)) out.push_back(std::move(item));
            }
            if (cur->子) 私有_收集场景可比较项_递归(cur, out);
            cur = static_cast<const 基础信息节点类*>(cur->下);
        } while (cur && cur != first);
    }

    bool 私有_条件场景可融合_(const 场景节点类* 既有, const 场景节点类* 新场景) const
    {
        std::vector<结构_可比较条件项> 左项;
        std::vector<结构_可比较条件项> 右项;
        私有_收集场景可比较项_递归(既有, 左项);
        私有_收集场景可比较项_递归(新场景, 右项);
        if (左项.empty() || 右项.empty()) return false;
        if (左项.size() != 右项.size()) return false;

        auto makeMap = [](const std::vector<结构_可比较条件项>& xs) {
            std::unordered_map<std::string, 结构_可比较条件项> m;
            for (const auto& x : xs) if (!x.键.empty()) m[x.键] = x;
            return m;
        };

        const auto 左图 = makeMap(左项);
        const auto 右图 = makeMap(右项);
        if (左图.size() != 右图.size()) return false;

        for (const auto& kv : 右图) {
            auto it = 左图.find(kv.first);
            if (it == 左图.end()) return false;
            const auto& a = it->second;
            const auto& b = kv.second;
            const bool 相交 = !(a.高值 < b.低值 || b.高值 < a.低值);
            if (!相交) return false;
        }
        return true;
    }
    static inline bool 私有_区间相等_(const I64区间& a, const I64区间& b) noexcept {
        return a.低值 == b.低值 && a.高值 == b.高值;
    }

    static inline I64 私有_方向_从标量_(I64 v) noexcept {
        return v > 0 ? 1 : (v < 0 ? -1 : 0);
    }

    std::vector<std::string> 私有_条件组合Token_从判定索引_(
        const std::vector<二次特征节点类>& 条件判定索引,
        const std::string& 调用点) const
    {
        std::vector<std::string> out;
        out.reserve(条件判定索引.size());
        if (条件判定索引.empty()) return out;

        for (const auto& sec : 条件判定索引) {
            auto* mi = sec.主信息 ? dynamic_cast<const 二次特征主信息类*>(sec.主信息) : nullptr;
            if (!mi || mi->度量签名_链键.empty()) continue;

            I64区间 大区间{ mi->标量值, mi->标量值 };
            auto* 链根 = 二次特征类::获取或创建_度量链根(mi->度量签名_链键, mi->种类, 调用点);
            if (链根 && 链根->子) {
                方法模块_detail::遍历同层(static_cast<二次特征节点类*>(链根->子), [&](二次特征节点类* n) {
                    auto* nmi = n && n->主信息 ? dynamic_cast<二次特征主信息类*>(n->主信息) : nullptr;
                    if (!nmi) return;
                    if (nmi->区间层级 != 二次特征主信息类::枚举_区间层级::大区间) return;
                    if (mi->标量值 < nmi->标量区间.低值 || mi->标量值 > nmi->标量区间.高值) return;
                    大区间 = nmi->标量区间;
                    });
            }

            std::ostringstream oss;
            oss << mi->度量签名_链键
                << "|R=" << 大区间.低值 << "," << 大区间.高值
                << "|D=" << 私有_方向_从标量_(mi->标量值);
            out.push_back(oss.str());
        }

        std::sort(out.begin(), out.end());
        out.erase(std::unique(out.begin(), out.end()), out.end());
        return out;
    }

    bool 私有_条件判定索引可融合_(
        const 方法条件节点主信息类* 既有,
        const std::vector<二次特征节点类>& 新索引,
        const std::string& 调用点) const
    {
        if (!既有) return false;
        const auto 左 = 私有_条件组合Token_从判定索引_(既有->条件判定索引, 调用点);
        const auto 右 = 私有_条件组合Token_从判定索引_(新索引, 调用点);
        if (左.empty() || 右.empty()) return false;
        return 左 == 右;
    }

    void 私有_吸收条件判定索引_已加锁(
        方法条件节点主信息类* 目标,
        const std::vector<二次特征节点类>& 新索引,
        const std::string& 调用点)
    {
        if (!目标 || 新索引.empty()) return;
        auto 已有 = 私有_条件组合Token_从判定索引_(目标->条件判定索引, 调用点);
        std::unordered_set<std::string> seen(已有.begin(), 已有.end());
        for (const auto& sec : 新索引) {
            std::vector<二次特征节点类> 单项{ sec };
            auto token = 私有_条件组合Token_从判定索引_(单项, 调用点);
            if (token.empty()) continue;
            if (seen.insert(token.front()).second) {
                目标->条件判定索引.push_back(sec);
            }
        }
    }

    基础信息节点类* 私有_查找可融合子节点_已加锁(基础信息节点类* 父, const 基础信息节点类* 源) const
    {
        if (!父 || !源 || !父->子 || !源->主信息) return nullptr;

        auto* 源特征 = 方法模块_detail::取特征主信息_(源);
        auto* 源存在 = 方法模块_detail::取存在主信息_(源);
        auto* 源场景 = 方法模块_detail::取场景主信息_(源);
        if (!源特征 && !源存在 && !源场景) return nullptr;

        auto* first = static_cast<基础信息节点类*>(父->子);
        auto* cur = first;
        do {
            if (源特征) {
                auto* 目标特征 = 方法模块_detail::取特征主信息_(cur);
                if (目标特征 && 目标特征->类型 && 源特征->类型 && 目标特征->类型->获取主键() == 源特征->类型->获取主键()) {
                    return cur;
                }
            }
            else if (源存在) {
                auto* 目标存在 = 方法模块_detail::取存在主信息_(cur);
                if (目标存在 && 目标存在->类型 && 源存在->类型 && 目标存在->类型->获取主键() == 源存在->类型->获取主键()) {
                    if (目标存在->名称 && 源存在->名称 && 目标存在->名称->获取主键() != 源存在->名称->获取主键()) {
                        cur = static_cast<基础信息节点类*>(cur->下);
                        continue;
                    }
                    return cur;
                }
            }
            else if (源场景) {
                auto* 目标场景 = 方法模块_detail::取场景主信息_(cur);
                if (!目标场景) {
                    cur = static_cast<基础信息节点类*>(cur->下);
                    continue;
                }
                if (目标场景->名称 && 源场景->名称 && 目标场景->名称->获取主键() == 源场景->名称->获取主键()) {
                    return cur;
                }
                if (!源场景->名称 && 目标场景->类型 && 源场景->类型 && 目标场景->类型->获取主键() == 源场景->类型->获取主键()) {
                    return cur;
                }
            }
            cur = static_cast<基础信息节点类*>(cur->下);
        } while (cur && cur != first);
        return nullptr;
    }

    基础信息节点类* 私有_克隆模板子树_已加锁(const 基础信息节点类* 源) const
    {
        if (!源 || !源->主信息) return nullptr;
        auto* 节点 = new 基础信息节点类();

        if (auto* 源特征 = 方法模块_detail::取特征主信息_(源)) {
            auto* mi = new 特征节点主信息类();
            mi->名称 = 源特征->名称;
            mi->类型 = 源特征->类型;
            mi->基础信息基类::命中次数 = 源特征->基础信息基类::命中次数;
            mi->当前快照 = 源特征->当前快照;
            mi->当前稳态索引 = 源特征->当前稳态索引;
            mi->稳态表 = 源特征->稳态表;
            mi->命中次数 = 源特征->命中次数;
            mi->可信度 = 源特征->可信度;
            mi->区间摘要 = 源特征->区间摘要;
            mi->区间语义 = 源特征->区间语义;
            mi->区间代表点 = 源特征->区间代表点;
            mi->连续测量误差区间 = 源特征->连续测量误差区间;
            节点->主信息 = mi;
        }
        else if (auto* 源存在 = 方法模块_detail::取存在主信息_(源)) {
            auto* mi = new 存在节点主信息类();
            mi->名称 = 源存在->名称;
            mi->类型 = 源存在->类型;
            mi->基础信息基类::命中次数 = 源存在->基础信息基类::命中次数;
            mi->概念模板 = 源存在->概念模板;
            mi->内部世界 = 源存在->内部世界;
            mi->创建时间 = 源存在->创建时间;
            mi->最后观测时间 = 源存在->最后观测时间;
            mi->可信度 = 源存在->可信度;
            mi->连续命中帧 = 源存在->连续命中帧;
            mi->连续未命中帧 = 源存在->连续未命中帧;
            mi->有位置历史 = 源存在->有位置历史;
            mi->上次观测位置_mm = 源存在->上次观测位置_mm;
            mi->最近观测位置_mm = 源存在->最近观测位置_mm;
            mi->最近位移_mm = 源存在->最近位移_mm;
            mi->连续静止帧 = 源存在->连续静止帧;
            节点->主信息 = mi;
        }
        else if (auto* 源场景 = 方法模块_detail::取场景主信息_(源)) {
            auto* mi = new 场景节点主信息类();
            mi->名称 = 源场景->名称;
            mi->类型 = 源场景->类型;
            mi->基础信息基类::命中次数 = 源场景->基础信息基类::命中次数;
            mi->宿主存在 = 源场景->宿主存在;
            mi->世界类型 = 源场景->世界类型;
            mi->来源文章 = 源场景->来源文章;
            mi->宿主存在主键_存档 = 源场景->宿主存在主键_存档;
            mi->来源文章主键_存档 = 源场景->来源文章主键_存档;
            mi->创建时间 = 源场景->创建时间;
            mi->最后观测时间 = 源场景->最后观测时间;
            mi->状态列表 = 源场景->状态列表;
            mi->动态列表 = 源场景->动态列表;
            mi->关系列表 = 源场景->关系列表;
            节点->主信息 = mi;
        }
        else {
            return nullptr;
        }

        if (源->子) {
            auto* first = static_cast<基础信息节点类*>(源->子);
            auto* cur = first;
            do {
                if (auto* 子 = 私有_克隆模板子树_已加锁(cur)) {
                    方法模块_detail::挂子_仅同层环_(节点, 子);
                }
                cur = static_cast<基础信息节点类*>(cur->下);
            } while (cur && cur != first);
        }
        return 节点;
    }

    void 私有_融合场景树_已加锁(基础信息节点类* 目标根, const 基础信息节点类* 源根)
    {
        if (!目标根 || !源根 || !源根->子) return;

        auto* first = static_cast<基础信息节点类*>(源根->子);
        auto* cur = first;
        do {
            auto* 命中 = 私有_查找可融合子节点_已加锁(目标根, cur);
            if (!命中) {
                if (auto* 克隆 = 私有_克隆模板子树_已加锁(cur)) {
                    方法模块_detail::挂子_仅同层环_(目标根, 克隆);
                }
            }
            else if (auto* 源特征 = 方法模块_detail::取特征主信息_(cur)) {
                if (auto* 目标特征 = 方法模块_detail::取特征主信息_(命中)) {
                    if (!目标特征->类型) 目标特征->类型 = 源特征->类型;

                    结构_可比较条件项 src{};
                    结构_可比较条件项 dst{};
                    const bool srcComparable = 私有_提取可比较项_(源特征, src);
                    const bool dstComparable = 私有_提取可比较项_(目标特征, dst);
                    if (srcComparable || dstComparable) {
                        const I64 lo = srcComparable && dstComparable ? (std::min)(src.低值, dst.低值) : (srcComparable ? src.低值 : dst.低值);
                        const I64 hi = srcComparable && dstComparable ? (std::max)(src.高值, dst.高值) : (srcComparable ? src.高值 : dst.高值);
                        I64 rep = srcComparable ? src.代表值 : dst.代表值;
                        if (srcComparable && dstComparable) rep = (dst.代表值 + src.代表值) / 2;

                        I64多维区间 box{};
                        box.维度.push_back(I64区间{ lo, hi });
                        目标特征->区间摘要 = box;
                        目标特征->区间语义 = 枚举_区间语义::值域;
                        目标特征->区间代表点 = 特征快照值{ rep };
                        目标特征->当前快照 = 特征快照值{ rep };
                    }
                    else if (!std::holds_alternative<std::monostate>(源特征->当前快照) || std::holds_alternative<std::monostate>(目标特征->当前快照)) {
                        目标特征->当前快照 = 源特征->当前快照;
                    }
                }
            }
            else {
                if (auto* 源存在 = 方法模块_detail::取存在主信息_(cur)) {
                    if (auto* 目标存在 = 方法模块_detail::取存在主信息_(命中)) {
                        if (!目标存在->类型) 目标存在->类型 = 源存在->类型;
                        if (!目标存在->名称) 目标存在->名称 = 源存在->名称;
                    }
                }
                if (auto* 源场景 = 方法模块_detail::取场景主信息_(cur)) {
                    if (auto* 目标场景 = 方法模块_detail::取场景主信息_(命中)) {
                        if (!目标场景->类型) 目标场景->类型 = 源场景->类型;
                        if (!目标场景->名称) 目标场景->名称 = 源场景->名称;
                    }
                }
                私有_融合场景树_已加锁(命中, cur);
            }
            cur = static_cast<基础信息节点类*>(cur->下);
        } while (cur && cur != first);
    }
    // 统计：首节点下条件桶数量
    static inline std::size_t 私有_统计条件桶数量_(方法节点类* 首节点) {
        using namespace 方法模块_detail;
        if (!首节点 || !首节点->子) return 0;
        std::size_t n = 0;
        遍历同层(static_cast<方法节点类*>(首节点->子), [&](方法节点类* ch) {
            if (MI条(ch)) ++n;
            });
        return n;
    }

    // 统计：条件节点下结果桶数量
    static inline std::size_t 私有_统计结果桶数量_(方法节点类* 条件节点) {
        using namespace 方法模块_detail;
        if (!条件节点 || !条件节点->子) return 0;
        std::size_t n = 0;
        遍历同层(static_cast<方法节点类*>(条件节点->子), [&](方法节点类* ch) {
            if (MI果(ch)) ++n;
            });
        return n;
    }

    // 从场景递归提取“稳定键”：
    // - 存在类型 key：E:<typeKey>
    // - 特征链键 ChainKey：F:<chainKey>
    void 私有_收集场景_稳定键_递归(const 场景节点类* scene, std::vector<std::string>& out, std::size_t maxKeys) const
    {
        if (!scene || !scene->子) return;
        if (out.size() >= maxKeys) return;

        特征类 feat{};

        std::function<void(const 场景节点类*)> walk;
        walk = [&](const 场景节点类* sc) {
            if (!sc || !sc->子) return;
            if (out.size() >= maxKeys) return;

            方法模块_detail::遍历同层(static_cast<基础信息节点类*>(sc->子), [&](基础信息节点类* ch) {
                if (!ch || !ch->主信息) return;
                if (out.size() >= maxKeys) return;

                if (dynamic_cast<场景节点主信息类*>(ch->主信息)) {
                    walk(static_cast<const 场景节点类*>(ch));
                    return;
                }

                if (auto* emi = dynamic_cast<存在节点主信息类*>(ch->主信息)) {
                    if (emi->类型) out.push_back(std::string("E:") + emi->类型->获取主键());

                    auto* e = static_cast<const 存在节点类*>(ch);
                    if (e->子) {
                        方法模块_detail::遍历同层(static_cast<基础信息节点类*>(e->子), [&](基础信息节点类* fh) {
                            if (!fh || !fh->主信息) return;
                            if (!dynamic_cast<特征节点主信息类*>(fh->主信息)) return;
                            auto* f = static_cast<特征节点类*>(const_cast<基础信息节点类*>(fh));
                            const std::string chainKey = feat.生成度量签名(f);
                            if (!chainKey.empty()) out.push_back(std::string("F:") + chainKey);
                            });
                    }
                }
                });
            };

        walk(scene);
    }

    // =============================================================
    // 桶虚拟存在 / 特征写入（最小实现）
    // =============================================================

    // 取或创建：parent 下子桶虚拟存在（存在节点），名称为 prefix + sig
    存在节点类* 私有_取或创建_子桶虚拟存在_(
        存在节点类* parent,
        const char* prefix,
        std::uint64_t sig,
        时间戳 now,
        const std::string& 调用点)
    {
        if (!parent || !prefix) return nullptr;
        私有_确保元词_();

        const std::string nm = std::string(prefix) + std::to_string(sig);
        const 词性节点类* nameWord = 语素集.添加词性词(nm, "名词");
        const std::string nameKey = nameWord->获取主键();

        锁调度器守卫 锁({
            锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点)
            });

        // 查找已有
        if (parent->子) {
            auto* first = static_cast<基础信息节点类*>(parent->子);
            auto* it = first;
            do {
                auto* emi = it ? dynamic_cast<存在节点主信息类*>(it->主信息) : nullptr;
                if (emi && emi->名称 && emi->名称->获取主键() == nameKey) {
                    return static_cast<存在节点类*>(it);
                }
                it = static_cast<基础信息节点类*>(it->下);
            } while (it && it != first);
        }

        // 新建
        auto* mi = new 存在节点主信息类();
        mi->类型 = const_cast<词性节点类*>(类型_存在_方法桶虚拟存在_);
        mi->名称 = const_cast<词性节点类*>(nameWord);
        // mi->记录观测(now);

        return static_cast<存在节点类*>(
            世界链.添加子节点_已加锁(parent, static_cast<基础信息基类*>(mi))
            );
    }

    特征节点类* 私有_取或创建_特征节点_按类型_(
        存在节点类* e,
        const 词性节点类* featureType,
        时间戳 now,
        const std::string& 调用点)
    {
        (void)now;
        (void)调用点;
        if (!e || !featureType) return nullptr;
        const std::string key = featureType->获取主键();

        if (e->子) {
            auto* first = static_cast<基础信息节点类*>(e->子);
            auto* it = first;
            do {
                auto* fmi = it ? dynamic_cast<特征节点主信息类*>(it->主信息) : nullptr;
                if (fmi && fmi->类型 && fmi->类型->获取主键() == key) {
                    return static_cast<特征节点类*>(it);
                }
                it = static_cast<基础信息节点类*>(it->下);
            } while (it && it != first);
        }

        auto* mi = new 特征节点主信息类();
        mi->类型 = const_cast<词性节点类*>(featureType);
        // mi->记录观测(now);

        return static_cast<特征节点类*>(
            世界链.添加子节点_已加锁(e, static_cast<基础信息基类*>(mi))
            );
    }

    I64 私有_读I64_存在特征_(存在节点类* e, const 词性节点类* featureType) const
    {
        if (!e || !featureType || !e->子) return 0;
        锁调度器守卫 锁({
            锁请求::读(世界链.链表锁, 枚举_锁域::世界链, "世界链", "方法类::读I64")
                });
        const std::string key = featureType->获取主键();

        auto* first = static_cast<基础信息节点类*>(e->子);
        auto* it = first;
        do {
            auto* fmi = it ? dynamic_cast<特征节点主信息类*>(it->主信息) : nullptr;
            if (fmi && fmi->类型 && fmi->类型->获取主键() == key) {
                if (auto* p = std::get_if<I64>(&fmi->当前快照)) return *p;
                return 0;
            }
            it = static_cast<基础信息节点类*>(it->下);
        } while (it && it != first);
        return 0;
    }

    void 私有_写I64_到存在特征_(
        存在节点类* e,
        const 词性节点类* featureType,
        I64 v,
        时间戳 now,
        const std::string& 调用点)
    {
        if (!e || !featureType) return;
        锁调度器守卫 锁({
            锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点)
            });

        auto* f = 私有_取或创建_特征节点_按类型_(e, featureType, now, 调用点);
        if (!f) return;

        auto* fmi = dynamic_cast<特征节点主信息类*>(f->主信息);
        if (!fmi) return;
        fmi->当前快照 = v;
        // fmi->记录观测(now);
    }

    void 私有_写入_方法内部状态_(
        方法节点类* 方法首节点,
        存在节点类* 方法虚拟存在,
        时间戳 now,
        const std::string& 调用点)
    {
        if (!方法首节点 || !方法虚拟存在) return;

        auto* 场景 = 取或创建_方法内部状态场景(方法首节点, now, 调用点);
        if (!场景) return;

        const I64 runCount = 私有_读I64_存在特征_(方法虚拟存在, 特征_运行次数_);
        const I64 successMark = 私有_读I64_存在特征_(方法虚拟存在, 特征_最近成功标记_);

        auto* runFeat = 世界树.确保特征(方法虚拟存在, 特征_运行次数_, 特征_运行次数_, 调用点);
        auto* successFeat = 世界树.确保特征(方法虚拟存在, 特征_最近成功标记_, 特征_最近成功标记_, 调用点);

        状态集.创建内部状态(场景, 方法虚拟存在, runFeat, 特征快照值{ runCount }, 枚举_存在状态事件::变化, true, now);
        状态集.创建内部状态(场景, 方法虚拟存在, successFeat, 特征快照值{ successMark }, 枚举_存在状态事件::变化, true, now);
    }

    void 私有_写U64_到存在特征_(
        存在节点类* e,
        const 词性节点类* featureType,
        U64 v,
        时间戳 now,
        const std::string& 调用点)
    {
        if (!e || !featureType) return;
        锁调度器守卫 锁({
            锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点)
            });

        auto* f = 私有_取或创建_特征节点_按类型_(e, featureType, now, 调用点);
        if (!f) return;

        auto* fmi = dynamic_cast<特征节点主信息类*>(f->主信息);
        if (!fmi) return;
        const U64 vmax = (U64)INT64_MAX;
        fmi->当前快照 = (I64)((v > vmax) ? vmax : v);
        // fmi->记录观测(now);
    }

    void 私有_累加I64_存在特征_(
        存在节点类* e,
        const 词性节点类* featureType,
        I64 delta,
        时间戳 now,
        const std::string& 调用点)
    {
        if (!e || !featureType) return;

        锁调度器守卫 锁({
            锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点)
            });

        auto* f = 私有_取或创建_特征节点_按类型_(e, featureType, now, 调用点);
        if (!f) return;

        auto* fmi = dynamic_cast<特征节点主信息类*>(f->主信息);
        if (!fmi) return;

        I64 cur = 0;
        if (auto* p = std::get_if<I64>(&fmi->当前快照)) cur = *p;

        fmi->当前快照 = (I64)(cur + delta);
        // fmi->记录观测(now);
    }


    // idx_mtx_ 已持有写锁时使用
    void 标记索引脏_已加锁() noexcept {
        // 注意：这里在 方法链 写锁内调用，避免再拿 idx_mtx_ 导致锁顺序问题
        idx_dirty_ = true;
    }

    // =============================================================
    // 结果摘要生成（最小版本）
    // 从“结果场景模板”提取：存在->特征，生成 ChainKey 并可选命中概念刻度
    // =============================================================
    void 私有_生成结果摘要索引_已加锁(方法节点类* 结果节点, const 场景节点类* 结果场景模板) {
        using namespace 方法模块_detail;

        auto* rmi = MI果(结果节点);
        if (!rmi) return;

        // 先清空
        rmi->结果摘要索引.clear();

        // 轻量特征工具：仅用来生成链键
        特征类 feat{};

        // 遍历场景模板：递归进入子场景；遇到存在节点则遍历其子特征
        std::function<void(const 场景节点类*)> walkScene;
        walkScene = [&](const 场景节点类* scene) {
            if (!scene || !scene->子) return;

            遍历同层(static_cast<基础信息节点类*>(scene->子), [&](基础信息节点类* ch) {
                if (!ch || !ch->主信息) return;

                if (dynamic_cast<场景节点主信息类*>(ch->主信息)) {
                    walkScene(static_cast<const 场景节点类*>(ch));
                    return;
                }

                if (dynamic_cast<存在节点主信息类*>(ch->主信息)) {
                    auto* e = static_cast<const 存在节点类*>(ch);
                    if (!e->子) return;

                    遍历同层(static_cast<基础信息节点类*>(e->子), [&](基础信息节点类* fh) {
                        if (!fh || !fh->主信息) return;
                        if (!dynamic_cast<特征节点主信息类*>(fh->主信息)) return;

                        auto* f = static_cast<特征节点类*>(const_cast<基础信息节点类*>(fh));
                        const std::string chainKey = feat.生成度量签名(f);
                        if (chainKey.empty()) return;

                        // 创建摘要项主信息（池托管）
                        auto* smi = 新建摘要项MI_(chainKey);                        // 这里只保留稳定链键摘要，避免在方法链锁内再进入概念树写锁。
                        {
                            auto* fmi = dynamic_cast<特征节点主信息类*>(f->主信息);
                            if (fmi) {
                                if (auto* p = std::get_if<I64>(&fmi->当前快照)) {
                                    smi->标量值 = *p;
                                }
                            }
                        }

                        二次特征节点类 node{};
                        node.主信息 = static_cast<基础信息基类*>(smi);
                        rmi->结果摘要索引.push_back(node);
                        });
                }
                });
            };

        walkScene(结果场景模板);
    }

    二次特征主信息类* 新建摘要项MI_(const std::string& chainKey) {
        std::lock_guard<std::mutex> g(pool_mtx_);
        auto ptr = std::make_unique<二次特征主信息类>();
        auto* mi = ptr.get();
        mi->形态 = 二次特征主信息类::枚举_二次特征形态::实例_state;
        mi->种类 = 枚举_二次特征种类::特征比较;
        mi->度量签名_链键 = chainKey;
        mi->标量区间 = I64区间{ 0, -1 };
        mi->区间层级 = 二次特征主信息类::枚举_区间层级::未定义;
        mi->概念模板 = nullptr;
        mi->概念名称 = nullptr;
        mi->标量值 = 0;
        mi->置信度 = 0;
        mi->是否满足 = false;
        pool_.push_back(std::move(ptr));
        return mi;
    }

private:

    // 元函数缓存（序列化暂不考虑）
    mutable std::shared_mutex 元缓存_mtx_{};
    std::unordered_map<方法节点类*, 存在节点类*> cache_首到虚拟存在_{};
    std::unordered_map<方法节点类*, 场景节点类*> cache_首到参数模板场景_{};
    std::unordered_map<方法节点类*, 场景节点类*> cache_首到内部状态场景_{};

    // 元词（按需懒初始化）
    inline static const 词性节点类* 类型_存在_方法虚拟存在_ = nullptr;
    inline static const 词性节点类* 名称_存在_方法虚拟存在_ = nullptr;
    inline static const 词性节点类* 名称_场景_方法参数模板_ = nullptr;
    inline static const 词性节点类* 名称_场景_方法内部状态_ = nullptr;

    inline static const 词性节点类* 类型_存在_方法桶虚拟存在_ = nullptr;

    // 桶/证据特征类型
    inline static const 词性节点类* 特征_桶签名_U64_ = nullptr;
    inline static const 词性节点类* 特征_方法运行状态_ = nullptr;
    inline static const 词性节点类* 特征_方法最后调用时间_ = nullptr;
    inline static const 词性节点类* 特征_方法动作句柄类型_ = nullptr;
    inline static const 词性节点类* 特征_方法本能动作ID_ = nullptr;
    inline static const 词性节点类* 特征_方法沉淀来源_ = nullptr;
    inline static const 词性节点类* 特征_方法影响签名数量_ = nullptr;
    inline static const 词性节点类* 特征_运行次数_ = nullptr;
    inline static const 词性节点类* 特征_成功次数_ = nullptr;
    inline static const 词性节点类* 特征_失败次数_ = nullptr;
    inline static const 词性节点类* 特征_样本数_ = nullptr;
    inline static const 词性节点类* 特征_最近错误码_ = nullptr;
    inline static const 词性节点类* 特征_最近成功标记_ = nullptr;
    inline static const 词性节点类* 特征_最近耗时_ms_ = nullptr;
    inline static const 词性节点类* 特征_平均耗时_ms_EMA_ = nullptr;


    static void 私有_确保元词_()
    {
        // 这些词只用于内部索引与调试，不参与外部语义推理
        if (!类型_存在_方法虚拟存在_) 类型_存在_方法虚拟存在_ = 语素集.添加词性词("存在_方法虚拟存在", "名词");
        if (!名称_存在_方法虚拟存在_) 名称_存在_方法虚拟存在_ = 语素集.添加词性词("方法虚拟存在", "名词");
        if (!名称_场景_方法参数模板_) 名称_场景_方法参数模板_ = 语素集.添加词性词("场景_方法参数模板", "名词");
        if (!名称_场景_方法内部状态_) 名称_场景_方法内部状态_ = 语素集.添加词性词("场景_方法内部状态", "名词");
        if (!类型_存在_方法桶虚拟存在_) 类型_存在_方法桶虚拟存在_ = 语素集.添加词性词("存在_方法桶虚拟存在", "名词");

        // 桶/证据特征（写入到桶虚拟存在）
        if (!特征_桶签名_U64_) 特征_桶签名_U64_ = 语素集.添加词性词("桶_签名_U64", "名词");
        if (!特征_方法运行状态_) 特征_方法运行状态_ = 语素集.添加词性词("方法_运行状态", "名词");
        if (!特征_方法最后调用时间_) 特征_方法最后调用时间_ = 语素集.添加词性词("方法_最后调用时间", "名词");
        if (!特征_方法动作句柄类型_) 特征_方法动作句柄类型_ = 语素集.添加词性词("方法_动作句柄类型", "名词");
        if (!特征_方法本能动作ID_) 特征_方法本能动作ID_ = 语素集.添加词性词("方法_本能动作ID", "名词");
        if (!特征_方法沉淀来源_) 特征_方法沉淀来源_ = 语素集.添加词性词("方法_沉淀来源", "名词");
        if (!特征_方法影响签名数量_) 特征_方法影响签名数量_ = 语素集.添加词性词("方法_影响签名数量", "名词");
        if (!特征_运行次数_)   特征_运行次数_ = 语素集.添加词性词("节点_运行次数", "名词");
        if (!特征_成功次数_)   特征_成功次数_ = 语素集.添加词性词("节点_成功次数", "名词");
        if (!特征_失败次数_)   特征_失败次数_ = 语素集.添加词性词("节点_失败次数", "名词");
        if (!特征_样本数_)     特征_样本数_ = 语素集.添加词性词("节点_样本数", "名词");
        if (!特征_最近错误码_) 特征_最近错误码_ = 语素集.添加词性词("节点_最近错误码", "名词");
        if (!特征_最近成功标记_) 特征_最近成功标记_ = 语素集.添加词性词("节点_最近成功标记", "名词");
        if (!特征_最近耗时_ms_) 特征_最近耗时_ms_ = 语素集.添加词性词("节点_最近耗时_ms", "名词");
        if (!特征_平均耗时_ms_EMA_) 特征_平均耗时_ms_EMA_ = 语素集.添加词性词("节点_平均耗时_ms_EMA", "名词");
    }

    // 本能函数注册表
    mutable std::mutex 本能_mtx_{};
    std::unordered_map<std::uint64_t, 本能函数> 本能表_{};

    // 结果摘要 MI 池（托管主信息指针，避免悬空）
    mutable std::mutex pool_mtx_{};
    std::vector<std::unique_ptr<二次特征主信息类>> pool_{};

    // 索引（动作名/影响签名/token/主信息->节点）
    mutable std::shared_mutex idx_mtx_{};
    bool idx_dirty_ = true;

    std::unordered_map<std::string, std::vector<方法节点类*>> idx_动作名到首_{};
    std::unordered_map<std::string, std::vector<方法节点类*>> idx_影响签名到首_{};
    std::unordered_map<std::string, std::vector<方法节点类*>> idx_Token到结果_{};
    std::unordered_map<方法节点类*, 方法节点类*> idx_结果到首_{};
    std::unordered_map<const 方法信息基类*, 方法节点类*> idx_主信息到节点_{};
};























