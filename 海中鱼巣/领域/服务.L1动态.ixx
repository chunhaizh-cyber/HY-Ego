module;

#include <array>
#include <cstdint>
#include <mutex>
#include <new>
#include <optional>
#include <utility>
#include <vector>

export module 海中鱼巣.领域.服务.L1动态;

import 海中鱼巣.核心.服务.L1事实基座;

export {
#include "L1动态.数据.h"
}

namespace 海中鱼巣::L1动态内部 {

// P14-诊断责任：无适用错误分支；最终责任边界：调用服务。
L1中性写集幂等键 映射登记幂等键(动态操作幂等身份 身份) noexcept {
    return {(L1动态登记幂等域 << 56) | 身份.值};
}

// P14-诊断责任：向上送出；最终责任边界：建立登记调用边。
L1动态状态 映射中性写入状态(L1中性写入状态 状态) noexcept {
    switch (状态) {
    case L1中性写入状态::成功: return L1动态状态::已登记;
    case L1中性写入状态::精确重复: return L1动态状态::已登记;
    case L1中性写入状态::幂等冲突: return L1动态状态::幂等冲突;
    case L1中性写入状态::事实代次漂移: return L1动态状态::版本漂移;
    case L1中性写入状态::未找到:
    case L1中性写入状态::已退出: return L1动态状态::未找到;
    case L1中性写入状态::资源失败: return L1动态状态::资源失败;
    case L1中性写入状态::内部不一致: return L1动态状态::内部不一致;
    case L1中性写入状态::入口拒绝: return L1动态状态::入口拒绝;
    default: return L1动态状态::入口拒绝;
    }
}

// P14-诊断责任：向上送出；最终责任边界：登记中性读回调用边。
L1动态状态 映射中性读取状态(L1中性读取状态 状态) noexcept {
    switch (状态) {
    case L1中性读取状态::未找到:
    case L1中性读取状态::属性未设置:
    case L1中性读取状态::已退出: return L1动态状态::未找到;
    case L1中性读取状态::资源失败: return L1动态状态::资源失败;
    case L1中性读取状态::内部不一致: return L1动态状态::内部不一致;
    case L1中性读取状态::入口拒绝: return L1动态状态::入口拒绝;
    default: return L1动态状态::内部不一致;
    }
}

// P14-诊断责任：无适用错误分支；最终责任边界：调用服务。
std::optional<稳定编码> 查找中性映射(
    const std::vector<std::pair<L1中性写集本地键, 稳定编码>>& 映射,
    std::uint32_t 本地键) noexcept {
    std::optional<稳定编码> 找到;
    for (const auto& [键, 编码] : 映射) {
        if (键.值 != 本地键) continue;
        if (找到) return std::nullopt;
        找到 = 编码;
    }
    return 找到;
}

// P14-诊断责任：无适用错误分支；最终责任边界：调用服务。
bool 中性映射完整唯一(
    const std::vector<std::pair<L1中性写集本地键, 稳定编码>>& 映射,
    std::uint32_t 数量) noexcept {
    if (映射.size() != 数量) return false;
    for (std::uint32_t i = 1; i <= 数量; ++i) {
        const auto 编码 = 查找中性映射(映射, i);
        if (!编码 || !有效(*编码)) return false;
        for (std::uint32_t j = i + 1; j <= 数量; ++j) {
            const auto 另一个 = 查找中性映射(映射, j);
            if (!另一个 || *另一个 == *编码) return false;
        }
    }
    return true;
}

// P14-诊断责任：无适用错误分支；登记中性节点形状谓词。
bool 中性节点类型符合(const L1中性节点事实& 节点, std::size_t 索引) noexcept {
    if (索引 < 2)
        return 节点.种类 == 节点种类::普通 && !节点.属性类型表示;
    const std::array<L1中性值表示种类, 8> 表示{L1中性值表示种类::I64,
        L1中性值表示种类::I64, L1中性值表示种类::I64,
        L1中性值表示种类::U64组, L1中性值表示种类::U64组,
        L1中性值表示种类::I64, L1中性值表示种类::U64组,
        L1中性值表示种类::I64组};
    return 节点.种类 == 节点种类::属性类型
        && 节点.属性类型表示 == std::optional{表示[索引 - 2]};
}

} // namespace 海中鱼巣::L1动态内部

export namespace 海中鱼巣 {

class L1动态服务 final {
public:
    // P14-诊断责任：无适用错误分支；最终责任边界：构造调用边。
    explicit L1动态服务(L1事实基座服务& L1) noexcept : L1_(L1) {}
    L1动态服务(const L1动态服务&) = delete;
    L1动态服务& operator=(const L1动态服务&) = delete;

    // P14-诊断责任：向上送出；最终责任边界：公开结构化结果或未来合法消费者。
    L1动态登记结果 建立登记(const L1动态登记请求& 请求) {
        std::lock_guard<std::recursive_mutex> 锁(锁_);
        try {
            if (请求.合同版本 != L1动态合同版本 || 请求.规则版本 != L1动态规则版本
                || !动态操作幂等身份有效(请求.幂等身份) || 请求.期望事实代次 == 0)
                return {};

            const auto 预读 = L1_.读取中性当前事实代次({L1中性CRUD合同版本});
            if (预读.合同版本 != L1中性CRUD合同版本)
                return {L1动态状态::内部不一致, std::nullopt};
            if (预读.状态 != L1中性读取状态::成功)
                return {L1动态内部::映射中性读取状态(预读.状态), std::nullopt};
            if (预读.事实代次 == 0)
                return {L1动态状态::内部不一致, std::nullopt};

            const auto 幂等键 = L1动态内部::映射登记幂等键(请求.幂等身份);
            L1中性写集请求 写集;
            写集.合同版本 = L1中性CRUD合同版本;
            写集.期望事实代次 = 请求.期望事实代次;
            写集.幂等键 = 幂等键;
            写集.节点 = {{{1}, 节点种类::普通, std::nullopt},
                {{2}, 节点种类::普通, std::nullopt},
                {{3}, 节点种类::属性类型, L1中性值表示种类::I64},
                {{4}, 节点种类::属性类型, L1中性值表示种类::I64},
                {{5}, 节点种类::属性类型, L1中性值表示种类::I64},
                {{6}, 节点种类::属性类型, L1中性值表示种类::U64组},
                {{7}, 节点种类::属性类型, L1中性值表示种类::U64组},
                {{8}, 节点种类::属性类型, L1中性值表示种类::I64},
                {{9}, 节点种类::属性类型, L1中性值表示种类::U64组},
                {{10}, 节点种类::属性类型, L1中性值表示种类::I64组}};

            const auto 写入 = L1_.提交中性写集(写集);
            if (写入.状态 != L1中性写入状态::成功
                && 写入.状态 != L1中性写入状态::精确重复)
                return {L1动态内部::映射中性写入状态(写入.状态), std::nullopt};

            if (写入.合同版本 != L1中性CRUD合同版本 || 写入.幂等键 != 幂等键
                || 写入.事实代次 == 0
                || !L1动态内部::中性映射完整唯一(写入.新编码映射, 10))
                return {L1动态状态::内部不一致, std::nullopt};
            if (写入.状态 == L1中性写入状态::成功) {
                if (预读.事实代次 != 请求.期望事实代次
                    || !写入.是否形成内存权威发布
                    || 写入.重试边界 != L1中性重试边界::不适用)
                    return {L1动态状态::内部不一致, std::nullopt};
            } else if (写入.是否形成内存权威发布
                || 写入.重试边界 != L1中性重试边界::原幂等键读回收敛) {
                return {L1动态状态::内部不一致, std::nullopt};
            }

            std::array<稳定编码, 10> 编码组{};
            for (std::uint32_t i = 1; i <= 编码组.size(); ++i) {
                const auto 编码 = L1动态内部::查找中性映射(写入.新编码映射, i);
                if (!编码) return {L1动态状态::内部不一致, std::nullopt};
                编码组[i - 1] = *编码;
            }
            auto 读回 = 精确读回登记(请求.幂等身份, 编码组);
            if (读回.状态 != L1动态状态::已读取 || !读回.登记)
                return 读回;
            登记_ = *读回.登记;
            return {L1动态状态::已登记, 登记_};
        } catch (const std::bad_alloc&) {
            return {L1动态状态::资源失败, std::nullopt};
        } catch (...) {
            return {L1动态状态::内部不一致, std::nullopt};
        }
    }

    // P14-诊断责任：向上送出；最终责任边界：公开结构化结果或未来合法消费者。
    L1动态登记结果 读取当前登记() const {
        std::lock_guard<std::recursive_mutex> 锁(锁_);
        try {
            if (!登记_ || !L1动态结构登记完整(*登记_))
                return {L1动态状态::登记未加载, std::nullopt};
            const auto& 定位 = *登记_;
            const std::array<稳定编码, 10> 编码组{定位.服务身份,
                定位.关系20动态组成关系类型, 定位.动态资格属性类型,
                定位.动态种类属性类型, 定位.聚合规则版本属性类型,
                定位.时间范围属性类型, 定位.迁移比较合同属性类型,
                定位.迁移差异I64属性类型, 定位.比较输入引用属性类型,
                定位.比较输入I64组属性类型};
            return 精确读回登记(定位.首次幂等身份, 编码组);
        } catch (const std::bad_alloc&) {
            return {L1动态状态::资源失败, std::nullopt};
        } catch (...) {
            return {L1动态状态::内部不一致, std::nullopt};
        }
    }

    // P14-诊断责任：向上送出；最终责任边界：公开结构化结果或未来合法消费者。
private:
    // P14-诊断责任：向上送出；最终责任边界：建立登记与读取当前登记调用边。
    L1动态登记结果 精确读回登记(动态操作幂等身份 身份,
        const std::array<稳定编码, 10>& 编码组) const {
        if (!动态操作幂等身份有效(身份))
            return {L1动态状态::内部不一致, std::nullopt};
        for (std::size_t i = 0; i < 编码组.size(); ++i) {
            if (!有效(编码组[i]))
                return {L1动态状态::内部不一致, std::nullopt};
            for (std::size_t j = i + 1; j < 编码组.size(); ++j)
                if (编码组[i] == 编码组[j])
                    return {L1动态状态::内部不一致, std::nullopt};
        }

        const auto G0 = L1_.读取中性当前事实代次({L1中性CRUD合同版本});
        if (G0.合同版本 != L1中性CRUD合同版本)
            return {L1动态状态::内部不一致, std::nullopt};
        if (G0.状态 != L1中性读取状态::成功)
            return {L1动态内部::映射中性读取状态(G0.状态), std::nullopt};
        if (G0.事实代次 == 0)
            return {L1动态状态::内部不一致, std::nullopt};

        std::array<L1中性节点事实, 10> 节点组{};
        for (std::size_t i = 0; i < 编码组.size(); ++i) {
            const auto 读取 = L1_.读取中性当前节点(
                {L1中性CRUD合同版本, 编码组[i]});
            if (读取.合同版本 != L1中性CRUD合同版本 || 读取.查询编码 != 编码组[i])
                return {L1动态状态::内部不一致, std::nullopt};
            if (读取.状态 != L1中性读取状态::成功)
                return {L1动态内部::映射中性读取状态(读取.状态), std::nullopt};
            if (读取.读取事实代次 != G0.事实代次)
                return {L1动态状态::版本漂移, std::nullopt};
            if (!读取.事实 || 读取.事实->编码 != 编码组[i]
                || 读取.事实->退出事实代次
                || !L1动态内部::中性节点类型符合(*读取.事实, i))
                return {L1动态状态::内部不一致, std::nullopt};
            节点组[i] = *读取.事实;
        }

        const auto G1 = L1_.读取中性当前事实代次({L1中性CRUD合同版本});
        if (G1.合同版本 != L1中性CRUD合同版本)
            return {L1动态状态::内部不一致, std::nullopt};
        if (G1.状态 != L1中性读取状态::成功)
            return {L1动态内部::映射中性读取状态(G1.状态), std::nullopt};
        if (G1.事实代次 == 0)
            return {L1动态状态::内部不一致, std::nullopt};
        if (G1.事实代次 != G0.事实代次)
            return {L1动态状态::版本漂移, std::nullopt};

        L1动态结构登记 登记;
        登记.首次幂等身份 = 身份;
        登记.事实截止代次 = G0.事实代次;
        登记.服务身份 = 节点组[0].编码;
        登记.关系20动态组成关系类型 = 节点组[1].编码;
        登记.动态资格属性类型 = 节点组[2].编码;
        登记.动态种类属性类型 = 节点组[3].编码;
        登记.聚合规则版本属性类型 = 节点组[4].编码;
        登记.时间范围属性类型 = 节点组[5].编码;
        登记.迁移比较合同属性类型 = 节点组[6].编码;
        登记.迁移差异I64属性类型 = 节点组[7].编码;
        登记.比较输入引用属性类型 = 节点组[8].编码;
        登记.比较输入I64组属性类型 = 节点组[9].编码;
        if (!L1动态结构登记完整(登记))
            return {L1动态状态::内部不一致, std::nullopt};
        return {L1动态状态::已读取, 登记};
    }

    L1事实基座服务& L1_;
    mutable std::recursive_mutex 锁_;
    std::optional<L1动态结构登记> 登记_;
};

} // namespace 海中鱼巣
