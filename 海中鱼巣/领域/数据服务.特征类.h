#pragma once
#include <algorithm>
#include <array>
#include <bit>
#include <cstdint>
#include <limits>
#include <map>
#include <mutex>
#include <new>
#include <numeric>
#include <optional>
#include <set>
#include <stdexcept>
#include <type_traits>
#include <tuple>
#include <utility>
#include <variant>
#include <vector>


#include "数据服务.特征值类.h"
#include "合同.结构操作公共.h"
namespace 海中鱼巣 {
class 概念树类数据服务;
class 特征值域比较数据服务;
class 特征值域事实读取会话_v1;
struct 特征信息身份 final {
    稳定编码 编码{};
    friend bool operator==(const 特征信息身份&, const 特征信息身份&) = default;
};
struct 特征类型身份 final {
    稳定编码 编码{};
    friend bool operator==(const 特征类型身份&, const 特征类型身份&) = default;
};
struct 特征比较规则身份 final {
    稳定编码 编码{};
    friend bool operator==(const 特征比较规则身份&, const 特征比较规则身份&) = default;
};
inline bool 有效(特征信息身份 v) noexcept { return 有效(v.编码); }
inline bool 有效(特征类型身份 v) noexcept { return 有效(v.编码); }
inline bool 有效(特征比较规则身份 v) noexcept { return 有效(v.编码); }
} // namespace 海中鱼巣
#include "数据服务.定位特征.h"

namespace 海中鱼巣 {
struct 特征I64闭区间 final {
    std::int64_t 下界{}, 上界{};
    friend bool operator==(const 特征I64闭区间&, const 特征I64闭区间&) = default;
};
using 特征准确值 = std::variant<std::int64_t, 特征值身份>;
struct 特征信息 final {
    特征信息身份 身份;
    特征类型身份 类型;
    特征准确值 准确值;
    friend bool operator==(const 特征信息&, const 特征信息&) = default;
};
inline bool 浅层结构有效(特征I64闭区间 v) noexcept { return v.下界 <= v.上界; }
inline bool 浅层结构有效(const 特征准确值& v) noexcept {
    return !v.valueless_by_exception()
        && (!std::holds_alternative<特征值身份>(v) || 有效(std::get<特征值身份>(v)));
}
inline bool 浅层结构有效(const 特征信息& v) noexcept {
    return 有效(v.身份) && 有效(v.类型) && 浅层结构有效(v.准确值);
}
enum class 特征数据错误 : std::uint8_t {
    未找到 = 0, 未设置 = 1, 入口拒绝 = 2, 类型不相容 = 3, 能力未提供 = 4,
    规则缺失 = 5, 引用冲突 = 6, 算术不可表示 = 7, 并发变化 = 8,
    资源失败 = 9, 内部不一致 = 10, 发布结果未确认 = 11, 前次写入待收敛 = 12,
    旧格式不支持 = 13, 幂等冲突 = 14
};
template<class T> using 特征数据结果 = std::variant<T, 特征数据错误>;
struct 准确特征读取请求 final {
    特征信息身份 身份;
    friend bool operator==(const 准确特征读取请求&, const 准确特征读取请求&) = default;
};
struct 准确特征读取事实 final {
    特征信息 信息;
    std::variant<std::int64_t, 特征值信息> 完整值;
    稳定编码 类型关系;
    std::optional<稳定编码> 准确值事实;
    friend bool operator==(const 准确特征读取事实&, const 准确特征读取事实&) = default;
};
struct 特征类型准确值核验请求 final {
    特征类型身份 正式特征类型;
    特征准确值 准确值;
};
struct 特征类型准确值核验事实 final {
    特征类型身份 正式特征类型;
    特征准确值 准确值;
};
enum class 特征类型准确值核验状态 : std::uint8_t {
    已核验 = 1, 入口拒绝 = 2,
    正式特征类型未找到 = 3, 准确值未找到 = 4, 准确值不相容 = 5,
    资源失败 = 7, 内部不一致 = 8
};
struct 特征类型准确值核验结果 final {
    特征类型准确值核验状态 状态 = 特征类型准确值核验状态::入口拒绝;
    std::optional<特征类型准确值核验事实> 事实;
    bool 成功() const noexcept {
        return 状态 == 特征类型准确值核验状态::已核验
            && 事实 && 有效(事实->正式特征类型)
            && 浅层结构有效(事实->准确值);
    }
};
struct I64特征域形成参数 final {
    std::int64_t 允许误差{};
    稳定编码 参数来源;
    friend bool operator==(const I64特征域形成参数&, const I64特征域形成参数&) = default;
};
enum class 特征类型来源 : std::uint8_t {
    外设能够获取 = 1, 先天定义 = 2, 后天派生 = 3
};
enum class I64基础特征单位绑定 : std::uint8_t {
    既有稳定单位 = 1, 新FT自身 = 2
};
struct I64基础特征类型形成规格 final {
    特征类型来源 来源 = 特征类型来源::外设能够获取;
    std::optional<稳定编码> 外设提供者;
    I64基础特征单位绑定 单位绑定 = I64基础特征单位绑定::既有稳定单位;
    std::optional<稳定编码> 既有单位;
    std::uint64_t 缩放分子{}, 缩放分母{};
    std::vector<特征I64闭区间> 允许集合;
    std::optional<I64特征域形成参数> 域形成;
    friend bool operator==(const I64基础特征类型形成规格&, const I64基础特征类型形成规格&) = default;
};
struct I64基础特征类型规格 final {
    特征类型来源 来源 = 特征类型来源::外设能够获取;
    std::optional<稳定编码> 外设提供者;
    稳定编码 单位;
    std::uint64_t 缩放分子{}, 缩放分母{};
    std::vector<特征I64闭区间> 允许集合;
    std::optional<I64特征域形成参数> 域形成;
    friend bool operator==(const I64基础特征类型规格&, const I64基础特征类型规格&) = default;
};
struct I64基础特征类型信息 final {
    特征类型身份 身份;
    I64基础特征类型规格 规格;
    std::optional<特征比较规则身份> 规则;
    friend bool operator==(const I64基础特征类型信息&, const I64基础特征类型信息&) = default;
};
struct 特征规范I64域 final {
    std::vector<特征I64闭区间> 区间;
    friend bool operator==(const 特征规范I64域&, const 特征规范I64域&) = default;
};
struct 特征域形成事实 final {
    特征类型身份 类型;
    特征规范I64域 域;
    特征信息身份 准确来源;
    特征比较规则身份 规则;
};
struct 特征类型截止请求 final {
    特征类型身份 类型;
};
struct 特征I64域判定请求 final { 特征类型截止请求 类型; 特征规范I64域 域; };
struct 特征I64域包含请求 final { 特征类型截止请求 类型; 特征规范I64域 外, 内; };
struct 准确特征域命中请求 final { 准确特征读取请求 特征; 特征规范I64域 域; };
template<class T> struct 特征截止事实 final { T 数据; };
enum class 特征类比较用途 : std::uint8_t { 目标判断 = 1, 状态迁移 = 2 };
enum class 特征类比较角色 : std::uint8_t { 当前事实 = 1, 目标状态 = 2, 前状态 = 3, 后当前事实 = 4 };
enum class 特征R材料类别 : std::uint8_t { I64闭区间 = 1, 类型规则U64组 = 2 };
struct 特征R区间材料 final {
    std::uint32_t 格式版本 = 1;
    特征R材料类别 类别 = 特征R材料类别::I64闭区间;
    std::vector<std::uint64_t> 规范化U64组;
    friend bool operator==(const 特征R区间材料&, const 特征R区间材料&) = default;
};
struct I64基础特征类型定义请求 final {
    L1所有者范围写入幂等身份 幂等身份;
    I64基础特征类型形成规格 规格;
    friend bool operator==(const I64基础特征类型定义请求&, const I64基础特征类型定义请求&) = default;
};
enum class I64基础特征类型定义状态 : std::uint8_t {
    已形成 = 1, 已恢复 = 2, 入口拒绝 = 3,
    幂等冲突 = 5, 首次材料不一致 = 6,
    引用冲突 = 8, 已可能发布 = 9, 资源失败 = 10, 内部不一致 = 11
};
struct I64基础特征类型定义结果 final {
    I64基础特征类型定义状态 状态 = I64基础特征类型定义状态::入口拒绝;
    I64基础特征类型定义请求 原请求;
    std::optional<L1所有者范围写入结果> 首次写入回执;
    std::optional<特征截止事实<I64基础特征类型信息>> 事实;
    bool 成功() const noexcept {
        return (状态 == I64基础特征类型定义状态::已形成
            || 状态 == I64基础特征类型定义状态::已恢复)
            && 事实
            && 有效(事实->数据.身份) && 事实->数据.规格.来源 == 原请求.规格.来源;
    }
};

struct 特征正式准确I64解析请求_v2 final {
    特征类型身份 正式特征类型;
    特征准确值 准确值;
};

enum class 特征正式准确I64解析状态_v2 : std::uint8_t {
    已解析 = 1,
    入口拒绝 = 2,
    正式特征类型未找到 = 3,
    准确值未找到 = 4,
    准确值不相容 = 5,
    非I64 = 6,
    资源失败 = 8,
    内部不一致 = 9
};

struct 特征正式准确I64解析事实_v2 final {
    特征类型身份 正式特征类型;
    特征准确值 原始准确值;
    std::int64_t I64 = 0;
};

struct 特征正式准确I64解析结果_v2 final {
    特征正式准确I64解析状态_v2 状态 = 特征正式准确I64解析状态_v2::入口拒绝;
    std::optional<特征正式准确I64解析事实_v2> 事实;
    bool 成功(const 特征正式准确I64解析请求_v2& 请求) const noexcept {
        return 有效(请求.正式特征类型)
            && 浅层结构有效(请求.准确值)
            && 状态 == 特征正式准确I64解析状态_v2::已解析
            && 事实
            && 事实->正式特征类型 == 请求.正式特征类型
            && 事实->原始准确值 == 请求.准确值;
    }
};
// FCv 是概念 owner 已核验后的当前投影；特征类不读取 F→FCv 关系。
struct 特征R成员规则投影 final {
    特征信息身份 F;
    稳定编码 FCv;
    friend bool operator==(const 特征R成员规则投影&, const 特征R成员规则投影&) = default;
};
struct 特征R规则项投影 final {
    稳定编码 R;
    特征R区间材料 材料;
    std::vector<特征R成员规则投影> 形成成员;
    friend bool operator==(const 特征R规则项投影&, const 特征R规则项投影&) = default;
};
struct 特征R集合版本规则投影 final {
    稳定编码 R集合;
    稳定编码 版本;
    std::vector<特征R规则项投影> R项;
    friend bool operator==(const 特征R集合版本规则投影&, const 特征R集合版本规则投影&) = default;
};
enum class 特征R规则状态 : std::uint8_t {
    唯一命中 = 1, 形成新R = 2, 已取得代表值 = 3, 已归并零输出 = 4, 已归并输出 = 5,
    规则未启用 = 6, 入口拒绝 = 7, 特征类型未找到 = 8,
    候选值不可读 = 10, 材料格式不支持 = 11,
    资源失败 = 14, 内部不一致 = 15
};
struct 特征R归组规则请求 final {
    特征类型身份 FT; 特征准确值 候选值;
    std::vector<特征R规则项投影> 当前R项;
};
struct 特征R归组规则结果 final {
    特征R规则状态 状态 = 特征R规则状态::入口拒绝;
    std::optional<稳定编码> 命中R; std::optional<特征R区间材料> 规范化材料;
};
struct 特征R代表值请求 final {
    特征类型身份 FT; 特征R区间材料 材料;
};
struct 特征R代表值结果 final {
    特征R规则状态 状态 = 特征R规则状态::入口拒绝;
    std::optional<特征准确值> 代表值;
};
struct 特征R概念归并请求 final {
    特征类型身份 FT; 特征R集合版本规则投影 R集合版本;
};
struct 特征RI64概念归并项 final {
    特征规范I64域 域; std::vector<稳定编码> FCv下位;
    friend bool operator==(const 特征RI64概念归并项&, const 特征RI64概念归并项&) = default;
};
struct 特征R概念归并结果 final {
    特征R规则状态 状态 = 特征R规则状态::入口拒绝;
    稳定编码 R集合, 版本;
    std::vector<特征RI64概念归并项> 项;
};
struct 补齐I64默认R规则请求 final {
    特征类型身份 FT; L1所有者范围写入幂等身份 幂等身份;
};
struct 补齐I64默认R规则结果 final {
    特征R规则状态 状态 = 特征R规则状态::入口拒绝;
    bool 规则已补齐 = false;
};
struct 特征类定义身份 final {
    稳定编码 结点{};
    friend bool operator==(const 特征类定义身份&, const 特征类定义身份&) = default;
};
struct 特征类比较注册身份 final {
    稳定编码 值{};
    friend bool operator==(const 特征类比较注册身份&, const 特征类比较注册身份&) = default;
};
struct 特征类派生规则 final {
    稳定编码 规则身份{};
    std::uint32_t 规则版本 = 0;
    friend bool operator==(const 特征类派生规则&, const 特征类派生规则&) = default;
};
enum class 特征类标量结果角色 : std::uint8_t { 排序 = 1, 关系 = 2, 差异 = 3 };
enum class 特征类标量量纲 : std::uint8_t { 无量纲 = 1, 有量纲 = 2 };
enum class 特征类标量舍入 : std::uint8_t { 不转换 = 1 };
enum class 特征类标量溢出 : std::uint8_t { 拒绝 = 1 };
enum class 特征类标量方向 : std::uint8_t { 左对右 = 1, 左到右 = 2, 右减左 = 3 };
enum class 特征类标量状态 : std::uint8_t {
    已读取 = 1, 已比较, 入口拒绝, 未找到, 已删除, 格式不支持, 算法不支持,
    类型不匹配, 来源不匹配, 单位量化不匹配, 结果范围不满足, 运算溢出,
    资源失败, 内部不一致,
    已创建, 精确重复, 幂等冲突, 引用冲突, 已可能发布
};
enum class 特征类标量发布确定性 : std::uint8_t {
    未派发 = 1, 确认未发布, 确认已发布, 可能已发布
};
struct 特征类标量量化合同 final {
    稳定编码 单位{}, 维度{}, 分量角色{};
    std::uint64_t 缩放分子 = 1, 缩放分母 = 1;
    std::int64_t 下界 = 0, 上界 = 0;
    特征类标量量纲 量纲类别 = 特征类标量量纲::无量纲;
    特征类标量舍入 舍入 = 特征类标量舍入::不转换;
    特征类标量溢出 溢出 = 特征类标量溢出::拒绝;
    bool 完整() const noexcept {
        return 有效(单位) && 有效(维度) && 有效(分量角色) && 缩放分子 && 缩放分母
            && std::gcd(缩放分子, 缩放分母) == 1 && 下界 <= 上界
            && (量纲类别 == 特征类标量量纲::无量纲 || 量纲类别 == 特征类标量量纲::有量纲)
            && 舍入 == 特征类标量舍入::不转换 && 溢出 == 特征类标量溢出::拒绝;
    }
    friend bool operator==(const 特征类标量量化合同&, const 特征类标量量化合同&) = default;
};
struct 特征类标量输出提交项 final {
    特征类标量结果角色 角色 = 特征类标量结果角色::排序;
    特征类标量量化合同 量化;
    friend bool operator==(const 特征类标量输出提交项&, const 特征类标量输出提交项&) = default;
};
struct 特征类标量输出事实 final {
    特征类标量输出提交项 声明;
    稳定编码 特征类型{}, 归属关系{}, 格式标记值事实{};
    friend bool operator==(const 特征类标量输出事实&, const 特征类标量输出事实&) = default;
};
struct 特征类标量基础来源 final {
    特征信息身份 F;
    friend bool operator==(const 特征类标量基础来源&, const 特征类标量基础来源&) = default;
};
struct 特征类标量派生来源 final {
    特征类定义身份 定义;
    特征类标量结果角色 上游输出角色 = 特征类标量结果角色::排序;
    friend bool operator==(const 特征类标量派生来源&, const 特征类标量派生来源&) = default;
};
inline 稳定编码 标量来源编码(const std::variant<特征类标量基础来源, 特征类标量派生来源>& v) {
    return std::visit([](const auto& x) {
        if constexpr (std::is_same_v<std::decay_t<decltype(x)>, 特征类标量基础来源>) return x.F.编码;
        else return x.定义.结点;
    }, v);
}
inline unsigned 标量来源输出角色(const std::variant<特征类标量基础来源, 特征类标量派生来源>& v) {
    if (const auto* x = std::get_if<特征类标量派生来源>(&v)) return static_cast<unsigned>(x->上游输出角色);
    return 0;
}
inline bool 标量来源形状有效(const std::variant<特征类标量基础来源, 特征类标量派生来源>& v) noexcept {
    if (const auto* x = std::get_if<特征类标量基础来源>(&v)) return 有效(x->F);
    if (const auto* x = std::get_if<特征类标量派生来源>(&v)) {
        const auto role = static_cast<unsigned>(x->上游输出角色);
        return 有效(x->定义.结点) && role >= 1 && role <= 3;
    }
    return false;
}
struct 特征类标量派生来源提交项 final {
    std::uint32_t 顺序 = 0;
    特征类比较角色 输入角色 = 特征类比较角色::当前事实;
    std::variant<特征类标量基础来源, 特征类标量派生来源> 来源;
    friend bool operator==(const 特征类标量派生来源提交项&, const 特征类标量派生来源提交项&) = default;
};
struct 特征类标量派生来源事实 final {
    特征类标量派生来源提交项 内容;
    稳定编码 关系{};
    friend bool operator==(const 特征类标量派生来源事实&, const 特征类标量派生来源事实&) = default;
};
struct 特征类标量比较注册合同 final {
    特征类比较用途 用途 = 特征类比较用途::目标判断;
    std::uint32_t 算法版本 = 1;
    特征类比较角色 左角色 = 特征类比较角色::当前事实;
    特征类比较角色 右角色 = 特征类比较角色::目标状态;
    std::uint8_t 允许结果位 = 0;
    std::uint32_t 误差合同版本 = 0;
    std::optional<std::int64_t> 误差预算, 相等容差;
    特征类标量量化合同 输入量化;
    std::vector<特征类标量输出提交项> 输出组;
    friend bool operator==(const 特征类标量比较注册合同&, const 特征类标量比较注册合同&) = default;
};

enum class 特征I64比较用途 : std::uint8_t { 识别区分=1, 变化分析=2, 场景关系=3, 目标判断=4, 概念材料=5 };
enum class 特征I64输入角色 : std::uint8_t {
    识别左=1, 识别右=2, 旧状态=3, 新状态=4, 参照B=5, 被描述A=6,
    当前事实=7, 目标值=8, 概念参照B=9, 概念被描述A=10
};
enum class 特征比较算法族 : std::uint8_t { 标量有序比较与安全差异=1 };
enum class 特征I64上下文要求 : std::uint8_t { 无=0, 参与者A=1, 参与者B=2, 左时间=4, 右时间=8, 参照=16 };
enum class 特征I64比较绑定状态 : std::uint8_t {
    已读取=1, 已创建=2, 已删除=3, 精确重复=4, 入口拒绝=5, 未找到=6,
    格式不支持=7, 注册不唯一=8, 幂等冲突=9, 引用冲突=10,
    资源失败=12, 内部不一致=13, 已可能发布=14
};
enum class 特征I64比较绑定操作 : std::uint8_t { 建立=1, 退出=4 };
struct 特征I64关系编码 final {
    std::int64_t 左小于{}, 等价{}, 左大于{};
    friend bool operator==(const 特征I64关系编码&, const 特征I64关系编码&) = default;
};
struct 特征I64比较绑定身份 final {
    稳定编码 编码{};
    friend bool operator==(const 特征I64比较绑定身份&, const 特征I64比较绑定身份&) = default;
};
inline bool 有效(特征I64比较绑定身份 k) noexcept { return 有效(k.编码); }
struct 特征I64比较绑定输出提交 final {
    特征类标量输出提交项 输出;
    特征类型身份 输出FT;
    friend bool operator==(const 特征I64比较绑定输出提交&, const 特征I64比较绑定输出提交&) = default;
};
struct 特征I64比较绑定输出事实 final {
    特征类标量输出提交项 输出;
    特征类型身份 输出FT;
    稳定编码 输出关系{};
    friend bool operator==(const 特征I64比较绑定输出事实&, const 特征I64比较绑定输出事实&) = default;
};
struct 特征I64比较绑定定义 final {
    特征类型身份 输入FT;
    特征I64比较用途 用途=特征I64比较用途::识别区分;
    特征比较算法族 算法族=特征比较算法族::标量有序比较与安全差异;
    std::uint32_t 算法版本=1;
    特征I64输入角色 左角色=特征I64输入角色::识别左, 右角色=特征I64输入角色::识别右;
    std::uint8_t 上下文要求位=0;
    特征类标量量化合同 输入量化;
    std::uint32_t 误差合同版本=1;
    std::optional<std::int64_t> 误差预算, 相等容差;
    std::optional<特征I64关系编码> 关系编码;
    std::vector<特征I64比较绑定输出提交> 输出组;
    friend bool operator==(const 特征I64比较绑定定义&, const 特征I64比较绑定定义&) = default;
};
inline bool I64绑定定义完整(const 特征I64比较绑定定义& d) noexcept {
    const auto purpose=static_cast<unsigned>(d.用途);
    if (!有效(d.输入FT) || purpose<1 || purpose>5
        || d.算法族!=特征比较算法族::标量有序比较与安全差异 || d.算法版本!=1
        || static_cast<unsigned>(d.左角色)!=purpose*2-1 || static_cast<unsigned>(d.右角色)!=purpose*2
        || d.上下文要求位>31 || !d.输入量化.完整() || d.误差合同版本!=1
        || (d.误差预算 && *d.误差预算<0) || (d.相等容差 && *d.相等容差<0)
        || d.输出组.empty() || d.输出组.size()>3) return false;
    unsigned prior=0; bool relation=false;
    for (const auto& o:d.输出组) {
        const auto role=static_cast<unsigned>(o.输出.角色); const auto& q=o.输出.量化;
        if (role<1 || role>3 || role<=prior || !有效(o.输出FT) || !q.完整()) return false;
        if (role==1 && (q.下界>-1 || q.上界<1 || q.缩放分子!=1 || q.缩放分母!=1
            || q.量纲类别!=特征类标量量纲::无量纲)) return false;
        if (role==2) {
            relation=true;
            if (!d.关系编码 || q.缩放分子!=1 || q.缩放分母!=1
                || q.量纲类别!=特征类标量量纲::无量纲) return false;
            const auto& c=*d.关系编码;
            if (c.左小于==c.等价 || c.左小于==c.左大于 || c.等价==c.左大于
                || c.左小于<q.下界 || c.左小于>q.上界 || c.等价<q.下界 || c.等价>q.上界
                || c.左大于<q.下界 || c.左大于>q.上界) return false;
        }
        if (role==3) {
            const auto& input=d.输入量化;
            if(q.单位!=input.单位 || q.维度!=input.维度 || q.分量角色!=input.分量角色
                || q.缩放分子!=input.缩放分子 || q.缩放分母!=input.缩放分母
                || q.量纲类别!=input.量纲类别 || q.舍入!=input.舍入 || q.溢出!=input.溢出)return false;
        }
        prior=role;
    }
    return relation==d.关系编码.has_value();
}
struct 特征I64比较绑定事实 final {
    特征I64比较绑定身份 身份;
    特征I64比较绑定定义 定义;
    std::vector<特征I64比较绑定输出事实> 输出组;
};
inline bool I64绑定事实完整(const 特征I64比较绑定事实& f) noexcept {
    if (!有效(f.身份) || !I64绑定定义完整(f.定义)
        || f.输出组.size()!=f.定义.输出组.size()) return false;
    for (std::size_t i=0;i<f.输出组.size();++i) {
        const auto& o=f.输出组[i]; const auto& d=f.定义.输出组[i];
        if (!有效(o.输出关系) || o.输出!=d.输出 || o.输出FT!=d.输出FT) return false;
        for (std::size_t j=0;j<i;++j) if (o.输出关系==f.输出组[j].输出关系) return false;
    }
    return true;
}
struct 特征I64比较绑定建立请求 final {
    L1所有者范围写入幂等身份 幂等身份;
    特征I64比较绑定定义 定义;
    friend bool operator==(const 特征I64比较绑定建立请求&,const 特征I64比较绑定建立请求&)=default;
};
enum class 特征I64比较绑定读取状态_v2 : std::uint8_t {
    已读取=1,入口拒绝=2,未找到=3,格式不支持=4,
    注册不唯一=5,资源失败=7,内部不一致=8
};
struct 特征I64比较绑定读取请求_v2 final {
    特征I64比较绑定身份 身份;
    friend bool operator==(const 特征I64比较绑定读取请求_v2&,
        const 特征I64比较绑定读取请求_v2&)=default;
};
struct 特征I64当前比较绑定读取请求_v2 final {
    特征类型身份 输入FT;
    特征I64比较用途 用途=特征I64比较用途::识别区分;
    friend bool operator==(const 特征I64当前比较绑定读取请求_v2&,
        const 特征I64当前比较绑定读取请求_v2&)=default;
};
struct 特征I64比较绑定读取结果_v2 final {
    特征I64比较绑定读取状态_v2 状态=特征I64比较绑定读取状态_v2::入口拒绝;
    std::optional<特征I64比较绑定事实> 事实;
    bool 成功() const noexcept {
        return 状态==特征I64比较绑定读取状态_v2::已读取
            && 事实 && I64绑定事实完整(*事实);
    }
    friend bool operator==(const 特征I64比较绑定读取结果_v2&,
        const 特征I64比较绑定读取结果_v2&)=default;
};
struct 特征I64比较绑定退出请求 final {
    L1所有者范围写入幂等身份 幂等身份;
    特征I64比较绑定身份 身份;
    friend bool operator==(const 特征I64比较绑定退出请求&,const 特征I64比较绑定退出请求&)=default;
};
struct 特征I64比较绑定结果 final {
    特征I64比较绑定操作 操作=特征I64比较绑定操作::建立;
    特征I64比较绑定状态 状态=特征I64比较绑定状态::入口拒绝;
    特征类标量发布确定性 发布确定性=特征类标量发布确定性::未派发;
    std::optional<特征I64比较绑定事实> 事实;
    std::optional<L1所有者范围写入结果> 正式回执;
    std::optional<特征I64比较绑定建立请求> 建立原请求;
    bool 成功() const noexcept {
        using O=特征I64比较绑定操作; using S=特征I64比较绑定状态;
        if (操作!=O::建立&&建立原请求) return false;
        if (!正式回执 || 发布确定性!=特征类标量发布确定性::确认已发布
            || (正式回执->状态!=L1所有者范围写入状态::成功 && 正式回执->状态!=L1所有者范围写入状态::精确重复)) return false;
        if (操作==O::建立) {
            if(!建立原请求||!事实)return false;
            const auto& r=*建立原请求;const auto& w=*正式回执;
            if(!有效(r.幂等身份)||!I64绑定定义完整(r.定义)
                ||(状态!=S::已创建&&状态!=S::精确重复)
                ||!I64绑定事实完整(*事实)||事实->定义!=r.定义
                ||!有效(w.所有者)||w.写入幂等身份!=r.幂等身份
                ||w.状态!=L1所有者范围写入状态::成功||!w.是否形成内存权威发布
                ||w.重试边界!=L1所有者范围重试边界::不适用
                ||w.新编码映射.size()!=5+事实->输出组.size())return false;
            auto code=[&](std::uint32_t key)->稳定编码 {
                稳定编码 found{};
                for(const auto& item:w.新编码映射)if(item.first.值==key){if(有效(found))return {};found=item.second;}
                return found;
            };
            if(code(1)!=事实->身份.编码)return false;
            for(std::uint32_t key=2;key<=5;++key)if(!有效(code(key)))return false;
            for(const auto& output:事实->输出组)
                if(code(0x200+static_cast<std::uint32_t>(output.输出.角色))!=output.输出关系)return false;
            for(std::size_t i=0;i<w.新编码映射.size();++i){
                if(!有效(w.新编码映射[i].second))return false;
                for(std::size_t j=0;j<i;++j)if(w.新编码映射[j].second==w.新编码映射[i].second)return false;
            }
            return true;
        }
        if (操作==O::退出)
            return (状态==S::已删除 || 状态==S::精确重复) && !事实
                && 有效(正式回执->所有者)
                && 有效(正式回执->写入幂等身份)&&正式回执->新编码映射.empty()
                && 正式回执->状态==L1所有者范围写入状态::成功&&正式回执->是否形成内存权威发布
                && 正式回执->重试边界==L1所有者范围重试边界::不适用;
        return false;
    }
};
struct 有界准确特征读取请求 final {
    特征信息身份 身份;
};
struct 有界准确特征读取结果 final {
    特征类标量状态 状态=特征类标量状态::入口拒绝;
    有界准确特征读取请求 原请求;
    std::optional<准确特征读取事实> 事实;
    bool 成功() const noexcept {
        const auto& r=原请求;
        if (状态!=特征类标量状态::已读取
            || !有效(r.身份) || !事实
            || 事实->信息.身份!=r.身份 || !浅层结构有效(事实->信息) || !有效(事实->类型关系)
            ) return false;
        if (const auto* v=std::get_if<std::int64_t>(&事实->信息.准确值))
            return std::holds_alternative<std::int64_t>(事实->完整值) && std::get<std::int64_t>(事实->完整值)==*v;
        const auto* value=std::get_if<特征值信息>(&事实->完整值);
        return value && 事实->准确值事实 && 有效(*事实->准确值事实)
            && value->值身份.编码==*事实->准确值事实
            && value->值身份==std::get<特征值身份>(事实->信息.准确值)
            && std::holds_alternative<std::int64_t>(value->值内容);
    }
};

struct 特征类标量派生建立请求 final {
    L1所有者范围写入幂等身份 幂等身份;
    std::vector<特征类标量派生来源提交项> 来源组;
    特征类派生规则 派生规则;
    std::optional<稳定编码> 宿主E;
    特征类标量比较注册合同 标量注册;
    friend bool operator==(const 特征类标量派生建立请求&, const 特征类标量派生建立请求&) = default;
};
struct 特征类标量派生读取请求 final {
    特征类定义身份 定义身份;
};
struct 特征类标量派生退出请求 final {
    L1所有者范围写入幂等身份 幂等身份;
    特征类定义身份 定义身份;
    friend bool operator==(const 特征类标量派生退出请求&, const 特征类标量派生退出请求&) = default;
};
struct 特征类标量派生事实 final {
    特征类定义身份 定义身份;
    std::uint32_t 真实阶次 = 0;
    std::vector<特征类标量派生来源事实> 来源组;
    特征类派生规则 派生规则;
    std::optional<稳定编码> 宿主E, 宿主关系;
    特征类比较注册身份 注册身份;
    稳定编码 注册归属{}, 阶次值{}, 规则值{}, 注册U64值{}, 注册I64值{};
    特征类标量比较注册合同 注册;
    std::vector<特征类标量输出事实> 输出组;
    bool 完整() const noexcept {
        if (!有效(定义身份.结点) || 真实阶次 <= 1
            || 来源组.size() != 2
            || !有效(派生规则.规则身份) || 派生规则.规则版本 != 1 || !有效(注册身份.值)
            || !有效(注册归属) || !有效(阶次值) || !有效(规则值) || !有效(注册U64值) || !有效(注册I64值)
            || 宿主E.has_value() != 宿主关系.has_value() || (宿主E && (!有效(*宿主E) || !有效(*宿主关系)))
            || 注册.算法版本 != 1 || !注册.误差合同版本 || !注册.输入量化.完整()
            || (注册.误差预算 && *注册.误差预算 < 0) || (注册.相等容差 && *注册.相等容差 < 0)
            || 输出组.empty() || 输出组.size() > 3 || 输出组.size() != 注册.输出组.size()) return false;
        const auto left = static_cast<unsigned>(注册.左角色), right = static_cast<unsigned>(注册.右角色);
        if (注册.用途 == 特征类比较用途::目标判断 ? left != 1 || right != 2
            : 注册.用途 != 特征类比较用途::状态迁移 || left != 3 || right != 4) return false;
        for (unsigned i = 0; i < 2; ++i) {
            const auto& x = 来源组[i];
            if (!有效(x.关系) || !标量来源形状有效(x.内容.来源) || x.内容.顺序 != i + 1
                || static_cast<unsigned>(x.内容.输入角色) != (i ? right : left)) return false;
        }
        if (标量来源编码(来源组[0].内容.来源) == 标量来源编码(来源组[1].内容.来源) || 来源组[0].关系 == 来源组[1].关系) return false;
        unsigned mask = 0, previous = 0;
        for (std::size_t i = 0; i < 输出组.size(); ++i) {
            const auto& x = 输出组[i]; const auto role = static_cast<unsigned>(x.声明.角色);
            if (role < 1 || role > 3 || role <= previous || !x.声明.量化.完整()
                || x.声明 != 注册.输出组[i] || !有效(x.特征类型) || !有效(x.归属关系) || !有效(x.格式标记值事实)) return false;
            for (std::size_t j = 0; j < i; ++j)
                if (x.特征类型 == 输出组[j].特征类型 || x.归属关系 == 输出组[j].归属关系
                    || x.格式标记值事实 == 输出组[j].格式标记值事实) return false;
            mask |= 1U << (role - 1); previous = role;
        }
        return mask == 注册.允许结果位;
    }
    friend bool operator==(const 特征类标量派生事实&, const 特征类标量派生事实&) = default;
};
struct 特征类标量叶回执 final {
    稳定编码 F{}, FT{};
    std::optional<稳定编码> 值事实;
    std::variant<std::int64_t, 特征值信息> 完整值;
    稳定编码 类型关系;
    std::int64_t 值 = 0;
    friend bool operator==(const 特征类标量叶回执&, const 特征类标量叶回执&) = default;
};
inline bool 标量叶完整(const 特征类标量叶回执& leaf) noexcept {
    if (!有效(leaf.F) || !有效(leaf.FT) || !有效(leaf.类型关系)
        || (leaf.值事实 && !有效(*leaf.值事实))) return false;
    if (const auto* direct = std::get_if<std::int64_t>(&leaf.完整值)) {
        // 内联值可同时回显真实 L1 属性值事实，该事实不是材料引用表示。
        return *direct == leaf.值;
    }
    const auto* material = std::get_if<特征值信息>(&leaf.完整值);
    if (!material || !leaf.值事实 || material->值身份.编码 != *leaf.值事实) return false;
    const auto* scalar = std::get_if<std::int64_t>(&material->值内容);
    return scalar && *scalar == leaf.值;
}
struct 特征类标量派生读取结果 final {
    特征类标量状态 状态 = 特征类标量状态::入口拒绝;
    std::optional<特征类标量派生事实> 定义事实;
    std::vector<特征类标量派生事实> 完整定义组;
    std::vector<特征类标量叶回执> 基础叶组;
    std::vector<稳定编码> 左叶组, 右叶组;
    bool 成功() const noexcept {
        if (状态 != 特征类标量状态::已读取
            || !定义事实 || !定义事实->完整() || 基础叶组.empty() || 左叶组.empty() || 右叶组.empty()) return false;
        if (完整定义组.empty()) return false;
        稳定编码 prevDefinition{}; unsigned roots=0;
        for (const auto& d:完整定义组) {
            if (!d.完整() || (有效(prevDefinition)&&!(prevDefinition<d.定义身份.结点))) return false;
            prevDefinition=d.定义身份.结点;
            if (d.定义身份==定义事实->定义身份) { if(d!=*定义事实) return false; ++roots; }
            for (const auto& s:d.来源组) {
                const auto id=标量来源编码(s.内容.来源);
                if (const auto* source=std::get_if<特征类标量派生来源>(&s.内容.来源)) {
                    auto it=std::find_if(完整定义组.begin(),完整定义组.end(),[&](const auto& x){return x.定义身份==source->定义;});
                    if (it==完整定义组.end() || it->真实阶次>=d.真实阶次
                        || std::none_of(it->输出组.begin(),it->输出组.end(),[&](const auto& x){return x.声明.角色==source->上游输出角色;})) return false;
                } else if (std::none_of(基础叶组.begin(),基础叶组.end(),[&](const auto& x){return x.F==id;})) return false;
            }
        }
        if (roots!=1) return false;
        稳定编码 previous{};
        for (const auto& x : 基础叶组) {
            if (!标量叶完整(x) || (有效(previous) && !(previous < x.F))) return false;
            if (std::find(左叶组.begin(), 左叶组.end(), x.F) == 左叶组.end()
                && std::find(右叶组.begin(), 右叶组.end(), x.F) == 右叶组.end()) return false;
            previous = x.F;
        }
        for (const auto* group : {&左叶组, &右叶组}) {
            稳定编码 prior{};
            for (auto id : *group) {
                if (!有效(id) || (有效(prior) && !(prior < id))
                    || std::none_of(基础叶组.begin(), 基础叶组.end(), [&](const auto& x) { return x.F == id; })) return false;
                prior = id;
            }
        }
        try {
            std::set<稳定编码> allDefinitions{定义事实->定义身份.结点};
            for(unsigned side=0;side<2;++side){
                std::vector<稳定编码> stack{标量来源编码(定义事实->来源组[side].内容.来源)};
                std::set<稳定编码> visited,expected;
                while(!stack.empty()){
                    const auto id=stack.back();stack.pop_back();
                    if(!visited.insert(id).second)continue;
                    const auto d=std::find_if(完整定义组.begin(),完整定义组.end(),[&](const auto& x){return x.定义身份.结点==id;});
                    if(d==完整定义组.end()){
                        if(std::none_of(基础叶组.begin(),基础叶组.end(),[&](const auto& f){return f.F==id;}))return false;
                        expected.insert(id);
                    }else{
                        allDefinitions.insert(id);
                        for(const auto& source:d->来源组)stack.push_back(标量来源编码(source.内容.来源));
                    }
                }
                const auto& actual=side ? 右叶组 : 左叶组;
                if(expected.size()!=actual.size()||!std::equal(actual.begin(),actual.end(),expected.begin()))return false;
            }
            if(allDefinitions.size()!=完整定义组.size())return false;
        }catch(...){return false;}
        return true;
    }
};
struct 特征类标量派生批量读取请求 final {
    std::vector<特征类定义身份> 根定义组;
};
struct 特征类标量派生根读取回执 final {
    特征类定义身份 定义身份;
    std::vector<稳定编码> 左叶组,右叶组;
};
struct 特征类标量派生批量读取结果 final {
    特征类标量状态 状态=特征类标量状态::入口拒绝;
    特征类标量派生批量读取请求 原请求;
    std::vector<特征类标量派生根读取回执> 根回执组;
    std::vector<特征类标量派生事实> 完整定义组;
    std::vector<特征类标量叶回执> 基础叶组;
    bool 成功() const noexcept;
};
struct 特征类标量派生写结果 final {
    特征类标量状态 状态 = 特征类标量状态::入口拒绝;
    特征类标量发布确定性 发布确定性 = 特征类标量发布确定性::未派发;
    std::optional<特征类标量派生事实> 定义事实;
    std::optional<L1所有者范围写入结果> 正式回执;
    bool 成功() const noexcept {
        return (状态 == 特征类标量状态::已创建 || 状态 == 特征类标量状态::已删除
                || 状态 == 特征类标量状态::精确重复)
            && ((状态 == 特征类标量状态::已创建 && 定义事实
                    && 定义事实->完整())
                || (状态 == 特征类标量状态::精确重复
                    && (!定义事实 || 定义事实->完整()))
                || (状态 == 特征类标量状态::已删除 && !定义事实))
            && 发布确定性 == 特征类标量发布确定性::确认已发布 && 正式回执
            && (正式回执->状态 == L1所有者范围写入状态::成功 || 正式回执->状态 == L1所有者范围写入状态::精确重复);
    }
};
// 定义和准确内容使用两个既有技术分区；本类不保存名称、观察或当前采用。
class 特征类数据服务 final : public 原子I64特征内容参与者 {
    friend class 概念树类数据服务;
    friend class 特征值域比较数据服务;
    friend class 特征值域事实读取会话_v1;
    template<class T> using R = 特征数据结果<T>;
    using S = 特征数据错误;
    enum class 分区 : std::uint8_t { 定义, 信息 };
    enum 定义角色 : std::size_t {
        定义锚点, 定义归属, 类型规格属性, 规则误差属性, 外设来源关系, 单位关系,
        域规则关系, 参数来源关系, 阶次属性, 派生规则属性, 直接来源关系,
        派生宿主关系, 注册归属关系, 注册U64属性, 注册I64属性, 输出FT归属关系, 定义角色数
    };
    enum 信息角色 : std::size_t {
        信息锚点, 信息归属, 准确内联属性, 准确引用属性, 准确类型关系, 信息角色数
    };
    enum I64比较绑定结构角色 : std::size_t {
        I64比较绑定输入FT关系, I64比较绑定输出FT关系,
        I64比较绑定U64属性, I64比较绑定I64属性, I64比较绑定结构角色数
    };
    enum R规则结构角色 : std::size_t {
        R规则归属关系, R规则版本属性, R规则参数属性, R规则结构角色数
    };
    enum 类型来源结构角色 : std::size_t { 类型来源属性, 类型来源结构角色数 };
public:
    特征类数据服务(const L1事实基座服务& l1, L1所有者范围写端口&& definitions,
        L1所有者范围写端口&& information, const 特征值类数据服务& values,
        稳定编码 producer);
    特征类数据服务(const 特征类数据服务&) = delete;
    特征类数据服务& operator=(const 特征类数据服务&) = delete;
    bool 绑定于(const L1事实基座服务&) const noexcept;
    bool 与特征服务同底座(const 特征类数据服务&) const noexcept;

    R<std::monostate> 初始化特征定义结构();
    R<std::monostate> 初始化准确特征结构();
    R<std::monostate> 收敛待确认写入();
    I64基础特征类型定义结果 形成或读取I64基础特征类型(
        const I64基础特征类型定义请求&) noexcept;
    R<I64基础特征类型信息> 读取I64基础特征类型(特征类型身份) const;
    R<特征信息> 读取准确特征(特征信息身份) const;
    R<std::vector<特征信息>> 查询准确特征(特征类型身份, const 特征准确值&) const;
    R<std::monostate> 删除准确特征(特征信息身份);
    R<特征类型身份> 读取准确特征类型(特征信息身份) const;
    R<特征准确值> 读取准确特征值(特征信息身份) const;
    有界准确特征读取结果 读取有界准确特征事实(const 有界准确特征读取请求&) const noexcept;
    R<准确特征读取事实> 读取准确特征事实(const 准确特征读取请求&) const;
    特征R归组规则结果 归组特征R(const 特征R归组规则请求&) const noexcept;
    特征R代表值结果 读取特征R代表值(const 特征R代表值请求&) const noexcept;
    特征R概念归并结果 归并特征R概念(const 特征R概念归并请求&) const noexcept;
    R<补齐I64默认R规则结果> 补齐I64默认R规则(const 补齐I64默认R规则请求&);
    特征类型准确值核验结果 核验正式特征类型准确值(
        const 特征类型准确值核验请求&) const;
    特征正式准确I64解析结果_v2 解析正式特征类型准确I64_v2(
        const 特征正式准确I64解析请求_v2&) const noexcept;
    R<特征截止事实<I64基础特征类型信息>> 读取I64基础特征类型事实(
        const 特征类型截止请求&) const;
    R<特征截止事实<特征规范I64域>> 读取I64类型完整域(const 特征类型截止请求&) const;
    R<特征域形成事实> 形成I64特征域(const 准确特征读取请求&) const;
    R<特征截止事实<特征规范I64域>> 规范化I64特征域(const 特征I64域判定请求&) const;
    R<特征截止事实<bool>> 判定准确特征命中域(const 准确特征域命中请求&) const;
    R<特征截止事实<bool>> 判定I64域包含(const 特征I64域包含请求&) const;

    特征I64比较绑定结果 建立I64比较绑定(const 特征I64比较绑定建立请求&);
    特征I64比较绑定结果 退出I64比较绑定(const 特征I64比较绑定退出请求&);
    特征I64比较绑定读取结果_v2 读取I64比较绑定_v2(
        const 特征I64比较绑定读取请求_v2&) const noexcept;
    特征I64比较绑定读取结果_v2 读取当前I64比较绑定_v2(
        const 特征I64当前比较绑定读取请求_v2&) const noexcept;

    特征类标量派生批量读取结果 批量读取标量派生定义(
        const 特征类标量派生批量读取请求&) const;
    特征类标量派生读取结果 读取标量派生定义(
        const 特征类标量派生读取请求&) const;
    特征类标量派生写结果 建立标量派生定义(
        const 特征类标量派生建立请求&);
    特征类标量派生写结果 退出标量派生定义(
        const 特征类标量派生退出请求&);

private:
    const L1事实基座服务& 原子I64底座() const noexcept override;
    L1所有者范围写端口& 原子I64端口() noexcept override;
    bool 原子I64结构已就绪() const noexcept override;
    原子I64特征候选查询结果 查询原子I64内容候选(
        const 原子I64特征候选查询请求&) const override;
    原子I64特征参与结果<L1有限N分区原子参与者写集>
    准备原子I64出生片段(const 原子I64特征出生请求&) const override;
    原子I64特征窄读取结果<原子I64特征内容事实>
    读取原子I64内容(const 原子I64特征内容读取请求&) const override;

    const L1事实基座服务& l1_;
    L1所有者范围写端口 definitions_, information_;
    const 特征值类数据服务& values_;
    稳定编码 producer_;
    std::array<稳定编码, 定义角色数> d_{};
    std::array<稳定编码, 信息角色数> f_{};
    std::array<稳定编码, I64比较绑定结构角色数> k_{};
    std::array<稳定编码, R规则结构角色数> r_{};
    std::array<稳定编码, 类型来源结构角色数> source_{};
    bool definition_ready_ = false, information_ready_ = false;
    mutable std::mutex mutex_;
};

class 特征值域事实读取会话_v1 final {
    friend class 概念树类数据服务;
    friend class 特征值域比较数据服务;
    特征值域事实读取会话_v1() = default;
};
} // namespace 海中鱼巣
