#pragma once

#include <cstdint>
#include <optional>
#include <variant>
#include <vector>

#include "../核心/服务.L1事实基座.h"
#include "数据服务.不可变材料.h"

namespace 海中鱼巣 {

struct 特征值身份 final {
    稳定编码 编码{};
    friend bool operator==(const 特征值身份&, const 特征值身份&) = default;
};

inline bool 有效(特征值身份 身份) noexcept { return 有效(身份.编码); }

struct 特征值独立材料引用 final {
    稳定编码 身份{};
    friend bool operator==(const 特征值独立材料引用&,
        const 特征值独立材料引用&) = default;
};

using 特征值内容 = std::variant<std::int64_t,
    std::vector<std::int64_t>, std::vector<std::uint64_t>,
    特征值独立材料引用>;

enum class 特征值表示类型 : std::uint8_t {
    I64 = 1,
    I64组 = 2,
    U64组 = 3,
    独立材料引用 = 4
};

struct 特征值信息 final {
    特征值身份 值身份{};
    特征值内容 值内容;
    friend bool operator==(const 特征值信息&, const 特征值信息&) = default;
};

enum class 特征值读取错误 : std::uint8_t {
    入口拒绝, 未找到, 材料已清理, 能力未提供, 资源失败, 内部不一致
};

using 特征值读取结果 = std::variant<特征值信息, 特征值读取错误>;

enum class 特征值读取状态_B1 : std::uint8_t {
    已读取 = 1, 未找到 = 8, 入口拒绝 = 9,
    类型不相容 = 15, 规则未提供 = 17,
    数量预算不足 = 24, 资源失败 = 26,
    内部不一致 = 27, 旧格式不支持 = 29
};

struct 特征值完整读取请求_B1 final {
    std::uint32_t 版本{1};
    特征值身份 值{};
    世界结构预算_B1 预算{};
    friend bool operator==(const 特征值完整读取请求_B1&,
        const 特征值完整读取请求_B1&) = default;
};

struct 完整特征值_B1 final {
    特征值身份 身份{};
    特征值表示类型 表示{特征值表示类型::I64};
    特征值内容 内容;
    friend bool operator==(const 完整特征值_B1&, const 完整特征值_B1&) = default;
};

struct 特征值完整读取结果_B1 final {
    std::uint32_t 版本{1};
    特征值读取状态_B1 状态{特征值读取状态_B1::入口拒绝};
    std::optional<完整特征值_B1> 值;
    std::optional<材料事实_B1> 材料;
    世界结构用量_B1 用量{};
    bool 成功(const 特征值完整读取请求_B1&) const noexcept;
};

// B2 只保存 L1 已定义的精确 U64 序列；不得在此解释轮廓、体素或其它业务格式。
struct 特征值U64组保存请求_B2 final {
    std::uint32_t 版本{1};
    L1所有者范围写入幂等身份 幂等{};
    std::vector<std::uint64_t> 内容;
    世界结构预算_B1 预算{};
};

enum class 特征值U64组保存状态_B2 : std::uint8_t {
    已复用 = 1, 已保存 = 2, 入口拒绝 = 8, 结构未就绪 = 9,
    许可拒绝 = 10, 未找到 = 11, 数量预算不足 = 14,
    幂等冲突 = 15, 引用冲突 = 16, 资源失败 = 17,
    内部不一致 = 18, 已可能发布 = 19
};

struct 特征值U64组保存结果_B2 final {
    std::uint32_t 版本{1};
    特征值U64组保存状态_B2 状态{特征值U64组保存状态_B2::入口拒绝};
    std::optional<特征值身份> 值;
    世界结构用量_B1 用量{};
    bool 成功(const 特征值U64组保存请求_B2&) const noexcept;
};

struct 特征值U64组结构登记请求_B2 final {
    std::uint32_t 版本{1};
    L1所有者范围写入幂等身份 幂等{};
};

struct 特征值U64组结构交付_B2 final {
    L1结构所有者身份 所有者{};
    稳定编码 承载节点{};
    稳定编码 U64组属性类型节点{};
    稳定编码 来源节点{};
};

enum class 特征值U64组结构登记状态_B2 : std::uint8_t {
    已登记 = 1, 入口拒绝 = 8, 许可拒绝 = 9, 未找到 = 10,
    幂等冲突 = 13, 引用冲突 = 14, 资源失败 = 15,
    内部不一致 = 16, 已可能发布 = 17
};

struct 特征值U64组结构登记结果_B2 final {
    std::uint32_t 版本{1};
    特征值U64组结构登记状态_B2 状态{
        特征值U64组结构登记状态_B2::入口拒绝};
    std::optional<特征值U64组结构交付_B2> 交付;
    bool 成功(const 特征值U64组结构登记请求_B2&) const noexcept;
};

class 特征值类数据服务 final {
public:
    explicit 特征值类数据服务(const L1事实基座服务& 第一层服务) noexcept;
    特征值类数据服务(const L1事实基座服务& 第一层服务,
        const 不可变材料数据服务& 材料服务) noexcept;
    特征值类数据服务(const L1事实基座服务& 第一层服务,
        L1所有者范围写端口&& 写端口,
        const 特征值U64组结构交付_B2& 结构) noexcept;

    特征值类数据服务() = delete;
    特征值类数据服务(const 特征值类数据服务&) = delete;
    特征值类数据服务& operator=(const 特征值类数据服务&) = delete;
    特征值类数据服务(特征值类数据服务&&) = delete;
    特征值类数据服务& operator=(特征值类数据服务&&) = delete;

    bool 绑定于(const L1事实基座服务& 第一层服务) const noexcept;
    static 特征值表示类型 获取值表示类型(
        const 特征值信息& 特征值) noexcept;
    static 特征值身份 获取值身份(const 特征值信息& 特征值) noexcept;
    static const 特征值内容& 获取值内容(
        const 特征值信息& 特征值) noexcept;
    static bool 内容结构有效(const 特征值内容& 内容) noexcept;

    // 诊断责任：向上送出。这里只读取仍然存在的当前值事实。
    特征值读取结果 获取特征值(特征值身份 身份) const;
    特征值完整读取结果_B1 获取完整值(
        const 特征值完整读取请求_B1& 请求) const noexcept;
    特征值完整读取结果_B1 读取完整U64组_B2(
        const 特征值完整读取请求_B1& 请求) const noexcept;
    特征值U64组保存结果_B2 保存U64组(
        const 特征值U64组保存请求_B2& 请求) noexcept;
    static 特征值U64组结构登记结果_B2 登记U64组结构_B2(
        const L1事实基座服务&, L1所有者范围写端口&,
        const 特征值U64组结构登记请求_B2&) noexcept;

private:
    static 特征值读取错误 映射读取错误(
        L1所有者范围读取状态 状态) noexcept;
    static 特征值读取结果 投影读取事实(特征值身份 身份,
        const std::optional<L1所有者范围事实副本>& 事实);
    static std::optional<特征值内容> 转换为特征值内容(
        const L1所有者范围原始值材料& 材料);

    const L1事实基座服务& 第一层服务_;
    const 不可变材料数据服务* 材料服务_;
    std::optional<L1所有者范围写端口> 写端口_;
    std::optional<特征值U64组结构交付_B2> U64组结构_;
};

} // namespace 海中鱼巣
