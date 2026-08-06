#pragma once

#include <cstdint>
#include <optional>
#include <utility>
#include <vector>

namespace 海中鱼巣 {

inline constexpr std::uint32_t 状态动态原子发布合同版本 = 1;
inline constexpr std::uint32_t 状态动态原子发布规则版本 = 1;
inline constexpr std::uint64_t 状态动态原子发布幂等域 = 0x10ULL;
inline constexpr std::uint8_t 状态动态原子发布意图组 = 7;
inline constexpr std::uint8_t 状态动态原子发布入口变体 = 4;
inline constexpr std::uint32_t 状态动态原子发布操作标签 = 0x15000603U;

enum class 状态动态原子发布状态 : std::uint8_t {
    已发布等价后状态 = 1, 已发布后状态与迁移动能 = 2, 幂等读回 = 3,
    无前状态 = 4, 入口拒绝 = 5, 未找到 = 6, 冲突 = 7,
    不可比较 = 8, 版本漂移 = 9, 许可拒绝 = 10,
    资源失败 = 11, 内部不一致 = 12
};

enum class 状态动态原子发布形状 : std::uint8_t {
    无 = 0, 仅后状态 = 1, 后状态与迁移动能 = 2
};

enum class 状态动态原子发布内部原因 : std::uint16_t {
    无 = 0, P13坏成功 = 1, P14坏成功 = 2, 规格截止不一致 = 3,
    局部键冲突 = 4, 动态后状态引用错误 = 5, L1计划形成失败 = 6,
    L1映射不完整 = 7, L1读回不完整 = 8, 状态读取请求映射失败 = 9,
    动态读取请求映射失败 = 10, 状态领域读回矛盾 = 11,
    动态领域读回矛盾 = 12, 发布代次不一致 = 13,
    幂等首次证据不完整 = 14, 执行证据材料不完整 = 15
};

struct 状态动态原子发布请求 final {
    std::uint32_t 合同版本 = 状态动态原子发布合同版本;
    std::uint32_t 规则版本 = 状态动态原子发布规则版本;
    std::uint64_t 整理幂等身份 = 0;
    实际存在I64后继状态规格请求 后状态;
    friend bool operator==(const 状态动态原子发布请求&,
        const 状态动态原子发布请求&) = default;
};

struct 状态动态原子发布首次证据 final {
    std::uint64_t 最终幂等键 = 0;
    L1确定性摘要 请求意图摘要;
    L1确定性摘要 首次执行证据摘要;
    L1确定性摘要 L1确定性结果摘要;
    std::uint64_t 发布事实代次 = 0;
    状态动态原子发布形状 形状 = 状态动态原子发布形状::无;
    std::vector<std::pair<写集本地键, 稳定编码>> 新编码映射;
    L1通用发布后读回结果 L1完整读回;
    实际存在I64基准状态读取请求 后状态读取请求;
    std::optional<状态迁移动能读取请求> 迁移动能读取请求;
    std::uint64_t P13确定性摘要 = 0;
    std::optional<std::uint64_t> P14确定性摘要;
};

struct 状态动态原子发布结果 final {
    状态动态原子发布状态 状态 = 状态动态原子发布状态::入口拒绝;
    状态动态原子发布内部原因 内部原因 = 状态动态原子发布内部原因::无;
    bool 来自精确重复 = false;
    std::optional<状态动态原子发布首次证据> 首次证据;
    std::optional<实际存在I64基准状态事实> 后状态事实;
    std::optional<状态迁移动能事实> 迁移动能事实;
    std::optional<L1失败见证身份> L1失败见证;
};

} // namespace 海中鱼巣
