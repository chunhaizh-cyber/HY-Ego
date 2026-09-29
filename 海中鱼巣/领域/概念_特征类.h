#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <variant>
#include <vector>

#include "../核心/基础数据集.h"
#include "新_特征值类.h"

namespace 海中鱼巣 {

// I64标量直接保存在特征节点中，其余材料由新_特征值类保存。
enum class 特征概念材料物理类型 : std::int64_t {
    I64标量 = 1,
    U64数组 = 2,
    I64数组 = 3,
    UTF8字符串 = 4,
    二维二值格 = 5,
    三维二值格 = 6
};

struct 特征概念I64闭区间 final {
    std::int64_t 下界 = 0;
    std::int64_t 上界 = 0;
    friend bool operator==(const 特征概念I64闭区间&,
        const 特征概念I64闭区间&) = default;
};

// 表示该材料物理类型下的全部合法元素。它不是空集合。
struct 特征概念全材料值域 final {
    friend bool operator==(const 特征概念全材料值域&,
        const 特征概念全材料值域&) = default;
};

struct 特征概念I64分量定义 final {
    稳定编码 角色;
    std::optional<稳定编码> 单位;
    std::vector<特征概念I64闭区间> 值域;
    friend bool operator==(const 特征概念I64分量定义&,
        const 特征概念I64分量定义&) = default;
};

// 维度数量由分量.size()唯一得出，不另存第二份数量事实。
struct 特征概念结构化I64值域 final {
    std::vector<特征概念I64分量定义> 分量;
    std::size_t 维度数量() const noexcept { return 分量.size(); }
    friend bool operator==(const 特征概念结构化I64值域&,
        const 特征概念结构化I64值域&) = default;
};

// I64标量使用规范化闭区间；结构化I64材料使用有序分量值域；
// 其它材料可使用全材料值域或完整特征值节点身份集合。
using 特征概念值域 = std::variant<
    std::vector<特征概念I64闭区间>,
    特征概念全材料值域,
    std::vector<稳定编码>,
    特征概念结构化I64值域>;

using 特征概念准确值 = std::variant<std::int64_t, 稳定编码>;

enum class 特征概念比较规则 : std::int64_t {
    I64数值 = 1,
    完整材料精确 = 2
};

enum class 特征概念聚合规则 : std::int64_t {
    不自动聚合 = 1,
    I64连续值归组 = 2,
    完整材料精确集合 = 3
};

struct 特征概念定义 final {
    特征概念材料物理类型 材料类型 = 特征概念材料物理类型::I64标量;
    特征概念值域 值域;
    std::optional<稳定编码> 单位;
    特征概念比较规则 比较规则 = 特征概念比较规则::I64数值;
    特征概念聚合规则 聚合规则 = 特征概念聚合规则::不自动聚合;
    std::vector<稳定编码> 名称关系;
    friend bool operator==(const 特征概念定义&, const 特征概念定义&) = default;
};

struct 特征概念信息 final {
    稳定编码 节点;
    特征概念材料物理类型 材料类型 = 特征概念材料物理类型::I64标量;
    特征概念值域 值域;
    std::optional<稳定编码> 单位;
    特征概念比较规则 比较规则 = 特征概念比较规则::I64数值;
    特征概念聚合规则 聚合规则 = 特征概念聚合规则::不自动聚合;

    // 保存由自然语言类建立的名称关系身份；词条和关系操作稍后实现。
    std::vector<稳定编码> 名称关系;

    // 当前通过正式特征概念字段直接引用本概念的实例特征节点数量。
    std::int64_t 当前引用实例数量 = 0;

    friend bool operator==(const 特征概念信息&, const 特征概念信息&) = default;
};

enum class 特征概念规则状态 : std::uint8_t {
    已完成 = 1,
    无变化 = 2,
    入口拒绝 = 3,
    材料类型不相容 = 4,
    值不存在 = 5,
    规则不支持 = 6,
    资源失败 = 7,
    内部不一致 = 8
};

enum class 特征概念比较次序 : std::int8_t {
    小于 = -1,
    相等 = 0,
    大于 = 1
};

struct 特征概念比较结果 final {
    特征概念规则状态 状态 = 特征概念规则状态::入口拒绝;
    std::optional<bool> 相等;
    std::optional<特征概念比较次序> 次序;
    std::optional<std::uint64_t> I64绝对差;
    bool 成功() const noexcept;
};

enum class 特征概念值域关系 : std::uint8_t {
    相等 = 1,
    左包含右 = 2,
    右包含左 = 3,
    相交 = 4,
    分离 = 5
};

struct 特征概念值域比较结果 final {
    特征概念规则状态 状态 = 特征概念规则状态::入口拒绝;
    std::optional<特征概念值域关系> 关系;
};

enum class 单特征概念关系 : std::uint8_t {
    同一 = 1,
    左为上位 = 2,
    右为上位 = 3,
    具有共同上位 = 4,
    无可比关系 = 5
};

struct 单特征概念关系结果 final {
    特征概念规则状态 状态 = 特征概念规则状态::入口拒绝;
    std::optional<单特征概念关系> 关系;
    std::optional<稳定编码> 特征类型根;
    std::optional<稳定编码> 最近共同上位;
};

struct 特征概念聚合结果 final {
    特征概念规则状态 状态 = 特征概念规则状态::入口拒绝;
    std::optional<特征概念值域> 值域;
    bool 成功() const noexcept;
};

enum class 特征概念查找状态 : std::uint8_t {
    已找到 = 1,
    未找到 = 2,
    入口拒绝 = 3,
    材料类型不相容 = 4,
    单位不相容 = 5,
    匹配冲突 = 6,
    结构不一致 = 7,
    资源失败 = 8
};

struct 特征概念按值查找结果 final {
    特征概念查找状态 状态 = 特征概念查找状态::入口拒绝;
    std::optional<稳定编码> 概念节点;
    std::vector<稳定编码> 冲突候选;
};

struct 先天特征概念集合 final {
    稳定编码 毫米单位;
    稳定编码 三维空间坐标;
    稳定编码 三维空间尺寸;
    稳定编码 RGB颜色;
    稳定编码 二维轮廓;
    稳定编码 三维体素;
    bool 完整() const noexcept;
    friend bool operator==(const 先天特征概念集合&,
        const 先天特征概念集合&) = default;
};

// 全局基础数据集中的特征概念树根节点。
extern 稳定编码 特征概念树;

class 概念_特征类 final {
public:
    bool 初始化(const 新_特征值类& 特征值服务) noexcept;

    std::optional<先天特征概念集合> 获取先天特征概念() const noexcept;

    // 未给上位概念时建立新的根链特征类型概念；根链节点自身就是类型身份，
    // 不按相同材料和值域合并。给出上位概念时，在其直接子链复用或建立
    // 严格子值域概念。
    稳定编码 建立或取得特征概念(
        const 特征概念定义& 定义,
        const 新_特征值类& 特征值服务,
        std::optional<稳定编码> 上位概念 = std::nullopt) noexcept;

    std::optional<特征概念信息> 获取特征概念(
        稳定编码 特征概念节点) const noexcept;

    // 直接修改既有概念的值域；实例只引用概念并独立保存自身历史聚合值域，
    // 不保存或同步第二份概念准入值域。
    bool 更新特征概念值域(
        稳定编码 特征概念节点,
        const 特征概念值域& 新值域,
        const 新_特征值类& 特征值服务) noexcept;

    // 只维护实例特征对概念的直接引用数量；概念父子关系不计数。
    // 减少后若概念已零引用且无子概念，则删除该概念节点。
    bool 增加当前实例引用(稳定编码 特征概念节点) noexcept;
    // 调用前必须已经解除对应实例的正式特征概念字段引用。
    bool 减少当前实例引用(稳定编码 特征概念节点) noexcept;

    std::vector<稳定编码> 查询特征概念(
        const 特征概念定义& 定义) const noexcept;

    // 从根链中的特征类型概念开始，沿外部父子关系查找包含该值的最具体概念。
    特征概念按值查找结果 查找值对应的最具体概念(
        稳定编码 特征类型概念,
        const 特征概念准确值& 特征值,
        std::optional<稳定编码> 单位,
        const 新_特征值类& 特征值服务) const noexcept;

    bool 是特征概念节点(稳定编码 节点) const noexcept;

    特征概念比较结果 执行比较规则(
        特征概念材料物理类型 材料类型,
        特征概念比较规则 规则,
        const 特征概念准确值& 左值,
        const 特征概念准确值& 右值,
        const 新_特征值类& 特征值服务) const noexcept;

    // 只比较同一材料类型下的值域，不把值域关系换算为相似度。
    特征概念值域比较结果 比较值域(
        特征概念材料物理类型 材料类型,
        const 特征概念值域& 左值域,
        const 特征概念值域& 右值域) const noexcept;

    // 只处理一个根链特征类型内部的概念层级，不处理多特征类型组合概念。
    单特征概念关系结果 比较单特征概念(
        稳定编码 左概念节点,
        稳定编码 右概念节点) const noexcept;

    // 只计算规范化值域，不在本函数中建立新概念节点。
    特征概念聚合结果 执行聚合规则(
        特征概念材料物理类型 材料类型,
        特征概念聚合规则 规则,
        const std::vector<特征概念准确值>& 历史不同值,
        const 新_特征值类& 特征值服务) const noexcept;
};

} // namespace 海中鱼巣
