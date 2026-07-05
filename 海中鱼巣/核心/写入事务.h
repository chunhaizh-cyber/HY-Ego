// 文件规则：写入事务只表达一组结构写入边界；真实回滚和审计在后续阶段扩展。
#pragma once

#include <cstdint>
#include <optional>
#include <string>

namespace 海中鱼巣 {

enum class 事务状态 : std::uint32_t {
    未开始 = 0,
    进行中 = 1,
    已提交 = 2,
    已取消 = 3,
    失败 = 4
};

class 写入事务 {
public:
    bool 开始();
    bool 提交();
    bool 取消();
    void 放弃();
    bool 标记失败(std::wstring 失败阶段, std::wstring 失败原因);
    事务状态 当前状态() const;
    std::optional<std::wstring> 读取失败阶段() const;
    std::optional<std::wstring> 读取失败原因() const;

private:
    事务状态 状态_ = 事务状态::未开始;
    std::optional<std::wstring> 失败阶段_;
    std::optional<std::wstring> 失败原因_;
};

}
