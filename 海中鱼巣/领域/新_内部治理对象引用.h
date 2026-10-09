#pragma once

#include <cstdint>
#include <optional>
#include "../核心/基础数据集.h"

namespace 海中鱼巣 {

struct 新任务虚拟存在引用 final {
    稳定编码 任务节点;
    稳定编码 虚拟存在节点;
    稳定编码 状态特征节点;
    friend bool operator==(const 新任务虚拟存在引用&,
        const 新任务虚拟存在引用&) = default;
};

struct 新任务虚拟存在读回 final {
    新任务虚拟存在引用 引用;
    稳定编码 状态特征类型概念节点;
    std::int64_t 当前状态 = 0;
};

// 中立只读合同：目标结构由其唯一owner核验，动态/场景不依赖任务实现。
class 新内部治理对象读取接口 {
public:
    virtual ~新内部治理对象读取接口() = default;
    virtual std::optional<新任务虚拟存在读回> 读取任务虚拟存在引用(
        const 新任务虚拟存在引用&) const noexcept = 0;
};

} // namespace 海中鱼巣
