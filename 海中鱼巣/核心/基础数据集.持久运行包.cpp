#include "基础数据集.持久运行包.h"

#include <new>
#include <stdexcept>

namespace 海中鱼巣 {
namespace {

using 状态 = 基础持久状态;

struct 提交资格释放 final {
    std::atomic_flag& 占用;
    ~提交资格释放() { 占用.clear(std::memory_order_release); }
};

bool 复用旧编码(const auto& 前组, const auto& 后组, std::uint64_t 原高水位) {
    for (auto it = 后组.begin(); it != 后组.end() && it->first < 原高水位; ++it)
        if (!前组.contains(it->first)) return true;
    return false;
}

} // namespace

基础持久运行包::基础持久运行包(std::unique_ptr<基础持久存储> 存储, 视图 初始) noexcept
    : 存储_(std::move(存储)), 当前_(std::move(初始)) {}

std::pair<状态, 基础持久运行包::视图> 基础持久运行包::准备(
    std::span<const std::uint8_t> 字节) {
    try {
        auto [解码, 映像] = 解码基础快照(字节);
        if (解码 != 状态::已读取) return {解码, {}};
        if (!映像) return {状态::内部不一致, {}};
        if (!映像->技术材料.empty()) return {状态::格式不支持, {}};

        auto 结构 = std::make_unique<基础数据集>();
        结构->下一个编码_ = 映像->下一个编码;
        for (const auto& 节点 : 映像->节点组)
            结构->节点_.emplace(节点.编码.值, 节点);
        for (auto& 关系 : 映像->外部关系组)
            结构->插入关系已加锁(std::move(关系));
        std::size_t 引用数 = 0;
        for (auto& 字段 : 映像->字段关系组) {
            if (std::holds_alternative<稳定编码>(字段.内容)) ++引用数;
            结构->插入字段已加锁(std::move(字段));
        }
        if (结构->节点_.size() != 映像->节点组.size()
            || 结构->外部关系_.size() != 映像->外部关系组.size()
            || 结构->源关系索引_.size() != 结构->外部关系_.size()
            || 结构->目标关系索引_.size() != 结构->外部关系_.size()
            || 结构->字段关系_.size() != 映像->字段关系组.size()
            || 结构->所属字段索引_.size() != 结构->字段关系_.size()
            || 结构->字段节点索引_.size() != 结构->字段关系_.size()
            || 结构->目标字段索引_.size() != 引用数
            || 结构->入向字段索引_.size() != 引用数) return {状态::内部不一致, {}};

        auto 结果 = std::shared_ptr<基础持久视图>(new 基础持久视图);
        结果->身份_ = 映像->数据集身份;
        结果->下一个编码_ = 映像->下一个编码;
        结果->结构_ = std::move(结构);
        return {状态::已读取, std::move(结果)};
    } catch (const std::bad_alloc&) {
        return {状态::资源失败, {}};
    } catch (const std::length_error&) {
        return {状态::资源失败, {}};
    } catch (const std::logic_error&) {
        return {状态::内部不一致, {}};
    }
}

std::pair<状态, std::unique_ptr<基础持久运行包>> 基础持久运行包::打开(
    const std::filesystem::path& 目录) {
    try {
        auto [打开状态, 存储] = 基础持久存储::打开(目录);
        if (!存储) return {打开状态, nullptr};
        视图 初始;
        if (打开状态 == 状态::已读取) {
            const auto [读取状态, 字节] = 存储->读取();
            if (读取状态 != 状态::已读取) return {读取状态, nullptr};
            auto [准入状态, 候选] = 准备(字节);
            if (准入状态 != 状态::已读取) return {准入状态, nullptr};
            初始 = std::move(候选);
        } else if (打开状态 != 状态::空仓) {
            return {状态::内部不一致, nullptr};
        }
        return {打开状态, std::unique_ptr<基础持久运行包>(
            new 基础持久运行包(std::move(存储), std::move(初始)))};
    } catch (const std::bad_alloc&) {
        return {状态::资源失败, nullptr};
    }
}

状态 基础持久运行包::校验变更(const 基础持久视图& 前, const 基础持久视图& 后) {
    const auto 原高水位 = 前.下一个编码();
    if (前.数据集身份() != 后.数据集身份() || 后.下一个编码() < 原高水位)
        return 状态::结构冲突;
    const auto& A = 前.结构();
    const auto& B = 后.结构();
    if (复用旧编码(A.节点_, B.节点_, 原高水位)
        || 复用旧编码(A.外部关系_, B.外部关系_, 原高水位)
        || 复用旧编码(A.字段关系_, B.字段关系_, 原高水位)) return 状态::结构冲突;
    if (后.下一个编码() == 原高水位 && A.节点_ == B.节点_
        && A.外部关系_ == B.外部关系_ && A.字段关系_ == B.字段关系_) return 状态::无变化;
    return 状态::已读取;
}

状态 基础持久运行包::提交(const 视图& 预期, const 基础快照映像& 候选) {
    if (提交中_.test_and_set(std::memory_order_acquire)) return 状态::提交占用;
    const 提交资格释放 资格{提交中_};
    if (发布未决_) return 状态::发布结果未知;
    const auto 前 = 读取();
    if (前 != 预期) return 状态::前提已变化;

    const auto [编码, 字节] = 编码基础快照(候选);
    if (编码 != 状态::已读取) return 编码;
    auto [准入, 后] = 准备(字节);
    if (准入 != 状态::已读取) return 准入;
    if (前) {
        const auto 变化 = 校验变更(*前, *后);
        if (变化 != 状态::已读取) return 变化;
    }
    const auto 提交结果 = 存储_->提交(字节);
    if (提交结果 != 状态::已提交) {
        发布未决_ = 提交结果 == 状态::发布结果未知;
        return 提交结果;
    }
    当前_.exchange(std::move(后), std::memory_order_acq_rel);
    return 状态::已提交;
}

} // namespace 海中鱼巣
