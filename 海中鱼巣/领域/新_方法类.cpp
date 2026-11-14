#include "新_方法类.h"
#include "../核心/容错检查.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <limits>
#include <map>
#include <memory>
#include <mutex>
#include <new>
#include <set>
#include <stdexcept>
#include <thread>
#include <tuple>
#include <utility>
#ifdef HY_EGO_DEBUG_LOG_METHOD_LOOKUP
#include <sstream>
#ifdef HY_EGO_DEBUG_LOG_SIMULATION_PROGRESS
#include "../核心/日志系统.h"
#endif
#endif

#include "新_存在类.h"
#include "新_场景类.h"
#include "新_特征类.h"
#include "新_状态类.h"
#include "新_本能函数集.h"
#include "新_本能函数管理类.h"
#include "概念_存在类.h"
#include "概念_特征类.h"
#include "概念_二次特征类.h"
#include "消息交互.数据.h"
#include "../核心/容错检查.h"

namespace 海中鱼巣 {

稳定编码 新方法组织根节点{};

namespace {

std::recursive_mutex 新方法互斥;

bool 同一学习条件(const 新方法条件定义& 左, const 新方法条件定义& 右) {
    if (左.关系规则 != 右.关系规则 || !std::is_permutation(左.场景事实限定组.begin(), 左.场景事实限定组.end(), 右.场景事实限定组.begin(), 右.场景事实限定组.end()) || !std::is_permutation(左.参数关系组.begin(), 左.参数关系组.end(), 右.参数关系组.begin(), 右.参数关系组.end())) return false;
    if (左.关系规则 == 新方法项关系规则::全部成立
        || 左.关系规则 == 新方法项关系规则::任一成立) {
        return std::is_permutation(左.条件项组.begin(), 左.条件项组.end(),
            右.条件项组.begin(), 右.条件项组.end());
    }
    return 左 == 右;
}
bool 同义结果(const 新方法结果定义& 左,const 新方法结果定义& 右) {
    if(!std::is_permutation(左.场景事实限定组.begin(),左.场景事实限定组.end(),右.场景事实限定组.begin(),右.场景事实限定组.end()))return false;
    auto a=左,b=右;a.场景事实限定组.clear();b.场景事实限定组.clear();return a==b;
}

稳定编码 节点类型字段{};
稳定编码 方法组织根类型{};
稳定编码 方法节点类型{};
稳定编码 混合方法实例根类型{};
稳定编码 混合方法实例成员字段{};
稳定编码 混合方法候选入口字段{};
稳定编码 方法头节点类型{};
稳定编码 方法虚拟存在节点类型{};
稳定编码 参数规格节点类型{};
稳定编码 参数特征规格节点类型{};
稳定编码 条件结果配对节点类型{};
稳定编码 条件节点类型{};
稳定编码 条件项节点类型{};
稳定编码 结果节点类型{};
稳定编码 结果项节点类型{};
稳定编码 组合步骤节点类型{};
稳定编码 结果转移节点类型{};

稳定编码 方法成员字段{};
稳定编码 方法作用域字段{};
稳定编码 方法实现方式字段{};
稳定编码 方法启用状态字段{};
稳定编码 方法头字段{};
稳定编码 方法虚拟存在字段{};
稳定编码 方法条件结果字段{};
稳定编码 方法本能函数登记名字段{};
稳定编码 方法组合步骤字段{};
稳定编码 方法组合入口步骤字段{};
稳定编码 方法结果转移字段{};
稳定编码 方法来源字段{};
稳定编码 方法实例根字段{};
// 单独扩展，不能因存量架构没有该键而重建整个方法根。
稳定编码 方法父代字段{};
稳定编码 方法旧网页退役成员字段{};
稳定编码 方法行为字段{};
稳定编码 虚拟存在启用特征字段{};
稳定编码 虚拟存在专属概念字段{};
稳定编码 方法启用特征类型{};
// METHOD-LEARN-STATE：方法虚拟存在的配对数特征（序号0条件结果配对数、1已知结果配对数）与方法状态治理结构
// （状态变化定义、学习记录）。独立扩展，不加入既有方法架构必需编码组，也不挂方法组织根。
std::array<稳定编码, 2> 方法配对数特征类型{};
std::array<稳定编码, 2> 虚拟存在配对数特征字段{};
稳定编码 方法状态治理根{}, 方法状态治理根类型{};
稳定编码 状态变化定义类型{}, 状态变化定义成员字段{}, 定义学习种类字段{}, 定义特征种类字段{}, 定义角色字段{};
稳定编码 状态变化角色类型{}, 角色所属定义字段{}, 角色规格字段{};
稳定编码 学习记录类型{}, 学习记录成员字段{}, 记录方法字段{}, 记录虚拟存在字段{}, 记录启用特征字段{},
    记录学习种类字段{}, 记录特征种类字段{}, 记录特征字段{}, 记录前值字段{}, 记录后值字段{},
    记录前状态字段{}, 记录时间字段{}, 记录动作定义字段{}, 记录主体字段{}, 记录现实场景字段{},
    记录来源材料字段{};
std::map<std::pair<std::int64_t, std::int64_t>, 稳定编码> 方法状态变化定义索引;

稳定编码 方法头作用存在概念字段{};
稳定编码 方法头特征类型概念字段{};
稳定编码 方法头结果概念字段{};

// 独立扩展，不加入既有方法架构必需编码组，也不挂方法组织根。
稳定编码 缺失方法列表节点{}, 缺失方法列表类型{}, 缺失方法成员字段{};
稳定编码 缺失方法值域字段{}, 缺失方法比较字段{}, 缺失方法方向字段{};
稳定编码 缺失方法需求字段{}, 缺失方法任务字段{};

// 独立直接调用来源组织；不加入可召回方法根，也不迁移旧方法结构。
稳定编码 直接调用根{}, 直接调用根类型{}, 直接调用类型{}, 直接调用参数类型{};
稳定编码 直接调用成员字段{}, 直接调用方法字段{}, 直接调用现实场景字段{};
稳定编码 直接调用外设场景字段{}, 直接调用参数字段{}, 直接调用序号字段{};
稳定编码 直接调用信息类型字段{}, 直接调用类别概念字段{}, 直接调用信息引用字段{}, 直接调用动作字段{};
稳定编码 直接调用收束字段{};

void 确保直接调用结构() {
    if (有效(直接调用根) && !有效(直接调用类别概念字段)) {
        直接调用类别概念字段 = 全局基础数据集.新建节点();
        if (!有效(直接调用类别概念字段))
            throw std::runtime_error("direct invocation category key unavailable");
    }
    std::vector<稳定编码*> 键{&直接调用根类型, &直接调用类型, &直接调用参数类型,
        &直接调用成员字段, &直接调用方法字段, &直接调用现实场景字段,
        &直接调用外设场景字段, &直接调用参数字段, &直接调用序号字段,
        &直接调用信息类型字段, &直接调用类别概念字段, &直接调用信息引用字段,
        &直接调用动作字段, &直接调用收束字段};
    if (!有效(直接调用根)) {
        if (std::any_of(键.begin(), 键.end(), [](auto* p) { return 有效(*p); }))
            throw std::runtime_error("partial direct invocation schema");
        std::vector<基础组内新节点> 组(键.size() + 1);
        组.back().字段组.push_back({节点类型字段, 基础组内节点引用{0}});
        const auto r = 全局基础数据集.原子建立节点组(组);
        if (r.状态 != 基础结构操作状态::已完成 || r.节点组.size() != 组.size())
            throw std::runtime_error("direct invocation schema publication failed");
        for (std::size_t i = 0; i < 键.size(); ++i) *键[i] = r.节点组[i];
        直接调用根 = r.节点组.back();
    }
    for (auto* p : 键)
        if (!全局基础数据集.查询节点(*p))
            throw std::runtime_error("direct invocation schema key missing");
    std::set<稳定编码> 唯一键;
    for (const auto* p : 键)
        if (!唯一键.insert(*p).second || *p == 直接调用根 || *p == 节点类型字段)
            throw std::runtime_error("direct invocation schema key collision");
    const auto 类型 = 全局基础数据集.查询字段(直接调用根, 节点类型字段);
    if (类型.size() != 1 || 类型.front().内容 != 基础字段内容{直接调用根类型}
        || !全局基础数据集.查询目标关系(直接调用根, 基础外部关系类型::父子).empty())
        throw std::runtime_error("direct invocation root damaged");
}

void 确保缺失方法列表() {
    std::vector<稳定编码*> 键{&缺失方法列表类型, &缺失方法成员字段,
        &缺失方法值域字段, &缺失方法比较字段, &缺失方法方向字段,
        &缺失方法需求字段, &缺失方法任务字段};
    if (!有效(缺失方法列表节点)) {
        if (std::any_of(键.begin(), 键.end(), [](auto p) { return 有效(*p); }))
            throw std::runtime_error("partial missing-method schema");
        // 类型键与根一起出生；组内引用只在这次原子调用内存在。
        std::vector<基础组内新节点> 组(键.size() + 1);
        组.back().字段组.push_back({节点类型字段, 基础组内节点引用{0}});
        const auto r = 全局基础数据集.原子建立节点组(组);
        if (r.状态 != 基础结构操作状态::已完成 || r.节点组.size() != 组.size())
            throw std::runtime_error("missing-method schema publication failed");
        for (std::size_t i = 0; i < 键.size(); ++i) *键[i] = r.节点组[i];
        缺失方法列表节点 = r.节点组.back();
    }
    for (auto p : 键)
        if (!全局基础数据集.查询节点(*p)) throw std::runtime_error("missing-method schema damaged");
    const auto 类型 = 全局基础数据集.查询字段(缺失方法列表节点, 节点类型字段);
    if (类型.size() != 1 || 类型.front().内容 != 基础字段内容{缺失方法列表类型}
        || 缺失方法列表节点 == 新方法组织根节点
        || !全局基础数据集.查询目标关系(缺失方法列表节点, 基础外部关系类型::父子).empty())
        throw std::runtime_error("missing-method list ownership damaged");
}

稳定编码 虚拟存在所属方法字段{};
稳定编码 虚拟存在参数规格字段{};
稳定编码 参数角色字段{};
稳定编码 参数存在概念字段{};
稳定编码 参数要求场景字段{};
稳定编码 参数对象种类字段{};
稳定编码 参数特征规格字段{};
稳定编码 参数特征类型字段{};
稳定编码 参数特征值域字段{};
稳定编码 参数二次特征概念字段{};
稳定编码 参数集合角色字段{};
稳定编码 参数集合特征类型字段{};
// 独立扩展键；旧结构不因缺少新格式字段而被重新分配或解释。
稳定编码 有序参数序号字段{}, 有序参数类型字段{}, 有序参数类别概念字段{}, 有序参数范围字段{};
// R97：存在位置必需特征类型（多引用）与特征位置所属存在位置（I64）。
稳定编码 有序参数必需特征类型字段{}, 有序参数所属位置字段{};

稳定编码 配对条件字段{};
稳定编码 配对结果字段{};
稳定编码 配对命中次数字段{};
稳定编码 配对线性关系组字段{};
稳定编码 配对来源字段{};
稳定编码 关系规则字段{};
稳定编码 参数位置关系字段{}, 模拟事件位置字段{}, 配对知识来源字段{};
稳定编码 模拟输入方案字段{},模拟输入方案节点类型{},模拟消息输入字段{},模拟消息输入节点类型{};
稳定编码 模拟消息参数位置字段{},模拟消息方向字段{},模拟正文范围字段{},模拟正文非空字段{},模拟输入时机字段{},模拟触发位置方向字段{};
稳定编码 初始必要条件字段{}, 初始声明节点类型{}, 初始声明项节点类型{}, 初始场景限定节点类型{};
稳定编码 登记结果组字段{};
稳定编码 执行分支字段{},分支节点类型{},分支所属方法字段{},分支所属虚拟字段{},分支条件字段{},配对分支来源字段{};
稳定编码 结果场景限定字段{},结果场景限定节点类型{},结果场景所属字段{},结果场景关系概念字段{},结果场景四位置字段{};
稳定编码 初始声明格式字段{}, 初始声明所属字段{}, 初始场景限定字段{}, 初始关系概念字段{}, 初始四位置字段{};
稳定编码 条件项字段{};
稳定编码 条件项存在概念字段{};
稳定编码 条件项特征类型字段{};
稳定编码 条件项特征值域字段{};
稳定编码 条件项二次特征字段{};
稳定编码 条件项状态动态字段{};
稳定编码 条件引用内容要求字段{}, 条件引用内容项字段{};
稳定编码 条件内容作用点字段{}, 条件前态当前字段{}, 条件前态历史数字段{};
稳定编码 条件内容认知有效字段{};
稳定编码 结果引用内容要求字段{}, 结果引用内容项字段{}, 结果内容作用点字段{};
稳定编码 内容关系成员字段{}, 内容关系节点类型{}, 内容关系所属字段{};
稳定编码 内容关系左字段{}, 内容关系右字段{}, 内容关系路径字段{};
稳定编码 结果项字段{};
稳定编码 结果项特征类型字段{};
稳定编码 结果项特征值域字段{};
稳定编码 结果项变化方向字段{};
稳定编码 结果项状态动态字段{};
稳定编码 结果项主要结果字段{};
稳定编码 结果被动需求目标字段{};
稳定编码 结果作用存在字段{};
稳定编码 结果必要证据质量字段{};
稳定编码 结果最大等待纳秒字段{};
稳定编码 结果等待条件概念字段{};
稳定编码 结果多证据关系字段{};
稳定编码 结果禁止关系规则字段{};
稳定编码 结果禁止项字段{};
稳定编码 项精确数值域字段{};
稳定编码 任务成员变量字段{};
稳定编码 顺序字段{};

稳定编码 步骤所属组合方法字段{};
稳定编码 步骤调用方法字段{};
稳定编码 转移所属组合方法字段{};
稳定编码 转移当前步骤字段{};
稳定编码 转移当前结果字段{};
稳定编码 转移匹配结果概念字段{};
稳定编码 转移下一步骤字段{};

std::vector<稳定编码*> 架构编码组() {
    return {
        &新方法组织根节点, &节点类型字段, &方法组织根类型,
        &方法节点类型, &混合方法实例根类型, &混合方法实例成员字段,
        &混合方法候选入口字段, &方法头节点类型, &方法虚拟存在节点类型,
        &参数规格节点类型, &参数特征规格节点类型,
        &条件结果配对节点类型, &条件节点类型, &条件项节点类型,
        &结果节点类型, &结果项节点类型, &组合步骤节点类型,
        &结果转移节点类型, &方法成员字段, &方法作用域字段,
        &方法实现方式字段, &方法启用状态字段, &方法头字段,
        &方法虚拟存在字段, &方法条件结果字段,
        &方法本能函数登记名字段, &方法组合步骤字段,
        &方法组合入口步骤字段, &方法结果转移字段,
        &方法来源字段, &方法实例根字段,
        &方法头作用存在概念字段, &方法头特征类型概念字段,
        &方法头结果概念字段, &虚拟存在所属方法字段,
        &虚拟存在参数规格字段, &参数角色字段, &参数存在概念字段,
        &参数要求场景字段, &参数对象种类字段,
        &参数特征规格字段, &参数特征类型字段,
        &参数特征值域字段, &参数二次特征概念字段,
        &配对条件字段, &配对结果字段, &配对命中次数字段, &配对线性关系组字段, &配对来源字段, &关系规则字段,
        &条件项字段, &条件项存在概念字段, &条件项特征类型字段,
        &条件项特征值域字段, &条件项二次特征字段,
        &条件项状态动态字段, &结果项字段, &结果项特征类型字段,
        &结果项特征值域字段, &结果项变化方向字段,
         &结果项状态动态字段, &结果项主要结果字段,
         &结果必要证据质量字段, &结果最大等待纳秒字段,
         &结果等待条件概念字段, &结果多证据关系字段,
         &结果禁止关系规则字段, &结果禁止项字段, &项精确数值域字段, &顺序字段,
        &步骤所属组合方法字段, &步骤调用方法字段,
        &转移所属组合方法字段, &转移当前步骤字段,
        &转移当前结果字段, &转移匹配结果概念字段,
        &转移下一步骤字段, &参数集合角色字段, &参数集合特征类型字段};
}

bool 节点仍存在(稳定编码 节点) noexcept {
    return 有效(节点) && 全局基础数据集.查询节点(节点).has_value();
}

bool 是作用域(std::int64_t 值) noexcept {
    return 值 == static_cast<std::int64_t>(新方法作用域::内部治理)
        || 值 == static_cast<std::int64_t>(新方法作用域::普通世界);
}

bool 是实现方式(std::int64_t 值) noexcept {
    return 值 == static_cast<std::int64_t>(新方法实现方式::本能)
        || 值 == static_cast<std::int64_t>(新方法实现方式::组合);
}

bool 是方法行为(std::int64_t 值) noexcept {
    return 值 == static_cast<std::int64_t>(新方法行为::读取)
        || 值 == static_cast<std::int64_t>(新方法行为::改变);
}

bool 行为结果相容(const std::optional<新方法行为>& 行为, const 新方法结果定义& 结果) {
    return 行为 != 新方法行为::读取 || std::none_of(结果.结果项组.begin(),
        结果.结果项组.end(), [](const auto& 项) { return 项.变化方向概念节点.has_value(); });
}

bool 通用概念必要声明形状有效(const 新方法初始必要条件定义&);
bool 行为声明有效(const 新方法知识建立请求& 请求) {
    return (!请求.初始必要条件 || 通用概念必要声明形状有效(*请求.初始必要条件))
        && (!请求.行为 || 是方法行为(static_cast<std::int64_t>(*请求.行为)))
        && std::all_of(请求.条件结果组.begin(), 请求.条件结果组.end(),
            [&](const auto& 配对) { return 行为结果相容(请求.行为, 配对.结果); });
}

bool 是关系规则(std::int64_t 值) noexcept {
    return 值 >= static_cast<std::int64_t>(新方法项关系规则::单项)
        && 值 <= static_cast<std::int64_t>(
            新方法项关系规则::主要结果及允许副作用);
}

bool 是必要证据质量(std::int64_t 值) noexcept {
    return 值 >= static_cast<std::int64_t>(
               新方法必要证据质量::正式间接证据)
        && 值 <= static_cast<std::int64_t>(
               新方法必要证据质量::正式直接事实);
}

bool 是多证据关系(std::int64_t 值) noexcept {
    return 值 >= static_cast<std::int64_t>(新方法多证据关系::任一满足)
        && 值 <= static_cast<std::int64_t>(新方法多证据关系::优先备用);
}

struct 唯一节点字段记录 final {
    稳定编码 关系;
    稳定编码 目标;
};

struct 唯一I64字段记录 final {
    稳定编码 关系;
    std::int64_t 值 = 0;
};

std::optional<唯一节点字段记录> 读取唯一节点字段记录(
    稳定编码 节点, 稳定编码 字段) {
    const auto 组 = 全局基础数据集.查询字段(节点, 字段);
    if (组.size() != 1) return std::nullopt;
    const auto* 目标 = std::get_if<稳定编码>(&组.front().内容);
    if (!目标 || !节点仍存在(*目标)) return std::nullopt;
    return 唯一节点字段记录{组.front().编码, *目标};
}

std::optional<稳定编码> 读取唯一节点字段(
    稳定编码 节点, 稳定编码 字段) {
    const auto 记录 = 读取唯一节点字段记录(节点, 字段);
    return 记录 ? std::optional<稳定编码>{记录->目标} : std::nullopt;
}

std::optional<稳定编码> 读取可选节点字段(
    稳定编码 节点, 稳定编码 字段) {
    const auto 组 = 全局基础数据集.查询字段(节点, 字段);
    if (组.empty()) return 稳定编码{};
    if (组.size() != 1) return std::nullopt;
    const auto* 目标 = std::get_if<稳定编码>(&组.front().内容);
    if (!目标 || !节点仍存在(*目标)) return std::nullopt;
    return *目标;
}

std::optional<唯一I64字段记录> 读取唯一I64字段记录(
    稳定编码 节点, 稳定编码 字段) {
    const auto 组 = 全局基础数据集.查询字段(节点, 字段);
    if (组.size() != 1) return std::nullopt;
    const auto* 值 = std::get_if<基础值>(&组.front().内容);
    const auto* I64 = 值 ? std::get_if<std::int64_t>(&值->材料) : nullptr;
    return I64 ? std::optional<唯一I64字段记录>{
        {组.front().编码, *I64}} : std::nullopt;
}

std::optional<std::int64_t> 读取唯一I64字段(
    稳定编码 节点, 稳定编码 字段) {
    const auto 记录 = 读取唯一I64字段记录(节点, 字段);
    return 记录 ? std::optional<std::int64_t>{记录->值} : std::nullopt;
}

// 外层optional表示字段结构是否可读，内层optional表示该字段是否缺省。
std::optional<std::optional<std::int64_t>> 读取可选I64字段(
    稳定编码 节点, 稳定编码 字段) {
    const auto 组 = 全局基础数据集.查询字段(节点, 字段);
    if (组.empty()) return std::optional<std::int64_t>{};
    if (组.size() != 1) return std::nullopt;
    const auto* 值 = std::get_if<基础值>(&组.front().内容);
    const auto* I64 = 值 ? std::get_if<std::int64_t>(&值->材料) : nullptr;
    if (!I64) return std::nullopt;
    return std::optional<std::int64_t>{*I64};
}

std::optional<std::string> 读取唯一字符串字段(
    稳定编码 节点, 稳定编码 字段) {
    const auto 组 = 全局基础数据集.查询字段(节点, 字段);
    if (组.size() != 1) return std::nullopt;
    const auto* 值 = std::get_if<基础值>(&组.front().内容);
    const auto* 字符串 = 值 ? std::get_if<std::string>(&值->材料) : nullptr;
    return 字符串 ? std::optional<std::string>{*字符串} : std::nullopt;
}


bool 参数关系顺序(const 新方法参数关系& a,const 新方法参数关系& b) {
    return std::tie(a.种类,a.左参数序号,a.右参数序号,a.左正式内容路径)
        <std::tie(b.种类,b.左参数序号,b.右参数序号,b.左正式内容路径);
}
bool 含内容关系(const std::vector<新方法参数关系>& 组) {
    return std::any_of(组.begin(),组.end(),[](const auto& r){return r.种类==新方法参数关系种类::内容引用参数;});
}
void 规范内容关系配对(新方法条件结果定义& d) {
    if(!含内容关系(d.条件.参数关系组)&&!含内容关系(d.结果.参数关系组))return;
    std::sort(d.条件.参数关系组.begin(),d.条件.参数关系组.end(),参数关系顺序);
    std::sort(d.结果.参数关系组.begin(),d.结果.参数关系组.end(),参数关系顺序);
}
void 规范内容关系声明(新方法知识建立请求& r) {
    if(含内容关系(r.初始模拟关系组))std::sort(r.初始模拟关系组.begin(),r.初始模拟关系组.end(),参数关系顺序);
    for(auto& p:r.条件结果组)规范内容关系配对(p);
    for(auto& b:r.执行分支组)if(含内容关系(b.必要条件.参数关系组))
        std::sort(b.必要条件.参数关系组.begin(),b.必要条件.参数关系组.end(),参数关系顺序);
}
bool 参数关系结构有效(const std::vector<新方法参数关系>& 关系) {
    for (std::size_t i=0;i<关系.size();++i) {
        const auto& r=关系[i];
        const auto k=static_cast<std::int64_t>(r.种类);
        // R97：特征所属存在(3)、同属场景(5)、场景包含存在(7)已退役，编号保留不复用。
        if(k<1||k>8||k==3||k==5||k==7||!r.左参数序号||!r.右参数序号) return false;
        if(r.种类==新方法参数关系种类::内容引用参数) {
            if(r.左正式内容路径.size()!=1||!节点仍存在(r.左正式内容路径.front()))return false;
        } else if(!r.左正式内容路径.empty())return false;
        if(std::find(关系.begin(),关系.begin()+i,r)!=关系.begin()+i) return false;
    }
    return true;
}
std::optional<std::vector<新方法参数关系>> 读取参数关系(稳定编码 n) {
    const auto 组=全局基础数据集.查询字段(n,参数位置关系字段);
    if(组.size()>1) return {};
    std::vector<新方法参数关系> r;
    if(!组.empty()) {
    const auto* v=std::get_if<基础值>(&组.front().内容);
    const auto* a=v?std::get_if<std::vector<std::int64_t>>(&v->材料):nullptr;
    if(!a||a->size()%3) return {};
    for(std::size_t i=0;i<a->size();i+=3) {
        if((*a)[i]<1||(*a)[i]>7||(*a)[i]==3||(*a)[i]==5||(*a)[i+1]<=0||(*a)[i+2]<=0
            ||(*a)[i+1]>(std::numeric_limits<std::uint32_t>::max)()
            ||(*a)[i+2]>(std::numeric_limits<std::uint32_t>::max)()) return {};
        r.push_back({static_cast<新方法参数关系种类>((*a)[i]),
            static_cast<std::uint32_t>((*a)[i+1]),static_cast<std::uint32_t>((*a)[i+2])});
    }
    }
    for(const auto& f:全局基础数据集.查询字段(n,内容关系成员字段)) {
        const auto* child=std::get_if<稳定编码>(&f.内容);
        if(!child||读取唯一节点字段(*child,节点类型字段)!=std::optional{内容关系节点类型}
            ||读取唯一节点字段(*child,内容关系所属字段)!=std::optional{n})return {};
        const auto parents=全局基础数据集.查询目标字段(*child,内容关系成员字段);
        if(parents.size()!=1||parents.front().所属节点!=n)return {};
        const auto l=读取唯一I64字段(*child,内容关系左字段),v=读取唯一I64字段(*child,内容关系右字段);
        const auto ft=读取唯一节点字段(*child,内容关系路径字段);
        if(!l||!v||!ft||*l<=0||*v<=0||*l>(std::numeric_limits<std::uint32_t>::max)()
            ||*v>(std::numeric_limits<std::uint32_t>::max)())return {};
        const auto snapshot=全局基础数据集.查询完整节点结构(*child);
        if(!snapshot.结构)return {};
        for(const auto& x:snapshot.结构->字段组)
            if(x.字段节点!=节点类型字段&&x.字段节点!=内容关系所属字段&&x.字段节点!=内容关系左字段
                &&x.字段节点!=内容关系右字段&&x.字段节点!=内容关系路径字段)return {};
        r.push_back({新方法参数关系种类::内容引用参数,static_cast<std::uint32_t>(*l),static_cast<std::uint32_t>(*v),{*ft}});
    }
    if(含内容关系(r))std::sort(r.begin(),r.end(),参数关系顺序);
    return 参数关系结构有效(r)?std::optional{std::move(r)}:std::nullopt;
}

// 外层为空表示结构损坏；内层为空才表示旧项没有私有域。
std::optional<std::optional<特征概念I64闭区间>> 读取可选精确数值域(
    稳定编码 项节点) {
    const auto 组 = 全局基础数据集.查询字段(项节点, 项精确数值域字段);
    if (组.empty()) return std::optional<特征概念I64闭区间>{};
    if (组.size() != 1) return std::nullopt;
    const auto* 值 = std::get_if<基础值>(&组.front().内容);
    const auto* 端点 = 值 ? std::get_if<std::vector<std::int64_t>>(&值->材料) : nullptr;
    if (!端点 || 端点->size() != 2 || (*端点)[0] > (*端点)[1]) return std::nullopt;
    return std::optional<特征概念I64闭区间>{{(*端点)[0], (*端点)[1]}};
}

std::optional<std::vector<稳定编码>> 读取多节点字段(
    稳定编码 节点, 稳定编码 字段, const 概念_特征类&) {
    std::vector<稳定编码> 结果;
    for (const auto& 关系 : 全局基础数据集.查询字段(节点, 字段)) {
        const auto* 目标 = std::get_if<稳定编码>(&关系.内容);
        if (!目标 || !节点仍存在(*目标)) return std::nullopt;
        结果.push_back(*目标);
    }
    std::sort(结果.begin(), 结果.end());
    if (std::adjacent_find(结果.begin(), 结果.end()) != 结果.end()) {
        return std::nullopt;
    }
    return 结果;
}

bool 节点类型为(稳定编码 节点, 稳定编码 类型) {
    const auto 实际 = 读取唯一节点字段(节点, 节点类型字段);
    return 实际 && *实际 == 类型;
}

bool 结构已分配() noexcept {
    const auto 编码 = 架构编码组();
    return std::all_of(编码.begin(), 编码.end(), [](const 稳定编码* 值) {
        return 有效(*值);
    });
}

bool 存在部分架构编码() noexcept {
    const auto 编码 = 架构编码组();
    return std::any_of(编码.begin(), 编码.end(), [](const 稳定编码* 值) {
        return 有效(*值);
    });
}

bool 结构仍存在() {
    if (!结构已分配()) return false;
    const auto 编码 = 架构编码组();
    if (!std::all_of(编码.begin(), 编码.end(), [](const 稳定编码* 值) {
        return 节点仍存在(*值);
    })) return false;
    if (!节点类型为(新方法组织根节点, 方法组织根类型)) return false;
    // 独立根不能经外部父子关系挂在任意其它节点下。
    return 全局基础数据集.查询目标关系(
        新方法组织根节点, 基础外部关系类型::父子).empty();
}

bool 写入节点字段(
    稳定编码 节点, 稳定编码 字段, 稳定编码 目标,
    std::vector<稳定编码>& 已写字段) {
    已写字段.reserve(已写字段.size() + 1);
    const auto 关系 = 全局基础数据集.添加字段节点(节点, 字段, 目标);
    if (!有效(关系)) return false;
    已写字段.push_back(关系);
    return true;
}

bool 写入I64字段(
    稳定编码 节点, 稳定编码 字段, std::int64_t 值,
    std::vector<稳定编码>& 已写字段) {
    已写字段.reserve(已写字段.size() + 1);
    const auto 关系 = 全局基础数据集.添加字段值(
        节点, 字段, 基础值{值});
    if (!有效(关系)) return false;
    已写字段.push_back(关系);
    return true;
}


稳定编码 新建类型化节点(稳定编码,std::vector<稳定编码>&,std::vector<稳定编码>&);
bool 写入参数关系(稳定编码 n,const std::vector<新方法参数关系>& r,
    std::vector<稳定编码>& 已建,std::vector<稳定编码>& 已写) {
    if(r.empty()) return true;
    if(!参数关系结构有效(r)) return false;
    std::vector<std::int64_t> a;
    for(const auto& x:r) {
        if(x.种类==新方法参数关系种类::内容引用参数) {
            const auto child=新建类型化节点(内容关系节点类型,已建,已写);
            if(!有效(child)||!写入节点字段(child,内容关系所属字段,n,已写)
                ||!写入I64字段(child,内容关系左字段,x.左参数序号,已写)
                ||!写入I64字段(child,内容关系右字段,x.右参数序号,已写)
                ||!写入节点字段(child,内容关系路径字段,x.左正式内容路径.front(),已写)
                ||!写入节点字段(n,内容关系成员字段,child,已写))return false;
            continue;
        }
        a.push_back(static_cast<std::int64_t>(x.种类));
        a.push_back(x.左参数序号);a.push_back(x.右参数序号);
    }
    if(a.empty())return true;
    已写.reserve(已写.size()+1);
    const auto f=全局基础数据集.添加字段值(n,参数位置关系字段,基础值{std::move(a)});
    if(!有效(f)) return false;
    已写.push_back(f);return true;
}


std::optional<std::vector<std::uint32_t>> 读取模拟事件位置(稳定编码 n) {
    const auto g=全局基础数据集.查询字段(n,模拟事件位置字段);
    if(g.empty())return std::vector<std::uint32_t>{};
    if(g.size()!=1)return {};
    const auto* v=std::get_if<基础值>(&g.front().内容);
    const auto* a=v?std::get_if<std::vector<std::int64_t>>(&v->材料):nullptr;
    if(!a)return {};
    std::vector<std::uint32_t> r;
    for(auto p:*a) {
        if(p<=0||p>(std::numeric_limits<std::uint32_t>::max)())return {};
        const auto x=static_cast<std::uint32_t>(p);
        if(std::find(r.begin(),r.end(),x)!=r.end())return {};
        r.push_back(x);
    }return r;
}
bool 写入模拟事件位置(稳定编码 n,const std::vector<std::uint32_t>& r,std::vector<稳定编码>& 已写) {
    if(r.empty())return true;
    std::vector<std::int64_t> a(r.begin(),r.end());
    已写.reserve(已写.size()+1);
    const auto f=全局基础数据集.添加字段值(n,模拟事件位置字段,基础值{std::move(a)});
    if(!有效(f))return false;
    已写.push_back(f);return true;
}

bool 写入字符串字段(
    稳定编码 节点, 稳定编码 字段, const std::string& 值,
    std::vector<稳定编码>& 已写字段) {
    已写字段.reserve(已写字段.size() + 1);
    const auto 关系 = 全局基础数据集.添加字段值(
        节点, 字段, 基础值{值});
    if (!有效(关系)) return false;
    已写字段.push_back(关系);
    return true;
}

bool 写入精确数值域(稳定编码 项节点, const 特征概念I64闭区间& 域,
    std::vector<稳定编码>& 已写字段) {
    已写字段.reserve(已写字段.size() + 1);
    const auto 关系 = 全局基础数据集.添加字段值(项节点, 项精确数值域字段,
        基础值{std::vector<std::int64_t>{域.下界, 域.上界}});
    if (!有效(关系)) return false;
    已写字段.push_back(关系);
    return true;
}

void 回滚(
    const std::vector<稳定编码>& 字段组,
    const std::vector<稳定编码>& 节点组) noexcept {
    for (auto i = 字段组.rbegin(); i != 字段组.rend(); ++i) {
        (void)全局基础数据集.删除字段(*i);
    }
    for (auto i = 节点组.rbegin(); i != 节点组.rend(); ++i) {
        (void)全局基础数据集.删除节点(*i);
    }
}

稳定编码 新建类型化节点(
    稳定编码 类型,
    std::vector<稳定编码>& 已建节点,
    std::vector<稳定编码>& 已写字段) {
    已建节点.reserve(已建节点.size() + 1);
    const auto 节点 = 全局基础数据集.新建节点();
    if (!有效(节点)) return {};
    已建节点.push_back(节点);
    if (!写入节点字段(节点, 节点类型字段, 类型, 已写字段)) return {};
    return 节点;
}

bool 引用组有效(const std::vector<稳定编码>& 组) {
    std::set<稳定编码> 去重;
    for (const auto 节点 : 组) {
        if (!节点仍存在(节点) || !去重.insert(节点).second) return false;
    }
    return true;
}

bool 可选引用有效(const std::optional<稳定编码>& 节点) noexcept {
    return !节点 || 节点仍存在(*节点);
}

bool 关系定义有效(新方法项关系规则 规则, std::size_t 项数) noexcept {
    if (!是关系规则(static_cast<std::int64_t>(规则)) || 项数 == 0) return false;
    return 规则 != 新方法项关系规则::单项 || 项数 == 1;
}

bool 方法头有效(const 新方法头定义& 头) {
    return !头.可能结果概念组.empty()
        && 引用组有效(头.作用存在概念组)
        && 引用组有效(头.涉及特征类型概念组)
        && 引用组有效(头.可能结果概念组);
}

bool 是服务要求参数对象(新方法参数对象种类 种类) noexcept {
    switch (种类) {
    case 新方法参数对象种类::服务要求来源人类:
    case 新方法参数对象种类::服务要求目标存在:
    case 新方法参数对象种类::服务要求目标特征类型:
    case 新方法参数对象种类::服务要求目标定义:
    case 新方法参数对象种类::服务要求确认: return true;
    default: return false;
    }
}

bool 是参数对象种类(std::int64_t 值) noexcept {
    return 值 == static_cast<std::int64_t>(新方法参数对象种类::普通世界)
        || 值 == static_cast<std::int64_t>(新方法参数对象种类::任务虚拟存在)
        || 是服务要求参数对象(static_cast<新方法参数对象种类>(值));
}

bool 服务要求分项匹配(新方法参数对象种类 种类, 新服务要求分项种类 分项) noexcept {
    switch (种类) {
    case 新方法参数对象种类::服务要求来源人类:
        return 分项 == 新服务要求分项种类::来源人类;
    case 新方法参数对象种类::服务要求目标存在:
        return 分项 == 新服务要求分项种类::目标存在;
    case 新方法参数对象种类::服务要求目标特征类型:
        return 分项 == 新服务要求分项种类::目标特征类型;
    case 新方法参数对象种类::服务要求目标定义:
        return 分项 == 新服务要求分项种类::目标定义;
    default: return false;
    }
}

bool 词条参数特征类型允许(稳定编码 FT, const 概念_特征类& owner) {
    for (const auto 槽 : {先天特征类型槽::概念名称挂接, 先天特征类型槽::词典材料}) {
        const auto 当前 = owner.读取当前先天特征类型槽(槽);
        if (当前.状态 == 特征概念规则状态::资源失败) throw std::bad_alloc{};
        if (当前.状态 == 特征概念规则状态::值不存在) continue;
        if (当前.状态 != 特征概念规则状态::已完成 || !当前.类型根)
            throw std::logic_error("word parameter feature slot unavailable");
        if (*当前.类型根 == FT) return true;
    }
    return false;
}

bool 是特征持有参数(新方法参数信息类型 类型) noexcept {
    return 类型 == 新方法参数信息类型::存在 || 类型 == 新方法参数信息类型::词条;
}

bool 参数规格有效(const std::vector<新方法参数规格>& 规格组,
    const 概念_特征类& 概念服务, bool 完整组 = true) {
    std::set<稳定编码> 角色组;
    const bool 有序 = !规格组.empty() && 规格组.front().有序规格.has_value();
    std::uint64_t 预期序号 = 1;
    for (const auto& 规格 : 规格组) {
        if (规格.有序规格.has_value() != 有序) return false;
        if (规格.有序规格) {
            const auto& 项 = *规格.有序规格;
            if (项.序号 == 0 || (完整组 && 项.序号 != 预期序号++)
                || !节点仍存在(项.范围概念节点)
                || (项.信息类型 != 新方法参数信息类型::存在
                    && 项.信息类型 != 新方法参数信息类型::特征
                    && 项.信息类型 != 新方法参数信息类型::词条)
                || 有效(规格.参数角色节点) || 规格.存在概念节点 || 规格.要求场景
                || !规格.特征规格组.empty() || 规格.集合成员约束
                || 规格.对象种类 != 新方法参数对象种类::普通世界) return false;
            if (项.信息类型 == 新方法参数信息类型::特征
                && !概念服务.获取特征概念(项.范围概念节点)) return false;
            // R97：存在位置声明必需特征类型组（升序去重、均为特征概念），不声明所属；
            // 特征位置声明所属存在位置，不声明必需特征类型组。
            if (是特征持有参数(项.信息类型)) {
                if (项.所属存在位置 != 0
                    || !std::is_sorted(项.必需特征类型组.begin(), 项.必需特征类型组.end())
                    || std::adjacent_find(项.必需特征类型组.begin(), 项.必需特征类型组.end())
                        != 项.必需特征类型组.end()) return false;
                for (const auto 类型 : 项.必需特征类型组)
                    if (!节点仍存在(类型) || !概念服务.获取特征概念(类型)) return false;
                if (项.信息类型 == 新方法参数信息类型::词条 && !项.必需特征类型组.empty()) {
                    for (const auto FT : 项.必需特征类型组)
                        if (!词条参数特征类型允许(FT, 概念服务)) return false;
                }
            } else {
                if (!项.必需特征类型组.empty() || 项.所属存在位置 == 0
                    || 项.所属存在位置 == 项.序号) return false;
                if (完整组) {
                    const auto 所属 = std::find_if(规格组.begin(), 规格组.end(), [&](const auto& 其它) {
                        return 其它.有序规格 && 其它.有序规格->序号 == 项.所属存在位置; });
                    if (所属 == 规格组.end()
                        || !是特征持有参数(所属->有序规格->信息类型)
                        || !std::binary_search(所属->有序规格->必需特征类型组.begin(),
                            所属->有序规格->必需特征类型组.end(), 项.范围概念节点)) return false;
                }
            }
            continue;
        }
        if (!节点仍存在(规格.参数角色节点)
            || !角色组.insert(规格.参数角色节点).second
            || !可选引用有效(规格.存在概念节点)
            || !是参数对象种类(static_cast<std::int64_t>(规格.对象种类))
            || (是服务要求参数对象(规格.对象种类)
                && (规格.存在概念节点 || 规格.要求场景 || !规格.特征规格组.empty()))
            || (规格.对象种类 == 新方法参数对象种类::任务虚拟存在
                && (规格.存在概念节点 || 规格.要求场景
                    || 规格.特征规格组.empty() || 规格.特征规格组.size() > 2))) return false;
        std::set<稳定编码> 特征类型组;
        for (const auto& 特征 : 规格.特征规格组) {
            if (!节点仍存在(特征.特征类型概念节点)
                || !特征类型组.insert(特征.特征类型概念节点).second
                || !可选引用有效(特征.值域概念节点)
                || !引用组有效(特征.二次特征概念组)) return false;
        }
        if (规格.集合成员约束) {
            const auto& 约束 = *规格.集合成员约束;
            const auto 先天 = 概念服务.获取先天特征概念();
            if (规格.对象种类 != 新方法参数对象种类::任务虚拟存在
                || !节点仍存在(约束.集合参数角色节点)
                || 约束.集合参数角色节点 == 规格.参数角色节点
                || !先天 || 约束.集合特征类型概念节点 != 先天->服务任务集合) return false;
            if (完整组) {
                const auto 目标 = std::find_if(规格组.begin(), 规格组.end(),
                    [&](const auto& 项) { return 项.参数角色节点 == 约束.集合参数角色节点; });
                if (目标 == 规格组.end() || 目标->对象种类 != 新方法参数对象种类::普通世界
                    || std::count_if(目标->特征规格组.begin(), 目标->特征规格组.end(),
                        [&](const auto& 项) { return 项.特征类型概念节点 == 约束.集合特征类型概念节点; }) != 1)
                    return false;
            }
        }
    }
    return true;
}

bool 精确数值域有效(const std::optional<特征概念I64闭区间>& 域,
    const std::optional<稳定编码>& 类型, const std::optional<稳定编码>& 概念域,
    const 概念_特征类& 概念服务) {
    if (!域) return true;
    if (概念域 || !类型 || 域->下界 > 域->上界) return false;
    const auto 特征概念 = 概念服务.获取特征概念(*类型);
    return 特征概念 && 特征概念->材料类型 == 特征概念材料物理类型::I64标量;
}

bool 结果项有效(const 新方法结果项定义& 项, const 概念_特征类& 概念服务) noexcept {
    if(项.结果作用存在)
        return !项.参数序号&&!有效(项.参数角色节点)&&!项.任务成员变量序号&&!项.引用内容结果&&!项.被动需求目标
            &&节点仍存在(*项.结果作用存在)&&项.特征类型概念节点&&可选引用有效(项.特征类型概念节点)
            &&(项.特征值域概念节点||项.精确数值域)&&可选引用有效(项.特征值域概念节点)
            &&精确数值域有效(项.精确数值域,项.特征类型概念节点,项.特征值域概念节点,概念服务)
            &&!项.变化方向概念节点&&!项.状态或动态概念节点;
    if(项.被动需求目标)
        return 项.参数序号&&!有效(项.参数角色节点)&&!项.任务成员变量序号&&!项.引用内容结果
            &&!项.特征类型概念节点&&!项.特征值域概念节点&&!项.精确数值域
            &&!项.变化方向概念节点&&!项.状态或动态概念节点;
    if(项.引用内容结果) {
        const auto& c=*项.引用内容结果;
        return 项.参数序号&&!有效(项.参数角色节点)&&!项.任务成员变量序号
            &&!项.特征类型概念节点&&!项.特征值域概念节点&&!项.精确数值域
            &&!项.变化方向概念节点&&!项.状态或动态概念节点
            &&c.作用点==新方法内容作用点::特征当前引用
            &&!c.要求认知来源当前有效&&c.正式内容路径.empty()&&节点仍存在(c.要求概念节点);
    }
    return 项.任务成员变量序号 <= 1 && (!项.任务成员变量序号 || 项.参数序号)
        && (项.参数序号 ? !有效(项.参数角色节点) : 节点仍存在(项.参数角色节点))
        && 可选引用有效(项.特征类型概念节点)
        && 可选引用有效(项.特征值域概念节点)
        && 精确数值域有效(项.精确数值域, 项.特征类型概念节点,
            项.特征值域概念节点, 概念服务)
        && 可选引用有效(项.变化方向概念节点)
        && 可选引用有效(项.状态或动态概念节点)
        && (项.特征类型概念节点 || 项.特征值域概念节点
            || 项.变化方向概念节点 || 项.状态或动态概念节点);
}

std::optional<稳定编码> 读取线性特征类型根(
    稳定编码 节点, const 概念_特征类& 服务) {
    std::set<稳定编码> 已查;
    while (有效(节点) && 已查.insert(节点).second) {
        if (!服务.获取特征概念(节点)) return std::nullopt;
        std::optional<稳定编码> 上位;
        for (const auto& 关系 : 全局基础数据集.查询目标关系(节点, 基础外部关系类型::父子)) {
            if (!服务.是特征概念节点(关系.源节点)) continue;
            if (上位) return std::nullopt;
            上位 = 关系.源节点;
        }
        if (!上位) return 节点;
        节点 = *上位;
    }
    return std::nullopt;
}

bool 是有限线性域(const 特征概念I64闭区间& 域) noexcept {
    return 域.下界 <= 域.上界
        && 域.下界 != (std::numeric_limits<std::int64_t>::min)()
        && 域.上界 != (std::numeric_limits<std::int64_t>::max)();
}

std::optional<特征概念I64闭区间> 读取线性项域(
    const std::optional<稳定编码>& 类型节点,
    const std::optional<稳定编码>& 概念节点,
    const std::optional<特征概念I64闭区间>& 精确域,
    const 概念_特征类& 服务) {
    if (!类型节点 || (概念节点.has_value() == 精确域.has_value())) return std::nullopt;
    const auto 类型 = 服务.获取特征概念(*类型节点);
    if (!类型 || 类型->材料类型 != 特征概念材料物理类型::I64标量) return std::nullopt;
    if (精确域) return 是有限线性域(*精确域) ? 精确域 : std::nullopt;
    const auto 域 = 服务.获取特征概念(*概念节点);
    const auto 根 = 读取线性特征类型根(*类型节点, 服务);
    if (!根 || !域 || 域->单位 != 类型->单位
        || 域->材料类型 != 特征概念材料物理类型::I64标量
        || 读取线性特征类型根(*概念节点, 服务) != 根) return std::nullopt;
    const auto* 组 = std::get_if<std::vector<特征概念I64闭区间>>(&域->值域);
    if (!组 || 组->size() != 1 || !是有限线性域(组->front())) return std::nullopt;
    return 组->front();
}

bool 线性关系位置小于(const 新方法条件结果线性关系& 左,
    const 新方法条件结果线性关系& 右) noexcept {
    return std::tie(左.条件序号, 左.结果序号) < std::tie(右.条件序号, 右.结果序号);
}

bool 线性关系等价(const std::vector<新方法条件结果线性关系>& 左,
    const std::vector<新方法条件结果线性关系>& 右) {
    return std::is_permutation(左.begin(), 左.end(), 右.begin(), 右.end());
}

bool 线性关系有效(const 新方法条件结果定义& 定义,
    const 概念_特征类& 服务, bool 实例) {
    if (定义.线性关系组.empty()) return true;
    if ((定义.条件.关系规则 != 新方法项关系规则::单项
            && 定义.条件.关系规则 != 新方法项关系规则::全部成立)
        || (定义.结果.关系规则 != 新方法项关系规则::单项
            && 定义.结果.关系规则 != 新方法项关系规则::全部成立
            && 定义.结果.关系规则 != 新方法项关系规则::主要结果及允许副作用)) return false;
    std::set<std::pair<std::size_t, std::size_t>> 位置组;
    std::map<std::size_t, 特征概念I64闭区间> 条件域组, 结果域组;
    for (const auto& 关系 : 定义.线性关系组) {
        if (关系.条件序号 >= 定义.条件.条件项组.size()
            || 关系.结果序号 >= 定义.结果.结果项组.size()
            || 关系.条件序号 > static_cast<std::uint64_t>((std::numeric_limits<std::int64_t>::max)())
            || 关系.结果序号 > static_cast<std::uint64_t>((std::numeric_limits<std::int64_t>::max)())
            || !位置组.emplace(关系.条件序号, 关系.结果序号).second
            || !是有限线性域(关系.原条件域) || !是有限线性域(关系.原结果域)) return false;
        switch (关系.方向) {
        case 新方法条件结果线性方向::不变:
            if (关系.原结果域.下界 != 关系.原结果域.上界) return false;
            break;
        case 新方法条件结果线性方向::同向:
        case 新方法条件结果线性方向::反向:
            if (关系.原条件域.下界 == 关系.原条件域.上界
                || 关系.原结果域.下界 == 关系.原结果域.上界) return false;
            break;
        default: return false;
        }
        const auto [C, 新C] = 条件域组.emplace(关系.条件序号, 关系.原条件域);
        const auto [R, 新R] = 结果域组.emplace(关系.结果序号, 关系.原结果域);
        if ((!新C && C->second != 关系.原条件域) || (!新R && R->second != 关系.原结果域)) return false;
        const auto& 条件 = 定义.条件.条件项组[关系.条件序号];
        if (条件.引用内容条件||条件.特征前态限定) return false;
        const auto& 结果 = 定义.结果.结果项组[关系.结果序号];
        if(结果.引用内容结果||结果.被动需求目标)return false;
        if (条件.二次特征概念节点 || 条件.状态或动态概念节点 || 结果.状态或动态概念节点) return false;
        const auto 当前C = 读取线性项域(条件.特征类型概念节点,
            条件.特征值域概念节点, 条件.精确数值域, 服务);
        const auto 当前R = 读取线性项域(结果.特征类型概念节点,
            结果.特征值域概念节点, 结果.精确数值域, 服务);
        if (!当前C || !当前R || *当前R != 关系.原结果域) return false;
        if (实例) {
            if (当前C->下界 < 关系.原条件域.下界 || 当前C->上界 > 关系.原条件域.上界) return false;
        } else if (*当前C != 关系.原条件域) return false;
    }
    return true;
}

std::optional<std::vector<新方法条件结果线性关系>> 读取线性关系组(稳定编码 配对) {
    const auto 字段 = 全局基础数据集.查询字段(配对, 配对线性关系组字段);
    if (字段.empty()) return std::vector<新方法条件结果线性关系>{};
    if (字段.size() != 1) return std::nullopt;
    const auto* 值 = std::get_if<基础值>(&字段.front().内容);
    const auto* 数据 = 值 ? std::get_if<std::vector<std::int64_t>>(&值->材料) : nullptr;
    if (!数据 || 数据->empty() || 数据->size() % 7 != 0) return std::nullopt;
    std::vector<新方法条件结果线性关系> 结果;
    for (std::size_t i = 0; i < 数据->size(); i += 7) {
        if ((*数据)[i] < 0 || (*数据)[i + 1] < 0 || (*数据)[i + 2] < -1 || (*数据)[i + 2] > 1
            || static_cast<std::uint64_t>((*数据)[i]) > (std::numeric_limits<std::size_t>::max)()
            || static_cast<std::uint64_t>((*数据)[i + 1]) > (std::numeric_limits<std::size_t>::max)()) return std::nullopt;
        新方法条件结果线性关系 项{static_cast<std::size_t>((*数据)[i]),
            static_cast<std::size_t>((*数据)[i + 1]), static_cast<新方法条件结果线性方向>((*数据)[i + 2]),
            {(*数据)[i + 3], (*数据)[i + 4]}, {(*数据)[i + 5], (*数据)[i + 6]}};
        if (!结果.empty() && !线性关系位置小于(结果.back(), 项)) return std::nullopt;
        结果.push_back(项);
    }
    return 结果;
}

bool 条件结果有效(const 新方法条件结果定义& 定义, const 概念_特征类& 概念服务,
    bool 实例 = false,bool 仅条件声明 = false) {
    std::optional<std::uint32_t> 成员参数;
    const auto 成员项有效 = [&](const auto& 项, bool 条件) {
        if (!项.任务成员变量序号) return true;
        if (项.任务成员变量序号 != 1 || !项.参数序号 || !项.特征类型概念节点
            || 项.状态或动态概念节点) return false;
        const auto 类型 = 概念服务.获取特征概念(*项.特征类型概念节点);
        if (!类型 || 类型->材料类型 != 特征概念材料物理类型::I64标量) return false;
        if constexpr (requires { 项.存在概念节点; })
            if (项.存在概念节点 || 项.二次特征概念节点) return false;
        if (条件 && !成员参数) 成员参数 = 项.参数序号;
        return 成员参数 && *成员参数 == 项.参数序号;
    };
    for (const auto& 项 : 定义.条件.条件项组) if (!成员项有效(项, true)) return false;
    if (成员参数 && 定义.条件.关系规则 != 新方法项关系规则::单项
        && 定义.条件.关系规则 != 新方法项关系规则::全部成立) return false;
    for (const auto& 项 : 定义.结果.结果项组) if (!成员项有效(项, false)) return false;
    for (const auto& 项 : 定义.结果.确认要求.禁止结果项组) if (!成员项有效(项, false)) return false;
    if (!线性关系有效(定义, 概念服务, 实例)) return false;
    const auto 条件数=定义.条件.条件项组.size()+定义.条件.参数关系组.size()+定义.条件.场景事实限定组.size();
    // 2026-11-10用户裁决：允许不含任何条件的条件节点（如获取交互者，随时可执行）——条件为空且规则为全部成立即有效；
    // 条件与结果皆空的配对由学习配对合法性另行拒绝。
    if (!(条件数==0&&定义.条件.关系规则==新方法项关系规则::全部成立)
        && !关系定义有效(定义.条件.关系规则,条件数))return false;
    if(!仅条件声明&&!关系定义有效(定义.结果.关系规则,定义.结果.结果项组.size()+定义.结果.参数关系组.size()))return false;
    if(!参数关系结构有效(定义.条件.参数关系组)||!参数关系结构有效(定义.结果.参数关系组)) return false;
    for (const auto& 项 : 定义.条件.条件项组) {
        if(项.特征前态限定) {
            const auto& p=*项.特征前态限定;
            if(!项.参数序号||有效(项.参数角色节点)||项.任务成员变量序号
                ||项.存在概念节点||项.特征值域概念节点||项.精确数值域
                ||项.引用内容条件||项.二次特征概念节点||项.状态或动态概念节点
                ||!可选引用有效(项.特征类型概念节点)
                ||p.当前值状态!=新方法当前值状态::未知
                ||p.历史不同值数量!=std::optional<std::uint64_t>{0})return false;
            continue;
        }
        if (项.引用内容条件) {
            const auto& 内容=*项.引用内容条件;
            if (!项.参数序号 || 有效(项.参数角色节点) || 项.任务成员变量序号
                || 项.存在概念节点 || 项.特征类型概念节点 || 项.特征值域概念节点
                || 项.精确数值域 || 项.二次特征概念节点 || 项.状态或动态概念节点
                || (内容.作用点!=新方法内容作用点::特征当前引用&&内容.作用点!=新方法内容作用点::参数自身)
                || (内容.要求认知来源当前有效&&(内容.作用点!=新方法内容作用点::参数自身||!内容.正式内容路径.empty()))
                || !节点仍存在(内容.要求概念节点) || 内容.正式内容路径.size()>1
                || std::any_of(内容.正式内容路径.begin(),内容.正式内容路径.end(),
                    [](稳定编码 FT){return !节点仍存在(FT);})) return false;
            continue;
        }
        if (项.任务成员变量序号 > 1 || (项.任务成员变量序号 && !项.参数序号)
            || !(项.参数序号 ? !有效(项.参数角色节点) : 节点仍存在(项.参数角色节点))
            || !可选引用有效(项.存在概念节点)
            || !可选引用有效(项.特征类型概念节点)
            || !可选引用有效(项.特征值域概念节点)
            || !精确数值域有效(项.精确数值域, 项.特征类型概念节点,
                项.特征值域概念节点, 概念服务)
            || !可选引用有效(项.二次特征概念节点)
            || !可选引用有效(项.状态或动态概念节点)
            || (!项.存在概念节点 && !项.特征类型概念节点
                && !项.特征值域概念节点 && !项.二次特征概念节点
                && !项.状态或动态概念节点)) return false;
    }
    const auto& 要求 = 定义.结果.确认要求;
    if (!是必要证据质量(static_cast<std::int64_t>(要求.必要证据质量))
        || !是多证据关系(static_cast<std::int64_t>(要求.多证据关系))
        || (要求.最大等待纳秒 && *要求.最大等待纳秒 < 0)
        || !可选引用有效(要求.等待条件概念节点)
        || (!要求.禁止结果项组.empty()
            && !关系定义有效(要求.禁止结果关系规则,
                要求.禁止结果项组.size()))) {
        return false;
    }
    std::size_t 主要结果数量 = 0;
    for (const auto& 项 : 定义.结果.结果项组) {
        if (!结果项有效(项, 概念服务)) return false;
        if (项.主要结果) ++主要结果数量;
    }
    if (std::any_of(要求.禁止结果项组.begin(),
            要求.禁止结果项组.end(), [&](const 新方法结果项定义& 项) {
                return !结果项有效(项, 概念服务) || 项.主要结果;
            })) {
        return false;
    }
    return 定义.结果.关系规则
            != 新方法项关系规则::主要结果及允许副作用
        || 主要结果数量 == 1;
}

struct 引用内容定义前提变化 final {};

// 范围定义属于概念owner；正式信息实例由注入reader读取，不投影为普通E。
bool 追加方法存在范围保护(稳定编码 C,const 概念_存在类& owner,
    std::vector<基础联合节点快照>& protection) {
    std::set<稳定编码> seen;
    const auto read=[&](auto&& self,稳定编码 n)->bool {
        if(!seen.insert(n).second)return true;
        const auto ordinary=owner.读取普通存在概念定义保护(n);
        if(ordinary.状态==正式概念读取状态::前提已变化)throw 引用内容定义前提变化{};
        if(ordinary.状态==正式概念读取状态::已读取) {
            if(!ordinary.信息||ordinary.完整保护组.empty())throw std::runtime_error("ordinary parameter range proof incomplete");
            protection.insert(protection.end(),ordinary.完整保护组.begin(),ordinary.完整保护组.end());
            for(const auto up:ordinary.信息->上位概念组)if(!self(self,up))return false;
            return true;
        }
        if(ordinary.状态!=正式概念读取状态::非本提供者概念)
            throw std::runtime_error("ordinary parameter range owner unavailable");
        const auto formal=owner.读取正式结构概念保护(n);
        if(formal.状态==正式结构操作状态::前提已变化)throw 引用内容定义前提变化{};
        if(formal.状态==正式结构操作状态::未找到||formal.状态==正式结构操作状态::输入拒绝)return false;
        if(formal.状态!=正式结构操作状态::已读取||!formal.信息||formal.完整保护组.empty())
            throw std::runtime_error("formal parameter range proof unavailable");
        if(!std::holds_alternative<信息结构概念定义>(formal.信息->定义))return false;
        protection.insert(protection.end(),formal.完整保护组.begin(),formal.完整保护组.end());
        return true;
    };
    return read(read,C);
}

bool 方法存在范围有效(稳定编码 C,const 概念_存在类& owner) {
    std::vector<基础联合节点快照> protection;
    return 追加方法存在范围保护(C,owner,protection);
}

bool 追加方法词条范围保护(稳定编码 C, const 概念_存在类& owner,
    std::vector<基础联合节点快照>& protection) {
    const auto 当前 = owner.读取词条身份范围概念保护();
    if (!当前.信息) return false;
    if (当前.信息->节点 != C) return false;
    protection.insert(protection.end(), 当前.完整保护组.begin(), 当前.完整保护组.end());
    return true;
}

// 配对可以收窄到词条同键；方法登记规格仍由原全域入口限定。
bool 追加方法配对词条范围保护(稳定编码 C, const 概念_存在类& owner,
    std::vector<基础联合节点快照>& protection) {
    const auto d = owner.读取当前基础范围概念(C);
    if (d.状态 == 新基础范围读取状态::资源失败) throw std::bad_alloc{};
    if (d.状态 == 新基础范围读取状态::依赖未就绪)
        throw std::logic_error("word condition concept reader unavailable");
    if (!d.定义 || d.定义->原语 != 基础概念原语::词条同键)
        return 追加方法词条范围保护(C, owner, protection);
    const auto p = owner.读取词条同键范围(C);
    if (p.状态 == 新词条同键操作状态::资源失败) throw std::bad_alloc{};
    if (p.状态 == 新词条同键操作状态::未找到) return false;
    if (p.状态 != 新词条同键操作状态::已读取 || !p.信息)
        throw std::logic_error("word key condition definition unavailable");
    // C的正式引用由最终L1发布保障；不把来源W的可变字段或旧整组快照加入门禁。
    return true;
}

bool 方法词条范围属于(稳定编码 child, 稳定编码 parent, const 概念_存在类& owner) {
    // 保留窄读口的资源/依赖状态，不把optional包含口的未知折成否定或结构损坏。
    const std::array<稳定编码,2> nodes{child,parent};
    std::array<bool,2> all{};
    std::array<std::optional<新词条同键信息>,2> keys;
    for (std::size_t i = 0; i < nodes.size(); ++i) {
        const auto d = owner.读取当前基础范围概念(nodes[i]);
        if (d.状态 == 新基础范围读取状态::资源失败) throw std::bad_alloc{};
        if (d.状态 == 新基础范围读取状态::未找到) return false;
        if (d.状态 == 新基础范围读取状态::依赖未就绪)
            throw std::logic_error("word condition containment reader unavailable");
        if (!d.定义) return false;
        if (d.定义->原语 == 基础概念原语::具名领域身份
            && d.定义->整数参数 == std::vector<std::int64_t>{16}) all[i] = true;
        else if (d.定义->原语 == 基础概念原语::词条同键) {
            const auto key = owner.读取词条同键范围(nodes[i]);
            if (key.状态 == 新词条同键操作状态::资源失败) throw std::bad_alloc{};
            if (key.状态 == 新词条同键操作状态::未找到) return false;
            if (key.状态 != 新词条同键操作状态::已读取 || !key.信息)
                throw std::logic_error("word condition containment source unavailable");
            keys[i] = key.信息;
        } else return false;
    }
    if (all[1]) return true;
    if (all[0]) return false;
    return keys[0]->词条语言 == keys[1]->词条语言
        && keys[0]->规范化合同 == keys[1]->规范化合同
        && keys[0]->原规范化词面 == keys[1]->原规范化词面;
}

bool 方法持有者范围有效(const 新方法有序参数规格& 规格, const 概念_存在类& owner) {
    if (规格.信息类型 == 新方法参数信息类型::存在) return 方法存在范围有效(规格.范围概念节点, owner);
    if (规格.信息类型 != 新方法参数信息类型::词条) return false;
    const auto 当前 = owner.读取词条身份范围概念保护();
    return 当前.信息 && 当前.信息->节点 == 规格.范围概念节点;
}

bool 追加方法持有者概念保护(稳定编码 C,const 概念_存在类& owner,
    std::vector<基础联合节点快照>& protection) {
    if(追加方法词条范围保护(C,owner,protection))return true;
    return 追加方法存在范围保护(C,owner,protection);
}

bool 持有者特征类型允许(新方法参数信息类型 类型,稳定编码 FT,const 概念_特征类& owner) {
    if(类型==新方法参数信息类型::存在)return true;
    if(类型!=新方法参数信息类型::词条)return false;
    return 词条参数特征类型允许(FT, owner);
}

std::pair<新方法操作状态, std::optional<新词条特征信息>> 读取方法词条实参(
    const 新_词类* 词条, const 新方法有序参数规格& 规格, 稳定编码 节点,
    const 概念_存在类& 概念, const 新_特征类&) {
    using S = 新方法操作状态;
    if (!词条) return {S::尚未初始化, {}};
    if (!方法持有者范围有效(规格, 概念)) return {S::参数概念不相容, {}};
    auto 当前 = 词条->读取词条特征信息(节点);
    if (当前.状态 == 新词条查询状态::资源失败) return {S::资源失败, {}};
    if (当前.状态 == 新词条查询状态::未找到) return {S::参数引用失效, {}};
    if (当前.状态 != 新词条查询状态::已找到 || !当前.信息)
        throw std::runtime_error("word parameter owner read failed");
    if (!规格.必需特征类型组.empty()) {
        for (const auto FT : 规格.必需特征类型组)
            if (std::none_of(当前.信息->特征组.begin(), 当前.信息->特征组.end(),
                [&](const auto& 项) { return 项.特征类型根 == FT; }))
                return {S::参数材料未到, {}};
    }
    // 名称集合为空也是合法W参数；是否达到条件/目标由当前F求值负责。
    return {S::已找到, std::move(当前.信息)};
}

新方法操作状态 正式参数状态(const 正式信息参数读取结果& r,稳定编码 N) {
    using S=正式信息参数读取状态;
    switch(r.状态) {
    case S::已读取:
        if(!r.信息||r.信息->信息节点!=N||!有效(r.信息->类别概念)||r.完整保护组.empty())
            throw std::runtime_error("formal information parameter success projection incomplete");
        return 新方法操作状态::已找到;
    case S::信息不存在:return 新方法操作状态::参数引用失效;
    case S::范围未支持:return 新方法操作状态::参数概念不相容;
    case S::前提已变化:return 新方法操作状态::定义前提已变化;
    case S::依赖未就绪:throw std::runtime_error("formal parameter reader dependencies unavailable");
    case S::资源失败:throw std::bad_alloc{};
    }
    throw std::runtime_error("formal parameter reader state invalid");
}

// 类别引用只由相应owner的唯一根证明解释；枚举仅用于现C++服务派发。
struct 参数类别概念保护 final {
    新方法参数信息类型 分派类型;
    稳定编码 类别概念;
    std::vector<基础联合节点快照> 完整保护组;
};

参数类别概念保护 读取参数类别概念保护(
    新方法参数信息类型 类型, const 概念_存在类& 存在, const 概念_特征类& 特征) {
    参数类别概念保护 结果{类型, {}, {}};
    if (类型 == 新方法参数信息类型::存在) {
        const auto p = 存在.读取存在根概念保护();
        if (p.状态 == 正式概念读取状态::前提已变化) throw 引用内容定义前提变化{};
        if (p.状态 != 正式概念读取状态::已读取 || !p.信息 || p.完整保护组.empty())
            throw std::runtime_error("parameter existence category root unavailable");
        结果.类别概念 = p.信息->节点;
        结果.完整保护组 = p.完整保护组;
    } else if (类型 == 新方法参数信息类型::特征) {
        const auto p = 特征.读取特征类别根概念保护();
        if (p.状态 == 正式概念读取状态::前提已变化) throw 引用内容定义前提变化{};
        if (p.状态 != 正式概念读取状态::已读取 || !p.信息 || p.完整保护组.empty())
            throw std::runtime_error("parameter feature category root unavailable");
        结果.类别概念 = p.信息->节点;
        结果.完整保护组 = p.完整保护组;
    } else if (类型 == 新方法参数信息类型::词条) {
        const auto p = 存在.读取词条身份范围概念保护();
        if (!p.信息) throw std::runtime_error("word parameter category unavailable");
        结果.类别概念 = p.信息->节点;
        结果.完整保护组 = p.完整保护组;
    } else throw std::invalid_argument("unsupported parameter category dispatch hint");
    return 结果;
}

参数类别概念保护 解析参数类别概念保护(
    稳定编码 类别, const 概念_存在类& 存在, const 概念_特征类& 特征) {
    auto e = 读取参数类别概念保护(新方法参数信息类型::存在, 存在, 特征);
    auto f = 读取参数类别概念保护(新方法参数信息类型::特征, 存在, 特征);
    if (e.类别概念 == f.类别概念)
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取方法参数类别", L"存在与特征类别根身份冲突。");
    if (类别 == e.类别概念) {
        return e;
    }
    if (类别 == f.类别概念) {
        return f;
    }
    const auto w = 存在.读取词条身份范围概念保护();
    if (w.信息 && 类别 == w.信息->节点)
        return {新方法参数信息类型::词条, 类别, w.完整保护组};
    工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取方法参数类别", L"持久类别引用不是本版本准入的正式类别根。");
}

// 新旧字段严格二选一。旧整数只读派发，不给旧材料补写新字段。
std::optional<新方法参数信息类型> 读取持久参数类别(
    稳定编码 节点, 稳定编码 旧字段, 稳定编码 新字段,
    const 概念_存在类& 存在, const 概念_特征类& 特征) {
    const auto 旧 = 全局基础数据集.查询字段(节点, 旧字段);
    const auto 新 = 全局基础数据集.查询字段(节点, 新字段);
    if (旧.empty() == 新.empty())
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取方法参数类别", L"新旧类别字段必须恰有一种，不能双写或全缺。");
    if (!新.empty()) {
        const auto c = 读取唯一节点字段(节点, 新字段);
        if (!c)
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取方法参数类别", L"类别概念字段不是唯一合法节点引用。");
        return 解析参数类别概念保护(*c, 存在, 特征).分派类型;
    }
    const auto v = 读取唯一I64字段(节点, 旧字段);
    if (!v || (*v != 1 && *v != 2 && *v != 3))
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取方法参数类别", L"旧类别字段不是唯一合法的兼容类型值。");
    return static_cast<新方法参数信息类型>(*v);
}
enum class 引用内容条件校验模式 : std::uint8_t { 持久结构=1, 完整语义 };

bool 读取引用内容条件定义保护(const 新方法引用内容条件& 内容,
    const 概念_特征类& 特征概念,const 概念_存在类& 存在概念,
    std::vector<基础联合节点快照>& 保护) {
    if(内容.正式内容路径.size()>1||(内容.作用点!=新方法内容作用点::特征当前引用
        &&内容.作用点!=新方法内容作用点::参数自身))return false;
    if(内容.要求认知来源当前有效) {
        if(内容.作用点!=新方法内容作用点::参数自身||!内容.正式内容路径.empty())return false;
        const auto full=存在概念.读取引用内容筹办概念();
        if(full.状态==正式结构操作状态::前提已变化)throw 引用内容定义前提变化{};
        if(full.状态!=正式结构操作状态::已读取||!full.信息||full.完整保护组.empty())
            throw std::runtime_error("method current provenance condition complete concept unavailable");
        if(full.信息->完整被认知需求!=内容.要求概念节点)return false;
        保护.insert(保护.end(),full.完整保护组.begin(),full.完整保护组.end());
    }
    const auto 定义=存在概念.读取正式结构概念保护(内容.要求概念节点);
    if(定义.状态==正式结构操作状态::前提已变化)throw 引用内容定义前提变化{};
    if(定义.状态==正式结构操作状态::未找到
        ||定义.状态==正式结构操作状态::输入拒绝)return false;
    if(定义.状态!=正式结构操作状态::已读取||!定义.信息||定义.完整保护组.empty())
        throw std::runtime_error("content condition formal definition read failed");
    const bool 结构=std::holds_alternative<信息结构概念定义>(定义.信息->定义);
    if(!结构&&!std::holds_alternative<基础范围概念定义>(定义.信息->定义))return false;
    保护.insert(保护.end(),定义.完整保护组.begin(),定义.完整保护组.end());
    if(结构) {
        const auto 有效内容=存在概念.读取正式结构有效内容保护(内容.要求概念节点);
        if(有效内容.状态==正式结构有效内容读取状态::资源失败)throw std::bad_alloc{};
        if(有效内容.状态==正式结构有效内容读取状态::前提已变化)throw 引用内容定义前提变化{};
        if(有效内容.状态!=正式结构有效内容读取状态::已读取||!有效内容.信息||有效内容.完整保护组.empty())
            throw std::runtime_error("content condition effective definition read failed");
        保护.insert(保护.end(),有效内容.完整保护组.begin(),有效内容.完整保护组.end());
    }
    if(!内容.正式内容路径.empty()) {
        for(std::uint32_t 槽=先天特征类型槽::需求主体;槽<=先天特征类型槽::内容确认来源;++槽) {
            const auto FT=特征概念.读取先天特征类型槽(槽);
            if(FT.状态==特征概念规则状态::资源失败)throw std::bad_alloc{};
            if(FT.状态==特征概念规则状态::前提已变化)throw 引用内容定义前提变化{};
            if(FT.状态==特征概念规则状态::值不存在
                ||FT.状态==特征概念规则状态::入口拒绝)return false;
            if(FT.状态!=特征概念规则状态::已完成||!FT.类型根||FT.完整保护组.empty())
                throw std::runtime_error("content condition innate FT read failed");
            保护.insert(保护.end(),FT.完整保护组.begin(),FT.完整保护组.end());
            if(*FT.类型根!=内容.正式内容路径.front())continue;
            const auto* 基础=std::get_if<基础范围概念定义>(&定义.信息->定义);
            if(!基础)return true; // 结构范围仍由正式内容求值器解释，不压成标量域。
            using namespace 先天特征类型槽;
            using 值类=正式内容值种类;
            const auto 值种类=[&] {
                switch(槽) {
                case 需求主体:case 需求目标存在:return 值类::存在引用;
                case 需求目标FT:case 目标域:case 目标比较:return 值类::概念引用;
                case 父需求:return 值类::需求引用;
                case 理解来源:return 值类::语言理解引用;
                case 内容确认来源:return 值类::内容确认引用;
                default:return 值类::I64数值;
                }
            }();
            switch(基础->原语) {
            case 基础概念原语::I64闭区间:case 基础概念原语::I64有限枚举:
                return 值种类==值类::I64数值;
            case 基础概念原语::合法存在:case 基础概念原语::唯一存在:
                return 值种类==值类::存在引用;
            case 基础概念原语::合法FT根:case 基础概念原语::合法特征概念:
            case 基础概念原语::合法比较关系:case 基础概念原语::正式概念身份:
                return 值种类==值类::概念引用;
            case 基础概念原语::正式信息身份:return 值种类!=值类::I64数值;
            case 基础概念原语::具名领域身份:
                if(基础->整数参数.size()!=1)return false;
                if(基础->整数参数.front()==2)return 值种类==值类::语言理解引用;
                if(基础->整数参数.front()==3)return 值种类==值类::内容确认引用;
                if(基础->整数参数.front()==15)return 值种类==值类::需求引用;
                return 值种类!=值类::I64数值; // 其它正式身份保留求值器的未支持边界。
            }
            return false;
        }
        return false;
    }
    if(!结构)return false;
    const auto 根=存在概念.读取先天需求任务方法概念();
    if(根.状态==正式结构操作状态::前提已变化)throw 引用内容定义前提变化{};
    if(根.状态==正式结构操作状态::未找到
        ||根.状态==正式结构操作状态::输入拒绝)return false;
    if(根.状态!=正式结构操作状态::已读取||!根.信息)
        throw std::runtime_error("content condition innate demand root read failed");
    std::set<稳定编码> 已读;std::vector<稳定编码> 待读{内容.要求概念节点};bool 命中=false;
    while(!待读.empty()) {
        const auto n=待读.back();待读.pop_back();if(!已读.insert(n).second)continue;
        const auto r=存在概念.读取正式结构概念保护(n);
        if(r.状态==正式结构操作状态::前提已变化)throw 引用内容定义前提变化{};
        if(r.状态!=正式结构操作状态::已读取||!r.信息||r.完整保护组.empty())
            throw std::runtime_error("content condition declared ancestor read failed");
        const auto* d=std::get_if<信息结构概念定义>(&r.信息->定义);if(!d)return false;
        保护.insert(保护.end(),r.完整保护组.begin(),r.完整保护组.end());
        if(n==根.信息->需求)命中=true;
        待读.insert(待读.end(),d->上位结构概念组.begin(),d->上位结构概念组.end());
    }
    return 命中;
}

bool 正式需求范围有效(稳定编码 范围,const 概念_特征类& f,const 概念_存在类& e) {
    新方法引用内容条件 c;c.要求概念节点=范围;c.作用点=新方法内容作用点::参数自身;
    std::vector<基础联合节点快照> p;
    return 读取引用内容条件定义保护(c,f,e,p);
}
bool 追加需求主体路径保护(稳定编码 FT,const 概念_特征类& f,const 概念_存在类& e,
    std::vector<基础联合节点快照>& p) {
    const auto slot=f.读取先天特征类型槽(先天特征类型槽::需求主体);
    if(slot.状态==特征概念规则状态::前提已变化)throw 引用内容定义前提变化{};
    if(slot.状态==特征概念规则状态::资源失败)throw std::bad_alloc{};
    if(slot.状态!=特征概念规则状态::已完成||!slot.类型根||slot.完整保护组.empty())
        throw std::runtime_error("method subject relation innate FT unavailable");
    if(*slot.类型根!=FT)return false;
    const auto def=f.读取特征概念定义保护(FT);
    if(def.前提已变化)throw 引用内容定义前提变化{};
    if(def.状态!=基础结构操作状态::已完成||!def.信息||def.预期节点组.empty())
        throw std::runtime_error("method subject relation FT definition unavailable");
    const auto* domain=std::get_if<特征概念信息引用值域>(&def.信息->定义.值域);
    if(def.信息->定义.材料类型!=特征概念材料物理类型::信息节点引用
        ||def.信息->定义.比较规则!=特征概念比较规则::信息引用身份精确
        ||!domain||domain->语义!=信息引用语义::信息对象)return false;
    const auto target=e.读取正式结构概念保护(domain->内容概念);
    if(target.状态==正式结构操作状态::前提已变化)throw 引用内容定义前提变化{};
    if(target.状态!=正式结构操作状态::已读取||!target.信息||target.完整保护组.empty())
        throw std::runtime_error("method subject content range unavailable");
    const auto* base=std::get_if<基础范围概念定义>(&target.信息->定义);
    if(!base||base->原语!=基础概念原语::合法存在)return false;
    p.insert(p.end(),slot.完整保护组.begin(),slot.完整保护组.end());
    p.insert(p.end(),def.预期节点组.begin(),def.预期节点组.end());
    p.insert(p.end(),target.完整保护组.begin(),target.完整保护组.end());
    return true;
}

#ifdef HY_EGO_DEBUG_LOG_SIMULATION_PROGRESS
thread_local const wchar_t* 角色声明失败点=nullptr;
#define HY_ROLE_POINT(x) (角色声明失败点=(x))
#else
#define HY_ROLE_POINT(x) ((void)0)
#endif
bool 条件结果角色已声明(
    const std::vector<新方法参数规格>& 参数规格组,
    const 新方法条件结果定义& 定义, const 概念_特征类& 概念服务,
    const 概念_存在类& 存在概念服务,
    引用内容条件校验模式 模式=引用内容条件校验模式::完整语义) {
    try {
    HY_ROLE_POINT(nullptr);

    const auto 找规格 = [&](std::uint32_t n) -> const 新方法有序参数规格* {
        const 新方法有序参数规格* found = nullptr;
        for (const auto& s : 参数规格组) if (s.有序规格 && s.有序规格->序号 == n) {
            if (found) return nullptr; found = &*s.有序规格;
        }
        return found;
    };
    for(const auto* group:{&定义.条件.场景事实限定组,&定义.结果.场景事实限定组}) {
      std::vector<新方法初始场景事实限定> 已核限定;
      for (const auto& r : *group) {
        if ((group==&定义.条件.场景事实限定组&&定义.条件.关系规则 != 新方法项关系规则::全部成立)
            || std::find(已核限定.begin(), 已核限定.end(), r) != 已核限定.end()) return false;
        const auto proof = 存在概念服务.读取已确认消息连接概念保护();
        if (!proof.信息 || proof.完整保护组.empty() || proof.信息->节点 != r.关系概念节点) return false;
        const std::array positions{r.场景参数序号,r.主体参数序号,r.对象参数序号,r.特征参数序号};
        for (std::size_t i=0;i<positions.size();++i) {
            const auto* p=找规格(positions[i]);
            if (!p || p->信息类型 != (i==3 ? 新方法参数信息类型::特征 : 新方法参数信息类型::存在)) return false;
        }
        const auto* f=找规格(r.特征参数序号);
        const auto fd=概念服务.获取特征概念(f->范围概念节点);
        if (!fd || fd->材料类型!=特征概念材料物理类型::存在节点引用
            || fd->比较规则!=特征概念比较规则::存在身份精确 || r.主体参数序号==r.对象参数序号) return false;
        已核限定.push_back(r);
      }
    }
    const auto 包含存在范围 = [&](稳定编码 范围, 稳定编码 所需上位) {
        if (!存在概念服务.是存在概念节点(范围) || !存在概念服务.是存在概念节点(所需上位)) return false;
        std::map<稳定编码, std::uint8_t> mark;
        bool reached = false;
        const auto visit = [&](auto&& self, 稳定编码 n) -> void {
            const auto old = mark[n];
            if (old == 1) 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"核验方法存在引用关系", L"正式存在概念上位链有环。");
            if (old == 2) return;
            mark[n] = 1;
            const auto c = 存在概念服务.获取存在概念(n);
            if (!c) 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"核验方法存在引用关系", L"正式存在概念上位链读取失败。");
            if (n == 所需上位) reached = true;
            for (const auto parent : c->上位概念组) self(self, parent);
            mark[n] = 2;
        };
        visit(visit, 范围);
        return reached;
    };
    // 被引用参数与引用特征值域相容：声明范围属于值域；或值域本身在声明范围之下（引用关系成立即把该参数
    // 收窄到值域，如范围“存在”的参数被“人类”值域的特征引用）；或本配对合取条件中对该参数的存在概念条件项属于值域。
    // 未满足上述可证明的相容条件时判不合法。
    const bool 条件合取=定义.条件.关系规则==新方法项关系规则::全部成立||定义.条件.关系规则==新方法项关系规则::单项;
    const auto 参数范围属于=[&](std::uint32_t 序号,稳定编码 声明范围,稳定编码 所需上位) {
        if(包含存在范围(声明范围,所需上位)||包含存在范围(所需上位,声明范围))return true;
        if(!条件合取)return false;
        return std::any_of(定义.条件.条件项组.begin(),定义.条件.条件项组.end(),[&](const auto& c) {
            return c.参数序号==序号&&c.存在概念节点&&!c.特征类型概念节点&&!c.引用内容条件
                &&包含存在范围(*c.存在概念节点,所需上位);
        });
    };
    for(const auto* group:{&定义.条件.场景事实限定组,&定义.结果.场景事实限定组})for(const auto& r:*group) {
        const auto* f=找规格(r.特征参数序号);
        const auto fd=概念服务.获取特征概念(f->范围概念节点);
        const auto* domain=fd?std::get_if<特征概念存在引用值域>(&fd->值域):nullptr;
        const auto* peer=找规格(r.对象参数序号);
        if(!domain||!peer||!参数范围属于(r.对象参数序号,peer->范围概念节点,domain->存在概念节点)){HY_ROLE_POINT(L"场景事实对象范围");return false;}
    }
    const auto 引用声明合法 = [&](const 新方法参数关系& r) {
        const auto* l = 找规格(r.左参数序号); const auto* e = 找规格(r.右参数序号);
        if (!l || !e || l->信息类型 != 新方法参数信息类型::特征 || e->信息类型 != 新方法参数信息类型::存在) return false;
        const auto f = 概念服务.获取特征概念(l->范围概念节点);
        if (!f && 概念服务.是特征概念节点(l->范围概念节点))
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"核验方法存在引用关系", L"正式特征参数范围读取失败。");
        const auto root = f ? 读取线性特征类型根(f->节点, 概念服务) : std::nullopt;
        if (f && !root)
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"核验方法存在引用关系", L"正式特征类型根或上位结构读取失败。");
        if(f&&root&&f->材料类型==特征概念材料物理类型::需求节点引用
            &&f->比较规则==特征概念比较规则::需求身份精确) {
            const auto* domain=std::get_if<特征概念信息引用值域>(&f->值域);
            if(!正式需求范围有效(e->范围概念节点,概念服务,存在概念服务))return false;
            if(std::holds_alternative<特征概念全材料值域>(f->值域))return true;
            if(!domain||domain->语义!=信息引用语义::信息对象)return false;
            // 正式需求的范围包含由结构概念上位闭包核验，不送普通存在树。
            std::set<稳定编码> seen;std::vector<稳定编码> todo{e->范围概念节点};
            bool found=false;
            while(!todo.empty()) {
                const auto n=todo.back();todo.pop_back();if(!seen.insert(n).second)continue;
                const auto d=存在概念服务.读取正式结构概念保护(n);
                if(d.状态==正式结构操作状态::前提已变化)throw 引用内容定义前提变化{};
                if(d.状态!=正式结构操作状态::已读取||!d.信息)throw std::runtime_error("method demand reference range unreadable");
                const auto* s=std::get_if<信息结构概念定义>(&d.信息->定义);if(!s)return false;
                if(n==domain->内容概念)found=true;
                todo.insert(todo.end(),s->上位结构概念组.begin(),s->上位结构概念组.end());
            }
            return found;
        }
        const auto* domain = f ? std::get_if<特征概念存在引用值域>(&f->值域) : nullptr;
        return f && root && domain && f->材料类型 == 特征概念材料物理类型::存在节点引用
            && f->比较规则 == 特征概念比较规则::存在身份精确
            && 参数范围属于(r.右参数序号, e->范围概念节点, domain->存在概念节点);
    };

    const auto 关系已声明=[&](const 新方法参数关系& r) {
        const auto 类型=[&](std::uint32_t n)->std::optional<新方法参数信息类型> {
            for(const auto& s:参数规格组) if(s.有序规格&&s.有序规格->序号==n) return s.有序规格->信息类型;
            return {};
        };
        const auto l=类型(r.左参数序号),v=类型(r.右参数序号);
        if(!l||!v) return false;
        switch(r.种类) {
        case 新方法参数关系种类::同一:
        case 新方法参数关系种类::不同: return l==v;
        case 新方法参数关系种类::集合包含存在:
            return *l==新方法参数信息类型::特征&&*v==新方法参数信息类型::存在;
        case 新方法参数关系种类::特征引用存在:
            return 引用声明合法(r);
        case 新方法参数关系种类::内容引用参数: {
            const auto* left=找规格(r.左参数序号);const auto* right=找规格(r.右参数序号);
            if(!left||!right||*l!=新方法参数信息类型::存在||*v!=新方法参数信息类型::存在
                ||r.左正式内容路径.size()!=1
                ||!正式需求范围有效(left->范围概念节点,概念服务,存在概念服务))return false;
            std::vector<基础联合节点快照> proof;
            if(!追加需求主体路径保护(r.左正式内容路径.front(),概念服务,存在概念服务,proof))return false;
            const auto ft=概念服务.获取特征概念(r.左正式内容路径.front());
            const auto* domain=ft?std::get_if<特征概念信息引用值域>(&ft->值域):nullptr;
            if(!domain||!存在概念服务.是存在概念节点(right->范围概念节点))return false;
            const auto target=存在概念服务.读取正式结构概念保护(domain->内容概念);
            if(target.状态==正式结构操作状态::前提已变化)throw 引用内容定义前提变化{};
            if(target.状态!=正式结构操作状态::已读取||!target.信息)throw std::runtime_error("method subject content range unavailable");
            const auto* base=std::get_if<基础范围概念定义>(&target.信息->定义);
            return base&&base->原语==基础概念原语::合法存在;
        }
        default: return false; // 特征所属存在、同属场景、场景包含存在已退役（R97）
        }
    };
    if(!std::all_of(定义.条件.参数关系组.begin(),定义.条件.参数关系组.end(),关系已声明)){HY_ROLE_POINT(L"条件关系已声明");return false;}
    if(!std::all_of(定义.结果.参数关系组.begin(),定义.结果.参数关系组.end(),关系已声明)){HY_ROLE_POINT(L"结果关系已声明");return false;}

    const auto 引用组合法 = [&](const std::vector<新方法参数关系>& group, bool result) {
        if (!参数关系结构有效(group)) return false;
        if (std::none_of(group.begin(), group.end(), [](const auto& r) {
            return r.种类 == 新方法参数关系种类::特征引用存在;
        })) return true; // 旧1..5的组合与未知项语义原样保留。
        const auto rule = result ? 定义.结果.关系规则 : 定义.条件.关系规则;
        if (rule != 新方法项关系规则::单项 && rule != 新方法项关系规则::全部成立) return false;
        std::map<std::uint32_t, std::uint32_t> alias;
        for (const auto& s : 参数规格组) if (s.有序规格) alias.emplace(s.有序规格->序号, s.有序规格->序号);
        const auto root = [&](std::uint32_t n) {
            for (;;) { const auto p = alias.find(n); if (p == alias.end()) return std::uint32_t{0}; if (p->second == n) return n; n = p->second; }
        };
        auto aliases = group;
        if (result && (定义.条件.关系规则 == 新方法项关系规则::单项
            || 定义.条件.关系规则 == 新方法项关系规则::全部成立)) for (const auto& r : 定义.条件.参数关系组)
            if (r.种类 == 新方法参数关系种类::同一 || r.种类 == 新方法参数关系种类::不同) aliases.push_back(r);
        for (const auto& r : aliases) if (r.种类 == 新方法参数关系种类::同一) {
            const auto l = root(r.左参数序号), e = root(r.右参数序号); if (!l || !e) return false;
            alias[(std::max)(l, e)] = (std::min)(l, e);
        }
        for (const auto& r : aliases) if (r.种类 == 新方法参数关系种类::不同
            && root(r.左参数序号) == root(r.右参数序号)) return false;
        std::map<std::uint32_t, std::uint32_t> targets;
        for (const auto& r : group) if (r.种类 == 新方法参数关系种类::特征引用存在) {
            const auto l = root(r.左参数序号), e = root(r.右参数序号);
            const auto [it, inserted] = targets.emplace(l, e);
            if (!l || !e || (!inserted && it->second != e)) return false;
            const auto* spec = 找规格(r.左参数序号);
            const auto ft = spec ? 读取线性特征类型根(spec->范围概念节点, 概念服务) : std::nullopt;
            if (result && (!ft || std::none_of(定义.结果.结果项组.begin(), 定义.结果.结果项组.end(), [&](const auto& item) {
                if(item.引用内容结果)return item.参数序号&&root(item.参数序号)==l&&!item.任务成员变量序号;
                return item.参数序号 && root(item.参数序号) == l && !item.任务成员变量序号 && item.特征类型概念节点
                    && !item.特征值域概念节点 && !item.精确数值域 && !item.变化方向概念节点 && !item.状态或动态概念节点
                    && 读取线性特征类型根(*item.特征类型概念节点, 概念服务) == ft;
            }))) return false;
        }
        if (result) for (const auto& item : 定义.结果.结果项组) {
            if (!item.特征类型概念节点 || item.特征值域概念节点 || item.精确数值域 || item.变化方向概念节点 || item.状态或动态概念节点) continue;
            const auto type = 概念服务.获取特征概念(*item.特征类型概念节点);
            if (!type) return false;
            if (type->材料类型 != 特征概念材料物理类型::存在节点引用) continue;
            // 旧无值结果仍是未知项；只有位置关系6才将其补为完整引用效果。
            if (!targets.contains(root(item.参数序号))) continue;
            if (!item.参数序号 || item.任务成员变量序号
                || type->比较规则 != 特征概念比较规则::存在身份精确) return false;
            const auto* spec = 找规格(item.参数序号);
            if (!spec || 读取线性特征类型根(spec->范围概念节点, 概念服务)
                != 读取线性特征类型根(*item.特征类型概念节点, 概念服务)) return false;
        }
        return true;
    };
    if (!引用组合法(定义.条件.参数关系组, false)){HY_ROLE_POINT(L"条件引用组合法");return false;}
    if (!引用组合法(定义.结果.参数关系组, true)){HY_ROLE_POINT(L"结果引用组合法");return false;}
    HY_ROLE_POINT(L"项已声明");

    const auto 已声明 = [&](const auto& 项) {
        if constexpr(requires{项.结果作用存在;}) if(项.结果作用存在) {
            // 结果不依附参数：特征类型须是类型根，值域须在该类型根下。
            if(!结果项有效(项,概念服务))return false;
            const auto 根=读取线性特征类型根(*项.特征类型概念节点,概念服务);
            return 根&&*根==*项.特征类型概念节点
                &&(!项.特征值域概念节点||读取线性特征类型根(*项.特征值域概念节点,概念服务)==根);
        }
        if constexpr(requires{项.被动需求目标;}) if(项.被动需求目标) {
            // 先天被动结果只作用于存在类正式信息参数（补问叶子需求自身）；与任务侧正式信息参数
            // 判据一致：范围本身是信息结构概念。需求根范围的上位不是正式结构，不走上位链核验。
            if(!结果项有效(项,概念服务))return false;
            const auto* p=找规格(项.参数序号);
            if(!p||p->信息类型!=新方法参数信息类型::存在)return false;
            const auto 范围=存在概念服务.读取正式结构概念保护(p->范围概念节点);
            if(范围.状态==正式结构操作状态::前提已变化)throw 引用内容定义前提变化{};
            return 范围.状态==正式结构操作状态::已读取&&范围.信息
                &&std::holds_alternative<信息结构概念定义>(范围.信息->定义)
                &&std::find(定义.结果.确认要求.禁止结果项组.begin(),定义.结果.确认要求.禁止结果项组.end(),项)
                    ==定义.结果.确认要求.禁止结果项组.end();
        }
        if constexpr(requires{项.引用内容结果;}) if(项.引用内容结果) {
            if(!结果项有效(项,概念服务))return false;
            const auto* p=找规格(项.参数序号);
            if(!p||p->信息类型!=新方法参数信息类型::特征)return false;
            const auto ft=概念服务.获取特征概念(p->范围概念节点);
            if(!ft||ft->材料类型!=特征概念材料物理类型::需求节点引用
                ||ft->比较规则!=特征概念比较规则::需求身份精确)return false;
            const auto known=存在概念服务.读取引用内容筹办概念();
            if(known.状态==正式结构操作状态::前提已变化)throw 引用内容定义前提变化{};
            if(known.状态!=正式结构操作状态::已读取||!known.信息||known.完整保护组.empty())
                throw std::runtime_error("method complete demand concept unavailable");
            if(项.引用内容结果->要求概念节点!=known.信息->完整被认知需求)return false;
            std::size_t count=0;
            for(const auto& r:定义.结果.参数关系组)if(r.种类==新方法参数关系种类::特征引用存在
                &&r.左参数序号==项.参数序号)++count;
            if(count!=1||std::find(定义.结果.确认要求.禁止结果项组.begin(),定义.结果.确认要求.禁止结果项组.end(),项)
                !=定义.结果.确认要求.禁止结果项组.end())return false;
            std::vector<基础联合节点快照> proof;
            return 读取引用内容条件定义保护(*项.引用内容结果,概念服务,存在概念服务,proof);
        }
        if constexpr(requires{项.特征前态限定;}) if(项.特征前态限定) {
            const auto* p=找规格(项.参数序号);
            if(!p||p->信息类型!=新方法参数信息类型::特征)return false;
            const auto ft=概念服务.获取特征概念(p->范围概念节点);
            if(!ft||ft->材料类型!=特征概念材料物理类型::需求节点引用
                ||ft->比较规则!=特征概念比较规则::需求身份精确)return false;
            if(项.特征类型概念节点&&读取线性特征类型根(*项.特征类型概念节点,概念服务)
                !=读取线性特征类型根(p->范围概念节点,概念服务))return false;
            return 项.特征前态限定->当前值状态==新方法当前值状态::未知
                &&项.特征前态限定->历史不同值数量==std::optional<std::uint64_t>{0};
        }
        if constexpr(requires{项.引用内容条件;}) if(项.引用内容条件) {
            if(!项.参数序号||有效(项.参数角色节点)||项.任务成员变量序号
                ||项.存在概念节点||项.特征类型概念节点||项.特征值域概念节点
                ||项.精确数值域||项.二次特征概念节点||项.状态或动态概念节点)return false;
            if(!节点仍存在(项.引用内容条件->要求概念节点)||项.引用内容条件->正式内容路径.size()>1
                ||std::any_of(项.引用内容条件->正式内容路径.begin(),项.引用内容条件->正式内容路径.end(),
                    [](稳定编码 FT){return !节点仍存在(FT);}))return false;
            const auto* 参数=找规格(项.参数序号);
            if(项.引用内容条件->作用点==新方法内容作用点::参数自身) {
                if(!参数||参数->信息类型!=新方法参数信息类型::存在
                    ||!正式需求范围有效(参数->范围概念节点,概念服务,存在概念服务))return false;
                std::vector<基础联合节点快照> proof;
                return 读取引用内容条件定义保护(*项.引用内容条件,概念服务,存在概念服务,proof);
            }
            if(项.引用内容条件->作用点!=新方法内容作用点::特征当前引用)return false;
            if(!参数||参数->信息类型!=新方法参数信息类型::特征)return false;
            const auto FT=概念服务.获取特征概念(参数->范围概念节点);
            if(!FT||FT->材料类型!=特征概念材料物理类型::需求节点引用
                ||FT->比较规则!=特征概念比较规则::需求身份精确)return false;
            if(定义.条件.关系规则!=新方法项关系规则::全部成立
                &&定义.条件.关系规则!=新方法项关系规则::单项)return false;
            std::map<std::uint32_t,std::uint32_t> 别名;
            for(const auto& s:参数规格组)if(s.有序规格)别名.emplace(s.有序规格->序号,s.有序规格->序号);
            const auto 根=[&](std::uint32_t n){for(;;){const auto i=别名.find(n);if(i==别名.end())return std::uint32_t{0};if(i->second==n)return n;n=i->second;}};
            for(const auto& r:定义.条件.参数关系组)if(r.种类==新方法参数关系种类::同一) {
                const auto l=根(r.左参数序号),v=根(r.右参数序号);if(!l||!v)return false;
                别名[(std::max)(l,v)]=(std::min)(l,v);
            }
            for(const auto& r:定义.条件.参数关系组)
                if(r.种类==新方法参数关系种类::不同&&根(r.左参数序号)==根(r.右参数序号))return false;
            // R97：内容特征的所属存在由规格的所属存在位置唯一给出。
            {
                const auto* E=找规格(参数->所属存在位置);
                if(!E||E->信息类型!=新方法参数信息类型::存在)return false;
            }
            if(模式==引用内容条件校验模式::持久结构)return true;
            std::vector<基础联合节点快照> 保护;
            return 读取引用内容条件定义保护(*项.引用内容条件,概念服务,存在概念服务,保护);
        }
        const auto 角色 = 项.参数角色节点;
        const auto 序号 = 项.参数序号;
        return std::any_of(参数规格组.begin(), 参数规格组.end(),
            [&](const 新方法参数规格& 规格) {
                if (!序号) return !规格.有序规格 && 规格.参数角色节点 == 角色;
                if (有效(角色) || !规格.有序规格 || 规格.有序规格->序号 != 序号) return false;
                if (项.任务成员变量序号) {
                    const auto 外层 = 概念服务.获取特征概念(规格.有序规格->范围概念节点);
                    const auto 内层 = 项.特征类型概念节点 ? 概念服务.获取特征概念(*项.特征类型概念节点) : std::nullopt;
                    return 项.任务成员变量序号 == 1 && 规格.有序规格->信息类型 == 新方法参数信息类型::特征
                        && 外层 && 外层->比较规则 == 特征概念比较规则::服务任务集合身份判等
                        && 内层 && 内层->材料类型 == 特征概念材料物理类型::I64标量;
                }
                if (是特征持有参数(规格.有序规格->信息类型)) {
                    if((项.特征值域概念节点||项.精确数值域)&&!项.特征类型概念节点)return false;
                    if constexpr(requires{项.二次特征概念节点;})
                        if(项.二次特征概念节点&&!项.特征类型概念节点)return false;
                    if constexpr(requires{项.变化方向概念节点;})
                        if(项.变化方向概念节点&&!项.特征类型概念节点)return false;
                    if(规格.有序规格->信息类型==新方法参数信息类型::词条) {
                        if constexpr(requires{项.存在概念节点;})
                            if(项.存在概念节点 && !方法词条范围属于(*项.存在概念节点,
                                规格.有序规格->范围概念节点, 存在概念服务))return false;
                        if(项.特征类型概念节点&&!持有者特征类型允许(规格.有序规格->信息类型,*项.特征类型概念节点,概念服务))return false;
                    }
                    if(项.特征类型概念节点) {
                        const auto type=概念服务.获取特征概念(*项.特征类型概念节点);
                        if(!type||读取线性特征类型根(*项.特征类型概念节点,概念服务)!=项.特征类型概念节点)return false;
                        if(项.特征值域概念节点) {
                            const auto domain=概念服务.获取特征概念(*项.特征值域概念节点);
                            if(!domain||读取线性特征类型根(*项.特征值域概念节点,概念服务)!=项.特征类型概念节点
                                ||domain->材料类型!=type->材料类型||domain->比较规则!=type->比较规则||domain->单位!=type->单位)return false;
                        }
                        if(!精确数值域有效(项.精确数值域,项.特征类型概念节点,项.特征值域概念节点,概念服务))return false;
                    }
                } else if constexpr (requires { 项.存在概念节点; }) {
                    if (项.存在概念节点) return false;
                }
                return true;
            });
    };
    return std::all_of(定义.条件.条件项组.begin(),
               定义.条件.条件项组.end(),
               [&](const 新方法条件项定义& 项) {
                   return 已声明(项);
               })
        && std::all_of(定义.结果.结果项组.begin(),
               定义.结果.结果项组.end(),
               [&](const 新方法结果项定义& 项) {
                   return 已声明(项);
               })
        && std::all_of(定义.结果.确认要求.禁止结果项组.begin(),
               定义.结果.确认要求.禁止结果项组.end(),
               [&](const 新方法结果项定义& 项) {
                   return 已声明(项);
               });
    } catch (const 引用内容定义前提变化&) {
        throw;
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"核验方法参数关系", L"参数关系完整性核验遇到工程资源或读取异常。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"核验方法参数关系", L"参数关系完整性核验遇到工程资源或读取异常。");
    }
}

std::optional<std::vector<稳定编码>> 读取有序子节点(
    稳定编码 父节点, 稳定编码 子字段, const 概念_特征类& 概念服务) {
    const auto 子节点组 = 读取多节点字段(父节点, 子字段, 概念服务);
    if (!子节点组) return std::nullopt;
    std::vector<std::pair<std::int64_t, 稳定编码>> 排序组;
    for (const auto 子节点 : *子节点组) {
        const auto 顺序 = 读取唯一I64字段(子节点, 顺序字段);
        if (!顺序 || *顺序 < 0) return std::nullopt;
        排序组.emplace_back(*顺序, 子节点);
    }
    std::sort(排序组.begin(), 排序组.end());
    for (std::size_t i = 1; i < 排序组.size(); ++i) {
        if (排序组[i - 1].first == 排序组[i].first) return std::nullopt;
    }
    std::vector<稳定编码> 结果;
    for (const auto& [_, 节点] : 排序组) 结果.push_back(节点);
    return 结果;
}

std::optional<新方法参数规格> 读取参数规格(稳定编码 节点, const 概念_特征类& 概念服务,
    const 概念_存在类& 存在概念服务) {
    if (!节点类型为(节点, 参数规格节点类型)) return std::nullopt;
    const auto 序号 = 读取可选I64字段(节点, 有序参数序号字段);
    if (!全局基础数据集.查询字段(节点, 有序参数类别概念字段).empty()
        && (!序号 || !*序号 || **序号 <= 0
            || **序号 > (std::numeric_limits<std::uint32_t>::max)()))
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取方法参数类别", L"类别概念字段缺少合法有序参数序号。");
    if (!序号) return std::nullopt;
    if (*序号) {
        const auto 类型 = 读取持久参数类别(节点, 有序参数类型字段, 有序参数类别概念字段,
            存在概念服务, 概念服务);
        const auto 范围 = 读取唯一节点字段(节点, 有序参数范围字段);
        if (**序号 <= 0 || **序号 > (std::numeric_limits<std::uint32_t>::max)()
            || !类型 || !范围) return std::nullopt;
        for (const auto 字段 : {参数角色字段, 参数存在概念字段, 参数要求场景字段,
                参数对象种类字段, 参数特征规格字段, 参数集合角色字段, 参数集合特征类型字段})
            if (!全局基础数据集.查询字段(节点, 字段).empty()) return std::nullopt;
        const auto 必需类型 = 读取多节点字段(节点, 有序参数必需特征类型字段, 概念服务);
        const auto 所属位置 = 读取可选I64字段(节点, 有序参数所属位置字段);
        if (!必需类型 || !所属位置 || (*所属位置 && (**所属位置 <= 0
                || **所属位置 > (std::numeric_limits<std::uint32_t>::max)()))) return std::nullopt;
        新方法参数规格 新;
        新.有序规格 = 新方法有序参数规格{static_cast<std::uint32_t>(**序号),
            *类型, *范围, *必需类型,
            *所属位置 ? static_cast<std::uint32_t>(**所属位置) : 0u};
        return 参数规格有效({新}, 概念服务, false) ? std::optional{新} : std::nullopt;
    }
    if (!全局基础数据集.查询字段(节点, 有序参数类型字段).empty()
        || !全局基础数据集.查询字段(节点, 有序参数类别概念字段).empty()
        || !全局基础数据集.查询字段(节点, 有序参数范围字段).empty()) return std::nullopt;
    const auto 角色 = 读取唯一节点字段(节点, 参数角色字段);
    const auto 存在概念 = 读取可选节点字段(节点, 参数存在概念字段);
    const auto 要求场景 = 读取唯一I64字段(节点, 参数要求场景字段);
    const auto 对象种类 = 读取可选I64字段(节点, 参数对象种类字段);
    const auto 集合角色 = 读取可选节点字段(节点, 参数集合角色字段);
    const auto 集合类型 = 读取可选节点字段(节点, 参数集合特征类型字段);
    const auto 特征规格节点组 = 读取有序子节点(节点, 参数特征规格字段, 概念服务);
    if (!角色 || !存在概念 || !要求场景 || !对象种类 || !集合角色 || !集合类型
        || 有效(*集合角色) != 有效(*集合类型)
        || (*要求场景 != 0 && *要求场景 != 1) || !特征规格节点组) {
        return std::nullopt;
    }
    新方法参数规格 结果;
    结果.参数角色节点 = *角色;
    if (有效(*集合角色)) 结果.集合成员约束 = 新方法参数集合成员约束{*集合角色, *集合类型};
    if (有效(*存在概念)) 结果.存在概念节点 = *存在概念;
    结果.要求场景 = *要求场景 != 0;
    if (*对象种类) {
        const auto 值 = **对象种类;
        if (!是参数对象种类(值)) {
            return std::nullopt;
        }
        结果.对象种类 = static_cast<新方法参数对象种类>(值);
    }
    for (const auto 特征节点 : *特征规格节点组) {
        if (!节点类型为(特征节点, 参数特征规格节点类型)) {
            return std::nullopt;
        }
        const auto 类型 = 读取唯一节点字段(特征节点, 参数特征类型字段);
        const auto 值域 = 读取可选节点字段(特征节点, 参数特征值域字段);
        const auto 二次 = 读取多节点字段(特征节点, 参数二次特征概念字段, 概念服务);
        if (!类型 || !值域 || !二次) return std::nullopt;
        新方法参数特征规格 特征;
        特征.特征类型概念节点 = *类型;
        if (有效(*值域)) 特征.值域概念节点 = *值域;
        特征.二次特征概念组 = *二次;
        结果.特征规格组.push_back(std::move(特征));
    }
    if (!参数规格有效({结果}, 概念服务, false)) return std::nullopt;
    return 结果;
}

std::optional<新方法条件定义> 读取条件(稳定编码 节点, const 概念_特征类& 概念服务) {
    if (!节点类型为(节点, 条件节点类型)) return std::nullopt;
    const auto 规则 = 读取唯一I64字段(节点, 关系规则字段);
    const auto 项节点组 = 读取有序子节点(节点, 条件项字段, 概念服务);
    if (!规则 || !是关系规则(*规则) || !项节点组) return std::nullopt;
    新方法条件定义 结果;
    结果.关系规则 = static_cast<新方法项关系规则>(*规则);
    const auto 位置关系=读取参数关系(节点);
    if(!位置关系) return std::nullopt;
    结果.参数关系组=*位置关系;
    const auto 限定节点组=读取有序子节点(节点,初始场景限定字段,概念服务);
    if(!限定节点组)return std::nullopt;
    for(const auto n:*限定节点组) {
        if(!节点类型为(n,初始场景限定节点类型)
            ||读取唯一节点字段(n,初始声明所属字段)!=std::optional<稳定编码>{节点})return std::nullopt;
        const auto parents=全局基础数据集.查询目标字段(n,初始场景限定字段);
        if(parents.size()!=1||parents.front().所属节点!=节点)return std::nullopt;
        const auto snapshot=全局基础数据集.查询节点组完整结构({n});
        if(snapshot.状态!=基础结构操作状态::已完成||snapshot.节点组.size()!=1||!snapshot.节点组.front().快照)return std::nullopt;
        for(const auto& f:snapshot.节点组.front().快照->结构.字段组)
            if(f.字段节点!=节点类型字段&&f.字段节点!=初始声明所属字段&&f.字段节点!=初始关系概念字段
                &&f.字段节点!=顺序字段&&f.字段节点!=初始四位置字段)return std::nullopt;
        const auto 关系概念=读取唯一节点字段(n,初始关系概念字段);
        const auto fields=全局基础数据集.查询字段(n,初始四位置字段);
        const auto* value=fields.size()==1?std::get_if<基础值>(&fields.front().内容):nullptr;
        const auto* positions=value?std::get_if<std::vector<std::int64_t>>(&value->材料):nullptr;
        if(!关系概念||!positions||positions->size()!=4)return std::nullopt;
        for(const auto p:*positions)if(p<=0||p>(std::numeric_limits<std::uint32_t>::max)())return std::nullopt;
        新方法初始场景事实限定 r{*关系概念,static_cast<std::uint32_t>((*positions)[0]),static_cast<std::uint32_t>((*positions)[1]),static_cast<std::uint32_t>((*positions)[2]),static_cast<std::uint32_t>((*positions)[3])};
        if(std::find(结果.场景事实限定组.begin(),结果.场景事实限定组.end(),r)!=结果.场景事实限定组.end())return std::nullopt;
        结果.场景事实限定组.push_back(r);
    }
    for (const auto 项节点 : *项节点组) {
        if (!节点类型为(项节点, 条件项节点类型)) return std::nullopt;
        if(!全局基础数据集.查询字段(项节点,结果被动需求目标字段).empty()
            ||!全局基础数据集.查询字段(项节点,结果引用内容要求字段).empty()
            ||!全局基础数据集.查询字段(项节点,结果引用内容项字段).empty()
            ||!全局基础数据集.查询字段(项节点,结果内容作用点字段).empty())return std::nullopt;
        const auto 角色 = 读取可选节点字段(项节点, 参数角色字段);
        const auto 序号 = 读取可选I64字段(项节点, 有序参数序号字段);
        const auto 存在 = 读取可选节点字段(项节点, 条件项存在概念字段);
        const auto 类型 = 读取可选节点字段(项节点, 条件项特征类型字段);
        const auto 值域 = 读取可选节点字段(项节点, 条件项特征值域字段);
        const auto 二次 = 读取可选节点字段(项节点, 条件项二次特征字段);
        const auto 状态动态 = 读取可选节点字段(项节点, 条件项状态动态字段);
        const auto 内容要求=读取可选节点字段(项节点,条件引用内容要求字段);
        const auto 内容项=读取可选节点字段(项节点,条件引用内容项字段);
        const auto 作用点=读取可选I64字段(项节点,条件内容作用点字段);
        const auto 认知有效=读取可选I64字段(项节点,条件内容认知有效字段);
        const auto 前态=读取可选I64字段(项节点,条件前态当前字段);
        const auto 历史数=读取可选I64字段(项节点,条件前态历史数字段);
        if(!内容要求||!内容项||!作用点||!认知有效||!前态||!历史数
            ||(!有效(*内容要求)&&(有效(*内容项)||作用点->has_value()||认知有效->has_value()))
            ||(*作用点&&**作用点!=1&&**作用点!=2)
            ||(*认知有效&&**认知有效!=0&&**认知有效!=1)
            ||(*前态&&**前态!=1&&**前态!=2)||(*历史数&&(!*前态||**历史数<0)))return std::nullopt;
        const auto 成员变量 = 读取可选I64字段(项节点, 任务成员变量字段);
        if (!成员变量 || (*成员变量 && **成员变量 != 1)) return std::nullopt;
        const auto 精确域 = 读取可选精确数值域(项节点);
        if (!角色 || !序号 || !存在 || !类型 || !值域 || !二次 || !状态动态 || !精确域
            || (*序号 && (**序号 <= 0 || **序号 > (std::numeric_limits<std::uint32_t>::max)()))
            || 有效(*角色) == 序号->has_value()) {
            return std::nullopt;
        }
        新方法条件项定义 项;
        if (*成员变量) 项.任务成员变量序号 = 1;
        项.参数角色节点 = *角色;
        if (*序号) 项.参数序号 = static_cast<std::uint32_t>(**序号);
        if (有效(*存在)) 项.存在概念节点 = *存在;
        if (有效(*类型)) 项.特征类型概念节点 = *类型;
        if (有效(*值域)) 项.特征值域概念节点 = *值域;
        if (有效(*二次)) 项.二次特征概念节点 = *二次;
        if (有效(*状态动态)) 项.状态或动态概念节点 = *状态动态;
        项.精确数值域 = *精确域;
        if(有效(*内容要求)) {
            新方法引用内容条件 内容;内容.要求概念节点=*内容要求;
            if(*作用点)内容.作用点=static_cast<新方法内容作用点>(**作用点);
            if(*认知有效)内容.要求认知来源当前有效=**认知有效==1;
            if(有效(*内容项))内容.正式内容路径.push_back(*内容项);
            if(内容.要求认知来源当前有效&&(内容.作用点!=新方法内容作用点::参数自身||!内容.正式内容路径.empty()))return std::nullopt;
            项.引用内容条件=std::move(内容);
        }
        if(*前态) {
            新方法特征前态限定 p;p.当前值状态=static_cast<新方法当前值状态>(**前态);
            if(*历史数)p.历史不同值数量=static_cast<std::uint64_t>(**历史数);
            项.特征前态限定=std::move(p);
        }
        if (!精确数值域有效(项.精确数值域, 项.特征类型概念节点,
                项.特征值域概念节点, 概念服务)) return std::nullopt;
        if(项.引用内容条件&&(项.存在概念节点||项.特征类型概念节点||项.特征值域概念节点
            ||项.精确数值域||项.二次特征概念节点||项.状态或动态概念节点
            ||项.任务成员变量序号||项.特征前态限定||!项.参数序号))return std::nullopt;
        if(项.特征前态限定&&(项.存在概念节点||项.特征值域概念节点||项.精确数值域
            ||项.二次特征概念节点||项.状态或动态概念节点||项.任务成员变量序号||!项.参数序号
            ||项.特征前态限定->当前值状态!=新方法当前值状态::未知
            ||项.特征前态限定->历史不同值数量!=std::optional<std::uint64_t>{0}))return std::nullopt;
        结果.条件项组.push_back(std::move(项));
    }
    return 结果;
}

bool 特征概念为或从属于(稳定编码,稳定编码,const 概念_特征类&);
bool 分支条件有效(const 新方法条件定义& c,const std::vector<新方法参数规格>& specs,
    const 概念_特征类& f,const 概念_存在类& e,
    引用内容条件校验模式 模式=引用内容条件校验模式::完整语义) {
    if(c.关系规则!=新方法项关系规则::全部成立||!条件结果有效({c,{},{}},f,false,true)
        ||!条件结果角色已声明(specs,{c,{},{}},f,e,模式))return false;
    for(const auto& x:c.条件项组) {
        if(x.存在概念节点&&!e.获取存在概念(*x.存在概念节点))return false;
    }
    return true;
}
void 合入必要条件(新方法初始必要条件定义& target,const 新方法条件定义& source) {
    target.条件.关系规则=新方法项关系规则::全部成立;
    const auto merge=[](auto& to,const auto& from) {for(const auto& x:from)if(std::find(to.begin(),to.end(),x)==to.end())to.push_back(x);};
    merge(target.条件.条件项组,source.条件项组);merge(target.条件.参数关系组,source.参数关系组);
    merge(target.场景事实限定组,target.条件.场景事实限定组);target.条件.场景事实限定组.clear();
    merge(target.场景事实限定组,source.场景事实限定组);
}
std::vector<新方法执行分支定义> 分支定义组(const 新方法信息& m) {
    std::vector<新方法执行分支定义> result;for(const auto& b:m.执行分支组)result.push_back(b.定义);return result;
}

bool 模拟输入方案有效(const std::vector<新方法模拟输入方案定义>& plans,
    const std::vector<新方法参数规格>& specs,const std::vector<新方法参数关系>& relations,
    const 概念_特征类& concepts) {
    const auto kind=[&](std::uint32_t p,新方法参数信息类型 k) {
        return p&&p<=specs.size()&&specs[p-1].有序规格&&specs[p-1].有序规格->信息类型==k;
    };
    const auto message=[&](std::uint32_t f,std::uint32_t w) {
        if(!kind(f,新方法参数信息类型::特征)||!kind(w,新方法参数信息类型::存在))return false;
        const auto d=concepts.读取特征概念定义保护(specs[f-1].有序规格->范围概念节点);
        return d.信息&&d.信息->定义.材料类型==特征概念材料物理类型::U64数组
            &&d.信息->定义.比较规则==特征概念比较规则::消息集合身份判等
            &&std::holds_alternative<特征概念全材料值域>(d.信息->定义.值域)
            &&specs[f-1].有序规格->所属存在位置==w; // R97：所属由规格声明
    };
    (void)relations;
    const auto direction=[](auto d){return d==新外设消息方向::接收||d==新外设消息方向::发送;};
    for(std::size_t i=0;i<plans.size();++i) {
        if(plans[i].顺序!=i||plans[i].消息组.empty())return false;
        for(std::size_t j=0;j<plans[i].消息组.size();++j) {
            const auto& x=plans[i].消息组[j];
            if(x.顺序!=j||!direction(x.方向)||!kind(x.对端存在参数序号,新方法参数信息类型::存在)
                ||!message(x.目标特征参数序号,x.窗口存在参数序号))return false;
            const auto d=concepts.读取特征概念定义保护(x.正文范围概念);
            if(!d.信息||d.信息->定义.材料类型!=特征概念材料物理类型::UTF8字符串
                ||d.信息->定义.比较规则!=特征概念比较规则::完整材料精确
                ||(!std::holds_alternative<特征概念全材料值域>(d.信息->定义.值域)
                    &&!std::holds_alternative<std::vector<稳定编码>>(d.信息->定义.值域)))return false;
            if(x.时机==新方法模拟输入时机::调用前) {if(x.触发)return false;}
            else if(x.时机!=新方法模拟输入时机::参数新增消息后||!x.触发||!direction(x.触发->方向)
                ||!message(x.触发->特征参数序号,x.窗口存在参数序号))return false;
        }
    }
    return true;
}

std::optional<std::vector<新方法模拟输入方案定义>> 读取模拟输入方案(
    稳定编码 vm,const 概念_特征类& concepts) {
    const auto plans=读取有序子节点(vm,模拟输入方案字段,concepts);
    if(!plans)return {};
    std::vector<新方法模拟输入方案定义> out;
    const auto array=[](稳定编码 n,稳定编码 k)->std::optional<std::vector<std::int64_t>> {
        const auto f=全局基础数据集.查询字段(n,k);
        if(f.size()!=1)return {};
        const auto* v=std::get_if<基础值>(&f.front().内容);
        const auto* a=v?std::get_if<std::vector<std::int64_t>>(&v->材料):nullptr;
        return a?std::optional{*a}:std::nullopt;
    };
    const auto pos=[](std::int64_t x){return x>0&&static_cast<std::uint64_t>(x)<=(std::numeric_limits<std::uint32_t>::max)();};
    for(const auto P:*plans) {
        if(!节点类型为(P,模拟输入方案节点类型)||读取唯一节点字段(P,初始声明所属字段)!=vm
            ||读取唯一I64字段(P,顺序字段)!=static_cast<std::int64_t>(out.size()))return {};
        const auto items=读取有序子节点(P,模拟消息输入字段,concepts);
        const auto structure=全局基础数据集.查询完整节点结构(P);
        if(!items||items->empty()||!structure.结构||structure.结构->字段组.size()!=3+items->size())return {};
        新方法模拟输入方案定义 plan;plan.顺序=static_cast<std::uint32_t>(out.size());
        for(const auto I:*items) {
            const auto p=array(I,模拟消息参数位置字段);
            const auto d=读取唯一I64字段(I,模拟消息方向字段),nonempty=读取唯一I64字段(I,模拟正文非空字段),when=读取唯一I64字段(I,模拟输入时机字段);
            const auto body=读取唯一节点字段(I,模拟正文范围字段);
            const auto shape=全局基础数据集.查询完整节点结构(I);
            if(!节点类型为(I,模拟消息输入节点类型)||读取唯一节点字段(I,初始声明所属字段)!=P
                ||读取唯一I64字段(I,顺序字段)!=static_cast<std::int64_t>(plan.消息组.size())
                ||!p||p->size()!=3||!std::all_of(p->begin(),p->end(),pos)||!d||(*d!=1&&*d!=2)
                ||!nonempty||(*nonempty!=0&&*nonempty!=1)||!when||(*when!=1&&*when!=2)||!body||!shape.结构
                ||shape.结构->字段组.size()!=static_cast<std::size_t>(*when==1?8:9))return {};
            新方法模拟消息输入定义 x;
            x.顺序=static_cast<std::uint32_t>(plan.消息组.size());
            x.目标特征参数序号=static_cast<std::uint32_t>((*p)[0]);x.窗口存在参数序号=static_cast<std::uint32_t>((*p)[1]);x.对端存在参数序号=static_cast<std::uint32_t>((*p)[2]);
            x.方向=static_cast<新外设消息方向>(*d);x.正文范围概念=*body;x.正文非空=*nonempty==1;x.时机=static_cast<新方法模拟输入时机>(*when);
            if(*when==2) {
                const auto t=array(I,模拟触发位置方向字段);
                if(!t||t->size()!=2||!pos((*t)[0])||((*t)[1]!=1&&(*t)[1]!=2))return {};
                x.触发=新方法模拟消息触发{static_cast<std::uint32_t>((*t)[0]),static_cast<新外设消息方向>((*t)[1])};
            } else if(!全局基础数据集.查询字段(I,模拟触发位置方向字段).empty())return {};
            plan.消息组.push_back(x);
        }
        out.push_back(std::move(plan));
    }
    return out;
}

void 追加模拟输入方案(基础节点组追加请求& q,稳定编码 vm,
    const std::vector<新方法模拟输入方案定义>& plans) {
    const auto field=[&](std::size_t n,稳定编码 k,基础组内字段内容 v){q.新节点组[n].字段组.push_back({k,std::move(v)});};
    const auto node=[&](稳定编码 type){const auto n=q.新节点组.size();q.新节点组.emplace_back();field(n,节点类型字段,type);return n;};
    for(const auto& p:plans) {
        const auto P=node(模拟输入方案节点类型);field(P,顺序字段,基础值{static_cast<std::int64_t>(p.顺序)});field(P,初始声明所属字段,vm);
        q.追加字段组.push_back({vm,模拟输入方案字段,基础组内节点引用{P}});
        for(const auto& x:p.消息组) {
            const auto I=node(模拟消息输入节点类型);field(I,顺序字段,基础值{static_cast<std::int64_t>(x.顺序)});field(I,初始声明所属字段,基础组内节点引用{P});
            field(P,模拟消息输入字段,基础组内节点引用{I});
            field(I,模拟消息参数位置字段,基础值{std::vector<std::int64_t>{x.目标特征参数序号,x.窗口存在参数序号,x.对端存在参数序号}});
            field(I,模拟消息方向字段,基础值{static_cast<std::int64_t>(x.方向)});field(I,模拟正文范围字段,x.正文范围概念);
            field(I,模拟正文非空字段,基础值{static_cast<std::int64_t>(x.正文非空)});field(I,模拟输入时机字段,基础值{static_cast<std::int64_t>(x.时机)});
            if(x.触发)field(I,模拟触发位置方向字段,基础值{std::vector<std::int64_t>{x.触发->特征参数序号,static_cast<std::int64_t>(x.触发->方向)}});
        }
    }
    std::set<稳定编码> endpoints;
    for(const auto& n:q.新节点组)for(const auto& f:n.字段组) {
        endpoints.insert(f.字段节点);if(const auto* v=std::get_if<稳定编码>(&f.内容))endpoints.insert(*v);
    }
    if(endpoints.empty())return; // 空声明没有新增端点，不向L1提交无效的空查询。
    const auto read=全局基础数据集.查询节点组完整结构({endpoints.begin(),endpoints.end()});
    if(read.状态!=基础结构操作状态::已完成||read.节点组.size()!=endpoints.size())throw std::runtime_error("simulation input declaration endpoints unavailable");
    for(const auto& n:read.节点组){if(!n.快照)throw std::runtime_error("simulation input declaration endpoint missing");q.预期联合节点组.push_back(*n.快照);}
}
bool 条件涵盖必要项(const 新方法条件定义& actual,const 新方法初始必要条件定义& required,
    const std::vector<新方法参数规格>& specs,
    const 概念_特征类& features,const 概念_存在类& concepts) {
    if(actual.关系规则!=新方法项关系规则::全部成立)return false;
    const auto isChild=[&](稳定编码 child,稳定编码 parent) {
        const auto d = concepts.读取当前基础范围概念(child);
        if (d.状态 == 新基础范围读取状态::资源失败) throw std::bad_alloc{};
        if (d.定义 && d.定义->原语 == 基础概念原语::词条同键)
            return 方法词条范围属于(child, parent, concepts);
        std::set<稳定编码> seen;std::vector<稳定编码> pending{child};
        while(!pending.empty()) {const auto n=pending.back();pending.pop_back();if(n==parent)return true;
            if(!seen.insert(n).second)continue;const auto c=concepts.获取存在概念(n);
            if(c)pending.insert(pending.end(),c->上位概念组.begin(),c->上位概念组.end());
            else {
                const auto f=concepts.读取正式结构概念保护(n);
                if(f.状态==正式结构操作状态::前提已变化)throw 引用内容定义前提变化{};
                if(f.状态!=正式结构操作状态::已读取||!f.信息)return false;
                const auto* s=std::get_if<信息结构概念定义>(&f.信息->定义);if(!s)return false;
                pending.insert(pending.end(),s->上位结构概念组.begin(),s->上位结构概念组.end());
            }}return false;
    };
    for(const auto& w:required.条件.条件项组) {
        if(w.存在概念节点&&std::none_of(actual.条件项组.begin(),actual.条件项组.end(),[&](const auto& a) {
            return a.参数序号==w.参数序号&&a.参数角色节点==w.参数角色节点
                &&a.任务成员变量序号==w.任务成员变量序号&&a.存在概念节点
                &&isChild(*a.存在概念节点,*w.存在概念节点);
        }))return false;
        if(std::none_of(actual.条件项组.begin(),actual.条件项组.end(),[&](const auto& a) {
            bool 同位置=a.参数序号==w.参数序号&&a.参数角色节点==w.参数角色节点
                &&a.任务成员变量序号==w.任务成员变量序号;
            if(!同位置&&w.参数序号&&a.参数序号&&w.特征类型概念节点
                &&!w.任务成员变量序号&&!a.任务成员变量序号&&!w.参数角色节点.值&&!a.参数角色节点.值) {
                // R97：特征位置a属于存在位置w时，以a上的特征条件涵盖w上的同类型条件。
                同位置=a.参数序号<=specs.size()&&specs[a.参数序号-1].有序规格
                    &&specs[a.参数序号-1].有序规格->所属存在位置==w.参数序号;
            }
            if(!同位置)return false;
            if(w.引用内容条件||a.引用内容条件)
                return w.引用内容条件&&a.引用内容条件&&w.引用内容条件==a.引用内容条件;
            if(w.特征前态限定&&w.特征前态限定!=a.特征前态限定)return false;
            if(w.特征类型概念节点&&a.特征类型概念节点!=w.特征类型概念节点)return false;
            if(w.二次特征概念节点&&a.二次特征概念节点!=w.二次特征概念节点)return false;
            if(w.状态或动态概念节点&&a.状态或动态概念节点!=w.状态或动态概念节点)return false;
            if(w.精确数值域||w.特征值域概念节点) {
                const auto domain=[&](const auto& x)->std::optional<特征概念值域> {
                    if(x.精确数值域)return 特征概念值域{std::vector<特征概念I64闭区间>{*x.精确数值域}};
                    if(x.特征值域概念节点) {const auto d=features.获取特征概念(*x.特征值域概念节点);if(d&&d->材料类型==特征概念材料物理类型::I64标量)return d->值域;}return {};
                };
                const auto lhs=domain(a),rhs=domain(w);
                if(lhs&&rhs) {const auto r=features.比较值域(特征概念材料物理类型::I64标量,*lhs,*rhs);
                    return r.状态==特征概念规则状态::已完成&&(r.关系==特征概念值域关系::相等||r.关系==特征概念值域关系::右包含左);}
                return !w.精确数值域&&a.特征值域概念节点&&w.特征值域概念节点
                    &&特征概念为或从属于(*a.特征值域概念节点,*w.特征值域概念节点,features);
            }
            return true;
        }))return false;
    }
    for(const auto& r:required.条件.参数关系组)if(std::find(actual.参数关系组.begin(),actual.参数关系组.end(),r)==actual.参数关系组.end())return false;
    for(const auto& r:required.场景事实限定组)if(std::find(actual.场景事实限定组.begin(),actual.场景事实限定组.end(),r)==actual.场景事实限定组.end())return false;
    return true;
}
bool 配对分支来源有效(const 新方法信息& m,const 新方法条件结果定义& p,
    const 概念_特征类& f,const 概念_存在类& e) {
    if(m.执行分支组.empty())return p.来源执行分支节点组.empty();
    if(p.来源执行分支节点组.empty())return false;
    std::set<稳定编码> seen;
    for(const auto B:p.来源执行分支节点组) {
        if(!seen.insert(B).second)return false;
        const auto branch=std::find_if(m.执行分支组.begin(),m.执行分支组.end(),[&](const auto& b){return b.节点==B;});
        if(branch==m.执行分支组.end())return false;
        新方法初始必要条件定义 needed;if(m.初始必要条件)needed=*m.初始必要条件;
        合入必要条件(needed,branch->定义.必要条件);
        for(const auto& r:m.初始模拟关系组)if(std::find(needed.条件.参数关系组.begin(),needed.条件.参数关系组.end(),r)==needed.条件.参数关系组.end())needed.条件.参数关系组.push_back(r);
        if(!条件涵盖必要项(p.条件,needed,m.参数规格组,f,e))return false;
    }return true;
}
std::optional<新方法结果项定义> 读取结果项(稳定编码 项节点, const 概念_特征类& 概念服务) {
        if (!节点类型为(项节点, 结果项节点类型)) return std::nullopt;
        if(!全局基础数据集.查询字段(项节点,条件引用内容要求字段).empty()
            ||!全局基础数据集.查询字段(项节点,条件引用内容项字段).empty()
            ||!全局基础数据集.查询字段(项节点,条件内容作用点字段).empty()
            ||!全局基础数据集.查询字段(项节点,条件内容认知有效字段).empty()
            ||!全局基础数据集.查询字段(项节点,条件前态当前字段).empty()
            ||!全局基础数据集.查询字段(项节点,条件前态历史数字段).empty())return std::nullopt;
        const auto 内容要求=读取可选节点字段(项节点,结果引用内容要求字段);
        const auto 内容路径=读取可选节点字段(项节点,结果引用内容项字段);
        const auto 内容作用=读取可选I64字段(项节点,结果内容作用点字段);
        if(!内容要求||!内容路径||!内容作用
            ||(!有效(*内容要求)&&(有效(*内容路径)||内容作用->has_value()))
            ||(有效(*内容要求)&&(!*内容作用||**内容作用!=1)))return std::nullopt;
        const auto 角色 = 读取可选节点字段(项节点, 参数角色字段);
        const auto 序号 = 读取可选I64字段(项节点, 有序参数序号字段);
        const auto 类型 = 读取可选节点字段(项节点, 结果项特征类型字段);
        const auto 值域 = 读取可选节点字段(项节点, 结果项特征值域字段);
        const auto 方向 = 读取可选节点字段(项节点, 结果项变化方向字段);
        const auto 状态动态 = 读取可选节点字段(项节点, 结果项状态动态字段);
        const auto 主要 = 读取唯一I64字段(项节点, 结果项主要结果字段);
        const auto 成员变量 = 读取可选I64字段(项节点, 任务成员变量字段);
        if (!成员变量 || (*成员变量 && **成员变量 != 1)) return std::nullopt;
        const auto 被动 = 读取可选I64字段(项节点, 结果被动需求目标字段);
        if (!被动 || (*被动 && **被动 != 1)) return std::nullopt;
        const auto 精确域 = 读取可选精确数值域(项节点);
        const auto 作用存在 = 读取可选节点字段(项节点, 结果作用存在字段);
        if (!角色 || !序号 || !类型 || !值域 || !方向 || !状态动态 || !主要 || !精确域 || !作用存在
            || (*序号 && (**序号 <= 0 || **序号 > (std::numeric_limits<std::uint32_t>::max)()))
            || (有效(*作用存在) ? (有效(*角色) || 序号->has_value()) : 有效(*角色) == 序号->has_value())
            || (*主要 != 0 && *主要 != 1)) return std::nullopt;
        新方法结果项定义 项;
        if (有效(*作用存在)) 项.结果作用存在 = *作用存在;
        if (*成员变量) 项.任务成员变量序号 = 1;
        项.参数角色节点 = *角色;
        if (*序号) 项.参数序号 = static_cast<std::uint32_t>(**序号);
        if (有效(*类型)) 项.特征类型概念节点 = *类型;
        if (有效(*值域)) 项.特征值域概念节点 = *值域;
        if (有效(*方向)) 项.变化方向概念节点 = *方向;
        if (有效(*状态动态)) 项.状态或动态概念节点 = *状态动态;
        项.主要结果 = *主要 != 0;
        项.精确数值域 = *精确域;
        项.被动需求目标 = *被动 && **被动 == 1;
        if(有效(*内容要求)) {
            新方法引用内容条件 c;c.要求概念节点=*内容要求;
            if(有效(*内容路径))c.正式内容路径.push_back(*内容路径);
            c.作用点=static_cast<新方法内容作用点>(**内容作用);
            项.引用内容结果=std::move(c);
        }
        if (!精确数值域有效(项.精确数值域, 项.特征类型概念节点,
                项.特征值域概念节点, 概念服务)) return std::nullopt;
        if(!结果项有效(项,概念服务))return std::nullopt;
        return 项;
}

std::optional<新方法结果定义> 读取结果(稳定编码 节点, const 概念_特征类& 概念服务) {
    if (!节点类型为(节点, 结果节点类型)) return std::nullopt;
    const auto 规则 = 读取唯一I64字段(节点, 关系规则字段);
    const auto 项节点组 = 读取有序子节点(节点, 结果项字段, 概念服务);
    const auto 证据质量 = 读取唯一I64字段(节点, 结果必要证据质量字段);
    const auto 最大等待 = 读取可选I64字段(节点, 结果最大等待纳秒字段);
    const auto 等待条件 = 读取可选节点字段(节点, 结果等待条件概念字段);
    const auto 多证据关系 = 读取唯一I64字段(节点, 结果多证据关系字段);
    const auto 禁止规则 = 读取唯一I64字段(节点, 结果禁止关系规则字段);
    const auto 禁止项节点组 = 读取有序子节点(节点, 结果禁止项字段, 概念服务);
    if (!规则 || !是关系规则(*规则) || !项节点组 || !证据质量
        || !是必要证据质量(*证据质量) || !最大等待
        || (*最大等待 && **最大等待 < 0) || !等待条件 || !多证据关系
        || !是多证据关系(*多证据关系) || !禁止规则
        || !是关系规则(*禁止规则) || !禁止项节点组) {
        return std::nullopt;
    }
    新方法结果定义 结果;
    结果.关系规则 = static_cast<新方法项关系规则>(*规则);
    const auto 位置关系=读取参数关系(节点);
    if(!位置关系) return std::nullopt;
    结果.参数关系组=*位置关系;
    const auto sceneNodes=读取有序子节点(节点,结果场景限定字段,概念服务);
    if(!sceneNodes)return std::nullopt;
    for(std::size_t i=0;i<sceneNodes->size();++i) {
        const auto n=(*sceneNodes)[i];
        const auto parents=全局基础数据集.查询目标字段(n,结果场景限定字段);
        if(!节点类型为(n,结果场景限定节点类型)||parents.size()!=1||parents.front().所属节点!=节点
            ||读取唯一节点字段(n,结果场景所属字段)!=std::optional{节点}
            ||读取唯一I64字段(n,顺序字段)!=std::optional<std::int64_t>{static_cast<std::int64_t>(i)})return std::nullopt;
        const auto snapshot=全局基础数据集.查询完整节点结构(n);
        if(!snapshot.结构)return std::nullopt;
        for(const auto& f:snapshot.结构->字段组)
            if(f.字段节点!=节点类型字段&&f.字段节点!=顺序字段&&f.字段节点!=结果场景所属字段
                &&f.字段节点!=结果场景关系概念字段&&f.字段节点!=结果场景四位置字段)return std::nullopt;
        const auto relation=读取唯一节点字段(n,结果场景关系概念字段);
        const auto fields=全局基础数据集.查询字段(n,结果场景四位置字段);
        const auto* value=fields.size()==1?std::get_if<基础值>(&fields.front().内容):nullptr;
        const auto* positions=value?std::get_if<std::vector<std::int64_t>>(&value->材料):nullptr;
        if(!relation||!positions||positions->size()!=4)return std::nullopt;
        for(const auto p:*positions)if(p<=0||p>(std::numeric_limits<std::uint32_t>::max)())return std::nullopt;
        新方法初始场景事实限定 r{*relation,static_cast<std::uint32_t>((*positions)[0]),static_cast<std::uint32_t>((*positions)[1]),static_cast<std::uint32_t>((*positions)[2]),static_cast<std::uint32_t>((*positions)[3])};
        if(std::find(结果.场景事实限定组.begin(),结果.场景事实限定组.end(),r)!=结果.场景事实限定组.end())return std::nullopt;
        结果.场景事实限定组.push_back(r);
    }
    for (const auto 项节点 : *项节点组) {
        const auto 项 = 读取结果项(项节点, 概念服务);
        if (!项) return std::nullopt;
        结果.结果项组.push_back(*项);
    }
    结果.确认要求.必要证据质量 =
        static_cast<新方法必要证据质量>(*证据质量);
    结果.确认要求.最大等待纳秒 = *最大等待;
    if (有效(*等待条件)) {
        结果.确认要求.等待条件概念节点 = *等待条件;
    }
    结果.确认要求.多证据关系 =
        static_cast<新方法多证据关系>(*多证据关系);
    结果.确认要求.禁止结果关系规则 =
        static_cast<新方法项关系规则>(*禁止规则);
    for (const auto 项节点 : *禁止项节点组) {
        const auto 项 = 读取结果项(项节点, 概念服务);
        if (!项 || 项->主要结果) return std::nullopt;
        结果.确认要求.禁止结果项组.push_back(*项);
    }
    return 结果;
}

bool 方法属于根(稳定编码 方法节点) {
    const auto 反向 = 全局基础数据集.查询目标字段(
        方法节点, 方法成员字段);
    return 反向.size() == 1 && 反向.front().所属节点 == 新方法组织根节点;
}

// 仅核验根、候选入口和调用闭包的身份，不递归调用读取方法信息。
// 后者会使用本函数，避免混合根与成员读回之间形成递归环。
// 调用入口已核对架构分配及组织根身份、独立性，不在此重复全架构扫描。
std::optional<新方法混合实例信息> 读取实例根结构(稳定编码 实例根, const 概念_特征类& 概念服务) {
    新方法混合实例信息 信息{实例根, {}};
    if (节点类型为(实例根, 混合方法实例根类型)) {
        const auto 归属 = 全局基础数据集.查询目标字段(
            实例根, 混合方法实例成员字段);
        const auto 候选 = 读取多节点字段(实例根, 混合方法候选入口字段, 概念服务);
        if (归属.size() != 1 || 归属.front().所属节点 != 新方法组织根节点
            || !候选 || 候选->empty()) return std::nullopt;
        信息.候选方法实例根组 = *候选;
    } else if (节点类型为(实例根, 方法节点类型)) {
        const auto 自指 = 读取唯一节点字段(实例根, 方法实例根字段);
        if (!自指 || *自指 != 实例根) return std::nullopt;
        信息.候选方法实例根组.push_back(实例根);
    } else {
        return std::nullopt;
    }
    std::vector<稳定编码> 待查 = 信息.候选方法实例根组;
    std::set<稳定编码> 方法闭包(待查.begin(), 待查.end());
    for (std::size_t i = 0; i < 待查.size(); ++i) {
        const auto 方法 = 待查[i];
        const auto 根 = 读取唯一节点字段(方法, 方法实例根字段);
        const auto 来源 = 读取唯一节点字段(方法, 方法来源字段);
        const auto 步骤 = 读取多节点字段(方法, 方法组合步骤字段, 概念服务);
        if (!节点类型为(方法, 方法节点类型) || !方法属于根(方法)
            || !根 || *根 != 实例根 || !来源 || !步骤
            || !节点类型为(*来源, 方法节点类型) || !方法属于根(*来源)
            || !全局基础数据集.查询字段(*来源, 方法来源字段).empty()
            || !全局基础数据集.查询字段(*来源, 方法实例根字段).empty()) {
            return std::nullopt;
        }
        for (const auto 节点 : *步骤) {
            const auto 所属 = 读取唯一节点字段(节点, 步骤所属组合方法字段);
            const auto 调用 = 读取唯一节点字段(节点, 步骤调用方法字段);
            if (!节点类型为(节点, 组合步骤节点类型) || !所属
                || *所属 != 方法 || !调用) return std::nullopt;
            if (方法闭包.insert(*调用).second) 待查.push_back(*调用);
        }
    }
    const auto 反向 = 全局基础数据集.查询目标字段(实例根, 方法实例根字段);
    if (反向.size() != 方法闭包.size()) return std::nullopt;
    std::set<稳定编码> 反向方法;
    for (const auto& 项 : 反向) {
        if (!方法闭包.contains(项.所属节点)
            || !反向方法.insert(项.所属节点).second) return std::nullopt;
    }
    return 信息;
}

enum class 复制字段种类 { 外部引用, 内部引用, 独占子节点, 调用方法, 直接值 };

struct 复制字段规格 final {
    稳定编码 字段;
    复制字段种类 种类;
    稳定编码 目标类型{};
};

// 白名单属于方法owner。只有独占子节点和调用方法向外展开，概念和
// 参数角色即使碰巧命中映射也不改写；新增schema须同步扩充此表。
std::vector<复制字段规格> 方法复制字段规格(稳定编码 类型) {
    using 类别 = 复制字段种类;
    std::vector<复制字段规格> 组{{节点类型字段, 类别::外部引用}};
    const auto 外部 = [&](std::initializer_list<稳定编码> 字段组) {
        for (const auto 字段 : 字段组) 组.push_back({字段, 类别::外部引用});
    };
    const auto 值 = [&](std::initializer_list<稳定编码> 字段组) {
        for (const auto 字段 : 字段组) 组.push_back({字段, 类别::直接值});
    };
    const auto 内部 = [&](std::initializer_list<稳定编码> 字段组) {
        for (const auto 字段 : 字段组) 组.push_back({字段, 类别::内部引用});
    };
    const auto 子 = [&](稳定编码 字段, 稳定编码 子类型) {
        组.push_back({字段, 类别::独占子节点, 子类型});
    };
    if (类型 == 方法节点类型) {
        值({方法作用域字段, 方法实现方式字段, 方法行为字段,
            方法本能函数登记名字段});
        子(方法头字段, 方法头节点类型);
        子(方法虚拟存在字段, 方法虚拟存在节点类型);
        子(方法条件结果字段, 条件结果配对节点类型);
        子(方法组合步骤字段, 组合步骤节点类型);
        子(方法结果转移字段, 结果转移节点类型);
        内部({方法组合入口步骤字段});
        外部({方法父代字段});
    } else if (类型 == 方法头节点类型) {
        外部({方法头作用存在概念字段, 方法头特征类型概念字段, 方法头结果概念字段});
    } else if (类型 == 方法虚拟存在节点类型) {
        内部({虚拟存在所属方法字段});
        值({参数位置关系字段, 模拟事件位置字段});
        子(内容关系成员字段,内容关系节点类型);
        子(虚拟存在参数规格字段, 参数规格节点类型);
        子(初始必要条件字段, 初始声明节点类型);
        子(登记结果组字段, 初始声明节点类型);
        子(执行分支字段,分支节点类型);
        子(模拟输入方案字段,模拟输入方案节点类型);
    } else if (类型 == 模拟输入方案节点类型) {
        内部({初始声明所属字段});值({顺序字段});子(模拟消息输入字段,模拟消息输入节点类型);
    } else if (类型 == 模拟消息输入节点类型) {
        内部({初始声明所属字段});外部({模拟正文范围字段});
        值({顺序字段,模拟消息参数位置字段,模拟消息方向字段,模拟正文非空字段,模拟输入时机字段,模拟触发位置方向字段});
    } else if (类型 == 分支节点类型) {
        内部({分支所属方法字段,分支所属虚拟字段});值({顺序字段});
        子(分支条件字段,条件节点类型);
    } else if (类型 == 初始声明节点类型) {
        内部({初始声明所属字段});值({初始声明格式字段,关系规则字段,参数位置关系字段});
        子(条件项字段,初始声明项节点类型);子(初始场景限定字段,初始场景限定节点类型);
        子(内容关系成员字段,内容关系节点类型);
    } else if (类型 == 初始声明项节点类型) {
        内部({初始声明所属字段});外部({条件项存在概念字段,条件项特征类型字段});值({顺序字段,有序参数序号字段});
    } else if (类型 == 初始场景限定节点类型) {
        内部({初始声明所属字段});外部({初始关系概念字段});值({顺序字段,初始四位置字段});
    } else if (类型 == 参数规格节点类型) {
        外部({参数角色字段, 参数存在概念字段, 参数集合角色字段, 参数集合特征类型字段, 有序参数范围字段, 有序参数类别概念字段,
            有序参数必需特征类型字段});
        值({参数要求场景字段, 参数对象种类字段, 顺序字段, 有序参数序号字段, 有序参数类型字段, 有序参数所属位置字段});
        子(参数特征规格字段, 参数特征规格节点类型);
    } else if (类型 == 参数特征规格节点类型) {
        外部({参数特征类型字段, 参数特征值域字段, 参数二次特征概念字段});
        值({顺序字段});
    } else if (类型 == 条件结果配对节点类型) {
        外部({配对来源字段});
        内部({配对分支来源字段});
        子(配对条件字段, 条件节点类型);
        子(配对结果字段, 结果节点类型);
        值({顺序字段, 配对命中次数字段, 配对线性关系组字段, 配对知识来源字段});
    } else if (类型 == 条件节点类型) {
        子(条件项字段, 条件项节点类型);
        子(初始场景限定字段, 初始场景限定节点类型);
        值({关系规则字段, 参数位置关系字段});
        子(内容关系成员字段,内容关系节点类型);
    } else if (类型 == 结果节点类型) {
        子(结果项字段, 结果项节点类型);
        子(结果禁止项字段, 结果项节点类型);
        子(结果场景限定字段,结果场景限定节点类型);
        子(内容关系成员字段,内容关系节点类型);
        外部({结果等待条件概念字段});
        值({关系规则字段, 参数位置关系字段, 结果必要证据质量字段, 结果最大等待纳秒字段,
            结果多证据关系字段, 结果禁止关系规则字段});
    } else if(类型==结果场景限定节点类型) {
        内部({结果场景所属字段});外部({结果场景关系概念字段});值({顺序字段,结果场景四位置字段});
    } else if (类型 == 条件项节点类型) {
        外部({参数角色字段, 条件项存在概念字段, 条件项特征类型字段,
            条件项特征值域字段, 条件项二次特征字段, 条件项状态动态字段,
            条件引用内容要求字段,条件引用内容项字段});
        值({顺序字段, 项精确数值域字段, 有序参数序号字段, 任务成员变量字段,
            条件内容作用点字段,条件内容认知有效字段,条件前态当前字段,条件前态历史数字段});
    } else if (类型 == 结果项节点类型) {
        外部({参数角色字段, 结果项特征类型字段, 结果项特征值域字段,
            结果项变化方向字段, 结果项状态动态字段,结果引用内容要求字段,结果引用内容项字段,结果作用存在字段});
        值({顺序字段, 结果项主要结果字段, 项精确数值域字段, 有序参数序号字段, 任务成员变量字段,结果内容作用点字段,结果被动需求目标字段});
    } else if(类型==内容关系节点类型) {
        内部({内容关系所属字段});外部({内容关系路径字段});值({内容关系左字段,内容关系右字段});
    } else if (类型 == 组合步骤节点类型) {
        内部({步骤所属组合方法字段});
        组.push_back({步骤调用方法字段, 类别::调用方法, 方法节点类型});
        值({顺序字段});
    } else if (类型 == 结果转移节点类型) {
        内部({转移所属组合方法字段, 转移当前步骤字段,
            转移当前结果字段, 转移下一步骤字段});
        外部({转移匹配结果概念字段});
    } else {
        return {};
    }
    return 组;
}

bool 规范化通用概念必要声明(新方法初始必要条件定义& d) {
    if(d.条件.关系规则!=新方法项关系规则::全部成立||d.条件.条件项组.empty()
        ||!d.条件.参数关系组.empty()||!d.条件.场景事实限定组.empty()||!d.场景事实限定组.empty())return false;
    for(const auto& c:d.条件.条件项组) {
        if(!c.参数序号||c.存在概念节点.has_value()==c.特征类型概念节点.has_value())return false;
        const auto 概念=c.存在概念节点 ? *c.存在概念节点 : *c.特征类型概念节点;
        if(!有效(概念))return false;
        新方法条件项定义 expected;expected.参数序号=c.参数序号;
        expected.存在概念节点=c.存在概念节点;expected.特征类型概念节点=c.特征类型概念节点;
        if(c!=expected)return false;
    }
    const auto key=[](const 新方法条件项定义& c) {
        return std::tuple{c.参数序号,c.存在概念节点 ? 0 : 1,
            c.存在概念节点 ? *c.存在概念节点 : *c.特征类型概念节点};
    };
    auto& items=d.条件.条件项组;
    std::sort(items.begin(),items.end(),[&](const auto& a,const auto& b){return key(a)<key(b);});
    items.erase(std::unique(items.begin(),items.end()),items.end());
    return true;
}

bool 通用概念必要声明形状有效(const 新方法初始必要条件定义& d) {
    auto normalized=d;
    return 规范化通用概念必要声明(normalized)&&normalized==d;
}

新方法操作状态 核验通用概念必要声明(const 新方法知识建立请求& 请求,
    const 概念_存在类& 存在概念,const 概念_特征类& 特征概念,
    std::vector<基础联合节点快照>& 完整保护) {
    if(!请求.初始必要条件)return 新方法操作状态::已找到;
    if(!通用概念必要声明形状有效(*请求.初始必要条件))return 新方法操作状态::参数规格不合法;
    std::set<std::pair<新方法参数信息类型,稳定编码>> 存在已读;
    std::set<稳定编码> 特征已读;
    const auto 保护存在概念=[&](稳定编码 C,新方法参数信息类型 类型) {
        if(!存在已读.emplace(类型,C).second)return 新方法操作状态::已找到;
        if(!(类型==新方法参数信息类型::词条
            ?追加方法词条范围保护(C,存在概念,完整保护):追加方法存在范围保护(C,存在概念,完整保护)))return 新方法操作状态::参数规格不合法;
        return 新方法操作状态::已找到;
    };
    for(const auto& c:请求.初始必要条件->条件.条件项组) {
        if(c.参数序号>请求.参数规格组.size())return 新方法操作状态::参数规格不合法;
        const auto& spec=请求.参数规格组[c.参数序号-1].有序规格;
        if(!spec||spec->序号!=c.参数序号|| !是特征持有参数(spec->信息类型))
            return 新方法操作状态::参数规格不合法;
        if(spec->信息类型==新方法参数信息类型::词条
            &&((c.存在概念节点&&*c.存在概念节点!=spec->范围概念节点)
                ||(c.特征类型概念节点&&!持有者特征类型允许(spec->信息类型,*c.特征类型概念节点,特征概念))))
            return 新方法操作状态::参数规格不合法;
        if(c.存在概念节点) {
            const auto 状态=保护存在概念(*c.存在概念节点,spec->信息类型);
            if(状态!=新方法操作状态::已找到)return 状态;
            continue;
        }
        if(特征已读.insert(*c.特征类型概念节点).second) {
            const auto p=特征概念.读取特征概念定义保护(*c.特征类型概念节点);
            if(p.前提已变化)throw 引用内容定义前提变化{};
            if(p.状态==基础结构操作状态::节点不存在)return 新方法操作状态::参数规格不合法;
            if(p.状态!=基础结构操作状态::已完成||!p.信息||p.预期节点组.empty())
                throw std::runtime_error("generic initial feature definition protection unavailable");
            if(p.信息->类型根!=*c.特征类型概念节点||p.信息->定义.节点!=*c.特征类型概念节点)
                return 新方法操作状态::参数规格不合法;
            完整保护.insert(完整保护.end(),p.预期节点组.begin(),p.预期节点组.end());
        }
    }
    for(const auto& p:请求.参数规格组)if(p.有序规格&&是特征持有参数(p.有序规格->信息类型)) {
        const auto 状态=保护存在概念(p.有序规格->范围概念节点,p.有序规格->信息类型);
        if(状态!=新方法操作状态::已找到)return 状态;
    }
    return 新方法操作状态::已找到;
}

void 追加通用概念必要声明(基础节点组追加请求& 请求,基础组内字段内容 vm,
    const 新方法初始必要条件定义& d) {
    if(!通用概念必要声明形状有效(d))throw std::invalid_argument("generic initial concept shape invalid");
    const auto root=请求.新节点组.size();请求.新节点组.emplace_back();
    const auto field=[&](std::size_t n,稳定编码 key,基础组内字段内容 value){请求.新节点组[n].字段组.push_back({key,std::move(value)});};
    const bool 含存在=std::any_of(d.条件.条件项组.begin(),d.条件.条件项组.end(),
        [](const auto& c){return c.存在概念节点.has_value();});
    field(root,节点类型字段,初始声明节点类型);field(root,初始声明格式字段,基础值{std::int64_t{含存在 ? 3 : 2}});
    field(root,初始声明所属字段,vm);field(root,关系规则字段,基础值{static_cast<std::int64_t>(新方法项关系规则::全部成立)});
    for(std::size_t i=0;i<d.条件.条件项组.size();++i) {
        const auto& c=d.条件.条件项组[i];const auto n=请求.新节点组.size();请求.新节点组.emplace_back();
        field(n,节点类型字段,初始声明项节点类型);field(n,初始声明所属字段,基础组内节点引用{root});
        field(n,顺序字段,基础值{static_cast<std::int64_t>(i)});
        field(n,有序参数序号字段,基础值{static_cast<std::int64_t>(c.参数序号)});
        if(c.存在概念节点)field(n,条件项存在概念字段,*c.存在概念节点);
        else field(n,条件项特征类型字段,*c.特征类型概念节点);
        field(root,条件项字段,基础组内节点引用{n});
    }
    if(const auto* n=std::get_if<稳定编码>(&vm))请求.追加字段组.push_back({*n,初始必要条件字段,基础组内节点引用{root}});
    else if(const auto* n=std::get_if<基础组内节点引用>(&vm))field(n->索引,初始必要条件字段,基础组内节点引用{root});
    else throw std::invalid_argument("generic initial owner reference invalid");
}

// 登记结果组（R110）：初始声明节点格式4，经 Vm 的登记结果组字段持有；项为初始声明项节点：
// 顺序、有序参数序号（0＝不依附参数）、条件项特征类型（必有）、条件项存在概念（参数序号0时必有）。
bool 登记结果组形状有效(const std::vector<新方法登记结果项>& 组,const std::vector<新方法参数规格>& 参数,
    const 概念_特征类& 特征概念) {
    for(std::size_t i=0;i<组.size();++i) {
        const auto& r=组[i];
        if(!有效(r.特征类型概念节点))return false;
        if(r.参数序号==0) { if(!r.作用存在概念节点||!有效(*r.作用存在概念节点))return false; }
        else {
            if(r.作用存在概念节点||r.参数序号>参数.size())return false;
            const auto& s=参数[r.参数序号-1];
            if(!s.有序规格||s.有序规格->序号!=r.参数序号
                ||!持有者特征类型允许(s.有序规格->信息类型,r.特征类型概念节点,特征概念))return false;
        }
        for(std::size_t j=0;j<i;++j)if(组[j]==r)return false;
    }
    return true;
}
void 追加登记结果组(基础节点组追加请求& 请求,基础组内字段内容 vm,const std::vector<新方法登记结果项>& 组) {
    if(组.empty())return;
    const auto root=请求.新节点组.size();请求.新节点组.emplace_back();
    const auto field=[&](std::size_t n,稳定编码 key,基础组内字段内容 value){请求.新节点组[n].字段组.push_back({key,std::move(value)});};
    field(root,节点类型字段,初始声明节点类型);field(root,初始声明格式字段,基础值{std::int64_t{4}});
    field(root,初始声明所属字段,vm);field(root,关系规则字段,基础值{static_cast<std::int64_t>(新方法项关系规则::全部成立)});
    for(std::size_t i=0;i<组.size();++i) {
        const auto& r=组[i];const auto n=请求.新节点组.size();请求.新节点组.emplace_back();
        field(n,节点类型字段,初始声明项节点类型);field(n,初始声明所属字段,基础组内节点引用{root});
        field(n,顺序字段,基础值{static_cast<std::int64_t>(i)});
        field(n,有序参数序号字段,基础值{static_cast<std::int64_t>(r.参数序号)});
        field(n,条件项特征类型字段,r.特征类型概念节点);
        if(r.作用存在概念节点)field(n,条件项存在概念字段,*r.作用存在概念节点);
        field(root,条件项字段,基础组内节点引用{n});
    }
    if(const auto* n=std::get_if<稳定编码>(&vm))请求.追加字段组.push_back({*n,登记结果组字段,基础组内节点引用{root}});
    else if(const auto* n=std::get_if<基础组内节点引用>(&vm))field(n->索引,登记结果组字段,基础组内节点引用{root});
    else throw std::invalid_argument("registered result group owner reference invalid");
}
std::vector<新方法登记结果项> 读取登记结果组(稳定编码 vm,const 概念_特征类& features) {
    const auto roots=全局基础数据集.查询字段(vm,登记结果组字段);
    if(roots.empty())return {};
    const auto* root=roots.size()==1?std::get_if<稳定编码>(&roots.front().内容):nullptr;
    if(!root||!节点类型为(*root,初始声明节点类型))throw std::runtime_error("registered result group reference damaged");
    const auto parent=全局基础数据集.查询目标字段(*root,登记结果组字段);
    if(parent.size()!=1||parent.front().所属节点!=vm
        ||读取唯一I64字段(*root,初始声明格式字段)!=std::optional<std::int64_t>{4}
        ||读取唯一节点字段(*root,初始声明所属字段)!=std::optional<稳定编码>{vm})
        throw std::runtime_error("registered result group owner or format");
    const auto items=读取有序子节点(*root,条件项字段,features);
    if(!items||items->empty())throw std::runtime_error("registered result group items");
    std::vector<新方法登记结果项> 组;
    for(std::size_t i=0;i<items->size();++i) {
        const auto n=(*items)[i];
        if(!节点类型为(n,初始声明项节点类型)||读取唯一节点字段(n,初始声明所属字段)!=std::optional<稳定编码>{*root}
            ||读取唯一I64字段(n,顺序字段)!=std::optional{static_cast<std::int64_t>(i)})
            throw std::runtime_error("registered result group item shape");
        const auto pos=读取唯一I64字段(n,有序参数序号字段);
        const auto ft=读取唯一节点字段(n,条件项特征类型字段);
        if(!pos||*pos<0||*pos>(std::numeric_limits<std::uint32_t>::max)()||!ft)
            throw std::runtime_error("registered result group item fields");
        新方法登记结果项 r;r.参数序号=static_cast<std::uint32_t>(*pos);r.特征类型概念节点=*ft;
        r.作用存在概念节点=读取唯一节点字段(n,条件项存在概念字段);
        组.push_back(r);
    }
    return 组;
}

bool 初始必要条件形状有效(const 新方法初始必要条件定义& d) {
    if(d.条件.关系规则!=新方法项关系规则::全部成立||d.条件.条件项组.size()!=1||!d.条件.场景事实限定组.empty()
        ||d.条件.参数关系组!=std::vector<新方法参数关系>{{新方法参数关系种类::不同,2,3}}
        ||d.场景事实限定组.size()!=1)return false;
    const auto& c=d.条件.条件项组.front();新方法条件项定义 expected;
    if(c.引用内容条件)return false; // 初始声明格式1不得静默丢弃新形状。
    expected.参数序号=3;expected.存在概念节点=c.存在概念节点;
    const auto& r=d.场景事实限定组.front();
    return c.存在概念节点&&有效(*c.存在概念节点)&&c==expected&&有效(r.关系概念节点)
        &&r.场景参数序号==1&&r.主体参数序号==2&&r.对象参数序号==3&&r.特征参数序号==4;
}
std::optional<新方法初始必要条件定义> 读取初始必要条件(稳定编码 vm,
    const 概念_存在类& concepts,const 概念_特征类& features,bool 核概念=true) {
    const auto root=读取可选节点字段(vm,初始必要条件字段);
    if(!root)throw std::runtime_error("initial requirements reference damaged");
    if(!有效(*root))return {};
    const auto parent=全局基础数据集.查询目标字段(*root,初始必要条件字段);
    if(parent.size()!=1||parent.front().所属节点!=vm)throw std::runtime_error("initial requirement shared root");
    const auto exactShape=[&](稳定编码 n,稳定编码 type) {
        if(!节点类型为(n,type))throw std::runtime_error("initial requirement node type");
        const auto s=全局基础数据集.查询完整节点结构(n);
        if(s.状态!=基础结构操作状态::已完成||!s.结构)throw std::runtime_error("initial requirement snapshot unavailable");
        const auto schema=方法复制字段规格(type);
        for(const auto& f:s.结构->字段组)
            if(std::none_of(schema.begin(),schema.end(),[&](const auto& x){return x.字段==f.字段节点;}))
                throw std::runtime_error("initial requirement unknown field");
    };
    exactShape(*root,初始声明节点类型);
    const auto format=读取唯一I64字段(*root,初始声明格式字段);
    if(!format||(*format!=1&&*format!=2&&*format!=3)
        ||读取唯一节点字段(*root,初始声明所属字段)!=std::optional<稳定编码>{vm})
        throw std::runtime_error("initial requirement format or owner");
    if(*format==2||*format==3) {
        新方法初始必要条件定义 d;d.条件.关系规则=新方法项关系规则::全部成立;
        if(读取唯一I64字段(*root,关系规则字段)!=std::optional{static_cast<std::int64_t>(d.条件.关系规则)}
            ||!全局基础数据集.查询字段(*root,参数位置关系字段).empty()
            ||!全局基础数据集.查询字段(*root,初始场景限定字段).empty())
            throw std::runtime_error("generic initial requirement relation rule");
        const auto items=读取有序子节点(*root,条件项字段,features);
        const auto rs=全局基础数据集.查询完整节点结构(*root);
        if(!items||items->empty()||!rs.结构||rs.结构->字段组.size()!=items->size()+4)
            throw std::runtime_error("generic initial requirement children or fields");
        for(std::size_t i=0;i<items->size();++i) {
            const auto n=(*items)[i];exactShape(n,初始声明项节点类型);
            const auto ps=全局基础数据集.查询目标字段(n,条件项字段);
            const auto ns=全局基础数据集.查询完整节点结构(n);
            const auto pos=读取唯一I64字段(n,有序参数序号字段);
            const auto ft=读取唯一节点字段(n,条件项特征类型字段);
            const auto existence=读取唯一节点字段(n,条件项存在概念字段);
            if(ps.size()!=1||ps.front().所属节点!=*root||!ns.结构||ns.结构->字段组.size()!=5
                ||读取唯一节点字段(n,初始声明所属字段)!=root
                ||读取唯一I64字段(n,顺序字段)!=std::optional{static_cast<std::int64_t>(i)}
                ||!pos||*pos<=0||*pos>(std::numeric_limits<std::uint32_t>::max)()
                ||ft.has_value()==existence.has_value()
                ||(*format==2&&(!ft||!全局基础数据集.查询字段(n,条件项存在概念字段).empty())))
                throw std::runtime_error("generic initial requirement item shape");
            新方法条件项定义 c;c.参数序号=static_cast<std::uint32_t>(*pos);
            c.特征类型概念节点=ft;c.存在概念节点=existence;
            if(核概念&&existence) {
                std::vector<基础联合节点快照> proof;
                if(!追加方法持有者概念保护(*existence,concepts,proof))
                    throw std::runtime_error("generic initial requirement formal existence unavailable");
            }
            if(核概念&&ft) {
                const auto proof=features.读取特征概念定义保护(*ft);
                if(proof.状态!=基础结构操作状态::已完成||proof.前提已变化||!proof.信息||proof.信息->类型根!=*ft
                    ||proof.信息->定义.节点!=*ft||proof.预期节点组.empty())
                    throw std::runtime_error("generic initial requirement formal feature unavailable");
            }
            d.条件.条件项组.push_back(std::move(c));
        }
        if(!通用概念必要声明形状有效(d)
            ||(*format==3&&std::none_of(d.条件.条件项组.begin(),d.条件.条件项组.end(),
                [](const auto& c){return c.存在概念节点.has_value();})))
            throw std::runtime_error("generic initial requirement definition shape");
        return d;
    }
    const auto item=读取唯一节点字段(*root,条件项字段),relation=读取唯一节点字段(*root,初始场景限定字段);
    if(!item||!relation)throw std::runtime_error("initial requirement children");
    for(const auto pair:{std::pair{*item,条件项字段},std::pair{*relation,初始场景限定字段}}) {
        const auto parents=全局基础数据集.查询目标字段(pair.first,pair.second);
        if(parents.size()!=1||parents.front().所属节点!=*root)throw std::runtime_error("initial requirement shared child");
    }
    exactShape(*item,初始声明项节点类型);exactShape(*relation,初始场景限定节点类型);
    if(!全局基础数据集.查询字段(*item,条件项特征类型字段).empty())
        throw std::runtime_error("format one initial requirement has generic feature field");
    for(const auto n:{*item,*relation})if(读取唯一节点字段(n,初始声明所属字段)!=root
        ||读取唯一I64字段(n,顺序字段)!=std::optional<std::int64_t>{0})throw std::runtime_error("initial requirement order/owner");
    新方法初始必要条件定义 d;d.条件.关系规则=新方法项关系规则::全部成立;
    const auto rule=读取唯一I64字段(*root,关系规则字段);const auto pairs=读取参数关系(*root);
    if(!rule||*rule!=static_cast<std::int64_t>(d.条件.关系规则)||!pairs)throw std::runtime_error("initial requirement relation rule");
    d.条件.参数关系组=*pairs;
    新方法条件项定义 c;c.参数序号=3;c.存在概念节点=读取唯一节点字段(*item,条件项存在概念字段);
    if(读取唯一I64字段(*item,有序参数序号字段)!=std::optional<std::int64_t>{3})throw std::runtime_error("initial requirement parameter");
    d.条件.条件项组.push_back(c);
    const auto rc=读取唯一节点字段(*relation,初始关系概念字段);
    const auto fields=全局基础数据集.查询字段(*relation,初始四位置字段);
    const auto* value=fields.size()==1 ? std::get_if<基础值>(&fields.front().内容) : nullptr;
    const auto* positions=value ? std::get_if<std::vector<std::int64_t>>(&value->材料) : nullptr;
    if(!rc||!positions||*positions!=std::vector<std::int64_t>{1,2,3,4})throw std::runtime_error("initial fact positions");
    d.场景事实限定组.push_back({*rc,1,2,3,4});
    if(!初始必要条件形状有效(d))throw std::runtime_error("initial requirement definition shape");
    if(核概念) {
        const auto relationProof=concepts.读取已确认消息连接概念保护();
        if(!concepts.获取存在概念(*c.存在概念节点)||!relationProof.信息||relationProof.信息->节点!=*rc)
            throw std::runtime_error("initial requirement formal definition unavailable");
    }
    return d;
}

thread_local int 方法读失败行 = 0; // 诊断：本线程最近一次读取方法信息本体返回空的行号
std::optional<新方法信息> 读取方法信息本体(稳定编码 方法节点, const 概念_特征类& 概念服务,
    const 新方法治理对象读取接口& 治理读取, const 概念_存在类& 存在概念服务, bool 只读旧形状,
    bool 核初始概念) {
    if (!节点类型为(方法节点, 方法节点类型)
        || !方法属于根(方法节点)) {方法读失败行=__LINE__;return std::nullopt;}
    const auto 作用域 = 读取唯一I64字段(方法节点, 方法作用域字段);
    const auto 实现方式 = 读取唯一I64字段(方法节点, 方法实现方式字段);
    std::optional<std::int64_t> 启用;
    const auto 头节点 = 读取唯一节点字段(方法节点, 方法头字段);
    const auto 虚拟存在 = 读取唯一节点字段(方法节点, 方法虚拟存在字段);
    std::optional<新方法虚拟存在读回> 治理;
    if (只读旧形状) {
        if (!虚拟存在 || !全局基础数据集.查询字段(*虚拟存在, 虚拟存在启用特征字段).empty()
            || !全局基础数据集.查询字段(*虚拟存在, 虚拟存在专属概念字段).empty()) {方法读失败行=__LINE__;return std::nullopt;}
        启用 = 读取唯一I64字段(方法节点, 方法启用状态字段);
    } else if (虚拟存在) {
        const auto F = 读取唯一节点字段(*虚拟存在, 虚拟存在启用特征字段);
        if (!F || !全局基础数据集.查询字段(方法节点, 方法启用状态字段).empty())
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取方法启用", L"方法仍含旧启用值或缺少唯一正式特征。");
        治理 = 治理读取.读取方法虚拟存在引用({方法节点, *虚拟存在, *F});
        if (治理) 启用 = 治理->当前启用;
    }
    const auto 配对节点组 = 读取有序子节点(方法节点, 方法条件结果字段, 概念服务);
    const auto 步骤节点组 = 读取多节点字段(方法节点, 方法组合步骤字段, 概念服务);
    const auto 转移节点组 = 读取多节点字段(方法节点, 方法结果转移字段, 概念服务);
    const auto 入口步骤 = 读取可选节点字段(方法节点, 方法组合入口步骤字段);
    if (!作用域 || !是作用域(*作用域) || !实现方式
        || !是实现方式(*实现方式) || !启用 || (*启用 != 0 && *启用 != 1)
        || !头节点 || !节点类型为(*头节点, 方法头节点类型)
        || !虚拟存在 || !节点类型为(*虚拟存在, 方法虚拟存在节点类型)
        || !配对节点组 || !步骤节点组 || !转移节点组 || !入口步骤) {
        {方法读失败行=__LINE__;return std::nullopt;}
    }
    const auto 虚拟所属 = 读取唯一节点字段(
        *虚拟存在, 虚拟存在所属方法字段);
    const auto 参数节点组 = 读取有序子节点(
        *虚拟存在, 虚拟存在参数规格字段, 概念服务);
    if (!虚拟所属 || *虚拟所属 != 方法节点 || !参数节点组) {
        {方法读失败行=__LINE__;return std::nullopt;}
    }

    新方法信息 信息;
    信息.节点 = 方法节点;
    const auto 初始关系=读取参数关系(*虚拟存在);
    if(!初始关系) {方法读失败行=__LINE__;return std::nullopt;}
    信息.初始模拟关系组=*初始关系;
    const auto 事件位置=读取模拟事件位置(*虚拟存在);
    if(!事件位置) {方法读失败行=__LINE__;return std::nullopt;}
    信息.初始模拟事件参数组=*事件位置;
    const auto 输入方案=读取模拟输入方案(*虚拟存在,概念服务);
    if(!输入方案){方法读失败行=__LINE__;return std::nullopt;}
    信息.初始模拟输入方案组=*输入方案;
    信息.初始必要条件=读取初始必要条件(*虚拟存在,存在概念服务,概念服务,核初始概念);
    信息.登记结果组=读取登记结果组(*虚拟存在,概念服务);
    if (有效(方法行为字段)) {
        const auto 字段组 = 全局基础数据集.查询字段(方法节点, 方法行为字段);
        if (字段组.size() > 1) {方法读失败行=__LINE__;return std::nullopt;}
        if (!字段组.empty()) {
            const auto* 基础 = std::get_if<基础值>(&字段组.front().内容);
            const auto* 值 = 基础 ? std::get_if<std::int64_t>(&基础->材料) : nullptr;
            if (!值 || !是方法行为(*值)) {方法读失败行=__LINE__;return std::nullopt;}
            信息.行为 = static_cast<新方法行为>(*值);
        }
    }
    bool 有祖代方法 = false;
    if (有效(方法父代字段)) {
        const auto 父代 = 读取可选节点字段(方法节点, 方法父代字段);
        if (!父代) {方法读失败行=__LINE__;return std::nullopt;}
        if (有效(*父代)) {
            std::set<稳定编码> 祖先组{方法节点};
            auto 祖先 = *父代;
            std::size_t 祖先数量 = 0;
            while (有效(祖先)) {
                if (++祖先数量 > 2 || !祖先组.insert(祖先).second
                    || !节点类型为(祖先, 方法节点类型) || !方法属于根(祖先)
                    || !全局基础数据集.查询字段(祖先, 方法来源字段).empty()
                    || !全局基础数据集.查询字段(祖先, 方法实例根字段).empty()) {方法读失败行=__LINE__;return std::nullopt;}
                const auto 上代 = 读取可选节点字段(祖先, 方法父代字段);
                if (!上代) {方法读失败行=__LINE__;return std::nullopt;}
                祖先 = *上代;
            }
            有祖代方法 = 祖先数量 == 2;
            信息.父代方法节点 = *父代;
        }
    }
    const auto 来源 = 读取可选节点字段(方法节点, 方法来源字段);
    const auto 实例根 = 读取可选节点字段(方法节点, 方法实例根字段);
    if (!来源 || !实例根 || 有效(*来源) != 有效(*实例根)) {方法读失败行=__LINE__;return std::nullopt;}
    if (有效(*来源)) {
        if (!节点类型为(*来源, 方法节点类型) || !方法属于根(*来源)
            || !全局基础数据集.查询字段(*来源, 方法来源字段).empty()
            || !全局基础数据集.查询字段(*来源, 方法实例根字段).empty()) {
            {方法读失败行=__LINE__;return std::nullopt;}
        }
        if (!读取实例根结构(*实例根, 概念服务)) {方法读失败行=__LINE__;return std::nullopt;}
        信息.来源方法节点 = *来源;
        信息.实例根节点 = *实例根;
    }
    信息.作用域 = static_cast<新方法作用域>(*作用域);
    信息.实现方式 = static_cast<新方法实现方式>(*实现方式);
    信息.启用状态 = static_cast<新方法启用状态>(*启用);
    信息.方法头节点 = *头节点;
    信息.方法虚拟存在节点 = *虚拟存在;
    if (治理) {
        信息.启用特征节点 = 治理->引用.启用特征节点;
        信息.专属存在概念节点 = 治理->专属存在概念节点;
    }
    const auto 头存在 = 读取多节点字段(*头节点, 方法头作用存在概念字段, 概念服务);
    const auto 头特征 = 读取多节点字段(*头节点, 方法头特征类型概念字段, 概念服务);
    const auto 头结果 = 读取多节点字段(*头节点, 方法头结果概念字段, 概念服务);
    if (!头存在 || !头特征 || !头结果) {方法读失败行=__LINE__;return std::nullopt;}
    信息.方法头 = {*头存在, *头特征, *头结果};
    for (const auto 参数节点 : *参数节点组) {
        const auto 参数 = 读取参数规格(参数节点, 概念服务, 存在概念服务);
        if (!参数) {方法读失败行=__LINE__;return std::nullopt;}
        信息.参数规格组.push_back(*参数);
    }
    if (!参数规格有效(信息.参数规格组, 概念服务)) {方法读失败行=__LINE__;return std::nullopt;}
    for(const auto& p:信息.参数规格组)if(p.有序规格
        &&是特征持有参数(p.有序规格->信息类型)
        &&!方法持有者范围有效(*p.有序规格,存在概念服务)){方法读失败行=__LINE__;return std::nullopt;}
    if(信息.初始必要条件&&通用概念必要声明形状有效(*信息.初始必要条件))
        for(const auto& c:信息.初始必要条件->条件.条件项组) {
            if(c.参数序号>信息.参数规格组.size()){方法读失败行=__LINE__;return std::nullopt;}
            const auto& p=信息.参数规格组[c.参数序号-1].有序规格;
            if(!p||p->序号!=c.参数序号||!是特征持有参数(p->信息类型)){方法读失败行=__LINE__;return std::nullopt;}
        }
    const auto branches=读取有序子节点(*虚拟存在,执行分支字段,概念服务);
    if(!branches){方法读失败行=__LINE__;return std::nullopt;}
    for(std::size_t i=0;i<branches->size();++i) {
        const auto B=(*branches)[i];
        const auto owners=全局基础数据集.查询目标字段(B,执行分支字段);
        const auto C=读取唯一节点字段(B,分支条件字段);
        if(!节点类型为(B,分支节点类型)||owners.size()!=1||owners.front().所属节点!=*虚拟存在
            ||读取唯一节点字段(B,分支所属方法字段)!=std::optional{方法节点}
            ||读取唯一节点字段(B,分支所属虚拟字段)!=虚拟存在||!C
            ||读取唯一I64字段(B,顺序字段)!=std::optional<std::int64_t>{static_cast<std::int64_t>(i)}){方法读失败行=__LINE__;return std::nullopt;}
        const auto node=全局基础数据集.查询完整节点结构(B);
        if(!node.结构){方法读失败行=__LINE__;return std::nullopt;}
        for(const auto& field:node.结构->字段组)
            if(field.字段节点!=节点类型字段&&field.字段节点!=顺序字段&&field.字段节点!=分支所属方法字段
                &&field.字段节点!=分支所属虚拟字段&&field.字段节点!=分支条件字段){方法读失败行=__LINE__;return std::nullopt;}
        const auto cOwners=全局基础数据集.查询目标字段(*C,分支条件字段);
        if(cOwners.size()!=1||cOwners.front().所属节点!=B||!全局基础数据集.查询目标字段(*C,配对条件字段).empty()){方法读失败行=__LINE__;return std::nullopt;}
        const auto conditionNode=全局基础数据集.查询完整节点结构(*C);
        if(!conditionNode.结构){方法读失败行=__LINE__;return std::nullopt;}
        for(const auto& field:conditionNode.结构->字段组)
            if(field.字段节点!=节点类型字段&&field.字段节点!=关系规则字段&&field.字段节点!=参数位置关系字段
                &&field.字段节点!=条件项字段&&field.字段节点!=初始场景限定字段){方法读失败行=__LINE__;return std::nullopt;}
        const auto c=读取条件(*C,概念服务);
        if(!c||!分支条件有效(*c,信息.参数规格组,概念服务,存在概念服务,引用内容条件校验模式::持久结构)){方法读失败行=__LINE__;return std::nullopt;}
        信息.执行分支组.push_back({B,{static_cast<std::uint32_t>(i),*c}});
    }
    新方法条件结果定义 初始核验;
    初始核验.条件.参数关系组 = 信息.初始模拟关系组;
    if(!模拟输入方案有效(信息.初始模拟输入方案组,信息.参数规格组,信息.初始模拟关系组,概念服务)){方法读失败行=__LINE__;return std::nullopt;}
    if (!条件结果角色已声明(信息.参数规格组, 初始核验, 概念服务, 存在概念服务,引用内容条件校验模式::持久结构)) {方法读失败行=__LINE__;return std::nullopt;}
    std::set<稳定编码> 已见来源配对;
    for (const auto 配对节点 : *配对节点组) {
        if (!节点类型为(配对节点, 条件结果配对节点类型)) {方法读失败行=__LINE__;return std::nullopt;}
        const auto 条件节点 = 读取唯一节点字段(配对节点, 配对条件字段);
        const auto 结果节点 = 读取唯一节点字段(配对节点, 配对结果字段);
        const auto 命中 = 读取可选I64字段(配对节点, 配对命中次数字段);
        const auto 配对来源 = 读取可选节点字段(配对节点, 配对来源字段);
        const auto 知识来源 = 读取可选I64字段(配对节点, 配对知识来源字段);
        if (!知识来源 || (*知识来源 && **知识来源 != 1 && **知识来源 != 2 && **知识来源 != 3)) {方法读失败行=__LINE__;return std::nullopt;}
        if (!条件节点 || !结果节点 || !命中 || (*命中 && **命中 < 0)
            || !配对来源) {方法读失败行=__LINE__;return std::nullopt;}
        if (有效(*配对来源)) {
            if (!已见来源配对.insert(*配对来源).second) {方法读失败行=__LINE__;return std::nullopt;}
            const auto 持有 = 全局基础数据集.查询目标字段(*配对来源, 方法条件结果字段);
            if (!信息.来源方法节点 || !节点类型为(*配对来源, 条件结果配对节点类型)
                || 持有.size() != 1 || 持有.front().所属节点 != *信息.来源方法节点
                || !全局基础数据集.查询字段(*配对来源, 配对来源字段).empty()) {方法读失败行=__LINE__;return std::nullopt;}
        }
        const auto 条件 = 读取条件(*条件节点, 概念服务);
        const auto 结果 = 读取结果(*结果节点, 概念服务);
        const auto 线性 = 读取线性关系组(配对节点);
        if (!条件 || !结果 || !线性
            || !条件结果角色已声明(信息.参数规格组, {*条件, *结果, *线性}, 概念服务, 存在概念服务,引用内容条件校验模式::持久结构)
            || !线性关系有效({*条件, *结果, *线性}, 概念服务, 信息.实例根节点.has_value())) {方法读失败行=__LINE__;return std::nullopt;}
        // 不变量（CONV-INSTINCT 6.3）：先天被动结果项只出现在先天来源配对中，先天配对恰一被动项。
        {
            const bool 先天 = *知识来源 && **知识来源 == 3;
            const auto 被动数 = std::count_if(结果->结果项组.begin(), 结果->结果项组.end(),
                [](const auto& 项) { return 项.被动需求目标; });
            if (std::any_of(结果->确认要求.禁止结果项组.begin(), 结果->确认要求.禁止结果项组.end(),
                    [](const auto& 项) { return 项.被动需求目标; })
                || (先天 ? (结果->结果项组.size() != 1
                        || (被动数 != 1 && !结果->结果项组.front().结果作用存在)) // 2026-11-10：或一个结果作用存在项
                    : 被动数 != 0)) {方法读失败行=__LINE__;return std::nullopt;}
        }
        信息.条件结果组.push_back({
            配对节点, *条件节点, *结果节点, *条件, *结果, 命中->value_or(0), *线性,
            有效(*配对来源) ? std::optional<稳定编码>{*配对来源} : std::nullopt,
            !*知识来源 ? 新方法知识来源种类::未注明
                : **知识来源 == 2 ? 新方法知识来源种类::尝试
                : **知识来源 == 3 ? 新方法知识来源种类::先天 : 新方法知识来源种类::模拟});
        const auto branchSources=读取多节点字段(配对节点,配对分支来源字段,概念服务);
        if(!branchSources){方法读失败行=__LINE__;return std::nullopt;}
        if(信息.执行分支组.empty()!=branchSources->empty()){方法读失败行=__LINE__;return std::nullopt;}
        for(const auto B:*branchSources)
            if(std::none_of(信息.执行分支组.begin(),信息.执行分支组.end(),[&](const auto& b){return b.节点==B;})){方法读失败行=__LINE__;return std::nullopt;}
        信息.条件结果组.back().来源执行分支节点组=*branchSources;
        if(!配对分支来源有效(信息,{*条件,*结果,*线性,*branchSources},概念服务,存在概念服务)){方法读失败行=__LINE__;return std::nullopt;}
    }
    const auto 函数组 = 全局基础数据集.查询字段(
        方法节点, 方法本能函数登记名字段);
    if (信息.实现方式 == 新方法实现方式::本能) {
        if (函数组.size() > 1) {方法读失败行=__LINE__;return std::nullopt;}
        if (函数组.size() == 1) {
            const auto* 值 = std::get_if<基础值>(&函数组.front().内容);
            const auto* 名称 = 值
                ? std::get_if<std::string>(&值->材料) : nullptr;
            if (!名称 || 名称->empty()) {方法读失败行=__LINE__;return std::nullopt;}
            信息.本能函数登记名 = *名称;
        }
    } else if (!函数组.empty()) {
        {方法读失败行=__LINE__;return std::nullopt;}
    }
    if (信息.父代方法节点) {
        if (有祖代方法)
            {方法读失败行=__LINE__;return std::nullopt;}
        if (信息.本能函数登记名 != 新本能函数登记名_旧两参数获取服务对象
            || 信息.作用域 != 新方法作用域::普通世界
            || 信息.实现方式 != 新方法实现方式::本能
            || !步骤节点组->empty() || !转移节点组->empty() || 有效(*入口步骤)) {方法读失败行=__LINE__;return std::nullopt;}
        if (信息.来源方法节点) {
            const auto 来源父代 = 读取可选节点字段(*信息.来源方法节点, 方法父代字段);
            if (!来源父代 || *来源父代 != *信息.父代方法节点) {方法读失败行=__LINE__;return std::nullopt;}
        }
    }

    for (const auto 步骤节点 : *步骤节点组) {
        if (!节点类型为(步骤节点, 组合步骤节点类型)) {方法读失败行=__LINE__;return std::nullopt;}
        const auto 所属 = 读取唯一节点字段(步骤节点, 步骤所属组合方法字段);
        const auto 调用 = 读取唯一节点字段(步骤节点, 步骤调用方法字段);
        const auto 顺序 = 读取唯一I64字段(步骤节点, 顺序字段);
        if (!所属 || *所属 != 方法节点 || !调用 || !顺序 || *顺序 < 0) {
            {方法读失败行=__LINE__;return std::nullopt;}
        }
        const auto 调用根 = 读取可选节点字段(*调用, 方法实例根字段);
        const auto 调用来源 = 读取可选节点字段(*调用, 方法来源字段);
        if (!节点类型为(*调用, 方法节点类型) || !方法属于根(*调用)
            || !调用根 || !调用来源 || *调用根 != *实例根
            || 有效(*调用根) != 有效(*调用来源)) {方法读失败行=__LINE__;return std::nullopt;}
        信息.组合步骤组.push_back({步骤节点, *调用, *顺序});
    }
    std::sort(信息.组合步骤组.begin(), 信息.组合步骤组.end(),
        [](const 新方法组合步骤信息& 左, const 新方法组合步骤信息& 右) {
            return std::tie(左.顺序, 左.步骤节点)
                < std::tie(右.顺序, 右.步骤节点);
        });
    for (std::size_t i = 1; i < 信息.组合步骤组.size(); ++i) {
        if (信息.组合步骤组[i - 1].顺序 == 信息.组合步骤组[i].顺序) {
            {方法读失败行=__LINE__;return std::nullopt;}
        }
    }
    if (有效(*入口步骤)) {
        const auto 找到 = std::find_if(信息.组合步骤组.begin(),
            信息.组合步骤组.end(), [&](const 新方法组合步骤信息& 步骤) {
                return 步骤.步骤节点 == *入口步骤;
            });
        if (找到 == 信息.组合步骤组.end()) {方法读失败行=__LINE__;return std::nullopt;}
        信息.组合入口步骤节点 = *入口步骤;
    }
    for (const auto 转移节点 : *转移节点组) {
        if (!节点类型为(转移节点, 结果转移节点类型)) {方法读失败行=__LINE__;return std::nullopt;}
        const auto 所属 = 读取唯一节点字段(转移节点, 转移所属组合方法字段);
        const auto 当前步骤 = 读取唯一节点字段(转移节点, 转移当前步骤字段);
        const auto 当前结果 = 读取唯一节点字段(转移节点, 转移当前结果字段);
        const auto 匹配 = 读取可选节点字段(转移节点, 转移匹配结果概念字段);
        const auto 下一步骤 = 读取唯一节点字段(转移节点, 转移下一步骤字段);
        if (!所属 || *所属 != 方法节点 || !当前步骤 || !当前结果
            || !匹配 || !下一步骤) {方法读失败行=__LINE__;return std::nullopt;}
        const auto 当前 = std::find_if(信息.组合步骤组.begin(), 信息.组合步骤组.end(),
            [&](const auto& 步骤) { return 步骤.步骤节点 == *当前步骤; });
        if (当前 == 信息.组合步骤组.end()
            || std::none_of(信息.组合步骤组.begin(), 信息.组合步骤组.end(),
                [&](const auto& 步骤) { return 步骤.步骤节点 == *下一步骤; })) {
            {方法读失败行=__LINE__;return std::nullopt;}
        }
        const auto 调用配对 = 读取有序子节点(当前->调用方法节点, 方法条件结果字段, 概念服务);
        if (!调用配对 || std::none_of(调用配对->begin(), 调用配对->end(),
            [&](稳定编码 配对) {
                return 读取唯一节点字段(配对, 配对结果字段) == 当前结果;
            })) {方法读失败行=__LINE__;return std::nullopt;}
        新方法结果转移信息 转移{
            转移节点, *当前步骤, *当前结果, std::nullopt, *下一步骤};
        if (有效(*匹配)) 转移.匹配结果概念节点 = *匹配;
        信息.结果转移组.push_back(std::move(转移));
    }
    std::sort(信息.结果转移组.begin(), 信息.结果转移组.end(),
        [](const 新方法结果转移信息& 左, const 新方法结果转移信息& 右) {
            return 左.转移节点 < 右.转移节点;
        });
    if (信息.实现方式 == 新方法实现方式::本能
        && (!信息.组合步骤组.empty() || 信息.组合入口步骤节点
            || !信息.结果转移组.empty())) {方法读失败行=__LINE__;return std::nullopt;}
    if (std::any_of(信息.条件结果组.begin(), 信息.条件结果组.end(),
            [&](const auto& 配对) { return !行为结果相容(信息.行为, 配对.结果); }))
        {方法读失败行=__LINE__;return std::nullopt;}
    return 信息;
}

std::optional<新方法信息> 读取方法信息(稳定编码 方法节点, const 概念_特征类& 概念服务,
    const 新方法治理对象读取接口& 治理读取, const 概念_存在类& 存在概念服务, bool 只读旧形状 = false,
    bool 核初始概念 = true) {
    if (!结构仍存在()) return std::nullopt;
    return 读取方法信息本体(方法节点, 概念服务, 治理读取, 存在概念服务, 只读旧形状, 核初始概念);
}

// 调用者已核验原登记声明并持有方法owner锁。未知不推断，唯一追加为发布点。
新方法建立结果 补登记方法行为(const 新方法信息& 原,
    const std::optional<新方法行为>& 声明, const 概念_特征类& 概念,
    const 新方法治理对象读取接口& 治理, const 概念_存在类& 存在概念服务) {
    if (!声明 || 原.行为 == 声明) return {新方法操作状态::已找到, 原.节点};
    if (原.行为 || !是方法行为(static_cast<std::int64_t>(*声明))
        || std::any_of(原.条件结果组.begin(), 原.条件结果组.end(),
            [&](const auto& 配对) { return !行为结果相容(声明, 配对.结果); }))
        return {新方法操作状态::函数登记冲突, std::nullopt};
    const auto 快照 = 全局基础数据集.查询节点组完整结构({原.节点, 方法行为字段});
    if (快照.状态 != 基础结构操作状态::已完成 || 快照.节点组.size() != 2)
        throw std::runtime_error("method behavior snapshot unavailable");
    基础节点组追加请求 请求;
    for (const auto& 节点 : 快照.节点组) {
        if (!节点.快照) throw std::runtime_error("method behavior snapshot incomplete");
        请求.预期联合节点组.push_back(*节点.快照);
    }
    const auto 当前 = 读取方法信息(原.节点, 概念, 治理, 存在概念服务);
    if (!当前 || *当前 != 原) throw std::runtime_error("method behavior declaration changed");
    auto 预期 = 原;
    预期.行为 = 声明;
    请求.追加字段组.push_back({原.节点, 方法行为字段, 基础值{static_cast<std::int64_t>(*声明)}});
    const auto 发布 = 全局基础数据集.原子追加节点组(请求);
    if (发布.状态 == 基础节点组追加状态::资源失败)
        return {新方法操作状态::资源失败, std::nullopt};
    if (发布.状态 != 基础节点组追加状态::已完成)
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"补登记方法行为", L"方法行为原子追加失败，保留现场。");
    try {
        const auto 读回 = 读取方法信息(原.节点, 概念, 治理, 存在概念服务);
        if (!读回 || *读回 != 预期) throw std::runtime_error("method behavior readback mismatch");
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"补登记方法行为", L"已发布分类读回不一致，保留已发表事实。");
    }
    return {新方法操作状态::已找到, 原.节点};
}

bool 写入有序引用组(
    稳定编码 所属节点,
    稳定编码 字段,
    const std::vector<稳定编码>& 目标组,
    std::vector<稳定编码>& 已写字段) {
    for (const auto 目标 : 目标组) {
        if (!写入节点字段(所属节点, 字段, 目标, 已写字段)) return false;
    }
    return true;
}

bool 写入参数规格(
    稳定编码 虚拟存在,
    const 新方法参数规格& 规格,
    std::int64_t 参数顺序,
    std::vector<稳定编码>& 已建节点,
    std::vector<稳定编码>& 已写字段,
    const 概念_存在类& 存在, const 概念_特征类& 特征) {
    const auto 参数节点 = 新建类型化节点(
        参数规格节点类型, 已建节点, 已写字段);
    if (规格.有序规格) {
        const auto& 新 = *规格.有序规格;
        const auto 类别 = 读取参数类别概念保护(新.信息类型, 存在, 特征);
        return 有效(参数节点)
            && 写入I64字段(参数节点, 顺序字段, 参数顺序, 已写字段)
            && 写入I64字段(参数节点, 有序参数序号字段, 新.序号, 已写字段)
            && 写入节点字段(参数节点, 有序参数类别概念字段, 类别.类别概念, 已写字段)
            && 写入节点字段(参数节点, 有序参数范围字段, 新.范围概念节点, 已写字段)
            && 写入有序引用组(参数节点, 有序参数必需特征类型字段, 新.必需特征类型组, 已写字段)
            && (新.所属存在位置 == 0
                || 写入I64字段(参数节点, 有序参数所属位置字段, 新.所属存在位置, 已写字段))
            && 写入节点字段(虚拟存在, 虚拟存在参数规格字段, 参数节点, 已写字段);
    }
    if (!有效(参数节点)
        || !写入I64字段(参数节点, 顺序字段, 参数顺序, 已写字段)
        || !写入节点字段(参数节点, 参数角色字段,
            规格.参数角色节点, 已写字段)
        || !写入I64字段(参数节点, 参数要求场景字段,
            规格.要求场景 ? 1 : 0, 已写字段)
        || !写入I64字段(参数节点, 参数对象种类字段,
            static_cast<std::int64_t>(规格.对象种类), 已写字段)) return false;
    if (规格.存在概念节点
        && !写入节点字段(参数节点, 参数存在概念字段,
            *规格.存在概念节点, 已写字段)) return false;
    if (规格.集合成员约束
        && (!写入节点字段(参数节点, 参数集合角色字段,
                规格.集合成员约束->集合参数角色节点, 已写字段)
            || !写入节点字段(参数节点, 参数集合特征类型字段,
                规格.集合成员约束->集合特征类型概念节点, 已写字段))) return false;
    for (std::size_t i = 0; i < 规格.特征规格组.size(); ++i) {
        const auto& 特征 = 规格.特征规格组[i];
        const auto 特征节点 = 新建类型化节点(
            参数特征规格节点类型, 已建节点, 已写字段);
        if (!有效(特征节点)
            || !写入I64字段(特征节点, 顺序字段,
                static_cast<std::int64_t>(i), 已写字段)
            || !写入节点字段(特征节点, 参数特征类型字段,
                特征.特征类型概念节点, 已写字段)) return false;
        if (特征.值域概念节点
            && !写入节点字段(特征节点, 参数特征值域字段,
                *特征.值域概念节点, 已写字段)) return false;
        if (!写入有序引用组(特征节点, 参数二次特征概念字段,
                特征.二次特征概念组, 已写字段)
            || !写入节点字段(参数节点, 参数特征规格字段,
                特征节点, 已写字段)) return false;
    }
    return 写入节点字段(虚拟存在, 虚拟存在参数规格字段,
        参数节点, 已写字段);
}

bool 写入结果项(
    稳定编码 结果节点,
    稳定编码 持有字段,
    const 新方法结果项定义& 源,
    std::int64_t 顺序,
    std::vector<稳定编码>& 已建节点,
    std::vector<稳定编码>& 已写字段) {
    if(源.引用内容结果&&源.引用内容结果->要求认知来源当前有效)return false;
    const auto 项 = 新建类型化节点(
        结果项节点类型, 已建节点, 已写字段);
    if (!有效(项)
        || !写入I64字段(项, 顺序字段, 顺序, 已写字段)
        || !(源.结果作用存在 ? 写入节点字段(项, 结果作用存在字段, *源.结果作用存在, 已写字段)
            : 源.参数序号 ? 写入I64字段(项, 有序参数序号字段, 源.参数序号, 已写字段)
            : 写入节点字段(项, 参数角色字段, 源.参数角色节点, 已写字段))
        || !写入I64字段(项, 结果项主要结果字段,
            源.主要结果 ? 1 : 0, 已写字段)) {
        return false;
    }
    const std::pair<const std::optional<稳定编码>*, 稳定编码> 可选组[]{
        {&源.特征类型概念节点, 结果项特征类型字段},
        {&源.特征值域概念节点, 结果项特征值域字段},
        {&源.变化方向概念节点, 结果项变化方向字段},
        {&源.状态或动态概念节点, 结果项状态动态字段}};
    for (const auto& [值, 字段] : 可选组) {
        if (*值 && !写入节点字段(项, 字段, **值, 已写字段)) return false;
    }
    if (源.精确数值域 && !写入精确数值域(项, *源.精确数值域, 已写字段)) return false;
    if (源.任务成员变量序号 && !写入I64字段(项, 任务成员变量字段, 源.任务成员变量序号, 已写字段)) return false;
    if (源.被动需求目标 && !写入I64字段(项, 结果被动需求目标字段, 1, 已写字段)) return false;
    if(源.引用内容结果) {
        const auto& c=*源.引用内容结果;
        if(!写入节点字段(项,结果引用内容要求字段,c.要求概念节点,已写字段)
            ||!写入I64字段(项,结果内容作用点字段,static_cast<std::int64_t>(c.作用点),已写字段))return false;
        if(!c.正式内容路径.empty()&&!写入节点字段(项,结果引用内容项字段,c.正式内容路径.front(),已写字段))return false;
    }
    return 写入节点字段(结果节点, 持有字段, 项, 已写字段);
}

稳定编码 写入条件子树(const 新方法条件定义& 定义,
    std::vector<稳定编码>& 已建节点, std::vector<稳定编码>& 已写字段) {
    const auto 条件 = 新建类型化节点(条件节点类型, 已建节点, 已写字段);
    if (!有效(条件) || !写入I64字段(条件, 关系规则字段,
        static_cast<std::int64_t>(定义.关系规则), 已写字段)) return {};
    for (std::size_t i = 0; i < 定义.条件项组.size(); ++i) {
        const auto& 源 = 定义.条件项组[i];
        const auto 项 = 新建类型化节点(条件项节点类型, 已建节点, 已写字段);
        if (!有效(项) || !写入I64字段(项, 顺序字段, static_cast<std::int64_t>(i), 已写字段)
            || !(源.参数序号 ? 写入I64字段(项, 有序参数序号字段, 源.参数序号, 已写字段)
                : 写入节点字段(项, 参数角色字段, 源.参数角色节点, 已写字段))) return {};
        const std::pair<const std::optional<稳定编码>*, 稳定编码> 可选组[]{
            {&源.存在概念节点, 条件项存在概念字段},
            {&源.特征类型概念节点, 条件项特征类型字段},
            {&源.特征值域概念节点, 条件项特征值域字段},
            {&源.二次特征概念节点, 条件项二次特征字段},
            {&源.状态或动态概念节点, 条件项状态动态字段}};
        for (const auto& [值, 字段] : 可选组) {
            if (*值 && !写入节点字段(项, 字段, **值, 已写字段)) return {};
        }
        if (源.精确数值域 && !写入精确数值域(项, *源.精确数值域, 已写字段)) return {};
        if (源.任务成员变量序号 && !写入I64字段(项, 任务成员变量字段, 源.任务成员变量序号, 已写字段)) return {};
        if(源.引用内容条件) {
            if(!写入节点字段(项,条件引用内容要求字段,源.引用内容条件->要求概念节点,已写字段))return {};
            if(源.引用内容条件->作用点!=新方法内容作用点::特征当前引用
                &&!写入I64字段(项,条件内容作用点字段,static_cast<std::int64_t>(源.引用内容条件->作用点),已写字段))return {};
            if(源.引用内容条件->要求认知来源当前有效&&!写入I64字段(项,条件内容认知有效字段,1,已写字段))return {};
            if(!源.引用内容条件->正式内容路径.empty()
                &&!写入节点字段(项,条件引用内容项字段,源.引用内容条件->正式内容路径.front(),已写字段))return {};
        }
        if(源.特征前态限定) {
            const auto& p=*源.特征前态限定;
            if(p.历史不同值数量&&*p.历史不同值数量>static_cast<std::uint64_t>((std::numeric_limits<std::int64_t>::max)()))return {};
            if(!写入I64字段(项,条件前态当前字段,static_cast<std::int64_t>(p.当前值状态),已写字段))return {};
            if(p.历史不同值数量&&!写入I64字段(项,条件前态历史数字段,static_cast<std::int64_t>(*p.历史不同值数量),已写字段))return {};
        }
        if (!写入节点字段(条件, 条件项字段, 项, 已写字段)) return {};
    }
    if(!写入参数关系(条件,定义.参数关系组,已建节点,已写字段)) return {};
    for(std::size_t i=0;i<定义.场景事实限定组.size();++i) {
        const auto& r=定义.场景事实限定组[i];
        const auto n=新建类型化节点(初始场景限定节点类型,已建节点,已写字段);
        if(!有效(n)||!写入I64字段(n,顺序字段,static_cast<std::int64_t>(i),已写字段)
            ||!写入节点字段(n,初始声明所属字段,条件,已写字段)
            ||!写入节点字段(n,初始关系概念字段,r.关系概念节点,已写字段))return {};
        已写字段.reserve(已写字段.size()+1);
        const auto f=全局基础数据集.添加字段值(n,初始四位置字段,基础值{std::vector<std::int64_t>{r.场景参数序号,r.主体参数序号,r.对象参数序号,r.特征参数序号}});
        if(!有效(f))return {};已写字段.push_back(f);
        if(!写入节点字段(条件,初始场景限定字段,n,已写字段))return {};
    }
    return 条件;
}

bool 写入条件结果(
    稳定编码 方法节点,
    const 新方法条件结果定义& 定义,
    std::int64_t 配对顺序,
    std::vector<稳定编码>& 已建节点,
    std::vector<稳定编码>& 已写字段,
    稳定编码* 未发布配对 = nullptr) {
    const auto 配对 = 新建类型化节点(
        条件结果配对节点类型, 已建节点, 已写字段);
    const auto 条件 = 写入条件子树(定义.条件, 已建节点, 已写字段);
    const auto 结果 = 新建类型化节点(
        结果节点类型, 已建节点, 已写字段);
    if (!有效(配对) || !有效(条件) || !有效(结果)
        || !写入I64字段(配对, 顺序字段, 配对顺序, 已写字段)
        || !写入I64字段(配对, 配对命中次数字段, 0, 已写字段)
        || !写入节点字段(配对, 配对条件字段, 条件, 已写字段)
        || !写入节点字段(配对, 配对结果字段, 结果, 已写字段)
        || !写入I64字段(结果, 关系规则字段,
            static_cast<std::int64_t>(定义.结果.关系规则), 已写字段)
        || !写入I64字段(结果, 结果必要证据质量字段,
            static_cast<std::int64_t>(
                定义.结果.确认要求.必要证据质量), 已写字段)
        || !写入I64字段(结果, 结果多证据关系字段,
            static_cast<std::int64_t>(
                定义.结果.确认要求.多证据关系), 已写字段)
        || !写入I64字段(结果, 结果禁止关系规则字段,
            static_cast<std::int64_t>(
                定义.结果.确认要求.禁止结果关系规则), 已写字段)) {
        return false;
    }
    if (定义.结果.确认要求.最大等待纳秒
        && !写入I64字段(结果, 结果最大等待纳秒字段,
            *定义.结果.确认要求.最大等待纳秒, 已写字段)) {
        return false;
    }
    if (!定义.线性关系组.empty()) {
        auto 关系组 = 定义.线性关系组;
        std::sort(关系组.begin(), 关系组.end(), 线性关系位置小于);
        std::vector<std::int64_t> 数据;
        for (const auto& 关系 : 关系组) {
            数据.insert(数据.end(), {static_cast<std::int64_t>(关系.条件序号),
                static_cast<std::int64_t>(关系.结果序号), static_cast<std::int64_t>(关系.方向),
                关系.原条件域.下界, 关系.原条件域.上界, 关系.原结果域.下界, 关系.原结果域.上界});
        }
        已写字段.reserve(已写字段.size() + 1);
        const auto 字段 = 全局基础数据集.添加字段值(配对, 配对线性关系组字段,
            基础值{基础原始值{std::move(数据)}});
        if (!有效(字段)) return false;
        已写字段.push_back(字段);
    }
    if (定义.结果.确认要求.等待条件概念节点
        && !写入节点字段(结果, 结果等待条件概念字段,
            *定义.结果.确认要求.等待条件概念节点, 已写字段)) {
        return false;
    }
    for (std::size_t i = 0; i < 定义.结果.结果项组.size(); ++i) {
        if (!写入结果项(结果, 结果项字段, 定义.结果.结果项组[i],
                static_cast<std::int64_t>(i), 已建节点, 已写字段)) {
            return false;
        }
    }
    for (std::size_t i = 0;
         i < 定义.结果.确认要求.禁止结果项组.size(); ++i) {
        if (!写入结果项(结果, 结果禁止项字段,
                定义.结果.确认要求.禁止结果项组[i],
                static_cast<std::int64_t>(i), 已建节点, 已写字段)) {
            return false;
        }
    }
    if(!写入参数关系(结果,定义.结果.参数关系组,已建节点,已写字段)) return false;
    for(std::size_t i=0;i<定义.结果.场景事实限定组.size();++i) {
        const auto& r=定义.结果.场景事实限定组[i];
        const auto n=新建类型化节点(结果场景限定节点类型,已建节点,已写字段);
        if(!有效(n)||!写入I64字段(n,顺序字段,static_cast<std::int64_t>(i),已写字段)
            ||!写入节点字段(n,结果场景所属字段,结果,已写字段)
            ||!写入节点字段(n,结果场景关系概念字段,r.关系概念节点,已写字段))return false;
        已写字段.reserve(已写字段.size()+1);
        const auto field=全局基础数据集.添加字段值(n,结果场景四位置字段,基础值{std::vector<std::int64_t>{r.场景参数序号,r.主体参数序号,r.对象参数序号,r.特征参数序号}});
        if(!有效(field))return false;已写字段.push_back(field);
        if(!写入节点字段(结果,结果场景限定字段,n,已写字段))return false;
    }
    for(const auto B:定义.来源执行分支节点组)
        if(!写入节点字段(配对,配对分支来源字段,B,已写字段))return false;
    if (未发布配对) {
        *未发布配对 = 配对;
        return true;
    }
    return 写入节点字段(方法节点, 方法条件结果字段, 配对, 已写字段);
}

bool 包含全部(
    const std::vector<稳定编码>& 候选,
    const std::vector<稳定编码>& 要求) {
    return std::all_of(要求.begin(), 要求.end(), [&](稳定编码 节点) {
        return std::find(候选.begin(), 候选.end(), 节点) != 候选.end();
    });
}

bool 特征概念为或从属于(
    稳定编码 实际,
    稳定编码 要求,
    const 概念_特征类& 服务) {
    if (实际 == 要求) return true;
    std::vector<稳定编码> 待查{实际};
    std::set<稳定编码> 已查;
    while (!待查.empty()) {
        const auto 当前 = 待查.back();
        待查.pop_back();
        if (!已查.insert(当前).second) continue;
        for (const auto& 关系 : 全局基础数据集.查询目标关系(
                 当前, 基础外部关系类型::父子)) {
            if (关系.源节点 == 要求) return true;
            if (服务.是特征概念节点(关系.源节点)) {
                待查.push_back(关系.源节点);
            }
        }
    }
    return false;
}


// 调用方已经持有方法结构锁。完整扫描，任何破损都不能解释为未登记。
bool 是有序服务对象规格(const std::vector<新方法参数规格>& 组) {
    return 组.size() == 2 && 组[0].有序规格 && 组[1].有序规格
        && 组[0].有序规格->序号 == 1 && 组[1].有序规格->序号 == 2
        && 组[0].有序规格->信息类型 == 新方法参数信息类型::存在
        && 组[1].有序规格->信息类型 == 新方法参数信息类型::特征;
}

bool 是服务对象空头前代(const 新方法信息& 旧, const std::vector<新方法参数规格>& 新规格) {
    if (!是有序服务对象规格(新规格) || 旧.参数规格组.size() != 1
        || !旧.条件结果组.empty() || !旧.组合步骤组.empty() || 旧.组合入口步骤节点
        || 旧.来源方法节点 || 旧.实例根节点 || 旧.父代方法节点
        || 旧.作用域 != 新方法作用域::普通世界 || 旧.实现方式 != 新方法实现方式::本能)
        return false;
    新方法参数规格 预期;
    预期.参数角色节点 = 新规格[0].有序规格->范围概念节点;
    预期.存在概念节点 = 预期.参数角色节点;
    预期.要求场景 = true;
    const auto FT = 新规格[1].有序规格->范围概念节点;
    预期.特征规格组.push_back({FT, FT, {}});
    return 旧.参数规格组.front() == 预期;
}

std::vector<稳定编码> 读取旧网页退役成员(const 概念_特征类& 概念服务) {
    if (!节点仍存在(方法旧网页退役成员字段))
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"核验旧网页方法退役结构", L"退役引用或服务对象方法族拓扑不完整，保留历史材料。");
    auto 组 = 读取多节点字段(新方法组织根节点, 方法旧网页退役成员字段, 概念服务);
    if (!组)
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"核验旧网页方法退役结构", L"退役引用或服务对象方法族拓扑不完整，保留历史材料。");
    return std::move(*组);
}

const 新方法信息& 核验服务对象方法族(const std::vector<新方法信息>& 组) {
    if (组.empty() || 组.size() > 2) 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"核验旧网页方法退役结构", L"退役引用或服务对象方法族拓扑不完整，保留历史材料。");
    for (const auto& 项 : 组)
        if (项.来源方法节点 || 项.实例根节点 || 项.实现方式 != 新方法实现方式::本能
            || 项.作用域 != 新方法作用域::普通世界
            || 项.本能函数登记名 != 新本能函数登记名_旧两参数获取服务对象)
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"核验旧网页方法退役结构", L"退役引用或服务对象方法族拓扑不完整，保留历史材料。");
    if (组.size() == 1) {
        if (组[0].父代方法节点) 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"核验旧网页方法退役结构", L"退役引用或服务对象方法族拓扑不完整，保留历史材料。");
        return 组[0];
    }
    const 新方法信息* 末代 = nullptr;
    const 新方法信息* 前代 = nullptr;
    for (const auto& 项 : 组) {
        if (项.父代方法节点) {
            if (末代) 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"核验旧网页方法退役结构", L"退役引用或服务对象方法族拓扑不完整，保留历史材料。");
            末代 = &项;
        } else {
            if (前代) 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"核验旧网页方法退役结构", L"退役引用或服务对象方法族拓扑不完整，保留历史材料。");
            前代 = &项;
        }
    }
    if (!末代 || !前代 || 末代->父代方法节点 != 前代->节点
        || 前代->启用状态 != 新方法启用状态::停用
        || !是服务对象空头前代(*前代, 末代->参数规格组)
        || 末代->方法头 != 前代->方法头)
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"核验旧网页方法退役结构", L"退役引用或服务对象方法族拓扑不完整，保留历史材料。");
    return *末代;
}

新方法建立结果 查询本能登记结构(const std::string& 登记名, const 概念_特征类& 概念服务,
    const 新方法治理对象读取接口& 治理读取, const 概念_存在类& 存在概念服务) {
    新方法建立结果 结果;
    if (!结构仍存在()) {
        结果.状态 = 新方法操作状态::尚未初始化;
        return 结果;
    }
    const auto 成员组 = 读取多节点字段(新方法组织根节点, 方法成员字段, 概念服务);
    if (!成员组) {
        结果.状态 = 新方法操作状态::结构不一致;
        return 结果;
    }
    std::vector<新方法信息> 同名;
    for (const auto 节点 : *成员组) {
        if (!节点仍存在(节点)) return {新方法操作状态::结构不一致, std::nullopt};
        // 登记名是本次查询范围；无关方法不再为查询另一个登记反复完整解码。
        const auto 登记字段 = 全局基础数据集.查询字段(节点, 方法本能函数登记名字段);
        if (登记字段.empty()) continue;
        const auto 名称 = 读取唯一字符串字段(节点, 方法本能函数登记名字段);
        if (!名称 || 名称->empty()) return {新方法操作状态::结构不一致, std::nullopt};
        if (*名称 != 登记名) continue;
        const auto 方法 = 读取方法信息本体(节点, 概念服务, 治理读取, 存在概念服务, false, true);
        if (!方法) {
            return {新方法操作状态::结构不一致, std::nullopt};
        }
        if (方法->来源方法节点 || 方法->本能函数登记名 != 登记名) continue;
        if (登记名 == 新本能函数登记名_旧两参数获取服务对象) {
            同名.push_back(*方法); continue;
        }
        if (结果.方法节点) {
            return {新方法操作状态::结构不一致, std::nullopt};
        }
        结果.方法节点 = 节点;
    }
    if (登记名 == 新本能函数登记名_旧两参数获取服务对象 && !同名.empty()) {
        const auto 停止 = []() { 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__,
            L"查询获取服务对象登记", L"获取服务对象方法代际或启用身份不完整，保留历史材料。"); };
        const auto& 当前 = 核验服务对象方法族(同名);
        const auto 退役 = 读取旧网页退役成员(概念服务);
        const auto 已退役 = [&](稳定编码 n) { return std::find(退役.begin(), 退役.end(), n) != 退役.end(); };
        const bool 全族退役 = std::all_of(同名.begin(), 同名.end(), [&](const auto& m) {
            return 已退役(m.节点) && m.启用状态 == 新方法启用状态::停用;
        });
        if (当前.启用状态 == 新方法启用状态::停用) {
            if (!全族退役) 停止();
            return {新方法操作状态::登记已退役, 当前.节点};
        }
        if (std::any_of(同名.begin(), 同名.end(), [&](const auto& m) { return 已退役(m.节点); })) 停止();
        结果.方法节点 = 当前.节点;
    }
    结果.状态 = 结果.方法节点 ? 新方法操作状态::已找到
        : 新方法操作状态::方法不存在;
    return 结果;
}

std::int64_t 方法当前UTC纳秒() {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
}

// 联合追加所读取的全部方法图、字段键和直接引用均有完整CAS预期。
基础封存追加片段 封存方法片段(基础节点组追加请求 请求,
    const std::vector<稳定编码>& 图, const 概念_存在类& 概念,const 概念_特征类& 特征概念) {
    std::set<稳定编码> 身份{新方法组织根节点};
    std::set<稳定编码> 内容保护身份;
    // 变化分类（2026-11-08/09用户裁决）：本次写入的方法图、追加字段宿主及调用者已严格预期的节点按严格快照预期；
    // 仅被引用的节点（概念、字段定义等）按容许变化预期——他处引用增删不阻止发布，后读者为准；其本体或原关系
    // 在两次读取间改变属并发变化，报 引用内容定义前提变化 交调用者重来，不当作工程错误。
    std::set<稳定编码> 严格身份(图.begin(), 图.end());
    for (const auto& v : 请求.预期联合节点组) 严格身份.insert(v.结构.节点.编码);
    for (const auto& f : 请求.追加字段组) 严格身份.insert(f.所属节点);
    std::set<稳定编码> 读取组(严格身份);
    // 字段替换/删除与外部边要求宿主及旧目标具备严格联合预期（基础层），含这些写入的请求保持全部严格。
    const bool 全部严格 = !请求.替换字段组.empty() || !请求.删除字段关系组.empty() || !请求.追加外部关系组.empty();
    // 先收集全部读取快照与身份，读取组完整后再逐份并入（否则组内新增关系可能因另一端尚未登记而被误放行）。
    struct 待并入项 { 基础联合节点快照 快照; bool 内容; const char* 说明; };
    std::vector<待并入项> 待并入;
    const auto 并入预期 = [&](const 基础联合节点快照& p, bool 内容, const char* 严格变化说明) {
        读取组.insert(p.结构.节点.编码);
        待并入.push_back({p, 内容, 严格变化说明});
    };
    const auto 实际并入 = [&](const 基础联合节点快照& p, bool 内容, const char* 严格变化说明) {
        const auto 码 = p.结构.节点.编码;
        if (全部严格 || 严格身份.contains(码)) {
            const auto old = std::find_if(请求.预期联合节点组.begin(), 请求.预期联合节点组.end(),
                [&](const auto& v) { return v.结构.节点 == p.结构.节点; });
            if (old == 请求.预期联合节点组.end()) 请求.预期联合节点组.push_back(p);
            else if (old->结构.字段组 != p.结构.字段组 || old->入向字段组 != p.入向字段组
                || old->入向外部关系组 != p.入向外部关系组 || old->出向外部关系组 != p.出向外部关系组) {
                if (内容) throw 引用内容定义前提变化{};
                throw std::runtime_error(严格变化说明);
            }
            return;
        }
        // 仅被引用：存在性以只读节点预期保证（删除即预期不符），内容以容许变化预期保证。
        if (std::none_of(请求.只读预期.节点组.begin(), 请求.只读预期.节点组.end(),
                [&](const auto& n) { return n.编码 == 码; }))
            请求.只读预期.节点组.push_back(基础节点{码});
        const auto old = std::find_if(请求.容许新增入向预期组.begin(), 请求.容许新增入向预期组.end(),
            [&](const auto& v) { return v.结构.节点 == p.结构.节点; });
        if (old == 请求.容许新增入向预期组.end()) 请求.容许新增入向预期组.push_back(p);
        else if (联合快照容许变化判定(p, *old, 读取组)) *old = p;
        else throw 引用内容定义前提变化{};
    };
    const auto 加类别保护 = [&](稳定编码 C) {
        const auto p = 解析参数类别概念保护(C, 概念, 特征概念);
        for (const auto& n : p.完整保护组) {
            身份.insert(n.结构.节点.编码);
            内容保护身份.insert(n.结构.节点.编码);
            并入预期(n, true, "method category protection changed");
        }
    };
    const auto 加内容保护=[&](稳定编码 C,std::optional<稳定编码> 路径FT) {
        新方法引用内容条件 条件;条件.要求概念节点=C;
        if(路径FT)条件.正式内容路径.push_back(*路径FT);
        std::vector<基础联合节点快照> 保护;
        if(!读取引用内容条件定义保护(条件,特征概念,概念,保护))
            throw std::runtime_error("method content requirement full semantics unavailable");
        for(const auto& 快照:保护) {
            身份.insert(快照.结构.节点.编码);
            内容保护身份.insert(快照.结构.节点.编码);
            并入预期(快照,true,"method content protection changed");
        }
    };
    const auto 加正式保护=[&](const std::vector<基础联合节点快照>& 保护) {
        for(const auto& p:保护) {
            身份.insert(p.结构.节点.编码);内容保护身份.insert(p.结构.节点.编码);
            并入预期(p,true,"method formal protection changed");
        }
    };
    const auto 加主体保护=[&](稳定编码 FT) {
        std::vector<基础联合节点快照> p;
        if(!追加需求主体路径保护(FT,特征概念,概念,p))throw std::runtime_error("method subject relation path unsupported");
        加正式保护(p);
    };
    const auto 加完整需求保护=[&](稳定编码 C) {
        const auto p=概念.读取引用内容筹办概念();
        if(p.状态==正式结构操作状态::前提已变化)throw 引用内容定义前提变化{};
        if(p.状态!=正式结构操作状态::已读取||!p.信息||p.完整保护组.empty()
            ||p.信息->完整被认知需求!=C)throw std::runtime_error("method content result complete concept unavailable");
        加正式保护(p.完整保护组);
    };
    身份.insert(图.begin(), 图.end());
    for (const auto& f : 请求.追加字段组) {
        身份.insert(f.所属节点); 身份.insert(f.字段节点);
        if (const auto* n = std::get_if<稳定编码>(&f.内容)) 身份.insert(*n);
    }
    for(const auto& n:请求.新节点组)for(const auto& f:n.字段组) {
        身份.insert(f.字段节点);
        if(const auto* ref=std::get_if<稳定编码>(&f.内容))身份.insert(*ref);
        if (f.字段节点 == 有序参数类别概念字段) {
            const auto* c = std::get_if<稳定编码>(&f.内容);
            if (!c) throw std::runtime_error("method parameter category is not a reference");
            加类别保护(*c);
        }
        if(f.字段节点==内容关系路径字段) {
            const auto* ft=std::get_if<稳定编码>(&f.内容);
            if(!ft)throw std::runtime_error("method content relation FT is not a reference");
            加主体保护(*ft);
        }
        if(f.字段节点==条件引用内容要求字段||f.字段节点==结果引用内容要求字段) {
            const auto* C=std::get_if<稳定编码>(&f.内容);
            if(!C)throw std::runtime_error("method content requirement is not a reference");
            std::optional<稳定编码> 路径FT;
            const auto 路径键=f.字段节点==条件引用内容要求字段 ? 条件引用内容项字段 : 结果引用内容项字段;
            for(const auto& 路径:n.字段组)if(路径.字段节点==路径键) {
                const auto* FT=std::get_if<稳定编码>(&路径.内容);
                if(!FT||路径FT)throw std::runtime_error("method content path is not a unique reference");
                路径FT=*FT;
            }
            加内容保护(*C,路径FT);
            if(f.字段节点==结果引用内容要求字段)加完整需求保护(*C);
            if(f.字段节点==条件引用内容要求字段)for(const auto& 限定:n.字段组)if(限定.字段节点==条件内容认知有效字段) {
                const auto* raw=std::get_if<基础值>(&限定.内容);
                const auto* value=raw ? std::get_if<std::int64_t>(&raw->材料) : nullptr;
                if(!value||(*value!=0&&*value!=1))throw std::runtime_error("method provenance condition flag malformed");
                if(*value==1)加完整需求保护(*C);
            }
        }
    }
    for (const auto n : 图) {
        const auto r = 全局基础数据集.查询节点组完整结构({n});
        if (r.状态 == 基础结构操作状态::资源失败) throw std::bad_alloc{};
        if (r.状态 != 基础结构操作状态::已完成 || r.节点组.size() != 1 || !r.节点组[0].快照)
            throw std::runtime_error("method graph snapshot unavailable");
        for (const auto& f : r.节点组[0].快照->结构.字段组) {
            身份.insert(f.字段节点);
            if (const auto* ref = std::get_if<稳定编码>(&f.内容)) 身份.insert(*ref);
            if (f.字段节点 == 有序参数类别概念字段) {
                const auto* c = std::get_if<稳定编码>(&f.内容);
                if (!c) throw std::runtime_error("method parameter category is not a reference");
                加类别保护(*c);
            }
            if(f.字段节点==内容关系路径字段) {
                const auto* ft=std::get_if<稳定编码>(&f.内容);
                if(!ft)throw std::runtime_error("method content relation FT is not a reference");
                加主体保护(*ft);
            }
            if(f.字段节点==条件引用内容要求字段||f.字段节点==结果引用内容要求字段) {
                const auto* C=std::get_if<稳定编码>(&f.内容);
                if(!C)throw std::runtime_error("method content requirement is not a reference");
                const auto 路径=读取可选节点字段(n,f.字段节点==条件引用内容要求字段 ? 条件引用内容项字段 : 结果引用内容项字段);
                if(!路径)throw std::runtime_error("method content path field damaged");
                加内容保护(*C, 有效(*路径) ? std::optional{*路径} : std::nullopt);
                if(f.字段节点==结果引用内容要求字段)加完整需求保护(*C);
                if(f.字段节点==条件引用内容要求字段) {
                    const auto 限定=读取可选I64字段(n,条件内容认知有效字段);
                    if(!限定||(*限定&&**限定!=0&&**限定!=1))throw std::runtime_error("method provenance condition flag malformed");
                    if(*限定&&**限定==1)加完整需求保护(*C);
                }
            }
            if(f.字段节点==初始关系概念字段||f.字段节点==结果场景关系概念字段) {
                const auto* ref=std::get_if<稳定编码>(&f.内容);
                const auto proof=概念.读取已确认消息连接概念保护();
                if(!ref||!proof.信息||proof.信息->节点!=*ref||proof.完整保护组.empty())
                    throw std::runtime_error("method scene condition formal protection unavailable");
                for(const auto& snapshot:proof.完整保护组) {
                    身份.insert(snapshot.结构.节点.编码);
                    并入预期(snapshot,false,"method scene condition protection changed");
                }
            }
        }
    }
    const auto r = 全局基础数据集.查询节点组完整结构({身份.begin(), 身份.end()});
    if (r.状态 == 基础结构操作状态::资源失败) throw std::bad_alloc{};
    if (r.状态 != 基础结构操作状态::已完成 || r.节点组.size() != 身份.size())
        throw std::runtime_error("method publication snapshot unavailable");
    for (const auto& n : r.节点组) {
        if (!n.快照) throw std::runtime_error("method publication incomplete snapshot");
        并入预期(*n.快照, 内容保护身份.contains(n.快照->结构.节点.编码), "method publication formal protection changed");
    }
    读取组.insert(身份.begin(), 身份.end());
    for (const auto& 项 : 待并入) 实际并入(项.快照, 项.内容, 项.说明);
    auto sealed = 全局基础数据集.封存原子追加片段(请求);
    if (sealed.状态 == 基础节点组追加状态::资源失败) throw std::bad_alloc{};
    if (sealed.状态 != 基础节点组追加状态::已完成 || !sealed.片段)
        throw std::runtime_error("method publication seal failed");
    return *sealed.片段;
}

void 追加方法虚拟归属(基础节点组追加请求& 请求, const 新方法虚拟存在读回& v) {
    请求.追加字段组.push_back({v.引用.虚拟存在节点, 虚拟存在启用特征字段, v.引用.启用特征节点});
    请求.追加字段组.push_back({v.引用.虚拟存在节点, 虚拟存在专属概念字段, v.专属存在概念节点});
}

struct 初始声明补齐候选 final {
    新方法信息 原, 预期;
    基础节点组追加请求 追加;
    std::vector<新方法执行分支定义> 新分支定义;
    bool 有变化=false;
};

新方法操作状态 核验初始声明补齐(const 新方法信息& 原,
    const 新方法知识建立请求& 原声明, const 概念_特征类& 特征概念,
    const 概念_存在类& 存在概念, 初始声明补齐候选& 候选) {
    auto 声明=原声明;规范内容关系声明(声明);
    候选.原=原; 候选.预期=原;
    if(原.初始必要条件) {
        const auto root=读取唯一节点字段(原.方法虚拟存在节点,初始必要条件字段);
        const auto format=root?读取唯一I64字段(*root,初始声明格式字段):std::nullopt;
        if(!format||(*format==1&&!初始必要条件形状有效(*原.初始必要条件))
            ||((*format==2||*format==3)&&!通用概念必要声明形状有效(*原.初始必要条件))
            ||(*format!=1&&*format!=2&&*format!=3))
            throw std::runtime_error("existing initial requirement format or shape damaged");
        // 格式1仍由原专用入口完成第二段完整核验；普通省略请求只读保留。
        const bool 旧专用省略=*format==1&&!声明.初始必要条件;
        if(!旧专用省略&&原.初始必要条件!=声明.初始必要条件)return 新方法操作状态::函数登记冲突;
    }
    const auto 原分支=分支定义组(原);
    const auto 可补=[](const auto& 旧, const auto& 新) { return 旧.empty() || 旧==新; };
    // 登记结果组（R110）：旧方法无结果组时补齐；已有时须与声明相同。结果组不受已有配对限制。
    if(!可补(原.登记结果组,声明.登记结果组))return 新方法操作状态::函数登记冲突;
    if(!登记结果组形状有效(声明.登记结果组,原.参数规格组,特征概念))return 新方法操作状态::参数规格不合法;
    const bool 补结果组=原.登记结果组.empty()&&!声明.登记结果组.empty();
    if (!可补(原分支,声明.执行分支组)
        || !可补(原.初始模拟关系组,声明.初始模拟关系组)
        || !可补(原.初始模拟事件参数组,声明.初始模拟事件参数组)
        || !可补(原.初始模拟输入方案组,声明.初始模拟输入方案组)
        || (原.行为 && 原.行为!=声明.行为)) return 新方法操作状态::函数登记冲突;
    if (声明.行为 && (!是方法行为(static_cast<std::int64_t>(*声明.行为))
        || std::any_of(原.条件结果组.begin(),原.条件结果组.end(),[&](const auto& p) {
            return !行为结果相容(声明.行为,p.结果);
        }))) return 新方法操作状态::函数登记冲突;
    if (!参数关系结构有效(声明.初始模拟关系组)) return 新方法操作状态::参数规格不合法;
    if(!模拟输入方案有效(声明.初始模拟输入方案组,原.参数规格组,声明.初始模拟关系组,特征概念))
        return 新方法操作状态::参数规格不合法;
    std::vector<基础联合节点快照> 必要保护;
    const auto 必要核验=核验通用概念必要声明(声明,存在概念,特征概念,必要保护);
    if(必要核验!=新方法操作状态::已找到)return 必要核验;
    新方法条件结果定义 关系核验;
    关系核验.条件.参数关系组=声明.初始模拟关系组;
    if (!条件结果角色已声明(原.参数规格组,关系核验,特征概念,存在概念))
        return 新方法操作状态::参数规格不合法;
    for (std::size_t i=0;i<声明.初始模拟事件参数组.size();++i) {
        const auto n=声明.初始模拟事件参数组[i];
        if (!n || n>原.参数规格组.size() || !原.参数规格组[n-1].有序规格
            || 原.参数规格组[n-1].有序规格->信息类型!=新方法参数信息类型::特征
            || std::find(声明.初始模拟事件参数组.begin(),声明.初始模拟事件参数组.begin()+i,n)
                !=声明.初始模拟事件参数组.begin()+i) return 新方法操作状态::参数规格不合法;
    }
    for (std::size_t i=0;i<声明.执行分支组.size();++i)
        if (声明.执行分支组[i].顺序!=i
            || !分支条件有效(声明.执行分支组[i].必要条件,原.参数规格组,特征概念,存在概念))
            return 新方法操作状态::参数规格不合法;
    const bool 补关系=原.初始模拟关系组.empty()&&!声明.初始模拟关系组.empty();
    const bool 补事件=原.初始模拟事件参数组.empty()&&!声明.初始模拟事件参数组.empty();
    const bool 补分支=原分支.empty()&&!声明.执行分支组.empty();
    const bool 补输入=原.初始模拟输入方案组.empty()&&!声明.初始模拟输入方案组.empty();
    const bool 补必要=!原.初始必要条件&&声明.初始必要条件;
    if ((补关系||补事件||补分支||补输入||补必要) && (!原.条件结果组.empty()||!声明.条件结果组.empty()))
        return 新方法操作状态::函数登记冲突;
    auto& 请求=候选.追加;
    请求.预期联合节点组.insert(请求.预期联合节点组.end(),必要保护.begin(),必要保护.end());
    if(补必要) {
        追加通用概念必要声明(请求,原.方法虚拟存在节点,*声明.初始必要条件);
        候选.预期.初始必要条件=声明.初始必要条件;
    }
    if(补结果组) {
        追加登记结果组(请求,原.方法虚拟存在节点,声明.登记结果组);
        候选.预期.登记结果组=声明.登记结果组;
    }
    if(补输入) {
        追加模拟输入方案(请求,原.方法虚拟存在节点,声明.初始模拟输入方案组);
        候选.预期.初始模拟输入方案组=声明.初始模拟输入方案组;
    }
    const auto 字段=[&](std::size_t n,稳定编码 key,基础组内字段内容 value) {
        请求.新节点组[n].字段组.push_back({key,std::move(value)});
    };
    const auto 新节点=[&](稳定编码 类型) {
        const auto n=请求.新节点组.size(); 请求.新节点组.emplace_back();
        字段(n,节点类型字段,类型); return n;
    };
    const auto 编关系=[](const std::vector<新方法参数关系>& r) {
        std::vector<std::int64_t> a;
        for (const auto& x:r)if(x.种类!=新方法参数关系种类::内容引用参数)
            a.insert(a.end(),{static_cast<std::int64_t>(x.种类),x.左参数序号,x.右参数序号});
        return a;
    };
    const auto 追加内容关系=[&](基础组内字段内容 host,const std::vector<新方法参数关系>& rs) {
        for(const auto& r:rs)if(r.种类==新方法参数关系种类::内容引用参数) {
            const auto child=新节点(内容关系节点类型);
            字段(child,内容关系所属字段,host);
            字段(child,内容关系左字段,基础值{static_cast<std::int64_t>(r.左参数序号)});
            字段(child,内容关系右字段,基础值{static_cast<std::int64_t>(r.右参数序号)});
            字段(child,内容关系路径字段,r.左正式内容路径.front());
            if(const auto* ref=std::get_if<基础组内节点引用>(&host))字段(ref->索引,内容关系成员字段,基础组内节点引用{child});
            else if(const auto* ref=std::get_if<稳定编码>(&host))请求.追加字段组.push_back({*ref,内容关系成员字段,基础组内节点引用{child}});
            else throw std::runtime_error("method content relation owner invalid");
        }
    };
    if (补关系) {
        const auto old=编关系(声明.初始模拟关系组);
        if(!old.empty())请求.追加字段组.push_back({原.方法虚拟存在节点,参数位置关系字段,基础值{old}});
        追加内容关系(原.方法虚拟存在节点,声明.初始模拟关系组);
        候选.预期.初始模拟关系组=声明.初始模拟关系组;
    }
    if (补事件) {
        请求.追加字段组.push_back({原.方法虚拟存在节点,模拟事件位置字段,
            基础值{std::vector<std::int64_t>(声明.初始模拟事件参数组.begin(),声明.初始模拟事件参数组.end())}});
        候选.预期.初始模拟事件参数组=声明.初始模拟事件参数组;
    }
    if (补分支) {
        候选.新分支定义=声明.执行分支组;
        for (const auto& b:声明.执行分支组) {
            const auto B=新节点(分支节点类型), C=新节点(条件节点类型);
            字段(B,顺序字段,基础值{static_cast<std::int64_t>(b.顺序)});
            字段(B,分支所属方法字段,原.节点); 字段(B,分支所属虚拟字段,原.方法虚拟存在节点);
            字段(B,分支条件字段,基础组内节点引用{C});
            请求.追加字段组.push_back({原.方法虚拟存在节点,执行分支字段,基础组内节点引用{B}});
            const auto& c=b.必要条件;
            字段(C,关系规则字段,基础值{static_cast<std::int64_t>(c.关系规则)});
            const auto old=编关系(c.参数关系组);
            if(!old.empty())字段(C,参数位置关系字段,基础值{old});
            追加内容关系(基础组内节点引用{C},c.参数关系组);
            for (std::size_t i=0;i<c.条件项组.size();++i) {
                const auto& x=c.条件项组[i]; const auto n=新节点(条件项节点类型);
                字段(n,顺序字段,基础值{static_cast<std::int64_t>(i)});
                if (x.参数序号) 字段(n,有序参数序号字段,基础值{static_cast<std::int64_t>(x.参数序号)});
                else 字段(n,参数角色字段,x.参数角色节点);
                const std::pair<const std::optional<稳定编码>*,稳定编码> refs[]{
                    {&x.存在概念节点,条件项存在概念字段},{&x.特征类型概念节点,条件项特征类型字段},
                    {&x.特征值域概念节点,条件项特征值域字段},{&x.二次特征概念节点,条件项二次特征字段},
                    {&x.状态或动态概念节点,条件项状态动态字段}};
                for (const auto& [v,key]:refs) if (*v) 字段(n,key,**v);
                if (x.精确数值域) 字段(n,项精确数值域字段,基础值{std::vector<std::int64_t>{x.精确数值域->下界,x.精确数值域->上界}});
                if (x.任务成员变量序号) 字段(n,任务成员变量字段,基础值{static_cast<std::int64_t>(x.任务成员变量序号)});
                if(x.引用内容条件) {
                    字段(n,条件引用内容要求字段,x.引用内容条件->要求概念节点);
                    if(x.引用内容条件->作用点!=新方法内容作用点::特征当前引用)
                        字段(n,条件内容作用点字段,基础值{static_cast<std::int64_t>(x.引用内容条件->作用点)});
                    if(x.引用内容条件->要求认知来源当前有效)字段(n,条件内容认知有效字段,基础值{std::int64_t{1}});
                    if(!x.引用内容条件->正式内容路径.empty())字段(n,条件引用内容项字段,x.引用内容条件->正式内容路径.front());
                }
                if(x.特征前态限定) {
                    字段(n,条件前态当前字段,基础值{static_cast<std::int64_t>(x.特征前态限定->当前值状态)});
                    if(x.特征前态限定->历史不同值数量)
                        字段(n,条件前态历史数字段,基础值{static_cast<std::int64_t>(*x.特征前态限定->历史不同值数量)});
                }
                字段(C,条件项字段,基础组内节点引用{n});
            }
            for (std::size_t i=0;i<c.场景事实限定组.size();++i) {
                const auto& r=c.场景事实限定组[i]; const auto n=新节点(初始场景限定节点类型);
                字段(n,顺序字段,基础值{static_cast<std::int64_t>(i)});
                字段(n,初始声明所属字段,基础组内节点引用{C}); 字段(n,初始关系概念字段,r.关系概念节点);
                字段(n,初始四位置字段,基础值{std::vector<std::int64_t>{r.场景参数序号,r.主体参数序号,r.对象参数序号,r.特征参数序号}});
                字段(C,初始场景限定字段,基础组内节点引用{n});
            }
        }
    }
    if (!原.行为 && 声明.行为) {
        请求.追加字段组.push_back({原.节点,方法行为字段,基础值{static_cast<std::int64_t>(*声明.行为)}});
        候选.预期.行为=声明.行为;
    }
    候选.有变化=!请求.追加字段组.empty();
    // 捕获所有新节点字段键及稳定引用端点；组内引用只在同次原子追加中解析。
    std::set<稳定编码> 端点;
    const auto 收字段=[&](稳定编码 key,const 基础组内字段内容& value) {
        端点.insert(key); if (const auto* n=std::get_if<稳定编码>(&value)) 端点.insert(*n);
    };
    for (const auto& n:请求.新节点组) for (const auto& f:n.字段组) 收字段(f.字段节点,f.内容);
    for (const auto& f:请求.追加字段组) {端点.insert(f.所属节点);收字段(f.字段节点,f.内容);}
    const auto 读=全局基础数据集.查询节点组完整结构({端点.begin(),端点.end()});
    if (读.状态!=基础结构操作状态::已完成 || 读.节点组.size()!=端点.size())
        throw std::runtime_error("initial declaration endpoint protection read failed");
    for (const auto& p:读.节点组) {
        if (!p.快照) throw std::runtime_error("initial declaration referenced endpoint missing");
        请求.预期联合节点组.push_back(*p.快照);
    }
    const auto 加保护=[&](const auto& 保护) {请求.预期联合节点组.insert(请求.预期联合节点组.end(),保护.begin(),保护.end());};
    std::set<稳定编码> 已存在范围,已特征范围;
    const auto 存在范围=[&](auto&& self,稳定编码 n)->void {
        (void)self;
        if (!已存在范围.insert(n).second) return;
        if(!追加方法存在范围保护(n,存在概念,请求.预期联合节点组))
            throw std::runtime_error("initial declaration existence proof unavailable");
    };
    const auto 特征范围=[&](稳定编码 n) {
        if (!已特征范围.insert(n).second) return true;
        const auto p=特征概念.读取特征概念定义保护(n);
        if (p.前提已变化) return false;
        if (!p.信息 || p.预期节点组.empty()) throw std::runtime_error("initial declaration feature proof unavailable");
        加保护(p.预期节点组); return true;
    };
    const auto 关系保护=[&](const std::vector<新方法参数关系>& rs) {
        for(const auto& r:rs)if(r.种类==新方法参数关系种类::内容引用参数) {
            std::vector<基础联合节点快照> p;
            if(!追加需求主体路径保护(r.左正式内容路径.front(),特征概念,存在概念,p))return false;
            加保护(p);
        }
        return true;
    };
    if(!关系保护(声明.初始模拟关系组))return 新方法操作状态::参数规格不合法;
    for (const auto& s:原.参数规格组) if (s.有序规格) {
        加保护(读取参数类别概念保护(s.有序规格->信息类型, 存在概念, 特征概念).完整保护组);
        if (s.有序规格->信息类型==新方法参数信息类型::存在) 存在范围(存在范围,s.有序规格->范围概念节点);
        if (s.有序规格->信息类型==新方法参数信息类型::词条) {
            std::vector<基础联合节点快照> p;
            if(!追加方法词条范围保护(s.有序规格->范围概念节点,存在概念,p))return 新方法操作状态::参数规格不合法;
            加保护(p);
        }
        if (s.有序规格->信息类型==新方法参数信息类型::特征 && !特征范围(s.有序规格->范围概念节点))
            return 新方法操作状态::结构不一致;
    }
    for (const auto& b:声明.执行分支组) {
        if(!关系保护(b.必要条件.参数关系组))return 新方法操作状态::参数规格不合法;
        for (const auto& x:b.必要条件.条件项组) {
            if(x.引用内容条件) {
                std::vector<基础联合节点快照> 内容保护;
                if(!读取引用内容条件定义保护(*x.引用内容条件,特征概念,存在概念,内容保护))
                    return 新方法操作状态::结构不一致;
                加保护(内容保护);
            }
            if (x.存在概念节点) {
                const auto p=std::find_if(原.参数规格组.begin(),原.参数规格组.end(),[&](const auto& v){
                    return v.有序规格&&v.有序规格->序号==x.参数序号;});
                if(p!=原.参数规格组.end()&&p->有序规格->信息类型==新方法参数信息类型::词条) {
                    std::vector<基础联合节点快照> proof;
                    if(!追加方法词条范围保护(*x.存在概念节点,存在概念,proof))return 新方法操作状态::参数规格不合法;
                    加保护(proof);
                } else 存在范围(存在范围,*x.存在概念节点);
            }
            if (x.特征类型概念节点 && !特征范围(*x.特征类型概念节点)) return 新方法操作状态::结构不一致;
            if (x.特征值域概念节点 && !特征范围(*x.特征值域概念节点)) return 新方法操作状态::结构不一致;
        }
        for (const auto& r:b.必要条件.场景事实限定组) {
            const auto p=存在概念.读取已确认消息连接概念保护();
            if (!p.信息 || p.信息->节点!=r.关系概念节点 || p.完整保护组.empty())
                throw std::runtime_error("initial declaration scene proof unavailable");
            加保护(p.完整保护组);
        }
    }
    for(const auto& p:声明.初始模拟输入方案组)for(const auto& x:p.消息组)
        if(!特征范围(x.正文范围概念))return 新方法操作状态::结构不一致;
    // 定义保护先捕获、后复核；提交时再比较同一保护快照，避免校验后定义变化。
    if (!条件结果角色已声明(原.参数规格组,关系核验,特征概念,存在概念))
        return 新方法操作状态::结构不一致;
    if(!模拟输入方案有效(声明.初始模拟输入方案组,原.参数规格组,声明.初始模拟关系组,特征概念))
        return 新方法操作状态::结构不一致;
    for (const auto& b:声明.执行分支组)
        if (!分支条件有效(b.必要条件,原.参数规格组,特征概念,存在概念))
            return 新方法操作状态::结构不一致;
    return 新方法操作状态::已找到;
}

新方法建立结果 发布初始声明补齐(初始声明补齐候选& 候选,
    const 概念_特征类& 概念, const 新方法治理对象读取接口& 治理,
    const 概念_存在类& 存在概念) {
    std::map<稳定编码,基础联合节点快照> 合并;
    const auto 相同=[](const auto& a,const auto& b) {
        return a.结构.节点==b.结构.节点 && a.结构.字段组==b.结构.字段组 && a.入向字段组==b.入向字段组
            && a.入向外部关系组==b.入向外部关系组 && a.出向外部关系组==b.出向外部关系组;
    };
    for (const auto& p:候选.追加.预期联合节点组) {
        const auto [it,新增]=合并.emplace(p.结构.节点.编码,p);
        if (!新增 && !相同(it->second,p)) return {新方法操作状态::结构不一致,{}};
    }
    候选.追加.预期联合节点组.clear();
    std::vector<稳定编码> 身份;
    for (const auto& [n,p]:合并) {身份.push_back(n);候选.追加.预期联合节点组.push_back(p);}
    基础节点组追加状态 发布状态=基础节点组追加状态::已完成;
    {
        std::lock_guard 锁(新方法互斥);
        if (候选.有变化) 发布状态=全局基础数据集.原子追加节点组(候选.追加).状态;
        else {
            const auto r=全局基础数据集.查询节点组完整结构(身份);
            if (r.状态!=基础结构操作状态::已完成 || r.节点组.size()!=身份.size())
                throw std::runtime_error("initial declaration reuse premise read failed");
            for (std::size_t i=0;i<r.节点组.size();++i)
                if (!r.节点组[i].快照 || !相同(*r.节点组[i].快照,合并.at(身份[i])))
                    发布状态=基础节点组追加状态::预期不符;
        }
    }
    // 完整读回在方法锁外；不以单项字段已经存在为成功消歧。
    try {
    const auto 当前=读取方法信息(候选.原.节点,概念,治理,存在概念);
    const auto 完整相符=[&] {
        if (!当前) return false;
        auto 预期=候选.预期;
        if (!候选.新分支定义.empty()) {
            if (分支定义组(*当前)!=候选.新分支定义) return false;
            预期.执行分支组=当前->执行分支组;
        }
        return *当前==预期;
    };
    if (完整相符()) return {新方法操作状态::已找到,候选.原.节点};
    if (发布状态==基础节点组追加状态::预期不符) return {新方法操作状态::结构不一致,{}};
    if (发布状态==基础节点组追加状态::资源失败) return {新方法操作状态::资源失败,{}};
    工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"补齐本能初始声明",
        L"原子发布后无法完整确认声明；保留已发布事实，不转为正常拒绝或重试。");
    } catch (const 引用内容定义前提变化&) {
        return {新方法操作状态::定义前提已变化,候选.原.节点};
    } catch (...) {
        if (候选.有变化 && 发布状态!=基础节点组追加状态::预期不符
            && 发布状态!=基础节点组追加状态::资源失败)
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"补齐本能初始声明",
                L"原子发布成功后的完整读回发生工程异常；保留已发布事实。");
        throw;
    }
}

} // namespace

// 共用结构发布器；函数绑定是完整结构的一部分，根成员始终最后发布。
// 只回滚本次未发布材料；已公开后读回失败保留现场向上送出。
新方法操作状态 新_方法类::准备新方法最终发布保护(
    const 新方法知识建立请求& 请求,const 方法虚拟存在准备& 虚拟,
    基础节点组追加请求& 原子) {
    if(!原子.预期联合节点组.empty()||!原子.预期既有节点组.empty())
        throw std::logic_error("final method validation requires a fresh protection set");
    if(!行为声明有效(请求)||!是作用域(static_cast<std::int64_t>(请求.作用域))
        ||!是实现方式(static_cast<std::int64_t>(请求.实现方式)))return 新方法操作状态::方法类型不符;
    if(!方法头有效(请求.方法头))return 新方法操作状态::方法头不合法;
    if(!参数规格有效(请求.参数规格组,特征概念服务_))return 新方法操作状态::参数规格不合法;
    const auto 必要=核验通用概念必要声明(请求,存在概念服务_,特征概念服务_,原子.预期联合节点组);
    if(必要!=新方法操作状态::已找到)return 必要;
    新方法条件结果定义 关系核验;
    关系核验.条件.参数关系组=请求.初始模拟关系组;
    if(!参数关系结构有效(请求.初始模拟关系组)
        ||!条件结果角色已声明(请求.参数规格组,关系核验,特征概念服务_,存在概念服务_)
        ||!模拟输入方案有效(请求.初始模拟输入方案组,请求.参数规格组,请求.初始模拟关系组,特征概念服务_))
        return 新方法操作状态::参数规格不合法;
    for(std::size_t i=0;i<请求.初始模拟事件参数组.size();++i) {
        const auto n=请求.初始模拟事件参数组[i];
        if(!n||n>请求.参数规格组.size()||!请求.参数规格组[n-1].有序规格
            ||请求.参数规格组[n-1].有序规格->信息类型!=新方法参数信息类型::特征
            ||std::find(请求.初始模拟事件参数组.begin(),请求.初始模拟事件参数组.begin()+i,n)
                !=请求.初始模拟事件参数组.begin()+i)return 新方法操作状态::参数规格不合法;
    }
    for(std::size_t i=0;i<请求.执行分支组.size();++i)
        if(请求.执行分支组[i].顺序!=i||!分支条件有效(请求.执行分支组[i].必要条件,
            请求.参数规格组,特征概念服务_,存在概念服务_))return 新方法操作状态::参数规格不合法;
    for(const auto& pair:请求.条件结果组)
        if(!条件结果有效(pair,特征概念服务_)||!条件结果角色已声明(请求.参数规格组,pair,
            特征概念服务_,存在概念服务_))return 新方法操作状态::条件结果不合法;
    const auto feature=[&](稳定编码 C) {
        const auto proof=特征概念服务_.读取特征概念定义保护(C);
        if(!proof.信息||proof.前提已变化||proof.状态!=基础结构操作状态::已完成||proof.预期节点组.empty())
            throw std::runtime_error("final method feature definition unavailable or changed");
        原子.预期联合节点组.insert(原子.预期联合节点组.end(),proof.预期节点组.begin(),proof.预期节点组.end());
    };
    std::set<稳定编码> seen;
    const auto existence=[&](auto&& self,稳定编码 C)->void {
        (void)self;
        if(!seen.insert(C).second)return;
        if(!追加方法存在范围保护(C,存在概念服务_,原子.预期联合节点组))
            throw std::runtime_error("final method existence definition unavailable");
    };
    for(const auto& spec:请求.参数规格组) {
        if(spec.有序规格) {
            const auto 类别 = 读取参数类别概念保护(spec.有序规格->信息类型, 存在概念服务_, 特征概念服务_);
            原子.预期联合节点组.insert(原子.预期联合节点组.end(), 类别.完整保护组.begin(), 类别.完整保护组.end());
            if(spec.有序规格->信息类型==新方法参数信息类型::存在)existence(existence,spec.有序规格->范围概念节点);
            else if(spec.有序规格->信息类型==新方法参数信息类型::词条) {
                if(!追加方法词条范围保护(spec.有序规格->范围概念节点,存在概念服务_,原子.预期联合节点组))
                    throw std::runtime_error("final method word range unavailable");
            } else feature(spec.有序规格->范围概念节点);
        } else {
            if(spec.存在概念节点)existence(existence,*spec.存在概念节点);
            for(const auto& f:spec.特征规格组) {
                feature(f.特征类型概念节点);
                if(f.值域概念节点)feature(*f.值域概念节点);
            }
            if(spec.集合成员约束)feature(spec.集合成员约束->集合特征类型概念节点);
        }
    }
    for(const auto& p:请求.初始模拟输入方案组)for(const auto& x:p.消息组)feature(x.正文范围概念);
    // 对实际按定义求值的附加条件/结果域取得owner完整保护；仅作身份引用的
    // 其它信息保留原准入，不能猜其类别或把它强制解释成FT。
    const auto known=[&](稳定编码 C) {
        if(特征概念服务_.获取特征概念(C))feature(C);
        else if(存在概念服务_.获取存在概念(C))existence(existence,C);
    };
    for(const auto C:请求.方法头.作用存在概念组)known(C);
    for(const auto C:请求.方法头.涉及特征类型概念组)known(C);
    for(const auto C:请求.方法头.可能结果概念组)known(C);
    const auto scene=[&](const std::vector<新方法初始场景事实限定>& group) {
        for(const auto& item:group) {
            const auto proof=存在概念服务_.读取已确认消息连接概念保护();
            if(!proof.信息||proof.信息->节点!=item.关系概念节点||proof.完整保护组.empty())
                throw std::runtime_error("final method scene relation definition unavailable");
            原子.预期联合节点组.insert(原子.预期联合节点组.end(),proof.完整保护组.begin(),proof.完整保护组.end());
        }
    };
    const auto condition=[&](const 新方法条件定义& c) {
        for(const auto& r:c.参数关系组)if(r.种类==新方法参数关系种类::内容引用参数) {
            std::vector<基础联合节点快照> p;
            if(!追加需求主体路径保护(r.左正式内容路径.front(),特征概念服务_,存在概念服务_,p))
                throw std::runtime_error("final method subject relation unsupported");
            原子.预期联合节点组.insert(原子.预期联合节点组.end(),p.begin(),p.end());
        }
        for(const auto& item:c.条件项组) {
            if(item.引用内容条件) {
                std::vector<基础联合节点快照> 内容保护;
                if(!读取引用内容条件定义保护(*item.引用内容条件,特征概念服务_,存在概念服务_,内容保护))
                    throw std::runtime_error("final method content condition protection unavailable");
                原子.预期联合节点组.insert(原子.预期联合节点组.end(),内容保护.begin(),内容保护.end());
            }
            if(item.存在概念节点) {
                const auto p=std::find_if(请求.参数规格组.begin(),请求.参数规格组.end(),[&](const auto& v){
                    return v.有序规格&&v.有序规格->序号==item.参数序号;});
                if(p!=请求.参数规格组.end()&&p->有序规格->信息类型==新方法参数信息类型::词条) {
                    if(!追加方法配对词条范围保护(*item.存在概念节点,存在概念服务_,原子.预期联合节点组))
                        throw std::runtime_error("final method condition word range unavailable");
                } else existence(existence,*item.存在概念节点);
            }
            if(item.特征类型概念节点)feature(*item.特征类型概念节点);
            if(item.特征值域概念节点)feature(*item.特征值域概念节点);
            if(item.二次特征概念节点)known(*item.二次特征概念节点);
        }
        scene(c.场景事实限定组);
    };
    const auto resultItem=[&](const 新方法结果项定义& item) {
        if(item.引用内容结果) {
            std::vector<基础联合节点快照> p;
            if(!读取引用内容条件定义保护(*item.引用内容结果,特征概念服务_,存在概念服务_,p))
                throw std::runtime_error("final method result content protection unavailable");
            const auto root=存在概念服务_.读取引用内容筹办概念();
            if(root.状态==正式结构操作状态::前提已变化)throw 引用内容定义前提变化{};
            if(root.状态!=正式结构操作状态::已读取||!root.信息||root.完整保护组.empty()
                ||root.信息->完整被认知需求!=item.引用内容结果->要求概念节点)
                throw std::runtime_error("final method complete demand result definition unavailable");
            p.insert(p.end(),root.完整保护组.begin(),root.完整保护组.end());
            原子.预期联合节点组.insert(原子.预期联合节点组.end(),p.begin(),p.end());
        }
        if(item.特征类型概念节点)feature(*item.特征类型概念节点);
        if(item.特征值域概念节点)feature(*item.特征值域概念节点);
    };
    for(const auto& branch:请求.执行分支组)condition(branch.必要条件);
    新方法条件定义 初始关系保护;初始关系保护.参数关系组=请求.初始模拟关系组;condition(初始关系保护);
    for(const auto& pair:请求.条件结果组) {
        condition(pair.条件);
        for(const auto& item:pair.结果.结果项组)resultItem(item);
        for(const auto& item:pair.结果.确认要求.禁止结果项组)resultItem(item);
        scene(pair.结果.场景事实限定组);
        新方法条件定义 结果关系保护;结果关系保护.参数关系组=pair.结果.参数关系组;condition(结果关系保护);
    }
    核验方法私有材料未消费(虚拟);
    if(虚拟.信息.当前启用!=1||虚拟.信息.启用特征类型概念节点!=方法启用特征类型)
        throw std::runtime_error("final method private enabled feature differs");
    feature(虚拟.信息.启用特征类型概念节点);feature(虚拟.信息.当前特征概念节点);
    const auto ec=存在概念服务_.读取普通存在概念定义保护(虚拟.信息.专属存在概念节点);
    if(ec.状态!=正式概念读取状态::已读取||!ec.信息||ec.完整保护组.empty()
        ||ec.信息->对应存在节点!=虚拟.信息.引用.虚拟存在节点)
        throw std::runtime_error("final method private existence definition differs");
    原子.预期联合节点组.insert(原子.预期联合节点组.end(),ec.完整保护组.begin(),ec.完整保护组.end());
    // 保护捕获后再完整检查原规则；CAS仍复核捕获的全部定义，不对变化重试。
    if(!行为声明有效(请求)||!方法头有效(请求.方法头)||!参数规格有效(请求.参数规格组,特征概念服务_)
        ||!模拟输入方案有效(请求.初始模拟输入方案组,请求.参数规格组,请求.初始模拟关系组,特征概念服务_)
        ||!条件结果角色已声明(请求.参数规格组,关系核验,特征概念服务_,存在概念服务_))
        return 新方法操作状态::结构不一致;
    for(const auto& branch:请求.执行分支组)
        if(!分支条件有效(branch.必要条件,请求.参数规格组,特征概念服务_,存在概念服务_))return 新方法操作状态::结构不一致;
    for(const auto& pair:请求.条件结果组)
        if(!条件结果有效(pair,特征概念服务_)||!条件结果角色已声明(请求.参数规格组,pair,特征概念服务_,存在概念服务_))
            return 新方法操作状态::结构不一致;
    std::vector<基础联合节点快照> 末次必要保护;
    const auto 末核=核验通用概念必要声明(请求,存在概念服务_,特征概念服务_,末次必要保护);
    if(末核!=新方法操作状态::已找到)return 末核;
    原子.预期联合节点组.insert(原子.预期联合节点组.end(),末次必要保护.begin(),末次必要保护.end());
    std::map<稳定编码,基础联合节点快照> merged;
    for(const auto& n:原子.预期联合节点组) {
        const auto [it,added]=merged.emplace(n.结构.节点.编码,n);
        if(!added&&(it->second.结构.字段组!=n.结构.字段组||it->second.入向字段组!=n.入向字段组
            ||it->second.入向外部关系组!=n.入向外部关系组||it->second.出向外部关系组!=n.出向外部关系组))
            throw std::runtime_error("final method definition protection changed");
    }
    原子.预期联合节点组.clear();for(const auto& [id,n]:merged)原子.预期联合节点组.push_back(n);
    return 新方法操作状态::已找到;
}

新方法建立结果 新_方法类::发布方法结构(
    const 新方法知识建立请求& 原请求, const std::string* 登记名,
    const 新方法信息* 替代父代,const std::vector<基础联合节点快照>* 初始必要保护) noexcept {
    std::vector<稳定编码> 已建节点;
    std::vector<稳定编码> 已写字段;
    bool 已发布 = false;
    std::vector<方法虚拟存在准备> 虚拟组;
    新特征I64准备结果 旧更新;
    std::optional<稳定编码> 前状态;
    std::int64_t 前时间 = 0, 动作时间 = 0;
    std::optional<稳定编码> 本次发布方法;
    try {
        auto 请求=原请求;
        规范内容关系声明(请求);
        if(请求.初始必要条件&&!规范化通用概念必要声明(*请求.初始必要条件))
            return {新方法操作状态::参数规格不合法,{}};
        核验治理组织就绪();
        // 入口保护只用于预检；新M的完整保护由全部候选完成后的工厂独立取得。
        (void)初始必要保护;
        for (const auto& 参数 : 请求.参数规格组) {
            if (参数.有序规格 && 是特征持有参数(参数.有序规格->信息类型)
                && !方法持有者范围有效(*参数.有序规格,存在概念服务_))
                throw std::logic_error("ordered parameter existence concept unavailable");
        }
        虚拟组.reserve(1);
        const auto 方法节点 = 新建类型化节点(
            方法节点类型, 已建节点, 已写字段);
        if(有效(方法节点))本次发布方法=方法节点;
        const auto 头节点 = 新建类型化节点(
            方法头节点类型, 已建节点, 已写字段);
        const auto 虚拟存在 = 新建类型化节点(
            方法虚拟存在节点类型, 已建节点, 已写字段);
        bool 完整 = 有效(方法节点) && 有效(头节点) && 有效(虚拟存在)
            && 写入I64字段(方法节点, 方法作用域字段,
                static_cast<std::int64_t>(请求.作用域), 已写字段)
            && 写入I64字段(方法节点, 方法实现方式字段,
                static_cast<std::int64_t>(请求.实现方式), 已写字段)
            && 写入节点字段(方法节点, 方法头字段, 头节点, 已写字段)
            && 写入节点字段(方法节点, 方法虚拟存在字段,
                虚拟存在, 已写字段)
            && 写入节点字段(虚拟存在, 虚拟存在所属方法字段,
                方法节点, 已写字段)
            && 写入有序引用组(头节点, 方法头作用存在概念字段,
                请求.方法头.作用存在概念组, 已写字段)
            && 写入有序引用组(头节点, 方法头特征类型概念字段,
                请求.方法头.涉及特征类型概念组, 已写字段)
            && 写入有序引用组(头节点, 方法头结果概念字段,
                请求.方法头.可能结果概念组, 已写字段);
        for (std::size_t i = 0; 完整 && i < 请求.参数规格组.size(); ++i) {
            完整 = 写入参数规格(虚拟存在, 请求.参数规格组[i],
                static_cast<std::int64_t>(i), 已建节点, 已写字段, 存在概念服务_, 特征概念服务_);
        }
        if(完整) 完整=写入参数关系(虚拟存在,请求.初始模拟关系组,已建节点,已写字段)
            && 写入模拟事件位置(虚拟存在,请求.初始模拟事件参数组,已写字段);
        for(std::size_t i=0;完整&&i<请求.执行分支组.size();++i) {
            const auto& b=请求.执行分支组[i];
            if(b.顺序!=i||!分支条件有效(b.必要条件,请求.参数规格组,特征概念服务_,存在概念服务_))
                throw std::invalid_argument("method branch declaration invalid");
            const auto B=新建类型化节点(分支节点类型,已建节点,已写字段);
            const auto C=写入条件子树(b.必要条件,已建节点,已写字段);
            完整=有效(B)&&有效(C)&&写入I64字段(B,顺序字段,static_cast<std::int64_t>(i),已写字段)
                &&写入节点字段(B,分支所属方法字段,方法节点,已写字段)
                &&写入节点字段(B,分支所属虚拟字段,虚拟存在,已写字段)
                &&写入节点字段(B,分支条件字段,C,已写字段)
                &&写入节点字段(虚拟存在,执行分支字段,B,已写字段);
        }
        for (std::size_t i = 0; 完整 && i < 请求.条件结果组.size(); ++i) {
            完整 = 写入条件结果(方法节点, 请求.条件结果组[i],
                static_cast<std::int64_t>(i), 已建节点, 已写字段);
        }
        if (完整 && 登记名) {
            完整 = 写入字符串字段(方法节点, 方法本能函数登记名字段,
                *登记名, 已写字段);
        }
        if (完整 && 请求.行为)
            完整 = 写入I64字段(方法节点, 方法行为字段,
                static_cast<std::int64_t>(*请求.行为), 已写字段);
        if (完整 && 替代父代)
            完整 = 写入节点字段(方法节点, 方法父代字段, 替代父代->节点, 已写字段);
        if (!完整) throw std::runtime_error("method private graph incomplete");
        虚拟组.push_back(准备方法虚拟存在(方法节点, 虚拟存在, 1));
        基础节点组追加请求 原子;
        if(请求.初始必要条件)追加通用概念必要声明(原子,虚拟存在,*请求.初始必要条件);
        if(!登记结果组形状有效(请求.登记结果组,请求.参数规格组,特征概念服务_))
            throw std::invalid_argument("registered result group invalid");
        追加登记结果组(原子,虚拟存在,请求.登记结果组);
        if(!模拟输入方案有效(请求.初始模拟输入方案组,请求.参数规格组,请求.初始模拟关系组,特征概念服务_))
            throw std::invalid_argument("simulation input declaration invalid");
        追加模拟输入方案(原子,虚拟存在,请求.初始模拟输入方案组);
        追加方法虚拟归属(原子, 虚拟组.front().信息);
        原子.追加字段组.push_back({新方法组织根节点, 方法成员字段, 方法节点});
        if (替代父代) {
            const auto 旧 = 读取方法信息本体(替代父代->节点, 特征概念服务_, *this, 存在概念服务_, false, true);
            if (!旧 || *旧 != *替代父代 || 旧->启用状态 != 新方法启用状态::启用)
                throw std::runtime_error("method replacement source changed");
            前时间 = 方法当前UTC纳秒();
            const auto s = 状态服务_.建立状态(旧->方法虚拟存在节点, 旧->启用特征节点,
                {新状态时间语义::实例绝对UTC, 前时间});
            if (s.状态 != 新状态操作状态::已完成 || !s.状态节点)
                throw std::runtime_error("method replacement before state failed");
            前状态 = s.状态节点;
            旧更新 = 特征服务_.准备顶层I64更新(旧->启用特征节点, 0);
            if (旧更新.状态 != 新特征I64准备状态::已准备 || !旧更新.准备
                || 旧更新.准备->原当前值() != 1 || !旧更新.准备->核心有变化())
                throw std::runtime_error("method replacement feature preparation failed");
        }
        std::string 工厂异常;
        const 新方法治理片段工厂 工厂=[&]() { try {
            // 声明编码器已捕获schema/直接端点的结构保护。保留它们不变；
            // 最终语义验证使用独立空请求，不能混入入口预检或刷新旧结构保护。
            基础节点组追加请求 最终定义保护;
            const auto 核验=准备新方法最终发布保护(请求,虚拟组.front(),最终定义保护);
            if(核验!=新方法操作状态::已找到)
                throw std::runtime_error("final method declaration validation refused, status="+std::to_string(static_cast<unsigned>(核验)));
            std::map<稳定编码,基础联合节点快照> 完整保护;
            int 合并段=0;
            const auto 合并=[&](const std::vector<基础联合节点快照>& group) {
                ++合并段;
                for(const auto& n:group) {
                    const auto [it,added]=完整保护.emplace(n.结构.节点.编码,n);
                    if(!added&&(it->second.结构.字段组!=n.结构.字段组||it->second.入向字段组!=n.入向字段组
                        ||it->second.入向外部关系组!=n.入向外部关系组||it->second.出向外部关系组!=n.出向外部关系组))
                        throw std::runtime_error("final method structural and definition protection conflict node="
                            +std::to_string(n.结构.节点.编码.值)+" segment="+std::to_string(合并段)
                            +" fields="+std::to_string(it->second.结构.字段组.size())+"/"+std::to_string(n.结构.字段组.size())
                            +" in="+std::to_string(it->second.入向字段组.size())+"/"+std::to_string(n.入向字段组.size())
                            +" diff="+std::to_string(it->second.结构.字段组!=n.结构.字段组)+std::to_string(it->second.入向字段组!=n.入向字段组)
                            +std::to_string(it->second.入向外部关系组!=n.入向外部关系组)+std::to_string(it->second.出向外部关系组!=n.出向外部关系组)
                            +" head="+(请求.方法头.作用存在概念组.empty()?std::string("-"):std::to_string(请求.方法头.作用存在概念组.front().值))
                            +" params="+std::to_string(请求.参数规格组.size())+" group="+std::to_string(请求.登记结果组.size()));
                }
            };
            合并(原子.预期联合节点组);合并(最终定义保护.预期联合节点组);
            std::vector<基础联合节点快照> 合并后;
            合并后.reserve(完整保护.size());
            for(const auto& [id,n]:完整保护)合并后.push_back(n);
            原子.预期联合节点组=std::move(合并后);
            if(替代父代) {
                const auto 旧=读取方法信息本体(替代父代->节点,特征概念服务_,*this,存在概念服务_,false,true);
                if(!旧||*旧!=*替代父代||旧->启用状态!=新方法启用状态::启用
                    ||!旧更新.准备||旧更新.准备->原当前值()!=1||!旧更新.准备->核心有变化()||!前状态)
                    throw std::runtime_error("method replacement final source or preparation changed");
            }
            return 封存方法片段(std::move(原子),已建节点,存在概念服务_,特征概念服务_);
        } catch (const std::exception& e) { 工厂异常 = e.what(); throw; } catch (...) { 工厂异常 = "non-standard"; throw; } };
        动作时间 = 方法当前UTC纳秒();
        const auto 发表 = 内部治理世界_->发布新方法治理场景组({虚拟组.front().信息.引用},
            工厂, 旧更新.准备 ? &*旧更新.准备 : nullptr);
        if (发表.状态 != 新方法治理场景发布工程状态::已发布)
            throw std::runtime_error("method publication refused without publication; factory: " + 工厂异常);
        已发布 = true;
        if (替代父代) {
            const auto 完成 = 特征服务_.完成顶层I64更新(*旧更新.准备);
            if (完成.状态 != 新特征写入状态::已完成 || !完成.当前值已改变)
                throw std::runtime_error("method replacement feature finalization failed");
            特征服务_.结束顶层I64更新(*旧更新.准备);
            const 新方法虚拟存在引用 引用{替代父代->节点, 替代父代->方法虚拟存在节点,
                替代父代->启用特征节点};
            同步方法专属概念(引用);
            // METHOD-LEARN-STATE：替代父代的启用修改经方法owner统一入口形成治理动作并交付因果；
            // 联动发布可被拒绝，故学习记录与动作在联动发布确认后以发布前取得的动作时间形成。
            形成方法状态变化已加锁(引用, 新方法学习种类::启用修改, 新方法状态特征种类::启用,
                引用.启用特征节点, 1, 0, *前状态, 动作时间, {}, {方法节点});
        }
        // 新方法登记时建立配对数特征，初值取实际数量（启动补齐之后登记的方法）。
        if (有效(方法状态治理根)) 确保方法配对数特征已加锁(方法节点);
        const auto 读回 = 读取方法信息本体(方法节点, 特征概念服务_, *this, 存在概念服务_, false, true);
        if (!读回 || 读回->行为 != 请求.行为 ||读回->初始必要条件!=请求.初始必要条件
            ||读回->登记结果组!=请求.登记结果组
            ||读回->初始模拟输入方案组!=请求.初始模拟输入方案组
            || (登记名 && 读回->本能函数登记名 != *登记名)) {
            throw std::runtime_error("published method readback failed");
        }
        return {新方法操作状态::已建立, 方法节点};
    } catch (const 引用内容定义前提变化&) {
        if(!已发布) {
            if(旧更新.准备)特征服务_.取消顶层I64更新(*旧更新.准备);
            if(前状态&&状态服务_.删除状态(*前状态)!=新状态操作状态::已完成)
                工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"建立方法",L"前提变化后的未发布前状态清理失败。");
            回收本批方法图(已建节点,已写字段,虚拟组,false);
        }
        return {新方法操作状态::定义前提已变化, 已发布 ? 本次发布方法 : std::nullopt};
    } catch (...) {
        std::string 说明;
        try { throw; } catch (const std::exception& e) { 说明 = e.what(); } catch (...) {}
        if (!已发布) {
            if (旧更新.准备) 特征服务_.取消顶层I64更新(*旧更新.准备);
            if (前状态 && 状态服务_.删除状态(*前状态) != 新状态操作状态::已完成)
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"建立方法", L"未发布前状态清理失败。");
            回收本批方法图(已建节点, 已写字段, 虚拟组, false);
        }
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"建立方法治理结构", L"方法结构、特征或场景未完整发布读回，保留已发表事实。",
            说明.empty() ? nullptr : 说明.c_str());
    }
}

新_方法类::新_方法类(
    新_存在类& 存在服务,
    新_场景类& 场景服务,
    新_特征值类& 特征值服务,
    新_特征类& 特征服务,
    概念_存在类& 存在概念服务,
    概念_特征类& 特征概念服务, 新_状态类& 状态服务) noexcept
    : 存在服务_(存在服务),
      场景服务_(场景服务),
      特征值服务_(特征值服务),
      特征服务_(特征服务),
      存在概念服务_(存在概念服务),
      特征概念服务_(特征概念服务), 状态服务_(状态服务) {}

bool 新_方法类::初始化() noexcept {
    try {
        const auto 函数集 = 新_本能函数管理类::初始化();
        if (函数集 != 新本能函数登记状态::已登记
            && 函数集 != 新本能函数登记状态::精确重复) return false;
        std::lock_guard 锁(新方法互斥);
        const auto 确保父代字段 = [&]() {
            const std::array 内容键组{&条件引用内容要求字段,&条件引用内容项字段};
            if(std::all_of(内容键组.begin(),内容键组.end(),[](const auto* p){return !有效(*p);})) {
                const auto 创建=全局基础数据集.原子建立节点组(std::vector<基础组内新节点>(内容键组.size()));
                if(创建.状态!=基础结构操作状态::已完成||创建.节点组.size()!=内容键组.size())
                    throw std::runtime_error("method content condition schema atomic creation failed");
                for(std::size_t i=0;i<内容键组.size();++i)*内容键组[i]=创建.节点组[i];
            }
            for(const auto* p:内容键组) {
                if(!有效(*p)||!节点仍存在(*p))throw std::runtime_error("method content condition schema incomplete");
                for(const auto* old:架构编码组())if(*old==*p)throw std::runtime_error("method content condition schema collision");
            }
            const std::array 正式内容键组{&条件内容作用点字段,&条件前态当前字段,&条件前态历史数字段,
                &结果引用内容要求字段,&结果引用内容项字段,&结果内容作用点字段,
                &内容关系成员字段,&内容关系节点类型,&内容关系所属字段,
                &内容关系左字段,&内容关系右字段,&内容关系路径字段};
            if(std::all_of(正式内容键组.begin(),正式内容键组.end(),[](const auto* p){return !有效(*p);})) {
                const auto r=全局基础数据集.原子建立节点组(std::vector<基础组内新节点>(正式内容键组.size()));
                if(r.状态!=基础结构操作状态::已完成||r.节点组.size()!=正式内容键组.size())
                    throw std::runtime_error("method formal content schema creation failed");
                for(std::size_t i=0;i<正式内容键组.size();++i)*正式内容键组[i]=r.节点组[i];
            }
            for(const auto* p:正式内容键组) {
                if(!有效(*p)||!节点仍存在(*p))throw std::runtime_error("method formal content schema incomplete");
                for(const auto* old:架构编码组())if(*old==*p)throw std::runtime_error("method formal content schema collision");
            }
            if(!有效(条件内容认知有效字段)) {
                const auto r=全局基础数据集.原子建立节点组(std::vector<基础组内新节点>(1));
                if(r.状态!=基础结构操作状态::已完成||r.节点组.size()!=1)
                    throw std::runtime_error("method current provenance condition key creation failed");
                条件内容认知有效字段=r.节点组.front();
            }
            if(!节点仍存在(条件内容认知有效字段))throw std::runtime_error("method current provenance condition key unavailable");
            for(const auto* old:架构编码组())if(*old==条件内容认知有效字段)
                throw std::runtime_error("method current provenance condition key collision");
            const std::array resultSceneKeys{&结果场景限定字段,&结果场景限定节点类型,&结果场景所属字段,&结果场景关系概念字段,&结果场景四位置字段};
            if(std::all_of(resultSceneKeys.begin(),resultSceneKeys.end(),[](const auto* p){return !有效(*p);})) {
                const auto group=全局基础数据集.原子建立节点组(std::vector<基础组内新节点>(resultSceneKeys.size()));
                if(group.状态!=基础结构操作状态::已完成||group.节点组.size()!=resultSceneKeys.size())
                    throw std::runtime_error("method result scene keys atomic creation failed");
                for(std::size_t i=0;i<resultSceneKeys.size();++i)*resultSceneKeys[i]=group.节点组[i];
            }
            for(const auto* p:resultSceneKeys) {
                if(!有效(*p)||!节点仍存在(*p))throw std::runtime_error("method result scene keys incomplete");
                for(const auto* old:架构编码组())if(*old==*p)throw std::runtime_error("method result scene keys overlap original schema");
            }
            if (!有效(方法父代字段)) 方法父代字段 = 全局基础数据集.新建节点();
            if (!节点仍存在(方法父代字段))
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"初始化方法代际字段", L"父代字段不可读。");
            for (const auto* 编码 : 架构编码组())
                if (*编码 == 方法父代字段)
                    工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"初始化方法代际字段", L"父代字段身份冲突。");
            for (auto* 键 : {&虚拟存在启用特征字段, &虚拟存在专属概念字段, &方法行为字段,
                    &有序参数序号字段, &有序参数类型字段, &有序参数类别概念字段, &有序参数范围字段,
                    &有序参数必需特征类型字段, &有序参数所属位置字段, &方法旧网页退役成员字段, &参数位置关系字段, &模拟事件位置字段, &配对知识来源字段, &任务成员变量字段,
                    &初始必要条件字段,&初始声明节点类型,&初始声明项节点类型,&初始场景限定节点类型,&登记结果组字段,
                    &初始声明格式字段,&初始声明所属字段,&初始场景限定字段,&初始关系概念字段,&初始四位置字段,
                    &执行分支字段,&分支节点类型,&分支所属方法字段,&分支所属虚拟字段,&分支条件字段,&配对分支来源字段,
                    &模拟输入方案字段,&模拟输入方案节点类型,&模拟消息输入字段,&模拟消息输入节点类型,
                    &模拟消息参数位置字段,&模拟消息方向字段,&模拟正文范围字段,&模拟正文非空字段,&模拟输入时机字段,&模拟触发位置方向字段,&结果被动需求目标字段,&结果作用存在字段}) {
                if (!有效(*键)) *键 = 全局基础数据集.新建节点();
                if (!节点仍存在(*键)) throw std::runtime_error("method governance key unavailable");
                for (const auto* 编码 : 架构编码组())
                    if (*编码 == *键) throw std::runtime_error("method governance key collision");
            }
            const std::vector<稳定编码> 扩展键组{方法父代字段, 虚拟存在启用特征字段,
                虚拟存在专属概念字段, 方法行为字段, 有序参数序号字段, 有序参数类型字段, 有序参数类别概念字段, 有序参数范围字段,
                有序参数必需特征类型字段, 有序参数所属位置字段, 方法旧网页退役成员字段, 参数位置关系字段, 模拟事件位置字段, 配对知识来源字段, 任务成员变量字段,
                初始必要条件字段,初始声明节点类型,初始声明项节点类型,初始场景限定节点类型,登记结果组字段,
                初始声明格式字段,初始声明所属字段,初始场景限定字段,初始关系概念字段,初始四位置字段,
                执行分支字段,分支节点类型,分支所属方法字段,分支所属虚拟字段,分支条件字段,配对分支来源字段,
                结果场景限定字段,结果场景限定节点类型,结果场景所属字段,结果场景关系概念字段,结果场景四位置字段,
                条件引用内容要求字段,条件引用内容项字段,
                条件内容作用点字段,条件内容认知有效字段,条件前态当前字段,条件前态历史数字段,
                结果引用内容要求字段,结果引用内容项字段,结果内容作用点字段,
                内容关系成员字段,内容关系节点类型,内容关系所属字段,内容关系左字段,内容关系右字段,内容关系路径字段,
                模拟输入方案字段,模拟输入方案节点类型,模拟消息输入字段,模拟消息输入节点类型,
                模拟消息参数位置字段,模拟消息方向字段,模拟正文范围字段,模拟正文非空字段,模拟输入时机字段,模拟触发位置方向字段,结果被动需求目标字段,结果作用存在字段};
            const std::set<稳定编码> 扩展键(扩展键组.begin(), 扩展键组.end());
            if (扩展键.size() != 扩展键组.size()) throw std::runtime_error("method extension keys collide");
            if (!有效(方法启用特征类型)) {
                特征概念定义 定义;
                定义.值域 = std::vector<特征概念I64闭区间>{{0, 1}};
                方法启用特征类型 = 特征概念服务_.建立或取得特征概念(定义, 特征值服务_);
            }
            const auto 类型 = 特征概念服务_.获取特征概念(方法启用特征类型);
            if (!类型 || 类型->材料类型 != 特征概念材料物理类型::I64标量
                || 类型->值域 != 特征概念值域{std::vector<特征概念I64闭区间>{{0, 1}}}
                || 类型->单位 || 类型->比较规则 != 特征概念比较规则::I64数值
                || 类型->聚合规则 != 特征概念聚合规则::不自动聚合)
                throw std::runtime_error("method enabled feature type invalid");
            try { 确保缺失方法列表(); }
            catch (...) {
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"初始化缺失方法列表", L"独立列表架构不完整，保留现场。");
            }
            try { 确保直接调用结构(); }
            catch (...) {
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"初始化直接调用来源", L"独立来源架构不完整，保留现场。");
            }
            return true;
        };
        if (结构已分配()) return 结构仍存在() && 确保父代字段();
        if (存在部分架构编码()) return false;
        std::vector<稳定编码> 已建节点;
        std::vector<稳定编码> 已写字段;
        const auto 编码指针 = 架构编码组();
        for (auto* 编码 : 编码指针) {
            *编码 = 全局基础数据集.新建节点();
            if (!有效(*编码)) {
                回滚(已写字段, 已建节点);
                for (auto* 清空 : 编码指针) *清空 = {};
                return false;
            }
            已建节点.push_back(*编码);
        }
        if (!写入节点字段(新方法组织根节点, 节点类型字段,
                方法组织根类型, 已写字段)) {
            回滚(已写字段, 已建节点);
            for (auto* 清空 : 编码指针) *清空 = {};
            return false;
        }
        return 结构仍存在() && 确保父代字段();
    } catch (...) {
        return false;
    }
}

std::vector<新缺失方法信息> 新_方法类::查询缺失方法列表() const noexcept {
    try {
        std::lock_guard 锁(新方法互斥);
        if (!结构仍存在() || !有效(缺失方法列表节点))
            throw std::runtime_error("missing-method list not initialized");
        确保缺失方法列表(); // 已初始化分支只核验，不补造丢失材料。
        const auto 成员 = 读取多节点字段(缺失方法列表节点, 缺失方法成员字段, 特征概念服务_);
        if (!成员) throw std::runtime_error("missing-method membership damaged");
        auto 节点组 = *成员;
        节点组.push_back(缺失方法列表节点);
        const auto 全部 = 全局基础数据集.查询节点组完整结构(节点组);
        if (全部.状态 != 基础结构操作状态::已完成 || 全部.节点组.size() != 节点组.size())
            throw std::runtime_error("missing-method graph unavailable");
        std::vector<新缺失方法信息> 结果;
        for (const auto& 项 : 全部.节点组) {
            if (!项.快照) throw std::runtime_error("missing-method snapshot absent");
            const auto& s = *项.快照;
            const auto n = s.结构.节点.编码;
            if (n == 缺失方法列表节点) {
                std::vector<稳定编码> 当前;
                for (const auto& f : s.结构.字段组) {
                    if (f.字段节点 != 缺失方法成员字段) continue;
                    const auto* p = std::get_if<稳定编码>(&f.内容);
                    if (!p) throw std::runtime_error("missing-method member is not reference");
                    当前.push_back(*p);
                }
                std::sort(当前.begin(), 当前.end());
                if (当前 != *成员) throw std::runtime_error("missing-method list changed during read");
                continue;
            }
            if (!s.入向外部关系组.empty() || !s.出向外部关系组.empty())
                throw std::runtime_error("missing-method head attached to a tree");
            const auto 归属 = std::count_if(s.入向字段组.begin(), s.入向字段组.end(), [](const auto& f) {
                return f.所属节点 == 缺失方法列表节点 && f.字段节点 == 缺失方法成员字段;
            });
            if (归属 != 1 || std::any_of(s.入向字段组.begin(), s.入向字段组.end(), [](const auto& f) {
                return f.字段节点 == 方法头字段 || f.字段节点 == 方法成员字段;
            })) throw std::runtime_error("missing-method head has executable ownership");
            新缺失方法信息 信息{n, {}};
            auto& d = 信息.定义;
            std::set<稳定编码> 单字段;
            for (const auto& f : s.结构.字段组) {
                const auto* p = std::get_if<稳定编码>(&f.内容);
                if (!p || !节点仍存在(*p)) throw std::runtime_error("missing-method reference damaged");
                if (f.字段节点 == 方法头作用存在概念字段) d.方法头.作用存在概念组.push_back(*p);
                else if (f.字段节点 == 方法头特征类型概念字段) d.方法头.涉及特征类型概念组.push_back(*p);
                else if (f.字段节点 == 方法头结果概念字段) d.方法头.可能结果概念组.push_back(*p);
                else {
                    if (!单字段.insert(f.字段节点).second) throw std::runtime_error("duplicate missing-method scalar field");
                    if (f.字段节点 == 节点类型字段) {
                        if (*p != 方法头节点类型) throw std::runtime_error("missing-method head type invalid");
                    } else if (f.字段节点 == 缺失方法值域字段) d.目标值域概念节点 = *p;
                    else if (f.字段节点 == 缺失方法比较字段) d.目标比较关系概念节点 = *p;
                    else if (f.字段节点 == 缺失方法方向字段) d.目标变化方向概念节点 = *p;
                    else if (f.字段节点 == 缺失方法需求字段) d.来源需求节点 = *p;
                    else if (f.字段节点 == 缺失方法任务字段) d.来源任务节点 = *p;
                    else throw std::runtime_error("unknown missing-method field");
                }
            }
            for (auto* g : {&d.方法头.作用存在概念组, &d.方法头.涉及特征类型概念组, &d.方法头.可能结果概念组}) {
                std::sort(g->begin(), g->end());
                if (std::adjacent_find(g->begin(), g->end()) != g->end())
                    throw std::runtime_error("duplicate missing-method concept");
            }
            if (!单字段.contains(节点类型字段) || !有效(d.来源需求节点) || !有效(d.来源任务节点)
                || d.方法头.作用存在概念组.empty() || d.方法头.涉及特征类型概念组.size() != 1
                || d.方法头.可能结果概念组.empty()
                || (d.目标变化方向概念节点 ? (d.目标值域概念节点 || d.目标比较关系概念节点)
                    : (!d.目标值域概念节点 || !d.目标比较关系概念节点)))
                throw std::runtime_error("incomplete missing-method definition");
            结果.push_back(std::move(信息));
        }
        std::sort(结果.begin(), 结果.end(), [](const auto& a, const auto& b) { return a.方法头节点 < b.方法头节点; });
        return 结果;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取缺失方法列表", L"列表或方法头结构损坏，不得作为空列表继续筹办。");
    }
}

std::optional<新缺失方法信息> 新_方法类::读取缺失方法(稳定编码 方法头节点) const noexcept {
    try {
        for (const auto& 项 : 查询缺失方法列表())
            if (项.方法头节点 == 方法头节点) return 项;
        return std::nullopt;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取缺失方法", L"方法头读回失败，不得作为合法不存在返回。");
    }
}

新缺失方法登记结果 新_方法类::登记缺失方法(const 新缺失方法登记请求& 请求) noexcept {
    try {
        std::lock_guard 锁(新方法互斥);
        if (!结构仍存在() || !有效(缺失方法列表节点))
            return {新方法操作状态::尚未初始化, {}};
        auto d = 请求;
        for (auto* g : {&d.方法头.作用存在概念组, &d.方法头.涉及特征类型概念组, &d.方法头.可能结果概念组}) {
            std::sort(g->begin(), g->end());
            g->erase(std::unique(g->begin(), g->end()), g->end());
        }
        if (!有效(d.来源需求节点) || !有效(d.来源任务节点)
            || d.来源需求节点 == d.来源任务节点 || !方法头有效(d.方法头)
            || d.方法头.作用存在概念组.empty() || d.方法头.涉及特征类型概念组.size() != 1
            || (d.目标变化方向概念节点 ? (d.目标值域概念节点 || d.目标比较关系概念节点)
                : (!d.目标值域概念节点 || !d.目标比较关系概念节点)))
            return {新方法操作状态::入口拒绝, {}};
        // 来源需求/任务可被任务线程并发删除：属内部世界变化，不是请求形状错误。
        if (!节点仍存在(d.来源需求节点) || !节点仍存在(d.来源任务节点))
            return {新方法操作状态::定义前提已变化, {}};
        for (auto ec : d.方法头.作用存在概念组)
            if (!存在概念服务_.获取存在概念(ec)) return {新方法操作状态::方法头不合法, {}};
        const auto ft = d.方法头.涉及特征类型概念组.front();
        if (读取线性特征类型根(ft, 特征概念服务_) != ft)
            return {新方法操作状态::方法头不合法, {}};
        概念_二次特征类 二次(特征概念服务_, 特征值服务_);
        const auto rc = 二次.获取二次特征概念(d.目标变化方向概念节点
            ? *d.目标变化方向概念节点 : *d.目标比较关系概念节点);
        if (!rc) return {新方法操作状态::方法头不合法, {}};
        bool 有特征输入 = false;
        for (const auto& i : rc->输入定义组) {
            if (i.来源种类 != 二次特征输入来源种类::特征准确值) continue;
            if (!i.特征类型概念节点 || 读取线性特征类型根(*i.特征类型概念节点, 特征概念服务_) != ft)
                return {新方法操作状态::方法头不合法, {}};
            有特征输入 = true;
        }
        if (!有特征输入) return {新方法操作状态::方法头不合法, {}};
        if (d.目标变化方向概念节点) {
            const auto* 域 = std::get_if<std::vector<特征概念I64闭区间>>(&rc->输出特征概念.值域);
            if (rc->算法 != 二次特征算法::有向特征差值 || rc->参照特征概念节点 || !域 || 域->empty()
                || rc->输入定义组.size() != 2
                || !std::all_of(rc->输入定义组.begin(), rc->输入定义组.end(), [](const auto& i) {
                    return i.来源种类 == 二次特征输入来源种类::特征准确值;
                })
                || !(std::all_of(域->begin(), 域->end(), [](const auto& x) { return x.下界 > 0; })
                    || std::all_of(域->begin(), 域->end(), [](const auto& x) { return x.上界 < 0; })))
                return {新方法操作状态::方法头不合法, {}};
        } else if (读取线性特征类型根(*d.目标值域概念节点, 特征概念服务_) != ft
            || rc->参照特征概念节点 != d.目标值域概念节点)
            return {新方法操作状态::方法头不合法, {}};
        // 头仅记录本次缺口所需的结果概念，不允许夹带其它结果能力声明。
        std::vector<稳定编码> 目标结果{rc->节点};
        if (d.目标值域概念节点) 目标结果.push_back(*d.目标值域概念节点);
        std::sort(目标结果.begin(), 目标结果.end());
        目标结果.erase(std::unique(目标结果.begin(), 目标结果.end()), 目标结果.end());
        if (d.方法头.可能结果概念组 != 目标结果) return {新方法操作状态::方法头不合法, {}};
        for (const auto& 项 : 查询缺失方法列表())
            if (项.定义 == d) return {新方法操作状态::已找到, 项};
        基础节点组追加请求 原子;
        基础组内新节点 头;
        头.字段组.push_back({节点类型字段, 方法头节点类型});
        const auto 组 = [&](稳定编码 键, const std::vector<稳定编码>& 值) {
            for (auto n : 值) 头.字段组.push_back({键, n});
        };
        组(方法头作用存在概念字段, d.方法头.作用存在概念组);
        组(方法头特征类型概念字段, d.方法头.涉及特征类型概念组);
        组(方法头结果概念字段, d.方法头.可能结果概念组);
        if (d.目标值域概念节点) 头.字段组.push_back({缺失方法值域字段, *d.目标值域概念节点});
        if (d.目标比较关系概念节点) 头.字段组.push_back({缺失方法比较字段, *d.目标比较关系概念节点});
        if (d.目标变化方向概念节点) 头.字段组.push_back({缺失方法方向字段, *d.目标变化方向概念节点});
        头.字段组.push_back({缺失方法需求字段, d.来源需求节点});
        头.字段组.push_back({缺失方法任务字段, d.来源任务节点});
        原子.新节点组.push_back(std::move(头));
        原子.追加字段组.push_back({缺失方法列表节点, 缺失方法成员字段, 基础组内节点引用{0}});
        const auto 根快照 = 全局基础数据集.查询节点组完整结构({缺失方法列表节点});
        if (根快照.状态 != 基础结构操作状态::已完成 || 根快照.节点组.size() != 1 || !根快照.节点组.front().快照)
            throw std::runtime_error("missing-method list snapshot unavailable");
        原子.预期联合节点组.push_back(*根快照.节点组.front().快照);
        const auto r = 全局基础数据集.原子追加节点组(原子);
        // 先查后追加期间来源需求/任务被并发删除：前提已变化，未发布。
        if (r.状态 == 基础节点组追加状态::引用不存在 || r.状态 == 基础节点组追加状态::节点不存在)
            return {新方法操作状态::定义前提已变化, {}};
        if (r.状态 == 基础节点组追加状态::资源失败) return {新方法操作状态::资源失败, {}};
        if (r.状态 != 基础节点组追加状态::已完成 || r.新节点组.size() != 1)
            throw std::runtime_error("missing-method atomic publication failed");
        const auto 已有 = 读取缺失方法(r.新节点组.front());
        if (!已有 || 已有->定义 != d) throw std::runtime_error("missing-method publication readback mismatch");
        return {新方法操作状态::已建立, 已有};
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"登记缺失方法", L"缺失方法发布或读回错误，保留现场，不转为业务缺口。");
    }
}

bool 新_方法类::装配内部治理世界(新方法治理世界协作接口& 世界) noexcept {
    std::lock_guard 锁(新方法互斥);
    if (内部治理世界_ && 内部治理世界_ != &世界) return false;
    内部治理世界_ = &世界;
    return true;
}

void 新_方法类::核验治理组织就绪() const {
    // 调用入口已核对方法架构，这里只确认治理依赖已装配并完成启动。
    if (!内部治理世界_ || !治理组织已就绪_)
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"核验方法治理就绪", L"方法治理尚未装配或完成启动迁接。");
}

std::optional<稳定编码> 新_方法类::获取方法启用特征类型() const noexcept {
    try {
        稳定编码 类型;
        { std::lock_guard 锁(新方法互斥); 类型 = 方法启用特征类型; }
        if (!有效(类型)) return {};
        const auto 定义 = 特征概念服务_.获取特征概念(类型);
        const auto 保护 = 特征概念服务_.读取特征概念定义保护(类型);
        if (保护.前提已变化) return {};
        if (!定义 || 保护.状态 != 基础结构操作状态::已完成 || !保护.信息
            || 保护.信息->类型根 != 类型
            || 定义->材料类型 != 特征概念材料物理类型::I64标量
            || 定义->值域 != 特征概念值域{std::vector<特征概念I64闭区间>{{0,1}}}
            || 定义->单位 || 定义->比较规则 != 特征概念比较规则::I64数值
            || 定义->聚合规则 != 特征概念聚合规则::不自动聚合)
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"方法启用类型读取", L"已初始化类型定义或根保护不符。");
        return 类型;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"方法启用类型读取", L"类型读取异常。");
    }
}

std::optional<方法统计特征类型> 新_方法类::初始化并读取方法统计特征类型() noexcept {
    try {
        if (!获取方法启用特征类型()) return {};
        const auto 建立 = [&](std::uint32_t 槽, std::int64_t 上界) {
            特征概念定义 定义;
            定义.材料类型 = 特征概念材料物理类型::I64标量;
            定义.值域 = std::vector<特征概念I64闭区间>{{0,上界}};
            定义.比较规则 = 特征概念比较规则::I64数值;
            定义.聚合规则 = 特征概念聚合规则::不自动聚合;
            const auto 结果 = 特征概念服务_.建立或取得先天特征类型槽({槽, 定义}, 特征值服务_, {});
            if (结果.状态 == 特征概念规则状态::前提已变化) return std::optional<稳定编码>{};
            if ((结果.状态 != 特征概念规则状态::已完成 && 结果.状态 != 特征概念规则状态::无变化)
                || !结果.类型根 || 结果.完整保护组.empty())
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"方法统计类型初始化", L"固定统计槽定义或发布读回不完整。");
            return 结果.类型根;
        };
        const auto 运行 = 建立(先天特征类型槽::方法运行次数, (std::numeric_limits<std::int64_t>::max)());
        const auto 成功 = 建立(先天特征类型槽::方法成功次数, (std::numeric_limits<std::int64_t>::max)());
        const auto 可靠 = 建立(先天特征类型槽::方法可靠度, 10000);
        if (!运行 || !成功 || !可靠) return {};
        if (*运行 == *成功 || *运行 == *可靠 || *成功 == *可靠)
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"方法统计类型初始化", L"不同统计语义共用了类型根。");
        return 方法统计特征类型{*运行, *成功, *可靠};
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"方法统计类型初始化", L"统计类型定义异常，不填运行值。");
    }
}

std::optional<新方法虚拟存在读回> 新_方法类::读取方法虚拟存在引用(
    const 新方法虚拟存在引用& 引用) const noexcept {
    try {
        std::lock_guard 锁(新方法互斥);
        if (!有效(引用.方法节点)) return std::nullopt;
        if (!节点类型为(引用.方法节点, 方法节点类型) || !方法属于根(引用.方法节点))
            return std::nullopt;
        const auto vm = 读取唯一节点字段(引用.方法节点, 方法虚拟存在字段);
        if (!vm || !节点类型为(*vm, 方法虚拟存在节点类型)
            || 读取唯一节点字段(*vm, 虚拟存在所属方法字段) != 引用.方法节点
            || !全局基础数据集.查询字段(引用.方法节点, 方法启用状态字段).empty())
            throw std::runtime_error("method virtual owner damaged");
        const auto f = 读取唯一节点字段(*vm, 虚拟存在启用特征字段);
        const auto ec = 读取唯一节点字段(*vm, 虚拟存在专属概念字段);
        if (!f || !ec) throw std::runtime_error("method virtual feature ownership incomplete");
        if (*vm != 引用.虚拟存在节点 || *f != 引用.启用特征节点) return std::nullopt;
        const auto v = 特征服务_.获取特征(*f);
        const auto e = 存在概念服务_.获取存在概念(*ec);
        const auto value = v && v->当前值 ? std::get_if<std::int64_t>(&*v->当前值) : nullptr;
        if (!v || v->上级特征节点 || v->实例单位 || !value || (*value != 0 && *value != 1)
            || !e || e->种类 != 存在概念种类::专属 || e->对应存在节点 != *vm
            || e->特征组.size() != 1
            || e->特征组.front().特征类型概念 != 方法启用特征类型
            || e->特征组.front().具体特征概念 != v->特征概念节点)
            throw std::runtime_error("method enabled feature/concept inconsistent");
        const auto 比较 = 特征概念服务_.比较单特征概念(v->特征概念节点, 方法启用特征类型);
        if (比较.状态 != 特征概念规则状态::已完成 || 比较.特征类型根 != 方法启用特征类型
            || !比较.关系 || (*比较.关系 != 单特征概念关系::同一 && *比较.关系 != 单特征概念关系::右为上位))
            throw std::runtime_error("method enabled feature has wrong type");
        const auto vm持有 = 全局基础数据集.查询目标字段(*vm, 方法虚拟存在字段);
        const auto f持有 = 全局基础数据集.查询目标字段(*f, 虚拟存在启用特征字段);
        const auto ec持有 = 全局基础数据集.查询目标字段(*ec, 虚拟存在专属概念字段);
        if (vm持有.size() != 1 || vm持有.front().所属节点 != 引用.方法节点
            || f持有.size() != 1 || f持有.front().所属节点 != *vm
            || ec持有.size() != 1 || ec持有.front().所属节点 != *vm)
            throw std::runtime_error("method virtual feature ownership not unique");
        新方法虚拟存在读回 读回{引用, *ec, 方法启用特征类型, v->特征概念节点, *value};
        // METHOD-LEARN-STATE：配对数特征（旧方法补齐前可缺）。
        for (std::size_t i = 0; i < 2; ++i) {
            if (!有效(虚拟存在配对数特征字段[i]) || !有效(方法配对数特征类型[i])) continue;
            const auto 组 = 全局基础数据集.查询字段(*vm, 虚拟存在配对数特征字段[i]);
            if (组.empty()) continue;
            const auto* F = 组.size() == 1 ? std::get_if<稳定编码>(&组.front().内容) : nullptr;
            const auto 特征 = F ? 特征服务_.获取特征(*F) : std::nullopt;
            const auto* 数 = 特征 && 特征->当前值 ? std::get_if<std::int64_t>(&*特征->当前值) : nullptr;
            const auto 持有 = F ? 全局基础数据集.查询目标字段(*F, 虚拟存在配对数特征字段[i])
                : decltype(全局基础数据集.查询目标字段(*vm, 虚拟存在配对数特征字段[i])){};
            if (!数 || *数 < 0 || 特征->上级特征节点 || 特征->实例单位
                || 持有.size() != 1 || 持有.front().所属节点 != *vm)
                throw std::runtime_error("method pair-count feature ownership/value inconsistent");
            const auto 类型 = 特征概念服务_.比较单特征概念(特征->特征概念节点, 方法配对数特征类型[i]);
            if (类型.状态 != 特征概念规则状态::已完成 || 类型.特征类型根 != 方法配对数特征类型[i])
                throw std::runtime_error("method pair-count feature has wrong type");
            (i == 0 ? 读回.条件结果配对数 : 读回.已知结果配对数)
                = 新方法配对数特征读回{*F, 方法配对数特征类型[i], *数};
        }
        return 读回;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取方法虚拟存在", L"方法虚拟存在、启用特征或专属概念损坏，保留现场。");
    }
}

新_方法类::方法虚拟存在准备 新_方法类::准备方法虚拟存在(
    稳定编码 方法, 稳定编码 虚拟存在, std::int64_t 启用) {
    if ((启用 != 0 && 启用 != 1) || !节点类型为(方法, 方法节点类型)
        || !节点类型为(虚拟存在, 方法虚拟存在节点类型))
        throw std::runtime_error("method virtual preparation identity invalid");
    方法虚拟存在准备 结果;
    auto& v = 结果.信息;
    v = {{方法, 虚拟存在, {}}, {}, 方法启用特征类型, {}, 启用};
    try {
        v.引用.启用特征节点 = 特征服务_.建立特征(方法启用特征类型);
        if (!有效(v.引用.启用特征节点)) throw std::runtime_error("method feature creation failed");
        const auto 写入 = 特征服务_.添加特征值(v.引用.启用特征节点, 新特征准确值{启用});
        const auto f = 特征服务_.获取特征(v.引用.启用特征节点);
        if (写入.状态 != 新特征写入状态::已完成 || !f || !f->当前值
            || *f->当前值 != 新特征准确值{启用}) throw std::runtime_error("method initial enabled value failed");
        v.当前特征概念节点 = f->特征概念节点;
        const auto ec = 存在概念服务_.建立专属存在概念(虚拟存在, {f->特征概念节点});
        if (ec.状态 != 存在概念操作状态::已建立 || !ec.概念节点)
            throw std::runtime_error("method private existence concept creation failed");
        v.专属存在概念节点 = *ec.概念节点;
        结果.完整准备 = true;
        核验方法私有材料未消费(结果);
        return 结果;
    } catch (...) { 清理未绑定方法特征(结果); throw; }
}

void 新_方法类::核验方法私有材料未消费(const 方法虚拟存在准备& 准备,
    std::optional<新方法治理场景信息> 本批场景) const {
    const auto& v = 准备.信息;
    if (!准备.完整准备 || !状态服务_.按实例特征查询状态(v.引用.启用特征节点).empty())
        throw std::runtime_error("method private material lacks complete preparation or has a state");
    std::vector<稳定编码> F允许入边, EC允许入边;
    if (本批场景) {
        if (本批场景->方法引用 != v.引用) throw std::runtime_error("foreign method scene in reclaim");
        const auto f = 读取唯一节点字段记录(v.引用.虚拟存在节点, 虚拟存在启用特征字段);
        const auto ec = 读取唯一节点字段记录(v.引用.虚拟存在节点, 虚拟存在专属概念字段);
        const auto c = 全局基础数据集.查询字段关系(本批场景->启用特征归属字段关系);
        if (!f || f->目标 != v.引用.启用特征节点 || !ec || ec->目标 != v.专属存在概念节点
            || !c || c->所属节点 != 本批场景->场景节点
            || c->内容 != 基础字段内容{v.引用.启用特征节点})
            throw std::runtime_error("method reclaim exact ownership edges mismatch");
        F允许入边 = {f->关系, c->编码};
        EC允许入边 = {ec->关系};
    } else if (!全局基础数据集.查询字段(v.引用.虚拟存在节点, 虚拟存在启用特征字段).empty()
        || !全局基础数据集.查询字段(v.引用.虚拟存在节点, 虚拟存在专属概念字段).empty()) {
        throw std::runtime_error("unbound method material still has virtual owner fields");
    }
    // 私有记录、域和特征项由各owner枚举；本类只提供已证明的外部归属字段ID。
    特征服务_.核验初建I64特征可回收(v.引用.启用特征节点, v.当前特征概念节点, v.当前启用, F允许入边);
    存在概念服务_.核验初建单特征专属概念可回收(v.专属存在概念节点,
        v.引用.虚拟存在节点, {v.启用特征类型概念节点, v.当前特征概念节点}, EC允许入边);
}

void 新_方法类::清理未绑定方法特征(const 方法虚拟存在准备& 准备) noexcept {
    // 准备尚未返回时可能只创建了F或EC；这些材料尚未交付任何外部调用。
    // 完整准备返回后的所有清理必须再经两个owner检查完整私有闭包。
    const auto& v = 准备.信息;
    if (准备.完整准备) {
        try { 核验方法私有材料未消费(准备); }
        catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"回收未采用方法", L"F或专属EC已被消费，禁止拆除内部内容。"); }
    }
    if (有效(v.专属存在概念节点)
        && !存在概念服务_.撤销未绑定专属存在概念(v.专属存在概念节点, v.引用.虚拟存在节点))
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"回收未采用方法", L"方法专属概念无法安全撤销。");
    if (有效(v.引用.启用特征节点) && !特征服务_.删除特征(v.引用.启用特征节点))
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"回收未采用方法", L"方法特征无法安全删除。");
}

void 新_方法类::同步方法专属概念(const 新方法虚拟存在引用& 引用) noexcept {
    try {
        const auto ec = 读取唯一节点字段(引用.虚拟存在节点, 虚拟存在专属概念字段);
        const auto f = 特征服务_.获取特征(引用.启用特征节点);
        if (!ec || !f || 读取唯一节点字段(引用.虚拟存在节点, 虚拟存在启用特征字段) != 引用.启用特征节点)
            throw std::runtime_error("method feature finalization ownership changed");
        const auto r = 存在概念服务_.更新专属存在概念(*ec, {f->特征概念节点});
        if (r != 存在概念操作状态::已更新 && r != 存在概念操作状态::无变化)
            throw std::runtime_error("method concept finalization failed");
        if (!读取方法虚拟存在引用(引用)) throw std::runtime_error("method feature final readback failed");
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"同步方法专属概念", L"启用真实值已写入但概念收尾未完整确认，保留事实。");
    }
}

void 新_方法类::初始化治理场景组织() noexcept {
    try {
        std::lock_guard 锁(新方法互斥);
        if (!结构仍存在() || !内部治理世界_) throw std::runtime_error("method governance startup order invalid");
        const auto 成员 = 读取多节点字段(新方法组织根节点, 方法成员字段, 特征概念服务_);
        if (!成员) throw std::runtime_error("method root unreadable");
        std::vector<新方法信息> 旧方法;
        std::vector<新方法虚拟存在引用> 全部;
        // 整批只读预检先完成；任何不完整旧方法都阻止第一次迁接写入。
        for (const auto m : *成员) {
            const bool 旧 = !全局基础数据集.查询字段(m, 方法启用状态字段).empty();
            const auto info = 读取方法信息本体(m, 特征概念服务_, *this, 存在概念服务_, 旧, true);
            if (!info) throw std::runtime_error("method migration preflight failed");
            if (旧) {
                if (场景服务_.属于内部治理组织(info->方法虚拟存在节点)
                    || !状态服务_.按存在查询状态(info->方法虚拟存在节点).empty())
                    throw std::runtime_error("legacy method has unexplained scene/state");
                旧方法.push_back(*info);
            } else {
                const 新方法虚拟存在引用 ref{m, info->方法虚拟存在节点, info->启用特征节点};
                if (!内部治理世界_->读取方法治理场景(ref))
                    throw std::runtime_error("migrated method lacks scene");
                全部.push_back(ref);
            }
        }
        // world入口只读核验；先证明全部既有C/S/D完整，再开始旧M迁接写入。
        // 旧M必须没有C；任何残留/未知方法场景也会在此完整根扫描中被拒绝。
        内部治理世界_->迁移已有方法治理场景(全部);
        for (const auto& old : 旧方法) {
            const auto 记录 = 读取唯一I64字段记录(old.节点, 方法启用状态字段);
            if (!记录 || 记录->值 != static_cast<std::int64_t>(old.启用状态))
                throw std::runtime_error("legacy method changed after preflight");
            const auto 准备 = 准备方法虚拟存在(old.节点, old.方法虚拟存在节点, 记录->值);
            const auto& v = 准备.信息;
            bool 已发布 = false;
            try {
                基础节点组追加请求 req;
                追加方法虚拟归属(req, v);
                req.删除字段关系组.push_back(记录->关系);
                const auto seal = 封存方法片段(std::move(req), {old.节点, old.方法虚拟存在节点}, 存在概念服务_,特征概念服务_);
                const auto r = 内部治理世界_->发布新方法治理场景组({v.引用}, seal);
                if (r.状态 != 新方法治理场景发布工程状态::已发布)
                    throw std::runtime_error("legacy method zero publication");
                已发布 = true;
                const auto info = 读取方法信息本体(old.节点, 特征概念服务_, *this, 存在概念服务_, false, true);
                auto expected = old; expected.启用特征节点 = v.引用.启用特征节点;
                expected.专属存在概念节点 = v.专属存在概念节点;
                if (!info || *info != expected) throw std::runtime_error("method migration readback mismatch");
                全部.push_back(v.引用);
            } catch (...) { if (!已发布) 清理未绑定方法特征(准备); throw; }
        }
        内部治理世界_->迁移已有方法治理场景(全部);
        // METHOD-LEARN-STATE：方法状态治理结构（配对数特征类型、状态变化定义、学习记录根）与旧方法配对数特征幂等补齐。
        确保方法状态治理结构已加锁();
        for (const auto& 引用 : 全部) 确保方法配对数特征已加锁(引用.方法节点);
        const auto 退役组 = 读取旧网页退役成员(特征概念服务_);
        for (const auto n : 退役组) {
            const auto m = 读取方法信息本体(n, 特征概念服务_, *this, 存在概念服务_, false, true);
            if (!m || m->来源方法节点 || m->实例根节点
                || m->本能函数登记名 != 新本能函数登记名_旧两参数获取服务对象
                || m->启用状态 != 新方法启用状态::停用
                || std::find(成员->begin(), 成员->end(), n) == 成员->end())
                throw std::runtime_error("retired method recovery identity invalid");
        }
        if (!退役组.empty()
            && 查询本能登记结构(std::string(新本能函数登记名_旧两参数获取服务对象), 特征概念服务_, *this, 存在概念服务_).状态
                != 新方法操作状态::登记已退役)
            throw std::runtime_error("retired method family recovery incomplete");
        治理组织已就绪_ = true;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"初始化方法治理场景", L"旧方法来源或新治理组织未完整确认，保留现场并停止启动。");
    }
}

void 新_方法类::回收本批方法图(const std::vector<稳定编码>& 节点,
    const std::vector<稳定编码>& 字段, const std::vector<方法虚拟存在准备>& 虚拟组,
    bool 已发布) noexcept {
    try {
        if (节点.empty()) {
            if (!字段.empty() || !虚拟组.empty()) throw std::runtime_error("empty method graph has a nonempty creation ledger");
            return;
        }
        const std::set<稳定编码> 图(节点.begin(), 节点.end());
        if (图.size() != 节点.size()) throw std::runtime_error("method creation ledger has duplicate nodes");
        struct 批外归属 final { 稳定编码 字段键, 精确关系; bool 来自场景 = false, 来自根 = false; };
        std::map<std::pair<稳定编码, 稳定编码>, 批外归属> 外部归属;
        const auto 增加归属 = [&](稳定编码 源, 稳定编码 目标, 稳定编码 键,
            bool 来自场景, bool 来自根) {
            if (!有效(源) || !图.contains(目标) || 图.contains(源)
                || !外部归属.emplace(std::pair{源, 目标}, 批外归属{键, {}, 来自场景, 来自根}).second)
                throw std::runtime_error("method reclaim ownership identities overlap");
        };
        std::vector<新方法虚拟存在引用> 引用;
        for (const auto& 准备 : 虚拟组) {
            const auto& v = 准备.信息;
            引用.push_back(v.引用);
            std::optional<新方法治理场景信息> 场景;
            if (已发布) {
                场景 = 内部治理世界_->读取方法治理场景(v.引用);
                if (!场景 || 场景->方法引用 != v.引用) throw std::runtime_error("method reclaim scene missing");
                增加归属(场景->场景节点, v.引用.方法节点, {}, true, false);
                增加归属(场景->场景节点, v.引用.虚拟存在节点, {}, true, false);
            }
            核验方法私有材料未消费(准备, 场景);
            // EC owner已用正式字段证明对应Vm；下方只能从该确定源/目标的恰一边取ID。
            增加归属(v.专属存在概念节点, v.引用.虚拟存在节点, {}, false, false);
        }
        std::vector<稳定编码> 自有字段;
        for (const auto n : 节点) {
            if (!全局基础数据集.查询目标字段(n, 方法旧网页退役成员字段).empty())
                throw std::runtime_error("retired method knowledge is protected from reclamation");
            if (已发布 && 节点类型为(n, 方法节点类型))
                增加归属(新方法组织根节点, n, 方法成员字段, false, true);
            else if (已发布 && 节点类型为(n, 混合方法实例根类型))
                增加归属(新方法组织根节点, n, 混合方法实例成员字段, false, true);
            const auto r = 全局基础数据集.查询节点组完整结构({n});
            if (r.状态 != 基础结构操作状态::已完成 || r.节点组.size() != 1 || !r.节点组[0].快照)
                throw std::runtime_error("method reclaim graph unreadable");
            const auto& s = *r.节点组[0].快照;
            if (!s.入向外部关系组.empty() || !s.出向外部关系组.empty())
                throw std::runtime_error("method graph has external relation");
            for (const auto& f : s.结构.字段组) {
                if (f.所属节点 != n) throw std::runtime_error("method own field owner mismatch");
                自有字段.push_back(f.编码);
            }
            for (const auto& f : s.入向字段组) {
                if (图.contains(f.所属节点)) continue;
                const auto found = 外部归属.find({f.所属节点, n});
                if (found == 外部归属.end() || f.内容 != 基础字段内容{n}
                    || 有效(found->second.精确关系)
                    || (有效(found->second.字段键) && f.字段节点 != found->second.字段键))
                    throw std::runtime_error("unadopted method graph already consumed");
                found->second.精确关系 = f.编码;
            }
        }
        std::vector<稳定编码> 允许入边, 撤销场景后允许入边, 根成员字段;
        std::set<稳定编码> 已见允许关系;
        for (const auto& [身份, 归属] : 外部归属) {
            if (!有效(归属.精确关系) || !已见允许关系.insert(归属.精确关系).second)
                throw std::runtime_error("method reclaim exact owner edge missing or duplicate");
            允许入边.push_back(归属.精确关系);
            if (!归属.来自场景) 撤销场景后允许入边.push_back(归属.精确关系);
            if (归属.来自根) 根成员字段.push_back(归属.精确关系);
        }
        const std::set<稳定编码> 自有集合(自有字段.begin(), 自有字段.end());
        if (自有集合.size() != 自有字段.size()
            || std::any_of(字段.begin(), 字段.end(), [&](auto f) { return !自有集合.contains(f); })
            || 全局基础数据集.检查节点结构引用闭包(节点, 自有字段, {}, 允许入边)
                != 基础结构操作状态::已完成)
            throw std::runtime_error("method graph complete reference preflight failed");
        // 上述全部方法图、F和EC均完成只读预检后，才允许本批第一笔删除。
        if (已发布) 内部治理世界_->回收未消费方法治理场景组(引用);
        if (全局基础数据集.检查节点结构引用闭包(节点, 自有字段, {}, 撤销场景后允许入边)
                != 基础结构操作状态::已完成)
            throw std::runtime_error("method graph changed after scene reclamation");
        std::set<稳定编码> 待删字段(自有字段.begin(), 自有字段.end());
        待删字段.insert(根成员字段.begin(), 根成员字段.end());
        for (const auto f : 待删字段)
            if (!全局基础数据集.删除字段(f)) throw std::runtime_error("method reclaim field deletion failed");
        for (const auto& v : 虚拟组) 清理未绑定方法特征(v);
        for (auto n = 节点.rbegin(); n != 节点.rend(); ++n)
            if (!全局基础数据集.删除节点(*n)) throw std::runtime_error("method reclaim node deletion failed");
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"回收未采用方法", L"本批方法存在外部消费或清理不完整，保留剩余现场。");
    }
}

新方法建立结果 新_方法类::建立方法知识(
    const 新方法知识建立请求& 原请求) noexcept {
    if(原请求.初始必要条件)return {新方法操作状态::函数登记冲突,{}};
    新方法建立结果 结果;
    try {
        auto 请求=原请求;规范内容关系声明(请求);
        std::lock_guard 锁(新方法互斥);
        if (!结构仍存在()) {
            结果.状态 = 新方法操作状态::尚未初始化;
            return 结果;
        }
        核验治理组织就绪();
        if (!行为声明有效(请求) || !是作用域(static_cast<std::int64_t>(请求.作用域))
            || !是实现方式(static_cast<std::int64_t>(请求.实现方式))) {
            return 结果;
        }
        if (!方法头有效(请求.方法头)) {
            结果.状态 = 新方法操作状态::方法头不合法;
            return 结果;
        }
        if (!参数规格有效(请求.参数规格组, 特征概念服务_)) {
            结果.状态 = 新方法操作状态::参数规格不合法;
            return 结果;
        }
        if (请求.条件结果组.empty()
            || !std::all_of(请求.条件结果组.begin(), 请求.条件结果组.end(),
                [&](const 新方法条件结果定义& 定义) {
                    return 条件结果有效(定义, 特征概念服务_)
                        && 条件结果角色已声明(请求.参数规格组, 定义, 特征概念服务_, 存在概念服务_);
                })) {
            结果.状态 = 新方法操作状态::条件结果不合法;
            return 结果;
        }
        const auto 成员组 = 读取多节点字段(新方法组织根节点, 方法成员字段, 特征概念服务_);
        if (!成员组) {
            结果.状态 = 新方法操作状态::结构不一致;
            return 结果;
        }
        for (const auto 已有节点 : *成员组) {
            const auto 已有 = 读取方法信息本体(已有节点, 特征概念服务_, *this, 存在概念服务_, false, true);
            if (!已有) {
                结果.状态 = 新方法操作状态::结构不一致;
                return 结果;
            }
            if (已有->作用域 == 请求.作用域
                && 已有->行为 == 请求.行为
                && 已有->实现方式 == 请求.实现方式
                && 已有->方法头 == 请求.方法头
                && 已有->参数规格组 == 请求.参数规格组
                && 分支定义组(*已有)==请求.执行分支组
                && 已有->初始模拟输入方案组==请求.初始模拟输入方案组
                && !已有->来源方法节点
                && 已有->条件结果组.size() == 请求.条件结果组.size()
                && std::equal(已有->条件结果组.begin(), 已有->条件结果组.end(), 请求.条件结果组.begin(),
                    [](const auto& 左, const auto& 右) {
                        return 左.条件 == 右.条件 && 同义结果(左.结果,右.结果)
                            && 线性关系等价(左.线性关系组, 右.线性关系组);
                    })) {
                结果.状态 = 新方法操作状态::已找到;
                结果.方法节点 = 已有节点;
                if (!内部治理世界_->读取方法治理场景({已有->节点, 已有->方法虚拟存在节点, 已有->启用特征节点}))
                    工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"复用方法知识", L"已登记方法没有完整治理场景。");
                return 结果;
            }
        }

        return 发布方法结构(请求, nullptr);
    } catch (const 引用内容定义前提变化&) {
        结果.状态 = 新方法操作状态::定义前提已变化;
        return 结果;
    } catch (...) {
        结果.状态 = 新方法操作状态::资源失败;
        return 结果;
    }
}

namespace {
// 追加模拟配对与尝试配对共用的完整定义合法性校验（只读；TRIAL-LEARNING-REVIEW 自追加抽出，语义不变）。
bool 学习配对定义合法(const 新方法信息& 方法信息, const 新方法条件结果定义& 定义,
    概念_特征类& 特征概念服务_, 概念_存在类& 存在概念服务_) {
    const auto* 方法 = &方法信息;
    const auto 类型已声明 = [&](const auto& 项) {
        // 结果不依附参数的结果项由条件结果角色已声明另行核验类型根与值域。
        if constexpr(requires{项.结果作用存在;}) if(项.结果作用存在) return true;
        const auto 规格 = std::find_if(方法->参数规格组.begin(), 方法->参数规格组.end(),
            [&](const auto& 参数) { return 项.参数序号
                ? 参数.有序规格 && 参数.有序规格->序号 == 项.参数序号
                : !参数.有序规格 && 参数.参数角色节点 == 项.参数角色节点; });
        if (规格 != 方法->参数规格组.end() && 规格->有序规格) {
            if (项.任务成员变量序号) {
                const auto 外层 = 特征概念服务_.获取特征概念(规格->有序规格->范围概念节点);
                const auto 内层 = 项.特征类型概念节点 ? 特征概念服务_.获取特征概念(*项.特征类型概念节点) : std::nullopt;
                return 项.任务成员变量序号 == 1
                    && 规格->有序规格->信息类型 == 新方法参数信息类型::特征
                    && 外层 && 外层->比较规则 == 特征概念比较规则::服务任务集合身份判等
                    && 内层 && 内层->材料类型 == 特征概念材料物理类型::I64标量;
            }
            return !项.特征类型概念节点
                || 持有者特征类型允许(规格->有序规格->信息类型,*项.特征类型概念节点,特征概念服务_)
                || (规格->有序规格->信息类型 == 新方法参数信息类型::特征
                    && 特征概念为或从属于(规格->有序规格->范围概念节点,
                        *项.特征类型概念节点, 特征概念服务_));
        }
        return 规格 != 方法->参数规格组.end()
            && (!项.特征类型概念节点 || std::any_of(规格->特征规格组.begin(),
                规格->特征规格组.end(), [&](const auto& 特征) {
                    return 特征.特征类型概念节点 == *项.特征类型概念节点;
                }));
    };
    const bool 仅条件节点 = 定义.结果.结果项组.empty() && 定义.结果.参数关系组.empty()
        && 定义.结果.场景事实限定组.empty();
#ifdef HY_EGO_DEBUG_LOG_SIMULATION_PROGRESS
    const auto 不合法=[&](const wchar_t* 项) {
        try { (void)调试日志<调试切片::SIMULATION_PROGRESS>(L"学习配对合法性",L"不合法",
            std::wstring(L"方法=")+std::to_wstring(方法->节点.值)+L"；检查="+项); } catch(...) {}
        return false;
    };
#else
    const auto 不合法=[](const wchar_t*) { return false; };
#endif
    if(!行为结果相容(方法->行为, 定义.结果))return 不合法(L"行为结果相容");
    if(!配对分支来源有效(*方法,定义,特征概念服务_,存在概念服务_))return 不合法(L"配对分支来源");
    if(!方法头有效(方法->方法头))return 不合法(L"方法头");
    if(!参数规格有效(方法->参数规格组, 特征概念服务_))return 不合法(L"参数规格");
    // 仅条件节点（5370 §5.3，R96）：模拟取不到结果时结果为空，有参数的方法条件不得为空；
    // 零参数方法（获取交互者，2026-11-10用户裁决“不需要任何条件的条件节点”）允许空条件。
    if(仅条件节点 && !方法->参数规格组.empty() && 定义.条件.条件项组.empty() && 定义.条件.参数关系组.empty()
        && 定义.条件.场景事实限定组.empty())return 不合法(L"仅条件节点条件为空");
    if(!条件结果有效(定义, 特征概念服务_, false, 仅条件节点))return 不合法(L"条件结果有效");
    if(!条件结果角色已声明(方法->参数规格组, 定义, 特征概念服务_, 存在概念服务_)) {
#ifdef HY_EGO_DEBUG_LOG_SIMULATION_PROGRESS
        try {
            std::wostringstream t;
            const auto 关系=[&](const wchar_t* 名,const auto& 组){t<<名;for(const auto& r:组)t<<static_cast<int>(r.种类)<<L"("<<r.左参数序号<<L","<<r.右参数序号<<L") ";};
            const auto 事实=[&](const wchar_t* 名,const auto& 组){t<<名;for(const auto& r:组)t<<r.关系概念节点.值<<L"["<<r.场景参数序号<<r.主体参数序号<<r.对象参数序号<<r.特征参数序号<<L"] ";};
            const auto 项=[&](const wchar_t* 名,const auto& 组){t<<名;for(const auto& i:组)t<<i.参数序号<<L":"<<(i.特征类型概念节点 ? i.特征类型概念节点->值 : 0)<<L" ";};
            关系(L"条件关系=",定义.条件.参数关系组);关系(L"；结果关系=",定义.结果.参数关系组);
            事实(L"；条件事实=",定义.条件.场景事实限定组);事实(L"；结果事实=",定义.结果.场景事实限定组);
            项(L"；条件项=",定义.条件.条件项组);项(L"；结果项=",定义.结果.结果项组);
            t<<L"；参数范围=";for(const auto& p:方法->参数规格组)if(p.有序规格)t<<p.有序规格->序号<<L":"<<p.有序规格->范围概念节点.值<<L" ";
            (void)调试日志<调试切片::SIMULATION_PROGRESS>(L"学习配对合法性",L"角色声明明细",t.str());
        } catch(...) {}
#endif
#ifdef HY_EGO_DEBUG_LOG_SIMULATION_PROGRESS
        try {
            std::wostringstream t;t<<L"失败点="<<(角色声明失败点 ? 角色声明失败点 : L"前段")<<L"；条件项存在概念=";
            for(const auto& c:定义.条件.条件项组)t<<c.参数序号<<L":"<<(c.存在概念节点 ? c.存在概念节点->值 : 0)<<L" ";
            t<<L"；结果项=";
            for(const auto& r:定义.结果.结果项组)t<<r.参数序号<<L":域"<<(r.特征值域概念节点 ? r.特征值域概念节点->值 : 0)<<L"/精确"<<r.精确数值域.has_value()<<L" ";
            (void)调试日志<调试切片::SIMULATION_PROGRESS>(L"学习配对合法性",L"角色声明失败点",t.str());
        } catch(...) {}
#endif
        return 不合法(L"条件结果角色已声明");
    }
    if(!std::all_of(定义.条件.条件项组.begin(), 定义.条件.条件项组.end(), 类型已声明))return 不合法(L"条件项类型已声明");
    if(!std::all_of(定义.结果.结果项组.begin(), 定义.结果.结果项组.end(), 类型已声明))return 不合法(L"结果项类型已声明");
    if(!std::all_of(定义.结果.确认要求.禁止结果项组.begin(),
        定义.结果.确认要求.禁止结果项组.end(), 类型已声明))return 不合法(L"禁止结果项类型已声明");
    return true;
}
}

namespace {
// 模拟学习排查：方法owner报告“前提已变化”时记下发生在哪一步（调试切片 SIMULATION_PROGRESS）。
void 记录方法前提变化诊断(const wchar_t* 入口, const std::wstring& 文本) noexcept {
#ifdef HY_EGO_DEBUG_LOG_SIMULATION_PROGRESS
    try { (void)调试日志<调试切片::SIMULATION_PROGRESS>(L"方法前提变化", 入口, 文本); } catch (...) {}
#else
    (void)入口; (void)文本;
#endif
}
// 学习基线读过的全部节点身份以只读存在预期进入最终原子提交（不附加旧内容相等要求，裁决①）。
void 加学习基线存在预期(基础节点组追加请求& 请求, const 新_方法类::执行结构读取结果* 基线) {
    if (!基线) return;
    std::set<稳定编码> 已有;
    for (const auto& n : 请求.只读预期.节点组) 已有.insert(n.编码);
    for (const auto& 快照 : 基线->完整保护组)
        if (已有.insert(快照.结构.节点.编码).second) 请求.只读预期.节点组.push_back(基础节点{快照.结构.节点.编码});
}
bool 提交前提已变化(基础节点组追加状态 s) {
    return s == 基础节点组追加状态::预期不符 || s == 基础节点组追加状态::节点不存在
        || s == 基础节点组追加状态::引用不存在;
}
}
新方法学习发布结果 新_方法类::追加模拟条件结果(
    稳定编码 方法节点,
    const std::vector<新方法条件结果定义>& 新配对定义组,
    const 执行结构读取结果* 学习基线) noexcept {
    新方法学习发布结果 结果;
    {
        std::lock_guard 锁(新方法互斥);
        学习前确保配对数特征(方法节点);
        结果 = 追加条件结果内部(方法节点,新配对定义组,新方法知识来源种类::模拟,学习基线);
        if (结果.状态 == 新方法学习发布状态::已发布)
            学习后形成方法状态(方法节点, 新方法学习种类::模拟发布, 结果.新配对节点组);
    }
    发出方法状态通知();
    return 结果;
}
新_方法类::学习发布基线状态 新_方法类::核对学习发布基线(const 执行结构读取结果& 基线) const {
    using 状态 = 学习发布基线状态;
    std::lock_guard 锁(新方法互斥);
    if (基线.状态 != 执行结构读取状态::已读取) return 状态::已变化;
    // 方法结构：除已学条件结果组外的全部定义。
    const auto 同结构 = [](const 新方法信息& 左, const 新方法信息& 右) {
        auto 甲 = std::tie(左.节点, 左.作用域, 左.实现方式, 左.启用状态, 左.方法头节点, 左.方法头, 左.方法虚拟存在节点,
            左.参数规格组, 左.本能函数登记名, 左.组合步骤组, 左.组合入口步骤节点, 左.结果转移组, 左.来源方法节点,
            左.实例根节点, 左.父代方法节点, 左.启用特征节点, 左.专属存在概念节点, 左.行为, 左.初始模拟关系组,
            左.初始模拟事件参数组, 左.初始模拟输入方案组, 左.初始必要条件, 左.执行分支组);
        auto 乙 = std::tie(右.节点, 右.作用域, 右.实现方式, 右.启用状态, 右.方法头节点, 右.方法头, 右.方法虚拟存在节点,
            右.参数规格组, 右.本能函数登记名, 右.组合步骤组, 右.组合入口步骤节点, 右.结果转移组, 右.来源方法节点,
            右.实例根节点, 右.父代方法节点, 右.启用特征节点, 右.专属存在概念节点, 右.行为, 右.初始模拟关系组,
            右.初始模拟事件参数组, 右.初始模拟输入方案组, 右.初始必要条件, 右.执行分支组);
        return 甲 == 乙;
    };
    // 逐个重读基线方法本身（不重走闭包读取器的整图严格核同），只比较结构；读不成方法即视为已变化。
    // 调用边由各方法的组合步骤组承载，结构相同即调用图相同。
    for (const auto& 原 : 基线.方法组) {
        std::optional<新方法信息> 今;
        try { 今 = 读取方法信息本体(原.节点, 特征概念服务_, *this, 存在概念服务_, false, true); }
        catch (const std::bad_alloc&) { throw; }
        catch (...) { 今.reset(); }
        if (!今 || !同结构(*今, 原)) {
            记录方法前提变化诊断(L"学习发布基线", L"原因=方法结构不同；方法=" + std::to_wstring(原.节点.值)
                + L"；可读=" + std::to_wstring(今.has_value()));
            return 状态::已变化;
        }
    }
    if (基线.完整保护组.empty()) return 状态::成立;
    std::vector<稳定编码> 节点组;
    for (const auto& 快照 : 基线.完整保护组) 节点组.push_back(快照.结构.节点.编码);
    const auto 读 = 全局基础数据集.查询节点组完整结构(节点组);
    if (读.状态 == 基础结构操作状态::资源失败) throw std::bad_alloc{};
    // 正常物理删除返回“已完成＋空快照”；其它状态或数量不符为工程损坏，不当作漂移。
    if (读.状态 != 基础结构操作状态::已完成 || 读.节点组.size() != 节点组.size())
        throw std::logic_error("learning publication baseline node read inconsistent");
    for (const auto& 项 : 读.节点组) if (!项.快照) {
        记录方法前提变化诊断(L"学习发布基线", L"原因=保护节点已删除；节点=" + std::to_wstring(项.查询节点.值));
        return 状态::已变化;
    }
    return 状态::成立;
}
新方法学习发布结果 新_方法类::确保先天被动配对(
    稳定编码 方法节点,const 新方法条件结果定义& 定义) noexcept {
    std::lock_guard 锁(新方法互斥);
    const auto 结果 = 追加条件结果内部(方法节点,{定义},新方法知识来源种类::先天);
    // 先天被动配对属本能登记的初始知识（不是学习）：只按实际数量校准配对数特征，不形成状态或动作。
    if (结果.状态 == 新方法学习发布状态::已发布) {
        try { 校准方法配对数特征已加锁(方法节点); }
        catch (...) {
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"确保先天被动配对", L"先天配对已发布但配对数特征校准失败，保留现场。");
        }
    }
    return 结果;
}
新方法学习发布结果 新_方法类::追加条件结果内部(
    稳定编码 方法节点,
    const std::vector<新方法条件结果定义>& 新配对定义组,
    新方法知识来源种类 来源,
    const 执行结构读取结果* 学习基线) noexcept {
    新方法学习发布结果 结果;
    if (!有效(方法节点) || 新配对定义组.size() != 1) return 结果;
    std::vector<稳定编码> 已建节点, 已写字段;
    bool 已发布 = false;
    try {
        std::lock_guard 锁(新方法互斥);
        if (!节点类型为(方法节点, 方法节点类型)) {
            结果.状态 = 新方法学习发布状态::方法不可用;
            return 结果;
        }
        const auto 方法 = 读取方法信息(方法节点, 特征概念服务_, *this, 存在概念服务_);
        if (!方法) {
            结果.状态 = 新方法学习发布状态::结构不一致;
            return 结果;
        }
        if (方法->来源方法节点 || 方法->实例根节点
            || 方法->实现方式 != 新方法实现方式::本能
            || 方法->启用状态 != 新方法启用状态::启用
            || !方法->本能函数登记名 || 方法->本能函数登记名->empty()) {
            结果.状态 = 新方法学习发布状态::方法不可用;
            return 结果;
        }
        // 学习基线在方法锁内复核（与发布同一锁区间），使挂接的方法结构与读过节点的存在性绑定到发布点。
        if (学习基线) {
            学习发布基线状态 基线状态 = 学习发布基线状态::已变化;
            try { 基线状态 = 核对学习发布基线(*学习基线); }
            catch (const std::logic_error& e) {
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"学习发布基线复核", L"基线节点读取出现工程损坏。", e.what());
            }
            if (基线状态 != 学习发布基线状态::成立) {
                结果.状态 = 新方法学习发布状态::前提已变化;
                return 结果;
            }
        }
        auto 定义 = 新配对定义组.front();规范内容关系配对(定义);
        if (!学习配对定义合法(*方法, 定义, 特征概念服务_, 存在概念服务_)) {
            结果.状态 = 新方法学习发布状态::条件结果不合法;
            return 结果;
        }
        // 先天配对恰含一个先天被动结果项；学习配对不得携带先天被动结果项（CONV-INSTINCT 6.3）。
        const bool 含被动=std::any_of(定义.结果.结果项组.begin(),定义.结果.结果项组.end(),[](const auto& 项){return 项.被动需求目标;})
            ||std::any_of(定义.结果.确认要求.禁止结果项组.begin(),定义.结果.确认要求.禁止结果项组.end(),[](const auto& 项){return 项.被动需求目标;});
        // 2026-11-10：先天配对也可是恰一个“结果作用存在”结果项（不依附参数的先天结果，如获取交互者“自我.交互者 ∈ 存在根”）。
        const bool 含作用存在=std::any_of(定义.结果.结果项组.begin(),定义.结果.结果项组.end(),[](const auto& 项){return 项.结果作用存在.has_value();});
        if((来源==新方法知识来源种类::先天)!=(含被动||含作用存在)||(含被动&&含作用存在)
            ||(来源==新方法知识来源种类::先天&&(定义.结果.结果项组.size()!=1||!定义.来源执行分支节点组.empty()||!定义.线性关系组.empty()))) {
            结果.状态 = 新方法学习发布状态::条件结果不合法;
            return 结果;
        }
        const 新方法条件结果信息* 已有配对 = nullptr;
        for (const auto& 配对 : 方法->条件结果组) {
            if (!同一学习条件(配对.条件, 定义.条件)) continue;
            if (已有配对) {
                结果.状态 = 新方法学习发布状态::结构不一致;
                return 结果;
            }
            已有配对 = &配对;
        }
        if (已有配对) {
            结果.已有配对节点 = 已有配对->配对节点;
            结果.状态 = 同义结果(已有配对->结果,定义.结果)
                && ((已有配对->线性关系组.empty() && 定义.线性关系组.empty())
                    || 已有配对->条件 == 定义.条件)
                && 线性关系等价(已有配对->线性关系组, 定义.线性关系组)
                ? 新方法学习发布状态::精确重复 : 新方法学习发布状态::同条件结果冲突;
            if(结果.状态==新方法学习发布状态::精确重复) {
                基础节点组追加请求 request;
                for(const auto B:定义.来源执行分支节点组)
                    if(std::find(已有配对->来源执行分支节点组.begin(),已有配对->来源执行分支节点组.end(),B)==已有配对->来源执行分支节点组.end())
                        request.追加字段组.push_back({已有配对->配对节点,配对分支来源字段,B});
                if(!request.追加字段组.empty()) {
                    std::vector<稳定编码> graph{方法节点};
                    for(std::size_t i=0;i<graph.size();++i) {
                        const auto type=读取唯一节点字段(graph[i],节点类型字段);
                        if(!type)throw std::runtime_error("method branch union graph unreadable");
                        for(const auto& field:方法复制字段规格(*type))if(field.种类==复制字段种类::独占子节点)
                            for(const auto& r:全局基础数据集.查询字段(graph[i],field.字段)) {
                                const auto* child=std::get_if<稳定编码>(&r.内容);
                                if(!child)throw std::runtime_error("method branch union child malformed");
                                if(std::find(graph.begin(),graph.end(),*child)==graph.end())graph.push_back(*child);
                            }
                    }
                    加学习基线存在预期(request,学习基线);
                    const auto sealed=封存方法片段(std::move(request),graph,存在概念服务_,特征概念服务_);
                    const auto published=全局基础数据集.原子追加封存片段组({sealed});
                    if(提交前提已变化(published.状态))throw 引用内容定义前提变化{};
                    if(published.状态==基础节点组追加状态::资源失败)throw std::bad_alloc{};
                    if(published.状态!=基础节点组追加状态::已完成)throw std::runtime_error("method branch source union failed");
                    已发布=true;   // 字段已生效：此后的异常只能报发布结果未知
                    const auto after=读取方法信息本体(方法节点,特征概念服务_,*this,存在概念服务_,false,true);
                    if(!after)throw std::runtime_error("method branch source union readback failed");
                    const auto pair=std::find_if(after->条件结果组.begin(),after->条件结果组.end(),[&](const auto& p){return p.配对节点==已有配对->配对节点;});
                    if(pair==after->条件结果组.end()||pair->条件!=已有配对->条件||pair->结果!=已有配对->结果||pair->命中次数!=已有配对->命中次数)
                        throw std::runtime_error("method branch source union changed semantic knowledge");
                    for(const auto B:定义.来源执行分支节点组)if(std::find(pair->来源执行分支节点组.begin(),pair->来源执行分支节点组.end(),B)==pair->来源执行分支节点组.end())
                        throw std::runtime_error("method branch source union lost new source");
                    for(const auto B:已有配对->来源执行分支节点组)if(std::find(pair->来源执行分支节点组.begin(),pair->来源执行分支节点组.end(),B)==pair->来源执行分支节点组.end())
                        throw std::runtime_error("method branch source union lost old source");
                }
            }
            return 结果;
        }
        std::int64_t 最大顺序 = -1;
        std::set<std::int64_t> 已有顺序;
        for (const auto& 配对 : 方法->条件结果组) {
            const auto 顺序 = 读取唯一I64字段(配对.配对节点, 顺序字段);
            if (!顺序 || *顺序 < 0 || !已有顺序.insert(*顺序).second
                || *顺序 == (std::numeric_limits<std::int64_t>::max)()) {
                结果.状态 = 新方法学习发布状态::结构不一致;
                return 结果;
            }
            最大顺序 = (std::max)(最大顺序, *顺序);
        }
        稳定编码 新配对{};
        if (!写入条件结果(方法节点, 定义, 最大顺序 + 1, 已建节点, 已写字段, &新配对)) {
            回滚(已写字段, 已建节点);
            结果.状态 = 新方法学习发布状态::资源失败;
            return 结果;
        }
        if (!写入I64字段(新配对, 配对知识来源字段, static_cast<std::int64_t>(来源), 已写字段)) {
            回滚(已写字段, 已建节点);
            结果.状态 = 新方法学习发布状态::资源失败; return 结果;
        }
        const auto 条件节点 = 读取唯一节点字段(新配对, 配对条件字段);
        const auto 结果节点 = 读取唯一节点字段(新配对, 配对结果字段);
        const auto 命中 = 读取唯一I64字段(新配对, 配对命中次数字段);
        const auto 条件 = 条件节点 ? 读取条件(*条件节点, 特征概念服务_) : std::nullopt;
        const auto 结果定义 = 结果节点 ? 读取结果(*结果节点, 特征概念服务_) : std::nullopt;
        const auto 线性 = 读取线性关系组(新配对);
        if (!条件 || !结果定义 || !命中 || *命中 != 0
            || *条件 != 定义.条件 || *结果定义 != 定义.结果
            || !线性 || !线性关系等价(*线性, 定义.线性关系组)) {
            回滚(已写字段, 已建节点);
            结果.状态 = 新方法学习发布状态::结构不一致;
            return 结果;
        }
        // 预先分配返回材料，唯一所属关系是本次单配对的发布点。
        结果.新配对节点组.push_back(新配对);
        基础节点组追加请求 发布请求;
        发布请求.追加字段组.push_back({方法节点,方法条件结果字段,新配对});
        加学习基线存在预期(发布请求,学习基线);
        std::vector<稳定编码> 发布图{方法节点};
        发布图.insert(发布图.end(),已建节点.begin(),已建节点.end());
        const auto 封存=封存方法片段(std::move(发布请求),发布图,存在概念服务_,特征概念服务_);
        const auto 发布=全局基础数据集.原子追加封存片段组({封存});
        if (发布.状态!=基础节点组追加状态::已完成) {
            回滚(已写字段, 已建节点);
            结果.新配对节点组.clear();
            // 提交时预期不符/节点或引用不存在是并发变化，交调用者重来；资源失败如实；其余为结构问题。
            if (提交前提已变化(发布.状态))
                记录方法前提变化诊断(L"学习发布提交", L"方法=" + std::to_wstring(方法节点.值)
                    + L"；提交状态=" + std::to_wstring(static_cast<int>(发布.状态)));
            结果.状态 = 提交前提已变化(发布.状态) ? 新方法学习发布状态::前提已变化
                : 发布.状态==基础节点组追加状态::资源失败 ? 新方法学习发布状态::资源失败
                : 新方法学习发布状态::结构不一致;
            return 结果;
        }
        已发布 = true;
        const auto 读回 = 读取方法信息本体(方法节点, 特征概念服务_, *this, 存在概念服务_, false, true);
        if (!读回 || 读回->行为 != 方法->行为
            || 读回->条件结果组.size() != 方法->条件结果组.size() + 1) {
            try { (void)记录运行日志(L"方法学习发布", L"读回失败",
                L"方法=" + std::to_wstring(方法节点.值) + L" 有读回=" + std::to_wstring(读回.has_value())
                + L" 读失败行=" + std::to_wstring(方法读失败行)
                + L" 配对数=" + std::to_wstring(读回 ? 读回->条件结果组.size() : 0) + L"/" + std::to_wstring(方法->条件结果组.size())); } catch (...) {}
            结果.状态 = 新方法学习发布状态::发布结果未知;
            return 结果;
        }
        const auto 新项 = std::find_if(读回->条件结果组.begin(), 读回->条件结果组.end(),
            [&](const auto& 项) { return 项.配对节点 == 新配对; });
        if (新项 == 读回->条件结果组.end()
            || std::count_if(读回->条件结果组.begin(), 读回->条件结果组.end(),
                [&](const auto& 项) { return 项.配对节点 == 新配对; }) != 1
            || 新项->命中次数 != 0 || 新项->条件 != 定义.条件 || 新项->结果 != 定义.结果
            || !std::is_permutation(新项->来源执行分支节点组.begin(),新项->来源执行分支节点组.end(),定义.来源执行分支节点组.begin(),定义.来源执行分支节点组.end())
            || !线性关系等价(新项->线性关系组, 定义.线性关系组)
            || !std::all_of(方法->条件结果组.begin(), 方法->条件结果组.end(),
                [&](const auto& 旧项) {
                    return std::count_if(读回->条件结果组.begin(), 读回->条件结果组.end(),
                        [&](const auto& 项) { return 项.配对节点 == 旧项.配对节点; }) == 1
                        && std::find(读回->条件结果组.begin(), 读回->条件结果组.end(), 旧项)
                            != 读回->条件结果组.end();
                })) {
            try { (void)记录运行日志(L"方法学习发布", L"读回比对不符",
                L"方法=" + std::to_wstring(方法节点.值) + L" 找到=" + std::to_wstring(新项 != 读回->条件结果组.end())
                + (新项 != 读回->条件结果组.end()
                    ? L" 条件同=" + std::to_wstring(新项->条件 == 定义.条件) + L" 结果同=" + std::to_wstring(新项->结果 == 定义.结果)
                        + L" 读回结果项数=" + std::to_wstring(新项->结果.结果项组.size())
                        + L" 作用存在=" + std::to_wstring(!新项->结果.结果项组.empty() && 新项->结果.结果项组.front().结果作用存在
                            ? 新项->结果.结果项组.front().结果作用存在->值 : 0)
                    : std::wstring{})); } catch (...) {}
            结果.状态 = 新方法学习发布状态::发布结果未知;
            return 结果;
        }
        结果.状态 = 新方法学习发布状态::已发布;
        return 结果;
    } catch (const 引用内容定义前提变化&) {
        if(!已发布) {
            回滚(已写字段,已建节点);
            结果.新配对节点组.clear();
        }
        // 引用的概念定义在本次读取与封存之间发生变化：属并发变化，交调用者按前提已变化重来。
        记录方法前提变化诊断(L"学习发布封存", L"方法=" + std::to_wstring(方法节点.值)
            + L"；已发布=" + std::to_wstring(已发布));
        结果.状态 = 已发布 ? 新方法学习发布状态::发布结果未知 : 新方法学习发布状态::前提已变化;
        return 结果;
    } catch (const std::bad_alloc&) {
        if (!已发布) {
            回滚(已写字段, 已建节点);
            结果.新配对节点组.clear();
        }
        结果.状态 = 已发布 ? 新方法学习发布状态::发布结果未知
            : 新方法学习发布状态::资源失败;
        return 结果;
    } catch (...) {
        // 结构读取或封存异常不是资源失败，如实报结构不一致。
        if (!已发布) {
            回滚(已写字段, 已建节点);
            结果.新配对节点组.clear();
        }
        结果.状态 = 已发布 ? 新方法学习发布状态::发布结果未知
            : 新方法学习发布状态::结构不一致;
        return 结果;
    }
}

新方法学习发布结果 新_方法类::融合模拟条件结果(
    稳定编码 方法节点, 稳定编码 新配对节点,
    std::vector<稳定编码>* 被替换配对组) noexcept {
    新方法学习发布结果 结果;
    {
        std::lock_guard 锁(新方法互斥);
        学习前确保配对数特征(方法节点);
        std::vector<稳定编码> 被替换;
        结果 = 融合模拟条件结果内部(方法节点, 新配对节点, &被替换);
        if (被替换配对组) *被替换配对组 = 被替换;
        if (结果.状态 == 新方法学习发布状态::已发布) {
            std::vector<稳定编码> 来源{新配对节点};
            来源.insert(来源.end(), 结果.新配对节点组.begin(), 结果.新配对节点组.end());
            来源.insert(来源.end(), 被替换.begin(), 被替换.end());
            学习后形成方法状态(方法节点, 新方法学习种类::融合, 来源);
        }
    }
    发出方法状态通知();
    return 结果;
}

新方法学习发布结果 新_方法类::融合模拟条件结果内部(
    稳定编码 方法节点, 稳定编码 新配对节点,
    std::vector<稳定编码>* 被替换配对组) noexcept {
    新方法学习发布结果 结果;
    结果.状态 = 新方法学习发布状态::精确重复;
    const auto 返回 = [&]([[maybe_unused]] int 行) -> const 新方法学习发布结果& {
#ifdef HY_EGO_DEBUG_LOG_PARAM_BINDING
        if (结果.状态 == 新方法学习发布状态::精确重复) try {
            std::wostringstream 文本;
            文本 << L"方法=" << 方法节点.值 << L" 新配对=" << 新配对节点.值 << L" 不融合出口行=" << 行;
            (void)调试日志<调试切片::PARAM_BINDING>(L"方法配对融合", L"不融合", 文本.str());
        } catch (...) {}
#endif
        return 结果;
    };
    if (被替换配对组) 被替换配对组->clear();
    if (!有效(方法节点) || !有效(新配对节点)) {
        结果.状态 = 新方法学习发布状态::入口拒绝;
        return 返回(__LINE__);
    }
    std::vector<稳定编码> 已建节点, 已写字段;
    bool 已发布 = false;
    try {
        std::lock_guard 锁(新方法互斥);
        if (!节点类型为(方法节点, 方法节点类型)) {
            结果.状态 = 新方法学习发布状态::方法不可用;
            return 返回(__LINE__);
        }
        const auto 方法 = 读取方法信息(方法节点, 特征概念服务_, *this, 存在概念服务_);
        if (!方法) {
            结果.状态 = 新方法学习发布状态::结构不一致;
            return 返回(__LINE__);
        }
        if (方法->来源方法节点 || 方法->实例根节点
            || 方法->实现方式 != 新方法实现方式::本能
            || 方法->启用状态 != 新方法启用状态::启用) {
            结果.状态 = 新方法学习发布状态::方法不可用;
            return 返回(__LINE__);
        }
        const auto 新项 = std::find_if(方法->条件结果组.begin(), 方法->条件结果组.end(),
            [&](const auto& 项) { return 项.配对节点 == 新配对节点; });
        if (新项 == 方法->条件结果组.end() || 新项->知识来源 != 新方法知识来源种类::模拟)
            return 返回(__LINE__);
        // 仅条件节点（没有结果，R96）不融合，等尝试学习取得结果后再判断方向（2026-11-06用户裁决）。
        const auto 无结果 = [](const 新方法结果定义& r) {
            return r.结果项组.empty() && r.参数关系组.empty() && r.场景事实限定组.empty();
        };
        if (无结果(新项->结果)) return 返回(__LINE__);
        using 区间 = 特征概念I64闭区间;
        // 条件只允许数值域不同；返回不同的条件项序号。
        const auto 条件差异 = [](const 新方法条件定义& a, const 新方法条件定义& b)
            -> std::optional<std::vector<std::size_t>> {
            if (a.关系规则 != b.关系规则 || a.参数关系组 != b.参数关系组
                || a.场景事实限定组 != b.场景事实限定组
                || a.条件项组.size() != b.条件项组.size()) return std::nullopt;
            std::vector<std::size_t> 差;
            for (std::size_t i = 0; i < a.条件项组.size(); ++i) {
                if (a.条件项组[i] == b.条件项组[i]) continue;
                auto x = a.条件项组[i], y = b.条件项组[i];
                if (!x.精确数值域 || !y.精确数值域) return std::nullopt;
                x.精确数值域.reset(); y.精确数值域.reset();
                if (x != y) return std::nullopt;
                差.push_back(i);
            }
            return 差;
        };
        const auto 结果可比 = [](const 新方法结果定义& a, const 新方法结果定义& b) {
            if (a.关系规则 != b.关系规则 || !(a.确认要求 == b.确认要求)
                || a.参数关系组 != b.参数关系组 || a.场景事实限定组 != b.场景事实限定组
                || a.结果项组.size() != b.结果项组.size()) return false;
            for (std::size_t j = 0; j < a.结果项组.size(); ++j) {
                if (a.结果项组[j] == b.结果项组[j]) continue;
                auto x = a.结果项组[j], y = b.结果项组[j];
                if (!x.精确数值域 || !y.精确数值域) return false;
                x.精确数值域.reset(); y.精确数值域.reset();
                if (x != y) return false;
            }
            return true;
        };
        std::map<std::size_t, std::vector<const 新方法条件结果信息*>> 候选;
        const 新方法条件结果信息* 吸收 = nullptr;
        const auto 包含 = [](const std::optional<区间>& 外, const std::optional<区间>& 内) {
            return 外 && 内 && 外->下界 <= 内->下界 && 内->上界 <= 外->上界;
        };
        for (const auto& p : 方法->条件结果组) {
            if (p.配对节点 == 新项->配对节点 || p.知识来源 != 新方法知识来源种类::模拟) continue;
            const auto 差 = 条件差异(新项->条件, p.条件);
            if (!差 || 差->size() != 1 || !结果可比(新项->结果, p.结果)) continue;
            const auto c = 差->front();
            // 新点落在已融合区间配对之内（条件与各变化结果均被包含）：并入该配对，不另存。
            bool 被包含 = 包含(p.条件.条件项组[c].精确数值域, 新项->条件.条件项组[c].精确数值域);
            for (std::size_t j = 0; 被包含 && j < p.结果.结果项组.size(); ++j)
                if (p.结果.结果项组[j] != 新项->结果.结果项组[j])
                    被包含 = 包含(p.结果.结果项组[j].精确数值域, 新项->结果.结果项组[j].精确数值域);
            if (被包含 && !吸收) 吸收 = &p;
            候选[c].push_back(&p);
        }
        if (吸收) {
            新方法条件结果定义 并入{吸收->条件, 吸收->结果, 吸收->线性关系组, 吸收->来源执行分支节点组};
            for (const auto B : 新项->来源执行分支节点组)
                if (std::find(并入.来源执行分支节点组.begin(), 并入.来源执行分支节点组.end(), B)
                    == 并入.来源执行分支节点组.end()) 并入.来源执行分支节点组.push_back(B);
            基础节点组追加请求 发布请求;
            for (const auto B : 并入.来源执行分支节点组)
                if (std::find(吸收->来源执行分支节点组.begin(), 吸收->来源执行分支节点组.end(), B)
                    == 吸收->来源执行分支节点组.end())
                    发布请求.追加字段组.push_back({吸收->配对节点, 配对分支来源字段, B});
            const auto 引用组 = 全局基础数据集.查询字段(方法节点, 方法条件结果字段);
            const auto 引用 = std::find_if(引用组.begin(), 引用组.end(), [&](const auto& f) {
                const auto* n = std::get_if<稳定编码>(&f.内容);
                return n && *n == 新项->配对节点;
            });
            if (引用 == 引用组.end()) {
                结果.状态 = 新方法学习发布状态::结构不一致;
                return 返回(__LINE__);
            }
            发布请求.删除字段关系组.push_back(引用->编码);
            const auto 封存 = 封存方法片段(std::move(发布请求), {方法节点, 吸收->配对节点},
                存在概念服务_, 特征概念服务_);
            const auto 发布 = 全局基础数据集.原子追加封存片段组({封存});
            if (发布.状态 != 基础节点组追加状态::已完成) {
                结果.状态 = 新方法学习发布状态::资源失败;
                return 返回(__LINE__);
            }
            已发布 = true;
            const auto 读回 = 读取方法信息本体(方法节点, 特征概念服务_, *this, 存在概念服务_, false, true);
            if (!读回 || std::any_of(读回->条件结果组.begin(), 读回->条件结果组.end(), [&](const auto& p) {
                    return p.配对节点 == 新项->配对节点; })
                || std::none_of(读回->条件结果组.begin(), 读回->条件结果组.end(), [&](const auto& p) {
                    return p.配对节点 == 吸收->配对节点 && p.条件 == 吸收->条件 && p.结果 == 吸收->结果; })) {
                结果.状态 = 新方法学习发布状态::发布结果未知;
                return 返回(__LINE__);
            }
            结果.新配对节点组 = {吸收->配对节点};
            if (被替换配对组) *被替换配对组 = {新项->配对节点};
            结果.状态 = 新方法学习发布状态::已发布;
            return 返回(__LINE__);
        }
#ifdef HY_EGO_DEBUG_LOG_PARAM_BINDING
        if (候选.empty()) try {
            std::wostringstream 文本;
            文本 << L"方法=" << 方法节点.值 << L" 新配对=" << 新配对节点.值 << L" 逐对:";
            int 已记 = 0;
            for (const auto& p : 方法->条件结果组) {
                if (p.配对节点 == 新项->配对节点 || 已记 >= 6) continue;
                ++已记;
                const auto 差 = 条件差异(新项->条件, p.条件);
                文本 << L" [" << p.配对节点.值 << L" 来源=" << static_cast<int>(p.知识来源)
                     << L" 条件差=" << (差 ? static_cast<long long>(差->size()) : -1LL)
                     << L" 结果可比=" << 结果可比(新项->结果, p.结果)
                     << L" 结果项数=" << p.结果.结果项组.size() << L"/" << 新项->结果.结果项组.size()
                     << L" 结果关系=" << p.结果.参数关系组.size() << L"/" << 新项->结果.参数关系组.size() << L"]";
            }
            (void)调试日志<调试切片::PARAM_BINDING>(L"方法配对融合", L"无候选明细", 文本.str());
        } catch (...) {}
#endif
        if (候选.empty()) return 返回(__LINE__);
        auto 最佳 = 候选.begin();
        for (auto it = 候选.begin(); it != 候选.end(); ++it)
            if (it->second.size() > 最佳->second.size()) 最佳 = it;
        const auto k = 最佳->first;
        std::vector<const 新方法条件结果信息*> 成员{&*新项};
        成员.insert(成员.end(), 最佳->second.begin(), 最佳->second.end());
        for (const auto* m : 成员)
            for (const auto& r : m->线性关系组) if (r.条件序号 != k) return 返回(__LINE__);
        const auto 条件域 = [&](const 新方法条件结果信息* m) { return *m->条件.条件项组[k].精确数值域; };
        std::sort(成员.begin(), 成员.end(), [&](const auto* a, const auto* b) {
            return 条件域(a).下界 < 条件域(b).下界;
        });
        for (std::size_t i = 1; i < 成员.size(); ++i)
            if (条件域(成员[i]).下界 <= 条件域(成员[i - 1]).上界) return 返回(__LINE__); // 条件区间重叠不融合
        新方法条件结果定义 合并定义{新项->条件, 新项->结果, {}, {}};
        const 区间 合并条件域{条件域(成员.front()).下界, 条件域(成员.back()).上界};
        合并定义.条件.条件项组[k].精确数值域 = 合并条件域;
        for (std::size_t j = 0; j < 合并定义.结果.结果项组.size(); ++j) {
            const bool 变化 = std::any_of(成员.begin(), 成员.end(), [&](const auto* m) {
                return m->结果.结果项组[j] != 新项->结果.结果项组[j];
            });
            if (!变化) continue; // 结果不变：条件融合、结果保持（用户裁决“结果不变算方向一致”）
            int 方向 = 0;
            const auto 记方向 = [&](int d) {
                if (d == 0) return false;
                if (方向 == 0) 方向 = d;
                return 方向 == d;
            };
            const auto 结果域 = [&](const 新方法条件结果信息* m) { return *m->结果.结果项组[j].精确数值域; };
            for (const auto* m : 成员) {
                const auto r = std::find_if(m->线性关系组.begin(), m->线性关系组.end(),
                    [&](const auto& x) { return x.条件序号 == k && x.结果序号 == j; });
                if (r != m->线性关系组.end()) {
                    if (!记方向(static_cast<int>(r->方向))) return 返回(__LINE__);
                } else if (条件域(m).下界 != 条件域(m).上界 && 结果域(m).下界 != 结果域(m).上界)
                    return 返回(__LINE__); // 区间配对无方向记录，无法判定
            }
            for (std::size_t i = 1; i < 成员.size(); ++i) {
                const auto 前 = 结果域(成员[i - 1]), 后 = 结果域(成员[i]);
                const int d = 后.下界 > 前.上界 ? 1 : 后.上界 < 前.下界 ? -1 : 0;
                // 某一段结果没有同步变化（相同或交叠）即方向不一致，不融合。
                if (!记方向(d)) return 返回(__LINE__);
            }
            区间 合并结果域 = 结果域(成员.front());
            for (const auto* m : 成员) {
                合并结果域.下界 = (std::min)(合并结果域.下界, 结果域(m).下界);
                合并结果域.上界 = (std::max)(合并结果域.上界, 结果域(m).上界);
            }
            合并定义.结果.结果项组[j].精确数值域 = 合并结果域;
            合并定义.线性关系组.push_back({k, j, 方向 > 0 ? 新方法条件结果线性方向::同向
                : 新方法条件结果线性方向::反向, 合并条件域, 合并结果域});
        }
        std::set<稳定编码> 分支;
        for (const auto* m : 成员) 分支.insert(m->来源执行分支节点组.begin(), m->来源执行分支节点组.end());
        合并定义.来源执行分支节点组.assign(分支.begin(), 分支.end());
        // 现行定义无法表达（如线性域含端点极值）时不融合，保留原配对。
        if (!条件结果有效(合并定义, 特征概念服务_)
            || !条件结果角色已声明(方法->参数规格组, 合并定义, 特征概念服务_, 存在概念服务_)
            || !配对分支来源有效(*方法, 合并定义, 特征概念服务_, 存在概念服务_)) return 返回(__LINE__);
        const 新方法条件结果信息* 保留 = nullptr;
        for (const auto* m : 成员)
            if (m->条件 == 合并定义.条件 && m->结果 == 合并定义.结果
                && 线性关系等价(m->线性关系组, 合并定义.线性关系组)) 保留 = m;
        基础节点组追加请求 发布请求;
        std::vector<稳定编码> 发布图{方法节点};
        稳定编码 新配对{};
        if (保留) {
            for (const auto B : 合并定义.来源执行分支节点组)
                if (std::find(保留->来源执行分支节点组.begin(), 保留->来源执行分支节点组.end(), B)
                    == 保留->来源执行分支节点组.end())
                    发布请求.追加字段组.push_back({保留->配对节点, 配对分支来源字段, B});
            发布图.push_back(保留->配对节点);
        } else {
            std::int64_t 最大顺序 = -1;
            for (const auto& 配对 : 方法->条件结果组) {
                const auto 顺序 = 读取唯一I64字段(配对.配对节点, 顺序字段);
                if (!顺序 || *顺序 < 0 || *顺序 == (std::numeric_limits<std::int64_t>::max)()) {
                    结果.状态 = 新方法学习发布状态::结构不一致;
                    return 返回(__LINE__);
                }
                最大顺序 = (std::max)(最大顺序, *顺序);
            }
            if (!写入条件结果(方法节点, 合并定义, 最大顺序 + 1, 已建节点, 已写字段, &新配对)
                || !写入I64字段(新配对, 配对知识来源字段, 1, 已写字段)) {
                回滚(已写字段, 已建节点);
                结果.状态 = 新方法学习发布状态::资源失败;
                return 返回(__LINE__);
            }
            发布请求.追加字段组.push_back({方法节点, 方法条件结果字段, 新配对});
            发布图.insert(发布图.end(), 已建节点.begin(), 已建节点.end());
        }
        // 只解除方法对被融合旧配对的引用；旧配对节点不物理删除（任务实例材料可能仍引用）。
        const auto 引用组 = 全局基础数据集.查询字段(方法节点, 方法条件结果字段);
        std::vector<稳定编码> 替换;
        for (const auto* m : 成员) {
            if (m == 保留) continue;
            const auto 引用 = std::find_if(引用组.begin(), 引用组.end(), [&](const auto& f) {
                const auto* n = std::get_if<稳定编码>(&f.内容);
                return n && *n == m->配对节点;
            });
            if (引用 == 引用组.end()) {
                回滚(已写字段, 已建节点);
                结果.状态 = 新方法学习发布状态::结构不一致;
                return 返回(__LINE__);
            }
            发布请求.删除字段关系组.push_back(引用->编码);
            替换.push_back(m->配对节点);
        }
        const auto 封存 = 封存方法片段(std::move(发布请求), 发布图, 存在概念服务_, 特征概念服务_);
        const auto 发布 = 全局基础数据集.原子追加封存片段组({封存});
        if (发布.状态 != 基础节点组追加状态::已完成) {
            回滚(已写字段, 已建节点);
            结果.状态 = 新方法学习发布状态::资源失败;
            return 返回(__LINE__);
        }
        已发布 = true;
        const auto 保留节点 = 保留 ? 保留->配对节点 : 新配对;
        const auto 读回 = 读取方法信息本体(方法节点, 特征概念服务_, *this, 存在概念服务_, false, true);
        if (!读回 || 读回->条件结果组.size() + 替换.size() != 方法->条件结果组.size() + (保留 ? 0u : 1u)
            || std::any_of(读回->条件结果组.begin(), 读回->条件结果组.end(), [&](const auto& p) {
                return std::find(替换.begin(), 替换.end(), p.配对节点) != 替换.end(); })
            || std::none_of(读回->条件结果组.begin(), 读回->条件结果组.end(), [&](const auto& p) {
                return p.配对节点 == 保留节点 && p.条件 == 合并定义.条件 && p.结果 == 合并定义.结果
                    && 线性关系等价(p.线性关系组, 合并定义.线性关系组); })) {
            结果.状态 = 新方法学习发布状态::发布结果未知;
            return 返回(__LINE__);
        }
        结果.新配对节点组 = {保留节点};
        if (被替换配对组) *被替换配对组 = 替换;
        结果.状态 = 新方法学习发布状态::已发布;
        return 返回(__LINE__);
    } catch (...) {
        if (!已发布) 回滚(已写字段, 已建节点);
        结果.新配对节点组.clear();
        结果.状态 = 已发布 ? 新方法学习发布状态::发布结果未知 : 新方法学习发布状态::资源失败;
        return 返回(__LINE__);
    }
}

新方法建立结果 新_方法类::确保本能方法登记(
    const std::string& 登记名,
    const 新方法知识建立请求& 原请求) noexcept {
    const auto 结果 = 确保本能方法登记内部(登记名, 原请求);
    发出方法状态通知();
    return 结果;
}

新方法建立结果 新_方法类::确保本能方法登记内部(
    const std::string& 登记名,
    const 新方法知识建立请求& 原请求) noexcept {
    try {
        auto 请求=原请求;
        规范内容关系声明(请求);
        if(请求.初始必要条件&&!规范化通用概念必要声明(*请求.初始必要条件))
            return {新方法操作状态::参数规格不合法,{}};
        if (登记名.empty() || !新_本能函数管理类::解析(登记名).函数) {
            return {新方法操作状态::函数登记不合法, std::nullopt};
        }
        std::vector<基础联合节点快照> 必要保护;
        if(请求.初始必要条件) {
            if(!参数规格有效(请求.参数规格组,特征概念服务_))return {新方法操作状态::参数规格不合法,{}};
            const auto 核验=核验通用概念必要声明(请求,存在概念服务_,特征概念服务_,必要保护);
            if(核验!=新方法操作状态::已找到)return {核验,{}};
        }
        std::unique_lock 锁(新方法互斥);
        if (!结构仍存在()) {
            return {新方法操作状态::尚未初始化, std::nullopt};
        }
        核验治理组织就绪();
        if (!行为声明有效(请求) || 请求.实现方式 != 新方法实现方式::本能
            || !是作用域(static_cast<std::int64_t>(请求.作用域))) {
            return {新方法操作状态::方法类型不符, std::nullopt};
        }
        if (!方法头有效(请求.方法头)) {
            return {新方法操作状态::方法头不合法, std::nullopt};
        }
        if (!参数规格有效(请求.参数规格组, 特征概念服务_)) {
            return {新方法操作状态::参数规格不合法, std::nullopt};
        }
        for (const auto& 参数 : 请求.参数规格组) {
            if (参数.有序规格 && 是特征持有参数(参数.有序规格->信息类型)
                && !方法持有者范围有效(*参数.有序规格,存在概念服务_))
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"登记有序参数", L"存在参数的范围概念不可读取。");
        }
        // R97（5350 §2）：方法头涉及特征类型集合由有序规格汇总，不另行声明。
        if (!请求.参数规格组.empty() && 请求.参数规格组.front().有序规格) {
            std::set<稳定编码> 汇总;
            for (const auto& 参数 : 请求.参数规格组) {
                const auto& 有序 = *参数.有序规格;
                if (是特征持有参数(有序.信息类型))
                    汇总.insert(有序.必需特征类型组.begin(), 有序.必需特征类型组.end());
                else 汇总.insert(有序.范围概念节点);
            }
            const std::set<稳定编码> 头集合(请求.方法头.涉及特征类型概念组.begin(),
                请求.方法头.涉及特征类型概念组.end());
            if (头集合 != 汇总)
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"登记有序参数",
                    L"方法头涉及特征类型与有序规格汇总不一致。");
        }
        const auto 已有 = 查询本能登记结构(登记名, 特征概念服务_, *this, 存在概念服务_);
        if (已有.状态 == 新方法操作状态::已找到 && 已有.方法节点) {
            const auto 方法 = 读取方法信息本体(*已有.方法节点, 特征概念服务_, *this, 存在概念服务_, false, true);
            if (!方法) return {新方法操作状态::结构不一致, std::nullopt};
            if (!内部治理世界_->读取方法治理场景({方法->节点, 方法->方法虚拟存在节点, 方法->启用特征节点}))
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"复用本能方法", L"已登记本能方法没有完整治理场景。");
            if (登记名 == 新本能函数登记名_旧两参数获取服务对象 && 方法->参数规格组 != 请求.参数规格组) {
                if (!是服务对象空头前代(*方法, 请求.参数规格组)
                    || 方法->方法头 != 请求.方法头 || !请求.条件结果组.empty()
                    || 方法->启用状态 != 新方法启用状态::启用
                    || (方法->行为 && 方法->行为 != 请求.行为))
                    工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"迁移获取服务对象参数",
                        L"旧方法不是可完整识别的空配对头；保留已有方法知识和执行记录。");
                const auto 发布 = 发布方法结构(请求, &登记名, &*方法, &必要保护);
                const auto 当前 = 查询本能登记结构(登记名, 特征概念服务_, *this, 存在概念服务_);
                if (发布.状态 != 新方法操作状态::已建立 || !发布.方法节点
                    || 当前.状态 != 新方法操作状态::已找到 || 当前.方法节点 != 发布.方法节点)
                    工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"迁移获取服务对象参数", L"新方法发表或读回不完整。");
                return 发布;
            }
            // 方案D（2026-11-12用户裁决，R119）：有序规格方法只在参数范围上不同（序号、信息类型、必需特征类型、
            // 所属位置均同，方法头/作用域/实现方式/启用均同）时允许修订参数规格；其它不一致仍按下方工程终止。
            if (!请求.参数规格组.empty() && 请求.参数规格组.front().有序规格
                && 方法->参数规格组 != 请求.参数规格组
                && 方法->参数规格组.size() == 请求.参数规格组.size()
                && 方法->作用域 == 请求.作用域 && 方法->实现方式 == 请求.实现方式
                && 方法->启用状态 == 新方法启用状态::启用
                && std::is_permutation(方法->方法头.作用存在概念组.begin(), 方法->方法头.作用存在概念组.end(),
                    请求.方法头.作用存在概念组.begin(), 请求.方法头.作用存在概念组.end())
                && std::is_permutation(方法->方法头.涉及特征类型概念组.begin(), 方法->方法头.涉及特征类型概念组.end(),
                    请求.方法头.涉及特征类型概念组.begin(), 请求.方法头.涉及特征类型概念组.end())
                && std::is_permutation(方法->方法头.可能结果概念组.begin(), 方法->方法头.可能结果概念组.end(),
                    请求.方法头.可能结果概念组.begin(), 请求.方法头.可能结果概念组.end())
                && [&] {
                    for (std::size_t i = 0; i < 请求.参数规格组.size(); ++i) {
                        const auto& 旧 = 方法->参数规格组[i];
                        const auto& 新 = 请求.参数规格组[i];
                        if (!旧.有序规格 || !新.有序规格) return false;
                        auto 仅范围 = *旧.有序规格; 仅范围.范围概念节点 = 新.有序规格->范围概念节点;
                        auto 旧其它 = 旧; 旧其它.有序规格 = 仅范围;
                        if (旧其它 != 新) return false;
                    }
                    return true;
                }()) {
                const auto 修订 = 修订本能参数范围已加锁(*方法, 请求.参数规格组);
                if (!修订.方法节点) return 修订;
                锁.unlock();
                // 修订后按复用口径继续核验初始声明补齐（与下方同一路径）。
                return 确保本能方法登记内部(登记名, 原请求);
            }
            if (方法->作用域 != 请求.作用域
                || 方法->实现方式 != 请求.实现方式
                || 方法->参数规格组 != 请求.参数规格组
                || !std::is_permutation(方法->方法头.作用存在概念组.begin(),
                    方法->方法头.作用存在概念组.end(),
                    请求.方法头.作用存在概念组.begin(), 请求.方法头.作用存在概念组.end())
                || !std::is_permutation(方法->方法头.涉及特征类型概念组.begin(),
                    方法->方法头.涉及特征类型概念组.end(),
                    请求.方法头.涉及特征类型概念组.begin(),
                    请求.方法头.涉及特征类型概念组.end())
                || !std::is_permutation(方法->方法头.可能结果概念组.begin(),
                    方法->方法头.可能结果概念组.end(),
                    请求.方法头.可能结果概念组.begin(), 请求.方法头.可能结果概念组.end())
                || 方法->启用状态 != 新方法启用状态::启用) {
                if (登记名 == 新本能函数登记名_旧两参数获取服务对象
                    || (!请求.参数规格组.empty() && 请求.参数规格组.front().有序规格))
                    工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"复用有序参数方法", L"函数声明与已固定的方法规格不一致。");
                return {新方法操作状态::函数登记冲突, std::nullopt};
            }

            锁.unlock();
            const auto 闭包=读取方法执行结构闭包({方法->节点});
            if (闭包.状态!=执行结构读取状态::已读取 || 闭包.方法组.size()!=1
                || 闭包.方法组.front()!=*方法) return {新方法操作状态::结构不一致,{}};
            初始声明补齐候选 候选;
            const auto 核验=核验初始声明补齐(*方法,请求,特征概念服务_,存在概念服务_,候选);
            if (核验!=新方法操作状态::已找到) return {核验,{}};
            候选.追加.预期联合节点组.insert(候选.追加.预期联合节点组.end(),
                闭包.完整保护组.begin(),闭包.完整保护组.end());
            return 发布初始声明补齐(候选,特征概念服务_,*this,存在概念服务_);
        }
        if (已有.状态 != 新方法操作状态::方法不存在) return 已有;
        // 本能登记允许尚无运行知识的头；非空配对仍必须完整合法。
        if (!std::all_of(请求.条件结果组.begin(), 请求.条件结果组.end(),
                [&](const 新方法条件结果定义& 定义) {
                    return 条件结果有效(定义, 特征概念服务_)
                        && 条件结果角色已声明(请求.参数规格组, 定义, 特征概念服务_, 存在概念服务_);
                })) {
            return {新方法操作状态::条件结果不合法, std::nullopt};
        }
        return 发布方法结构(请求, &登记名, nullptr, &必要保护);
    } catch (const 引用内容定义前提变化&) {
        return {新方法操作状态::定义前提已变化, std::nullopt};
    } catch (...) {
        return {新方法操作状态::资源失败, std::nullopt};
    }
}

新方法建立结果 新_方法类::修订本能参数范围已加锁(const 新方法信息& 根方法,
    const std::vector<新方法参数规格>& 新规格组) {
  try {
    // 调用者持有新方法互斥（递归锁）。来源方法族一并修订：族内方法与根共享同一固定规格。
    const auto 成员组 = 读取多节点字段(新方法组织根节点, 方法成员字段, 特征概念服务_);
    if (!成员组) throw std::runtime_error("method root unreadable for parameter range revision");
    std::map<稳定编码, 新方法信息> 全部;
    for (const auto n : *成员组) {
        const auto m = 读取方法信息本体(n, 特征概念服务_, *this, 存在概念服务_, false, true);
        if (!m || !全部.emplace(n, *m).second)
            throw std::runtime_error("method enumeration incomplete for parameter range revision");
    }
    if (!全部.contains(根方法.节点) || 全部.at(根方法.节点) != 根方法) return {新方法操作状态::定义前提已变化, {}};
    std::set<稳定编码> 族{根方法.节点};
    for (bool 变化 = true; 变化;) {
        变化 = false;
        for (const auto& [n, m] : 全部)
            if (!族.contains(n) && ((m.来源方法节点 && 族.contains(*m.来源方法节点))
                || (m.父代方法节点 && 族.contains(*m.父代方法节点)))) { 族.insert(n); 变化 = true; }
    }
    for (const auto& 规格 : 新规格组)
        if (规格.有序规格 && 是特征持有参数(规格.有序规格->信息类型)
            && !方法持有者范围有效(*规格.有序规格, 存在概念服务_))
            throw std::runtime_error("revised parameter range unreadable");
    基础节点组追加请求 请求;
    std::vector<稳定编码> 图;
    std::map<稳定编码, std::vector<稳定编码>> 删除配对;
    std::map<稳定编码, std::size_t> 预期配对数;
    for (const auto n : 族) {
        const auto& m = 全部.at(n);
        if (m.参数规格组 != 根方法.参数规格组)
            throw std::runtime_error("source family method has parameter specs differing from its root");
        图.push_back(n); 图.push_back(m.方法虚拟存在节点);
        const auto 参数组 = 读取有序子节点(m.方法虚拟存在节点, 虚拟存在参数规格字段, 特征概念服务_);
        if (!参数组 || 参数组->size() != 新规格组.size())
            throw std::runtime_error("method parameter spec nodes unreadable for revision");
        for (std::size_t i = 0; i < 新规格组.size(); ++i) {
            if (m.参数规格组[i] == 新规格组[i]) continue;
            const auto P = (*参数组)[i];
            if (读取参数规格(P, 特征概念服务_, 存在概念服务_) != std::optional{m.参数规格组[i]})
                throw std::runtime_error("method parameter spec node differs from projection");
            const auto 字段 = 全局基础数据集.查询字段(P, 有序参数范围字段);
            if (字段.size() != 1) throw std::runtime_error("ordered parameter range field not unique");
            请求.替换字段组.push_back({字段.front().编码, 新规格组[i].有序规格->范围概念节点});
            图.push_back(P);
            // 替换写入的新目标与旧目标同按严格联合预期（旧目标经P字段并入）。
            if (std::find(图.begin(), 图.end(), 新规格组[i].有序规格->范围概念节点) == 图.end())
                图.push_back(新规格组[i].有序规格->范围概念节点);
        }
        // 不相容配对：引用的参数位置或条件值域与新规格对不上（按登记同一口径核验）；相容的保留。
        auto 新方法 = m; 新方法.参数规格组 = 新规格组;
        const auto 引用组 = 全局基础数据集.查询字段(n, 方法条件结果字段);
        for (const auto& 配对 : m.条件结果组) {
            const 新方法条件结果定义 定义{配对.条件, 配对.结果, 配对.线性关系组, 配对.来源执行分支节点组};
            if (条件结果角色已声明(新规格组, 定义, 特征概念服务_, 存在概念服务_)
                && 配对分支来源有效(新方法, 定义, 特征概念服务_, 存在概念服务_)) continue;
            const auto 引用 = std::find_if(引用组.begin(), 引用组.end(), [&](const auto& f) {
                const auto* x = std::get_if<稳定编码>(&f.内容);
                return x && *x == 配对.配对节点;
            });
            if (引用 == 引用组.end()) throw std::runtime_error("method pair reference missing for revision");
            // 复用融合路径：只解除方法对配对的引用；配对节点不物理删除（执行材料可能仍引用）。
            请求.删除字段关系组.push_back(引用->编码);
            删除配对[n].push_back(配对.配对节点);
        }
        预期配对数[n] = m.条件结果组.size() - 删除配对[n].size();
    }
    const auto 封存 = 封存方法片段(std::move(请求), 图, 存在概念服务_, 特征概念服务_);
    const auto 发布 = 全局基础数据集.原子追加封存片段组({封存});
    if (发布.状态 == 基础节点组追加状态::预期不符) return {新方法操作状态::定义前提已变化, {}};
    if (发布.状态 != 基础节点组追加状态::已完成)
        throw std::runtime_error("method parameter range revision publication failed");
    for (const auto n : 族) {
        const auto 读回 = 读取方法信息本体(n, 特征概念服务_, *this, 存在概念服务_, false, true);
        if (!读回 || 读回->参数规格组 != 新规格组 || 读回->条件结果组.size() != 预期配对数.at(n))
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"修订本能参数范围",
                L"参数范围修订已发布但读回不一致；保留已发布事实。");
    }
    // 删除后按实际数经方法状态统一入口同步配对数（值不变不形成状态）；治理结构未建时由启动补齐按实际数建立。
    if (有效(方法状态治理根))
        for (const auto n : 族) 同步方法配对数状态已加锁(n, 新方法学习种类::规格修订删除, 删除配对[n]);
    return {新方法操作状态::已修改, 根方法.节点};
  } catch (const 引用内容定义前提变化&) {
    throw;
  } catch (const std::exception& e) {
    工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"修订本能参数范围",
        L"参数范围修订的方法族、规格节点或配对引用读取不完整；保留已有事实。", e.what());
  }
}

新方法建立结果 新_方法类::退役旧服务对象需求登记(
    const 新方法知识建立请求& 原预期) noexcept {
    try {
        auto expected=原预期;
        if(expected.初始必要条件&&!规范化通用概念必要声明(*expected.初始必要条件))
            throw std::runtime_error("legacy demand expected declaration invalid");
        if(expected.作用域!=新方法作用域::普通世界||expected.实现方式!=新方法实现方式::本能
            ||expected.行为!=新方法行为::改变||expected.参数规格组.size()!=3
            ||!expected.条件结果组.empty()||!expected.初始必要条件)
            throw std::runtime_error("legacy demand migration declaration invalid");
        for(std::size_t i=0;i<3;++i) {
            const auto& spec=expected.参数规格组[i].有序规格;
            if(!spec||spec->序号!=i+1||spec->信息类型!=(i==1?新方法参数信息类型::特征:新方法参数信息类型::存在))
                throw std::runtime_error("legacy demand expected ordered declaration invalid");
            if(i!=1&&!存在概念服务_.获取存在概念(spec->范围概念节点))
                throw std::runtime_error("legacy demand migration requires ordinary existence ranges");
        }
        std::lock_guard lock(新方法互斥);
        if(!结构仍存在())return {新方法操作状态::尚未初始化,{}};
        核验治理组织就绪();
        const auto members=读取多节点字段(新方法组织根节点,方法成员字段,特征概念服务_);
        if(!members)
            throw std::runtime_error("legacy demand full method root unreadable");
        std::map<稳定编码,新方法信息> all;
        std::set<稳定编码> family;
        std::optional<稳定编码> root;
        const std::string name="builtin.service_object.demand.acquire_new.v1";
        for(const auto n:*members) {
            const auto m=读取方法信息本体(n,特征概念服务_,*this,存在概念服务_,false,true);
            if(!m||!all.emplace(n,*m).second)throw std::runtime_error("legacy demand family enumeration incomplete");
            if(m->本能函数登记名==name&&!m->来源方法节点&&!m->父代方法节点) {
                if(root)throw std::runtime_error("legacy demand registration is ambiguous");
                root=n;family.insert(n);
            }
        }
        if(!root) {
            for(const auto& [n,m]:all)if(m.本能函数登记名==name)
                throw std::runtime_error("legacy demand family lacks root");
            return {新方法操作状态::无变化,{}};
        }
        bool changed=true;
        while(changed) {
            changed=false;
            for(const auto& [n,m]:all)if(!family.contains(n)
                &&((m.来源方法节点&&family.contains(*m.来源方法节点))
                    ||(m.父代方法节点&&family.contains(*m.父代方法节点)))) {
                family.insert(n);changed=true;
            }
        }
        const auto matches=[&](const 新方法信息& m) {
            if(m.作用域!=expected.作用域||m.实现方式!=expected.实现方式||m.行为!=expected.行为
                ||m.本能函数登记名!=name||m.方法头!=expected.方法头||m.参数规格组!=expected.参数规格组
                ||m.初始必要条件!=expected.初始必要条件||m.初始模拟关系组!=expected.初始模拟关系组
                ||m.初始模拟事件参数组!=expected.初始模拟事件参数组
                ||m.初始模拟输入方案组!=expected.初始模拟输入方案组
                ||m.执行分支组.size()!=expected.执行分支组.size()
                ||!m.组合步骤组.empty()||m.组合入口步骤节点||!m.结果转移组.empty())return false;
            for(std::size_t i=0;i<m.执行分支组.size();++i)
                if(m.执行分支组[i].定义!=expected.执行分支组[i])return false;
            return true;
        };
        for(const auto& [n,m]:all) {
            if(m.本能函数登记名==name&&!family.contains(n))throw std::runtime_error("legacy demand disconnected source family");
            if(family.contains(n)&&(!matches(m)
                ||!内部治理世界_->读取方法治理场景({m.节点,m.方法虚拟存在节点,m.启用特征节点})))
                throw std::runtime_error("legacy demand declaration or governance scene differs");
        }
        for(const auto n:family) {
            const auto before=读取方法信息本体(n,特征概念服务_,*this,存在概念服务_,false,true);
            if(before!=std::optional{all.at(n)})throw std::runtime_error("legacy demand family changed before retirement");
            const auto status=修改启用状态(n,新方法启用状态::停用);
            if(status!=新方法操作状态::已修改&&status!=新方法操作状态::无变化)
                throw std::runtime_error("legacy demand retirement incomplete");
            auto retired=all.at(n);retired.启用状态=新方法启用状态::停用;
            if(读取方法信息本体(n,特征概念服务_,*this,存在概念服务_,false,true)!=std::optional{retired})
                throw std::runtime_error("legacy demand retirement readback differs");
        }
        return {新方法操作状态::登记已退役,root};
    } catch(...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"退役旧服务对象需求登记",L"旧登记声明、完整来源族或停用读回不完整，保留已有变化及历史材料。");
    }
}

新方法建立结果 新_方法类::退役旧网页本能登记(const std::string& 登记名) noexcept {
    const auto 结果 = 退役旧网页本能登记内部(登记名);
    发出方法状态通知();
    return 结果;
}

新方法建立结果 新_方法类::退役旧网页本能登记内部(const std::string& 登记名) noexcept {
    // 网页对话三项已删除（5380 v1.10 §3.8，AGENTS §5 旧代码处置），只剩旧两参数获取服务对象的退役。
    if (登记名 != 新本能函数登记名_旧两参数获取服务对象)
        return {新方法操作状态::函数登记不合法, std::nullopt};
    新特征I64准备结果 准备;
    std::optional<稳定编码> 前状态;
    bool 零发表 = true;
    try {
        std::lock_guard 锁(新方法互斥);
        if (!结构仍存在()) return {新方法操作状态::尚未初始化, std::nullopt};
        核验治理组织就绪();
        const auto 已有 = 查询本能登记结构(登记名, 特征概念服务_, *this, 存在概念服务_);
        if (已有.状态 == 新方法操作状态::方法不存在) return 已有;
        if (已有.状态 == 新方法操作状态::登记已退役) return 已有;
        if (已有.状态 != 新方法操作状态::已找到 || !已有.方法节点)
            throw std::runtime_error("legacy method registration unreadable");
        const auto 成员 = 读取多节点字段(新方法组织根节点, 方法成员字段, 特征概念服务_);
        if (!成员) throw std::runtime_error("legacy method root unreadable");
        std::vector<新方法信息> 家族;
        for (const auto n : *成员) {
            const auto m = 读取方法信息本体(n, 特征概念服务_, *this, 存在概念服务_, false, true);
            if (!m) throw std::runtime_error("legacy method member unreadable");
            if (!m->来源方法节点 && m->本能函数登记名 == 登记名) 家族.push_back(*m);
        }
        const auto& 末代 = 核验服务对象方法族(家族);
        if (末代.节点 != *已有.方法节点 || 末代.启用状态 != 新方法启用状态::启用)
            throw std::runtime_error("legacy method terminal changed");
        const 新方法虚拟存在引用 引用{末代.节点, 末代.方法虚拟存在节点, 末代.启用特征节点};
        if (!内部治理世界_->读取方法治理场景(引用))
            throw std::runtime_error("legacy method governance scene missing");
        const auto 前时间 = 方法当前UTC纳秒();
        const auto s0 = 状态服务_.建立状态(引用.虚拟存在节点, 引用.启用特征节点,
            {新状态时间语义::实例绝对UTC, 前时间});
        if (s0.状态 != 新状态操作状态::已完成 || !s0.状态节点)
            throw std::runtime_error("legacy retirement before state unavailable");
        前状态 = s0.状态节点;
        准备 = 特征服务_.准备顶层I64更新(引用.启用特征节点, 0);
        if (准备.状态 != 新特征I64准备状态::已准备 || !准备.准备
            || 准备.准备->原当前值() != 1 || !准备.准备->核心有变化())
            throw std::runtime_error("legacy retirement preparation failed");
        基础节点组追加请求 请求;
        std::vector<稳定编码> 保护组;
        for (const auto& m : 家族) {
            请求.追加字段组.push_back({新方法组织根节点, 方法旧网页退役成员字段, m.节点});
            保护组.push_back(m.节点);
        }
        const auto 片段 = 封存方法片段(std::move(请求), 保护组, 存在概念服务_,特征概念服务_);
        const auto 动作时间 = 方法当前UTC纳秒();
        零发表 = false;
        const auto 发布 = 特征服务_.提交顶层I64更新(*准备.准备, {片段});
        if (发布.状态 != 基础节点组追加状态::已完成) {
            if (!发布.附加片段回执组.empty()) throw std::runtime_error("retirement publication ambiguous");
            零发表 = true;
            throw std::runtime_error("retirement zero publication");
        }
        if (发布.附加片段回执组.size() != 1 || !发布.附加片段回执组[0].对应片段(片段)
            || 发布.附加片段回执组[0].追加字段关系组().size() != 家族.size()
            || !发布.附加片段回执组[0].新节点组().empty()
            || !发布.附加片段回执组[0].追加外部关系组().empty())
            throw std::runtime_error("retirement receipts incomplete");
        const auto 完成 = 特征服务_.完成顶层I64更新(*准备.准备);
        if (完成.状态 != 新特征写入状态::已完成 || !完成.当前值已改变)
            throw std::runtime_error("retirement finalization failed");
        特征服务_.结束顶层I64更新(*准备.准备);
        同步方法专属概念(引用);
        (void)前时间;
        // METHOD-LEARN-STATE：退役的启用修改经方法owner统一入口形成治理动作并交付因果。
        形成方法状态变化已加锁(引用, 新方法学习种类::启用修改, 新方法状态特征种类::启用,
            引用.启用特征节点, 1, 0, *前状态, 动作时间, {}, {});
        const auto 读回 = 查询本能登记结构(登记名, 特征概念服务_, *this, 存在概念服务_);
        if (读回.状态 != 新方法操作状态::登记已退役 || 读回.方法节点 != 末代.节点)
            throw std::runtime_error("retirement final readback failed");
        return 读回;
    } catch (const 引用内容定义前提变化&) {
        return {新方法操作状态::定义前提已变化,{}};
    } catch (...) {
        if (零发表) {
            if (准备.准备) 特征服务_.取消顶层I64更新(*准备.准备);
            if (前状态 && 状态服务_.删除状态(*前状态) != 新状态操作状态::已完成)
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"退役旧网页本能", L"零发表状态清理失败。");
        }
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"退役旧网页本能",
            L"旧方法族、退役依据或治理变化不完整，保留现场。");
    }
}

新方法建立结果 新_方法类::查询本能登记(
    const std::string& 登记名) const noexcept {
    if (登记名.empty()) return {新方法操作状态::函数登记不合法, std::nullopt};
    try {
        std::lock_guard 锁(新方法互斥);
        return 查询本能登记结构(登记名, 特征概念服务_, *this, 存在概念服务_);
    } catch (const 引用内容定义前提变化&) {
        return {新方法操作状态::定义前提已变化, std::nullopt};
    } catch (...) {
        return {新方法操作状态::资源失败, std::nullopt};
    }
}



新方法操作状态 新_方法类::登记本能函数(
    稳定编码 方法节点,
    const std::string& 本能函数登记名) noexcept {
    if (!有效(方法节点) || 本能函数登记名.empty()) {
        return 新方法操作状态::函数登记不合法;
    }
    稳定编码 新关系;
    try {
        const auto 函数 = 新_本能函数管理类::解析(本能函数登记名);
        if (!函数.函数) return 新方法操作状态::函数登记不合法;

        std::lock_guard 锁(新方法互斥);
        if (!结构仍存在()) return 新方法操作状态::尚未初始化;
        const auto 方法 = 读取方法信息本体(方法节点, 特征概念服务_, *this, 存在概念服务_, false, true);
        if (!方法) return 新方法操作状态::方法不存在;
        if (方法->实现方式 != 新方法实现方式::本能) {
            return 新方法操作状态::方法类型不符;
        }
        if (方法->本能函数登记名) {
            return *方法->本能函数登记名 == 本能函数登记名
                ? 新方法操作状态::无变化
                : 新方法操作状态::函数登记冲突;
        }

        新关系 = 全局基础数据集.添加字段值(
            方法节点, 方法本能函数登记名字段,
            基础值{本能函数登记名});
        if (!有效(新关系)) {
            return 新方法操作状态::资源失败;
        }
        const auto 读回 = 读取方法信息本体(方法节点, 特征概念服务_, *this, 存在概念服务_, false, true);
        if (!读回 || !读回->本能函数登记名
            || *读回->本能函数登记名 != 本能函数登记名) {
            (void)全局基础数据集.删除字段(新关系);
            return 新方法操作状态::结构不一致;
        }
        return 新方法操作状态::已修改;
    } catch (const 引用内容定义前提变化&) {
        return 新方法操作状态::定义前提已变化;
    } catch (...) {
        if (有效(新关系)) (void)全局基础数据集.删除字段(新关系);
        return 新方法操作状态::资源失败;
    }
}

新方法目录读取结果 新_方法类::读取方法目录() const noexcept {
    using 状态 = 新方法目录读取状态;
    const auto 非成功 = [](状态 值) {
        新方法目录读取结果 结果; 结果.状态 = 值; return 结果;
    };
    try {
        std::lock_guard 锁(新方法互斥);
        const auto 架构 = 架构编码组();
        if (std::none_of(架构.begin(), 架构.end(), [](const auto* 键) { return 有效(*键); }))
            return 非成功(状态::尚未初始化);
        if (!结构仍存在()) return 非成功(状态::结构不一致);
        const auto 成员 = 读取多节点字段(新方法组织根节点, 方法成员字段, 特征概念服务_);
        if (!成员) return 非成功(状态::结构不一致);
        const auto 唯一持有 = [](稳定编码 节点, 稳定编码 字段, 稳定编码 持有者) {
            const auto 归属 = 全局基础数据集.查询目标字段(节点, 字段);
            return 归属.size() == 1 && 归属.front().所属节点 == 持有者;
        };
        新方法目录读取结果 结果;
        for (const auto M : *成员) {
            if (!节点类型为(M, 方法节点类型) || !方法属于根(M)) return 非成功(状态::结构不一致);
            const auto 作用域 = 读取唯一I64字段(M, 方法作用域字段);
            const auto 实现方式 = 读取唯一I64字段(M, 方法实现方式字段);
            const auto 头 = 读取唯一节点字段(M, 方法头字段);
            const auto Vm = 读取唯一节点字段(M, 方法虚拟存在字段);
            if (!作用域 || !是作用域(*作用域) || !实现方式 || !是实现方式(*实现方式)
                || !头 || !节点类型为(*头, 方法头节点类型) || !唯一持有(*头, 方法头字段, M)
                || !Vm || !节点类型为(*Vm, 方法虚拟存在节点类型) || !唯一持有(*Vm, 方法虚拟存在字段, M)
                || 读取唯一节点字段(*Vm, 虚拟存在所属方法字段) != std::optional{M}
                || !全局基础数据集.查询字段(M, 方法启用状态字段).empty())
                return 非成功(状态::结构不一致);
            const auto F = 读取唯一节点字段(*Vm, 虚拟存在启用特征字段);
            if (!F || !唯一持有(*F, 虚拟存在启用特征字段, *Vm)) return 非成功(状态::结构不一致);
            const auto 启用 = 特征服务_.捕获特征匹配实例(*F);
            const auto* 值 = 启用 && 启用->当前值 ? std::get_if<std::int64_t>(&*启用->当前值) : nullptr;
            if (!启用 || 启用->上级特征节点 || 启用->实例单位 || !值 || (*值 != 0 && *值 != 1)
                || 启用->定义.特征类型根 != 方法启用特征类型
                || 启用->定义.材料类型 != 特征概念材料物理类型::I64标量)
                return 非成功(状态::结构不一致);
            const auto 头存在 = 读取多节点字段(*头, 方法头作用存在概念字段, 特征概念服务_);
            const auto 头特征 = 读取多节点字段(*头, 方法头特征类型概念字段, 特征概念服务_);
            const auto 头结果 = 读取多节点字段(*头, 方法头结果概念字段, 特征概念服务_);
            if (!头存在 || !头特征 || !头结果) return 非成功(状态::结构不一致);
            新方法目录项 项;
            项.节点 = M; 项.方法头节点 = *头; 项.方法虚拟存在节点 = *Vm;
            项.作用域 = static_cast<新方法作用域>(*作用域);
            项.实现方式 = static_cast<新方法实现方式>(*实现方式);
            项.启用状态 = static_cast<新方法启用状态>(*值);
            项.方法头 = {*头存在, *头特征, *头结果};
            if (!方法头有效(项.方法头)) return 非成功(状态::结构不一致);
            const auto 参数组 = 读取有序子节点(*Vm, 虚拟存在参数规格字段, 特征概念服务_);
            const auto 配对组 = 读取有序子节点(M, 方法条件结果字段, 特征概念服务_);
            if (!参数组 || !配对组) return 非成功(状态::结构不一致);
            for (const auto P : *参数组) {
                if (!唯一持有(P, 虚拟存在参数规格字段, *Vm)) return 非成功(状态::结构不一致);
                const auto 规格 = 读取参数规格(P, 特征概念服务_, 存在概念服务_);
                if (!规格) return 非成功(状态::结构不一致);
                项.参数规格组.push_back(*规格);
            }
            if (!参数规格有效(项.参数规格组, 特征概念服务_)) return 非成功(状态::结构不一致);
            for (const auto P : *配对组)
                if (!节点类型为(P, 条件结果配对节点类型) || !唯一持有(P, 方法条件结果字段, M))
                    return 非成功(状态::结构不一致);
            项.条件结果配对数量 = static_cast<std::uint64_t>(配对组->size());
            const auto 来源 = 读取可选节点字段(M, 方法来源字段);
            const auto 实例根 = 读取可选节点字段(M, 方法实例根字段);
            if (!来源 || !实例根 || 有效(*来源) != 有效(*实例根)) return 非成功(状态::结构不一致);
            if (有效(*来源)) {
                if (*来源 == M || !节点类型为(*来源, 方法节点类型) || !方法属于根(*来源)
                    || !全局基础数据集.查询字段(*来源, 方法来源字段).empty()
                    || !全局基础数据集.查询字段(*来源, 方法实例根字段).empty())
                    return 非成功(状态::结构不一致);
                const bool 单方法根 = 节点类型为(*实例根, 方法节点类型) && 方法属于根(*实例根)
                    && 读取唯一节点字段(*实例根, 方法实例根字段) == 实例根;
                const bool 混合根 = 节点类型为(*实例根, 混合方法实例根类型)
                    && 唯一持有(*实例根, 混合方法实例成员字段, 新方法组织根节点);
                if (!单方法根 && !混合根) return 非成功(状态::结构不一致);
                项.来源方法节点 = *来源; 项.实例根节点 = *实例根;
            }
            if (有效(方法父代字段)) {
                const auto 父代 = 读取可选节点字段(M, 方法父代字段);
                if (!父代) return 非成功(状态::结构不一致);
                if (有效(*父代)) {
                    if (*父代 == M || !节点类型为(*父代, 方法节点类型) || !方法属于根(*父代))
                        return 非成功(状态::结构不一致);
                    项.父代方法节点 = *父代;
                }
            }
            if (有效(方法行为字段)) {
                const auto 行为 = 读取可选I64字段(M, 方法行为字段);
                if (!行为 || (*行为 && !是方法行为(**行为))) return 非成功(状态::结构不一致);
                if (*行为) 项.行为 = static_cast<新方法行为>(**行为);
            }
            const auto 函数组 = 全局基础数据集.查询字段(M, 方法本能函数登记名字段);
            if (函数组.size() > 1 || (项.实现方式 == 新方法实现方式::组合 && !函数组.empty()))
                return 非成功(状态::结构不一致);
            if (!函数组.empty()) {
                const auto 名称 = 读取唯一字符串字段(M, 方法本能函数登记名字段);
                if (!名称 || 名称->empty()) return 非成功(状态::结构不一致);
                项.本能函数登记名 = *名称;
            }
            结果.项组.push_back(std::move(项));
        }
        结果.方法组织根 = 新方法组织根节点;
        结果.状态 = 状态::已读取;
        return 结果;
    } catch (const std::bad_alloc&) { return 非成功(状态::资源失败); }
    catch (...) { return 非成功(状态::内部错误); }
}

std::optional<新方法信息> 新_方法类::获取方法(
    稳定编码 方法节点) const noexcept {
    try {
        // 方法节点仍物理存在而一次读不成完整方法：多次基础读取之间遇并发发布（撕裂读取）。每次重读前释放
        // 方法锁，使同锁发布者得以完成（调用方已持外层递归锁时不会让出外层锁）；有限次数后仍不可读则如实返回空。
        std::optional<新方法信息> 方法;
        for (int 重读 = 0; 重读 < 16; ++重读) {
            if (重读) std::this_thread::yield();
            std::lock_guard 锁(新方法互斥);
            if (!结构已分配() || !节点类型为(新方法组织根节点, 方法组织根类型)
                || !全局基础数据集.查询目标关系(新方法组织根节点, 基础外部关系类型::父子).empty())
                return std::nullopt;
            方法 = 读取方法信息本体(方法节点, 特征概念服务_, *this, 存在概念服务_, false, true);
            // 不是正式方法（类型或归属不符）属如实结果，不重读。
            if (方法 || !有效(方法节点) || !全局基础数据集.查询节点(方法节点)
                || !节点类型为(方法节点, 方法节点类型) || !方法属于根(方法节点)) break;
        }
#ifdef HY_EGO_DEBUG_LOG_SIMULATION_PROGRESS
        if (!方法 && 有效(方法节点) && 节点类型为(方法节点, 方法节点类型) && 方法属于根(方法节点))
            try { (void)调试日志<调试切片::SIMULATION_PROGRESS>(L"获取方法", L"读不成方法",
                L"方法=" + std::to_wstring(方法节点.值) + L"；返回行=" + std::to_wstring(方法读失败行)); } catch (...) {}
#endif
        std::lock_guard 锁(新方法互斥);
        if (方法) for (const auto& 参数 : 方法->参数规格组) {
            if (参数.有序规格 && 是特征持有参数(参数.有序规格->信息类型)
                && !方法持有者范围有效(*参数.有序规格,存在概念服务_))
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取有序方法参数", L"存在参数范围不是可读取的正式存在概念。");
        }
        return 方法;
    }
#ifdef HY_EGO_DEBUG_LOG_SIMULATION_PROGRESS
    catch (const std::exception& 异常) {
        try {
            const std::string w=异常.what();
            (void)调试日志<调试切片::SIMULATION_PROGRESS>(L"获取方法",L"读回异常",
                L"方法="+std::to_wstring(方法节点.值)+L"；what="+std::wstring(w.begin(),w.end()));
        } catch(...) {}
        return std::nullopt;
    }
#endif
    catch (...) {
        return std::nullopt;
    }
}

新_方法类::执行结构读取结果 新_方法类::读取方法执行结构闭包(
    const std::vector<稳定编码>& 根方法节点组) const noexcept {
    using 状态 = 执行结构读取状态;
    try {
        if (根方法节点组.empty() || std::any_of(根方法节点组.begin(),
            根方法节点组.end(), [](auto 编码) { return !有效(编码); }))
            return {状态::入口拒绝, {}, {}};
        std::lock_guard 锁(新方法互斥);
        if (!结构已分配()) return {状态::尚未初始化, {}, {}};
        if (!结构仍存在()) 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__,
            L"读取方法执行结构闭包", L"方法结构元数据损坏");
        std::set<稳定编码> 根组(根方法节点组.begin(), 根方法节点组.end());
        for (const auto 根 : 根组) {
            const auto 读取 = 全局基础数据集.查询完整节点结构(根);
            if (读取.状态 == 基础结构操作状态::节点不存在)
                return {状态::方法不存在, {}, {}};
            if (读取.状态 != 基础结构操作状态::已完成 || !读取.结构)
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__,
                    L"读取方法执行结构闭包", L"方法入口读取失败");
            if (!节点类型为(根, 方法节点类型)) return {状态::方法不存在, {}, {}};
        }
        std::set<稳定编码> 待读 = 根组, 已读;
        执行结构读取结果 结果{状态::已读取, {根组.begin(), 根组.end()}, {}};
        while (!待读.empty()) {
            const auto 节点 = *待读.begin();
            待读.erase(待读.begin());
            if (!已读.insert(节点).second) continue;
            auto 方法 = 读取方法信息本体(节点, 特征概念服务_, *this, 存在概念服务_, false, true);
            if (!方法) 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__,
                L"读取方法执行结构闭包", L"正式方法闭包断链或定义损坏");
            for(const auto& 配对:方法->条件结果组)
                if(!条件结果角色已声明(方法->参数规格组,{配对.条件,配对.结果,配对.线性关系组},
                    特征概念服务_,存在概念服务_,引用内容条件校验模式::完整语义))
                    throw std::runtime_error("method execution content condition semantic validation failed");
            for(const auto& 分支:方法->执行分支组)
                if(!分支条件有效(分支.定义.必要条件,方法->参数规格组,特征概念服务_,存在概念服务_,
                    引用内容条件校验模式::完整语义))
                    throw std::runtime_error("method execution branch semantic validation failed");
            for (const auto& 参数 : 方法->参数规格组) {
                if (参数.有序规格 && 是特征持有参数(参数.有序规格->信息类型)
                    && !方法持有者范围有效(*参数.有序规格,存在概念服务_))
                    工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取方法执行结构闭包", L"存在参数范围不是正式存在概念。");
            }
            if (方法->来源方法节点 || 方法->实例根节点)
                return {状态::来源不是抽象方法, {}, {}};
            if (方法->实现方式 == 新方法实现方式::本能
                && (!方法->组合步骤组.empty() || 方法->组合入口步骤节点
                    || !方法->结果转移组.empty()))
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__,
                    L"读取方法执行结构闭包", L"本能方法含组合执行结构");
            for (const auto& 步骤 : 方法->组合步骤组)
                if (!已读.contains(步骤.调用方法节点)) 待读.insert(步骤.调用方法节点);
            结果.方法组.push_back(std::move(*方法));
        }
        std::sort(结果.方法组.begin(), 结果.方法组.end(),
            [](const auto& 左, const auto& 右) { return 左.节点 < 右.节点; });
        std::set<稳定编码> 图节点,保护身份{新方法组织根节点};
        std::vector<基础联合节点快照> 场景概念保护;
        const auto addProof=[&](const std::vector<基础联合节点快照>& proof) {
            for(const auto& p:proof){保护身份.insert(p.结构.节点.编码);场景概念保护.push_back(p);}
        };
        for(const auto& 方法:结果.方法组) {
            const auto 内容保护=[&](const 新方法条件定义& 条件) {
                for(const auto& 项:条件.条件项组)if(项.引用内容条件) {
                    std::vector<基础联合节点快照> 保护;
                    if(!读取引用内容条件定义保护(*项.引用内容条件,特征概念服务_,存在概念服务_,保护))
                        throw std::runtime_error("method execution content condition protection unavailable");
                    addProof(保护);
                    const auto 参数=std::find_if(方法.参数规格组.begin(),方法.参数规格组.end(),
                        [&](const auto& 规格){return 规格.有序规格&&规格.有序规格->序号==项.参数序号;});
                    if(参数==方法.参数规格组.end())throw std::runtime_error("method content parameter specification missing");
                    if(项.引用内容条件->作用点==新方法内容作用点::参数自身) {
                        std::vector<基础联合节点快照> p;
                        if(!追加方法存在范围保护(参数->有序规格->范围概念节点,存在概念服务_,p))
                            throw std::runtime_error("method direct content formal range unavailable");
                        addProof(p);continue;
                    }
                    const auto FT=特征概念服务_.读取特征概念定义保护(参数->有序规格->范围概念节点);
                    if(FT.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
                    if(FT.前提已变化)throw 引用内容定义前提变化{};
                    if(FT.状态!=基础结构操作状态::已完成||!FT.信息||FT.预期节点组.empty())
                        throw std::runtime_error("method content parameter range protection unavailable");
                    addProof(FT.预期节点组);
                }
            };
            for(const auto& 分支:方法.执行分支组)内容保护(分支.定义.必要条件);
            for(const auto& 配对:方法.条件结果组)内容保护(配对.条件);
            for(const auto& 配对:方法.条件结果组)for(const auto& item:配对.结果.结果项组)if(item.引用内容结果) {
                std::vector<基础联合节点快照> p;
                if(!读取引用内容条件定义保护(*item.引用内容结果,特征概念服务_,存在概念服务_,p))
                    throw std::runtime_error("method execution result content definition unavailable");
                addProof(p);
                const auto c=存在概念服务_.读取引用内容筹办概念();
                if(c.状态==正式结构操作状态::前提已变化)throw 引用内容定义前提变化{};
                if(c.状态!=正式结构操作状态::已读取||!c.信息||c.完整保护组.empty()
                    ||c.信息->完整被认知需求!=item.引用内容结果->要求概念节点)
                    throw std::runtime_error("method execution complete demand result unavailable");
                addProof(c.完整保护组);
            }
        }
        for(const auto& method:结果.方法组)if(method.初始必要条件
            &&通用概念必要声明形状有效(*method.初始必要条件)) {
            新方法知识建立请求 request;request.参数规格组=method.参数规格组;request.初始必要条件=method.初始必要条件;
            std::vector<基础联合节点快照> proof;
            const auto checked=核验通用概念必要声明(request,存在概念服务_,特征概念服务_,proof);
            if(checked==新方法操作状态::结构不一致)return {状态::前提已变化,{},{},{}};
            if(checked!=新方法操作状态::已找到)throw std::runtime_error("generic initial execution declaration invalid");
            addProof(proof);
        }
        std::set<稳定编码> messageExistence;
        const auto existenceProof=[&](auto&& self,稳定编码 C)->void {
            (void)self;
            if(!messageExistence.insert(C).second)return;
            std::vector<基础联合节点快照> proof;
            if(!追加方法存在范围保护(C,存在概念服务_,proof))
                throw std::runtime_error("message range existence protection unavailable");
            addProof(proof);
        };
        for(const auto& method:结果.方法组)for(const auto& spec:method.参数规格组)if(spec.有序规格) {
            if(spec.有序规格->信息类型==新方法参数信息类型::存在)existenceProof(existenceProof,spec.有序规格->范围概念节点);
            else if(spec.有序规格->信息类型==新方法参数信息类型::词条) {
                std::vector<基础联合节点快照> proof;
                if(!追加方法词条范围保护(spec.有序规格->范围概念节点,存在概念服务_,proof))
                    throw std::runtime_error("word parameter range unavailable");
                addProof(proof);
            } else {
                const auto proof=特征概念服务_.读取特征概念定义保护(spec.有序规格->范围概念节点);
                if(!proof.信息||proof.前提已变化)throw std::runtime_error("message feature range protection unavailable");
                addProof(proof.预期节点组);
            }
        }
        std::vector<稳定编码> 栈;
        for(const auto& M:结果.方法组)栈.push_back(M.节点);
        while(!栈.empty()) {
            const auto n=栈.back();栈.pop_back();
            if(!图节点.insert(n).second)continue;
            const auto 类型=读取唯一节点字段(n,节点类型字段);
            const auto 快照=全局基础数据集.查询完整节点结构(n);
            if(!类型||!快照.结构)throw std::runtime_error("method execution protection graph unreadable");
            const auto 规格=方法复制字段规格(*类型);
            if(规格.empty())throw std::runtime_error("method execution protection schema unavailable");
            保护身份.insert(n);保护身份.insert(*类型);
            for(const auto& s:规格) {
                if(有效(s.字段))保护身份.insert(s.字段);
                if(有效(s.目标类型))保护身份.insert(s.目标类型);
            }
            for(const auto& f:快照.结构->字段组) {
                保护身份.insert(f.字段节点);
                const auto* 引用=std::get_if<稳定编码>(&f.内容);
                if (f.字段节点 == 有序参数类别概念字段) {
                    if (!引用) throw std::runtime_error("method execution category is not a reference");
                    addProof(解析参数类别概念保护(*引用, 存在概念服务_, 特征概念服务_).完整保护组);
                }
                if(!引用)continue;
                const auto s=std::find_if(规格.begin(),规格.end(),[&](const auto& v){return v.字段==f.字段节点;});
                if(s!=规格.end()&&(s->种类==复制字段种类::独占子节点||s->种类==复制字段种类::调用方法))
                    栈.push_back(*引用);
                if(f.字段节点==虚拟存在启用特征字段)保护身份.insert(*引用);
                if(f.字段节点==条件引用内容要求字段||f.字段节点==条件引用内容项字段
                    ||f.字段节点==结果引用内容要求字段||f.字段节点==结果引用内容项字段)保护身份.insert(*引用);
                if(f.字段节点==内容关系路径字段) {
                    std::vector<基础联合节点快照> p;
                    if(!追加需求主体路径保护(*引用,特征概念服务_,存在概念服务_,p))
                        throw std::runtime_error("method execution subject relation path unsupported");
                    addProof(p);
                }
                if(f.字段节点==模拟正文范围字段) {
                    const auto proof=特征概念服务_.读取特征概念定义保护(*引用);
                    if(!proof.信息||proof.前提已变化)throw std::runtime_error("simulation body definition protection unavailable");
                    for(const auto& snapshot:proof.预期节点组) {
                        保护身份.insert(snapshot.结构.节点.编码);场景概念保护.push_back(snapshot);
                    }
                }
                if(f.字段节点==初始关系概念字段||f.字段节点==结果场景关系概念字段) {
                    const auto proof=存在概念服务_.读取已确认消息连接概念保护();
                    if(!proof.信息||proof.信息->节点!=*引用||proof.完整保护组.empty())
                        throw std::runtime_error("method execution scene formal protection unavailable");
                    for(const auto& snapshot:proof.完整保护组) {
                        保护身份.insert(snapshot.结构.节点.编码);
                        场景概念保护.push_back(snapshot);
                    }
                }
            }
        }
        const std::vector<稳定编码> 请求身份{保护身份.begin(),保护身份.end()};
        const auto 完整=全局基础数据集.查询节点组完整结构(请求身份);
        if(完整.状态!=基础结构操作状态::已完成||完整.节点组.size()!=请求身份.size())
            throw std::runtime_error("method execution protection capture failed");
        for(const auto& n:完整.节点组) {
            if(!n.快照)throw std::runtime_error("method execution protection missing snapshot");
            结果.完整保护组.push_back(*n.快照);
        }
        // 变化分类（2026-11-08用户裁决，判据见基础层 联合快照容许变化判定）：本闭包读取组之外的节点指向组内节点的
        // 引用增删（他处召回的候选材料引用、学习记录、状态、概念新成员等）不算定义变化；组内节点本体字段改变、
        // 原外部关系消失、组内来源的原入向引用被删除才报前提已变化。本读取持方法锁，方法owner自身写入不会插入其间。
        const std::set<稳定编码> 读取组(请求身份.begin(),请求身份.end());
        const auto 报变化=[&](const 基础联合节点快照& 原,const 基础联合节点快照* 今,const wchar_t* 阶段)->执行结构读取结果 {
#ifdef HY_EGO_DEBUG_LOG_SIMULATION_PROGRESS
            try {
                std::wostringstream t;
                t<<L"阶段="<<阶段<<L"；节点="<<原.结构.节点.编码.值;
                if(今)t<<L"；本体字段数="<<原.结构.字段组.size()<<L"->"<<今->结构.字段组.size()
                    <<L"；入向字段数="<<原.入向字段组.size()<<L"->"<<今->入向字段组.size()
                    <<L"；出向关系数="<<原.出向外部关系组.size()<<L"->"<<今->出向外部关系组.size()
                    <<L"；入向关系数="<<原.入向外部关系组.size()<<L"->"<<今->入向外部关系组.size();
                else t<<L"；无可容许对照快照";
                (void)调试日志<调试切片::SIMULATION_PROGRESS>(L"方法执行结构闭包",L"前提已变化",t.str());
            } catch(...) {}
#else
            (void)原;(void)今;(void)阶段;
#endif
            return {状态::前提已变化,{},{},{}};
        };
        for(const auto& a:场景概念保护) {
            const auto b=std::find_if(结果.完整保护组.begin(),结果.完整保护组.end(),[&](const auto& v){return v.结构.节点==a.结构.节点;});
            if(b==结果.完整保护组.end())return 报变化(a,nullptr,L"概念证明");
            if(!联合快照容许变化判定(*b,a,读取组))return 报变化(a,&*b,L"概念证明");
        }
        for(const auto& old:结果.方法组)
            if(读取方法信息本体(old.节点,特征概念服务_,*this,存在概念服务_,false,true)!=old) {
                const auto 方法快照=std::find_if(结果.完整保护组.begin(),结果.完整保护组.end(),
                    [&](const auto& v){return v.结构.节点.编码==old.节点;});
                if(方法快照==结果.完整保护组.end())throw std::runtime_error("method execution protection lacks method snapshot");
                return 报变化(*方法快照,nullptr,L"方法定义重读");
            }
        const auto 再读=全局基础数据集.查询节点组完整结构(请求身份);
        if(再读.状态!=基础结构操作状态::已完成||再读.节点组.size()!=结果.完整保护组.size())
            throw std::runtime_error("method execution protection recapture failed");
        std::vector<基础联合节点快照> 较新保护;
        较新保护.reserve(结果.完整保护组.size());
        for(const auto& a:结果.完整保护组) {
            const auto b=std::find_if(再读.节点组.begin(),再读.节点组.end(),
                [&](const auto& v){return v.查询节点==a.结构.节点.编码;});
            if(b==再读.节点组.end()||!b->快照)return 报变化(a,nullptr,L"保护重读");
            if(!联合快照容许变化判定(*b->快照,a,读取组))return 报变化(a,&*b->快照,L"保护重读");
            较新保护.push_back(*b->快照);
        }
        // 容许的组外引用增删以后读者为准，交给使用方的保护保持最新。
        结果.完整保护组=std::move(较新保护);
        return 结果;
    } catch (const 引用内容定义前提变化&) {
        return {状态::前提已变化,{},{},{}};
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__,
            L"读取方法执行结构闭包", L"工程读取异常", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__,
            L"读取方法执行结构闭包", L"未知工程读取异常");
    }
}

新方法复制结果 新_方法类::复制方法(稳定编码 来源方法节点) noexcept {
    try {
        return 复制方法闭包({来源方法节点}, false);
    } catch (...) {
        return {新方法复制状态::资源失败, std::nullopt, {}};
    }
}

新方法混合实例结果 新_方法类::建立混合方法实例(
    const std::vector<稳定编码>& 来源方法节点组) noexcept {
    const auto 结果 = 复制方法闭包(来源方法节点组, true);
    return {结果.状态, 结果.方法实例节点};
}

新方法混合实例结果 新_方法类::建立并交付混合方法实例(
    const std::vector<稳定编码>& 来源方法节点组,
    const std::function<新方法实例交付状态(稳定编码)>& 采用) noexcept {
    if (!采用) return {新方法复制状态::入口拒绝, std::nullopt};
    const auto 结果 = 复制方法闭包(来源方法节点组, true, 采用);
    return {结果.状态, 结果.方法实例节点};
}

新方法复制结果 新_方法类::复制方法闭包(
    const std::vector<稳定编码>& 来源方法节点组, bool 混合,
    const std::function<新方法实例交付状态(稳定编码)>& 采用) noexcept {
    std::vector<稳定编码> 已建节点;
    std::vector<稳定编码> 已写字段;
    std::vector<方法虚拟存在准备> 虚拟组;
    bool 已发布 = false, 明确未采用 = false;
    std::unique_lock<std::recursive_mutex> 锁;
    const auto 失败 = [&](新方法复制状态 状态) noexcept {
        if (已发布 && !明确未采用)
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"复制方法闭包", L"已发表副本读回或采用未确认，保留方法及场景。");
        回收本批方法图(已建节点, 已写字段, 虚拟组, 已发布);
        return 新方法复制结果{状态, std::nullopt, {}};
    };
    try {
        锁 = std::unique_lock<std::recursive_mutex>{新方法互斥};
        if (来源方法节点组.empty() || (!混合 && 来源方法节点组.size() != 1)) {
            return 失败(新方法复制状态::入口拒绝);
        }
        auto 来源组 = 来源方法节点组;
        std::sort(来源组.begin(), 来源组.end());
        if (std::adjacent_find(来源组.begin(), 来源组.end()) != 来源组.end()
            || !std::all_of(来源组.begin(), 来源组.end(),
                [](稳定编码 节点) { return 有效(节点); })) {
            return 失败(新方法复制状态::入口拒绝);
        }
        if (!结构仍存在()) return 失败(新方法复制状态::尚未初始化);
        核验治理组织就绪();
        for (const auto 来源方法节点 : 来源组) {
            if (!节点类型为(来源方法节点, 方法节点类型) || !方法属于根(来源方法节点)) {
                return 失败(新方法复制状态::方法不存在);
            }
            const auto 来源 = 读取方法信息本体(来源方法节点, 特征概念服务_, *this, 存在概念服务_, false, true);
            if (!来源) return 失败(新方法复制状态::结构不一致);
            if (来源->来源方法节点) return 失败(新方法复制状态::来源不是抽象方法);
        }

        struct 复制字段材料 final { 基础字段关系 字段; bool 重映射 = false; };
        struct 复制节点材料 final {
            稳定编码 类型;
            稳定编码 新节点;
            std::vector<复制字段材料> 字段组;
        };
        std::map<稳定编码, 复制节点材料> 材料;
        std::vector<稳定编码> 待查 = 来源组;
        std::vector<稳定编码> 方法组;
        std::set<稳定编码> 独占子节点;
        for (const auto 来源 : 来源组) {
            材料.emplace(来源, 复制节点材料{方法节点类型, {}, {}});
        }
        std::size_t 字段数量 = 0;
        for (std::size_t 位置 = 0; 位置 < 待查.size(); ++位置) {
            const auto 当前 = 待查[位置];
            auto& 当前材料 = 材料.at(当前);
            if (!节点类型为(当前, 当前材料.类型)) return 失败(新方法复制状态::结构不一致);
            if (当前材料.类型 == 方法节点类型) {
                const auto 方法 = 读取方法信息本体(当前, 特征概念服务_, *this, 存在概念服务_, false, true);
                if (!方法) return 失败(新方法复制状态::结构不一致);
                if (方法->来源方法节点) return 失败(新方法复制状态::来源不是抽象方法);
                if (!方法头有效(方法->方法头) || !参数规格有效(方法->参数规格组, 特征概念服务_)
                    || (方法->实现方式 != 新方法实现方式::本能 && 方法->条件结果组.empty())
                    || !std::all_of(方法->条件结果组.begin(), 方法->条件结果组.end(),
                        [&](const auto& 配对) {
                            const 新方法条件结果定义 定义{配对.条件, 配对.结果, 配对.线性关系组};
                            return 条件结果有效(定义, 特征概念服务_)
                                && 条件结果角色已声明(方法->参数规格组, 定义, 特征概念服务_, 存在概念服务_);
                        })) return 失败(新方法复制状态::结构不一致);
                for(const auto& 分支:方法->执行分支组)
                    if(!分支条件有效(分支.定义.必要条件,方法->参数规格组,特征概念服务_,存在概念服务_))
                        return 失败(新方法复制状态::结构不一致);
                方法组.push_back(当前);
            }
            const auto 规格组 = 方法复制字段规格(当前材料.类型);
            if (规格组.empty()) return 失败(新方法复制状态::结构不一致);
            for (const auto& 规格 : 规格组) {
                for (const auto& 字段 : 全局基础数据集.查询字段(当前, 规格.字段)) {
                    const auto* 目标 = std::get_if<稳定编码>(&字段.内容);
                    if (规格.种类 == 复制字段种类::直接值) {
                        if (目标) return 失败(新方法复制状态::结构不一致);
                    } else if (!目标 || !节点仍存在(*目标)) {
                        return 失败(新方法复制状态::结构不一致);
                    }
                    const bool 展开 = 规格.种类 == 复制字段种类::独占子节点
                        || 规格.种类 == 复制字段种类::调用方法;
                    if (展开) {
                        if (规格.种类 == 复制字段种类::独占子节点
                            && !独占子节点.insert(*目标).second) {
                            return 失败(新方法复制状态::结构不一致);
                        }
                        if (规格.种类 == 复制字段种类::独占子节点) {
                            // 不仅检查本次闭包，也拒绝与未被复制的方法非法共用owned节点。
                            std::size_t 持有数 = 0;
                            for (const auto 字段码 : {方法头字段, 方法虚拟存在字段,
                                方法条件结果字段, 方法组合步骤字段, 方法结果转移字段,
                                虚拟存在参数规格字段, 参数特征规格字段, 配对条件字段,
                                配对结果字段, 条件项字段, 结果项字段, 结果禁止项字段,
                                初始必要条件字段,登记结果组字段,初始场景限定字段,结果场景限定字段,执行分支字段,分支条件字段,模拟输入方案字段,模拟消息输入字段}) {
                                const auto 持有组 = 全局基础数据集.查询目标字段(*目标, 字段码);
                                for (const auto& 持有 : 持有组) {
                                    if (持有.所属节点 != 当前 || 持有.字段节点 != 规格.字段) {
                                        return 失败(新方法复制状态::结构不一致);
                                    }
                                    ++持有数;
                                }
                            }
                            if (持有数 != 1) return 失败(新方法复制状态::结构不一致);
                        }
                        const auto [项, 新增] = 材料.emplace(*目标,
                            复制节点材料{规格.目标类型, {}, {}});
                        if (项->second.类型 != 规格.目标类型) {
                            return 失败(新方法复制状态::结构不一致);
                        }
                        if (新增) 待查.push_back(*目标);
                    }
                    当前材料.字段组.push_back({字段,
                        展开 || 规格.种类 == 复制字段种类::内部引用});
                    ++字段数量;
                }
            }
        }
        // 所有反向所属及转移引用必须落在本次完整闭包内；写入前校验。
        for (const auto& [旧节点, 节点材料] : 材料) {
            for (const auto& 字段 : 节点材料.字段组) {
                if (字段.重映射 && !材料.contains(std::get<稳定编码>(字段.字段.内容))) {
                    return 失败(新方法复制状态::结构不一致);
                }
            }
        }
        新方法复制结果 结果;
        结果.节点映射组.reserve(材料.size());
        已建节点.reserve(材料.size() + (混合 ? 1 : 0));
        已写字段.reserve(字段数量 + 材料.size() + 方法组.size() * 3
            + (混合 ? 来源组.size() + 2 : 0));
        for (auto& [旧节点, 节点材料] : 材料) {
            节点材料.新节点 = 全局基础数据集.新建节点();
            if (!有效(节点材料.新节点)) return 失败(新方法复制状态::资源失败);
            已建节点.push_back(节点材料.新节点);
            结果.节点映射组.push_back({旧节点, 节点材料.新节点});
        }
        const auto 新根 = 混合
            ? 新建类型化节点(混合方法实例根类型, 已建节点, 已写字段)
            : 材料.at(来源组.front()).新节点;
        if (!有效(新根)) return 失败(新方法复制状态::资源失败);
        for (const auto& [旧节点, 节点材料] : 材料) {
            if (节点材料.类型 == 条件结果配对节点类型
                && !写入节点字段(节点材料.新节点, 配对来源字段, 旧节点, 已写字段)) {
                return 失败(新方法复制状态::资源失败);
            }
            for (const auto& 字段 : 节点材料.字段组) {
                稳定编码 关系;
                if (const auto* 目标 = std::get_if<稳定编码>(&字段.字段.内容)) {
                    关系 = 全局基础数据集.添加字段节点(节点材料.新节点,
                        字段.字段.字段节点, 字段.重映射 ? 材料.at(*目标).新节点 : *目标);
                } else {
                    关系 = 全局基础数据集.添加字段值(节点材料.新节点,
                        字段.字段.字段节点, std::get<基础值>(字段.字段.内容));
                }
                if (!有效(关系)) return 失败(新方法复制状态::资源失败);
                已写字段.push_back(关系);
            }
        }
        虚拟组.reserve(方法组.size());
        基础节点组追加请求 发布请求;
        std::vector<新方法虚拟存在引用> 引用组;
        for (const auto 方法 : 方法组) {
            const auto 副本 = 材料.at(方法).新节点;
            if (!写入节点字段(副本, 方法来源字段, 方法, 已写字段)
                || !写入节点字段(副本, 方法实例根字段, 新根, 已写字段)) {
                return 失败(新方法复制状态::资源失败);
            }
            const auto source = 读取方法信息本体(方法, 特征概念服务_, *this, 存在概念服务_, false, true);
            if (!source) return 失败(新方法复制状态::结构不一致);
            虚拟组.push_back(准备方法虚拟存在(副本,
                材料.at(source->方法虚拟存在节点).新节点, static_cast<std::int64_t>(source->启用状态)));
            追加方法虚拟归属(发布请求, 虚拟组.back().信息);
            引用组.push_back(虚拟组.back().信息.引用);
            发布请求.追加字段组.push_back({新方法组织根节点, 方法成员字段, 副本});
        }
        if (混合) {
            for (const auto 来源 : 来源组) {
                发布请求.追加字段组.push_back({新根, 混合方法候选入口字段, 材料.at(来源).新节点});
            }
            发布请求.追加字段组.push_back({新方法组织根节点, 混合方法实例成员字段, 新根});
        }
        const auto 片段 = 封存方法片段(std::move(发布请求), 已建节点, 存在概念服务_,特征概念服务_);
        const auto 发表 = 内部治理世界_->发布新方法治理场景组(引用组, 片段);
        if (发表.状态 != 新方法治理场景发布工程状态::已发布) return 失败(新方法复制状态::资源失败);
        已发布 = true;
        for (const auto 方法 : 方法组) {
            const auto 读回 = 读取方法信息本体(材料.at(方法).新节点, 特征概念服务_, *this, 存在概念服务_, false, true);
            if (!读回 || 读回->来源方法节点 != 方法 || 读回->实例根节点 != 新根) {
                return 失败(新方法复制状态::结构不一致);
            }
        }
        结果.状态 = 新方法复制状态::已复制;
        结果.方法实例节点 = 新根;
        if (采用) {
            // task已经持有任务锁；回调仅绑定与读回。创建账保持存活，
            // 采用状态不明时不能进入一般失败清理，更不能删除已绑定图。
            新方法实例交付状态 交付 = 新方法实例交付状态::采用状态不明;
            try {
                交付 = 采用(新根);
            } catch (...) {
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"交付方法闭包", L"采用回调异常，保留已发表方法及场景。");
            }
            if (交付 == 新方法实例交付状态::未采用) {
                明确未采用 = true;
                return 失败(新方法复制状态::入口拒绝);
            }
            if (交付 != 新方法实例交付状态::已采用) {
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"交付方法闭包", L"采用状态未知，保留已发表方法及场景。");
            }
        }
        // 所有可分配输出均在采用前形成；此后只移动值式结果。
        return 结果;
    } catch (const 引用内容定义前提变化&) {
        // 本控制只会在采用回调前的定义读取中产生；回收仍由原owner逐项核实未消费。
        if(已发布)明确未采用=true;
        return 失败(新方法复制状态::入口拒绝);
    } catch (...) {
        return 失败(新方法复制状态::资源失败);
    }
}

新_方法类::新方法实例增补结果 新_方法类::扩充并交付混合方法实例(
    稳定编码 预期旧实例根,
    const std::vector<稳定编码>& 候选抽象方法组,
    const std::function<新方法实例交付状态(稳定编码)>& 采用) noexcept {
    using 状态 = 新方法实例增补状态;
    std::vector<稳定编码> 已建节点, 已写字段;
    std::vector<方法虚拟存在准备> 虚拟组;
    bool 已发布 = false, 明确未采用 = false;
    std::optional<稳定编码> 新根;
    bool 可能已采用 = false;
    std::unique_lock<std::recursive_mutex> 锁;
    const auto 失败 = [&](状态 原因) noexcept -> 新方法实例增补结果 {
        if (可能已采用 || (已发布 && !明确未采用))
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"增补方法闭包", L"已发表方法读回或采用未确认，保留方法及场景。");
        回收本批方法图(已建节点, 已写字段, 虚拟组, 已发布);
        return {原因, std::nullopt};
    };
    try {
        锁 = std::unique_lock<std::recursive_mutex>{新方法互斥};
        if (!有效(预期旧实例根) || 候选抽象方法组.empty() || !采用)
            return 失败(状态::入口拒绝);
        if (!结构仍存在()) return 失败(状态::尚未初始化);
        核验治理组织就绪();
        const auto 旧根 = 获取混合方法实例(预期旧实例根);
        if (!旧根) return 失败(状态::实例不存在);
        std::set<稳定编码> 候选来源;
        for (const auto 节点 : 候选抽象方法组)
            if (!有效(节点) || !候选来源.insert(节点).second) return 失败(状态::入口拒绝);

        std::map<稳定编码, 新方法信息> 旧方法组, 来源方法组;
        // 抽象身份到本轮材料身份；已有来源一律复用旧副本的材料，绝不复制第二份。
        std::map<稳定编码, 稳定编码> 来源映射;
        std::map<稳定编码, 稳定编码> 配对来源组, 旧结果来源组;
        std::map<稳定编码, std::vector<稳定编码>> 追加配对组,追加配对分支来源组;
        std::vector<稳定编码> 待读来源(候选来源.begin(), 候选来源.end());
        for (const auto& 成员 : 全局基础数据集.查询目标字段(预期旧实例根, 方法实例根字段)) {
            const auto 旧 = 读取方法信息本体(成员.所属节点, 特征概念服务_, *this, 存在概念服务_, false, true);
            if (!旧 || !旧->来源方法节点 || !来源映射.emplace(*旧->来源方法节点, 旧->节点).second)
                return 失败(状态::来源无法对齐);
            旧方法组.emplace(旧->节点, *旧);
            待读来源.push_back(*旧->来源方法节点);
        }
        for (std::size_t i = 0; i < 待读来源.size(); ++i) {
            const auto 编码 = 待读来源[i];
            if (来源方法组.contains(编码)) continue;
            const auto 方法 = 读取方法信息本体(编码, 特征概念服务_, *this, 存在概念服务_, false, true);
            if (!方法) return 失败(状态::来源方法不存在);
            if (方法->来源方法节点 || 方法->实例根节点) return 失败(状态::来源无法对齐);
            if (!方法头有效(方法->方法头) || !参数规格有效(方法->参数规格组, 特征概念服务_)
                || (方法->实现方式 != 新方法实现方式::本能 && 方法->条件结果组.empty())
                || !std::all_of(方法->条件结果组.begin(), 方法->条件结果组.end(), [&](const auto& 对) {
                    const 新方法条件结果定义 定义{对.条件, 对.结果, 对.线性关系组};
                    return 条件结果有效(定义, 特征概念服务_)
                        && 条件结果角色已声明(方法->参数规格组, 定义, 特征概念服务_, 存在概念服务_);
                })) return 失败(状态::结构不一致);
            for(const auto& 分支:方法->执行分支组)
                if(!分支条件有效(分支.定义.必要条件,方法->参数规格组,特征概念服务_,存在概念服务_))
                    return 失败(状态::结构不一致);
            来源方法组.emplace(编码, *方法);
            for (const auto& 步骤 : 方法->组合步骤组) 待读来源.push_back(步骤.调用方法节点);
        }
        const auto 配对可对齐 = [&](const 新方法条件结果信息& 旧, const 新方法条件结果信息& 源) {
            if (!同义结果(旧.结果,源.结果) || !线性关系等价(旧.线性关系组, 源.线性关系组)) return false;
            if (旧.线性关系组.empty()) return 旧.条件 == 源.条件;
            if (旧.条件.关系规则 != 源.条件.关系规则
                || 旧.条件.条件项组.size() != 源.条件.条件项组.size()) return false;
            auto 还原 = 旧.条件;
            for (const auto& 关系 : 旧.线性关系组) {
                if (关系.条件序号 >= 还原.条件项组.size()) return false;
                auto& 项 = 还原.条件项组[关系.条件序号];
                const auto& 原项 = 源.条件.条件项组[关系.条件序号];
                const auto& 旧项 = 旧.条件.条件项组[关系.条件序号];
                const auto 域 = 读取线性项域(旧项.特征类型概念节点,
                    旧项.特征值域概念节点, 旧项.精确数值域, 特征概念服务_);
                if (!域 || 域->下界 < 关系.原条件域.下界 || 域->上界 > 关系.原条件域.上界) return false;
                项.特征值域概念节点 = 原项.特征值域概念节点;
                项.精确数值域 = 原项.精确数值域;
            }
            return 还原 == 源.条件;
        };
        bool 有增量 = false;
        for (const auto& [旧编码, 旧] : 旧方法组) {
            const auto& 源 = 来源方法组.at(*旧.来源方法节点);
            if (旧.方法头 != 源.方法头 || 旧.参数规格组 != 源.参数规格组
                ||分支定义组(旧)!=分支定义组(源)
                ||旧.初始模拟输入方案组!=源.初始模拟输入方案组
                || 旧.作用域 != 源.作用域 || 旧.实现方式 != 源.实现方式
                || 旧.启用状态 != 源.启用状态 || 旧.本能函数登记名 != 源.本能函数登记名)
                return 失败(状态::来源无法对齐);
            for(std::size_t i=0;i<源.执行分支组.size();++i)
                来源映射.emplace(源.执行分支组[i].节点,旧.执行分支组[i].节点);
            std::set<稳定编码> 已导入;
            for (const auto& 旧对 : 旧.条件结果组) {
                const 新方法条件结果信息* 匹配 = nullptr;
                for (const auto& 源对 : 源.条件结果组) {
                    if (旧对.来源配对节点 && *旧对.来源配对节点 != 源对.配对节点) continue;
                    if (!配对可对齐(旧对, 源对)) continue;
                    if (匹配) return 失败(状态::来源无法对齐);
                    匹配 = &源对;
                }
                if (!匹配 || !已导入.insert(匹配->配对节点).second
                    || 读取唯一I64字段(旧对.配对节点, 顺序字段)
                        != 读取唯一I64字段(匹配->配对节点, 顺序字段)) return 失败(状态::来源无法对齐);
                配对来源组.emplace(旧对.配对节点, 匹配->配对节点);
                std::vector<稳定编码> mapped;
                for(const auto B:匹配->来源执行分支节点组) {
                    if(!来源映射.contains(B))return 失败(状态::来源无法对齐);
                    mapped.push_back(来源映射.at(B));
                }
                for(const auto B:旧对.来源执行分支节点组)
                    if(std::find(mapped.begin(),mapped.end(),B)==mapped.end())return 失败(状态::来源无法对齐);
                for(const auto B:mapped)if(std::find(旧对.来源执行分支节点组.begin(),旧对.来源执行分支节点组.end(),B)==旧对.来源执行分支节点组.end()) {
                    追加配对分支来源组[旧对.配对节点].push_back(B);有增量=true;
                }
                旧结果来源组.emplace(旧对.结果节点, 匹配->结果节点);
                if (!来源映射.emplace(匹配->配对节点, 旧对.配对节点).second
                    || !来源映射.emplace(匹配->结果节点, 旧对.结果节点).second)
                    return 失败(状态::来源无法对齐);
            }
            for (const auto& 源对 : 源.条件结果组) {
                if (已导入.contains(源对.配对节点)) continue;
                if (旧.实现方式 != 新方法实现方式::本能) return 失败(状态::来源无法对齐);
                追加配对组[旧编码].push_back(源对.配对节点);
                有增量 = true;
            }
        }
        // 组合结构只允许原样复制。按明确的步骤顺序及来源身份规范化后比较，
        // 不以节点数量、名称或位置猜测配对身份，也不迁移已经改变的组合知识。
        for (const auto& [旧编码, 旧] : 旧方法组) {
            if (旧.实现方式 != 新方法实现方式::组合) continue;
            const auto& 源 = 来源方法组.at(*旧.来源方法节点);
            if (旧.组合步骤组.size() != 源.组合步骤组.size()
                || 旧.结果转移组.size() != 源.结果转移组.size()) return 失败(状态::来源无法对齐);
            std::map<稳定编码, 稳定编码> 步骤来源;
            for (std::size_t i = 0; i < 旧.组合步骤组.size(); ++i) {
                const auto& 左 = 旧.组合步骤组[i];
                const auto& 右 = 源.组合步骤组[i];
                const auto 调用 = 旧方法组.find(左.调用方法节点);
                if (左.顺序 != 右.顺序 || 调用 == 旧方法组.end()
                    || 调用->second.来源方法节点 != 右.调用方法节点)
                    return 失败(状态::来源无法对齐);
                步骤来源.emplace(左.步骤节点, 右.步骤节点);
            }
            if (旧.组合入口步骤节点.has_value() != 源.组合入口步骤节点.has_value()
                || (旧.组合入口步骤节点
                    && 步骤来源.at(*旧.组合入口步骤节点) != *源.组合入口步骤节点))
                return 失败(状态::来源无法对齐);
            auto 规范转移 = 旧.结果转移组;
            auto 源转移 = 源.结果转移组;
            for (auto& 项 : 规范转移) {
                if (!步骤来源.contains(项.当前步骤节点) || !步骤来源.contains(项.下一步骤节点)
                    || !旧结果来源组.contains(项.当前结果节点)) return 失败(状态::来源无法对齐);
                项.转移节点 = {};
                项.当前步骤节点 = 步骤来源.at(项.当前步骤节点);
                项.下一步骤节点 = 步骤来源.at(项.下一步骤节点);
                项.当前结果节点 = 旧结果来源组.at(项.当前结果节点);
            }
            for (auto& 项 : 源转移) 项.转移节点 = {};
            if (!std::is_permutation(规范转移.begin(), 规范转移.end(), 源转移.begin(), 源转移.end()))
                return 失败(状态::来源无法对齐);
        }
        std::vector<稳定编码> 入口材料 = 旧根->候选方法实例根组;
        std::set<稳定编码> 旧入口来源;
        for (const auto 编码 : 入口材料) 旧入口来源.insert(*旧方法组.at(编码).来源方法节点);
        for (const auto 来源 : 候选来源) {
            if (旧入口来源.contains(来源)) continue;
            入口材料.push_back(来源映射.contains(来源) ? 来源映射.at(来源) : 来源);
            有增量 = true;
        }
        if (!有增量) return {状态::精确重复, 预期旧实例根};

        struct 字段材料 final { 基础字段关系 字段; bool 重映射 = false; };
        struct 节点材料 final { 稳定编码 类型; 稳定编码 新节点; std::vector<字段材料> 字段组; };
        std::map<稳定编码, 节点材料> 材料;
        std::vector<稳定编码> 待查 = 入口材料, 方法材料组;
        std::set<稳定编码> 独占子节点;
        const auto 规范身份 = [&](稳定编码 节点) {
            const auto i = 来源映射.find(节点);
            return i == 来源映射.end() ? 节点 : i->second;
        };
        for (const auto 节点 : 待查) 材料.emplace(节点, 节点材料{方法节点类型, {}, {}});
        std::size_t 字段数量 = 0;
        for (std::size_t i = 0; i < 待查.size(); ++i) {
            const auto 当前 = 待查[i];
            auto& 当前材料 = 材料.at(当前);
            if (!节点类型为(当前, 当前材料.类型)) return 失败(状态::结构不一致);
            if (当前材料.类型 == 方法节点类型) {
                if (!旧方法组.contains(当前) && !来源方法组.contains(当前)) return 失败(状态::来源无法对齐);
                方法材料组.push_back(当前);
            }
            const auto 规格组 = 方法复制字段规格(当前材料.类型);
            if (规格组.empty()) return 失败(状态::结构不一致);
            for (const auto& 规格 : 规格组) {
                // 来源字段统一写原抽象P，不沿旧cloneP建立另一层来源。
                if (规格.字段 == 配对来源字段) continue;
                auto 字段组 = 全局基础数据集.查询字段(当前, 规格.字段);
                if (规格.字段 == 方法条件结果字段 && 追加配对组.contains(当前)) {
                    for (const auto 配对 : 追加配对组.at(当前))
                        字段组.push_back({{}, 当前, 方法条件结果字段, 配对});
                }
                if(规格.字段==配对分支来源字段&&追加配对分支来源组.contains(当前))
                    for(const auto B:追加配对分支来源组.at(当前))字段组.push_back({{},当前,配对分支来源字段,B});
                for (auto 字段 : 字段组) {
                    const auto* 目标 = std::get_if<稳定编码>(&字段.内容);
                    if (规格.种类 == 复制字段种类::直接值) {
                        if (目标) return 失败(状态::结构不一致);
                    } else if (!目标 || !节点仍存在(*目标)) return 失败(状态::结构不一致);
                    const bool 展开 = 规格.种类 == 复制字段种类::独占子节点
                        || 规格.种类 == 复制字段种类::调用方法;
                    const bool 重映射 = 展开 || 规格.种类 == 复制字段种类::内部引用;
                    if (展开) {
                        const auto 规范目标 = 规范身份(*目标);
                        if (规格.种类 == 复制字段种类::独占子节点) {
                            if (!独占子节点.insert(规范目标).second) return 失败(状态::结构不一致);
                            std::size_t 持有数 = 0;
                            // 新追加源P的实际持有者仍为抽象M；合成边只进入内存材料。
                            const auto 预期持有 = 有效(字段.编码) ? 当前 : *旧方法组.at(当前).来源方法节点;
                            for (const auto 字段码 : {方法头字段, 方法虚拟存在字段, 方法条件结果字段,
                                方法组合步骤字段, 方法结果转移字段, 虚拟存在参数规格字段,
                                参数特征规格字段, 配对条件字段, 配对结果字段, 条件项字段,
                                结果项字段, 结果禁止项字段,初始必要条件字段,登记结果组字段,初始场景限定字段,结果场景限定字段,执行分支字段,分支条件字段,模拟输入方案字段,模拟消息输入字段}) {
                                for (const auto& 持有 : 全局基础数据集.查询目标字段(*目标, 字段码)) {
                                    if (持有.所属节点 != 预期持有 || 持有.字段节点 != 规格.字段)
                                        return 失败(状态::结构不一致);
                                    ++持有数;
                                }
                            }
                            if (持有数 != 1) return 失败(状态::结构不一致);
                        }
                        const auto [项, 新增] = 材料.emplace(规范目标, 节点材料{规格.目标类型, {}, {}});
                        if (项->second.类型 != 规格.目标类型) return 失败(状态::结构不一致);
                        if (新增) 待查.push_back(规范目标);
                    }
                    if (重映射) 字段.内容 = 规范身份(*目标);
                    当前材料.字段组.push_back({std::move(字段), 重映射});
                    ++字段数量;
                }
            }
        }
        for (const auto& [节点, 项] : 材料)
            for (const auto& 字段 : 项.字段组)
                if (字段.重映射 && !材料.contains(std::get<稳定编码>(字段.字段.内容)))
                    return 失败(状态::结构不一致);
        已建节点.reserve(材料.size() + 1);
        已写字段.reserve(字段数量 + 材料.size() + 方法材料组.size() * 3 + 入口材料.size() + 2);
        // 先建立新根，使清理不完整时始终可返回本批图的定位；尚不注册/采用。
        const auto 根编码 = 全局基础数据集.新建节点();
        if (!有效(根编码)) return 失败(状态::资源失败);
        已建节点.push_back(根编码);
        新根 = 根编码;
        if (!写入节点字段(*新根, 节点类型字段, 混合方法实例根类型, 已写字段))
            return 失败(状态::资源失败);
        for (auto& [节点, 项] : 材料) {
            项.新节点 = 全局基础数据集.新建节点();
            if (!有效(项.新节点)) return 失败(状态::资源失败);
            已建节点.push_back(项.新节点);
        }
        for (const auto& [节点, 项] : 材料) {
            if (项.类型 == 条件结果配对节点类型
                && !写入节点字段(项.新节点, 配对来源字段,
                    配对来源组.contains(节点) ? 配对来源组.at(节点) : 节点, 已写字段))
                return 失败(状态::资源失败);
            for (const auto& 字段 : 项.字段组) {
                稳定编码 关系;
                if (const auto* 目标 = std::get_if<稳定编码>(&字段.字段.内容))
                    关系 = 全局基础数据集.添加字段节点(项.新节点, 字段.字段.字段节点,
                        字段.重映射 ? 材料.at(*目标).新节点 : *目标);
                else 关系 = 全局基础数据集.添加字段值(项.新节点, 字段.字段.字段节点,
                    std::get<基础值>(字段.字段.内容));
                if (!有效(关系)) return 失败(状态::资源失败);
                已写字段.push_back(关系);
            }
        }
        虚拟组.reserve(方法材料组.size());
        基础节点组追加请求 发布请求;
        std::vector<新方法虚拟存在引用> 引用组;
        std::map<稳定编码, 新方法虚拟存在读回> 新虚拟组;
        for (const auto 节点 : 方法材料组) {
            const auto 原始方法 = 旧方法组.contains(节点) ? *旧方法组.at(节点).来源方法节点 : 节点;
            if (!写入节点字段(材料.at(节点).新节点, 方法来源字段, 原始方法, 已写字段)
                || !写入节点字段(材料.at(节点).新节点, 方法实例根字段, *新根, 已写字段))
                return 失败(状态::资源失败);
            const auto& 源 = 旧方法组.contains(节点) ? 旧方法组.at(节点) : 来源方法组.at(节点);
            虚拟组.push_back(准备方法虚拟存在(材料.at(节点).新节点,
                材料.at(源.方法虚拟存在节点).新节点, static_cast<std::int64_t>(源.启用状态)));
            新虚拟组.emplace(节点, 虚拟组.back().信息);
            追加方法虚拟归属(发布请求, 虚拟组.back().信息);
            引用组.push_back(虚拟组.back().信息.引用);
            发布请求.追加字段组.push_back({新方法组织根节点, 方法成员字段, 材料.at(节点).新节点});
        }
        for (const auto 入口 : 入口材料)
            发布请求.追加字段组.push_back({*新根, 混合方法候选入口字段, 材料.at(入口).新节点});
        发布请求.追加字段组.push_back({新方法组织根节点, 混合方法实例成员字段, *新根});
        const auto 片段 = 封存方法片段(std::move(发布请求), 已建节点, 存在概念服务_,特征概念服务_);
        const auto 发表 = 内部治理世界_->发布新方法治理场景组(引用组, 片段);
        if (发表.状态 != 新方法治理场景发布工程状态::已发布) return 失败(状态::资源失败);
        已发布 = true;
        if (!获取混合方法实例(*新根)) return 失败(状态::结构不一致);
        // 结构可读不是值已完整复制的替代证明；逐M重建预期DTO后比较全量读回。
        const auto 副本 = [&](稳定编码 节点) { return 材料.at(规范身份(节点)).新节点; };
        for (const auto 节点 : 方法材料组) {
            auto 预期 = 旧方法组.contains(节点) ? 旧方法组.at(节点) : 来源方法组.at(节点);
            const auto 原始方法 = 预期.来源方法节点.value_or(节点);
            if (追加配对组.contains(节点)) {
                for (const auto 配对 : 追加配对组.at(节点)) {
                    const auto& 源 = 来源方法组.at(原始方法).条件结果组;
                    const auto 项 = std::find_if(源.begin(), 源.end(), [&](const auto& x) { return x.配对节点 == 配对; });
                    if (项 == 源.end()) return 失败(状态::结构不一致);
                    预期.条件结果组.push_back(*项);
                }
                std::sort(预期.条件结果组.begin(), 预期.条件结果组.end(), [&](const auto& 左, const auto& 右) {
                    return 读取唯一I64字段(左.配对节点, 顺序字段) < 读取唯一I64字段(右.配对节点, 顺序字段);
                });
            }
            预期.节点 = 副本(节点);
            预期.方法头节点 = 副本(预期.方法头节点);
            预期.方法虚拟存在节点 = 副本(预期.方法虚拟存在节点);
            预期.启用特征节点 = 新虚拟组.at(节点).引用.启用特征节点;
            预期.专属存在概念节点 = 新虚拟组.at(节点).专属存在概念节点;
            预期.来源方法节点 = 原始方法;
            预期.实例根节点 = 新根;
            for(auto& b:预期.执行分支组)b.节点=副本(b.节点);
            for (auto& 项 : 预期.条件结果组) {
                if(追加配对分支来源组.contains(项.配对节点))
                    项.来源执行分支节点组.insert(项.来源执行分支节点组.end(),追加配对分支来源组.at(项.配对节点).begin(),追加配对分支来源组.at(项.配对节点).end());
                for(auto& B:项.来源执行分支节点组)B=副本(B);
                std::sort(项.来源执行分支节点组.begin(),项.来源执行分支节点组.end());
                项.来源配对节点 = 配对来源组.contains(项.配对节点) ? 配对来源组.at(项.配对节点) : 项.配对节点;
                项.配对节点 = 副本(项.配对节点);
                项.条件节点 = 副本(项.条件节点);
                项.结果节点 = 副本(项.结果节点);
            }
            for (auto& 项 : 预期.组合步骤组) {
                项.步骤节点 = 副本(项.步骤节点);
                项.调用方法节点 = 副本(项.调用方法节点);
            }
            if (预期.组合入口步骤节点) 预期.组合入口步骤节点 = 副本(*预期.组合入口步骤节点);
            for (auto& 项 : 预期.结果转移组) {
                项.转移节点 = 副本(项.转移节点);
                项.当前步骤节点 = 副本(项.当前步骤节点);
                项.当前结果节点 = 副本(项.当前结果节点);
                项.下一步骤节点 = 副本(项.下一步骤节点);
            }
            std::sort(预期.结果转移组.begin(), 预期.结果转移组.end(), [](const auto& 左, const auto& 右) {
                return 左.转移节点 < 右.转移节点;
            });
            const auto 读回 = 读取方法信息本体(预期.节点, 特征概念服务_, *this, 存在概念服务_, false, true);
            if (!读回 || *读回 != 预期) return 失败(状态::结构不一致);
        }
        // 采用回调只做task CAS。此后任何异常/未知均保留新旧图，不撤销可能已绑定的数据。
        可能已采用 = true;
        const auto 交付 = 采用(*新根);
        if (交付 == 新方法实例交付状态::已采用) return {状态::已扩充, 新根};
        if (交付 == 新方法实例交付状态::未采用) {
            可能已采用 = false;
            明确未采用 = true;
            return 失败(状态::未采用);
        }
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"交付增补方法", L"采用状态未知，保留已发表方法及场景。");
    } catch (const 引用内容定义前提变化&) {
        if(已发布||可能已采用)return {状态::发布结果未知,新根};
        return 失败(状态::入口拒绝);
    } catch (...) {
        return 失败(状态::资源失败);
    }
}

std::optional<新方法混合实例信息> 新_方法类::获取混合方法实例(
    稳定编码 实例根节点) const noexcept {
    try {
        std::lock_guard 锁(新方法互斥);
        if (!结构仍存在()) return std::nullopt;
        const auto 信息 = 读取实例根结构(实例根节点, 特征概念服务_);
        if (!信息) return std::nullopt;
        // 候选之外的共享被调方法也必须完整可读，不能只确认根标签。
        for (const auto& 成员 : 全局基础数据集.查询目标字段(
            实例根节点, 方法实例根字段)) {
            const auto 方法 = 读取方法信息本体(成员.所属节点, 特征概念服务_, *this, 存在概念服务_, false, true);
            if (!方法 || !方法头有效(方法->方法头)
                || !参数规格有效(方法->参数规格组, 特征概念服务_)
                || (方法->实现方式 != 新方法实现方式::本能 && 方法->条件结果组.empty())
                || !std::all_of(方法->条件结果组.begin(), 方法->条件结果组.end(),
                    [&](const auto& 配对) {
                        const 新方法条件结果定义 定义{配对.条件, 配对.结果, 配对.线性关系组};
                        return 条件结果有效(定义, 特征概念服务_, true)
                            && 条件结果角色已声明(方法->参数规格组, 定义, 特征概念服务_, 存在概念服务_);
                    })) return std::nullopt;
        }
        return 信息;
    } catch (...) {
        return std::nullopt;
    }
}

新_方法类::新方法实例条件覆盖结果 新_方法类::覆盖实例配对条件值域(
    稳定编码 实例根节点, 稳定编码 方法节点,
    const 新方法条件结果信息& 预期配对,
    const std::vector<特征概念I64闭区间>& 所需条件域组) noexcept {
    using 状态 = 新方法实例条件覆盖状态;
    新方法实例条件覆盖结果 结果;
    std::vector<稳定编码> 新节点组, 新字段组;
    const auto 清理 = [](const auto& 字段组, const auto& 节点组) noexcept {
        bool 完整 = true;
        for (auto i = 字段组.rbegin(); i != 字段组.rend(); ++i)
            完整 = 全局基础数据集.删除字段(*i) && 完整;
        for (auto i = 节点组.rbegin(); i != 节点组.rend(); ++i)
            完整 = 全局基础数据集.删除节点(*i) && 完整;
        return 完整;
    };
    const auto 失败 = [&](状态 原因) {
        if (结果.已生效) {
            结果.状态 = 状态::已生效但结构不一致;
        } else {
            结果.状态 = 清理(新字段组, 新节点组) ? 原因 : 状态::结构不一致;
            结果.当前配对.reset();
        }
        return std::move(结果);
    };
    try {
        std::lock_guard 锁(新方法互斥);
        const auto 根 = 获取混合方法实例(实例根节点);
        const auto 方法 = 读取方法信息(方法节点, 特征概念服务_, *this, 存在概念服务_);
        if (!根 || !方法 || !方法->来源方法节点
            || 方法->实例根节点 != std::optional<稳定编码>{实例根节点}) return 失败(状态::入口拒绝);
        const auto 配对 = std::find_if(方法->条件结果组.begin(), 方法->条件结果组.end(),
            [&](const auto& 项) { return 项.配对节点 == 预期配对.配对节点; });
        if (配对 == 方法->条件结果组.end() || *配对 != 预期配对) return 失败(状态::来源已变化);
        if (所需条件域组.empty() || 所需条件域组.size() != 配对->条件.条件项组.size()
            || 配对->线性关系组.empty()) return 失败(状态::入口拒绝);
        if(std::any_of(配对->条件.条件项组.begin(),配对->条件.条件项组.end(),
            [](const auto& 项){return 项.引用内容条件.has_value()||项.特征前态限定.has_value();})
            ||std::any_of(配对->结果.结果项组.begin(),配对->结果.结果项组.end(),[](const auto& x){return x.引用内容结果.has_value();})
            ||含内容关系(配对->条件.参数关系组)||含内容关系(配对->结果.参数关系组))return 失败(状态::入口拒绝);
        auto 新条件 = 配对->条件;
        // 全组验证先于概念发布；不使用已经收窄的C重新定义比例。
        std::vector<特征概念信息> 类型组;
        std::vector<特征概念I64闭区间> 当前域组;
        for (std::size_t i = 0; i < 所需条件域组.size(); ++i) {
            const auto& 域 = 所需条件域组[i];
            const auto& 项 = 新条件.条件项组[i];
            const auto 关系 = std::find_if(配对->线性关系组.begin(), 配对->线性关系组.end(),
                [&](const auto& x) { return x.条件序号 == i; });
            const auto 当前域 = 读取线性项域(项.特征类型概念节点,
                项.特征值域概念节点, 项.精确数值域, 特征概念服务_);
            if (!是有限线性域(域) || !当前域 || 关系 == 配对->线性关系组.end()
                || 域.下界 < 关系->原条件域.下界 || 域.上界 > 关系->原条件域.上界)
                return 失败(状态::值域不支持);
            const auto 类型根 = 读取线性特征类型根(*项.特征类型概念节点, 特征概念服务_);
            const auto 类型 = 类型根 ? 特征概念服务_.获取特征概念(*类型根) : std::nullopt;
            if (!类型 || 类型->材料类型 != 特征概念材料物理类型::I64标量) return 失败(状态::结构不一致);
            const auto* 根域 = std::get_if<std::vector<特征概念I64闭区间>>(&类型->值域);
            if (!根域 || !std::any_of(根域->begin(), 根域->end(), [&](const auto& x) {
                return x.下界 <= 域.下界 && x.上界 >= 域.上界;
            })) return 失败(状态::值域不支持);
            类型组.push_back(*类型);
            当前域组.push_back(*当前域);
        }
        for (std::size_t i = 0; i < 所需条件域组.size(); ++i) {
            auto& 项 = 新条件.条件项组[i];
            if (项.特征值域概念节点 && 当前域组[i] == 所需条件域组[i]) continue;
            const auto& 类型 = 类型组[i];
            const std::vector<特征概念I64闭区间> 域组{所需条件域组[i]};
            稳定编码 概念;
            if (类型.值域 == 特征概念值域{域组}) 概念 = 类型.节点;
            else {
                const 特征概念定义 定义{类型.材料类型, 域组, 类型.单位,
                    类型.比较规则, 类型.聚合规则, {}};
                概念 = 特征概念服务_.建立或取得特征概念(定义, 特征值服务_, 类型.节点);
            }
            if (!有效(概念)) return 失败(状态::资源失败);
            const auto 读回 = 特征概念服务_.获取特征概念(概念);
            if (!读回 || 读回->值域 != 特征概念值域{域组} || 读回->单位 != 类型.单位
                || 读取线性特征类型根(概念, 特征概念服务_) != std::optional<稳定编码>{类型.节点})
                return 失败(状态::结构不一致);
            项.特征值域概念节点 = 概念;
            项.精确数值域.reset();
        }
        if (新条件 == 配对->条件) return {状态::无变化, false, *配对};
        if (!条件结果有效({新条件, 配对->结果, 配对->线性关系组}, 特征概念服务_, true))
            return 失败(状态::结构不一致);

        const auto 条件引用 = 读取唯一节点字段记录(配对->配对节点, 配对条件字段);
        const auto 项节点组 = 读取有序子节点(配对->条件节点, 条件项字段, 特征概念服务_);
        if (!条件引用 || 条件引用->目标 != 配对->条件节点 || !项节点组)
            return 失败(状态::结构不一致);
        std::vector<稳定编码> 旧节点组{配对->条件节点}, 旧字段组;
        旧节点组.insert(旧节点组.end(), 项节点组->begin(), 项节点组->end());
        const auto 旧限定组=读取有序子节点(配对->条件节点,初始场景限定字段,特征概念服务_);
        if(!旧限定组)return 失败(状态::结构不一致);
        旧节点组.insert(旧节点组.end(),旧限定组->begin(),旧限定组->end());
        for (const auto 节点 : 旧节点组) {
            for (const auto 关系类型 : {基础外部关系类型::父子, 基础外部关系类型::兄弟}) {
                if (!全局基础数据集.查询源关系(节点, 关系类型).empty()
                    || !全局基础数据集.查询目标关系(节点, 关系类型).empty()) return 失败(状态::结构不一致);
            }
            std::size_t 持有数 = 0;
            const bool 是C = 节点 == 配对->条件节点;
            const bool 是限定=std::find(旧限定组->begin(),旧限定组->end(),节点)!=旧限定组->end();
            // 公开L1只能按已知字段查询；未知非法引用由删除拒绝保留，不冒充已证明不存在。
            for (const auto* 字段 : 架构编码组()) {
                for (const auto& 持有 : 全局基础数据集.查询目标字段(节点, *字段)) {
                    if (持有.所属节点 != (是C ? 配对->配对节点 : 配对->条件节点)
                        || 持有.字段节点 != (是C ? 配对条件字段 : 是限定 ? 初始场景限定字段 : 条件项字段)) return 失败(状态::结构不一致);
                    ++持有数;
                }
            }
            if (持有数 != 1) return 失败(状态::结构不一致);
            for (const auto& 规格 : 方法复制字段规格(是C ? 条件节点类型 : 是限定 ? 初始场景限定节点类型 : 条件项节点类型))
                for (const auto& 字段 : 全局基础数据集.查询字段(节点, 规格.字段)) 旧字段组.push_back(字段.编码);
        }
        // 保证建立过程中登记新节点/字段不再因容器扩容而漏记。
        新节点组.reserve(1 + 新条件.条件项组.size());
        新字段组.reserve(2 + 新条件.条件项组.size() * 10);
        const auto 新C = 写入条件子树(新条件, 新节点组, 新字段组);
        if (!有效(新C)) return 失败(状态::资源失败);
        const auto 暂存 = 读取条件(新C, 特征概念服务_);
        if (!暂存 || *暂存 != 新条件) return 失败(状态::结构不一致);
        if (!全局基础数据集.修改字段节点(条件引用->关系, 新C)) return 失败(状态::资源失败);
        结果.已生效 = true;
        const auto 读回 = 读取方法信息本体(方法节点, 特征概念服务_, *this, 存在概念服务_, false, true);
        if (!读回) return 失败(状态::已生效但结构不一致);
        const auto 新配对 = std::find_if(读回->条件结果组.begin(), 读回->条件结果组.end(),
            [&](const auto& x) { return x.配对节点 == 配对->配对节点; });
        if (新配对 == 读回->条件结果组.end() || 新配对->条件节点 != 新C
            || 新配对->条件 != 新条件 || 新配对->结果节点 != 配对->结果节点
            || 新配对->结果 != 配对->结果 || 新配对->线性关系组 != 配对->线性关系组
            || 新配对->命中次数 != 配对->命中次数) return 失败(状态::已生效但结构不一致);
        结果.当前配对 = *新配对;
        if (!清理(旧字段组, 旧节点组)) return 失败(状态::已生效但结构不一致);
        结果.状态 = 状态::已覆盖;
        return 结果;
    } catch (const 引用内容定义前提变化&) {
        return 失败(状态::来源已变化);
    } catch (...) {
        return 失败(状态::资源失败);
    }
}

新方法实例配对结果 新_方法类::查询实例条件结果(
    稳定编码 实例根节点) const noexcept {
    try {
        std::lock_guard 锁(新方法互斥);
        if (!有效(实例根节点)) return {新方法操作状态::入口拒绝, {}};
        if (!结构仍存在()) return {新方法操作状态::尚未初始化, {}};
        const auto 信息 = 获取混合方法实例(实例根节点);
        if (!信息) return {新方法操作状态::结构不一致, {}};
        新方法实例配对结果 结果;
        for (const auto 方法节点 : 信息->候选方法实例根组) {
            const auto 方法 = 读取方法信息本体(方法节点, 特征概念服务_, *this, 存在概念服务_, false, true);
            if (!方法) return {新方法操作状态::结构不一致, {}};
            for (const auto& 配对 : 方法->条件结果组) {
                结果.配对组.push_back({方法节点, 配对});
            }
        }
        结果.状态 = 新方法操作状态::已找到;
        return 结果;
    } catch (const 引用内容定义前提变化&) {
        return {新方法操作状态::定义前提已变化, {}};
    } catch (...) {
        return {新方法操作状态::资源失败, {}};
    }
}

bool 新_方法类::是方法节点(稳定编码 节点) const noexcept {
    try {
        std::lock_guard 锁(新方法互斥);
        return 读取方法信息(节点, 特征概念服务_, *this, 存在概念服务_).has_value();
    } catch (...) {
        return false;
    }
}


新方法候选结果 新_方法类::查询候选方法(
    const 新方法候选查询& 查询) const noexcept {
#ifdef HY_EGO_DEBUG_LOG_METHOD_LOOKUP
    // 守卫在业务锁外创建，所有返回路径均先释放方法锁，再输出可选投影。
    struct 查找监控 final {
        std::wstring 内容;
        bool 已截断 = false;
        void 追加(std::wstring_view 行) noexcept {
            try {
                if (已截断) return;
                if (行.size() > 65536 || 内容.size() > 65536 - 行.size()) {
                    内容 += L"监控文本已截断；不裁剪实际方法遍历或查询结果。\n";
                    已截断 = true;
                    return;
                }
                内容.append(行);
                内容 += L'\n';
            } catch (...) {} // 仅丢失诊断，不改变业务结果。
        }
        ~查找监控() noexcept {
            try {
                (void)调试日志<调试切片::METHOD_LOOKUP>(L"方法查找", L"查询候选方法", 内容);
            } catch (...) {}
        }
    } 监控;
    const auto 记录监控 = [&](const auto& 写入) noexcept {
        try {
            std::wostringstream 行;
            写入(行);
            监控.追加(行.str());
        } catch (...) {}
    };
    const auto 写编码组 = [](auto& 文本, const auto& 组) {
        文本 << L"数量=" << 组.size() << L" [";
        for (std::size_t i = 0; i < 组.size() && i < 8; ++i) 文本 << 组[i].值 << L",";
        if (组.size() > 8) 文本 << L"后续编码省略";
        文本 << L"]";
    };
    记录监控([&](auto& 文本) {
        文本 << L"查询开始；所需FT="; 写编码组(文本, 查询.涉及特征类型概念组);
        文本 << L"；所需存在概念="; 写编码组(文本, 查询.作用存在概念组);
        文本 << L"；所需结果概念="; 写编码组(文本, 查询.可能结果概念组);
    });
#endif
    新方法候选结果 结果;
    try {
        std::unique_lock 锁(新方法互斥);
        if (!结构仍存在()) {
            结果.状态 = 新方法操作状态::尚未初始化;
            return 结果;
        }
        if ((查询.作用域
                && !是作用域(static_cast<std::int64_t>(*查询.作用域)))
            || (查询.实现方式
                && !是实现方式(static_cast<std::int64_t>(*查询.实现方式)))
            || !引用组有效(查询.作用存在概念组)
            || !引用组有效(查询.涉及特征类型概念组)
            || !引用组有效(查询.可能结果概念组)) return 结果;
        // 撕裂读取（多次基础读取之间遇并发发布）：任一成员方法读不成或遇引用内容定义变化时，释放方法锁让发布者
        // 完成，再从成员组起整轮重查（结果全部重建，不拼接不同时刻的筛选）；总共至多16轮，仍不成按原状态报告，
        // 持久损坏不被当作暂态。
        for (int 轮 = 0;; ++轮) {
        if (轮) {
            锁.unlock();
            std::this_thread::yield();
            锁.lock();
            if (!结构仍存在()) { 结果.状态 = 新方法操作状态::尚未初始化; 结果.方法节点组.clear(); return 结果; }
        }
        结果.方法节点组.clear();
        bool 重查 = false;
        const auto 成员组 = 读取多节点字段(新方法组织根节点, 方法成员字段, 特征概念服务_);
        if (!成员组) {
            结果.状态 = 新方法操作状态::结构不一致;
            return 结果;
        }
#ifdef HY_EGO_DEBUG_LOG_METHOD_LOOKUP
        记录监控([&](auto& 文本) {
            文本 << L"方法根=" << 新方法组织根节点.值 << L"；正式成员数量=" << 成员组->size();
        });
#endif
        for (const auto 方法节点 : *成员组) {
            std::optional<新方法信息> 方法;
            bool 定义变化 = false;
            try { 方法 = 读取方法信息本体(方法节点, 特征概念服务_, *this, 存在概念服务_, false, true); }
            catch (const 引用内容定义前提变化&) { 定义变化 = true; }
            if (!方法 && 轮 < 15 && (定义变化 || (节点类型为(方法节点, 方法节点类型) && 方法属于根(方法节点)))) {
                重查 = true;
                break;
            }
            if (定义变化) throw 引用内容定义前提变化{};
            if (!方法) {
#ifdef HY_EGO_DEBUG_LOG_METHOD_LOOKUP
                记录监控([&](auto& 文本) { 文本 << L"遍历方法=" << 方法节点.值 << L"；方法读取失败"; });
#endif
#ifdef HY_EGO_DEBUG_LOG_SIMULATION_PROGRESS
                try { (void)调试日志<调试切片::SIMULATION_PROGRESS>(L"查询候选方法", L"成员方法读不成",
                    L"方法=" + std::to_wstring(方法节点.值) + L"；返回行=" + std::to_wstring(方法读失败行)); } catch (...) {}
#endif
                结果.状态 = 新方法操作状态::结构不一致;
                结果.方法节点组.clear();
                return 结果;
            }
#ifdef HY_EGO_DEBUG_LOG_METHOD_LOOKUP
            记录监控([&](auto& 文本) {
                文本 << L"已读取方法头=" << 方法节点.值
                    << L"；启用状态=" << static_cast<int>(方法->启用状态)
                    << L"；来源副本=" << 方法->来源方法节点.has_value()
                    << L"；条件结果对数量=" << 方法->条件结果组.size() << L"；头FT=";
                写编码组(文本, 方法->方法头.涉及特征类型概念组);
            });
#endif
            if (方法->来源方法节点 || 方法->启用状态 != 新方法启用状态::启用
                || (查询.作用域 && 方法->作用域 != *查询.作用域)
                || (查询.实现方式 && 方法->实现方式 != *查询.实现方式)
                || !包含全部(方法->方法头.作用存在概念组,
                    查询.作用存在概念组)
                || !包含全部(方法->方法头.涉及特征类型概念组,
                    查询.涉及特征类型概念组)
                || !包含全部(方法->方法头.可能结果概念组,
                    查询.可能结果概念组)) {
#ifdef HY_EGO_DEBUG_LOG_METHOD_LOOKUP
                记录监控([&](auto& 文本) {
                    const wchar_t* 原因 = 方法->来源方法节点 ? L"方法副本"
                        : 方法->启用状态 != 新方法启用状态::启用 ? L"方法未启用"
                        : 查询.作用域 && 方法->作用域 != *查询.作用域 ? L"作用域不匹配"
                        : 查询.实现方式 && 方法->实现方式 != *查询.实现方式 ? L"实现方式不匹配"
                        : !包含全部(方法->方法头.作用存在概念组, 查询.作用存在概念组) ? L"作用存在概念不匹配"
                        : !包含全部(方法->方法头.涉及特征类型概念组, 查询.涉及特征类型概念组) ? L"目标特征类型不匹配"
                        : L"结果概念不匹配";
                    文本 << L"排除方法=" << 方法节点.值 << L"；原因=" << 原因;
                });
#endif
                continue;
            }
            结果.方法节点组.push_back(方法节点);
#ifdef HY_EGO_DEBUG_LOG_METHOD_LOOKUP
            记录监控([&](auto& 文本) { 文本 << L"纳入候选方法=" << 方法节点.值; });
#endif
        }
        if (重查) continue;
        break;
        }
        结果.状态 = 新方法操作状态::已找到;
#ifdef HY_EGO_DEBUG_LOG_METHOD_LOOKUP
        记录监控([&](auto& 文本) { 文本 << L"查询完成；候选数量=" << 结果.方法节点组.size(); });
#endif
        return 结果;
    } catch (const 引用内容定义前提变化&) {
        结果.状态 = 新方法操作状态::定义前提已变化;
        结果.方法节点组.clear();
        return 结果;
    } catch (...) {
        结果.状态 = 新方法操作状态::资源失败;
        结果.方法节点组.clear();
        return 结果;
    }
}

新方法操作状态 新_方法类::修改启用状态(
    稳定编码 方法节点,
    新方法启用状态 新状态) noexcept {
    const auto 结果 = 修改启用状态内部(方法节点, 新状态);
    发出方法状态通知();
    return 结果;
}

新方法操作状态 新_方法类::修改启用状态内部(
    稳定编码 方法节点,
    新方法启用状态 新状态) noexcept {
    新特征I64准备结果 准备;
    std::optional<稳定编码> 前状态;
    bool 已证零发表 = true;
    try {
        std::lock_guard 锁(新方法互斥);
        if (!结构仍存在()) return 新方法操作状态::尚未初始化;
        核验治理组织就绪();
        const auto 方法 = 读取方法信息本体(方法节点, 特征概念服务_, *this, 存在概念服务_, false, true);
        if (!方法) return 新方法操作状态::方法不存在;
        const 新方法虚拟存在引用 引用{方法节点, 方法->方法虚拟存在节点, 方法->启用特征节点};
        if (!内部治理世界_->读取方法治理场景(引用)) throw std::runtime_error("method scene missing before enabled update");
        if (新状态 != 新方法启用状态::停用
            && 新状态 != 新方法启用状态::启用) {
            return 新方法操作状态::入口拒绝;
        }
        if (方法->启用状态 == 新状态) return 新方法操作状态::无变化;
        const auto 退役组 = 读取旧网页退役成员(特征概念服务_);
        if (新状态 == 新方法启用状态::启用
            && std::find(退役组.begin(), 退役组.end(), 方法节点) != 退役组.end())
            return 新方法操作状态::入口拒绝;
        if (方法->本能函数登记名 == 新本能函数登记名_旧两参数获取服务对象
            && (方法->父代方法节点 || !全局基础数据集.查询目标字段(方法节点, 方法父代字段).empty()
                || 是有序服务对象规格(方法->参数规格组)))
            return 新方法操作状态::入口拒绝;
        const auto 前时间 = 方法当前UTC纳秒();
        const auto s0 = 状态服务_.建立状态(引用.虚拟存在节点, 引用.启用特征节点,
            {新状态时间语义::实例绝对UTC, 前时间});
        if (s0.状态 != 新状态操作状态::已完成 || !s0.状态节点)
            throw std::runtime_error("method enabled before state unavailable");
        前状态 = s0.状态节点;
        const auto 新值 = static_cast<std::int64_t>(新状态);
        准备 = 特征服务_.准备顶层I64更新(引用.启用特征节点, 新值);
        if (准备.状态 != 新特征I64准备状态::已准备 || !准备.准备
            || 准备.准备->原当前值() != static_cast<std::int64_t>(方法->启用状态)
            || !准备.准备->核心有变化()) throw std::runtime_error("method enabled update not prepared");
        const auto 动作时间 = 方法当前UTC纳秒();
        已证零发表 = false;
        const auto commit = 特征服务_.提交顶层I64更新(*准备.准备, {});
        if (commit.状态 != 基础节点组追加状态::已完成) {
            if (!commit.附加片段回执组.empty()) throw std::runtime_error("method feature ambiguous publication");
            已证零发表 = true;
            throw std::runtime_error("method enabled update zero publication");
        }
        if (!commit.附加片段回执组.empty()) throw std::runtime_error("method feature unexpected receipts");
        const auto finish = 特征服务_.完成顶层I64更新(*准备.准备);
        if (finish.状态 != 新特征写入状态::已完成 || !finish.当前值已改变)
            throw std::runtime_error("method enabled feature finalization failed");
        特征服务_.结束顶层I64更新(*准备.准备);
        同步方法专属概念(引用);
        // METHOD-LEARN-STATE：启用修改经方法owner统一入口形成学习记录、治理动作、后状态并交付因果。
        (void)前时间;
        形成方法状态变化已加锁(引用, 新方法学习种类::启用修改, 新方法状态特征种类::启用,
            引用.启用特征节点, static_cast<std::int64_t>(方法->启用状态), 新值, *前状态, 动作时间, {}, {});
        const auto v = 读取方法虚拟存在引用(引用);
        if (!v || v->当前启用 != 新值)
            throw std::runtime_error("method enabled update final readback failed");
        return 新方法操作状态::已修改;
    } catch (const 引用内容定义前提变化&) {
        return 新方法操作状态::定义前提已变化;
    } catch (...) {
        if (已证零发表) {
            if (准备.准备) 特征服务_.取消顶层I64更新(*准备.准备);
            if (前状态 && 状态服务_.删除状态(*前状态) != 新状态操作状态::已完成)
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"修改方法启用", L"零发表前状态清理失败。");
        }
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"修改方法启用", L"方法启用特征或状态动态未完整读回，保留真实变化。");
    }
}

新方法结构节点结果 新_方法类::添加组合步骤(
    const 新方法组合步骤添加请求& 请求) noexcept {
    新方法结构节点结果 结果;
    try {
        std::lock_guard 锁(新方法互斥);
        if (!结构仍存在()) {
            结果.状态 = 新方法操作状态::尚未初始化;
            return 结果;
        }
        const auto 组合 = 读取方法信息本体(请求.组合方法节点, 特征概念服务_, *this, 存在概念服务_, false, true);
        const auto 调用 = 读取方法信息本体(请求.调用方法节点, 特征概念服务_, *this, 存在概念服务_, false, true);
        if (!组合 || !调用) {
            结果.状态 = 新方法操作状态::方法不存在;
            return 结果;
        }
        if (组合->实现方式 != 新方法实现方式::组合
            || 调用->实现方式 != 新方法实现方式::本能
            || 组合->实例根节点 != 调用->实例根节点
            || 组合->作用域 != 调用->作用域 || 请求.顺序 < 0
            || 请求.组合方法节点 == 请求.调用方法节点) {
            结果.状态 = 新方法操作状态::方法类型不符;
            return 结果;
        }
        for (const auto& 步骤 : 组合->组合步骤组) {
            if (步骤.顺序 == 请求.顺序) {
                if (步骤.调用方法节点 == 请求.调用方法节点) {
                    结果.状态 = 新方法操作状态::已找到;
                    结果.节点 = 步骤.步骤节点;
                } else {
                    结果.状态 = 新方法操作状态::结构不一致;
                }
                return 结果;
            }
        }
        std::vector<稳定编码> 已建节点;
        std::vector<稳定编码> 已写字段;
        const auto 步骤 = 新建类型化节点(
            组合步骤节点类型, 已建节点, 已写字段);
        bool 完整 = 有效(步骤)
            && 写入节点字段(步骤, 步骤所属组合方法字段,
                请求.组合方法节点, 已写字段)
            && 写入节点字段(步骤, 步骤调用方法字段,
                请求.调用方法节点, 已写字段)
            && 写入I64字段(步骤, 顺序字段, 请求.顺序, 已写字段)
            && 写入节点字段(请求.组合方法节点, 方法组合步骤字段,
                步骤, 已写字段);
        if (完整 && !组合->组合入口步骤节点) {
            完整 = 写入节点字段(请求.组合方法节点,
                方法组合入口步骤字段, 步骤, 已写字段);
        }
        if (!完整) {
            回滚(已写字段, 已建节点);
            结果.状态 = 新方法操作状态::资源失败;
            return 结果;
        }
        结果.状态 = 新方法操作状态::已建立;
        结果.节点 = 步骤;
        return 结果;
    } catch (const 引用内容定义前提变化&) {
        结果.状态 = 新方法操作状态::定义前提已变化;
        return 结果;
    } catch (...) {
        结果.状态 = 新方法操作状态::资源失败;
        return 结果;
    }
}

新方法操作状态 新_方法类::设置组合入口步骤(
    稳定编码 组合方法节点,
    稳定编码 步骤节点) noexcept {
    try {
        std::lock_guard 锁(新方法互斥);
        const auto 方法 = 读取方法信息(组合方法节点, 特征概念服务_, *this, 存在概念服务_);
        if (!方法) return 新方法操作状态::方法不存在;
        if (方法->实现方式 != 新方法实现方式::组合) {
            return 新方法操作状态::方法类型不符;
        }
        const auto 找到 = std::find_if(方法->组合步骤组.begin(),
            方法->组合步骤组.end(), [&](const 新方法组合步骤信息& 步骤) {
                return 步骤.步骤节点 == 步骤节点;
            });
        if (找到 == 方法->组合步骤组.end()) return 新方法操作状态::步骤不存在;
        if (方法->组合入口步骤节点 == 步骤节点) return 新方法操作状态::无变化;
        const auto 当前组 = 全局基础数据集.查询字段(
            组合方法节点, 方法组合入口步骤字段);
        if (当前组.size() != 1) return 新方法操作状态::结构不一致;
        return 全局基础数据集.修改字段节点(
                当前组.front().编码, 步骤节点)
            ? 新方法操作状态::已修改 : 新方法操作状态::资源失败;
    } catch (const 引用内容定义前提变化&) {
        return 新方法操作状态::定义前提已变化;
    } catch (...) {
        return 新方法操作状态::资源失败;
    }
}

新方法结构节点结果 新_方法类::添加结果转移(
    const 新方法结果转移添加请求& 请求) noexcept {
    新方法结构节点结果 结果;
    try {
        std::lock_guard 锁(新方法互斥);
        const auto 组合 = 读取方法信息(请求.组合方法节点, 特征概念服务_, *this, 存在概念服务_);
        if (!组合) {
            结果.状态 = 新方法操作状态::方法不存在;
            return 结果;
        }
        if (组合->实现方式 != 新方法实现方式::组合
            || !可选引用有效(请求.匹配结果概念节点)) {
            结果.状态 = 新方法操作状态::方法类型不符;
            return 结果;
        }
        const auto 当前步骤 = std::find_if(组合->组合步骤组.begin(),
            组合->组合步骤组.end(), [&](const 新方法组合步骤信息& 步骤) {
                return 步骤.步骤节点 == 请求.当前步骤节点;
            });
        const auto 下一步骤 = std::find_if(组合->组合步骤组.begin(),
            组合->组合步骤组.end(), [&](const 新方法组合步骤信息& 步骤) {
                return 步骤.步骤节点 == 请求.下一步骤节点;
            });
        if (当前步骤 == 组合->组合步骤组.end()
            || 下一步骤 == 组合->组合步骤组.end()) {
            结果.状态 = 新方法操作状态::步骤不存在;
            return 结果;
        }
        const auto 被调方法 = 读取方法信息本体(当前步骤->调用方法节点, 特征概念服务_, *this, 存在概念服务_, false, true);
        if (!被调方法) {
            结果.状态 = 新方法操作状态::结构不一致;
            return 结果;
        }
        bool 结果属于方法 = false;
        for (const auto& 配对 : 被调方法->条件结果组) {
            if (配对.结果节点 == 请求.当前结果节点) {
                结果属于方法 = true;
                break;
            }
        }
        if (!结果属于方法) {
            结果.状态 = 新方法操作状态::结果节点不属于步骤方法;
            return 结果;
        }
        for (const auto& 转移 : 组合->结果转移组) {
            if (转移.当前步骤节点 == 请求.当前步骤节点
                && 转移.当前结果节点 == 请求.当前结果节点
                && 转移.匹配结果概念节点 == 请求.匹配结果概念节点) {
                if (转移.下一步骤节点 == 请求.下一步骤节点) {
                    结果.状态 = 新方法操作状态::已找到;
                    结果.节点 = 转移.转移节点;
                } else {
                    结果.状态 = 新方法操作状态::结构不一致;
                }
                return 结果;
            }
        }
        std::vector<稳定编码> 已建节点;
        std::vector<稳定编码> 已写字段;
        const auto 转移 = 新建类型化节点(
            结果转移节点类型, 已建节点, 已写字段);
        bool 完整 = 有效(转移)
            && 写入节点字段(转移, 转移所属组合方法字段,
                请求.组合方法节点, 已写字段)
            && 写入节点字段(转移, 转移当前步骤字段,
                请求.当前步骤节点, 已写字段)
            && 写入节点字段(转移, 转移当前结果字段,
                请求.当前结果节点, 已写字段)
            && 写入节点字段(转移, 转移下一步骤字段,
                请求.下一步骤节点, 已写字段);
        if (完整 && 请求.匹配结果概念节点) {
            完整 = 写入节点字段(转移, 转移匹配结果概念字段,
                *请求.匹配结果概念节点, 已写字段);
        }
        if (完整) 完整 = 写入节点字段(请求.组合方法节点,
            方法结果转移字段, 转移, 已写字段);
        if (!完整) {
            回滚(已写字段, 已建节点);
            结果.状态 = 新方法操作状态::资源失败;
            return 结果;
        }
        结果.状态 = 新方法操作状态::已建立;
        结果.节点 = 转移;
        return 结果;
    } catch (const 引用内容定义前提变化&) {
        结果.状态 = 新方法操作状态::定义前提已变化;
        return 结果;
    } catch (...) {
        结果.状态 = 新方法操作状态::资源失败;
        return 结果;
    }
}

bool 新_方法类::装配正式信息参数读取接口(const 正式信息参数读取接口* reader) noexcept {
    std::lock_guard lock(新方法互斥);
    if(reader&&正式信息读取_&&reader!=正式信息读取_)return false;
    正式信息读取_=reader;
    return true;
}
void 新_方法类::装配词条读取(const 新_词类& 词条) noexcept {
    std::lock_guard 锁(新方法互斥);
    if (词条读取_ && 词条读取_ != &词条)
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"装配词条读取", L"运行期不能更换词条读取所有者。");
    词条读取_ = &词条;
}
bool 新_方法类::装配字段询问运行接口(std::shared_ptr<新本能字段询问运行接口> p) noexcept {
    std::lock_guard lock(新方法互斥);
    if (!p || (字段询问运行_ && 字段询问运行_!=p)) return false;
    if (!字段询问运行_ && !退役字段询问运行_.expired()) return false;
    字段询问运行_=std::move(p);
    return true;
}
bool 新_方法类::解除字段询问运行接口(const 新本能字段询问运行接口& p) noexcept {
    std::lock_guard lock(新方法互斥);
    if (!字段询问运行_) return 退役字段询问运行_.lock().get()==&p;
    if (字段询问运行_.get()!=&p) return false;
    退役字段询问运行_=字段询问运行_;
    字段询问运行_.reset();
    return true;
}

新方法参数场景结果 新_方法类::建立参数场景(
    稳定编码 方法节点,
    std::vector<新方法参数角色绑定> 角色绑定组,
    const 新内部治理对象读取接口* 治理对象读取,
    const 新服务要求材料读取接口* 服务要求读取,
    std::optional<新方法任务成员绑定> 任务成员绑定,std::optional<稳定编码> 执行分支,
    std::shared_ptr<新本能需求运行接口> 需求运行,
    std::shared_ptr<新本能词典运行接口> 词典运行) const noexcept {
    for (;;) {
    新方法参数场景结果 结果;
    try {
        std::optional<新方法信息> 方法;
        const 正式信息参数读取接口* reader=nullptr;
        const 新_词类* 词条读取=nullptr;
        std::shared_ptr<新本能字段询问运行接口> 字段询问运行;
        std::vector<基础联合节点快照> 正式保护;
        {
            // 这里只在复制方法规格时持有本类锁；后续调用场景、存在、
            // 特征与概念服务时不跨服务持锁。
            std::lock_guard 锁(新方法互斥);
            方法 = 读取方法信息(方法节点, 特征概念服务_, *this, 存在概念服务_);
            reader=正式信息读取_;
            词条读取=词条读取_;
            字段询问运行=字段询问运行_;
        }
        if (!方法) {
            结果.状态 = 新方法操作状态::方法不存在;
            return 结果;
        }
        if(!方法->执行分支组.empty()||执行分支) {
            const auto 分支读取=读取执行分支必要条件(方法节点,执行分支);
            if(分支读取.状态!=新方法执行分支读取状态::已读取) {
                if(分支读取.状态==新方法执行分支读取状态::前提已变化)throw 引用内容定义前提变化{};
                结果.状态=分支读取.状态==新方法执行分支读取状态::方法不存在
                        ? 新方法操作状态::方法不存在 : 新方法操作状态::参数概念不相容;
                return 结果;
            }
        }
        if (!方法->参数规格组.empty() && 方法->参数规格组.front().有序规格) {
            if (!参数规格有效(方法->参数规格组, 特征概念服务_))
                throw std::logic_error("ordered method parameter declarations are corrupt");
            if (角色绑定组.size() != 方法->参数规格组.size()) {
                结果.状态 = 新方法操作状态::参数角色缺失; return 结果;
            }
            for (std::size_t i = 0; i < 角色绑定组.size(); ++i) {
                const auto& 绑定 = 角色绑定组[i];
                const auto& 规格 = *方法->参数规格组[i].有序规格;
                if (!绑定.有序引用 || 有效(绑定.参数角色节点) || 绑定.实际存在节点
                    || !绑定.实际特征组.empty() || 绑定.任务虚拟存在引用
                    || 绑定.服务要求分项引用 || 绑定.服务要求确认引用
                    || 绑定.有序引用->序号 != 规格.序号
                    || 绑定.有序引用->信息类型 != 规格.信息类型) {
                    结果.状态 = 新方法操作状态::参数概念不相容; return 结果;
                }
                const auto 节点 = 绑定.有序引用->信息节点;
                if (!节点仍存在(节点)) {
                    结果.状态 = 新方法操作状态::参数引用失效; return 结果;
                }
                const bool 词条场景=规格.信息类型==新方法参数信息类型::词条
                    ||(规格.信息类型==新方法参数信息类型::特征&&规格.所属存在位置
                        &&方法->参数规格组[规格.所属存在位置-1].有序规格->信息类型==新方法参数信息类型::词条);
                if (!词条场景 && 绑定.世界场景节点 && !场景服务_.是场景节点(*绑定.世界场景节点)) {
                    结果.状态 = 新方法操作状态::参数引用失效; return 结果;
                }
                if (规格.信息类型 == 新方法参数信息类型::词条) {
                    const auto 当前=读取方法词条实参(词条读取,规格,节点,存在概念服务_,特征服务_);
                    if(当前.first!=新方法操作状态::已找到){结果.状态=当前.first;return 结果;}
                    if(绑定.世界场景节点!=std::nullopt
                        &&绑定.世界场景节点!=当前.second->所属自然语言场景) {
                        结果.状态=新方法操作状态::参数概念不相容;return 结果;
                    }
                } else if (规格.信息类型 == 新方法参数信息类型::存在) {
                    const auto 普通定义=存在概念服务_.读取普通存在概念定义保护(规格.范围概念节点);
                    if(普通定义.状态==正式概念读取状态::前提已变化)throw 引用内容定义前提变化{};
                    if(普通定义.状态==正式概念读取状态::非本提供者概念) {
                        if(!追加方法存在范围保护(规格.范围概念节点,存在概念服务_,正式保护))
                            throw std::logic_error("formal parameter range declaration unavailable");
                        if(!reader)throw std::logic_error("formal parameter reader not assembled");
                        const auto read=reader->读取正式信息参数保护(节点,规格.范围概念节点);
                        const auto state=正式参数状态(read,节点);
                        if(state==新方法操作状态::定义前提已变化)throw 引用内容定义前提变化{};
                        if(state!=新方法操作状态::已找到){结果.状态=state;return 结果;}
                        正式保护.insert(正式保护.end(),read.完整保护组.begin(),read.完整保护组.end());
                        continue;
                    }
                    if(普通定义.状态!=正式概念读取状态::已读取||!普通定义.信息)
                        throw std::logic_error("ordinary parameter range declaration unavailable");
                    // 场景本身也是存在（与筹办 核验筹办世界对象概念 一致）：存在位置接受场景节点。
                    if (!存在服务_.是存在节点(节点) && !场景服务_.是场景节点(节点)) {
                        结果.状态 = 新方法操作状态::参数概念不相容; return 结果;
                    }
                    const auto 概念 = 存在概念服务_.获取存在概念(规格.范围概念节点);
                    if (!概念) throw std::logic_error("ordered existence concept unavailable");
                    if (概念->种类 == 存在概念种类::专属 && !概念->对应存在节点)
                        throw std::logic_error("ordered dedicated concept has no existence");
                    // 参数类型归属（2026-11-10用户裁决“不做无意义的判断”，与筹办侧 核验参数类型归属 同一口径）：
                    // 对象具备范围概念定义的全部特征类型即属于；特征有无值、取值是否符合由条件核验。
                    const auto 匹配 = [&] {
                        if (概念->种类 == 存在概念种类::专属)
                            return *概念->对应存在节点 == 节点 ? 新存在概念匹配状态::匹配 : 新存在概念匹配状态::不符合;
                        const auto 比较 = 存在服务_.比较存在与概念信息(节点, 规格.范围概念节点);
                        if (比较.状态 == 特征概念规则状态::资源失败) throw std::bad_alloc{};
                        if (比较.状态 == 特征概念规则状态::前提已变化) throw 引用内容定义前提变化{};
                        if (比较.状态 != 特征概念规则状态::已完成)
                            throw std::logic_error("执行参数对象归属判定入口不成立");
                        if (比较.归属 == 特征成员概念归属::属于) return 新存在概念匹配状态::匹配;
                        if (比较.归属 == 特征成员概念归属::不属于) return 新存在概念匹配状态::不符合;
                        return 比较.缺少特征类型组.empty() ? 新存在概念匹配状态::匹配 : 新存在概念匹配状态::材料不足;
                    }();
                    if (匹配 != 新存在概念匹配状态::匹配) {
                        结果.状态 = 匹配 == 新存在概念匹配状态::材料不足
                            ? 新方法操作状态::参数材料未到 : 新方法操作状态::参数概念不相容;
                        return 结果;
                    }
                } else {
                    const auto 特征 = 特征服务_.获取特征(节点);
                    if (!特征) { 结果.状态 = 新方法操作状态::参数引用失效; return 结果; }
                    if (!特征概念为或从属于(特征->特征概念节点, 规格.范围概念节点, 特征概念服务_)) {
                        结果.状态 = 新方法操作状态::参数概念不相容; return 结果;
                    }
                    const auto 所属=规格.所属存在位置;
                    if(所属 && 所属<=角色绑定组.size()
                        &&方法->参数规格组[所属-1].有序规格->信息类型==新方法参数信息类型::词条) {
                        const auto& 所属绑定=角色绑定组[所属-1].有序引用;
                        if(!所属绑定){结果.状态=新方法操作状态::参数角色缺失;return 结果;}
                        const auto 当前=读取方法词条实参(词条读取,*方法->参数规格组[所属-1].有序规格,
                            所属绑定->信息节点,存在概念服务_,特征服务_);
                        if(当前.first!=新方法操作状态::已找到){结果.状态=当前.first;return 结果;}
                        if(std::none_of(当前.second->特征组.begin(), 当前.second->特征组.end(),
                            [&](const auto& 项) { return 项.特征节点 == 节点; })
                            ||(绑定.世界场景节点&&绑定.世界场景节点!=当前.second->所属自然语言场景)) {
                            结果.状态=新方法操作状态::参数概念不相容;return 结果;
                        }
                    }
                }
            }
            结果.参数场景 = std::shared_ptr<const 方法参数场景>(new 方法参数场景(
                方法节点, std::move(角色绑定组), 特征服务_, 特征值服务_,
                新方法参数场景用途::执行, 治理对象读取, {}, 服务要求读取, {}, 任务成员绑定,执行分支,this,&存在服务_,&特征概念服务_,std::move(需求运行),reader,std::move(字段询问运行),词条读取,std::move(词典运行)));
            结果.状态 = 新方法操作状态::已建立;
            return 结果;
        }
        std::set<稳定编码> 已有角色;
        for (const auto& 绑定 : 角色绑定组) {
            if (绑定.有序引用 || !节点仍存在(绑定.参数角色节点)
                || !已有角色.insert(绑定.参数角色节点).second) {
                结果.状态 = 新方法操作状态::参数角色重复;
                return 结果;
            }
            if (绑定.服务要求分项引用 || 绑定.服务要求确认引用) {
                if (!服务要求读取 || (绑定.服务要求分项引用 && 绑定.服务要求确认引用)
                    || 绑定.任务虚拟存在引用 || 绑定.世界场景节点
                    || 绑定.实际存在节点 || !绑定.实际特征组.empty()) {
                    结果.状态 = 新方法操作状态::参数引用失效;
                    return 结果;
                }
                if (绑定.服务要求分项引用) {
                    const auto& 引用 = *绑定.服务要求分项引用;
                    if (!有效(引用.理解节点)) {
                        结果.状态 = 新方法操作状态::参数引用失效;
                        return 结果;
                    }
                    const auto 读回 = 服务要求读取->读取服务要求分项(引用);
                    if (读回.状态 != 新服务要求材料读取状态::已读取) {
                        结果.状态 = 读回.状态 == 新服务要求材料读取状态::材料不存在
                            ? 新方法操作状态::参数引用失效 : 新方法操作状态::结构不一致;
                        return 结果;
                    }
                    if (!读回.材料 || 读回.材料->引用 != 引用) {
                        结果.状态 = 新方法操作状态::结构不一致;
                        return 结果;
                    }
                    switch (引用.分项) {
                    case 新服务要求分项种类::来源人类:
                    case 新服务要求分项种类::目标存在:
                    case 新服务要求分项种类::目标特征类型: {
                        const auto* 编码 = std::get_if<稳定编码>(&读回.材料->值);
                        if (!编码 || !有效(*编码)) {
                            结果.状态 = 新方法操作状态::结构不一致;
                            return 结果;
                        }
                        break;
                    }
                    case 新服务要求分项种类::目标定义:
                        if (!std::holds_alternative<新服务要求目标分项>(读回.材料->值)) {
                            结果.状态 = 新方法操作状态::结构不一致;
                            return 结果;
                        }
                        break;
                    default:
                        结果.状态 = 新方法操作状态::参数概念不相容;
                        return 结果;
                    }
                } else {
                    const auto 编码 = 绑定.服务要求确认引用->确认节点;
                    if (!有效(编码)) {
                        结果.状态 = 新方法操作状态::参数引用失效;
                        return 结果;
                    }
                    const auto 读回 = 服务要求读取->读取服务要求确认身份(编码);
                    if (读回.状态 != 新服务要求确认状态::已读取) {
                        结果.状态 = 读回.状态 == 新服务要求确认状态::资源失败
                            ? 新方法操作状态::资源失败
                            : 读回.状态 == 新服务要求确认状态::确认不存在
                                ? 新方法操作状态::参数引用失效 : 新方法操作状态::结构不一致;
                        return 结果;
                    }
                    if (!读回.确认 || 读回.确认->确认节点 != 编码) {
                        结果.状态 = 新方法操作状态::结构不一致;
                        return 结果;
                    }
                }
                continue;
            }
            if (绑定.任务虚拟存在引用) {
                const auto& 引用 = *绑定.任务虚拟存在引用;
                if (!治理对象读取 || 绑定.世界场景节点 || 绑定.实际存在节点
                    || !有效(引用.任务节点) || !有效(引用.虚拟存在节点)
                    || !有效(引用.状态特征节点)
                    || 绑定.实际特征组.empty() || 绑定.实际特征组.size() > 2
                    || std::count(绑定.实际特征组.begin(), 绑定.实际特征组.end(),
                        新方法实际特征绑定{引用.状态特征节点}) != 1) {
                    结果.状态 = 新方法操作状态::参数引用失效;
                    return 结果;
                }
                const auto 读回 = 治理对象读取->读取任务虚拟存在引用(引用);
                if (!读回 || 读回->引用 != 引用
                    || !特征概念服务_.是特征概念节点(
                        读回->状态特征类型概念节点)) {
                    结果.状态 = 新方法操作状态::参数引用失效;
                    return 结果;
                }
                if (绑定.实际特征组.size() == 2) {
                    const auto 组 = 治理对象读取->读取任务虚拟存在特征组(引用);
                    if (!组 || 组->引用 != 引用 || 组->状态.特征节点 != 引用.状态特征节点
                        || !组->服务值 || 组->服务值->特征节点 == 引用.状态特征节点
                        || std::count(绑定.实际特征组.begin(), 绑定.实际特征组.end(),
                            新方法实际特征绑定{组->服务值->特征节点}) != 1) {
                        结果.状态 = 新方法操作状态::参数引用失效; return 结果;
                    }
                }
            }
            if (绑定.世界场景节点
                && !场景服务_.是场景节点(*绑定.世界场景节点)) {
                结果.状态 = 新方法操作状态::参数引用失效;
                return 结果;
            }
            if (绑定.实际存在节点
                && !存在服务_.是存在节点(*绑定.实际存在节点)
                && !场景服务_.是场景节点(*绑定.实际存在节点)) {
                结果.状态 = 新方法操作状态::参数引用失效;
                return 结果;
            }
            if (绑定.世界场景节点 && 绑定.实际存在节点
                && *绑定.世界场景节点 != *绑定.实际存在节点
                && !场景服务_.存在属于场景范围(
                    *绑定.世界场景节点, *绑定.实际存在节点)) {
                结果.状态 = 新方法操作状态::参数引用失效;
                return 结果;
            }
            std::set<稳定编码> 已有特征;
            for (const auto& 特征 : 绑定.实际特征组) {
                if (!特征服务_.是特征节点(特征.特征节点)
                    || !已有特征.insert(特征.特征节点).second) {
                    结果.状态 = 新方法操作状态::参数引用失效;
                    return 结果;
                }
                if (绑定.实际存在节点) {
                    const auto 存在 = 存在服务_.获取存在(*绑定.实际存在节点);
                    const auto 场景 = 存在 ? std::optional<新场景信息>{}
                        : 场景服务_.获取场景(*绑定.实际存在节点);
                    const auto* 持有组 = 存在 ? &存在->特征节点组 : 场景 ? &场景->特征节点组 : nullptr;
                    if (!持有组 || std::find(持有组->begin(), 持有组->end(), 特征.特征节点) == 持有组->end()) {
                        结果.状态 = 新方法操作状态::参数引用失效;
                        return 结果;
                    }
                }
            }
        }
        if (角色绑定组.size() != 方法->参数规格组.size()) {
            结果.状态 = 新方法操作状态::参数角色缺失;
            return 结果;
        }
        for (const auto& 规格 : 方法->参数规格组) {
            const auto 绑定位置 = std::find_if(角色绑定组.begin(),
                角色绑定组.end(), [&](const 新方法参数角色绑定& 绑定) {
                    return 绑定.参数角色节点 == 规格.参数角色节点;
                });
            if (绑定位置 == 角色绑定组.end()) {
                结果.状态 = 新方法操作状态::参数角色缺失;
                return 结果;
            }
            const auto& 绑定 = *绑定位置;
            if (绑定.任务虚拟存在引用 && 绑定.实际特征组.size() != 规格.特征规格组.size()) {
                结果.状态 = 新方法操作状态::参数规格不合法; return 结果;
            }
            if (是服务要求参数对象(规格.对象种类)) {
                const bool 匹配 = 规格.对象种类 == 新方法参数对象种类::服务要求确认
                    ? 绑定.服务要求确认引用.has_value()
                    : 绑定.服务要求分项引用
                        && 服务要求分项匹配(规格.对象种类, 绑定.服务要求分项引用->分项);
                if (!匹配) {
                    结果.状态 = 新方法操作状态::参数概念不相容;
                    return 结果;
                }
                continue;
            }
            if (绑定.服务要求分项引用 || 绑定.服务要求确认引用) {
                结果.状态 = 新方法操作状态::参数概念不相容;
                return 结果;
            }
            if ((规格.对象种类 == 新方法参数对象种类::任务虚拟存在)
                    != 绑定.任务虚拟存在引用.has_value()) {
                结果.状态 = 新方法操作状态::参数概念不相容;
                return 结果;
            }
            if (规格.要求场景 && !绑定.世界场景节点) {
                结果.状态 = 新方法操作状态::参数角色缺失;
                return 结果;
            }
            if (规格.存在概念节点) {
                const auto 实际节点 = 绑定.实际存在节点
                    ? 绑定.实际存在节点 : 绑定.世界场景节点;
                if (!实际节点) {
                    结果.状态 = 新方法操作状态::参数角色缺失;
                    return 结果;
                }
                const auto 定义 = 存在概念服务_.获取存在概念(*规格.存在概念节点);
                if (!定义) throw std::logic_error("参数存在概念定义不可读");
                if (定义->种类 == 存在概念种类::专属 && !定义->对应存在节点)
                    throw std::logic_error("参数专属概念缺对应实例");
                const auto 匹配 = 定义->种类 == 存在概念种类::专属
                    ? (*定义->对应存在节点 == *实际节点 ? 新存在概念匹配状态::匹配 : 新存在概念匹配状态::不符合)
                    : 存在服务_.判定存在概念匹配状态(*实际节点, *规格.存在概念节点);
                if (匹配 != 新存在概念匹配状态::匹配) {
                    结果.状态 = 匹配 == 新存在概念匹配状态::材料不足
                        ? 新方法操作状态::参数材料未到 : 新方法操作状态::参数概念不相容;
                    return 结果;
                }
            }
            for (const auto& 特征规格 : 规格.特征规格组) {
                std::size_t 命中数量 = 0;
                for (const auto& 特征绑定 : 绑定.实际特征组) {
                    const auto 特征 = 特征服务_.获取特征(特征绑定.特征节点);
                    if (!特征) {
                        结果.状态 = 新方法操作状态::参数引用失效;
                        return 结果;
                    }
                    const bool 类型命中 = 特征概念为或从属于(
                        特征->特征概念节点,
                        特征规格.特征类型概念节点,
                        特征概念服务_);
                    bool 值域命中 = !特征规格.值域概念节点
                        || 特征概念为或从属于(
                            特征->特征概念节点,
                            *特征规格.值域概念节点,
                            特征概念服务_);
                    if (类型命中 && 特征规格.值域概念节点) {
                        const auto 域 = 特征概念服务_.获取特征概念(*特征规格.值域概念节点);
                        if (!域) { 结果.状态 = 新方法操作状态::结构不一致; return 结果; }
                        if (域->材料类型 == 特征概念材料物理类型::I64标量
                            || std::holds_alternative<特征概念任务集合值域>(域->值域)) {
                            if (!特征->当前值) { 结果.状态 = 新方法操作状态::参数概念不相容; return 结果; }
                            const auto 比较 = 特征服务_.比较特征值域(
                                {特征->特征概念节点, 特征->实例单位, 新特征准确值比较材料{*特征->当前值}},
                                {*特征规格.值域概念节点, 域->单位,
                                    新特征概念值域比较材料{*特征规格.值域概念节点}});
                            if (比较.状态 != 新特征比较状态::已完成 || !比较.关系编码) {
                                结果.状态 = 比较.状态 == 新特征比较状态::资源失败
                                    ? 新方法操作状态::资源失败 : 新方法操作状态::结构不一致;
                                return 结果;
                            }
                            值域命中 = *比较.关系编码 == static_cast<std::int64_t>(特征概念值域关系::相等)
                                || *比较.关系编码 == static_cast<std::int64_t>(特征概念值域关系::右包含左);
                        }
                    }
                    if (类型命中 && 值域命中) ++命中数量;
                }
                if (命中数量 != 1) {
                    结果.状态 = 新方法操作状态::参数概念不相容;
                    return 结果;
                }
            }
        }
        // 所有角色已绑定以后再检查关系，不能依赖规格遍历顺序。
        for (const auto& 规格 : 方法->参数规格组) {
            if (!规格.集合成员约束) continue;
            const auto& 约束 = *规格.集合成员约束;
            const auto 来源 = std::find_if(角色绑定组.begin(), 角色绑定组.end(),
                [&](const auto& b) { return b.参数角色节点 == 规格.参数角色节点; });
            const auto 集合角色 = std::find_if(角色绑定组.begin(), 角色绑定组.end(),
                [&](const auto& b) { return b.参数角色节点 == 约束.集合参数角色节点; });
            if (来源 == 角色绑定组.end() || !来源->任务虚拟存在引用 || 集合角色 == 角色绑定组.end()) {
                结果.状态 = 新方法操作状态::参数角色缺失; return 结果;
            }
            std::optional<新特征信息> 集合;
            for (const auto& b : 集合角色->实际特征组) {
                const auto f = 特征服务_.获取特征(b.特征节点);
                if (!f) { 结果.状态 = 新方法操作状态::参数引用失效; return 结果; }
                if (特征概念为或从属于(f->特征概念节点, 约束.集合特征类型概念节点, 特征概念服务_)) {
                    if (集合) { 结果.状态 = 新方法操作状态::结构不一致; return 结果; }
                    集合 = f;
                }
            }
            if (!集合 || !集合->当前值) { 结果.状态 = 新方法操作状态::参数概念不相容; return 结果; }
            特征概念任务集合值域 目标;
            目标.必含任务节点组 = {来源->任务虚拟存在引用->任务节点};
            const auto 比较 = 特征服务_.比较任务集合条件(约束.集合特征类型概念节点, *集合->当前值, 目标);
            if (比较.状态 != 特征概念规则状态::已完成 || !比较.比较值) {
                结果.状态 = 比较.状态 == 特征概念规则状态::资源失败
                    ? 新方法操作状态::资源失败 : 新方法操作状态::结构不一致;
                return 结果;
            }
            if (*比较.比较值 != 1) { 结果.状态 = 新方法操作状态::参数概念不相容; return 结果; }
        }
        std::sort(角色绑定组.begin(), 角色绑定组.end(),
            [](const 新方法参数角色绑定& 左,
               const 新方法参数角色绑定& 右) {
                return 左.参数角色节点 < 右.参数角色节点;
            });
        结果.参数场景 = std::shared_ptr<const 方法参数场景>(
            new 方法参数场景(方法节点, std::move(角色绑定组),
                特征服务_, 特征值服务_, 新方法参数场景用途::执行,
                治理对象读取, {}, 服务要求读取, {}, 任务成员绑定,执行分支,this,&存在服务_,&特征概念服务_,std::move(需求运行),nullptr,{},nullptr,std::move(词典运行)));
        结果.状态 = 新方法操作状态::已建立;
        return 结果;
    } catch (const std::bad_alloc&) {
        结果.状态 = 新方法操作状态::资源失败;
        结果.参数场景.reset();
        return 结果;
    } catch (const std::length_error&) {
        结果.状态 = 新方法操作状态::资源失败;
        结果.参数场景.reset();
        return 结果;
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"建立参数场景", L"参数结构或概念核验发生工程异常。", 异常.what());
    } catch (const 引用内容定义前提变化&) {
        // 零发布的参数准备丢弃后重读；没有动作或写入可被重放。
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"建立参数场景", L"参数结构或概念核验发生未知工程异常。");
    }
    std::this_thread::yield();
    }
}

新方法参数场景结果 新_方法类::建立模拟参数场景(
    稳定编码 方法节点,
    std::vector<新方法参数角色绑定> 绑定,
    std::shared_ptr<新方法模拟运行接口> 模拟,
    std::optional<新方法任务成员绑定> 任务成员绑定,std::optional<稳定编码> 执行分支) const noexcept {
    新方法参数场景结果 结果;
    try {
        std::optional<新方法信息> 方法;
        {
            std::lock_guard 锁(新方法互斥);
            方法 = 读取方法信息(方法节点, 特征概念服务_, *this, 存在概念服务_);
        }
        if (!方法 || 方法->来源方法节点 || 方法->实例根节点
            || 方法->实现方式 != 新方法实现方式::本能
            || 方法->启用状态 != 新方法启用状态::启用
            || !方法->本能函数登记名 || 方法->本能函数登记名->empty()) {
            结果.状态 = 新方法操作状态::方法不存在;
            return 结果;
        }

        if(std::any_of(方法->参数规格组.begin(),方法->参数规格组.end(),[](const auto& 项) {
            return 项.有序规格&&项.有序规格->信息类型==新方法参数信息类型::词条;
        })) {
            结果.状态=新方法操作状态::能力未支持;
            return 结果;
        }

        if(!方法->执行分支组.empty()||执行分支) {
            const auto 分支读取=读取执行分支必要条件(方法节点,执行分支);
            if(分支读取.状态!=新方法执行分支读取状态::已读取) {
                结果.状态=分支读取.状态==新方法执行分支读取状态::前提已变化
                    ? 新方法操作状态::定义前提已变化
                    : 分支读取.状态==新方法执行分支读取状态::方法不存在
                        ? 新方法操作状态::方法不存在 : 新方法操作状态::参数概念不相容;
                return 结果;
            }
        }

        if(方法->参数规格组.empty()||方法->参数规格组.front().有序规格) {
            if(!模拟||!模拟->核验模拟参数(方法节点,绑定)||绑定.size()!=方法->参数规格组.size()) {
                结果.状态=新方法操作状态::参数引用失效;return 结果;
            }
            for(std::size_t i=0;i<绑定.size();++i) {
                const auto& spec=*方法->参数规格组[i].有序规格;
                const auto& b=绑定[i];
                if(!b.有序引用||b.有序引用->序号!=spec.序号||b.有序引用->信息类型!=spec.信息类型) {
                    结果.状态=新方法操作状态::参数概念不相容;return 结果;
                }
                if(spec.信息类型==新方法参数信息类型::特征) {
                    const auto f=模拟->读取模拟特征(b.有序引用->信息节点);
                    const auto 定义 = f ? 特征概念服务_.获取特征概念(f->特征概念节点) : std::nullopt;
                    const bool 合法未知需求引用 = 定义
                        && 定义->材料类型==特征概念材料物理类型::需求节点引用
                        && 定义->比较规则==特征概念比较规则::需求身份精确;
                    if(!f||(!f->当前值&&!合法未知需求引用)||!特征概念为或从属于(f->特征概念节点,spec.范围概念节点,特征概念服务_)) {
                        结果.状态=新方法操作状态::参数概念不相容;return 结果;
                    }
                } else {
                    const auto ordinary=存在概念服务_.读取普通存在概念定义保护(spec.范围概念节点);
                    if(ordinary.状态==正式概念读取状态::前提已变化)throw 引用内容定义前提变化{};
                    if(ordinary.状态==正式概念读取状态::非本提供者概念) {
                        if(!方法存在范围有效(spec.范围概念节点,存在概念服务_)) {
                            结果.状态=新方法操作状态::参数规格不合法;return 结果;
                        }
                        const auto read=模拟->读取正式信息参数保护(b.有序引用->信息节点,spec.范围概念节点);
                        const auto state=正式参数状态(read,b.有序引用->信息节点);
                        if(state!=新方法操作状态::已找到){结果.状态=state;return 结果;}
                    } else if(ordinary.状态!=正式概念读取状态::已读取||!ordinary.信息) {
                        throw std::runtime_error("simulation ordinary range unavailable");
                    } else if(!模拟->读取模拟存在特征组(b.有序引用->信息节点)) {
                        结果.状态=新方法操作状态::参数引用失效;return 结果;
                    }
                }
            }
            结果.参数场景=std::shared_ptr<const 方法参数场景>(new 方法参数场景(
                方法节点,std::move(绑定),特征服务_,特征值服务_,新方法参数场景用途::模拟,nullptr,std::move(模拟),nullptr,{},任务成员绑定,执行分支,this,&存在服务_,&特征概念服务_));
            结果.状态=新方法操作状态::已建立;return 结果;
        }

        // 本切片没有服务要求材料的模拟owner，必须在调用任何模拟/现实端口前拒绝。
        if (std::any_of(方法->参数规格组.begin(), 方法->参数规格组.end(),
                [](const auto& 规格) { return 规格.有序规格 || 是服务要求参数对象(规格.对象种类); })
            || std::any_of(绑定.begin(), 绑定.end(), [](const auto& 角色) {
                return 角色.有序引用 || 角色.服务要求分项引用 || 角色.服务要求确认引用;
            })) {
            结果.状态 = 新方法操作状态::参数概念不相容;
            return 结果;
        }
        // 外部模拟owner核验在方法锁之外；这里绝不调用现实场景/存在owner。
        if (!模拟 || !节点仍存在(模拟->模拟根节点())
            || !模拟->核验模拟参数(方法节点, 绑定)) {
            结果.状态 = 新方法操作状态::参数引用失效;
            return 结果;
        }
        if (!方法头有效(方法->方法头) || !参数规格有效(方法->参数规格组, 特征概念服务_)
            || 绑定.size() != 方法->参数规格组.size()) {
            结果.状态 = 新方法操作状态::参数规格不合法;
            return 结果;
        }
        std::set<稳定编码> 已有角色, 已有特征;
        for (const auto& 角色 : 绑定) {
            if (!节点仍存在(角色.参数角色节点)
                || !已有角色.insert(角色.参数角色节点).second) {
                结果.状态 = 新方法操作状态::参数角色重复;
                return 结果;
            }
            const auto 规格 = std::find_if(方法->参数规格组.begin(), 方法->参数规格组.end(),
                [&](const auto& 候选) { return 候选.参数角色节点 == 角色.参数角色节点; });
            if (规格 == 方法->参数规格组.end() || 规格->存在概念节点
                || 规格->特征规格组.size() != 角色.实际特征组.size()
                || (规格->要求场景 && !角色.世界场景节点)
                || (规格->对象种类 == 新方法参数对象种类::任务虚拟存在)
                    != 角色.任务虚拟存在引用.has_value()) {
                结果.状态 = 新方法操作状态::参数概念不相容;
                return 结果;
            }
            std::optional<新任务虚拟存在读回> 治理读回;
            if (角色.任务虚拟存在引用) {
                治理读回 = 模拟->读取任务虚拟存在引用(*角色.任务虚拟存在引用);
                if (!治理读回 || 治理读回->引用 != *角色.任务虚拟存在引用
                    || 角色.世界场景节点 || 角色.实际存在节点 || 角色.实际特征组.size() != 1
                    || 角色.实际特征组.front().特征节点 != 治理读回->引用.状态特征节点) {
                    结果.状态 = 新方法操作状态::参数引用失效;
                    return 结果;
                }
            }
            std::vector<新特征信息> 特征组;
            for (const auto& 项 : 角色.实际特征组) {
                const auto 特征 = 模拟->读取模拟特征(项.特征节点);
                if (!已有特征.insert(项.特征节点).second || !特征
                    || 特征->节点 != 项.特征节点 || !特征->当前值) {
                    结果.状态 = 新方法操作状态::参数引用失效;
                    return 结果;
                }
                if (治理读回 && (*特征->当前值 != 新特征准确值{治理读回->当前状态}
                    || !特征概念为或从属于(特征->特征概念节点,
                        治理读回->状态特征类型概念节点, 特征概念服务_))) {
                    结果.状态 = 新方法操作状态::参数概念不相容;
                    return 结果;
                }
                特征组.push_back(*特征);
            }
            for (const auto& 特征规格 : 规格->特征规格组) {
                if (!特征规格.值域概念节点 || !特征规格.二次特征概念组.empty()) {
                    结果.状态 = 新方法操作状态::参数概念不相容;
                    return 结果;
                }
                const auto 域概念 = 特征概念服务_.获取特征概念(*特征规格.值域概念节点);
                const auto* 集合域 = 域概念 ? std::get_if<特征概念任务集合值域>(&域概念->值域) : nullptr;
                if (集合域) {
                    const auto 先天 = 特征概念服务_.获取先天特征概念();
                    if (!先天 || 特征规格.特征类型概念节点 != 先天->服务任务集合
                        || 域概念->材料类型 != 特征概念材料物理类型::U64数组
                        || 域概念->比较规则 != 特征概念比较规则::服务任务集合身份判等
                        || 域概念->单位 || !集合域->至少一个任务
                        || !集合域->必含任务节点组.empty() || !集合域->必含需求任务组.empty()
                        || !集合域->必含服务要求任务组.empty()
                        || !特征概念为或从属于(域概念->节点, 先天->服务任务集合, 特征概念服务_)) {
                        结果.状态 = 新方法操作状态::参数概念不相容; return 结果;
                    }
                    std::size_t 命中 = 0;
                    for (const auto& f : 特征组) {
                        if (!特征概念为或从属于(f.特征概念节点, 先天->服务任务集合, 特征概念服务_)) continue;
                        const auto* 引用 = std::get_if<稳定编码>(&*f.当前值);
                        const auto 值 = 引用 ? 模拟->读取模拟特征值材料(*引用) : std::nullopt;
                        const auto* 成员 = 值 ? std::get_if<std::vector<std::uint64_t>>(&值->材料) : nullptr;
                        if (!值 || !引用 || 值->节点 != *引用 || 值->物理类型 != 材料物理类型::U64数组
                            || !成员 || f.实例单位) { 结果.状态 = 新方法操作状态::参数引用失效; return 结果; }
                        if (!成员->empty()) ++命中;
                    }
                    if (命中 != 1) { 结果.状态 = 新方法操作状态::参数概念不相容; return 结果; }
                    continue;
                }
                const auto* 区间 = 域概念
                    ? std::get_if<std::vector<特征概念I64闭区间>>(&域概念->值域) : nullptr;
                if (!域概念 || 域概念->材料类型 != 特征概念材料物理类型::I64标量
                    || !区间 || 区间->size() != 1 || 区间->front().下界 != 区间->front().上界
                    || 区间->front().下界 == (std::numeric_limits<std::int64_t>::min)()
                    || 区间->front().上界 == (std::numeric_limits<std::int64_t>::max)()
                    || !特征概念为或从属于(*特征规格.值域概念节点,
                        特征规格.特征类型概念节点, 特征概念服务_)) {
                    结果.状态 = 新方法操作状态::参数概念不相容;
                    return 结果;
                }
                const auto 命中 = std::count_if(特征组.begin(), 特征组.end(), [&](const auto& 特征) {
                    return 特征概念为或从属于(特征.特征概念节点,
                        特征规格.特征类型概念节点, 特征概念服务_)
                        && std::holds_alternative<std::int64_t>(*特征.当前值)
                        && std::get<std::int64_t>(*特征.当前值) == 区间->front().下界;
                });
                if (命中 != 1) {
                    结果.状态 = 新方法操作状态::参数概念不相容;
                    return 结果;
                }
            }
        }
        for (const auto& 规格 : 方法->参数规格组) {
            if (!规格.集合成员约束) continue;
            const auto& 约束 = *规格.集合成员约束;
            const auto 来源 = std::find_if(绑定.begin(), 绑定.end(),
                [&](const auto& b) { return b.参数角色节点 == 规格.参数角色节点; });
            const auto 集合角色 = std::find_if(绑定.begin(), 绑定.end(),
                [&](const auto& b) { return b.参数角色节点 == 约束.集合参数角色节点; });
            if (来源 == 绑定.end() || !来源->任务虚拟存在引用 || 集合角色 == 绑定.end()) {
                结果.状态 = 新方法操作状态::参数角色缺失; return 结果;
            }
            std::size_t 命中 = 0;
            for (const auto& b : 集合角色->实际特征组) {
                const auto f = 模拟->读取模拟特征(b.特征节点);
                if (!f || !f->当前值) { 结果.状态 = 新方法操作状态::参数引用失效; return 结果; }
                if (!特征概念为或从属于(f->特征概念节点, 约束.集合特征类型概念节点, 特征概念服务_)) continue;
                const auto* 引用 = std::get_if<稳定编码>(&*f->当前值);
                const auto 值 = 引用 ? 模拟->读取模拟特征值材料(*引用) : std::nullopt;
                const auto* 成员 = 值 ? std::get_if<std::vector<std::uint64_t>>(&值->材料) : nullptr;
                if (!成员 || 值->物理类型 != 材料物理类型::U64数组) {
                    结果.状态 = 新方法操作状态::参数引用失效; return 结果;
                }
                if (std::find(成员->begin(), 成员->end(), 来源->任务虚拟存在引用->任务节点.值) != 成员->end()) ++命中;
            }
            if (命中 != 1) { 结果.状态 = 新方法操作状态::参数概念不相容; return 结果; }
        }
        结果.参数场景 = std::shared_ptr<const 方法参数场景>(new 方法参数场景(
            方法节点, std::move(绑定), 特征服务_, 特征值服务_,
            新方法参数场景用途::模拟, nullptr, std::move(模拟),nullptr,{},任务成员绑定,执行分支,this,&存在服务_,&特征概念服务_));
        结果.状态 = 新方法操作状态::已建立;
        return 结果;
    } catch (const 引用内容定义前提变化&) {
        结果.状态 = 新方法操作状态::定义前提已变化;
        结果.参数场景.reset();
        return 结果;
    } catch (...) {
        结果.状态 = 新方法操作状态::资源失败;
        结果.参数场景.reset();
        return 结果;
    }
}

新方法参数场景结果 新_方法类::建立本能登记参数场景(
    std::vector<新方法参数角色绑定> 角色绑定组) const noexcept {
    新方法参数场景结果 结果;
    try {
        {
            std::lock_guard 锁(新方法互斥);
            if (!结构仍存在()) {
                结果.状态 = 新方法操作状态::尚未初始化;
                return 结果;
            }
        }
        // 空登记只携带用途；本能从登记端口取得先天规格，不造运行实参。
        if (角色绑定组.empty()) {
            结果.参数场景 = std::shared_ptr<const 方法参数场景>(new 方法参数场景(
                {}, {}, 特征服务_, 特征值服务_, 新方法参数场景用途::登记, nullptr));
            结果.状态 = 新方法操作状态::已建立;
            return 结果;
        }
        // 非空登记只接收已存在的现实目标与一个特征，不接收来源任务占位。
        // 自我/服务FT的业务身份由登记本能对照自身正式材料再核验。
        if (角色绑定组.size() != 1) {
            结果.状态 = 新方法操作状态::参数角色缺失;
            return 结果;
        }
        const auto& 绑定 = 角色绑定组.front();
        if (绑定.有序引用 || !节点仍存在(绑定.参数角色节点) || 绑定.任务虚拟存在引用
            || 绑定.服务要求分项引用 || 绑定.服务要求确认引用
            || !绑定.世界场景节点 || !绑定.实际存在节点
            || !场景服务_.是场景节点(*绑定.世界场景节点)
            || !存在服务_.是存在节点(*绑定.实际存在节点)
            || !场景服务_.存在属于场景范围(
                *绑定.世界场景节点, *绑定.实际存在节点)
            || 绑定.实际特征组.size() != 1) {
            结果.状态 = 新方法操作状态::参数引用失效;
            return 结果;
        }
        const auto 存在 = 存在服务_.获取存在(*绑定.实际存在节点);
        const auto 特征节点 = 绑定.实际特征组.front().特征节点;
        if (!存在 || !特征服务_.是特征节点(特征节点)
            || std::find(存在->特征节点组.begin(), 存在->特征节点组.end(),
                特征节点) == 存在->特征节点组.end()) {
            结果.状态 = 新方法操作状态::参数引用失效;
            return 结果;
        }
        结果.参数场景 = std::shared_ptr<const 方法参数场景>(
            new 方法参数场景({}, std::move(角色绑定组),
                特征服务_, 特征值服务_, 新方法参数场景用途::登记, nullptr,{},nullptr,{},{},{}));
        结果.状态 = 新方法操作状态::已建立;
    } catch (...) {
        结果.状态 = 新方法操作状态::资源失败;
        结果.参数场景.reset();
    }
    return 结果;
}

新方法参数场景结果 新_方法类::建立服务需求登记参数场景() const noexcept {
    新方法参数场景结果 结果;
    try {
        {
            std::lock_guard 锁(新方法互斥);
            if (!结构仍存在()) {
                结果.状态 = 新方法操作状态::尚未初始化;
                return 结果;
            }
        }
        结果.参数场景 = std::shared_ptr<const 方法参数场景>(new 方法参数场景(
            {}, {}, 特征服务_, 特征值服务_, 新方法参数场景用途::登记, nullptr,{},nullptr,{},{},{}));
        结果.状态 = 新方法操作状态::已建立;
    } catch (...) {
        结果.状态 = 新方法操作状态::资源失败;
        结果.参数场景.reset();
    }
    return 结果;
}

新方法本能调用结果 新_方法类::调用本能函数(
    稳定编码 方法节点,
    const 方法参数场景& 参数场景) const noexcept {
    新方法本能调用结果 结果;
    bool 已进入函数 = false;
    if (!有效(方法节点) || 参数场景.方法节点() != 方法节点
        || (参数场景.用途() != 新方法参数场景用途::执行
            && 参数场景.用途() != 新方法参数场景用途::模拟
            && 参数场景.用途() != 新方法参数场景用途::任务贡献治理)) {
        结果.状态 = 新方法本能调用状态::参数场景不匹配;
        return 结果;
    }
    try {
        std::optional<新方法信息> 方法;
        {
            // 外部函数调用期间不得持有方法结构互斥。
            std::lock_guard 锁(新方法互斥);
            方法 = 读取方法信息(方法节点, 特征概念服务_, *this, 存在概念服务_);
        }
        if (!方法) {
            结果.状态 = 新方法本能调用状态::方法不存在;
            return 结果;
        }
        if (方法->启用状态 != 新方法启用状态::启用) {
            结果.状态 = 新方法本能调用状态::方法停用;
            return 结果;
        }
        if (方法->实现方式 != 新方法实现方式::本能) {
            结果.状态 = 新方法本能调用状态::不是本能方法;
            return 结果;
        }
        if (!方法->本能函数登记名 || 方法->本能函数登记名->empty()) {
            结果.状态 = 新方法本能调用状态::函数未登记;
            return 结果;
        }
        if (参数场景.用途() == 新方法参数场景用途::任务贡献治理
            || 参数场景.读取任务贡献治理参数()) {
            结果.状态 = 新方法本能调用状态::参数场景不匹配; return 结果;
        }
        if (方法->本能函数登记名 == 新本能函数登记名_旧两参数获取服务对象) {
            std::lock_guard 锁(新方法互斥);
            const auto 当前 = 查询本能登记结构(std::string(新本能函数登记名_旧两参数获取服务对象), 特征概念服务_, *this, 存在概念服务_);
            if (当前.状态 != 新方法操作状态::已找到
                || 当前.方法节点 != 方法->来源方法节点.value_or(方法->节点)
                || !是有序服务对象规格(方法->参数规格组)) {
                结果.状态 = 新方法本能调用状态::方法停用; return 结果;
            }
        }
        const auto 函数 = 新_本能函数管理类::解析(*方法->本能函数登记名);
        if (!函数.函数) {
            结果.状态 = 函数.状态 == 新本能函数解析状态::内部错误
                ? 新方法本能调用状态::内部错误
                : 新方法本能调用状态::函数未登记;
            return 结果;
        }
        if(!方法->执行分支组.empty()||参数场景.执行分支节点()) {
            const auto 分支读取=读取执行分支必要条件(方法节点,参数场景.执行分支节点());
            if(分支读取.状态!=新方法执行分支读取状态::已读取) {
                if(分支读取.状态==新方法执行分支读取状态::前提已变化)
                    记录方法前提变化诊断(L"本能调用",L"阶段=执行分支读取；方法="+std::to_wstring(方法节点.值));
                结果.状态=分支读取.状态==新方法执行分支读取状态::前提已变化
                    ? 新方法本能调用状态::定义前提已变化
                    : 分支读取.状态==新方法执行分支读取状态::方法不存在
                        ? 新方法本能调用状态::方法不存在 : 新方法本能调用状态::参数场景不匹配;
                return 结果;
            }
        }
        已进入函数 = true;
        结果.本次条件结果对 = 函数.函数(参数场景);
        const auto returned=结果.本次条件结果对->条件.执行分支节点;
        if((returned&&returned!=参数场景.执行分支节点())
            ||(结果.本次条件结果对->结果.状态==新本能函数执行状态::已按合同返回&&returned!=参数场景.执行分支节点()))
            工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"核验本能执行分支",L"函数返回分支与本次参数场景选择不同。");
        结果.状态 = 新方法本能调用状态::已调用;
        return 结果;
    } catch (const std::bad_alloc&) {
        结果.状态 = 新方法本能调用状态::资源失败;
    } catch (const 引用内容定义前提变化&) {
        记录方法前提变化诊断(L"本能调用", std::wstring(L"阶段=") + (已进入函数 ? L"本能函数内" : L"方法读取")
            + L"；方法=" + std::to_wstring(方法节点.值));
        结果.状态 = 新方法本能调用状态::定义前提已变化;
    } catch (...) {
        结果.状态 = 新方法本能调用状态::内部错误;
    }
    结果.本次条件结果对.reset();
    return 结果;
}

std::optional<新方法直接调用身份> 新_方法类::读取直接调用身份(
    稳定编码 调用) const {
    for (;;) {
    try {
    std::unique_lock 锁(新方法互斥);
    if (!有效(调用) || !有效(直接调用根)) return std::nullopt;
    确保直接调用结构();
    const auto 成员 = 读取多节点字段(直接调用根, 直接调用成员字段, 特征概念服务_);
    if (!成员) throw std::runtime_error("direct invocation membership damaged");
    if (std::find(成员->begin(), 成员->end(), 调用) == 成员->end()) return std::nullopt;
    if (!节点类型为(调用, 直接调用类型))
        throw std::runtime_error("direct invocation type damaged");
    const auto 方法 = 读取唯一节点字段(调用, 直接调用方法字段);
    const auto 现实 = 读取唯一节点字段(调用, 直接调用现实场景字段);
    const auto 外设 = 读取唯一节点字段(调用, 直接调用外设场景字段);
    const auto 收束 = 读取唯一I64字段(调用, 直接调用收束字段);
    const auto 动作 = 读取可选节点字段(调用, 直接调用动作字段);
    const auto 参数节点组 = 读取多节点字段(调用, 直接调用参数字段, 特征概念服务_);
    const auto 方法信息 = 方法 ? 获取方法(*方法) : std::nullopt;
    if (!方法 || !现实 || !外设 || !收束 || !动作 || !参数节点组
        || !方法信息 || !参数规格有效(方法信息->参数规格组, 特征概念服务_)
        || (*收束 != 0 && *收束 != 1)
        || 参数节点组->size() != 方法信息->参数规格组.size())
        throw std::runtime_error("direct invocation content damaged");
    新方法直接调用身份 结果{调用, *方法, *现实, *外设, {}, {}, *收束 == 1};
    std::vector<基础联合节点快照> 类别保护;
    if (有效(*动作)) 结果.动作节点 = *动作;
    for (const auto 参数 : *参数节点组) {
        const auto 序号 = 读取唯一I64字段(参数, 直接调用序号字段);
        const auto 类型 = 读取持久参数类别(参数, 直接调用信息类型字段, 直接调用类别概念字段,
            存在概念服务_, 特征概念服务_);
        const auto 引用 = 读取唯一节点字段(参数, 直接调用信息引用字段);
        if (!节点类型为(参数, 直接调用参数类型) || !序号 || !类型 || !引用
            || *序号 < 1 || static_cast<std::uint64_t>(*序号) > 参数节点组->size())
            throw std::runtime_error("direct invocation parameter damaged");
        if (const auto c = 读取唯一节点字段(参数, 直接调用类别概念字段)) {
            const auto p = 解析参数类别概念保护(*c, 存在概念服务_, 特征概念服务_);
            if (p.分派类型 != *类型) throw 引用内容定义前提变化{};
            类别保护.insert(类别保护.end(), p.完整保护组.begin(), p.完整保护组.end());
        }
        结果.参数引用组.push_back({static_cast<std::uint32_t>(*序号),
            *类型, *引用});
    }
    std::sort(结果.参数引用组.begin(), 结果.参数引用组.end(),
        [](const auto& a, const auto& b) { return a.序号 < b.序号; });
    for (std::size_t i = 0; i < 结果.参数引用组.size(); ++i) {
        const auto& 规格 = 方法信息->参数规格组[i].有序规格;
        if (!规格 || 结果.参数引用组[i].序号 != 规格->序号
            || 结果.参数引用组[i].信息类型 != 规格->信息类型)
            throw std::runtime_error("direct invocation parameter order damaged");
        const auto 引用 = 结果.参数引用组[i].信息节点;
        if (规格->信息类型 == 新方法参数信息类型::词条) {
            const auto* reader=词条读取_;
            const auto 当前规格=*规格;
            锁.unlock();
            const auto 当前=读取方法词条实参(reader,当前规格,引用,存在概念服务_,特征服务_);
            锁.lock();
            if(当前.first==新方法操作状态::参数引用失效)return {};
            if(当前.first==新方法操作状态::资源失败)throw std::bad_alloc{};
            if(当前.first!=新方法操作状态::已找到)
                throw std::runtime_error("direct invocation word reader unavailable or incompatible");
        } else if (规格->信息类型 == 新方法参数信息类型::存在) {
            const auto ordinary=存在概念服务_.读取普通存在概念定义保护(规格->范围概念节点);
            if(ordinary.状态==正式概念读取状态::前提已变化)throw 引用内容定义前提变化{};
            if(ordinary.状态==正式概念读取状态::非本提供者概念) {
                if(!追加方法存在范围保护(规格->范围概念节点,存在概念服务_,类别保护))
                    throw std::runtime_error("direct invocation formal range invalid");
                const auto* reader=正式信息读取_;
                if(!reader)throw std::runtime_error("direct invocation formal reader unavailable");
                const auto C=规格->范围概念节点;
                锁.unlock();
                const auto read=reader->读取正式信息参数保护(引用,C);
                锁.lock();
                const auto state=正式参数状态(read,引用);
                if(state==新方法操作状态::定义前提已变化)throw 引用内容定义前提变化{};
                if(state!=新方法操作状态::已找到)throw std::runtime_error("direct invocation formal identity unavailable");
                类别保护.insert(类别保护.end(),read.完整保护组.begin(),read.完整保护组.end());
            } else if(ordinary.状态!=正式概念读取状态::已读取||!ordinary.信息||!存在服务_.是存在节点(引用))
                throw std::runtime_error("direct invocation reference identity damaged");
        } else if (!特征服务_.获取特征(引用)) throw std::runtime_error("direct invocation reference identity damaged");
    }
    return 结果;
    } catch(const 引用内容定义前提变化&) {
        // 仅重读本次身份，没有动作或知识发布。
    }
    std::this_thread::yield();
    }
}

新方法直接调用身份 新_方法类::建立直接调用身份(
    稳定编码 方法节点, const std::vector<新方法参数角色绑定>& 绑定组,
    稳定编码 现实场景, 稳定编码 外设场景) {
    for (;;) {
    try {
    std::unique_lock 锁(新方法互斥);
    确保直接调用结构();
    const auto 方法 = 获取方法(方法节点);
    if (!方法 || 方法->实现方式 != 新方法实现方式::本能
        || 方法->启用状态 != 新方法启用状态::启用
        || !有效(现实场景) || !有效(外设场景)
        || !全局基础数据集.查询节点(现实场景)
        || !全局基础数据集.查询节点(外设场景)
        || !参数规格有效(方法->参数规格组, 特征概念服务_)
        || 绑定组.size() != 方法->参数规格组.size())
        throw std::invalid_argument("direct invocation input invalid");
    std::vector<新方法有序参数引用> 参数;
    std::vector<基础联合节点快照> 正式保护;
    for (const auto& 绑定 : 绑定组) {
        if (!绑定.有序引用 || !有效(绑定.有序引用->信息节点)
            || 有效(绑定.参数角色节点) || 绑定.实际存在节点
            || !绑定.实际特征组.empty() || 绑定.任务虚拟存在引用
            || 绑定.服务要求分项引用 || 绑定.服务要求确认引用
            || (绑定.世界场景节点 && !有效(*绑定.世界场景节点)))
            throw std::invalid_argument("direct invocation parameter missing");
        参数.push_back(*绑定.有序引用);
    }
    std::sort(参数.begin(), 参数.end(),
        [](const auto& a, const auto& b) { return a.序号 < b.序号; });
    for (std::size_t i = 0; i < 参数.size(); ++i) {
        const auto& 规格 = 方法->参数规格组[i].有序规格;
        if (!规格 || 参数[i].序号 != 规格->序号
            || 参数[i].信息类型 != 规格->信息类型
            || !全局基础数据集.查询节点(参数[i].信息节点))
            throw std::invalid_argument("direct invocation parameter shape invalid");
        const auto& 场景绑定=*std::find_if(绑定组.begin(),绑定组.end(),[&](const auto& b){
            return b.有序引用&&b.有序引用->序号==规格->序号;});
        const auto 所属=规格->所属存在位置;
        const bool 词条特征=规格->信息类型==新方法参数信息类型::特征&&所属
            &&方法->参数规格组[所属-1].有序规格->信息类型==新方法参数信息类型::词条;
        if(规格->信息类型==新方法参数信息类型::词条||词条特征) {
            const auto 当前规格=词条特征 ? *方法->参数规格组[所属-1].有序规格 : *规格;
            const auto W=词条特征 ? 参数[所属-1].信息节点 : 参数[i].信息节点;
            const auto* reader=词条读取_;
            锁.unlock();
            const auto 当前=读取方法词条实参(reader,当前规格,W,存在概念服务_,特征服务_);
            锁.lock();
            if(当前.first==新方法操作状态::资源失败)throw std::bad_alloc{};
            if(当前.first!=新方法操作状态::已找到)
                throw std::invalid_argument("direct invocation word parameter unavailable or incompatible");
            if((词条特征 && std::none_of(当前.second->特征组.begin(), 当前.second->特征组.end(),
                [&](const auto& 项) { return 项.特征节点 == 参数[i].信息节点; }))
                ||(场景绑定.世界场景节点&&场景绑定.世界场景节点!=当前.second->所属自然语言场景))
                throw std::invalid_argument("direct invocation word feature or language scene differs");
            if(!词条特征)continue;
        } else if(场景绑定.世界场景节点&&!场景服务_.是场景节点(*场景绑定.世界场景节点))
            throw std::invalid_argument("direct invocation parameter scene unavailable");
        if (规格->信息类型 == 新方法参数信息类型::存在) {
            const auto ordinary=存在概念服务_.读取普通存在概念定义保护(规格->范围概念节点);
            if(ordinary.状态==正式概念读取状态::前提已变化)throw 引用内容定义前提变化{};
            if(ordinary.状态==正式概念读取状态::非本提供者概念) {
                if(!追加方法存在范围保护(规格->范围概念节点,存在概念服务_,正式保护))
                    throw std::invalid_argument("direct invocation formal range invalid");
                const auto* reader=正式信息读取_;
                if(!reader)throw std::runtime_error("direct invocation formal reader unavailable");
                const auto N=参数[i].信息节点,C=规格->范围概念节点;
                锁.unlock();
                const auto read=reader->读取正式信息参数保护(N,C);
                锁.lock();
                const auto state=正式参数状态(read,N);
                if(state==新方法操作状态::定义前提已变化)throw 引用内容定义前提变化{};
                if(state!=新方法操作状态::已找到)throw std::invalid_argument("direct invocation formal parameter unsupported or absent");
                正式保护.insert(正式保护.end(),read.完整保护组.begin(),read.完整保护组.end());
                continue;
            }
            if(ordinary.状态!=正式概念读取状态::已读取||!ordinary.信息)
                throw std::runtime_error("direct invocation ordinary range unavailable");
            const auto 概念 = 存在概念服务_.获取存在概念(规格->范围概念节点);
            if (!存在服务_.是存在节点(参数[i].信息节点) || !概念
                || (概念->种类 == 存在概念种类::专属
                    ? 概念->对应存在节点 != 参数[i].信息节点
                    : 存在服务_.判定存在概念匹配状态(参数[i].信息节点, 规格->范围概念节点) != 新存在概念匹配状态::匹配))
                throw std::invalid_argument("direct invocation existence outside declared range");
        } else {
            const auto 特征 = 特征服务_.获取特征(参数[i].信息节点);
            if (!特征 || !特征概念为或从属于(特征->特征概念节点, 规格->范围概念节点, 特征概念服务_))
                throw std::invalid_argument("direct invocation feature outside declared range");
        }
    }
    基础节点组追加请求 请求;
    请求.预期联合节点组=std::move(正式保护);
    基础组内新节点 调用;
    调用.字段组 = {{节点类型字段, 直接调用类型},
        {直接调用方法字段, 方法节点}, {直接调用现实场景字段, 现实场景},
        {直接调用外设场景字段, 外设场景},
        {直接调用收束字段, 基础值{std::int64_t{0}}}};
    for (std::size_t i = 0; i < 参数.size(); ++i)
        调用.字段组.push_back({直接调用参数字段, 基础组内节点引用{i + 1}});
    请求.新节点组.push_back(std::move(调用));
    for (const auto& 项 : 参数) {
        const auto 类别 = 读取参数类别概念保护(项.信息类型, 存在概念服务_, 特征概念服务_);
        请求.预期联合节点组.insert(请求.预期联合节点组.end(), 类别.完整保护组.begin(), 类别.完整保护组.end());
        基础组内新节点 p;
        p.字段组 = {{节点类型字段, 直接调用参数类型},
            {直接调用序号字段, 基础值{static_cast<std::int64_t>(项.序号)}},
            {直接调用类别概念字段, 类别.类别概念},
            {直接调用信息引用字段, 项.信息节点}};
        请求.新节点组.push_back(std::move(p));
    }
    请求.追加字段组.push_back({直接调用根, 直接调用成员字段, 基础组内节点引用{0}});
    const std::vector<稳定编码> 直接保护节点{直接调用根, 节点类型字段, 直接调用类型,
        直接调用参数类型, 直接调用方法字段, 直接调用现实场景字段, 直接调用外设场景字段,
        直接调用参数字段, 直接调用序号字段, 直接调用类别概念字段,
        直接调用信息引用字段, 直接调用收束字段, 直接调用成员字段};
    const auto 根快照 = 全局基础数据集.查询节点组完整结构(直接保护节点);
    if (根快照.状态 != 基础结构操作状态::已完成 || 根快照.节点组.size() != 直接保护节点.size())
        throw std::runtime_error("direct invocation root snapshot unavailable");
    for (const auto& n : 根快照.节点组) {
        if (!n.快照) throw std::runtime_error("direct invocation schema protection unavailable");
        请求.预期联合节点组.push_back(*n.快照);
    }
    std::map<稳定编码, 基础联合节点快照> 唯一保护;
    for (const auto& n : 请求.预期联合节点组) {
        const auto [it, added] = 唯一保护.emplace(n.结构.节点.编码, n);
        if (!added && (it->second.结构.字段组 != n.结构.字段组 || it->second.入向字段组 != n.入向字段组
            || it->second.入向外部关系组 != n.入向外部关系组 || it->second.出向外部关系组 != n.出向外部关系组))
            throw 引用内容定义前提变化{};
    }
    请求.预期联合节点组.clear();
    for (const auto& [id, n] : 唯一保护) 请求.预期联合节点组.push_back(n);
    const auto 发布 = 全局基础数据集.原子追加节点组(请求);
    if(发布.状态==基础节点组追加状态::预期不符)throw 引用内容定义前提变化{};
    if (发布.状态 != 基础节点组追加状态::已完成 || 发布.新节点组.size() != 参数.size() + 1)
        throw std::runtime_error("direct invocation atomic publication failed");
    // 成功发布即返回本次事实，后续读取观察当前状态，不重放已发布身份。
    return {发布.新节点组.front(),方法节点,现实场景,外设场景,std::move(参数),{},false};
    } catch(const 引用内容定义前提变化&) {
        // L1零发布冲突；锁及准备材料在本轮结束时释放。
    }
    std::this_thread::yield();
    }
}

void 新_方法类::绑定直接调用动作(稳定编码 调用, 稳定编码 动作) {
    const auto 旧 = 读取直接调用身份(调用);
    std::unique_lock 锁(新方法互斥);
    if (!旧 || 旧->已收束 || !有效(动作) || !全局基础数据集.查询节点(动作))
        throw std::invalid_argument("direct invocation action invalid");
    if (旧->动作节点 == 动作) return;
    if (旧->动作节点) throw std::runtime_error("direct invocation action conflict");
    const auto currentAction=读取可选节点字段(调用,直接调用动作字段);
    if(!currentAction||有效(*currentAction)||读取唯一I64字段(调用,直接调用收束字段)!=std::optional<std::int64_t>{0})
        throw 引用内容定义前提变化{};
    const auto 快照 = 全局基础数据集.查询节点组完整结构({调用});
    if (快照.状态 != 基础结构操作状态::已完成 || 快照.节点组.size() != 1
        || !快照.节点组.front().快照)
        throw std::runtime_error("direct invocation snapshot unavailable");
    基础字段组合变化请求 请求;
    请求.预期节点组.push_back(*快照.节点组.front().快照);
    请求.追加字段组.push_back({调用, 直接调用动作字段, 动作});
    if (全局基础数据集.原子比较变更字段组(请求).状态 != 基础字段组合变化状态::已完成)
        throw std::runtime_error("direct invocation action publication failed");
    锁.unlock();
    const auto readback=读取直接调用身份(调用);
    if (!readback||readback->动作节点 != 动作)
        throw std::runtime_error("direct invocation action readback failed");
}

void 新_方法类::收束直接调用身份(稳定编码 调用) {
    const auto 旧 = 读取直接调用身份(调用);
    std::unique_lock 锁(新方法互斥);
    if (!旧) throw std::invalid_argument("direct invocation absent");
    if (旧->已收束) return;
    const auto 字段 = 读取唯一I64字段记录(调用, 直接调用收束字段);
    const auto 快照 = 全局基础数据集.查询节点组完整结构({调用});
    if (!字段 || 字段->值 != 0 || 快照.状态 != 基础结构操作状态::已完成
        || 快照.节点组.size() != 1 || !快照.节点组.front().快照)
        throw std::runtime_error("direct invocation closing snapshot damaged");
    基础字段组合变化请求 请求;
    请求.预期节点组.push_back(*快照.节点组.front().快照);
    请求.替换字段组.push_back({字段->关系, 基础值{std::int64_t{1}}});
    if (全局基础数据集.原子比较变更字段组(请求).状态 != 基础字段组合变化状态::已完成)
        throw std::runtime_error("direct invocation close failed");
    锁.unlock();
    const auto readback=读取直接调用身份(调用);
    if (!readback||!readback->已收束)
        throw std::runtime_error("direct invocation close readback failed");
}

} // namespace 海中鱼巣

namespace 海中鱼巣 {
std::optional<存在概念信息> 新_方法类::读取模拟存在范围(稳定编码 C) const noexcept {
    return 存在概念服务_.获取存在概念(C);
}
存在范围语义保护结果 新_方法类::读取模拟存在范围语义保护(稳定编码 C) const noexcept {
    return 存在概念服务_.读取存在范围语义保护(C);
}
存在概念实例候选读取结果 新_方法类::读取模拟存在原型候选(稳定编码 C) const noexcept {
    return 存在概念服务_.读取下位专属实例候选完整(C);
}
新存在概念匹配结果 新_方法类::比较模拟存在匹配材料(const 新存在概念匹配材料& m) const noexcept {
    return 存在服务_.比较存在概念匹配材料(m);
}
新方法执行分支读取结果 新_方法类::读取执行分支必要条件(稳定编码 M,std::optional<稳定编码> B) const noexcept {
    for (;;) {
    try {
        std::lock_guard lock(新方法互斥);
        新方法执行分支读取结果 out;
        if(!节点类型为(M,方法节点类型))return out;
        const auto m=读取方法信息(M,特征概念服务_,*this,存在概念服务_);
        if(!m)throw std::runtime_error("method branch structure unreadable");
        if(m->执行分支组.empty()) {
            if(B){out.状态=新方法执行分支读取状态::分支不属于方法;return out;}
        } else {
            if(!B){out.状态=新方法执行分支读取状态::分支未指定;return out;}
            const auto it=std::find_if(m->执行分支组.begin(),m->执行分支组.end(),[&](const auto& b){return b.节点==*B;});
            if(it==m->执行分支组.end()){out.状态=新方法执行分支读取状态::分支不属于方法;return out;}
            out.分支=*it;
        }
        out.有效必要条件.条件.关系规则=新方法项关系规则::全部成立;
        // 规格隐含的存在范围只投影到调用期视图，不回写Vm或历史C/R。
        for(const auto& p:m->参数规格组)if(p.有序规格
            &&是特征持有参数(p.有序规格->信息类型)) {
            新方法条件项定义 c;c.参数序号=p.有序规格->序号;
            c.存在概念节点=p.有序规格->范围概念节点;
            out.有效必要条件.条件.条件项组.push_back(std::move(c));
        }
        if(m->初始必要条件) {
            合入必要条件(out.有效必要条件,m->初始必要条件->条件);
            for(const auto& r:m->初始必要条件->场景事实限定组)
                if(std::find(out.有效必要条件.场景事实限定组.begin(),out.有效必要条件.场景事实限定组.end(),r)
                    ==out.有效必要条件.场景事实限定组.end())out.有效必要条件.场景事实限定组.push_back(r);
        }
        if(out.分支)合入必要条件(out.有效必要条件,out.分支->定义.必要条件);
        for(const auto& r:m->初始模拟关系组)
            if(std::find(out.有效必要条件.条件.参数关系组.begin(),out.有效必要条件.条件.参数关系组.end(),r)==out.有效必要条件.条件.参数关系组.end())out.有效必要条件.条件.参数关系组.push_back(r);
        out.状态=新方法执行分支读取状态::已读取;return out;
    } catch (const 引用内容定义前提变化&) {
        // owner锁随本轮局部对象释放后重读当前分支。
    } catch(...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取正式执行分支",L"方法分支完整读取或投影发生工程故障。");
    }
    std::this_thread::yield();
    }
}
新_方法类::必要条件合入状态 新_方法类::合入未涵盖必要条件(const 新方法初始必要条件定义& 必要条件,
    const std::vector<新方法参数规格>& 参数规格组,新方法条件定义& 条件) const noexcept {
    try {
        for(const auto& w:必要条件.条件.条件项组) {
            新方法初始必要条件定义 单项;单项.条件.关系规则=新方法项关系规则::全部成立;单项.条件.条件项组.push_back(w);
            if(!条件涵盖必要项(条件,单项,参数规格组,特征概念服务_,存在概念服务_))条件.条件项组.push_back(w);
        }
        for(const auto& r:必要条件.条件.参数关系组)
            if(std::find(条件.参数关系组.begin(),条件.参数关系组.end(),r)==条件.参数关系组.end())条件.参数关系组.push_back(r);
        for(const auto* 组:{&必要条件.条件.场景事实限定组,&必要条件.场景事实限定组})for(const auto& r:*组)
            if(std::find(条件.场景事实限定组.begin(),条件.场景事实限定组.end(),r)==条件.场景事实限定组.end())条件.场景事实限定组.push_back(r);
        return 必要条件合入状态::已合入;
    } catch (const 引用内容定义前提变化&) {
        return 必要条件合入状态::前提已变化;
    } catch(...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"合入未涵盖必要条件",L"必要条件涵盖判定发生工程故障。");
    }
}
新方法参数特征声明读取结果 新_方法类::读取存在参数特征声明(
    稳定编码 M,std::optional<稳定编码> B,std::uint32_t position,稳定编码 FT) const noexcept {
    for (;;) {
        try {
            新方法参数特征声明读取结果 out;
            // 当前方法和定义只读取本轮需要的部分；不维护上一轮声明快照或遍历完整方法图互证。
            const auto method=获取方法(M);
            if(!method)return out;
            const auto branch=读取执行分支必要条件(M,B);
            if(branch.状态==新方法执行分支读取状态::前提已变化)throw 引用内容定义前提变化{};
            if(branch.状态!=新方法执行分支读取状态::已读取)return out;
            const auto spec=std::find_if(method->参数规格组.begin(),method->参数规格组.end(),[&](const auto& x){
                return x.有序规格&&x.有序规格->序号==position;});
            if(spec==method->参数规格组.end()){out.状态=新方法参数特征读取状态::参数未绑定;return out;}
            const auto& 参数=*spec->有序规格;
            if(!是特征持有参数(参数.信息类型)){out.状态=新方法参数特征读取状态::信息类型不符;return out;}
            if(!特征概念服务_.是特征概念节点(FT))return out;
            const auto definition=特征服务_.捕获特征匹配定义(FT);
            if(!definition||definition->特征类型根!=FT||definition->概念节点!=FT)return out;
            bool authorized=false;
            if(参数.信息类型==新方法参数信息类型::词条) {
                if(!方法持有者范围有效(参数,存在概念服务_))return out;
                if (!词条参数特征类型允许(FT, 特征概念服务_)) return out;
            } else {
                const auto existence=存在概念服务_.读取普通存在概念定义保护(参数.范围概念节点);
                if(existence.状态==正式概念读取状态::前提已变化)throw 引用内容定义前提变化{};
                // 正式需求等内容走原信息reader，不在这里冒充普通E的实际F。
                if(existence.状态==正式概念读取状态::非本提供者概念)return out;
                if(!existence.信息)return out;
                authorized=std::any_of(existence.信息->特征组.begin(),existence.信息->特征组.end(),
                    [&](const auto& f){return f.特征类型概念==FT;});
            }
            const auto fromItems=[&](const auto& group) {
                return std::any_of(group.begin(),group.end(),[&](const auto& x){
                    return !x.任务成员变量序号&&x.参数序号==position&&x.特征类型概念节点==std::optional{FT};});
            };
            authorized=authorized||fromItems(branch.有效必要条件.条件.条件项组);
            for(const auto& pair:method->条件结果组) {
                const bool selected=B?std::find(pair.来源执行分支节点组.begin(),pair.来源执行分支节点组.end(),*B)!=pair.来源执行分支节点组.end():pair.来源执行分支节点组.empty();
                if(selected)authorized=authorized||fromItems(pair.条件.条件项组)||fromItems(pair.结果.结果项组)||fromItems(pair.结果.确认要求.禁止结果项组);
            }
            authorized=authorized||std::binary_search(参数.必需特征类型组.begin(),参数.必需特征类型组.end(),FT);
            for(const auto& 项:method->参数规格组) {
                if(!项.有序规格||项.有序规格->信息类型!=新方法参数信息类型::特征
                    ||项.有序规格->所属存在位置!=position)continue;
                const auto d=特征服务_.捕获特征匹配定义(项.有序规格->范围概念节点);
                if(d&&d->特征类型根==FT)authorized=true;
            }
            if(authorized){out.状态=新方法参数特征读取状态::已读取;out.特征定义=*definition;}
            return out;
        } catch(const 引用内容定义前提变化&) {
            // 本轮只读材料失效，离开全部owner读取后重新读取；不向上返回旧快照错误。
        } catch(...) {
            工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取存在参数特征声明",L"读取方法当前声明发生工程故障。");
        }
        std::this_thread::yield();
    }
}
稳定编码 新_方法类::确保已确认消息连接概念() {
    const auto result=存在概念服务_.建立或取得已确认消息连接概念();
    const auto proof=存在概念服务_.读取已确认消息连接概念保护();
    if(!result.信息||!proof.信息||result.信息!=proof.信息||proof.完整保护组.empty())
        throw std::runtime_error("confirmed message connection concept unavailable");
    return proof.信息->节点;
}
正式结构概念保护结果 新_方法类::读取初始场景事实概念保护(稳定编码 c) const noexcept {
    const auto proof=存在概念服务_.读取已确认消息连接概念保护();
    if(!proof.信息)return proof;
    if(proof.信息->节点==c)return proof;
    return {};
}
新方法建立结果 新_方法类::补齐空头初始必要条件(稳定编码 method,const 新方法信息& expected,
    const 新方法初始必要条件定义& definition) noexcept {
    struct 初始声明前提变化 final {};
    try {
        if(!有效(method)||method!=expected.节点||!初始必要条件形状有效(definition))
            return {新方法操作状态::函数登记冲突,{}};
        // 本轮例外的唯一函数声明来源；租约与所有跨owner读取均先于方法锁。
        const auto source=[]() -> std::pair<std::optional<新服务对象获取配置>,std::optional<稳定编码>> {
            const auto port=新_本能函数管理类::租用服务对象获取端口();
            if(!port)return {};
            return {port->读取登记材料(),port->读取初始连接关系概念()};
        }(); // 只保留值副本；端口租约已在进入方法锁前释放。
        const auto& config=source.first;
        if(!config||!source.second)return {新方法操作状态::结构不一致,{}};
        if(definition.条件.条件项组.front().存在概念节点!=std::optional{config->人类概念节点}
            ||source.second!=std::optional{definition.场景事实限定组.front().关系概念节点}
            ||!新_本能函数管理类::解析(新本能函数登记名_获取服务对象).函数)
            return {新方法操作状态::函数登记冲突,{}};
        const auto 预期规格=获取服务对象有序规格(*config);
        if(expected.本能函数登记名!=新本能函数登记名_获取服务对象||expected.来源方法节点||expected.实例根节点
            ||expected.作用域!=新方法作用域::普通世界||expected.实现方式!=新方法实现方式::本能
            ||expected.行为!=新方法行为::改变||expected.启用状态!=新方法启用状态::启用
            ||expected.参数规格组.size()!=5||!expected.初始模拟事件参数组.empty()
            ||!expected.初始模拟关系组.empty()
            ||expected.方法头.作用存在概念组!=std::vector<稳定编码>{config->自我概念节点}
            ||expected.方法头.涉及特征类型概念组!=有序规格涉及特征类型(预期规格)
            ||expected.方法头.可能结果概念组!=std::vector<稳定编码>{config->服务对象特征类型节点})
            return {新方法操作状态::函数登记冲突,{}};
        for(std::uint32_t i=0;i<5;++i) {
            新方法参数规格 spec;spec.有序规格=预期规格[i];
            if(expected.参数规格组[i]!=spec)return {新方法操作状态::函数登记冲突,{}};
        }
        std::vector<基础联合节点快照> protection;
        const auto merge=[&](const auto& group) {
            for(const auto& s:group) {
                const auto it=std::find_if(protection.begin(),protection.end(),[&](const auto& old){return old.结构.节点==s.结构.节点;});
                if(it!=protection.end()) {
                    if(it->结构.字段组!=s.结构.字段组
                        ||it->入向字段组!=s.入向字段组||it->入向外部关系组!=s.入向外部关系组
                        ||it->出向外部关系组!=s.出向外部关系组)throw 初始声明前提变化{};
                }
                else protection.push_back(s);
            }
        };
        const auto relation=读取初始场景事实概念保护(definition.场景事实限定组.front().关系概念节点);
        if(relation.状态==正式结构操作状态::前提已变化)return {新方法操作状态::结构不一致,{}};
        if(!relation.信息||relation.完整保护组.empty())return {新方法操作状态::函数登记冲突,{}};
        merge(relation.完整保护组);
        for(const auto c:{config->人类概念节点,config->自我概念节点,config->通用存在概念节点}) {
            const auto p=存在概念服务_.读取普通存在概念定义保护(c);
            if(p.状态==正式概念读取状态::前提已变化)return {新方法操作状态::结构不一致,{}};
            if(!p.信息||p.完整保护组.empty())throw std::runtime_error("initial existence concept protection unavailable");
            merge(p.完整保护组);
        }
        for(const auto c:{config->交互者特征类型节点,config->服务对象特征类型节点}) {
            const auto ft=特征概念服务_.读取特征概念定义保护(c);
            if(ft.前提已变化)return {新方法操作状态::结构不一致,{}};
            if(!ft.信息||ft.预期节点组.empty())throw std::runtime_error("initial feature concept protection unavailable");
            merge(ft.预期节点组);
        }
        std::lock_guard lock(新方法互斥);
        if(!结构仍存在()||!方法属于根(method))return {新方法操作状态::结构不一致,{}};
        const auto registered=读取多节点字段(新方法组织根节点,方法成员字段,特征概念服务_);
        if(!registered)return {新方法操作状态::结构不一致,{}};
        std::optional<稳定编码> named;
        for(const auto n:*registered) {
            const auto info=读取方法信息本体(n,特征概念服务_,*this,存在概念服务_,false,false);
            if(!info)return {新方法操作状态::结构不一致,{}};
            if(info->来源方法节点||info->本能函数登记名!=新本能函数登记名_获取服务对象)continue;
            if(named)return {新方法操作状态::函数登记冲突,{}};
            named=n;
        }
        if(named!=std::optional{method})return {新方法操作状态::函数登记冲突,{}};
        const auto current=读取方法信息本体(method,特征概念服务_,*this,存在概念服务_,false,false);
        if(!current||*current!=expected)return {新方法操作状态::结构不一致,{}};
        if(current->初始必要条件) {
            if(*current->初始必要条件!=definition)return {新方法操作状态::函数登记冲突,{}};
            return {新方法操作状态::已找到,method};
        }
        if(!current->条件结果组.empty())return {新方法操作状态::函数登记冲突,{}};
        // 本owner完整独占子图及根登记均参与CAS；包括Vm缺声明和M空C/R。
        std::vector<稳定编码> nodes{method,新方法组织根节点};
        for(const auto n:*registered)if(std::find(nodes.begin(),nodes.end(),n)==nodes.end())nodes.push_back(n);
        for(std::size_t i=0;i<nodes.size();++i) {
            const auto n=nodes[i];const auto type=读取唯一节点字段(n,节点类型字段);
            if(!type) { if(n==新方法组织根节点)continue;throw std::runtime_error("initial declaration method graph type"); }
            for(const auto& schema:方法复制字段规格(*type))if(schema.种类==复制字段种类::独占子节点)
                for(const auto& f:全局基础数据集.查询字段(n,schema.字段)) {
                    const auto* target=std::get_if<稳定编码>(&f.内容);
                    if(!target)throw std::runtime_error("initial declaration method child shape");
                    if(std::find(nodes.begin(),nodes.end(),*target)==nodes.end())nodes.push_back(*target);
                }
        }
        for(const auto key:{初始必要条件字段,初始声明节点类型,初始声明项节点类型,初始场景限定节点类型,
            初始声明格式字段,初始声明所属字段,初始场景限定字段,初始关系概念字段,初始四位置字段,
            节点类型字段,关系规则字段,参数位置关系字段,条件项字段,条件项存在概念字段,顺序字段,有序参数序号字段})
            if(std::find(nodes.begin(),nodes.end(),key)==nodes.end())nodes.push_back(key);
        const auto snapshots=全局基础数据集.查询节点组完整结构(nodes);
        if(snapshots.状态!=基础结构操作状态::已完成||snapshots.节点组.size()!=nodes.size())throw std::runtime_error("initial declaration snapshots");
        for(const auto& n:snapshots.节点组) { if(!n.快照)throw std::runtime_error("initial declaration incomplete snapshot");merge(std::vector<基础联合节点快照>{*n.快照}); }
        const auto again=读取方法信息本体(method,特征概念服务_,*this,存在概念服务_,false,false);
        if(!again||*again!=expected)return {新方法操作状态::结构不一致,{}};
        基础节点组追加请求 request;request.预期联合节点组=std::move(protection);
        request.新节点组.resize(3);
        const auto field=[&](std::size_t n,稳定编码 key,基础组内字段内容 value){request.新节点组[n].字段组.push_back({key,std::move(value)});};
        field(0,节点类型字段,初始声明节点类型);field(0,初始声明格式字段,基础值{std::int64_t{1}});
        field(0,初始声明所属字段,current->方法虚拟存在节点);field(0,关系规则字段,基础值{static_cast<std::int64_t>(新方法项关系规则::全部成立)});
        field(0,参数位置关系字段,基础值{std::vector<std::int64_t>{2,2,3}});
        field(0,条件项字段,基础组内节点引用{1});field(0,初始场景限定字段,基础组内节点引用{2});
        field(1,节点类型字段,初始声明项节点类型);field(1,初始声明所属字段,基础组内节点引用{0});
        field(1,顺序字段,基础值{std::int64_t{0}});field(1,有序参数序号字段,基础值{std::int64_t{3}});
        field(1,条件项存在概念字段,config->人类概念节点);
        field(2,节点类型字段,初始场景限定节点类型);field(2,初始声明所属字段,基础组内节点引用{0});
        field(2,顺序字段,基础值{std::int64_t{0}});field(2,初始关系概念字段,relation.信息->节点);
        field(2,初始四位置字段,基础值{std::vector<std::int64_t>{1,2,3,4}});
        request.追加字段组.push_back({current->方法虚拟存在节点,初始必要条件字段,基础组内节点引用{0}});
        const auto published=全局基础数据集.原子追加节点组(request);
        if(published.状态==基础节点组追加状态::预期不符)return {新方法操作状态::结构不一致,{}};
        auto wanted=expected;wanted.初始必要条件=definition;
        const auto readback=读取方法信息本体(method,特征概念服务_,*this,存在概念服务_,false,false);
        if(published.状态!=基础节点组追加状态::已完成) {
            if(published.状态==基础节点组追加状态::内部不一致&&readback&&*readback==wanted)
                return {新方法操作状态::已找到,method}; // 只以同M完整请求消歧，不重发。
            throw std::runtime_error("initial declaration publication failed");
        }
        if(published.新节点组.size()!=3)throw std::runtime_error("initial declaration publication identities");
        if(!readback||*readback!=wanted)throw std::runtime_error("initial declaration publication readback mismatch");
        return {新方法操作状态::已找到,method};
    }catch(const 初始声明前提变化&) { return {新方法操作状态::结构不一致,{}}; }
    catch(const 引用内容定义前提变化&) {
        return {新方法操作状态::定义前提已变化,method};
    } catch(...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"补齐空头必要条件",L"必要声明保护、原子发表或读回失败；保留已有事实。");
    }
}
std::optional<新特征匹配定义快照> 新_方法类::读取模拟特征范围(稳定编码 C) const noexcept {
    return 特征服务_.捕获特征匹配定义(C);
}
bool 新_方法类::是模拟特征概念(稳定编码 C) const noexcept {
    const auto 判定 = 特征概念服务_.判定特征概念节点(C);
    if (!判定)
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"是模拟特征概念",
            L"特征概念身份判定未完成，未形成业务判定。");
    return *判定;
}
std::optional<新特征值信息> 新_方法类::读取模拟候选材料(稳定编码 V) const noexcept {
    return 特征值服务_.获取特征值(V);
}
std::optional<新存在信息> 新_方法类::读取模拟来源存在(稳定编码 E) const {
    return 存在服务_.获取存在(E);
}
std::optional<新特征匹配实例快照> 新_方法类::读取模拟来源特征(稳定编码 F) const {
    return 特征服务_.捕获特征匹配实例(F);
}
std::optional<新特征信息> 新_方法类::读取模拟来源完整特征(稳定编码 F) const {
    return 特征服务_.获取特征(F);
}
先天信息概念结果 新_方法类::读取模拟需求概念组() const noexcept {
    return 存在概念服务_.读取先天需求任务方法概念();
}
正式结构概念保护结果 新_方法类::读取模拟正式结构(稳定编码 C) const noexcept {
    return 存在概念服务_.读取正式结构概念保护(C);
}
先天特征类型槽结果 新_方法类::读取模拟先天特征类型槽(std::uint32_t 语义序号) const noexcept {
    return 特征概念服务_.读取先天特征类型槽(语义序号);
}
稳定编码 新_方法类::建立模拟I64目标域(稳定编码 C,std::int64_t x) {
    const auto c=特征概念服务_.获取特征概念(C);
    if(!c||c->材料类型!=特征概念材料物理类型::I64标量
        ||c->比较规则!=特征概念比较规则::I64数值)
        throw std::invalid_argument("simulation target requires an I64 definition");
    const auto* ranges=std::get_if<std::vector<特征概念I64闭区间>>(&c->值域);
    if(!ranges||std::none_of(ranges->begin(),ranges->end(),[&](const auto& r){return r.下界<=x&&x<=r.上界;}))
        throw std::invalid_argument("simulation target outside formal domain");
    const 特征概念值域 exact=std::vector<特征概念I64闭区间>{{x,x}};
    if(c->值域==exact)return C;
    特征概念定义 d;d.材料类型=c->材料类型;d.单位=c->单位;
    d.比较规则=c->比较规则;d.聚合规则=c->聚合规则;d.值域=exact;
    const auto result=特征概念服务_.建立或取得特征概念(d,特征值服务_,C);
    if(!有效(result))throw std::runtime_error("simulation target concept publication failed");
    return result;
}
}
namespace 海中鱼巣 {
稳定编码 新_方法类::保存模拟材料值域(稳定编码 C,const 新特征值信息& v) {
    const auto c=特征概念服务_.获取特征概念(C);
    if(!c)throw std::runtime_error("simulation input concept unavailable");
    if(c->材料类型==特征概念材料物理类型::需求节点引用
        ||c->材料类型==特征概念材料物理类型::信息节点引用
        ||c->比较规则==特征概念比较规则::需求集合身份判等)
        throw std::invalid_argument("typed information reference requires a rebindable knowledge abstraction");
    const auto n=特征值服务_.保存或取得特征值(v.物理类型,v.材料);
    if(!有效(n))throw std::runtime_error("simulation knowledge material publish failed");
    特征概念定义 d;d.材料类型=c->材料类型;d.单位=c->单位;
    d.比较规则=c->比较规则;d.聚合规则=c->聚合规则;
    d.值域=std::vector<稳定编码>{n};
    if(d.值域==c->值域)return C;
    const auto result=特征概念服务_.建立或取得特征概念(d,特征值服务_,C);
    if(!有效(result))throw std::runtime_error("simulation exact material domain publish failed");
    return result;
}
稳定编码 新_方法类::保存模拟集合约束值域(稳定编码 C,const 特征概念值域& 约束) {
    const auto c=特征概念服务_.获取特征概念(C);
    if(!c)throw std::runtime_error("simulation input concept unavailable");
    if(!std::holds_alternative<特征概念存在集合值域>(约束)
        ||c->比较规则!=特征概念比较规则::存在集合身份判等)
        throw std::invalid_argument("collection constraint domain only supports existence sets in this slice");
    特征概念定义 d;d.材料类型=c->材料类型;d.单位=c->单位;
    d.比较规则=c->比较规则;d.聚合规则=c->聚合规则;d.值域=约束;
    if(d.值域==c->值域)return C;
    const auto result=特征概念服务_.建立或取得特征概念(d,特征值服务_,C);
    if(!有效(result))throw std::runtime_error("simulation collection constraint domain publish failed");
    return result;
}
// R102（SET-DOMAIN-MEMBER-CONCEPT §4.2）：不取方法锁。槽3方法按节点身份判定；下位子概念（221/222等）
// 继承的共同约束、条件结果知识为成员组织项，通用判定器尚不判成员组织，本轮如实返回未知。
// TRIAL-LEARNING-REVIEW §3.3：只读合法性核验。诊断责任：向上送出（工程异常→资源失败状态，不写入）。
新方法学习发布状态 新_方法类::核验条件结果定义(
    稳定编码 方法节点, const 新方法条件结果定义& 原定义) const noexcept {
    try {
        std::lock_guard 锁(新方法互斥);
        if (!有效(方法节点) || !节点类型为(方法节点, 方法节点类型)) return 新方法学习发布状态::方法不可用;
        const auto 方法 = 读取方法信息(方法节点, 特征概念服务_, *this, 存在概念服务_);
        if (!方法) return 新方法学习发布状态::结构不一致;
        if (方法->来源方法节点 || 方法->实例根节点 || 方法->实现方式 != 新方法实现方式::本能
            || 方法->启用状态 != 新方法启用状态::启用) return 新方法学习发布状态::方法不可用;
        auto 定义 = 原定义; 规范内容关系配对(定义);
        if (定义.结果.结果项组.empty() && 定义.结果.参数关系组.empty() && 定义.结果.场景事实限定组.empty())
            return 新方法学习发布状态::条件结果不合法;
        return 学习配对定义合法(*方法, 定义, 特征概念服务_, 存在概念服务_)
            ? 新方法学习发布状态::已发布 : 新方法学习发布状态::条件结果不合法;
    } catch (...) {
        return 新方法学习发布状态::资源失败;
    }
}

// TRIAL-LEARNING-DIRECT：方法owner直接发布尝试配对（无评审授权）。诊断责任：向上送出（发布后读回不符→发布结果未知，由学习侧按原请求收敛并上报）。
新方法学习发布结果 新_方法类::发布尝试条件结果(const 新尝试补全候选& 候选) noexcept {
    新方法学习发布结果 结果;
    {
        std::lock_guard 锁(新方法互斥);
        学习前确保配对数特征(候选.方法节点);
        结果 = 发布尝试条件结果内部(候选);
        if (结果.状态 == 新方法学习发布状态::已发布) {
            std::vector<稳定编码> 来源 = 结果.新配对节点组;
            if (结果.已有配对节点) 来源.push_back(*结果.已有配对节点);
            if (有效(候选.执行实例节点)) 来源.push_back(候选.执行实例节点);
            学习后形成方法状态(候选.方法节点, 新方法学习种类::尝试补全, 来源);
        }
    }
    发出方法状态通知();
    return 结果;
}

新方法学习发布结果 新_方法类::发布尝试条件结果内部(const 新尝试补全候选& 候选) noexcept {
    新方法学习发布结果 结果;
    const auto 方法节点 = 候选.方法节点;
    if (!有效(方法节点) || !有效(候选.原配对节点)) {
        结果.状态 = 新方法学习发布状态::入口拒绝;
        return 结果;
    }
    std::vector<稳定编码> 已建节点, 已写字段;
    bool 已发布 = false;
    try {
        std::lock_guard 锁(新方法互斥);
        if (!节点类型为(方法节点, 方法节点类型)) {
            结果.状态 = 新方法学习发布状态::方法不可用;
            return 结果;
        }
        const auto 方法 = 读取方法信息(方法节点, 特征概念服务_, *this, 存在概念服务_);
        if (!方法) {
            结果.状态 = 新方法学习发布状态::结构不一致;
            return 结果;
        }
        if (方法->来源方法节点 || 方法->实例根节点
            || 方法->实现方式 != 新方法实现方式::本能
            || 方法->启用状态 != 新方法启用状态::启用) {
            结果.状态 = 新方法学习发布状态::方法不可用;
            return 结果;
        }
        // 按当前方法实时核对：原配对仍被引用、结果仍为空、条件仍为候选形成时的原条件。
        const auto 原配对 = std::find_if(方法->条件结果组.begin(), 方法->条件结果组.end(),
            [&](const auto& 项) { return 项.配对节点 == 候选.原配对节点; });
        if (原配对 == 方法->条件结果组.end()
            || 原配对->条件 != 候选.原配对条件
            || !原配对->结果.结果项组.empty() || !原配对->结果.参数关系组.empty()
            || !原配对->结果.场景事实限定组.empty()
            || (候选.与原条件相同 && 候选.实际条件 != 原配对->条件)) {
            结果.状态 = 新方法学习发布状态::前提已变化;
            return 结果;
        }
        新方法条件结果定义 定义;
        定义.条件 = 候选.实际条件;
        定义.结果 = 候选.实际结果;
        定义.来源执行分支节点组 = 候选.来源执行分支节点组;
        规范内容关系配对(定义);
        if (定义.结果.结果项组.empty() && 定义.结果.参数关系组.empty() && 定义.结果.场景事实限定组.empty()) {
            结果.状态 = 新方法学习发布状态::条件结果不合法;
            return 结果;
        }
        if (!学习配对定义合法(*方法, 定义, 特征概念服务_, 存在概念服务_)) {
            结果.状态 = 新方法学习发布状态::条件结果不合法;
            return 结果;
        }
        if (!候选.与原条件相同)
            for (const auto& 配对 : 方法->条件结果组)
                if (同一学习条件(配对.条件, 定义.条件)) {
                    结果.已有配对节点 = 配对.配对节点;
                    结果.状态 = 新方法学习发布状态::前提已变化;
                    return 结果;
                }
        std::int64_t 最大顺序 = -1;
        std::set<std::int64_t> 已有顺序;
        for (const auto& 配对 : 方法->条件结果组) {
            const auto 顺序 = 读取唯一I64字段(配对.配对节点, 顺序字段);
            if (!顺序 || *顺序 < 0 || !已有顺序.insert(*顺序).second
                || *顺序 == (std::numeric_limits<std::int64_t>::max)()) {
                结果.状态 = 新方法学习发布状态::结构不一致;
                return 结果;
            }
            最大顺序 = (std::max)(最大顺序, *顺序);
        }
        稳定编码 新配对{};
        if (!写入条件结果(方法节点, 定义, 最大顺序 + 1, 已建节点, 已写字段, &新配对)
            || !写入I64字段(新配对, 配对知识来源字段,
                static_cast<std::int64_t>(新方法知识来源种类::尝试), 已写字段)) {
            回滚(已写字段, 已建节点);
            结果.状态 = 新方法学习发布状态::资源失败;
            return 结果;
        }
        结果.新配对节点组.push_back(新配对);
        基础节点组追加请求 发布请求;
        发布请求.追加字段组.push_back({方法节点, 方法条件结果字段, 新配对});
        if (候选.与原条件相同) {
            // 只解除方法对原空结果配对的引用；原配对节点不物理删除（在途X与冻结采用仍引用）。
            const auto 引用组 = 全局基础数据集.查询字段(方法节点, 方法条件结果字段);
            const auto 引用 = std::find_if(引用组.begin(), 引用组.end(), [&](const auto& f) {
                const auto* n = std::get_if<稳定编码>(&f.内容);
                return n && *n == 候选.原配对节点;
            });
            if (引用 == 引用组.end()) {
                回滚(已写字段, 已建节点);
                结果.新配对节点组.clear();
                结果.状态 = 新方法学习发布状态::结构不一致;
                return 结果;
            }
            发布请求.删除字段关系组.push_back(引用->编码);
            结果.已有配对节点 = 候选.原配对节点;
        }
        std::vector<稳定编码> 发布图{方法节点};
        发布图.insert(发布图.end(), 已建节点.begin(), 已建节点.end());
        const auto 封存 = 封存方法片段(std::move(发布请求), 发布图, 存在概念服务_, 特征概念服务_);
        const auto 发布 = 全局基础数据集.原子追加封存片段组({封存});
        if (发布.状态 != 基础节点组追加状态::已完成) {
            回滚(已写字段, 已建节点);
            结果.新配对节点组.clear();
            结果.状态 = 新方法学习发布状态::资源失败;
            return 结果;
        }
        已发布 = true;
        const auto 读回 = 读取方法信息本体(方法节点, 特征概念服务_, *this, 存在概念服务_, false, true);
        const std::size_t 预期数 = 方法->条件结果组.size() + (候选.与原条件相同 ? 0u : 1u);
        if (!读回 || 读回->条件结果组.size() != 预期数) {
            结果.状态 = 新方法学习发布状态::发布结果未知;
            return 结果;
        }
        const auto 新项 = std::find_if(读回->条件结果组.begin(), 读回->条件结果组.end(),
            [&](const auto& 项) { return 项.配对节点 == 新配对; });
        const bool 原仍引用 = std::any_of(读回->条件结果组.begin(), 读回->条件结果组.end(),
            [&](const auto& 项) { return 项.配对节点 == 候选.原配对节点; });
        if (新项 == 读回->条件结果组.end() || 新项->命中次数 != 0
            || 新项->知识来源 != 新方法知识来源种类::尝试
            || 新项->条件 != 定义.条件 || 新项->结果 != 定义.结果
            || !std::is_permutation(新项->来源执行分支节点组.begin(), 新项->来源执行分支节点组.end(),
                定义.来源执行分支节点组.begin(), 定义.来源执行分支节点组.end())
            || 原仍引用 == 候选.与原条件相同
            || !std::all_of(方法->条件结果组.begin(), 方法->条件结果组.end(), [&](const auto& 旧项) {
                if (候选.与原条件相同 && 旧项.配对节点 == 候选.原配对节点) return true;
                return std::find(读回->条件结果组.begin(), 读回->条件结果组.end(), 旧项)
                    != 读回->条件结果组.end();
            })) {
            结果.状态 = 新方法学习发布状态::发布结果未知;
            return 结果;
        }
        结果.状态 = 新方法学习发布状态::已发布;
        return 结果;
    } catch (...) {
        if (!已发布) {
            回滚(已写字段, 已建节点);
            结果.新配对节点组.clear();
        }
        结果.状态 = 已发布 ? 新方法学习发布状态::发布结果未知 : 新方法学习发布状态::资源失败;
        return 结果;
    }
}

// TRIAL-LEARNING-REVIEW §3.3：只读，按原请求（规范化后的条件与结果、来源=尝试）查找。诊断责任：无适用错误分支（读取失败返回空，由调用方按未知处理）。
std::optional<稳定编码> 新_方法类::查找尝试配对(稳定编码 方法节点,
    const 新方法条件定义& 条件, const 新方法结果定义& 结果) const noexcept {
    try {
        新方法条件结果定义 定义; 定义.条件 = 条件; 定义.结果 = 结果; 规范内容关系配对(定义);
        const auto 方法 = 获取方法(方法节点);
        if (!方法) return std::nullopt;
        for (const auto& 配对 : 方法->条件结果组)
            if (配对.知识来源 == 新方法知识来源种类::尝试
                && 配对.条件 == 定义.条件 && 配对.结果 == 定义.结果) return 配对.配对节点;
    } catch (...) {}
    return std::nullopt;
}

特征成员概念归属结果 新_方法类::判定方法集合成员概念(稳定编码 M,稳定编码 C) const noexcept {
    using R=特征概念规则状态;using G=特征成员概念归属;
    try {
        if(!有效(M)||!有效(C))return {R::入口拒绝,G::未知};
        正式嵌套判定防护 防护(M,C);
        if(!防护.允许())return {R::已完成,G::未知};
        const auto 先天=存在概念服务_.读取先天需求任务方法概念();
        if(先天.状态!=正式结构操作状态::已读取||!先天.信息)return {R::读取接口未装配,G::未知};
        const auto 合法=存在概念服务_.正式结构概念属于(C,先天.信息->方法);
        if(!合法)return {R::前提已变化,G::未知};
        if(!*合法)return {R::入口拒绝,G::未知};
        if(!节点类型为(M,方法节点类型))return {R::已完成,G::不属于};
        if(C==先天.信息->方法)return {R::已完成,G::属于};
        return {R::已完成,G::未知};
    }catch(const std::bad_alloc&){return {R::资源失败,G::未知};}
    catch(...){return {R::内部不一致,G::未知};}
}

// ---- METHOD-LEARN-STATE：学习改变方法状态（3300、4211 §4.2、5350、5370）----
namespace {
std::array<std::int64_t, 2> 计算方法配对数(const 新方法信息& 方法) {
    std::int64_t 已知 = 0;
    for (const auto& 配对 : 方法.条件结果组)
        if (!配对.结果.结果项组.empty() || !配对.结果.参数关系组.empty()
            || !配对.结果.场景事实限定组.empty()) ++已知;
    return {static_cast<std::int64_t>(方法.条件结果组.size()), 已知};
}
std::size_t 配对数序号(新方法状态特征种类 种类) {
    if (种类 == 新方法状态特征种类::条件结果配对数) return 0;
    if (种类 == 新方法状态特征种类::已知结果配对数) return 1;
    throw std::logic_error("not a pair-count feature kind");
}
bool 方法学习种类合法(std::int64_t v) noexcept {
    return v >= static_cast<std::int64_t>(新方法学习种类::模拟发布)
        && v <= static_cast<std::int64_t>(新方法学习种类::规格修订删除);
}
bool 方法状态特征种类合法(std::int64_t v) noexcept {
    return v >= static_cast<std::int64_t>(新方法状态特征种类::启用)
        && v <= static_cast<std::int64_t>(新方法状态特征种类::已知结果配对数);
}
基础联合节点快照 读取方法状态治理根快照() {
    const auto 读 = 全局基础数据集.查询节点组完整结构({方法状态治理根});
    if (读.状态 != 基础结构操作状态::已完成 || 读.节点组.size() != 1 || !读.节点组.front().快照)
        throw std::runtime_error("method state governance root snapshot unavailable");
    return *读.节点组.front().快照;
}
}

bool 新_方法类::装配治理执行主体读取(std::function<std::optional<稳定编码>()> 读取) noexcept {
    std::lock_guard 锁(新方法互斥);
    if (!读取 || 治理执行主体读取_) return false;
    治理执行主体读取_ = std::move(读取);
    return true;
}

bool 新_方法类::装配方法状态就绪通知(std::function<void(稳定编码)> 通知) noexcept {
    std::lock_guard 锁(新方法互斥);
    if (!通知 || 方法状态就绪通知_) return false;
    方法状态就绪通知_ = std::move(通知);
    return true;
}

// 结构与类型根由方法owner登记（与任务因素特征同口径）；已有登记只核验不重建。定义按“学习种类×改变的特征”全量登记。
void 新_方法类::确保方法状态治理结构已加锁() {
    std::vector<稳定编码*> 键{&方法状态治理根类型, &状态变化定义类型, &状态变化定义成员字段,
        &定义学习种类字段, &定义特征种类字段, &定义角色字段, &状态变化角色类型, &角色所属定义字段,
        &角色规格字段, &学习记录类型, &学习记录成员字段, &记录方法字段, &记录虚拟存在字段,
        &记录启用特征字段, &记录学习种类字段, &记录特征种类字段, &记录特征字段, &记录前值字段,
        &记录后值字段, &记录前状态字段, &记录时间字段, &记录动作定义字段, &记录主体字段,
        &记录现实场景字段, &记录来源材料字段, &虚拟存在配对数特征字段[0], &虚拟存在配对数特征字段[1]};
    if (!有效(方法状态治理根)) {
        if (std::any_of(键.begin(), 键.end(), [](auto* p) { return 有效(*p); }))
            throw std::runtime_error("partial method state governance schema");
        std::vector<基础组内新节点> 组(键.size() + 1);
        组.back().字段组.push_back({节点类型字段, 基础组内节点引用{0}});
        const auto r = 全局基础数据集.原子建立节点组(组);
        if (r.状态 != 基础结构操作状态::已完成 || r.节点组.size() != 组.size())
            throw std::runtime_error("method state governance schema publication failed");
        for (std::size_t i = 0; i < 键.size(); ++i) *键[i] = r.节点组[i];
        方法状态治理根 = r.节点组.back();
    }
    std::set<稳定编码> 唯一键;
    for (auto* p : 键) {
        if (!节点仍存在(*p) || !唯一键.insert(*p).second)
            throw std::runtime_error("method state governance schema key damaged");
        for (const auto* 旧 : 架构编码组())
            if (*旧 == *p) throw std::runtime_error("method state governance key collision");
    }
    if (读取唯一节点字段(方法状态治理根, 节点类型字段) != 方法状态治理根类型
        || 方法状态治理根 == 新方法组织根节点
        || !全局基础数据集.查询目标关系(方法状态治理根, 基础外部关系类型::父子).empty())
        throw std::runtime_error("method state governance root ownership damaged");
    特征概念定义 期望;
    期望.材料类型 = 特征概念材料物理类型::I64标量;
    期望.值域 = std::vector<特征概念I64闭区间>{{0, (std::numeric_limits<std::int64_t>::max)()}};
    期望.比较规则 = 特征概念比较规则::I64数值;
    期望.聚合规则 = 特征概念聚合规则::不自动聚合;
    for (auto& 类型 : 方法配对数特征类型) {
        if (!有效(类型)) 类型 = 特征概念服务_.建立或取得特征概念(期望, 特征值服务_);
        const auto 概念 = 有效(类型) ? 特征概念服务_.获取特征概念(类型) : std::nullopt;
        if (!概念 || 概念->材料类型 != 期望.材料类型 || 概念->值域 != 期望.值域 || 概念->单位
            || 概念->比较规则 != 期望.比较规则 || 概念->聚合规则 != 期望.聚合规则)
            throw std::runtime_error("method pair-count feature type invalid");
    }
    if (方法配对数特征类型[0] == 方法配对数特征类型[1]
        || 方法配对数特征类型[0] == 方法启用特征类型 || 方法配对数特征类型[1] == 方法启用特征类型)
        throw std::runtime_error("method state feature types share a type root");
    const auto 读取索引 = [&] {
        std::map<std::pair<std::int64_t, std::int64_t>, 稳定编码> 索引;
        for (const auto& 字段 : 全局基础数据集.查询字段(方法状态治理根, 状态变化定义成员字段)) {
            const auto* n = std::get_if<稳定编码>(&字段.内容);
            const auto d = n ? 读取方法状态变化定义(*n) : std::nullopt;
            if (!d || !索引.emplace(std::make_pair(static_cast<std::int64_t>(d->学习种类),
                    static_cast<std::int64_t>(d->特征种类)), *n).second)
                throw std::runtime_error("method state change definition damaged or duplicate");
        }
        return 索引;
    };
    auto 索引 = 读取索引();
    基础节点组追加请求 请求;
    for (std::int64_t 种类 = 1; 种类 <= static_cast<std::int64_t>(新方法学习种类::规格修订删除); ++种类)
        for (std::int64_t 特征 = 1; 特征 <= static_cast<std::int64_t>(新方法状态特征种类::已知结果配对数); ++特征) {
            if (索引.contains({种类, 特征})) continue;
            const auto 规格 = 特征 == static_cast<std::int64_t>(新方法状态特征种类::启用) ? 方法启用特征类型
                : 方法配对数特征类型[配对数序号(static_cast<新方法状态特征种类>(特征))];
            const auto 起点 = 请求.新节点组.size();
            基础组内新节点 定义, 角色;
            定义.字段组 = {{节点类型字段, 状态变化定义类型}, {定义学习种类字段, 基础值{种类}},
                {定义特征种类字段, 基础值{特征}}, {定义角色字段, 基础组内节点引用{起点 + 1}}};
            角色.字段组 = {{节点类型字段, 状态变化角色类型}, {角色所属定义字段, 基础组内节点引用{起点}},
                {角色规格字段, 规格}};
            请求.新节点组.push_back(std::move(定义));
            请求.新节点组.push_back(std::move(角色));
            请求.追加字段组.push_back({方法状态治理根, 状态变化定义成员字段, 基础组内节点引用{起点}});
        }
    if (!请求.新节点组.empty()) {
        请求.预期联合节点组.push_back(读取方法状态治理根快照());
        const auto 发布 = 全局基础数据集.原子追加节点组(请求);
        if (发布.状态 != 基础节点组追加状态::已完成 || 发布.新节点组.size() != 请求.新节点组.size())
            throw std::runtime_error("method state change definitions publication failed");
        索引 = 读取索引();
    }
    // 5种学习×3种特征（R119追加规格修订删除）。
    if (索引.size() != 15) throw std::runtime_error("method state change definitions incomplete");
    方法状态变化定义索引 = std::move(索引);
}

std::optional<新方法状态变化定义> 新_方法类::读取方法状态变化定义(稳定编码 n) const noexcept {
    try {
        std::lock_guard 锁(新方法互斥);
        if (!有效(状态变化定义类型) || !有效(状态变化定义成员字段) || !有效(n)) return std::nullopt;
        const auto 归属 = 全局基础数据集.查询目标字段(n, 状态变化定义成员字段);
        if (归属.empty() && !节点类型为(n, 状态变化定义类型)) return std::nullopt;
        if (!节点类型为(n, 状态变化定义类型) || 归属.size() != 1 || 归属.front().所属节点 != 方法状态治理根)
            throw std::runtime_error("method state change definition ownership damaged");
        const auto 种类 = 读取唯一I64字段(n, 定义学习种类字段);
        const auto 特征 = 读取唯一I64字段(n, 定义特征种类字段);
        const auto 角色 = 读取唯一节点字段(n, 定义角色字段);
        if (!种类 || !特征 || !方法学习种类合法(*种类) || !方法状态特征种类合法(*特征) || !角色
            || !节点类型为(*角色, 状态变化角色类型) || 读取唯一节点字段(*角色, 角色所属定义字段) != n)
            throw std::runtime_error("method state change definition fields damaged");
        const auto 特征种类 = static_cast<新方法状态特征种类>(*特征);
        const auto 规格 = 特征种类 == 新方法状态特征种类::启用 ? 方法启用特征类型
            : 方法配对数特征类型[配对数序号(特征种类)];
        const auto 持有 = 全局基础数据集.查询目标字段(*角色, 定义角色字段);
        if (!有效(规格) || 读取唯一节点字段(*角色, 角色规格字段) != 规格
            || 持有.size() != 1 || 持有.front().所属节点 != n)
            throw std::runtime_error("method state change definition role damaged");
        return 新方法状态变化定义{n, 规格, *角色, static_cast<新方法学习种类>(*种类), 特征种类};
    } catch (const std::exception& e) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取方法状态变化定义", L"定义结构损坏，保留现场。", e.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取方法状态变化定义", L"未知工程读取异常。");
    }
}

std::optional<新方法学习记录> 新_方法类::读取方法学习记录(稳定编码 n) const noexcept {
    try {
        std::lock_guard 锁(新方法互斥);
        if (!有效(学习记录类型) || !有效(学习记录成员字段) || !有效(n)) return std::nullopt;
        const auto 归属 = 全局基础数据集.查询目标字段(n, 学习记录成员字段);
        if (归属.empty() && !节点类型为(n, 学习记录类型)) return std::nullopt;
        if (!节点类型为(n, 学习记录类型) || 归属.size() != 1 || 归属.front().所属节点 != 方法状态治理根)
            throw std::runtime_error("method learning record ownership damaged");
        新方法学习记录 r;
        r.节点 = n;
        const auto M = 读取唯一节点字段(n, 记录方法字段), Vm = 读取唯一节点字段(n, 记录虚拟存在字段),
            Fe = 读取唯一节点字段(n, 记录启用特征字段);
        if (!M || !Vm || !Fe) throw std::runtime_error("method learning record target missing");
        r.方法引用 = {*M, *Vm, *Fe};
        const auto 方法 = 读取方法虚拟存在引用(r.方法引用);
        if (!方法) throw std::runtime_error("method learning record target no longer owned");
        const auto 种类 = 读取唯一I64字段(n, 记录学习种类字段), 特征种类 = 读取唯一I64字段(n, 记录特征种类字段);
        if (!种类 || !特征种类 || !方法学习种类合法(*种类) || !方法状态特征种类合法(*特征种类))
            throw std::runtime_error("method learning record kind damaged");
        r.学习种类 = static_cast<新方法学习种类>(*种类);
        r.特征种类 = static_cast<新方法状态特征种类>(*特征种类);
        const auto F = 读取唯一节点字段(n, 记录特征字段);
        if (!F) throw std::runtime_error("method learning record feature missing");
        r.特征节点 = *F;
        const auto 应有特征 = r.特征种类 == 新方法状态特征种类::启用 ? std::optional<稳定编码>{方法->引用.启用特征节点}
            : r.特征种类 == 新方法状态特征种类::条件结果配对数
                ? (方法->条件结果配对数 ? std::optional<稳定编码>{方法->条件结果配对数->特征节点} : std::nullopt)
                : (方法->已知结果配对数 ? std::optional<稳定编码>{方法->已知结果配对数->特征节点} : std::nullopt);
        const auto 前值 = 读取唯一I64字段(n, 记录前值字段), 后值 = 读取唯一I64字段(n, 记录后值字段);
        const bool 启用 = r.特征种类 == 新方法状态特征种类::启用;
        if (应有特征 != r.特征节点 || !前值 || !后值 || *前值 == *后值
            || (启用 ? ((*前值 != 0 && *前值 != 1) || (*后值 != 0 && *后值 != 1)) : (*前值 < 0 || *后值 < 0))
            || (启用 != (r.学习种类 == 新方法学习种类::启用修改)))
            throw std::runtime_error("method learning record feature/value damaged");
        r.前值 = *前值; r.后值 = *后值;
        const auto 前状态 = 读取唯一节点字段(n, 记录前状态字段), 定义 = 读取唯一节点字段(n, 记录动作定义字段),
            主体 = 读取唯一节点字段(n, 记录主体字段), 场景 = 读取唯一节点字段(n, 记录现实场景字段);
        const auto 时间 = 读取唯一I64字段(n, 记录时间字段);
        if (!前状态 || !定义 || !主体 || !场景 || !时间 || *时间 <= 0)
            throw std::runtime_error("method learning record action fields damaged");
        r.前状态节点 = *前状态; r.动作定义节点 = *定义; r.执行主体节点 = *主体; r.现实来源场景节点 = *场景;
        r.动作时间 = {新状态时间语义::实例绝对UTC, *时间};
        const auto 定义信息 = 读取方法状态变化定义(*定义);
        if (!定义信息 || 定义信息->学习种类 != r.学习种类 || 定义信息->特征种类 != r.特征种类)
            throw std::runtime_error("method learning record definition mismatch");
        const auto 材料 = 全局基础数据集.查询字段(n, 记录来源材料字段);
        if (材料.size() > 1) throw std::runtime_error("method learning record source materials duplicated");
        if (!材料.empty()) {
            const auto* 值 = std::get_if<基础值>(&材料.front().内容);
            const auto* 编码组 = 值 ? std::get_if<std::vector<std::uint64_t>>(&值->材料) : nullptr;
            if (!编码组) throw std::runtime_error("method learning record source materials damaged");
            for (const auto 编码 : *编码组) {
                if (编码 == 0) throw std::runtime_error("method learning record source material empty");
                r.来源材料引用组.push_back(稳定编码{编码});
            }
        }
        return r;
    } catch (const std::exception& e) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取方法学习记录", L"学习记录结构损坏，保留现场。", e.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取方法学习记录", L"未知工程读取异常。");
    }
}

// 缺哪个配对数特征就建立并挂上Vm，初值取当前实际数量；已有则不动（值由统一入口维护）。
void 新_方法类::确保方法配对数特征已加锁(稳定编码 方法节点) {
    if (!有效(方法状态治理根)) throw std::logic_error("method state governance schema not initialized");
    const auto vm = 读取唯一节点字段(方法节点, 方法虚拟存在字段);
    if (!vm) throw std::runtime_error("method virtual existence missing for pair-count features");
    bool 缺 = false;
    for (std::size_t i = 0; i < 2; ++i) {
        const auto 已有 = 全局基础数据集.查询字段(*vm, 虚拟存在配对数特征字段[i]);
        if (已有.size() > 1) throw std::runtime_error("method pair-count feature duplicated");
        缺 = 缺 || 已有.empty();
    }
    if (!缺) return;
    const auto 方法 = 读取方法信息本体(方法节点, 特征概念服务_, *this, 存在概念服务_, false, true);
    if (!方法 || 方法->方法虚拟存在节点 != *vm) throw std::runtime_error("method unreadable for pair-count features");
    const auto 数 = 计算方法配对数(*方法);
    基础节点组追加请求 请求;
    std::vector<稳定编码> 新特征;
    try {
        for (std::size_t i = 0; i < 2; ++i) {
            if (!全局基础数据集.查询字段(*vm, 虚拟存在配对数特征字段[i]).empty()) continue;
            const auto F = 特征服务_.建立特征(方法配对数特征类型[i]);
            if (!有效(F)) throw std::runtime_error("method pair-count feature creation failed");
            新特征.push_back(F);
            const auto 写入 = 特征服务_.添加特征值(F, 新特征准确值{数[i]});
            if (写入.状态 != 新特征写入状态::已完成) throw std::runtime_error("method pair-count initial value failed");
            请求.追加字段组.push_back({*vm, 虚拟存在配对数特征字段[i], F});
        }
        const auto 读 = 全局基础数据集.查询节点组完整结构({*vm});
        if (读.状态 != 基础结构操作状态::已完成 || 读.节点组.size() != 1 || !读.节点组.front().快照)
            throw std::runtime_error("method virtual existence snapshot unavailable");
        请求.预期联合节点组.push_back(*读.节点组.front().快照);
        const auto 发布 = 全局基础数据集.原子追加节点组(请求);
        if (发布.状态 != 基础节点组追加状态::已完成) throw std::runtime_error("method pair-count feature attachment failed");
    } catch (...) {
        // 只撤销确认未挂上Vm的候选特征；本函数在方法owner锁内，只有本owner写Vm。
        for (const auto F : 新特征) {
            bool 已挂 = false;
            for (std::size_t i = 0; i < 2; ++i)
                if (!全局基础数据集.查询目标字段(F, 虚拟存在配对数特征字段[i]).empty()) 已挂 = true;
            if (!已挂) (void)特征服务_.删除特征(F);
        }
        throw;
    }
}

// 先天被动配对（本能登记的初始知识）后按实际数量校准；不形成状态、动作或因果。
void 新_方法类::校准方法配对数特征已加锁(稳定编码 方法节点) {
    if (!有效(方法状态治理根)) return; // 启动补齐尚未运行：届时按实际数量建立。
    确保方法配对数特征已加锁(方法节点);
    const auto 方法 = 读取方法信息本体(方法节点, 特征概念服务_, *this, 存在概念服务_, false, true);
    if (!方法) throw std::runtime_error("method unreadable for pair-count calibration");
    const auto 读回 = 读取方法虚拟存在引用({方法节点, 方法->方法虚拟存在节点, 方法->启用特征节点});
    if (!读回 || !读回->条件结果配对数 || !读回->已知结果配对数)
        throw std::runtime_error("method pair-count features missing after ensure");
    const auto 数 = 计算方法配对数(*方法);
    for (std::size_t i = 0; i < 2; ++i) {
        const auto& f = i == 0 ? *读回->条件结果配对数 : *读回->已知结果配对数;
        if (f.当前值 == 数[i]) continue;
        const auto 写入 = 特征服务_.添加特征值(f.特征节点, 新特征准确值{数[i]});
        if (写入.状态 != 新特征写入状态::已完成) throw std::runtime_error("method pair-count calibration failed");
    }
}

std::pair<稳定编码, 稳定编码> 新_方法类::读取治理执行主体已加锁() const {
    if (!治理执行主体读取_) throw std::logic_error("method governance actor reader not assembled");
    const auto 自我 = 治理执行主体读取_();
    if (!自我) throw std::runtime_error("method governance actor (self) unavailable");
    const auto 位置 = 场景服务_.查询所属场景(*自我);
    if (!存在服务_.获取存在(*自我) || !位置 || !场景服务_.存在属于场景范围(*位置, *自我))
        throw std::runtime_error("method governance actor has no real source scene");
    return {*自我, *位置};
}

稳定编码 新_方法类::发布方法学习记录已加锁(const 新方法虚拟存在引用& 引用, 新方法学习种类 种类,
    新方法状态特征种类 特征种类, 稳定编码 F, std::int64_t 前值, std::int64_t 后值,
    稳定编码 前状态, std::int64_t 动作时间, const std::vector<稳定编码>& 来源材料) {
    const auto 定义 = 方法状态变化定义索引.find({static_cast<std::int64_t>(种类), static_cast<std::int64_t>(特征种类)});
    if (定义 == 方法状态变化定义索引.end()) throw std::logic_error("method state change definition not registered");
    const auto [主体, 场景] = 读取治理执行主体已加锁();
    基础节点组追加请求 请求;
    基础组内新节点 记录;
    记录.字段组 = {{节点类型字段, 学习记录类型}, {记录方法字段, 引用.方法节点},
        {记录虚拟存在字段, 引用.虚拟存在节点}, {记录启用特征字段, 引用.启用特征节点},
        {记录学习种类字段, 基础值{static_cast<std::int64_t>(种类)}},
        {记录特征种类字段, 基础值{static_cast<std::int64_t>(特征种类)}}, {记录特征字段, F},
        {记录前值字段, 基础值{前值}}, {记录后值字段, 基础值{后值}}, {记录前状态字段, 前状态},
        {记录时间字段, 基础值{动作时间}}, {记录动作定义字段, 定义->second},
        {记录主体字段, 主体}, {记录现实场景字段, 场景}};
    if (!来源材料.empty()) {
        std::vector<std::uint64_t> 编码组;
        for (const auto n : 来源材料) {
            if (!有效(n)) throw std::logic_error("method learning record source material invalid");
            编码组.push_back(n.值);
        }
        记录.字段组.push_back({记录来源材料字段, 基础值{std::move(编码组)}});
    }
    请求.新节点组.push_back(std::move(记录));
    请求.追加字段组.push_back({方法状态治理根, 学习记录成员字段, 基础组内节点引用{0}});
    请求.预期联合节点组.push_back(读取方法状态治理根快照());
    const auto 发布 = 全局基础数据集.原子追加节点组(请求);
    if (发布.状态 != 基础节点组追加状态::已完成 || 发布.新节点组.size() != 1)
        throw std::runtime_error("method learning record publication failed; status="
            + std::to_string(static_cast<int>(发布.状态)));
    const auto 读回 = 读取方法学习记录(发布.新节点组.front());
    if (!读回 || 读回->方法引用 != 引用 || 读回->特征节点 != F || 读回->前值 != 前值
        || 读回->后值 != 后值 || 读回->前状态节点 != 前状态 || 读回->动作时间.纳秒 != 动作时间)
        throw std::runtime_error("method learning record readback mismatch");
    return 发布.新节点组.front();
}

// 统一入口：学习记录（动作来源）→治理动作（主体自我）→提交特征（提交为空表示调用方已在动作时间后提交）
// →后状态→完成（挂后状态、原子动态、“导致变化”，交付实际发生）。前状态在动作时间之前取得。
void 新_方法类::形成方法状态变化已加锁(const 新方法虚拟存在引用& 引用, 新方法学习种类 种类,
    新方法状态特征种类 特征种类, 稳定编码 F, std::int64_t 前值, std::int64_t 后值,
    稳定编码 前状态, std::int64_t 动作时间, const std::function<void()>& 提交,
    const std::vector<稳定编码>& 来源材料) {
    if (!内部治理世界_ || !治理组织已就绪_ || !有效(方法状态治理根))
        throw std::logic_error("method state change before governance is ready");
    if (前值 == 后值) throw std::logic_error("method state change without value change");
    const auto 记录 = 发布方法学习记录已加锁(引用, 种类, 特征种类, F, 前值, 后值, 前状态, 动作时间, 来源材料);
    const auto 动作 = 内部治理世界_->建立方法状态变化动作(记录);
    if (动作.状态 != 新治理动作登记状态::已建立 || !动作.动作节点)
        throw std::runtime_error("method state change governance action failed");
    if (提交) 提交();
    auto 后时间 = 方法当前UTC纳秒();
    while (后时间 <= 动作时间) { std::this_thread::yield(); 后时间 = 方法当前UTC纳秒(); }
    const auto 后 = 状态服务_.建立状态(引用.虚拟存在节点, F, {新状态时间语义::实例绝对UTC, 后时间});
    if (后.状态 != 新状态操作状态::已完成 || !后.状态节点)
        throw std::runtime_error("method state change after state failed");
    const auto 完成 = 内部治理世界_->完成方法状态变化(记录, *后.状态节点);
    const auto 包 = 内部治理世界_->读取方法状态变化(记录);
    if (!包 || 完成.记录节点 != 记录 || 完成.动作节点 != *动作.动作节点
        || 包->动作节点 != 完成.动作节点 || 包->后状态节点 != *后.状态节点
        || 包->变化动态节点 != 完成.变化动态节点)
        throw std::runtime_error("method state change package readback mismatch");
    待发方法状态通知_.push_back(引用.虚拟存在节点);
}

// 学习发布后：配对数或已知结果配对数确实变化时才形成状态；值未变不建动作、不交付因果。
void 新_方法类::同步方法配对数状态已加锁(稳定编码 方法节点, 新方法学习种类 种类,
    const std::vector<稳定编码>& 来源材料) {
    确保方法配对数特征已加锁(方法节点);
    const auto 方法 = 读取方法信息本体(方法节点, 特征概念服务_, *this, 存在概念服务_, false, true);
    if (!方法) throw std::runtime_error("learned method unreadable");
    const 新方法虚拟存在引用 引用{方法节点, 方法->方法虚拟存在节点, 方法->启用特征节点};
    const auto 数 = 计算方法配对数(*方法);
    for (std::size_t i = 0; i < 2; ++i) {
        const auto 读回 = 读取方法虚拟存在引用(引用);
        if (!读回 || !读回->条件结果配对数 || !读回->已知结果配对数)
            throw std::runtime_error("learned method pair-count features missing");
        const auto f = i == 0 ? *读回->条件结果配对数 : *读回->已知结果配对数;
        if (f.当前值 == 数[i]) continue;
        auto 前时间 = 方法当前UTC纳秒();
        const auto 前 = 状态服务_.建立状态(引用.虚拟存在节点, f.特征节点, {新状态时间语义::实例绝对UTC, 前时间});
        if (前.状态 != 新状态操作状态::已完成 || !前.状态节点)
            throw std::runtime_error("method pair-count before state failed");
        const auto 动作时间 = 方法当前UTC纳秒();
        const auto 后值 = 数[i];
        const auto 提交 = [&, F = f.特征节点] {
            const auto 写入 = 特征服务_.添加特征值(F, 新特征准确值{后值});
            if (写入.状态 != 新特征写入状态::已完成 || !写入.当前值已改变)
                throw std::runtime_error("method pair-count feature commit failed");
        };
        形成方法状态变化已加锁(引用, 种类, i == 0 ? 新方法状态特征种类::条件结果配对数
            : 新方法状态特征种类::已知结果配对数, f.特征节点, f.当前值, 后值, *前.状态节点,
            动作时间, 提交, 来源材料);
    }
}

void 新_方法类::学习前确保配对数特征(稳定编码 方法节点) noexcept {
    try {
        std::lock_guard 锁(新方法互斥);
        if (!有效(方法状态治理根) || !有效(方法节点) || !节点类型为(方法节点, 方法节点类型)) return;
        确保方法配对数特征已加锁(方法节点);
    } catch (const std::exception& e) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"学习前确保配对数特征", L"配对数特征补齐工程故障。", e.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"学习前确保配对数特征", L"未知工程故障。");
    }
}

void 新_方法类::学习后形成方法状态(稳定编码 方法节点, 新方法学习种类 种类,
    const std::vector<稳定编码>& 来源材料) noexcept {
    try {
        std::lock_guard 锁(新方法互斥);
        同步方法配对数状态已加锁(方法节点, 种类, 来源材料);
    } catch (const std::exception& e) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"学习后形成方法状态",
            L"配对已发布后方法状态、治理动作或因果交付工程故障；保留已发生事实，不重做。", e.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"学习后形成方法状态", L"未知工程故障；保留事实。");
    }
}

void 新_方法类::发出方法状态通知() noexcept {
    std::vector<稳定编码> 组;
    std::function<void(稳定编码)> 通知;
    try {
        std::lock_guard 锁(新方法互斥);
        组.swap(待发方法状态通知_);
        通知 = 方法状态就绪通知_;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"发出方法状态通知", L"通知队列读取异常。");
    }
    if (!通知) return;
    for (const auto n : 组) 通知(n);
}
}
