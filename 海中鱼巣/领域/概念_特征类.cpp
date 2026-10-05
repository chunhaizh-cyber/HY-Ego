#include "概念_特征类.h"

#include <algorithm>
#include <limits>
#include <mutex>
#include <utility>

namespace 海中鱼巣 {

稳定编码 特征概念树{};

namespace {

std::recursive_mutex 特征概念树互斥;

bool 是I64类型(特征概念材料物理类型 类型) noexcept;
std::optional<材料物理类型> 转换特征值材料类型(
    特征概念材料物理类型 类型) noexcept;
std::uint64_t 计算I64绝对差(
    std::int64_t 左值, std::int64_t 右值) noexcept;
bool 相邻或重叠(
    const 特征概念I64闭区间& 左,
    const 特征概念I64闭区间& 右) noexcept;
std::optional<稳定编码> 读取唯一节点字段(
    稳定编码 节点, 稳定编码 字段);

稳定编码 节点类型字段{};
稳定编码 特征概念节点类型{};
稳定编码 材料类型字段{};
稳定编码 I64值域字段{};
稳定编码 全材料值域字段{};
稳定编码 非I64值域字段{};
稳定编码 存在引用值域字段{};
稳定编码 结构化分量字段{};
稳定编码 结构化分量节点类型{};
稳定编码 分量顺序字段{};
稳定编码 分量角色字段{};
稳定编码 分量材料类型字段{};
稳定编码 分量I64值域字段{};
稳定编码 分量单位字段{};
稳定编码 单位字段{};
稳定编码 比较规则字段{};
稳定编码 聚合规则字段{};
稳定编码 名称关系字段{};
稳定编码 当前引用实例数字段{};

稳定编码 三维坐标X角色{};
稳定编码 三维坐标Y角色{};
稳定编码 三维坐标Z角色{};
稳定编码 三维尺寸X跨度角色{};
稳定编码 三维尺寸Y跨度角色{};
稳定编码 三维尺寸Z跨度角色{};
稳定编码 RGB红色角色{};
稳定编码 RGB绿色角色{};
稳定编码 RGB蓝色角色{};
先天特征概念集合 先天概念{};

bool 结构已初始化() noexcept {
    return 有效(特征概念树)
        && 有效(节点类型字段)
        && 有效(特征概念节点类型)
        && 有效(材料类型字段)
        && 有效(I64值域字段)
        && 有效(全材料值域字段)
        && 有效(非I64值域字段)
        && 有效(存在引用值域字段)
        && 有效(结构化分量字段)
        && 有效(结构化分量节点类型)
        && 有效(分量顺序字段)
        && 有效(分量角色字段)
        && 有效(分量材料类型字段)
        && 有效(分量I64值域字段)
        && 有效(分量单位字段)
        && 有效(单位字段)
        && 有效(比较规则字段)
        && 有效(聚合规则字段)
        && 有效(名称关系字段)
        && 有效(当前引用实例数字段);
}

bool 节点仍存在(稳定编码 节点) noexcept {
    return 有效(节点) && 全局基础数据集.查询节点(节点).has_value();
}

bool 结构仍存在() noexcept {
    return 结构已初始化()
        && 节点仍存在(特征概念树)
        && 节点仍存在(节点类型字段)
        && 节点仍存在(特征概念节点类型)
        && 节点仍存在(材料类型字段)
        && 节点仍存在(I64值域字段)
        && 节点仍存在(全材料值域字段)
        && 节点仍存在(非I64值域字段)
        && 节点仍存在(存在引用值域字段)
        && 节点仍存在(结构化分量字段)
        && 节点仍存在(结构化分量节点类型)
        && 节点仍存在(分量顺序字段)
        && 节点仍存在(分量角色字段)
        && 节点仍存在(分量材料类型字段)
        && 节点仍存在(分量I64值域字段)
        && 节点仍存在(分量单位字段)
        && 节点仍存在(单位字段)
        && 节点仍存在(比较规则字段)
        && 节点仍存在(聚合规则字段)
        && 节点仍存在(名称关系字段)
        && 节点仍存在(当前引用实例数字段);
}

稳定编码 新建结构节点() noexcept {
    // 字段定义节点只作为内部字段关系的键，不进入概念树外部根链。
    return 全局基础数据集.新建节点();
}

bool 是支持的材料类型(特征概念材料物理类型 类型) noexcept {
    switch (类型) {
    case 特征概念材料物理类型::I64标量:
    case 特征概念材料物理类型::U64数组:
    case 特征概念材料物理类型::I64数组:
    case 特征概念材料物理类型::UTF8字符串:
    case 特征概念材料物理类型::二维二值格:
    case 特征概念材料物理类型::三维二值格:
    case 特征概念材料物理类型::存在节点引用:
        return true;
    }
    return false;
}

bool 是支持的比较规则(特征概念比较规则 规则) noexcept {
    return 规则 == 特征概念比较规则::I64数值
        || 规则 == 特征概念比较规则::完整材料精确
        || 规则 == 特征概念比较规则::存在身份精确;
}

bool 是支持的聚合规则(特征概念聚合规则 规则) noexcept {
    return 规则 == 特征概念聚合规则::不自动聚合
        || 规则 == 特征概念聚合规则::I64连续值归组
        || 规则 == 特征概念聚合规则::完整材料精确集合;
}

bool 规范化I64区间组(std::vector<特征概念I64闭区间>& 区间组) {
    if (区间组.empty()) return false;
    for (const auto& 区间 : 区间组) {
        if (区间.下界 > 区间.上界) return false;
    }
    std::sort(区间组.begin(), 区间组.end(), [](const auto& 左, const auto& 右) {
        return 左.下界 < 右.下界
            || (左.下界 == 右.下界 && 左.上界 < 右.上界);
    });
    std::vector<特征概念I64闭区间> 归并;
    for (const auto& 区间 : 区间组) {
        if (归并.empty() || !相邻或重叠(归并.back(), 区间)) {
            归并.push_back(区间);
        } else if (区间.上界 > 归并.back().上界) {
            归并.back().上界 = 区间.上界;
        }
    }
    区间组 = std::move(归并);
    return true;
}

bool 规则与材料相容(const 特征概念定义& 定义) noexcept {
    if (!是支持的材料类型(定义.材料类型)
        || !是支持的比较规则(定义.比较规则)
        || !是支持的聚合规则(定义.聚合规则)) {
        return false;
    }
    if (定义.材料类型 == 特征概念材料物理类型::存在节点引用) {
        const auto* 引用域 = std::get_if<特征概念存在引用值域>(&定义.值域);
        return 引用域 && 有效(引用域->存在概念节点)
            && !定义.单位
            && 定义.比较规则 == 特征概念比较规则::存在身份精确
            && 定义.聚合规则 == 特征概念聚合规则::不自动聚合;
    }
    if (是I64类型(定义.材料类型)) {
        return 定义.比较规则 == 特征概念比较规则::I64数值
            && 定义.聚合规则 != 特征概念聚合规则::完整材料精确集合
            && std::holds_alternative<std::vector<特征概念I64闭区间>>(定义.值域);
    }
    if (定义.比较规则 != 特征概念比较规则::完整材料精确
        || 定义.聚合规则 == 特征概念聚合规则::I64连续值归组) {
        return false;
    }
    if (std::holds_alternative<特征概念结构化I64值域>(定义.值域)) {
        return 定义.材料类型 == 特征概念材料物理类型::I64数组;
    }
    return std::holds_alternative<特征概念全材料值域>(定义.值域)
        || std::holds_alternative<std::vector<稳定编码>>(定义.值域);
}

std::optional<特征概念定义> 规范化定义(const 特征概念定义& 输入) {
    if (!规则与材料相容(输入)) return std::nullopt;

    特征概念定义 结果 = 输入;
    if (auto* 区间组 = std::get_if<std::vector<特征概念I64闭区间>>(&结果.值域)) {
        if (!规范化I64区间组(*区间组)) return std::nullopt;
    } else if (auto* 节点组 = std::get_if<std::vector<稳定编码>>(&结果.值域)) {
        if (节点组->empty()
            || std::any_of(节点组->begin(), 节点组->end(),
                [](稳定编码 节点) { return !有效(节点); })) {
            return std::nullopt;
        }
        std::sort(节点组->begin(), 节点组->end());
        节点组->erase(
            std::unique(节点组->begin(), 节点组->end()), 节点组->end());
    } else if (auto* 结构化 =
        std::get_if<特征概念结构化I64值域>(&结果.值域)) {
        if (结构化->分量.empty()) return std::nullopt;
        std::vector<稳定编码> 已有角色;
        for (auto& 分量 : 结构化->分量) {
            if (!有效(分量.角色) || !规范化I64区间组(分量.值域)
                || (分量.单位 && !有效(*分量.单位))) {
                return std::nullopt;
            }
            if (std::find(已有角色.begin(), 已有角色.end(), 分量.角色)
                != 已有角色.end()) {
                return std::nullopt;
            }
            已有角色.push_back(分量.角色);
        }
    } else if (const auto* 引用域 =
        std::get_if<特征概念存在引用值域>(&结果.值域)) {
        if (!有效(引用域->存在概念节点)) return std::nullopt;
    }

    if (结果.单位 && !有效(*结果.单位)) return std::nullopt;
    if (std::any_of(结果.名称关系.begin(), 结果.名称关系.end(),
        [](稳定编码 节点) { return !有效(节点); })) {
        return std::nullopt;
    }
    std::sort(结果.名称关系.begin(), 结果.名称关系.end());
    结果.名称关系.erase(
        std::unique(结果.名称关系.begin(), 结果.名称关系.end()),
        结果.名称关系.end());
    return 结果;
}

特征概念定义 转为定义(const 特征概念信息& 信息) {
    return {
        信息.材料类型,
        信息.值域,
        信息.单位,
        信息.比较规则,
        信息.聚合规则,
        信息.名称关系};
}

bool 结构定义相同(
    const 特征概念定义& 左,
    const 特征概念定义& 右) noexcept {
    return 左.材料类型 == 右.材料类型
        && 左.值域 == 右.值域
        && 左.单位 == 右.单位
        && 左.比较规则 == 右.比较规则
        && 左.聚合规则 == 右.聚合规则;
}

bool I64值域包含(
    const std::vector<特征概念I64闭区间>& 父域,
    const std::vector<特征概念I64闭区间>& 子域) noexcept {
    return std::all_of(子域.begin(), 子域.end(),
        [&](const 特征概念I64闭区间& 子区间) {
            return std::any_of(父域.begin(), 父域.end(),
                [&](const 特征概念I64闭区间& 父区间) {
                    return 父区间.下界 <= 子区间.下界
                        && 父区间.上界 >= 子区间.上界;
                });
        });
}

bool 节点集合包含(
    const std::vector<稳定编码>& 父域,
    const std::vector<稳定编码>& 子域) noexcept {
    return std::all_of(子域.begin(), 子域.end(), [&](稳定编码 子项) {
        return std::binary_search(父域.begin(), 父域.end(), 子项);
    });
}

bool 有限材料节点受定义包含(
    const 特征概念定义& 父,
    const 特征概念定义& 子,
    const 新_特征值类& 特征值服务) noexcept {
    const auto* 子节点组 = std::get_if<std::vector<稳定编码>>(&子.值域);
    if (!子节点组 || 子节点组->empty()
        || 父.材料类型 != 子.材料类型 || 父.单位 != 子.单位) {
        return false;
    }
    if (std::holds_alternative<特征概念全材料值域>(父.值域)) return true;
    if (const auto* 父节点组 = std::get_if<std::vector<稳定编码>>(&父.值域)) {
        return 节点集合包含(*父节点组, *子节点组);
    }
    const auto* 父结构 = std::get_if<特征概念结构化I64值域>(&父.值域);
    if (!父结构 || 父.材料类型 != 特征概念材料物理类型::I64数组) {
        return false;
    }
    for (const auto 值节点 : *子节点组) {
        const auto 信息 = 特征值服务.获取特征值(值节点);
        const auto* 数组 = 信息
            ? std::get_if<std::vector<std::int64_t>>(&信息->材料) : nullptr;
        if (!数组 || 数组->size() != 父结构->分量.size()) return false;
        for (std::size_t i = 0; i < 数组->size(); ++i) {
            const bool 命中 = std::any_of(
                父结构->分量[i].值域.begin(), 父结构->分量[i].值域.end(),
                [&](const auto& 区间) {
                    return 区间.下界 <= (*数组)[i] && (*数组)[i] <= 区间.上界;
                });
            if (!命中) return false;
        }
    }
    return true;
}

bool 是严格子值域(
    const 特征概念定义& 父,
    const 特征概念定义& 子) noexcept {
    if (父.材料类型 != 子.材料类型 || 父.单位 != 子.单位) {
        return false;
    }
    if (父.材料类型 == 特征概念材料物理类型::存在节点引用) {
        return false;
    }
    if (const auto* 父I64 =
        std::get_if<std::vector<特征概念I64闭区间>>(&父.值域)) {
        const auto* 子I64 =
            std::get_if<std::vector<特征概念I64闭区间>>(&子.值域);
        if (!子I64) return false;
        return *父I64 != *子I64 && I64值域包含(*父I64, *子I64);
    }
    if (std::holds_alternative<特征概念全材料值域>(父.值域)) {
        return std::holds_alternative<std::vector<稳定编码>>(子.值域);
    }
    if (const auto* 父节点组 = std::get_if<std::vector<稳定编码>>(&父.值域)) {
        const auto* 子节点组 = std::get_if<std::vector<稳定编码>>(&子.值域);
        return 子节点组 && *父节点组 != *子节点组
            && 节点集合包含(*父节点组, *子节点组);
    }
    const auto* 父结构 = std::get_if<特征概念结构化I64值域>(&父.值域);
    const auto* 子结构 = std::get_if<特征概念结构化I64值域>(&子.值域);
    if (!父结构 || !子结构
        || 父结构->分量.size() != 子结构->分量.size()) {
        return false;
    }
    bool 存在严格收缩 = false;
    for (std::size_t i = 0; i < 父结构->分量.size(); ++i) {
        const auto& 父分量 = 父结构->分量[i];
        const auto& 子分量 = 子结构->分量[i];
        if (父分量.角色 != 子分量.角色 || 父分量.单位 != 子分量.单位
            || !I64值域包含(父分量.值域, 子分量.值域)) {
            return false;
        }
        存在严格收缩 = 存在严格收缩 || 父分量.值域 != 子分量.值域;
    }
    return 存在严格收缩;
}

bool I64值域相交(
    const std::vector<特征概念I64闭区间>& 左域,
    const std::vector<特征概念I64闭区间>& 右域) noexcept {
    return std::any_of(左域.begin(), 左域.end(), [&](const auto& 左区间) {
        return std::any_of(右域.begin(), 右域.end(), [&](const auto& 右区间) {
            return 左区间.下界 <= 右区间.上界
                && 右区间.下界 <= 左区间.上界;
        });
    });
}

bool 节点集合相交(
    const std::vector<稳定编码>& 左域,
    const std::vector<稳定编码>& 右域) noexcept {
    return std::any_of(左域.begin(), 左域.end(), [&](稳定编码 左项) {
        return std::binary_search(右域.begin(), 右域.end(), 左项);
    });
}

std::optional<特征概念值域> 规范化待比较值域(
    特征概念材料物理类型 材料类型,
    const 特征概念值域& 值域) {
    特征概念定义 定义;
    定义.材料类型 = 材料类型;
    定义.值域 = 值域;
    if (材料类型 == 特征概念材料物理类型::I64标量) {
        定义.比较规则 = 特征概念比较规则::I64数值;
        定义.聚合规则 = 特征概念聚合规则::不自动聚合;
    } else if (材料类型 == 特征概念材料物理类型::存在节点引用) {
        定义.比较规则 = 特征概念比较规则::存在身份精确;
        定义.聚合规则 = 特征概念聚合规则::不自动聚合;
    } else {
        定义.比较规则 = 特征概念比较规则::完整材料精确;
        定义.聚合规则 = 特征概念聚合规则::完整材料精确集合;
    }
    const auto 规范定义 = 规范化定义(定义);
    return 规范定义
        ? std::optional<特征概念值域>{规范定义->值域}
        : std::nullopt;
}

struct 单概念路径 final {
    std::vector<稳定编码> 从自身到根;
    稳定编码 根;
};

std::optional<单概念路径> 读取单概念路径(稳定编码 概念节点) {
    if (!有效(概念节点)) return std::nullopt;
    单概念路径 结果;
    稳定编码 当前 = 概念节点;
    while (当前 != 特征概念树) {
        if (std::find(结果.从自身到根.begin(), 结果.从自身到根.end(), 当前)
            != 结果.从自身到根.end()) {
            return std::nullopt;
        }
        const auto 类型 = 读取唯一节点字段(当前, 节点类型字段);
        if (!类型 || *类型 != 特征概念节点类型) return std::nullopt;
        结果.从自身到根.push_back(当前);
        const auto 上位 = 全局基础数据集.查询目标关系(
            当前, 基础外部关系类型::父子);
        if (上位.size() != 1) return std::nullopt;
        当前 = 上位.front().源节点;
    }
    if (结果.从自身到根.empty()) return std::nullopt;
    结果.根 = 结果.从自身到根.back();
    return 结果;
}

std::optional<std::uint64_t> I64命中区间宽度(
    const std::vector<特征概念I64闭区间>& 值域,
    std::int64_t 值) noexcept {
    std::optional<std::uint64_t> 最小宽度;
    for (const auto& 区间 : 值域) {
        if (值 < 区间.下界 || 值 > 区间.上界) continue;
        const auto 宽度 = 计算I64绝对差(区间.下界, 区间.上界);
        if (!最小宽度 || 宽度 < *最小宽度) 最小宽度 = 宽度;
    }
    return 最小宽度;
}

std::optional<std::uint64_t> 值域匹配尺度(
    const 特征概念信息& 概念,
    const 特征概念准确值& 值,
    const 新_特征值类& 特征值服务,
    const 特征存在引用读取接口* 存在读取接口) noexcept {
    if (概念.材料类型 == 特征概念材料物理类型::I64标量) {
        const auto* I64 = std::get_if<std::int64_t>(&值);
        if (!I64) return std::nullopt;
        return I64命中区间宽度(
            std::get<std::vector<特征概念I64闭区间>>(概念.值域), *I64);
    }

    const auto* 值节点 = std::get_if<稳定编码>(&值);
    if (概念.材料类型 == 特征概念材料物理类型::存在节点引用) {
        const auto* 引用域 = std::get_if<特征概念存在引用值域>(&概念.值域);
        return 存在读取接口 && 值节点 && 引用域
            && 存在读取接口->存在节点属于概念(
                *值节点, 引用域->存在概念节点)
            ? std::optional<std::uint64_t>{0}
            : std::nullopt;
    }
    const auto 期望类型 = 转换特征值材料类型(概念.材料类型);
    if (!期望类型 || !值节点 || !有效(*值节点)) return std::nullopt;
    const auto 值信息 = 特征值服务.获取特征值(*值节点);
    if (!值信息 || 值信息->物理类型 != *期望类型) return std::nullopt;
    if (std::holds_alternative<特征概念全材料值域>(概念.值域)) {
        return (std::numeric_limits<std::uint64_t>::max)();
    }
    if (const auto* 值域 = std::get_if<std::vector<稳定编码>>(&概念.值域)) {
        return std::binary_search(值域->begin(), 值域->end(), *值节点)
            ? std::optional<std::uint64_t>{static_cast<std::uint64_t>(值域->size())}
            : std::nullopt;
    }
    const auto* 结构化 = std::get_if<特征概念结构化I64值域>(&概念.值域);
    const auto* I64组 = std::get_if<std::vector<std::int64_t>>(&值信息->材料);
    if (!结构化 || !I64组 || I64组->size() != 结构化->分量.size()) {
        return std::nullopt;
    }
    for (std::size_t i = 0; i < I64组->size(); ++i) {
        if (!I64命中区间宽度(结构化->分量[i].值域, (*I64组)[i])) {
            return std::nullopt;
        }
    }
    // 结构化概念的具体性由已建立的严格子值域层级裁决；维度数量不作为尺度。
    return (std::numeric_limits<std::uint64_t>::max)();
}

std::optional<std::int64_t> 读取唯一I64字段(
    稳定编码 节点, 稳定编码 字段) {
    const auto 字段组 = 全局基础数据集.查询字段(节点, 字段);
    if (字段组.size() != 1) return std::nullopt;
    const auto* 值 = std::get_if<基础值>(&字段组.front().内容);
    if (!值) return std::nullopt;
    return std::get_if<std::int64_t>(&值->材料)
        ? std::optional<std::int64_t>{std::get<std::int64_t>(值->材料)}
        : std::nullopt;
}

std::optional<std::vector<std::int64_t>> 读取唯一I64组字段(
    稳定编码 节点, 稳定编码 字段) {
    const auto 字段组 = 全局基础数据集.查询字段(节点, 字段);
    if (字段组.size() != 1) return std::nullopt;
    const auto* 值 = std::get_if<基础值>(&字段组.front().内容);
    if (!值) return std::nullopt;
    const auto* 数组 = std::get_if<std::vector<std::int64_t>>(&值->材料);
    return 数组 ? std::optional<std::vector<std::int64_t>>{*数组} : std::nullopt;
}

std::optional<稳定编码> 读取唯一节点字段(
    稳定编码 节点, 稳定编码 字段) {
    const auto 字段组 = 全局基础数据集.查询字段(节点, 字段);
    if (字段组.size() != 1) return std::nullopt;
    const auto* 目标 = std::get_if<稳定编码>(&字段组.front().内容);
    return 目标 ? std::optional<稳定编码>{*目标} : std::nullopt;
}

std::vector<稳定编码> 读取多节点字段(稳定编码 节点, 稳定编码 字段) {
    std::vector<稳定编码> 结果;
    for (const auto& 关系 : 全局基础数据集.查询字段(节点, 字段)) {
        const auto* 目标 = std::get_if<稳定编码>(&关系.内容);
        if (!目标) return {};
        结果.push_back(*目标);
    }
    std::sort(结果.begin(), 结果.end());
    结果.erase(std::unique(结果.begin(), 结果.end()), 结果.end());
    return 结果;
}

void 回滚字段和节点(
    稳定编码 节点, const std::vector<稳定编码>& 字段关系组) noexcept {
    for (auto 位置 = 字段关系组.rbegin(); 位置 != 字段关系组.rend(); ++位置) {
        全局基础数据集.删除字段(*位置);
    }
    全局基础数据集.删除节点(节点);
}

enum class 零引用概念清理结果 : std::uint8_t {
    无须删除 = 0,
    已删除 = 1,
    失败 = 2
};

零引用概念清理结果 尝试删除零引用叶概念(
    稳定编码 概念节点) noexcept {
    const auto 当前数量 = 读取唯一I64字段(
        概念节点, 当前引用实例数字段);
    if (!当前数量 || *当前数量 != 0) {
        return 当前数量 ? 零引用概念清理结果::无须删除
                         : 零引用概念清理结果::失败;
    }

    const auto 子关系 = 全局基础数据集.查询源关系(
        概念节点, 基础外部关系类型::父子);
    if (!子关系.empty()) return 零引用概念清理结果::无须删除;

    const auto 父关系 = 全局基础数据集.查询目标关系(
        概念节点, 基础外部关系类型::父子);
    if (父关系.size() != 1) return 零引用概念清理结果::失败;
    std::vector<稳定编码> 分量节点组;
    for (const auto& 字段 : 全局基础数据集.查询字段(
        概念节点, 结构化分量字段)) {
        const auto* 分量节点 = std::get_if<稳定编码>(&字段.内容);
        if (!分量节点) return 零引用概念清理结果::失败;
        分量节点组.push_back(*分量节点);
    }

    const std::vector<稳定编码> 概念字段类型{
        节点类型字段,
        材料类型字段,
        I64值域字段,
        全材料值域字段,
        非I64值域字段,
        存在引用值域字段,
        结构化分量字段,
        单位字段,
        比较规则字段,
        聚合规则字段,
        名称关系字段,
        当前引用实例数字段};
    std::vector<稳定编码> 概念字段关系;
    for (const auto 字段类型 : 概念字段类型) {
        for (const auto& 字段 : 全局基础数据集.查询字段(
            概念节点, 字段类型)) {
            概念字段关系.push_back(字段.编码);
        }
    }

    struct 分量清理材料 final {
        稳定编码 节点;
        std::vector<稳定编码> 字段关系;
    };
    std::vector<分量清理材料> 分量材料;
    const std::vector<稳定编码> 分量字段类型{
        节点类型字段,
        分量顺序字段,
        分量角色字段,
        分量材料类型字段,
        分量I64值域字段,
        分量单位字段};
    for (const auto 分量节点 : 分量节点组) {
        分量清理材料 材料{分量节点, {}};
        for (const auto 字段类型 : 分量字段类型) {
            for (const auto& 字段 : 全局基础数据集.查询字段(
                分量节点, 字段类型)) {
                材料.字段关系.push_back(字段.编码);
            }
        }
        分量材料.push_back(std::move(材料));
    }

    auto 兄弟关系 = 全局基础数据集.查询源关系(
        概念节点, 基础外部关系类型::兄弟);
    const auto 指向本节点的兄弟关系 = 全局基础数据集.查询目标关系(
        概念节点, 基础外部关系类型::兄弟);
    兄弟关系.insert(
        兄弟关系.end(), 指向本节点的兄弟关系.begin(), 指向本节点的兄弟关系.end());
    std::sort(兄弟关系.begin(), 兄弟关系.end(), [](const auto& 左, const auto& 右) {
        return 左.编码.值 < 右.编码.值;
    });
    兄弟关系.erase(std::unique(
        兄弟关系.begin(), 兄弟关系.end(), [](const auto& 左, const auto& 右) {
            return 左.编码 == 右.编码;
        }), 兄弟关系.end());

    if (!全局基础数据集.删除关系(父关系.front().编码)) {
        return 零引用概念清理结果::失败;
    }
    for (const auto& 关系 : 兄弟关系) {
        if (!全局基础数据集.删除关系(关系.编码)) {
            return 零引用概念清理结果::失败;
        }
    }
    for (const auto 字段关系 : 概念字段关系) {
        if (!全局基础数据集.删除字段(字段关系)) {
            return 零引用概念清理结果::失败;
        }
    }
    for (const auto& 分量 : 分量材料) {
        for (const auto 字段关系 : 分量.字段关系) {
            if (!全局基础数据集.删除字段(字段关系)) {
                return 零引用概念清理结果::失败;
            }
        }
        if (!全局基础数据集.删除节点(分量.节点)) {
            return 零引用概念清理结果::失败;
        }
    }
    if (!全局基础数据集.删除节点(概念节点)) {
        return 零引用概念清理结果::失败;
    }
    if (先天概念.三维空间坐标 == 概念节点) 先天概念.三维空间坐标 = {};
    if (先天概念.三维空间尺寸 == 概念节点) 先天概念.三维空间尺寸 = {};
    if (先天概念.RGB颜色 == 概念节点) 先天概念.RGB颜色 = {};
    if (先天概念.二维轮廓 == 概念节点) 先天概念.二维轮廓 = {};
    if (先天概念.三维体素 == 概念节点) 先天概念.三维体素 = {};
    return 零引用概念清理结果::已删除;
}

bool 是I64类型(特征概念材料物理类型 类型) noexcept {
    return 类型 == 特征概念材料物理类型::I64标量;
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
    case 特征概念材料物理类型::存在节点引用:
        return std::nullopt;
    }
    return std::nullopt;
}

std::uint64_t 计算I64绝对差(std::int64_t 左值, std::int64_t 右值) noexcept {
    constexpr auto 符号位 = std::uint64_t{1} << 63;
    const auto 左有序值 = static_cast<std::uint64_t>(左值) ^ 符号位;
    const auto 右有序值 = static_cast<std::uint64_t>(右值) ^ 符号位;
    return 左有序值 >= 右有序值
        ? 左有序值 - 右有序值 : 右有序值 - 左有序值;
}

bool 相邻或重叠(
    const 特征概念I64闭区间& 左, const 特征概念I64闭区间& 右) noexcept {
    if (右.下界 <= 左.上界) return true;
    return 左.上界 != (std::numeric_limits<std::int64_t>::max)()
        && 右.下界 == 左.上界 + 1;
}

} // namespace

bool 特征概念比较结果::成功() const noexcept {
    return 状态 == 特征概念规则状态::已完成 && 相等.has_value();
}

bool 特征概念聚合结果::成功() const noexcept {
    return (状态 == 特征概念规则状态::已完成
        || 状态 == 特征概念规则状态::无变化)
        && 值域.has_value();
}

bool 先天特征概念集合::完整() const noexcept {
    return 有效(毫米单位) && 有效(三维空间坐标)
        && 有效(三维空间尺寸) && 有效(RGB颜色)
        && 有效(二维轮廓) && 有效(三维体素);
}

void 概念_特征类::装配存在引用读取接口(
    const 特征存在引用读取接口& 读取接口) noexcept {
    std::lock_guard 锁(特征概念树互斥);
    存在引用读取接口_ = &读取接口;
}

bool 概念_特征类::初始化(const 新_特征值类& 特征值服务) noexcept {
    try {
        std::lock_guard 锁(特征概念树互斥);
        if (结构已初始化() && !结构仍存在()) return false;
        if (有效(特征概念树) && !节点仍存在(特征概念树)) return false;
        if (!有效(特征概念树)) {
            特征概念树 = 全局基础数据集.新建节点();
            if (!有效(特征概念树)) return false;
        }

        auto 建立 = [](稳定编码& 节点) {
            if (有效(节点)) return 节点仍存在(节点);
            节点 = 新建结构节点();
            return 有效(节点);
        };
        if (!(建立(节点类型字段)
            && 建立(特征概念节点类型)
            && 建立(材料类型字段)
            && 建立(I64值域字段)
            && 建立(全材料值域字段)
            && 建立(非I64值域字段)
            && 建立(存在引用值域字段)
            && 建立(结构化分量字段)
            && 建立(结构化分量节点类型)
            && 建立(分量顺序字段)
            && 建立(分量角色字段)
            && 建立(分量材料类型字段)
            && 建立(分量I64值域字段)
            && 建立(分量单位字段)
            && 建立(单位字段)
            && 建立(比较规则字段)
            && 建立(聚合规则字段)
            && 建立(名称关系字段)
            && 建立(当前引用实例数字段))) {
            return false;
        }

        if (!(建立(先天概念.毫米单位)
            && 建立(三维坐标X角色)
            && 建立(三维坐标Y角色)
            && 建立(三维坐标Z角色)
            && 建立(三维尺寸X跨度角色)
            && 建立(三维尺寸Y跨度角色)
            && 建立(三维尺寸Z跨度角色)
            && 建立(RGB红色角色)
            && 建立(RGB绿色角色)
            && 建立(RGB蓝色角色))) {
            return false;
        }

        const auto I64最小 = (std::numeric_limits<std::int64_t>::min)();
        const auto I64最大 = (std::numeric_limits<std::int64_t>::max)();
        const std::vector<特征概念I64闭区间> 有符号全域{{I64最小, I64最大}};
        const std::vector<特征概念I64闭区间> 非负全域{{0, I64最大}};
        const std::vector<特征概念I64闭区间> 颜色分量域{{0, 255}};

        const auto 结构化定义 = [](
            std::vector<特征概念I64分量定义> 分量) {
            特征概念定义 定义;
            定义.材料类型 = 特征概念材料物理类型::I64数组;
            定义.值域 = 特征概念结构化I64值域{std::move(分量)};
            定义.比较规则 = 特征概念比较规则::完整材料精确;
            定义.聚合规则 = 特征概念聚合规则::完整材料精确集合;
            return 定义;
        };
        const auto 全材料定义 = [](特征概念材料物理类型 材料类型) {
            特征概念定义 定义;
            定义.材料类型 = 材料类型;
            定义.值域 = 特征概念全材料值域{};
            定义.比较规则 = 特征概念比较规则::完整材料精确;
            定义.聚合规则 = 特征概念聚合规则::完整材料精确集合;
            return 定义;
        };
        const auto 确保根概念 = [&](稳定编码& 节点, const 特征概念定义& 定义) {
            if (有效(节点)) {
                const auto 信息 = 获取特征概念(节点);
                const auto 规范定义 = 规范化定义(定义);
                if (!信息 || !规范定义
                    || !结构定义相同(转为定义(*信息), *规范定义)) {
                    return false;
                }
                const auto 上位 = 全局基础数据集.查询目标关系(
                    节点, 基础外部关系类型::父子);
                return 上位.size() == 1 && 上位.front().源节点 == 特征概念树;
            }
            节点 = 建立或取得特征概念(定义, 特征值服务);
            return 有效(节点);
        };

        const auto 三维坐标定义 = 结构化定义({
            {三维坐标X角色, 先天概念.毫米单位, 有符号全域},
            {三维坐标Y角色, 先天概念.毫米单位, 有符号全域},
            {三维坐标Z角色, 先天概念.毫米单位, 有符号全域}});
        const auto 三维尺寸定义 = 结构化定义({
            {三维尺寸X跨度角色, 先天概念.毫米单位, 非负全域},
            {三维尺寸Y跨度角色, 先天概念.毫米单位, 非负全域},
            {三维尺寸Z跨度角色, 先天概念.毫米单位, 非负全域}});
        const auto RGB定义 = 结构化定义({
            {RGB红色角色, std::nullopt, 颜色分量域},
            {RGB绿色角色, std::nullopt, 颜色分量域},
            {RGB蓝色角色, std::nullopt, 颜色分量域}});

        return 确保根概念(先天概念.三维空间坐标, 三维坐标定义)
            && 确保根概念(先天概念.三维空间尺寸, 三维尺寸定义)
            && 确保根概念(先天概念.RGB颜色, RGB定义)
            && 确保根概念(先天概念.二维轮廓,
                全材料定义(特征概念材料物理类型::二维二值格))
            && 确保根概念(先天概念.三维体素,
                全材料定义(特征概念材料物理类型::三维二值格));
    } catch (...) {
        return false;
    }
}

std::optional<先天特征概念集合> 概念_特征类::获取先天特征概念() const noexcept {
    try {
        std::lock_guard 锁(特征概念树互斥);
        if (!结构仍存在() || !先天概念.完整()
            || !节点仍存在(先天概念.毫米单位)
            || !是特征概念节点(先天概念.三维空间坐标)
            || !是特征概念节点(先天概念.三维空间尺寸)
            || !是特征概念节点(先天概念.RGB颜色)
            || !是特征概念节点(先天概念.二维轮廓)
            || !是特征概念节点(先天概念.三维体素)) {
            return std::nullopt;
        }
        return 先天概念;
    } catch (...) {
        return std::nullopt;
    }
}

稳定编码 概念_特征类::建立或取得特征概念(
    const 特征概念定义& 定义,
    const 新_特征值类& 特征值服务,
    std::optional<稳定编码> 上位概念) noexcept {
    try {
        std::lock_guard 锁(特征概念树互斥);
        if (!结构仍存在()) return {};
        const auto 规范定义 = 规范化定义(定义);
        if (!规范定义) return {};
        if (const auto* 引用域 =
            std::get_if<特征概念存在引用值域>(&规范定义->值域)) {
            if (!存在引用读取接口_
                || !存在引用读取接口_->是存在概念节点(
                    引用域->存在概念节点)) {
                return {};
            }
        }

        const auto 父节点 = 上位概念.value_or(特征概念树);
        if (父节点 != 特征概念树 && !是特征概念节点(父节点)) return {};
        if (父节点 == 特征概念树 && !节点仍存在(父节点)) return {};
        if (规范定义->单位 && !节点仍存在(*规范定义->单位)) return {};
        for (const auto 名称 : 规范定义->名称关系) {
            if (!节点仍存在(名称)) return {};
        }

        if (const auto* 值节点组 = std::get_if<std::vector<稳定编码>>(&规范定义->值域)) {
            const auto 期望类型 = 转换特征值材料类型(规范定义->材料类型);
            if (!期望类型) return {};
            for (const auto 值节点 : *值节点组) {
                const auto 值信息 = 特征值服务.获取特征值(值节点);
                if (!值信息 || 值信息->物理类型 != *期望类型) return {};
            }
        }
        if (const auto* 结构化 =
            std::get_if<特征概念结构化I64值域>(&规范定义->值域)) {
            for (const auto& 分量 : 结构化->分量) {
                if (!节点仍存在(分量.角色)
                    || (分量.单位 && !节点仍存在(*分量.单位))) {
                    return {};
                }
            }
        }

        if (父节点 != 特征概念树) {
            const auto 父信息 = 获取特征概念(父节点);
            if (!父信息) return {};
            const auto 父定义 = 规范化定义(转为定义(*父信息));
            bool 是合法子域 = 父定义
                && (是严格子值域(*父定义, *规范定义)
                    || 有限材料节点受定义包含(
                        *父定义, *规范定义, 特征值服务));
            if (父定义
                && 父定义->材料类型 == 特征概念材料物理类型::存在节点引用
                && 规范定义->材料类型 == 特征概念材料物理类型::存在节点引用) {
                const auto* 父域 =
                    std::get_if<特征概念存在引用值域>(&父定义->值域);
                const auto* 子域 =
                    std::get_if<特征概念存在引用值域>(&规范定义->值域);
                是合法子域 = 存在引用读取接口_ && 父域 && 子域
                    && 父域->存在概念节点 != 子域->存在概念节点
                    && 存在引用读取接口_->存在概念包含(
                        父域->存在概念节点, 子域->存在概念节点);
            }
            if (!是合法子域) {
                return {};
            }
        }

        // 根链节点自身就是不同特征类型的身份。即使物理材料、单位和值域
        // 完全相同，也不能把X/Y/Z坐标等不同根链类型合并成同一节点。
        if (父节点 != 特征概念树) {
            std::vector<稳定编码> 同定义直接子节点;
            for (const auto& 关系 : 全局基础数据集.查询源关系(
                父节点, 基础外部关系类型::父子)) {
                const auto 子信息 = 获取特征概念(关系.目标节点);
                if (!子信息) continue;
                const auto 子定义 = 规范化定义(转为定义(*子信息));
                if (子定义 && 结构定义相同(*子定义, *规范定义)) {
                    同定义直接子节点.push_back(关系.目标节点);
                }
            }
            std::sort(同定义直接子节点.begin(), 同定义直接子节点.end());
            同定义直接子节点.erase(
                std::unique(同定义直接子节点.begin(), 同定义直接子节点.end()),
                同定义直接子节点.end());
            if (同定义直接子节点.size() > 1) return {};
            if (同定义直接子节点.size() == 1) {
                const auto 节点 = 同定义直接子节点.front();
                const auto 已有信息 = 获取特征概念(节点);
                if (!已有信息) return {};
                std::vector<稳定编码> 本次新增名称字段;
                for (const auto 名称 : 规范定义->名称关系) {
                    if (std::find(已有信息->名称关系.begin(),
                        已有信息->名称关系.end(), 名称)
                        != 已有信息->名称关系.end()) {
                        continue;
                    }
                    const auto 字段关系 = 全局基础数据集.添加字段节点(
                        节点, 名称关系字段, 名称);
                    if (!有效(字段关系)) {
                        for (auto 位置 = 本次新增名称字段.rbegin();
                            位置 != 本次新增名称字段.rend(); ++位置) {
                            全局基础数据集.删除字段(*位置);
                        }
                        return {};
                    }
                    本次新增名称字段.push_back(字段关系);
                }
                return 节点;
            }
        }

        const auto 节点 = 全局基础数据集.新建节点();
        if (!有效(节点)) return {};
        std::vector<稳定编码> 已写字段;
        struct 已建分量节点 final {
            稳定编码 节点;
            std::vector<稳定编码> 字段;
        };
        std::vector<已建分量节点> 已建分量;
        const auto 清理本次节点 = [&]() noexcept {
            回滚字段和节点(节点, 已写字段);
            for (auto 分量位置 = 已建分量.rbegin();
                分量位置 != 已建分量.rend(); ++分量位置) {
                回滚字段和节点(分量位置->节点, 分量位置->字段);
            }
        };
        auto 写节点字段 = [&](稳定编码 字段, 稳定编码 目标) {
            const auto 关系 = 全局基础数据集.添加字段节点(节点, 字段, 目标);
            if (有效(关系)) 已写字段.push_back(关系);
            return 有效(关系);
        };
        auto 写I64字段 = [&](稳定编码 字段, std::int64_t 值) {
            const auto 关系 = 全局基础数据集.添加字段值(
                节点, 字段, 基础值{基础原始值{值}});
            if (有效(关系)) 已写字段.push_back(关系);
            return 有效(关系);
        };

        bool 完成 = 写节点字段(节点类型字段, 特征概念节点类型)
            && 写I64字段(材料类型字段, static_cast<std::int64_t>(规范定义->材料类型));

        if (完成) {
            if (const auto* 区间组 =
                std::get_if<std::vector<特征概念I64闭区间>>(&规范定义->值域)) {
                std::vector<std::int64_t> 展平值域;
                展平值域.reserve(区间组->size() * 2);
                for (const auto& 区间 : *区间组) {
                    展平值域.push_back(区间.下界);
                    展平值域.push_back(区间.上界);
                }
                const auto 关系 = 全局基础数据集.添加字段值(
                    节点, I64值域字段, 基础值{基础原始值{std::move(展平值域)}});
                if (有效(关系)) 已写字段.push_back(关系);
                完成 = 有效(关系);
            } else if (std::holds_alternative<特征概念全材料值域>(规范定义->值域)) {
                const auto 关系 = 全局基础数据集.添加字段值(
                    节点, 全材料值域字段, 基础值{基础原始值{std::int64_t{1}}});
                if (有效(关系)) 已写字段.push_back(关系);
                完成 = 有效(关系);
            } else if (const auto* 值节点组 =
                std::get_if<std::vector<稳定编码>>(&规范定义->值域)) {
                for (const auto 值节点 : *值节点组) {
                    if (!写节点字段(非I64值域字段, 值节点)) {
                        完成 = false;
                        break;
                    }
                }
            } else if (const auto* 引用域 =
                std::get_if<特征概念存在引用值域>(&规范定义->值域)) {
                完成 = 写节点字段(
                    存在引用值域字段, 引用域->存在概念节点);
            } else {
                const auto& 结构化 =
                    std::get<特征概念结构化I64值域>(规范定义->值域);
                for (std::size_t 顺序 = 0; 顺序 < 结构化.分量.size(); ++顺序) {
                    const auto& 分量定义 = 结构化.分量[顺序];
                    已建分量.push_back({全局基础数据集.新建节点(), {}});
                    auto& 分量 = 已建分量.back();
                    if (!有效(分量.节点)) {
                        已建分量.pop_back();
                        完成 = false;
                        break;
                    }
                    const auto 写分量节点字段 = [&](稳定编码 字段, 稳定编码 目标) {
                        const auto 关系 = 全局基础数据集.添加字段节点(
                            分量.节点, 字段, 目标);
                        if (有效(关系)) 分量.字段.push_back(关系);
                        return 有效(关系);
                    };
                    const auto 写分量I64字段 = [&](稳定编码 字段, std::int64_t 值) {
                        const auto 关系 = 全局基础数据集.添加字段值(
                            分量.节点, 字段, 基础值{基础原始值{值}});
                        if (有效(关系)) 分量.字段.push_back(关系);
                        return 有效(关系);
                    };
                    std::vector<std::int64_t> 展平分量域;
                    展平分量域.reserve(分量定义.值域.size() * 2);
                    for (const auto& 区间 : 分量定义.值域) {
                        展平分量域.push_back(区间.下界);
                        展平分量域.push_back(区间.上界);
                    }
                    完成 = 写分量节点字段(节点类型字段, 结构化分量节点类型)
                        && 写分量I64字段(分量顺序字段,
                            static_cast<std::int64_t>(顺序))
                        && 写分量节点字段(分量角色字段, 分量定义.角色)
                        && 写分量I64字段(分量材料类型字段,
                            static_cast<std::int64_t>(
                                特征概念材料物理类型::I64标量));
                    if (完成) {
                        const auto 域关系 = 全局基础数据集.添加字段值(
                            分量.节点, 分量I64值域字段,
                            基础值{基础原始值{std::move(展平分量域)}});
                        if (有效(域关系)) 分量.字段.push_back(域关系);
                        完成 = 有效(域关系);
                    }
                    if (完成 && 分量定义.单位) {
                        完成 = 写分量节点字段(
                            分量单位字段, *分量定义.单位);
                    }
                    if (完成) {
                        完成 = 写节点字段(结构化分量字段, 分量.节点);
                    }
                    if (!完成) break;
                }
            }
        }
        if (完成 && 规范定义->单位) {
            完成 = 写节点字段(单位字段, *规范定义->单位);
        }
        if (完成) {
            完成 = 写I64字段(比较规则字段,
                    static_cast<std::int64_t>(规范定义->比较规则))
                && 写I64字段(聚合规则字段,
                    static_cast<std::int64_t>(规范定义->聚合规则))
                && 写I64字段(当前引用实例数字段, 0);
        }
        if (完成) {
            for (const auto 名称 : 规范定义->名称关系) {
                if (!写节点字段(名称关系字段, 名称)) {
                    完成 = false;
                    break;
                }
            }
        }
        if (!完成) {
            清理本次节点();
            return {};
        }

        if (!有效(全局基础数据集.添加关系(
            父节点, 节点, 基础外部关系类型::父子))) {
            清理本次节点();
            return {};
        }
        return 节点;
    } catch (...) {
        return {};
    }
}

std::optional<特征概念信息> 概念_特征类::获取特征概念(
    稳定编码 特征概念节点) const noexcept {
    try {
        std::lock_guard 锁(特征概念树互斥);
        if (!结构仍存在() || !节点仍存在(特征概念节点)) return std::nullopt;
        const auto 类型 = 读取唯一节点字段(特征概念节点, 节点类型字段);
        if (!类型 || *类型 != 特征概念节点类型) return std::nullopt;

        const auto 材料 = 读取唯一I64字段(特征概念节点, 材料类型字段);
        const auto 比较 = 读取唯一I64字段(特征概念节点, 比较规则字段);
        const auto 聚合 = 读取唯一I64字段(特征概念节点, 聚合规则字段);
        if (!材料 || !比较 || !聚合) return std::nullopt;

        特征概念定义 定义;
        定义.材料类型 = static_cast<特征概念材料物理类型>(*材料);
        定义.比较规则 = static_cast<特征概念比较规则>(*比较);
        定义.聚合规则 = static_cast<特征概念聚合规则>(*聚合);
        const auto I64字段组 = 全局基础数据集.查询字段(
            特征概念节点, I64值域字段);
        const auto 全材料字段组 = 全局基础数据集.查询字段(
            特征概念节点, 全材料值域字段);
        const auto 非I64字段组 = 全局基础数据集.查询字段(
            特征概念节点, 非I64值域字段);
        const auto 结构化字段组 = 全局基础数据集.查询字段(
            特征概念节点, 结构化分量字段);
        const auto 存在引用字段组 = 全局基础数据集.查询字段(
            特征概念节点, 存在引用值域字段);
        const auto 表示数量 = static_cast<int>(!I64字段组.empty())
            + static_cast<int>(!全材料字段组.empty())
            + static_cast<int>(!非I64字段组.empty())
            + static_cast<int>(!结构化字段组.empty())
            + static_cast<int>(!存在引用字段组.empty());
        if (表示数量 != 1) return std::nullopt;

        if (!I64字段组.empty()) {
            const auto 展平值域 = 读取唯一I64组字段(特征概念节点, I64值域字段);
            if (!展平值域 || 展平值域->empty() || 展平值域->size() % 2 != 0) {
                return std::nullopt;
            }
            std::vector<特征概念I64闭区间> 区间组;
            for (std::size_t i = 0; i < 展平值域->size(); i += 2) {
                区间组.push_back({(*展平值域)[i], (*展平值域)[i + 1]});
            }
            定义.值域 = std::move(区间组);
        } else if (!全材料字段组.empty()) {
            if (全材料字段组.size() != 1) return std::nullopt;
            const auto* 标记值 = std::get_if<基础值>(&全材料字段组.front().内容);
            const auto* 标记 = 标记值
                ? std::get_if<std::int64_t>(&标记值->材料) : nullptr;
            if (!标记 || *标记 != 1) return std::nullopt;
            定义.值域 = 特征概念全材料值域{};
        } else if (!非I64字段组.empty()) {
            auto 值域 = 读取多节点字段(特征概念节点, 非I64值域字段);
            if (值域.empty() || 值域.size() != 非I64字段组.size()) {
                return std::nullopt;
            }
            定义.值域 = std::move(值域);
        } else if (!存在引用字段组.empty()) {
            if (存在引用字段组.size() != 1) return std::nullopt;
            const auto* 存在概念 =
                std::get_if<稳定编码>(&存在引用字段组.front().内容);
            if (!存在概念 || !节点仍存在(*存在概念)) return std::nullopt;
            定义.值域 = 特征概念存在引用值域{*存在概念};
        } else {
            std::vector<std::pair<std::size_t, 特征概念I64分量定义>> 有序分量;
            有序分量.reserve(结构化字段组.size());
            for (const auto& 分量字段 : 结构化字段组) {
                const auto* 分量节点 = std::get_if<稳定编码>(&分量字段.内容);
                if (!分量节点 || !节点仍存在(*分量节点)) return std::nullopt;
                const auto 分量类型 = 读取唯一节点字段(*分量节点, 节点类型字段);
                const auto 顺序 = 读取唯一I64字段(*分量节点, 分量顺序字段);
                const auto 角色 = 读取唯一节点字段(*分量节点, 分量角色字段);
                const auto 分量材料 = 读取唯一I64字段(
                    *分量节点, 分量材料类型字段);
                const auto 展平分量域 = 读取唯一I64组字段(
                    *分量节点, 分量I64值域字段);
                if (!分量类型 || *分量类型 != 结构化分量节点类型
                    || !顺序 || *顺序 < 0 || !角色 || !分量材料
                    || *分量材料 != static_cast<std::int64_t>(
                        特征概念材料物理类型::I64标量)
                    || !展平分量域 || 展平分量域->empty()
                    || 展平分量域->size() % 2 != 0) {
                    return std::nullopt;
                }
                特征概念I64分量定义 分量;
                分量.角色 = *角色;
                for (std::size_t i = 0; i < 展平分量域->size(); i += 2) {
                    分量.值域.push_back({
                        (*展平分量域)[i], (*展平分量域)[i + 1]});
                }
                const auto 分量单位组 = 全局基础数据集.查询字段(
                    *分量节点, 分量单位字段);
                if (分量单位组.size() > 1) return std::nullopt;
                if (!分量单位组.empty()) {
                    const auto* 单位节点 =
                        std::get_if<稳定编码>(&分量单位组.front().内容);
                    if (!单位节点) return std::nullopt;
                    分量.单位 = *单位节点;
                }
                有序分量.emplace_back(static_cast<std::size_t>(*顺序), std::move(分量));
            }
            std::sort(有序分量.begin(), 有序分量.end(),
                [](const auto& 左, const auto& 右) { return 左.first < 右.first; });
            特征概念结构化I64值域 结构化;
            结构化.分量.reserve(有序分量.size());
            for (std::size_t i = 0; i < 有序分量.size(); ++i) {
                if (有序分量[i].first != i) return std::nullopt;
                结构化.分量.push_back(std::move(有序分量[i].second));
            }
            定义.值域 = std::move(结构化);
        }

        const auto 单位组 = 全局基础数据集.查询字段(特征概念节点, 单位字段);
        if (单位组.size() > 1) return std::nullopt;
        if (!单位组.empty()) {
            const auto* 单位 = std::get_if<稳定编码>(&单位组.front().内容);
            if (!单位) return std::nullopt;
            定义.单位 = *单位;
        }
        定义.名称关系 = 读取多节点字段(特征概念节点, 名称关系字段);
        if (定义.名称关系.size()
            != 全局基础数据集.查询字段(特征概念节点, 名称关系字段).size()) {
            return std::nullopt;
        }
        const auto 当前引用实例数 = 读取唯一I64字段(
            特征概念节点, 当前引用实例数字段);
        if (!当前引用实例数 || *当前引用实例数 < 0) return std::nullopt;

        const auto 规范定义 = 规范化定义(定义);
        if (!规范定义) return std::nullopt;
        return 特征概念信息{
            特征概念节点,
            规范定义->材料类型,
            规范定义->值域,
            规范定义->单位,
            规范定义->比较规则,
            规范定义->聚合规则,
            规范定义->名称关系,
            *当前引用实例数};
    } catch (...) {
        return std::nullopt;
    }
}

bool 概念_特征类::更新特征概念值域(
    稳定编码 特征概念节点,
    const 特征概念值域& 新值域,
    const 新_特征值类& 特征值服务) noexcept {
    try {
        std::lock_guard 锁(特征概念树互斥);
        const auto 原信息 = 获取特征概念(特征概念节点);
        if (!原信息) return false;

        auto 新定义 = 转为定义(*原信息);
        新定义.值域 = 新值域;
        const auto 规范新定义 = 规范化定义(新定义);
        const auto 规范原定义 = 规范化定义(转为定义(*原信息));
        if (!规范新定义 || !规范原定义) return false;
        if (const auto* 引用域 =
            std::get_if<特征概念存在引用值域>(&规范新定义->值域)) {
            if (!存在引用读取接口_
                || !存在引用读取接口_->是存在概念节点(
                    引用域->存在概念节点)) {
                return false;
            }
        }
        if (规范新定义->值域 == 规范原定义->值域) return true;

        if (const auto* 值节点组 =
            std::get_if<std::vector<稳定编码>>(&规范新定义->值域)) {
            const auto 期望类型 = 转换特征值材料类型(
                规范新定义->材料类型);
            if (!期望类型) return false;
            for (const auto 值节点 : *值节点组) {
                const auto 值信息 = 特征值服务.获取特征值(值节点);
                if (!值信息 || 值信息->物理类型 != *期望类型) {
                    return false;
                }
            }
        }

        const auto 父关系 = 全局基础数据集.查询目标关系(
            特征概念节点, 基础外部关系类型::父子);
        if (父关系.size() != 1) return false;
        if (父关系.front().源节点 != 特征概念树) {
            const auto 父信息 = 获取特征概念(父关系.front().源节点);
            const auto 父定义 = 父信息
                ? 规范化定义(转为定义(*父信息)) : std::nullopt;
            bool 合法 = 父定义
                && (是严格子值域(*父定义, *规范新定义)
                    || 有限材料节点受定义包含(
                        *父定义, *规范新定义, 特征值服务));
            if (父定义
                && 父定义->材料类型 == 特征概念材料物理类型::存在节点引用
                && 规范新定义->材料类型 == 特征概念材料物理类型::存在节点引用) {
                const auto* 父域 =
                    std::get_if<特征概念存在引用值域>(&父定义->值域);
                const auto* 子域 =
                    std::get_if<特征概念存在引用值域>(&规范新定义->值域);
                合法 = 存在引用读取接口_ && 父域 && 子域
                    && 父域->存在概念节点 != 子域->存在概念节点
                    && 存在引用读取接口_->存在概念包含(
                        父域->存在概念节点, 子域->存在概念节点);
            }
            if (!合法) {
                return false;
            }
        }
        for (const auto& 子关系 : 全局基础数据集.查询源关系(
            特征概念节点, 基础外部关系类型::父子)) {
            const auto 子信息 = 获取特征概念(子关系.目标节点);
            const auto 子定义 = 子信息
                ? 规范化定义(转为定义(*子信息)) : std::nullopt;
            bool 合法 = 子定义
                && (是严格子值域(*规范新定义, *子定义)
                    || 有限材料节点受定义包含(
                        *规范新定义, *子定义, 特征值服务));
            if (子定义
                && 规范新定义->材料类型 == 特征概念材料物理类型::存在节点引用
                && 子定义->材料类型 == 特征概念材料物理类型::存在节点引用) {
                const auto* 父域 =
                    std::get_if<特征概念存在引用值域>(&规范新定义->值域);
                const auto* 子域 =
                    std::get_if<特征概念存在引用值域>(&子定义->值域);
                合法 = 存在引用读取接口_ && 父域 && 子域
                    && 父域->存在概念节点 != 子域->存在概念节点
                    && 存在引用读取接口_->存在概念包含(
                        父域->存在概念节点, 子域->存在概念节点);
            }
            if (!合法) {
                return false;
            }
        }

        if (const auto* 新引用域 =
            std::get_if<特征概念存在引用值域>(&规范新定义->值域)) {
            const auto* 原引用域 =
                std::get_if<特征概念存在引用值域>(&规范原定义->值域);
            const auto 字段组 = 全局基础数据集.查询字段(
                特征概念节点, 存在引用值域字段);
            if (!原引用域 || 字段组.size() != 1) return false;
            if (!全局基础数据集.修改字段节点(
                字段组.front().编码, 新引用域->存在概念节点)) {
                return false;
            }
            const auto 读回 = 获取特征概念(特征概念节点);
            if (读回 && 读回->值域 == 规范新定义->值域) return true;
            (void)全局基础数据集.修改字段节点(
                字段组.front().编码, 原引用域->存在概念节点);
            return false;
        }

        if (const auto* 新区间组 =
            std::get_if<std::vector<特征概念I64闭区间>>(
                &规范新定义->值域)) {
            const auto* 原区间组 =
                std::get_if<std::vector<特征概念I64闭区间>>(
                    &规范原定义->值域);
            const auto 字段组 = 全局基础数据集.查询字段(
                特征概念节点, I64值域字段);
            if (!原区间组 || 字段组.size() != 1) return false;
            std::vector<std::int64_t> 新展平;
            新展平.reserve(新区间组->size() * 2);
            for (const auto& 区间 : *新区间组) {
                新展平.push_back(区间.下界);
                新展平.push_back(区间.上界);
            }
            std::vector<std::int64_t> 原展平;
            原展平.reserve(原区间组->size() * 2);
            for (const auto& 区间 : *原区间组) {
                原展平.push_back(区间.下界);
                原展平.push_back(区间.上界);
            }
            if (!全局基础数据集.修改字段值(
                字段组.front().编码,
                基础值{基础原始值{std::move(新展平)}})) {
                return false;
            }
            const auto 读回 = 获取特征概念(特征概念节点);
            if (读回 && 读回->值域 == 规范新定义->值域) return true;
            (void)全局基础数据集.修改字段值(
                字段组.front().编码,
                基础值{基础原始值{std::move(原展平)}});
            return false;
        }

        const auto* 新节点组 =
            std::get_if<std::vector<稳定编码>>(&规范新定义->值域);
        const auto* 原节点组 =
            std::get_if<std::vector<稳定编码>>(&规范原定义->值域);
        if (!新节点组 || !原节点组) return false;

        const auto 原字段组 = 全局基础数据集.查询字段(
            特征概念节点, 非I64值域字段);
        if (原字段组.size() != 原节点组->size()) return false;
        std::vector<稳定编码> 新增字段;
        for (const auto 节点 : *新节点组) {
            if (std::binary_search(原节点组->begin(), 原节点组->end(), 节点)) {
                continue;
            }
            const auto 关系 = 全局基础数据集.添加字段节点(
                特征概念节点, 非I64值域字段, 节点);
            if (!有效(关系)) {
                for (const auto 已增 : 新增字段) {
                    (void)全局基础数据集.删除字段(已增);
                }
                return false;
            }
            新增字段.push_back(关系);
        }

        std::vector<稳定编码> 已删原节点;
        for (const auto& 字段 : 原字段组) {
            const auto* 节点 = std::get_if<稳定编码>(&字段.内容);
            if (!节点) return false;
            if (std::binary_search(新节点组->begin(), 新节点组->end(), *节点)) {
                continue;
            }
            if (!全局基础数据集.删除字段(字段.编码)) {
                for (const auto 已删节点 : 已删原节点) {
                    (void)全局基础数据集.添加字段节点(
                        特征概念节点, 非I64值域字段, 已删节点);
                }
                for (const auto 已增 : 新增字段) {
                    (void)全局基础数据集.删除字段(已增);
                }
                return false;
            }
            已删原节点.push_back(*节点);
        }

        const auto 读回 = 获取特征概念(特征概念节点);
        return 读回 && 读回->值域 == 规范新定义->值域;
    } catch (...) {
        return false;
    }
}

bool 概念_特征类::增加当前实例引用(
    稳定编码 特征概念节点) noexcept {
    try {
        std::lock_guard 锁(特征概念树互斥);
        if (!结构仍存在() || !是特征概念节点(特征概念节点)) return false;
        const auto 字段组 = 全局基础数据集.查询字段(
            特征概念节点, 当前引用实例数字段);
        const auto 当前数量 = 读取唯一I64字段(
            特征概念节点, 当前引用实例数字段);
        if (字段组.size() != 1 || !当前数量 || *当前数量 < 0
            || *当前数量 == (std::numeric_limits<std::int64_t>::max)()) {
            return false;
        }
        const auto 新数量 = *当前数量 + 1;
        if (!全局基础数据集.修改字段值(
            字段组.front().编码, 基础值{基础原始值{新数量}})) {
            return false;
        }
        const auto 读回 = 读取唯一I64字段(
            特征概念节点, 当前引用实例数字段);
        if (读回 && *读回 == 新数量) return true;
        (void)全局基础数据集.修改字段值(
            字段组.front().编码, 基础值{基础原始值{*当前数量}});
        return false;
    } catch (...) {
        return false;
    }
}

bool 概念_特征类::减少当前实例引用(
    稳定编码 特征概念节点) noexcept {
    try {
        std::lock_guard 锁(特征概念树互斥);
        if (!结构仍存在() || !是特征概念节点(特征概念节点)) return false;
        const auto 字段组 = 全局基础数据集.查询字段(
            特征概念节点, 当前引用实例数字段);
        const auto 当前数量 = 读取唯一I64字段(
            特征概念节点, 当前引用实例数字段);
        if (字段组.size() != 1 || !当前数量 || *当前数量 <= 0) return false;
        const auto 新数量 = *当前数量 - 1;
        if (!全局基础数据集.修改字段值(
            字段组.front().编码, 基础值{基础原始值{新数量}})) {
            return false;
        }
        const auto 读回 = 读取唯一I64字段(
            特征概念节点, 当前引用实例数字段);
        if (读回 && *读回 == 新数量) {
            if (新数量 != 0) return true;
            return 尝试删除零引用叶概念(特征概念节点)
                != 零引用概念清理结果::失败;
        }
        (void)全局基础数据集.修改字段值(
            字段组.front().编码, 基础值{基础原始值{*当前数量}});
        return false;
    } catch (...) {
        return false;
    }
}

std::vector<稳定编码> 概念_特征类::查询特征概念(
    const 特征概念定义& 定义) const noexcept {
    try {
        std::lock_guard 锁(特征概念树互斥);
        const auto 规范定义 = 规范化定义(定义);
        if (!结构仍存在() || !规范定义) return {};

        std::vector<稳定编码> 待查{特征概念树};
        std::vector<稳定编码> 已查;
        std::vector<稳定编码> 结果;
        while (!待查.empty()) {
            const auto 父节点 = 待查.back();
            待查.pop_back();
            if (std::find(已查.begin(), 已查.end(), 父节点) != 已查.end()) continue;
            已查.push_back(父节点);
            for (const auto& 关系 : 全局基础数据集.查询源关系(
                父节点, 基础外部关系类型::父子)) {
                const auto 子节点 = 关系.目标节点;
                待查.push_back(子节点);
                const auto 信息 = 获取特征概念(子节点);
                if (!信息) continue;
                const auto 候选 = 规范化定义(转为定义(*信息));
                if (候选 && 结构定义相同(*候选, *规范定义)) {
                    结果.push_back(子节点);
                }
            }
        }
        std::sort(结果.begin(), 结果.end());
        结果.erase(std::unique(结果.begin(), 结果.end()), 结果.end());
        return 结果;
    } catch (...) {
        return {};
    }
}

特征概念按值查找结果 概念_特征类::查找值对应的最具体概念(
    稳定编码 特征类型概念,
    const 特征概念准确值& 特征值,
    std::optional<稳定编码> 单位,
    const 新_特征值类& 特征值服务) const noexcept {
    特征概念按值查找结果 结果;
    try {
        std::lock_guard 锁(特征概念树互斥);
        if (!结构仍存在() || !是特征概念节点(特征类型概念)) return 结果;

        const auto 根链关系 = 全局基础数据集.查询目标关系(
            特征类型概念, 基础外部关系类型::父子);
        if (根链关系.size() != 1
            || 根链关系.front().源节点 != 特征概念树) {
            结果.状态 = 特征概念查找状态::结构不一致;
            return 结果;
        }

        const auto 根信息 = 获取特征概念(特征类型概念);
        if (!根信息) {
            结果.状态 = 特征概念查找状态::结构不一致;
            return 结果;
        }
        if (根信息->单位 != 单位) {
            结果.状态 = 特征概念查找状态::单位不相容;
            return 结果;
        }
        if (根信息->材料类型 == 特征概念材料物理类型::I64标量) {
            if (!std::holds_alternative<std::int64_t>(特征值)) {
                结果.状态 = 特征概念查找状态::材料类型不相容;
                return 结果;
            }
        } else if (根信息->材料类型
            == 特征概念材料物理类型::存在节点引用) {
            const auto* 值节点 = std::get_if<稳定编码>(&特征值);
            const auto* 引用域 =
                std::get_if<特征概念存在引用值域>(&根信息->值域);
            if (!存在引用读取接口_ || !值节点 || !引用域
                || !存在引用读取接口_->存在节点属于概念(
                    *值节点, 引用域->存在概念节点)) {
                结果.状态 = 特征概念查找状态::材料类型不相容;
                return 结果;
            }
        } else {
            const auto 期望类型 = 转换特征值材料类型(根信息->材料类型);
            const auto* 值节点 = std::get_if<稳定编码>(&特征值);
            const auto 值信息 = 值节点
                ? 特征值服务.获取特征值(*值节点) : std::nullopt;
            if (!期望类型 || !值信息 || 值信息->物理类型 != *期望类型) {
                结果.状态 = 特征概念查找状态::材料类型不相容;
                return 结果;
            }
        }

        struct 待查项 final {
            稳定编码 节点;
            std::vector<稳定编码> 路径;
        };
        struct 命中项 final {
            稳定编码 节点;
            std::uint64_t 尺度 = 0;
            std::vector<稳定编码> 路径;
        };
        std::vector<待查项> 待查{{特征类型概念, {特征类型概念}}};
        std::vector<稳定编码> 已查;
        std::vector<命中项> 命中组;

        while (!待查.empty()) {
            auto 当前 = std::move(待查.back());
            待查.pop_back();
            if (std::find(已查.begin(), 已查.end(), 当前.节点) != 已查.end()) {
                结果.状态 = 特征概念查找状态::结构不一致;
                return 结果;
            }
            已查.push_back(当前.节点);

            const auto 当前信息 = 获取特征概念(当前.节点);
            if (!当前信息 || 当前信息->材料类型 != 根信息->材料类型
                || 当前信息->单位 != 根信息->单位) {
                结果.状态 = 特征概念查找状态::结构不一致;
                return 结果;
            }
            const auto 当前尺度 = 值域匹配尺度(
                *当前信息, 特征值, 特征值服务, 存在引用读取接口_);
            if (!当前尺度) continue;
            命中组.push_back({
                当前.节点, *当前尺度, 当前.路径});

            const auto 当前定义 = 规范化定义(转为定义(*当前信息));
            if (!当前定义) {
                结果.状态 = 特征概念查找状态::结构不一致;
                return 结果;
            }
            for (const auto& 关系 : 全局基础数据集.查询源关系(
                当前.节点, 基础外部关系类型::父子)) {
                if (std::find(当前.路径.begin(), 当前.路径.end(), 关系.目标节点)
                    != 当前.路径.end()) {
                    结果.状态 = 特征概念查找状态::结构不一致;
                    return 结果;
                }
                const auto 子信息 = 获取特征概念(关系.目标节点);
                if (!子信息) {
                    结果.状态 = 特征概念查找状态::结构不一致;
                    return 结果;
                }
                const auto 子定义 = 规范化定义(转为定义(*子信息));
                bool 合法子域 = 子定义
                    && (是严格子值域(*当前定义, *子定义)
                        || 有限材料节点受定义包含(
                            *当前定义, *子定义, 特征值服务));
                if (子定义
                    && 当前定义->材料类型
                        == 特征概念材料物理类型::存在节点引用
                    && 子定义->材料类型
                        == 特征概念材料物理类型::存在节点引用) {
                    const auto* 父域 =
                        std::get_if<特征概念存在引用值域>(&当前定义->值域);
                    const auto* 子域 =
                        std::get_if<特征概念存在引用值域>(&子定义->值域);
                    合法子域 = 存在引用读取接口_ && 父域 && 子域
                        && 父域->存在概念节点 != 子域->存在概念节点
                        && 存在引用读取接口_->存在概念包含(
                            父域->存在概念节点, 子域->存在概念节点);
                }
                if (!合法子域) {
                    结果.状态 = 特征概念查找状态::结构不一致;
                    return 结果;
                }
                auto 子路径 = 当前.路径;
                子路径.push_back(关系.目标节点);
                待查.push_back({关系.目标节点, std::move(子路径)});
            }
        }

        if (命中组.empty()) {
            结果.状态 = 特征概念查找状态::未找到;
            return 结果;
        }
        const auto 最佳尺度 = std::min_element(
            命中组.begin(), 命中组.end(),
            [](const 命中项& 左, const 命中项& 右) {
                return 左.尺度 < 右.尺度;
            })->尺度;
        std::vector<const 命中项*> 最具体候选;
        for (const auto& 候选 : 命中组) {
            if (候选.尺度 != 最佳尺度) continue;
            const bool 是同尺度候选的祖先 = std::any_of(
                命中组.begin(), 命中组.end(),
                [&](const 命中项& 其它) {
                    return 其它.尺度 == 最佳尺度
                        && 其它.节点 != 候选.节点
                        && std::find(其它.路径.begin(), 其它.路径.end(), 候选.节点)
                            != 其它.路径.end();
                });
            if (!是同尺度候选的祖先) 最具体候选.push_back(&候选);
        }
        if (最具体候选.size() > 1) {
            std::vector<稳定编码> 冲突候选;
            冲突候选.reserve(最具体候选.size());
            for (const auto* 候选 : 最具体候选) {
                冲突候选.push_back(候选->节点);
            }
            std::sort(冲突候选.begin(), 冲突候选.end());
            结果.状态 = 特征概念查找状态::匹配冲突;
            结果.冲突候选 = std::move(冲突候选);
            return 结果;
        }
        if (最具体候选.size() != 1) {
            结果.状态 = 特征概念查找状态::结构不一致;
            return 结果;
        }
        结果.状态 = 特征概念查找状态::已找到;
        结果.概念节点 = 最具体候选.front()->节点;
        return 结果;
    } catch (...) {
        结果.状态 = 特征概念查找状态::资源失败;
        结果.概念节点.reset();
        结果.冲突候选.clear();
        return 结果;
    }
}

bool 概念_特征类::是特征概念节点(稳定编码 节点) const noexcept {
    try {
        std::lock_guard 锁(特征概念树互斥);
        if (!结构仍存在() || !节点仍存在(节点)) return false;
        const auto 类型 = 读取唯一节点字段(节点, 节点类型字段);
        return 类型 && *类型 == 特征概念节点类型;
    } catch (...) {
        return false;
    }
}

特征概念比较结果 概念_特征类::执行比较规则(
    特征概念材料物理类型 材料类型,
    特征概念比较规则 规则,
    const 特征概念准确值& 左值,
    const 特征概念准确值& 右值,
    const 新_特征值类& 特征值服务) const noexcept {
    特征概念比较结果 结果;
    try {
        if (规则 == 特征概念比较规则::I64数值) {
            if (!是I64类型(材料类型)) {
                结果.状态 = 特征概念规则状态::材料类型不相容;
                return 结果;
            }
            const auto* 左 = std::get_if<std::int64_t>(&左值);
            const auto* 右 = std::get_if<std::int64_t>(&右值);
            if (!左 || !右) {
                结果.状态 = 特征概念规则状态::材料类型不相容;
                return 结果;
            }
            结果.次序 = *左 < *右 ? 特征概念比较次序::小于
                : *左 > *右 ? 特征概念比较次序::大于
                : 特征概念比较次序::相等;
            结果.相等 = *左 == *右;
            结果.I64绝对差 = 计算I64绝对差(*左, *右);
            结果.状态 = 特征概念规则状态::已完成;
            return 结果;
        }

        if (规则 == 特征概念比较规则::存在身份精确) {
            if (材料类型 != 特征概念材料物理类型::存在节点引用) {
                结果.状态 = 特征概念规则状态::材料类型不相容;
                return 结果;
            }
            const auto* 左节点 = std::get_if<稳定编码>(&左值);
            const auto* 右节点 = std::get_if<稳定编码>(&右值);
            if (!左节点 || !右节点 || !有效(*左节点) || !有效(*右节点)) {
                结果.状态 = 特征概念规则状态::材料类型不相容;
                return 结果;
            }
            结果.相等 = *左节点 == *右节点;
            if (*结果.相等) 结果.次序 = 特征概念比较次序::相等;
            结果.状态 = 特征概念规则状态::已完成;
            return 结果;
        }

        if (规则 != 特征概念比较规则::完整材料精确
            || 是I64类型(材料类型)) {
            结果.状态 = 特征概念规则状态::规则不支持;
            return 结果;
        }
        const auto 期望类型 = 转换特征值材料类型(材料类型);
        const auto* 左节点 = std::get_if<稳定编码>(&左值);
        const auto* 右节点 = std::get_if<稳定编码>(&右值);
        if (!期望类型 || !左节点 || !右节点
            || !有效(*左节点) || !有效(*右节点)) {
            结果.状态 = 特征概念规则状态::材料类型不相容;
            return 结果;
        }
        const auto 左信息 = 特征值服务.获取特征值(*左节点);
        const auto 右信息 = 特征值服务.获取特征值(*右节点);
        if (!左信息 || !右信息) {
            结果.状态 = 特征概念规则状态::值不存在;
            return 结果;
        }
        if (左信息->物理类型 != *期望类型 || 右信息->物理类型 != *期望类型) {
            结果.状态 = 特征概念规则状态::材料类型不相容;
            return 结果;
        }
        结果.相等 = 左信息->材料 == 右信息->材料;
        if (*结果.相等) 结果.次序 = 特征概念比较次序::相等;
        结果.状态 = 特征概念规则状态::已完成;
        return 结果;
    } catch (...) {
        结果.状态 = 特征概念规则状态::资源失败;
        结果.相等.reset();
        结果.次序.reset();
        结果.I64绝对差.reset();
        return 结果;
    }
}

特征概念值域比较结果 概念_特征类::比较值域(
    特征概念材料物理类型 材料类型,
    const 特征概念值域& 左值域,
    const 特征概念值域& 右值域) const noexcept {
    特征概念值域比较结果 结果;
    try {
        std::lock_guard 锁(特征概念树互斥);
        const auto 左 = 规范化待比较值域(材料类型, 左值域);
        const auto 右 = 规范化待比较值域(材料类型, 右值域);
        if (!左 || !右) {
            结果.状态 = 特征概念规则状态::材料类型不相容;
            return 结果;
        }
        if (*左 == *右) {
            结果.状态 = 特征概念规则状态::已完成;
            结果.关系 = 特征概念值域关系::相等;
            return 结果;
        }

        bool 左包含右 = false;
        bool 右包含左 = false;
        bool 相交 = false;
        if (std::holds_alternative<特征概念全材料值域>(*左)) {
            左包含右 = true;
            相交 = true;
        } else if (std::holds_alternative<特征概念全材料值域>(*右)) {
            右包含左 = true;
            相交 = true;
        } else if (const auto* 左I64 =
            std::get_if<std::vector<特征概念I64闭区间>>(&*左)) {
            const auto* 右I64 =
                std::get_if<std::vector<特征概念I64闭区间>>(&*右);
            if (!右I64) {
                结果.状态 = 特征概念规则状态::规则不支持;
                return 结果;
            }
            左包含右 = I64值域包含(*左I64, *右I64);
            右包含左 = I64值域包含(*右I64, *左I64);
            相交 = I64值域相交(*左I64, *右I64);
        } else if (const auto* 左节点组 =
            std::get_if<std::vector<稳定编码>>(&*左)) {
            const auto* 右节点组 = std::get_if<std::vector<稳定编码>>(&*右);
            if (!右节点组) {
                结果.状态 = 特征概念规则状态::规则不支持;
                return 结果;
            }
            左包含右 = 节点集合包含(*左节点组, *右节点组);
            右包含左 = 节点集合包含(*右节点组, *左节点组);
            相交 = 节点集合相交(*左节点组, *右节点组);
        } else if (const auto* 左引用 =
            std::get_if<特征概念存在引用值域>(&*左)) {
            const auto* 右引用 =
                std::get_if<特征概念存在引用值域>(&*右);
            if (!右引用 || !存在引用读取接口_) {
                结果.状态 = 特征概念规则状态::规则不支持;
                return 结果;
            }
            左包含右 = 存在引用读取接口_->存在概念包含(
                左引用->存在概念节点, 右引用->存在概念节点);
            右包含左 = 存在引用读取接口_->存在概念包含(
                右引用->存在概念节点, 左引用->存在概念节点);
            相交 = 存在引用读取接口_->存在概念相交(
                左引用->存在概念节点, 右引用->存在概念节点);
        } else {
            const auto* 左结构 = std::get_if<特征概念结构化I64值域>(&*左);
            const auto* 右结构 = std::get_if<特征概念结构化I64值域>(&*右);
            if (!左结构 || !右结构
                || 左结构->分量.size() != 右结构->分量.size()) {
                结果.状态 = 特征概念规则状态::规则不支持;
                return 结果;
            }
            左包含右 = true;
            右包含左 = true;
            相交 = true;
            for (std::size_t i = 0; i < 左结构->分量.size(); ++i) {
                const auto& 左分量 = 左结构->分量[i];
                const auto& 右分量 = 右结构->分量[i];
                if (左分量.角色 != 右分量.角色
                    || 左分量.单位 != 右分量.单位) {
                    结果.状态 = 特征概念规则状态::材料类型不相容;
                    return 结果;
                }
                左包含右 = 左包含右
                    && I64值域包含(左分量.值域, 右分量.值域);
                右包含左 = 右包含左
                    && I64值域包含(右分量.值域, 左分量.值域);
                相交 = 相交 && I64值域相交(左分量.值域, 右分量.值域);
            }
        }

        结果.关系 = 左包含右 ? 特征概念值域关系::左包含右
            : 右包含左 ? 特征概念值域关系::右包含左
            : 相交 ? 特征概念值域关系::相交
            : 特征概念值域关系::分离;
        结果.状态 = 特征概念规则状态::已完成;
        return 结果;
    } catch (...) {
        结果.状态 = 特征概念规则状态::资源失败;
        结果.关系.reset();
        return 结果;
    }
}

单特征概念关系结果 概念_特征类::比较单特征概念(
    稳定编码 左概念节点,
    稳定编码 右概念节点) const noexcept {
    单特征概念关系结果 结果;
    try {
        std::lock_guard 锁(特征概念树互斥);
        if (!结构仍存在() || !有效(左概念节点) || !有效(右概念节点)) {
            return 结果;
        }
        if (!节点仍存在(左概念节点) || !节点仍存在(右概念节点)) {
            结果.状态 = 特征概念规则状态::值不存在;
            return 结果;
        }
        const auto 左路径 = 读取单概念路径(左概念节点);
        const auto 右路径 = 读取单概念路径(右概念节点);
        if (!左路径 || !右路径) {
            结果.状态 = 特征概念规则状态::内部不一致;
            return 结果;
        }
        if (左路径->根 != 右路径->根) {
            结果.状态 = 特征概念规则状态::已完成;
            结果.关系 = 单特征概念关系::无可比关系;
            return 结果;
        }
        结果.特征类型根 = 左路径->根;
        if (左概念节点 == 右概念节点) {
            结果.状态 = 特征概念规则状态::已完成;
            结果.关系 = 单特征概念关系::同一;
            结果.最近共同上位 = 左概念节点;
            return 结果;
        }
        if (std::find(右路径->从自身到根.begin(),
            右路径->从自身到根.end(), 左概念节点)
            != 右路径->从自身到根.end()) {
            结果.状态 = 特征概念规则状态::已完成;
            结果.关系 = 单特征概念关系::左为上位;
            结果.最近共同上位 = 左概念节点;
            return 结果;
        }
        if (std::find(左路径->从自身到根.begin(),
            左路径->从自身到根.end(), 右概念节点)
            != 左路径->从自身到根.end()) {
            结果.状态 = 特征概念规则状态::已完成;
            结果.关系 = 单特征概念关系::右为上位;
            结果.最近共同上位 = 右概念节点;
            return 结果;
        }
        for (const auto 左上位 : 左路径->从自身到根) {
            if (std::find(右路径->从自身到根.begin(),
                右路径->从自身到根.end(), 左上位)
                != 右路径->从自身到根.end()) {
                结果.状态 = 特征概念规则状态::已完成;
                结果.关系 = 单特征概念关系::具有共同上位;
                结果.最近共同上位 = 左上位;
                return 结果;
            }
        }
        结果.状态 = 特征概念规则状态::内部不一致;
        return 结果;
    } catch (...) {
        结果.状态 = 特征概念规则状态::资源失败;
        结果.关系.reset();
        结果.特征类型根.reset();
        结果.最近共同上位.reset();
        return 结果;
    }
}

特征概念聚合结果 概念_特征类::执行聚合规则(
    特征概念材料物理类型 材料类型,
    特征概念聚合规则 规则,
    const std::vector<特征概念准确值>& 历史不同值,
    const 新_特征值类& 特征值服务) const noexcept {
    特征概念聚合结果 结果;
    try {
        if (历史不同值.empty()) return 结果;

        if (规则 == 特征概念聚合规则::不自动聚合) {
            if (是I64类型(材料类型)) {
                std::vector<特征概念I64闭区间> 域;
                for (const auto& 值 : 历史不同值) {
                    const auto* I64 = std::get_if<std::int64_t>(&值);
                    if (!I64) {
                        结果.状态 = 特征概念规则状态::材料类型不相容;
                        return 结果;
                    }
                    域.push_back({*I64, *I64});
                }
                std::sort(域.begin(), 域.end(), [](const auto& 左, const auto& 右) {
                    return 左.下界 < 右.下界;
                });
                域.erase(std::unique(域.begin(), 域.end()), 域.end());
                结果.值域 = std::move(域);
            } else {
                std::vector<稳定编码> 域;
                const auto 期望类型 = 转换特征值材料类型(材料类型);
                if (!期望类型) {
                    结果.状态 = 特征概念规则状态::材料类型不相容;
                    return 结果;
                }
                for (const auto& 值 : 历史不同值) {
                    const auto* 节点 = std::get_if<稳定编码>(&值);
                    if (!节点 || !有效(*节点)) {
                        结果.状态 = 特征概念规则状态::材料类型不相容;
                        return 结果;
                    }
                    const auto 信息 = 特征值服务.获取特征值(*节点);
                    if (!信息) {
                        结果.状态 = 特征概念规则状态::值不存在;
                        return 结果;
                    }
                    if (信息->物理类型 != *期望类型) {
                        结果.状态 = 特征概念规则状态::材料类型不相容;
                        return 结果;
                    }
                    域.push_back(*节点);
                }
                std::sort(域.begin(), 域.end(), [](稳定编码 左, 稳定编码 右) {
                    return 左.值 < 右.值;
                });
                域.erase(std::unique(域.begin(), 域.end()), 域.end());
                结果.值域 = std::move(域);
            }
            结果.状态 = 特征概念规则状态::无变化;
            return 结果;
        }

        if (规则 == 特征概念聚合规则::I64连续值归组) {
            if (!是I64类型(材料类型)) {
                结果.状态 = 特征概念规则状态::材料类型不相容;
                return 结果;
            }
            std::vector<std::int64_t> 值组;
            值组.reserve(历史不同值.size());
            for (const auto& 值 : 历史不同值) {
                const auto* I64 = std::get_if<std::int64_t>(&值);
                if (!I64) {
                    结果.状态 = 特征概念规则状态::材料类型不相容;
                    return 结果;
                }
                值组.push_back(*I64);
            }
            std::sort(值组.begin(), 值组.end());
            值组.erase(std::unique(值组.begin(), 值组.end()), 值组.end());
            std::vector<特征概念I64闭区间> 域;
            for (const auto 值 : 值组) {
                const 特征概念I64闭区间 单值{值, 值};
                if (域.empty() || !相邻或重叠(域.back(), 单值)) {
                    域.push_back(单值);
                } else {
                    域.back().上界 = 值;
                }
            }
            结果.值域 = std::move(域);
            结果.状态 = 特征概念规则状态::已完成;
            return 结果;
        }

        if (规则 == 特征概念聚合规则::完整材料精确集合) {
            if (是I64类型(材料类型)) {
                结果.状态 = 特征概念规则状态::材料类型不相容;
                return 结果;
            }
            std::vector<稳定编码> 域;
            const auto 期望类型 = 转换特征值材料类型(材料类型);
            if (!期望类型) {
                结果.状态 = 特征概念规则状态::材料类型不相容;
                return 结果;
            }
            for (const auto& 值 : 历史不同值) {
                const auto* 节点 = std::get_if<稳定编码>(&值);
                if (!节点 || !有效(*节点)) {
                    结果.状态 = 特征概念规则状态::材料类型不相容;
                    return 结果;
                }
                const auto 信息 = 特征值服务.获取特征值(*节点);
                if (!信息) {
                    结果.状态 = 特征概念规则状态::值不存在;
                    return 结果;
                }
                if (信息->物理类型 != *期望类型) {
                    结果.状态 = 特征概念规则状态::材料类型不相容;
                    return 结果;
                }
                域.push_back(*节点);
            }
            std::sort(域.begin(), 域.end(), [](稳定编码 左, 稳定编码 右) {
                return 左.值 < 右.值;
            });
            域.erase(std::unique(域.begin(), 域.end()), 域.end());
            结果.值域 = std::move(域);
            结果.状态 = 特征概念规则状态::已完成;
            return 结果;
        }

        结果.状态 = 特征概念规则状态::规则不支持;
        return 结果;
    } catch (...) {
        结果.状态 = 特征概念规则状态::资源失败;
        结果.值域.reset();
        return 结果;
    }
}

} // namespace 海中鱼巣
