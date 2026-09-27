#pragma once

#include <cstdint>
#include <optional>
#include <variant>
#include <vector>

#include "数据服务.概念树类.h"

namespace 海中鱼巣 {

enum class 特征值域比较状态 : std::uint8_t {
    已读取 = 1,
    已核验 = 2,
    未找到 = 3,
    类别冲突 = 5,
    类型不相容 = 6,
    规则缺失 = 7,
    未实现 = 8,
    // 9、10 对应已退出的旧状态，稳定数值不复用。
    资源失败 = 11,
    内部不一致 = 12,
    入口拒绝 = 13
};

enum class 特征值域关系 : std::uint8_t {
    相等 = 1,
    左包含右,
    右包含左,
    不包含
};

struct 特征值域读取请求 final {
    概念树概念身份 FC;
};

struct 特征值域关系核验请求 final {
    概念树概念身份 左FC, 右FC;
};

struct 实例值域命中核验请求 final {
    特征信息身份 F;
    概念树概念身份 FC;
};

struct 特征I64组有限域 final {
    std::vector<std::vector<std::int64_t>> 点组;
    friend bool operator==(const 特征I64组有限域&,
                           const 特征I64组有限域&) = default;
};

struct 特征U64组有限域 final {
    std::vector<std::vector<std::uint64_t>> 点组;
    friend bool operator==(const 特征U64组有限域&,
                           const 特征U64组有限域&) = default;
};

struct 特征独立材料域项 final {
    不可变材料格式身份_B1 格式;
    std::vector<std::uint8_t> 完整载荷;
    friend bool operator==(const 特征独立材料域项&,
                           const 特征独立材料域项&) = default;
};

struct 特征独立材料有限域 final {
    std::vector<特征独立材料域项> 材料组;
    friend bool operator==(const 特征独立材料有限域&,
                           const 特征独立材料有限域&) = default;
};

using 特征规范值域 = std::variant<特征规范I64域, 特征I64组有限域,
                                  特征U64组有限域, 特征独立材料有限域>;

struct 特征值域事实 final {
    概念树概念身份 FC;
    特征类型身份 FT;
    特征值表示类型 原始表示 = 特征值表示类型::I64;
    特征比较规则身份 规则身份;
    // 这是特征比较规则自身的业务修订号。
    std::uint32_t 规则版本 = 0;
    特征规范值域 规范化值域;
    特征概念值域基础事实 基础读回;
};

struct 特征值域读取结果 final {
    特征值域比较状态 状态 = 特征值域比较状态::入口拒绝;
    std::optional<特征值域事实> 域;
    bool 成功(const 特征值域读取请求&) const noexcept;
};

struct 特征值域关系结果 final {
    特征值域比较状态 状态 = 特征值域比较状态::入口拒绝;
    std::optional<特征值域关系> 关系;
    std::optional<特征值域事实> 左域, 右域;
    bool 成功(const 特征值域关系核验请求&) const noexcept;
};

struct 实例值域命中结果 final {
    特征值域比较状态 状态 = 特征值域比较状态::入口拒绝;
    std::optional<特征值域关系> 关系;
    std::optional<特征值域事实> 域;
    bool 成功(const 实例值域命中核验请求&) const noexcept;
};

class 特征值域比较数据服务 final {
public:
    特征值域比较数据服务(const 概念树类数据服务&,
                           const 特征类数据服务&,
                           const 特征值类数据服务&) noexcept;
    特征值域比较数据服务() = delete;
    特征值域比较数据服务(const 特征值域比较数据服务&) = delete;
    特征值域比较数据服务& operator=(const 特征值域比较数据服务&) = delete;

    bool 绑定于(const L1事实基座服务&) const noexcept;
    特征值域读取结果 读取特征值域(const 特征值域读取请求&) const noexcept;
    特征值域关系结果 核验特征值域关系(
        const 特征值域关系核验请求&) const noexcept;
    实例值域命中结果 核验实例值域命中(
        const 实例值域命中核验请求&) const noexcept;

private:
    const 概念树类数据服务& concepts_;
    const 特征类数据服务& features_;
    const 特征值类数据服务& values_;
};

} // namespace 海中鱼巣
