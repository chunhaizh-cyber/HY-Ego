// 文件规则：写入事务只表达一组结构写入边界；真实回滚和审计在后续阶段扩展。
#pragma once

#include <cstdint>

namespace 海中鱼巣 {

enum class 事务状态 : std::uint32_t {
    未开始 = 0,
    已开始 = 1,
    已提交 = 2,
    已放弃 = 3
};

class 写入事务 {
public:
    void 开始();
    bool 提交();
    void 放弃();
    事务状态 当前状态() const;

private:
    事务状态 状态_ = 事务状态::未开始;
};

}
