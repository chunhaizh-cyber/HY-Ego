// 文件规则：参与者只保存本次结构会话的值式记录，不暴露仓库、索引、令牌、锁或可写引用。
module;

#include "状态动态类型化记录.数据.h"

#include <optional>

export module 海中鱼巣.领域.参与者.状态动态类型化记录;

export namespace 海中鱼巣 {

class 状态动态类型化记录事务参与者 final {
public:
    bool 登记状态记录(const 状态域记录& 记录) noexcept {
        if (状态记录_.has_value() || !记录.值域有效()) return false;
        状态记录_ = 记录;
        return true;
    }

    bool 登记动态记录(const 动态域记录& 记录) noexcept {
        if (动态记录_.has_value() || !记录.值域有效()) return false;
        动态记录_ = 记录;
        return true;
    }

    bool 已登记状态记录() const noexcept { return 状态记录_.has_value(); }
    bool 已登记动态记录() const noexcept { return 动态记录_.has_value(); }

    bool 准备提交() const noexcept {
        return (!状态记录_ || 状态记录_->值域有效())
            && (!动态记录_ || 动态记录_->值域有效())
            && (状态记录_.has_value() || 动态记录_.has_value());
    }

    void 完成撤销() noexcept {
        状态记录_.reset();
        动态记录_.reset();
    }

    std::optional<状态域记录> 读取状态记录() const { return 状态记录_; }
    std::optional<动态域记录> 读取动态记录() const { return 动态记录_; }

private:
    std::optional<状态域记录> 状态记录_;
    std::optional<动态域记录> 动态记录_;
};

class 状态动态类型化记录只读访问器 final {
public:
    explicit 状态动态类型化记录只读访问器(
        const 状态动态类型化记录事务参与者& 参与者) noexcept
        : 参与者_(参与者) {}

    std::optional<状态域记录> 读取状态记录() const {
        return 参与者_.读取状态记录();
    }

    std::optional<动态域记录> 读取动态记录() const {
        return 参与者_.读取动态记录();
    }

private:
    const 状态动态类型化记录事务参与者& 参与者_;
};

}
