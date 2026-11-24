#pragma once
#include "关系陈述.语言服务.h"
#include "../领域/关系概念类.h"
#include <string>
#include <vector>

namespace 海中鱼巣 {

struct 关系概念出站自检依赖 final {
    关系概念类& 关系;
    关系陈述语言服务& 陈述;
    新_词类& 词;
    新_存在类& 存在;
    新_需求类& 需求;
    稳定编码 中文语言{};
    std::optional<稳定编码> 英文语言;
    std::string 种子文本;
    // 正式装配得到的存在节点对；空则跳过“正式组成成句”断言并记未验证。
    std::optional<稳定编码> 正式整体存在;
    std::optional<稳定编码> 正式部分存在;
    // 正式需求父子；空则跳过需求父子成句断言并记未验证。
    std::optional<稳定编码> 正式父需求;
    std::optional<稳定编码> 正式子需求;
};

struct 关系概念出站自检条目 final {
    std::string 标识;
    bool 通过 = false;
    std::string 实际验证说明;
};

struct 关系概念出站自检结果 final {
    bool 全部强制通过 = false;
    std::vector<关系概念出站自检条目> 条目组;
};

// 功能自检（用户已授权测试）。缺规则/缺正式数据的项记未验证，不报功能通过。
关系概念出站自检结果 运行关系概念出站自检(const 关系概念出站自检依赖&);

} // namespace 海中鱼巣
