#pragma once
#include <cstdint>
#include <optional>
#include <string_view>
#include <vector>
#include "新_方法公共类型.h"
#include "新_内部治理对象引用.h"
#include "新_状态类.h"
#include "新_需求类.h"

namespace 海中鱼巣 {
class 方法参数场景;
struct 新本能函数本次条件结果对;
// 完成来源是已有的正式T/X/A，不为内部治理工作建立新的执行实例。
struct 新任务贡献反馈定位 final {
    稳定编码 任务节点, 执行实例节点, 影响评估节点;
    friend bool operator==(const 新任务贡献反馈定位&,
        const 新任务贡献反馈定位&) = default;
};
enum class 新任务贡献推进状态 : std::uint8_t {
    已完成 = 1, 精确重复, 等待直接子结算, 等待直接父任务,
    等待目标分类, 历史结算缺贡献, 无直接父任务,
    父完成结算已封口, 预期已变化, 工程故障, 等待结算配置
};
struct 新任务贡献推进结果 final {
    新任务贡献推进状态 状态 = 新任务贡献推进状态::工程故障;
    std::optional<稳定编码> 完成结算节点, 接收节点, 直接父任务节点;
    std::vector<稳定编码> 待归集直接子需求节点组;
    friend bool operator==(const 新任务贡献推进结果&,
        const 新任务贡献推进结果&) = default;
};
struct 新任务贡献治理对象引用 final {
    稳定编码 需求节点;
    std::optional<新任务虚拟存在引用> 任务引用;
    std::optional<稳定编码> 服务值特征节点, 治理场景节点;
    std::optional<稳定编码> 执行实例节点, 影响评估节点;
    std::optional<稳定编码> 完成结算节点, 接收节点;
    friend bool operator==(const 新任务贡献治理对象引用&,
        const 新任务贡献治理对象引用&) = default;
};
struct 新任务贡献治理参数 final {
    新任务贡献反馈定位 完成来源;
    新任务贡献治理对象引用 来源;
    std::optional<新任务贡献治理对象引用> 直接父;
    std::vector<新任务贡献治理对象引用> 直接子;
    friend bool operator==(const 新任务贡献治理参数&,
        const 新任务贡献治理参数&) = default;
};
enum class 新任务贡献治理参数读取状态 : std::uint8_t {
    已读取 = 1, 来源已变化
};
struct 新任务贡献治理参数读取结果 final {
    新任务贡献治理参数读取状态 状态 = 新任务贡献治理参数读取状态::来源已变化;
    std::optional<新任务贡献治理参数> 参数;
    friend bool operator==(const 新任务贡献治理参数读取结果&,
        const 新任务贡献治理参数读取结果&) = default;
};
struct 新任务贡献治理动作材料 final {
    新任务虚拟存在特征引用 目标;
    稳定编码 阶段节点, 治理场景节点, 动作节点;
    稳定编码 前状态节点, 后状态节点, 变化动态节点;
    friend bool operator==(const 新任务贡献治理动作材料&,
        const 新任务贡献治理动作材料&) = default;
};
struct 新任务贡献治理返回材料 final {
    新任务贡献反馈定位 来源;
    新任务贡献推进结果 推进;
    std::vector<新任务贡献治理动作材料> 本次动作组;
    friend bool operator==(const 新任务贡献治理返回材料&,
        const 新任务贡献治理返回材料&) = default;
};
struct 新任务贡献治理执行材料 final {
    std::optional<新任务贡献治理参数> 实际参数;
    std::vector<新任务虚拟存在特征组> 动作前特征组;
    新任务贡献治理返回材料 返回;
    friend bool operator==(const 新任务贡献治理执行材料&,
        const 新任务贡献治理执行材料&) = default;
};
struct 新服务值最终来源定位 final {
    稳定编码 来源任务节点, 完成结算节点;
    std::optional<稳定编码> 直接接收节点;
    friend bool operator==(const 新服务值最终来源定位&,
        const 新服务值最终来源定位&) = default;
};
enum class 新服务值最终消费状态 : std::uint8_t {
    已完成 = 1, 精确重复, 来源条件不具备, 来源已变化, 等待配置,
    等待同源工作, 尚未消费, 已入账待收尾
};
struct 新服务值最终消费结果 final {
    新服务值最终消费状态 状态 = 新服务值最终消费状态::尚未消费;
    新服务值最终来源定位 来源;
    std::optional<稳定编码> 消费节点, 普通动作节点, 收尾阶段节点;
};
enum class 新服务值结算协作状态 : std::uint8_t {
    已完成 = 1, 精确重复 = 2, 非本方法 = 3, 来源不具备 = 4,
    已有其他来源 = 5, 尚无来源 = 6, 入口拒绝 = 7, 工程停止 = 8, 目标已满足 = 9,
    // 仅表示manager收存消息，不表示来源绑定、方法执行或结算完成。
    已接收待筹办 = 10,
    // 新贡献来源等待最终原子消费提供者，不是本能业务结果或工程故障。
    最终贡献消费尚未装配 = 11
};
enum class 新服务值结算改变进度 : std::uint8_t { 未改变 = 1, 已改变 = 2, 未知 = 3 };
struct 新服务值结算登记材料 final {
    稳定编码 自我所在场景, 自我存在, 服务值特征, 服务根需求;
    稳定编码 服务值类型根, 任务状态类型根, 任务完成值域;
    稳定编码 服务值合法值域, 动作前单值域;
    稳定编码 增加方向概念, 不变方向概念, 准确相等概念, 目标值域;
    std::int64_t 冻结前值 = 0;
    稳定编码 服务任务集合类型根, 服务任务非空值域;
    稳定编码 自我概念, 任务服务值类型根, 任务服务值合法值域;
};
struct 新服务值结算登记结果 final {
    新服务值结算协作状态 状态 = 新服务值结算协作状态::入口拒绝;
    std::optional<稳定编码> 方法节点;
    std::optional<新服务值结算登记材料> 材料;
};
struct 新服务值结算动作回执 final {
    新服务值结算协作状态 状态 = 新服务值结算协作状态::入口拒绝;
    std::optional<稳定编码> 动作节点;
    std::optional<新状态强时间> 动作时间;
};
struct 新服务值结算变化回执 final {
    新服务值结算协作状态 状态 = 新服务值结算协作状态::入口拒绝;
    新服务值结算改变进度 改变进度 = 新服务值结算改变进度::未改变;
    std::optional<稳定编码> 前状态节点, 后状态节点, 结果动态节点;
};
struct 新服务值结算确认结果 final {
    新服务值结算协作状态 状态 = 新服务值结算协作状态::入口拒绝;
    std::optional<稳定编码> 内部动作节点;
    std::optional<新任务虚拟存在读回> 来源任务读回;
};
class 新服务值结算本能端口 {
public:
    virtual ~新服务值结算本能端口() = default;
    virtual std::optional<新服务值结算登记材料> 读取登记材料() const noexcept = 0;
    virtual 新方法建立结果 确保方法登记(const 新方法知识建立请求&) noexcept = 0;
    virtual 新本能函数本次条件结果对 消费最终来源(
        const 方法参数场景&) noexcept = 0;
    virtual 新本能函数本次条件结果对 执行贡献治理场景(
        const 方法参数场景&) noexcept = 0;
    virtual 新服务值结算动作回执 建立普通动作(稳定编码 方法节点,
        const 新任务虚拟存在引用& 来源, 稳定编码 场景节点, 稳定编码 自我节点,
        稳定编码 服务值特征节点, std::int64_t 动作前值) noexcept = 0;
    virtual 新服务值结算变化回执 更新普通结果(稳定编码 动作节点,
        稳定编码 场景节点, 稳定编码 自我节点, 稳定编码 服务值特征节点,
        std::int64_t 目标值) noexcept = 0;
    virtual void 报告并停止(std::string_view 具名错误) noexcept = 0;
};
class 新服务值结算确认接口 {
public:
    virtual ~新服务值结算确认接口() = default;
    virtual void 报告并停止(std::string_view 具名错误) noexcept = 0;
    virtual 新服务值结算登记结果 初始化(稳定编码 场景, 稳定编码 自我,
        稳定编码 服务特征, 稳定编码 服务根) noexcept = 0;
    virtual 新服务值结算确认结果 确认并收尾(稳定编码 结算任务节点,
        稳定编码 执行实例节点, 稳定编码 影响评估节点) noexcept = 0;
    virtual 新服务值最终消费结果 执行最终来源工作(
        const 新服务值最终来源定位&) noexcept = 0;
    virtual 新本能函数本次条件结果对 执行贡献治理工作(
        const 新任务贡献反馈定位&) noexcept = 0;
    virtual 新服务值最终消费结果 读取最终来源结果(
        const 新服务值最终来源定位&) const noexcept = 0;
    virtual void 报告待处理工程异常() noexcept = 0;
};
using 新服务值结算工程报告函数 = void (*)(std::string_view) noexcept;
} // namespace 海中鱼巣
