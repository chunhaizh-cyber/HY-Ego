#pragma once

namespace 海中鱼巣 {
inline constexpr std::uint32_t L1事实基座持久快照格式版本_v1 = 5;

struct L1事实基座持久存储配置 final {
    std::filesystem::path 受控根;
    friend bool operator==(const L1事实基座持久存储配置&,
        const L1事实基座持久存储配置&) = default;
};

enum class L1事实基座持久恢复状态 : std::uint8_t {
    已建立空仓 = 1,
    已恢复 = 2,
    入口拒绝 = 3,
    存储占用 = 4,
    材料不完整 = 5,
    格式不支持 = 6,
    摘要不一致 = 7,
    编码或所有者冲突 = 8,
    资源失败 = 10,
    持久证据未知 = 11,
    内部不一致 = 12
};

struct L1事实基座持久恢复见证 final {
    std::uint32_t 格式版本 = L1事实基座持久快照格式版本_v1;
    std::uint64_t 快照序号 = 0;
    std::array<std::uint8_t, 32> 载荷SHA256{};
    friend bool operator==(const L1事实基座持久恢复见证&,
        const L1事实基座持久恢复见证&) = default;
};

struct L1事实基座持久恢复结果 final {
    L1事实基座持久恢复状态 状态 =
        L1事实基座持久恢复状态::内部不一致;
    std::optional<L1事实基座持久恢复见证> 恢复见证;
    friend bool operator==(const L1事实基座持久恢复结果&,
        const L1事实基座持久恢复结果&) = default;
};

} // namespace 海中鱼巣
