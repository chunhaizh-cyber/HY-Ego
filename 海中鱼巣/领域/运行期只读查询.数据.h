#pragma once

// 运行期只读查询 BIZ 层 DTO：BIZ-L3-002-02-01/02/03 子快照
// 对接现有 L2 需求/任务/状态/特征结构正式公开读取接口；
// 待 DATA-EXT-12/15/16 最终 ABI 发布后机械替换为专用 DATA 强类型。
//
// 【模块使用约定】本头文件只能在 L2 服务模块均已 import 的上下文中被
// export { #include } 展开；所需 L2存在身份 / L2需求事实 等类型均来自
// 前述模块 IFC，不直接 #include L2数据头（避免 enum / 类型跨 IFC 重定义）。
#ifndef 运行期只读查询_NO_INCLUDES
#include <cstdint>
#include <optional>
#include <vector>
#endif

namespace 海中鱼巣 {

inline constexpr std::uint32_t 运行期只读查询合同版本 = 1;

// ===== BIZ-L3-002-02-01 自我世界当前一致子快照（占位：由 WORLD-SELF 组合器承接）=====
enum class 自我世界快照状态 : std::uint8_t {
    已形成 = 1,
    请求拒绝 = 2,
    材料缺失 = 3,
    当前性漂移 = 4,
    资源失败 = 5,
    内部不一致 = 6
};

struct 自我世界快照请求 final {
    std::uint32_t 合同版本 = 运行期只读查询合同版本;
    std::uint64_t 运行代次 = 0;
    std::uint64_t 事实范围版本 = 0;
    L2存在身份 唯一自我;
    std::uint64_t 共享事实截止G0 = 0;
    std::uint64_t 完整秒边界 = 0;
};

struct 自我世界一致子快照 final {
    L2场景树身份 现实世界树;
    L2场景身份 根场景;
    L2存在事实 自我存在;
    L2场景树节点事实 自我所在场景;
    L2场景树节点事实 自我内部世界场景;
    std::uint64_t 事实截止代次 = 0;
};

struct 自我世界快照结果 final {
    自我世界快照状态 状态 = 自我世界快照状态::材料缺失;
    std::optional<自我世界一致子快照> 快照;
    bool 成功() const noexcept {
        return 状态 == 自我世界快照状态::已形成 && 快照.has_value();
    }
};

// ===== BIZ-L3-002-02-02 本能根当前快照 =====
enum class 生存安全控制态 : std::uint8_t {
    终止 = 0,
    休眠 = 1,
    主动运行 = 2
};

enum class 本能根当前快照状态 : std::uint8_t {
    已形成 = 1,
    请求拒绝 = 2,
    尚无权威 = 3,
    材料缺失 = 4,
    当前性漂移 = 5,
    资源失败 = 6,
    内部不一致 = 7
};

struct 本能根当前快照请求 final {
    std::uint32_t 合同版本 = 运行期只读查询合同版本;
    std::uint64_t 运行代次 = 0;
    std::uint64_t 事实范围版本 = 0;
    L2存在身份 唯一自我;
    std::uint64_t 共享事实截止G0 = 0;
    L2特征定义身份 安全根特征定义;
    L2特征定义身份 服务根特征定义;
};

struct 生存安全根当前值 final {
    std::int64_t A = 0;
    std::int64_t L = 0;
    std::int64_t H = 0;
    生存安全控制态 控制态 = 生存安全控制态::终止;
};

struct 本能根当前快照 final {
    生存安全根当前值 安全根;
    std::int64_t 服务根当前值 = 0;
    std::uint64_t 事实截止代次 = 0;
};

struct 本能根当前快照结果 final {
    本能根当前快照状态 状态 = 本能根当前快照状态::材料缺失;
    std::optional<本能根当前快照> 快照;
    bool 成功() const noexcept {
        return 状态 == 本能根当前快照状态::已形成 && 快照.has_value();
    }
};

// ===== BIZ-L3-002-02-03 需求结构与任务后继快照 =====
enum class 需求任务快照状态 : std::uint8_t {
    已形成 = 1,
    请求拒绝 = 2,
    材料缺失 = 3,
    当前性漂移 = 4,
    数量预算不足 = 5,
    资源失败 = 6,
    内部不一致 = 7
};

struct 需求任务快照请求 final {
    std::uint32_t 合同版本 = 运行期只读查询合同版本;
    std::uint64_t 运行代次 = 0;
    std::uint64_t 事实范围版本 = 0;
    L2存在身份 唯一自我;
    std::uint64_t 共享事实截止G0 = 0;
    std::uint64_t 最大根需求数 = 64;
    std::uint64_t 最大需求节点数 = 256;
    std::uint64_t 最大需求结构深度 = 16;
};

struct 需求当前结构项 final {
    L2需求事实 需求;
    std::optional<L2需求父子关系事实> 当前父关系;
    L2需求列表成员关系事实 当前列表成员关系;
    std::uint64_t 事实截止代次 = 0;
};

struct 需求列表任务后继项 final {
    L2需求列表项事实 列表项;
    std::optional<L2任务事实> 当前任务;
    std::uint64_t 事实截止代次 = 0;
};

struct 需求结构与任务后继快照 final {
    std::vector<L2需求事实> 根需求组;
    std::vector<需求当前结构项> 当前需求结构组;
    std::vector<需求列表任务后继项> 列表任务后继组;
    std::uint64_t 事实截止代次 = 0;
};

struct 需求任务快照结果 final {
    需求任务快照状态 状态 = 需求任务快照状态::材料缺失;
    std::optional<需求结构与任务后继快照> 快照;
    bool 成功() const noexcept {
        return 状态 == 需求任务快照状态::已形成 && 快照.has_value();
    }
};

// ===== 任务实际结果同 G0 独立读回 =====
enum class 任务实际结果独立读回状态 : std::uint8_t {
    已形成 = 1, 尚未形成 = 2, 请求拒绝 = 3, 轮次不匹配 = 4,
    当前性漂移 = 5, 数量预算不足 = 6, 引用冲突 = 7,
    资源失败 = 8, 内部不一致 = 9
};

struct 任务实际结果独立读回请求 final {
    std::uint32_t 合同版本 = 运行期只读查询合同版本;
    L2任务身份 任务;
    std::uint64_t 执行轮次 = 0;
    L2方法身份 方法;
    L2方法动作入口身份 动作入口;
    L2场景身份 场景;
    L2存在身份 主体;
    L2状态身份 实际状态;
    L2动态身份 动态证据;
    std::uint64_t 来源材料版本 = 0;
    std::uint64_t 运行代次 = 0;
    std::uint64_t 共享事实截止G0 = 0;
    std::uint64_t 最大需求成员数 = 0;
};

struct 任务实际结果独立读回快照 final {
    L2任务实际结果事实 结果;
    L2实例方法事实 实例方法;
    L2任务方法路径事实 选中路径;
    L2任务事实 任务;
    L2状态事实 状态;
    L2动态事实 动态;
    L2场景事实 场景;
    L2存在事实 主体;
    L2需求列表项事实 需求列表项;
    std::vector<L2需求列表成员关系事实> 来源成员关系组;
    std::vector<L2需求事实> 来源需求组;
    std::uint64_t 事实截止代次 = 0;
};

struct 任务实际结果独立读回结果 final {
    任务实际结果独立读回状态 状态 = 任务实际结果独立读回状态::尚未形成;
    std::optional<任务实际结果独立读回快照> 快照;
    bool 成功() const noexcept {
        return 状态 == 任务实际结果独立读回状态::已形成 && 快照.has_value();
    }
};

// ===== 任务实际结果后继的任务目标达成裁决 =====
struct 任务目标达成裁决请求 final {
    std::uint32_t 合同版本 = 运行期只读查询合同版本;
    std::uint64_t 治理输入幂等键 = 0;
    L2任务身份 任务;
    L2实例方法身份 实例方法;
    std::uint64_t 执行轮次 = 0;
    L2任务实际结果身份 任务实际结果;
    std::uint64_t 共享事实截止G0 = 0;
};

enum class 任务目标达成裁决状态 : std::uint8_t {
    目标已达成 = 1,
    目标未达成 = 2,
    合法等待 = 3,
    当前性漂移 = 4,
    引用冲突 = 5,
    不可比较 = 6,
    入口拒绝 = 7,
    资源失败 = 8,
    内部错误 = 9
};

enum class 任务生命周期后继准备 : std::uint8_t {
    无 = 0,
    待验证后完成 = 1,
    目标未达成待重筹办 = 2
};

struct 任务目标达成裁决快照 final {
    std::uint64_t 治理输入幂等键 = 0;
    L2任务身份 任务;
    L2实例方法身份 实例方法;
    std::uint64_t 执行轮次 = 0;
    L2任务实际结果身份 任务实际结果;
    L2需求列表项身份 任务目标来源;
    L2存在身份 目标宿主;
    L2特征定义身份 目标特征;
    L2目标状态合同身份 目标状态合同;
    L2状态身份 实际状态;
    L2动态身份 动态证据;
    L2方法身份 方法;
    稳定编码 路径预期结果;
    稳定编码 路径验证合同;
    L2特征比较注册身份 比较注册;
    std::uint32_t 比较算法版本 = 0;
    L2特征比较具名关系 具名关系 =
        L2特征比较具名关系::当前达到目标;
    std::uint8_t 允许关系位 = 0;
    任务生命周期后继准备 生命周期准备 = 任务生命周期后继准备::无;
    std::uint64_t 事实截止代次 = 0;
    friend bool operator==(const 任务目标达成裁决快照&,
        const 任务目标达成裁决快照&) = default;
};

struct 任务目标达成裁决结果 final {
    任务目标达成裁决状态 状态 = 任务目标达成裁决状态::入口拒绝;
    std::optional<任务目标达成裁决快照> 裁决;
    bool 成功() const noexcept {
        return (状态 == 任务目标达成裁决状态::目标已达成
                || 状态 == 任务目标达成裁决状态::目标未达成)
            && 裁决.has_value();
    }
};

// ===== 需求当前满足实时裁决 =====
inline constexpr std::uint32_t 需求当前满足裁决规则版本 = 1;

struct 需求当前满足裁决请求 final {
    std::uint32_t 合同版本 = 运行期只读查询合同版本;
    std::uint64_t 裁决请求身份 = 0;
    L2需求身份 需求;
    std::uint64_t 共享事实截止G0 = 0;
    friend bool operator==(const 需求当前满足裁决请求&,
        const 需求当前满足裁决请求&) = default;
};

enum class 需求当前满足裁决状态 : std::uint8_t {
    已满足 = 1,
    未满足 = 2,
    证据不足 = 3,
    当前性漂移 = 4,
    引用冲突 = 5,
    不可比较 = 6,
    入口拒绝 = 7,
    许可拒绝 = 8,
    资源失败 = 9,
    内部错误 = 10
};

struct 需求当前满足裁决快照 final {
    std::uint32_t 规则版本 = 需求当前满足裁决规则版本;
    std::uint64_t 裁决请求身份 = 0;
    L2需求事实 需求事实;
    L2需求列表成员关系事实 当前成员关系;
    L2需求列表项事实 所属列表项;
    L2特征实例事实 当前特征实例;
    L2特征值事实 当前特征值;
    L2目标状态合同事实 目标合同;
    L2特征比较注册身份 比较注册;
    std::uint32_t 比较算法版本 = 0;
    L2特征比较具名关系 具名关系 =
        L2特征比较具名关系::当前达到目标;
    std::uint8_t 允许关系位 = 0;
    std::uint64_t 事实截止代次 = 0;
    friend bool operator==(const 需求当前满足裁决快照&,
        const 需求当前满足裁决快照&) = default;
};

struct 需求当前满足裁决结果 final {
    需求当前满足裁决状态 状态 = 需求当前满足裁决状态::入口拒绝;
    std::optional<需求当前满足裁决快照> 裁决;
    bool 成功() const noexcept {
        return (状态 == 需求当前满足裁决状态::已满足
                || 状态 == 需求当前满足裁决状态::未满足)
            && 裁决.has_value();
    }
    friend bool operator==(const 需求当前满足裁决结果&,
        const 需求当前满足裁决结果&) = default;
};

// ===== 自我治理一致事实最终快照 =====
struct 自我治理一致事实快照 final {
    自我世界一致子快照 世界自我;
    本能根当前快照 本能根;
    需求结构与任务后继快照 需求任务;
    std::uint64_t 事实截止代次 = 0;
};

} // namespace 海中鱼巣
