module;

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

export module 学习写回模块_v0;

import 元层核心模块;

export struct 结构_学习写回结果
{
    bool 成功 = false;
    std::string 错误摘要;
    std::string 反馈标识;
    std::string 经验标识;
    std::string 适配关系标识;
};

export class 学习写回模块_v0
{
public:
    explicit 学习写回模块_v0(元层接口& 元层);

    bool 处理反馈并学习(
        const std::string& 任务标识,
        const std::string& 方法标识,
        const std::string& 反馈标识);

    结构_学习写回结果 写入反馈并学习写回(
        const std::string& 任务标识,
        const std::string& 方法标识,
        bool 是否成功,
        元字段表 反馈内容 = {},
        std::string 原因 = "学习写回");

private:
    bool 写回任务统计(const std::string& 任务标识, bool 是否成功);
    bool 写回方法统计(const std::string& 方法标识, bool 是否成功);
    bool 写回任务方法适配关系(
        const std::string& 任务标识,
        const std::string& 方法标识,
        bool 是否成功);

    bool 更新任务方法适配关系(
        const std::string& 任务标识,
        const std::string& 方法标识,
        bool 是否成功,
        const 元字段表& 反馈内容,
        std::string* 关系标识输出 = nullptr);

    结构_学习写回结果 私有_处理反馈实体并学习(
        const std::string& 任务标识,
        const std::string& 方法标识,
        const 结构_元实体& 反馈实体,
        std::string 原因 = "学习写回");

private:
    元层接口& 元层_;
};

namespace 学习写回模块_v0_内部 {
inline std::string 取字段_(const 元字段表& 字段, std::string_view 键)
{
    if (const auto it = 字段.find(std::string(键)); it != 字段.end()) {
        return it->second;
    }
    return {};
}

inline double 取数字字段_(const 元字段表& 字段, std::string_view 键, double 默认值 = 0.0)
{
    const auto 值 = 取字段_(字段, 键);
    if (值.empty()) {
        return 默认值;
    }
    try {
        return std::stod(值);
    }
    catch (...) {
        return 默认值;
    }
}

inline bool 取布尔字段_(const 元字段表& 字段, std::string_view 键, bool 默认值 = false)
{
    const auto 值 = 取字段_(字段, 键);
    if (值.empty()) {
        return 默认值;
    }
    if (值 == "1" || 值 == "true" || 值 == "TRUE" || 值 == "成功") {
        return true;
    }
    if (值 == "0" || 值 == "false" || 值 == "FALSE" || 值 == "失败") {
        return false;
    }
    return 默认值;
}

inline std::optional<结构_元关系> 查找绑定关系_(
    元层接口& 元层,
    const std::string& 任务标识,
    const std::string& 方法标识)
{
    for (const auto& 关系 : 元层.查询关系(任务标识, 枚举_元关系类型::绑定, false)) {
        if (关系.终点标识 == 方法标识 && !关系.是否失效) {
            return 关系;
        }
    }
    return std::nullopt;
}

inline bool 确保任务方法绑定_(
    元层接口& 元层,
    const std::string& 任务标识,
    const std::string& 方法标识,
    std::string_view 原因)
{
    for (auto 关系 : 元层.查询关系(任务标识, 枚举_元关系类型::绑定, true)) {
        if (关系.终点标识 != 方法标识 || 关系.是否失效) {
            continue;
        }
        关系.内容["最近绑定原因"] = std::string(原因);
        关系.内容["最近绑定时间"] = std::to_string(元层当前时间());
        return 元层.写关系(关系);
    }

    结构_元关系 绑定关系{};
    绑定关系.标识 = 生成元层标识("bind");
    绑定关系.起点标识 = 任务标识;
    绑定关系.终点标识 = 方法标识;
    绑定关系.类型 = 枚举_元关系类型::绑定;
    绑定关系.状态 = 枚举_元对象状态::待处理;
    绑定关系.内容 = {
        {"原因", std::string(原因)},
        {"最近绑定时间", std::to_string(元层当前时间())},
        {"成功次数", "0"},
        {"失败次数", "0"},
        {"总次数", "0"},
        {"成功率", "0"},
        {"累计服务收益修正", "0"},
        {"累计风险成本修正", "0"},
        {"平均服务收益修正", "0"},
        {"平均风险成本修正", "0"},
        {"适配得分", "0"}
    };
    return 元层.写关系(绑定关系);
}

inline bool 确保反馈关系_(
    元层接口& 元层,
    const std::string& 反馈标识,
    const std::string& 任务标识,
    const std::string& 方法标识,
    std::string_view 原因)
{
    bool 有任务边 = false;
    bool 有方法边 = false;
    for (const auto& 关系 : 元层.查询关系(反馈标识, 枚举_元关系类型::反馈到, false)) {
        if (关系.终点标识 == 任务标识) {
            有任务边 = true;
        }
        if (关系.终点标识 == 方法标识) {
            有方法边 = true;
        }
    }

    if (!有任务边) {
        结构_元关系 反馈到任务{};
        反馈到任务.标识 = 生成元层标识("rel");
        反馈到任务.起点标识 = 反馈标识;
        反馈到任务.终点标识 = 任务标识;
        反馈到任务.类型 = 枚举_元关系类型::反馈到;
        反馈到任务.状态 = 枚举_元对象状态::已完成;
        反馈到任务.内容 = {
            {"原因", std::string(原因)}
        };
        if (!元层.写关系(反馈到任务)) {
            return false;
        }
    }

    if (!有方法边) {
        结构_元关系 反馈到方法{};
        反馈到方法.标识 = 生成元层标识("rel");
        反馈到方法.起点标识 = 反馈标识;
        反馈到方法.终点标识 = 方法标识;
        反馈到方法.类型 = 枚举_元关系类型::反馈到;
        反馈到方法.状态 = 枚举_元对象状态::已完成;
        反馈到方法.内容 = {
            {"原因", std::string(原因)}
        };
        if (!元层.写关系(反馈到方法)) {
            return false;
        }
    }

    return true;
}

inline bool 写入反馈实体_(
    元层接口& 元层,
    const std::string& 任务标识,
    const std::string& 方法标识,
    bool 是否成功,
    元字段表 反馈内容,
    结构_元实体& 反馈实体输出)
{
    结构_元实体 反馈实体{};
    反馈实体.标识 = 生成元层标识("feedback");
    反馈实体.类型 = 枚举_元实体类型::反馈;
    反馈实体.状态 = 枚举_元对象状态::已完成;
    反馈实体.内容 = std::move(反馈内容);
    反馈实体.内容["任务标识"] = 任务标识;
    反馈实体.内容["方法标识"] = 方法标识;
    反馈实体.内容["是否成功"] = 是否成功 ? "1" : "0";
    if (!反馈实体.内容.contains("反馈摘要")) {
        反馈实体.内容["反馈摘要"] = 是否成功 ? "执行成功" : "执行失败";
    }
    if (!元层.写实体(反馈实体)) {
        return false;
    }
    反馈实体输出 = std::move(反馈实体);
    return true;
}

inline bool 写入经验实体_(
    元层接口& 元层,
    const std::string& 任务标识,
    const std::string& 方法标识,
    const 结构_元实体& 反馈实体,
    const std::string& 适配关系标识,
    结构_元实体& 经验实体输出)
{
    结构_元实体 经验实体{};
    经验实体.标识 = 生成元层标识("exp");
    经验实体.类型 = 枚举_元实体类型::经验;
    经验实体.状态 = 枚举_元对象状态::已完成;
    经验实体.内容 = {
        {"任务标识", 任务标识},
        {"方法标识", 方法标识},
        {"反馈标识", 反馈实体.标识},
        {"学习摘要", 取字段_(反馈实体.内容, "反馈摘要")},
        {"是否成功", 取字段_(反馈实体.内容, "是否成功")},
        {"适配关系标识", 适配关系标识}
    };
    if (!元层.写实体(经验实体)) {
        return false;
    }
    经验实体输出 = std::move(经验实体);
    return true;
}

inline bool 写入学习回写关系_(
    元层接口& 元层,
    const std::string& 经验标识,
    const std::string& 反馈标识,
    const std::string& 任务标识,
    const std::string& 方法标识)
{
    结构_元关系 写回到任务{};
    写回到任务.标识 = 生成元层标识("rel");
    写回到任务.起点标识 = 经验标识;
    写回到任务.终点标识 = 任务标识;
    写回到任务.类型 = 枚举_元关系类型::学习回写;
    写回到任务.状态 = 枚举_元对象状态::已完成;
    写回到任务.内容 = {
        {"反馈标识", 反馈标识}
    };
    if (!元层.写关系(写回到任务)) {
        return false;
    }

    结构_元关系 写回到方法{};
    写回到方法.标识 = 生成元层标识("rel");
    写回到方法.起点标识 = 经验标识;
    写回到方法.终点标识 = 方法标识;
    写回到方法.类型 = 枚举_元关系类型::学习回写;
    写回到方法.状态 = 枚举_元对象状态::已完成;
    写回到方法.内容 = {
        {"反馈标识", 反馈标识}
    };
    return 元层.写关系(写回到方法);
}

inline bool 记录学习写回事务事件_(
    元层接口& 元层,
    const std::string& 经验标识,
    const std::string& 任务标识,
    const std::string& 方法标识,
    const std::string& 反馈标识,
    const std::string& 适配关系标识)
{
    结构_元事件 学习事件{};
    学习事件.标识 = 生成元层标识("event");
    学习事件.事件类型 = "学习写回";
    学习事件.对象标识 = 经验标识;
    学习事件.内容 = {
        {"任务标识", 任务标识},
        {"方法标识", 方法标识},
        {"反馈标识", 反馈标识},
        {"适配关系标识", 适配关系标识}
    };
    return 元层.记录事件(学习事件);
}
}

inline 学习写回模块_v0::学习写回模块_v0(元层接口& 元层)
    : 元层_(元层)
{
}

inline bool 学习写回模块_v0::处理反馈并学习(
    const std::string& 任务标识,
    const std::string& 方法标识,
    const std::string& 反馈标识)
{
    const auto 反馈实体 = 元层_.读实体(反馈标识);
    if (!反馈实体.has_value()
        || 反馈实体->类型 != 枚举_元实体类型::反馈
        || 反馈实体->是否失效) {
        return false;
    }
    return 私有_处理反馈实体并学习(任务标识, 方法标识, *反馈实体, "处理反馈并学习").成功;
}

inline 结构_学习写回结果 学习写回模块_v0::写入反馈并学习写回(
    const std::string& 任务标识,
    const std::string& 方法标识,
    bool 是否成功,
    元字段表 反馈内容,
    std::string 原因)
{
    结构_元实体 反馈实体{};
    if (!学习写回模块_v0_内部::写入反馈实体_(元层_, 任务标识, 方法标识, 是否成功, std::move(反馈内容), 反馈实体)) {
        return { false, "写入反馈实体失败" };
    }
    return 私有_处理反馈实体并学习(任务标识, 方法标识, 反馈实体, std::move(原因));
}

inline bool 学习写回模块_v0::写回任务统计(const std::string& 任务标识, bool 是否成功)
{
    auto 任务实体 = 元层_.读实体(任务标识);
    if (!任务实体.has_value()
        || 任务实体->类型 != 枚举_元实体类型::任务
        || 任务实体->是否失效) {
        return false;
    }

    auto 待更新 = *任务实体;
    auto 成功次数 = static_cast<std::int64_t>(学习写回模块_v0_内部::取数字字段_(待更新.内容, "历史成功次数", 0.0));
    auto 失败次数 = static_cast<std::int64_t>(学习写回模块_v0_内部::取数字字段_(待更新.内容, "历史失败次数", 0.0));
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
    待更新.内容["最近执行是否成功"] = 是否成功 ? "1" : "0";
    待更新.内容["最近学习时间"] = std::to_string(元层当前时间());
    return 元层_.写实体(待更新);
}

inline bool 学习写回模块_v0::写回方法统计(const std::string& 方法标识, bool 是否成功)
{
    auto 方法实体 = 元层_.读实体(方法标识);
    if (!方法实体.has_value()
        || 方法实体->类型 != 枚举_元实体类型::方法
        || 方法实体->是否失效) {
        return false;
    }

    auto 待更新 = *方法实体;
    auto 成功次数 = static_cast<std::int64_t>(学习写回模块_v0_内部::取数字字段_(待更新.内容, "历史成功次数", 0.0));
    auto 失败次数 = static_cast<std::int64_t>(学习写回模块_v0_内部::取数字字段_(待更新.内容, "历史失败次数", 0.0));
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
    待更新.内容["最近执行是否成功"] = 是否成功 ? "1" : "0";
    待更新.内容["最近学习时间"] = std::to_string(元层当前时间());
    return 元层_.写实体(待更新);
}

inline bool 学习写回模块_v0::写回任务方法适配关系(
    const std::string& 任务标识,
    const std::string& 方法标识,
    bool 是否成功)
{
    return 更新任务方法适配关系(任务标识, 方法标识, 是否成功, {});
}

inline bool 学习写回模块_v0::更新任务方法适配关系(
    const std::string& 任务标识,
    const std::string& 方法标识,
    bool 是否成功,
    const 元字段表& 反馈内容,
    std::string* 关系标识输出)
{
    if (!学习写回模块_v0_内部::确保任务方法绑定_(元层_, 任务标识, 方法标识, "学习写回确保适配关系")) {
        return false;
    }

    auto 绑定关系 = 学习写回模块_v0_内部::查找绑定关系_(元层_, 任务标识, 方法标识);
    if (!绑定关系.has_value()) {
        return false;
    }

    auto 待更新 = *绑定关系;
    auto 成功次数 = static_cast<std::int64_t>(学习写回模块_v0_内部::取数字字段_(待更新.内容, "成功次数", 0.0));
    auto 失败次数 = static_cast<std::int64_t>(学习写回模块_v0_内部::取数字字段_(待更新.内容, "失败次数", 0.0));
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
    const double 累计服务收益 = 学习写回模块_v0_内部::取数字字段_(待更新.内容, "累计服务收益修正", 0.0)
        + 学习写回模块_v0_内部::取数字字段_(反馈内容, "服务收益修正", 学习写回模块_v0_内部::取数字字段_(反馈内容, "服务收益", 0.0));
    const double 累计风险成本 = 学习写回模块_v0_内部::取数字字段_(待更新.内容, "累计风险成本修正", 0.0)
        + 学习写回模块_v0_内部::取数字字段_(反馈内容, "风险成本修正", 学习写回模块_v0_内部::取数字字段_(反馈内容, "风险成本", 0.0));
    const double 平均服务收益 = 总次数 > 0 ? 累计服务收益 / static_cast<double>(总次数) : 0.0;
    const double 平均风险成本 = 总次数 > 0 ? 累计风险成本 / static_cast<double>(总次数) : 0.0;
    const double 适配得分 = 成功率 + 平均服务收益 - 平均风险成本;

    待更新.内容["成功次数"] = std::to_string(成功次数);
    待更新.内容["失败次数"] = std::to_string(失败次数);
    待更新.内容["总次数"] = std::to_string(总次数);
    待更新.内容["成功率"] = std::to_string(成功率);
    待更新.内容["累计服务收益修正"] = std::to_string(累计服务收益);
    待更新.内容["累计风险成本修正"] = std::to_string(累计风险成本);
    待更新.内容["平均服务收益修正"] = std::to_string(平均服务收益);
    待更新.内容["平均风险成本修正"] = std::to_string(平均风险成本);
    待更新.内容["适配得分"] = std::to_string(适配得分);
    待更新.内容["最近反馈摘要"] = 学习写回模块_v0_内部::取字段_(反馈内容, "反馈摘要");
    待更新.内容["最近学习时间"] = std::to_string(元层当前时间());

    if (!元层_.写关系(待更新)) {
        return false;
    }
    if (关系标识输出) {
        *关系标识输出 = 待更新.标识;
    }
    return true;
}

inline 结构_学习写回结果 学习写回模块_v0::私有_处理反馈实体并学习(
    const std::string& 任务标识,
    const std::string& 方法标识,
    const 结构_元实体& 反馈实体,
    std::string 原因)
{
    结构_学习写回结果 结果{};
    const auto 任务实体 = 元层_.读实体(任务标识);
    const auto 方法实体 = 元层_.读实体(方法标识);
    if (!任务实体.has_value()
        || 任务实体->类型 != 枚举_元实体类型::任务
        || 任务实体->是否失效) {
        结果.错误摘要 = "任务不存在或无效";
        return 结果;
    }
    if (!方法实体.has_value()
        || 方法实体->类型 != 枚举_元实体类型::方法
        || 方法实体->是否失效) {
        结果.错误摘要 = "方法不存在或无效";
        return 结果;
    }

    const bool 是否成功 = 学习写回模块_v0_内部::取布尔字段_(反馈实体.内容, "是否成功", false);
    if (!学习写回模块_v0_内部::确保任务方法绑定_(元层_, 任务标识, 方法标识, 原因)) {
        结果.错误摘要 = "补建任务-方法绑定失败";
        return 结果;
    }
    if (!学习写回模块_v0_内部::确保反馈关系_(元层_, 反馈实体.标识, 任务标识, 方法标识, 原因)) {
        结果.错误摘要 = "补建反馈关系失败";
        return 结果;
    }
    if (!写回任务统计(任务标识, 是否成功)) {
        结果.错误摘要 = "写回任务统计失败";
        return 结果;
    }
    if (!写回方法统计(方法标识, 是否成功)) {
        结果.错误摘要 = "写回方法统计失败";
        return 结果;
    }

    std::string 适配关系标识;
    if (!更新任务方法适配关系(任务标识, 方法标识, 是否成功, 反馈实体.内容, &适配关系标识)) {
        结果.错误摘要 = "写回任务-方法适配关系失败";
        return 结果;
    }

    结构_元实体 经验实体{};
    if (!学习写回模块_v0_内部::写入经验实体_(元层_, 任务标识, 方法标识, 反馈实体, 适配关系标识, 经验实体)) {
        结果.错误摘要 = "写入经验实体失败";
        return 结果;
    }
    if (!学习写回模块_v0_内部::写入学习回写关系_(元层_, 经验实体.标识, 反馈实体.标识, 任务标识, 方法标识)) {
        结果.错误摘要 = "写入学习回写关系失败";
        return 结果;
    }
    if (!学习写回模块_v0_内部::记录学习写回事务事件_(元层_, 经验实体.标识, 任务标识, 方法标识, 反馈实体.标识, 适配关系标识)) {
        结果.错误摘要 = "记录学习写回事件失败";
        return 结果;
    }

    if (auto 最新任务实体 = 元层_.读实体(任务标识); 最新任务实体.has_value() && !最新任务实体->是否失效) {
        auto 待更新任务 = *最新任务实体;
        待更新任务.内容["最近反馈标识"] = 反馈实体.标识;
        待更新任务.内容["最近反馈摘要"] = 学习写回模块_v0_内部::取字段_(反馈实体.内容, "反馈摘要");
        (void)元层_.写实体(待更新任务);
    }
    if (auto 最新方法实体 = 元层_.读实体(方法标识); 最新方法实体.has_value() && !最新方法实体->是否失效) {
        auto 待更新方法 = *最新方法实体;
        待更新方法.内容["最近反馈标识"] = 反馈实体.标识;
        待更新方法.内容["最近反馈摘要"] = 学习写回模块_v0_内部::取字段_(反馈实体.内容, "反馈摘要");
        (void)元层_.写实体(待更新方法);
    }

    结果.成功 = true;
    结果.反馈标识 = 反馈实体.标识;
    结果.经验标识 = 经验实体.标识;
    结果.适配关系标识 = 适配关系标识;
    return 结果;
}
