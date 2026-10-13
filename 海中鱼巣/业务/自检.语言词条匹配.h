#pragma once

#include "../领域/新_语言类.h"
#include "../领域/新_词类.h"

namespace 海中鱼巣 {

// 只读使用调用者已登记的样本，不初始化或建词。
// 0=全部通过，1=断言/读取失败，3=样本不足 NOT_RUN；运行期间材料须保持稳定。
int 运行语言词条匹配自检(
    const 新_语言类& 语言服务,
    const 新_词类& 词服务,
    稳定编码 语言节点,
    const std::string& 短词面,
    const std::string& 长词面,
    const std::string& 未登记文本) noexcept;

} // namespace 海中鱼巣
