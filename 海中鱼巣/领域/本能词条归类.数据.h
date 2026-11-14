#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "词条.数据.h"
#include "词典语言.数据.h"

namespace 海中鱼巣 {

enum class 新词条归类运行状态 : std::uint8_t {
    已就绪 = 1, 已完成, 条件未具备, 需重新组织, 已停止
};

struct 新词条归类本能输入 final {
    稳定编码 词条, 挂接特征, 归类材料, 执行主体, 采用概念;
    std::vector<稳定编码> 动作前概念组;
    std::optional<新词典归类需求来源> 来源;
    friend bool operator==(const 新词条归类本能输入&, const 新词条归类本能输入&) = default;
};

// 同次调用实际建立或复用的名称及挂接，不推定其它义项已处理。
struct 新本能词条归类回执 final {
    新词条归类本能输入 实际输入;
    稳定编码 动作;
    std::vector<稳定编码> 名称关系组;
    std::optional<稳定编码> 新状态, 变化动态;
    bool 新增名称 = false;
    新词条动态记录原因 动态原因 = 新词条动态记录原因::无值变化;
    friend bool operator==(const 新本能词条归类回执&, const 新本能词条归类回执&) = default;
};

struct 新词条归类运行处置 final {
    新词条归类运行状态 状态 = 新词条归类运行状态::条件未具备;
    std::vector<稳定编码> 变化依赖节点组;
    friend bool operator==(const 新词条归类运行处置&, const 新词条归类运行处置&) = default;
};

} // namespace 海中鱼巣
