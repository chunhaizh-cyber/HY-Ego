#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "../核心/基础数据集.h"

namespace 海中鱼巣 {

// 物理全局根（1150 §2、4200 §2）：只挂接各世界根，不是逻辑场景，
// 不持有成员、特征、状态、动态或专属概念。各世界逻辑独立，世界根在本世界
// 场景树内仍无父场景；全局根关系只由“世界根挂接”边表达。
extern 稳定编码 全局根场景节点;

// 查找世界类型登记节点的固定键；世界类型身份是登记节点本身。
enum class 世界类型登记键 : std::int64_t {
    现实 = 1,
    自然语言 = 2,
    内部治理 = 3,
    模拟 = 4,
    外设 = 5
};

enum class 全局根操作状态 : std::uint8_t {
    已完成 = 1,
    无变化 = 2,
    入口拒绝 = 3,
    结构不一致 = 4,
    资源失败 = 5
};

struct 世界根挂接信息 final {
    稳定编码 世界类型节点;
    稳定编码 世界根节点;
    std::int64_t 登记键 = 0;
    friend bool operator==(const 世界根挂接信息&, const 世界根挂接信息&) = default;
};

struct 全局根读回结果 final {
    全局根操作状态 状态 = 全局根操作状态::入口拒绝;
    std::optional<稳定编码> 全局根节点;
    std::vector<世界根挂接信息> 挂接组; // 按登记键升序
};

// 无实例状态；全部入口静态、内部单互斥，不调用任何世界服务。
// 诊断责任：业务非成功向上送出；读回不符等内部不变量破坏本地记录并工程终止。
class 新_全局根类 final {
public:
    新_全局根类() = delete;
    // 建立或复用全局根与全部已知世界类型登记节点；重复调用精确复用。
    static 全局根操作状态 初始化() noexcept;
    // 把世界根挂到全局根下并写入其所属世界类型；同类型同根为无变化。
    static 全局根操作状态 挂接世界根(世界类型登记键 键, 稳定编码 世界根) noexcept;
    static std::optional<稳定编码> 查询世界根(世界类型登记键 键) noexcept;
    static 全局根读回结果 完整读回() noexcept;
};

} // namespace 海中鱼巣
