#pragma once

#include <cstdint>
#include <optional>
#include <span>
#include <variant>
#include <vector>

#include "../核心/服务.L1事实基座.h"

namespace 海中鱼巣 {

#define 海中鱼巣_概念强类型(名称)                                                \
struct 名称 final {                                                             \
    稳定编码 值{};                                                              \
    名称() = default;                                                           \
    explicit 名称(稳定编码 编码) : 值(编码) {}                                  \
    friend bool operator==(const 名称&, const 名称&) = default;                  \
}

海中鱼巣_概念强类型(概念树概念身份);
海中鱼巣_概念强类型(概念树存在引用);
海中鱼巣_概念强类型(概念树特征类型引用);
海中鱼巣_概念强类型(概念树特征引用);
海中鱼巣_概念强类型(概念树场景引用);
海中鱼巣_概念强类型(概念树规则身份);

#undef 海中鱼巣_概念强类型

using 概念树世界引用 = std::variant<概念树存在引用, 概念树特征引用>;
using 概念树形成世界引用 =
    std::variant<概念树存在引用, 概念树特征引用,
                 概念树特征类型引用, 概念树场景引用>;
using 概念树精确值 =
    std::variant<std::int64_t, std::vector<std::int64_t>,
                 std::vector<std::uint64_t>>;

struct 概念树I64区间 final {
    std::int64_t 下界 = 0;
    std::int64_t 上界 = 0;
    friend bool operator==(const 概念树I64区间&,
                           const 概念树I64区间&) = default;
};
using 概念树特征值域 = std::variant<概念树精确值, 概念树I64区间>;

struct 概念树特征定义 final {
    概念树特征类型引用 特征类型;
    概念树特征值域 值域;
    friend bool operator==(const 概念树特征定义&,
                           const 概念树特征定义&) = default;
};
struct 概念树存在定义 final {
    std::vector<概念树概念身份> 特征模板组;
    std::vector<概念树概念身份> 已知子存在概念组;
    friend bool operator==(const 概念树存在定义&,
                           const 概念树存在定义&) = default;
};
using 概念树定义 = std::variant<概念树特征定义, 概念树存在定义>;

enum class 相关概念类别 : std::uint8_t { 存在 = 1, 特征 = 2, 动态 = 3 };
enum class 概念树生命周期状态 : std::uint8_t { 活跃 = 1, 冷却 = 2, 退役 = 3 };

// 生命周期仍是逻辑组成；这里不保存已经退出的L1技术时钟。
struct 概念树生命周期 final {
    friend bool operator==(const 概念树生命周期&,
                           const 概念树生命周期&) = default;
};
struct 概念树直接上位事实 final {
    稳定编码 关系{};
    概念树概念身份 上位, 下位;
    friend bool operator==(const 概念树直接上位事实&,
                           const 概念树直接上位事实&) = default;
};
struct 概念树形成引用事实 final {
    稳定编码 关系{}, 记录{};
    概念树概念身份 所属概念;
    概念树形成世界引用 世界引用;
    friend bool operator==(const 概念树形成引用事实&,
                           const 概念树形成引用事实&) = default;
};

enum class 通用存在定义规则 : std::uint8_t { 未指定 = 0, 不预设特征 = 1 };
struct 通用存在概念定义 final {
    通用存在定义规则 规则 = 通用存在定义规则::未指定;
    friend bool operator==(const 通用存在概念定义&,
                           const 通用存在概念定义&) = default;
};
struct 纯I64特征概念定义 final {
    概念树特征类型引用 特征类型;
    std::vector<概念树I64区间> 规范域;
    friend bool operator==(const 纯I64特征概念定义&,
                           const 纯I64特征概念定义&) = default;
};
struct 纯合取存在概念定义 final {
    std::vector<概念树概念身份> 特征模板组;
    friend bool operator==(const 纯合取存在概念定义&,
                           const 纯合取存在概念定义&) = default;
};
using 纯概念定义 = std::variant<纯I64特征概念定义,
                                纯合取存在概念定义,
                                通用存在概念定义>;

struct 概念树写入头 final {
    L1所有者范围写入幂等身份 幂等身份{};
    friend bool operator==(const 概念树写入头&,
                           const 概念树写入头&) = default;
};
enum class 概念初始组织指定 : std::uint8_t {
    未指定 = 0, 显式顶层 = 1, 具名上位 = 2
};
enum class 纯概念状态 : std::uint8_t {
    已读取 = 1, 已创建 = 2, 精确重复 = 3, 已删除 = 4, 未找到 = 5,
    入口拒绝 = 6, 类别冲突 = 7, 定义不支持 = 8, 定义不相容 = 9,
    组织冲突 = 10, 上位成环 = 11, 概念已退役 = 12,
    引用冲突 = 14, 幂等冲突 = 16, 资源失败 = 19, 内部不一致 = 20,
    已可能发布 = 21, 旧格式不支持 = 22, 已迁移生命周期 = 23,
    未实现 = 24
};
enum class 纯概念发布状态 : std::uint8_t {
    未进入 = 0, 确认未发布 = 1, 确认发布 = 2, 可能发布 = 3
};
enum class 纯概念定义关系种类 : std::uint8_t {
    未指定 = 0, 定义成员 = 1, 定义特征类型 = 2, 定义模板 = 3
};
struct 纯概念定义关系事实 final {
    稳定编码 关系{}, 源{}, 目标{}, 关系类型{};
    纯概念定义关系种类 种类 = 纯概念定义关系种类::未指定;
    std::uint64_t 顺序 = 0;
    friend bool operator==(const 纯概念定义关系事实&,
                           const 纯概念定义关系事实&) = default;
};
struct 纯概念事实 final {
    概念树概念身份 概念;
    相关概念类别 类别 = 相关概念类别::存在;
    纯概念定义 定义;
    稳定编码 定义记录{};
    std::vector<纯概念定义关系事实> 定义关系组;
    std::vector<概念树直接上位事实> 直接上位;
    概念树生命周期状态 治理状态 = 概念树生命周期状态::活跃;
    稳定编码 生命周期值事实{};
    friend bool operator==(const 纯概念事实&, const 纯概念事实&) = default;
};

struct 纯概念读取请求 final {
    概念树概念身份 概念;
    friend bool operator==(const 纯概念读取请求&,
                           const 纯概念读取请求&) = default;
};
struct 纯概念查询请求 final {
    纯概念定义 定义;
    friend bool operator==(const 纯概念查询请求&,
                           const 纯概念查询请求&) = default;
};
struct 纯概念创建请求 final {
    L1所有者范围写入幂等身份 幂等身份;
    纯概念定义 定义;
    概念初始组织指定 组织 = 概念初始组织指定::未指定;
    std::vector<概念树概念身份> 直接上位;
    friend bool operator==(const 纯概念创建请求&,
                           const 纯概念创建请求&) = default;
};
struct 纯概念读取结果 final {
    纯概念状态 状态 = 纯概念状态::入口拒绝;
    std::optional<纯概念事实> 事实;
    bool 成功(const 纯概念读取请求&) const noexcept;
};
struct 纯概念查询结果 final {
    纯概念状态 状态 = 纯概念状态::入口拒绝;
    std::optional<纯概念事实> 事实;
    bool 成功(const 纯概念查询请求&) const noexcept;
    bool 确认未找到(const 纯概念查询请求&) const noexcept;
};
struct 纯概念写入结果 final {
    纯概念状态 状态 = 纯概念状态::入口拒绝;
    纯概念发布状态 发布 = 纯概念发布状态::未进入;
    std::optional<纯概念创建请求> 原请求;
    std::optional<纯概念事实> 事实;
    bool 成功(const 纯概念创建请求&) const noexcept;
};

struct 纯概念结构类型 final {
    稳定编码 类型登记{}, 概念族成员{}, 格式版本{}, 概念类别{};
    稳定编码 定义成员{}, 定义种类{}, 定义特征类型{}, 定义模板{};
    稳定编码 I64域{}, 通用规则{}, 直接上位{}, 生命周期{};
    稳定编码 存在概念使用{};
};
struct 纯概念结构交付 final {
    稳定编码 格式锚点{}, 概念族锚点{};
    纯概念结构类型 类型;
};
struct 纯概念结构登记请求 final {
    L1所有者范围写入幂等身份 幂等身份;
};
struct 纯概念结构登记结果 final {
    纯概念状态 状态 = 纯概念状态::入口拒绝;
    纯概念发布状态 发布 = 纯概念发布状态::未进入;
    std::optional<纯概念结构登记请求> 原请求;
    std::optional<纯概念结构交付> 交付;
    bool 成功(const 纯概念结构登记请求&) const noexcept;
};

struct 存在概念特征值域项 final {
    稳定编码 FT{};
    概念树概念身份 FC;
    friend bool operator==(const 存在概念特征值域项&,
                           const 存在概念特征值域项&) = default;
};
struct 存在概念两组定义 final {
    bool 自身特征组已完整声明 = false;
    std::vector<存在概念特征值域项> 自身特征值域组;
    bool 子存在概念组已完整声明 = false;
    std::vector<概念树概念身份> 已知子存在概念组;
    friend bool operator==(const 存在概念两组定义&,
                           const 存在概念两组定义&) = default;
};
enum class 存在概念两组状态 : std::uint8_t {
    入口拒绝 = 1, 已规范化 = 2, 已枚举 = 3, 已读取 = 4,
    未找到 = 5, 已创建 = 6, 精确重复 = 7, 概念已退役 = 9,
    类别冲突 = 10, 定义不相容 = 11, 规则缺失 = 12,
    幂等冲突 = 14, 资源失败 = 17, 内部不一致 = 18,
    已可能发布 = 19, 旧格式不支持 = 20, 未实现 = 21
};
struct 存在概念两组结构类型 final {
    稳定编码 两组定义成员{}, 自身特征值域项{}, 自身项特征类型{};
    稳定编码 自身项值域概念{}, 已知子存在概念{};
};
struct 存在概念两组结构交付 final {
    稳定编码 格式锚点{};
    存在概念两组结构类型 类型;
};
struct 存在概念两组结构登记请求 final {
    L1所有者范围写入幂等身份 幂等身份;
    纯概念结构交付 纯概念结构;
};
struct 存在概念两组结构登记结果 final {
    存在概念两组状态 状态 = 存在概念两组状态::入口拒绝;
    纯概念发布状态 发布 = 纯概念发布状态::未进入;
    std::optional<存在概念两组结构登记请求> 原请求;
    std::optional<存在概念两组结构交付> 交付;
    bool 成功(const 存在概念两组结构登记请求&) const noexcept;
};
struct 存在概念两组定义关系事实 final {
    稳定编码 关系{}, 源{}, 目标{}, 关系类型{};
    std::uint64_t 顺序 = 0;
};
struct 存在概念两组事实 final {
    概念树概念身份 概念;
    概念树生命周期状态 治理状态 = 概念树生命周期状态::活跃;
    存在概念两组定义 定义;
    std::vector<存在概念两组定义关系事实> 自身特征项关系组;
    std::vector<存在概念两组定义关系事实> 子概念关系组;
    std::vector<概念树直接上位事实> 直接上位;
};
struct 存在概念两组规范化请求 final { 存在概念两组定义 定义; };
struct 存在概念两组枚举请求 final {};
struct 存在概念两组读取请求 final { 概念树概念身份 EC; };
struct 存在概念两组查询请求 final { 存在概念两组定义 定义; };
struct 存在概念两组创建请求 final {
    L1所有者范围写入幂等身份 幂等身份;
    存在概念两组定义 定义;
    std::vector<概念树概念身份> 直接上位;
};
struct 存在概念两组规范化结果 final {
    存在概念两组状态 状态 = 存在概念两组状态::入口拒绝;
    std::optional<存在概念两组定义> 定义;
    bool 成功(const 存在概念两组规范化请求&) const noexcept;
};
struct 存在概念两组枚举结果 final {
    存在概念两组状态 状态 = 存在概念两组状态::入口拒绝;
    std::vector<存在概念两组事实> 候选;
    bool 成功(const 存在概念两组枚举请求&) const noexcept;
};
struct 存在概念两组读取结果 final {
    存在概念两组状态 状态 = 存在概念两组状态::入口拒绝;
    std::optional<存在概念两组事实> 事实;
    bool 成功(const 存在概念两组读取请求&) const noexcept;
};
struct 存在概念两组查询结果 final {
    存在概念两组状态 状态 = 存在概念两组状态::入口拒绝;
    std::optional<存在概念两组事实> 事实;
    bool 成功(const 存在概念两组查询请求&) const noexcept;
    bool 确认未找到(const 存在概念两组查询请求&) const noexcept;
};
struct 存在概念两组写入结果 final {
    存在概念两组状态 状态 = 存在概念两组状态::入口拒绝;
    纯概念发布状态 发布 = 纯概念发布状态::未进入;
    std::optional<存在概念两组创建请求> 原请求;
    std::optional<存在概念两组事实> 事实;
    bool 成功(const 存在概念两组创建请求&) const noexcept;
};

struct 存在概念使用事实 final {
    稳定编码 关系{}, E{};
    概念树概念身份 EC;
};
struct 存在概念使用读取请求 final { 稳定编码 E{}; };
struct 存在概念使用读取结果 final {
    纯概念状态 状态 = 纯概念状态::入口拒绝;
    std::optional<存在概念使用事实> 事实;
    bool 成功(const 存在概念使用读取请求&) const noexcept;
};
struct 存在概念使用完整读取结果 final {
    纯概念状态 状态 = 纯概念状态::入口拒绝;
    std::optional<存在概念使用事实> 使用;
    std::optional<纯概念事实> 概念;
    bool 成功(const 存在概念使用读取请求&) const noexcept;
};

enum class 已发布概念引用参与状态 : std::uint8_t {
    已准备 = 1, 精确重复 = 2, 入口拒绝 = 3, 概念未找到 = 4,
    概念已退役 = 5, 类别冲突 = 7, 定义不相容 = 8, 引用冲突 = 9,
    幂等冲突 = 11, 资源失败 = 14, 内部不一致 = 15,
    已可能发布 = 16, 旧格式不支持 = 17, 已读取 = 18,
    未派发 = 19, 未实现 = 20
};
struct 已发布存在概念引用准备请求 final {
    L1所有者范围写入幂等身份 幂等身份;
    L1有限N分区原子参与者身份 参与者;
    L1有限N分区原子事实引用 新存在;
    概念树概念身份 EC;
    纯概念定义 预期定义;
};
struct 已发布概念引用片段结果 final {
    已发布概念引用参与状态 状态 = 已发布概念引用参与状态::入口拒绝;
    std::optional<L1有限N分区原子参与者写集> 写集;
};

enum class 相关概念参与状态 : std::uint8_t {
    已准备 = 1, 已读取 = 2, 精确重复 = 3, 入口拒绝 = 4,
    概念未找到 = 5, 类别冲突 = 7, 签名冲突 = 8, 上位成环 = 9,
    挂靠无效 = 10, 幂等冲突 = 12, 资源失败 = 15,
    内部不一致 = 16, 已可能发布 = 17, 旧格式不支持 = 18,
    未实现 = 19
};
struct 相关概念世界挂靠 final { 概念树形成世界引用 世界事实; };
struct 相关概念精确复用方案 final {
    概念树概念身份 概念;
    相关概念类别 类别 = 相关概念类别::存在;
    概念树定义 预期定义;
};
struct 相关概念创建方案 final {
    相关概念类别 类别 = 相关概念类别::存在;
    概念树定义 定义;
    std::vector<概念树概念身份> 直接上位;
};
using 相关概念方案 = std::variant<相关概念精确复用方案, 相关概念创建方案>;
struct 相关概念参与请求 final {
    L1所有者范围写入幂等身份 幂等身份;
    相关概念方案 方案;
    相关概念世界挂靠 挂靠;
};
struct 相关概念参与片段 final {
    相关概念参与状态 状态 = 相关概念参与状态::入口拒绝;
    std::optional<L1有限N分区原子参与者写集> 写集;
};
struct 相关概念完整事实 final {
    概念树概念身份 概念;
    相关概念类别 类别 = 相关概念类别::存在;
    概念树定义 定义;
    std::vector<概念树直接上位事实> 直接上位;
    概念树形成引用事实 挂接;
};
struct 相关概念参与读回 final {
    相关概念参与状态 状态 = 相关概念参与状态::入口拒绝;
    std::optional<相关概念完整事实> 概念;
};
struct 相关概念组合提交请求 final {
    L1所有者范围写入幂等身份 组合幂等身份;
    相关概念参与请求 概念请求;
    std::vector<L1有限N分区原子参与者写集> 前序参与者写集组;
};
struct 相关概念组合提交结果 final {
    相关概念参与状态 概念状态 = 相关概念参与状态::入口拒绝;
    bool 已进入L1 = false;
    L1有限N分区原子事务结果 事务结果;
};
class 相关概念添加参与者 {
public:
    virtual ~相关概念添加参与者() = default;
    virtual bool 绑定于(const L1事实基座服务&) const noexcept = 0;
    virtual 相关概念参与片段 准备相关概念片段(
        const 相关概念参与请求&,
        L1有限N分区原子参与者身份) const noexcept = 0;
    virtual 相关概念组合提交结果 提交相关概念组合事务(
        const 相关概念组合提交请求&,
        std::span<L1所有者范围写端口* const>) noexcept = 0;
    virtual 相关概念参与读回 读取相关概念结果(
        const 相关概念参与请求&) const noexcept = 0;
};

struct 相关概念结构类型 final {
    稳定编码 类型登记{}, 概念族成员{}, 概念类别{}, 定义成员{}, 定义种类{};
    稳定编码 定义特征类型{}, 定义模板{}, 精确I64{}, 精确I64组{};
    稳定编码 精确U64组{}, 区间下界{}, 区间上界{}, 直接上位{};
    稳定编码 形成成员{}, 形成存在{}, 形成特征{}, 形成特征类型{};
    稳定编码 形成场景{}, 生命周期{};
};
struct 相关概念结构交付 final {
    std::uint32_t 格式 = 1;
    稳定编码 格式锚点{}, 概念族锚点{};
    相关概念结构类型 类型;
};

} // namespace 海中鱼巣
