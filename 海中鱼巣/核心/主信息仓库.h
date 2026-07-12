// 文件规则：主信息只保存身份、拓扑锚点、值容器或非权威缓存；不得在此加入业务裁决字段。
#pragma once

#include "句柄.h"
#include "结构事务接线.数据.h"

#include <cstdint>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <unordered_map>
#include <vector>

namespace 海中鱼巣 {

struct 主信息记录 {
    std::uint64_t 主信息编号 = 0;
    std::uint32_t 版本号 = 0;
    记录状态 状态 = 记录状态::无效;
    std::uint64_t 拓扑锚点编号 = 0;
    std::vector<std::optional<std::int64_t>> 值容器;
};

class 主信息仓库 {
public:
    explicit 主信息仓库(std::uint64_t 仓库编号 = 1, 结构事务接线 接线 = {});

    主信息句柄 创建主信息();
    主信息句柄 创建主信息(const 结构事务令牌& 令牌);
    std::optional<主信息记录> 读取主信息(主信息句柄 主信息) const;
    std::optional<主信息记录> 读取主信息(主信息句柄 主信息, const 结构事务令牌& 令牌) const;
    bool 删除主信息(主信息句柄 主信息);
    bool 删除主信息(主信息句柄 主信息, const 结构事务令牌& 令牌);
    bool 主信息是否有效(主信息句柄 主信息) const;
    bool 主信息是否有效(主信息句柄 主信息, const 结构事务令牌& 令牌) const;
    bool 写入I64值(主信息句柄 主信息, std::int64_t 值);
    bool 写入I64值(主信息句柄 主信息, std::uint64_t 值索引, std::int64_t 值);
    bool 写入I64值(主信息句柄 主信息, std::uint64_t 值索引, std::int64_t 值, const 结构事务令牌& 令牌);
    std::optional<std::int64_t> 读取I64值(主信息句柄 主信息) const;
    std::optional<std::int64_t> 读取I64值(主信息句柄 主信息, std::uint64_t 值索引) const;
    std::optional<std::int64_t> 读取I64值(主信息句柄 主信息, std::uint64_t 值索引, const 结构事务令牌& 令牌) const;

private:
    friend class 节点仓库;
    std::uint64_t 仓库编号_ = 1;
    结构事务接线 事务接线_;
    std::uint64_t 下个主信息编号_ = 1;
    mutable std::shared_mutex 仓库锁_;
    std::unordered_map<std::uint64_t, 主信息记录> 主信息表_;
};

}
