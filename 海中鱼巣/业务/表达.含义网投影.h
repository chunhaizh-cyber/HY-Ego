#pragma once
// CONV-EXPR-TEXT：具体含义网投影为旧含义图，作为成文与整段回读的内部中间表示。
// 旧图与23条描述结构在CONV-INPUT（解释器直接产出含义网）时整体退出。
#include <optional>
#include "../领域/语言含义网.数据.h"
#include "../领域/语言含义图.数据.h"

namespace 海中鱼巣 {
struct 新含义网投影上下文 final {
    稳定编码 语言节点{};
    std::optional<稳定编码> 正式指称作用域;
    // 询问算子的参与者在旧图中无位置，须与本次成文的说话者、受话者完全一致。
    std::optional<新绑定指称> 说话者,受话者;
};
enum class 新含义网投影状态 : std::uint8_t { 已投影=1, 形状未支持 };
struct 新含义网投影结果 final {
    新含义网投影状态 状态=新含义网投影状态::形状未支持;
    std::optional<语言图含义材料> 图;
};
// 只读、无owner访问；未列入投影表的形状整体返回形状未支持，不截断、不丢弃算子或角色。
新含义网投影结果 含义网投影旧图(const 新语言含义网&, const 新含义网投影上下文&);
} // namespace 海中鱼巣
