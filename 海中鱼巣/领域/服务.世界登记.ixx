module;

#include <array>
#include <cstddef>
#include <cstdint>
#include <mutex>
#include <new>
#include <optional>
#include <variant>

export module 海中鱼巣.领域.服务.世界登记;

export import 海中鱼巣.核心.合同.L1中性CRUD;
import 海中鱼巣.核心.服务.L1事实基座;

#define 世界登记数据已导入L1
export {
#include "世界登记.数据.h"
}
#undef 世界登记数据已导入L1

export namespace 海中鱼巣 {

namespace 世界登记内部 {

// 诊断责任：无适用错误分支；纯值幂等身份映射。
L1中性写集幂等键 映射L1幂等键(世界操作幂等身份 身份) noexcept {
    return {(世界登记中性幂等域 << 56) | 身份.值};
}

} // namespace 世界登记内部

class 世界登记服务 final {
public:
    explicit 世界登记服务(L1事实基座服务& L1) noexcept : L1_(L1) {}
    世界登记服务(const 世界登记服务&) = delete;
    世界登记服务& operator=(const 世界登记服务&) = delete;

    // 诊断责任：向上送出；全部非成功保留为结构化世界登记状态。
    世界登记结果 建立空世界(const 世界登记建立请求& 请求) {
        std::lock_guard<std::mutex> 锁(锁_);
        try {
            if (!世界登记建立请求有效(请求)) return {};

            const auto 幂等键 = 世界登记内部::映射L1幂等键(请求.幂等身份);
            L1中性写集请求 写集;
            写集.合同版本 = L1中性CRUD合同版本;
            写集.期望事实代次 = 请求.期望事实代次;
            写集.幂等键 = 幂等键;
            写集.节点 = {
                {{1}, 节点种类::普通, std::nullopt},
                {{2}, 节点种类::属性类型, L1中性值表示种类::I64},
                {{3}, 节点种类::普通, std::nullopt},
                {{4}, 节点种类::普通, std::nullopt},
                {{5}, 节点种类::普通, std::nullopt}
            };
            写集.值 = {{
                {6}, L1中性写集本地键{5}, L1中性写集本地键{2},
                std::int64_t{1}, L1中性写集本地键{1}
            }};
            写集.属性槽变更 = {{
                L1中性写集本地键{5}, L1中性写集本地键{2},
                L1中性写集本地键{6}
            }};

            const auto 写入 = L1_.提交中性写集(写集);
            const auto 写入状态 = 映射写入状态(写入.状态);
            if (写入状态 != 世界登记状态::已提交
                && 写入状态 != 世界登记状态::幂等读回) {
                return {写入状态, std::nullopt};
            }

            const bool 首次成功完整 = 写入.状态 == L1中性写入状态::成功
                && 写入.是否形成内存权威发布
                && 写入.重试边界 == L1中性重试边界::不适用;
            const bool 重复成功完整 = 写入.状态 == L1中性写入状态::精确重复
                && !写入.是否形成内存权威发布
                && 写入.重试边界 == L1中性重试边界::原幂等键读回收敛;
            if (写入.合同版本 != L1中性CRUD合同版本
                || !(写入.幂等键 == 幂等键) || 写入.事实代次 == 0
                || 写入.新编码映射.size() != 6
                || (!首次成功完整 && !重复成功完整)
                || !映射唯一(写入)) {
                return {世界登记状态::内部不一致, std::nullopt};
            }

            const auto 编码1 = 查找映射(写入, 1);
            const auto 编码2 = 查找映射(写入, 2);
            const auto 编码3 = 查找映射(写入, 3);
            const auto 编码4 = 查找映射(写入, 4);
            const auto 编码5 = 查找映射(写入, 5);
            const auto 编码6 = 查找映射(写入, 6);
            if (!编码1 || !编码2 || !编码3 || !编码4 || !编码5 || !编码6) {
                return {世界登记状态::内部不一致, std::nullopt};
            }

            世界结构登记 新登记;
            新登记.首次幂等身份 = 请求.幂等身份;
            新登记.首次中性写集幂等键 = 幂等键.值;
            新登记.已验证事实代次 = 写入.事实代次;
            新登记.服务身份 = *编码1;
            新登记.场景标记属性类型 = *编码2;
            新登记.直接父场景关系类型 = *编码3;
            新登记.直接成员关系类型 = *编码4;
            新登记.世界根场景 = *编码5;
            if (!世界结构登记完整(新登记)) {
                return {世界登记状态::内部不一致, std::nullopt};
            }

            const auto 读回 = 按登记读取权威事实(新登记);
            if (!读回.成功()) return {读回.状态, std::nullopt};
            登记_ = 新登记;
            return {写入状态, 登记_};
        } catch (const std::bad_alloc&) {
            return {世界登记状态::资源失败, std::nullopt};
        } catch (...) {
            return {世界登记状态::内部不一致, std::nullopt};
        }
    }

    // 诊断责任：向上送出；缓存只作定位，成功必须来自L1权威精确读回。
    世界登记结果 读取当前登记() const {
        std::lock_guard<std::mutex> 锁(锁_);
        if (!登记_.has_value()) return {世界登记状态::登记未加载, std::nullopt};
        try {
            return 按登记读取权威事实(*登记_);
        } catch (const std::bad_alloc&) {
            return {世界登记状态::资源失败, std::nullopt};
        } catch (...) {
            return {世界登记状态::内部不一致, std::nullopt};
        }
    }

private:
    // 诊断责任：无适用错误分支；中性读取状态机械映射。
    static 世界登记状态 映射读取失败(L1中性读取状态 状态) noexcept {
        return 状态 == L1中性读取状态::资源失败
            ? 世界登记状态::资源失败 : 世界登记状态::内部不一致;
    }

    // 诊断责任：无适用错误分支；中性写入状态机械映射。
    static 世界登记状态 映射写入状态(L1中性写入状态 状态) noexcept {
        switch (状态) {
        case L1中性写入状态::成功: return 世界登记状态::已提交;
        case L1中性写入状态::精确重复: return 世界登记状态::幂等读回;
        case L1中性写入状态::入口拒绝: return 世界登记状态::入口拒绝;
        case L1中性写入状态::事实代次漂移: return 世界登记状态::事实代次漂移;
        case L1中性写入状态::幂等冲突: return 世界登记状态::幂等冲突;
        case L1中性写入状态::资源失败: return 世界登记状态::资源失败;
        case L1中性写入状态::未找到:
        case L1中性写入状态::已退出:
        case L1中性写入状态::内部不一致:
            return 世界登记状态::内部不一致;
        }
        return 世界登记状态::内部不一致;
    }

    // 诊断责任：无适用错误分支；纯值映射唯一查找。
    static std::optional<稳定编码> 查找映射(
        const L1中性写入结果& 结果, std::uint32_t 本地键) noexcept {
        std::optional<稳定编码> 找到;
        for (const auto& [键, 编码] : 结果.新编码映射) {
            if (键.值 != 本地键) continue;
            if (找到.has_value()) return std::nullopt;
            找到 = 编码;
        }
        return 找到;
    }

    // 诊断责任：无适用错误分支；纯值映射完整性判断。
    static bool 映射唯一(const L1中性写入结果& 结果) noexcept {
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

    // 诊断责任：向上送出；逐项读取失败和字段矛盾保留为结构化状态。
    世界登记结果 按登记读取权威事实(const 世界结构登记& 登记) const {
        if (!世界结构登记完整(登记)) {
            return {世界登记状态::内部不一致, std::nullopt};
        }

        const auto 起始 = L1_.读取中性当前事实代次({});
        if (起始.状态 != L1中性读取状态::成功) {
            return {映射读取失败(起始.状态), std::nullopt};
        }

        const std::array<稳定编码, 5> 编码组{
            登记.服务身份, 登记.场景标记属性类型,
            登记.直接父场景关系类型, 登记.直接成员关系类型,
            登记.世界根场景};
        std::array<L1中性节点读取结果, 5> 节点组;
        for (std::size_t i = 0; i < 编码组.size(); ++i) {
            节点组[i] = L1_.读取中性当前节点({L1中性CRUD合同版本, 编码组[i]});
            if (节点组[i].状态 != L1中性读取状态::成功) {
                return {映射读取失败(节点组[i].状态), std::nullopt};
            }
        }

        const auto 属性 = L1_.读取中性当前属性({
            L1中性CRUD合同版本, 登记.世界根场景, 登记.场景标记属性类型});
        if (属性.状态 != L1中性读取状态::成功 || !属性.当前值.has_value()) {
            return {映射读取失败(属性.状态), std::nullopt};
        }
        const auto 值 = L1_.读取中性当前值({L1中性CRUD合同版本, *属性.当前值});
        if (值.状态 != L1中性读取状态::成功 || !值.事实.has_value()) {
            return {映射读取失败(值.状态), std::nullopt};
        }
        const auto 结束 = L1_.读取中性当前事实代次({});
        if (结束.状态 != L1中性读取状态::成功) {
            return {映射读取失败(结束.状态), std::nullopt};
        }

        const auto 代次 = 登记.已验证事实代次;
        if (起始.合同版本 != L1中性CRUD合同版本
            || 结束.合同版本 != L1中性CRUD合同版本
            || 起始.事实代次 != 代次 || 结束.事实代次 != 代次) {
            return {世界登记状态::事实代次漂移, std::nullopt};
        }

        for (std::size_t i = 0; i < 节点组.size(); ++i) {
            const auto& 结果 = 节点组[i];
            if (结果.合同版本 != L1中性CRUD合同版本
                || 结果.查询编码 != 编码组[i]
                || 结果.读取事实代次 != 代次 || !结果.事实.has_value()) {
                return {结果.读取事实代次 != 代次
                        ? 世界登记状态::事实代次漂移
                        : 世界登记状态::内部不一致,
                    std::nullopt};
            }
            const auto& 事实 = *结果.事实;
            const auto 期望种类 = i == 1 ? 节点种类::属性类型 : 节点种类::普通;
            if (事实.编码 != 编码组[i] || 事实.种类 != 期望种类
                || 事实.创建事实代次 != 代次 || 事实.退出事实代次.has_value()
                || (i == 1 && 事实.属性类型表示
                    != std::optional<L1中性值表示种类>{L1中性值表示种类::I64})
                || (i != 1 && 事实.属性类型表示.has_value())
                || (i != 4 && !事实.当前属性.empty())) {
                return {世界登记状态::内部不一致, std::nullopt};
            }
        }

        const auto& 根 = *节点组[4].事实;
        if (属性.合同版本 != L1中性CRUD合同版本
            || 属性.节点 != 登记.世界根场景
            || 属性.属性类型 != 登记.场景标记属性类型
            || 属性.读取事实代次 != 代次
            || 根.当前属性.size() != 1
            || 根.当前属性[0].属性类型节点 != 登记.场景标记属性类型
            || 根.当前属性[0].当前值 != *属性.当前值) {
            return {属性.读取事实代次 != 代次
                    ? 世界登记状态::事实代次漂移
                    : 世界登记状态::内部不一致,
                std::nullopt};
        }

        const auto& 值事实 = *值.事实;
        if (值.合同版本 != L1中性CRUD合同版本
            || 值.查询编码 != *属性.当前值 || 值.读取事实代次 != 代次
            || 值事实.编码 != *属性.当前值
            || 值事实.所属节点 != 登记.世界根场景
            || 值事实.属性类型节点 != 登记.场景标记属性类型
            || 值事实.来源节点 != 登记.服务身份
            || 值事实.创建事实代次 != 代次 || 值事实.退出事实代次.has_value()
            || !std::holds_alternative<std::int64_t>(值事实.材料)
            || std::get<std::int64_t>(值事实.材料) != 1) {
            return {值.读取事实代次 != 代次
                    ? 世界登记状态::事实代次漂移
                    : 世界登记状态::内部不一致,
                std::nullopt};
        }
        return {世界登记状态::已读取, 登记};
    }

    L1事实基座服务& L1_;
    mutable std::mutex 锁_;
    std::optional<世界结构登记> 登记_;
};

} // namespace 海中鱼巣
