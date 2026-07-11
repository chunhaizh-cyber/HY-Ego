// 文件规则：本文件只计算规范签名与候选图值材料；不得写入权威结构。
#pragma once

#include "../核心/句柄.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <limits>
#include <optional>
#include <thread>
#include <utility>
#include <vector>

namespace 海中鱼巣 {

enum class 概念根类别 : std::uint32_t {
    未定义 = 0,
    存在 = 1,
    动态 = 2,
    关系 = 3,
    因果 = 4
};

enum class 概念约束维度 : std::uint32_t {
    未定义 = 0,
    特征原始值 = 1,
    动态目标类型 = 2,
    动态改变前值 = 3,
    动态改变后值 = 4,
    动态来源动作类型 = 5,
    关系角色类型 = 6,
    因果动态目标类型 = 7,
    因果动态改变前值 = 8,
    因果动态改变后值 = 9,
    因果来源动作类型 = 10
};

enum class 概念约束值类型 : std::uint32_t {
    未定义 = 0,
    I64 = 1,
    VecI64 = 2,
    VecU64 = 3,
    节点类型值 = 4,
    稳定句柄 = 5
};

struct 概念约束材料 {
    概念约束维度 维度 = 概念约束维度::未定义;
    std::uint64_t 角色序号 = 0;
    节点句柄 定义节点;
    概念约束值类型 值类型 = 概念约束值类型::未定义;
    节点类型 节点类型值 = 节点类型::未分类;
    std::int64_t I64值 = 0;
    std::vector<std::int64_t> VecI64值;
    std::vector<std::uint64_t> VecU64值;

    bool 完整() const {
        if (维度 == 概念约束维度::未定义 || 值类型 == 概念约束值类型::未定义) {
            return false;
        }
        const bool 特征维度定义完整 = 维度 != 概念约束维度::特征原始值 || 句柄有效(定义节点);
        if (!特征维度定义完整) {
            return false;
        }
        switch (值类型) {
        case 概念约束值类型::I64:
            return VecI64值.empty() && VecU64值.empty();
        case 概念约束值类型::VecI64:
            return !VecI64值.empty() && VecU64值.empty();
        case 概念约束值类型::VecU64:
            return VecI64值.empty() && !VecU64值.empty();
        case 概念约束值类型::节点类型值:
            return 节点类型值 != 节点类型::未分类 && VecI64值.empty() && VecU64值.empty();
        case 概念约束值类型::稳定句柄:
            return 句柄有效(定义节点) && VecI64值.empty() && VecU64值.empty();
        default:
            return false;
        }
    }
};

struct 概念签名材料 {
    概念根类别 类别 = 概念根类别::未定义;
    std::uint32_t 签名版本 = 1;
    std::vector<概念约束材料> 约束组;

    bool 完整() const {
        if (类别 < 概念根类别::存在 || 类别 > 概念根类别::因果 || 签名版本 == 0) {
            return false;
        }
        return std::all_of(约束组.begin(), 约束组.end(), [](const auto& 约束) {
            return 约束.完整();
        });
    }
};

struct 概念候选根材料 {
    概念根类别 类别 = 概念根类别::未定义;
    节点句柄 根节点;
};

struct 概念候选输入材料 {
    概念签名材料 签名;
    节点句柄 来源实例;
    std::vector<节点句柄> 定义材料组;
    节点句柄 来源动态;
};

enum class 概念候选操作类型 : std::uint32_t {
    未定义 = 0,
    复用根 = 1,
    创建概念 = 2
};

struct 概念候选项 {
    std::uint64_t 候选编号 = 0;
    概念候选操作类型 操作 = 概念候选操作类型::未定义;
    概念签名材料 签名;
    节点句柄 现有概念;
    std::vector<节点句柄> 来源实例组;
    std::vector<节点句柄> 定义材料组;
    节点句柄 来源动态;

    bool 完整() const {
        if (候选编号 == 0 || !签名.完整()) {
            return false;
        }
        if (操作 == 概念候选操作类型::复用根) {
            return 句柄有效(现有概念) && 签名.约束组.empty();
        }
        return 操作 == 概念候选操作类型::创建概念
            && !句柄有效(现有概念)
            && !签名.约束组.empty()
            && !来源实例组.empty();
    }
};

struct 概念候选直接边 {
    std::uint64_t 上位候选编号 = 0;
    std::uint64_t 下位候选编号 = 0;
};

struct 概念签名编号材料 {
    std::uint64_t 项编号 = 0;
    概念签名材料 签名;

    bool 完整() const {
        return 项编号 != 0 && 签名.完整();
    }
};

enum class 完整签名直接边计算状态 : std::uint32_t {
    输入拒绝 = 0,
    已完成 = 1,
    待重算 = 2
};

struct 完整签名直接边计算材料 {
    完整签名直接边计算状态 状态 = 完整签名直接边计算状态::输入拒绝;
    std::vector<概念候选直接边> 直接边组;
    std::size_t 输入项数量 = 0;
    std::size_t 所需计算预算 = 0;

    bool 已完成() const {
        return 状态 == 完整签名直接边计算状态::已完成;
    }
};

struct 概念候选实例支持边 {
    节点句柄 实例;
    std::uint64_t 概念候选编号 = 0;
};

struct 概念候选定义材料边 {
    std::uint64_t 概念候选编号 = 0;
    节点句柄 定义材料;
};

enum class 待重算原因 : std::uint32_t {
    未定义 = 0,
    候选数量预算耗尽 = 1,
    迭代预算耗尽 = 2
};

struct 待重算请求材料 {
    std::vector<概念根类别> 受影响根类别组;
    std::vector<节点句柄> 起点实例组;
    std::size_t 候选数量预算 = 0;
    std::size_t 迭代预算 = 0;
    待重算原因 原因 = 待重算原因::未定义;

    bool 完整() const {
        return !受影响根类别组.empty()
            && 候选数量预算 != 0
            && 原因 != 待重算原因::未定义;
    }
};

enum class 概念候选计算状态 : std::uint32_t {
    输入拒绝 = 0,
    已收敛 = 1,
    待重算 = 2
};

struct 概念图候选版本材料 {
    std::uint64_t 基准活动版本 = 0;
    概念候选计算状态 状态 = 概念候选计算状态::输入拒绝;
    std::vector<概念候选项> 候选项组;
    std::vector<概念候选直接边> 直接边组;
    std::vector<概念候选实例支持边> 实例支持边组;
    std::vector<概念候选定义材料边> 定义材料边组;
    std::size_t 迭代次数 = 0;
    std::optional<待重算请求材料> 待重算材料;

    bool 已收敛() const {
        return 状态 == 概念候选计算状态::已收敛 && !待重算材料.has_value();
    }
};

class 概念图算法 {
public:
    static std::optional<概念签名材料> 规范化概念签名(const 概念签名材料& 输入) {
        if (!输入.完整()) {
            return std::nullopt;
        }
        概念签名材料 结果 = 输入;
        std::sort(结果.约束组.begin(), 结果.约束组.end(), 约束小于);
        std::vector<概念约束材料> 规范约束组;
        规范约束组.reserve(结果.约束组.size());
        for (const auto& 约束 : 结果.约束组) {
            if (!规范约束组.empty() && 约束身份相同(规范约束组.back(), 约束)) {
                if (!约束相同(规范约束组.back(), 约束)) {
                    return std::nullopt;
                }
                continue;
            }
            规范约束组.push_back(约束);
        }
        结果.约束组 = std::move(规范约束组);
        return 结果;
    }

    static bool 概念签名等价(const 概念签名材料& 左, const 概念签名材料& 右) {
        const auto 左规范 = 规范化概念签名(左);
        const auto 右规范 = 规范化概念签名(右);
        return 左规范.has_value()
            && 右规范.has_value()
            && 签名相同(左规范.value(), 右规范.value());
    }

    static bool 概念签名更一般(const 概念签名材料& 一般, const 概念签名材料& 具体) {
        const auto 一般规范 = 规范化概念签名(一般);
        const auto 具体规范 = 规范化概念签名(具体);
        if (!一般规范.has_value()
            || !具体规范.has_value()
            || !签名更一般_已规范化(一般规范.value(), 具体规范.value())) {
            return false;
        }
        return true;
    }

    static 完整签名直接边计算材料 重算完整签名直接边(
        const std::vector<概念签名编号材料>& 完整签名组,
        std::size_t 计算预算) {
        完整签名直接边计算材料 输出;
        输出.输入项数量 = 完整签名组.size();
        if (完整签名组.empty()) {
            return 输出;
        }

        std::vector<概念签名编号材料> 规范签名组;
        规范签名组.reserve(完整签名组.size());
        for (const auto& 材料 : 完整签名组) {
            const auto 规范签名 = 规范化概念签名(材料.签名);
            if (材料.项编号 == 0 || !规范签名.has_value()) {
                return 输出;
            }
            for (const auto& 已有 : 规范签名组) {
                if (已有.项编号 == 材料.项编号
                    || 签名相同(已有.签名, 规范签名.value())) {
                    return 输出;
                }
            }
            规范签名组.push_back({材料.项编号, 规范签名.value()});
        }

        const auto 所需预算 = 计算直接边所需预算(规范签名组.size());
        if (!所需预算.has_value()) {
            输出.状态 = 完整签名直接边计算状态::待重算;
            输出.所需计算预算 = std::numeric_limits<std::size_t>::max();
            return 输出;
        }
        输出.所需计算预算 = 所需预算.value();
        if (计算预算 < 输出.所需计算预算) {
            输出.状态 = 完整签名直接边计算状态::待重算;
            return 输出;
        }

        const auto 项数量 = 规范签名组.size();
        std::vector<std::uint8_t> 更一般矩阵(项数量 * 项数量, 0);
        const auto 填充行 = [&](std::size_t 起始行, std::size_t 结束行) {
            for (std::size_t 上位索引 = 起始行; 上位索引 < 结束行; ++上位索引) {
                for (std::size_t 下位索引 = 0; 下位索引 < 项数量; ++下位索引) {
                    if (上位索引 == 下位索引) {
                        continue;
                    }
                    更一般矩阵[上位索引 * 项数量 + 下位索引] =
                        签名更一般_已规范化(
                            规范签名组[上位索引].签名,
                            规范签名组[下位索引].签名)
                        ? 1
                        : 0;
                }
            }
        };

        constexpr std::size_t 并行计算阈值 = 8;
        const auto 硬件线程数 = static_cast<std::size_t>(std::thread::hardware_concurrency());
        const auto 工作线程数 = 项数量 >= 并行计算阈值 && 硬件线程数 > 1
            ? std::min(项数量, 硬件线程数)
            : std::size_t{1};
        if (工作线程数 == 1) {
            填充行(0, 项数量);
        }
        else {
            std::vector<std::thread> 线程组;
            线程组.reserve(工作线程数 - 1);
            const auto 每线程行数 = (项数量 + 工作线程数 - 1) / 工作线程数;
            for (std::size_t 线程索引 = 1; 线程索引 < 工作线程数; ++线程索引) {
                const auto 起始行 = 线程索引 * 每线程行数;
                const auto 结束行 = std::min(项数量, 起始行 + 每线程行数);
                if (起始行 >= 结束行) {
                    break;
                }
                线程组.emplace_back(填充行, 起始行, 结束行);
            }
            填充行(0, std::min(项数量, 每线程行数));
            for (auto& 线程 : 线程组) {
                线程.join();
            }
        }

        std::vector<std::vector<概念候选直接边>> 每行直接边组(项数量);
        const auto 约简行 = [&](std::size_t 起始行, std::size_t 结束行) {
            for (std::size_t 上位索引 = 起始行; 上位索引 < 结束行; ++上位索引) {
                auto& 当前行 = 每行直接边组[上位索引];
                for (std::size_t 下位索引 = 0; 下位索引 < 项数量; ++下位索引) {
                    if (更一般矩阵[上位索引 * 项数量 + 下位索引] == 0) {
                        continue;
                    }
                    bool 存在中间 = false;
                    for (std::size_t 中间索引 = 0; 中间索引 < 项数量; ++中间索引) {
                        if (中间索引 == 上位索引 || 中间索引 == 下位索引) {
                            continue;
                        }
                        if (更一般矩阵[上位索引 * 项数量 + 中间索引] != 0
                            && 更一般矩阵[中间索引 * 项数量 + 下位索引] != 0) {
                            存在中间 = true;
                            break;
                        }
                    }
                    if (!存在中间) {
                        当前行.push_back({
                            规范签名组[上位索引].项编号,
                            规范签名组[下位索引].项编号});
                    }
                }
            }
        };
        if (工作线程数 == 1) {
            约简行(0, 项数量);
        }
        else {
            std::vector<std::thread> 线程组;
            线程组.reserve(工作线程数 - 1);
            const auto 每线程行数 = (项数量 + 工作线程数 - 1) / 工作线程数;
            for (std::size_t 线程索引 = 1; 线程索引 < 工作线程数; ++线程索引) {
                const auto 起始行 = 线程索引 * 每线程行数;
                const auto 结束行 = std::min(项数量, 起始行 + 每线程行数);
                if (起始行 >= 结束行) {
                    break;
                }
                线程组.emplace_back(约简行, 起始行, 结束行);
            }
            约简行(0, std::min(项数量, 每线程行数));
            for (auto& 线程 : 线程组) {
                线程.join();
            }
        }
        for (auto& 当前行 : 每行直接边组) {
            输出.直接边组.insert(
                输出.直接边组.end(),
                std::make_move_iterator(当前行.begin()),
                std::make_move_iterator(当前行.end()));
        }
        输出.状态 = 完整签名直接边计算状态::已完成;
        return 输出;
    }

    static std::optional<概念签名材料> 提取共同一般签名(
        const 概念签名材料& 左,
        const 概念签名材料& 右) {
        const auto 左规范 = 规范化概念签名(左);
        const auto 右规范 = 规范化概念签名(右);
        if (!左规范.has_value()
            || !右规范.has_value()
            || 左规范->类别 != 右规范->类别
            || 左规范->签名版本 != 右规范->签名版本) {
            return std::nullopt;
        }
        概念签名材料 结果;
        结果.类别 = 左规范->类别;
        结果.签名版本 = 左规范->签名版本;
        std::set_intersection(
            左规范->约束组.begin(), 左规范->约束组.end(),
            右规范->约束组.begin(), 右规范->约束组.end(),
            std::back_inserter(结果.约束组),
            约束小于);
        return 结果;
    }

    static 概念图候选版本材料 计算固定点候选(
        std::uint64_t 基准活动版本,
        const std::vector<概念候选根材料>& 根材料组,
        const std::vector<概念候选输入材料>& 输入组,
        std::size_t 候选数量预算,
        std::size_t 迭代预算) {
        概念图候选版本材料 输出;
        输出.基准活动版本 = 基准活动版本;
        if (根材料组.empty() || 候选数量预算 == 0 || !根材料有效且唯一(根材料组)) {
            return 输出;
        }

        std::vector<概念候选项> 工作候选组;
        工作候选组.reserve(根材料组.size() + 输入组.size());
        for (const auto& 根 : 根材料组) {
            概念候选项 候选;
            候选.操作 = 概念候选操作类型::复用根;
            候选.签名.类别 = 根.类别;
            候选.现有概念 = 根.根节点;
            工作候选组.push_back(std::move(候选));
        }
        if (工作候选组.size() > 候选数量预算) {
            return 形成待重算输出(
                输出, 根材料组, 输入组, 候选数量预算, 迭代预算, 待重算原因::候选数量预算耗尽, 工作候选组);
        }

        for (const auto& 输入 : 输入组) {
            const auto 规范签名 = 规范化概念签名(输入.签名);
            if (!规范签名.has_value()
                || 规范签名->约束组.empty()
                || !句柄有效(输入.来源实例)
                || !存在对应根(根材料组, 规范签名->类别)
                || (规范签名->类别 == 概念根类别::因果 && !句柄有效(输入.来源动态))) {
                return 输出;
            }
            概念候选项 候选;
            候选.操作 = 概念候选操作类型::创建概念;
            候选.签名 = 规范签名.value();
            候选.来源实例组 = {输入.来源实例};
            候选.定义材料组 = 规范化句柄组(输入.定义材料组);
            候选.来源动态 = 输入.来源动态;
            const bool 新增候选 = 合并候选(工作候选组, 候选);
            if (新增候选 && 工作候选组.size() > 候选数量预算) {
                return 形成待重算输出(
                    输出, 根材料组, 输入组, 候选数量预算, 迭代预算, 待重算原因::候选数量预算耗尽, 工作候选组);
            }
        }

        for (const auto& 输入 : 输入组) {
            auto 根候选 = 查找签名候选(工作候选组, 概念签名材料{输入.签名.类别, 输入.签名.签名版本, {}});
            if (根候选.has_value()) {
                合并句柄(工作候选组[根候选.value()].来源实例组, 输入.来源实例);
            }
        }

        if (迭代预算 == 0 && !输入组.empty()) {
            return 形成待重算输出(
                输出, 根材料组, 输入组, 候选数量预算, 迭代预算, 待重算原因::迭代预算耗尽, 工作候选组);
        }

        bool 已收敛 = 输入组.empty();
        for (std::size_t 轮次 = 1; 轮次 <= 迭代预算 && !已收敛; ++轮次) {
            输出.迭代次数 = 轮次;
            std::vector<概念候选项> 本轮候选组;
            for (std::size_t 左索引 = 0; 左索引 < 工作候选组.size(); ++左索引) {
                if (工作候选组[左索引].操作 != 概念候选操作类型::创建概念) {
                    continue;
                }
                for (std::size_t 右索引 = 左索引 + 1; 右索引 < 工作候选组.size(); ++右索引) {
                    if (工作候选组[右索引].操作 != 概念候选操作类型::创建概念) {
                        continue;
                    }
                    const auto 共同签名 = 提取共同一般签名(
                        工作候选组[左索引].签名, 工作候选组[右索引].签名);
                    if (!共同签名.has_value() || 共同签名->约束组.empty()) {
                        continue;
                    }
                    概念候选项 共同候选;
                    共同候选.操作 = 概念候选操作类型::创建概念;
                    共同候选.签名 = 共同签名.value();
                    共同候选.来源实例组 = 合并句柄组副本(
                        工作候选组[左索引].来源实例组, 工作候选组[右索引].来源实例组);
                    共同候选.定义材料组 = 交集句柄组(
                        工作候选组[左索引].定义材料组, 工作候选组[右索引].定义材料组);
                    共同候选.来源动态 = 选择较小有效句柄(
                        工作候选组[左索引].来源动态, 工作候选组[右索引].来源动态);
                    (void)合并候选(本轮候选组, 共同候选);
                }
            }

            bool 本轮变化 = false;
            std::sort(本轮候选组.begin(), 本轮候选组.end(), 候选小于);
            for (const auto& 新候选 : 本轮候选组) {
                const auto 已有索引 = 查找签名候选(工作候选组, 新候选.签名);
                if (!已有索引.has_value()) {
                    if (工作候选组.size() >= 候选数量预算) {
                        return 形成待重算输出(
                            输出, 根材料组, 输入组, 候选数量预算, 迭代预算,
                            待重算原因::候选数量预算耗尽, 工作候选组);
                    }
                    工作候选组.push_back(新候选);
                    本轮变化 = true;
                    continue;
                }
                auto& 已有 = 工作候选组[已有索引.value()];
                const auto 原来源数量 = 已有.来源实例组.size();
                已有.来源实例组 = 合并句柄组副本(已有.来源实例组, 新候选.来源实例组);
                const auto 新定义组 = 交集句柄组(已有.定义材料组, 新候选.定义材料组);
                const bool 定义变化 = !句柄组相同(已有.定义材料组, 新定义组);
                已有.定义材料组 = 新定义组;
                const auto 新来源动态 = 选择较小有效句柄(已有.来源动态, 新候选.来源动态);
                const bool 动态变化 = !(已有.来源动态 == 新来源动态);
                已有.来源动态 = 新来源动态;
                本轮变化 = 本轮变化
                    || 原来源数量 != 已有.来源实例组.size()
                    || 定义变化
                    || 动态变化;
            }
            已收敛 = !本轮变化;
        }

        if (!已收敛) {
            return 形成待重算输出(
                输出, 根材料组, 输入组, 候选数量预算, 迭代预算, 待重算原因::迭代预算耗尽, 工作候选组);
        }

        输出.状态 = 概念候选计算状态::已收敛;
        if (!整理候选输出(输出, 工作候选组, true)
            || !候选直接边无环(输出)) {
            输出 = 概念图候选版本材料{};
            输出.基准活动版本 = 基准活动版本;
        }
        return 输出;
    }

    static bool 候选直接边无环(const 概念图候选版本材料& 候选图) {
        for (const auto& 边 : 候选图.直接边组) {
            const auto 上位 = 按编号查找候选(候选图.候选项组, 边.上位候选编号);
            const auto 下位 = 按编号查找候选(候选图.候选项组, 边.下位候选编号);
            if (!上位.has_value()
                || !下位.has_value()
                || !概念签名更一般(上位.value()->签名, 下位.value()->签名)) {
                return false;
            }
        }
        return true;
    }

    static bool 候选版本结构等价(
        const 概念图候选版本材料& 左,
        const 概念图候选版本材料& 右) {
        if (左.基准活动版本 != 右.基准活动版本
            || 左.状态 != 右.状态
            || 左.迭代次数 != 右.迭代次数
            || 左.候选项组.size() != 右.候选项组.size()
            || 左.直接边组.size() != 右.直接边组.size()
            || 左.实例支持边组.size() != 右.实例支持边组.size()
            || 左.定义材料边组.size() != 右.定义材料边组.size()
            || 左.待重算材料.has_value() != 右.待重算材料.has_value()) {
            return false;
        }
        for (std::size_t 索引 = 0; 索引 < 左.候选项组.size(); ++索引) {
            if (!候选项相同(左.候选项组[索引], 右.候选项组[索引])) {
                return false;
            }
        }
        for (std::size_t 索引 = 0; 索引 < 左.直接边组.size(); ++索引) {
            if (左.直接边组[索引].上位候选编号 != 右.直接边组[索引].上位候选编号
                || 左.直接边组[索引].下位候选编号 != 右.直接边组[索引].下位候选编号) {
                return false;
            }
        }
        for (std::size_t 索引 = 0; 索引 < 左.实例支持边组.size(); ++索引) {
            if (!(左.实例支持边组[索引].实例 == 右.实例支持边组[索引].实例)
                || 左.实例支持边组[索引].概念候选编号 != 右.实例支持边组[索引].概念候选编号) {
                return false;
            }
        }
        for (std::size_t 索引 = 0; 索引 < 左.定义材料边组.size(); ++索引) {
            if (左.定义材料边组[索引].概念候选编号 != 右.定义材料边组[索引].概念候选编号
                || !(左.定义材料边组[索引].定义材料 == 右.定义材料边组[索引].定义材料)) {
                return false;
            }
        }
        if (左.待重算材料.has_value()) {
            return 待重算材料相同(左.待重算材料.value(), 右.待重算材料.value());
        }
        return true;
    }

private:
    static bool 节点句柄小于(const 节点句柄& 左, const 节点句柄& 右) {
        if (左.仓库编号 != 右.仓库编号) {
            return 左.仓库编号 < 右.仓库编号;
        }
        if (左.节点编号 != 右.节点编号) {
            return 左.节点编号 < 右.节点编号;
        }
        return 左.版本号 < 右.版本号;
    }

    static bool 约束身份相同(const 概念约束材料& 左, const 概念约束材料& 右) {
        return 左.维度 == 右.维度
            && 左.角色序号 == 右.角色序号
            && 左.定义节点 == 右.定义节点;
    }

    static bool 约束相同(const 概念约束材料& 左, const 概念约束材料& 右) {
        return 约束身份相同(左, 右)
            && 左.值类型 == 右.值类型
            && 左.节点类型值 == 右.节点类型值
            && 左.I64值 == 右.I64值
            && 左.VecI64值 == 右.VecI64值
            && 左.VecU64值 == 右.VecU64值;
    }

    static bool 约束小于(const 概念约束材料& 左, const 概念约束材料& 右) {
        if (左.维度 != 右.维度) {
            return 左.维度 < 右.维度;
        }
        if (左.角色序号 != 右.角色序号) {
            return 左.角色序号 < 右.角色序号;
        }
        if (节点句柄小于(左.定义节点, 右.定义节点)) {
            return true;
        }
        if (节点句柄小于(右.定义节点, 左.定义节点)) {
            return false;
        }
        if (左.值类型 != 右.值类型) {
            return 左.值类型 < 右.值类型;
        }
        if (左.节点类型值 != 右.节点类型值) {
            return 左.节点类型值 < 右.节点类型值;
        }
        if (左.I64值 != 右.I64值) {
            return 左.I64值 < 右.I64值;
        }
        if (左.VecI64值 != 右.VecI64值) {
            return std::lexicographical_compare(
                左.VecI64值.begin(), 左.VecI64值.end(), 右.VecI64值.begin(), 右.VecI64值.end());
        }
        return std::lexicographical_compare(
            左.VecU64值.begin(), 左.VecU64值.end(), 右.VecU64值.begin(), 右.VecU64值.end());
    }

    static bool 签名相同(const 概念签名材料& 左, const 概念签名材料& 右) {
        if (左.类别 != 右.类别
            || 左.签名版本 != 右.签名版本
            || 左.约束组.size() != 右.约束组.size()) {
            return false;
        }
        for (std::size_t 索引 = 0; 索引 < 左.约束组.size(); ++索引) {
            if (!约束相同(左.约束组[索引], 右.约束组[索引])) {
                return false;
            }
        }
        return true;
    }

    static bool 签名更一般_已规范化(
        const 概念签名材料& 一般,
        const 概念签名材料& 具体) {
        return 一般.类别 == 具体.类别
            && 一般.签名版本 == 具体.签名版本
            && !签名相同(一般, 具体)
            && std::includes(
                具体.约束组.begin(), 具体.约束组.end(),
                一般.约束组.begin(), 一般.约束组.end(),
                约束小于);
    }

    static std::optional<std::size_t> 计算直接边所需预算(std::size_t 项数量) {
        const auto 上限 = std::numeric_limits<std::size_t>::max();
        if (项数量 != 0 && 项数量 > 上限 / 项数量) {
            return std::nullopt;
        }
        const auto 平方预算 = 项数量 * 项数量;
        if (项数量 != 0 && 平方预算 > 上限 / 项数量) {
            return std::nullopt;
        }
        const auto 立方预算 = 平方预算 * 项数量;
        if (平方预算 > 上限 - 立方预算) {
            return std::nullopt;
        }
        return 平方预算 + 立方预算;
    }

    static bool 签名小于(const 概念签名材料& 左, const 概念签名材料& 右) {
        if (左.类别 != 右.类别) {
            return 左.类别 < 右.类别;
        }
        if (左.签名版本 != 右.签名版本) {
            return 左.签名版本 < 右.签名版本;
        }
        return std::lexicographical_compare(
            左.约束组.begin(), 左.约束组.end(), 右.约束组.begin(), 右.约束组.end(), 约束小于);
    }

    static bool 候选小于(const 概念候选项& 左, const 概念候选项& 右) {
        return 签名小于(左.签名, 右.签名);
    }

    static bool 根材料有效且唯一(const std::vector<概念候选根材料>& 根材料组) {
        std::vector<概念根类别> 类别组;
        std::vector<节点句柄> 句柄组;
        for (const auto& 根 : 根材料组) {
            if (根.类别 < 概念根类别::存在
                || 根.类别 > 概念根类别::因果
                || !句柄有效(根.根节点)
                || std::find(类别组.begin(), 类别组.end(), 根.类别) != 类别组.end()
                || std::find(句柄组.begin(), 句柄组.end(), 根.根节点) != 句柄组.end()) {
                return false;
            }
            类别组.push_back(根.类别);
            句柄组.push_back(根.根节点);
        }
        return true;
    }

    static bool 存在对应根(const std::vector<概念候选根材料>& 根材料组, 概念根类别 类别) {
        return std::any_of(根材料组.begin(), 根材料组.end(), [类别](const auto& 根) {
            return 根.类别 == 类别;
        });
    }

    static std::vector<节点句柄> 规范化句柄组(const std::vector<节点句柄>& 输入) {
        std::vector<节点句柄> 结果;
        for (const auto& 句柄 : 输入) {
            if (句柄有效(句柄)) {
                结果.push_back(句柄);
            }
        }
        std::sort(结果.begin(), 结果.end(), 节点句柄小于);
        结果.erase(std::unique(结果.begin(), 结果.end()), 结果.end());
        return 结果;
    }

    static bool 句柄组相同(const std::vector<节点句柄>& 左, const std::vector<节点句柄>& 右) {
        return 左 == 右;
    }

    static bool 合并句柄(std::vector<节点句柄>& 目标, 节点句柄 值) {
        if (!句柄有效(值) || std::find(目标.begin(), 目标.end(), 值) != 目标.end()) {
            return false;
        }
        目标.push_back(值);
        std::sort(目标.begin(), 目标.end(), 节点句柄小于);
        return true;
    }

    static std::vector<节点句柄> 合并句柄组副本(
        const std::vector<节点句柄>& 左,
        const std::vector<节点句柄>& 右) {
        auto 结果 = 规范化句柄组(左);
        for (const auto& 值 : 右) {
            (void)合并句柄(结果, 值);
        }
        return 结果;
    }

    static std::vector<节点句柄> 交集句柄组(
        const std::vector<节点句柄>& 左,
        const std::vector<节点句柄>& 右) {
        const auto 左规范 = 规范化句柄组(左);
        const auto 右规范 = 规范化句柄组(右);
        std::vector<节点句柄> 结果;
        std::set_intersection(
            左规范.begin(), 左规范.end(), 右规范.begin(), 右规范.end(),
            std::back_inserter(结果), 节点句柄小于);
        return 结果;
    }

    static 节点句柄 选择较小有效句柄(节点句柄 左, 节点句柄 右) {
        if (!句柄有效(左)) {
            return 右;
        }
        if (!句柄有效(右)) {
            return 左;
        }
        return 节点句柄小于(左, 右) ? 左 : 右;
    }

    static std::optional<std::size_t> 查找签名候选(
        const std::vector<概念候选项>& 候选组,
        const 概念签名材料& 签名) {
        const auto 规范签名 = 规范化概念签名(签名);
        if (!规范签名.has_value()) {
            return std::nullopt;
        }
        for (std::size_t 索引 = 0; 索引 < 候选组.size(); ++索引) {
            if (签名相同(候选组[索引].签名, 规范签名.value())) {
                return 索引;
            }
        }
        return std::nullopt;
    }

    static bool 合并候选(std::vector<概念候选项>& 候选组, const 概念候选项& 新候选) {
        const auto 已有索引 = 查找签名候选(候选组, 新候选.签名);
        if (!已有索引.has_value()) {
            候选组.push_back(新候选);
            return true;
        }
        auto& 已有 = 候选组[已有索引.value()];
        已有.来源实例组 = 合并句柄组副本(已有.来源实例组, 新候选.来源实例组);
        已有.定义材料组 = 交集句柄组(已有.定义材料组, 新候选.定义材料组);
        已有.来源动态 = 选择较小有效句柄(已有.来源动态, 新候选.来源动态);
        return false;
    }

    static 待重算请求材料 构造待重算材料(
        const std::vector<概念候选根材料>& 根材料组,
        const std::vector<概念候选输入材料>& 输入组,
        std::size_t 候选数量预算,
        std::size_t 迭代预算,
        待重算原因 原因) {
        待重算请求材料 材料;
        材料.候选数量预算 = 候选数量预算;
        材料.迭代预算 = 迭代预算;
        材料.原因 = 原因;
        for (const auto& 根 : 根材料组) {
            材料.受影响根类别组.push_back(根.类别);
        }
        std::sort(材料.受影响根类别组.begin(), 材料.受影响根类别组.end());
        材料.受影响根类别组.erase(
            std::unique(材料.受影响根类别组.begin(), 材料.受影响根类别组.end()),
            材料.受影响根类别组.end());
        for (const auto& 输入 : 输入组) {
            (void)合并句柄(材料.起点实例组, 输入.来源实例);
        }
        return 材料;
    }

    static 概念图候选版本材料 形成待重算输出(
        概念图候选版本材料 输出,
        const std::vector<概念候选根材料>& 根材料组,
        const std::vector<概念候选输入材料>& 输入组,
        std::size_t 候选数量预算,
        std::size_t 迭代预算,
        待重算原因 原因,
        std::vector<概念候选项> 工作候选组) {
        输出.状态 = 概念候选计算状态::待重算;
        输出.待重算材料 = 构造待重算材料(
            根材料组, 输入组, 候选数量预算, 迭代预算, 原因);
        (void)整理候选输出(输出, 工作候选组, false);
        return 输出;
    }

    static bool 整理候选输出(
        概念图候选版本材料& 输出,
        std::vector<概念候选项>& 工作候选组,
        bool 生成直接边) {
        for (auto& 候选 : 工作候选组) {
            候选.来源实例组 = 规范化句柄组(候选.来源实例组);
            候选.定义材料组 = 规范化句柄组(候选.定义材料组);
        }
        std::sort(工作候选组.begin(), 工作候选组.end(), 候选小于);
        for (std::size_t 索引 = 0; 索引 < 工作候选组.size(); ++索引) {
            工作候选组[索引].候选编号 = static_cast<std::uint64_t>(索引 + 1);
        }
        输出.候选项组 = 工作候选组;
        输出.直接边组.clear();
        输出.实例支持边组.clear();
        输出.定义材料边组.clear();
        for (const auto& 候选 : 输出.候选项组) {
            for (const auto& 实例 : 候选.来源实例组) {
                输出.实例支持边组.push_back({实例, 候选.候选编号});
            }
            for (const auto& 材料 : 候选.定义材料组) {
                输出.定义材料边组.push_back({候选.候选编号, 材料});
            }
        }
        if (!生成直接边) {
            return true;
        }
        std::vector<概念签名编号材料> 完整签名组;
        完整签名组.reserve(输出.候选项组.size());
        for (const auto& 候选 : 输出.候选项组) {
            完整签名组.push_back({候选.候选编号, 候选.签名});
        }
        const auto 直接边结果 = 重算完整签名直接边(
            完整签名组,
            std::numeric_limits<std::size_t>::max());
        if (!直接边结果.已完成()) {
            return false;
        }
        输出.直接边组 = 直接边结果.直接边组;
        return true;
    }

    static std::optional<const 概念候选项*> 按编号查找候选(
        const std::vector<概念候选项>& 候选组,
        std::uint64_t 编号) {
        for (const auto& 候选 : 候选组) {
            if (候选.候选编号 == 编号) {
                return &候选;
            }
        }
        return std::nullopt;
    }

    static bool 候选项相同(const 概念候选项& 左, const 概念候选项& 右) {
        return 左.候选编号 == 右.候选编号
            && 左.操作 == 右.操作
            && 签名相同(左.签名, 右.签名)
            && 左.现有概念 == 右.现有概念
            && 左.来源实例组 == 右.来源实例组
            && 左.定义材料组 == 右.定义材料组
            && 左.来源动态 == 右.来源动态;
    }

    static bool 待重算材料相同(const 待重算请求材料& 左, const 待重算请求材料& 右) {
        return 左.受影响根类别组 == 右.受影响根类别组
            && 左.起点实例组 == 右.起点实例组
            && 左.候选数量预算 == 右.候选数量预算
            && 左.迭代预算 == 右.迭代预算
            && 左.原因 == 右.原因;
    }
};

}
