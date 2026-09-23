#pragma once

#include "数据服务.概念树类.h"

namespace 海中鱼巣 {

enum class 特征值域比较状态_v1 : std::uint8_t {
    已读取 = 1, 已核验, 未找到, 目标已退出, 类别冲突, 类型不相容,
    规则缺失, 未实现, 事实代次漂移, 数量预算不足, 历史材料不可用,
    资源失败, 内部不一致, 入口拒绝
};
enum class 特征值域关系_v1 : std::uint8_t { 相等 = 1, 左包含右, 右包含左, 不包含 };
struct 特征值域比较预算_v1 final {
    世界结构预算_B1 值与材料预算;
    概念树预算 概念预算;
    有界事实读取预算_B1 概念读取预算;
    std::uint64_t 最大域项数 = 0;
};
struct 特征值域读取请求_v1 final {
    std::uint32_t 版本 = 1; std::uint64_t Gread = 0, H = 0;
    概念树概念身份 FC; 特征值域比较预算_v1 预算;
};
struct 特征值域关系核验请求_v1 final {
    std::uint32_t 版本 = 1; std::uint64_t Gread = 0, H = 0;
    概念树概念身份 左FC, 右FC; 特征值域比较预算_v1 预算;
};
struct 实例值域命中核验请求_v1 final {
    std::uint32_t 版本 = 1; std::uint64_t Gread = 0, H = 0;
    特征信息身份 F; 概念树概念身份 FC; 特征值域比较预算_v1 预算;
};
struct 特征值域事实_v1 final {
    概念树概念身份 FC; 特征类型身份 FT; 特征值表示类型 表示 = 特征值表示类型::I64;
    特征比较规则身份 规则; 特征规范I64域 I64域;
    特征概念值域基础事实_v1 基础读回;
    世界结构用量_B1 值与材料用量;
};
struct 特征值域读取结果_v1 final {
    std::uint32_t 版本 = 1; 特征值域比较状态_v1 状态 = 特征值域比较状态_v1::入口拒绝;
    std::uint64_t Gread = 0, H = 0; std::optional<特征值域事实_v1> 域;
    有界事实读取用量_B1 概念读取用量;
    bool 成功(const 特征值域读取请求_v1&) const noexcept;
};
struct 特征值域关系结果_v1 final {
    std::uint32_t 版本 = 1; 特征值域比较状态_v1 状态 = 特征值域比较状态_v1::入口拒绝;
    std::uint64_t Gread = 0, H = 0; std::optional<特征值域关系_v1> 关系;
    std::optional<特征值域事实_v1> 左域, 右域;
    有界事实读取用量_B1 概念读取用量;
    bool 成功(const 特征值域关系核验请求_v1&) const noexcept;
};
struct 实例值域命中结果_v1 final {
    std::uint32_t 版本 = 1; 特征值域比较状态_v1 状态 = 特征值域比较状态_v1::入口拒绝;
    std::uint64_t Gread = 0, H = 0; std::optional<特征值域关系_v1> 关系;
    std::optional<特征值域事实_v1> 域;
    有界事实读取用量_B1 概念读取用量;
    bool 成功(const 实例值域命中核验请求_v1&) const noexcept;
};

class 特征值域比较数据服务 final {
public:
    特征值域比较数据服务(const 概念树类数据服务&, const 特征类数据服务&, const 特征值类数据服务&) noexcept;
    特征值域比较数据服务() = delete;
    特征值域比较数据服务(const 特征值域比较数据服务&) = delete;
    特征值域比较数据服务& operator=(const 特征值域比较数据服务&) = delete;
    bool 绑定于(const L1事实基座服务&) const noexcept;
    特征值域读取结果_v1 读取特征值域(const 特征值域读取请求_v1&) const;
    特征值域关系结果_v1 核验特征值域关系(const 特征值域关系核验请求_v1&) const;
    实例值域命中结果_v1 核验实例值域命中(const 实例值域命中核验请求_v1&) const;
private:
    friend class 概念树类数据服务;
    特征值域读取结果_v1 读取特征值域共享(
        const 特征值域读取请求_v1&, 概念事实读取会话_v1&) const;
    特征值域读取结果_v1 读取特征值域共享(
        const 特征值域读取请求_v1&, 概念事实读取会话_v1&,
        特征值域事实读取会话_v1&) const;
    // 待实现：I64组值域专用适配器；只能返回未实现或规则缺失，零写入。
    特征值域读取结果_v1 读取I64组值域待实现(const 特征值域读取请求_v1&, const 特征概念值域基础事实_v1&) const;
    // 待实现：U64组值域专用适配器；只能返回未实现或规则缺失，零写入。
    特征值域读取结果_v1 读取U64组值域待实现(const 特征值域读取请求_v1&, const 特征概念值域基础事实_v1&) const;
    // 待实现：独立材料值域专用适配器；只能返回未实现或规则缺失，零写入。
    特征值域读取结果_v1 读取独立材料值域待实现(const 特征值域读取请求_v1&, const 特征概念值域基础事实_v1&) const;
    const 概念树类数据服务& concepts_; const 特征类数据服务& features_; const 特征值类数据服务& values_;
};
} // namespace 海中鱼巣
