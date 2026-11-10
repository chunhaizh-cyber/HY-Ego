#pragma once
#include "模拟学习.数据.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "../核心/基础数据集.h"
#include "概念_特征类.h"

namespace 海中鱼巣 {

enum class 新方法作用域 : std::int64_t {
    内部治理 = 1,
    普通世界 = 2
};

enum class 新方法实现方式 : std::int64_t {
    本能 = 1,
    组合 = 2
};

enum class 新方法行为 : std::int64_t { 读取 = 1, 改变 = 2 };

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
    资源失败 = 20,
    参数材料未到 = 22,
    登记已退役 = 23,
    定义前提已变化 = 24
};

struct 新方法头定义 final {
    // 方法头只用于召回。集合间没有默认AND/OR关系。
    std::vector<稳定编码> 作用存在概念组;
    std::vector<稳定编码> 涉及特征类型概念组;
    std::vector<稳定编码> 可能结果概念组;
    friend bool operator==(const 新方法头定义&, const 新方法头定义&) = default;
};

// 待形成能力的目标，不是已掌握的方法知识。来源T→D由自我/任务owner确认，
// 方法owner仅保存引用，不代行任务归属裁决。单目标FT；域+比较关系或方向二选一。
// 可能结果概念组恰为方向概念，或目标域及比较概念。读取返回引用材料；
// 不冻结被引用概念的未来定义，后续消费者必须fresh复核，不能据此直接生成方法。
struct 新缺失方法登记请求 final {
    新方法头定义 方法头;
    std::optional<稳定编码> 目标值域概念节点;
    std::optional<稳定编码> 目标比较关系概念节点;
    std::optional<稳定编码> 目标变化方向概念节点;
    稳定编码 来源需求节点;
    稳定编码 来源任务节点;
    friend bool operator==(const 新缺失方法登记请求&, const 新缺失方法登记请求&) = default;
};

struct 新缺失方法信息 final {
    稳定编码 方法头节点;
    新缺失方法登记请求 定义;
    friend bool operator==(const 新缺失方法信息&, const 新缺失方法信息&) = default;
};

struct 新缺失方法登记结果 final {
    新方法操作状态 状态 = 新方法操作状态::入口拒绝;
    std::optional<新缺失方法信息> 信息;
};

// 旧角色格式继续用于尚未迁移的方法和已发生的执行材料。新格式的
// 序号是位置，不是节点编码；不能写入旧角色字段或据此创建角色概念。
enum class 新方法参数信息类型 : std::int64_t {
    存在 = 1,
    特征 = 2
};

struct 新方法有序参数规格 final {
    std::uint32_t 序号 = 0;
    新方法参数信息类型 信息类型 = 新方法参数信息类型::存在;
    稳定编码 范围概念节点;
    // R97（5350 §6）：存在位置对象须具有的特征类型根，升序去重；特征位置为空。
    std::vector<稳定编码> 必需特征类型组;
    // R97：特征位置所属存在位置的序号；存在位置为0。
    std::uint32_t 所属存在位置 = 0;
    friend bool operator==(const 新方法有序参数规格&,
        const 新方法有序参数规格&) = default;
};

// 声明侧统一把必需特征类型组整理为升序去重，登记校验只接受整理后的形状。
inline 新方法有序参数规格 整理有序参数规格(新方法有序参数规格 规格) {
    std::sort(规格.必需特征类型组.begin(), 规格.必需特征类型组.end());
    规格.必需特征类型组.erase(std::unique(规格.必需特征类型组.begin(), 规格.必需特征类型组.end()),
        规格.必需特征类型组.end());
    return 规格;
}
// 方法头涉及特征类型：有序规格汇总（5350 §2）——存在位置必需类型 ∪ 特征位置范围概念，升序去重。
inline std::vector<稳定编码> 有序规格涉及特征类型(const std::vector<新方法有序参数规格>& 组) {
    std::vector<稳定编码> 结果;
    for (const auto& 项 : 组) {
        if (项.信息类型 == 新方法参数信息类型::存在)
            结果.insert(结果.end(), 项.必需特征类型组.begin(), 项.必需特征类型组.end());
        else 结果.push_back(项.范围概念节点);
    }
    std::sort(结果.begin(), 结果.end());
    结果.erase(std::unique(结果.begin(), 结果.end()), 结果.end());
    return 结果;
}

enum class 新外设消息方向 : std::int64_t;
enum class 新方法模拟输入时机 : std::int64_t { 调用前=1, 参数新增消息后=2 };
struct 新方法模拟消息触发 final {
    std::uint32_t 特征参数序号=0;
    新外设消息方向 方向{};
    friend bool operator==(const 新方法模拟消息触发&,const 新方法模拟消息触发&)=default;
};
struct 新方法模拟消息输入定义 final {
    std::uint32_t 顺序=0;
    std::uint32_t 目标特征参数序号=0,窗口存在参数序号=0,对端存在参数序号=0;
    新外设消息方向 方向{};
    稳定编码 正文范围概念;
    bool 正文非空=false;
    新方法模拟输入时机 时机=新方法模拟输入时机::调用前;
    std::optional<新方法模拟消息触发> 触发;
    friend bool operator==(const 新方法模拟消息输入定义&,const 新方法模拟消息输入定义&)=default;
};
struct 新方法模拟输入方案定义 final {
    std::uint32_t 顺序=0;
    std::vector<新方法模拟消息输入定义> 消息组;
    friend bool operator==(const 新方法模拟输入方案定义&,const 新方法模拟输入方案定义&)=default;
};

struct 新方法有序参数引用 final {
    std::uint32_t 序号 = 0;
    新方法参数信息类型 信息类型 = 新方法参数信息类型::存在;
    稳定编码 信息节点;
    friend bool operator==(const 新方法有序参数引用&,
        const 新方法有序参数引用&) = default;
};

struct 新方法参数特征规格 final {
    稳定编码 特征类型概念节点;
    std::optional<稳定编码> 值域概念节点;
    std::vector<稳定编码> 二次特征概念组;
    friend bool operator==(const 新方法参数特征规格&,
        const 新方法参数特征规格&) = default;
};

enum class 新方法参数对象种类 : std::int64_t {
    普通世界 = 1,
    任务虚拟存在 = 2,
    服务要求来源人类 = 3,
    服务要求目标存在 = 4,
    服务要求目标特征类型 = 5,
    服务要求目标定义 = 6,
    服务要求确认 = 7
};

// 任务角色绑定的同一个 T 必须属于另一角色的指定集合特征当前值。
struct 新方法参数集合成员约束 final {
    稳定编码 集合参数角色节点;
    稳定编码 集合特征类型概念节点;
    friend bool operator==(const 新方法参数集合成员约束&,
        const 新方法参数集合成员约束&) = default;
};

struct 新方法参数规格 final {
    稳定编码 参数角色节点;
    std::optional<稳定编码> 存在概念节点;
    bool 要求场景 = false;
    std::vector<新方法参数特征规格> 特征规格组;
    新方法参数对象种类 对象种类 = 新方法参数对象种类::普通世界;
    std::optional<新方法参数集合成员约束> 集合成员约束;
    // 有值时旧角色/存在/特征规格/集合约束必须全部为空。两种物理
    // 格式互斥，不能因新字段缺失而猜测旧角色编码就是参数序号。
    std::optional<新方法有序参数规格> 有序规格;
    friend bool operator==(const 新方法参数规格&,
        const 新方法参数规格&) = default;
};

enum class 新方法内容作用点 : std::uint8_t { 特征当前引用=1, 参数自身=2 };
struct 新方法引用内容条件 final {
    std::vector<稳定编码> 正式内容路径;
    稳定编码 要求概念节点;
    新方法内容作用点 作用点=新方法内容作用点::特征当前引用;
    bool 要求认知来源当前有效=false;
    friend bool operator==(const 新方法引用内容条件&,
        const 新方法引用内容条件&) = default;
};

enum class 新方法当前值状态 : std::uint8_t { 未知=1, 已知=2 };
struct 新方法特征前态限定 final {
    新方法当前值状态 当前值状态=新方法当前值状态::未知;
    std::optional<std::uint64_t> 历史不同值数量;
    friend bool operator==(const 新方法特征前态限定&, const 新方法特征前态限定&)=default;
};

struct 新方法条件项定义 final {
    稳定编码 参数角色节点;
    std::optional<稳定编码> 存在概念节点;
    std::optional<稳定编码> 特征类型概念节点;
    std::optional<稳定编码> 特征值域概念节点;
    std::optional<稳定编码> 二次特征概念节点;
    std::optional<稳定编码> 状态或动态概念节点;
    // 本项自身的有限I64区间，与特征值域概念节点互斥；单位沿特征类型定义。
    std::optional<特征概念I64闭区间> 精确数值域;
    // 0仅指旧角色格式；非零时参数角色节点必须为空。
    std::uint32_t 参数序号 = 0;
    std::uint32_t 任务成员变量序号 = 0;
    std::optional<新方法引用内容条件> 引用内容条件;
    std::optional<新方法特征前态限定> 特征前态限定;
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
    // 禁止结果项复用本结构，同样不得同时保存概念域和私有精确域。
    std::optional<特征概念I64闭区间> 精确数值域;
    std::uint32_t 参数序号 = 0;
    std::uint32_t 任务成员变量序号 = 0;
    std::optional<新方法引用内容条件> 引用内容结果;
    // 先天被动结果：执行后，参数序号所绑定需求的目标由外部事件（如对方回答写回）被动达成。
    // 只出现在先天配对中，与特征、值域、方向、状态、引用内容结果互斥。
    bool 被动需求目标 = false;
    // 2026-11-10用户裁决：结果不依附参数——直接写“具体存在的某特征类型落在某值域”（如“自我.交互者 ∈ 存在根”）。
    // 与参数序号、参数角色、被动需求目标、引用内容结果、任务成员变量互斥；须有特征类型与值域（或精确域）。
    std::optional<稳定编码> 结果作用存在;
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

struct 新方法初始场景事实限定 final {
    稳定编码 关系概念节点;
    std::uint32_t 场景参数序号=0,主体参数序号=0,对象参数序号=0,特征参数序号=0;
    friend bool operator==(const 新方法初始场景事实限定&,const 新方法初始场景事实限定&)=default;
};

struct 新方法条件定义 final {
    新方法项关系规则 关系规则 = 新方法项关系规则::单项;
    std::vector<新方法条件项定义> 条件项组;
    std::vector<新方法参数关系> 参数关系组;
    std::vector<新方法初始场景事实限定> 场景事实限定组;
    friend bool operator==(const 新方法条件定义&,
        const 新方法条件定义&) = default;
};

struct 新方法本次场景事实引用 final {
    新方法初始场景事实限定 限定;
    稳定编码 事实来源身份;
    稳定编码 动作节点;
    friend bool operator==(const 新方法本次场景事实引用&,
        const 新方法本次场景事实引用&) = default;
};

struct 新方法结果定义 final {
    新方法项关系规则 关系规则 = 新方法项关系规则::单项;
    std::vector<新方法结果项定义> 结果项组;
    新方法结果确认要求 确认要求;
    std::vector<新方法参数关系> 参数关系组;
    std::vector<新方法初始场景事实限定> 场景事实限定组;
    friend bool operator==(const 新方法结果定义&,
        const 新方法结果定义&) = default;
};
struct 新方法执行分支定义 final {
    std::uint32_t 顺序=0;
    新方法条件定义 必要条件;
    friend bool operator==(const 新方法执行分支定义&,const 新方法执行分支定义&)=default;
};
struct 新方法执行分支信息 final {
    稳定编码 节点;
    新方法执行分支定义 定义;
    friend bool operator==(const 新方法执行分支信息&,const 新方法执行分支信息&)=default;
};

// 输入上升时输出的方向，区别于一次动作前后的结果变化方向。
enum class 新方法条件结果线性方向 : std::int8_t {
    反向 = -1, 不变 = 0, 同向 = 1
};

struct 新方法条件结果线性关系 final {
    std::size_t 条件序号 = 0;
    std::size_t 结果序号 = 0;
    新方法条件结果线性方向 方向 = 新方法条件结果线性方向::不变;
    特征概念I64闭区间 原条件域;
    特征概念I64闭区间 原结果域;
    friend bool operator==(const 新方法条件结果线性关系&,
        const 新方法条件结果线性关系&) = default;
};

struct 新方法条件结果定义 final {
    新方法条件定义 条件;
    新方法结果定义 结果;
    std::vector<新方法条件结果线性关系> 线性关系组;
    std::vector<稳定编码> 来源执行分支节点组;
    friend bool operator==(const 新方法条件结果定义&,
        const 新方法条件结果定义&) = default;
};

struct 新方法初始必要条件定义 final {
    新方法条件定义 条件;
    std::vector<新方法初始场景事实限定> 场景事实限定组;
    friend bool operator==(const 新方法初始必要条件定义&,const 新方法初始必要条件定义&)=default;
};
struct 新方法知识建立请求 final {
    新方法作用域 作用域 = 新方法作用域::普通世界;
    新方法实现方式 实现方式 = 新方法实现方式::本能;
    新方法头定义 方法头;
    std::vector<新方法参数规格> 参数规格组;
    std::vector<新方法条件结果定义> 条件结果组;
    std::optional<新方法行为> 行为;
    std::vector<新方法参数关系> 初始模拟关系组;
    std::vector<std::uint32_t> 初始模拟事件参数组;
    std::vector<新方法模拟输入方案定义> 初始模拟输入方案组;
    std::optional<新方法初始必要条件定义> 初始必要条件;
    std::vector<新方法执行分支定义> 执行分支组;
};

struct 新方法条件结果信息 final {
    稳定编码 配对节点;
    稳定编码 条件节点;
    稳定编码 结果节点;
    新方法条件定义 条件;
    新方法结果定义 结果;
    std::int64_t 命中次数 = 0;
    // 原始模型随实例复制；收窄当前条件不能重释这些端点。
    std::vector<新方法条件结果线性关系> 线性关系组;
    // 实例配对指向最初的抽象配对；抽象配对为空，旧实例允许缺省。
    std::optional<稳定编码> 来源配对节点;
    新方法知识来源种类 知识来源 = 新方法知识来源种类::未注明;
    std::vector<稳定编码> 来源执行分支节点组;
    friend bool operator==(const 新方法条件结果信息&,
        const 新方法条件结果信息&) = default;
};

enum class 新方法学习发布状态 : std::uint8_t {
    已发布 = 1, 精确重复, 入口拒绝, 方法不可用,
    条件结果不合法, 结构不一致, 资源失败, 发布结果未知, 同条件结果冲突,
    前提已变化
};

struct 新方法学习发布结果 final {
    新方法学习发布状态 状态 = 新方法学习发布状态::入口拒绝;
    std::vector<稳定编码> 新配对节点组;
    std::optional<稳定编码> 已有配对节点;
};

// TRIAL-LEARNING-DIRECT：尝试执行实际结果入方法知识的候选（无评审授权，由方法owner直接发布）。
enum class 新尝试补全差异种类 : std::uint8_t { 条件收窄 = 1, 结果新增, 结果关系新增 };
struct 新尝试补全差异项 final {
    新尝试补全差异种类 种类 = 新尝试补全差异种类::结果新增;
    std::uint32_t 参数序号 = 0;
    std::uint32_t 右参数序号 = 0;  // 仅结果关系新增
    friend bool operator==(const 新尝试补全差异项&, const 新尝试补全差异项&) = default;
};
// 学习侧可重建、不持久；同键（方法、原配对、执行实例、规则版本）同义复用。
struct 新尝试补全候选 final {
    稳定编码 方法节点, 原配对节点, 执行实例节点;
    新方法条件定义 原配对条件;
    std::uint32_t 规则版本 = 1;
    新方法条件定义 实际条件;
    新方法结果定义 实际结果;
    std::vector<稳定编码> 来源执行分支节点组;
    bool 与原条件相同 = false;
    std::vector<新尝试补全差异项> 差异项组;
    friend bool operator==(const 新尝试补全候选&, const 新尝试补全候选&) = default;
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
    // 受控声明迁移的正式代际（结算Q、获取服务对象有序参数）；与任务副本来源严格分账。
    std::optional<稳定编码> 父代方法节点;
    稳定编码 启用特征节点, 专属存在概念节点;
    std::optional<新方法行为> 行为;
    std::vector<新方法参数关系> 初始模拟关系组;
    std::vector<std::uint32_t> 初始模拟事件参数组;
    std::vector<新方法模拟输入方案定义> 初始模拟输入方案组;
    std::optional<新方法初始必要条件定义> 初始必要条件;
    std::vector<新方法执行分支信息> 执行分支组;
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
