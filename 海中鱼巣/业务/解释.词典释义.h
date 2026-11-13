#pragma once

#include "../适配/词典.离线数据源.h"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace 海中鱼巣 {

// 词典释义最小结构（7250 §2.6）：被释词条X由义项给出，各结构含义都是“X是一种N”。
enum class 词典释义结构 : std::uint8_t {
    一种N = 1,   // D01：一种 M? N
    N的一种,     // D02：M? N 的 一种／一类
    定义式       // D03：M 的 M? N
};

enum class 词典释义解读状态 : std::uint8_t {
    已解读 = 1,       // 唯一匹配一种结构
    缺规则,           // 无结构匹配（含另行裁决句型、字词说明、词性不适用），保留原文材料
    辅助表未配置,     // 词头表／繁简字表／词性表未就绪，读释义不可用（工程配置，不是未知词义）
    输入不合法        // 释义原文不是合法 UTF-8 或为空
};

// 释义中的一个切分词段；标量位置同时是原文与规范化文本的位置（繁简逐标量对应）。
struct 词典释义词段 final {
    std::size_t 起始标量 = 0, 结束标量 = 0;
    std::string 规范化词面;
    bool 是词头 = false;
};

struct 词典释义解读结果 final {
    词典释义解读状态 状态 = 词典释义解读状态::输入不合法;
    std::string 释义原文;       // 本行原文（未去句末“。”）
    std::string 规范化释义;     // 去句末“。”后的繁简规范化文本
    std::vector<词典释义词段> 词段组; // 以词典词头表正向最长匹配的切分（7250 §3.4第3项）
    std::optional<词典释义结构> 结构;
    std::optional<词典释义词段> 中心词;             // N
    std::optional<std::size_t> 修饰起始标量, 修饰结束标量; // M（未解析原文材料），无修饰时为空
    std::string 诊断;
};

// 读一个义项的全部释义行（义项内多行释义以换行分隔，逐行读）。
// 只读：不登记词条、名称、概念或任何事实；N未登记为词条时由调用方按 7250 §3.3.1第3项处理。
std::vector<词典释义解读结果> 解读词典义项释义(const 离线词典数据源&, const 词典义项材料&);

// 读单行释义；被释词面X用于排除“N与X相同”的自指释义。
词典释义解读结果 解读词典释义行(const 离线词典数据源&, std::string_view 被释词面,
    std::string_view 释义行, const std::optional<std::string>& 记录词性);

} // namespace 海中鱼巣
