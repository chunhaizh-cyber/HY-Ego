module;

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <mutex>
#include <new>
#include <optional>
#include <variant>
#include <vector>

export module 海中鱼巣.领域.服务.世界登记;

export import 海中鱼巣.核心.合同.L1公共事实;
import 海中鱼巣.核心.服务.L1事实基座;

#define 世界登记数据已导入L1
export {
#include "世界登记.数据.h"
}
#undef 世界登记数据已导入L1

export namespace 海中鱼巣 {

namespace 世界登记内部 {

inline constexpr std::uint64_t L1幂等域 = 0x01ULL;

写集幂等键 映射L1幂等键(世界操作幂等身份 身份) noexcept {
    return { (L1幂等域 << 56) | 身份.值 };
}

L1领域意图凭证 形成G1世界登记意图凭证(const 世界登记建立请求& 请求) {
    std::vector<std::uint8_t> 编码{'H','Z','Y','-','L','1','G','1'};
    L1确定性编码内部::写U32(编码, L1请求意图格式版本);
    L1确定性编码内部::写U8(编码, 1);
    L1确定性编码内部::写U32(编码, 0x15000701U);
    L1确定性编码内部::写U32(编码, 请求.合同版本);
    L1确定性编码内部::写U32(编码, 请求.世界规则版本);
    L1确定性编码内部::写U64(编码, 请求.幂等身份.值);
    L1确定性编码内部::写U64(编码, 请求.期望事实代次);
    return {1, L1请求意图格式版本, 0x15000701U,
        L1确定性编码内部::哈希(std::move(编码))};
}

} // namespace 世界登记内部

class 世界登记服务 final {
public:
    explicit 世界登记服务(L1事实基座服务& L1) noexcept : L1_(L1) {}
    世界登记服务(const 世界登记服务&) = delete;
    世界登记服务& operator=(const 世界登记服务&) = delete;

    世界登记结果 建立空世界(const 世界登记建立请求& 请求) {
        std::lock_guard<std::mutex> 锁(锁_);
        try {
            if (!世界登记建立请求有效(请求)) return {};
            const auto 凭证 = 世界登记内部::形成G1世界登记意图凭证(请求);
            const auto 幂等键 = 世界登记内部::映射L1幂等键(请求.幂等身份);
            const auto 探测 = L1_.探测幂等({L1事实基座合同版本,
                L1幂等摘要合同版本, 幂等键, 凭证.请求意图摘要, 凭证});
            if (探测.状态 == L1幂等探测状态::同义已发布) {
                return 首次请求_.has_value() && *首次请求_ == 请求 && 登记_.has_value()
                    ? 世界登记结果{世界登记状态::幂等读回, 登记_}
                    : 世界登记结果{世界登记状态::内部不一致, std::nullopt};
            }
            if (探测.状态 == L1幂等探测状态::异义冲突)
                return {世界登记状态::幂等冲突, std::nullopt};
            if (探测.状态 == L1幂等探测状态::已隔离
                || 探测.状态 == L1幂等探测状态::内部不一致)
                return {世界登记状态::内部不一致, std::nullopt};
            if (探测.状态 != L1幂等探测状态::未找到)
                return {世界登记状态::入口拒绝, std::nullopt};

            const auto 初始 = L1_.读取完整快照({});
            if (初始.状态 != L1读取状态::成功 || !初始.快照.has_value()) {
                return {映射读取失败(初始.状态), std::nullopt};
            }
            if (初始.快照->事实代次 != 0 || 请求.期望事实代次 != 0
                || !初始.快照->当前节点.empty()
                || !初始.快照->当前关系.empty()
                || !初始.快照->当前值.empty()
                || !初始.快照->永久占用编码.empty()) {
                return {世界登记状态::入口拒绝, std::nullopt};
            }

            L1写集请求 写集;
            写集.期望事实代次 = 初始.快照->事实代次;
            写集.幂等键 = 幂等键;
            写集.领域意图凭证 = 凭证;
            写集.节点 = {
                {{1}, 节点种类::普通, std::nullopt},
                {{2}, 节点种类::属性类型, 值表示种类::I64},
                {{3}, 节点种类::普通, std::nullopt},
                {{4}, 节点种类::普通, std::nullopt},
                {{5}, 节点种类::普通, std::nullopt}
            };
            写集.值 = {{
                {6}, 写集本地键{5}, 写集本地键{2},
                std::int64_t{1}, 写集本地键{1}
            }};
            写集.属性槽变更 = {{
                写集本地键{5}, 写集本地键{2}, 写集本地键{6}
            }};

            const auto 写入 = L1_.提交写集(写集, 0x15000701U);
            const auto 写入状态 = 映射写入状态(写入.状态);
            if (写入状态 != 世界登记状态::已提交
                && 写入状态 != 世界登记状态::幂等读回) {
                return {写入状态, std::nullopt};
            }
            if (写入.事实代次 == 0 || 写入.新编码映射.size() != 6) {
                return {世界登记状态::内部不一致, std::nullopt};
            }

            const auto 编码1 = 查找映射(写入, 1);
            const auto 编码2 = 查找映射(写入, 2);
            const auto 编码3 = 查找映射(写入, 3);
            const auto 编码4 = 查找映射(写入, 4);
            const auto 编码5 = 查找映射(写入, 5);
            const auto 编码6 = 查找映射(写入, 6);
            if (!编码1 || !编码2 || !编码3 || !编码4 || !编码5 || !编码6
                || !映射唯一(写入)) {
                return {世界登记状态::内部不一致, std::nullopt};
            }

            const auto 读回 = L1_.读取完整快照({});
            if (读回.状态 != L1读取状态::成功 || !读回.快照.has_value()) {
                return {读回.状态 == L1读取状态::事实代次漂移
                        ? 世界登记状态::事实代次漂移
                        : 世界登记状态::内部不一致,
                    std::nullopt};
            }
            if (读回.快照->事实代次 != 写入.事实代次) {
                return {世界登记状态::事实代次漂移, std::nullopt};
            }
            if (!快照匹配(*读回.快照, 写入.事实代次,
                    {*编码1, *编码2, *编码3, *编码4, *编码5, *编码6})) {
                return {世界登记状态::内部不一致, std::nullopt};
            }

            世界结构登记 新登记;
            新登记.首次幂等身份 = 请求.幂等身份;
            新登记.已验证事实代次 = 写入.事实代次;
            新登记.服务身份 = *编码1;
            新登记.场景标记属性类型 = *编码2;
            新登记.直接父场景关系类型 = *编码3;
            新登记.直接成员关系类型 = *编码4;
            新登记.世界根场景 = *编码5;
            if (!世界结构登记完整(新登记)) {
                return {世界登记状态::内部不一致, std::nullopt};
            }
            首次请求_ = 请求;
            登记_ = 新登记;
            return {写入.状态 == L1写入状态::精确重复
                    ? 世界登记状态::幂等读回 : 世界登记状态::已提交,
                登记_};
        } catch (const std::bad_alloc&) {
            return {世界登记状态::资源失败, std::nullopt};
        } catch (...) {
            return {世界登记状态::内部不一致, std::nullopt};
        }
    }

    世界登记结果 读取当前登记() const {
        std::lock_guard<std::mutex> 锁(锁_);
        if (!登记_.has_value()) return {世界登记状态::登记未加载, std::nullopt};
        return {世界登记状态::已读取, 登记_};
    }

private:
    static 世界登记状态 映射读取失败(L1读取状态 状态) noexcept {
        if (状态 == L1读取状态::资源失败) return 世界登记状态::资源失败;
        if (状态 == L1读取状态::内部不一致) return 世界登记状态::内部不一致;
        return 世界登记状态::入口拒绝;
    }

    static 世界登记状态 映射写入状态(L1写入状态 状态) noexcept {
        switch (状态) {
        case L1写入状态::成功: return 世界登记状态::已提交;
        case L1写入状态::精确重复: return 世界登记状态::幂等读回;
        case L1写入状态::幂等冲突: return 世界登记状态::幂等冲突;
        case L1写入状态::事实代次漂移: return 世界登记状态::事实代次漂移;
        case L1写入状态::资源失败: return 世界登记状态::资源失败;
        case L1写入状态::内部不一致: return 世界登记状态::内部不一致;
        default: return 世界登记状态::入口拒绝;
        }
    }

    static std::optional<稳定编码> 查找映射(
        const L1写入结果& 结果, std::uint32_t 本地键) noexcept {
        std::optional<稳定编码> 找到;
        for (const auto& [键, 编码] : 结果.新编码映射) {
            if (键.值 != 本地键) continue;
            if (找到.has_value()) return std::nullopt;
            找到 = 编码;
        }
        return 找到;
    }

    static bool 映射唯一(const L1写入结果& 结果) noexcept {
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

    static bool 快照匹配(
        const L1完整快照& 快照, std::uint64_t 代次,
        const std::array<稳定编码, 6>& 编码) noexcept {
        if (快照.事实代次 != 代次 || 快照.当前节点.size() != 5
            || !快照.当前关系.empty() || 快照.当前值.size() != 1
            || 快照.永久占用编码.size() != 6) return false;
        auto 节点 = [&](稳定编码 编码值) -> const 节点事实* {
            for (const auto& 事实 : 快照.当前节点)
                if (事实.编码 == 编码值) return &事实;
            return nullptr;
        };
        auto 永久存在 = [&](稳定编码 编码值) {
            return std::find(快照.永久占用编码.begin(),
                快照.永久占用编码.end(), 编码值) != 快照.永久占用编码.end();
        };
        const auto *身份 = 节点(编码[0]);
        const auto *类型 = 节点(编码[1]);
        const auto *父类型 = 节点(编码[2]);
        const auto *成员类型 = 节点(编码[3]);
        const auto *根 = 节点(编码[4]);
        if (身份 == nullptr || 类型 == nullptr || 父类型 == nullptr
            || 成员类型 == nullptr || 根 == nullptr
            || 身份->种类 != 节点种类::普通
            || 类型->种类 != 节点种类::属性类型
            || !类型->属性类型表示.has_value()
            || *类型->属性类型表示 != 值表示种类::I64
            || 父类型->种类 != 节点种类::普通
            || 成员类型->种类 != 节点种类::普通
            || 根->种类 != 节点种类::普通
            || !身份->当前属性.empty() || !类型->当前属性.empty()
            || !父类型->当前属性.empty()
            || !成员类型->当前属性.empty()
            || 身份->创建事实代次 != 代次 || 类型->创建事实代次 != 代次
            || 父类型->创建事实代次 != 代次 || 成员类型->创建事实代次 != 代次
            || 根->创建事实代次 != 代次
            || 身份->退出事实代次 || 类型->退出事实代次
            || 父类型->退出事实代次 || 成员类型->退出事实代次
            || 根->退出事实代次) return false;
        if (根->当前属性.size() != 1
            || 根->当前属性[0].属性类型节点 != 编码[1]
            || 根->当前属性[0].当前值 != 编码[5]) return false;
        const auto& 值 = 快照.当前值[0];
        if (值.编码 != 编码[5] || 值.所属节点 != 编码[4]
            || 值.属性类型节点 != 编码[1] || 值.来源节点 != 编码[0]
            || 值.创建事实代次 != 代次 || 值.退出事实代次
            || !std::holds_alternative<std::int64_t>(值.材料)
            || std::get<std::int64_t>(值.材料) != 1) return false;
        return std::all_of(编码.begin(), 编码.end(), 永久存在);
    }

    L1事实基座服务& L1_;
    mutable std::mutex 锁_;
    std::optional<世界登记建立请求> 首次请求_;
    std::optional<世界结构登记> 登记_;
};

} // namespace 海中鱼巣
