#pragma once

#include "../领域/算法.有序I64特征比较.h"

#include <array>
#include <cstdint>
#include <optional>
#include <utility>
#include <variant>
#include <vector>

namespace 海中鱼巣 {

enum class 二次计算状态 : std::uint8_t {
    已计算=1, 入口拒绝=2, 来源缺失=3,
    事实冲突=5, 未注册=6, 算法未实现=7, 类型不匹配=8,
    单位量化不匹配=9, 参照不相容=10, 运算溢出=11,
    // 12、13 对应已经退出的旧技术状态，稳定数值不复用。
    固定规则不兼容=14, 结果范围不满足=15,
    资源失败=16, 内部不一致=17, 未实现=18
};

struct 二次准确F来源 final {
    特征信息身份 F;
    friend bool operator==(const 二次准确F来源&, const 二次准确F来源&) = default;
};

struct 二次已保存定义输出来源 final {
    特征类定义身份 定义;
    特征类标量结果角色 输出角色 = 特征类标量结果角色::排序;
    friend bool operator==(const 二次已保存定义输出来源&,
                           const 二次已保存定义输出来源&) = default;
};

struct 二次本次输出来源 final {
    std::uint64_t 局部编号 = 0;
    特征类标量结果角色 输出角色 = 特征类标量结果角色::排序;
    friend bool operator==(const 二次本次输出来源&, const 二次本次输出来源&) = default;
};

struct 二次已保存定义节点身份 final {
    特征类定义身份 定义;
    friend bool operator==(const 二次已保存定义节点身份&,
                           const 二次已保存定义节点身份&) = default;
};

struct 二次本次节点身份 final {
    std::uint64_t 局部编号 = 0;
    friend bool operator==(const 二次本次节点身份&, const 二次本次节点身份&) = default;
};

using 二次计算节点身份 = std::variant<二次已保存定义节点身份, 二次本次节点身份>;
using 二次计算来源 = std::variant<二次准确F来源,
    二次已保存定义输出来源, 二次本次输出来源>;

struct 二次计算输入 final {
    std::uint32_t 顺序 = 0;
    特征I64输入角色 角色 = 特征I64输入角色::识别左;
    二次计算来源 来源;
    friend bool operator==(const 二次计算输入&, const 二次计算输入&) = default;
};

struct 二次计算项 final {
    std::uint64_t 局部编号 = 0;
    特征类型身份 输入FT;
    特征I64比较用途 用途 = 特征I64比较用途::识别区分;
    std::optional<特征I64比较绑定身份> 预期K;
    std::array<二次计算输入, 2> 输入;
    std::uint8_t 要求结果位 = 0;
    friend bool operator==(const 二次计算项&, const 二次计算项&) = default;
};

struct 二次根输出 final {
    std::variant<二次已保存定义输出来源, 二次本次输出来源> 来源;
    friend bool operator==(const 二次根输出&, const 二次根输出&) = default;
};

struct 二次计算上下文 final {
    std::optional<稳定编码> 参与者A, 参与者B;
    std::optional<std::int64_t> 左时间, 右时间;
    std::optional<稳定编码> 参照;
    friend bool operator==(const 二次计算上下文&, const 二次计算上下文&) = default;
};

struct 二次计算请求 final {
    std::uint64_t 请求身份 = 0;
    std::vector<二次计算项> 图项组;
    std::vector<二次根输出> 根输出组;
    二次计算上下文 上下文;
    friend bool operator==(const 二次计算请求&, const 二次计算请求&) = default;
};

struct 二次基础叶回执 final {
    准确特征读取事实 事实;
    friend bool operator==(const 二次基础叶回执&, const 二次基础叶回执&) = default;
};

struct 二次计算输入回执 final {
    std::uint32_t 顺序 = 0;
    特征I64输入角色 角色 = 特征I64输入角色::识别左;
    二次计算来源 来源;
    特征类型身份 实际FT;
    std::int64_t 值 = 0;
    std::uint32_t 实际阶次 = 0;
    friend bool operator==(const 二次计算输入回执&, const 二次计算输入回执&) = default;
};

struct 二次计算输出回执 final {
    二次计算节点身份 节点;
    特征I64比较绑定身份 K;
    特征类型身份 输出FT;
    特征类标量结果角色 输出角色 = 特征类标量结果角色::排序;
    std::int64_t 值 = 0;
    特征类标量量化合同 量化;
    std::uint32_t 真实阶次 = 0;
    friend bool operator==(const 二次计算输出回执&, const 二次计算输出回执&) = default;
};

struct 二次计算项回执 final {
    二次计算节点身份 节点;
    有序I64比较合同快照 K;
    std::array<二次计算输入回执, 2> 输入;
    std::vector<二次计算输出回执> 输出组;
    std::uint32_t 真实阶次 = 0;
    friend bool operator==(const 二次计算项回执&, const 二次计算项回执&) = default;
};

struct 二次准确计算结果 final {
    二次计算状态 状态 = 二次计算状态::入口拒绝;
    std::uint64_t 请求身份 = 0;
    二次计算上下文 上下文;
    std::vector<二次根输出> 根输出组;
    std::vector<二次基础叶回执> 基础叶组;
    std::vector<二次计算项回执> 计算项组;
    std::vector<二次计算输出回执> 结果组;
    bool 成功() const noexcept;
    friend bool operator==(const 二次准确计算结果&, const 二次准确计算结果&) = default;
};

namespace 二次计算内部 {
using 节点键 = std::pair<bool, std::uint64_t>;
节点键 键(const 二次计算节点身份&);
二次计算节点身份 身份(节点键);
二次计算来源 来源(const 二次根输出&);
节点键 输出键(const 二次计算来源&);
特征类标量结果角色 输出角色(const 二次计算来源&);
unsigned 角色位(特征类标量结果角色) noexcept;
unsigned 上下文位(const 二次计算上下文&) noexcept;
bool 上下文有效(const 二次计算上下文&) noexcept;
bool 来源有效(const 二次计算来源&) noexcept;
std::optional<std::int64_t> 整数(const 准确特征读取事实&) noexcept;
bool 准确来源完整(const 准确特征读取事实&) noexcept;
bool 快照完整(const 有序I64比较合同快照&);
bool 同量化(const 特征类标量量化合同&, const 特征类标量量化合同&) noexcept;
有序I64比较合同快照 快照(const 特征I64比较绑定事实&);
bool 固定相容(const 有序I64比较合同快照&, const 有序I64比较合同快照&) noexcept;
}

class 二次特征计算应用服务 final {
public:
    二次特征计算应用服务(const 特征类数据服务&,
                           const 有序I64特征比较提供者&) noexcept;
    二次特征计算应用服务() = delete;
    二次特征计算应用服务(const 二次特征计算应用服务&) = delete;
    二次特征计算应用服务& operator=(const 二次特征计算应用服务&) = delete;
    二次特征计算应用服务(二次特征计算应用服务&&) = delete;
    二次特征计算应用服务& operator=(二次特征计算应用服务&&) = delete;

    bool 与特征服务同底座(const 特征类数据服务&) const noexcept;
    二次准确计算结果 计算(const 二次计算请求&) const noexcept;

private:
    const 特征类数据服务& feature_;
    const 有序I64特征比较提供者& provider_;
};

} // namespace 海中鱼巣
