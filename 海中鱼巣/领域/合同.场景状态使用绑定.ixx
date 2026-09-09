module;

#include <cstdint>
#include <optional>
#include <vector>

export module 海中鱼巣.领域.合同.场景状态使用绑定;

export import 海中鱼巣.领域.数据服务.状态类;

export namespace 海中鱼巣 {

inline constexpr std::uint32_t 场景状态使用绑定合同版本 = 1;

struct 状态使用绑定身份 final {
    稳定编码 编码{};
    friend bool operator==(const 状态使用绑定身份&,
        const 状态使用绑定身份&) = default;
};

inline bool 有效(状态使用绑定身份 v) noexcept { return 有效(v.编码); }

struct 状态使用绑定信息 final {
    状态使用绑定身份 身份;
    稳定编码 发生场景{};
    稳定编码 被描述存在{};
    状态信息身份 状态;
    friend bool operator==(const 状态使用绑定信息&,
        const 状态使用绑定信息&) = default;
};

struct 状态使用绑定事实 final {
    std::uint64_t Gread = 0;
    std::uint64_t H = 0;
    状态使用绑定信息 信息;
    稳定编码 场景成员关系{};
    稳定编码 存在成员关系{};
    稳定编码 状态成员关系{};
    std::uint64_t 创建事实代次 = 0;
    std::optional<std::uint64_t> 退出事实代次;
    friend bool operator==(const 状态使用绑定事实&,
        const 状态使用绑定事实&) = default;
};

inline bool 状态使用绑定事实完整(const 状态使用绑定事实& v) noexcept {
    if (v.Gread == 0 || v.H == 0 || v.H > v.Gread
        || !有效(v.信息.身份) || !有效(v.信息.发生场景)
        || !有效(v.信息.被描述存在) || !有效(v.信息.状态)
        || !有效(v.场景成员关系) || !有效(v.存在成员关系)
        || !有效(v.状态成员关系) || v.创建事实代次 == 0
        || v.创建事实代次 > v.H)
        return false;

    const auto B = v.信息.身份.编码;
    const auto C = v.信息.发生场景;
    const auto E = v.信息.被描述存在;
    const auto S = v.信息.状态.编码;
    if (B == C || B == E || B == S || S == C || S == E)
        return false;

    const 稳定编码 关系组[]{
        v.场景成员关系, v.存在成员关系, v.状态成员关系};
    for (std::size_t i = 0; i < 3; ++i) {
        if (关系组[i] == B || 关系组[i] == C
            || 关系组[i] == E || 关系组[i] == S)
            return false;
        for (std::size_t j = i + 1; j < 3; ++j)
            if (关系组[i] == 关系组[j]) return false;
    }

    return !v.退出事实代次
        || (*v.退出事实代次 > v.创建事实代次
            && v.H < *v.退出事实代次);
}

struct 状态使用绑定结构交付 final {
    稳定编码 场景族锚点{};
    稳定编码 场景族归属关系类型{};
    稳定编码 状态使用绑定成员关系类型{};
    friend bool operator==(const 状态使用绑定结构交付&,
        const 状态使用绑定结构交付&) = default;
};

enum class 状态使用绑定数据状态 : std::uint8_t {
    已创建=1, 精确重复=2, 已读取=3, 已退出=4,
    入口拒绝=5, 未找到=6, 目标已退出=7,
    场景未找到=8, 场景已退出=9,
    存在未找到=10, 存在已退出=11,
    状态未找到=12, 状态已退出=13,
    旧格式不支持=14, 事实代次漂移=15,
    幂等冲突=16, 引用冲突=17, 数量预算不足=18,
    历史材料已清理=19, 资源失败=20,
    内部不一致=21, 已可能发布=22, 未实现=23
};

struct 状态使用绑定结果头 final {
    状态使用绑定数据状态 状态 = 状态使用绑定数据状态::入口拒绝;
    std::uint32_t 合同版本 = 场景状态使用绑定合同版本;
    std::uint64_t 事实截止代次 = 0;
    std::optional<std::uint64_t> 变更事实代次;
};

struct 状态使用绑定创建请求 final {
    std::uint32_t 合同版本 = 1;
    std::uint64_t G0 = 0;
    L1所有者范围写入幂等身份 幂等身份{};
    稳定编码 发生场景{}, 被描述存在{};
    状态信息身份 状态;
};
struct 状态使用绑定当前读取请求 final {
    std::uint32_t 合同版本 = 1;
    std::uint64_t G0 = 0;
    状态使用绑定身份 身份;
};
struct 状态使用绑定历史读取请求 final {
    std::uint32_t 合同版本 = 1;
    std::uint64_t Gread = 0, H = 0;
    状态使用绑定身份 身份;
};
struct 状态使用绑定按发生场景查询请求 final {
    std::uint32_t 合同版本 = 1;
    std::uint64_t G0 = 0;
    稳定编码 发生场景{};
    std::uint64_t 最大候选数量 = 0;
};
struct 状态使用绑定按被描述存在查询请求 final {
    std::uint32_t 合同版本 = 1;
    std::uint64_t G0 = 0;
    稳定编码 被描述存在{};
    std::uint64_t 最大候选数量 = 0;
};
struct 状态使用绑定按状态查询请求 final {
    std::uint32_t 合同版本 = 1;
    std::uint64_t G0 = 0;
    状态信息身份 状态;
    std::uint64_t 最大候选数量 = 0;
};
struct 状态使用绑定退出请求 final {
    std::uint32_t 合同版本 = 1;
    std::uint64_t G0 = 0;
    L1所有者范围写入幂等身份 幂等身份{};
    状态使用绑定身份 身份;
};

struct 状态使用绑定创建结果 final {
    状态使用绑定结果头 结果头;
    std::optional<状态使用绑定事实> 绑定;
    bool 成功() const noexcept {
        return (结果头.状态 == 状态使用绑定数据状态::已创建
                || 结果头.状态 == 状态使用绑定数据状态::精确重复)
            && 结果头.合同版本 == 场景状态使用绑定合同版本
            && 结果头.事实截止代次 != 0 && 结果头.变更事实代次
            && *结果头.变更事实代次 == 结果头.事实截止代次
            && 绑定 && 状态使用绑定事实完整(*绑定)
            && 绑定->Gread == 结果头.事实截止代次
            && 绑定->H == 结果头.事实截止代次
            && 绑定->创建事实代次 == 结果头.事实截止代次
            && !绑定->退出事实代次;
    }
};

struct 状态使用绑定读取结果 final {
    状态使用绑定结果头 结果头;
    std::optional<状态使用绑定事实> 绑定;
    bool 成功() const noexcept {
        return 结果头.状态 == 状态使用绑定数据状态::已读取
            && 结果头.合同版本 == 场景状态使用绑定合同版本
            && 结果头.事实截止代次 != 0 && !结果头.变更事实代次
            && 绑定 && 状态使用绑定事实完整(*绑定)
            && 绑定->Gread == 结果头.事实截止代次;
    }
};

struct 状态使用绑定组查询结果 final {
    状态使用绑定结果头 结果头;
    std::vector<状态使用绑定事实> 绑定组;
    bool 成功() const noexcept {
        if (结果头.状态 != 状态使用绑定数据状态::已读取
            || 结果头.合同版本 != 场景状态使用绑定合同版本
            || 结果头.事实截止代次 == 0 || 结果头.变更事实代次)
            return false;
        稳定编码 上一个{};
        for (const auto& v : 绑定组) {
            if (!状态使用绑定事实完整(v)
                || v.Gread != 结果头.事实截止代次
                || v.H != 结果头.事实截止代次
                || (有效(上一个) && !(上一个 < v.信息.身份.编码)))
                return false;
            上一个 = v.信息.身份.编码;
        }
        return true;
    }
};

struct 状态使用绑定退出结果 final {
    状态使用绑定结果头 结果头;
    std::optional<状态使用绑定事实> 已退出绑定;
    bool 成功() const noexcept {
        return (结果头.状态 == 状态使用绑定数据状态::已退出
                || 结果头.状态 == 状态使用绑定数据状态::精确重复)
            && 结果头.合同版本 == 场景状态使用绑定合同版本
            && 结果头.事实截止代次 > 1 && 结果头.变更事实代次
            && *结果头.变更事实代次 == 结果头.事实截止代次
            && 已退出绑定 && 状态使用绑定事实完整(*已退出绑定)
            && 已退出绑定->Gread == 结果头.事实截止代次
            && 已退出绑定->H == 结果头.事实截止代次 - 1
            && 已退出绑定->退出事实代次
            && *已退出绑定->退出事实代次 == 结果头.事实截止代次;
    }
};

enum class 存在结构身份只读状态 : std::uint8_t {
    已读取=1, 入口拒绝=2, 未找到=3, 目标已退出=4,
    事实代次漂移=5, 历史材料已清理=6, 资源失败=7,
    内部不一致=8, 未实现=9
};

struct 存在结构身份只读结果 final {
    存在结构身份只读状态 状态 = 存在结构身份只读状态::入口拒绝;
    std::uint32_t 合同版本 = 1;
    std::uint64_t Gread = 0, H = 0;
    稳定编码 身份{}, 族锚点{}, 族归属关系类型{}, 族归属关系{};
    std::uint64_t 创建事实代次 = 0;
    std::optional<std::uint64_t> 退出事实代次;
    bool 成功() const noexcept {
        return 状态 == 存在结构身份只读状态::已读取
            && 合同版本 == 1 && H != 0 && H <= Gread
            && 有效(身份) && 有效(族锚点) && 有效(族归属关系类型)
            && 有效(族归属关系) && 创建事实代次 != 0
            && 创建事实代次 <= H
            && (!退出事实代次
                || (*退出事实代次 > 创建事实代次
                    && H < *退出事实代次));
    }
};

class 存在结构身份只读提供者 {
public:
    virtual ~存在结构身份只读提供者() = default;
    virtual bool 绑定于(const L1事实基座服务&) const noexcept = 0;
    virtual 存在结构身份只读结果 确认当前存在结构身份(
        std::uint64_t G0, 稳定编码 身份) const = 0;
    virtual 存在结构身份只读结果 读取存在身份来源历史见证(
        std::uint64_t Gread, std::uint64_t H, 稳定编码 身份) const = 0;
};

class 状态使用绑定只读提供者 {
public:
    virtual ~状态使用绑定只读提供者() = default;
    virtual bool 绑定于(const L1事实基座服务&) const noexcept = 0;
    virtual 状态使用绑定读取结果 读取当前状态使用绑定(
        const 状态使用绑定当前读取请求&) const = 0;
    virtual 状态使用绑定读取结果 读取状态使用绑定历史(
        const 状态使用绑定历史读取请求&) const = 0;
};

} // namespace 海中鱼巣
