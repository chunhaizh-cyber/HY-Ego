// 文件规则：运行期上下文只拥有同域结构核心与发布生命周期骨架；缺少领域服务装配时不得生产发布。
module;

#include "核心/主信息仓库.h"
#include "核心/节点仓库.h"
#include "核心/关系仓库.h"
#include "核心/索引仓库.h"

#include <cstdint>
#include <memory>
#include <mutex>
#include <optional>

export module 海中鱼巣.启动.运行期上下文;

import 海中鱼巣.核心.协调.结构事务;

export namespace 海中鱼巣 {

enum class 运行期上下文发布状态 : std::uint32_t {
    未定义 = 0,
    已发布 = 1,
    无效候选 = 2,
    结构核心不完整 = 3,
    缺少领域服务装配 = 4,
    已有当前上下文 = 5
};

class 运行期上下文 {
public:
    explicit 运行期上下文(std::uint64_t 域编号, std::uint64_t 仓库编号 = 1)
        : 协调器_(域编号),
          接线_(协调器_.生成接线()),
          主信息_(仓库编号, 接线_),
          节点_(主信息_, 仓库编号, 接线_),
          关系_(节点_, 仓库编号, 接线_),
          索引_(节点_, 接线_),
          仓库编号_(仓库编号) {
    }

    运行期上下文(const 运行期上下文&) = delete;
    运行期上下文& operator=(const 运行期上下文&) = delete;

    bool 结构核心完整() const {
        if (仓库编号_ == 0 || !接线_.已接域()
            || 协调器_.读取域编号() != 接线_.域编号
            || 节点_.仓库编号() != 仓库编号_) {
            return false;
        }
        auto 许可 = 接线_.取得共享许可(接线_.运行期状态);
        return 许可.有效()
            && 接线_.验证共享路径令牌(接线_.运行期状态, 许可.读取令牌())
            && 主信息_.读取主信息({}, 许可.读取令牌()) == std::nullopt
            && 节点_.读取节点({}, 许可.读取令牌()) == std::nullopt
            && 关系_.读取关系({}, 许可.读取令牌()) == std::nullopt
            && 索引_.按主键查节点(0, 许可.读取令牌()) == std::nullopt;
    }

    bool 完整() const {
        return false;
    }

    const 结构事务接线& 读取接线() const { return 接线_; }
    主信息仓库& 读取主信息仓库() { return 主信息_; }
    节点仓库& 读取节点仓库() { return 节点_; }
    关系仓库& 读取关系仓库() { return 关系_; }
    索引仓库& 读取索引仓库() { return 索引_; }
    const 主信息仓库& 读取主信息仓库() const { return 主信息_; }
    const 节点仓库& 读取节点仓库() const { return 节点_; }
    const 关系仓库& 读取关系仓库() const { return 关系_; }
    const 索引仓库& 读取索引仓库() const { return 索引_; }

private:
    结构事务协调器 协调器_;
    结构事务接线 接线_;
    主信息仓库 主信息_;
    节点仓库 节点_;
    关系仓库 关系_;
    索引仓库 索引_;
    std::uint64_t 仓库编号_ = 0;
};

class 运行期上下文租约 {
public:
    运行期上下文租约() = default;
    bool 有效() const { return 上下文_ && 上下文_->完整(); }
    const 运行期上下文* 读取() const { return 有效() ? 上下文_.get() : nullptr; }

private:
    friend class 运行期上下文宿主;
    explicit 运行期上下文租约(std::shared_ptr<const 运行期上下文> 上下文)
        : 上下文_(std::move(上下文)) {
    }
    std::shared_ptr<const 运行期上下文> 上下文_;
};

class 运行期上下文宿主 {
public:
    运行期上下文发布状态 尝试发布(std::shared_ptr<运行期上下文> 候选) {
        if (!候选) {
            return 运行期上下文发布状态::无效候选;
        }
        if (!候选->结构核心完整()) {
            return 运行期上下文发布状态::结构核心不完整;
        }
        if (!候选->完整()) {
            return 运行期上下文发布状态::缺少领域服务装配;
        }
        std::lock_guard<std::mutex> 锁(发布锁_);
        if (当前上下文_) {
            return 运行期上下文发布状态::已有当前上下文;
        }
        return 运行期上下文发布状态::缺少领域服务装配;
    }

    运行期上下文租约 读取租约() const {
        std::lock_guard<std::mutex> 锁(发布锁_);
        return 运行期上下文租约{当前上下文_};
    }

    bool 为空() const {
        std::lock_guard<std::mutex> 锁(发布锁_);
        return !当前上下文_;
    }

private:
    mutable std::mutex 发布锁_;
    std::shared_ptr<运行期上下文> 当前上下文_;
};

}
