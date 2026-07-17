// 文件规则：索引仓库只维护主键到节点句柄的查找关系，不裁决机器事实。
#pragma once

#include "句柄.h"
#include "节点仓库.h"
#include "结果.结构写入.h"
#include "../领域/概念安全删除提交能力.数据.h"

#include <algorithm>
#include <cstdint>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <unordered_map>
#include <vector>

namespace 海中鱼巣 {

struct 节点主键删除准备包 {
    节点句柄 目标;
    std::vector<std::uint64_t> 主键组;
    std::uint64_t 写集身份 = 0;

    bool 完整() const {
        if (!句柄有效(目标) || 写集身份 == 0) return false;
        for (std::size_t 索引 = 0; 索引 < 主键组.size(); ++索引) {
            if (主键组[索引] == 0
                || std::find(主键组.begin(), 主键组.begin() + 索引, 主键组[索引])
                    != 主键组.begin() + 索引) return false;
        }
        return true;
    }
};

struct 主键绑定记录 {
    std::uint64_t 主键 = 0;
    节点句柄 节点;

    bool 完整() const { return 主键 != 0 && 句柄有效(节点); }
};

class 索引仓库 {
public:
    explicit 索引仓库(const 节点仓库& 节点, 结构事务接线 接线 = {});

    bool 绑定主键(std::uint64_t 主键, 节点句柄 节点);
    bool 绑定主键(std::uint64_t 主键, 节点句柄 节点, const 结构事务令牌& 令牌);
    结构写入结果 结构化绑定主键(
        std::uint64_t 主键,
        节点句柄 节点,
        const 结构事务令牌& 令牌);
    std::optional<节点句柄> 按主键查节点(std::uint64_t 主键) const;
    std::optional<节点句柄> 按主键查节点(std::uint64_t 主键, const 结构事务令牌& 令牌) const;
    std::optional<主键绑定记录> 读取主键绑定记录(
        std::uint64_t 主键,
        const 结构事务令牌& 令牌) const;
    std::vector<std::uint64_t> 读取节点主键组(节点句柄 节点) const;
    std::vector<std::uint64_t> 读取节点主键组(节点句柄 节点, const 结构事务令牌& 令牌) const;
    std::vector<主键绑定记录> 读取全部主键绑定组(const 结构事务令牌& 令牌) const;
    std::optional<节点主键删除准备包> 准备节点主键删除包(
        节点句柄 节点,
        const std::vector<std::uint64_t>& 精确主键组,
        const 结构事务令牌& 令牌) const;
    bool 删除主键(std::uint64_t 主键);
    bool 删除主键(std::uint64_t 主键, const 结构事务令牌& 令牌);
    结构写入结果 严格删除主键(
        std::uint64_t 主键,
        节点句柄 预期节点,
        const 结构事务令牌& 令牌);
    std::uint64_t 有效主键数量() const;
    std::uint64_t 有效主键数量(const 结构事务令牌& 令牌) const;

private:
    friend class 领域::概念安全删除编排器;
    void 提交节点主键删除包(
        const 节点主键删除准备包& 包,
        const 结构事务令牌& 令牌,
        const 概念安全删除提交会话& 会话);
    const 节点仓库& 节点_;
    结构事务接线 事务接线_;
    mutable std::shared_mutex 仓库锁_;
    std::unordered_map<std::uint64_t, 节点句柄> 主键索引_;
    std::unordered_map<std::uint64_t, std::vector<std::uint64_t>> 节点主键组_;
};

}
