// 文件规则：本模块只建立实际存在规格的最终公开合同和安全未实现骨架。
module;

#include "世界树事实.数据.h"

#include <cstdint>
#include <optional>
#include <utility>

export module 海中鱼巣.领域.服务.节点直接存在;

export namespace 海中鱼巣 {

enum class 节点直接存在规格状态 : std::uint8_t {
    已形成 = 1, 入口拒绝 = 2, 版本漂移 = 3, 内部不一致 = 4, 未实现 = 5
};
struct 形成节点直接实际存在规格请求 {
    世界树幂等身份 幂等身份;
    std::uint32_t 存在资格规则版本 = 1;
};
struct 节点直接实际存在写入规格 {
    世界树幂等身份 幂等身份;
    std::uint32_t 存在资格规则版本 = 1;
};
struct 节点直接实际存在规格结果 {
    节点直接存在规格状态 状态 = 节点直接存在规格状态::入口拒绝;
    std::optional<节点直接实际存在写入规格> 规格;
};

class 节点直接存在业务服务 final {
public:
    节点直接实际存在规格结果 形成实际存在写入规格(
        const 形成节点直接实际存在规格请求&) const noexcept {
        return {节点直接存在规格状态::未实现, std::nullopt};
    }
};

static_assert(noexcept(std::declval<const 节点直接存在业务服务&>()
    .形成实际存在写入规格(
        std::declval<const 形成节点直接实际存在规格请求&>())));

} // namespace 海中鱼巣
