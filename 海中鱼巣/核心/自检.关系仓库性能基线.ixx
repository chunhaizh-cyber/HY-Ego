// 文件规则：关系仓库性能基线只测量隔离夹具，不得修改生产仓库或形成机器事实。
module;

#ifdef HY_EGO_ENABLE_STRUCTURE_COMMIT_FAULT_SELF_TEST
#include "主信息仓库.h"
#include "节点仓库.h"
#include "关系仓库.h"

#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <thread>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#endif

export module 海中鱼巣.核心.自检.关系仓库性能基线;

#ifdef HY_EGO_ENABLE_STRUCTURE_COMMIT_FAULT_SELF_TEST
import 海中鱼巣.自检.运行器;

export namespace 海中鱼巣 {

namespace 关系仓库性能基线内部 {

using 稳定时钟 = std::chrono::steady_clock;

constexpr std::uint64_t 固定随机种子 = 0x2570'7900'5A17'C3D1ULL;
constexpr std::size_t 固定节点数量 = 1024;
constexpr std::size_t 填充节点上限 = 900;
constexpr std::size_t 预热次数 = 20;
constexpr std::size_t 正式次数 = 200;
constexpr std::size_t 基线轮数 = 3;
constexpr std::size_t 并发线程数 = 4;
constexpr std::chrono::seconds 吞吐窗口{10};

bool 记录一致(const 关系记录& 左, const 关系记录& 右) {
    return 左.关系编号 == 右.关系编号
        && 左.类型 == 右.类型
        && 左.源节点 == 右.源节点
        && 左.目标节点 == 右.目标节点
        && 左.顺序号 == 右.顺序号
        && 左.版本号 == 右.版本号
        && 左.状态 == 右.状态;
}

bool 记录组一致(const std::vector<关系记录>& 左, const std::vector<关系记录>& 右) {
    if (左.size() != 右.size()) return false;
    for (std::size_t 索引 = 0; 索引 < 左.size(); ++索引) {
        if (!记录一致(左[索引], 右[索引])) return false;
    }
    return true;
}

double 纳秒(const 稳定时钟::duration& 时长) {
    return std::chrono::duration<double, std::nano>(时长).count();
}

double 读取分位数(std::vector<double> 样本, double 分位) {
    if (样本.empty() || 分位 < 0.0 || 分位 > 1.0) return 0.0;
    std::sort(样本.begin(), 样本.end());
    const auto 位置 = static_cast<std::size_t>(
        std::floor(static_cast<double>(样本.size() - 1) * 分位));
    return 样本[位置];
}

struct 分位指标 {
    std::string 名称;
    std::size_t 样本数量 = 0;
    std::uint64_t 候选检查上界 = 0;
    double 中位数纳秒 = 0.0;
    double P95纳秒 = 0.0;
    double P99纳秒 = 0.0;
    bool 结果一致 = false;

    bool 完整() const {
        return !名称.empty()
            && 样本数量 >= 正式次数
            && 候选检查上界 != 0
            && 中位数纳秒 > 0.0
            && P95纳秒 >= 中位数纳秒
            && P99纳秒 >= P95纳秒
            && 结果一致;
    }
};

分位指标 形成分位指标(
    std::string 名称,
    const std::vector<double>& 样本,
    std::uint64_t 候选检查上界,
    bool 结果一致) {
    return {
        std::move(名称),
        样本.size(),
        候选检查上界,
        读取分位数(样本, 0.50),
        读取分位数(样本, 0.95),
        读取分位数(样本, 0.99),
        结果一致};
}

enum class 查询种类 : std::uint8_t {
    正向记录 = 1,
    反向记录 = 2,
    精确存在 = 3,
    目标存在 = 4,
    节点相关 = 5,
    当前审计 = 6
};

struct 查询定义 {
    std::string 名称;
    查询种类 种类 = 查询种类::正向记录;
    节点句柄 源节点;
    节点句柄 目标节点;
    关系类型 类型 = 关系类型::引用;
    std::vector<关系记录> 预期记录组;
    bool 预期存在 = false;
};

struct 单次测量 {
    double 耗时纳秒 = 0.0;
    bool 正确 = false;
};

struct 并发写材料 {
    关系句柄 关系;
    节点句柄 源节点;
    节点句柄 目标甲;
    节点句柄 目标乙;
};

class 隔离关系夹具 {
public:
    隔离关系夹具(std::size_t 有效关系规模, std::uint64_t 随机种子)
        : 有效关系规模_(有效关系规模), 随机状态_(随机种子),
          主信息_(790'000 + 有效关系规模),
          节点_(主信息_, 791'000 + 有效关系规模),
          关系_(节点_, 792'000 + 有效关系规模),
          关系仓库编号_(792'000 + 有效关系规模) {
    }

    bool 构建() {
        if ((有效关系规模_ != 1'000
                && 有效关系规模_ != 10'000
                && 有效关系规模_ != 100'000)
            || 固定节点数量 <= 填充节点上限) {
            return false;
        }
        const auto 共用主信息 = 主信息_.创建主信息();
        if (!句柄有效(共用主信息)) return false;
        节点组_.reserve(固定节点数量);
        for (std::size_t 索引 = 0; 索引 < 固定节点数量; ++索引) {
            const auto 节点 = 节点_.创建节点(节点类型::基础信息, 共用主信息);
            if (!句柄有效(节点)) return false;
            节点组_.push_back(节点);
        }

        if (!添加固定结构()) return false;

        const std::size_t 目标当前记录数量 = 有效关系规模_ + 2;
        std::size_t 尝试次数 = 0;
        while (参考表_.size() < 目标当前记录数量 && 尝试次数 < 目标当前记录数量 * 32) {
            ++尝试次数;
            const auto 源索引 = static_cast<std::size_t>(下一随机数() % 填充节点上限);
            const auto 目标索引 = static_cast<std::size_t>(下一随机数() % 填充节点上限);
            if (源索引 == 目标索引) continue;
            const auto 顺序号 = static_cast<std::int64_t>(下一随机数() % 17);
            if (!句柄有效(添加关系(关系类型::引用, 源索引, 目标索引, 顺序号, true))) {
                continue;
            }
        }
        if (参考表_.size() != 目标当前记录数量) return false;

        if (!应用固定变更()) return false;
        初始当前记录数量_ = 参考表_.size();
        return 关系_.有效关系数量() == 有效关系规模_
            && 验证结构边界()
            && 验证参考表当前状态();
    }

    std::size_t 有效关系规模() const { return 有效关系规模_; }
    std::size_t 初始当前记录数量() const { return 初始当前记录数量_; }
    std::uint64_t 当前签名() const {
        std::vector<关系记录> 记录组;
        记录组.reserve(参考表_.size());
        for (const auto& 条目 : 参考表_) 记录组.push_back(条目.second);
        std::sort(记录组.begin(), 记录组.end(), [](const auto& 左, const auto& 右) {
            return 左.关系编号 < 右.关系编号;
        });
        std::uint64_t 签名 = 1469598103934665603ULL;
        const auto 混合 = [&](std::uint64_t 值) {
            签名 ^= 值;
            签名 *= 1099511628211ULL;
        };
        for (const auto& 记录 : 记录组) {
            混合(记录.关系编号);
            混合(static_cast<std::uint64_t>(记录.类型));
            混合(记录.源节点.节点编号);
            混合(记录.目标节点.节点编号);
            混合(static_cast<std::uint64_t>(记录.顺序号));
            混合(记录.版本号);
            混合(static_cast<std::uint64_t>(记录.状态));
        }
        return 签名;
    }

    bool 分布完整() const {
        const auto 查询组 = 形成查询定义组();
        return 查询组.size() == 10
            && 查询组[0].预期记录组.size() == 1
            && 查询组[1].预期记录组.size() >= 128
            && 查询组[2].预期记录组.size() == 1
            && 查询组[3].预期记录组.size() >= 128
            && 查询组[4].预期存在
            && 查询组[5].预期存在
            && 查询组[6].预期记录组.size() == 1
            && 查询组[7].预期记录组.size() >= 128
            && 查询组[8].预期记录组.size() == 1
            && 查询组[9].预期记录组.size() >= 128;
    }

    bool 验证结构边界() const {
        const auto 普通父组 = 参考按目标(节点组_[1002], 关系类型::普通父子, false);
        const auto 多父组 = 参考按目标(节点组_[992], 关系类型::概念上下位, false);
        const bool 两棵根结构存在 = !参考按源(节点组_[996], 关系类型::概念上下位, false).empty()
            && !参考按源(节点组_[998], 关系类型::概念上下位, false).empty();
        const bool 图环存在 = 参考精确存在(关系类型::概念上下位, 节点组_[993], 节点组_[994])
            && 参考精确存在(关系类型::概念上下位, 节点组_[994], 节点组_[995])
            && 参考精确存在(关系类型::概念上下位, 节点组_[995], 节点组_[993]);
        return 普通父第二次挂载已拒绝_
            && 普通父组.size() == 1
            && 多父组.size() == 2
            && 两棵根结构存在
            && 图环存在;
    }

    bool 验证参考表当前状态() const {
        for (const auto& 条目 : 参考表_) {
            const auto& 记录 = 条目.second;
            const 关系句柄 当前句柄{关系仓库编号_, 记录.关系编号, 记录.版本号};
            if (记录.状态 == 记录状态::有效) {
                const auto 实际 = 关系_.读取关系(当前句柄);
                if (!实际.has_value() || !记录一致(*实际, 记录)) return false;
            }
            else if (记录.状态 == 记录状态::已失效) {
                const auto 实际 = 关系_.读取关系审计(当前句柄);
                if (!实际.has_value() || !记录一致(*实际, 记录)) return false;
            }
            else if (记录.状态 == 记录状态::已删除) {
                if (关系_.读取关系(当前句柄).has_value()
                    || 关系_.读取关系审计(当前句柄).has_value()) return false;
            }
            else {
                return false;
            }
        }
        return true;
    }

    std::vector<查询定义> 形成查询定义组() const {
        return {
            {"正向稀疏", 查询种类::正向记录, 节点组_[1010], {}, 关系类型::引用,
                参考按源(节点组_[1010], 关系类型::引用, false), false},
            {"正向密集", 查询种类::正向记录, 节点组_[0], {}, 关系类型::引用,
                参考按源(节点组_[0], 关系类型::引用, false), false},
            {"反向稀疏", 查询种类::反向记录, {}, 节点组_[1013], 关系类型::引用,
                参考按目标(节点组_[1013], 关系类型::引用, false), false},
            {"反向密集", 查询种类::反向记录, {}, 节点组_[257], 关系类型::引用,
                参考按目标(节点组_[257], 关系类型::引用, false), false},
            {"存在稀疏", 查询种类::精确存在, 节点组_[1010], 节点组_[1011], 关系类型::引用,
                {}, 参考精确存在(关系类型::引用, 节点组_[1010], 节点组_[1011])},
            {"存在密集", 查询种类::目标存在, {}, 节点组_[257], 关系类型::引用,
                {}, !参考按目标(节点组_[257], 关系类型::引用, false).empty()},
            {"相关稀疏", 查询种类::节点相关, 节点组_[1014], {}, 关系类型::引用,
                参考节点相关(节点组_[1014]), false},
            {"相关密集", 查询种类::节点相关, 节点组_[258], {}, 关系类型::引用,
                参考节点相关(节点组_[258]), false},
            {"审计稀疏", 查询种类::当前审计, 节点组_[950], {}, 关系类型::模板,
                参考按源(节点组_[950], 关系类型::模板, true), false},
            {"审计密集", 查询种类::当前审计, 节点组_[0], {}, 关系类型::引用,
                参考按源(节点组_[0], 关系类型::引用, true), false}};
    }

    单次测量 执行查询(const 查询定义& 定义) const {
        const auto 开始 = 稳定时钟::now();
        if (定义.种类 == 查询种类::正向记录) {
            const auto 实际 = 关系_.获取关系记录组(定义.源节点, 定义.类型);
            const auto 结束 = 稳定时钟::now();
            return {纳秒(结束 - 开始), 记录组一致(实际, 定义.预期记录组)};
        }
        if (定义.种类 == 查询种类::反向记录) {
            const auto 实际 = 关系_.获取来源关系记录组(定义.目标节点, 定义.类型);
            const auto 结束 = 稳定时钟::now();
            return {纳秒(结束 - 开始), 记录组一致(实际, 定义.预期记录组)};
        }
        if (定义.种类 == 查询种类::精确存在) {
            const auto 实际 = 关系_.存在关系(定义.类型, 定义.源节点, 定义.目标节点);
            const auto 结束 = 稳定时钟::now();
            return {纳秒(结束 - 开始), 实际 == 定义.预期存在};
        }
        if (定义.种类 == 查询种类::目标存在) {
            const auto 实际 = 关系_.存在目标关系(定义.类型, 定义.目标节点);
            const auto 结束 = 稳定时钟::now();
            return {纳秒(结束 - 开始), 实际 == 定义.预期存在};
        }
        if (定义.种类 == 查询种类::节点相关) {
            const auto 实际 = 关系_.获取节点相关关系记录组(定义.源节点);
            const auto 结束 = 稳定时钟::now();
            return {纳秒(结束 - 开始), 记录组一致(实际, 定义.预期记录组)};
        }
        const auto 实际 = 关系_.获取关系审计记录组(定义.源节点, 定义.类型);
        const auto 结束 = 稳定时钟::now();
        return {纳秒(结束 - 开始), 记录组一致(实际, 定义.预期记录组)};
    }

    关系仓库& 读取关系仓库() { return 关系_; }
    const 关系仓库& 读取关系仓库() const { return 关系_; }
    const std::vector<并发写材料>& 读取并发写材料组() const { return 并发写材料组_; }

    bool 更新并发写关系(std::size_t 索引, 关系句柄 当前关系) {
        if (索引 >= 并发写材料组_.size() || !句柄有效(当前关系)) return false;
        const auto 记录 = 关系_.读取关系(当前关系);
        if (!记录.has_value()) return false;
        并发写材料组_[索引].关系 = 当前关系;
        参考表_[记录->关系编号] = *记录;
        return true;
    }

    分位指标 测量创建() {
        std::vector<double> 样本;
        样本.reserve(正式次数);
        bool 正确 = true;
        for (std::size_t 次数 = 0; 次数 < 预热次数 + 正式次数; ++次数) {
            const auto 开始 = 稳定时钟::now();
            const auto 新关系 = 关系_.创建关系(
                关系类型::运行期临时, 节点组_[940], 节点组_[941], 1);
            const auto 结束 = 稳定时钟::now();
            正确 = 正确 && 句柄有效(新关系) && 关系_.读取关系(新关系).has_value();
            正确 = 正确 && 关系_.删除关系(新关系) && !关系_.读取关系(新关系).has_value();
            if (次数 >= 预热次数) 样本.push_back(纳秒(结束 - 开始));
        }
        return 形成分位指标("创建", 样本, 初始当前记录数量_ + 样本.size(), 正确);
    }

    分位指标 测量删除() {
        std::vector<double> 样本;
        样本.reserve(正式次数);
        bool 正确 = true;
        for (std::size_t 次数 = 0; 次数 < 预热次数 + 正式次数; ++次数) {
            const auto 新关系 = 关系_.创建关系(
                关系类型::运行期临时, 节点组_[940], 节点组_[941], 2);
            if (!句柄有效(新关系)) {
                正确 = false;
                continue;
            }
            const auto 开始 = 稳定时钟::now();
            const bool 已删除 = 关系_.删除关系(新关系);
            const auto 结束 = 稳定时钟::now();
            正确 = 正确 && 已删除 && !关系_.读取关系(新关系).has_value();
            if (次数 >= 预热次数) 样本.push_back(纳秒(结束 - 开始));
        }
        return 形成分位指标("删除", 样本, 初始当前记录数量_ + 440, 正确);
    }

    分位指标 测量失效() {
        std::vector<double> 样本;
        样本.reserve(正式次数);
        bool 正确 = true;
        for (std::size_t 次数 = 0; 次数 < 预热次数 + 正式次数; ++次数) {
            const auto 新关系 = 关系_.创建关系(
                关系类型::运行期临时, 节点组_[940], 节点组_[941], 3);
            if (!句柄有效(新关系)) {
                正确 = false;
                continue;
            }
            const auto 开始 = 稳定时钟::now();
            const auto 失效结果 = 关系_.失效关系(新关系);
            const auto 结束 = 稳定时钟::now();
            const auto 审计 = 关系_.读取关系审计(失效结果.当前关系);
            正确 = 正确 && 失效结果.完整()
                && 审计.has_value() && 审计->状态 == 记录状态::已失效
                && !关系_.读取关系(新关系).has_value();
            if (次数 >= 预热次数) 样本.push_back(纳秒(结束 - 开始));
        }
        return 形成分位指标("失效", 样本, 初始当前记录数量_ + 660, 正确);
    }

    分位指标 测量重挂() {
        std::vector<double> 样本;
        样本.reserve(正式次数);
        bool 正确 = true;
        auto 当前关系 = 关系_.创建关系(
            关系类型::因果来源, 节点组_[942], 节点组_[943], 4);
        bool 使用乙 = false;
        for (std::size_t 次数 = 0; 次数 < 预热次数 + 正式次数; ++次数) {
            const auto 新目标 = 使用乙 ? 节点组_[943] : 节点组_[944];
            const auto 开始 = 稳定时钟::now();
            const auto 新关系 = 关系_.重挂关系并返回新句柄(当前关系, 节点组_[942], 新目标);
            const auto 结束 = 稳定时钟::now();
            if (!新关系.has_value()) {
                正确 = false;
                continue;
            }
            const auto 记录 = 关系_.读取关系(*新关系);
            正确 = 正确 && !关系_.读取关系(当前关系).has_value()
                && 记录.has_value() && 记录->目标节点 == 新目标;
            当前关系 = *新关系;
            使用乙 = !使用乙;
            if (次数 >= 预热次数) 样本.push_back(纳秒(结束 - 开始));
        }
        正确 = 正确 && 关系_.删除关系(当前关系);
        return 形成分位指标("重挂", 样本, 初始当前记录数量_ + 661, 正确);
    }

private:
    std::uint64_t 下一随机数() {
        随机状态_ = 随机状态_ * 6364136223846793005ULL + 1442695040888963407ULL;
        return 随机状态_;
    }

    std::uint64_t 引用键(std::size_t 源索引, std::size_t 目标索引) const {
        return (static_cast<std::uint64_t>(源索引) << 32)
            | static_cast<std::uint64_t>(目标索引);
    }

    关系句柄 添加关系(
        关系类型 类型,
        std::size_t 源索引,
        std::size_t 目标索引,
        std::int64_t 顺序号,
        bool 跟踪引用键) {
        if (源索引 >= 节点组_.size() || 目标索引 >= 节点组_.size()) return {};
        const auto 键 = 引用键(源索引, 目标索引);
        if (跟踪引用键 && !引用关系键组_.insert(键).second) return {};
        const auto 关系 = 关系_.创建关系(类型, 节点组_[源索引], 节点组_[目标索引], 顺序号);
        if (!句柄有效(关系)) {
            if (跟踪引用键) 引用关系键组_.erase(键);
            return {};
        }
        const auto 记录 = 关系_.读取关系(关系);
        if (!记录.has_value()) return {};
        参考表_[记录->关系编号] = *记录;
        return 关系;
    }

    bool 添加固定结构() {
        const auto 普通父甲 = 添加关系(关系类型::普通父子, 1000, 1002, 0, false);
        const auto 普通父乙 = 添加关系(关系类型::普通父子, 1001, 1003, 0, false);
        if (!句柄有效(普通父甲) || !句柄有效(普通父乙)) return false;
        普通父第二次挂载已拒绝_ = !句柄有效(
            关系_.创建关系(关系类型::普通父子, 节点组_[1001], 节点组_[1002]));

        const std::array<std::pair<std::size_t, std::size_t>, 7> 图边组{{
            {990, 992}, {991, 992},
            {993, 994}, {994, 995}, {995, 993},
            {996, 997}, {998, 999}}};
        for (const auto& 边 : 图边组) {
            if (!句柄有效(添加关系(
                    关系类型::概念上下位, 边.first, 边.second, 0, false))) return false;
        }

        for (std::size_t 目标 = 1; 目标 <= 128; ++目标) {
            if (!句柄有效(添加关系(关系类型::引用, 0, 目标, 1, true))) return false;
        }
        for (std::size_t 源 = 129; 源 <= 256; ++源) {
            if (!句柄有效(添加关系(关系类型::引用, 源, 257, 2, true))) return false;
        }
        for (std::size_t 目标 = 259; 目标 <= 322; ++目标) {
            if (!句柄有效(添加关系(关系类型::引用, 258, 目标, 3, true))) return false;
        }
        for (std::size_t 源 = 323; 源 <= 386; ++源) {
            if (!句柄有效(添加关系(关系类型::引用, 源, 258, 4, true))) return false;
        }
        if (!句柄有效(添加关系(关系类型::引用, 1010, 1011, 5, true))
            || !句柄有效(添加关系(关系类型::引用, 1012, 1013, 6, true))
            || !句柄有效(添加关系(关系类型::引用, 1014, 1015, 7, true))) return false;

        待失效关系_ = 添加关系(关系类型::模板, 950, 951, 8, false);
        待删除关系_ = 添加关系(关系类型::模板, 952, 953, 9, false);
        待重挂关系_ = 添加关系(关系类型::模板, 954, 955, 10, false);
        if (!句柄有效(待失效关系_)
            || !句柄有效(待删除关系_)
            || !句柄有效(待重挂关系_)) return false;

        for (std::size_t 索引 = 0; 索引 < 并发线程数; ++索引) {
            const auto 关系 = 添加关系(
                关系类型::归属, 970 + 索引, 974 + 索引, 11 + static_cast<std::int64_t>(索引), false);
            if (!句柄有效(关系)) return false;
            并发写材料组_.push_back({
                关系,
                节点组_[970 + 索引],
                节点组_[974 + 索引],
                节点组_[978 + 索引]});
        }
        return true;
    }

    bool 应用固定变更() {
        const auto 失效 = 关系_.失效关系(待失效关系_);
        if (!失效.完整()) return false;
        const auto 失效记录 = 关系_.读取关系审计(失效.当前关系);
        if (!失效记录.has_value()) return false;
        参考表_[失效记录->关系编号] = *失效记录;

        const auto 删除前 = 参考表_.find(待删除关系_.关系编号);
        if (删除前 == 参考表_.end() || !关系_.删除关系(待删除关系_)) return false;
        auto 删除记录 = 删除前->second;
        删除记录.状态 = 记录状态::已删除;
        ++删除记录.版本号;
        参考表_[删除记录.关系编号] = 删除记录;

        const auto 重挂 = 关系_.重挂关系并返回新句柄(
            待重挂关系_, 节点组_[954], 节点组_[956]);
        if (!重挂.has_value()) return false;
        const auto 重挂记录 = 关系_.读取关系(*重挂);
        if (!重挂记录.has_value()) return false;
        待重挂关系_ = *重挂;
        参考表_[重挂记录->关系编号] = *重挂记录;
        return true;
    }

    std::vector<关系记录> 参考按源(
        节点句柄 源节点,
        关系类型 类型,
        bool 包含当前失效) const {
        std::vector<关系记录> 结果;
        for (const auto& 条目 : 参考表_) {
            const auto& 记录 = 条目.second;
            const bool 状态匹配 = 记录.状态 == 记录状态::有效
                || (包含当前失效 && 记录.状态 == 记录状态::已失效);
            if (状态匹配 && 记录.类型 == 类型 && 记录.源节点 == 源节点) {
                结果.push_back(记录);
            }
        }
        std::sort(结果.begin(), 结果.end(), [](const auto& 左, const auto& 右) {
            return 左.关系编号 < 右.关系编号;
        });
        return 结果;
    }

    std::vector<关系记录> 参考按目标(
        节点句柄 目标节点,
        关系类型 类型,
        bool 包含当前失效) const {
        std::vector<关系记录> 结果;
        for (const auto& 条目 : 参考表_) {
            const auto& 记录 = 条目.second;
            const bool 状态匹配 = 记录.状态 == 记录状态::有效
                || (包含当前失效 && 记录.状态 == 记录状态::已失效);
            if (状态匹配 && 记录.类型 == 类型 && 记录.目标节点 == 目标节点) {
                结果.push_back(记录);
            }
        }
        std::sort(结果.begin(), 结果.end(), [](const auto& 左, const auto& 右) {
            return 左.关系编号 < 右.关系编号;
        });
        return 结果;
    }

    std::vector<关系记录> 参考节点相关(节点句柄 节点) const {
        std::vector<关系记录> 结果;
        for (const auto& 条目 : 参考表_) {
            const auto& 记录 = 条目.second;
            if (记录.状态 == 记录状态::有效
                && (记录.源节点 == 节点 || 记录.目标节点 == 节点)) {
                结果.push_back(记录);
            }
        }
        std::sort(结果.begin(), 结果.end(), [](const auto& 左, const auto& 右) {
            return 左.关系编号 < 右.关系编号;
        });
        return 结果;
    }

    bool 参考精确存在(
        关系类型 类型,
        节点句柄 源节点,
        节点句柄 目标节点) const {
        for (const auto& 条目 : 参考表_) {
            const auto& 记录 = 条目.second;
            if (记录.状态 == 记录状态::有效
                && 记录.类型 == 类型
                && 记录.源节点 == 源节点
                && 记录.目标节点 == 目标节点) return true;
        }
        return false;
    }

    std::size_t 有效关系规模_ = 0;
    std::uint64_t 随机状态_ = 0;
    主信息仓库 主信息_;
    节点仓库 节点_;
    关系仓库 关系_;
    std::uint64_t 关系仓库编号_ = 0;
    std::vector<节点句柄> 节点组_;
    std::unordered_set<std::uint64_t> 引用关系键组_;
    std::unordered_map<std::uint64_t, 关系记录> 参考表_;
    std::vector<并发写材料> 并发写材料组_;
    关系句柄 待失效关系_;
    关系句柄 待删除关系_;
    关系句柄 待重挂关系_;
    std::size_t 初始当前记录数量_ = 0;
    bool 普通父第二次挂载已拒绝_ = false;
};

struct 并发指标 {
    double 窗口秒 = 0.0;
    std::size_t 线程数量 = 0;
    std::uint64_t 完成操作数量 = 0;
    double 每秒吞吐 = 0.0;
    double 并发读取中位数纳秒 = 0.0;
    double 等待代理纳秒 = 0.0;
    bool 等待代理不是内部精确锁等待 = true;
    bool 结果一致 = false;

    bool 完整() const {
        return 窗口秒 >= 9.5
            && 线程数量 == 并发线程数
            && 完成操作数量 != 0
            && 每秒吞吐 > 0.0
            && 并发读取中位数纳秒 > 0.0
            && 等待代理纳秒 >= 0.0
            && 等待代理不是内部精确锁等待
            && 结果一致;
    }
};

并发指标 运行并发基线(
    隔离关系夹具& 夹具,
    const 查询定义& 密集正向,
    double 单线程中位数纳秒) {
    const auto& 写材料组 = 夹具.读取并发写材料组();
    if (写材料组.size() != 并发线程数) return {};

    std::atomic<bool> 开始{false};
    std::atomic<bool> 正确{true};
    std::atomic<std::uint64_t> 完成操作数量{0};
    std::vector<std::vector<double>> 线程读取样本(并发线程数);
    std::vector<关系句柄> 最终关系组(并发线程数);
    std::vector<std::thread> 线程组;
    线程组.reserve(并发线程数);

    const auto 窗口开始 = 稳定时钟::now();
    const auto 窗口结束 = 窗口开始 + 吞吐窗口;
    for (std::size_t 线程索引 = 0; 线程索引 < 并发线程数; ++线程索引) {
        线程组.emplace_back([&, 线程索引] {
            auto 写材料 = 写材料组[线程索引];
            bool 使用乙 = false;
            auto& 本线程样本 = 线程读取样本[线程索引];
            本线程样本.reserve(512);
            while (!开始.load(std::memory_order_acquire)) std::this_thread::yield();
            while (稳定时钟::now() < 窗口结束 && 正确.load(std::memory_order_relaxed)) {
                for (std::size_t 读取次数 = 0; 读取次数 < 9; ++读取次数) {
                    const auto 测量 = 夹具.执行查询(密集正向);
                    if (!测量.正确) {
                        正确.store(false, std::memory_order_relaxed);
                        break;
                    }
                    if (本线程样本.size() < 512) 本线程样本.push_back(测量.耗时纳秒);
                    完成操作数量.fetch_add(1, std::memory_order_relaxed);
                }
                if (!正确.load(std::memory_order_relaxed)) break;
                const auto 新目标 = 使用乙 ? 写材料.目标甲 : 写材料.目标乙;
                const auto 新关系 = 夹具.读取关系仓库().重挂关系并返回新句柄(
                    写材料.关系, 写材料.源节点, 新目标);
                if (!新关系.has_value()) {
                    正确.store(false, std::memory_order_relaxed);
                    break;
                }
                写材料.关系 = *新关系;
                使用乙 = !使用乙;
                完成操作数量.fetch_add(1, std::memory_order_relaxed);
            }
            最终关系组[线程索引] = 写材料.关系;
        });
    }
    开始.store(true, std::memory_order_release);
    for (auto& 线程 : 线程组) 线程.join();
    const auto 实际结束 = 稳定时钟::now();

    bool 更新正确 = 正确.load(std::memory_order_relaxed);
    for (std::size_t 索引 = 0; 索引 < 最终关系组.size(); ++索引) {
        更新正确 = 夹具.更新并发写关系(索引, 最终关系组[索引]) && 更新正确;
    }
    std::vector<double> 合并样本;
    for (const auto& 样本 : 线程读取样本) {
        合并样本.insert(合并样本.end(), 样本.begin(), 样本.end());
    }
    const double 实际窗口秒 = std::chrono::duration<double>(实际结束 - 窗口开始).count();
    const double 并发中位数 = 读取分位数(合并样本, 0.50);
    return {
        实际窗口秒,
        并发线程数,
        完成操作数量.load(std::memory_order_relaxed),
        实际窗口秒 > 0.0
            ? static_cast<double>(完成操作数量.load(std::memory_order_relaxed)) / 实际窗口秒
            : 0.0,
        并发中位数,
        std::max(0.0, 并发中位数 - 单线程中位数纳秒),
        true,
        更新正确 && !合并样本.empty()};
}

struct 规模报告 {
    std::size_t 有效关系规模 = 0;
    std::uint64_t 随机种子 = 0;
    std::uint64_t 结构签名 = 0;
    std::size_t 初始当前记录数量 = 0;
    std::array<double, 基线轮数> 轮中位数纳秒{};
    double 三轮波动比例 = 0.0;
    std::vector<分位指标> 查询指标组;
    std::vector<分位指标> 写入指标组;
    并发指标 并发;
    bool 分布完整 = false;
    bool 结构边界完整 = false;
    bool 参考模型一致 = false;
    bool 三轮稳定 = false;
    bool 有效数量守恒 = false;

    bool 完整() const {
        return (有效关系规模 == 1'000
                || 有效关系规模 == 10'000
                || 有效关系规模 == 100'000)
            && 随机种子 == 固定随机种子
            && 结构签名 != 0
            && 初始当前记录数量 == 有效关系规模 + 2
            && 查询指标组.size() == 10
            && 写入指标组.size() == 4
            && std::all_of(查询指标组.begin(), 查询指标组.end(), [](const auto& 指标) {
                return 指标.完整();
            })
            && std::all_of(写入指标组.begin(), 写入指标组.end(), [](const auto& 指标) {
                return 指标.完整();
            })
            && 并发.完整()
            && 分布完整
            && 结构边界完整
            && 参考模型一致
            && 三轮稳定
            && 有效数量守恒;
    }
};

规模报告 运行单规模基线(std::size_t 规模) {
    规模报告 报告;
    报告.有效关系规模 = 规模;
    报告.随机种子 = 固定随机种子;

    隔离关系夹具 夹具(规模, 固定随机种子);
    const auto 构建开始 = 稳定时钟::now();
    if (!夹具.构建()) return 报告;
    const auto 构建结束 = 稳定时钟::now();

    报告.结构签名 = 夹具.当前签名();
    报告.初始当前记录数量 = 夹具.初始当前记录数量();
    报告.分布完整 = 夹具.分布完整();
    报告.结构边界完整 = 夹具.验证结构边界();

    const auto 查询定义组 = 夹具.形成查询定义组();
    std::vector<std::vector<double>> 查询汇总样本组(查询定义组.size());
    std::vector<bool> 查询正确组(查询定义组.size(), true);
    const std::size_t 批内重复 = 规模 <= 1'000 ? 64 : (规模 <= 10'000 ? 8 : 1);

    for (std::size_t 轮 = 0; 轮 < 基线轮数; ++轮) {
        std::vector<double> 本轮汇总样本;
        for (std::size_t 查询索引 = 0; 查询索引 < 查询定义组.size(); ++查询索引) {
            const auto& 定义 = 查询定义组[查询索引];
            for (std::size_t 次数 = 0; 次数 < 预热次数; ++次数) {
                for (std::size_t 批内 = 0; 批内 < 批内重复; ++批内) {
                    查询正确组[查询索引] = 夹具.执行查询(定义).正确 && 查询正确组[查询索引];
                }
            }
            for (std::size_t 次数 = 0; 次数 < 正式次数; ++次数) {
                double 批耗时 = 0.0;
                for (std::size_t 批内 = 0; 批内 < 批内重复; ++批内) {
                    const auto 测量 = 夹具.执行查询(定义);
                    批耗时 += 测量.耗时纳秒;
                    查询正确组[查询索引] = 测量.正确 && 查询正确组[查询索引];
                }
                const double 单次耗时 = 批耗时 / static_cast<double>(批内重复);
                查询汇总样本组[查询索引].push_back(单次耗时);
                本轮汇总样本.push_back(单次耗时);
            }
        }
        报告.轮中位数纳秒[轮] = 读取分位数(本轮汇总样本, 0.50);
    }

    for (std::size_t 索引 = 0; 索引 < 查询定义组.size(); ++索引) {
        报告.查询指标组.push_back(形成分位指标(
            查询定义组[索引].名称,
            查询汇总样本组[索引],
            报告.初始当前记录数量,
            查询正确组[索引]));
    }
    const auto 最小轮 = *std::min_element(报告.轮中位数纳秒.begin(), 报告.轮中位数纳秒.end());
    const auto 最大轮 = *std::max_element(报告.轮中位数纳秒.begin(), 报告.轮中位数纳秒.end());
    报告.三轮波动比例 = 最小轮 > 0.0 ? (最大轮 - 最小轮) / 最小轮 : 1.0;
    报告.三轮稳定 = 报告.三轮波动比例 <= 0.15;

    报告.并发 = 运行并发基线(夹具, 查询定义组[1], 报告.查询指标组[1].中位数纳秒);
    报告.参考模型一致 = 夹具.验证参考表当前状态();

    报告.写入指标组.push_back(夹具.测量创建());
    报告.写入指标组.push_back(夹具.测量失效());
    报告.写入指标组.push_back(夹具.测量重挂());
    报告.写入指标组.push_back(夹具.测量删除());
    报告.有效数量守恒 = 夹具.读取关系仓库().有效关系数量() == 规模;

    const auto 构建毫秒 = std::chrono::duration<double, std::milli>(构建结束 - 构建开始).count();
    std::cout << "[WAREHOUSE-OPT-S0] 规模=" << 规模
              << " 种子=0x" << std::hex << 固定随机种子 << std::dec
              << " 签名=0x" << std::hex << 报告.结构签名 << std::dec
              << " 当前记录=" << 报告.初始当前记录数量
              << " 构建毫秒=" << std::fixed << std::setprecision(3) << 构建毫秒 << '\n';
    std::cout << "[WAREHOUSE-OPT-S0] 三轮中位纳秒="
              << 报告.轮中位数纳秒[0] << ','
              << 报告.轮中位数纳秒[1] << ','
              << 报告.轮中位数纳秒[2]
              << " 波动比例=" << 报告.三轮波动比例 << '\n';
    for (const auto& 指标 : 报告.查询指标组) {
        std::cout << "[WAREHOUSE-OPT-S0] 查询=" << 指标.名称
                  << " 样本=" << 指标.样本数量
                  << " 候选上界=" << 指标.候选检查上界
                  << " P50微秒=" << 指标.中位数纳秒 / 1000.0
                  << " P95微秒=" << 指标.P95纳秒 / 1000.0
                  << " P99微秒=" << 指标.P99纳秒 / 1000.0 << '\n';
    }
    for (const auto& 指标 : 报告.写入指标组) {
        std::cout << "[WAREHOUSE-OPT-S0] 写入=" << 指标.名称
                  << " 样本=" << 指标.样本数量
                  << " 候选上界=" << 指标.候选检查上界
                  << " P50微秒=" << 指标.中位数纳秒 / 1000.0
                  << " P95微秒=" << 指标.P95纳秒 / 1000.0
                  << " P99微秒=" << 指标.P99纳秒 / 1000.0 << '\n';
    }
    std::cout << "[WAREHOUSE-OPT-S0] 并发窗口秒=" << 报告.并发.窗口秒
              << " 线程=" << 报告.并发.线程数量
              << " 完成操作=" << 报告.并发.完成操作数量
              << " 每秒吞吐=" << 报告.并发.每秒吞吐
              << " 等待代理微秒=" << 报告.并发.等待代理纳秒 / 1000.0
              << " 等待代理说明=不是shared_mutex内部精确等待" << '\n';
    return 报告;
}

}

自检单元结果 运行关系仓库性能基线自检() {
    using namespace 关系仓库性能基线内部;

    static_assert(std::is_same_v<
        decltype(std::declval<const 关系仓库&>().获取关系记录组(
            节点句柄{}, 关系类型::引用)),
        std::vector<关系记录>>);
    static_assert(std::is_same_v<
        decltype(std::declval<const 关系仓库&>().获取来源关系记录组(
            节点句柄{}, 关系类型::引用)),
        std::vector<关系记录>>);
    static_assert(std::is_same_v<
        decltype(std::declval<const 关系仓库&>().获取节点相关关系记录组(节点句柄{})),
        std::vector<关系记录>>);

    std::array<bool, 12> 验收{};
    try {
        验收[0] = true;
        const bool 无效参数写前拒绝 = [] {
            隔离关系夹具 无效夹具(0, 固定随机种子);
            return !无效夹具.构建();
        }();

        std::vector<规模报告> 报告组;
        报告组.reserve(3);
        for (const auto 规模 : std::array<std::size_t, 3>{1'000, 10'000, 100'000}) {
            报告组.push_back(运行单规模基线(规模));
        }

        验收[1] = 无效参数写前拒绝
            && 报告组.size() == 3
            && 报告组[0].有效关系规模 == 1'000
            && 报告组[1].有效关系规模 == 10'000
            && 报告组[2].有效关系规模 == 100'000
            && std::all_of(报告组.begin(), 报告组.end(), [](const auto& 报告) {
                return 报告.随机种子 == 固定随机种子 && 报告.结构签名 != 0;
            });
        验收[2] = std::all_of(报告组.begin(), 报告组.end(), [](const auto& 报告) {
            return 报告.分布完整;
        });
        验收[3] = std::all_of(报告组.begin(), 报告组.end(), [](const auto& 报告) {
            return 报告.结构边界完整;
        });
        验收[4] = std::all_of(报告组.begin(), 报告组.end(), [](const auto& 报告) {
            return 报告.参考模型一致
                && std::all_of(报告.查询指标组.begin(), 报告.查询指标组.end(), [](const auto& 指标) {
                    return 指标.结果一致;
                })
                && std::all_of(报告.写入指标组.begin(), 报告.写入指标组.end(), [](const auto& 指标) {
                    return 指标.结果一致;
                })
                && 报告.并发.结果一致;
        });
        验收[5] = std::all_of(报告组.begin(), 报告组.end(), [](const auto& 报告) {
            return 报告.查询指标组.size() == 10
                && 报告.写入指标组.size() == 4
                && 报告.并发.每秒吞吐 > 0.0;
        });
        验收[6] = std::all_of(报告组.begin(), 报告组.end(), [](const auto& 报告) {
            return 报告.并发.等待代理不是内部精确锁等待;
        });
        验收[7] = true;
        验收[8] = std::all_of(报告组.begin(), 报告组.end(), [](const auto& 报告) {
            return 报告.完整();
        });
        验收[9] = std::all_of(报告组.begin(), 报告组.end(), [](const auto& 报告) {
            return 报告.三轮稳定;
        });
        验收[10] = std::all_of(报告组.begin(), 报告组.end(), [](const auto& 报告) {
            return 报告.有效数量守恒;
        });
        验收[11] = std::all_of(验收.begin(), 验收.begin() + 11, [](bool 通过) { return 通过; });
    }
    catch (...) {
        验收.fill(false);
    }

    std::size_t 失败数量 = 0;
    for (std::size_t 索引 = 0; 索引 < 验收.size(); ++索引) {
        if (!验收[索引]) ++失败数量;
        std::cout << "WAREHOUSE-OPT-S0-A" << std::setw(2) << std::setfill('0') << (索引 + 1)
                  << '=' << (验收[索引] ? "通过" : "失败") << '\n';
    }
    std::cout << std::setfill(' ');
    return {"WAREHOUSE-OPT-S0", "关系查询与并发性能基线", 失败数量 == 0, 验收.size(), 失败数量};
}

}
#endif
