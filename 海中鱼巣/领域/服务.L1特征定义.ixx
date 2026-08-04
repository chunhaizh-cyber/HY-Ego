module;

#include <algorithm>
#include <array>
#include <cstdint>
#include <mutex>
#include <new>
#include <optional>
#include <type_traits>
#include <variant>

export module 海中鱼巣.领域.服务.L1特征定义;

export import 海中鱼巣.核心.合同.L1公共事实;
import 海中鱼巣.核心.服务.L1事实基座;

export {
#include "L1特征定义.数据.h"
}

export namespace 海中鱼巣 {

namespace L1特征定义内部 {

inline constexpr std::uint64_t 登记幂等域 = 0x03ULL;
inline constexpr std::uint64_t 定义幂等域 = 0x04ULL;

写集幂等键 映射幂等键(std::uint64_t 域, 特征操作幂等身份 身份) noexcept {
    return {(域 << 56) | 身份.值};
}

std::optional<稳定编码> 查找映射(
    const L1写入结果& 结果, std::uint32_t 本地键) noexcept {
    std::optional<稳定编码> 找到;
    for (const auto& [键, 编码] : 结果.新编码映射) {
        if (键.值 != 本地键) continue;
        if (找到.has_value()) return std::nullopt;
        找到 = 编码;
    }
    return 找到;
}

bool 映射唯一(const L1写入结果& 结果) noexcept {
    for (std::size_t i = 0; i < 结果.新编码映射.size(); ++i) {
        if (!有效(结果.新编码映射[i].first)
            || !有效(结果.新编码映射[i].second)) return false;
        for (std::size_t j = i + 1; j < 结果.新编码映射.size(); ++j) {
            if (结果.新编码映射[i].first == 结果.新编码映射[j].first
                || 结果.新编码映射[i].second == 结果.新编码映射[j].second) {
                return false;
            }
        }
    }
    return true;
}

特征定义状态 映射写入状态(L1写入状态 状态) noexcept {
    switch (状态) {
    case L1写入状态::成功: return 特征定义状态::已提交;
    case L1写入状态::精确重复: return 特征定义状态::幂等读回;
    case L1写入状态::幂等冲突: return 特征定义状态::幂等冲突;
    case L1写入状态::事实代次漂移: return 特征定义状态::事实代次漂移;
    case L1写入状态::资源失败: return 特征定义状态::资源失败;
    case L1写入状态::内部不一致: return 特征定义状态::内部不一致;
    default: return 特征定义状态::入口拒绝;
    }
}

enum class 读取判定 : std::uint8_t { 成功, 未找到, 代次漂移, 资源失败, 内部不一致 };

读取判定 映射读取状态(L1读取状态 状态) noexcept {
    switch (状态) {
    case L1读取状态::未找到:
    case L1读取状态::已退出:
    case L1读取状态::属性未设置: return 读取判定::未找到;
    case L1读取状态::事实代次漂移: return 读取判定::代次漂移;
    case L1读取状态::资源失败: return 读取判定::资源失败;
    case L1读取状态::内部不一致: return 读取判定::内部不一致;
    default: return 读取判定::内部不一致;
    }
}

} // namespace L1特征定义内部

class L1特征定义服务 final {
public:
    explicit L1特征定义服务(L1事实基座服务& L1) noexcept : L1_(L1) {}
    L1特征定义服务(const L1特征定义服务&) = delete;
    L1特征定义服务& operator=(const L1特征定义服务&) = delete;

    特征定义登记结果 建立登记(const 特征定义登记请求& 请求) {
        std::lock_guard<std::mutex> 锁(锁_);
        try {
            if (!特征定义登记请求有效(请求)) return {};
            if (首次登记请求_.has_value()) {
                if (*首次登记请求_ == 请求) {
                    return {特征定义状态::幂等读回, 登记_};
                }
                return {特征定义状态::幂等冲突, std::nullopt};
            }

            const auto 初始 = L1_.读取完整快照({});
            if (初始.状态 != L1读取状态::成功 || !初始.快照.has_value()) {
                return {映射读取结果(初始.状态), std::nullopt};
            }
            if (初始.快照->事实代次 != 请求.期望事实代次
                || !初始.快照->当前节点.empty()
                || !初始.快照->当前关系.empty()
                || !初始.快照->当前值.empty()
                || !初始.快照->永久占用编码.empty()) {
                return {};
            }

            L1写集请求 写集;
            写集.期望事实代次 = 初始.快照->事实代次;
            写集.幂等键 = L1特征定义内部::映射幂等键(
                L1特征定义内部::登记幂等域, 请求.幂等身份);
            写集.节点 = {
                {{1}, 节点种类::普通, std::nullopt},
                {{2}, 节点种类::属性类型, 值表示种类::I64},
                {{3}, 节点种类::属性类型, 值表示种类::I64},
                {{4}, 节点种类::属性类型, 值表示种类::I64}};
            const auto 写入 = L1_.提交写集(写集);
            const auto 映射状态 = L1特征定义内部::映射写入状态(写入.状态);
            if (映射状态 != 特征定义状态::已提交
                && 映射状态 != 特征定义状态::幂等读回) {
                return {映射状态, std::nullopt};
            }
            if (写入.事实代次 == 0 || 写入.新编码映射.size() != 4
                || !L1特征定义内部::映射唯一(写入)) {
                return {特征定义状态::内部不一致, std::nullopt};
            }
            const auto 服务 = L1特征定义内部::查找映射(写入, 1);
            const auto 标记 = L1特征定义内部::查找映射(写入, 2);
            const auto 下界 = L1特征定义内部::查找映射(写入, 3);
            const auto 上界 = L1特征定义内部::查找映射(写入, 4);
            if (!服务 || !标记 || !下界 || !上界) {
                return {特征定义状态::内部不一致, std::nullopt};
            }

            std::uint64_t 代次 = 0;
            const auto 服务节点 = 读取节点(*服务, 代次);
            const auto 标记节点 = 读取节点(*标记, 代次);
            const auto 下界节点 = 读取节点(*下界, 代次);
            const auto 上界节点 = 读取节点(*上界, 代次);
            if (!服务节点 || !标记节点 || !下界节点 || !上界节点
                || 代次 != 写入.事实代次
                || 服务节点->种类 != 节点种类::普通
                || 标记节点->种类 != 节点种类::属性类型
                || 下界节点->种类 != 节点种类::属性类型
                || 上界节点->种类 != 节点种类::属性类型
                || 标记节点->属性类型表示 != std::optional{值表示种类::I64}
                || 下界节点->属性类型表示 != std::optional{值表示种类::I64}
                || 上界节点->属性类型表示 != std::optional{值表示种类::I64}) {
                return {特征定义状态::内部不一致, std::nullopt};
            }

            特征定义登记 新登记;
            新登记.事实截止代次 = 写入.事实代次;
            新登记.服务身份 = *服务;
            新登记.定义标记属性类型 = *标记;
            新登记.I64下界属性类型 = *下界;
            新登记.I64上界属性类型 = *上界;
            if (!特征定义登记完整(新登记)) {
                return {特征定义状态::内部不一致, std::nullopt};
            }
            首次登记请求_ = 请求;
            登记_ = 新登记;
            return {写入.状态 == L1写入状态::精确重复
                    ? 特征定义状态::幂等读回 : 特征定义状态::已提交,
                登记_};
        } catch (const std::bad_alloc&) {
            return {特征定义状态::资源失败, std::nullopt};
        } catch (...) {
            return {特征定义状态::内部不一致, std::nullopt};
        }
    }

    特征定义登记结果 读取当前登记() const {
        std::lock_guard<std::mutex> 锁(锁_);
        if (!登记_.has_value()) return {特征定义状态::登记未加载, std::nullopt};
        return {特征定义状态::已读取, 登记_};
    }

    I64特征定义结果 建立I64特征定义(const I64特征定义建立请求& 请求) {
        std::lock_guard<std::mutex> 锁(锁_);
        try {
            if (!I64特征定义建立请求有效(请求)) return {};
            if (!登记_.has_value()) {
                return {特征定义状态::登记未加载, std::nullopt, {}, {}, {}};
            }
            if (定义请求_.has_value()) {
                if (*定义请求_ != 请求) {
                    return {特征定义状态::幂等冲突, std::nullopt, {}, {}, {}};
                }
                auto 读回 = 读取I64特征定义内部(定义编码_[0], 定义编码_[1],
                    定义编码_[2], 定义编码_[3]);
                if (读回.状态 == 特征定义状态::已读取) {
                    读回.状态 = 特征定义状态::幂等读回;
                }
                return 读回;
            }
            if (请求.期望事实代次 != 登记_->事实截止代次) {
                return {特征定义状态::事实代次漂移, std::nullopt, {}, {}, {}};
            }

            L1写集请求 写集;
            写集.期望事实代次 = 请求.期望事实代次;
            写集.幂等键 = L1特征定义内部::映射幂等键(
                L1特征定义内部::定义幂等域, 请求.幂等身份);
            写集.节点 = {{{1}, 节点种类::普通, std::nullopt}};
            写集.值 = {
                {{2}, 写集本地键{1}, 登记_->定义标记属性类型,
                    std::int64_t{1}, 登记_->服务身份},
                {{3}, 写集本地键{1}, 登记_->I64下界属性类型,
                    请求.下界, 登记_->服务身份},
                {{4}, 写集本地键{1}, 登记_->I64上界属性类型,
                    请求.上界, 登记_->服务身份}};
            写集.属性槽变更 = {
                {写集本地键{1}, 登记_->定义标记属性类型, 写集本地键{2}},
                {写集本地键{1}, 登记_->I64下界属性类型, 写集本地键{3}},
                {写集本地键{1}, 登记_->I64上界属性类型, 写集本地键{4}}};
            const auto 写入 = L1_.提交写集(写集);
            const auto 映射状态 = L1特征定义内部::映射写入状态(写入.状态);
            if (映射状态 != 特征定义状态::已提交
                && 映射状态 != 特征定义状态::幂等读回) {
                return {映射状态, std::nullopt, {}, {}, {}};
            }
            if (写入.事实代次 == 0 || 写入.新编码映射.size() != 4
                || !L1特征定义内部::映射唯一(写入)) {
                return {特征定义状态::内部不一致, std::nullopt, {}, {}, {}};
            }
            const auto 定义 = L1特征定义内部::查找映射(写入, 1);
            const auto 标记 = L1特征定义内部::查找映射(写入, 2);
            const auto 下界 = L1特征定义内部::查找映射(写入, 3);
            const auto 上界 = L1特征定义内部::查找映射(写入, 4);
            if (!定义 || !标记 || !下界 || !上界) {
                return {特征定义状态::内部不一致, std::nullopt, {}, {}, {}};
            }
            const auto 读回 = 读取I64特征定义内部(*定义, *标记, *下界, *上界);
            if (读回.状态 != 特征定义状态::已读取) {
                return {读回.状态, std::nullopt, {}, {}, {}};
            }
            if (!读回.事实.has_value() || 读回.事实->事实截止代次 != 写入.事实代次) {
                return {特征定义状态::内部不一致, std::nullopt, {}, {}, {}};
            }
            定义请求_ = 请求;
            定义编码_ = {*定义, *标记, *下界, *上界};
            定义事实_ = 读回.事实;
            auto 结果 = 读回;
            结果.状态 = 写入.状态 == L1写入状态::精确重复
                ? 特征定义状态::幂等读回 : 特征定义状态::已提交;
            return 结果;
        } catch (const std::bad_alloc&) {
            return {特征定义状态::资源失败, std::nullopt, {}, {}, {}};
        } catch (...) {
            return {特征定义状态::内部不一致, std::nullopt, {}, {}, {}};
        }
    }

    I64特征定义结果 读取I64特征定义(const I64特征定义读取请求& 请求) const {
        std::lock_guard<std::mutex> 锁(锁_);
        try {
            if (请求.合同版本 != L1特征定义合同版本
                || 请求.规则版本 != L1特征定义规则版本
                || !有效(请求.特征定义) || !有效(请求.定义标记值)
                || !有效(请求.下界值) || !有效(请求.上界值)) return {};
            if (!登记_.has_value()) {
                return {特征定义状态::登记未加载, std::nullopt, {}, {}, {}};
            }
            return 读取I64特征定义内部(请求.特征定义, 请求.定义标记值,
                请求.下界值, 请求.上界值);
        } catch (const std::bad_alloc&) {
            return {特征定义状态::资源失败, std::nullopt, {}, {}, {}};
        } catch (...) {
            return {特征定义状态::内部不一致, std::nullopt, {}, {}, {}};
        }
    }

private:
    static 特征定义状态 映射读取结果(L1读取状态 状态) noexcept {
        switch (状态) {
        case L1读取状态::资源失败: return 特征定义状态::资源失败;
        case L1读取状态::内部不一致: return 特征定义状态::内部不一致;
        case L1读取状态::未找到:
        case L1读取状态::已退出:
        case L1读取状态::属性未设置: return 特征定义状态::未找到;
        default: return 特征定义状态::内部不一致;
        }
    }

    std::optional<节点事实> 读取节点(稳定编码 编码, std::uint64_t& 代次,
        特征定义状态* 失败 = nullptr) const {
        const auto 结果 = L1_.读取当前节点({L1事实基座合同版本, 编码});
        if (结果.状态 != L1读取状态::成功 || !结果.事实.has_value()) {
            if (失败) *失败 = 映射读取结果(结果.状态);
            return std::nullopt;
        }
        const auto* 节点 = std::get_if<节点事实>(&*结果.事实);
        if (!节点 || 结果.读取事实代次 == 0) {
            if (失败) *失败 = 特征定义状态::内部不一致;
            return std::nullopt;
        }
        if (代次 == 0) 代次 = 结果.读取事实代次;
        else if (代次 != 结果.读取事实代次) {
            if (失败) *失败 = 特征定义状态::事实代次漂移;
            return std::nullopt;
        }
        return *节点;
    }

    I64特征定义结果 读取I64特征定义内部(
        稳定编码 定义, 稳定编码 标记值, 稳定编码 下界值, 稳定编码 上界值) const {
        std::uint64_t 代次 = 0;
        特征定义状态 失败 = 特征定义状态::内部不一致;
        const auto 定义节点 = 读取节点(定义, 代次, &失败);
        if (!定义节点) return {失败, std::nullopt, {}, {}, {}};
        if (定义节点->种类 != 节点种类::普通 || 定义节点->退出事实代次
            || 定义节点->当前属性.size() != 3) {
            return {特征定义状态::内部不一致, std::nullopt, {}, {}, {}};
        }
        const auto 标记属性 = L1_.读取当前属性(
            {L1事实基座合同版本, 定义, 登记_->定义标记属性类型});
        const auto 下界属性 = L1_.读取当前属性(
            {L1事实基座合同版本, 定义, 登记_->I64下界属性类型});
        const auto 上界属性 = L1_.读取当前属性(
            {L1事实基座合同版本, 定义, 登记_->I64上界属性类型});
        if (标记属性.状态 != L1读取状态::成功
            || 下界属性.状态 != L1读取状态::成功
            || 上界属性.状态 != L1读取状态::成功
            || !标记属性.属性.has_value() || !下界属性.属性.has_value()
            || !上界属性.属性.has_value()) {
            return {映射读取结果(标记属性.状态), std::nullopt, {}, {}, {}};
        }
        if (标记属性.读取事实代次 != 代次 || 下界属性.读取事实代次 != 代次
            || 上界属性.读取事实代次 != 代次) {
            return {特征定义状态::事实代次漂移, std::nullopt, {}, {}, {}};
        }
        if (标记属性.属性->当前值 != 标记值
            || 下界属性.属性->当前值 != 下界值
            || 上界属性.属性->当前值 != 上界值) {
            return {特征定义状态::内部不一致, std::nullopt, {}, {}, {}};
        }
        const auto 标记 = 读取值(标记值, 代次, 失败);
        const auto 下界 = 读取值(下界值, 代次, 失败);
        const auto 上界 = 读取值(上界值, 代次, 失败);
        if (!标记 || !下界 || !上界) return {失败, std::nullopt, {}, {}, {}};
        if (标记->所属节点 != 定义 || 下界->所属节点 != 定义
            || 上界->所属节点 != 定义
            || 标记->属性类型节点 != 登记_->定义标记属性类型
            || 下界->属性类型节点 != 登记_->I64下界属性类型
            || 上界->属性类型节点 != 登记_->I64上界属性类型
            || 标记->来源节点 != 登记_->服务身份
            || 下界->来源节点 != 登记_->服务身份
            || 上界->来源节点 != 登记_->服务身份
            || 标记->退出事实代次 || 下界->退出事实代次 || 上界->退出事实代次
            || !std::holds_alternative<std::int64_t>(标记->材料)
            || !std::holds_alternative<std::int64_t>(下界->材料)
            || !std::holds_alternative<std::int64_t>(上界->材料)) {
            return {特征定义状态::内部不一致, std::nullopt, {}, {}, {}};
        }
        const auto 标记数值 = std::get<std::int64_t>(标记->材料);
        const auto 下界数值 = std::get<std::int64_t>(下界->材料);
        const auto 上界数值 = std::get<std::int64_t>(上界->材料);
        if (标记数值 != 1 || 下界数值 > 上界数值) {
            return {特征定义状态::内部不一致, std::nullopt, {}, {}, {}};
        }
        I64特征定义事实 事实;
        事实.事实截止代次 = 代次;
        事实.特征定义 = 定义;
        事实.下界 = 下界数值;
        事实.上界 = 上界数值;
        事实.来源服务 = 登记_->服务身份;
        if (!I64特征定义事实完整(事实)) {
            return {特征定义状态::内部不一致, std::nullopt, {}, {}, {}};
        }
        return {特征定义状态::已读取, 事实, 标记值, 下界值, 上界值};
    }

    std::optional<值事实> 读取值(稳定编码 编码, std::uint64_t& 代次,
        特征定义状态& 失败) const {
        const auto 结果 = L1_.读取当前值({L1事实基座合同版本, 编码});
        if (结果.状态 != L1读取状态::成功 || !结果.事实.has_value()) {
            失败 = 映射读取结果(结果.状态);
            return std::nullopt;
        }
        const auto* 值 = std::get_if<值事实>(&*结果.事实);
        if (!值 || 结果.读取事实代次 == 0) {
            失败 = 特征定义状态::内部不一致;
            return std::nullopt;
        }
        if (代次 == 0) 代次 = 结果.读取事实代次;
        else if (代次 != 结果.读取事实代次) {
            失败 = 特征定义状态::事实代次漂移;
            return std::nullopt;
        }
        return *值;
    }

    L1事实基座服务& L1_;
    mutable std::mutex 锁_;
    std::optional<特征定义登记请求> 首次登记请求_;
    std::optional<特征定义登记> 登记_;
    std::optional<I64特征定义建立请求> 定义请求_;
    std::array<稳定编码, 4> 定义编码_{};
    std::optional<I64特征定义事实> 定义事实_;
};

} // namespace 海中鱼巣
