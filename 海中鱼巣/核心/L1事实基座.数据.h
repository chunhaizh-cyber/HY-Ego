#pragma once

#ifndef L1_FACT_BASE_NO_INCLUDES
#include <cstddef>
#include <cstdint>
#include <optional>
#include <variant>
#include <vector>
#endif

#ifndef L1_PUBLIC_FACT_NO_INCLUDES
#include "L1公共事实.数据.h"
#endif

#ifndef L1_OWNER_SCOPED_CRUD_NO_INCLUDES
#include "L1所有者范围CRUD.数据.h"
#endif

namespace 海中鱼巣 {

struct 独立材料引用 final {
    稳定编码 编码;
    friend bool operator==(const 独立材料引用&, const 独立材料引用&) = default;
};

using 原始值材料 = std::variant<std::int64_t, std::vector<std::int64_t>,
    std::vector<std::uint64_t>, 独立材料引用>;

enum class 值表示种类 : std::uint8_t {
    I64 = 1, I64组 = 2, U64组 = 3, 独立材料引用 = 4
};

struct 属性槽 final {
    稳定编码 属性类型节点;
    稳定编码 当前值;
    friend bool operator==(const 属性槽&, const 属性槽&) = default;
};

struct 节点事实 final {
    稳定编码 编码;
    节点种类 种类 = 节点种类::普通;
    std::optional<值表示种类> 属性类型表示;
    std::uint64_t 创建事实代次 = 0;
    std::optional<std::uint64_t> 退出事实代次;
    std::vector<属性槽> 当前属性;
    L1结构所有者身份 写入所有者;
    friend bool operator==(const 节点事实&, const 节点事实&) = default;
};

struct 关系事实 final {
    稳定编码 编码;
    稳定编码 源节点;
    稳定编码 目标节点;
    稳定编码 关系类型节点;
    std::int64_t 角色或顺序 = 0;
    std::uint64_t 创建事实代次 = 0;
    std::optional<std::uint64_t> 退出事实代次;
    L1结构所有者身份 写入所有者;
    friend bool operator==(const 关系事实&, const 关系事实&) = default;
};

struct 值事实 final {
    稳定编码 编码;
    稳定编码 所属节点;
    稳定编码 属性类型节点;
    原始值材料 材料;
    稳定编码 来源节点;
    std::uint64_t 创建事实代次 = 0;
    std::optional<std::uint64_t> 退出事实代次;
    L1结构所有者身份 写入所有者;
    friend bool operator==(const 值事实&, const 值事实&) = default;
};

struct L1属性读取副本 final {
    稳定编码 节点;
    稳定编码 属性类型节点;
    稳定编码 当前值;
    friend bool operator==(const L1属性读取副本&, const L1属性读取副本&) = default;
};

enum class L1读取状态 : std::uint8_t {
    成功 = 1, 入口拒绝 = 2, 未找到 = 3, 已退出 = 4,
    属性未设置 = 5, 事实代次漂移 = 6, 资源失败 = 7,
    内部不一致 = 8, 许可拒绝 = 9
};

using L1事实副本 = std::variant<节点事实, 关系事实, 值事实>;

struct L1读取结果 final {
    L1读取状态 状态 = L1读取状态::入口拒绝;
    std::uint64_t 读取事实代次 = 0;
    std::optional<L1事实副本> 事实;
};

struct L1属性读取结果 final {
    L1读取状态 状态 = L1读取状态::入口拒绝;
    std::uint64_t 读取事实代次 = 0;
    std::optional<L1属性读取副本> 属性;
};

struct L1历史事实副本 final {
    稳定编码 查询编码;
    L1事实副本 事实;
    bool 当前有效 = false;
};

struct L1历史读取结果 final {
    L1读取状态 状态 = L1读取状态::入口拒绝;
    std::optional<L1历史事实副本> 历史;
};

struct L1事实代次读取结果 final {
    L1读取状态 状态 = L1读取状态::入口拒绝;
    std::uint64_t 事实截止代次 = 0;
    // 诊断责任：无适用错误分支；默认比较只比较纯值结果。
    friend bool operator==(const L1事实代次读取结果&,
        const L1事实代次读取结果&) = default;
};

// 诊断责任：无适用错误分支；只核对属性槽稳定排序和编码有效性。
inline bool 属性排序唯一(const std::vector<属性槽>& 槽组) noexcept {
    for (std::size_t 序号 = 0; 序号 < 槽组.size(); ++序号) {
        if (!有效(槽组[序号].属性类型节点) || !有效(槽组[序号].当前值))
            return false;
        if (序号 != 0
            && !(槽组[序号 - 1].属性类型节点 < 槽组[序号].属性类型节点))
            return false;
    }
    return true;
}

} // namespace 海中鱼巣
