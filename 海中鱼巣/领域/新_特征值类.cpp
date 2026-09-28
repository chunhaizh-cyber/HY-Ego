#include "新_特征值类.h"

#include <algorithm>
#include <limits>
#include <map>
#include <mutex>
#include <utility>

namespace 海中鱼巣 {

namespace {

struct 材料键 final {
    材料物理类型 类型 = 材料物理类型::U64数组;
    新特征值材料 材料;

    bool operator<(const 材料键& 另一项) const noexcept {
        if (类型 != 另一项.类型) {
            return static_cast<std::int64_t>(类型)
                < static_cast<std::int64_t>(另一项.类型);
        }
        return 材料 < 另一项.材料;
    }
};

bool 是已知物理类型(材料物理类型 类型) noexcept {
    switch (类型) {
    case 材料物理类型::U64数组:
    case 材料物理类型::I64数组:
    case 材料物理类型::UTF8字符串:
    case 材料物理类型::二维二值格:
    case 材料物理类型::三维二值格:
        return true;
    }
    return false;
}

bool 材料形态匹配(材料物理类型 类型, const 新特征值材料& 材料) noexcept {
    switch (类型) {
    case 材料物理类型::U64数组:
    case 材料物理类型::二维二值格:
    case 材料物理类型::三维二值格:
        return std::holds_alternative<std::vector<std::uint64_t>>(材料);
    case 材料物理类型::I64数组:
        return std::holds_alternative<std::vector<std::int64_t>>(材料);
    case 材料物理类型::UTF8字符串:
        return std::holds_alternative<std::string>(材料);
    }
    return false;
}

bool 安全平方(std::size_t 值, std::size_t& 结果) noexcept {
    if (值 != 0 && 值 > (std::numeric_limits<std::size_t>::max)() / 值) return false;
    结果 = 值 * 值;
    return true;
}

bool 安全立方(std::size_t 值, std::size_t& 结果) noexcept {
    std::size_t 平方 = 0;
    if (!安全平方(值, 平方)
        || (值 != 0 && 平方 > (std::numeric_limits<std::size_t>::max)() / 值)) {
        return false;
    }
    结果 = 平方 * 值;
    return true;
}

std::optional<std::size_t> 二维边长(std::size_t 总位数) noexcept {
    std::size_t 边长 = 8;
    for (;;) {
        std::size_t 当前位数 = 0;
        if (!安全平方(边长, 当前位数)) return std::nullopt;
        if (当前位数 == 总位数) return 边长;
        if (当前位数 > 总位数
            || 边长 > (std::numeric_limits<std::size_t>::max)() / 2) {
            return std::nullopt;
        }
        边长 *= 2;
    }
}

std::optional<std::size_t> 三维边长(std::size_t 总位数) noexcept {
    std::size_t 边长 = 4;
    for (;;) {
        std::size_t 当前位数 = 0;
        if (!安全立方(边长, 当前位数)) return std::nullopt;
        if (当前位数 == 总位数) return 边长;
        if (当前位数 > 总位数
            || 边长 > (std::numeric_limits<std::size_t>::max)() / 2) {
            return std::nullopt;
        }
        边长 *= 2;
    }
}

bool 读取位(const std::vector<std::uint64_t>& 材料, std::size_t 位置) noexcept {
    return (材料[位置 / 64] & (std::uint64_t{1} << (位置 % 64))) != 0;
}

void 写入位(std::vector<std::uint64_t>& 材料, std::size_t 位置) noexcept {
    材料[位置 / 64] |= std::uint64_t{1} << (位置 % 64);
}

std::vector<std::uint64_t> 压缩二维一层(
    const std::vector<std::uint64_t>& 下层, std::size_t 下层边长) {
    const std::size_t 上层边长 = 下层边长 / 2;
    const std::size_t 上层位数 = 上层边长 * 上层边长;
    std::vector<std::uint64_t> 上层(上层位数 / 64, 0);

    for (std::size_t 上层Y = 0; 上层Y < 上层边长; ++上层Y) {
        for (std::size_t 上层X = 0; 上层X < 上层边长; ++上层X) {
            bool 占用 = false;
            for (std::size_t 偏移Y = 0; 偏移Y < 2 && !占用; ++偏移Y) {
                for (std::size_t 偏移X = 0; 偏移X < 2; ++偏移X) {
                    const std::size_t 下层X = 上层X * 2 + 偏移X;
                    const std::size_t 下层Y = 上层Y * 2 + 偏移Y;
                    if (读取位(下层, 下层Y * 下层边长 + 下层X)) {
                        占用 = true;
                        break;
                    }
                }
            }
            if (占用) 写入位(上层, 上层Y * 上层边长 + 上层X);
        }
    }
    return 上层;
}

std::vector<std::uint64_t> 压缩三维一层(
    const std::vector<std::uint64_t>& 下层, std::size_t 下层边长) {
    const std::size_t 上层边长 = 下层边长 / 2;
    const std::size_t 上层位数 = 上层边长 * 上层边长 * 上层边长;
    std::vector<std::uint64_t> 上层(上层位数 / 64, 0);

    for (std::size_t 上层Z = 0; 上层Z < 上层边长; ++上层Z) {
        for (std::size_t 上层Y = 0; 上层Y < 上层边长; ++上层Y) {
            for (std::size_t 上层X = 0; 上层X < 上层边长; ++上层X) {
                bool 占用 = false;
                for (std::size_t 偏移Z = 0; 偏移Z < 2 && !占用; ++偏移Z) {
                    for (std::size_t 偏移Y = 0; 偏移Y < 2 && !占用; ++偏移Y) {
                        for (std::size_t 偏移X = 0; 偏移X < 2; ++偏移X) {
                            const std::size_t 下层X = 上层X * 2 + 偏移X;
                            const std::size_t 下层Y = 上层Y * 2 + 偏移Y;
                            const std::size_t 下层Z = 上层Z * 2 + 偏移Z;
                            const std::size_t 下层位置 =
                                (下层Z * 下层边长 + 下层Y) * 下层边长 + 下层X;
                            if (读取位(下层, 下层位置)) {
                                占用 = true;
                                break;
                            }
                        }
                    }
                }
                if (占用) {
                    const std::size_t 上层位置 =
                        (上层Z * 上层边长 + 上层Y) * 上层边长 + 上层X;
                    写入位(上层, 上层位置);
                }
            }
        }
    }
    return 上层;
}

std::optional<std::vector<std::vector<std::uint64_t>>> 生成压缩层(
    材料物理类型 类型, const 新特征值材料& 材料) {
    if (类型 != 材料物理类型::二维二值格
        && 类型 != 材料物理类型::三维二值格) {
        return std::vector<std::vector<std::uint64_t>>{};
    }

    const auto* 原始材料 = std::get_if<std::vector<std::uint64_t>>(&材料);
    if (!原始材料
        || 原始材料->size() > (std::numeric_limits<std::size_t>::max)() / 64) {
        return std::nullopt;
    }
    const std::size_t 总位数 = 原始材料->size() * 64;
    const auto 边长 = 类型 == 材料物理类型::二维二值格
        ? 二维边长(总位数) : 三维边长(总位数);
    if (!边长) return std::nullopt;

    std::vector<std::vector<std::uint64_t>> 层;
    层.push_back(*原始材料);
    std::size_t 当前边长 = *边长;
    const std::size_t 根边长 =
        类型 == 材料物理类型::二维二值格 ? 8 : 4;
    while (当前边长 > 根边长) {
        层.push_back(类型 == 材料物理类型::二维二值格
            ? 压缩二维一层(层.back(), 当前边长)
            : 压缩三维一层(层.back(), 当前边长));
        当前边长 /= 2;
    }
    return 层;
}

std::optional<稳定编码> 读取唯一节点字段(
    const 基础数据集& 数据集, 稳定编码 节点, 稳定编码 字段节点) {
    const auto 字段 = 数据集.查询字段(节点, 字段节点);
    if (字段.size() != 1) return std::nullopt;
    const auto* 目标 = std::get_if<稳定编码>(&字段.front().内容);
    if (!目标 || !有效(*目标)) return std::nullopt;
    return *目标;
}

std::optional<基础值> 读取唯一值字段(
    const 基础数据集& 数据集, 稳定编码 节点, 稳定编码 字段节点) {
    const auto 字段 = 数据集.查询字段(节点, 字段节点);
    if (字段.size() != 1) return std::nullopt;
    const auto* 值 = std::get_if<基础值>(&字段.front().内容);
    return 值 ? std::optional<基础值>{*值} : std::nullopt;
}

void 清理新节点(基础数据集& 数据集, 稳定编码 节点,
    const std::vector<稳定编码>& 字段关系) noexcept {
    for (auto 位置 = 字段关系.rbegin(); 位置 != 字段关系.rend(); ++位置) {
        数据集.删除字段(*位置);
    }
    try {
        for (const auto& 关系 : 数据集.查询目标关系(节点, 基础外部关系类型::父子)) {
            数据集.删除关系(关系.编码);
        }
        for (const auto& 关系 : 数据集.查询源关系(节点, 基础外部关系类型::父子)) {
            数据集.删除关系(关系.编码);
        }
    } catch (...) {
        return;
    }
    数据集.删除节点(节点);
}

} // namespace

class 新_特征值类::实现 final {
public:
    explicit 实现(基础数据集& 数据集) : 数据集_(数据集) {}

    bool 初始化() noexcept {
        std::scoped_lock 锁(互斥_);
        if (已初始化_) return true;

        std::vector<稳定编码> 已建节点;
        for (auto* 目标 : {&节点类型字段_, &特征值类型节点_, &压缩层类型节点_,
                &材料物理类型字段_, &原始材料字段_, &压缩材料字段_}) {
            *目标 = 数据集_.新建节点();
            if (!有效(*目标)) {
                for (auto 位置 = 已建节点.rbegin(); 位置 != 已建节点.rend(); ++位置) {
                    数据集_.删除节点(*位置);
                }
                清空结构节点();
                return false;
            }
            已建节点.push_back(*目标);
        }
        已初始化_ = true;
        return true;
    }

    稳定编码 保存或取得特征值(
        材料物理类型 类型, const 新特征值材料& 材料) noexcept {
        try {
            std::scoped_lock 锁(互斥_);
            if (!已初始化_ || !是已知物理类型(类型) || !材料形态匹配(类型, 材料)) {
                return {};
            }
            const auto 层 = 生成压缩层(类型, 材料);
            if (!层) return {};
            return 层->empty()
                ? 保存或取得普通材料(类型, 材料)
                : 保存或取得二值格(类型, 材料, *层);
        } catch (...) {
            return {};
        }
    }

    std::optional<新特征值信息> 获取特征值(稳定编码 节点) const noexcept {
        try {
            std::scoped_lock 锁(互斥_);
            return 获取特征值已加锁(节点);
        } catch (...) {
            return std::nullopt;
        }
    }

    std::vector<稳定编码> 查询特征值(
        材料物理类型 类型, const 新特征值材料& 材料) const noexcept {
        try {
            std::scoped_lock 锁(互斥_);
            if (!已初始化_ || !是已知物理类型(类型) || !材料形态匹配(类型, 材料)) {
                return {};
            }
            const auto 层 = 生成压缩层(类型, 材料);
            if (!层) return {};
            return 层->empty()
                ? 查询普通材料(类型, 材料)
                : 查询二值格(类型, 材料, *层);
        } catch (...) {
            return {};
        }
    }

    bool 是特征值节点(稳定编码 节点) const noexcept {
        try {
            std::scoped_lock 锁(互斥_);
            return 已初始化_ && 是指定类型节点(节点, 特征值类型节点_);
        } catch (...) {
            return false;
        }
    }

private:
    void 清空结构节点() noexcept {
        节点类型字段_ = {};
        特征值类型节点_ = {};
        压缩层类型节点_ = {};
        材料物理类型字段_ = {};
        原始材料字段_ = {};
        压缩材料字段_ = {};
    }

    bool 是指定类型节点(稳定编码 节点, 稳定编码 类型节点) const {
        const auto 实际类型 = 读取唯一节点字段(数据集_, 节点, 节点类型字段_);
        return 实际类型 && *实际类型 == 类型节点;
    }

    bool 压缩材料相同(稳定编码 节点,
        const std::vector<std::uint64_t>& 材料) const {
        if (!是指定类型节点(节点, 压缩层类型节点_)) return false;
        const auto 实际材料 = 读取唯一值字段(数据集_, 节点, 压缩材料字段_);
        if (!实际材料) return false;
        const auto* 值 = std::get_if<std::vector<std::uint64_t>>(&实际材料->材料);
        return 值 && *值 == 材料;
    }

    std::optional<新特征值信息> 获取特征值已加锁(稳定编码 节点) const {
        if (!有效(节点) || !是指定类型节点(节点, 特征值类型节点_)) {
            return std::nullopt;
        }

        const auto 类型字段 = 读取唯一值字段(数据集_, 节点, 材料物理类型字段_);
        const auto 材料字段 = 读取唯一值字段(数据集_, 节点, 原始材料字段_);
        if (!类型字段 || !材料字段) return std::nullopt;
        const auto* 类型值 = std::get_if<std::int64_t>(&类型字段->材料);
        if (!类型值) return std::nullopt;

        const auto 类型 = static_cast<材料物理类型>(*类型值);
        if (!是已知物理类型(类型)) return std::nullopt;

        新特征值材料 材料;
        if (const auto* U64值 =
            std::get_if<std::vector<std::uint64_t>>(&材料字段->材料)) {
            材料 = *U64值;
        } else if (const auto* I64值 =
            std::get_if<std::vector<std::int64_t>>(&材料字段->材料)) {
            材料 = *I64值;
        } else if (const auto* 字符串值 = std::get_if<std::string>(&材料字段->材料)) {
            材料 = *字符串值;
        } else {
            return std::nullopt;
        }
        if (!材料形态匹配(类型, 材料)) return std::nullopt;
        return 新特征值信息{节点, 类型, std::move(材料)};
    }

    稳定编码 建立压缩节点(
        std::optional<稳定编码> 上级节点,
        const std::vector<std::uint64_t>& 材料) {
        const 稳定编码 节点 = 上级节点
            ? 数据集_.新建节点(*上级节点) : 数据集_.新建节点();
        if (!有效(节点)) return {};

        std::vector<稳定编码> 字段关系;
        const auto 类型关系 = 数据集_.添加字段节点(
            节点, 节点类型字段_, 压缩层类型节点_);
        if (!有效(类型关系)) {
            清理新节点(数据集_, 节点, 字段关系);
            return {};
        }
        字段关系.push_back(类型关系);

        const auto 材料关系 = 数据集_.添加字段值(
            节点, 压缩材料字段_, 基础值{材料});
        if (!有效(材料关系)) {
            清理新节点(数据集_, 节点, 字段关系);
            return {};
        }
        return 节点;
    }

    稳定编码 建立原始材料节点(std::optional<稳定编码> 上级节点,
        材料物理类型 类型, const 新特征值材料& 材料) {
        const 稳定编码 节点 = 上级节点
            ? 数据集_.新建节点(*上级节点) : 数据集_.新建节点();
        if (!有效(节点)) return {};

        std::vector<稳定编码> 字段关系;
        const auto 类型关系 = 数据集_.添加字段节点(
            节点, 节点类型字段_, 特征值类型节点_);
        if (!有效(类型关系)) {
            清理新节点(数据集_, 节点, 字段关系);
            return {};
        }
        字段关系.push_back(类型关系);

        const auto 物理类型关系 = 数据集_.添加字段值(节点,
            材料物理类型字段_, 基础值{static_cast<std::int64_t>(类型)});
        if (!有效(物理类型关系)) {
            清理新节点(数据集_, 节点, 字段关系);
            return {};
        }
        字段关系.push_back(物理类型关系);

        基础值 原始材料;
        std::visit([&原始材料](const auto& 值) { 原始材料.材料 = 值; }, 材料);
        const auto 材料关系 = 数据集_.添加字段值(
            节点, 原始材料字段_, std::move(原始材料));
        if (!有效(材料关系)) {
            清理新节点(数据集_, 节点, 字段关系);
            return {};
        }
        return 节点;
    }

    std::vector<稳定编码> 子节点(稳定编码 上级节点) const {
        std::vector<稳定编码> 结果;
        for (const auto& 关系 :
            数据集_.查询源关系(上级节点, 基础外部关系类型::父子)) {
            结果.push_back(关系.目标节点);
        }
        return 结果;
    }

    稳定编码 查找压缩子节点(稳定编码 上级节点,
        const std::vector<std::uint64_t>& 材料) const {
        for (const auto 节点 : 子节点(上级节点)) {
            if (压缩材料相同(节点, 材料)) return 节点;
        }
        return {};
    }

    稳定编码 查找原始子节点(稳定编码 上级节点,
        材料物理类型 类型, const 新特征值材料& 材料) const {
        for (const auto 节点 : 子节点(上级节点)) {
            const auto 信息 = 获取特征值已加锁(节点);
            if (信息 && 信息->物理类型 == 类型 && 信息->材料 == 材料) return 节点;
        }
        return {};
    }

    稳定编码 保存或取得普通材料(
        材料物理类型 类型, const 新特征值材料& 材料) {
        const 材料键 键{类型, 材料};
        auto& 候选 = 普通材料索引_[键];
        for (const auto 节点 : 候选) {
            const auto 信息 = 获取特征值已加锁(节点);
            if (信息 && 信息->物理类型 == 类型 && 信息->材料 == 材料) return 节点;
        }

        const auto 节点 = 建立原始材料节点(std::nullopt, 类型, 材料);
        if (有效(节点)) 候选.push_back(节点);
        return 节点;
    }

    稳定编码 保存或取得二值格(材料物理类型 类型,
        const 新特征值材料& 原始材料,
        const std::vector<std::vector<std::uint64_t>>& 层) {
        const 材料键 根键{类型, 新特征值材料{层.back()}};
        auto& 根候选 = 根索引_[根键];
        稳定编码 当前节点;
        for (const auto 节点 : 根候选) {
            if (压缩材料相同(节点, 层.back())) {
                当前节点 = 节点;
                break;
            }
        }
        if (!有效(当前节点)) {
            当前节点 = 建立压缩节点(std::nullopt, 层.back());
            if (!有效(当前节点)) return {};
            根候选.push_back(当前节点);
        }

        for (std::size_t 位置 = 层.size(); 位置 > 1; --位置) {
            const auto& 细层材料 = 层[位置 - 2];
            if (位置 - 2 == 0) break;
            auto 下一节点 = 查找压缩子节点(当前节点, 细层材料);
            if (!有效(下一节点)) {
                下一节点 = 建立压缩节点(当前节点, 细层材料);
                if (!有效(下一节点)) return {};
            }
            当前节点 = 下一节点;
        }

        const auto 已有节点 = 查找原始子节点(当前节点, 类型, 原始材料);
        return 有效(已有节点)
            ? 已有节点 : 建立原始材料节点(当前节点, 类型, 原始材料);
    }

    std::vector<稳定编码> 查询普通材料(
        材料物理类型 类型, const 新特征值材料& 材料) const {
        std::vector<稳定编码> 结果;
        const auto 位置 = 普通材料索引_.find(材料键{类型, 材料});
        if (位置 == 普通材料索引_.end()) return 结果;
        for (const auto 节点 : 位置->second) {
            const auto 信息 = 获取特征值已加锁(节点);
            if (信息 && 信息->物理类型 == 类型 && 信息->材料 == 材料) {
                结果.push_back(节点);
            }
        }
        return 结果;
    }

    std::vector<稳定编码> 查询二值格(材料物理类型 类型,
        const 新特征值材料& 原始材料,
        const std::vector<std::vector<std::uint64_t>>& 层) const {
        std::vector<稳定编码> 当前层;
        const auto 根位置 = 根索引_.find(材料键{类型, 新特征值材料{层.back()}});
        if (根位置 == 根索引_.end()) return {};
        for (const auto 节点 : 根位置->second) {
            if (压缩材料相同(节点, 层.back())) 当前层.push_back(节点);
        }

        for (std::size_t 位置 = 层.size(); 位置 > 1; --位置) {
            const auto& 细层材料 = 层[位置 - 2];
            if (位置 - 2 == 0) break;
            std::vector<稳定编码> 下一层;
            for (const auto 上级节点 : 当前层) {
                for (const auto 节点 : 子节点(上级节点)) {
                    if (压缩材料相同(节点, 细层材料)) 下一层.push_back(节点);
                }
            }
            当前层 = std::move(下一层);
            if (当前层.empty()) return {};
        }

        std::vector<稳定编码> 结果;
        for (const auto 上级节点 : 当前层) {
            for (const auto 节点 : 子节点(上级节点)) {
                const auto 信息 = 获取特征值已加锁(节点);
                if (信息 && 信息->物理类型 == 类型 && 信息->材料 == 原始材料) {
                    结果.push_back(节点);
                }
            }
        }
        std::sort(结果.begin(), 结果.end(),
            [](稳定编码 左, 稳定编码 右) { return 左.值 < 右.值; });
        结果.erase(std::unique(结果.begin(), 结果.end()), 结果.end());
        return 结果;
    }

    基础数据集& 数据集_;
    mutable std::mutex 互斥_;
    bool 已初始化_ = false;

    稳定编码 节点类型字段_;
    稳定编码 特征值类型节点_;
    稳定编码 压缩层类型节点_;
    稳定编码 材料物理类型字段_;
    稳定编码 原始材料字段_;
    稳定编码 压缩材料字段_;

    std::map<材料键, std::vector<稳定编码>> 普通材料索引_;
    std::map<材料键, std::vector<稳定编码>> 根索引_;
};

新_特征值类::新_特征值类(基础数据集& 数据集)
    : 实现_(std::make_unique<实现>(数据集)) {}

新_特征值类::~新_特征值类() = default;

bool 新_特征值类::初始化() noexcept {
    return 实现_->初始化();
}

稳定编码 新_特征值类::保存或取得特征值(
    材料物理类型 物理类型, const 新特征值材料& 材料) noexcept {
    return 实现_->保存或取得特征值(物理类型, 材料);
}

std::optional<新特征值信息> 新_特征值类::获取特征值(
    稳定编码 节点) const noexcept {
    return 实现_->获取特征值(节点);
}

std::vector<稳定编码> 新_特征值类::查询特征值(
    材料物理类型 物理类型, const 新特征值材料& 材料) const noexcept {
    return 实现_->查询特征值(物理类型, 材料);
}

bool 新_特征值类::是特征值节点(稳定编码 节点) const noexcept {
    return 实现_->是特征值节点(节点);
}

} // namespace 海中鱼巣
