#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

#include "新_场景类.h"
#include "新_本能函数集.h"
#include "新_方法公共类型.h"
#include "任务执行采用.数据.h"

namespace 海中鱼巣 {

enum class 新方法问题种类 : std::uint8_t {
    方向不符 = 1, 值域不符, 预计变化未发生, 发生计划外变化
};
enum class 新场景快照阶段 : std::uint8_t { 动作前 = 1, 结果取得时 };
enum class 新场景快照对象种类 : std::uint8_t {
    场景 = 1, 存在, 特征, 特征值材料, 状态, 动态,
    特征概念, 存在概念, 动态概念, 二次特征概念, 其它引用
};
enum class 新场景快照引用状态 : std::uint8_t {
    边界未展开 = 1, 已知可读未展开, 缺失或不可读, 资源失败
};
struct 新场景快照外部引用 final {
    新场景快照对象种类 种类 = 新场景快照对象种类::其它引用;
    稳定编码 来源节点;
    新场景快照引用状态 状态 = 新场景快照引用状态::缺失或不可读;
    friend bool operator==(const 新场景快照外部引用&, const 新场景快照外部引用&) = default;
};
struct 新场景快照缺口 final {
    新场景快照对象种类 种类 = 新场景快照对象种类::其它引用;
    稳定编码 来源节点;
    新场景快照引用状态 原因 = 新场景快照引用状态::缺失或不可读;
    friend bool operator==(const 新场景快照缺口&, const 新场景快照缺口&) = default;
};

// 来源身份只用于追溯。内容是采集窗口内逐owner形成的值副本，不是原子世界快照。
struct 新执行场景快照 final {
    新场景快照阶段 阶段 = 新场景快照阶段::动作前;
    std::optional<新状态强时间> 采集开始, 采集结束;
    std::vector<稳定编码> 入口场景组;
    std::vector<新场景信息> 场景组;
    std::vector<新存在信息> 存在组;
    std::vector<新特征信息> 特征组;
    std::vector<特征概念信息> 特征概念组;
    std::vector<存在概念信息> 存在概念组;
    std::vector<新特征值信息> 值材料组;
    std::vector<新状态信息> 状态组;
    std::vector<新动态信息> 动态组;
    std::vector<新场景快照外部引用> 外部引用组;
    std::vector<新场景快照缺口> 缺口组;
    friend bool operator==(const 新执行场景快照&, const 新执行场景快照&) = default;
};

enum class 新方法问题采用来源 : std::uint8_t { 旧方法副本 = 1, 任务执行组织 = 2 };

struct 新方法问题反馈材料 final {
    // 实例方法节点、采用配对仅用于旧方法副本；新材料明确保存任务采用。
    稳定编码 任务节点, 执行实例节点, 来源抽象方法节点, 实例方法节点;
    新方法条件结果信息 采用配对;
    std::optional<新本能函数本次条件结果对> 本次运行对;
    新执行场景快照 动作前场景, 结果场景;
    稳定编码 影响评估节点;
    std::int64_t 原方向验证值 = 0, 原值域验证值 = 0;
    std::vector<新方法问题种类> 问题组;
    std::vector<稳定编码> 符合预测状态组, 预计但未发生项组, 计划外状态组;
    std::vector<新状态信息> 实际状态组;
    新方法问题采用来源 采用来源 = 新方法问题采用来源::旧方法副本;
    std::optional<新任务本能采用材料> 任务采用;
    friend bool operator==(const 新方法问题反馈材料&, const 新方法问题反馈材料&) = default;
};
struct 新方法问题学习消息 final {
    稳定编码 方法节点, 任务节点, 执行实例节点, 归档节点;
    friend bool operator==(const 新方法问题学习消息&, const 新方法问题学习消息&) = default;
};
enum class 新方法问题归档状态 : std::uint8_t {
    已保存 = 1, 精确重复, 未找到, 入口拒绝, 身份冲突,
    结构不一致, 资源失败, 发布结果未知
};
struct 新方法问题归档结果 final {
    新方法问题归档状态 状态 = 新方法问题归档状态::入口拒绝;
    std::optional<稳定编码> 归档节点;
    friend bool operator==(const 新方法问题归档结果&, const 新方法问题归档结果&) = default;
};
struct 新方法问题归档读回结果 final {
    新方法问题归档状态 状态 = 新方法问题归档状态::入口拒绝;
    std::optional<稳定编码> 归档节点;
    std::shared_ptr<const 新方法问题反馈材料> 材料;
};

class 新方法问题归档接口 {
public:
    virtual ~新方法问题归档接口() = default;
    virtual 新方法问题归档结果 保存方法问题(const 新方法问题反馈材料&) noexcept = 0;
    virtual 新方法问题归档读回结果 读取方法问题归档(稳定编码 归档节点) const noexcept = 0;
    virtual 新方法问题归档读回结果 查询方法问题归档(
        稳定编码 任务节点, 稳定编码 执行实例节点) const noexcept = 0;
};

} // namespace 海中鱼巣
