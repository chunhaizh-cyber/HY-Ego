// 文件规则：本模块只定义分层安全维护与任务权限的纯值协议，不拥有仓库、核心写入类型或生产接线。
module;

#include "../核心/句柄.h"

#include <array>
#include <cstdint>
#include <optional>
#include <vector>

export module 海中鱼巣.领域.协议.分层安全维护;

export namespace 海中鱼巣 {

inline constexpr std::uint32_t 分层安全维护合同ABI = 1;
inline constexpr std::uint32_t 分层安全维护规则版本 = 1;
inline constexpr std::uint32_t 安全权限规则版本 = 1;
inline constexpr std::uint32_t 安全任务排序规则版本 = 1;
inline constexpr std::int64_t 安全权限满刻度 = 1'000'000;
inline constexpr std::int64_t 安全最小保留倍率 = 10'000;
inline constexpr std::int64_t 安全最大释放倍率 = 990'000;

enum class 安全结果分类 : std::uint32_t {
    已形成 = 1,
    已维护 = 2,
    无变化 = 3,
    未归层 = 4,
    权限为零 = 5,
    当前无可执行任务 = 6,
    精确重复 = 7,
    入口拒绝 = 8,
    版本漂移 = 9,
    材料缺失 = 10,
    结构拒绝 = 11,
    资源失败 = 12,
    内部不一致 = 13
};

enum class 适用性 : std::uint32_t {
    不适用 = 1,
    适用 = 2
};

enum class 记录当前性 : std::uint32_t {
    当前 = 1,
    失效 = 2
};

enum class 因素搜索状态 : std::uint32_t {
    未开始 = 1,
    搜索中 = 2,
    已完成 = 3
};

enum class 安全因素状态 : std::uint32_t {
    已确认仍相关 = 1,
    待验证 = 2,
    已验证排除 = 3,
    证据失效 = 4
};

enum class 因素排除结论 : std::uint32_t {
    未成立 = 1,
    已验证排除 = 2,
    证据失效 = 3
};

enum class 复发机会状态 : std::uint32_t {
    不成立 = 1,
    成立 = 2,
    材料缺失 = 3,
    版本漂移 = 4
};

enum class 感知覆盖状态 : std::uint32_t {
    不成立 = 1,
    成立 = 2,
    材料缺失 = 3,
    版本漂移 = 4
};

enum class 身份比较状态 : std::uint32_t {
    不成立 = 1,
    成立 = 2,
    材料缺失 = 3,
    版本漂移 = 4
};

enum class 复发结论 : std::uint32_t {
    确认未复发 = 1,
    确认复发 = 2,
    无法裁决 = 3
};

enum class 安全维护方向 : std::uint32_t {
    无变化 = 1,
    低位回升 = 2,
    高位回落 = 3,
    主动事实优先 = 4
};

enum class 安全组权限状态 : std::uint32_t {
    已形成 = 1,
    权限为零 = 2,
    材料缺口 = 3,
    版本漂移 = 4
};

enum class 安全因果信息类型 : std::uint32_t {
    普通因果结果 = 1,
    具体安全结果 = 2,
    安全根结果 = 3
};

enum class 安全发生状态 : std::uint32_t {
    尚未发生 = 1,
    正在发生 = 2,
    已到达 = 3
};

enum class 安全值变化方向 : std::uint32_t {
    无变化 = 1,
    安全值增加 = 2,
    安全值减少 = 3
};

using 安全五组权限值 = std::array<std::int64_t, 5>;
using 安全四门释放倍率 = std::array<std::int64_t, 4>;
using 安全五组权限状态值 = std::array<安全组权限状态, 5>;
using 安全四门限制层来源 = std::array<std::vector<节点句柄>, 4>;

struct 安全因果路径快照 final {
    std::vector<节点句柄> 因果信息序列;
    std::vector<节点句柄> 直接因果边序列;
};

struct 安全权限材料缺口 final {
    std::int64_t 首个受影响组 = 0;
    节点句柄 缺口来源;
    std::uint64_t 期望版本 = 0;
    std::optional<std::uint64_t> 当前版本;
};

struct 安全输入版本快照 final {
    std::uint64_t 图版本 = 0;
    std::uint64_t 事实截止版本 = 0;
    std::uint64_t 任务集合版本 = 0;
    std::uint64_t 值集合版本 = 0;
    std::uint64_t 阈值集合版本 = 0;
    std::uint32_t 维护规则版本 = 0;
    std::uint32_t 权限规则版本 = 0;
    std::uint32_t 排序规则版本 = 0;
};

struct 安全直接因果边快照 final {
    节点句柄 边身份;
    节点句柄 原因因果信息;
    节点句柄 结果因果信息;
    节点句柄 证据动态;
    节点句柄 适用场景;
    std::uint64_t 记录版本 = 0;
    std::uint32_t 因果规则版本 = 0;
    记录当前性 当前性 = 记录当前性::失效;
};

struct 安全因果信息快照 final {
    节点句柄 因果信息身份;
    节点句柄 对应结果身份;
    安全因果信息类型 信息类型 = 安全因果信息类型::普通因果结果;
    节点句柄 适用场景;
    std::uint64_t 信息版本 = 0;
    记录当前性 当前性 = 记录当前性::失效;
};

struct 安全因果图快照 final {
    节点句柄 自我;
    节点句柄 目标安全结果;
    节点句柄 适用场景;
    std::uint64_t 图版本 = 0;
    std::uint64_t 节点集合版本 = 0;
    std::uint64_t 边集合版本 = 0;
    std::uint64_t 场景版本 = 0;
    std::int64_t 发布时间 = 0;
    std::vector<安全因果信息快照> 因果信息组;
    std::vector<安全直接因果边快照> 直接因果边组;
};

struct 安全分层读取请求 final {
    节点句柄 自我;
    节点句柄 来源因果信息;
    节点句柄 目标安全结果;
    节点句柄 适用场景;
    std::uint64_t 期望图版本 = 0;
    std::uint64_t 事实截止版本 = 0;
};

struct 安全分层读取载荷 final {
    节点句柄 自我;
    节点句柄 来源因果信息;
    节点句柄 目标安全结果;
    适用性 精确深度适用性 = 适用性::不适用;
    std::int64_t 精确深度 = 0;
    适用性 分组适用性 = 适用性::不适用;
    std::optional<std::int64_t> 优先级组;
    安全因果路径快照 规范化主路径;
    std::vector<安全因果路径快照> 全部并列最短路径;
    std::uint64_t 图版本 = 0;
    std::uint32_t 规则版本 = 0;
    std::uint64_t 事实截止版本 = 0;
};

struct 安全分层读取结果 final {
    安全结果分类 结果分类 = 安全结果分类::内部不一致;
    std::optional<安全分层读取载荷> 载荷;
};

struct 安全层定义快照 final {
    节点句柄 定义身份;
    std::int64_t 值域下界 = 0;
    std::int64_t 值域上界 = 0;
    std::int64_t 低位阈值 = 0;
    std::int64_t 高位阈值 = 0;
    std::int64_t 低位回升速率 = 0;
    std::int64_t 高位回落速率 = 0;
    std::int64_t 维护时间单位 = 0;
    节点句柄 被动维护提交方法;
    std::uint64_t 定义版本 = 0;
    std::uint32_t 规则版本 = 0;
};

struct 安全层当前快照 final {
    节点句柄 维护族;
    节点句柄 自我;
    节点句柄 来源因果信息;
    节点句柄 目标安全结果;
    节点句柄 最终安全结果;
    std::uint64_t 图版本 = 0;
    安全层定义快照 定义快照;
    节点句柄 当前状态;
    std::int64_t 当前安全值 = 0;
    std::uint64_t 值版本 = 0;
    std::uint64_t 残余约束版本 = 0;
    std::uint64_t 方向维护纪元 = 0;
    std::int64_t 累计有效时间 = 0;
    std::uint64_t 事实处理游标 = 0;
    记录当前性 来源当前性 = 记录当前性::失效;
};

struct 安全事件快照 final {
    节点句柄 事件;
    节点句柄 自我;
    节点句柄 维护族;
    节点句柄 目标安全结果;
    节点句柄 具体安全结果;
    节点句柄 场景;
    std::int64_t 发生时间 = 0;
    节点句柄 来源状态;
    节点句柄 来源动态;
    std::int64_t 后果严重度 = 0;
    因素搜索状态 搜索状态 = 因素搜索状态::未开始;
    std::uint64_t 事件版本 = 0;
    std::uint32_t 规则版本 = 0;
};

struct 安全因素快照 final {
    节点句柄 事件;
    节点句柄 因素;
    节点句柄 原因因果信息;
    节点句柄 适用场景;
    节点句柄 复发结果;
    安全因素状态 因素状态 = 安全因素状态::待验证;
    std::uint64_t 证据版本 = 0;
    std::uint64_t 记录版本 = 0;
};

struct 因素排除快照 final {
    节点句柄 事件;
    节点句柄 因素;
    因素排除结论 排除结论 = 因素排除结论::未成立;
    节点句柄 验证方法;
    节点句柄 验证动态;
    节点句柄 适用场景;
    std::uint64_t 有效起始版本 = 0;
    std::uint64_t 有效结束版本 = 0;
    std::uint64_t 排除版本 = 0;
};

struct 未复发证据段 final {
    节点句柄 事件;
    节点句柄 因素;
    节点句柄 场景;
    std::uint64_t 运行代次 = 0;
    std::int64_t 单调开始时间 = 0;
    std::int64_t 单调结束时间 = 0;
    复发机会状态 复发机会 = 复发机会状态::材料缺失;
    感知覆盖状态 感知覆盖 = 感知覆盖状态::材料缺失;
    身份比较状态 身份比较 = 身份比较状态::材料缺失;
    复发结论 结论 = 复发结论::无法裁决;
    std::uint64_t 证据版本 = 0;
};

struct 主动安全事实结算快照 final {
    节点句柄 维护族;
    节点句柄 事实批次;
    节点句柄 前状态;
    节点句柄 后状态;
    std::int64_t 前值 = 0;
    std::int64_t 后值 = 0;
    安全值变化方向 变化方向 = 安全值变化方向::无变化;
    std::vector<安全事件快照> 来源事件组;
    std::vector<未复发证据段> 来源复发组;
    std::vector<安全因素快照> 来源因素变化组;
    节点句柄 结算方法;
    std::uint64_t 结算版本 = 0;
};

struct 安全维护外部事实身份规格 final {
    节点稳定主键 后安全特征值稳定主键;
    节点稳定主键 后状态稳定主键;
    节点稳定主键 状态变化动态稳定主键;
};

struct 安全维护请求 final {
    节点句柄 幂等主键;
    节点句柄 自我;
    节点句柄 维护族;
    节点句柄 适用场景;
    std::uint64_t 期望当前值版本 = 0;
    std::uint64_t 事实截止版本 = 0;
    std::int64_t 当前单调时间 = 0;
    std::optional<安全维护外部事实身份规格> 外部事实身份规格;
    std::uint32_t 维护规则版本 = 0;
};

struct 安全维护载荷 final {
    节点句柄 维护族;
    std::int64_t 前值 = 0;
    std::int64_t 后值 = 0;
    std::int64_t 整数变化量 = 0;
    安全维护方向 维护方向 = 安全维护方向::无变化;
    std::int64_t 累计有效时间 = 0;
    std::uint64_t 消费事实截止版本 = 0;
    std::uint64_t 当前值版本 = 0;
    std::optional<std::uint64_t> 新值版本;
    std::optional<节点句柄> 来源已发布事实批次;
    std::uint64_t 维护账版本 = 0;
    std::uint64_t 发布代次 = 0;
    std::uint32_t 维护规则版本 = 0;
};

struct 安全维护结果 final {
    安全结果分类 结果分类 = 安全结果分类::内部不一致;
    std::optional<安全维护载荷> 载荷;
};

struct 安全组层快照 final {
    节点句柄 来源需求;
    节点句柄 来源因果信息;
    节点句柄 目标安全结果;
    std::int64_t 精确深度 = 0;
    std::int64_t 优先级组 = 0;
    std::int64_t 当前值 = 0;
    std::int64_t 值域下界 = 0;
    std::int64_t 值域上界 = 0;
    std::int64_t 低位阈值 = 0;
    std::int64_t 高位阈值 = 0;
    std::int64_t 释放倍率 = 0;
    std::uint64_t 图版本 = 0;
    std::uint64_t 值版本 = 0;
    std::uint64_t 阈值版本 = 0;
    std::uint32_t 规则版本 = 0;
};

struct 权限读取请求 final {
    节点句柄 自我;
    节点句柄 适用场景;
    std::uint64_t 事实截止版本 = 0;
    std::uint64_t 图版本 = 0;
    std::uint32_t 权限规则版本 = 0;
};

struct 权限读取载荷 final {
    安全五组权限值 收到权限{};
    安全五组权限值 保留权限{};
    安全四门释放倍率 释放倍率{};
    安全四门限制层来源 限制层来源{};
    安全五组权限状态值 权限状态{};
    std::vector<安全权限材料缺口> 材料缺口;
    std::vector<std::int64_t> 受影响组;
    std::int64_t 五组已形成权限总量 = 0;
    std::vector<安全组层快照> 全部安全组层快照;
    安全输入版本快照 全部来源版本;
    std::uint32_t 权限规则版本 = 0;
};

struct 权限读取结果 final {
    安全结果分类 结果分类 = 安全结果分类::内部不一致;
    std::optional<权限读取载荷> 载荷;
};

struct 任务安全关联快照 final {
    节点句柄 任务稳定句柄;
    节点句柄 来源需求;
    节点句柄 来源因果信息;
    节点句柄 目标安全结果;
    适用性 精确深度适用性 = 适用性::不适用;
    std::int64_t 精确深度 = 0;
    适用性 分组适用性 = 适用性::不适用;
    std::optional<std::int64_t> 优先级组;
    std::uint64_t 关联版本 = 0;
    记录当前性 当前性 = 记录当前性::失效;
};

struct 安全任务执行候选快照 final {
    节点句柄 任务稳定句柄;
    std::uint64_t 授权版本 = 0;
    std::uint64_t 状态版本 = 0;
    std::int64_t 基础优先级 = 0;
    std::uint64_t 创建序号 = 0;
    std::uint64_t 方法冻结版本 = 0;
};

struct 安全后果严重度材料 final {
    节点句柄 来源需求;
    节点句柄 具体安全结果;
    节点句柄 后果事实;
    std::int64_t 严重度 = 0;
    std::uint64_t 值定义版本 = 0;
    std::uint64_t 事实版本 = 0;
    记录当前性 当前性 = 记录当前性::失效;
};

struct 安全预计时间材料 final {
    节点句柄 来源需求;
    节点句柄 具体安全结果;
    适用性 时间适用性 = 适用性::不适用;
    安全发生状态 发生状态 = 安全发生状态::尚未发生;
    std::optional<std::int64_t> 预计剩余单调时间;
    std::uint64_t 证据截止版本 = 0;
    std::uint64_t 预计模型版本 = 0;
    std::uint64_t 事实版本 = 0;
    记录当前性 当前性 = 记录当前性::失效;
};

struct 安全因果证据序材料 final {
    节点句柄 来源需求;
    节点句柄 来源因果信息;
    std::int64_t 有效基础正样本数 = 0;
    std::int64_t 当前反例数 = 0;
    std::uint64_t 适用范围版本 = 0;
    std::uint32_t 因果规则版本 = 0;
    std::uint64_t 证据截止版本 = 0;
    记录当前性 当前性 = 记录当前性::失效;
};

struct 安全任务来源权限 final {
    节点句柄 来源需求;
    std::int64_t 权限 = 0;
    std::int64_t 精确深度 = 0;
    安全因果路径快照 路径;
};

struct 任务权限请求 final {
    节点句柄 自我;
    节点句柄 任务稳定句柄;
    std::uint64_t 事实截止版本 = 0;
    std::uint32_t 权限规则版本 = 0;
};

struct 任务权限载荷 final {
    节点句柄 任务稳定句柄;
    std::int64_t 最高权限 = 0;
    std::int64_t 有效执行优先级 = 0;
    节点句柄 主来源需求;
    std::vector<节点句柄> 全部并列最高来源;
    std::vector<安全任务来源权限> 全部来源权限;
    安全输入版本快照 输入版本;
    std::uint32_t 权限规则版本 = 0;
};

struct 任务权限结果 final {
    安全结果分类 结果分类 = 安全结果分类::内部不一致;
    std::optional<任务权限载荷> 载荷;
};

struct 任务排序请求 final {
    节点句柄 自我;
    节点句柄 适用场景;
    std::vector<安全任务执行候选快照> 候选任务组;
    std::uint64_t 事实截止版本 = 0;
    std::uint32_t 权限规则版本 = 0;
    std::uint32_t 排序规则版本 = 0;
};

struct 任务排序载荷 final {
    std::vector<任务权限载荷> 有序任务权限载荷组;
    std::vector<安全任务执行候选快照> 零权限任务组;
    std::vector<安全任务执行候选快照> 材料缺口任务组;
    安全输入版本快照 全部输入版本;
    std::uint32_t 排序规则版本 = 0;
};

struct 任务排序结果 final {
    安全结果分类 结果分类 = 安全结果分类::内部不一致;
    std::optional<任务排序载荷> 载荷;
};

struct 安全因果图来源请求 final {
    节点句柄 自我;
    节点句柄 目标安全结果;
    节点句柄 适用场景;
    std::uint64_t 期望图版本 = 0;
    std::uint64_t 事实截止版本 = 0;
};

struct 安全因果图来源载荷 final {
    安全因果图快照 不可变安全因果图快照;
    std::uint64_t 来源版本 = 0;
    std::uint64_t 事实截止版本 = 0;
};

struct 安全因果图来源结果 final {
    安全结果分类 结果分类 = 安全结果分类::内部不一致;
    std::optional<安全因果图来源载荷> 载荷;
};

struct 安全维护事实来源请求 final {
    节点句柄 自我;
    节点句柄 维护族;
    节点句柄 适用场景;
    std::uint64_t 期望当前值版本 = 0;
    std::uint64_t 事实截止版本 = 0;
};

struct 安全维护事实来源载荷 final {
    安全层定义快照 定义;
    安全层当前快照 当前;
    std::vector<安全事件快照> 安全事件组;
    std::vector<安全因素快照> 安全因素组;
    std::vector<因素排除快照> 因素排除组;
    std::vector<未复发证据段> 未复发证据段组;
    std::vector<主动安全事实结算快照> 主动安全事实结算快照组;
    安全输入版本快照 来源版本;
};

struct 安全维护事实来源结果 final {
    安全结果分类 结果分类 = 安全结果分类::内部不一致;
    std::optional<安全维护事实来源载荷> 载荷;
};

struct 安全任务事实来源请求 final {
    节点句柄 自我;
    节点句柄 适用场景;
    std::uint64_t 事实截止版本 = 0;
    std::uint64_t 任务集合版本 = 0;
};

struct 安全任务事实来源载荷 final {
    std::vector<任务安全关联快照> 任务安全关联组;
    std::vector<安全任务执行候选快照> 任务执行候选组;
    std::vector<安全后果严重度材料> 后果严重度材料组;
    std::vector<安全预计时间材料> 预计时间证据组;
    std::vector<安全因果证据序材料> 因果证据序材料组;
    安全输入版本快照 来源版本;
};

struct 安全任务事实来源结果 final {
    安全结果分类 结果分类 = 安全结果分类::内部不一致;
    std::optional<安全任务事实来源载荷> 载荷;
};

struct 安全维护外部事实规格 final {
    节点句柄 幂等主键;
    节点句柄 自我;
    节点句柄 维护族;
    节点句柄 适用场景;
    安全维护外部事实身份规格 外部事实身份规格;
    节点句柄 前状态;
    std::int64_t 前值 = 0;
    std::int64_t 后值 = 0;
    std::int64_t 整数变化量 = 0;
    安全维护方向 维护方向 = 安全维护方向::无变化;
    std::int64_t 发生时间 = 0;
    std::optional<节点句柄> 来源已发布事实批次;
    节点句柄 被动维护提交方法;
    安全输入版本快照 来源版本;
};

}
