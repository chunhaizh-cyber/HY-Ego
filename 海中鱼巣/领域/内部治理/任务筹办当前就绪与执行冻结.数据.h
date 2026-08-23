#pragma once

#ifndef 任务筹办当前就绪与执行冻结数据_NO_INCLUDES
#include <cstdint>
#include <optional>
#include <vector>

#include "../L2任务结构.数据.h"
#include "任务重筹办.数据.h"
#include "任务特征方法查找与条件提议.数据.h"
#endif

namespace 海中鱼巣 {

inline constexpr std::uint32_t 任务筹办当前就绪合同版本 = 1;

struct 任务条件当前就绪证据 final {
    L2方法条件事实 条件;
    L2存在身份 绑定目标;
    std::optional<L2场景身份> 场景;
    std::optional<L2存在场景成员引用事实> 场景成员;
    L2状态事实 当前状态;
    L2特征比较具名关系 具名关系 =
        L2特征比较具名关系::当前达到目标;
    std::uint8_t 允许关系位 = 0;
    friend bool operator==(const 任务条件当前就绪证据&,
        const 任务条件当前就绪证据&) = default;
};

struct 任务输入规格冻结项 final {
    L2方法输入规格事实 输入规格;
    L2存在身份 绑定目标;
    friend bool operator==(const 任务输入规格冻结项&,
        const 任务输入规格冻结项&) = default;
};

struct 任务参数绑定冻结项 final {
    L2方法输入规格身份 来源输入规格;
    std::uint64_t 作用对象角色 = 0;
    L2方法规格材料 参数规格;
    L2存在身份 绑定目标;
    friend bool operator==(const 任务参数绑定冻结项&,
        const 任务参数绑定冻结项&) = default;
};

struct 任务限制条件复判证据 final {
    L2方法限制条件事实 限制条件;
    L2存在身份 绑定目标;
    L2方法限制解释器身份 解释器;
    std::uint32_t 解释规则版本 = 0;
    L2任务限制当前事实证据引用 当前事实证据;
    bool 已满足 = false;
    friend bool operator==(const 任务限制条件复判证据&,
        const 任务限制条件复判证据&) = default;
};

enum class 任务筹办当前就绪结论 : std::uint8_t {
    已就绪 = 1,
    条件未满足 = 2,
    输入待绑定 = 3,
    限制待解释 = 4,
    方法结构待补齐 = 5,
    授权安全等待 = 6,
    当前性漂移 = 7,
    引用冲突 = 8,
    数量预算不足 = 9,
    资源失败 = 10,
    内部错误 = 11
};

struct 任务筹办当前就绪请求 final {
    std::uint32_t 合同版本 = 任务筹办当前就绪合同版本;
    L2结构请求头 请求头;
    下一筹办工作包 工作包;
    任务特征方法查找结果 方法查找结果;
    std::uint64_t 最大来源需求成员数 = 0;
    std::uint64_t 最大条件状态数 = 0;
    std::uint64_t 最大场景成员数 = 0;
    friend bool operator==(const 任务筹办当前就绪请求&,
        const 任务筹办当前就绪请求&) = default;
};

struct 任务筹办当前就绪结果 final {
    任务筹办当前就绪结论 结论 = 任务筹办当前就绪结论::内部错误;
    std::uint64_t 共同事实截止 = 0;
    std::optional<L2普通方法事实> 当前方法;
    std::vector<L2需求事实> 来源需求组;
    std::vector<任务条件当前就绪证据> 条件证据组;
    std::vector<任务输入规格冻结项> 输入冻结组;
    std::vector<任务参数绑定冻结项> 参数冻结组;
    std::vector<任务限制条件复判证据> 限制证据组;
    std::optional<L2任务授权安全复判材料> 授权安全材料;
    std::optional<L2方法结果事实> 主轴结果;
    std::optional<L2方法动作入口事实> 动作入口;
    friend bool operator==(const 任务筹办当前就绪结果&,
        const 任务筹办当前就绪结果&) = default;
};

enum class 任务筹办收束类别 : std::uint8_t {
    执行冻结候选 = 1,
    合法等待 = 2,
    子目标承接 = 3,
    学习需求承接 = 4,
    方法补齐承接 = 5,
    当前性漂移 = 6,
    引用冲突 = 7,
    数量预算不足 = 8,
    资源失败 = 9,
    内部错误 = 10
};

#define 定义任务筹办等待身份(类型名) \
    struct 类型名 final { \
        稳定编码 值; \
        explicit 类型名(稳定编码 编码 = {}) noexcept : 值(编码) {} \
        friend bool operator==(const 类型名&, const 类型名&) = default; \
    }

定义任务筹办等待身份(任务筹办等待对象身份);
定义任务筹办等待身份(任务筹办合法生产者身份);
定义任务筹办等待身份(任务筹办重新触发条件身份);

#undef 定义任务筹办等待身份

struct 任务筹办等待合同登记项 final {
    std::optional<任务特征方法查找结论> 方法查找结论;
    std::optional<任务筹办当前就绪结论> 当前就绪结论;
    任务筹办等待对象身份 等待对象;
    任务筹办合法生产者身份 合法生产者;
    任务筹办重新触发条件身份 重新触发条件;
    friend bool operator==(const 任务筹办等待合同登记项&,
        const 任务筹办等待合同登记项&) = default;
};

struct 任务筹办等待合同登记 final {
    std::uint32_t 登记版本 = 1;
    std::vector<任务筹办等待合同登记项> 项;
    friend bool operator==(const 任务筹办等待合同登记&,
        const 任务筹办等待合同登记&) = default;
};

struct 任务筹办等待合同 final {
    任务筹办等待对象身份 等待对象;
    任务筹办合法生产者身份 合法生产者;
    任务筹办重新触发条件身份 重新触发条件;
    下一筹办工作包 原工作包;
    friend bool operator==(const 任务筹办等待合同&,
        const 任务筹办等待合同&) = default;
};

struct 任务学习需求承接草案 final {
    L2任务身份 来源任务;
    std::uint64_t 筹办轮次 = 0;
    任务筹办目标三元组 目标;
    L2状态事实 当前状态;
    L2方法变化方向 所需方向 = L2方法变化方向::无变化;
    std::vector<L2方法特征粗召回项> 完整空召回;
    std::vector<稳定编码> 学习门禁证据组;
    任务筹办合法生产者身份 合法学习需求生产者;
    friend bool operator==(const 任务学习需求承接草案&,
        const 任务学习需求承接草案&) = default;
};

struct 任务方法补齐承接草案 final {
    L2任务身份 来源任务;
    std::uint64_t 筹办轮次 = 0;
    L2方法身份 方法;
    L2方法内容版本 内容版本;
    L2方法规格版本 规格版本;
    std::vector<L2方法结构缺口> 结构缺口组;
    std::vector<任务未绑定输入限制证据> 未绑定材料组;
    任务筹办合法生产者身份 合法补齐处理方;
    friend bool operator==(const 任务方法补齐承接草案&,
        const 任务方法补齐承接草案&) = default;
};

struct 任务筹办非成功承接材料 final {
    任务筹办收束类别 类别 = 任务筹办收束类别::内部错误;
    std::optional<任务筹办等待合同> 等待合同;
    std::vector<任务缺失条件草案> 子目标草案组;
    std::optional<任务学习需求承接草案> 学习需求草案;
    std::optional<任务方法补齐承接草案> 方法补齐草案;
    friend bool operator==(const 任务筹办非成功承接材料&,
        const 任务筹办非成功承接材料&) = default;
};

struct 任务筹办推进请求 final {
    std::uint32_t 合同版本 = 任务筹办当前就绪合同版本;
    L2结构请求头 请求头;
    下一筹办工作包 工作包;
    std::uint64_t 最大扫描用途事实数 = 0;
    std::uint64_t 最大粗召回方法数 = 0;
    std::uint64_t 最大概念支持扫描事实数 = 0;
    std::uint64_t 最大概念支持关系数 = 0;
    std::uint64_t 最大概念祖先关系数 = 0;
    std::uint64_t 最大主体状态数 = 0;
    std::uint64_t 最大场景成员数 = 0;
    std::uint64_t 最大方法条件数 = 0;
    std::uint64_t 最大来源需求成员数 = 0;
    friend bool operator==(const 任务筹办推进请求&,
        const 任务筹办推进请求&) = default;
};

struct 任务执行冻结形成材料 final {
    下一筹办工作包 来源工作包;
    L2任务方法选择完整事实 正式选择;
    L2任务执行绑定冻结材料事实 冻结材料;
    L2实例方法事实 实例方法;
    std::uint64_t 形成事实截止 = 0;
    friend bool operator==(const 任务执行冻结形成材料&,
        const 任务执行冻结形成材料&) = default;
};

struct 任务筹办推进结果 final {
    任务筹办收束类别 类别 = 任务筹办收束类别::内部错误;
    std::optional<任务执行冻结形成材料> 执行冻结材料;
    std::optional<任务筹办非成功承接材料> 非成功承接;
    std::uint64_t 最终事实截止 = 0;
    bool 成功() const noexcept {
        return 类别 == 任务筹办收束类别::执行冻结候选
            && 执行冻结材料 && !非成功承接 && 最终事实截止 != 0;
    }
    friend bool operator==(const 任务筹办推进结果&,
        const 任务筹办推进结果&) = default;
};

inline bool 任务筹办等待合同登记完整(
    const 任务筹办等待合同登记& 登记) noexcept {
    if (登记.登记版本 != 1 || 登记.项.empty()) return false;
    for (std::size_t 索引 = 0; 索引 < 登记.项.size(); ++索引) {
        const auto& 项 = 登记.项[索引];
        if (项.方法查找结论.has_value() == 项.当前就绪结论.has_value()
            || !有效(项.等待对象.值) || !有效(项.合法生产者.值)
            || !有效(项.重新触发条件.值)) return false;
        for (std::size_t 前 = 0; 前 < 索引; ++前) {
            const auto& 已有 = 登记.项[前];
            if ((项.方法查找结论
                    && 已有.方法查找结论 == 项.方法查找结论)
                || (项.当前就绪结论
                    && 已有.当前就绪结论 == 项.当前就绪结论)) return false;
        }
    }
    return true;
}

} // namespace 海中鱼巣
