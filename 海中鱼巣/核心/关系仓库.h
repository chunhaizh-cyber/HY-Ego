// 文件规则：关系变更必须通过本仓库入口；不得裸改关系记录。
#pragma once

#include "句柄.h"
#include "节点仓库.h"

#include <cstdint>
#include <optional>
#include <unordered_map>
#include <vector>

namespace 海中鱼巣 {

struct 关系记录 {
    std::uint64_t 关系编号 = 0;
    关系类型 类型 = 关系类型::普通父子;
    节点句柄 源节点;
    节点句柄 目标节点;
    std::int64_t 顺序号 = 0;
    std::uint32_t 版本号 = 0;
    记录状态 状态 = 记录状态::无效;
};

class 关系仓库 {
public:
    explicit 关系仓库(const 节点仓库& 节点, std::uint64_t 仓库编号 = 1);

    关系句柄 创建关系(关系类型 类型, 节点句柄 源节点, 节点句柄 目标节点, std::int64_t 顺序号 = 0);
    std::optional<关系记录> 读取关系(关系句柄 关系) const;
    bool 删除关系(关系句柄 关系);
    bool 重挂关系(关系句柄 关系, 节点句柄 新源节点, 节点句柄 新目标节点);
    bool 重挂节点(节点句柄 节点, 节点句柄 新父节点);
    std::vector<节点句柄> 获取子节点(节点句柄 父节点) const;
    std::optional<节点句柄> 获取父节点(节点句柄 子节点) const;
    std::optional<节点句柄> 获取目标节点(节点句柄 源节点, 关系类型 类型) const;
    std::vector<节点句柄> 获取目标节点组(节点句柄 源节点, 关系类型 类型) const;
    std::vector<节点句柄> 获取目标节点组(节点句柄 源节点, 关系类型 类型, std::int64_t 顺序号) const;
    bool 存在关系(关系类型 类型, 节点句柄 源节点, 节点句柄 目标节点) const;
    bool 存在目标关系(关系类型 类型, 节点句柄 目标节点) const;
    std::uint64_t 有效关系数量() const;

private:
    bool 节点在父链中(节点句柄 起点, 节点句柄 目标) const;

    const 节点仓库& 节点_;
    std::uint64_t 仓库编号_ = 1;
    std::uint64_t 下个关系编号_ = 1;
    std::unordered_map<std::uint64_t, 关系记录> 关系表_;
};

}
