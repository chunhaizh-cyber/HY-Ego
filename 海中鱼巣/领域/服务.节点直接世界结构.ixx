// 文件规则：本模块只建立世界结构读取的最终公开合同和安全未实现骨架。
module;

#include "世界树事实.数据.h"

#include <cstdint>
#include <utility>
#include <vector>

export module 海中鱼巣.领域.服务.节点直接世界结构;

import 海中鱼巣.核心.服务.节点直接结构;

export namespace 海中鱼巣 {

inline constexpr std::uint32_t 节点直接世界结构读取合同版本 = 1;

enum class 节点直接世界结构读取状态 : std::uint8_t {
    已读取 = 1, 未找到 = 2, 入口拒绝 = 3, 版本漂移 = 4,
    许可拒绝 = 5, 资源失败 = 6, 内部不一致 = 7, 未实现 = 8
};

struct 节点直接世界结构读取请求 {
    std::uint32_t 合同版本 = 节点直接世界结构读取合同版本;
    世界树请求头 头;
};

struct 世界结构读取结果 {
    节点直接世界结构读取状态 状态 = 节点直接世界结构读取状态::入口拒绝;
    std::uint64_t 事实截止代次 = 0;
    世界树节点身份见证 世界根;
    std::vector<世界树场景事实> 场景组;
    std::vector<世界树存在事实> 存在组;
    std::vector<世界树缺项> 缺项组;
};

struct 节点直接世界事实代次读取结果 {
    节点直接世界结构读取状态 状态 = 节点直接世界结构读取状态::入口拒绝;
    std::uint64_t 事实截止代次 = 0;
};

class 节点直接世界结构读取服务 final {
public:
    explicit 节点直接世界结构读取服务(
        节点直接结构查询服务& 结构) noexcept : 结构_(结构) {}

    世界结构读取结果 读取世界结构(
        const 节点直接世界结构读取请求&) const noexcept {
        世界结构读取结果 结果;
        结果.状态 = 节点直接世界结构读取状态::未实现;
        return 结果;
    }

    节点直接世界事实代次读取结果 读取当前事实代次() const noexcept {
        return {节点直接世界结构读取状态::未实现, 0};
    }

private:
    节点直接结构查询服务& 结构_;
};

static_assert(noexcept(std::declval<const 节点直接世界结构读取服务&>()
    .读取世界结构(std::declval<const 节点直接世界结构读取请求&>())));
static_assert(noexcept(std::declval<const 节点直接世界结构读取服务&>()
    .读取当前事实代次()));

} // namespace 海中鱼巣
