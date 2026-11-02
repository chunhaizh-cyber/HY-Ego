#pragma once
#include <cstdint>
#include <vector>
#include "../核心/基础数据集.h"

namespace 海中鱼巣 {
enum class 新方法参数关系种类 : std::int64_t {
    同一=1, 不同=2, 特征所属存在=3, 集合包含存在=4, 同属场景=5, 特征引用存在=6,
    场景包含存在=7, 内容引用参数=8
};
struct 新方法参数关系 final {
    新方法参数关系种类 种类 = 新方法参数关系种类::同一;
    std::uint32_t 左参数序号=0, 右参数序号=0;
    std::vector<稳定编码> 左正式内容路径;
    friend bool operator==(const 新方法参数关系&,const 新方法参数关系&) = default;
};
enum class 新模拟操作状态 : std::int64_t { 已完成=1, 输入不适用=2, 输入耗尽=3, 停止取消=4, 等待触发=5 };
enum class 新方法知识来源种类 : std::int64_t { 未注明=0, 模拟=1 };
}
