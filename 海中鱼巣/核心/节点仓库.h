// 文件规则：节点仓库只维护节点身份和主信息锚点；外部业务写入必须经领域服务或正式仓库入口。
#pragma once

#include "句柄.h"
#include "主信息仓库.h"
#include "结果.结构写入.h"
#include "../领域/概念安全删除提交能力.数据.h"

#include <cstdint>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <unordered_map>
#include <vector>

namespace 海中鱼巣 {

class 节点仓库;

class 节点未发布候选 {
public:
    节点未发布候选() = delete;
    节点未发布候选(const 节点未发布候选&) = delete;
    节点未发布候选& operator=(const 节点未发布候选&) = delete;
    节点未发布候选(节点未发布候选&& 其它) noexcept
        : 仓库_(其它.仓库_), 令牌_(其它.令牌_), 节点_(其它.节点_), 阶段_(其它.阶段_) {
        其它.仓库_ = nullptr;
        其它.令牌_ = {};
        其它.节点_ = {};
        其它.阶段_ = 阶段::无效;
    }
    节点未发布候选& operator=(节点未发布候选&&) = delete;

    节点句柄 读取节点() const { return 节点_; }
    bool 完整() const {
        return 仓库_ != nullptr && 句柄有效(节点_)
            && 令牌_.域编号 != 0 && 令牌_.运行期纪元 != 0 && 令牌_.许可序号 != 0
            && 令牌_.类型 != 结构许可类型::无效 && 阶段_ != 阶段::无效;
    }

private:
    friend class 节点仓库;
    enum class 阶段 : std::uint8_t { 无效, 未发布, 已确认, 已撤销 };
    节点未发布候选(
        const 节点仓库* 仓库,
        结构事务令牌 令牌,
        节点句柄 节点)
        : 仓库_(仓库), 令牌_(令牌), 节点_(节点), 阶段_(阶段::未发布) {
    }

    const 节点仓库* 仓库_ = nullptr;
    结构事务令牌 令牌_;
    节点句柄 节点_;
    阶段 阶段_ = 阶段::无效;
};

struct 节点记录 {
    std::uint64_t 节点编号 = 0;
    节点类型 类型 = 节点类型::未分类;
    主信息句柄 主信息;
    std::uint32_t 版本号 = 0;
    记录状态 状态 = 记录状态::无效;
    std::uint64_t 创建序号 = 0;
};

struct 节点删除准备包 {
    节点句柄 目标;
    主信息句柄 主信息;
    std::uint32_t 预期版本 = 0;
    std::uint64_t 写集身份 = 0;

    bool 完整() const {
        return 句柄有效(目标) && 句柄有效(主信息)
            && 预期版本 == 目标.版本号 && 写集身份 != 0;
    }
};

class 节点仓库 {
public:
    explicit 节点仓库(const 主信息仓库& 主信息, std::uint64_t 仓库编号 = 1, 结构事务接线 接线 = {});

    节点句柄 创建节点(节点类型 类型, 主信息句柄 主信息);
    节点句柄 创建节点(节点类型 类型, 主信息句柄 主信息, const 结构事务令牌& 令牌);
    带值结构写入结果<节点未发布候选> 结构化创建节点未发布候选(
        节点类型 类型,
        主信息句柄 主信息,
        const 结构事务令牌& 令牌);
    std::optional<节点未发布候选> 创建节点未发布候选(
        节点类型 类型,
        主信息句柄 主信息,
        const 结构事务令牌& 令牌);
    结构写入结果 结构化确认未发布候选(
        节点未发布候选& 候选,
        const 结构事务令牌& 令牌);
    核心未发布候选操作状态 确认未发布候选(
        节点未发布候选& 候选,
        const 结构事务令牌& 令牌);
    结构写入结果 结构化撤销未发布候选(
        节点未发布候选& 候选,
        const 结构事务令牌& 令牌);
    核心未发布候选操作状态 撤销未发布候选(
        节点未发布候选& 候选,
        const 结构事务令牌& 令牌);
    std::optional<节点记录> 读取节点(节点句柄 节点) const;
    std::optional<节点记录> 读取节点(节点句柄 节点, const 结构事务令牌& 令牌) const;
    std::vector<节点记录> 读取指定类型有效节点记录组(
        节点类型 类型,
        const 结构事务令牌& 令牌) const;
    std::optional<节点记录> 读取节点审计(节点句柄 节点) const;
    std::optional<节点记录> 读取节点审计(节点句柄 节点, const 结构事务令牌& 令牌) const;
    std::optional<节点删除准备包> 准备节点删除包(节点句柄 节点, const 结构事务令牌& 令牌) const;
    bool 删除节点(节点句柄 节点);
    bool 删除节点(节点句柄 节点, const 结构事务令牌& 令牌);
    结构写入结果 严格删除节点(
        节点句柄 节点,
        const 结构事务令牌& 令牌);
    bool 节点是否有效(节点句柄 节点) const;
    bool 节点是否有效(节点句柄 节点, const 结构事务令牌& 令牌) const;
    std::uint64_t 有效节点数量() const;
    std::uint64_t 有效节点数量(const 结构事务令牌& 令牌) const;
    std::uint64_t 仓库编号() const;

private:
    friend class 领域::概念安全删除编排器;
    friend class 关系仓库;
    friend class 索引仓库;
    void 提交节点删除包(
        const 节点删除准备包& 包,
        const 结构事务令牌& 令牌,
        const 概念安全删除提交会话& 会话);
    const 主信息仓库& 主信息_;
    std::uint64_t 仓库编号_ = 1;
    结构事务接线 事务接线_;
    std::uint64_t 下个节点编号_ = 1;
    std::uint64_t 下个创建序号_ = 1;
    mutable std::shared_mutex 仓库锁_;
    std::unordered_map<std::uint64_t, 节点记录> 节点表_;
};

}
