#pragma once

#include "../核心/基础数据集.h"

namespace 海中鱼巣 {

class 基础持久视图;

struct 特征概念恢复读回结果 final {
    基础结构操作状态 状态 = 基础结构操作状态::入口拒绝;
    稳定编码 根;
    稳定编码 词典来源批次类型根;
    std::size_t 键数 = 0;
    稳定编码 服务权限类型根;
};

// 只读候选中已发表的 owner 结构和普通先天槽，不重绑运行期 owner。
特征概念恢复读回结果 读取恢复特征概念结构(const 基础持久视图& 视图) noexcept;

} // namespace 海中鱼巣
