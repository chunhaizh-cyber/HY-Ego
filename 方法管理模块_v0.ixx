module;

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_set>
#include <utility>
#include <vector>

export module 方法管理模块_v0;

import 元层核心模块;

namespace 方法管理模块_v0_内部 {
inline std::string 取字段(const 元字段表& 字段, std::string_view 键)
{
    if (const auto it = 字段.find(std::string(键)); it != 字段.end()) {
        return it->second;
    }
    return {};
}

inline double 取数字字段(const 元字段表& 字段, std::string_view 键, double 默认值 = 0.0)
{
    const auto 值 = 取字段(字段, 键);
    if (值.empty()) return 默认值;
    try {
        return std::stod(值);
    }
    catch (...) {
        return 默认值;
    }
}

inline std::vector<std::string> 拆分标签串(std::string_view 文本)
{
    std::vector<std::string> 结果;
    std::string 当前;
    for (const char ch : 文本) {
        if (ch == ',' || ch == ';' || ch == '|' || ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n') {
            if (!当前.empty()) {
                结果.push_back(std::move(当前));
                当前.clear();
            }
            continue;
        }
        当前.push_back(ch);
    }
    if (!当前.empty()) {
        结果.push_back(std::move(当前));
    }
    return 结果;
}

inline std::vector<std::string> 取实体标签列表(const 结构_元实体& 实体, std::string_view 字段键)
{
    std::vector<std::string> 结果;
    for (const auto& 标签 : 实体.标签) {
        if (!标签.empty()) {
            结果.push_back(标签);
        }
    }
    const auto 扩展标签 = 拆分标签串(取字段(实体.内容, 字段键));
    结果.insert(结果.end(), 扩展标签.begin(), 扩展标签.end());
    return 结果;
}

inline std::string 取任务类型(const 结构_元实体& 任务)
{
    auto 值 = 取字段(任务.内容, "任务类型");
    if (值.empty()) {
        值 = 取字段(任务.内容, "类型");
    }
    return 值;
}

inline std::string 取方法适用任务类型(const 结构_元实体& 方法)
{
    auto 值 = 取字段(方法.内容, "适用任务类型");
    if (值.empty()) {
        值 = 取字段(方法.内容, "任务类型");
    }
    return 值;
}

inline double 取任务当前安全值(const 结构_元实体& 任务)
{
    return 取数字字段(任务.内容, "当前安全值",
        取数字字段(任务.内容, "安全值", 50.0));
}

inline double 取任务当前服务值(const 结构_元实体& 任务)
{
    return 取数字字段(任务.内容, "当前服务值",
        取数字字段(任务.内容, "服务值", 0.0));
}

inline double 取任务安全下限(const 结构_元实体& 任务)
{
    return 取数字字段(任务.内容, "安全下限", 1.0);
}

inline double 计算安全风险权重(const 结构_元实体& 任务)
{
    const double 当前安全值 = 取任务当前安全值(任务);
    const double 安全下限 = 取任务安全下限(任务);
    const double 分母 = std::max(当前安全值 - 安全下限 + 1.0, 1.0);
    return std::clamp(1.0 / 分母, 0.0, 64.0);
}

inline double 计算服务偏好系数(const 结构_元实体& 任务)
{
    const double 当前服务值 = 取任务当前服务值(任务);
    const double 服务缺口 = std::max(0.0, 50.0 - 当前服务值);
    return 1.0 + std::clamp(服务缺口 / 50.0, 0.0, 1.0);
}

inline int 计算方法选择自检强度(const 结构_元实体& 任务)
{
    const double 当前安全值 = 取任务当前安全值(任务);
    const double 安全下限 = 取任务安全下限(任务);
    if (当前安全值 <= 安全下限 + 8.0) return 2;
    if (当前安全值 < 40.0) return 1;
    return 0;
}

inline const 结构_元关系* 查找激活绑定关系(
    const std::vector<结构_元关系>& 关系集,
    const std::string& 方法标识)
{
    for (const auto& 关系 : 关系集) {
        if (关系.终点标识 == 方法标识 && !关系.是否失效) {
            return &关系;
        }
    }
    return nullptr;
}

inline double 计算历史成功率修正(const 结构_元实体& 方法实体, const 结构_元关系* 绑定关系)
{
    double 成功次数 = 0.0;
    double 失败次数 = 0.0;
    if (绑定关系) {
        成功次数 = 取数字字段(绑定关系->内容, "成功次数", 0.0);
        失败次数 = 取数字字段(绑定关系->内容, "失败次数", 0.0);
    }
    if (成功次数 <= 0.0 && 失败次数 <= 0.0) {
        成功次数 = 取数字字段(方法实体.内容, "历史成功次数", 0.0);
        失败次数 = 取数字字段(方法实体.内容, "历史失败次数", 0.0);
    }

    const double 总次数 = 成功次数 + 失败次数;
    if (总次数 <= 0.0) {
        return 0.0;
    }
    return (成功次数 / 总次数) - 0.5;
}

inline double 计算服务收益修正(const 结构_元实体& 方法实体, const 结构_元关系* 绑定关系)
{
    if (绑定关系) {
        const double 关系值 = 取数字字段(绑定关系->内容, "平均服务收益修正", std::numeric_limits<double>::quiet_NaN());
        if (!std::isnan(关系值)) {
            return 关系值;
        }
    }
    return 取数字字段(方法实体.内容, "服务收益修正", 0.0);
}

inline double 计算风险成本修正(const 结构_元实体& 方法实体, const 结构_元关系* 绑定关系)
{
    if (绑定关系) {
        const double 关系值 = 取数字字段(绑定关系->内容, "平均风险成本修正", std::numeric_limits<double>::quiet_NaN());
        if (!std::isnan(关系值)) {
            return 关系值;
        }
    }
    return 取数字字段(方法实体.内容, "风险成本修正", 0.0);
}

inline double 计算服务值投影收益(
    const 结构_元实体& 任务实体,
    const 结构_元实体& 方法实体,
    const 结构_元关系* 绑定关系)
{
    double 投影收益 = std::numeric_limits<double>::quiet_NaN();
    if (绑定关系) {
        投影收益 = 取数字字段(绑定关系->内容, "服务值投影收益",
            取数字字段(绑定关系->内容, "平均服务收益修正", std::numeric_limits<double>::quiet_NaN()));
    }
    if (std::isnan(投影收益)) {
        投影收益 = 取数字字段(方法实体.内容, "服务值投影收益",
            取数字字段(方法实体.内容, "服务收益修正", 0.0));
    }
    return 投影收益 * 计算服务偏好系数(任务实体);
}

inline double 计算安全值投影风险(
    const 结构_元实体& 任务实体,
    const 结构_元实体& 方法实体,
    const 结构_元关系* 绑定关系)
{
    double 投影风险 = std::numeric_limits<double>::quiet_NaN();
    if (绑定关系) {
        投影风险 = 取数字字段(绑定关系->内容, "安全值投影风险",
            取数字字段(绑定关系->内容, "平均风险成本修正", std::numeric_limits<double>::quiet_NaN()));
    }
    if (std::isnan(投影风险)) {
        投影风险 = 取数字字段(方法实体.内容, "安全值投影风险",
            取数字字段(方法实体.内容, "风险成本修正", 0.0));
    }
    return 投影风险 * 计算安全风险权重(任务实体);
}

inline double 计算方法得分(
    const 结构_元实体& 任务实体,
    const std::vector<std::string>& 任务标签,
    const 结构_元实体& 方法实体,
    const 结构_元关系* 绑定关系)
{
    const auto 任务类型 = 取任务类型(任务实体);
    const auto 方法任务类型 = 取方法适用任务类型(方法实体);
    const bool 类型匹配 = 方法任务类型.empty() || 任务类型.empty() || 方法任务类型 == 任务类型;
    if (!类型匹配) {
        return -1e9;
    }

    const auto 方法标签 = 取实体标签列表(方法实体, "适用标签");
    std::unordered_set<std::string> 任务标签集合(任务标签.begin(), 任务标签.end());
    std::size_t 命中标签数 = 0;
    for (const auto& 标签 : 方法标签) {
        if (任务标签集合.contains(标签)) {
            ++命中标签数;
        }
    }
    if (!任务标签.empty() && !方法标签.empty() && 命中标签数 == 0) {
        return -1e9;
    }

    const double 基础适配度 = 取数字字段(方法实体.内容, "基础适配度", 0.0)
        + (类型匹配 && !任务类型.empty() && !方法任务类型.empty() ? 0.5 : 0.0)
        + static_cast<double>(命中标签数) * 0.25;
    const double 服务值投影收益 = 计算服务值投影收益(任务实体, 方法实体, 绑定关系);
    const double 安全值投影风险 = 计算安全值投影风险(任务实体, 方法实体, 绑定关系);
    return 基础适配度
        + 计算历史成功率修正(方法实体, 绑定关系)
        + 服务值投影收益
        - 安全值投影风险;
}
}

export class 方法管理模块_v0
{
public:
    explicit 方法管理模块_v0(元层接口& 元层);

    bool 注册方法(const 结构_元实体& 方法实体);

    std::vector<结构_元实体> 查询候选方法(
        const std::string& 任务标识,
        const std::vector<std::string>& 任务标签) const;

    std::optional<结构_元实体> 为任务选择最佳方法(
        const std::string& 任务标识,
        const std::vector<std::string>& 任务标签) const;

    std::optional<结构_元实体> 为任务选择候补方法(
        const std::string& 任务标识,
        const std::vector<std::string>& 任务标签,
        const std::string& 排除方法标识) const;

    bool 绑定方法到任务(
        const std::string& 任务标识,
        const std::string& 方法标识,
        const std::string& 原因);

    bool 替换任务方法(
        const std::string& 任务标识,
        const std::string& 旧方法标识,
        const std::string& 新方法标识,
        const std::string& 原因);

    bool 记录方法执行结果(
        const std::string& 方法标识,
        bool 是否成功,
        const std::string& 反馈摘要);

private:
    元层接口& 元层_;
};

inline 方法管理模块_v0::方法管理模块_v0(元层接口& 元层)
    : 元层_(元层)
{
}

inline bool 方法管理模块_v0::注册方法(const 结构_元实体& 方法实体)
{
    结构_元实体 待写入 = 方法实体;
    if (待写入.标识.empty()) {
        待写入.标识 = 生成元层标识("method");
    }
    待写入.类型 = 枚举_元实体类型::方法;
    if (待写入.状态 == 枚举_元对象状态::新建) {
        待写入.状态 = 枚举_元对象状态::待处理;
    }
    if (!待写入.内容.contains("基础适配度")) {
        待写入.内容.emplace("基础适配度", "0");
    }
    if (!待写入.内容.contains("历史成功次数")) {
        待写入.内容.emplace("历史成功次数", "0");
    }
    if (!待写入.内容.contains("历史失败次数")) {
        待写入.内容.emplace("历史失败次数", "0");
    }
    if (!待写入.内容.contains("服务收益修正")) {
        待写入.内容.emplace("服务收益修正", "0");
    }
    if (!待写入.内容.contains("风险成本修正")) {
        待写入.内容.emplace("风险成本修正", "0");
    }
    if (!待写入.内容.contains("服务值投影收益")) {
        待写入.内容.emplace("服务值投影收益", 待写入.内容["服务收益修正"]);
    }
    if (!待写入.内容.contains("安全值投影风险")) {
        待写入.内容.emplace("安全值投影风险", 待写入.内容["风险成本修正"]);
    }
    return 元层_.写实体(待写入);
}

inline std::vector<结构_元实体> 方法管理模块_v0::查询候选方法(
    const std::string& 任务标识,
    const std::vector<std::string>& 任务标签) const
{
    std::vector<结构_元实体> 结果;
    const auto 任务实体 = 元层_.读实体(任务标识);
    if (!任务实体.has_value() || 任务实体->类型 != 枚举_元实体类型::任务 || 任务实体->是否失效) {
        return 结果;
    }

    auto 合并任务标签 = 任务标签;
    const auto 实体标签 = 方法管理模块_v0_内部::取实体标签列表(*任务实体, "任务标签");
    合并任务标签.insert(合并任务标签.end(), 实体标签.begin(), 实体标签.end());

    std::vector<std::pair<double, 结构_元实体>> 带分候选;
    const auto 绑定关系集 = 元层_.查询关系(任务标识, 枚举_元关系类型::绑定, false);
    for (const auto& 方法实体 : 元层_.查询实体(枚举_元实体类型::方法, false)) {
        if (方法实体.是否失效) {
            continue;
        }
        const auto* 绑定关系 = 方法管理模块_v0_内部::查找激活绑定关系(绑定关系集, 方法实体.标识);
        const double 评分 = 方法管理模块_v0_内部::计算方法得分(*任务实体, 合并任务标签, 方法实体, 绑定关系);
        if (评分 <= -1e8) {
            continue;
        }
        auto 带摘要实体 = 方法实体;
        带摘要实体.内容["当前候选评分"] = std::to_string(评分);
        带摘要实体.内容["当前风险权重"] = std::to_string(方法管理模块_v0_内部::计算安全风险权重(*任务实体));
        带摘要实体.内容["当前服务偏好系数"] = std::to_string(方法管理模块_v0_内部::计算服务偏好系数(*任务实体));
        带摘要实体.内容["当前服务值投影收益"] = std::to_string(方法管理模块_v0_内部::计算服务值投影收益(*任务实体, 方法实体, 绑定关系));
        带摘要实体.内容["当前安全值投影风险"] = std::to_string(方法管理模块_v0_内部::计算安全值投影风险(*任务实体, 方法实体, 绑定关系));
        带摘要实体.内容["当前自检强度"] = std::to_string(方法管理模块_v0_内部::计算方法选择自检强度(*任务实体));
        带分候选.emplace_back(评分, std::move(带摘要实体));
    }

    std::sort(带分候选.begin(), 带分候选.end(), [](const auto& 左, const auto& 右) {
        return 左.first > 右.first;
    });
    for (auto& [_, 实体] : 带分候选) {
        结果.push_back(std::move(实体));
    }
    return 结果;
}

inline std::optional<结构_元实体> 方法管理模块_v0::为任务选择最佳方法(
    const std::string& 任务标识,
    const std::vector<std::string>& 任务标签) const
{
    auto 候选 = 查询候选方法(任务标识, 任务标签);
    if (候选.empty()) {
        return std::nullopt;
    }
    return 候选.front();
}

inline std::optional<结构_元实体> 方法管理模块_v0::为任务选择候补方法(
    const std::string& 任务标识,
    const std::vector<std::string>& 任务标签,
    const std::string& 排除方法标识) const
{
    auto 候选 = 查询候选方法(任务标识, 任务标签);
    for (auto& 方法 : 候选) {
        if (方法.标识 != 排除方法标识) {
            return 方法;
        }
    }
    return std::nullopt;
}

inline bool 方法管理模块_v0::绑定方法到任务(
    const std::string& 任务标识,
    const std::string& 方法标识,
    const std::string& 原因)
{
    const auto 任务实体 = 元层_.读实体(任务标识);
    const auto 方法实体 = 元层_.读实体(方法标识);
    if (!任务实体.has_value() || 任务实体->类型 != 枚举_元实体类型::任务 || 任务实体->是否失效) {
        return false;
    }
    if (!方法实体.has_value() || 方法实体->类型 != 枚举_元实体类型::方法 || 方法实体->是否失效) {
        return false;
    }

    for (auto 关系 : 元层_.查询关系(任务标识, 枚举_元关系类型::绑定, false)) {
        if (关系.终点标识 != 方法标识 || 关系.是否失效) {
            continue;
        }
        关系.内容["最近绑定原因"] = 原因;
        关系.内容["最近绑定时间"] = std::to_string(元层当前时间());
        return 元层_.写关系(关系);
    }

    结构_元关系 绑定关系{};
    绑定关系.标识 = 生成元层标识("bind");
    绑定关系.起点标识 = 任务标识;
    绑定关系.终点标识 = 方法标识;
    绑定关系.类型 = 枚举_元关系类型::绑定;
    绑定关系.状态 = 枚举_元对象状态::待处理;
    绑定关系.内容 = {
        {"原因", 原因},
        {"最近绑定时间", std::to_string(元层当前时间())},
        {"成功次数", "0"},
        {"失败次数", "0"},
        {"平均服务收益修正", "0"},
        {"平均风险成本修正", "0"}
    };
    return 元层_.写关系(绑定关系);
}

inline bool 方法管理模块_v0::替换任务方法(
    const std::string& 任务标识,
    const std::string& 旧方法标识,
    const std::string& 新方法标识,
    const std::string& 原因)
{
    for (const auto& 关系 : 元层_.查询关系(任务标识, 枚举_元关系类型::绑定, false)) {
        if (关系.终点标识 != 旧方法标识 || 关系.是否失效) {
            continue;
        }
        (void)元层_.失效关系(关系.标识, "替换任务方法:" + 原因);
    }

    if (!绑定方法到任务(任务标识, 新方法标识, 原因)) {
        return false;
    }

    结构_元关系 替代关系{};
    替代关系.标识 = 生成元层标识("replace");
    替代关系.起点标识 = 新方法标识;
    替代关系.终点标识 = 旧方法标识;
    替代关系.类型 = 枚举_元关系类型::替代;
    替代关系.状态 = 枚举_元对象状态::已完成;
    替代关系.内容 = {
        {"任务标识", 任务标识},
        {"原因", 原因}
    };
    (void)元层_.写关系(替代关系);
    return true;
}

inline bool 方法管理模块_v0::记录方法执行结果(
    const std::string& 方法标识,
    bool 是否成功,
    const std::string& 反馈摘要)
{
    auto 方法实体 = 元层_.读实体(方法标识);
    if (!方法实体.has_value() || 方法实体->类型 != 枚举_元实体类型::方法 || 方法实体->是否失效) {
        return false;
    }

    auto 待更新 = *方法实体;
    auto 成功次数 = static_cast<std::int64_t>(方法管理模块_v0_内部::取数字字段(待更新.内容, "历史成功次数", 0.0));
    auto 失败次数 = static_cast<std::int64_t>(方法管理模块_v0_内部::取数字字段(待更新.内容, "历史失败次数", 0.0));
    if (是否成功) {
        ++成功次数;
    }
    else {
        ++失败次数;
    }
    const auto 总次数 = 成功次数 + 失败次数;
    const double 成功率 = 总次数 > 0
        ? static_cast<double>(成功次数) / static_cast<double>(总次数)
        : 0.0;

    待更新.内容["历史成功次数"] = std::to_string(成功次数);
    待更新.内容["历史失败次数"] = std::to_string(失败次数);
    待更新.内容["历史执行次数"] = std::to_string(总次数);
    待更新.内容["历史成功率"] = std::to_string(成功率);
    待更新.内容["最近反馈摘要"] = 反馈摘要;
    待更新.内容["最近执行是否成功"] = 是否成功 ? "1" : "0";
    待更新.内容["最近执行时间"] = std::to_string(元层当前时间());
    return 元层_.写实体(待更新);
}
