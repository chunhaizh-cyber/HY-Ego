#pragma once
#include "方法问题反馈.数据.h"

// 模拟世界 owner 的内部格式编解码，不提供业务写入口。
namespace 海中鱼巣::方法问题归档内部 {
enum class 格式 : std::uint8_t {
    旧方法副本 = 1, 任务执行组织 = 2, 有序参数任务执行组织 = 3,
    任务成员有序参数任务执行组织 = 4,
    场景事实有序参数任务执行组织 = 5,
    正式分支有序参数任务执行组织 = 6,
    场景结果有序参数任务执行组织 = 7,
    需求结果有序参数任务执行组织 = 8
};
struct 编码记录 final {
    std::int64_t 类型 = 0;
    std::vector<std::uint64_t> 载荷;
};
std::vector<std::uint64_t> 编码问题头(const 新方法问题反馈材料&, 格式);
新方法问题反馈材料 解码问题头(const std::vector<std::uint64_t>&, 格式);
std::vector<编码记录> 编码场景快照(const 新执行场景快照&, 格式);
新执行场景快照 解码场景快照(const std::vector<编码记录>&, 格式);
bool 采用身份有效(const 新方法问题反馈材料&) noexcept;
bool 含有序参数(const 新方法问题反馈材料&) noexcept;
bool 含任务成员路径(const 新方法问题反馈材料&) noexcept;
// 格式4还保留C/R参数关系，旧1/2/3的字节布局维持原样。
bool 需要格式4(const 新方法问题反馈材料&) noexcept;
bool 需要格式5(const 新方法问题反馈材料&) noexcept;
bool 需要格式6(const 新方法问题反馈材料&) noexcept;
bool 需要格式7(const 新方法问题反馈材料&) noexcept;
bool 需要格式8(const 新方法问题反馈材料&) noexcept;
} // namespace 海中鱼巣::方法问题归档内部
