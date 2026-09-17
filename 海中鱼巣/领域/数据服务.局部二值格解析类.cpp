#include "数据服务.局部二值格解析类.h"

#include <bit>
#include <limits>

namespace 海中鱼巣 {
namespace {

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

constexpr std::uint64_t 整数平方根(const std::uint64_t 值) noexcept {
    std::uint64_t 低 = 0;
    std::uint64_t 高 = 0xFFFFFFFFULL;
    std::uint64_t 最佳 = 0;
    while (低 <= 高) {
        const std::uint64_t 中 = 低 + (高 - 低) / 2;
        if (中 == 0 || 中 <= 值 / 中) {
            最佳 = 中;
            if (中 == 高) {
                break;
            }
            低 = 中 + 1;
        } else {
            高 = 中 - 1;
        }
    }
    return 最佳;
}

bool 为精确平方(const std::uint64_t 值, const std::uint64_t 根) noexcept {
    return 根 != 0 && 根 == 值 / 根 && 值 % 根 == 0;
}

} // namespace

bool 局部二值轮廓解析结果_v1::成功(const 局部二值格解析请求_v1& 请求) const noexcept {
    return 版本 == 1
        && 请求.版本 == 1
        && 请求.U64组 != nullptr
        && 状态 == 局部二值格解析状态_v1::已解析
        && 格式.has_value()
        && 格式->N != 0
        && 格式->位图块数 != 0;
}

bool 局部二值体素解析结果_v1::成功(const 局部二值格解析请求_v1& 请求) const noexcept {
    return 版本 == 1
        && 请求.版本 == 1
        && 请求.U64组 != nullptr
        && 状态 == 局部二值格解析状态_v1::已解析
        && 格式.has_value()
        && 格式->N != 0
        && 格式->占用位图块数 != 0;
}

局部二值轮廓解析结果_v1 局部二值格解析数据服务::解析局部二值轮廓(
    const 局部二值格解析请求_v1& 请求) const noexcept {
    局部二值轮廓解析结果_v1 结果;
    try {
        if (请求.版本 != 1 || 请求.U64组 == nullptr) {
            return 结果;
        }
        if (!可转换为U64(请求.U64组->size())) {
            结果.状态 = 局部二值格解析状态_v1::数量超限;
            return 结果;
        }

        const auto 长度 = static_cast<std::uint64_t>(请求.U64组->size());
        if (长度 == 0) {
            结果.状态 = 局部二值格解析状态_v1::格式不合法;
            return 结果;
        }
        const auto 根 = 整数平方根(长度);
        if (!为精确平方(长度, 根)) {
            结果.状态 = 局部二值格解析状态_v1::格式不合法;
            return 结果;
        }

        std::uint64_t N = 0;
        if (!受检乘法(8, 根, N)) {
            结果.状态 = 局部二值格解析状态_v1::数量超限;
            return 结果;
        }
        结果.格式 = 局部二值轮廓格式_v1{N, 长度};
        结果.状态 = 局部二值格解析状态_v1::已解析;
        return 结果;
    } catch (...) {
        结果.格式.reset();
        结果.状态 = 局部二值格解析状态_v1::内部不一致;
        return 结果;
    }
}

局部二值体素解析结果_v1 局部二值格解析数据服务::解析局部二值体素(
    const 局部二值格解析请求_v1& 请求) const noexcept {
    局部二值体素解析结果_v1 结果;
    try {
        if (请求.版本 != 1 || 请求.U64组 == nullptr) {
            return 结果;
        }
        if (!可转换为U64(请求.U64组->size())) {
            结果.状态 = 局部二值格解析状态_v1::数量超限;
            return 结果;
        }

        const auto 长度 = static_cast<std::uint64_t>(请求.U64组->size());
        if (长度 == 0) {
            结果.状态 = 局部二值格解析状态_v1::格式不合法;
            return 结果;
        }
        const auto N = (*请求.U64组)[0];
        if (N == 0 || N % 4 != 0) {
            结果.状态 = 局部二值格解析状态_v1::格式不合法;
            return 结果;
        }

        std::uint64_t N平方 = 0;
        std::uint64_t N立方 = 0;
        if (!受检乘法(N, N, N平方) || !受检乘法(N平方, N, N立方)) {
            结果.状态 = 局部二值格解析状态_v1::数量超限;
            return 结果;
        }
        const auto 块数 = N立方 / 64;
        std::uint64_t 颜色段起点 = 0;
        if (!受检加法(1, 块数, 颜色段起点) || !可转换为SizeT(颜色段起点)) {
            结果.状态 = 局部二值格解析状态_v1::数量超限;
            return 结果;
        }
        if (颜色段起点 > 长度) {
            结果.状态 = 局部二值格解析状态_v1::格式不合法;
            return 结果;
        }

        std::uint64_t 占用数 = 0;
        for (std::uint64_t 块索引 = 0; 块索引 < 块数; ++块索引) {
            const auto 位数 = static_cast<std::uint64_t>(std::popcount((*请求.U64组)[static_cast<std::size_t>(1 + 块索引)]));
            if (!受检加法(占用数, 位数, 占用数)) {
                结果.状态 = 局部二值格解析状态_v1::数量超限;
                return 结果;
            }
        }

        std::uint64_t 期待长度 = 0;
        if (!受检加法(颜色段起点, 占用数, 期待长度)) {
            结果.状态 = 局部二值格解析状态_v1::数量超限;
            return 结果;
        }
        if (长度 != 期待长度) {
            结果.状态 = 局部二值格解析状态_v1::格式不合法;
            return 结果;
        }
        for (std::uint64_t 颜色索引 = 0; 颜色索引 < 占用数; ++颜色索引) {
            const auto 值 = (*请求.U64组)[static_cast<std::size_t>(颜色段起点 + 颜色索引)];
            if ((值 >> 24) != 0) {
                结果.状态 = 局部二值格解析状态_v1::格式不合法;
                return 结果;
            }
        }

        结果.格式 = 局部二值体素格式_v1{N, 块数, 占用数};
        结果.状态 = 局部二值格解析状态_v1::已解析;
        return 结果;
    } catch (...) {
        结果.格式.reset();
        结果.状态 = 局部二值格解析状态_v1::内部不一致;
        return 结果;
    }
}

} // namespace 海中鱼巣
