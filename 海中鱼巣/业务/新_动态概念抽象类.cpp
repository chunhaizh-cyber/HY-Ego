#include "新_动态概念抽象类.h"

#include <algorithm>
#include <limits>
#include <tuple>

namespace 海中鱼巣 {
namespace {

using 候选距离 = std::tuple<std::uint64_t, std::uint64_t, std::uint64_t>;

std::uint64_t 饱和相加(std::uint64_t 左, std::uint64_t 右) noexcept {
    const auto 最大 = std::numeric_limits<std::uint64_t>::max();
    return 左 > 最大 - 右 ? 最大 : 左 + 右;
}

std::uint64_t I64距离(std::int64_t 左, std::int64_t 右) noexcept {
    if ((左 < 0) == (右 < 0)) {
        return 左 >= 右
            ? static_cast<std::uint64_t>(左) - static_cast<std::uint64_t>(右)
            : static_cast<std::uint64_t>(右) - static_cast<std::uint64_t>(左);
    }
    const auto 左绝对值 = 左 < 0
        ? std::uint64_t{0} - static_cast<std::uint64_t>(左)
        : static_cast<std::uint64_t>(左);
    const auto 右绝对值 = 右 < 0
        ? std::uint64_t{0} - static_cast<std::uint64_t>(右)
        : static_cast<std::uint64_t>(右);
    return 饱和相加(左绝对值, 右绝对值);
}

std::optional<std::pair<std::int64_t, std::int64_t>> 读取I64边界(
    const 概念_二次特征类& 二次特征概念服务,
    稳定编码 值域概念节点) {
    const auto 值域 = 二次特征概念服务.获取二次特征概念(
        值域概念节点);
    if (!值域 || 值域->输出特征概念.材料类型
        != 特征概念材料物理类型::I64标量) {
        return std::nullopt;
    }
    const auto* 区间组 = std::get_if<std::vector<特征概念I64闭区间>>(
        &值域->输出特征概念.值域);
    if (!区间组 || 区间组->empty()) return std::nullopt;
    auto 最小值 = 区间组->front().下界;
    auto 最大值 = 区间组->front().上界;
    for (const auto& 区间 : *区间组) {
        最小值 = std::min(最小值, 区间.下界);
        最大值 = std::max(最大值, 区间.上界);
    }
    return std::pair{最小值, 最大值};
}

bool 同结构(
    const 动态概念信息& 左,
    const 动态概念信息& 右) noexcept {
    if (左.状态变化组.size() != 右.状态变化组.size()
        || 左.动态特征组.size() != 右.动态特征组.size()) {
        return false;
    }
    for (std::size_t i = 0; i < 左.状态变化组.size(); ++i) {
        if (左.状态变化组[i].特征类型概念节点
            != 右.状态变化组[i].特征类型概念节点) {
            return false;
        }
    }
    for (std::size_t i = 0; i < 左.动态特征组.size(); ++i) {
        if (左.动态特征组[i].状态间位置
                != 右.动态特征组[i].状态间位置
            || 左.动态特征组[i].二次特征类型概念节点
                != 右.动态特征组[i].二次特征类型概念节点) {
            return false;
        }
    }
    return true;
}

std::optional<候选距离> 计算候选距离(
    const 动态概念信息& 当前,
    const 动态概念信息& 候选,
    const 概念_二次特征类& 二次特征概念服务) {
    if (!同结构(当前, 候选)) return std::nullopt;
    std::uint64_t 变化差异 = 0;
    std::uint64_t 方向差异 = 0;
    std::uint64_t 幅度距离 = 0;
    for (std::size_t i = 0; i < 当前.动态特征组.size(); ++i) {
        const auto& 当前项 = 当前.动态特征组[i];
        const auto& 候选项 = 候选.动态特征组[i];
        if (!当前项.准确结果) return std::nullopt;
        const auto 类型 = 二次特征概念服务.获取二次特征概念(
            当前项.二次特征类型概念节点);
        const auto 候选边界 = 候选项.准确结果
            ? std::optional<std::pair<std::int64_t, std::int64_t>>{
                std::pair{*候选项.准确结果, *候选项.准确结果}}
            : 读取I64边界(
                二次特征概念服务, 候选项.结果值域概念节点);
        if (!类型 || !候选边界) return std::nullopt;

        const auto 当前值 = *当前项.准确结果;
        const auto [候选下界, 候选上界] = *候选边界;
        if (类型->算法 == 二次特征算法::服务任务集合判等) {
            if (当前值 != 0 && 当前值 != 10000) return std::nullopt;
            bool 候选不同 = false;
            bool 候选相同 = false;
            const auto 收集类别 = [&](std::int64_t 值) {
                if (值 == 0) 候选不同 = true;
                else if (值 == 10000) 候选相同 = true;
                else return false;
                return true;
            };
            if (候选项.准确结果) {
                if (!收集类别(*候选项.准确结果)) return std::nullopt;
            } else {
                const auto 域 = 二次特征概念服务.获取二次特征概念(候选项.结果值域概念节点);
                if (!域 || 域->二次特征类型根节点 != 当前项.二次特征类型概念节点)
                    return std::nullopt;
                const auto* 区间组 = std::get_if<std::vector<特征概念I64闭区间>>(
                    &域->输出特征概念.值域);
                if (!区间组 || 区间组->empty()) return std::nullopt;
                for (const auto& 区间 : *区间组) {
                    if (区间.下界 != 区间.上界 || !收集类别(区间.下界)) return std::nullopt;
                }
            }
            const bool 命中 = 当前值 == 10000 ? 候选相同 : 候选不同;
            if (!命中) {
                变化差异 = 饱和相加(变化差异, 1);
                幅度距离 = 饱和相加(幅度距离, 10000);
            }
            // 判等是类别：不按整数正负推导变化方向。
            continue;
        }
        const bool 当前不变 = 类型->算法 == 二次特征算法::特征材料相似度
            ? 当前值 == 10000 : 当前值 == 0;
        const bool 候选可不变 = 类型->算法 == 二次特征算法::特征材料相似度
            ? 候选下界 <= 10000 && 候选上界 >= 10000
            : 候选下界 <= 0 && 候选上界 >= 0;
        if (当前不变 != 候选可不变) {
            变化差异 = 饱和相加(变化差异, 1);
        }

        if (类型->算法 != 二次特征算法::特征材料相似度) {
            const bool 当前正 = 当前值 > 0;
            const bool 当前负 = 当前值 < 0;
            const bool 候选可正 = 候选上界 > 0;
            const bool 候选可负 = 候选下界 < 0;
            if ((当前正 && !候选可正) || (当前负 && !候选可负)) {
                方向差异 = 饱和相加(方向差异, 1);
            }
        }

        if (当前值 < 候选下界) {
            幅度距离 = 饱和相加(
                幅度距离, I64距离(当前值, 候选下界));
        } else if (当前值 > 候选上界) {
            幅度距离 = 饱和相加(
                幅度距离, I64距离(当前值, 候选上界));
        }
    }
    return 候选距离{变化差异, 方向差异, 幅度距离};
}

bool 是包含成功(const 动态概念包含结果& 结果) noexcept {
    return 结果.状态 == 动态概念操作状态::已判断
        && 结果.包含.has_value();
}

} // namespace

新_动态概念抽象类::新_动态概念抽象类(
    新_动态类& 动态服务,
    概念_动态类& 动态概念服务,
    const 概念_二次特征类& 二次特征概念服务,
    const 新_动态特征抽取类& 动态特征抽取服务) noexcept
    : 动态服务_(动态服务),
      动态概念服务_(动态概念服务),
      二次特征概念服务_(二次特征概念服务),
      动态特征抽取服务_(动态特征抽取服务) {}

新动态概念自动抽象结果 新_动态概念抽象类::处理新动态(
    稳定编码 动态节点) noexcept {
    新动态概念自动抽象结果 结果;
    try {
        if (!有效(动态节点)) return 结果;
        结果.动态节点 = 动态节点;
        const auto 动态 = 动态服务_.获取动态(动态节点);
        if (!动态) {
            结果.状态 = 新动态概念自动抽象状态::动态不存在;
            return 结果;
        }
        if (!动态->动态概念节点) {
            结果.状态 = 新动态概念自动抽象状态::实例概念不存在;
            return 结果;
        }
        const auto 初始概念 = 动态概念服务_.获取动态概念(
            *动态->动态概念节点);
        if (!初始概念) {
            结果.状态 = 新动态概念自动抽象状态::实例概念不存在;
            return 结果;
        }
        if (初始概念->种类 == 动态概念种类::聚合) {
            结果.状态 = 新动态概念自动抽象状态::已是聚合概念;
            结果.采用概念节点 = 初始概念->节点;
            return 结果;
        }
        结果.原实例概念节点 = 初始概念->节点;

        const auto 抽取 = 动态特征抽取服务_.抽取({动态节点});
        if (抽取.状态 != 新动态特征抽取状态::已完成) {
            结果.状态 = 新动态概念自动抽象状态::特征抽取失败;
            结果.特征抽取失败状态 = 抽取.状态;
            return 结果;
        }
        const auto 保存 = 动态概念服务_.保存动态特征(
            初始概念->节点, 抽取.特征组);
        if (保存 != 动态概念操作状态::已更新
            && 保存 != 动态概念操作状态::无变化) {
            结果.状态 = 新动态概念自动抽象状态::特征保存失败;
            结果.动态概念失败状态 = 保存;
            return 结果;
        }
        const auto 当前概念 = 动态概念服务_.获取动态概念(初始概念->节点);
        if (!当前概念 || 当前概念->动态特征组.empty()) {
            结果.状态 = 新动态概念自动抽象状态::结构不一致;
            return 结果;
        }

        std::vector<稳定编码> 包含候选;
        const auto 全部概念 = 动态概念服务_.查询全部动态概念();
        for (const auto 候选节点 : 全部概念) {
            if (候选节点 == 当前概念->节点) continue;
            const auto 候选 = 动态概念服务_.获取动态概念(候选节点);
            if (!候选 || 候选->种类 != 动态概念种类::聚合
                || !同结构(*当前概念, *候选)) {
                continue;
            }
            const auto 包含 = 动态概念服务_.判断动态概念包含(
                候选节点, 当前概念->节点);
            if (!是包含成功(包含)) {
                结果.状态 = 新动态概念自动抽象状态::结构不一致;
                结果.动态概念失败状态 = 包含.状态;
                return 结果;
            }
            if (*包含.包含) 包含候选.push_back(候选节点);
        }

        std::vector<稳定编码> 最具体候选;
        for (const auto 候选 : 包含候选) {
            bool 被更窄候选替代 = false;
            for (const auto 其它 : 包含候选) {
                if (其它 == 候选) continue;
                const auto 候选包含其它 = 动态概念服务_.判断动态概念包含(
                    候选, 其它);
                const auto 其它包含候选 = 动态概念服务_.判断动态概念包含(
                    其它, 候选);
                if (!是包含成功(候选包含其它)
                    || !是包含成功(其它包含候选)) {
                    结果.状态 = 新动态概念自动抽象状态::结构不一致;
                    return 结果;
                }
                if (*候选包含其它.包含 && !*其它包含候选.包含) {
                    被更窄候选替代 = true;
                    break;
                }
            }
            if (!被更窄候选替代) 最具体候选.push_back(候选);
        }

        std::optional<稳定编码> 目标聚合概念;
        bool 新形成上位 = false;
        if (最具体候选.size() == 1) {
            const auto 补关系 = 动态概念服务_.聚合共同动态概念(
                {最具体候选.front(), 当前概念->节点});
            if ((补关系.状态 != 动态概念操作状态::已建立
                    && 补关系.状态 != 动态概念操作状态::已复用)
                || 补关系.聚合概念节点 != 最具体候选.front()) {
                结果.状态 = 新动态概念自动抽象状态::聚合失败;
                结果.动态概念失败状态 = 补关系.状态;
                return 结果;
            }
            目标聚合概念 = 最具体候选.front();
        } else if (最具体候选.size() > 1) {
            结果.状态 = 新动态概念自动抽象状态::最具体概念冲突;
            结果.冲突候选 = std::move(最具体候选);
            return 结果;
        } else {
            std::optional<稳定编码> 最近候选;
            std::optional<候选距离> 最近距离;
            for (const auto 候选节点 : 全部概念) {
                if (候选节点 == 当前概念->节点) continue;
                const auto 候选 = 动态概念服务_.获取动态概念(候选节点);
                if (!候选) continue;
                const auto 距离 = 计算候选距离(
                    *当前概念, *候选, 二次特征概念服务_);
                if (!距离) continue;
                if (!最近距离 || *距离 < *最近距离
                    || (*距离 == *最近距离
                        && 候选节点.值 < 最近候选->值)) {
                    最近候选 = 候选节点;
                    最近距离 = 距离;
                }
            }
            if (!最近候选) {
                结果.状态 = 新动态概念自动抽象状态::无同结构候选;
                return 结果;
            }
            const auto 聚合 = 动态概念服务_.聚合共同动态概念(
                {当前概念->节点, *最近候选});
            if ((聚合.状态 != 动态概念操作状态::已建立
                    && 聚合.状态 != 动态概念操作状态::已复用)
                || !聚合.聚合概念节点) {
                结果.状态 = 新动态概念自动抽象状态::聚合失败;
                结果.动态概念失败状态 = 聚合.状态;
                return 结果;
            }
            目标聚合概念 = *聚合.聚合概念节点;
            新形成上位 = true;
        }

        const auto 采用 = 动态服务_.采用聚合动态概念(
            动态节点, *目标聚合概念);
        if (采用 != 新动态操作状态::已完成) {
            结果.状态 = 新动态概念自动抽象状态::采用失败;
            结果.动态采用失败状态 = 采用;
            return 结果;
        }
        结果.状态 = 新形成上位
            ? 新动态概念自动抽象状态::已形成并采用上位概念
            : 新动态概念自动抽象状态::已采用已有概念;
        结果.采用概念节点 = *目标聚合概念;
        return 结果;
    } catch (...) {
        结果.状态 = 新动态概念自动抽象状态::资源失败;
        结果.采用概念节点.reset();
        return 结果;
    }
}

} // namespace 海中鱼巣
