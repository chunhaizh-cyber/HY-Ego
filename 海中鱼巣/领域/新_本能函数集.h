#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "新_方法参数场景.h"
#include "新_状态类.h"
#include "服务要求.材料合同.h"
#include "服务对象.材料合同.h"

namespace 海中鱼巣 {

class 新_世界树类;
class 新_任务类;

enum class 新本能函数动作阶段 : std::uint8_t {
    未进入动作 = 1,
    已进入动作,
    部分执行,
    已按函数合同返回,
    结果未知
};

enum class 新本能函数执行状态 : std::uint8_t {
    已按合同返回 = 1,
    异步已受理,
    入口拒绝,
    函数未登记,
    参数不匹配,
    依赖未装配,
    动作执行失败,
    动作结果未知,
    资源失败,
    内部错误,
    工程停止
};

// 本能函数只报告一次观察实际得到的二次特征材料；如何使用该材料判断
// 连接、需求或任务状态，属于上层治理职责。
struct 新本能二次特征观察结果 final {
    稳定编码 二次特征类型概念节点;
    std::int64_t 上次累计值 = 0;
    std::int64_t 本次累计值 = 0;
    std::int64_t 结果值 = 0;
    稳定编码 结果值域概念节点;

    friend bool operator==(const 新本能二次特征观察结果&,
        const 新本能二次特征观察结果&) = default;
};

// 条件侧只保存本能函数本次实际读取并使用的参数材料。这里的准确值是
// 调用期快照，不是第二份世界事实，也不写回方法Vm或参数场景。
struct 新本能函数条件特征材料 final {
    稳定编码 特征节点;
    std::optional<新特征准确值> 动作前准确值;

    friend bool operator==(const 新本能函数条件特征材料&,
        const 新本能函数条件特征材料&) = default;
};

struct 新本能函数条件角色材料 final {
    稳定编码 参数角色节点;
    std::optional<稳定编码> 世界场景节点;
    std::optional<稳定编码> 实际存在节点;
    std::vector<新本能函数条件特征材料> 实际特征组;
    std::optional<新任务虚拟存在引用> 任务虚拟存在引用;
    std::optional<新服务要求分项引用> 服务要求分项引用;
    std::optional<新服务要求确认引用> 服务要求确认引用;
    std::optional<新服务要求分项材料> 服务要求分项快照;
    std::optional<新服务要求确认目标> 服务要求确认快照;

    friend bool operator==(const 新本能函数条件角色材料&,
        const 新本能函数条件角色材料&) = default;
};

struct 新本能函数本次条件 final {
    std::vector<新本能函数条件角色材料> 参数角色组;

    friend bool operator==(const 新本能函数本次条件&,
        const 新本能函数本次条件&) = default;
};

enum class 新本能函数结果取得状态 : std::uint8_t {
    未取得 = 1,
    已取得,
    待取得,
    部分取得,
    无法取得
};

// 结果侧只报告本次函数调用实际到达的位置及已经形成的正式材料。
// 它不证明任务完成、需求满足、预测命中、服务结算或现实效果成立。
struct 新本能函数本次结果 final {
    新本能函数执行状态 状态 = 新本能函数执行状态::入口拒绝;
    新本能函数动作阶段 动作阶段 = 新本能函数动作阶段::未进入动作;
    新本能函数结果取得状态 结果取得状态 =
        新本能函数结果取得状态::未取得;
    std::optional<稳定编码> 动作动态节点;
    std::vector<稳定编码> 已形成状态节点组;
    std::vector<稳定编码> 已形成动态节点组;
    std::vector<新本能二次特征观察结果> 已形成二次特征结果组;
    std::vector<稳定编码> 建议观察特征节点组;
    std::optional<新服务需求信息包> 服务需求信息包;
    std::optional<新服务对象材料> 服务对象观察材料;

    friend bool operator==(const 新本能函数本次结果&,
        const 新本能函数本次结果&) = default;
};

// 这是一次运行材料，不是方法树中的长期条件—结果定义。即时结果不足时
// 右侧明确保持待取得或部分取得，后续由任务沿同次执行补齐并验证。
struct 新本能函数本次条件结果对 final {
    新本能函数本次条件 条件;
    新本能函数本次结果 结果;

    friend bool operator==(const 新本能函数本次条件结果对&,
        const 新本能函数本次条件结果对&) = default;
};

using 新本能函数 = 新本能函数本次条件结果对 (*)(
    const 方法参数场景& 参数场景) noexcept;

// 只保存本能函数实现。登记、解析、可用性检查、统一调用和外设端口
// 装配由新_本能函数管理类负责；本类不持有登记表或方法节点。
class 新_本能函数集 final {
public:
    // 以下三项是函数实现，不负责登记或外设端口所有权。
    static 新本能函数本次条件结果对 读取网页对话消息(
        const 方法参数场景& 参数场景) noexcept;
    static 新本能函数本次条件结果对 设置网页对话输入文本(
        const 方法参数场景& 参数场景) noexcept;
    static 新本能函数本次条件结果对 提交网页对话消息(
        const 方法参数场景& 参数场景) noexcept;

    // 登记阶段形成初始方法知识；真实动作只能经任务统一参数场景进入。
    static 新本能函数本次条件结果对 执行服务值结算(
        const 方法参数场景& 参数场景) noexcept;
    static 新本能函数本次条件结果对 主动获取服务需求(
        const 方法参数场景&) noexcept;
    static 新本能函数本次条件结果对 被动获取服务需求(
        const 方法参数场景&) noexcept;
    static 新本能函数本次条件结果对 获取服务对象(
        const 方法参数场景&) noexcept;
private:
    static 新本能函数本次条件结果对 获取服务需求(
        const 方法参数场景&, 新服务需求获取方式) noexcept;
};

} // namespace 海中鱼巣
