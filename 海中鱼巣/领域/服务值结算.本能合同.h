#pragma once
#include <optional>
#include <string_view>
#include "新_方法公共类型.h"
#include "新_内部治理对象引用.h"
#include "新_状态类.h"
#include "新_需求类.h"

namespace 海中鱼巣 {
enum class 新服务值结算协作状态 : std::uint8_t {
    已完成 = 1, 精确重复 = 2, 非本方法 = 3, 来源不具备 = 4,
    已有其他来源 = 5, 尚无来源 = 6, 入口拒绝 = 7, 工程停止 = 8, 目标已满足 = 9,
    // 仅表示manager收存消息，不表示来源绑定、方法执行或结算完成。
    已接收待筹办 = 10
};
enum class 新服务值结算改变进度 : std::uint8_t { 未改变 = 1, 已改变 = 2, 未知 = 3 };
struct 新服务值结算登记材料 final {
    稳定编码 自我所在场景, 自我存在, 服务值特征, 服务根需求;
    稳定编码 服务值类型根, 任务状态类型根, 任务完成值域;
    稳定编码 服务值合法值域, 动作前单值域;
    稳定编码 增加方向概念, 不变方向概念, 准确相等概念, 目标值域;
    std::int64_t 冻结前值 = 0;
};
struct 新服务值结算登记结果 final {
    新服务值结算协作状态 状态 = 新服务值结算协作状态::入口拒绝;
    std::optional<稳定编码> 方法节点, 结算需求节点;
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
    virtual 新需求建立结果 确保首子需求(稳定编码 方法节点) noexcept = 0;
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
    virtual void 报告待处理工程异常() noexcept = 0;
};
using 新服务值结算工程报告函数 = void (*)(std::string_view) noexcept;
} // namespace 海中鱼巣
