#pragma once

#ifndef 任务特征方法查找与条件提议数据_NO_INCLUDES
#include <cstdint>
#include <optional>
#include <vector>
#include "任务重筹办.数据.h"
#include "../L2普通方法结构.数据.h"
#include "../L2特征结构.数据.h"
#endif

namespace 海中鱼巣 {

inline constexpr std::uint32_t 任务特征方法查找合同版本 = 1;

enum class 任务特征方法查找状态 : std::uint8_t {
    已评估 = 1, 数量预算不足 = 2, 当前性漂移 = 3, 引用冲突 = 4,
    入口拒绝 = 5, 资源失败 = 6, 内部错误 = 7
};

enum class 任务特征方法查找结论 : std::uint8_t {
    方法能力缺失 = 1, 当前无适用方法 = 2, 对象概念待形成 = 3,
    所需方向不唯一 = 4, 方法结构待补齐 = 5,
    多候选待选择证据 = 6, 已采用且无需条件提议 = 7,
    已采用且存在缺失条件 = 8, 已采用且输入限制待绑定 = 9
};

struct 任务筹办目标三元组 final {
    L2存在身份 目标宿主;
    L2特征定义身份 目标特征;
    L2目标状态合同身份 目标状态合同;
    friend bool operator==(const 任务筹办目标三元组&,
        const 任务筹办目标三元组&) = default;
};

struct 任务对象概念闭包项 final {
    L2概念身份 当前对象概念;
    L2概念身份 存在根祖先概念;
    friend bool operator==(const 任务对象概念闭包项&,
        const 任务对象概念闭包项&) = default;
};

struct 任务方法适用候选 final {
    L2方法身份 方法;
    L2方法内容版本 内容版本;
    L2方法规格版本 规格版本;
    L2方法结果身份 主轴结果;
    L2方法用途身份 匹配用途;
    L2方法六项静态能力签名 方法完整描述;
    friend bool operator==(const 任务方法适用候选&,
        const 任务方法适用候选&) = default;
};

struct 任务方法结构缺口项 final {
    L2方法身份 方法;
    std::vector<L2方法结构缺口> 结构缺口;
    friend bool operator==(const 任务方法结构缺口项&,
        const 任务方法结构缺口项&) = default;
};

enum class 任务方法不兼容原因 : std::uint8_t {
    对象类型不兼容 = 1, 所需方向不兼容 = 2,
    结果类型不是目标状态合同 = 3, 目标状态合同不相同 = 4
};

struct 任务方法不兼容证据 final {
    L2方法身份 方法;
    L2方法内容版本 内容版本;
    L2方法规格版本 规格版本;
    L2方法结果身份 主轴结果;
    L2方法用途身份 匹配用途;
    std::vector<任务方法不兼容原因> 原因;
    friend bool operator==(const 任务方法不兼容证据&,
        const 任务方法不兼容证据&) = default;
};

enum class 任务未绑定材料类别 : std::uint8_t {
    条件 = 1, 输入规格 = 2, 限制条件 = 3
};

struct 任务未绑定输入限制证据 final {
    任务未绑定材料类别 类别 = 任务未绑定材料类别::条件;
    std::uint64_t 作用对象角色 = 0;
    std::optional<L2方法条件身份> 条件;
    std::optional<L2方法输入规格身份> 输入规格;
    std::optional<L2方法限制条件身份> 限制条件;
    friend bool operator==(const 任务未绑定输入限制证据&,
        const 任务未绑定输入限制证据&) = default;
};

struct 任务输入规格绑定项 final {
    L2方法输入规格身份 输入规格;
    L2方法输入规格语义项 语义;
    std::optional<L2存在身份> 绑定目标存在;
    friend bool operator==(const 任务输入规格绑定项&,
        const 任务输入规格绑定项&) = default;
};

struct 任务限制条件绑定项 final {
    L2方法限制条件身份 限制条件;
    L2方法限制条件语义项 语义;
    std::optional<L2存在身份> 绑定目标存在;
    friend bool operator==(const 任务限制条件绑定项&,
        const 任务限制条件绑定项&) = default;
};

struct 任务缺失条件草案 final {
    L2任务身份 来源任务;
    L2方法身份 方法;
    L2方法内容版本 内容版本;
    L2方法规格版本 规格版本;
    L2方法结果身份 主轴结果;
    L2方法条件身份 条件;
    std::uint64_t 作用对象角色 = 0;
    L2存在身份 绑定目标存在;
    std::optional<L2场景身份> 场景;
    L2特征定义身份 条件特征;
    L2目标状态合同身份 条件状态合同;
    稳定编码 来源稳定编码;
    std::uint64_t 共同事实截止 = 0;
    friend bool operator==(const 任务缺失条件草案&,
        const 任务缺失条件草案&) = default;
};

struct 任务特征方法查找请求 final {
    std::uint32_t 合同版本 = 任务特征方法查找合同版本;
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
};

struct 任务特征方法查找结果 final {
    任务特征方法查找状态 状态 = 任务特征方法查找状态::入口拒绝;
    std::optional<任务特征方法查找结论> 结论;
    std::uint64_t 共同事实截止 = 0;
    std::optional<下一筹办工作包> 已互证工作包;
    std::optional<任务筹办目标三元组> 任务目标;
    std::optional<L2状态事实> 当前实际状态;
    std::optional<L2特征比较具名关系> 目标比较具名关系;
    std::uint8_t 允许关系位 = 0;
    std::optional<L2方法变化方向> 所需方向;
    std::uint64_t 实际扫描用途事实数 = 0;
    std::vector<L2概念身份> 当前对象概念;
    std::vector<任务对象概念闭包项> 对象概念祖先闭包;
    std::optional<std::vector<L2方法特征粗召回项>> 完整粗召回;
    std::vector<任务方法不兼容证据> 不兼容证据;
    std::vector<任务方法适用候选> 适用候选;
    std::vector<任务方法结构缺口项> 结构缺口;
    std::optional<任务方法适用候选> 采用方案;
    std::vector<任务输入规格绑定项> 采用方案输入规格;
    std::vector<任务限制条件绑定项> 采用方案限制条件;
    std::vector<任务未绑定输入限制证据> 未绑定输入限制;
    std::vector<任务缺失条件草案> 缺失条件草案;
    bool 成功() const noexcept;
};

} // namespace 海中鱼巣
