module;

#include <cstdint>
#include <array>
#include <algorithm>
#include <optional>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#define L1_PUBLIC_FACT_NO_INCLUDES
#define L1_FACT_BASE_NO_INCLUDES

export module 海中鱼巣.核心.合同.L1事实基座;
export import 海中鱼巣.核心.合同.L1公共事实;

export {
#include "L1事实基座.数据.h"
}

export namespace 海中鱼巣 {

inline constexpr std::uint32_t L1事实基座合同版本 = 2;


struct L1事实读取请求 final {
    std::uint32_t 合同版本 = L1事实基座合同版本;
    稳定编码 编码;
};
struct L1属性读取请求 final {
    std::uint32_t 合同版本 = L1事实基座合同版本;
    稳定编码 节点;
    稳定编码 属性类型;
};
struct L1完整快照读取请求 final {
    std::uint32_t 合同版本 = L1事实基座合同版本;
};
struct L1事实代次读取请求 final {
    std::uint32_t 合同版本 = L1事实基座合同版本;
    // 诊断责任：无适用错误分支；默认比较只比较纯值请求。
    friend bool operator==(const L1事实代次读取请求&,
        const L1事实代次读取请求&) = default;
};
} // namespace 海中鱼巣
