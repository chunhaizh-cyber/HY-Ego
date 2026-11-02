#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "信息内容.数据.h"

namespace 海中鱼巣 {

// 调用期表达目标，不是需求树中的新需求或字段已明确的事实。
struct 新正式字段询问目标 final {
    正式内容引用定位 被询内容;
    稳定编码 需求范围概念{}, 语言节点{};
    稳定编码 说话者{}, 受询者{}, 交互窗口{}, 交互作用域{};
    稳定编码 第三方指代概念{}, 选用指代名称关系{};
};

enum class 新字段询问文字状态 : std::uint8_t {
    已生成且唯一回读=1, 缺知识, 缺名称, 缺指称,
    多义, 范围未支持, 前提已变化
};
struct 新字段询问文字结果 final {
    新字段询问文字状态 状态=新字段询问文字状态::缺知识;
    std::string UTF8正文;
    std::vector<稳定编码> 所用知识节点组;
};

} // namespace 海中鱼巣
