// 文件规则：本头只承载节点直接结构域跨仓共享的稳定值类型与纯谓词，不承载仓库、事务或业务服务。
#pragma once

#include "句柄.h"

#include <array>
#include <cstdint>
#include <optional>
#include <variant>
#include <vector>

namespace 海中鱼巣 {

inline constexpr std::uint32_t 关系稳定主键命名域ABI版本 = 1;
enum class 关系稳定主键命名域 : std::uint64_t {
    无效 = 0,
    正式关系 = 0x52534B5900000001ULL
};

struct 关系稳定主键 {
    std::uint64_t 命名域 = 0;
    std::uint64_t 键值 = 0;
    friend bool operator==(const 关系稳定主键&, const 关系稳定主键&) = default;
};

inline bool 关系稳定主键完整(const 关系稳定主键& 主键) noexcept {
    return 主键.命名域 == static_cast<std::uint64_t>(关系稳定主键命名域::正式关系)
        && 主键.键值 != 0;
}

struct 节点稳定身份见证 {
    节点稳定主键 稳定主键;
    节点类型 类型 = 节点类型::未分类;
    std::uint32_t 身份版本 = 0;
    friend bool operator==(const 节点稳定身份见证&, const 节点稳定身份见证&) = default;
};

inline bool 节点稳定身份见证完整(const 节点稳定身份见证& 见证) noexcept {
    return 稳定主键有效(见证.稳定主键)
        && 见证.类型 != 节点类型::未分类
        && 见证.身份版本 != 0;
}

struct 关系稳定身份见证 {
    关系稳定主键 稳定主键;
    关系类型 类型 = 关系类型::普通父子;
    std::uint32_t 关系版本 = 0;
    节点稳定身份见证 源端;
    节点稳定身份见证 目标端;
    std::int64_t 角色或顺序 = 0;
    friend bool operator==(const 关系稳定身份见证&, const 关系稳定身份见证&) = default;
};

struct 节点直接事务幂等身份 {
    std::uint64_t 命名域 = 0;
    std::uint64_t 键值 = 0;
    friend bool operator==(const 节点直接事务幂等身份&, const 节点直接事务幂等身份&) = default;
};

inline bool 节点直接事务幂等身份完整(const 节点直接事务幂等身份& 身份) noexcept {
    return 身份.命名域 != 0 && 身份.键值 != 0;
}

inline constexpr std::uint32_t 类型合同稳定身份ABI版本 = 1;
enum class 类型合同稳定身份命名域 : std::uint64_t {
    无效 = 0,
    L2领域合同 = 0x54434B5900000001ULL
};

struct 类型合同稳定身份 {
    std::uint64_t 命名域 = 0;
    std::uint64_t 键值 = 0;
    friend bool operator==(const 类型合同稳定身份&, const 类型合同稳定身份&) = default;
};

inline bool 类型合同稳定身份完整(const 类型合同稳定身份& 身份) noexcept {
    return 身份.命名域 == static_cast<std::uint64_t>(类型合同稳定身份命名域::L2领域合同)
        && 身份.键值 != 0;
}

enum class 类型化值记录稳定身份命名域 : std::uint64_t {
    无效 = 0,
    通用类型化值记录 = 0x54564B5900000001ULL
};

struct 类型化值记录稳定身份 {
    std::uint64_t 命名域 = 0;
    std::uint64_t 键值 = 0;
    friend bool operator==(const 类型化值记录稳定身份&, const 类型化值记录稳定身份&) = default;
};

inline bool 类型化值记录稳定身份完整(const 类型化值记录稳定身份& 身份) noexcept {
    return 身份.命名域 == static_cast<std::uint64_t>(类型化值记录稳定身份命名域::通用类型化值记录)
        && 身份.键值 != 0;
}

struct 索引物理键 {
    std::uint64_t 所有者身份 = 0;
    std::uint32_t 命名域 = 0;
    std::uint32_t 键格式版本 = 0;
    std::uint32_t 探测规则版本 = 0;
    std::uint64_t 键值 = 0;
    friend bool operator==(const 索引物理键&, const 索引物理键&) = default;
};

inline bool 索引物理键完整(const 索引物理键& 键) noexcept {
    return 键.所有者身份 != 0 && 键.命名域 != 0
        && 键.键格式版本 != 0 && 键.探测规则版本 != 0 && 键.键值 != 0;
}

enum class 服务稳定身份命名域 : std::uint64_t {
    无效 = 0,
    L2领域服务 = 0x5356435900000001ULL
};

struct 服务稳定身份 {
    std::uint64_t 命名域 = 0;
    std::uint64_t 键值 = 0;
    friend bool operator==(const 服务稳定身份&, const 服务稳定身份&) = default;
};

inline bool 服务稳定身份完整(const 服务稳定身份& 身份) noexcept {
    return 身份.命名域 == static_cast<std::uint64_t>(服务稳定身份命名域::L2领域服务)
        && 身份.键值 != 0;
}

using 类型化值来源见证 = std::variant<节点稳定身份见证, 服务稳定身份>;

enum class 持久证据状态 : std::uint8_t {
    已与内存代次一致 = 1,
    待持久化 = 2,
    持久化结果未知 = 3,
    持久证据落后 = 4,
    持久证据损坏 = 5,
    不适用 = 6
};

enum class 类型化值表示种类 : std::uint8_t {
    未定义 = 0,
    I64标量 = 1,
    U64标量 = 2,
    I64区间 = 3,
    I64有序组 = 4,
    U64有序组 = 5,
    稳定身份有序组 = 6,
    独立材料引用 = 7
};

struct I64区间材料 {
    std::int64_t 下界 = 0;
    std::int64_t 上界 = 0;
    friend bool operator==(const I64区间材料&, const I64区间材料&) = default;
};

struct U64区间材料 {
    std::uint64_t 下界 = 0;
    std::uint64_t 上界 = 0;
    friend bool operator==(const U64区间材料&, const U64区间材料&) = default;
};

struct 稳定身份有序组材料 {
    std::vector<节点稳定身份见证> 项目组;
    friend bool operator==(const 稳定身份有序组材料&, const 稳定身份有序组材料&) = default;
};

struct 独立材料引用 {
    节点稳定主键 材料身份;
    std::uint32_t 格式版本 = 0;
    std::uint64_t 字节数 = 0;
    std::array<std::uint8_t, 32> SHA256{};
    friend bool operator==(const 独立材料引用&, const 独立材料引用&) = default;
};

using 类型化值材料 = std::variant<
    std::int64_t,
    std::uint64_t,
    I64区间材料,
    std::vector<std::int64_t>,
    std::vector<std::uint64_t>,
    稳定身份有序组材料,
    独立材料引用>;

enum class 类型合同兼容方向 : std::uint8_t { 仅旧读新 = 1, 仅新读旧 = 2, 双向读 = 3 };
enum class 类型合同兼容操作 : std::uint8_t { 读取 = 1, 写入 = 2, 比较 = 3, 迁移 = 4 };
enum class 类型合同生命周期状态 : std::uint8_t { 当前可写 = 1, 停止新写 = 2, 退出普通使用 = 3 };

struct 类型合同命名空间身份 {
    std::uint64_t 命名域 = 0;
    std::uint64_t 键值 = 0;
    friend bool operator==(const 类型合同命名空间身份&, const 类型合同命名空间身份&) = default;
};

inline constexpr std::uint32_t 类型合同命名空间ABI版本 = 1;
enum class 类型合同命名域 : std::uint64_t { 无效 = 0, L2领域 = 0x54434E5300000001ULL };

struct I64标量值域 {
    std::int64_t 下界 = 0;
    std::int64_t 上界 = 0;
    friend bool operator==(const I64标量值域&, const I64标量值域&) = default;
};
struct U64标量值域 {
    std::uint64_t 下界 = 0;
    std::uint64_t 上界 = 0;
    friend bool operator==(const U64标量值域&, const U64标量值域&) = default;
};
struct I64有序组值域 {
    std::int64_t 元素下界 = 0;
    std::int64_t 元素上界 = 0;
    std::uint64_t 最少项目数 = 0;
    std::uint64_t 最多项目数 = 0;
    friend bool operator==(const I64有序组值域&, const I64有序组值域&) = default;
};
struct U64有序组值域 {
    std::uint64_t 元素下界 = 0;
    std::uint64_t 元素上界 = 0;
    std::uint64_t 最少项目数 = 0;
    std::uint64_t 最多项目数 = 0;
    friend bool operator==(const U64有序组值域&, const U64有序组值域&) = default;
};
struct 稳定身份有序组值域 {
    std::vector<std::uint64_t> 允许节点类型稳定值组;
    std::uint64_t 最少项目数 = 0;
    std::uint64_t 最多项目数 = 0;
    friend bool operator==(const 稳定身份有序组值域&, const 稳定身份有序组值域&) = default;
};
struct 独立材料引用值域 {
    std::uint32_t 格式版本 = 0;
    std::uint64_t 最少字节数 = 0;
    std::uint64_t 最多字节数 = 0;
    friend bool operator==(const 独立材料引用值域&, const 独立材料引用值域&) = default;
};

using 类型合同值域 = std::variant<
    I64标量值域,
    U64标量值域,
    I64区间材料,
    I64有序组值域,
    U64有序组值域,
    稳定身份有序组值域,
    独立材料引用值域>;

struct 类型合同直接兼容声明 {
    类型合同稳定身份 另一合同身份;
    std::uint32_t 另一合同版本 = 0;
    类型合同兼容操作 操作 = 类型合同兼容操作::读取;
    类型合同兼容方向 方向 = 类型合同兼容方向::仅旧读新;
    friend bool operator==(const 类型合同直接兼容声明&, const 类型合同直接兼容声明&) = default;
};

struct 类型合同读回 {
    类型合同稳定身份 合同身份;
    类型合同命名空间身份 命名空间;
    std::uint32_t 合同版本 = 0;
    类型化值表示种类 表示 = 类型化值表示种类::未定义;
    类型合同值域 值域;
    服务稳定身份 所有者服务;
    类型合同生命周期状态 生命周期 = 类型合同生命周期状态::当前可写;
    std::vector<类型合同直接兼容声明> 直接兼容组;
    std::uint64_t 首次发布代次 = 0;
    std::uint64_t 当前状态发布代次 = 0;
    friend bool operator==(const 类型合同读回&, const 类型合同读回&) = default;
};

struct 类型合同稳定身份历史占用 { 类型合同稳定身份 身份; };

struct 类型化值读回 {
    节点稳定身份见证 所属身份;
    类型合同稳定身份 类型合同身份;
    std::uint32_t 类型合同版本 = 0;
    类型化值记录稳定身份 值记录身份;
    std::uint64_t 值记录版本 = 0;
    bool 当前 = false;
    类型化值材料 材料;
    类型化值来源见证 来源;
    std::optional<std::uint64_t> 前一值记录版本;
    std::uint64_t 首次发布代次 = 0;
    std::uint64_t 当前状态发布代次 = 0;
    friend bool operator==(const 类型化值读回&, const 类型化值读回&) = default;
};

struct 类型化值记录稳定身份历史占用 { 类型化值记录稳定身份 身份; };

struct 可重建索引读回 {
    索引物理键 键;
    bool 当前 = false;
    std::optional<节点稳定身份见证> 节点目标;
    std::optional<关系稳定身份见证> 关系目标;
    friend bool operator==(const 可重建索引读回&, const 可重建索引读回&) = default;
};

enum class 节点直接事务幂等记录状态 : std::uint8_t { 待发布 = 1, 已发布 = 2, 已隔离 = 3 };

struct 节点直接事务幂等记录 {
    节点直接事务幂等身份 安装实例身份;
    节点直接事务幂等身份 幂等身份;
    std::array<std::uint8_t, 32> 请求意图摘要{};
    std::array<std::uint8_t, 32> 执行证据摘要{};
    std::uint32_t 材料格式版本 = 2;
    std::uint32_t 写集规则版本 = 2;
    std::vector<std::uint8_t> 写集材料;
    节点直接事务幂等记录状态 状态 = 节点直接事务幂等记录状态::待发布;
    std::uint64_t 发布代次 = 0;
    std::array<std::uint8_t, 32> 结果摘要{};
    std::vector<节点稳定身份见证> 节点组;
    std::vector<关系稳定身份见证> 关系组;
    std::vector<类型化值读回> 类型化值组;
    std::vector<可重建索引读回> 索引组;
    friend bool operator==(const 节点直接事务幂等记录&, const 节点直接事务幂等记录&) = default;
};

struct 节点直接持久证据侧账记录 {
    节点直接事务幂等身份 幂等身份;
    std::uint64_t 尝试序号 = 0;
    持久证据状态 状态 = 持久证据状态::不适用;
    friend bool operator==(const 节点直接持久证据侧账记录&,
        const 节点直接持久证据侧账记录&) = default;
};

enum class 节点直接仓候选阶段 : std::uint8_t {
    持有 = 1, 已确认待发布 = 2, 已撤销 = 3, 已发布 = 4, 已移动 = 5
};
enum class 节点直接仓候选操作状态 : std::uint8_t {
    已形成候选 = 1, 已确认待发布 = 2, 已撤销 = 3,
    已发布 = 4, 精确同义 = 5, 入口拒绝 = 6,
    版本漂移 = 7, 资源失败 = 8, 内部不一致 = 9
};

inline constexpr std::uint32_t 节点直接结构服务合同版本 = 1;

enum class 节点直接结构服务状态 : std::uint8_t {
    成功 = 1, 未找到 = 2, 入口拒绝 = 3, 版本漂移 = 4,
    许可拒绝 = 5, 资源失败 = 6, 内部不一致 = 7
};

struct 节点直接结构结果头 {
    节点直接结构服务状态 状态 = 节点直接结构服务状态::入口拒绝;
    std::uint64_t 事实截止代次 = 0;
};
struct 节点直接节点读取结果 {
    节点直接结构结果头 头;
    std::optional<节点稳定身份见证> 节点;
};
struct 节点直接关系组读取结果 {
    节点直接结构结果头 头;
    std::vector<关系稳定身份见证> 关系组;
};
struct 节点直接代次读取结果 { 节点直接结构结果头 头; };
struct 节点直接类型化值读取结果 {
    节点直接结构结果头 头;
    std::vector<类型化值读回> 值组;
};
struct 节点直接类型合同读取结果 {
    节点直接结构结果头 头;
    std::optional<类型合同读回> 合同;
};
struct 节点直接索引读取结果 {
    节点直接结构结果头 头;
    std::vector<可重建索引读回> 索引组;
};

inline constexpr std::uint32_t 节点直接两级关系值投影合同版本 = 1;

struct 节点直接两级关系值投影请求 {
    std::uint32_t 合同版本 = 节点直接两级关系值投影合同版本;
    std::vector<节点稳定身份见证> 必须节点组;
    std::vector<节点稳定身份见证> 根节点组;
    关系类型 关系种类 = 关系类型::普通父子;
    std::int64_t 第一跳角色 = 0;
    std::int64_t 类型化值目标角色 = 0;
};

struct 节点直接关系目标当前类型化值投影 {
    关系稳定身份见证 目标关系;
    std::vector<类型化值读回> 当前值组;
};

struct 节点直接两级关系值投影结果 {
    节点直接结构结果头 头;
    std::vector<节点稳定身份见证> 必须节点组;
    std::vector<关系稳定身份见证> 第一跳关系组;
    std::vector<关系稳定身份见证> 中间节点相关关系组;
    std::vector<节点直接关系目标当前类型化值投影> 目标值组;
    std::optional<节点稳定身份见证> 未满足节点;
};

inline constexpr std::uint32_t 节点直接目标角色源投影合同版本 = 1;

struct 节点直接目标角色筛选 {
    节点稳定身份见证 目标;
    关系类型 关系种类 = 关系类型::普通父子;
    std::int64_t 目标角色 = 0;
};

struct 节点直接目标角色源投影请求 {
    std::uint32_t 合同版本 = 节点直接目标角色源投影合同版本;
    std::vector<节点稳定身份见证> 必须节点组;
    std::vector<节点直接目标角色筛选> 筛选组;
};

struct 节点直接当前关系源投影 {
    关系类型 关系种类 = 关系类型::普通父子;
    节点稳定身份见证 源;
    std::vector<关系稳定身份见证> 当前关系组;
    std::vector<类型化值读回> 当前类型化值组;
};

struct 节点直接目标角色源投影结果 {
    节点直接结构结果头 头;
    std::vector<节点稳定身份见证> 必须节点组;
    std::vector<节点直接当前关系源投影> 源组;
    std::optional<节点稳定身份见证> 未满足节点;
};

enum class 节点直接写集局部身份种类 : std::uint8_t {
    节点 = 1, 关系 = 2, 类型化值记录 = 3
};
struct 节点直接写集局部身份 {
    节点直接写集局部身份种类 种类 = 节点直接写集局部身份种类::节点;
    std::uint64_t 值 = 0;
    friend bool operator==(const 节点直接写集局部身份&, const 节点直接写集局部身份&) = default;
};
struct 节点直接节点创建项 {
    节点直接写集局部身份 局部身份;
    节点稳定主键命名域 命名域 = 节点稳定主键命名域::无效;
    节点类型 类型 = 节点类型::未分类;
};
using 节点直接节点端点引用 = std::variant<节点稳定身份见证, 节点直接写集局部身份>;
using 节点直接类型化值来源 = std::variant<节点直接节点端点引用, 服务稳定身份>;
struct 节点直接关系创建项 {
    节点直接写集局部身份 局部身份;
    关系类型 类型 = 关系类型::普通父子;
    节点直接节点端点引用 源端;
    节点直接节点端点引用 目标端;
    std::int64_t 角色或顺序 = 0;
};
struct 节点直接关系失效项 { 关系稳定身份见证 预期当前; };
struct 节点直接类型合同发布项 {
    类型合同稳定身份 合同身份;
    类型合同命名空间身份 命名空间;
    std::uint32_t 合同版本 = 0;
    类型化值表示种类 表示 = 类型化值表示种类::未定义;
    类型合同值域 值域;
    服务稳定身份 所有者服务;
    类型合同生命周期状态 生命周期 = 类型合同生命周期状态::当前可写;
    std::vector<类型合同直接兼容声明> 直接兼容组;
};
struct 节点直接类型化值发布项 {
    节点直接节点端点引用 所属身份;
    类型合同稳定身份 类型合同身份;
    std::uint32_t 类型合同版本 = 0;
    节点直接写集局部身份 值记录局部身份;
    std::optional<std::uint64_t> 预期当前值记录版本;
    类型化值材料 材料;
    节点直接类型化值来源 来源;
};
struct 节点直接类型化值退役项 {
    节点稳定身份见证 所属身份;
    类型合同稳定身份 类型合同身份;
    std::uint32_t 类型合同版本 = 0;
    类型化值记录稳定身份 值记录身份;
    std::uint64_t 预期值记录版本 = 0;
};
struct 节点直接索引创建项 { 索引物理键 键; 节点直接节点端点引用 目标; };
struct 节点直接索引移除项 { 索引物理键 键; 节点稳定身份见证 预期目标; };
struct 节点直接节点删除项 { 节点稳定身份见证 预期当前; };
using 节点直接类型化结构写项 = std::variant<
    节点直接类型合同发布项, 节点直接节点创建项,
    节点直接关系创建项, 节点直接关系失效项,
    节点直接类型化值发布项, 节点直接类型化值退役项,
    节点直接索引创建项, 节点直接索引移除项, 节点直接节点删除项>;
using 节点直接类型化结构写项组 = std::vector<节点直接类型化结构写项>;

enum class 节点直接发布后读回对象种类 : std::uint8_t {
    当前节点 = 1, 当前关系 = 2, 已失效关系 = 3,
    当前类型化值 = 4, 当前索引 = 5, 已移除索引 = 6
};
struct 节点直接发布后读回项 {
    节点直接发布后读回对象种类 种类 = 节点直接发布后读回对象种类::当前节点;
    std::variant<节点直接写集局部身份, 节点稳定主键, 关系稳定主键,
        类型化值记录稳定身份, 索引物理键> 身份;
    std::uint64_t 预期版本 = 0;
};
struct 节点直接发布后读回规格 { std::vector<节点直接发布后读回项> 项目组; };

struct 节点直接类型化结构事务请求 {
    std::uint32_t 合同版本 = 节点直接结构服务合同版本;
    std::uint32_t 写集规则版本 = 2;
    节点直接事务幂等身份 幂等身份;
    std::array<std::uint8_t, 32> 请求意图摘要{};
    std::array<std::uint8_t, 32> 执行证据摘要{};
    std::uint64_t 预期事实截止代次 = 0;
    节点直接类型化结构写项组 写项组;
    节点直接发布后读回规格 读回规格;
};
enum class 节点直接类型化结构提交状态 : std::uint8_t {
    已提交 = 1, 幂等读回 = 2, 入口拒绝 = 3, 幂等冲突 = 4,
    版本漂移 = 5, 许可拒绝 = 6, 资源失败 = 7, 内部不一致 = 8
};
struct 节点直接类型化结构提交结果 {
    节点直接类型化结构提交状态 状态 = 节点直接类型化结构提交状态::入口拒绝;
    std::uint64_t 发布代次 = 0;
    持久证据状态 持久状态 = 持久证据状态::不适用;
    std::vector<节点稳定身份见证> 节点组;
    std::vector<关系稳定身份见证> 关系组;
    std::vector<类型化值读回> 类型化值组;
    std::vector<可重建索引读回> 索引组;
};
enum class 节点直接类型化结构幂等探测状态 : std::uint8_t {
    未找到 = 1, 同义已发布 = 2, 异义冲突 = 3,
    待发布或隔离 = 4, 许可拒绝 = 5, 内部不一致 = 6
};
struct 节点直接类型化结构幂等探测结果 {
    节点直接类型化结构幂等探测状态 状态 = 节点直接类型化结构幂等探测状态::内部不一致;
    std::optional<节点直接类型化结构提交结果> 原结果;
};
enum class 节点直接持久见证重试状态 : std::uint8_t {
    已与内存代次一致 = 1, 无需重试 = 2, 未找到 = 3,
    许可拒绝 = 4, 资源失败 = 5, 持久证据损坏 = 6, 内部不一致 = 7
};
struct 节点直接持久见证重试结果 {
    节点直接持久见证重试状态 状态 = 节点直接持久见证重试状态::内部不一致;
    持久证据状态 持久状态 = 持久证据状态::不适用;
};

} // namespace 海中鱼巣
