#include "新_方法类.h"

#include <algorithm>
#include <chrono>
#include <limits>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <stdexcept>
#include <tuple>
#include <utility>

#include "新_存在类.h"
#include "新_场景类.h"
#include "新_特征类.h"
#include "新_状态类.h"
#include "新_本能函数集.h"
#include "新_本能函数管理类.h"
#include "概念_存在类.h"
#include "概念_特征类.h"
#include "../核心/容错检查.h"

namespace 海中鱼巣 {

稳定编码 新方法组织根节点{};

namespace {

std::recursive_mutex 新方法互斥;

bool 同一学习条件(const 新方法条件定义& 左, const 新方法条件定义& 右) {
    if (左.关系规则 != 右.关系规则) return false;
    if (左.关系规则 == 新方法项关系规则::全部成立
        || 左.关系规则 == 新方法项关系规则::任一成立) {
        return std::is_permutation(左.条件项组.begin(), 左.条件项组.end(),
            右.条件项组.begin(), 右.条件项组.end());
    }
    return 左 == 右;
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
稳定编码 方法行为字段{};
稳定编码 虚拟存在启用特征字段{};
稳定编码 虚拟存在专属概念字段{};
稳定编码 方法启用特征类型{};

稳定编码 方法头作用存在概念字段{};
稳定编码 方法头特征类型概念字段{};
稳定编码 方法头结果概念字段{};

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

稳定编码 配对条件字段{};
稳定编码 配对结果字段{};
稳定编码 配对命中次数字段{};
稳定编码 配对线性关系组字段{};
稳定编码 配对来源字段{};
稳定编码 关系规则字段{};
稳定编码 条件项字段{};
稳定编码 条件项存在概念字段{};
稳定编码 条件项特征类型字段{};
稳定编码 条件项特征值域字段{};
稳定编码 条件项二次特征字段{};
稳定编码 条件项状态动态字段{};
稳定编码 结果项字段{};
稳定编码 结果项特征类型字段{};
稳定编码 结果项特征值域字段{};
稳定编码 结果项变化方向字段{};
稳定编码 结果项状态动态字段{};
稳定编码 结果项主要结果字段{};
稳定编码 结果必要证据质量字段{};
稳定编码 结果最大等待纳秒字段{};
稳定编码 结果等待条件概念字段{};
稳定编码 结果多证据关系字段{};
稳定编码 结果禁止关系规则字段{};
稳定编码 结果禁止项字段{};
稳定编码 项精确数值域字段{};
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

bool 行为声明有效(const 新方法知识建立请求& 请求) {
    return (!请求.行为 || 是方法行为(static_cast<std::int64_t>(*请求.行为)))
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

// 外层为空表示结构损坏；内层为空才表示旧项没有私有域。
std::optional<std::optional<特征概念I64闭区间>> 读取可选精确数值域(
    稳定编码 项节点) {
    const auto 组 = 全局基础数据集.查询字段(项节点, 项精确数值域字段);
    if (组.empty()) return std::optional<特征概念I64闭区间>{};
    if (组.size() != 1) return std::nullopt;
    const auto* 值 = std::get_if<基础值>(&组.front().内容);
    const auto* 端点 = 值 ? std::get_if<std::vector<std::int64_t>>(&值->材料) : nullptr;
    if (!端点 || 端点->size() != 2 || (*端点)[0] > (*端点)[1]
        || (*端点)[0] == (std::numeric_limits<std::int64_t>::min)()
        || (*端点)[1] == (std::numeric_limits<std::int64_t>::max)()) return std::nullopt;
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

bool 参数规格有效(const std::vector<新方法参数规格>& 规格组,
    const 概念_特征类& 概念服务, bool 完整组 = true) {
    std::set<稳定编码> 角色组;
    for (const auto& 规格 : 规格组) {
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
    if (概念域 || !类型 || 域->下界 > 域->上界
        || 域->下界 == (std::numeric_limits<std::int64_t>::min)()
        || 域->上界 == (std::numeric_limits<std::int64_t>::max)()) return false;
    const auto 特征概念 = 概念服务.获取特征概念(*类型);
    return 特征概念 && 特征概念->材料类型 == 特征概念材料物理类型::I64标量;
}

bool 结果项有效(const 新方法结果项定义& 项, const 概念_特征类& 概念服务) noexcept {
    return 节点仍存在(项.参数角色节点)
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
        const auto& 结果 = 定义.结果.结果项组[关系.结果序号];
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
    bool 实例 = false) {
    if (!线性关系有效(定义, 概念服务, 实例)) return false;
    if (!关系定义有效(
            定义.条件.关系规则, 定义.条件.条件项组.size())
        || !关系定义有效(
            定义.结果.关系规则, 定义.结果.结果项组.size())) return false;
    for (const auto& 项 : 定义.条件.条件项组) {
        if (!节点仍存在(项.参数角色节点)
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

bool 条件结果角色已声明(
    const std::vector<新方法参数规格>& 参数规格组,
    const 新方法条件结果定义& 定义) noexcept {
    const auto 已声明 = [&](稳定编码 角色) {
        return std::any_of(参数规格组.begin(), 参数规格组.end(),
            [&](const 新方法参数规格& 规格) {
                return 规格.参数角色节点 == 角色;
            });
    };
    return std::all_of(定义.条件.条件项组.begin(),
               定义.条件.条件项组.end(),
               [&](const 新方法条件项定义& 项) {
                   return 已声明(项.参数角色节点);
               })
        && std::all_of(定义.结果.结果项组.begin(),
               定义.结果.结果项组.end(),
               [&](const 新方法结果项定义& 项) {
                   return 已声明(项.参数角色节点);
               })
        && std::all_of(定义.结果.确认要求.禁止结果项组.begin(),
               定义.结果.确认要求.禁止结果项组.end(),
               [&](const 新方法结果项定义& 项) {
                   return 已声明(项.参数角色节点);
               });
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

std::optional<新方法参数规格> 读取参数规格(稳定编码 节点, const 概念_特征类& 概念服务) {
    if (!节点类型为(节点, 参数规格节点类型)) return std::nullopt;
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
    for (const auto 项节点 : *项节点组) {
        if (!节点类型为(项节点, 条件项节点类型)) return std::nullopt;
        const auto 角色 = 读取唯一节点字段(项节点, 参数角色字段);
        const auto 存在 = 读取可选节点字段(项节点, 条件项存在概念字段);
        const auto 类型 = 读取可选节点字段(项节点, 条件项特征类型字段);
        const auto 值域 = 读取可选节点字段(项节点, 条件项特征值域字段);
        const auto 二次 = 读取可选节点字段(项节点, 条件项二次特征字段);
        const auto 状态动态 = 读取可选节点字段(项节点, 条件项状态动态字段);
        const auto 精确域 = 读取可选精确数值域(项节点);
        if (!角色 || !存在 || !类型 || !值域 || !二次 || !状态动态 || !精确域) {
            return std::nullopt;
        }
        新方法条件项定义 项;
        项.参数角色节点 = *角色;
        if (有效(*存在)) 项.存在概念节点 = *存在;
        if (有效(*类型)) 项.特征类型概念节点 = *类型;
        if (有效(*值域)) 项.特征值域概念节点 = *值域;
        if (有效(*二次)) 项.二次特征概念节点 = *二次;
        if (有效(*状态动态)) 项.状态或动态概念节点 = *状态动态;
        项.精确数值域 = *精确域;
        if (!精确数值域有效(项.精确数值域, 项.特征类型概念节点,
                项.特征值域概念节点, 概念服务)) return std::nullopt;
        结果.条件项组.push_back(std::move(项));
    }
    return 结果;
}

std::optional<新方法结果项定义> 读取结果项(稳定编码 项节点, const 概念_特征类& 概念服务) {
        if (!节点类型为(项节点, 结果项节点类型)) return std::nullopt;
        const auto 角色 = 读取唯一节点字段(项节点, 参数角色字段);
        const auto 类型 = 读取可选节点字段(项节点, 结果项特征类型字段);
        const auto 值域 = 读取可选节点字段(项节点, 结果项特征值域字段);
        const auto 方向 = 读取可选节点字段(项节点, 结果项变化方向字段);
        const auto 状态动态 = 读取可选节点字段(项节点, 结果项状态动态字段);
        const auto 主要 = 读取唯一I64字段(项节点, 结果项主要结果字段);
        const auto 精确域 = 读取可选精确数值域(项节点);
        if (!角色 || !类型 || !值域 || !方向 || !状态动态 || !主要 || !精确域
            || (*主要 != 0 && *主要 != 1)) return std::nullopt;
        新方法结果项定义 项;
        项.参数角色节点 = *角色;
        if (有效(*类型)) 项.特征类型概念节点 = *类型;
        if (有效(*值域)) 项.特征值域概念节点 = *值域;
        if (有效(*方向)) 项.变化方向概念节点 = *方向;
        if (有效(*状态动态)) 项.状态或动态概念节点 = *状态动态;
        项.主要结果 = *主要 != 0;
        项.精确数值域 = *精确域;
        if (!精确数值域有效(项.精确数值域, 项.特征类型概念节点,
                项.特征值域概念节点, 概念服务)) return std::nullopt;
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
std::optional<新方法混合实例信息> 读取实例根结构(稳定编码 实例根, const 概念_特征类& 概念服务) {
    if (!结构仍存在()) return std::nullopt;
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
        子(虚拟存在参数规格字段, 参数规格节点类型);
    } else if (类型 == 参数规格节点类型) {
        外部({参数角色字段, 参数存在概念字段, 参数集合角色字段, 参数集合特征类型字段});
        值({参数要求场景字段, 参数对象种类字段, 顺序字段});
        子(参数特征规格字段, 参数特征规格节点类型);
    } else if (类型 == 参数特征规格节点类型) {
        外部({参数特征类型字段, 参数特征值域字段, 参数二次特征概念字段});
        值({顺序字段});
    } else if (类型 == 条件结果配对节点类型) {
        外部({配对来源字段});
        子(配对条件字段, 条件节点类型);
        子(配对结果字段, 结果节点类型);
        值({顺序字段, 配对命中次数字段, 配对线性关系组字段});
    } else if (类型 == 条件节点类型) {
        子(条件项字段, 条件项节点类型);
        值({关系规则字段});
    } else if (类型 == 结果节点类型) {
        子(结果项字段, 结果项节点类型);
        子(结果禁止项字段, 结果项节点类型);
        外部({结果等待条件概念字段});
        值({关系规则字段, 结果必要证据质量字段, 结果最大等待纳秒字段,
            结果多证据关系字段, 结果禁止关系规则字段});
    } else if (类型 == 条件项节点类型) {
        外部({参数角色字段, 条件项存在概念字段, 条件项特征类型字段,
            条件项特征值域字段, 条件项二次特征字段, 条件项状态动态字段});
        值({顺序字段, 项精确数值域字段});
    } else if (类型 == 结果项节点类型) {
        外部({参数角色字段, 结果项特征类型字段, 结果项特征值域字段,
            结果项变化方向字段, 结果项状态动态字段});
        值({顺序字段, 结果项主要结果字段, 项精确数值域字段});
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

std::optional<新方法信息> 读取方法信息(稳定编码 方法节点, const 概念_特征类& 概念服务,
    const 新方法治理对象读取接口& 治理读取, bool 只读旧形状 = false) {
    if (!结构仍存在() || !节点类型为(方法节点, 方法节点类型)
        || !方法属于根(方法节点)) return std::nullopt;
    const auto 作用域 = 读取唯一I64字段(方法节点, 方法作用域字段);
    const auto 实现方式 = 读取唯一I64字段(方法节点, 方法实现方式字段);
    std::optional<std::int64_t> 启用;
    const auto 头节点 = 读取唯一节点字段(方法节点, 方法头字段);
    const auto 虚拟存在 = 读取唯一节点字段(方法节点, 方法虚拟存在字段);
    std::optional<新方法虚拟存在读回> 治理;
    if (只读旧形状) {
        if (!虚拟存在 || !全局基础数据集.查询字段(*虚拟存在, 虚拟存在启用特征字段).empty()
            || !全局基础数据集.查询字段(*虚拟存在, 虚拟存在专属概念字段).empty()) return std::nullopt;
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
        return std::nullopt;
    }
    const auto 虚拟所属 = 读取唯一节点字段(
        *虚拟存在, 虚拟存在所属方法字段);
    const auto 参数节点组 = 读取有序子节点(
        *虚拟存在, 虚拟存在参数规格字段, 概念服务);
    if (!虚拟所属 || *虚拟所属 != 方法节点 || !参数节点组) {
        return std::nullopt;
    }

    新方法信息 信息;
    信息.节点 = 方法节点;
    if (有效(方法行为字段)) {
        const auto 字段组 = 全局基础数据集.查询字段(方法节点, 方法行为字段);
        if (字段组.size() > 1) return std::nullopt;
        if (!字段组.empty()) {
            const auto* 基础 = std::get_if<基础值>(&字段组.front().内容);
            const auto* 值 = 基础 ? std::get_if<std::int64_t>(&基础->材料) : nullptr;
            if (!值 || !是方法行为(*值)) return std::nullopt;
            信息.行为 = static_cast<新方法行为>(*值);
        }
    }
    if (有效(方法父代字段)) {
        const auto 父代 = 读取可选节点字段(方法节点, 方法父代字段);
        if (!父代) return std::nullopt;
        if (有效(*父代)) {
            if (*父代 == 方法节点 || !节点类型为(*父代, 方法节点类型)
                || !方法属于根(*父代)
                || !全局基础数据集.查询字段(*父代, 方法父代字段).empty()
                || !全局基础数据集.查询字段(*父代, 方法来源字段).empty()
                || !全局基础数据集.查询字段(*父代, 方法实例根字段).empty()) return std::nullopt;
            信息.父代方法节点 = *父代;
        }
    }
    const auto 来源 = 读取可选节点字段(方法节点, 方法来源字段);
    const auto 实例根 = 读取可选节点字段(方法节点, 方法实例根字段);
    if (!来源 || !实例根 || 有效(*来源) != 有效(*实例根)) return std::nullopt;
    if (有效(*来源)) {
        if (!节点类型为(*来源, 方法节点类型) || !方法属于根(*来源)
            || !全局基础数据集.查询字段(*来源, 方法来源字段).empty()
            || !全局基础数据集.查询字段(*来源, 方法实例根字段).empty()) {
            return std::nullopt;
        }
        if (!读取实例根结构(*实例根, 概念服务)) return std::nullopt;
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
    if (!头存在 || !头特征 || !头结果) return std::nullopt;
    信息.方法头 = {*头存在, *头特征, *头结果};
    for (const auto 参数节点 : *参数节点组) {
        const auto 参数 = 读取参数规格(参数节点, 概念服务);
        if (!参数) return std::nullopt;
        信息.参数规格组.push_back(*参数);
    }
    if (!参数规格有效(信息.参数规格组, 概念服务)) return std::nullopt;
    std::set<稳定编码> 已见来源配对;
    for (const auto 配对节点 : *配对节点组) {
        if (!节点类型为(配对节点, 条件结果配对节点类型)) return std::nullopt;
        const auto 条件节点 = 读取唯一节点字段(配对节点, 配对条件字段);
        const auto 结果节点 = 读取唯一节点字段(配对节点, 配对结果字段);
        const auto 命中 = 读取可选I64字段(配对节点, 配对命中次数字段);
        const auto 配对来源 = 读取可选节点字段(配对节点, 配对来源字段);
        if (!条件节点 || !结果节点 || !命中 || (*命中 && **命中 < 0)
            || !配对来源) return std::nullopt;
        if (有效(*配对来源)) {
            if (!已见来源配对.insert(*配对来源).second) return std::nullopt;
            const auto 持有 = 全局基础数据集.查询目标字段(*配对来源, 方法条件结果字段);
            if (!信息.来源方法节点 || !节点类型为(*配对来源, 条件结果配对节点类型)
                || 持有.size() != 1 || 持有.front().所属节点 != *信息.来源方法节点
                || !全局基础数据集.查询字段(*配对来源, 配对来源字段).empty()) return std::nullopt;
        }
        const auto 条件 = 读取条件(*条件节点, 概念服务);
        const auto 结果 = 读取结果(*结果节点, 概念服务);
        const auto 线性 = 读取线性关系组(配对节点);
        if (!条件 || !结果 || !线性
            || !线性关系有效({*条件, *结果, *线性}, 概念服务, 信息.实例根节点.has_value())) return std::nullopt;
        信息.条件结果组.push_back({
            配对节点, *条件节点, *结果节点, *条件, *结果, 命中->value_or(0), *线性,
            有效(*配对来源) ? std::optional<稳定编码>{*配对来源} : std::nullopt});
    }
    const auto 函数组 = 全局基础数据集.查询字段(
        方法节点, 方法本能函数登记名字段);
    if (信息.实现方式 == 新方法实现方式::本能) {
        if (函数组.size() > 1) return std::nullopt;
        if (函数组.size() == 1) {
            const auto* 值 = std::get_if<基础值>(&函数组.front().内容);
            const auto* 名称 = 值
                ? std::get_if<std::string>(&值->材料) : nullptr;
            if (!名称 || 名称->empty()) return std::nullopt;
            信息.本能函数登记名 = *名称;
        }
    } else if (!函数组.empty()) {
        return std::nullopt;
    }
    if (信息.父代方法节点) {
        if (信息.本能函数登记名 != 新本能函数登记名_服务值结算
            || 信息.作用域 != 新方法作用域::普通世界
            || 信息.实现方式 != 新方法实现方式::本能
            || !步骤节点组->empty() || !转移节点组->empty() || 有效(*入口步骤)) return std::nullopt;
        if (信息.来源方法节点) {
            const auto 来源父代 = 读取可选节点字段(*信息.来源方法节点, 方法父代字段);
            if (!来源父代 || *来源父代 != *信息.父代方法节点) return std::nullopt;
        }
    }

    for (const auto 步骤节点 : *步骤节点组) {
        if (!节点类型为(步骤节点, 组合步骤节点类型)) return std::nullopt;
        const auto 所属 = 读取唯一节点字段(步骤节点, 步骤所属组合方法字段);
        const auto 调用 = 读取唯一节点字段(步骤节点, 步骤调用方法字段);
        const auto 顺序 = 读取唯一I64字段(步骤节点, 顺序字段);
        if (!所属 || *所属 != 方法节点 || !调用 || !顺序 || *顺序 < 0) {
            return std::nullopt;
        }
        const auto 调用根 = 读取可选节点字段(*调用, 方法实例根字段);
        const auto 调用来源 = 读取可选节点字段(*调用, 方法来源字段);
        if (!节点类型为(*调用, 方法节点类型) || !方法属于根(*调用)
            || !调用根 || !调用来源 || *调用根 != *实例根
            || 有效(*调用根) != 有效(*调用来源)) return std::nullopt;
        信息.组合步骤组.push_back({步骤节点, *调用, *顺序});
    }
    std::sort(信息.组合步骤组.begin(), 信息.组合步骤组.end(),
        [](const 新方法组合步骤信息& 左, const 新方法组合步骤信息& 右) {
            return std::tie(左.顺序, 左.步骤节点)
                < std::tie(右.顺序, 右.步骤节点);
        });
    for (std::size_t i = 1; i < 信息.组合步骤组.size(); ++i) {
        if (信息.组合步骤组[i - 1].顺序 == 信息.组合步骤组[i].顺序) {
            return std::nullopt;
        }
    }
    if (有效(*入口步骤)) {
        const auto 找到 = std::find_if(信息.组合步骤组.begin(),
            信息.组合步骤组.end(), [&](const 新方法组合步骤信息& 步骤) {
                return 步骤.步骤节点 == *入口步骤;
            });
        if (找到 == 信息.组合步骤组.end()) return std::nullopt;
        信息.组合入口步骤节点 = *入口步骤;
    }
    for (const auto 转移节点 : *转移节点组) {
        if (!节点类型为(转移节点, 结果转移节点类型)) return std::nullopt;
        const auto 所属 = 读取唯一节点字段(转移节点, 转移所属组合方法字段);
        const auto 当前步骤 = 读取唯一节点字段(转移节点, 转移当前步骤字段);
        const auto 当前结果 = 读取唯一节点字段(转移节点, 转移当前结果字段);
        const auto 匹配 = 读取可选节点字段(转移节点, 转移匹配结果概念字段);
        const auto 下一步骤 = 读取唯一节点字段(转移节点, 转移下一步骤字段);
        if (!所属 || *所属 != 方法节点 || !当前步骤 || !当前结果
            || !匹配 || !下一步骤) return std::nullopt;
        const auto 当前 = std::find_if(信息.组合步骤组.begin(), 信息.组合步骤组.end(),
            [&](const auto& 步骤) { return 步骤.步骤节点 == *当前步骤; });
        if (当前 == 信息.组合步骤组.end()
            || std::none_of(信息.组合步骤组.begin(), 信息.组合步骤组.end(),
                [&](const auto& 步骤) { return 步骤.步骤节点 == *下一步骤; })) {
            return std::nullopt;
        }
        const auto 调用配对 = 读取有序子节点(当前->调用方法节点, 方法条件结果字段, 概念服务);
        if (!调用配对 || std::none_of(调用配对->begin(), 调用配对->end(),
            [&](稳定编码 配对) {
                return 读取唯一节点字段(配对, 配对结果字段) == 当前结果;
            })) return std::nullopt;
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
            || !信息.结果转移组.empty())) return std::nullopt;
    if (std::any_of(信息.条件结果组.begin(), 信息.条件结果组.end(),
            [&](const auto& 配对) { return !行为结果相容(信息.行为, 配对.结果); }))
        return std::nullopt;
    return 信息;
}

// 调用者已核验原登记声明并持有方法owner锁。未知不推断，唯一追加为发布点。
新方法建立结果 补登记方法行为(const 新方法信息& 原,
    const std::optional<新方法行为>& 声明, const 概念_特征类& 概念,
    const 新方法治理对象读取接口& 治理) {
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
    const auto 当前 = 读取方法信息(原.节点, 概念, 治理);
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
        const auto 读回 = 读取方法信息(原.节点, 概念, 治理);
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
    std::vector<稳定编码>& 已写字段) {
    const auto 参数节点 = 新建类型化节点(
        参数规格节点类型, 已建节点, 已写字段);
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
    const auto 项 = 新建类型化节点(
        结果项节点类型, 已建节点, 已写字段);
    if (!有效(项)
        || !写入I64字段(项, 顺序字段, 顺序, 已写字段)
        || !写入节点字段(项, 参数角色字段, 源.参数角色节点, 已写字段)
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
            || !写入节点字段(项, 参数角色字段, 源.参数角色节点, 已写字段)) return {};
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
        if (!写入节点字段(条件, 条件项字段, 项, 已写字段)) return {};
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

enum class 服务结算声明种类 { 非法, 固定加一, 按贡献Q };

std::optional<特征概念I64闭区间> 结算声明区间(
    const 概念_特征类& 概念, const 新方法参数特征规格& 规格) {
    if (!规格.值域概念节点 || !规格.二次特征概念组.empty()) return {};
    const auto 域 = 概念.获取特征概念(*规格.值域概念节点);
    const auto 比较 = 概念.比较单特征概念(*规格.值域概念节点, 规格.特征类型概念节点);
    if (!域 || 域->材料类型 != 特征概念材料物理类型::I64标量
        || 比较.状态 != 特征概念规则状态::已完成
        || 比较.特征类型根 != 规格.特征类型概念节点 || !比较.关系
        || (*比较.关系 != 单特征概念关系::同一
            && *比较.关系 != 单特征概念关系::右为上位)) return {};
    const auto* 区间 = std::get_if<std::vector<特征概念I64闭区间>>(&域->值域);
    return 区间 && 区间->size() == 1 ? std::optional{区间->front()} : std::nullopt;
}

服务结算声明种类 读取服务结算声明种类(const 新方法头定义& 头,
    const std::vector<新方法参数规格>& 参数, const 概念_特征类& 概念) {
    using 类 = 服务结算声明种类;
    if (参数.size() != 2 || 头.涉及特征类型概念组.size() != 1
        || 头.可能结果概念组.size() != 1) return 类::非法;
    const auto& 自我 = 参数[0]; const auto& 来源 = 参数[1];
    if (自我.对象种类 != 新方法参数对象种类::普通世界 || !自我.要求场景
        || 自我.集合成员约束 || 自我.特征规格组.size() != 2
        || 来源.对象种类 != 新方法参数对象种类::任务虚拟存在 || 来源.要求场景
        || 来源.存在概念节点 || !来源.集合成员约束
        || (来源.特征规格组.size() != 1 && 来源.特征规格组.size() != 2)
        || 自我.参数角色节点 != 头.涉及特征类型概念组.front()
        || 自我.特征规格组[0].特征类型概念节点 != 自我.参数角色节点
        || 来源.参数角色节点 != 来源.特征规格组[0].特征类型概念节点
        || 来源.参数角色节点 == 自我.参数角色节点
        || 来源.集合成员约束->集合参数角色节点 != 自我.参数角色节点
        || 来源.集合成员约束->集合特征类型概念节点 != 自我.特征规格组[1].特征类型概念节点
        || !自我.特征规格组[1].值域概念节点
        || !自我.特征规格组[1].二次特征概念组.empty()) return 类::非法;
    const auto 前 = 结算声明区间(概念, 自我.特征规格组[0]);
    const auto 状态 = 结算声明区间(概念, 来源.特征规格组[0]);
    新方法参数特征规格 结果规格{自我.参数角色节点, 头.可能结果概念组.front(), {}};
    const auto 结果 = 结算声明区间(概念, 结果规格);
    if (!前 || !状态 || *状态 != 特征概念I64闭区间{9, 9} || !结果) return 类::非法;
    const auto 集合 = 概念.获取特征概念(*自我.特征规格组[1].值域概念节点);
    const auto 集合比较 = 概念.比较单特征概念(*自我.特征规格组[1].值域概念节点,
        自我.特征规格组[1].特征类型概念节点);
    特征概念任务集合值域 非空; 非空.至少一个任务 = true;
    if (!集合 || 集合->值域 != 特征概念值域{非空}
        || 集合比较.状态 != 特征概念规则状态::已完成
        || 集合比较.特征类型根 != 自我.特征规格组[1].特征类型概念节点
        || !集合比较.关系 || (*集合比较.关系 != 单特征概念关系::同一
            && *集合比较.关系 != 单特征概念关系::右为上位)) return 类::非法;
    constexpr auto 最大 = (std::numeric_limits<std::int64_t>::max)();
    if (来源.特征规格组.size() == 1) {
        if (自我.存在概念节点 || !头.作用存在概念组.empty()
            || 前->下界 < 0 || 前->下界 != 前->上界 || 前->上界 == 最大
            || *结果 != 特征概念I64闭区间{前->下界 + 1, 前->下界 + 1}) return 类::非法;
        return 类::固定加一;
    }
    const auto Q = 结算声明区间(概念, 来源.特征规格组[1]);
    if (!自我.存在概念节点 || 头.作用存在概念组 != std::vector<稳定编码>{*自我.存在概念节点}
        || !Q || *Q != 特征概念I64闭区间{0, 最大}
        || *前 != 特征概念I64闭区间{0, 最大} || *结果 != *前
        || 来源.特征规格组[1].特征类型概念节点 == 来源.参数角色节点) return 类::非法;
    return 类::按贡献Q;
}

服务结算声明种类 读取服务结算声明种类(const 新方法信息& 方法,
    const 概念_特征类& 概念) {
    if (方法.作用域 != 新方法作用域::普通世界 || 方法.实现方式 != 新方法实现方式::本能
        || 方法.本能函数登记名 != 新本能函数登记名_服务值结算
        || !方法.组合步骤组.empty() || 方法.组合入口步骤节点 || !方法.结果转移组.empty())
        return 服务结算声明种类::非法;
    return 读取服务结算声明种类(方法.方法头, 方法.参数规格组, 概念);
}

// 调用方已经持有方法结构锁。完整扫描，任何破损都不能解释为未登记。
新方法建立结果 查询本能登记结构(const std::string& 登记名, const 概念_特征类& 概念服务,
    const 新方法治理对象读取接口& 治理读取) {
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
        const auto 方法 = 读取方法信息(节点, 概念服务, 治理读取);
        if (!方法) {
            return {新方法操作状态::结构不一致, std::nullopt};
        }
        if (方法->来源方法节点 || 方法->本能函数登记名 != 登记名) continue;
        if (登记名 == 新本能函数登记名_服务值结算) {
            同名.push_back(*方法); continue;
        }
        if (结果.方法节点) {
            return {新方法操作状态::结构不一致, std::nullopt};
        }
        结果.方法节点 = 节点;
    }
    if (登记名 == 新本能函数登记名_服务值结算 && !同名.empty()) {
        const auto 停止 = []() { 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__,
            L"查询服务结算方法登记", L"服务结算方法代际、启用或规格不完整，保留方法知识。"); };
        if (同名.size() > 2) 停止();
        const 新方法信息* 当前 = nullptr; const 新方法信息* 旧 = nullptr;
        for (const auto& m : 同名) {
            if (m.来源方法节点 || m.实例根节点 || 读取服务结算声明种类(m, 概念服务) == 服务结算声明种类::非法) 停止();
            if (m.启用状态 == 新方法启用状态::启用) {
                if (当前) 停止(); 当前 = &m;
            } else { if (旧) 停止(); 旧 = &m; }
        }
        if (!当前) 停止();
        if (旧) {
            if (读取服务结算声明种类(*当前, 概念服务) != 服务结算声明种类::按贡献Q
                || 读取服务结算声明种类(*旧, 概念服务) != 服务结算声明种类::固定加一
                || 当前->父代方法节点 != 旧->节点 || 旧->父代方法节点) 停止();
        } else if (当前->父代方法节点) 停止();
        结果.方法节点 = 当前->节点;
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
    const std::vector<稳定编码>& 图) {
    std::set<稳定编码> 身份{新方法组织根节点};
    身份.insert(图.begin(), 图.end());
    for (const auto& f : 请求.追加字段组) {
        身份.insert(f.所属节点); 身份.insert(f.字段节点);
        if (const auto* n = std::get_if<稳定编码>(&f.内容)) 身份.insert(*n);
    }
    for (const auto n : 图) {
        const auto r = 全局基础数据集.查询节点组完整结构({n});
        if (r.状态 != 基础结构操作状态::已完成 || r.节点组.size() != 1 || !r.节点组[0].快照)
            throw std::runtime_error("method graph snapshot unavailable");
        for (const auto& f : r.节点组[0].快照->结构.字段组) {
            身份.insert(f.字段节点);
            if (const auto* ref = std::get_if<稳定编码>(&f.内容)) 身份.insert(*ref);
        }
    }
    const auto r = 全局基础数据集.查询节点组完整结构({身份.begin(), 身份.end()});
    if (r.状态 != 基础结构操作状态::已完成 || r.节点组.size() != 身份.size())
        throw std::runtime_error("method publication snapshot unavailable");
    for (const auto& n : r.节点组) {
        if (!n.快照) throw std::runtime_error("method publication incomplete snapshot");
        请求.预期联合节点组.push_back(*n.快照);
    }
    auto sealed = 全局基础数据集.封存原子追加片段(请求);
    if (sealed.状态 != 基础节点组追加状态::已完成 || !sealed.片段)
        throw std::runtime_error("method publication seal failed");
    return *sealed.片段;
}

void 追加方法虚拟归属(基础节点组追加请求& 请求, const 新方法虚拟存在读回& v) {
    请求.追加字段组.push_back({v.引用.虚拟存在节点, 虚拟存在启用特征字段, v.引用.启用特征节点});
    请求.追加字段组.push_back({v.引用.虚拟存在节点, 虚拟存在专属概念字段, v.专属存在概念节点});
}

} // namespace

// 共用结构发布器；函数绑定是完整结构的一部分，根成员始终最后发布。
// 只回滚本次未发布材料；已公开后读回失败保留现场向上送出。
新方法建立结果 新_方法类::发布方法结构(
    const 新方法知识建立请求& 请求, const std::string* 登记名,
    const 新方法信息* 替代父代) noexcept {
    std::vector<稳定编码> 已建节点;
    std::vector<稳定编码> 已写字段;
    bool 已发布 = false;
    std::vector<方法虚拟存在准备> 虚拟组;
    新特征I64准备结果 旧更新;
    std::optional<稳定编码> 前状态;
    std::int64_t 前时间 = 0;
    try {
        核验治理组织就绪();
        虚拟组.reserve(1);
        const auto 方法节点 = 新建类型化节点(
            方法节点类型, 已建节点, 已写字段);
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
                static_cast<std::int64_t>(i), 已建节点, 已写字段);
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
        追加方法虚拟归属(原子, 虚拟组.front().信息);
        原子.追加字段组.push_back({新方法组织根节点, 方法成员字段, 方法节点});
        if (替代父代) {
            const auto 旧 = 读取方法信息(替代父代->节点, 特征概念服务_, *this);
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
        const auto 片段 = 封存方法片段(std::move(原子), 已建节点);
        const auto 发表 = 内部治理世界_->发布新方法治理场景组({虚拟组.front().信息.引用},
            片段, 旧更新.准备 ? &*旧更新.准备 : nullptr);
        if (发表.状态 != 新方法治理场景发布工程状态::已发布)
            throw std::runtime_error("method publication refused without publication");
        已发布 = true;
        if (替代父代) {
            const auto 完成 = 特征服务_.完成顶层I64更新(*旧更新.准备);
            if (完成.状态 != 新特征写入状态::已完成 || !完成.当前值已改变)
                throw std::runtime_error("method replacement feature finalization failed");
            特征服务_.结束顶层I64更新(*旧更新.准备);
            const 新方法虚拟存在引用 引用{替代父代->节点, 替代父代->方法虚拟存在节点,
                替代父代->启用特征节点};
            同步方法专属概念(引用);
            const auto 后时间 = 方法当前UTC纳秒();
            if (后时间 <= 前时间) throw std::runtime_error("method UTC not increasing");
            const auto s = 状态服务_.建立状态(引用.虚拟存在节点, 引用.启用特征节点,
                {新状态时间语义::实例绝对UTC, 后时间});
            if (s.状态 != 新状态操作状态::已完成 || !s.状态节点)
                throw std::runtime_error("method replacement after state failed");
            内部治理世界_->完成方法特征变化(引用, *前状态, *s.状态节点);
        }
        const auto 读回 = 读取方法信息(方法节点, 特征概念服务_, *this);
        if (!读回 || 读回->行为 != 请求.行为
            || (登记名 && 读回->本能函数登记名 != *登记名)) {
            throw std::runtime_error("published method readback failed");
        }
        return {新方法操作状态::已建立, 方法节点};
    } catch (...) {
        if (!已发布) {
            if (旧更新.准备) 特征服务_.取消顶层I64更新(*旧更新.准备);
            if (前状态 && 状态服务_.删除状态(*前状态) != 新状态操作状态::已完成)
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"建立方法", L"未发布前状态清理失败。");
            回收本批方法图(已建节点, 已写字段, 虚拟组, false);
        }
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"建立方法治理结构", L"方法结构、特征或场景未完整发布读回，保留已发表事实。");
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
            if (!有效(方法父代字段)) 方法父代字段 = 全局基础数据集.新建节点();
            if (!节点仍存在(方法父代字段))
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"初始化方法代际字段", L"父代字段不可读。");
            for (const auto* 编码 : 架构编码组())
                if (*编码 == 方法父代字段)
                    工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"初始化方法代际字段", L"父代字段身份冲突。");
            for (auto* 键 : {&虚拟存在启用特征字段, &虚拟存在专属概念字段, &方法行为字段}) {
                if (!有效(*键)) *键 = 全局基础数据集.新建节点();
                if (!节点仍存在(*键)) throw std::runtime_error("method governance key unavailable");
                for (const auto* 编码 : 架构编码组())
                    if (*编码 == *键) throw std::runtime_error("method governance key collision");
            }
            if (虚拟存在启用特征字段 == 虚拟存在专属概念字段
                || 虚拟存在启用特征字段 == 方法父代字段 || 虚拟存在专属概念字段 == 方法父代字段
                || 方法行为字段 == 方法父代字段 || 方法行为字段 == 虚拟存在启用特征字段
                || 方法行为字段 == 虚拟存在专属概念字段)
                throw std::runtime_error("method extension keys collide");
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

bool 新_方法类::装配内部治理世界(新方法治理世界协作接口& 世界) noexcept {
    std::lock_guard 锁(新方法互斥);
    if (内部治理世界_ && 内部治理世界_ != &世界) return false;
    内部治理世界_ = &世界;
    return true;
}

void 新_方法类::核验治理组织就绪() const {
    if (!结构仍存在() || !内部治理世界_ || !治理组织已就绪_)
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"核验方法治理就绪", L"方法治理尚未装配或完成启动迁接。");
}

std::optional<新方法虚拟存在读回> 新_方法类::读取方法虚拟存在引用(
    const 新方法虚拟存在引用& 引用) const noexcept {
    try {
        std::lock_guard 锁(新方法互斥);
        if (!节点仍存在(引用.方法节点)) return std::nullopt;
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
        return 新方法虚拟存在读回{引用, *ec, 方法启用特征类型, v->特征概念节点, *value};
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
            const auto info = 读取方法信息(m, 特征概念服务_, *this, 旧);
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
                const auto seal = 封存方法片段(std::move(req), {old.节点, old.方法虚拟存在节点});
                const auto r = 内部治理世界_->发布新方法治理场景组({v.引用}, seal);
                if (r.状态 != 新方法治理场景发布工程状态::已发布)
                    throw std::runtime_error("legacy method zero publication");
                已发布 = true;
                const auto info = 读取方法信息(old.节点, 特征概念服务_, *this);
                auto expected = old; expected.启用特征节点 = v.引用.启用特征节点;
                expected.专属存在概念节点 = v.专属存在概念节点;
                if (!info || *info != expected) throw std::runtime_error("method migration readback mismatch");
                全部.push_back(v.引用);
            } catch (...) { if (!已发布) 清理未绑定方法特征(准备); throw; }
        }
        内部治理世界_->迁移已有方法治理场景(全部);
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
    const 新方法知识建立请求& 请求) noexcept {
    新方法建立结果 结果;
    try {
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
                        && 条件结果角色已声明(请求.参数规格组, 定义);
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
            const auto 已有 = 读取方法信息(已有节点, 特征概念服务_, *this);
            if (!已有) {
                结果.状态 = 新方法操作状态::结构不一致;
                return 结果;
            }
            std::vector<新方法条件结果定义> 已有配对;
            for (const auto& 项 : 已有->条件结果组) {
                已有配对.push_back({项.条件, 项.结果, 项.线性关系组});
            }
            if (已有->作用域 == 请求.作用域
                && 已有->行为 == 请求.行为
                && 已有->实现方式 == 请求.实现方式
                && 已有->方法头 == 请求.方法头
                && 已有->参数规格组 == 请求.参数规格组
                && !已有->来源方法节点
                && 已有配对.size() == 请求.条件结果组.size()
                && std::equal(已有配对.begin(), 已有配对.end(), 请求.条件结果组.begin(),
                    [](const auto& 左, const auto& 右) {
                        return 左.条件 == 右.条件 && 左.结果 == 右.结果
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
    } catch (...) {
        结果.状态 = 新方法操作状态::资源失败;
        return 结果;
    }
}

新方法学习发布结果 新_方法类::追加模拟条件结果(
    稳定编码 方法节点,
    const std::vector<新方法条件结果定义>& 新配对定义组) noexcept {
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
        const auto 方法 = 读取方法信息(方法节点, 特征概念服务_, *this);
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
        const auto& 定义 = 新配对定义组.front();
        const auto 类型已声明 = [&](const auto& 项) {
            const auto 规格 = std::find_if(方法->参数规格组.begin(), 方法->参数规格组.end(),
                [&](const auto& 参数) { return 参数.参数角色节点 == 项.参数角色节点; });
            return 规格 != 方法->参数规格组.end()
                && (!项.特征类型概念节点 || std::any_of(规格->特征规格组.begin(),
                    规格->特征规格组.end(), [&](const auto& 特征) {
                        return 特征.特征类型概念节点 == *项.特征类型概念节点;
                    }));
        };
        if (!行为结果相容(方法->行为, 定义.结果)
            || !方法头有效(方法->方法头) || !参数规格有效(方法->参数规格组, 特征概念服务_)
            || !条件结果有效(定义, 特征概念服务_)
            || !条件结果角色已声明(方法->参数规格组, 定义)
            || !std::all_of(定义.条件.条件项组.begin(), 定义.条件.条件项组.end(), 类型已声明)
            || !std::all_of(定义.结果.结果项组.begin(), 定义.结果.结果项组.end(), 类型已声明)
            || !std::all_of(定义.结果.确认要求.禁止结果项组.begin(),
                定义.结果.确认要求.禁止结果项组.end(), 类型已声明)) {
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
            结果.状态 = 已有配对->结果 == 定义.结果
                && ((已有配对->线性关系组.empty() && 定义.线性关系组.empty())
                    || 已有配对->条件 == 定义.条件)
                && 线性关系等价(已有配对->线性关系组, 定义.线性关系组)
                ? 新方法学习发布状态::精确重复 : 新方法学习发布状态::同条件结果冲突;
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
        if (!写入节点字段(方法节点, 方法条件结果字段, 新配对, 已写字段)) {
            回滚(已写字段, 已建节点);
            结果.新配对节点组.clear();
            结果.状态 = 新方法学习发布状态::资源失败;
            return 结果;
        }
        已发布 = true;
        const auto 读回 = 读取方法信息(方法节点, 特征概念服务_, *this);
        if (!读回 || 读回->行为 != 方法->行为
            || 读回->条件结果组.size() != 方法->条件结果组.size() + 1) {
            结果.状态 = 新方法学习发布状态::发布结果未知;
            return 结果;
        }
        const auto 新项 = std::find_if(读回->条件结果组.begin(), 读回->条件结果组.end(),
            [&](const auto& 项) { return 项.配对节点 == 新配对; });
        if (新项 == 读回->条件结果组.end()
            || std::count_if(读回->条件结果组.begin(), 读回->条件结果组.end(),
                [&](const auto& 项) { return 项.配对节点 == 新配对; }) != 1
            || 新项->命中次数 != 0 || 新项->条件 != 定义.条件 || 新项->结果 != 定义.结果
            || !线性关系等价(新项->线性关系组, 定义.线性关系组)
            || !std::all_of(方法->条件结果组.begin(), 方法->条件结果组.end(),
                [&](const auto& 旧项) {
                    return std::count_if(读回->条件结果组.begin(), 读回->条件结果组.end(),
                        [&](const auto& 项) { return 项.配对节点 == 旧项.配对节点; }) == 1
                        && std::find(读回->条件结果组.begin(), 读回->条件结果组.end(), 旧项)
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
        结果.状态 = 已发布 ? 新方法学习发布状态::发布结果未知
            : 新方法学习发布状态::资源失败;
        return 结果;
    }
}

新方法建立结果 新_方法类::确保本能方法登记(
    const std::string& 登记名,
    const 新方法知识建立请求& 请求) noexcept {
    try {
        if (登记名.empty() || !新_本能函数管理类::解析(登记名).函数) {
            return {新方法操作状态::函数登记不合法, std::nullopt};
        }
        std::lock_guard 锁(新方法互斥);
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
        if (登记名 == 新本能函数登记名_服务值结算) {
            const auto 类别 = 读取服务结算声明种类(请求.方法头, 请求.参数规格组, 特征概念服务_);
            if (类别 == 服务结算声明种类::按贡献Q) return 确保服务结算Q方法登记(请求);
            if (类别 != 服务结算声明种类::固定加一)
                return {新方法操作状态::函数登记冲突, std::nullopt};
        }
        const auto 已有 = 查询本能登记结构(登记名, 特征概念服务_, *this);
        if (已有.状态 == 新方法操作状态::已找到 && 已有.方法节点) {
            const auto 方法 = 读取方法信息(*已有.方法节点, 特征概念服务_, *this);
            if (!方法) return {新方法操作状态::结构不一致, std::nullopt};
            if (!内部治理世界_->读取方法治理场景({方法->节点, 方法->方法虚拟存在节点, 方法->启用特征节点}))
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"复用本能方法", L"已登记本能方法没有完整治理场景。");
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
                return {新方法操作状态::函数登记冲突, std::nullopt};
            }
            return 补登记方法行为(*方法, 请求.行为, 特征概念服务_, *this);
        }
        if (已有.状态 != 新方法操作状态::方法不存在) return 已有;
        // 本能登记允许尚无运行知识的头；非空配对仍必须完整合法。
        if (!std::all_of(请求.条件结果组.begin(), 请求.条件结果组.end(),
                [&](const 新方法条件结果定义& 定义) {
                    return 条件结果有效(定义, 特征概念服务_)
                        && 条件结果角色已声明(请求.参数规格组, 定义);
                })) {
            return {新方法操作状态::条件结果不合法, std::nullopt};
        }
        return 发布方法结构(请求, &登记名);
    } catch (...) {
        return {新方法操作状态::资源失败, std::nullopt};
    }
}

新方法建立结果 新_方法类::查询本能登记(
    const std::string& 登记名) const noexcept {
    if (登记名.empty()) return {新方法操作状态::函数登记不合法, std::nullopt};
    try {
        std::lock_guard 锁(新方法互斥);
        return 查询本能登记结构(登记名, 特征概念服务_, *this);
    } catch (...) {
        return {新方法操作状态::资源失败, std::nullopt};
    }
}

新方法建立结果 新_方法类::确保服务结算Q方法登记(
    const 新方法知识建立请求& 请求) noexcept {
    try {
        std::lock_guard 锁(新方法互斥);
        if (!结构仍存在() || !节点仍存在(方法父代字段))
            throw std::runtime_error("method schema unavailable");
        核验治理组织就绪();
        if (!行为声明有效(请求) || 请求.作用域 != 新方法作用域::普通世界 || 请求.实现方式 != 新方法实现方式::本能
            || !请求.条件结果组.empty() || !方法头有效(请求.方法头)
            || !参数规格有效(请求.参数规格组, 特征概念服务_)
            || 读取服务结算声明种类(请求.方法头, 请求.参数规格组, 特征概念服务_)
                != 服务结算声明种类::按贡献Q
            || !新_本能函数管理类::解析(新本能函数登记名_服务值结算).函数)
            throw std::runtime_error("Q registration contract invalid");
        const std::string 名称(新本能函数登记名_服务值结算);
        const auto 已有 = 查询本能登记结构(名称, 特征概念服务_, *this);
        std::optional<新方法信息> 旧;
        if (已有.状态 == 新方法操作状态::已找到 && 已有.方法节点) {
            旧 = 读取方法信息(*已有.方法节点, 特征概念服务_, *this);
            if (!旧) throw std::runtime_error("registered method unreadable");
            if (旧->行为 && 请求.行为 && 旧->行为 != 请求.行为)
                return {新方法操作状态::函数登记冲突, std::nullopt};
            if (!内部治理世界_->读取方法治理场景({旧->节点, 旧->方法虚拟存在节点, 旧->启用特征节点}))
                throw std::runtime_error("registered settlement method has no complete scene");
            if (读取服务结算声明种类(*旧, 特征概念服务_) == 服务结算声明种类::按贡献Q) {
                if (旧->方法头 != 请求.方法头 || 旧->参数规格组 != 请求.参数规格组)
                    throw std::runtime_error("Q registration changed meaning");
                return 补登记方法行为(*旧, 请求.行为, 特征概念服务_, *this);
            }
            if (旧->父代方法节点 || 旧->启用状态 != 新方法启用状态::启用
                || 读取服务结算声明种类(*旧, 特征概念服务_) != 服务结算声明种类::固定加一)
                throw std::runtime_error("legacy method is not an authorized predecessor");
            // 同一个作用FT、状态FT及集合角色；仅改变已冻结的前值/Q规格。
            const auto& 老自我 = 旧->参数规格组[0]; const auto& 新自我 = 请求.参数规格组[0];
            const auto& 老来源 = 旧->参数规格组[1]; const auto& 新来源 = 请求.参数规格组[1];
            if (旧->方法头.涉及特征类型概念组 != 请求.方法头.涉及特征类型概念组
                || 老自我.参数角色节点 != 新自我.参数角色节点
                || 老自我.特征规格组[1] != 新自我.特征规格组[1]
                || 老来源.参数角色节点 != 新来源.参数角色节点
                || 老来源.特征规格组[0] != 新来源.特征规格组[0]
                || 老来源.集合成员约束 != 新来源.集合成员约束)
                throw std::runtime_error("legacy and Q roles differ");
        } else if (已有.状态 != 新方法操作状态::方法不存在)
            throw std::runtime_error("method lookup failed");
        auto 发布请求 = 请求;
        if (旧 && !发布请求.行为) 发布请求.行为 = 旧->行为;
        const auto 发表 = 发布方法结构(发布请求, &名称, 旧 ? &*旧 : nullptr);
        if (发表.状态 != 新方法操作状态::已建立 || !发表.方法节点)
            throw std::runtime_error("Q method publication incomplete");
        const auto 读回 = 查询本能登记结构(名称, 特征概念服务_, *this);
        const auto 新 = 读取方法信息(*发表.方法节点, 特征概念服务_, *this);
        if (读回.状态 != 新方法操作状态::已找到 || 读回.方法节点 != 发表.方法节点
            || !新 || 新->方法头 != 请求.方法头 || 新->参数规格组 != 请求.参数规格组
            || !新->条件结果组.empty() || 新->父代方法节点 != (旧 ? std::optional{旧->节点} : std::nullopt))
            throw std::runtime_error("Q method readback incomplete");
        return 发表;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"确保服务结算Q方法登记",
            L"限定方法替代不完整或发生冲突，保留旧方法及已发表事实。");
    }
}

std::optional<稳定编码> 新_方法类::读取服务结算固定加一父代(
    稳定编码 当前Q方法) const noexcept {
    try {
        std::lock_guard 锁(新方法互斥);
        const auto 当前 = 读取方法信息(当前Q方法, 特征概念服务_, *this);
        const auto 登记 = 查询本能登记结构(std::string(新本能函数登记名_服务值结算), 特征概念服务_, *this);
        if (!当前 || 当前->来源方法节点 || 当前->实例根节点
            || 读取服务结算声明种类(*当前, 特征概念服务_) != 服务结算声明种类::按贡献Q
            || 登记.状态 != 新方法操作状态::已找到 || 登记.方法节点 != 当前Q方法)
            throw std::runtime_error("Q predecessor lookup mismatch");
        return 当前->父代方法节点;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取服务结算固定加一父代",
            L"方法父代关系损坏，不能改用当前Q规格解释历史。");
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
        const auto 方法 = 读取方法信息(方法节点, 特征概念服务_, *this);
        if (!方法) return 新方法操作状态::方法不存在;
        if (方法->实现方式 != 新方法实现方式::本能) {
            return 新方法操作状态::方法类型不符;
        }
        if (方法->本能函数登记名) {
            return *方法->本能函数登记名 == 本能函数登记名
                ? 新方法操作状态::无变化
                : 新方法操作状态::函数登记冲突;
        }
        // 服务结算的有限代际只能通过完整登记事务建立，禁止先建空方法再绑同名函数。
        if (本能函数登记名 == 新本能函数登记名_服务值结算)
            return 新方法操作状态::函数登记冲突;

        新关系 = 全局基础数据集.添加字段值(
            方法节点, 方法本能函数登记名字段,
            基础值{本能函数登记名});
        if (!有效(新关系)) {
            return 新方法操作状态::资源失败;
        }
        const auto 读回 = 读取方法信息(方法节点, 特征概念服务_, *this);
        if (!读回 || !读回->本能函数登记名
            || *读回->本能函数登记名 != 本能函数登记名) {
            (void)全局基础数据集.删除字段(新关系);
            return 新方法操作状态::结构不一致;
        }
        return 新方法操作状态::已修改;
    } catch (...) {
        if (有效(新关系)) (void)全局基础数据集.删除字段(新关系);
        return 新方法操作状态::资源失败;
    }
}

std::optional<新方法信息> 新_方法类::获取方法(
    稳定编码 方法节点) const noexcept {
    try {
        std::lock_guard 锁(新方法互斥);
        return 读取方法信息(方法节点, 特征概念服务_, *this);
    } catch (...) {
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
            auto 方法 = 读取方法信息(节点, 特征概念服务_, *this);
            if (!方法) 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__,
                L"读取方法执行结构闭包", L"正式方法闭包断链或定义损坏");
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
        return 结果;
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
            const auto 来源 = 读取方法信息(来源方法节点, 特征概念服务_, *this);
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
                const auto 方法 = 读取方法信息(当前, 特征概念服务_, *this);
                if (!方法) return 失败(新方法复制状态::结构不一致);
                if (方法->来源方法节点) return 失败(新方法复制状态::来源不是抽象方法);
                if (!方法头有效(方法->方法头) || !参数规格有效(方法->参数规格组, 特征概念服务_)
                    || (方法->实现方式 != 新方法实现方式::本能 && 方法->条件结果组.empty())
                    || !std::all_of(方法->条件结果组.begin(), 方法->条件结果组.end(),
                        [&](const auto& 配对) {
                            const 新方法条件结果定义 定义{配对.条件, 配对.结果, 配对.线性关系组};
                            return 条件结果有效(定义, 特征概念服务_)
                                && 条件结果角色已声明(方法->参数规格组, 定义);
                        })) return 失败(新方法复制状态::结构不一致);
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
                                配对结果字段, 条件项字段, 结果项字段, 结果禁止项字段}) {
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
            const auto source = 读取方法信息(方法, 特征概念服务_, *this);
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
        const auto 片段 = 封存方法片段(std::move(发布请求), 已建节点);
        const auto 发表 = 内部治理世界_->发布新方法治理场景组(引用组, 片段);
        if (发表.状态 != 新方法治理场景发布工程状态::已发布) return 失败(新方法复制状态::资源失败);
        已发布 = true;
        for (const auto 方法 : 方法组) {
            const auto 读回 = 读取方法信息(材料.at(方法).新节点, 特征概念服务_, *this);
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
        std::map<稳定编码, std::vector<稳定编码>> 追加配对组;
        std::vector<稳定编码> 待读来源(候选来源.begin(), 候选来源.end());
        for (const auto& 成员 : 全局基础数据集.查询目标字段(预期旧实例根, 方法实例根字段)) {
            const auto 旧 = 读取方法信息(成员.所属节点, 特征概念服务_, *this);
            if (!旧 || !旧->来源方法节点 || !来源映射.emplace(*旧->来源方法节点, 旧->节点).second)
                return 失败(状态::来源无法对齐);
            旧方法组.emplace(旧->节点, *旧);
            待读来源.push_back(*旧->来源方法节点);
        }
        for (std::size_t i = 0; i < 待读来源.size(); ++i) {
            const auto 编码 = 待读来源[i];
            if (来源方法组.contains(编码)) continue;
            const auto 方法 = 读取方法信息(编码, 特征概念服务_, *this);
            if (!方法) return 失败(状态::来源方法不存在);
            if (方法->来源方法节点 || 方法->实例根节点) return 失败(状态::来源无法对齐);
            if (!方法头有效(方法->方法头) || !参数规格有效(方法->参数规格组, 特征概念服务_)
                || (方法->实现方式 != 新方法实现方式::本能 && 方法->条件结果组.empty())
                || !std::all_of(方法->条件结果组.begin(), 方法->条件结果组.end(), [&](const auto& 对) {
                    const 新方法条件结果定义 定义{对.条件, 对.结果, 对.线性关系组};
                    return 条件结果有效(定义, 特征概念服务_)
                        && 条件结果角色已声明(方法->参数规格组, 定义);
                })) return 失败(状态::结构不一致);
            来源方法组.emplace(编码, *方法);
            for (const auto& 步骤 : 方法->组合步骤组) 待读来源.push_back(步骤.调用方法节点);
        }
        const auto 配对可对齐 = [&](const 新方法条件结果信息& 旧, const 新方法条件结果信息& 源) {
            if (旧.结果 != 源.结果 || !线性关系等价(旧.线性关系组, 源.线性关系组)) return false;
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
                || 旧.作用域 != 源.作用域 || 旧.实现方式 != 源.实现方式
                || 旧.启用状态 != 源.启用状态 || 旧.本能函数登记名 != 源.本能函数登记名)
                return 失败(状态::来源无法对齐);
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
                                结果项字段, 结果禁止项字段}) {
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
        const auto 片段 = 封存方法片段(std::move(发布请求), 已建节点);
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
            for (auto& 项 : 预期.条件结果组) {
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
            const auto 读回 = 读取方法信息(预期.节点, 特征概念服务_, *this);
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
    } catch (...) {
        return 失败(状态::资源失败);
    }
}

std::optional<新方法混合实例信息> 新_方法类::获取混合方法实例(
    稳定编码 实例根节点) const noexcept {
    try {
        std::lock_guard 锁(新方法互斥);
        const auto 信息 = 读取实例根结构(实例根节点, 特征概念服务_);
        if (!信息) return std::nullopt;
        // 候选之外的共享被调方法也必须完整可读，不能只确认根标签。
        for (const auto& 成员 : 全局基础数据集.查询目标字段(
            实例根节点, 方法实例根字段)) {
            const auto 方法 = 读取方法信息(成员.所属节点, 特征概念服务_, *this);
            if (!方法 || !方法头有效(方法->方法头)
                || !参数规格有效(方法->参数规格组, 特征概念服务_)
                || (方法->实现方式 != 新方法实现方式::本能 && 方法->条件结果组.empty())
                || !std::all_of(方法->条件结果组.begin(), 方法->条件结果组.end(),
                    [&](const auto& 配对) {
                        const 新方法条件结果定义 定义{配对.条件, 配对.结果, 配对.线性关系组};
                        return 条件结果有效(定义, 特征概念服务_, true)
                            && 条件结果角色已声明(方法->参数规格组, 定义);
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
        const auto 方法 = 读取方法信息(方法节点, 特征概念服务_, *this);
        if (!根 || !方法 || !方法->来源方法节点
            || 方法->实例根节点 != std::optional<稳定编码>{实例根节点}) return 失败(状态::入口拒绝);
        const auto 配对 = std::find_if(方法->条件结果组.begin(), 方法->条件结果组.end(),
            [&](const auto& 项) { return 项.配对节点 == 预期配对.配对节点; });
        if (配对 == 方法->条件结果组.end() || *配对 != 预期配对) return 失败(状态::来源已变化);
        if (所需条件域组.empty() || 所需条件域组.size() != 配对->条件.条件项组.size()
            || 配对->线性关系组.empty()) return 失败(状态::入口拒绝);
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
        for (const auto 节点 : 旧节点组) {
            for (const auto 关系类型 : {基础外部关系类型::父子, 基础外部关系类型::兄弟}) {
                if (!全局基础数据集.查询源关系(节点, 关系类型).empty()
                    || !全局基础数据集.查询目标关系(节点, 关系类型).empty()) return 失败(状态::结构不一致);
            }
            std::size_t 持有数 = 0;
            const bool 是C = 节点 == 配对->条件节点;
            // 公开L1只能按已知字段查询；未知非法引用由删除拒绝保留，不冒充已证明不存在。
            for (const auto* 字段 : 架构编码组()) {
                for (const auto& 持有 : 全局基础数据集.查询目标字段(节点, *字段)) {
                    if (持有.所属节点 != (是C ? 配对->配对节点 : 配对->条件节点)
                        || 持有.字段节点 != (是C ? 配对条件字段 : 条件项字段)) return 失败(状态::结构不一致);
                    ++持有数;
                }
            }
            if (持有数 != 1) return 失败(状态::结构不一致);
            for (const auto& 规格 : 方法复制字段规格(是C ? 条件节点类型 : 条件项节点类型))
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
        const auto 读回 = 读取方法信息(方法节点, 特征概念服务_, *this);
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
            const auto 方法 = 读取方法信息(方法节点, 特征概念服务_, *this);
            if (!方法) return {新方法操作状态::结构不一致, {}};
            for (const auto& 配对 : 方法->条件结果组) {
                结果.配对组.push_back({方法节点, 配对});
            }
        }
        结果.状态 = 新方法操作状态::已找到;
        return 结果;
    } catch (...) {
        return {新方法操作状态::资源失败, {}};
    }
}

bool 新_方法类::是方法节点(稳定编码 节点) const noexcept {
    try {
        std::lock_guard 锁(新方法互斥);
        return 读取方法信息(节点, 特征概念服务_, *this).has_value();
    } catch (...) {
        return false;
    }
}

新方法候选结果 新_方法类::查询候选方法(
    const 新方法候选查询& 查询) const noexcept {
    新方法候选结果 结果;
    try {
        std::lock_guard 锁(新方法互斥);
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
        const auto 成员组 = 读取多节点字段(新方法组织根节点, 方法成员字段, 特征概念服务_);
        if (!成员组) {
            结果.状态 = 新方法操作状态::结构不一致;
            return 结果;
        }
        for (const auto 方法节点 : *成员组) {
            const auto 方法 = 读取方法信息(方法节点, 特征概念服务_, *this);
            if (!方法) {
                结果.状态 = 新方法操作状态::结构不一致;
                结果.方法节点组.clear();
                return 结果;
            }
            if (方法->来源方法节点 || 方法->启用状态 != 新方法启用状态::启用
                || (查询.作用域 && 方法->作用域 != *查询.作用域)
                || (查询.实现方式 && 方法->实现方式 != *查询.实现方式)
                || !包含全部(方法->方法头.作用存在概念组,
                    查询.作用存在概念组)
                || !包含全部(方法->方法头.涉及特征类型概念组,
                    查询.涉及特征类型概念组)
                || !包含全部(方法->方法头.可能结果概念组,
                    查询.可能结果概念组)) continue;
            结果.方法节点组.push_back(方法节点);
        }
        std::sort(结果.方法节点组.begin(), 结果.方法节点组.end());
        结果.状态 = 新方法操作状态::已找到;
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
    新特征I64准备结果 准备;
    std::optional<稳定编码> 前状态;
    bool 已证零发表 = true;
    try {
        std::lock_guard 锁(新方法互斥);
        if (!结构仍存在()) return 新方法操作状态::尚未初始化;
        核验治理组织就绪();
        const auto 方法 = 读取方法信息(方法节点, 特征概念服务_, *this);
        if (!方法) return 新方法操作状态::方法不存在;
        const 新方法虚拟存在引用 引用{方法节点, 方法->方法虚拟存在节点, 方法->启用特征节点};
        if (!内部治理世界_->读取方法治理场景(引用)) throw std::runtime_error("method scene missing before enabled update");
        if (新状态 != 新方法启用状态::停用
            && 新状态 != 新方法启用状态::启用) {
            return 新方法操作状态::入口拒绝;
        }
        if (方法->启用状态 == 新状态) return 新方法操作状态::无变化;
        if (方法->本能函数登记名 == 新本能函数登记名_服务值结算
            && (方法->父代方法节点 || !全局基础数据集.查询目标字段(方法节点, 方法父代字段).empty()
                || 读取服务结算声明种类(*方法, 特征概念服务_) == 服务结算声明种类::按贡献Q))
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
        const auto 后时间 = 方法当前UTC纳秒();
        if (后时间 <= 前时间) throw std::runtime_error("method feature UTC not increasing");
        const auto s1 = 状态服务_.建立状态(引用.虚拟存在节点, 引用.启用特征节点,
            {新状态时间语义::实例绝对UTC, 后时间});
        if (s1.状态 != 新状态操作状态::已完成 || !s1.状态节点)
            throw std::runtime_error("method enabled after state unavailable");
        内部治理世界_->完成方法特征变化(引用, *前状态, *s1.状态节点);
        const auto v = 读取方法虚拟存在引用(引用);
        if (!v || v->当前启用 != 新值
            || !内部治理世界_->读取方法特征变化(引用, *前状态, *s1.状态节点))
            throw std::runtime_error("method enabled update final readback failed");
        return 新方法操作状态::已修改;
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
        const auto 组合 = 读取方法信息(请求.组合方法节点, 特征概念服务_, *this);
        const auto 调用 = 读取方法信息(请求.调用方法节点, 特征概念服务_, *this);
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
        const auto 方法 = 读取方法信息(组合方法节点, 特征概念服务_, *this);
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
    } catch (...) {
        return 新方法操作状态::资源失败;
    }
}

新方法结构节点结果 新_方法类::添加结果转移(
    const 新方法结果转移添加请求& 请求) noexcept {
    新方法结构节点结果 结果;
    try {
        std::lock_guard 锁(新方法互斥);
        const auto 组合 = 读取方法信息(请求.组合方法节点, 特征概念服务_, *this);
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
        const auto 被调方法 = 读取方法信息(当前步骤->调用方法节点, 特征概念服务_, *this);
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
    } catch (...) {
        结果.状态 = 新方法操作状态::资源失败;
        return 结果;
    }
}

新方法参数场景结果 新_方法类::建立参数场景(
    稳定编码 方法节点,
    std::vector<新方法参数角色绑定> 角色绑定组,
    const 新内部治理对象读取接口* 治理对象读取,
    const 新服务要求材料读取接口* 服务要求读取) const noexcept {
    新方法参数场景结果 结果;
    try {
        std::optional<新方法信息> 方法;
        {
            // 这里只在复制方法规格时持有本类锁；后续调用场景、存在、
            // 特征与概念服务时不跨服务持锁。
            std::lock_guard 锁(新方法互斥);
            方法 = 读取方法信息(方法节点, 特征概念服务_, *this);
        }
        if (!方法) {
            结果.状态 = 新方法操作状态::方法不存在;
            return 结果;
        }
        std::set<稳定编码> 已有角色;
        for (const auto& 绑定 : 角色绑定组) {
            if (!节点仍存在(绑定.参数角色节点)
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
                    : 存在服务_.匹配存在概念(*实际节点, *规格.存在概念节点).状态;
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
                治理对象读取, {}, 服务要求读取));
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
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"建立参数场景", L"参数结构或概念核验发生未知工程异常。");
    }
}

新方法参数场景结果 新_方法类::建立服务结算治理参数场景(
    稳定编码 方法节点, const 新任务贡献治理参数& 参数,
    const 新内部治理对象读取接口& 治理对象读取) const noexcept {
    新方法参数场景结果 结果;
    try {
        {
            std::lock_guard 锁(新方法互斥);
            const auto 方法 = 读取方法信息(方法节点, 特征概念服务_, *this);
            const auto 当前 = 查询本能登记结构(std::string(新本能函数登记名_服务值结算), 特征概念服务_, *this);
            if (!方法 || 当前.状态 != 新方法操作状态::已找到 || 当前.方法节点 != 方法节点
                || 方法->启用状态 != 新方法启用状态::启用 || 方法->来源方法节点 || 方法->实例根节点
                || 读取服务结算声明种类(*方法, 特征概念服务_) != 服务结算声明种类::按贡献Q) {
                结果.状态 = 新方法操作状态::参数引用失效; return 结果;
            }
        }
        // 只消费中立读取接口；D/C/X/A的正式关系由任务owner在读取和执行两端核验。
        std::set<稳定编码> 已见需求, 已见任务;
        const auto 核验对象 = [&](const 新任务贡献治理对象引用& 对象, bool 必须有任务) {
            if (!有效(对象.需求节点) || !已见需求.insert(对象.需求节点).second) return false;
            if (!对象.任务引用) return !必须有任务 && !对象.服务值特征节点 && !对象.治理场景节点
                && !对象.执行实例节点 && !对象.影响评估节点 && !对象.完成结算节点 && !对象.接收节点;
            if (!对象.服务值特征节点 || !有效(*对象.服务值特征节点)
                || !对象.治理场景节点 || !有效(*对象.治理场景节点)
                || !已见任务.insert(对象.任务引用->任务节点).second) return false;
            const auto f = 治理对象读取.读取任务虚拟存在特征组(*对象.任务引用);
            return f && f->引用 == *对象.任务引用 && f->状态.特征节点 == 对象.任务引用->状态特征节点
                && f->服务值 && f->服务值->特征节点 == 对象.服务值特征节点;
        };
        if (!有效(参数.完成来源.任务节点) || !有效(参数.完成来源.执行实例节点)
            || !有效(参数.完成来源.影响评估节点) || !核验对象(参数.来源, true)
            || 参数.来源.任务引用->任务节点 != 参数.完成来源.任务节点
            || 参数.来源.执行实例节点 != 参数.完成来源.执行实例节点
            || 参数.来源.影响评估节点 != 参数.完成来源.影响评估节点
            || (参数.直接父 && !核验对象(*参数.直接父, false))) {
            结果.状态 = 新方法操作状态::参数引用失效; return 结果;
        }
        std::optional<稳定编码> 前子需求;
        for (const auto& 子 : 参数.直接子) {
            if ((前子需求 && !(*前子需求 < 子.需求节点)) || !核验对象(子, false)) {
                结果.状态 = 新方法操作状态::参数引用失效; return 结果;
            }
            前子需求 = 子.需求节点;
        }
        结果.参数场景 = std::shared_ptr<const 方法参数场景>(new 方法参数场景(
            方法节点, {}, 特征服务_, 特征值服务_, 新方法参数场景用途::任务贡献治理,
            &治理对象读取, {}, nullptr, 参数));
        结果.状态 = 新方法操作状态::已建立;
        return 结果;
    } catch (...) {
        结果.状态 = 新方法操作状态::资源失败;
        结果.参数场景.reset();
        return 结果;
    }
}

新方法参数场景结果 新_方法类::建立模拟参数场景(
    稳定编码 方法节点,
    std::vector<新方法参数角色绑定> 绑定,
    std::shared_ptr<新方法模拟运行接口> 模拟) const noexcept {
    新方法参数场景结果 结果;
    try {
        std::optional<新方法信息> 方法;
        {
            std::lock_guard 锁(新方法互斥);
            方法 = 读取方法信息(方法节点, 特征概念服务_, *this);
        }
        if (!方法 || 方法->来源方法节点 || 方法->实例根节点
            || 方法->实现方式 != 新方法实现方式::本能
            || 方法->启用状态 != 新方法启用状态::启用
            || !方法->本能函数登记名 || 方法->本能函数登记名->empty()) {
            结果.状态 = 新方法操作状态::方法不存在;
            return 结果;
        }
        if (方法->本能函数登记名 == 新本能函数登记名_服务值结算
            && 读取服务结算声明种类(*方法, 特征概念服务_) == 服务结算声明种类::按贡献Q) {
            // 新Q尚无模拟owner；在取模拟端口或读取现实材料前结束。
            结果.状态 = 新方法操作状态::参数概念不相容;
            return 结果;
        }
        // 本切片没有服务要求材料的模拟owner，必须在调用任何模拟/现实端口前拒绝。
        if (std::any_of(方法->参数规格组.begin(), 方法->参数规格组.end(),
                [](const auto& 规格) { return 是服务要求参数对象(规格.对象种类); })
            || std::any_of(绑定.begin(), 绑定.end(), [](const auto& 角色) {
                return 角色.服务要求分项引用 || 角色.服务要求确认引用;
            })) {
            结果.状态 = 新方法操作状态::参数概念不相容;
            return 结果;
        }
        // 外部模拟owner核验在方法锁之外；这里绝不调用现实场景/存在owner。
        if (!模拟 || !节点仍存在(模拟->模拟根节点())
            || !模拟->核验模拟参数(方法节点, 绑定)
            || !模拟->获取服务值结算模拟端口()) {
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
            新方法参数场景用途::模拟, nullptr, std::move(模拟)));
        结果.状态 = 新方法操作状态::已建立;
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
        // 登记只接收已存在的现实目标与一个特征，不接收来源任务占位。
        // 自我/服务FT的业务身份由登记本能对照自身正式材料再核验。
        if (角色绑定组.size() != 1) {
            结果.状态 = 新方法操作状态::参数角色缺失;
            return 结果;
        }
        const auto& 绑定 = 角色绑定组.front();
        if (!节点仍存在(绑定.参数角色节点) || 绑定.任务虚拟存在引用
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
                特征服务_, 特征值服务_, 新方法参数场景用途::登记, nullptr));
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
            {}, {}, 特征服务_, 特征值服务_, 新方法参数场景用途::登记, nullptr));
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
            方法 = 读取方法信息(方法节点, 特征概念服务_, *this);
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
        const bool 治理 = 参数场景.用途() == 新方法参数场景用途::任务贡献治理;
        if (治理 != 参数场景.读取任务贡献治理参数().has_value()
            || (治理 && (方法->本能函数登记名 != 新本能函数登记名_服务值结算
                || !参数场景.全部角色绑定().empty()
                || 读取服务结算声明种类(*方法, 特征概念服务_) != 服务结算声明种类::按贡献Q))) {
            结果.状态 = 新方法本能调用状态::参数场景不匹配; return 结果;
        }
        if (方法->本能函数登记名 == 新本能函数登记名_服务值结算) {
            std::lock_guard 锁(新方法互斥);
            const auto 当前 = 查询本能登记结构(std::string(新本能函数登记名_服务值结算), 特征概念服务_, *this);
            // 旧冻结副本也不能借原登记名执行新Q函数。
            const auto 抽象 = 方法->来源方法节点.value_or(方法->节点);
            if (当前.状态 != 新方法操作状态::已找到 || 当前.方法节点 != 抽象) {
                结果.状态 = 新方法本能调用状态::方法停用; return 结果;
            }
            if (方法->来源方法节点 || 方法->实例根节点) {
                结果.状态 = 新方法本能调用状态::参数场景不匹配; return 结果;
            }
        }
        const auto 函数 = 新_本能函数管理类::解析(*方法->本能函数登记名);
        if (!函数.函数) {
            结果.状态 = 函数.状态 == 新本能函数解析状态::内部错误
                ? 新方法本能调用状态::内部错误
                : 新方法本能调用状态::函数未登记;
            return 结果;
        }
        结果.本次条件结果对 = 函数.函数(参数场景);
        结果.状态 = 新方法本能调用状态::已调用;
        return 结果;
    } catch (const std::bad_alloc&) {
        结果.状态 = 新方法本能调用状态::资源失败;
    } catch (...) {
        结果.状态 = 新方法本能调用状态::内部错误;
    }
    结果.本次条件结果对.reset();
    return 结果;
}

} // namespace 海中鱼巣
