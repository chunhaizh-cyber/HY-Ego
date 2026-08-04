#pragma once

#ifndef L1_PUBLIC_FACT_NO_INCLUDES
#include <cstdint>
#endif

namespace 海中鱼巣 {

struct 稳定编码 final {
    std::uint64_t 值 = 0;
    friend bool operator==(const 稳定编码&, const 稳定编码&) = default;
    friend bool operator<(const 稳定编码& 左, const 稳定编码& 右) noexcept { return 左.值 < 右.值; }
};

enum class 节点种类 : std::uint8_t { 普通 = 1, 属性类型 = 2 };

inline bool 有效(稳定编码 编码) noexcept { return 编码.值 != 0; }

} // namespace 海中鱼巣
