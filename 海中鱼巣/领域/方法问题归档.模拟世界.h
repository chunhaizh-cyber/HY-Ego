#pragma once
#include "方法问题反馈.数据.h"

// 模拟世界 owner 的内部格式编解码，不提供业务写入口。
namespace 海中鱼巣::方法问题归档内部 {
struct 编码记录 final {
    std::int64_t 类型 = 0;
    std::vector<std::uint64_t> 载荷;
};
std::vector<std::uint64_t> 编码问题头(const 新方法问题反馈材料&);
新方法问题反馈材料 解码问题头(const std::vector<std::uint64_t>&);
std::vector<编码记录> 编码场景快照(const 新执行场景快照&);
新执行场景快照 解码场景快照(const std::vector<编码记录>&);
} // namespace 海中鱼巣::方法问题归档内部
