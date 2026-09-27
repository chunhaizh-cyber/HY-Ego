#pragma once

#include <cstdint>
#include <optional>
#include <variant>
#include <vector>

#include "../核心/服务.L1事实基座.h"
#include "数据服务.特征类.h"

namespace 海中鱼巣 {

inline constexpr std::uint32_t 状态类数据合同版本 = 2;

struct 状态信息身份 final {
    稳定编码 编码{};
    friend bool operator==(const 状态信息身份&, const 状态信息身份&) = default;
};

inline bool 有效(状态信息身份 身份) noexcept { return 有效(身份.编码); }

enum class 状态强时间语义 : std::uint8_t {
    实例绝对UTC纳秒 = 1,
    抽象相对纳秒 = 2
};

struct 状态强时间 final {
    状态强时间语义 语义 = 状态强时间语义::实例绝对UTC纳秒;
    std::int64_t 纳秒 = 0;
    friend bool operator==(const 状态强时间&, const 状态强时间&) = default;
};

using 状态固定准确值 = 特征准确值;

// 状态本体固定为身份、正式特征类型、固定准确值和强时间四个字段。
struct 状态信息 final {
    状态信息身份 身份;
    特征类型身份 正式特征类型;
    状态固定准确值 固定准确值;
    状态强时间 强时间;
    friend bool operator==(const 状态信息&, const 状态信息&) = default;
};

enum class 状态类数据状态 : std::uint8_t {
    已创建 = 1, 精确重复 = 2, 已读取 = 3, 已删除 = 4,
    入口拒绝 = 6, 未找到 = 7,
    正式特征类型未找到 = 9, 准确值不相容 = 11,
    旧格式不支持 = 12,
    幂等冲突 = 14, 引用冲突 = 15, 数量预算不足 = 16,
    资源失败 = 18, 内部不一致 = 19
};

struct 状态类结构交付 final {
    稳定编码 状态族锚点;
    稳定编码 状态族归属关系类型;
    稳定编码 正式特征类型关系类型;
    稳定编码 内联准确值属性类型;
    稳定编码 引用准确值属性类型;
    稳定编码 实例绝对时间属性类型;
    稳定编码 抽象相对时间属性类型;
    稳定编码 首次形成UTC属性类型;
};

struct 状态类结果头 final {
    状态类数据状态 状态 = 状态类数据状态::入口拒绝;
    std::uint32_t 合同版本 = 状态类数据合同版本;
};

struct 状态内容事实 final {
    状态信息 信息;
    稳定编码 族归属关系;
    稳定编码 正式特征类型关系;
    稳定编码 准确值事实;
    稳定编码 强时间事实;
    稳定编码 首次形成UTC事实;
    std::int64_t 首次形成UTC纳秒 = 0;
};

bool 状态内容事实完整(const 状态内容事实&) noexcept;

struct 状态创建请求 final {
    std::uint32_t 合同版本 = 状态类数据合同版本;
    L1所有者范围写入幂等身份 幂等身份{};
    特征类型身份 正式特征类型;
    状态固定准确值 固定准确值;
    状态强时间 强时间;
};

struct 状态当前读取请求 final {
    std::uint32_t 合同版本 = 状态类数据合同版本;
    状态信息身份 身份;
};

struct 状态按正式特征类型查询请求 final {
    std::uint32_t 合同版本 = 状态类数据合同版本;
    特征类型身份 正式特征类型;
    std::uint64_t 最大候选数量 = 0;
};

struct 状态按固定准确值查询请求 final {
    std::uint32_t 合同版本 = 状态类数据合同版本;
    状态固定准确值 固定准确值;
    std::uint64_t 最大候选数量 = 0;
};

struct 状态按强时间范围查询请求 final {
    std::uint32_t 合同版本 = 状态类数据合同版本;
    状态强时间语义 语义 = 状态强时间语义::实例绝对UTC纳秒;
    std::int64_t 起始纳秒 = 0;
    std::int64_t 终止纳秒 = 0;
    std::uint64_t 最大候选数量 = 0;
};

struct 状态退出请求 final {
    std::uint32_t 合同版本 = 状态类数据合同版本;
    L1所有者范围写入幂等身份 幂等身份{};
    状态信息身份 身份;
};

struct 状态当前身份确认请求 final {
    std::uint32_t 合同版本 = 状态类数据合同版本;
    状态信息身份 身份;
};

struct 状态创建结果 final {
    状态类结果头 结果头;
    std::optional<状态内容事实> 内容;
};

struct 状态读取结果 final {
    状态类结果头 结果头;
    std::optional<状态内容事实> 内容;
};

struct 状态组查询结果 final {
    状态类结果头 结果头;
    std::vector<状态内容事实> 状态组;
};

struct 状态退出结果 final {
    状态类结果头 结果头;
};

class 状态类数据服务 final {
public:
    状态类数据服务() = delete;
    状态类数据服务(const 状态类数据服务&) = delete;
    状态类数据服务& operator=(const 状态类数据服务&) = delete;
    状态类数据服务(状态类数据服务&&) = delete;
    状态类数据服务& operator=(状态类数据服务&&) = delete;
    状态类数据服务(L1事实基座服务& 第一层服务,
        const 特征类数据服务& 特征服务,
        L1所有者范围写端口&& 写端口,
        const 状态类结构交付& 结构);

    bool 绑定于(const L1事实基座服务&) const noexcept;
    状态创建结果 创建状态(const 状态创建请求&);
    状态读取结果 读取当前状态(const 状态当前读取请求&) const;
    状态组查询结果 按正式特征类型查询当前状态组(
        const 状态按正式特征类型查询请求&) const;
    状态组查询结果 按固定准确值查询当前状态组(
        const 状态按固定准确值查询请求&) const;
    状态组查询结果 按强时间范围查询当前状态组(
        const 状态按强时间范围查询请求&) const;
    状态退出结果 退出状态(const 状态退出请求&);
    状态类结果头 确认当前状态结构身份(
        const 状态当前身份确认请求&) const;

private:
    inline static constexpr L1所有者范围写集本地键 节点键{1};
    inline static constexpr L1所有者范围写集本地键 族关系键{2};
    inline static constexpr L1所有者范围写集本地键 FT关系键{3};
    inline static constexpr L1所有者范围写集本地键 准确值键{4};
    inline static constexpr L1所有者范围写集本地键 时间键{5};
    inline static constexpr L1所有者范围写集本地键 首次UTC键{6};

    bool 布局浅层有效() const noexcept;
    bool 布局材料有效() const;
    static bool 时间语义有效(状态强时间语义) noexcept;
    static bool 创建请求有效(const 状态创建请求&) noexcept;
    static std::int64_t 当前UTC纳秒() noexcept;

    L1所有者范围写集请求 形成创建写集(
        const 状态创建请求&, std::int64_t 首次UTC) const;
    static std::optional<std::int64_t> 查找首次UTC(
        const L1所有者范围写集请求&);
    std::optional<状态创建结果> 重放创建(const 状态创建请求&);

    状态读取结果 读取当前内容(状态信息身份) const;
    状态组查询结果 读取全部当前状态() const;
    std::optional<状态内容事实> 解析闭包状态(
        const L1所有者范围一致关系类型闭包成员&) const;

    static bool 当前值事实完整(const L1所有者范围值事实&,
        L1结构所有者身份, 稳定编码 所属节点,
        稳定编码 属性类型) noexcept;
    static bool 写入头完整(const L1所有者范围写入结果&,
        L1结构所有者身份, L1所有者范围写入幂等身份) noexcept;
    static std::optional<稳定编码> 映射编码(
        const L1所有者范围写入结果&, L1所有者范围写集本地键);
    static bool 创建映射完整(const L1所有者范围写入结果&) noexcept;

    L1所有者范围写集请求 形成退出写集(
        const 状态内容事实&, L1所有者范围写入幂等身份) const;
    static void 添加退出身份(std::vector<稳定编码>&,
        const 状态内容事实&);
    static void 规范化退出组(std::vector<稳定编码>&);
    bool 首次读取回显完整(const L1所有者范围首次写入读取结果&,
        L1所有者范围写入幂等身份) const noexcept;
    static bool 退出写集形状有效(const L1所有者范围写集请求&,
        L1所有者范围写入幂等身份) noexcept;
    std::optional<状态退出结果> 重放退出(const 状态退出请求&);

    static 状态类结果头 头(状态类数据状态) noexcept;
    static 状态创建结果 创建失败(状态类数据状态) noexcept;
    static 状态读取结果 读取失败(状态类数据状态) noexcept;
    static 状态组查询结果 组失败(状态类数据状态) noexcept;
    static 状态退出结果 退出失败(状态类数据状态) noexcept;
    static 状态类数据状态 映射读取(L1所有者范围读取状态) noexcept;
    static 状态类数据状态 映射一致读取(
        L1所有者范围一致当前读取状态) noexcept;
    static 状态类数据状态 映射写入(
        L1所有者范围写入状态, 状态类数据状态 成功状态) noexcept;

    L1事实基座服务& l1_;
    const 特征类数据服务& feature_;
    L1所有者范围写端口 port_;
    L1结构所有者身份 owner_{};
    状态类结构交付 layout_;
};

} // namespace 海中鱼巣
