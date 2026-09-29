#include "新_特征类.h"

#include <algorithm>
#include <array>
#include <bit>
#include <limits>
#include <mutex>

namespace 海中鱼巣 {

namespace {

std::recursive_mutex 新特征互斥;

稳定编码 节点类型字段{};
稳定编码 特征节点类型{};
稳定编码 特征概念字段{};
稳定编码 值域节点类型{};
稳定编码 实例历史聚合值域字段{};
稳定编码 I64值域字段{};
稳定编码 全材料值域字段{};
稳定编码 非I64值域字段{};
稳定编码 结构化分量字段{};
稳定编码 结构化分量节点类型{};
稳定编码 分量顺序字段{};
稳定编码 分量角色字段{};
稳定编码 分量单位字段{};
稳定编码 分量I64值域字段{};
稳定编码 实例单位字段{};
稳定编码 当前值字段{};
稳定编码 历史值字段{};
稳定编码 标准值字段{};
稳定编码 历史值命中记录字段{};
稳定编码 历史值命中记录节点类型{};
稳定编码 历史值记录准确值字段{};
稳定编码 历史值记录命中次数字段{};
稳定编码 历史值记录所属特征字段{};
稳定编码 归组子特征标记字段{};

bool 节点仍存在(稳定编码 节点) noexcept {
    return 有效(节点) && 全局基础数据集.查询节点(节点).has_value();
}

bool 结构已初始化() noexcept {
    return 有效(节点类型字段)
        && 有效(特征节点类型)
        && 有效(特征概念字段)
        && 有效(值域节点类型)
        && 有效(实例历史聚合值域字段)
        && 有效(I64值域字段)
        && 有效(全材料值域字段)
        && 有效(非I64值域字段)
        && 有效(结构化分量字段)
        && 有效(结构化分量节点类型)
        && 有效(分量顺序字段)
        && 有效(分量角色字段)
        && 有效(分量单位字段)
        && 有效(分量I64值域字段)
        && 有效(实例单位字段)
        && 有效(当前值字段)
        && 有效(历史值字段)
        && 有效(标准值字段)
        && 有效(历史值命中记录字段)
        && 有效(历史值命中记录节点类型)
        && 有效(历史值记录准确值字段)
        && 有效(历史值记录命中次数字段)
        && 有效(历史值记录所属特征字段)
        && 有效(归组子特征标记字段);
}

bool 结构仍存在() noexcept {
    return 结构已初始化()
        && 节点仍存在(节点类型字段)
        && 节点仍存在(特征节点类型)
        && 节点仍存在(特征概念字段)
        && 节点仍存在(值域节点类型)
        && 节点仍存在(实例历史聚合值域字段)
        && 节点仍存在(I64值域字段)
        && 节点仍存在(全材料值域字段)
        && 节点仍存在(非I64值域字段)
        && 节点仍存在(结构化分量字段)
        && 节点仍存在(结构化分量节点类型)
        && 节点仍存在(分量顺序字段)
        && 节点仍存在(分量角色字段)
        && 节点仍存在(分量单位字段)
        && 节点仍存在(分量I64值域字段)
        && 节点仍存在(实例单位字段)
        && 节点仍存在(当前值字段)
        && 节点仍存在(历史值字段)
        && 节点仍存在(标准值字段)
        && 节点仍存在(历史值命中记录字段)
        && 节点仍存在(历史值命中记录节点类型)
        && 节点仍存在(历史值记录准确值字段)
        && 节点仍存在(历史值记录命中次数字段)
        && 节点仍存在(历史值记录所属特征字段)
        && 节点仍存在(归组子特征标记字段);
}

std::optional<稳定编码> 读取唯一节点字段(
    稳定编码 节点, 稳定编码 字段) {
    const auto 字段组 = 全局基础数据集.查询字段(节点, 字段);
    if (字段组.size() != 1) return std::nullopt;
    const auto* 目标 = std::get_if<稳定编码>(&字段组.front().内容);
    return 目标 ? std::optional<稳定编码>{*目标} : std::nullopt;
}

std::optional<新特征准确值> 解码准确值(const 基础字段关系& 字段) {
    if (const auto* 目标 = std::get_if<稳定编码>(&字段.内容)) {
        return 新特征准确值{*目标};
    }
    const auto* 值 = std::get_if<基础值>(&字段.内容);
    if (!值) return std::nullopt;
    const auto* I64 = std::get_if<std::int64_t>(&值->材料);
    return I64 ? std::optional<新特征准确值>{新特征准确值{*I64}}
        : std::nullopt;
}

稳定编码 写入准确值字段(
    稳定编码 节点, 稳定编码 字段, const 新特征准确值& 值) noexcept {
    if (const auto* I64 = std::get_if<std::int64_t>(&值)) {
        return 全局基础数据集.添加字段值(
            节点, 字段, 基础值{基础原始值{*I64}});
    }
    const auto* 值节点 = std::get_if<稳定编码>(&值);
    return 值节点
        ? 全局基础数据集.添加字段节点(节点, 字段, *值节点)
        : 稳定编码{};
}

bool 修改准确值字段(
    稳定编码 字段关系, const 新特征准确值& 值) noexcept {
    if (const auto* I64 = std::get_if<std::int64_t>(&值)) {
        return 全局基础数据集.修改字段值(
            字段关系, 基础值{基础原始值{*I64}});
    }
    const auto* 值节点 = std::get_if<稳定编码>(&值);
    return 值节点 && 全局基础数据集.修改字段节点(字段关系, *值节点);
}

std::optional<std::int64_t> 读取唯一I64字段(
    稳定编码 节点, 稳定编码 字段) {
    const auto 字段组 = 全局基础数据集.查询字段(节点, 字段);
    if (字段组.size() != 1) return std::nullopt;
    const auto* 值 = std::get_if<基础值>(&字段组.front().内容);
    const auto* I64 = 值 ? std::get_if<std::int64_t>(&值->材料) : nullptr;
    return I64 ? std::optional<std::int64_t>{*I64} : std::nullopt;
}

struct 历史值命中物理记录 final {
    稳定编码 记录节点;
    稳定编码 所属特征;
    稳定编码 持有字段关系;
    稳定编码 命中次数字段关系;
    新特征准确值 值;
    std::int64_t 命中次数 = 0;
};

std::optional<std::vector<历史值命中物理记录>> 读取历史值命中记录(
    稳定编码 特征节点) {
    std::vector<历史值命中物理记录> 结果;
    for (const auto& 持有关系 : 全局基础数据集.查询字段(
        特征节点, 历史值命中记录字段)) {
        const auto* 记录节点 = std::get_if<稳定编码>(&持有关系.内容);
        if (!记录节点 || !节点仍存在(*记录节点)) return std::nullopt;
        const auto 类型 = 读取唯一节点字段(*记录节点, 节点类型字段);
        const auto 准确值组 = 全局基础数据集.查询字段(
            *记录节点, 历史值记录准确值字段);
        const auto 次数组 = 全局基础数据集.查询字段(
            *记录节点, 历史值记录命中次数字段);
        const auto 所属特征 = 读取唯一节点字段(
            *记录节点, 历史值记录所属特征字段);
        if (!类型 || *类型 != 历史值命中记录节点类型
            || !所属特征 || 准确值组.size() != 1 || 次数组.size() != 1) {
            return std::nullopt;
        }
        if (*所属特征 != 特征节点) {
            const auto 父关系 = 全局基础数据集.查询目标关系(
                特征节点, 基础外部关系类型::父子);
            const auto 标记 = 读取唯一I64字段(
                特征节点, 归组子特征标记字段);
            if (父关系.size() != 1
                || 父关系.front().源节点 != *所属特征
                || !标记 || *标记 != 1) {
                return std::nullopt;
            }
        }
        const auto 准确值 = 解码准确值(准确值组.front());
        const auto 次数 = 读取唯一I64字段(
            *记录节点, 历史值记录命中次数字段);
        if (!准确值 || !次数 || *次数 <= 0) return std::nullopt;
        结果.push_back({
            *记录节点, *所属特征,
            持有关系.编码, 次数组.front().编码,
            *准确值, *次数});
    }
    return 结果;
}

std::optional<历史值命中物理记录> 建立历史值命中记录(
    稳定编码 特征节点, const 新特征准确值& 值,
    std::int64_t 初始命中次数 = 1) noexcept {
    if (初始命中次数 <= 0) return std::nullopt;
    const auto 记录节点 = 全局基础数据集.新建节点();
    if (!有效(记录节点)) return std::nullopt;
    std::vector<稳定编码> 已写字段;
    const auto 类型关系 = 全局基础数据集.添加字段节点(
        记录节点, 节点类型字段, 历史值命中记录节点类型);
    if (有效(类型关系)) 已写字段.push_back(类型关系);
    const auto 值关系 = 有效(类型关系)
        ? 写入准确值字段(记录节点, 历史值记录准确值字段, 值)
        : 稳定编码{};
    if (有效(值关系)) 已写字段.push_back(值关系);
    const auto 所属关系 = 有效(值关系)
        ? 全局基础数据集.添加字段节点(
            记录节点, 历史值记录所属特征字段, 特征节点)
        : 稳定编码{};
    if (有效(所属关系)) 已写字段.push_back(所属关系);
    const auto 次数关系 = 有效(所属关系)
        ? 全局基础数据集.添加字段值(
            记录节点, 历史值记录命中次数字段,
            基础值{基础原始值{初始命中次数}})
        : 稳定编码{};
    if (有效(次数关系)) 已写字段.push_back(次数关系);
    const auto 持有关系 = 有效(次数关系)
        ? 全局基础数据集.添加字段节点(
            特征节点, 历史值命中记录字段, 记录节点)
        : 稳定编码{};
    if (有效(持有关系)) {
        return 历史值命中物理记录{
            记录节点, 特征节点, 持有关系, 次数关系,
            值, 初始命中次数};
    }
    for (auto 位置 = 已写字段.rbegin(); 位置 != 已写字段.rend(); ++位置) {
        (void)全局基础数据集.删除字段(*位置);
    }
    (void)全局基础数据集.删除节点(记录节点);
    return std::nullopt;
}

void 删除历史值命中记录(const 历史值命中物理记录& 记录) noexcept {
    (void)全局基础数据集.删除字段(记录.持有字段关系);
    for (const auto 字段 : std::array{
        节点类型字段, 历史值记录准确值字段,
        历史值记录命中次数字段, 历史值记录所属特征字段}) {
        for (const auto& 关系 : 全局基础数据集.查询字段(记录.记录节点, 字段)) {
            (void)全局基础数据集.删除字段(关系.编码);
        }
    }
    (void)全局基础数据集.删除节点(记录.记录节点);
}

std::optional<材料物理类型> 转换特征值材料类型(
    特征概念材料物理类型 类型) noexcept {
    switch (类型) {
    case 特征概念材料物理类型::U64数组:
        return 材料物理类型::U64数组;
    case 特征概念材料物理类型::I64数组:
        return 材料物理类型::I64数组;
    case 特征概念材料物理类型::UTF8字符串:
        return 材料物理类型::UTF8字符串;
    case 特征概念材料物理类型::二维二值格:
        return 材料物理类型::二维二值格;
    case 特征概念材料物理类型::三维二值格:
        return 材料物理类型::三维二值格;
    case 特征概念材料物理类型::I64标量:
        return std::nullopt;
    }
    return std::nullopt;
}

bool 准确值命中值域(
    特征概念材料物理类型 材料类型,
    const 特征概念值域& 值域,
    const 新特征准确值& 值,
    const 新_特征值类& 特征值服务) noexcept {
    const auto I64命中值域 = [](const std::vector<特征概念I64闭区间>& 值域,
                                std::int64_t I64) {
        return std::any_of(值域.begin(), 值域.end(),
            [I64](const auto& 区间) {
                return 区间.下界 <= I64 && I64 <= 区间.上界;
            });
    };
    if (材料类型 == 特征概念材料物理类型::I64标量) {
        const auto* I64 = std::get_if<std::int64_t>(&值);
        const auto* I64域 =
            std::get_if<std::vector<特征概念I64闭区间>>(&值域);
        return I64 && I64域 && I64命中值域(*I64域, *I64);
    }
    const auto 期望类型 = 转换特征值材料类型(材料类型);
    const auto* 值节点 = std::get_if<稳定编码>(&值);
    if (!期望类型 || !值节点 || !有效(*值节点)) return false;
    const auto 信息 = 特征值服务.获取特征值(*值节点);
    if (!信息 || 信息->物理类型 != *期望类型) return false;
    if (std::holds_alternative<特征概念全材料值域>(值域)) return true;
    if (const auto* 节点组 = std::get_if<std::vector<稳定编码>>(&值域)) {
        return std::binary_search(节点组->begin(), 节点组->end(), *值节点);
    }
    const auto* 结构化 = std::get_if<特征概念结构化I64值域>(&值域);
    const auto* I64组 = std::get_if<std::vector<std::int64_t>>(&信息->材料);
    if (!结构化 || !I64组 || I64组->size() != 结构化->分量.size()) {
        return false;
    }
    for (std::size_t i = 0; i < I64组->size(); ++i) {
        if (!I64命中值域(结构化->分量[i].值域, (*I64组)[i])) return false;
    }
    return true;
}

bool 准确值材料类型相容(
    特征概念材料物理类型 材料类型,
    const 新特征准确值& 值,
    const 新_特征值类& 特征值服务) noexcept {
    if (材料类型 == 特征概念材料物理类型::I64标量) {
        return std::holds_alternative<std::int64_t>(值);
    }
    const auto 期望类型 = 转换特征值材料类型(材料类型);
    const auto* 值节点 = std::get_if<稳定编码>(&值);
    if (!期望类型 || !值节点 || !有效(*值节点)) return false;
    const auto 信息 = 特征值服务.获取特征值(*值节点);
    return 信息 && 信息->物理类型 == *期望类型;
}

std::vector<std::int64_t> 展平I64值域(
    const std::vector<特征概念I64闭区间>& 区间组) {
    std::vector<std::int64_t> 结果;
    结果.reserve(区间组.size() * 2);
    for (const auto& 区间 : 区间组) {
        结果.push_back(区间.下界);
        结果.push_back(区间.上界);
    }
    return 结果;
}

bool 写入值域内容(
    稳定编码 值域节点,
    const 特征概念值域& 值域,
    std::vector<稳定编码>& 已写字段,
    std::vector<稳定编码>& 已建分量节点) {
    if (const auto* I64域 =
        std::get_if<std::vector<特征概念I64闭区间>>(&值域)) {
        const auto 关系 = 全局基础数据集.添加字段值(
            值域节点, I64值域字段,
            基础值{基础原始值{展平I64值域(*I64域)}});
        if (有效(关系)) 已写字段.push_back(关系);
        return 有效(关系);
    }
    if (std::holds_alternative<特征概念全材料值域>(值域)) {
        const auto 关系 = 全局基础数据集.添加字段值(
            值域节点, 全材料值域字段,
            基础值{基础原始值{std::int64_t{1}}});
        if (有效(关系)) 已写字段.push_back(关系);
        return 有效(关系);
    }
    if (const auto* 节点组 = std::get_if<std::vector<稳定编码>>(&值域)) {
        for (const auto 值节点 : *节点组) {
            const auto 关系 = 全局基础数据集.添加字段节点(
                值域节点, 非I64值域字段, 值节点);
            if (!有效(关系)) return false;
            已写字段.push_back(关系);
        }
        return true;
    }
    const auto& 结构化 = std::get<特征概念结构化I64值域>(值域);
    for (std::size_t 顺序 = 0; 顺序 < 结构化.分量.size(); ++顺序) {
        const auto& 分量 = 结构化.分量[顺序];
        if (!节点仍存在(分量.角色)
            || (分量.单位 && !节点仍存在(*分量.单位))
            || 顺序 > static_cast<std::size_t>(
                (std::numeric_limits<std::int64_t>::max)())) {
            return false;
        }
        const auto 分量节点 = 全局基础数据集.新建节点();
        if (!有效(分量节点)) return false;
        已建分量节点.push_back(分量节点);
        const auto 写分量节点字段 = [&](稳定编码 字段, 稳定编码 目标) {
            const auto 关系 = 全局基础数据集.添加字段节点(
                分量节点, 字段, 目标);
            if (有效(关系)) 已写字段.push_back(关系);
            return 有效(关系);
        };
        const auto 类型关系 = 全局基础数据集.添加字段节点(
            分量节点, 节点类型字段, 结构化分量节点类型);
        if (有效(类型关系)) 已写字段.push_back(类型关系);
        const auto 顺序关系 = 有效(类型关系)
            ? 全局基础数据集.添加字段值(
                分量节点, 分量顺序字段,
                基础值{基础原始值{static_cast<std::int64_t>(顺序)}})
            : 稳定编码{};
        if (有效(顺序关系)) 已写字段.push_back(顺序关系);
        const auto 角色关系 = 有效(顺序关系)
            ? 写分量节点字段(分量角色字段, 分量.角色)
            : false;
        const auto 值域关系 = 角色关系
            ? 全局基础数据集.添加字段值(
                分量节点, 分量I64值域字段,
                基础值{基础原始值{展平I64值域(分量.值域)}})
            : 稳定编码{};
        if (有效(值域关系)) 已写字段.push_back(值域关系);
        bool 完成 = 有效(值域关系);
        if (完成 && 分量.单位) {
            完成 = 写分量节点字段(分量单位字段, *分量.单位);
        }
        if (完成) {
            const auto 分量关系 = 全局基础数据集.添加字段节点(
                值域节点, 结构化分量字段, 分量节点);
            if (有效(分量关系)) 已写字段.push_back(分量关系);
            完成 = 有效(分量关系);
        }
        if (!完成) return false;
    }
    return !结构化.分量.empty();
}

std::optional<特征概念值域> 读取值域节点(
    稳定编码 值域节点,
    特征概念材料物理类型 材料类型) {
    const auto 类型 = 读取唯一节点字段(值域节点, 节点类型字段);
    if (!类型 || *类型 != 值域节点类型) return std::nullopt;
    const auto I64字段组 = 全局基础数据集.查询字段(
        值域节点, I64值域字段);
    const auto 全材料字段组 = 全局基础数据集.查询字段(
        值域节点, 全材料值域字段);
    const auto 非I64字段组 = 全局基础数据集.查询字段(
        值域节点, 非I64值域字段);
    const auto 结构分量组 = 全局基础数据集.查询字段(
        值域节点, 结构化分量字段);
    const auto 表示数量 = static_cast<int>(!I64字段组.empty())
        + static_cast<int>(!全材料字段组.empty())
        + static_cast<int>(!非I64字段组.empty())
        + static_cast<int>(!结构分量组.empty());
    if (表示数量 != 1) return std::nullopt;

    if (!I64字段组.empty()) {
        if (材料类型 != 特征概念材料物理类型::I64标量
            || I64字段组.size() != 1) return std::nullopt;
        const auto* 值 = std::get_if<基础值>(&I64字段组.front().内容);
        if (!值) return std::nullopt;
        const auto* 展平 = std::get_if<std::vector<std::int64_t>>(&值->材料);
        if (!展平 || 展平->empty() || 展平->size() % 2 != 0) return std::nullopt;
        std::vector<特征概念I64闭区间> 区间组;
        for (std::size_t i = 0; i < 展平->size(); i += 2) {
            if ((*展平)[i] > (*展平)[i + 1]) return std::nullopt;
            区间组.push_back({(*展平)[i], (*展平)[i + 1]});
        }
        return 特征概念值域{std::move(区间组)};
    }
    if (!全材料字段组.empty()) {
        if (材料类型 == 特征概念材料物理类型::I64标量
            || 全材料字段组.size() != 1) return std::nullopt;
        const auto* 值 = std::get_if<基础值>(&全材料字段组.front().内容);
        const auto* 标记 = 值 ? std::get_if<std::int64_t>(&值->材料) : nullptr;
        return 标记 && *标记 == 1
            ? std::optional<特征概念值域>{特征概念全材料值域{}}
            : std::nullopt;
    }
    if (!非I64字段组.empty()) {
        if (材料类型 == 特征概念材料物理类型::I64标量) return std::nullopt;
        std::vector<稳定编码> 节点组;
        for (const auto& 字段 : 非I64字段组) {
            const auto* 目标 = std::get_if<稳定编码>(&字段.内容);
            if (!目标) return std::nullopt;
            节点组.push_back(*目标);
        }
        std::sort(节点组.begin(), 节点组.end());
        const auto 去重末尾 = std::unique(节点组.begin(), 节点组.end());
        if (去重末尾 != 节点组.end()) return std::nullopt;
        return 特征概念值域{std::move(节点组)};
    }

    if (材料类型 != 特征概念材料物理类型::I64数组) return std::nullopt;
    std::vector<std::pair<std::size_t, 特征概念I64分量定义>> 有序分量;
    有序分量.reserve(结构分量组.size());
    for (const auto& 分量字段 : 结构分量组) {
        const auto* 分量节点 = std::get_if<稳定编码>(&分量字段.内容);
        if (!分量节点 || !节点仍存在(*分量节点)) return std::nullopt;
        const auto 分量类型 = 读取唯一节点字段(*分量节点, 节点类型字段);
        const auto 顺序字段组 = 全局基础数据集.查询字段(
            *分量节点, 分量顺序字段);
        const auto 角色 = 读取唯一节点字段(*分量节点, 分量角色字段);
        const auto 分量域字段组 = 全局基础数据集.查询字段(
            *分量节点, 分量I64值域字段);
        if (!分量类型 || *分量类型 != 结构化分量节点类型
            || 顺序字段组.size() != 1 || !角色
            || 分量域字段组.size() != 1) return std::nullopt;
        const auto* 顺序值 = std::get_if<基础值>(&顺序字段组.front().内容);
        const auto* 顺序 = 顺序值
            ? std::get_if<std::int64_t>(&顺序值->材料) : nullptr;
        const auto* 域值 = std::get_if<基础值>(&分量域字段组.front().内容);
        const auto* 展平 = 域值
            ? std::get_if<std::vector<std::int64_t>>(&域值->材料) : nullptr;
        if (!顺序 || *顺序 < 0 || !展平 || 展平->empty()
            || 展平->size() % 2 != 0) return std::nullopt;
        特征概念I64分量定义 分量;
        分量.角色 = *角色;
        for (std::size_t i = 0; i < 展平->size(); i += 2) {
            if ((*展平)[i] > (*展平)[i + 1]) return std::nullopt;
            分量.值域.push_back({(*展平)[i], (*展平)[i + 1]});
        }
        const auto 单位字段组 = 全局基础数据集.查询字段(
            *分量节点, 分量单位字段);
        if (单位字段组.size() > 1) return std::nullopt;
        if (!单位字段组.empty()) {
            const auto* 单位 = std::get_if<稳定编码>(&单位字段组.front().内容);
            if (!单位 || !节点仍存在(*单位)) return std::nullopt;
            分量.单位 = *单位;
        }
        有序分量.emplace_back(static_cast<std::size_t>(*顺序), std::move(分量));
    }
    std::sort(有序分量.begin(), 有序分量.end(),
        [](const auto& 左, const auto& 右) { return 左.first < 右.first; });
    特征概念结构化I64值域 结构化;
    for (std::size_t i = 0; i < 有序分量.size(); ++i) {
        if (有序分量[i].first != i) return std::nullopt;
        结构化.分量.push_back(std::move(有序分量[i].second));
    }
    return 特征概念值域{std::move(结构化)};
}

// 本类建立的值域节点是实例私有实现节点，身份不向公开DTO暴露，也不共享。
// 因而引用切换后旧节点已脱离活动特征；回收失败最多留下不可达孤儿。
void 删除值域节点(稳定编码 值域节点) noexcept {
    std::vector<稳定编码> 分量节点组;
    for (const auto& 字段 : 全局基础数据集.查询字段(
        值域节点, 结构化分量字段)) {
        if (const auto* 分量 = std::get_if<稳定编码>(&字段.内容)) {
            分量节点组.push_back(*分量);
        }
    }
    const std::array 值域字段组{
        节点类型字段, I64值域字段, 全材料值域字段,
        非I64值域字段, 结构化分量字段};
    for (const auto 字段 : 值域字段组) {
        for (const auto& 关系 : 全局基础数据集.查询字段(值域节点, 字段)) {
            (void)全局基础数据集.删除字段(关系.编码);
        }
    }
    for (const auto 分量节点 : 分量节点组) {
        const std::array 分量字段组{
            节点类型字段, 分量顺序字段, 分量角色字段,
            分量单位字段, 分量I64值域字段};
        for (const auto 字段 : 分量字段组) {
            for (const auto& 关系 : 全局基础数据集.查询字段(分量节点, 字段)) {
                (void)全局基础数据集.删除字段(关系.编码);
            }
        }
        (void)全局基础数据集.删除节点(分量节点);
    }
    (void)全局基础数据集.删除节点(值域节点);
}

稳定编码 建立值域节点(const 特征概念值域& 值域) noexcept {
    const auto 节点 = 全局基础数据集.新建节点();
    if (!有效(节点)) return {};
    std::vector<稳定编码> 已写字段;
    std::vector<稳定编码> 已建分量节点;
    const auto 类型关系 = 全局基础数据集.添加字段节点(
        节点, 节点类型字段, 值域节点类型);
    if (有效(类型关系)) 已写字段.push_back(类型关系);
    if (有效(类型关系)
        && 写入值域内容(节点, 值域, 已写字段, 已建分量节点)) {
        return 节点;
    }
    for (auto 位置 = 已写字段.rbegin(); 位置 != 已写字段.rend(); ++位置) {
        (void)全局基础数据集.删除字段(*位置);
    }
    for (auto 位置 = 已建分量节点.rbegin();
        位置 != 已建分量节点.rend(); ++位置) {
        (void)全局基础数据集.删除节点(*位置);
    }
    (void)全局基础数据集.删除节点(节点);
    return {};
}

bool 发布历史聚合值域(
    稳定编码 特征节点,
    特征概念材料物理类型 材料类型,
    const 特征概念值域& 新值域) noexcept {
    const auto 新值域节点 = 建立值域节点(新值域);
    if (!有效(新值域节点)) return false;
    const auto 新值域读回 = 读取值域节点(新值域节点, 材料类型);
    if (!新值域读回 || *新值域读回 != 新值域) {
        删除值域节点(新值域节点);
        return false;
    }
    const auto 原引用组 = 全局基础数据集.查询字段(
        特征节点, 实例历史聚合值域字段);
    if (原引用组.size() > 1) {
        删除值域节点(新值域节点);
        return false;
    }
    if (原引用组.empty()) {
        const auto 新引用 = 全局基础数据集.添加字段节点(
            特征节点, 实例历史聚合值域字段, 新值域节点);
        if (!有效(新引用)) {
            删除值域节点(新值域节点);
            return false;
        }
        return true;
    }
    const auto* 原值域节点 = std::get_if<稳定编码>(&原引用组.front().内容);
    if (!原值域节点 || !全局基础数据集.修改字段节点(
        原引用组.front().编码, 新值域节点)) {
        删除值域节点(新值域节点);
        return false;
    }
    删除值域节点(*原值域节点);
    return true;
}

std::optional<特征概念值域> 历史记录形成有限值域(
    特征概念材料物理类型 材料类型,
    const std::vector<历史值命中物理记录>& 记录组) {
    if (记录组.empty()) return std::nullopt;
    if (材料类型 == 特征概念材料物理类型::I64标量) {
        std::vector<特征概念I64闭区间> 区间组;
        for (const auto& 记录 : 记录组) {
            const auto* I64 = std::get_if<std::int64_t>(&记录.值);
            if (!I64) return std::nullopt;
            区间组.push_back({*I64, *I64});
        }
        std::sort(区间组.begin(), 区间组.end(), [](const auto& 左, const auto& 右) {
            return 左.下界 < 右.下界;
        });
        std::vector<特征概念I64闭区间> 归并;
        for (const auto& 区间 : 区间组) {
            const bool 相邻 = !归并.empty()
                && 归并.back().上界 != (std::numeric_limits<std::int64_t>::max)()
                && 区间.下界 == 归并.back().上界 + 1;
            if (归并.empty() || (!相邻 && 区间.下界 > 归并.back().上界)) {
                归并.push_back(区间);
            } else if (区间.上界 > 归并.back().上界) {
                归并.back().上界 = 区间.上界;
            }
        }
        return 特征概念值域{std::move(归并)};
    }
    std::vector<稳定编码> 节点组;
    for (const auto& 记录 : 记录组) {
        const auto* 节点 = std::get_if<稳定编码>(&记录.值);
        if (!节点) return std::nullopt;
        节点组.push_back(*节点);
    }
    std::sort(节点组.begin(), 节点组.end());
    节点组.erase(std::unique(节点组.begin(), 节点组.end()), 节点组.end());
    return 特征概念值域{std::move(节点组)};
}

bool 概念包含全部历史值(
    const 特征概念信息& 概念,
    const std::vector<历史值命中物理记录>& 记录组,
    const 新_特征值类& 特征值服务) noexcept {
    return !记录组.empty()
        && std::all_of(记录组.begin(), 记录组.end(), [&](const auto& 记录) {
            return 准确值命中值域(
                概念.材料类型, 概念.值域, 记录.值, 特征值服务);
        });
}

std::optional<稳定编码> 查找最具体的历史包含概念(
    稳定编码 起点,
    const std::vector<历史值命中物理记录>& 记录组,
    const 新_特征值类& 特征值服务,
    const 概念_特征类& 特征概念服务) {
    struct 待查项 final {
        稳定编码 节点;
        std::size_t 深度 = 0;
    };
    std::vector<待查项> 待查{{起点, 0}};
    std::vector<稳定编码> 已查;
    std::optional<待查项> 最佳;
    while (!待查.empty()) {
        const auto 当前 = 待查.back();
        待查.pop_back();
        if (std::find(已查.begin(), 已查.end(), 当前.节点) != 已查.end()) {
            return std::nullopt;
        }
        已查.push_back(当前.节点);
        const auto 信息 = 特征概念服务.获取特征概念(当前.节点);
        if (!信息 || !概念包含全部历史值(*信息, 记录组, 特征值服务)) {
            continue;
        }
        if (!最佳 || 当前.深度 > 最佳->深度
            || (当前.深度 == 最佳->深度 && 当前.节点 < 最佳->节点)) {
            最佳 = 当前;
        }
        for (const auto& 关系 : 全局基础数据集.查询源关系(
            当前.节点, 基础外部关系类型::父子)) {
            待查.push_back({关系.目标节点, 当前.深度 + 1});
        }
    }
    return 最佳 ? std::optional<稳定编码>{最佳->节点} : std::nullopt;
}

bool 切换实例所属概念(
    稳定编码 特征节点,
    稳定编码 新概念节点,
    概念_特征类& 特征概念服务) noexcept {
    const auto 字段组 = 全局基础数据集.查询字段(特征节点, 特征概念字段);
    if (字段组.size() != 1) return false;
    const auto* 原概念 = std::get_if<稳定编码>(&字段组.front().内容);
    if (!原概念) return false;
    if (*原概念 == 新概念节点) return true;
    if (!特征概念服务.增加当前实例引用(新概念节点)) {
        return false;
    }
    if (!全局基础数据集.修改字段节点(
        字段组.front().编码, 新概念节点)) {
        (void)特征概念服务.减少当前实例引用(新概念节点);
        return false;
    }
    if (特征概念服务.减少当前实例引用(*原概念)) return true;
    if (!全局基础数据集.修改字段节点(
        字段组.front().编码, *原概念)) {
        return false;
    }
    (void)特征概念服务.减少当前实例引用(新概念节点);
    return false;
}

std::optional<特征概念值域> 合并同表示值域(
    const 特征概念值域& 左,
    const 特征概念值域& 右) {
    if (std::holds_alternative<特征概念全材料值域>(左)
        || std::holds_alternative<特征概念全材料值域>(右)) {
        return 特征概念值域{特征概念全材料值域{}};
    }
    if (const auto* 左I64 =
        std::get_if<std::vector<特征概念I64闭区间>>(&左)) {
        const auto* 右I64 =
            std::get_if<std::vector<特征概念I64闭区间>>(&右);
        if (!右I64) return std::nullopt;
        auto 合并 = *左I64;
        合并.insert(合并.end(), 右I64->begin(), 右I64->end());
        std::sort(合并.begin(), 合并.end(), [](const auto& a, const auto& b) {
            return a.下界 < b.下界 || (a.下界 == b.下界 && a.上界 < b.上界);
        });
        std::vector<特征概念I64闭区间> 规范;
        for (const auto& 区间 : 合并) {
            const bool 相邻 = !规范.empty()
                && 规范.back().上界 != (std::numeric_limits<std::int64_t>::max)()
                && 区间.下界 == 规范.back().上界 + 1;
            if (规范.empty() || (!相邻 && 区间.下界 > 规范.back().上界)) {
                规范.push_back(区间);
            } else {
                规范.back().上界 = (std::max)(规范.back().上界, 区间.上界);
            }
        }
        return 特征概念值域{std::move(规范)};
    }
    if (const auto* 左节点 = std::get_if<std::vector<稳定编码>>(&左)) {
        const auto* 右节点 = std::get_if<std::vector<稳定编码>>(&右);
        if (!右节点) return std::nullopt;
        auto 合并 = *左节点;
        合并.insert(合并.end(), 右节点->begin(), 右节点->end());
        std::sort(合并.begin(), 合并.end());
        合并.erase(std::unique(合并.begin(), 合并.end()), 合并.end());
        return 特征概念值域{std::move(合并)};
    }
    return std::nullopt;
}

bool 整理变化概念的兄弟层级(
    稳定编码 特征节点,
    稳定编码 当前概念,
    概念_特征类& 特征概念服务) noexcept {
    const auto 当前信息 = 特征概念服务.获取特征概念(当前概念);
    const auto 父关系 = 全局基础数据集.查询目标关系(
        当前概念, 基础外部关系类型::父子);
    if (!当前信息 || 父关系.size() != 1) return false;
    const auto 父节点 = 父关系.front().源节点;
    std::vector<稳定编码> 包含当前的兄弟;
    std::vector<基础外部关系> 当前包含的兄弟;
    for (const auto& 关系 : 全局基础数据集.查询源关系(
        父节点, 基础外部关系类型::父子)) {
        if (关系.目标节点 == 当前概念) continue;
        const auto 兄弟 = 特征概念服务.获取特征概念(关系.目标节点);
        if (!兄弟 || 兄弟->材料类型 != 当前信息->材料类型
            || 兄弟->单位 != 当前信息->单位) {
            continue;
        }
        const auto 比较 = 特征概念服务.比较值域(
            当前信息->材料类型, 当前信息->值域, 兄弟->值域);
        if (比较.状态 != 特征概念规则状态::已完成 || !比较.关系) {
            continue;
        }
        if (*比较.关系 == 特征概念值域关系::相等) {
            return 切换实例所属概念(
                特征节点, 关系.目标节点, 特征概念服务);
        }
        if (*比较.关系 == 特征概念值域关系::左包含右) {
            当前包含的兄弟.push_back(关系);
        } else if (*比较.关系 == 特征概念值域关系::右包含左) {
            包含当前的兄弟.push_back(关系.目标节点);
        }
    }
    for (const auto& 关系 : 当前包含的兄弟) {
        if (!全局基础数据集.修改关系(
            关系.编码, 当前概念, 关系.目标节点,
            基础外部关系类型::父子, 关系.角色或顺序)) {
            return false;
        }
    }
    if (!包含当前的兄弟.empty()) {
        std::sort(包含当前的兄弟.begin(), 包含当前的兄弟.end());
        const auto 新父 = 包含当前的兄弟.front();
        if (!全局基础数据集.修改关系(
            父关系.front().编码, 新父, 当前概念,
            基础外部关系类型::父子, 父关系.front().角色或顺序)) {
            return false;
        }
    }
    return true;
}

bool 同步实例所属概念(
    稳定编码 特征节点,
    稳定编码 当前概念,
    const 特征概念值域& 历史值域,
    const std::vector<历史值命中物理记录>& 记录组,
    新_特征值类& 特征值服务,
    概念_特征类& 特征概念服务) noexcept {
    const auto 当前信息 = 特征概念服务.获取特征概念(当前概念);
    const auto 定位 = 特征概念服务.比较单特征概念(当前概念, 当前概念);
    if (!当前信息 || 定位.状态 != 特征概念规则状态::已完成
        || !定位.特征类型根) {
        return false;
    }
    const auto 当前对历史 = 特征概念服务.比较值域(
        当前信息->材料类型, 当前信息->值域, 历史值域);
    if (当前信息->值域 == 历史值域) {
        const auto 父关系 = 全局基础数据集.查询目标关系(
            当前概念, 基础外部关系类型::父子);
        if (父关系.size() != 1) return false;
        if (父关系.front().源节点 == 特征概念树) return true;

        auto 新父 = 父关系.front().源节点;
        const auto 直接上位 = 特征概念服务.获取特征概念(新父);
        if (!直接上位) return false;
        const auto 上位对当前 = 特征概念服务.比较值域(
            当前信息->材料类型, 直接上位->值域, 当前信息->值域);
        if (上位对当前.状态 == 特征概念规则状态::已完成
            && 上位对当前.关系 == 特征概念值域关系::相等) {
            return 切换实例所属概念(
                特征节点, 新父, 特征概念服务);
        }
        if (概念包含全部历史值(*直接上位, 记录组, 特征值服务)) {
            return true;
        }

        for (;;) {
            const auto 上级关系 = 全局基础数据集.查询目标关系(
                新父, 基础外部关系类型::父子);
            if (上级关系.size() != 1) return false;
            新父 = 上级关系.front().源节点;
            if (新父 == 特征概念树) break;
            const auto 上级信息 = 特征概念服务.获取特征概念(新父);
            if (!上级信息) return false;
            if (概念包含全部历史值(*上级信息, 记录组, 特征值服务)) {
                if (上级信息->值域 == 当前信息->值域) {
                    return 切换实例所属概念(
                        特征节点, 新父, 特征概念服务);
                }
                break;
            }
        }
        if (新父 == 特征概念树) {
            const auto 根信息 = 特征概念服务.获取特征概念(*定位.特征类型根);
            if (!根信息) return false;
            if (!概念包含全部历史值(*根信息, 记录组, 特征值服务)) {
                const auto 根新值域 = 合并同表示值域(
                    根信息->值域, 当前信息->值域);
                if (!根新值域 || !特征概念服务.更新特征概念值域(
                        *定位.特征类型根, *根新值域, 特征值服务)) {
                    return false;
                }
            }
            新父 = *定位.特征类型根;
        }
        if (新父 == 当前概念) return true;
        const auto 新父信息 = 特征概念服务.获取特征概念(新父);
        if (!新父信息) return false;
        if (新父信息->值域 == 当前信息->值域) {
            return 切换实例所属概念(
                特征节点, 新父, 特征概念服务);
        }
        if (!全局基础数据集.修改关系(
            父关系.front().编码, 新父, 当前概念,
            基础外部关系类型::父子, 父关系.front().角色或顺序)) {
            return false;
        }
        return 整理变化概念的兄弟层级(
            特征节点, 当前概念, 特征概念服务);
    }

    if (概念包含全部历史值(*当前信息, 记录组, 特征值服务)) {
        const auto 最具体 = 查找最具体的历史包含概念(
            当前概念, 记录组, 特征值服务, 特征概念服务);
        if (!最具体) return false;
        const auto 最具体信息 = 特征概念服务.获取特征概念(*最具体);
        if (!最具体信息) return false;
        if (最具体信息->值域 == 历史值域) {
            return 切换实例所属概念(
                特征节点, *最具体, 特征概念服务);
        }
        特征概念定义 定义;
        定义.材料类型 = 最具体信息->材料类型;
        定义.值域 = 历史值域;
        定义.单位 = 最具体信息->单位;
        定义.比较规则 = 最具体信息->比较规则;
        定义.聚合规则 = 最具体信息->聚合规则;
        const auto 子概念 = 特征概念服务.建立或取得特征概念(
            定义, 特征值服务, *最具体);
        return 有效(子概念) && 切换实例所属概念(
            特征节点, 子概念, 特征概念服务);
    }

    const bool 历史扩展当前 =
        当前对历史.状态 == 特征概念规则状态::已完成
        && 当前对历史.关系 == 特征概念值域关系::右包含左;
    if (!历史扩展当前) {
        auto 最具体 = 查找最具体的历史包含概念(
            *定位.特征类型根, 记录组, 特征值服务, 特征概念服务);
        if (!最具体) {
            const auto 根信息 = 特征概念服务.获取特征概念(*定位.特征类型根);
            const auto 根新值域 = 根信息
                ? 合并同表示值域(根信息->值域, 历史值域) : std::nullopt;
            if (!根新值域 || !特征概念服务.更新特征概念值域(
                    *定位.特征类型根, *根新值域, 特征值服务)) {
                return false;
            }
            最具体 = *定位.特征类型根;
        }
        const auto 上位 = 特征概念服务.获取特征概念(*最具体);
        if (!上位) return false;
        if (上位->值域 == 历史值域) {
            return 切换实例所属概念(
                特征节点, *最具体, 特征概念服务);
        }
        特征概念定义 定义;
        定义.材料类型 = 上位->材料类型;
        定义.值域 = 历史值域;
        定义.单位 = 上位->单位;
        定义.比较规则 = 上位->比较规则;
        定义.聚合规则 = 上位->聚合规则;
        const auto 新概念 = 特征概念服务.建立或取得特征概念(
            定义, 特征值服务, *最具体);
        return 有效(新概念) && 切换实例所属概念(
            特征节点, 新概念, 特征概念服务);
    }

    auto 父关系 = 全局基础数据集.查询目标关系(
        当前概念, 基础外部关系类型::父子);
    if (父关系.size() != 1) return false;
    const auto 原父关系 = 父关系.front();
    稳定编码 新父 = 原父关系.源节点;
    while (新父 != 特征概念树) {
        const auto 父信息 = 特征概念服务.获取特征概念(新父);
        if (!父信息) return false;
        if (概念包含全部历史值(*父信息, 记录组, 特征值服务)) break;
        const auto 上级 = 全局基础数据集.查询目标关系(
            新父, 基础外部关系类型::父子);
        if (上级.size() != 1) return false;
        新父 = 上级.front().源节点;
    }
    if (新父 == 特征概念树 && 当前概念 != *定位.特征类型根) {
        const auto 根信息 = 特征概念服务.获取特征概念(*定位.特征类型根);
        if (!根信息) return false;
        if (!概念包含全部历史值(*根信息, 记录组, 特征值服务)) {
            const auto 根新值域 = 合并同表示值域(根信息->值域, 历史值域);
            if (!根新值域 || !特征概念服务.更新特征概念值域(
                    *定位.特征类型根, *根新值域, 特征值服务)) {
                return false;
            }
        }
        新父 = *定位.特征类型根;
    }
    if (新父 != 特征概念树) {
        const auto 新父信息 = 特征概念服务.获取特征概念(新父);
        if (!新父信息) return false;
        if (新父信息->值域 == 历史值域) {
            return 切换实例所属概念(
                特征节点, 新父, 特征概念服务);
        }
    }
    bool 已改父 = false;
    if (当前概念 != *定位.特征类型根 && 新父 != 原父关系.源节点) {
        if (!全局基础数据集.修改关系(
            原父关系.编码, 新父, 当前概念,
            基础外部关系类型::父子, 原父关系.角色或顺序)) {
            return false;
        }
        已改父 = true;
    }
    if (!特征概念服务.更新特征概念值域(
        当前概念, 历史值域, 特征值服务)) {
        if (已改父) {
            (void)全局基础数据集.修改关系(
                原父关系.编码, 原父关系.源节点, 当前概念,
                基础外部关系类型::父子, 原父关系.角色或顺序);
        }
        return false;
    }
    return 整理变化概念的兄弟层级(
        特征节点, 当前概念, 特征概念服务);
}

std::vector<稳定编码> 查询归组子特征(稳定编码 父特征节点) {
    std::vector<稳定编码> 结果;
    for (const auto& 关系 : 全局基础数据集.查询源关系(
        父特征节点, 基础外部关系类型::父子)) {
        const auto 标记 = 读取唯一I64字段(
            关系.目标节点, 归组子特征标记字段);
        if (标记 && *标记 == 1) 结果.push_back(关系.目标节点);
    }
    std::sort(结果.begin(), 结果.end());
    return 结果;
}

std::int64_t 计算饱和有向差值(
    std::int64_t 左值, std::int64_t 右值) noexcept {
    const auto 最小 = (std::numeric_limits<std::int64_t>::min)();
    const auto 最大 = (std::numeric_limits<std::int64_t>::max)();
    if (右值 > 0 && 左值 < 最小 + 右值) return 最小;
    if (右值 < 0 && 左值 > 最大 + 右值) return 最大;
    return 左值 - 右值;
}

std::uint64_t I64绝对量(std::int64_t 值) noexcept {
    if (值 >= 0) return static_cast<std::uint64_t>(值);
    return static_cast<std::uint64_t>(-(值 + 1)) + 1;
}

std::int64_t 计算单维相对相似度(
    std::int64_t 左值, std::int64_t 右值) noexcept {
    if (左值 == 右值) return 10000;
    if ((左值 < 0) != (右值 < 0)) return 0;
    const auto 左量 = I64绝对量(左值);
    const auto 右量 = I64绝对量(右值);
    const auto 较大 = (std::max)(左量, 右量);
    const auto 较小 = (std::min)(左量, 右量);
    if (较大 == 0) return 10000;
    const auto 比例 = static_cast<long double>(较小)
        / static_cast<long double>(较大);
    const auto 分数 = static_cast<std::int64_t>(比例 * 10000.0L);
    return (std::clamp)(分数, std::int64_t{0}, std::int64_t{10000});
}

std::optional<std::int64_t> 计算多维相似度(
    const std::vector<std::int64_t>& 左值,
    const std::vector<std::int64_t>& 右值) noexcept {
    if (左值.empty() || 左值.size() != 右值.size()
        || 左值.size() > (std::numeric_limits<std::uint64_t>::max)() / 10000) {
        return std::nullopt;
    }
    std::uint64_t 总分 = 0;
    for (std::size_t i = 0; i < 左值.size(); ++i) {
        总分 += static_cast<std::uint64_t>(
            计算单维相对相似度(左值[i], 右值[i]));
    }
    return static_cast<std::int64_t>(总分 / 左值.size());
}

std::optional<std::int64_t> 计算RGB相似度(
    const std::vector<std::int64_t>& 左值,
    const std::vector<std::int64_t>& 右值) noexcept {
    if (左值.size() != 3 || 右值.size() != 3) return std::nullopt;
    std::uint64_t 总差 = 0;
    for (std::size_t i = 0; i < 3; ++i) {
        if (左值[i] < 0 || 左值[i] > 255 || 右值[i] < 0 || 右值[i] > 255) {
            return std::nullopt;
        }
        总差 += static_cast<std::uint64_t>(
            左值[i] >= 右值[i] ? 左值[i] - 右值[i] : 右值[i] - 左值[i]);
    }
    constexpr std::uint64_t 最大总差 = 3 * 255;
    const auto 扣分 = (总差 * 10000 + 最大总差 / 2) / 最大总差;
    return static_cast<std::int64_t>(10000 - (std::min)(扣分, std::uint64_t{10000}));
}

bool 读取二值位(
    const std::vector<std::uint64_t>& 材料,
    std::size_t 位置) noexcept {
    return 位置 / 64 < 材料.size()
        && (材料[位置 / 64] & (std::uint64_t{1} << (位置 % 64))) != 0;
}

void 写入二值位(
    std::vector<std::uint64_t>& 材料,
    std::size_t 位置) noexcept {
    if (位置 / 64 < 材料.size()) {
        材料[位置 / 64] |= std::uint64_t{1} << (位置 % 64);
    }
}

std::optional<std::size_t> 推导二维边长(
    const std::vector<std::uint64_t>& 材料) noexcept {
    if (材料.empty()
        || 材料.size() > (std::numeric_limits<std::size_t>::max)() / 64) {
        return std::nullopt;
    }
    const auto 总位数 = 材料.size() * 64;
    std::size_t 边长 = 8;
    for (;;) {
        if (边长 > (std::numeric_limits<std::size_t>::max)() / 边长) {
            return std::nullopt;
        }
        const auto 当前位数 = 边长 * 边长;
        if (当前位数 == 总位数) return 边长;
        if (当前位数 > 总位数
            || 边长 > (std::numeric_limits<std::size_t>::max)() / 2) {
            return std::nullopt;
        }
        边长 *= 2;
    }
}

std::optional<std::size_t> 推导三维边长(
    const std::vector<std::uint64_t>& 材料) noexcept {
    if (材料.empty()
        || 材料.size() > (std::numeric_limits<std::size_t>::max)() / 64) {
        return std::nullopt;
    }
    const auto 总位数 = 材料.size() * 64;
    std::size_t 边长 = 4;
    for (;;) {
        if (边长 > (std::numeric_limits<std::size_t>::max)() / 边长) {
            return std::nullopt;
        }
        const auto 平方 = 边长 * 边长;
        if (边长 != 0 && 平方 > (std::numeric_limits<std::size_t>::max)() / 边长) {
            return std::nullopt;
        }
        const auto 当前位数 = 平方 * 边长;
        if (当前位数 == 总位数) return 边长;
        if (当前位数 > 总位数
            || 边长 > (std::numeric_limits<std::size_t>::max)() / 2) {
            return std::nullopt;
        }
        边长 *= 2;
    }
}

std::uint8_t 读取灰度(
    const std::vector<std::uint64_t>& 材料,
    std::size_t 位置) noexcept {
    const auto 位移 = static_cast<unsigned>((位置 % 8) * 8);
    return static_cast<std::uint8_t>((材料[位置 / 8] >> 位移) & 0xFF);
}

void 写入灰度(
    std::vector<std::uint64_t>& 材料,
    std::size_t 位置,
    std::uint8_t 灰度) noexcept {
    const auto 位移 = static_cast<unsigned>((位置 % 8) * 8);
    const auto 掩码 = std::uint64_t{0xFF} << 位移;
    材料[位置 / 8] = (材料[位置 / 8] & ~掩码)
        | (static_cast<std::uint64_t>(灰度) << 位移);
}

bool 二维外圈空白(
    const std::vector<std::uint64_t>& 原始材料,
    std::size_t 边长) noexcept {
    for (std::size_t i = 0; i < 边长; ++i) {
        if (读取二值位(原始材料, i)
            || 读取二值位(原始材料, (边长 - 1) * 边长 + i)
            || 读取二值位(原始材料, i * 边长)
            || 读取二值位(原始材料, i * 边长 + 边长 - 1)) {
            return false;
        }
    }
    return true;
}

std::optional<std::vector<std::uint64_t>> 从原图生成二维面积层(
    const std::vector<std::uint64_t>& 原始材料,
    std::size_t 原始边长,
    std::size_t 目标边长) {
    if (原始边长 < 8 || 目标边长 < 8 || 目标边长 > 原始边长
        || 原始边长 > (std::numeric_limits<std::uint64_t>::max)() / 原始边长) {
        return std::nullopt;
    }
    const auto 目标像素数 = 目标边长 * 目标边长;
    std::vector<std::uint64_t> 结果((目标像素数 + 7) / 8, 0);
    std::size_t 最小X = 原始边长;
    std::size_t 最小Y = 原始边长;
    std::size_t 最大X = 0;
    std::size_t 最大Y = 0;
    bool 有占用 = false;
    for (std::size_t y = 0; y < 原始边长; ++y) {
        for (std::size_t x = 0; x < 原始边长; ++x) {
            if (!读取二值位(原始材料, y * 原始边长 + x)) continue;
            有占用 = true;
            最小X = (std::min)(最小X, x);
            最小Y = (std::min)(最小Y, y);
            最大X = (std::max)(最大X, x);
            最大Y = (std::max)(最大Y, y);
        }
    }
    if (!有占用) return 结果;
    const bool 保留空白外圈 = 二维外圈空白(原始材料, 原始边长);
    const std::size_t 边距 = 保留空白外圈 ? 1 : 0;
    const std::size_t 可用边长 = 目标边长 - 边距 * 2;
    const std::size_t 原始宽 = 最大X - 最小X + 1;
    const std::size_t 原始高 = 最大Y - 最小Y + 1;
    std::size_t 目标宽 = 可用边长;
    std::size_t 目标高 = 可用边长;
    if (原始宽 >= 原始高) {
        目标高 = (std::max)(std::size_t{1},
            (原始高 * 可用边长 + 原始宽 / 2) / 原始宽);
    } else {
        目标宽 = (std::max)(std::size_t{1},
            (原始宽 * 可用边长 + 原始高 / 2) / 原始高);
    }
    const std::size_t 目标起点X = 边距 + (可用边长 - 目标宽) / 2;
    const std::size_t 目标起点Y = 边距 + (可用边长 - 目标高) / 2;
    if (原始宽 > (std::numeric_limits<std::uint64_t>::max)() / 原始高) {
        return std::nullopt;
    }
    const auto 单格总权重 = static_cast<std::uint64_t>(原始宽) * 原始高;
    for (std::size_t 目标Y = 0; 目标Y < 目标高; ++目标Y) {
        const auto 上边界 = 目标Y * 原始高;
        const auto 下边界 = (目标Y + 1) * 原始高;
        const auto 原始Y首 = 上边界 / 目标高;
        const auto 原始Y尾 = 下边界 / 目标高
            + static_cast<std::size_t>(下边界 % 目标高 != 0);
        for (std::size_t 目标X = 0; 目标X < 目标宽; ++目标X) {
            const auto 左边界 = 目标X * 原始宽;
            const auto 右边界 = (目标X + 1) * 原始宽;
            const auto 原始X首 = 左边界 / 目标宽;
            const auto 原始X尾 = 右边界 / 目标宽
                + static_cast<std::size_t>(右边界 % 目标宽 != 0);
            std::uint64_t 占用权重 = 0;
            for (std::size_t 原始Y = 原始Y首; 原始Y < 原始Y尾; ++原始Y) {
                const auto 像素上 = 原始Y * 目标高;
                const auto 像素下 = (原始Y + 1) * 目标高;
                const auto Y权重 = (std::min)(下边界, 像素下)
                    - (std::max)(上边界, 像素上);
                for (std::size_t 原始X = 原始X首; 原始X < 原始X尾; ++原始X) {
                    if (!读取二值位(原始材料,
                        (原始Y + 最小Y) * 原始边长 + 原始X + 最小X)) {
                        continue;
                    }
                    const auto 像素左 = 原始X * 目标宽;
                    const auto 像素右 = (原始X + 1) * 目标宽;
                    const auto X权重 = (std::min)(右边界, 像素右)
                        - (std::max)(左边界, 像素左);
                    占用权重 += static_cast<std::uint64_t>(X权重) * Y权重;
                }
            }
            const auto 灰度 = static_cast<std::uint8_t>(
                static_cast<long double>(占用权重) * 255.0L
                    / static_cast<long double>(单格总权重) + 0.5L);
            写入灰度(结果,
                (目标Y + 目标起点Y) * 目标边长 + 目标X + 目标起点X, 灰度);
        }
    }
    return 结果;
}

std::optional<std::vector<std::vector<std::uint64_t>>> 生成二维比较层级(
    const std::vector<std::uint64_t>& 叶子材料,
    std::size_t 叶子边长) {
    if (叶子边长 < 8 || 叶子边长 % 8 != 0
        || 叶子边长 > (std::numeric_limits<std::size_t>::max)() / 叶子边长) {
        return std::nullopt;
    }
    const auto 叶子位数 = 叶子边长 * 叶子边长;
    if (叶子材料.size() != (叶子位数 + 63) / 64) return std::nullopt;

    std::vector<std::vector<std::uint64_t>> 从根到叶;
    for (std::size_t 目标边长 = 8;; 目标边长 *= 2) {
        const auto 当前层 = 从原图生成二维面积层(
            叶子材料, 叶子边长, 目标边长);
        if (!当前层) return std::nullopt;
        从根到叶.push_back(*当前层);
        if (目标边长 == 叶子边长) break;
        if (目标边长 > 叶子边长 / 2) return std::nullopt;
    }
    return 从根到叶;
}

std::optional<std::int64_t> 计算灰度层相似度(
    const std::vector<std::uint64_t>& 左层,
    const std::vector<std::uint64_t>& 右层,
    std::size_t 像素数) noexcept {
    if (像素数 == 0
        || 像素数 > (std::numeric_limits<std::uint64_t>::max)() / 255
        || 左层.size() != 右层.size()
        || 左层.size() != (像素数 + 7) / 8) {
        return std::nullopt;
    }
    std::uint64_t 总差 = 0;
    for (std::size_t i = 0; i < 像素数; ++i) {
        const auto 左 = 读取灰度(左层, i);
        const auto 右 = 读取灰度(右层, i);
        总差 += 左 >= 右 ? 左 - 右 : 右 - 左;
    }
    const auto 最大总差 = static_cast<std::uint64_t>(像素数) * 255;
    return static_cast<std::int64_t>(10000
        - (总差 * 10000 + 最大总差 / 2) / 最大总差);
}

std::optional<std::int64_t> 比较二维轮廓材料(
    const std::vector<std::uint64_t>& 左材料,
    const std::vector<std::uint64_t>& 右材料) {
    const auto 左边长 = 推导二维边长(左材料);
    const auto 右边长 = 推导二维边长(右材料);
    if (!左边长 || !右边长) return std::nullopt;
    const auto 左层级 = 生成二维比较层级(左材料, *左边长);
    const auto 右层级 = 生成二维比较层级(右材料, *右边长);
    if (!左层级 || !右层级) return std::nullopt;

    const auto 共同层数 = (std::min)(左层级->size(), 右层级->size());
    std::int64_t 相似度 = 10000;
    std::size_t 当前边长 = 8;
    for (std::size_t 层号 = 0; 层号 < 共同层数; ++层号) {
        const auto 当前 = 计算灰度层相似度(
            (*左层级)[层号], (*右层级)[层号], 当前边长 * 当前边长);
        if (!当前) return std::nullopt;
        相似度 = *当前;
        if (相似度 < 5000) return 0;
        当前边长 *= 2;
    }
    return 相似度;
}

std::optional<std::array<std::vector<std::uint64_t>, 3>> 生成体素正交投影(
    const std::vector<std::uint64_t>& 体素,
    std::size_t 边长) {
    if (边长 == 0 || 边长 > (std::numeric_limits<std::size_t>::max)() / 边长) {
        return std::nullopt;
    }
    const auto 投影位数 = 边长 * 边长;
    std::array<std::vector<std::uint64_t>, 3> 投影{
        std::vector<std::uint64_t>((投影位数 + 63) / 64, 0),
        std::vector<std::uint64_t>((投影位数 + 63) / 64, 0),
        std::vector<std::uint64_t>((投影位数 + 63) / 64, 0)};
    for (std::size_t z = 0; z < 边长; ++z) {
        for (std::size_t y = 0; y < 边长; ++y) {
            for (std::size_t x = 0; x < 边长; ++x) {
                const auto 位置 = (z * 边长 + y) * 边长 + x;
                if (!读取二值位(体素, 位置)) continue;
                写入二值位(投影[0], y * 边长 + x); // XY，沿Z投影
                写入二值位(投影[1], z * 边长 + x); // XZ，沿Y投影
                写入二值位(投影[2], z * 边长 + y); // YZ，沿X投影
            }
        }
    }
    return 投影;
}

std::optional<std::int64_t> 比较三维体素投影(
    const std::vector<std::uint64_t>& 左体素,
    const std::vector<std::uint64_t>& 右体素) {
    const auto 左边长 = 推导三维边长(左体素);
    const auto 右边长 = 推导三维边长(右体素);
    if (!左边长 || !右边长) return std::nullopt;
    const auto 左投影 = 生成体素正交投影(左体素, *左边长);
    const auto 右投影 = 生成体素正交投影(右体素, *右边长);
    if (!左投影 || !右投影) return std::nullopt;
    std::uint64_t 总分 = 0;
    for (std::size_t i = 0; i < 3; ++i) {
        auto 左投影边长 = *左边长;
        auto 右投影边长 = *右边长;
        auto 左轮廓 = (*左投影)[i];
        auto 右轮廓 = (*右投影)[i];
        while (左投影边长 < 8) {
            std::vector<std::uint64_t> 放大后(1, 0);
            for (std::size_t y = 0; y < 左投影边长; ++y) {
                for (std::size_t x = 0; x < 左投影边长; ++x) {
                    if (读取二值位(左轮廓, y * 左投影边长 + x)) {
                        for (std::size_t dy = 0; dy < 2; ++dy) {
                            for (std::size_t dx = 0; dx < 2; ++dx) {
                                写入二值位(放大后,
                                    (y * 2 + dy) * (左投影边长 * 2) + x * 2 + dx);
                            }
                        }
                    }
                }
            }
            左轮廓 = std::move(放大后);
            左投影边长 *= 2;
        }
        while (右投影边长 < 8) {
            std::vector<std::uint64_t> 放大后(1, 0);
            for (std::size_t y = 0; y < 右投影边长; ++y) {
                for (std::size_t x = 0; x < 右投影边长; ++x) {
                    if (读取二值位(右轮廓, y * 右投影边长 + x)) {
                        for (std::size_t dy = 0; dy < 2; ++dy) {
                            for (std::size_t dx = 0; dx < 2; ++dx) {
                                写入二值位(放大后,
                                    (y * 2 + dy) * (右投影边长 * 2) + x * 2 + dx);
                            }
                        }
                    }
                }
            }
            右轮廓 = std::move(放大后);
            右投影边长 *= 2;
        }
        const auto 分数 = 比较二维轮廓材料(左轮廓, 右轮廓);
        if (!分数) return std::nullopt;
        总分 += static_cast<std::uint64_t>(*分数);
    }
    return static_cast<std::int64_t>(总分 / 3);
}

std::uint64_t I64有序编码(std::int64_t 值) noexcept {
    return std::bit_cast<std::uint64_t>(值) ^ (std::uint64_t{1} << 63);
}

std::int64_t I64有序编码还原(std::uint64_t 值) noexcept {
    return std::bit_cast<std::int64_t>(值 ^ (std::uint64_t{1} << 63));
}

long double I64区间测度(const 特征概念I64闭区间& 区间) noexcept {
    const auto 下界 = I64有序编码(区间.下界);
    const auto 上界 = I64有序编码(区间.上界);
    const auto 差值 = 上界 - 下界;
    return 差值 == (std::numeric_limits<std::uint64_t>::max)()
        ? 18446744073709551616.0L
        : static_cast<long double>(差值 + 1);
}

long double I64区间集合测度(
    const std::vector<特征概念I64闭区间>& 值域) noexcept {
    long double 总量 = 0;
    for (const auto& 区间 : 值域) {
        总量 += I64区间测度(区间);
    }
    return 总量;
}

long double I64区间集合交集测度(
    const std::vector<特征概念I64闭区间>& 左值域,
    const std::vector<特征概念I64闭区间>& 右值域) noexcept {
    long double 总量 = 0;
    std::size_t 左位置 = 0;
    std::size_t 右位置 = 0;
    while (左位置 < 左值域.size() && 右位置 < 右值域.size()) {
        const auto 下界 = (std::max)(
            左值域[左位置].下界, 右值域[右位置].下界);
        const auto 上界 = (std::min)(
            左值域[左位置].上界, 右值域[右位置].上界);
        if (下界 <= 上界) {
            总量 += I64区间测度({下界, 上界});
        }
        if (左值域[左位置].上界 < 右值域[右位置].上界) {
            ++左位置;
        } else {
            ++右位置;
        }
    }
    return 总量;
}

std::optional<std::int64_t> 测度转重叠度(
    long double 左测度,
    long double 右测度,
    long double 交集测度) noexcept {
    const auto 并集测度 = 左测度 + 右测度 - 交集测度;
    if (!(并集测度 > 0.0L) || 交集测度 < 0.0L) return std::nullopt;
    const auto 比例 = (std::max)(0.0L,
        (std::min)(1.0L, 交集测度 / 并集测度));
    return static_cast<std::int64_t>(比例 * 10000.0L + 0.5L);
}

std::optional<std::int64_t> 计算值域重叠度(
    const 特征概念值域& 左值域,
    const 特征概念值域& 右值域) noexcept {
    if (左值域 == 右值域) return 10000;
    if (std::holds_alternative<特征概念全材料值域>(左值域)
        || std::holds_alternative<特征概念全材料值域>(右值域)) {
        return std::nullopt;
    }
    if (const auto* 左I64 =
        std::get_if<std::vector<特征概念I64闭区间>>(&左值域)) {
        const auto* 右I64 =
            std::get_if<std::vector<特征概念I64闭区间>>(&右值域);
        if (!右I64) return std::nullopt;
        return 测度转重叠度(
            I64区间集合测度(*左I64),
            I64区间集合测度(*右I64),
            I64区间集合交集测度(*左I64, *右I64));
    }
    if (const auto* 左节点 =
        std::get_if<std::vector<稳定编码>>(&左值域)) {
        const auto* 右节点 = std::get_if<std::vector<稳定编码>>(&右值域);
        if (!右节点 || 左节点->empty() || 右节点->empty()) return std::nullopt;
        std::size_t 交集数量 = 0;
        std::size_t 左位置 = 0;
        std::size_t 右位置 = 0;
        while (左位置 < 左节点->size() && 右位置 < 右节点->size()) {
            if ((*左节点)[左位置] == (*右节点)[右位置]) {
                ++交集数量;
                ++左位置;
                ++右位置;
            } else if ((*左节点)[左位置].值 < (*右节点)[右位置].值) {
                ++左位置;
            } else {
                ++右位置;
            }
        }
        return 测度转重叠度(
            static_cast<long double>(左节点->size()),
            static_cast<long double>(右节点->size()),
            static_cast<long double>(交集数量));
    }

    const auto* 左结构 = std::get_if<特征概念结构化I64值域>(&左值域);
    const auto* 右结构 = std::get_if<特征概念结构化I64值域>(&右值域);
    if (!左结构 || !右结构
        || 左结构->分量.size() != 右结构->分量.size()
        || 左结构->分量.empty()) {
        return std::nullopt;
    }
    long double 交占左比例 = 1.0L;
    long double 交占右比例 = 1.0L;
    for (std::size_t i = 0; i < 左结构->分量.size(); ++i) {
        const auto& 左分量 = 左结构->分量[i];
        const auto& 右分量 = 右结构->分量[i];
        if (左分量.角色 != 右分量.角色 || 左分量.单位 != 右分量.单位) {
            return std::nullopt;
        }
        const auto 左测度 = I64区间集合测度(左分量.值域);
        const auto 右测度 = I64区间集合测度(右分量.值域);
        const auto 交集测度 = I64区间集合交集测度(
            左分量.值域, 右分量.值域);
        if (!(左测度 > 0.0L) || !(右测度 > 0.0L)) return std::nullopt;
        if (!(交集测度 > 0.0L)) return 0;
        交占左比例 *= 交集测度 / 左测度;
        交占右比例 *= 交集测度 / 右测度;
    }
    if (!(交占左比例 > 0.0L) || !(交占右比例 > 0.0L)) return 0;
    const auto 分母 = 1.0L / 交占左比例 + 1.0L / 交占右比例 - 1.0L;
    if (!(分母 >= 1.0L)) return std::nullopt;
    const auto 比例 = (std::max)(0.0L, (std::min)(1.0L, 1.0L / 分母));
    return static_cast<std::int64_t>(比例 * 10000.0L + 0.5L);
}

std::optional<std::int64_t> 取得I64值域代表值(
    const std::vector<特征概念I64闭区间>& 值域) noexcept {
    std::uint64_t 总量 = 0;
    bool 全I64域数量 = false;
    for (const auto& 区间 : 值域) {
        const auto 差值 = I64有序编码(区间.上界) - I64有序编码(区间.下界);
        if (差值 == (std::numeric_limits<std::uint64_t>::max)()) {
            全I64域数量 = true;
            break;
        }
        const auto 长度 = 差值 + 1;
        if (总量 > (std::numeric_limits<std::uint64_t>::max)() - 长度) {
            全I64域数量 = true;
            break;
        }
        总量 += 长度;
    }
    if (!全I64域数量 && 总量 == 0) return std::nullopt;
    std::uint64_t 位置 = 全I64域数量
        ? ((std::numeric_limits<std::uint64_t>::max)() >> 1)
        : (总量 - 1) / 2;
    for (const auto& 区间 : 值域) {
        const auto 下界 = I64有序编码(区间.下界);
        const auto 差值 = I64有序编码(区间.上界) - 下界;
        if (差值 == (std::numeric_limits<std::uint64_t>::max)()
            || 位置 <= 差值) {
            return I64有序编码还原(下界 + 位置);
        }
        位置 -= 差值 + 1;
    }
    return std::nullopt;
}

新特征比较状态 映射概念规则状态(特征概念规则状态 状态) noexcept {
    switch (状态) {
    case 特征概念规则状态::已完成:
    case 特征概念规则状态::无变化:
        return 新特征比较状态::已完成;
    case 特征概念规则状态::入口拒绝:
        return 新特征比较状态::入口拒绝;
    case 特征概念规则状态::材料类型不相容:
        return 新特征比较状态::材料类型不相容;
    case 特征概念规则状态::值不存在:
        return 新特征比较状态::值不存在;
    case 特征概念规则状态::规则不支持:
        return 新特征比较状态::规则不支持;
    case 特征概念规则状态::资源失败:
        return 新特征比较状态::资源失败;
    case 特征概念规则状态::内部不一致:
        return 新特征比较状态::结构不一致;
    }
    return 新特征比较状态::结构不一致;
}

struct 同类型特征概念上下文 final {
    特征概念信息 左;
    特征概念信息 右;
    稳定编码 特征类型根;
};

新特征比较状态 读取同类型特征概念(
    稳定编码 左概念节点,
    稳定编码 右概念节点,
    概念_特征类& 特征概念服务,
    std::optional<同类型特征概念上下文>& 上下文) noexcept {
    if (!有效(左概念节点) || !有效(右概念节点)) {
        return 新特征比较状态::入口拒绝;
    }
    const auto 左 = 特征概念服务.获取特征概念(左概念节点);
    const auto 右 = 特征概念服务.获取特征概念(右概念节点);
    if (!左 || !右) return 新特征比较状态::概念不存在;
    const auto 定位 = 特征概念服务.比较单特征概念(
        左概念节点, 右概念节点);
    if (定位.状态 != 特征概念规则状态::已完成 || !定位.关系) {
        return 映射概念规则状态(定位.状态);
    }
    if (*定位.关系 == 单特征概念关系::无可比关系
        || !定位.特征类型根) {
        return 新特征比较状态::概念不相容;
    }
    if (左->材料类型 != 右->材料类型) {
        return 新特征比较状态::材料类型不相容;
    }
    if (左->单位 != 右->单位) return 新特征比较状态::单位不相容;
    if (左->比较规则 != 右->比较规则) {
        return 新特征比较状态::结构不一致;
    }
    上下文 = 同类型特征概念上下文{*左, *右, *定位.特征类型根};
    return 新特征比较状态::已完成;
}

新特征比较结果 比较双准确值(
    稳定编码 比较概念节点,
    稳定编码 特征类型根,
    const 新特征准确值& 左值,
    const 新特征准确值& 右值,
    新_特征值类& 特征值服务,
    概念_特征类& 特征概念服务) noexcept {
    新特征比较结果 结果;
    结果.材料 = 新特征比较材料::双准确值;
    const auto 概念 = 特征概念服务.获取特征概念(比较概念节点);
    const auto 根概念 = 特征概念服务.获取特征概念(特征类型根);
    if (!概念 || !根概念) {
        结果.状态 = 新特征比较状态::概念不存在;
        return 结果;
    }
    if (概念->材料类型 != 根概念->材料类型
        || 概念->单位 != 根概念->单位) {
        结果.状态 = 新特征比较状态::结构不一致;
        return 结果;
    }
    if (!准确值材料类型相容(
        概念->材料类型, 左值, 特征值服务)
        || !准确值材料类型相容(
            概念->材料类型, 右值, 特征值服务)) {
        结果.状态 = 新特征比较状态::材料类型不相容;
        return 结果;
    }

    if (概念->材料类型 == 特征概念材料物理类型::I64标量) {
        const auto* 左I64 = std::get_if<std::int64_t>(&左值);
        const auto* 右I64 = std::get_if<std::int64_t>(&右值);
        if (!左I64 || !右I64) {
            结果.状态 = 新特征比较状态::材料类型不相容;
            return 结果;
        }
        结果.数值类型 = 新特征比较数值类型::有向差值;
        结果.比较值 = 计算饱和有向差值(*左I64, *右I64);
        结果.状态 = 新特征比较状态::已完成;
        return 结果;
    }

    const auto* 左节点 = std::get_if<稳定编码>(&左值);
    const auto* 右节点 = std::get_if<稳定编码>(&右值);
    if (!左节点 || !右节点) {
        结果.状态 = 新特征比较状态::材料类型不相容;
        return 结果;
    }
    const auto 左信息 = 特征值服务.获取特征值(*左节点);
    const auto 右信息 = 特征值服务.获取特征值(*右节点);
    if (!左信息 || !右信息) {
        结果.状态 = 新特征比较状态::值不存在;
        return 结果;
    }

    if (概念->材料类型 == 特征概念材料物理类型::I64数组) {
        const auto 左抽象 = 特征概念服务.查找值对应的最具体概念(
            特征类型根, 左值, 根概念->单位, 特征值服务);
        const auto 右抽象 = 特征概念服务.查找值对应的最具体概念(
            特征类型根, 右值, 根概念->单位, 特征值服务);
        if (左抽象.状态 == 特征概念查找状态::已找到
            && 右抽象.状态 == 特征概念查找状态::已找到
            && 左抽象.概念节点 && 右抽象.概念节点) {
            const auto 抽象关系 = 特征概念服务.比较单特征概念(
                *左抽象.概念节点, *右抽象.概念节点);
            if (抽象关系.状态 != 特征概念规则状态::已完成
                || !抽象关系.关系) {
                结果.状态 = 映射概念规则状态(抽象关系.状态);
                return 结果;
            }
            结果.抽象概念关系 = *抽象关系.关系;
            结果.抽象共同上位 = 抽象关系.最近共同上位;
        } else if ((左抽象.状态 != 特征概念查找状态::未找到
                && 左抽象.状态 != 特征概念查找状态::匹配冲突)
            || (右抽象.状态 != 特征概念查找状态::未找到
                && 右抽象.状态 != 特征概念查找状态::匹配冲突)) {
            结果.状态 = 新特征比较状态::结构不一致;
            return 结果;
        }
    }

    结果.数值类型 = 新特征比较数值类型::相似度;
    if (左信息->材料 == 右信息->材料) {
        结果.比较值 = 10000;
        结果.状态 = 新特征比较状态::已完成;
        return 结果;
    }
    if (概念->材料类型 == 特征概念材料物理类型::I64数组) {
        const auto* 结构化 =
            std::get_if<特征概念结构化I64值域>(&概念->值域);
        const auto* 左数组 =
            std::get_if<std::vector<std::int64_t>>(&左信息->材料);
        const auto* 右数组 =
            std::get_if<std::vector<std::int64_t>>(&右信息->材料);
        if (!结构化 || !左数组 || !右数组
            || 左数组->size() != 结构化->分量.size()
            || 右数组->size() != 结构化->分量.size()) {
            结果.比较值 = -1;
            结果.状态 = 新特征比较状态::已完成;
            return 结果;
        }
        const auto 先天 = 特征概念服务.获取先天特征概念();
        结果.比较值 = 先天 && 特征类型根 == 先天->RGB颜色
            ? 计算RGB相似度(*左数组, *右数组).value_or(-1)
            : 计算多维相似度(*左数组, *右数组).value_or(-1);
        结果.状态 = 新特征比较状态::已完成;
        return 结果;
    }
    if (概念->材料类型 == 特征概念材料物理类型::二维二值格) {
        const auto* 左轮廓 =
            std::get_if<std::vector<std::uint64_t>>(&左信息->材料);
        const auto* 右轮廓 =
            std::get_if<std::vector<std::uint64_t>>(&右信息->材料);
        结果.比较值 = 左轮廓 && 右轮廓
            ? 比较二维轮廓材料(*左轮廓, *右轮廓).value_or(-1) : -1;
        结果.状态 = 新特征比较状态::已完成;
        return 结果;
    }
    if (概念->材料类型 == 特征概念材料物理类型::三维二值格) {
        const auto* 左体素 =
            std::get_if<std::vector<std::uint64_t>>(&左信息->材料);
        const auto* 右体素 =
            std::get_if<std::vector<std::uint64_t>>(&右信息->材料);
        结果.比较值 = 左体素 && 右体素
            ? 比较三维体素投影(*左体素, *右体素).value_or(-1) : -1;
        结果.状态 = 新特征比较状态::已完成;
        return 结果;
    }
    结果.比较值 = -1;
    结果.状态 = 新特征比较状态::已完成;
    return 结果;
}

std::optional<std::int64_t> 计算节点材料接近度(
    稳定编码 特征类型根,
    特征概念材料物理类型 材料类型,
    稳定编码 左值节点,
    稳定编码 右值节点,
    新_特征值类& 特征值服务,
    概念_特征类& 特征概念服务) noexcept {
    const auto 期望类型 = 转换特征值材料类型(材料类型);
    const auto 左信息 = 特征值服务.获取特征值(左值节点);
    const auto 右信息 = 特征值服务.获取特征值(右值节点);
    if (!期望类型 || !左信息 || !右信息
        || 左信息->物理类型 != *期望类型
        || 右信息->物理类型 != *期望类型) {
        return std::nullopt;
    }
    if (左信息->材料 == 右信息->材料) return 10000;

    if (材料类型 == 特征概念材料物理类型::I64数组) {
        const auto* 左数组 =
            std::get_if<std::vector<std::int64_t>>(&左信息->材料);
        const auto* 右数组 =
            std::get_if<std::vector<std::int64_t>>(&右信息->材料);
        if (!左数组 || !右数组 || 左数组->size() != 右数组->size()) {
            return std::nullopt;
        }
        const auto 先天 = 特征概念服务.获取先天特征概念();
        return 先天 && 特征类型根 == 先天->RGB颜色
            ? 计算RGB相似度(*左数组, *右数组)
            : 计算多维相似度(*左数组, *右数组);
    }
    if (材料类型 == 特征概念材料物理类型::二维二值格) {
        const auto* 左轮廓 =
            std::get_if<std::vector<std::uint64_t>>(&左信息->材料);
        const auto* 右轮廓 =
            std::get_if<std::vector<std::uint64_t>>(&右信息->材料);
        return 左轮廓 && 右轮廓
            ? 比较二维轮廓材料(*左轮廓, *右轮廓) : std::nullopt;
    }
    if (材料类型 == 特征概念材料物理类型::三维二值格) {
        const auto* 左体素 =
            std::get_if<std::vector<std::uint64_t>>(&左信息->材料);
        const auto* 右体素 =
            std::get_if<std::vector<std::uint64_t>>(&右信息->材料);
        return 左体素 && 右体素
            ? 比较三维体素投影(*左体素, *右体素) : std::nullopt;
    }
    return std::nullopt;
}

void 回滚字段和节点(
    稳定编码 节点, const std::vector<稳定编码>& 字段关系组) noexcept {
    for (auto 位置 = 字段关系组.rbegin(); 位置 != 字段关系组.rend(); ++位置) {
        全局基础数据集.删除字段(*位置);
    }
    全局基础数据集.删除节点(节点);
}

} // namespace

新_特征类::新_特征类(
    新_特征值类& 特征值服务,
    概念_特征类& 特征概念服务) noexcept
    : 特征值服务_(特征值服务), 特征概念服务_(特征概念服务) {}

bool 新_特征类::初始化() noexcept {
    try {
        std::lock_guard 锁(新特征互斥);
        if (!特征值服务_.初始化()
            || !特征概念服务_.初始化(特征值服务_)) return false;
        if (结构已初始化()) return 结构仍存在();

        auto 建立 = [](稳定编码& 节点) {
            if (有效(节点)) return 节点仍存在(节点);
            节点 = 全局基础数据集.新建节点();
            return 有效(节点);
        };
        return 建立(节点类型字段)
            && 建立(特征节点类型)
            && 建立(特征概念字段)
            && 建立(值域节点类型)
            && 建立(实例历史聚合值域字段)
            && 建立(I64值域字段)
            && 建立(全材料值域字段)
            && 建立(非I64值域字段)
            && 建立(结构化分量字段)
            && 建立(结构化分量节点类型)
            && 建立(分量顺序字段)
            && 建立(分量角色字段)
            && 建立(分量单位字段)
            && 建立(分量I64值域字段)
            && 建立(实例单位字段)
            && 建立(当前值字段)
            && 建立(历史值字段)
            && 建立(标准值字段)
            && 建立(历史值命中记录字段)
            && 建立(历史值命中记录节点类型)
            && 建立(历史值记录准确值字段)
            && 建立(历史值记录命中次数字段)
            && 建立(历史值记录所属特征字段)
            && 建立(归组子特征标记字段);
    } catch (...) {
        return false;
    }
}

稳定编码 新_特征类::建立或取得特征(
    稳定编码 持有节点,
    稳定编码 特征概念节点,
    std::optional<新特征准确值> 实际值) noexcept {
    try {
        std::lock_guard 锁(新特征互斥);
        if (!结构仍存在()
            || !节点仍存在(持有节点)
            || !特征概念服务_.是特征概念节点(特征概念节点)) {
            return {};
        }
        const auto 概念信息 = 特征概念服务_.获取特征概念(特征概念节点);
        if (!概念信息) return {};

        const auto 查找已有实例 = [&](稳定编码 概念节点)
            -> std::optional<稳定编码> {
            std::vector<稳定编码> 已有;
            for (const auto& 关系 : 全局基础数据集.查询源关系(
                持有节点, 基础外部关系类型::父子)) {
                const auto 类型 = 读取唯一节点字段(
                    关系.目标节点, 节点类型字段);
                const auto 概念 = 读取唯一节点字段(
                    关系.目标节点, 特征概念字段);
                if (类型 && *类型 == 特征节点类型
                    && 概念 && *概念 == 概念节点) {
                    已有.push_back(关系.目标节点);
                }
            }
            std::sort(已有.begin(), 已有.end());
            已有.erase(std::unique(已有.begin(), 已有.end()), 已有.end());
            if (已有.size() > 1) return 稳定编码{};
            if (已有.empty()) return std::nullopt;
            return 获取特征(已有.front())
                ? std::optional<稳定编码>{已有.front()}
                : std::optional<稳定编码>{稳定编码{}};
        };

        if (const auto 已有 = 查找已有实例(特征概念节点)) {
            return *已有;
        }

        auto 模板概念节点 = 特征概念节点;
        auto 模板概念信息 = 概念信息;
        if (实际值) {
            if (!准确值材料类型相容(
                    概念信息->材料类型, *实际值, 特征值服务_)) {
                return {};
            }
            const auto 类型定位 = 特征概念服务_.比较单特征概念(
                特征概念节点, 特征概念节点);
            if (类型定位.状态 != 特征概念规则状态::已完成
                || !类型定位.特征类型根) {
                return {};
            }
            const auto 查找结果 = 特征概念服务_.查找值对应的最具体概念(
                *类型定位.特征类型根, *实际值,
                概念信息->单位, 特征值服务_);
            if (查找结果.状态 == 特征概念查找状态::已找到
                && 查找结果.概念节点) {
                模板概念节点 = *查找结果.概念节点;
            } else if (查找结果.状态 == 特征概念查找状态::未找到
                || 查找结果.状态 == 特征概念查找状态::匹配冲突) {
                模板概念节点 = *类型定位.特征类型根;
            } else {
                return {};
            }
            模板概念信息 = 特征概念服务_.获取特征概念(模板概念节点);
            if (!模板概念信息) return {};
            if (const auto 已有 = 查找已有实例(模板概念节点)) {
                return *已有;
            }
        }

        const auto 节点 = 全局基础数据集.新建节点();
        if (!有效(节点)) return {};
        std::vector<稳定编码> 已写字段;
        const auto 类型关系 = 全局基础数据集.添加字段节点(
            节点, 节点类型字段, 特征节点类型);
        if (有效(类型关系)) 已写字段.push_back(类型关系);
        const auto 概念关系 = 有效(类型关系)
            ? 全局基础数据集.添加字段节点(
                节点, 特征概念字段, 模板概念节点)
            : 稳定编码{};
        if (有效(概念关系)) 已写字段.push_back(概念关系);
        bool 完成 = 有效(类型关系) && 有效(概念关系);
        if (完成 && 模板概念信息->单位) {
            const auto 单位关系 = 全局基础数据集.添加字段节点(
                节点, 实例单位字段, *模板概念信息->单位);
            if (有效(单位关系)) 已写字段.push_back(单位关系);
            完成 = 有效(单位关系);
        }
        if (!完成) {
            回滚字段和节点(节点, 已写字段);
            return {};
        }
        const auto 持有关系 = 全局基础数据集.添加关系(
            持有节点, 节点, 基础外部关系类型::父子);
        if (!有效(持有关系)) {
            回滚字段和节点(节点, 已写字段);
            return {};
        }
        if (!特征概念服务_.增加当前实例引用(模板概念节点)) {
            (void)全局基础数据集.删除关系(持有关系);
            回滚字段和节点(节点, 已写字段);
            return {};
        }
        return 节点;
    } catch (...) {
        return {};
    }
}

稳定编码 新_特征类::建立或取得子特征节点(
    稳定编码 父特征节点,
    const 特征概念值域& 子值域) noexcept {
    try {
        std::lock_guard 锁(新特征互斥);
        const auto 父特征 = 获取特征(父特征节点);
        if (!父特征) return {};

        const auto 父概念 = 特征概念服务_.获取特征概念(
            父特征->特征概念节点);
        if (!父概念) return {};

        特征概念定义 子概念定义;
        子概念定义.材料类型 = 父概念->材料类型;
        子概念定义.值域 = 子值域;
        子概念定义.单位 = 父概念->单位;
        子概念定义.比较规则 = 父概念->比较规则;
        子概念定义.聚合规则 = 父概念->聚合规则;
        稳定编码 上位概念 = 父特征->特征概念节点;
        稳定编码 子概念节点{};
        const auto 父域关系 = 特征概念服务_.比较值域(
            父概念->材料类型, 父概念->值域, 子值域);
        if (父域关系.状态 == 特征概念规则状态::已完成
            && 父域关系.关系 == 特征概念值域关系::相等) {
            子概念节点 = 父特征->特征概念节点;
        } else {
            const bool 父可包含 =
                父域关系.状态 == 特征概念规则状态::已完成
                && 父域关系.关系 == 特征概念值域关系::左包含右;
            if (!父可包含) {
                const auto 类型定位 = 特征概念服务_.比较单特征概念(
                    父特征->特征概念节点, 父特征->特征概念节点);
                if (类型定位.状态 != 特征概念规则状态::已完成
                    || !类型定位.特征类型根) {
                    return {};
                }
                上位概念 = *类型定位.特征类型根;
            }
            子概念节点 = 特征概念服务_.建立或取得特征概念(
                子概念定义, 特征值服务_, 上位概念);
        }
        if (!有效(子概念节点)
            || !特征概念服务_.获取特征概念(子概念节点)) {
            return {};
        }
        const auto 概念父关系 = 全局基础数据集.查询目标关系(
            子概念节点, 基础外部关系类型::父子);
        if (概念父关系.size() != 1
            || (子概念节点 != 父特征->特征概念节点
                && 概念父关系.front().源节点 != 上位概念)) {
            return {};
        }
        return 建立或取得特征(父特征节点, 子概念节点);
    } catch (...) {
        return {};
    }
}

std::optional<稳定编码> 新_特征类::查询特征(
    稳定编码 持有节点,
    稳定编码 特征概念节点) const noexcept {
    try {
        std::lock_guard 锁(新特征互斥);
        if (!结构仍存在() || !节点仍存在(持有节点)
            || !特征概念服务_.是特征概念节点(特征概念节点)) {
            return std::nullopt;
        }
        std::vector<稳定编码> 结果;
        for (const auto& 关系 : 全局基础数据集.查询源关系(
            持有节点, 基础外部关系类型::父子)) {
            const auto 类型 = 读取唯一节点字段(关系.目标节点, 节点类型字段);
            const auto 概念 = 读取唯一节点字段(关系.目标节点, 特征概念字段);
            if (类型 && *类型 == 特征节点类型
                && 概念 && *概念 == 特征概念节点) {
                结果.push_back(关系.目标节点);
            }
        }
        std::sort(结果.begin(), 结果.end());
        结果.erase(std::unique(结果.begin(), 结果.end()), 结果.end());
        return 结果.size() == 1
            ? std::optional<稳定编码>{结果.front()} : std::nullopt;
    } catch (...) {
        return std::nullopt;
    }
}

std::optional<新特征信息> 新_特征类::获取特征(
    稳定编码 特征节点) const noexcept {
    try {
        std::lock_guard 锁(新特征互斥);
        if (!结构仍存在() || !节点仍存在(特征节点)) return std::nullopt;
        const auto 类型 = 读取唯一节点字段(特征节点, 节点类型字段);
        const auto 概念 = 读取唯一节点字段(特征节点, 特征概念字段);
        if (!类型 || *类型 != 特征节点类型 || !概念
            || !特征概念服务_.是特征概念节点(*概念)) {
            return std::nullopt;
        }
        const auto 概念信息 = 特征概念服务_.获取特征概念(*概念);
        if (!概念信息) return std::nullopt;

        const auto 父关系 = 全局基础数据集.查询目标关系(
            特征节点, 基础外部关系类型::父子);
        if (父关系.size() != 1) return std::nullopt;

        新特征信息 结果;
        结果.节点 = 特征节点;
        结果.持有节点 = 父关系.front().源节点;
        结果.特征概念节点 = *概念;

        const auto 聚合引用组 = 全局基础数据集.查询字段(
            特征节点, 实例历史聚合值域字段);
        if (聚合引用组.size() > 1) return std::nullopt;
        if (!聚合引用组.empty()) {
            const auto* 聚合值域节点 =
                std::get_if<稳定编码>(&聚合引用组.front().内容);
            if (!聚合值域节点) return std::nullopt;
            结果.实例历史聚合值域 = 读取值域节点(
                *聚合值域节点, 概念信息->材料类型);
            if (!结果.实例历史聚合值域) return std::nullopt;
        }

        const auto 单位字段组 = 全局基础数据集.查询字段(
            特征节点, 实例单位字段);
        if (单位字段组.size() > 1) return std::nullopt;
        if (!单位字段组.empty()) {
            const auto* 单位节点 = std::get_if<稳定编码>(&单位字段组.front().内容);
            if (!单位节点 || !节点仍存在(*单位节点)) {
                return std::nullopt;
            }
            结果.实例单位 = *单位节点;
        }

        const auto 当前字段组 = 全局基础数据集.查询字段(特征节点, 当前值字段);
        if (当前字段组.size() > 1) return std::nullopt;
        if (!当前字段组.empty()) {
            结果.当前值 = 解码准确值(当前字段组.front());
            if (!结果.当前值) return std::nullopt;
        }

        const auto 标准字段组 = 全局基础数据集.查询字段(特征节点, 标准值字段);
        if (标准字段组.size() > 1) return std::nullopt;
        if (!标准字段组.empty()) {
            结果.标准值 = 解码准确值(标准字段组.front());
            if (!结果.标准值) return std::nullopt;
        }

        const auto 命中记录组 = 读取历史值命中记录(特征节点);
        if (!命中记录组) return std::nullopt;
        for (const auto& 记录 : *命中记录组) {
            if (!准确值材料类型相容(
                概念信息->材料类型, 记录.值, 特征值服务_)) {
                return std::nullopt;
            }
            结果.历史不同值.push_back(记录.值);
            结果.历史值命中信息.push_back({记录.值, 记录.命中次数});
        }

        for (const auto& 字段 : 全局基础数据集.查询字段(特征节点, 历史值字段)) {
            const auto 值 = 解码准确值(字段);
            if (!值 || !准确值材料类型相容(
                概念信息->材料类型, *值, 特征值服务_)) {
                return std::nullopt;
            }
            结果.历史不同值.push_back(*值);
            结果.历史值命中信息.push_back({*值, 1});
        }
        std::sort(结果.历史不同值.begin(), 结果.历史不同值.end(),
            [](const 新特征准确值& 左, const 新特征准确值& 右) {
                if (左.index() != 右.index()) return 左.index() < 右.index();
                if (const auto* 左I64 = std::get_if<std::int64_t>(&左)) {
                    return *左I64 < std::get<std::int64_t>(右);
                }
                return std::get<稳定编码>(左) < std::get<稳定编码>(右);
            });
        std::sort(结果.历史值命中信息.begin(), 结果.历史值命中信息.end(),
            [](const 新特征值命中信息& 左, const 新特征值命中信息& 右) {
                if (左.值.index() != 右.值.index()) return 左.值.index() < 右.值.index();
                if (const auto* 左I64 = std::get_if<std::int64_t>(&左.值)) {
                    return *左I64 < std::get<std::int64_t>(右.值);
                }
                return std::get<稳定编码>(左.值) < std::get<稳定编码>(右.值);
            });
        if (结果.当前值
            && !准确值材料类型相容(
                概念信息->材料类型, *结果.当前值, 特征值服务_)) {
            return std::nullopt;
        }

        auto 值相等 = [&](const 新特征准确值& 左, const 新特征准确值& 右)
            -> std::optional<bool> {
            const auto 比较 = 特征概念服务_.执行比较规则(
                概念信息->材料类型, 概念信息->比较规则,
                左, 右, 特征值服务_);
            if (比较.状态 != 特征概念规则状态::已完成 || !比较.相等) {
                return std::nullopt;
            }
            return *比较.相等;
        };
        for (std::size_t 左 = 0; 左 < 结果.历史不同值.size(); ++左) {
            for (std::size_t 右 = 左 + 1; 右 < 结果.历史不同值.size(); ++右) {
                const auto 相等 = 值相等(
                    结果.历史不同值[左], 结果.历史不同值[右]);
                if (!相等 || *相等) return std::nullopt;
            }
        }
        if (结果.历史不同值.size() != 结果.历史值命中信息.size()) {
            return std::nullopt;
        }
        if (结果.当前值) {
            bool 在历史中 = false;
            for (const auto& 历史值 : 结果.历史不同值) {
                const auto 相等 = 值相等(*结果.当前值, 历史值);
                if (!相等) return std::nullopt;
                if (*相等) {
                    在历史中 = true;
                    break;
                }
            }
            if (!在历史中) return std::nullopt;
        }
        if (结果.标准值) {
            std::optional<std::int64_t> 标准命中次数;
            std::int64_t 最高命中次数 = 0;
            for (const auto& 历史值 : 结果.历史值命中信息) {
                if (历史值.命中次数 <= 0) return std::nullopt;
                最高命中次数 = (std::max)(最高命中次数, 历史值.命中次数);
                const auto 相等 = 值相等(*结果.标准值, 历史值.值);
                if (!相等) return std::nullopt;
                if (*相等) 标准命中次数 = 历史值.命中次数;
            }
            if (!标准命中次数 || *标准命中次数 != 最高命中次数) {
                return std::nullopt;
            }
        } else if (!结果.历史不同值.empty()) {
            return std::nullopt;
        }
        if (结果.历史不同值.empty()) {
            结果.实例历史聚合值域.reset();
        } else {
            const auto 应有聚合 = 特征概念服务_.执行聚合规则(
                概念信息->材料类型, 概念信息->聚合规则,
                结果.历史不同值, 特征值服务_);
            if ((应有聚合.状态 != 特征概念规则状态::已完成
                    && 应有聚合.状态 != 特征概念规则状态::无变化)
                || !应有聚合.值域
                || !结果.实例历史聚合值域
                || *结果.实例历史聚合值域 != *应有聚合.值域) {
                // 聚合域缺失或过期不使准确值失效；写入口可据此重建。
                结果.实例历史聚合值域.reset();
            }
        }
        return 结果;
    } catch (...) {
        return std::nullopt;
    }
}

bool 新_特征类::是特征节点(稳定编码 节点) const noexcept {
    try {
        std::lock_guard 锁(新特征互斥);
        if (!结构仍存在() || !节点仍存在(节点)) return false;
        const auto 类型 = 读取唯一节点字段(节点, 节点类型字段);
        return 类型 && *类型 == 特征节点类型;
    } catch (...) {
        return false;
    }
}

新特征比较结果 新_特征类::比较特征值(
    稳定编码 特征节点,
    const 新特征准确值& 左值,
    const 新特征准确值& 右值) const noexcept {
    新特征比较结果 结果;
    try {
        std::lock_guard 锁(新特征互斥);
        if (!结构仍存在() || !有效(特征节点)) return 结果;
        const auto 特征 = 获取特征(特征节点);
        if (!特征) {
            结果.状态 = 节点仍存在(特征节点)
                ? 新特征比较状态::结构不一致
                : 新特征比较状态::特征不存在;
            return 结果;
        }
        const auto 概念 = 特征概念服务_.获取特征概念(特征->特征概念节点);
        if (!概念) {
            结果.状态 = 新特征比较状态::概念不存在;
            return 结果;
        }
        if (!准确值材料类型相容(
            概念->材料类型, 左值, 特征值服务_)
            || !准确值材料类型相容(
                概念->材料类型, 右值, 特征值服务_)) {
            结果.状态 = 新特征比较状态::材料类型不相容;
            return 结果;
        }
        const auto 概念定位 = 特征概念服务_.比较单特征概念(
            特征->特征概念节点, 特征->特征概念节点);
        if (概念定位.状态 != 特征概念规则状态::已完成
            || !概念定位.特征类型根) {
            结果.状态 = 映射概念规则状态(概念定位.状态);
            return 结果;
        }
        return 比较双准确值(
            特征->特征概念节点, *概念定位.特征类型根,
            左值, 右值, 特征值服务_, 特征概念服务_);
    } catch (...) {
        结果.状态 = 新特征比较状态::资源失败;
        return 结果;
    }
}

新特征比较结果 新_特征类::比较特征(
    稳定编码 左特征节点,
    稳定编码 右特征节点) const noexcept {
    新特征比较结果 结果;
    try {
        std::lock_guard 锁(新特征互斥);
        if (!结构仍存在() || !有效(左特征节点) || !有效(右特征节点)) {
            return 结果;
        }
        const auto 左特征 = 获取特征(左特征节点);
        const auto 右特征 = 获取特征(右特征节点);
        if (!左特征 || !右特征) {
            const bool 左存在 = 节点仍存在(左特征节点);
            const bool 右存在 = 节点仍存在(右特征节点);
            结果.状态 = 左存在 && 右存在
                ? 新特征比较状态::结构不一致
                : 新特征比较状态::特征不存在;
            return 结果;
        }
        const auto 左概念 = 特征概念服务_.获取特征概念(
            左特征->特征概念节点);
        const auto 右概念 = 特征概念服务_.获取特征概念(
            右特征->特征概念节点);
        if (!左概念 || !右概念) {
            结果.状态 = 新特征比较状态::概念不存在;
            return 结果;
        }
        const auto 概念关系 = 特征概念服务_.比较单特征概念(
            左特征->特征概念节点, 右特征->特征概念节点);
        if (概念关系.状态 != 特征概念规则状态::已完成
            || !概念关系.关系) {
            结果.状态 = 映射概念规则状态(概念关系.状态);
            return 结果;
        }
        if (*概念关系.关系 == 单特征概念关系::无可比关系
            || !概念关系.特征类型根 || !概念关系.最近共同上位) {
            结果.状态 = 新特征比较状态::概念不相容;
            return 结果;
        }
        if (左概念->材料类型 != 右概念->材料类型) {
            结果.状态 = 新特征比较状态::材料类型不相容;
            return 结果;
        }
        if (左特征->实例单位 != 右特征->实例单位
            || 左概念->单位 != 右概念->单位) {
            结果.状态 = 新特征比较状态::单位不相容;
            return 结果;
        }

        if (左特征->当前值 && 右特征->当前值) {
            结果 = 比较双准确值(
                *概念关系.最近共同上位, *概念关系.特征类型根,
                *左特征->当前值, *右特征->当前值,
                特征值服务_, 特征概念服务_);
            return 结果;
        }
        if (左特征->当前值 && 右特征->实例历史聚合值域) {
            结果.材料 = 新特征比较材料::左准确值与右历史域;
            结果.准确值命中值域 = 准确值命中值域(
                右概念->材料类型, *右特征->实例历史聚合值域,
                *左特征->当前值, 特征值服务_);
            结果.状态 = 新特征比较状态::已完成;
            return 结果;
        }
        if (左特征->实例历史聚合值域 && 右特征->当前值) {
            结果.材料 = 新特征比较材料::左历史域与右准确值;
            结果.准确值命中值域 = 准确值命中值域(
                左概念->材料类型, *左特征->实例历史聚合值域,
                *右特征->当前值, 特征值服务_);
            结果.状态 = 新特征比较状态::已完成;
            return 结果;
        }
        if (左特征->实例历史聚合值域
            && 右特征->实例历史聚合值域) {
            结果.材料 = 新特征比较材料::双历史聚合值域;
            const auto 值域结果 = 特征概念服务_.比较值域(
                左概念->材料类型,
                *左特征->实例历史聚合值域,
                *右特征->实例历史聚合值域);
            结果.状态 = 映射概念规则状态(值域结果.状态);
            结果.值域关系 = 值域结果.关系;
            return 结果;
        }

        结果.材料 = 新特征比较材料::仅特征概念;
        结果.数值类型 = 新特征比较数值类型::相似度;
        结果.比较值 = -1;
        结果.状态 = 新特征比较状态::已完成;
        return 结果;
    } catch (...) {
        结果.状态 = 新特征比较状态::资源失败;
        return 结果;
    }
}

新特征概念值域关系结果 新_特征类::比较特征概念值域关系(
    稳定编码 左概念节点,
    稳定编码 右概念节点) const noexcept {
    新特征概念值域关系结果 结果;
    try {
        std::lock_guard 锁(新特征互斥);
        std::optional<同类型特征概念上下文> 上下文;
        结果.状态 = 读取同类型特征概念(
            左概念节点, 右概念节点, 特征概念服务_, 上下文);
        if (结果.状态 != 新特征比较状态::已完成 || !上下文) return 结果;
        const auto 值域结果 = 特征概念服务_.比较值域(
            上下文->左.材料类型, 上下文->左.值域, 上下文->右.值域);
        结果.状态 = 映射概念规则状态(值域结果.状态);
        结果.关系 = 值域结果.关系;
        return 结果;
    } catch (...) {
        结果.状态 = 新特征比较状态::资源失败;
        结果.关系.reset();
        return 结果;
    }
}

新特征概念相似度结果 新_特征类::计算特征概念值域重叠度(
    稳定编码 左概念节点,
    稳定编码 右概念节点) const noexcept {
    新特征概念相似度结果 结果;
    try {
        std::lock_guard 锁(新特征互斥);
        std::optional<同类型特征概念上下文> 上下文;
        结果.状态 = 读取同类型特征概念(
            左概念节点, 右概念节点, 特征概念服务_, 上下文);
        if (结果.状态 != 新特征比较状态::已完成 || !上下文) return 结果;
        const auto 值域关系 = 特征概念服务_.比较值域(
            上下文->左.材料类型, 上下文->左.值域, 上下文->右.值域);
        if (值域关系.状态 != 特征概念规则状态::已完成
            || !值域关系.关系) {
            结果.状态 = 映射概念规则状态(值域关系.状态);
            return 结果;
        }
        结果.相似度 = 计算值域重叠度(上下文->左.值域, 上下文->右.值域);
        if (!结果.相似度) 结果.状态 = 新特征比较状态::规则不支持;
        return 结果;
    } catch (...) {
        结果.状态 = 新特征比较状态::资源失败;
        结果.相似度.reset();
        return 结果;
    }
}

新特征概念相似度结果 新_特征类::计算特征概念材料接近度(
    稳定编码 左概念节点,
    稳定编码 右概念节点) const noexcept {
    新特征概念相似度结果 结果;
    try {
        std::lock_guard 锁(新特征互斥);
        std::optional<同类型特征概念上下文> 上下文;
        结果.状态 = 读取同类型特征概念(
            左概念节点, 右概念节点, 特征概念服务_, 上下文);
        if (结果.状态 != 新特征比较状态::已完成 || !上下文) return 结果;
        if (上下文->左.值域 == 上下文->右.值域) {
            结果.相似度 = 10000;
            return 结果;
        }

        if (const auto* 左I64 =
            std::get_if<std::vector<特征概念I64闭区间>>(&上下文->左.值域)) {
            const auto* 右I64 =
                std::get_if<std::vector<特征概念I64闭区间>>(&上下文->右.值域);
            const auto 左代表 = 左I64 ? 取得I64值域代表值(*左I64) : std::nullopt;
            const auto 右代表 = 右I64 ? 取得I64值域代表值(*右I64) : std::nullopt;
            if (!左代表 || !右代表) {
                结果.状态 = 新特征比较状态::结构不一致;
                return 结果;
            }
            结果.相似度 = 计算单维相对相似度(*左代表, *右代表);
            return 结果;
        }

        if (const auto* 左结构 =
            std::get_if<特征概念结构化I64值域>(&上下文->左.值域)) {
            const auto* 右结构 =
                std::get_if<特征概念结构化I64值域>(&上下文->右.值域);
            if (!右结构 || 左结构->分量.size() != 右结构->分量.size()
                || 左结构->分量.empty()) {
                结果.状态 = 新特征比较状态::结构不一致;
                return 结果;
            }
            std::vector<std::int64_t> 左代表;
            std::vector<std::int64_t> 右代表;
            左代表.reserve(左结构->分量.size());
            右代表.reserve(右结构->分量.size());
            for (std::size_t i = 0; i < 左结构->分量.size(); ++i) {
                const auto& 左分量 = 左结构->分量[i];
                const auto& 右分量 = 右结构->分量[i];
                if (左分量.角色 != 右分量.角色
                    || 左分量.单位 != 右分量.单位) {
                    结果.状态 = 新特征比较状态::结构不一致;
                    return 结果;
                }
                const auto 左值 = 取得I64值域代表值(左分量.值域);
                const auto 右值 = 取得I64值域代表值(右分量.值域);
                if (!左值 || !右值) {
                    结果.状态 = 新特征比较状态::结构不一致;
                    return 结果;
                }
                左代表.push_back(*左值);
                右代表.push_back(*右值);
            }
            const auto 先天 = 特征概念服务_.获取先天特征概念();
            结果.相似度 = 先天 && 上下文->特征类型根 == 先天->RGB颜色
                ? 计算RGB相似度(左代表, 右代表)
                : 计算多维相似度(左代表, 右代表);
            if (!结果.相似度) 结果.状态 = 新特征比较状态::规则不支持;
            return 结果;
        }

        const auto* 左节点组 =
            std::get_if<std::vector<稳定编码>>(&上下文->左.值域);
        const auto* 右节点组 =
            std::get_if<std::vector<稳定编码>>(&上下文->右.值域);
        if (!左节点组 || !右节点组 || 左节点组->empty() || 右节点组->empty()) {
            结果.状态 = 新特征比较状态::规则不支持;
            return 结果;
        }
        const auto 成对相似度 = [&](稳定编码 左值, 稳定编码 右值)
            -> std::optional<std::int64_t> {
            return 计算节点材料接近度(
                上下文->特征类型根, 上下文->左.材料类型,
                左值, 右值,
                特征值服务_, 特征概念服务_);
        };
        const auto 单向最佳平均 = [&](const std::vector<稳定编码>& 来源,
                                     const std::vector<稳定编码>& 目标)
            -> std::optional<long double> {
            long double 总分 = 0;
            for (const auto 来源值 : 来源) {
                std::optional<std::int64_t> 最佳;
                for (const auto 目标值 : 目标) {
                    const auto 当前 = 成对相似度(来源值, 目标值);
                    if (!当前) return std::nullopt;
                    最佳 = 最佳 ? (std::max)(*最佳, *当前) : *当前;
                }
                if (!最佳) return std::nullopt;
                总分 += *最佳;
            }
            return 总分 / static_cast<long double>(来源.size());
        };
        const auto 左到右 = 单向最佳平均(*左节点组, *右节点组);
        const auto 右到左 = 单向最佳平均(*右节点组, *左节点组);
        if (!左到右 || !右到左) {
            结果.状态 = 新特征比较状态::规则不支持;
            return 结果;
        }
        结果.相似度 = static_cast<std::int64_t>(
            (*左到右 + *右到左) / 2.0L + 0.5L);
        return 结果;
    } catch (...) {
        结果.状态 = 新特征比较状态::资源失败;
        结果.相似度.reset();
        return 结果;
    }
}

新特征值添加结果 新_特征类::添加特征值(
    稳定编码 特征节点,
    const 新特征准确值& 新值) noexcept {
    新特征值添加结果 结果;
    try {
        std::lock_guard 锁(新特征互斥);
        if (!结构仍存在() || !有效(特征节点)) return 结果;
        const auto 特征 = 获取特征(特征节点);
        if (!特征) {
            结果.状态 = 节点仍存在(特征节点)
                ? 新特征写入状态::结构不一致
                : 新特征写入状态::特征不存在;
            return 结果;
        }
        const auto 概念 = 特征概念服务_.获取特征概念(特征->特征概念节点);
        if (!概念) {
            结果.状态 = 新特征写入状态::概念不存在;
            return 结果;
        }
        if (!准确值材料类型相容(
            概念->材料类型, 新值, 特征值服务_)) {
            结果.状态 = 新特征写入状态::材料类型不相容;
            return 结果;
        }

        auto 值相等 = [&](const 新特征准确值& 左, const 新特征准确值& 右)
            -> std::optional<bool> {
            const auto 比较 = 特征概念服务_.执行比较规则(
                概念->材料类型, 概念->比较规则, 左, 右, 特征值服务_);
            if (比较.状态 != 特征概念规则状态::已完成 || !比较.相等) {
                return std::nullopt;
            }
            return *比较.相等;
        };

        bool 历史已有 = false;
        for (const auto& 历史值 : 特征->历史不同值) {
            const auto 相等 = 值相等(历史值, 新值);
            if (!相等) {
                结果.状态 = 新特征写入状态::结构不一致;
                return 结果;
            }
            if (*相等) {
                历史已有 = true;
                break;
            }
        }

        const auto 物理记录组 = 读取历史值命中记录(特征节点);
        if (!物理记录组) {
            结果.状态 = 新特征写入状态::结构不一致;
            return 结果;
        }
        std::optional<历史值命中物理记录> 原记录;
        for (const auto& 记录 : *物理记录组) {
            const auto 相等 = 值相等(记录.值, 新值);
            if (!相等) {
                结果.状态 = 新特征写入状态::结构不一致;
                return 结果;
            }
            if (*相等) {
                if (原记录) {
                    结果.状态 = 新特征写入状态::结构不一致;
                    return 结果;
                }
                原记录 = 记录;
            }
        }

        std::optional<基础字段关系> 旧直接历史字段;
        for (const auto& 字段 : 全局基础数据集.查询字段(
            特征节点, 历史值字段)) {
            const auto 值 = 解码准确值(字段);
            if (!值) {
                结果.状态 = 新特征写入状态::结构不一致;
                return 结果;
            }
            const auto 相等 = 值相等(*值, 新值);
            if (!相等) {
                结果.状态 = 新特征写入状态::结构不一致;
                return 结果;
            }
            if (*相等) {
                if (旧直接历史字段 || 原记录) {
                    结果.状态 = 新特征写入状态::结构不一致;
                    return 结果;
                }
                旧直接历史字段 = 字段;
            }
        }

        std::optional<历史值命中物理记录> 活动记录;
        bool 新建记录 = false;
        bool 修改过次数 = false;
        std::int64_t 原命中次数 = 0;
        if (原记录) {
            活动记录 = *原记录;
            原命中次数 = 原记录->命中次数;
            if (原命中次数 < (std::numeric_limits<std::int64_t>::max)()) {
                const auto 新次数 = 原命中次数 + 1;
                if (!全局基础数据集.修改字段值(
                    原记录->命中次数字段关系,
                    基础值{基础原始值{新次数}})) {
                    结果.状态 = 新特征写入状态::资源失败;
                    return 结果;
                }
                活动记录->命中次数 = 新次数;
                修改过次数 = true;
                结果.命中次数已增加 = true;
            }
        } else {
            const auto 初始次数 = 旧直接历史字段 ? std::int64_t{2} : std::int64_t{1};
            活动记录 = 建立历史值命中记录(特征节点, 新值, 初始次数);
            if (!活动记录) {
                结果.状态 = 新特征写入状态::资源失败;
                return 结果;
            }
            新建记录 = true;
            结果.命中次数已增加 = true;
            结果.历史集合已增加 = !历史已有;
        }

        const auto 回滚命中记录 = [&]() noexcept {
            if (!活动记录) return;
            if (新建记录) {
                删除历史值命中记录(*活动记录);
            } else if (修改过次数) {
                (void)全局基础数据集.修改字段值(
                    活动记录->命中次数字段关系,
                    基础值{基础原始值{原命中次数}});
            }
            结果.命中次数已增加 = false;
            结果.历史集合已增加 = false;
        };

        bool 当前相同 = false;
        if (特征->当前值) {
            const auto 相等 = 值相等(*特征->当前值, 新值);
            if (!相等) {
                回滚命中记录();
                结果.状态 = 新特征写入状态::结构不一致;
                return 结果;
            }
            当前相同 = *相等;
        }

        稳定编码 新当前字段关系{};
        std::optional<基础字段关系> 原当前字段关系;
        if (!当前相同) {
            const auto 当前字段组 = 全局基础数据集.查询字段(特征节点, 当前值字段);
            bool 写入成功 = false;
            if (当前字段组.empty()) {
                新当前字段关系 = 写入准确值字段(特征节点, 当前值字段, 新值);
                写入成功 = 有效(新当前字段关系);
            } else if (当前字段组.size() == 1) {
                原当前字段关系 = 当前字段组.front();
                写入成功 = 修改准确值字段(当前字段组.front().编码, 新值);
            }
            if (!写入成功) {
                回滚命中记录();
                结果.状态 = 当前字段组.size() > 1
                    ? 新特征写入状态::结构不一致
                    : 新特征写入状态::资源失败;
                return 结果;
            }
            结果.当前值已改变 = true;
        }

        const auto 回滚当前值 = [&]() noexcept {
            if (!结果.当前值已改变) return;
            if (有效(新当前字段关系)) {
                (void)全局基础数据集.删除字段(新当前字段关系);
            } else if (原当前字段关系 && 特征->当前值) {
                (void)修改准确值字段(
                    原当前字段关系->编码, *特征->当前值);
            }
            结果.当前值已改变 = false;
        };

        const auto 标准字段组 = 全局基础数据集.查询字段(特征节点, 标准值字段);
        if (标准字段组.size() > 1) {
            回滚当前值();
            回滚命中记录();
            结果.状态 = 新特征写入状态::结构不一致;
            return 结果;
        }
        bool 应替换标准值 = !特征->标准值;
        if (特征->标准值) {
            std::optional<std::int64_t> 当前标准次数;
            for (const auto& 历史值 : 特征->历史值命中信息) {
                const auto 相等 = 值相等(历史值.值, *特征->标准值);
                if (!相等) {
                    回滚当前值();
                    回滚命中记录();
                    结果.状态 = 新特征写入状态::结构不一致;
                    return 结果;
                }
                if (*相等) 当前标准次数 = 历史值.命中次数;
            }
            if (!当前标准次数) {
                回滚当前值();
                回滚命中记录();
                结果.状态 = 新特征写入状态::结构不一致;
                return 结果;
            }
            const auto 与标准相等 = 值相等(*特征->标准值, 新值);
            if (!与标准相等) {
                回滚当前值();
                回滚命中记录();
                结果.状态 = 新特征写入状态::结构不一致;
                return 结果;
            }
            应替换标准值 = !*与标准相等
                && 活动记录->命中次数 > *当前标准次数;
        }
        if (应替换标准值) {
            bool 写入成功 = false;
            if (标准字段组.empty()) {
                写入成功 = 有效(写入准确值字段(
                    特征节点, 标准值字段, 新值));
            } else {
                写入成功 = 修改准确值字段(标准字段组.front().编码, 新值);
            }
            if (!写入成功) {
                回滚当前值();
                回滚命中记录();
                结果.状态 = 新特征写入状态::资源失败;
                return 结果;
            }
            结果.标准值已改变 = true;
        }

        if (旧直接历史字段
            && !全局基础数据集.删除字段(旧直接历史字段->编码)) {
            结果.状态 = 新特征写入状态::值已更新但聚合未完成;
            return 结果;
        }

        const auto 最新记录组 = 读取历史值命中记录(特征节点);
        if (!最新记录组 || 最新记录组->empty()) {
            结果.状态 = 新特征写入状态::值已更新但聚合未完成;
            return 结果;
        }
        const auto 当前记录位置 = std::find_if(
            最新记录组->begin(), 最新记录组->end(),
            [&](const auto& 记录) { return 记录.记录节点 == 活动记录->记录节点; });
        if (当前记录位置 == 最新记录组->end()) {
            结果.状态 = 新特征写入状态::值已更新但聚合未完成;
            return 结果;
        }

        const auto 类型定位 = 特征概念服务_.比较单特征概念(
            特征->特征概念节点, 特征->特征概念节点);
        if (类型定位.状态 != 特征概念规则状态::已完成
            || !类型定位.特征类型根) {
            结果.状态 = 新特征写入状态::值已更新但聚合未完成;
            return 结果;
        }
        constexpr std::int64_t 初始归组相似度阈值 = 9000;
        const bool 连续连接归组 =
            概念->材料类型 == 特征概念材料物理类型::I64标量;
        const auto 计算归组相似度 = [&](const 新特征准确值& 左,
            const 新特征准确值& 右) -> std::optional<std::int64_t> {
            if (连续连接归组) {
                const auto* 左I64 = std::get_if<std::int64_t>(&左);
                const auto* 右I64 = std::get_if<std::int64_t>(&右);
                return 左I64 && 右I64
                    ? std::optional<std::int64_t>{
                        计算单维相对相似度(*左I64, *右I64)}
                    : std::nullopt;
            }
            const auto* 左节点 = std::get_if<稳定编码>(&左);
            const auto* 右节点 = std::get_if<稳定编码>(&右);
            if (!左节点 || !右节点) return std::nullopt;
            return 计算节点材料接近度(
                *类型定位.特征类型根, 概念->材料类型,
                *左节点, *右节点, 特征值服务_, 特征概念服务_)
                .value_or(-1);
        };

        const auto 设置唯一准确值字段 = [&](稳定编码 节点,
            稳定编码 字段, const 新特征准确值& 值) {
            const auto 字段组 = 全局基础数据集.查询字段(节点, 字段);
            if (字段组.empty()) return 有效(写入准确值字段(节点, 字段, 值));
            return 字段组.size() == 1
                && 修改准确值字段(字段组.front().编码, 值);
        };

        const auto 建立或更新归组子特征 = [&](
            std::optional<稳定编码> 已有子特征,
            const std::vector<历史值命中物理记录>& 成员记录,
            const 历史值命中物理记录& 标准记录)
            -> std::optional<稳定编码> {
            const auto 子值域 = 历史记录形成有限值域(
                概念->材料类型, 成员记录);
            if (!子值域) return std::nullopt;
            稳定编码 子特征节点{};
            if (已有子特征) {
                const auto 子概念 = 读取唯一节点字段(
                    *已有子特征, 特征概念字段);
                const auto 子概念信息 = 子概念
                    ? 特征概念服务_.获取特征概念(*子概念) : std::nullopt;
                const auto 上位关系 = 子概念
                    ? 全局基础数据集.查询目标关系(
                        *子概念, 基础外部关系类型::父子)
                    : std::vector<基础外部关系>{};
                if (!子概念信息 || 上位关系.size() != 1) {
                    return std::nullopt;
                }
                子特征节点 = *已有子特征;
            } else {
                子特征节点 = 建立或取得子特征节点(特征节点, *子值域);
                if (!有效(子特征节点)) return std::nullopt;
                const auto 标记组 = 全局基础数据集.查询字段(
                    子特征节点, 归组子特征标记字段);
                if (标记组.empty()) {
                    if (!有效(全局基础数据集.添加字段值(
                        子特征节点, 归组子特征标记字段,
                        基础值{基础原始值{std::int64_t{1}}}))) {
                        return std::nullopt;
                    }
                } else if (标记组.size() != 1
                    || 读取唯一I64字段(
                        子特征节点, 归组子特征标记字段) != 1) {
                    return std::nullopt;
                }
            }

            auto 已有关联 = 全局基础数据集.查询字段(
                子特征节点, 历史值命中记录字段);
            for (const auto& 记录 : 成员记录) {
                const bool 已有 = std::any_of(
                    已有关联.begin(), 已有关联.end(), [&](const auto& 字段) {
                        const auto* 节点 = std::get_if<稳定编码>(&字段.内容);
                        return 节点 && *节点 == 记录.记录节点;
                    });
                if (!已有 && !有效(全局基础数据集.添加字段节点(
                    子特征节点, 历史值命中记录字段, 记录.记录节点))) {
                    return std::nullopt;
                }
            }
            std::vector<特征概念准确值> 聚合输入;
            聚合输入.reserve(成员记录.size());
            for (const auto& 记录 : 成员记录) 聚合输入.push_back(记录.值);
            const auto 子聚合 = 特征概念服务_.执行聚合规则(
                概念->材料类型, 概念->聚合规则,
                聚合输入, 特征值服务_);
            if ((子聚合.状态 != 特征概念规则状态::已完成
                    && 子聚合.状态 != 特征概念规则状态::无变化)
                || !子聚合.值域) {
                return std::nullopt;
            }
            if (!设置唯一准确值字段(
                子特征节点, 标准值字段, 标准记录.值)
                || !发布历史聚合值域(
                    子特征节点, 概念->材料类型, *子聚合.值域)) {
                return std::nullopt;
            }
            const auto 子特征信息 = 获取特征(子特征节点);
            if (!子特征信息 || !同步实例所属概念(
                子特征节点, 子特征信息->特征概念节点,
                *子聚合.值域, 成员记录,
                特征值服务_, 特征概念服务_)) {
                return std::nullopt;
            }
            const bool 接收当前值 = std::any_of(
                成员记录.begin(), 成员记录.end(), [&](const auto& 记录) {
                    return 记录.记录节点 == 活动记录->记录节点;
                });
            if (接收当前值 && !设置唯一准确值字段(
                子特征节点, 当前值字段, 新值)) {
                return std::nullopt;
            }
            return 子特征节点;
        };

        const auto 子特征组 = 查询归组子特征(特征节点);
        if (子特征组.empty()) {
            bool 加入现有单区间 = !结果.历史集合已增加
                || 特征->历史不同值.empty();
            if (!加入现有单区间 && 特征->标准值) {
                if (连续连接归组) {
                    for (const auto& 历史值 : 特征->历史不同值) {
                        const auto 相似度 = 计算归组相似度(历史值, 新值);
                        if (!相似度) {
                            结果.状态 = 新特征写入状态::值已更新但聚合未完成;
                            return 结果;
                        }
                        if (*相似度 >= 初始归组相似度阈值) {
                            加入现有单区间 = true;
                            break;
                        }
                    }
                } else {
                    const auto 相似度 = 计算归组相似度(*特征->标准值, 新值);
                    if (!相似度) {
                        结果.状态 = 新特征写入状态::值已更新但聚合未完成;
                        return 结果;
                    }
                    加入现有单区间 = *相似度 >= 初始归组相似度阈值;
                }
            }

            if (加入现有单区间) {
                // 单一区间直接由本实例的正式所属概念表达，不再维护旁路概念字段。
            } else {
                std::vector<历史值命中物理记录> 原区间记录;
                for (const auto& 记录 : *最新记录组) {
                    if (记录.记录节点 != 活动记录->记录节点) {
                        原区间记录.push_back(记录);
                    }
                }
                if (原区间记录.empty() || !特征->标准值) {
                    结果.状态 = 新特征写入状态::值已更新但聚合未完成;
                    return 结果;
                }
                const auto 原标准位置 = std::find_if(
                    原区间记录.begin(), 原区间记录.end(), [&](const auto& 记录) {
                        const auto 相等 = 值相等(记录.值, *特征->标准值);
                        return 相等 && *相等;
                    });
                if (原标准位置 == 原区间记录.end()) {
                    结果.状态 = 新特征写入状态::值已更新但聚合未完成;
                    return 结果;
                }
                const auto 原子特征 = 建立或更新归组子特征(
                    std::nullopt, 原区间记录, *原标准位置);
                const auto 新子特征 = 建立或更新归组子特征(
                    std::nullopt,
                    std::vector<历史值命中物理记录>{*当前记录位置},
                    *当前记录位置);
                if (!原子特征 || !新子特征) {
                    结果.状态 = 新特征写入状态::值已更新但聚合未完成;
                    return 结果;
                }
                结果.接收子特征节点 = *新子特征;
            }
        } else {
            struct 候选区间 final {
                稳定编码 子特征;
                std::vector<历史值命中物理记录> 记录;
                历史值命中物理记录 标准;
                std::int64_t 标准相似度 = -1;
                bool 已含当前记录 = false;
            };
            std::vector<候选区间> 候选组;
            std::optional<候选区间> 已属区间;
            for (const auto 子特征 : 子特征组) {
                const auto 子记录 = 读取历史值命中记录(子特征);
                const auto 子标准字段组 = 全局基础数据集.查询字段(
                    子特征, 标准值字段);
                const auto 子标准值 = 子标准字段组.size() == 1
                    ? 解码准确值(子标准字段组.front()) : std::nullopt;
                if (!子标准值 || !子记录 || 子记录->empty()) {
                    结果.状态 = 新特征写入状态::值已更新但聚合未完成;
                    return 结果;
                }
                const auto 标准位置 = std::find_if(
                    子记录->begin(), 子记录->end(), [&](const auto& 记录) {
                        const auto 相等 = 值相等(记录.值, *子标准值);
                        return 相等 && *相等;
                    });
                if (标准位置 == 子记录->end()) {
                    结果.状态 = 新特征写入状态::值已更新但聚合未完成;
                    return 结果;
                }
                const bool 已含 = std::any_of(
                    子记录->begin(), 子记录->end(), [&](const auto& 记录) {
                        return 记录.记录节点 == 活动记录->记录节点;
                    });
                const auto 标准相似度 = 计算归组相似度(标准位置->值, 新值);
                if (!标准相似度) {
                    结果.状态 = 新特征写入状态::值已更新但聚合未完成;
                    return 结果;
                }
                候选区间 候选{子特征, *子记录, *标准位置, *标准相似度, 已含};
                if (已含) {
                    if (已属区间) {
                        结果.状态 = 新特征写入状态::结构不一致;
                        return 结果;
                    }
                    已属区间 = 候选;
                    continue;
                }
                bool 连接 = *标准相似度 >= 初始归组相似度阈值;
                if (连续连接归组 && !连接) {
                    for (const auto& 记录 : *子记录) {
                        const auto 相似度 = 计算归组相似度(记录.值, 新值);
                        if (!相似度) {
                            结果.状态 = 新特征写入状态::值已更新但聚合未完成;
                            return 结果;
                        }
                        if (*相似度 >= 初始归组相似度阈值) {
                            连接 = true;
                            break;
                        }
                    }
                }
                if (连接) 候选组.push_back(std::move(候选));
            }

            候选区间 接收区间;
            if (已属区间) {
                接收区间 = *已属区间;
            } else if (!候选组.empty()) {
                std::sort(候选组.begin(), 候选组.end(), [](const auto& 左, const auto& 右) {
                    if (左.标准相似度 != 右.标准相似度) {
                        return 左.标准相似度 > 右.标准相似度;
                    }
                    return 左.子特征 < 右.子特征;
                });
                接收区间 = 候选组.front();
                接收区间.记录.push_back(*当前记录位置);
            } else {
                const auto 新子特征 = 建立或更新归组子特征(
                    std::nullopt,
                    std::vector<历史值命中物理记录>{*当前记录位置},
                    *当前记录位置);
                if (!新子特征) {
                    结果.状态 = 新特征写入状态::值已更新但聚合未完成;
                    return 结果;
                }
                结果.接收子特征节点 = *新子特征;
            }
            if (!结果.接收子特征节点) {
                auto 新标准 = 接收区间.标准;
                if (当前记录位置->命中次数 > 新标准.命中次数) {
                    新标准 = *当前记录位置;
                }
                const auto 已更新子特征 = 建立或更新归组子特征(
                    接收区间.子特征, 接收区间.记录, 新标准);
                if (!已更新子特征) {
                    结果.状态 = 新特征写入状态::值已更新但聚合未完成;
                    return 结果;
                }
                结果.接收子特征节点 = *已更新子特征;
            }
        }

        auto 聚合输入 = 特征->历史不同值;
        if (结果.历史集合已增加) 聚合输入.push_back(新值);
        结果.聚合结果 = 特征概念服务_.执行聚合规则(
            概念->材料类型, 概念->聚合规则, 聚合输入, 特征值服务_);
        if (结果.聚合结果->状态 != 特征概念规则状态::已完成
            && 结果.聚合结果->状态 != 特征概念规则状态::无变化) {
            结果.状态 = 新特征写入状态::值已更新但聚合未完成;
            return 结果;
        }
        if (!结果.聚合结果->值域) {
            结果.状态 = 新特征写入状态::值已更新但聚合未完成;
            return 结果;
        }
        const bool 值域需要写回 = !特征->实例历史聚合值域
            || *特征->实例历史聚合值域 != *结果.聚合结果->值域;
        if (值域需要写回
            && !发布历史聚合值域(
                特征节点, 概念->材料类型, *结果.聚合结果->值域)) {
            结果.状态 = 新特征写入状态::值已更新但聚合未完成;
            return 结果;
        }
        if (!同步实例所属概念(
            特征节点, 特征->特征概念节点,
            *结果.聚合结果->值域, *最新记录组,
            特征值服务_, 特征概念服务_)) {
            结果.状态 = 新特征写入状态::值已更新但聚合未完成;
            return 结果;
        }
        if (!结果.历史集合已增加
            && !结果.命中次数已增加
            && !结果.当前值已改变
            && !结果.标准值已改变
            && !值域需要写回) {
            结果.状态 = 新特征写入状态::无变化;
            return 结果;
        }
        结果.状态 = 新特征写入状态::已完成;
        return 结果;
    } catch (...) {
        结果.状态 = 结果.历史集合已增加
            || 结果.命中次数已增加
            || 结果.当前值已改变
            || 结果.标准值已改变
            ? 新特征写入状态::值已更新但聚合未完成
            : 新特征写入状态::资源失败;
        return 结果;
    }
}

} // namespace 海中鱼巣
