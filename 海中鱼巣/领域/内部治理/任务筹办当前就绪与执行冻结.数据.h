#pragma once

#ifndef 任务筹办当前就绪与执行冻结数据_NO_INCLUDES
#include <cstdint>
#include <optional>
#include <variant>
#include <vector>

#include "../L2任务结构.数据.h"
#include "../L2任务子目标承接记录.数据.h"
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
    可执行冻结 = 1,
    子目标承接 = 2,
    合法等待 = 3,
    目标已完成 = 4,
    结构化非成功 = 5
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

struct 任务筹办可执行冻结定位 final {
    L2任务方法选择记录身份 正式选择;
    L2任务方法路径身份 路径;
    L2任务执行绑定冻结材料身份 冻结材料;
    L2实例方法身份 实例方法;
    friend bool operator==(const 任务筹办可执行冻结定位&,
        const 任务筹办可执行冻结定位&) = default;
};

struct 任务筹办子目标承接定位 final {
    std::vector<L2任务子目标承接记录身份> 记录身份组;
    friend bool operator==(const 任务筹办子目标承接定位&,
        const 任务筹办子目标承接定位&) = default;
};

struct 任务筹办合法等待定位 final {
    任务筹办等待合同 等待合同;
    friend bool operator==(const 任务筹办合法等待定位&,
        const 任务筹办合法等待定位&) = default;
};

struct 任务筹办目标已完成定位 final {
    L2任务目标裁决证据身份 目标裁决证据;
    friend bool operator==(const 任务筹办目标已完成定位&,
        const 任务筹办目标已完成定位&) = default;
};

enum class 任务筹办非成功类别 : std::uint8_t {
    当前性漂移 = 1,
    引用冲突 = 2,
    数量预算不足 = 3,
    入口拒绝 = 4,
    资源失败 = 5,
    内部错误 = 6,
    待实现能力 = 7
};

struct 任务筹办结构化非成功材料 final {
    任务筹办非成功类别 类别 = 任务筹办非成功类别::内部错误;
    std::uint64_t 失败事实截止 = 0;
    friend bool operator==(const 任务筹办结构化非成功材料&,
        const 任务筹办结构化非成功材料&) = default;
};

using 任务筹办结果载荷 = std::variant<
    任务筹办可执行冻结定位,
    任务筹办子目标承接定位,
    任务筹办合法等待定位,
    任务筹办目标已完成定位,
    任务筹办结构化非成功材料>;

struct 任务筹办推进结果 final {
    任务筹办收束类别 类别 = 任务筹办收束类别::结构化非成功;
    任务筹办结果载荷 载荷 = 任务筹办结构化非成功材料{};
    std::uint64_t 最终事实截止 = 0;
    bool 完整() const noexcept {
        if (载荷.valueless_by_exception() || 最终事实截止 == 0) return false;
        switch (类别) {
        case 任务筹办收束类别::可执行冻结: {
            if (!std::holds_alternative<任务筹办可执行冻结定位>(载荷))
                return false;
            const auto& 定位 = std::get<任务筹办可执行冻结定位>(载荷);
            return 有效(定位.正式选择.值) && 有效(定位.路径.值)
                && 有效(定位.冻结材料.值) && 有效(定位.实例方法.值);
        }
        case 任务筹办收束类别::子目标承接: {
            if (!std::holds_alternative<任务筹办子目标承接定位>(载荷))
                return false;
            const auto& 定位 = std::get<任务筹办子目标承接定位>(载荷);
            if (定位.记录身份组.empty()) return false;
            for (std::size_t 索引 = 0; 索引 < 定位.记录身份组.size(); ++索引) {
                if (!有效(定位.记录身份组[索引].值)) return false;
                for (std::size_t 前 = 0; 前 < 索引; ++前)
                    if (定位.记录身份组[前] == 定位.记录身份组[索引])
                        return false;
            }
            return true;
        }
        case 任务筹办收束类别::合法等待: {
            if (!std::holds_alternative<任务筹办合法等待定位>(载荷))
                return false;
            const auto& 合同 =
                std::get<任务筹办合法等待定位>(载荷).等待合同;
            return 有效(合同.等待对象.值) && 有效(合同.合法生产者.值)
                && 有效(合同.重新触发条件.值)
                && 下一筹办工作包完整(合同.原工作包);
        }
        case 任务筹办收束类别::目标已完成:
            return std::holds_alternative<任务筹办目标已完成定位>(载荷)
                && 有效(std::get<任务筹办目标已完成定位>(载荷)
                    .目标裁决证据.值);
        case 任务筹办收束类别::结构化非成功: {
            if (!std::holds_alternative<任务筹办结构化非成功材料>(载荷))
                return false;
            const auto& 非成功 =
                std::get<任务筹办结构化非成功材料>(载荷);
            switch (非成功.类别) {
            case 任务筹办非成功类别::当前性漂移:
            case 任务筹办非成功类别::引用冲突:
            case 任务筹办非成功类别::数量预算不足:
            case 任务筹办非成功类别::入口拒绝:
            case 任务筹办非成功类别::资源失败:
            case 任务筹办非成功类别::内部错误:
            case 任务筹办非成功类别::待实现能力:
                return 非成功.失败事实截止 == 最终事实截止;
            }
            return false;
        }
        }
        return false;
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
