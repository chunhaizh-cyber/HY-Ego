#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "../核心/基础数据集.h"

namespace 海中鱼巣 {

enum class 新词典语言理解状态 : std::uint8_t { 已理解 = 1, 缺规则 };
enum class 新词典词段依据 : std::uint8_t { 同批词头表 = 1, 单标量回退 };

struct 新词典语言词段 final {
    std::int64_t 起始标量 = 0, 结束标量 = 0;
    std::string 规范化词面;
    新词典词段依据 依据 = 新词典词段依据::单标量回退;
    friend bool operator==(const 新词典语言词段&, const 新词典语言词段&) = default;
};

struct 新词典归类材料定义 final {
    稳定编码 材料Q, 义项I, 来源场景, 说话者, 释义语言, 词条X;
    std::int64_t 原行号 = 0, 原文起始标量 = 0, 原文结束标量 = 0;
    std::string 释义原文, 规范化释义;
    新词典语言理解状态 状态 = 新词典语言理解状态::缺规则;
    std::optional<std::int64_t> 结构编号;
    std::vector<新词典语言词段> 词段组;
    std::optional<新词典语言词段> 中心词;
    std::optional<std::int64_t> 修饰起始标量, 修饰结束标量;
    std::optional<稳定编码> 词条N;
    std::string 诊断;
    friend bool operator==(const 新词典归类材料定义&, const 新词典归类材料定义&) = default;
};

struct 新词典归类材料信息 final {
    稳定编码 节点;
    新词典归类材料定义 定义;
};

} // namespace 海中鱼巣
