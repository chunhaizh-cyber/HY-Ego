// 文件规则：节点直接身份结构写入执行器只编排隔离新域独占许可、结构会话和强类型参与者，不承载领域值或业务准入。
module;

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <array>
#include <atomic>
#include <Windows.h>
#include <bcrypt.h>
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iterator>
#include <limits>
#include <memory>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <span>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>
#include "节点直接类型化结构事务.数据.h"

#pragma comment(lib, "bcrypt.lib")

export module 海中鱼巣.核心.执行器.节点直接身份结构写入;

export import 海中鱼巣.核心.会话.节点直接身份结构写入;

import 海中鱼巣.核心.端口.节点直接持久证据;
import 海中鱼巣.核心.仓库.节点直接类型合同;
import 海中鱼巣.核心.仓库.节点直接类型化值;
import 海中鱼巣.核心.仓库.节点直接事务幂等;

export namespace 海中鱼巣 {

class 节点直接身份结构写入执行器;
class 节点直接身份结构事务域;

inline constexpr std::uint32_t 节点直接统一冻结规则版本 = 1;

struct 节点直接统一冻结见证 final {
    std::uint64_t 运行期域身份 = 0;
    std::uint64_t 已发布代次 = 0;
    std::uint32_t 冻结规则版本 = 0;
    friend bool operator==(const 节点直接统一冻结见证&,
        const 节点直接统一冻结见证&) = default;
};

enum class 节点直接统一冻结状态 : std::uint8_t {
    已形成 = 1,
    入口拒绝 = 2,
    许可拒绝 = 3,
    许可竞争 = 4,
    资源失败 = 5,
    未实现 = 6,
    内部不一致 = 7
};

class 节点直接统一冻结许可 final {
public:
    节点直接统一冻结许可() noexcept = default;
    节点直接统一冻结许可(const 节点直接统一冻结许可&) = delete;
    节点直接统一冻结许可& operator=(const 节点直接统一冻结许可&) = delete;
    节点直接统一冻结许可(节点直接统一冻结许可&&) noexcept = default;
    节点直接统一冻结许可& operator=(节点直接统一冻结许可&&) noexcept = default;
    ~节点直接统一冻结许可() noexcept = default;

    bool 有效() const noexcept { return false; }
    节点直接统一冻结见证 读取见证() const noexcept { return {}; }

private:
    friend class 节点直接身份结构事务域;
    struct 私有状态 final {};
    std::unique_ptr<私有状态> 状态_;
};

struct 节点直接统一冻结许可取得结果 final {
    节点直接统一冻结状态 状态 = 节点直接统一冻结状态::未实现;
    std::optional<节点直接统一冻结许可> 许可;
};

enum class 节点直接身份结构事务域启动模式 : std::uint8_t {
    兼容既有直接开放 = 1, 等待恢复 = 2
};

class 节点直接身份结构读取许可 final {
public:
    节点直接身份结构读取许可() = default;
    节点直接身份结构读取许可(const 节点直接身份结构读取许可&) = delete;
    节点直接身份结构读取许可& operator=(const 节点直接身份结构读取许可&) = delete;
    节点直接身份结构读取许可(节点直接身份结构读取许可&&) noexcept = default;
    节点直接身份结构读取许可& operator=(节点直接身份结构读取许可&&) noexcept = default;

    bool 有效() const noexcept {
        return 锁_.owns_lock() && 普通入口开放_->load(std::memory_order_acquire)
            && !隔离标记_->load(std::memory_order_acquire);
    }

    std::uint64_t 读取已发布代次() const noexcept {
        return 有效() ? 当前已发布代次_->load(std::memory_order_acquire) : 0;
    }

private:
    friend class 节点直接身份结构事务域;

    节点直接身份结构读取许可(
        std::shared_ptr<std::shared_mutex> 互斥,
        std::shared_ptr<std::atomic_bool> 隔离标记,
        std::shared_ptr<std::atomic_bool> 普通入口开放,
        std::shared_ptr<std::atomic_uint64_t> 当前已发布代次)
        : 隔离标记_(std::move(隔离标记)), 普通入口开放_(std::move(普通入口开放)),
          当前已发布代次_(std::move(当前已发布代次)), 锁_(*互斥, std::try_to_lock),
          互斥_(std::move(互斥)) {
        if (锁_.owns_lock() && (!普通入口开放_->load(std::memory_order_acquire)
                || 隔离标记_->load(std::memory_order_acquire))) 锁_.unlock();
    }

    std::shared_ptr<std::atomic_bool> 隔离标记_ = std::make_shared<std::atomic_bool>(true);
    std::shared_ptr<std::atomic_bool> 普通入口开放_ = std::make_shared<std::atomic_bool>(false);
    std::shared_ptr<std::atomic_uint64_t> 当前已发布代次_ = std::make_shared<std::atomic_uint64_t>(0);
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
    std::uint64_t 读取已发布代次() const noexcept {
        return 有效() ? 当前已发布代次_->load(std::memory_order_acquire) : 0;
    }

private:
    friend class 节点直接身份结构事务域;
    friend class 节点直接身份结构写入执行器;

    节点直接身份结构事务许可(
        std::shared_ptr<std::shared_mutex> 互斥,
        std::shared_ptr<std::atomic_bool> 隔离标记,
        std::shared_ptr<std::atomic_uint64_t> 当前已发布代次,
        std::uint64_t 事务序号)
        : 隔离标记_(std::move(隔离标记)), 锁_(*互斥, std::try_to_lock),
          互斥_(std::move(互斥)), 当前已发布代次_(std::move(当前已发布代次)),
          事务序号_(锁_.owns_lock() ? 事务序号 : 0) {
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
    std::shared_ptr<std::atomic_uint64_t> 当前已发布代次_ = std::make_shared<std::atomic_uint64_t>(0);
    std::uint64_t 事务序号_ = 0;
};

class 节点直接身份结构事务域 final {
public:
    explicit 节点直接身份结构事务域(
        节点直接身份结构事务域启动模式 模式 = 节点直接身份结构事务域启动模式::兼容既有直接开放)
        : 互斥_(std::make_shared<std::shared_mutex>()),
          隔离标记_(std::make_shared<std::atomic_bool>(false)),
          普通入口开放_(std::make_shared<std::atomic_bool>(
              模式 == 节点直接身份结构事务域启动模式::兼容既有直接开放)),
          当前已发布代次_(std::make_shared<std::atomic_uint64_t>(0)) {
    }

    节点直接身份结构事务域(const 节点直接身份结构事务域&) = delete;
    节点直接身份结构事务域& operator=(const 节点直接身份结构事务域&) = delete;

    节点直接身份结构读取许可 取得读取许可() const {
        return 节点直接身份结构读取许可(
            互斥_, 隔离标记_, 普通入口开放_, 当前已发布代次_);
    }

    bool 已隔离() const noexcept {
        return 隔离标记_->load(std::memory_order_acquire);
    }

    节点直接统一冻结许可取得结果 取得统一冻结许可(
        std::uint32_t) const {
        return {};
    }

    bool 冻结许可属于本域(
        const 节点直接统一冻结许可&) const noexcept {
        return false;
    }

private:
    friend class 节点直接身份结构写入执行器;

    节点直接身份结构事务许可 取得独占许可() {
        if (已隔离() || !普通入口开放_->load(std::memory_order_acquire)) return {};
        const auto 事务序号 = 下一事务序号_.fetch_add(1, std::memory_order_relaxed);
        return 节点直接身份结构事务许可(
            互斥_, 隔离标记_, 当前已发布代次_, 事务序号);
    }

    节点直接身份结构事务许可 取得恢复独占许可() {
        if (已隔离() || 普通入口开放_->load(std::memory_order_acquire)) return {};
        const auto 事务序号 = 下一事务序号_.fetch_add(1, std::memory_order_relaxed);
        return 节点直接身份结构事务许可(
            互斥_, 隔离标记_, 当前已发布代次_, 事务序号);
    }

    bool 推进普通已发布代次(
        节点直接身份结构事务许可& 许可,
        std::uint64_t 预期当前代次) noexcept {
        if (!许可.有效() || !普通入口开放_->load(std::memory_order_acquire)
            || 预期当前代次 == std::numeric_limits<std::uint64_t>::max()) return false;
        return 当前已发布代次_->compare_exchange_strong(
            预期当前代次, 预期当前代次 + 1,
            std::memory_order_acq_rel, std::memory_order_acquire);
    }

    bool 从零设置恢复事实截止代次并开放(
        节点直接身份结构事务许可& 许可,
        std::uint64_t 恢复事实截止代次) noexcept {
        if (!许可.有效() || 普通入口开放_->load(std::memory_order_acquire)
            || 恢复事实截止代次 == 0) return false;
        std::uint64_t 零 = 0;
        if (!当前已发布代次_->compare_exchange_strong(
                零, 恢复事实截止代次,
                std::memory_order_acq_rel, std::memory_order_acquire)) return false;
        普通入口开放_->store(true, std::memory_order_release);
        return true;
    }

    std::shared_ptr<std::shared_mutex> 互斥_;
    std::shared_ptr<std::atomic_bool> 隔离标记_;
    std::shared_ptr<std::atomic_bool> 普通入口开放_;
    std::shared_ptr<std::atomic_uint64_t> 当前已发布代次_;
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

enum class 节点直接类型化结构事务参与者状态 : std::uint8_t {
    已准备 = 1,
    已确认待发布 = 2,
    已撤销 = 3,
    已完成发布 = 4,
    入口拒绝 = 5,
    版本漂移 = 6,
    许可拒绝 = 7,
    资源失败 = 8,
    内部不一致 = 9,
    第一写前已复核 = 10
};

struct 节点直接类型化结构事务参与者结果 {
    节点直接类型化结构事务参与者状态 状态 =
        节点直接类型化结构事务参与者状态::内部不一致;
};

using 节点直接类型化结构计划稳定见证 = std::variant<
    节点稳定身份见证,
    关系稳定身份见证,
    类型化值读回>;

struct 节点直接类型化结构计划身份见证项 {
    节点直接写集局部身份 局部身份;
    节点直接类型化结构计划稳定见证 稳定见证;
};

struct 节点直接类型化结构候选读回 {
    std::vector<节点稳定身份见证> 节点组;
    std::vector<关系稳定身份见证> 关系组;
    std::vector<类型化值读回> 类型化值组;
    std::vector<可重建索引读回> 索引组;
};

class 节点直接类型化结构第一写前复核只读视图 final {
public:
    std::uint64_t 读取事务序号() const noexcept { return 事务序号_; }
    std::uint64_t 读取当前事实截止代次() const noexcept { return 当前事实截止代次_; }
    std::uint64_t 读取目标发布代次() const noexcept { return 目标发布代次_; }
    const 节点直接类型化结构数据操作请求& 读取请求() const noexcept { return *请求_; }

private:
    friend class 节点直接身份结构写入执行器;

    节点直接类型化结构第一写前复核只读视图(
        std::uint64_t 事务序号,
        std::uint64_t 当前事实截止代次,
        std::uint64_t 目标发布代次,
        const 节点直接类型化结构数据操作请求& 请求) noexcept
        : 事务序号_(事务序号), 当前事实截止代次_(当前事实截止代次),
          目标发布代次_(目标发布代次), 请求_(&请求) {}

    std::uint64_t 事务序号_ = 0;
    std::uint64_t 当前事实截止代次_ = 0;
    std::uint64_t 目标发布代次_ = 0;
    const 节点直接类型化结构数据操作请求* 请求_ = nullptr;
};

class 节点直接类型化结构提交准备只读视图 final {
public:
    std::uint64_t 读取事务序号() const noexcept { return 事务序号_; }
    std::uint64_t 读取当前事实截止代次() const noexcept { return 当前事实截止代次_; }
    std::uint64_t 读取目标发布代次() const noexcept { return 目标发布代次_; }
    const 节点直接类型化结构数据操作请求& 读取请求() const noexcept { return *请求_; }
    std::span<const 节点直接类型化结构计划身份见证项>
        读取计划身份见证组() const noexcept { return 计划身份见证组_; }
    std::optional<节点直接类型化结构计划稳定见证> 读取计划身份见证(
        节点直接写集局部身份 局部身份) const {
        const auto 位置 = std::find_if(计划身份见证组_.begin(), 计划身份见证组_.end(),
            [&](const auto& 项) { return 项.局部身份 == 局部身份; });
        return 位置 == 计划身份见证组_.end()
            ? std::nullopt
            : std::optional<节点直接类型化结构计划稳定见证>{位置->稳定见证};
    }
    const 节点直接类型化结构候选读回& 读取候选读回() const noexcept {
        return *候选读回_;
    }

private:
    friend class 节点直接身份结构写入执行器;

    节点直接类型化结构提交准备只读视图(
        std::uint64_t 事务序号,
        std::uint64_t 当前事实截止代次,
        std::uint64_t 目标发布代次,
        const 节点直接类型化结构数据操作请求& 请求,
        std::span<const 节点直接类型化结构计划身份见证项> 计划身份见证组,
        const 节点直接类型化结构候选读回& 候选读回) noexcept
        : 事务序号_(事务序号), 当前事实截止代次_(当前事实截止代次),
          目标发布代次_(目标发布代次), 请求_(&请求),
          计划身份见证组_(计划身份见证组), 候选读回_(&候选读回) {}

    std::uint64_t 事务序号_ = 0;
    std::uint64_t 当前事实截止代次_ = 0;
    std::uint64_t 目标发布代次_ = 0;
    const 节点直接类型化结构数据操作请求* 请求_ = nullptr;
    std::span<const 节点直接类型化结构计划身份见证项> 计划身份见证组_;
    const 节点直接类型化结构候选读回* 候选读回_ = nullptr;
};

class 节点直接类型化结构事务参与者 {
public:
    virtual ~节点直接类型化结构事务参与者() = default;

protected:
    节点直接类型化结构事务参与者() = default;

private:
    friend class 节点直接身份结构写入执行器;

    virtual 节点直接类型化结构事务参与者结果 第一写前复核(
        const 节点直接类型化结构第一写前复核只读视图& 视图) = 0;
    virtual 节点直接类型化结构事务参与者结果 准备提交(
        const 节点直接类型化结构提交准备只读视图& 视图) = 0;
    virtual 节点直接类型化结构事务参与者结果 确认待发布() = 0;
    virtual 节点直接类型化结构事务参与者结果 完成撤销() = 0;
    virtual 节点直接类型化结构事务参与者结果 完成发布() = 0;
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
        return {会话确认.状态 == 节点直接身份结构写入状态::幂等读回
                ? 节点直接身份结构写入状态::幂等读回
                : 节点直接身份结构写入状态::已提交,
            会话确认.结构编号, 会话确认.请求版本, 会话确认.当前版本};
    }

    inline 节点直接身份结构写入执行器(
        节点直接身份结构事务域* 事务域,
        节点直接身份仓库* 节点,
        正式关系仓库* 关系,
        可重建索引仓库* 索引,
        节点直接类型合同仓库* 类型合同,
        节点直接类型化值仓库* 类型化值,
        节点直接事务幂等仓库* 幂等,
        节点直接持久证据写入端口* 持久证据) noexcept
        : 事务域_(事务域), 节点_(节点), 关系_(关系), 索引_(索引),
          类型合同_(类型合同), 类型化值_(类型化值), 幂等_(幂等),
          持久证据_(持久证据) {}

    bool 类型化结构有效() const noexcept {
        return 有效() && 类型合同_ != nullptr && 类型化值_ != nullptr
            && 幂等_ != nullptr && 持久证据_ != nullptr;
    }

    节点直接幂等探测数据操作结果 探测节点直接类型化结构幂等(
        节点直接事务幂等身份 幂等身份,
        const std::array<std::uint8_t, 32>& 请求意图摘要) const {
        节点直接幂等探测数据操作结果 结果;
        if (!类型化结构有效() || !节点直接事务幂等身份完整(幂等身份)
            || !摘要非零_(请求意图摘要)) return 结果;
        auto 许可 = 事务域_->取得读取许可();
        if (!许可.有效()) {
            结果.状态 = 节点直接幂等探测数据操作状态::许可拒绝;
            return 结果;
        }
        const auto 记录 = 幂等_->读取(幂等身份);
        if (!记录) {
            结果.状态 = 节点直接幂等探测数据操作状态::未找到;
            return 结果;
        }
        if (记录->请求意图摘要 != 请求意图摘要) {
            结果.状态 = 节点直接幂等探测数据操作状态::异义冲突;
            return 结果;
        }
        if (记录->状态 != 节点直接事务幂等记录状态::已发布) {
            结果.状态 = 节点直接幂等探测数据操作状态::待发布或隔离;
            return 结果;
        }
        结果.状态 = 节点直接幂等探测数据操作状态::同义已发布;
        结果.原结果 = 形成幂等读回_(*记录);
        return 结果;
    }

    节点直接类型化结构数据操作结果 执行节点直接类型化结构事务(
        const 节点直接类型化结构数据操作规格& 规格) const {
        const std::span<节点直接类型化结构事务参与者* const> 空参与者组;
        return 执行节点直接类型化结构事务(规格, 空参与者组);
    }

    节点直接类型化结构数据操作结果 执行节点直接类型化结构事务(
        const 节点直接类型化结构数据操作规格& 规格,
        std::span<节点直接类型化结构事务参与者* const> 参与者组) const {
        return 执行带参与者类型化结构事务_(规格.请求, false, 参与者组);
    }

    节点直接持久见证数据操作结果 执行持久发布见证重试(
        节点直接事务幂等身份 幂等身份) const {
        节点直接持久见证数据操作结果 结果;
        if (!类型化结构有效()) return 结果;
        auto 许可 = 事务域_->取得独占许可();
        if (!许可.有效()) {
            结果.状态 = 节点直接持久见证数据操作状态::许可拒绝;
            return 结果;
        }
        const auto 记录 = 幂等_->读取(幂等身份);
        if (!记录) {
            结果.状态 = 节点直接持久见证数据操作状态::未找到;
            return 结果;
        }
        const auto 侧账 = 幂等_->读取持久证据状态(幂等身份);
        if (侧账 == 持久证据状态::已与内存代次一致) {
            结果.状态 = 节点直接持久见证数据操作状态::无需重试;
            结果.持久状态 = 侧账;
            return 结果;
        }
        const auto 侧账记录 = 幂等_->读取持久证据侧账(幂等身份);
        if (!侧账记录 || 侧账记录->尝试序号 == 0
            || 记录->状态 != 节点直接事务幂等记录状态::已发布) {
            许可.标记隔离();
            结果.状态 = 节点直接持久见证数据操作状态::内部不一致;
            结果.持久状态 = 侧账;
            return 结果;
        }
        节点直接持久发布见证请求 请求;
        请求.安装实例身份 = 记录->安装实例身份;
        请求.事务身份 = 记录->幂等身份;
        请求.尝试序号 = 侧账记录->尝试序号;
        请求.请求意图摘要 = 记录->请求意图摘要;
        请求.执行证据摘要 = 记录->执行证据摘要;
        请求.发布代次 = 记录->发布代次;
        请求.结果摘要 = 记录->结果摘要;
        const auto 持久结果 = 持久证据_->标记已发布(请求);
        if ((持久结果.状态 == 节点直接持久端口状态::已见证
                || 持久结果.状态 == 节点直接持久端口状态::精确同义)
            && 持久结果.尝试序号 == 侧账记录->尝试序号
            && 幂等_->单调记录持久证据状态(
                幂等身份, 侧账记录->尝试序号, 持久证据状态::已与内存代次一致)) {
            结果.状态 = 节点直接持久见证数据操作状态::已与内存代次一致;
            结果.持久状态 = 持久证据状态::已与内存代次一致;
            return 结果;
        }
        if (持久结果.状态 == 节点直接持久端口状态::结果未知) {
            (void)幂等_->单调记录持久证据状态(
                幂等身份, 侧账记录->尝试序号, 持久证据状态::持久化结果未知);
            结果.状态 = 节点直接持久见证数据操作状态::资源失败;
            结果.持久状态 = 持久证据状态::持久化结果未知;
            return 结果;
        }
        if (持久结果.状态 == 节点直接持久端口状态::损坏
            || 持久结果.状态 == 节点直接持久端口状态::冲突) {
            (void)幂等_->单调记录持久证据状态(
                幂等身份, 侧账记录->尝试序号, 持久证据状态::持久证据损坏);
            许可.标记隔离();
            结果.状态 = 节点直接持久见证数据操作状态::持久证据损坏;
            结果.持久状态 = 持久证据状态::持久证据损坏;
            return 结果;
        }
        结果.状态 = 节点直接持久见证数据操作状态::资源失败;
        结果.持久状态 = 侧账;
        return 结果;
    }

    节点直接类型化结构数据操作结果 初始化确认空域治理代次(
        节点直接事务幂等身份 安装实例身份) const {
        节点直接类型化结构数据操作请求 请求;
        请求.安装实例身份 = 安装实例身份;
        请求.幂等身份 = {static_cast<std::uint64_t>(节点直接事务命名域::安装实例初始化),
            安装实例身份.键值};
        请求.请求意图摘要.fill(1);
        请求.执行证据摘要.fill(2);
        请求.预期事实截止代次 = 0;
        const std::span<节点直接类型化结构事务参与者* const> 空参与者组;
        return 执行带参与者类型化结构事务_(请求, true, 空参与者组);
    }

    节点直接恢复数据操作结果 执行节点直接恢复事务(
        const 节点直接恢复数据操作规格& 规格) const {
        节点直接恢复数据操作结果 结果;
        if (!有效() || 规格.请求.合同版本 != 1 || 规格.请求.材料格式版本 != 2
            || !节点直接事务幂等身份完整(规格.请求.安装实例身份)
            || 规格.请求.恢复事实截止代次 == 0) return 结果;
        auto 许可 = 事务域_->取得恢复独占许可();
        if (!许可.有效() || 许可.读取已发布代次() != 0) {
            结果.状态 = 节点直接恢复数据操作状态::版本漂移;
            return 结果;
        }

        auto 节点键小于 = [](节点稳定主键 左, 节点稳定主键 右) {
            return 左.命名域 != 右.命名域 ? 左.命名域 < 右.命名域 : 左.键值 < 右.键值;
        };
        auto 关系键小于 = [](关系稳定主键 左, 关系稳定主键 右) {
            return 左.命名域 != 右.命名域 ? 左.命名域 < 右.命名域 : 左.键值 < 右.键值;
        };
        try {
            节点直接身份仓库权威材料 节点材料;
            节点材料.仓库编号 = 节点_->仓库编号();
            std::vector<std::pair<节点稳定主键, 节点类型>> 节点键组;
            for (const auto& 占用 : 规格.请求.历史占用组) {
                if (占用.种类 != 节点直接恢复占用种类::节点) continue;
                节点稳定主键 键{占用.命名域, 占用.键值};
                if (!稳定主键有效(键) || 占用.节点类型见证 == 节点类型::未分类) {
                    结果.状态 = 节点直接恢复数据操作状态::材料不完整; return 结果;
                }
                节点键组.push_back({键, 占用.节点类型见证});
            }
            for (const auto& 历史 : 规格.请求.节点历史组) {
                if (!节点稳定身份见证完整(历史.身份)) {
                    结果.状态 = 节点直接恢复数据操作状态::材料不完整; return 结果;
                }
                节点键组.push_back({历史.身份.稳定主键, 历史.身份.类型});
            }
            std::sort(节点键组.begin(), 节点键组.end(), [&](const auto& 左, const auto& 右) {
                return 节点键小于(左.first, 右.first);
            });
            std::vector<std::pair<节点稳定主键, 节点类型>> 唯一节点键组;
            for (const auto& 项 : 节点键组) {
                if (!唯一节点键组.empty() && 唯一节点键组.back().first == 项.first) {
                    if (唯一节点键组.back().second != 项.second) {
                        结果.状态 = 节点直接恢复数据操作状态::身份冲突; return 结果;
                    }
                } else 唯一节点键组.push_back(项);
            }
            if (唯一节点键组.size() == std::numeric_limits<std::uint64_t>::max()) {
                结果.状态 = 节点直接恢复数据操作状态::版本漂移; return 结果;
            }
            std::vector<节点直接恢复节点内部投影> 节点投影组;
            for (std::size_t 序号 = 0; 序号 < 唯一节点键组.size(); ++序号) {
                const auto& [键, 类型] = 唯一节点键组[序号];
                std::vector<节点直接恢复节点记录> 历史组;
                for (const auto& 历史 : 规格.请求.节点历史组)
                    if (历史.身份.稳定主键 == 键) 历史组.push_back(历史);
                std::sort(历史组.begin(), 历史组.end(), [](const auto& 左, const auto& 右) {
                    return 左.身份.身份版本 < 右.身份.身份版本;
                });
                for (std::size_t 版本序号 = 0; 版本序号 < 历史组.size(); ++版本序号) {
                    if (历史组[版本序号].身份.身份版本 != 版本序号 + 1
                        || 历史组[版本序号].身份.类型 != 类型
                        || (版本序号 + 1 < 历史组.size() && 历史组[版本序号].当前有效)) {
                        结果.状态 = 节点直接恢复数据操作状态::版本漂移; return 结果;
                    }
                }
                const auto 本地编号 = static_cast<std::uint64_t>(序号 + 1);
                节点材料.历史占用.push_back({键, 类型, 本地编号, true});
                if (!历史组.empty()) {
                    const auto& 最大版本 = 历史组.back();
                    节点材料.记录组.push_back({本地编号, 键, 类型, 最大版本.身份.身份版本,
                        最大版本.当前有效 ? 记录状态::有效 : 记录状态::已删除, 本地编号});
                    节点投影组.push_back({最大版本.身份,
                        {节点材料.仓库编号, 本地编号, 最大版本.身份.身份版本}, 最大版本.当前有效});
                }
            }
            节点材料.下个节点编号 = static_cast<std::uint64_t>(唯一节点键组.size() + 1);
            节点材料.下个创建序号 = 节点材料.下个节点编号;
            for (const auto& 高水位 : 规格.请求.高水位组)
                if (高水位.种类 == 节点直接恢复占用种类::节点)
                    节点材料.每域高水位.push_back({高水位.命名域, 高水位.高水位});

            auto 解析节点 = [&](const 节点稳定身份见证& 身份) -> std::optional<节点句柄> {
                const auto 位置 = std::find_if(节点投影组.begin(), 节点投影组.end(),
                    [&](const auto& 项) { return 项.身份 == 身份; });
                return 位置 == 节点投影组.end() ? std::nullopt
                    : std::optional<节点句柄>{位置->内部句柄};
            };

            节点直接类型合同仓库权威材料 合同材料;
            合同材料.记录组 = 规格.请求.类型合同历史组;
            for (const auto& 高水位 : 规格.请求.高水位组)
                if (高水位.种类 == 节点直接恢复占用种类::类型合同)
                    合同材料.每域高水位.push_back({高水位.命名域, 高水位.高水位});
            for (const auto& 占用 : 规格.请求.历史占用组)
                if (占用.种类 == 节点直接恢复占用种类::类型合同)
                    合同材料.历史占用.push_back({{占用.命名域, 占用.键值}});

            节点直接类型化值仓库权威材料 值材料;
            值材料.记录组 = 规格.请求.类型化值历史组;
            for (const auto& 高水位 : 规格.请求.高水位组)
                if (高水位.种类 == 节点直接恢复占用种类::类型化值记录)
                    值材料.每域高水位.push_back({高水位.命名域, 高水位.高水位});
            for (const auto& 占用 : 规格.请求.历史占用组)
                if (占用.种类 == 节点直接恢复占用种类::类型化值记录)
                    值材料.历史占用.push_back({{占用.命名域, 占用.键值}});

            正式关系仓库权威材料 关系材料;
            for (const auto& 高水位 : 规格.请求.高水位组)
                if (高水位.种类 == 节点直接恢复占用种类::关系)
                    关系材料.每域高水位.push_back({static_cast<关系稳定主键命名域>(高水位.命名域), 高水位.高水位});
            for (const auto& 占用 : 规格.请求.历史占用组)
                if (占用.种类 == 节点直接恢复占用种类::关系)
                    关系材料.历史占用.push_back({{占用.命名域, 占用.键值}});
            auto 关系历史组 = 规格.请求.关系历史组;
            std::sort(关系历史组.begin(), 关系历史组.end(), [&](const auto& 左, const auto& 右) {
                if (左.身份.稳定主键 != 右.身份.稳定主键)
                    return 关系键小于(左.身份.稳定主键, 右.身份.稳定主键);
                return 左.身份.关系版本 < 右.身份.关系版本;
            });
            std::vector<std::pair<关系稳定主键, std::uint64_t>> 关系编号映射;
            for (const auto& 历史 : 关系历史组) {
                auto 映射 = std::find_if(关系编号映射.begin(), 关系编号映射.end(),
                    [&](const auto& 项) { return 项.first == 历史.身份.稳定主键; });
                if (映射 == 关系编号映射.end()) {
                    关系编号映射.push_back({历史.身份.稳定主键, 关系编号映射.size() + 1});
                    映射 = std::prev(关系编号映射.end());
                }
                const auto 源 = 解析节点(历史.身份.源端);
                const auto 目标 = 解析节点(历史.身份.目标端);
                if (!源 || !目标) { 结果.状态 = 节点直接恢复数据操作状态::关系矛盾; return 结果; }
                关系材料.记录组.push_back({历史.身份.稳定主键, 映射->second, 历史.身份.类型,
                    *源, *目标, 历史.身份.角色或顺序, 历史.身份.关系版本,
                    历史.当前有效 ? 记录状态::有效 : 记录状态::已失效});
            }
            auto 解析关系 = [&](const 关系稳定身份见证& 身份) -> std::optional<关系句柄> {
                const auto 映射 = std::find_if(关系编号映射.begin(), 关系编号映射.end(),
                    [&](const auto& 项) { return 项.first == 身份.稳定主键; });
                return 映射 == 关系编号映射.end() ? std::nullopt
                    : std::optional<关系句柄>{{关系_->仓库编号(), 映射->second, 身份.关系版本}};
            };

            可重建索引权威材料 索引材料;
            for (const auto& 索引 : 规格.请求.索引历史组) {
                if (!索引.当前) continue;
                if (索引.节点目标) {
                    const auto 句柄 = 解析节点(*索引.节点目标);
                    if (!句柄) { 结果.状态 = 节点直接恢复数据操作状态::关系矛盾; return 结果; }
                    索引材料.记录组.push_back({索引.键, 索引目标种类::节点, *句柄, {}});
                } else if (索引.关系目标) {
                    const auto 句柄 = 解析关系(*索引.关系目标);
                    if (!句柄) { 结果.状态 = 节点直接恢复数据操作状态::关系矛盾; return 结果; }
                    索引材料.记录组.push_back({索引.键, 索引目标种类::关系, {}, *句柄});
                } else { 结果.状态 = 节点直接恢复数据操作状态::材料不完整; return 结果; }
            }

            节点直接事务幂等仓库恢复材料 幂等材料;
            幂等材料.记录组 = 规格.请求.幂等历史组;
            for (const auto& 侧账 : 规格.请求.持久证据状态组)
                幂等材料.持久证据侧账组.push_back({侧账.幂等身份, 侧账.尝试序号, 侧账.状态});

            auto 节点建立 = 节点_->结构化建立恢复未发布候选(节点材料, 许可.事务序号());
            if (!节点建立.候选) { 结果.状态 = 节点直接恢复数据操作状态::身份冲突; return 结果; }
            auto 合同建立 = 类型合同_->结构化建立恢复未发布候选(std::move(合同材料), 许可.事务序号());
            if (!合同建立.候选) { (void)节点_->撤销恢复候选(*节点建立.候选, 许可.事务序号()); 结果.状态 = 节点直接恢复数据操作状态::身份冲突; return 结果; }
            auto 值建立 = 类型化值_->结构化建立恢复未发布候选(std::move(值材料), 许可.事务序号());
            if (!值建立.候选) { (void)类型合同_->撤销恢复候选(*合同建立.候选, 许可.事务序号()); (void)节点_->撤销恢复候选(*节点建立.候选, 许可.事务序号()); 结果.状态 = 节点直接恢复数据操作状态::身份冲突; return 结果; }
            auto 关系建立 = 关系_->结构化建立恢复未发布候选(std::move(关系材料), 许可.事务序号());
            if (!关系建立.候选) { (void)类型化值_->撤销恢复候选(*值建立.候选, 许可.事务序号()); (void)类型合同_->撤销恢复候选(*合同建立.候选, 许可.事务序号()); (void)节点_->撤销恢复候选(*节点建立.候选, 许可.事务序号()); 结果.状态 = 节点直接恢复数据操作状态::关系矛盾; return 结果; }
            auto 索引建立 = 索引_->结构化建立恢复未发布候选(std::move(索引材料), 许可.事务序号());
            if (!索引建立.候选) { (void)关系_->撤销恢复候选(*关系建立.候选, 许可.事务序号()); (void)类型化值_->撤销恢复候选(*值建立.候选, 许可.事务序号()); (void)类型合同_->撤销恢复候选(*合同建立.候选, 许可.事务序号()); (void)节点_->撤销恢复候选(*节点建立.候选, 许可.事务序号()); 结果.状态 = 节点直接恢复数据操作状态::关系矛盾; return 结果; }
            auto 幂等建立 = 幂等_->结构化建立恢复未发布候选(std::move(幂等材料), 许可.事务序号());
            if (!幂等建立.候选) { (void)索引_->撤销恢复候选(*索引建立.候选, 许可.事务序号()); (void)关系_->撤销恢复候选(*关系建立.候选, 许可.事务序号()); (void)类型化值_->撤销恢复候选(*值建立.候选, 许可.事务序号()); (void)类型合同_->撤销恢复候选(*合同建立.候选, 许可.事务序号()); (void)节点_->撤销恢复候选(*节点建立.候选, 许可.事务序号()); 结果.状态 = 节点直接恢复数据操作状态::材料不完整; return 结果; }

            const bool 确认完整 = 节点_->确认恢复候选(*节点建立.候选, 许可.事务序号()) == 节点直接恢复仓操作状态::已确认待发布
                && 类型合同_->确认恢复候选(*合同建立.候选, 许可.事务序号()) == 节点直接恢复仓操作状态::已确认待发布
                && 类型化值_->确认恢复候选(*值建立.候选, 许可.事务序号()) == 节点直接恢复仓操作状态::已确认待发布
                && 关系_->确认恢复候选(*关系建立.候选, 许可.事务序号()) == 节点直接恢复仓操作状态::已确认待发布
                && 索引_->确认恢复候选(*索引建立.候选, 许可.事务序号()) == 节点直接恢复仓操作状态::已确认待发布
                && 幂等_->确认恢复候选(*幂等建立.候选, 许可.事务序号()) == 节点直接恢复仓操作状态::已确认待发布;
            if (!确认完整) {
                (void)幂等_->撤销恢复候选(*幂等建立.候选, 许可.事务序号());
                (void)索引_->撤销恢复候选(*索引建立.候选, 许可.事务序号());
                (void)关系_->撤销恢复候选(*关系建立.候选, 许可.事务序号());
                (void)类型化值_->撤销恢复候选(*值建立.候选, 许可.事务序号());
                (void)类型合同_->撤销恢复候选(*合同建立.候选, 许可.事务序号());
                (void)节点_->撤销恢复候选(*节点建立.候选, 许可.事务序号());
                结果.状态 = 节点直接恢复数据操作状态::内部不一致; return 结果;
            }
            const bool 发布完整 = 节点_->完成发布恢复候选(*节点建立.候选, 许可.事务序号()) == 节点直接恢复仓操作状态::已发布
                && 类型合同_->完成发布恢复候选(*合同建立.候选, 许可.事务序号()) == 节点直接恢复仓操作状态::已发布
                && 类型化值_->完成发布恢复候选(*值建立.候选, 许可.事务序号()) == 节点直接恢复仓操作状态::已发布
                && 关系_->完成发布恢复候选(*关系建立.候选, 许可.事务序号()) == 节点直接恢复仓操作状态::已发布
                && 索引_->完成发布恢复候选(*索引建立.候选, 许可.事务序号()) == 节点直接恢复仓操作状态::已发布
                && 幂等_->完成发布恢复候选(*幂等建立.候选, 许可.事务序号()) == 节点直接恢复仓操作状态::已发布;
            if (!发布完整 || !事务域_->从零设置恢复事实截止代次并开放(
                    许可, 规格.请求.恢复事实截止代次)) {
                许可.标记隔离();
                结果.状态 = 节点直接恢复数据操作状态::内部不一致; return 结果;
            }
            结果.状态 = 节点直接恢复数据操作状态::已恢复;
            结果.事实截止代次 = 规格.请求.恢复事实截止代次;
            return 结果;
        } catch (...) {
            许可.标记隔离();
            结果.状态 = 节点直接恢复数据操作状态::资源失败;
            return 结果;
        }
    }

    inline 节点直接身份结构写入结果 执行仅参与者事务(
        std::span<节点直接身份结构写入事务参与者* const> 参与者组) const {
        if (!有效() || 参与者组.empty() || !有效参与者组(参与者组)) {
            return 入口拒绝结果();
        }
        auto 许可 = 事务域_->取得独占许可();
        if (!许可.有效()) {
            return {节点直接身份结构写入状态::许可拒绝, 0, 0, 0};
        }

        节点直接身份结构写入会话 会话(*节点_, *关系_, *索引_, 许可.事务序号());
        会话.请求仅参与者提交();
        if (会话.有失败() || !会话.已请求提交()) {
            return 撤销并收口(
                会话, 参与者组, 许可,
                会话.有失败() ? 会话.读取首次失败()
                              : 节点直接身份结构写入结果{
                                    节点直接身份结构写入状态::内部不一致, 0, 0, 0});
        }

        bool 全部幂等读回 = true;
        for (auto* 参与者 : 参与者组) {
            const auto 准备结果 = 安全准备参与者(会话, *参与者);
            if (准备结果.状态 != 节点直接身份结构写入状态::已提交
                && 准备结果.状态 != 节点直接身份结构写入状态::幂等读回) {
                const auto 首次结果 = 准备结果.成功()
                    ? 节点直接身份结构写入结果{
                          节点直接身份结构写入状态::内部不一致, 0, 0, 0}
                    : 准备结果;
                return 撤销并收口(会话, 参与者组, 许可, 首次结果);
            }
            全部幂等读回 = 准备结果.状态 == 节点直接身份结构写入状态::幂等读回
                && 全部幂等读回;
        }

        const auto 会话确认 = 会话.完成仅参与者确认();
        if (会话确认.状态 != 节点直接身份结构写入状态::候选已确认) {
            return 撤销并收口(会话, 参与者组, 许可, 会话确认);
        }
        for (auto* 参与者 : 参与者组) {
            const auto 参与者确认 = 安全确认参与者(*参与者);
            if (参与者确认.状态 != 节点直接身份结构写入状态::候选已确认) {
                const auto 首次结果 = 参与者确认.成功()
                    ? 节点直接身份结构写入结果{
                          节点直接身份结构写入状态::内部不一致, 0, 0, 0}
                    : 参与者确认;
                return 撤销并收口(会话, 参与者组, 许可, 首次结果);
            }
        }

        会话.完成发布();
        for (auto* 参与者 : 参与者组) 完成参与者发布(*参与者);
        return {全部幂等读回 ? 节点直接身份结构写入状态::幂等读回
                             : 节点直接身份结构写入状态::已提交,
            0, 0, 0};
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
    static constexpr std::size_t 最大写集材料字节数_ = 67'108'864;
    static constexpr std::size_t 最大向量项目数_ = 1'048'576;

    static bool 追加字节_(std::vector<std::uint8_t>& 输出, std::uint8_t 值) {
        if (输出.size() == 最大写集材料字节数_) return false;
        输出.push_back(值);
        return true;
    }

    static bool 追加无符号_(std::vector<std::uint8_t>& 输出, std::uint64_t 值, std::size_t 字节数) {
        if (字节数 > 8 || 输出.size() > 最大写集材料字节数_ - 字节数) return false;
        for (std::size_t 序号 = 字节数; 序号 > 0; --序号)
            输出.push_back(static_cast<std::uint8_t>(值 >> ((序号 - 1) * 8)));
        return true;
    }

    static bool 追加稳定主键_(std::vector<std::uint8_t>& 输出, 节点稳定主键 值) {
        return 追加无符号_(输出, 值.命名域, 8) && 追加无符号_(输出, 值.键值, 8);
    }

    static bool 追加节点见证_(std::vector<std::uint8_t>& 输出, const 节点稳定身份见证& 值) {
        return 追加稳定主键_(输出, 值.稳定主键)
            && 追加无符号_(输出, static_cast<std::uint64_t>(值.类型), 4)
            && 追加无符号_(输出, 值.身份版本, 4);
    }

    static bool 追加索引键_(std::vector<std::uint8_t>& 输出, const 索引物理键& 值) {
        return 追加无符号_(输出, 值.所有者身份, 8)
            && 追加无符号_(输出, 值.命名域, 4)
            && 追加无符号_(输出, 值.键格式版本, 4)
            && 追加无符号_(输出, 值.探测规则版本, 4)
            && 追加无符号_(输出, 值.键值, 8);
    }

    static bool 追加端点_(std::vector<std::uint8_t>& 输出, const 节点直接节点端点引用& 值) {
        return std::visit([&](const auto& 项) -> bool {
            using 类型 = std::decay_t<decltype(项)>;
            if constexpr (std::is_same_v<类型, 节点稳定身份见证>)
                return 追加字节_(输出, 1) && 追加节点见证_(输出, 项);
            else return 追加字节_(输出, 2) && 追加字节_(输出, static_cast<std::uint8_t>(项.种类))
                && 追加无符号_(输出, 项.值, 8);
        }, 值);
    }

    static bool 追加类型化值材料_(std::vector<std::uint8_t>& 输出, const 类型化值材料& 材料) {
        return std::visit([&](const auto& 值) -> bool {
            using 类型 = std::decay_t<decltype(值)>;
            if constexpr (std::is_same_v<类型, std::int64_t>) return 追加字节_(输出, 1) && 追加无符号_(输出, static_cast<std::uint64_t>(值), 8);
            else if constexpr (std::is_same_v<类型, std::uint64_t>) return 追加字节_(输出, 2) && 追加无符号_(输出, 值, 8);
            else if constexpr (std::is_same_v<类型, I64区间材料>) return 追加字节_(输出, 3) && 追加无符号_(输出, static_cast<std::uint64_t>(值.下界), 8)
                && 追加无符号_(输出, static_cast<std::uint64_t>(值.上界), 8);
            else if constexpr (std::is_same_v<类型, std::vector<std::int64_t>>
                || std::is_same_v<类型, std::vector<std::uint64_t>>) {
                constexpr std::uint8_t 标签 = std::is_same_v<类型, std::vector<std::int64_t>> ? 4 : 5;
                if (!追加字节_(输出, 标签) || 值.size() > 最大向量项目数_ || !追加无符号_(输出, 值.size(), 8)) return false;
                for (const auto 项 : 值) if (!追加无符号_(输出, static_cast<std::uint64_t>(项), 8)) return false;
                return true;
            } else if constexpr (std::is_same_v<类型, 稳定身份有序组材料>) {
                if (!追加字节_(输出, 6) || 值.项目组.size() > 最大向量项目数_ || !追加无符号_(输出, 值.项目组.size(), 8)) return false;
                for (const auto& 项 : 值.项目组) if (!追加节点见证_(输出, 项)) return false;
                return true;
            } else {
                if (!追加字节_(输出, 7) || !追加稳定主键_(输出, 值.材料身份)
                    || !追加无符号_(输出, 值.格式版本, 4)
                    || !追加无符号_(输出, 值.字节数, 8)) return false;
                for (const auto 字节 : 值.SHA256) if (!追加字节_(输出, 字节)) return false;
                return true;
            }
        }, 材料);
    }

    static bool 追加类型合同值域_(std::vector<std::uint8_t>& 输出, const 类型合同值域& 值域) {
        return std::visit([&](const auto& 值) -> bool {
            using 类型 = std::decay_t<decltype(值)>;
            if constexpr (std::is_same_v<类型, I64标量值域>)
                return 追加字节_(输出, 1) && 追加无符号_(输出, static_cast<std::uint64_t>(值.下界), 8)
                    && 追加无符号_(输出, static_cast<std::uint64_t>(值.上界), 8);
            else if constexpr (std::is_same_v<类型, U64标量值域>)
                return 追加字节_(输出, 2) && 追加无符号_(输出, 值.下界, 8) && 追加无符号_(输出, 值.上界, 8);
            else if constexpr (std::is_same_v<类型, I64区间材料>)
                return 追加字节_(输出, 3) && 追加无符号_(输出, static_cast<std::uint64_t>(值.下界), 8)
                    && 追加无符号_(输出, static_cast<std::uint64_t>(值.上界), 8);
            else if constexpr (std::is_same_v<类型, I64有序组值域>)
                return 追加字节_(输出, 4) && 追加无符号_(输出, static_cast<std::uint64_t>(值.元素下界), 8)
                    && 追加无符号_(输出, static_cast<std::uint64_t>(值.元素上界), 8)
                    && 追加无符号_(输出, 值.最少项目数, 8) && 追加无符号_(输出, 值.最多项目数, 8);
            else if constexpr (std::is_same_v<类型, U64有序组值域>)
                return 追加字节_(输出, 5) && 追加无符号_(输出, 值.元素下界, 8) && 追加无符号_(输出, 值.元素上界, 8)
                    && 追加无符号_(输出, 值.最少项目数, 8) && 追加无符号_(输出, 值.最多项目数, 8);
            else if constexpr (std::is_same_v<类型, 稳定身份有序组值域>) {
                if (!追加字节_(输出, 6) || 值.允许节点类型稳定值组.size() > 最大向量项目数_
                    || !追加无符号_(输出, 值.允许节点类型稳定值组.size(), 8)) return false;
                for (const auto 项 : 值.允许节点类型稳定值组) if (!追加无符号_(输出, 项, 8)) return false;
                return 追加无符号_(输出, 值.最少项目数, 8) && 追加无符号_(输出, 值.最多项目数, 8);
            } else return 追加字节_(输出, 7) && 追加无符号_(输出, 值.格式版本, 4)
                && 追加无符号_(输出, 值.最少字节数, 8) && 追加无符号_(输出, 值.最多字节数, 8);
        }, 值域);
    }

    std::optional<std::vector<节点直接计划身份映射项>> 形成计划身份映射_(
        const 节点直接类型化结构数据操作请求& 请求) const {
        std::vector<const 节点直接节点创建项*> 节点项组;
        std::vector<const 节点直接关系创建项*> 关系项组;
        std::vector<const 节点直接类型化值发布项*> 值项组;
        std::vector<节点直接写集局部身份> 已用局部身份;
        auto 登记局部身份 = [&](节点直接写集局部身份 身份, 节点直接写集局部身份种类 期望种类) {
            if (身份.种类 != 期望种类 || 身份.值 == 0
                || std::find(已用局部身份.begin(), 已用局部身份.end(), 身份) != 已用局部身份.end()) return false;
            已用局部身份.push_back(身份);
            return true;
        };
        for (const auto& 写项 : 请求.写项组) {
            if (const auto* 项 = std::get_if<节点直接节点创建项>(&写项)) {
                if (!登记局部身份(项->局部身份, 节点直接写集局部身份种类::节点)
                    || !节点直接身份仓库::命名域已定义(项->命名域)
                    || !节点直接身份仓库::命名域与节点类型匹配(项->命名域, 项->类型)) return std::nullopt;
                节点项组.push_back(项);
            } else if (const auto* 项 = std::get_if<节点直接关系创建项>(&写项)) {
                if (!登记局部身份(项->局部身份, 节点直接写集局部身份种类::关系)) return std::nullopt;
                关系项组.push_back(项);
            } else if (const auto* 项 = std::get_if<节点直接类型化值发布项>(&写项)) {
                if (!登记局部身份(项->值记录局部身份, 节点直接写集局部身份种类::类型化值记录)) return std::nullopt;
                值项组.push_back(项);
            }
        }
        std::sort(节点项组.begin(), 节点项组.end(), [](const auto* 左, const auto* 右) {
            if (左->命名域 != 右->命名域)
                return static_cast<std::uint64_t>(左->命名域) < static_cast<std::uint64_t>(右->命名域);
            return 左->局部身份.值 < 右->局部身份.值;
        });
        std::sort(关系项组.begin(), 关系项组.end(), [](const auto* 左, const auto* 右) {
            return 左->局部身份.值 < 右->局部身份.值;
        });
        std::sort(值项组.begin(), 值项组.end(), [](const auto* 左, const auto* 右) {
            return 左->值记录局部身份.值 < 右->值记录局部身份.值;
        });
        std::vector<节点直接计划身份映射项> 映射;
        try { 映射.reserve(节点项组.size() + 关系项组.size() + 值项组.size()); }
        catch (...) { return std::nullopt; }
        std::uint64_t 当前命名域 = 0;
        std::uint64_t 当前计划键 = 0;
        for (const auto* 项 : 节点项组) {
            const auto 命名域 = static_cast<std::uint64_t>(项->命名域);
            if (命名域 != 当前命名域) {
                当前命名域 = 命名域;
                当前计划键 = 节点_->读取命名域高水位(项->命名域);
            }
            if (当前计划键 == std::numeric_limits<std::uint64_t>::max()) return std::nullopt;
            映射.push_back({项->局部身份, 命名域, ++当前计划键, 1});
        }
        当前计划键 = 关系_->读取命名域高水位(关系稳定主键命名域::正式关系);
        for (const auto* 项 : 关系项组) {
            if (当前计划键 == std::numeric_limits<std::uint64_t>::max()) return std::nullopt;
            映射.push_back({项->局部身份,
                static_cast<std::uint64_t>(关系稳定主键命名域::正式关系), ++当前计划键, 1});
        }
        当前计划键 = 类型化值_->读取命名域高水位(
            static_cast<std::uint64_t>(类型化值记录稳定身份命名域::通用类型化值记录));
        for (const auto* 项 : 值项组) {
            if (当前计划键 == std::numeric_limits<std::uint64_t>::max()) return std::nullopt;
            const auto 初始版本 = 项->预期当前值记录版本
                ? (*项->预期当前值记录版本 == std::numeric_limits<std::uint64_t>::max()
                    ? 0 : *项->预期当前值记录版本 + 1)
                : 1;
            if (初始版本 == 0) return std::nullopt;
            映射.push_back({项->值记录局部身份,
                static_cast<std::uint64_t>(类型化值记录稳定身份命名域::通用类型化值记录),
                ++当前计划键, 初始版本});
        }
        std::sort(映射.begin(), 映射.end(), [](const auto& 左, const auto& 右) {
            if (左.局部身份.种类 != 右.局部身份.种类)
                return static_cast<std::uint8_t>(左.局部身份.种类) < static_cast<std::uint8_t>(右.局部身份.种类);
            return 左.局部身份.值 < 右.局部身份.值;
        });
        return 映射;
    }

    static const 节点直接计划身份映射项* 查找计划身份_(
        const std::vector<节点直接计划身份映射项>& 映射,
        节点直接写集局部身份 身份) noexcept {
        const auto 位置 = std::find_if(映射.begin(), 映射.end(), [&](const auto& 项) { return 项.局部身份 == 身份; });
        return 位置 == 映射.end() ? nullptr : &*位置;
    }

    static 节点直接写集编码结果 编码写集_(
        const 节点直接类型化结构数据操作请求& 请求,
        std::uint64_t 预计后继代次,
        const std::vector<节点直接计划身份映射项>& 计划身份映射) {
        节点直接写集编码结果 结果;
        if (请求.写项组.size() > 最大向量项目数_ || 请求.读回规格.项目组.size() > 最大向量项目数_) {
            结果.状态 = 节点直接材料转换状态::入口拒绝;
            return 结果;
        }
        try {
            auto& 输出 = 结果.材料;
            if (!追加无符号_(输出, 2, 4)
                || !追加无符号_(输出, 请求.合同版本, 4)
                || !追加无符号_(输出, 请求.写集规则版本, 4)
                || !追加无符号_(输出, 请求.安装实例身份.命名域, 8)
                || !追加无符号_(输出, 请求.安装实例身份.键值, 8)
                || !追加无符号_(输出, 请求.幂等身份.命名域, 8)
                || !追加无符号_(输出, 请求.幂等身份.键值, 8)
                || !追加无符号_(输出, 请求.预期事实截止代次, 8)) {
                结果.状态 = 节点直接材料转换状态::入口拒绝;
                return 结果;
            }
            for (const auto 字节 : 请求.请求意图摘要) if (!追加字节_(输出, 字节)) {
                结果.状态 = 节点直接材料转换状态::入口拒绝; return 结果;
            }
            for (const auto 字节 : 请求.执行证据摘要) if (!追加字节_(输出, 字节)) {
                结果.状态 = 节点直接材料转换状态::入口拒绝; return 结果;
            }
            if (!追加无符号_(输出, 预计后继代次, 8)
                || !追加无符号_(输出, 计划身份映射.size(), 8)) {
                结果.状态 = 节点直接材料转换状态::入口拒绝;
                return 结果;
            }
            for (const auto& 映射 : 计划身份映射) {
                if (!追加字节_(输出, static_cast<std::uint8_t>(映射.局部身份.种类))
                    || !追加无符号_(输出, 映射.局部身份.值, 8)
                    || !追加无符号_(输出, 映射.稳定身份命名域, 8)
                    || !追加无符号_(输出, 映射.计划键, 8)
                    || !追加无符号_(输出, 映射.初始版本, 8)) {
                    结果.状态 = 节点直接材料转换状态::入口拒绝; return 结果;
                }
            }
            if (!追加无符号_(输出, 请求.写项组.size(), 8)) {
                结果.状态 = 节点直接材料转换状态::入口拒绝;
                return 结果;
            }
            for (const auto& 写项 : 请求.写项组) {
                const bool 成功 = std::visit([&](const auto& 项) -> bool {
                    using 类型 = std::decay_t<decltype(项)>;
                    if constexpr (std::is_same_v<类型, 节点直接类型合同发布项>) {
                        if (!追加字节_(输出, 1) || !追加无符号_(输出, 项.合同身份.命名域, 8) || !追加无符号_(输出, 项.合同身份.键值, 8)
                            || !追加无符号_(输出, 项.命名空间.命名域, 8) || !追加无符号_(输出, 项.命名空间.键值, 8)
                            || !追加无符号_(输出, 项.合同版本, 4) || !追加字节_(输出, static_cast<std::uint8_t>(项.表示))
                            || !追加类型合同值域_(输出, 项.值域) || !追加无符号_(输出, 项.所有者服务.命名域, 8)
                            || !追加无符号_(输出, 项.所有者服务.键值, 8) || !追加字节_(输出, static_cast<std::uint8_t>(项.生命周期))
                            || 项.直接兼容组.size() > 最大向量项目数_ || !追加无符号_(输出, 项.直接兼容组.size(), 8)) return false;
                        for (const auto& 兼容 : 项.直接兼容组) if (!追加无符号_(输出, 兼容.另一合同身份.命名域, 8)
                            || !追加无符号_(输出, 兼容.另一合同身份.键值, 8) || !追加无符号_(输出, 兼容.另一合同版本, 4)
                            || !追加字节_(输出, static_cast<std::uint8_t>(兼容.操作)) || !追加字节_(输出, static_cast<std::uint8_t>(兼容.方向))) return false;
                        return true;
                    } else if constexpr (std::is_same_v<类型, 节点直接节点创建项>)
                        return 追加字节_(输出, 2) && 追加字节_(输出, static_cast<std::uint8_t>(项.局部身份.种类)) && 追加无符号_(输出, 项.局部身份.值, 8)
                            && 追加无符号_(输出, static_cast<std::uint64_t>(项.命名域), 8)
                            && 追加无符号_(输出, static_cast<std::uint64_t>(项.类型), 4);
                    else if constexpr (std::is_same_v<类型, 节点直接关系创建项>)
                        return 追加字节_(输出, 3) && 追加字节_(输出, static_cast<std::uint8_t>(项.局部身份.种类)) && 追加无符号_(输出, 项.局部身份.值, 8)
                            && 追加无符号_(输出, static_cast<std::uint64_t>(项.类型), 4) && 追加端点_(输出, 项.源端)
                            && 追加端点_(输出, 项.目标端) && 追加无符号_(输出, static_cast<std::uint64_t>(项.角色或顺序), 8);
                    else if constexpr (std::is_same_v<类型, 节点直接关系失效项>)
                        return 追加字节_(输出, 4) && 追加无符号_(输出, 项.预期当前.稳定主键.命名域, 8) && 追加无符号_(输出, 项.预期当前.稳定主键.键值, 8)
                            && 追加无符号_(输出, static_cast<std::uint64_t>(项.预期当前.类型), 4) && 追加无符号_(输出, 项.预期当前.关系版本, 4)
                            && 追加节点见证_(输出, 项.预期当前.源端) && 追加节点见证_(输出, 项.预期当前.目标端)
                            && 追加无符号_(输出, static_cast<std::uint64_t>(项.预期当前.角色或顺序), 8);
                    else if constexpr (std::is_same_v<类型, 节点直接类型化值发布项>) {
                        if (!追加字节_(输出, 5) || !追加端点_(输出, 项.所属身份) || !追加无符号_(输出, 项.类型合同身份.命名域, 8)
                            || !追加无符号_(输出, 项.类型合同身份.键值, 8) || !追加无符号_(输出, 项.类型合同版本, 4)
                            || !追加字节_(输出, static_cast<std::uint8_t>(项.值记录局部身份.种类)) || !追加无符号_(输出, 项.值记录局部身份.值, 8)
                            || !追加字节_(输出, 项.预期当前值记录版本.has_value() ? 1 : 0)
                            || (项.预期当前值记录版本 && !追加无符号_(输出, *项.预期当前值记录版本, 8))
                            || !追加类型化值材料_(输出, 项.材料)) return false;
                        return std::visit([&](const auto& 来源) -> bool {
                            using 来源类型 = std::decay_t<decltype(来源)>;
                            if constexpr (std::is_same_v<来源类型, 节点直接节点端点引用>) return 追加字节_(输出, 1) && 追加端点_(输出, 来源);
                            else return 追加字节_(输出, 2) && 追加无符号_(输出, 来源.命名域, 8) && 追加无符号_(输出, 来源.键值, 8);
                        }, 项.来源);
                    } else if constexpr (std::is_same_v<类型, 节点直接类型化值退役项>)
                        return 追加字节_(输出, 6) && 追加节点见证_(输出, 项.所属身份) && 追加无符号_(输出, 项.类型合同身份.命名域, 8)
                            && 追加无符号_(输出, 项.类型合同身份.键值, 8) && 追加无符号_(输出, 项.类型合同版本, 4)
                            && 追加无符号_(输出, 项.值记录身份.命名域, 8) && 追加无符号_(输出, 项.值记录身份.键值, 8)
                            && 追加无符号_(输出, 项.预期值记录版本, 8);
                    else if constexpr (std::is_same_v<类型, 节点直接索引创建项>)
                        return 追加字节_(输出, 7) && 追加无符号_(输出, 项.键.所有者身份, 8) && 追加无符号_(输出, 项.键.命名域, 4)
                            && 追加无符号_(输出, 项.键.键格式版本, 4) && 追加无符号_(输出, 项.键.探测规则版本, 4)
                            && 追加无符号_(输出, 项.键.键值, 8) && 追加端点_(输出, 项.目标);
                    else if constexpr (std::is_same_v<类型, 节点直接索引移除项>)
                        return 追加字节_(输出, 8) && 追加无符号_(输出, 项.键.所有者身份, 8) && 追加无符号_(输出, 项.键.命名域, 4)
                            && 追加无符号_(输出, 项.键.键格式版本, 4) && 追加无符号_(输出, 项.键.探测规则版本, 4)
                            && 追加无符号_(输出, 项.键.键值, 8) && 追加节点见证_(输出, 项.预期目标);
                    else return 追加字节_(输出, 9) && 追加节点见证_(输出, 项.预期当前);
                }, 写项);
                if (!成功) { 结果.状态 = 节点直接材料转换状态::入口拒绝; return 结果; }
            }
            if (!追加无符号_(输出, 请求.读回规格.项目组.size(), 8)) {
                结果.状态 = 节点直接材料转换状态::入口拒绝; return 结果;
            }
            for (const auto& 项 : 请求.读回规格.项目组) {
                if (!追加字节_(输出, static_cast<std::uint8_t>(项.种类))
                    || !追加无符号_(输出, 项.预期版本, 8)) {
                    结果.状态 = 节点直接材料转换状态::入口拒绝; return 结果;
                }
                const bool 成功 = std::visit([&](const auto& 身份) -> bool {
                    using 类型 = std::decay_t<decltype(身份)>;
                    if constexpr (std::is_same_v<类型, 节点直接写集局部身份>) return 追加字节_(输出, 1) && 追加字节_(输出, static_cast<std::uint8_t>(身份.种类)) && 追加无符号_(输出, 身份.值, 8);
                    else if constexpr (std::is_same_v<类型, 节点稳定主键>) return 追加字节_(输出, 2) && 追加稳定主键_(输出, 身份);
                    else if constexpr (std::is_same_v<类型, 关系稳定主键> || std::is_same_v<类型, 类型化值记录稳定身份>)
                        return 追加字节_(输出, std::is_same_v<类型, 关系稳定主键> ? 3 : 4) && 追加无符号_(输出, 身份.命名域, 8) && 追加无符号_(输出, 身份.键值, 8);
                    else return 追加字节_(输出, 5) && 追加无符号_(输出, 身份.所有者身份, 8) && 追加无符号_(输出, 身份.命名域, 4)
                        && 追加无符号_(输出, 身份.键格式版本, 4) && 追加无符号_(输出, 身份.探测规则版本, 4)
                        && 追加无符号_(输出, 身份.键值, 8);
                }, 项.身份);
                if (!成功) { 结果.状态 = 节点直接材料转换状态::入口拒绝; return 结果; }
            }
            结果.状态 = 节点直接材料转换状态::成功;
        } catch (...) {
            结果.材料.clear();
            结果.状态 = 节点直接材料转换状态::资源失败;
        }
        return 结果;
    }

    static 节点直接摘要计算结果 计算SHA256_(std::span<const std::uint8_t> 材料) noexcept {
        节点直接摘要计算结果 结果;
        BCRYPT_ALG_HANDLE 算法 = nullptr;
        BCRYPT_HASH_HANDLE 哈希 = nullptr;
        if (BCryptOpenAlgorithmProvider(&算法, BCRYPT_SHA256_ALGORITHM, nullptr, 0) < 0) return 结果;
        const auto 关闭算法 = [&]() noexcept { if (算法 != nullptr) BCryptCloseAlgorithmProvider(算法, 0); };
        if (BCryptCreateHash(算法, &哈希, nullptr, 0, nullptr, 0, 0) < 0) { 关闭算法(); return 结果; }
        const bool 成功 = (材料.empty() || BCryptHashData(哈希, const_cast<PUCHAR>(材料.data()), static_cast<ULONG>(材料.size()), 0) >= 0)
            && BCryptFinishHash(哈希, 结果.摘要.data(), static_cast<ULONG>(结果.摘要.size()), 0) >= 0;
        BCryptDestroyHash(哈希);
        关闭算法();
        结果.状态 = 成功 && 摘要非零_(结果.摘要)
            ? 节点直接材料转换状态::成功 : 节点直接材料转换状态::内部不一致;
        return 结果;
    }

    static bool 摘要非零_(const std::array<std::uint8_t, 32>& 摘要) noexcept {
        for (const auto 值 : 摘要) if (值 != 0) return true;
        return false;
    }

    节点直接类型化结构数据操作结果 形成幂等读回_(
        const 节点直接事务幂等记录& 记录) const {
        return {节点直接类型化结构数据操作状态::幂等读回, 记录.发布代次,
            幂等_->读取持久证据状态(记录.幂等身份), 记录.节点组, 记录.关系组,
            记录.类型化值组, 记录.索引组};
    }

    static bool 局部身份有效_(
        节点直接写集局部身份 身份,
        节点直接写集局部身份种类 期望种类) noexcept {
        return 身份.种类 == 期望种类 && 身份.值 != 0;
    }

    static bool 端点引用静态完整_(const 节点直接节点端点引用& 引用) noexcept {
        if (const auto* 见证 = std::get_if<节点稳定身份见证>(&引用))
            return 节点稳定身份见证完整(*见证);
        return 局部身份有效_(std::get<节点直接写集局部身份>(引用),
            节点直接写集局部身份种类::节点);
    }

    static bool 类型合同值域静态完整_(const 类型合同值域& 值域) noexcept {
        return std::visit([](const auto& 值) noexcept {
            using 类型 = std::decay_t<decltype(值)>;
            if constexpr (std::is_same_v<类型, I64标量值域>
                || std::is_same_v<类型, U64标量值域>
                || std::is_same_v<类型, I64区间材料>) return 值.下界 <= 值.上界;
            else if constexpr (std::is_same_v<类型, I64有序组值域>
                || std::is_same_v<类型, U64有序组值域>) {
                return 值.元素下界 <= 值.元素上界 && 值.最少项目数 <= 值.最多项目数;
            } else if constexpr (std::is_same_v<类型, 稳定身份有序组值域>) {
                return 值.最少项目数 <= 值.最多项目数
                    && !值.允许节点类型稳定值组.empty();
            } else return 值.格式版本 != 0 && 值.最少字节数 <= 值.最多字节数;
        }, 值域);
    }

    static bool 表示与值域匹配_(
        类型化值表示种类 表示,
        const 类型合同值域& 值域) noexcept {
        return std::visit([&](const auto& 值) noexcept {
            using 类型 = std::decay_t<decltype(值)>;
            if constexpr (std::is_same_v<类型, I64标量值域>)
                return 表示 == 类型化值表示种类::I64标量;
            else if constexpr (std::is_same_v<类型, U64标量值域>)
                return 表示 == 类型化值表示种类::U64标量;
            else if constexpr (std::is_same_v<类型, I64区间材料>)
                return 表示 == 类型化值表示种类::I64区间;
            else if constexpr (std::is_same_v<类型, I64有序组值域>)
                return 表示 == 类型化值表示种类::I64有序组;
            else if constexpr (std::is_same_v<类型, U64有序组值域>)
                return 表示 == 类型化值表示种类::U64有序组;
            else if constexpr (std::is_same_v<类型, 稳定身份有序组值域>)
                return 表示 == 类型化值表示种类::稳定身份有序组;
            else return 表示 == 类型化值表示种类::独立材料引用;
        }, 值域);
    }

    static bool 类型化值材料静态完整_(const 类型化值材料& 材料) noexcept {
        return std::visit([](const auto& 值) noexcept {
            using 类型 = std::decay_t<decltype(值)>;
            if constexpr (std::is_same_v<类型, I64区间材料>) return 值.下界 <= 值.上界;
            else if constexpr (std::is_same_v<类型, std::vector<std::int64_t>>
                || std::is_same_v<类型, std::vector<std::uint64_t>>) {
                return 值.size() <= 最大向量项目数_;
            } else if constexpr (std::is_same_v<类型, 稳定身份有序组材料>) {
                return 值.项目组.size() <= 最大向量项目数_
                    && std::all_of(值.项目组.begin(), 值.项目组.end(), 节点稳定身份见证完整);
            } else if constexpr (std::is_same_v<类型, 独立材料引用>) {
                return 稳定主键有效(值.材料身份) && 值.格式版本 != 0 && 值.字节数 != 0
                    && std::any_of(值.SHA256.begin(), 值.SHA256.end(), [](auto 字节) { return 字节 != 0; });
            } else return true;
        }, 材料);
    }

    static bool 类型化值材料符合合同_(
        const 类型化值材料& 材料,
        const 类型合同读回& 合同) noexcept {
        if (!表示与值域匹配_(合同.表示, 合同.值域)
            || 材料.index() != 合同.值域.index()) return false;
        return std::visit([&](const auto& 值) noexcept {
            using 类型 = std::decay_t<decltype(值)>;
            if constexpr (std::is_same_v<类型, std::int64_t>) {
                const auto& 值域 = std::get<I64标量值域>(合同.值域);
                return 值 >= 值域.下界 && 值 <= 值域.上界;
            } else if constexpr (std::is_same_v<类型, std::uint64_t>) {
                const auto& 值域 = std::get<U64标量值域>(合同.值域);
                return 值 >= 值域.下界 && 值 <= 值域.上界;
            } else if constexpr (std::is_same_v<类型, I64区间材料>) {
                const auto& 值域 = std::get<I64区间材料>(合同.值域);
                return 值.下界 >= 值域.下界 && 值.上界 <= 值域.上界;
            } else if constexpr (std::is_same_v<类型, std::vector<std::int64_t>>) {
                const auto& 值域 = std::get<I64有序组值域>(合同.值域);
                if (值.size() < 值域.最少项目数 || 值.size() > 值域.最多项目数) return false;
                return std::all_of(值.begin(), 值.end(), [&](auto 项) {
                    return 项 >= 值域.元素下界 && 项 <= 值域.元素上界;
                });
            } else if constexpr (std::is_same_v<类型, std::vector<std::uint64_t>>) {
                const auto& 值域 = std::get<U64有序组值域>(合同.值域);
                if (值.size() < 值域.最少项目数 || 值.size() > 值域.最多项目数) return false;
                return std::all_of(值.begin(), 值.end(), [&](auto 项) {
                    return 项 >= 值域.元素下界 && 项 <= 值域.元素上界;
                });
            } else if constexpr (std::is_same_v<类型, 稳定身份有序组材料>) {
                const auto& 值域 = std::get<稳定身份有序组值域>(合同.值域);
                if (值.项目组.size() < 值域.最少项目数 || 值.项目组.size() > 值域.最多项目数)
                    return false;
                return std::all_of(值.项目组.begin(), 值.项目组.end(), [&](const auto& 项) {
                    const auto 稳定值 = static_cast<std::uint64_t>(项.类型);
                    return std::find(值域.允许节点类型稳定值组.begin(),
                        值域.允许节点类型稳定值组.end(), 稳定值)
                        != 值域.允许节点类型稳定值组.end();
                });
            } else {
                const auto& 值域 = std::get<独立材料引用值域>(合同.值域);
                return 值.格式版本 == 值域.格式版本
                    && 值.字节数 >= 值域.最少字节数 && 值.字节数 <= 值域.最多字节数;
            }
        }, 材料);
    }

    static bool 类型合同写项静态内容等于读回_(
        const 节点直接类型合同发布项& 写项,
        const 类型合同读回& 读回) noexcept {
        return 写项.合同身份 == 读回.合同身份
            && 写项.命名空间 == 读回.命名空间
            && 写项.合同版本 == 读回.合同版本
            && 写项.表示 == 读回.表示
            && 写项.值域 == 读回.值域
            && 写项.所有者服务 == 读回.所有者服务
            && 写项.生命周期 == 读回.生命周期
            && 写项.直接兼容组 == 读回.直接兼容组;
    }

    static bool 含固定混合创建种类组合_(
        const 节点直接类型化结构数据操作请求& 请求) noexcept {
        const auto 合同数 = static_cast<std::size_t>(std::count_if(
            请求.写项组.begin(), 请求.写项组.end(), [](const auto& 项) {
                return std::holds_alternative<节点直接类型合同发布项>(项);
            }));
        const auto 值数 = static_cast<std::size_t>(std::count_if(
            请求.写项组.begin(), 请求.写项组.end(), [](const auto& 项) {
                return std::holds_alternative<节点直接类型化值发布项>(项);
            }));
        return (合同数 != 0 && 合同数 != 请求.写项组.size())
            || (值数 != 0 && 值数 != 请求.写项组.size());
    }

    static bool 是固定混合创建形状_(
        const 节点直接类型化结构数据操作请求& 请求,
        std::span<节点直接类型化结构事务参与者* const> 参与者组) noexcept {
        if (!参与者组.empty() || 请求.写项组.size() != 8
            || 请求.读回规格.项目组.size() != 7) return false;
        const auto* 合同 = std::get_if<节点直接类型合同发布项>(&请求.写项组[0]);
        const auto* 节点 = std::get_if<节点直接节点创建项>(&请求.写项组[1]);
        const auto* 值 = std::get_if<节点直接类型化值发布项>(&请求.写项组[2]);
        if (合同 == nullptr || 节点 == nullptr || 值 == nullptr
            || 合同->生命周期 != 类型合同生命周期状态::当前可写
            || 值->类型合同身份 != 合同->合同身份
            || 值->类型合同版本 != 合同->合同版本
            || 值->预期当前值记录版本.has_value()) return false;
        const auto* 所属 = std::get_if<节点直接写集局部身份>(&值->所属身份);
        const auto* 来源联合 = std::get_if<节点直接节点端点引用>(&值->来源);
        const auto* 来源 = 来源联合 == nullptr
            ? nullptr : std::get_if<节点稳定身份见证>(来源联合);
        if (所属 == nullptr || *所属 != 节点->局部身份 || 来源 == nullptr) return false;
        for (std::size_t 序号 = 3; 序号 < 8; ++序号) {
            const auto* 关系 = std::get_if<节点直接关系创建项>(&请求.写项组[序号]);
            if (关系 == nullptr) return false;
            const auto* 源 = std::get_if<节点直接写集局部身份>(&关系->源端);
            const auto* 目标 = std::get_if<节点稳定身份见证>(&关系->目标端);
            if (源 == nullptr || *源 != 节点->局部身份 || 目标 == nullptr) return false;
        }
        const std::array<节点直接写集局部身份, 7> 局部身份组{
            节点->局部身份,
            值->值记录局部身份,
            std::get<节点直接关系创建项>(请求.写项组[3]).局部身份,
            std::get<节点直接关系创建项>(请求.写项组[4]).局部身份,
            std::get<节点直接关系创建项>(请求.写项组[5]).局部身份,
            std::get<节点直接关系创建项>(请求.写项组[6]).局部身份,
            std::get<节点直接关系创建项>(请求.写项组[7]).局部身份};
        for (std::size_t 序号 = 0; 序号 < 局部身份组.size(); ++序号) {
            const auto& 读回 = 请求.读回规格.项目组[序号];
            const auto* 身份 = std::get_if<节点直接写集局部身份>(&读回.身份);
            const auto 预期种类 = 序号 == 0
                ? 节点直接发布后读回对象种类::当前节点
                : 序号 == 1
                    ? 节点直接发布后读回对象种类::当前类型化值
                    : 节点直接发布后读回对象种类::当前关系;
            if (身份 == nullptr || *身份 != 局部身份组[序号]
                || 读回.种类 != 预期种类 || 读回.预期版本 != 1) return false;
        }
        return true;
    }

    static bool 请求静态完整_(const 节点直接类型化结构数据操作请求& 请求) noexcept {
        if (请求.写项组.size() > 最大向量项目数_
            || 请求.读回规格.项目组.size() > 最大向量项目数_) return false;
        std::vector<节点直接写集局部身份> 局部身份组;
        auto 登记局部身份 = [&](节点直接写集局部身份 身份,
            节点直接写集局部身份种类 期望种类) {
            if (!局部身份有效_(身份, 期望种类)
                || std::find(局部身份组.begin(), 局部身份组.end(), 身份) != 局部身份组.end()) return false;
            局部身份组.push_back(身份);
            return true;
        };
        for (const auto& 写项 : 请求.写项组) {
            const bool 完整 = std::visit([&](const auto& 项) {
                using 类型 = std::decay_t<decltype(项)>;
                if constexpr (std::is_same_v<类型, 节点直接类型合同发布项>) {
                    if (!类型合同稳定身份完整(项.合同身份)
                        || 项.命名空间.命名域 == 0 || 项.命名空间.键值 == 0
                        || 项.合同版本 == 0 || 项.表示 == 类型化值表示种类::未定义
                        || static_cast<std::uint8_t>(项.表示) > 7
                        || !表示与值域匹配_(项.表示, 项.值域)
                        || !类型合同值域静态完整_(项.值域)
                        || !服务稳定身份完整(项.所有者服务)
                        || 项.生命周期 != 类型合同生命周期状态::当前可写
                        || 项.直接兼容组.size() > 最大向量项目数_) return false;
                    for (std::size_t 序号 = 0; 序号 < 项.直接兼容组.size(); ++序号) {
                        const auto& 兼容 = 项.直接兼容组[序号];
                        if (!类型合同稳定身份完整(兼容.另一合同身份) || 兼容.另一合同版本 == 0
                            || static_cast<std::uint8_t>(兼容.操作) < 1 || static_cast<std::uint8_t>(兼容.操作) > 4
                            || static_cast<std::uint8_t>(兼容.方向) < 1 || static_cast<std::uint8_t>(兼容.方向) > 3) return false;
                        for (std::size_t 既有序号 = 0; 既有序号 < 序号; ++既有序号)
                            if (项.直接兼容组[既有序号] == 兼容) return false;
                    }
                    return true;
                } else if constexpr (std::is_same_v<类型, 节点直接节点创建项>) {
                    return 登记局部身份(项.局部身份, 节点直接写集局部身份种类::节点)
                        && 节点直接身份仓库::命名域已定义(项.命名域)
                        && 节点直接身份仓库::命名域与节点类型匹配(项.命名域, 项.类型);
                } else if constexpr (std::is_same_v<类型, 节点直接关系创建项>) {
                    return 登记局部身份(项.局部身份, 节点直接写集局部身份种类::关系)
                        && static_cast<std::uint32_t>(项.类型) < 正式关系类型ABI数量
                        && 端点引用静态完整_(项.源端) && 端点引用静态完整_(项.目标端);
                } else if constexpr (std::is_same_v<类型, 节点直接关系失效项>) {
                    return 关系稳定主键完整(项.预期当前.稳定主键)
                        && 项.预期当前.关系版本 != 0
                        && 节点稳定身份见证完整(项.预期当前.源端)
                        && 节点稳定身份见证完整(项.预期当前.目标端);
                } else if constexpr (std::is_same_v<类型, 节点直接类型化值发布项>) {
                    const bool 来源完整 = std::visit([](const auto& 来源) {
                        using 来源类型 = std::decay_t<decltype(来源)>;
                        if constexpr (std::is_same_v<来源类型, 节点直接节点端点引用>)
                            return 端点引用静态完整_(来源);
                        else return 服务稳定身份完整(来源);
                    }, 项.来源);
                    return 端点引用静态完整_(项.所属身份)
                        && 类型合同稳定身份完整(项.类型合同身份) && 项.类型合同版本 != 0
                        && 登记局部身份(项.值记录局部身份, 节点直接写集局部身份种类::类型化值记录)
                        && (!项.预期当前值记录版本 || *项.预期当前值记录版本 != 0)
                        && 类型化值材料静态完整_(项.材料) && 来源完整;
                } else if constexpr (std::is_same_v<类型, 节点直接类型化值退役项>) {
                    return 节点稳定身份见证完整(项.所属身份)
                        && 类型合同稳定身份完整(项.类型合同身份) && 项.类型合同版本 != 0
                        && 类型化值记录稳定身份完整(项.值记录身份) && 项.预期值记录版本 != 0;
                } else if constexpr (std::is_same_v<类型, 节点直接索引创建项>) {
                    return 索引物理键完整(项.键) && 端点引用静态完整_(项.目标);
                } else if constexpr (std::is_same_v<类型, 节点直接索引移除项>) {
                    return 索引物理键完整(项.键) && 节点稳定身份见证完整(项.预期目标);
                } else return 节点稳定身份见证完整(项.预期当前);
            }, 写项);
            if (!完整) return false;
        }
        auto 局部身份已登记 = [&](节点直接写集局部身份 身份) {
            return std::find(局部身份组.begin(), 局部身份组.end(), 身份) != 局部身份组.end();
        };
        for (const auto& 写项 : 请求.写项组) {
            const bool 引用存在 = std::visit([&](const auto& 项) {
                using 类型 = std::decay_t<decltype(项)>;
                if constexpr (std::is_same_v<类型, 节点直接关系创建项>) {
                    const auto 本地存在 = [&](const auto& 端点) {
                        const auto* 本地 = std::get_if<节点直接写集局部身份>(&端点);
                        return 本地 == nullptr || 局部身份已登记(*本地);
                    };
                    return 本地存在(项.源端) && 本地存在(项.目标端);
                } else if constexpr (std::is_same_v<类型, 节点直接类型化值发布项>) {
                    const auto* 所属本地 = std::get_if<节点直接写集局部身份>(&项.所属身份);
                    if (所属本地 != nullptr && !局部身份已登记(*所属本地)) return false;
                    if (const auto* 节点来源 = std::get_if<节点直接节点端点引用>(&项.来源)) {
                        const auto* 来源本地 = std::get_if<节点直接写集局部身份>(节点来源);
                        if (来源本地 != nullptr && !局部身份已登记(*来源本地)) return false;
                    }
                    return true;
                } else if constexpr (std::is_same_v<类型, 节点直接索引创建项>) {
                    const auto* 本地 = std::get_if<节点直接写集局部身份>(&项.目标);
                    return 本地 == nullptr || 局部身份已登记(*本地);
                } else return true;
            }, 写项);
            if (!引用存在) return false;
        }
        for (std::size_t 序号 = 0; 序号 < 请求.写项组.size(); ++序号) {
            const 索引物理键* 键 = nullptr;
            std::visit([&](const auto& 项) {
                using 类型 = std::decay_t<decltype(项)>;
                if constexpr (std::is_same_v<类型, 节点直接索引创建项>
                    || std::is_same_v<类型, 节点直接索引移除项>) 键 = &项.键;
            }, 请求.写项组[序号]);
            if (键 == nullptr) continue;
            for (std::size_t 既有序号 = 0; 既有序号 < 序号; ++既有序号) {
                const 索引物理键* 既有键 = nullptr;
                std::visit([&](const auto& 项) {
                    using 类型 = std::decay_t<decltype(项)>;
                    if constexpr (std::is_same_v<类型, 节点直接索引创建项>
                        || std::is_same_v<类型, 节点直接索引移除项>) 既有键 = &项.键;
                }, 请求.写项组[既有序号]);
                if (既有键 != nullptr && *既有键 == *键) return false;
            }
        }
        for (std::size_t 读回序号 = 0; 读回序号 < 请求.读回规格.项目组.size(); ++读回序号) {
            const auto& 读回 = 请求.读回规格.项目组[读回序号];
            const bool 完整 = std::visit([&](const auto& 身份) {
                using 类型 = std::decay_t<decltype(身份)>;
                if constexpr (std::is_same_v<类型, 节点直接写集局部身份>) {
                    if (!局部身份已登记(身份) || 读回.预期版本 == 0) return false;
                    if (读回.种类 == 节点直接发布后读回对象种类::当前节点)
                        return 身份.种类 == 节点直接写集局部身份种类::节点;
                    if (读回.种类 == 节点直接发布后读回对象种类::当前关系)
                        return 身份.种类 == 节点直接写集局部身份种类::关系;
                    return 读回.种类 == 节点直接发布后读回对象种类::当前类型化值
                        && 身份.种类 == 节点直接写集局部身份种类::类型化值记录;
                } else if constexpr (std::is_same_v<类型, 节点稳定主键>) {
                    return 读回.种类 == 节点直接发布后读回对象种类::当前节点
                        && 稳定主键有效(身份) && 读回.预期版本 != 0;
                } else if constexpr (std::is_same_v<类型, 关系稳定主键>) {
                    return (读回.种类 == 节点直接发布后读回对象种类::当前关系
                            || 读回.种类 == 节点直接发布后读回对象种类::已失效关系)
                        && 关系稳定主键完整(身份) && 读回.预期版本 != 0;
                } else if constexpr (std::is_same_v<类型, 类型化值记录稳定身份>) {
                    return 读回.种类 == 节点直接发布后读回对象种类::当前类型化值
                        && 类型化值记录稳定身份完整(身份) && 读回.预期版本 != 0;
                } else return (读回.种类 == 节点直接发布后读回对象种类::当前索引
                        || 读回.种类 == 节点直接发布后读回对象种类::已移除索引)
                    && 索引物理键完整(身份) && 读回.预期版本 == 0;
            }, 读回.身份);
            if (!完整) return false;
            const auto* 索引键 = std::get_if<索引物理键>(&读回.身份);
            if (索引键 == nullptr) continue;
            for (std::size_t 既有序号 = 0; 既有序号 < 读回序号; ++既有序号) {
                const auto& 既有 = 请求.读回规格.项目组[既有序号];
                const auto* 既有键 = std::get_if<索引物理键>(&既有.身份);
                if (既有键 != nullptr && 既有.种类 == 读回.种类 && *既有键 == *索引键) return false;
            }
            std::size_t 配对数 = 0;
            for (const auto& 写项 : 请求.写项组) {
                if (读回.种类 == 节点直接发布后读回对象种类::当前索引) {
                    const auto* 创建 = std::get_if<节点直接索引创建项>(&写项);
                    if (创建 != nullptr && 创建->键 == *索引键) ++配对数;
                } else {
                    const auto* 移除 = std::get_if<节点直接索引移除项>(&写项);
                    if (移除 != nullptr && 移除->键 == *索引键) ++配对数;
                }
            }
            if (配对数 != 1) return false;
        }
        return true;
    }

    bool 请求动态准备前完整_(
        const 节点直接类型化结构数据操作请求& 请求,
        std::uint64_t 事务序号,
        std::uint64_t 当前代次) const {
        auto 节点见证当前 = [&](const 节点稳定身份见证& 见证) {
            const auto 当前 = 节点_->读取稳定主键当前身份(见证.稳定主键, 事务序号);
            return 当前.状态 == 稳定主键当前身份状态::当前有效
                && 当前.当前记录 && 当前.当前记录->类型 == 见证.类型
                && 当前.当前记录->版本号 == 见证.身份版本;
        };
        auto 查找合同 = [&](类型合同稳定身份 身份, std::uint32_t 版本)
            -> std::optional<类型合同读回> {
            const auto 已发布 = 类型合同_->读取精确合同(身份, 版本);
            if (已发布) {
                const auto 位置 = std::find_if(请求.写项组.begin(), 请求.写项组.end(),
                    [&](const auto& 写项) {
                        const auto* 项 = std::get_if<节点直接类型合同发布项>(&写项);
                        return 项 != nullptr && 项->合同身份 == 身份 && 项->合同版本 == 版本;
                    });
                if (位置 != 请求.写项组.end()
                    && !类型合同写项静态内容等于读回_(
                        std::get<节点直接类型合同发布项>(*位置), *已发布)) return std::nullopt;
                return 已发布;
            }
            for (const auto& 写项 : 请求.写项组) {
                const auto* 项 = std::get_if<节点直接类型合同发布项>(&写项);
                if (项 != nullptr && 项->合同身份 == 身份 && 项->合同版本 == 版本) {
                    return 类型合同读回{项->合同身份, 项->命名空间, 项->合同版本,
                        项->表示, 项->值域, 项->所有者服务, 项->生命周期,
                        项->直接兼容组, 当前代次 + 1, 当前代次 + 1};
                }
            }
            return std::nullopt;
        };
        for (const auto& 写项 : 请求.写项组) {
            const bool 完整 = std::visit([&](const auto& 项) {
                using 类型 = std::decay_t<decltype(项)>;
                if constexpr (std::is_same_v<类型, 节点直接关系创建项>) {
                    const auto 验证端点 = [&](const auto& 端点) {
                        const auto* 见证 = std::get_if<节点稳定身份见证>(&端点);
                        return 见证 == nullptr || 节点见证当前(*见证);
                    };
                    return 验证端点(项.源端) && 验证端点(项.目标端);
                } else if constexpr (std::is_same_v<类型, 节点直接关系失效项>) {
                    const auto 当前 = 关系_->读取稳定主键当前关系(
                        项.预期当前.稳定主键, 事务序号);
                    if (当前.状态 != 稳定关系当前读取状态::当前有效 || !当前.记录
                        || 当前.记录->类型 != 项.预期当前.类型
                        || 当前.记录->版本号 != 项.预期当前.关系版本
                        || 当前.记录->顺序号 != 项.预期当前.角色或顺序) return false;
                    const auto 源记录 = 节点_->读取节点(当前.记录->源节点, 事务序号);
                    const auto 目标记录 = 节点_->读取节点(当前.记录->目标节点, 事务序号);
                    return 源记录 && 目标记录
                        && 源记录->稳定主键 == 项.预期当前.源端.稳定主键
                        && 源记录->类型 == 项.预期当前.源端.类型
                        && 源记录->版本号 == 项.预期当前.源端.身份版本
                        && 目标记录->稳定主键 == 项.预期当前.目标端.稳定主键
                        && 目标记录->类型 == 项.预期当前.目标端.类型
                        && 目标记录->版本号 == 项.预期当前.目标端.身份版本;
                } else if constexpr (std::is_same_v<类型, 节点直接类型化值发布项>) {
                    const auto 合同 = 查找合同(项.类型合同身份, 项.类型合同版本);
                    if (!合同 || 合同->生命周期 != 类型合同生命周期状态::当前可写
                        || !类型化值材料符合合同_(项.材料, *合同)) return false;
                    const auto* 所属见证 = std::get_if<节点稳定身份见证>(&项.所属身份);
                    if (所属见证 != nullptr && !节点见证当前(*所属见证)) return false;
                    if (const auto* 来源节点 = std::get_if<节点直接节点端点引用>(&项.来源)) {
                        const auto* 来源见证 = std::get_if<节点稳定身份见证>(来源节点);
                        if (来源见证 != nullptr && !节点见证当前(*来源见证)) return false;
                    }
                    if (所属见证 == nullptr) return !项.预期当前值记录版本.has_value();
                    const auto 当前组 = 类型化值_->读取所属身份当前值组(所属见证->稳定主键);
                    const auto 位置 = std::find_if(当前组.begin(), 当前组.end(), [&](const auto& 当前) {
                        return 当前.类型合同身份 == 项.类型合同身份
                            && 当前.类型合同版本 == 项.类型合同版本;
                    });
                    return 项.预期当前值记录版本
                        ? 位置 != 当前组.end()
                            && 位置->值记录版本 == *项.预期当前值记录版本
                        : 位置 == 当前组.end();
                } else if constexpr (std::is_same_v<类型, 节点直接类型化值退役项>) {
                    if (!节点见证当前(项.所属身份)) return false;
                    const auto 当前组 = 类型化值_->读取所属身份当前值组(项.所属身份.稳定主键);
                    return std::any_of(当前组.begin(), 当前组.end(), [&](const auto& 当前) {
                        return 当前.类型合同身份 == 项.类型合同身份
                            && 当前.类型合同版本 == 项.类型合同版本
                            && 当前.值记录身份 == 项.值记录身份
                            && 当前.值记录版本 == 项.预期值记录版本;
                    });
                } else if constexpr (std::is_same_v<类型, 节点直接索引创建项>) {
                    const auto* 见证 = std::get_if<节点稳定身份见证>(&项.目标);
                    return 见证 == nullptr || 节点见证当前(*见证);
                } else if constexpr (std::is_same_v<类型, 节点直接索引移除项>) {
                    const auto 当前 = 索引_->读取索引物理键(项.键, 事务序号);
                    if (!当前 || 当前->目标种类 != 索引目标种类::节点
                        || !节点见证当前(项.预期目标)) return false;
                    const auto 目标 = 节点_->读取节点(当前->节点, 事务序号);
                    return 目标 && 目标->稳定主键 == 项.预期目标.稳定主键
                        && 目标->类型 == 项.预期目标.类型
                        && 目标->版本号 == 项.预期目标.身份版本;
                } else if constexpr (std::is_same_v<类型, 节点直接节点删除项>) {
                    return 节点见证当前(项.预期当前);
                } else return true;
            }, 写项);
            if (!完整) return false;
        }
        return true;
    }

    节点直接类型化结构数据操作结果 执行带参与者类型化结构事务_(
        const 节点直接类型化结构数据操作请求& 请求,
        bool 允许空域治理,
        std::span<节点直接类型化结构事务参与者* const> 参与者组) const {
        节点直接类型化结构数据操作结果 结果;
        if (!类型化结构有效() || 请求.合同版本 != 节点直接结构服务合同版本
            || 请求.写集规则版本 != 2
            || !节点直接事务幂等身份完整(请求.安装实例身份)
            || !节点直接事务幂等身份完整(请求.幂等身份)
            || !摘要非零_(请求.请求意图摘要) || !摘要非零_(请求.执行证据摘要)
            || !请求静态完整_(请求)
            || !有效类型化结构参与者组_(参与者组)
            || (允许空域治理 && !参与者组.empty())
            || (!允许空域治理 && (请求.预期事实截止代次 == 0 || 请求.写项组.empty()))) {
            结果.状态 = 节点直接类型化结构数据操作状态::入口拒绝;
            return 结果;
        }
        const bool 含固定混合组合 = 含固定混合创建种类组合_(请求);
        const bool 固定混合形状 = 是固定混合创建形状_(请求, 参与者组);
        if (含固定混合组合 && !固定混合形状) {
            结果.状态 = 节点直接类型化结构数据操作状态::入口拒绝;
            return 结果;
        }
        for (const auto& 写项 : 请求.写项组) {
            const auto* 节点项 = std::get_if<节点直接节点创建项>(&写项);
            if (节点项 != nullptr
                && (!节点直接身份仓库::命名域已定义(节点项->命名域)
                    || !节点直接身份仓库::命名域与节点类型匹配(
                        节点项->命名域, 节点项->类型))) {
                结果.状态 = 节点直接类型化结构数据操作状态::入口拒绝;
                return 结果;
            }
        }
        auto 许可 = 事务域_->取得独占许可();
        if (!许可.有效()) {
            结果.状态 = 节点直接类型化结构数据操作状态::许可拒绝;
            return 结果;
        }
        const auto 当前代次 = 许可.读取已发布代次();
        const auto 既有 = 幂等_->读取(请求.幂等身份);
        if (既有) {
            if (既有->请求意图摘要 == 请求.请求意图摘要
                && 既有->状态 == 节点直接事务幂等记录状态::已发布) return 形成幂等读回_(*既有);
            结果.状态 = 既有->请求意图摘要 == 请求.请求意图摘要
                ? 节点直接类型化结构数据操作状态::内部不一致
                : 节点直接类型化结构数据操作状态::幂等冲突;
            return 结果;
        }
        if ((!允许空域治理 && 当前代次 != 请求.预期事实截止代次)
            || (允许空域治理 && 当前代次 != 0)) {
            结果.状态 = 节点直接类型化结构数据操作状态::版本漂移;
            return 结果;
        }
        if (!请求动态准备前完整_(请求, 许可.事务序号(), 当前代次)) {
            结果.状态 = 节点直接类型化结构数据操作状态::入口拒绝;
            return 结果;
        }
        if (当前代次 == std::numeric_limits<std::uint64_t>::max()) {
            许可.标记隔离();
            结果.状态 = 节点直接类型化结构数据操作状态::内部不一致;
            return 结果;
        }
        const auto 计划身份映射 = 形成计划身份映射_(请求);
        if (!计划身份映射) {
            结果.状态 = 节点直接类型化结构数据操作状态::入口拒绝;
            return 结果;
        }
        const auto 编码 = 编码写集_(请求, 当前代次 + 1, *计划身份映射);
        if (编码.状态 != 节点直接材料转换状态::成功) {
            结果.状态 = 编码.状态 == 节点直接材料转换状态::资源失败
                ? 节点直接类型化结构数据操作状态::资源失败
                : 节点直接类型化结构数据操作状态::入口拒绝;
            return 结果;
        }
        const auto 写集摘要 = 计算SHA256_(编码.材料);
        if (写集摘要.状态 != 节点直接材料转换状态::成功) {
            结果.状态 = 节点直接类型化结构数据操作状态::资源失败;
            return 结果;
        }
        节点直接持久准备请求 准备请求;
        准备请求.安装实例身份 = 请求.安装实例身份;
        准备请求.事务身份 = 请求.幂等身份;
        准备请求.材料格式版本 = 2;
        准备请求.请求意图摘要 = 请求.请求意图摘要;
        准备请求.执行证据摘要 = 请求.执行证据摘要;
        准备请求.写集材料 = 编码.材料;
        const auto 准备结果 = 持久证据_->准备(准备请求);
        if (准备结果.状态 == 节点直接持久端口状态::已见证) {
            许可.标记隔离();
            结果.状态 = 节点直接类型化结构数据操作状态::内部不一致;
            return 结果;
        }
        if ((准备结果.状态 != 节点直接持久端口状态::已准备
                && 准备结果.状态 != 节点直接持久端口状态::精确同义)
            || 准备结果.尝试序号 == 0) {
            结果.状态 = 准备结果.状态 == 节点直接持久端口状态::资源失败
                || 准备结果.状态 == 节点直接持久端口状态::结果未知
                ? 节点直接类型化结构数据操作状态::资源失败
                : 准备结果.状态 == 节点直接持久端口状态::冲突
                    ? 节点直接类型化结构数据操作状态::幂等冲突
                    : 节点直接类型化结构数据操作状态::内部不一致;
            return 结果;
        }
        const auto 尝试序号 = 准备结果.尝试序号;
        const bool 侧账已建立 = 幂等_->建立临时持久证据侧账(请求.幂等身份, 尝试序号);
        if (!侧账已建立) {
            节点直接持久撤销见证请求 撤销请求;
            撤销请求.安装实例身份 = 请求.安装实例身份;
            撤销请求.事务身份 = 请求.幂等身份;
            撤销请求.尝试序号 = 尝试序号;
            撤销请求.请求意图摘要 = 请求.请求意图摘要;
            撤销请求.执行证据摘要 = 请求.执行证据摘要;
            撤销请求.写集材料摘要 = 写集摘要.摘要;
            const auto 撤销见证 = 持久证据_->标记已撤销未发布(撤销请求);
            if ((撤销见证.状态 == 节点直接持久端口状态::已撤销未发布
                    || 撤销见证.状态 == 节点直接持久端口状态::精确同义)
                && 撤销见证.尝试序号 == 尝试序号) {
                结果.状态 = 节点直接类型化结构数据操作状态::资源失败;
                return 结果;
            }
            许可.标记隔离();
            结果.状态 = 节点直接类型化结构数据操作状态::内部不一致;
            return 结果;
        }

        if (固定混合形状) {
            const auto& 合同项 = std::get<节点直接类型合同发布项>(请求.写项组[0]);
            const auto& 节点项 = std::get<节点直接节点创建项>(请求.写项组[1]);
            const auto& 值项 = std::get<节点直接类型化值发布项>(请求.写项组[2]);
            std::optional<节点直接类型合同候选> 合同候选;
            std::optional<节点直接身份未发布候选> 节点候选;
            std::optional<节点直接类型化值候选> 值候选;
            std::vector<正式关系候选> 关系候选组;
            节点句柄 节点句柄值;
            节点稳定身份见证 节点见证;
            类型合同读回 合同读回;
            类型化值读回 值读回;
            std::vector<关系稳定身份见证> 关系见证组;
            std::vector<std::pair<节点句柄, 节点句柄>> 关系端点句柄组;
            std::optional<节点直接事务幂等候选> 幂等候选;
            节点直接事务幂等记录 计划记录;
            bool 候选完整 = true;
            auto 首次失败 = 节点直接类型化结构数据操作状态::内部不一致;
            try {
                关系候选组.reserve(5);
                关系见证组.reserve(5);
                关系端点句柄组.reserve(5);
            } catch (...) {
                候选完整 = false;
                首次失败 = 节点直接类型化结构数据操作状态::资源失败;
            }

            const auto 已发布合同 = 类型合同_->读取精确合同(
                合同项.合同身份, 合同项.合同版本);
            if (候选完整) {
                合同读回 = 已发布合同.value_or(类型合同读回{
                    合同项.合同身份, 合同项.命名空间, 合同项.合同版本,
                    合同项.表示, 合同项.值域, 合同项.所有者服务, 合同项.生命周期,
                    合同项.直接兼容组, 当前代次 + 1, 当前代次 + 1});
                if (!已发布合同) {
                    auto 建立 = 类型合同_->结构化发布合同未发布候选(
                        合同读回, 许可.事务序号());
                    if (建立.状态 == 节点直接仓候选操作状态::已形成候选 && 建立.候选
                        && 建立.候选->读取候选合同() == 合同读回) {
                        合同候选.emplace(std::move(*建立.候选));
                    } else {
                        候选完整 = false;
                        首次失败 = 建立.状态 == 节点直接仓候选操作状态::资源失败
                            ? 节点直接类型化结构数据操作状态::资源失败
                            : 建立.状态 == 节点直接仓候选操作状态::版本漂移
                                ? 节点直接类型化结构数据操作状态::版本漂移
                                : 节点直接类型化结构数据操作状态::内部不一致;
                    }
                }
            }
            if (候选完整) {
                const auto* 映射 = 查找计划身份_(*计划身份映射, 节点项.局部身份);
                if (映射 == nullptr
                    || 映射->稳定身份命名域 != static_cast<std::uint64_t>(节点项.命名域)) {
                    候选完整 = false;
                    首次失败 = 节点直接类型化结构数据操作状态::入口拒绝;
                } else {
                    const 节点稳定主键 稳定主键{映射->稳定身份命名域, 映射->计划键};
                    auto 建立 = 节点_->结构化创建节点未发布候选(
                        节点项.类型, 稳定主键, 许可.事务序号());
                    if (建立.状态 == 节点直接身份写入状态::已形成候选 && 建立.候选) {
                        节点句柄值 = 建立.当前身份;
                        节点见证 = {稳定主键, 节点项.类型,
                            static_cast<std::uint32_t>(映射->初始版本)};
                        const auto 读回 = 节点_->读取节点(节点句柄值, 许可.事务序号());
                        if (读回 && 读回->稳定主键 == 稳定主键
                            && 读回->类型 == 节点项.类型
                            && 读回->版本号 == 映射->初始版本
                            && 读回->状态 == 记录状态::有效) {
                            节点候选.emplace(std::move(*建立.候选));
                        } else {
                            候选完整 = false;
                        }
                    } else {
                        候选完整 = false;
                        首次失败 = 建立.状态 == 节点直接身份写入状态::资源失败
                            ? 节点直接类型化结构数据操作状态::资源失败
                            : 建立.状态 == 节点直接身份写入状态::版本漂移
                                ? 节点直接类型化结构数据操作状态::版本漂移
                                : 节点直接类型化结构数据操作状态::入口拒绝;
                    }
                }
            }
            if (候选完整) {
                const auto* 映射 = 查找计划身份_(*计划身份映射, 值项.值记录局部身份);
                const auto* 来源联合 = std::get_if<节点直接节点端点引用>(&值项.来源);
                const auto* 来源见证 = 来源联合 == nullptr
                    ? nullptr : std::get_if<节点稳定身份见证>(来源联合);
                if (映射 == nullptr || 来源见证 == nullptr) {
                    候选完整 = false;
                    首次失败 = 节点直接类型化结构数据操作状态::入口拒绝;
                } else {
                    值读回 = {节点见证, 值项.类型合同身份, 值项.类型合同版本,
                        {映射->稳定身份命名域, 映射->计划键}, 映射->初始版本,
                        true, 值项.材料, *来源见证, std::nullopt,
                        当前代次 + 1, 当前代次 + 1};
                    auto 建立 = 类型化值_->结构化发布值未发布候选(
                        值读回, 许可.事务序号());
                    if (建立.状态 == 节点直接仓候选操作状态::已形成候选 && 建立.候选
                        && 建立.候选->读取候选值() == 值读回) {
                        值候选.emplace(std::move(*建立.候选));
                    } else {
                        候选完整 = false;
                        首次失败 = 建立.状态 == 节点直接仓候选操作状态::资源失败
                            ? 节点直接类型化结构数据操作状态::资源失败
                            : 建立.状态 == 节点直接仓候选操作状态::版本漂移
                                ? 节点直接类型化结构数据操作状态::版本漂移
                                : 节点直接类型化结构数据操作状态::内部不一致;
                    }
                }
            }
            for (std::size_t 序号 = 0; 候选完整 && 序号 < 5; ++序号) {
                const auto& 项 = std::get<节点直接关系创建项>(请求.写项组[序号 + 3]);
                const auto* 映射 = 查找计划身份_(*计划身份映射, 项.局部身份);
                const auto& 目标见证 = std::get<节点稳定身份见证>(项.目标端);
                const auto 目标当前 = 节点_->读取稳定主键当前身份(
                    目标见证.稳定主键, 许可.事务序号());
                if (映射 == nullptr || !目标当前.当前身份 || !目标当前.当前记录) {
                    候选完整 = false;
                    首次失败 = 节点直接类型化结构数据操作状态::入口拒绝;
                    break;
                }
                const 关系稳定主键 稳定主键{映射->稳定身份命名域, 映射->计划键};
                auto 建立 = 关系_->结构化创建关系未发布候选(
                    项.类型, 节点句柄值, *目标当前.当前身份, 项.角色或顺序,
                    稳定主键, 许可.事务序号());
                const 关系稳定身份见证 见证{稳定主键, 项.类型,
                    static_cast<std::uint32_t>(映射->初始版本),
                    节点见证, 目标见证, 项.角色或顺序};
                const auto 读回 = 关系_->读取稳定主键当前关系(稳定主键, 许可.事务序号());
                if ((建立.状态 == 正式关系操作状态::已创建候选
                        || 建立.状态 == 正式关系操作状态::已变更候选)
                    && 建立.候选 && 建立.当前关系
                    && 读回.状态 == 稳定关系当前读取状态::当前有效 && 读回.记录
                    && 读回.记录->稳定主键 == 稳定主键
                    && 读回.记录->类型 == 项.类型
                    && 读回.记录->源节点 == 节点句柄值
                    && 读回.记录->目标节点 == *目标当前.当前身份
                    && 读回.记录->顺序号 == 项.角色或顺序) {
                    关系候选组.push_back(std::move(*建立.候选));
                    关系见证组.push_back(见证);
                    关系端点句柄组.push_back({节点句柄值, *目标当前.当前身份});
                } else {
                    候选完整 = false;
                    首次失败 = 建立.状态 == 正式关系操作状态::入口拒绝_版本漂移
                        ? 节点直接类型化结构数据操作状态::版本漂移
                        : 建立.状态 == 正式关系操作状态::内部不一致
                            ? 节点直接类型化结构数据操作状态::内部不一致
                            : 节点直接类型化结构数据操作状态::入口拒绝;
                }
            }

            if (候选完整) {
                try {
                    std::vector<std::uint8_t> 结果材料 = 编码.材料;
                    bool 可编码 = 追加无符号_(结果材料, 0x4C314D4E4F444553ULL, 8)
                        && 追加无符号_(结果材料, 1, 8)
                        && 追加节点见证_(结果材料, 节点见证)
                        && 追加无符号_(结果材料, 0x4C314D56414C5545ULL, 8)
                        && 追加无符号_(结果材料, 1, 8)
                        && 追加无符号_(结果材料, 值读回.值记录身份.命名域, 8)
                        && 追加无符号_(结果材料, 值读回.值记录身份.键值, 8)
                        && 追加无符号_(结果材料, 值读回.值记录版本, 8)
                        && 追加无符号_(结果材料, 0x4C314D52454C4154ULL, 8)
                        && 追加无符号_(结果材料, 5, 8);
                    for (const auto& 见证 : 关系见证组) {
                        可编码 = 可编码
                            && 追加无符号_(结果材料, 见证.稳定主键.命名域, 8)
                            && 追加无符号_(结果材料, 见证.稳定主键.键值, 8)
                            && 追加无符号_(结果材料, static_cast<std::uint32_t>(见证.类型), 4)
                            && 追加无符号_(结果材料, 见证.关系版本, 4)
                            && 追加节点见证_(结果材料, 见证.源端)
                            && 追加节点见证_(结果材料, 见证.目标端)
                            && 追加无符号_(结果材料,
                                static_cast<std::uint64_t>(见证.角色或顺序), 8);
                    }
                    const auto 摘要 = 可编码 ? 计算SHA256_(结果材料) : 节点直接摘要计算结果{};
                    if (摘要.状态 != 节点直接材料转换状态::成功) {
                        候选完整 = false;
                        首次失败 = 节点直接类型化结构数据操作状态::资源失败;
                    } else {
                        计划记录.安装实例身份 = 请求.安装实例身份;
                        计划记录.幂等身份 = 请求.幂等身份;
                        计划记录.请求意图摘要 = 请求.请求意图摘要;
                        计划记录.执行证据摘要 = 请求.执行证据摘要;
                        计划记录.材料格式版本 = 2;
                        计划记录.写集规则版本 = 2;
                        计划记录.写集材料 = 编码.材料;
                        计划记录.状态 = 节点直接事务幂等记录状态::待发布;
                        计划记录.发布代次 = 当前代次 + 1;
                        计划记录.结果摘要 = 摘要.摘要;
                        计划记录.节点组 = {节点见证};
                        计划记录.类型化值组 = {值读回};
                        计划记录.关系组 = 关系见证组;
                        auto 建立 = 幂等_->结构化建立记录未发布候选(
                            计划记录, 许可.事务序号());
                        if (建立.状态 == 节点直接仓候选操作状态::已形成候选 && 建立.候选
                            && 建立.候选->读取候选记录() == 计划记录) {
                            幂等候选.emplace(std::move(*建立.候选));
                        } else {
                            候选完整 = false;
                            首次失败 = 建立.状态 == 节点直接仓候选操作状态::资源失败
                                ? 节点直接类型化结构数据操作状态::资源失败
                                : 节点直接类型化结构数据操作状态::内部不一致;
                        }
                    }
                } catch (...) {
                    候选完整 = false;
                    首次失败 = 节点直接类型化结构数据操作状态::资源失败;
                }
            }

            if (候选完整 && 合同候选) {
                if (类型合同_->确认候选(*合同候选, 许可.事务序号())
                        != 节点直接仓候选操作状态::已确认待发布) 候选完整 = false;
            }
            if (候选完整 && (节点_->确认节点候选(*节点候选, 许可.事务序号()).状态
                        != 节点直接身份写入状态::已确认待发布)) 候选完整 = false;
            if (候选完整 && (类型化值_->确认候选(*值候选, 许可.事务序号())
                        != 节点直接仓候选操作状态::已确认待发布)) 候选完整 = false;
            for (std::size_t 序号 = 0; 候选完整 && 序号 < 关系候选组.size(); ++序号) {
                if (关系_->确认候选(关系候选组[序号], 许可.事务序号())
                        != 正式关系操作状态::已确认待发布) 候选完整 = false;
            }
            if (候选完整 && (幂等_->确认候选(*幂等候选, 许可.事务序号())
                        != 节点直接仓候选操作状态::已确认待发布)) 候选完整 = false;

            auto 撤销业务候选 = [&]() noexcept {
                bool 完整 = true;
                if (幂等候选) {
                    完整 = 幂等_->撤销候选(*幂等候选, 许可.事务序号())
                        == 节点直接仓候选操作状态::已撤销 && 完整;
                }
                for (std::size_t 序号 = 关系候选组.size(); 序号 > 0; --序号) {
                    完整 = 关系_->撤销候选(关系候选组[序号 - 1], 许可.事务序号())
                        == 正式关系操作状态::已撤销 && 完整;
                }
                if (值候选) {
                    完整 = 类型化值_->撤销候选(*值候选, 许可.事务序号())
                        == 节点直接仓候选操作状态::已撤销 && 完整;
                }
                if (节点候选) {
                    完整 = 节点_->撤销节点候选(*节点候选, 许可.事务序号()).状态
                        == 节点直接身份写入状态::已撤销 && 完整;
                }
                if (合同候选) {
                    完整 = 类型合同_->撤销候选(*合同候选, 许可.事务序号())
                        == 节点直接仓候选操作状态::已撤销 && 完整;
                }
                return 完整;
            };
            auto 撤销并返回 = [&]() {
                const bool 已完整撤销 = 撤销业务候选();
                节点直接持久撤销见证请求 撤销请求;
                撤销请求.安装实例身份 = 请求.安装实例身份;
                撤销请求.事务身份 = 请求.幂等身份;
                撤销请求.尝试序号 = 尝试序号;
                撤销请求.请求意图摘要 = 请求.请求意图摘要;
                撤销请求.执行证据摘要 = 请求.执行证据摘要;
                撤销请求.写集材料摘要 = 写集摘要.摘要;
                const auto 撤销见证 = 已完整撤销
                    ? 持久证据_->标记已撤销未发布(撤销请求)
                    : 节点直接持久端口结果{};
                if (已完整撤销
                    && (撤销见证.状态 == 节点直接持久端口状态::已撤销未发布
                        || 撤销见证.状态 == 节点直接持久端口状态::精确同义)
                    && 撤销见证.尝试序号 == 尝试序号
                    && 幂等_->清除临时持久证据侧账(请求.幂等身份, 尝试序号)) {
                    结果.状态 = 首次失败;
                    return 结果;
                }
                许可.标记隔离();
                结果.状态 = 节点直接类型化结构数据操作状态::内部不一致;
                return 结果;
            };
            if (!候选完整) return 撤销并返回();

            if (合同候选) {
                if (类型合同_->完成发布(*合同候选, 许可.事务序号())
                    != 节点直接仓候选操作状态::已发布) {
                    许可.标记隔离(); 结果.状态 = 节点直接类型化结构数据操作状态::内部不一致; return 结果;
                }
            }
            节点_->完成发布节点候选(*节点候选, 许可.事务序号());
            if (类型化值_->完成发布(*值候选, 许可.事务序号())
                != 节点直接仓候选操作状态::已发布) {
                许可.标记隔离(); 结果.状态 = 节点直接类型化结构数据操作状态::内部不一致; return 结果;
            }
            for (std::size_t 序号 = 0; 序号 < 关系候选组.size(); ++序号) {
                关系_->完成发布(关系候选组[序号], 许可.事务序号());
            }
            if (幂等_->完成发布(*幂等候选, 许可.事务序号())
                != 节点直接仓候选操作状态::已发布) {
                许可.标记隔离(); 结果.状态 = 节点直接类型化结构数据操作状态::内部不一致; return 结果;
            }
            if (!事务域_->推进普通已发布代次(许可, 当前代次)) {
                许可.标记隔离(); 结果.状态 = 节点直接类型化结构数据操作状态::内部不一致; return 结果;
            }

            bool 读回一致 = true;
            const auto 最终合同 = 类型合同_->读取精确合同(合同项.合同身份, 合同项.合同版本);
            读回一致 = 最终合同 && 类型合同写项静态内容等于读回_(合同项, *最终合同)
                && (!合同候选 || (最终合同->首次发布代次 == 当前代次 + 1
                    && 最终合同->当前状态发布代次 == 当前代次 + 1));
            const auto 最终节点 = 节点_->读取稳定主键当前身份(节点见证.稳定主键, 许可.事务序号());
            读回一致 = 最终节点.状态 == 稳定主键当前身份状态::当前有效
                && 最终节点.当前身份 && 最终节点.当前记录
                && *最终节点.当前身份 == 节点句柄值
                && 最终节点.当前记录->稳定主键 == 节点见证.稳定主键
                && 最终节点.当前记录->类型 == 节点见证.类型
                && 最终节点.当前记录->版本号 == 节点见证.身份版本 && 读回一致;
            const auto 最终值组 = 类型化值_->读取所属身份当前值组(节点见证.稳定主键);
            读回一致 = std::find(最终值组.begin(), 最终值组.end(), 值读回) != 最终值组.end()
                && 读回一致;
            for (std::size_t 序号 = 0; 序号 < 关系见证组.size(); ++序号) {
                const auto 最终关系 = 关系_->读取稳定主键当前关系(
                    关系见证组[序号].稳定主键, 许可.事务序号());
                读回一致 = 最终关系.状态 == 稳定关系当前读取状态::当前有效
                    && 最终关系.记录
                    && 最终关系.记录->类型 == 关系见证组[序号].类型
                    && 最终关系.记录->源节点 == 关系端点句柄组[序号].first
                    && 最终关系.记录->目标节点 == 关系端点句柄组[序号].second
                    && 最终关系.记录->顺序号 == 关系见证组[序号].角色或顺序
                    && 读回一致;
            }
            const auto 最终幂等 = 幂等_->读取(请求.幂等身份);
            读回一致 = 最终幂等 && 最终幂等->状态 == 节点直接事务幂等记录状态::已发布
                && 最终幂等->发布代次 == 当前代次 + 1
                && 最终幂等->结果摘要 == 计划记录.结果摘要 && 读回一致;
            结果 = 形成幂等读回_(计划记录);
            结果.状态 = 读回一致
                ? 节点直接类型化结构数据操作状态::已提交
                : 节点直接类型化结构数据操作状态::内部不一致;
            if (!读回一致) 许可.标记隔离();

            节点直接持久发布见证请求 发布请求;
            发布请求.安装实例身份 = 请求.安装实例身份;
            发布请求.事务身份 = 请求.幂等身份;
            发布请求.尝试序号 = 尝试序号;
            发布请求.请求意图摘要 = 请求.请求意图摘要;
            发布请求.执行证据摘要 = 请求.执行证据摘要;
            发布请求.发布代次 = 当前代次 + 1;
            发布请求.结果摘要 = 计划记录.结果摘要;
            const auto 发布见证 = 持久证据_->标记已发布(发布请求);
            if ((发布见证.状态 == 节点直接持久端口状态::已见证
                    || 发布见证.状态 == 节点直接持久端口状态::精确同义)
                && 发布见证.尝试序号 == 尝试序号
                && 幂等_->单调记录持久证据状态(
                    请求.幂等身份, 尝试序号, 持久证据状态::已与内存代次一致)) {
                结果.持久状态 = 持久证据状态::已与内存代次一致;
            } else if (发布见证.状态 == 节点直接持久端口状态::结果未知
                || 发布见证.状态 == 节点直接持久端口状态::资源失败) {
                (void)幂等_->单调记录持久证据状态(
                    请求.幂等身份, 尝试序号, 持久证据状态::持久化结果未知);
                结果.持久状态 = 持久证据状态::持久化结果未知;
            } else {
                (void)幂等_->单调记录持久证据状态(
                    请求.幂等身份, 尝试序号, 持久证据状态::持久证据损坏);
                许可.标记隔离();
                结果.状态 = 节点直接类型化结构数据操作状态::内部不一致;
                结果.持久状态 = 持久证据状态::持久证据损坏;
            }
            return 结果;
        }

        if (允许空域治理 && 请求.写项组.empty() && 请求.读回规格.项目组.empty()
            && 计划身份映射->empty()) {
            节点直接事务幂等记录 计划记录;
            计划记录.安装实例身份 = 请求.安装实例身份;
            计划记录.幂等身份 = 请求.幂等身份;
            计划记录.请求意图摘要 = 请求.请求意图摘要;
            计划记录.执行证据摘要 = 请求.执行证据摘要;
            计划记录.材料格式版本 = 2;
            计划记录.写集规则版本 = 2;
            计划记录.写集材料 = 编码.材料;
            计划记录.状态 = 节点直接事务幂等记录状态::待发布;
            计划记录.发布代次 = 当前代次 + 1;
            计划记录.结果摘要 = 写集摘要.摘要;

            auto 幂等候选结果 = 幂等_->结构化建立记录未发布候选(
                计划记录, 许可.事务序号());
            if (幂等候选结果.状态 != 节点直接仓候选操作状态::已形成候选
                || !幂等候选结果.候选) {
                const auto 首次状态 = 幂等候选结果.状态 == 节点直接仓候选操作状态::资源失败
                    ? 节点直接类型化结构数据操作状态::资源失败
                    : 节点直接类型化结构数据操作状态::内部不一致;
                节点直接持久撤销见证请求 撤销请求;
                撤销请求.安装实例身份 = 请求.安装实例身份;
                撤销请求.事务身份 = 请求.幂等身份;
                撤销请求.尝试序号 = 尝试序号;
                撤销请求.请求意图摘要 = 请求.请求意图摘要;
                撤销请求.执行证据摘要 = 请求.执行证据摘要;
                撤销请求.写集材料摘要 = 写集摘要.摘要;
                const auto 撤销见证 = 持久证据_->标记已撤销未发布(撤销请求);
                if ((撤销见证.状态 == 节点直接持久端口状态::已撤销未发布
                        || 撤销见证.状态 == 节点直接持久端口状态::精确同义)
                    && 撤销见证.尝试序号 == 尝试序号
                    && 幂等_->清除临时持久证据侧账(请求.幂等身份, 尝试序号)) {
                    结果.状态 = 首次状态;
                    return 结果;
                }
                许可.标记隔离();
                结果.状态 = 节点直接类型化结构数据操作状态::内部不一致;
                return 结果;
            }
            auto& 幂等候选 = *幂等候选结果.候选;
            if (幂等_->确认候选(幂等候选, 许可.事务序号())
                    != 节点直接仓候选操作状态::已确认待发布
                || 幂等_->完成发布(幂等候选, 许可.事务序号())
                    != 节点直接仓候选操作状态::已发布
                || !事务域_->推进普通已发布代次(许可, 当前代次)) {
                许可.标记隔离();
                结果.状态 = 节点直接类型化结构数据操作状态::内部不一致;
                return 结果;
            }

            节点直接持久发布见证请求 发布请求;
            发布请求.安装实例身份 = 请求.安装实例身份;
            发布请求.事务身份 = 请求.幂等身份;
            发布请求.尝试序号 = 尝试序号;
            发布请求.请求意图摘要 = 请求.请求意图摘要;
            发布请求.执行证据摘要 = 请求.执行证据摘要;
            发布请求.发布代次 = 当前代次 + 1;
            发布请求.结果摘要 = 计划记录.结果摘要;
            const auto 发布见证 = 持久证据_->标记已发布(发布请求);
            结果.状态 = 节点直接类型化结构数据操作状态::已提交;
            结果.发布代次 = 当前代次 + 1;
            if ((发布见证.状态 == 节点直接持久端口状态::已见证
                    || 发布见证.状态 == 节点直接持久端口状态::精确同义)
                && 发布见证.尝试序号 == 尝试序号
                && 幂等_->单调记录持久证据状态(
                    请求.幂等身份, 尝试序号, 持久证据状态::已与内存代次一致)) {
                结果.持久状态 = 持久证据状态::已与内存代次一致;
            } else if (发布见证.状态 == 节点直接持久端口状态::结果未知
                || 发布见证.状态 == 节点直接持久端口状态::资源失败) {
                (void)幂等_->单调记录持久证据状态(
                    请求.幂等身份, 尝试序号, 持久证据状态::持久化结果未知);
                结果.持久状态 = 持久证据状态::持久化结果未知;
            } else {
                (void)幂等_->单调记录持久证据状态(
                    请求.幂等身份, 尝试序号, 持久证据状态::持久证据损坏);
                许可.标记隔离();
                结果.状态 = 节点直接类型化结构数据操作状态::内部不一致;
                结果.持久状态 = 持久证据状态::持久证据损坏;
            }
            return 结果;
        }

        const bool 仅含类型合同 = !请求.写项组.empty()
            && 请求.读回规格.项目组.empty()
            && std::all_of(请求.写项组.begin(), 请求.写项组.end(), [](const auto& 写项) {
                return std::holds_alternative<节点直接类型合同发布项>(写项);
            });
        if (仅含类型合同) {
            std::vector<类型合同读回> 计划合同组;
            std::vector<节点直接类型合同候选> 合同候选组;
            bool 候选完整 = true;
            节点直接类型化结构数据操作状态 首次失败 = 节点直接类型化结构数据操作状态::内部不一致;
            try {
                计划合同组.reserve(请求.写项组.size());
                合同候选组.reserve(请求.写项组.size());
                for (const auto& 写项 : 请求.写项组) {
                    const auto& 项 = std::get<节点直接类型合同发布项>(写项);
                    计划合同组.push_back({项.合同身份, 项.命名空间, 项.合同版本,
                        项.表示, 项.值域, 项.所有者服务, 项.生命周期,
                        项.直接兼容组, 当前代次 + 1, 当前代次 + 1});
                }
                std::sort(计划合同组.begin(), 计划合同组.end(), [](const auto& 左, const auto& 右) {
                    if (左.合同身份.命名域 != 右.合同身份.命名域)
                        return 左.合同身份.命名域 < 右.合同身份.命名域;
                    if (左.合同身份.键值 != 右.合同身份.键值)
                        return 左.合同身份.键值 < 右.合同身份.键值;
                    return 左.合同版本 < 右.合同版本;
                });
                std::vector<std::pair<std::uint64_t, std::uint64_t>> 模拟高水位组;
                模拟高水位组.reserve(计划合同组.size());
                for (std::size_t 序号 = 0; 序号 < 计划合同组.size(); ++序号) {
                    const auto& 计划合同 = 计划合同组[序号];
                    if (序号 > 0
                        && 计划合同组[序号 - 1].合同身份 == 计划合同.合同身份) {
                        候选完整 = false;
                        首次失败 = 计划合同组[序号 - 1].合同版本 == 计划合同.合同版本
                            ? 节点直接类型化结构数据操作状态::入口拒绝
                            : 节点直接类型化结构数据操作状态::版本漂移;
                        break;
                    }
                    if (const auto 当前 = 类型合同_->读取精确合同(
                            计划合同.合同身份, 计划合同.合同版本)) {
                        const auto 写项位置 = std::find_if(
                            请求.写项组.begin(), 请求.写项组.end(), [&](const auto& 写项) {
                                const auto* 项 = std::get_if<节点直接类型合同发布项>(&写项);
                                return 项 && 项->合同身份 == 计划合同.合同身份
                                    && 项->合同版本 == 计划合同.合同版本;
                            });
                        if (写项位置 != 请求.写项组.end()) {
                            const bool 精确同义 = 类型合同写项静态内容等于读回_(
                                std::get<节点直接类型合同发布项>(*写项位置), *当前);
                            (void)精确同义;
                        }
                        候选完整 = false;
                        首次失败 = 节点直接类型化结构数据操作状态::入口拒绝;
                        break;
                    }
                    const auto 命名域 = 计划合同.合同身份.命名域;
                    auto 高水位位置 = std::find_if(
                        模拟高水位组.begin(), 模拟高水位组.end(),
                        [&](const auto& 项) { return 项.first == 命名域; });
                    if (高水位位置 == 模拟高水位组.end()) {
                        模拟高水位组.push_back({命名域, 类型合同_->读取命名域高水位(命名域)});
                        高水位位置 = std::prev(模拟高水位组.end());
                    }
                    if (高水位位置->second == std::numeric_limits<std::uint64_t>::max()
                        || 计划合同.合同身份.键值 != 高水位位置->second + 1) {
                        候选完整 = false;
                        首次失败 = 节点直接类型化结构数据操作状态::版本漂移;
                        break;
                    }
                    高水位位置->second = 计划合同.合同身份.键值;
                }
            } catch (...) {
                候选完整 = false;
                首次失败 = 节点直接类型化结构数据操作状态::资源失败;
            }

            std::size_t 已进入事务参与者数量 = 0;
            if (候选完整) {
                const auto 复核状态 = 第一写前复核类型化结构参与者组_(
                    请求, 许可.事务序号(), 当前代次,
                    参与者组, 已进入事务参与者数量);
                if (复核状态 != 节点直接类型化结构事务参与者状态::第一写前已复核) {
                    候选完整 = false;
                    首次失败 = 映射类型化结构参与者失败_(复核状态);
                }
            }
            if (候选完整) {
                try {
                    for (const auto& 计划合同 : 计划合同组) {
                        auto 建立 = 类型合同_->结构化发布合同未发布候选(
                            计划合同, 许可.事务序号());
                        if (建立.状态 != 节点直接仓候选操作状态::已形成候选
                            || !建立.候选) {
                            候选完整 = false;
                            首次失败 = 建立.状态 == 节点直接仓候选操作状态::资源失败
                                ? 节点直接类型化结构数据操作状态::资源失败
                                : 建立.状态 == 节点直接仓候选操作状态::版本漂移
                                    ? 节点直接类型化结构数据操作状态::版本漂移
                                    : 节点直接类型化结构数据操作状态::入口拒绝;
                            break;
                        }
                        合同候选组.push_back(std::move(*建立.候选));
                    }
                } catch (...) {
                    候选完整 = false;
                    首次失败 = 节点直接类型化结构数据操作状态::资源失败;
                }
            }

            auto 撤销合同候选 = [&]() noexcept {
                bool 完整 = true;
                for (std::size_t 序号 = 合同候选组.size(); 序号 > 0; --序号) {
                    完整 = 类型合同_->撤销候选(合同候选组[序号 - 1], 许可.事务序号())
                        == 节点直接仓候选操作状态::已撤销 && 完整;
                }
                return 完整;
            };
            节点直接事务幂等记录 计划记录;
            std::optional<节点直接事务幂等候选> 幂等候选;
            类型化结构参与者准备材料_ 参与者准备材料;
            if (候选完整) {
                计划记录.安装实例身份 = 请求.安装实例身份;
                计划记录.幂等身份 = 请求.幂等身份;
                计划记录.请求意图摘要 = 请求.请求意图摘要;
                计划记录.执行证据摘要 = 请求.执行证据摘要;
                计划记录.材料格式版本 = 2;
                计划记录.写集规则版本 = 2;
                计划记录.写集材料 = 编码.材料;
                计划记录.状态 = 节点直接事务幂等记录状态::待发布;
                计划记录.发布代次 = 当前代次 + 1;
                计划记录.结果摘要 = 写集摘要.摘要;
                auto 幂等建立 = 幂等_->结构化建立记录未发布候选(计划记录, 许可.事务序号());
                if (幂等建立.状态 == 节点直接仓候选操作状态::已形成候选 && 幂等建立.候选) {
                    幂等候选.emplace(std::move(*幂等建立.候选));
                } else {
                    候选完整 = false;
                    首次失败 = 幂等建立.状态 == 节点直接仓候选操作状态::资源失败
                        ? 节点直接类型化结构数据操作状态::资源失败
                        : 节点直接类型化结构数据操作状态::内部不一致;
                }
            }
            if (候选完整) {
                const auto 准备状态 = 准备类型化结构参与者组_(
                    请求, *计划身份映射, 计划记录, 许可.事务序号(), 当前代次,
                    参与者组, 已进入事务参与者数量, 参与者准备材料);
                if (准备状态 != 节点直接类型化结构事务参与者状态::已准备) {
                    候选完整 = false;
                    首次失败 = 映射类型化结构参与者失败_(准备状态);
                }
            }
            if (候选完整) {
                for (auto& 候选 : 合同候选组) {
                    if (类型合同_->确认候选(候选, 许可.事务序号())
                        != 节点直接仓候选操作状态::已确认待发布) {
                        候选完整 = false;
                        首次失败 = 节点直接类型化结构数据操作状态::内部不一致;
                        break;
                    }
                }
            }
            if (候选完整
                && 幂等_->确认候选(*幂等候选, 许可.事务序号())
                    != 节点直接仓候选操作状态::已确认待发布) {
                候选完整 = false;
                首次失败 = 节点直接类型化结构数据操作状态::内部不一致;
            }
            if (候选完整) {
                const auto 确认状态 = 确认类型化结构参与者组_(参与者组);
                if (确认状态 != 节点直接类型化结构事务参与者状态::已确认待发布) {
                    候选完整 = false;
                    首次失败 = 映射类型化结构参与者失败_(确认状态);
                }
            }
            if (候选完整) {
                    bool 发布完整 = true;
                    for (auto& 候选 : 合同候选组) {
                        发布完整 = 类型合同_->完成发布(候选, 许可.事务序号())
                            == 节点直接仓候选操作状态::已发布 && 发布完整;
                    }
                    发布完整 = 幂等_->完成发布(*幂等候选, 许可.事务序号())
                        == 节点直接仓候选操作状态::已发布 && 发布完整;
                    发布完整 = 发布完整 && 完成类型化结构参与者发布_(参与者组);
                    发布完整 = 发布完整 && 事务域_->推进普通已发布代次(许可, 当前代次);
                    bool 读回一致 = 发布完整;
                    for (const auto& 计划合同 : 计划合同组) {
                        const auto 当前 = 类型合同_->读取精确合同(
                            计划合同.合同身份, 计划合同.合同版本);
                        读回一致 = 当前 && *当前 == 计划合同 && 读回一致;
                    }
                    if (!读回一致) 许可.标记隔离();
                    结果 = 形成幂等读回_(计划记录);
                    结果.状态 = 读回一致
                        ? 节点直接类型化结构数据操作状态::已提交
                        : 节点直接类型化结构数据操作状态::内部不一致;
                    节点直接持久发布见证请求 发布请求;
                    发布请求.安装实例身份 = 请求.安装实例身份;
                    发布请求.事务身份 = 请求.幂等身份;
                    发布请求.尝试序号 = 尝试序号;
                    发布请求.请求意图摘要 = 请求.请求意图摘要;
                    发布请求.执行证据摘要 = 请求.执行证据摘要;
                    发布请求.发布代次 = 当前代次 + 1;
                    发布请求.结果摘要 = 计划记录.结果摘要;
                    const auto 发布见证 = 持久证据_->标记已发布(发布请求);
                    if ((发布见证.状态 == 节点直接持久端口状态::已见证
                            || 发布见证.状态 == 节点直接持久端口状态::精确同义)
                        && 发布见证.尝试序号 == 尝试序号
                        && 幂等_->单调记录持久证据状态(
                            请求.幂等身份, 尝试序号, 持久证据状态::已与内存代次一致)) {
                        结果.持久状态 = 持久证据状态::已与内存代次一致;
                    } else if (发布见证.状态 == 节点直接持久端口状态::结果未知
                        || 发布见证.状态 == 节点直接持久端口状态::资源失败) {
                        (void)幂等_->单调记录持久证据状态(
                            请求.幂等身份, 尝试序号, 持久证据状态::持久化结果未知);
                        结果.持久状态 = 持久证据状态::持久化结果未知;
                    } else {
                        (void)幂等_->单调记录持久证据状态(
                            请求.幂等身份, 尝试序号, 持久证据状态::持久证据损坏);
                        许可.标记隔离();
                        结果.状态 = 节点直接类型化结构数据操作状态::内部不一致;
                        结果.持久状态 = 持久证据状态::持久证据损坏;
                    }
                    return 结果;
            }

            const bool 参与者已完整撤销 = 逆序撤销类型化结构参与者组_(
                参与者组, 已进入事务参与者数量);
            const bool 幂等已完整撤销 = !幂等候选
                || 幂等_->撤销候选(*幂等候选, 许可.事务序号())
                    == 节点直接仓候选操作状态::已撤销;
            const bool 已完整撤销 = 参与者已完整撤销
                && 幂等已完整撤销 && 撤销合同候选();
            节点直接持久撤销见证请求 撤销请求;
            撤销请求.安装实例身份 = 请求.安装实例身份;
            撤销请求.事务身份 = 请求.幂等身份;
            撤销请求.尝试序号 = 尝试序号;
            撤销请求.请求意图摘要 = 请求.请求意图摘要;
            撤销请求.执行证据摘要 = 请求.执行证据摘要;
            撤销请求.写集材料摘要 = 写集摘要.摘要;
            const auto 撤销见证 = 持久证据_->标记已撤销未发布(撤销请求);
            if (已完整撤销
                && (撤销见证.状态 == 节点直接持久端口状态::已撤销未发布
                    || 撤销见证.状态 == 节点直接持久端口状态::精确同义)
                && 撤销见证.尝试序号 == 尝试序号
                && 幂等_->清除临时持久证据侧账(请求.幂等身份, 尝试序号)) {
                结果.状态 = 首次失败;
                return 结果;
            }
            许可.标记隔离();
            结果.状态 = 节点直接类型化结构数据操作状态::内部不一致;
            return 结果;
        }

        const bool 仅含关系失效 = !请求.写项组.empty()
            && std::all_of(请求.写项组.begin(), 请求.写项组.end(), [](const auto& 写项) {
                return std::holds_alternative<节点直接关系失效项>(写项);
            })
            && std::all_of(请求.读回规格.项目组.begin(), 请求.读回规格.项目组.end(),
                [](const auto& 读回) {
                    return 读回.种类 == 节点直接发布后读回对象种类::已失效关系
                        && std::holds_alternative<关系稳定主键>(读回.身份);
                });
        if (仅含关系失效) {
            std::vector<const 节点直接关系失效项*> 失效项组;
            std::vector<关系句柄> 失效关系句柄组;
            std::vector<关系稳定身份见证> 失效后见证组;
            std::vector<正式关系候选> 关系候选组;
            bool 候选完整 = true;
            节点直接类型化结构数据操作状态 首次失败 =
                节点直接类型化结构数据操作状态::内部不一致;
            try {
                失效项组.reserve(请求.写项组.size());
                失效关系句柄组.reserve(请求.写项组.size());
                失效后见证组.reserve(请求.写项组.size());
                关系候选组.reserve(请求.写项组.size());
                for (const auto& 写项 : 请求.写项组)
                    失效项组.push_back(&std::get<节点直接关系失效项>(写项));
                std::sort(失效项组.begin(), 失效项组.end(), [](const auto* 左, const auto* 右) {
                    if (左->预期当前.稳定主键.命名域 != 右->预期当前.稳定主键.命名域)
                        return 左->预期当前.稳定主键.命名域 < 右->预期当前.稳定主键.命名域;
                    return 左->预期当前.稳定主键.键值 < 右->预期当前.稳定主键.键值;
                });
                for (std::size_t 序号 = 1; 序号 < 失效项组.size(); ++序号) {
                    if (失效项组[序号 - 1]->预期当前.稳定主键
                        == 失效项组[序号]->预期当前.稳定主键) {
                        候选完整 = false;
                        首次失败 = 节点直接类型化结构数据操作状态::入口拒绝;
                        break;
                    }
                }
            } catch (...) {
                候选完整 = false;
                首次失败 = 节点直接类型化结构数据操作状态::资源失败;
            }
            for (const auto* 项 : 失效项组) {
                if (!候选完整) break;
                const auto 当前 = 关系_->读取稳定主键当前关系(
                    项->预期当前.稳定主键, 许可.事务序号());
                if (当前.状态 != 稳定关系当前读取状态::当前有效 || !当前.记录
                    || 当前.记录->版本号 == std::numeric_limits<std::uint32_t>::max()) {
                    候选完整 = false;
                    首次失败 = 节点直接类型化结构数据操作状态::版本漂移;
                    break;
                }
                const auto 句柄 = 形成正式关系句柄(关系_->仓库编号(), *当前.记录);
                auto 失效后 = 项->预期当前;
                ++失效后.关系版本;
                失效关系句柄组.push_back(句柄);
                失效后见证组.push_back(失效后);
            }
            std::size_t 已进入事务参与者数量 = 0;
            if (候选完整) {
                const auto 复核状态 = 第一写前复核类型化结构参与者组_(
                    请求, 许可.事务序号(), 当前代次,
                    参与者组, 已进入事务参与者数量);
                if (复核状态 != 节点直接类型化结构事务参与者状态::第一写前已复核) {
                    候选完整 = false;
                    首次失败 = 映射类型化结构参与者失败_(复核状态);
                }
            }
            for (const auto& 句柄 : 失效关系句柄组) {
                if (!候选完整) break;
                auto 建立 = 关系_->结构化失效已发布关系(句柄, 许可.事务序号());
                if (建立.状态 != 正式关系操作状态::已变更候选 || !建立.候选) {
                    候选完整 = false;
                    首次失败 = 建立.状态 == 正式关系操作状态::入口拒绝_版本漂移
                        ? 节点直接类型化结构数据操作状态::版本漂移
                        : 建立.状态 == 正式关系操作状态::内部不一致
                            ? 节点直接类型化结构数据操作状态::内部不一致
                            : 节点直接类型化结构数据操作状态::入口拒绝;
                    break;
                }
                关系候选组.push_back(std::move(*建立.候选));
            }
            auto 撤销关系候选 = [&]() noexcept {
                bool 完整 = true;
                for (std::size_t 序号 = 关系候选组.size(); 序号 > 0; --序号)
                    完整 = 关系_->撤销候选(关系候选组[序号 - 1], 许可.事务序号())
                        == 正式关系操作状态::已撤销 && 完整;
                return 完整;
            };
            节点直接事务幂等记录 计划记录;
            std::optional<节点直接事务幂等候选> 幂等候选;
            类型化结构参与者准备材料_ 参与者准备材料;
            if (候选完整) {
                计划记录.安装实例身份 = 请求.安装实例身份;
                计划记录.幂等身份 = 请求.幂等身份;
                计划记录.请求意图摘要 = 请求.请求意图摘要;
                计划记录.执行证据摘要 = 请求.执行证据摘要;
                计划记录.材料格式版本 = 2;
                计划记录.写集规则版本 = 2;
                计划记录.写集材料 = 编码.材料;
                计划记录.状态 = 节点直接事务幂等记录状态::待发布;
                计划记录.发布代次 = 当前代次 + 1;
                bool 读回规格完整 = true;
                for (const auto& 读回 : 请求.读回规格.项目组) {
                    const auto 稳定主键 = std::get<关系稳定主键>(读回.身份);
                    const auto 位置 = std::find_if(失效后见证组.begin(), 失效后见证组.end(),
                        [&](const auto& 见证) { return 见证.稳定主键 == 稳定主键; });
                    if (位置 == 失效后见证组.end() || 读回.预期版本 != 位置->关系版本) {
                        读回规格完整 = false;
                        break;
                    }
                    计划记录.关系组.push_back(*位置);
                }
                计划记录.结果摘要 = 写集摘要.摘要;
                auto 幂等建立 = 读回规格完整
                    ? 幂等_->结构化建立记录未发布候选(计划记录, 许可.事务序号())
                    : 节点直接事务幂等候选结果{};
                if (幂等建立.状态 == 节点直接仓候选操作状态::已形成候选
                    && 幂等建立.候选) {
                    幂等候选.emplace(std::move(*幂等建立.候选));
                } else {
                    候选完整 = false;
                    首次失败 = 幂等建立.状态 == 节点直接仓候选操作状态::资源失败
                        ? 节点直接类型化结构数据操作状态::资源失败
                        : 节点直接类型化结构数据操作状态::内部不一致;
                }
            }
            if (候选完整) {
                const auto 准备状态 = 准备类型化结构参与者组_(
                    请求, *计划身份映射, 计划记录, 许可.事务序号(), 当前代次,
                    参与者组, 已进入事务参与者数量, 参与者准备材料);
                if (准备状态 != 节点直接类型化结构事务参与者状态::已准备) {
                    候选完整 = false;
                    首次失败 = 映射类型化结构参与者失败_(准备状态);
                }
            }
            if (候选完整) {
                for (auto& 候选 : 关系候选组) {
                    if (关系_->确认候选(候选, 许可.事务序号())
                        != 正式关系操作状态::已确认待发布) {
                        候选完整 = false;
                        首次失败 = 节点直接类型化结构数据操作状态::内部不一致;
                        break;
                    }
                }
            }
            if (候选完整
                && 幂等_->确认候选(*幂等候选, 许可.事务序号())
                    != 节点直接仓候选操作状态::已确认待发布) {
                候选完整 = false;
                首次失败 = 节点直接类型化结构数据操作状态::内部不一致;
            }
            if (候选完整) {
                const auto 确认状态 = 确认类型化结构参与者组_(参与者组);
                if (确认状态 != 节点直接类型化结构事务参与者状态::已确认待发布) {
                    候选完整 = false;
                    首次失败 = 映射类型化结构参与者失败_(确认状态);
                }
            }
            if (候选完整) {
                    for (auto& 候选 : 关系候选组) 关系_->完成发布(候选, 许可.事务序号());
                    bool 发布完整 = std::all_of(关系候选组.begin(), 关系候选组.end(),
                        [](const auto& 候选) { return 候选.读取阶段() == 正式关系候选阶段::已发布; });
                    发布完整 = 幂等_->完成发布(*幂等候选, 许可.事务序号())
                        == 节点直接仓候选操作状态::已发布 && 发布完整;
                    发布完整 = 发布完整 && 完成类型化结构参与者发布_(参与者组);
                    发布完整 = 发布完整 && 事务域_->推进普通已发布代次(许可, 当前代次);
                    for (const auto& 见证 : 失效后见证组) {
                        const auto 当前 = 关系_->读取稳定主键当前关系(
                            见证.稳定主键, 许可.事务序号());
                        发布完整 = 当前.状态 == 稳定关系当前读取状态::当前已失效
                            && 当前.记录 && 当前.记录->版本号 == 见证.关系版本
                            && 发布完整;
                    }
                    结果 = 形成幂等读回_(计划记录);
                    结果.状态 = 发布完整
                        ? 节点直接类型化结构数据操作状态::已提交
                        : 节点直接类型化结构数据操作状态::内部不一致;
                    if (!发布完整) 许可.标记隔离();
                    节点直接持久发布见证请求 发布请求;
                    发布请求.安装实例身份 = 请求.安装实例身份;
                    发布请求.事务身份 = 请求.幂等身份;
                    发布请求.尝试序号 = 尝试序号;
                    发布请求.请求意图摘要 = 请求.请求意图摘要;
                    发布请求.执行证据摘要 = 请求.执行证据摘要;
                    发布请求.发布代次 = 当前代次 + 1;
                    发布请求.结果摘要 = 计划记录.结果摘要;
                    const auto 发布见证 = 持久证据_->标记已发布(发布请求);
                    if ((发布见证.状态 == 节点直接持久端口状态::已见证
                            || 发布见证.状态 == 节点直接持久端口状态::精确同义)
                        && 发布见证.尝试序号 == 尝试序号
                        && 幂等_->单调记录持久证据状态(
                            请求.幂等身份, 尝试序号, 持久证据状态::已与内存代次一致)) {
                        结果.持久状态 = 持久证据状态::已与内存代次一致;
                    } else if (发布见证.状态 == 节点直接持久端口状态::结果未知
                        || 发布见证.状态 == 节点直接持久端口状态::资源失败) {
                        (void)幂等_->单调记录持久证据状态(
                            请求.幂等身份, 尝试序号, 持久证据状态::持久化结果未知);
                        结果.持久状态 = 持久证据状态::持久化结果未知;
                    } else {
                        (void)幂等_->单调记录持久证据状态(
                            请求.幂等身份, 尝试序号, 持久证据状态::持久证据损坏);
                        许可.标记隔离();
                        结果.状态 = 节点直接类型化结构数据操作状态::内部不一致;
                        结果.持久状态 = 持久证据状态::持久证据损坏;
                    }
                    return 结果;
            }
            const bool 参与者已完整撤销 = 逆序撤销类型化结构参与者组_(
                参与者组, 已进入事务参与者数量);
            const bool 幂等已完整撤销 = !幂等候选
                || 幂等_->撤销候选(*幂等候选, 许可.事务序号())
                    == 节点直接仓候选操作状态::已撤销;
            const bool 已完整撤销 = 参与者已完整撤销
                && 幂等已完整撤销 && 撤销关系候选();
            节点直接持久撤销见证请求 撤销请求;
            撤销请求.安装实例身份 = 请求.安装实例身份;
            撤销请求.事务身份 = 请求.幂等身份;
            撤销请求.尝试序号 = 尝试序号;
            撤销请求.请求意图摘要 = 请求.请求意图摘要;
            撤销请求.执行证据摘要 = 请求.执行证据摘要;
            撤销请求.写集材料摘要 = 写集摘要.摘要;
            const auto 撤销见证 = 已完整撤销
                ? 持久证据_->标记已撤销未发布(撤销请求)
                : 节点直接持久端口结果{};
            if (已完整撤销
                && (撤销见证.状态 == 节点直接持久端口状态::已撤销未发布
                    || 撤销见证.状态 == 节点直接持久端口状态::精确同义)
                && 撤销见证.尝试序号 == 尝试序号
                && 幂等_->清除临时持久证据侧账(请求.幂等身份, 尝试序号)) {
                结果.状态 = 首次失败;
                return 结果;
            }
            许可.标记隔离();
            结果.状态 = 节点直接类型化结构数据操作状态::内部不一致;
            return 结果;
        }

        const bool 仅含类型化值退役 = !请求.写项组.empty()
            && 请求.读回规格.项目组.empty()
            && std::all_of(请求.写项组.begin(), 请求.写项组.end(), [](const auto& 写项) {
                return std::holds_alternative<节点直接类型化值退役项>(写项);
            });
        if (仅含类型化值退役) {
            std::vector<const 节点直接类型化值退役项*> 退役项组;
            std::vector<类型化值读回> 退役前值组;
            std::vector<节点直接类型化值候选> 退役候选组;
            bool 候选完整 = true;
            节点直接类型化结构数据操作状态 首次失败 =
                节点直接类型化结构数据操作状态::内部不一致;
            try {
                退役项组.reserve(请求.写项组.size());
                退役前值组.reserve(请求.写项组.size());
                退役候选组.reserve(请求.写项组.size());
                for (const auto& 写项 : 请求.写项组)
                    退役项组.push_back(&std::get<节点直接类型化值退役项>(写项));
                std::sort(退役项组.begin(), 退役项组.end(), [](const auto* 左, const auto* 右) {
                    if (左->值记录身份.命名域 != 右->值记录身份.命名域)
                        return 左->值记录身份.命名域 < 右->值记录身份.命名域;
                    return 左->值记录身份.键值 < 右->值记录身份.键值;
                });
                for (std::size_t 序号 = 1; 序号 < 退役项组.size(); ++序号) {
                    if (退役项组[序号 - 1]->值记录身份 == 退役项组[序号]->值记录身份) {
                        候选完整 = false;
                        首次失败 = 节点直接类型化结构数据操作状态::入口拒绝;
                        break;
                    }
                }
            } catch (...) {
                候选完整 = false;
                首次失败 = 节点直接类型化结构数据操作状态::资源失败;
            }
            for (const auto* 项 : 退役项组) {
                if (!候选完整) break;
                const auto 当前组 = 类型化值_->读取所属身份当前值组(项->所属身份.稳定主键);
                const auto 位置 = std::find_if(当前组.begin(), 当前组.end(), [&](const auto& 当前) {
                    return 当前.类型合同身份 == 项->类型合同身份
                        && 当前.类型合同版本 == 项->类型合同版本
                        && 当前.值记录身份 == 项->值记录身份
                        && 当前.值记录版本 == 项->预期值记录版本;
                });
                if (位置 == 当前组.end()) {
                    候选完整 = false;
                    首次失败 = 节点直接类型化结构数据操作状态::版本漂移;
                    break;
                }
                try {
                    退役前值组.push_back(*位置);
                } catch (...) {
                    候选完整 = false;
                    首次失败 = 节点直接类型化结构数据操作状态::资源失败;
                }
            }
            std::size_t 已进入事务参与者数量 = 0;
            if (候选完整) {
                const auto 复核状态 = 第一写前复核类型化结构参与者组_(
                    请求, 许可.事务序号(), 当前代次,
                    参与者组, 已进入事务参与者数量);
                if (复核状态 != 节点直接类型化结构事务参与者状态::第一写前已复核) {
                    候选完整 = false;
                    首次失败 = 映射类型化结构参与者失败_(复核状态);
                }
            }
            for (const auto& 退役前值 : 退役前值组) {
                if (!候选完整) break;
                auto 建立 = 类型化值_->结构化退役值未发布候选(
                    退役前值, 当前代次 + 1, 许可.事务序号());
                if (建立.状态 != 节点直接仓候选操作状态::已形成候选 || !建立.候选) {
                    候选完整 = false;
                    首次失败 = 建立.状态 == 节点直接仓候选操作状态::资源失败
                        ? 节点直接类型化结构数据操作状态::资源失败
                        : 建立.状态 == 节点直接仓候选操作状态::版本漂移
                            ? 节点直接类型化结构数据操作状态::版本漂移
                            : 节点直接类型化结构数据操作状态::入口拒绝;
                    break;
                }
                退役候选组.push_back(std::move(*建立.候选));
            }
            auto 撤销退役候选 = [&]() noexcept {
                bool 完整 = true;
                for (std::size_t 序号 = 退役候选组.size(); 序号 > 0; --序号)
                    完整 = 类型化值_->撤销候选(退役候选组[序号 - 1], 许可.事务序号())
                        == 节点直接仓候选操作状态::已撤销 && 完整;
                return 完整;
            };
            节点直接事务幂等记录 计划记录;
            std::optional<节点直接事务幂等候选> 幂等候选;
            类型化结构参与者准备材料_ 参与者准备材料;
            if (候选完整) {
                计划记录.安装实例身份 = 请求.安装实例身份;
                计划记录.幂等身份 = 请求.幂等身份;
                计划记录.请求意图摘要 = 请求.请求意图摘要;
                计划记录.执行证据摘要 = 请求.执行证据摘要;
                计划记录.材料格式版本 = 2;
                计划记录.写集规则版本 = 2;
                计划记录.写集材料 = 编码.材料;
                计划记录.状态 = 节点直接事务幂等记录状态::待发布;
                计划记录.发布代次 = 当前代次 + 1;
                计划记录.结果摘要 = 写集摘要.摘要;
                auto 幂等建立 = 幂等_->结构化建立记录未发布候选(
                    计划记录, 许可.事务序号());
                if (幂等建立.状态 == 节点直接仓候选操作状态::已形成候选
                    && 幂等建立.候选) {
                    幂等候选.emplace(std::move(*幂等建立.候选));
                } else {
                    候选完整 = false;
                    首次失败 = 幂等建立.状态 == 节点直接仓候选操作状态::资源失败
                        ? 节点直接类型化结构数据操作状态::资源失败
                        : 节点直接类型化结构数据操作状态::内部不一致;
                }
            }
            if (候选完整) {
                const auto 准备状态 = 准备类型化结构参与者组_(
                    请求, *计划身份映射, 计划记录, 许可.事务序号(), 当前代次,
                    参与者组, 已进入事务参与者数量, 参与者准备材料);
                if (准备状态 != 节点直接类型化结构事务参与者状态::已准备) {
                    候选完整 = false;
                    首次失败 = 映射类型化结构参与者失败_(准备状态);
                }
            }
            if (候选完整) {
                for (auto& 候选 : 退役候选组) {
                    if (类型化值_->确认候选(候选, 许可.事务序号())
                        != 节点直接仓候选操作状态::已确认待发布) {
                        候选完整 = false;
                        首次失败 = 节点直接类型化结构数据操作状态::内部不一致;
                        break;
                    }
                }
            }
            if (候选完整
                && 幂等_->确认候选(*幂等候选, 许可.事务序号())
                    != 节点直接仓候选操作状态::已确认待发布) {
                候选完整 = false;
                首次失败 = 节点直接类型化结构数据操作状态::内部不一致;
            }
            if (候选完整) {
                const auto 确认状态 = 确认类型化结构参与者组_(参与者组);
                if (确认状态 != 节点直接类型化结构事务参与者状态::已确认待发布) {
                    候选完整 = false;
                    首次失败 = 映射类型化结构参与者失败_(确认状态);
                }
            }
            if (候选完整) {
                    bool 发布完整 = true;
                    for (auto& 候选 : 退役候选组)
                        发布完整 = 类型化值_->完成发布(候选, 许可.事务序号())
                            == 节点直接仓候选操作状态::已发布 && 发布完整;
                    发布完整 = 幂等_->完成发布(*幂等候选, 许可.事务序号())
                        == 节点直接仓候选操作状态::已发布 && 发布完整;
                    发布完整 = 发布完整 && 完成类型化结构参与者发布_(参与者组);
                    发布完整 = 发布完整 && 事务域_->推进普通已发布代次(许可, 当前代次);
                    bool 读回一致 = 发布完整;
                    for (const auto& 退役前值 : 退役前值组) {
                        const auto 当前组 = 类型化值_->读取所属身份当前值组(
                            退役前值.所属身份.稳定主键);
                        读回一致 = std::none_of(当前组.begin(), 当前组.end(), [&](const auto& 当前) {
                            return 当前.值记录身份 == 退役前值.值记录身份;
                        }) && 读回一致;
                    }
                    结果 = 形成幂等读回_(计划记录);
                    结果.状态 = 读回一致
                        ? 节点直接类型化结构数据操作状态::已提交
                        : 节点直接类型化结构数据操作状态::内部不一致;
                    if (!读回一致) 许可.标记隔离();
                    节点直接持久发布见证请求 发布请求;
                    发布请求.安装实例身份 = 请求.安装实例身份;
                    发布请求.事务身份 = 请求.幂等身份;
                    发布请求.尝试序号 = 尝试序号;
                    发布请求.请求意图摘要 = 请求.请求意图摘要;
                    发布请求.执行证据摘要 = 请求.执行证据摘要;
                    发布请求.发布代次 = 当前代次 + 1;
                    发布请求.结果摘要 = 计划记录.结果摘要;
                    const auto 发布见证 = 持久证据_->标记已发布(发布请求);
                    if ((发布见证.状态 == 节点直接持久端口状态::已见证
                            || 发布见证.状态 == 节点直接持久端口状态::精确同义)
                        && 发布见证.尝试序号 == 尝试序号
                        && 幂等_->单调记录持久证据状态(
                            请求.幂等身份, 尝试序号, 持久证据状态::已与内存代次一致)) {
                        结果.持久状态 = 持久证据状态::已与内存代次一致;
                    } else if (发布见证.状态 == 节点直接持久端口状态::结果未知
                        || 发布见证.状态 == 节点直接持久端口状态::资源失败) {
                        (void)幂等_->单调记录持久证据状态(
                            请求.幂等身份, 尝试序号, 持久证据状态::持久化结果未知);
                        结果.持久状态 = 持久证据状态::持久化结果未知;
                    } else {
                        (void)幂等_->单调记录持久证据状态(
                            请求.幂等身份, 尝试序号, 持久证据状态::持久证据损坏);
                        许可.标记隔离();
                        结果.状态 = 节点直接类型化结构数据操作状态::内部不一致;
                        结果.持久状态 = 持久证据状态::持久证据损坏;
                    }
                    return 结果;
            }
            const bool 参与者已完整撤销 = 逆序撤销类型化结构参与者组_(
                参与者组, 已进入事务参与者数量);
            const bool 幂等已完整撤销 = !幂等候选
                || 幂等_->撤销候选(*幂等候选, 许可.事务序号())
                    == 节点直接仓候选操作状态::已撤销;
            const bool 已完整撤销 = 参与者已完整撤销
                && 幂等已完整撤销 && 撤销退役候选();
            节点直接持久撤销见证请求 撤销请求;
            撤销请求.安装实例身份 = 请求.安装实例身份;
            撤销请求.事务身份 = 请求.幂等身份;
            撤销请求.尝试序号 = 尝试序号;
            撤销请求.请求意图摘要 = 请求.请求意图摘要;
            撤销请求.执行证据摘要 = 请求.执行证据摘要;
            撤销请求.写集材料摘要 = 写集摘要.摘要;
            const auto 撤销见证 = 已完整撤销
                ? 持久证据_->标记已撤销未发布(撤销请求)
                : 节点直接持久端口结果{};
            if (已完整撤销
                && (撤销见证.状态 == 节点直接持久端口状态::已撤销未发布
                    || 撤销见证.状态 == 节点直接持久端口状态::精确同义)
                && 撤销见证.尝试序号 == 尝试序号
                && 幂等_->清除临时持久证据侧账(请求.幂等身份, 尝试序号)) {
                结果.状态 = 首次失败;
                return 结果;
            }
            许可.标记隔离();
            结果.状态 = 节点直接类型化结构数据操作状态::内部不一致;
            return 结果;
        }

        const bool 仅含类型化值发布 = !请求.写项组.empty()
            && std::all_of(请求.写项组.begin(), 请求.写项组.end(), [](const auto& 写项) {
                return std::holds_alternative<节点直接类型化值发布项>(写项);
            });
        const bool 读回仅含本次类型化值 = std::all_of(
            请求.读回规格.项目组.begin(), 请求.读回规格.项目组.end(), [](const auto& 项) {
                if (项.种类 != 节点直接发布后读回对象种类::当前类型化值
                    || !std::holds_alternative<节点直接写集局部身份>(项.身份)) return false;
                return std::get<节点直接写集局部身份>(项.身份).种类
                    == 节点直接写集局部身份种类::类型化值记录;
            });
        if (仅含类型化值发布 && 读回仅含本次类型化值) {
            struct 值结果项 {
                节点直接写集局部身份 局部身份;
                类型化值读回 见证;
            };
            std::vector<const 节点直接类型化值发布项*> 值项组;
            std::vector<值结果项> 值结果组;
            std::vector<节点直接类型化值候选> 值候选组;
            bool 候选完整 = true;
            节点直接类型化结构数据操作状态 首次失败 =
                节点直接类型化结构数据操作状态::内部不一致;
            try {
                值项组.reserve(请求.写项组.size());
                值结果组.reserve(请求.写项组.size());
                值候选组.reserve(请求.写项组.size());
                for (const auto& 写项 : 请求.写项组)
                    值项组.push_back(&std::get<节点直接类型化值发布项>(写项));
                std::sort(值项组.begin(), 值项组.end(), [](const auto* 左, const auto* 右) {
                    return 左->值记录局部身份.值 < 右->值记录局部身份.值;
                });
            } catch (...) {
                候选完整 = false;
                首次失败 = 节点直接类型化结构数据操作状态::资源失败;
            }

            auto 解析已发布节点 = [&](const 节点直接节点端点引用& 引用)
                -> std::optional<节点稳定身份见证> {
                const auto* 见证 = std::get_if<节点稳定身份见证>(&引用);
                if (见证 == nullptr) return std::nullopt;
                const auto 当前 = 节点_->读取稳定主键当前身份(
                    见证->稳定主键, 许可.事务序号());
                if (当前.状态 != 稳定主键当前身份状态::当前有效
                    || !当前.当前记录 || 当前.当前记录->类型 != 见证->类型
                    || 当前.当前记录->版本号 != 见证->身份版本) return std::nullopt;
                return *见证;
            };
            std::vector<std::tuple<节点稳定主键, 类型合同稳定身份, std::uint32_t>> 已用合同键;
            if (候选完整) {
                try { 已用合同键.reserve(值项组.size()); }
                catch (...) {
                    候选完整 = false;
                    首次失败 = 节点直接类型化结构数据操作状态::资源失败;
                }
            }
            for (const auto* 项 : 值项组) {
                if (!候选完整) break;
                const auto* 映射 = 查找计划身份_(*计划身份映射, 项->值记录局部身份);
                const auto 所属身份 = 解析已发布节点(项->所属身份);
                const auto 合同 = 类型合同_->读取精确合同(项->类型合同身份, 项->类型合同版本);
                if (映射 == nullptr || !所属身份 || !合同
                    || 合同->生命周期 != 类型合同生命周期状态::当前可写
                    || !类型化值材料符合合同_(项->材料, *合同)) {
                    候选完整 = false;
                    首次失败 = 节点直接类型化结构数据操作状态::入口拒绝;
                    break;
                }
                const auto 合同键 = std::tuple{
                    所属身份->稳定主键, 项->类型合同身份, 项->类型合同版本};
                if (std::find(已用合同键.begin(), 已用合同键.end(), 合同键)
                    != 已用合同键.end()) {
                    候选完整 = false;
                    首次失败 = 节点直接类型化结构数据操作状态::入口拒绝;
                    break;
                }
                已用合同键.push_back(合同键);
                类型化值来源见证 来源;
                if (const auto* 节点来源 = std::get_if<节点直接节点端点引用>(&项->来源)) {
                    const auto 来源见证 = 解析已发布节点(*节点来源);
                    if (!来源见证) {
                        候选完整 = false;
                        首次失败 = 节点直接类型化结构数据操作状态::入口拒绝;
                        break;
                    }
                    来源 = *来源见证;
                } else {
                    来源 = std::get<服务稳定身份>(项->来源);
                }
                const auto 当前组 = 类型化值_->读取所属身份当前值组(所属身份->稳定主键);
                const auto 当前位置 = std::find_if(当前组.begin(), 当前组.end(), [&](const auto& 当前) {
                    return 当前.类型合同身份 == 项->类型合同身份
                        && 当前.类型合同版本 == 项->类型合同版本;
                });
                if ((项->预期当前值记录版本.has_value()
                        && (当前位置 == 当前组.end()
                            || 当前位置->值记录版本 != *项->预期当前值记录版本))
                    || (!项->预期当前值记录版本.has_value() && 当前位置 != 当前组.end())) {
                    候选完整 = false;
                    首次失败 = 节点直接类型化结构数据操作状态::版本漂移;
                    break;
                }
                类型化值读回 计划值{
                    *所属身份,
                    项->类型合同身份,
                    项->类型合同版本,
                    {映射->稳定身份命名域, 映射->计划键},
                    映射->初始版本,
                    true,
                    项->材料,
                    来源,
                    项->预期当前值记录版本,
                    当前代次 + 1,
                    当前代次 + 1};
                try {
                    值结果组.push_back({项->值记录局部身份, std::move(计划值)});
                } catch (...) {
                    候选完整 = false;
                    首次失败 = 节点直接类型化结构数据操作状态::资源失败;
                }
            }

            std::size_t 已进入事务参与者数量 = 0;
            if (候选完整) {
                const auto 复核状态 = 第一写前复核类型化结构参与者组_(
                    请求, 许可.事务序号(), 当前代次,
                    参与者组, 已进入事务参与者数量);
                if (复核状态 != 节点直接类型化结构事务参与者状态::第一写前已复核) {
                    候选完整 = false;
                    首次失败 = 映射类型化结构参与者失败_(复核状态);
                }
            }
            for (const auto& 项 : 值结果组) {
                if (!候选完整) break;
                auto 建立 = 类型化值_->结构化发布值未发布候选(
                    项.见证, 许可.事务序号());
                if (建立.状态 != 节点直接仓候选操作状态::已形成候选 || !建立.候选) {
                    候选完整 = false;
                    首次失败 = 建立.状态 == 节点直接仓候选操作状态::资源失败
                        ? 节点直接类型化结构数据操作状态::资源失败
                        : 建立.状态 == 节点直接仓候选操作状态::版本漂移
                            ? 节点直接类型化结构数据操作状态::版本漂移
                            : 节点直接类型化结构数据操作状态::入口拒绝;
                    break;
                }
                值候选组.push_back(std::move(*建立.候选));
            }

            auto 撤销值候选 = [&]() noexcept {
                bool 完整 = true;
                for (std::size_t 序号 = 值候选组.size(); 序号 > 0; --序号)
                    完整 = 类型化值_->撤销候选(值候选组[序号 - 1], 许可.事务序号())
                        == 节点直接仓候选操作状态::已撤销 && 完整;
                return 完整;
            };
            节点直接事务幂等记录 计划记录;
            std::optional<节点直接事务幂等候选> 幂等候选;
            类型化结构参与者准备材料_ 参与者准备材料;
            if (候选完整) {
                计划记录.安装实例身份 = 请求.安装实例身份;
                计划记录.幂等身份 = 请求.幂等身份;
                计划记录.请求意图摘要 = 请求.请求意图摘要;
                计划记录.执行证据摘要 = 请求.执行证据摘要;
                计划记录.材料格式版本 = 2;
                计划记录.写集规则版本 = 2;
                计划记录.写集材料 = 编码.材料;
                计划记录.状态 = 节点直接事务幂等记录状态::待发布;
                计划记录.发布代次 = 当前代次 + 1;
                std::vector<std::uint8_t> 结果材料 = 编码.材料;
                bool 结果完整 = true;
                for (const auto& 读回 : 请求.读回规格.项目组) {
                    const auto 局部 = std::get<节点直接写集局部身份>(读回.身份);
                    const auto 位置 = std::find_if(值结果组.begin(), 值结果组.end(),
                        [&](const auto& 项) { return 项.局部身份 == 局部; });
                    if (位置 == 值结果组.end() || 读回.预期版本 != 位置->见证.值记录版本) {
                        结果完整 = false;
                        break;
                    }
                    计划记录.类型化值组.push_back(位置->见证);
                    结果完整 = 追加无符号_(结果材料, 位置->见证.值记录身份.命名域, 8)
                        && 追加无符号_(结果材料, 位置->见证.值记录身份.键值, 8)
                        && 追加无符号_(结果材料, 位置->见证.值记录版本, 8)
                        && 结果完整;
                }
                const auto 结果摘要 = 结果完整 ? 计算SHA256_(结果材料) : 节点直接摘要计算结果{};
                if (结果摘要.状态 == 节点直接材料转换状态::成功)
                    计划记录.结果摘要 = 结果摘要.摘要;
                auto 幂等建立 = 结果摘要.状态 == 节点直接材料转换状态::成功
                    ? 幂等_->结构化建立记录未发布候选(计划记录, 许可.事务序号())
                    : 节点直接事务幂等候选结果{};
                if (幂等建立.状态 == 节点直接仓候选操作状态::已形成候选
                    && 幂等建立.候选) {
                    幂等候选.emplace(std::move(*幂等建立.候选));
                    try {
                        参与者准备材料.候选读回.类型化值组 = 计划记录.类型化值组;
                        参与者准备材料.计划身份见证组.reserve(值结果组.size());
                        for (const auto& 项 : 值结果组)
                            参与者准备材料.计划身份见证组.push_back({项.局部身份, 项.见证});
                    } catch (...) {
                        候选完整 = false;
                        首次失败 = 节点直接类型化结构数据操作状态::资源失败;
                    }
                } else {
                    候选完整 = false;
                    首次失败 = 幂等建立.状态 == 节点直接仓候选操作状态::资源失败
                        ? 节点直接类型化结构数据操作状态::资源失败
                        : 节点直接类型化结构数据操作状态::内部不一致;
                }
            }
            if (候选完整) {
                const auto 准备状态 = 准备类型化结构参与者组_(
                    请求, 许可.事务序号(), 当前代次, 参与者组,
                    已进入事务参与者数量, 参与者准备材料);
                if (准备状态 != 节点直接类型化结构事务参与者状态::已准备) {
                    候选完整 = false;
                    首次失败 = 映射类型化结构参与者失败_(准备状态);
                }
            }
            if (候选完整) {
                for (auto& 候选 : 值候选组) {
                    if (类型化值_->确认候选(候选, 许可.事务序号())
                        != 节点直接仓候选操作状态::已确认待发布) {
                        候选完整 = false;
                        首次失败 = 节点直接类型化结构数据操作状态::内部不一致;
                        break;
                    }
                }
            }
            if (候选完整
                && 幂等_->确认候选(*幂等候选, 许可.事务序号())
                    != 节点直接仓候选操作状态::已确认待发布) {
                候选完整 = false;
                首次失败 = 节点直接类型化结构数据操作状态::内部不一致;
            }
            if (候选完整) {
                const auto 确认状态 = 确认类型化结构参与者组_(参与者组);
                if (确认状态 != 节点直接类型化结构事务参与者状态::已确认待发布) {
                    候选完整 = false;
                    首次失败 = 映射类型化结构参与者失败_(确认状态);
                }
            }
            if (候选完整) {
                    bool 发布完整 = true;
                    for (auto& 候选 : 值候选组)
                        发布完整 = 类型化值_->完成发布(候选, 许可.事务序号())
                            == 节点直接仓候选操作状态::已发布 && 发布完整;
                    发布完整 = 幂等_->完成发布(*幂等候选, 许可.事务序号())
                        == 节点直接仓候选操作状态::已发布 && 发布完整;
                    发布完整 = 发布完整 && 完成类型化结构参与者发布_(参与者组);
                    发布完整 = 发布完整 && 事务域_->推进普通已发布代次(许可, 当前代次);
                    bool 读回一致 = 发布完整;
                    for (const auto& 项 : 值结果组) {
                        const auto 当前组 = 类型化值_->读取所属身份当前值组(项.见证.所属身份.稳定主键);
                        读回一致 = std::find(当前组.begin(), 当前组.end(), 项.见证)
                            != 当前组.end() && 读回一致;
                    }
                    结果 = 形成幂等读回_(计划记录);
                    结果.状态 = 读回一致
                        ? 节点直接类型化结构数据操作状态::已提交
                        : 节点直接类型化结构数据操作状态::内部不一致;
                    if (!读回一致) 许可.标记隔离();
                    节点直接持久发布见证请求 发布请求;
                    发布请求.安装实例身份 = 请求.安装实例身份;
                    发布请求.事务身份 = 请求.幂等身份;
                    发布请求.尝试序号 = 尝试序号;
                    发布请求.请求意图摘要 = 请求.请求意图摘要;
                    发布请求.执行证据摘要 = 请求.执行证据摘要;
                    发布请求.发布代次 = 当前代次 + 1;
                    发布请求.结果摘要 = 计划记录.结果摘要;
                    const auto 发布见证 = 持久证据_->标记已发布(发布请求);
                    if ((发布见证.状态 == 节点直接持久端口状态::已见证
                            || 发布见证.状态 == 节点直接持久端口状态::精确同义)
                        && 发布见证.尝试序号 == 尝试序号
                        && 幂等_->单调记录持久证据状态(
                            请求.幂等身份, 尝试序号, 持久证据状态::已与内存代次一致)) {
                        结果.持久状态 = 持久证据状态::已与内存代次一致;
                    } else if (发布见证.状态 == 节点直接持久端口状态::结果未知
                        || 发布见证.状态 == 节点直接持久端口状态::资源失败) {
                        (void)幂等_->单调记录持久证据状态(
                            请求.幂等身份, 尝试序号, 持久证据状态::持久化结果未知);
                        结果.持久状态 = 持久证据状态::持久化结果未知;
                    } else {
                        (void)幂等_->单调记录持久证据状态(
                            请求.幂等身份, 尝试序号, 持久证据状态::持久证据损坏);
                        许可.标记隔离();
                        结果.状态 = 节点直接类型化结构数据操作状态::内部不一致;
                        结果.持久状态 = 持久证据状态::持久证据损坏;
                    }
                    return 结果;
            }
            const bool 参与者已完整撤销 = 逆序撤销类型化结构参与者组_(
                参与者组, 已进入事务参与者数量);
            const bool 幂等已完整撤销 = !幂等候选
                || 幂等_->撤销候选(*幂等候选, 许可.事务序号())
                    == 节点直接仓候选操作状态::已撤销;
            const bool 已完整撤销 = 参与者已完整撤销
                && 幂等已完整撤销 && 撤销值候选();
            节点直接持久撤销见证请求 撤销请求;
            撤销请求.安装实例身份 = 请求.安装实例身份;
            撤销请求.事务身份 = 请求.幂等身份;
            撤销请求.尝试序号 = 尝试序号;
            撤销请求.请求意图摘要 = 请求.请求意图摘要;
            撤销请求.执行证据摘要 = 请求.执行证据摘要;
            撤销请求.写集材料摘要 = 写集摘要.摘要;
            const auto 撤销见证 = 已完整撤销
                ? 持久证据_->标记已撤销未发布(撤销请求)
                : 节点直接持久端口结果{};
            if (已完整撤销
                && (撤销见证.状态 == 节点直接持久端口状态::已撤销未发布
                    || 撤销见证.状态 == 节点直接持久端口状态::精确同义)
                && 撤销见证.尝试序号 == 尝试序号
                && 幂等_->清除临时持久证据侧账(请求.幂等身份, 尝试序号)) {
                结果.状态 = 首次失败;
                return 结果;
            }
            许可.标记隔离();
            结果.状态 = 节点直接类型化结构数据操作状态::内部不一致;
            return 结果;
        }

        const bool 仅含索引和节点删除 = !请求.写项组.empty()
            && std::all_of(请求.写项组.begin(), 请求.写项组.end(), [](const auto& 写项) {
                return std::holds_alternative<节点直接索引创建项>(写项)
                    || std::holds_alternative<节点直接索引移除项>(写项)
                    || std::holds_alternative<节点直接节点删除项>(写项);
            });
        const bool 读回仅含索引 = std::all_of(
            请求.读回规格.项目组.begin(), 请求.读回规格.项目组.end(), [](const auto& 项) {
                return (项.种类 == 节点直接发布后读回对象种类::当前索引
                        || 项.种类 == 节点直接发布后读回对象种类::已移除索引)
                    && std::holds_alternative<索引物理键>(项.身份) && 项.预期版本 == 0;
            });
        if (仅含索引和节点删除 && 读回仅含索引) {
            struct 索引操作计划 {
                索引物理键 键;
                节点稳定身份见证 节点目标;
                节点句柄 节点目标句柄;
                bool 是移除 = false;
            };
            struct 节点删除计划 {
                节点句柄 节点目标句柄;
                节点稳定身份见证 删除后见证;
            };
            std::vector<const 节点直接类型化结构写项*> 索引项组;
            std::vector<const 节点直接节点删除项*> 删除项组;
            for (const auto& 写项 : 请求.写项组) {
                if (const auto* 删除项 = std::get_if<节点直接节点删除项>(&写项)) 删除项组.push_back(删除项);
                else 索引项组.push_back(&写项);
            }
            std::sort(索引项组.begin(), 索引项组.end(), [](const auto* 左项, const auto* 右项) {
                const auto 取键 = [](const auto& 写项) -> const 索引物理键& {
                    if (const auto* 创建 = std::get_if<节点直接索引创建项>(&写项)) return 创建->键;
                    return std::get<节点直接索引移除项>(写项).键;
                };
                const auto& 左 = 取键(*左项);
                const auto& 右 = 取键(*右项);
                return std::tie(左.所有者身份, 左.命名域, 左.键格式版本, 左.探测规则版本, 左.键值)
                    < std::tie(右.所有者身份, 右.命名域, 右.键格式版本, 右.探测规则版本, 右.键值);
            });
            std::sort(删除项组.begin(), 删除项组.end(), [](const auto* 左, const auto* 右) {
                return std::tie(左->预期当前.稳定主键.命名域, 左->预期当前.稳定主键.键值)
                    < std::tie(右->预期当前.稳定主键.命名域, 右->预期当前.稳定主键.键值);
            });
            std::vector<可重建索引候选> 候选组;
            std::vector<节点直接身份删除未发布候选> 删除候选组;
            std::vector<索引操作计划> 计划组;
            std::vector<节点删除计划> 删除计划组;
            bool 候选完整 = true;
            节点直接类型化结构数据操作状态 首次失败 = 节点直接类型化结构数据操作状态::内部不一致;
            try {
                候选组.reserve(索引项组.size());
                删除候选组.reserve(删除项组.size());
                计划组.reserve(索引项组.size());
                删除计划组.reserve(删除项组.size());
            } catch (...) {
                候选完整 = false;
                首次失败 = 节点直接类型化结构数据操作状态::资源失败;
            }
            for (const auto* 写项 : 索引项组) {
                if (!候选完整) break;
                if (const auto* 创建项 = std::get_if<节点直接索引创建项>(写项)) {
                    const auto& 见证 = std::get<节点稳定身份见证>(创建项->目标);
                    const auto 当前 = 节点_->读取稳定主键当前身份(见证.稳定主键, 许可.事务序号());
                    if (当前.状态 != 稳定主键当前身份状态::当前有效 || !当前.当前身份) {
                        候选完整 = false;
                        首次失败 = 节点直接类型化结构数据操作状态::入口拒绝;
                        break;
                    }
                    const auto 已占用 = 索引_->读取索引物理键(
                        创建项->键, 许可.事务序号());
                    if (已占用) {
                        const bool 精确同义 = 已占用->目标种类 == 索引目标种类::节点
                            && 已占用->节点 == *当前.当前身份;
                        (void)精确同义;
                        候选完整 = false;
                        首次失败 = 节点直接类型化结构数据操作状态::入口拒绝;
                        break;
                    }
                    计划组.push_back({创建项->键, 见证, *当前.当前身份, false});
                } else {
                    const auto& 移除项 = std::get<节点直接索引移除项>(*写项);
                    const auto 当前 = 索引_->读取索引物理键(
                        移除项.键, 许可.事务序号());
                    if (!当前 || 当前->目标种类 != 索引目标种类::节点) {
                        候选完整 = false;
                        首次失败 = 节点直接类型化结构数据操作状态::入口拒绝;
                        break;
                    }
                    计划组.push_back({移除项.键, 移除项.预期目标, 当前->节点, true});
                }
            }
            for (const auto* 删除项 : 删除项组) {
                if (!候选完整) break;
                const auto 当前 = 节点_->读取稳定主键当前身份(
                    删除项->预期当前.稳定主键, 许可.事务序号());
                if (当前.状态 != 稳定主键当前身份状态::当前有效 || !当前.当前身份
                    || !当前.当前记录 || 当前.当前记录->类型 != 删除项->预期当前.类型
                    || 当前.当前记录->版本号 != 删除项->预期当前.身份版本) {
                    候选完整 = false;
                    首次失败 = 节点直接类型化结构数据操作状态::入口拒绝;
                    break;
                }
                if (删除项->预期当前.身份版本 == std::numeric_limits<std::uint32_t>::max()) {
                    候选完整 = false;
                    首次失败 = 节点直接类型化结构数据操作状态::版本漂移;
                    break;
                }
                删除计划组.push_back({*当前.当前身份,
                    {删除项->预期当前.稳定主键,
                        删除项->预期当前.类型, 删除项->预期当前.身份版本 + 1}});
            }
            std::size_t 已进入事务参与者数量 = 0;
            if (候选完整) {
                const auto 复核状态 = 第一写前复核类型化结构参与者组_(
                    请求, 许可.事务序号(), 当前代次,
                    参与者组, 已进入事务参与者数量);
                if (复核状态 != 节点直接类型化结构事务参与者状态::第一写前已复核) {
                    候选完整 = false;
                    首次失败 = 映射类型化结构参与者失败_(复核状态);
                }
            }
            for (const auto& 项 : 计划组) {
                if (!候选完整) break;
                auto 建立 = 项.是移除
                    ? 索引_->结构化移除索引候选(
                        项.键, 项.节点目标, 许可.事务序号())
                    : 索引_->结构化创建索引候选(
                        {项.键, 索引目标种类::节点, 项.节点目标句柄, {}},
                        许可.事务序号());
                if (建立.状态 != 可重建索引操作状态::已创建候选 || !建立.候选) {
                    候选完整 = false;
                    首次失败 = 建立.状态 == 可重建索引操作状态::内部不一致
                        ? 节点直接类型化结构数据操作状态::内部不一致
                        : 节点直接类型化结构数据操作状态::入口拒绝;
                    break;
                }
                候选组.push_back(std::move(*建立.候选));
            }
            for (const auto& 项 : 删除计划组) {
                if (!候选完整) break;
                auto 删除 = 节点_->结构化删除节点未发布候选(
                    项.节点目标句柄, 许可.事务序号());
                if (删除.状态 != 节点直接身份写入状态::已形成候选 || !删除.候选) {
                    候选完整 = false;
                    首次失败 = 删除.状态 == 节点直接身份写入状态::资源失败
                        ? 节点直接类型化结构数据操作状态::资源失败
                        : 删除.状态 == 节点直接身份写入状态::版本漂移
                            ? 节点直接类型化结构数据操作状态::版本漂移
                            : 节点直接类型化结构数据操作状态::内部不一致;
                    break;
                }
                删除候选组.push_back(std::move(*删除.候选));
            }
            节点直接事务幂等记录 计划记录;
            bool 结果可编码 = 候选完整;
            if (候选完整) {
                计划记录.安装实例身份 = 请求.安装实例身份;
                计划记录.幂等身份 = 请求.幂等身份;
                计划记录.请求意图摘要 = 请求.请求意图摘要;
                计划记录.执行证据摘要 = 请求.执行证据摘要;
                计划记录.材料格式版本 = 2;
                计划记录.写集规则版本 = 2;
                计划记录.写集材料 = 编码.材料;
                计划记录.状态 = 节点直接事务幂等记录状态::待发布;
                计划记录.发布代次 = 当前代次 + 1;
                for (const auto& 读回项 : 请求.读回规格.项目组) {
                    const auto& 键 = std::get<索引物理键>(读回项.身份);
                    const auto 位置 = std::find_if(计划组.begin(), 计划组.end(),
                        [&](const auto& 项) { return 项.键 == 键; });
                    const bool 要求移除 = 读回项.种类 == 节点直接发布后读回对象种类::已移除索引;
                    if (位置 == 计划组.end() || 位置->是移除 != 要求移除) {
                        结果可编码 = false;
                        break;
                    }
                    计划记录.索引组.push_back({键, !要求移除, 位置->节点目标, std::nullopt});
                }
                std::vector<std::uint8_t> 结果材料 = 编码.材料;
                结果可编码 = 结果可编码 && 追加无符号_(结果材料, 计划记录.索引组.size(), 8);
                for (const auto& 项 : 计划记录.索引组) {
                    结果可编码 = 结果可编码 && 追加索引键_(结果材料, 项.键)
                        && 追加字节_(结果材料, 项.当前 ? 1 : 0)
                        && 项.节点目标.has_value() && 追加节点见证_(结果材料, *项.节点目标)
                        && 追加字节_(结果材料, 0);
                }
                const auto 摘要 = 结果可编码 ? 计算SHA256_(结果材料) : 节点直接摘要计算结果{};
                结果可编码 = 结果可编码 && 摘要.状态 == 节点直接材料转换状态::成功;
                if (结果可编码) 计划记录.结果摘要 = 摘要.摘要;
            }

            std::optional<节点直接事务幂等候选> 幂等候选;
            类型化结构参与者准备材料_ 参与者准备材料;
            if (结果可编码) {
                auto 建立 = 幂等_->结构化建立记录未发布候选(计划记录, 许可.事务序号());
                if (建立.状态 == 节点直接仓候选操作状态::已形成候选 && 建立.候选) {
                    幂等候选.emplace(std::move(*建立.候选));
                    try {
                        参与者准备材料.候选读回.索引组 = 计划记录.索引组;
                    } catch (...) {
                        候选完整 = false;
                        首次失败 = 节点直接类型化结构数据操作状态::资源失败;
                    }
                } else {
                    候选完整 = false;
                    首次失败 = 建立.状态 == 节点直接仓候选操作状态::资源失败
                        ? 节点直接类型化结构数据操作状态::资源失败
                        : 节点直接类型化结构数据操作状态::内部不一致;
                }
            } else {
                候选完整 = false;
            }

            if (候选完整) {
                const auto 准备状态 = 准备类型化结构参与者组_(
                    请求, 许可.事务序号(), 当前代次, 参与者组,
                    已进入事务参与者数量, 参与者准备材料);
                if (准备状态 != 节点直接类型化结构事务参与者状态::已准备) {
                    候选完整 = false;
                    首次失败 = 映射类型化结构参与者失败_(准备状态);
                }
            }
            if (候选完整) {
                for (auto& 候选 : 候选组) {
                    if (索引_->确认候选(候选, 许可.事务序号())
                        != 可重建索引操作状态::已确认待发布) {
                        候选完整 = false;
                        首次失败 = 节点直接类型化结构数据操作状态::内部不一致;
                        break;
                    }
                }
            }
            if (候选完整) {
                for (auto& 候选 : 删除候选组) {
                    if (节点_->确认节点删除候选(候选, 许可.事务序号()).状态
                        != 节点直接身份写入状态::已确认待发布) {
                        候选完整 = false;
                        首次失败 = 节点直接类型化结构数据操作状态::内部不一致;
                        break;
                    }
                }
            }
            if (候选完整
                && 幂等_->确认候选(*幂等候选, 许可.事务序号())
                    != 节点直接仓候选操作状态::已确认待发布) {
                候选完整 = false;
                首次失败 = 节点直接类型化结构数据操作状态::内部不一致;
            }
            if (候选完整) {
                const auto 确认状态 = 确认类型化结构参与者组_(参与者组);
                if (确认状态 != 节点直接类型化结构事务参与者状态::已确认待发布) {
                    候选完整 = false;
                    首次失败 = 映射类型化结构参与者失败_(确认状态);
                }
            }
            if (候选完整 && 幂等候选) {
                bool 发布完整 = true;
                for (auto& 候选 : 候选组)
                    索引_->完成发布(候选, 许可.事务序号());
                for (auto& 候选 : 删除候选组)
                    节点_->完成发布节点删除候选(候选, 许可.事务序号());
                发布完整 = 幂等_->完成发布(*幂等候选, 许可.事务序号())
                    == 节点直接仓候选操作状态::已发布 && 发布完整;
                发布完整 = 发布完整 && 完成类型化结构参与者发布_(参与者组);
                发布完整 = 发布完整 && 事务域_->推进普通已发布代次(许可, 当前代次);
                if (!发布完整) {
                    许可.标记隔离();
                    结果.状态 = 节点直接类型化结构数据操作状态::内部不一致;
                    return 结果;
                }
                bool 发布后读回一致 = true;
                for (const auto& 项 : 计划组) {
                    const auto 当前 = 索引_->读取索引物理键(项.键, 许可.事务序号());
                    if (项.是移除) {
                        发布后读回一致 = !当前 && 发布后读回一致;
                    } else if (!当前 || 当前->目标种类 != 索引目标种类::节点) {
                        发布后读回一致 = false;
                    } else {
                        const auto 节点记录 = 节点_->读取节点(当前->节点, 许可.事务序号());
                        发布后读回一致 = 节点记录
                            && 节点记录->稳定主键 == 项.节点目标.稳定主键
                            && 节点记录->类型 == 项.节点目标.类型
                            && 节点记录->版本号 == 项.节点目标.身份版本
                            && 发布后读回一致;
                    }
                }
                for (const auto& 计划 : 删除计划组) {
                    const auto& 见证 = 计划.删除后见证;
                    const auto 当前 = 节点_->读取稳定主键当前身份(
                        见证.稳定主键, 许可.事务序号());
                    发布后读回一致 = 当前.状态 == 稳定主键当前身份状态::历史占用
                        && !当前.当前身份 && !当前.当前记录
                        && 发布后读回一致;
                }
                结果 = 形成幂等读回_(计划记录);
                结果.状态 = 发布后读回一致
                    ? 节点直接类型化结构数据操作状态::已提交
                    : 节点直接类型化结构数据操作状态::内部不一致;
                if (!发布后读回一致) 许可.标记隔离();
                节点直接持久发布见证请求 发布请求;
                发布请求.安装实例身份 = 请求.安装实例身份;
                发布请求.事务身份 = 请求.幂等身份;
                发布请求.尝试序号 = 尝试序号;
                发布请求.请求意图摘要 = 请求.请求意图摘要;
                发布请求.执行证据摘要 = 请求.执行证据摘要;
                发布请求.发布代次 = 当前代次 + 1;
                发布请求.结果摘要 = 计划记录.结果摘要;
                const auto 发布见证 = 持久证据_->标记已发布(发布请求);
                if ((发布见证.状态 == 节点直接持久端口状态::已见证
                        || 发布见证.状态 == 节点直接持久端口状态::精确同义)
                    && 发布见证.尝试序号 == 尝试序号
                    && 幂等_->单调记录持久证据状态(
                        请求.幂等身份, 尝试序号, 持久证据状态::已与内存代次一致)) {
                    结果.持久状态 = 持久证据状态::已与内存代次一致;
                } else if (发布见证.状态 == 节点直接持久端口状态::结果未知
                    || 发布见证.状态 == 节点直接持久端口状态::资源失败) {
                    (void)幂等_->单调记录持久证据状态(
                        请求.幂等身份, 尝试序号, 持久证据状态::持久化结果未知);
                    结果.持久状态 = 持久证据状态::持久化结果未知;
                } else {
                    (void)幂等_->单调记录持久证据状态(
                        请求.幂等身份, 尝试序号, 持久证据状态::持久证据损坏);
                    许可.标记隔离();
                    结果.状态 = 节点直接类型化结构数据操作状态::内部不一致;
                    结果.持久状态 = 持久证据状态::持久证据损坏;
                }
                return 结果;
            }

            const bool 参与者已完整撤销 = 逆序撤销类型化结构参与者组_(
                参与者组, 已进入事务参与者数量);
            const bool 幂等已完整撤销 = !幂等候选
                || 幂等_->撤销候选(*幂等候选, 许可.事务序号())
                    == 节点直接仓候选操作状态::已撤销;
            bool 业务已完整撤销 = true;
            for (std::size_t 序号 = 删除候选组.size(); 序号 > 0; --序号)
                业务已完整撤销 = 节点_->撤销节点删除候选(
                    删除候选组[序号 - 1], 许可.事务序号()).状态
                    == 节点直接身份写入状态::已撤销 && 业务已完整撤销;
            for (std::size_t 序号 = 候选组.size(); 序号 > 0; --序号)
                业务已完整撤销 = 索引_->撤销候选(候选组[序号 - 1], 许可.事务序号())
                    == 可重建索引操作状态::已撤销 && 业务已完整撤销;
            const bool 已完整撤销 = 参与者已完整撤销
                && 幂等已完整撤销 && 业务已完整撤销;
            节点直接持久撤销见证请求 撤销请求;
            撤销请求.安装实例身份 = 请求.安装实例身份;
            撤销请求.事务身份 = 请求.幂等身份;
            撤销请求.尝试序号 = 尝试序号;
            撤销请求.请求意图摘要 = 请求.请求意图摘要;
            撤销请求.执行证据摘要 = 请求.执行证据摘要;
            撤销请求.写集材料摘要 = 写集摘要.摘要;
            const auto 撤销见证 = 已完整撤销
                ? 持久证据_->标记已撤销未发布(撤销请求)
                : 节点直接持久端口结果{};
            if (已完整撤销
                && (撤销见证.状态 == 节点直接持久端口状态::已撤销未发布
                    || 撤销见证.状态 == 节点直接持久端口状态::精确同义)
                && 撤销见证.尝试序号 == 尝试序号
                && 幂等_->清除临时持久证据侧账(请求.幂等身份, 尝试序号)) {
                结果.状态 = 首次失败;
                return 结果;
            }
            许可.标记隔离();
            结果.状态 = 节点直接类型化结构数据操作状态::内部不一致;
            return 结果;
        }

        const bool 仅含节点关系和索引创建 = !请求.写项组.empty()
            && std::all_of(请求.写项组.begin(), 请求.写项组.end(), [](const auto& 写项) {
                return std::holds_alternative<节点直接节点创建项>(写项)
                    || std::holds_alternative<节点直接关系创建项>(写项)
                    || std::holds_alternative<节点直接索引创建项>(写项);
            });
        const bool 读回仅含本次节点关系和当前索引 = std::all_of(
            请求.读回规格.项目组.begin(), 请求.读回规格.项目组.end(), [](const auto& 项) {
                if (const auto* 身份 = std::get_if<节点直接写集局部身份>(&项.身份)) {
                    return (项.种类 == 节点直接发布后读回对象种类::当前节点
                            && 身份->种类 == 节点直接写集局部身份种类::节点)
                        || (项.种类 == 节点直接发布后读回对象种类::当前关系
                            && 身份->种类 == 节点直接写集局部身份种类::关系);
                }
                return 项.种类 == 节点直接发布后读回对象种类::当前索引
                    && std::holds_alternative<索引物理键>(项.身份) && 项.预期版本 == 0;
            });
        if (仅含节点关系和索引创建 && 读回仅含本次节点关系和当前索引) {
            struct 节点计划 {
                节点直接写集局部身份 局部身份;
                节点类型 类型{};
                节点稳定身份见证 见证;
            };
            struct 节点端点计划 {
                bool 是局部 = false;
                节点直接写集局部身份 局部身份;
                节点句柄 外部句柄;
                节点稳定身份见证 见证;
            };
            struct 节点计划结果 {
                节点直接写集局部身份 局部身份;
                节点句柄 句柄;
                节点稳定身份见证 见证;
            };
            struct 关系计划 {
                const 节点直接关系创建项* 项 = nullptr;
                关系稳定主键 稳定主键;
                节点端点计划 源端;
                节点端点计划 目标端;
            };
            struct 关系计划结果 {
                节点直接写集局部身份 局部身份;
                关系句柄 句柄;
                关系稳定身份见证 见证;
                节点句柄 源端句柄;
                节点句柄 目标端句柄;
            };
            struct 索引计划 {
                索引物理键 键;
                节点端点计划 目标;
            };
            struct 索引计划结果 {
                索引物理键 键;
                节点稳定身份见证 节点目标;
            };
            std::vector<const 节点直接节点创建项*> 节点项组;
            std::vector<const 节点直接关系创建项*> 关系项组;
            std::vector<const 节点直接索引创建项*> 索引项组;
            for (const auto& 写项 : 请求.写项组) {
                if (const auto* 项 = std::get_if<节点直接节点创建项>(&写项)) 节点项组.push_back(项);
                else if (const auto* 项 = std::get_if<节点直接关系创建项>(&写项)) 关系项组.push_back(项);
                else 索引项组.push_back(&std::get<节点直接索引创建项>(写项));
            }
            const auto 按计划键排序 = [&](const auto* 左, const auto* 右) {
                const auto* 左映射 = 查找计划身份_(*计划身份映射,
                    std::is_same_v<std::decay_t<decltype(*左)>, 节点直接节点创建项>
                        ? 左->局部身份 : 左->局部身份);
                const auto* 右映射 = 查找计划身份_(*计划身份映射,
                    std::is_same_v<std::decay_t<decltype(*右)>, 节点直接节点创建项>
                        ? 右->局部身份 : 右->局部身份);
                return 左映射 != nullptr && 右映射 != nullptr
                    && (左映射->稳定身份命名域 != 右映射->稳定身份命名域
                        ? 左映射->稳定身份命名域 < 右映射->稳定身份命名域
                        : 左映射->计划键 < 右映射->计划键);
            };
            std::sort(节点项组.begin(), 节点项组.end(), 按计划键排序);
            std::sort(关系项组.begin(), 关系项组.end(), 按计划键排序);
            std::sort(索引项组.begin(), 索引项组.end(), [](const auto* 左, const auto* 右) {
                return std::tie(左->键.所有者身份, 左->键.命名域, 左->键.键格式版本,
                        左->键.探测规则版本, 左->键.键值)
                    < std::tie(右->键.所有者身份, 右->键.命名域, 右->键.键格式版本,
                        右->键.探测规则版本, 右->键.键值);
            });

            std::vector<节点直接身份未发布候选> 节点候选组;
            std::vector<正式关系候选> 关系候选组;
            std::vector<可重建索引候选> 索引候选组;
            std::vector<节点计划> 节点计划组;
            std::vector<关系计划> 关系计划组;
            std::vector<索引计划> 索引计划组;
            std::vector<节点计划结果> 节点结果组;
            std::vector<关系计划结果> 关系结果组;
            std::vector<索引计划结果> 索引结果组;
            节点直接类型化结构数据操作状态 首次失败 = 节点直接类型化结构数据操作状态::内部不一致;
            bool 候选完整 = true;
            try {
                节点候选组.reserve(节点项组.size());
                关系候选组.reserve(关系项组.size());
                索引候选组.reserve(索引项组.size());
                节点计划组.reserve(节点项组.size());
                关系计划组.reserve(关系项组.size());
                索引计划组.reserve(索引项组.size());
                节点结果组.reserve(节点项组.size());
                关系结果组.reserve(关系项组.size());
                索引结果组.reserve(索引项组.size());
            } catch (...) {
                候选完整 = false;
                首次失败 = 节点直接类型化结构数据操作状态::资源失败;
            }

            auto 查找节点计划 = [&](节点直接写集局部身份 身份) -> const 节点计划* {
                const auto 位置 = std::find_if(节点计划组.begin(), 节点计划组.end(),
                    [&](const auto& 项) { return 项.局部身份 == 身份; });
                return 位置 == 节点计划组.end() ? nullptr : &*位置;
            };
            auto 预扫描节点端点 = [&](const 节点直接节点端点引用& 引用)
                -> std::optional<节点端点计划> {
                if (const auto* 局部 = std::get_if<节点直接写集局部身份>(&引用)) {
                    const auto* 计划 = 查找节点计划(*局部);
                    if (计划 == nullptr) return std::nullopt;
                    return 节点端点计划{true, *局部, {}, 计划->见证};
                }
                const auto& 见证 = std::get<节点稳定身份见证>(引用);
                const auto 当前 = 节点_->读取稳定主键当前身份(见证.稳定主键, 许可.事务序号());
                if (当前.状态 != 稳定主键当前身份状态::当前有效 || !当前.当前身份 || !当前.当前记录
                    || 当前.当前记录->类型 != 见证.类型 || 当前.当前记录->版本号 != 见证.身份版本) {
                    return std::nullopt;
                }
                return 节点端点计划{false, {}, *当前.当前身份, 见证};
            };
            auto 解析已复核节点端点 = [&](const 节点端点计划& 计划)
                -> std::optional<std::pair<节点句柄, 节点稳定身份见证>> {
                if (!计划.是局部) return std::pair{计划.外部句柄, 计划.见证};
                const auto 位置 = std::find_if(节点结果组.begin(), 节点结果组.end(),
                    [&](const auto& 项) { return 项.局部身份 == 计划.局部身份; });
                return 位置 == 节点结果组.end()
                    ? std::nullopt
                    : std::optional<std::pair<节点句柄, 节点稳定身份见证>>{
                        std::pair{位置->句柄, 位置->见证}};
            };

            std::uint64_t 当前节点命名域 = 0;
            std::uint64_t 当前节点模拟高水位 = 0;
            std::uint64_t 当前关系模拟高水位 = 关系_->读取命名域高水位(
                关系稳定主键命名域::正式关系);
            try {
                for (const auto* 项 : 节点项组) {
                    if (!候选完整) break;
                    const auto* 映射 = 查找计划身份_(*计划身份映射, 项->局部身份);
                    if (映射 == nullptr || 映射->稳定身份命名域 != static_cast<std::uint64_t>(项->命名域)) {
                        候选完整 = false;
                        首次失败 = 节点直接类型化结构数据操作状态::入口拒绝;
                        break;
                    }
                    const 节点稳定主键 稳定主键{映射->稳定身份命名域, 映射->计划键};
                    if (当前节点命名域 != 映射->稳定身份命名域) {
                        当前节点命名域 = 映射->稳定身份命名域;
                        当前节点模拟高水位 = 节点_->读取命名域高水位(项->命名域);
                    }
                    if (当前节点模拟高水位 == std::numeric_limits<std::uint64_t>::max()
                        || 映射->计划键 != 当前节点模拟高水位 + 1) {
                        候选完整 = false;
                        首次失败 = 节点直接类型化结构数据操作状态::版本漂移;
                        break;
                    }
                    当前节点模拟高水位 = 映射->计划键;
                    节点计划组.push_back({项->局部身份, 项->类型, {稳定主键, 项->类型, 1}});
                }
                for (const auto* 项 : 关系项组) {
                    if (!候选完整) break;
                    const auto* 映射 = 查找计划身份_(*计划身份映射, 项->局部身份);
                    const auto 源端 = 预扫描节点端点(项->源端);
                    const auto 目标端 = 预扫描节点端点(项->目标端);
                    if (映射 == nullptr || !源端 || !目标端) {
                        候选完整 = false;
                        首次失败 = 节点直接类型化结构数据操作状态::入口拒绝;
                        break;
                    }
                    if (当前关系模拟高水位 == std::numeric_limits<std::uint64_t>::max()
                        || 映射->计划键 != 当前关系模拟高水位 + 1) {
                        候选完整 = false;
                        首次失败 = 节点直接类型化结构数据操作状态::版本漂移;
                        break;
                    }
                    当前关系模拟高水位 = 映射->计划键;
                    关系计划组.push_back({项,
                        {映射->稳定身份命名域, 映射->计划键}, *源端, *目标端});
                }
                for (const auto* 项 : 索引项组) {
                    if (!候选完整) break;
                    const auto 目标 = 预扫描节点端点(项->目标);
                    if (!目标) {
                        候选完整 = false;
                        首次失败 = 节点直接类型化结构数据操作状态::入口拒绝;
                        break;
                    }
                    if (const auto 当前 = 索引_->读取索引物理键(
                            项->键, 许可.事务序号())) {
                        bool 精确同义 = 当前->目标种类 == 索引目标种类::节点;
                        if (精确同义) {
                            const auto 当前节点 = 节点_->读取节点(
                                当前->节点, 许可.事务序号());
                            精确同义 = 当前节点
                                && 当前节点->稳定主键 == 目标->见证.稳定主键
                                && 当前节点->类型 == 目标->见证.类型
                                && 当前节点->版本号 == 目标->见证.身份版本;
                        }
                        (void)精确同义;
                        候选完整 = false;
                        首次失败 = 节点直接类型化结构数据操作状态::入口拒绝;
                        break;
                    }
                    索引计划组.push_back({项->键, *目标});
                }
            } catch (...) {
                候选完整 = false;
                首次失败 = 节点直接类型化结构数据操作状态::资源失败;
            }

            std::size_t 已进入事务参与者数量 = 0;
            if (候选完整) {
                const auto 复核状态 = 第一写前复核类型化结构参与者组_(
                    请求, 许可.事务序号(), 当前代次,
                    参与者组, 已进入事务参与者数量);
                if (复核状态 != 节点直接类型化结构事务参与者状态::第一写前已复核) {
                    候选完整 = false;
                    首次失败 = 映射类型化结构参与者失败_(复核状态);
                }
            }
            try {
                for (const auto& 计划 : 节点计划组) {
                    if (!候选完整) break;
                    auto 创建 = 节点_->结构化创建节点未发布候选(
                        计划.类型, 计划.见证.稳定主键, 许可.事务序号());
                    if (创建.状态 != 节点直接身份写入状态::已形成候选 || !创建.候选) {
                        候选完整 = false;
                        首次失败 = 创建.状态 == 节点直接身份写入状态::资源失败
                            ? 节点直接类型化结构数据操作状态::资源失败
                            : 创建.状态 == 节点直接身份写入状态::版本漂移
                                ? 节点直接类型化结构数据操作状态::版本漂移
                                : 节点直接类型化结构数据操作状态::入口拒绝;
                        break;
                    }
                    节点结果组.push_back({计划.局部身份, 创建.当前身份, 计划.见证});
                    节点候选组.push_back(std::move(*创建.候选));
                }
                for (const auto& 计划 : 关系计划组) {
                    if (!候选完整) break;
                    const auto 源端 = 解析已复核节点端点(计划.源端);
                    const auto 目标端 = 解析已复核节点端点(计划.目标端);
                    if (!源端 || !目标端) {
                        候选完整 = false;
                        首次失败 = 节点直接类型化结构数据操作状态::内部不一致;
                        break;
                    }
                    auto 创建 = 关系_->结构化创建关系未发布候选(
                        计划.项->类型, 源端->first, 目标端->first, 计划.项->角色或顺序,
                        计划.稳定主键, 许可.事务序号());
                    if ((创建.状态 != 正式关系操作状态::已创建候选
                            && 创建.状态 != 正式关系操作状态::已变更候选)
                        || !创建.候选 || !创建.当前关系) {
                        候选完整 = false;
                        首次失败 = 创建.状态 == 正式关系操作状态::内部不一致
                            ? 节点直接类型化结构数据操作状态::内部不一致
                            : 创建.状态 == 正式关系操作状态::入口拒绝_版本漂移
                                ? 节点直接类型化结构数据操作状态::版本漂移
                                : 节点直接类型化结构数据操作状态::入口拒绝;
                        break;
                    }
                    关系结果组.push_back({计划.项->局部身份, *创建.当前关系,
                        {计划.稳定主键, 计划.项->类型, 1,
                            源端->second, 目标端->second, 计划.项->角色或顺序},
                        源端->first, 目标端->first});
                    关系候选组.push_back(std::move(*创建.候选));
                }
                for (const auto& 计划 : 索引计划组) {
                    if (!候选完整) break;
                    const auto 目标 = 解析已复核节点端点(计划.目标);
                    if (!目标) {
                        候选完整 = false;
                        首次失败 = 节点直接类型化结构数据操作状态::内部不一致;
                        break;
                    }
                    auto 创建 = 索引_->结构化创建索引候选(
                        {计划.键, 索引目标种类::节点, 目标->first, {}}, 许可.事务序号());
                    if (创建.状态 != 可重建索引操作状态::已创建候选 || !创建.候选) {
                        候选完整 = false;
                        首次失败 = 创建.状态 == 可重建索引操作状态::内部不一致
                            ? 节点直接类型化结构数据操作状态::内部不一致
                            : 节点直接类型化结构数据操作状态::入口拒绝;
                        break;
                    }
                    索引结果组.push_back({计划.键, 目标->second});
                    索引候选组.push_back(std::move(*创建.候选));
                }
            } catch (...) {
                候选完整 = false;
                首次失败 = 节点直接类型化结构数据操作状态::资源失败;
            }

            auto 撤销业务候选 = [&]() noexcept {
                bool 完整 = true;
                for (std::size_t 序号 = 索引候选组.size(); 序号 > 0; --序号)
                    完整 = 索引_->撤销候选(索引候选组[序号 - 1], 许可.事务序号())
                        == 可重建索引操作状态::已撤销 && 完整;
                for (std::size_t 序号 = 关系候选组.size(); 序号 > 0; --序号)
                    完整 = 关系_->撤销候选(关系候选组[序号 - 1], 许可.事务序号())
                        == 正式关系操作状态::已撤销 && 完整;
                for (std::size_t 序号 = 节点候选组.size(); 序号 > 0; --序号)
                    完整 = 节点_->撤销节点候选(节点候选组[序号 - 1], 许可.事务序号()).状态
                        == 节点直接身份写入状态::已撤销 && 完整;
                return 完整;
            };

            std::optional<节点直接事务幂等候选> 幂等候选;
            类型化结构参与者准备材料_ 参与者准备材料;
            if (候选完整) {
                std::vector<std::uint8_t> 结果材料 = 编码.材料;
                bool 结果可编码 = 追加无符号_(结果材料, 节点结果组.size(), 8);
                for (const auto& 项 : 节点结果组) 结果可编码 = 结果可编码 && 追加节点见证_(结果材料, 项.见证);
                结果可编码 = 结果可编码 && 追加无符号_(结果材料, 关系结果组.size(), 8);
                for (const auto& 项 : 关系结果组) {
                    结果可编码 = 结果可编码
                        && 追加无符号_(结果材料, 项.见证.稳定主键.命名域, 8)
                        && 追加无符号_(结果材料, 项.见证.稳定主键.键值, 8)
                        && 追加无符号_(结果材料, static_cast<std::uint32_t>(项.见证.类型), 4)
                        && 追加无符号_(结果材料, 项.见证.关系版本, 4)
                        && 追加节点见证_(结果材料, 项.见证.源端)
                        && 追加节点见证_(结果材料, 项.见证.目标端)
                        && 追加无符号_(结果材料, static_cast<std::uint64_t>(项.见证.角色或顺序), 8);
                }
                结果可编码 = 结果可编码 && 追加无符号_(结果材料, 索引结果组.size(), 8);
                for (const auto& 项 : 索引结果组) {
                    结果可编码 = 结果可编码 && 追加索引键_(结果材料, 项.键)
                        && 追加无符号_(结果材料, 1, 1)
                        && 追加节点见证_(结果材料, 项.节点目标)
                        && 追加无符号_(结果材料, 0, 1);
                }
                const auto 结果摘要 = 结果可编码 ? 计算SHA256_(结果材料) : 节点直接摘要计算结果{};
                节点直接事务幂等记录 计划记录;
                计划记录.安装实例身份 = 请求.安装实例身份;
                计划记录.幂等身份 = 请求.幂等身份;
                计划记录.请求意图摘要 = 请求.请求意图摘要;
                计划记录.执行证据摘要 = 请求.执行证据摘要;
                计划记录.材料格式版本 = 2;
                计划记录.写集规则版本 = 2;
                计划记录.写集材料 = 编码.材料;
                计划记录.状态 = 节点直接事务幂等记录状态::待发布;
                计划记录.发布代次 = 当前代次 + 1;
                if (结果摘要.状态 == 节点直接材料转换状态::成功) 计划记录.结果摘要 = 结果摘要.摘要;
                for (const auto& 读回项 : 请求.读回规格.项目组) {
                    if (读回项.种类 == 节点直接发布后读回对象种类::当前节点) {
                        const auto 局部身份 = std::get<节点直接写集局部身份>(读回项.身份);
                        const auto 位置 = std::find_if(节点结果组.begin(), 节点结果组.end(),
                            [&](const auto& 项) { return 项.局部身份 == 局部身份; });
                        if (位置 == 节点结果组.end() || 读回项.预期版本 != 位置->见证.身份版本) {
                            结果可编码 = false;
                            break;
                        }
                        计划记录.节点组.push_back(位置->见证);
                    } else if (读回项.种类 == 节点直接发布后读回对象种类::当前关系) {
                        const auto 局部身份 = std::get<节点直接写集局部身份>(读回项.身份);
                        const auto 位置 = std::find_if(关系结果组.begin(), 关系结果组.end(),
                            [&](const auto& 项) { return 项.局部身份 == 局部身份; });
                        if (位置 == 关系结果组.end() || 读回项.预期版本 != 位置->见证.关系版本) {
                            结果可编码 = false;
                            break;
                        }
                        计划记录.关系组.push_back(位置->见证);
                    } else {
                        const auto& 键 = std::get<索引物理键>(读回项.身份);
                        const auto 位置 = std::find_if(索引结果组.begin(), 索引结果组.end(),
                            [&](const auto& 项) { return 项.键 == 键; });
                        if (位置 == 索引结果组.end() || 读回项.预期版本 != 0) {
                            结果可编码 = false;
                            break;
                        }
                        计划记录.索引组.push_back({位置->键, true, 位置->节点目标, std::nullopt});
                    }
                }
                auto 幂等结果 = 结果摘要.状态 == 节点直接材料转换状态::成功
                        && 结果可编码
                    ? 幂等_->结构化建立记录未发布候选(计划记录, 许可.事务序号())
                    : 节点直接事务幂等候选结果{};
                if (幂等结果.状态 == 节点直接仓候选操作状态::已形成候选 && 幂等结果.候选) {
                    幂等候选.emplace(std::move(*幂等结果.候选));
                    try {
                        参与者准备材料.候选读回.节点组 = 计划记录.节点组;
                        参与者准备材料.候选读回.关系组 = 计划记录.关系组;
                        参与者准备材料.候选读回.索引组 = 计划记录.索引组;
                        参与者准备材料.计划身份见证组.reserve(
                            节点结果组.size() + 关系结果组.size());
                        for (const auto& 项 : 节点结果组)
                            参与者准备材料.计划身份见证组.push_back({项.局部身份, 项.见证});
                        for (const auto& 项 : 关系结果组)
                            参与者准备材料.计划身份见证组.push_back({项.局部身份, 项.见证});
                    } catch (...) {
                        候选完整 = false;
                        首次失败 = 节点直接类型化结构数据操作状态::资源失败;
                    }
                } else {
                    候选完整 = false;
                    首次失败 = 幂等结果.状态 == 节点直接仓候选操作状态::资源失败
                        ? 节点直接类型化结构数据操作状态::资源失败
                        : 节点直接类型化结构数据操作状态::内部不一致;
                }
                if (候选完整) {
                    const auto 准备状态 = 准备类型化结构参与者组_(
                        请求, 许可.事务序号(), 当前代次, 参与者组,
                        已进入事务参与者数量, 参与者准备材料);
                    if (准备状态 != 节点直接类型化结构事务参与者状态::已准备) {
                        候选完整 = false;
                        首次失败 = 映射类型化结构参与者失败_(准备状态);
                    }
                }
                if (候选完整) {
                    for (auto& 候选 : 节点候选组) {
                        if (节点_->确认节点候选(候选, 许可.事务序号()).状态
                            != 节点直接身份写入状态::已确认待发布) {
                            候选完整 = false;
                            break;
                        }
                    }
                }
                if (候选完整) {
                    for (auto& 候选 : 关系候选组) {
                        if (关系_->确认候选(候选, 许可.事务序号())
                            != 正式关系操作状态::已确认待发布) {
                            候选完整 = false;
                            break;
                        }
                    }
                }
                if (候选完整) {
                    for (auto& 候选 : 索引候选组) {
                        if (索引_->确认候选(候选, 许可.事务序号())
                            != 可重建索引操作状态::已确认待发布) {
                            候选完整 = false;
                            break;
                        }
                    }
                }
                if (候选完整
                    && 幂等_->确认候选(*幂等候选, 许可.事务序号())
                        != 节点直接仓候选操作状态::已确认待发布) {
                    候选完整 = false;
                }
                if (候选完整) {
                    const auto 确认状态 = 确认类型化结构参与者组_(参与者组);
                    if (确认状态 != 节点直接类型化结构事务参与者状态::已确认待发布) {
                        候选完整 = false;
                        首次失败 = 映射类型化结构参与者失败_(确认状态);
                    }
                }
                if (候选完整) {
                    bool 发布完整 = true;
                    for (auto& 候选 : 节点候选组)
                        节点_->完成发布节点候选(候选, 许可.事务序号());
                    for (auto& 候选 : 关系候选组)
                        关系_->完成发布(候选, 许可.事务序号());
                    for (auto& 候选 : 索引候选组)
                        索引_->完成发布(候选, 许可.事务序号());
                    发布完整 = 幂等_->完成发布(*幂等候选, 许可.事务序号())
                        == 节点直接仓候选操作状态::已发布 && 发布完整;
                    发布完整 = 发布完整 && 完成类型化结构参与者发布_(参与者组);
                    发布完整 = 发布完整 && 事务域_->推进普通已发布代次(许可, 当前代次);
                    if (发布完整) {
                        结果 = 形成幂等读回_(计划记录);
                        结果.状态 = 节点直接类型化结构数据操作状态::已提交;
                        bool 发布后读回一致 = true;
                        for (const auto& 项 : 节点结果组) {
                            const auto 当前 = 节点_->读取稳定主键当前身份(
                                项.见证.稳定主键, 许可.事务序号());
                            发布后读回一致 = 当前.状态 == 稳定主键当前身份状态::当前有效
                                && 当前.当前身份 && 当前.当前记录
                                && *当前.当前身份 == 项.句柄
                                && 当前.当前记录->稳定主键 == 项.见证.稳定主键
                                && 当前.当前记录->类型 == 项.见证.类型
                                && 当前.当前记录->版本号 == 项.见证.身份版本
                                && 当前.当前记录->状态 == 记录状态::有效
                                && 发布后读回一致;
                        }
                        for (const auto& 项 : 关系结果组) {
                            const auto 当前 = 关系_->读取稳定主键当前关系(
                                项.见证.稳定主键, 许可.事务序号());
                            发布后读回一致 = 当前.状态 == 稳定关系当前读取状态::当前有效
                                && 当前.记录
                                && 当前.记录->稳定主键 == 项.见证.稳定主键
                                && 当前.记录->类型 == 项.见证.类型
                                && 当前.记录->版本号 == 项.见证.关系版本
                                && 当前.记录->源节点 == 项.源端句柄
                                && 当前.记录->目标节点 == 项.目标端句柄
                                && 当前.记录->顺序号 == 项.见证.角色或顺序
                                && 当前.记录->状态 == 记录状态::有效
                                && 发布后读回一致;
                        }
                        for (const auto& 项 : 索引结果组) {
                            const auto 当前 = 索引_->读取索引物理键(项.键, 许可.事务序号());
                            if (!当前 || 当前->目标种类 != 索引目标种类::节点) {
                                发布后读回一致 = false;
                                continue;
                            }
                            const auto 节点记录 = 节点_->读取节点(当前->节点, 许可.事务序号());
                            发布后读回一致 = 节点记录
                                && 节点记录->稳定主键 == 项.节点目标.稳定主键
                                && 节点记录->类型 == 项.节点目标.类型
                                && 节点记录->版本号 == 项.节点目标.身份版本
                                && 发布后读回一致;
                        }
                        if (!发布后读回一致) {
                            许可.标记隔离();
                            结果.状态 = 节点直接类型化结构数据操作状态::内部不一致;
                        }
                        节点直接持久发布见证请求 发布请求;
                        发布请求.安装实例身份 = 请求.安装实例身份;
                        发布请求.事务身份 = 请求.幂等身份;
                        发布请求.尝试序号 = 尝试序号;
                        发布请求.请求意图摘要 = 请求.请求意图摘要;
                        发布请求.执行证据摘要 = 请求.执行证据摘要;
                        发布请求.发布代次 = 当前代次 + 1;
                        发布请求.结果摘要 = 计划记录.结果摘要;
                        const auto 发布见证 = 持久证据_->标记已发布(发布请求);
                        if ((发布见证.状态 == 节点直接持久端口状态::已见证
                                || 发布见证.状态 == 节点直接持久端口状态::精确同义)
                            && 发布见证.尝试序号 == 尝试序号
                            && 幂等_->单调记录持久证据状态(
                                请求.幂等身份, 尝试序号, 持久证据状态::已与内存代次一致)) {
                            结果.持久状态 = 持久证据状态::已与内存代次一致;
                        } else if (发布见证.状态 == 节点直接持久端口状态::结果未知
                            || 发布见证.状态 == 节点直接持久端口状态::资源失败) {
                            (void)幂等_->单调记录持久证据状态(
                                请求.幂等身份, 尝试序号, 持久证据状态::持久化结果未知);
                            结果.持久状态 = 持久证据状态::持久化结果未知;
                        } else {
                            (void)幂等_->单调记录持久证据状态(
                                请求.幂等身份, 尝试序号, 持久证据状态::持久证据损坏);
                            许可.标记隔离();
                            结果.状态 = 节点直接类型化结构数据操作状态::内部不一致;
                            结果.持久状态 = 持久证据状态::持久证据损坏;
                        }
                        return 结果;
                    }
                    许可.标记隔离();
                    结果.状态 = 节点直接类型化结构数据操作状态::内部不一致;
                    return 结果;
                }
            }

            const bool 参与者已完整撤销 = 逆序撤销类型化结构参与者组_(
                参与者组, 已进入事务参与者数量);
            const bool 幂等已完整撤销 = !幂等候选
                || 幂等_->撤销候选(*幂等候选, 许可.事务序号())
                    == 节点直接仓候选操作状态::已撤销;
            const bool 已完整撤销 = 参与者已完整撤销
                && 幂等已完整撤销 && 撤销业务候选();
            节点直接持久撤销见证请求 撤销请求;
            撤销请求.安装实例身份 = 请求.安装实例身份;
            撤销请求.事务身份 = 请求.幂等身份;
            撤销请求.尝试序号 = 尝试序号;
            撤销请求.请求意图摘要 = 请求.请求意图摘要;
            撤销请求.执行证据摘要 = 请求.执行证据摘要;
            撤销请求.写集材料摘要 = 写集摘要.摘要;
            const auto 撤销见证 = 已完整撤销
                ? 持久证据_->标记已撤销未发布(撤销请求)
                : 节点直接持久端口结果{};
            if (已完整撤销
                && (撤销见证.状态 == 节点直接持久端口状态::已撤销未发布
                    || 撤销见证.状态 == 节点直接持久端口状态::精确同义)
                && 撤销见证.尝试序号 == 尝试序号
                && 幂等_->清除临时持久证据侧账(请求.幂等身份, 尝试序号)) {
                结果.状态 = 首次失败;
                return 结果;
            }
            许可.标记隔离();
            结果.状态 = 节点直接类型化结构数据操作状态::内部不一致;
            return 结果;
        }

        // 非空业务候选链在后续固定顺序段接续；在形成任何候选前先持久见证本轮安全撤销。
        节点直接持久撤销见证请求 撤销请求;
        撤销请求.安装实例身份 = 请求.安装实例身份;
        撤销请求.事务身份 = 请求.幂等身份;
        撤销请求.尝试序号 = 尝试序号;
        撤销请求.请求意图摘要 = 请求.请求意图摘要;
        撤销请求.执行证据摘要 = 请求.执行证据摘要;
        撤销请求.写集材料摘要 = 写集摘要.摘要;
        const auto 撤销见证 = 持久证据_->标记已撤销未发布(撤销请求);
        if ((撤销见证.状态 == 节点直接持久端口状态::已撤销未发布
                || 撤销见证.状态 == 节点直接持久端口状态::精确同义)
            && 撤销见证.尝试序号 == 尝试序号
            && 幂等_->清除临时持久证据侧账(请求.幂等身份, 尝试序号)) {
            结果.状态 = 节点直接类型化结构数据操作状态::内部不一致;
            return 结果;
        }
        许可.标记隔离();
        结果.状态 = 节点直接类型化结构数据操作状态::内部不一致;
        return 结果;
    }

    static inline 节点直接身份结构写入结果 入口拒绝结果() noexcept {
        return {节点直接身份结构写入状态::入口拒绝, 0, 0, 0};
    }

    struct 类型化结构参与者准备材料_ {
        std::vector<节点直接类型化结构计划身份见证项> 计划身份见证组;
        节点直接类型化结构候选读回 候选读回;
    };

    static bool 有效类型化结构参与者组_(
        std::span<节点直接类型化结构事务参与者* const> 参与者组) noexcept {
        for (std::size_t 左序号 = 0; 左序号 < 参与者组.size(); ++左序号) {
            if (参与者组[左序号] == nullptr) return false;
            for (std::size_t 右序号 = 左序号 + 1; 右序号 < 参与者组.size(); ++右序号) {
                if (参与者组[左序号] == 参与者组[右序号]) return false;
            }
        }
        return true;
    }

    static 节点直接类型化结构事务参与者状态 形成类型化结构参与者准备材料_(
        const std::vector<节点直接计划身份映射项>& 计划身份映射,
        const 节点直接事务幂等记录& 计划记录,
        类型化结构参与者准备材料_& 输出) noexcept {
        try {
            输出.候选读回.节点组 = 计划记录.节点组;
            输出.候选读回.关系组 = 计划记录.关系组;
            输出.候选读回.类型化值组 = 计划记录.类型化值组;
            输出.候选读回.索引组 = 计划记录.索引组;
            输出.计划身份见证组.clear();
            输出.计划身份见证组.reserve(计划身份映射.size());
            for (const auto& 映射 : 计划身份映射) {
                if (映射.局部身份.种类 == 节点直接写集局部身份种类::节点) {
                    const auto 匹配 = [&](const auto& 项) {
                        return 项.稳定主键.命名域 == 映射.稳定身份命名域
                            && 项.稳定主键.键值 == 映射.计划键
                            && 项.身份版本 == 映射.初始版本;
                    };
                    const auto 位置 = std::find_if(计划记录.节点组.begin(), 计划记录.节点组.end(), 匹配);
                    if (位置 == 计划记录.节点组.end()
                        || std::count_if(计划记录.节点组.begin(), 计划记录.节点组.end(), 匹配) != 1)
                        return 节点直接类型化结构事务参与者状态::内部不一致;
                    输出.计划身份见证组.push_back({映射.局部身份, *位置});
                } else if (映射.局部身份.种类 == 节点直接写集局部身份种类::关系) {
                    const auto 匹配 = [&](const auto& 项) {
                        return 项.稳定主键.命名域 == 映射.稳定身份命名域
                            && 项.稳定主键.键值 == 映射.计划键
                            && 项.关系版本 == 映射.初始版本;
                    };
                    const auto 位置 = std::find_if(计划记录.关系组.begin(), 计划记录.关系组.end(), 匹配);
                    if (位置 == 计划记录.关系组.end()
                        || std::count_if(计划记录.关系组.begin(), 计划记录.关系组.end(), 匹配) != 1)
                        return 节点直接类型化结构事务参与者状态::内部不一致;
                    输出.计划身份见证组.push_back({映射.局部身份, *位置});
                } else if (映射.局部身份.种类 == 节点直接写集局部身份种类::类型化值记录) {
                    const auto 匹配 = [&](const auto& 项) {
                        return 项.值记录身份.命名域 == 映射.稳定身份命名域
                            && 项.值记录身份.键值 == 映射.计划键
                            && 项.值记录版本 == 映射.初始版本;
                    };
                    const auto 位置 = std::find_if(
                        计划记录.类型化值组.begin(), 计划记录.类型化值组.end(), 匹配);
                    if (位置 == 计划记录.类型化值组.end()
                        || std::count_if(
                            计划记录.类型化值组.begin(), 计划记录.类型化值组.end(), 匹配) != 1)
                        return 节点直接类型化结构事务参与者状态::内部不一致;
                    输出.计划身份见证组.push_back({映射.局部身份, *位置});
                } else {
                    return 节点直接类型化结构事务参与者状态::内部不一致;
                }
            }
            return 节点直接类型化结构事务参与者状态::已准备;
        } catch (...) {
            return 节点直接类型化结构事务参与者状态::资源失败;
        }
    }

    static 节点直接类型化结构事务参与者结果 安全第一写前复核类型化结构参与者_(
        节点直接类型化结构事务参与者& 参与者,
        const 节点直接类型化结构第一写前复核只读视图& 视图) noexcept {
        try { return 参与者.第一写前复核(视图); }
        catch (...) { return {节点直接类型化结构事务参与者状态::内部不一致}; }
    }

    static 节点直接类型化结构事务参与者状态 第一写前复核类型化结构参与者组_(
        const 节点直接类型化结构数据操作请求& 请求,
        std::uint64_t 事务序号,
        std::uint64_t 当前事实截止代次,
        std::span<节点直接类型化结构事务参与者* const> 参与者组,
        std::size_t& 已进入事务参与者数量) noexcept {
        已进入事务参与者数量 = 0;
        if (参与者组.empty())
            return 节点直接类型化结构事务参与者状态::第一写前已复核;
        const 节点直接类型化结构第一写前复核只读视图 视图(
            事务序号, 当前事实截止代次, 当前事实截止代次 + 1, 请求);
        for (std::size_t 序号 = 0; 序号 < 参与者组.size(); ++序号) {
            已进入事务参与者数量 = 序号 + 1;
            const auto 结果 = 安全第一写前复核类型化结构参与者_(
                *参与者组[序号], 视图);
            if (结果.状态 != 节点直接类型化结构事务参与者状态::第一写前已复核)
                return 结果.状态;
        }
        return 节点直接类型化结构事务参与者状态::第一写前已复核;
    }

    static 节点直接类型化结构事务参与者结果 安全准备类型化结构参与者_(
        节点直接类型化结构事务参与者& 参与者,
        const 节点直接类型化结构提交准备只读视图& 视图) noexcept {
        try { return 参与者.准备提交(视图); }
        catch (...) { return {节点直接类型化结构事务参与者状态::内部不一致}; }
    }

    static 节点直接类型化结构事务参与者结果 安全确认类型化结构参与者_(
        节点直接类型化结构事务参与者& 参与者) noexcept {
        try { return 参与者.确认待发布(); }
        catch (...) { return {节点直接类型化结构事务参与者状态::内部不一致}; }
    }

    static 节点直接类型化结构事务参与者结果 安全撤销类型化结构参与者_(
        节点直接类型化结构事务参与者& 参与者) noexcept {
        try { return 参与者.完成撤销(); }
        catch (...) { return {节点直接类型化结构事务参与者状态::内部不一致}; }
    }

    static 节点直接类型化结构事务参与者结果 安全完成类型化结构参与者发布_(
        节点直接类型化结构事务参与者& 参与者) noexcept {
        try { return 参与者.完成发布(); }
        catch (...) { return {节点直接类型化结构事务参与者状态::内部不一致}; }
    }

    static 节点直接类型化结构事务参与者状态 准备类型化结构参与者组_(
        const 节点直接类型化结构数据操作请求& 请求,
        std::uint64_t 事务序号,
        std::uint64_t 当前代次,
        std::span<节点直接类型化结构事务参与者* const> 参与者组,
        std::size_t& 已进入事务参与者数量,
        const 类型化结构参与者准备材料_& 材料) noexcept {
        if (参与者组.empty()) return 节点直接类型化结构事务参与者状态::已准备;
        const 节点直接类型化结构提交准备只读视图 视图(
            事务序号, 当前代次, 当前代次 + 1, 请求,
            材料.计划身份见证组, 材料.候选读回);
        for (std::size_t 序号 = 0; 序号 < 参与者组.size(); ++序号) {
            已进入事务参与者数量 = std::max(已进入事务参与者数量, 序号 + 1);
            const auto 结果 = 安全准备类型化结构参与者_(*参与者组[序号], 视图);
            if (结果.状态 != 节点直接类型化结构事务参与者状态::已准备) return 结果.状态;
        }
        return 节点直接类型化结构事务参与者状态::已准备;
    }

    static 节点直接类型化结构事务参与者状态 准备类型化结构参与者组_(
        const 节点直接类型化结构数据操作请求& 请求,
        const std::vector<节点直接计划身份映射项>& 计划身份映射,
        const 节点直接事务幂等记录& 计划记录,
        std::uint64_t 事务序号,
        std::uint64_t 当前代次,
        std::span<节点直接类型化结构事务参与者* const> 参与者组,
        std::size_t& 已进入事务参与者数量,
        类型化结构参与者准备材料_& 材料) noexcept {
        if (参与者组.empty()) return 节点直接类型化结构事务参与者状态::已准备;
        const auto 材料状态 = 形成类型化结构参与者准备材料_(计划身份映射, 计划记录, 材料);
        if (材料状态 != 节点直接类型化结构事务参与者状态::已准备) return 材料状态;
        return 准备类型化结构参与者组_(
            请求, 事务序号, 当前代次, 参与者组, 已进入事务参与者数量, 材料);
    }

    static 节点直接类型化结构事务参与者状态 确认类型化结构参与者组_(
        std::span<节点直接类型化结构事务参与者* const> 参与者组) noexcept {
        for (auto* 参与者 : 参与者组) {
            const auto 结果 = 安全确认类型化结构参与者_(*参与者);
            if (结果.状态 != 节点直接类型化结构事务参与者状态::已确认待发布) return 结果.状态;
        }
        return 节点直接类型化结构事务参与者状态::已确认待发布;
    }

    static bool 逆序撤销类型化结构参与者组_(
        std::span<节点直接类型化结构事务参与者* const> 参与者组,
        std::size_t 已触及数量) noexcept {
        bool 完整 = true;
        for (std::size_t 序号 = std::min(已触及数量, 参与者组.size()); 序号 > 0; --序号) {
            const auto 结果 = 安全撤销类型化结构参与者_(*参与者组[序号 - 1]);
            完整 = 结果.状态 == 节点直接类型化结构事务参与者状态::已撤销 && 完整;
        }
        return 完整;
    }

    static bool 完成类型化结构参与者发布_(
        std::span<节点直接类型化结构事务参与者* const> 参与者组) noexcept {
        for (auto* 参与者 : 参与者组) {
            const auto 结果 = 安全完成类型化结构参与者发布_(*参与者);
            if (结果.状态 != 节点直接类型化结构事务参与者状态::已完成发布) return false;
        }
        return true;
    }

    static 节点直接类型化结构数据操作状态 映射类型化结构参与者失败_(
        节点直接类型化结构事务参与者状态 状态) noexcept {
        switch (状态) {
        case 节点直接类型化结构事务参与者状态::入口拒绝:
            return 节点直接类型化结构数据操作状态::入口拒绝;
        case 节点直接类型化结构事务参与者状态::版本漂移:
            return 节点直接类型化结构数据操作状态::版本漂移;
        case 节点直接类型化结构事务参与者状态::许可拒绝:
            return 节点直接类型化结构数据操作状态::许可拒绝;
        case 节点直接类型化结构事务参与者状态::资源失败:
            return 节点直接类型化结构数据操作状态::资源失败;
        default:
            return 节点直接类型化结构数据操作状态::内部不一致;
        }
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
    节点直接类型合同仓库* 类型合同_ = nullptr;
    节点直接类型化值仓库* 类型化值_ = nullptr;
    节点直接事务幂等仓库* 幂等_ = nullptr;
    节点直接持久证据写入端口* 持久证据_ = nullptr;
};

}
