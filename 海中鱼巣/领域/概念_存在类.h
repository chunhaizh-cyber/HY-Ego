#pragma once

#include <cstdint>
#include <exception>
#include <optional>
#include <vector>

#include "../核心/基础数据集.h"
#include "概念_特征类.h"

namespace 海中鱼巣 {

class 概念_名称关系类;

enum class 存在概念种类 : std::uint8_t {
    专属 = 1,
    抽象 = 2
};

struct 存在概念特征项 final {
    稳定编码 特征类型概念;
    稳定编码 具体特征概念;
    friend bool operator==(const 存在概念特征项&,
        const 存在概念特征项&) = default;
};

struct 存在概念信息 final {
    稳定编码 节点;
    存在概念种类 种类 = 存在概念种类::专属;
    std::optional<稳定编码> 对应存在节点;
    std::vector<存在概念特征项> 特征组;
    std::vector<稳定编码> 名称关系;
    std::vector<稳定编码> 上位概念组;
    std::vector<稳定编码> 下位概念组;
    friend bool operator==(const 存在概念信息&,
        const 存在概念信息&) = default;
};

enum class 存在概念操作状态 : std::uint8_t {
    已建立 = 1,
    已复用 = 2,
    已更新 = 3,
    无变化 = 4,
    无共同点 = 5,
    入口拒绝 = 6,
    来源不足 = 7,
    概念不存在 = 8,
    特征概念不一致 = 9,
    层级不成立 = 10,
    会形成环 = 11,
    结构不一致 = 12,
    资源失败 = 13
};

struct 存在概念建立结果 final {
    存在概念操作状态 状态 = 存在概念操作状态::入口拒绝;
    std::optional<稳定编码> 概念节点;
};

struct 存在概念抽象结果 final {
    存在概念操作状态 状态 = 存在概念操作状态::入口拒绝;
    std::optional<稳定编码> 抽象概念节点;
    std::vector<存在概念特征项> 共同特征组;
};

enum class 指代目标范围 : std::uint8_t { 单个已登记强类型信息节点 = 1 };
enum class 指代参数角色 : std::uint8_t {
    目标 = 1, 作用域 = 2, 参照中心 = 3, 参照维度规则 = 4, 主体类型限制 = 5
};
enum class 指代参数类型 : std::uint8_t {
    强类型信息节点 = 1, 身份变量 = 2, 交互作用域 = 3,
    参照信息节点 = 4, 正式参照规则 = 5, 正式主体类型限制 = 6
};
struct 指代参数定义 final {
    指代参数角色 角色;
    指代参数类型 类型;
    friend bool operator==(const 指代参数定义&, const 指代参数定义&) = default;
};
enum class 指代声明规则 : std::uint8_t {
    作用域说话者 = 1, 作用域受话者 = 2, 作用域第三方 = 3,
    相对近指 = 4, 相对远指 = 5, 受限身份变量 = 6
};
struct 指代I64特征约束 final {
    指代参数角色 被约束角色;
    稳定编码 特征类型根;
    std::vector<特征概念I64闭区间> 允许值域;
    特征概念定义 特征类型快照;
    friend bool operator==(const 指代I64特征约束&,
        const 指代I64特征约束&) = default;
};
struct 指代概念定义 final {
    指代目标范围 目标范围 = 指代目标范围::单个已登记强类型信息节点;
    std::vector<指代参数定义> 参数组;
    指代声明规则 规则;
    std::vector<指代I64特征约束> 特征约束组;
    friend bool operator==(const 指代概念定义&,
        const 指代概念定义&) = default;
};
struct 指代概念信息 final {
    稳定编码 节点;
    指代概念定义 定义;
    friend bool operator==(const 指代概念信息&,
        const 指代概念信息&) = default;
};
enum class 指代定义结构状态 : std::uint8_t {
    已就绪 = 1, 入口拒绝, 内部不一致, 资源失败
};
enum class 指代概念发布阶段 : std::uint8_t {
    未发布 = 1, 已发布待核验, 已完整读回
};
enum class 指代概念写入状态 : std::uint8_t {
    已建立 = 1, 已复用, 输入拒绝, 依赖缺失,
    依赖已变化, 结构不一致, 资源失败, 读取未完成, 读取漂移
};
struct 指代概念写入结果 final {
    指代概念写入状态 状态 = 指代概念写入状态::输入拒绝;
    指代概念发布阶段 阶段 = 指代概念发布阶段::未发布;
    std::optional<稳定编码> 已发生身份;
    std::optional<指代概念信息> 信息;
    std::exception_ptr 发布后异常;
};
enum class 指代概念查询状态 : std::uint8_t {
    已找到 = 1, 未找到, 输入拒绝, 依赖缺失,
    依赖已变化, 结构不一致, 资源失败, 读取未完成, 读取漂移
};
struct 指代概念查询结果 final {
    指代概念查询状态 状态 = 指代概念查询状态::输入拒绝;
    std::optional<指代概念信息> 信息;
};

// 全局基础数据集中的物理组织根。它不是存在概念，不参与抽象和匹配。
extern 稳定编码 存在概念图根节点;

class 概念_存在类 final {
public:
    explicit 概念_存在类(概念_特征类& 特征概念服务) noexcept;

    bool 初始化() noexcept;

    指代定义结构状态 初始化指代定义结构();
    指代概念写入结果 建立或取得指代概念(const 指代概念定义& 请求);
    指代概念查询结果 查询指代概念(const 指代概念定义& 请求) const;
    指代概念查询结果 获取指代概念(稳定编码 概念节点) const;

    // 每个实际存在总是建立一个新的专属概念，不按特征相同复用。
    // 专属概念允许暂时没有特征项；空专属概念只作为该存在的概念
    // 承载节点，不参与匹配，也不能形成空抽象概念。
    存在概念建立结果 建立专属存在概念(
        稳定编码 实际存在节点,
        const std::vector<稳定编码>& 具体特征概念组) noexcept;

    // 只允许更新专属概念的完整特征投影；不自动产生抽象概念。
    存在概念操作状态 更新专属存在概念(
        稳定编码 专属概念节点,
        const std::vector<稳定编码>& 具体特征概念组) noexcept;

    // 仅供存在建立失败回滚：概念尚未进入上下位图、且存在节点尚未写入
    // 专属概念字段时才允许撤销。
    bool 撤销未绑定专属存在概念(
        稳定编码 专属概念节点,
        稳定编码 实际存在节点) noexcept;

    // 至少两个不同来源概念才可执行。默认提取全部来源的最大共同特征集；
    // 没有共同点时正常返回“无共同点”，不会建立空抽象概念。
    存在概念抽象结果 抽象共同存在概念(
        const std::vector<稳定编码>& 来源概念组) noexcept;

    // 为定义级分类建立新的抽象概念。调用者负责用名称关系赋予其语义身份；
    // 本入口至少要求一个正式下位概念，不参与“两个以上实例求共同点”的
    // 自动抽象，也不会按空特征组复用其它人工分类概念。
    存在概念建立结果 建立人工抽象存在概念(
        const std::vector<稳定编码>& 直接下位概念组) noexcept;

    std::optional<存在概念信息> 获取存在概念(
        稳定编码 概念节点) const noexcept;

    bool 是存在概念节点(稳定编码 节点) const noexcept;
    bool 是专属存在概念节点(稳定编码 节点) const noexcept;
    bool 是抽象存在概念节点(稳定编码 节点) const noexcept;

    std::vector<稳定编码> 查询上位存在概念(
        稳定编码 概念节点) const noexcept;
    std::vector<稳定编码> 查询下位存在概念(
        稳定编码 概念节点) const noexcept;

private:
    friend class 概念_名称关系类;

    bool 绑定名称关系(
        稳定编码 概念节点, 稳定编码 名称关系节点) noexcept;
    bool 解除名称关系(
        稳定编码 概念节点, 稳定编码 名称关系节点) noexcept;

    概念_特征类& 特征概念服务_;
};

} // namespace 海中鱼巣
