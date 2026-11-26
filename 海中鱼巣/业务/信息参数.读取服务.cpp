#include "信息参数.读取服务.h"
#include "../领域/新_需求类.h"
#include "../领域/新_语言类.h"
#include "../领域/新_存在类.h"
#include <stdexcept>

#include <algorithm>
#include <thread>

namespace 海中鱼巣 {
指代目标保护读取服务::指代目标保护读取服务(const 新_存在类& 存在, const 新_需求类& 需求,
    const 新_特征值类& 特征值, std::span<const 正式概念身份读取接口* const> 概念)
    : 存在_(存在), 需求_(需求), 特征值_(特征值), 概念_(概念.begin(), 概念.end()) {}

新接收指代调用结果<新指代目标保护> 指代目标保护读取服务::读取指代目标保护(
    const 新接收指代关系值& 值) const {
    using S = 新接收指代调用状态;
    using R = 特征概念规则状态;
    using B = 基础结构操作状态;
    using V = 新特征值保护读取状态;
    try {
        新指代目标保护 结果;
        auto 取存在 = [&](稳定编码 节点) -> S {
            const auto 读 = 存在_.读取存在身份保护(节点);
            if (读.前提已变化) return S::前提已变化;
            if (读.状态 == B::资源失败) return S::资源失败;
            if (读.状态 == B::节点不存在) return S::引用失效;
            if (读.状态 == B::入口拒绝) return S::输入拒绝;
            if (读.状态 != B::已完成 || !读.信息
                || 读.信息->存在节点 != 节点 || 读.预期节点组.empty()) return S::结构不一致;
            结果.联合预期_.insert(结果.联合预期_.end(), 读.预期节点组.begin(), 读.预期节点组.end());
            return S::已完成;
        };
        auto 取材料 = [&](稳定编码 节点, const std::vector<稳定编码>* 成员) -> S {
            auto 读 = 特征值_.读取特征值保护(节点);
            switch (读.状态) {
            case V::输入拒绝: case V::非本类节点: return S::输入拒绝;
            case V::未初始化: return S::目标保护未装配;
            case V::节点不存在: return S::引用失效;
            case V::结构不一致: return S::结构不一致;
            case V::资源失败: return S::资源失败;
            case V::已读取: break;
            }
            if (!读.信息 || 读.信息->节点 != 节点) return S::结构不一致;
            if (成员) {
                const auto* 原始 = std::get_if<std::vector<std::uint64_t>>(&读.信息->材料);
                if (读.信息->物理类型 != 材料物理类型::U64数组 || !原始
                    || 原始->size() != 成员->size()) return S::输入拒绝;
                for (std::size_t i = 0; i < 成员->size(); ++i)
                    if (稳定编码{(*原始)[i]} != (*成员)[i]) return S::输入拒绝;
            }
            结果.材料预期_ = std::move(读.窄预期);
            结果.目标_ = 节点;
            return S::已完成;
        };
        if (const auto* 集合 = std::get_if<新接收指代集合值>(&值)) {
            if (!std::is_sorted(集合->成员.begin(), 集合->成员.end())
                || std::adjacent_find(集合->成员.begin(), 集合->成员.end()) != 集合->成员.end())
                return {S::输入拒绝};
            auto 状态 = 取材料(集合->材料节点, &集合->成员);
            if (状态 != S::已完成) return {状态};
            for (const auto 成员 : 集合->成员) {
                状态 = 取存在(成员);
                if (状态 != S::已完成) return {状态};
            }
            结果.成员_ = 集合->成员;
            return {S::已完成, std::move(结果)};
        }
        const auto& 直接 = std::get<新接收指代直接目标>(值);
        if (const auto* 材料 = std::get_if<新指代特征值引用>(&直接)) {
            const auto 状态 = 取材料(材料->节点, nullptr);
            if (状态 != S::已完成) return {状态};
            return {S::已完成, std::move(结果)};
        }
        const auto& 指称 = std::get<新绑定指称>(直接);
        结果.目标_ = 指称.节点;
        if (指称.类别 == 新指称类别::存在) {
            const auto 状态 = 取存在(指称.节点);
            if (状态 != S::已完成) return {状态};
        } else if (指称.类别 == 新指称类别::需求) {
            auto 读 = 需求_.读取需求节点引用身份(指称.节点);
            if (读.状态 == R::值不存在) return {S::引用失效};
            if (读.状态 == R::资源失败) return {S::资源失败};
            if (读.状态 == R::前提已变化) return {S::前提已变化};
            if (读.状态 == R::读取接口未装配) return {S::目标保护未装配};
            if (读.状态 == R::入口拒绝) return {S::输入拒绝};
            if (读.状态 != R::已完成 || !读.身份 || 读.身份->需求节点 != 指称.节点
                || 读.身份保护.empty()) return {S::结构不一致};
            结果.联合预期_ = std::move(读.身份保护);
        } else if (指称.类别 == 新指称类别::概念) {
            if (概念_.empty()) return {S::目标保护未装配};
            bool 依赖未就绪 = false;
            for (const auto* 读取 : 概念_) {
                if (!读取) { 依赖未就绪 = true; continue; }
                auto 读 = 读取->读取正式概念身份保护(指称.节点);
                if (读.状态 == 正式概念读取状态::非本提供者概念) continue;
                if (读.状态 == 正式概念读取状态::依赖未就绪) { 依赖未就绪 = true; continue; }
                if (读.状态 == 正式概念读取状态::前提已变化) return {S::前提已变化};
                if (读.状态 != 正式概念读取状态::已读取 || !读.身份
                    || 读.身份->概念节点 != 指称.节点 || 读.身份->完整保护组.empty()
                    || !结果.联合预期_.empty()) return {S::结构不一致};
                结果.联合预期_ = std::move(读.身份->完整保护组);
            }
            if (结果.联合预期_.empty())
                return {依赖未就绪 ? S::目标保护未装配 : S::引用失效};
        } else {
            return {S::目标保护未装配};
        }
        return {S::已完成, std::move(结果)};
    } catch (const std::bad_alloc&) {
        return {S::资源失败};
    } catch (const std::length_error&) {
        return {S::资源失败};
    }
}

稳定信息对象身份组合读取服务::稳定信息对象身份组合读取服务(新_特征类& 特征,
    const 新信息对象身份读取接口& 词典, const 新信息对象身份读取接口& 语言)
    : 特征_(特征), 词典_(词典), 语言_(语言) {
    特征_.装配信息对象身份读取接口(this);
}
稳定信息对象身份组合读取服务::~稳定信息对象身份组合读取服务() {
    特征_.装配信息对象身份读取接口(nullptr);
}
新信息对象身份读取结果 稳定信息对象身份组合读取服务::读取信息对象身份(
    稳定编码 信息) const noexcept {
    auto 结果 = 词典_.读取信息对象身份(信息);
    if (结果.状态 == 新信息对象身份读取状态::信息不存在)
        return 语言_.读取信息对象身份(信息);
    return 结果;
}

正式信息参数读取服务::正式信息参数读取服务(const 新_需求类& 需求, const 新_语言类& 语言,
    std::array<稳定编码,3> 需求范围, 新词典归类信息概念组 归类范围,
    稳定编码 语言分界范围, 稳定编码 语言参数结构范围) noexcept
    : 需求_(需求),语言_(语言),需求范围_(需求范围),归类范围_(归类范围),
      语言分界范围_(语言分界范围),语言参数结构范围_(语言参数结构范围) {}

正式信息参数读取结果 正式信息参数读取服务::读取正式信息参数保护(
    稳定编码 信息, 稳定编码 范围) const noexcept {
    const bool 是需求 = std::find(需求范围_.begin(),需求范围_.end(),范围) != 需求范围_.end();
    const bool 是语言范围 = (有效(语言分界范围_) && 范围 == 语言分界范围_)
        || (有效(语言参数结构范围_) && 范围 == 语言参数结构范围_);
    if (!是需求 && !是语言范围 && 范围 != 归类范围_.材料范围) return {};
    for (;;) {
        auto 结果 = 是需求 ? 需求_.读取正式信息参数保护(信息,范围)
            : 是语言范围 ? 语言_.读取分界参数保护(信息,范围)
            : 语言_.读取词典归类参数保护(信息,范围,归类范围_);
        if (结果.状态 != 正式信息参数读取状态::前提已变化) return 结果;
        std::this_thread::yield();
    }
}
} // namespace 海中鱼巣
