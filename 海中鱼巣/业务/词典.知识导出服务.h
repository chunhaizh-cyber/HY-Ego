#pragma once

#include "../领域/新_方法公共类型.h"
#include "../领域/本能词典.数据.h"

namespace 海中鱼巣 {
class 概念_存在类;
class 概念_特征类;
class 新_特征值类;
struct 新本能函数本次条件;

enum class 新词典知识导出状态 : std::uint8_t {
    已形成 = 1, 正式来源已退出
};
struct 新词典知识导出结果 final {
    新词典知识导出状态 状态;
    std::optional<新方法条件结果定义> 定义;
};

// 只抽象已经采用的本次材料；不查询词典、不重读当前F、不发表方法配对。
新词典知识导出结果 导出词典条件结果(
    const 新方法条件定义& 基础条件,
    const 新本能函数本次条件& 动作前条件,
    const 新词典本能输入& 实际输入,
    稳定编码 正式来源词条,
    const 新本能词典读取回执& 已采用终态,
    概念_存在类&, 概念_特征类&, const 新_特征值类&);
} // namespace 海中鱼巣
