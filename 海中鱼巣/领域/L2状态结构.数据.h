#pragma once

#ifndef L2_STATE_STRUCTURE_NO_INCLUDES
#include <algorithm>
#include <cstdint>
#include <optional>
#include <type_traits>
#include <variant>
#include <vector>
#include "L2结构公共.数据.h"
#include "L2特征结构.数据.h"
#include "../核心/L1所有者范围CRUD.数据.h"
#endif

namespace 海中鱼巣 {

inline constexpr std::uint32_t L2状态结构类型登记规则版本 = 1;

#define 定义L2状态结构类型身份(类型名) \
    struct 类型名 final { \
        稳定编码 值; \
        explicit 类型名(稳定编码 编码 = {}) noexcept : 值(编码) {} \
        friend bool operator==(const 类型名&, const 类型名&) = default; \
    }

定义L2状态结构类型身份(L2状态主体关系类型身份);
定义L2状态结构类型身份(L2状态特征实例关系类型身份);
定义L2状态结构类型身份(L2状态当前选择关系类型身份);
定义L2状态结构类型身份(L2状态精确I64值类型身份);
定义L2状态结构类型身份(L2状态精确I64组值类型身份);
定义L2状态结构类型身份(L2状态精确U64组值类型身份);
定义L2状态结构类型身份(L2状态精确独立材料值类型身份);
定义L2状态结构类型身份(L2状态时间I64值类型身份);
定义L2状态结构类型身份(L2状态时间I64组值类型身份);
定义L2状态结构类型身份(L2状态时间U64组值类型身份);
定义L2状态结构类型身份(L2状态时间独立材料值类型身份);

#undef 定义L2状态结构类型身份

enum class L2状态结构类型登记状态 : std::uint8_t {
    已提交 = 1,
    精确重复 = 2,
    已读取 = 3,
    登记未加载 = 4,
    入口拒绝 = 5,
    许可拒绝 = 6,
    事实代次漂移 = 7,
    幂等冲突 = 8,
    资源失败 = 9,
    内部不一致 = 10
};

struct L2状态结构类型登记请求 final {
    L2结构请求头 请求头;
    std::uint32_t 规则版本 = L2状态结构类型登记规则版本;
    L2结构幂等身份 幂等身份{1};
    friend bool operator==(const L2状态结构类型登记请求&,
        const L2状态结构类型登记请求&) = default;
};

struct L2状态结构类型登记 final {
    std::uint32_t 合同版本 = L2结构合同版本;
    std::uint32_t 规则版本 = L2状态结构类型登记规则版本;
    L2结构幂等身份 首次幂等身份{1};
    std::uint64_t 首次期望事实代次 = 0;
    std::uint64_t 建立事实代次 = 0;
    std::uint64_t 事实截止代次 = 0;
    L2状态主体关系类型身份 主体关系类型;
    L2状态特征实例关系类型身份 特征实例关系类型;
    L2状态当前选择关系类型身份 当前选择关系类型;
    L2状态精确I64值类型身份 精确I64值类型;
    L2状态精确I64组值类型身份 精确I64组值类型;
    L2状态精确U64组值类型身份 精确U64组值类型;
    L2状态精确独立材料值类型身份 精确独立材料值类型;
    L2状态时间I64值类型身份 时间I64值类型;
    L2状态时间I64组值类型身份 时间I64组值类型;
    L2状态时间U64组值类型身份 时间U64组值类型;
    L2状态时间独立材料值类型身份 时间独立材料值类型;
    friend bool operator==(const L2状态结构类型登记&,
        const L2状态结构类型登记&) = default;
};

struct L2状态结构类型登记结果 final {
    L2状态结构类型登记状态 状态 = L2状态结构类型登记状态::入口拒绝;
    std::uint64_t 事实截止代次 = 0;
    std::optional<L2状态结构类型登记> 登记;
    bool 成功() const noexcept;
    friend bool operator==(const L2状态结构类型登记结果&,
        const L2状态结构类型登记结果&) = default;
};

inline constexpr std::uint32_t L2目标状态合同规则版本 = 1;
inline constexpr std::uint32_t L2目标状态合同结构类型登记规则版本 = 1;

#define 定义L2目标状态合同类型身份(类型名) \
    struct 类型名 final { \
        稳定编码 值; \
        explicit 类型名(稳定编码 编码 = {}) noexcept : 值(编码) {} \
        friend bool operator==(const 类型名&, const 类型名&) = default; \
    }

定义L2目标状态合同类型身份(L2目标状态合同身份);
定义L2目标状态合同类型身份(L2目标状态特征定义关系类型身份);
定义L2目标状态合同类型身份(L2目标状态比较注册关系类型身份);
定义L2目标状态合同类型身份(L2目标状态I64值类型身份);
定义L2目标状态合同类型身份(L2目标状态U64合同属性类型身份);

#undef 定义L2目标状态合同类型身份

enum class L2目标状态合同结构类型登记状态 : std::uint8_t {
    已提交 = 1,
    精确重复 = 2,
    已读取 = 3,
    登记未加载 = 4,
    入口拒绝 = 5,
    许可拒绝 = 6,
    事实代次漂移 = 7,
    幂等冲突 = 8,
    资源失败 = 9,
    内部不一致 = 10
};

struct L2目标状态合同结构类型登记请求 final {
    L2结构请求头 请求头;
    std::uint32_t 规则版本 = L2目标状态合同结构类型登记规则版本;
    L2结构幂等身份 幂等身份{1};
    friend bool operator==(const L2目标状态合同结构类型登记请求&,
        const L2目标状态合同结构类型登记请求&) = default;
};

struct L2目标状态合同结构类型登记 final {
    std::uint32_t 合同版本 = L2结构合同版本;
    std::uint32_t 规则版本 = L2目标状态合同结构类型登记规则版本;
    L2结构幂等身份 首次幂等身份{1};
    std::uint64_t 首次期望事实代次 = 0;
    std::uint64_t 建立事实代次 = 0;
    std::uint64_t 事实截止代次 = 0;
    L2目标状态特征定义关系类型身份 特征定义关系类型;
    L2目标状态比较注册关系类型身份 比较注册关系类型;
    L2目标状态I64值类型身份 I64值类型;
    L2目标状态U64合同属性类型身份 U64合同属性类型;
    friend bool operator==(const L2目标状态合同结构类型登记&,
        const L2目标状态合同结构类型登记&) = default;
};

struct L2目标状态合同结构类型登记结果 final {
    L2目标状态合同结构类型登记状态 状态 =
        L2目标状态合同结构类型登记状态::入口拒绝;
    std::uint64_t 事实截止代次 = 0;
    std::optional<L2目标状态合同结构类型登记> 登记;
    bool 成功() const noexcept;
    friend bool operator==(const L2目标状态合同结构类型登记结果&,
        const L2目标状态合同结构类型登记结果&) = default;
};

struct L2目标状态合同事实 final {
    L2目标状态合同身份 身份;
    L2特征定义身份 特征定义;
    L2特征比较注册身份 比较注册;
    L2特征比较用途 比较用途 = L2特征比较用途::目标判断;
    std::int64_t 目标I64 = 0;
    std::uint8_t 允许关系位 = 0;
    std::uint32_t 规则版本 = L2目标状态合同规则版本;
    L2结构幂等身份 首次幂等身份;
    std::uint64_t 首次期望事实代次 = 0;
    稳定编码 来源稳定编码;
    稳定编码 特征定义关系稳定编码;
    稳定编码 比较注册关系稳定编码;
    稳定编码 目标值稳定编码;
    稳定编码 合同值稳定编码;
    L2生命周期 生命周期;
    friend bool operator==(const L2目标状态合同事实&,
        const L2目标状态合同事实&) = default;
};

struct L2目标状态合同新增请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2特征定义身份 特征定义;
    L2特征比较注册身份 比较注册;
    std::int64_t 目标I64 = 0;
    std::uint8_t 允许关系位 = 0;
    std::uint32_t 规则版本 = L2目标状态合同规则版本;
    稳定编码 来源稳定编码;
    friend bool operator==(const L2目标状态合同新增请求&,
        const L2目标状态合同新增请求&) = default;
};

struct L2目标状态合同替换请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2目标状态合同身份 旧合同;
    L2特征定义身份 特征定义;
    L2特征比较注册身份 比较注册;
    std::int64_t 新目标I64 = 0;
    std::uint8_t 新允许关系位 = 0;
    std::uint32_t 新规则版本 = L2目标状态合同规则版本;
    稳定编码 来源稳定编码;
    friend bool operator==(const L2目标状态合同替换请求&,
        const L2目标状态合同替换请求&) = default;
};

struct L2目标状态合同退出请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2目标状态合同身份 合同;
    稳定编码 特征定义关系稳定编码;
    稳定编码 比较注册关系稳定编码;
    稳定编码 目标值稳定编码;
    稳定编码 合同值稳定编码;
    friend bool operator==(const L2目标状态合同退出请求&,
        const L2目标状态合同退出请求&) = default;
};

struct L2目标状态合同完整读取请求 final {
    L2结构请求头 请求头;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2目标状态合同身份 合同;
    std::uint64_t 历史截止事实代次 = 0;
    friend bool operator==(const L2目标状态合同完整读取请求&,
        const L2目标状态合同完整读取请求&) = default;
};

struct L2目标状态合同新增结果 final {
    L2结构结果头 结果头;
    std::optional<L2目标状态合同事实> 当前合同;
    bool 成功() const noexcept;
    friend bool operator==(const L2目标状态合同新增结果&,
        const L2目标状态合同新增结果&) = default;
};

struct L2目标状态合同替换结果 final {
    L2结构结果头 结果头;
    std::optional<L2目标状态合同事实> 当前合同;
    std::optional<L2目标状态合同事实> 已退出合同;
    bool 成功() const noexcept;
    friend bool operator==(const L2目标状态合同替换结果&,
        const L2目标状态合同替换结果&) = default;
};

struct L2目标状态合同退出结果 final {
    L2结构结果头 结果头;
    std::optional<L2目标状态合同事实> 已退出合同;
    bool 成功() const noexcept;
    friend bool operator==(const L2目标状态合同退出结果&,
        const L2目标状态合同退出结果&) = default;
};

struct L2目标状态合同完整读取结果 final {
    L2结构结果头 结果头;
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止事实代次 = 0;
    std::optional<L2目标状态合同事实> 合同;
    bool 成功() const noexcept;
    friend bool operator==(const L2目标状态合同完整读取结果&,
        const L2目标状态合同完整读取结果&) = default;
};

struct L2状态事实 final {
    L2状态身份 身份;
    L2存在身份 主体存在;
    L2特征实例身份 特征实例;
    L2属性事实 精确值;
    L2属性事实 时间值;
    稳定编码 来源稳定编码;
    L2生命周期 生命周期;
    friend bool operator==(const L2状态事实&, const L2状态事实&) = default;
};

struct L2状态自有关系引用 final {
    稳定编码 族归属关系稳定编码;
    稳定编码 主体关系稳定编码;
    稳定编码 特征实例关系稳定编码;
    friend bool operator==(const L2状态自有关系引用&,
        const L2状态自有关系引用&) = default;
};

inline constexpr std::uint32_t L2中性状态动态合同版本_v1 = 1;

enum class L2中性材料类别_v1 : std::uint8_t {
    实例 = 1,
    抽象 = 2
};

enum class L2中性时间语义_v1 : std::uint8_t {
    绝对UTC纳秒 = 1,
    非负相对纳秒 = 2
};

enum class L2中性事实种类_v1 : std::uint8_t {
    状态 = 1,
    动态 = 2,
    普通引用 = 3,
    当前选择引用 = 4
};

enum class L2中性结构状态_v1 : std::uint8_t {
    已提交 = 1,
    精确重复 = 2,
    已读取 = 3,
    已退出 = 4,
    已清理 = 5,
    未找到 = 6,
    无须清理 = 7,
    入口拒绝 = 8,
    事实代次漂移 = 9,
    幂等冲突 = 10,
    引用冲突 = 11,
    动态不闭合 = 12,
    历史材料已清理 = 13,
    资源失败 = 14,
    内部错误 = 15
};

struct L2中性时间_v1 final {
    L2中性时间语义_v1 语义 = L2中性时间语义_v1::绝对UTC纳秒;
    std::int64_t 纳秒 = 0;
    friend bool operator==(const L2中性时间_v1&,
        const L2中性时间_v1&) = default;
};

struct L2中性结构结果头_v1 final {
    std::uint32_t 合同版本 = L2中性状态动态合同版本_v1;
    L2中性结构状态_v1 状态 = L2中性结构状态_v1::内部错误;
    std::uint64_t 事实截止代次 = 0;
    std::optional<std::uint64_t> 变更事实代次{};
    friend bool operator==(const L2中性结构结果头_v1&,
        const L2中性结构结果头_v1&) = default;
};

struct L2中性结构墓碑_v1 final {
    稳定编码 稳定身份{};
    L2中性事实种类_v1 事实种类 = L2中性事实种类_v1::状态;
    稳定编码 内部结构分区{};
    std::uint64_t 创建事实代次 = 0;
    std::optional<std::uint64_t> 退出事实代次{};
    std::uint64_t 清理事实代次 = 0;
    friend bool operator==(const L2中性结构墓碑_v1&,
        const L2中性结构墓碑_v1&) = default;
};

struct L2中性状态事实_v1 final {
    L2状态身份 身份{};
    L2中性材料类别_v1 材料类别 = L2中性材料类别_v1::实例;
    L2特征定义身份 特征类型{};
    L2原始值材料 特征值{};
    L2中性时间_v1 时间{};
    std::int64_t 首次形成UTC纳秒 = 0;
    std::uint64_t 形成事实代次 = 0;
    friend bool operator==(const L2中性状态事实_v1&,
        const L2中性状态事实_v1&) = default;
};

struct L2中性状态新增请求_v1 final {
    L2结构请求头 请求头{};
    std::uint32_t 专属合同版本 = L2中性状态动态合同版本_v1;
    L2结构幂等身份 幂等身份{};
    L2中性材料类别_v1 材料类别 = L2中性材料类别_v1::实例;
    L2特征定义身份 特征类型{};
    L2原始值材料 特征值{};
    L2中性时间_v1 时间{};
    friend bool operator==(const L2中性状态新增请求_v1&,
        const L2中性状态新增请求_v1&) = default;
};

struct L2中性状态读取请求_v1 final {
    L2结构请求头 请求头{};
    std::uint32_t 专属合同版本 = L2中性状态动态合同版本_v1;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2状态身份 状态{};
    std::uint64_t 历史截止事实代次 = 0;
    friend bool operator==(const L2中性状态读取请求_v1&,
        const L2中性状态读取请求_v1&) = default;
};

struct L2中性状态退出请求_v1 final {
    L2结构请求头 请求头{};
    std::uint32_t 专属合同版本 = L2中性状态动态合同版本_v1;
    L2结构幂等身份 幂等身份{};
    L2状态身份 状态{};
    friend bool operator==(const L2中性状态退出请求_v1&,
        const L2中性状态退出请求_v1&) = default;
};

struct L2中性状态写入结果_v1 final {
    L2中性结构结果头_v1 结果头{};
    std::optional<L2中性状态事实_v1> 状态{};
    bool 成功() const noexcept;
    friend bool operator==(const L2中性状态写入结果_v1&,
        const L2中性状态写入结果_v1&) = default;
};

struct L2中性状态读取结果_v1 final {
    L2中性结构结果头_v1 结果头{};
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止事实代次 = 0;
    std::optional<L2中性状态事实_v1> 状态{};
    std::optional<L2中性结构墓碑_v1> 墓碑{};
    bool 成功() const noexcept;
    friend bool operator==(const L2中性状态读取结果_v1&,
        const L2中性状态读取结果_v1&) = default;
};

struct L2中性状态退出结果_v1 final {
    L2中性结构结果头_v1 结果头{};
    std::optional<L2中性状态事实_v1> 已退出状态{};
    bool 成功() const noexcept;
    friend bool operator==(const L2中性状态退出结果_v1&,
        const L2中性状态退出结果_v1&) = default;
};

struct L2中性状态引用身份_v1 final {
    稳定编码 值{};
    friend bool operator==(const L2中性状态引用身份_v1&,
        const L2中性状态引用身份_v1&) = default;
};

enum class L2中性状态引用类别_v1 : std::uint8_t {
    普通 = 1,
    当前选择 = 2
};

struct L2中性状态引用事实_v1 final {
    L2中性状态引用身份_v1 身份{};
    L2中性状态引用类别_v1 类别 = L2中性状态引用类别_v1::普通;
    稳定编码 引用方{};
    稳定编码 引用角色{};
    L2状态身份 目标状态{};
    L2生命周期 生命周期{};
    friend bool operator==(const L2中性状态引用事实_v1& 左,
        const L2中性状态引用事实_v1& 右) noexcept {
        return 左.身份 == 右.身份 && 左.类别 == 右.类别
            && 左.引用方 == 右.引用方 && 左.引用角色 == 右.引用角色
            && 左.目标状态 == 右.目标状态
            && 左.生命周期.创建事实代次 == 右.生命周期.创建事实代次
            && 左.生命周期.退出事实代次 == 右.生命周期.退出事实代次;
    }
};

struct L2中性状态普通引用建立请求_v1 final {
    L2结构请求头 请求头{};
    std::uint32_t 专属合同版本 = L2中性状态动态合同版本_v1;
    L2结构幂等身份 幂等身份{};
    稳定编码 引用方{};
    稳定编码 引用角色{};
    L2状态身份 目标状态{};
    friend bool operator==(const L2中性状态普通引用建立请求_v1&,
        const L2中性状态普通引用建立请求_v1&) = default;
};

struct L2中性状态当前选择设置请求_v1 final {
    L2结构请求头 请求头{};
    std::uint32_t 专属合同版本 = L2中性状态动态合同版本_v1;
    L2结构幂等身份 幂等身份{};
    稳定编码 引用方{};
    稳定编码 引用角色{};
    std::optional<L2中性状态引用身份_v1> 预期旧当前选择{};
    L2状态身份 新当前状态{};
    friend bool operator==(const L2中性状态当前选择设置请求_v1&,
        const L2中性状态当前选择设置请求_v1&) = default;
};

struct L2中性状态引用退出请求_v1 final {
    L2结构请求头 请求头{};
    std::uint32_t 专属合同版本 = L2中性状态动态合同版本_v1;
    L2结构幂等身份 幂等身份{};
    L2中性状态引用身份_v1 引用{};
    friend bool operator==(const L2中性状态引用退出请求_v1&,
        const L2中性状态引用退出请求_v1&) = default;
};

struct L2中性状态引用读取请求_v1 final {
    L2结构请求头 请求头{};
    std::uint32_t 专属合同版本 = L2中性状态动态合同版本_v1;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2中性状态引用身份_v1 引用{};
    std::uint64_t 历史截止事实代次 = 0;
    friend bool operator==(const L2中性状态引用读取请求_v1&,
        const L2中性状态引用读取请求_v1&) = default;
};

struct L2中性状态当前选择读取请求_v1 final {
    L2结构请求头 请求头{};
    std::uint32_t 专属合同版本 = L2中性状态动态合同版本_v1;
    L2读取类别 读取类别 = L2读取类别::当前;
    稳定编码 引用方{};
    稳定编码 引用角色{};
    std::uint64_t 历史截止事实代次 = 0;
    friend bool operator==(const L2中性状态当前选择读取请求_v1&,
        const L2中性状态当前选择读取请求_v1&) = default;
};

struct L2中性状态引用写入结果_v1 final {
    L2中性结构结果头_v1 结果头{};
    std::optional<L2中性状态引用事实_v1> 当前引用{};
    std::optional<L2中性状态引用事实_v1> 已退出旧当前选择{};
    bool 成功() const noexcept;
    friend bool operator==(const L2中性状态引用写入结果_v1&,
        const L2中性状态引用写入结果_v1&) = default;
};

struct L2中性状态引用读取结果_v1 final {
    L2中性结构结果头_v1 结果头{};
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止事实代次 = 0;
    std::optional<L2中性状态引用事实_v1> 引用{};
    std::optional<L2中性状态事实_v1> 目标状态{};
    std::optional<L2中性结构墓碑_v1> 目标墓碑{};
    bool 成功() const noexcept;
    friend bool operator==(const L2中性状态引用读取结果_v1&,
        const L2中性状态引用读取结果_v1&) = default;
};

inline constexpr std::uint32_t L2中性实例材料保留策略版本_v1 = 1;

struct L2中性实例材料保留策略_v1 final {
    std::uint32_t 策略版本 = L2中性实例材料保留策略版本_v1;
    std::uint64_t 实例状态TTL纳秒 = 0;
    std::uint64_t 实例动态TTL纳秒 = 0;
    std::uint64_t 实例状态总量上限 = 0;
    std::uint64_t 实例动态总量上限 = 0;
    friend bool operator==(const L2中性实例材料保留策略_v1&,
        const L2中性实例材料保留策略_v1&) = default;
};

struct L2中性实例状态清理阶段请求_v1 final {
    std::uint32_t 专属合同版本 = L2中性状态动态合同版本_v1;
    L2结构请求头 请求头{};
    std::int64_t 本次绝对UTC纳秒 = 0;
    std::vector<L2状态身份> 完整活动动态成员状态身份规范组{};
    std::uint64_t 活动动态成员覆盖事实截止代次 = 0;
    L2结构幂等身份 清理幂等身份{};
    friend bool operator==(const L2中性实例状态清理阶段请求_v1&,
        const L2中性实例状态清理阶段请求_v1&) = default;
};

struct L2中性实例状态清理阶段结果_v1 final {
    L2中性结构结果头_v1 结果头{};
    std::vector<L2中性结构墓碑_v1> 已清理状态{};
    std::vector<L2中性状态引用身份_v1> 已退出当前选择{};
    std::uint64_t 清理前实例状态数量 = 0;
    std::uint64_t 清理后实例状态数量 = 0;
    bool 成功() const noexcept;
    friend bool operator==(const L2中性实例状态清理阶段结果_v1&,
        const L2中性实例状态清理阶段结果_v1&) = default;
};

inline constexpr L2中性实例材料保留策略_v1
    L2中性实例材料默认保留策略_v1{
        L2中性实例材料保留策略版本_v1,
        604'800'000'000'000ULL,
        604'800'000'000'000ULL,
        1'000'000ULL,
        250'000ULL};

inline bool L2中性原始值材料有效_v1(const L2原始值材料& 材料) noexcept {
    return std::visit([](const auto& 值) noexcept {
        using 类型 = std::decay_t<decltype(值)>;
        if constexpr (std::is_same_v<类型, std::int64_t>) return true;
        else if constexpr (std::is_same_v<类型, L2独立材料引用>)
            return 有效(值.编码);
        else return !值.empty();
    }, 材料);
}

inline bool L2中性时间有效_v1(
    L2中性材料类别_v1 类别, const L2中性时间_v1& 时间) noexcept {
    return (类别 == L2中性材料类别_v1::实例
            && 时间.语义 == L2中性时间语义_v1::绝对UTC纳秒
            && 时间.纳秒 > 0)
        || (类别 == L2中性材料类别_v1::抽象
            && 时间.语义 == L2中性时间语义_v1::非负相对纳秒
            && 时间.纳秒 >= 0);
}

inline bool L2中性实例材料保留策略有效_v1(
    const L2中性实例材料保留策略_v1& 策略) noexcept {
    return 策略.策略版本 == L2中性实例材料保留策略版本_v1
        && 策略.实例状态TTL纳秒 != 0 && 策略.实例动态TTL纳秒 != 0
        && 策略.实例状态总量上限 != 0 && 策略.实例动态总量上限 != 0;
}

inline bool L2中性状态事实完整_v1(
    const L2中性状态事实_v1& 状态) noexcept {
    return 有效(状态.身份.值) && 有效(状态.特征类型.值)
        && L2中性原始值材料有效_v1(状态.特征值)
        && L2中性时间有效_v1(状态.材料类别, 状态.时间)
        && 状态.首次形成UTC纳秒 > 0 && 状态.形成事实代次 != 0;
}

inline bool L2中性状态新增请求有效_v1(
    const L2中性状态新增请求_v1& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && 请求.专属合同版本 == L2中性状态动态合同版本_v1
        && L2结构幂等身份有效(请求.幂等身份)
        && 有效(请求.特征类型.值)
        && L2中性原始值材料有效_v1(请求.特征值)
        && L2中性时间有效_v1(请求.材料类别, 请求.时间);
}

inline bool L2中性状态读取请求有效_v1(
    const L2中性状态读取请求_v1& 请求) noexcept {
    const bool 截止有效 = 请求.读取类别 == L2读取类别::当前
        ? 请求.历史截止事实代次 == 0
        : 请求.读取类别 == L2读取类别::历史
            && 请求.历史截止事实代次 != 0
            && 请求.历史截止事实代次 <= 请求.请求头.期望事实代次;
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && 请求.专属合同版本 == L2中性状态动态合同版本_v1
        && 有效(请求.状态.值) && 截止有效;
}

inline bool L2中性状态退出请求有效_v1(
    const L2中性状态退出请求_v1& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && 请求.专属合同版本 == L2中性状态动态合同版本_v1
        && L2结构幂等身份有效(请求.幂等身份)
        && 有效(请求.状态.值);
}

inline bool L2中性状态普通引用建立请求有效_v1(
    const L2中性状态普通引用建立请求_v1& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && 请求.专属合同版本 == L2中性状态动态合同版本_v1
        && L2结构幂等身份有效(请求.幂等身份)
        && 有效(请求.引用方) && 有效(请求.引用角色)
        && 有效(请求.目标状态.值);
}

inline bool L2中性状态当前选择设置请求有效_v1(
    const L2中性状态当前选择设置请求_v1& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && 请求.专属合同版本 == L2中性状态动态合同版本_v1
        && L2结构幂等身份有效(请求.幂等身份)
        && 有效(请求.引用方) && 有效(请求.引用角色)
        && (!请求.预期旧当前选择 || 有效(请求.预期旧当前选择->值))
        && 有效(请求.新当前状态.值);
}

inline bool L2中性状态引用退出请求有效_v1(
    const L2中性状态引用退出请求_v1& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && 请求.专属合同版本 == L2中性状态动态合同版本_v1
        && L2结构幂等身份有效(请求.幂等身份) && 有效(请求.引用.值);
}

inline bool L2中性状态引用读取请求有效_v1(
    const L2中性状态引用读取请求_v1& 请求) noexcept {
    const bool 截止有效 = 请求.读取类别 == L2读取类别::当前
        ? 请求.历史截止事实代次 == 0
        : 请求.读取类别 == L2读取类别::历史
            && 请求.历史截止事实代次 != 0
            && 请求.历史截止事实代次 <= 请求.请求头.期望事实代次;
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && 请求.专属合同版本 == L2中性状态动态合同版本_v1
        && 有效(请求.引用.值) && 截止有效;
}

inline bool L2中性状态当前选择读取请求有效_v1(
    const L2中性状态当前选择读取请求_v1& 请求) noexcept {
    const bool 截止有效 = 请求.读取类别 == L2读取类别::当前
        ? 请求.历史截止事实代次 == 0
        : 请求.读取类别 == L2读取类别::历史
            && 请求.历史截止事实代次 != 0
            && 请求.历史截止事实代次 <= 请求.请求头.期望事实代次;
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && 请求.专属合同版本 == L2中性状态动态合同版本_v1
        && 有效(请求.引用方) && 有效(请求.引用角色) && 截止有效;
}

inline bool L2中性实例状态清理阶段请求有效_v1(
    const L2中性实例状态清理阶段请求_v1& 请求) noexcept {
    if (请求.专属合同版本 != L2中性状态动态合同版本_v1
        || !L2结构请求头合同有效(请求.请求头)
        || 请求.请求头.期望事实代次 == 0
        || 请求.本次绝对UTC纳秒 <= 0
        || 请求.活动动态成员覆盖事实截止代次
            != 请求.请求头.期望事实代次
        || !L2结构幂等身份有效(请求.清理幂等身份)) return false;
    for (std::size_t i = 0;
        i < 请求.完整活动动态成员状态身份规范组.size(); ++i) {
        if (!有效(请求.完整活动动态成员状态身份规范组[i].值)
            || (i != 0 && !(请求.完整活动动态成员状态身份规范组[i - 1].值
                < 请求.完整活动动态成员状态身份规范组[i].值))) return false;
    }
    return true;
}

inline bool L2中性状态引用事实完整_v1(
    const L2中性状态引用事实_v1& 引用) noexcept {
    return 有效(引用.身份.值) && 有效(引用.引用方)
        && 有效(引用.引用角色) && 有效(引用.目标状态.值)
        && (引用.类别 == L2中性状态引用类别_v1::普通
            || 引用.类别 == L2中性状态引用类别_v1::当前选择)
        && L2生命周期完整(引用.生命周期);
}

inline bool L2中性状态写入结果_v1::成功() const noexcept {
    return (结果头.状态 == L2中性结构状态_v1::已提交
            || 结果头.状态 == L2中性结构状态_v1::精确重复)
        && 结果头.合同版本 == L2中性状态动态合同版本_v1
        && 结果头.事实截止代次 != 0 && 结果头.变更事实代次
        && *结果头.变更事实代次 <= 结果头.事实截止代次
        && 状态 && L2中性状态事实完整_v1(*状态)
        && 状态->形成事实代次 == *结果头.变更事实代次;
}

inline bool L2中性状态读取结果_v1::成功() const noexcept {
    return 结果头.状态 == L2中性结构状态_v1::已读取
        && 结果头.事实截止代次 != 0 && !结果头.变更事实代次
        && 状态 && !墓碑 && L2中性状态事实完整_v1(*状态);
}

inline bool L2中性状态退出结果_v1::成功() const noexcept {
    return (结果头.状态 == L2中性结构状态_v1::已退出
            || 结果头.状态 == L2中性结构状态_v1::精确重复)
        && 结果头.事实截止代次 != 0 && 结果头.变更事实代次
        && *结果头.变更事实代次 <= 结果头.事实截止代次
        && 已退出状态 && L2中性状态事实完整_v1(*已退出状态);
}

inline bool L2中性状态引用写入结果_v1::成功() const noexcept {
    return (结果头.状态 == L2中性结构状态_v1::已提交
            || 结果头.状态 == L2中性结构状态_v1::已退出
            || 结果头.状态 == L2中性结构状态_v1::精确重复)
        && 结果头.事实截止代次 != 0 && 结果头.变更事实代次
        && *结果头.变更事实代次 <= 结果头.事实截止代次
        && 当前引用 && L2中性状态引用事实完整_v1(*当前引用);
}

inline bool L2中性状态引用读取结果_v1::成功() const noexcept {
    return 结果头.状态 == L2中性结构状态_v1::已读取
        && 结果头.事实截止代次 != 0 && !结果头.变更事实代次
        && 引用 && 目标状态 && !目标墓碑
        && L2中性状态引用事实完整_v1(*引用)
        && L2中性状态事实完整_v1(*目标状态)
        && 引用->目标状态 == 目标状态->身份;
}

inline bool L2中性实例状态清理阶段结果_v1::成功() const noexcept {
    if (结果头.合同版本 != L2中性状态动态合同版本_v1
        || 结果头.事实截止代次 == 0
        || 清理后实例状态数量 > 清理前实例状态数量) return false;
    if (结果头.状态 == L2中性结构状态_v1::无须清理)
        return !结果头.变更事实代次 && 已清理状态.empty()
            && 已退出当前选择.empty()
            && 清理前实例状态数量 == 清理后实例状态数量;
    return (结果头.状态 == L2中性结构状态_v1::已清理
            || 结果头.状态 == L2中性结构状态_v1::精确重复)
        && 结果头.变更事实代次
        && *结果头.变更事实代次 <= 结果头.事实截止代次
        && !已清理状态.empty()
        && 清理前实例状态数量 - 清理后实例状态数量
            == 已清理状态.size();
}

inline constexpr std::uint32_t L2状态首态专属合同版本_v2 = 2;
inline constexpr std::int64_t L2状态当前选择关系角色值 = 1;

struct L2状态当前选择关系引用 final {
    稳定编码 关系稳定编码{};
    L2特征实例身份 特征实例{稳定编码{}};
    L2状态身份 状态{稳定编码{}};
    L2状态当前选择关系类型身份 关系类型{稳定编码{}};
    std::int64_t 角色或顺序 = L2状态当前选择关系角色值;
    L1结构所有者身份 写入所有者{};
    L2生命周期 生命周期{};
    friend bool operator==(const L2状态当前选择关系引用&,
        const L2状态当前选择关系引用&) = default;
};

struct L2状态首态请求头_v2 final {
    L2结构请求头 公共请求头;
    std::uint32_t 专属合同版本 = L2状态首态专属合同版本_v2;
    friend bool operator==(const L2状态首态请求头_v2&,
        const L2状态首态请求头_v2&) = default;
};

struct L2状态首态请求_v2 final {
    L2状态首态请求头_v2 请求头;
    L2结构幂等身份 幂等身份;
    L2存在身份 主体存在;
    L2特征实例身份 特征实例;
    L2原始值材料 精确值材料;
    L2原始值材料 时间值材料;
    稳定编码 来源稳定编码;
    friend bool operator==(const L2状态首态请求_v2&,
        const L2状态首态请求_v2&) = default;
};

enum class L2状态首态操作状态_v2 : std::uint8_t {
    已提交 = 1,
    精确重复 = 2,
    已有当前选择 = 3,
    待迁移 = 4,
    入口拒绝 = 8,
    许可拒绝 = 9,
    事实代次漂移 = 10,
    幂等冲突 = 11,
    引用冲突 = 12,
    资源失败 = 13,
    内部不一致 = 14,
    已可能发布 = 15
};

struct L2状态首态结果_v2 final {
    L2结构结果头 结果头;
    std::uint32_t 专属合同版本 = L2状态首态专属合同版本_v2;
    L2状态首态操作状态_v2 操作状态 = L2状态首态操作状态_v2::入口拒绝;
    std::optional<L2状态事实> 状态;
    std::optional<L2状态自有关系引用> 自有关系;
    std::optional<L2状态当前选择关系引用> 当前选择关系;
    bool 成功() const noexcept;
    friend bool operator==(const L2状态首态结果_v2&,
        const L2状态首态结果_v2&) = default;
};

enum class L2状态当前选择读取状态 : std::uint8_t {
    已读取 = 3,
    待迁移 = 4,
    入口拒绝 = 8,
    许可拒绝 = 9,
    事实代次漂移 = 10,
    资源失败 = 13,
    内部不一致 = 14
};

struct L2状态当前选择读取请求 final {
    L2结构请求头 请求头;
    L2特征实例身份 特征实例;
    friend bool operator==(const L2状态当前选择读取请求&,
        const L2状态当前选择读取请求&) = default;
};

struct L2状态当前选择读取结果 final {
    L2结构结果头 结果头;
    L2状态当前选择读取状态 读取状态 = L2状态当前选择读取状态::入口拒绝;
    std::optional<L2状态当前选择关系引用> 当前选择关系;
    std::optional<L2状态事实> 当前状态;
    bool 成功() const noexcept;
    friend bool operator==(const L2状态当前选择读取结果&,
        const L2状态当前选择读取结果&) = default;
};

enum class L2状态历史选择读取状态 : std::uint8_t {
    已读取 = 3,
    入口拒绝 = 8,
    许可拒绝 = 9,
    事实代次漂移 = 10,
    资源失败 = 13,
    内部不一致 = 14
};

struct L2状态历史选择读取请求 final {
    L2结构请求头 请求头;
    L2特征实例身份 特征实例;
    std::uint64_t 历史截止事实代次 = 0;
    friend bool operator==(const L2状态历史选择读取请求&,
        const L2状态历史选择读取请求&) = default;
};

struct L2状态历史选择读取结果 final {
    L2结构结果头 结果头;
    L2状态历史选择读取状态 读取状态 = L2状态历史选择读取状态::入口拒绝;
    std::uint64_t 历史截止事实代次 = 0;
    std::optional<L2状态当前选择关系引用> 当前选择关系;
    std::optional<L2状态事实> 状态;
    bool 成功() const noexcept;
    friend bool operator==(const L2状态历史选择读取结果&,
        const L2状态历史选择读取结果&) = default;
};

struct L2状态身份来源读取请求 final {
    L2结构请求头 请求头;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2状态身份 状态;
    std::uint64_t 历史截止事实代次 = 0;
    friend bool operator==(const L2状态身份来源读取请求&,
        const L2状态身份来源读取请求&) = default;
};

struct L2状态身份来源读取结果 final {
    L2结构结果头 结果头;
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止事实代次 = 0;
    std::optional<L2节点族身份来源事实> 身份来源;
    bool 成功() const noexcept;
    friend bool operator==(const L2状态身份来源读取结果&,
        const L2状态身份来源读取结果&) = default;
};

struct L2状态新增请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2存在身份 主体存在;
    L2特征实例身份 特征实例;
    L2原始值材料 精确值材料;
    L2原始值材料 时间值材料;
    稳定编码 来源稳定编码;
    friend bool operator==(const L2状态新增请求&,
        const L2状态新增请求&) = default;
};

struct L2状态写入结果 final {
    L2结构结果头 结果头;
    std::optional<L2状态事实> 状态;
    std::optional<L2状态自有关系引用> 自有关系;
    bool 成功() const noexcept;
    friend bool operator==(const L2状态写入结果&,
        const L2状态写入结果&) = default;
};

struct L2状态完整读取请求 final {
    L2结构请求头 请求头;
    L2读取类别 读取类别 = L2读取类别::当前;
    L2状态身份 状态;
    std::uint64_t 历史截止事实代次 = 0;
    friend bool operator==(const L2状态完整读取请求&,
        const L2状态完整读取请求&) = default;
};

struct L2状态完整读取结果 final {
    L2结构结果头 结果头;
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止事实代次 = 0;
    std::optional<L2状态事实> 状态;
    std::optional<L2状态自有关系引用> 自有关系;
    friend bool operator==(const L2状态完整读取结果&,
        const L2状态完整读取结果&) = default;
};

struct L2按主体当前状态读取请求 final {
    L2结构请求头 请求头;
    L2存在身份 主体存在;
    std::uint64_t 最大数量 = 0;
    friend bool operator==(const L2按主体当前状态读取请求&,
        const L2按主体当前状态读取请求&) = default;
};

struct L2按特征实例当前状态读取请求 final {
    L2结构请求头 请求头;
    L2特征实例身份 特征实例;
    std::uint64_t 最大数量 = 0;
    friend bool operator==(const L2按特征实例当前状态读取请求&,
        const L2按特征实例当前状态读取请求&) = default;
};

struct L2按来源当前状态读取请求 final {
    L2结构请求头 请求头;
    稳定编码 来源稳定编码;
    std::uint64_t 最大数量 = 0;
    friend bool operator==(const L2按来源当前状态读取请求&,
        const L2按来源当前状态读取请求&) = default;
};

struct L2按I64时间范围当前状态读取请求 final {
    L2结构请求头 请求头;
    std::int64_t 最小时间I64 = 0;
    std::int64_t 最大时间I64 = 0;
    std::uint64_t 最大候选数量 = 0;
    std::uint64_t 最大返回数量 = 0;
    friend bool operator==(const L2按I64时间范围当前状态读取请求&,
        const L2按I64时间范围当前状态读取请求&) = default;
};

struct L2当前状态组读取结果 final {
    L2结构结果头 结果头;
    std::vector<L2状态事实> 状态;
    bool 成功() const noexcept;
    friend bool operator==(const L2当前状态组读取结果&,
        const L2当前状态组读取结果&) = default;
};

enum class L2状态值角色 : std::uint8_t {
    精确值 = 1,
    时间值 = 2
};

struct L2状态引用对关系引用 final {
    稳定编码 主体关系稳定编码;
    稳定编码 特征实例关系稳定编码;
    friend bool operator==(const L2状态引用对关系引用&,
        const L2状态引用对关系引用&) = default;
};

struct L2状态关系退出事实 final {
    稳定编码 关系稳定编码;
    L2生命周期 生命周期;
    friend bool operator==(const L2状态关系退出事实&,
        const L2状态关系退出事实&) = default;
};

struct L2状态引用对关系退出投影 final {
    L2状态关系退出事实 主体关系;
    L2状态关系退出事实 特征实例关系;
    friend bool operator==(const L2状态引用对关系退出投影&,
        const L2状态引用对关系退出投影&) = default;
};

struct L2状态自有关系退出投影 final {
    L2状态关系退出事实 族归属关系;
    L2状态关系退出事实 主体关系;
    L2状态关系退出事实 特征实例关系;
    friend bool operator==(const L2状态自有关系退出投影&,
        const L2状态自有关系退出投影&) = default;
};

struct L2状态原子参与者请求 final {
    std::uint32_t 合同版本 = L2结构合同版本;
    std::uint64_t 共同期望事实代次 = 0;
    L1所有者范围写入幂等身份 组合写入幂等身份;
    L2存在身份 主体存在;
    L2状态身份 旧当前状态;
    L2特征实例身份 新状态特征实例;
    L2原始值材料 新状态精确值材料;
    L2原始值材料 新状态时间值材料;
    稳定编码 来源方法稳定编码;
    L2状态当前选择关系引用 旧当前选择;
    friend bool operator==(const L2状态原子参与者请求&,
        const L2状态原子参与者请求&) = default;
};

struct L2状态原子参与者局部键 final {
    L1所有者范围写集本地键 状态节点{0x02041001};
    L1所有者范围写集本地键 族归属关系{0x02042003};
    L1所有者范围写集本地键 主体关系{0x02041101};
    L1所有者范围写集本地键 特征实例关系{0x02041102};
    L1所有者范围写集本地键 当前选择关系{0x02041103};
    L1所有者范围写集本地键 精确值{0x02041201};
    L1所有者范围写集本地键 时间值{0x02041202};
    friend bool operator==(const L2状态原子参与者局部键&,
        const L2状态原子参与者局部键&) = default;
};

struct L2状态原子参与者形成结果 final {
    L2结构状态 状态 = L2结构状态::内部不一致;
    std::uint64_t 共同期望事实代次 = 0;
    std::optional<L1跨所有者原子参与者写集> 参与者;
    L2状态原子参与者局部键 局部键;
    std::optional<L2状态当前选择关系引用> 旧当前选择;
    friend bool operator==(const L2状态原子参与者形成结果&,
        const L2状态原子参与者形成结果&) = default;
};

struct L2状态引用替换请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2状态身份 状态;
    L2状态引用对关系引用 旧引用;
    L2存在身份 新主体存在;
    L2特征实例身份 新特征实例;
    friend bool operator==(const L2状态引用替换请求&,
        const L2状态引用替换请求&) = default;
};

struct L2状态引用替换结果 final {
    L2结构结果头 结果头;
    std::optional<L2状态事实> 当前状态;
    std::optional<L2状态自有关系引用> 当前自有关系;
    std::optional<L2状态引用对关系退出投影> 已退出引用;
    bool 成功() const noexcept;
    friend bool operator==(const L2状态引用替换结果&,
        const L2状态引用替换结果&) = default;
};

struct L2状态值换代请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2状态身份 状态;
    L2状态值角色 值角色 = L2状态值角色::精确值;
    稳定编码 旧值稳定编码;
    L2原始值材料 新值材料;
    稳定编码 来源稳定编码;
    friend bool operator==(const L2状态值换代请求&,
        const L2状态值换代请求&) = default;
};

struct L2状态值换代结果 final {
    L2结构结果头 结果头;
    std::optional<L2状态事实> 当前状态;
    std::optional<L2状态自有关系引用> 自有关系;
    std::optional<L2属性事实> 已退出值;
    bool 成功() const noexcept;
    friend bool operator==(const L2状态值换代结果&,
        const L2状态值换代结果&) = default;
};

struct L2状态退出闭包 final {
    L2状态自有关系引用 自有关系;
    稳定编码 精确值稳定编码;
    稳定编码 时间值稳定编码;
    friend bool operator==(const L2状态退出闭包&,
        const L2状态退出闭包&) = default;
};

struct L2状态退出请求 final {
    L2结构请求头 请求头;
    L2结构幂等身份 幂等身份;
    L2状态身份 状态;
    L2状态退出闭包 自有事实闭包;
    friend bool operator==(const L2状态退出请求&,
        const L2状态退出请求&) = default;
};

struct L2状态退出结果 final {
    L2结构结果头 结果头;
    std::optional<L2状态事实> 已退出状态;
    std::optional<L2状态自有关系退出投影> 已退出自有关系;
    bool 成功() const noexcept;
    friend bool operator==(const L2状态退出结果&,
        const L2状态退出结果&) = default;
};

// 诊断责任：无适用错误分支；固定登记合同只接受唯一规则与幂等身份。
inline bool L2目标状态合同结构类型登记请求有效(
    const L2目标状态合同结构类型登记请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && 请求.规则版本 == L2目标状态合同结构类型登记规则版本
        && 请求.幂等身份 == L2结构幂等身份{1};
}

// 诊断责任：无适用错误分支；四个专属类型必须保持首次见证和唯一编码。
inline bool L2目标状态合同结构类型登记完整(
    const L2目标状态合同结构类型登记& 登记) noexcept {
    const 稳定编码 编码组[] = {登记.特征定义关系类型.值,
        登记.比较注册关系类型.值, 登记.I64值类型.值,
        登记.U64合同属性类型.值};
    if (登记.合同版本 != L2结构合同版本
        || 登记.规则版本 != L2目标状态合同结构类型登记规则版本
        || 登记.首次幂等身份 != L2结构幂等身份{1}
        || 登记.首次期望事实代次 == 0 || 登记.建立事实代次 == 0
        || 登记.首次期望事实代次 > 登记.建立事实代次
        || 登记.建立事实代次 > 登记.事实截止代次) return false;
    for (std::size_t 左 = 0; 左 < std::size(编码组); ++左) {
        if (!有效(编码组[左])) return false;
        for (std::size_t 右 = 0; 右 < 左; ++右)
            if (编码组[左] == 编码组[右]) return false;
    }
    return true;
}

// 诊断责任：无适用错误分支；目标关系只接受冻结的三个具名位组合。
inline bool L2目标状态允许关系位有效(std::uint8_t 允许关系位) noexcept {
    return 允许关系位 >= 1 && 允许关系位 <= 7;
}

// 诊断责任：无适用错误分支；值式事实必须闭合首次见证、自有编码和生命周期。
inline bool L2目标状态合同事实完整(
    const L2目标状态合同事实& 合同, std::uint64_t 截止) noexcept {
    const 稳定编码 自有编码组[] = {合同.身份.值,
        合同.特征定义关系稳定编码, 合同.比较注册关系稳定编码,
        合同.目标值稳定编码, 合同.合同值稳定编码};
    if (截止 == 0 || 合同.比较用途 != L2特征比较用途::目标判断
        || !有效(合同.特征定义.值) || !有效(合同.比较注册.值)
        || 合同.特征定义.值 == 合同.比较注册.值
        || !有效(合同.来源稳定编码)
        || !L2目标状态允许关系位有效(合同.允许关系位)
        || 合同.规则版本 != L2目标状态合同规则版本
        || !L2结构幂等身份有效(合同.首次幂等身份)
        || 合同.首次期望事实代次 == 0
        || !L2生命周期完整(合同.生命周期)
        || 合同.首次期望事实代次 > 合同.生命周期.创建事实代次
        || 合同.生命周期.创建事实代次 > 截止
        || (合同.生命周期.退出事实代次
            && *合同.生命周期.退出事实代次 > 截止)) return false;
    for (std::size_t 左 = 0; 左 < std::size(自有编码组); ++左) {
        if (!有效(自有编码组[左])
            || 自有编码组[左] == 合同.特征定义.值
            || 自有编码组[左] == 合同.比较注册.值
            || 自有编码组[左] == 合同.来源稳定编码) return false;
        for (std::size_t 右 = 0; 右 < 左; ++右)
            if (自有编码组[左] == 自有编码组[右]) return false;
    }
    return true;
}

// 诊断责任：无适用错误分支；新增请求只接受目标判断注册和冻结规则。
inline bool L2目标状态合同新增请求有效(
    const L2目标状态合同新增请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && L2结构幂等身份有效(请求.幂等身份)
        && 有效(请求.特征定义.值) && 有效(请求.比较注册.值)
        && 请求.特征定义.值 != 请求.比较注册.值
        && L2目标状态允许关系位有效(请求.允许关系位)
        && 请求.规则版本 == L2目标状态合同规则版本
        && 有效(请求.来源稳定编码);
}

// 诊断责任：无适用错误分支；替换必须建立新身份而不是原位换值。
inline bool L2目标状态合同替换请求有效(
    const L2目标状态合同替换请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && L2结构幂等身份有效(请求.幂等身份)
        && 有效(请求.旧合同.值) && 有效(请求.特征定义.值)
        && 有效(请求.比较注册.值)
        && 请求.旧合同.值 != 请求.特征定义.值
        && 请求.旧合同.值 != 请求.比较注册.值
        && 请求.特征定义.值 != 请求.比较注册.值
        && L2目标状态允许关系位有效(请求.新允许关系位)
        && 请求.新规则版本 == L2目标状态合同规则版本
        && 有效(请求.来源稳定编码);
}

// 诊断责任：无适用错误分支；退出必须回显一个合同节点及四项互异自有闭包。
inline bool L2目标状态合同退出请求有效(
    const L2目标状态合同退出请求& 请求) noexcept {
    if (!L2结构请求头合同有效(请求.请求头)
        || 请求.请求头.期望事实代次 == 0
        || !L2结构幂等身份有效(请求.幂等身份)
        || !有效(请求.合同.值)) return false;
    const 稳定编码 编码组[] = {请求.合同.值,
        请求.特征定义关系稳定编码, 请求.比较注册关系稳定编码,
        请求.目标值稳定编码, 请求.合同值稳定编码};
    for (std::size_t 左 = 0; 左 < std::size(编码组); ++左) {
        if (!有效(编码组[左])) return false;
        for (std::size_t 右 = 0; 右 < 左; ++右)
            if (编码组[左] == 编码组[右]) return false;
    }
    return true;
}

// 诊断责任：无适用错误分支；当前与历史读取截止参数互斥。
inline bool L2目标状态合同完整读取请求有效(
    const L2目标状态合同完整读取请求& 请求) noexcept {
    if (!L2结构请求头合同有效(请求.请求头)
        || 请求.请求头.期望事实代次 == 0 || !有效(请求.合同.值)) return false;
    if (请求.读取类别 == L2读取类别::当前)
        return 请求.历史截止事实代次 == 0;
    return 请求.读取类别 == L2读取类别::历史
        && 请求.历史截止事实代次 != 0
        && 请求.历史截止事实代次 <= 请求.请求头.期望事实代次;
}

// 诊断责任：无适用错误分支；固定登记合同只接受唯一规则与幂等身份。
inline bool L2状态结构类型登记请求有效(
    const L2状态结构类型登记请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && 请求.规则版本 == L2状态结构类型登记规则版本
        && 请求.幂等身份 == L2结构幂等身份{1};
}

// 诊断责任：无适用错误分支；十一个登记编码必须非零且互异。
inline bool L2状态结构类型登记完整(
    const L2状态结构类型登记& 登记) noexcept {
    const 稳定编码 编码组[] = {
        登记.主体关系类型.值, 登记.特征实例关系类型.值,
        登记.当前选择关系类型.值,
        登记.精确I64值类型.值, 登记.精确I64组值类型.值,
        登记.精确U64组值类型.值, 登记.精确独立材料值类型.值,
        登记.时间I64值类型.值, 登记.时间I64组值类型.值,
        登记.时间U64组值类型.值, 登记.时间独立材料值类型.值};
    if (登记.合同版本 != L2结构合同版本
        || 登记.规则版本 != L2状态结构类型登记规则版本
        || 登记.首次幂等身份 != L2结构幂等身份{1}
        || 登记.首次期望事实代次 == 0 || 登记.建立事实代次 == 0
        || 登记.事实截止代次 < 登记.建立事实代次) return false;
    for (std::size_t 左 = 0; 左 < std::size(编码组); ++左) {
        if (!有效(编码组[左])) return false;
        for (std::size_t 右 = 0; 右 < 左; ++右)
            if (编码组[左] == 编码组[右]) return false;
    }
    return true;
}

// 诊断责任：无适用错误分支；纯值材料形状判断不读取事实。
inline bool L2状态原始值材料有效(const L2原始值材料& 材料) noexcept {
    return std::visit([](const auto& 值) noexcept {
        using 类型 = std::decay_t<decltype(值)>;
        if constexpr (std::is_same_v<类型, std::int64_t>) return true;
        else if constexpr (std::is_same_v<类型, L2独立材料引用>)
            return 有效(值.编码);
        else return !值.empty();
    }, 材料);
}

// 诊断责任：无适用错误分支；只核对两条具名关系编码。
inline bool L2状态引用对关系引用有效(
    const L2状态引用对关系引用& 引用) noexcept {
    return 有效(引用.主体关系稳定编码)
        && 有效(引用.特征实例关系稳定编码)
        && 引用.主体关系稳定编码 != 引用.特征实例关系稳定编码;
}

// 诊断责任：无适用错误分支；只核对带退出生命周期的关系事实。
inline bool L2状态关系退出事实完整(
    const L2状态关系退出事实& 关系, std::uint64_t 退出代次) noexcept {
    return 退出代次 != 0 && 有效(关系.关系稳定编码)
        && L2生命周期完整(关系.生命周期)
        && 关系.生命周期.退出事实代次 == 退出代次;
}

// 诊断责任：无适用错误分支；引用替换请求只接受完整强类型引用对。
inline bool L2状态引用替换请求有效(
    const L2状态引用替换请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && L2结构幂等身份有效(请求.幂等身份)
        && 有效(请求.状态.值) && L2状态引用对关系引用有效(请求.旧引用)
        && 有效(请求.新主体存在.值) && 有效(请求.新特征实例.值);
}

// 诊断责任：无适用错误分支；值换代请求只接受两个冻结角色和有效材料。
inline bool L2状态值换代请求有效(
    const L2状态值换代请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && L2结构幂等身份有效(请求.幂等身份)
        && 有效(请求.状态.值) && 有效(请求.旧值稳定编码)
        && (请求.值角色 == L2状态值角色::精确值
            || 请求.值角色 == L2状态值角色::时间值)
        && L2状态原始值材料有效(请求.新值材料)
        && 有效(请求.来源稳定编码);
}

// 诊断责任：无适用错误分支；退出闭包五项必须非零、互异且不含状态节点。
inline bool L2状态退出请求有效(const L2状态退出请求& 请求) noexcept {
    if (!L2结构请求头合同有效(请求.请求头)
        || 请求.请求头.期望事实代次 == 0
        || !L2结构幂等身份有效(请求.幂等身份)
        || !有效(请求.状态.值)) return false;
    const 稳定编码 编码组[] = {
        请求.自有事实闭包.自有关系.族归属关系稳定编码,
        请求.自有事实闭包.自有关系.主体关系稳定编码,
        请求.自有事实闭包.自有关系.特征实例关系稳定编码,
        请求.自有事实闭包.精确值稳定编码,
        请求.自有事实闭包.时间值稳定编码};
    for (std::size_t 左 = 0; 左 < std::size(编码组); ++左) {
        if (!有效(编码组[左]) || 编码组[左] == 请求.状态.值) return false;
        for (std::size_t 右 = 0; 右 < 左; ++右)
            if (编码组[左] == 编码组[右]) return false;
    }
    return true;
}

// 诊断责任：无适用错误分支；精确与时间材料分别映射自己的登记角色。
inline bool L2状态属性角色与材料一致(
    const L2状态结构类型登记& 登记, const L2属性事实& 属性,
    bool 精确角色) noexcept {
    return std::visit([&](const auto& 材料) noexcept {
        using 类型 = std::decay_t<decltype(材料)>;
        稳定编码 期望;
        if constexpr (std::is_same_v<类型, std::int64_t>)
            期望 = 精确角色 ? 登记.精确I64值类型.值 : 登记.时间I64值类型.值;
        else if constexpr (std::is_same_v<类型, std::vector<std::int64_t>>)
            期望 = 精确角色 ? 登记.精确I64组值类型.值 : 登记.时间I64组值类型.值;
        else if constexpr (std::is_same_v<类型, std::vector<std::uint64_t>>)
            期望 = 精确角色 ? 登记.精确U64组值类型.值 : 登记.时间U64组值类型.值;
        else
            期望 = 精确角色 ? 登记.精确独立材料值类型.值
                : 登记.时间独立材料值类型.值;
        return 属性.属性类型身份 == 期望;
    }, 属性.类型化不可变材料);
}

// 诊断责任：无适用错误分支；只检查具名截止的状态值式投影。
inline bool L2状态事实完整(const L2状态事实& 状态,
    const L2状态结构类型登记& 登记, std::uint64_t 截止) noexcept {
    return 截止 != 0 && L2状态结构类型登记完整(登记)
        && 有效(状态.身份.值) && 有效(状态.主体存在.值)
        && 有效(状态.特征实例.值) && 有效(状态.来源稳定编码)
        && L2生命周期完整(状态.生命周期)
        && 状态.生命周期.创建事实代次 <= 截止
        && (!状态.生命周期.退出事实代次
            || *状态.生命周期.退出事实代次 <= 截止)
        && L2属性事实截止投影完整(状态.精确值, 截止)
        && L2属性事实截止投影完整(状态.时间值, 截止)
        && 状态.精确值.值稳定编码 != 状态.时间值.值稳定编码
        && 状态.精确值.来源稳定编码 == 状态.来源稳定编码
        && 状态.时间值.来源稳定编码 == 状态.来源稳定编码
        && L2状态属性角色与材料一致(登记, 状态.精确值, true)
        && L2状态属性角色与材料一致(登记, 状态.时间值, false);
}

// 诊断责任：无适用错误分支；纯值新增请求判断不读取事实。
inline bool L2状态新增请求有效(const L2状态新增请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && L2结构幂等身份有效(请求.幂等身份)
        && 有效(请求.主体存在.值) && 有效(请求.特征实例.值)
        && 有效(请求.来源稳定编码)
        && L2状态原始值材料有效(请求.精确值材料)
        && L2状态原始值材料有效(请求.时间值材料);
}

inline bool L2状态当前选择关系引用完整(
    const L2状态当前选择关系引用& 关系, std::uint64_t 截止) noexcept {
    return 截止 != 0 && 有效(关系.关系稳定编码)
        && 有效(关系.特征实例.值) && 有效(关系.状态.值)
        && 有效(关系.关系类型.值) && 有效(关系.写入所有者)
        && 关系.角色或顺序 == L2状态当前选择关系角色值
        && L2生命周期完整(关系.生命周期)
        && 关系.生命周期.创建事实代次 <= 截止
        && (!关系.生命周期.退出事实代次
            || *关系.生命周期.退出事实代次 <= 截止);
}

inline bool L2状态首态请求有效_v2(
    const L2状态首态请求_v2& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头.公共请求头)
        && 请求.请求头.公共请求头.期望事实代次 != 0
        && 请求.请求头.专属合同版本 == L2状态首态专属合同版本_v2
        && L2结构幂等身份有效(请求.幂等身份)
        && 有效(请求.主体存在.值) && 有效(请求.特征实例.值)
        && 有效(请求.来源稳定编码)
        && L2状态原始值材料有效(请求.精确值材料)
        && L2状态原始值材料有效(请求.时间值材料);
}

inline bool L2状态当前选择读取请求有效(
    const L2状态当前选择读取请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0 && 有效(请求.特征实例.值);
}

inline bool L2状态历史选择读取请求有效(
    const L2状态历史选择读取请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0 && 有效(请求.特征实例.值)
        && 请求.历史截止事实代次 != 0
        && 请求.历史截止事实代次 <= 请求.请求头.期望事实代次;
}

// 诊断责任：无适用错误分支；当前与历史读取参数按类别互斥。
inline bool L2状态完整读取请求有效(
    const L2状态完整读取请求& 请求) noexcept {
    if (!L2结构请求头合同有效(请求.请求头)
        || 请求.请求头.期望事实代次 == 0 || !有效(请求.状态.值)) return false;
    if (请求.读取类别 == L2读取类别::当前)
        return 请求.历史截止事实代次 == 0;
    return 请求.读取类别 == L2读取类别::历史
        && 请求.历史截止事实代次 != 0
        && 请求.历史截止事实代次 <= 请求.请求头.期望事实代次;
}

// 诊断责任：无适用错误分支；当前与历史来源读取参数按类别互斥。
inline bool L2状态身份来源读取请求有效(
    const L2状态身份来源读取请求& 请求) noexcept {
    if (!L2结构请求头合同有效(请求.请求头)
        || 请求.请求头.期望事实代次 == 0 || !有效(请求.状态.值)) return false;
    if (请求.读取类别 == L2读取类别::当前)
        return 请求.历史截止事实代次 == 0;
    return 请求.读取类别 == L2读取类别::历史
        && 请求.历史截止事实代次 != 0
        && 请求.历史截止事实代次 <= 请求.请求头.期望事实代次;
}

// 诊断责任：无适用错误分支；条件读取只接受具名当前端点。
inline bool L2按主体当前状态读取请求有效(
    const L2按主体当前状态读取请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && 有效(请求.主体存在.值) && 请求.最大数量 != 0;
}

// 诊断责任：无适用错误分支；条件读取只接受具名当前端点。
inline bool L2按特征实例当前状态读取请求有效(
    const L2按特征实例当前状态读取请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && 有效(请求.特征实例.值) && 请求.最大数量 != 0;
}

// 诊断责任：无适用错误分支；来源条件读取必须使用非零守卫与数量预算。
inline bool L2按来源当前状态读取请求有效(
    const L2按来源当前状态读取请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && 有效(请求.来源稳定编码) && 请求.最大数量 != 0;
}

// 诊断责任：无适用错误分支；I64 闭区间读取使用非零守卫和双预算。
inline bool L2按I64时间范围当前状态读取请求有效(
    const L2按I64时间范围当前状态读取请求& 请求) noexcept {
    return L2结构请求头合同有效(请求.请求头)
        && 请求.请求头.期望事实代次 != 0
        && 请求.最小时间I64 <= 请求.最大时间I64
        && 请求.最大候选数量 != 0 && 请求.最大返回数量 != 0;
}

inline bool L2目标状态合同结构类型登记结果::成功() const noexcept {
    return (状态 == L2目标状态合同结构类型登记状态::已提交
            || 状态 == L2目标状态合同结构类型登记状态::精确重复
            || 状态 == L2目标状态合同结构类型登记状态::已读取)
        && 事实截止代次 != 0 && 登记
        && 登记->事实截止代次 == 事实截止代次
        && L2目标状态合同结构类型登记完整(*登记);
}

inline bool L2目标状态合同新增结果::成功() const noexcept {
    return (结果头.状态 == L2结构状态::已提交
            || 结果头.状态 == L2结构状态::精确重复)
        && 结果头.事实截止代次 != 0 && 结果头.变更事实代次
        && *结果头.变更事实代次 == 结果头.事实截止代次
        && 当前合同
        && L2目标状态合同事实完整(*当前合同, 结果头.事实截止代次)
        && !当前合同->生命周期.退出事实代次;
}

inline bool L2目标状态合同替换结果::成功() const noexcept {
    return (结果头.状态 == L2结构状态::已提交
            || 结果头.状态 == L2结构状态::精确重复)
        && 结果头.事实截止代次 != 0 && 结果头.变更事实代次
        && *结果头.变更事实代次 == 结果头.事实截止代次
        && 当前合同 && 已退出合同
        && 当前合同->身份 != 已退出合同->身份
        && L2目标状态合同事实完整(*当前合同, 结果头.事实截止代次)
        && L2目标状态合同事实完整(*已退出合同, 结果头.事实截止代次)
        && !当前合同->生命周期.退出事实代次
        && 已退出合同->生命周期.退出事实代次
            == 结果头.事实截止代次;
}

inline bool L2目标状态合同退出结果::成功() const noexcept {
    return (结果头.状态 == L2结构状态::已提交
            || 结果头.状态 == L2结构状态::精确重复)
        && 结果头.事实截止代次 != 0 && 结果头.变更事实代次
        && *结果头.变更事实代次 == 结果头.事实截止代次
        && 已退出合同
        && L2目标状态合同事实完整(*已退出合同, 结果头.事实截止代次)
        && 已退出合同->生命周期.退出事实代次
            == 结果头.事实截止代次;
}

inline bool L2目标状态合同完整读取结果::成功() const noexcept {
    if (结果头.状态 != L2结构状态::已读取
        || 结果头.事实截止代次 == 0 || 结果头.变更事实代次
        || !合同 || !L2目标状态合同事实完整(*合同,
            读取类别 == L2读取类别::历史
                ? 历史截止事实代次 : 结果头.事实截止代次)) return false;
    if (读取类别 == L2读取类别::当前)
        return 历史截止事实代次 == 0 && !合同->生命周期.退出事实代次;
    return 读取类别 == L2读取类别::历史
        && 历史截止事实代次 != 0
        && 结果头.事实截止代次 == 历史截止事实代次;
}

inline bool L2状态结构类型登记结果::成功() const noexcept {
    const bool 成功状态 = 状态 == L2状态结构类型登记状态::已提交
        || 状态 == L2状态结构类型登记状态::精确重复
        || 状态 == L2状态结构类型登记状态::已读取;
    return 成功状态 && 事实截止代次 != 0 && 登记
        && 登记->事实截止代次 == 事实截止代次
        && L2状态结构类型登记完整(*登记);
}

inline bool L2状态写入结果::成功() const noexcept {
    const bool 成功状态 = 结果头.状态 == L2结构状态::已提交
        || 结果头.状态 == L2结构状态::精确重复;
    return 成功状态 && 结果头.事实截止代次 != 0
        && 结果头.变更事实代次
        && *结果头.变更事实代次 == 结果头.事实截止代次
        && 状态 && 自有关系
        && 有效(自有关系->族归属关系稳定编码)
        && 有效(自有关系->主体关系稳定编码)
        && 有效(自有关系->特征实例关系稳定编码)
        && 自有关系->族归属关系稳定编码
            != 自有关系->主体关系稳定编码
        && 自有关系->族归属关系稳定编码
            != 自有关系->特征实例关系稳定编码
        && 自有关系->主体关系稳定编码
            != 自有关系->特征实例关系稳定编码;
}

inline bool L2状态首态结果_v2::成功() const noexcept {
    const bool 首次 = 结果头.状态 == L2结构状态::已提交
        && 操作状态 == L2状态首态操作状态_v2::已提交;
    const bool 重复 = 结果头.状态 == L2结构状态::精确重复
        && 操作状态 == L2状态首态操作状态_v2::精确重复;
    if (专属合同版本 != L2状态首态专属合同版本_v2
        || (!首次 && !重复) || 结果头.事实截止代次 == 0
        || !结果头.变更事实代次 || *结果头.变更事实代次 == 0
        || *结果头.变更事实代次 > 结果头.事实截止代次
        || !状态 || !自有关系 || !当前选择关系) return false;
    const auto G1 = *结果头.变更事实代次;
    return 状态->身份 == 当前选择关系->状态
        && 状态->特征实例 == 当前选择关系->特征实例
        && 状态->生命周期.创建事实代次 == G1
        && !状态->生命周期.退出事实代次
        && 状态->精确值.创建事实代次 == G1
        && 状态->时间值.创建事实代次 == G1
        && !状态->精确值.退出事实代次 && !状态->时间值.退出事实代次
        && 有效(自有关系->族归属关系稳定编码)
        && 有效(自有关系->主体关系稳定编码)
        && 有效(自有关系->特征实例关系稳定编码)
        && L2状态当前选择关系引用完整(*当前选择关系, G1)
        && 当前选择关系->生命周期.创建事实代次 == G1
        && !当前选择关系->生命周期.退出事实代次
        && 当前选择关系->关系稳定编码
            != 自有关系->族归属关系稳定编码
        && 当前选择关系->关系稳定编码
            != 自有关系->主体关系稳定编码
        && 当前选择关系->关系稳定编码
            != 自有关系->特征实例关系稳定编码;
}

inline bool L2状态当前选择读取结果::成功() const noexcept {
    return 结果头.状态 == L2结构状态::已读取
        && 读取状态 == L2状态当前选择读取状态::已读取
        && 结果头.事实截止代次 != 0 && !结果头.变更事实代次
        && 当前选择关系 && 当前状态
        && L2状态当前选择关系引用完整(
            *当前选择关系, 结果头.事实截止代次)
        && !当前选择关系->生命周期.退出事实代次
        && 当前状态->身份 == 当前选择关系->状态
        && 当前状态->特征实例 == 当前选择关系->特征实例
        && !当前状态->生命周期.退出事实代次;
}

inline bool L2状态历史选择读取结果::成功() const noexcept {
    if (结果头.状态 != L2结构状态::已读取
        || 读取状态 != L2状态历史选择读取状态::已读取
        || 历史截止事实代次 == 0
        || 结果头.事实截止代次 != 历史截止事实代次
        || 结果头.变更事实代次
        || static_cast<bool>(当前选择关系) != static_cast<bool>(状态)) return false;
    if (!当前选择关系) return true;
    return L2状态当前选择关系引用完整(
            *当前选择关系, 历史截止事实代次)
        && !当前选择关系->生命周期.退出事实代次
        && 状态->身份 == 当前选择关系->状态
        && 状态->特征实例 == 当前选择关系->特征实例;
}

inline bool L2状态身份来源读取结果::成功() const noexcept {
    return 结果头.状态 == L2结构状态::已读取
        && 结果头.事实截止代次 != 0 && !结果头.变更事实代次
        && 身份来源
        && L2节点族身份来源事实完整(*身份来源,
            读取类别 == L2读取类别::历史
                ? 历史截止事实代次 : 结果头.事实截止代次)
        && 身份来源->节点族 == L2节点族::状态;
}

inline bool L2当前状态组读取结果::成功() const noexcept {
    if (结果头.状态 != L2结构状态::已读取
        || 结果头.事实截止代次 == 0 || 结果头.变更事实代次) return false;
    for (std::size_t 索引 = 0; 索引 < 状态.size(); ++索引) {
        if (状态[索引].生命周期.退出事实代次
            || (索引 != 0 && !(状态[索引 - 1].身份.值 < 状态[索引].身份.值)))
            return false;
    }
    return true;
}

inline bool L2状态引用替换结果::成功() const noexcept {
    if ((结果头.状态 != L2结构状态::已提交
            && 结果头.状态 != L2结构状态::精确重复)
        || 结果头.事实截止代次 == 0 || !结果头.变更事实代次
        || *结果头.变更事实代次 != 结果头.事实截止代次
        || !当前状态 || !当前自有关系 || !已退出引用
        || !L2状态引用对关系引用有效({当前自有关系->主体关系稳定编码,
            当前自有关系->特征实例关系稳定编码})
        || !有效(当前自有关系->族归属关系稳定编码)) return false;
    const auto 截止 = 结果头.事实截止代次;
    return L2状态关系退出事实完整(已退出引用->主体关系, 截止)
        && L2状态关系退出事实完整(已退出引用->特征实例关系, 截止)
        && 已退出引用->主体关系.关系稳定编码
            != 已退出引用->特征实例关系.关系稳定编码;
}

inline bool L2状态值换代结果::成功() const noexcept {
    return (结果头.状态 == L2结构状态::已提交
            || 结果头.状态 == L2结构状态::精确重复)
        && 结果头.事实截止代次 != 0 && 结果头.变更事实代次
        && *结果头.变更事实代次 == 结果头.事实截止代次
        && 当前状态 && 自有关系 && 已退出值
        && L2属性事实截止投影完整(*已退出值, 结果头.事实截止代次)
        && 已退出值->退出事实代次 == 结果头.事实截止代次;
}

inline bool L2状态退出结果::成功() const noexcept {
    if ((结果头.状态 != L2结构状态::已提交
            && 结果头.状态 != L2结构状态::精确重复)
        || 结果头.事实截止代次 == 0 || !结果头.变更事实代次
        || *结果头.变更事实代次 != 结果头.事实截止代次
        || !已退出状态 || !已退出自有关系
        || 已退出状态->生命周期.退出事实代次 != 结果头.事实截止代次
        || 已退出状态->精确值.退出事实代次 != 结果头.事实截止代次
        || 已退出状态->时间值.退出事实代次 != 结果头.事实截止代次)
        return false;
    const auto 截止 = 结果头.事实截止代次;
    return L2状态关系退出事实完整(已退出自有关系->族归属关系, 截止)
        && L2状态关系退出事实完整(已退出自有关系->主体关系, 截止)
        && L2状态关系退出事实完整(已退出自有关系->特征实例关系, 截止);
}

} // namespace 海中鱼巣
