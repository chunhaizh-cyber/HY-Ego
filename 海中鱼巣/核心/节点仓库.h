// 文件规则：节点仓库只维护节点身份和主信息锚点；外部业务写入必须经领域服务或正式仓库入口。
#pragma once

#include "句柄.h"
#include "主信息仓库.h"

#include <cstdint>
#include <optional>
#include <unordered_map>

namespace 海中鱼巣 {

struct 节点记录 {
    std::uint64_t 节点编号 = 0;
    节点类型 类型 = 节点类型::未分类;
    主信息句柄 主信息;
    std::uint32_t 版本号 = 0;
    记录状态 状态 = 记录状态::无效;
    std::uint64_t 创建序号 = 0;
};

class 节点仓库 {
public:
    explicit 节点仓库(std::uint64_t 仓库编号 = 1);

    节点句柄 创建节点(节点类型 类型, 主信息句柄 主信息);
    std::optional<节点记录> 读取节点(节点句柄 节点) const;
    bool 删除节点(节点句柄 节点);
    bool 节点是否有效(节点句柄 节点) const;
    std::uint64_t 仓库编号() const;

private:
    std::uint64_t 仓库编号_ = 1;
    std::uint64_t 下个节点编号_ = 1;
    std::uint64_t 下个创建序号_ = 1;
    std::unordered_map<std::uint64_t, 节点记录> 节点表_;
};

}
