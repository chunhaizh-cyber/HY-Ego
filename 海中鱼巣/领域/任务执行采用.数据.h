#pragma once
#include "新_方法公共类型.h"
#include "新_方法参数场景.h"

namespace 海中鱼巣 {
struct 新任务本能采用定位 final {
    稳定编码 组织节点, 方法作用域节点, 配对要求节点;
    friend bool operator==(const 新任务本能采用定位&, const 新任务本能采用定位&) = default;
};
struct 新任务本能采用材料 final {
    新任务本能采用定位 定位;
    稳定编码 来源方法节点, 来源配对节点, 来源条件节点, 来源结果节点;
    std::vector<新方法参数规格> 参数要求组;
    新方法条件定义 原始条件, 需要条件;
    新方法结果定义 结果;
    std::vector<新方法条件结果线性关系> 线性关系组;
    std::vector<新方法参数角色绑定> 参数绑定组;
    friend bool operator==(const 新任务本能采用材料&, const 新任务本能采用材料&) = default;
};
}
