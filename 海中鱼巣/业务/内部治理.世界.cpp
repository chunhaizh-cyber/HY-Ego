#include "内部治理.世界.h"
#include "../领域/新_任务类.h"
#include "../核心/容错检查.h"
#include <algorithm>
#include <limits>
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

std::optional<新治理场景参与者> 新_内部治理世界类::读取治理参与者(
    稳定编码 场景, 稳定编码 节点) const noexcept {
    if (!任务读取_) return std::nullopt;
    return 治理场景_.读取治理参与者(场景, 节点, *任务读取_);
}

新治理动作登记结果 新_内部治理世界类::建立任务状态动作(
    const 新治理任务动作请求& 请求) noexcept {
    try {
        if (!任务读取_ || !获取根场景()) return {};
        const auto 正式定义 = 任务读取_->获取任务已结算动作定义();
        if (!正式定义 || *正式定义 != 请求.动作定义节点) return {};
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
        核验挂接(治理场景_.添加治理任务引用(根场景节点_, 请求.目标, *任务读取_));
        核验挂接(治理场景_.添加治理执行主体引用(根场景节点_, 请求.执行主体节点,
            请求.现实来源场景节点, 现实场景_));
        核验挂接(治理场景_.添加治理状态(根场景节点_, 请求.前状态节点, *任务读取_));
        const auto 动作 = 动态服务_.建立内部治理动作动态({请求.执行主体节点,
            请求.动作定义节点, 请求.来源普通动作节点, 请求.目标, 请求.动作时间}, *任务读取_);
        if (动作.状态 != 新动态操作状态::已完成 || !动作.动态节点)
            throw std::runtime_error("governance action publication failed");
        核验挂接(治理场景_.添加治理动态(根场景节点_, *动作.动态节点, *任务读取_));
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
        核验挂接(治理场景_.添加治理状态(根场景节点_, 后节点, *任务读取_));
        const auto 变化 = 动态服务_.建立原子动态(目标.虚拟存在节点, 前节点, 后节点);
        if (变化.状态 != 新动态操作状态::已完成 || !变化.动态节点)
            throw std::runtime_error("governance atomic dynamic publication failed");
        const auto 关联 = 动态服务_.关联动作结果(动作节点, *变化.动态节点, 新动作结果关系类型::导致变化);
        if (关联 != 新动态操作状态::已完成) throw std::runtime_error("governance action result association failed");
        核验挂接(治理场景_.添加治理动态(根场景节点_, *变化.动态节点, *任务读取_));
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
    try {
        if (!任务读取_ || !获取根场景()) return std::nullopt;
        const auto 来源 = 动态服务_.按来源查询动作(来源普通动作);
        if (来源.状态 != 新动态操作状态::已完成) throw std::runtime_error("governance source read failed");
        if (!来源.动作) return std::nullopt;
        const auto& 动作 = *来源.动作;
        const auto 当前 = 任务读取_->读取任务虚拟存在引用(目标);
        const auto 场景 = 治理场景_.获取场景(根场景节点_);
        if (!当前 || 当前->当前状态 != static_cast<std::int64_t>(新任务状态::已结算)
            || !场景 || 动作.动作目标种类 != 新动作目标种类::任务虚拟存在
            || 动作.动作目标任务虚拟存在 != 目标 || !动作.动作时间
            || !动作.动作定义节点 || 动作.动作来源节点 != 来源普通动作
            || !含节点(场景->动态节点组, 动作.节点) || 动作.动作结果组.size() != 1
            || 动作.动作结果组.front().类型 != 新动作结果关系类型::导致变化)
            throw std::runtime_error("governance action readback mismatch");
        const auto 主体 = 读取治理参与者(根场景节点_, 动作.主题存在节点);
        const auto 客体 = 读取治理参与者(根场景节点_, 目标.虚拟存在节点);
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
        return 新治理任务变化结果{根场景节点_, 动作.节点, 前->节点, 后->节点, 变化->节点};
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取任务状态变化", L"治理变化已有动作但证据不完整或不一致。");
    }
}

std::optional<新因果动作定义> 新_内部治理世界类::读取自我动作事实(稳定编码 节点) const noexcept {
    try {
        const auto 场景 = 治理场景_.获取场景(根场景节点_);
        if (!场景 || !含节点(场景->动态节点组, 节点)) return std::nullopt;
        const auto 动作 = 动态服务_.获取动态(节点);
        if (!动作) throw std::runtime_error("registered governance action missing");
        if (动作->种类 != 新动态种类::动作) return std::nullopt;
        if (动作->动作目标种类 != 新动作目标种类::任务虚拟存在
            || !动作->动作目标任务虚拟存在 || !动作->动态概念节点)
            throw std::runtime_error("governance action projection missing");
        const auto 主体 = 读取治理参与者(根场景节点_, 动作->主题存在节点);
        const auto 客体 = 读取治理参与者(根场景节点_, 动作->动作目标任务虚拟存在->虚拟存在节点);
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
                const auto 读取 = 读取治理参与者(*限定.共同场景节点, 参与者.任务虚拟存在引用->虚拟存在节点);
                if (!读取 || 读取->任务引用 != 参与者.任务虚拟存在引用) return {};
            } else if (参与者.实际存在节点
                && !治理场景_.是场景节点(*参与者.实际存在节点)
                && !读取治理参与者(*限定.共同场景节点, *参与者.实际存在节点)) return {};
        }
        return 反查服务_.查询(限定);
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"治理因果链查询", L"治理范围查询异常。");
    }
}
} // namespace 海中鱼巣
