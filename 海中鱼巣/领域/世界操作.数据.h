#pragma once

#include <cstdint>

namespace 海中鱼巣 {

struct 世界操作幂等身份 final {
    std::uint64_t 值 = 0;
    friend bool operator==(const 世界操作幂等身份&, const 世界操作幂等身份&) = default;
};

inline bool 世界操作幂等身份有效(世界操作幂等身份 身份) noexcept {
    return 身份.值 != 0
        && 身份.值 <= 0x00FF'FFFF'FFFF'FFFFULL;
}

} // namespace 海中鱼巣
