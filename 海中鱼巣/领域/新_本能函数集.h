#pragma once

#include <cstdint>
#include <optional>
#include <utility>
#include <vector>

#include "新_方法参数场景.h"
#include "新_状态类.h"
#include "服务要求.材料合同.h"
#include "服务对象.材料合同.h"

namespace 海中鱼巣 {

class 新_世界树类;
class 新_任务类;
struct 新字段询问答案读回;

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
    工程停止,
    // 入口复核发现目标已经达成（5380 §2.4）：零动作，动作阶段＝未进入动作，结果取得＝未取得，
    // 不建立动作、不写结果；任务侧走执行前目标收束（未执行但目标已满足）。实现层枚举（5360 §4）。
    目标已满足
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
    // 有序引用保留本次E/F身份；特征的动作前值仍在实际特征组中。
    std::optional<新方法有序参数引用> 有序引用;

    friend bool operator==(const 新本能函数条件角色材料&,
        const 新本能函数条件角色材料&) = default;
};

struct 新本能函数本次条件 final {
    std::vector<新本能函数条件角色材料> 参数角色组;
    std::optional<新任务贡献治理参数> 任务贡献治理参数;
    std::vector<新任务虚拟存在特征组> 治理动作前特征组;
    std::optional<稳定编码> 执行分支节点;

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
struct 新本能已发布需求结果 final {
    稳定编码 需求节点, 主体存在节点, 需求特征节点, 结果场景节点;
    稳定编码 本次动作节点, 首次发布动作节点;
    friend bool operator==(const 新本能已发布需求结果&, const 新本能已发布需求结果&) = default;
};
enum class 新本能已发布需求核验状态 : std::uint8_t {
    已核验=1, 前提已变化, 已关闭, 资源失败
};
struct 新本能已发布需求核验结果 final {
    新本能已发布需求核验状态 状态=新本能已发布需求核验状态::前提已变化;
    std::optional<新本能已发布需求结果> 结果;
    std::vector<基础联合节点快照> 完整保护;
};
enum class 新本能字段询问原因 : std::uint8_t {
    字段已有值=1, 字段不适用, 前提已变化, 定位未支持,
    缺语言知识, 缺名称, 缺指称, 多义, 已入列,
    // ASK-READ-SCENE v0.5（5380 §3.6）：自然语言世界中该问题已有答案，目标已满足，未问。
    问题已有答案
};
struct 新本能已提交输出结果 final {
    稳定编码 窗口节点{}, 输出队列特征节点{}, 消息节点{}, 动作节点{}, 入列状态{}, 入列动态{};
    std::uint64_t 提交序号=0;
    friend bool operator==(const 新本能已提交输出结果&,const 新本能已提交输出结果&)=default;
};
struct 新本能字段询问返回材料 final {
    新本能字段询问原因 原因=新本能字段询问原因::定位未支持;
    std::optional<正式内容引用定位> 正式定位;
    std::optional<正式内容值> 已读取字段;
    std::optional<新本能已提交输出结果> 已提交输出;
    // 已入列：本次建立的问题；问题已有答案：已有答案的问题（7240 §5.5）。
    std::optional<稳定编码> 问题节点;
    friend bool operator==(const 新本能字段询问返回材料& a,const 新本能字段询问返回材料& b) {
        if (a.原因!=b.原因 || a.正式定位.has_value()!=b.正式定位.has_value()
            || a.已读取字段.has_value()!=b.已读取字段.has_value() || a.已提交输出!=b.已提交输出
            || a.问题节点!=b.问题节点) return false;
        if (a.正式定位 && (a.正式定位->信息节点!=b.正式定位->信息节点 || a.正式定位->正式内容路径!=b.正式定位->正式内容路径)) return false;
        return !a.已读取字段 || (a.已读取字段->种类==b.已读取字段->种类
            && a.已读取字段->状态==b.已读取字段->状态 && a.已读取字段->值==b.已读取字段->值);
    }
};
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
    // 仅供格式2历史归档保真；生产返回非空值属于旧合同违规。
    std::optional<新服务需求信息包> 服务需求信息包;
    std::optional<新服务对象材料> 服务对象观察材料;
    std::optional<稳定编码> 服务值消费节点;
    // 纯治理发生与普通方法经验、动作和服务值消费分账。
    std::optional<新任务贡献治理返回材料> 任务贡献治理;
    std::vector<新方法本次场景事实引用> 已取得场景事实组;
    std::optional<新本能已发布需求结果> 已发布需求;
    std::optional<新本能字段询问返回材料> 字段询问;

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

// 只保存本能函数实现。函数入口注册、解析、统一调用和外设端口装配
// 由新_本能函数管理类负责；正式方法信息由函数内声明、经方法owner登记。
// 本类不持有注册表或方法节点，不以函数入口注册代替方法树中的正式信息。
class 新_本能函数集 final {
public:
    static 新本能函数本次条件结果对 获取交互者(
        const 方法参数场景& 场景) noexcept;
    static 新本能函数本次条件结果对 获取服务对象(
        const 方法参数场景&) noexcept;
    static 新本能函数本次条件结果对 获取服务对象新需求(
        const 方法参数场景&) noexcept;
    static 新本能函数本次条件结果对 询问指定需求字段(const 方法参数场景&) noexcept;
    // ASK-READ-SCENE v0.5 §3.2：询问等待答案（经字段询问端口，入参为出站来源即询问执行实例）。
    // 端口未装配属装配缺陷，按工程异常终止。读取只读；取得在找到答案时登记问题答案。
    static 新字段询问答案读回 读取字段询问执行问题答案(稳定编码 出站来源) noexcept;
    static 新字段询问答案读回 取得字段询问执行问题答案(稳定编码 出站来源) noexcept;
    static 新本能函数本次条件结果对 进入场景(const 方法参数场景&) noexcept;
};

} // namespace 海中鱼巣
