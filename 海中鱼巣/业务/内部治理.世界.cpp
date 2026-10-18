#include "内部治理.世界.h"
#include "../领域/新_任务类.h"
#include "../核心/容错检查.h"
#include <algorithm>
#include <limits>
#include <map>
#include <set>
#include <stdexcept>

namespace 海中鱼巣 {
namespace {
bool 含节点(const std::vector<稳定编码>& 组, 稳定编码 节点) {
    return std::find(组.begin(), 组.end(), 节点) != 组.end();
}
void 核验挂接(新场景操作状态 状态) {
    if (状态 != 新场景操作状态::已完成 && 状态 != 新场景操作状态::无变化)
        throw std::runtime_error("governance scene attachment failed");
}
}

新_内部治理世界类::新_内部治理世界类(新_特征值类& 值, 概念_特征类& 特征概念,
    新_特征类& 特征, 概念_存在类& 存在概念, const 概念_动态类& 动态概念,
    新_状态类& 状态, 新_存在类& 存在, 新_动态类& 动态, const 新_场景类& 现实) noexcept
    : 状态服务_(状态), 动态服务_(动态), 现实场景_(现实),
      治理场景_(根场景节点_, 存在, 状态, 动态, 新场景树用途::内部治理),
      因果服务_(状态, 动态, 存在, 存在概念, 特征概念, 动态概念, 值, 治理场景_, this, this),
      反查服务_(因果服务_, 特征, 特征概念, 存在概念, 动态概念, 动态, 存在, 治理场景_, this) {}

bool 新_内部治理世界类::装配任务读取(const 新内部治理对象读取接口& 读取) noexcept {
    if (任务读取_) return 任务读取_ == &读取;
    if (有效(根场景节点_)) return false;
    任务读取_ = &读取;
    return true;
}

bool 新_内部治理世界类::初始化() noexcept {
    try {
        if (!任务读取_) return false;
        if (!治理场景_.初始化() || !因果服务_.初始化())
            throw std::runtime_error("governance dependencies initialization failed");
        const auto 根 = 治理场景_.建立根场景();
        核验挂接(根.状态);
        if (!根.场景节点 || *根.场景节点 != 根场景节点_
            || 现实场景_.是场景节点(根场景节点_))
            throw std::runtime_error("governance root is not independent");
        return true;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"内部治理世界初始化", L"独立根或共享依赖初始化异常。");
    }
}

std::optional<稳定编码> 新_内部治理世界类::获取根场景() const noexcept {
    return 治理场景_.获取根场景();
}

std::optional<新任务治理场景信息> 新_内部治理世界类::读取任务治理场景(
    const 新任务虚拟存在引用& 引用) const noexcept {
    try {
        if (!任务读取_ || !获取根场景()) throw std::runtime_error("task scene dependencies missing");
        const auto 场景 = 治理场景_.读取任务治理场景(引用.任务节点);
        if (!场景) return std::nullopt;
        const auto 语义 = 任务读取_->读取任务虚拟存在语义(引用);
        if (场景->根场景节点 != 根场景节点_ || 场景->任务引用 != 引用 || !语义
            || 语义->状态.引用 != 引用) throw std::runtime_error("task scene owner readback mismatch");
        return 场景;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取任务治理场景", L"任务场景强归属读取异常。");
    }
}

新任务治理场景发布结果 新_内部治理世界类::发布新任务治理场景(
    const 新任务虚拟存在引用& 引用, const 基础封存追加片段& 任务片段) noexcept {
    std::optional<新任务治理场景发布准备> 准备;
    bool 发布结果须保留 = false;
    try {
        if (!任务读取_ || !获取根场景()) throw std::runtime_error("task scene publisher is not initialized");
        准备 = 治理场景_.准备任务治理场景(根场景节点_, 引用);
        if (!准备) return {};
        发布结果须保留 = true;
        const auto 发布 = 全局基础数据集.原子追加封存片段组({任务片段, 准备->场景片段()});
        if (发布.状态 != 基础节点组追加状态::已完成) {
            if (!发布.片段回执组.empty()) throw std::runtime_error("zero publication carries receipts");
            发布结果须保留 = false;
            治理场景_.撤销未发布任务治理场景(*准备);
            return {};
        }
        if (发布.片段回执组.size() != 2
            || std::count_if(发布.片段回执组.begin(), 发布.片段回执组.end(),
                [&](const auto& 回执) { return 回执.对应片段(任务片段); }) != 1
            || std::count_if(发布.片段回执组.begin(), 发布.片段回执组.end(),
                [&](const auto& 回执) { return 回执.对应片段(准备->场景片段()); }) != 1)
            throw std::runtime_error("task/scene publication receipt mismatch");
        const auto 场景 = 读取任务治理场景(引用);
        const auto 参与者 = 场景 ? 读取治理参与者(场景->场景节点, 引用.虚拟存在节点) : std::nullopt;
        if (!场景 || 场景->场景节点 != 准备->候选场景节点() || !参与者 || 参与者->任务引用 != 引用)
            throw std::runtime_error("published task/scene readback failed");
        return {新任务治理场景发布工程状态::已发布, 场景};
    } catch (...) {
        if (发布结果须保留)
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"发布任务治理场景", L"发表结果未知或发表后读回异常，保留任务和场景事实。");
        if (准备) 治理场景_.撤销未发布任务治理场景(*准备);
        return {}; // 明确未发表；task owner清理自己的候选后统一工程终止。
    }
}

std::optional<新治理场景参与者> 新_内部治理世界类::读取治理参与者(
    稳定编码 场景, 稳定编码 节点) const noexcept {
    if (!任务读取_) return std::nullopt;
    return 治理场景_.读取治理参与者(场景, 节点, *任务读取_);
}

void 新_内部治理世界类::迁移已有任务治理场景(
    const std::vector<新任务虚拟存在引用>& 引用组) noexcept {
    try {
        if (!任务读取_ || !获取根场景()) throw std::runtime_error("migration dependencies missing");
        std::map<稳定编码, 新任务虚拟存在语义读回> 任务快照;
        std::map<稳定编码, 新任务治理场景信息> 任务场景;
        std::set<稳定编码> 已有场景;
        for (const auto& 引用 : 引用组) {
            const auto 语义 = 任务读取_->读取任务虚拟存在语义(引用);
            if (!语义 || 语义->状态.引用 != 引用 || !任务快照.emplace(引用.任务节点, *语义).second)
                throw std::runtime_error("migration task set is incomplete or duplicated");
            if (const auto 场景 = 读取任务治理场景(引用)) {
                if (!已有场景.insert(场景->场景节点).second)
                    throw std::runtime_error("migration scene has multiple task owners");
                任务场景.emplace(引用.任务节点, *场景);
            }
        }
        const auto 根 = 治理场景_.获取场景(根场景节点_);
        if (!根 || std::set<稳定编码>(根->直接子场景组.begin(), 根->直接子场景组.end()) != 已有场景
            || 根->直接子场景组.size() != 已有场景.size())
            throw std::runtime_error("governance root contains orphan or unregistered task scenes");

        std::map<稳定编码, 新场景信息> 场景快照;
        std::map<std::pair<稳定编码, 稳定编码>, 新治理场景参与者> 参与者快照;
        std::map<稳定编码, 新状态信息> 状态快照;
        std::map<稳定编码, 新动态信息> 动态快照;
        struct 迁移包 {
            新任务虚拟存在引用 目标;
            新治理任务变化结果 变化;
            新治理场景参与者 主体;
            稳定编码 来源动作;
        };
        std::vector<迁移包> 包组;
        std::map<稳定编码, 稳定编码> 动作所属场景;
        auto 待核验场景 = 已有场景;
        待核验场景.insert(根场景节点_);
        const auto 正式动作定义 = 任务读取_->获取任务已结算动作定义();
        for (const auto c : 待核验场景) {
            const auto 场景 = 治理场景_.获取场景(c);
            if (!场景 || (c != 根场景节点_ && !场景->直接子场景组.empty()))
                throw std::runtime_error("migration scene structure unavailable");
            场景快照.emplace(c, *场景);
            for (const auto e : 治理场景_.查询治理参与节点(c)) {
                const auto 参与者 = 读取治理参与者(c, e);
                if (!参与者 || !参与者快照.emplace(std::make_pair(c, e), *参与者).second)
                    throw std::runtime_error("migration participant unavailable or duplicated");
                if (参与者->种类 == 新治理参与者种类::任务虚拟存在) {
                    if (!参与者->任务引用 || !任务快照.contains(参与者->任务引用->任务节点)
                        || 任务快照.at(参与者->任务引用->任务节点).状态.引用 != *参与者->任务引用
                        || (c != 根场景节点_ && 任务场景.at(参与者->任务引用->任务节点).场景节点 != c))
                        throw std::runtime_error("migration participant task is not in complete task set");
                } else if (参与者->种类 != 新治理参与者种类::现实执行主体引用
                    || !参与者->现实来源场景节点
                    || !现实场景_.存在属于场景范围(*参与者->现实来源场景节点, e))
                    throw std::runtime_error("migration real actor source unavailable");
            }
            for (const auto s : 场景->状态节点组) {
                const auto 状态 = 状态服务_.获取状态(s);
                if (!状态 || !状态快照.emplace(s, *状态).second)
                    throw std::runtime_error("migration state is missing or attached more than once");
            }
            for (const auto d : 场景->动态节点组) {
                const auto 动态 = 动态服务_.获取动态(d);
                if (!动态 || !动态快照.emplace(d, *动态).second)
                    throw std::runtime_error("migration dynamic is missing or attached more than once");
            }
            std::set<稳定编码> 包状态, 包动态;
            for (const auto d : 场景->动态节点组) {
                const auto& 动作 = 动态快照.at(d);
                if (动作.种类 != 新动态种类::动作) continue;
                if (!动作.动作目标任务虚拟存在 || !动作.动作来源节点 || !动作.动态概念节点
                    || !正式动作定义 || 动作.动作定义节点 != 正式动作定义
                    || !任务快照.contains(动作.动作目标任务虚拟存在->任务节点)
                    || 任务快照.at(动作.动作目标任务虚拟存在->任务节点).状态.引用 != *动作.动作目标任务虚拟存在)
                    throw std::runtime_error("legacy action has no reconstructible task/definition/concept");
                const auto 变化 = 读取场景内任务状态变化(*动作.动作目标任务虚拟存在, *动作.动作来源节点, c);
                const auto 主体 = 读取治理参与者(c, 动作.主题存在节点);
                const auto 来源 = 动态服务_.获取动态(*动作.动作来源节点);
                const auto 现实 = 主体 && 主体->现实来源场景节点
                    ? 现实场景_.获取场景(*主体->现实来源场景节点) : std::nullopt;
                if (!变化 || 变化->动作节点 != d || !来源 || !主体 || !现实
                    || !含节点(现实->动态节点组, 来源->节点)
                    || 来源->种类 != 新动态种类::动作 || 来源->动作目标种类 != 新动作目标种类::普通存在
                    || 来源->动作来源节点 != 动作.动作目标任务虚拟存在->任务节点
                    || 来源->主题存在节点 != 动作.主题存在节点 || !来源->动作时间
                    || 来源->动作时间->语义 != 新状态时间语义::实例绝对UTC
                    || !动作.动作时间 || 动作.动作时间->语义 != 新状态时间语义::实例绝对UTC
                    || 来源->动作时间->纳秒 >= 动作.动作时间->纳秒
                    || !动态快照.at(变化->变化动态节点).动态概念节点)
                    throw std::runtime_error("legacy settlement action source/time/concept cannot be reconstructed");
                for (const auto s : {变化->前状态节点, 变化->后状态节点})
                    if (!包状态.insert(s).second) throw std::runtime_error("legacy state belongs to multiple action packages");
                for (const auto n : {变化->动作节点, 变化->变化动态节点})
                    if (!包动态.insert(n).second) throw std::runtime_error("legacy dynamic belongs to multiple action packages");
                动作所属场景.emplace(d, c);
                包组.push_back({*动作.动作目标任务虚拟存在, *变化, *主体, *动作.动作来源节点});
            }
            if (包状态 != std::set<稳定编码>(场景->状态节点组.begin(), 场景->状态节点组.end())
                || 包动态 != std::set<稳定编码>(场景->动态节点组.begin(), 场景->动态节点组.end()))
                throw std::runtime_error("legacy scene contains incomplete or unsupported fact packages");
        }

        // 旧因果仍保留原发生场景及计数；先证明全部原参与者和发生身份可读。
        const auto 因果范围 = 因果服务_.查询当前场景树因果();
        std::map<稳定编码, 新因果信息> 因果快照;
        std::map<稳定编码, 新因果共同抽象结果> 概念因果快照;
        for (const auto q : 因果范围.具体因果节点组) {
            const auto 信息 = 因果服务_.获取因果(q);
            if (!信息 || !信息->发生场景节点 || !场景快照.contains(*信息->发生场景节点))
                throw std::runtime_error("legacy causal occurrence scope cannot be reconstructed");
            const auto c = *信息->发生场景节点;
            const auto 核验参与 = [&](稳定编码 e) {
                if (!参与者快照.contains(std::make_pair(c, e)))
                    throw std::runtime_error("legacy causal participant cannot be reconstructed");
            };
            核验参与(信息->定义.结果状态.被描述存在节点);
            for (const auto& s : 信息->定义.条件状态组) 核验参与(s.被描述存在节点);
            for (const auto& d : 信息->定义.条件动态组) 核验参与(d.主题存在节点);
            if (信息->定义.动作) {
                const auto& a = *信息->定义.动作;
                核验参与(a.执行主体存在节点);
                if (a.作用部位存在节点) 核验参与(*a.作用部位存在节点);
                if (a.作用对象存在节点) 核验参与(*a.作用对象存在节点);
                if (a.作用对象任务虚拟存在) {
                    核验参与(a.作用对象任务虚拟存在->虚拟存在节点);
                    if (参与者快照.at({c, a.作用对象任务虚拟存在->虚拟存在节点}).任务引用 != a.作用对象任务虚拟存在)
                        throw std::runtime_error("legacy causal target identity mismatch");
                }
            }
            for (const auto id : 信息->已计数发生身份组)
                if (!动作所属场景.contains(id) || (c != 根场景节点_ && 动作所属场景.at(id) != c))
                    throw std::runtime_error("legacy counted occurrence is outside original scene scope");
            因果快照.emplace(q, *信息);
        }
        for (const auto q : 因果范围.概念因果节点组) {
            const auto 信息 = 因果服务_.获取概念因果(q);
            if (!信息.信息) throw std::runtime_error("legacy conceptual causal information unavailable");
            概念因果快照.emplace(q, 信息);
        }

        // 此前均为只读预检。启动持task锁，逐个完整发布；不承诺全世界大事务。
        const auto 核验任务未变 = [&](const 新任务虚拟存在语义读回& 旧) {
            const auto 当前 = 任务读取_->读取任务虚拟存在语义(旧.状态.引用);
            if (!当前 || 当前->状态.引用 != 旧.状态.引用
                || 当前->状态.当前状态 != 旧.状态.当前状态
                || 当前->状态.状态特征类型概念节点 != 旧.状态.状态特征类型概念节点
                || 当前->专属存在概念节点 != 旧.专属存在概念节点)
                throw std::runtime_error("task changed during governance migration");
        };
        for (const auto& [t, 旧] : 任务快照) {
            核验任务未变(旧);
            if (!任务场景.contains(t)) {
                const auto 准备 = 治理场景_.准备任务治理场景(根场景节点_, 旧.状态.引用);
                if (!准备) throw std::runtime_error("legacy task scene preparation failed");
                // 调用开始后若抛出，状态未知，不撤销候选；外层保留现场终止。
                const auto 发布 = 全局基础数据集.原子追加封存片段组({准备->场景片段()});
                if (发布.状态 != 基础节点组追加状态::已完成) {
                    if (发布.片段回执组.empty()) 治理场景_.撤销未发布任务治理场景(*准备);
                    throw std::runtime_error("legacy task scene publication failed");
                }
                if (发布.片段回执组.size() != 1 || !发布.片段回执组.front().对应片段(准备->场景片段()))
                    throw std::runtime_error("legacy task scene receipt mismatch");
                const auto 场景 = 读取任务治理场景(旧.状态.引用);
                if (!场景 || 场景->场景节点 != 准备->候选场景节点())
                    throw std::runtime_error("legacy task scene publication readback failed");
                任务场景.emplace(t, *场景);
            }
            核验任务未变(旧);
        }
        for (const auto& 包 : 包组) {
            const auto c = 任务场景.at(包.目标.任务节点).场景节点;
            if (包.变化.治理场景节点 == 根场景节点_) {
                核验挂接(治理场景_.添加治理执行主体引用(c, 包.主体.参与节点,
                    *包.主体.现实来源场景节点, 现实场景_));
                核验挂接(治理场景_.原子迁移治理状态动态(根场景节点_, c,
                    {包.变化.前状态节点, 包.变化.后状态节点},
                    {包.变化.动作节点, 包.变化.变化动态节点}, *任务读取_));
            } else if (包.变化.治理场景节点 != c)
                throw std::runtime_error("migrated package is attached to another task scene");
            const auto 读回 = 读取任务状态变化(包.目标, 包.来源动作);
            if (!读回 || 读回->动作节点 != 包.变化.动作节点 || 读回->前状态节点 != 包.变化.前状态节点
                || 读回->后状态节点 != 包.变化.后状态节点 || 读回->变化动态节点 != 包.变化.变化动态节点)
                throw std::runtime_error("migrated action package readback mismatch");
        }
        for (const auto& [key, 旧] : 参与者快照)
            if (读取治理参与者(key.first, key.second) != 旧)
                throw std::runtime_error("migration modified existing participant projections");
        for (const auto& [s, 旧] : 状态快照)
            if (状态服务_.获取状态(s) != 旧) throw std::runtime_error("migration modified state identity/value/count");
        for (const auto& [d, 旧] : 动态快照)
            if (动态服务_.获取动态(d) != 旧) throw std::runtime_error("migration modified dynamic identity/value/count");
        if (因果服务_.查询当前场景树因果() != 因果范围)
            throw std::runtime_error("migration changed causal identities or scopes");
        for (const auto& [q, 旧] : 因果快照)
            if (因果服务_.获取因果(q) != 旧) throw std::runtime_error("migration changed causal definitions/statistics");
        for (const auto& [q, 旧] : 概念因果快照)
            if (因果服务_.获取概念因果(q) != 旧) throw std::runtime_error("migration changed conceptual causal knowledge");
    } catch (const std::exception& e) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"迁移任务治理场景",
            L"旧治理材料无法完整还原或发布读回异常；保留原事实及已完成的完整迁移包，停止启动。", e.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"迁移任务治理场景", L"未知工程异常；保留现场，停止启动。");
    }
}

std::optional<新治理场景参与者> 新_内部治理世界类::读取范围治理参与者(
    稳定编码 范围, 稳定编码 节点) const {
    std::vector<稳定编码> 待查{范围};
    std::set<稳定编码> 已查;
    std::optional<新治理场景参与者> 结果;
    while (!待查.empty()) {
        const auto c = 待查.back();
        待查.pop_back();
        if (!已查.insert(c).second) throw std::runtime_error("governance scene range contains a cycle or duplicate");
        const auto 场景 = 治理场景_.获取场景(c);
        if (!场景) throw std::runtime_error("governance scene range unreadable");
        if (auto 当前 = 读取治理参与者(c, 节点)) {
            当前->场景节点 = 范围; // 仅合并值式投影，公共本级读取的场景身份不变。
            if (结果 && *结果 != *当前) throw std::runtime_error("governance participant projections conflict across scenes");
            结果 = *当前;
        }
        待查.insert(待查.end(), 场景->直接子场景组.begin(), 场景->直接子场景组.end());
    }
    return 结果;
}

新治理动作登记结果 新_内部治理世界类::建立任务状态动作(
    const 新治理任务动作请求& 请求) noexcept {
    try {
        if (!任务读取_ || !获取根场景()) return {};
        const auto 正式定义 = 任务读取_->获取任务已结算动作定义();
        if (!正式定义 || *正式定义 != 请求.动作定义节点) return {};
        const auto 治理 = 读取任务治理场景(请求.目标);
        if (!治理) throw std::runtime_error("task has no published governance scene");
        const auto 本场景 = 治理->场景节点;
        const auto 目标 = 任务读取_->读取任务虚拟存在语义(请求.目标);
        const auto 前 = 状态服务_.获取状态(请求.前状态节点);
        const auto 来源 = 动态服务_.获取动态(请求.来源普通动作节点);
        if (!目标 || !前 || !来源 || 来源->种类 != 新动态种类::动作
            || 来源->动作目标种类 != 新动作目标种类::普通存在
            || 来源->动作来源节点 != 请求.目标.任务节点
            || 来源->主题存在节点 != 请求.执行主体节点 || !来源->动作时间
            || 请求.动作时间.语义 != 新状态时间语义::实例绝对UTC
            || 来源->动作时间->纳秒 >= 请求.动作时间.纳秒
            || 前->被描述存在节点 != 请求.目标.虚拟存在节点
            || 前->来源实例特征节点 != 请求.目标.状态特征节点
            || 前->强时间 != 请求.动作时间
            || !现实场景_.存在属于场景范围(请求.现实来源场景节点, 请求.执行主体节点)) return {};
        const auto 现实 = 现实场景_.获取场景(请求.现实来源场景节点);
        if (!现实 || !含节点(现实->动态节点组, 来源->节点)) return {};
        const auto 已有 = 动态服务_.按来源查询动作(请求.来源普通动作节点);
        if (已有.状态 != 新动态操作状态::已完成)
            throw std::runtime_error("governance action source lookup failed");
        if (已有.动作) {
            const auto& 动作 = *已有.动作;
            if (动作.动作目标任务虚拟存在 != 请求.目标 || 动作.动作定义节点 != 请求.动作定义节点
                || 动作.主题存在节点 != 请求.执行主体节点 || 动作.动作时间 != 请求.动作时间)
                return {};
            const auto 完整 = 读取任务状态变化(请求.目标, 请求.来源普通动作节点);
            if (!完整 || 完整->前状态节点 != 请求.前状态节点)
                throw std::runtime_error("partial governance action cannot be replayed");
            return {新治理动作登记状态::精确重复, 动作.节点};
        }
        if (目标->状态.当前状态 != static_cast<std::int64_t>(新任务状态::已完成)
            || 前->固定准确值 != 新特征准确值{目标->状态.当前状态}) return {};
        核验挂接(治理场景_.添加治理任务引用(本场景, 请求.目标, *任务读取_));
        核验挂接(治理场景_.添加治理执行主体引用(本场景, 请求.执行主体节点,
            请求.现实来源场景节点, 现实场景_));
        核验挂接(治理场景_.添加治理状态(本场景, 请求.前状态节点, *任务读取_));
        const auto 动作 = 动态服务_.建立内部治理动作动态({请求.执行主体节点,
            请求.动作定义节点, 请求.来源普通动作节点, 请求.目标, 请求.动作时间}, *任务读取_);
        if (动作.状态 != 新动态操作状态::已完成 || !动作.动态节点)
            throw std::runtime_error("governance action publication failed");
        核验挂接(治理场景_.添加治理动态(本场景, *动作.动态节点, *任务读取_));
        return {新治理动作登记状态::已建立, 动作.动态节点};
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"建立任务状态动作", L"治理动作或前状态组织异常。");
    }
}

新治理任务变化结果 新_内部治理世界类::完成任务状态变化(
    稳定编码 动作节点, 稳定编码 前节点, 稳定编码 后节点) noexcept {
    try {
        const auto 动作 = 动态服务_.获取动态(动作节点);
        const auto 前 = 状态服务_.获取状态(前节点);
        const auto 后 = 状态服务_.获取状态(后节点);
        if (!任务读取_ || !动作 || !动作->动作目标任务虚拟存在 || !动作->动作来源节点
            || !动作->动作时间 || !前 || !后) throw std::runtime_error("governance completion materials missing");
        const auto& 目标 = *动作->动作目标任务虚拟存在;
        const auto 治理 = 读取任务治理场景(目标);
        if (!治理) throw std::runtime_error("governance completion has no task scene");
        const auto 当前 = 任务读取_->读取任务虚拟存在语义(目标);
        if (!当前 || 前->被描述存在节点 != 目标.虚拟存在节点
            || 后->被描述存在节点 != 目标.虚拟存在节点
            || 前->来源实例特征节点 != 目标.状态特征节点 || 后->来源实例特征节点 != 目标.状态特征节点
            || 前->强时间 != *动作->动作时间 || 后->强时间.语义 != 新状态时间语义::实例绝对UTC
            || 后->强时间.纳秒 <= 动作->动作时间->纳秒
            || 后->固定准确值 != 新特征准确值{当前->状态.当前状态})
            throw std::runtime_error("governance completion identity/value/time mismatch");
        if (!动作->动作结果组.empty()) {
            const auto 完整 = 读取任务状态变化(目标, *动作->动作来源节点);
            if (!完整 || 完整->前状态节点 != 前节点 || 完整->后状态节点 != 后节点)
                throw std::runtime_error("governance completion replay conflicts");
            return *完整;
        }
        核验挂接(治理场景_.添加治理状态(治理->场景节点, 后节点, *任务读取_));
        const auto 变化 = 动态服务_.建立原子动态(目标.虚拟存在节点, 前节点, 后节点);
        if (变化.状态 != 新动态操作状态::已完成 || !变化.动态节点)
            throw std::runtime_error("governance atomic dynamic publication failed");
        const auto 关联 = 动态服务_.关联动作结果(动作节点, *变化.动态节点, 新动作结果关系类型::导致变化);
        if (关联 != 新动态操作状态::已完成) throw std::runtime_error("governance action result association failed");
        核验挂接(治理场景_.添加治理动态(治理->场景节点, *变化.动态节点, *任务读取_));
        const auto 完整 = 读取任务状态变化(目标, *动作->动作来源节点);
        if (!完整) throw std::runtime_error("governance completion readback failed");
        交付实际发生(*完整);
        return *完整;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"完成任务状态变化", L"已发生治理变化无法完整组织；不重做动作。");
    }
}

std::optional<新治理任务变化结果> 新_内部治理世界类::读取任务状态变化(
    const 新任务虚拟存在引用& 目标, 稳定编码 来源普通动作) const noexcept {
    const auto 场景 = 读取任务治理场景(目标);
    if (!场景) 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取任务状态变化", L"正式任务缺少治理场景。");
    return 读取场景内任务状态变化(目标, 来源普通动作, 场景->场景节点);
}

std::optional<新治理任务变化结果> 新_内部治理世界类::读取场景内任务状态变化(
    const 新任务虚拟存在引用& 目标, 稳定编码 来源普通动作, 稳定编码 本场景) const {
    try {
        if (!任务读取_ || !获取根场景()) return std::nullopt;
        const auto 来源 = 动态服务_.按来源查询动作(来源普通动作);
        if (来源.状态 != 新动态操作状态::已完成) throw std::runtime_error("governance source read failed");
        if (!来源.动作) return std::nullopt;
        const auto& 动作 = *来源.动作;
        const auto 当前 = 任务读取_->读取任务虚拟存在引用(目标);
        const auto 场景 = 治理场景_.获取场景(本场景);
        if (!当前 || 当前->当前状态 != static_cast<std::int64_t>(新任务状态::已结算)
            || !场景 || 动作.动作目标种类 != 新动作目标种类::任务虚拟存在
            || 动作.动作目标任务虚拟存在 != 目标 || !动作.动作时间
            || !动作.动作定义节点 || 动作.动作来源节点 != 来源普通动作
            || !含节点(场景->动态节点组, 动作.节点) || 动作.动作结果组.size() != 1
            || 动作.动作结果组.front().类型 != 新动作结果关系类型::导致变化)
            throw std::runtime_error("governance action readback mismatch");
        const auto 主体 = 读取治理参与者(本场景, 动作.主题存在节点);
        const auto 客体 = 读取治理参与者(本场景, 目标.虚拟存在节点);
        if (!主体 || 主体->种类 != 新治理参与者种类::现实执行主体引用
            || !客体 || 客体->任务引用 != 目标) throw std::runtime_error("governance actor/target mismatch");
        const auto 变化 = 动态服务_.获取动态(动作.动作结果组.front().结果动态节点);
        if (!变化 || 变化->种类 != 新动态种类::原子 || 变化->主题存在节点 != 目标.虚拟存在节点
            || 变化->状态节点组.size() != 2 || !含节点(场景->动态节点组, 变化->节点))
            throw std::runtime_error("governance result dynamic mismatch");
        const auto 前 = 状态服务_.获取状态(变化->状态节点组[0]);
        const auto 后 = 状态服务_.获取状态(变化->状态节点组[1]);
        // 首叶只组织任务owner的“已完成(9) -> 已结算(12)”真实变化。
        if (!前 || !后 || 前->被描述存在节点 != 目标.虚拟存在节点
            || 后->被描述存在节点 != 目标.虚拟存在节点
            || 前->来源实例特征节点 != 目标.状态特征节点 || 后->来源实例特征节点 != 目标.状态特征节点
            || 前->固定准确值 != 新特征准确值{static_cast<std::int64_t>(新任务状态::已完成)}
            || 后->固定准确值 != 新特征准确值{static_cast<std::int64_t>(新任务状态::已结算)}
            || 前->强时间 != *动作.动作时间 || 后->强时间.语义 != 新状态时间语义::实例绝对UTC
            || 后->强时间.纳秒 <= 动作.动作时间->纳秒
            || !含节点(场景->状态节点组, 前->节点) || !含节点(场景->状态节点组, 后->节点))
            throw std::runtime_error("governance before/after states mismatch");
        return 新治理任务变化结果{本场景, 动作.节点, 前->节点, 后->节点, 变化->节点};
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取任务状态变化", L"治理变化已有动作但证据不完整或不一致。");
    }
}

std::optional<新因果动作定义> 新_内部治理世界类::读取自我动作事实(稳定编码 节点) const noexcept {
    try {
        const auto 动作 = 动态服务_.获取动态(节点);
        if (!动作 || 动作->种类 != 新动态种类::动作
            || 动作->动作目标种类 != 新动作目标种类::任务虚拟存在) return std::nullopt;
        if (动作->动作目标种类 != 新动作目标种类::任务虚拟存在
            || !动作->动作目标任务虚拟存在 || !动作->动态概念节点)
            throw std::runtime_error("governance action projection missing");
        const auto 治理 = 读取任务治理场景(*动作->动作目标任务虚拟存在);
        const auto 场景 = 治理 ? 治理场景_.获取场景(治理->场景节点) : std::nullopt;
        if (!场景 || !含节点(场景->动态节点组, 节点)) throw std::runtime_error("governance action scene mismatch");
        const auto 主体 = 读取治理参与者(场景->节点, 动作->主题存在节点);
        const auto 客体 = 读取治理参与者(场景->节点, 动作->动作目标任务虚拟存在->虚拟存在节点);
        if (!主体 || 主体->种类 != 新治理参与者种类::现实执行主体引用
            || !客体 || 客体->任务引用 != 动作->动作目标任务虚拟存在)
            throw std::runtime_error("governance causal participants mismatch");
        新因果动作定义 结果;
        结果.动作概念节点 = *动作->动态概念节点;
        结果.执行主体存在节点 = 动作->主题存在节点;
        结果.作用对象任务虚拟存在 = 动作->动作目标任务虚拟存在;
        const auto 已结算定义 = 任务读取_ ? 任务读取_->获取任务已结算动作定义() : std::nullopt;
        if (已结算定义 && 动作->动作定义节点 == 已结算定义) {
            if (!动作->动作来源节点 || !动作->动作时间)
                throw std::runtime_error("settlement governance action source/time missing");
            const auto 来源 = 动态服务_.获取动态(*动作->动作来源节点);
            if (!来源 || 来源->种类 != 新动态种类::动作
                || 来源->动作目标种类 != 新动作目标种类::普通存在
                || 来源->动作来源节点 != 动作->动作目标任务虚拟存在->任务节点
                || 来源->主题存在节点 != 动作->主题存在节点 || !来源->动作时间
                || 来源->动作时间->语义 != 新状态时间语义::实例绝对UTC
                || 动作->动作时间->语义 != 新状态时间语义::实例绝对UTC
                || 来源->动作时间->纳秒 >= 动作->动作时间->纳秒)
                throw std::runtime_error("settlement governance source identity/time mismatch");
            const auto 完整 = 读取任务状态变化(*动作->动作目标任务虚拟存在, *动作->动作来源节点);
            if (!完整 || 完整->动作节点 != 节点)
                throw std::runtime_error("settlement governance causal action is not complete");
            // 只确认既有owner收尾路径的触发方式，不把结果反作触发前提。
            结果.触发方式 = 新因果动作触发方式::被动触发;
        }
        return 结果;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取治理动作事实", L"治理动作概念或参与者投影异常。");
    }
}

std::optional<新因果动作定义> 新_内部治理世界类::推断外部动作(稳定编码) const noexcept {
    // 本世界不以治理事实冒充现实外部动作观察。
    return std::nullopt;
}

void 新_内部治理世界类::交付实际发生(const 新治理任务变化结果& 变化) noexcept {
    try {
        const auto 场景 = 治理场景_.获取场景(变化.治理场景节点);
        const auto 动作 = 动态服务_.获取动态(变化.动作节点);
        const auto 结果 = 状态服务_.获取状态(变化.后状态节点);
        if (!场景 || !动作 || !动作->动作时间 || !结果) throw std::runtime_error("causal occurrence missing");
        std::optional<std::int64_t> 下界;
        for (const auto 节点 : 场景->动态节点组) {
            if (节点 == 变化.动作节点) continue;
            const auto 项 = 动态服务_.获取动态(节点);
            if (!项) throw std::runtime_error("scene dynamic missing");
            if (项->种类 != 新动态种类::动作) continue;
            for (const auto& 关系 : 项->动作结果组) {
                const auto 结果动态 = 动态服务_.获取动态(关系.结果动态节点);
                if (!结果动态) throw std::runtime_error("action result missing");
                if (结果动态->种类 != 新动态种类::原子 || 结果动态->状态节点组.size() != 2) continue;
                const auto 状态 = 状态服务_.获取状态(结果动态->状态节点组.back());
                if (!状态) throw std::runtime_error("action result state missing");
                if (状态->被描述存在节点 == 结果->被描述存在节点
                    && 状态->特征类型概念节点 == 结果->特征类型概念节点
                    && 状态->固定准确值 == 结果->固定准确值
                    && 状态->强时间.纳秒 < 动作->动作时间->纳秒
                    && (!下界 || 状态->强时间.纳秒 > *下界)) 下界 = 状态->强时间.纳秒;
            }
        }
        新因果发生记录 发生{变化.动作节点, 变化.治理场景节点, {}, {},
            变化.后状态节点, 变化.变化动态节点, 变化.动作节点};
        const auto 在窗口 = [&](const 新状态强时间& 时间) {
            return 时间.语义 == 新状态时间语义::实例绝对UTC
                && (!下界 || 时间.纳秒 > *下界) && 时间.纳秒 <= 动作->动作时间->纳秒;
        };
        for (const auto 节点 : 场景->状态节点组) {
            if (节点 == 变化.后状态节点) continue;
            const auto 状态 = 状态服务_.获取状态(节点);
            if (!状态) throw std::runtime_error("causal window state missing");
            if (在窗口(状态->强时间)) 发生.前置状态节点组.push_back(节点);
        }
        if (!含节点(发生.前置状态节点组, 变化.前状态节点)) 发生.前置状态节点组.push_back(变化.前状态节点);
        for (const auto 节点 : 场景->动态节点组) {
            if (节点 == 变化.动作节点 || 节点 == 变化.变化动态节点) continue;
            const auto 项 = 动态服务_.获取动态(节点);
            if (!项) throw std::runtime_error("causal window dynamic missing");
            if (项->种类 == 新动态种类::动作 || 项->状态节点组.empty()) continue;
            bool 全在窗口 = true;
            for (const auto 状态节点 : 项->状态节点组) {
                const auto 状态 = 状态服务_.获取状态(状态节点);
                if (!状态) throw std::runtime_error("causal window endpoint missing");
                if (!在窗口(状态->强时间)) 全在窗口 = false;
            }
            if (全在窗口) 发生.前置动态节点组.push_back(节点);
        }
        const auto 交付 = 因果服务_.处理动作事实(发生);
        switch (交付.状态) {
        case 新因果操作状态::已记录首例: case 新因果操作状态::已建立:
        case 新因果操作状态::已更新: case 新因果操作状态::重复发生:
        case 新因果操作状态::无共同条件: return;
        default: throw std::runtime_error("governance causal occurrence rejected");
        }
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"交付治理实际发生", L"真实治理材料向因果服务交付异常。");
    }
}

反向因果查询结果 新_内部治理世界类::查询因果链(const 反向因果查询请求& 请求) const noexcept {
    try {
        if (!获取根场景()) return {};
        auto 限定 = 请求;
        if (!限定.共同场景节点) 限定.共同场景节点 = 根场景节点_;
        if (!治理场景_.是场景节点(*限定.共同场景节点)) return {};
        for (const auto& 参与者 : 限定.参与者组) {
            if (参与者.任务虚拟存在引用) {
                const auto 读取 = 读取范围治理参与者(*限定.共同场景节点, 参与者.任务虚拟存在引用->虚拟存在节点);
                if (!读取 || 读取->任务引用 != 参与者.任务虚拟存在引用) return {};
            } else if (参与者.实际存在节点) {
                if (治理场景_.是场景节点(*参与者.实际存在节点)) {
                    // 场景也是存在，但不能以“属于同一世界”替代本次查询范围。
                    auto 当前 = *参与者.实际存在节点;
                    std::set<稳定编码> 已查;
                    while (当前 != *限定.共同场景节点) {
                        if (!已查.insert(当前).second) throw std::runtime_error("query participant scene ancestry cycles");
                        const auto 场景 = 治理场景_.获取场景(当前);
                        if (!场景) throw std::runtime_error("query participant scene ancestry unreadable");
                        if (!场景->父场景节点) return {};
                        当前 = *场景->父场景节点;
                    }
                } else if (!读取范围治理参与者(*限定.共同场景节点, *参与者.实际存在节点)) return {};
            }
        }
        return 反查服务_.查询(限定);
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"治理因果链查询", L"治理范围查询异常。");
    }
}
} // namespace 海中鱼巣
