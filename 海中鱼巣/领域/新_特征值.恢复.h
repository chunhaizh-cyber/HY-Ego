#pragma once

#include "新_特征值类.h"

#include <array>

namespace 海中鱼巣 {

class 基础持久视图;

struct 新特征值恢复结构读回 final {
    新特征值保护读取状态 状态 = 新特征值保护读取状态::未初始化;
    std::array<稳定编码, 6> 键{};

    新特征值快照解码结构 解码结构() const noexcept {
        return {键[0], 键[1], 键[3], 键[4]};
    }
};

// 仅从已隔离的不可变候选读取，不启动 owner 或重绑进程当前结构。
新特征值恢复结构读回 读取恢复特征值结构(const 基础持久视图& 视图) noexcept;
新特征值保护读取结果 读取恢复特征值材料(const 基础持久视图& 视图,
    const 新特征值恢复结构读回& 结构, 材料物理类型 类型, const 新特征值材料& 材料) noexcept;

} // namespace 海中鱼巣
