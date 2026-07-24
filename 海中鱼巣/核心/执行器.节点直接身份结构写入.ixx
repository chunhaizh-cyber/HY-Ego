// 文件规则：节点直接身份结构写入执行器只编排隔离新域独占许可、结构会话和强类型参与者，不承载领域值或业务准入。
module;

#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <limits>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <span>
#include <type_traits>
#include <utility>

export module 海中鱼巣.核心.执行器.节点直接身份结构写入;

export import 海中鱼巣.核心.会话.节点直接身份结构写入;

export namespace 海中鱼巣 {

class 节点直接身份结构写入执行器;

struct 节点直接统一冻结见证 final {
    std::uint64_t 冻结域身份 = 0;
    std::uint64_t 已发布代次 = 0;
    std::uint32_t 冻结规则版本 = 0;
};

class 节点直接统一冻结许可 final {
public:
    节点直接统一冻结许可() = default;
    节点直接统一冻结许可(const 节点直接统一冻结许可&) = delete;
    节点直接统一冻结许可& operator=(const 节点直接统一冻结许可&) = delete;
    节点直接统一冻结许可(节点直接统一冻结许可&&) noexcept = default;
    节点直接统一冻结许可& operator=(节点直接统一冻结许可&& 来源) noexcept {
        if (this == &来源) return *this;
        if (锁_.owns_lock()) 锁_.unlock();
        锁_ = std::move(来源.锁_);
        互斥_ = std::move(来源.互斥_);
        隔离标记_ = std::move(来源.隔离标记_);
        冻结域身份_ = std::move(来源.冻结域身份_);
        已发布代次_ = std::move(来源.已发布代次_);
        冻结规则版本_ = std::exchange(来源.冻结规则版本_, 0);
        return *this;
    }

    bool 有效() const noexcept {
        return 锁_.owns_lock() && 互斥_ != nullptr && 隔离标记_ != nullptr
            && 冻结域身份_ != nullptr && 已发布代次_ != nullptr
            && *冻结域身份_ != 0 && 冻结规则版本_ != 0
            && !隔离标记_->load(std::memory_order_acquire);
    }

    节点直接统一冻结见证 读取见证() const noexcept {
        if (!有效()) return {};
        return {*冻结域身份_,
            已发布代次_->load(std::memory_order_acquire),
            冻结规则版本_};
    }

private:
    friend class 节点直接身份结构事务域;

    节点直接统一冻结许可(
        std::shared_ptr<std::shared_mutex> 互斥,
        std::shared_ptr<std::atomic_bool> 隔离标记,
        std::shared_ptr<const std::uint64_t> 冻结域身份,
        std::shared_ptr<std::atomic_uint64_t> 已发布代次,
        std::uint32_t 冻结规则版本,
        std::uint32_t 预期冻结规则版本)
        : 互斥_(std::move(互斥)), 隔离标记_(std::move(隔离标记)),
          冻结域身份_(std::move(冻结域身份)), 已发布代次_(std::move(已发布代次)),
          冻结规则版本_(冻结规则版本) {
        if (冻结规则版本_ == 0 || 冻结规则版本_ != 预期冻结规则版本
            || 互斥_ == nullptr || 隔离标记_ == nullptr || 冻结域身份_ == nullptr
            || 已发布代次_ == nullptr || *冻结域身份_ == 0
            || 已发布代次_->load(std::memory_order_acquire) == 0
            || 隔离标记_->load(std::memory_order_acquire)) {
            return;
        }
        锁_ = std::shared_lock<std::shared_mutex>(*互斥_, std::try_to_lock);
        if (锁_.owns_lock() && 隔离标记_->load(std::memory_order_acquire)) {
            锁_.unlock();
        }
    }

    std::shared_ptr<std::shared_mutex> 互斥_;
    std::shared_ptr<std::atomic_bool> 隔离标记_;
    std::shared_ptr<const std::uint64_t> 冻结域身份_;
    std::shared_ptr<std::atomic_uint64_t> 已发布代次_;
    std::uint32_t 冻结规则版本_ = 0;
    std::shared_lock<std::shared_mutex> 锁_;
};

class 节点直接身份结构读取许可 final {
public:
    节点直接身份结构读取许可() = default;
    节点直接身份结构读取许可(const 节点直接身份结构读取许可&) = delete;
    节点直接身份结构读取许可& operator=(const 节点直接身份结构读取许可&) = delete;
    节点直接身份结构读取许可(节点直接身份结构读取许可&&) noexcept = default;
    节点直接身份结构读取许可& operator=(节点直接身份结构读取许可&&) noexcept = default;

    bool 有效() const noexcept {
        return 锁_.owns_lock() && !隔离标记_->load(std::memory_order_acquire);
    }

private:
    friend class 节点直接身份结构事务域;

    节点直接身份结构读取许可(
        std::shared_ptr<std::shared_mutex> 互斥,
        std::shared_ptr<std::atomic_bool> 隔离标记)
        : 隔离标记_(std::move(隔离标记)), 锁_(*互斥, std::try_to_lock), 互斥_(std::move(互斥)) {
        if (锁_.owns_lock() && 隔离标记_->load(std::memory_order_acquire)) 锁_.unlock();
    }

    std::shared_ptr<std::atomic_bool> 隔离标记_ = std::make_shared<std::atomic_bool>(true);
    std::shared_lock<std::shared_mutex> 锁_;
    std::shared_ptr<std::shared_mutex> 互斥_;
};

class 节点直接身份结构事务许可 final {
public:
    节点直接身份结构事务许可() = default;
    节点直接身份结构事务许可(const 节点直接身份结构事务许可&) = delete;
    节点直接身份结构事务许可& operator=(const 节点直接身份结构事务许可&) = delete;
    节点直接身份结构事务许可(节点直接身份结构事务许可&&) noexcept = default;
    节点直接身份结构事务许可& operator=(节点直接身份结构事务许可&&) noexcept = default;

    bool 有效() const noexcept {
        return 锁_.owns_lock() && 事务序号_ != 0
            && !隔离标记_->load(std::memory_order_acquire);
    }

    std::uint64_t 事务序号() const noexcept { return 事务序号_; }

private:
    friend class 节点直接身份结构事务域;
    friend class 节点直接身份结构写入执行器;

    节点直接身份结构事务许可(
        std::shared_ptr<std::shared_mutex> 互斥,
        std::shared_ptr<std::atomic_bool> 隔离标记,
        std::uint64_t 事务序号)
        : 隔离标记_(std::move(隔离标记)), 锁_(*互斥, std::try_to_lock),
          互斥_(std::move(互斥)), 事务序号_(锁_.owns_lock() ? 事务序号 : 0) {
        if (锁_.owns_lock() && 隔离标记_->load(std::memory_order_acquire)) {
            锁_.unlock();
            事务序号_ = 0;
        }
    }

    void 标记隔离() noexcept {
        if (锁_.owns_lock()) 隔离标记_->store(true, std::memory_order_release);
    }

    std::shared_ptr<std::atomic_bool> 隔离标记_ = std::make_shared<std::atomic_bool>(true);
    std::unique_lock<std::shared_mutex> 锁_;
    std::shared_ptr<std::shared_mutex> 互斥_;
    std::uint64_t 事务序号_ = 0;
};

class 节点直接身份结构事务域 final {
public:
    节点直接身份结构事务域()
        : 互斥_(std::make_shared<std::shared_mutex>()),
          隔离标记_(std::make_shared<std::atomic_bool>(false)),
          冻结域身份_(std::make_shared<const std::uint64_t>(签发冻结域身份())),
          已发布代次_(std::make_shared<std::atomic_uint64_t>(1)) {
        if (*冻结域身份_ == 0) {
            隔离标记_->store(true, std::memory_order_release);
        }
    }

    节点直接身份结构事务域(const 节点直接身份结构事务域&) = delete;
    节点直接身份结构事务域& operator=(const 节点直接身份结构事务域&) = delete;

    节点直接身份结构读取许可 取得读取许可() const {
        return 节点直接身份结构读取许可(互斥_, 隔离标记_);
    }

    节点直接统一冻结许可 取得统一冻结许可(
        std::uint32_t 预期冻结规则版本) const {
        return 节点直接统一冻结许可(
            互斥_, 隔离标记_, 冻结域身份_, 已发布代次_,
            当前冻结规则版本, 预期冻结规则版本);
    }

    bool 冻结许可属于本域(
        const 节点直接统一冻结许可& 许可) const noexcept {
        if (!许可.有效() || 许可.互斥_.get() != 互斥_.get()
            || 许可.隔离标记_.get() != 隔离标记_.get()
            || 许可.冻结域身份_.get() != 冻结域身份_.get()
            || 许可.已发布代次_.get() != 已发布代次_.get()
            || 许可.冻结规则版本_ != 当前冻结规则版本) {
            return false;
        }
        const auto 见证 = 许可.读取见证();
        return 见证.冻结域身份 == *冻结域身份_
            && 见证.已发布代次 == 已发布代次_->load(std::memory_order_acquire)
            && 见证.冻结规则版本 == 当前冻结规则版本;
    }

    bool 已隔离() const noexcept {
        return 隔离标记_->load(std::memory_order_acquire);
    }

private:
    friend class 节点直接身份结构写入执行器;

    节点直接身份结构事务许可 取得独占许可() {
        if (已隔离()) return {};
        const auto 事务序号 = 下一事务序号_.fetch_add(1, std::memory_order_relaxed);
        return 节点直接身份结构事务许可(互斥_, 隔离标记_, 事务序号);
    }

    static std::uint64_t 签发冻结域身份() noexcept {
        static std::atomic_uint64_t 下一冻结域身份{1};
        auto 候选 = 下一冻结域身份.load(std::memory_order_relaxed);
        while (候选 != std::numeric_limits<std::uint64_t>::max()) {
            if (下一冻结域身份.compare_exchange_weak(
                    候选, 候选 + 1,
                    std::memory_order_relaxed, std::memory_order_relaxed)) {
                return 候选;
            }
        }
        return 0;
    }

    bool 推进已发布代次(
        const 节点直接身份结构事务许可& 许可) noexcept {
        if (!许可.有效() || 许可.互斥_.get() != 互斥_.get()) {
            隔离标记_->store(true, std::memory_order_release);
            return false;
        }
        const auto 当前代次 = 已发布代次_->load(std::memory_order_relaxed);
        if (当前代次 == 0
            || 当前代次 == std::numeric_limits<std::uint64_t>::max()) {
            隔离标记_->store(true, std::memory_order_release);
            return false;
        }
        已发布代次_->store(当前代次 + 1, std::memory_order_release);
        return true;
    }

    static constexpr std::uint32_t 当前冻结规则版本 = 1;
    std::shared_ptr<std::shared_mutex> 互斥_;
    std::shared_ptr<std::atomic_bool> 隔离标记_;
    std::shared_ptr<const std::uint64_t> 冻结域身份_;
    std::shared_ptr<std::atomic_uint64_t> 已发布代次_;
    std::atomic_uint64_t 下一事务序号_{1};
};

class 节点直接身份结构写入事务参与者 {
public:
    virtual ~节点直接身份结构写入事务参与者() = default;

protected:
    节点直接身份结构写入事务参与者() = default;

private:
    friend class 节点直接身份结构写入执行器;

    virtual 节点直接身份结构写入结果 准备提交(
        const 节点直接身份结构提交准备只读视图& 视图) = 0;
    virtual 节点直接身份结构写入结果 确认待发布() = 0;
    virtual void 完成发布() noexcept = 0;
    virtual 节点直接身份结构写入结果 完成撤销() = 0;
};

class 节点直接身份结构写入执行器 final {
public:
    inline 节点直接身份结构写入执行器(
        节点直接身份结构事务域* 事务域,
        节点直接身份仓库* 节点,
        正式关系仓库* 关系,
        可重建索引仓库* 索引) noexcept
        : 事务域_(事务域), 节点_(节点), 关系_(关系), 索引_(索引) {
    }

    inline bool 有效() const noexcept {
        return 事务域_ != nullptr && 节点_ != nullptr && 关系_ != nullptr && 索引_ != nullptr;
    }

    inline 节点直接身份结构写入结果 执行(
        const std::function<void(节点直接身份结构写入会话&)>& 回调) const {
        const std::span<节点直接身份结构写入事务参与者* const> 空参与者组;
        return 执行(回调, 空参与者组);
    }

    inline 节点直接身份结构写入结果 执行(
        const std::function<void(节点直接身份结构写入会话&)>& 回调,
        std::span<节点直接身份结构写入事务参与者* const> 参与者组) const {
        if (!有效() || !回调 || !有效参与者组(参与者组)) return 入口拒绝结果();
        auto 许可 = 事务域_->取得独占许可();
        if (!许可.有效()) return {节点直接身份结构写入状态::许可拒绝, 0, 0, 0};

        节点直接身份结构写入会话 会话(*节点_, *关系_, *索引_, 许可.事务序号());
        try {
            回调(会话);
        } catch (...) {
            return 撤销并收口(
                会话, 参与者组, 许可,
                {节点直接身份结构写入状态::内部不一致, 0, 0, 0});
        }

        if (会话.有失败()) {
            return 撤销并收口(会话, 参与者组, 许可, 会话.读取首次失败());
        }
        if (!会话.已请求提交()) {
            const bool 显式撤销 = 会话.已请求撤销();
            const auto 撤销结果 = 撤销并收口(
                会话, 参与者组, 许可,
                {节点直接身份结构写入状态::候选已撤销, 0, 0, 0});
            return 显式撤销 ? 撤销结果 : 入口拒绝结果();
        }

        for (auto* 参与者 : 参与者组) {
            const auto 准备结果 = 安全准备参与者(会话, *参与者);
            if (!准备结果.成功()) {
                return 撤销并收口(会话, 参与者组, 许可, 准备结果);
            }
        }

        const auto 会话确认 = 会话.完成确认();
        if (!会话确认.成功()) {
            return 撤销并收口(会话, 参与者组, 许可, 会话确认);
        }
        for (auto* 参与者 : 参与者组) {
            const auto 参与者确认 = 安全确认参与者(*参与者);
            if (!参与者确认.成功()) {
                return 撤销并收口(会话, 参与者组, 许可, 参与者确认);
            }
        }

        会话.完成发布();
        for (auto* 参与者 : 参与者组) 完成参与者发布(*参与者);
        const auto 最终状态 = 会话确认.状态 == 节点直接身份结构写入状态::幂等读回
                ? 节点直接身份结构写入状态::幂等读回
                : 节点直接身份结构写入状态::已提交;
        if (最终状态 == 节点直接身份结构写入状态::已提交
            && !事务域_->推进已发布代次(许可)) {
            return {节点直接身份结构写入状态::内部不一致,
                会话确认.结构编号, 会话确认.请求版本, 会话确认.当前版本};
        }
        return {最终状态, 会话确认.结构编号,
            会话确认.请求版本, 会话确认.当前版本};
    }

    template <typename 参与者类型>
    节点直接身份结构写入结果 执行(
        const std::function<void(节点直接身份结构写入会话&)>& 回调,
        参与者类型& 参与者) const {
        static_assert(std::is_base_of_v<节点直接身份结构写入事务参与者, 参与者类型>);
        std::array<节点直接身份结构写入事务参与者*, 1> 参与者组{&参与者};
        return 执行(回调,
            std::span<节点直接身份结构写入事务参与者* const>(参与者组));
    }

private:
    static inline 节点直接身份结构写入结果 入口拒绝结果() noexcept {
        return {节点直接身份结构写入状态::入口拒绝, 0, 0, 0};
    }

    static inline bool 有效参与者组(
        std::span<节点直接身份结构写入事务参与者* const> 参与者组) noexcept {
        for (std::size_t 左序号 = 0; 左序号 < 参与者组.size(); ++左序号) {
            if (参与者组[左序号] == nullptr) return false;
            for (std::size_t 右序号 = 左序号 + 1; 右序号 < 参与者组.size(); ++右序号) {
                if (参与者组[左序号] == 参与者组[右序号]) return false;
            }
        }
        return true;
    }

    static inline 节点直接身份结构写入结果 安全准备参与者(
        const 节点直接身份结构写入会话& 会话,
        节点直接身份结构写入事务参与者& 参与者) noexcept {
        try {
            const 节点直接身份结构提交准备只读视图 视图(会话);
            return 参与者.准备提交(视图);
        } catch (...) {
            return {节点直接身份结构写入状态::内部不一致, 0, 0, 0};
        }
    }

    static inline 节点直接身份结构写入结果 安全确认参与者(
        节点直接身份结构写入事务参与者& 参与者) noexcept {
        try {
            return 参与者.确认待发布();
        } catch (...) {
            return {节点直接身份结构写入状态::内部不一致, 0, 0, 0};
        }
    }

    static inline 节点直接身份结构写入结果 安全撤销参与者(
        节点直接身份结构写入事务参与者& 参与者) noexcept {
        try {
            return 参与者.完成撤销();
        } catch (...) {
            return {节点直接身份结构写入状态::内部不一致, 0, 0, 0};
        }
    }

    static inline void 完成参与者发布(
        节点直接身份结构写入事务参与者& 参与者) noexcept {
        参与者.完成发布();
    }

    static inline bool 逆序撤销参与者(
        std::span<节点直接身份结构写入事务参与者* const> 参与者组) noexcept {
        bool 全部成功 = true;
        for (std::size_t 反向序号 = 参与者组.size(); 反向序号 > 0; --反向序号) {
            const auto 结果 = 安全撤销参与者(*参与者组[反向序号 - 1]);
            全部成功 = 结果.状态 == 节点直接身份结构写入状态::候选已撤销
                && 全部成功;
        }
        return 全部成功;
    }

    static inline 节点直接身份结构写入结果 撤销并收口(
        节点直接身份结构写入会话& 会话,
        std::span<节点直接身份结构写入事务参与者* const> 参与者组,
        节点直接身份结构事务许可& 许可,
        const 节点直接身份结构写入结果& 首次结果) noexcept {
        const bool 参与者撤销成功 = 逆序撤销参与者(参与者组);
        const auto 会话撤销 = 会话.完成撤销();
        if (参与者撤销成功
            && 会话撤销.状态 == 节点直接身份结构写入状态::候选已撤销) {
            return 首次结果;
        }
        许可.标记隔离();
        return {节点直接身份结构写入状态::内部不一致,
            首次结果.结构编号, 首次结果.请求版本, 首次结果.当前版本};
    }

    节点直接身份结构事务域* 事务域_ = nullptr;
    节点直接身份仓库* 节点_ = nullptr;
    正式关系仓库* 关系_ = nullptr;
    可重建索引仓库* 索引_ = nullptr;
};

}
