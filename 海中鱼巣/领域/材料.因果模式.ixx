// 文件规则：因果模式材料只承载非权威机会、结果证据和整数概率候选；不得承载事务能力或稳定因果事实。
module;

#include <cstddef>
#include <cstdint>
#include <limits>
#include <vector>

export module 海中鱼巣.领域.材料.因果模式;

import 海中鱼巣.领域.材料.动态模式;

export namespace 海中鱼巣 {

inline constexpr std::uint32_t 因果模式规则版本 = 1;
inline constexpr std::uint64_t 因果概率满量程 = 10'000;
inline constexpr std::size_t 因果模式最大机会数量 = 动态聚类最大步骤数量;

enum class 因果模式处理状态 : std::uint8_t {
    已形成 = 1,
    入口拒绝 = 2,
    不支持 = 3,
    内部不一致 = 4
};

enum class 因果模式拒绝原因 : std::uint8_t {
    无 = 0,
    请求不完整 = 1,
    容量越界 = 2,
    规则版本不支持 = 3,
    原因结果相同 = 4,
    原因不是重复聚类 = 5,
    机会起点歧义 = 6,
    片段顺序歧义 = 7,
    正式聚类材料矛盾 = 8,
    算法不变量破坏 = 9
};

enum class 因果机会结果分类 : std::uint8_t {
    正结果 = 1,
    负结果 = 2
};

struct 因果模式请求 {
    动态聚类结果 聚类结果;
    动态模式结构键 原因结构键;
    动态模式结构键 结果结构键;
    std::uint32_t 规则版本 = 因果模式规则版本;

    bool 数量有效() const noexcept {
        return !聚类结果.单步实例组.empty()
            && 聚类结果.单步实例组.size() <= 动态聚类最大步骤数量
            && 聚类结果.重复聚类组.size() <= 动态聚类最大步骤数量
            && 聚类结果.运动基元组.size() <= 动态聚类最大步骤数量;
    }
};

struct 因果机会证据 {
    动态模式回合身份 回合;
    单步片段实例候选 原因实例;
    std::vector<单步片段实例候选> 原因前结果组;
    std::vector<单步片段实例候选> 原因后结果组;
    因果机会结果分类 分类 = 因果机会结果分类::负结果;

    bool 完整() const noexcept {
        if (!回合.完整() || !原因实例.完整()
            || 原因前结果组.size() > 动态聚类最大步骤数量
            || 原因后结果组.size() > 动态聚类最大步骤数量) {
            return false;
        }
        for (const auto& 实例 : 原因前结果组) {
            if (!实例.完整()) return false;
        }
        for (const auto& 实例 : 原因后结果组) {
            if (!实例.完整()) return false;
        }
        return (分类 == 因果机会结果分类::正结果 && !原因后结果组.empty())
            || (分类 == 因果机会结果分类::负结果 && 原因后结果组.empty());
    }
};

struct 因果概率候选 {
    动态模式结构键 原因结构键;
    动态模式结构键 结果结构键;
    std::uint32_t 规则版本 = 0;
    std::vector<因果机会证据> 机会证据组;
    std::uint64_t 机会数 = 0;
    std::uint64_t 正结果数 = 0;
    std::uint64_t 负结果数 = 0;
    std::uint64_t Q10000 = 0;

    bool 完整() const noexcept {
        if (!原因结构键.完整() || !结果结构键.完整() || 规则版本 == 0
            || 机会证据组.empty() || 机会证据组.size() > 因果模式最大机会数量
            || 机会数 != 机会证据组.size() || 正结果数 > 机会数
            || 负结果数 > 机会数 || 正结果数 + 负结果数 != 机会数
            || Q10000 > 因果概率满量程
            || 正结果数 > std::numeric_limits<std::uint64_t>::max() / 因果概率满量程) {
            return false;
        }
        for (const auto& 证据 : 机会证据组) {
            if (!证据.完整()) return false;
        }
        return Q10000 == 正结果数 * 因果概率满量程 / 机会数;
    }
};

struct 因果模式结果 {
    因果模式处理状态 状态 = 因果模式处理状态::入口拒绝;
    因果模式拒绝原因 拒绝原因 = 因果模式拒绝原因::请求不完整;
    因果概率候选 候选;

    bool 成功() const noexcept {
        return 状态 == 因果模式处理状态::已形成
            && 拒绝原因 == 因果模式拒绝原因::无
            && 候选.完整();
    }
};

}
