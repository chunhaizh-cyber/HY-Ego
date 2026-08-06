#pragma once

#include <cstdint>
#include <optional>

namespace 海中鱼巣 {

inline constexpr std::uint32_t L1方法登记根当前读取合同版本 = 1;
inline constexpr std::uint32_t 方法登记根当前读取规则版本 = 1;

enum class 方法固定系统角色 : std::int64_t {
    方法登记根 = 1
};

struct 方法登记根节点见证 final {
    稳定编码 编码;
    节点种类 种类 = 节点种类::普通;
    std::uint64_t 创建事实代次 = 0;
    friend bool operator==(const 方法登记根节点见证&,
        const 方法登记根节点见证&) = default;
};

struct 方法登记根定位凭证 final {
    方法登记根节点见证 登记根;
    方法登记根节点见证 系统角色属性类型;
    方法登记根节点见证 方法领域服务;
    std::uint32_t 角色合同版本 = 1;
    friend bool operator==(const 方法登记根定位凭证&,
        const 方法登记根定位凭证&) = default;
};

struct 方法登记根角色值见证 final {
    稳定编码 编码;
    方法登记根节点见证 所属根;
    方法登记根节点见证 属性类型;
    方法固定系统角色 角色 = 方法固定系统角色::方法登记根;
    方法登记根节点见证 来源服务;
    std::uint64_t 创建事实代次 = 0;
    friend bool operator==(const 方法登记根角色值见证&,
        const 方法登记根角色值见证&) = default;
};

struct 方法登记根当前读取请求 final {
    std::uint32_t 合同版本 = L1方法登记根当前读取合同版本;
    std::uint32_t 读取规则版本 = 方法登记根当前读取规则版本;
    方法登记根定位凭证 定位;
    std::uint64_t 期望事实截止代次 = 0;
};

enum class 方法登记根当前读取状态 : std::uint8_t {
    已读取 = 1,
    未找到 = 2,
    入口拒绝 = 3,
    许可拒绝 = 4,
    版本漂移 = 5,
    资源失败 = 6,
    内部不一致 = 7
};

struct 方法登记根当前事实 final {
    std::uint32_t 合同版本 = L1方法登记根当前读取合同版本;
    std::uint32_t 读取规则版本 = 方法登记根当前读取规则版本;
    std::uint64_t 事实截止代次 = 0;
    方法登记根定位凭证 定位;
    方法登记根角色值见证 角色值;
    friend bool operator==(const 方法登记根当前事实&,
        const 方法登记根当前事实&) = default;
};

struct 方法登记根当前读取结果 final {
    方法登记根当前读取状态 状态 = 方法登记根当前读取状态::入口拒绝;
    std::uint64_t 事实截止代次 = 0;
    std::optional<方法登记根当前事实> 事实;
};

} // namespace 海中鱼巣
