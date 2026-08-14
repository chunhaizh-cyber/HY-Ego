#pragma once

#ifndef IMMUTABLE_MATERIAL_NO_INCLUDES
#include <array>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <variant>
#include "L2结构公共.数据.h"
#endif

namespace 海中鱼巣 {

inline constexpr std::uint32_t 不可变材料规则版本 = 1;

struct 不可变材料身份 final {
    稳定编码 值{};
    friend bool operator==(const 不可变材料身份&,
        const 不可变材料身份&) = default;
};

enum class 不可变材料格式角色 : std::uint8_t {
    严格UTF8 = 1,
    通用字节 = 2
};

struct 不可变材料格式身份 final {
    稳定编码 值{};
    friend bool operator==(const 不可变材料格式身份&,
        const 不可变材料格式身份&) = default;
};

struct 材料SHA256摘要 final {
    std::array<std::uint8_t, 32> 字节{};
    friend bool operator==(const 材料SHA256摘要&,
        const 材料SHA256摘要&) = default;
};

struct 不可变材料受控存储根 final {
    std::filesystem::path 绝对路径;
    friend bool operator==(const 不可变材料受控存储根&,
        const 不可变材料受控存储根&) = default;
};

struct 不可变材料存储配置 final {
    std::uint32_t 合同版本 = L2结构合同版本;
    不可变材料受控存储根 受控根;
    friend bool operator==(const 不可变材料存储配置&,
        const 不可变材料存储配置&) = default;
};

struct 不可变材料物理定位 final {
    std::filesystem::path 摘要对象绝对路径;
    friend bool operator==(const 不可变材料物理定位&,
        const 不可变材料物理定位&) = default;
};

struct 不可变材料系统签发来源 final {
    friend bool operator==(const 不可变材料系统签发来源&,
        const 不可变材料系统签发来源&) = default;
};

struct 不可变材料已发布节点来源 final {
    稳定编码 节点{};
    friend bool operator==(const 不可变材料已发布节点来源&,
        const 不可变材料已发布节点来源&) = default;
};

using 不可变材料来源 = std::variant<不可变材料系统签发来源,
    不可变材料已发布节点来源>;

struct 不可变材料登记事实 final {
    稳定编码 系统签发者节点{};
    不可变材料格式身份 严格UTF8格式身份;
    不可变材料格式身份 通用字节格式身份;
    稳定编码 格式属性类型{};
    稳定编码 长度属性类型{};
    稳定编码 摘要属性类型{};
    std::uint64_t 事实截止代次 = 0;
    friend bool operator==(const 不可变材料登记事实&,
        const 不可变材料登记事实&) = default;
};

struct 不可变材料事实 final {
    不可变材料身份 身份;
    不可变材料格式角色 格式角色 =
        static_cast<不可变材料格式角色>(0);
    不可变材料格式身份 格式身份;
    std::uint64_t 字节长度 = 0;
    材料SHA256摘要 摘要;
    稳定编码 来源节点{};
    L2生命周期 生命周期;
    std::optional<不可变材料物理定位> 物理定位;
    friend bool operator==(const 不可变材料事实&,
        const 不可变材料事实&) = default;
};

struct 不可变材料写入请求 final {
    L2结构请求头 请求头;
    std::uint32_t 规则版本 = 不可变材料规则版本;
    L2结构幂等身份 幂等身份;
    不可变材料格式角色 格式角色 =
        static_cast<不可变材料格式角色>(0);
    std::filesystem::path 隔离候选路径;
    std::uint64_t 字节长度 = 0;
    材料SHA256摘要 摘要;
    不可变材料来源 来源;
    friend bool operator==(const 不可变材料写入请求&,
        const 不可变材料写入请求&) = default;
};

struct 不可变材料写入结果 final {
    L2结构结果头 结果头;
    std::optional<不可变材料事实> 材料;

    bool 成功() const noexcept {
        if ((结果头.状态 != L2结构状态::已提交
                && 结果头.状态 != L2结构状态::精确重复)
            || 结果头.合同版本 != L2结构合同版本
            || 结果头.事实截止代次 == 0 || !结果头.变更事实代次
            || !材料 || !有效(材料->身份.值) || !有效(材料->格式身份.值)
            || 材料->字节长度 == 0 || !有效(材料->来源节点)
            || !L2生命周期完整(材料->生命周期)
            || 材料->生命周期.退出事实代次 || !材料->物理定位
            || 材料->物理定位->摘要对象绝对路径.empty()) return false;
        return *结果头.变更事实代次 == 结果头.事实截止代次
            && 材料->生命周期.创建事实代次 == 结果头.事实截止代次;
    }

    friend bool operator==(const 不可变材料写入结果&,
        const 不可变材料写入结果&) = default;
};

struct 不可变材料读取请求 final {
    L2结构请求头 请求头;
    L2读取类别 读取类别 = L2读取类别::当前;
    不可变材料身份 身份;
    std::uint64_t 历史截止事实代次 = 0;
    friend bool operator==(const 不可变材料读取请求&,
        const 不可变材料读取请求&) = default;
};

struct 不可变材料读取结果 final {
    L2结构结果头 结果头;
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止事实代次 = 0;
    std::optional<不可变材料事实> 材料;
    friend bool operator==(const 不可变材料读取结果&,
        const 不可变材料读取结果&) = default;
};

struct 不可变材料退出请求 final {
    L2结构请求头 请求头;
    std::uint32_t 规则版本 = 不可变材料规则版本;
    L2结构幂等身份 幂等身份;
    不可变材料身份 身份;
    friend bool operator==(const 不可变材料退出请求&,
        const 不可变材料退出请求&) = default;
};

struct 不可变材料退出结果 final {
    L2结构结果头 结果头;
    std::optional<不可变材料事实> 已退出材料;

    bool 成功() const noexcept {
        if ((结果头.状态 != L2结构状态::已提交
                && 结果头.状态 != L2结构状态::精确重复)
            || 结果头.合同版本 != L2结构合同版本
            || 结果头.事实截止代次 == 0 || !结果头.变更事实代次
            || !已退出材料 || !有效(已退出材料->身份.值)
            || !有效(已退出材料->格式身份.值)
            || 已退出材料->字节长度 == 0 || !有效(已退出材料->来源节点)
            || !L2生命周期完整(已退出材料->生命周期)
            || !已退出材料->生命周期.退出事实代次
            || !已退出材料->物理定位) return false;
        return *结果头.变更事实代次 == 结果头.事实截止代次
            && *已退出材料->生命周期.退出事实代次
                == 结果头.事实截止代次;
    }

    friend bool operator==(const 不可变材料退出结果&,
        const 不可变材料退出结果&) = default;
};

struct 不可变材料登记读取请求 final {
    L2结构请求头 请求头;
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止事实代次 = 0;
    friend bool operator==(const 不可变材料登记读取请求&,
        const 不可变材料登记读取请求&) = default;
};

struct 不可变材料登记读取结果 final {
    L2结构结果头 结果头;
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止事实代次 = 0;
    std::optional<不可变材料登记事实> 登记;

    bool 成功() const noexcept {
        if (结果头.状态 != L2结构状态::已读取
            || 结果头.合同版本 != L2结构合同版本
            || 结果头.事实截止代次 == 0 || 结果头.变更事实代次
            || !登记 || 登记->事实截止代次 != 结果头.事实截止代次
            || !有效(登记->系统签发者节点)
            || !有效(登记->严格UTF8格式身份.值)
            || !有效(登记->通用字节格式身份.值)
            || !有效(登记->格式属性类型) || !有效(登记->长度属性类型)
            || !有效(登记->摘要属性类型)) return false;
        const std::uint64_t 值组[] = {登记->系统签发者节点.值,
            登记->严格UTF8格式身份.值.值, 登记->通用字节格式身份.值.值,
            登记->格式属性类型.值, 登记->长度属性类型.值,
            登记->摘要属性类型.值};
        for (std::size_t 左 = 0; 左 < 6; ++左)
            for (std::size_t 右 = 左 + 1; 右 < 6; ++右)
                if (值组[左] == 值组[右]) return false;
        return (读取类别 == L2读取类别::当前
                && 历史截止事实代次 == 0)
            || (读取类别 == L2读取类别::历史
                && 历史截止事实代次 != 0
                && 历史截止事实代次 == 结果头.事实截止代次);
    }

    friend bool operator==(const 不可变材料登记读取结果&,
        const 不可变材料登记读取结果&) = default;
};

} // namespace 海中鱼巣
