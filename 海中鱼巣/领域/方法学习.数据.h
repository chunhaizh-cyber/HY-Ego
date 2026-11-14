#pragma once

#include <optional>
#include <vector>
#include "词典.数据.h"
#include "新_方法公共类型.h"

namespace 海中鱼巣 {

struct 新词典定向学习输入 final {
    std::vector<新方法有序参数引用> 参数组;
    稳定编码 词条语言, 规范化合同;
    std::string 原规范化词面, 可信批次标识;
    std::uint64_t 来源场景标识 = 0;
    friend bool operator==(const 新词典定向学习输入&, const 新词典定向学习输入&) = default;
};
struct 新方法学习请求 final {
    稳定编码 方法节点;
    std::optional<新词典定向学习输入> 定向输入;
    friend bool operator==(const 新方法学习请求&, const 新方法学习请求&) = default;
};

enum class 新定向学习结局 : std::uint8_t {
    已完成学习=1, 已被现有知识覆盖, 已有空结果配对,
    正式未命中, 请求已无需要, 需重新组织输入, 条件未具备
};
// 正常调度结局，不是方法执行结果，不作为条件结果样本。
struct 新定向学习结束材料 final {
    新定向学习结局 结局=新定向学习结局::已完成学习;
    std::vector<新词典学习缺口> 缺口组;
    std::vector<稳定编码> 正式变化依赖节点组;
};

} // namespace 海中鱼巣
