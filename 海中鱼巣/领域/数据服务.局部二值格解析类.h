#pragma once

#include <cstdint>
#include <optional>
#include <vector>

namespace 海中鱼巣 {

enum class 局部二值格类型_v1 : std::uint8_t { 轮廓 = 1, 体素 = 2 };
enum class 局部二值格规范化状态_v1 : std::uint8_t {
    已规范化 = 1, 入口拒绝 = 8, 格式不合法 = 9, 数量超限 = 10, 资源失败 = 26, 内部不一致 = 27
};
enum class 局部二值格解析状态_v3 : std::uint8_t {
    已解析 = 1, 入口拒绝 = 8, 格式不合法 = 9, 数量超限 = 10, 内部不一致 = 27
};
enum class 局部二值格编码形态_v3 : std::uint8_t { 普通完整 = 1, 全零描述 = 2 };

struct 局部二值格规范化请求_v1 final {
    std::uint32_t 版本{1};
    局部二值格类型_v1 类型{};
    const std::vector<std::uint64_t>* 原始完整组{nullptr};
};

class 局部二值格规范化载荷_v1 final {
public:
    局部二值格规范化载荷_v1(const 局部二值格规范化载荷_v1&) = delete;
    局部二值格规范化载荷_v1& operator=(const 局部二值格规范化载荷_v1&) = delete;
    局部二值格规范化载荷_v1(局部二值格规范化载荷_v1&&) noexcept = default;
    局部二值格规范化载荷_v1& operator=(局部二值格规范化载荷_v1&&) noexcept = default;

    局部二值格类型_v1 获取类型() const noexcept;
    const std::vector<std::uint64_t>& 获取U64组() const noexcept;

private:
    friend class 局部二值格解析数据服务;
    局部二值格规范化载荷_v1(局部二值格类型_v1 类型, std::vector<std::uint64_t>&& U64组) noexcept;

    局部二值格类型_v1 类型_{};
    std::vector<std::uint64_t> U64组_;
};

struct 局部二值格规范化结果_v1 final {
    std::uint32_t 版本{1};
    局部二值格规范化状态_v1 状态{局部二值格规范化状态_v1::入口拒绝};
    std::optional<局部二值格规范化载荷_v1> 载荷;
    bool 成功(const 局部二值格规范化请求_v1& 请求) const noexcept;
};

struct 局部二值格解析请求_v3 final {
    std::uint32_t 版本{3};
    const 局部二值格规范化载荷_v1* 规范化载荷{nullptr};
};

struct 局部二值轮廓格式_v3 final {
    std::uint64_t N{0};
    std::uint64_t 最高层位图块数{0};
    std::uint64_t 压缩层数{0};
    局部二值格编码形态_v3 编码形态{};
};
struct 局部二值体素格式_v3 final {
    std::uint64_t N{0};
    std::uint64_t 最高层位图块数{0};
    std::uint64_t 压缩层数{0};
    std::uint64_t 颜色项数{0};
    局部二值格编码形态_v3 编码形态{};
};

struct 局部二值轮廓解析结果_v3 final {
    std::uint32_t 版本{3};
    局部二值格解析状态_v3 状态{局部二值格解析状态_v3::入口拒绝};
    std::optional<局部二值轮廓格式_v3> 格式;
    bool 成功(const 局部二值格解析请求_v3& 请求) const noexcept;
};
struct 局部二值体素解析结果_v3 final {
    std::uint32_t 版本{3};
    局部二值格解析状态_v3 状态{局部二值格解析状态_v3::入口拒绝};
    std::optional<局部二值体素格式_v3> 格式;
    bool 成功(const 局部二值格解析请求_v3& 请求) const noexcept;
};

// 唯一 DATA-L2 局部二值格格式服务：规范化原始完整组，并只解析成功规范化载荷。
class 局部二值格解析数据服务 final {
public:
    局部二值格规范化结果_v1 规范化局部二值格(const 局部二值格规范化请求_v1& 请求) const noexcept;
    局部二值轮廓解析结果_v3 解析局部二值轮廓(const 局部二值格解析请求_v3& 请求) const noexcept;
    局部二值体素解析结果_v3 解析局部二值体素(const 局部二值格解析请求_v3& 请求) const noexcept;
};

} // namespace 海中鱼巣
