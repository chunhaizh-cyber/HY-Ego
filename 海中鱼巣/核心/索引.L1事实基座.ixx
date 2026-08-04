module;

#include <algorithm>
#include <atomic>
#include <cstdint>
#include <limits>
#include <map>
#include <memory>
#include <mutex>
#include <new>
#include <shared_mutex>
#include <type_traits>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

export module 海中鱼巣.核心.索引.L1事实基座;

export import 海中鱼巣.核心.合同.L1可重建索引;
import 海中鱼巣.核心.服务.L1事实基座;

export namespace 海中鱼巣 {

class L1事实可重建索引 final {
public:
    explicit L1事实可重建索引(L1事实基座服务& L1) noexcept : L1_(L1) {}
    L1事实可重建索引(const L1事实可重建索引&) = delete;
    L1事实可重建索引& operator=(const L1事实可重建索引&) = delete;

    L1索引维护结果 从指定快照确定重建(const L1索引重建请求& 请求) {
        try {
            if (!重建请求有效(请求)) return {};
            std::uint64_t 开始序号 = 0;
            {
                std::shared_lock<std::shared_mutex> 锁(锁_);
                开始序号 = 视图序号_;
            }
            const auto 第一次 = L1_.读取完整快照({});
            if (第一次.状态 == L1读取状态::资源失败)
                return {L1索引维护状态::资源失败, 请求.来源事实代次, 请求.规则版本, 开始序号};
            if (第一次.状态 != L1读取状态::成功 || !第一次.快照)
                return {L1索引维护状态::内部不一致, 请求.来源事实代次, 请求.规则版本, 开始序号};
            if (!快照相同(*第一次.快照, 请求.指定快照))
                return {L1索引维护状态::事实代次漂移, 请求.来源事实代次, 请求.规则版本, 开始序号};

            auto 候选 = 构造候选(请求.指定快照, 请求.规则版本);
#ifdef _DEBUG
            if (损坏下一候选_.exchange(false)) 候选.关系源[{0, 1}].push_back({1});
#endif
            if (!候选完整(候选, 请求.指定快照))
                return {L1索引维护状态::内部不一致, 请求.来源事实代次, 请求.规则版本, 开始序号};

            const auto 第二次 = L1_.读取完整快照({});
            if (第二次.状态 == L1读取状态::资源失败)
                return {L1索引维护状态::资源失败, 请求.来源事实代次, 请求.规则版本, 开始序号};
            if (第二次.状态 != L1读取状态::成功 || !第二次.快照)
                return {L1索引维护状态::内部不一致, 请求.来源事实代次, 请求.规则版本, 开始序号};
            if (!快照相同(*第二次.快照, 请求.指定快照)
                || !快照相同(*第二次.快照, *第一次.快照))
                return {L1索引维护状态::事实代次漂移, 请求.来源事实代次, 请求.规则版本, 开始序号};
            if (开始序号 == (std::numeric_limits<std::uint64_t>::max)())
                return {L1索引维护状态::内部不一致, 请求.来源事实代次, 请求.规则版本, 开始序号};
            候选.视图序号 = 开始序号 + 1;
            std::unique_ptr<const L1索引视图> 不可变候选 = std::make_unique<L1索引视图>(std::move(候选));

            std::unique_lock<std::shared_mutex> 锁(锁_);
            if (视图_ && 内容相同(*视图_, *不可变候选))
                return {L1索引维护状态::精确重复, 视图_->来源事实代次, 视图_->规则版本, 视图_->视图序号};
            if (视图序号_ != 开始序号) {
                const auto 来源 = 视图_ ? 视图_->来源事实代次 : 0;
                const auto 规则 = 视图_ ? 视图_->规则版本 : 0;
                return {L1索引维护状态::索引视图漂移, 来源, 规则, 视图序号_};
            }
            视图_.swap(不可变候选);
            视图序号_ = 开始序号 + 1;
            return {L1索引维护状态::重建已发布, 请求.来源事实代次, 请求.规则版本, 视图序号_};
        } catch (const std::bad_alloc&) { return {L1索引维护状态::资源失败, 0, 0, 0}; }
        catch (...) { return {L1索引维护状态::内部不一致, 0, 0, 0}; }
    }

    L1索引维护结果 清空(const L1索引清空请求& 请求) noexcept {
        try {
            if (请求.合同版本 != L1可重建索引合同版本) return {};
            std::unique_lock<std::shared_mutex> 锁(锁_);
            if (!视图_) return {L1索引维护状态::精确重复, 0, 0, 视图序号_};
            const auto 来源 = 视图_->来源事实代次;
            const auto 规则 = 视图_->规则版本;
            if (视图序号_ == (std::numeric_limits<std::uint64_t>::max)())
                return {L1索引维护状态::内部不一致, 来源, 规则, 视图序号_};
            视图_.reset();
            ++视图序号_;
            return {L1索引维护状态::已清空, 来源, 规则, 视图序号_};
        } catch (...) { return {L1索引维护状态::内部不一致, 0, 0, 0}; }
    }

    L1索引维护结果 失效(const L1索引失效请求& 请求) noexcept {
        try {
            if (请求.合同版本 != L1可重建索引合同版本
                || 请求.规则版本 != L1可重建索引规则版本) return {};
            std::unique_lock<std::shared_mutex> 锁(锁_);
            if (!视图_) return {L1索引维护状态::精确重复, 0, 0, 视图序号_};
            if (视图_->来源事实代次 != 请求.被失效来源事实代次
                || 视图_->规则版本 != 请求.规则版本)
                return {L1索引维护状态::索引视图漂移, 视图_->来源事实代次, 视图_->规则版本, 视图序号_};
            if (视图序号_ == (std::numeric_limits<std::uint64_t>::max)())
                return {L1索引维护状态::内部不一致, 视图_->来源事实代次, 视图_->规则版本, 视图序号_};
            const auto 来源 = 视图_->来源事实代次;
            const auto 规则 = 视图_->规则版本;
            视图_.reset();
            ++视图序号_;
            return {L1索引维护状态::已失效, 来源, 规则, 视图序号_};
        } catch (...) { return {L1索引维护状态::内部不一致, 0, 0, 0}; }
    }

    L1索引读取结果 查询并回读当前事实(const L1索引查询请求& 请求) {
        try {
            if (请求.合同版本 != L1可重建索引合同版本
                || 请求.规则版本 != L1可重建索引规则版本 || !键完整(请求.键)) return {};
            std::uint64_t 来源 = 0;
            std::uint64_t 捕获序号 = 0;
            std::uint32_t 捕获合同版本 = 0;
            std::uint32_t 捕获数据版本 = 0;
            std::vector<稳定编码> 候选;
            {
                std::shared_lock<std::shared_mutex> 锁(锁_);
                if (!视图_ || 视图_->合同版本 != L1可重建索引合同版本
                    || 视图_->数据版本 != 索引数据版本_
                    || 视图_->规则版本 != 请求.规则版本)
                    return {L1索引读取状态::索引不可用, 0, 0, {}};
                来源 = 视图_->来源事实代次;
                捕获序号 = 视图_->视图序号;
                捕获合同版本 = 视图_->合同版本;
                捕获数据版本 = 视图_->数据版本;
                候选 = 取得候选(*视图_, 请求.键);
            }
            std::vector<L1事实副本> 事实组;
            事实组.reserve(候选.size());
            for (const auto 编码 : 候选) {
                const auto 读取 = 读取一个(请求.键, 编码);
                if (读取.状态 == L1读取状态::资源失败)
                    return {L1索引读取状态::资源失败, 来源, 请求.规则版本, {}};
                if (读取.状态 == L1读取状态::未找到 || 读取.状态 == L1读取状态::已退出
                    || 读取.状态 == L1读取状态::事实代次漂移 || 读取.读取事实代次 != 来源) {
                    if (!条件失效(捕获序号, 来源, 请求.规则版本))
                        return {L1索引读取状态::内部不一致, 来源, 请求.规则版本, {}};
                    return {L1索引读取状态::索引不可用, 来源, 请求.规则版本, {}};
                }
                if (读取.状态 != L1读取状态::成功 || !读取.事实
                    || !事实匹配键(*读取.事实, 请求.键, 编码)) {
                    if (读取.状态 == L1读取状态::成功 && 读取.事实)
                        (void)条件失效(捕获序号, 来源, 请求.规则版本);
                    return {L1索引读取状态::内部不一致, 来源, 请求.规则版本, {}};
                }
                事实组.push_back(*读取.事实);
            }
            const auto 末次 = L1_.读取完整快照({});
            if (末次.状态 == L1读取状态::资源失败)
                return {L1索引读取状态::资源失败, 来源, 请求.规则版本, {}};
            if (末次.状态 != L1读取状态::成功 || !末次.快照)
                return {L1索引读取状态::内部不一致, 来源, 请求.规则版本, {}};
            if (末次.快照->事实代次 != 来源) {
                if (!条件失效(捕获序号, 来源, 请求.规则版本))
                    return {L1索引读取状态::内部不一致, 来源, 请求.规则版本, {}};
                return {L1索引读取状态::索引不可用, 来源, 请求.规则版本, {}};
            }
            {
                std::shared_lock<std::shared_mutex> 锁(锁_);
                if (!视图_ || 视图_->视图序号 != 捕获序号
                    || 视图_->合同版本 != 捕获合同版本 || 视图_->数据版本 != 捕获数据版本
                    || 视图_->来源事实代次 != 来源 || 视图_->规则版本 != 请求.规则版本)
                    return {L1索引读取状态::索引不可用, 来源, 请求.规则版本, {}};
            }
            return {事实组.empty() ? L1索引读取状态::确定未命中 : L1索引读取状态::命中,
                来源, 请求.规则版本, std::move(事实组)};
        } catch (const std::bad_alloc&) { return {L1索引读取状态::资源失败, 0, 0, {}}; }
        catch (...) { return {L1索引读取状态::内部不一致, 0, 0, {}}; }
    }

#ifdef _DEBUG
    void 自检_损坏下一次重建候选() noexcept { 损坏下一候选_.store(true); }
#endif

private:
    static constexpr std::uint32_t 索引数据版本_ = 1;
    using 双编码键 = std::pair<std::uint64_t, std::uint64_t>;
    struct L1索引视图 final {
        std::uint32_t 合同版本 = L1可重建索引合同版本;
        std::uint32_t 数据版本 = 索引数据版本_;
        std::uint64_t 来源事实代次 = 0;
        std::uint32_t 规则版本 = L1可重建索引规则版本;
        std::uint64_t 视图序号 = 0;
        std::map<std::uint8_t, std::vector<稳定编码>> 节点种类;
        std::map<双编码键, std::vector<稳定编码>> 关系源;
        std::map<双编码键, std::vector<稳定编码>> 关系目标;
        std::map<双编码键, std::vector<稳定编码>> 当前属性值;
    };

    static bool 节点种类有效(节点种类 种类) noexcept {
        return 种类 == 节点种类::普通 || 种类 == 节点种类::属性类型;
    }
    static bool 原始值相同(const 原始值材料& 左, const 原始值材料& 右) noexcept {
        if (左.index() != 右.index()) return false;
        switch (左.index()) {
        case 0: return std::get<std::int64_t>(左) == std::get<std::int64_t>(右);
        case 1: return std::get<std::vector<std::int64_t>>(左) == std::get<std::vector<std::int64_t>>(右);
        case 2: return std::get<std::vector<std::uint64_t>>(左) == std::get<std::vector<std::uint64_t>>(右);
        case 3: return std::get<独立材料引用>(左).编码 == std::get<独立材料引用>(右).编码;
        default: return false;
        }
    }
    static bool 节点相同(const 节点事实& 左, const 节点事实& 右) noexcept {
        return 左.编码 == 右.编码 && 左.种类 == 右.种类
            && 左.属性类型表示.has_value() == 右.属性类型表示.has_value()
            && (!左.属性类型表示 || *左.属性类型表示 == *右.属性类型表示)
            && 左.创建事实代次 == 右.创建事实代次
            && 左.退出事实代次.has_value() == 右.退出事实代次.has_value()
            && (!左.退出事实代次 || *左.退出事实代次 == *右.退出事实代次)
            && 左.当前属性 == 右.当前属性;
    }
    static bool 关系相同(const 关系事实& 左, const 关系事实& 右) noexcept {
        return 左.编码 == 右.编码 && 左.源节点 == 右.源节点 && 左.目标节点 == 右.目标节点
            && 左.关系类型节点 == 右.关系类型节点 && 左.角色或顺序 == 右.角色或顺序
            && 左.创建事实代次 == 右.创建事实代次
            && 左.退出事实代次.has_value() == 右.退出事实代次.has_value()
            && (!左.退出事实代次 || *左.退出事实代次 == *右.退出事实代次);
    }
    static bool 值相同(const 值事实& 左, const 值事实& 右) noexcept {
        return 左.编码 == 右.编码 && 左.所属节点 == 右.所属节点
            && 左.属性类型节点 == 右.属性类型节点 && 原始值相同(左.材料, 右.材料)
            && 左.来源节点 == 右.来源节点 && 左.创建事实代次 == 右.创建事实代次
            && 左.退出事实代次.has_value() == 右.退出事实代次.has_value()
            && (!左.退出事实代次 || *左.退出事实代次 == *右.退出事实代次);
    }
    static bool 快照相同(const L1完整快照& 左, const L1完整快照& 右) noexcept {
        if (左.事实代次 != 右.事实代次 || 左.当前节点.size() != 右.当前节点.size()
            || 左.当前关系.size() != 右.当前关系.size() || 左.当前值.size() != 右.当前值.size()
            || 左.永久占用编码 != 右.永久占用编码) return false;
        for (std::size_t i = 0; i < 左.当前节点.size(); ++i)
            if (!节点相同(左.当前节点[i], 右.当前节点[i])) return false;
        for (std::size_t i = 0; i < 左.当前关系.size(); ++i)
            if (!关系相同(左.当前关系[i], 右.当前关系[i])) return false;
        for (std::size_t i = 0; i < 左.当前值.size(); ++i)
            if (!值相同(左.当前值[i], 右.当前值[i])) return false;
        return true;
    }
    static bool 键完整(const L1结构索引键& 键) noexcept {
        return std::visit([](const auto& 值) noexcept {
            using T = std::decay_t<decltype(值)>;
            if constexpr (std::is_same_v<T, L1节点种类索引键>) return 节点种类有效(值.种类);
            else if constexpr (std::is_same_v<T, L1关系源索引键>) return 有效(值.源节点) && 有效(值.关系类型节点);
            else if constexpr (std::is_same_v<T, L1关系目标索引键>) return 有效(值.目标节点) && 有效(值.关系类型节点);
            else return 有效(值.所属节点) && 有效(值.属性类型节点);
        }, 键);
    }
    static bool 重建请求有效(const L1索引重建请求& 请求) noexcept {
        if (请求.合同版本 != L1可重建索引合同版本
            || 请求.规则版本 != L1可重建索引规则版本
            || 请求.来源事实代次 != 请求.指定快照.事实代次) return false;
        if (请求.来源事实代次 != 0) return true;
        return 请求.指定快照.当前节点.empty() && 请求.指定快照.当前关系.empty()
            && 请求.指定快照.当前值.empty() && 请求.指定快照.永久占用编码.empty();
    }
    static void 排序映射(auto& 映射) {
        for (auto& [_, 组] : 映射) std::sort(组.begin(), 组.end());
    }
    static L1索引视图 构造候选(const L1完整快照& 快照, std::uint32_t 规则版本) {
        L1索引视图 候选;
        候选.合同版本 = L1可重建索引合同版本;
        候选.数据版本 = 索引数据版本_;
        候选.来源事实代次 = 快照.事实代次;
        候选.规则版本 = 规则版本;
        for (const auto& 节点 : 快照.当前节点)
            候选.节点种类[static_cast<std::uint8_t>(节点.种类)].push_back(节点.编码);
        for (const auto& 关系 : 快照.当前关系) {
            候选.关系源[{关系.源节点.值, 关系.关系类型节点.值}].push_back(关系.编码);
            候选.关系目标[{关系.目标节点.值, 关系.关系类型节点.值}].push_back(关系.编码);
        }
        for (const auto& 值 : 快照.当前值)
            候选.当前属性值[{值.所属节点.值, 值.属性类型节点.值}].push_back(值.编码);
        排序映射(候选.节点种类); 排序映射(候选.关系源);
        排序映射(候选.关系目标); 排序映射(候选.当前属性值);
        return 候选;
    }
    static bool 组完整(const auto& 映射) noexcept {
        for (const auto& [_, 组] : 映射) {
            if (组.empty()) return false;
            for (std::size_t i = 0; i < 组.size(); ++i)
                if (!有效(组[i]) || (i != 0 && !(组[i - 1] < 组[i]))) return false;
        }
        return true;
    }
    static bool 节点种类映射键完整(
        const std::map<std::uint8_t, std::vector<稳定编码>>& 映射) noexcept {
        for (const auto& [键, _] : 映射)
            if (!节点种类有效(static_cast<节点种类>(键))) return false;
        return true;
    }
    static bool 双编码映射键完整(const auto& 映射) noexcept {
        for (const auto& [键, _] : 映射)
            if (键.first == 0 || 键.second == 0) return false;
        return true;
    }
    static bool 内容相同(const L1索引视图& 左, const L1索引视图& 右) noexcept {
        return 左.合同版本 == 右.合同版本 && 左.数据版本 == 右.数据版本
            && 左.来源事实代次 == 右.来源事实代次 && 左.规则版本 == 右.规则版本
            && 左.节点种类 == 右.节点种类 && 左.关系源 == 右.关系源
            && 左.关系目标 == 右.关系目标 && 左.当前属性值 == 右.当前属性值;
    }
    static bool 候选完整(const L1索引视图& 候选, const L1完整快照& 快照) {
        if (候选.合同版本 != L1可重建索引合同版本 || 候选.数据版本 != 索引数据版本_
            || 候选.来源事实代次 != 快照.事实代次 || 候选.规则版本 != L1可重建索引规则版本
            || !组完整(候选.节点种类) || !组完整(候选.关系源)
            || !组完整(候选.关系目标) || !组完整(候选.当前属性值)
            || !节点种类映射键完整(候选.节点种类)
            || !双编码映射键完整(候选.关系源)
            || !双编码映射键完整(候选.关系目标)
            || !双编码映射键完整(候选.当前属性值)) return false;
        std::unordered_set<std::uint64_t> 编码组;
        for (const auto& 节点 : 快照.当前节点) {
            if (!有效(节点.编码) || 节点.编码.值 == 0 || !节点种类有效(节点.种类)
                || !编码组.insert(节点.编码.值).second) return false;
            for (const auto& 槽 : 节点.当前属性)
                if (!有效(槽.属性类型节点) || !有效(槽.当前值)) return false;
        }
        for (const auto& 关系 : 快照.当前关系)
            if (!有效(关系.编码) || !有效(关系.源节点) || !有效(关系.目标节点)
                || !有效(关系.关系类型节点) || !编码组.insert(关系.编码.值).second) return false;
        for (const auto& 值 : 快照.当前值)
            if (!有效(值.编码) || !有效(值.所属节点) || !有效(值.属性类型节点)
                || !有效(值.来源节点) || !编码组.insert(值.编码.值).second) return false;
        const auto 期望 = 构造候选(快照, 候选.规则版本);
        return 内容相同(候选, 期望);
    }
    static std::vector<稳定编码> 取得候选(const L1索引视图& 视图, const L1结构索引键& 键) {
        return std::visit([&](const auto& 值) -> std::vector<稳定编码> {
            using T = std::decay_t<decltype(值)>;
            if constexpr (std::is_same_v<T, L1节点种类索引键>) {
                const auto it = 视图.节点种类.find(static_cast<std::uint8_t>(值.种类));
                return it == 视图.节点种类.end() ? std::vector<稳定编码>{} : it->second;
            } else if constexpr (std::is_same_v<T, L1关系源索引键>) {
                const auto it = 视图.关系源.find({值.源节点.值, 值.关系类型节点.值});
                return it == 视图.关系源.end() ? std::vector<稳定编码>{} : it->second;
            } else if constexpr (std::is_same_v<T, L1关系目标索引键>) {
                const auto it = 视图.关系目标.find({值.目标节点.值, 值.关系类型节点.值});
                return it == 视图.关系目标.end() ? std::vector<稳定编码>{} : it->second;
            } else {
                const auto it = 视图.当前属性值.find({值.所属节点.值, 值.属性类型节点.值});
                return it == 视图.当前属性值.end() ? std::vector<稳定编码>{} : it->second;
            }
        }, 键);
    }
    L1读取结果 读取一个(const L1结构索引键& 键, 稳定编码 编码) {
        return std::visit([&](const auto& 值) {
            using T = std::decay_t<decltype(值)>;
            if constexpr (std::is_same_v<T, L1节点种类索引键>) return L1_.读取当前节点({L1事实基座合同版本, 编码});
            else if constexpr (std::is_same_v<T, L1关系源索引键> || std::is_same_v<T, L1关系目标索引键>)
                return L1_.读取当前关系({L1事实基座合同版本, 编码});
            else return L1_.读取当前值({L1事实基座合同版本, 编码});
        }, 键);
    }
    static bool 事实匹配键(const L1事实副本& 事实, const L1结构索引键& 键, 稳定编码 编码) noexcept {
        return std::visit([&](const auto& 值) noexcept {
            using T = std::decay_t<decltype(值)>;
            if constexpr (std::is_same_v<T, L1节点种类索引键>) {
                const auto* 节点 = std::get_if<节点事实>(&事实);
                return 节点 && 节点->编码 == 编码 && 节点->种类 == 值.种类;
            } else if constexpr (std::is_same_v<T, L1关系源索引键>) {
                const auto* 关系 = std::get_if<关系事实>(&事实);
                return 关系 && 关系->编码 == 编码 && 关系->源节点 == 值.源节点
                    && 关系->关系类型节点 == 值.关系类型节点;
            } else if constexpr (std::is_same_v<T, L1关系目标索引键>) {
                const auto* 关系 = std::get_if<关系事实>(&事实);
                return 关系 && 关系->编码 == 编码 && 关系->目标节点 == 值.目标节点
                    && 关系->关系类型节点 == 值.关系类型节点;
            } else {
                const auto* 当前值 = std::get_if<值事实>(&事实);
                return 当前值 && 当前值->编码 == 编码 && 当前值->所属节点 == 值.所属节点
                    && 当前值->属性类型节点 == 值.属性类型节点;
            }
        }, 键);
    }
    bool 条件失效(std::uint64_t 序号, std::uint64_t 来源, std::uint32_t 规则) noexcept {
        try {
            std::unique_lock<std::shared_mutex> 锁(锁_);
            if (!视图_ || 视图_->视图序号 != 序号 || 视图_->来源事实代次 != 来源
                || 视图_->规则版本 != 规则) return true;
            if (视图序号_ == (std::numeric_limits<std::uint64_t>::max)()) return false;
            视图_.reset();
            ++视图序号_;
            return true;
        } catch (...) { return false; }
    }

    L1事实基座服务& L1_;
    mutable std::shared_mutex 锁_;
    std::unique_ptr<const L1索引视图> 视图_;
    std::uint64_t 视图序号_ = 0;
#ifdef _DEBUG
    std::atomic_bool 损坏下一候选_ = false;
#endif
};

} // namespace 海中鱼巣
