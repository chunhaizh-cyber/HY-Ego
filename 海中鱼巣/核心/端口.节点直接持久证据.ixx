// 文件规则：本模块只定义节点直接结构事务持久证据的纯值端口，不实现 SQL 或运行期裁决。
module;

#include "节点直接结构合同.数据.h"

#include <array>
#include <cstdint>
#include <optional>
#include <vector>

export module 海中鱼巣.核心.端口.节点直接持久证据;

export namespace 海中鱼巣 {

enum class 节点直接持久端口状态 : std::uint8_t {
    已准备 = 1, 已见证 = 2, 精确同义 = 3, 未找到 = 4,
    冲突 = 5, 资源失败 = 6, 结果未知 = 7, 损坏 = 8, 已撤销未发布 = 9
};
struct 节点直接持久准备请求 {
    节点直接事务幂等身份 安装实例身份;
    节点直接事务幂等身份 事务身份;
    std::uint32_t 材料格式版本 = 1;
    std::array<std::uint8_t, 32> 请求意图摘要{};
    std::array<std::uint8_t, 32> 执行证据摘要{};
    std::vector<std::uint8_t> 写集材料;
};
struct 节点直接持久发布见证请求 {
    节点直接事务幂等身份 安装实例身份;
    节点直接事务幂等身份 事务身份;
    std::uint64_t 尝试序号 = 0;
    std::array<std::uint8_t, 32> 请求意图摘要{};
    std::array<std::uint8_t, 32> 执行证据摘要{};
    std::uint64_t 发布代次 = 0;
    std::array<std::uint8_t, 32> 结果摘要{};
};
struct 节点直接持久撤销见证请求 {
    节点直接事务幂等身份 安装实例身份;
    节点直接事务幂等身份 事务身份;
    std::uint64_t 尝试序号 = 0;
    std::array<std::uint8_t, 32> 请求意图摘要{};
    std::array<std::uint8_t, 32> 执行证据摘要{};
    std::array<std::uint8_t, 32> 写集材料摘要{};
};
struct 节点直接持久端口结果 {
    节点直接持久端口状态 状态 = 节点直接持久端口状态::资源失败;
    std::uint64_t 尝试序号 = 0;
};

class 节点直接持久证据写入端口 {
public:
    virtual ~节点直接持久证据写入端口() = default;
    virtual 节点直接持久端口结果 准备(const 节点直接持久准备请求& 请求) = 0;
    virtual 节点直接持久端口结果 标记已发布(const 节点直接持久发布见证请求& 请求) = 0;
    virtual 节点直接持久端口结果 标记已撤销未发布(const 节点直接持久撤销见证请求& 请求) = 0;
};

enum class 节点直接安装实例证据状态 : std::uint8_t {
    确认从未发布 = 1, 存在事务证据 = 2, 未登记 = 3, 资源失败 = 4, 损坏 = 5
};
struct 节点直接持久事务证据 {
    节点直接事务幂等身份 事务身份;
    std::uint64_t 尝试序号 = 0;
    std::uint32_t 材料格式版本 = 0;
    std::array<std::uint8_t, 32> 请求意图摘要{};
    std::array<std::uint8_t, 32> 执行证据摘要{};
    std::vector<std::uint8_t> 写集材料;
    节点直接持久端口状态 状态 = 节点直接持久端口状态::未找到;
    std::optional<std::uint64_t> 发布代次;
    std::optional<std::array<std::uint8_t, 32>> 结果摘要;
};
struct 节点直接恢复材料读取结果 {
    节点直接安装实例证据状态 安装状态 = 节点直接安装实例证据状态::未登记;
    std::vector<节点直接持久事务证据> 事务组;
};
class 节点直接恢复材料读取端口 {
public:
    virtual ~节点直接恢复材料读取端口() = default;
    virtual 节点直接恢复材料读取结果 读取安装实例(
        节点直接事务幂等身份 安装实例身份) = 0;
};

} // namespace 海中鱼巣
