#pragma once
#include "关系概念.数据.h"
#include <mutex>

namespace 海中鱼巣 {

class 新_语言类;
class 新_词类;

// 世界树关系概念登记：挂在自然语言组织根下（与概念名称关系根并列），非第五世界根。
class 关系概念类 final {
public:
    关系概念类() = default;

    bool 初始化() noexcept;
    bool 已装配() const noexcept;

    关系概念建立结果 建立或取得关系概念(const 关系概念定义&) noexcept;
    关系概念读取结果 按种子键读取(const std::string& 稳定种子键) const noexcept;
    关系概念读取结果 按节点读取(稳定编码 节点) const noexcept;
    关系概念列表结果 列出全部() const noexcept;

    // 只追加表达知识节点；不得由初始化路径调用去覆盖已有后天关联。
    关系概念操作状态 追加表达知识(
        稳定编码 关系概念节点, 稳定编码 语言节点, 稳定编码 知识节点) noexcept;

private:
    mutable std::recursive_mutex 互斥_;
    bool 已初始化_ = false;
};

} // namespace 海中鱼巣
