#pragma once

#ifndef L2_ORDINARY_METHOD_STRUCTURE_NO_INCLUDES
#include <cstdint>
#include "L2结构公共.数据.h"
#endif

namespace 海中鱼巣 {

inline constexpr std::uint32_t L2普通方法结构合同版本 = 1;

#define 定义L2普通方法节点身份(类型名) \
    struct 类型名 final { \
        稳定编码 值; \
        explicit 类型名(稳定编码 编码 = {}) noexcept : 值(编码) {} \
        friend bool operator==(const 类型名&, const 类型名&) = default; \
    }

定义L2普通方法节点身份(L2方法治理虚拟存在身份);
定义L2普通方法节点身份(L2方法条件身份);
定义L2普通方法节点身份(L2方法结果身份);
定义L2普通方法节点身份(L2方法动作入口身份);

#undef 定义L2普通方法节点身份

struct L2方法内容版本 final {
    std::uint64_t 值 = 0;
    friend bool operator==(const L2方法内容版本&,
        const L2方法内容版本&) = default;
};

struct L2方法规格版本 final {
    std::uint64_t 值 = 0;
    friend bool operator==(const L2方法规格版本&,
        const L2方法规格版本&) = default;
};

// 诊断责任：无适用错误分支；只判断治理虚拟存在身份的非零稳定编码。
inline bool L2方法治理虚拟存在身份有效(
    L2方法治理虚拟存在身份 身份) noexcept {
    return 有效(身份.值);
}

// 诊断责任：无适用错误分支；只判断方法条件身份的非零稳定编码。
inline bool L2方法条件身份有效(L2方法条件身份 身份) noexcept {
    return 有效(身份.值);
}

// 诊断责任：无适用错误分支；只判断方法结果身份的非零稳定编码。
inline bool L2方法结果身份有效(L2方法结果身份 身份) noexcept {
    return 有效(身份.值);
}

// 诊断责任：无适用错误分支；只判断方法动作入口身份的非零稳定编码。
inline bool L2方法动作入口身份有效(L2方法动作入口身份 身份) noexcept {
    return 有效(身份.值);
}

// 诊断责任：无适用错误分支；只判断独立方法内容版本非零。
inline bool L2方法内容版本有效(L2方法内容版本 版本) noexcept {
    return 版本.值 != 0;
}

// 诊断责任：无适用错误分支；只判断独立方法规格版本非零。
inline bool L2方法规格版本有效(L2方法规格版本 版本) noexcept {
    return 版本.值 != 0;
}

} // namespace 海中鱼巣
