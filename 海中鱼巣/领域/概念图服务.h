// 文件规则：概念图服务只编排四类概念根和概念专用关系；不得用名称、日志、显示或索引候选裁决概念事实。
#pragma once

#include "../核心/关系仓库.h"
#include "../核心/容错检查.h"
#include "二次特征服务.h"
#include "动态服务.h"
#include "因果服务.h"
#include "概念图算法.h"
#include "存在服务.h"
#include "状态服务.h"
#include "统计服务.h"
#include "特征服务.h"
#include "语素服务.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <utility>
#include <vector>

namespace 海中鱼巣 {

struct 概念根登记材料 {
    概念根类别 根类别 = 概念根类别::未定义;
    std::uint64_t 稳定非名称键 = 0;
    节点句柄 根节点;
};

enum class 概念生命周期阶段 : std::uint32_t {
    未定义 = 0,
    活跃 = 1,
    冷却 = 2,
    退役 = 3
};

enum class 概念待命名原因 : std::uint32_t {
    未定义 = 0,
    名称入口为空 = 1
};

struct 概念生命周期材料 {
    节点句柄 概念;
    概念生命周期阶段 阶段 = 概念生命周期阶段::未定义;
    节点句柄 状态;
    关系句柄 关系;

    bool 完整() const {
        return 句柄有效(概念)
            && 阶段 >= 概念生命周期阶段::活跃
            && 阶段 <= 概念生命周期阶段::退役
            && 句柄有效(状态)
            && 句柄有效(关系);
    }
};

struct 概念待命名请求材料 {
    节点句柄 概念;
    概念根类别 根类别 = 概念根类别::未定义;
    std::uint64_t 活动图版本 = 0;
    概念待命名原因 原因 = 概念待命名原因::未定义;

    bool 完整() const {
        return 句柄有效(概念)
            && 根类别 >= 概念根类别::存在
            && 根类别 <= 概念根类别::因果
            && 活动图版本 != 0
            && 原因 == 概念待命名原因::名称入口为空;
    }
};

struct 概念临时指代材料 {
    std::uint64_t 表达上下文编号 = 0;
    std::uint64_t 指代序号 = 0;
    节点句柄 概念;
    std::uint64_t 活动图版本 = 0;

    bool 完整() const {
        return 表达上下文编号 != 0
            && 指代序号 != 0
            && 句柄有效(概念)
            && 活动图版本 != 0;
    }
};

enum class 概念生命周期复核建议 : std::uint32_t {
    未定义 = 0,
    保持活跃 = 1,
    建议冷却 = 2,
    建议退役复核 = 3,
    建议重新激活 = 4,
    材料不足 = 5
};

struct 概念生命周期复核规则 {
    std::uint64_t 规则版本 = 0;
    std::uint64_t 当前时间戳 = 0;
    std::uint64_t 最近使用截止时间戳 = 0;
    std::uint64_t 退役复核截止时间戳 = 0;

    bool 完整() const {
        const auto 上限 = static_cast<std::uint64_t>(std::numeric_limits<std::int64_t>::max());
        return 规则版本 != 0
            && 当前时间戳 != 0
            && 最近使用截止时间戳 != 0
            && 退役复核截止时间戳 != 0
            && 当前时间戳 <= 上限
            && 最近使用截止时间戳 <= 当前时间戳
            && 退役复核截止时间戳 <= 最近使用截止时间戳;
    }
};

struct 概念生命周期复核候选 {
    缓存命名空间 命名空间;
    节点句柄 概念;
    std::uint64_t 活动图版本 = 0;
    概念生命周期阶段 当前阶段 = 概念生命周期阶段::未定义;
    std::uint64_t 规则版本 = 0;
    概念生命周期复核建议 建议 = 概念生命周期复核建议::未定义;
    std::uint64_t 观察次数候选 = 0;
    std::uint64_t 最近观察时间戳 = 0;
    std::int64_t 累计结果效用候选 = 0;
    std::uint64_t 活动实例支持数量 = 0;
    std::uint64_t 外部引用数量 = 0;

    bool 完整() const {
        return 命名空间.类型 == 缓存类型::概念用途观察
            && 命名空间.来源 == 来源结构类型::概念
            && 命名空间.来源版本 == 活动图版本
            && 命名空间.规则版本 != 0
            && 命名空间.值类型 == 缓存值类型::用途观察
            && !命名空间.可持久化
            && 句柄有效(概念)
            && 活动图版本 != 0
            && 当前阶段 >= 概念生命周期阶段::活跃
            && 当前阶段 <= 概念生命周期阶段::退役
            && 规则版本 != 0
            && 建议 >= 概念生命周期复核建议::保持活跃
            && 建议 <= 概念生命周期复核建议::材料不足
            && ((观察次数候选 == 0 && 最近观察时间戳 == 0)
                || (观察次数候选 != 0 && 最近观察时间戳 != 0));
    }
};

struct 概念活动关系材料 {
    关系句柄 关系;
    关系类型 类型 = 关系类型::普通父子;
    节点句柄 源节点;
    节点句柄 目标节点;

    bool 完整() const {
        return 句柄有效(关系)
            && 句柄有效(源节点)
            && 句柄有效(目标节点)
            && (类型 == 关系类型::实例支持概念
                || 类型 == 关系类型::概念上下位
                || 类型 == 关系类型::概念定义材料);
    }
};

struct 概念活动签名材料 {
    节点句柄 概念;
    概念签名材料 签名;
};

struct 抽象树投影项 {
    节点句柄 概念;
    概念根类别 根类别 = 概念根类别::未定义;
    std::vector<节点句柄> 路径;
    std::size_t 深度 = 0;
    bool 重复投影 = false;
    std::vector<节点句柄> 名称入口组;

    bool 完整() const {
        return 句柄有效(概念)
            && 根类别 >= 概念根类别::存在
            && 根类别 <= 概念根类别::因果
            && !路径.empty()
            && 路径.back() == 概念
            && 深度 + 1 == 路径.size();
    }
};

struct 抽象树视图材料 {
    std::uint64_t 活动版本 = 0;
    概念根类别 根类别 = 概念根类别::未定义;
    节点句柄 根概念;
    std::vector<抽象树投影项> 投影项组;

    bool 完整() const {
        return 活动版本 != 0
            && 根类别 >= 概念根类别::存在
            && 根类别 <= 概念根类别::因果
            && 句柄有效(根概念)
            && !投影项组.empty()
            && 投影项组.front().概念 == 根概念
            && std::all_of(投影项组.begin(), 投影项组.end(), [](const auto& 项) {
                return 项.完整();
            });
    }
};

class 概念图服务 {
public:
    概念图服务(节点仓库& 节点, 关系仓库& 关系)
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

    std::optional<节点句柄> 登记生命周期状态(
        概念生命周期阶段 阶段,
        节点句柄 状态节点,
        const 状态服务& 状态) {
        const auto 状态记录 = 节点_.读取节点(状态节点);
        const auto 状态值 = 状态.读取状态值(状态节点);
        if (!生命周期阶段有效(阶段)
            || !状态记录.has_value()
            || 状态记录->类型 != 节点类型::状态
            || !状态值.has_value()
            || 状态值.value() != static_cast<std::int64_t>(阶段)
            || 状态.状态是否实例状态(状态节点)) {
            return std::nullopt;
        }

        std::unique_lock<std::shared_mutex> 锁(生命周期状态锁_);
        const auto 索引 = 生命周期阶段索引(阶段);
        const auto& 已登记 = 生命周期状态组_[索引];
        if (已登记.has_value()) {
            return 已登记.value() == 状态节点 ? 已登记 : std::nullopt;
        }
        for (const auto& 其他状态 : 生命周期状态组_) {
            if (其他状态.has_value() && 其他状态.value() == 状态节点) {
                return std::nullopt;
            }
        }
        生命周期状态组_[索引] = 状态节点;
        const auto& 已发布 = 生命周期状态组_[索引];
        if (!追根因检查(已发布.has_value() && 已发布.value() == 状态节点,
            L"概念生命周期状态登记发布后读回不符合内部预期。")) {
            return std::nullopt;
        }
        return 已发布;
    }

    std::optional<节点句柄> 读取生命周期状态(概念生命周期阶段 阶段) const {
        if (!生命周期阶段有效(阶段)) {
            return std::nullopt;
        }
        std::shared_lock<std::shared_mutex> 锁(生命周期状态锁_);
        const auto& 状态 = 生命周期状态组_[生命周期阶段索引(阶段)];
        if (!状态.has_value() || !节点_.节点是否有效(状态.value())) {
            return std::nullopt;
        }
        return 状态;
    }

    bool 初始化活动概念生命周期() {
        if (!生命周期状态登记完整()) {
            return false;
        }
        const auto 根组 = 读取全部概念根();
        if (根组.size() != 4) {
            return false;
        }

        std::unique_lock<std::shared_mutex> 活动锁(活动图锁_);
        std::lock_guard<std::mutex> 图锁(图写锁_);
        const auto 原关系数量 = 关系_.有效关系数量();
        std::vector<关系句柄> 本轮新增关系组;
        bool 全部就绪 = true;
        for (const auto& 根 : 根组) {
            if (!确保概念生命周期_已加锁(
                    根.根节点,
                    概念生命周期阶段::活跃,
                    true,
                    本轮新增关系组).has_value()) {
                全部就绪 = false;
                break;
            }
        }
        if (全部就绪) {
            for (const auto& 根 : 根组) {
                const auto 生命周期 = 读取概念生命周期_已加锁(根.根节点);
                if (!生命周期.has_value()
                    || 生命周期->阶段 != 概念生命周期阶段::活跃) {
                    全部就绪 = false;
                    break;
                }
            }
        }
        if (!全部就绪) {
            const bool 清理完成 = 清理生命周期关系_已加锁(本轮新增关系组);
            (void)追根因检查(
                清理完成 && 关系_.有效关系数量() == 原关系数量,
                L"概念根生命周期初始化失败后的关系清理不符合内部预期。");
            return false;
        }
        if (活动快照_.has_value()
            && !追根因检查(验证活动快照_已加锁(活动快照_.value()),
                L"概念根生命周期初始化后活动快照读回不符合内部预期。")) {
            return false;
        }
        return true;
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

    std::optional<std::uint64_t> 初始化活动图基础版本() {
        const auto 根组 = 读取全部概念根();
        if (根组.size() != 4) {
            return std::nullopt;
        }
        std::unique_lock<std::shared_mutex> 活动锁(活动图锁_);
        if (活动快照_.has_value()) {
            return 追根因检查(验证活动快照_已加锁(活动快照_.value()),
                L"概念活动图基础版本重复读取时生命周期或结构不完整。")
                ? std::optional<std::uint64_t>{活动快照_->活动版本}
                : std::nullopt;
        }
        std::lock_guard<std::mutex> 图锁(图写锁_);
        if (!生命周期状态登记完整()) {
            return std::nullopt;
        }
        for (const auto& 根 : 根组) {
            const auto 生命周期 = 读取概念生命周期_已加锁(根.根节点);
            if (!生命周期.has_value()
                || 生命周期->阶段 != 概念生命周期阶段::活跃) {
                return std::nullopt;
            }
        }
        概念活动快照 候选;
        候选.活动版本 = 1;
        for (const auto& 根 : 根组) {
            候选.根组.push_back(根.根节点);
            候选.概念节点组.push_back(根.根节点);
            候选.签名组.push_back({根.根节点, 概念签名材料{根.根类别, 1, {}}});
        }
        for (const auto& 登记 : 专用关系登记组_) {
            if (登记.类型 == 关系类型::实例支持概念
                && std::find(候选.根组.begin(), 候选.根组.end(), 登记.目标节点) != 候选.根组.end()) {
                候选.关系组.push_back({登记.关系, 登记.类型, 登记.源节点, 登记.目标节点});
            }
        }
        规范化活动快照(候选);
        if (!追根因检查(验证活动快照_已加锁(候选), L"概念活动图基础版本候选不符合内部预期。")) {
            return std::nullopt;
        }
        活动快照_ = 候选;
        return 活动快照_->活动版本;
    }

    std::uint64_t 读取活动图版本() const {
        std::shared_lock<std::shared_mutex> 锁(活动图锁_);
        return 活动快照_.has_value() ? 活动快照_->活动版本 : 0;
    }

    std::vector<节点句柄> 读取活动概念根组() const {
        std::shared_lock<std::shared_mutex> 锁(活动图锁_);
        return 活动快照_.has_value() ? 活动快照_->根组 : std::vector<节点句柄>{};
    }

    std::vector<节点句柄> 读取活动概念节点组() const {
        std::shared_lock<std::shared_mutex> 锁(活动图锁_);
        return 活动快照_.has_value() ? 活动快照_->概念节点组 : std::vector<节点句柄>{};
    }

    std::vector<概念活动关系材料> 读取活动关系组() const {
        std::shared_lock<std::shared_mutex> 锁(活动图锁_);
        return 活动快照_.has_value() ? 活动快照_->关系组 : std::vector<概念活动关系材料>{};
    }

    std::vector<节点句柄> 读取活动实例支持概念(节点句柄 实例) const {
        return 读取活动关系目标组(关系类型::实例支持概念, 实例);
    }

    std::vector<节点句柄> 读取活动支持概念的实例(节点句柄 概念) const {
        return 读取活动关系来源组(关系类型::实例支持概念, 概念);
    }

    std::vector<节点句柄> 读取活动概念直接上位(节点句柄 概念) const {
        return 读取活动关系来源组(关系类型::概念上下位, 概念);
    }

    std::vector<节点句柄> 读取活动概念直接下位(节点句柄 概念) const {
        return 读取活动关系目标组(关系类型::概念上下位, 概念);
    }

    std::vector<节点句柄> 读取活动概念定义材料(节点句柄 概念) const {
        return 读取活动关系目标组(关系类型::概念定义材料, 概念);
    }

    std::optional<概念签名材料> 读取活动概念签名(节点句柄 概念) const {
        std::shared_lock<std::shared_mutex> 锁(活动图锁_);
        if (!活动快照_.has_value()) {
            return std::nullopt;
        }
        for (const auto& 材料 : 活动快照_->签名组) {
            if (材料.概念 == 概念) {
                return 材料.签名;
            }
        }
        return std::nullopt;
    }

    std::optional<概念生命周期材料> 读取概念生命周期(节点句柄 概念) const {
        std::shared_lock<std::shared_mutex> 锁(活动图锁_);
        if (!读取节点概念类别(概念).has_value()) {
            return std::nullopt;
        }
        return 读取概念生命周期_已加锁(概念);
    }

    std::optional<概念待命名请求材料> 读取待命名请求(
        节点句柄 概念,
        const 语素服务& 语素) const {
        std::shared_lock<std::shared_mutex> 锁(活动图锁_);
        if (!活动快照_.has_value()) {
            return std::nullopt;
        }
        const auto 签名 = 从快照读取签名(活动快照_.value(), 概念);
        if (!签名.has_value()) {
            return std::nullopt;
        }
        if (!追根因检查(读取概念生命周期_已加锁(概念).has_value(),
            L"读取待命名请求时活动概念生命周期不完整。")) {
            return std::nullopt;
        }
        if (!语素.读取概念名称入口组(概念).empty()) {
            return std::nullopt;
        }
        概念待命名请求材料 材料{
            概念,
            签名->类别,
            活动快照_->活动版本,
            概念待命名原因::名称入口为空
        };
        return 材料.完整() ? std::optional<概念待命名请求材料>{材料} : std::nullopt;
    }

    std::optional<std::vector<概念待命名请求材料>> 读取全部待命名请求(
        const 语素服务& 语素) const {
        std::shared_lock<std::shared_mutex> 锁(活动图锁_);
        if (!活动快照_.has_value()) {
            return std::nullopt;
        }
        std::vector<概念待命名请求材料> 结果;
        for (const auto& 签名材料 : 活动快照_->签名组) {
            if (!追根因检查(读取概念生命周期_已加锁(签名材料.概念).has_value(),
                L"读取全部待命名请求时活动概念生命周期不完整。")) {
                return std::nullopt;
            }
            if (!语素.读取概念名称入口组(签名材料.概念).empty()) {
                continue;
            }
            概念待命名请求材料 材料{
                签名材料.概念,
                签名材料.签名.类别,
                活动快照_->活动版本,
                概念待命名原因::名称入口为空
            };
            if (!追根因检查(材料.完整(), L"概念待命名请求材料组装后不完整。")) {
                return std::nullopt;
            }
            结果.push_back(材料);
        }
        std::sort(结果.begin(), 结果.end(), [](const auto& 左, const auto& 右) {
            return 节点句柄小于(左.概念, 右.概念);
        });
        return 结果;
    }

    std::optional<概念临时指代材料> 构造临时指代(
        节点句柄 概念,
        std::uint64_t 预期活动图版本,
        std::uint64_t 表达上下文编号,
        std::uint64_t 指代序号) const {
        if (预期活动图版本 == 0 || 表达上下文编号 == 0 || 指代序号 == 0) {
            return std::nullopt;
        }
        std::shared_lock<std::shared_mutex> 锁(活动图锁_);
        if (!活动快照_.has_value()
            || 活动快照_->活动版本 != 预期活动图版本
            || !从快照读取签名(活动快照_.value(), 概念).has_value()
            || !读取概念生命周期_已加锁(概念).has_value()) {
            return std::nullopt;
        }
        概念临时指代材料 材料{
            表达上下文编号,
            指代序号,
            概念,
            活动快照_->活动版本
        };
        return 材料.完整() ? std::optional<概念临时指代材料>{材料} : std::nullopt;
    }

    std::optional<概念临时指代材料> 构造临时指代(
        节点句柄 概念,
        std::uint64_t 表达上下文编号,
        std::uint64_t 指代序号) const {
        return 构造临时指代(概念, 读取活动图版本(), 表达上下文编号, 指代序号);
    }

    std::optional<概念用途观察材料> 记录活动概念用途观察(
        缓存命名空间 命名空间,
        节点句柄 概念,
        概念用途类型 用途,
        std::uint64_t 观察时间戳,
        std::int64_t 结果效用变化,
        统计服务& 统计) const {
        if (!统计.概念用途观察命名空间有效(命名空间)
            || !句柄有效(概念)
            || !概念用途类型有效(用途)
            || 观察时间戳 == 0
            || 观察时间戳 > static_cast<std::uint64_t>(std::numeric_limits<std::int64_t>::max())
            || (用途 == 概念用途类型::一般 && 结果效用变化 != 0)) {
            return std::nullopt;
        }
        std::shared_lock<std::shared_mutex> 锁(活动图锁_);
        if (!活动快照_.has_value()
            || 命名空间.来源版本 != 活动快照_->活动版本) {
            return std::nullopt;
        }
        const auto 签名 = 从快照读取签名(活动快照_.value(), 概念);
        if (!签名.has_value()) {
            return std::nullopt;
        }
        if ((用途 == 概念用途类型::因果预测 || 用途 == 概念用途类型::因果行动)
            && 签名->类别 != 概念根类别::因果) {
            return std::nullopt;
        }
        if (!追根因检查(读取概念生命周期_已加锁(概念).has_value(),
            L"记录活动概念用途观察时生命周期不完整。")) {
            return std::nullopt;
        }
        const auto 记录 = 统计.记录概念用途观察(
            命名空间,
            概念,
            用途,
            观察时间戳,
            结果效用变化);
        if (!记录.has_value()) {
            return std::nullopt;
        }
        if (!追根因检查(
                记录->完整()
                    && 记录->概念 == 概念
                    && 记录->用途 == 用途
                    && 记录->最近观察时间戳 == 观察时间戳,
                L"活动概念用途观察写入后返回材料不符合内部预期。")) {
            return std::nullopt;
        }
        return 记录;
    }

    std::optional<概念生命周期复核候选> 读取生命周期复核候选(
        节点句柄 概念,
        缓存命名空间 命名空间,
        const 概念生命周期复核规则& 规则,
        const 统计服务& 统计) const {
        if (!句柄有效(概念)
            || !规则.完整()
            || !统计.概念用途观察命名空间有效(命名空间)) {
            return std::nullopt;
        }
        std::shared_lock<std::shared_mutex> 锁(活动图锁_);
        if (!活动快照_.has_value()
            || 命名空间.来源版本 != 活动快照_->活动版本) {
            return std::nullopt;
        }
        const auto 签名 = 从快照读取签名(活动快照_.value(), 概念);
        if (!签名.has_value()) {
            return std::nullopt;
        }
        const auto 生命周期 = 读取概念生命周期_已加锁(概念);
        if (!追根因检查(生命周期.has_value(), L"读取生命周期复核候选时概念生命周期不完整。")) {
            return std::nullopt;
        }

        const auto 活动支持数量 = static_cast<std::uint64_t>(std::count_if(
            活动快照_->关系组.begin(),
            活动快照_->关系组.end(),
            [&概念](const auto& 关系材料) {
                return 关系材料.类型 == 关系类型::实例支持概念
                    && 关系材料.目标节点 == 概念;
            }));
        const auto 外部引用数量 = static_cast<std::uint64_t>(读取外部引用数量(概念));
        概念生命周期复核候选 候选{
            命名空间,
            概念,
            活动快照_->活动版本,
            生命周期->阶段,
            规则.规则版本,
            概念生命周期复核建议::材料不足,
            0,
            0,
            0,
            活动支持数量,
            外部引用数量
        };

        if (命名空间.规则版本 != 规则.规则版本) {
            return 追根因检查(候选.完整(), L"规则版本变化候选组装后不完整。")
                ? std::optional<概念生命周期复核候选>{候选}
                : std::nullopt;
        }

        const auto 观察组 = 统计.读取概念用途观察组(命名空间, 概念);
        if (!追根因检查(观察组.has_value(), L"读取生命周期复核候选时用途缓存组不可读。")) {
            return std::nullopt;
        }
        bool 存在正向用途 = false;
        for (const auto& 观察 : 观察组.value()) {
            if ((观察.用途 == 概念用途类型::因果预测
                    || 观察.用途 == 概念用途类型::因果行动)
                && 签名->类别 != 概念根类别::因果) {
                return std::nullopt;
            }
            std::uint64_t 新次数 = 0;
            std::int64_t 新效用 = 0;
            if (!安全累加U64(候选.观察次数候选, 观察.观察次数候选, 新次数)
                || !安全累加I64(候选.累计结果效用候选, 观察.累计结果效用候选, 新效用)
                || 观察.最近观察时间戳 > 规则.当前时间戳) {
                return std::nullopt;
            }
            候选.观察次数候选 = 新次数;
            候选.累计结果效用候选 = 新效用;
            候选.最近观察时间戳 = std::max(
                候选.最近观察时间戳,
                观察.最近观察时间戳);
            存在正向用途 = 存在正向用途 || 观察.累计结果效用候选 > 0;
        }

        const bool 是根 = std::find(
            活动快照_->根组.begin(),
            活动快照_->根组.end(),
            概念) != 活动快照_->根组.end();
        const bool 有活性依据 = 是根
            || 候选.活动实例支持数量 != 0
            || 候选.外部引用数量 != 0
            || 存在正向用途
            || (候选.最近观察时间戳 != 0
                && 候选.最近观察时间戳 >= 规则.最近使用截止时间戳);

        switch (候选.当前阶段) {
        case 概念生命周期阶段::活跃:
            候选.建议 = 有活性依据
                ? 概念生命周期复核建议::保持活跃
                : (候选.观察次数候选 == 0
                    ? 概念生命周期复核建议::材料不足
                    : 概念生命周期复核建议::建议冷却);
            break;
        case 概念生命周期阶段::冷却:
            if (有活性依据) {
                候选.建议 = 概念生命周期复核建议::建议重新激活;
            }
            else if (候选.观察次数候选 == 0) {
                候选.建议 = 概念生命周期复核建议::材料不足;
            }
            else if (候选.最近观察时间戳 <= 规则.退役复核截止时间戳) {
                候选.建议 = 概念生命周期复核建议::建议退役复核;
            }
            else {
                候选.建议 = 概念生命周期复核建议::建议冷却;
            }
            break;
        case 概念生命周期阶段::退役:
            候选.建议 = 有活性依据
                ? 概念生命周期复核建议::建议重新激活
                : (候选.观察次数候选 == 0
                    ? 概念生命周期复核建议::材料不足
                    : 概念生命周期复核建议::建议退役复核);
            break;
        case 概念生命周期阶段::未定义:
        default:
            return std::nullopt;
        }

        if (!追根因检查(候选.完整(), L"概念生命周期复核候选组装后不完整。")) {
            return std::nullopt;
        }
        return 候选;
    }

    std::optional<std::uint64_t> 发布候选图版本(
        const 概念图候选版本材料& 候选图,
        存在服务& 存在,
        动态服务& 动态,
        二次特征服务& 二次特征,
        因果服务& 因果) {
        if (!初始化活动图基础版本().has_value()) {
            return std::nullopt;
        }
        std::unique_lock<std::shared_mutex> 活动锁(活动图锁_);
        if (!活动快照_.has_value()) {
            return std::nullopt;
        }
        if (上一候选图_.has_value() && 候选图内容等价(上一候选图_.value(), 候选图)) {
            return 追根因检查(验证活动快照_已加锁(活动快照_.value()),
                L"概念候选图重复发布时活动结构或生命周期不完整。")
                ? std::optional<std::uint64_t>{活动快照_->活动版本}
                : std::nullopt;
        }
        if (候选图.基准活动版本 != 活动快照_->活动版本
            || !候选图可发布_已加锁(候选图, 活动快照_.value())) {
            return std::nullopt;
        }

        std::lock_guard<std::mutex> 图锁(图写锁_);
        const auto 旧快照 = 活动快照_.value();
        const auto 原专用关系登记数量 = 专用关系登记组_.size();
        std::vector<节点句柄> 本轮新增节点组;
        std::vector<关系句柄> 本轮新增生命周期关系组;
        auto 新快照 = 构造候选活动快照_已加锁(
            候选图,
            旧快照,
            存在,
            动态,
            二次特征,
            因果,
            本轮新增节点组,
            本轮新增生命周期关系组);
        if (!新快照.has_value()) {
            (void)追根因检查(
                清理未发布候选_已加锁(
                    原专用关系登记数量,
                    本轮新增生命周期关系组,
                    本轮新增节点组),
                L"候选概念图构造失败后的新增结构清理不符合内部预期。");
            return std::nullopt;
        }
        if (!追根因检查(
                验证活动快照_已加锁(新快照.value()),
            L"候选概念图发布前完整读回不符合内部预期。")) {
            (void)追根因检查(
                清理未发布候选_已加锁(
                    原专用关系登记数量,
                    本轮新增生命周期关系组,
                    本轮新增节点组),
                L"候选概念图读回失败后的新增结构清理不符合内部预期。");
            return std::nullopt;
        }
        活动快照_ = 新快照.value();
        上一候选图_ = 候选图;
        if (!追根因检查(失效旧活动关系_已加锁(旧快照, 活动快照_.value()),
            L"活动图替换后旧关系失效不符合内部预期。")) {
            return std::nullopt;
        }
        return 活动快照_->活动版本;
    }

    std::optional<抽象树视图材料> 读取抽象树视图(
        节点句柄 根概念,
        std::size_t 节点预算 = 4096,
        std::size_t 深度预算 = 128) const {
        std::shared_lock<std::shared_mutex> 锁(活动图锁_);
        if (!活动快照_.has_value()) {
            return std::nullopt;
        }
        return 生成抽象树视图(活动快照_.value(), 根概念, 节点预算, 深度预算);
    }

    std::optional<std::vector<抽象树视图材料>> 读取全部抽象树视图(
        std::size_t 单树节点预算 = 4096,
        std::size_t 深度预算 = 128) const {
        std::shared_lock<std::shared_mutex> 锁(活动图锁_);
        if (!活动快照_.has_value() || 活动快照_->根组.size() != 4) {
            return std::nullopt;
        }
        std::vector<抽象树视图材料> 结果;
        结果.reserve(活动快照_->根组.size());
        for (const auto& 根 : 活动快照_->根组) {
            const auto 视图 = 生成抽象树视图(活动快照_.value(), 根, 单树节点预算, 深度预算);
            if (!视图.has_value()) {
                return std::nullopt;
            }
            结果.push_back(视图.value());
        }
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
    struct 专用关系登记材料 {
        关系句柄 关系;
        关系类型 类型 = 关系类型::普通父子;
        节点句柄 源节点;
        节点句柄 目标节点;
    };

    struct 生命周期关系登记材料 {
        关系句柄 关系;
        节点句柄 概念;
        节点句柄 状态;
    };

    struct 概念活动快照 {
        std::uint64_t 活动版本 = 0;
        std::vector<节点句柄> 根组;
        std::vector<节点句柄> 概念节点组;
        std::vector<概念活动关系材料> 关系组;
        std::vector<概念活动签名材料> 签名组;
    };

    struct 候选节点映射材料 {
        std::uint64_t 候选编号 = 0;
        节点句柄 概念;
    };

    struct 抽象树待投影材料 {
        节点句柄 概念;
        std::vector<节点句柄> 路径;
    };

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

    std::optional<概念候选输入材料> 规范化候选输入(概念候选输入材料 输入) const {
        const auto 规范签名 = 概念图算法::规范化概念签名(输入.签名);
        if (!规范签名.has_value() || 规范签名->约束组.empty() || !句柄有效(输入.来源实例)) {
            return std::nullopt;
        }
        输入.签名 = 规范签名.value();
        std::sort(输入.定义材料组.begin(), 输入.定义材料组.end(), 节点句柄小于);
        输入.定义材料组.erase(
            std::remove_if(输入.定义材料组.begin(), 输入.定义材料组.end(), [this](const auto& 材料) {
                const auto 记录 = 节点_.读取节点(材料);
                return !记录.has_value() || !定义材料类型允许(记录->类型);
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

    static bool 生命周期阶段有效(概念生命周期阶段 阶段) {
        return 阶段 >= 概念生命周期阶段::活跃
            && 阶段 <= 概念生命周期阶段::退役;
    }

    static std::size_t 生命周期阶段索引(概念生命周期阶段 阶段) {
        return static_cast<std::size_t>(阶段) - 1;
    }

    static bool 概念用途类型有效(概念用途类型 用途) {
        return 用途 >= 概念用途类型::一般
            && 用途 <= 概念用途类型::因果行动;
    }

    static bool 安全累加U64(std::uint64_t 当前, std::uint64_t 变化, std::uint64_t& 结果) {
        if (当前 > std::numeric_limits<std::uint64_t>::max() - 变化) {
            return false;
        }
        结果 = 当前 + 变化;
        return true;
    }

    static bool 安全累加I64(std::int64_t 当前, std::int64_t 变化, std::int64_t& 结果) {
        if ((变化 > 0 && 当前 > std::numeric_limits<std::int64_t>::max() - 变化)
            || (变化 < 0 && 当前 < std::numeric_limits<std::int64_t>::min() - 变化)) {
            return false;
        }
        结果 = 当前 + 变化;
        return true;
    }

    std::size_t 读取外部引用数量(节点句柄 概念) const {
        const std::array<关系类型, 9> 外部关系类型组{
            关系类型::普通父子,
            关系类型::归属,
            关系类型::引用,
            关系类型::索引,
            关系类型::模板,
            关系类型::运行期临时,
            关系类型::因果来源,
            关系类型::语素对应信息,
            关系类型::语素概念追溯
        };
        std::vector<std::pair<std::uint64_t, std::uint32_t>> 关系身份组;
        const auto 收集 = [&关系身份组](const std::vector<关系记录>& 记录组) {
            for (const auto& 记录 : 记录组) {
                const std::pair<std::uint64_t, std::uint32_t> 身份{记录.关系编号, 记录.版本号};
                if (std::find(关系身份组.begin(), 关系身份组.end(), 身份) == 关系身份组.end()) {
                    关系身份组.push_back(身份);
                }
            }
        };
        for (const auto 类型 : 外部关系类型组) {
            收集(关系_.获取关系记录组(概念, 类型));
            收集(关系_.获取来源关系记录组(概念, 类型));
        }
        return 关系身份组.size();
    }

    bool 生命周期状态登记完整() const {
        std::shared_lock<std::shared_mutex> 锁(生命周期状态锁_);
        std::vector<节点句柄> 状态组;
        状态组.reserve(生命周期状态组_.size());
        for (const auto& 状态 : 生命周期状态组_) {
            if (!状态.has_value() || !节点_.节点是否有效(状态.value())) {
                return false;
            }
            状态组.push_back(状态.value());
        }
        std::sort(状态组.begin(), 状态组.end(), 节点句柄小于);
        return std::adjacent_find(状态组.begin(), 状态组.end()) == 状态组.end();
    }

    std::optional<概念生命周期阶段> 读取生命周期阶段_已加锁(节点句柄 状态节点) const {
        std::shared_lock<std::shared_mutex> 锁(生命周期状态锁_);
        for (std::size_t 索引 = 0; 索引 < 生命周期状态组_.size(); ++索引) {
            const auto& 已登记 = 生命周期状态组_[索引];
            if (已登记.has_value()
                && 已登记.value() == 状态节点
                && 节点_.节点是否有效(状态节点)) {
                return static_cast<概念生命周期阶段>(索引 + 1);
            }
        }
        return std::nullopt;
    }

    std::optional<关系句柄> 确保概念生命周期_已加锁(
        节点句柄 概念,
        std::optional<概念生命周期阶段> 预期阶段,
        bool 允许创建,
        std::vector<关系句柄>& 本轮新增关系组) {
        if (!读取节点概念类别(概念).has_value()
            || (预期阶段.has_value() && !生命周期阶段有效(预期阶段.value()))) {
            return std::nullopt;
        }

        std::optional<节点句柄> 预期状态;
        if (预期阶段.has_value()) {
            预期状态 = 读取生命周期状态(预期阶段.value());
            if (!预期状态.has_value()) {
                return std::nullopt;
            }
        }

        const auto 记录组 = 关系_.获取关系记录组(概念, 关系类型::概念生命周期);
        if (记录组.size() > 1) {
            (void)追根因检查(false, L"概念生命周期出现多个有效目标。");
            return std::nullopt;
        }
        if (!记录组.empty()) {
            const auto& 记录 = 记录组.front();
            const auto 阶段 = 读取生命周期阶段_已加锁(记录.目标节点);
            if (!追根因检查(阶段.has_value(), L"概念生命周期关系指向未登记状态。")) {
                return std::nullopt;
            }
            if (预期阶段.has_value()
                && !追根因检查(阶段.value() == 预期阶段.value(),
                    L"概念生命周期关系阶段与当前写入路径预期不一致。")) {
                return std::nullopt;
            }
            const auto 匹配数量 = std::count_if(
                生命周期关系登记组_.begin(), 生命周期关系登记组_.end(),
                [this, &记录, &概念](const auto& 登记) {
                    if (登记.概念 != 概念 || 登记.状态 != 记录.目标节点) {
                        return false;
                    }
                    const auto 读回 = 关系_.读取关系(登记.关系);
                    return 读回.has_value()
                        && 读回->关系编号 == 记录.关系编号
                        && 读回->版本号 == 记录.版本号
                        && 读回->类型 == 关系类型::概念生命周期
                        && 读回->源节点 == 概念
                        && 读回->目标节点 == 记录.目标节点;
                });
            if (!追根因检查(匹配数量 == 1, L"概念生命周期关系存在但服务登记不唯一。")) {
                return std::nullopt;
            }
            const auto 已登记 = std::find_if(
                生命周期关系登记组_.begin(), 生命周期关系登记组_.end(),
                [this, &记录, &概念](const auto& 登记) {
                    if (登记.概念 != 概念 || 登记.状态 != 记录.目标节点) {
                        return false;
                    }
                    const auto 读回 = 关系_.读取关系(登记.关系);
                    return 读回.has_value()
                        && 读回->关系编号 == 记录.关系编号
                        && 读回->版本号 == 记录.版本号;
                });
            return 已登记 != 生命周期关系登记组_.end()
                ? std::optional<关系句柄>{已登记->关系}
                : std::nullopt;
        }

        const bool 有残留登记 = std::any_of(
            生命周期关系登记组_.begin(), 生命周期关系登记组_.end(),
            [&概念](const auto& 登记) { return 登记.概念 == 概念; });
        if (!允许创建 || !预期状态.has_value() || 有残留登记) {
            if (有残留登记) {
                (void)追根因检查(false, L"概念生命周期关系已失效但服务登记仍残留。");
            }
            return std::nullopt;
        }

        const auto 新关系 = 关系_.创建关系(关系类型::概念生命周期, 概念, 预期状态.value());
        const auto 读回 = 关系_.读取关系(新关系);
        if (!追根因检查(句柄有效(新关系)
            && 读回.has_value()
            && 读回->类型 == 关系类型::概念生命周期
            && 读回->源节点 == 概念
            && 读回->目标节点 == 预期状态.value(),
            L"概念生命周期关系写入后读回不符合内部预期。")) {
            if (句柄有效(新关系)) {
                (void)追根因检查(
                    关系_.删除关系(新关系) || !关系_.读取关系(新关系).has_value(),
                    L"概念生命周期关系写入读回失败后的清理不符合内部预期。");
            }
            return std::nullopt;
        }
        生命周期关系登记组_.push_back({新关系, 概念, 预期状态.value()});
        本轮新增关系组.push_back(新关系);
        return 新关系;
    }

    std::optional<概念生命周期材料> 读取概念生命周期_已加锁(节点句柄 概念) const {
        const auto 记录组 = 关系_.获取关系记录组(概念, 关系类型::概念生命周期);
        if (记录组.empty()) {
            return std::nullopt;
        }
        if (!追根因检查(记录组.size() == 1, L"读取概念生命周期时发现多个有效目标。")) {
            return std::nullopt;
        }
        const auto& 记录 = 记录组.front();
        const auto 阶段 = 读取生命周期阶段_已加锁(记录.目标节点);
        if (!追根因检查(阶段.has_value(), L"读取概念生命周期时目标状态未登记。")) {
            return std::nullopt;
        }
        std::optional<关系句柄> 关系句柄值;
        for (const auto& 登记 : 生命周期关系登记组_) {
            if (登记.概念 != 概念 || 登记.状态 != 记录.目标节点) {
                continue;
            }
            const auto 读回 = 关系_.读取关系(登记.关系);
            if (!读回.has_value()
                || 读回->关系编号 != 记录.关系编号
                || 读回->版本号 != 记录.版本号
                || 读回->类型 != 关系类型::概念生命周期
                || 读回->源节点 != 概念
                || 读回->目标节点 != 记录.目标节点
                || 关系句柄值.has_value()) {
                (void)追根因检查(false, L"读取概念生命周期时服务登记与权威关系不一致。");
                return std::nullopt;
            }
            关系句柄值 = 登记.关系;
        }
        if (!追根因检查(关系句柄值.has_value(), L"读取概念生命周期时缺少服务关系登记。")) {
            return std::nullopt;
        }
        概念生命周期材料 材料{概念, 阶段.value(), 记录.目标节点, 关系句柄值.value()};
        return 材料.完整() ? std::optional<概念生命周期材料>{材料} : std::nullopt;
    }

    bool 清理生命周期关系_已加锁(const std::vector<关系句柄>& 关系组) {
        bool 全部完成 = true;
        for (auto 迭代 = 关系组.rbegin(); 迭代 != 关系组.rend(); ++迭代) {
            const auto 待清理关系 = *迭代;
            const auto 位置 = std::find_if(
                生命周期关系登记组_.begin(), 生命周期关系登记组_.end(),
                [&待清理关系](const auto& 登记) { return 登记.关系 == 待清理关系; });
            if (位置 == 生命周期关系登记组_.end()) {
                全部完成 = false;
                continue;
            }
            const bool 已清理 = 关系_.删除关系(待清理关系)
                || !关系_.读取关系(待清理关系).has_value();
            if (!已清理) {
                全部完成 = false;
                continue;
            }
            生命周期关系登记组_.erase(位置);
        }
        return 全部完成;
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

    static bool 关系句柄小于(const 关系句柄& 左, const 关系句柄& 右) {
        if (左.仓库编号 != 右.仓库编号) {
            return 左.仓库编号 < 右.仓库编号;
        }
        if (左.关系编号 != 右.关系编号) {
            return 左.关系编号 < 右.关系编号;
        }
        return 左.版本号 < 右.版本号;
    }

    static bool 活动关系相同(const 概念活动关系材料& 左, const 概念活动关系材料& 右) {
        return 左.类型 == 右.类型 && 左.源节点 == 右.源节点 && 左.目标节点 == 右.目标节点;
    }

    static bool 活动关系小于(const 概念活动关系材料& 左, const 概念活动关系材料& 右) {
        if (左.类型 != 右.类型) {
            return 左.类型 < 右.类型;
        }
        if (节点句柄小于(左.源节点, 右.源节点)) {
            return true;
        }
        if (节点句柄小于(右.源节点, 左.源节点)) {
            return false;
        }
        if (节点句柄小于(左.目标节点, 右.目标节点)) {
            return true;
        }
        if (节点句柄小于(右.目标节点, 左.目标节点)) {
            return false;
        }
        return 关系句柄小于(左.关系, 右.关系);
    }

    static void 规范化活动快照(概念活动快照& 快照) {
        std::sort(快照.概念节点组.begin(), 快照.概念节点组.end(), 节点句柄小于);
        快照.概念节点组.erase(
            std::unique(快照.概念节点组.begin(), 快照.概念节点组.end()),
            快照.概念节点组.end());
        std::sort(快照.关系组.begin(), 快照.关系组.end(), 活动关系小于);
        快照.关系组.erase(
            std::unique(快照.关系组.begin(), 快照.关系组.end(), 活动关系相同),
            快照.关系组.end());
        std::sort(快照.签名组.begin(), 快照.签名组.end(), [](const auto& 左, const auto& 右) {
            return 节点句柄小于(左.概念, 右.概念);
        });
        快照.签名组.erase(
            std::unique(快照.签名组.begin(), 快照.签名组.end(), [](const auto& 左, const auto& 右) {
                return 左.概念 == 右.概念;
            }),
            快照.签名组.end());
    }

    std::vector<节点句柄> 读取活动关系目标组(关系类型 类型, 节点句柄 源节点) const {
        std::vector<节点句柄> 结果;
        std::shared_lock<std::shared_mutex> 锁(活动图锁_);
        if (!活动快照_.has_value()) {
            return 结果;
        }
        for (const auto& 关系材料 : 活动快照_->关系组) {
            if (关系材料.类型 == 类型 && 关系材料.源节点 == 源节点) {
                结果.push_back(关系材料.目标节点);
            }
        }
        std::sort(结果.begin(), 结果.end(), 节点句柄小于);
        结果.erase(std::unique(结果.begin(), 结果.end()), 结果.end());
        return 结果;
    }

    std::vector<节点句柄> 读取活动关系来源组(关系类型 类型, 节点句柄 目标节点) const {
        std::vector<节点句柄> 结果;
        std::shared_lock<std::shared_mutex> 锁(活动图锁_);
        if (!活动快照_.has_value()) {
            return 结果;
        }
        for (const auto& 关系材料 : 活动快照_->关系组) {
            if (关系材料.类型 == 类型 && 关系材料.目标节点 == 目标节点) {
                结果.push_back(关系材料.源节点);
            }
        }
        std::sort(结果.begin(), 结果.end(), 节点句柄小于);
        结果.erase(std::unique(结果.begin(), 结果.end()), 结果.end());
        return 结果;
    }

    static const 概念候选项* 按编号读取候选(
        const std::vector<概念候选项>& 候选组,
        std::uint64_t 编号) {
        for (const auto& 候选 : 候选组) {
            if (候选.候选编号 == 编号) {
                return &候选;
            }
        }
        return nullptr;
    }

    static std::optional<节点句柄> 按编号读取概念(
        const std::vector<候选节点映射材料>& 映射组,
        std::uint64_t 编号) {
        for (const auto& 映射 : 映射组) {
            if (映射.候选编号 == 编号) {
                return 映射.概念;
            }
        }
        return std::nullopt;
    }

    static std::optional<概念签名材料> 从快照读取签名(
        const 概念活动快照& 快照,
        节点句柄 概念) {
        for (const auto& 材料 : 快照.签名组) {
            if (材料.概念 == 概念) {
                return 材料.签名;
            }
        }
        return std::nullopt;
    }

    static bool 候选图内容等价(
        const 概念图候选版本材料& 左,
        const 概念图候选版本材料& 右) {
        auto 左副本 = 左;
        auto 右副本 = 右;
        左副本.基准活动版本 = 0;
        右副本.基准活动版本 = 0;
        左副本.迭代次数 = 0;
        右副本.迭代次数 = 0;
        return 概念图算法::候选版本结构等价(左副本, 右副本);
    }

    bool 候选图可发布_已加锁(
        const 概念图候选版本材料& 候选图,
        const 概念活动快照& 当前快照) const {
        if (!候选图.已收敛()
            || 候选图.候选项组.empty()
            || !概念图算法::候选直接边无环(候选图)) {
            return false;
        }
        std::vector<std::uint64_t> 编号组;
        std::vector<节点句柄> 候选根组;
        for (const auto& 候选 : 候选图.候选项组) {
            if (!候选.完整()
                || std::find(编号组.begin(), 编号组.end(), 候选.候选编号) != 编号组.end()) {
                return false;
            }
            const bool 签名重复 = std::any_of(
                候选图.候选项组.begin(), 候选图.候选项组.end(), [&候选](const auto& 其他) {
                    return 其他.候选编号 < 候选.候选编号
                        && 概念图算法::概念签名等价(其他.签名, 候选.签名);
                });
            if (签名重复) {
                return false;
            }
            编号组.push_back(候选.候选编号);
            const auto 规范签名 = 概念图算法::规范化概念签名(候选.签名);
            if (!规范签名.has_value()
                || !概念图算法::概念签名等价(规范签名.value(), 候选.签名)) {
                return false;
            }
            if (候选.操作 == 概念候选操作类型::复用根) {
                const auto 根材料 = 读取概念根(候选.签名.类别);
                if (!根材料.has_value()
                    || 根材料->根节点 != 候选.现有概念
                    || !候选.定义材料组.empty()
                    || 句柄有效(候选.来源动态)
                    || std::find(当前快照.根组.begin(), 当前快照.根组.end(), 候选.现有概念)
                        == 当前快照.根组.end()) {
                    return false;
                }
                候选根组.push_back(候选.现有概念);
            }
            else if (候选.签名.类别 == 概念根类别::因果 && !句柄有效(候选.来源动态)) {
                return false;
            }
            for (const auto& 实例 : 候选.来源实例组) {
                const auto 类别 = 读取节点概念类别(实例);
                const auto 支持边数 = std::count_if(
                    候选图.实例支持边组.begin(), 候选图.实例支持边组.end(),
                    [&候选, &实例](const auto& 边) {
                        return 边.概念候选编号 == 候选.候选编号 && 边.实例 == 实例;
                    });
                if (!类别.has_value()
                    || 类别.value() != 候选.签名.类别
                    || 支持边数 != 1) {
                    return false;
                }
            }
            for (const auto& 材料 : 候选.定义材料组) {
                const auto 记录 = 节点_.读取节点(材料);
                const auto 定义边数 = std::count_if(
                    候选图.定义材料边组.begin(), 候选图.定义材料边组.end(),
                    [&候选, &材料](const auto& 边) {
                        return 边.概念候选编号 == 候选.候选编号 && 边.定义材料 == 材料;
                    });
                if (!记录.has_value()
                    || !定义材料类型允许(记录->类型)
                    || 定义边数 != 1) {
                    return false;
                }
            }
        }
        std::sort(候选根组.begin(), 候选根组.end(), 节点句柄小于);
        auto 当前根组 = 当前快照.根组;
        std::sort(当前根组.begin(), 当前根组.end(), 节点句柄小于);
        if (候选根组 != 当前根组 || 候选根组.size() != 4) {
            return false;
        }

        for (const auto& 边 : 候选图.实例支持边组) {
            const auto* 目标候选 = 按编号读取候选(候选图.候选项组, 边.概念候选编号);
            const auto 实例类别 = 读取节点概念类别(边.实例);
            if (目标候选 == nullptr
                || !实例类别.has_value()
                || 实例类别.value() != 目标候选->签名.类别
                || std::find(目标候选->来源实例组.begin(), 目标候选->来源实例组.end(), 边.实例)
                    == 目标候选->来源实例组.end()) {
                return false;
            }
        }
        for (const auto& 候选 : 候选图.候选项组) {
            for (const auto& 实例 : 候选.来源实例组) {
                const bool 已承载 = std::any_of(
                    候选图.实例支持边组.begin(), 候选图.实例支持边组.end(),
                    [&候选, &实例](const auto& 边) {
                        return 边.概念候选编号 == 候选.候选编号 && 边.实例 == 实例;
                    });
                if (!已承载) {
                    return false;
                }
            }
            if (候选.操作 == 概念候选操作类型::创建概念) {
                const bool 有直接上位 = std::any_of(
                    候选图.直接边组.begin(), 候选图.直接边组.end(),
                    [&候选](const auto& 边) {
                        return 边.下位候选编号 == 候选.候选编号;
                    });
                if (!有直接上位) {
                    return false;
                }
            }
        }
        for (const auto& 边 : 候选图.定义材料边组) {
            const auto* 候选 = 按编号读取候选(候选图.候选项组, 边.概念候选编号);
            const auto 记录 = 节点_.读取节点(边.定义材料);
            if (候选 == nullptr
                || !记录.has_value()
                || !定义材料类型允许(记录->类型)
                || std::find(候选->定义材料组.begin(), 候选->定义材料组.end(), 边.定义材料)
                    == 候选->定义材料组.end()) {
                return false;
            }
        }
        for (const auto& 边 : 候选图.直接边组) {
            const auto* 上位 = 按编号读取候选(候选图.候选项组, 边.上位候选编号);
            const auto* 下位 = 按编号读取候选(候选图.候选项组, 边.下位候选编号);
            if (上位 == nullptr
                || 下位 == nullptr
                || !概念图算法::概念签名更一般(上位->签名, 下位->签名)) {
                return false;
            }
        }
        return true;
    }

    std::optional<关系句柄> 确保专用关系_已加锁(
        关系类型 类型,
        节点句柄 源节点,
        节点句柄 目标节点) {
        const auto 已登记 = std::find_if(
            专用关系登记组_.begin(), 专用关系登记组_.end(),
            [类型, &源节点, &目标节点](const auto& 材料) {
                return 材料.类型 == 类型 && 材料.源节点 == 源节点 && 材料.目标节点 == 目标节点;
            });
        if (关系_.存在关系(类型, 源节点, 目标节点)) {
            if (已登记 == 专用关系登记组_.end()) {
                (void)追根因检查(false, L"概念专用关系存在但缺少同一服务运行期登记。");
                return std::nullopt;
            }
            const auto 记录 = 关系_.读取关系(已登记->关系);
            if (!追根因检查(记录.has_value()
                && 记录->类型 == 类型
                && 记录->源节点 == 源节点
                && 记录->目标节点 == 目标节点,
                L"概念专用关系登记读回不符合内部预期。")) {
                return std::nullopt;
            }
            return 已登记->关系;
        }
        const auto 新关系 = 关系_.创建关系(类型, 源节点, 目标节点);
        const auto 记录 = 关系_.读取关系(新关系);
        if (!追根因检查(句柄有效(新关系)
            && 记录.has_value()
            && 记录->类型 == 类型
            && 记录->源节点 == 源节点
            && 记录->目标节点 == 目标节点,
            L"概念图专用关系写入后读回不符合内部预期。")) {
            return std::nullopt;
        }
        专用关系登记组_.push_back({新关系, 类型, 源节点, 目标节点});
        return 新关系;
    }

    static bool 追加活动关系(
        概念活动快照& 快照,
        const 概念活动关系材料& 材料) {
        if (!材料.完整()) {
            return false;
        }
        const auto 已有 = std::find_if(快照.关系组.begin(), 快照.关系组.end(), [&材料](const auto& 当前) {
            return 活动关系相同(当前, 材料);
        });
        if (已有 == 快照.关系组.end()) {
            快照.关系组.push_back(材料);
        }
        return true;
    }

    std::optional<节点句柄> 创建候选概念节点(
        const 概念候选项& 候选,
        存在服务& 存在,
        动态服务& 动态,
        二次特征服务& 二次特征,
        因果服务& 因果,
        std::vector<节点句柄>& 本轮新增节点组) {
        节点句柄 新概念;
        switch (候选.签名.类别) {
        case 概念根类别::存在:
            新概念 = 存在.创建存在概念根();
            break;
        case 概念根类别::动态:
            新概念 = 动态.创建动态概念根();
            break;
        case 概念根类别::关系:
            新概念 = 二次特征.创建关系概念根();
            break;
        case 概念根类别::因果:
            新概念 = 因果.记录因果引用(候选.来源动态, 动态);
            break;
        default:
            return std::nullopt;
        }
        if (!句柄有效(新概念)) {
            return std::nullopt;
        }
        本轮新增节点组.push_back(新概念);
        const auto 类别 = 读取节点概念类别(新概念);
        if (!追根因检查(类别.has_value() && 类别.value() == 候选.签名.类别,
            L"创建候选概念节点后类别读回不符合内部预期。")) {
            return std::nullopt;
        }
        return 新概念;
    }

    std::optional<概念活动快照> 构造候选活动快照_已加锁(
        const 概念图候选版本材料& 候选图,
        const 概念活动快照& 旧快照,
        存在服务& 存在,
        动态服务& 动态,
        二次特征服务& 二次特征,
        因果服务& 因果,
        std::vector<节点句柄>& 本轮新增节点组,
        std::vector<关系句柄>& 本轮新增生命周期关系组) {
        概念活动快照 新快照;
        新快照.活动版本 = 旧快照.活动版本 + 1;
        新快照.根组 = 旧快照.根组;
        std::vector<候选节点映射材料> 映射组;
        映射组.reserve(候选图.候选项组.size());
        for (const auto& 候选 : 候选图.候选项组) {
            节点句柄 概念;
            if (候选.操作 == 概念候选操作类型::复用根) {
                概念 = 候选.现有概念;
            }
            else {
                for (const auto& 已有签名 : 旧快照.签名组) {
                    if (概念图算法::概念签名等价(已有签名.签名, 候选.签名)
                        && std::find(旧快照.根组.begin(), 旧快照.根组.end(), 已有签名.概念)
                            == 旧快照.根组.end()) {
                        概念 = 已有签名.概念;
                        break;
                    }
                }
                if (!句柄有效(概念)) {
                    const auto 新概念 = 创建候选概念节点(
                        候选, 存在, 动态, 二次特征, 因果, 本轮新增节点组);
                    if (!新概念.has_value()) {
                        return std::nullopt;
                    }
                    概念 = 新概念.value();
                }
            }
            const bool 是根 = std::find(旧快照.根组.begin(), 旧快照.根组.end(), 概念)
                != 旧快照.根组.end();
            const bool 是本轮新增 = std::find(
                本轮新增节点组.begin(), 本轮新增节点组.end(), 概念) != 本轮新增节点组.end();
            const auto 预期阶段 = (是根 || 是本轮新增)
                ? std::optional<概念生命周期阶段>{概念生命周期阶段::活跃}
                : std::nullopt;
            if (!确保概念生命周期_已加锁(
                    概念,
                    预期阶段,
                    是本轮新增,
                    本轮新增生命周期关系组).has_value()) {
                return std::nullopt;
            }
            映射组.push_back({候选.候选编号, 概念});
            新快照.概念节点组.push_back(概念);
            新快照.签名组.push_back({概念, 候选.签名});
        }

        for (const auto& 旧关系 : 旧快照.关系组) {
            if (旧关系.类型 == 关系类型::实例支持概念
                && std::find(旧快照.根组.begin(), 旧快照.根组.end(), 旧关系.目标节点)
                    != 旧快照.根组.end()) {
                (void)追加活动关系(新快照, 旧关系);
            }
        }
        for (const auto& 边 : 候选图.实例支持边组) {
            const auto 概念 = 按编号读取概念(映射组, 边.概念候选编号);
            if (!概念.has_value()) {
                return std::nullopt;
            }
            const auto 关系句柄值 = 确保专用关系_已加锁(
                关系类型::实例支持概念, 边.实例, 概念.value());
            if (!关系句柄值.has_value()
                || !追加活动关系(新快照,
                    {关系句柄值.value(), 关系类型::实例支持概念, 边.实例, 概念.value()})) {
                return std::nullopt;
            }
        }
        for (const auto& 边 : 候选图.定义材料边组) {
            const auto 概念 = 按编号读取概念(映射组, 边.概念候选编号);
            if (!概念.has_value()) {
                return std::nullopt;
            }
            const auto 关系句柄值 = 确保专用关系_已加锁(
                关系类型::概念定义材料, 概念.value(), 边.定义材料);
            if (!关系句柄值.has_value()
                || !追加活动关系(新快照,
                    {关系句柄值.value(), 关系类型::概念定义材料, 概念.value(), 边.定义材料})) {
                return std::nullopt;
            }
        }
        for (const auto& 边 : 候选图.直接边组) {
            const auto 上位 = 按编号读取概念(映射组, 边.上位候选编号);
            const auto 下位 = 按编号读取概念(映射组, 边.下位候选编号);
            if (!上位.has_value() || !下位.has_value()) {
                return std::nullopt;
            }
            const auto 关系句柄值 = 确保专用关系_已加锁(
                关系类型::概念上下位, 上位.value(), 下位.value());
            if (!关系句柄值.has_value()
                || !追加活动关系(新快照,
                    {关系句柄值.value(), 关系类型::概念上下位, 上位.value(), 下位.value()})) {
                return std::nullopt;
            }
        }
        规范化活动快照(新快照);
        return 新快照;
    }

    bool 清理未发布候选_已加锁(
        std::size_t 原专用关系登记数量,
        const std::vector<关系句柄>& 本轮新增生命周期关系组,
        const std::vector<节点句柄>& 本轮新增节点组) {
        if (原专用关系登记数量 > 专用关系登记组_.size()) {
            return false;
        }
        std::vector<专用关系登记材料> 保留登记组(
            专用关系登记组_.begin(),
            专用关系登记组_.begin() + 原专用关系登记数量);
        bool 关系清理完成 = true;
        for (std::size_t 索引 = 专用关系登记组_.size(); 索引 > 原专用关系登记数量; --索引) {
            const auto& 登记 = 专用关系登记组_[索引 - 1];
            const bool 已清理 = 关系_.删除关系(登记.关系)
                || !关系_.读取关系(登记.关系).has_value();
            if (!已清理) {
                保留登记组.push_back(登记);
                关系清理完成 = false;
            }
        }
        专用关系登记组_ = std::move(保留登记组);
        const bool 生命周期关系清理完成 = 清理生命周期关系_已加锁(本轮新增生命周期关系组);
        if (!关系清理完成 || !生命周期关系清理完成) {
            return false;
        }
        bool 节点清理完成 = true;
        for (auto 迭代 = 本轮新增节点组.rbegin(); 迭代 != 本轮新增节点组.rend(); ++迭代) {
            const bool 已清理 = 节点_.删除节点(*迭代) || !节点_.读取节点(*迭代).has_value();
            节点清理完成 = 节点清理完成 && 已清理;
        }
        return 节点清理完成;
    }

    bool 验证活动快照_已加锁(const 概念活动快照& 快照) const {
        if (快照.活动版本 == 0
            || 快照.根组.size() != 4
            || 快照.概念节点组.size() != 快照.签名组.size()) {
            return false;
        }
        for (const auto& 根 : 快照.根组) {
            if (!节点是否已登记概念根(根)
                || std::find(快照.概念节点组.begin(), 快照.概念节点组.end(), 根)
                    == 快照.概念节点组.end()) {
                return false;
            }
        }
        if (!生命周期状态登记完整()) {
            return false;
        }
        for (const auto& 概念 : 快照.概念节点组) {
            const auto 生命周期 = 读取概念生命周期_已加锁(概念);
            if (!生命周期.has_value()) {
                return false;
            }
            const bool 是根 = std::find(快照.根组.begin(), 快照.根组.end(), 概念)
                != 快照.根组.end();
            if (是根 && 生命周期->阶段 != 概念生命周期阶段::活跃) {
                return false;
            }
        }
        for (const auto& 签名材料 : 快照.签名组) {
            const auto 类别 = 读取节点概念类别(签名材料.概念);
            if (!类别.has_value()
                || 类别.value() != 签名材料.签名.类别
                || !概念图算法::规范化概念签名(签名材料.签名).has_value()) {
                return false;
            }
        }
        for (const auto& 关系材料 : 快照.关系组) {
            const auto 记录 = 关系_.读取关系(关系材料.关系);
            if (!关系材料.完整()
                || !记录.has_value()
                || 记录->类型 != 关系材料.类型
                || 记录->源节点 != 关系材料.源节点
                || 记录->目标节点 != 关系材料.目标节点) {
                return false;
            }
            if (关系材料.类型 == 关系类型::概念上下位) {
                const auto 上位签名 = 从快照读取签名(快照, 关系材料.源节点);
                const auto 下位签名 = 从快照读取签名(快照, 关系材料.目标节点);
                if (!上位签名.has_value()
                    || !下位签名.has_value()
                    || !概念图算法::概念签名更一般(上位签名.value(), 下位签名.value())) {
                    return false;
                }
            }
            else if (关系材料.类型 == 关系类型::实例支持概念) {
                const auto 实例类别 = 读取节点概念类别(关系材料.源节点);
                const auto 概念签名 = 从快照读取签名(快照, 关系材料.目标节点);
                if (!实例类别.has_value()
                    || !概念签名.has_value()
                    || 实例类别.value() != 概念签名->类别) {
                    return false;
                }
            }
            else {
                const auto 材料记录 = 节点_.读取节点(关系材料.目标节点);
                if (!从快照读取签名(快照, 关系材料.源节点).has_value()
                    || !材料记录.has_value()
                    || !定义材料类型允许(材料记录->类型)) {
                    return false;
                }
            }
        }
        for (const auto& 概念 : 快照.概念节点组) {
            const bool 是根 = std::find(快照.根组.begin(), 快照.根组.end(), 概念) != 快照.根组.end();
            const bool 有直接上位 = std::any_of(
                快照.关系组.begin(), 快照.关系组.end(), [&概念](const auto& 关系材料) {
                    return 关系材料.类型 == 关系类型::概念上下位 && 关系材料.目标节点 == 概念;
                });
            if (是根 == 有直接上位) {
                return false;
            }
            std::vector<节点句柄> 待检查{概念};
            std::vector<节点句柄> 已检查;
            std::vector<节点句柄> 可达根组;
            while (!待检查.empty()) {
                const auto 当前 = 待检查.back();
                待检查.pop_back();
                if (std::find(已检查.begin(), 已检查.end(), 当前) != 已检查.end()) {
                    continue;
                }
                已检查.push_back(当前);
                if (std::find(快照.根组.begin(), 快照.根组.end(), 当前) != 快照.根组.end()) {
                    可达根组.push_back(当前);
                    continue;
                }
                for (const auto& 关系材料 : 快照.关系组) {
                    if (关系材料.类型 == 关系类型::概念上下位 && 关系材料.目标节点 == 当前) {
                        待检查.push_back(关系材料.源节点);
                    }
                }
            }
            std::sort(可达根组.begin(), 可达根组.end(), 节点句柄小于);
            可达根组.erase(std::unique(可达根组.begin(), 可达根组.end()), 可达根组.end());
            if (可达根组.size() != 1) {
                return false;
            }
        }
        return true;
    }

    bool 失效旧活动关系_已加锁(
        const 概念活动快照& 旧快照,
        const 概念活动快照& 新快照) {
        for (const auto& 旧关系 : 旧快照.关系组) {
            const bool 仍活动 = std::any_of(
                新快照.关系组.begin(), 新快照.关系组.end(), [&旧关系](const auto& 新关系) {
                    return 活动关系相同(旧关系, 新关系);
                });
            if (仍活动) {
                continue;
            }
            if (!关系_.删除关系(旧关系.关系)) {
                return false;
            }
            专用关系登记组_.erase(
                std::remove_if(
                    专用关系登记组_.begin(), 专用关系登记组_.end(),
                    [&旧关系](const auto& 登记) {
                        return 登记.关系 == 旧关系.关系;
                    }),
                专用关系登记组_.end());
        }
        return true;
    }

    static std::optional<抽象树视图材料> 生成抽象树视图(
        const 概念活动快照& 快照,
        节点句柄 根概念,
        std::size_t 节点预算,
        std::size_t 深度预算) {
        const auto 根签名 = 从快照读取签名(快照, 根概念);
        if (节点预算 == 0
            || 深度预算 == 0
            || !根签名.has_value()
            || !根签名->约束组.empty()
            || std::find(快照.根组.begin(), 快照.根组.end(), 根概念) == 快照.根组.end()) {
            return std::nullopt;
        }
        抽象树视图材料 视图;
        视图.活动版本 = 快照.活动版本;
        视图.根类别 = 根签名->类别;
        视图.根概念 = 根概念;
        std::vector<抽象树待投影材料> 待处理{{根概念, {根概念}}};
        std::vector<节点句柄> 已投影概念组;
        while (!待处理.empty()) {
            auto 当前 = std::move(待处理.back());
            待处理.pop_back();
            if (当前.路径.empty()
                || 当前.路径.size() > 深度预算
                || 视图.投影项组.size() >= 节点预算) {
                return std::nullopt;
            }
            const bool 重复 = std::find(已投影概念组.begin(), 已投影概念组.end(), 当前.概念)
                != 已投影概念组.end();
            if (!重复) {
                已投影概念组.push_back(当前.概念);
            }
            视图.投影项组.push_back({
                当前.概念,
                视图.根类别,
                当前.路径,
                当前.路径.size() - 1,
                重复,
                {}
            });
            std::vector<节点句柄> 下位组;
            for (const auto& 关系材料 : 快照.关系组) {
                if (关系材料.类型 == 关系类型::概念上下位 && 关系材料.源节点 == 当前.概念) {
                    下位组.push_back(关系材料.目标节点);
                }
            }
            std::sort(下位组.begin(), 下位组.end(), 节点句柄小于);
            下位组.erase(std::unique(下位组.begin(), 下位组.end()), 下位组.end());
            for (auto 迭代 = 下位组.rbegin(); 迭代 != 下位组.rend(); ++迭代) {
                if (std::find(当前.路径.begin(), 当前.路径.end(), *迭代) != 当前.路径.end()) {
                    return std::nullopt;
                }
                auto 新路径 = 当前.路径;
                新路径.push_back(*迭代);
                待处理.push_back({*迭代, std::move(新路径)});
            }
        }
        return 视图.完整() ? std::optional<抽象树视图材料>{视图} : std::nullopt;
    }

    bool 幂等创建专用关系(关系类型 类型, 节点句柄 源节点, 节点句柄 目标节点) {
        return 确保专用关系_已加锁(类型, 源节点, 目标节点).has_value();
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

    节点仓库& 节点_;
    关系仓库& 关系_;
    mutable std::shared_mutex 根登记锁_;
    std::array<std::optional<概念根登记材料>, 4> 根登记组_;
    mutable std::shared_mutex 生命周期状态锁_;
    std::array<std::optional<节点句柄>, 3> 生命周期状态组_;
    std::mutex 图写锁_;
    std::vector<专用关系登记材料> 专用关系登记组_;
    std::vector<生命周期关系登记材料> 生命周期关系登记组_;
    mutable std::shared_mutex 活动图锁_;
    std::optional<概念活动快照> 活动快照_;
    std::optional<概念图候选版本材料> 上一候选图_;
};

}
