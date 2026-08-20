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

// ===== BIZ-L3-002-02-02 生存安全与服务维护当前快照 =====
enum class 生存安全控制态 : std::uint8_t {
    终止 = 0,
    休眠 = 1,
    主动运行 = 2
};

enum class 生存服务快照状态 : std::uint8_t {
    已形成 = 1,
    请求拒绝 = 2,
    尚无权威 = 3,
    材料缺失 = 4,
    当前性漂移 = 5,
    数量预算不足 = 6,
    资源失败 = 7,
    内部不一致 = 8
};

struct 生存服务快照请求 final {
    std::uint32_t 合同版本 = 运行期只读查询合同版本;
    std::uint64_t 运行代次 = 0;
    std::uint64_t 事实范围版本 = 0;
    L2存在身份 唯一自我;
    std::uint64_t 共享事实截止G0 = 0;
    std::uint64_t 完整秒边界 = 0;
    L2特征定义身份 安全根特征定义;
    L2特征定义身份 服务值特征定义;
    std::uint64_t 最大合同数 = 64;
    std::uint64_t 最大准备数 = 64;
    std::uint64_t 最大进展数 = 64;
    std::uint64_t 最大到期事件数 = 64;
};

struct 生存安全根当前值 final {
    std::int64_t A = 0;
    std::int64_t L = 0;
    std::int64_t H = 0;
    生存安全控制态 控制态 = 生存安全控制态::终止;
};

struct 服务值与维护游标 final {
    std::int64_t 服务值 = 0;
    std::uint64_t 时间纪元 = 0;
    std::uint64_t 上一已结算完整秒 = 0;
};

struct 生存服务共享截止快照 final {
    生存安全根当前值 安全根;
    服务值与维护游标 服务游标;
    std::vector<L2状态事实> 当前合同组;
    std::vector<L2状态事实> 当前准备组;
    std::vector<L2状态事实> 当前进展组;
    std::vector<L2状态事实> 当前到期未满足组;
    std::uint64_t 事实截止代次 = 0;
};

struct 生存服务快照结果 final {
    生存服务快照状态 状态 = 生存服务快照状态::材料缺失;
    std::optional<生存服务共享截止快照> 快照;
    bool 成功() const noexcept {
        return 状态 == 生存服务快照状态::已形成 && 快照.has_value();
    }
};

// ===== BIZ-L3-002-02-03 需求活动与任务后继快照 =====
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
    std::uint32_t 需求范围版本 = 0;
    L2存在身份 唯一自我;
    std::uint64_t 共享事实截止G0 = 0;
    std::uint64_t 任务树合同版本 = 0;
    std::uint64_t 最大根需求数 = 64;
    std::uint64_t 最大活动路径深度 = 8;
};

struct 需求活动路径项 final {
    L2需求事实 需求;
    std::optional<L2需求父子关系事实> 父关系;
    std::optional<L2需求列表成员关系事实> 列表成员关系;
    std::uint64_t 事实截止代次 = 0;
};

struct 根需求活动路径 final {
    L2需求事实 根需求;
    std::vector<需求活动路径项> 活动路径;
};

struct 活动需求任务树项 final {
    L2需求身份 活动需求;
    std::optional<L2任务事实> 当前任务;
    std::uint64_t 事实截止代次 = 0;
};

struct 需求活动与任务后继快照 final {
    std::vector<根需求活动路径> 根需求活动路径组;
    std::vector<活动需求任务树项> 活动需求任务组;
    std::uint64_t 事实截止代次 = 0;
};

struct 需求任务快照结果 final {
    需求任务快照状态 状态 = 需求任务快照状态::材料缺失;
    std::optional<需求活动与任务后继快照> 快照;
    bool 成功() const noexcept {
        return 状态 == 需求任务快照状态::已形成 && 快照.has_value();
    }
};

// ===== 自我治理一致事实最终快照 =====
struct 自我治理一致事实快照 final {
    自我世界一致子快照 世界自我;
    生存服务共享截止快照 生存服务;
    需求活动与任务后继快照 需求任务;
    std::uint64_t 事实截止代次 = 0;
};

} // namespace 海中鱼巣
