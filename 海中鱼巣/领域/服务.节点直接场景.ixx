// 文件规则：本模块只建立场景归属写入的最终公开合同和安全未实现骨架。
module;

#include "世界树事实.数据.h"

#include <cstdint>
#include <optional>
#include <utility>
#include <variant>
#include <vector>

export module 海中鱼巣.领域.服务.节点直接场景;

import 海中鱼巣.核心.服务.节点直接类型化结构提交;
import 海中鱼巣.领域.服务.节点直接存在;
import 海中鱼巣.领域.服务.节点直接世界结构;

export namespace 海中鱼巣 {

struct 节点直接场景创建并接纳请求 {
    世界树请求头 头;
    世界树节点身份见证 场景;
    节点直接实际存在写入规格 存在规格;
};
enum class 世界树成员种类 : std::uint8_t { 存在 = 1, 子场景 = 2 };
struct 移动世界树成员请求 {
    世界树请求头 头;
    世界树幂等身份 幂等身份;
    世界树成员种类 种类 = 世界树成员种类::存在;
    世界树节点身份见证 成员;
    世界树节点身份见证 预期旧父场景;
    世界树关系身份见证 预期旧关系;
    世界树节点身份见证 新父场景;
};
struct 移除世界树存在请求 {
    世界树请求头 头;
    世界树幂等身份 幂等身份;
    世界树节点身份见证 存在;
    世界树节点身份见证 预期场景;
    世界树关系身份见证 预期归属关系;
};
enum class 节点直接场景写入状态 : std::uint8_t {
    已提交 = 1, 幂等读回 = 2, 入口拒绝 = 3, 幂等冲突 = 4,
    版本漂移 = 5, 许可拒绝 = 6, 资源失败 = 7, 内部不一致 = 8, 未实现 = 9
};
using 节点直接场景写后事实 = std::variant<
    世界树创建存在读回, 世界树移动读回, 世界树移除读回>;
struct 节点直接场景写入结果 {
    节点直接场景写入状态 状态 = 节点直接场景写入状态::入口拒绝;
    std::uint64_t 发布代次 = 0;
    持久证据状态 持久状态 = 持久证据状态::不适用;
    std::optional<节点直接场景写后事实> 读回;
    std::vector<世界树缺项> 缺项组;
};

class 节点直接场景业务服务 final {
public:
    节点直接场景业务服务(
        节点直接世界结构读取服务& 世界结构,
        节点直接类型化结构提交服务& 提交) noexcept
        : 世界结构_(世界结构), 提交_(提交) {}

    节点直接场景写入结果 在已有场景中创建并接纳实际存在(
        const 节点直接场景创建并接纳请求&) const noexcept {
        return {节点直接场景写入状态::未实现, 0,
            持久证据状态::不适用, std::nullopt, {}};
    }
    节点直接场景写入结果 移动世界树成员(
        const 移动世界树成员请求&) const noexcept {
        return {节点直接场景写入状态::未实现, 0,
            持久证据状态::不适用, std::nullopt, {}};
    }
    节点直接场景写入结果 移除世界树存在(
        const 移除世界树存在请求&) const noexcept {
        return {节点直接场景写入状态::未实现, 0,
            持久证据状态::不适用, std::nullopt, {}};
    }

private:
    节点直接世界结构读取服务& 世界结构_;
    节点直接类型化结构提交服务& 提交_;
};

static_assert(noexcept(std::declval<const 节点直接场景业务服务&>()
    .在已有场景中创建并接纳实际存在(
        std::declval<const 节点直接场景创建并接纳请求&>())));
static_assert(noexcept(std::declval<const 节点直接场景业务服务&>()
    .移动世界树成员(std::declval<const 移动世界树成员请求&>())));
static_assert(noexcept(std::declval<const 节点直接场景业务服务&>()
    .移除世界树存在(std::declval<const 移除世界树存在请求&>())));

} // namespace 海中鱼巣
