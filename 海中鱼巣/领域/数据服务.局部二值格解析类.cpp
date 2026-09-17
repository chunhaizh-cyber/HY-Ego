#include "数据服务.局部二值格解析类.h"

#include <bit>
#include <limits>
#include <new>
#include <utility>

namespace 海中鱼巣 {
namespace {

constexpr std::uint64_t 全零描述标识 = 0;
constexpr std::uint64_t 轮廓编码 = 1;
constexpr std::uint64_t 体素编码 = 2;

enum class 核验状态 : std::uint8_t { 成功, 格式不合法, 数量超限 };
struct 布局 final {
    std::uint64_t N{};
    std::uint64_t 最高层块数{};
    std::uint64_t 压缩层数{};
    std::uint64_t 全部块数{};
};
struct 核验结果 final {
    核验状态 状态{核验状态::格式不合法};
    布局 布局{};
    std::uint64_t 颜色项数{};
    bool 逻辑全零{};
};

constexpr bool 可转换为U64(std::size_t 值) noexcept {
    if constexpr (sizeof(std::size_t) > sizeof(std::uint64_t)) {
        return 值 <= static_cast<std::size_t>(std::numeric_limits<std::uint64_t>::max());
    }
    return true;
}
constexpr bool 可转换为SizeT(std::uint64_t 值) noexcept {
    if constexpr (sizeof(std::size_t) < sizeof(std::uint64_t)) {
        return 值 <= static_cast<std::uint64_t>(std::numeric_limits<std::size_t>::max());
    }
    return true;
}
constexpr bool 受检加法(std::uint64_t 左, std::uint64_t 右, std::uint64_t& 结果) noexcept {
    if (左 > std::numeric_limits<std::uint64_t>::max() - 右) return false;
    结果 = 左 + 右;
    return true;
}
constexpr bool 受检乘法(std::uint64_t 左, std::uint64_t 右, std::uint64_t& 结果) noexcept {
    if (左 != 0 && 右 > std::numeric_limits<std::uint64_t>::max() / 左) return false;
    结果 = 左 * 右;
    return true;
}
constexpr bool 是二倍幂分辨率(std::uint64_t N, std::uint64_t 基准) noexcept {
    if (N < 基准 || N % 基准 != 0) return false;
    const auto 商 = N / 基准;
    return 商 != 0 && (商 & (商 - 1)) == 0;
}

核验状态 计算轮廓布局(std::uint64_t N, 布局& 输出) noexcept {
    if (!是二倍幂分辨率(N, 8)) return 核验状态::格式不合法;
    输出 = 布局{};
    输出.N = N;
    for (auto 当前N = N;; 当前N /= 2) {
        std::uint64_t 平方{};
        if (!受检乘法(当前N, 当前N, 平方)) return 核验状态::数量超限;
        const auto 块数 = 平方 / 64;
        if (当前N == N) 输出.最高层块数 = 块数;
        if (!受检加法(输出.全部块数, 块数, 输出.全部块数)) return 核验状态::数量超限;
        if (当前N == 8) return 核验状态::成功;
        ++输出.压缩层数;
    }
}
核验状态 计算体素布局(std::uint64_t N, 布局& 输出) noexcept {
    if (!是二倍幂分辨率(N, 4)) return 核验状态::格式不合法;
    输出 = 布局{};
    输出.N = N;
    for (auto 当前N = N;; 当前N /= 2) {
        std::uint64_t 平方{}, 立方{};
        if (!受检乘法(当前N, 当前N, 平方) || !受检乘法(平方, 当前N, 立方)) return 核验状态::数量超限;
        const auto 块数 = 立方 / 64;
        if (当前N == N) 输出.最高层块数 = 块数;
        if (!受检加法(输出.全部块数, 块数, 输出.全部块数)) return 核验状态::数量超限;
        if (当前N == 4) return 核验状态::成功;
        ++输出.压缩层数;
    }
}

bool 读取位(const std::vector<std::uint64_t>& 组, std::uint64_t 层起点, std::uint64_t 单元索引) noexcept {
    const auto 块索引 = 层起点 + 单元索引 / 64;
    return (组[static_cast<std::size_t>(块索引)] >> (单元索引 % 64)) & 1U;
}
bool 验证轮廓OR(const std::vector<std::uint64_t>& 组, const 布局& 布局) noexcept {
    auto 上层N = 布局.N;
    std::uint64_t 上层起点{};
    auto 下层起点 = 布局.最高层块数;
    while (上层N > 8) {
        const auto 下层N = 上层N / 2;
        std::uint64_t 下层单元数{};
        if (!受检乘法(下层N, 下层N, 下层单元数)) return false;
        for (std::uint64_t j{}; j < 下层单元数; ++j) {
            const auto x = j % 下层N;
            const auto y = j / 下层N;
            const auto 左上 = (2 * y) * 上层N + 2 * x;
            const auto 应有值 = 读取位(组, 上层起点, 左上)
                || 读取位(组, 上层起点, 左上 + 1)
                || 读取位(组, 上层起点, 左上 + 上层N)
                || 读取位(组, 上层起点, 左上 + 上层N + 1);
            if (读取位(组, 下层起点, j) != 应有值) return false;
        }
        上层起点 = 下层起点;
        下层起点 += 下层单元数 / 64;
        上层N = 下层N;
    }
    return true;
}
bool 验证体素OR(const std::vector<std::uint64_t>& 组, const 布局& 布局, std::uint64_t 压缩层起点) noexcept {
    auto 上层N = 布局.N;
    std::uint64_t 上层起点{1};
    auto 下层起点 = 压缩层起点;
    while (上层N > 4) {
        const auto 下层N = 上层N / 2;
        std::uint64_t 平方{}, 单元数{};
        if (!受检乘法(下层N, 下层N, 平方) || !受检乘法(平方, 下层N, 单元数)) return false;
        for (std::uint64_t j{}; j < 单元数; ++j) {
            const auto x = j % 下层N;
            const auto yz = j / 下层N;
            const auto y = yz % 下层N;
            const auto z = yz / 下层N;
            bool 应有值{};
            for (std::uint64_t dz{}; dz < 2; ++dz) for (std::uint64_t dy{}; dy < 2; ++dy) for (std::uint64_t dx{}; dx < 2; ++dx) {
                const auto 子索引 = ((2 * z + dz) * 上层N + (2 * y + dy)) * 上层N + 2 * x + dx;
                应有值 = 应有值 || 读取位(组, 上层起点, 子索引);
            }
            if (读取位(组, 下层起点, j) != 应有值) return false;
        }
        上层起点 = 下层起点;
        下层起点 += 单元数 / 64;
        上层N = 下层N;
    }
    return true;
}
bool 全部为零(const std::vector<std::uint64_t>& 组, std::uint64_t 起点, std::uint64_t 数量) noexcept {
    for (std::uint64_t i{}; i < 数量; ++i) if (组[static_cast<std::size_t>(起点 + i)] != 0) return false;
    return true;
}
bool 是描述(const std::vector<std::uint64_t>& 组) noexcept {
    return 组.size() == 3 && 组[0] == 全零描述标识 && (组[1] == 轮廓编码 || 组[1] == 体素编码);
}
核验结果 核验轮廓原始完整组(const std::vector<std::uint64_t>& 组) noexcept {
    核验结果 结果;
    if (!可转换为U64(组.size()) || 组.empty() || 是描述(组)) {
        结果.状态 = 可转换为U64(组.size()) ? 核验状态::格式不合法 : 核验状态::数量超限;
        return 结果;
    }
    const auto 长度 = static_cast<std::uint64_t>(组.size());
    for (auto N = std::uint64_t{8};;) {
        布局 布局;
        const auto 状态 = 计算轮廓布局(N, 布局);
        if (状态 != 核验状态::成功) { 结果.状态 = 状态; return 结果; }
        if (布局.全部块数 == 长度) {
            结果.状态 = 验证轮廓OR(组, 布局) ? 核验状态::成功 : 核验状态::格式不合法;
            结果.布局 = 布局;
            结果.逻辑全零 = 结果.状态 == 核验状态::成功 && 全部为零(组, 0, 长度);
            return 结果;
        }
        if (布局.全部块数 > 长度) { 结果.状态 = 核验状态::格式不合法; return 结果; }
        if (N > std::numeric_limits<std::uint64_t>::max() / 2) { 结果.状态 = 核验状态::数量超限; return 结果; }
        N *= 2;
    }
}
核验结果 核验体素原始完整组(const std::vector<std::uint64_t>& 组) noexcept {
    核验结果 结果;
    if (!可转换为U64(组.size()) || 组.empty() || 是描述(组)) {
        结果.状态 = 可转换为U64(组.size()) ? 核验状态::格式不合法 : 核验状态::数量超限;
        return 结果;
    }
    布局 布局;
    const auto 布局状态 = 计算体素布局(组[0], 布局);
    if (布局状态 != 核验状态::成功) { 结果.状态 = 布局状态; return 结果; }
    const auto 长度 = static_cast<std::uint64_t>(组.size());
    std::uint64_t 固定段长度{};
    if (!受检加法(1, 布局.全部块数, 固定段长度)) { 结果.状态 = 核验状态::数量超限; return 结果; }
    if (固定段长度 > 长度) { 结果.状态 = 核验状态::格式不合法; return 结果; }
    const auto 颜色项数 = 长度 - 固定段长度;
    std::uint64_t 占用数{};
    for (std::uint64_t i{}; i < 布局.最高层块数; ++i) {
        const auto 位数 = static_cast<std::uint64_t>(std::popcount(组[static_cast<std::size_t>(1 + i)]));
        if (!受检加法(占用数, 位数, 占用数)) { 结果.状态 = 核验状态::数量超限; return 结果; }
    }
    if (颜色项数 != 占用数) { 结果.状态 = 核验状态::格式不合法; return 结果; }
    std::uint64_t 压缩层起点{};
    if (!受检加法(1, 布局.最高层块数, 压缩层起点) || !受检加法(压缩层起点, 颜色项数, 压缩层起点) || !可转换为SizeT(压缩层起点)) {
        结果.状态 = 核验状态::数量超限; return 结果;
    }
    for (std::uint64_t i{}; i < 颜色项数; ++i) {
        if ((组[static_cast<std::size_t>(1 + 布局.最高层块数 + i)] >> 24) != 0) { 结果.状态 = 核验状态::格式不合法; return 结果; }
    }
    结果.状态 = 验证体素OR(组, 布局, 压缩层起点) ? 核验状态::成功 : 核验状态::格式不合法;
    结果.布局 = 布局;
    结果.颜色项数 = 颜色项数;
    结果.逻辑全零 = 结果.状态 == 核验状态::成功 && 占用数 == 0;
    return 结果;
}
局部二值格规范化状态_v1 转换规范化状态(核验状态 状态) noexcept {
    return 状态 == 核验状态::数量超限 ? 局部二值格规范化状态_v1::数量超限 : 局部二值格规范化状态_v1::格式不合法;
}
局部二值格解析状态_v3 转换解析状态(核验状态 状态) noexcept {
    return 状态 == 核验状态::数量超限 ? 局部二值格解析状态_v3::数量超限 : 局部二值格解析状态_v3::格式不合法;
}
bool 是有效类型(局部二值格类型_v1 类型) noexcept { return 类型 == 局部二值格类型_v1::轮廓 || 类型 == 局部二值格类型_v1::体素; }

} // namespace

局部二值格规范化载荷_v1::局部二值格规范化载荷_v1(局部二值格类型_v1 类型, std::vector<std::uint64_t>&& U64组) noexcept
    : 类型_(类型), U64组_(std::move(U64组)) {}
局部二值格类型_v1 局部二值格规范化载荷_v1::获取类型() const noexcept { return 类型_; }
const std::vector<std::uint64_t>& 局部二值格规范化载荷_v1::获取U64组() const noexcept { return U64组_; }

bool 局部二值格规范化结果_v1::成功(const 局部二值格规范化请求_v1& 请求) const noexcept {
    return 版本 == 1 && 请求.版本 == 1 && 请求.原始完整组 != nullptr && !请求.原始完整组->empty()
        && 是有效类型(请求.类型) && 状态 == 局部二值格规范化状态_v1::已规范化 && 载荷.has_value()
        && 载荷->获取类型() == 请求.类型 && !载荷->获取U64组().empty();
}
bool 局部二值轮廓解析结果_v3::成功(const 局部二值格解析请求_v3& 请求) const noexcept {
    return 版本 == 3 && 请求.版本 == 3 && 请求.规范化载荷 != nullptr
        && 请求.规范化载荷->获取类型() == 局部二值格类型_v1::轮廓 && 状态 == 局部二值格解析状态_v3::已解析
        && 格式.has_value() && 格式->N != 0 && 格式->最高层位图块数 != 0
        && (格式->编码形态 == 局部二值格编码形态_v3::普通完整 || 格式->编码形态 == 局部二值格编码形态_v3::全零描述);
}
bool 局部二值体素解析结果_v3::成功(const 局部二值格解析请求_v3& 请求) const noexcept {
    return 版本 == 3 && 请求.版本 == 3 && 请求.规范化载荷 != nullptr
        && 请求.规范化载荷->获取类型() == 局部二值格类型_v1::体素 && 状态 == 局部二值格解析状态_v3::已解析
        && 格式.has_value() && 格式->N != 0 && 格式->最高层位图块数 != 0
        && (格式->编码形态 == 局部二值格编码形态_v3::普通完整 || 格式->编码形态 == 局部二值格编码形态_v3::全零描述);
}

局部二值格规范化结果_v1 局部二值格解析数据服务::规范化局部二值格(const 局部二值格规范化请求_v1& 请求) const noexcept {
    局部二值格规范化结果_v1 结果;
    try {
        if (请求.版本 != 1 || !是有效类型(请求.类型) || 请求.原始完整组 == nullptr || 请求.原始完整组->empty()) return 结果;
        const auto& 组 = *请求.原始完整组;
        const auto 核验 = 请求.类型 == 局部二值格类型_v1::轮廓 ? 核验轮廓原始完整组(组) : 核验体素原始完整组(组);
        if (核验.状态 != 核验状态::成功) { 结果.状态 = 转换规范化状态(核验.状态); return 结果; }
        std::vector<std::uint64_t> 输出 = 核验.逻辑全零
            ? std::vector<std::uint64_t>{全零描述标识, 请求.类型 == 局部二值格类型_v1::轮廓 ? 轮廓编码 : 体素编码, 核验.布局.N}
            : 组;
        局部二值格规范化载荷_v1 载荷(请求.类型, std::move(输出));
        结果.载荷 = std::move(载荷);
        结果.状态 = 局部二值格规范化状态_v1::已规范化;
        return 结果;
    } catch (const std::bad_alloc&) {
        结果.载荷.reset(); 结果.状态 = 局部二值格规范化状态_v1::资源失败; return 结果;
    } catch (...) {
        结果.载荷.reset(); 结果.状态 = 局部二值格规范化状态_v1::内部不一致; return 结果;
    }
}

局部二值轮廓解析结果_v3 局部二值格解析数据服务::解析局部二值轮廓(const 局部二值格解析请求_v3& 请求) const noexcept {
    局部二值轮廓解析结果_v3 结果;
    try {
        if (请求.版本 != 3 || 请求.规范化载荷 == nullptr || 请求.规范化载荷->获取类型() != 局部二值格类型_v1::轮廓) return 结果;
        const auto& 组 = 请求.规范化载荷->获取U64组();
        if (组.size() == 3 && 组[0] == 全零描述标识 && 组[1] == 轮廓编码) {
            布局 布局; const auto 状态 = 计算轮廓布局(组[2], 布局);
            if (状态 != 核验状态::成功) { 结果.状态 = 转换解析状态(状态); return 结果; }
            结果.格式 = 局部二值轮廓格式_v3{布局.N, 布局.最高层块数, 布局.压缩层数, 局部二值格编码形态_v3::全零描述};
        } else {
            const auto 核验 = 核验轮廓原始完整组(组);
            if (核验.状态 != 核验状态::成功 || 核验.逻辑全零) { 结果.状态 = 转换解析状态(核验.状态); return 结果; }
            结果.格式 = 局部二值轮廓格式_v3{核验.布局.N, 核验.布局.最高层块数, 核验.布局.压缩层数, 局部二值格编码形态_v3::普通完整};
        }
        结果.状态 = 局部二值格解析状态_v3::已解析; return 结果;
    } catch (...) { 结果.格式.reset(); 结果.状态 = 局部二值格解析状态_v3::内部不一致; return 结果; }
}

局部二值体素解析结果_v3 局部二值格解析数据服务::解析局部二值体素(const 局部二值格解析请求_v3& 请求) const noexcept {
    局部二值体素解析结果_v3 结果;
    try {
        if (请求.版本 != 3 || 请求.规范化载荷 == nullptr || 请求.规范化载荷->获取类型() != 局部二值格类型_v1::体素) return 结果;
        const auto& 组 = 请求.规范化载荷->获取U64组();
        if (组.size() == 3 && 组[0] == 全零描述标识 && 组[1] == 体素编码) {
            布局 布局; const auto 状态 = 计算体素布局(组[2], 布局);
            if (状态 != 核验状态::成功) { 结果.状态 = 转换解析状态(状态); return 结果; }
            结果.格式 = 局部二值体素格式_v3{布局.N, 布局.最高层块数, 布局.压缩层数, 0, 局部二值格编码形态_v3::全零描述};
        } else {
            const auto 核验 = 核验体素原始完整组(组);
            if (核验.状态 != 核验状态::成功 || 核验.逻辑全零) { 结果.状态 = 转换解析状态(核验.状态); return 结果; }
            结果.格式 = 局部二值体素格式_v3{核验.布局.N, 核验.布局.最高层块数, 核验.布局.压缩层数, 核验.颜色项数, 局部二值格编码形态_v3::普通完整};
        }
        结果.状态 = 局部二值格解析状态_v3::已解析; return 结果;
    } catch (...) { 结果.格式.reset(); 结果.状态 = 局部二值格解析状态_v3::内部不一致; return 结果; }
}

} // namespace 海中鱼巣
