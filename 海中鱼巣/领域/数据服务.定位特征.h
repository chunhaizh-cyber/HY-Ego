#pragma once

#include <cstdint>
#include <mutex>
#include <optional>
#include <vector>

#include "数据服务.特征值类.h"
#include "合同.相关概念添加参与.h"

namespace 海中鱼巣 {

struct 定位特征位置 final {
    稳定编码 场景{}, 组织父{};
    friend bool operator==(const 定位特征位置&, const 定位特征位置&) = default;
};

struct 原子I64特征出生键 final {
    L1所有者范围写入幂等身份 组合{}, 内容{}, 已知{}, 组织{}, 概念使用{};
    friend bool operator==(const 原子I64特征出生键&, const 原子I64特征出生键&) = default;
};

struct 原子I64特征候选查询请求 final {
    定位特征位置 位置;
    稳定编码 正式特征类型{};
    std::int64_t 准确I64 = 0;
    friend bool operator==(const 原子I64特征候选查询请求&,
        const 原子I64特征候选查询请求&) = default;
};

struct 原子I64特征候选 final {
    稳定编码 F{};
    friend bool operator==(const 原子I64特征候选&, const 原子I64特征候选&) = default;
};

enum class 原子I64特征候选查询状态 : std::uint8_t {
    已读取 = 1, 入口拒绝 = 2, 位置冲突 = 3,
    资源失败 = 6, 内部不一致 = 7
};

struct 原子I64特征候选查询结果 final {
    原子I64特征候选查询状态 状态 = 原子I64特征候选查询状态::入口拒绝;
    std::vector<原子I64特征候选> 候选;
    原子I64特征候选查询请求 原请求;
};

struct 特征概念出生使用事实 final {
    稳定编码 关系{};
    稳定编码 F{};
    概念树概念身份 FCv;
    概念树生命周期 生命周期;
    friend bool operator==(const 特征概念出生使用事实&,
        const 特征概念出生使用事实&) = default;
};

struct 原子I64特征出生请求 final {
    定位特征位置 位置;
    稳定编码 正式特征类型{};
    std::int64_t 准确I64 = 0;
    概念树概念身份 FCv;
    原子I64特征出生键 键;
    friend bool operator==(const 原子I64特征出生请求&,
        const 原子I64特征出生请求&) = default;
};

struct 原子I64特征出生事实 final {
    稳定编码 F{}, 正式特征类型{}, FCv{};
    std::int64_t 准确I64 = 0;
    定位特征位置 位置;
    稳定编码 holder{}, 已知关系{}, 组织关系{}, 概念使用关系{};
    friend bool operator==(const 原子I64特征出生事实&,
        const 原子I64特征出生事实&) = default;
};

enum class 原子I64特征出生状态 : std::uint8_t {
    已创建 = 1, 精确重复 = 2, 入口拒绝 = 3, 未找到 = 4,
    位置冲突 = 5, 存量未定位 = 6, 概念不适配 = 7,
    幂等冲突 = 9, 资源失败 = 11, 内部不一致 = 12,
    已可能发布 = 13, 已读取 = 14
};

struct 原子I64特征出生结果 final {
    原子I64特征出生状态 状态 = 原子I64特征出生状态::入口拒绝;
    std::optional<原子I64特征出生事实> 事实;
    原子I64特征出生请求 原请求;
    bool 成功() const noexcept;
};

struct 原子I64特征出生读取请求 final { 稳定编码 F{}; 定位特征位置 位置; };

enum class 原子I64特征窄读取状态 : std::uint8_t {
    已读取 = 1, 入口拒绝 = 2, 未找到 = 3, 位置冲突 = 4,
    资源失败 = 7, 内部不一致 = 8
};

template<class T>
struct 原子I64特征窄读取结果 final {
    原子I64特征窄读取状态 状态 = 原子I64特征窄读取状态::入口拒绝;
    std::optional<T> 事实;
};

struct 原子I64特征内容读取请求 final { 稳定编码 F{}; };
struct 原子I64特征内容事实 final {
    稳定编码 F{}, 正式特征类型{}, 类型关系{}, I64值事实{};
    std::int64_t 准确I64 = 0;
};
struct 原子I64特征holder读取请求 final { 稳定编码 holder{}, F{}; };
struct 原子I64特征holder事实 final { 稳定编码 holder{}, F{}, 已知关系{}, 关系类型{}; };
struct 原子I64特征组织读取请求 final { 稳定编码 C{}, P{}, F{}; };
struct 原子I64特征场景路径边事实 final {
    稳定编码 关系{}, 源{}, 目标{}, 关系类型{};
    std::int64_t 角色 = 0;
};
struct 原子I64特征组织事实 final {
    稳定编码 C{}, P{}, F{}, 组织关系{}, 组织关系类型{};
    std::vector<原子I64特征场景路径边事实> 路径;
};

struct 原子I64特征出生读取结果 final {
    原子I64特征出生状态 状态 = 原子I64特征出生状态::入口拒绝;
    std::optional<原子I64特征出生事实> 事实;
    bool 成功(const 原子I64特征出生读取请求&) const noexcept;
};

enum class 原子I64特征出生使用读取状态 : std::uint8_t {
    已读取 = 1, 入口拒绝 = 2, 未找到 = 3, 概念不适配 = 4,
    资源失败 = 7, 内部不一致 = 8
};

struct 原子I64特征出生使用读取请求 final { 稳定编码 F{}; };
struct 原子I64特征出生使用读取结果 final {
    原子I64特征出生使用读取状态 状态 = 原子I64特征出生使用读取状态::入口拒绝;
    std::optional<特征概念出生使用事实> 事实;
    bool 成功(const 原子I64特征出生使用读取请求&) const noexcept;
};

struct 原子I64特征出生使用退出片段请求 final {
    稳定编码 F{};
    L1所有者范围写入幂等身份 概念使用退出键{};
};

template<class T>
struct 原子I64特征参与结果 final {
    原子I64特征出生状态 状态 = 原子I64特征出生状态::入口拒绝;
    std::optional<T> 数据;
};

class 原子I64特征出生数据服务;

class 原子I64特征内容参与者 {
public:
    virtual ~原子I64特征内容参与者() = default;
private:
    friend class 原子I64特征出生数据服务;
    virtual const L1事实基座服务& 原子I64底座() const noexcept = 0;
    virtual L1所有者范围写端口& 原子I64端口() noexcept = 0;
    virtual bool 原子I64结构已就绪() const noexcept = 0;
    virtual 原子I64特征参与结果<L1有限N分区原子参与者写集>
        准备原子I64出生片段(const 原子I64特征出生请求&) const = 0;
    virtual 原子I64特征候选查询结果 查询原子I64内容候选(
        const 原子I64特征候选查询请求&) const = 0;
    virtual 原子I64特征窄读取结果<原子I64特征内容事实>
        读取原子I64内容(const 原子I64特征内容读取请求&) const = 0;
};

class 原子I64特征holder参与者 {
public:
    virtual ~原子I64特征holder参与者() = default;
private:
    friend class 原子I64特征出生数据服务;
    virtual const L1事实基座服务& 原子I64底座() const noexcept = 0;
    virtual L1所有者范围写端口& 原子I64端口() noexcept = 0;
    virtual bool 原子I64结构已就绪() const noexcept = 0;
    virtual 原子I64特征参与结果<L1有限N分区原子参与者写集>
        准备原子I64出生片段(const 原子I64特征出生请求&) const = 0;
    virtual 原子I64特征窄读取结果<原子I64特征holder事实>
        读取原子I64holder(const 原子I64特征holder读取请求&) const = 0;
};

class 原子I64特征组织参与者 {
public:
    virtual ~原子I64特征组织参与者() = default;
private:
    friend class 原子I64特征出生数据服务;
    virtual const L1事实基座服务& 原子I64底座() const noexcept = 0;
    virtual L1所有者范围写端口& 原子I64端口() noexcept = 0;
    virtual bool 原子I64结构已就绪() const noexcept = 0;
    virtual 原子I64特征参与结果<L1有限N分区原子参与者写集>
        准备原子I64出生片段(const 原子I64特征出生请求&) const = 0;
    virtual 原子I64特征窄读取结果<原子I64特征组织事实>
        读取原子I64组织(const 原子I64特征组织读取请求&) const = 0;
};

class 原子I64特征概念参与者 {
public:
    virtual ~原子I64特征概念参与者() = default;
private:
    friend class 原子I64特征出生数据服务;
    virtual const L1事实基座服务& 原子I64底座() const noexcept = 0;
    virtual L1所有者范围写端口& 原子I64端口() noexcept = 0;
    virtual bool 原子I64结构已就绪() const noexcept = 0;
    virtual 原子I64特征参与结果<L1有限N分区原子参与者写集>
        准备原子I64出生片段(const 原子I64特征出生请求&) const = 0;
    virtual 原子I64特征出生使用读取结果 读取原子I64出生使用(
        const 原子I64特征出生使用读取请求&) const = 0;
    virtual 原子I64特征参与结果<L1有限N分区原子参与者写集>
        准备原子I64出生使用退出片段(
            const 原子I64特征出生使用退出片段请求&) const = 0;
};

class 原子I64特征出生数据服务 final {
public:
    原子I64特征出生数据服务(原子I64特征内容参与者&,
        原子I64特征holder参与者&, 原子I64特征组织参与者&,
        原子I64特征概念参与者&);
    原子I64特征候选查询结果 查询精确候选(
        const 原子I64特征候选查询请求&) const;
    原子I64特征出生结果 创建(const 原子I64特征出生请求&);
    原子I64特征出生读取结果 读取(const 原子I64特征出生读取请求&) const;
private:
    原子I64特征内容参与者& 内容_;
    原子I64特征holder参与者& holder_;
    原子I64特征组织参与者& 组织_;
    原子I64特征概念参与者& 概念_;
    mutable std::mutex mutex_;
};

} // namespace 海中鱼巣
