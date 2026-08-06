#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>

namespace 海中鱼巣 {

inline constexpr std::uint32_t L1动态合同版本 = 1;
inline constexpr std::uint32_t L1动态规则版本 = 1;
inline constexpr std::uint32_t 状态迁移动能规格版本 = 1;
inline constexpr std::uint32_t 状态迁移动能比较证据版本 = 1;
inline constexpr std::uint64_t L1动态登记幂等域 = 0x0FULL;
inline constexpr std::uint32_t L1动态登记操作标签 = 0x15000F01U;
inline constexpr std::uint8_t L1动态登记意图组 = 7;
inline constexpr std::uint8_t L1动态登记入口变体 = 3;

enum class L1动态状态 : std::uint8_t {
    已登记 = 1, 已读取 = 2, 已形成 = 3, 无需形成 = 4,
    禁止形成 = 5, 登记未加载 = 6, 入口拒绝 = 7, 未找到 = 8,
    幂等冲突 = 9, 版本漂移 = 10, 许可拒绝 = 11,
    资源失败 = 12, 内部不一致 = 13
};

enum class 动态种类 : std::uint8_t { 状态迁移 = 1 };

enum class 状态迁移动能规格原因 : std::uint8_t {
    无 = 0, 无前状态 = 1, 材料等价 = 2, 状态规格禁止动态 = 3,
    上游非成功 = 4, 迁移证据不完整 = 5, 角色或时间不合法 = 6
};

struct 动态操作幂等身份 final {
    std::uint64_t 值 = 0;
    friend bool operator==(动态操作幂等身份, 动态操作幂等身份) = default;
};

struct L1动态结构登记 final {
    std::uint32_t 合同版本 = L1动态合同版本;
    std::uint32_t 规则版本 = L1动态规则版本;
    动态操作幂等身份 首次幂等身份;
    std::uint64_t 事实截止代次 = 0;
    稳定编码 服务身份;
    稳定编码 关系20动态组成关系类型;
    稳定编码 动态资格属性类型;
    稳定编码 动态种类属性类型;
    稳定编码 聚合规则版本属性类型;
    稳定编码 时间范围属性类型;
    稳定编码 迁移比较合同属性类型;
    稳定编码 迁移差异I64属性类型;
    稳定编码 比较输入引用属性类型;
    稳定编码 比较输入I64组属性类型;
    friend bool operator==(const L1动态结构登记&, const L1动态结构登记&) = default;
};

struct L1动态登记请求 final {
    std::uint32_t 合同版本 = L1动态合同版本;
    std::uint32_t 规则版本 = L1动态规则版本;
    动态操作幂等身份 幂等身份;
    std::uint64_t 期望事实代次 = 0;
    friend bool operator==(const L1动态登记请求&, const L1动态登记请求&) = default;
};

struct L1动态登记结果 final {
    L1动态状态 状态 = L1动态状态::入口拒绝;
    std::optional<L1动态结构登记> 登记;
};

struct 状态迁移动能请求意图摘要输入 final {
    std::uint64_t 业务请求身份 = 0;
    std::uint64_t 整理幂等身份 = 0;
    std::uint64_t 前状态发生时间 = 0;
    std::uint64_t 后状态发生时间 = 0;
    稳定编码 主体;
    稳定编码 场景;
    稳定编码 实例槽;
    稳定编码 前状态;
    写集本地键 后状态本地键{0x53000001};
    friend bool operator==(const 状态迁移动能请求意图摘要输入&,
        const 状态迁移动能请求意图摘要输入&) = default;
};

struct 状态迁移动能写入规格 final {
    std::uint32_t 规格版本 = 状态迁移动能规格版本;
    std::uint64_t 事实截止代次 = 0;
    状态迁移动能请求意图摘要输入 请求意图;
    L1写集请求 写集;
    std::uint64_t 确定性摘要 = 0;
    friend bool operator==(const 状态迁移动能写入规格&,
        const 状态迁移动能写入规格&) = default;
};

struct 状态迁移动能规格结果 final {
    L1动态状态 状态 = L1动态状态::入口拒绝;
    状态迁移动能规格原因 原因 = 状态迁移动能规格原因::无;
    std::optional<状态迁移动能写入规格> 规格;
};

struct 状态迁移动能读取请求 final {
    std::uint32_t 合同版本 = L1动态合同版本;
    std::uint32_t 规则版本 = L1动态规则版本;
    稳定编码 动态;
    稳定编码 动态资格值;
    稳定编码 动态种类值;
    稳定编码 聚合规则版本值;
    稳定编码 时间范围值;
    稳定编码 迁移比较合同值;
    稳定编码 迁移差异值;
    稳定编码 比较输入引用值;
    稳定编码 比较输入I64组值;
    稳定编码 主体关系;
    稳定编码 场景关系;
    稳定编码 被改变目标关系;
    稳定编码 前状态关系;
    稳定编码 后状态关系;
    稳定编码 来源存在关系;
};

struct 动态节点见证 final {
    稳定编码 编码;
    std::uint64_t 创建事实代次 = 0;
    std::optional<std::uint64_t> 退出事实代次;
    std::uint64_t 当前状态发布代次 = 0;
};

struct 动态关系见证 final {
    稳定编码 编码;
    稳定编码 源节点;
    稳定编码 目标节点;
    稳定编码 关系类型;
    std::int64_t 角色 = 0;
    std::uint64_t 创建事实代次 = 0;
    std::optional<std::uint64_t> 退出事实代次;
    std::uint64_t 当前状态发布代次 = 0;
};

struct 动态I64事实见证 final {
    稳定编码 编码;
    稳定编码 所属节点;
    稳定编码 属性类型;
    std::int64_t 材料 = 0;
    稳定编码 来源节点;
    std::optional<稳定编码> 前一值;
    std::uint64_t 创建事实代次 = 0;
    std::optional<std::uint64_t> 退出事实代次;
    std::uint64_t 当前状态发布代次 = 0;
};

struct 动态U64组事实见证 final {
    稳定编码 编码;
    稳定编码 所属节点;
    稳定编码 属性类型;
    std::vector<std::uint64_t> 材料;
    稳定编码 来源节点;
    std::optional<稳定编码> 前一值;
    std::uint64_t 创建事实代次 = 0;
    std::optional<std::uint64_t> 退出事实代次;
    std::uint64_t 当前状态发布代次 = 0;
};

struct 动态I64组事实见证 final {
    稳定编码 编码;
    稳定编码 所属节点;
    稳定编码 属性类型;
    std::vector<std::int64_t> 材料;
    稳定编码 来源节点;
    std::optional<稳定编码> 前一值;
    std::uint64_t 创建事实代次 = 0;
    std::optional<std::uint64_t> 退出事实代次;
    std::uint64_t 当前状态发布代次 = 0;
};

struct 状态迁移持久比较证据 final {
    std::uint32_t 证据版本 = 状态迁移动能比较证据版本;
    稳定编码 特征定义;
    稳定编码 注册身份;
    std::uint32_t 算法版本 = 0;
    std::uint8_t 比较用途数值 = 0;
    std::uint8_t 结果原始类型数值 = 0;
    std::uint8_t 具名关系数值 = 0;
    std::uint8_t 三态编码 = 0;
    bool 等价 = false;
    稳定编码 单位;
    稳定编码 维度;
    稳定编码 分量角色;
    std::uint8_t 差异方向数值 = 0;
    bool 可表示 = false;
    std::uint8_t 边界包含数值 = 0;
    std::uint8_t 量化规则数值 = 0;
    std::uint32_t 误差合同版本 = 0;
    std::uint64_t 比较事实截止代次 = 0;
    稳定编码 差异属性类型;
    std::uint32_t 适用范围版本 = 0;
};

struct 状态迁移输入见证 final {
    稳定编码 前状态;
    稳定编码 旧值;
    稳定编码 旧值属性类型;
    std::uint64_t 旧值创建代次 = 0;
    std::optional<std::uint64_t> 旧值退出代次;
    稳定编码 旧来源;
    std::int64_t 旧I64 = 0;
    稳定编码 新值;
    稳定编码 新值属性类型;
    std::uint64_t 新值创建代次 = 0;
    稳定编码 新来源;
    std::int64_t 新I64 = 0;
    稳定编码 实例槽;
    稳定编码 特征定义;
    std::uint64_t 事实截止代次 = 0;
};

struct 状态迁移动能事实 final {
    std::uint32_t 合同版本 = L1动态合同版本;
    std::uint32_t 规则版本 = L1动态规则版本;
    std::uint64_t 读取事实截止代次 = 0;
    动态节点见证 动态;
    动态关系见证 主体关系;
    动态关系见证 场景关系;
    动态关系见证 被改变目标关系;
    动态关系见证 前状态关系;
    动态关系见证 后状态关系;
    动态关系见证 来源存在关系;
    动态I64事实见证 动态资格;
    动态I64事实见证 动态种类值;
    动态I64事实见证 聚合规则版本值;
    动态U64组事实见证 时间范围值;
    动态U64组事实见证 迁移比较合同值;
    动态I64事实见证 迁移差异值;
    动态U64组事实见证 比较输入引用值;
    动态I64组事实见证 比较输入I64组值;
    动态种类 种类 = 动态种类::状态迁移;
    std::uint64_t 前状态发生时间 = 0;
    std::uint64_t 后状态发生时间 = 0;
    状态迁移持久比较证据 比较证据;
    状态迁移输入见证 输入见证;
};

struct 状态迁移动能结果 final {
    L1动态状态 状态 = L1动态状态::入口拒绝;
    std::optional<状态迁移动能事实> 事实;
};

// P14-诊断责任：无适用错误分支；最终责任边界：调用谓词的服务。
inline bool 动态操作幂等身份有效(动态操作幂等身份 身份) noexcept {
    return 身份.值 != 0 && 身份.值 <= 0x00FF'FFFF'FFFF'FFFFULL;
}

// P14-诊断责任：无适用错误分支；最终责任边界：调用谓词的服务。
inline bool L1动态结构登记完整(const L1动态结构登记& 登记) noexcept {
    if (登记.合同版本 != L1动态合同版本 || 登记.规则版本 != L1动态规则版本
        || !动态操作幂等身份有效(登记.首次幂等身份) || 登记.事实截止代次 == 0)
        return false;
    const 稳定编码 编码组[] = {登记.服务身份, 登记.关系20动态组成关系类型,
        登记.动态资格属性类型, 登记.动态种类属性类型, 登记.聚合规则版本属性类型,
        登记.时间范围属性类型, 登记.迁移比较合同属性类型,
        登记.迁移差异I64属性类型, 登记.比较输入引用属性类型,
        登记.比较输入I64组属性类型};
    for (std::size_t i = 0; i < 10; ++i) {
        if (!有效(编码组[i])) return false;
        for (std::size_t j = i + 1; j < 10; ++j)
            if (编码组[i] == 编码组[j]) return false;
    }
    return true;
}

// P14-诊断责任：无适用错误分支；最终责任边界：调用谓词的服务。
inline bool 状态迁移动能读取请求有效(const 状态迁移动能读取请求& 请求) noexcept {
    if (请求.合同版本 != L1动态合同版本 || 请求.规则版本 != L1动态规则版本)
        return false;
    const 稳定编码 编码组[] = {请求.动态, 请求.动态资格值, 请求.动态种类值,
        请求.聚合规则版本值, 请求.时间范围值, 请求.迁移比较合同值,
        请求.迁移差异值, 请求.比较输入引用值, 请求.比较输入I64组值,
        请求.主体关系, 请求.场景关系, 请求.被改变目标关系, 请求.前状态关系,
        请求.后状态关系, 请求.来源存在关系};
    for (std::size_t i = 0; i < 15; ++i) {
        if (!有效(编码组[i])) return false;
        for (std::size_t j = i + 1; j < 15; ++j)
            if (编码组[i] == 编码组[j]) return false;
    }
    return true;
}

} // namespace 海中鱼巣
