#pragma once
#include <cstdint>
#include <vector>

namespace 海中鱼巣 {
enum class 新方法参数关系种类 : std::int64_t {
    同一=1, 不同=2, 特征所属存在=3, 集合包含存在=4, 同属场景=5
};
struct 新方法参数关系 final {
    新方法参数关系种类 种类 = 新方法参数关系种类::同一;
    std::uint32_t 左参数序号=0, 右参数序号=0;
    friend bool operator==(const 新方法参数关系&,const 新方法参数关系&) = default;
};
enum class 新模拟操作状态 : std::int64_t { 已完成=1, 输入不适用=2, 输入耗尽=3, 停止取消=4 };
enum class 新方法知识来源种类 : std::int64_t { 未注明=0, 模拟=1 };
}
