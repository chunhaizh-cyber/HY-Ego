#pragma once

namespace 海中鱼巣 {

inline constexpr std::uint32_t L1事实基座持久恢复合同版本_v1 = 1;
inline constexpr std::uint32_t L1事实基座持久快照格式版本_v1 = 4;

struct L1事实基座持久存储配置_v1 final {
    std::uint32_t 合同版本 = L1事实基座持久恢复合同版本_v1;
    std::filesystem::path 受控根;
    friend bool operator==(const L1事实基座持久存储配置_v1&,
        const L1事实基座持久存储配置_v1&) = default;
};

enum class L1事实基座持久恢复状态_v1 : std::uint8_t {
    已建立空仓 = 1,
    已恢复 = 2,
    入口拒绝 = 3,
    存储占用 = 4,
    材料不完整 = 5,
    格式不支持 = 6,
    摘要不一致 = 7,
    编码或所有者冲突 = 8,
    事实代次漂移 = 9,
    资源失败 = 10,
    持久证据未知 = 11,
    内部不一致 = 12
};

struct L1事实基座持久恢复见证_v1 final {
    std::uint32_t 格式版本 = L1事实基座持久快照格式版本_v1;
    std::uint64_t 快照序号 = 0;
    std::uint64_t 事实代次 = 0;
    std::array<std::uint8_t, 32> 载荷SHA256{};
    friend bool operator==(const L1事实基座持久恢复见证_v1&,
        const L1事实基座持久恢复见证_v1&) = default;
};

struct L1事实基座持久恢复结果_v1 final {
    L1事实基座持久恢复状态_v1 状态 =
        L1事实基座持久恢复状态_v1::内部不一致;
    std::optional<L1事实基座持久恢复见证_v1> 恢复见证;
    friend bool operator==(const L1事实基座持久恢复结果_v1&,
        const L1事实基座持久恢复结果_v1&) = default;
};

} // namespace 海中鱼巣
