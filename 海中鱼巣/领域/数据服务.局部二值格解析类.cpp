#include "数据服务.局部二值格解析类.h"

#include <bit>
#include <limits>

namespace 海中鱼巣 {
namespace {

constexpr std::uint64_t 全零描述标识 = 0;
constexpr std::uint64_t 局部二值轮廓类型编码 = 1;
constexpr std::uint64_t 局部二值体素类型编码 = 2;

constexpr bool 可转换为U64(const std::size_t 值) noexcept {
    if constexpr (sizeof(std::size_t) > sizeof(std::uint64_t)) {
        return 值 <= static_cast<std::size_t>(std::numeric_limits<std::uint64_t>::max());
    }
    return true;
}

constexpr bool 可转换为SizeT(const std::uint64_t 值) noexcept {
    if constexpr (sizeof(std::size_t) < sizeof(std::uint64_t)) {
        return 值 <= static_cast<std::uint64_t>(std::numeric_limits<std::size_t>::max());
    }
    return true;
}

constexpr bool 受检加法(
    const std::uint64_t 左,
    const std::uint64_t 右,
    std::uint64_t& 结果) noexcept {
    if (左 > std::numeric_limits<std::uint64_t>::max() - 右) {
        return false;
    }
    结果 = 左 + 右;
    return true;
}

constexpr bool 受检乘法(
    const std::uint64_t 左,
    const std::uint64_t 右,
    std::uint64_t& 结果) noexcept {
    if (左 != 0 && 右 > std::numeric_limits<std::uint64_t>::max() / 左) {
        return false;
    }
    结果 = 左 * 右;
    return true;
}

constexpr bool 是二倍幂分辨率(const std::uint64_t N, const std::uint64_t 基准) noexcept {
    if (N < 基准 || N % 基准 != 0) {
        return false;
    }
    const auto 商 = N / 基准;
    return 商 != 0 && (商 & (商 - 1)) == 0;
}

enum class 布局计算结果 : std::uint8_t {
    成功,
    非法分辨率,
    数量超限
};

布局计算结果 计算轮廓布局(
    const std::uint64_t N,
    std::uint64_t& 最高层块数,
    std::uint64_t& 压缩层数,
    std::uint64_t& 全部位图块数) noexcept {
    if (!是二倍幂分辨率(N, 8)) {
        return 布局计算结果::非法分辨率;
    }

    最高层块数 = 0;
    压缩层数 = 0;
    全部位图块数 = 0;
    auto 当前N = N;
    bool 首层 = true;
    for (;;) {
        std::uint64_t 平方 = 0;
        if (!受检乘法(当前N, 当前N, 平方)) {
            return 布局计算结果::数量超限;
        }
        const auto 本层块数 = 平方 / 64;
        if (首层) {
            最高层块数 = 本层块数;
            首层 = false;
        }
        if (!受检加法(全部位图块数, 本层块数, 全部位图块数)) {
            return 布局计算结果::数量超限;
        }
        if (当前N == 8) {
            return 布局计算结果::成功;
        }
        ++压缩层数;
        当前N /= 2;
    }
}

布局计算结果 计算体素布局(
    const std::uint64_t N,
    std::uint64_t& 最高层块数,
    std::uint64_t& 压缩层数,
    std::uint64_t& 全部位图块数) noexcept {
    if (!是二倍幂分辨率(N, 4)) {
        return 布局计算结果::非法分辨率;
    }

    最高层块数 = 0;
    压缩层数 = 0;
    全部位图块数 = 0;
    auto 当前N = N;
    bool 首层 = true;
    for (;;) {
        std::uint64_t 平方 = 0;
        std::uint64_t 立方 = 0;
        if (!受检乘法(当前N, 当前N, 平方)
            || !受检乘法(平方, 当前N, 立方)) {
            return 布局计算结果::数量超限;
        }
        const auto 本层块数 = 立方 / 64;
        if (首层) {
            最高层块数 = 本层块数;
            首层 = false;
        }
        if (!受检加法(全部位图块数, 本层块数, 全部位图块数)) {
            return 布局计算结果::数量超限;
        }
        if (当前N == 4) {
            return 布局计算结果::成功;
        }
        ++压缩层数;
        当前N /= 2;
    }
}

bool 读取位(
    const std::vector<std::uint64_t>& 组,
    const std::uint64_t 层起点,
    const std::uint64_t 单元索引) noexcept {
    const auto 块索引 = 层起点 + 单元索引 / 64;
    return (组[static_cast<std::size_t>(块索引)] >> (单元索引 % 64)) & 1U;
}

bool 验证轮廓压缩层(
    const std::vector<std::uint64_t>& 组,
    const std::uint64_t N,
    const std::uint64_t 最高层块数) noexcept {
    auto 上层N = N;
    auto 上层起点 = std::uint64_t{0};
    auto 下层起点 = 最高层块数;
    while (上层N > 8) {
        const auto 下层N = 上层N / 2;
        std::uint64_t 下层单元数 = 0;
        if (!受检乘法(下层N, 下层N, 下层单元数)) {
            return false;
        }
        for (std::uint64_t j = 0; j < 下层单元数; ++j) {
            const auto x = j % 下层N;
            const auto y = j / 下层N;
            const auto 左上 = (2 * y) * 上层N + 2 * x;
            const bool 应有值 = 读取位(组, 上层起点, 左上)
                || 读取位(组, 上层起点, 左上 + 1)
                || 读取位(组, 上层起点, 左上 + 上层N)
                || 读取位(组, 上层起点, 左上 + 上层N + 1);
            if (读取位(组, 下层起点, j) != 应有值) {
                return false;
            }
        }
        上层起点 = 下层起点;
        下层起点 += 下层单元数 / 64;
        上层N = 下层N;
    }
    return true;
}

bool 验证体素压缩层(
    const std::vector<std::uint64_t>& 组,
    const std::uint64_t N,
    const std::uint64_t 压缩层起点) noexcept {
    auto 上层N = N;
    auto 上层起点 = std::uint64_t{1};
    auto 下层起点 = 压缩层起点;
    while (上层N > 4) {
        const auto 下层N = 上层N / 2;
        std::uint64_t 下层平方 = 0;
        std::uint64_t 下层单元数 = 0;
        if (!受检乘法(下层N, 下层N, 下层平方)
            || !受检乘法(下层平方, 下层N, 下层单元数)) {
            return false;
        }
        for (std::uint64_t j = 0; j < 下层单元数; ++j) {
            const auto x = j % 下层N;
            const auto yz = j / 下层N;
            const auto y = yz % 下层N;
            const auto z = yz / 下层N;
            bool 应有值 = false;
            for (std::uint64_t dz = 0; dz < 2; ++dz) {
                for (std::uint64_t dy = 0; dy < 2; ++dy) {
                    for (std::uint64_t dx = 0; dx < 2; ++dx) {
                        const auto 子索引 = ((2 * z + dz) * 上层N + (2 * y + dy)) * 上层N + 2 * x + dx;
                        应有值 = 应有值 || 读取位(组, 上层起点, 子索引);
                    }
                }
            }
            if (读取位(组, 下层起点, j) != 应有值) {
                return false;
            }
        }
        上层起点 = 下层起点;
        下层起点 += 下层单元数 / 64;
        上层N = 下层N;
    }
    return true;
}

bool 全部为零(
    const std::vector<std::uint64_t>& 组,
    const std::uint64_t 起点,
    const std::uint64_t 数量) noexcept {
    for (std::uint64_t i = 0; i < 数量; ++i) {
        if (组[static_cast<std::size_t>(起点 + i)] != 0) {
            return false;
        }
    }
    return true;
}

} // namespace

bool 局部二值轮廓解析结果_v2::成功(const 局部二值格解析请求_v2& 请求) const noexcept {
    return 版本 == 2
        && 请求.版本 == 2
        && 请求.U64组 != nullptr
        && 状态 == 局部二值格解析状态_v2::已解析
        && 格式.has_value()
        && 格式->N != 0
        && 格式->最高层位图块数 != 0
        && (格式->编码形态 == 局部二值格编码形态_v2::普通完整
            || 格式->编码形态 == 局部二值格编码形态_v2::全零描述);
}

bool 局部二值体素解析结果_v2::成功(const 局部二值格解析请求_v2& 请求) const noexcept {
    return 版本 == 2
        && 请求.版本 == 2
        && 请求.U64组 != nullptr
        && 状态 == 局部二值格解析状态_v2::已解析
        && 格式.has_value()
        && 格式->N != 0
        && 格式->最高层位图块数 != 0
        && (格式->编码形态 == 局部二值格编码形态_v2::普通完整
            || 格式->编码形态 == 局部二值格编码形态_v2::全零描述);
}

局部二值轮廓解析结果_v2 局部二值格解析数据服务::解析局部二值轮廓(
    const 局部二值格解析请求_v2& 请求) const noexcept {
    局部二值轮廓解析结果_v2 结果;
    try {
        if (请求.版本 != 2 || 请求.U64组 == nullptr) {
            return 结果;
        }
        const auto& 组 = *请求.U64组;
        if (!可转换为U64(组.size())) {
            结果.状态 = 局部二值格解析状态_v2::数量超限;
            return 结果;
        }
        const auto 长度 = static_cast<std::uint64_t>(组.size());
        if (长度 == 0) {
            结果.状态 = 局部二值格解析状态_v2::格式不合法;
            return 结果;
        }

        if (长度 == 3 && 组[0] == 全零描述标识) {
            if (组[1] != 局部二值轮廓类型编码) {
                结果.状态 = 局部二值格解析状态_v2::格式不合法;
                return 结果;
            }
            std::uint64_t 最高层块数 = 0;
            std::uint64_t 压缩层数 = 0;
            std::uint64_t 全部块数 = 0;
            const auto 布局 = 计算轮廓布局(组[2], 最高层块数, 压缩层数, 全部块数);
            if (布局 != 布局计算结果::成功) {
                结果.状态 = 布局 == 布局计算结果::数量超限
                    ? 局部二值格解析状态_v2::数量超限
                    : 局部二值格解析状态_v2::格式不合法;
                return 结果;
            }
            结果.格式 = 局部二值轮廓格式_v2{组[2], 最高层块数, 压缩层数, 局部二值格编码形态_v2::全零描述};
            结果.状态 = 局部二值格解析状态_v2::已解析;
            return 结果;
        }

        auto N = std::uint64_t{8};
        for (;;) {
            std::uint64_t 最高层块数 = 0;
            std::uint64_t 压缩层数 = 0;
            std::uint64_t 全部块数 = 0;
            const auto 布局 = 计算轮廓布局(N, 最高层块数, 压缩层数, 全部块数);
            if (布局 == 布局计算结果::数量超限) {
                结果.状态 = 局部二值格解析状态_v2::数量超限;
                return 结果;
            }
            if (全部块数 == 长度) {
                if (!验证轮廓压缩层(组, N, 最高层块数)
                    || 全部为零(组, 0, 长度)) {
                    结果.状态 = 局部二值格解析状态_v2::格式不合法;
                    return 结果;
                }
                结果.格式 = 局部二值轮廓格式_v2{N, 最高层块数, 压缩层数, 局部二值格编码形态_v2::普通完整};
                结果.状态 = 局部二值格解析状态_v2::已解析;
                return 结果;
            }
            if (全部块数 > 长度) {
                结果.状态 = 局部二值格解析状态_v2::格式不合法;
                return 结果;
            }
            if (N > std::numeric_limits<std::uint64_t>::max() / 2) {
                结果.状态 = 局部二值格解析状态_v2::数量超限;
                return 结果;
            }
            N *= 2;
        }
    } catch (...) {
        结果.格式.reset();
        结果.状态 = 局部二值格解析状态_v2::内部不一致;
        return 结果;
    }
}

局部二值体素解析结果_v2 局部二值格解析数据服务::解析局部二值体素(
    const 局部二值格解析请求_v2& 请求) const noexcept {
    局部二值体素解析结果_v2 结果;
    try {
        if (请求.版本 != 2 || 请求.U64组 == nullptr) {
            return 结果;
        }
        const auto& 组 = *请求.U64组;
        if (!可转换为U64(组.size())) {
            结果.状态 = 局部二值格解析状态_v2::数量超限;
            return 结果;
        }
        const auto 长度 = static_cast<std::uint64_t>(组.size());
        if (长度 == 0) {
            结果.状态 = 局部二值格解析状态_v2::格式不合法;
            return 结果;
        }

        if (长度 == 3 && 组[0] == 全零描述标识) {
            if (组[1] != 局部二值体素类型编码) {
                结果.状态 = 局部二值格解析状态_v2::格式不合法;
                return 结果;
            }
            std::uint64_t 最高层块数 = 0;
            std::uint64_t 压缩层数 = 0;
            std::uint64_t 全部块数 = 0;
            const auto 布局 = 计算体素布局(组[2], 最高层块数, 压缩层数, 全部块数);
            if (布局 != 布局计算结果::成功) {
                结果.状态 = 布局 == 布局计算结果::数量超限
                    ? 局部二值格解析状态_v2::数量超限
                    : 局部二值格解析状态_v2::格式不合法;
                return 结果;
            }
            结果.格式 = 局部二值体素格式_v2{组[2], 最高层块数, 压缩层数, 0, 局部二值格编码形态_v2::全零描述};
            结果.状态 = 局部二值格解析状态_v2::已解析;
            return 结果;
        }

        const auto N = 组[0];
        std::uint64_t 最高层块数 = 0;
        std::uint64_t 压缩层数 = 0;
        std::uint64_t 全部块数 = 0;
        const auto 布局 = 计算体素布局(N, 最高层块数, 压缩层数, 全部块数);
        if (布局 != 布局计算结果::成功) {
            结果.状态 = 布局 == 布局计算结果::数量超限
                ? 局部二值格解析状态_v2::数量超限
                : 局部二值格解析状态_v2::格式不合法;
            return 结果;
        }

        std::uint64_t 分辨率项和全部位图块数 = 0;
        if (!受检加法(1, 全部块数, 分辨率项和全部位图块数)) {
            结果.状态 = 局部二值格解析状态_v2::数量超限;
            return 结果;
        }
        if (分辨率项和全部位图块数 > 长度) {
            结果.状态 = 局部二值格解析状态_v2::格式不合法;
            return 结果;
        }
        const auto 颜色项数 = 长度 - 分辨率项和全部位图块数;
        std::uint64_t 占用数 = 0;
        for (std::uint64_t i = 0; i < 最高层块数; ++i) {
            const auto 位数 = static_cast<std::uint64_t>(std::popcount(组[static_cast<std::size_t>(1 + i)]));
            if (!受检加法(占用数, 位数, 占用数)) {
                结果.状态 = 局部二值格解析状态_v2::数量超限;
                return 结果;
            }
        }
        if (颜色项数 != 占用数) {
            结果.状态 = 局部二值格解析状态_v2::格式不合法;
            return 结果;
        }
        const auto 压缩层起点 = 1 + 最高层块数 + 颜色项数;
        if (!可转换为SizeT(压缩层起点)) {
            结果.状态 = 局部二值格解析状态_v2::数量超限;
            return 结果;
        }
        for (std::uint64_t i = 0; i < 颜色项数; ++i) {
            if ((组[static_cast<std::size_t>(1 + 最高层块数 + i)] >> 24) != 0) {
                结果.状态 = 局部二值格解析状态_v2::格式不合法;
                return 结果;
            }
        }
        if (!验证体素压缩层(组, N, 压缩层起点)
            || (占用数 == 0 && 颜色项数 == 0)) {
            结果.状态 = 局部二值格解析状态_v2::格式不合法;
            return 结果;
        }
        结果.格式 = 局部二值体素格式_v2{N, 最高层块数, 压缩层数, 颜色项数, 局部二值格编码形态_v2::普通完整};
        结果.状态 = 局部二值格解析状态_v2::已解析;
        return 结果;
    } catch (...) {
        结果.格式.reset();
        结果.状态 = 局部二值格解析状态_v2::内部不一致;
        return 结果;
    }
}

} // namespace 海中鱼巣
