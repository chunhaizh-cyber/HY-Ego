#include "基础数据集.持久运行包.h"

#define NOMINMAX
#include <Windows.h>
#include <bcrypt.h>

#include <algorithm>
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

void 拷贝权威记录(const auto& 组, auto& 记录) {
    记录.reserve(组.size());
    for (const auto& 项 : 组) 记录.push_back(项.second);
}

} // namespace

基础持久运行包::基础持久运行包(std::unique_ptr<基础持久存储> 存储, 视图 初始)
    : 封存身份_(std::make_shared<const int>(0)), 存储_(std::move(存储)), 当前_(std::move(初始)) {}

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
        建立自键索引(*结果);
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
    return 发表已准备(std::move(后), 字节);
}

状态 基础持久运行包::发表已准备(视图 后, std::span<const std::uint8_t> 字节) {
    const auto 提交结果 = 存储_->提交(字节);
    if (提交结果 != 状态::已提交) {
        发布未决_ = 提交结果 == 状态::发布结果未知;
        return 提交结果;
    }
    当前_.exchange(std::move(后), std::memory_order_acq_rel);
    return 状态::已提交;
}

std::vector<基础字段关系> 基础持久视图::查询自键标记(std::string_view 值) const {
    std::vector<基础字段关系> 结果;
    const auto [起点, 终点] = 自键标记_.equal_range(值);
    for (auto it = 起点; it != 终点; ++it) 结果.push_back(*it->second);
    return 结果;
}

void 基础持久运行包::建立自键索引(基础持久视图& 视图) {
    for (const auto& [编码, 字段] : 视图.结构().字段关系_) {
        if (字段.所属节点 != 字段.字段节点) continue;
        const auto* 直接值 = std::get_if<基础值>(&字段.内容);
        const auto* 字符串 = 直接值 ? std::get_if<std::string>(&直接值->材料) : nullptr;
        if (字符串) 视图.自键标记_.emplace(*字符串, &字段);
    }
}

bool 基础持久运行包::可开始首份(const 基础数据集& 源) {
    const std::shared_lock 锁(源.互斥_);
    return 源.下一个编码_ == 1 && 源.节点_.empty()
        && 源.外部关系_.empty() && 源.字段关系_.empty();
}

std::pair<状态, std::unique_ptr<const 基础持久首份候选>> 基础持久运行包::准备首份(
    const 基础数据集& 源) const {
    if (读取()) return {状态::入口拒绝, nullptr};
    try {
        基础快照映像 映像;
        const auto 随机 = BCryptGenRandom(nullptr, 映像.数据集身份.data(),
            static_cast<ULONG>(映像.数据集身份.size()), BCRYPT_USE_SYSTEM_PREFERRED_RNG);
        if (随机 != 0 || std::ranges::all_of(映像.数据集身份, [](auto 值) { return 值 == 0; }))
            return {状态::资源失败, nullptr};
        {
            const std::shared_lock 锁(源.互斥_);
            if (源.节点_.empty()) return {状态::入口拒绝, nullptr};
            映像.下一个编码 = 源.下一个编码_;
            拷贝权威记录(源.节点_, 映像.节点组);
            拷贝权威记录(源.外部关系_, 映像.外部关系组);
            拷贝权威记录(源.字段关系_, 映像.字段关系组);
        }
        auto [编码, 字节] = 编码基础快照(映像);
        if (编码 != 状态::已读取) return {编码, nullptr};
        auto [准入, 视图] = 准备(字节);
        if (准入 != 状态::已读取) return {准入, nullptr};
        auto 候选 = std::unique_ptr<基础持久首份候选>(new 基础持久首份候选);
        候选->会话身份_ = 封存身份_;
        候选->视图_ = std::move(视图);
        候选->字节_ = std::move(字节);
        return {状态::已读取, std::move(候选)};
    } catch (const std::bad_alloc&) {
        return {状态::资源失败, nullptr};
    } catch (const std::length_error&) {
        return {状态::资源失败, nullptr};
    }
}

状态 基础持久运行包::提交首份(const 基础持久首份候选& 候选) {
    if (提交中_.test_and_set(std::memory_order_acquire)) return 状态::提交占用;
    const 提交资格释放 资格{提交中_};
    if (发布未决_) return 状态::发布结果未知;
    if (读取() || 候选.会话身份_ != 封存身份_) return 状态::入口拒绝;
    return 发表已准备(候选.视图_, 候选.字节_);
}

基础片段封存结果 基础持久运行包::封存追加(const 基础节点组追加请求& 请求) const {
    基础数据集 封存环境;
    封存环境.封存实例身份_ = 封存身份_;
    return 封存环境.封存原子追加片段(请求);
}

基础持久追加结果 基础持久运行包::提交追加(
    const 视图& 预期, const std::vector<基础封存追加片段>& 片段组) {
    using 结构状态 = 基础节点组追加状态;
    if (提交中_.test_and_set(std::memory_order_acquire)) return {状态::提交占用};
    const 提交资格释放 资格{提交中_};
    if (发布未决_) return {状态::发布结果未知};
    const auto 前 = 读取();
    if (前 != 预期) return {状态::前提已变化};
    if (!前) return {状态::入口拒绝};
    try {
        auto 草稿 = std::make_unique<基础数据集>();
        const auto& 原结构 = 前->结构();
        草稿->封存实例身份_ = 封存身份_;
        草稿->下一个编码_ = 原结构.下一个编码_;
        草稿->节点_ = 原结构.节点_;
        草稿->外部关系_ = 原结构.外部关系_;
        草稿->字段关系_ = 原结构.字段关系_;
        草稿->所属字段索引_ = 原结构.所属字段索引_;
        草稿->目标字段索引_ = 原结构.目标字段索引_;
        草稿->入向字段索引_ = 原结构.入向字段索引_;
        草稿->字段节点索引_ = 原结构.字段节点索引_;
        草稿->源关系索引_ = 原结构.源关系索引_;
        草稿->目标关系索引_ = 原结构.目标关系索引_;

        auto 组合 = 草稿->原子追加封存片段组(片段组);
        if (组合.状态 != 结构状态::已完成) {
            const auto 原因 = 组合.状态 == 结构状态::资源失败 ? 状态::资源失败 : 状态::结构拒绝;
            return {原因, 组合.状态};
        }

        auto 后 = std::shared_ptr<基础持久视图>(new 基础持久视图);
        后->身份_ = 前->数据集身份();
        后->下一个编码_ = 草稿->下一个编码_;
        后->结构_ = std::move(草稿);
        const auto 变化 = 校验变更(*前, *后);
        if (变化 == 状态::无变化) return {变化, 组合.状态, std::move(组合.片段回执组)};
        if (变化 != 状态::已读取) return {状态::内部不一致};

        基础快照映像 映像;
        映像.数据集身份 = 后->数据集身份();
        映像.下一个编码 = 后->下一个编码();
        拷贝权威记录(后->结构().节点_, 映像.节点组);
        拷贝权威记录(后->结构().外部关系_, 映像.外部关系组);
        拷贝权威记录(后->结构().字段关系_, 映像.字段关系组);
        const auto [编码, 字节] = 编码基础快照(映像);
        if (编码 != 状态::已读取)
            return {编码 == 状态::资源失败 ? 编码 : 状态::内部不一致};
        建立自键索引(*后);
        const auto 提交状态 = 发表已准备(std::move(后), 字节);
        if (提交状态 != 状态::已提交) return {提交状态};
        return {提交状态, 组合.状态, std::move(组合.片段回执组)};
    } catch (const std::bad_alloc&) {
        return {状态::资源失败};
    } catch (const std::length_error&) {
        return {状态::资源失败};
    }
}

} // namespace 海中鱼巣
