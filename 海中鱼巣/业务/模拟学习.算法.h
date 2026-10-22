#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <vector>
#include "../领域/概念_特征类.h"

namespace 海中鱼巣 {

struct 数值模拟输入区间 final {
    std::int64_t 下界 = 0;
    std::int64_t 上界 = 0;
    friend bool operator==(const 数值模拟输入区间&, const 数值模拟输入区间&) = default;
};

struct 数值模拟起始区域 final {
    std::vector<数值模拟输入区间> 输入区间组;
};

struct 数值模拟细分请求 final {
    std::vector<数值模拟输入区间> 输入区间组;
    std::size_t 输出维数 = 0;
    std::vector<数值模拟起始区域> 已有区域组;
};

enum class 数值模拟样例状态 : std::uint8_t {
    已求值 = 1, 输入不可求值, 结果未知, 求值失败, 输入组合不合法
};

struct 数值模拟样例结果 final {
    数值模拟样例状态 状态 = 数值模拟样例状态::求值失败;
    std::vector<std::int64_t> 输出值组;
};

// 调用者须保证每个样例从同一初始基准求值；本算法不提供世界隔离。
using 数值模拟求值函数 = std::function<数值模拟样例结果(
    const std::vector<std::int64_t>& 输入值组)>;

struct 数值模拟样例 final {
    std::vector<std::int64_t> 输入值组;
    std::vector<std::int64_t> 输出值组;
};

// 只表示本轮采样检查结束，不表示未采样点已验证或已发布方法知识。
struct 数值模拟完成区域 final {
    std::vector<数值模拟输入区间> 输入区间组;
};

struct 数值模拟未知输入 final {
    std::vector<std::int64_t> 输入值组;
    数值模拟样例状态 原因 = 数值模拟样例状态::结果未知;
};
struct 数值模拟同输入差异 final {
    std::vector<std::int64_t> 输入值组;
    std::vector<std::vector<std::int64_t>> 不同输出组;
};
struct 数值模拟求值材料 final {
    std::vector<std::int64_t> 输入值组;
    数值模拟样例结果 结果;
    bool 收尾复测 = false;
};

enum class 数值模拟操作状态 : std::uint8_t {
    尚未初始化 = 0, 就绪, 推进中, 已完成,
    输入不合法, 样例求值失败, 输出维数不符,
    输出包含无穷哨兵, 求值抛异常, 资源失败
};

struct 数值模拟推进结果 final {
    数值模拟操作状态 状态 = 数值模拟操作状态::尚未初始化;
    std::optional<数值模拟样例> 本次新样例;
    std::optional<数值模拟完成区域> 本次完成区域;
    std::optional<数值模拟样例状态> 求值失败状态;
    std::optional<数值模拟求值材料> 本次求值;
};

struct 数值模拟进度 final {
    数值模拟操作状态 状态 = 数值模拟操作状态::尚未初始化;
    std::size_t 已求值样例数 = 0;
    std::size_t 已完成区域数 = 0;
    std::size_t 实际求值次数 = 0;
    std::size_t 未完善区域数 = 0;
};

struct 数值模拟材料读取结果 final {
    数值模拟操作状态 状态 = 数值模拟操作状态::尚未初始化;
    std::vector<数值模拟样例> 样例组;
    std::vector<数值模拟完成区域> 完成区域组;
    std::vector<数值模拟完成区域> 未完善区域组;
    std::vector<数值模拟未知输入> 未知输入组;
    std::vector<std::vector<std::int64_t>> 不合法输入组;
    std::vector<数值模拟同输入差异> 同输入差异组;
    std::vector<数值模拟求值材料> 实际求值组;
};

// 串行使用。每次推进最多求值一次；停止调用即暂停，不改变完成状态。
// 求值期间只允许重入读取，不允许重入初始化/推进或析构本对象。
class 数值模拟细分器 final {
public:
    数值模拟细分器() noexcept;
    ~数值模拟细分器();
    数值模拟细分器(const 数值模拟细分器&) = delete;
    数值模拟细分器& operator=(const 数值模拟细分器&) = delete;
    数值模拟细分器(数值模拟细分器&&) = delete;
    数值模拟细分器& operator=(数值模拟细分器&&) = delete;

    数值模拟操作状态 初始化(
        const 数值模拟细分请求& 请求,
        const 数值模拟求值函数& 求值) noexcept;
    数值模拟推进结果 推进() noexcept;
    数值模拟进度 读取进度() const noexcept;
    数值模拟材料读取结果 读取材料() const noexcept;

private:
    struct 实现;
    std::unique_ptr<实现> 实现_;
    bool 正在推进_ = false;
};

} // namespace 海中鱼巣

namespace 海中鱼巣 {

// 轴值中的稳定编码只作为有限枚举材料身份，不参与数值运算。
using 通用模拟候选值 = 特征概念准确值;
struct 通用模拟候选轴 final {
    std::uint32_t 参数序号 = 0;
    std::vector<通用模拟候选值> 候选;
    friend bool operator==(const 通用模拟候选轴&, const 通用模拟候选轴&) = default;
};
struct 通用模拟组合位置 final {
    std::vector<std::size_t> 轴位置;
    bool 已结束 = false;
    friend bool operator==(const 通用模拟组合位置&, const 通用模拟组合位置&) = default;
};

// 输入必须是合法有限闭区间。规范化合并重叠/相邻分量后逐分量取三点。
std::vector<通用模拟候选值> 生成通用模拟整数候选(
    std::vector<特征概念I64闭区间> 域);
// 全有限枚举随机排列、不放回；不削减合法候选覆盖。
std::vector<通用模拟候选值> 生成通用模拟枚举候选(
    std::vector<通用模拟候选值> 候选, std::uint64_t 种子);

class 通用模拟组合游标 final {
public:
    explicit 通用模拟组合游标(std::vector<通用模拟候选轴> 轴);
    bool 已结束() const noexcept;
    std::vector<通用模拟候选值> 读取当前组合() const;
    void 推进() noexcept;
    通用模拟组合位置 读取位置() const;
    // 调用方同时持有同一不可变候选轴；位置不跨候选集复用。
    void 恢复位置(const 通用模拟组合位置&);
    const std::vector<通用模拟候选轴>& 读取轴() const noexcept;
private:
    std::vector<通用模拟候选轴> 轴_;
    通用模拟组合位置 位置_;
};
}
