#pragma once

#ifndef L1_NEUTRAL_CRUD_NO_INCLUDES
#include <cstdint>
#include <optional>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>
#include "L1公共事实.数据.h"
#endif

namespace 海中鱼巣 {

inline constexpr std::uint32_t L1中性CRUD合同版本 = 1;

struct L1中性写集本地键 final {
    std::uint32_t 值 = 0;
    friend bool operator==(const L1中性写集本地键&, const L1中性写集本地键&) = default;
    // 诊断责任：无适用错误分支；纯值全序比较不访问外部状态。
    friend bool operator<(const L1中性写集本地键& 左,
        const L1中性写集本地键& 右) noexcept { return 左.值 < 右.值; }
};

struct L1中性写集幂等键 final {
    std::uint64_t 值 = 0;
    friend bool operator==(const L1中性写集幂等键&, const L1中性写集幂等键&) = default;
};

struct L1中性独立材料引用 final {
    稳定编码 编码;
    friend bool operator==(const L1中性独立材料引用&,
        const L1中性独立材料引用&) = default;
};

using L1中性事实引用 = std::variant<稳定编码, L1中性写集本地键>;
using L1中性原始值材料 = std::variant<std::int64_t, std::vector<std::int64_t>,
    std::vector<std::uint64_t>, L1中性独立材料引用>;

enum class L1中性值表示种类 : std::uint8_t {
    I64 = 1, I64组 = 2, U64组 = 3, 独立材料引用 = 4
};

struct L1中性属性槽 final {
    稳定编码 属性类型节点;
    稳定编码 当前值;
    friend bool operator==(const L1中性属性槽&, const L1中性属性槽&) = default;
};

struct L1中性节点事实 final {
    稳定编码 编码;
    节点种类 种类 = 节点种类::普通;
    std::optional<L1中性值表示种类> 属性类型表示;
    std::uint64_t 创建事实代次 = 0;
    std::optional<std::uint64_t> 退出事实代次;
    std::vector<L1中性属性槽> 当前属性;
    friend bool operator==(const L1中性节点事实&, const L1中性节点事实&) = default;
};

struct L1中性关系事实 final {
    稳定编码 编码;
    稳定编码 源节点;
    稳定编码 目标节点;
    稳定编码 关系类型节点;
    std::int64_t 角色或顺序 = 0;
    std::uint64_t 创建事实代次 = 0;
    std::optional<std::uint64_t> 退出事实代次;
    friend bool operator==(const L1中性关系事实&, const L1中性关系事实&) = default;
};

struct L1中性值事实 final {
    稳定编码 编码;
    稳定编码 所属节点;
    稳定编码 属性类型节点;
    L1中性原始值材料 材料;
    稳定编码 来源节点;
    std::uint64_t 创建事实代次 = 0;
    std::optional<std::uint64_t> 退出事实代次;
    friend bool operator==(const L1中性值事实&, const L1中性值事实&) = default;
};

using L1中性事实副本 = std::variant<L1中性节点事实, L1中性关系事实, L1中性值事实>;

struct L1中性节点新建项 final {
    L1中性写集本地键 本地键;
    节点种类 种类 = 节点种类::普通;
    std::optional<L1中性值表示种类> 属性类型表示;
    friend bool operator==(const L1中性节点新建项&, const L1中性节点新建项&) = default;
};

struct L1中性关系新建项 final {
    L1中性写集本地键 本地键;
    L1中性事实引用 源节点;
    L1中性事实引用 目标节点;
    L1中性事实引用 关系类型节点;
    std::int64_t 角色或顺序 = 0;
    friend bool operator==(const L1中性关系新建项&, const L1中性关系新建项&) = default;
};

struct L1中性值新建项 final {
    L1中性写集本地键 本地键;
    L1中性事实引用 所属节点;
    L1中性事实引用 属性类型节点;
    L1中性原始值材料 材料;
    L1中性事实引用 来源节点;
    friend bool operator==(const L1中性值新建项&, const L1中性值新建项&) = default;
};

struct L1中性属性槽变更项 final {
    L1中性事实引用 所属节点;
    L1中性事实引用 属性类型节点;
    L1中性写集本地键 新当前值;
    friend bool operator==(const L1中性属性槽变更项&,
        const L1中性属性槽变更项&) = default;
};

struct L1中性写集请求 final {
    std::uint32_t 合同版本 = L1中性CRUD合同版本;
    std::uint64_t 期望事实代次 = 0;
    L1中性写集幂等键 幂等键;
    std::vector<L1中性节点新建项> 节点;
    std::vector<L1中性关系新建项> 关系;
    std::vector<L1中性值新建项> 值;
    std::vector<L1中性属性槽变更项> 属性槽变更;
    std::vector<稳定编码> 退出事实;
    friend bool operator==(const L1中性写集请求&, const L1中性写集请求&) = default;
};

enum class L1中性写入状态 : std::uint8_t {
    成功 = 1, 精确重复 = 2, 入口拒绝 = 3, 未找到 = 4, 已退出 = 5,
    事实代次漂移 = 6, 幂等冲突 = 7, 资源失败 = 8, 内部不一致 = 9
};

enum class L1中性重试边界 : std::uint8_t {
    不适用 = 1, 原请求可重试 = 2, 修正请求后可重试 = 3, 原幂等键读回收敛 = 4
};

struct L1中性写入结果 final {
    L1中性写入状态 状态 = L1中性写入状态::入口拒绝;
    std::uint32_t 合同版本 = L1中性CRUD合同版本;
    L1中性写集幂等键 幂等键;
    std::uint64_t 事实代次 = 0;
    bool 是否形成内存权威发布 = false;
    L1中性重试边界 重试边界 = L1中性重试边界::修正请求后可重试;
    std::vector<std::pair<L1中性写集本地键, 稳定编码>> 新编码映射;
    friend bool operator==(const L1中性写入结果&, const L1中性写入结果&) = default;
};

enum class L1中性读取状态 : std::uint8_t {
    成功 = 1, 入口拒绝 = 2, 未找到 = 3, 已退出 = 4,
    属性未设置 = 5, 资源失败 = 6, 内部不一致 = 7
};

struct L1中性事实读取请求 final {
    std::uint32_t 合同版本 = L1中性CRUD合同版本;
    稳定编码 编码;
    friend bool operator==(const L1中性事实读取请求&, const L1中性事实读取请求&) = default;
};

struct L1中性属性读取请求 final {
    std::uint32_t 合同版本 = L1中性CRUD合同版本;
    稳定编码 节点;
    稳定编码 属性类型;
    friend bool operator==(const L1中性属性读取请求&, const L1中性属性读取请求&) = default;
};

struct L1中性事实代次读取请求 final {
    std::uint32_t 合同版本 = L1中性CRUD合同版本;
    friend bool operator==(const L1中性事实代次读取请求&,
        const L1中性事实代次读取请求&) = default;
};

struct L1中性目标关系读取请求 final {
    std::uint32_t 合同版本 = L1中性CRUD合同版本;
    稳定编码 目标节点;
    稳定编码 关系类型节点;
    friend bool operator==(const L1中性目标关系读取请求&,
        const L1中性目标关系读取请求&) = default;
};

template<class 事实类型>
struct L1中性具名事实读取结果 final {
    L1中性读取状态 状态 = L1中性读取状态::入口拒绝;
    std::uint32_t 合同版本 = L1中性CRUD合同版本;
    稳定编码 查询编码;
    std::uint64_t 读取事实代次 = 0;
    std::optional<事实类型> 事实;
    friend bool operator==(const L1中性具名事实读取结果&,
        const L1中性具名事实读取结果&) = default;
};

using L1中性节点读取结果 = L1中性具名事实读取结果<L1中性节点事实>;
using L1中性关系读取结果 = L1中性具名事实读取结果<L1中性关系事实>;
using L1中性值读取结果 = L1中性具名事实读取结果<L1中性值事实>;

struct L1中性目标关系读取结果 final {
    L1中性读取状态 状态 = L1中性读取状态::入口拒绝;
    std::uint32_t 合同版本 = L1中性CRUD合同版本;
    稳定编码 目标节点;
    稳定编码 关系类型节点;
    std::uint64_t 读取事实代次 = 0;
    std::vector<L1中性关系事实> 关系组;
    friend bool operator==(const L1中性目标关系读取结果&,
        const L1中性目标关系读取结果&) = default;
};

struct L1中性属性读取结果 final {
    L1中性读取状态 状态 = L1中性读取状态::入口拒绝;
    std::uint32_t 合同版本 = L1中性CRUD合同版本;
    稳定编码 节点;
    稳定编码 属性类型;
    std::uint64_t 读取事实代次 = 0;
    std::optional<稳定编码> 当前值;
    friend bool operator==(const L1中性属性读取结果&, const L1中性属性读取结果&) = default;
};

struct L1中性历史读取结果 final {
    L1中性读取状态 状态 = L1中性读取状态::入口拒绝;
    std::uint32_t 合同版本 = L1中性CRUD合同版本;
    稳定编码 查询编码;
    std::uint64_t 读取事实代次 = 0;
    std::optional<L1中性事实副本> 事实;
    friend bool operator==(const L1中性历史读取结果&, const L1中性历史读取结果&) = default;
};

struct L1中性事实代次读取结果 final {
    L1中性读取状态 状态 = L1中性读取状态::入口拒绝;
    std::uint32_t 合同版本 = L1中性CRUD合同版本;
    std::uint64_t 事实代次 = 0;
    friend bool operator==(const L1中性事实代次读取结果&,
        const L1中性事实代次读取结果&) = default;
};

// 诊断责任：无适用错误分支；纯值有效性判断。
inline bool 有效(L1中性写集本地键 键) noexcept { return 键.值 != 0; }
// 诊断责任：无适用错误分支；纯值有效性判断。
inline bool 有效(L1中性写集幂等键 键) noexcept { return 键.值 != 0; }

// 诊断责任：无适用错误分支；纯值材料形状判断。
inline bool L1中性原始材料完整(const L1中性原始值材料& 材料) noexcept {
    return std::visit([](const auto& 值) noexcept -> bool {
        using 类型 = std::decay_t<decltype(值)>;
        if constexpr (std::is_same_v<类型, std::int64_t>) return true;
        else if constexpr (std::is_same_v<类型, L1中性独立材料引用>) return 有效(值.编码);
        else return !值.empty();
    }, 材料);
}

} // namespace 海中鱼巣
