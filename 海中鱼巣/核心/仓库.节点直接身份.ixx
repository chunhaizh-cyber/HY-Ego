// 文件规则：本模块只维护隔离新域的节点直接身份、候选生命周期和值式读取；不得依赖旧主信息、旧仓库或领域载荷。
module;

#include "句柄.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <limits>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

export module 海中鱼巣.核心.仓库.节点直接身份;

export namespace 海中鱼巣 {

enum class 节点直接身份写入状态 : std::uint8_t {
    已形成候选,
    幂等读回,
    已确认待发布,
    已撤销,
    已发布,
    身份冲突,
    入口拒绝,
    事务拒绝,
    版本漂移,
    资源失败,
    内部不一致
};

enum class 稳定主键当前身份状态 : std::uint8_t {
    未占用,
    当前有效,
    历史占用,
    入口拒绝,
    内部不一致
};

struct 节点直接身份记录 {
    std::uint64_t 节点编号 = 0;
    节点稳定主键 稳定主键;
    节点类型 类型 = 节点类型::未分类;
    std::uint32_t 版本号 = 0;
    记录状态 状态 = 记录状态::无效;
    std::uint64_t 创建序号 = 0;
};

struct 稳定主键当前身份读取结果 {
    稳定主键当前身份状态 状态 = 稳定主键当前身份状态::入口拒绝;
    std::optional<节点句柄> 当前身份;
    std::optional<节点直接身份记录> 当前记录;
};

struct 节点稳定主键高水位 {
    std::uint64_t 命名域 = 0;
    std::uint64_t 键值 = 0;
};

struct 节点稳定主键历史占用 {
    节点稳定主键 稳定主键;
    节点类型 类型 = 节点类型::未分类;
    std::uint64_t 节点编号 = 0;
    bool 曾发布 = false;
};

struct 节点直接身份仓库权威材料 {
    std::uint64_t 仓库编号 = 0;
    std::uint64_t 下个节点编号 = 0;
    std::uint64_t 下个创建序号 = 0;
    std::vector<节点稳定主键高水位> 每域高水位;
    std::vector<节点稳定主键历史占用> 历史占用;
    std::vector<节点直接身份记录> 记录组;
};

class 节点直接身份仓库;

class 节点直接身份未发布候选 {
public:
    节点直接身份未发布候选() = delete;
    节点直接身份未发布候选(const 节点直接身份未发布候选&) = delete;
    节点直接身份未发布候选& operator=(const 节点直接身份未发布候选&) = delete;
    节点直接身份未发布候选(节点直接身份未发布候选&& 其它) noexcept
        : 仓库_(其它.仓库_), 事务序号_(其它.事务序号_), 节点_(其它.节点_),
          稳定主键_(其它.稳定主键_), 阶段_(其它.阶段_) {
        其它.仓库_ = nullptr;
        其它.事务序号_ = 0;
        其它.节点_ = {};
        其它.稳定主键_ = {};
        其它.阶段_ = 候选阶段::无效;
    }
    节点直接身份未发布候选& operator=(节点直接身份未发布候选&&) = delete;

    节点句柄 读取节点() const noexcept { return 节点_; }
    节点稳定主键 读取稳定主键() const noexcept { return 稳定主键_; }
    bool 完整() const noexcept {
        return 仓库_ != nullptr && 事务序号_ != 0 && 句柄有效(节点_)
            && 稳定主键有效(稳定主键_) && 阶段_ != 候选阶段::无效;
    }

private:
    friend class 节点直接身份仓库;
    enum class 候选阶段 : std::uint8_t { 无效, 未发布, 已确认待发布, 已撤销, 已发布 };

    节点直接身份未发布候选(
        const 节点直接身份仓库* 仓库,
        std::uint64_t 事务序号,
        节点句柄 节点,
        节点稳定主键 稳定主键) noexcept
        : 仓库_(仓库), 事务序号_(事务序号), 节点_(节点),
          稳定主键_(稳定主键), 阶段_(候选阶段::未发布) {
    }

    const 节点直接身份仓库* 仓库_ = nullptr;
    std::uint64_t 事务序号_ = 0;
    节点句柄 节点_;
    节点稳定主键 稳定主键_;
    候选阶段 阶段_ = 候选阶段::无效;
};

class 节点直接身份删除未发布候选 {
public:
    节点直接身份删除未发布候选() = delete;
    节点直接身份删除未发布候选(const 节点直接身份删除未发布候选&) = delete;
    节点直接身份删除未发布候选& operator=(const 节点直接身份删除未发布候选&) = delete;
    节点直接身份删除未发布候选(节点直接身份删除未发布候选&& 其它) noexcept
        : 仓库_(其它.仓库_), 事务序号_(其它.事务序号_), 写前节点_(其它.写前节点_),
          写后节点_(其它.写后节点_), 阶段_(其它.阶段_) {
        其它.仓库_ = nullptr;
        其它.事务序号_ = 0;
        其它.写前节点_ = {};
        其它.写后节点_ = {};
        其它.阶段_ = 候选阶段::无效;
    }
    节点直接身份删除未发布候选& operator=(节点直接身份删除未发布候选&&) = delete;

    节点句柄 读取写前节点() const noexcept { return 写前节点_; }
    节点句柄 读取写后节点() const noexcept { return 写后节点_; }
    bool 完整() const noexcept {
        return 仓库_ != nullptr && 事务序号_ != 0
            && 句柄有效(写前节点_) && 句柄有效(写后节点_)
            && 写前节点_.仓库编号 == 写后节点_.仓库编号
            && 写前节点_.节点编号 == 写后节点_.节点编号
            && 写前节点_.版本号 != std::numeric_limits<std::uint32_t>::max()
            && 写前节点_.版本号 + 1 == 写后节点_.版本号
            && 阶段_ != 候选阶段::无效;
    }

private:
    friend class 节点直接身份仓库;
    enum class 候选阶段 : std::uint8_t { 无效, 未发布, 已确认待发布, 已撤销, 已发布 };

    节点直接身份删除未发布候选(
        const 节点直接身份仓库* 仓库,
        std::uint64_t 事务序号,
        节点句柄 写前节点,
        节点句柄 写后节点) noexcept
        : 仓库_(仓库), 事务序号_(事务序号), 写前节点_(写前节点),
          写后节点_(写后节点), 阶段_(候选阶段::未发布) {
    }

    const 节点直接身份仓库* 仓库_ = nullptr;
    std::uint64_t 事务序号_ = 0;
    节点句柄 写前节点_;
    节点句柄 写后节点_;
    候选阶段 阶段_ = 候选阶段::无效;
};

struct 节点直接身份操作结果 {
    节点直接身份写入状态 状态 = 节点直接身份写入状态::入口拒绝;
    节点句柄 当前身份;

    bool 成功() const noexcept {
        return 状态 == 节点直接身份写入状态::已形成候选
            || 状态 == 节点直接身份写入状态::幂等读回
            || 状态 == 节点直接身份写入状态::已确认待发布
            || 状态 == 节点直接身份写入状态::已撤销
            || 状态 == 节点直接身份写入状态::已发布;
    }
};

struct 节点直接身份创建结果 {
    节点直接身份写入状态 状态 = 节点直接身份写入状态::入口拒绝;
    节点句柄 当前身份;
    std::optional<节点直接身份未发布候选> 候选;

    bool 成功() const noexcept {
        return 状态 == 节点直接身份写入状态::幂等读回
            || (状态 == 节点直接身份写入状态::已形成候选 && 候选.has_value());
    }
};

struct 节点直接身份删除结果 {
    节点直接身份写入状态 状态 = 节点直接身份写入状态::入口拒绝;
    节点句柄 当前身份;
    std::optional<节点直接身份删除未发布候选> 候选;

    bool 成功() const noexcept {
        return 状态 == 节点直接身份写入状态::幂等读回
            || (状态 == 节点直接身份写入状态::已形成候选 && 候选.has_value());
    }
};

class 节点直接身份仓库 {
public:
    explicit 节点直接身份仓库(std::uint64_t 仓库编号 = 1)
        : 仓库编号_(仓库编号) {
        if (仓库编号_ == 0) {
            throw std::invalid_argument("节点直接身份仓库编号必须非零");
        }
    }

    节点直接身份仓库(const 节点直接身份仓库&) = delete;
    节点直接身份仓库& operator=(const 节点直接身份仓库&) = delete;

    static bool 命名域已定义(节点稳定主键命名域 命名域) noexcept {
        switch (命名域) {
        case 节点稳定主键命名域::词身份:
        case 节点稳定主键命名域::语素入口身份:
        case 节点稳定主键命名域::基础信息身份:
        case 节点稳定主键命名域::存在身份:
        case 节点稳定主键命名域::场景身份:
        case 节点稳定主键命名域::抽象特征定义身份:
        case 节点稳定主键命名域::实例特征槽身份:
        case 节点稳定主键命名域::特征值身份:
        case 节点稳定主键命名域::需求身份:
        case 节点稳定主键命名域::任务身份:
        case 节点稳定主键命名域::方法身份:
        case 节点稳定主键命名域::动态身份:
        case 节点稳定主键命名域::因果身份:
        case 节点稳定主键命名域::状态身份:
        case 节点稳定主键命名域::二次特征身份:
        case 节点稳定主键命名域::任务方法选择记录身份:
        case 节点稳定主键命名域::用途观察记录身份:
        case 节点稳定主键命名域::文章身份:
        case 节点稳定主键命名域::段落身份:
        case 节点稳定主键命名域::自然句身份:
        case 节点稳定主键命名域::子句身份:
            return true;
        default:
            return false;
        }
    }

    static bool 命名域与节点类型匹配(节点稳定主键命名域 命名域, 节点类型 类型) noexcept {
        switch (命名域) {
        case 节点稳定主键命名域::词身份:
        case 节点稳定主键命名域::语素入口身份:
            return 类型 == 节点类型::语素;
        case 节点稳定主键命名域::基础信息身份:
            return 类型 == 节点类型::基础信息;
        case 节点稳定主键命名域::存在身份:
            return 类型 == 节点类型::存在;
        case 节点稳定主键命名域::场景身份:
            return 类型 == 节点类型::场景;
        case 节点稳定主键命名域::抽象特征定义身份:
        case 节点稳定主键命名域::实例特征槽身份:
            return 类型 == 节点类型::特征;
        case 节点稳定主键命名域::特征值身份:
            return 类型 == 节点类型::特征值;
        case 节点稳定主键命名域::需求身份:
            return 类型 == 节点类型::需求;
        case 节点稳定主键命名域::任务身份:
            return 类型 == 节点类型::任务;
        case 节点稳定主键命名域::方法身份:
            return 类型 == 节点类型::方法;
        case 节点稳定主键命名域::动态身份:
            return 类型 == 节点类型::动态;
        case 节点稳定主键命名域::因果身份:
            return 类型 == 节点类型::因果引用;
        case 节点稳定主键命名域::状态身份:
            return 类型 == 节点类型::状态;
        case 节点稳定主键命名域::二次特征身份:
            return 类型 == 节点类型::二次特征;
        case 节点稳定主键命名域::任务方法选择记录身份:
            return 类型 == 节点类型::任务方法选择记录;
        case 节点稳定主键命名域::用途观察记录身份:
            return 类型 == 节点类型::用途观察记录;
        case 节点稳定主键命名域::文章身份:
            return 类型 == 节点类型::文章;
        case 节点稳定主键命名域::段落身份:
            return 类型 == 节点类型::段落;
        case 节点稳定主键命名域::自然句身份:
            return 类型 == 节点类型::自然句;
        case 节点稳定主键命名域::子句身份:
            return 类型 == 节点类型::子句;
        default:
            return false;
        }
    }

    节点直接身份创建结果 结构化创建节点未发布候选(
        节点类型 类型,
        节点稳定主键 稳定主键,
        std::uint64_t 事务序号) {
        节点直接身份创建结果 结果;
        if (事务序号 == 0) {
            结果.状态 = 节点直接身份写入状态::事务拒绝;
            return 结果;
        }
        const auto 命名域 = static_cast<节点稳定主键命名域>(稳定主键.命名域);
        if (!稳定主键有效(稳定主键) || !命名域与节点类型匹配(命名域, 类型)) {
            return 结果;
        }

        std::unique_lock<std::shared_mutex> 锁(仓库锁_);
        const auto 已占用 = 主键占用表_.find(稳定主键);
        if (已占用 != 主键占用表_.end()) {
            const auto 记录位置 = 节点表_.find(已占用->second.节点编号);
            if (记录位置 != 节点表_.end() && 记录位置->second.已发布
                && 记录位置->second.记录.状态 == 记录状态::有效) {
                结果.当前身份 = 形成句柄(记录位置->second.记录);
                结果.状态 = 已占用->second.类型 == 类型
                    ? 节点直接身份写入状态::幂等读回
                    : 节点直接身份写入状态::身份冲突;
                return 结果;
            }
            if (记录位置 != 节点表_.end() && !记录位置->second.已发布
                && 记录位置->second.创建事务序号 == 事务序号
                && 已占用->second.类型 == 类型) {
                结果.当前身份 = 形成句柄(记录位置->second.记录);
                结果.状态 = 节点直接身份写入状态::幂等读回;
                return 结果;
            }
            结果.状态 = 节点直接身份写入状态::身份冲突;
            return 结果;
        }

        auto& 高水位 = 每域高水位_[稳定主键.命名域];
        if (高水位 == std::numeric_limits<std::uint64_t>::max()
            || 稳定主键.键值 != 高水位 + 1
            || 下个节点编号_ == std::numeric_limits<std::uint64_t>::max()
            || 下个创建序号_ == std::numeric_limits<std::uint64_t>::max()) {
            结果.状态 = 节点直接身份写入状态::身份冲突;
            return 结果;
        }

        高水位 = 稳定主键.键值;
        const auto 节点编号 = 下个节点编号_++;
        const auto 创建序号 = 下个创建序号_++;
        节点直接身份记录 记录{节点编号, 稳定主键, 类型, 1, 记录状态::有效, 创建序号};
        try {
            const auto [记录位置, 已插入记录] = 节点表_.emplace(
                节点编号, 节点内部条目{记录, false, 事务序号, 0, false});
            if (!已插入记录) {
                结果.状态 = 节点直接身份写入状态::内部不一致;
                return 结果;
            }
            try {
                const auto [占用位置, 已插入占用] = 主键占用表_.emplace(
                    稳定主键, 主键占用条目{类型, 节点编号, false});
                if (!已插入占用) {
                    节点表_.erase(记录位置);
                    结果.状态 = 节点直接身份写入状态::内部不一致;
                    return 结果;
                }
                (void)占用位置;
            } catch (...) {
                节点表_.erase(记录位置);
                结果.状态 = 节点直接身份写入状态::资源失败;
                return 结果;
            }
        } catch (...) {
            结果.状态 = 节点直接身份写入状态::资源失败;
            return 结果;
        }

        结果.当前身份 = {仓库编号_, 节点编号, 1};
        结果.状态 = 节点直接身份写入状态::已形成候选;
        结果.候选.emplace(节点直接身份未发布候选{
            this, 事务序号, 结果.当前身份, 稳定主键});
        return 结果;
    }

    节点直接身份操作结果 确认节点候选(
        节点直接身份未发布候选& 候选,
        std::uint64_t 事务序号) {
        节点直接身份操作结果 结果{节点直接身份写入状态::入口拒绝, 候选.读取节点()};
        if (!候选属于本次事务(候选, 事务序号)) {
            结果.状态 = 节点直接身份写入状态::事务拒绝;
            return 结果;
        }
        if (候选.阶段_ == 节点直接身份未发布候选::候选阶段::已确认待发布) {
            结果.状态 = 节点直接身份写入状态::已确认待发布;
            return 结果;
        }
        if (候选.阶段_ != 节点直接身份未发布候选::候选阶段::未发布) {
            return 结果;
        }
        std::shared_lock<std::shared_mutex> 锁(仓库锁_);
        const auto 位置 = 节点表_.find(候选.节点_.节点编号);
        if (!候选条目匹配(位置, 候选)) {
            结果.状态 = 节点直接身份写入状态::内部不一致;
            return 结果;
        }
        候选.阶段_ = 节点直接身份未发布候选::候选阶段::已确认待发布;
        结果.状态 = 节点直接身份写入状态::已确认待发布;
        return 结果;
    }

    节点直接身份操作结果 撤销节点候选(
        节点直接身份未发布候选& 候选,
        std::uint64_t 事务序号) {
        节点直接身份操作结果 结果{节点直接身份写入状态::入口拒绝, 候选.读取节点()};
        if (!候选属于本次事务(候选, 事务序号)) {
            结果.状态 = 节点直接身份写入状态::事务拒绝;
            return 结果;
        }
        if (候选.阶段_ == 节点直接身份未发布候选::候选阶段::已撤销) {
            结果.状态 = 节点直接身份写入状态::已撤销;
            return 结果;
        }
        if (候选.阶段_ != 节点直接身份未发布候选::候选阶段::未发布
            && 候选.阶段_ != 节点直接身份未发布候选::候选阶段::已确认待发布) {
            return 结果;
        }
        std::unique_lock<std::shared_mutex> 锁(仓库锁_);
        const auto 位置 = 节点表_.find(候选.节点_.节点编号);
        if (!候选条目匹配(位置, 候选)) {
            结果.状态 = 节点直接身份写入状态::内部不一致;
            return 结果;
        }
        节点表_.erase(位置);
        候选.阶段_ = 节点直接身份未发布候选::候选阶段::已撤销;
        结果.状态 = 节点直接身份写入状态::已撤销;
        return 结果;
    }

    void 完成发布节点候选(
        节点直接身份未发布候选& 候选,
        std::uint64_t 事务序号) noexcept {
        if (!候选属于本次事务(候选, 事务序号)
            || 候选.阶段_ != 节点直接身份未发布候选::候选阶段::已确认待发布) {
            std::terminate();
        }
        std::unique_lock<std::shared_mutex> 锁(仓库锁_);
        const auto 位置 = 节点表_.find(候选.节点_.节点编号);
        if (!候选条目匹配(位置, 候选)) {
            std::terminate();
        }
        位置->second.已发布 = true;
        const auto 占用位置 = 主键占用表_.find(候选.稳定主键_);
        if (占用位置 == 主键占用表_.end()
            || 占用位置->second.节点编号 != 候选.节点_.节点编号) {
            std::terminate();
        }
        占用位置->second.曾发布 = true;
        候选.阶段_ = 节点直接身份未发布候选::候选阶段::已发布;
    }

    节点直接身份删除结果 结构化删除节点未发布候选(
        节点句柄 节点,
        std::uint64_t 事务序号) {
        节点直接身份删除结果 结果;
        if (事务序号 == 0) {
            结果.状态 = 节点直接身份写入状态::事务拒绝;
            return 结果;
        }
        if (!句柄有效(节点) || 节点.仓库编号 != 仓库编号_) {
            return 结果;
        }
        std::unique_lock<std::shared_mutex> 锁(仓库锁_);
        const auto 位置 = 节点表_.find(节点.节点编号);
        if (位置 == 节点表_.end() || !位置->second.已发布) {
            return 结果;
        }
        auto& 条目 = 位置->second;
        结果.当前身份 = 形成句柄(条目.记录);
        if (节点.版本号 != 条目.记录.版本号) {
            const bool 已完成同次删除 = 条目.记录.状态 == 记录状态::已删除
                && 节点.版本号 != std::numeric_limits<std::uint32_t>::max()
                && 节点.版本号 + 1 == 条目.记录.版本号;
            结果.状态 = 已完成同次删除
                ? 节点直接身份写入状态::幂等读回
                : 节点直接身份写入状态::版本漂移;
            return 结果;
        }
        if (条目.记录.状态 == 记录状态::已删除) {
            结果.状态 = 节点直接身份写入状态::幂等读回;
            return 结果;
        }
        if (条目.记录.状态 != 记录状态::有效
            || 条目.记录.版本号 == std::numeric_limits<std::uint32_t>::max()
            || (条目.待删除事务序号 != 0 && 条目.待删除事务序号 != 事务序号)) {
            return 结果;
        }
        if (条目.待删除事务序号 == 事务序号) {
            结果.状态 = 节点直接身份写入状态::幂等读回;
            return 结果;
        }
        条目.待删除事务序号 = 事务序号;
        条目.删除已确认 = false;
        const 节点句柄 写后{仓库编号_, 节点.节点编号, 节点.版本号 + 1};
        结果.状态 = 节点直接身份写入状态::已形成候选;
        结果.候选.emplace(节点直接身份删除未发布候选{
            this, 事务序号, 节点, 写后});
        return 结果;
    }

    节点直接身份操作结果 确认节点删除候选(
        节点直接身份删除未发布候选& 候选,
        std::uint64_t 事务序号) {
        节点直接身份操作结果 结果{节点直接身份写入状态::入口拒绝, 候选.读取写前节点()};
        if (!删除候选属于本次事务(候选, 事务序号)) {
            结果.状态 = 节点直接身份写入状态::事务拒绝;
            return 结果;
        }
        if (候选.阶段_ == 节点直接身份删除未发布候选::候选阶段::已确认待发布) {
            结果.状态 = 节点直接身份写入状态::已确认待发布;
            return 结果;
        }
        if (候选.阶段_ != 节点直接身份删除未发布候选::候选阶段::未发布) {
            return 结果;
        }
        std::unique_lock<std::shared_mutex> 锁(仓库锁_);
        const auto 位置 = 节点表_.find(候选.写前节点_.节点编号);
        if (!删除候选条目匹配(位置, 候选)) {
            结果.状态 = 节点直接身份写入状态::内部不一致;
            return 结果;
        }
        位置->second.删除已确认 = true;
        候选.阶段_ = 节点直接身份删除未发布候选::候选阶段::已确认待发布;
        结果.状态 = 节点直接身份写入状态::已确认待发布;
        return 结果;
    }

    节点直接身份操作结果 撤销节点删除候选(
        节点直接身份删除未发布候选& 候选,
        std::uint64_t 事务序号) {
        节点直接身份操作结果 结果{节点直接身份写入状态::入口拒绝, 候选.读取写前节点()};
        if (!删除候选属于本次事务(候选, 事务序号)) {
            结果.状态 = 节点直接身份写入状态::事务拒绝;
            return 结果;
        }
        if (候选.阶段_ == 节点直接身份删除未发布候选::候选阶段::已撤销) {
            结果.状态 = 节点直接身份写入状态::已撤销;
            return 结果;
        }
        if (候选.阶段_ != 节点直接身份删除未发布候选::候选阶段::未发布
            && 候选.阶段_ != 节点直接身份删除未发布候选::候选阶段::已确认待发布) {
            return 结果;
        }
        std::unique_lock<std::shared_mutex> 锁(仓库锁_);
        const auto 位置 = 节点表_.find(候选.写前节点_.节点编号);
        if (!删除候选条目匹配(位置, 候选)) {
            结果.状态 = 节点直接身份写入状态::内部不一致;
            return 结果;
        }
        位置->second.待删除事务序号 = 0;
        位置->second.删除已确认 = false;
        候选.阶段_ = 节点直接身份删除未发布候选::候选阶段::已撤销;
        结果.状态 = 节点直接身份写入状态::已撤销;
        return 结果;
    }

    void 完成发布节点删除候选(
        节点直接身份删除未发布候选& 候选,
        std::uint64_t 事务序号) noexcept {
        if (!删除候选属于本次事务(候选, 事务序号)
            || 候选.阶段_ != 节点直接身份删除未发布候选::候选阶段::已确认待发布) {
            std::terminate();
        }
        std::unique_lock<std::shared_mutex> 锁(仓库锁_);
        const auto 位置 = 节点表_.find(候选.写前节点_.节点编号);
        if (!删除候选条目匹配(位置, 候选) || !位置->second.删除已确认) {
            std::terminate();
        }
        位置->second.记录.状态 = 记录状态::已删除;
        ++位置->second.记录.版本号;
        位置->second.待删除事务序号 = 0;
        位置->second.删除已确认 = false;
        候选.阶段_ = 节点直接身份删除未发布候选::候选阶段::已发布;
    }

    std::optional<节点直接身份记录> 读取节点(节点句柄 节点) const {
        std::shared_lock<std::shared_mutex> 锁(仓库锁_);
        const auto 位置 = 节点表_.find(节点.节点编号);
        if (位置 == 节点表_.end() || !位置->second.已发布
            || !完整句柄匹配(节点, 位置->second.记录)
            || 位置->second.记录.状态 != 记录状态::有效) {
            return std::nullopt;
        }
        return 位置->second.记录;
    }

    std::optional<节点直接身份记录> 读取节点(
        节点句柄 节点,
        std::uint64_t 事务序号) const {
        if (事务序号 == 0) return std::nullopt;
        std::shared_lock<std::shared_mutex> 锁(仓库锁_);
        const auto 位置 = 节点表_.find(节点.节点编号);
        if (位置 == 节点表_.end() || !完整句柄匹配(节点, 位置->second.记录)
            || 位置->second.记录.状态 != 记录状态::有效) {
            return std::nullopt;
        }
        if (!位置->second.已发布 && 位置->second.创建事务序号 != 事务序号) {
            return std::nullopt;
        }
        return 位置->second.记录;
    }

    std::optional<节点直接身份记录> 读取节点审计(节点句柄 节点) const {
        return 读取节点审计实现(节点, 0, false);
    }

    std::optional<节点直接身份记录> 读取节点审计(
        节点句柄 节点,
        std::uint64_t 事务序号) const {
        if (事务序号 == 0) return std::nullopt;
        return 读取节点审计实现(节点, 事务序号, true);
    }

    std::optional<节点稳定主键> 读取节点稳定主键(节点句柄 节点) const {
        const auto 记录 = 读取节点(节点);
        return 记录.has_value()
            ? std::optional<节点稳定主键>{记录->稳定主键}
            : std::nullopt;
    }

    std::optional<节点稳定主键> 读取节点稳定主键(
        节点句柄 节点,
        std::uint64_t 事务序号) const {
        const auto 记录 = 读取节点(节点, 事务序号);
        return 记录.has_value()
            ? std::optional<节点稳定主键>{记录->稳定主键}
            : std::nullopt;
    }

    稳定主键当前身份读取结果 读取稳定主键当前身份(节点稳定主键 稳定主键) const {
        return 读取稳定主键当前身份实现(稳定主键, 0, false);
    }

    稳定主键当前身份读取结果 读取稳定主键当前身份(
        节点稳定主键 稳定主键,
        std::uint64_t 事务序号) const {
        if (事务序号 == 0) return {};
        return 读取稳定主键当前身份实现(稳定主键, 事务序号, true);
    }

    bool 节点是否有效(节点句柄 节点) const {
        return 读取节点(节点).has_value();
    }

    bool 节点是否有效(节点句柄 节点, std::uint64_t 事务序号) const {
        return 读取节点(节点, 事务序号).has_value();
    }

    bool 节点是本事务候选(节点句柄 节点, std::uint64_t 事务序号) const {
        if (事务序号 == 0) return false;
        std::shared_lock<std::shared_mutex> 锁(仓库锁_);
        const auto 位置 = 节点表_.find(节点.节点编号);
        return 位置 != 节点表_.end() && !位置->second.已发布
            && 位置->second.创建事务序号 == 事务序号
            && 完整句柄匹配(节点, 位置->second.记录)
            && 位置->second.记录.状态 == 记录状态::有效;
    }

    std::uint64_t 有效节点数量() const {
        std::shared_lock<std::shared_mutex> 锁(仓库锁_);
        std::uint64_t 数量 = 0;
        for (const auto& [节点编号, 条目] : 节点表_) {
            (void)节点编号;
            if (条目.已发布 && 条目.记录.状态 == 记录状态::有效) ++数量;
        }
        return 数量;
    }

    std::uint64_t 仓库编号() const noexcept { return 仓库编号_; }

    std::uint64_t 读取命名域高水位(节点稳定主键命名域 命名域) const {
        std::shared_lock<std::shared_mutex> 锁(仓库锁_);
        const auto 位置 = 每域高水位_.find(static_cast<std::uint64_t>(命名域));
        return 位置 == 每域高水位_.end() ? 0 : 位置->second;
    }

    节点直接身份仓库权威材料 导出权威状态() const {
        std::shared_lock<std::shared_mutex> 锁(仓库锁_);
        节点直接身份仓库权威材料 材料;
        材料.仓库编号 = 仓库编号_;
        材料.下个节点编号 = 下个节点编号_;
        材料.下个创建序号 = 下个创建序号_;
        材料.每域高水位.reserve(每域高水位_.size());
        材料.历史占用.reserve(主键占用表_.size());
        材料.记录组.reserve(节点表_.size());
        for (const auto& [命名域, 键值] : 每域高水位_) {
            材料.每域高水位.push_back({命名域, 键值});
        }
        for (const auto& [稳定主键, 占用] : 主键占用表_) {
            材料.历史占用.push_back({稳定主键, 占用.类型, 占用.节点编号, 占用.曾发布});
        }
        for (const auto& [节点编号, 条目] : 节点表_) {
            (void)节点编号;
            if (条目.已发布) 材料.记录组.push_back(条目.记录);
        }
        std::sort(材料.每域高水位.begin(), 材料.每域高水位.end(),
            [](const 节点稳定主键高水位& 左, const 节点稳定主键高水位& 右) {
                return 左.命名域 < 右.命名域;
            });
        std::sort(材料.历史占用.begin(), 材料.历史占用.end(),
            [](const 节点稳定主键历史占用& 左, const 节点稳定主键历史占用& 右) {
                if (左.稳定主键.命名域 != 右.稳定主键.命名域) {
                    return 左.稳定主键.命名域 < 右.稳定主键.命名域;
                }
                return 左.稳定主键.键值 < 右.稳定主键.键值;
            });
        std::sort(材料.记录组.begin(), 材料.记录组.end(),
            [](const 节点直接身份记录& 左, const 节点直接身份记录& 右) {
                return 左.节点编号 < 右.节点编号;
            });
        return 材料;
    }

private:
    struct 稳定主键哈希 {
        std::size_t operator()(const 节点稳定主键& 主键) const noexcept {
            const auto 左 = static_cast<std::size_t>(主键.命名域 ^ (主键.命名域 >> 32));
            const auto 右 = static_cast<std::size_t>(主键.键值 ^ (主键.键值 >> 32));
            return 左 ^ (右 + static_cast<std::size_t>(0x9E3779B9U) + (左 << 6) + (左 >> 2));
        }
    };

    struct 主键占用条目 {
        节点类型 类型 = 节点类型::未分类;
        std::uint64_t 节点编号 = 0;
        bool 曾发布 = false;
    };

    struct 节点内部条目 {
        节点直接身份记录 记录;
        bool 已发布 = false;
        std::uint64_t 创建事务序号 = 0;
        std::uint64_t 待删除事务序号 = 0;
        bool 删除已确认 = false;
    };

    using 节点表位置 = std::unordered_map<std::uint64_t, 节点内部条目>::const_iterator;

    节点句柄 形成句柄(const 节点直接身份记录& 记录) const noexcept {
        return {仓库编号_, 记录.节点编号, 记录.版本号};
    }

    bool 完整句柄匹配(节点句柄 节点, const 节点直接身份记录& 记录) const noexcept {
        return 节点.仓库编号 == 仓库编号_
            && 节点.节点编号 == 记录.节点编号
            && 节点.版本号 == 记录.版本号;
    }

    bool 候选属于本次事务(
        const 节点直接身份未发布候选& 候选,
        std::uint64_t 事务序号) const noexcept {
        return 事务序号 != 0 && 候选.仓库_ == this
            && 候选.事务序号_ == 事务序号 && 候选.完整();
    }

    bool 删除候选属于本次事务(
        const 节点直接身份删除未发布候选& 候选,
        std::uint64_t 事务序号) const noexcept {
        return 事务序号 != 0 && 候选.仓库_ == this
            && 候选.事务序号_ == 事务序号 && 候选.完整();
    }

    bool 候选条目匹配(
        节点表位置 位置,
        const 节点直接身份未发布候选& 候选) const noexcept {
        return 位置 != 节点表_.end() && !位置->second.已发布
            && 位置->second.创建事务序号 == 候选.事务序号_
            && 完整句柄匹配(候选.节点_, 位置->second.记录)
            && 位置->second.记录.稳定主键 == 候选.稳定主键_
            && 位置->second.记录.状态 == 记录状态::有效;
    }

    bool 删除候选条目匹配(
        节点表位置 位置,
        const 节点直接身份删除未发布候选& 候选) const noexcept {
        return 位置 != 节点表_.end() && 位置->second.已发布
            && 位置->second.待删除事务序号 == 候选.事务序号_
            && 完整句柄匹配(候选.写前节点_, 位置->second.记录)
            && 位置->second.记录.状态 == 记录状态::有效;
    }

    std::optional<节点直接身份记录> 读取节点审计实现(
        节点句柄 节点,
        std::uint64_t 事务序号,
        bool 允许本事务候选) const {
        std::shared_lock<std::shared_mutex> 锁(仓库锁_);
        const auto 位置 = 节点表_.find(节点.节点编号);
        if (位置 == 节点表_.end() || 节点.仓库编号 != 仓库编号_) return std::nullopt;
        if (!位置->second.已发布
            && (!允许本事务候选 || 位置->second.创建事务序号 != 事务序号)) {
            return std::nullopt;
        }
        const auto& 记录 = 位置->second.记录;
        const bool 版本匹配 = 节点.版本号 == 记录.版本号
            || (记录.状态 == 记录状态::已删除
                && 节点.版本号 != std::numeric_limits<std::uint32_t>::max()
                && 节点.版本号 + 1 == 记录.版本号);
        return 版本匹配 ? std::optional<节点直接身份记录>{记录} : std::nullopt;
    }

    稳定主键当前身份读取结果 读取稳定主键当前身份实现(
        节点稳定主键 稳定主键,
        std::uint64_t 事务序号,
        bool 允许本事务候选) const {
        稳定主键当前身份读取结果 结果;
        if (!稳定主键有效(稳定主键)
            || !命名域已定义(static_cast<节点稳定主键命名域>(稳定主键.命名域))) {
            return 结果;
        }
        std::shared_lock<std::shared_mutex> 锁(仓库锁_);
        const auto 占用位置 = 主键占用表_.find(稳定主键);
        if (占用位置 == 主键占用表_.end()) {
            结果.状态 = 稳定主键当前身份状态::未占用;
            return 结果;
        }
        const auto 记录位置 = 节点表_.find(占用位置->second.节点编号);
        if (记录位置 == 节点表_.end()) {
            结果.状态 = 稳定主键当前身份状态::历史占用;
            return 结果;
        }
        const auto& 条目 = 记录位置->second;
        if (!条目.已发布
            && (!允许本事务候选 || 条目.创建事务序号 != 事务序号)) {
            结果.状态 = 稳定主键当前身份状态::未占用;
            return 结果;
        }
        if (条目.记录.稳定主键 != 稳定主键
            || 条目.记录.节点编号 != 占用位置->second.节点编号) {
            结果.状态 = 稳定主键当前身份状态::内部不一致;
            return 结果;
        }
        if (条目.记录.状态 == 记录状态::有效) {
            结果.状态 = 稳定主键当前身份状态::当前有效;
            结果.当前身份 = 形成句柄(条目.记录);
            结果.当前记录 = 条目.记录;
            return 结果;
        }
        结果.状态 = 稳定主键当前身份状态::历史占用;
        return 结果;
    }

    std::uint64_t 仓库编号_ = 0;
    std::uint64_t 下个节点编号_ = 1;
    std::uint64_t 下个创建序号_ = 1;
    mutable std::shared_mutex 仓库锁_;
    std::unordered_map<std::uint64_t, 节点内部条目> 节点表_;
    std::unordered_map<节点稳定主键, 主键占用条目, 稳定主键哈希> 主键占用表_;
    std::unordered_map<std::uint64_t, std::uint64_t> 每域高水位_;
};

}
