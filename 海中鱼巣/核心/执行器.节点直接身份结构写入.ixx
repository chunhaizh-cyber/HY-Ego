// 文件规则：节点直接身份结构写入执行器只编排隔离新域独占许可、结构会话和强类型参与者，不承载领域值或业务准入。
module;

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <array>
#include <atomic>
#include <Windows.h>
#include <bcrypt.h>
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
        return 执行类型化结构事务_(规格.请求, false);
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
        return 执行类型化结构事务_(请求, true);
    }

    节点直接恢复数据操作结果 执行节点直接恢复事务(
        const 节点直接恢复数据操作规格&) const {
        return {};
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

    static bool 追加端点_(std::vector<std::uint8_t>& 输出, const 节点直接节点端点引用& 值) {
        if (!追加字节_(输出, static_cast<std::uint8_t>(值.index() + 1))) return false;
        return std::visit([&](const auto& 项) -> bool {
            using 类型 = std::decay_t<decltype(项)>;
            if constexpr (std::is_same_v<类型, 节点稳定身份见证>) return 追加节点见证_(输出, 项);
            else return 追加字节_(输出, static_cast<std::uint8_t>(项.种类))
                && 追加无符号_(输出, 项.值, 8);
        }, 值);
    }

    static bool 追加类型化值材料_(std::vector<std::uint8_t>& 输出, const 类型化值材料& 材料) {
        if (!追加字节_(输出, static_cast<std::uint8_t>(材料.index() + 1))) return false;
        return std::visit([&](const auto& 值) -> bool {
            using 类型 = std::decay_t<decltype(值)>;
            if constexpr (std::is_same_v<类型, std::int64_t>) return 追加无符号_(输出, static_cast<std::uint64_t>(值), 8);
            else if constexpr (std::is_same_v<类型, std::uint64_t>) return 追加无符号_(输出, 值, 8);
            else if constexpr (std::is_same_v<类型, I64区间材料>) return 追加无符号_(输出, static_cast<std::uint64_t>(值.下界), 8)
                && 追加无符号_(输出, static_cast<std::uint64_t>(值.上界), 8);
            else if constexpr (std::is_same_v<类型, std::vector<std::int64_t>>
                || std::is_same_v<类型, std::vector<std::uint64_t>>) {
                if (值.size() > 最大向量项目数_ || !追加无符号_(输出, 值.size(), 8)) return false;
                for (const auto 项 : 值) if (!追加无符号_(输出, static_cast<std::uint64_t>(项), 8)) return false;
                return true;
            } else if constexpr (std::is_same_v<类型, 稳定身份有序组材料>) {
                if (值.项目组.size() > 最大向量项目数_ || !追加无符号_(输出, 值.项目组.size(), 8)) return false;
                for (const auto& 项 : 值.项目组) if (!追加节点见证_(输出, 项)) return false;
                return true;
            } else {
                if (!追加稳定主键_(输出, 值.材料身份)
                    || !追加无符号_(输出, 值.格式版本, 4)
                    || !追加无符号_(输出, 值.字节数, 8)) return false;
                for (const auto 字节 : 值.SHA256) if (!追加字节_(输出, 字节)) return false;
                return true;
            }
        }, 材料);
    }

    static bool 追加类型合同值域_(std::vector<std::uint8_t>& 输出, const 类型合同值域& 值域) {
        if (!追加字节_(输出, static_cast<std::uint8_t>(值域.index() + 1))) return false;
        return std::visit([&](const auto& 值) -> bool {
            using 类型 = std::decay_t<decltype(值)>;
            if constexpr (std::is_same_v<类型, I64标量值域> || std::is_same_v<类型, I64区间材料>)
                return 追加无符号_(输出, static_cast<std::uint64_t>(值.下界), 8)
                    && 追加无符号_(输出, static_cast<std::uint64_t>(值.上界), 8);
            else if constexpr (std::is_same_v<类型, U64标量值域>)
                return 追加无符号_(输出, 值.下界, 8) && 追加无符号_(输出, 值.上界, 8);
            else if constexpr (std::is_same_v<类型, I64有序组值域>)
                return 追加无符号_(输出, static_cast<std::uint64_t>(值.元素下界), 8)
                    && 追加无符号_(输出, static_cast<std::uint64_t>(值.元素上界), 8)
                    && 追加无符号_(输出, 值.最少项目数, 8) && 追加无符号_(输出, 值.最多项目数, 8);
            else if constexpr (std::is_same_v<类型, U64有序组值域>)
                return 追加无符号_(输出, 值.元素下界, 8) && 追加无符号_(输出, 值.元素上界, 8)
                    && 追加无符号_(输出, 值.最少项目数, 8) && 追加无符号_(输出, 值.最多项目数, 8);
            else if constexpr (std::is_same_v<类型, 稳定身份有序组值域>) {
                if (值.允许节点类型稳定值组.size() > 最大向量项目数_
                    || !追加无符号_(输出, 值.允许节点类型稳定值组.size(), 8)) return false;
                for (const auto 项 : 值.允许节点类型稳定值组) if (!追加无符号_(输出, 项, 8)) return false;
                return 追加无符号_(输出, 值.最少项目数, 8) && 追加无符号_(输出, 值.最多项目数, 8);
            } else return 追加无符号_(输出, 值.格式版本, 4)
                && 追加无符号_(输出, 值.最少字节数, 8) && 追加无符号_(输出, 值.最多字节数, 8);
        }, 值域);
    }

    static 节点直接写集编码结果 编码写集_(const 节点直接类型化结构数据操作请求& 请求) {
        节点直接写集编码结果 结果;
        if (请求.写项组.size() > 最大向量项目数_ || 请求.读回规格.项目组.size() > 最大向量项目数_) {
            结果.状态 = 节点直接材料转换状态::入口拒绝;
            return 结果;
        }
        try {
            auto& 输出 = 结果.材料;
            if (!追加无符号_(输出, 请求.合同版本, 4)
                || !追加无符号_(输出, 请求.写集规则版本, 4)
                || !追加无符号_(输出, 请求.安装实例身份.命名域, 8)
                || !追加无符号_(输出, 请求.安装实例身份.键值, 8)
                || !追加无符号_(输出, 请求.幂等身份.命名域, 8)
                || !追加无符号_(输出, 请求.幂等身份.键值, 8)
                || !追加无符号_(输出, 请求.预期事实截止代次, 8)
                || !追加无符号_(输出, 请求.写项组.size(), 8)) {
                结果.状态 = 节点直接材料转换状态::入口拒绝;
                return 结果;
            }
            for (const auto& 写项 : 请求.写项组) {
                if (!追加字节_(输出, static_cast<std::uint8_t>(写项.index() + 1))) {
                    结果.状态 = 节点直接材料转换状态::入口拒绝; return 结果;
                }
                const bool 成功 = std::visit([&](const auto& 项) -> bool {
                    using 类型 = std::decay_t<decltype(项)>;
                    if constexpr (std::is_same_v<类型, 节点直接类型合同发布项>) {
                        if (!追加无符号_(输出, 项.合同身份.命名域, 8) || !追加无符号_(输出, 项.合同身份.键值, 8)
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
                        return 追加字节_(输出, static_cast<std::uint8_t>(项.局部身份.种类)) && 追加无符号_(输出, 项.局部身份.值, 8)
                            && 追加无符号_(输出, static_cast<std::uint64_t>(项.类型), 4);
                    else if constexpr (std::is_same_v<类型, 节点直接关系创建项>)
                        return 追加字节_(输出, static_cast<std::uint8_t>(项.局部身份.种类)) && 追加无符号_(输出, 项.局部身份.值, 8)
                            && 追加无符号_(输出, static_cast<std::uint64_t>(项.类型), 4) && 追加端点_(输出, 项.源端)
                            && 追加端点_(输出, 项.目标端) && 追加无符号_(输出, static_cast<std::uint64_t>(项.角色或顺序), 8);
                    else if constexpr (std::is_same_v<类型, 节点直接关系失效项>)
                        return 追加无符号_(输出, 项.预期当前.稳定主键.命名域, 8) && 追加无符号_(输出, 项.预期当前.稳定主键.键值, 8)
                            && 追加无符号_(输出, static_cast<std::uint64_t>(项.预期当前.类型), 4) && 追加无符号_(输出, 项.预期当前.关系版本, 4)
                            && 追加节点见证_(输出, 项.预期当前.源端) && 追加节点见证_(输出, 项.预期当前.目标端)
                            && 追加无符号_(输出, static_cast<std::uint64_t>(项.预期当前.角色或顺序), 8);
                    else if constexpr (std::is_same_v<类型, 节点直接类型化值发布项>) {
                        if (!追加端点_(输出, 项.所属身份) || !追加无符号_(输出, 项.类型合同身份.命名域, 8)
                            || !追加无符号_(输出, 项.类型合同身份.键值, 8) || !追加无符号_(输出, 项.类型合同版本, 4)
                            || !追加字节_(输出, static_cast<std::uint8_t>(项.值记录局部身份.种类)) || !追加无符号_(输出, 项.值记录局部身份.值, 8)
                            || !追加字节_(输出, 项.预期当前值记录版本.has_value() ? 1 : 0)
                            || (项.预期当前值记录版本 && !追加无符号_(输出, *项.预期当前值记录版本, 8))
                            || !追加类型化值材料_(输出, 项.材料) || !追加字节_(输出, static_cast<std::uint8_t>(项.来源.index() + 1))) return false;
                        return std::visit([&](const auto& 来源) -> bool {
                            using 来源类型 = std::decay_t<decltype(来源)>;
                            if constexpr (std::is_same_v<来源类型, 节点直接节点端点引用>) return 追加端点_(输出, 来源);
                            else return 追加无符号_(输出, 来源.命名域, 8) && 追加无符号_(输出, 来源.键值, 8);
                        }, 项.来源);
                    } else if constexpr (std::is_same_v<类型, 节点直接类型化值退役项>)
                        return 追加节点见证_(输出, 项.所属身份) && 追加无符号_(输出, 项.类型合同身份.命名域, 8)
                            && 追加无符号_(输出, 项.类型合同身份.键值, 8) && 追加无符号_(输出, 项.类型合同版本, 4)
                            && 追加无符号_(输出, 项.值记录身份.命名域, 8) && 追加无符号_(输出, 项.值记录身份.键值, 8)
                            && 追加无符号_(输出, 项.预期值记录版本, 8);
                    else if constexpr (std::is_same_v<类型, 节点直接索引创建项>)
                        return 追加无符号_(输出, 项.键.所有者身份, 8) && 追加无符号_(输出, 项.键.命名域, 4)
                            && 追加无符号_(输出, 项.键.键格式版本, 4) && 追加无符号_(输出, 项.键.探测规则版本, 4)
                            && 追加无符号_(输出, 项.键.键值, 8) && 追加端点_(输出, 项.目标);
                    else if constexpr (std::is_same_v<类型, 节点直接索引移除项>)
                        return 追加无符号_(输出, 项.键.所有者身份, 8) && 追加无符号_(输出, 项.键.命名域, 4)
                            && 追加无符号_(输出, 项.键.键格式版本, 4) && 追加无符号_(输出, 项.键.探测规则版本, 4)
                            && 追加无符号_(输出, 项.键.键值, 8) && 追加节点见证_(输出, 项.预期目标);
                    else return 追加节点见证_(输出, 项.预期当前);
                }, 写项);
                if (!成功) { 结果.状态 = 节点直接材料转换状态::入口拒绝; return 结果; }
            }
            if (!追加无符号_(输出, 请求.读回规格.项目组.size(), 8)) {
                结果.状态 = 节点直接材料转换状态::入口拒绝; return 结果;
            }
            for (const auto& 项 : 请求.读回规格.项目组) {
                if (!追加字节_(输出, static_cast<std::uint8_t>(项.种类))
                    || !追加字节_(输出, static_cast<std::uint8_t>(项.身份.index() + 1))
                    || !追加无符号_(输出, 项.预期版本, 8)) {
                    结果.状态 = 节点直接材料转换状态::入口拒绝; return 结果;
                }
                const bool 成功 = std::visit([&](const auto& 身份) -> bool {
                    using 类型 = std::decay_t<decltype(身份)>;
                    if constexpr (std::is_same_v<类型, 节点直接写集局部身份>) return 追加字节_(输出, static_cast<std::uint8_t>(身份.种类)) && 追加无符号_(输出, 身份.值, 8);
                    else if constexpr (std::is_same_v<类型, 节点稳定主键>) return 追加稳定主键_(输出, 身份);
                    else if constexpr (std::is_same_v<类型, 关系稳定主键> || std::is_same_v<类型, 类型化值记录稳定身份>)
                        return 追加无符号_(输出, 身份.命名域, 8) && 追加无符号_(输出, 身份.键值, 8);
                    else return 追加无符号_(输出, 身份.所有者身份, 8) && 追加无符号_(输出, 身份.命名域, 4)
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

    节点直接类型化结构数据操作结果 执行类型化结构事务_(
        const 节点直接类型化结构数据操作请求& 请求,
        bool 允许空域治理) const {
        节点直接类型化结构数据操作结果 结果;
        if (!类型化结构有效() || 请求.合同版本 != 节点直接结构服务合同版本
            || 请求.写集规则版本 != 1
            || !节点直接事务幂等身份完整(请求.安装实例身份)
            || !节点直接事务幂等身份完整(请求.幂等身份)
            || !摘要非零_(请求.请求意图摘要) || !摘要非零_(请求.执行证据摘要)
            || (!允许空域治理 && (请求.预期事实截止代次 == 0 || 请求.写项组.empty()))) {
            结果.状态 = 节点直接类型化结构数据操作状态::入口拒绝;
            return 结果;
        }
        auto 许可 = 事务域_->取得独占许可();
        if (!许可.有效()) {
            结果.状态 = 节点直接类型化结构数据操作状态::许可拒绝;
            return 结果;
        }
        const auto 当前代次 = 许可.读取已发布代次();
        if ((!允许空域治理 && 当前代次 != 请求.预期事实截止代次)
            || (允许空域治理 && 当前代次 != 0)) {
            结果.状态 = 节点直接类型化结构数据操作状态::版本漂移;
            return 结果;
        }
        const auto 既有 = 幂等_->读取(请求.幂等身份);
        if (既有) {
            if (既有->请求意图摘要 == 请求.请求意图摘要
                && 既有->状态 == 节点直接事务幂等记录状态::已发布) return 形成幂等读回_(*既有);
            结果.状态 = 既有->请求意图摘要 == 请求.请求意图摘要
                ? 节点直接类型化结构数据操作状态::内部不一致
                : 节点直接类型化结构数据操作状态::幂等冲突;
            return 结果;
        }
        const auto 编码 = 编码写集_(请求);
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
        准备请求.材料格式版本 = 1;
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

        // 业务候选施工在后续固定顺序段接续；在形成任何候选前先持久见证本轮安全撤销。
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
