// 文件规则：概念图服务只编排四类概念根和概念专用关系；不得用名称、日志、显示或索引候选裁决概念事实。
#pragma once

#include "../核心/关系仓库.h"
#include "../核心/容错检查.h"
#include "二次特征服务.h"
#include "动态服务.h"
#include "因果服务.h"
#include "概念图算法.h"
#include "特征服务.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <vector>

namespace 海中鱼巣 {

struct 概念根登记材料 {
    概念根类别 根类别 = 概念根类别::未定义;
    std::uint64_t 稳定非名称键 = 0;
    节点句柄 根节点;
};

class 概念图服务 {
public:
    概念图服务(const 节点仓库& 节点, 关系仓库& 关系)
        : 节点_(节点), 关系_(关系) {
    }

    std::optional<节点句柄> 登记概念根(const 概念根登记材料& 材料) {
        const auto 根记录 = 节点_.读取节点(材料.根节点);
        const auto 节点类别 = 根记录.has_value() ? 节点类型对应根类别(根记录->类型) : std::nullopt;
        if (!根类别有效(材料.根类别)
            || 材料.稳定非名称键 == 0
            || !节点类别.has_value()
            || 节点类别.value() != 材料.根类别) {
            return std::nullopt;
        }

        std::lock_guard<std::mutex> 图锁(图写锁_);
        if (!关系_.获取来源节点组(材料.根节点, 关系类型::概念上下位).empty()) {
            return std::nullopt;
        }

        std::unique_lock<std::shared_mutex> 根锁(根登记锁_);
        for (const auto& 已登记 : 根登记组_) {
            if (!已登记.has_value()) {
                continue;
            }
            if (已登记->根类别 == 材料.根类别) {
                if (已登记->稳定非名称键 == 材料.稳定非名称键 && 已登记->根节点 == 材料.根节点) {
                    return 已登记->根节点;
                }
                return std::nullopt;
            }
            if (已登记->稳定非名称键 == 材料.稳定非名称键 || 已登记->根节点 == 材料.根节点) {
                return std::nullopt;
            }
        }

        根登记组_[根类别索引(材料.根类别)] = 材料;
        const auto& 已发布 = 根登记组_[根类别索引(材料.根类别)];
        if (!追根因检查(已发布.has_value()
            && 已发布->根类别 == 材料.根类别
            && 已发布->稳定非名称键 == 材料.稳定非名称键
            && 已发布->根节点 == 材料.根节点,
            L"概念根登记发布后读回不符合内部预期。")) {
            return std::nullopt;
        }
        return 已发布->根节点;
    }

    std::optional<概念根登记材料> 读取概念根(概念根类别 类别) const {
        if (!根类别有效(类别)) {
            return std::nullopt;
        }
        std::shared_lock<std::shared_mutex> 锁(根登记锁_);
        const auto& 材料 = 根登记组_[根类别索引(类别)];
        if (!材料.has_value() || !节点_.节点是否有效(材料->根节点)) {
            return std::nullopt;
        }
        return 材料;
    }

    std::vector<概念根登记材料> 读取全部概念根() const {
        std::vector<概念根登记材料> 结果;
        std::shared_lock<std::shared_mutex> 锁(根登记锁_);
        结果.reserve(根登记组_.size());
        for (const auto& 材料 : 根登记组_) {
            if (材料.has_value() && 节点_.节点是否有效(材料->根节点)) {
                结果.push_back(材料.value());
            }
        }
        return 结果;
    }

    bool 节点是否已登记概念根(节点句柄 节点) const {
        if (!节点_.节点是否有效(节点)) {
            return false;
        }
        std::shared_lock<std::shared_mutex> 锁(根登记锁_);
        return std::any_of(根登记组_.begin(), 根登记组_.end(), [&节点](const auto& 材料) {
            return 材料.has_value() && 材料->根节点 == 节点;
        });
    }

    std::optional<概念根类别> 读取节点概念类别(节点句柄 节点) const {
        const auto 记录 = 节点_.读取节点(节点);
        if (!记录.has_value()) {
            return std::nullopt;
        }
        return 节点类型对应根类别(记录->类型);
    }

    bool 绑定实例支持概念(节点句柄 实例, 节点句柄 概念) {
        std::lock_guard<std::mutex> 图锁(图写锁_);
        const auto 实例类别 = 读取节点概念类别(实例);
        const auto 概念类别 = 读取节点概念类别(概念);
        if (实例 == 概念
            || !实例类别.has_value()
            || !概念类别.has_value()
            || 实例类别.value() != 概念类别.value()
            || 节点是否已登记概念根(实例)) {
            return false;
        }
        return 幂等创建专用关系(关系类型::实例支持概念, 实例, 概念);
    }

    std::vector<节点句柄> 读取实例支持概念(节点句柄 实例) const {
        return 关系_.获取目标节点组(实例, 关系类型::实例支持概念);
    }

    std::vector<节点句柄> 读取支持概念的实例(节点句柄 概念) const {
        return 关系_.获取来源节点组(概念, 关系类型::实例支持概念);
    }

    std::optional<节点句柄> 确保存在实例根支持(节点句柄 存在实例) {
        const auto 实例记录 = 节点_.读取节点(存在实例);
        if (!实例记录.has_value()
            || 实例记录->类型 != 节点类型::存在) {
            return std::nullopt;
        }
        return 确保实例支持对应根(存在实例, 概念根类别::存在);
    }

    std::optional<节点句柄> 确保动态实例根支持(节点句柄 动态实例, const 动态服务& 动态) {
        const auto 完成材料 = 动态.读取动态实例完成材料(动态实例);
        if (!完成材料.has_value() || 完成材料->动态 != 动态实例) {
            return std::nullopt;
        }
        return 确保实例支持对应根(动态实例, 概念根类别::动态);
    }

    std::optional<节点句柄> 确保关系实例根支持(
        节点句柄 关系实例,
        const 二次特征服务& 二次特征) {
        const auto 完成材料 = 二次特征.读取关系实例完成材料(关系实例);
        if (!完成材料.has_value() || 完成材料->二次特征 != 关系实例) {
            return std::nullopt;
        }
        return 确保实例支持对应根(关系实例, 概念根类别::关系);
    }

    std::optional<节点句柄> 确保因果实例根支持(
        节点句柄 因果实例,
        节点句柄 来源动态,
        const 因果服务& 因果,
        const 动态服务& 动态) {
        const auto 完成材料 = 因果.读取因果实例完成材料(因果实例, 来源动态, 动态);
        if (!完成材料.has_value()
            || 完成材料->因果 != 因果实例
            || 完成材料->来源动态 != 来源动态) {
            return std::nullopt;
        }
        return 确保实例支持对应根(因果实例, 概念根类别::因果);
    }

    std::optional<概念候选输入材料> 构造存在概念候选输入(
        节点句柄 存在实例,
        const 特征服务& 特征) const {
        const auto 实例记录 = 节点_.读取节点(存在实例);
        const auto 特征约束组 = 特征.读取宿主概念特征约束组(存在实例);
        if (!实例记录.has_value()
            || 实例记录->类型 != 节点类型::存在
            || !实例已支持对应根(存在实例, 概念根类别::存在)
            || !特征约束组.has_value()
            || 特征约束组->empty()) {
            return std::nullopt;
        }
        概念候选输入材料 输入;
        输入.签名.类别 = 概念根类别::存在;
        输入.来源实例 = 存在实例;
        for (const auto& 特征约束 : 特征约束组.value()) {
            const auto 约束 = 构造特征原始值约束(特征约束);
            if (!约束.has_value()) {
                return std::nullopt;
            }
            输入.签名.约束组.push_back(约束.value());
            输入.定义材料组.push_back(特征约束.特征定义);
        }
        return 规范化候选输入(输入);
    }

    std::optional<概念候选输入材料> 构造动态概念候选输入(
        节点句柄 动态实例,
        const 动态服务& 动态) const {
        const auto 完成材料 = 动态.读取动态实例完成材料(动态实例);
        if (!完成材料.has_value()
            || !实例已支持对应根(动态实例, 概念根类别::动态)) {
            return std::nullopt;
        }
        const auto 目标记录 = 节点_.读取节点(完成材料->被改变目标);
        if (!目标记录.has_value()) {
            return std::nullopt;
        }
        概念候选输入材料 输入;
        输入.签名.类别 = 概念根类别::动态;
        输入.来源实例 = 动态实例;
        输入.签名.约束组.push_back(构造节点类型约束(
            概念约束维度::动态目标类型, 0, 目标记录->类型));
        输入.签名.约束组.push_back(构造I64约束(
            概念约束维度::动态改变前值, 0, 完成材料->改变前I64));
        输入.签名.约束组.push_back(构造I64约束(
            概念约束维度::动态改变后值, 0, 完成材料->改变后I64));
        输入.定义材料组 = {
            完成材料->被改变目标,
            完成材料->改变前值,
            完成材料->改变后值
        };
        if (句柄有效(完成材料->来源动作)) {
            const auto 动作记录 = 节点_.读取节点(完成材料->来源动作);
            if (!动作记录.has_value()) {
                return std::nullopt;
            }
            输入.签名.约束组.push_back(构造节点类型约束(
                概念约束维度::动态来源动作类型, 0, 动作记录->类型));
            输入.定义材料组.push_back(完成材料->来源动作);
        }
        return 规范化候选输入(输入);
    }

    std::optional<概念候选输入材料> 构造关系概念候选输入(
        节点句柄 关系实例,
        const 二次特征服务& 二次特征) const {
        const auto 完成材料 = 二次特征.读取关系实例完成材料(关系实例);
        if (!完成材料.has_value()
            || !实例已支持对应根(关系实例, 概念根类别::关系)) {
            return std::nullopt;
        }
        概念候选输入材料 输入;
        输入.签名.类别 = 概念根类别::关系;
        输入.来源实例 = 关系实例;
        输入.定义材料组 = 完成材料->组成项组;
        for (const auto& 角色 : 完成材料->角色候选组) {
            输入.签名.约束组.push_back(构造节点类型约束(
                概念约束维度::关系角色类型, 角色.组成顺序, 角色.组成项类型));
        }
        return 规范化候选输入(输入);
    }

    std::optional<概念候选输入材料> 构造因果概念候选输入(
        节点句柄 因果实例,
        节点句柄 来源动态,
        const 因果服务& 因果,
        const 动态服务& 动态) const {
        const auto 完成材料 = 因果.读取因果实例完成材料(因果实例, 来源动态, 动态);
        if (!完成材料.has_value()
            || !实例已支持对应根(因果实例, 概念根类别::因果)) {
            return std::nullopt;
        }
        const auto 目标记录 = 节点_.读取节点(完成材料->来源动态材料.被改变目标);
        if (!目标记录.has_value()) {
            return std::nullopt;
        }
        概念候选输入材料 输入;
        输入.签名.类别 = 概念根类别::因果;
        输入.来源实例 = 因果实例;
        输入.来源动态 = 来源动态;
        输入.签名.约束组.push_back(构造节点类型约束(
            概念约束维度::因果动态目标类型, 0, 目标记录->类型));
        输入.签名.约束组.push_back(构造I64约束(
            概念约束维度::因果动态改变前值, 0, 完成材料->来源动态材料.改变前I64));
        输入.签名.约束组.push_back(构造I64约束(
            概念约束维度::因果动态改变后值, 0, 完成材料->来源动态材料.改变后I64));
        输入.定义材料组 = {
            完成材料->来源动态材料.被改变目标,
            完成材料->来源动态材料.改变前值,
            完成材料->来源动态材料.改变后值
        };
        if (句柄有效(完成材料->来源动态材料.来源动作)) {
            const auto 动作记录 = 节点_.读取节点(完成材料->来源动态材料.来源动作);
            if (!动作记录.has_value()) {
                return std::nullopt;
            }
            输入.签名.约束组.push_back(构造节点类型约束(
                概念约束维度::因果来源动作类型, 0, 动作记录->类型));
            输入.定义材料组.push_back(完成材料->来源动态材料.来源动作);
        }
        return 规范化候选输入(输入);
    }

    概念图候选版本材料 计算固定点候选图(
        std::uint64_t 基准活动版本,
        const std::vector<概念候选输入材料>& 输入组,
        std::size_t 候选数量预算,
        std::size_t 迭代预算) const {
        std::vector<概念候选根材料> 根材料组;
        const auto 已登记根组 = 读取全部概念根();
        if (已登记根组.size() != 4) {
            return {};
        }
        根材料组.reserve(已登记根组.size());
        for (const auto& 根 : 已登记根组) {
            根材料组.push_back({根.根类别, 根.根节点});
        }
        return 概念图算法::计算固定点候选(
            基准活动版本, 根材料组, 输入组, 候选数量预算, 迭代预算);
    }

    std::vector<节点句柄> 计算受影响分量(节点句柄 起点概念) const {
        std::vector<节点句柄> 结果;
        const auto 类别 = 读取节点概念类别(起点概念);
        if (!类别.has_value() || !节点是已知概念(起点概念)) {
            return 结果;
        }
        std::vector<节点句柄> 待检查{起点概念};
        while (!待检查.empty()) {
            const auto 当前 = 待检查.back();
            待检查.pop_back();
            if (std::find(结果.begin(), 结果.end(), 当前) != 结果.end()) {
                continue;
            }
            const auto 当前类别 = 读取节点概念类别(当前);
            if (!当前类别.has_value() || 当前类别.value() != 类别.value()) {
                continue;
            }
            结果.push_back(当前);
            const auto 上位组 = 读取概念直接上位(当前);
            const auto 下位组 = 读取概念直接下位(当前);
            待检查.insert(待检查.end(), 上位组.begin(), 上位组.end());
            待检查.insert(待检查.end(), 下位组.begin(), 下位组.end());
        }
        std::sort(结果.begin(), 结果.end(), 节点句柄小于);
        return 结果;
    }

    bool 绑定概念上下位(节点句柄 上位概念, 节点句柄 下位概念) {
        std::lock_guard<std::mutex> 图锁(图写锁_);
        const auto 上位类别 = 读取节点概念类别(上位概念);
        const auto 下位类别 = 读取节点概念类别(下位概念);
        if (上位概念 == 下位概念
            || !上位类别.has_value()
            || !下位类别.has_value()
            || 上位类别.value() != 下位类别.value()
            || !节点是已知概念(上位概念)
            || !节点是已知概念(下位概念)) {
            return false;
        }
        if (关系_.存在关系(关系类型::概念上下位, 上位概念, 下位概念)) {
            return true;
        }
        if (沿概念下位可达(下位概念, 上位概念)) {
            return false;
        }
        return 幂等创建专用关系(关系类型::概念上下位, 上位概念, 下位概念);
    }

    std::vector<节点句柄> 读取概念直接上位(节点句柄 概念) const {
        return 关系_.获取来源节点组(概念, 关系类型::概念上下位);
    }

    std::vector<节点句柄> 读取概念直接下位(节点句柄 概念) const {
        return 关系_.获取目标节点组(概念, 关系类型::概念上下位);
    }

    bool 绑定概念定义材料(节点句柄 概念, 节点句柄 定义材料) {
        std::lock_guard<std::mutex> 图锁(图写锁_);
        const auto 材料记录 = 节点_.读取节点(定义材料);
        if (概念 == 定义材料
            || !节点是已知概念(概念)
            || !材料记录.has_value()
            || !定义材料类型允许(材料记录->类型)) {
            return false;
        }
        return 幂等创建专用关系(关系类型::概念定义材料, 概念, 定义材料);
    }

    std::vector<节点句柄> 读取概念定义材料(节点句柄 概念) const {
        return 关系_.获取目标节点组(概念, 关系类型::概念定义材料);
    }

private:
    static bool 节点句柄小于(const 节点句柄& 左, const 节点句柄& 右) {
        if (左.仓库编号 != 右.仓库编号) {
            return 左.仓库编号 < 右.仓库编号;
        }
        if (左.节点编号 != 右.节点编号) {
            return 左.节点编号 < 右.节点编号;
        }
        return 左.版本号 < 右.版本号;
    }

    static 概念约束材料 构造I64约束(
        概念约束维度 维度,
        std::uint64_t 角色序号,
        std::int64_t 值) {
        概念约束材料 约束;
        约束.维度 = 维度;
        约束.角色序号 = 角色序号;
        约束.值类型 = 概念约束值类型::I64;
        约束.I64值 = 值;
        return 约束;
    }

    static 概念约束材料 构造节点类型约束(
        概念约束维度 维度,
        std::uint64_t 角色序号,
        节点类型 类型) {
        概念约束材料 约束;
        约束.维度 = 维度;
        约束.角色序号 = 角色序号;
        约束.值类型 = 概念约束值类型::节点类型值;
        约束.节点类型值 = 类型;
        return 约束;
    }

    static std::optional<概念约束材料> 构造特征原始值约束(const 概念特征约束材料& 材料) {
        if (!材料.完整()) {
            return std::nullopt;
        }
        概念约束材料 约束;
        约束.维度 = 概念约束维度::特征原始值;
        约束.定义节点 = 材料.特征定义;
        switch (材料.原始类型) {
        case 特征值原始类型::I64:
            约束.值类型 = 概念约束值类型::I64;
            约束.I64值 = 材料.I64值.value();
            break;
        case 特征值原始类型::VecI64:
            约束.值类型 = 概念约束值类型::VecI64;
            约束.VecI64值 = 材料.VecI64值;
            break;
        case 特征值原始类型::VecU64:
            约束.值类型 = 概念约束值类型::VecU64;
            约束.VecU64值 = 材料.VecU64值;
            break;
        default:
            return std::nullopt;
        }
        return 约束.完整() ? std::optional<概念约束材料>{约束} : std::nullopt;
    }

    static std::optional<概念候选输入材料> 规范化候选输入(概念候选输入材料 输入) {
        const auto 规范签名 = 概念图算法::规范化概念签名(输入.签名);
        if (!规范签名.has_value() || 规范签名->约束组.empty() || !句柄有效(输入.来源实例)) {
            return std::nullopt;
        }
        输入.签名 = 规范签名.value();
        std::sort(输入.定义材料组.begin(), 输入.定义材料组.end(), 节点句柄小于);
        输入.定义材料组.erase(
            std::remove_if(输入.定义材料组.begin(), 输入.定义材料组.end(), [](const auto& 材料) {
                return !句柄有效(材料);
            }),
            输入.定义材料组.end());
        输入.定义材料组.erase(
            std::unique(输入.定义材料组.begin(), 输入.定义材料组.end()),
            输入.定义材料组.end());
        return 输入;
    }

    bool 实例已支持对应根(节点句柄 实例, 概念根类别 类别) const {
        const auto 根材料 = 读取概念根(类别);
        if (!根材料.has_value()) {
            return false;
        }
        const auto 支持组 = 读取实例支持概念(实例);
        return std::find(支持组.begin(), 支持组.end(), 根材料->根节点) != 支持组.end();
    }

    static bool 根类别有效(概念根类别 类别) {
        return 类别 >= 概念根类别::存在 && 类别 <= 概念根类别::因果;
    }

    static std::size_t 根类别索引(概念根类别 类别) {
        return static_cast<std::size_t>(类别) - 1;
    }

    static std::optional<概念根类别> 节点类型对应根类别(节点类型 类型) {
        switch (类型) {
        case 节点类型::存在:
            return 概念根类别::存在;
        case 节点类型::动态:
            return 概念根类别::动态;
        case 节点类型::二次特征:
            return 概念根类别::关系;
        case 节点类型::因果引用:
            return 概念根类别::因果;
        default:
            return std::nullopt;
        }
    }

    static bool 定义材料类型允许(节点类型 类型) {
        return 类型 == 节点类型::特征
            || 类型 == 节点类型::状态
            || 类型 == 节点类型::存在
            || 类型 == 节点类型::动态
            || 类型 == 节点类型::二次特征
            || 类型 == 节点类型::因果引用;
    }

    bool 节点是已知概念(节点句柄 节点) const {
        return 节点是否已登记概念根(节点)
            || !关系_.获取来源节点组(节点, 关系类型::实例支持概念).empty()
            || !关系_.获取来源节点组(节点, 关系类型::概念上下位).empty()
            || !关系_.获取目标节点组(节点, 关系类型::概念上下位).empty()
            || !关系_.获取目标节点组(节点, 关系类型::概念定义材料).empty();
    }

    bool 沿概念下位可达(节点句柄 起点, 节点句柄 目标) const {
        std::vector<节点句柄> 待检查{起点};
        std::vector<节点句柄> 已检查;
        while (!待检查.empty()) {
            const auto 当前 = 待检查.back();
            待检查.pop_back();
            if (当前 == 目标) {
                return true;
            }
            if (std::find(已检查.begin(), 已检查.end(), 当前) != 已检查.end()) {
                continue;
            }
            已检查.push_back(当前);
            const auto 下位组 = 关系_.获取目标节点组(当前, 关系类型::概念上下位);
            待检查.insert(待检查.end(), 下位组.begin(), 下位组.end());
        }
        return false;
    }

    bool 幂等创建专用关系(关系类型 类型, 节点句柄 源节点, 节点句柄 目标节点) {
        if (关系_.存在关系(类型, 源节点, 目标节点)) {
            return true;
        }
        const auto 新关系 = 关系_.创建关系(类型, 源节点, 目标节点);
        return 追根因检查(句柄有效(新关系) && 关系_.存在关系(类型, 源节点, 目标节点),
            L"概念图专用关系写入后读回不符合内部预期。");
    }

    std::optional<节点句柄> 确保实例支持对应根(节点句柄 实例, 概念根类别 类别) {
        const auto 实例类别 = 读取节点概念类别(实例);
        const auto 根材料 = 读取概念根(类别);
        if (!实例类别.has_value()
            || 实例类别.value() != 类别
            || !根材料.has_value()
            || 实例 == 根材料->根节点) {
            return std::nullopt;
        }
        if (!绑定实例支持概念(实例, 根材料->根节点)) {
            return std::nullopt;
        }
        const auto 实例目标组 = 读取实例支持概念(实例);
        const auto 根来源组 = 读取支持概念的实例(根材料->根节点);
        const bool 双向可读 = std::find(实例目标组.begin(), 实例目标组.end(), 根材料->根节点) != 实例目标组.end()
            && std::find(根来源组.begin(), 根来源组.end(), 实例) != 根来源组.end();
        if (!追根因检查(双向可读, L"确保实例支持对应根后双向读回不符合内部预期。")) {
            return std::nullopt;
        }
        return 根材料->根节点;
    }

    const 节点仓库& 节点_;
    关系仓库& 关系_;
    mutable std::shared_mutex 根登记锁_;
    std::array<std::optional<概念根登记材料>, 4> 根登记组_;
    std::mutex 图写锁_;
};

}
