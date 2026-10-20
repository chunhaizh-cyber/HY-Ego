#include "概念_特征类.h"
#include "../核心/容错检查.h"
#include <algorithm>
#include <iterator>
#include <limits>
#include <map>
#include <mutex>
#include <set>
#include <stdexcept>
#include <thread>
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
稳定编码 任务集合值域字段{};
稳定编码 至少一个任务字段{};
稳定编码 必含任务字段{};
稳定编码 必含需求任务字段{};
稳定编码 必含服务要求任务字段{};
稳定编码 需求集合值域字段{};
稳定编码 至少一个需求字段{};
稳定编码 必含需求字段{};
稳定编码 存在集合值域字段{}, 至少一个存在字段{}, 必含存在字段{};
稳定编码 存在集合保护节点类型{}, 存在集合保护来源字段{}, 存在集合成员保护键{};
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
        && 有效(任务集合值域字段)
        && 有效(至少一个任务字段)
        && 有效(必含任务字段)
        && 有效(必含需求任务字段)
        && 有效(必含服务要求任务字段)
        && 有效(需求集合值域字段)
        && 有效(存在集合值域字段) && 有效(至少一个存在字段) && 有效(必含存在字段)
        && 有效(存在集合保护节点类型) && 有效(存在集合保护来源字段) && 有效(存在集合成员保护键)
        && 有效(至少一个需求字段)
        && 有效(必含需求字段)
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
        && 节点仍存在(任务集合值域字段)
        && 节点仍存在(至少一个任务字段)
        && 节点仍存在(必含任务字段)
        && 节点仍存在(必含需求任务字段)
        && 节点仍存在(必含服务要求任务字段)
        && 节点仍存在(需求集合值域字段)
        && 节点仍存在(存在集合值域字段) && 节点仍存在(至少一个存在字段) && 节点仍存在(必含存在字段)
        && 节点仍存在(存在集合保护节点类型) && 节点仍存在(存在集合保护来源字段) && 节点仍存在(存在集合成员保护键)
        && 节点仍存在(至少一个需求字段)
        && 节点仍存在(必含需求字段)
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
        || 规则 == 特征概念比较规则::存在身份精确
        || 规则 == 特征概念比较规则::服务任务集合身份判等
        || 规则 == 特征概念比较规则::需求集合身份判等
        || 规则 == 特征概念比较规则::存在集合身份判等;
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
    if (定义.比较规则 == 特征概念比较规则::存在集合身份判等) {
        return 定义.材料类型 == 特征概念材料物理类型::U64数组 && !定义.单位
            && 定义.聚合规则 == 特征概念聚合规则::完整材料精确集合
            && (std::holds_alternative<特征概念全材料值域>(定义.值域)
                || std::holds_alternative<特征概念存在集合值域>(定义.值域)
                || std::holds_alternative<std::vector<稳定编码>>(定义.值域));
    }
    if (定义.比较规则 == 特征概念比较规则::需求集合身份判等) {
        return 定义.材料类型 == 特征概念材料物理类型::U64数组
            && !定义.单位
            && 定义.聚合规则 == 特征概念聚合规则::完整材料精确集合
            && (std::holds_alternative<特征概念全材料值域>(定义.值域)
                || std::holds_alternative<特征概念需求集合值域>(定义.值域)
                || std::holds_alternative<std::vector<稳定编码>>(定义.值域));
    }
    if (定义.比较规则 == 特征概念比较规则::服务任务集合身份判等) {
        return 定义.材料类型 == 特征概念材料物理类型::U64数组
            && !定义.单位
            && 定义.聚合规则 == 特征概念聚合规则::不自动聚合
            && (std::holds_alternative<特征概念全材料值域>(定义.值域)
                || std::holds_alternative<特征概念任务集合值域>(定义.值域)
                || std::holds_alternative<std::vector<稳定编码>>(定义.值域));
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
    } else if (auto* 集合 = std::get_if<特征概念存在集合值域>(&结果.值域)) {
        auto& 组 = 集合->必含存在节点组;
        if (std::any_of(组.begin(), 组.end(), [](稳定编码 n) { return !有效(n); })) return {};
        std::sort(组.begin(), 组.end());
        组.erase(std::unique(组.begin(), 组.end()), 组.end());
        if (!组.empty()) 集合->至少一个存在 = false;
    } else if (auto* 需求集合 = std::get_if<特征概念需求集合值域>(&结果.值域)) {
        auto& 组 = 需求集合->必含需求节点组;
        if (std::any_of(组.begin(), 组.end(),
            [](稳定编码 节点) { return !有效(节点); })) return std::nullopt;
        std::sort(组.begin(), 组.end());
        组.erase(std::unique(组.begin(), 组.end()), 组.end());
        if (!组.empty()) 需求集合->至少一个需求 = false;
    } else if (auto* 集合 = std::get_if<特征概念任务集合值域>(&结果.值域)) {
        for (auto* 组 : {&集合->必含任务节点组, &集合->必含需求任务组,
                &集合->必含服务要求任务组}) {
            if (std::any_of(组->begin(), 组->end(),
                [](稳定编码 节点) { return !有效(节点); })) return std::nullopt;
            std::sort(组->begin(), 组->end());
            组->erase(std::unique(组->begin(), 组->end()), 组->end());
        }
        if (!集合->必含任务节点组.empty() || !集合->必含需求任务组.empty()
            || !集合->必含服务要求任务组.empty()) {
            集合->至少一个任务 = false;
        }
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

特征概念规则状态 转换任务成员读取状态(特征任务成员读取状态 状态) noexcept {
    switch (状态) {
    case 特征任务成员读取状态::已读取: return 特征概念规则状态::已完成;
    case 特征任务成员读取状态::未装配: return 特征概念规则状态::读取接口未装配;
    case 特征任务成员读取状态::节点不存在: return 特征概念规则状态::值不存在;
    case 特征任务成员读取状态::资源失败: return 特征概念规则状态::资源失败;
    case 特征任务成员读取状态::结构不一致: return 特征概念规则状态::内部不一致;
    }
    return 特征概念规则状态::内部不一致;
}

using 任务需求身份组 = std::vector<std::pair<稳定编码, 稳定编码>>;

// 诊断责任：向上送出。接口只解码需求结构，不递归确认或其它领域锁。
特征概念规则状态 校验需求成员(
    const std::vector<稳定编码>& 成员, const 特征需求成员读取接口* 接口) {
    if (!接口) return 特征概念规则状态::读取接口未装配;
    for (const auto 节点 : 成员) {
        if (!有效(节点)) return 特征概念规则状态::入口拒绝;
        const auto 读取 = 接口->读取需求集合成员身份(节点);
        if (读取.状态 != 特征概念规则状态::已完成) return 读取.状态;
        if (!读取.身份 || 读取.身份->需求节点 != 节点
            || !有效(读取.身份->主体存在节点) || !读取.身份->被认知) {
            return 特征概念规则状态::内部不一致;
        }
    }
    return 特征概念规则状态::已完成;
}

特征概念规则状态 读取需求集合材料(
    const 特征概念准确值& 值, const 新_特征值类& 值服务,
    const 特征需求成员读取接口* 接口, std::vector<稳定编码>& 成员) {
    const auto* 节点 = std::get_if<稳定编码>(&值);
    if (!节点 || !有效(*节点)) return 特征概念规则状态::材料类型不相容;
    const auto 信息 = 值服务.获取特征值(*节点);
    if (!信息) return 特征概念规则状态::值不存在;
    const auto* 数组 = std::get_if<std::vector<std::uint64_t>>(&信息->材料);
    if (!数组 || 信息->物理类型 != 材料物理类型::U64数组)
        return 特征概念规则状态::材料类型不相容;
    成员.reserve(数组->size());
    for (const auto 编码 : *数组) 成员.push_back(稳定编码{编码});
    std::sort(成员.begin(), 成员.end());
    成员.erase(std::unique(成员.begin(), 成员.end()), 成员.end());
    return 校验需求成员(成员, 接口);
}

bool 需求约束蕴含(const 特征概念需求集合值域& 强,
    const 特征概念需求集合值域& 弱) {
    return (!弱.至少一个需求 || 强.至少一个需求 || !强.必含需求节点组.empty())
        && std::includes(强.必含需求节点组.begin(), 强.必含需求节点组.end(),
            弱.必含需求节点组.begin(), 弱.必含需求节点组.end());
}

特征概念规则状态 读取真实任务身份(
    稳定编码 任务, const 特征任务成员读取接口* 接口,
    任务需求身份组& 身份组) {
    if (!接口) return 特征概念规则状态::读取接口未装配;
    if (!有效(任务)) return 特征概念规则状态::入口拒绝;
    const auto 读取 = 接口->读取任务所属需求(任务);
    const auto 状态 = 转换任务成员读取状态(读取.状态);
    if (状态 != 特征概念规则状态::已完成) return 状态;
    if (!读取.需求节点 || !有效(*读取.需求节点)) {
        return 特征概念规则状态::内部不一致;
    }
    身份组.emplace_back(任务, *读取.需求节点);
    return 特征概念规则状态::已完成;
}

特征概念规则状态 校验集合目标身份(
    const 特征概念任务集合值域& 目标,
    const 特征任务成员读取接口* 接口,
    任务需求身份组& 指定任务身份) {
    if (!接口) return 特征概念规则状态::读取接口未装配;
    for (const auto 任务 : 目标.必含任务节点组) {
        const auto 状态 = 读取真实任务身份(任务, 接口, 指定任务身份);
        if (状态 != 特征概念规则状态::已完成) return 状态;
    }
    for (const auto 需求 : 目标.必含需求任务组) {
        if (!有效(需求)) return 特征概念规则状态::入口拒绝;
        const auto 状态 = 接口->验证正式需求(需求);
        if (状态 != 特征概念规则状态::已完成) return 状态;
    }
    for (const auto 确认 : 目标.必含服务要求任务组) {
        if (!有效(确认)) return 特征概念规则状态::入口拒绝;
        const auto 状态 = 接口->验证正式服务要求(确认);
        if (状态 != 特征概念规则状态::已完成) return 状态;
    }
    return 特征概念规则状态::已完成;
}

bool 任务集合约束蕴含(
    const 特征概念任务集合值域& 强,
    const 任务需求身份组& 强身份,
    const 特征概念任务集合值域& 弱,
    const 任务需求身份组& 弱身份) {
    const auto 包含 = [](const auto& 组, 稳定编码 节点) {
        return std::binary_search(组.begin(), 组.end(), 节点);
    };
    if (弱.至少一个任务 && !强.至少一个任务
        && 强.必含任务节点组.empty() && 强.必含需求任务组.empty()
        && 强.必含服务要求任务组.empty()) return false;
    for (const auto& [任务, 需求] : 弱身份) {
        // 正式端口已证明一个需求唯一对应该任务，因此需求约束也可证明身份。
        if (!包含(强.必含任务节点组, 任务)
            && !包含(强.必含需求任务组, 需求)) return false;
    }
    for (const auto 需求 : 弱.必含需求任务组) {
        if (!包含(强.必含需求任务组, 需求)
            && !std::any_of(强身份.begin(), 强身份.end(),
                [&](const auto& 项) { return 项.second == 需求; })) return false;
    }
    for (const auto 确认 : 弱.必含服务要求任务组) {
        if (!包含(强.必含服务要求任务组, 确认)) return false;
    }
    return true;
}

// 只展开已经正式采用的身份，不以同义文本或未来可能形成的D证明包含。
// 诊断责任：向上送出；端口只读物理来源，不进入fresh概念语义核验。
特征概念规则状态 展开集合确认来源(
    特征概念任务集合值域& 域, const 特征任务成员读取接口* 接口,
    std::vector<std::pair<稳定编码, 特征任务成员读取结果>>& 本次来源组) {
    if (域.必含服务要求任务组.empty()) return 特征概念规则状态::已完成;
    if (!接口) return 特征概念规则状态::读取接口未装配;
    std::vector<稳定编码> 未采用;
    for (const auto 确认 : 域.必含服务要求任务组) {
        auto 已读 = std::find_if(本次来源组.begin(), 本次来源组.end(),
            [&](const auto& 项) { return 项.first == 确认; });
        if (已读 == 本次来源组.end()) {
            本次来源组.emplace_back(确认, 接口->读取服务要求对应需求(确认));
            已读 = std::prev(本次来源组.end());
        }
        const auto& 来源 = 已读->second;
        const auto 状态 = 转换任务成员读取状态(来源.状态);
        if (状态 != 特征概念规则状态::已完成) return 状态;
        if (来源.需求节点) {
            if (!有效(*来源.需求节点)) return 特征概念规则状态::内部不一致;
            域.必含需求任务组.push_back(*来源.需求节点);
        } else 未采用.push_back(确认);
    }
    域.必含服务要求任务组 = std::move(未采用);
    std::sort(域.必含需求任务组.begin(), 域.必含需求任务组.end());
    域.必含需求任务组.erase(std::unique(域.必含需求任务组.begin(),
        域.必含需求任务组.end()), 域.必含需求任务组.end());
    return 特征概念规则状态::已完成;
}

std::optional<特征概念值域> 规范化待比较值域(特征概念材料物理类型, const 特征概念值域&);
using 需求快照签名组 = std::map<std::vector<稳定编码>, 稳定编码>;
特征概念规则状态 规范需求快照(const std::vector<稳定编码>& 材料,
    const 新_特征值类* 值服务, const 特征需求成员读取接口* 接口, 需求快照签名组& out) {
    if (!值服务) return 特征概念规则状态::读取接口未装配;
    for (const auto n : 材料) {
        std::vector<稳定编码> 签名;
        const auto s = 读取需求集合材料(n, *值服务, 接口, 签名);
        if (s != 特征概念规则状态::已完成) return s;
        const auto [i, 新] = out.emplace(std::move(签名), n);
        if (!新 && n < i->second) i->second = n;
    }
    return 特征概念规则状态::已完成;
}
特征概念值域比较结果 需求规则域关系(const 特征概念定义& 定义,
    const 特征概念值域& 左域, const 特征概念值域& 右域,
    const 新_特征值类* 值服务, const 特征需求成员读取接口* 接口) {
    特征概念值域比较结果 r;
    if (定义.比较规则 != 特征概念比较规则::需求集合身份判等
        || 定义.材料类型 != 特征概念材料物理类型::U64数组) return r;
    const auto l = 规范化待比较值域(定义.材料类型, 左域);
    const auto z = 规范化待比较值域(定义.材料类型, 右域);
    if (!l || !z) { r.状态 = 特征概念规则状态::材料类型不相容; return r; }
    const auto* lf = std::get_if<std::vector<稳定编码>>(&*l);
    const auto* rf = std::get_if<std::vector<稳定编码>>(&*z);
    const auto* lc = std::get_if<特征概念需求集合值域>(&*l);
    const auto* rc = std::get_if<特征概念需求集合值域>(&*z);
    const bool lu = std::holds_alternative<特征概念全材料值域>(*l);
    const bool ru = std::holds_alternative<特征概念全材料值域>(*z);
    if ((!lf && !lc && !lu) || (!rf && !rc && !ru)) { r.状态 = 特征概念规则状态::材料类型不相容; return r; }
    需求快照签名组 a, b;
    for (const auto& pair : {std::pair{lf, &a}, std::pair{rf, &b}}) if (pair.first) {
        r.状态 = 规范需求快照(*pair.first, 值服务, 接口, *pair.second);
        if (r.状态 != 特征概念规则状态::已完成) return r;
    }
    for (const auto* c : {lc, rc}) if (c) {
        r.状态 = 校验需求成员(c->必含需求节点组, 接口);
        if (r.状态 != 特征概念规则状态::已完成) return r;
    }
    const auto 满足 = [](const auto& 签名, const 特征概念需求集合值域& c) {
        return (!c.至少一个需求 || !签名.empty())
            && std::includes(签名.begin(), 签名.end(), c.必含需求节点组.begin(), c.必含需求节点组.end());
    };
    bool 左含右 = false, 右含左 = false, 相交 = true;
    if (lf && rf) {
        左含右 = std::all_of(b.begin(), b.end(), [&](const auto& x) { return a.contains(x.first); });
        右含左 = std::all_of(a.begin(), a.end(), [&](const auto& x) { return b.contains(x.first); });
        相交 = std::any_of(a.begin(), a.end(), [&](const auto& x) { return b.contains(x.first); });
    } else if (lf || rf) {
        const auto& f = lf ? a : b;
        const auto* c = lf ? rc : lc;
        const auto 命中 = std::count_if(f.begin(), f.end(), [&](const auto& x) { return !c || 满足(x.first, *c); });
        相交 = 命中 != 0;
        if (lf) 右含左 = 命中 == f.size(); else 左含右 = 命中 == f.size();
    } else {
        const 特征概念需求集合值域 空约束{};
        const auto& a域 = lc ? *lc : 空约束;
        const auto& b域 = rc ? *rc : 空约束;
        左含右 = 需求约束蕴含(b域, a域);
        右含左 = 需求约束蕴含(a域, b域);
    }
    r.关系 = 左含右 && 右含左 ? 特征概念值域关系::相等
        : 左含右 ? 特征概念值域关系::左包含右 : 右含左 ? 特征概念值域关系::右包含左
        : 相交 ? 特征概念值域关系::相交 : 特征概念值域关系::分离;
    r.状态 = 特征概念规则状态::已完成; return r;
}

struct 存在保护读取 final {
    std::optional<基础联合节点快照> 快照;
    std::vector<基础字段关系> 成员字段;
};
存在保护读取 读取存在保护(稳定编码 来源) {
    const auto 引用 = 全局基础数据集.查询目标字段(来源, 存在集合保护来源字段);
    if (引用.empty()) return {};
    if (引用.size() != 1) throw std::logic_error("duplicate existence-set protection");
    const auto b = 全局基础数据集.查询节点组完整结构({引用.front().所属节点});
    if (b.状态 == 基础结构操作状态::资源失败) throw std::bad_alloc{};
    if (b.状态 != 基础结构操作状态::已完成 || b.节点组.size() != 1 || !b.节点组[0].快照)
        throw std::logic_error("unreadable existence-set protection");
    存在保护读取 out{*b.节点组[0].快照, {}};
    unsigned 类型数 = 0, 来源数 = 0;
    std::set<稳定编码> 成员;
    for (const auto& f : out.快照->结构.字段组) {
        const auto* n = std::get_if<稳定编码>(&f.内容);
        if (!n || !有效(*n)) throw std::logic_error("invalid protection field");
        if (f.字段节点 == 节点类型字段 && *n == 存在集合保护节点类型) ++类型数;
        else if (f.字段节点 == 存在集合保护来源字段 && *n == 来源) ++来源数;
        else if (f.字段节点 == 存在集合成员保护键 && 成员.insert(*n).second) out.成员字段.push_back(f);
        else throw std::logic_error("unknown or duplicate protection field");
    }
    if (类型数 != 1 || 来源数 != 1 || !out.快照->入向字段组.empty()
        || !out.快照->入向外部关系组.empty() || !out.快照->出向外部关系组.empty())
        throw std::logic_error("invalid protection structure");
    return out;
}

std::vector<稳定编码> 保护成员组(const 存在保护读取& g) {
    std::vector<稳定编码> out;
    for (const auto& f : g.成员字段) out.push_back(std::get<稳定编码>(f.内容));
    std::sort(out.begin(), out.end());
    return out;
}
std::vector<稳定编码> 存在材料成员(稳定编码 n, const 新_特征值类& v) {
    const auto m = v.获取特征值(n);
    const auto* a = m ? std::get_if<std::vector<std::uint64_t>>(&m->材料) : nullptr;
    if (!a || m->物理类型 != 材料物理类型::U64数组)
        throw std::logic_error("existence-set source material invalid");
    std::vector<稳定编码> out;
    for (const auto x : *a) {
        if (!有效(稳定编码{x})) throw std::logic_error("zero existence member");
        out.push_back(稳定编码{x});
    }
    std::sort(out.begin(), out.end());
    out.erase(std::unique(out.begin(), out.end()), out.end());
    return out;
}
std::vector<基础联合节点快照> 查找存在成员保护来源(
    稳定编码 e, const 新_特征值类* v) {
    if (!v) return {};
    for (const auto& f : 全局基础数据集.查询目标字段(e, 存在集合成员保护键)) {
        const auto 来源 = 读取唯一节点字段(f.所属节点, 存在集合保护来源字段);
        if (!来源) throw std::logic_error("existence-set proof has no source");
        const auto g = 读取存在保护(*来源);
        if (!g.快照 || g.快照->结构.节点.编码 != f.所属节点
            || std::find(g.成员字段.begin(), g.成员字段.end(), f) == g.成员字段.end())
            throw std::logic_error("existence-set source proof mismatch");
        const auto b = 全局基础数据集.查询节点组完整结构({*来源});
        if (b.状态 == 基础结构操作状态::资源失败) throw std::bad_alloc{};
        if (b.状态 != 基础结构操作状态::已完成 || b.节点组.size()!=1 || !b.节点组[0].快照)
            throw std::logic_error("existence-set source unreadable");
        std::vector<稳定编码> 成员;
        if (v->获取特征值(*来源)) 成员 = 存在材料成员(*来源, *v);
        else {
            const auto& fields = b.节点组[0].快照->结构.字段组;
            const auto 单项 = [&](稳定编码 key) -> const 基础字段关系& {
                const 基础字段关系* found = nullptr;
                for (const auto& x : fields) if (x.字段节点 == key) {
                    if (found) throw std::logic_error("duplicate domain proof field");
                    found = &x;
                }
                if (!found) throw std::logic_error("missing domain proof field");
                return *found;
            };
            const auto 数 = [&](稳定编码 key) {
                const auto* value = std::get_if<基础值>(&单项(key).内容);
                const auto* n = value ? std::get_if<std::int64_t>(&value->材料) : nullptr;
                if (!n) throw std::logic_error("invalid domain proof scalar");
                return *n;
            };
            if (std::get<稳定编码>(单项(节点类型字段).内容) != 特征概念节点类型
                || 数(材料类型字段) != static_cast<std::int64_t>(特征概念材料物理类型::U64数组)
                || 数(比较规则字段) != static_cast<std::int64_t>(特征概念比较规则::存在集合身份判等)
                || 数(存在集合值域字段) != 1)
                throw std::logic_error("existence-set domain proof invalid");
            const auto nonempty = 数(至少一个存在字段);
            if (nonempty != 0 && nonempty != 1) throw std::logic_error("invalid domain nonempty");
            特征概念定义 definition;
            definition.材料类型 = 特征概念材料物理类型::U64数组;
            definition.比较规则 = 特征概念比较规则::存在集合身份判等;
            definition.聚合规则 = static_cast<特征概念聚合规则>(数(聚合规则字段));
            if (数(当前引用实例数字段) < 0) throw std::logic_error("invalid domain reference count");
            for (const auto& x : fields) {
                if (x.字段节点 == 必含存在字段) {
                    const auto* member = std::get_if<稳定编码>(&x.内容);
                    if (!member || !有效(*member)) throw std::logic_error("invalid domain member");
                    成员.push_back(*member);
                } else if (x.字段节点 == 名称关系字段) {
                    const auto* name = std::get_if<稳定编码>(&x.内容);
                    if (!name || !有效(*name)) throw std::logic_error("invalid domain name");
                    definition.名称关系.push_back(*name);
                } else if (x.字段节点 != 节点类型字段 && x.字段节点 != 材料类型字段
                    && x.字段节点 != 比较规则字段 && x.字段节点 != 聚合规则字段
                    && x.字段节点 != 存在集合值域字段 && x.字段节点 != 至少一个存在字段
                    && x.字段节点 != 当前引用实例数字段)
                    throw std::logic_error("mixed or unknown domain proof field");
            }
            std::sort(成员.begin(), 成员.end());
            if (std::adjacent_find(成员.begin(), 成员.end()) != 成员.end())
                throw std::logic_error("duplicate domain member");
            definition.值域 = 特征概念存在集合值域{成员, nonempty != 0};
            const auto normalized = 规范化定义(definition);
            if (!normalized || normalized->值域 != definition.值域
                || normalized->名称关系.size() != definition.名称关系.size())
                throw std::logic_error("invalid domain proof definition");
        }
        if (成员 != 保护成员组(g) || !std::binary_search(成员.begin(), 成员.end(), e))
            throw std::logic_error("existence-set source membership mismatch");
        // 来源V不可变；D由本FC锁保护。身份持续性只需冻结G，
        // 不把V/D无关入向引用变化重新引入成员接纳。
        return {*g.快照};
    }
    return {};
}
特征概念规则状态 校验存在成员(
    const std::vector<稳定编码>& 成员, const 特征存在成员读取接口* 接口,
    const 新_特征值类* 值服务 = nullptr) {
    for (const auto 节点 : 成员) {
        if (!有效(节点)) return 特征概念规则状态::入口拒绝;
        if (!查找存在成员保护来源(节点, 值服务).empty()) continue;
        if (!接口) return 特征概念规则状态::读取接口未装配;
        const auto 读取 = 接口->读取存在集合成员身份(节点);
        if (读取.状态 != 特征概念规则状态::已完成) return 读取.状态;
        if (读取.存在节点 != 节点 || 读取.身份保护.size() != 2
            || 读取.身份保护.front().结构.节点.编码 != 节点
            || 读取.身份保护.back().结构.节点.编码 == 节点) {
            return 特征概念规则状态::内部不一致;
        }
    }
    return 特征概念规则状态::已完成;
}

特征概念规则状态 读取存在集合材料(
    const 特征概念准确值& 值, const 新_特征值类& 值服务,
    const 特征存在成员读取接口* 接口, std::vector<稳定编码>& 成员) {
    const auto* 节点 = std::get_if<稳定编码>(&值);
    if (!节点 || !有效(*节点)) return 特征概念规则状态::材料类型不相容;
    const auto 信息 = 值服务.获取特征值(*节点);
    if (!信息) return 特征概念规则状态::值不存在;
    const auto* 数组 = std::get_if<std::vector<std::uint64_t>>(&信息->材料);
    if (!数组 || 信息->物理类型 != 材料物理类型::U64数组)
        return 特征概念规则状态::材料类型不相容;
    成员.reserve(数组->size());
    for (const auto 编码 : *数组) 成员.push_back(稳定编码{编码});
    std::sort(成员.begin(), 成员.end());
    成员.erase(std::unique(成员.begin(), 成员.end()), 成员.end());
    const auto g = 读取存在保护(*节点);
    if (g.快照) return 保护成员组(g) == 成员
        ? 特征概念规则状态::已完成 : 特征概念规则状态::内部不一致;
    return 校验存在成员(成员, 接口, &值服务);
}

bool 存在约束蕴含(const 特征概念存在集合值域& 强,
    const 特征概念存在集合值域& 弱) {
    return (!弱.至少一个存在 || 强.至少一个存在 || !强.必含存在节点组.empty())
        && std::includes(强.必含存在节点组.begin(), 强.必含存在节点组.end(),
            弱.必含存在节点组.begin(), 弱.必含存在节点组.end());
}


using 存在快照签名组 = std::map<std::vector<稳定编码>, 稳定编码>;
特征概念规则状态 规范存在快照(const std::vector<稳定编码>& 材料,
    const 新_特征值类* 值服务, const 特征存在成员读取接口* 接口, 存在快照签名组& out) {
    if (!值服务) return 特征概念规则状态::读取接口未装配;
    for (const auto n : 材料) {
        std::vector<稳定编码> 签名;
        const auto s = 读取存在集合材料(n, *值服务, 接口, 签名);
        if (s != 特征概念规则状态::已完成) return s;
        const auto [i, 新] = out.emplace(std::move(签名), n);
        if (!新 && n < i->second) i->second = n;
    }
    return 特征概念规则状态::已完成;
}
特征概念值域比较结果 存在规则域关系(const 特征概念定义& 定义,
    const 特征概念值域& 左域, const 特征概念值域& 右域,
    const 新_特征值类* 值服务, const 特征存在成员读取接口* 接口) {
    特征概念值域比较结果 r;
    if (定义.比较规则 != 特征概念比较规则::存在集合身份判等
        || 定义.材料类型 != 特征概念材料物理类型::U64数组) return r;
    const auto l = 规范化待比较值域(定义.材料类型, 左域);
    const auto z = 规范化待比较值域(定义.材料类型, 右域);
    if (!l || !z) { r.状态 = 特征概念规则状态::材料类型不相容; return r; }
    const auto* lf = std::get_if<std::vector<稳定编码>>(&*l);
    const auto* rf = std::get_if<std::vector<稳定编码>>(&*z);
    const auto* lc = std::get_if<特征概念存在集合值域>(&*l);
    const auto* rc = std::get_if<特征概念存在集合值域>(&*z);
    const bool lu = std::holds_alternative<特征概念全材料值域>(*l);
    const bool ru = std::holds_alternative<特征概念全材料值域>(*z);
    if ((!lf && !lc && !lu) || (!rf && !rc && !ru)) { r.状态 = 特征概念规则状态::材料类型不相容; return r; }
    存在快照签名组 a, b;
    for (const auto& pair : {std::pair{lf, &a}, std::pair{rf, &b}}) if (pair.first) {
        r.状态 = 规范存在快照(*pair.first, 值服务, 接口, *pair.second);
        if (r.状态 != 特征概念规则状态::已完成) return r;
    }
    for (const auto* c : {lc, rc}) if (c) {
        r.状态 = 校验存在成员(c->必含存在节点组, 接口, 值服务);
        if (r.状态 != 特征概念规则状态::已完成) return r;
    }
    const auto 满足 = [](const auto& 签名, const 特征概念存在集合值域& c) {
        return (!c.至少一个存在 || !签名.empty())
            && std::includes(签名.begin(), 签名.end(), c.必含存在节点组.begin(), c.必含存在节点组.end());
    };
    bool 左含右 = false, 右含左 = false, 相交 = true;
    if (lf && rf) {
        左含右 = std::all_of(b.begin(), b.end(), [&](const auto& x) { return a.contains(x.first); });
        右含左 = std::all_of(a.begin(), a.end(), [&](const auto& x) { return b.contains(x.first); });
        相交 = std::any_of(a.begin(), a.end(), [&](const auto& x) { return b.contains(x.first); });
    } else if (lf || rf) {
        const auto& f = lf ? a : b;
        const auto* c = lf ? rc : lc;
        const auto 命中 = std::count_if(f.begin(), f.end(), [&](const auto& x) { return !c || 满足(x.first, *c); });
        相交 = 命中 != 0;
        if (lf) 右含左 = 命中 == f.size(); else 左含右 = 命中 == f.size();
    } else {
        const 特征概念存在集合值域 空约束{};
        const auto& a域 = lc ? *lc : 空约束;
        const auto& b域 = rc ? *rc : 空约束;
        左含右 = 存在约束蕴含(b域, a域);
        右含左 = 存在约束蕴含(a域, b域);
    }
    r.关系 = 左含右 && 右含左 ? 特征概念值域关系::相等
        : 左含右 ? 特征概念值域关系::左包含右 : 右含左 ? 特征概念值域关系::右包含左
        : 相交 ? 特征概念值域关系::相交 : 特征概念值域关系::分离;
    r.状态 = 特征概念规则状态::已完成; return r;
}

struct 存在保护前提变化 final {};
struct 特征运算技术拒绝 final { 特征概念规则状态 状态; };
void 要求运算完成(特征概念规则状态 s) {
    if (s != 特征概念规则状态::已完成) throw 特征运算技术拒绝{s};
}
bool 存在域已经保护(const 特征概念值域& 域, const 新_特征值类& v) {
    if (const auto* c = std::get_if<特征概念存在集合值域>(&域)) {
        for (const auto e : c->必含存在节点组)
            if (查找存在成员保护来源(e, &v).empty()) return false;
        return true;
    }
    if (const auto* a = std::get_if<std::vector<稳定编码>>(&域)) {
        for (const auto n : *a) {
            const auto g = 读取存在保护(n);
            if (!g.快照) return false;
            if (保护成员组(g) != 存在材料成员(n, v))
                throw std::logic_error("persistent existence material proof mismatch");
        }
        return true;
    }
    return std::holds_alternative<特征概念全材料值域>(域);
}
void 合并存在保护预期(基础节点组追加请求& 请求, const 基础联合节点快照& s) {
    for (const auto& old : 请求.预期联合节点组) if (old.结构.节点.编码 == s.结构.节点.编码) {
        if (old.结构.字段组 != s.结构.字段组 || old.入向字段组 != s.入向字段组
            || old.入向外部关系组 != s.入向外部关系组 || old.出向外部关系组 != s.出向外部关系组)
            throw 存在保护前提变化{};
        return;
    }
    请求.预期联合节点组.push_back(s);
}
特征概念规则状态 追加存在集合保护写集(
    基础节点组追加请求& 请求,
    const std::variant<稳定编码, 基础组内节点引用>& 来源,
    const std::vector<稳定编码>& 成员,
    const 特征存在成员读取接口& 接口, const 新_特征值类& 值服务) noexcept {
    try {
        for (const auto e : 成员) {
            const auto 来源保护 = 查找存在成员保护来源(e, &值服务);
            if (!来源保护.empty()) {
                for (const auto& s : 来源保护) 合并存在保护预期(请求, s);
                continue;
            }
            const auto r = 接口.读取存在集合成员身份(e);
            if (r.状态 != 特征概念规则状态::已完成) return r.状态;
            if (r.存在节点 != e || r.身份保护.size() != 2
                || r.身份保护[0].结构.节点.编码 != e
                || r.身份保护[1].结构.节点.编码 == e) return 特征概念规则状态::内部不一致;
            for (const auto& s : r.身份保护) 合并存在保护预期(请求, s);
        }
        存在保护读取 旧;
        if (const auto* n = std::get_if<稳定编码>(&来源)) {
            const auto b = 全局基础数据集.查询节点组完整结构({*n});
            if (b.状态 == 基础结构操作状态::资源失败) return 特征概念规则状态::资源失败;
            if (b.状态 != 基础结构操作状态::已完成 || b.节点组.size() != 1 || !b.节点组[0].快照)
                return 特征概念规则状态::内部不一致;
            合并存在保护预期(请求, *b.节点组[0].快照);
            旧 = 读取存在保护(*n);
        }
        if (旧.快照) {
            合并存在保护预期(请求, *旧.快照);
            std::vector<稳定编码> 原;
            for (const auto& f : 旧.成员字段) 原.push_back(std::get<稳定编码>(f.内容));
            std::sort(原.begin(), 原.end());
            if (原 != 成员) {
                for (const auto& f : 旧.成员字段) 请求.删除字段关系组.push_back(f.编码);
                for (const auto e : 成员)
                    请求.追加字段组.push_back({旧.快照->结构.节点.编码, 存在集合成员保护键, e});
            }
        } else {
            基础组内新节点 g;
            g.字段组.push_back({节点类型字段, 存在集合保护节点类型});
            std::visit([&](const auto& n) { g.字段组.push_back({存在集合保护来源字段, n}); }, 来源);
            for (const auto e : 成员) g.字段组.push_back({存在集合成员保护键, e});
            请求.新节点组.push_back(std::move(g));
        }
        return 特征概念规则状态::已完成;
    } catch (const 存在保护前提变化&) { return 特征概念规则状态::前提已变化; }
    catch (const std::bad_alloc&) { return 特征概念规则状态::资源失败; }
    catch (...) { return 特征概念规则状态::内部不一致; }
}

特征概念规则状态 确保存在材料保护内部(稳定编码 v, const 新_特征值类& 值服务,
    const 特征存在成员读取接口* 接口) {
    std::vector<稳定编码> 成员;
    const auto s = 读取存在集合材料(v, 值服务, 接口, 成员);
    if (s != 特征概念规则状态::已完成) return s;
    const auto g = 读取存在保护(v);
    if (g.快照) {
        std::vector<稳定编码> 原;
        for (const auto& f : g.成员字段) 原.push_back(std::get<稳定编码>(f.内容));
        std::sort(原.begin(), 原.end());
        return 原 == 成员 ? 特征概念规则状态::已完成 : 特征概念规则状态::内部不一致;
    }
    基础节点组追加请求 q;
    const auto p = 追加存在集合保护写集(q, v, 成员, *接口, 值服务);
    if (p != 特征概念规则状态::已完成) return p;
    const auto seal = 全局基础数据集.封存原子追加片段(q);
    if (seal.状态 != 基础节点组追加状态::已完成 || !seal.片段)
        return 特征概念规则状态::资源失败;
    const auto r = 全局基础数据集.原子追加封存片段组({*seal.片段});
    if (r.状态 == 基础节点组追加状态::预期不符) return 特征概念规则状态::前提已变化;
    if (r.状态 != 基础节点组追加状态::已完成) return 特征概念规则状态::资源失败;
    if (!读取存在保护(v).快照) throw std::logic_error("existence material protection readback failed");
    return 特征概念规则状态::已完成;
}
std::vector<基础组内初始字段> 编码存在集合域(const 特征概念值域& 域) {
    std::vector<基础组内初始字段> out;
    if (const auto* c = std::get_if<特征概念存在集合值域>(&域)) {
        out.push_back({存在集合值域字段, 基础值{std::int64_t{1}}});
        out.push_back({至少一个存在字段, 基础值{std::int64_t{c->至少一个存在 ? 1 : 0}}});
        for (const auto e : c->必含存在节点组) out.push_back({必含存在字段, e});
    } else if (std::holds_alternative<特征概念全材料值域>(域))
        out.push_back({全材料值域字段, 基础值{std::int64_t{1}}});
    else if (const auto* v = std::get_if<std::vector<稳定编码>>(&域))
        for (const auto n : *v) out.push_back({非I64值域字段, n});
    else throw std::logic_error("invalid existence-set domain");
    return out;
}

bool 结构定义相同(
    const 特征概念定义& 左,
    const 特征概念定义& 右, const 新_特征值类* 值服务 = nullptr,
    const 特征需求成员读取接口* 接口 = nullptr,
    const 特征存在成员读取接口* 存在接口 = nullptr) {
    if (左.比较规则 == 特征概念比较规则::存在集合身份判等) {
        if (左.材料类型 != 右.材料类型 || 左.单位 != 右.单位
            || 左.比较规则 != 右.比较规则 || 左.聚合规则 != 右.聚合规则) return false;
        const auto r = 存在规则域关系(左, 左.值域, 右.值域, 值服务, 存在接口);
        要求运算完成(r.状态);
        return r.关系 == 特征概念值域关系::相等;
    }
    if (左.比较规则 == 特征概念比较规则::需求集合身份判等) {
        if (左.材料类型 != 右.材料类型 || 左.单位 != 右.单位
            || 左.比较规则 != 右.比较规则 || 左.聚合规则 != 右.聚合规则) return false;
        const auto r = 需求规则域关系(左, 左.值域, 右.值域, 值服务, 接口);
        return r.状态 == 特征概念规则状态::已完成 && r.关系 == 特征概念值域关系::相等;
    }
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
    if (父.比较规则 == 特征概念比较规则::需求集合身份判等
        || 父.比较规则 == 特征概念比较规则::存在集合身份判等) return false;
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
    const 特征概念定义& 子,
    const 概念_特征类* 概念服务 = nullptr, const 新_特征值类* 值服务 = nullptr,
    const 特征需求成员读取接口* 接口 = nullptr,
    const 特征存在成员读取接口* 存在接口 = nullptr) {
    if (父.材料类型 != 子.材料类型 || 父.单位 != 子.单位) {
        return false;
    }
    if (父.比较规则 == 特征概念比较规则::存在集合身份判等) {
        if (父.比较规则 != 子.比较规则 || 父.聚合规则 != 子.聚合规则) return false;
        const auto r = 存在规则域关系(父, 父.值域, 子.值域, 值服务, 存在接口);
        要求运算完成(r.状态);
        return r.关系 == 特征概念值域关系::左包含右;
    }
    if (父.比较规则 == 特征概念比较规则::需求集合身份判等) {
        if (父.比较规则 != 子.比较规则 || 父.聚合规则 != 子.聚合规则) return false;
        const auto r = 需求规则域关系(父, 父.值域, 子.值域, 值服务, 接口);
        return r.状态 == 特征概念规则状态::已完成 && r.关系 == 特征概念值域关系::左包含右;
    }
    if (std::holds_alternative<特征概念需求集合值域>(父.值域)
        || std::holds_alternative<特征概念需求集合值域>(子.值域)
        || std::holds_alternative<特征概念任务集合值域>(父.值域)
        || std::holds_alternative<特征概念任务集合值域>(子.值域)) {
        if (!概念服务 || 父.比较规则 != 子.比较规则
            || 父.聚合规则 != 子.聚合规则) return false;
        const auto 比较 = 概念服务->比较值域(父.材料类型, 父.值域, 子.值域);
        return 比较.状态 == 特征概念规则状态::已完成
            && 比较.关系 == 特征概念值域关系::左包含右;
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
    } else if (std::holds_alternative<特征概念存在集合值域>(值域)) {
        定义.比较规则 = 特征概念比较规则::存在集合身份判等;
        定义.聚合规则 = 特征概念聚合规则::完整材料精确集合;
    } else if (std::holds_alternative<特征概念需求集合值域>(值域)) {
        定义.比较规则 = 特征概念比较规则::需求集合身份判等;
        定义.聚合规则 = 特征概念聚合规则::完整材料精确集合;
    } else if (std::holds_alternative<特征概念任务集合值域>(值域)) {
        定义.比较规则 = 特征概念比较规则::服务任务集合身份判等;
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
    const 特征存在引用读取接口* 存在读取接口,
    const 特征需求成员读取接口* 需求接口 = nullptr,
    const 特征存在成员读取接口* 存在接口 = nullptr) {
    if (概念.比较规则 == 特征概念比较规则::存在集合身份判等) {
        const auto* n = std::get_if<稳定编码>(&值);
        if (!n) return {};
        const auto r = 存在规则域关系(转为定义(概念), 概念.值域,
            std::vector<稳定编码>{*n}, &特征值服务, 存在接口);
        要求运算完成(r.状态);
        if (r.关系 != 特征概念值域关系::相等 && r.关系 != 特征概念值域关系::左包含右) return {};
        if (const auto* f = std::get_if<std::vector<稳定编码>>(&概念.值域)) {
            存在快照签名组 签名;
            要求运算完成(规范存在快照(*f, &特征值服务, 存在接口, 签名));
            return 签名.size();
        }
        return (std::numeric_limits<std::uint64_t>::max)();
    }
    if (概念.比较规则 == 特征概念比较规则::需求集合身份判等) {
        const auto* n = std::get_if<稳定编码>(&值);
        if (!n) return {};
        const auto r = 需求规则域关系(转为定义(概念), 概念.值域,
            std::vector<稳定编码>{*n}, &特征值服务, 需求接口);
        if (r.状态 != 特征概念规则状态::已完成
            || (r.关系 != 特征概念值域关系::相等 && r.关系 != 特征概念值域关系::左包含右)) return {};
        if (const auto* f = std::get_if<std::vector<稳定编码>>(&概念.值域)) {
            需求快照签名组 签名;
            if (规范需求快照(*f, &特征值服务, 需求接口, 签名) != 特征概念规则状态::已完成) return {};
            return 签名.size();
        }
        return (std::numeric_limits<std::uint64_t>::max)();
    }
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
    try {
    const auto 当前数量 = 读取唯一I64字段(
        概念节点, 当前引用实例数字段);
    if (!当前数量 || *当前数量 != 0) {
        return 当前数量 ? 零引用概念清理结果::无须删除
                         : 零引用概念清理结果::失败;
    }

    const auto 子关系 = 全局基础数据集.查询源关系(
        概念节点, 基础外部关系类型::父子);
    if (!子关系.empty()) return 零引用概念清理结果::无须删除;
    const auto 自有保护 = 读取存在保护(概念节点);
    const auto 完整 = 全局基础数据集.查询节点组完整结构({概念节点});
    if (完整.状态 != 基础结构操作状态::已完成 || 完整.节点组.size() != 1 || !完整.节点组[0].快照)
        return 零引用概念清理结果::失败;
    for (const auto& f : 完整.节点组[0].快照->入向字段组)
        if (!自有保护.快照 || f.所属节点 != 自有保护.快照->结构.节点.编码
            || f.字段节点 != 存在集合保护来源字段)
            return 零引用概念清理结果::无须删除;

    // 名称关系节点会反向引用本概念。只要名称仍存在，概念就仍是
    // 自然语言可召回的正式概念，不能在实例引用归零时物理删除。
    if (!全局基础数据集.查询字段(
        概念节点, 名称关系字段).empty()) {
        return 零引用概念清理结果::无须删除;
    }

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
        任务集合值域字段,
        至少一个任务字段,
        必含任务字段,
        必含需求任务字段,
        必含服务要求任务字段,
        需求集合值域字段,
        至少一个需求字段,
        必含需求字段,
        存在集合值域字段, 至少一个存在字段, 必含存在字段,
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

    std::vector<稳定编码> 删除节点组{概念节点};
    std::vector<稳定编码> 删除字段组 = 概念字段关系;
    if (自有保护.快照) {
        删除节点组.push_back(自有保护.快照->结构.节点.编码);
        for (const auto& f : 自有保护.快照->结构.字段组) 删除字段组.push_back(f.编码);
    }
    std::vector<稳定编码> 删除关系组{父关系.front().编码};
    for (const auto& 关系 : 兄弟关系) {
        删除关系组.push_back(关系.编码);
    }
    for (const auto& 分量 : 分量材料) {
        删除节点组.push_back(分量.节点);
        删除字段组.insert(删除字段组.end(), 分量.字段关系.begin(), 分量.字段关系.end());
    }
    const auto 删除结果 = 全局基础数据集.原子删除节点结构(删除节点组, 删除字段组, 删除关系组);
    if (删除结果 == 基础结构操作状态::引用仍存在) return 零引用概念清理结果::无须删除;
    if (删除结果 != 基础结构操作状态::已完成)
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"删除零引用特征概念", L"完整结构删除失败。");
    if (先天概念.三维空间坐标 == 概念节点) 先天概念.三维空间坐标 = {};
    if (先天概念.三维空间尺寸 == 概念节点) 先天概念.三维空间尺寸 = {};
    if (先天概念.RGB颜色 == 概念节点) 先天概念.RGB颜色 = {};
    if (先天概念.二维轮廓 == 概念节点) 先天概念.二维轮廓 = {};
    if (先天概念.三维体素 == 概念节点) 先天概念.三维体素 = {};
    if (先天概念.服务任务集合 == 概念节点) 先天概念.服务任务集合 = {};
    if (先天概念.服务对象需求集合 == 概念节点) 先天概念.服务对象需求集合 = {};
    if (先天概念.交互对象集合 == 概念节点) 先天概念.交互对象集合 = {};
    return 零引用概念清理结果::已删除;
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"删除零引用特征概念", L"收集删除结构失败。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"删除零引用特征概念", L"未知工程异常。");
    }
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

struct 存在集合保护准备凭据::实现 final {
    const 概念_特征类* owner = nullptr;
    const 新_特征值类* 值服务 = nullptr;
    std::thread::id 线程 = std::this_thread::get_id();
    稳定编码 类型根, 材料;
    std::vector<基础联合节点快照> 原依据;
    基础节点组追加请求 写集;
    std::optional<基础封存追加片段> 片段;
    std::optional<基础片段发布回执> 回执;
    bool 已消费 = false;
    ~实现() {
        if (线程 != std::this_thread::get_id())
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"存在集合保护凭据", L"凭据必须在创建线程释放。");
    }
};
存在集合保护准备凭据::存在集合保护准备凭据(std::unique_ptr<实现> p)
    : 实现_(std::move(p)) {}
存在集合保护准备凭据::~存在集合保护准备凭据() = default;
存在集合保护准备凭据::存在集合保护准备凭据(存在集合保护准备凭据&&) noexcept = default;
存在集合保护准备凭据& 存在集合保护准备凭据::operator=(存在集合保护准备凭据&&) noexcept = default;

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
        && 有效(二维轮廓) && 有效(三维体素) && 有效(服务任务集合)
        && 有效(服务对象需求集合) && 有效(交互对象集合);
}

void 概念_特征类::装配存在引用读取接口(
    const 特征存在引用读取接口& 读取接口) noexcept {
    std::lock_guard 锁(特征概念树互斥);
    存在引用读取接口_ = &读取接口;
}

void 概念_特征类::装配任务成员读取接口(
    const 特征任务成员读取接口* 接口) noexcept {
    std::lock_guard 锁(特征概念树互斥);
    任务成员读取接口_ = 接口;
}

void 概念_特征类::装配需求成员读取接口(
    const 特征需求成员读取接口* 接口) noexcept {
    std::lock_guard 锁(特征概念树互斥);
    需求成员读取接口_ = 接口;
}

void 概念_特征类::装配存在成员读取接口(const 特征存在成员读取接口* 接口) noexcept {
    std::lock_guard 锁(特征概念树互斥);
    if (!接口 || (存在成员读取接口_ && 存在成员读取接口_ != 接口))
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"存在成员接口装配", L"不可替换运行中的存在成员提供者。");
    存在成员读取接口_ = 接口;
}
稳定编码 概念_特征类::存在集合成员保护字段() const noexcept {
    return 存在集合成员保护键;
}
特征概念规则状态 概念_特征类::核验存在集合材料保护(
    稳定编码 类型根, 稳定编码 材料, const 新_特征值类& 值服务) const noexcept {
    using S = 特征概念规则状态;
    try {
        std::lock_guard 锁(特征概念树互斥);
        const auto d = 获取特征概念(类型根);
        const auto path = 读取单概念路径(类型根);
        if (!d || !path || path->根 != 类型根
            || d->比较规则 != 特征概念比较规则::存在集合身份判等)
            return S::入口拒绝;
        const auto value = 值服务.获取特征值(材料);
        if (!value) return S::值不存在;
        if (value->物理类型 != 材料物理类型::U64数组
            || !std::holds_alternative<std::vector<std::uint64_t>>(value->材料))
            return S::材料类型不相容;
        const auto members = 存在材料成员(材料, 值服务);
        const auto g = 读取存在保护(材料);
        return g.快照 && 保护成员组(g) == members ? S::已完成 : S::内部不一致;
    } catch (const std::bad_alloc&) { return S::资源失败; }
    catch (...) { return S::内部不一致; }
}
存在集合保护准备结果 概念_特征类::准备存在集合材料保护(
    稳定编码 类型根, 稳定编码 材料, const 新_特征值类& 值服务,
    const std::vector<基础联合节点快照>& 旧依据) noexcept {
    using S = 特征概念规则状态;
    try {
        std::lock_guard 锁(特征概念树互斥);
        const auto d = 获取特征概念(类型根);
        const auto path = 读取单概念路径(类型根);
        if (!d || !path || path->根 != 类型根
            || d->比较规则 != 特征概念比较规则::存在集合身份判等)
            return {S::入口拒绝, {}};
        if (!存在成员读取接口_) return {S::读取接口未装配, {}};
        auto p = std::make_unique<存在集合保护准备凭据::实现>();
        p->owner = this; p->值服务 = &值服务; p->类型根 = 类型根; p->材料 = 材料;
        std::vector<稳定编码> nodes;
        for (const auto& s : 旧依据) {
            if (!有效(s.结构.节点.编码)
                || std::find(nodes.begin(), nodes.end(), s.结构.节点.编码) != nodes.end())
                return {S::入口拒绝, {}};
            nodes.push_back(s.结构.节点.编码);
        }
        p->原依据 = 旧依据;
        if (!nodes.empty()) {
            const auto b = 全局基础数据集.查询节点组完整结构(nodes);
            if (b.状态 == 基础结构操作状态::资源失败) return {S::资源失败, {}};
            if (b.状态 != 基础结构操作状态::已完成 || b.节点组.size() != nodes.size())
                return {S::内部不一致, {}};
            for (std::size_t i = 0; i < nodes.size(); ++i) {
                const auto found = std::find_if(b.节点组.begin(), b.节点组.end(),
                    [&](const auto& item) { return item.查询节点 == nodes[i]; });
                if (found == b.节点组.end()) return {S::内部不一致, {}};
                const auto& current = *found;
                const auto& old = 旧依据[i];
                if (current.查询节点 != nodes[i]) return {S::内部不一致, {}};
                if (!current.快照 || current.快照->结构.字段组 != old.结构.字段组
                    || current.快照->入向字段组 != old.入向字段组
                    || current.快照->入向外部关系组 != old.入向外部关系组
                    || current.快照->出向外部关系组 != old.出向外部关系组)
                    return {S::前提已变化, {}};
            }
        }
        std::vector<稳定编码> members;
        const auto read = 读取存在集合材料(材料, 值服务, 存在成员读取接口_, members);
        if (read != S::已完成) return {read, {}};
        const auto oldG = 读取存在保护(材料);
        if (oldG.快照) {
            if (保护成员组(oldG) != members) return {S::内部不一致, {}};
        } else {
            p->写集.预期联合节点组 = 旧依据;
            const auto ready = 追加存在集合保护写集(
                p->写集, 材料, members, *存在成员读取接口_, 值服务);
            if (ready != S::已完成) return {ready, {}};
            // 本凭据仅允许追加；禁止以后将删除/替换混入追加依据推进。
            if (!p->写集.替换字段组.empty() || !p->写集.删除字段关系组.empty())
                return {S::内部不一致, {}};
            const auto seal = 全局基础数据集.封存原子追加片段(p->写集);
            if (seal.状态 != 基础节点组追加状态::已完成 || !seal.片段)
                return {seal.状态 == 基础节点组追加状态::资源失败 ? S::资源失败 : S::内部不一致, {}};
            p->片段 = *seal.片段;
            const auto result = 全局基础数据集.原子追加封存片段组({*p->片段});
            if (result.状态 == 基础节点组追加状态::预期不符) return {S::前提已变化, {}};
            if (result.状态 != 基础节点组追加状态::已完成)
                return {result.状态 == 基础节点组追加状态::资源失败 ? S::资源失败 : S::内部不一致, {}};
            if (result.片段回执组.size() != 1 || !result.片段回执组.front().对应片段(*p->片段))
                throw std::logic_error("existence protection receipt mismatch");
            p->回执 = result.片段回执组.front();
        }
        存在集合保护准备结果 result;
        result.状态 = S::已完成;
        result.凭据 = 存在集合保护准备凭据{std::move(p)};
        return result;
    } catch (const std::bad_alloc&) { return {S::资源失败, {}}; }
    catch (...) { return {S::内部不一致, {}}; }
}
存在集合依据推进结果 概念_特征类::消费存在集合保护依据(
    存在集合保护准备凭据&& token) const noexcept {
    using S = 特征概念规则状态;
    if (!token.实现_ || token.实现_->owner != this || token.实现_->已消费
        || token.实现_->线程 != std::this_thread::get_id())
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"消费存在集合保护依据",
            L"凭据owner、线程或消费阶段错误。");
    auto& p = *token.实现_;
    p.已消费 = true;
    try {
        std::lock_guard 锁(特征概念树互斥);
        auto advanced = p.原依据;
        if (p.片段.has_value() != p.回执.has_value()) throw std::logic_error("missing protection receipt");
        if (p.回执) {
            const auto& r = *p.回执;
            const auto& q = p.写集;
            if (!r.对应片段(*p.片段) || r.新节点组().size() != q.新节点组.size()
                || r.新节点初始字段关系组().size() != q.新节点组.size()
                || r.追加字段关系组().size() != q.追加字段组.size()
                || r.追加外部关系组().size() != q.追加外部关系组.size()
                || !q.删除字段关系组.empty() || !q.替换字段组.empty())
                throw std::logic_error("protection receipt shape invalid");
            const auto node = [&](const auto& ref) -> 稳定编码 {
                if (const auto* n = std::get_if<稳定编码>(&ref)) return *n;
                const auto index = std::get<基础组内节点引用>(ref).索引;
                if (index >= r.新节点组().size()) throw std::logic_error("protection local reference invalid");
                return r.新节点组()[index];
            };
            const auto value = [&](const 基础组内字段内容& input) -> 基础字段内容 {
                if (const auto* direct = std::get_if<基础值>(&input)) return *direct;
                if (const auto* n = std::get_if<稳定编码>(&input)) return *n;
                const auto index = std::get<基础组内节点引用>(input).索引;
                if (index >= r.新节点组().size()) throw std::logic_error("protection field reference invalid");
                return r.新节点组()[index];
            };
            const auto addField = [&](const 基础字段关系& field) {
                if (!有效(field.编码)) throw std::logic_error("protection field code missing");
                for (auto& old : advanced) {
                    if (old.结构.节点.编码 == field.所属节点) old.结构.字段组.push_back(field);
                    const auto* target = std::get_if<稳定编码>(&field.内容);
                    if (target && *target == old.结构.节点.编码) old.入向字段组.push_back(field);
                }
            };
            for (std::size_t i = 0; i < q.新节点组.size(); ++i) {
                const auto& fields = q.新节点组[i].字段组;
                if (r.新节点初始字段关系组()[i].size() != fields.size())
                    throw std::logic_error("protection initial field receipt mismatch");
                for (std::size_t j = 0; j < fields.size(); ++j)
                    addField({r.新节点初始字段关系组()[i][j], r.新节点组()[i],
                        fields[j].字段节点, value(fields[j].内容)});
            }
            for (std::size_t i = 0; i < q.追加字段组.size(); ++i) {
                const auto& f = q.追加字段组[i];
                addField({r.追加字段关系组()[i], f.所属节点, f.字段节点, value(f.内容)});
            }
            for (std::size_t i = 0; i < q.追加外部关系组.size(); ++i) {
                const auto& source = q.追加外部关系组[i];
                const 基础外部关系 edge{r.追加外部关系组()[i], node(source.源节点),
                    node(source.目标节点), source.类型, source.角色或顺序};
                for (auto& old : advanced) {
                    if (old.结构.节点.编码 == edge.源节点) old.出向外部关系组.push_back(edge);
                    if (old.结构.节点.编码 == edge.目标节点) old.入向外部关系组.push_back(edge);
                }
            }
        }
        const auto protection = 核验存在集合材料保护(p.类型根, p.材料, *p.值服务);
        if (protection != S::已完成) return {protection, {}};
        std::vector<稳定编码> nodes;
        for (auto& old : advanced) {
            nodes.push_back(old.结构.节点.编码);
            const auto less = [](const auto& a, const auto& b) { return a.编码 < b.编码; };
            std::sort(old.结构.字段组.begin(), old.结构.字段组.end(), less);
            std::sort(old.入向字段组.begin(), old.入向字段组.end(), less);
            std::sort(old.入向外部关系组.begin(), old.入向外部关系组.end(), less);
            std::sort(old.出向外部关系组.begin(), old.出向外部关系组.end(), less);
        }
        if (!nodes.empty()) {
            const auto back = 全局基础数据集.查询节点组完整结构(nodes);
            if (back.状态 == 基础结构操作状态::资源失败) return {S::资源失败, {}};
            if (back.状态 != 基础结构操作状态::已完成 || back.节点组.size() != nodes.size())
                return {S::内部不一致, {}};
            for (std::size_t i = 0; i < nodes.size(); ++i) {
                const auto found = std::find_if(back.节点组.begin(), back.节点组.end(),
                    [&](const auto& item) { return item.查询节点 == nodes[i]; });
                if (found == back.节点组.end()) return {S::内部不一致, {}};
                const auto& current = *found;
                const auto& expected = advanced[i];
                if (current.查询节点 != nodes[i]) return {S::内部不一致, {}};
                if (!current.快照 || current.快照->结构.字段组 != expected.结构.字段组
                    || current.快照->入向字段组 != expected.入向字段组
                    || current.快照->入向外部关系组 != expected.入向外部关系组
                    || current.快照->出向外部关系组 != expected.出向外部关系组)
                    return {S::前提已变化, {}};
            }
        }
        return {S::已完成, std::move(advanced)};
    } catch (const std::bad_alloc&) { return {S::资源失败, {}}; }
    catch (...) { return {S::内部不一致, {}}; }
}
特征概念规则状态 概念_特征类::确保存在集合材料保护(
    稳定编码 类型根, 稳定编码 材料, const 新_特征值类& 值服务) noexcept {
    auto ready = 准备存在集合材料保护(类型根, 材料, 值服务, {});
    if (ready.状态 != 特征概念规则状态::已完成) return ready.状态;
    if (!ready.凭据) return 特征概念规则状态::内部不一致;
    return 消费存在集合保护依据(std::move(*ready.凭据)).状态;
}

特征需求集合条件比较结果 概念_特征类::比较需求集合值域(
    稳定编码 类型根, const 特征概念准确值& 当前,
    const 特征概念需求集合值域& 目标,
    const 新_特征值类& 值服务) const noexcept {
    try {
        std::lock_guard 锁(特征概念树互斥);
        特征需求集合条件比较结果 结果;
        const auto 类型 = 获取特征概念(类型根);
        const auto 路径 = 读取单概念路径(类型根);
        if (!类型 || !路径 || 路径->根 != 类型根
            || 类型->比较规则 != 特征概念比较规则::需求集合身份判等) return 结果;
        const auto 规范域 = 规范化待比较值域(特征概念材料物理类型::U64数组, 目标);
        if (!规范域) return 结果;
        const auto& 域 = std::get<特征概念需求集合值域>(*规范域);
        结果.状态 = 校验需求成员(域.必含需求节点组, 需求成员读取接口_);
        if (结果.状态 != 特征概念规则状态::已完成) return 结果;
        std::vector<稳定编码> 成员;
        结果.状态 = 读取需求集合材料(当前, 值服务, 需求成员读取接口_, 成员);
        if (结果.状态 != 特征概念规则状态::已完成) return 结果;
        std::set_difference(域.必含需求节点组.begin(), 域.必含需求节点组.end(),
            成员.begin(), 成员.end(), std::back_inserter(结果.缺少需求节点组));
        结果.缺少任意需求 = 域.至少一个需求 && 成员.empty();
        结果.满足 = 结果.缺少需求节点组.empty() && !结果.缺少任意需求 ? 1 : 0;
        return 结果;
    } catch (...) {
        特征需求集合条件比较结果 结果;
        结果.状态 = 特征概念规则状态::资源失败;
        return 结果;
    }
}

特征存在集合条件比较结果 概念_特征类::比较存在集合值域(
    稳定编码 类型根, const 特征概念准确值& 当前,
    const 特征概念存在集合值域& 目标,
    const 新_特征值类& 值服务) const noexcept {
    try {
        std::lock_guard 锁(特征概念树互斥);
        特征存在集合条件比较结果 结果;
        const auto 类型 = 获取特征概念(类型根);
        const auto 路径 = 读取单概念路径(类型根);
        if (!类型 || !路径 || 路径->根 != 类型根
            || 类型->比较规则 != 特征概念比较规则::存在集合身份判等) return 结果;
        const auto 规范域 = 规范化待比较值域(特征概念材料物理类型::U64数组, 目标);
        if (!规范域) return 结果;
        const auto& 域 = std::get<特征概念存在集合值域>(*规范域);
        结果.状态 = 校验存在成员(域.必含存在节点组, 存在成员读取接口_, &值服务);
        if (结果.状态 != 特征概念规则状态::已完成) return 结果;
        std::vector<稳定编码> 成员;
        结果.状态 = 读取存在集合材料(当前, 值服务, 存在成员读取接口_, 成员);
        if (结果.状态 != 特征概念规则状态::已完成) return 结果;
        std::set_difference(域.必含存在节点组.begin(), 域.必含存在节点组.end(),
            成员.begin(), 成员.end(), std::back_inserter(结果.缺少存在节点组));
        结果.缺少任意存在 = 域.至少一个存在 && 成员.empty();
        结果.满足 = 结果.缺少存在节点组.empty() && !结果.缺少任意存在 ? 1 : 0;
        return 结果;
    } catch (...) {
        特征存在集合条件比较结果 结果;
        结果.状态 = 特征概念规则状态::资源失败;
        return 结果;
    }
}

特征任务集合条件比较结果 概念_特征类::比较任务集合值域(
    const 特征概念准确值& 当前值,
    const 特征概念任务集合值域& 目标,
    const 新_特征值类& 特征值服务) const noexcept {
    try {
        std::lock_guard 锁(特征概念树互斥);
        const auto 失败 = [](特征概念规则状态 状态) {
            特征任务集合条件比较结果 结果;
            结果.状态 = 状态;
            return 结果;
        };
        const auto 规范域 = 规范化待比较值域(
            特征概念材料物理类型::U64数组, 特征概念值域{目标});
        if (!规范域) return 失败(特征概念规则状态::入口拒绝);
        const auto& 规范目标 = std::get<特征概念任务集合值域>(*规范域);
        任务需求身份组 目标身份;
        const auto 校验 = 校验集合目标身份(规范目标, 任务成员读取接口_, 目标身份);
        if (校验 != 特征概念规则状态::已完成) return 失败(校验);
        const auto* 材料节点 = std::get_if<稳定编码>(&当前值);
        if (!材料节点) return 失败(特征概念规则状态::材料类型不相容);
        const auto 材料 = 特征值服务.获取特征值(*材料节点);
        if (!材料) return 失败(特征概念规则状态::值不存在);
        const auto* 数组 = std::get_if<std::vector<std::uint64_t>>(&材料->材料);
        if (材料->物理类型 != 材料物理类型::U64数组 || !数组) {
            return 失败(特征概念规则状态::材料类型不相容);
        }
        std::vector<稳定编码> 当前任务;
        当前任务.reserve(数组->size());
        for (const auto 值 : *数组) 当前任务.push_back(稳定编码{值});
        std::sort(当前任务.begin(), 当前任务.end());
        当前任务.erase(std::unique(当前任务.begin(), 当前任务.end()), 当前任务.end());
        任务需求身份组 当前身份;
        for (const auto 任务 : 当前任务) {
            const auto 状态 = 读取真实任务身份(任务, 任务成员读取接口_, 当前身份);
            if (状态 != 特征概念规则状态::已完成) return 失败(状态);
        }
        特征任务集合条件比较结果 结果;
        for (const auto 任务 : 规范目标.必含任务节点组) {
            auto& 组 = std::binary_search(当前任务.begin(), 当前任务.end(), 任务)
                ? 结果.命中任务节点组 : 结果.缺少任务节点组;
            组.push_back(任务);
        }
        for (const auto 需求 : 规范目标.必含需求任务组) {
            const auto 命中 = std::find_if(当前身份.begin(), 当前身份.end(),
                [&](const auto& 项) { return 项.second == 需求; });
            if (命中 == 当前身份.end()) 结果.缺少需求节点组.push_back(需求);
            else 结果.命中任务节点组.push_back(命中->first);
        }
        for (const auto 确认 : 规范目标.必含服务要求任务组) {
            const auto 来源 = 任务成员读取接口_->读取服务要求对应需求(确认);
            const auto 状态 = 转换任务成员读取状态(来源.状态);
            if (状态 != 特征概念规则状态::已完成) return 失败(状态);
            if (来源.需求节点 && !有效(*来源.需求节点))
                return 失败(特征概念规则状态::内部不一致);
            const auto 命中 = std::find_if(当前身份.begin(), 当前身份.end(),
                [&](const auto& 项) { return 来源.需求节点 == 项.second; });
            if (命中 == 当前身份.end()) 结果.缺少服务要求节点组.push_back(确认);
            else 结果.命中任务节点组.push_back(命中->first);
        }
        结果.缺少任意任务 = 规范目标.至少一个任务 && 当前任务.empty();
        if (规范目标.至少一个任务 && !当前任务.empty()) {
            结果.命中任务节点组.push_back(当前任务.front());
        }
        std::sort(结果.命中任务节点组.begin(), 结果.命中任务节点组.end());
        结果.命中任务节点组.erase(std::unique(结果.命中任务节点组.begin(),
            结果.命中任务节点组.end()), 结果.命中任务节点组.end());
        结果.比较值 = 结果.缺少任务节点组.empty() && 结果.缺少需求节点组.empty()
            && 结果.缺少服务要求节点组.empty() && !结果.缺少任意任务 ? 1 : 0;
        结果.状态 = 特征概念规则状态::已完成;
        return 结果;
    } catch (...) {
        特征任务集合条件比较结果 结果;
        结果.状态 = 特征概念规则状态::资源失败;
        return 结果;
    }
}

bool 概念_特征类::初始化(const 新_特征值类& 特征值服务) noexcept {
    try {
        std::lock_guard 锁(特征概念树互斥);
        if (特征值读取服务_ && 特征值读取服务_ != &特征值服务) return false;
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
            && 建立(任务集合值域字段)
            && 建立(至少一个任务字段)
            && 建立(必含任务字段)
            && 建立(必含需求任务字段)
            && 建立(必含服务要求任务字段)
            && 建立(存在集合值域字段)
            && 建立(至少一个存在字段) && 建立(必含存在字段)
            && 建立(存在集合保护节点类型) && 建立(存在集合保护来源字段)
            && 建立(存在集合成员保护键)
            && 建立(需求集合值域字段)
            && 建立(至少一个需求字段)
            && 建立(必含需求字段)
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
                    || !结构定义相同(转为定义(*信息), *规范定义, &特征值服务, 需求成员读取接口_, 存在成员读取接口_)) {
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

        auto 服务任务定义 = 全材料定义(特征概念材料物理类型::U64数组);
        服务任务定义.比较规则 = 特征概念比较规则::服务任务集合身份判等;
        服务任务定义.聚合规则 = 特征概念聚合规则::不自动聚合;
        auto 需求集合定义 = 全材料定义(特征概念材料物理类型::U64数组);
        需求集合定义.比较规则 = 特征概念比较规则::需求集合身份判等;
        auto 存在集合定义 = 需求集合定义;
        存在集合定义.比较规则 = 特征概念比较规则::存在集合身份判等;
        const bool 完成 = 确保根概念(先天概念.三维空间坐标, 三维坐标定义)
            && 确保根概念(先天概念.三维空间尺寸, 三维尺寸定义)
            && 确保根概念(先天概念.RGB颜色, RGB定义)
            && 确保根概念(先天概念.二维轮廓,
                全材料定义(特征概念材料物理类型::二维二值格))
            && 确保根概念(先天概念.三维体素,
                全材料定义(特征概念材料物理类型::三维二值格))
            && 确保根概念(先天概念.服务任务集合, 服务任务定义)
            && 确保根概念(先天概念.服务对象需求集合, 需求集合定义)
            && 确保根概念(先天概念.交互对象集合, 存在集合定义);
        if (完成) 特征值读取服务_ = &特征值服务;
        return 完成;
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
            || !是特征概念节点(先天概念.三维体素)
            || !是特征概念节点(先天概念.服务任务集合)
            || !是特征概念节点(先天概念.服务对象需求集合)
            || !是特征概念节点(先天概念.交互对象集合)) {
            return std::nullopt;
        }
        return 先天概念;
    } catch (...) {
        return std::nullopt;
    }
}

特征概念编码操作结果 概念_特征类::建立或取得特征概念强类型(
    const 特征概念定义& d, const 新_特征值类& v, std::optional<稳定编码> p) noexcept {
    try {
        const auto n = 建立特征概念内部(d, v, p);
        return 有效(n) ? 特征概念编码操作结果{特征概念规则状态::已完成, n}
            : 特征概念编码操作结果{};
    } catch (const 特征运算技术拒绝& e) { return {e.状态, {}}; }
    catch (const std::bad_alloc&) { return {特征概念规则状态::资源失败, {}}; }
    catch (...) { return {特征概念规则状态::内部不一致, {}}; }
}
稳定编码 概念_特征类::建立或取得特征概念(
    const 特征概念定义& d, const 新_特征值类& v, std::optional<稳定编码> p) noexcept {
    try {
        std::lock_guard 锁(特征概念树互斥);
        if (d.比较规则 == 特征概念比较规则::存在集合身份判等 && !存在域已经保护(d.值域, v))
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"建立特征概念旧入口", L"未保护存在集合必须使用强类型入口。");
        const auto r = 建立或取得特征概念强类型(d, v, p);
        if (r.状态 == 特征概念规则状态::前提已变化)
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"建立特征概念旧入口", L"已保护内部路径出现未隔离的技术竞争。");
        return r.概念节点.value_or(稳定编码{});
    } catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"建立特征概念旧入口", L"保护结构读取异常。"); }
}
特征概念规则状态 概念_特征类::更新特征概念值域强类型(
    稳定编码 n, const 特征概念值域& d, const 新_特征值类& v) noexcept {
    try { return 更新特征概念值域内部(n, d, v) ? 特征概念规则状态::已完成 : 特征概念规则状态::入口拒绝; }
    catch (const 特征运算技术拒绝& e) { return e.状态; }
    catch (const std::bad_alloc&) { return 特征概念规则状态::资源失败; }
    catch (...) { return 特征概念规则状态::内部不一致; }
}
bool 概念_特征类::更新特征概念值域(
    稳定编码 n, const 特征概念值域& d, const 新_特征值类& v) noexcept {
    try {
        std::lock_guard 锁(特征概念树互斥);
        const auto c = 获取特征概念(n);
        if (c && c->比较规则 == 特征概念比较规则::存在集合身份判等 && !存在域已经保护(d, v))
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"更新特征概念旧入口", L"未保护存在集合必须使用强类型入口。");
        const auto r = 更新特征概念值域强类型(n, d, v);
        if (r == 特征概念规则状态::前提已变化)
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"更新特征概念旧入口", L"已保护内部路径出现未隔离的技术竞争。");
        return r == 特征概念规则状态::已完成;
    } catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"更新特征概念旧入口", L"保护结构读取异常。"); }
}
特征概念查询操作结果 概念_特征类::查询特征概念强类型(const 特征概念定义& d) const noexcept {
    try { return {特征概念规则状态::已完成, 查询特征概念内部(d)}; }
    catch (const 特征运算技术拒绝& e) { return {e.状态, {}}; }
    catch (const std::bad_alloc&) { return {特征概念规则状态::资源失败, {}}; }
    catch (...) { return {特征概念规则状态::内部不一致, {}}; }
}
std::vector<稳定编码> 概念_特征类::查询特征概念(const 特征概念定义& d) const noexcept {
    try {
        std::lock_guard 锁(特征概念树互斥);
        if (d.比较规则 == 特征概念比较规则::存在集合身份判等
            && (!特征值读取服务_ || !存在域已经保护(d.值域, *特征值读取服务_)))
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"查询特征概念旧入口", L"未保护输入必须使用强类型查询。");
        const auto r = 查询特征概念强类型(d);
        if (r.状态 != 特征概念规则状态::已完成)
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"查询特征概念旧入口", L"未完成查询不能折成无匹配。");
        return r.概念节点组;
    } catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"查询特征概念旧入口", L"保护结构读取异常。"); }
}

稳定编码 概念_特征类::建立特征概念内部(
    const 特征概念定义& 定义,
    const 新_特征值类& 特征值服务,
    std::optional<稳定编码> 上位概念) {
    try {
        std::lock_guard 锁(特征概念树互斥);
        if (!结构仍存在()) return {};
        const auto 规范定义 = 规范化定义(定义);
        if (!规范定义) return {};
        if (const auto* 域 = std::get_if<特征概念存在集合值域>(&规范定义->值域))
            要求运算完成(校验存在成员(域->必含存在节点组, 存在成员读取接口_, &特征值服务));
        if (const auto* 域 = std::get_if<特征概念需求集合值域>(&规范定义->值域)) {
            if (校验需求成员(域->必含需求节点组, 需求成员读取接口_)
                != 特征概念规则状态::已完成) return {};
        }
        if (const auto* 集合域 = std::get_if<特征概念任务集合值域>(&规范定义->值域)) {
            任务需求身份组 身份;
            if (校验集合目标身份(*集合域, 任务成员读取接口_, 身份)
                != 特征概念规则状态::已完成) return {};
        }
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
            if (规范定义->比较规则 == 特征概念比较规则::存在集合身份判等)
                for (const auto v : *值节点组)
                    要求运算完成(确保存在材料保护内部(v, 特征值服务, 存在成员读取接口_));
            if (规范定义->比较规则 == 特征概念比较规则::需求集合身份判等) {
                for (const auto 材料 : *值节点组) {
                    std::vector<稳定编码> 成员;
                    if (读取需求集合材料(材料, 特征值服务, 需求成员读取接口_, 成员)
                        != 特征概念规则状态::已完成) return {};
                }
            }
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
            if (父定义 && (规范定义->比较规则 == 特征概念比较规则::需求集合身份判等
                    || 规范定义->比较规则 == 特征概念比较规则::存在集合身份判等)
                && 结构定义相同(*父定义, *规范定义, &特征值服务, 需求成员读取接口_, 存在成员读取接口_))
                return 父节点;
            bool 是合法子域 = 父定义
                && (是严格子值域(*父定义, *规范定义, this, &特征值服务, 需求成员读取接口_, 存在成员读取接口_)
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
                if (子定义 && 结构定义相同(*子定义, *规范定义, &特征值服务, 需求成员读取接口_, 存在成员读取接口_)) {
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

        if (规范定义->比较规则 == 特征概念比较规则::存在集合身份判等) {
            基础节点组追加请求 q;
            基础组内新节点 d{编码存在集合域(规范定义->值域)};
            d.字段组.push_back({节点类型字段, 特征概念节点类型});
            d.字段组.push_back({材料类型字段, 基础值{static_cast<std::int64_t>(规范定义->材料类型)}});
            d.字段组.push_back({比较规则字段, 基础值{static_cast<std::int64_t>(规范定义->比较规则)}});
            d.字段组.push_back({聚合规则字段, 基础值{static_cast<std::int64_t>(规范定义->聚合规则)}});
            d.字段组.push_back({当前引用实例数字段, 基础值{std::int64_t{0}}});
            for (const auto n : 规范定义->名称关系) d.字段组.push_back({名称关系字段, n});
            q.新节点组.push_back(std::move(d));
            q.追加外部关系组.push_back({父节点, 基础组内节点引用{0}, 基础外部关系类型::父子, 0});
            const auto parent = 全局基础数据集.查询节点组完整结构({父节点});
            if (parent.状态 != 基础结构操作状态::已完成 || parent.节点组.size() != 1 || !parent.节点组[0].快照)
                return {};
            q.预期联合节点组.push_back(*parent.节点组[0].快照);
            if (const auto* c = std::get_if<特征概念存在集合值域>(&规范定义->值域)) {
                if (!存在成员读取接口_) throw 特征运算技术拒绝{特征概念规则状态::读取接口未装配};
                要求运算完成(追加存在集合保护写集(q, 基础组内节点引用{0},
                    c->必含存在节点组, *存在成员读取接口_, 特征值服务));
            }
            const auto r = 全局基础数据集.原子追加节点组(q);
            if (r.状态 == 基础节点组追加状态::预期不符) throw 特征运算技术拒绝{特征概念规则状态::前提已变化};
            if (r.状态 != 基础节点组追加状态::已完成 || r.新节点组.empty()) return {};
            const auto n = r.新节点组.front();
            const auto read = 获取特征概念(n);
            if (!read || 转为定义(*read) != *规范定义)
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"存在集合概念发布", L"原子发布后完整定义读回不一致。");
            return n;
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
            } else if (const auto* 需求域 =
                std::get_if<特征概念需求集合值域>(&规范定义->值域)) {
                完成 = 写I64字段(需求集合值域字段, 1)
                    && 写I64字段(至少一个需求字段, 需求域->至少一个需求 ? 1 : 0);
                for (const auto 需求 : 需求域->必含需求节点组) {
                    if (完成) 完成 = 写节点字段(必含需求字段, 需求);
                }
            } else if (const auto* 集合域 =
                std::get_if<特征概念任务集合值域>(&规范定义->值域)) {
                完成 = 写I64字段(任务集合值域字段, 1)
                    && 写I64字段(至少一个任务字段, 集合域->至少一个任务 ? 1 : 0);
                for (const auto 任务 : 集合域->必含任务节点组) {
                    if (完成) 完成 = 写节点字段(必含任务字段, 任务);
                }
                for (const auto 需求 : 集合域->必含需求任务组) {
                    if (完成) 完成 = 写节点字段(必含需求任务字段, 需求);
                }
                for (const auto 确认 : 集合域->必含服务要求任务组) {
                    if (完成) 完成 = 写节点字段(必含服务要求任务字段, 确认);
                }
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
    } catch (const 特征运算技术拒绝&) { throw; }
    catch (const 存在保护前提变化&) { throw 特征运算技术拒绝{特征概念规则状态::前提已变化}; }
    catch (...) {
        throw;
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
        const auto 集合字段组 = 全局基础数据集.查询字段(特征概念节点, 任务集合值域字段);
        const auto 任意任务字段组 = 全局基础数据集.查询字段(特征概念节点, 至少一个任务字段);
        const auto 指定任务字段组 = 全局基础数据集.查询字段(特征概念节点, 必含任务字段);
        const auto 需求任务字段组 = 全局基础数据集.查询字段(特征概念节点, 必含需求任务字段);
        const auto 服务要求任务字段组 = 全局基础数据集.查询字段(特征概念节点, 必含服务要求任务字段);
        const auto 需求集合字段组 = 全局基础数据集.查询字段(特征概念节点, 需求集合值域字段);
        const auto 任意需求字段组 = 全局基础数据集.查询字段(特征概念节点, 至少一个需求字段);
        const auto 指定需求字段组 = 全局基础数据集.查询字段(特征概念节点, 必含需求字段);
        const auto 存在集合字段组 = 全局基础数据集.查询字段(特征概念节点, 存在集合值域字段);
        const auto 任意存在字段组 = 全局基础数据集.查询字段(特征概念节点, 至少一个存在字段);
        const auto 指定存在字段组 = 全局基础数据集.查询字段(特征概念节点, 必含存在字段);
        if (存在集合字段组.empty() && (!任意存在字段组.empty() || !指定存在字段组.empty()))
            return std::nullopt;
        if (需求集合字段组.empty() && (!任意需求字段组.empty()
            || !指定需求字段组.empty())) return std::nullopt;
        if (集合字段组.empty() && (!任意任务字段组.empty()
            || !指定任务字段组.empty() || !需求任务字段组.empty()
            || !服务要求任务字段组.empty())) return std::nullopt;
        const auto 表示数量 = static_cast<int>(!I64字段组.empty())
            + static_cast<int>(!全材料字段组.empty())
            + static_cast<int>(!非I64字段组.empty())
            + static_cast<int>(!结构化字段组.empty())
            + static_cast<int>(!存在引用字段组.empty())
            + static_cast<int>(!集合字段组.empty())
            + static_cast<int>(!需求集合字段组.empty())
            + static_cast<int>(!存在集合字段组.empty());
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
        } else if (!存在集合字段组.empty()) {
            const auto 标记 = 读取唯一I64字段(特征概念节点, 存在集合值域字段);
            const auto 任意 = 读取唯一I64字段(特征概念节点, 至少一个存在字段);
            if (!标记 || *标记 != 1 || !任意 || (*任意 != 0 && *任意 != 1)) return {};
            特征概念存在集合值域 域{读取多节点字段(特征概念节点, 必含存在字段), *任意 != 0};
            if (域.必含存在节点组.size() != 指定存在字段组.size()) return {};
            定义.值域 = std::move(域);
        } else if (!需求集合字段组.empty()) {
            const auto 标记 = 读取唯一I64字段(特征概念节点, 需求集合值域字段);
            const auto 任意 = 读取唯一I64字段(特征概念节点, 至少一个需求字段);
            if (!标记 || *标记 != 1 || !任意 || (*任意 != 0 && *任意 != 1))
                return std::nullopt;
            特征概念需求集合值域 域{读取多节点字段(特征概念节点, 必含需求字段), *任意 != 0};
            if (域.必含需求节点组.size() != 指定需求字段组.size()) return std::nullopt;
            定义.值域 = std::move(域);
        } else if (!集合字段组.empty()) {
            const auto 标记 = 读取唯一I64字段(特征概念节点, 任务集合值域字段);
            const auto 任意 = 读取唯一I64字段(特征概念节点, 至少一个任务字段);
            if (!标记 || *标记 != 1 || !任意 || (*任意 != 0 && *任意 != 1)) {
                return std::nullopt;
            }
            特征概念任务集合值域 集合;
            集合.至少一个任务 = *任意 != 0;
            集合.必含任务节点组 = 读取多节点字段(特征概念节点, 必含任务字段);
            集合.必含需求任务组 = 读取多节点字段(特征概念节点, 必含需求任务字段);
            集合.必含服务要求任务组 = 读取多节点字段(特征概念节点, 必含服务要求任务字段);
            if (集合.必含任务节点组.size() != 指定任务字段组.size()
                || 集合.必含需求任务组.size() != 需求任务字段组.size()
                || 集合.必含服务要求任务组.size() != 服务要求任务字段组.size()) return std::nullopt;
            for (const auto* 组 : {&集合.必含任务节点组, &集合.必含需求任务组,
                    &集合.必含服务要求任务组}) {
                if (std::any_of(组->begin(), 组->end(),
                    [](稳定编码 节点) { return !节点仍存在(节点); })) return std::nullopt;
            }
            定义.值域 = std::move(集合);
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
        if (规范定义->比较规则 == 特征概念比较规则::存在集合身份判等) {
            try {
            if (const auto* values = std::get_if<std::vector<稳定编码>>(&规范定义->值域)) {
                if (!特征值读取服务_ || !存在域已经保护(*values, *特征值读取服务_))
                    工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取存在集合概念",
                        L"已发布有限域缺少完整材料保护。");
            } else if (const auto* domain = std::get_if<特征概念存在集合值域>(&规范定义->值域)) {
                const auto g = 读取存在保护(特征概念节点);
                if (!g.快照 || 保护成员组(g) != domain->必含存在节点组)
                    工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取存在集合概念",
                        L"已发布正向域与保护不一致。");
            }
            } catch (...) {
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取存在集合概念",
                    L"正式域保护读取异常，不能降格为概念不存在。");
            }
        }
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

bool 概念_特征类::更新特征概念值域内部(
    稳定编码 特征概念节点,
    const 特征概念值域& 新值域,
    const 新_特征值类& 特征值服务) {
    try {
        std::lock_guard 锁(特征概念树互斥);
        const auto 原信息 = 获取特征概念(特征概念节点);
        if (!原信息) return false;

        auto 新定义 = 转为定义(*原信息);
        新定义.值域 = 新值域;
        const auto 规范新定义 = 规范化定义(新定义);
        const auto 规范原定义 = 规范化定义(转为定义(*原信息));
        if (!规范新定义 || !规范原定义) return false;
        if (const auto* c = std::get_if<特征概念存在集合值域>(&规范新定义->值域))
            要求运算完成(校验存在成员(c->必含存在节点组, 存在成员读取接口_, &特征值服务));
        if (const auto* 域 = std::get_if<特征概念需求集合值域>(&规范新定义->值域)) {
            if (校验需求成员(域->必含需求节点组, 需求成员读取接口_)
                != 特征概念规则状态::已完成) return false;
        }
        if (const auto* 集合域 = std::get_if<特征概念任务集合值域>(&规范新定义->值域)) {
            任务需求身份组 身份;
            if (校验集合目标身份(*集合域, 任务成员读取接口_, 身份)
                != 特征概念规则状态::已完成) return false;
        }
        if (const auto* 引用域 =
            std::get_if<特征概念存在引用值域>(&规范新定义->值域)) {
            if (!存在引用读取接口_
                || !存在引用读取接口_->是存在概念节点(
                    引用域->存在概念节点)) {
                return false;
            }
        }
        if (结构定义相同(*规范新定义, *规范原定义, &特征值服务, 需求成员读取接口_, 存在成员读取接口_)) return true;

        if (const auto* 值节点组 =
            std::get_if<std::vector<稳定编码>>(&规范新定义->值域)) {
            if (规范新定义->比较规则 == 特征概念比较规则::存在集合身份判等)
                for (const auto v : *值节点组)
                    要求运算完成(确保存在材料保护内部(v, 特征值服务, 存在成员读取接口_));
            if (规范新定义->比较规则 == 特征概念比较规则::需求集合身份判等) {
                for (const auto 材料 : *值节点组) {
                    std::vector<稳定编码> 成员;
                    if (读取需求集合材料(材料, 特征值服务, 需求成员读取接口_, 成员)
                        != 特征概念规则状态::已完成) return false;
                }
            }
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
                && (是严格子值域(*父定义, *规范新定义, this, &特征值服务, 需求成员读取接口_, 存在成员读取接口_)
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
                && (是严格子值域(*规范新定义, *子定义, this, &特征值服务, 需求成员读取接口_, 存在成员读取接口_)
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

        if (规范新定义->比较规则 == 特征概念比较规则::存在集合身份判等) {
            if (!存在成员读取接口_) return false;
            基础节点组追加请求 q;
            const auto before = 全局基础数据集.查询节点组完整结构({特征概念节点});
            if (before.状态 != 基础结构操作状态::已完成 || before.节点组.size() != 1 || !before.节点组[0].快照)
                return false;
            q.预期联合节点组.push_back(*before.节点组[0].快照);
            for (const auto& f : before.节点组[0].快照->结构.字段组)
                if (f.字段节点 == 存在集合值域字段 || f.字段节点 == 至少一个存在字段
                    || f.字段节点 == 必含存在字段 || f.字段节点 == 全材料值域字段 || f.字段节点 == 非I64值域字段)
                    q.删除字段关系组.push_back(f.编码);
            for (const auto& f : 编码存在集合域(规范新定义->值域))
                q.追加字段组.push_back({特征概念节点, f.字段节点, f.内容});
            const auto* c = std::get_if<特征概念存在集合值域>(&规范新定义->值域);
            const auto p = 追加存在集合保护写集(q, 特征概念节点,
                c ? c->必含存在节点组 : std::vector<稳定编码>{}, *存在成员读取接口_, 特征值服务);
            要求运算完成(p);
            const auto r = 全局基础数据集.原子追加节点组(q);
            if (r.状态 == 基础节点组追加状态::预期不符) throw 特征运算技术拒绝{特征概念规则状态::前提已变化};
            if (r.状态 != 基础节点组追加状态::已完成) return false;
            const auto back = 获取特征概念(特征概念节点);
            const auto g = 读取存在保护(特征概念节点);
            if (!back || 转为定义(*back) != *规范新定义 || !g.快照)
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"存在集合域更新", L"提交后定义或保护读回失败。");
            return true;
        }
        if (std::holds_alternative<特征概念需求集合值域>(规范新定义->值域)
            || std::holds_alternative<特征概念需求集合值域>(规范原定义->值域)
            || std::holds_alternative<特征概念任务集合值域>(规范新定义->值域)
            || std::holds_alternative<特征概念任务集合值域>(规范原定义->值域)) {
            // 仅在本owner锁内替换完整域字段，沿用既有失败回滚合同。
            std::vector<基础节点初始字段> 新字段;
            if (const auto* 域 = std::get_if<特征概念需求集合值域>(&规范新定义->值域)) {
                新字段.push_back({需求集合值域字段, 基础值{基础原始值{std::int64_t{1}}}});
                新字段.push_back({至少一个需求字段, 基础值{基础原始值{std::int64_t{域->至少一个需求 ? 1 : 0}}}});
                for (const auto 需求 : 域->必含需求节点组) 新字段.push_back({必含需求字段, 需求});
            } else if (const auto* 集合 = std::get_if<特征概念任务集合值域>(&规范新定义->值域)) {
                新字段.push_back({任务集合值域字段, 基础值{基础原始值{std::int64_t{1}}}});
                新字段.push_back({至少一个任务字段, 基础值{基础原始值{
                    std::int64_t{集合->至少一个任务 ? 1 : 0}}}});
                for (const auto 任务 : 集合->必含任务节点组) 新字段.push_back({必含任务字段, 任务});
                for (const auto 需求 : 集合->必含需求任务组) 新字段.push_back({必含需求任务字段, 需求});
                for (const auto 确认 : 集合->必含服务要求任务组) 新字段.push_back({必含服务要求任务字段, 确认});
            } else if (std::holds_alternative<特征概念全材料值域>(规范新定义->值域)) {
                新字段.push_back({全材料值域字段, 基础值{基础原始值{std::int64_t{1}}}});
            } else if (const auto* 材料组 = std::get_if<std::vector<稳定编码>>(&规范新定义->值域)) {
                for (const auto 材料 : *材料组) 新字段.push_back({非I64值域字段, 材料});
            } else return false;
            std::vector<基础字段关系> 原字段;
            for (const auto 字段 : {任务集合值域字段, 至少一个任务字段,
                    需求集合值域字段, 至少一个需求字段, 必含需求字段,
                    必含任务字段, 必含需求任务字段, 必含服务要求任务字段,
                    全材料值域字段, 非I64值域字段}) {
                const auto 组 = 全局基础数据集.查询字段(特征概念节点, 字段);
                原字段.insert(原字段.end(), 组.begin(), 组.end());
            }
            const auto 写字段 = [&](稳定编码 字段, const 基础字段内容& 内容) {
                if (const auto* 节点 = std::get_if<稳定编码>(&内容)) {
                    return 全局基础数据集.添加字段节点(特征概念节点, 字段, *节点);
                }
                return 全局基础数据集.添加字段值(特征概念节点, 字段, std::get<基础值>(内容));
            };
            std::vector<稳定编码> 新增;
            新增.reserve(新字段.size());
            std::size_t 已删 = 0;
            const auto 回滚 = [&]() {
                for (const auto 字段 : 新增) (void)全局基础数据集.删除字段(字段);
                for (std::size_t i = 0; i < 已删; ++i) {
                    (void)写字段(原字段[i].字段节点, 原字段[i].内容);
                }
            };
            for (const auto& 字段 : 新字段) {
                const auto 编码 = 写字段(字段.字段节点, 字段.内容);
                if (!有效(编码)) { 回滚(); return false; }
                新增.push_back(编码);
            }
            for (const auto& 字段 : 原字段) {
                if (!全局基础数据集.删除字段(字段.编码)) { 回滚(); return false; }
                ++已删;
            }
            const auto 读回 = 获取特征概念(特征概念节点);
            if (读回 && 读回->值域 == 规范新定义->值域) return true;
            回滚();
            return false;
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
    } catch (const 特征运算技术拒绝&) { throw; }
    catch (const 存在保护前提变化&) { throw 特征运算技术拒绝{特征概念规则状态::前提已变化}; }
    catch (...) {
        throw;
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

bool 概念_特征类::绑定名称关系(
    稳定编码 概念节点, 稳定编码 名称关系节点) noexcept {
    try {
        std::lock_guard 锁(特征概念树互斥);
        if (!结构仍存在() || !是特征概念节点(概念节点)
            || !节点仍存在(名称关系节点)) {
            return false;
        }
        const auto 字段组 = 全局基础数据集.查询字段(
            概念节点, 名称关系字段);
        std::size_t 命中数 = 0;
        for (const auto& 字段 : 字段组) {
            const auto* 目标 = std::get_if<稳定编码>(&字段.内容);
            if (!目标) return false;
            if (*目标 == 名称关系节点) ++命中数;
        }
        if (命中数 == 1) return true;
        if (命中数 != 0) return false;
        const auto 新字段 = 全局基础数据集.添加字段节点(
            概念节点, 名称关系字段, 名称关系节点);
        if (!有效(新字段)) return false;
        const auto 读回 = 全局基础数据集.查询字段(
            概念节点, 名称关系字段);
        const auto 读回命中 = std::count_if(
            读回.begin(), 读回.end(), [&](const auto& 字段) {
                const auto* 目标 = std::get_if<稳定编码>(&字段.内容);
                return 目标 && *目标 == 名称关系节点;
            });
        if (读回命中 == 1) return true;
        (void)全局基础数据集.删除字段(新字段);
        return false;
    } catch (...) {
        return false;
    }
}

bool 概念_特征类::解除名称关系(
    稳定编码 概念节点, 稳定编码 名称关系节点) noexcept {
    try {
        std::lock_guard 锁(特征概念树互斥);
        if (!结构仍存在() || !是特征概念节点(概念节点)) return false;
        const auto 字段组 = 全局基础数据集.查询字段(
            概念节点, 名称关系字段);
        std::optional<稳定编码> 待删字段;
        for (const auto& 字段 : 字段组) {
            const auto* 目标 = std::get_if<稳定编码>(&字段.内容);
            if (!目标) return false;
            if (*目标 != 名称关系节点) continue;
            if (待删字段) return false;
            待删字段 = 字段.编码;
        }
        return 待删字段 && 全局基础数据集.删除字段(*待删字段);
    } catch (...) {
        return false;
    }
}

特征概念直接子查询结果 概念_特征类::查询直接子特征概念(
    稳定编码 直接父概念, const 特征概念定义& 完整定义) const {
    try {
    std::lock_guard 锁(特征概念树互斥);
    if (!有效(直接父概念)) return {};
    const auto 规范定义 = 规范化定义(完整定义);
    if (!规范定义) return {};
    if (!结构仍存在()) {
        return {特征概念直接子查询状态::工程未完成, {}};
    }
    const auto 父信息 = 获取特征概念(直接父概念);
    if (!父信息) {
        return {特征概念直接子查询状态::工程未完成, {}};
    }
    if (父信息->节点 != 直接父概念) {
        return {特征概念直接子查询状态::内部不一致, {}};
    }

    const auto 直接子关系 = 全局基础数据集.查询源关系(
        直接父概念, 基础外部关系类型::父子);
    std::vector<稳定编码> 已核对子节点;
    已核对子节点.reserve(直接子关系.size());
    std::vector<稳定编码> 匹配节点组;
    for (const auto& 关系 : 直接子关系) {
        if (!有效(关系.编码) || 关系.源节点 != 直接父概念
            || !有效(关系.目标节点) || 关系.目标节点 == 直接父概念
            || 关系.类型 != 基础外部关系类型::父子 || 关系.角色或顺序 != 0
            || std::find(已核对子节点.begin(), 已核对子节点.end(), 关系.目标节点)
                != 已核对子节点.end()) {
            return {特征概念直接子查询状态::内部不一致, {}};
        }
        已核对子节点.push_back(关系.目标节点);
        const auto 反向父关系 = 全局基础数据集.查询目标关系(
            关系.目标节点, 基础外部关系类型::父子);
        if (反向父关系.size() != 1 || 反向父关系.front() != 关系) {
            return {特征概念直接子查询状态::内部不一致, {}};
        }
        const auto 子信息 = 获取特征概念(关系.目标节点);
        if (!子信息) {
            return {特征概念直接子查询状态::工程未完成, {}};
        }
        if (子信息->节点 != 关系.目标节点) {
            return {特征概念直接子查询状态::内部不一致, {}};
        }
        const auto 子定义 = 规范化定义(转为定义(*子信息));
        if (!子定义) {
            return {特征概念直接子查询状态::内部不一致, {}};
        }
        if (结构定义相同(*子定义, *规范定义, 特征值读取服务_, 需求成员读取接口_, 存在成员读取接口_)) {
            匹配节点组.push_back(关系.目标节点);
        }
    }
    std::sort(匹配节点组.begin(), 匹配节点组.end());
    return {特征概念直接子查询状态::已完成, std::move(匹配节点组)};
    } catch (const 特征运算技术拒绝& e) {
        return {e.状态 == 特征概念规则状态::前提已变化
            ? 特征概念直接子查询状态::前提已变化 : 特征概念直接子查询状态::工程未完成, {}};
    }
}

std::vector<稳定编码> 概念_特征类::查询特征概念内部(
    const 特征概念定义& 定义) const {
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
                if (候选 && 结构定义相同(*候选, *规范定义, 特征值读取服务_, 需求成员读取接口_, 存在成员读取接口_)) {
                    结果.push_back(子节点);
                }
            }
        }
        std::sort(结果.begin(), 结果.end());
        结果.erase(std::unique(结果.begin(), 结果.end()), 结果.end());
        return 结果;
    } catch (const 特征运算技术拒绝&) { throw; }
    catch (const 存在保护前提变化&) { throw 特征运算技术拒绝{特征概念规则状态::前提已变化}; }
    catch (...) {
        throw;
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

        if (根信息->比较规则 == 特征概念比较规则::需求集合身份判等
            || 根信息->比较规则 == 特征概念比较规则::存在集合身份判等) {
            std::vector<稳定编码> 成员;
            const auto 状态 = 根信息->比较规则 == 特征概念比较规则::存在集合身份判等
                ? 读取存在集合材料(特征值, 特征值服务, 存在成员读取接口_, 成员)
                : 读取需求集合材料(特征值, 特征值服务, 需求成员读取接口_, 成员);
            if (状态 != 特征概念规则状态::已完成) {
                结果.状态 = 状态 == 特征概念规则状态::资源失败
                    ? 特征概念查找状态::资源失败 : 状态 == 特征概念规则状态::前提已变化
                        ? 特征概念查找状态::前提已变化 : 特征概念查找状态::结构不一致;
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
            std::optional<std::uint64_t> 当前尺度;
            if (const auto* 存在域 = std::get_if<特征概念存在集合值域>(&当前信息->值域)) {
                const auto 比较 = 比较存在集合值域(特征类型概念, 特征值, *存在域, 特征值服务);
                if (比较.状态 != 特征概念规则状态::已完成) {
                    结果.状态 = 比较.状态 == 特征概念规则状态::资源失败
                        ? 特征概念查找状态::资源失败 : 比较.状态 == 特征概念规则状态::前提已变化
                            ? 特征概念查找状态::前提已变化 : 特征概念查找状态::结构不一致;
                    return 结果;
                }
                if (比较.满足 == 1)
                    当前尺度 = (std::numeric_limits<std::uint64_t>::max)() - 1;
            } else if (const auto* 需求域 = std::get_if<特征概念需求集合值域>(&当前信息->值域)) {
                const auto 比较 = 比较需求集合值域(特征类型概念, 特征值, *需求域, 特征值服务);
                if (比较.状态 != 特征概念规则状态::已完成) {
                    结果.状态 = 比较.状态 == 特征概念规则状态::资源失败
                        ? 特征概念查找状态::资源失败 : 特征概念查找状态::结构不一致;
                    return 结果;
                }
                if (比较.满足 == 1)
                    当前尺度 = (std::numeric_limits<std::uint64_t>::max)() - 1;
            } else if (const auto* 集合域 = std::get_if<特征概念任务集合值域>(&当前信息->值域)) {
                const auto 比较 = 比较任务集合值域(特征值, *集合域, 特征值服务);
                if (比较.状态 != 特征概念规则状态::已完成 || !比较.比较值) {
                    结果.状态 = 比较.状态 == 特征概念规则状态::资源失败
                        ? 特征概念查找状态::资源失败
                        : 比较.状态 == 特征概念规则状态::读取接口未装配
                            ? 特征概念查找状态::入口拒绝
                            : 特征概念查找状态::结构不一致;
                    return 结果;
                }
                if (*比较.比较值 == 1) {
                    // 开放集合约束不以成员数量当作域宽度；严格子链裁决具体性。
                    当前尺度 = (std::numeric_limits<std::uint64_t>::max)() - 1;
                }
            } else {
                当前尺度 = 值域匹配尺度(
                    *当前信息, 特征值, 特征值服务, 存在引用读取接口_, 需求成员读取接口_, 存在成员读取接口_);
            }
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
                    && (是严格子值域(*当前定义, *子定义, this, &特征值服务, 需求成员读取接口_, 存在成员读取接口_)
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
    } catch (const 特征运算技术拒绝& e) {
        结果.状态 = e.状态 == 特征概念规则状态::前提已变化
            ? 特征概念查找状态::前提已变化 : 特征概念查找状态::结构不一致;
        结果.概念节点.reset();
        结果.冲突候选.clear();
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

        if (规则 == 特征概念比较规则::需求集合身份判等
            || 规则 == 特征概念比较规则::存在集合身份判等) {
            if (材料类型 != 特征概念材料物理类型::U64数组) {
                结果.状态 = 特征概念规则状态::材料类型不相容;
                return 结果;
            }
            std::vector<稳定编码> 左成员, 右成员;
            结果.状态 = 规则 == 特征概念比较规则::存在集合身份判等
                ? 读取存在集合材料(左值, 特征值服务, 存在成员读取接口_, 左成员)
                : 读取需求集合材料(左值, 特征值服务, 需求成员读取接口_, 左成员);
            if (结果.状态 != 特征概念规则状态::已完成) return 结果;
            结果.状态 = 规则 == 特征概念比较规则::存在集合身份判等
                ? 读取存在集合材料(右值, 特征值服务, 存在成员读取接口_, 右成员)
                : 读取需求集合材料(右值, 特征值服务, 需求成员读取接口_, 右成员);
            if (结果.状态 != 特征概念规则状态::已完成) return 结果;
            结果.相等 = 左成员 == 右成员;
            if (*结果.相等) 结果.次序 = 特征概念比较次序::相等;
            return 结果;
        }
        const bool 服务集合判等 = 规则 == 特征概念比较规则::服务任务集合身份判等;
        if (服务集合判等 && 材料类型 != 特征概念材料物理类型::U64数组) {
            结果.状态 = 特征概念规则状态::材料类型不相容;
            return 结果;
        }
        if ((!服务集合判等 && 规则 != 特征概念比较规则::完整材料精确)
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
        if (服务集合判等) {
            const auto* 左材料 = std::get_if<std::vector<std::uint64_t>>(&左信息->材料);
            const auto* 右材料 = std::get_if<std::vector<std::uint64_t>>(&右信息->材料);
            if (!左材料 || !右材料) {
                结果.状态 = 特征概念规则状态::材料类型不相容;
                return 结果;
            }
            auto 左成员 = *左材料;
            auto 右成员 = *右材料;
            const auto 规范成员 = [](auto& 成员) {
                std::sort(成员.begin(), 成员.end());
                return (成员.empty() || 成员.front() != 0)
                    && std::adjacent_find(成员.begin(), 成员.end()) == 成员.end();
            };
            if (!规范成员(左成员) || !规范成员(右成员)) {
                结果.状态 = 特征概念规则状态::内部不一致;
                return 结果;
            }
            结果.相等 = 左成员 == 右成员;
        } else {
            结果.相等 = 左信息->材料 == 右信息->材料;
        }
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
    稳定编码 根, const 特征概念值域& 左, const 特征概念值域& 右) const noexcept {
    try {
        std::lock_guard 锁(特征概念树互斥);
        const auto 定义 = 获取特征概念(根);
        const auto 父 = 全局基础数据集.查询目标关系(根, 基础外部关系类型::父子);
        if (!定义 || 父.size() != 1 || 父.front().源节点 != 特征概念树) return {};
        if (定义->比较规则 == 特征概念比较规则::需求集合身份判等)
            return 需求规则域关系(转为定义(*定义), 左, 右, 特征值读取服务_, 需求成员读取接口_);
        if (定义->比较规则 == 特征概念比较规则::存在集合身份判等)
            return 存在规则域关系(转为定义(*定义), 左, 右, 特征值读取服务_, 存在成员读取接口_);
        return 比较值域(定义->材料类型, 左, 右);
    } catch (...) { 特征概念值域比较结果 r; r.状态 = 特征概念规则状态::资源失败; return r; }
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
        if (std::holds_alternative<特征概念存在集合值域>(*左)
            || std::holds_alternative<特征概念存在集合值域>(*右)) {
            特征概念定义 d;
            d.材料类型 = 材料类型;
            d.比较规则 = 特征概念比较规则::存在集合身份判等;
            return 存在规则域关系(d, *左, *右, 特征值读取服务_, 存在成员读取接口_);
        }
        const auto* 左需求 = std::get_if<特征概念需求集合值域>(&*左);
        const auto* 右需求 = std::get_if<特征概念需求集合值域>(&*右);
        if (左需求 || 右需求) {
            for (const auto* 域 : {左需求, 右需求}) {
                if (!域) continue;
                结果.状态 = 校验需求成员(域->必含需求节点组, 需求成员读取接口_);
                if (结果.状态 != 特征概念规则状态::已完成) return 结果;
            }
            if (左需求 && 右需求) {
                const bool 左含右 = 需求约束蕴含(*右需求, *左需求);
                const bool 右含左 = 需求约束蕴含(*左需求, *右需求);
                结果.关系 = 左含右 && 右含左 ? 特征概念值域关系::相等
                    : 左含右 ? 特征概念值域关系::左包含右
                    : 右含左 ? 特征概念值域关系::右包含左 : 特征概念值域关系::相交;
            } else {
                const auto& 域 = 左需求 ? *左需求 : *右需求;
                const auto& 另一域 = 左需求 ? *右 : *左;
                if (std::holds_alternative<特征概念全材料值域>(另一域)) {
                    结果.关系 = !域.至少一个需求 && 域.必含需求节点组.empty()
                        ? 特征概念值域关系::相等
                        : 左需求 ? 特征概念值域关系::右包含左 : 特征概念值域关系::左包含右;
                } else if (const auto* 材料组 = std::get_if<std::vector<稳定编码>>(&另一域)) {
                    if (!特征值读取服务_) {
                        结果.状态 = 特征概念规则状态::读取接口未装配;
                        return 结果;
                    }
                    std::size_t 命中 = 0;
                    for (const auto 材料 : *材料组) {
                        std::vector<稳定编码> 成员;
                        结果.状态 = 读取需求集合材料(材料, *特征值读取服务_, 需求成员读取接口_, 成员);
                        if (结果.状态 != 特征概念规则状态::已完成) return 结果;
                        if ((!域.至少一个需求 || !成员.empty())
                            && std::includes(成员.begin(), 成员.end(),
                                域.必含需求节点组.begin(), 域.必含需求节点组.end())) ++命中;
                    }
                    结果.关系 = 命中 == 材料组->size()
                        ? (左需求 ? 特征概念值域关系::左包含右 : 特征概念值域关系::右包含左)
                        : 命中 ? 特征概念值域关系::相交 : 特征概念值域关系::分离;
                } else {
                    结果.状态 = 特征概念规则状态::材料类型不相容;
                    return 结果;
                }
            }
            结果.状态 = 特征概念规则状态::已完成;
            return 结果;
        }
        const auto* 左集合 = std::get_if<特征概念任务集合值域>(&*左);
        const auto* 右集合 = std::get_if<特征概念任务集合值域>(&*右);
        if (左集合 || 右集合) {
            任务需求身份组 左身份, 右身份;
            for (const auto& 项 : {std::pair{左集合, &左身份}, std::pair{右集合, &右身份}}) {
                if (!项.first) continue;
                结果.状态 = 校验集合目标身份(*项.first, 任务成员读取接口_, *项.second);
                if (结果.状态 != 特征概念规则状态::已完成) return 结果;
            }
            if (左集合 && 右集合) {
                auto 左展开 = *左集合;
                auto 右展开 = *右集合;
                std::vector<std::pair<稳定编码, 特征任务成员读取结果>> 本次来源组;
                for (auto* 域 : {&左展开, &右展开}) {
                    结果.状态 = 展开集合确认来源(*域, 任务成员读取接口_, 本次来源组);
                    if (结果.状态 != 特征概念规则状态::已完成) return 结果;
                }
                const bool 左含右 = 任务集合约束蕴含(右展开, 右身份, 左展开, 左身份);
                const bool 右含左 = 任务集合约束蕴含(左展开, 左身份, 右展开, 右身份);
                结果.关系 = 左含右 && 右含左 ? 特征概念值域关系::相等
                    : 左含右 ? 特征概念值域关系::左包含右
                    : 右含左 ? 特征概念值域关系::右包含左
                    : 特征概念值域关系::相交;
            } else {
                const auto& 另一域 = 左集合 ? *右 : *左;
                if (std::holds_alternative<特征概念全材料值域>(另一域)) {
                    结果.关系 = 左集合 ? 特征概念值域关系::右包含左
                        : 特征概念值域关系::左包含右;
                } else if (const auto* 材料组 = std::get_if<std::vector<稳定编码>>(&另一域)) {
                    if (!特征值读取服务_) {
                        结果.状态 = 特征概念规则状态::读取接口未装配;
                        return 结果;
                    }
                    std::size_t 命中数 = 0;
                    for (const auto 材料 : *材料组) {
                        const auto 比较 = 比较任务集合值域(特征概念准确值{材料},
                            左集合 ? *左集合 : *右集合, *特征值读取服务_);
                        if (比较.状态 != 特征概念规则状态::已完成 || !比较.比较值) {
                            结果.状态 = 比较.状态 == 特征概念规则状态::已完成
                                ? 特征概念规则状态::内部不一致 : 比较.状态;
                            return 结果;
                        }
                        if (*比较.比较值 == 1) ++命中数;
                    }
                    结果.关系 = 命中数 == 材料组->size()
                        ? (左集合 ? 特征概念值域关系::左包含右 : 特征概念值域关系::右包含左)
                        : 命中数 != 0 ? 特征概念值域关系::相交 : 特征概念值域关系::分离;
                } else {
                    结果.状态 = 特征概念规则状态::材料类型不相容;
                    return 结果;
                }
            }
            结果.状态 = 特征概念规则状态::已完成;
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
    稳定编码 根, const std::vector<特征概念准确值>& 历史, const 新_特征值类& 值服务) const noexcept {
    try {
        std::lock_guard 锁(特征概念树互斥);
        const auto d = 获取特征概念(根);
        const auto 父 = 全局基础数据集.查询目标关系(根, 基础外部关系类型::父子);
        if (!d || 父.size() != 1 || 父.front().源节点 != 特征概念树) return {};
        if (d->比较规则 != 特征概念比较规则::需求集合身份判等
            && d->比较规则 != 特征概念比较规则::存在集合身份判等)
            return 执行聚合规则(d->材料类型, d->聚合规则, 历史, 值服务);
        特征概念聚合结果 r;
        if (历史.empty()) return r;
        std::vector<稳定编码> 材料;
        for (const auto& v : 历史) {
            const auto* n = std::get_if<稳定编码>(&v);
            if (!n) { r.状态 = 特征概念规则状态::材料类型不相容; return r; }
            材料.push_back(*n);
        }
        需求快照签名组 签名;
        r.状态 = d->比较规则 == 特征概念比较规则::存在集合身份判等
            ? 规范存在快照(材料, &值服务, 存在成员读取接口_, 签名)
            : 规范需求快照(材料, &值服务, 需求成员读取接口_, 签名);
        if (r.状态 != 特征概念规则状态::已完成) return r;
        材料.clear();
        for (const auto& [s, n] : 签名) { (void)s; 材料.push_back(n); }
        std::sort(材料.begin(), 材料.end());
        r.值域 = std::move(材料); return r;
    } catch (...) { 特征概念聚合结果 r; r.状态 = 特征概念规则状态::资源失败; return r; }
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

特征概念定义保护结果 概念_特征类::读取特征概念定义保护(稳定编码 节点) const noexcept {
    try {
        std::lock_guard 锁(特征概念树互斥);
        std::set<稳定编码> 编码组;
        std::map<稳定编码, 基础联合节点快照> 枚举;
        稳定编码 根{};
        auto 当前 = 节点;
        for (;;) {
            if (!编码组.insert(当前).second) return {基础结构操作状态::内部不一致, {}, {}, false};
            const auto b = 全局基础数据集.查询节点组完整结构({当前});
            if (b.状态 != 基础结构操作状态::已完成) return {b.状态, {}, {}, false};
            if (b.节点组.size() != 1 || !b.节点组.front().快照)
                return {基础结构操作状态::节点不存在, {}, {}, false};
            const auto& s = *b.节点组.front().快照;
            枚举.emplace(当前, s);
            std::vector<稳定编码> 父;
            for (const auto& r : s.入向外部关系组)
                if (r.类型 == 基础外部关系类型::父子) 父.push_back(r.源节点);
            if (父.size() != 1) return {基础结构操作状态::内部不一致, {}, {}, false};
            // 只沿owner定义边取得分量、单位、有限材料；不沿名称或实例入向引用。
            for (const auto& f : s.结构.字段组)
                if (f.字段节点 == 结构化分量字段 || f.字段节点 == 非I64值域字段
                    || f.字段节点 == 单位字段 || f.字段节点 == 存在引用值域字段
                    || f.字段节点 == 必含需求字段 || f.字段节点 == 必含任务字段
                    || f.字段节点 == 必含需求任务字段 || f.字段节点 == 必含服务要求任务字段) {
                    const auto* n = std::get_if<稳定编码>(&f.内容);
                    if (!n || !有效(*n)) return {基础结构操作状态::内部不一致, {}, {}, false};
                    编码组.insert(*n);
                }
            if (父.front() == 特征概念树) { 根 = 当前; break; }
            当前 = 父.front();
        }
        const auto 批 = 全局基础数据集.查询节点组完整结构({编码组.begin(), 编码组.end()});
        if (批.状态 != 基础结构操作状态::已完成) return {批.状态, {}, {}, false};
        std::map<稳定编码, const 基础联合节点快照*> 图;
        特征概念定义保护结果 out;
        for (const auto& x : 批.节点组) {
            if (!x.快照) return {基础结构操作状态::内部不一致, {}, {}, false};
            图.emplace(x.查询节点, &*x.快照);
            if (const auto i = 枚举.find(x.查询节点); i != 枚举.end()) {
                const auto& a = i->second; const auto& b = *x.快照;
                if (a.结构.字段组 != b.结构.字段组 || a.结构.节点 != b.结构.节点
                    || a.入向外部关系组 != b.入向外部关系组
                    || a.出向外部关系组 != b.出向外部关系组 || a.入向字段组 != b.入向字段组)
                    return {基础结构操作状态::已完成, {}, {}, true};
            }
            out.预期节点组.push_back(*x.快照);
        }
        const auto 字段组 = [&](稳定编码 n, 稳定编码 key) {
            std::vector<基础字段内容> v;
            for (const auto& f : 图.at(n)->结构.字段组) if (f.字段节点 == key) v.push_back(f.内容);
            return v;
        };
        const auto 单项 = [&](稳定编码 n, 稳定编码 key) {
            const auto v = 字段组(n, key);
            if (v.size() != 1) throw std::logic_error("feature definition cardinality");
            return v.front();
        };
        const auto 数 = [&](稳定编码 n, 稳定编码 key) {
            return std::get<std::int64_t>(std::get<基础值>(单项(n, key)).材料);
        };
        const auto 引用组 = [&](稳定编码 n, 稳定编码 key) {
            std::vector<稳定编码> v;
            for (const auto& x : 字段组(n, key)) v.push_back(std::get<稳定编码>(x));
            std::sort(v.begin(), v.end());
            if (std::adjacent_find(v.begin(), v.end()) != v.end()) throw std::logic_error("duplicate definition reference");
            return v;
        };
        const auto 区间 = [&](稳定编码 n, 稳定编码 key) {
            const auto v = std::get<std::vector<std::int64_t>>(std::get<基础值>(单项(n, key)).材料);
            if (v.empty() || v.size() % 2) throw std::logic_error("invalid interval");
            std::vector<特征概念I64闭区间> r;
            for (std::size_t i = 0; i < v.size(); i += 2) r.push_back({v[i], v[i + 1]});
            return r;
        };
        if (std::get<稳定编码>(单项(节点, 节点类型字段)) != 特征概念节点类型)
            return {基础结构操作状态::内部不一致, {}, {}, false};
        特征概念定义 d;
        d.材料类型 = static_cast<特征概念材料物理类型>(数(节点, 材料类型字段));
        d.比较规则 = static_cast<特征概念比较规则>(数(节点, 比较规则字段));
        d.聚合规则 = static_cast<特征概念聚合规则>(数(节点, 聚合规则字段));
        unsigned 表示 = 0;
        for (const auto key : {I64值域字段, 全材料值域字段, 非I64值域字段,
            结构化分量字段, 存在引用值域字段, 任务集合值域字段, 需求集合值域字段, 存在集合值域字段})
            表示 += !字段组(节点, key).empty();
        if (表示 != 1) throw std::logic_error("multiple domain representations");
        if (!字段组(节点, I64值域字段).empty()) d.值域 = 区间(节点, I64值域字段);
        else if (!字段组(节点, 全材料值域字段).empty()) {
            if (数(节点, 全材料值域字段) != 1) throw std::logic_error("invalid universe");
            d.值域 = 特征概念全材料值域{};
        } else if (!字段组(节点, 非I64值域字段).empty()) d.值域 = 引用组(节点, 非I64值域字段);
        else if (!字段组(节点, 存在引用值域字段).empty())
            d.值域 = 特征概念存在引用值域{std::get<稳定编码>(单项(节点, 存在引用值域字段))};
        else if (!字段组(节点, 存在集合值域字段).empty()) {
            const auto b = 数(节点, 至少一个存在字段);
            if (数(节点, 存在集合值域字段) != 1 || (b != 0 && b != 1)) throw std::logic_error("invalid existence domain");
            d.值域 = 特征概念存在集合值域{引用组(节点, 必含存在字段), b != 0};
        } else if (!字段组(节点, 需求集合值域字段).empty()) {
            const auto b = 数(节点, 至少一个需求字段);
            if (数(节点, 需求集合值域字段) != 1 || (b != 0 && b != 1)) throw std::logic_error("invalid demand domain");
            d.值域 = 特征概念需求集合值域{引用组(节点, 必含需求字段), b != 0};
        } else if (!字段组(节点, 任务集合值域字段).empty()) {
            特征概念任务集合值域 v;
            const auto b = 数(节点, 至少一个任务字段);
            if (数(节点, 任务集合值域字段) != 1 || (b != 0 && b != 1)) throw std::logic_error("invalid task domain");
            v.至少一个任务 = b != 0; v.必含任务节点组 = 引用组(节点, 必含任务字段);
            v.必含需求任务组 = 引用组(节点, 必含需求任务字段);
            v.必含服务要求任务组 = 引用组(节点, 必含服务要求任务字段); d.值域 = std::move(v);
        } else {
            std::map<std::int64_t, 特征概念I64分量定义> 组;
            for (const auto n : 引用组(节点, 结构化分量字段)) {
                if (std::get<稳定编码>(单项(n, 节点类型字段)) != 结构化分量节点类型
                    || 数(n, 分量材料类型字段) != static_cast<std::int64_t>(特征概念材料物理类型::I64标量))
                    throw std::logic_error("invalid component");
                特征概念I64分量定义 v;
                v.角色 = std::get<稳定编码>(单项(n, 分量角色字段)); v.值域 = 区间(n, 分量I64值域字段);
                if (!字段组(n, 分量单位字段).empty()) v.单位 = std::get<稳定编码>(单项(n, 分量单位字段));
                if (!组.emplace(数(n, 分量顺序字段), v).second) throw std::logic_error("duplicate component order");
            }
            特征概念结构化I64值域 v;
            for (const auto& [i, x] : 组) {
                if (i != static_cast<std::int64_t>(v.分量.size())) throw std::logic_error("component order gap");
                v.分量.push_back(x);
            }
            d.值域 = std::move(v);
        }
        if (!字段组(节点, 单位字段).empty()) d.单位 = std::get<稳定编码>(单项(节点, 单位字段));
        d.名称关系 = 引用组(节点, 名称关系字段);
        const auto 命中 = 数(节点, 当前引用实例数字段);
        const auto 规范 = 规范化定义(d);
        if (!规范 || 命中 < 0) throw std::logic_error("invalid feature definition");
        out.信息 = 特征概念定义保护信息{{节点, 规范->材料类型, 规范->值域, 规范->单位,
            规范->比较规则, 规范->聚合规则, 规范->名称关系, 命中}, 根};
        out.状态 = 基础结构操作状态::已完成;
        return out;
    } catch (const std::bad_alloc&) { return {基础结构操作状态::资源失败, {}, {}, false}; }
    catch (...) { return {基础结构操作状态::内部不一致, {}, {}, false}; }
}

} // namespace 海中鱼巣
