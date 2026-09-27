#pragma once

#include <cstdint>
#include <optional>
#include <span>
#include <vector>

#include "合同.相关概念添加参与.h"
#include "数据服务.定位特征.h"
#include "数据服务.绑定存在.h"

namespace 海中鱼巣 {

class 特征类数据服务;
class 存在类数据服务;
class 特征值类数据服务;
class 场景类数据服务;
class 特征值域比较数据服务;

enum class 概念树数据状态 : std::uint8_t {
    已创建 = 1,
    精确重复 = 2,
    已读取 = 3,
    已添加支持 = 4,
    已删除支持 = 5,
    已添加上位 = 6,
    已替换上位 = 7,
    已删除上位 = 8,
    已迁移生命周期 = 9,
    已删除概念 = 10,
    已释放引用 = 11,
    无须变更 = 12,
    入口拒绝 = 13,
    未找到 = 14,
    引用冲突 = 16,
    形成环 = 17,
    根不可退出 = 18,
    幂等冲突 = 20,
    资源失败 = 22,
    内部不一致 = 23,
    已可能发布 = 24,
    已创建共享定义 = 26,
    已绑定共享名称 = 27,
    已删除共享名称 = 28,
    已记录共享用途 = 29,
    不支持 = 30,
    差异不可表示 = 31,
    已登记类型观察 = 32,
    已登记特征概念命中 = 33,
    已变更特征概念组织 = 34,
    旧格式不支持 = 35,
    规则缺失 = 36,
    类型不相容 = 37,
    前次写入待收敛 = 38,
    未实现 = 39
};

struct 特征概念出生使用结构交付 final {
    稳定编码 锚点{};
    std::uint32_t 格式 = 1;
    稳定编码 F到FCv出生使用关系类型{};
    friend bool operator==(const 特征概念出生使用结构交付&,
                           const 特征概念出生使用结构交付&) = default;
};

struct 特征概念出生使用结构登记请求 final {
    L1所有者范围写入幂等身份 幂等键{};
    纯概念结构交付 纯概念结构{};
    friend bool operator==(const 特征概念出生使用结构登记请求&,
                           const 特征概念出生使用结构登记请求&) = default;
};

struct 特征概念出生使用结构登记结果 final {
    纯概念状态 状态 = 纯概念状态::入口拒绝;
    纯概念发布状态 发布 = 纯概念发布状态::未进入;
    std::optional<特征概念出生使用结构登记请求> 原请求;
    std::optional<特征概念出生使用结构交付> 交付;
    bool 成功(const 特征概念出生使用结构登记请求&) const noexcept;
};

enum class 特征概念值域基础读取状态 : std::uint8_t {
    已读取 = 1,
    未找到 = 2,
    类别冲突 = 4,
    规则缺失 = 5,
    未实现 = 6,
    资源失败 = 8,
    内部不一致 = 9,
    入口拒绝 = 10
};

struct 特征概念值域基础读取请求 final {
    概念树概念身份 FC;
    friend bool operator==(const 特征概念值域基础读取请求&,
                           const 特征概念值域基础读取请求&) = default;
};

struct 特征概念值域基础事实 final {
    概念树概念身份 FC;
    稳定编码 FT{};
    std::optional<std::vector<概念树I64区间>> I64域;
    std::optional<纯概念事实> 完整纯概念事实;
};

struct 特征概念值域基础读取结果 final {
    特征概念值域基础读取状态 状态 =
        特征概念值域基础读取状态::入口拒绝;
    std::optional<特征概念值域基础事实> 事实;
    bool 成功(const 特征概念值域基础读取请求&) const noexcept;
};

struct I64特征概念组织读取请求 final { 稳定编码 FT{}; };
struct I64特征概念组织读取结果 final {
    纯概念状态 状态 = 纯概念状态::入口拒绝;
    std::vector<纯概念事实> 概念组;
    bool 成功(const I64特征概念组织读取请求&) const noexcept;
};

class 概念树类数据服务 final : public 相关概念添加参与者,
                              public 已发布概念引用参与者,
                              public 原子I64特征概念参与者 {
public:
    static 纯概念结构登记结果 登记纯概念结构(
        const L1事实基座服务&, L1所有者范围写端口&,
        const 纯概念结构登记请求&) noexcept;
    static 特征概念出生使用结构登记结果 登记特征概念出生使用结构(
        const L1事实基座服务&, L1所有者范围写端口&,
        const 特征概念出生使用结构登记请求&) noexcept;
    static 存在概念两组结构登记结果 登记存在概念两组结构(
        const L1事实基座服务&, L1所有者范围写端口&,
        const 存在概念两组结构登记请求&) noexcept;

    概念树类数据服务(const L1事实基座服务&,
                     const 特征类数据服务&,
                     const 存在类数据服务&,
                     const 特征值类数据服务&,
                     const 场景类数据服务&,
                     L1所有者范围写端口&&,
                     const 相关概念结构交付&);
    概念树类数据服务(const L1事实基座服务&,
                     const 特征类数据服务&,
                     const 存在类数据服务&,
                     const 特征值类数据服务&,
                     const 场景类数据服务&,
                     L1所有者范围写端口&&,
                     const 纯概念结构交付&,
                     const 特征概念出生使用结构交付&,
                     const 存在概念两组结构交付&);

    bool 绑定于(const L1事实基座服务&) const noexcept override;
    bool 使用特征服务(const 特征类数据服务&) const noexcept;
    bool 使用存在服务(const 存在类数据服务&) const noexcept;
    bool 使用特征值服务(const 特征值类数据服务&) const noexcept;
    bool 使用场景服务(const 场景类数据服务&) const noexcept;

    相关概念参与片段 准备相关概念片段(
        const 相关概念参与请求&,
        L1有限N分区原子参与者身份) const noexcept override;
    相关概念组合提交结果 提交相关概念组合事务(
        const 相关概念组合提交请求&,
        std::span<L1所有者范围写端口* const>) noexcept override;
    相关概念参与读回 读取相关概念结果(
        const 相关概念参与请求&) const noexcept override;

    存在概念两组规范化结果 规范化存在概念两组定义(
        const 存在概念两组规范化请求&,
        const 特征值域比较数据服务&) const noexcept;
    存在概念两组枚举结果 枚举存在概念候选(
        const 存在概念两组枚举请求&,
        const 特征值域比较数据服务&) const noexcept;
    存在概念两组查询结果 精确查询存在概念(
        const 存在概念两组查询请求&,
        const 特征值域比较数据服务&) const noexcept;
    存在概念两组写入结果 创建或复用存在概念(
        const 存在概念两组创建请求&,
        const 特征值域比较数据服务&) noexcept;
    存在概念两组读取结果 读取存在概念两组定义(
        const 存在概念两组读取请求&,
        const 特征值域比较数据服务&) const noexcept;

    纯概念查询结果 精确查询纯概念(const 纯概念查询请求&) const noexcept;
    纯概念写入结果 创建或复用纯概念(const 纯概念创建请求&) noexcept;
    纯概念读取结果 读取纯概念(const 纯概念读取请求&) const noexcept;
    I64特征概念组织读取结果 读取当前I64特征概念(
        const I64特征概念组织读取请求&) const noexcept;
    特征概念值域基础读取结果 读取特征概念值域基础(
        const 特征概念值域基础读取请求&) const noexcept;
    存在概念使用读取结果 读取存在概念使用(
        const 存在概念使用读取请求&) const noexcept;
    存在概念使用完整读取结果 读取存在概念使用完整(
        const 存在概念使用读取请求&) const noexcept;

    概念树类数据服务() = delete;
    概念树类数据服务(const 概念树类数据服务&) = delete;
    概念树类数据服务& operator=(const 概念树类数据服务&) = delete;
    概念树类数据服务(概念树类数据服务&&) = delete;
    概念树类数据服务& operator=(概念树类数据服务&&) = delete;

private:
    const L1事实基座服务& 原子I64底座() const noexcept override;
    L1所有者范围写端口& 原子I64端口() noexcept override;
    bool 原子I64结构已就绪() const noexcept override;
    原子I64特征参与结果<L1有限N分区原子参与者写集>
    准备原子I64出生片段(const 原子I64特征出生请求&) const override;
    原子I64特征出生使用读取结果 读取原子I64出生使用(
        const 原子I64特征出生使用读取请求&) const override;
    原子I64特征参与结果<L1有限N分区原子参与者写集>
    准备原子I64出生使用退出片段(
        const 原子I64特征出生使用退出片段请求&) const override;

    const L1事实基座服务& l1_;
    const 特征类数据服务& features_;
    const 存在类数据服务& existences_;
    const 特征值类数据服务& values_;
    const 场景类数据服务& scenes_;
    L1所有者范围写端口 port_;
    std::optional<相关概念结构交付> related_layout_;
    std::optional<纯概念结构交付> pure_layout_;
    std::optional<特征概念出生使用结构交付> feature_birth_layout_;
    std::optional<存在概念两组结构交付> two_group_layout_;
};

} // namespace 海中鱼巣
