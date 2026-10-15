#pragma once
#include "方法问题反馈.数据.h"

// 模拟世界 owner 的内部格式编解码，不提供业务写入口。
namespace 海中鱼巣::方法问题归档内部 {
enum class 格式 : std::uint8_t { 旧方法副本 = 1, 任务执行组织 = 2 };
struct 编码记录 final {
    std::int64_t 类型 = 0;
    std::vector<std::uint64_t> 载荷;
};
std::vector<std::uint64_t> 编码问题头(const 新方法问题反馈材料&, 格式);
新方法问题反馈材料 解码问题头(const std::vector<std::uint64_t>&, 格式);
std::vector<编码记录> 编码场景快照(const 新执行场景快照&, 格式);
新执行场景快照 解码场景快照(const std::vector<编码记录>&, 格式);
bool 采用身份有效(const 新方法问题反馈材料&) noexcept;
} // namespace 海中鱼巣::方法问题归档内部
