#include "信息参数.读取服务.h"
#include "../领域/新_需求类.h"
#include "../领域/新_语言类.h"

#include <algorithm>
#include <thread>

namespace 海中鱼巣 {
正式信息参数读取服务::正式信息参数读取服务(const 新_需求类& 需求, const 新_语言类& 语言,
    std::array<稳定编码,3> 需求范围, 新词典归类信息概念组 归类范围) noexcept
    : 需求_(需求),语言_(语言),需求范围_(需求范围),归类范围_(归类范围) {}

正式信息参数读取结果 正式信息参数读取服务::读取正式信息参数保护(
    稳定编码 信息, 稳定编码 范围) const noexcept {
    const bool 是需求 = std::find(需求范围_.begin(),需求范围_.end(),范围) != 需求范围_.end();
    if (!是需求 && 范围 != 归类范围_.材料范围) return {};
    for (;;) {
        auto 结果 = 是需求 ? 需求_.读取正式信息参数保护(信息,范围)
            : 语言_.读取词典归类参数保护(信息,范围,归类范围_);
        if (结果.状态 != 正式信息参数读取状态::前提已变化) return 结果;
        std::this_thread::yield();
    }
}
} // namespace 海中鱼巣
