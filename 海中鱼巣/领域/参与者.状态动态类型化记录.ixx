// 文件规则：本模块唯一承载状态/动态双记录仓、创建与退役参与者、值式访问器和一次性退役包。
module;

#include "../核心/句柄.h"
#include "节点直接领域合同.数据.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <functional>
#include <limits>
#include <memory>
#include <mutex>
#include <new>
#include <optional>
#include <shared_mutex>
#include <utility>
#include <vector>

export module 海中鱼巣.领域.参与者.状态动态类型化记录;

export import 海中鱼巣.核心.执行器.节点直接身份结构写入;

export {
#include "状态动态类型化记录.数据.h"
}

export namespace 海中鱼巣 {

enum class 状态动态记录读取状态 : std::uint32_t {
    合法未找到 = 1,
    可读 = 2,
    许可竞争 = 3,
    内部不一致 = 0x8000
};

template <typename 记录类型>
struct 状态动态记录读取结果 final {
    状态动态记录读取状态 状态 = 状态动态记录读取状态::合法未找到;
    std::optional<记录类型> 记录;

    bool 可读() const noexcept {
        return 状态 == 状态动态记录读取状态::可读 && 记录.has_value();
    }
};

enum class 状态动态记录全量读取状态 : std::uint32_t {
    已形成 = 1,
    许可竞争 = 2,
    资源失败 = 3,
    内部不一致 = 0x8000
};

template <typename 记录类型>
struct 状态动态记录全量读取结果 final {
    状态动态记录全量读取状态 状态 =
        状态动态记录全量读取状态::内部不一致;
    std::uint64_t 结构版本 = 0;
    std::uint64_t 记录数量 = 0;
    std::vector<记录类型> 记录组;
};

class 状态类型化记录事务参与者;
class 动态类型化记录事务参与者;
class 状态记录退役事务参与者;
class 动态记录退役事务参与者;
class 状态类型化记录只读访问器;
class 动态类型化记录只读访问器;

class 状态类型化记录仓 final {
public:
    状态类型化记录仓() = default;
    状态类型化记录仓(const 状态类型化记录仓&) = delete;
    状态类型化记录仓& operator=(const 状态类型化记录仓&) = delete;
    状态类型化记录仓(状态类型化记录仓&&) = delete;
    状态类型化记录仓& operator=(状态类型化记录仓&&) = delete;

private:
    friend class 状态类型化记录事务参与者;
    friend class 状态记录退役事务参与者;
    friend class 状态类型化记录只读访问器;

    struct 内部记录 final {
        状态域记录 记录;
        bool 已发布 = false;
    };

    struct 当前指针 final {
        节点句柄 所属状态;
        std::uint64_t 记录版本 = 0;
        bool 已发布 = false;
    };

    mutable std::shared_mutex 锁_;
    std::vector<内部记录> 记录组_;
    std::vector<当前指针> 当前指针组_;
    std::uint64_t 结构版本_ = 1;
};

class 动态类型化记录仓 final {
public:
    动态类型化记录仓() = default;
    动态类型化记录仓(const 动态类型化记录仓&) = delete;
    动态类型化记录仓& operator=(const 动态类型化记录仓&) = delete;
    动态类型化记录仓(动态类型化记录仓&&) = delete;
    动态类型化记录仓& operator=(动态类型化记录仓&&) = delete;

private:
    friend class 动态类型化记录事务参与者;
    friend class 动态记录退役事务参与者;
    friend class 动态类型化记录只读访问器;

    struct 内部记录 final {
        动态域记录 记录;
        bool 已发布 = false;
    };

    struct 当前指针 final {
        节点句柄 所属动态;
        std::uint64_t 记录版本 = 0;
        bool 已发布 = false;
    };

    mutable std::shared_mutex 锁_;
    std::vector<内部记录> 记录组_;
    std::vector<当前指针> 当前指针组_;
    std::uint64_t 结构版本_ = 1;
};

class 状态类型化记录只读访问器 final {
public:
    explicit 状态类型化记录只读访问器(
        const 状态类型化记录仓& 记录仓) noexcept : 记录仓_(&记录仓) {
    }

    状态动态记录读取结果<状态域记录> 读取当前记录(
        节点句柄 状态) const {
        状态动态记录读取结果<状态域记录> 输出;
        if (记录仓_ == nullptr || !句柄有效(状态)) {
            输出.状态 = 状态动态记录读取状态::内部不一致;
            return 输出;
        }
        std::shared_lock<std::shared_mutex> 锁(记录仓_->锁_, std::try_to_lock);
        if (!锁.owns_lock()) {
            输出.状态 = 状态动态记录读取状态::许可竞争;
            return 输出;
        }
        const auto 指针数量 = 当前指针数量_已加锁(状态);
        if (指针数量 == 0) return 输出;
        if (指针数量 != 1) {
            输出.状态 = 状态动态记录读取状态::内部不一致;
            return 输出;
        }
        const auto 指针 = 查找当前指针_已加锁(状态);
        if (指针 == nullptr
            || 记录数量_已加锁(状态, 指针->记录版本) != 1) {
            输出.状态 = 状态动态记录读取状态::内部不一致;
            return 输出;
        }
        const auto 记录 = 查找记录_已加锁(状态, 指针->记录版本);
        if (记录 == nullptr || !记录->记录.完整()) {
            输出.状态 = 状态动态记录读取状态::内部不一致;
            return 输出;
        }
        if (记录->记录.状态 == 记录状态::已失效) {
            输出.状态 = 状态动态记录读取状态::内部不一致;
            return 输出;
        }
        if (记录->记录.状态 != 记录状态::有效) return 输出;
        try {
            输出.记录 = 记录->记录;
        } catch (...) {
            输出.状态 = 状态动态记录读取状态::内部不一致;
            return 输出;
        }
        输出.状态 = 状态动态记录读取状态::可读;
        return 输出;
    }

    状态动态记录读取结果<状态域记录> 读取审计记录(
        节点句柄 状态,
        std::uint64_t 记录版本) const {
        状态动态记录读取结果<状态域记录> 输出;
        if (记录仓_ == nullptr || !句柄有效(状态) || 记录版本 == 0) {
            输出.状态 = 状态动态记录读取状态::内部不一致;
            return 输出;
        }
        std::shared_lock<std::shared_mutex> 锁(记录仓_->锁_, std::try_to_lock);
        if (!锁.owns_lock()) {
            输出.状态 = 状态动态记录读取状态::许可竞争;
            return 输出;
        }
        const auto 记录数量 = 记录数量_已加锁(状态, 记录版本);
        if (记录数量 == 0) return 输出;
        if (记录数量 != 1) {
            输出.状态 = 状态动态记录读取状态::内部不一致;
            return 输出;
        }
        const auto 记录 = 查找记录_已加锁(状态, 记录版本);
        if (记录 == nullptr) {
            输出.状态 = 状态动态记录读取状态::内部不一致;
            return 输出;
        }
        if (!记录->记录.完整()) {
            输出.状态 = 状态动态记录读取状态::内部不一致;
            return 输出;
        }
        try {
            输出.记录 = 记录->记录;
        } catch (...) {
            输出.状态 = 状态动态记录读取状态::内部不一致;
            return 输出;
        }
        输出.状态 = 状态动态记录读取状态::可读;
        return 输出;
    }

    std::uint64_t 读取结构版本() const noexcept {
        if (记录仓_ == nullptr) return 0;
        std::shared_lock<std::shared_mutex> 锁(记录仓_->锁_, std::try_to_lock);
        return 锁.owns_lock() ? 记录仓_->结构版本_ : 0;
    }

    状态动态记录全量读取结果<状态域记录>
    复制全部已发布记录() const {
        状态动态记录全量读取结果<状态域记录> 输出;
        if (记录仓_ == nullptr) return 输出;
        std::shared_lock<std::shared_mutex> 锁(记录仓_->锁_, std::try_to_lock);
        if (!锁.owns_lock()) {
            输出.状态 = 状态动态记录全量读取状态::许可竞争;
            return 输出;
        }
        if (!仓完整_已加锁()) return 输出;
        try {
            输出.记录组.reserve(记录仓_->记录组_.size());
            for (const auto& 项目 : 记录仓_->记录组_) {
                if (项目.已发布) 输出.记录组.push_back(项目.记录);
            }
        } catch (const std::bad_alloc&) {
            输出.记录组.clear();
            输出.状态 = 状态动态记录全量读取状态::资源失败;
            return 输出;
        } catch (...) {
            输出.记录组.clear();
            return 输出;
        }
        输出.结构版本 = 记录仓_->结构版本_;
        输出.记录数量 = static_cast<std::uint64_t>(输出.记录组.size());
        输出.状态 = 状态动态记录全量读取状态::已形成;
        return 输出;
    }

private:
    std::size_t 当前指针数量_已加锁(节点句柄 状态) const noexcept {
        std::size_t 数量 = 0;
        for (const auto& 指针 : 记录仓_->当前指针组_) {
            if (指针.已发布 && 指针.所属状态 == 状态) ++数量;
        }
        return 数量;
    }

    std::size_t 记录数量_已加锁(
        节点句柄 状态,
        std::uint64_t 记录版本) const noexcept {
        std::size_t 数量 = 0;
        for (const auto& 项目 : 记录仓_->记录组_) {
            if (项目.已发布 && 项目.记录.所属状态 == 状态
                && 项目.记录.记录版本 == 记录版本) {
                ++数量;
            }
        }
        return 数量;
    }

    const 状态类型化记录仓::当前指针* 查找当前指针_已加锁(
        节点句柄 状态) const noexcept {
        const 状态类型化记录仓::当前指针* 输出 = nullptr;
        for (const auto& 指针 : 记录仓_->当前指针组_) {
            if (!指针.已发布 || 指针.所属状态 != 状态) continue;
            if (输出 != nullptr) return nullptr;
            输出 = &指针;
        }
        return 输出;
    }

    const 状态类型化记录仓::内部记录* 查找记录_已加锁(
        节点句柄 状态,
        std::uint64_t 记录版本) const noexcept {
        const 状态类型化记录仓::内部记录* 输出 = nullptr;
        for (const auto& 项目 : 记录仓_->记录组_) {
            if (!项目.已发布 || 项目.记录.所属状态 != 状态
                || 项目.记录.记录版本 != 记录版本) {
                continue;
            }
            if (输出 != nullptr) return nullptr;
            输出 = &项目;
        }
        return 输出;
    }

    bool 仓完整_已加锁() const noexcept {
        if (记录仓_->结构版本_ == 0) return false;
        for (const auto& 项目 : 记录仓_->记录组_) {
            if (!项目.已发布 || !项目.记录.完整()) return false;
            std::size_t 相同身份数量 = 0;
            for (const auto& 其它 : 记录仓_->记录组_) {
                if (其它.已发布 && 其它.记录.所属状态 == 项目.记录.所属状态
                    && 其它.记录.记录版本 == 项目.记录.记录版本) {
                    ++相同身份数量;
                }
            }
            if (相同身份数量 != 1) return false;
        }
        for (const auto& 指针 : 记录仓_->当前指针组_) {
            if (!指针.已发布 || 指针.记录版本 == 0) return false;
            const auto 当前 = 查找记录_已加锁(指针.所属状态, 指针.记录版本);
            if (当前 == nullptr || 当前->记录.状态 == 记录状态::已失效) {
                return false;
            }
            for (std::uint64_t 版本 = 1;; ++版本) {
                if (查找记录_已加锁(指针.所属状态, 版本) == nullptr) return false;
                if (版本 == 指针.记录版本) break;
            }
        }
        for (const auto& 项目 : 记录仓_->记录组_) {
            if (!项目.已发布) continue;
            const auto 指针 = 查找当前指针_已加锁(项目.记录.所属状态);
            if (指针 == nullptr || 指针->记录版本 < 项目.记录.记录版本) return false;
        }
        return true;
    }

    const 状态类型化记录仓* 记录仓_ = nullptr;
};

class 动态类型化记录只读访问器 final {
public:
    explicit 动态类型化记录只读访问器(
        const 动态类型化记录仓& 记录仓) noexcept : 记录仓_(&记录仓) {
    }

    状态动态记录读取结果<动态域记录> 读取当前记录(
        节点句柄 动态) const {
        状态动态记录读取结果<动态域记录> 输出;
        if (记录仓_ == nullptr || !句柄有效(动态)) {
            输出.状态 = 状态动态记录读取状态::内部不一致;
            return 输出;
        }
        std::shared_lock<std::shared_mutex> 锁(记录仓_->锁_, std::try_to_lock);
        if (!锁.owns_lock()) {
            输出.状态 = 状态动态记录读取状态::许可竞争;
            return 输出;
        }
        const auto 指针数量 = 当前指针数量_已加锁(动态);
        if (指针数量 == 0) return 输出;
        if (指针数量 != 1) {
            输出.状态 = 状态动态记录读取状态::内部不一致;
            return 输出;
        }
        const auto 指针 = 查找当前指针_已加锁(动态);
        if (指针 == nullptr
            || 记录数量_已加锁(动态, 指针->记录版本) != 1) {
            输出.状态 = 状态动态记录读取状态::内部不一致;
            return 输出;
        }
        const auto 记录 = 查找记录_已加锁(动态, 指针->记录版本);
        if (记录 == nullptr || !记录->记录.完整()) {
            输出.状态 = 状态动态记录读取状态::内部不一致;
            return 输出;
        }
        if (记录->记录.状态 == 记录状态::已失效) {
            输出.状态 = 状态动态记录读取状态::内部不一致;
            return 输出;
        }
        if (记录->记录.状态 != 记录状态::有效) return 输出;
        try {
            输出.记录 = 记录->记录;
        } catch (...) {
            输出.状态 = 状态动态记录读取状态::内部不一致;
            return 输出;
        }
        输出.状态 = 状态动态记录读取状态::可读;
        return 输出;
    }

    状态动态记录读取结果<动态域记录> 读取审计记录(
        节点句柄 动态,
        std::uint64_t 记录版本) const {
        状态动态记录读取结果<动态域记录> 输出;
        if (记录仓_ == nullptr || !句柄有效(动态) || 记录版本 == 0) {
            输出.状态 = 状态动态记录读取状态::内部不一致;
            return 输出;
        }
        std::shared_lock<std::shared_mutex> 锁(记录仓_->锁_, std::try_to_lock);
        if (!锁.owns_lock()) {
            输出.状态 = 状态动态记录读取状态::许可竞争;
            return 输出;
        }
        const auto 记录数量 = 记录数量_已加锁(动态, 记录版本);
        if (记录数量 == 0) return 输出;
        if (记录数量 != 1) {
            输出.状态 = 状态动态记录读取状态::内部不一致;
            return 输出;
        }
        const auto 记录 = 查找记录_已加锁(动态, 记录版本);
        if (记录 == nullptr) {
            输出.状态 = 状态动态记录读取状态::内部不一致;
            return 输出;
        }
        if (!记录->记录.完整()) {
            输出.状态 = 状态动态记录读取状态::内部不一致;
            return 输出;
        }
        try {
            输出.记录 = 记录->记录;
        } catch (...) {
            输出.状态 = 状态动态记录读取状态::内部不一致;
            return 输出;
        }
        输出.状态 = 状态动态记录读取状态::可读;
        return 输出;
    }

    std::uint64_t 读取结构版本() const noexcept {
        if (记录仓_ == nullptr) return 0;
        std::shared_lock<std::shared_mutex> 锁(记录仓_->锁_, std::try_to_lock);
        return 锁.owns_lock() ? 记录仓_->结构版本_ : 0;
    }

    状态动态记录全量读取结果<动态域记录>
    复制全部已发布记录() const {
        状态动态记录全量读取结果<动态域记录> 输出;
        if (记录仓_ == nullptr) return 输出;
        std::shared_lock<std::shared_mutex> 锁(记录仓_->锁_, std::try_to_lock);
        if (!锁.owns_lock()) {
            输出.状态 = 状态动态记录全量读取状态::许可竞争;
            return 输出;
        }
        if (!仓完整_已加锁()) return 输出;
        try {
            输出.记录组.reserve(记录仓_->记录组_.size());
            for (const auto& 项目 : 记录仓_->记录组_) {
                if (项目.已发布) 输出.记录组.push_back(项目.记录);
            }
        } catch (const std::bad_alloc&) {
            输出.记录组.clear();
            输出.状态 = 状态动态记录全量读取状态::资源失败;
            return 输出;
        } catch (...) {
            输出.记录组.clear();
            return 输出;
        }
        输出.结构版本 = 记录仓_->结构版本_;
        输出.记录数量 = static_cast<std::uint64_t>(输出.记录组.size());
        输出.状态 = 状态动态记录全量读取状态::已形成;
        return 输出;
    }

private:
    std::size_t 当前指针数量_已加锁(节点句柄 动态) const noexcept {
        std::size_t 数量 = 0;
        for (const auto& 指针 : 记录仓_->当前指针组_) {
            if (指针.已发布 && 指针.所属动态 == 动态) ++数量;
        }
        return 数量;
    }

    std::size_t 记录数量_已加锁(
        节点句柄 动态,
        std::uint64_t 记录版本) const noexcept {
        std::size_t 数量 = 0;
        for (const auto& 项目 : 记录仓_->记录组_) {
            if (项目.已发布 && 项目.记录.所属动态 == 动态
                && 项目.记录.记录版本 == 记录版本) {
                ++数量;
            }
        }
        return 数量;
    }

    const 动态类型化记录仓::当前指针* 查找当前指针_已加锁(
        节点句柄 动态) const noexcept {
        const 动态类型化记录仓::当前指针* 输出 = nullptr;
        for (const auto& 指针 : 记录仓_->当前指针组_) {
            if (!指针.已发布 || 指针.所属动态 != 动态) continue;
            if (输出 != nullptr) return nullptr;
            输出 = &指针;
        }
        return 输出;
    }

    const 动态类型化记录仓::内部记录* 查找记录_已加锁(
        节点句柄 动态,
        std::uint64_t 记录版本) const noexcept {
        const 动态类型化记录仓::内部记录* 输出 = nullptr;
        for (const auto& 项目 : 记录仓_->记录组_) {
            if (!项目.已发布 || 项目.记录.所属动态 != 动态
                || 项目.记录.记录版本 != 记录版本) {
                continue;
            }
            if (输出 != nullptr) return nullptr;
            输出 = &项目;
        }
        return 输出;
    }

    bool 仓完整_已加锁() const noexcept {
        if (记录仓_->结构版本_ == 0) return false;
        for (const auto& 项目 : 记录仓_->记录组_) {
            if (!项目.已发布 || !项目.记录.完整()) return false;
            std::size_t 相同身份数量 = 0;
            for (const auto& 其它 : 记录仓_->记录组_) {
                if (其它.已发布 && 其它.记录.所属动态 == 项目.记录.所属动态
                    && 其它.记录.记录版本 == 项目.记录.记录版本) {
                    ++相同身份数量;
                }
            }
            if (相同身份数量 != 1) return false;
        }
        for (const auto& 指针 : 记录仓_->当前指针组_) {
            if (!指针.已发布 || 指针.记录版本 == 0) return false;
            const auto 当前 = 查找记录_已加锁(指针.所属动态, 指针.记录版本);
            if (当前 == nullptr || 当前->记录.状态 == 记录状态::已失效) {
                return false;
            }
            for (std::uint64_t 版本 = 1;; ++版本) {
                if (查找记录_已加锁(指针.所属动态, 版本) == nullptr) return false;
                if (版本 == 指针.记录版本) break;
            }
        }
        for (const auto& 项目 : 记录仓_->记录组_) {
            if (!项目.已发布) continue;
            const auto 指针 = 查找当前指针_已加锁(项目.记录.所属动态);
            if (指针 == nullptr || 指针->记录版本 < 项目.记录.记录版本) return false;
        }
        return true;
    }

    const 动态类型化记录仓* 记录仓_ = nullptr;
};

class 状态类型化记录事务参与者 final
    : public 节点直接身份结构写入事务参与者 {
public:
    explicit 状态类型化记录事务参与者(
        状态类型化记录仓& 记录仓) noexcept : 记录仓_(&记录仓) {
    }

    bool 登记初始记录(状态域记录初始规格 规格) {
        if (阶段_ != 阶段::登记中 || !规格.完整()) return false;
        for (const auto& 已有 : 待发布规格组_) {
            if (已有.所属状态 == 规格.所属状态) return false;
        }
        try {
            待发布规格组_.push_back(std::move(规格));
        } catch (...) {
            return false;
        }
        return true;
    }

private:
    enum class 阶段 : std::uint8_t {
        登记中,
        已准备,
        已确认待发布,
        已发布,
        已撤销
    };

    节点直接身份结构写入结果 准备提交(
        const 节点直接身份结构提交准备只读视图& 视图) override {
        if (记录仓_ == nullptr || 阶段_ != 阶段::登记中
            || 待发布规格组_.empty()) {
            return {节点直接身份结构写入状态::入口拒绝, 0, 0, 0};
        }
        for (const auto& 规格 : 待发布规格组_) {
            const auto 主键 = 视图.读取候选节点稳定主键(规格.所属状态);
            if (!视图.节点是本会话候选(规格.所属状态)
                || 视图.读取候选节点类型(规格.所属状态)
                    != std::optional<节点类型>{节点类型::状态}
                || !主键.has_value()
                || 主键->命名域 != static_cast<std::uint64_t>(
                    节点稳定主键命名域::状态身份)) {
                return {节点直接身份结构写入状态::内部不一致,
                    规格.所属状态.节点编号, 0, 0};
            }
        }
        std::unique_lock<std::shared_mutex> 锁(记录仓_->锁_, std::try_to_lock);
        if (!锁.owns_lock()) {
            return {节点直接身份结构写入状态::许可拒绝, 0, 0, 0};
        }
        if (记录仓_->结构版本_
            == std::numeric_limits<std::uint64_t>::max()) {
            return {节点直接身份结构写入状态::内部不一致, 0, 0, 0};
        }
        for (const auto& 规格 : 待发布规格组_) {
            for (const auto& 指针 : 记录仓_->当前指针组_) {
                if (指针.所属状态 == 规格.所属状态) {
                    return {节点直接身份结构写入状态::版本漂移,
                        规格.所属状态.节点编号, 1, 指针.记录版本};
                }
            }
        }
        try {
            记录位置组_.reserve(待发布规格组_.size());
            指针位置组_.reserve(待发布规格组_.size());
            记录仓_->记录组_.reserve(
                记录仓_->记录组_.size() + 待发布规格组_.size());
            记录仓_->当前指针组_.reserve(
                记录仓_->当前指针组_.size() + 待发布规格组_.size());
            for (const auto& 规格 : 待发布规格组_) {
                状态域记录 记录{
                    规格.所属状态,
                    状态动态类型化记录当前格式版本,
                    1,
                    记录状态::有效,
                    规格.发生时间,
                    规格.接收时间,
                    规格.记录模式};
                if (!记录.完整()) throw std::bad_alloc();
                记录仓_->记录组_.push_back({std::move(记录), false});
                记录位置组_.push_back(记录仓_->记录组_.size() - 1);
                记录仓_->当前指针组_.push_back({规格.所属状态, 1, false});
                指针位置组_.push_back(记录仓_->当前指针组_.size() - 1);
            }
        } catch (...) {
            (void)撤销候选_已加锁();
            return {节点直接身份结构写入状态::资源失败, 0, 0, 0};
        }
        锁_ = std::move(锁);
        阶段_ = 阶段::已准备;
        return {节点直接身份结构写入状态::候选已确认, 0, 0, 0};
    }

    节点直接身份结构写入结果 确认待发布() override {
        if (阶段_ != 阶段::已准备 || !锁_.owns_lock()
            || !候选完整_已加锁()) {
            return {节点直接身份结构写入状态::内部不一致, 0, 0, 0};
        }
        阶段_ = 阶段::已确认待发布;
        return {节点直接身份结构写入状态::候选已确认, 0, 0, 0};
    }

    void 完成发布() noexcept override {
        if (阶段_ != 阶段::已确认待发布 || !锁_.owns_lock()
            || !候选完整_已加锁()
            || 记录仓_->结构版本_ == std::numeric_limits<std::uint64_t>::max()) {
            std::terminate();
        }
        for (const auto 位置 : 记录位置组_) 记录仓_->记录组_[位置].已发布 = true;
        for (const auto 位置 : 指针位置组_) 记录仓_->当前指针组_[位置].已发布 = true;
        ++记录仓_->结构版本_;
        阶段_ = 阶段::已发布;
        锁_.unlock();
    }

    节点直接身份结构写入结果 完成撤销() override {
        if (阶段_ == 阶段::已撤销) {
            return {节点直接身份结构写入状态::候选已撤销, 0, 0, 0};
        }
        if (阶段_ == 阶段::已发布) {
            return {节点直接身份结构写入状态::内部不一致, 0, 0, 0};
        }
        bool 完整 = true;
        if (锁_.owns_lock()) {
            完整 = 撤销候选_已加锁();
            锁_.unlock();
        }
        阶段_ = 阶段::已撤销;
        return {完整 ? 节点直接身份结构写入状态::候选已撤销
                         : 节点直接身份结构写入状态::内部不一致,
            0, 0, 0};
    }

    bool 候选完整_已加锁() const noexcept {
        if (记录位置组_.size() != 待发布规格组_.size()
            || 指针位置组_.size() != 待发布规格组_.size()) {
            return false;
        }
        for (std::size_t 序号 = 0; 序号 < 待发布规格组_.size(); ++序号) {
            const auto 记录位置 = 记录位置组_[序号];
            const auto 指针位置 = 指针位置组_[序号];
            if (记录位置 >= 记录仓_->记录组_.size()
                || 指针位置 >= 记录仓_->当前指针组_.size()
                || 记录仓_->记录组_[记录位置].已发布
                || 记录仓_->当前指针组_[指针位置].已发布
                || !记录仓_->记录组_[记录位置].记录.完整()
                || 记录仓_->记录组_[记录位置].记录.所属状态
                    != 待发布规格组_[序号].所属状态
                || 记录仓_->当前指针组_[指针位置].所属状态
                    != 待发布规格组_[序号].所属状态
                || 记录仓_->当前指针组_[指针位置].记录版本 != 1) {
                return false;
            }
        }
        return true;
    }

    bool 撤销候选_已加锁() noexcept {
        bool 完整 = true;
        for (auto 位置 = 指针位置组_.rbegin(); 位置 != 指针位置组_.rend(); ++位置) {
            if (*位置 >= 记录仓_->当前指针组_.size()
                || 记录仓_->当前指针组_[*位置].已发布) {
                完整 = false;
                continue;
            }
            记录仓_->当前指针组_.erase(
                记录仓_->当前指针组_.begin() + static_cast<std::ptrdiff_t>(*位置));
        }
        for (auto 位置 = 记录位置组_.rbegin(); 位置 != 记录位置组_.rend(); ++位置) {
            if (*位置 >= 记录仓_->记录组_.size()
                || 记录仓_->记录组_[*位置].已发布) {
                完整 = false;
                continue;
            }
            记录仓_->记录组_.erase(
                记录仓_->记录组_.begin() + static_cast<std::ptrdiff_t>(*位置));
        }
        指针位置组_.clear();
        记录位置组_.clear();
        return 完整;
    }

    状态类型化记录仓* 记录仓_ = nullptr;
    阶段 阶段_ = 阶段::登记中;
    std::vector<状态域记录初始规格> 待发布规格组_;
    std::vector<std::size_t> 记录位置组_;
    std::vector<std::size_t> 指针位置组_;
    std::unique_lock<std::shared_mutex> 锁_;
};

class 动态类型化记录事务参与者 final
    : public 节点直接身份结构写入事务参与者 {
public:
    explicit 动态类型化记录事务参与者(
        动态类型化记录仓& 记录仓) noexcept : 记录仓_(&记录仓) {
    }

    bool 登记初始记录(动态域记录初始规格 规格) {
        if (阶段_ != 阶段::登记中 || !规格.完整()) return false;
        for (const auto& 已有 : 待发布规格组_) {
            if (已有.所属动态 == 规格.所属动态) return false;
        }
        try {
            待发布规格组_.push_back(std::move(规格));
        } catch (...) {
            return false;
        }
        return true;
    }

private:
    enum class 阶段 : std::uint8_t {
        登记中,
        已准备,
        已确认待发布,
        已发布,
        已撤销
    };

    节点直接身份结构写入结果 准备提交(
        const 节点直接身份结构提交准备只读视图& 视图) override {
        if (记录仓_ == nullptr || 阶段_ != 阶段::登记中
            || 待发布规格组_.empty()) {
            return {节点直接身份结构写入状态::入口拒绝, 0, 0, 0};
        }
        for (const auto& 规格 : 待发布规格组_) {
            const auto 主键 = 视图.读取候选节点稳定主键(规格.所属动态);
            if (!视图.节点是本会话候选(规格.所属动态)
                || 视图.读取候选节点类型(规格.所属动态)
                    != std::optional<节点类型>{节点类型::动态}
                || !主键.has_value()
                || 主键->命名域 != static_cast<std::uint64_t>(
                    节点稳定主键命名域::动态身份)) {
                return {节点直接身份结构写入状态::内部不一致,
                    规格.所属动态.节点编号, 0, 0};
            }
        }
        std::unique_lock<std::shared_mutex> 锁(记录仓_->锁_, std::try_to_lock);
        if (!锁.owns_lock()) {
            return {节点直接身份结构写入状态::许可拒绝, 0, 0, 0};
        }
        if (记录仓_->结构版本_
            == std::numeric_limits<std::uint64_t>::max()) {
            return {节点直接身份结构写入状态::内部不一致, 0, 0, 0};
        }
        for (const auto& 规格 : 待发布规格组_) {
            for (const auto& 指针 : 记录仓_->当前指针组_) {
                if (指针.所属动态 == 规格.所属动态) {
                    return {节点直接身份结构写入状态::版本漂移,
                        规格.所属动态.节点编号, 1, 指针.记录版本};
                }
            }
        }
        try {
            记录位置组_.reserve(待发布规格组_.size());
            指针位置组_.reserve(待发布规格组_.size());
            记录仓_->记录组_.reserve(
                记录仓_->记录组_.size() + 待发布规格组_.size());
            记录仓_->当前指针组_.reserve(
                记录仓_->当前指针组_.size() + 待发布规格组_.size());
            for (const auto& 规格 : 待发布规格组_) {
                动态域记录 记录{
                    规格.所属动态,
                    状态动态类型化记录当前格式版本,
                    1,
                    记录状态::有效,
                    规格.发生时间,
                    规格.种类,
                    规格.聚合规则版本};
                if (!记录.完整()) throw std::bad_alloc();
                记录仓_->记录组_.push_back({std::move(记录), false});
                记录位置组_.push_back(记录仓_->记录组_.size() - 1);
                记录仓_->当前指针组_.push_back({规格.所属动态, 1, false});
                指针位置组_.push_back(记录仓_->当前指针组_.size() - 1);
            }
        } catch (...) {
            (void)撤销候选_已加锁();
            return {节点直接身份结构写入状态::资源失败, 0, 0, 0};
        }
        锁_ = std::move(锁);
        阶段_ = 阶段::已准备;
        return {节点直接身份结构写入状态::候选已确认, 0, 0, 0};
    }

    节点直接身份结构写入结果 确认待发布() override {
        if (阶段_ != 阶段::已准备 || !锁_.owns_lock()
            || !候选完整_已加锁()) {
            return {节点直接身份结构写入状态::内部不一致, 0, 0, 0};
        }
        阶段_ = 阶段::已确认待发布;
        return {节点直接身份结构写入状态::候选已确认, 0, 0, 0};
    }

    void 完成发布() noexcept override {
        if (阶段_ != 阶段::已确认待发布 || !锁_.owns_lock()
            || !候选完整_已加锁()
            || 记录仓_->结构版本_ == std::numeric_limits<std::uint64_t>::max()) {
            std::terminate();
        }
        for (const auto 位置 : 记录位置组_) 记录仓_->记录组_[位置].已发布 = true;
        for (const auto 位置 : 指针位置组_) 记录仓_->当前指针组_[位置].已发布 = true;
        ++记录仓_->结构版本_;
        阶段_ = 阶段::已发布;
        锁_.unlock();
    }

    节点直接身份结构写入结果 完成撤销() override {
        if (阶段_ == 阶段::已撤销) {
            return {节点直接身份结构写入状态::候选已撤销, 0, 0, 0};
        }
        if (阶段_ == 阶段::已发布) {
            return {节点直接身份结构写入状态::内部不一致, 0, 0, 0};
        }
        bool 完整 = true;
        if (锁_.owns_lock()) {
            完整 = 撤销候选_已加锁();
            锁_.unlock();
        }
        阶段_ = 阶段::已撤销;
        return {完整 ? 节点直接身份结构写入状态::候选已撤销
                         : 节点直接身份结构写入状态::内部不一致,
            0, 0, 0};
    }

    bool 候选完整_已加锁() const noexcept {
        if (记录位置组_.size() != 待发布规格组_.size()
            || 指针位置组_.size() != 待发布规格组_.size()) {
            return false;
        }
        for (std::size_t 序号 = 0; 序号 < 待发布规格组_.size(); ++序号) {
            const auto 记录位置 = 记录位置组_[序号];
            const auto 指针位置 = 指针位置组_[序号];
            if (记录位置 >= 记录仓_->记录组_.size()
                || 指针位置 >= 记录仓_->当前指针组_.size()
                || 记录仓_->记录组_[记录位置].已发布
                || 记录仓_->当前指针组_[指针位置].已发布
                || !记录仓_->记录组_[记录位置].记录.完整()
                || 记录仓_->记录组_[记录位置].记录.所属动态
                    != 待发布规格组_[序号].所属动态
                || 记录仓_->当前指针组_[指针位置].所属动态
                    != 待发布规格组_[序号].所属动态
                || 记录仓_->当前指针组_[指针位置].记录版本 != 1) {
                return false;
            }
        }
        return true;
    }

    bool 撤销候选_已加锁() noexcept {
        bool 完整 = true;
        for (auto 位置 = 指针位置组_.rbegin(); 位置 != 指针位置组_.rend(); ++位置) {
            if (*位置 >= 记录仓_->当前指针组_.size()
                || 记录仓_->当前指针组_[*位置].已发布) {
                完整 = false;
                continue;
            }
            记录仓_->当前指针组_.erase(
                记录仓_->当前指针组_.begin() + static_cast<std::ptrdiff_t>(*位置));
        }
        for (auto 位置 = 记录位置组_.rbegin(); 位置 != 记录位置组_.rend(); ++位置) {
            if (*位置 >= 记录仓_->记录组_.size()
                || 记录仓_->记录组_[*位置].已发布) {
                完整 = false;
                continue;
            }
            记录仓_->记录组_.erase(
                记录仓_->记录组_.begin() + static_cast<std::ptrdiff_t>(*位置));
        }
        指针位置组_.clear();
        记录位置组_.clear();
        return 完整;
    }

    动态类型化记录仓* 记录仓_ = nullptr;
    阶段 阶段_ = 阶段::登记中;
    std::vector<动态域记录初始规格> 待发布规格组_;
    std::vector<std::size_t> 记录位置组_;
    std::vector<std::size_t> 指针位置组_;
    std::unique_lock<std::shared_mutex> 锁_;
};

}

namespace 海中鱼巣 {

class 状态记录退役事务参与者 final
    : public 节点直接身份结构写入事务参与者 {
public:
    状态记录退役事务参与者(
        状态类型化记录仓& 记录仓,
        状态退役规格 规格,
        状态域记录 写前记录) noexcept
        : 记录仓_(&记录仓), 规格_(规格), 写前记录_(std::move(写前记录)) {
    }

    std::optional<状态记录退役审计材料> 读取审计() const noexcept {
        return 审计_;
    }

    bool 发布后记录精确读回() const {
        if (!审计_.has_value() || 记录仓_ == nullptr) return false;
        try {
            std::shared_lock<std::shared_mutex> 锁(记录仓_->锁_);
            const 状态类型化记录仓::内部记录* 读回 = nullptr;
            std::size_t 记录数量 = 0;
            for (const auto& 项目 : 记录仓_->记录组_) {
                if (!项目.已发布
                    || 项目.记录.所属状态 != 规格_.状态
                    || 项目.记录.记录版本
                        != 审计_->退役后记录版本) {
                    continue;
                }
                ++记录数量;
                读回 = &项目;
            }
            std::size_t 指针数量 = 0;
            const 状态类型化记录仓::当前指针* 当前 = nullptr;
            for (const auto& 指针 : 记录仓_->当前指针组_) {
                if (!指针.已发布 || 指针.所属状态 != 规格_.状态) {
                    continue;
                }
                ++指针数量;
                当前 = &指针;
            }
            return 记录数量 == 1 && 指针数量 == 1
                && 读回 != nullptr && 当前 != nullptr
                && 当前->记录版本 == 审计_->退役后记录版本
                && 记录仓_->结构版本_ >= 审计_->发布后结构版本
                && 读回->记录.所属状态 == 写前记录_.所属状态
                && 读回->记录.格式版本 == 写前记录_.格式版本
                && 读回->记录.记录版本 == 写前记录_.记录版本 + 1
                && 读回->记录.状态 == 记录状态::已删除
                && 读回->记录.发生时间 == 写前记录_.发生时间
                && 读回->记录.接收时间 == 写前记录_.接收时间
                && 读回->记录.记录模式 == 写前记录_.记录模式;
        } catch (...) {
            return false;
        }
    }

private:
    enum class 阶段 : std::uint8_t {
        待准备,
        已准备,
        已确认待发布,
        已发布,
        已撤销
    };

    节点直接身份结构写入结果 准备提交(
        const 节点直接身份结构提交准备只读视图& 视图) override {
        if (记录仓_ == nullptr || 阶段_ != 阶段::待准备
            || !写前记录_.完整() || 写前记录_.状态 != 记录状态::有效
            || 写前记录_.记录版本 == std::numeric_limits<std::uint64_t>::max()
            || 视图.读取候选节点类型(规格_.状态)
                != std::optional<节点类型>{节点类型::状态}) {
            return {节点直接身份结构写入状态::内部不一致,
                规格_.状态.节点编号, 0, 0};
        }
        std::unique_lock<std::shared_mutex> 锁(记录仓_->锁_, std::try_to_lock);
        if (!锁.owns_lock()) {
            return {节点直接身份结构写入状态::许可拒绝, 0, 0, 0};
        }
        if (记录仓_->结构版本_
            == std::numeric_limits<std::uint64_t>::max()) {
            return {节点直接身份结构写入状态::内部不一致, 0, 0, 0};
        }
        std::size_t 指针数量 = 0;
        for (std::size_t 序号 = 0; 序号 < 记录仓_->当前指针组_.size(); ++序号) {
            const auto& 指针 = 记录仓_->当前指针组_[序号];
            if (指针.已发布 && 指针.所属状态 == 规格_.状态) {
                ++指针数量;
                指针位置_ = 序号;
                if (指针.记录版本 != 写前记录_.记录版本) {
                    return {节点直接身份结构写入状态::版本漂移,
                        规格_.状态.节点编号, 0, 0};
                }
            }
        }
        if (指针数量 != 1) {
            return {节点直接身份结构写入状态::内部不一致,
                规格_.状态.节点编号, 0, 0};
        }
        std::size_t 记录数量 = 0;
        for (const auto& 项目 : 记录仓_->记录组_) {
            if (项目.已发布 && 项目.记录.所属状态 == 规格_.状态
                && 项目.记录.记录版本 == 写前记录_.记录版本) {
                ++记录数量;
                if (!(项目.记录 == 写前记录_)) {
                    return {节点直接身份结构写入状态::版本漂移,
                        规格_.状态.节点编号, 0, 0};
                }
            }
        }
        if (记录数量 != 1) {
            return {节点直接身份结构写入状态::内部不一致,
                规格_.状态.节点编号, 0, 0};
        }
        try {
            记录仓_->记录组_.reserve(记录仓_->记录组_.size() + 1);
            auto 退役记录 = 写前记录_;
            ++退役记录.记录版本;
            退役记录.状态 = 记录状态::已删除;
            记录仓_->记录组_.push_back({std::move(退役记录), false});
            记录位置_ = 记录仓_->记录组_.size() - 1;
        } catch (...) {
            return {节点直接身份结构写入状态::资源失败, 0, 0, 0};
        }
        锁_ = std::move(锁);
        阶段_ = 阶段::已准备;
        return {节点直接身份结构写入状态::候选已确认, 0, 0, 0};
    }

    节点直接身份结构写入结果 确认待发布() override {
        if (阶段_ != 阶段::已准备 || !候选完整_已加锁()) {
            return {节点直接身份结构写入状态::内部不一致, 0, 0, 0};
        }
        阶段_ = 阶段::已确认待发布;
        return {节点直接身份结构写入状态::候选已确认, 0, 0, 0};
    }

    void 完成发布() noexcept override {
        if (阶段_ != 阶段::已确认待发布 || !候选完整_已加锁()
            || 记录仓_->结构版本_ == std::numeric_limits<std::uint64_t>::max()) {
            std::terminate();
        }
        auto& 新记录 = 记录仓_->记录组_[记录位置_];
        新记录.已发布 = true;
        记录仓_->当前指针组_[指针位置_].记录版本 = 新记录.记录.记录版本;
        ++记录仓_->结构版本_;
        审计_ = 状态记录退役审计材料{
            规格_.状态,
            写前记录_.记录版本,
            写前记录_.状态,
            新记录.记录.记录版本,
            新记录.记录.状态,
            记录仓_->结构版本_};
        阶段_ = 阶段::已发布;
        锁_.unlock();
    }

    节点直接身份结构写入结果 完成撤销() override {
        if (阶段_ == 阶段::已撤销) {
            return {节点直接身份结构写入状态::候选已撤销, 0, 0, 0};
        }
        if (阶段_ == 阶段::已发布) {
            return {节点直接身份结构写入状态::内部不一致, 0, 0, 0};
        }
        bool 完整 = true;
        if (锁_.owns_lock()) {
            if (记录位置_ >= 记录仓_->记录组_.size()
                || 记录仓_->记录组_[记录位置_].已发布) {
                完整 = false;
            } else {
                记录仓_->记录组_.erase(
                    记录仓_->记录组_.begin() + static_cast<std::ptrdiff_t>(记录位置_));
            }
            锁_.unlock();
        }
        阶段_ = 阶段::已撤销;
        return {完整 ? 节点直接身份结构写入状态::候选已撤销
                         : 节点直接身份结构写入状态::内部不一致,
            0, 0, 0};
    }

    bool 候选完整_已加锁() const noexcept {
        return 锁_.owns_lock()
            && 记录位置_ < 记录仓_->记录组_.size()
            && 指针位置_ < 记录仓_->当前指针组_.size()
            && !记录仓_->记录组_[记录位置_].已发布
            && 记录仓_->记录组_[记录位置_].记录.完整()
            && 记录仓_->记录组_[记录位置_].记录.所属状态 == 规格_.状态
            && 记录仓_->记录组_[记录位置_].记录.记录版本
                == 写前记录_.记录版本 + 1
            && 记录仓_->记录组_[记录位置_].记录.状态 == 记录状态::已删除
            && 记录仓_->当前指针组_[指针位置_].已发布
            && 记录仓_->当前指针组_[指针位置_].记录版本
                == 写前记录_.记录版本;
    }

    状态类型化记录仓* 记录仓_ = nullptr;
    状态退役规格 规格_;
    状态域记录 写前记录_;
    阶段 阶段_ = 阶段::待准备;
    std::size_t 记录位置_ = 0;
    std::size_t 指针位置_ = 0;
    std::unique_lock<std::shared_mutex> 锁_;
    std::optional<状态记录退役审计材料> 审计_;
};

class 动态记录退役事务参与者 final
    : public 节点直接身份结构写入事务参与者 {
public:
    动态记录退役事务参与者(
        动态类型化记录仓& 记录仓,
        动态退役规格 规格,
        动态域记录 写前记录) noexcept
        : 记录仓_(&记录仓), 规格_(规格), 写前记录_(std::move(写前记录)) {
    }

    std::optional<动态记录退役审计材料> 读取审计() const noexcept {
        return 审计_;
    }

    bool 发布后记录精确读回() const {
        if (!审计_.has_value() || 记录仓_ == nullptr) return false;
        try {
            std::shared_lock<std::shared_mutex> 锁(记录仓_->锁_);
            const 动态类型化记录仓::内部记录* 读回 = nullptr;
            std::size_t 记录数量 = 0;
            for (const auto& 项目 : 记录仓_->记录组_) {
                if (!项目.已发布
                    || 项目.记录.所属动态 != 规格_.动态
                    || 项目.记录.记录版本
                        != 审计_->退役后记录版本) {
                    continue;
                }
                ++记录数量;
                读回 = &项目;
            }
            std::size_t 指针数量 = 0;
            const 动态类型化记录仓::当前指针* 当前 = nullptr;
            for (const auto& 指针 : 记录仓_->当前指针组_) {
                if (!指针.已发布 || 指针.所属动态 != 规格_.动态) {
                    continue;
                }
                ++指针数量;
                当前 = &指针;
            }
            return 记录数量 == 1 && 指针数量 == 1
                && 读回 != nullptr && 当前 != nullptr
                && 当前->记录版本 == 审计_->退役后记录版本
                && 记录仓_->结构版本_ >= 审计_->发布后结构版本
                && 读回->记录.所属动态 == 写前记录_.所属动态
                && 读回->记录.格式版本 == 写前记录_.格式版本
                && 读回->记录.记录版本 == 写前记录_.记录版本 + 1
                && 读回->记录.状态 == 记录状态::已删除
                && 读回->记录.发生时间 == 写前记录_.发生时间
                && 读回->记录.种类 == 写前记录_.种类
                && 读回->记录.聚合规则版本
                    == 写前记录_.聚合规则版本;
        } catch (...) {
            return false;
        }
    }

private:
    enum class 阶段 : std::uint8_t {
        待准备,
        已准备,
        已确认待发布,
        已发布,
        已撤销
    };

    节点直接身份结构写入结果 准备提交(
        const 节点直接身份结构提交准备只读视图& 视图) override {
        if (记录仓_ == nullptr || 阶段_ != 阶段::待准备
            || !写前记录_.完整() || 写前记录_.状态 != 记录状态::有效
            || 写前记录_.记录版本 == std::numeric_limits<std::uint64_t>::max()
            || 视图.读取候选节点类型(规格_.动态)
                != std::optional<节点类型>{节点类型::动态}) {
            return {节点直接身份结构写入状态::内部不一致,
                规格_.动态.节点编号, 0, 0};
        }
        std::unique_lock<std::shared_mutex> 锁(记录仓_->锁_, std::try_to_lock);
        if (!锁.owns_lock()) {
            return {节点直接身份结构写入状态::许可拒绝, 0, 0, 0};
        }
        if (记录仓_->结构版本_
            == std::numeric_limits<std::uint64_t>::max()) {
            return {节点直接身份结构写入状态::内部不一致, 0, 0, 0};
        }
        std::size_t 指针数量 = 0;
        for (std::size_t 序号 = 0; 序号 < 记录仓_->当前指针组_.size(); ++序号) {
            const auto& 指针 = 记录仓_->当前指针组_[序号];
            if (指针.已发布 && 指针.所属动态 == 规格_.动态) {
                ++指针数量;
                指针位置_ = 序号;
                if (指针.记录版本 != 写前记录_.记录版本) {
                    return {节点直接身份结构写入状态::版本漂移,
                        规格_.动态.节点编号, 0, 0};
                }
            }
        }
        if (指针数量 != 1) {
            return {节点直接身份结构写入状态::内部不一致,
                规格_.动态.节点编号, 0, 0};
        }
        std::size_t 记录数量 = 0;
        for (const auto& 项目 : 记录仓_->记录组_) {
            if (项目.已发布 && 项目.记录.所属动态 == 规格_.动态
                && 项目.记录.记录版本 == 写前记录_.记录版本) {
                ++记录数量;
                if (!(项目.记录 == 写前记录_)) {
                    return {节点直接身份结构写入状态::版本漂移,
                        规格_.动态.节点编号, 0, 0};
                }
            }
        }
        if (记录数量 != 1) {
            return {节点直接身份结构写入状态::内部不一致,
                规格_.动态.节点编号, 0, 0};
        }
        try {
            记录仓_->记录组_.reserve(记录仓_->记录组_.size() + 1);
            auto 退役记录 = 写前记录_;
            ++退役记录.记录版本;
            退役记录.状态 = 记录状态::已删除;
            记录仓_->记录组_.push_back({std::move(退役记录), false});
            记录位置_ = 记录仓_->记录组_.size() - 1;
        } catch (...) {
            return {节点直接身份结构写入状态::资源失败, 0, 0, 0};
        }
        锁_ = std::move(锁);
        阶段_ = 阶段::已准备;
        return {节点直接身份结构写入状态::候选已确认, 0, 0, 0};
    }

    节点直接身份结构写入结果 确认待发布() override {
        if (阶段_ != 阶段::已准备 || !候选完整_已加锁()) {
            return {节点直接身份结构写入状态::内部不一致, 0, 0, 0};
        }
        阶段_ = 阶段::已确认待发布;
        return {节点直接身份结构写入状态::候选已确认, 0, 0, 0};
    }

    void 完成发布() noexcept override {
        if (阶段_ != 阶段::已确认待发布 || !候选完整_已加锁()
            || 记录仓_->结构版本_ == std::numeric_limits<std::uint64_t>::max()) {
            std::terminate();
        }
        auto& 新记录 = 记录仓_->记录组_[记录位置_];
        新记录.已发布 = true;
        记录仓_->当前指针组_[指针位置_].记录版本 = 新记录.记录.记录版本;
        ++记录仓_->结构版本_;
        审计_ = 动态记录退役审计材料{
            规格_.动态,
            写前记录_.记录版本,
            写前记录_.状态,
            新记录.记录.记录版本,
            新记录.记录.状态,
            记录仓_->结构版本_};
        阶段_ = 阶段::已发布;
        锁_.unlock();
    }

    节点直接身份结构写入结果 完成撤销() override {
        if (阶段_ == 阶段::已撤销) {
            return {节点直接身份结构写入状态::候选已撤销, 0, 0, 0};
        }
        if (阶段_ == 阶段::已发布) {
            return {节点直接身份结构写入状态::内部不一致, 0, 0, 0};
        }
        bool 完整 = true;
        if (锁_.owns_lock()) {
            if (记录位置_ >= 记录仓_->记录组_.size()
                || 记录仓_->记录组_[记录位置_].已发布) {
                完整 = false;
            } else {
                记录仓_->记录组_.erase(
                    记录仓_->记录组_.begin() + static_cast<std::ptrdiff_t>(记录位置_));
            }
            锁_.unlock();
        }
        阶段_ = 阶段::已撤销;
        return {完整 ? 节点直接身份结构写入状态::候选已撤销
                         : 节点直接身份结构写入状态::内部不一致,
            0, 0, 0};
    }

    bool 候选完整_已加锁() const noexcept {
        return 锁_.owns_lock()
            && 记录位置_ < 记录仓_->记录组_.size()
            && 指针位置_ < 记录仓_->当前指针组_.size()
            && !记录仓_->记录组_[记录位置_].已发布
            && 记录仓_->记录组_[记录位置_].记录.完整()
            && 记录仓_->记录组_[记录位置_].记录.所属动态 == 规格_.动态
            && 记录仓_->记录组_[记录位置_].记录.记录版本
                == 写前记录_.记录版本 + 1
            && 记录仓_->记录组_[记录位置_].记录.状态 == 记录状态::已删除
            && 记录仓_->当前指针组_[指针位置_].已发布
            && 记录仓_->当前指针组_[指针位置_].记录版本
                == 写前记录_.记录版本;
    }

    动态类型化记录仓* 记录仓_ = nullptr;
    动态退役规格 规格_;
    动态域记录 写前记录_;
    阶段 阶段_ = 阶段::待准备;
    std::size_t 记录位置_ = 0;
    std::size_t 指针位置_ = 0;
    std::unique_lock<std::shared_mutex> 锁_;
    std::optional<动态记录退役审计材料> 审计_;
};

}

export namespace 海中鱼巣 {

class 状态记录退役参与包 final {
public:
    状态记录退役参与包(const 状态记录退役参与包&) = delete;
    状态记录退役参与包& operator=(const 状态记录退役参与包&) = delete;
    状态记录退役参与包(状态记录退役参与包&&) noexcept = default;
    状态记录退役参与包& operator=(状态记录退役参与包&&) noexcept = default;

    bool 有效() const noexcept { return 参与者_ != nullptr && !已执行_; }

    状态记录退役执行结果 执行一次(
        节点直接身份结构写入执行器& 执行器,
        const std::function<void(节点直接身份结构写入会话&)>& 回调) {
        if (!有效() || !执行器.有效() || !回调) {
            return {{节点直接身份结构写入状态::入口拒绝, 0, 0, 0},
                合同结果状态::入口拒绝, 合同结果原因::无效身份, std::nullopt};
        }
        已执行_ = true;
        const auto 权威结果 = 执行器.执行(回调, *参与者_);
        if (权威结果.状态 != 节点直接身份结构写入状态::已提交) {
            return {权威结果, 映射状态(权威结果), 映射原因(权威结果), std::nullopt};
        }
        const auto 审计 = 参与者_->读取审计();
        if (!审计.has_value() || !参与者_->发布后记录精确读回()
            || 审计->退役前记录状态 != 记录状态::有效
            || 审计->退役后记录状态 != 记录状态::已删除
            || 审计->退役后记录版本 != 审计->退役前记录版本 + 1
            || 审计->发布后结构版本 == 0) {
            return {权威结果, 合同结果状态::内部错误,
                合同结果原因::权威读回不一致, std::nullopt};
        }
        return {权威结果, 合同结果状态::成功, 合同结果原因::无, 审计};
    }

private:
    friend class 状态动态记录退役包工厂;

    explicit 状态记录退役参与包(
        std::unique_ptr<状态记录退役事务参与者> 参与者) noexcept
        : 参与者_(std::move(参与者)) {
    }

    static 合同结果状态 映射状态(
        const 节点直接身份结构写入结果& 结果) noexcept {
        switch (结果.状态) {
        case 节点直接身份结构写入状态::许可拒绝:
            return 合同结果状态::许可竞争;
        case 节点直接身份结构写入状态::版本漂移:
            return 合同结果状态::写前漂移;
        case 节点直接身份结构写入状态::入口拒绝:
            return 合同结果状态::入口拒绝;
        default:
            return 合同结果状态::内部错误;
        }
    }

    static 合同结果原因 映射原因(
        const 节点直接身份结构写入结果& 结果) noexcept {
        switch (结果.状态) {
        case 节点直接身份结构写入状态::许可拒绝:
            return 合同结果原因::并发许可失败;
        case 节点直接身份结构写入状态::版本漂移:
            return 合同结果原因::错版本;
        case 节点直接身份结构写入状态::入口拒绝:
            return 合同结果原因::无效身份;
        default:
            return 合同结果原因::事务阶段不一致;
        }
    }

    std::unique_ptr<状态记录退役事务参与者> 参与者_;
    bool 已执行_ = false;
};

class 动态记录退役参与包 final {
public:
    动态记录退役参与包(const 动态记录退役参与包&) = delete;
    动态记录退役参与包& operator=(const 动态记录退役参与包&) = delete;
    动态记录退役参与包(动态记录退役参与包&&) noexcept = default;
    动态记录退役参与包& operator=(动态记录退役参与包&&) noexcept = default;

    bool 有效() const noexcept { return 参与者_ != nullptr && !已执行_; }

    动态记录退役执行结果 执行一次(
        节点直接身份结构写入执行器& 执行器,
        const std::function<void(节点直接身份结构写入会话&)>& 回调) {
        if (!有效() || !执行器.有效() || !回调) {
            return {{节点直接身份结构写入状态::入口拒绝, 0, 0, 0},
                合同结果状态::入口拒绝, 合同结果原因::无效身份, std::nullopt};
        }
        已执行_ = true;
        const auto 权威结果 = 执行器.执行(回调, *参与者_);
        if (权威结果.状态 != 节点直接身份结构写入状态::已提交) {
            return {权威结果, 映射状态(权威结果), 映射原因(权威结果), std::nullopt};
        }
        const auto 审计 = 参与者_->读取审计();
        if (!审计.has_value() || !参与者_->发布后记录精确读回()
            || 审计->退役前记录状态 != 记录状态::有效
            || 审计->退役后记录状态 != 记录状态::已删除
            || 审计->退役后记录版本 != 审计->退役前记录版本 + 1
            || 审计->发布后结构版本 == 0) {
            return {权威结果, 合同结果状态::内部错误,
                合同结果原因::权威读回不一致, std::nullopt};
        }
        return {权威结果, 合同结果状态::成功, 合同结果原因::无, 审计};
    }

private:
    friend class 状态动态记录退役包工厂;

    explicit 动态记录退役参与包(
        std::unique_ptr<动态记录退役事务参与者> 参与者) noexcept
        : 参与者_(std::move(参与者)) {
    }

    static 合同结果状态 映射状态(
        const 节点直接身份结构写入结果& 结果) noexcept {
        switch (结果.状态) {
        case 节点直接身份结构写入状态::许可拒绝:
            return 合同结果状态::许可竞争;
        case 节点直接身份结构写入状态::版本漂移:
            return 合同结果状态::写前漂移;
        case 节点直接身份结构写入状态::入口拒绝:
            return 合同结果状态::入口拒绝;
        default:
            return 合同结果状态::内部错误;
        }
    }

    static 合同结果原因 映射原因(
        const 节点直接身份结构写入结果& 结果) noexcept {
        switch (结果.状态) {
        case 节点直接身份结构写入状态::许可拒绝:
            return 合同结果原因::并发许可失败;
        case 节点直接身份结构写入状态::版本漂移:
            return 合同结果原因::错版本;
        case 节点直接身份结构写入状态::入口拒绝:
            return 合同结果原因::无效身份;
        default:
            return 合同结果原因::事务阶段不一致;
        }
    }

    std::unique_ptr<动态记录退役事务参与者> 参与者_;
    bool 已执行_ = false;
};

struct 状态记录退役参与包形成结果 final {
    退役参与包形成状态 状态 = 退役参与包形成状态::入口拒绝;
    std::optional<状态记录退役参与包> 参与包;
};

struct 动态记录退役参与包形成结果 final {
    退役参与包形成状态 状态 = 退役参与包形成状态::入口拒绝;
    std::optional<动态记录退役参与包> 参与包;
};

class 状态动态记录退役包工厂 final {
public:
    static 状态记录退役参与包 形成状态包(
        状态类型化记录仓& 记录仓,
        状态退役规格 规格,
        状态域记录 写前记录) {
        return 状态记录退役参与包(
            std::make_unique<状态记录退役事务参与者>(
                记录仓, 规格, std::move(写前记录)));
    }

    static 动态记录退役参与包 形成动态包(
        动态类型化记录仓& 记录仓,
        动态退役规格 规格,
        动态域记录 写前记录) {
        return 动态记录退役参与包(
            std::make_unique<动态记录退役事务参与者>(
                记录仓, 规格, std::move(写前记录)));
    }
};

}
