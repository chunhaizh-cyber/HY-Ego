#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "../核心/基础数据集.h"

namespace 海中鱼巣 {

enum class 新方法作用域 : std::int64_t {
    内部治理 = 1,
    普通世界 = 2
};

enum class 新方法实现方式 : std::int64_t {
    本能 = 1,
    组合 = 2
};

// 条件与结果必须显式声明关系；方法头中的概念集合不使用本枚举。
enum class 新方法项关系规则 : std::int64_t {
    单项 = 1,
    全部成立 = 2,
    任一成立 = 3,
    有序成立 = 4,
    条件选择 = 5,
    主要结果及允许副作用 = 6
};

enum class 新方法启用状态 : std::int64_t {
    停用 = 0,
    启用 = 1
};

enum class 新方法操作状态 : std::uint8_t {
    已建立 = 1,
    已找到 = 2,
    已修改 = 3,
    无变化 = 4,
    入口拒绝 = 5,
    尚未初始化 = 6,
    方法不存在 = 7,
    方法类型不符 = 8,
    方法头不合法 = 9,
    参数规格不合法 = 10,
    条件结果不合法 = 11,
    函数登记不合法 = 12,
    函数登记冲突 = 21,
    步骤不存在 = 13,
    结果节点不属于步骤方法 = 14,
    参数角色缺失 = 15,
    参数角色重复 = 16,
    参数引用失效 = 17,
    参数概念不相容 = 18,
    结构不一致 = 19,
    资源失败 = 20
};

struct 新方法头定义 final {
    // 方法头只用于召回。集合间没有默认AND/OR关系。
    std::vector<稳定编码> 作用存在概念组;
    std::vector<稳定编码> 涉及特征类型概念组;
    std::vector<稳定编码> 可能结果概念组;
    friend bool operator==(const 新方法头定义&, const 新方法头定义&) = default;
};

struct 新方法参数特征规格 final {
    稳定编码 特征类型概念节点;
    std::optional<稳定编码> 值域概念节点;
    std::vector<稳定编码> 二次特征概念组;
    friend bool operator==(const 新方法参数特征规格&,
        const 新方法参数特征规格&) = default;
};

struct 新方法参数规格 final {
    稳定编码 参数角色节点;
    std::optional<稳定编码> 存在概念节点;
    bool 要求场景 = false;
    std::vector<新方法参数特征规格> 特征规格组;
    friend bool operator==(const 新方法参数规格&,
        const 新方法参数规格&) = default;
};

struct 新方法条件项定义 final {
    稳定编码 参数角色节点;
    std::optional<稳定编码> 存在概念节点;
    std::optional<稳定编码> 特征类型概念节点;
    std::optional<稳定编码> 特征值域概念节点;
    std::optional<稳定编码> 二次特征概念节点;
    std::optional<稳定编码> 状态或动态概念节点;
    friend bool operator==(const 新方法条件项定义&,
        const 新方法条件项定义&) = default;
};

struct 新方法结果项定义 final {
    稳定编码 参数角色节点;
    std::optional<稳定编码> 特征类型概念节点;
    std::optional<稳定编码> 特征值域概念节点;
    std::optional<稳定编码> 变化方向概念节点;
    std::optional<稳定编码> 状态或动态概念节点;
    bool 主要结果 = false;
    friend bool operator==(const 新方法结果项定义&,
        const 新方法结果项定义&) = default;
};

// 数值越大代表对目标事实的直接性要求越高。间接正式证据必须保留其
// 直接证明的陈述或来源事实，不能伪装成对目标状态的直接观察。
enum class 新方法必要证据质量 : std::int64_t {
    正式间接证据 = 1,
    正式直接事实 = 2
};

// 冲突不是可选择的“成功关系”。无论采用哪种组合方式，冲突都必须
// 保留并交影响评估处理，不能多数表决或任选有利结果。
enum class 新方法多证据关系 : std::int64_t {
    任一满足 = 1,
    共同满足 = 2,
    优先备用 = 3
};

struct 新方法结果确认要求 final {
    新方法必要证据质量 必要证据质量 =
        新方法必要证据质量::正式直接事实;
    // 空表示没有固定的时长上限；这不等于允许永远丢失结果。
    std::optional<std::int64_t> 最大等待纳秒;
    // 非空时表示必须等待该正式条件具备；不是线程定时器或轮询次数。
    std::optional<稳定编码> 等待条件概念节点;
    新方法多证据关系 多证据关系 = 新方法多证据关系::任一满足;
    // 空组表示没有具名禁止结果。非空时使用禁止结果关系规则判断
    // 是否命中；命中任何正式禁止组合都阻断完成。
    新方法项关系规则 禁止结果关系规则 = 新方法项关系规则::任一成立;
    std::vector<新方法结果项定义> 禁止结果项组;
    friend bool operator==(const 新方法结果确认要求&,
        const 新方法结果确认要求&) = default;
};

struct 新方法条件定义 final {
    新方法项关系规则 关系规则 = 新方法项关系规则::单项;
    std::vector<新方法条件项定义> 条件项组;
    friend bool operator==(const 新方法条件定义&,
        const 新方法条件定义&) = default;
};

struct 新方法结果定义 final {
    新方法项关系规则 关系规则 = 新方法项关系规则::单项;
    std::vector<新方法结果项定义> 结果项组;
    新方法结果确认要求 确认要求;
    friend bool operator==(const 新方法结果定义&,
        const 新方法结果定义&) = default;
};

struct 新方法条件结果定义 final {
    新方法条件定义 条件;
    新方法结果定义 结果;
    friend bool operator==(const 新方法条件结果定义&,
        const 新方法条件结果定义&) = default;
};

struct 新方法知识建立请求 final {
    新方法作用域 作用域 = 新方法作用域::普通世界;
    新方法实现方式 实现方式 = 新方法实现方式::本能;
    新方法头定义 方法头;
    std::vector<新方法参数规格> 参数规格组;
    std::vector<新方法条件结果定义> 条件结果组;
};

struct 新方法条件结果信息 final {
    稳定编码 配对节点;
    稳定编码 条件节点;
    稳定编码 结果节点;
    新方法条件定义 条件;
    新方法结果定义 结果;
    friend bool operator==(const 新方法条件结果信息&,
        const 新方法条件结果信息&) = default;
};

struct 新方法组合步骤信息 final {
    稳定编码 步骤节点;
    稳定编码 调用方法节点;
    std::int64_t 顺序 = 0;
    friend bool operator==(const 新方法组合步骤信息&,
        const 新方法组合步骤信息&) = default;
};

struct 新方法结果转移信息 final {
    稳定编码 转移节点;
    稳定编码 当前步骤节点;
    稳定编码 当前结果节点;
    std::optional<稳定编码> 匹配结果概念节点;
    稳定编码 下一步骤节点;
    friend bool operator==(const 新方法结果转移信息&,
        const 新方法结果转移信息&) = default;
};

struct 新方法信息 final {
    稳定编码 节点;
    新方法作用域 作用域 = 新方法作用域::普通世界;
    新方法实现方式 实现方式 = 新方法实现方式::本能;
    新方法启用状态 启用状态 = 新方法启用状态::启用;
    稳定编码 方法头节点;
    新方法头定义 方法头;
    稳定编码 方法虚拟存在节点;
    std::vector<新方法参数规格> 参数规格组;
    std::vector<新方法条件结果信息> 条件结果组;
    std::optional<std::string> 本能函数登记名;
    std::vector<新方法组合步骤信息> 组合步骤组;
    std::optional<稳定编码> 组合入口步骤节点;
    std::vector<新方法结果转移信息> 结果转移组;
    // 抽象方法均为空；副本保留其来源，整份复制图共用一个实例根。
    std::optional<稳定编码> 来源方法节点;
    std::optional<稳定编码> 实例根节点;
    friend bool operator==(const 新方法信息&, const 新方法信息&) = default;
};

enum class 新方法复制状态 : std::uint8_t {
    已复制 = 1, 入口拒绝, 尚未初始化, 方法不存在,
    来源不是抽象方法, 结构不一致, 资源失败
};

struct 新方法节点映射 final {
    稳定编码 源节点;
    稳定编码 实例节点;
};

struct 新方法复制结果 final {
    新方法复制状态 状态 = 新方法复制状态::入口拒绝;
    std::optional<稳定编码> 方法实例节点;
    std::vector<新方法节点映射> 节点映射组;
};

struct 新方法混合实例信息 final {
    稳定编码 节点;
    std::vector<稳定编码> 候选方法实例根组;
};

struct 新方法混合实例结果 final {
    新方法复制状态 状态 = 新方法复制状态::入口拒绝;
    std::optional<稳定编码> 实例根节点;
};

struct 新方法实例配对信息 final {
    稳定编码 方法节点;
    新方法条件结果信息 配对;
};

struct 新方法实例配对结果 final {
    新方法操作状态 状态 = 新方法操作状态::入口拒绝;
    std::vector<新方法实例配对信息> 配对组;
};

struct 新方法建立结果 final {
    新方法操作状态 状态 = 新方法操作状态::入口拒绝;
    std::optional<稳定编码> 方法节点;
};

struct 新方法结构节点结果 final {
    新方法操作状态 状态 = 新方法操作状态::入口拒绝;
    std::optional<稳定编码> 节点;
};

struct 新方法候选查询 final {
    std::optional<新方法作用域> 作用域;
    std::optional<新方法实现方式> 实现方式;
    std::vector<稳定编码> 作用存在概念组;
    std::vector<稳定编码> 涉及特征类型概念组;
    std::vector<稳定编码> 可能结果概念组;
};

struct 新方法候选结果 final {
    新方法操作状态 状态 = 新方法操作状态::入口拒绝;
    std::vector<稳定编码> 方法节点组;
};

struct 新方法组合步骤添加请求 final {
    稳定编码 组合方法节点;
    稳定编码 调用方法节点;
    std::int64_t 顺序 = 0;
};

struct 新方法结果转移添加请求 final {
    稳定编码 组合方法节点;
    稳定编码 当前步骤节点;
    稳定编码 当前结果节点;
    std::optional<稳定编码> 匹配结果概念节点;
    稳定编码 下一步骤节点;
};

} // namespace 海中鱼巣
