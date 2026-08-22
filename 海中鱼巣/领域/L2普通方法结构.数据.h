#pragma once

#ifndef L2_ORDINARY_METHOD_STRUCTURE_NO_INCLUDES
#include <algorithm>
#include <cstdint>
#include <optional>
#include <type_traits>
#include <variant>
#include <vector>
#include "L2结构公共.数据.h"
#include "L2状态结构.数据.h"
#endif

namespace 海中鱼巣 {

inline constexpr std::uint32_t L2普通方法结构合同版本 = 2;
inline constexpr std::uint32_t L2普通方法规格引用合同版本 = 2;

#define 定义L2普通方法节点身份(类型名) \
    struct 类型名 final { \
        稳定编码 值; \
        explicit 类型名(稳定编码 编码 = {}) noexcept : 值(编码) {} \
        friend bool operator==(const 类型名&, const 类型名&) = default; \
    }

定义L2普通方法节点身份(L2方法治理虚拟存在身份);
定义L2普通方法节点身份(L2方法条件身份);
定义L2普通方法节点身份(L2方法结果身份);
定义L2普通方法节点身份(L2方法动作入口身份);
定义L2普通方法节点身份(L2方法用途身份);
定义L2普通方法节点身份(L2方法输入规格身份);
定义L2普通方法节点身份(L2方法限制条件身份);
定义L2普通方法节点身份(L2方法条件结果关系身份);

#undef 定义L2普通方法节点身份

enum class L2方法变化方向 : std::uint8_t { 负方向 = 1, 无变化 = 2, 正方向 = 3 };
enum class L2方法限制条件类别 : std::uint8_t {
    禁止项 = 1, 适用范围 = 2, 规格约束 = 3, 安全限制 = 4
};

using L2方法结果类型引用 = std::variant<L2目标状态合同身份, L2概念身份>;

inline bool L2方法结果类型引用相同(const L2方法结果类型引用& 左,
    const L2方法结果类型引用& 右) noexcept {
    if (const auto* 左状态 = std::get_if<L2目标状态合同身份>(&左)) {
        const auto* 右状态 = std::get_if<L2目标状态合同身份>(&右);
        return 右状态 && *左状态 == *右状态;
    }
    const auto* 左概念 = std::get_if<L2概念身份>(&左);
    const auto* 右概念 = std::get_if<L2概念身份>(&右);
    return 左概念 && 右概念 && *左概念 == *右概念;
}

struct L2方法稳定动作键 final {
    std::uint64_t 值 = 0;
    friend bool operator==(const L2方法稳定动作键&,
        const L2方法稳定动作键&) = default;
};
struct L2方法内容版本 final {
    std::uint64_t 值 = 0;
    friend bool operator==(const L2方法内容版本&, const L2方法内容版本&) = default;
};
struct L2方法规格版本 final {
    std::uint64_t 值 = 0;
    friend bool operator==(const L2方法规格版本&, const L2方法规格版本&) = default;
};
struct L2方法生命周期版本 final {
    std::uint64_t 值 = 0;
    friend bool operator==(const L2方法生命周期版本&,
        const L2方法生命周期版本&) = default;
};
struct L2方法规格材料 final {
    std::uint64_t 角色 = 0;
    std::uint64_t 顺序 = 0;
    L2原始值材料 值 = std::int64_t{0};
    friend bool operator==(const L2方法规格材料&, const L2方法规格材料&) = default;
};

struct L2方法用途提交项 final {
    std::uint64_t 用途本地顺序 = 0;
    std::uint64_t 作用对象角色 = 0;
    L2概念身份 目标对象类型;
    L2特征定义身份 可改变特征类型;
    L2方法变化方向 变化方向 = L2方法变化方向::无变化;
    L2方法结果类型引用 结果类型 = L2目标状态合同身份{};
    稳定编码 来源稳定编码;
    friend bool operator==(const L2方法用途提交项& 左,
        const L2方法用途提交项& 右) noexcept {
        return 左.用途本地顺序 == 右.用途本地顺序
            && 左.作用对象角色 == 右.作用对象角色
            && 左.目标对象类型 == 右.目标对象类型
            && 左.可改变特征类型 == 右.可改变特征类型
            && 左.变化方向 == 右.变化方向
            && L2方法结果类型引用相同(左.结果类型, 右.结果类型)
            && 左.来源稳定编码 == 右.来源稳定编码;
    }
};
struct L2方法条件提交项 final {
    std::uint64_t 条件本地顺序 = 0;
    std::uint64_t 作用对象角色 = 0;
    L2概念身份 目标对象类型;
    std::optional<L2场景身份> 场景;
    L2特征定义身份 特征定义;
    L2目标状态合同身份 条件状态合同;
    稳定编码 来源稳定编码;
    friend bool operator==(const L2方法条件提交项&, const L2方法条件提交项&) = default;
};
struct L2方法结果提交项 final {
    std::uint64_t 结果本地顺序 = 0;
    std::uint64_t 作用对象角色 = 0;
    L2概念身份 目标对象类型;
    L2特征定义身份 特征定义;
    L2方法变化方向 变化方向 = L2方法变化方向::无变化;
    L2方法结果类型引用 结果类型 = L2目标状态合同身份{};
    稳定编码 来源稳定编码;
    friend bool operator==(const L2方法结果提交项& 左,
        const L2方法结果提交项& 右) noexcept {
        return 左.结果本地顺序 == 右.结果本地顺序
            && 左.作用对象角色 == 右.作用对象角色
            && 左.目标对象类型 == 右.目标对象类型
            && 左.特征定义 == 右.特征定义
            && 左.变化方向 == 右.变化方向
            && L2方法结果类型引用相同(左.结果类型, 右.结果类型)
            && 左.来源稳定编码 == 右.来源稳定编码;
    }
};
struct L2方法条件结果关系提交项 final {
    std::uint64_t 顺序 = 0;
    std::uint64_t 条件本地顺序 = 0;
    std::uint64_t 结果本地顺序 = 0;
    稳定编码 来源稳定编码;
    friend bool operator==(const L2方法条件结果关系提交项&,
        const L2方法条件结果关系提交项&) = default;
};
struct L2方法输入规格提交项 final {
    std::uint64_t 输入规格本地顺序 = 0;
    std::optional<std::uint64_t> 条件本地顺序;
    std::uint64_t 作用对象角色 = 0;
    std::uint64_t 顺序 = 0;
    L2方法规格材料 规格;
    稳定编码 来源稳定编码;
    friend bool operator==(const L2方法输入规格提交项&,
        const L2方法输入规格提交项&) = default;
};
struct L2方法限制条件提交项 final {
    std::uint64_t 限制条件本地顺序 = 0;
    std::optional<std::uint64_t> 条件本地顺序;
    L2方法限制条件类别 类别 = L2方法限制条件类别::规格约束;
    std::uint64_t 作用对象角色 = 0;
    std::uint64_t 顺序 = 0;
    L2方法规格材料 规格;
    稳定编码 来源稳定编码;
    friend bool operator==(const L2方法限制条件提交项&,
        const L2方法限制条件提交项&) = default;
};
struct L2方法动作入口提交项 final {
    L2场景身份 输入场景;
    L2场景身份 输出场景;
    L2方法稳定动作键 稳定动作键;
    稳定编码 来源稳定编码;
    friend bool operator==(const L2方法动作入口提交项&,
        const L2方法动作入口提交项&) = default;
};

struct L2方法用途事实 final {
    L2方法用途身份 身份;
    L2方法身份 方法;
    std::uint64_t 作用对象角色 = 0;
    L2概念身份 目标对象类型;
    L2特征定义身份 可改变特征类型;
    L2方法变化方向 变化方向 = L2方法变化方向::无变化;
    L2方法结果类型引用 结果类型 = L2目标状态合同身份{};
    稳定编码 来源稳定编码;
    L2方法内容版本 内容版本;
    L2生命周期 生命周期;
    friend bool operator==(const L2方法用途事实& 左,
        const L2方法用途事实& 右) noexcept {
        return 左.身份 == 右.身份 && 左.方法 == 右.方法
            && 左.作用对象角色 == 右.作用对象角色
            && 左.目标对象类型 == 右.目标对象类型
            && 左.可改变特征类型 == 右.可改变特征类型
            && 左.变化方向 == 右.变化方向
            && L2方法结果类型引用相同(左.结果类型, 右.结果类型)
            && 左.来源稳定编码 == 右.来源稳定编码
            && 左.内容版本 == 右.内容版本 && 左.生命周期 == 右.生命周期;
    }
};
struct L2方法条件事实 final {
    L2方法条件身份 身份;
    L2方法身份 方法;
    std::uint64_t 作用对象角色 = 0;
    L2概念身份 目标对象类型;
    std::optional<L2场景身份> 场景;
    L2特征定义身份 特征定义;
    L2目标状态合同身份 条件状态合同;
    稳定编码 来源稳定编码;
    L2方法内容版本 内容版本;
    L2生命周期 生命周期;
    friend bool operator==(const L2方法条件事实&, const L2方法条件事实&) = default;
};
struct L2方法结果事实 final {
    L2方法结果身份 身份;
    L2方法身份 方法;
    std::uint64_t 作用对象角色 = 0;
    L2概念身份 目标对象类型;
    L2特征定义身份 特征定义;
    L2方法变化方向 变化方向 = L2方法变化方向::无变化;
    L2方法结果类型引用 结果类型 = L2目标状态合同身份{};
    稳定编码 来源稳定编码;
    L2方法内容版本 内容版本;
    L2生命周期 生命周期;
    friend bool operator==(const L2方法结果事实& 左,
        const L2方法结果事实& 右) noexcept {
        return 左.身份 == 右.身份 && 左.方法 == 右.方法
            && 左.作用对象角色 == 右.作用对象角色
            && 左.目标对象类型 == 右.目标对象类型
            && 左.特征定义 == 右.特征定义
            && 左.变化方向 == 右.变化方向
            && L2方法结果类型引用相同(左.结果类型, 右.结果类型)
            && 左.来源稳定编码 == 右.来源稳定编码
            && 左.内容版本 == 右.内容版本 && 左.生命周期 == 右.生命周期;
    }
};
struct L2方法条件结果关系事实 final {
    L2方法条件结果关系身份 身份;
    L2方法身份 方法;
    L2方法条件身份 条件;
    L2方法结果身份 结果;
    std::uint64_t 顺序 = 0;
    稳定编码 来源稳定编码;
    L2方法内容版本 内容版本;
    L2生命周期 生命周期;
    friend bool operator==(const L2方法条件结果关系事实&,
        const L2方法条件结果关系事实&) = default;
};
struct L2方法输入规格事实 final {
    L2方法输入规格身份 身份;
    L2方法身份 方法;
    std::optional<L2方法条件身份> 条件;
    std::uint64_t 作用对象角色 = 0;
    std::uint64_t 顺序 = 0;
    L2方法规格材料 规格;
    稳定编码 来源稳定编码;
    L2方法内容版本 内容版本;
    L2生命周期 生命周期;
    friend bool operator==(const L2方法输入规格事实&,
        const L2方法输入规格事实&) = default;
};
struct L2方法限制条件事实 final {
    L2方法限制条件身份 身份;
    L2方法身份 方法;
    std::optional<L2方法条件身份> 条件;
    L2方法限制条件类别 类别 = L2方法限制条件类别::规格约束;
    std::uint64_t 作用对象角色 = 0;
    std::uint64_t 顺序 = 0;
    L2方法规格材料 规格;
    稳定编码 来源稳定编码;
    L2方法内容版本 内容版本;
    L2生命周期 生命周期;
    friend bool operator==(const L2方法限制条件事实&,
        const L2方法限制条件事实&) = default;
};
struct L2方法动作入口事实 final {
    L2方法动作入口身份 身份;
    L2方法身份 方法;
    L2场景身份 输入场景;
    L2场景身份 输出场景;
    L2方法稳定动作键 稳定动作键;
    L2方法内容版本 内容版本;
    稳定编码 来源稳定编码;
    L2生命周期 生命周期;
    friend bool operator==(const L2方法动作入口事实&,
        const L2方法动作入口事实&) = default;
};
struct L2方法生命周期事实 final {
    L2方法身份 方法;
    L2方法生命周期版本 生命周期版本;
    std::int64_t 发生时间 = 0;
    稳定编码 来源稳定编码;
    L2生命周期 生命周期;
    friend bool operator==(const L2方法生命周期事实&,
        const L2方法生命周期事实&) = default;
};

enum class L2方法结构缺口类型 : std::uint8_t {
    缺方法用途 = 1, 用途结果冲突 = 2, 条件结果关系缺失 = 3,
    输入规格引用冲突 = 4, 限制条件引用冲突 = 5, 旧合同不能投影 = 6
};
struct L2方法结构缺口 final {
    L2方法结构缺口类型 类型 = L2方法结构缺口类型::缺方法用途;
    L2方法身份 方法;
    std::optional<L2方法条件身份> 条件;
    std::optional<L2方法结果身份> 结果;
    friend bool operator==(const L2方法结构缺口&, const L2方法结构缺口&) = default;
};
struct L2方法条件语义项 final {
    std::uint64_t 作用对象角色 = 0;
    L2概念身份 目标对象类型;
    std::optional<L2场景身份> 场景;
    L2特征定义身份 特征定义;
    L2目标状态合同身份 条件状态合同;
    friend bool operator==(const L2方法条件语义项&,
        const L2方法条件语义项&) = default;
};
struct L2方法输入规格语义项 final {
    std::uint64_t 作用对象角色 = 0;
    std::uint64_t 顺序 = 0;
    L2方法规格材料 规格;
    friend bool operator==(const L2方法输入规格语义项&,
        const L2方法输入规格语义项&) = default;
};
struct L2方法限制条件语义项 final {
    L2方法限制条件类别 类别 = L2方法限制条件类别::规格约束;
    std::uint64_t 作用对象角色 = 0;
    std::uint64_t 顺序 = 0;
    L2方法规格材料 规格;
    friend bool operator==(const L2方法限制条件语义项&,
        const L2方法限制条件语义项&) = default;
};
struct L2方法输入条件签名 final {
    std::vector<L2方法条件语义项> 条件;
    std::vector<L2方法输入规格语义项> 输入规格;
    friend bool operator==(const L2方法输入条件签名&,
        const L2方法输入条件签名&) = default;
};
struct L2方法六项静态能力签名 final {
    L2方法输入条件签名 输入条件;
    L2概念身份 目标对象类型;
    L2特征定义身份 可改变特征类型;
    L2方法变化方向 变化方向 = L2方法变化方向::无变化;
    L2方法结果类型引用 结果类型 = L2目标状态合同身份{};
    std::vector<L2方法限制条件语义项> 限制条件;
    friend bool operator==(const L2方法六项静态能力签名& 左,
        const L2方法六项静态能力签名& 右) noexcept {
        return 左.输入条件 == 右.输入条件
            && 左.目标对象类型 == 右.目标对象类型
            && 左.可改变特征类型 == 右.可改变特征类型
            && 左.变化方向 == 右.变化方向
            && L2方法结果类型引用相同(左.结果类型, 右.结果类型)
            && 左.限制条件 == 右.限制条件;
    }
};
struct L2方法六项静态能力投影 final {
    L2方法身份 方法;
    L2方法内容版本 内容版本;
    L2方法规格版本 规格版本;
    L2方法结果身份 主轴结果;
    L2方法用途身份 匹配用途;
    L2方法六项静态能力签名 签名;
    friend bool operator==(const L2方法六项静态能力投影&,
        const L2方法六项静态能力投影&) = default;
};

struct L2普通方法事实 final {
    std::uint32_t 普通方法合同版本 = L2普通方法结构合同版本;
    L2方法身份 方法;
    L2节点族身份来源事实 身份来源;
    L2方法治理虚拟存在身份 治理虚拟存在;
    L2方法内容版本 内容版本;
    L2方法规格版本 规格版本;
    std::vector<L2方法用途事实> 用途;
    std::vector<L2方法条件事实> 条件;
    std::vector<L2方法结果事实> 结果;
    std::vector<L2方法条件结果关系事实> 条件结果关系;
    std::vector<L2方法输入规格事实> 输入规格;
    std::vector<L2方法限制条件事实> 限制条件;
    L2方法动作入口事实 动作入口;
    L2方法生命周期事实 生命周期事实;
    friend bool operator==(const L2普通方法事实&, const L2普通方法事实&) = default;
};
struct L2普通方法提交材料 final {
    L2方法内容版本 内容版本;
    L2方法规格版本 规格版本;
    L2方法生命周期版本 生命周期版本;
    std::int64_t 生命周期发生时间 = 0;
    稳定编码 生命周期来源稳定编码;
    std::vector<L2方法用途提交项> 用途;
    std::vector<L2方法条件提交项> 条件;
    std::vector<L2方法结果提交项> 结果;
    std::vector<L2方法条件结果关系提交项> 条件结果关系;
    std::vector<L2方法输入规格提交项> 输入规格;
    std::vector<L2方法限制条件提交项> 限制条件;
    L2方法动作入口提交项 动作入口;
    friend bool operator==(const L2普通方法提交材料&,
        const L2普通方法提交材料&) = default;
};

struct L2普通方法新增请求 final {
    L2结构请求头 请求头;
    std::uint32_t 普通方法合同版本 = L2普通方法结构合同版本;
    L2结构幂等身份 幂等身份;
    L2普通方法提交材料 方法;
};
struct L2普通方法替换请求 final {
    L2结构请求头 请求头;
    std::uint32_t 普通方法合同版本 = L2普通方法结构合同版本;
    L2结构幂等身份 幂等身份;
    L2方法身份 旧方法;
    L2方法内容版本 旧内容版本;
    L2方法规格版本 旧规格版本;
    L2方法生命周期版本 旧生命周期版本;
    L2普通方法提交材料 新方法;
};
struct L2普通方法退出请求 final {
    L2结构请求头 请求头;
    std::uint32_t 普通方法合同版本 = L2普通方法结构合同版本;
    L2结构幂等身份 幂等身份;
    L2方法身份 方法;
    L2方法内容版本 内容版本;
    L2方法规格版本 规格版本;
    L2方法生命周期版本 生命周期版本;
};
struct L2普通方法读取请求 final {
    L2结构请求头 请求头;
    std::uint32_t 普通方法合同版本 = L2普通方法结构合同版本;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2方法身份 方法;
    std::uint64_t 历史截止事实代次 = 0;
};
struct L2方法用途粗召回键 final {
    L2概念身份 目标对象类型;
    L2特征定义身份 可改变特征类型;
    L2方法变化方向 变化方向 = L2方法变化方向::无变化;
    L2方法结果类型引用 结果类型 = L2目标状态合同身份{};
    friend bool operator==(const L2方法用途粗召回键& 左,
        const L2方法用途粗召回键& 右) noexcept {
        return 左.目标对象类型 == 右.目标对象类型
            && 左.可改变特征类型 == 右.可改变特征类型
            && 左.变化方向 == 右.变化方向
            && L2方法结果类型引用相同(左.结果类型, 右.结果类型);
    }
};
struct L2方法用途粗召回请求 final {
    L2结构请求头 请求头;
    std::uint32_t 普通方法合同版本 = L2普通方法结构合同版本;
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止事实代次 = 0;
    L2方法用途粗召回键 用途;
    std::uint64_t 最大扫描用途事实数 = 0;
    std::uint64_t 最大粗召回方法数 = 0;
};
struct L2方法用途粗召回项 final {
    L2方法身份 方法;
    L2方法用途事实 用途;
    friend bool operator==(const L2方法用途粗召回项&,
        const L2方法用途粗召回项&) = default;
};
struct L2方法静态能力投影读取请求 final {
    L2结构请求头 请求头;
    std::uint32_t 普通方法合同版本 = L2普通方法结构合同版本;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2方法身份 方法;
    std::uint64_t 历史截止事实代次 = 0;
};

struct L2普通方法写入结果 final {
    L2结构结果头 结果头;
    std::optional<L2普通方法事实> 方法;
    bool 成功() const noexcept;
};
struct L2普通方法替换结果 final {
    L2结构结果头 结果头;
    std::optional<L2普通方法事实> 当前方法;
    std::optional<L2普通方法事实> 已退出方法;
    bool 成功() const noexcept;
};
struct L2普通方法退出结果 final {
    L2结构结果头 结果头;
    std::optional<L2普通方法事实> 已退出方法;
    bool 成功() const noexcept;
};
struct L2普通方法读取结果 final {
    L2结构结果头 结果头;
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止事实代次 = 0;
    std::optional<L2普通方法事实> 方法;
    bool 成功() const noexcept;
};
struct L2方法用途粗召回结果 final {
    L2结构结果头 结果头;
    std::uint64_t 实际扫描用途事实数 = 0;
    std::vector<L2方法用途粗召回项> 粗召回;
    bool 成功() const noexcept;
};
struct L2方法静态能力投影读取结果 final {
    L2结构结果头 结果头;
    std::vector<L2方法六项静态能力投影> 投影;
    std::vector<L2方法结构缺口> 结构缺口;
    bool 成功() const noexcept;
};

inline bool L2方法变化方向有效(L2方法变化方向 值) noexcept {
    return 值 == L2方法变化方向::负方向 || 值 == L2方法变化方向::无变化
        || 值 == L2方法变化方向::正方向;
}
inline bool L2方法限制条件类别有效(L2方法限制条件类别 值) noexcept {
    return 值 == L2方法限制条件类别::禁止项
        || 值 == L2方法限制条件类别::适用范围
        || 值 == L2方法限制条件类别::规格约束
        || 值 == L2方法限制条件类别::安全限制;
}
inline std::uint64_t L2方法结果类型分支机器序号(
    const L2方法结果类型引用& 引用) noexcept {
    return std::holds_alternative<L2目标状态合同身份>(引用) ? 1U : 2U;
}
inline 稳定编码 L2方法结果类型稳定编码值(
    const L2方法结果类型引用& 引用) noexcept {
    if (const auto* 状态 = std::get_if<L2目标状态合同身份>(&引用)) return 状态->值;
    return std::get<L2概念身份>(引用).值;
}
inline bool L2方法结果类型引用有效(
    const L2方法结果类型引用& 引用) noexcept {
    return 有效(L2方法结果类型稳定编码值(引用));
}
inline bool L2方法结果类型引用小于(const L2方法结果类型引用& 左,
    const L2方法结果类型引用& 右) noexcept {
    const auto 左分支 = L2方法结果类型分支机器序号(左);
    const auto 右分支 = L2方法结果类型分支机器序号(右);
    return 左分支 != 右分支 ? 左分支 < 右分支
        : L2方法结果类型稳定编码值(左).值
            < L2方法结果类型稳定编码值(右).值;
}

inline std::uint64_t L2方法原始值材料分支机器序号(
    const L2原始值材料& 材料) noexcept {
    if (std::holds_alternative<std::int64_t>(材料)) return 1U;
    if (std::holds_alternative<std::vector<std::int64_t>>(材料)) return 2U;
    if (std::holds_alternative<std::vector<std::uint64_t>>(材料)) return 3U;
    return 4U;
}

inline bool L2方法原始值材料小于(const L2原始值材料& 左,
    const L2原始值材料& 右) noexcept {
    const auto 左分支 = L2方法原始值材料分支机器序号(左);
    const auto 右分支 = L2方法原始值材料分支机器序号(右);
    if (左分支 != 右分支) return 左分支 < 右分支;
    if (const auto* 左值 = std::get_if<std::int64_t>(&左))
        return *左值 < *std::get_if<std::int64_t>(&右);
    if (const auto* 左值 = std::get_if<std::vector<std::int64_t>>(&左))
        return std::lexicographical_compare(左值->begin(), 左值->end(),
            std::get<std::vector<std::int64_t>>(右).begin(),
            std::get<std::vector<std::int64_t>>(右).end());
    if (const auto* 左值 = std::get_if<std::vector<std::uint64_t>>(&左))
        return std::lexicographical_compare(左值->begin(), 左值->end(),
            std::get<std::vector<std::uint64_t>>(右).begin(),
            std::get<std::vector<std::uint64_t>>(右).end());
    return std::get<L2独立材料引用>(左).编码.值
        < std::get<L2独立材料引用>(右).编码.值;
}

#define 定义L2普通方法身份有效函数(函数名, 类型名) \
    inline bool 函数名(类型名 身份) noexcept { return 有效(身份.值); }
定义L2普通方法身份有效函数(L2方法治理虚拟存在身份有效, L2方法治理虚拟存在身份);
定义L2普通方法身份有效函数(L2方法条件身份有效, L2方法条件身份);
定义L2普通方法身份有效函数(L2方法结果身份有效, L2方法结果身份);
定义L2普通方法身份有效函数(L2方法动作入口身份有效, L2方法动作入口身份);
定义L2普通方法身份有效函数(L2方法用途身份有效, L2方法用途身份);
定义L2普通方法身份有效函数(L2方法输入规格身份有效, L2方法输入规格身份);
定义L2普通方法身份有效函数(L2方法限制条件身份有效, L2方法限制条件身份);
定义L2普通方法身份有效函数(L2方法条件结果关系身份有效,
    L2方法条件结果关系身份);
#undef 定义L2普通方法身份有效函数

inline bool L2方法内容版本有效(L2方法内容版本 值) noexcept { return 值.值 != 0; }
inline bool L2方法规格版本有效(L2方法规格版本 值) noexcept { return 值.值 != 0; }
inline bool L2方法生命周期版本有效(L2方法生命周期版本 值) noexcept { return 值.值 != 0; }
inline bool L2方法稳定动作键有效(L2方法稳定动作键 值) noexcept { return 值.值 != 0; }
inline bool L2方法规格材料有效(const L2方法规格材料& 材料) noexcept {
    if (材料.角色 == 0 || 材料.顺序 == 0) return false;
    return std::visit([](const auto& 值) noexcept {
        using 类型 = std::decay_t<decltype(值)>;
        if constexpr (std::is_same_v<类型, std::int64_t>) return true;
        else if constexpr (std::is_same_v<类型, L2独立材料引用>) return 有效(值.编码);
        else return !值.empty();
    }, 材料.值);
}

template<class 类型, class 取值>
inline bool L2方法本地顺序连续(const std::vector<类型>& 组, 取值 取顺序) noexcept {
    for (std::size_t i = 0; i < 组.size(); ++i)
        if (取顺序(组[i]) != i + 1) return false;
    return true;
}

inline bool L2普通方法提交材料有效(const L2普通方法提交材料& 方法) noexcept {
    if (!L2方法内容版本有效(方法.内容版本)
        || !L2方法规格版本有效(方法.规格版本)
        || !L2方法生命周期版本有效(方法.生命周期版本)
        || !有效(方法.生命周期来源稳定编码) || 方法.用途.empty()
        || 方法.结果.empty()
        || !L2方法本地顺序连续(方法.用途,
            [](const auto& 项) { return 项.用途本地顺序; })
        || !L2方法本地顺序连续(方法.条件,
            [](const auto& 项) { return 项.条件本地顺序; })
        || !L2方法本地顺序连续(方法.结果,
            [](const auto& 项) { return 项.结果本地顺序; })
        || !L2方法本地顺序连续(方法.输入规格,
            [](const auto& 项) { return 项.输入规格本地顺序; })
        || !L2方法本地顺序连续(方法.限制条件,
            [](const auto& 项) { return 项.限制条件本地顺序; })
        || !L2方法本地顺序连续(方法.条件结果关系,
            [](const auto& 项) { return 项.顺序; })
        || !有效(方法.动作入口.输入场景.值)
        || !有效(方法.动作入口.输出场景.值)
        || !L2方法稳定动作键有效(方法.动作入口.稳定动作键)
        || !有效(方法.动作入口.来源稳定编码)) return false;
    for (const auto& 项 : 方法.用途)
        if (项.作用对象角色 == 0 || !有效(项.目标对象类型.值)
            || !有效(项.可改变特征类型.值)
            || !L2方法变化方向有效(项.变化方向)
            || !L2方法结果类型引用有效(项.结果类型)
            || !有效(项.来源稳定编码)) return false;
    for (const auto& 项 : 方法.条件)
        if (项.作用对象角色 == 0 || !有效(项.目标对象类型.值)
            || (项.场景 && !有效(项.场景->值))
            || !有效(项.特征定义.值) || !有效(项.条件状态合同.值)
            || !有效(项.来源稳定编码)) return false;
    for (const auto& 项 : 方法.结果)
        if (项.作用对象角色 == 0 || !有效(项.目标对象类型.值)
            || !有效(项.特征定义.值)
            || !L2方法变化方向有效(项.变化方向)
            || !L2方法结果类型引用有效(项.结果类型)
            || !有效(项.来源稳定编码)) return false;
    for (const auto& 项 : 方法.条件结果关系)
        if (项.顺序 == 0 || 项.条件本地顺序 == 0
            || 项.条件本地顺序 > 方法.条件.size()
            || 项.结果本地顺序 == 0 || 项.结果本地顺序 > 方法.结果.size()
            || !有效(项.来源稳定编码)) return false;
    for (const auto& 项 : 方法.输入规格)
        if ((项.条件本地顺序 && (*项.条件本地顺序 == 0
                || *项.条件本地顺序 > 方法.条件.size()))
            || 项.作用对象角色 == 0 || 项.顺序 == 0
            || 项.规格.角色 != 项.作用对象角色 || 项.规格.顺序 != 项.顺序
            || !L2方法规格材料有效(项.规格) || !有效(项.来源稳定编码))
            return false;
    for (const auto& 项 : 方法.限制条件)
        if ((项.条件本地顺序 && (*项.条件本地顺序 == 0
                || *项.条件本地顺序 > 方法.条件.size()))
            || !L2方法限制条件类别有效(项.类别)
            || 项.作用对象角色 == 0 || 项.顺序 == 0
            || 项.规格.角色 != 项.作用对象角色 || 项.规格.顺序 != 项.顺序
            || !L2方法规格材料有效(项.规格) || !有效(项.来源稳定编码))
            return false;
    return true;
}

inline bool L2普通方法新增请求有效(const L2普通方法新增请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && 请求.普通方法合同版本 == L2普通方法结构合同版本
        && L2结构幂等身份有效(请求.幂等身份)
        && 请求.方法.生命周期版本.值 == 1
        && L2普通方法提交材料有效(请求.方法);
}
inline bool L2普通方法替换请求有效(const L2普通方法替换请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && 请求.普通方法合同版本 == L2普通方法结构合同版本
        && L2结构幂等身份有效(请求.幂等身份) && 有效(请求.旧方法.值)
        && L2方法内容版本有效(请求.旧内容版本)
        && L2方法规格版本有效(请求.旧规格版本)
        && L2方法生命周期版本有效(请求.旧生命周期版本)
        && L2普通方法提交材料有效(请求.新方法)
        && 请求.新方法.生命周期版本.值
            == 请求.旧生命周期版本.值 + 1;
}
inline bool L2普通方法退出请求有效(const L2普通方法退出请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && 请求.普通方法合同版本 == L2普通方法结构合同版本
        && L2结构幂等身份有效(请求.幂等身份) && 有效(请求.方法.值)
        && L2方法内容版本有效(请求.内容版本)
        && L2方法规格版本有效(请求.规格版本)
        && L2方法生命周期版本有效(请求.生命周期版本);
}
inline bool L2读取截止请求有效(L2结构请求头 头, L2读取类别 类别,
    std::uint64_t 历史截止) noexcept {
    return L2结构请求头合同有效(头) && 头.期望事实代次 != 0
        && (类别 == L2读取类别::当前 || 类别 == L2读取类别::历史)
        && ((类别 == L2读取类别::当前 && 历史截止 == 0)
            || (类别 == L2读取类别::历史 && 历史截止 != 0
                && 历史截止 <= 头.期望事实代次));
}
inline bool L2普通方法读取请求有效(const L2普通方法读取请求& 请求) noexcept {
    return 请求.普通方法合同版本 == L2普通方法结构合同版本
        && 有效(请求.方法.值)
        && L2读取截止请求有效(请求.请求头, 请求.读取类别,
            请求.历史截止事实代次);
}
inline bool L2方法用途粗召回请求有效(const L2方法用途粗召回请求& 请求) noexcept {
    return 请求.普通方法合同版本 == L2普通方法结构合同版本
        && 有效(请求.用途.目标对象类型.值)
        && 有效(请求.用途.可改变特征类型.值)
        && L2方法变化方向有效(请求.用途.变化方向)
        && L2方法结果类型引用有效(请求.用途.结果类型)
        && 请求.最大扫描用途事实数 != 0 && 请求.最大粗召回方法数 != 0
        && L2读取截止请求有效(请求.请求头, 请求.读取类别,
            请求.历史截止事实代次);
}
inline bool L2方法静态能力投影读取请求有效(
    const L2方法静态能力投影读取请求& 请求) noexcept {
    return 请求.普通方法合同版本 == L2普通方法结构合同版本
        && 有效(请求.方法.值)
        && L2读取截止请求有效(请求.请求头, 请求.读取类别,
            请求.历史截止事实代次);
}

inline bool L2方法条件语义小于(const L2方法条件语义项& 左,
    const L2方法条件语义项& 右) noexcept {
    const auto 左场景 = 左.场景 ? 左.场景->值.值 : 0;
    const auto 右场景 = 右.场景 ? 右.场景->值.值 : 0;
    if (左.作用对象角色 != 右.作用对象角色) return 左.作用对象角色 < 右.作用对象角色;
    if (左.目标对象类型.值.值 != 右.目标对象类型.值.值)
        return 左.目标对象类型.值.值 < 右.目标对象类型.值.值;
    if (左场景 != 右场景) return 左场景 < 右场景;
    if (左.特征定义.值.值 != 右.特征定义.值.值)
        return 左.特征定义.值.值 < 右.特征定义.值.值;
    return 左.条件状态合同.值.值 < 右.条件状态合同.值.值;
}
inline bool L2方法输入规格语义小于(const L2方法输入规格语义项& 左,
    const L2方法输入规格语义项& 右) noexcept {
    if (左.作用对象角色 != 右.作用对象角色) return 左.作用对象角色 < 右.作用对象角色;
    if (左.顺序 != 右.顺序) return 左.顺序 < 右.顺序;
    return L2方法原始值材料小于(左.规格.值, 右.规格.值);
}
inline bool L2方法限制条件语义小于(const L2方法限制条件语义项& 左,
    const L2方法限制条件语义项& 右) noexcept {
    if (左.类别 != 右.类别)
        return static_cast<std::uint8_t>(左.类别) < static_cast<std::uint8_t>(右.类别);
    if (左.作用对象角色 != 右.作用对象角色) return 左.作用对象角色 < 右.作用对象角色;
    if (左.顺序 != 右.顺序) return 左.顺序 < 右.顺序;
    return L2方法原始值材料小于(左.规格.值, 右.规格.值);
}

inline bool L2方法输入条件签名小于(const L2方法输入条件签名& 左,
    const L2方法输入条件签名& 右) noexcept {
    if (std::lexicographical_compare(左.条件.begin(), 左.条件.end(),
            右.条件.begin(), 右.条件.end(), L2方法条件语义小于)) return true;
    if (std::lexicographical_compare(右.条件.begin(), 右.条件.end(),
            左.条件.begin(), 左.条件.end(), L2方法条件语义小于)) return false;
    return std::lexicographical_compare(左.输入规格.begin(), 左.输入规格.end(),
        右.输入规格.begin(), 右.输入规格.end(), L2方法输入规格语义小于);
}

inline bool L2方法六项静态能力签名小于(
    const L2方法六项静态能力签名& 左,
    const L2方法六项静态能力签名& 右) noexcept {
    if (L2方法输入条件签名小于(左.输入条件, 右.输入条件)) return true;
    if (L2方法输入条件签名小于(右.输入条件, 左.输入条件)) return false;
    if (左.目标对象类型.值.值 != 右.目标对象类型.值.值)
        return 左.目标对象类型.值.值 < 右.目标对象类型.值.值;
    if (左.可改变特征类型.值.值 != 右.可改变特征类型.值.值)
        return 左.可改变特征类型.值.值 < 右.可改变特征类型.值.值;
    if (左.变化方向 != 右.变化方向)
        return static_cast<std::uint8_t>(左.变化方向)
            < static_cast<std::uint8_t>(右.变化方向);
    if (L2方法结果类型引用小于(左.结果类型, 右.结果类型)) return true;
    if (L2方法结果类型引用小于(右.结果类型, 左.结果类型)) return false;
    return std::lexicographical_compare(左.限制条件.begin(), 左.限制条件.end(),
        右.限制条件.begin(), 右.限制条件.end(), L2方法限制条件语义小于);
}

inline bool L2方法六项静态能力投影小于(
    const L2方法六项静态能力投影& 左,
    const L2方法六项静态能力投影& 右) noexcept {
    if (左.签名.目标对象类型.值.值 != 右.签名.目标对象类型.值.值)
        return 左.签名.目标对象类型.值.值 < 右.签名.目标对象类型.值.值;
    if (左.签名.可改变特征类型.值.值 != 右.签名.可改变特征类型.值.值)
        return 左.签名.可改变特征类型.值.值
            < 右.签名.可改变特征类型.值.值;
    if (左.签名.变化方向 != 右.签名.变化方向)
        return static_cast<std::uint8_t>(左.签名.变化方向)
            < static_cast<std::uint8_t>(右.签名.变化方向);
    if (L2方法结果类型引用小于(左.签名.结果类型, 右.签名.结果类型))
        return true;
    if (L2方法结果类型引用小于(右.签名.结果类型, 左.签名.结果类型))
        return false;
    if (L2方法输入条件签名小于(左.签名.输入条件, 右.签名.输入条件))
        return true;
    if (L2方法输入条件签名小于(右.签名.输入条件, 左.签名.输入条件))
        return false;
    if (std::lexicographical_compare(左.签名.限制条件.begin(),
            左.签名.限制条件.end(), 右.签名.限制条件.begin(),
            右.签名.限制条件.end(), L2方法限制条件语义小于)) return true;
    if (std::lexicographical_compare(右.签名.限制条件.begin(),
            右.签名.限制条件.end(), 左.签名.限制条件.begin(),
            左.签名.限制条件.end(), L2方法限制条件语义小于)) return false;
    if (左.主轴结果.值.值 != 右.主轴结果.值.值)
        return 左.主轴结果.值.值 < 右.主轴结果.值.值;
    return 左.匹配用途.值.值 < 右.匹配用途.值.值;
}
template<class 类型, class 小于>
inline bool L2方法语义组规范化(const std::vector<类型>& 组, 小于 比较) noexcept {
    for (std::size_t i = 1; i < 组.size(); ++i)
        if (!比较(组[i - 1], 组[i])) return false;
    return true;
}
inline bool L2方法六项静态能力签名完整(
    const L2方法六项静态能力签名& 签名) noexcept {
    if (!有效(签名.目标对象类型.值) || !有效(签名.可改变特征类型.值)
        || !L2方法变化方向有效(签名.变化方向)
        || !L2方法结果类型引用有效(签名.结果类型)
        || !L2方法语义组规范化(签名.输入条件.条件, L2方法条件语义小于)
        || !L2方法语义组规范化(签名.输入条件.输入规格,
            L2方法输入规格语义小于)
        || !L2方法语义组规范化(签名.限制条件,
            L2方法限制条件语义小于)) return false;
    for (const auto& 项 : 签名.输入条件.条件)
        if (项.作用对象角色 == 0 || !有效(项.目标对象类型.值)
            || (项.场景 && !有效(项.场景->值)) || !有效(项.特征定义.值)
            || !有效(项.条件状态合同.值)) return false;
    for (const auto& 项 : 签名.输入条件.输入规格)
        if (项.作用对象角色 == 0 || 项.顺序 == 0
            || 项.规格.角色 != 项.作用对象角色 || 项.规格.顺序 != 项.顺序
            || !L2方法规格材料有效(项.规格)) return false;
    for (const auto& 项 : 签名.限制条件)
        if (!L2方法限制条件类别有效(项.类别) || 项.作用对象角色 == 0
            || 项.顺序 == 0 || 项.规格.角色 != 项.作用对象角色
            || 项.规格.顺序 != 项.顺序 || !L2方法规格材料有效(项.规格))
            return false;
    return true;
}
inline bool L2方法六项静态能力签名相同(
    const L2方法六项静态能力签名& 左,
    const L2方法六项静态能力签名& 右) noexcept {
    return L2方法六项静态能力签名完整(左)
        && L2方法六项静态能力签名完整(右) && 左 == 右;
}

inline bool L2普通方法事实完整(const L2普通方法事实& 事实,
    std::uint64_t 截止) noexcept {
    const auto 生命周期适用 = [截止](const L2生命周期& 生命周期) noexcept {
        return L2生命周期完整(生命周期)
            && 生命周期.创建事实代次 <= 截止
            && (!生命周期.退出事实代次
                || *生命周期.退出事实代次 <= 截止);
    };
    const auto 身份组稳定 = [](const auto& 组) noexcept {
        for (std::size_t i = 0; i < 组.size(); ++i) {
            if (!有效(组[i].身份.值)) return false;
            if (i != 0 && 组[i - 1].身份.值.值 >= 组[i].身份.值.值)
                return false;
        }
        return true;
    };
    if (截止 == 0 || (事实.普通方法合同版本 != 1
            && 事实.普通方法合同版本 != L2普通方法结构合同版本)
        || !有效(事实.方法.值)
        || !L2节点族身份来源事实完整(事实.身份来源, 截止)
        || 事实.身份来源.节点族 != L2节点族::方法
        || 事实.身份来源.节点稳定编码 != 事实.方法.值
        || !L2方法治理虚拟存在身份有效(事实.治理虚拟存在)
        || !L2方法内容版本有效(事实.内容版本)
        || !L2方法规格版本有效(事实.规格版本)
        || !L2方法动作入口身份有效(事实.动作入口.身份)
        || 事实.动作入口.方法 != 事实.方法
        || !有效(事实.动作入口.输入场景.值)
        || !有效(事实.动作入口.输出场景.值)
        || !L2方法稳定动作键有效(事实.动作入口.稳定动作键)
        || 事实.动作入口.内容版本 != 事实.内容版本
        || !有效(事实.动作入口.来源稳定编码)
        || !生命周期适用(事实.动作入口.生命周期)
        || 事实.生命周期事实.方法 != 事实.方法
        || !L2方法生命周期版本有效(事实.生命周期事实.生命周期版本)
        || !有效(事实.生命周期事实.来源稳定编码)
        || !生命周期适用(事实.生命周期事实.生命周期)) return false;
    if (事实.普通方法合同版本 == 1)
        return 事实.用途.empty() && 事实.条件.empty() && 事实.结果.empty()
            && 事实.条件结果关系.empty() && 事实.输入规格.empty()
            && 事实.限制条件.empty();
    if (事实.用途.empty() || 事实.结果.empty()
        || !身份组稳定(事实.用途) || !身份组稳定(事实.条件)
        || !身份组稳定(事实.结果) || !身份组稳定(事实.条件结果关系)
        || !身份组稳定(事实.输入规格) || !身份组稳定(事实.限制条件))
        return false;
    const auto 条件存在 = [&](L2方法条件身份 身份) noexcept {
        return std::any_of(事实.条件.begin(), 事实.条件.end(),
            [&](const auto& 项) { return 项.身份 == 身份; });
    };
    const auto 结果存在 = [&](L2方法结果身份 身份) noexcept {
        return std::any_of(事实.结果.begin(), 事实.结果.end(),
            [&](const auto& 项) { return 项.身份 == 身份; });
    };
    for (const auto& 项 : 事实.用途)
        if (项.方法 != 事实.方法 || 项.作用对象角色 == 0
            || !有效(项.目标对象类型.值) || !有效(项.可改变特征类型.值)
            || !L2方法变化方向有效(项.变化方向)
            || !L2方法结果类型引用有效(项.结果类型)
            || !有效(项.来源稳定编码) || 项.内容版本 != 事实.内容版本
            || !生命周期适用(项.生命周期)) return false;
    for (std::size_t i = 0; i < 事实.用途.size(); ++i)
        for (std::size_t j = i + 1; j < 事实.用途.size(); ++j)
            if (事实.用途[i].作用对象角色 == 事实.用途[j].作用对象角色
                && 事实.用途[i].目标对象类型 == 事实.用途[j].目标对象类型
                && 事实.用途[i].可改变特征类型
                    == 事实.用途[j].可改变特征类型
                && 事实.用途[i].变化方向 == 事实.用途[j].变化方向
                && L2方法结果类型引用相同(事实.用途[i].结果类型,
                    事实.用途[j].结果类型)) return false;
    for (const auto& 项 : 事实.条件)
        if (项.方法 != 事实.方法 || 项.作用对象角色 == 0
            || !有效(项.目标对象类型.值) || (项.场景 && !有效(项.场景->值))
            || !有效(项.特征定义.值) || !有效(项.条件状态合同.值)
            || !有效(项.来源稳定编码) || 项.内容版本 != 事实.内容版本
            || !生命周期适用(项.生命周期)) return false;
    for (const auto& 项 : 事实.结果)
        if (项.方法 != 事实.方法 || 项.作用对象角色 == 0
            || !有效(项.目标对象类型.值) || !有效(项.特征定义.值)
            || !L2方法变化方向有效(项.变化方向)
            || !L2方法结果类型引用有效(项.结果类型)
            || !有效(项.来源稳定编码) || 项.内容版本 != 事实.内容版本
            || !生命周期适用(项.生命周期)) return false;
    for (std::size_t i = 0; i < 事实.条件结果关系.size(); ++i) {
        const auto& 项 = 事实.条件结果关系[i];
        if (项.方法 != 事实.方法 || !条件存在(项.条件) || !结果存在(项.结果)
            || 项.顺序 == 0 || !有效(项.来源稳定编码)
            || 项.内容版本 != 事实.内容版本
            || !生命周期适用(项.生命周期)) return false;
        for (std::size_t j = i + 1; j < 事实.条件结果关系.size(); ++j) {
            const auto& 后 = 事实.条件结果关系[j];
            if (项.条件 == 后.条件 && 项.结果 == 后.结果 && 项.顺序 == 后.顺序)
                return false;
        }
    }
    for (std::size_t i = 0; i < 事实.输入规格.size(); ++i) {
        const auto& 项 = 事实.输入规格[i];
        if (项.方法 != 事实.方法 || (项.条件 && !条件存在(*项.条件))
            || 项.作用对象角色 == 0 || 项.顺序 == 0
            || 项.规格.角色 != 项.作用对象角色 || 项.规格.顺序 != 项.顺序
            || !L2方法规格材料有效(项.规格) || !有效(项.来源稳定编码)
            || 项.内容版本 != 事实.内容版本
            || !生命周期适用(项.生命周期)) return false;
        for (std::size_t j = i + 1; j < 事实.输入规格.size(); ++j) {
            const auto& 后 = 事实.输入规格[j];
            if (项.条件 == 后.条件 && 项.作用对象角色 == 后.作用对象角色
                && 项.顺序 == 后.顺序) return false;
        }
    }
    for (std::size_t i = 0; i < 事实.限制条件.size(); ++i) {
        const auto& 项 = 事实.限制条件[i];
        if (项.方法 != 事实.方法 || (项.条件 && !条件存在(*项.条件))
            || !L2方法限制条件类别有效(项.类别) || 项.作用对象角色 == 0
            || 项.顺序 == 0 || 项.规格.角色 != 项.作用对象角色
            || 项.规格.顺序 != 项.顺序 || !L2方法规格材料有效(项.规格)
            || !有效(项.来源稳定编码) || 项.内容版本 != 事实.内容版本
            || !生命周期适用(项.生命周期)) return false;
        for (std::size_t j = i + 1; j < 事实.限制条件.size(); ++j) {
            const auto& 后 = 事实.限制条件[j];
            if (项.条件 == 后.条件 && 项.类别 == 后.类别
                && 项.作用对象角色 == 后.作用对象角色
                && 项.顺序 == 后.顺序) return false;
        }
    }
    return true;
}
inline bool L2普通方法写入结果::成功() const noexcept {
    return (结果头.状态 == L2结构状态::已提交
            || 结果头.状态 == L2结构状态::精确重复)
        && 结果头.事实截止代次 != 0 && 结果头.变更事实代次
        && *结果头.变更事实代次 == 结果头.事实截止代次
        && 方法 && L2普通方法事实完整(*方法, 结果头.事实截止代次);
}
inline bool L2普通方法替换结果::成功() const noexcept {
    return (结果头.状态 == L2结构状态::已提交
            || 结果头.状态 == L2结构状态::精确重复)
        && 结果头.事实截止代次 != 0 && 结果头.变更事实代次
        && 当前方法 && 已退出方法
        && L2普通方法事实完整(*当前方法, 结果头.事实截止代次)
        && L2普通方法事实完整(*已退出方法, 结果头.事实截止代次);
}
inline bool L2普通方法退出结果::成功() const noexcept {
    return (结果头.状态 == L2结构状态::已提交
            || 结果头.状态 == L2结构状态::精确重复)
        && 已退出方法 && L2普通方法事实完整(*已退出方法,
            结果头.事实截止代次);
}
inline bool L2普通方法读取结果::成功() const noexcept {
    const auto 截止 = 读取类别 == L2读取类别::当前
        ? 结果头.事实截止代次 : 历史截止事实代次;
    return 结果头.状态 == L2结构状态::已读取 && !结果头.变更事实代次
        && 方法 && L2普通方法事实完整(*方法, 截止);
}
inline bool L2方法用途粗召回结果::成功() const noexcept {
    if (结果头.状态 != L2结构状态::已读取 || 结果头.事实截止代次 == 0
        || 结果头.变更事实代次) return false;
    for (std::size_t i = 0; i < 粗召回.size(); ++i) {
        if (粗召回[i].方法 != 粗召回[i].用途.方法) return false;
        if (i && (粗召回[i - 1].方法.值.值 > 粗召回[i].方法.值.值
            || (粗召回[i - 1].方法 == 粗召回[i].方法
                && 粗召回[i - 1].用途.身份.值.值
                    >= 粗召回[i].用途.身份.值.值))) return false;
    }
    return true;
}
inline bool L2方法静态能力投影读取结果::成功() const noexcept {
    if (结果头.状态 != L2结构状态::已读取 || 结果头.事实截止代次 == 0
        || 结果头.变更事实代次) return false;
    for (std::size_t i = 0; i < 投影.size(); ++i) {
        const auto& 项 = 投影[i];
        if (!有效(项.方法.值) || !L2方法内容版本有效(项.内容版本)
            || !L2方法规格版本有效(项.规格版本)
            || !L2方法结果身份有效(项.主轴结果)
            || !L2方法用途身份有效(项.匹配用途)
            || !L2方法六项静态能力签名完整(项.签名)) return false;
        if (i != 0 && !L2方法六项静态能力投影小于(投影[i - 1], 项))
            return false;
    }
    for (const auto& 项 : 结构缺口)
        if (!有效(项.方法.值)
            || (项.条件 && !L2方法条件身份有效(*项.条件))
            || (项.结果 && !L2方法结果身份有效(*项.结果))) return false;
    return true;
}

} // namespace 海中鱼巣
