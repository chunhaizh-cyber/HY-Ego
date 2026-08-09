// 文件规则：索引仓库只维护主键到节点句柄的查找关系，不裁决机器事实。
#pragma once

#include "句柄.h"
#include "节点仓库.h"
#include "结果.结构写入.h"
#include "索引所有权.数据.h"

#include <algorithm>
#include <cstdint>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <unordered_map>
#include <vector>

namespace 海中鱼巣 {

struct 主键绑定记录 {
    std::uint64_t 主键 = 0;
    节点句柄 节点;
    索引所有者声明 所有者声明 = 形成未知兼容索引所有者声明();
    std::uint32_t 探测序号 = 0;

    bool 完整() const {
        return 索引绑定请求{主键, 节点, 所有者声明, 探测序号}.兼容完整();
    }
};

inline bool operator==(const 主键绑定记录& 左, const 主键绑定记录& 右) noexcept {
    return 左.主键 == 右.主键 && 左.节点 == 右.节点
        && 左.所有者声明 == 右.所有者声明 && 左.探测序号 == 右.探测序号;
}

enum class 主键绑定组读取状态 : std::uint8_t {
    已形成,
    入口拒绝,
    资源失败,
    内部不一致
};

struct 主键绑定组读取结果 {
    主键绑定组读取状态 状态 = 主键绑定组读取状态::入口拒绝;
    std::vector<主键绑定记录> 记录组;
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
    结构写入结果 结构化绑定主键(
        const 索引绑定请求& 请求,
        const 结构事务令牌& 令牌);
    std::optional<节点句柄> 按主键查节点(std::uint64_t 主键) const;
    std::optional<节点句柄> 按主键查节点(std::uint64_t 主键, const 结构事务令牌& 令牌) const;
    std::optional<主键绑定记录> 读取主键绑定记录(
        std::uint64_t 主键,
        const 结构事务令牌& 令牌) const;
    std::vector<std::uint64_t> 读取节点主键组(节点句柄 节点) const;
    std::vector<std::uint64_t> 读取节点主键组(节点句柄 节点, const 结构事务令牌& 令牌) const;
    主键绑定组读取结果 读取全部主键绑定组(const 结构事务令牌& 令牌) const;
    bool 删除主键(std::uint64_t 主键);
    bool 删除主键(std::uint64_t 主键, const 结构事务令牌& 令牌);
    结构写入结果 严格删除主键(
        std::uint64_t 主键,
        节点句柄 预期节点,
        const 结构事务令牌& 令牌);
    std::uint64_t 有效主键数量() const;
    std::uint64_t 有效主键数量(const 结构事务令牌& 令牌) const;

private:
    const 节点仓库& 节点_;
    结构事务接线 事务接线_;
    mutable std::shared_mutex 仓库锁_;
    std::unordered_map<std::uint64_t, 主键绑定记录> 主键索引_;
    std::unordered_map<std::uint64_t, std::vector<std::uint64_t>> 节点主键组_;
    std::unordered_map<std::uint64_t, 主键绑定记录> 永久保留主键组_;
};

}
