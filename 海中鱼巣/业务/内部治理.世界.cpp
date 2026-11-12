#include "内部治理.世界.h"
#include "../领域/世界挂接.全局根.h"
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
// METHOD-LEARN-STATE：方法状态变化治理动作与其学习记录逐项一致（来源、定义、主体、目标与目标特征、动作时间）。
void 核验学习记录动作(const 新方法学习记录& 记录, const 新动态信息& 动作) {
    if (动作.种类 != 新动态种类::动作 || !动作.动态概念节点
        || 动作.动作目标种类 != 新动作目标种类::方法虚拟存在
        || 动作.动作目标方法虚拟存在 != 记录.方法引用
        || 动作.动作目标特征节点 != 记录.特征节点
        || 动作.动作目标存在节点 || 动作.动作目标任务虚拟存在
        || 动作.动作来源节点 != 记录.节点
        || 动作.动作定义节点 != 记录.动作定义节点
        || 动作.主题存在节点 != 记录.执行主体节点 || 动作.动作时间 != 记录.动作时间)
        throw std::runtime_error("method state change action differs from its learning record");
}
// 方法当前读回中被记录改变的特征；当前值供新鲜度核验。
std::pair<稳定编码, std::int64_t> 学习记录目标特征(const 新方法学习记录& 记录,
    const 新方法虚拟存在读回& 方法, const 新方法状态变化定义& 定义) {
    if (定义.节点 != 记录.动作定义节点 || 定义.学习种类 != 记录.学习种类
        || 定义.特征种类 != 记录.特征种类 || 方法.引用 != 记录.方法引用)
        throw std::runtime_error("method learning record/definition/target mismatch");
    switch (记录.特征种类) {
    case 新方法状态特征种类::启用:
        if (记录.特征节点 != 方法.引用.启用特征节点 || 定义.目标特征类型概念节点 != 方法.启用特征类型概念节点)
            break;
        return {方法.启用特征类型概念节点, 方法.当前启用};
    case 新方法状态特征种类::条件结果配对数:
        if (!方法.条件结果配对数 || 记录.特征节点 != 方法.条件结果配对数->特征节点
            || 定义.目标特征类型概念节点 != 方法.条件结果配对数->特征类型概念节点) break;
        return {方法.条件结果配对数->特征类型概念节点, 方法.条件结果配对数->当前值};
    case 新方法状态特征种类::已知结果配对数:
        if (!方法.已知结果配对数 || 记录.特征节点 != 方法.已知结果配对数->特征节点
            || 定义.目标特征类型概念节点 != 方法.已知结果配对数->特征类型概念节点) break;
        return {方法.已知结果配对数->特征类型概念节点, 方法.已知结果配对数->当前值};
    }
    throw std::runtime_error("method learning record feature is not the method's feature of its kind");
}
// 学习记录与前状态一致：前状态是该特征在动作时间之前（含）的前值观察。返回目标特征类型根与当前值。
std::pair<稳定编码, std::int64_t> 核验学习记录前状态(const 新方法学习记录& 记录,
    const 新方法状态变化定义& 定义, const 新方法虚拟存在读回& 方法,
    const 新状态信息& 前, const 概念_特征类& 概念服务) {
    const auto 目标 = 学习记录目标特征(记录, 方法, 定义);
    if (前.节点 != 记录.前状态节点 || 前.被描述存在节点 != 记录.方法引用.虚拟存在节点
        || 前.来源实例特征节点 != 记录.特征节点
        || 前.固定准确值 != 新特征准确值{记录.前值}
        || 前.强时间.语义 != 新状态时间语义::实例绝对UTC
        || 记录.动作时间.语义 != 新状态时间语义::实例绝对UTC
        || 前.强时间.纳秒 <= 0 || 前.强时间.纳秒 > 记录.动作时间.纳秒)
        throw std::runtime_error("method learning record before state mismatch");
    const auto 关系 = 概念服务.比较单特征概念(前.特征类型概念节点, 前.特征类型概念节点);
    if (关系.状态 != 特征概念规则状态::已完成 || 关系.关系 != 单特征概念关系::同一
        || 关系.特征类型根 != 目标.first)
        throw std::runtime_error("method learning record before state has a different feature type");
    return 目标;
}
// TASK-STATE-CAUSAL 切片2：任务治理场景中的单个因素特征状态（任务owner在迁移前为因素新值建立）。
// 只认可本任务Vt的因素特征；“不合法子需求”值为U64数组材料引用，其余为I64枚举值。
const 新任务因素特征读回& 核验治理因素状态(const 新任务虚拟存在特征组& 特征组,
    const 新状态信息& 状态, const 概念_特征类& 概念服务) {
    const auto* 因素 = 特征组.查找因素(状态.来源实例特征节点);
    const bool 引用值 = 因素 && 因素->种类 == 新任务因素特征种类::不合法子需求;
    if (!因素 || 状态.被描述存在节点 != 特征组.引用.虚拟存在节点 || !状态.固定准确值
        || (引用值 ? !std::holds_alternative<稳定编码>(*状态.固定准确值)
            : !(std::holds_alternative<std::int64_t>(*状态.固定准确值)
                && std::get<std::int64_t>(*状态.固定准确值) >= 1))
        || 状态.强时间.语义 != 新状态时间语义::实例绝对UTC || 状态.强时间.纳秒 <= 0)
        throw std::runtime_error("governance factor state identity/value/time mismatch");
    const auto 关系 = 概念服务.比较单特征概念(状态.特征类型概念节点, 状态.特征类型概念节点);
    if (关系.状态 != 特征概念规则状态::已完成 || 关系.关系 != 单特征概念关系::同一
        || 关系.特征类型根 != 因素->特征类型概念节点)
        throw std::runtime_error("governance factor state has a different feature type root");
    return *因素;
}
// 返回状态或服务值特征的读回；F为因素特征时按因素状态逐端核验并返回空（因素无I64读回值）。
const 新任务虚拟存在特征读回* 核验治理特征状态对(
    const 新任务虚拟存在特征组& 特征组, 稳定编码 F,
    const 新状态信息& 前, const 新状态信息& 后, const 概念_特征类& 概念服务) {
    if (特征组.查找因素(F)) {
        if (前.节点 == 后.节点 || 前.来源实例特征节点 != F || 后.来源实例特征节点 != F
            || 前.固定准确值 == 后.固定准确值 || 后.强时间.纳秒 <= 前.强时间.纳秒)
            throw std::runtime_error("governance factor state pair identity/value/time mismatch");
        (void)核验治理因素状态(特征组, 前, 概念服务);
        (void)核验治理因素状态(特征组, 后, 概念服务);
        return nullptr;
    }
    const auto* 特征 = F == 特征组.状态.特征节点 ? &特征组.状态
        : 特征组.服务值 && F == 特征组.服务值->特征节点 ? &*特征组.服务值 : nullptr;
    if (!特征 || 前.节点 == 后.节点
        || 前.被描述存在节点 != 特征组.引用.虚拟存在节点
        || 后.被描述存在节点 != 前.被描述存在节点
        || 前.来源实例特征节点 != F || 后.来源实例特征节点 != F
        || !(前.固定准确值 && std::holds_alternative<std::int64_t>(*前.固定准确值))
        || !(后.固定准确值 && std::holds_alternative<std::int64_t>(*后.固定准确值))
        || 前.固定准确值 == 后.固定准确值
        || 前.强时间.语义 != 新状态时间语义::实例绝对UTC
        || 后.强时间.语义 != 新状态时间语义::实例绝对UTC
        || 前.强时间.纳秒 <= 0 || 后.强时间.纳秒 <= 前.强时间.纳秒)
        throw std::runtime_error("governance feature state pair identity/value/time mismatch");
    for (const auto 概念 : {前.特征类型概念节点, 后.特征类型概念节点}) {
        const auto 关系 = 概念服务.比较单特征概念(概念, 概念);
        if (关系.状态 != 特征概念规则状态::已完成 || 关系.关系 != 单特征概念关系::同一
            || 关系.特征类型根 != 特征->特征类型概念节点)
            throw std::runtime_error("historical governance feature concept has a different type root");
    }
    return 特征;
}
// TASK-STATE-CAUSAL 切片3（5260 §4.2第3项、6190 §3.4）：贡献阶段的动作定义。服务值阶段及历史状态收尾J
// 持贡献定义（种类相符）；新的状态收尾J（直属/最终结算收尾）持“已完成→已结算”迁移定义，以J为迁移记录。
// 迁移定义按贡献定义形状返回（种类取阶段种类），供既有前状态核验；迁移定义置位“迁移”。
std::optional<新任务贡献动作定义> 读取贡献阶段定义(const 新内部治理对象读取接口& 读取,
    const 新任务贡献动作阶段& 阶段, bool& 迁移) {
    迁移 = false;
    if (auto 定义 = 读取.读取任务贡献动作定义(阶段.动作定义节点)) return 定义;
    if (阶段.种类 != 新任务贡献动作种类::直属结算收尾
        && 阶段.种类 != 新任务贡献动作种类::最终结算收尾) return std::nullopt;
    const auto 定义 = 读取.读取任务状态迁移定义(阶段.动作定义节点);
    if (!定义 || 定义->前状态值 != static_cast<std::int64_t>(新任务状态::已完成)
        || 定义->后状态值 != static_cast<std::int64_t>(新任务状态::已结算)) return std::nullopt;
    迁移 = true;
    return 新任务贡献动作定义{定义->节点, 定义->目标特征类型概念节点, 定义->任务参数角色节点, 阶段.种类};
}
const 新任务虚拟存在特征读回& 核验贡献阶段前状态(const 新任务贡献动作阶段& 阶段,
    const 新任务贡献动作定义& 定义, const 新任务虚拟存在特征组& 特征组,
    const 新状态信息& 前, const 概念_特征类& 概念服务) {
    const 新任务虚拟存在特征读回* 特征 = nullptr;
    const auto 匹配 = [&](const 新任务虚拟存在特征读回& 候选) {
        if (候选.特征节点 != 阶段.目标.特征节点
            && 候选.特征类型概念节点 != 定义.目标特征类型概念节点) return;
        if (特征 || 候选.特征节点 != 阶段.目标.特征节点
            || 候选.特征类型概念节点 != 定义.目标特征类型概念节点)
            throw std::runtime_error("contribution target feature identity/type is ambiguous");
        特征 = &候选;
    };
    匹配(特征组.状态);
    if (特征组.服务值) 匹配(*特征组.服务值);
    const 新任务虚拟存在特征读回* 应有特征 = nullptr;
    switch (阶段.种类) {
    case 新任务贡献动作种类::来源计值:
    case 新任务贡献动作种类::直接接收:
    case 新任务贡献动作种类::权威迁移:
        if (特征组.服务值) 应有特征 = &*特征组.服务值;
        break;
    case 新任务贡献动作种类::直属结算收尾:
    case 新任务贡献动作种类::最终结算收尾:
        应有特征 = &特征组.状态;
        break;
    default:
        throw std::runtime_error("unknown contribution action kind");
    }
    if (定义.节点 != 阶段.动作定义节点 || 定义.种类 != 阶段.种类
        || 特征组.引用 != 阶段.目标.任务引用
        || 特征组.状态.特征节点 != 阶段.目标.任务引用.状态特征节点
        || !特征 || 特征 != 应有特征
        || ((阶段.种类 == 新任务贡献动作种类::直属结算收尾
                || 阶段.种类 == 新任务贡献动作种类::最终结算收尾)
            && (阶段.原值 < 0 || 阶段.新值 < 0))
        || 阶段.原值 == 阶段.新值
        || 前.节点 != 阶段.前状态节点
        || 前.被描述存在节点 != 阶段.目标.任务引用.虚拟存在节点
        || 前.来源实例特征节点 != 阶段.目标.特征节点
        || 前.固定准确值 != 新特征准确值{阶段.原值}
        || 前.强时间.语义 != 新状态时间语义::实例绝对UTC
        || 阶段.动作时间.语义 != 新状态时间语义::实例绝对UTC
        || 前.强时间.纳秒 <= 0 || 前.强时间.纳秒 > 阶段.动作时间.纳秒)
        throw std::runtime_error("contribution phase feature/before state mismatch");
    const auto 关系 = 概念服务.比较单特征概念(前.特征类型概念节点, 前.特征类型概念节点);
    if (关系.状态 != 特征概念规则状态::已完成 || 关系.关系 != 单特征概念关系::同一
        || 关系.特征类型根 != 定义.目标特征类型概念节点)
        throw std::runtime_error("contribution before state has a different feature type");
    return *特征;
}
void 核验贡献阶段动作(const 新任务贡献动作阶段& 阶段, const 新动态信息& 动作) {
    if (动作.种类 != 新动态种类::动作 || !动作.动态概念节点
        || 动作.动作目标种类 != 新动作目标种类::任务虚拟存在
        || 动作.动作目标任务虚拟存在 != 阶段.目标.任务引用
        || 动作.动作目标特征节点 != 阶段.目标.特征节点
        || 动作.动作目标存在节点 || 动作.动作来源节点 != 阶段.节点
        || 动作.动作定义节点 != 阶段.动作定义节点
        || 动作.主题存在节点 != 阶段.执行主体节点 || 动作.动作时间 != 阶段.动作时间)
        throw std::runtime_error("contribution action differs from its formal phase");
}
// TASK-STATE-CAUSAL：治理动作与其治理迁移记录逐项一致（来源、定义、主体、目标、动作时间）。
void 核验迁移记录动作(const 新任务状态迁移记录& 记录, const 新动态信息& 动作) {
    if (动作.种类 != 新动态种类::动作 || !动作.动态概念节点
        || 动作.动作目标种类 != 新动作目标种类::任务虚拟存在
        || 动作.动作目标任务虚拟存在 != 记录.任务引用
        || 动作.动作目标特征节点 != 记录.任务引用.状态特征节点
        || 动作.动作目标存在节点 || 动作.动作来源节点 != 记录.节点
        || 动作.动作定义节点 != 记录.动作定义节点
        || 动作.主题存在节点 != 记录.执行主体节点 || 动作.动作时间 != 记录.动作时间)
        throw std::runtime_error("task state migration action differs from its record");
}
// 迁移记录、迁移定义与前状态一致：前状态是本任务状态特征在动作时间之前的前值观察。
void 核验迁移记录前状态(const 新任务状态迁移记录& 记录, const 新任务状态迁移定义& 定义,
    const 新任务虚拟存在读回& 任务, const 新状态信息& 前, const 概念_特征类& 概念服务) {
    if (定义.节点 != 记录.动作定义节点 || 定义.前状态值 != 记录.前状态值
        || 定义.后状态值 != 记录.后状态值 || 定义.目标特征类型概念节点 != 任务.状态特征类型概念节点
        || 任务.引用 != 记录.任务引用 || 前.节点 != 记录.前状态节点
        || 前.被描述存在节点 != 记录.任务引用.虚拟存在节点
        || 前.来源实例特征节点 != 记录.任务引用.状态特征节点
        || 前.固定准确值 != 新特征准确值{记录.前状态值}
        || 前.强时间.语义 != 新状态时间语义::实例绝对UTC
        || 记录.动作时间.语义 != 新状态时间语义::实例绝对UTC
        || 前.强时间.纳秒 <= 0 || 前.强时间.纳秒 > 记录.动作时间.纳秒)
        throw std::runtime_error("task state migration record/definition/before state mismatch");
    const auto 关系 = 概念服务.比较单特征概念(前.特征类型概念节点, 前.特征类型概念节点);
    if (关系.状态 != 特征概念规则状态::已完成 || 关系.关系 != 单特征概念关系::同一
        || 关系.特征类型根 != 任务.状态特征类型概念节点)
        throw std::runtime_error("task state migration before state has a different feature type");
}
}

新_内部治理世界类::新_内部治理世界类(新_特征值类& 值, 概念_特征类& 特征概念,
    新_特征类& 特征, 概念_存在类& 存在概念, const 概念_动态类& 动态概念,
    新_状态类& 状态, 新_存在类& 存在, 新_动态类& 动态, const 新_场景类& 现实) noexcept
    : 特征概念服务_(特征概念), 特征服务_(特征), 状态服务_(状态), 动态服务_(动态), 现实场景_(现实),
      治理场景_(根场景节点_, 存在, 状态, 动态, 新场景树用途::内部治理),
      因果服务_(状态, 动态, 存在, 存在概念, 特征概念, 动态概念, 值, 治理场景_, this, this),
      反查服务_(因果服务_, 特征, 特征概念, 存在概念, 动态概念, 动态, 存在, 治理场景_, this) {}

bool 新_内部治理世界类::装配任务读取(const 新内部治理对象读取接口& 读取) noexcept {
    if (任务读取_) return 任务读取_ == &读取;
    if (有效(根场景节点_)) return false;
    任务读取_ = &读取;
    return true;
}

bool 新_内部治理世界类::装配方法读取(const 新方法治理对象读取接口& 读取) noexcept {
    if (方法读取_) return 方法读取_ == &读取;
    if (有效(根场景节点_)) return false;
    方法读取_ = &读取;
    return true;
}

bool 新_内部治理世界类::初始化() noexcept {
    try {
        if (!任务读取_ || !方法读取_) return false;
        if (!治理场景_.初始化() || !因果服务_.初始化())
            throw std::runtime_error("governance dependencies initialization failed");
        const auto 根 = 治理场景_.建立根场景();
        核验挂接(根.状态);
        if (!根.场景节点 || *根.场景节点 != 根场景节点_
            || 现实场景_.是场景节点(根场景节点_))
            throw std::runtime_error("governance root is not independent");
        const auto 全局根 = 新_全局根类::初始化();
        const auto 挂接 = 全局根 == 全局根操作状态::已完成 || 全局根 == 全局根操作状态::无变化
            ? 新_全局根类::挂接世界根(世界类型登记键::内部治理, 根场景节点_) : 全局根;
        if (挂接 != 全局根操作状态::已完成 && 挂接 != 全局根操作状态::无变化)
            throw std::runtime_error("governance root global attachment failed");
        return true;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"内部治理世界初始化", L"独立根或共享依赖初始化异常。");
    }
}

std::optional<稳定编码> 新_内部治理世界类::获取根场景() const noexcept {
    return 治理场景_.获取根场景();
}

std::optional<新方法治理场景信息> 新_内部治理世界类::读取方法治理场景(
    const 新方法虚拟存在引用& 引用) const noexcept {
    try {
        if (!方法读取_ || !获取根场景()) throw std::runtime_error("method scene dependencies missing");
        const auto 场景 = 治理场景_.读取方法治理场景(引用.方法节点);
        if (!场景) return std::nullopt;
        // 读取器不反查C(M)，且scene不持锁跨入method。合法未采用回收
        // 可以发生在两次读取之间，必须复核场景，不能把旧投影当损坏。
        const auto 方法 = 方法读取_->读取方法虚拟存在引用(引用);
        const auto 复核 = 治理场景_.读取方法治理场景(引用.方法节点);
        if (!复核) return std::nullopt;
        if (!方法 || 方法->引用 != 引用 || 场景->方法引用 != 引用
            || 场景->根场景节点 != 根场景节点_ || *复核 != *场景)
            throw std::runtime_error("method scene ownership readback mismatch");
        return 场景;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取方法治理场景", L"方法场景强归属读取异常。");
    }
}

新方法治理场景发布结果 新_内部治理世界类::发布新方法治理场景组(
    const std::vector<新方法虚拟存在引用>& 引用组, const 基础封存追加片段& 方法片段,
    新特征I64更新准备* 联动特征准备) noexcept {
    try {
        // 旧调用保持原封存身份及前提；适配器只在本次同步调用期间借用。
        const 新方法治理片段工厂 工厂 = [&方法片段]() { return 方法片段; };
        return 发布新方法治理场景组(引用组, 工厂, 联动特征准备);
    } catch (...) {
        return {}; // 工厂适配尚未进入候选准备，明确零发表。
    }
}

新方法治理场景发布结果 新_内部治理世界类::发布新方法治理场景组(
    const std::vector<新方法虚拟存在引用>& 引用组, const 新方法治理片段工厂& 方法片段工厂,
    新特征I64更新准备* 联动特征准备) noexcept {
    std::vector<新方法治理场景发布准备> 准备组;
    bool 发布结果须保留 = false;
    try {
        if (!方法读取_ || !获取根场景() || 引用组.empty())
            throw std::runtime_error("method scene publisher is not initialized or batch is empty");
        准备组.reserve(引用组.size());
        新方法治理场景发布结果 结果;
        结果.场景组.reserve(引用组.size());
        std::set<稳定编码> 方法组, 虚拟存在组, 特征组;
        for (const auto& 引用 : 引用组) {
            if (!方法组.insert(引用.方法节点).second || !虚拟存在组.insert(引用.虚拟存在节点).second
                || !特征组.insert(引用.启用特征节点).second)
                throw std::runtime_error("method scene batch has duplicate owners");
            auto 准备 = 治理场景_.准备方法治理场景(根场景节点_, 引用);
            if (!准备) throw std::runtime_error("method scene preparation failed");
            准备组.push_back(std::move(*准备));
        }
        // 全部场景prepare已经返回；这里不持场景/E/EC锁。仅同步调用一次，
        // 不保存工厂、不刷新旧片段，也不在CAS失败后重新调用。
        if (!方法片段工厂) throw std::runtime_error("method publication fragment factory is empty");
        const auto 方法片段 = 方法片段工厂();
        std::vector<基础封存追加片段> 片段组{方法片段};
        for (const auto& 准备 : 准备组) 片段组.push_back(准备.场景片段());
        基础节点组追加状态 状态;
        std::vector<基础片段发布回执> 回执组;
        发布结果须保留 = true;
        if (联动特征准备) {
            const auto 发布 = 特征服务_.提交顶层I64更新(*联动特征准备, 片段组);
            状态 = 发布.状态;
            回执组 = 发布.附加片段回执组;
        } else {
            const auto 发布 = 全局基础数据集.原子追加封存片段组(片段组);
            状态 = 发布.状态;
            回执组 = 发布.片段回执组;
        }
        if (状态 != 基础节点组追加状态::已完成) {
            if (!回执组.empty()) throw std::runtime_error("zero publication carries method receipts");
            发布结果须保留 = false;
            for (const auto& 准备 : 准备组) 治理场景_.撤销未发布方法治理场景(准备);
            return {};
        }
        if (回执组.size() != 片段组.size()) throw std::runtime_error("method publication receipt count mismatch");
        for (const auto& 片段 : 片段组)
            if (std::count_if(回执组.begin(), 回执组.end(), [&](const auto& r) { return r.对应片段(片段); }) != 1)
                throw std::runtime_error("method publication receipt identity mismatch");
        for (std::size_t i = 0; i < 引用组.size(); ++i) {
            const auto 场景 = 读取方法治理场景(引用组[i]);
            const auto 参与者 = 场景 ? 治理场景_.读取治理参与者(
                场景->场景节点, 引用组[i].虚拟存在节点, *方法读取_) : std::nullopt;
            if (!场景 || 场景->场景节点 != 准备组[i].候选场景节点()
                || !参与者 || 参与者->方法引用 != 引用组[i])
                throw std::runtime_error("published method/scene readback failed");
            结果.场景组.push_back(*场景);
        }
        // 联动旧F仍由方法owner收尾；这里不读取旧方法的派生概念。
        结果.状态 = 新方法治理场景发布工程状态::已发布;
        return 结果;
    } catch (...) {
        if (发布结果须保留)
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"发布方法治理场景", L"发表结果未知或发表后读回异常，保留方法、特征及场景。");
        for (const auto& 准备 : 准备组) 治理场景_.撤销未发布方法治理场景(准备);
        return {}; // 明确零发表，方法owner清理候选并报告工程失败。
    }
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
    if (!任务读取_ || !方法读取_) return std::nullopt;
    if (auto 方法 = 治理场景_.读取治理参与者(场景, 节点, *方法读取_)) return 方法;
    return 治理场景_.读取治理参与者(场景, 节点, *任务读取_);
}

新治理动作登记结果 新_内部治理世界类::建立任务特征动作(稳定编码 阶段节点) noexcept {
    try {
        if (!任务读取_) throw std::runtime_error("contribution task reader is not assembled");
        const auto 阶段 = 任务读取_->读取任务贡献动作阶段(阶段节点);
        if (!阶段 || 阶段->节点 != 阶段节点) throw std::runtime_error("formal contribution phase missing");
        bool 迁移定义 = false;
        const auto 定义 = 读取贡献阶段定义(*任务读取_, *阶段, 迁移定义);
        const auto 特征组 = 任务读取_->读取任务虚拟存在特征组(阶段->目标.任务引用);
        const auto 前 = 状态服务_.获取状态(阶段->前状态节点);
        const auto 治理 = 读取任务治理场景(阶段->目标.任务引用);
        if (!定义 || !特征组 || !前 || !治理) throw std::runtime_error("contribution action material missing");
        const auto& 目标特征 = 核验贡献阶段前状态(*阶段, *定义, *特征组, *前, 特征概念服务_);
        if (!现实场景_.存在属于场景范围(阶段->现实来源场景节点, 阶段->执行主体节点))
            throw std::runtime_error("contribution actor has no real scene source");
        const auto 已有 = 动态服务_.按来源查询动作(阶段节点);
        if (已有.状态 != 新动态操作状态::已完成) throw std::runtime_error("contribution action lookup failed");
        if (已有.动作) {
            核验贡献阶段动作(*阶段, *已有.动作);
            const auto 完整 = 读取任务特征动作(阶段节点);
            if (!完整 || 完整->动作节点 != 已有.动作->节点)
                throw std::runtime_error("partial contribution action cannot be replayed");
            return {新治理动作登记状态::精确重复, 已有.动作->节点};
        }
        if (阶段->发表结果节点 || 目标特征.当前值 != 阶段->原值)
            throw std::runtime_error("contribution action is already published or stale");
        const auto 场景 = 治理场景_.获取场景(治理->场景节点);
        if (!场景 || 含节点(场景->状态节点组, 阶段->前状态节点))
            throw std::runtime_error("contribution phase is partly attached without its action");
        核验挂接(治理场景_.添加治理任务引用(治理->场景节点, 阶段->目标.任务引用, *任务读取_));
        核验挂接(治理场景_.添加治理执行主体引用(治理->场景节点,
            阶段->执行主体节点, 阶段->现实来源场景节点, 现实场景_));
        核验挂接(治理场景_.添加治理状态(治理->场景节点, 阶段->前状态节点, *任务读取_));
        // 状态收尾J持迁移定义：按任务状态迁移治理动作建立（目标＝任务状态特征，来源＝J）。
        if (迁移定义 && 阶段->目标.特征节点 != 阶段->目标.任务引用.状态特征节点)
            throw std::runtime_error("settlement migration phase does not target the task state feature");
        const auto 动作 = 迁移定义
            ? 动态服务_.建立内部治理动作动态({阶段->执行主体节点,
                阶段->动作定义节点, 阶段节点, 阶段->目标.任务引用, 阶段->动作时间}, *任务读取_)
            : 动态服务_.建立内部治理特征动作动态({阶段->执行主体节点,
                阶段->动作定义节点, 阶段节点, 阶段->目标, 阶段->动作时间}, *任务读取_);
        if (动作.状态 != 新动态操作状态::已完成 || !动作.动态节点)
            throw std::runtime_error("contribution action publication failed");
        核验挂接(治理场景_.添加治理动态(治理->场景节点, *动作.动态节点, *任务读取_));
        const auto 读回 = 动态服务_.按来源查询动作(阶段节点);
        if (读回.状态 != 新动态操作状态::已完成 || !读回.动作
            || 读回.动作->节点 != *动作.动态节点 || !读回.动作->动作结果组.empty())
            throw std::runtime_error("new contribution action readback failed");
        核验贡献阶段动作(*阶段, *读回.动作);
        return {新治理动作登记状态::已建立, 动作.动态节点};
    } catch (const std::exception& e) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"建立任务特征动作",
            L"贡献阶段或治理动作不完整；保留已发布事实，不重放。", e.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"建立任务特征动作", L"未知工程异常；保留事实。");
    }
}

std::optional<新治理任务特征动作变化结果> 新_内部治理世界类::读取任务特征动作(
    稳定编码 阶段节点) const noexcept {
    try {
        if (!任务读取_) throw std::runtime_error("contribution task reader is not assembled");
        const auto 阶段 = 任务读取_->读取任务贡献动作阶段(阶段节点);
        if (!阶段) return std::nullopt;
        bool 迁移定义 = false;
        const auto 定义 = 读取贡献阶段定义(*任务读取_, *阶段, 迁移定义);
        const auto 特征组 = 任务读取_->读取任务虚拟存在特征组(阶段->目标.任务引用);
        const auto 前 = 状态服务_.获取状态(阶段->前状态节点);
        const auto 治理 = 读取任务治理场景(阶段->目标.任务引用);
        if (阶段->节点 != 阶段节点 || !阶段->发表结果节点 || !定义 || !特征组 || !前 || !治理)
            throw std::runtime_error("contribution phase is incomplete");
        (void)核验贡献阶段前状态(*阶段, *定义, *特征组, *前, 特征概念服务_);
        const auto 来源 = 动态服务_.按来源查询动作(阶段节点);
        if (来源.状态 != 新动态操作状态::已完成 || !来源.动作)
            throw std::runtime_error("published contribution phase has no unique action");
        const auto& 动作 = *来源.动作;
        核验贡献阶段动作(*阶段, 动作);
        const auto 场景 = 治理场景_.获取场景(治理->场景节点);
        const auto 主体 = 读取治理参与者(治理->场景节点, 阶段->执行主体节点);
        const auto 客体 = 读取治理参与者(治理->场景节点, 阶段->目标.任务引用.虚拟存在节点);
        if (!场景 || !含节点(场景->动态节点组, 动作.节点) || !主体
            || 主体->种类 != 新治理参与者种类::现实执行主体引用
            || 主体->现实来源场景节点 != 阶段->现实来源场景节点
            || !现实场景_.存在属于场景范围(阶段->现实来源场景节点, 阶段->执行主体节点)
            || !客体 || 客体->任务引用 != 阶段->目标.任务引用
            || 动作.动作结果组.size() != 1
            || 动作.动作结果组.front().类型 != 新动作结果关系类型::导致变化)
            throw std::runtime_error("contribution action scene/result ownership mismatch");
        const auto 变化 = 动态服务_.获取动态(动作.动作结果组.front().结果动态节点);
        if (!变化 || 变化->种类 != 新动态种类::原子 || !变化->动态概念节点
            || 变化->主题存在节点 != 阶段->目标.任务引用.虚拟存在节点
            || 变化->状态节点组.size() != 2 || 变化->状态节点组.front() != 前->节点
            || !含节点(场景->动态节点组, 变化->节点))
            throw std::runtime_error("contribution result dynamic is incomplete");
        const auto 后 = 状态服务_.获取状态(变化->状态节点组.back());
        if (!后 || !含节点(场景->状态节点组, 前->节点) || !含节点(场景->状态节点组, 后->节点))
            throw std::runtime_error("contribution frozen states are not in the task scene");
        (void)核验治理特征状态对(*特征组, 阶段->目标.特征节点, *前, *后, 特征概念服务_);
        if (后->固定准确值 != 新特征准确值{阶段->新值} || 后->强时间.纳秒 <= 阶段->动作时间.纳秒)
            throw std::runtime_error("contribution result does not match the frozen phase");
        // 历史动作按冻结S读取，不能要求历史新值仍等于当前F。
        for (const auto d : 场景->动态节点组) {
            const auto 候选 = 动态服务_.获取动态(d);
            if (!候选) throw std::runtime_error("contribution scene dynamic unreadable");
            if (d != 变化->节点 && 候选->种类 == 新动态种类::原子
                && 候选->状态节点组 == 变化->状态节点组)
                throw std::runtime_error("duplicate contribution state-pair dynamic");
            if (d != 动作.节点 && 候选->种类 == 新动态种类::动作)
                for (const auto& 关系 : 候选->动作结果组)
                    if (关系.结果动态节点 == 变化->节点)
                        throw std::runtime_error("contribution result belongs to more than one action");
        }
        // 场景列表本身不足以发现未挂接重复D或异场景引用；沿正式完整入边
        // 识别D及其状态项，仍由dynamic owner解释内容，不解析其私有字段。
        const std::vector<稳定编码> 端点{前->节点, 后->节点, 动作.节点, 变化->节点};
        const auto 结构 = 全局基础数据集.查询节点组完整结构(端点);
        if (结构.状态 != 基础结构操作状态::已完成 || 结构.节点组.size() != 端点.size())
            throw std::runtime_error("contribution package references unreadable");
        const auto 核验引用动态 = [&](稳定编码 n) {
            if (!动态服务_.是动态节点(n)) return false;
            const auto d = 动态服务_.获取动态(n);
            if (!d || !含节点(场景->动态节点组, n))
                throw std::runtime_error("contribution state has an unattached or foreign dynamic consumer");
            if (d->种类 == 新动态种类::原子 && d->状态节点组 == 变化->状态节点组
                && d->节点 != 变化->节点)
                throw std::runtime_error("contribution package has an unattached duplicate dynamic");
            if (d->种类 == 新动态种类::动作 && d->节点 != 动作.节点)
                for (const auto& r : d->动作结果组)
                    if (r.结果动态节点 == 变化->节点)
                        throw std::runtime_error("contribution result has another action consumer");
            return true;
        };
        std::set<稳定编码> 已读, 已检查引用;
        for (const auto& 项 : 结构.节点组) {
            if (!含节点(端点, 项.查询节点) || !项.快照 || !已读.insert(项.查询节点).second)
                throw std::runtime_error("contribution reference identity mismatch");
            for (const auto& r : 项.快照->入向外部关系组) (void)核验引用动态(r.源节点);
            for (const auto& f : 项.快照->入向字段组) {
                const auto n = f.所属节点;
                if (n == 场景->节点 || !已检查引用.insert(n).second) continue;
                if (治理场景_.是场景节点(n) || 现实场景_.是场景节点(n))
                    throw std::runtime_error("contribution fact is attached to another scene");
                if (核验引用动态(n)) continue;
                const auto 引用 = 全局基础数据集.查询节点组完整结构({n});
                if (引用.状态 != 基础结构操作状态::已完成 || 引用.节点组.size() != 1
                    || 引用.节点组.front().查询节点 != n || !引用.节点组.front().快照)
                    throw std::runtime_error("contribution state reference item unreadable");
                for (const auto& f2 : 引用.节点组.front().快照->入向字段组)
                    (void)核验引用动态(f2.所属节点);
            }
        }
        return 新治理任务特征动作变化结果{阶段->目标, 阶段节点, 场景->节点,
            动作.节点, 前->节点, 后->节点, 变化->节点};
    } catch (const std::exception& e) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取任务特征动作",
            L"贡献动作阶段或状态动态闭包不完整；保留现场。", e.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取任务特征动作", L"未知工程读取异常。");
    }
}

新治理任务特征动作变化结果 新_内部治理世界类::完成任务特征动作(
    稳定编码 阶段节点, 稳定编码 后节点) noexcept {
    try {
        if (!任务读取_) throw std::runtime_error("contribution task reader is not assembled");
        const auto 阶段 = 任务读取_->读取任务贡献动作阶段(阶段节点);
        if (!阶段 || 阶段->节点 != 阶段节点 || !阶段->发表结果节点)
            throw std::runtime_error("contribution core has not been published");
        bool 迁移定义 = false;
        const auto 定义 = 读取贡献阶段定义(*任务读取_, *阶段, 迁移定义);
        const auto 特征组 = 任务读取_->读取任务虚拟存在特征组(阶段->目标.任务引用);
        const auto 前 = 状态服务_.获取状态(阶段->前状态节点), 后 = 状态服务_.获取状态(后节点);
        const auto 治理 = 读取任务治理场景(阶段->目标.任务引用);
        const auto 来源 = 动态服务_.按来源查询动作(阶段节点);
        if (!定义 || !特征组 || !前 || !后 || !治理
            || 来源.状态 != 新动态操作状态::已完成 || !来源.动作)
            throw std::runtime_error("contribution completion material missing");
        const auto& 目标特征 = 核验贡献阶段前状态(*阶段, *定义, *特征组, *前, 特征概念服务_);
        核验贡献阶段动作(*阶段, *来源.动作);
        (void)核验治理特征状态对(*特征组, 阶段->目标.特征节点, *前, *后, 特征概念服务_);
        if (后->固定准确值 != 新特征准确值{阶段->新值} || 后->强时间.纳秒 <= 阶段->动作时间.纳秒)
            throw std::runtime_error("contribution after state differs from core result");
        if (!来源.动作->动作结果组.empty()) {
            const auto 完整 = 读取任务特征动作(阶段节点);
            if (!完整 || 完整->后状态节点 != 后节点)
                throw std::runtime_error("contribution completion replay conflicts");
            return *完整;
        }
        if (目标特征.当前值 != 阶段->新值 || 后->被引用次数 != 0)
            throw std::runtime_error("new contribution result is stale or partly consumed");
        const auto 场景 = 治理场景_.获取场景(治理->场景节点);
        if (!场景 || !含节点(场景->动态节点组, 来源.动作->节点)
            || !含节点(场景->状态节点组, 前->节点) || 含节点(场景->状态节点组, 后节点))
            throw std::runtime_error("contribution action was not completely attached before core publication");
        核验挂接(治理场景_.添加治理状态(治理->场景节点, 后节点, *任务读取_));
        const auto 变化 = 动态服务_.建立原子动态(阶段->目标.任务引用.虚拟存在节点, 前->节点, 后节点);
        if (变化.状态 != 新动态操作状态::已完成 || !变化.动态节点)
            throw std::runtime_error("contribution result dynamic publication failed");
        核验挂接(治理场景_.添加治理动态(治理->场景节点, *变化.动态节点, *任务读取_));
        if (动态服务_.关联动作结果(来源.动作->节点, *变化.动态节点,
            新动作结果关系类型::导致变化) != 新动态操作状态::已完成)
            throw std::runtime_error("contribution action result association failed");
        const auto 完整 = 读取任务特征动作(阶段节点);
        if (!完整 || 完整->后状态节点 != 后节点 || 完整->变化动态节点 != *变化.动态节点)
            throw std::runtime_error("contribution completion readback failed");
        // TASK-STATE-CAUSAL 切片3（5260 §4.2第3、6项）：持迁移定义的结算收尾J即“已完成→已结算”迁移记录，
        // 完成后同步交付实际发生；服务值阶段及历史状态收尾J不交付。
        if (迁移定义)
            交付实际发生(新治理任务变化结果{完整->治理场景节点, 完整->动作节点,
                完整->前状态节点, 完整->后状态节点, 完整->变化动态节点});
        return *完整;
    } catch (const std::exception& e) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"完成任务特征动作",
            L"贡献核心已发表后的组织异常；保留动作及全部事实。", e.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"完成任务特征动作", L"未知工程异常；保留事实。");
    }
}

// ---- TASK-STATE-CAUSAL：任务状态迁移治理动作（来源＝治理迁移记录，主体＝自我）----
新治理动作登记结果 新_内部治理世界类::建立任务状态迁移动作(稳定编码 记录节点) noexcept {
    try {
        if (!任务读取_) throw std::runtime_error("migration task reader is not assembled");
        const auto 记录 = 任务读取_->读取任务状态迁移记录(记录节点);
        if (!记录 || 记录->节点 != 记录节点) throw std::runtime_error("formal migration record missing");
        const auto 定义 = 任务读取_->读取任务状态迁移定义(记录->动作定义节点);
        const auto 当前 = 任务读取_->读取任务虚拟存在引用(记录->任务引用);
        const auto 前 = 状态服务_.获取状态(记录->前状态节点);
        const auto 治理 = 读取任务治理场景(记录->任务引用);
        if (!定义 || !当前 || !前 || !治理) throw std::runtime_error("migration action material missing");
        核验迁移记录前状态(*记录, *定义, *当前, *前, 特征概念服务_);
        if (!现实场景_.存在属于场景范围(记录->现实来源场景节点, 记录->执行主体节点))
            throw std::runtime_error("migration actor has no real scene source");
        const auto 已有 = 动态服务_.按来源查询动作(记录节点);
        if (已有.状态 != 新动态操作状态::已完成) throw std::runtime_error("migration action lookup failed");
        if (已有.动作) {
            核验迁移记录动作(*记录, *已有.动作);
            const auto 完整 = 读取任务状态迁移(记录节点);
            if (!完整 || 完整->动作节点 != 已有.动作->节点)
                throw std::runtime_error("partial migration action cannot be replayed");
            return {新治理动作登记状态::精确重复, 已有.动作->节点};
        }
        if (当前->当前状态 != 记录->前状态值)
            throw std::runtime_error("migration record is stale against current task state");
        const auto 场景 = 治理场景_.获取场景(治理->场景节点);
        if (!场景 || 含节点(场景->状态节点组, 记录->前状态节点))
            throw std::runtime_error("migration before state is attached without its action");
        核验挂接(治理场景_.添加治理任务引用(治理->场景节点, 记录->任务引用, *任务读取_));
        核验挂接(治理场景_.添加治理执行主体引用(治理->场景节点,
            记录->执行主体节点, 记录->现实来源场景节点, 现实场景_));
        核验挂接(治理场景_.添加治理状态(治理->场景节点, 记录->前状态节点, *任务读取_));
        const auto 动作 = 动态服务_.建立内部治理动作动态({记录->执行主体节点,
            记录->动作定义节点, 记录节点, 记录->任务引用, 记录->动作时间}, *任务读取_);
        if (动作.状态 != 新动态操作状态::已完成 || !动作.动态节点)
            throw std::runtime_error("migration action publication failed; status="
                + std::to_string(static_cast<int>(动作.状态)));
        核验挂接(治理场景_.添加治理动态(治理->场景节点, *动作.动态节点, *任务读取_));
        const auto 读回 = 动态服务_.按来源查询动作(记录节点);
        if (读回.状态 != 新动态操作状态::已完成 || !读回.动作
            || 读回.动作->节点 != *动作.动态节点 || !读回.动作->动作结果组.empty())
            throw std::runtime_error("new migration action readback failed");
        核验迁移记录动作(*记录, *读回.动作);
        return {新治理动作登记状态::已建立, 动作.动态节点};
    } catch (const std::exception& e) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"建立任务状态迁移动作",
            L"治理迁移记录或治理动作不完整；保留已发布事实，不重放。", e.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"建立任务状态迁移动作", L"未知工程异常；保留事实。");
    }
}

std::optional<新治理任务状态迁移结果> 新_内部治理世界类::读取任务状态迁移(
    稳定编码 记录节点) const noexcept {
    try {
        if (!任务读取_) throw std::runtime_error("migration task reader is not assembled");
        const auto 记录 = 任务读取_->读取任务状态迁移记录(记录节点);
        if (!记录) return std::nullopt;
        const auto 定义 = 任务读取_->读取任务状态迁移定义(记录->动作定义节点);
        const auto 当前 = 任务读取_->读取任务虚拟存在引用(记录->任务引用);
        const auto 前 = 状态服务_.获取状态(记录->前状态节点);
        const auto 治理 = 读取任务治理场景(记录->任务引用);
        if (记录->节点 != 记录节点 || !定义 || !当前 || !前 || !治理)
            throw std::runtime_error("migration record material is incomplete");
        核验迁移记录前状态(*记录, *定义, *当前, *前, 特征概念服务_);
        const auto 来源 = 动态服务_.按来源查询动作(记录节点);
        if (来源.状态 != 新动态操作状态::已完成) throw std::runtime_error("migration action lookup failed");
        // 记录已发布而动作或结果尚未形成：只可能处于同一owner锁内的迁移过程中，按未完成返回。
        if (!来源.动作) return std::nullopt;
        const auto& 动作 = *来源.动作;
        核验迁移记录动作(*记录, 动作);
        if (动作.动作结果组.empty()) return std::nullopt;
        const auto 场景 = 治理场景_.获取场景(治理->场景节点);
        const auto 主体 = 读取治理参与者(治理->场景节点, 记录->执行主体节点);
        const auto 客体 = 读取治理参与者(治理->场景节点, 记录->任务引用.虚拟存在节点);
        if (!场景 || !含节点(场景->动态节点组, 动作.节点) || !主体
            || 主体->种类 != 新治理参与者种类::现实执行主体引用
            || 主体->现实来源场景节点 != 记录->现实来源场景节点
            || !现实场景_.存在属于场景范围(记录->现实来源场景节点, 记录->执行主体节点)
            || !客体 || 客体->任务引用 != 记录->任务引用
            || 动作.动作结果组.size() != 1
            || 动作.动作结果组.front().类型 != 新动作结果关系类型::导致变化)
            throw std::runtime_error("migration action scene/result ownership mismatch");
        const auto 变化 = 动态服务_.获取动态(动作.动作结果组.front().结果动态节点);
        if (!变化 || 变化->种类 != 新动态种类::原子 || !变化->动态概念节点
            || 变化->主题存在节点 != 记录->任务引用.虚拟存在节点
            || 变化->状态节点组.size() != 2 || 变化->状态节点组.front() != 前->节点
            || !含节点(场景->动态节点组, 变化->节点))
            throw std::runtime_error("migration result dynamic is incomplete");
        const auto 后 = 状态服务_.获取状态(变化->状态节点组.back());
        if (!后 || !含节点(场景->状态节点组, 前->节点) || !含节点(场景->状态节点组, 后->节点)
            || 后->被描述存在节点 != 记录->任务引用.虚拟存在节点
            || 后->来源实例特征节点 != 记录->任务引用.状态特征节点
            || 后->固定准确值 != 新特征准确值{记录->后状态值}
            || 后->强时间.语义 != 新状态时间语义::实例绝对UTC
            || 后->强时间.纳秒 <= 记录->动作时间.纳秒)
            throw std::runtime_error("migration after state does not match its record");
        const auto 关系 = 特征概念服务_.比较单特征概念(后->特征类型概念节点, 后->特征类型概念节点);
        if (关系.状态 != 特征概念规则状态::已完成 || 关系.关系 != 单特征概念关系::同一
            || 关系.特征类型根 != 当前->状态特征类型概念节点)
            throw std::runtime_error("migration after state has a different feature type");
        for (const auto d : 场景->动态节点组) {
            if (d == 动作.节点 || d == 变化->节点) continue;
            const auto 候选 = 动态服务_.获取动态(d);
            if (!候选) throw std::runtime_error("migration scene dynamic unreadable");
            if (候选->种类 == 新动态种类::原子 && 候选->状态节点组 == 变化->状态节点组)
                throw std::runtime_error("duplicate migration state-pair dynamic");
            if (候选->种类 == 新动态种类::动作)
                for (const auto& 关系项 : 候选->动作结果组)
                    if (关系项.结果动态节点 == 变化->节点)
                        throw std::runtime_error("migration result belongs to more than one action");
        }
        return 新治理任务状态迁移结果{记录节点, 场景->节点, 动作.节点, 前->节点, 后->节点, 变化->节点};
    } catch (const std::exception& e) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取任务状态迁移",
            L"治理迁移记录、动作或状态动态闭包不一致；保留现场。", e.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取任务状态迁移", L"未知工程读取异常。");
    }
}

新治理任务状态迁移结果 新_内部治理世界类::完成任务状态迁移(
    稳定编码 记录节点, 稳定编码 后节点) noexcept {
    try {
        if (!任务读取_) throw std::runtime_error("migration task reader is not assembled");
        const auto 记录 = 任务读取_->读取任务状态迁移记录(记录节点);
        if (!记录 || 记录->节点 != 记录节点) throw std::runtime_error("formal migration record missing");
        const auto 定义 = 任务读取_->读取任务状态迁移定义(记录->动作定义节点);
        const auto 当前 = 任务读取_->读取任务虚拟存在引用(记录->任务引用);
        const auto 前 = 状态服务_.获取状态(记录->前状态节点), 后 = 状态服务_.获取状态(后节点);
        const auto 治理 = 读取任务治理场景(记录->任务引用);
        const auto 来源 = 动态服务_.按来源查询动作(记录节点);
        if (!定义 || !当前 || !前 || !后 || !治理
            || 来源.状态 != 新动态操作状态::已完成 || !来源.动作)
            throw std::runtime_error("migration completion material missing");
        核验迁移记录前状态(*记录, *定义, *当前, *前, 特征概念服务_);
        核验迁移记录动作(*记录, *来源.动作);
        if (后->节点 == 前->节点 || 后->被描述存在节点 != 记录->任务引用.虚拟存在节点
            || 后->来源实例特征节点 != 记录->任务引用.状态特征节点
            || 后->固定准确值 != 新特征准确值{记录->后状态值}
            || 后->强时间.语义 != 新状态时间语义::实例绝对UTC
            || 后->强时间.纳秒 <= 记录->动作时间.纳秒)
            throw std::runtime_error("migration after state differs from its record");
        if (!来源.动作->动作结果组.empty()) {
            const auto 完整 = 读取任务状态迁移(记录节点);
            if (!完整 || 完整->后状态节点 != 后节点)
                throw std::runtime_error("migration completion replay conflicts");
            return *完整; // 精确重放不重复交付。
        }
        if (当前->当前状态 != 记录->后状态值 || 后->被引用次数 != 0)
            throw std::runtime_error("new migration result is stale or partly consumed");
        const auto 场景 = 治理场景_.获取场景(治理->场景节点);
        if (!场景 || !含节点(场景->动态节点组, 来源.动作->节点)
            || !含节点(场景->状态节点组, 前->节点) || 含节点(场景->状态节点组, 后节点))
            throw std::runtime_error("migration action was not completely attached before state commit");
        核验挂接(治理场景_.添加治理状态(治理->场景节点, 后节点, *任务读取_));
        const auto 变化 = 动态服务_.建立原子动态(记录->任务引用.虚拟存在节点, 前->节点, 后节点);
        if (变化.状态 != 新动态操作状态::已完成 || !变化.动态节点)
            throw std::runtime_error("migration result dynamic publication failed");
        核验挂接(治理场景_.添加治理动态(治理->场景节点, *变化.动态节点, *任务读取_));
        if (动态服务_.关联动作结果(来源.动作->节点, *变化.动态节点,
            新动作结果关系类型::导致变化) != 新动态操作状态::已完成)
            throw std::runtime_error("migration action result association failed");
        const auto 完整 = 读取任务状态迁移(记录节点);
        if (!完整 || 完整->后状态节点 != 后节点 || 完整->变化动态节点 != *变化.动态节点)
            throw std::runtime_error("migration completion readback failed");
        // 5260 §4.2第6项：迁移完成后同步交付实际发生，前置窗口规则不变。
        交付实际发生(新治理任务变化结果{完整->治理场景节点, 完整->动作节点,
            完整->前状态节点, 完整->后状态节点, 完整->变化动态节点});
        return *完整;
    } catch (const std::exception& e) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"完成任务状态迁移",
            L"任务状态已提交后的治理组织或交付异常；保留动作及全部事实，不重做迁移。", e.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"完成任务状态迁移", L"未知工程异常；保留事实。");
    }
}

void 新_内部治理世界类::登记任务因素状态(
    const 新任务虚拟存在引用& 引用, 稳定编码 状态节点) noexcept {
    try {
        if (!任务读取_) throw std::runtime_error("factor state task reader is not assembled");
        const auto 治理 = 读取任务治理场景(引用);
        const auto 特征组 = 任务读取_->读取任务虚拟存在特征组(引用);
        const auto 状态 = 状态服务_.获取状态(状态节点);
        if (!治理 || !特征组 || 特征组->引用 != 引用 || !状态)
            throw std::runtime_error("factor state material missing");
        (void)核验治理因素状态(*特征组, *状态, 特征概念服务_);
        if (状态->被引用次数 != 0) throw std::runtime_error("new factor state is already consumed");
        核验挂接(治理场景_.添加治理状态(治理->场景节点, 状态节点, *任务读取_));
    } catch (const std::exception& e) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"登记任务因素状态",
            L"因素特征状态无法挂入任务治理场景；保留事实，不重做。", e.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"登记任务因素状态", L"未知工程异常；保留事实。");
    }
}

std::optional<新治理任务特征变化结果> 新_内部治理世界类::读取任务特征变化(
    const 新任务虚拟存在特征引用& 目标, 稳定编码 前节点, 稳定编码 后节点) const noexcept {
    try {
        if (!任务读取_) throw std::runtime_error("task feature reader is not assembled");
        const auto 治理 = 读取任务治理场景(目标.任务引用);
        const auto 特征组 = 任务读取_->读取任务虚拟存在特征组(目标.任务引用);
        const auto 前 = 状态服务_.获取状态(前节点), 后 = 状态服务_.获取状态(后节点);
        if (!治理 || !特征组 || 特征组->引用 != 目标.任务引用 || !前 || !后)
            throw std::runtime_error("task feature change material missing");
        (void)核验治理特征状态对(*特征组, 目标.特征节点, *前, *后, 特征概念服务_);
        const auto 场景 = 治理场景_.获取场景(治理->场景节点);
        if (!场景) throw std::runtime_error("task feature change scene unreadable");
        std::set<稳定编码> 已覆盖状态, 动作结果;
        std::set<std::pair<稳定编码, 稳定编码>> 已见配对;
        std::optional<新治理任务特征变化结果> 结果;
        for (const auto d : 场景->动态节点组) {
            const auto 动态 = 动态服务_.获取动态(d);
            if (!动态 || !动态->动态概念节点) throw std::runtime_error("task scene dynamic unreadable");
            if (动态->种类 == 新动态种类::动作) {
                if (动态->动作目标任务虚拟存在 != 目标.任务引用
                    || !动态->动作来源节点 || !动态->动作定义节点)
                    throw std::runtime_error("unsupported task scene action");
                // 切片3：以贡献阶段为来源的动作（含持迁移定义的结算收尾J）按贡献包读回。
                if (任务读取_->读取任务贡献动作定义(*动态->动作定义节点)
                    || 任务读取_->读取任务贡献动作阶段(*动态->动作来源节点)) {
                    const auto 包 = 读取任务特征动作(*动态->动作来源节点);
                    if (!包 || 包->动作节点 != d || 包->治理场景节点 != 场景->节点
                        || !动作结果.insert(包->变化动态节点).second)
                        throw std::runtime_error("task scene contribution package mismatch");
                    continue;
                }
                if (任务读取_->读取任务状态迁移定义(*动态->动作定义节点)) {
                    const auto 包 = 读取任务状态迁移(*动态->动作来源节点);
                    if (!包 || 包->动作节点 != d || 包->治理场景节点 != 场景->节点
                        || !动作结果.insert(包->变化动态节点).second)
                        throw std::runtime_error("task scene migration package mismatch");
                    continue;
                }
                if (动态->动作定义节点 != 任务读取_->获取任务已结算动作定义())
                    throw std::runtime_error("task scene action has no formal governance definition");
                const auto 包 = 读取场景内任务状态变化(目标.任务引用, *动态->动作来源节点, 场景->节点);
                if (!包 || 包->动作节点 != d || !动作结果.insert(包->变化动态节点).second)
                    throw std::runtime_error("task scene action package mismatch");
                continue;
            }
            if (动态->种类 != 新动态种类::原子 || 动态->状态节点组.size() != 2
                || 动态->主题存在节点 != 目标.任务引用.虚拟存在节点)
                throw std::runtime_error("unsupported task scene change");
            const auto a = 状态服务_.获取状态(动态->状态节点组[0]);
            const auto b = 状态服务_.获取状态(动态->状态节点组[1]);
            if (!a || !b || !含节点(场景->状态节点组, a->节点) || !含节点(场景->状态节点组, b->节点)
                || !已见配对.emplace(a->节点, b->节点).second)
                throw std::runtime_error("task scene has partial or duplicate state pairs");
            (void)核验治理特征状态对(*特征组, a->来源实例特征节点, *a, *b, 特征概念服务_);
            已覆盖状态.insert(a->节点);
            已覆盖状态.insert(b->节点);
            if (a->节点 == 前节点 && b->节点 == 后节点)
                结果 = 新治理任务特征变化结果{目标, 场景->节点, 前节点, 后节点, d};
        }
        // 切片2：因素特征的单个状态（不成对、无动态）是合法的任务治理场景状态。
        for (const auto s : 场景->状态节点组) {
            if (已覆盖状态.contains(s)) continue;
            const auto 状态 = 状态服务_.获取状态(s);
            if (!状态 || !特征组->查找因素(状态->来源实例特征节点)) continue;
            (void)核验治理因素状态(*特征组, *状态, 特征概念服务_);
            已覆盖状态.insert(s);
        }
        if (已覆盖状态 != std::set<稳定编码>(场景->状态节点组.begin(), 场景->状态节点组.end()))
            throw std::runtime_error("task scene contains orphan state facts");

        // 检查完整入边，既能发现异场景挂接，也能发现尚未挂接的重复D。
        // 不用按状态查询动态的容错空列表推定“没有”。
        const auto 结构 = 全局基础数据集.查询节点组完整结构({前节点, 后节点});
        if (结构.状态 != 基础结构操作状态::已完成 || 结构.节点组.size() != 2)
            throw std::runtime_error("task state incoming references unreadable");
        const auto 核验引用动态 = [&](稳定编码 节点) {
            if (!动态服务_.是动态节点(节点)) return false;
            const auto 动态 = 动态服务_.获取动态(节点);
            if (!动态 || !含节点(场景->动态节点组, 节点))
                throw std::runtime_error("task state is consumed by an unreadable or unattached dynamic");
            if (动态->种类 == 新动态种类::原子
                && 动态->状态节点组 == std::vector<稳定编码>{前节点, 后节点}
                && (!结果 || 动态->节点 != 结果->变化动态节点))
                throw std::runtime_error("duplicate exact task change");
            return true;
        };
        std::set<稳定编码> 已核验引用项, 已读端点;
        for (const auto& 项 : 结构.节点组) {
            if (!项.快照 || (项.查询节点 != 前节点 && 项.查询节点 != 后节点)
                || !已读端点.insert(项.查询节点).second)
                throw std::runtime_error("task state reference readback identity mismatch");
            const bool 已在本场景 = 含节点(场景->状态节点组, 项.查询节点);
            if (!已在本场景 && (!项.快照->入向字段组.empty() || !项.快照->入向外部关系组.empty()))
                throw std::runtime_error("unattached task state is already consumed or attached elsewhere");
            for (const auto& 入边 : 项.快照->入向字段组) {
                const auto 引用者 = 入边.所属节点;
                if (引用者 == 场景->节点 || !已核验引用项.insert(引用者).second
                    || 核验引用动态(引用者)) continue;
                // 动态正式布局经状态项引用S：D→R→S。这里只沿完整入边
                // 发现持有者，再由动态owner解释D，不猜测R的私有字段编码。
                const auto 引用结构 = 全局基础数据集.查询节点组完整结构({引用者});
                if (引用结构.状态 != 基础结构操作状态::已完成 || 引用结构.节点组.size() != 1
                    || 引用结构.节点组.front().查询节点 != 引用者 || !引用结构.节点组.front().快照)
                    throw std::runtime_error("task state reference item unreadable");
                for (const auto& 持有 : 引用结构.节点组.front().快照->入向字段组)
                    (void)核验引用动态(持有.所属节点);
                // S也可被任务执行/结果材料合法引用；没有动态owner不能
                // 被解释为坏R。只有正式识别到D后才按动态合同严格读回。
            }
        }
        return 结果;
    } catch (const std::exception& e) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取任务特征变化",
            L"任务特征状态动态不完整或归属、值、时间不一致。", e.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取任务特征变化", L"未知工程读取异常。");
    }
}

新治理任务特征变化结果 新_内部治理世界类::完成任务特征变化(
    const 新任务虚拟存在特征引用& 目标, 稳定编码 前节点, 稳定编码 后节点) noexcept {
    try {
        if (const auto 既有 = 读取任务特征变化(目标, 前节点, 后节点)) return *既有;
        const auto 特征组 = 任务读取_->读取任务虚拟存在特征组(目标.任务引用);
        const auto 治理 = 读取任务治理场景(目标.任务引用);
        const auto 前 = 状态服务_.获取状态(前节点), 后 = 状态服务_.获取状态(后节点);
        if (!特征组 || !治理 || !前 || !后) throw std::runtime_error("task feature completion material missing");
        const auto* 特征 = 核验治理特征状态对(*特征组, 目标.特征节点, *前, *后, 特征概念服务_);
        if (!特征) throw std::runtime_error("task feature change target is a factor feature");
        if (后->固定准确值 != 新特征准确值{特征->当前值}
            || 前->被引用次数 != 0 || 后->被引用次数 != 0)
            throw std::runtime_error("new task change is stale or partly consumed; do not replay");
        核验挂接(治理场景_.添加治理状态(治理->场景节点, 前节点, *任务读取_));
        核验挂接(治理场景_.添加治理状态(治理->场景节点, 后节点, *任务读取_));
        const auto 变化 = 动态服务_.建立原子动态(目标.任务引用.虚拟存在节点, 前节点, 后节点);
        if (变化.状态 != 新动态操作状态::已完成 || !变化.动态节点)
            throw std::runtime_error("task feature atomic dynamic publication failed; status="
                + std::to_string(static_cast<int>(变化.状态)) + " pre=" + std::to_string(前节点.值)
                + " post=" + std::to_string(后节点.值) + " vt=" + std::to_string(目标.任务引用.虚拟存在节点.值));
        核验挂接(治理场景_.添加治理动态(治理->场景节点, *变化.动态节点, *任务读取_));
        const auto 读回 = 读取任务特征变化(目标, 前节点, 后节点);
        if (!读回 || 读回->变化动态节点 != *变化.动态节点)
            throw std::runtime_error("task feature completion readback failed");
        // 已知自动代码的变化不能伪装成未知动作因果；本叶只交付真实S/D。
        return *读回;
    } catch (const std::exception& e) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"完成任务特征变化",
            L"已发生治理特征变化无法完整组织；保留事实，禁止自动重放。", e.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"完成任务特征变化", L"未知工程异常；保留事实。");
    }
}

// ---- METHOD-LEARN-STATE：方法状态变化治理动作（来源＝学习记录，主体＝自我，目标＝方法虚拟存在上的特征）----
新治理动作登记结果 新_内部治理世界类::建立方法状态变化动作(稳定编码 记录节点) noexcept {
    try {
        if (!方法读取_) throw std::runtime_error("method reader is not assembled");
        const auto 记录 = 方法读取_->读取方法学习记录(记录节点);
        if (!记录 || 记录->节点 != 记录节点) throw std::runtime_error("formal learning record missing");
        const auto 定义 = 方法读取_->读取方法状态变化定义(记录->动作定义节点);
        const auto 方法 = 方法读取_->读取方法虚拟存在引用(记录->方法引用);
        const auto 前 = 状态服务_.获取状态(记录->前状态节点);
        const auto 治理 = 读取方法治理场景(记录->方法引用);
        if (!定义 || !方法 || !前 || !治理) throw std::runtime_error("method state change action material missing");
        const auto 目标 = 核验学习记录前状态(*记录, *定义, *方法, *前, 特征概念服务_);
        if (!现实场景_.存在属于场景范围(记录->现实来源场景节点, 记录->执行主体节点))
            throw std::runtime_error("method state change actor has no real scene source");
        const auto 已有 = 动态服务_.按来源查询动作(记录节点);
        if (已有.状态 != 新动态操作状态::已完成) throw std::runtime_error("method state change action lookup failed");
        if (已有.动作) {
            核验学习记录动作(*记录, *已有.动作);
            const auto 完整 = 读取方法状态变化(记录节点);
            if (!完整 || 完整->动作节点 != 已有.动作->节点)
                throw std::runtime_error("partial method state change action cannot be replayed");
            return {新治理动作登记状态::精确重复, 已有.动作->节点};
        }
        // 启用修改的联动发布在确认后才形成记录与动作，此时特征已是后值；其余变化在提交前形成。
        if (目标.second != 记录->前值 && 目标.second != 记录->后值)
            throw std::runtime_error("learning record is stale against current method feature");
        const auto 场景 = 治理场景_.获取场景(治理->场景节点);
        if (!场景 || 含节点(场景->状态节点组, 记录->前状态节点))
            throw std::runtime_error("method before state is attached without its action");
        核验挂接(治理场景_.添加治理执行主体引用(治理->场景节点,
            记录->执行主体节点, 记录->现实来源场景节点, 现实场景_));
        核验挂接(治理场景_.添加治理状态(治理->场景节点, 记录->前状态节点, *方法读取_, 特征概念服务_));
        const auto 动作 = 动态服务_.建立内部治理方法动作动态({记录->执行主体节点,
            记录->动作定义节点, 记录节点, 记录->方法引用, 记录->特征节点, 记录->动作时间}, *方法读取_);
        if (动作.状态 != 新动态操作状态::已完成 || !动作.动态节点)
            throw std::runtime_error("method state change action publication failed; status="
                + std::to_string(static_cast<int>(动作.状态)));
        核验挂接(治理场景_.添加治理动态(治理->场景节点, *动作.动态节点, *方法读取_, 特征概念服务_));
        const auto 读回 = 动态服务_.按来源查询动作(记录节点);
        if (读回.状态 != 新动态操作状态::已完成 || !读回.动作
            || 读回.动作->节点 != *动作.动态节点 || !读回.动作->动作结果组.empty())
            throw std::runtime_error("new method state change action readback failed");
        核验学习记录动作(*记录, *读回.动作);
        return {新治理动作登记状态::已建立, 动作.动态节点};
    } catch (const std::exception& e) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"建立方法状态变化动作",
            L"学习记录或治理动作不完整；保留已发布事实，不重放。", e.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"建立方法状态变化动作", L"未知工程异常；保留事实。");
    }
}

std::optional<新治理方法状态变化结果> 新_内部治理世界类::读取方法状态变化(
    稳定编码 记录节点) const noexcept {
    try {
        if (!方法读取_) throw std::runtime_error("method reader is not assembled");
        const auto 记录 = 方法读取_->读取方法学习记录(记录节点);
        if (!记录) return std::nullopt;
        const auto 定义 = 方法读取_->读取方法状态变化定义(记录->动作定义节点);
        const auto 方法 = 方法读取_->读取方法虚拟存在引用(记录->方法引用);
        const auto 前 = 状态服务_.获取状态(记录->前状态节点);
        const auto 治理 = 读取方法治理场景(记录->方法引用);
        if (记录->节点 != 记录节点 || !定义 || !方法 || !前 || !治理)
            throw std::runtime_error("learning record material is incomplete");
        const auto 目标 = 核验学习记录前状态(*记录, *定义, *方法, *前, 特征概念服务_);
        const auto 来源 = 动态服务_.按来源查询动作(记录节点);
        if (来源.状态 != 新动态操作状态::已完成) throw std::runtime_error("method state change action lookup failed");
        // 记录已发布而动作或结果尚未形成：只可能处于同一方法owner锁内的变化过程中，按未完成返回。
        if (!来源.动作) return std::nullopt;
        const auto& 动作 = *来源.动作;
        核验学习记录动作(*记录, 动作);
        if (动作.动作结果组.empty()) return std::nullopt;
        const auto 场景 = 治理场景_.获取场景(治理->场景节点);
        const auto 主体 = 读取治理参与者(治理->场景节点, 记录->执行主体节点);
        const auto 客体 = 读取治理参与者(治理->场景节点, 记录->方法引用.虚拟存在节点);
        if (!场景 || !含节点(场景->动态节点组, 动作.节点) || !主体
            || 主体->种类 != 新治理参与者种类::现实执行主体引用
            || 主体->现实来源场景节点 != 记录->现实来源场景节点
            || !现实场景_.存在属于场景范围(记录->现实来源场景节点, 记录->执行主体节点)
            || !客体 || 客体->方法引用 != 记录->方法引用
            || 动作.动作结果组.size() != 1
            || 动作.动作结果组.front().类型 != 新动作结果关系类型::导致变化)
            throw std::runtime_error("method state change action scene/result ownership mismatch");
        const auto 变化 = 动态服务_.获取动态(动作.动作结果组.front().结果动态节点);
        if (!变化 || 变化->种类 != 新动态种类::原子 || !变化->动态概念节点
            || 变化->主题存在节点 != 记录->方法引用.虚拟存在节点
            || 变化->状态节点组.size() != 2 || 变化->状态节点组.front() != 前->节点
            || !含节点(场景->动态节点组, 变化->节点))
            throw std::runtime_error("method state change result dynamic is incomplete");
        const auto 后 = 状态服务_.获取状态(变化->状态节点组.back());
        if (!后 || !含节点(场景->状态节点组, 前->节点) || !含节点(场景->状态节点组, 后->节点)
            || 后->被描述存在节点 != 记录->方法引用.虚拟存在节点
            || 后->来源实例特征节点 != 记录->特征节点
            || 后->固定准确值 != 新特征准确值{记录->后值}
            || 后->强时间.语义 != 新状态时间语义::实例绝对UTC
            || 后->强时间.纳秒 <= 记录->动作时间.纳秒)
            throw std::runtime_error("method state change after state does not match its record");
        const auto 关系 = 特征概念服务_.比较单特征概念(后->特征类型概念节点, 后->特征类型概念节点);
        if (关系.状态 != 特征概念规则状态::已完成 || 关系.关系 != 单特征概念关系::同一
            || 关系.特征类型根 != 目标.first)
            throw std::runtime_error("method state change after state has a different feature type");
        for (const auto d : 场景->动态节点组) {
            if (d == 动作.节点 || d == 变化->节点) continue;
            const auto 候选 = 动态服务_.获取动态(d);
            if (!候选) throw std::runtime_error("method scene dynamic unreadable");
            if (候选->种类 == 新动态种类::原子 && 候选->状态节点组 == 变化->状态节点组)
                throw std::runtime_error("duplicate method state-pair dynamic");
            if (候选->种类 == 新动态种类::动作)
                for (const auto& 关系项 : 候选->动作结果组)
                    if (关系项.结果动态节点 == 变化->节点)
                        throw std::runtime_error("method state change result belongs to more than one action");
        }
        return 新治理方法状态变化结果{记录节点, 场景->节点, 动作.节点, 前->节点, 后->节点, 变化->节点};
    } catch (const std::exception& e) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取方法状态变化",
            L"学习记录、动作或状态动态闭包不一致；保留现场。", e.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取方法状态变化", L"未知工程读取异常。");
    }
}

新治理方法状态变化结果 新_内部治理世界类::完成方法状态变化(
    稳定编码 记录节点, 稳定编码 后节点) noexcept {
    try {
        if (!方法读取_) throw std::runtime_error("method reader is not assembled");
        const auto 记录 = 方法读取_->读取方法学习记录(记录节点);
        if (!记录 || 记录->节点 != 记录节点) throw std::runtime_error("formal learning record missing");
        const auto 定义 = 方法读取_->读取方法状态变化定义(记录->动作定义节点);
        const auto 方法 = 方法读取_->读取方法虚拟存在引用(记录->方法引用);
        const auto 前 = 状态服务_.获取状态(记录->前状态节点), 后 = 状态服务_.获取状态(后节点);
        const auto 治理 = 读取方法治理场景(记录->方法引用);
        const auto 来源 = 动态服务_.按来源查询动作(记录节点);
        if (!定义 || !方法 || !前 || !后 || !治理
            || 来源.状态 != 新动态操作状态::已完成 || !来源.动作)
            throw std::runtime_error("method state change completion material missing");
        const auto 目标 = 核验学习记录前状态(*记录, *定义, *方法, *前, 特征概念服务_);
        核验学习记录动作(*记录, *来源.动作);
        if (后->节点 == 前->节点 || 后->被描述存在节点 != 记录->方法引用.虚拟存在节点
            || 后->来源实例特征节点 != 记录->特征节点
            || 后->固定准确值 != 新特征准确值{记录->后值}
            || 后->强时间.语义 != 新状态时间语义::实例绝对UTC
            || 后->强时间.纳秒 <= 记录->动作时间.纳秒)
            throw std::runtime_error("method state change after state differs from its record");
        if (!来源.动作->动作结果组.empty()) {
            const auto 完整 = 读取方法状态变化(记录节点);
            if (!完整 || 完整->后状态节点 != 后节点)
                throw std::runtime_error("method state change completion replay conflicts");
            return *完整; // 精确重放不重复交付。
        }
        if (目标.second != 记录->后值 || 后->被引用次数 != 0)
            throw std::runtime_error("new method state change result is stale or partly consumed");
        const auto 场景 = 治理场景_.获取场景(治理->场景节点);
        if (!场景 || !含节点(场景->动态节点组, 来源.动作->节点)
            || !含节点(场景->状态节点组, 前->节点) || 含节点(场景->状态节点组, 后节点))
            throw std::runtime_error("method state change action was not completely attached before commit");
        核验挂接(治理场景_.添加治理状态(治理->场景节点, 后节点, *方法读取_, 特征概念服务_));
        const auto 变化 = 动态服务_.建立原子动态(记录->方法引用.虚拟存在节点, 前->节点, 后节点);
        if (变化.状态 != 新动态操作状态::已完成 || !变化.动态节点)
            throw std::runtime_error("method state change result dynamic publication failed; status="
                + std::to_string(static_cast<int>(变化.状态)));
        核验挂接(治理场景_.添加治理动态(治理->场景节点, *变化.动态节点, *方法读取_, 特征概念服务_));
        if (动态服务_.关联动作结果(来源.动作->节点, *变化.动态节点,
            新动作结果关系类型::导致变化) != 新动态操作状态::已完成)
            throw std::runtime_error("method state change action result association failed");
        const auto 完整 = 读取方法状态变化(记录节点);
        if (!完整 || 完整->后状态节点 != 后节点 || 完整->变化动态节点 != *变化.动态节点)
            throw std::runtime_error("method state change completion readback failed");
        // 1190 §4.1：方法状态变化按已知动作同步交付实际发生，前置窗口规则不变。
        交付实际发生(新治理任务变化结果{完整->治理场景节点, 完整->动作节点,
            完整->前状态节点, 完整->后状态节点, 完整->变化动态节点});
        return *完整;
    } catch (const std::exception& e) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"完成方法状态变化",
            L"方法特征已提交后的治理组织或交付异常；保留动作及全部事实，不重做。", e.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"完成方法状态变化", L"未知工程异常；保留事实。");
    }
}

void 新_内部治理世界类::回收未消费方法治理场景组(
    const std::vector<新方法虚拟存在引用>& 引用组) noexcept {
    try {
        if (!方法读取_) throw std::runtime_error("method cleanup reader missing");
        std::set<稳定编码> 已查;
        // 全批只读预检；方法owner仍持方法锁，并已证明无任务/学习采用。
        for (const auto& 引用 : 引用组) {
            const auto 治理 = 读取方法治理场景(引用);
            const auto 场景 = 治理 ? 治理场景_.获取场景(治理->场景节点) : std::nullopt;
            if (!治理 || !场景 || !已查.insert(场景->节点).second
                || !场景->状态节点组.empty() || !场景->动态节点组.empty()
                || !场景->直接子场景组.empty()
                || !场景->直接成员存在组.empty() || !场景->边界存在组.empty() || !场景->特征节点组.empty()
                || 治理场景_.查询治理参与节点(场景->节点) != std::vector<稳定编码>{引用.虚拟存在节点})
                throw std::runtime_error("unadopted method scene has been consumed");
        }
        for (const auto& 引用 : 引用组) 治理场景_.回收未消费方法治理场景(引用, *方法读取_);
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"回收未消费方法治理场景",
            L"未采用闭包不能证明未被消费或场景回收失败；保留剩余事实。");
    }
}

void 新_内部治理世界类::迁移已有方法治理场景(
    const std::vector<新方法虚拟存在引用>& 引用组) noexcept {
    try {
        if (!方法读取_ || !任务读取_ || !获取根场景())
            throw std::runtime_error("method migration dependencies missing");
        std::map<稳定编码, 新方法虚拟存在引用> 方法场景;
        std::set<稳定编码> 方法组, 虚拟存在组, 特征组;
        for (const auto& 引用 : 引用组) {
            const auto 方法 = 方法读取_->读取方法虚拟存在引用(引用);
            const auto 治理 = 读取方法治理场景(引用);
            if (!方法 || 方法->引用 != 引用 || !治理
                || !方法组.insert(引用.方法节点).second || !虚拟存在组.insert(引用.虚拟存在节点).second
                || !特征组.insert(引用.启用特征节点).second
                || !方法场景.emplace(治理->场景节点, 引用).second)
                throw std::runtime_error("method migration set has incomplete or duplicate ownership");
        }
        const auto 根 = 治理场景_.获取场景(根场景节点_);
        if (!根) throw std::runtime_error("method migration root unreadable");
        std::set<稳定编码> 已查场景;
        for (const auto c : 根->直接子场景组) {
            if (!已查场景.insert(c).second) throw std::runtime_error("duplicate governance child scene");
            const auto 场景 = 治理场景_.获取场景(c);
            if (!场景 || 场景->父场景节点 != 根场景节点_ || !场景->直接子场景组.empty())
                throw std::runtime_error("governance child scene structure invalid");
            bool 已证所属 = false;
            for (const auto e : 治理场景_.查询治理参与节点(c)) {
                const auto 参与者 = 读取治理参与者(c, e);
                if (!参与者) throw std::runtime_error("governance participant missing during method migration");
                if (参与者->种类 == 新治理参与者种类::方法虚拟存在) {
                    if (!参与者->方法引用 || !方法组.contains(参与者->方法引用->方法节点))
                        throw std::runtime_error("method participant is outside complete method set");
                    const auto 所属 = 读取方法治理场景(*参与者->方法引用);
                    if (!所属) throw std::runtime_error("method participant has no owned scene");
                    if (所属->场景节点 == c) {
                        if (已证所属 || !方法场景.contains(c) || 方法场景.at(c) != *参与者->方法引用)
                            throw std::runtime_error("ambiguous method scene owner");
                        已证所属 = true;
                    }
                } else if (参与者->种类 == 新治理参与者种类::任务虚拟存在) {
                    if (!参与者->任务引用) throw std::runtime_error("task participant reference missing");
                    const auto 所属 = 读取任务治理场景(*参与者->任务引用);
                    if (!所属) throw std::runtime_error("task participant has no owned scene");
                    if (所属->场景节点 == c) {
                        if (已证所属 || 方法场景.contains(c)) throw std::runtime_error("mixed governance ownership");
                        已证所属 = true;
                    }
                } else if (参与者->种类 != 新治理参与者种类::现实执行主体引用)
                    throw std::runtime_error("unknown governance participant kind");
            }
            if (!已证所属) throw std::runtime_error("unowned governance child scene");
            if (!方法场景.contains(c)) continue; // 合法C(T)由任务owner完整迁接，不移动其事实。
            if (!场景->直接成员存在组.empty() || !场景->边界存在组.empty() || !场景->特征节点组.empty())
                throw std::runtime_error("method scene contains ordinary instances");
            const auto& 引用 = 方法场景.at(c);
            // METHOD-LEARN-STATE：方法治理场景的事实只能是学习记录来源的治理动作及其“导致变化”的状态对。
            std::set<稳定编码> 已覆盖状态, 已组织动态;
            for (const auto d : 场景->动态节点组) {
                const auto 动态 = 动态服务_.获取动态(d);
                if (!动态) throw std::runtime_error("method migration dynamic unreadable");
                if (动态->种类 == 新动态种类::原子) continue;
                if (动态->种类 != 新动态种类::动作 || 动态->动作目标方法虚拟存在 != 引用
                    || !动态->动作来源节点)
                    throw std::runtime_error("unsupported method migration dynamic");
                const auto 包 = 读取方法状态变化(*动态->动作来源节点);
                if (!包 || 包->动作节点 != d || 包->治理场景节点 != c)
                    throw std::runtime_error("method migration change package mismatch");
                已覆盖状态.insert(包->前状态节点);
                已覆盖状态.insert(包->后状态节点);
                已组织动态.insert(d);
                已组织动态.insert(包->变化动态节点);
            }
            if (已组织动态 != std::set<稳定编码>(场景->动态节点组.begin(), 场景->动态节点组.end()))
                throw std::runtime_error("method migration contains orphan dynamics");
            if (已覆盖状态 != std::set<稳定编码>(场景->状态节点组.begin(), 场景->状态节点组.end()))
                throw std::runtime_error("method migration contains orphan states");
            // 完整入边发现动作中断后未挂接的S，不用容错空查询掩盖部分事实。
            const auto 当前 = 方法读取_->读取方法虚拟存在引用(引用);
            if (!当前) throw std::runtime_error("method migration owner readback failed");
            std::vector<稳定编码> 方法特征组{引用.启用特征节点};
            if (当前->条件结果配对数) 方法特征组.push_back(当前->条件结果配对数->特征节点);
            if (当前->已知结果配对数) 方法特征组.push_back(当前->已知结果配对数->特征节点);
            const auto 结构 = 全局基础数据集.查询节点组完整结构(方法特征组);
            if (结构.状态 != 基础结构操作状态::已完成 || 结构.节点组.size() != 方法特征组.size())
                throw std::runtime_error("method feature incoming references unreadable");
            for (const auto& 项 : 结构.节点组) {
                if (!项.快照) throw std::runtime_error("method feature incoming references unreadable");
                for (const auto& 边 : 项.快照->入向字段组) {
                    if (!状态服务_.是状态节点(边.所属节点)) continue;
                    const auto 状态 = 状态服务_.获取状态(边.所属节点);
                    if (!状态 || 状态->被描述存在节点 != 引用.虚拟存在节点
                        || 状态->来源实例特征节点 != 项.查询节点 || !已覆盖状态.contains(状态->节点))
                        throw std::runtime_error("method feature has an unorganized state; preserve and stop");
                }
            }
        }
        for (const auto& [c, 引用] : 方法场景) {
            (void)引用;
            if (!已查场景.contains(c)) throw std::runtime_error("method scene missing from root");
        }
        for (const auto s : 根->状态节点组) {
            const auto 状态 = 状态服务_.获取状态(s);
            if (!状态 || 虚拟存在组.contains(状态->被描述存在节点)
                || 特征组.contains(状态->来源实例特征节点))
                throw std::runtime_error("legacy root has untraceable method state");
        }
        for (const auto d : 根->动态节点组) {
            const auto 动态 = 动态服务_.获取动态(d);
            if (!动态 || 虚拟存在组.contains(动态->主题存在节点))
                throw std::runtime_error("legacy root has untraceable method dynamic");
        }
    } catch (const std::exception& e) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"迁移方法治理场景",
            L"方法治理归属或旧事实无法完整证明；保留材料并停止启动。", e.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"迁移方法治理场景", L"未知迁接工程异常。");
    }
}

void 新_内部治理世界类::迁移已有任务治理场景(
    const std::vector<新任务虚拟存在引用>& 引用组) noexcept {
    try {
        if (!任务读取_ || !获取根场景()) throw std::runtime_error("migration dependencies missing");
        std::map<稳定编码, 新任务虚拟存在语义读回> 任务快照;
        std::map<稳定编码, 新任务虚拟存在特征组> 特征快照;
        std::map<稳定编码, 新任务治理场景信息> 任务场景;
        std::set<稳定编码> 已有场景;
        for (const auto& 引用 : 引用组) {
            const auto 语义 = 任务读取_->读取任务虚拟存在语义(引用);
            if (!语义 || 语义->状态.引用 != 引用 || !任务快照.emplace(引用.任务节点, *语义).second)
                throw std::runtime_error("migration task set is incomplete or duplicated");
            const auto 特征组 = 任务读取_->读取任务虚拟存在特征组(引用);
            if (!特征组 || 特征组->引用 != 引用)
                throw std::runtime_error("migration task features are not formally owned");
            特征快照.emplace(引用.任务节点, *特征组);
            if (const auto 场景 = 读取任务治理场景(引用)) {
                if (!已有场景.insert(场景->场景节点).second)
                    throw std::runtime_error("migration scene has multiple task owners");
                任务场景.emplace(引用.任务节点, *场景);
            }
        }
        const auto 根 = 治理场景_.获取场景(根场景节点_);
        if (!根) throw std::runtime_error("governance root unavailable");
        auto 全部已知场景 = 已有场景;
        for (const auto c : 根->直接子场景组) {
            if (已有场景.contains(c)) continue;
            bool 已证方法场景 = false;
            for (const auto e : 治理场景_.查询治理参与节点(c)) {
                const auto 参与者 = 读取治理参与者(c, e);
                if (!参与者) throw std::runtime_error("foreign governance participant unreadable");
                if (参与者->种类 != 新治理参与者种类::方法虚拟存在) continue;
                if (!参与者->方法引用) throw std::runtime_error("method participant reference missing");
                const auto 所属 = 读取方法治理场景(*参与者->方法引用);
                if (所属 && 所属->场景节点 == c) {
                    if (已证方法场景) throw std::runtime_error("method scene has duplicate ownership");
                    已证方法场景 = true;
                }
            }
            if (!已证方法场景 || !全部已知场景.insert(c).second)
                throw std::runtime_error("governance root contains unknown or duplicate scene");
        }
        if (std::set<稳定编码>(根->直接子场景组.begin(), 根->直接子场景组.end()) != 全部已知场景
            || 根->直接子场景组.size() != 全部已知场景.size())
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
                } else if (参与者->种类 == 新治理参与者种类::方法虚拟存在) {
                    if (!参与者->方法引用 || !读取方法治理场景(*参与者->方法引用))
                        throw std::runtime_error("migration method participant has no complete owned scene");
                    // C(T)/根仅引用共享Vm；不混入任务目标或任务状态动态迁接。
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
                    || !动作.动作定义节点
                    || !任务快照.contains(动作.动作目标任务虚拟存在->任务节点)
                    || 任务快照.at(动作.动作目标任务虚拟存在->任务节点).状态.引用 != *动作.动作目标任务虚拟存在)
                    throw std::runtime_error("legacy action has no reconstructible task/definition/concept");
                // 切片3：以贡献阶段为来源的动作（含持迁移定义的结算收尾J）按贡献包读回。
                const bool 贡献定义 = 任务读取_->读取任务贡献动作定义(*动作.动作定义节点).has_value();
                if (贡献定义 || 任务读取_->读取任务贡献动作阶段(*动作.动作来源节点)) {
                    // 新贡献包只在独占C(T)中成立，不扩张旧Cg迁移白名单。
                    if (c == 根场景节点_) throw std::runtime_error("contribution action cannot be migrated from governance root");
                    const auto 变化 = 读取任务特征动作(*动作.动作来源节点);
                    if (!变化 || 变化->动作节点 != d || 变化->治理场景节点 != c
                        || 变化->目标.任务引用 != *动作.动作目标任务虚拟存在
                        || !动态快照.contains(变化->变化动态节点)
                        || !动态快照.at(变化->变化动态节点).动态概念节点)
                        throw std::runtime_error("contribution action package is incomplete");
                    包状态.insert(变化->前状态节点);
                    包状态.insert(变化->后状态节点);
                    for (const auto n : {变化->动作节点, 变化->变化动态节点})
                        if (!包动态.insert(n).second)
                            throw std::runtime_error("contribution dynamic belongs to multiple packages");
                    // 不加入旧动作包；它有自己的阶段来源。持迁移定义的结算收尾J已交付因果，发生身份归本场景。
                    if (!贡献定义) 动作所属场景.emplace(d, c);
                    continue;
                }
                if (任务读取_->读取任务状态迁移定义(*动作.动作定义节点)) {
                    // TASK-STATE-CAUSAL：迁移包只在独占C(T)中成立；不进旧包，但其交付的因果发生身份归本场景。
                    if (c == 根场景节点_) throw std::runtime_error("migration action cannot be migrated from governance root");
                    const auto 变化 = 读取任务状态迁移(*动作.动作来源节点);
                    if (!变化 || 变化->动作节点 != d || 变化->治理场景节点 != c
                        || !动态快照.contains(变化->变化动态节点)
                        || !动态快照.at(变化->变化动态节点).动态概念节点)
                        throw std::runtime_error("migration action package is incomplete");
                    包状态.insert(变化->前状态节点);
                    包状态.insert(变化->后状态节点);
                    for (const auto n : {变化->动作节点, 变化->变化动态节点})
                        if (!包动态.insert(n).second)
                            throw std::runtime_error("migration dynamic belongs to multiple packages");
                    动作所属场景.emplace(d, c);
                    continue;
                }
                if (!正式动作定义 || 动作.动作定义节点 != 正式动作定义)
                    throw std::runtime_error("unrecognized governance action definition");
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
                    if (!包状态.insert(s).second && c == 根场景节点_)
                        throw std::runtime_error("legacy state belongs to multiple action packages");
                for (const auto n : {变化->动作节点, 变化->变化动态节点})
                    if (!包动态.insert(n).second) throw std::runtime_error("legacy dynamic belongs to multiple action packages");
                动作所属场景.emplace(d, c);
                包组.push_back({*动作.动作目标任务虚拟存在, *变化, *主体, *动作.动作来源节点});
            }
            if (c != 根场景节点_) {
                const auto owner = std::find_if(任务场景.begin(), 任务场景.end(),
                    [&](const auto& 项) { return 项.second.场景节点 == c; });
                if (owner == 任务场景.end()) throw std::runtime_error("task scene has no registered owner");
                const auto& 特征组 = 特征快照.at(owner->first);
                std::set<std::pair<稳定编码, 稳定编码>> 已见配对;
                for (const auto d : 场景->动态节点组) {
                    const auto& 动态 = 动态快照.at(d);
                    if (动态.种类 == 新动态种类::动作) continue;
                    if (动态.种类 != 新动态种类::原子 || 动态.状态节点组.size() != 2
                        || 动态.主题存在节点 != 特征组.引用.虚拟存在节点 || !动态.动态概念节点
                        || !状态快照.contains(动态.状态节点组[0])
                        || !状态快照.contains(动态.状态节点组[1])
                        || !含节点(场景->状态节点组, 动态.状态节点组[0])
                        || !含节点(场景->状态节点组, 动态.状态节点组[1])
                        || !已见配对.emplace(动态.状态节点组[0], 动态.状态节点组[1]).second)
                        throw std::runtime_error("task scene has incomplete or duplicate atomic change");
                    const auto& 前 = 状态快照.at(动态.状态节点组[0]);
                    const auto& 后 = 状态快照.at(动态.状态节点组[1]);
                    (void)核验治理特征状态对(特征组, 前.来源实例特征节点, 前, 后, 特征概念服务_);
                    包状态.insert(前.节点);
                    包状态.insert(后.节点); // 合法历史D可以共享端点。
                    包动态.insert(d); // 9→12的D已覆盖，不再生成或交付因果。
                }
                // 切片2：本任务因素特征的单个状态属于C(T)自身事实，不进迁移包。
                for (const auto s : 场景->状态节点组) {
                    if (包状态.contains(s) || !特征组.查找因素(状态快照.at(s).来源实例特征节点)) continue;
                    (void)核验治理因素状态(特征组, 状态快照.at(s), 特征概念服务_);
                    包状态.insert(s);
                }
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
    std::vector<std::pair<稳定编码, std::optional<稳定编码>>> 待查{{范围, std::nullopt}};
    std::set<稳定编码> 已查;
    std::optional<新治理场景参与者> 结果;
    while (!待查.empty()) {
        const auto [c, 父] = 待查.back();
        待查.pop_back();
        if (!已查.insert(c).second) throw std::runtime_error("governance scene range contains a cycle or duplicate");
        const auto 场景 = 治理场景_.获取场景(c);
        if (!场景) {
            if (!父) return std::nullopt;
            const auto 父当前 = 治理场景_.获取场景(*父);
            if (父当前 && !含节点(父当前->直接子场景组, c)) continue;
            throw std::runtime_error("governance scene range has a dangling child");
        }
        if (auto 当前 = 读取治理参与者(c, 节点)) {
            当前->场景节点 = 范围; // 仅合并值式投影，公共本级读取的场景身份不变。
            if (结果 && *结果 != *当前) throw std::runtime_error("governance participant projections conflict across scenes");
            结果 = *当前;
        }
        for (const auto 子 : 场景->直接子场景组) 待查.emplace_back(子, c);
    }
    return 结果;
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
        if (!任务读取_ || !获取根场景()) throw std::runtime_error("governance state reader is not initialized");
        const auto 来源 = 动态服务_.按来源查询动作(来源普通动作);
        if (来源.状态 != 新动态操作状态::已完成) throw std::runtime_error("governance source read failed");
        if (!来源.动作) return std::nullopt;
        const auto& 动作 = *来源.动作;
        const auto 当前 = 任务读取_->读取任务虚拟存在引用(目标);
        const auto 场景 = 治理场景_.获取场景(本场景);
        const auto 正式定义 = 任务读取_->获取任务已结算动作定义();
        // R108：重启或放弃后旧“已结算”动作仍可读；后值为已结算由动作结果自身核验，不要求任务当前状态。
        if (!当前 || 当前->引用 != 目标
            || !场景 || 动作.种类 != 新动态种类::动作 || !动作.动态概念节点
            || 动作.动作目标种类 != 新动作目标种类::任务虚拟存在
            || 动作.动作目标任务虚拟存在 != 目标 || !动作.动作时间
            || 动作.动作目标存在节点 || 动作.动作目标特征节点 != 目标.状态特征节点
            || !正式定义 || 动作.动作定义节点 != 正式定义 || 动作.动作来源节点 != 来源普通动作
            || 动作.动作时间->语义 != 新状态时间语义::实例绝对UTC
            || !含节点(场景->动态节点组, 动作.节点) || 动作.动作结果组.size() != 1
            || 动作.动作结果组.front().类型 != 新动作结果关系类型::导致变化)
            throw std::runtime_error("governance action readback mismatch");
        const auto 主体 = 读取治理参与者(本场景, 动作.主题存在节点);
        const auto 客体 = 读取治理参与者(本场景, 目标.虚拟存在节点);
        if (!主体 || 主体->种类 != 新治理参与者种类::现实执行主体引用
            || !主体->现实来源场景节点
            || !现实场景_.存在属于场景范围(*主体->现实来源场景节点, 动作.主题存在节点)
            || !客体 || 客体->种类 != 新治理参与者种类::任务虚拟存在 || 客体->任务引用 != 目标)
            throw std::runtime_error("governance actor/target mismatch");
        const auto 普通来源 = 动态服务_.按来源查询动作(目标.任务节点);
        const auto& 普通动作 = 普通来源.动作;
        const auto 现实 = 现实场景_.获取场景(*主体->现实来源场景节点);
        if (普通来源.状态 != 新动态操作状态::已完成 || !普通动作 || 普通动作->节点 != 来源普通动作
            || !现实 || !含节点(现实->动态节点组, 来源普通动作)
            || 普通动作->种类 != 新动态种类::动作 || !普通动作->动态概念节点
            || !普通动作->动作定义节点 || 普通动作->动作目标种类 != 新动作目标种类::普通存在
            || 普通动作->动作来源节点 != 目标.任务节点
            || 普通动作->主题存在节点 != 动作.主题存在节点 || !普通动作->动作时间
            || 普通动作->动作时间->语义 != 新状态时间语义::实例绝对UTC
            || 普通动作->动作时间->纳秒 >= 动作.动作时间->纳秒)
            throw std::runtime_error("legacy governance real action source/time/concept mismatch");
        const auto 变化 = 动态服务_.获取动态(动作.动作结果组.front().结果动态节点);
        if (!变化 || 变化->种类 != 新动态种类::原子 || !变化->动态概念节点
            || 变化->主题存在节点 != 目标.虚拟存在节点
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
        for (const auto 概念 : {前->特征类型概念节点, 后->特征类型概念节点}) {
            const auto 关系 = 特征概念服务_.比较单特征概念(概念, 概念);
            if (关系.状态 != 特征概念规则状态::已完成 || 关系.关系 != 单特征概念关系::同一
                || 关系.特征类型根 != 当前->状态特征类型概念节点)
                throw std::runtime_error("legacy governance state concept has a different type root");
        }
        for (const auto d : 场景->动态节点组) {
            if (d == 动作.节点 || d == 变化->节点) continue;
            const auto 候选 = 动态服务_.获取动态(d);
            if (!候选) throw std::runtime_error("legacy governance scene dynamic unreadable");
            if (候选->种类 == 新动态种类::原子 && 候选->状态节点组 == 变化->状态节点组)
                throw std::runtime_error("duplicate legacy governance state-pair dynamic");
            if (候选->种类 != 新动态种类::动作) continue;
            if (候选->动作目标任务虚拟存在 == 目标 && 候选->动作定义节点 == 正式定义)
                throw std::runtime_error("task has more than one legacy settlement action");
            for (const auto& 关系 : 候选->动作结果组)
                if (关系.结果动态节点 == 变化->节点)
                    throw std::runtime_error("legacy governance result belongs to more than one action");
        }
        return 新治理任务变化结果{本场景, 动作.节点, 前->节点, 后->节点, 变化->节点};
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取任务状态变化", L"治理变化已有动作但证据不完整或不一致。");
    }
}

std::optional<新因果动作定义> 新_内部治理世界类::读取自我动作事实(稳定编码 节点) const noexcept {
    try {
        const auto 动作 = 动态服务_.获取动态(节点);
        if (!动作 || 动作->种类 != 新动态种类::动作) return std::nullopt;
        if (动作->动作目标种类 == 新动作目标种类::方法虚拟存在) {
            // METHOD-LEARN-STATE（1190 §4.1）：方法状态变化治理动作按已知动作交付，作用对象为方法虚拟存在。
            if (!动作->动作目标方法虚拟存在 || !动作->动态概念节点 || !动作->动作来源节点 || !方法读取_)
                throw std::runtime_error("method governance action projection missing");
            const auto 治理 = 读取方法治理场景(*动作->动作目标方法虚拟存在);
            const auto 场景 = 治理 ? 治理场景_.获取场景(治理->场景节点) : std::nullopt;
            if (!场景 || !含节点(场景->动态节点组, 节点)) throw std::runtime_error("method governance action scene mismatch");
            const auto 主体 = 读取治理参与者(场景->节点, 动作->主题存在节点);
            const auto 客体 = 读取治理参与者(场景->节点, 动作->动作目标方法虚拟存在->虚拟存在节点);
            if (!主体 || 主体->种类 != 新治理参与者种类::现实执行主体引用
                || !客体 || 客体->方法引用 != 动作->动作目标方法虚拟存在)
                throw std::runtime_error("method governance causal participants mismatch");
            if (!动作->动作定义节点 || !方法读取_->读取方法状态变化定义(*动作->动作定义节点))
                throw std::runtime_error("method governance action has no formal state change definition");
            const auto 完整 = 读取方法状态变化(*动作->动作来源节点);
            if (!完整 || 完整->动作节点 != 节点 || 完整->治理场景节点 != 场景->节点)
                throw std::runtime_error("method state change causal projection is not complete");
            新因果动作定义 结果;
            结果.动作概念节点 = *动作->动态概念节点;
            结果.执行主体存在节点 = 动作->主题存在节点;
            结果.作用对象方法虚拟存在 = 动作->动作目标方法虚拟存在;
            结果.触发方式 = 新因果动作触发方式::被动触发;
            return 结果;
        }
        if (动作->动作目标种类 != 新动作目标种类::任务虚拟存在) return std::nullopt;
        if (!动作->动作目标任务虚拟存在 || !动作->动态概念节点)
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
        } else if (动作->动作定义节点 && 动作->动作来源节点
            && 任务读取_->读取任务状态迁移定义(*动作->动作定义节点)
            && 任务读取_->读取任务贡献动作阶段(*动作->动作来源节点)) {
            // 切片3：结算收尾J（直属/最终）为“已完成→已结算”迁移记录，按贡献包读回。
            const auto 完整 = 读取任务特征动作(*动作->动作来源节点);
            if (!完整 || 完整->动作节点 != 节点 || 完整->治理场景节点 != 场景->节点)
                throw std::runtime_error("settlement migration causal projection is not complete");
            结果.触发方式 = 新因果动作触发方式::被动触发;
        } else if (动作->动作定义节点
            && 任务读取_->读取任务状态迁移定义(*动作->动作定义节点)) {
            // TASK-STATE-CAUSAL：任务状态迁移治理动作（来源＝治理迁移记录），按已知动作交付（1190 §4.1）。
            if (!动作->动作来源节点) throw std::runtime_error("migration action record missing");
            const auto 完整 = 读取任务状态迁移(*动作->动作来源节点);
            if (!完整 || 完整->动作节点 != 节点 || 完整->治理场景节点 != 场景->节点)
                throw std::runtime_error("migration causal projection is not complete");
            结果.触发方式 = 新因果动作触发方式::被动触发;
        } else if (动作->动作定义节点
            && 任务读取_->读取任务贡献动作定义(*动作->动作定义节点)) {
            if (!动作->动作来源节点) throw std::runtime_error("contribution action phase missing");
            const auto 完整 = 读取任务特征动作(*动作->动作来源节点);
            if (!完整 || 完整->动作节点 != 节点 || 完整->治理场景节点 != 场景->节点)
                throw std::runtime_error("contribution causal projection is not complete");
            结果.触发方式 = 新因果动作触发方式::被动触发;
        } else {
            throw std::runtime_error("governance causal action has no recognized formal definition");
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
