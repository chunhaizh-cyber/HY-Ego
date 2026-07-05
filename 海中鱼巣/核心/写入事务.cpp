// 文件规则：事务状态是结构写入边界状态，不代表需求、任务或方法结果。
#include "写入事务.h"

#include <utility>

namespace 海中鱼巣 {

bool 写入事务::开始() {
    if (状态_ == 事务状态::未开始) {
        状态_ = 事务状态::进行中;
        return true;
    }
    return false;
}

bool 写入事务::提交() {
    if (状态_ != 事务状态::进行中) {
        return false;
    }
    状态_ = 事务状态::已提交;
    return true;
}

bool 写入事务::取消() {
    if (状态_ == 事务状态::进行中) {
        状态_ = 事务状态::已取消;
        return true;
    }
    return false;
}

void 写入事务::放弃() {
    取消();
}

bool 写入事务::标记失败(std::wstring 失败阶段, std::wstring 失败原因) {
    if (状态_ != 事务状态::进行中) {
        return false;
    }
    状态_ = 事务状态::失败;
    失败阶段_ = std::move(失败阶段);
    失败原因_ = std::move(失败原因);
    return true;
}

事务状态 写入事务::当前状态() const {
    return 状态_;
}

std::optional<std::wstring> 写入事务::读取失败阶段() const {
    return 失败阶段_;
}

std::optional<std::wstring> 写入事务::读取失败原因() const {
    return 失败原因_;
}

}
