#pragma once

#ifndef L1_OWNER_SCOPED_CRUD_NO_INCLUDES
#include <cstdint>
#include <optional>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>
#include "L1公共事实.数据.h"
#endif

namespace 海中鱼巣 {

inline constexpr std::uint32_t L1所有者范围CRUD合同版本 = 2;
inline constexpr std::uint32_t L1所有者范围一致当前读取合同版本 = 2;
inline constexpr std::uint32_t L1所有者范围一致关系类型闭包读取合同版本 = 1;
inline constexpr std::uint32_t L1所有者范围首次写入读取合同版本 = 1;
inline constexpr std::uint32_t L1所有者范围来源当前值组读取合同版本 = 1;
inline constexpr std::uint32_t L1所有者范围属性类型当前值组读取合同版本 = 1;

struct L1结构所有者身份 final {
    稳定编码 编码;
    friend bool operator==(const L1结构所有者身份&,
        const L1结构所有者身份&) = default;
};

enum class L1所有者范围种类 : std::uint8_t {
    旧共享范围 = 1,
    独占结构范围 = 2
};

struct L1结构所有者事实 final {
    L1结构所有者身份 所有者;
    L1所有者范围种类 范围种类 = L1所有者范围种类::旧共享范围;
    std::uint64_t 创建事实代次 = 0;
    std::optional<std::uint64_t> 退出事实代次;
    friend bool operator==(const L1结构所有者事实&,
        const L1结构所有者事实&) = default;
};

struct L1所有者范围建立幂等身份 final {
    std::uint64_t 值 = 0;
    friend bool operator==(const L1所有者范围建立幂等身份&,
        const L1所有者范围建立幂等身份&) = default;
};

struct L1所有者范围写入幂等身份 final {
    std::uint64_t 值 = 0;
    friend bool operator==(const L1所有者范围写入幂等身份&,
        const L1所有者范围写入幂等身份&) = default;
};

enum class L1所有者范围管理状态 : std::uint8_t {
    成功 = 1, 精确重复 = 2, 入口拒绝 = 3, 许可拒绝 = 4,
    未找到 = 5, 已退出 = 6, 事实代次漂移 = 7, 幂等冲突 = 8,
    引用冲突 = 9, 资源失败 = 10, 内部不一致 = 11
};

enum class L1所有者范围重试边界 : std::uint8_t {
    不适用 = 1, 原请求可重试 = 2, 修正请求后可重试 = 3,
    原幂等身份读回收敛 = 4
};

struct L1所有者范围建立请求 final {
    std::uint32_t 合同版本 = L1所有者范围CRUD合同版本;
    L1所有者范围建立幂等身份 建立幂等身份;
    L1所有者范围种类 范围种类 = L1所有者范围种类::独占结构范围;
    friend bool operator==(const L1所有者范围建立请求&,
        const L1所有者范围建立请求&) = default;
};

struct L1所有者范围重入请求 final {
    std::uint32_t 合同版本 = L1所有者范围CRUD合同版本;
    L1结构所有者身份 所有者;
    L1所有者范围建立幂等身份 建立幂等身份;
    friend bool operator==(const L1所有者范围重入请求&,
        const L1所有者范围重入请求&) = default;
};

struct L1所有者范围退出请求 final {
    std::uint32_t 合同版本 = L1所有者范围CRUD合同版本;
    L1结构所有者身份 所有者;
    L1所有者范围建立幂等身份 建立幂等身份;
    std::uint64_t 期望事实代次 = 0;
    friend bool operator==(const L1所有者范围退出请求&,
        const L1所有者范围退出请求&) = default;
};

struct L1结构所有者读取请求 final {
    std::uint32_t 合同版本 = L1所有者范围CRUD合同版本;
    L1结构所有者身份 所有者;
    friend bool operator==(const L1结构所有者读取请求&,
        const L1结构所有者读取请求&) = default;
};

#define 定义L1所有者管理结果(类型名) \
    struct 类型名 final { \
        L1所有者范围管理状态 状态 = L1所有者范围管理状态::入口拒绝; \
        std::uint32_t 合同版本 = L1所有者范围CRUD合同版本; \
        L1所有者范围建立幂等身份 建立幂等身份; \
        std::optional<L1结构所有者事实> 所有者事实; \
        std::uint64_t 事实代次 = 0; \
        bool 是否形成内存权威发布 = false; \
        L1所有者范围重试边界 重试边界 = \
            L1所有者范围重试边界::修正请求后可重试; \
        friend bool operator==(const 类型名&, const 类型名&) = default; \
    }

定义L1所有者管理结果(L1所有者范围建立结果);
定义L1所有者管理结果(L1所有者范围重入结果);
定义L1所有者管理结果(L1所有者范围退出结果);

#undef 定义L1所有者管理结果

struct L1所有者范围写集本地键 final {
    std::uint32_t 值 = 0;
    friend bool operator==(const L1所有者范围写集本地键&,
        const L1所有者范围写集本地键&) = default;
    friend bool operator<(const L1所有者范围写集本地键& 左,
        const L1所有者范围写集本地键& 右) noexcept { return 左.值 < 右.值; }
};

struct L1所有者范围独立材料引用 final {
    稳定编码 编码;
    friend bool operator==(const L1所有者范围独立材料引用&,
        const L1所有者范围独立材料引用&) = default;
};

using L1所有者范围事实引用 =
    std::variant<稳定编码, L1所有者范围写集本地键>;
using L1所有者范围原始值材料 = std::variant<std::int64_t,
    std::vector<std::int64_t>, std::vector<std::uint64_t>,
    L1所有者范围独立材料引用>;

enum class L1所有者范围值表示种类 : std::uint8_t {
    I64 = 1, I64组 = 2, U64组 = 3, 独立材料引用 = 4
};

struct L1所有者范围属性槽 final {
    稳定编码 属性类型节点;
    稳定编码 当前值;
    friend bool operator==(const L1所有者范围属性槽&,
        const L1所有者范围属性槽&) = default;
};

struct L1所有者范围节点新建项 final {
    L1所有者范围写集本地键 本地键;
    节点种类 种类 = 节点种类::普通;
    std::optional<L1所有者范围值表示种类> 属性类型表示;
    friend bool operator==(const L1所有者范围节点新建项&,
        const L1所有者范围节点新建项&) = default;
};

struct L1所有者范围关系新建项 final {
    L1所有者范围写集本地键 本地键;
    L1所有者范围事实引用 源节点;
    L1所有者范围事实引用 目标节点;
    L1所有者范围事实引用 关系类型节点;
    std::int64_t 角色或顺序 = 0;
    friend bool operator==(const L1所有者范围关系新建项&,
        const L1所有者范围关系新建项&) = default;
};

struct L1所有者范围值新建项 final {
    L1所有者范围写集本地键 本地键;
    L1所有者范围事实引用 所属节点;
    L1所有者范围事实引用 属性类型节点;
    L1所有者范围原始值材料 材料;
    L1所有者范围事实引用 来源节点;
    friend bool operator==(const L1所有者范围值新建项&,
        const L1所有者范围值新建项&) = default;
};

struct L1所有者范围属性槽变更项 final {
    L1所有者范围事实引用 所属节点;
    L1所有者范围事实引用 属性类型节点;
    L1所有者范围写集本地键 新当前值;
    friend bool operator==(const L1所有者范围属性槽变更项&,
        const L1所有者范围属性槽变更项&) = default;
};

struct L1所有者范围写集请求 final {
    std::uint32_t 合同版本 = L1所有者范围CRUD合同版本;
    std::uint64_t 期望事实代次 = 0;
    L1所有者范围写入幂等身份 写入幂等身份;
    std::vector<L1所有者范围节点新建项> 节点;
    std::vector<L1所有者范围关系新建项> 关系;
    std::vector<L1所有者范围值新建项> 值;
    std::vector<L1所有者范围属性槽变更项> 属性槽变更;
    std::vector<稳定编码> 退出事实;
    friend bool operator==(const L1所有者范围写集请求&,
        const L1所有者范围写集请求&) = default;
};

enum class L1所有者范围写入状态 : std::uint8_t {
    成功 = 1, 精确重复 = 2, 入口拒绝 = 3, 许可拒绝 = 4,
    未找到 = 5, 已退出 = 6, 事实代次漂移 = 7, 幂等冲突 = 8,
    引用冲突 = 9, 资源失败 = 10, 内部不一致 = 11
};

struct L1所有者范围写入结果 final {
    L1所有者范围写入状态 状态 = L1所有者范围写入状态::入口拒绝;
    std::uint32_t 合同版本 = L1所有者范围CRUD合同版本;
    L1结构所有者身份 所有者;
    L1所有者范围写入幂等身份 写入幂等身份;
    std::uint64_t 事实代次 = 0;
    bool 是否形成内存权威发布 = false;
    L1所有者范围重试边界 重试边界 =
        L1所有者范围重试边界::修正请求后可重试;
    std::vector<std::pair<L1所有者范围写集本地键, 稳定编码>> 新编码映射;
    friend bool operator==(const L1所有者范围写入结果&,
        const L1所有者范围写入结果&) = default;
};

// ARCH-L2 跨结构组合发布只使用中性参与者合同；普通 owner CRUD 不经过此合同。
inline constexpr std::uint32_t L1跨所有者原子事务合同版本 = 1;

enum class L1跨所有者原子事务参与者序号 : std::uint8_t {
    状态 = 1,
    动态 = 2
};

enum class L1跨所有者原子事务状态 : std::uint8_t {
    已提交 = 1,
    精确重复 = 2,
    入口拒绝 = 3,
    许可拒绝 = 4,
    未找到 = 5,
    已退出 = 6,
    事实代次漂移 = 7,
    幂等冲突 = 8,
    引用冲突 = 9,
    资源失败 = 10,
    内部不一致 = 11
};

struct L1跨所有者原子事实引用 final {
    L1跨所有者原子事务参与者序号 参与者 =
        L1跨所有者原子事务参与者序号::状态;
    L1所有者范围写集本地键 本地键;
    friend bool operator==(const L1跨所有者原子事实引用&,
        const L1跨所有者原子事实引用&) = default;
};

// 仅供跨 owner 原子参与者写集使用。普通 owner CRUD 永远只接受
// L1所有者范围事实引用，不暴露跨参与者局部位置。
using L1跨所有者原子事实引用值 = std::variant<
    稳定编码,
    L1所有者范围写集本地键,
    L1跨所有者原子事实引用>;

struct L1跨所有者原子节点新建项 final {
    L1所有者范围写集本地键 本地键;
    节点种类 种类 = 节点种类::普通;
    std::optional<L1所有者范围值表示种类> 属性类型表示;
    friend bool operator==(const L1跨所有者原子节点新建项&,
        const L1跨所有者原子节点新建项&) = default;
};

struct L1跨所有者原子关系新建项 final {
    L1所有者范围写集本地键 本地键;
    L1跨所有者原子事实引用值 源节点;
    L1跨所有者原子事实引用值 目标节点;
    L1跨所有者原子事实引用值 关系类型节点;
    std::int64_t 角色或顺序 = 0;
    friend bool operator==(const L1跨所有者原子关系新建项&,
        const L1跨所有者原子关系新建项&) = default;
};

struct L1跨所有者原子值新建项 final {
    L1所有者范围写集本地键 本地键;
    L1跨所有者原子事实引用值 所属节点;
    L1跨所有者原子事实引用值 属性类型节点;
    L1所有者范围原始值材料 材料;
    L1跨所有者原子事实引用值 来源节点;
    friend bool operator==(const L1跨所有者原子值新建项&,
        const L1跨所有者原子值新建项&) = default;
};

struct L1跨所有者原子属性槽变更项 final {
    L1跨所有者原子事实引用值 所属节点;
    L1跨所有者原子事实引用值 属性类型节点;
    L1所有者范围写集本地键 新当前值;
    friend bool operator==(const L1跨所有者原子属性槽变更项&,
        const L1跨所有者原子属性槽变更项&) = default;
};

struct L1跨所有者原子写集请求 final {
    std::uint32_t 合同版本 = L1所有者范围CRUD合同版本;
    std::uint64_t 期望事实代次 = 0;
    L1所有者范围写入幂等身份 写入幂等身份;
    std::vector<L1跨所有者原子节点新建项> 节点;
    std::vector<L1跨所有者原子关系新建项> 关系;
    std::vector<L1跨所有者原子值新建项> 值;
    std::vector<L1跨所有者原子属性槽变更项> 属性槽变更;
    std::vector<稳定编码> 退出事实;
    friend bool operator==(const L1跨所有者原子写集请求&,
        const L1跨所有者原子写集请求&) = default;
};

struct L1跨所有者原子参与者写集 final {
    L1跨所有者原子事务参与者序号 参与者 =
        L1跨所有者原子事务参与者序号::状态;
    L1结构所有者身份 所有者;
    L1跨所有者原子写集请求 写集;
    friend bool operator==(const L1跨所有者原子参与者写集&,
        const L1跨所有者原子参与者写集&) = default;
};

struct L1跨所有者原子事务请求 final {
    std::uint32_t 合同版本 = L1跨所有者原子事务合同版本;
    std::uint64_t 共同期望事实代次 = 0;
    L1所有者范围写入幂等身份 组合写入幂等身份;
    L1跨所有者原子参与者写集 状态写集;
    L1跨所有者原子参与者写集 动态写集;
    friend bool operator==(const L1跨所有者原子事务请求&,
        const L1跨所有者原子事务请求&) = default;
};

struct L1跨所有者原子事务结果 final {
    L1跨所有者原子事务状态 状态 =
        L1跨所有者原子事务状态::入口拒绝;
    std::uint32_t 合同版本 = L1跨所有者原子事务合同版本;
    std::uint64_t 共同事实代次 = 0;
    bool 是否形成内存权威发布 = false;
    L1所有者范围重试边界 重试边界 =
        L1所有者范围重试边界::修正请求后可重试;
    std::vector<std::pair<L1所有者范围写集本地键, 稳定编码>> 状态编码映射;
    std::vector<std::pair<L1所有者范围写集本地键, 稳定编码>> 动态编码映射;
    friend bool operator==(const L1跨所有者原子事务结果&,
        const L1跨所有者原子事务结果&) = default;
};

// 恰好三个中性内部结构分区的一次原子发布合同。该 v2 合同与上方
// 固定状态/动态双参与者 v1 物理隔离，不复义也不迁移旧请求。
inline constexpr std::uint32_t L1三分区原子事务合同版本_v2 = 2;

struct L1三分区原子参与者身份_v2 final {
    std::uint8_t 值 = 0;
    friend bool operator==(const L1三分区原子参与者身份_v2&,
        const L1三分区原子参与者身份_v2&) = default;
};

struct L1三分区原子事实引用_v2 final {
    L1三分区原子参与者身份_v2 参与者;
    L1所有者范围写集本地键 本地键;
    friend bool operator==(const L1三分区原子事实引用_v2&,
        const L1三分区原子事实引用_v2&) = default;
};

using L1三分区原子事实引用值_v2 = std::variant<
    稳定编码,
    L1所有者范围写集本地键,
    L1三分区原子事实引用_v2>;

struct L1三分区原子节点新建项_v2 final {
    L1所有者范围写集本地键 本地键;
    节点种类 种类 = 节点种类::普通;
    std::optional<L1所有者范围值表示种类> 属性类型表示;
    friend bool operator==(const L1三分区原子节点新建项_v2&,
        const L1三分区原子节点新建项_v2&) = default;
};

struct L1三分区原子关系新建项_v2 final {
    L1所有者范围写集本地键 本地键;
    L1三分区原子事实引用值_v2 源节点;
    L1三分区原子事实引用值_v2 目标节点;
    L1三分区原子事实引用值_v2 关系类型节点;
    std::int64_t 角色或顺序 = 0;
    friend bool operator==(const L1三分区原子关系新建项_v2&,
        const L1三分区原子关系新建项_v2&) = default;
};

struct L1三分区原子值新建项_v2 final {
    L1所有者范围写集本地键 本地键;
    L1三分区原子事实引用值_v2 所属节点;
    L1三分区原子事实引用值_v2 属性类型节点;
    L1所有者范围原始值材料 材料;
    L1三分区原子事实引用值_v2 来源节点;
    friend bool operator==(const L1三分区原子值新建项_v2&,
        const L1三分区原子值新建项_v2&) = default;
};

struct L1三分区原子属性槽变更项_v2 final {
    L1三分区原子事实引用值_v2 所属节点;
    L1三分区原子事实引用值_v2 属性类型节点;
    L1所有者范围写集本地键 新当前值;
    friend bool operator==(const L1三分区原子属性槽变更项_v2&,
        const L1三分区原子属性槽变更项_v2&) = default;
};

struct L1三分区原子写集请求_v2 final {
    std::uint32_t 合同版本 = L1所有者范围CRUD合同版本;
    std::uint64_t 期望事实代次 = 0;
    L1所有者范围写入幂等身份 写入幂等身份;
    std::vector<L1三分区原子节点新建项_v2> 节点;
    std::vector<L1三分区原子关系新建项_v2> 关系;
    std::vector<L1三分区原子值新建项_v2> 值;
    std::vector<L1三分区原子属性槽变更项_v2> 属性槽变更;
    std::vector<稳定编码> 退出事实;
    friend bool operator==(const L1三分区原子写集请求_v2&,
        const L1三分区原子写集请求_v2&) = default;
};

struct L1三分区原子参与者写集_v2 final {
    L1三分区原子参与者身份_v2 参与者;
    L1结构所有者身份 所有者;
    L1三分区原子写集请求_v2 写集;
    friend bool operator==(const L1三分区原子参与者写集_v2&,
        const L1三分区原子参与者写集_v2&) = default;
};

struct L1三分区原子事务请求_v2 final {
    std::uint32_t 合同版本 = L1三分区原子事务合同版本_v2;
    std::uint64_t 共同期望事实代次 = 0;
    L1所有者范围写入幂等身份 组合写入幂等身份;
    std::vector<L1三分区原子参与者写集_v2> 参与者写集组;
    friend bool operator==(const L1三分区原子事务请求_v2&,
        const L1三分区原子事务请求_v2&) = default;
};

enum class L1三分区原子事务状态_v2 : std::uint8_t {
    已提交 = 1,
    精确重复 = 2,
    入口拒绝 = 3,
    事实代次漂移 = 4,
    幂等冲突 = 5,
    引用冲突 = 6,
    资源失败 = 7,
    内部不一致 = 8,
    已可能发布 = 9
};

struct L1三分区原子参与者结果_v2 final {
    L1三分区原子参与者身份_v2 参与者;
    L1结构所有者身份 所有者;
    std::vector<std::pair<L1所有者范围写集本地键, 稳定编码>> 新编码映射;
    friend bool operator==(const L1三分区原子参与者结果_v2&,
        const L1三分区原子参与者结果_v2&) = default;
};

struct L1三分区原子事务结果_v2 final {
    L1三分区原子事务状态_v2 状态 =
        L1三分区原子事务状态_v2::入口拒绝;
    std::uint32_t 合同版本 = L1三分区原子事务合同版本_v2;
    L1所有者范围写入幂等身份 组合写入幂等身份;
    std::uint64_t 共同事实代次 = 0;
    bool 是否已确认形成内存权威发布 = false;
    L1所有者范围重试边界 重试边界 =
        L1所有者范围重试边界::修正请求后可重试;
    std::vector<L1三分区原子参与者结果_v2> 参与者结果组;
    friend bool operator==(const L1三分区原子事务结果_v2&,
        const L1三分区原子事务结果_v2&) = default;
};

struct L1所有者范围节点事实 final {
    稳定编码 编码;
    节点种类 种类 = 节点种类::普通;
    std::optional<L1所有者范围值表示种类> 属性类型表示;
    L1结构所有者身份 写入所有者;
    std::uint64_t 创建事实代次 = 0;
    std::optional<std::uint64_t> 退出事实代次;
    std::vector<L1所有者范围属性槽> 当前属性;
    friend bool operator==(const L1所有者范围节点事实&,
        const L1所有者范围节点事实&) = default;
};

struct L1所有者范围关系事实 final {
    稳定编码 编码;
    稳定编码 源节点;
    稳定编码 目标节点;
    稳定编码 关系类型节点;
    std::int64_t 角色或顺序 = 0;
    L1结构所有者身份 写入所有者;
    std::uint64_t 创建事实代次 = 0;
    std::optional<std::uint64_t> 退出事实代次;
    friend bool operator==(const L1所有者范围关系事实&,
        const L1所有者范围关系事实&) = default;
};

struct L1所有者范围值事实 final {
    稳定编码 编码;
    稳定编码 所属节点;
    稳定编码 属性类型节点;
    L1所有者范围原始值材料 材料;
    稳定编码 来源节点;
    L1结构所有者身份 写入所有者;
    std::uint64_t 创建事实代次 = 0;
    std::optional<std::uint64_t> 退出事实代次;
    friend bool operator==(const L1所有者范围值事实&,
        const L1所有者范围值事实&) = default;
};

using L1所有者范围事实副本 = std::variant<L1所有者范围节点事实,
    L1所有者范围关系事实, L1所有者范围值事实>;

enum class L1所有者范围物理清理事实种类 : std::uint8_t {
    节点 = 1, 关系 = 2, 值 = 3
};

struct L1所有者范围物理清理墓碑 final {
    稳定编码 编码;
    L1所有者范围物理清理事实种类 事实种类 =
        L1所有者范围物理清理事实种类::节点;
    稳定编码 内部结构分区;
    std::uint64_t 创建事实代次 = 0;
    std::uint64_t 退出事实代次 = 0;
    std::uint64_t 物理清理事实代次 = 0;
    friend bool operator==(const L1所有者范围物理清理墓碑&,
        const L1所有者范围物理清理墓碑&) = default;
};

enum class L1所有者范围读取状态 : std::uint8_t {
    成功 = 1, 入口拒绝 = 2, 许可拒绝 = 3, 未找到 = 4,
    已退出 = 5, 属性未设置 = 6, 事实代次漂移 = 7,
    资源失败 = 8, 内部不一致 = 9, 历史材料已清理 = 10
};

enum class L1所有者范围来源当前值组读取状态 : std::uint8_t {
    成功 = 1,
    入口拒绝 = 2,
    未找到 = 3,
    已退出 = 4,
    事实代次漂移 = 5,
    数量预算不足 = 6,
    资源失败 = 7,
    内部不一致 = 8
};

struct L1所有者范围来源当前值组读取请求 final {
    std::uint32_t 合同版本 = L1所有者范围来源当前值组读取合同版本;
    L1结构所有者身份 所有者;
    稳定编码 来源节点;
    std::uint64_t 期望事实代次 = 0;
    std::uint64_t 最大数量 = 0;
    friend bool operator==(const L1所有者范围来源当前值组读取请求&,
        const L1所有者范围来源当前值组读取请求&) = default;
};

struct L1所有者范围来源当前值组读取结果 final {
    L1所有者范围来源当前值组读取状态 状态 =
        L1所有者范围来源当前值组读取状态::入口拒绝;
    std::uint32_t 合同版本 = L1所有者范围来源当前值组读取合同版本;
    L1结构所有者身份 所有者;
    稳定编码 来源节点;
    std::uint64_t 期望事实代次 = 0;
    std::uint64_t 读取事实代次 = 0;
    std::vector<L1所有者范围值事实> 当前值;
    friend bool operator==(const L1所有者范围来源当前值组读取结果&,
        const L1所有者范围来源当前值组读取结果&) = default;
};

enum class L1所有者范围属性类型当前值组读取状态 : std::uint8_t {
    成功 = 1,
    入口拒绝 = 2,
    未找到 = 3,
    已退出 = 4,
    事实代次漂移 = 5,
    数量预算不足 = 6,
    资源失败 = 7,
    内部不一致 = 8
};

struct L1所有者范围属性类型当前值组读取请求 final {
    std::uint32_t 合同版本 = L1所有者范围属性类型当前值组读取合同版本;
    L1结构所有者身份 所有者;
    稳定编码 属性类型节点;
    std::uint64_t 期望事实代次 = 0;
    std::uint64_t 最大数量 = 0;
    friend bool operator==(
        const L1所有者范围属性类型当前值组读取请求&,
        const L1所有者范围属性类型当前值组读取请求&) = default;
};

struct L1所有者范围属性类型当前值组读取结果 final {
    L1所有者范围属性类型当前值组读取状态 状态 =
        L1所有者范围属性类型当前值组读取状态::入口拒绝;
    std::uint32_t 合同版本 = L1所有者范围属性类型当前值组读取合同版本;
    L1结构所有者身份 所有者;
    稳定编码 属性类型节点;
    std::uint64_t 期望事实代次 = 0;
    std::uint64_t 读取事实代次 = 0;
    std::vector<L1所有者范围值事实> 当前值;
    friend bool operator==(
        const L1所有者范围属性类型当前值组读取结果&,
        const L1所有者范围属性类型当前值组读取结果&) = default;
};

struct L1所有者范围首次写入读取请求 final {
    std::uint32_t 合同版本 = L1所有者范围首次写入读取合同版本;
    L1所有者范围写入幂等身份 写入幂等身份;
    friend bool operator==(const L1所有者范围首次写入读取请求&,
        const L1所有者范围首次写入读取请求&) = default;
};

struct L1所有者范围首次写入读取结果 final {
    L1所有者范围读取状态 状态 = L1所有者范围读取状态::入口拒绝;
    std::uint32_t 合同版本 = L1所有者范围首次写入读取合同版本;
    L1结构所有者身份 所有者;
    L1所有者范围写入幂等身份 写入幂等身份;
    std::uint64_t 读取事实代次 = 0;
    std::optional<L1所有者范围写集请求> 首次规范化写集;
    std::optional<L1所有者范围写入结果> 首次写入结果;
    friend bool operator==(const L1所有者范围首次写入读取结果&,
        const L1所有者范围首次写入读取结果&) = default;
};

struct L1所有者范围事实读取请求 final {
    std::uint32_t 合同版本 = L1所有者范围CRUD合同版本;
    稳定编码 编码;
    friend bool operator==(const L1所有者范围事实读取请求&,
        const L1所有者范围事实读取请求&) = default;
};

struct L1所有者范围当前读取结果 final {
    L1所有者范围读取状态 状态 = L1所有者范围读取状态::入口拒绝;
    std::uint32_t 合同版本 = L1所有者范围CRUD合同版本;
    稳定编码 查询编码;
    std::uint64_t 读取事实代次 = 0;
    std::optional<L1所有者范围事实副本> 事实;
    std::optional<std::uint64_t> 物理清理事实代次;
    std::optional<L1所有者范围物理清理墓碑> 物理清理墓碑;
    friend bool operator==(const L1所有者范围当前读取结果&,
        const L1所有者范围当前读取结果&) = default;
};

struct L1所有者范围历史读取结果 final {
    L1所有者范围读取状态 状态 = L1所有者范围读取状态::入口拒绝;
    std::uint32_t 合同版本 = L1所有者范围CRUD合同版本;
    稳定编码 查询编码;
    std::uint64_t 读取事实代次 = 0;
    std::optional<L1所有者范围事实副本> 事实;
    std::optional<std::uint64_t> 物理清理事实代次;
    std::optional<L1所有者范围物理清理墓碑> 物理清理墓碑;
    friend bool operator==(const L1所有者范围历史读取结果&,
        const L1所有者范围历史读取结果&) = default;
};

struct L1所有者范围源关系组读取请求 final {
    std::uint32_t 合同版本 = L1所有者范围CRUD合同版本;
    稳定编码 源节点;
    稳定编码 关系类型节点;
    friend bool operator==(const L1所有者范围源关系组读取请求&,
        const L1所有者范围源关系组读取请求&) = default;
};

struct L1所有者范围目标关系组读取请求 final {
    std::uint32_t 合同版本 = L1所有者范围CRUD合同版本;
    稳定编码 目标节点;
    稳定编码 关系类型节点;
    friend bool operator==(const L1所有者范围目标关系组读取请求&,
        const L1所有者范围目标关系组读取请求&) = default;
};

struct L1所有者范围源关系组读取结果 final {
    L1所有者范围读取状态 状态 = L1所有者范围读取状态::入口拒绝;
    std::uint32_t 合同版本 = L1所有者范围CRUD合同版本;
    稳定编码 源节点;
    稳定编码 关系类型节点;
    std::uint64_t 读取事实代次 = 0;
    std::vector<L1所有者范围关系事实> 关系组;
    friend bool operator==(const L1所有者范围源关系组读取结果&,
        const L1所有者范围源关系组读取结果&) = default;
};

struct L1所有者范围目标关系组读取结果 final {
    L1所有者范围读取状态 状态 = L1所有者范围读取状态::入口拒绝;
    std::uint32_t 合同版本 = L1所有者范围CRUD合同版本;
    稳定编码 目标节点;
    稳定编码 关系类型节点;
    std::uint64_t 读取事实代次 = 0;
    std::vector<L1所有者范围关系事实> 关系组;
    friend bool operator==(const L1所有者范围目标关系组读取结果&,
        const L1所有者范围目标关系组读取结果&) = default;
};

enum class L1所有者范围关系端点方向 : std::uint8_t { 源 = 1, 目标 = 2 };

struct L1所有者范围历史关系组读取请求 final {
    std::uint32_t 合同版本 = L1所有者范围CRUD合同版本;
    L1所有者范围关系端点方向 方向 = L1所有者范围关系端点方向::源;
    稳定编码 端点节点;
    稳定编码 关系类型节点;
    std::uint64_t 历史截止事实代次 = 0;
    friend bool operator==(const L1所有者范围历史关系组读取请求&,
        const L1所有者范围历史关系组读取请求&) = default;
};

struct L1所有者范围历史关系组读取结果 final {
    L1所有者范围读取状态 状态 = L1所有者范围读取状态::入口拒绝;
    std::uint32_t 合同版本 = L1所有者范围CRUD合同版本;
    L1所有者范围关系端点方向 方向 = L1所有者范围关系端点方向::源;
    稳定编码 端点节点;
    稳定编码 关系类型节点;
    std::uint64_t 历史截止事实代次 = 0;
    std::uint64_t 读取事实代次 = 0;
    std::vector<L1所有者范围关系事实> 关系组;
    friend bool operator==(const L1所有者范围历史关系组读取结果&,
        const L1所有者范围历史关系组读取结果&) = default;
};

struct L1所有者范围历史属性值组读取请求 final {
    std::uint32_t 合同版本 = L1所有者范围CRUD合同版本;
    稳定编码 所属节点;
    std::uint64_t 历史截止事实代次 = 0;
    friend bool operator==(const L1所有者范围历史属性值组读取请求&,
        const L1所有者范围历史属性值组读取请求&) = default;
};

struct L1所有者范围历史属性值组读取结果 final {
    L1所有者范围读取状态 状态 = L1所有者范围读取状态::入口拒绝;
    std::uint32_t 合同版本 = L1所有者范围CRUD合同版本;
    稳定编码 所属节点;
    std::uint64_t 历史截止事实代次 = 0;
    std::uint64_t 读取事实代次 = 0;
    std::vector<L1所有者范围值事实> 属性值组;
    friend bool operator==(const L1所有者范围历史属性值组读取结果&,
        const L1所有者范围历史属性值组读取结果&) = default;
};

struct L1结构所有者当前读取结果 final {
    L1所有者范围读取状态 状态 = L1所有者范围读取状态::入口拒绝;
    std::uint32_t 合同版本 = L1所有者范围CRUD合同版本;
    L1结构所有者身份 查询所有者;
    std::uint64_t 读取事实代次 = 0;
    std::optional<L1结构所有者事实> 所有者事实;
    friend bool operator==(const L1结构所有者当前读取结果&,
        const L1结构所有者当前读取结果&) = default;
};

struct L1结构所有者历史读取结果 final {
    L1所有者范围读取状态 状态 = L1所有者范围读取状态::入口拒绝;
    std::uint32_t 合同版本 = L1所有者范围CRUD合同版本;
    L1结构所有者身份 查询所有者;
    std::uint64_t 读取事实代次 = 0;
    std::optional<L1结构所有者事实> 所有者事实;
    friend bool operator==(const L1结构所有者历史读取结果&,
        const L1结构所有者历史读取结果&) = default;
};

struct L1所有者范围一致属性值选择项 final {
    稳定编码 节点;
    稳定编码 属性类型;
    friend bool operator==(const L1所有者范围一致属性值选择项&,
        const L1所有者范围一致属性值选择项&) = default;
};

struct L1所有者范围一致源关系组选择项 final {
    稳定编码 源节点;
    稳定编码 关系类型节点;
    friend bool operator==(const L1所有者范围一致源关系组选择项&,
        const L1所有者范围一致源关系组选择项&) = default;
};

struct L1所有者范围一致目标关系组选择项 final {
    稳定编码 目标节点;
    稳定编码 关系类型节点;
    friend bool operator==(const L1所有者范围一致目标关系组选择项&,
        const L1所有者范围一致目标关系组选择项&) = default;
};

enum class L1所有者范围一致当前读取状态 : std::uint8_t {
    成功 = 1, 入口拒绝 = 2, 许可拒绝 = 3,
    事实代次漂移 = 4, 资源失败 = 5, 内部不一致 = 6
};

enum class L1所有者范围一致当前读取项目状态 : std::uint8_t {
    成功 = 1, 未找到 = 2, 已退出 = 3, 属性未设置 = 4,
    种类不匹配 = 5, 历史材料已清理 = 6
};

struct L1所有者范围一致当前读取请求 final {
    std::uint32_t 合同版本 = L1所有者范围一致当前读取合同版本;
    std::uint64_t 期望事实代次 = 0;
    std::vector<L1结构所有者身份> 所有者;
    std::vector<稳定编码> 节点;
    std::vector<稳定编码> 关系;
    std::vector<稳定编码> 值;
    std::vector<L1所有者范围一致属性值选择项> 属性值;
    std::vector<L1所有者范围一致源关系组选择项> 源关系组;
    std::vector<L1所有者范围一致目标关系组选择项> 目标关系组;
    friend bool operator==(const L1所有者范围一致当前读取请求&,
        const L1所有者范围一致当前读取请求&) = default;
};

template<class 事实类型>
struct L1所有者范围一致具名事实读取结果项 final {
    稳定编码 查询编码;
    L1所有者范围一致当前读取项目状态 状态 =
        L1所有者范围一致当前读取项目状态::未找到;
    std::optional<事实类型> 事实;
    std::optional<std::uint64_t> 物理清理事实代次;
    std::optional<L1所有者范围物理清理墓碑> 物理清理墓碑;
    friend bool operator==(const L1所有者范围一致具名事实读取结果项&,
        const L1所有者范围一致具名事实读取结果项&) = default;
};

using L1所有者范围一致节点读取结果项 =
    L1所有者范围一致具名事实读取结果项<L1所有者范围节点事实>;
using L1所有者范围一致关系读取结果项 =
    L1所有者范围一致具名事实读取结果项<L1所有者范围关系事实>;
using L1所有者范围一致值读取结果项 =
    L1所有者范围一致具名事实读取结果项<L1所有者范围值事实>;

struct L1所有者范围一致所有者读取结果项 final {
    L1结构所有者身份 查询所有者;
    L1所有者范围一致当前读取项目状态 状态 =
        L1所有者范围一致当前读取项目状态::未找到;
    std::optional<L1结构所有者事实> 所有者事实;
    friend bool operator==(const L1所有者范围一致所有者读取结果项&,
        const L1所有者范围一致所有者读取结果项&) = default;
};

struct L1所有者范围一致属性值投影 final {
    L1所有者范围属性槽 属性槽;
    L1所有者范围值事实 当前值事实;
    friend bool operator==(const L1所有者范围一致属性值投影&,
        const L1所有者范围一致属性值投影&) = default;
};

struct L1所有者范围一致属性值读取结果项 final {
    稳定编码 节点;
    稳定编码 属性类型;
    L1所有者范围一致当前读取项目状态 状态 =
        L1所有者范围一致当前读取项目状态::未找到;
    std::optional<L1所有者范围一致属性值投影> 投影;
    friend bool operator==(const L1所有者范围一致属性值读取结果项&,
        const L1所有者范围一致属性值读取结果项&) = default;
};

struct L1所有者范围一致关系对端投影 final {
    L1所有者范围关系事实 关系;
    L1所有者范围节点事实 对端节点;
    friend bool operator==(const L1所有者范围一致关系对端投影&,
        const L1所有者范围一致关系对端投影&) = default;
};

struct L1所有者范围一致源关系组读取结果项 final {
    稳定编码 源节点;
    稳定编码 关系类型节点;
    std::vector<L1所有者范围一致关系对端投影> 成员;
    friend bool operator==(const L1所有者范围一致源关系组读取结果项&,
        const L1所有者范围一致源关系组读取结果项&) = default;
};

struct L1所有者范围一致目标关系组读取结果项 final {
    稳定编码 目标节点;
    稳定编码 关系类型节点;
    std::vector<L1所有者范围一致关系对端投影> 成员;
    friend bool operator==(const L1所有者范围一致目标关系组读取结果项&,
        const L1所有者范围一致目标关系组读取结果项&) = default;
};

struct L1所有者范围一致当前读取结果 final {
    L1所有者范围一致当前读取状态 状态 =
        L1所有者范围一致当前读取状态::入口拒绝;
    std::uint32_t 合同版本 = L1所有者范围一致当前读取合同版本;
    std::uint64_t 期望事实代次 = 0;
    std::uint64_t 读取事实代次 = 0;
    std::vector<L1所有者范围一致所有者读取结果项> 所有者;
    std::vector<L1所有者范围一致节点读取结果项> 节点;
    std::vector<L1所有者范围一致关系读取结果项> 关系;
    std::vector<L1所有者范围一致值读取结果项> 值;
    std::vector<L1所有者范围一致属性值读取结果项> 属性值;
    std::vector<L1所有者范围一致源关系组读取结果项> 源关系组;
    std::vector<L1所有者范围一致目标关系组读取结果项> 目标关系组;
    friend bool operator==(const L1所有者范围一致当前读取结果&,
        const L1所有者范围一致当前读取结果&) = default;
};

struct L1所有者范围一致关系类型闭包选择项 final {
    稳定编码 入口关系类型节点;
    std::vector<稳定编码> 源节点属性类型;
    std::vector<稳定编码> 源节点源关系类型;
    std::vector<稳定编码> 源节点目标关系类型;
    std::vector<稳定编码> 目标节点属性类型;
    std::vector<稳定编码> 目标节点源关系类型;
    std::vector<稳定编码> 目标节点目标关系类型;
    friend bool operator==(const L1所有者范围一致关系类型闭包选择项&,
        const L1所有者范围一致关系类型闭包选择项&) = default;
};

struct L1所有者范围一致闭包端点关系组读取结果项 final {
    稳定编码 端点节点;
    稳定编码 关系类型节点;
    L1所有者范围一致当前读取项目状态 状态 =
        L1所有者范围一致当前读取项目状态::未找到;
    std::optional<L1所有者范围节点事实> 关系类型事实;
    std::vector<L1所有者范围一致关系对端投影> 成员;
    friend bool operator==(const L1所有者范围一致闭包端点关系组读取结果项&,
        const L1所有者范围一致闭包端点关系组读取结果项&) = default;
};

struct L1所有者范围一致关系类型闭包成员 final {
    L1所有者范围关系事实 关系;
    L1所有者范围节点事实 源节点;
    L1所有者范围节点事实 目标节点;
    std::vector<L1所有者范围一致属性值读取结果项> 源节点属性值;
    std::vector<L1所有者范围一致闭包端点关系组读取结果项>
        源节点源关系组;
    std::vector<L1所有者范围一致闭包端点关系组读取结果项>
        源节点目标关系组;
    std::vector<L1所有者范围一致属性值读取结果项> 目标节点属性值;
    std::vector<L1所有者范围一致闭包端点关系组读取结果项>
        目标节点源关系组;
    std::vector<L1所有者范围一致闭包端点关系组读取结果项>
        目标节点目标关系组;
    friend bool operator==(const L1所有者范围一致关系类型闭包成员&,
        const L1所有者范围一致关系类型闭包成员&) = default;
};

struct L1所有者范围一致关系类型闭包读取结果项 final {
    稳定编码 入口关系类型节点;
    L1所有者范围一致当前读取项目状态 状态 =
        L1所有者范围一致当前读取项目状态::未找到;
    std::optional<L1所有者范围节点事实> 关系类型事实;
    std::vector<L1所有者范围一致关系类型闭包成员> 成员;
    friend bool operator==(const L1所有者范围一致关系类型闭包读取结果项&,
        const L1所有者范围一致关系类型闭包读取结果项&) = default;
};

struct L1所有者范围一致关系类型闭包读取请求 final {
    std::uint32_t 合同版本 =
        L1所有者范围一致关系类型闭包读取合同版本;
    std::uint64_t 期望事实代次 = 0;
    std::vector<L1结构所有者身份> 所有者;
    std::vector<稳定编码> 节点;
    std::vector<稳定编码> 关系;
    std::vector<稳定编码> 值;
    std::vector<L1所有者范围一致属性值选择项> 属性值;
    std::vector<L1所有者范围一致源关系组选择项> 源关系组;
    std::vector<L1所有者范围一致目标关系组选择项> 目标关系组;
    std::vector<L1所有者范围一致关系类型闭包选择项> 关系类型闭包;
    friend bool operator==(const L1所有者范围一致关系类型闭包读取请求&,
        const L1所有者范围一致关系类型闭包读取请求&) = default;
};

struct L1所有者范围一致关系类型闭包读取结果 final {
    L1所有者范围一致当前读取状态 状态 =
        L1所有者范围一致当前读取状态::入口拒绝;
    std::uint32_t 合同版本 =
        L1所有者范围一致关系类型闭包读取合同版本;
    std::uint64_t 期望事实代次 = 0;
    std::uint64_t 读取事实代次 = 0;
    std::vector<L1所有者范围一致所有者读取结果项> 所有者;
    std::vector<L1所有者范围一致节点读取结果项> 节点;
    std::vector<L1所有者范围一致关系读取结果项> 关系;
    std::vector<L1所有者范围一致值读取结果项> 值;
    std::vector<L1所有者范围一致属性值读取结果项> 属性值;
    std::vector<L1所有者范围一致源关系组读取结果项> 源关系组;
    std::vector<L1所有者范围一致目标关系组读取结果项> 目标关系组;
    std::vector<L1所有者范围一致关系类型闭包读取结果项> 关系类型闭包;
    friend bool operator==(const L1所有者范围一致关系类型闭包读取结果&,
        const L1所有者范围一致关系类型闭包读取结果&) = default;
};

inline bool 有效(L1结构所有者身份 身份) noexcept { return 有效(身份.编码); }
inline bool 有效(L1所有者范围建立幂等身份 身份) noexcept { return 身份.值 != 0; }
inline bool 有效(L1所有者范围写入幂等身份 身份) noexcept { return 身份.值 != 0; }
inline bool 有效(L1所有者范围写集本地键 键) noexcept { return 键.值 != 0; }

inline bool L1所有者范围原始材料完整(
    const L1所有者范围原始值材料& 材料) noexcept {
    return std::visit([](const auto& 值) noexcept {
        using 类型 = std::decay_t<decltype(值)>;
        if constexpr (std::is_same_v<类型, std::int64_t>) return true;
        else if constexpr (std::is_same_v<类型, L1所有者范围独立材料引用>)
            return 有效(值.编码);
        else return !值.empty();
    }, 材料);
}

} // namespace 海中鱼巣
