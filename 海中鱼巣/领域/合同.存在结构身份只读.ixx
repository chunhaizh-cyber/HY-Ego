module;

#include <cstdint>
#include <cstddef>
#include <optional>

export module 海中鱼巣.领域.合同.存在结构身份只读;

export import 海中鱼巣.核心.服务.L1事实基座;

export namespace 海中鱼巣 {

enum class 存在结构身份只读状态 : std::uint8_t {
    已读取 = 1,
    入口拒绝 = 2,
    未找到 = 3,
    目标已退出 = 4,
    事实代次漂移 = 5,
    历史材料已清理 = 6,
    资源失败 = 7,
    内部不一致 = 8,
    未实现 = 9
};

inline constexpr std::uint32_t 存在结构身份只读合同版本 = 2;

struct 存在当前身份确认结果 final {
    存在结构身份只读状态 状态 = 存在结构身份只读状态::入口拒绝;
    std::uint32_t 合同版本 = 存在结构身份只读合同版本;
    std::uint64_t Gread = 0;

    bool 成功(std::uint64_t 请求G0) const noexcept {
        return 状态 == 存在结构身份只读状态::已读取
            && 合同版本 == 存在结构身份只读合同版本
            && 请求G0 != 0 && Gread == 请求G0;
    }
};

struct 存在身份来源生命周期 final {
    std::uint64_t 创建事实代次 = 0;
    std::optional<std::uint64_t> 退出事实代次;
};

struct 存在身份来源历史见证 final {
    稳定编码 身份{};
    稳定编码 族锚点{};
    稳定编码 族归属关系类型{};
    稳定编码 族归属关系{};
    std::uint64_t 角色 = 0;
    存在身份来源生命周期 节点生命周期;
    存在身份来源生命周期 族锚点生命周期;
    存在身份来源生命周期 关系类型生命周期;
    存在身份来源生命周期 归属关系生命周期;
};

inline bool 存在身份来源历史见证完整(
    const 存在身份来源历史见证& v,
    std::uint64_t H,
    稳定编码 请求身份) noexcept {
    if (H == 0 || !有效(请求身份) || v.身份 != 请求身份
        || v.角色 != 1)
        return false;

    const 稳定编码 编码组[]{
        v.身份, v.族锚点, v.族归属关系类型, v.族归属关系};
    for (std::size_t i = 0; i < 4; ++i) {
        if (!有效(编码组[i])) return false;
        for (std::size_t j = i + 1; j < 4; ++j)
            if (编码组[i] == 编码组[j]) return false;
    }

    const 存在身份来源生命周期 生命周期组[]{
        v.节点生命周期, v.族锚点生命周期,
        v.关系类型生命周期, v.归属关系生命周期};
    for (const auto& 生命周期 : 生命周期组)
        if (生命周期.创建事实代次 == 0
            || 生命周期.创建事实代次 > H
            || 生命周期.退出事实代次)
            return false;

    return v.节点生命周期.创建事实代次
            == v.归属关系生命周期.创建事实代次
        && v.族锚点生命周期.创建事实代次
            <= v.节点生命周期.创建事实代次
        && v.关系类型生命周期.创建事实代次
            <= v.节点生命周期.创建事实代次;
}

struct 存在身份来源历史见证读取结果 final {
    存在结构身份只读状态 状态 = 存在结构身份只读状态::入口拒绝;
    std::uint32_t 合同版本 = 存在结构身份只读合同版本;
    std::uint64_t Gread = 0;
    std::uint64_t H = 0;
    std::optional<存在身份来源历史见证> 见证;

    bool 成功(std::uint64_t 请求Gread,
        std::uint64_t 请求H,
        稳定编码 请求身份) const noexcept {
        return 状态 == 存在结构身份只读状态::已读取
            && 合同版本 == 存在结构身份只读合同版本
            && 请求Gread != 0 && 请求H != 0 && 请求H <= 请求Gread
            && 有效(请求身份) && Gread == 请求Gread && H == 请求H
            && 见证
            && 存在身份来源历史见证完整(*见证, 请求H, 请求身份);
    }
};

class 存在结构身份只读提供者 {
public:
    virtual ~存在结构身份只读提供者() = default;
    virtual bool 绑定于(const L1事实基座服务&) const noexcept = 0;
    virtual 存在当前身份确认结果 确认当前存在结构身份(
        std::uint64_t G0, 稳定编码 身份) const = 0;
    virtual 存在身份来源历史见证读取结果 读取存在身份来源历史见证(
        std::uint64_t Gread, std::uint64_t H, 稳定编码 身份) const = 0;
};

} // namespace 海中鱼巣
