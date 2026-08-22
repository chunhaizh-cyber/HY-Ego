module;

#include <algorithm>
#include <cstdint>
#include <new>
#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>

#define 方法静态能力准入数据_NO_INCLUDES

export module 海中鱼巣.领域.内部治理.服务.方法静态能力候选准入;

import 海中鱼巣.领域.服务.L2方法结构;

export {
#include "方法静态能力准入.数据.h"
}

namespace 海中鱼巣::方法静态能力准入内部 {

方法静态能力准入状态 映射状态(L2结构状态 状态) noexcept {
    switch (状态) {
    case L2结构状态::数量预算不足:
        return 方法静态能力准入状态::数量预算不足;
    case L2结构状态::事实代次漂移:
        return 方法静态能力准入状态::当前性漂移;
    case L2结构状态::引用冲突:
    case L2结构状态::未找到:
    case L2结构状态::已退出:
    case L2结构状态::属性未设置:
        return 方法静态能力准入状态::引用冲突;
    case L2结构状态::入口拒绝:
    case L2结构状态::许可拒绝:
    case L2结构状态::未实现:
        return 方法静态能力准入状态::入口拒绝;
    case L2结构状态::资源失败:
        return 方法静态能力准入状态::资源失败;
    case L2结构状态::已读取:
    case L2结构状态::已提交:
    case L2结构状态::精确重复:
    case L2结构状态::幂等冲突:
    case L2结构状态::内部不一致:
    default:
        return 方法静态能力准入状态::内部错误;
    }
}

方法静态能力准入结果 失败(方法静态能力准入状态 状态) noexcept {
    return {状态, std::nullopt, 0, 0, {}, {}};
}

bool 缺口小于(const L2方法结构缺口& 左,
    const L2方法结构缺口& 右) noexcept {
    if (左.类型 != 右.类型)
        return static_cast<std::uint8_t>(左.类型)
            < static_cast<std::uint8_t>(右.类型);
    const auto 左条件 = 左.条件 ? 左.条件->值.值 : 0;
    const auto 右条件 = 右.条件 ? 右.条件->值.值 : 0;
    if (左条件 != 右条件) return 左条件 < 右条件;
    const auto 左结果 = 左.结果 ? 左.结果->值.值 : 0;
    const auto 右结果 = 右.结果 ? 右.结果->值.值 : 0;
    return 左结果 < 右结果;
}

} // namespace 海中鱼巣::方法静态能力准入内部

export namespace 海中鱼巣 {

class 方法静态能力候选准入提供者 final {
public:
    explicit 方法静态能力候选准入提供者(
        const L2方法结构服务& 方法服务) noexcept
        : 方法服务_(方法服务) {}

    方法静态能力准入结果 读取匹配候选(
        const 方法静态能力准入请求& 请求) const noexcept {
        using namespace 方法静态能力准入内部;
        if (!方法静态能力准入请求有效(请求))
            return 失败(方法静态能力准入状态::入口拒绝);
        try {
            const L2方法用途粗召回请求 粗召回请求{
                请求.请求头, L2普通方法结构合同版本, 请求.读取类别,
                请求.历史截止事实代次,
                {请求.需求.目标对象类型, 请求.需求.可改变特征类型,
                    请求.需求.变化方向, 请求.需求.结果类型},
                请求.最大扫描用途事实数, 请求.最大粗召回方法数};
            const auto 粗召回 = 方法服务_.按用途读取方法粗召回组(粗召回请求);
            if (!粗召回.成功()) return 失败(映射状态(粗召回.结果头.状态));
            const auto 共同截止 = 请求.读取类别 == L2读取类别::当前
                ? 请求.请求头.期望事实代次 : 请求.历史截止事实代次;
            if (粗召回.结果头.事实截止代次 != 共同截止)
                return 失败(方法静态能力准入状态::当前性漂移);

            std::vector<L2方法身份> 已读取方法;
            std::vector<方法静态能力候选> 候选;
            std::vector<方法结构不完整粗召回项> 不完整;
            for (const auto& 粗项 : 粗召回.粗召回) {
                if (std::find(已读取方法.begin(), 已读取方法.end(), 粗项.方法)
                    != 已读取方法.end()) continue;
                已读取方法.push_back(粗项.方法);
                const auto 投影读取 = 方法服务_.读取方法六项静态能力投影组({
                    请求.请求头, L2普通方法结构合同版本, 请求.读取类别,
                    粗项.方法, 请求.历史截止事实代次});
                if (!投影读取.成功()) return 失败(映射状态(投影读取.结果头.状态));
                if (投影读取.结果头.事实截止代次 != 共同截止)
                    return 失败(方法静态能力准入状态::当前性漂移);
                if (!投影读取.结构缺口.empty()) {
                    auto 缺口 = 投影读取.结构缺口;
                    std::sort(缺口.begin(), 缺口.end(), 缺口小于);
                    缺口.erase(std::unique(缺口.begin(), 缺口.end()), 缺口.end());
                    不完整.push_back({粗项.方法, std::move(缺口)});
                }
                for (const auto& 投影 : 投影读取.投影) {
                    if (!L2方法六项静态能力签名相同(投影.签名, 请求.需求))
                        continue;
                    候选.push_back({投影.方法, 投影.内容版本, 投影.规格版本,
                        投影.主轴结果, 投影.匹配用途, 投影.签名});
                }
            }
            std::sort(候选.begin(), 候选.end(), [](const auto& 左,
                const auto& 右) {
                if (左.方法.值.值 != 右.方法.值.值)
                    return 左.方法.值.值 < 右.方法.值.值;
                if (左.主轴结果.值.值 != 右.主轴结果.值.值)
                    return 左.主轴结果.值.值 < 右.主轴结果.值.值;
                return 左.匹配用途.值.值 < 右.匹配用途.值.值;
            });
            std::vector<方法静态能力候选> 规范候选;
            规范候选.reserve(候选.size());
            for (auto& 当前 : 候选) {
                if (!规范候选.empty()) {
                    const auto& 前 = 规范候选.back();
                    if (前.方法 == 当前.方法 && 前.内容版本 == 当前.内容版本
                        && 前.规格版本 == 当前.规格版本
                        && 前.主轴结果 == 当前.主轴结果) {
                        if (L2方法六项静态能力签名相同(前.签名, 当前.签名))
                            continue;
                        return 失败(方法静态能力准入状态::内部错误);
                    }
                }
                规范候选.push_back(std::move(当前));
            }
            候选 = std::move(规范候选);
            std::sort(不完整.begin(), 不完整.end(), [](const auto& 左,
                const auto& 右) { return 左.方法.值.值 < 右.方法.值.值; });
            if (std::adjacent_find(不完整.begin(), 不完整.end(),
                    [](const auto& 左, const auto& 右) {
                        return 左.方法 == 右.方法;
                    }) != 不完整.end())
                return 失败(方法静态能力准入状态::内部错误);

            const auto 结论 = !候选.empty()
                ? (!不完整.empty()
                    ? 方法静态能力准入结论::有匹配且有结构不完整
                    : 方法静态能力准入结论::有匹配候选)
                : (!不完整.empty()
                    ? 方法静态能力准入结论::仅结构不完整
                    : 方法静态能力准入结论::匹配为空);
            方法静态能力准入结果 结果{方法静态能力准入状态::已读取,
                结论, 共同截止, 粗召回.实际扫描用途事实数,
                std::move(候选), std::move(不完整)};
            return 结果.成功() ? 结果
                : 失败(方法静态能力准入状态::内部错误);
        } catch (const std::bad_alloc&) {
            return 失败(方法静态能力准入状态::资源失败);
        } catch (const std::length_error&) {
            return 失败(方法静态能力准入状态::资源失败);
        } catch (...) {
            return 失败(方法静态能力准入状态::内部错误);
        }
    }

private:
    const L2方法结构服务& 方法服务_;
};

} // namespace 海中鱼巣
