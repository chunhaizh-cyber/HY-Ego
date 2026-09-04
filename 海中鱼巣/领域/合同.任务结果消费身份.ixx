module;

#include <cstdint>

export module 海中鱼巣.领域.合同.任务结果消费身份;

import 海中鱼巣.核心.合同.L1公共事实;

export namespace 海中鱼巣 {

#define 定义任务结果消费强类型身份(类型名) \
    struct 类型名 final { \
        稳定编码 值; \
        explicit 类型名(稳定编码 编码 = {}) noexcept : 值(编码) {} \
        bool 有效() const noexcept { return ::海中鱼巣::有效(值); } \
        friend bool operator==(const 类型名&, const 类型名&) = default; \
    }

定义任务结果消费强类型身份(L2任务结果消费分配记录身份);
定义任务结果消费强类型身份(L2需求本轮核算身份);
定义任务结果消费强类型身份(L2需求满足记录身份);
定义任务结果消费强类型身份(L2自我现实执行授权身份);
定义任务结果消费强类型身份(L2自我现实执行授权收口身份);

#undef 定义任务结果消费强类型身份

}
