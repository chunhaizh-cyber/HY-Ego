#pragma once

#include <cstdint>
#include <optional>
#include <vector>

namespace 海中鱼巣 {

enum class 局部二值格解析状态_v1 : std::uint8_t {
    已解析 = 1,
    入口拒绝 = 8,
    格式不合法 = 9,
    数量超限 = 10,
    内部不一致 = 27
};

struct 局部二值格解析请求_v1 {
    std::uint32_t 版本 = 1;
    const std::vector<std::uint64_t>* U64组 = nullptr;
};

struct 局部二值轮廓格式_v1 {
    std::uint64_t N = 0;
    std::uint64_t 位图块数 = 0;
};

struct 局部二值体素格式_v1 {
    std::uint64_t N = 0;
    std::uint64_t 占用位图块数 = 0;
    std::uint64_t 颜色项数 = 0;
};

struct 局部二值轮廓解析结果_v1 {
    std::uint32_t 版本 = 1;
    局部二值格解析状态_v1 状态 = 局部二值格解析状态_v1::入口拒绝;
    std::optional<局部二值轮廓格式_v1> 格式;

    bool 成功(const 局部二值格解析请求_v1& 请求) const noexcept;
};

struct 局部二值体素解析结果_v1 {
    std::uint32_t 版本 = 1;
    局部二值格解析状态_v1 状态 = 局部二值格解析状态_v1::入口拒绝;
    std::optional<局部二值体素格式_v1> 格式;

    bool 成功(const 局部二值格解析请求_v1& 请求) const noexcept;
};

// 只验证完整 U64 组的局部二值格格式；不读取或写入任何项目事实。
class 局部二值格解析数据服务 final {
public:
    局部二值轮廓解析结果_v1 解析局部二值轮廓(
        const 局部二值格解析请求_v1& 请求) const noexcept;

    局部二值体素解析结果_v1 解析局部二值体素(
        const 局部二值格解析请求_v1& 请求) const noexcept;
};

} // namespace 海中鱼巣
