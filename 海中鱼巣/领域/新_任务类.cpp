#include "新_任务类.h"
#include "新_方法类.h"
#include "新_场景类.h"
#include "新_存在类.h"
#include "概念_二次特征类.h"
#include "概念_存在类.h"
#include "新_本能函数管理类.h"
#include "新_动态类.h"
#include "../业务/执行场景采集.服务.h"
#include "../核心/容错检查.h"

#include <algorithm>
#include <bit>
#include <chrono>
#include <limits>
#include <map>
#include <mutex>
#include <new>
#include <numeric>
#include <set>
#include <stdexcept>
#include <tuple>
#include <utility>

namespace 海中鱼巣 {

稳定编码 任务集合根节点{};

struct 新_任务类::方法问题暂存 final {
    稳定编码 任务节点, 执行实例节点;
    新执行场景采集请求 采集请求;
    std::shared_ptr<新方法问题反馈材料> 材料;
    新任务问题归档交付结果 回执;
    bool 正在处理 = false;
    bool 结果已冻结 = false;
};

namespace {

std::recursive_mutex 新任务互斥;

// 持有原回调引用，不复制function。必须在owner锁之后构造，先通知后释放锁。
struct 关联检查离域通知 final {
    const std::function<void()>& 通知;
    ~关联检查离域通知() noexcept {
        try { if (通知) 通知(); } catch (...) {}
    }
};

bool 任务线性区间含无穷(const 特征概念I64闭区间& 区间) noexcept {
    constexpr auto 最小 = (std::numeric_limits<std::int64_t>::min)();
    constexpr auto 最大 = (std::numeric_limits<std::int64_t>::max)();
    return 区间.下界 == 最小 || 区间.下界 == 最大
        || 区间.上界 == 最小 || 区间.上界 == 最大;
}

std::uint64_t 任务线性有序坐标(std::int64_t 值) noexcept {
    return std::bit_cast<std::uint64_t>(值) ^ (std::uint64_t{1} << 63);
}

std::int64_t 任务线性坐标值(std::uint64_t 坐标) noexcept {
    return std::bit_cast<std::int64_t>(坐标 ^ (std::uint64_t{1} << 63));
}

// 前置：分子<=分母且分母>0。商不超过乘数，不需要构造溢出的乘积。
// 每步保持 已处理乘数前缀*分子=商*分母+余数，且余数<分母。
std::pair<std::uint64_t, std::uint64_t> 任务线性乘除商余(
    std::uint64_t 分子, std::uint64_t 乘数, std::uint64_t 分母) noexcept {
    std::uint64_t 商 = 0, 余数 = 0;
    for (unsigned 位数 = 64; 位数 != 0;) {
        --位数;
        商 *= 2;
        if (余数 >= 分母 - 余数) {
            余数 -= 分母 - 余数;
            ++商;
        } else {
            余数 += 余数;
        }
        if (((乘数 >> 位数) & 1U) != 0) {
            if (余数 >= 分母 - 分子) {
                余数 -= 分母 - 分子;
                ++商;
            } else {
                余数 += 分子;
            }
        }
    }
    return {商, 余数};
}

bool 含私有数值域(const 新方法条件结果信息& 配对) noexcept {
    for (const auto& 项 : 配对.条件.条件项组) {
        if (项.精确数值域) return true;
    }
    for (const auto& 项 : 配对.结果.结果项组) {
        if (项.精确数值域) return true;
    }
    for (const auto& 项 : 配对.结果.确认要求.禁止结果项组) {
        if (项.精确数值域) return true;
    }
    return false;
}

bool 含私有数值域(const 新方法信息& 方法) noexcept {
    for (const auto& 配对 : 方法.条件结果组) {
        if (含私有数值域(配对)) return true;
    }
    return false;
}

enum class 方法私有域闭包检查状态 : std::uint8_t {
    未含私有域, 含私有域, 方法不可读, 资源失败
};

方法私有域闭包检查状态 检查方法私有域闭包(
    const 新_方法类& 方法服务,
    const 新方法信息& 根方法) noexcept {
    try {
        if (!根方法.节点.值) return 方法私有域闭包检查状态::方法不可读;
        bool 发现私有域 = 含私有数值域(根方法);
        std::set<std::uint64_t> 已访问{根方法.节点.值};
        std::vector<稳定编码> 待查;
        const auto 收集调用 = [&](const 新方法信息& 方法) {
            for (const auto& 步骤 : 方法.组合步骤组) {
                if (!步骤.调用方法节点.值) return false;
                if (已访问.insert(步骤.调用方法节点.值).second)
                    待查.push_back(步骤.调用方法节点);
            }
            return true;
        };
        if (!收集调用(根方法)) return 方法私有域闭包检查状态::方法不可读;
        while (!待查.empty()) {
            const auto 节点 = 待查.back();
            待查.pop_back();
            const auto 方法 = 方法服务.获取方法(节点);
            if (!方法 || 方法->节点 != 节点)
                return 方法私有域闭包检查状态::方法不可读;
            if (含私有数值域(*方法)) 发现私有域 = true;
            if (!收集调用(*方法)) return 方法私有域闭包检查状态::方法不可读;
        }
        return 发现私有域 ? 方法私有域闭包检查状态::含私有域
            : 方法私有域闭包检查状态::未含私有域;
    } catch (const std::bad_alloc&) {
        return 方法私有域闭包检查状态::资源失败;
    } catch (...) {
        return 方法私有域闭包检查状态::方法不可读;
    }
}

enum class 预计变化方向 : std::uint8_t {
    增加 = 1,
    减少,
    不变,
    不明确
};

bool 精确域有效(const 特征概念I64闭区间& 域) noexcept {
    return 域.下界 <= 域.上界
        && 域.下界 != (std::numeric_limits<std::int64_t>::min)()
        && 域.上界 != (std::numeric_limits<std::int64_t>::max)();
}

std::optional<新特征值域比较输入> 取得任务值域输入(
    const 概念_特征类& 概念, 稳定编码 类型,
    std::optional<稳定编码> 概念域,
    const std::optional<特征概念I64闭区间>& 精确域) {
    if (概念域.has_value() == 精确域.has_value()) return std::nullopt;
    const auto 信息 = 概念.获取特征概念(类型);
    if (!信息) return std::nullopt;
    新特征值域比较输入 输入{类型, 信息->单位, {}};
    if (精确域) {
        if (!精确域有效(*精确域)
            || 信息->材料类型 != 特征概念材料物理类型::I64标量) return std::nullopt;
        输入.材料 = 新特征直接I64值域比较材料{*精确域};
    } else 输入.材料 = 新特征概念值域比较材料{*概念域};
    return 输入;
}

std::optional<bool> 准确I64落在任务值域(
    const 新_特征类& 特征, const 概念_特征类& 概念,
    稳定编码 类型, std::optional<稳定编码> 单位, std::int64_t 值,
    std::optional<稳定编码> 概念域,
    const std::optional<特征概念I64闭区间>& 精确域) {
    const auto 域 = 取得任务值域输入(概念, 类型, 概念域, 精确域);
    if (!域 || 域->单位 != 单位) return std::nullopt;
    const auto 比较 = 特征.比较特征值域(
        {类型, 单位, 新特征准确I64比较材料{值}}, *域);
    if (比较.状态 != 新特征比较状态::已完成 || !比较.关系编码) return std::nullopt;
    return *比较.关系编码 == static_cast<std::int64_t>(特征概念值域关系::相等)
        || *比较.关系编码 == static_cast<std::int64_t>(特征概念值域关系::右包含左);
}

预计变化方向 从任务值域推导方向(
    const 新_特征类& 特征, const 概念_特征类& 概念,
    稳定编码 类型, std::optional<稳定编码> 概念域,
    const std::optional<特征概念I64闭区间>& 精确域,
    const 新特征信息& 当前) {
    if (!当前.当前值) return 预计变化方向::不明确;
    const auto* 值 = std::get_if<std::int64_t>(&*当前.当前值);
    const auto 输入 = 取得任务值域输入(概念, 类型, 概念域, 精确域);
    if (!值 || !输入 || 输入->单位 != 当前.实例单位) return 预计变化方向::不明确;
    const auto 比较 = 特征.比较特征值域(*输入,
        {类型, 当前.实例单位, 新特征准确I64比较材料{*值}});
    if (比较.状态 != 新特征比较状态::已完成) return 预计变化方向::不明确;
    std::vector<特征概念I64闭区间> 区间;
    if (精确域) 区间.push_back(*精确域);
    else {
        const auto 信息 = 概念.获取特征概念(*概念域);
        const auto* 组 = 信息 ? std::get_if<std::vector<特征概念I64闭区间>>(&信息->值域) : nullptr;
        if (!组) return 预计变化方向::不明确;
        区间 = *组;
    }
    if (区间.empty()) return 预计变化方向::不明确;
    if (std::all_of(区间.begin(), 区间.end(), [&](const auto& x) { return x.下界 > *值; })) return 预计变化方向::增加;
    if (std::all_of(区间.begin(), 区间.end(), [&](const auto& x) { return x.上界 < *值; })) return 预计变化方向::减少;
    if (区间.size() == 1 && 区间.front().下界 == *值 && 区间.front().上界 == *值) return 预计变化方向::不变;
    return 预计变化方向::不明确;
}

std::optional<稳定编码> 取得特征概念类型根(
    const 概念_特征类& 服务,
    稳定编码 概念节点) noexcept {
    const auto 关系 = 服务.比较单特征概念(概念节点, 概念节点);
    return 关系.状态 == 特征概念规则状态::已完成
            && 关系.关系 == 单特征概念关系::同一
        ? 关系.特征类型根 : std::nullopt;
}

bool 是规范条件I64区间组(
    const std::vector<特征概念I64闭区间>& 区间组) noexcept {
    if (区间组.empty()) return false;
    for (std::size_t 序号 = 0; 序号 < 区间组.size(); ++序号) {
        const auto& 区间 = 区间组[序号];
        if (区间.下界 > 区间.上界) return false;
        if (序号 != 0) {
            const auto 前上界 = 区间组[序号 - 1].上界;
            if (前上界 == (std::numeric_limits<std::int64_t>::max)()
                || 区间.下界 <= 前上界 + 1) return false;
        }
    }
    return true;
}

bool 是条件包含目标(
    const 新需求目标定义& 目标,
    const 概念_特征类& 特征概念服务,
    const 概念_二次特征类& 二次特征概念服务) noexcept {
    if (目标.目标形式 != 新需求目标形式::值 || 目标.目标变化方向
        || !目标.目标值域概念节点 || !目标.目标比较关系概念节点) return false;
    const auto 关系 = 二次特征概念服务.获取二次特征概念(*目标.目标比较关系概念节点);
    const auto 域 = 特征概念服务.获取特征概念(*目标.目标值域概念节点);
    if (!关系 || !域
        || 关系->参照特征概念节点 != 目标.目标值域概念节点
        || 关系->输入定义组.size() != 2 || 关系->结果域种类组.empty()
        || 取得特征概念类型根(特征概念服务, 域->节点)
            != std::optional<稳定编码>{目标.目标特征类型概念节点}) return false;
    const auto* 区间组 = std::get_if<std::vector<特征概念I64闭区间>>(&域->值域);
    const auto* 集合 = std::get_if<特征概念任务集合值域>(&域->值域);
    if (集合) {
        if (域->材料类型 != 特征概念材料物理类型::U64数组 || 域->单位
            || 域->比较规则 != 特征概念比较规则::服务任务集合身份判等
            || 关系->算法 != 二次特征算法::特征材料相似度) return false;
    } else if (!区间组 || !是规范条件I64区间组(*区间组)
        || 域->材料类型 != 特征概念材料物理类型::I64标量
        || 关系->算法 != 二次特征算法::有向特征差值) return false;
    for (std::size_t 序号 = 0; 序号 < 2; ++序号) {
        const auto& 输入 = 关系->输入定义组[序号];
        if (输入.顺序 != static_cast<std::int64_t>(序号)
            || 输入.角色 != (序号 == 0 ? 二次特征输入角色::左输入 : 二次特征输入角色::右输入)
            || 输入.来源种类 != 二次特征输入来源种类::特征准确值
            || !输入.特征类型概念节点
            || 取得特征概念类型根(特征概念服务, *输入.特征类型概念节点)
                != std::optional<稳定编码>{目标.目标特征类型概念节点}) return false;
    }
    return std::all_of(关系->结果域种类组.begin(), 关系->结果域种类组.end(),
        [集合](auto 种类) {
            return 种类 == 二次特征结果域种类::包含于参照值域
                || (!集合 && 种类 == 二次特征结果域种类::等于);
        });
}

bool 条件投影一致(const 新任务条件子需求项& 项, const 特征概念信息& 域) {
    if (域.单位 != 项.单位) return false;
    if (项.集合目标) return 项.目标区间组.empty()
        && 域.值域 == 特征概念值域{*项.集合目标};
    return 是规范条件I64区间组(项.目标区间组)
        && 域.值域 == 特征概念值域{项.目标区间组};
}

预计变化方向 读取预计变化方向(
    const 概念_二次特征类& 服务,
    稳定编码 方向概念节点) noexcept {
    const auto 概念 = 服务.获取二次特征概念(方向概念节点);
    if (!概念 || 概念->算法 != 二次特征算法::有向特征差值
        || 概念->参照特征概念节点) {
        return 预计变化方向::不明确;
    }
    bool 可增加 = false, 可减少 = false, 可不变 = false;
    auto 关系组 = 概念->结果域种类组;
    if (关系组.empty()) 关系组.push_back(二次特征结果域种类::任意);
    for (const auto 关系 : 关系组) {
        switch (关系) {
        case 二次特征结果域种类::大于:
            可增加 = true; break;
        case 二次特征结果域种类::大于等于:
            可增加 = true; 可不变 = true; break;
        case 二次特征结果域种类::等于:
            可不变 = true; break;
        case 二次特征结果域种类::小于等于:
            可减少 = true; 可不变 = true; break;
        case 二次特征结果域种类::小于:
            可减少 = true; break;
        case 二次特征结果域种类::任意:
            可增加 = 可减少 = 可不变 = true; break;
        default:
            return 预计变化方向::不明确;
        }
    }
    if (可增加 && !可减少 && !可不变) return 预计变化方向::增加;
    if (可减少 && !可增加 && !可不变) return 预计变化方向::减少;
    if (可不变 && !可增加 && !可减少) return 预计变化方向::不变;
    return 预计变化方向::不明确;
}


bool 任务值域包含(
    const 新_特征类& 特征, const 概念_特征类& 概念, 稳定编码 类型,
    std::optional<稳定编码> 左概念, const std::optional<特征概念I64闭区间>& 左精确,
    std::optional<稳定编码> 右概念, const std::optional<特征概念I64闭区间>& 右精确,
    bool& 结构有效) {
    结构有效 = false;
    const auto 左 = 取得任务值域输入(概念, 类型, 左概念, 左精确);
    const auto 右 = 取得任务值域输入(概念, 类型, 右概念, 右精确);
    if (!左 || !右) return false;
    const auto 比较 = 特征.比较特征值域(*左, *右);
    if (比较.状态 != 新特征比较状态::已完成 || !比较.关系编码) return false;
    结构有效 = true;
    return *比较.关系编码 == static_cast<std::int64_t>(特征概念值域关系::相等)
        || *比较.关系编码 == static_cast<std::int64_t>(特征概念值域关系::左包含右);
}

std::optional<新状态强时间> 当前实例UTC强时间() noexcept {
    try {
        const auto 计数 = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
        if (计数 < 0
            || static_cast<std::uintmax_t>(计数)
                > static_cast<std::uintmax_t>(
                    (std::numeric_limits<std::int64_t>::max)())) {
            return std::nullopt;
        }
        return 新状态强时间{
            新状态时间语义::实例绝对UTC,
            static_cast<std::int64_t>(计数)};
    } catch (...) {
        return std::nullopt;
    }
}

// 完整解释需求的比较关系；不能把“>、不包含”等目标都当作落域。
// 空返回表示当前材料/关系不能完整比较，false才是已证明不保证满足。
std::optional<bool> 结果域保证满足需求(
    const 概念_特征类& 特征概念,
    const 概念_二次特征类& 二次概念,
    const 新需求目标定义& 目标,
    const 特征概念信息& 结果域) {
    if (目标.目标形式 != 新需求目标形式::值) return false;
    if (!目标.目标值域概念节点 || !目标.目标比较关系概念节点) return std::nullopt;
    const auto 域 = 特征概念.获取特征概念(*目标.目标值域概念节点);
    const auto 关系 = 二次概念.获取二次特征概念(*目标.目标比较关系概念节点);
    if (!域 || !关系 || 关系->参照特征概念节点 != 目标.目标值域概念节点
        || 域->材料类型 != 结果域.材料类型 || 域->单位 != 结果域.单位
        || 取得特征概念类型根(特征概念, 结果域.节点)
            != std::optional<稳定编码>{目标.目标特征类型概念节点}) return std::nullopt;
    bool 有特征输入 = false;
    for (const auto& 输入 : 关系->输入定义组) {
        if (输入.来源种类 != 二次特征输入来源种类::特征准确值) continue;
        if (!输入.特征类型概念节点 || 取得特征概念类型根(特征概念,
            *输入.特征类型概念节点) != std::optional<稳定编码>{目标.目标特征类型概念节点})
            return std::nullopt;
        有特征输入 = true;
    }
    if (!有特征输入) return std::nullopt;
    auto 关系组 = 关系->结果域种类组;
    if (关系组.empty()) 关系组.push_back(二次特征结果域种类::任意);
    bool 接受低于 = false, 接受域内 = false, 接受高于 = false, 接受空隙 = false;
    for (const auto 项 : 关系组) {
        switch (项) {
        case 二次特征结果域种类::任意:
            接受低于 = 接受域内 = 接受高于 = 接受空隙 = true; break;
        case 二次特征结果域种类::小于:
        case 二次特征结果域种类::低于参照值域: 接受低于 = true; break;
        case 二次特征结果域种类::小于等于: 接受低于 = 接受域内 = true; break;
        case 二次特征结果域种类::等于:
        case 二次特征结果域种类::包含于参照值域: 接受域内 = true; break;
        case 二次特征结果域种类::大于等于: 接受高于 = 接受域内 = true; break;
        case 二次特征结果域种类::大于:
        case 二次特征结果域种类::高于参照值域: 接受高于 = true; break;
        case 二次特征结果域种类::不包含于参照值域:
            接受低于 = 接受高于 = 接受空隙 = true; break;
        default: return std::nullopt;
        }
    }
    if (域->材料类型 != 特征概念材料物理类型::I64标量) {
        // 无自然次序的材料只处理域内/域外集合关系。
        if (接受低于 != 接受高于 || 接受低于 != 接受空隙) return std::nullopt;
        if (接受域内 && 接受空隙) return true;
        const auto 比较 = 特征概念.比较值域(域->材料类型, 域->值域, 结果域.值域);
        if (比较.状态 != 特征概念规则状态::已完成 || !比较.关系) return std::nullopt;
        return (接受域内 && (*比较.关系 == 特征概念值域关系::相等
                || *比较.关系 == 特征概念值域关系::左包含右))
            || (接受空隙 && *比较.关系 == 特征概念值域关系::分离);
    }
    using 区间组类型 = std::vector<特征概念I64闭区间>;
    const auto* 目标组 = std::get_if<区间组类型>(&域->值域);
    const auto* 结果组 = std::get_if<区间组类型>(&结果域.值域);
    if (!目标组 || 目标组->empty() || !结果组 || 结果组->empty()) return std::nullopt;
    constexpr auto 最小 = (std::numeric_limits<std::int64_t>::min)();
    constexpr auto 最大 = (std::numeric_limits<std::int64_t>::max)();
    区间组类型 满足组;
    if (接受低于 && 目标组->front().下界 != 最小)
        满足组.push_back({最小, 目标组->front().下界 - 1});
    for (std::size_t i = 0; i < 目标组->size(); ++i) {
        const auto& 段 = (*目标组)[i];
        if (接受域内) 满足组.push_back(段);
        if (接受空隙 && i + 1 < 目标组->size() && 段.上界 != 最大
            && 段.上界 + 1 < (*目标组)[i + 1].下界)
            满足组.push_back({段.上界 + 1, (*目标组)[i + 1].下界 - 1});
    }
    if (接受高于 && 目标组->back().上界 != 最大)
        满足组.push_back({目标组->back().上界 + 1, 最大});
    for (const auto& 段 : *结果组) {
        auto 未覆盖 = 段.下界;
        bool 完整 = false;
        for (const auto& 允许 : 满足组) {
            if (允许.上界 < 未覆盖) continue;
            if (允许.下界 > 未覆盖) break;
            if (允许.上界 >= 段.上界) { 完整 = true; break; }
            未覆盖 = 允许.上界 + 1; // 小于段上界，故不可能溢出。
        }
        if (!完整) return false;
    }
    return true;
}


稳定编码 节点类型字段{};
extern 稳定编码 任务虚拟存在字段;
extern 稳定编码 执行所属任务字段,执行方法字段,任务执行实例节点类型;
// 以下结构只由任务 owner 发布。字段引用保留身份，直接值只保存枚举和数值。
namespace 任务执行组织存储 {
enum class 类型 : std::size_t {
    根, 作用域, 参数, 参数特征, 集合约束, 配对, 条件项, 结果项,
    禁止项, 确认, 线性, 步骤, 分支, 执行采用, 执行条件, 执行绑定, 数量
};
enum class 字段 : std::size_t {
    组织根, 所属任务, 治理宿主, 根作用域, 作用域成员,
    来源方法, 来源虚拟存在, 作用域值, 实现方式, 参数成员, 配对成员,
    步骤成员, 入口步骤, 分支成员, 所属作用域, 顺序, 角色,
    存在概念, 要求场景, 对象种类, 特征规格, 集合约束,
    所属参数, 特征类型, 值域概念, 二次特征, 集合角色, 集合特征,
    来源配对, 来源条件, 来源结果, 条件关系, 条件成员, 结果关系,
    结果成员, 确认要求, 线性成员, 所属配对, 状态动态概念, 下界, 上界,
    方向概念, 主要结果, 所属确认, 证据质量, 等待纳秒, 等待条件,
    多证据关系, 禁止关系, 禁止成员, 条件序号, 结果序号, 线性方向,
    原条件下界, 原条件上界, 原结果下界, 原结果上界,
    来源步骤, 调用作用域, 来源转移, 当前步骤, 当前结果要求,
    匹配概念, 下一步骤, 执行采用引用, 采用所属执行, 采用根, 采用组织,
    采用作用域, 采用配对, 绑定成员, 绑定所属采用, 世界场景, 实际存在,
    实际特征, 引用任务, 引用虚拟存在, 引用状态特征, 服务理解, 服务分项,
    服务确认, 数量
};
稳定编码 类型节点[static_cast<std::size_t>(类型::数量)]{};
稳定编码 字段节点[static_cast<std::size_t>(字段::数量)]{};
稳定编码 取(类型 值) { return 类型节点[static_cast<std::size_t>(值)]; }
稳定编码 取(字段 值) { return 字段节点[static_cast<std::size_t>(值)]; }
void 要求(bool 成立, const char* 原因) {
    if (!成立) throw std::logic_error(原因);
}
bool 已分配() {
    return std::all_of(std::begin(类型节点), std::end(类型节点), 有效)
        && std::all_of(std::begin(字段节点), std::end(字段节点), 有效);
}
bool 仍存在() noexcept {
    if (!已分配()) return false;
    // 此入口也被旧 optional getters 使用，不能把已分配架构损坏返回为空任务。
    try {
        const auto 存在 = [](稳定编码 节点) {
            const auto 回执 = 全局基础数据集.查询完整节点结构(节点);
            要求(回执.状态 == 基础结构操作状态::已完成 && 回执.结构,
                "allocated task organization schema missing or unreadable");
        };
        for (const auto 节点 : 类型节点) 存在(节点);
        for (const auto 节点 : 字段节点) 存在(节点);
        return true;
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"任务执行组织架构核验", L"正式元数据损坏", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"任务执行组织架构核验", L"未知元数据故障");
    }
}
template<class 建立器> bool 初始化(建立器&& 建立) {
    for (auto& 节点 : 类型节点) if (!建立(节点)) return false;
    for (auto& 节点 : 字段节点) if (!建立(节点)) return false;
    return true;
}
std::int64_t 序号(std::size_t 值) {
    要求(值 <= static_cast<std::size_t>((std::numeric_limits<std::int64_t>::max)()),
        "task organization index overflow");
    return static_cast<std::int64_t>(值);
}
void 规范集合(std::vector<稳定编码>& 组) {
    std::sort(组.begin(), 组.end());
    组.erase(std::unique(组.begin(), 组.end()), 组.end());
}

// 将任务内部身份投影回各自来源身份，仅用于同义比较；从不写入基座。
新任务方法执行组织 同义定义(新任务方法执行组织 组织) {
    std::map<稳定编码, 稳定编码> 映射;
    for (const auto& 域 : 组织.方法作用域组) {
        要求(映射.emplace(域.节点, 域.来源方法节点).second, "duplicate scope identity");
        for (const auto& 对 : 域.配对要求组)
            要求(映射.emplace(对.节点, 对.来源配对节点).second, "duplicate pair identity");
        for (const auto& 步 : 域.步骤组)
            要求(映射.emplace(步.节点, 步.来源步骤节点).second, "duplicate step identity");
    }
    组织.节点 = {};
    for (auto& 根 : 组织.根方法作用域节点组) 根 = 映射.at(根);
    规范集合(组织.根方法作用域节点组);
    for (auto& 域 : 组织.方法作用域组) {
        域.节点 = 域.来源方法节点;
        for (auto& 参数 : 域.参数要求组) {
            参数.节点 = {};
            for (auto& 规格 : 参数.规格.特征规格组) 规范集合(规格.二次特征概念组);
        }
        for (auto& 对 : 域.配对要求组) 对.节点 = 对.来源配对节点;
        std::sort(域.配对要求组.begin(), 域.配对要求组.end(),
            [](const auto& 左, const auto& 右) { return 左.来源配对节点 < 右.来源配对节点; });
        for (auto& 步 : 域.步骤组) {
            步.节点 = 步.来源步骤节点;
            步.调用作用域节点 = 映射.at(步.调用作用域节点);
        }
        std::sort(域.步骤组.begin(), 域.步骤组.end(),
            [](const auto& 左, const auto& 右) { return 左.顺序 < 右.顺序; });
        if (域.入口步骤节点) 域.入口步骤节点 = 映射.at(*域.入口步骤节点);
        for (auto& 分支 : 域.分支组) {
            分支.节点 = {};
            分支.当前步骤节点 = 映射.at(分支.当前步骤节点);
            分支.下一步骤节点 = 映射.at(分支.下一步骤节点);
            分支.当前结果要求节点 = 映射.at(分支.当前结果要求节点);
        }
        std::sort(域.分支组.begin(), 域.分支组.end(),
            [](const auto& 左, const auto& 右) { return 左.来源转移节点 < 右.来源转移节点; });
    }
    std::sort(组织.方法作用域组.begin(), 组织.方法作用域组.end(),
        [](const auto& 左, const auto& 右) { return 左.来源方法节点 < 右.来源方法节点; });
    return 组织;
}

新任务方法执行组织 转换定义(稳定编码 任务, 稳定编码 虚拟存在,
    const 新_方法类::执行结构读取结果& 闭包) {
    新任务方法执行组织 组织{{}, 任务, 虚拟存在, 闭包.根方法节点组, {}};
    // 临时值式图使用已存在的来源身份作为映射键；发布器统一转为组内引用。
    std::map<稳定编码, 稳定编码> 结果到配对;
    for (const auto& 方法 : 闭包.方法组)
        for (const auto& 对 : 方法.条件结果组)
            要求(结果到配对.emplace(对.结果节点, 对.配对节点).second,
                "method result has multiple pair owners");
    for (const auto& 方法 : 闭包.方法组) {
        新任务执行方法作用域 域;
        域.节点 = 域.来源方法节点 = 方法.节点;
        域.来源方法虚拟存在节点 = 方法.方法虚拟存在节点;
        域.作用域 = 方法.作用域; 域.实现方式 = 方法.实现方式;
        for (const auto& 参数 : 方法.参数规格组) 域.参数要求组.push_back({{}, 参数});
        for (const auto& 对 : 方法.条件结果组)
            域.配对要求组.push_back({对.配对节点, 对.配对节点, 对.条件节点,
                对.结果节点, 对.条件, 对.结果, 对.线性关系组});
        for (const auto& 步 : 方法.组合步骤组)
            域.步骤组.push_back({步.步骤节点, 步.步骤节点, 步.调用方法节点, 步.顺序});
        域.入口步骤节点 = 方法.组合入口步骤节点;
        for (const auto& 转移 : 方法.结果转移组)
            域.分支组.push_back({{}, 转移.转移节点, 转移.当前步骤节点,
                结果到配对.at(转移.当前结果节点), 转移.匹配结果概念节点, 转移.下一步骤节点});
        组织.方法作用域组.push_back(std::move(域));
    }
    return 同义定义(std::move(组织));
}

// 值式合并不发布。旧定义必须原样保留，组合控制的纯新增单独报告能力缺口。
std::optional<新任务方法执行组织> 合并定义(
    const 新任务方法执行组织& 已有, const 新任务方法执行组织& 输入) {
    auto 合并 = 同义定义(已有);
    bool 控制新增 = false;
    for (const auto& 新域 : 输入.方法作用域组) {
        const auto 位置 = std::find_if(合并.方法作用域组.begin(), 合并.方法作用域组.end(),
            [&](const auto& 域) { return 域.来源方法节点 == 新域.来源方法节点; });
        if (位置 == 合并.方法作用域组.end()) {
            合并.方法作用域组.push_back(新域); continue;
        }
        auto& 旧域 = *位置;
        要求(旧域.来源方法虚拟存在节点 == 新域.来源方法虚拟存在节点
            && 旧域.作用域 == 新域.作用域 && 旧域.实现方式 == 新域.实现方式
            && 旧域.参数要求组 == 新域.参数要求组, "adopted method parameter definition changed");
        for (const auto& 对 : 旧域.配对要求组) {
            const auto 新对 = std::find_if(新域.配对要求组.begin(), 新域.配对要求组.end(),
                [&](const auto& 项) { return 项.来源配对节点 == 对.来源配对节点; });
            要求(新对 != 新域.配对要求组.end() && *新对 == 对, "adopted pair changed or disappeared");
        }
        for (const auto& 对 : 新域.配对要求组)
            if (std::none_of(旧域.配对要求组.begin(), 旧域.配对要求组.end(),
                [&](const auto& 项) { return 项.来源配对节点 == 对.来源配对节点; })) 旧域.配对要求组.push_back(对);
        for (const auto& 步 : 旧域.步骤组) {
            const auto 新步 = std::find_if(新域.步骤组.begin(), 新域.步骤组.end(),
                [&](const auto& 项) { return 项.来源步骤节点 == 步.来源步骤节点; });
            要求(新步 != 新域.步骤组.end() && *新步 == 步, "adopted call step changed or disappeared");
        }
        for (const auto& 分支 : 旧域.分支组) {
            const auto 新支 = std::find_if(新域.分支组.begin(), 新域.分支组.end(),
                [&](const auto& 项) { return 项.来源转移节点 == 分支.来源转移节点; });
            要求(新支 != 新域.分支组.end() && *新支 == 分支, "adopted branch changed or disappeared");
        }
        if (旧域.入口步骤节点)
            要求(旧域.入口步骤节点 == 新域.入口步骤节点, "adopted entry step changed or disappeared");
        控制新增 = 控制新增 || 旧域.步骤组.size() != 新域.步骤组.size()
            || 旧域.分支组.size() != 新域.分支组.size()
            || 旧域.入口步骤节点 != 新域.入口步骤节点;
    }
    if (控制新增) return {};
    合并.根方法作用域节点组.insert(合并.根方法作用域节点组.end(),
        输入.根方法作用域节点组.begin(), 输入.根方法作用域节点组.end());
    规范集合(合并.根方法作用域节点组);
    return 同义定义(std::move(合并));
}

struct 写器 final {
    bool 执行采用模式=false;
    std::vector<基础组内新节点> 节点组;
    std::map<稳定编码, std::size_t> 身份映射;
    std::map<std::size_t, 稳定编码> 既有节点;
    std::map<稳定编码, 稳定编码> 既有来源;
    std::size_t 预留(类型 种类, 稳定编码 来源) {
        const auto 节点 = 新节点(种类);
        if (const auto 旧 = 既有来源.find(来源); 旧 != 既有来源.end()) {
            既有节点.emplace(节点, 旧->second);
            节点组.at(节点).字段组.clear();
        }
        要求(身份映射.emplace(来源, 节点).second, "duplicate reserved source");
        return 节点;
    }
    void 引用(std::size_t 节点, 字段 项, 稳定编码 值) {
        要求(有效(值), "empty external task definition reference");
        节点组.at(节点).字段组.push_back({取(项), 值});
    }
    void 引用(std::size_t 节点, 字段 项, const std::optional<稳定编码>& 值) {
        if (值) 引用(节点, 项, *值);
    }
    void 内链(std::size_t 节点, 字段 项, std::size_t 目标) {
        节点组.at(节点).字段组.push_back({取(项), 基础组内节点引用{目标}});
    }
    void 数(std::size_t 节点, 字段 项, std::int64_t 值) {
        节点组.at(节点).字段组.push_back({取(项), 基础值{值}});
    }
    std::size_t 新节点(类型 种类) {
        const auto 位置 = 节点组.size();
        节点组.push_back({{{节点类型字段, 取(种类)}}});
        if (执行采用模式) {if(位置!=0)内链(位置,字段::采用根,0);}
        else if (种类 != 类型::根) 内链(位置, 字段::组织根, 0);
        return 位置;
    }
    std::size_t 子节点(类型 种类, std::size_t 父, 字段 成员, 字段 所属,
        std::optional<std::int64_t> 顺序 = {}) {
        const auto 节点 = 新节点(种类);
        内链(父, 成员, 节点); 内链(节点, 所属, 父);
        if (顺序) 数(节点, 字段::顺序, *顺序);
        return 节点;
    }
    void 值域(std::size_t 节点, const std::optional<特征概念I64闭区间>& 域) {
        if (域) { 数(节点, 字段::下界, 域->下界); 数(节点, 字段::上界, 域->上界); }
    }
    void 写结果(std::size_t 父, const std::vector<新方法结果项定义>& 组, bool 禁止) {
        for (std::size_t i = 0; i < 组.size(); ++i) {
            const auto& 项 = 组[i];
            const auto 节点 = 子节点(禁止 ? 类型::禁止项 : 类型::结果项, 父,
                禁止 ? 字段::禁止成员 : 字段::结果成员,
                禁止 ? 字段::所属确认 : 字段::所属配对, 序号(i));
            引用(节点, 字段::角色, 项.参数角色节点);
            引用(节点, 字段::特征类型, 项.特征类型概念节点);
            引用(节点, 字段::值域概念, 项.特征值域概念节点);
            引用(节点, 字段::方向概念, 项.变化方向概念节点);
            引用(节点, 字段::状态动态概念, 项.状态或动态概念节点);
            数(节点, 字段::主要结果, 项.主要结果 ? 1 : 0); 值域(节点, 项.精确数值域);
        }
    }
    void 写配对(std::size_t 节点, const 新任务执行配对要求& 对) {
        引用(节点, 字段::来源配对, 对.来源配对节点);
        引用(节点, 字段::来源条件, 对.来源条件节点);
        引用(节点, 字段::来源结果, 对.来源结果节点);
        数(节点, 字段::条件关系, static_cast<std::int64_t>(对.条件.关系规则));
        数(节点, 字段::结果关系, static_cast<std::int64_t>(对.结果.关系规则));
        for (std::size_t i = 0; i < 对.条件.条件项组.size(); ++i) {
            const auto& 项 = 对.条件.条件项组[i];
            const auto 子 = 子节点(类型::条件项, 节点, 字段::条件成员, 字段::所属配对, 序号(i));
            引用(子, 字段::角色, 项.参数角色节点); 引用(子, 字段::存在概念, 项.存在概念节点);
            引用(子, 字段::特征类型, 项.特征类型概念节点);
            引用(子, 字段::值域概念, 项.特征值域概念节点);
            引用(子, 字段::二次特征, 项.二次特征概念节点);
            引用(子, 字段::状态动态概念, 项.状态或动态概念节点); 值域(子, 项.精确数值域);
        }
        写结果(节点, 对.结果.结果项组, false);
        const auto 确认 = 子节点(类型::确认, 节点, 字段::确认要求, 字段::所属配对);
        const auto& 要求值 = 对.结果.确认要求;
        数(确认, 字段::证据质量, static_cast<std::int64_t>(要求值.必要证据质量));
        if (要求值.最大等待纳秒) 数(确认, 字段::等待纳秒, *要求值.最大等待纳秒);
        引用(确认, 字段::等待条件, 要求值.等待条件概念节点);
        数(确认, 字段::多证据关系, static_cast<std::int64_t>(要求值.多证据关系));
        数(确认, 字段::禁止关系, static_cast<std::int64_t>(要求值.禁止结果关系规则));
        写结果(确认, 要求值.禁止结果项组, true);
        for (std::size_t i = 0; i < 对.线性关系组.size(); ++i) {
            const auto& 项 = 对.线性关系组[i];
            const auto 子 = 子节点(类型::线性, 节点, 字段::线性成员, 字段::所属配对, 序号(i));
            数(子, 字段::条件序号, 序号(项.条件序号)); 数(子, 字段::结果序号, 序号(项.结果序号));
            数(子, 字段::线性方向, static_cast<std::int64_t>(项.方向));
            数(子, 字段::原条件下界, 项.原条件域.下界); 数(子, 字段::原条件上界, 项.原条件域.上界);
            数(子, 字段::原结果下界, 项.原结果域.下界); 数(子, 字段::原结果上界, 项.原结果域.上界);
        }
    }
    写器() = default;
    explicit 写器(const 新任务方法执行组织& 组织, const 新任务方法执行组织* 旧组织 = nullptr) {
        const auto 根 = 新节点(类型::根);
        std::set<稳定编码> 旧根方法;
        if (旧组织) {
            既有节点.emplace(根, 旧组织->节点); 节点组.at(根).字段组.clear();
            for (const auto& 域 : 旧组织->方法作用域组) {
                既有来源.emplace(域.来源方法节点, 域.节点);
                if (std::find(旧组织->根方法作用域节点组.begin(), 旧组织->根方法作用域节点组.end(),
                    域.节点) != 旧组织->根方法作用域节点组.end()) 旧根方法.insert(域.来源方法节点);
                for (const auto& 对 : 域.配对要求组) 既有来源.emplace(对.来源配对节点, 对.节点);
                for (const auto& 步 : 域.步骤组) 既有来源.emplace(步.来源步骤节点, 步.节点);
            }
        } else {
            引用(根, 字段::所属任务, 组织.任务节点); 引用(根, 字段::治理宿主, 组织.任务虚拟存在节点);
        }
        // 先为所有调用目标和结果要求预分配组内索引，支持向前引用与递归图。
        for (const auto& 域 : 组织.方法作用域组) {
            const auto 节点 = 预留(类型::作用域, 域.节点);
            if (!既有节点.contains(节点)) 内链(根, 字段::作用域成员, 节点);
            for (const auto& 对 : 域.配对要求组) {
                const auto 子 = 预留(类型::配对, 对.节点);
                if (!既有节点.contains(子)) {
                    内链(节点, 字段::配对成员, 子); 内链(子, 字段::所属作用域, 节点);
                }
            }
            for (const auto& 步 : 域.步骤组) {
                const auto 子 = 预留(类型::步骤, 步.节点);
                if (!既有节点.contains(子)) {
                    内链(节点, 字段::步骤成员, 子); 内链(子, 字段::所属作用域, 节点);
                    数(子, 字段::顺序, 步.顺序);
                }
            }
        }
        for (const auto 根域 : 组织.根方法作用域节点组)
            if (!旧根方法.contains(根域)) 内链(根, 字段::根作用域, 身份映射.at(根域));
        for (const auto& 域 : 组织.方法作用域组) {
            const auto 节点 = 身份映射.at(域.节点);
            if (既有节点.contains(节点)) {
                for (const auto& 对 : 域.配对要求组)
                    if (!既有节点.contains(身份映射.at(对.节点))) 写配对(身份映射.at(对.节点), 对);
                continue;
            }
            引用(节点, 字段::来源方法, 域.来源方法节点);
            引用(节点, 字段::来源虚拟存在, 域.来源方法虚拟存在节点);
            数(节点, 字段::作用域值, static_cast<std::int64_t>(域.作用域));
            数(节点, 字段::实现方式, static_cast<std::int64_t>(域.实现方式));
            for (std::size_t i = 0; i < 域.参数要求组.size(); ++i) {
                const auto& 规格 = 域.参数要求组[i].规格;
                const auto 子 = 子节点(类型::参数, 节点, 字段::参数成员, 字段::所属作用域, 序号(i));
                引用(子, 字段::角色, 规格.参数角色节点); 引用(子, 字段::存在概念, 规格.存在概念节点);
                数(子, 字段::要求场景, 规格.要求场景 ? 1 : 0);
                数(子, 字段::对象种类, static_cast<std::int64_t>(规格.对象种类));
                for (std::size_t j = 0; j < 规格.特征规格组.size(); ++j) {
                    const auto& 特征 = 规格.特征规格组[j];
                    const auto 特 = 子节点(类型::参数特征, 子, 字段::特征规格, 字段::所属参数, 序号(j));
                    引用(特, 字段::特征类型, 特征.特征类型概念节点); 引用(特, 字段::值域概念, 特征.值域概念节点);
                    for (const auto 二次 : 特征.二次特征概念组) 引用(特, 字段::二次特征, 二次);
                }
                if (规格.集合成员约束) {
                    const auto 约束 = 子节点(类型::集合约束, 子, 字段::集合约束, 字段::所属参数);
                    引用(约束, 字段::集合角色, 规格.集合成员约束->集合参数角色节点);
                    引用(约束, 字段::集合特征, 规格.集合成员约束->集合特征类型概念节点);
                }
            }
            for (const auto& 对 : 域.配对要求组) 写配对(身份映射.at(对.节点), 对);
            for (const auto& 步 : 域.步骤组) {
                const auto 子 = 身份映射.at(步.节点);
                引用(子, 字段::来源步骤, 步.来源步骤节点);
                内链(子, 字段::调用作用域, 身份映射.at(步.调用作用域节点));
            }
            if (域.入口步骤节点) 内链(节点, 字段::入口步骤, 身份映射.at(*域.入口步骤节点));
            for (const auto& 分支 : 域.分支组) {
                const auto 子 = 子节点(类型::分支, 节点, 字段::分支成员, 字段::所属作用域);
                引用(子, 字段::来源转移, 分支.来源转移节点);
                内链(子, 字段::当前步骤, 身份映射.at(分支.当前步骤节点));
                内链(子, 字段::当前结果要求, 身份映射.at(分支.当前结果要求节点));
                引用(子, 字段::匹配概念, 分支.匹配结果概念节点);
                内链(子, 字段::下一步骤, 身份映射.at(分支.下一步骤节点));
            }
        }
    }
    // 诊断责任：向上送出。占位索引只活在调用内，发布前统一压缩。
    基础节点组追加请求 追加请求(const std::map<稳定编码, 基础节点结构>& 原始结构) const {
        基础节点组追加请求 请求;
        std::map<std::size_t, std::size_t> 新索引;
        for (std::size_t i = 0; i < 节点组.size(); ++i)
            if (!既有节点.contains(i)) {
                新索引.emplace(i, 请求.新节点组.size()); 请求.新节点组.emplace_back();
            }
        const auto 转换 = [&](const 基础组内字段内容& 内容) -> 基础组内字段内容 {
            const auto* 内部 = std::get_if<基础组内节点引用>(&内容);
            if (!内部) return 内容;
            if (const auto 旧 = 既有节点.find(内部->索引); 旧 != 既有节点.end()) return 旧->second;
            return 基础组内节点引用{新索引.at(内部->索引)};
        };
        std::set<稳定编码> 守卫{既有节点.at(0)};
        for (std::size_t i = 0; i < 节点组.size(); ++i) {
            for (const auto& 项 : 节点组[i].字段组) {
                if (const auto 旧 = 既有节点.find(i); 旧 != 既有节点.end()) {
                    要求((i == 0 && (项.字段节点 == 取(字段::作用域成员) || 项.字段节点 == 取(字段::根作用域)))
                        || (i != 0 && 项.字段节点 == 取(字段::配对成员)), "append field outside whitelist");
                    守卫.insert(旧->second);
                    请求.追加字段组.push_back({旧->second, 项.字段节点, 转换(项.内容)});
                } else 请求.新节点组.at(新索引.at(i)).字段组.push_back({项.字段节点, 转换(项.内容)});
            }
        }
        for (const auto 节点 : 守卫) 请求.预期既有节点组.push_back(原始结构.at(节点));
        return 请求;
    }
};
struct 读器 final {
    稳定编码 根;
    std::map<稳定编码, 基础节点结构> 节点组;
    std::set<稳定编码> 已访问, 已用字段, 引用目标;
    bool 执行采用模式=false;
    explicit 读器(稳定编码 根节点,bool 采用模式=false) : 根(根节点),执行采用模式(采用模式) {
        // 反向枚举全部组织成员，不能只相信根列出的成员而漏掉孤儿节点。
        const auto 根字段 = 执行采用模式 ? 字段::采用根 : 字段::组织根;
        const auto 反查 = 全局基础数据集.查询字段引用节点结构(根, 取(根字段));
        要求(反查.状态 == 基础结构操作状态::已完成, "organization members reverse read failed");
        std::set<稳定编码> 编码组{根};
        for (const auto& 引用 : 反查.引用字段组) {
            要求(引用.字段节点 == 取(根字段)
                && std::get_if<稳定编码>(&引用.内容) && std::get<稳定编码>(引用.内容) == 根
                && 编码组.insert(引用.所属节点).second, "duplicate organization ownership");
        }
        const auto 批读 = 全局基础数据集.查询节点组完整结构({编码组.begin(), 编码组.end()});
        要求(批读.状态 == 基础结构操作状态::已完成 && 批读.节点组.size() == 编码组.size(),
            "organization batch read failed");
        for (const auto& 项 : 批读.节点组) {
            要求(项.快照 && 项.快照->结构.节点.编码 == 项.查询节点
                && 项.快照->入向外部关系组.empty() && 项.快照->出向外部关系组.empty(),
                "organization node missing or has external containment");
            要求(节点组.emplace(项.查询节点, 项.快照->结构).second, "duplicate batch node");
        }
    }
    std::vector<const 基础字段关系*> 取字段(稳定编码 节点, 稳定编码 字段码) {
        std::vector<const 基础字段关系*> 结果;
        for (const auto& 项 : 节点组.at(节点).字段组) if (项.字段节点 == 字段码) {
            要求(项.所属节点 == 节点 && 已用字段.insert(项.编码).second,
                "organization field consumed twice");
            结果.push_back(&项);
        }
        return 结果;
    }
    std::vector<稳定编码> 引用组(稳定编码 节点, 字段 项) {
        std::vector<稳定编码> 结果;
        std::set<稳定编码> 唯一;
        for (const auto* 字 : 取字段(节点, 取(项))) {
            const auto* 值 = std::get_if<稳定编码>(&字->内容);
            要求(值 && 有效(*值) && 唯一.insert(*值).second, "invalid/duplicate node reference");
            结果.push_back(*值); 引用目标.insert(*值);
        }
        return 结果;
    }
    std::optional<稳定编码> 可选引用(稳定编码 节点, 字段 项) {
        const auto 组 = 引用组(节点, 项);
        要求(组.size() <= 1, "optional reference cardinality");
        return 组.empty() ? std::nullopt : std::optional<稳定编码>{组.front()};
    }
    稳定编码 引用(稳定编码 节点, 字段 项) {
        const auto 值 = 可选引用(节点, 项);
        要求(值.has_value(), "required reference missing"); return *值;
    }
    std::optional<std::int64_t> 可选数(稳定编码 节点, 字段 项) {
        const auto 组 = 取字段(节点, 取(项));
        要求(组.size() <= 1, "optional integer cardinality");
        if (组.empty()) return {};
        const auto* 值 = std::get_if<基础值>(&组.front()->内容);
        const auto* 数值 = 值 ? std::get_if<std::int64_t>(&值->材料) : nullptr;
        要求(数值 != nullptr, "integer field type mismatch"); return *数值;
    }
    std::int64_t 数(稳定编码 节点, 字段 项) {
        const auto 值 = 可选数(节点, 项);
        要求(值.has_value(), "required integer missing"); return *值;
    }
    template<class 枚举> 枚举 枚举值(稳定编码 节点, 字段 项, std::int64_t 小, std::int64_t 大) {
        const auto 值 = 数(节点, 项); 要求(值 >= 小 && 值 <= 大, "invalid enum value");
        return static_cast<枚举>(值);
    }
    bool 布尔(稳定编码 节点, 字段 项) {
        const auto 值 = 数(节点, 项); 要求(值 == 0 || 值 == 1, "invalid boolean"); return 值 == 1;
    }
    void 开节点(稳定编码 节点, 类型 种类,
        std::optional<std::pair<字段, 稳定编码>> 父 = {}) {
        要求(已访问.insert(节点).second, "duplicate containment or cycle");
        const auto 组 = 取字段(节点, 节点类型字段);
        要求(组.size() == 1 && std::get_if<稳定编码>(&组.front()->内容)
            && std::get<稳定编码>(组.front()->内容) == 取(种类), "wrong organization node type");
        if(执行采用模式){if(节点!=根)要求(引用(节点,字段::采用根)==根,"cross-execution-adoption node");}
        else if (种类 != 类型::根) 要求(引用(节点, 字段::组织根) == 根, "cross-root node");
        if (父) 要求(引用(节点, 父->first) == 父->second, "containment reverse mismatch");
    }
    std::vector<稳定编码> 子组(稳定编码 父, 字段 成员, 类型 种类, 字段 所属, bool 有序) {
        auto 组 = 引用组(父, 成员);
        std::map<std::int64_t, 稳定编码> 顺序组;
        for (const auto 子 : 组) {
            开节点(子, 种类, std::pair{所属, 父});
            if (有序) 要求(顺序组.emplace(数(子, 字段::顺序), 子).second, "duplicate child sequence");
        }
        if (有序) {
            组.clear();
            for (const auto& [序, 子] : 顺序组) {
                要求(序 == 序号(组.size()), "non-contiguous child sequence"); 组.push_back(子);
            }
        }
        return 组;
    }
    std::optional<特征概念I64闭区间> 值域(稳定编码 节点) {
        const auto 小 = 可选数(节点, 字段::下界), 大 = 可选数(节点, 字段::上界);
        要求(小.has_value() == 大.has_value(), "half interval");
        if (!小) return {};
        要求(*小 <= *大, "reversed interval"); return 特征概念I64闭区间{*小, *大};
    }
    std::vector<新方法结果项定义> 读结果(稳定编码 父, bool 禁止) {
        std::vector<新方法结果项定义> 结果;
        for (const auto 节点 : 子组(父, 禁止 ? 字段::禁止成员 : 字段::结果成员,
                禁止 ? 类型::禁止项 : 类型::结果项,
                禁止 ? 字段::所属确认 : 字段::所属配对, true)) {
            新方法结果项定义 项;
            项.参数角色节点 = 引用(节点, 字段::角色);
            项.特征类型概念节点 = 可选引用(节点, 字段::特征类型);
            项.特征值域概念节点 = 可选引用(节点, 字段::值域概念);
            项.变化方向概念节点 = 可选引用(节点, 字段::方向概念);
            项.状态或动态概念节点 = 可选引用(节点, 字段::状态动态概念);
            项.主要结果 = 布尔(节点, 字段::主要结果); 项.精确数值域 = 值域(节点);
            要求(!(项.精确数值域 && 项.特征值域概念节点), "conflicting result domains");
            结果.push_back(std::move(项));
        }
        return 结果;
    }
    新任务执行配对要求 读配对(稳定编码 节点) {
        新任务执行配对要求 对;
        对.节点 = 节点; 对.来源配对节点 = 引用(节点, 字段::来源配对);
        对.来源条件节点 = 引用(节点, 字段::来源条件); 对.来源结果节点 = 引用(节点, 字段::来源结果);
        对.条件.关系规则 = 枚举值<新方法项关系规则>(节点, 字段::条件关系, 1, 6);
        对.结果.关系规则 = 枚举值<新方法项关系规则>(节点, 字段::结果关系, 1, 6);
        for (const auto 子 : 子组(节点, 字段::条件成员, 类型::条件项, 字段::所属配对, true)) {
            新方法条件项定义 项;
            项.参数角色节点 = 引用(子, 字段::角色); 项.存在概念节点 = 可选引用(子, 字段::存在概念);
            项.特征类型概念节点 = 可选引用(子, 字段::特征类型);
            项.特征值域概念节点 = 可选引用(子, 字段::值域概念);
            项.二次特征概念节点 = 可选引用(子, 字段::二次特征);
            项.状态或动态概念节点 = 可选引用(子, 字段::状态动态概念); 项.精确数值域 = 值域(子);
            要求(!(项.精确数值域 && 项.特征值域概念节点), "conflicting condition domains");
            对.条件.条件项组.push_back(std::move(项));
        }
        对.结果.结果项组 = 读结果(节点, false);
        const auto 确认组 = 子组(节点, 字段::确认要求, 类型::确认, 字段::所属配对, false);
        要求(确认组.size() == 1, "confirmation requirement cardinality");
        const auto 确认 = 确认组.front(); auto& 确 = 对.结果.确认要求;
        确.必要证据质量 = 枚举值<新方法必要证据质量>(确认, 字段::证据质量, 1, 2);
        确.最大等待纳秒 = 可选数(确认, 字段::等待纳秒);
        要求(!确.最大等待纳秒 || *确.最大等待纳秒 >= 0, "negative wait interval");
        确.等待条件概念节点 = 可选引用(确认, 字段::等待条件);
        确.多证据关系 = 枚举值<新方法多证据关系>(确认, 字段::多证据关系, 1, 3);
        确.禁止结果关系规则 = 枚举值<新方法项关系规则>(确认, 字段::禁止关系, 1, 6);
        确.禁止结果项组 = 读结果(确认, true);
        for (const auto 子 : 子组(节点, 字段::线性成员, 类型::线性, 字段::所属配对, true)) {
            新方法条件结果线性关系 项;
            const auto 条 = 数(子, 字段::条件序号), 果 = 数(子, 字段::结果序号);
            要求(条 >= 0 && 果 >= 0 && static_cast<std::uint64_t>(条) < 对.条件.条件项组.size()
                && static_cast<std::uint64_t>(果) < 对.结果.结果项组.size(), "linear index out of range");
            项.条件序号 = static_cast<std::size_t>(条); 项.结果序号 = static_cast<std::size_t>(果);
            项.方向 = 枚举值<新方法条件结果线性方向>(子, 字段::线性方向, -1, 1);
            项.原条件域 = {数(子, 字段::原条件下界), 数(子, 字段::原条件上界)};
            项.原结果域 = {数(子, 字段::原结果下界), 数(子, 字段::原结果上界)};
            要求(项.原条件域.下界 <= 项.原条件域.上界 && 项.原结果域.下界 <= 项.原结果域.上界,
                "invalid original linear domains");
            对.线性关系组.push_back(项);
        }
        return 对;
    }
    新任务方法执行组织 读取() {
        开节点(根, 类型::根);
        新任务方法执行组织 组织{根, 引用(根, 字段::所属任务), 引用(根, 字段::治理宿主),
            引用组(根, 字段::根作用域), {}};
        for (const auto 节点 : 引用组(根, 字段::作用域成员)) {
            开节点(节点, 类型::作用域);
            新任务执行方法作用域 域;
            域.节点 = 节点; 域.来源方法节点 = 引用(节点, 字段::来源方法);
            域.来源方法虚拟存在节点 = 引用(节点, 字段::来源虚拟存在);
            域.作用域 = 枚举值<新方法作用域>(节点, 字段::作用域值, 1, 2);
            域.实现方式 = 枚举值<新方法实现方式>(节点, 字段::实现方式, 1, 2);
            for (const auto 子 : 子组(节点, 字段::参数成员, 类型::参数, 字段::所属作用域, true)) {
                新任务执行参数要求 参数{子, {}}; auto& 规格 = 参数.规格;
                规格.参数角色节点 = 引用(子, 字段::角色); 规格.存在概念节点 = 可选引用(子, 字段::存在概念);
                规格.要求场景 = 布尔(子, 字段::要求场景);
                规格.对象种类 = 枚举值<新方法参数对象种类>(子, 字段::对象种类, 1, 7);
                for (const auto 特 : 子组(子, 字段::特征规格, 类型::参数特征, 字段::所属参数, true)) {
                    新方法参数特征规格 特征{引用(特, 字段::特征类型), 可选引用(特, 字段::值域概念),
                        引用组(特, 字段::二次特征)};
                    规范集合(特征.二次特征概念组); 规格.特征规格组.push_back(std::move(特征));
                }
                const auto 约束组 = 子组(子, 字段::集合约束, 类型::集合约束, 字段::所属参数, false);
                要求(约束组.size() <= 1, "collection constraint cardinality");
                if (!约束组.empty()) 规格.集合成员约束 = 新方法参数集合成员约束{
                    引用(约束组.front(), 字段::集合角色), 引用(约束组.front(), 字段::集合特征)};
                域.参数要求组.push_back(std::move(参数));
            }
            for (const auto 子 : 子组(节点, 字段::配对成员, 类型::配对, 字段::所属作用域, false))
                域.配对要求组.push_back(读配对(子));
            for (const auto 子 : 子组(节点, 字段::步骤成员, 类型::步骤, 字段::所属作用域, false))
                域.步骤组.push_back({子, 引用(子, 字段::来源步骤), 引用(子, 字段::调用作用域), 数(子, 字段::顺序)});
            域.入口步骤节点 = 可选引用(节点, 字段::入口步骤);
            for (const auto 子 : 子组(节点, 字段::分支成员, 类型::分支, 字段::所属作用域, false))
                域.分支组.push_back({子, 引用(子, 字段::来源转移), 引用(子, 字段::当前步骤),
                    引用(子, 字段::当前结果要求), 可选引用(子, 字段::匹配概念), 引用(子, 字段::下一步骤)});
            std::sort(域.配对要求组.begin(), 域.配对要求组.end(),
                [](const auto& 左, const auto& 右) { return 左.来源配对节点 < 右.来源配对节点; });
            std::sort(域.步骤组.begin(), 域.步骤组.end(),
                [](const auto& 左, const auto& 右) { return 左.顺序 < 右.顺序; });
            std::sort(域.分支组.begin(), 域.分支组.end(),
                [](const auto& 左, const auto& 右) { return 左.来源转移节点 < 右.来源转移节点; });
            组织.方法作用域组.push_back(std::move(域));
        }
        要求(已访问.size() == 节点组.size(), "unlisted orphan organization member");
        for (const auto& [编码, 节点] : 节点组) {
            (void)编码;
            for (const auto& 项 : 节点.字段组)
                要求(已用字段.contains(项.编码), "unknown organization field");
        }
        const auto 引用读回 = 全局基础数据集.查询节点组完整结构({引用目标.begin(), 引用目标.end()});
        要求(引用读回.状态 == 基础结构操作状态::已完成
            && 引用读回.节点组.size() == 引用目标.size(), "reference batch read failed");
        for (const auto& 项 : 引用读回.节点组) 要求(项.快照.has_value(), "dangling definition reference");
        std::sort(组织.方法作用域组.begin(), 组织.方法作用域组.end(),
            [](const auto& 左, const auto& 右) { return 左.来源方法节点 < 右.来源方法节点; });
        std::map<稳定编码, 稳定编码> 来源;
        for (const auto& 域 : 组织.方法作用域组) 来源.emplace(域.节点, 域.来源方法节点);
        std::sort(组织.根方法作用域节点组.begin(), 组织.根方法作用域节点组.end(),
            [&](auto 左, auto 右) { return 来源.at(左) < 来源.at(右); });
        return 组织;
    }
};

void 核验图(const 新任务方法执行组织& 组织) {
    要求(!组织.根方法作用域节点组.empty() && !组织.方法作用域组.empty(), "empty organization roots");
    std::map<稳定编码, const 新任务执行方法作用域*> 域表;
    std::map<稳定编码, 稳定编码> 配对所属;
    std::set<稳定编码> 方法源, 配对源, 条件源, 结果源, 步骤源, 分支源, 虚拟源;
    for (const auto& 域 : 组织.方法作用域组) {
        要求(域表.emplace(域.节点, &域).second && 方法源.insert(域.来源方法节点).second
            && 虚拟源.insert(域.来源方法虚拟存在节点).second, "duplicate method scope source");
        要求(域.作用域 == 新方法作用域::普通世界 || 域.作用域 == 新方法作用域::内部治理,
            "invalid method scope");
        要求(域.实现方式 == 新方法实现方式::本能 || 域.实现方式 == 新方法实现方式::组合,
            "invalid method implementation");
        if (域.实现方式 == 新方法实现方式::本能)
            要求(域.步骤组.empty() && 域.分支组.empty() && !域.入口步骤节点, "innate contains combination graph");
        std::map<稳定编码, const 新方法参数规格*> 角色;
        for (const auto& 参数 : 域.参数要求组)
            要求(角色.emplace(参数.规格.参数角色节点, &参数.规格).second, "duplicate parameter role");
        for (const auto& 参数 : 域.参数要求组) if (参数.规格.集合成员约束) {
            const auto& 约束 = *参数.规格.集合成员约束;
            要求(角色.contains(约束.集合参数角色节点)
                && 约束.集合参数角色节点 != 参数.规格.参数角色节点, "collection role unresolved");
            const auto& 集合角色 = *角色.at(约束.集合参数角色节点);
            要求(std::count_if(集合角色.特征规格组.begin(), 集合角色.特征规格组.end(),
                [&](const auto& 项) { return 项.特征类型概念节点 == 约束.集合特征类型概念节点; }) == 1,
                "collection feature unresolved");
        }
        for (const auto& 对 : 域.配对要求组) {
            要求(配对所属.emplace(对.节点, 域.节点).second && 配对源.insert(对.来源配对节点).second
                && 条件源.insert(对.来源条件节点).second && 结果源.insert(对.来源结果节点).second,
                "duplicate pair source");
            for (const auto& 项 : 对.条件.条件项组) 要求(角色.contains(项.参数角色节点), "unbound condition role");
            for (const auto& 项 : 对.结果.结果项组) 要求(角色.contains(项.参数角色节点), "unbound result role");
            for (const auto& 项 : 对.结果.确认要求.禁止结果项组)
                要求(角色.contains(项.参数角色节点), "unbound forbidden-result role");
            std::set<std::pair<std::size_t, std::size_t>> 线性索引;
            for (const auto& 项 : 对.线性关系组)
                要求(项.条件序号 < 对.条件.条件项组.size() && 项.结果序号 < 对.结果.结果项组.size()
                    && 线性索引.emplace(项.条件序号, 项.结果序号).second, "invalid linear pair index");
        }
    }
    std::set<稳定编码> 根组;
    for (const auto 根域 : 组织.根方法作用域节点组)
        要求(域表.contains(根域) && 根组.insert(根域).second, "invalid root scope");
    for (const auto& 域 : 组织.方法作用域组) {
        std::map<稳定编码, 稳定编码> 步骤表;
        std::set<std::int64_t> 顺序;
        for (const auto& 步 : 域.步骤组)
            要求(步骤表.emplace(步.节点, 步.调用作用域节点).second
                && 步骤源.insert(步.来源步骤节点).second && 域表.contains(步.调用作用域节点)
                && 步.顺序 >= 0 && 顺序.insert(步.顺序).second, "invalid call step");
        要求(!域.入口步骤节点 || 步骤表.contains(*域.入口步骤节点), "entry step outside scope");
        std::map<std::tuple<稳定编码, 稳定编码, std::optional<稳定编码>>, 稳定编码> 转移;
        for (const auto& 分支 : 域.分支组) {
            要求(分支源.insert(分支.来源转移节点).second && 步骤表.contains(分支.当前步骤节点)
                && 步骤表.contains(分支.下一步骤节点) && 配对所属.contains(分支.当前结果要求节点)
                && 配对所属.at(分支.当前结果要求节点) == 步骤表.at(分支.当前步骤节点),
                "branch result not in current call scope");
            const auto [位置, 新增] = 转移.emplace(std::tuple{分支.当前步骤节点,
                分支.当前结果要求节点, 分支.匹配结果概念节点}, 分支.下一步骤节点);
            要求(新增 || 位置->second == 分支.下一步骤节点, "ambiguous identical branch key");
        }
    }
    // 闭包只包含从根可达的方法定义；调用边可成环，包含关系不可成环。
    std::set<稳定编码> 到达;
    std::vector<稳定编码> 待查 = 组织.根方法作用域节点组;
    while (!待查.empty()) {
        const auto 当前 = 待查.back(); 待查.pop_back();
        if (!到达.insert(当前).second) continue;
        for (const auto& 步 : 域表.at(当前)->步骤组) 待查.push_back(步.调用作用域节点);
    }
    要求(到达.size() == 域表.size(), "unreachable method scope");
}

std::optional<新任务方法执行组织> 读取已加锁(稳定编码 任务, 稳定编码 虚拟存在,
    std::map<稳定编码, 基础节点结构>* 原始结构 = nullptr) {
    const auto 反查 = 全局基础数据集.查询字段引用节点结构(任务, 取(字段::所属任务));
    要求(反查.状态 == 基础结构操作状态::已完成, "task organization reverse read failed");
    要求(反查.引用字段组.size() <= 1, "task has multiple organizations");
    if (反查.引用字段组.empty()) return {};
    读器 读取器(反查.引用字段组.front().所属节点);
    auto 组织 = 读取器.读取();
    要求(组织.任务节点 == 任务 && 组织.任务虚拟存在节点 == 虚拟存在, "organization host mismatch");
    核验图(组织);
    if (原始结构) *原始结构 = std::move(读取器.节点组);
    return 组织;
}

void 核验概念引用(const 新任务方法执行组织& 组织,
    const 概念_特征类& 特征, const 概念_存在类& 存在, const 概念_二次特征类& 二次) {
    const auto 存在概念 = [&](const std::optional<稳定编码>& 节点) {
        要求(!节点 || 存在.是存在概念节点(*节点), "invalid existence concept reference");
    };
    const auto 特征概念 = [&](const std::optional<稳定编码>& 节点) {
        // 存在概念可作为复合特征类型/值域；不能排除这条已确定的语义。
        要求(!节点 || 特征.是特征概念节点(*节点) || 存在.是存在概念节点(*节点),
            "invalid feature/domain concept reference");
    };
    const auto 二次概念 = [&](const std::optional<稳定编码>& 节点) {
        要求(!节点 || 二次.是二次特征概念(*节点), "invalid secondary concept reference");
    };
    const auto 精确域 = [&](const auto& 项) {
        if (!项.精确数值域) return;
        要求(项.特征类型概念节点 && !项.特征值域概念节点
            && 项.精确数值域->下界 <= 项.精确数值域->上界, "invalid exact domain");
        const auto 定义 = 特征.获取特征概念(*项.特征类型概念节点);
        要求(定义 && 定义->材料类型 == 特征概念材料物理类型::I64标量,
            "exact domain requires I64 feature type");
    };
    const auto 结果组 = [&](const auto& 组) {
        for (const auto& 项 : 组) {
            特征概念(项.特征类型概念节点); 特征概念(项.特征值域概念节点);
            二次概念(项.变化方向概念节点); 精确域(项);
        }
    };
    for (const auto& 域 : 组织.方法作用域组) {
        for (const auto& 参数 : 域.参数要求组) {
            存在概念(参数.规格.存在概念节点);
            for (const auto& 项 : 参数.规格.特征规格组) {
                特征概念(项.特征类型概念节点); 特征概念(项.值域概念节点);
                for (const auto 节点 : 项.二次特征概念组) 二次概念(节点);
            }
            if (参数.规格.集合成员约束) 特征概念(参数.规格.集合成员约束->集合特征类型概念节点);
        }
        for (const auto& 对 : 域.配对要求组) {
            for (const auto& 项 : 对.条件.条件项组) {
                存在概念(项.存在概念节点); 特征概念(项.特征类型概念节点);
                特征概念(项.特征值域概念节点); 二次概念(项.二次特征概念节点); 精确域(项);
            }
            结果组(对.结果.结果项组); 结果组(对.结果.确认要求.禁止结果项组);
        }
    }
    // 状态/动态、等待、匹配概念尚无完整类型提供者；保留方法闭包具名引用，
    // 完整读回检查引用存在，但不伪称已完成其语义求值或授予执行许可。
}

// 旧任务 getters 会 catch-all；本边界必须先诊断终止，不让损坏降格为空组织。
std::optional<稳定编码> 读取定位投影(稳定编码 任务, 稳定编码 虚拟存在) noexcept {
    try {
        const auto 组织 = 读取已加锁(任务, 虚拟存在);
        return 组织 ? std::optional<稳定编码>{组织->节点} : std::nullopt;
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"任务执行组织定位投影", L"组织读取故障", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"任务执行组织定位投影", L"未知组织读取故障");
    }
}
struct 来源方法视图 final {
    稳定编码 节点, 作用域节点;
    新方法启用状态 启用状态=新方法启用状态::启用;
    新方法实现方式 实现方式=新方法实现方式::本能;
    std::optional<std::string> 本能函数登记名;
    std::vector<新方法参数规格> 参数规格组;
    std::vector<新方法条件结果信息> 条件结果组;
};
新任务本能采用材料 取得采用定义(const 新任务方法执行组织& 组织,
    const 新任务本能采用定位& 定位) {
    要求(组织.节点==定位.组织节点,"采用组织身份不一致");
    const auto 域=std::find_if(组织.方法作用域组.begin(),组织.方法作用域组.end(),
        [&](const auto& 项){return 项.节点==定位.方法作用域节点;});
    要求(域!=组织.方法作用域组.end()&&域->实现方式==新方法实现方式::本能
        &&std::find(组织.根方法作用域节点组.begin(),组织.根方法作用域节点组.end(),域->节点)
            !=组织.根方法作用域节点组.end(),"采用不是本能根作用域");
    const auto 对=std::find_if(域->配对要求组.begin(),域->配对要求组.end(),
        [&](const auto& 项){return 项.节点==定位.配对要求节点;});
    要求(对!=域->配对要求组.end(),"采用配对不属于根作用域");
    新任务本能采用材料 采用;
    采用.定位=定位;采用.来源方法节点=域->来源方法节点;采用.来源配对节点=对->来源配对节点;
    采用.来源条件节点=对->来源条件节点;采用.来源结果节点=对->来源结果节点;
    for(const auto& 参数:域->参数要求组)采用.参数要求组.push_back(参数.规格);
    采用.原始条件=对->条件;采用.需要条件=对->条件;采用.结果=对->结果;采用.线性关系组=对->线性关系组;
    return 采用;
}
void 核对采用定义(const 新任务方法执行组织& 组织,const 新任务本能采用材料& 采用) {
    auto 原定义=取得采用定义(组织,采用.定位);
    原定义.需要条件=采用.需要条件;原定义.参数绑定组=采用.参数绑定组;
    要求(原定义==采用,"采用源稳定定义不一致");
    要求(采用.需要条件.关系规则==采用.原始条件.关系规则
        &&采用.需要条件.条件项组.size()==采用.原始条件.条件项组.size(),"有效条件结构异义");
    for(std::size_t 序号=0;序号<采用.原始条件.条件项组.size();++序号){
        auto 原项=采用.原始条件.条件项组[序号];const auto& 有效项=采用.需要条件.条件项组[序号];
        原项.精确数值域=有效项.精确数值域;原项.特征值域概念节点=有效项.特征值域概念节点;
        要求(原项==有效项,"有效条件改写非值字段");
    }
}
std::optional<来源方法视图> 读取来源视图(const 新任务方法执行组织& 组织,
    稳定编码 来源M,const 新_方法类& 方法服务,const 新任务本能采用材料* 固定采用=nullptr) {
    const auto 域=std::find_if(组织.方法作用域组.begin(),组织.方法作用域组.end(),
        [&](const auto& 项){return 项.来源方法节点==来源M;});
    要求(域!=组织.方法作用域组.end(),"来源方法无任务要求");
    if(固定采用){核对采用定义(组织,*固定采用);要求(固定采用->定位.方法作用域节点==域->节点,"固定采用作用域异义");}
    const auto M=方法服务.获取方法(来源M);
    要求(M.has_value(),"已冻结来源方法读取失败");
    要求(!M->来源方法节点&&!M->实例根节点,"任务要求引用不是抽象方法");
    来源方法视图 视图;视图.节点=来源M;视图.作用域节点=域->节点;视图.启用状态=M->启用状态;
    视图.实现方式=域->实现方式;视图.本能函数登记名=M->本能函数登记名;
    for(const auto& 项:域->参数要求组)视图.参数规格组.push_back(项.规格);
    要求(M->参数规格组==视图.参数规格组&&M->实现方式==域->实现方式,"源参数或实现方式同编码异义");
    for(const auto& 项:域->配对要求组){
        if(固定采用&&项.节点!=固定采用->定位.配对要求节点)continue;
        新方法条件结果信息 对;
        对.配对节点=项.来源配对节点;对.条件节点=项.来源条件节点;对.结果节点=项.来源结果节点;
        对.条件 = 固定采用 ? 固定采用->需要条件 : 项.条件;对.结果=项.结果;对.线性关系组=项.线性关系组;
        视图.条件结果组.push_back(std::move(对));
    }
    return 视图;
}
std::optional<新方法条件定义> 计算采用需要条件(const 新任务本能采用材料& 采用,
    const 新需求信息& 需求,const 概念_特征类& 特征概念,const 概念_二次特征类& 二次,
    const 新_特征类& 特征) {
    auto 条件=采用.原始条件;
    if(!是条件包含目标(需求.目标,特征概念,二次))return 条件;
    const 新方法结果项定义* 目标项=nullptr;
    for(const auto& 项:采用.结果.结果项组){
        if(!项.特征类型概念节点||取得特征概念类型根(特征概念,*项.特征类型概念节点)
            !=std::optional<稳定编码>{需求.目标.目标特征类型概念节点})continue;
        const auto 绑定=std::find_if(采用.参数绑定组.begin(),采用.参数绑定组.end(),[&](const auto& 值){return 值.参数角色节点==项.参数角色节点;});
        if(绑定==采用.参数绑定组.end()||绑定->实际存在节点!=需求.目标存在节点)continue;
        if(采用.结果.关系规则==新方法项关系规则::主要结果及允许副作用&&!项.主要结果)continue;
        if(目标项)return {};目标项=&项;
    }
    if(!目标项||(!目标项->精确数值域&&!目标项->特征值域概念节点))return 条件;
    const auto 类型=特征概念.获取特征概念(*目标项->特征类型概念节点);
    if(!类型)throw std::logic_error("采用结果类型读取失败");
    if(类型->材料类型!=特征概念材料物理类型::I64标量)return 条件;
    const auto 结果输入=取得任务值域输入(特征概念,*目标项->特征类型概念节点,目标项->特征值域概念节点,目标项->精确数值域);
    if(!结果输入||!需求.目标.目标值域概念节点)return {};
    const auto 比较=特征.比较特征值域(*结果输入,{需求.目标.目标特征类型概念节点,类型->单位,
        新特征概念值域比较材料{*需求.目标.目标值域概念节点}});
    if(比较.状态!=新特征比较状态::已完成||!比较.关系编码)throw std::logic_error("采用目标域比较故障");
    if(*比较.关系编码!=static_cast<std::int64_t>(特征概念值域关系::左包含右)
        &&*比较.关系编码!=static_cast<std::int64_t>(特征概念值域关系::相交))return 条件;
    const auto 单域=[&](const std::optional<稳定编码>& 概念,const std::optional<特征概念I64闭区间>& 精确)->std::optional<特征概念I64闭区间>{
        if(精确)return 精确;if(!概念)return {};
        const auto 定义=特征概念.获取特征概念(*概念);if(!定义)throw std::logic_error("采用值域读取失败");
        const auto* 域=std::get_if<std::vector<特征概念I64闭区间>>(&定义->值域);
        return 域 && 域->size() == 1 ? std::optional<特征概念I64闭区间>{域->front()} : std::nullopt;
    };
    const auto 结果域=单域(目标项->特征值域概念节点,目标项->精确数值域),目标域=单域(需求.目标.目标值域概念节点,{});
    if(!结果域||!目标域)return {};
    const auto 目标序号=static_cast<std::size_t>(目标项-采用.结果.结果项组.data());
    新任务线性条件输入请求 请求;请求.条件域组.resize(条件.条件项组.size());
    请求.结果约束组.push_back({目标序号,*结果域,*目标域});std::vector<bool> 已覆盖(条件.条件项组.size(),false);
    for(const auto& 映射:采用.线性关系组){
        if(映射.结果序号!=目标序号)continue;
        if(映射.条件序号>=请求.条件域组.size()||映射.原结果域!=*结果域)throw std::logic_error("采用线性定义异义");
        请求.条件域组[映射.条件序号]=映射.原条件域;已覆盖[映射.条件序号]=true;
        新任务条件结果线性方向 方向;
        switch(映射.方向){
        case 新方法条件结果线性方向::同向:方向=新任务条件结果线性方向::同向;break;
        case 新方法条件结果线性方向::反向:方向=新任务条件结果线性方向::反向;break;
        case 新方法条件结果线性方向::不变:方向=新任务条件结果线性方向::不变;break;
        default:throw std::logic_error("采用线性方向非法");
        }
        请求.关系组.push_back({映射.条件序号,映射.结果序号,方向});
    }
    if(已覆盖.empty()||std::any_of(已覆盖.begin(),已覆盖.end(),[](bool 值){return !值;}))return {};
    const auto 反求=新_任务类::按线性关系计算任务条件输入(请求);
    if(反求.状态==新任务线性条件输入状态::资源失败)throw std::bad_alloc{};
    if(反求.状态!=新任务线性条件输入状态::已算出||反求.条件输入组.size()!=条件.条件项组.size())return {};
    for(std::size_t 序号=0;序号<条件.条件项组.size();++序号){
        auto& 项=条件.条件项组[序号];const auto 原域=单域(项.特征值域概念节点,项.精确数值域);if(!原域)return {};
        auto 域=反求.条件输入组[序号].需要值域;
        域.下界=(std::max)(域.下界,原域->下界);域.上界=(std::min)(域.上界,原域->上界);if(域.下界>域.上界)return {};
        项.特征值域概念节点.reset();项.精确数值域=域;
    }
    return 条件;
}
void 建立执行采用已加锁(稳定编码 X,const 新任务本能采用材料& 采用) {
    要求(全局基础数据集.查询字段(X,取(字段::执行采用引用)).empty(),"执行采用不能重复发布");
    const auto 原结构=全局基础数据集.查询完整节点结构(X);
    要求(原结构.状态==基础结构操作状态::已完成&&原结构.结构.has_value(),"执行构造结构读取失败");
    写器 写;写.执行采用模式=true;
    const auto 根=写.新节点(类型::执行采用);
    写.引用(根,字段::采用所属执行,X);写.引用(根,字段::采用组织,采用.定位.组织节点);
    写.引用(根,字段::采用作用域,采用.定位.方法作用域节点);写.引用(根,字段::采用配对,采用.定位.配对要求节点);
    写.数(根,字段::条件关系,static_cast<std::int64_t>(采用.需要条件.关系规则));
    for(std::size_t 序=0;序<采用.需要条件.条件项组.size();++序){
        const auto& 项=采用.需要条件.条件项组[序];
        const auto 子=写.子节点(类型::执行条件,根,字段::条件成员,字段::所属配对,序号(序));
        写.引用(子,字段::角色,项.参数角色节点);写.引用(子,字段::存在概念,项.存在概念节点);
        写.引用(子,字段::特征类型,项.特征类型概念节点);写.引用(子,字段::值域概念,项.特征值域概念节点);
        写.引用(子,字段::二次特征,项.二次特征概念节点);写.引用(子,字段::状态动态概念,项.状态或动态概念节点);
        写.值域(子,项.精确数值域);
    }
    for(std::size_t 序=0;序<采用.参数绑定组.size();++序){
        const auto& 项=采用.参数绑定组[序];
        const auto 子=写.子节点(类型::执行绑定,根,字段::绑定成员,字段::绑定所属采用,序号(序));
        写.引用(子,字段::角色,项.参数角色节点);写.引用(子,字段::世界场景,项.世界场景节点);
        写.引用(子,字段::实际存在,项.实际存在节点);
        for(const auto& 特征:项.实际特征组)写.引用(子,字段::实际特征,特征.特征节点);
        if(项.任务虚拟存在引用){
            写.引用(子,字段::引用任务,项.任务虚拟存在引用->任务节点);
            写.引用(子,字段::引用虚拟存在,项.任务虚拟存在引用->虚拟存在节点);
            写.引用(子,字段::引用状态特征,项.任务虚拟存在引用->状态特征节点);
        }
        if(项.服务要求分项引用){写.引用(子,字段::服务理解,项.服务要求分项引用->理解节点);写.数(子,字段::服务分项,static_cast<std::int64_t>(项.服务要求分项引用->分项));}
        if(项.服务要求确认引用)写.引用(子,字段::服务确认,项.服务要求确认引用->确认节点);
    }
    基础节点组追加请求 请求;请求.新节点组=std::move(写.节点组);
    请求.预期既有节点组.push_back(*原结构.结构);
    请求.追加字段组.push_back({X,取(字段::执行采用引用),基础组内节点引用{根}});
    const auto 发布=全局基础数据集.原子追加节点组(请求);
    要求(发布.状态==基础节点组追加状态::已完成,"执行采用原子追加失败");
}
std::optional<新任务本能采用材料> 读取执行采用已加锁(稳定编码 X,稳定编码 T,稳定编码 M) {
    const auto 采用字段=全局基础数据集.查询字段(X,取(字段::执行采用引用));
    if(采用字段.empty())return {};
    要求(采用字段.size()==1&&std::get_if<稳定编码>(&采用字段.front().内容),"执行采用字段错误");
    const auto 根=std::get<稳定编码>(采用字段.front().内容);
    读器 读(根,true);读.开节点(根,类型::执行采用);
    要求(读.引用(根,字段::采用所属执行)==X,"执行采用所属X不一致");
    新任务本能采用定位 定位{读.引用(根,字段::采用组织),读.引用(根,字段::采用作用域),读.引用(根,字段::采用配对)};
    const auto 虚拟字段=全局基础数据集.查询字段(T,任务虚拟存在字段);
    要求(虚拟字段.size()==1&&std::get_if<稳定编码>(&虚拟字段.front().内容),"执行任务虚拟存在缺失");
    const auto 组织=读取已加锁(T,std::get<稳定编码>(虚拟字段.front().内容));
    要求(组织.has_value(),"执行采用组织已丢失");
    auto 采用=取得采用定义(*组织,定位);
    要求(采用.来源方法节点==M,"执行采用真实M不一致");
    采用.需要条件.关系规则=读.枚举值<新方法项关系规则>(根,字段::条件关系,1,6);
    采用.需要条件.条件项组.clear();
    for(const auto 子:读.子组(根,字段::条件成员,类型::执行条件,字段::所属配对,true)){
        新方法条件项定义 项;项.参数角色节点=读.引用(子,字段::角色);
        项.存在概念节点=读.可选引用(子,字段::存在概念);项.特征类型概念节点=读.可选引用(子,字段::特征类型);
        项.特征值域概念节点=读.可选引用(子,字段::值域概念);项.二次特征概念节点=读.可选引用(子,字段::二次特征);
        项.状态或动态概念节点=读.可选引用(子,字段::状态动态概念);项.精确数值域=读.值域(子);
        要求(!(项.精确数值域&&项.特征值域概念节点),"执行条件双值域");
        采用.需要条件.条件项组.push_back(std::move(项));
    }
    std::set<稳定编码> 已有角色;
    for(const auto 子:读.子组(根,字段::绑定成员,类型::执行绑定,字段::绑定所属采用,true)){
        新方法参数角色绑定 项;项.参数角色节点=读.引用(子,字段::角色);
        要求(已有角色.insert(项.参数角色节点).second,"执行绑定角色重复");
        项.世界场景节点=读.可选引用(子,字段::世界场景);项.实际存在节点=读.可选引用(子,字段::实际存在);
        for(const auto F:读.引用组(子,字段::实际特征))项.实际特征组.push_back({F});
        std::sort(项.实际特征组.begin(),项.实际特征组.end(),[](const auto& 左,const auto& 右){return 左.特征节点<右.特征节点;});
        const auto 任务=读.可选引用(子,字段::引用任务),虚拟=读.可选引用(子,字段::引用虚拟存在),状态F=读.可选引用(子,字段::引用状态特征);
        要求(任务.has_value()==虚拟.has_value()&&任务.has_value()==状态F.has_value(),"虚拟绑定半引用");
        if(任务)项.任务虚拟存在引用=新任务虚拟存在引用{*任务,*虚拟,*状态F};
        const auto 理解=读.可选引用(子,字段::服务理解);const auto 分项=读.可选数(子,字段::服务分项);
        要求(理解.has_value()==分项.has_value(),"服务分项半引用");
        if(理解){要求(*分项>=1&&*分项<=4,"服务分项枚举非法");项.服务要求分项引用=新服务要求分项引用{*理解,static_cast<新服务要求分项种类>(*分项)};}
        const auto 确认=读.可选引用(子,字段::服务确认);if(确认)项.服务要求确认引用=新服务要求确认引用{*确认};
        const auto 分支数=static_cast<unsigned>(项.实际存在节点.has_value())+static_cast<unsigned>(项.任务虚拟存在引用.has_value())
            +static_cast<unsigned>(项.服务要求分项引用.has_value())+static_cast<unsigned>(项.服务要求确认引用.has_value());
        要求(分支数==1,"执行绑定类型组合非法");采用.参数绑定组.push_back(std::move(项));
    }
    要求(读.已访问.size()==读.节点组.size(),"执行采用孤儿子节点");
    for(const auto& [编码,内容]:读.节点组){(void)编码;for(const auto& 字:内容.字段组)要求(读.已用字段.contains(字.编码),"执行采用未知字段");}
    要求(采用.参数绑定组.size()==采用.参数要求组.size(),"执行采用角色集不完整");
    for(const auto& 规格:采用.参数要求组){
        const auto 位置=std::find_if(采用.参数绑定组.begin(),采用.参数绑定组.end(),[&](const auto& 项){return 项.参数角色节点==规格.参数角色节点;});
        要求(位置!=采用.参数绑定组.end(),"执行采用未声明角色");const auto& 绑定=*位置;
        switch(规格.对象种类){
        case 新方法参数对象种类::普通世界:
            要求(绑定.实际存在节点&&!绑定.任务虚拟存在引用&&!绑定.服务要求分项引用&&!绑定.服务要求确认引用
                &&(!规格.要求场景||绑定.世界场景节点)&&绑定.实际特征组.size()==规格.特征规格组.size(),"普通世界绑定形状不符");break;
        case 新方法参数对象种类::任务虚拟存在:
            要求(绑定.任务虚拟存在引用&&!绑定.世界场景节点&&!绑定.实际存在节点&&!绑定.服务要求分项引用&&!绑定.服务要求确认引用
                &&绑定.实际特征组.size()==1&&绑定.实际特征组.front().特征节点==绑定.任务虚拟存在引用->状态特征节点,"任务虚拟绑定形状不符");break;
        case 新方法参数对象种类::服务要求来源人类:
        case 新方法参数对象种类::服务要求目标存在:
        case 新方法参数对象种类::服务要求目标特征类型:
        case 新方法参数对象种类::服务要求目标定义:
            要求(绑定.服务要求分项引用&&!绑定.世界场景节点&&!绑定.实际存在节点&&!绑定.任务虚拟存在引用&&!绑定.服务要求确认引用
                &&绑定.实际特征组.empty()&&static_cast<std::int64_t>(绑定.服务要求分项引用->分项)==static_cast<std::int64_t>(规格.对象种类)-2,"服务分项绑定形状不符");break;
        case 新方法参数对象种类::服务要求确认:
            要求(绑定.服务要求确认引用&&!绑定.世界场景节点&&!绑定.实际存在节点&&!绑定.任务虚拟存在引用&&!绑定.服务要求分项引用&&绑定.实际特征组.empty(),"服务确认绑定形状不符");break;
        default:要求(false,"未知执行参数种类");
        }
    }
    const auto 引用读回=全局基础数据集.查询节点组完整结构({读.引用目标.begin(),读.引用目标.end()});
    要求(引用读回.状态==基础结构操作状态::已完成&&引用读回.节点组.size()==读.引用目标.size(),"执行采用引用批读失败");
    for(const auto& 项:引用读回.节点组)要求(项.快照.has_value(),"执行采用引用失效");
    核对采用定义(*组织,采用);return 采用;
}
void 清理执行采用已加锁(稳定编码 X) {
    const auto 字段组=全局基础数据集.查询字段(X,取(字段::执行采用引用));
    if(字段组.empty())return;
    要求(字段组.size()==1&&std::get_if<稳定编码>(&字段组.front().内容),"清理执行采用根损坏");
    const auto 读取X引用=[&](稳定编码 字段码){
        const auto 组=全局基础数据集.查询字段(X,字段码);
        要求(组.size()==1&&std::get_if<稳定编码>(&组.front().内容),"清理采用的X结构损坏");return std::get<稳定编码>(组.front().内容);
    };
    要求(读取X引用(节点类型字段)==任务执行实例节点类型,"清理采用的宿主不是X");
    const auto 已核验=读取执行采用已加锁(X,读取X引用(执行所属任务字段),读取X引用(执行方法字段));
    要求(已核验.has_value(),"清理采用前完整owner核验失败");
    读器 读(std::get<稳定编码>(字段组.front().内容),true);
    std::vector<稳定编码> 节点组,字段关系组;
    for(const auto& [编码,内容]:读.节点组){节点组.push_back(编码);for(const auto& 字:内容.字段组)字段关系组.push_back(字.编码);}
    // X仍保留构造/撤销权；先精确解除它的唯一引用，再删除本X独占结构。
    要求(全局基础数据集.删除字段(字段组.front().编码),"执行采用X引用解除失败");
    要求(全局基础数据集.原子删除节点结构(节点组,字段关系组,{})==基础结构操作状态::已完成,"执行采用独占结构删除失败");
}
} // namespace 任务执行组织存储
方法私有域闭包检查状态 检查方法私有域闭包(const 新_方法类&,
    const 任务执行组织存储::来源方法视图& 方法) noexcept {
    return std::any_of(方法.条件结果组.begin(),方法.条件结果组.end(),
        [](const auto& 对){return 含私有数值域(对);})
        ?方法私有域闭包检查状态::含私有域:方法私有域闭包检查状态::未含私有域;
}
稳定编码 任务集合根类型{};
稳定编码 任务节点类型{};
稳定编码 任务虚拟存在节点类型{};
稳定编码 任务执行实例节点类型{};
稳定编码 任务预计变化阶段节点类型{};
稳定编码 任务预计变化项节点类型{};
稳定编码 任务结果获取渠道方案节点类型{};
稳定编码 任务结果获取目标节点类型{};
稳定编码 任务禁止结果目标节点类型{};
稳定编码 任务实际状态项节点类型{};
稳定编码 任务影响评估节点类型{};
稳定编码 任务完成结算节点类型{};
稳定编码 任务候选方法材料节点类型{};

稳定编码 任务成员字段{};
稳定编码 任务需求字段{};
稳定编码 任务虚拟存在字段{};
稳定编码 任务最近方法字段{};
稳定编码 任务方法实例字段{};
稳定编码 任务最近执行字段{};
稳定编码 任务影响评估字段{};
稳定编码 任务完成结算字段{};
稳定编码 任务候选方法材料字段{};
稳定编码 任务待回收候选方法材料编码字段{};

稳定编码 候选所属任务字段{};
稳定编码 候选需求字段{};
稳定编码 候选目标特征类型字段{};
稳定编码 候选目标形式字段{};
稳定编码 候选目标值域字段{};
稳定编码 候选目标比较关系字段{};
稳定编码 候选目标变化方向字段{};
稳定编码 候选查询作用域字段{};
稳定编码 候选查询实现方式字段{};
稳定编码 候选查询作用存在字段{};
稳定编码 候选查询特征类型字段{};
稳定编码 候选查询可能结果字段{};
稳定编码 候选查询状态字段{};
稳定编码 候选查询完整字段{};
稳定编码 候选方法字段{};

稳定编码 虚拟存在所属任务字段{};
稳定编码 虚拟存在状态特征字段{};
稳定编码 虚拟存在专属概念字段{};

稳定编码 执行所属任务字段{};
稳定编码 执行方法字段{};
稳定编码 执行状态字段{};
稳定编码 执行开始时间语义字段{};
稳定编码 执行开始时间字段{};
稳定编码 执行结束时间语义字段{};
稳定编码 执行结束时间字段{};
稳定编码 执行预计阶段字段{};
稳定编码 执行结果获取多渠道关系字段{};
稳定编码 执行结果获取渠道方案字段{};
稳定编码 执行结果获取目标字段{};
稳定编码 执行禁止结果关系规则字段{};
稳定编码 执行禁止结果目标字段{};
稳定编码 执行动作输出状态字段{};
稳定编码 执行实际状态项字段{};
稳定编码 执行评估字段{};

稳定编码 阶段顺序字段{};
稳定编码 阶段变化项字段{};
稳定编码 变化作用对象字段{};
稳定编码 变化特征类型字段{};
稳定编码 变化关系字段{};
稳定编码 变化方向符号字段{};
稳定编码 变化值域字段{};
稳定编码 变化精确数值域字段{};
稳定编码 变化必须出现字段{};

稳定编码 结果目标顺序字段{};
稳定编码 结果目标作用对象字段{};
稳定编码 结果目标实际特征字段{};
稳定编码 结果目标特征类型根字段{};
稳定编码 结果目标动作前准确值字段{};
稳定编码 结果目标必须出现字段{};

稳定编码 渠道方案顺序字段{};
稳定编码 渠道方案类型字段{};
稳定编码 渠道方案证据质量字段{};
稳定编码 渠道方案最大等待纳秒字段{};
稳定编码 渠道方案等待条件概念字段{};

稳定编码 禁止目标顺序字段{};
稳定编码 禁止目标作用对象字段{};
稳定编码 禁止目标实际特征字段{};
稳定编码 禁止目标特征类型根字段{};
稳定编码 禁止目标动作前准确值字段{};
稳定编码 禁止目标方向字段{};
稳定编码 禁止目标值域字段{};
稳定编码 禁止目标精确数值域字段{};
稳定编码 禁止目标状态动态字段{};

稳定编码 实际状态顺序字段{};
稳定编码 实际状态节点字段{};

稳定编码 评估所属任务字段{};
稳定编码 评估需求字段{};
稳定编码 评估执行实例字段{};
稳定编码 评估方法字段{};
稳定编码 评估需求结果状态字段{};
稳定编码 评估需求实际特征字段{};
稳定编码 评估需求准确差异字段{};
稳定编码 评估符合预测状态字段{};
稳定编码 评估缺失变化项字段{};
稳定编码 评估计划外状态字段{};
稳定编码 评估方向验证字段{};
稳定编码 评估幅度验证字段{};
稳定编码 评估值域验证字段{};
稳定编码 评估多渠道验证字段{};
稳定编码 评估禁止结果验证字段{};
稳定编码 评估副作用验证字段{};
稳定编码 评估归因验证字段{};

稳定编码 结算所属任务字段{};
稳定编码 结算需求字段{};
稳定编码 结算执行实例字段{};
稳定编码 结算影响评估字段{};
稳定编码 结算完成来源字段{};
稳定编码 结算结果状态字段{};

稳定编码 任务状态特征概念{};
稳定编码 任务已结算动作定义类型{};
稳定编码 任务内部动作定义字段{};
稳定编码 内部动作操作码字段{};
稳定编码 结算来源任务字段{};
稳定编码 结算来源虚拟存在字段{};
稳定编码 结算来源状态特征字段{};
稳定编码 结算冻结前值字段{};
稳定编码 结算目标值域字段{};
稳定编码 执行普通动作字段{};

bool 节点仍存在(稳定编码 节点) noexcept {
    return 有效(节点) && 全局基础数据集.查询节点(节点).has_value();
}

bool 是任务状态值(std::int64_t 值) noexcept {
    return 值 >= static_cast<std::int64_t>(新任务状态::已建立)
        && 值 <= static_cast<std::int64_t>(新任务状态::已结算);
}

bool 是执行状态值(std::int64_t 值) noexcept {
    return 值 >= static_cast<std::int64_t>(新任务执行状态::执行中)
        && 值 <= static_cast<std::int64_t>(新任务执行状态::已评估);
}

bool 是影响验证状态值(std::int64_t 值) noexcept {
    return 值 >= static_cast<std::int64_t>(新任务影响验证状态::未验证)
        && 值 <= static_cast<std::int64_t>(新任务影响验证状态::冲突);
}

bool 是预期变化方向值(std::int64_t 值) noexcept {
    return 值 >= static_cast<std::int64_t>(新任务预期变化方向::减少)
        && 值 <= static_cast<std::int64_t>(新任务预期变化方向::增加);
}

bool 是结果获取渠道值(std::int64_t 值) noexcept {
    return 值 >= static_cast<std::int64_t>(新任务结果获取渠道::世界树状态通知)
        && 值 <= static_cast<std::int64_t>(新任务结果获取渠道::他人正式陈述);
}

bool 是必要证据质量值(std::int64_t 值) noexcept {
    return 值 >= static_cast<std::int64_t>(
               新任务必要证据质量::正式间接证据)
        && 值 <= static_cast<std::int64_t>(
               新任务必要证据质量::正式直接事实);
}

bool 是多渠道关系值(std::int64_t 值) noexcept {
    return 值 >= static_cast<std::int64_t>(新任务多渠道关系::任一满足)
        && 值 <= static_cast<std::int64_t>(新任务多渠道关系::优先备用);
}

bool 是方法项关系规则值(std::int64_t 值) noexcept {
    return 值 >= static_cast<std::int64_t>(新方法项关系规则::单项)
        && 值 <= static_cast<std::int64_t>(
               新方法项关系规则::主要结果及允许副作用);
}

bool 是完成来源值(std::int64_t 值) noexcept {
    return 值 == static_cast<std::int64_t>(新任务完成来源::自我后继决议);
}

bool 是终结状态(新任务状态 状态) noexcept {
    return 状态 == 新任务状态::已完成
        || 状态 == 新任务状态::已失败
        || 状态 == 新任务状态::已取消
        || 状态 == 新任务状态::已结算;
}

bool 是合法状态迁移(新任务状态 原状态, 新任务状态 新状态) noexcept {
    if (原状态 == 新状态) return true;
    if (是终结状态(原状态)) return false;
    switch (原状态) {
    case 新任务状态::已建立:
        return 新状态 == 新任务状态::待查找方法
            || 新状态 == 新任务状态::等待条件
            || 新状态 == 新任务状态::已取消;
    case 新任务状态::待查找方法:
        return 新状态 == 新任务状态::待检查条件
            || 新状态 == 新任务状态::等待条件
            || 新状态 == 新任务状态::已失败
            || 新状态 == 新任务状态::已取消;
    case 新任务状态::待检查条件:
        return 新状态 == 新任务状态::待执行
            || 新状态 == 新任务状态::待查找方法
            || 新状态 == 新任务状态::等待条件
            || 新状态 == 新任务状态::已失败
            || 新状态 == 新任务状态::已取消;
    case 新任务状态::待执行:
        return 新状态 == 新任务状态::执行中
            || 新状态 == 新任务状态::待查找方法
            || 新状态 == 新任务状态::等待条件
            || 新状态 == 新任务状态::已取消;
    case 新任务状态::执行中:
        return 新状态 == 新任务状态::待影响评估;
    case 新任务状态::待影响评估:
        return 新状态 == 新任务状态::等待自我决议;
    case 新任务状态::等待自我决议:
        return 新状态 == 新任务状态::待查找方法
            || 新状态 == 新任务状态::待检查条件
            || 新状态 == 新任务状态::待执行
            || 新状态 == 新任务状态::等待条件
            || 新状态 == 新任务状态::已完成
            || 新状态 == 新任务状态::已失败
            || 新状态 == 新任务状态::已取消;
    case 新任务状态::等待条件:
        return 新状态 == 新任务状态::待查找方法
            || 新状态 == 新任务状态::待检查条件
            || 新状态 == 新任务状态::待执行
            || 新状态 == 新任务状态::已失败
            || 新状态 == 新任务状态::已取消;
    case 新任务状态::已完成:
    case 新任务状态::已失败:
    case 新任务状态::已取消:
    case 新任务状态::已结算:
        return false;
    }
    return false;
}

bool 强时间有效(const 新状态强时间& 时间) noexcept {
    return 时间.语义 == 新状态时间语义::实例绝对UTC && 时间.纳秒 >= 0;
}

bool 结构已初始化() {
    if (!任务执行组织存储::已分配()) return false;
    const std::vector<稳定编码> 节点组{
        任务已结算动作定义类型, 任务内部动作定义字段, 内部动作操作码字段,
        结算来源任务字段, 结算来源虚拟存在字段, 结算来源状态特征字段,
        结算冻结前值字段, 结算目标值域字段, 执行普通动作字段,
        任务集合根节点, 节点类型字段, 任务集合根类型, 任务节点类型,
        任务虚拟存在节点类型, 任务执行实例节点类型,
        任务预计变化阶段节点类型, 任务预计变化项节点类型,
        任务结果获取渠道方案节点类型, 任务结果获取目标节点类型,
        任务禁止结果目标节点类型,
        任务实际状态项节点类型, 任务影响评估节点类型,
        任务完成结算节点类型,
        任务候选方法材料节点类型,
        任务成员字段, 任务需求字段, 任务虚拟存在字段,
        任务最近方法字段, 任务方法实例字段, 任务最近执行字段, 任务影响评估字段,
        任务完成结算字段,
        任务候选方法材料字段, 任务待回收候选方法材料编码字段,
        候选所属任务字段, 候选需求字段,
        候选目标特征类型字段, 候选目标形式字段,
        候选目标值域字段, 候选目标比较关系字段,
        候选目标变化方向字段, 候选查询作用域字段,
        候选查询实现方式字段, 候选查询作用存在字段,
        候选查询特征类型字段, 候选查询可能结果字段,
        候选查询状态字段, 候选查询完整字段, 候选方法字段,
        虚拟存在所属任务字段, 虚拟存在状态特征字段, 虚拟存在专属概念字段,
        执行所属任务字段, 执行方法字段, 执行状态字段,
        执行开始时间语义字段, 执行开始时间字段,
        执行结束时间语义字段, 执行结束时间字段,
        执行预计阶段字段, 执行结果获取多渠道关系字段,
        执行结果获取渠道方案字段, 执行结果获取目标字段,
        执行禁止结果关系规则字段, 执行禁止结果目标字段,
        执行动作输出状态字段, 执行实际状态项字段, 执行评估字段,
        阶段顺序字段, 阶段变化项字段, 变化作用对象字段,
        变化特征类型字段, 变化关系字段, 变化方向符号字段,
        变化值域字段, 变化精确数值域字段,
        变化必须出现字段, 结果目标顺序字段,
        结果目标作用对象字段, 结果目标实际特征字段,
        结果目标特征类型根字段, 结果目标动作前准确值字段,
        结果目标必须出现字段, 渠道方案顺序字段,
        渠道方案类型字段, 渠道方案证据质量字段,
        渠道方案最大等待纳秒字段, 渠道方案等待条件概念字段,
        禁止目标顺序字段, 禁止目标作用对象字段,
        禁止目标实际特征字段, 禁止目标特征类型根字段,
        禁止目标动作前准确值字段, 禁止目标方向字段,
        禁止目标值域字段, 禁止目标精确数值域字段, 禁止目标状态动态字段,
        实际状态顺序字段, 实际状态节点字段,
        评估所属任务字段, 评估需求字段, 评估执行实例字段,
        评估方法字段, 评估需求结果状态字段, 评估需求实际特征字段,
        评估需求准确差异字段,
        评估符合预测状态字段, 评估缺失变化项字段,
        评估计划外状态字段, 评估方向验证字段, 评估幅度验证字段,
        评估值域验证字段, 评估多渠道验证字段,
        评估禁止结果验证字段, 评估副作用验证字段,
        评估归因验证字段, 结算所属任务字段, 结算需求字段,
        结算执行实例字段, 结算影响评估字段, 结算完成来源字段,
        结算结果状态字段, 任务状态特征概念};
    return std::all_of(节点组.begin(), 节点组.end(), [](稳定编码 节点) {
        return 有效(节点);
    });
}

std::optional<稳定编码> 读取唯一节点字段(
    稳定编码 节点, 稳定编码 字段) {
    const auto 字段组 = 全局基础数据集.查询字段(节点, 字段);
    if (字段组.size() != 1) return std::nullopt;
    const auto* 目标 = std::get_if<稳定编码>(&字段组.front().内容);
    return 目标 && 节点仍存在(*目标)
        ? std::optional<稳定编码>{*目标} : std::nullopt;
}

struct 唯一节点字段记录 final {
    稳定编码 关系;
    稳定编码 目标;
};

std::optional<唯一节点字段记录> 读取唯一节点字段记录(
    稳定编码 节点, 稳定编码 字段) {
    const auto 字段组 = 全局基础数据集.查询字段(节点, 字段);
    if (字段组.size() != 1) return std::nullopt;
    const auto* 目标 = std::get_if<稳定编码>(&字段组.front().内容);
    return 目标 && 节点仍存在(*目标)
        ? std::optional<唯一节点字段记录>{{字段组.front().编码, *目标}}
        : std::nullopt;
}

struct 唯一I64字段记录 final {
    稳定编码 关系;
    std::int64_t 值 = 0;
};

std::optional<唯一I64字段记录> 读取唯一I64字段记录(
    稳定编码 节点, 稳定编码 字段) {
    const auto 字段组 = 全局基础数据集.查询字段(节点, 字段);
    if (字段组.size() != 1) return std::nullopt;
    const auto* 值 = std::get_if<基础值>(&字段组.front().内容);
    const auto* I64 = 值 ? std::get_if<std::int64_t>(&值->材料) : nullptr;
    return I64
        ? std::optional<唯一I64字段记录>{{字段组.front().编码, *I64}}
        : std::nullopt;
}

std::optional<新特征准确值> 读取唯一准确值字段(
    稳定编码 节点, 稳定编码 字段) {
    const auto 字段组 = 全局基础数据集.查询字段(节点, 字段);
    if (字段组.size() != 1) return std::nullopt;
    if (const auto* 目标 = std::get_if<稳定编码>(&字段组.front().内容)) {
        return 节点仍存在(*目标)
            ? std::optional<新特征准确值>{新特征准确值{*目标}}
            : std::nullopt;
    }
    const auto* 值 = std::get_if<基础值>(&字段组.front().内容);
    const auto* I64 = 值 ? std::get_if<std::int64_t>(&值->材料) : nullptr;
    return I64
        ? std::optional<新特征准确值>{新特征准确值{*I64}}
        : std::nullopt;
}

// 外层空表示字段破损；内层空表示旧记录未声明直接域。
std::optional<std::optional<特征概念I64闭区间>> 读取任务精确域(
    稳定编码 节点, 稳定编码 字段) {
    const auto 组 = 全局基础数据集.查询字段(节点, 字段);
    if (组.empty()) return std::optional<特征概念I64闭区间>{};
    if (组.size() != 1) return std::nullopt;
    const auto* 值 = std::get_if<基础值>(&组.front().内容);
    const auto* 向量 = 值 ? std::get_if<std::vector<std::int64_t>>(&值->材料) : nullptr;
    if (!向量 || 向量->size() != 2) return std::nullopt;
    特征概念I64闭区间 域{(*向量)[0], (*向量)[1]};
    if (!精确域有效(域)) return std::nullopt;
    return std::optional<特征概念I64闭区间>{域};
}

bool 写入任务精确域(稳定编码 节点, 稳定编码 字段,
    const 特征概念I64闭区间& 域, std::vector<稳定编码>& 已写) {
    if (!精确域有效(域)) return false;
    已写.reserve(已写.size() + 1);
    const auto 新字段 = 全局基础数据集.添加字段值(节点, 字段,
        基础值{std::vector<std::int64_t>{域.下界, 域.上界}});
    if (!有效(新字段)) return false;
    已写.push_back(新字段);
    return true;
}

std::vector<稳定编码> 读取多节点字段(稳定编码 节点, 稳定编码 字段) {
    std::vector<稳定编码> 结果;
    for (const auto& 关系 : 全局基础数据集.查询字段(节点, 字段)) {
        const auto* 目标 = std::get_if<稳定编码>(&关系.内容);
        if (!目标 || !节点仍存在(*目标)) return {};
        结果.push_back(*目标);
    }
    std::sort(结果.begin(), 结果.end());
    结果.erase(std::unique(结果.begin(), 结果.end()), 结果.end());
    return 结果;
}

bool 节点类型为(稳定编码 节点, 稳定编码 类型) {
    const auto 实际类型 = 读取唯一节点字段(节点, 节点类型字段);
    return 实际类型 && *实际类型 == 类型;
}

std::optional<std::vector<稳定编码>> 读取多节点字段严格(
    稳定编码 节点, 稳定编码 字段) {
    std::vector<稳定编码> 结果;
    for (const auto& 关系 : 全局基础数据集.查询字段(节点, 字段)) {
        const auto* 目标 = std::get_if<稳定编码>(&关系.内容);
        if (!目标 || !节点仍存在(*目标)) return std::nullopt;
        结果.push_back(*目标);
    }
    std::sort(结果.begin(), 结果.end());
    if (std::adjacent_find(结果.begin(), 结果.end()) != 结果.end()) {
        return std::nullopt;
    }
    return 结果;
}

void 排序去重(std::vector<稳定编码>& 节点组) {
    std::sort(节点组.begin(), 节点组.end());
    节点组.erase(std::unique(节点组.begin(), 节点组.end()), 节点组.end());
}

bool 候选查询相同(
    const 新方法候选查询& 左, const 新方法候选查询& 右) noexcept {
    return 左.作用域 == 右.作用域
        && 左.实现方式 == 右.实现方式
        && 左.作用存在概念组 == 右.作用存在概念组
        && 左.涉及特征类型概念组 == 右.涉及特征类型概念组
        && 左.可能结果概念组 == 右.可能结果概念组;
}

bool 候选材料内容相同(
    const 新任务候选方法材料& 现有,
    const 新任务候选方法发布请求& 请求) noexcept {
    return 现有.任务节点 == 请求.任务节点
        && 现有.需求节点 == 请求.需求节点
        && 现有.需求目标 == 请求.需求目标
        && 候选查询相同(现有.查询, 请求.查询)
        && 现有.查询状态 == 请求.查询状态
        && 现有.查询完整 == 请求.查询完整
        && 现有.候选方法节点组 == 请求.候选方法节点组;
}

std::optional<新任务候选方法材料> 读取候选方法材料信息(
    稳定编码 材料节点) {
    if (!节点类型为(材料节点, 任务候选方法材料节点类型)) {
        return std::nullopt;
    }
    const auto 任务 = 读取唯一节点字段(材料节点, 候选所属任务字段);
    const auto 需求 = 读取唯一节点字段(材料节点, 候选需求字段);
    const auto 目标类型 = 读取唯一节点字段(
        材料节点, 候选目标特征类型字段);
    const auto 目标形式值 = 读取唯一I64字段记录(
        材料节点, 候选目标形式字段);
    const auto 查询状态 = 读取唯一I64字段记录(材料节点, 候选查询状态字段);
    const auto 查询完整 = 读取唯一I64字段记录(材料节点, 候选查询完整字段);
    if (!任务 || !需求 || !目标类型 || !目标形式值
        || (目标形式值->值 != static_cast<std::int64_t>(新需求目标形式::值)
            && 目标形式值->值 != static_cast<std::int64_t>(
                新需求目标形式::变化方向))
        || !查询状态 || 查询状态->值 != static_cast<std::int64_t>(
            新方法操作状态::已找到)
        || !查询完整 || 查询完整->值 != 1) return std::nullopt;

    新任务候选方法材料 结果;
    结果.节点 = 材料节点;
    结果.任务节点 = *任务;
    结果.需求节点 = *需求;
    结果.需求目标.目标特征类型概念节点 = *目标类型;
    结果.需求目标.目标形式 = static_cast<新需求目标形式>(
        目标形式值->值);
    const auto 值域组 = 全局基础数据集.查询字段(
        材料节点, 候选目标值域字段);
    const auto 关系组 = 全局基础数据集.查询字段(
        材料节点, 候选目标比较关系字段);
    const auto 方向组 = 全局基础数据集.查询字段(
        材料节点, 候选目标变化方向字段);
    if (值域组.size() > 1 || 关系组.size() > 1 || 方向组.size() > 1) {
        return std::nullopt;
    }
    if (!值域组.empty()) {
        结果.需求目标.目标值域概念节点 = 读取唯一节点字段(
            材料节点, 候选目标值域字段);
    }
    if (!关系组.empty()) {
        结果.需求目标.目标比较关系概念节点 = 读取唯一节点字段(
            材料节点, 候选目标比较关系字段);
    }
    if (!方向组.empty()) {
        const auto 方向值 = 读取唯一I64字段记录(
            材料节点, 候选目标变化方向字段);
        if (!方向值 || (方向值->值 != static_cast<std::int64_t>(
                新需求变化方向::增加)
            && 方向值->值 != static_cast<std::int64_t>(
                新需求变化方向::减少))) {
            return std::nullopt;
        }
        结果.需求目标.目标变化方向 = static_cast<新需求变化方向>(
            方向值->值);
    }
    const bool 值形状 = 结果.需求目标.目标形式 == 新需求目标形式::值
        && 结果.需求目标.目标值域概念节点
        && 结果.需求目标.目标比较关系概念节点
        && !结果.需求目标.目标变化方向;
    const bool 方向形状 = 结果.需求目标.目标形式
            == 新需求目标形式::变化方向
        && !结果.需求目标.目标值域概念节点
        && !结果.需求目标.目标比较关系概念节点
        && 结果.需求目标.目标变化方向;
    if (!值形状 && !方向形状) return std::nullopt;
    结果.查询状态 = 新方法操作状态::已找到;
    结果.查询完整 = true;

    const auto 作用域组 = 全局基础数据集.查询字段(
        材料节点, 候选查询作用域字段);
    const auto 实现组 = 全局基础数据集.查询字段(
        材料节点, 候选查询实现方式字段);
    if (作用域组.size() > 1 || 实现组.size() > 1) return std::nullopt;
    if (!作用域组.empty()) {
        const auto 作用域 = 读取唯一I64字段记录(材料节点, 候选查询作用域字段);
        if (!作用域 || 作用域->值 < static_cast<std::int64_t>(新方法作用域::内部治理)
            || 作用域->值 > static_cast<std::int64_t>(新方法作用域::普通世界)) {
            return std::nullopt;
        }
        结果.查询.作用域 = static_cast<新方法作用域>(作用域->值);
    }
    if (!实现组.empty()) {
        const auto 实现 = 读取唯一I64字段记录(材料节点, 候选查询实现方式字段);
        if (!实现 || 实现->值 < static_cast<std::int64_t>(新方法实现方式::本能)
            || 实现->值 > static_cast<std::int64_t>(新方法实现方式::组合)) {
            return std::nullopt;
        }
        结果.查询.实现方式 = static_cast<新方法实现方式>(实现->值);
    }
    const auto 作用存在 = 读取多节点字段严格(
        材料节点, 候选查询作用存在字段);
    const auto 特征类型 = 读取多节点字段严格(
        材料节点, 候选查询特征类型字段);
    const auto 可能结果 = 读取多节点字段严格(
        材料节点, 候选查询可能结果字段);
    const auto 候选方法 = 读取多节点字段严格(材料节点, 候选方法字段);
    if (!作用存在 || !特征类型 || !可能结果 || !候选方法) {
        return std::nullopt;
    }
    结果.查询.作用存在概念组 = *作用存在;
    结果.查询.涉及特征类型概念组 = *特征类型;
    结果.查询.可能结果概念组 = *可能结果;
    结果.候选方法节点组 = *候选方法;
    return 结果;
}

bool 删除候选方法材料节点(稳定编码 材料节点) noexcept {
    if (!全局基础数据集.查询节点(材料节点)) return true;
    const std::vector<稳定编码> 字段节点组{
        节点类型字段, 候选所属任务字段, 候选需求字段,
        候选目标特征类型字段, 候选目标形式字段,
        候选目标值域字段, 候选目标比较关系字段,
        候选目标变化方向字段, 候选查询作用域字段,
        候选查询实现方式字段, 候选查询作用存在字段,
        候选查询特征类型字段, 候选查询可能结果字段,
        候选查询状态字段, 候选查询完整字段, 候选方法字段};
    bool 字段已清空 = true;
    for (const auto 字段节点 : 字段节点组) {
        const auto 字段组 = 全局基础数据集.查询字段(材料节点, 字段节点);
        for (const auto& 字段 : 字段组) {
            if (!全局基础数据集.删除字段(字段.编码)
                && 全局基础数据集.查询字段关系(字段.编码)) {
                字段已清空 = false;
            }
        }
    }
    if (!字段已清空) return false;
    return !全局基础数据集.查询节点(材料节点)
        || 全局基础数据集.删除节点(材料节点);
}

struct 待回收候选材料记录 final {
    稳定编码 字段关系;
    稳定编码 材料节点;
};

std::optional<待回收候选材料记录> 读取待回收候选材料(
    稳定编码 任务节点) {
    const auto 字段组 = 全局基础数据集.查询字段(
        任务节点, 任务待回收候选方法材料编码字段);
    if (字段组.empty()) return std::nullopt;
    if (字段组.size() != 1) throw std::logic_error("待回收候选材料定位不唯一");
    const auto* 值 = std::get_if<基础值>(&字段组.front().内容);
    const auto* 编码组 = 值
        ? std::get_if<std::vector<std::uint64_t>>(&值->材料) : nullptr;
    if (!编码组 || 编码组->size() != 1 || 编码组->front() == 0) {
        throw std::logic_error("待回收候选材料定位损坏");
    }
    return 待回收候选材料记录{
        字段组.front().编码, 稳定编码{编码组->front()}};
}

bool 登记待回收候选材料(
    稳定编码 任务节点, 稳定编码 材料节点) noexcept {
    try {
        if (读取待回收候选材料(任务节点)) return false;
        return 有效(全局基础数据集.添加字段值(
            任务节点, 任务待回收候选方法材料编码字段,
            基础值{std::vector<std::uint64_t>{材料节点.值}}));
    } catch (...) {
        return false;
    }
}

bool 收口待回收候选材料(稳定编码 任务节点) noexcept {
    try {
        const auto 待回收 = 读取待回收候选材料(任务节点);
        if (!待回收) return true;

        const auto 当前字段组 = 全局基础数据集.查询字段(
            任务节点, 任务候选方法材料字段);
        if (当前字段组.size() > 1) return false;
        if (!当前字段组.empty()) {
            const auto* 当前节点 = std::get_if<稳定编码>(
                &当前字段组.front().内容);
            if (!当前节点) return false;
            // 切换未发生时，待回收定位只是预写保护；撤销定位，保留当前材料。
            if (*当前节点 == 待回收->材料节点) {
                return 全局基础数据集.删除字段(待回收->字段关系);
            }
        }

        // 编码定位是直接值，不会阻止节点物理删除；失败时仍可据此重试。
        if (!删除候选方法材料节点(待回收->材料节点)) return false;
        return 全局基础数据集.删除字段(待回收->字段关系);
    } catch (...) {
        return false;
    }
}

bool 结构仍存在() {
    if (!结构已初始化()) return false;
    if (!任务执行组织存储::仍存在()) return false;
    const std::vector<稳定编码> 节点组{
        任务已结算动作定义类型, 任务内部动作定义字段, 内部动作操作码字段,
        结算来源任务字段, 结算来源虚拟存在字段, 结算来源状态特征字段,
        结算冻结前值字段, 结算目标值域字段, 执行普通动作字段,
        任务集合根节点, 节点类型字段, 任务集合根类型, 任务节点类型,
        任务虚拟存在节点类型, 任务执行实例节点类型,
        任务预计变化阶段节点类型, 任务预计变化项节点类型,
        任务结果获取渠道方案节点类型, 任务结果获取目标节点类型,
        任务禁止结果目标节点类型,
        任务实际状态项节点类型, 任务影响评估节点类型,
        任务完成结算节点类型, 任务候选方法材料节点类型,
        任务成员字段, 任务需求字段, 任务虚拟存在字段,
        任务最近方法字段, 任务方法实例字段, 任务最近执行字段, 任务影响评估字段,
        任务完成结算字段, 任务候选方法材料字段,
        任务待回收候选方法材料编码字段,
        候选所属任务字段, 候选需求字段,
        候选目标特征类型字段, 候选目标形式字段,
        候选目标值域字段, 候选目标比较关系字段,
        候选目标变化方向字段, 候选查询作用域字段,
        候选查询实现方式字段, 候选查询作用存在字段,
        候选查询特征类型字段, 候选查询可能结果字段,
        候选查询状态字段, 候选查询完整字段, 候选方法字段,
        虚拟存在所属任务字段, 虚拟存在状态特征字段, 虚拟存在专属概念字段,
        执行所属任务字段, 执行方法字段, 执行状态字段,
        执行开始时间语义字段, 执行开始时间字段,
        执行结束时间语义字段, 执行结束时间字段,
        执行预计阶段字段, 执行结果获取多渠道关系字段,
        执行结果获取渠道方案字段, 执行结果获取目标字段,
        执行禁止结果关系规则字段, 执行禁止结果目标字段,
        执行动作输出状态字段, 执行实际状态项字段, 执行评估字段,
        阶段顺序字段, 阶段变化项字段, 变化作用对象字段,
        变化特征类型字段, 变化关系字段, 变化方向符号字段,
        变化值域字段, 变化精确数值域字段,
        变化必须出现字段, 结果目标顺序字段,
        结果目标作用对象字段, 结果目标实际特征字段,
        结果目标特征类型根字段, 结果目标动作前准确值字段,
        结果目标必须出现字段, 渠道方案顺序字段,
        渠道方案类型字段, 渠道方案证据质量字段,
        渠道方案最大等待纳秒字段, 渠道方案等待条件概念字段,
        禁止目标顺序字段, 禁止目标作用对象字段,
        禁止目标实际特征字段, 禁止目标特征类型根字段,
        禁止目标动作前准确值字段, 禁止目标方向字段,
        禁止目标值域字段, 禁止目标精确数值域字段, 禁止目标状态动态字段,
        实际状态顺序字段, 实际状态节点字段,
        评估所属任务字段, 评估需求字段, 评估执行实例字段,
        评估方法字段, 评估需求结果状态字段, 评估需求实际特征字段,
        评估需求准确差异字段,
        评估符合预测状态字段, 评估缺失变化项字段,
        评估计划外状态字段, 评估方向验证字段, 评估幅度验证字段,
        评估值域验证字段, 评估多渠道验证字段,
        评估禁止结果验证字段, 评估副作用验证字段,
        评估归因验证字段, 结算所属任务字段, 结算需求字段,
        结算执行实例字段, 结算影响评估字段, 结算完成来源字段,
        结算结果状态字段, 任务状态特征概念};
    return std::all_of(节点组.begin(), 节点组.end(), 节点仍存在)
        && 节点类型为(任务集合根节点, 任务集合根类型);
}

std::optional<新任务状态> 读取任务状态(
    稳定编码 状态特征节点, const 新_特征类& 特征服务) {
    const auto 特征 = 特征服务.获取特征(状态特征节点);
    if (!特征 || !特征->当前值) return std::nullopt;
    const auto* I64 = std::get_if<std::int64_t>(&*特征->当前值);
    if (!I64 || !是任务状态值(*I64)) return std::nullopt;
    return static_cast<新任务状态>(*I64);
}

std::optional<新任务虚拟存在信息> 读取任务虚拟存在信息(
    稳定编码 任务节点, const 新_特征类& 特征服务) {
    if (!节点类型为(任务节点, 任务节点类型)) return std::nullopt;
    const auto 虚拟存在 = 读取唯一节点字段(任务节点, 任务虚拟存在字段);
    if (!虚拟存在 || !节点类型为(*虚拟存在, 任务虚拟存在节点类型)) {
        return std::nullopt;
    }
    const auto 所属任务 = 读取唯一节点字段(*虚拟存在, 虚拟存在所属任务字段);
    const auto 状态特征 = 读取唯一节点字段(*虚拟存在, 虚拟存在状态特征字段);
    if (!所属任务 || *所属任务 != 任务节点 || !状态特征
        || !特征服务.是特征节点(*状态特征)) return std::nullopt;
    const auto 状态 = 读取任务状态(*状态特征, 特征服务);
    if (!状态) return std::nullopt;
    return 新任务虚拟存在信息{
        *虚拟存在, 任务节点, *状态特征, *状态,
        任务执行组织存储::读取定位投影(任务节点, *虚拟存在)};
}

bool 读取服务结算来源字段(
    稳定编码 节点, std::optional<新任务服务结算来源绑定>& 结果) {
    结果.reset();
    std::size_t 数量 = 0;
    for (const auto 字段 : {结算来源任务字段, 结算来源虚拟存在字段,
            结算来源状态特征字段, 结算冻结前值字段, 结算目标值域字段}) {
        const auto 组 = 全局基础数据集.查询字段(节点, 字段);
        if (组.size() > 1) return false;
        数量 += 组.size();
    }
    if (数量 == 0) return true;
    if (数量 != 5) return false;
    const auto 任务 = 读取唯一节点字段(节点, 结算来源任务字段);
    const auto 虚拟 = 读取唯一节点字段(节点, 结算来源虚拟存在字段);
    const auto 特征 = 读取唯一节点字段(节点, 结算来源状态特征字段);
    const auto 前值 = 读取唯一I64字段记录(节点, 结算冻结前值字段);
    const auto 目标 = 读取唯一节点字段(节点, 结算目标值域字段);
    if (!任务 || !虚拟 || !特征 || !前值 || 前值->值 < 0 || !目标
        || !节点类型为(*任务, 任务节点类型)
        || !节点类型为(*虚拟, 任务虚拟存在节点类型)
        || 读取唯一节点字段(*任务, 任务虚拟存在字段) != 虚拟
        || 读取唯一节点字段(*虚拟, 虚拟存在所属任务字段) != 任务
        || 读取唯一节点字段(*虚拟, 虚拟存在状态特征字段) != 特征) return false;
    结果 = 新任务服务结算来源绑定{{*任务, *虚拟, *特征}, 前值->值, *目标};
    return true;
}

std::optional<新任务信息> 读取任务信息(
    稳定编码 任务节点, const 新_特征类& 特征服务) {
    if (!结构仍存在() || !节点类型为(任务节点, 任务节点类型)) {
        return std::nullopt;
    }
    const auto 需求 = 读取唯一节点字段(任务节点, 任务需求字段);
    const auto 虚拟存在 = 读取任务虚拟存在信息(任务节点, 特征服务);
    if (!需求 || !虚拟存在) return std::nullopt;
    新任务信息 结果;
    结果.节点 = 任务节点;
    结果.需求节点 = *需求;
    结果.任务虚拟存在节点 = 虚拟存在->节点;
    结果.方法执行组织节点 = 虚拟存在->方法执行组织节点;
    结果.当前状态 = 虚拟存在->当前状态;
    if (!读取服务结算来源字段(任务节点, 结果.服务结算来源)) return std::nullopt;
    const auto 候选材料组 = 全局基础数据集.查询字段(
        任务节点, 任务候选方法材料字段);
    const auto 方法组 = 全局基础数据集.查询字段(任务节点, 任务最近方法字段);
    const auto 实例组 = 全局基础数据集.查询字段(任务节点, 任务方法实例字段);
    if (实例组.size() > 1) return std::nullopt;
    if (!实例组.empty()) {
        const auto* 实例 = std::get_if<稳定编码>(&实例组.front().内容);
        if (!实例 || !节点仍存在(*实例)) return std::nullopt;
        const auto 引用组 = 全局基础数据集.查询目标字段(*实例, 任务方法实例字段);
        if (引用组.size() != 1 || 引用组.front().所属节点 != 任务节点) return std::nullopt;
        结果.方法实例节点 = *实例;
    }
    const auto 执行组 = 全局基础数据集.查询字段(任务节点, 任务最近执行字段);
    const auto 结算组 = 全局基础数据集.查询字段(
        任务节点, 任务完成结算字段);
    if (候选材料组.size() > 1 || 方法组.size() > 1 || 执行组.size() > 1
        || 结算组.size() > 1) {
        return std::nullopt;
    }
    if (!候选材料组.empty()) {
        const auto* 材料 = std::get_if<稳定编码>(&候选材料组.front().内容);
        const auto 读回 = 材料
            ? 读取候选方法材料信息(*材料) : std::nullopt;
        if (!读回 || 读回->任务节点 != 任务节点
            || 读回->需求节点 != *需求) return std::nullopt;
        结果.当前候选方法材料节点 = *材料;
    }
    if (!方法组.empty()) {
        const auto* 方法 = std::get_if<稳定编码>(&方法组.front().内容);
        if (!方法 || !节点仍存在(*方法)) return std::nullopt;
        结果.最近方法节点 = *方法;
    }
    if (!执行组.empty()) {
        const auto* 执行 = std::get_if<稳定编码>(&执行组.front().内容);
        if (!执行 || !节点类型为(*执行, 任务执行实例节点类型)) {
            return std::nullopt;
        }
        结果.最近执行实例节点 = *执行;
    }
    结果.影响评估节点组 = 读取多节点字段(任务节点, 任务影响评估字段);
    if (!结算组.empty()) {
        const auto* 结算 = std::get_if<稳定编码>(&结算组.front().内容);
        if (!结算 || !节点类型为(*结算, 任务完成结算节点类型)) {
            return std::nullopt;
        }
        结果.完成结算节点 = *结算;
    }
    return 结果;
}

void 回滚字段(const std::vector<稳定编码>& 字段组) noexcept {
    for (auto 位置 = 字段组.rbegin(); 位置 != 字段组.rend(); ++位置) {
        (void)全局基础数据集.删除字段(*位置);
    }
}

bool 写入节点字段(
    稳定编码 节点, 稳定编码 字段, 稳定编码 目标,
    std::vector<稳定编码>& 已写字段) {
    const auto 关系 = 全局基础数据集.添加字段节点(节点, 字段, 目标);
    if (!有效(关系)) return false;
    已写字段.push_back(关系);
    return true;
}

bool 写入I64字段(
    稳定编码 节点, 稳定编码 字段, std::int64_t 值,
    std::vector<稳定编码>& 已写字段) {
    const auto 关系 = 全局基础数据集.添加字段值(节点, 字段, 基础值{值});
    if (!有效(关系)) return false;
    已写字段.push_back(关系);
    return true;
}

bool 写入服务结算来源字段(
    稳定编码 节点, const 新任务服务结算来源绑定& 绑定,
    std::vector<稳定编码>& 已写字段) {
    已写字段.reserve(已写字段.size() + 5);
    return 写入节点字段(节点, 结算来源任务字段, 绑定.来源任务.任务节点, 已写字段)
        && 写入节点字段(节点, 结算来源虚拟存在字段,
            绑定.来源任务.虚拟存在节点, 已写字段)
        && 写入节点字段(节点, 结算来源状态特征字段,
            绑定.来源任务.状态特征节点, 已写字段)
        && 写入I64字段(节点, 结算冻结前值字段, 绑定.冻结前值, 已写字段)
        && 写入节点字段(节点, 结算目标值域字段, 绑定.目标值域概念节点, 已写字段);
}

bool 写入准确值字段(
    稳定编码 节点, 稳定编码 字段, const 新特征准确值& 值,
    std::vector<稳定编码>& 已写字段) {
    if (const auto* I64 = std::get_if<std::int64_t>(&值)) {
        return 写入I64字段(节点, 字段, *I64, 已写字段);
    }
    const auto* 值节点 = std::get_if<稳定编码>(&值);
    return 值节点 && 节点仍存在(*值节点)
        && 写入节点字段(节点, 字段, *值节点, 已写字段);
}

新任务操作状态 写入已核验任务状态值(
    const 新任务虚拟存在信息& 虚拟存在,
    新任务状态 新状态,
    新_特征类& 特征服务) {
    if (虚拟存在.当前状态 == 新状态) return 新任务操作状态::无变化;
    const auto 写入 = 特征服务.添加特征值(
        虚拟存在.任务状态特征节点,
        新特征准确值{static_cast<std::int64_t>(新状态)});
    if (写入.状态 == 新特征写入状态::已完成) {
        return 新任务操作状态::已修改;
    }
    if (写入.状态 == 新特征写入状态::无变化) {
        return 新任务操作状态::无变化;
    }
    if (写入.状态 == 新特征写入状态::结构不一致
        || 写入.状态 == 新特征写入状态::值已更新但聚合未完成) {
        // 特征写入会先更新当前值，再维护历史聚合材料。若后半段失败，
        // 不能把已经生效的任务状态当作未发生并回滚其它任务结构。
        // 当前值是任务状态的正式事实；精确读回后据此收敛结果。
        const auto 已写状态 = 读取任务状态(
            虚拟存在.任务状态特征节点, 特征服务);
        if (已写状态 && *已写状态 == 新状态) {
            return 新任务操作状态::已修改;
        }
        return 新任务操作状态::结构不一致;
    }
    return 新任务操作状态::资源失败;
}

新任务操作状态 写入任务状态(
    const 新任务虚拟存在信息& 虚拟存在,
    新任务状态 新状态,
    新_特征类& 特征服务) {
    if (!是合法状态迁移(虚拟存在.当前状态, 新状态)) {
        return 新任务操作状态::任务状态不允许;
    }
    return 写入已核验任务状态值(虚拟存在, 新状态, 特征服务);
}

bool 预计变化定义有效(const std::vector<新任务预计变化阶段>& 阶段组) {
    if (阶段组.empty()) return false;
    std::vector<新任务预计变化阶段> 排序 = 阶段组;
    std::sort(排序.begin(), 排序.end(), [](const auto& 左, const auto& 右) {
        return 左.顺序 < 右.顺序;
    });
    for (std::size_t i = 0; i < 排序.size(); ++i) {
        if (排序[i].顺序 != static_cast<std::int64_t>(i)
            || 排序[i].变化组.empty()) return false;
        std::set<std::pair<std::uint64_t, std::uint64_t>> 已见;
        for (const auto& 变化 : 排序[i].变化组) {
            if (变化.预期精确数值域 && (变化.预期值域概念节点
                || !精确域有效(*变化.预期精确数值域))) return false;
            if (!节点仍存在(变化.作用对象节点)
                || !节点仍存在(变化.特征类型概念节点)
                || (变化.预期变化关系节点
                    && !节点仍存在(*变化.预期变化关系节点))
                || (变化.预期值域概念节点
                    && !节点仍存在(*变化.预期值域概念节点))) return false;
            const auto 键 = std::pair{
                变化.作用对象节点.值, 变化.特征类型概念节点.值};
            if (!已见.insert(键).second) return false;
        }
    }
    return true;
}

struct 预计项物理 final {
    稳定编码 节点;
    新任务预计特征变化 变化;
};

struct 预计阶段物理 final {
    稳定编码 节点;
    std::int64_t 顺序 = 0;
    std::vector<预计项物理> 变化组;
};

std::optional<std::vector<预计阶段物理>> 读取预计阶段物理组(
    稳定编码 执行节点) {
    std::vector<预计阶段物理> 结果;
    for (const auto& 持有 : 全局基础数据集.查询字段(
        执行节点, 执行预计阶段字段)) {
        const auto* 阶段节点 = std::get_if<稳定编码>(&持有.内容);
        if (!阶段节点 || !节点类型为(*阶段节点, 任务预计变化阶段节点类型)) {
            return std::nullopt;
        }
        const auto 顺序 = 读取唯一I64字段记录(*阶段节点, 阶段顺序字段);
        if (!顺序 || 顺序->值 < 0) return std::nullopt;
        预计阶段物理 阶段{*阶段节点, 顺序->值, {}};
        for (const auto& 项持有 : 全局基础数据集.查询字段(
            *阶段节点, 阶段变化项字段)) {
            const auto* 项节点 = std::get_if<稳定编码>(&项持有.内容);
            if (!项节点 || !节点类型为(*项节点, 任务预计变化项节点类型)) {
                return std::nullopt;
            }
            const auto 对象 = 读取唯一节点字段(*项节点, 变化作用对象字段);
            const auto 类型 = 读取唯一节点字段(*项节点, 变化特征类型字段);
            const auto 必须 = 读取唯一I64字段记录(*项节点, 变化必须出现字段);
            if (!对象 || !类型 || !必须 || (必须->值 != 0 && 必须->值 != 1)) {
                return std::nullopt;
            }
            新任务预计特征变化 变化;
            变化.作用对象节点 = *对象;
            变化.特征类型概念节点 = *类型;
            变化.必须出现 = 必须->值 == 1;
            const auto 关系组 = 全局基础数据集.查询字段(*项节点, 变化关系字段);
            const auto 方向组 = 全局基础数据集.查询字段(
                *项节点, 变化方向符号字段);
            const auto 值域组 = 全局基础数据集.查询字段(*项节点, 变化值域字段);
            if (关系组.size() > 1 || 方向组.size() > 1
                || 值域组.size() > 1) return std::nullopt;
            if (!关系组.empty()) {
                const auto* 关系 = std::get_if<稳定编码>(&关系组.front().内容);
                if (!关系 || !节点仍存在(*关系)) return std::nullopt;
                变化.预期变化关系节点 = *关系;
            }
            if (!方向组.empty()) {
                const auto 方向 = 读取唯一I64字段记录(
                    *项节点, 变化方向符号字段);
                if (!方向 || !是预期变化方向值(方向->值)) return std::nullopt;
                变化.预期方向 = static_cast<新任务预期变化方向>(方向->值);
            }
            if (!值域组.empty()) {
                const auto* 值域 = std::get_if<稳定编码>(&值域组.front().内容);
                if (!值域 || !节点仍存在(*值域)) return std::nullopt;
                变化.预期值域概念节点 = *值域;
            }
            const auto 精确域 = 读取任务精确域(*项节点, 变化精确数值域字段);
            if (!精确域 || (*精确域 && 变化.预期值域概念节点)
                || (变化.预期方向 && !变化.预期变化关系节点
                    && !变化.预期值域概念节点 && !*精确域)) return std::nullopt;
            变化.预期精确数值域 = *精确域;
            阶段.变化组.push_back({*项节点, std::move(变化)});
        }
        if (阶段.变化组.empty()) return std::nullopt;
        结果.push_back(std::move(阶段));
    }
    std::sort(结果.begin(), 结果.end(), [](const auto& 左, const auto& 右) {
        return 左.顺序 < 右.顺序;
    });
    for (std::size_t i = 0; i < 结果.size(); ++i) {
        if (结果[i].顺序 != static_cast<std::int64_t>(i)) return std::nullopt;
    }
    return 结果.empty()
        ? std::nullopt : std::optional<std::vector<预计阶段物理>>{std::move(结果)};
}

struct 结果获取目标物理 final {
    稳定编码 节点;
    std::int64_t 顺序 = 0;
    新任务结果获取目标 目标;
};

std::optional<新任务结果获取方案> 读取结果获取方案(
    稳定编码 执行节点) {
    const auto 多渠道关系 = 读取唯一I64字段记录(
        执行节点, 执行结果获取多渠道关系字段);
    const auto 禁止规则 = 读取唯一I64字段记录(
        执行节点, 执行禁止结果关系规则字段);
    if (!多渠道关系 || !是多渠道关系值(多渠道关系->值)
        || !禁止规则 || !是方法项关系规则值(禁止规则->值)) {
        return std::nullopt;
    }
    新任务结果获取方案 结果;
    结果.多渠道关系 =
        static_cast<新任务多渠道关系>(多渠道关系->值);
    结果.禁止结果关系规则 =
        static_cast<新方法项关系规则>(禁止规则->值);

    struct 渠道物理 final {
        std::int64_t 顺序 = 0;
        新任务结果获取渠道方案 方案;
    };
    std::vector<渠道物理> 渠道排序组;
    for (const auto& 持有 : 全局基础数据集.查询字段(
        执行节点, 执行结果获取渠道方案字段)) {
        const auto* 渠道节点 = std::get_if<稳定编码>(&持有.内容);
        if (!渠道节点 || !节点类型为(
                *渠道节点, 任务结果获取渠道方案节点类型)) {
            return std::nullopt;
        }
        const auto 顺序 = 读取唯一I64字段记录(*渠道节点, 渠道方案顺序字段);
        const auto 类型 = 读取唯一I64字段记录(*渠道节点, 渠道方案类型字段);
        const auto 质量 = 读取唯一I64字段记录(
            *渠道节点, 渠道方案证据质量字段);
        if (!顺序 || 顺序->值 < 0 || !类型 || !是结果获取渠道值(类型->值)
            || !质量 || !是必要证据质量值(质量->值)) {
            return std::nullopt;
        }
        新任务结果获取渠道方案 方案;
        方案.顺序 = 顺序->值;
        方案.渠道 = static_cast<新任务结果获取渠道>(类型->值);
        方案.证据质量 = static_cast<新任务必要证据质量>(质量->值);
        const auto 延迟字段组 = 全局基础数据集.查询字段(
            *渠道节点, 渠道方案最大等待纳秒字段);
        if (延迟字段组.size() > 1) return std::nullopt;
        if (!延迟字段组.empty()) {
            const auto 延迟 = 读取唯一I64字段记录(
                *渠道节点, 渠道方案最大等待纳秒字段);
            if (!延迟 || 延迟->值 < 0) return std::nullopt;
            方案.最大等待纳秒 = 延迟->值;
        }
        const auto 条件字段组 = 全局基础数据集.查询字段(
            *渠道节点, 渠道方案等待条件概念字段);
        if (条件字段组.size() > 1) return std::nullopt;
        if (!条件字段组.empty()) {
            const auto 条件 = 读取唯一节点字段(
                *渠道节点, 渠道方案等待条件概念字段);
            if (!条件) return std::nullopt;
            方案.等待条件概念节点 = *条件;
        }
        渠道排序组.push_back({顺序->值, std::move(方案)});
    }
    std::sort(渠道排序组.begin(), 渠道排序组.end(),
        [](const auto& 左, const auto& 右) { return 左.顺序 < 右.顺序; });
    for (std::size_t i = 0; i < 渠道排序组.size(); ++i) {
        if (渠道排序组[i].顺序 != static_cast<std::int64_t>(i)) {
            return std::nullopt;
        }
        结果.渠道方案组.push_back(std::move(渠道排序组[i].方案));
    }

    std::vector<结果获取目标物理> 排序组;
    for (const auto& 持有 : 全局基础数据集.查询字段(
        执行节点, 执行结果获取目标字段)) {
        const auto* 目标节点 = std::get_if<稳定编码>(&持有.内容);
        if (!目标节点 || !节点类型为(
                *目标节点, 任务结果获取目标节点类型)) {
            return std::nullopt;
        }
        const auto 顺序 = 读取唯一I64字段记录(
            *目标节点, 结果目标顺序字段);
        const auto 作用对象 = 读取唯一节点字段(
            *目标节点, 结果目标作用对象字段);
        const auto 实际特征 = 读取唯一节点字段(
            *目标节点, 结果目标实际特征字段);
        const auto 类型根 = 读取唯一节点字段(
            *目标节点, 结果目标特征类型根字段);
        const auto 动作前准确值 = 读取唯一准确值字段(
            *目标节点, 结果目标动作前准确值字段);
        const auto 必须 = 读取唯一I64字段记录(
            *目标节点, 结果目标必须出现字段);
        if (!顺序 || 顺序->值 < 0 || !作用对象 || !实际特征
            || !类型根 || !动作前准确值 || !必须
            || (必须->值 != 0 && 必须->值 != 1)) {
            return std::nullopt;
        }
        排序组.push_back({*目标节点, 顺序->值,
            {*作用对象, *实际特征, *类型根, *动作前准确值,
                必须->值 == 1}});
    }
    std::sort(排序组.begin(), 排序组.end(), [](const auto& 左, const auto& 右) {
        return 左.顺序 < 右.顺序;
    });
    for (std::size_t i = 0; i < 排序组.size(); ++i) {
        if (排序组[i].顺序 != static_cast<std::int64_t>(i)) {
            return std::nullopt;
        }
        结果.目标组.push_back(std::move(排序组[i].目标));
    }

    struct 禁止结果物理 final {
        std::int64_t 顺序 = 0;
        新任务禁止结果目标 目标;
    };
    std::vector<禁止结果物理> 禁止排序组;
    for (const auto& 持有 : 全局基础数据集.查询字段(
        执行节点, 执行禁止结果目标字段)) {
        const auto* 目标节点 = std::get_if<稳定编码>(&持有.内容);
        if (!目标节点 || !节点类型为(
                *目标节点, 任务禁止结果目标节点类型)) {
            return std::nullopt;
        }
        const auto 顺序 = 读取唯一I64字段记录(*目标节点, 禁止目标顺序字段);
        const auto 作用对象 = 读取唯一节点字段(
            *目标节点, 禁止目标作用对象字段);
        const auto 实际特征 = 读取唯一节点字段(
            *目标节点, 禁止目标实际特征字段);
        const auto 类型根 = 读取唯一节点字段(
            *目标节点, 禁止目标特征类型根字段);
        const auto 动作前准确值 = 读取唯一准确值字段(
            *目标节点, 禁止目标动作前准确值字段);
        if (!顺序 || 顺序->值 < 0 || !作用对象 || !实际特征
            || !类型根 || !动作前准确值) {
            return std::nullopt;
        }
        新任务禁止结果目标 目标{
            *作用对象, *实际特征, *类型根, *动作前准确值};
        const auto 方向字段组 = 全局基础数据集.查询字段(
            *目标节点, 禁止目标方向字段);
        if (方向字段组.size() > 1) return std::nullopt;
        if (!方向字段组.empty()) {
            const auto 方向 = 读取唯一I64字段记录(*目标节点, 禁止目标方向字段);
            if (!方向 || !是预期变化方向值(方向->值)) return std::nullopt;
            目标.禁止方向 = static_cast<新任务预期变化方向>(方向->值);
        }
        const std::pair<稳定编码, std::optional<稳定编码>*> 可选节点组[]{
            {禁止目标值域字段, &目标.禁止值域概念节点},
            {禁止目标状态动态字段, &目标.禁止状态或动态概念节点}};
        for (const auto& [字段, 接收] : 可选节点组) {
            const auto 字段组 = 全局基础数据集.查询字段(*目标节点, 字段);
            if (字段组.size() > 1) return std::nullopt;
            if (!字段组.empty()) {
                const auto 节点 = 读取唯一节点字段(*目标节点, 字段);
                if (!节点) return std::nullopt;
                *接收 = *节点;
            }
        }
        const auto 精确域 = 读取任务精确域(*目标节点, 禁止目标精确数值域字段);
        if (!精确域 || (*精确域 && 目标.禁止值域概念节点)) return std::nullopt;
        目标.禁止精确数值域 = *精确域;
        禁止排序组.push_back({顺序->值, std::move(目标)});
    }
    std::sort(禁止排序组.begin(), 禁止排序组.end(),
        [](const auto& 左, const auto& 右) { return 左.顺序 < 右.顺序; });
    for (std::size_t i = 0; i < 禁止排序组.size(); ++i) {
        if (禁止排序组[i].顺序 != static_cast<std::int64_t>(i)) {
            return std::nullopt;
        }
        结果.禁止结果组.push_back(std::move(禁止排序组[i].目标));
    }
    return 结果.完整()
        ? std::optional<新任务结果获取方案>{std::move(结果)}
        : std::nullopt;
}

struct 实际状态物理 final {
    稳定编码 项节点;
    稳定编码 状态节点;
    新状态信息 状态;
};

std::optional<std::vector<实际状态物理>> 读取实际状态物理组(
    稳定编码 执行节点, const 新_状态类& 状态服务) {
    std::vector<std::pair<std::int64_t, 实际状态物理>> 排序组;
    for (const auto& 持有 : 全局基础数据集.查询字段(
        执行节点, 执行实际状态项字段)) {
        const auto* 项节点 = std::get_if<稳定编码>(&持有.内容);
        if (!项节点 || !节点类型为(*项节点, 任务实际状态项节点类型)) {
            return std::nullopt;
        }
        const auto 顺序 = 读取唯一I64字段记录(*项节点, 实际状态顺序字段);
        const auto 状态节点 = 读取唯一节点字段(*项节点, 实际状态节点字段);
        const auto 状态 = 状态节点 ? 状态服务.获取状态(*状态节点) : std::nullopt;
        if (!顺序 || 顺序->值 < 0 || !状态节点 || !状态) return std::nullopt;
        排序组.push_back({顺序->值, {*项节点, *状态节点, *状态}});
    }
    std::sort(排序组.begin(), 排序组.end(), [](const auto& 左, const auto& 右) {
        return 左.first < 右.first;
    });
    std::vector<实际状态物理> 结果;
    for (std::size_t i = 0; i < 排序组.size(); ++i) {
        if (排序组[i].first != static_cast<std::int64_t>(i)) return std::nullopt;
        结果.push_back(std::move(排序组[i].second));
    }
    return 结果;
}

std::optional<新任务执行信息> 读取执行信息(
    稳定编码 执行节点, const 新_状态类& 状态服务,
    const 新_特征类& 特征服务, const 概念_特征类& 特征概念服务) {
    if (!结构仍存在() || !节点类型为(执行节点, 任务执行实例节点类型)) {
        return std::nullopt;
    }
    const auto 任务 = 读取唯一节点字段(执行节点, 执行所属任务字段);
    const auto 方法 = 读取唯一节点字段(执行节点, 执行方法字段);
    const auto 状态 = 读取唯一I64字段记录(执行节点, 执行状态字段);
    const auto 开始语义 = 读取唯一I64字段记录(执行节点, 执行开始时间语义字段);
    const auto 开始时间 = 读取唯一I64字段记录(执行节点, 执行开始时间字段);
    const auto 阶段组 = 读取预计阶段物理组(执行节点);
    const auto 结果获取方案 = 读取结果获取方案(执行节点);
    if (!任务 || !方法 || !状态 || !是执行状态值(状态->值)
        || !开始语义 || !开始时间 || !阶段组
        || !结果获取方案) return std::nullopt;
    新任务执行信息 结果;
    结果.节点 = 执行节点;
    结果.任务节点 = *任务;
    结果.方法节点 = *方法;
    结果.本能采用=任务执行组织存储::读取执行采用已加锁(执行节点,*任务,*方法);
    结果.状态 = static_cast<新任务执行状态>(状态->值);
    结果.开始时间 = {
        static_cast<新状态时间语义>(开始语义->值), 开始时间->值};
    if (!强时间有效(结果.开始时间)) return std::nullopt;
    for (const auto& 阶段 : *阶段组) {
        新任务预计变化阶段 公开阶段;
        公开阶段.顺序 = 阶段.顺序;
        for (const auto& 项 : 阶段.变化组) 公开阶段.变化组.push_back(项.变化);
        结果.预计变化阶段组.push_back(std::move(公开阶段));
    }
    结果.结果获取方案 = *结果获取方案;
    const auto 核验精确 = [&](稳定编码 类型, 稳定编码 实际特征,
        const std::optional<特征概念I64闭区间>& 域) {
        if (!域) return true;
        const auto 输入 = 取得任务值域输入(特征概念服务, 类型, {}, 域);
        const auto 特征 = 特征服务.获取特征(实际特征);
        if (!输入 || !特征 || 输入->单位 != 特征->实例单位
            || 取得特征概念类型根(特征概念服务, 类型)
                != 取得特征概念类型根(特征概念服务, 特征->特征概念节点)) return false;
        const auto 自比 = 特征服务.比较特征值域(*输入, *输入);
        return 自比.状态 == 新特征比较状态::已完成
            && 自比.关系编码 == static_cast<std::int64_t>(特征概念值域关系::相等);
    };
    for (const auto& 阶段 : 结果.预计变化阶段组) for (const auto& 变化 : 阶段.变化组) {
        if (!变化.预期精确数值域) continue;
        const auto 类型 = 取得特征概念类型根(特征概念服务, 变化.特征类型概念节点);
        const auto 目标 = std::find_if(结果获取方案->目标组.begin(), 结果获取方案->目标组.end(),
            [&](const auto& x) { return x.作用对象节点 == 变化.作用对象节点 && 类型 == x.特征类型根概念节点; });
        if (目标 == 结果获取方案->目标组.end()
            || !核验精确(变化.特征类型概念节点, 目标->实际特征节点, 变化.预期精确数值域)) return std::nullopt;
    }
    for (const auto& 目标 : 结果获取方案->禁止结果组)
        if (!核验精确(目标.特征类型根概念节点, 目标.实际特征节点, 目标.禁止精确数值域)) return std::nullopt;
    if (!读取服务结算来源字段(执行节点, 结果.服务结算来源)) return std::nullopt;
    const auto 动作字段组 = 全局基础数据集.查询字段(执行节点, 执行普通动作字段);
    if (动作字段组.size() > 1) return std::nullopt;
    if (!动作字段组.empty()) {
        结果.普通动作节点 = 读取唯一节点字段(执行节点, 执行普通动作字段);
        if (!结果.普通动作节点 || !节点仍存在(*结果.普通动作节点)) return std::nullopt;
    }
    结果.动作输出状态节点组 = 读取多节点字段(
        执行节点, 执行动作输出状态字段);
    std::sort(结果.动作输出状态节点组.begin(),
        结果.动作输出状态节点组.end());
    if (std::adjacent_find(结果.动作输出状态节点组.begin(),
            结果.动作输出状态节点组.end())
        != 结果.动作输出状态节点组.end()
        || std::any_of(结果.动作输出状态节点组.begin(),
            结果.动作输出状态节点组.end(), [&](稳定编码 状态节点) {
                return !状态服务.获取状态(状态节点).has_value();
            })) {
        return std::nullopt;
    }
    const auto 结束语义组 = 全局基础数据集.查询字段(
        执行节点, 执行结束时间语义字段);
    const auto 结束时间组 = 全局基础数据集.查询字段(
        执行节点, 执行结束时间字段);
    if (结束语义组.size() != 结束时间组.size() || 结束语义组.size() > 1) {
        return std::nullopt;
    }
    if (!结束语义组.empty()) {
        const auto 语义 = 读取唯一I64字段记录(执行节点, 执行结束时间语义字段);
        const auto 时间 = 读取唯一I64字段记录(执行节点, 执行结束时间字段);
        if (!语义 || !时间) return std::nullopt;
        结果.结束时间 = 新状态强时间{
            static_cast<新状态时间语义>(语义->值), 时间->值};
        if (!强时间有效(*结果.结束时间)
            || 结果.结束时间->纳秒 < 结果.开始时间.纳秒) return std::nullopt;
    }
    const auto 实际组 = 读取实际状态物理组(执行节点, 状态服务);
    if (!实际组) return std::nullopt;
    for (const auto& 项 : *实际组) 结果.实际状态节点组.push_back(项.状态节点);
    const auto 评估组 = 全局基础数据集.查询字段(执行节点, 执行评估字段);
    if (评估组.size() > 1) return std::nullopt;
    if (!评估组.empty()) {
        const auto* 评估 = std::get_if<稳定编码>(&评估组.front().内容);
        if (!评估 || !节点类型为(*评估, 任务影响评估节点类型)) {
            return std::nullopt;
        }
        结果.影响评估节点 = *评估;
    }
    if (结果.状态 == 新任务执行状态::执行中
        && (结果.结束时间 || !结果.实际状态节点组.empty()
            || 结果.影响评估节点)) return std::nullopt;
    if (结果.状态 != 新任务执行状态::执行中 && !结果.结束时间) {
        return std::nullopt;
    }
    if (结果.状态 == 新任务执行状态::已评估 && !结果.影响评估节点) {
        return std::nullopt;
    }
    return 结果;
}

std::optional<新任务影响评估信息> 读取评估信息(稳定编码 评估节点) {
    if (!结构仍存在() || !节点类型为(评估节点, 任务影响评估节点类型)) {
        return std::nullopt;
    }
    const auto 任务 = 读取唯一节点字段(评估节点, 评估所属任务字段);
    const auto 需求 = 读取唯一节点字段(评估节点, 评估需求字段);
    const auto 执行 = 读取唯一节点字段(评估节点, 评估执行实例字段);
    const auto 方法 = 读取唯一节点字段(评估节点, 评估方法字段);
    const auto 需求状态 = 读取唯一I64字段记录(
        评估节点, 评估需求结果状态字段);
    const auto 方向验证 = 读取唯一I64字段记录(评估节点, 评估方向验证字段);
    const auto 幅度验证 = 读取唯一I64字段记录(评估节点, 评估幅度验证字段);
    const auto 值域验证 = 读取唯一I64字段记录(评估节点, 评估值域验证字段);
    const auto 多渠道验证 = 读取唯一I64字段记录(
        评估节点, 评估多渠道验证字段);
    const auto 禁止结果验证 = 读取唯一I64字段记录(
        评估节点, 评估禁止结果验证字段);
    const auto 副作用验证 = 读取唯一I64字段记录(
        评估节点, 评估副作用验证字段);
    const auto 归因验证 = 读取唯一I64字段记录(评估节点, 评估归因验证字段);
    if (!任务 || !需求 || !执行 || !方法 || !需求状态
        || !方向验证 || !幅度验证 || !值域验证 || !多渠道验证
        || !禁止结果验证 || !副作用验证 || !归因验证
        || !是影响验证状态值(方向验证->值)
        || !是影响验证状态值(幅度验证->值)
        || !是影响验证状态值(值域验证->值)
        || !是影响验证状态值(多渠道验证->值)
        || !是影响验证状态值(禁止结果验证->值)
        || !是影响验证状态值(副作用验证->值)
        || !是影响验证状态值(归因验证->值)
        || 需求状态->值 < static_cast<std::int64_t>(新需求当前满足状态::已满足)
        || 需求状态->值 > static_cast<std::int64_t>(
            新需求当前满足状态::读取接口未装配)) {
        return std::nullopt;
    }
    新任务影响评估信息 结果;
    结果.节点 = 评估节点;
    结果.任务节点 = *任务;
    结果.需求节点 = *需求;
    结果.执行实例节点 = *执行;
    结果.方法节点 = *方法;
    结果.目标需求结果.状态 =
        static_cast<新需求当前满足状态>(需求状态->值);
    结果.方向验证 = static_cast<新任务影响验证状态>(方向验证->值);
    结果.幅度验证 = static_cast<新任务影响验证状态>(幅度验证->值);
    结果.值域验证 = static_cast<新任务影响验证状态>(值域验证->值);
    结果.多渠道验证 = static_cast<新任务影响验证状态>(多渠道验证->值);
    结果.禁止结果验证 = static_cast<新任务影响验证状态>(
        禁止结果验证->值);
    结果.副作用验证 = static_cast<新任务影响验证状态>(副作用验证->值);
    结果.归因验证 = static_cast<新任务影响验证状态>(归因验证->值);
    const auto 实际特征组 = 全局基础数据集.查询字段(
        评估节点, 评估需求实际特征字段);
    if (实际特征组.size() > 1) return std::nullopt;
    if (!实际特征组.empty()) {
        const auto* 实际特征 = std::get_if<稳定编码>(
            &实际特征组.front().内容);
        if (!实际特征 || !节点仍存在(*实际特征)) return std::nullopt;
        结果.目标需求结果.实际特征节点 = *实际特征;
    }
    const auto 差异组 = 全局基础数据集.查询字段(
        评估节点, 评估需求准确差异字段);
    if (差异组.size() > 1) return std::nullopt;
    if (!差异组.empty()) {
        const auto 差异 = 读取唯一I64字段记录(
            评估节点, 评估需求准确差异字段);
        if (!差异) return std::nullopt;
        结果.目标需求结果.准确差异 = 差异->值;
    }
    结果.符合预测状态组 = 读取多节点字段(
        评估节点, 评估符合预测状态字段);
    结果.预计但未发生项组 = 读取多节点字段(
        评估节点, 评估缺失变化项字段);
    结果.计划外状态组 = 读取多节点字段(
        评估节点, 评估计划外状态字段);
    return 结果;
}

std::optional<新任务完成结算信息> 读取完成结算信息(
    稳定编码 结算节点,
    const 新_状态类& 状态服务) {
    if (!结构仍存在() || !节点类型为(
            结算节点, 任务完成结算节点类型)) return std::nullopt;
    const auto 任务 = 读取唯一节点字段(结算节点, 结算所属任务字段);
    const auto 需求 = 读取唯一节点字段(结算节点, 结算需求字段);
    const auto 执行 = 读取唯一节点字段(结算节点, 结算执行实例字段);
    const auto 评估 = 读取唯一节点字段(结算节点, 结算影响评估字段);
    const auto 来源 = 读取唯一I64字段记录(结算节点, 结算完成来源字段);
    if (!任务 || !需求 || !执行 || !评估 || !来源
        || !是完成来源值(来源->值)) return std::nullopt;
    auto 结果状态组 = 读取多节点字段(结算节点, 结算结果状态字段);
    if (结果状态组.empty()) return std::nullopt;
    std::sort(结果状态组.begin(), 结果状态组.end(), [](稳定编码 左, 稳定编码 右) {
        return 左.值 < 右.值;
    });
    if (std::adjacent_find(结果状态组.begin(), 结果状态组.end())
            != 结果状态组.end()
        || std::any_of(结果状态组.begin(), 结果状态组.end(),
            [&](稳定编码 状态节点) {
                return !状态服务.获取状态(状态节点).has_value();
            })) {
        return std::nullopt;
    }
    新任务完成结算信息 结果;
    结果.节点 = 结算节点;
    结果.任务节点 = *任务;
    结果.需求节点 = *需求;
    结果.执行实例节点 = *执行;
    结果.影响评估节点 = *评估;
    结果.完成来源 = static_cast<新任务完成来源>(来源->值);
    结果.结果状态节点组 = std::move(结果状态组);
    return 结果;
}

struct 已建预计结构 final {
    std::vector<稳定编码> 阶段节点组;
    std::vector<稳定编码> 项节点组;
    std::vector<稳定编码> 字段组;
};

void 清理预计结构(已建预计结构& 已建) noexcept {
    回滚字段(已建.字段组);
    for (auto 位置 = 已建.项节点组.rbegin(); 位置 != 已建.项节点组.rend(); ++位置) {
        (void)全局基础数据集.删除节点(*位置);
    }
    for (auto 位置 = 已建.阶段节点组.rbegin();
        位置 != 已建.阶段节点组.rend(); ++位置) {
        (void)全局基础数据集.删除节点(*位置);
    }
}

bool 删除节点具名字段(
    稳定编码 节点,
    const std::vector<稳定编码>& 字段节点组) noexcept {
    for (const auto 字段节点 : 字段节点组) {
        const auto 字段组 = 全局基础数据集.查询字段(节点, 字段节点);
        for (const auto& 字段 : 字段组) {
            if (!全局基础数据集.删除字段(字段.编码)
                && 全局基础数据集.查询字段关系(字段.编码)) {
                return false;
            }
        }
        if (!全局基础数据集.查询字段(节点, 字段节点).empty()) {
            return false;
        }
    }
    return true;
}

bool 删除未进入动作执行结构(
    稳定编码 执行节点,
    const std::vector<预计阶段物理>& 阶段组,
    新_动态类* 动态服务) noexcept {
    const auto 动作字段组 = 全局基础数据集.查询字段(执行节点, 执行普通动作字段);
    if (动作字段组.size() > 1) return false;
    if (!动作字段组.empty()) {
        const auto 动作 = 读取唯一节点字段(执行节点, 执行普通动作字段);
        if (!动作 || !动态服务 || !动态服务->获取动态(*动作)) return false;
        if (!全局基础数据集.删除字段(动作字段组.front().编码)) return false;
        if (动态服务->减少引用(*动作) != 新动态操作状态::已完成) {
            // 减引用失败由dynamic恢复计数；保留X对应的强引用，不假报清理完成。
            (void)全局基础数据集.添加字段节点(执行节点, 执行普通动作字段, *动作);
            return false;
        }
    }
    std::vector<稳定编码> 渠道方案节点组;
    for (const auto& 持有 : 全局基础数据集.查询字段(
        执行节点, 执行结果获取渠道方案字段)) {
        const auto* 渠道节点 = std::get_if<稳定编码>(&持有.内容);
        if (!渠道节点 || !节点类型为(
                *渠道节点, 任务结果获取渠道方案节点类型)) {
            return false;
        }
        渠道方案节点组.push_back(*渠道节点);
    }
    std::vector<稳定编码> 结果目标节点组;
    for (const auto& 持有 : 全局基础数据集.查询字段(
        执行节点, 执行结果获取目标字段)) {
        const auto* 目标节点 = std::get_if<稳定编码>(&持有.内容);
        if (!目标节点 || !节点类型为(
                *目标节点, 任务结果获取目标节点类型)) {
            return false;
        }
        结果目标节点组.push_back(*目标节点);
    }
    std::vector<稳定编码> 禁止目标节点组;
    for (const auto& 持有 : 全局基础数据集.查询字段(
        执行节点, 执行禁止结果目标字段)) {
        const auto* 目标节点 = std::get_if<稳定编码>(&持有.内容);
        if (!目标节点 || !节点类型为(
                *目标节点, 任务禁止结果目标节点类型)) {
            return false;
        }
        禁止目标节点组.push_back(*目标节点);
    }
    const std::vector<稳定编码> 执行字段组{
        结算来源任务字段, 结算来源虚拟存在字段, 结算来源状态特征字段,
        结算冻结前值字段, 结算目标值域字段,
        节点类型字段, 执行所属任务字段, 执行方法字段,
        执行状态字段, 执行开始时间语义字段, 执行开始时间字段,
        执行结束时间语义字段, 执行结束时间字段,
        执行预计阶段字段, 执行结果获取多渠道关系字段,
        执行结果获取渠道方案字段, 执行结果获取目标字段,
        执行禁止结果关系规则字段, 执行禁止结果目标字段,
        执行动作输出状态字段, 执行实际状态项字段, 执行评估字段};
    try{任务执行组织存储::清理执行采用已加锁(执行节点);}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"清理未入动作X采用",L"独占采用结构核验失败");}
    if (!删除节点具名字段(执行节点, 执行字段组)) return false;

    const std::vector<稳定编码> 阶段字段组{
        节点类型字段, 阶段顺序字段, 阶段变化项字段};
    const std::vector<稳定编码> 项字段组{
        节点类型字段, 变化作用对象字段, 变化特征类型字段,
        变化关系字段, 变化方向符号字段, 变化值域字段, 变化精确数值域字段,
        变化必须出现字段};
    for (const auto& 阶段 : 阶段组) {
        if (!删除节点具名字段(阶段.节点, 阶段字段组)) return false;
        for (const auto& 项 : 阶段.变化组) {
            if (!删除节点具名字段(项.节点, 项字段组)
                || (!全局基础数据集.删除节点(项.节点)
                    && 全局基础数据集.查询节点(项.节点))) {
                return false;
            }
        }
        if (!全局基础数据集.删除节点(阶段.节点)
            && 全局基础数据集.查询节点(阶段.节点)) {
            return false;
        }
    }
    const std::vector<稳定编码> 结果目标字段组{
        节点类型字段, 结果目标顺序字段, 结果目标作用对象字段,
        结果目标实际特征字段, 结果目标特征类型根字段,
        结果目标动作前准确值字段, 结果目标必须出现字段};
    for (const auto 目标节点 : 结果目标节点组) {
        if (!删除节点具名字段(目标节点, 结果目标字段组)
            || (!全局基础数据集.删除节点(目标节点)
                && 全局基础数据集.查询节点(目标节点))) {
            return false;
        }
    }
    const std::vector<稳定编码> 渠道字段组{
        节点类型字段, 渠道方案顺序字段, 渠道方案类型字段,
        渠道方案证据质量字段, 渠道方案最大等待纳秒字段,
        渠道方案等待条件概念字段};
    for (const auto 渠道节点 : 渠道方案节点组) {
        if (!删除节点具名字段(渠道节点, 渠道字段组)
            || (!全局基础数据集.删除节点(渠道节点)
                && 全局基础数据集.查询节点(渠道节点))) {
            return false;
        }
    }
    const std::vector<稳定编码> 禁止字段组{
        节点类型字段, 禁止目标顺序字段, 禁止目标作用对象字段,
        禁止目标实际特征字段, 禁止目标特征类型根字段,
        禁止目标动作前准确值字段, 禁止目标方向字段,
        禁止目标值域字段, 禁止目标精确数值域字段, 禁止目标状态动态字段};
    for (const auto 目标节点 : 禁止目标节点组) {
        if (!删除节点具名字段(目标节点, 禁止字段组)
            || (!全局基础数据集.删除节点(目标节点)
                && 全局基础数据集.查询节点(目标节点))) {
            return false;
        }
    }
    return 全局基础数据集.删除节点(执行节点)
        || !全局基础数据集.查询节点(执行节点);
}

bool 建立预计结构(
    稳定编码 执行节点,
    std::vector<新任务预计变化阶段> 阶段组,
    已建预计结构& 已建) {
    std::sort(阶段组.begin(), 阶段组.end(), [](const auto& 左, const auto& 右) {
        return 左.顺序 < 右.顺序;
    });
    for (const auto& 阶段 : 阶段组) {
        const auto 阶段节点 = 全局基础数据集.新建节点();
        if (!有效(阶段节点)) return false;
        已建.阶段节点组.push_back(阶段节点);
        if (!写入节点字段(阶段节点, 节点类型字段,
                任务预计变化阶段节点类型, 已建.字段组)
            || !写入I64字段(阶段节点, 阶段顺序字段,
                阶段.顺序, 已建.字段组)) return false;
        for (const auto& 变化 : 阶段.变化组) {
            const auto 项节点 = 全局基础数据集.新建节点();
            if (!有效(项节点)) return false;
            已建.项节点组.push_back(项节点);
            if (!写入节点字段(项节点, 节点类型字段,
                    任务预计变化项节点类型, 已建.字段组)
                || !写入节点字段(项节点, 变化作用对象字段,
                    变化.作用对象节点, 已建.字段组)
                || !写入节点字段(项节点, 变化特征类型字段,
                    变化.特征类型概念节点, 已建.字段组)
                || !写入I64字段(项节点, 变化必须出现字段,
                    变化.必须出现 ? 1 : 0, 已建.字段组)) return false;
            if (变化.预期变化关系节点
                && !写入节点字段(项节点, 变化关系字段,
                    *变化.预期变化关系节点, 已建.字段组)) return false;
            if (变化.预期方向
                && !写入I64字段(项节点, 变化方向符号字段,
                    static_cast<std::int64_t>(*变化.预期方向),
                    已建.字段组)) return false;
            if (变化.预期值域概念节点
                && !写入节点字段(项节点, 变化值域字段,
                    *变化.预期值域概念节点, 已建.字段组)) return false;
            if (变化.预期精确数值域
                && !写入任务精确域(项节点, 变化精确数值域字段,
                    *变化.预期精确数值域, 已建.字段组)) return false;
            if (!写入节点字段(阶段节点, 阶段变化项字段,
                项节点, 已建.字段组)) return false;
        }
        if (!写入节点字段(执行节点, 执行预计阶段字段,
            阶段节点, 已建.字段组)) return false;
    }
    return true;
}

struct 已建结果获取结构 final {
    std::vector<稳定编码> 渠道节点组;
    std::vector<稳定编码> 目标节点组;
    std::vector<稳定编码> 禁止目标节点组;
    std::vector<稳定编码> 字段组;
};

void 清理结果获取结构(已建结果获取结构& 已建) noexcept {
    回滚字段(已建.字段组);
    for (auto 位置 = 已建.禁止目标节点组.rbegin();
        位置 != 已建.禁止目标节点组.rend(); ++位置) {
        (void)全局基础数据集.删除节点(*位置);
    }
    for (auto 位置 = 已建.目标节点组.rbegin();
        位置 != 已建.目标节点组.rend(); ++位置) {
        (void)全局基础数据集.删除节点(*位置);
    }
    for (auto 位置 = 已建.渠道节点组.rbegin();
        位置 != 已建.渠道节点组.rend(); ++位置) {
        (void)全局基础数据集.删除节点(*位置);
    }
}

bool 建立结果获取结构(
    稳定编码 执行节点,
    const 新任务结果获取方案& 方案,
    已建结果获取结构& 已建) {
    if (!方案.完整()
        || !写入I64字段(执行节点, 执行结果获取多渠道关系字段,
            static_cast<std::int64_t>(方案.多渠道关系), 已建.字段组)
        || !写入I64字段(执行节点, 执行禁止结果关系规则字段,
            static_cast<std::int64_t>(方案.禁止结果关系规则),
            已建.字段组)) {
        return false;
    }
    for (const auto& 渠道 : 方案.渠道方案组) {
        const auto 渠道节点 = 全局基础数据集.新建节点();
        if (!有效(渠道节点)) return false;
        已建.渠道节点组.push_back(渠道节点);
        if (!写入节点字段(渠道节点, 节点类型字段,
                任务结果获取渠道方案节点类型, 已建.字段组)
            || !写入I64字段(渠道节点, 渠道方案顺序字段,
                渠道.顺序, 已建.字段组)
            || !写入I64字段(渠道节点, 渠道方案类型字段,
                static_cast<std::int64_t>(渠道.渠道), 已建.字段组)
            || !写入I64字段(渠道节点, 渠道方案证据质量字段,
                static_cast<std::int64_t>(渠道.证据质量), 已建.字段组)) {
            return false;
        }
        if (渠道.最大等待纳秒
            && !写入I64字段(渠道节点, 渠道方案最大等待纳秒字段,
                *渠道.最大等待纳秒, 已建.字段组)) {
            return false;
        }
        if (渠道.等待条件概念节点
            && !写入节点字段(渠道节点, 渠道方案等待条件概念字段,
                *渠道.等待条件概念节点, 已建.字段组)) {
            return false;
        }
        if (!写入节点字段(执行节点, 执行结果获取渠道方案字段,
                渠道节点, 已建.字段组)) {
            return false;
        }
    }
    for (std::size_t i = 0; i < 方案.目标组.size(); ++i) {
        const auto& 目标 = 方案.目标组[i];
        const auto 目标节点 = 全局基础数据集.新建节点();
        if (!有效(目标节点)) return false;
        已建.目标节点组.push_back(目标节点);
        if (!写入节点字段(目标节点, 节点类型字段,
                任务结果获取目标节点类型, 已建.字段组)
            || !写入I64字段(目标节点, 结果目标顺序字段,
                static_cast<std::int64_t>(i), 已建.字段组)
            || !写入节点字段(目标节点, 结果目标作用对象字段,
                目标.作用对象节点, 已建.字段组)
            || !写入节点字段(目标节点, 结果目标实际特征字段,
                目标.实际特征节点, 已建.字段组)
            || !写入节点字段(目标节点, 结果目标特征类型根字段,
                目标.特征类型根概念节点, 已建.字段组)
            || !写入准确值字段(目标节点,
                结果目标动作前准确值字段, 目标.动作前准确值,
                已建.字段组)
            || !写入I64字段(目标节点, 结果目标必须出现字段,
                目标.必须出现 ? 1 : 0, 已建.字段组)
            || !写入节点字段(执行节点, 执行结果获取目标字段,
                目标节点, 已建.字段组)) {
            return false;
        }
    }
    for (std::size_t i = 0; i < 方案.禁止结果组.size(); ++i) {
        const auto& 目标 = 方案.禁止结果组[i];
        const auto 目标节点 = 全局基础数据集.新建节点();
        if (!有效(目标节点)) return false;
        已建.禁止目标节点组.push_back(目标节点);
        if (!写入节点字段(目标节点, 节点类型字段,
                任务禁止结果目标节点类型, 已建.字段组)
            || !写入I64字段(目标节点, 禁止目标顺序字段,
                static_cast<std::int64_t>(i), 已建.字段组)
            || !写入节点字段(目标节点, 禁止目标作用对象字段,
                目标.作用对象节点, 已建.字段组)
            || !写入节点字段(目标节点, 禁止目标实际特征字段,
                目标.实际特征节点, 已建.字段组)
            || !写入节点字段(目标节点, 禁止目标特征类型根字段,
                目标.特征类型根概念节点, 已建.字段组)
            || !写入准确值字段(目标节点,
                禁止目标动作前准确值字段, 目标.动作前准确值,
                已建.字段组)) {
            return false;
        }
        if (目标.禁止方向
            && !写入I64字段(目标节点, 禁止目标方向字段,
                static_cast<std::int64_t>(*目标.禁止方向), 已建.字段组)) {
            return false;
        }
        if (目标.禁止值域概念节点
            && !写入节点字段(目标节点, 禁止目标值域字段,
                *目标.禁止值域概念节点, 已建.字段组)) {
            return false;
        }
        if (目标.禁止状态或动态概念节点
            && !写入节点字段(目标节点, 禁止目标状态动态字段,
                *目标.禁止状态或动态概念节点, 已建.字段组)) {
            return false;
        }
        if (目标.禁止精确数值域
            && !写入任务精确域(目标节点, 禁止目标精确数值域字段,
                *目标.禁止精确数值域, 已建.字段组)) return false;
        if (!写入节点字段(执行节点, 执行禁止结果目标字段,
                目标节点, 已建.字段组)) {
            return false;
        }
    }
    return true;
}

// 只核对集合身份，不把某个T的内容错误扩大成全集丢失。
// 原全集查询及调度扫描共用同一登记/类型双向算法。
std::optional<std::vector<稳定编码>> 读取任务登记全集() {
    std::vector<稳定编码> 登记任务;
    for (const auto& 字段 : 全局基础数据集.查询字段(
            任务集合根节点, 任务成员字段)) {
        const auto* 任务 = std::get_if<稳定编码>(&字段.内容);
        if (!任务 || !有效(*任务)) return std::nullopt;
        登记任务.push_back(*任务);
    }
    std::sort(登记任务.begin(), 登记任务.end());
    if (std::adjacent_find(登记任务.begin(), 登记任务.end()) != 登记任务.end())
        return std::nullopt;
    std::vector<稳定编码> 类型任务;
    for (const auto& 字段 : 全局基础数据集.查询目标字段(
            任务节点类型, 节点类型字段)) {
        if (!节点类型为(字段.所属节点, 任务节点类型)) return std::nullopt;
        类型任务.push_back(字段.所属节点);
    }
    std::sort(类型任务.begin(), 类型任务.end());
    if (std::adjacent_find(类型任务.begin(), 类型任务.end()) != 类型任务.end()
        || 类型任务 != 登记任务) return std::nullopt;
    return 登记任务;
}

} // namespace

bool 新任务调度定位::完整() const noexcept {
    const auto 可选有效 = [](const std::optional<稳定编码>& 节点) noexcept {
        return !节点 || 有效(*节点);
    };
    return 有效(任务节点) && 有效(需求节点)
        && 是任务状态值(static_cast<std::int64_t>(任务状态))
        && 可选有效(候选材料节点) && 可选有效(方法实例节点)&&可选有效(方法执行组织节点)
        && 可选有效(最近执行实例节点) && 可选有效(影响评估节点)
        && (!影响评估节点 || 最近执行实例节点);
}

新任务线性条件输入结果 新_任务类::按线性关系计算任务条件输入(
    const 新任务线性条件输入请求& 请求) noexcept {
    using 状态 = 新任务线性条件输入状态;
    try {
        if (请求.条件域组.empty() || 请求.结果约束组.empty() || 请求.关系组.empty())
            return {状态::输入不合法, {}};

        bool 含无穷 = false;
        for (const auto& 域 : 请求.条件域组) {
            if (域.下界 > 域.上界) return {状态::输入不合法, {}};
            含无穷 = 含无穷 || 任务线性区间含无穷(域);
        }
        // 仅是调用期位置索引，不保存节点或第二份方法知识。
        std::map<std::size_t, const 新任务线性结果约束*> 结果索引;
        for (const auto& 项 : 请求.结果约束组) {
            if (项.方法结果域.下界 > 项.方法结果域.上界
                || 项.任务目标域.下界 > 项.任务目标域.上界
                || !结果索引.emplace(项.结果序号, &项).second)
                return {状态::输入不合法, {}};
            含无穷 = 含无穷 || 任务线性区间含无穷(项.方法结果域)
                || 任务线性区间含无穷(项.任务目标域);
        }
        std::vector<bool> 条件覆盖(请求.条件域组.size(), false);
        std::set<std::size_t> 结果覆盖;
        std::set<std::pair<std::size_t, std::size_t>> 已见关系;
        for (const auto& 关系 : 请求.关系组) {
            const auto 定位 = 结果索引.find(关系.结果序号);
            if (关系.条件序号 >= 请求.条件域组.size() || 定位 == 结果索引.end()
                || !已见关系.emplace(关系.条件序号, 关系.结果序号).second)
                return {状态::输入不合法, {}};
            const auto& 条件域 = 请求.条件域组[关系.条件序号];
            const auto& 结果域 = 定位->second->方法结果域;
            switch (关系.方向) {
            case 新任务条件结果线性方向::不变:
                if (结果域.下界 != 结果域.上界) return {状态::输入不合法, {}};
                break;
            case 新任务条件结果线性方向::同向:
            case 新任务条件结果线性方向::反向:
                if (条件域.下界 == 条件域.上界 || 结果域.下界 == 结果域.上界)
                    return {状态::输入不合法, {}};
                break;
            default:
                return {状态::输入不合法, {}};
            }
            条件覆盖[关系.条件序号] = true;
            结果覆盖.insert(关系.结果序号);
        }
        // 先完成全体结构校验，再按无穷、缺关系、空逆像的优先级返回。
        if (含无穷) return {状态::不支持无穷域, {}};
        if (std::find(条件覆盖.begin(), 条件覆盖.end(), false) != 条件覆盖.end()
            || 结果覆盖.size() != 结果索引.size())
            return {状态::关系材料不足, {}};

        auto 所需域组 = 请求.条件域组;
        for (const auto& 关系 : 请求.关系组) {
            const auto& 约束 = *结果索引.find(关系.结果序号)->second;
            const auto 下 = (std::max)(约束.方法结果域.下界, 约束.任务目标域.下界);
            const auto 上 = (std::min)(约束.方法结果域.上界, 约束.任务目标域.上界);
            if (下 > 上) return {状态::目标无交集, {}};
            if (关系.方向 == 新任务条件结果线性方向::不变) continue;

            const auto& 原条件域 = 请求.条件域组[关系.条件序号];
            const auto 条件起点 = 任务线性有序坐标(原条件域.下界);
            const auto 条件跨度 = 任务线性有序坐标(原条件域.上界) - 条件起点;
            const auto 结果起点 = 任务线性有序坐标(约束.方法结果域.下界);
            const auto 结果终点 = 任务线性有序坐标(约束.方法结果域.上界);
            const auto 结果跨度 = 结果终点 - 结果起点;
            const bool 同向 = 关系.方向 == 新任务条件结果线性方向::同向;
            const auto 下分子 = 同向 ? 任务线性有序坐标(下) - 结果起点
                                     : 结果终点 - 任务线性有序坐标(上);
            const auto 上分子 = 同向 ? 任务线性有序坐标(上) - 结果起点
                                     : 结果终点 - 任务线性有序坐标(下);
            const auto 下商余 = 任务线性乘除商余(下分子, 条件跨度, 结果跨度);
            const auto 上商余 = 任务线性乘除商余(上分子, 条件跨度, 结果跨度);
            const auto 下偏移 = 下商余.first + (下商余.second != 0 ? 1U : 0U);
            const auto 上偏移 = 上商余.first;
            if (下偏移 > 上偏移) return {状态::目标无交集, {}};
            auto& 所需域 = 所需域组[关系.条件序号];
            所需域.下界 = (std::max)(所需域.下界, 任务线性坐标值(条件起点 + 下偏移));
            所需域.上界 = (std::min)(所需域.上界, 任务线性坐标值(条件起点 + 上偏移));
            if (所需域.下界 > 所需域.上界) return {状态::目标无交集, {}};
        }

        新任务线性条件输入结果 结果;
        结果.条件输入组.reserve(所需域组.size());
        for (std::size_t 序号 = 0; 序号 < 所需域组.size(); ++序号) {
            const auto& 域 = 所需域组[序号];
            结果.条件输入组.push_back({序号, 域,
                域.下界 == 域.上界 ? std::optional<std::int64_t>{域.下界} : std::nullopt});
        }
        结果.状态 = 状态::已算出;
        return 结果;
    } catch (...) {
        // 本函数只有调用期容器分配，无外部调用；失败不交付部分条件。
        return {状态::资源失败, {}};
    }
}

bool 新任务结果获取方案::完整() const noexcept {
    if (!是多渠道关系值(static_cast<std::int64_t>(多渠道关系))
        || 渠道方案组.empty() || 目标组.empty()
        || !是方法项关系规则值(
            static_cast<std::int64_t>(禁止结果关系规则))) {
        return false;
    }
    if ((多渠道关系 == 新任务多渠道关系::共同满足
            || 多渠道关系 == 新任务多渠道关系::优先备用)
        && 渠道方案组.size() < 2) {
        return false;
    }
    std::set<std::int64_t> 已见渠道;
    for (std::size_t i = 0; i < 渠道方案组.size(); ++i) {
        const auto& 渠道 = 渠道方案组[i];
        if (渠道.顺序 != static_cast<std::int64_t>(i)
            || !是结果获取渠道值(static_cast<std::int64_t>(渠道.渠道))
            || !是必要证据质量值(
                static_cast<std::int64_t>(渠道.证据质量))
            || (渠道.最大等待纳秒 && *渠道.最大等待纳秒 < 0)
            || (渠道.等待条件概念节点
                && !有效(*渠道.等待条件概念节点))
            || !已见渠道.insert(
                static_cast<std::int64_t>(渠道.渠道)).second) {
            return false;
        }
    }
    std::set<std::pair<std::uint64_t, std::uint64_t>> 已见目标;
    for (const auto& 目标 : 目标组) {
        if (!有效(目标.作用对象节点) || !有效(目标.实际特征节点)
            || !有效(目标.特征类型根概念节点)
            || (std::holds_alternative<稳定编码>(目标.动作前准确值)
                && !有效(std::get<稳定编码>(目标.动作前准确值)))
            || !已见目标.insert({目标.作用对象节点.值,
                目标.实际特征节点.值}).second) return false;
    }
    if (禁止结果组.empty()) return true;
    if ((禁止结果关系规则 == 新方法项关系规则::单项
            && 禁止结果组.size() != 1)
        || (禁止结果关系规则 != 新方法项关系规则::单项
            && 禁止结果组.size() < 2)
        || 禁止结果关系规则
            == 新方法项关系规则::主要结果及允许副作用) {
        return false;
    }
    std::set<std::pair<std::uint64_t, std::uint64_t>> 已见禁止目标;
    for (const auto& 目标 : 禁止结果组) {
        if (!有效(目标.作用对象节点) || !有效(目标.实际特征节点)
            || !有效(目标.特征类型根概念节点)
            || (std::holds_alternative<稳定编码>(目标.动作前准确值)
                && !有效(std::get<稳定编码>(目标.动作前准确值)))
            || (!目标.禁止方向 && !目标.禁止值域概念节点 && !目标.禁止精确数值域
                && !目标.禁止状态或动态概念节点)
            || (目标.禁止方向
                && !是预期变化方向值(
                    static_cast<std::int64_t>(*目标.禁止方向)))
            || (目标.禁止值域概念节点
                && !有效(*目标.禁止值域概念节点))
            || (目标.禁止精确数值域 && (目标.禁止值域概念节点
                || !精确域有效(*目标.禁止精确数值域)))
            || (目标.禁止状态或动态概念节点
                && !有效(*目标.禁止状态或动态概念节点))
            || !已见禁止目标.insert({目标.作用对象节点.值,
                目标.实际特征节点.值}).second) {
            return false;
        }
    }
    return true;
}

bool 新任务影响评估信息::允许形成完成决议() const noexcept {
    const auto 已通过或不适用 = [](新任务影响验证状态 状态) noexcept {
        return 状态 == 新任务影响验证状态::已验证通过
            || 状态 == 新任务影响验证状态::不适用;
    };
    // 预测准确性留给学习消费，不能用预测方向、幅度或值域代替需求满足。
    // 证据、禁止结果和归因门仍保留，不在这里放宽结算依据。
    return 目标需求结果.状态 == 新需求当前满足状态::已满足
        && 预计但未发生项组.empty()
        && 计划外状态组.empty()
        && 已通过或不适用(多渠道验证)
        && 已通过或不适用(禁止结果验证)
        && 副作用验证 == 新任务影响验证状态::已验证通过
        && 归因验证 == 新任务影响验证状态::已验证通过;
}

bool 新任务治理本能执行请求::完整() const noexcept {
    return 有效(任务节点) && 有效(方法节点) && 有效(配对节点)
        && 有效(结果节点) && 有效(自我存在节点)
        && 有效(安全需求节点) && 有效(服务需求节点)
        && !预计变化阶段组.empty() && 结果获取方案.完整()
        && 本能采用&&本能采用->来源方法节点==方法节点&&本能采用->来源配对节点==配对节点
        && 本能采用->来源结果节点==结果节点&&本能采用->参数绑定组==参数绑定组;
}

bool 新任务治理结果获取请求::完整() const noexcept {
    if (!有效(任务节点) || !有效(执行实例节点)
        || 状态节点组.empty()) return false;
    std::set<std::uint64_t> 已见;
    for (const auto 状态 : 状态节点组) {
        if (!有效(状态) || !已见.insert(状态.值).second) return false;
    }
    return true;
}

bool 新_任务类::装配方法问题反馈(
    执行场景采集服务& 采集, 新方法问题归档接口& 归档) noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        if (执行场景采集_ || 方法问题归档_) {
            return 执行场景采集_ == &采集 && 方法问题归档_ == &归档;
        }
        if (已开始过执行_ || !方法问题暂存组_.empty()) return false;
        执行场景采集_ = &采集;
        方法问题归档_ = &归档;
        return true;
    } catch (...) { return false; }
}

void 新_任务类::准备执行问题材料(
    const 新任务治理本能执行请求& 请求, 稳定编码 执行实例节点) noexcept {
    try {
        执行场景采集服务* 采集 = nullptr;
        auto 暂存 = std::make_shared<方法问题暂存>();
        auto 材料 = std::make_shared<新方法问题反馈材料>();
        {
            std::lock_guard 锁(新任务互斥);
            if (!执行场景采集_ || !方法问题归档_) return;
            const auto 执行 = 获取执行实例(执行实例节点);
            if (!执行 || !执行->本能采用 || 执行->任务节点 != 请求.任务节点
                || 执行->方法节点 != 请求.方法节点||执行->本能采用!=请求.本能采用)throw std::logic_error("反馈与本次X采用不一致");
            材料->任务节点 = 请求.任务节点;
            材料->执行实例节点 = 执行实例节点;
            材料->来源抽象方法节点 = 执行->本能采用->来源方法节点;
            材料->采用来源=新方法问题采用来源::任务执行组织;
            材料->任务采用=执行->本能采用;
            材料->结果场景.阶段 = 新场景快照阶段::结果取得时;
            采集 = 执行场景采集_;
        }
        暂存->任务节点 = 请求.任务节点;
        暂存->执行实例节点 = 执行实例节点;
        暂存->回执.状态 = 新任务问题归档交付状态::尚未评估;
        auto& 采集请求 = 暂存->采集请求;
        const auto 加概念 = [&](const std::optional<稳定编码>& 概念) {
            if (概念) 采集请求.额外特征概念组.push_back(*概念);
        };
        for (const auto& 绑定 : 请求.参数绑定组) {
            if (绑定.世界场景节点) 采集请求.入口场景组.push_back(*绑定.世界场景节点);
            if (绑定.实际存在节点) 采集请求.额外存在组.push_back(*绑定.实际存在节点);
            for (const auto& 特征 : 绑定.实际特征组) 采集请求.额外特征组.push_back(特征.特征节点);
            if (绑定.任务虚拟存在引用) 采集请求.额外特征组.push_back(绑定.任务虚拟存在引用->状态特征节点);
        }
        for (const auto& 目标 : 请求.结果获取方案.目标组) {
            const auto 场景 = 场景服务_.是场景节点(目标.作用对象节点)
                ? std::optional<稳定编码>{目标.作用对象节点}
                : 场景服务_.查询所属场景(目标.作用对象节点);
            if (场景) 采集请求.入口场景组.push_back(*场景);
            采集请求.额外特征组.push_back(目标.实际特征节点);
            采集请求.额外特征概念组.push_back(目标.特征类型根概念节点);
        }
        for (const auto& 项 : 材料->任务采用->需要条件.条件项组) {
            加概念(项.特征类型概念节点); 加概念(项.特征值域概念节点);
        }
        for (const auto& 项 : 材料->任务采用->结果.结果项组) {
            加概念(项.特征类型概念节点); 加概念(项.特征值域概念节点);
        }
        for (const auto& 项 : 材料->任务采用->结果.确认要求.禁止结果项组) {
            加概念(项.特征类型概念节点); 加概念(项.特征值域概念节点);
        }
        auto 首场景 = 采集->采集(采集请求);
        if (首场景.状态 == 新执行场景采集状态::资源失败) {
            暂存->回执.状态 = 新任务问题归档交付状态::资源失败;
        } else {
            材料->动作前场景 = std::move(首场景.快照);
            暂存->材料 = std::move(材料);
        }
        std::lock_guard 锁(新任务互斥);
        方法问题暂存组_.try_emplace(执行实例节点.值, std::move(暂存));
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"准备执行问题材料", L"执行采用或问题材料读取工程故障");
    }
}

void 新_任务类::保存执行问题运行对(稳定编码 任务节点, 稳定编码 执行实例节点,
    const 新本能函数本次条件结果对& 运行) noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        const auto 位置 = 方法问题暂存组_.find(执行实例节点.值);
        if (位置 == 方法问题暂存组_.end() || 位置->second->任务节点 != 任务节点) return;
        auto& 暂存 = *位置->second;
        // W3-B一旦开始冻结结果，运行对就不可再被补写或替换。
        if (暂存.正在处理 || 暂存.结果已冻结) return;
        if (运行.结果.动作阶段 == 新本能函数动作阶段::未进入动作) {
            方法问题暂存组_.erase(位置);
            return;
        }
        if (!暂存.材料) return;
        if (暂存.材料->本次运行对) {
            if (*暂存.材料->本次运行对 != 运行) 暂存.回执.状态 = 新任务问题归档交付状态::身份冲突;
            return;
        }
        暂存.材料->本次运行对 = 运行;
    } catch (...) {
        // 函数已经进入动作，记录失败不能撤销执行或重做动作。
        try {
            std::lock_guard 锁(新任务互斥);
            const auto 位置 = 方法问题暂存组_.find(执行实例节点.值);
            if (位置 != 方法问题暂存组_.end()) {
                位置->second->材料.reset();
                位置->second->回执.状态 = 新任务问题归档交付状态::资源失败;
            }
        } catch (...) {}
    }
}

void 新_任务类::完成执行问题采集(稳定编码 任务节点, 稳定编码 执行实例节点,
    const 新任务影响评估信息& 评估) noexcept {
    std::shared_ptr<方法问题暂存> 暂存;
    try {
        执行场景采集服务* 采集 = nullptr;
        新任务执行信息 执行;
        {
            std::lock_guard 锁(新任务互斥);
            const auto 位置 = 方法问题暂存组_.find(执行实例节点.值);
            if (位置 == 方法问题暂存组_.end()) return;
            暂存 = 位置->second;
            if (暂存->任务节点 != 任务节点 || 暂存->结果已冻结 || 暂存->正在处理) return;
            if (!暂存->材料 || 暂存->回执.状态 != 新任务问题归档交付状态::尚未评估) return;
            const auto 当前 = 获取执行实例(执行实例节点);
            if (!当前 || 当前->任务节点 != 任务节点 || 当前->影响评估节点 != 评估.节点
                || 评估.任务节点 != 任务节点 || 评估.执行实例节点 != 执行实例节点
                || 当前->方法节点 != 暂存->材料->来源抽象方法节点
                ||当前->本能采用!=暂存->材料->任务采用) {
                暂存->回执.状态 = 新任务问题归档交付状态::身份冲突;
                暂存->材料.reset();
                return;
            }
            执行 = *当前;
            采集 = 执行场景采集_;
            暂存->正在处理 = true;
        }
        auto& 材料 = *暂存->材料;
        材料.影响评估节点 = 评估.节点;
        材料.原方向验证值 = static_cast<std::int64_t>(评估.方向验证);
        材料.原值域验证值 = static_cast<std::int64_t>(评估.值域验证);
        if (评估.方向验证 == 新任务影响验证状态::已验证不通过) 材料.问题组.push_back(新方法问题种类::方向不符);
        if (评估.值域验证 == 新任务影响验证状态::已验证不通过) 材料.问题组.push_back(新方法问题种类::值域不符);
        if (!评估.预计但未发生项组.empty()) 材料.问题组.push_back(新方法问题种类::预计变化未发生);
        if (!评估.计划外状态组.empty()) 材料.问题组.push_back(新方法问题种类::发生计划外变化);
        if (材料.问题组.empty()) {
            std::lock_guard 锁(新任务互斥);
            暂存->材料.reset();
            暂存->采集请求 = {};
            暂存->结果已冻结 = true;
            暂存->正在处理 = false;
            暂存->回执.状态 = 新任务问题归档交付状态::无问题;
            return;
        }
        材料.符合预测状态组 = 评估.符合预测状态组;
        材料.预计但未发生项组 = 评估.预计但未发生项组;
        材料.计划外状态组 = 评估.计划外状态组;
        auto 请求 = 暂存->采集请求;
        请求.阶段 = 新场景快照阶段::结果取得时;
        请求.额外状态组 = 执行.实际状态节点组;
        for (const auto 状态节点 : 执行.实际状态节点组) {
            const auto 状态 = 状态服务_.获取状态(状态节点);
            if (状态) 材料.实际状态组.push_back(*状态);
        }
        std::sort(材料.实际状态组.begin(), 材料.实际状态组.end(),
            [](const auto& 左, const auto& 右) { return 左.节点 < 右.节点; });
        if (材料.本次运行对) {
            const auto& 运行 = *材料.本次运行对;
            请求.额外状态组.insert(请求.额外状态组.end(), 运行.结果.已形成状态节点组.begin(), 运行.结果.已形成状态节点组.end());
            请求.额外动态组 = 运行.结果.已形成动态节点组;
            if (运行.结果.动作动态节点) 请求.额外动态组.push_back(*运行.结果.动作动态节点);
            for (const auto& 角色 : 运行.条件.参数角色组) {
                for (const auto& 特征 : 角色.实际特征组) 请求.额外特征组.push_back(特征.特征节点);
            }
        }
        auto 后场景 = 采集->采集(请求);
        if (后场景.状态 == 新执行场景采集状态::资源失败) {
            // 已错过采集窗口时不能通过重采后来场景修补本次证据。
            std::lock_guard 锁(新任务互斥);
            暂存->材料.reset();
            暂存->正在处理 = false;
            暂存->结果已冻结 = true;
            暂存->回执.状态 = 新任务问题归档交付状态::资源失败;
            return;
        }
        材料.结果场景 = std::move(后场景.快照);
        {
            std::lock_guard 锁(新任务互斥);
            暂存->正在处理 = false;
            暂存->结果已冻结 = true;
            暂存->回执.状态 = 新任务问题归档交付状态::待重试;
        }
        (void)重试执行问题归档(任务节点, 执行实例节点);
    } catch (...) {
        if (暂存) {
            try {
                std::lock_guard 锁(新任务互斥);
                暂存->材料.reset();
                暂存->正在处理 = false;
                暂存->结果已冻结 = true;
                暂存->回执.状态 = 新任务问题归档交付状态::资源失败;
            } catch (...) {}
        }
    }
}

新任务问题归档交付结果 新_任务类::读取执行问题归档回执(
    稳定编码 任务节点, 稳定编码 执行实例节点) const noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        const auto 位置 = 方法问题暂存组_.find(执行实例节点.值);
        if (位置 != 方法问题暂存组_.end()) {
            if (位置->second->任务节点 != 任务节点) return {新任务问题归档交付状态::身份冲突};
            return 位置->second->回执;
        }
        const auto 执行 = 获取执行实例(执行实例节点);
        if (!执行) return {新任务问题归档交付状态::任务或执行不存在};
        return {执行->任务节点 == 任务节点 ? 新任务问题归档交付状态::材料缺失 : 新任务问题归档交付状态::身份冲突};
    } catch (...) { return {新任务问题归档交付状态::资源失败}; }
}

新任务问题归档交付结果 新_任务类::重试执行问题归档(
    稳定编码 任务节点, 稳定编码 执行实例节点) noexcept {
    std::shared_ptr<方法问题暂存> 暂存;
    try {
        新方法问题归档接口* 归档 = nullptr;
        std::shared_ptr<const 新方法问题反馈材料> 材料;
        {
            std::lock_guard 锁(新任务互斥);
            const auto 位置 = 方法问题暂存组_.find(执行实例节点.值);
            if (位置 == 方法问题暂存组_.end()) return 读取执行问题归档回执(任务节点, 执行实例节点);
            暂存 = 位置->second;
            if (暂存->任务节点 != 任务节点) return {新任务问题归档交付状态::身份冲突};
            if (暂存->正在处理 || 暂存->回执.状态 != 新任务问题归档交付状态::待重试) return 暂存->回执;
            if (!暂存->材料 || !方法问题归档_ || !暂存->结果已冻结) return {新任务问题归档交付状态::材料缺失};
            暂存->正在处理 = true;
            材料 = 暂存->材料;
            归档 = 方法问题归档_;
        }
        新方法问题归档结果 保存;
        const auto 已有 = 归档->查询方法问题归档(任务节点, 执行实例节点);
        if (已有.状态 == 新方法问题归档状态::已保存 || 已有.状态 == 新方法问题归档状态::精确重复) {
            if (!已有.材料 || !已有.归档节点) 保存.状态 = 新方法问题归档状态::结构不一致;
            else if (*已有.材料 != *材料) 保存.状态 = 新方法问题归档状态::身份冲突;
            else 保存 = {新方法问题归档状态::精确重复, 已有.归档节点};
        } else if (已有.状态 == 新方法问题归档状态::未找到) {
            保存 = 归档->保存方法问题(*材料);
        } else {
            保存.状态 = 已有.状态;
        }
        std::lock_guard 锁(新任务互斥);
        暂存->正在处理 = false;
        暂存->回执.归档状态 = 保存.状态;
        if ((保存.状态 == 新方法问题归档状态::已保存 || 保存.状态 == 新方法问题归档状态::精确重复)
            && 保存.归档节点 && 有效(*保存.归档节点)) {
            暂存->回执.状态 = 新任务问题归档交付状态::已归档;
            暂存->回执.消息 = 新方法问题学习消息{材料->来源抽象方法节点, 任务节点, 执行实例节点, *保存.归档节点};
            暂存->材料.reset();
            暂存->采集请求 = {};
        } else if (保存.状态 == 新方法问题归档状态::资源失败 || 保存.状态 == 新方法问题归档状态::发布结果未知) {
            暂存->回执.状态 = 新任务问题归档交付状态::待重试;
        } else {
            暂存->回执.状态 = 保存.状态 == 新方法问题归档状态::身份冲突
                ? 新任务问题归档交付状态::身份冲突 : 新任务问题归档交付状态::结构不一致;
            暂存->材料.reset();
            暂存->采集请求 = {};
        }
        return 暂存->回执;
    } catch (...) {
        if (暂存) {
            try {
                std::lock_guard 锁(新任务互斥);
                暂存->正在处理 = false;
                暂存->回执.状态 = 新任务问题归档交付状态::待重试;
                暂存->回执.归档状态 = 新方法问题归档状态::资源失败;
            } catch (...) {}
        }
        return {新任务问题归档交付状态::资源失败};
    }
}

新_任务类::新_任务类(
    新_需求类& 需求服务,
    新_方法类& 方法服务,
    新_特征值类& 特征值服务,
    新_特征类& 特征服务,
    概念_特征类& 特征概念服务,
    新_状态类& 状态服务,
    新_场景类& 场景服务,
    新_存在类& 存在服务,
    概念_二次特征类& 二次特征概念服务,
    概念_存在类& 存在概念服务) noexcept
    : 需求服务_(需求服务),
      方法服务_(方法服务),
      特征值服务_(特征值服务),
      特征服务_(特征服务),
      特征概念服务_(特征概念服务),
      状态服务_(状态服务),
      场景服务_(场景服务),
      存在服务_(存在服务),
      二次特征概念服务_(二次特征概念服务),
      存在概念服务_(存在概念服务) {}

特征概念规则状态 新_任务类::验证正式需求(稳定编码 需求节点) const noexcept {
    return 需求服务_.校验集合成员需求身份(需求节点);
}

特征任务成员读取结果 新_任务类::读取服务要求对应需求(稳定编码 确认节点) const noexcept {
    // 概念锁内的反向读取只能消费物理关联，不得获取task/demand锁或捕获概念语义。
    const auto 来源 = 需求服务_.查询服务要求采用关系(确认节点);
    switch (来源.状态) {
    case 新服务要求采用关系状态::已读取:
        if (来源.需求节点组.size() > 1)
            return {特征任务成员读取状态::结构不一致, {}};
        if (来源.需求节点组.empty()) return {特征任务成员读取状态::已读取, {}};
        if (!有效(来源.需求节点组.front())) return {特征任务成员读取状态::结构不一致, {}};
        return {特征任务成员读取状态::已读取, 来源.需求节点组.front()};
    case 新服务要求采用关系状态::未装配:
        return {特征任务成员读取状态::未装配, {}};
    case 新服务要求采用关系状态::确认不存在:
        return {特征任务成员读取状态::节点不存在, {}};
    case 新服务要求采用关系状态::资源失败:
        return {特征任务成员读取状态::资源失败, {}};
    default:
        return {特征任务成员读取状态::结构不一致, {}};
    }
}

特征概念规则状态 新_任务类::验证正式服务要求(稳定编码 确认节点) const noexcept {
    const auto 来源 = 读取服务要求对应需求(确认节点);
    switch (来源.状态) {
    case 特征任务成员读取状态::已读取: return 特征概念规则状态::已完成;
    case 特征任务成员读取状态::未装配: return 特征概念规则状态::读取接口未装配;
    case 特征任务成员读取状态::节点不存在: return 特征概念规则状态::值不存在;
    case 特征任务成员读取状态::资源失败: return 特征概念规则状态::资源失败;
    default: return 特征概念规则状态::内部不一致;
    }
}

特征任务成员读取结果 新_任务类::读取任务所属需求(稳定编码 任务节点) const noexcept {
    // schema在启动阶段冻结。反向调用不得取得任务锁，也不读取Vt的可变状态。
    try {
        if (!结构已初始化()) return {};
        const auto 失败 = [](基础结构操作状态 状态) -> 特征任务成员读取结果 {
            return {状态 == 基础结构操作状态::资源失败 ? 特征任务成员读取状态::资源失败
                : 状态 == 基础结构操作状态::节点不存在 ? 特征任务成员读取状态::节点不存在
                : 特征任务成员读取状态::结构不一致, {}};
        };
        const auto 唯一引用 = [](const 基础节点结构& 结构, 稳定编码 字段)
            -> std::optional<稳定编码> {
            std::optional<稳定编码> 值;
            for (const auto& 项 : 结构.字段组) {
                if (项.字段节点 != 字段) continue;
                const auto* 引用 = std::get_if<稳定编码>(&项.内容);
                if (值 || !引用 || !有效(*引用)) return std::nullopt;
                值 = *引用;
            }
            return 值;
        };
        const auto 任务 = 全局基础数据集.查询完整节点结构(任务节点);
        if (任务.状态 != 基础结构操作状态::已完成 || !任务.结构) return 失败(任务.状态);
        const auto 需求 = 唯一引用(*任务.结构, 任务需求字段);
        const auto 虚拟 = 唯一引用(*任务.结构, 任务虚拟存在字段);
        if (唯一引用(*任务.结构, 节点类型字段) != 任务节点类型 || !需求 || !虚拟)
            return 失败(基础结构操作状态::内部不一致);
        const auto 身份 = 验证正式需求(*需求);
        if (身份 != 特征概念规则状态::已完成)
            return {身份 == 特征概念规则状态::资源失败 ? 特征任务成员读取状态::资源失败
                : 特征任务成员读取状态::结构不一致, {}};
        const auto 根 = 全局基础数据集.查询完整节点结构(任务集合根节点);
        if (根.状态 != 基础结构操作状态::已完成 || !根.结构) return 失败(根.状态);
        if (唯一引用(*根.结构, 节点类型字段) != 任务集合根类型)
            return 失败(基础结构操作状态::内部不一致);
        const auto 已登记 = [&](稳定编码 节点) {
            std::size_t 数量 = 0;
            for (const auto& 项 : 根.结构->字段组) {
                if (项.字段节点 != 任务成员字段) continue;
                const auto* 引用 = std::get_if<稳定编码>(&项.内容);
                if (!引用 || !有效(*引用)) return false;
                if (*引用 == 节点) ++数量;
            }
            return 数量 == 1;
        };
        if (!已登记(任务节点)) return 失败(基础结构操作状态::内部不一致);
        const auto Vt = 全局基础数据集.查询完整节点结构(*虚拟);
        if (Vt.状态 != 基础结构操作状态::已完成 || !Vt.结构) return 失败(Vt.状态);
        if (唯一引用(*Vt.结构, 节点类型字段) != 任务虚拟存在节点类型
            || 唯一引用(*Vt.结构, 虚拟存在所属任务字段) != 任务节点)
            return 失败(基础结构操作状态::内部不一致);
        const auto 反向 = 全局基础数据集.查询字段引用节点结构(*需求, 任务需求字段);
        if (反向.状态 != 基础结构操作状态::已完成) return 失败(反向.状态);
        if (反向.引用字段组.size() != 1 || 反向.来源节点组.size() != 1)
            return 失败(基础结构操作状态::内部不一致);
        const auto& 唯一任务 = 反向.来源节点组.front();
        if (唯一任务.节点.编码 != 任务节点
            || 唯一引用(唯一任务, 节点类型字段) != 任务节点类型
            || 唯一引用(唯一任务, 任务需求字段) != 需求
            || 唯一引用(唯一任务, 任务虚拟存在字段) != 虚拟
            || !已登记(唯一任务.节点.编码)) return 失败(基础结构操作状态::内部不一致);
        return {特征任务成员读取状态::已读取, 需求};
    } catch (...) {
        return {特征任务成员读取状态::资源失败, {}};
    }
}

新任务操作状态 新_任务类::增加状态引用组(
    const std::vector<稳定编码>& 状态节点组,
    std::vector<稳定编码>& 已增加状态节点组) noexcept {
    try {
        已增加状态节点组.clear();
        已增加状态节点组.reserve(状态节点组.size());
        for (const auto 状态节点 : 状态节点组) {
            const auto 状态 = 状态服务_.增加引用(状态节点);
            if (状态 == 新状态操作状态::已完成) {
                已增加状态节点组.push_back(状态节点);
                continue;
            }
            if (!撤销增加状态引用组(已增加状态节点组)) {
                已增加状态节点组.clear();
                return 新任务操作状态::结构不一致;
            }
            已增加状态节点组.clear();
            if (状态 == 新状态操作状态::状态不存在) {
                return 新任务操作状态::状态不存在;
            }
            return 状态 == 新状态操作状态::资源失败
                ? 新任务操作状态::资源失败
                : 新任务操作状态::结构不一致;
        }
        return 新任务操作状态::已修改;
    } catch (...) {
        const bool 已回滚 = 撤销增加状态引用组(已增加状态节点组);
        已增加状态节点组.clear();
        return 已回滚
            ? 新任务操作状态::资源失败
            : 新任务操作状态::结构不一致;
    }
}

bool 新_任务类::撤销增加状态引用组(
    const std::vector<稳定编码>& 状态节点组) noexcept {
    bool 完整 = true;
    for (auto 位置 = 状态节点组.rbegin();
         位置 != 状态节点组.rend(); ++位置) {
        if (状态服务_.撤销增加引用(*位置)
            != 新状态操作状态::已完成) {
            完整 = false;
        }
    }
    return 完整;
}

bool 新_任务类::绑定关联检查通知(
    const void* 所有者, std::function<void()> 通知) noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        if (!所有者 || !通知) return false;
        if (关联检查通知所有者_) return 关联检查通知所有者_ == 所有者;
        关联检查通知_ = std::move(通知);
        关联检查通知所有者_ = 所有者;
        return true;
    } catch (...) { return false; }
}

void 新_任务类::解除关联检查通知(const void* 所有者) noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        if (!所有者 || 关联检查通知所有者_ != 所有者) return;
        关联检查通知_ = {};
        关联检查通知所有者_ = nullptr;
    } catch (...) {}
}

bool 新_任务类::初始化() noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        if (!需求服务_.初始化() || !方法服务_.初始化()
            || !特征服务_.初始化()
            || !状态服务_.初始化()) return false;
        const auto 核对任务状态域 = [&]() {
            const auto 概念 = 特征概念服务_.获取特征概念(任务状态特征概念);
            if (!概念 || 概念->材料类型 != 特征概念材料物理类型::I64标量) return false;
            const 特征概念值域 新域 = std::vector<特征概念I64闭区间>{{1, 12}};
            const 特征概念值域 旧域 = std::vector<特征概念I64闭区间>{{1, 11}};
            return 概念->值域 == 新域 || (概念->值域 == 旧域
                && 特征概念服务_.更新特征概念值域(
                    任务状态特征概念, 新域, 特征值服务_));
        };
        const auto 确保内部动作定义 = [&]() {
            const auto 已有 = 全局基础数据集.查询字段(任务集合根节点, 任务内部动作定义字段);
            if (!已有.empty()) return 获取任务已结算动作定义().has_value();
            const auto 定义 = 全局基础数据集.新建节点();
            if (!有效(定义)) return false;
            std::vector<稳定编码> 字段;
            字段.reserve(3);
            if (!写入节点字段(定义, 节点类型字段, 任务已结算动作定义类型, 字段)
                || !写入I64字段(定义, 内部动作操作码字段, 1, 字段)
                || !写入节点字段(任务集合根节点, 任务内部动作定义字段, 定义, 字段)) {
                回滚字段(字段);
                (void)全局基础数据集.删除节点(定义);
                return false;
            }
            if (获取任务已结算动作定义() == 定义) return true;
            回滚字段(字段);
            (void)全局基础数据集.删除节点(定义);
            return false;
        };
        if (结构已初始化())
            return 结构仍存在() && 核对任务状态域() && 确保内部动作定义();

        auto 建立 = [](稳定编码& 节点) {
            if (有效(节点)) return 节点仍存在(节点);
            节点 = 全局基础数据集.新建节点();
            return 有效(节点);
        };
        if (!任务执行组织存储::初始化(建立)) return false;
        const bool 基础完成 = 建立(任务集合根节点)
            && 建立(任务已结算动作定义类型)
            && 建立(任务内部动作定义字段)
            && 建立(内部动作操作码字段)
            && 建立(结算来源任务字段)
            && 建立(结算来源虚拟存在字段)
            && 建立(结算来源状态特征字段)
            && 建立(结算冻结前值字段)
            && 建立(结算目标值域字段)
            && 建立(执行普通动作字段)
            && 建立(节点类型字段)
            && 建立(任务集合根类型)
            && 建立(任务节点类型)
            && 建立(任务虚拟存在节点类型)
            && 建立(任务执行实例节点类型)
            && 建立(任务预计变化阶段节点类型)
            && 建立(任务预计变化项节点类型)
            && 建立(任务结果获取渠道方案节点类型)
            && 建立(任务结果获取目标节点类型)
            && 建立(任务禁止结果目标节点类型)
            && 建立(任务实际状态项节点类型)
            && 建立(任务影响评估节点类型)
            && 建立(任务完成结算节点类型)
            && 建立(任务候选方法材料节点类型)
            && 建立(任务成员字段)
            && 建立(任务需求字段)
            && 建立(任务虚拟存在字段)
            && 建立(任务最近方法字段)
            && 建立(任务方法实例字段)
            && 建立(任务最近执行字段)
            && 建立(任务影响评估字段)
            && 建立(任务完成结算字段)
            && 建立(任务候选方法材料字段)
            && 建立(任务待回收候选方法材料编码字段)
            && 建立(候选所属任务字段)
            && 建立(候选需求字段)
            && 建立(候选目标特征类型字段)
            && 建立(候选目标形式字段)
            && 建立(候选目标值域字段)
            && 建立(候选目标比较关系字段)
            && 建立(候选目标变化方向字段)
            && 建立(候选查询作用域字段)
            && 建立(候选查询实现方式字段)
            && 建立(候选查询作用存在字段)
            && 建立(候选查询特征类型字段)
            && 建立(候选查询可能结果字段)
            && 建立(候选查询状态字段)
            && 建立(候选查询完整字段)
            && 建立(候选方法字段)
            && 建立(虚拟存在所属任务字段)
            && 建立(虚拟存在状态特征字段)
            && 建立(虚拟存在专属概念字段)
            && 建立(执行所属任务字段)
            && 建立(执行方法字段)
            && 建立(执行状态字段)
            && 建立(执行开始时间语义字段)
            && 建立(执行开始时间字段)
            && 建立(执行结束时间语义字段)
            && 建立(执行结束时间字段)
            && 建立(执行预计阶段字段)
            && 建立(执行结果获取多渠道关系字段)
            && 建立(执行结果获取渠道方案字段)
            && 建立(执行结果获取目标字段)
            && 建立(执行禁止结果关系规则字段)
            && 建立(执行禁止结果目标字段)
            && 建立(执行动作输出状态字段)
            && 建立(执行实际状态项字段)
            && 建立(执行评估字段)
            && 建立(阶段顺序字段)
            && 建立(阶段变化项字段)
            && 建立(变化作用对象字段)
            && 建立(变化特征类型字段)
            && 建立(变化关系字段)
            && 建立(变化方向符号字段)
            && 建立(变化值域字段)
            && 建立(变化精确数值域字段)
            && 建立(变化必须出现字段)
            && 建立(结果目标顺序字段)
            && 建立(结果目标作用对象字段)
            && 建立(结果目标实际特征字段)
            && 建立(结果目标特征类型根字段)
            && 建立(结果目标动作前准确值字段)
            && 建立(结果目标必须出现字段)
            && 建立(渠道方案顺序字段)
            && 建立(渠道方案类型字段)
            && 建立(渠道方案证据质量字段)
            && 建立(渠道方案最大等待纳秒字段)
            && 建立(渠道方案等待条件概念字段)
            && 建立(禁止目标顺序字段)
            && 建立(禁止目标作用对象字段)
            && 建立(禁止目标实际特征字段)
            && 建立(禁止目标特征类型根字段)
            && 建立(禁止目标动作前准确值字段)
            && 建立(禁止目标方向字段)
            && 建立(禁止目标值域字段)
            && 建立(禁止目标精确数值域字段)
            && 建立(禁止目标状态动态字段)
            && 建立(实际状态顺序字段)
            && 建立(实际状态节点字段)
            && 建立(评估所属任务字段)
            && 建立(评估需求字段)
            && 建立(评估执行实例字段)
            && 建立(评估方法字段)
            && 建立(评估需求结果状态字段)
            && 建立(评估需求实际特征字段)
            && 建立(评估需求准确差异字段)
            && 建立(评估符合预测状态字段)
            && 建立(评估缺失变化项字段)
            && 建立(评估计划外状态字段)
            && 建立(评估方向验证字段)
            && 建立(评估幅度验证字段)
            && 建立(评估值域验证字段)
            && 建立(评估多渠道验证字段)
            && 建立(评估禁止结果验证字段)
            && 建立(评估副作用验证字段)
            && 建立(评估归因验证字段)
            && 建立(结算所属任务字段)
            && 建立(结算需求字段)
            && 建立(结算执行实例字段)
            && 建立(结算影响评估字段)
            && 建立(结算完成来源字段)
            && 建立(结算结果状态字段);
        if (!基础完成) return false;

        const auto 根类型组 = 全局基础数据集.查询字段(
            任务集合根节点, 节点类型字段);
        if (根类型组.empty()) {
            if (!有效(全局基础数据集.添加字段节点(
                任务集合根节点, 节点类型字段, 任务集合根类型))) return false;
        } else if (!节点类型为(任务集合根节点, 任务集合根类型)) {
            return false;
        }

        if (!有效(任务状态特征概念)) {
            特征概念定义 定义;
            定义.材料类型 = 特征概念材料物理类型::I64标量;
            定义.值域 = std::vector<特征概念I64闭区间>{{
                static_cast<std::int64_t>(新任务状态::已建立),
                static_cast<std::int64_t>(新任务状态::已结算)}};
            定义.比较规则 = 特征概念比较规则::I64数值;
            定义.聚合规则 = 特征概念聚合规则::不自动聚合;
            任务状态特征概念 = 特征概念服务_.建立或取得特征概念(
                定义, 特征值服务_);
        }
        if (!有效(任务状态特征概念)
            || !特征概念服务_.是特征概念节点(任务状态特征概念)) {
            return false;
        }
        return 结构仍存在() && 核对任务状态域() && 确保内部动作定义();
    } catch (...) {
        return false;
    }
}

新任务执行组织结果 新_任务类::建立或取得任务方法执行组织(
    稳定编码 任务节点, const std::vector<稳定编码>& 来源方法节点组) noexcept {
    return 实施任务方法执行组织(任务节点, 来源方法节点组, false);
}

新任务执行组织结果 新_任务类::增补任务方法执行组织(
    稳定编码 任务节点, const std::vector<稳定编码>& 来源方法节点组) noexcept {
    return 实施任务方法执行组织(任务节点, 来源方法节点组, true);
}

新任务执行组织结果 新_任务类::实施任务方法执行组织(
    稳定编码 任务节点, const std::vector<稳定编码>& 来源方法节点组, bool 允许增补) noexcept {
    using 状态 = 新任务执行组织状态;
    using 方法状态 = 新_方法类::执行结构读取状态;
    using namespace 任务执行组织存储;
    try {
        if (!有效(任务节点)) return {状态::入口拒绝, {}};
        // 方法快照先结束；禁止持有任务锁再进入方法闭包锁。
        const auto 闭包 = 方法服务_.读取方法执行结构闭包(来源方法节点组);
        switch (闭包.状态) {
        case 方法状态::入口拒绝: return {状态::入口拒绝, {}};
        case 方法状态::尚未初始化: return {状态::尚未初始化, {}};
        case 方法状态::方法不存在: return {状态::来源方法不存在, {}};
        case 方法状态::来源不是抽象方法: return {状态::来源不是抽象方法, {}};
        case 方法状态::已读取: break;
        default: 要求(false, "unknown method closure status");
        }
        std::lock_guard 锁(新任务互斥);
        if (!结构已初始化()) return {状态::尚未初始化, {}};
        要求(结构仍存在(), "task schema missing");
        const auto 节点 = 全局基础数据集.查询完整节点结构(任务节点);
        if (节点.状态 == 基础结构操作状态::节点不存在) return {状态::任务不存在, {}};
        要求(节点.状态 == 基础结构操作状态::已完成 && 节点.结构, "task read failed");
        if (!节点类型为(任务节点, 任务节点类型)) return {状态::任务不存在, {}};
        const auto 任务 = 读取任务信息(任务节点, 特征服务_);
        要求(任务.has_value() && 需求服务_.获取需求(任务->需求节点).has_value(), "formal task closure invalid");
        auto 定义 = 转换定义(任务节点, 任务->任务虚拟存在节点, 闭包);
        核验图(定义); 核验概念引用(定义, 特征概念服务_, 存在概念服务_, 二次特征概念服务_);
        std::map<稳定编码, 基础节点结构> 原始结构;
        auto 已有 = 读取已加锁(任务节点, 任务->任务虚拟存在节点, &原始结构);
        if (已有) {
            if (!允许增补) {
                if (同义定义(*已有) != 定义) return {状态::已有组织不同, {}};
                return {状态::精确重复, std::move(已有)};
            }
            auto 合并 = 合并定义(*已有, 定义);
            if (合并 && 同义定义(*已有) == *合并) return {状态::精确重复, std::move(已有)};
            if (任务->当前状态 != 新任务状态::已建立 && 任务->当前状态 != 新任务状态::待查找方法
                && 任务->当前状态 != 新任务状态::待检查条件 && 任务->当前状态 != 新任务状态::等待条件)
                return {状态::任务状态不允许, {}};
            if (!合并) return {状态::增补结构暂不支持, {}};
            核验图(*合并); 核验概念引用(*合并, 特征概念服务_, 存在概念服务_, 二次特征概念服务_);
            const 写器 增量(*合并, &*已有);
            const auto 请求 = 增量.追加请求(原始结构);
            const auto 发布 = 全局基础数据集.原子追加节点组(请求);
            要求(发布.状态 == 基础节点组追加状态::已完成
                && 发布.新节点组.size() == 请求.新节点组.size()
                && 发布.追加字段关系组.size() == 请求.追加字段组.size(), "atomic organization append failed");
            std::map<稳定编码, 基础节点结构> 当前结构;
            auto 读回 = 读取已加锁(任务节点, 任务->任务虚拟存在节点, &当前结构);
            要求(读回 && 读回->节点 == 已有->节点 && 同义定义(*读回) == *合并,
                "appended organization differs from merged definition");
            // 同义图校验之外，还逐字段核对旧身份以及唯一允许的新增引用。
            for (const auto& [节点码, 旧结构] : 原始结构) {
                const auto& 新结构 = 当前结构.at(节点码);
                auto 预期字段 = 旧结构.字段组;
                for (std::size_t i = 0; i < 请求.追加字段组.size(); ++i) {
                    const auto& 项 = 请求.追加字段组[i];
                    if (项.所属节点 != 节点码) continue;
                    基础字段内容 内容;
                    if (const auto* 值 = std::get_if<基础值>(&项.内容)) 内容 = *值;
                    else if (const auto* 引用 = std::get_if<稳定编码>(&项.内容)) 内容 = *引用;
                    else 内容 = 发布.新节点组.at(std::get<基础组内节点引用>(项.内容).索引);
                    预期字段.push_back({发布.追加字段关系组[i], 节点码, 项.字段节点, std::move(内容)});
                }
                auto 实际字段 = 新结构.字段组;
                const auto 排序 = [](auto& 组) {
                    std::sort(组.begin(), 组.end(), [](const auto& 左, const auto& 右) { return 左.编码 < 右.编码; });
                };
                排序(预期字段); 排序(实际字段);
                要求(新结构.节点 == 旧结构.节点 && 预期字段 == 实际字段, "old organization field changed during append");
            }
            return {状态::已增补, std::move(读回)};
        }
        if (任务->当前状态 != 新任务状态::已建立 && 任务->当前状态 != 新任务状态::待查找方法
            && 任务->当前状态 != 新任务状态::待检查条件 && 任务->当前状态 != 新任务状态::等待条件)
            return {状态::任务状态不允许, {}};
        const 写器 待发布(定义);
        const auto 发布 = 全局基础数据集.原子建立节点组(待发布.节点组);
        要求(发布.状态 == 基础结构操作状态::已完成 && 发布.节点组.size() == 待发布.节点组.size(),
            "atomic task organization publication failed");
        auto 读回 = 读取已加锁(任务节点, 任务->任务虚拟存在节点);
        要求(读回 && 读回->节点 == 发布.节点组.front() && 同义定义(*读回) == 定义,
            "published task organization differs from adopted definition");
        return {状态::已建立, std::move(读回)};
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"建立任务方法执行组织", L"组织转换或发布故障", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"建立任务方法执行组织", L"未知组织转换或发布故障");
    }
}

新任务执行组织结果 新_任务类::读取任务方法执行组织(稳定编码 任务节点) const noexcept {
    using 状态 = 新任务执行组织状态;
    using namespace 任务执行组织存储;
    try {
        if (!有效(任务节点)) return {状态::入口拒绝, {}};
        std::lock_guard 锁(新任务互斥);
        if (!结构已初始化()) return {状态::尚未初始化, {}};
        要求(结构仍存在(), "task schema missing");
        const auto 节点 = 全局基础数据集.查询完整节点结构(任务节点);
        if (节点.状态 == 基础结构操作状态::节点不存在) return {状态::任务不存在, {}};
        要求(节点.状态 == 基础结构操作状态::已完成 && 节点.结构, "task read failed");
        if (!节点类型为(任务节点, 任务节点类型)) return {状态::任务不存在, {}};
        const auto 任务 = 读取任务信息(任务节点, 特征服务_);
        要求(任务.has_value() && 需求服务_.获取需求(任务->需求节点).has_value(), "formal task closure invalid");
        auto 组织 = 读取已加锁(任务节点, 任务->任务虚拟存在节点);
        if (!组织) return {状态::尚未建立, {}};
        核验概念引用(*组织, 特征概念服务_, 存在概念服务_, 二次特征概念服务_);
        return {状态::已读取, std::move(组织)};
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取任务方法执行组织", L"组织读取故障", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取任务方法执行组织", L"未知组织读取故障");
    }
}

新任务建立结果 新_任务类::建立或取得任务(
    稳定编码 需求节点) noexcept {
    新任务建立结果 结果;
    try {
        std::lock_guard 锁(新任务互斥);
        if (!结构仍存在()) {
            结果.状态 = 新任务操作状态::尚未初始化;
            return 结果;
        }
        if (!需求服务_.获取需求(需求节点)) {
            结果.状态 = 新任务操作状态::需求不存在;
            return 结果;
        }
        std::vector<稳定编码> 已有任务;
        for (const auto& 字段 : 全局基础数据集.查询目标字段(
            需求节点, 任务需求字段)) {
            if (!节点类型为(字段.所属节点, 任务节点类型)) {
                结果.状态 = 新任务操作状态::结构不一致;
                return 结果;
            }
            已有任务.push_back(字段.所属节点);
        }
        std::sort(已有任务.begin(), 已有任务.end());
        已有任务.erase(std::unique(已有任务.begin(), 已有任务.end()),
            已有任务.end());
        if (已有任务.size() > 1) {
            结果.状态 = 新任务操作状态::需求已有多个任务;
            return 结果;
        }
        if (已有任务.size() == 1) {
            const auto 现有 = 读取任务信息(已有任务.front(), 特征服务_);
            if (!现有 || 现有->需求节点 != 需求节点) {
                结果.状态 = 新任务操作状态::结构不一致;
                return 结果;
            }
            结果.状态 = 新任务操作状态::已复用;
            结果.任务节点 = 现有->节点;
            结果.任务虚拟存在节点 = 现有->任务虚拟存在节点;
            if (!同步服务任务集合已加锁()) {
                结果.状态 = 新任务操作状态::服务任务集合同步失败;
            }
            return 结果;
        }

        const 关联检查离域通知 变更通知{关联检查通知_};
        const auto 状态特征 = 特征服务_.建立特征(任务状态特征概念);
        if (!有效(状态特征)) {
            结果.状态 = 新任务操作状态::资源失败;
            return 结果;
        }
        const auto 初始写入 = 特征服务_.添加特征值(
            状态特征, 新特征准确值{
                static_cast<std::int64_t>(新任务状态::已建立)});
        if (初始写入.状态 != 新特征写入状态::已完成
            && 初始写入.状态 != 新特征写入状态::无变化) {
            (void)特征服务_.删除特征(状态特征);
            结果.状态 = 新任务操作状态::资源失败;
            return 结果;
        }

        const auto 任务节点 = 全局基础数据集.新建节点();
        const auto 虚拟存在 = 全局基础数据集.新建节点();
        if (!有效(任务节点) || !有效(虚拟存在)) {
            if (有效(任务节点)) (void)全局基础数据集.删除节点(任务节点);
            if (有效(虚拟存在)) (void)全局基础数据集.删除节点(虚拟存在);
            (void)特征服务_.删除特征(状态特征);
            结果.状态 = 新任务操作状态::资源失败;
            return 结果;
        }

        std::vector<稳定编码> 已写字段;
        const bool 完成 =
            写入节点字段(任务节点, 节点类型字段,
                任务节点类型, 已写字段)
            && 写入节点字段(任务节点, 任务需求字段,
                需求节点, 已写字段)
            && 写入节点字段(任务节点, 任务虚拟存在字段,
                虚拟存在, 已写字段)
            && 写入节点字段(虚拟存在, 节点类型字段,
                任务虚拟存在节点类型, 已写字段)
            && 写入节点字段(虚拟存在, 虚拟存在所属任务字段,
                任务节点, 已写字段)
            && 写入节点字段(虚拟存在, 虚拟存在状态特征字段,
                状态特征, 已写字段)
            && 写入节点字段(任务集合根节点, 任务成员字段,
                任务节点, 已写字段);
        if (!完成) {
            回滚字段(已写字段);
            (void)全局基础数据集.删除节点(虚拟存在);
            (void)全局基础数据集.删除节点(任务节点);
            (void)特征服务_.删除特征(状态特征);
            结果.状态 = 新任务操作状态::资源失败;
            return 结果;
        }
        const auto 读回 = 读取任务信息(任务节点, 特征服务_);
        if (!读回 || 读回->需求节点 != 需求节点
            || 读回->任务虚拟存在节点 != 虚拟存在
            || 读回->当前状态 != 新任务状态::已建立) {
            回滚字段(已写字段);
            (void)全局基础数据集.删除节点(虚拟存在);
            (void)全局基础数据集.删除节点(任务节点);
            (void)特征服务_.删除特征(状态特征);
            结果.状态 = 新任务操作状态::结构不一致;
            return 结果;
        }
        结果.状态 = 新任务操作状态::已建立;
        结果.任务节点 = 任务节点;
        结果.任务虚拟存在节点 = 虚拟存在;
        if (!同步服务任务集合已加锁()) {
            结果.状态 = 新任务操作状态::服务任务集合同步失败;
        }
        return 结果;
    } catch (...) {
        结果.状态 = 新任务操作状态::资源失败;
        return 结果;
    }
}

std::optional<新任务信息> 新_任务类::获取任务(
    稳定编码 任务节点) const noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        auto 任务 = 读取任务信息(任务节点, 特征服务_);
        if (任务 && 任务->方法实例节点) {
            if (!方法服务_.获取混合方法实例(*任务->方法实例节点)) return std::nullopt;
        }
        return 任务;
    } catch (...) {
        return std::nullopt;
    }
}

std::optional<新任务虚拟存在信息> 新_任务类::获取任务虚拟存在(
    稳定编码 任务节点) const noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        if (!结构仍存在()) return std::nullopt;
        return 读取任务虚拟存在信息(任务节点, 特征服务_);
    } catch (...) {
        return std::nullopt;
    }
}

std::optional<新任务执行信息> 新_任务类::获取执行实例(
    稳定编码 执行实例节点) const noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        return 读取执行信息(执行实例节点, 状态服务_, 特征服务_, 特征概念服务_);
    } catch (...) {工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取任务执行实例",L"执行证据结构读取故障");}
}

std::optional<新任务本能采用材料> 新_任务类::读取执行本能采用(稳定编码 X) const noexcept {
    const auto 执行 = 获取执行实例(X);return 执行 ? 执行->本能采用 : std::nullopt;
}
新任务本能输入复核状态 新_任务类::复核执行本能输入(稳定编码 X,const 方法参数场景& 场景) const noexcept {
    try{
        std::lock_guard 锁(新任务互斥);const auto 执行=获取执行实例(X);
        if(!执行||!执行->本能采用)return 新任务本能输入复核状态::执行不在当前位;
        const auto 任务=获取任务(执行->任务节点);
        if(!任务)throw std::logic_error("执行所属任务不可读");
        if(任务->当前状态!=新任务状态::执行中||任务->最近执行实例节点!=X
            ||任务->最近方法节点!=执行->方法节点||执行->状态!=新任务执行状态::执行中||执行->结束时间)
            return 新任务本能输入复核状态::执行不在当前位;
        if(场景.方法节点()!=执行->方法节点||场景.全部角色绑定()!=执行->本能采用->参数绑定组)
            return 新任务本能输入复核状态::场景不一致;
        const auto 核验=核验任务条件和参数内部(任务->节点,&*执行->本能采用,true);
        return 核验.状态==新任务治理条件筹办状态::已完成&&核验.方法筹办组.size()==1
            &&核验.方法筹办组.front().状态==新任务候选方法筹办状态::条件具备
            ?新任务本能输入复核状态::仍具备:新任务本能输入复核状态::条件已变化;
    }catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"复核执行本能输入",L"执行采用核验工程故障");}
}

std::optional<新任务影响评估信息> 新_任务类::获取影响评估(
    稳定编码 评估节点) const noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        return 读取评估信息(评估节点);
    } catch (...) {
        return std::nullopt;
    }
}

std::optional<新任务完成结算信息> 新_任务类::获取完成结算(
    稳定编码 结算节点) const noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        return 读取完成结算信息(结算节点, 状态服务_);
    } catch (...) {
        return std::nullopt;
    }
}

std::optional<新任务候选方法材料> 新_任务类::获取候选方法材料(
    稳定编码 材料节点) const noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        if (!结构仍存在()) return std::nullopt;
        return 读取候选方法材料信息(材料节点);
    } catch (...) {
        return std::nullopt;
    }
}

std::optional<新任务候选方法材料> 新_任务类::获取当前候选方法材料(
    稳定编码 任务节点) const noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        const auto 任务 = 读取任务信息(任务节点, 特征服务_);
        if (!任务 || !任务->当前候选方法材料节点) return std::nullopt;
        return 读取候选方法材料信息(*任务->当前候选方法材料节点);
    } catch (...) {
        return std::nullopt;
    }
}

std::optional<稳定编码> 新_任务类::查询需求任务(
    稳定编码 需求节点) const noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        if (!结构仍存在() || !需求服务_.获取需求(需求节点)) {
            return std::nullopt;
        }
        std::vector<稳定编码> 任务组;
        for (const auto& 字段 : 全局基础数据集.查询目标字段(
            需求节点, 任务需求字段)) {
            if (!节点类型为(字段.所属节点, 任务节点类型)) {
                return std::nullopt;
            }
            任务组.push_back(字段.所属节点);
        }
        std::sort(任务组.begin(), 任务组.end());
        任务组.erase(std::unique(任务组.begin(), 任务组.end()), 任务组.end());
        return 任务组.size() == 1
            ? std::optional<稳定编码>{任务组.front()} : std::nullopt;
    } catch (...) {
        return std::nullopt;
    }
}

新任务列表结果 新_任务类::查询服务任务(稳定编码 排除需求) const noexcept {
    新任务列表结果 结果;
    try {
        std::lock_guard 锁(新任务互斥);
        const auto 来源 = 读取需求列表任务对应();
        if (来源.状态 != 新任务操作状态::已找到) {
            结果.状态 = 来源.状态;
            return 结果;
        }
        结果.状态 = 新任务操作状态::结构不一致;
        std::map<稳定编码, const 新需求任务对应项*> 需求索引;
        for (const auto& 项 : 来源.对应项组) {
            if (!需求索引.emplace(项.需求.节点, &项).second) return 结果;
        }
        std::vector<稳定编码> 成员;
        bool 排除合法 = false;
        for (const auto& 项 : 来源.对应项组) {
            const auto* 祖先 = &项.需求;
            std::size_t 步数 = 0;
            while (!祖先->根角色) {
                if (++步数 > 需求索引.size() || !祖先->父需求节点) return 结果;
                const auto 父 = 需求索引.find(*祖先->父需求节点);
                if (父 == 需求索引.end()) return 结果;
                祖先 = &父->second->需求;
            }
            if (祖先->父需求节点) return 结果;
            if (*祖先->根角色 == 新需求根角色::服务) {
                if (祖先->节点 != 服务根需求节点) return 结果;
                if (项.需求.节点 == 排除需求) {
                    排除合法 = 项.需求.父需求节点.has_value();
                } else if (项.任务) {
                    成员.push_back(项.任务->节点);
                }
            } else if (*祖先->根角色 != 新需求根角色::安全
                || 祖先->节点 != 安全根需求节点) {
                return 结果;
            }
        }
        if (!排除合法) {
            结果.状态 = 新任务操作状态::入口拒绝;
            return 结果;
        }
        // 完整来源已证明唯一性；这里只规范顺序，不掩盖重复来源。
        std::sort(成员.begin(), 成员.end());
        结果.任务节点组 = std::move(成员);
        结果.状态 = 新任务操作状态::已找到;
        return 结果;
    } catch (...) {
        结果.任务节点组.clear();
        结果.状态 = 新任务操作状态::资源失败;
        return 结果;
    }
}

bool 新_任务类::同步服务任务集合已加锁() noexcept {
    if (!服务任务同步所有者_) return true;
    try {
        const auto 来源 = 查询服务任务(服务任务排除需求_);
        return 来源.状态 == 新任务操作状态::已找到
            && 服务任务同步_ && 服务任务同步_(来源.任务节点组);
    } catch (...) {
        return false;
    }
}

新任务操作状态 新_任务类::绑定服务任务集合同步(
    const void* 所有者, 稳定编码 排除需求,
    新服务任务集合同步函数 同步) noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        if (!所有者 || !同步 || !有效(排除需求)) return 新任务操作状态::入口拒绝;
        if (服务任务同步所有者_) {
            if (服务任务同步所有者_ != 所有者 || 服务任务排除需求_ != 排除需求) {
                return 新任务操作状态::入口拒绝;
            }
            return 同步服务任务集合已加锁() ? 新任务操作状态::精确重复
                : 新任务操作状态::服务任务集合同步失败;
        }
        const auto 来源 = 查询服务任务(排除需求);
        if (来源.状态 != 新任务操作状态::已找到) return 来源.状态;
        服务任务同步_ = std::move(同步);
        服务任务排除需求_ = 排除需求;
        服务任务同步所有者_ = 所有者;
        // 回填失败也保留绑定，不能让后续任务静默绕过同步。
        return 同步服务任务集合已加锁() ? 新任务操作状态::已修改
            : 新任务操作状态::服务任务集合同步失败;
    } catch (...) {
        return 新任务操作状态::资源失败;
    }
}

新任务操作状态 新_任务类::解除服务任务集合同步(const void* 所有者) noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        if (!所有者) return 新任务操作状态::入口拒绝;
        if (!服务任务同步所有者_) return 新任务操作状态::无变化;
        if (服务任务同步所有者_ != 所有者) return 新任务操作状态::入口拒绝;
        服务任务同步_ = {};
        服务任务排除需求_ = {};
        服务任务同步所有者_ = nullptr;
        return 新任务操作状态::已修改;
    } catch (...) {
        return 新任务操作状态::资源失败;
    }
}

新需求任务对应结果 新_任务类::读取需求列表任务对应() const noexcept {
    新需求任务对应结果 结果;
    结果.状态 = 新任务操作状态::结构不一致;
    try {
        // 与既有任务写入保持task→demand方向。整个对应读取期间固定T，
        // D快照之后新增的需求不属于本次快照，调用者下次须重新读取。
        std::lock_guard 锁(新任务互斥);
        if (!结构仍存在()) {
            结果.状态 = 新任务操作状态::尚未初始化;
            return 结果;
        }
        const auto 列表 = 需求服务_.查询全部需求列表();
        if (列表.状态 != 新需求操作状态::已找到) {
            if (列表.状态 == 新需求操作状态::尚未初始化)
                结果.状态 = 新任务操作状态::尚未初始化;
            else if (列表.状态 == 新需求操作状态::资源失败)
                结果.状态 = 新任务操作状态::资源失败;
            return 结果;
        }
        std::map<稳定编码, const 新需求信息*> 需求映射;
        for (const auto& 需求 : 列表.需求组) 需求映射.emplace(需求.节点, &需求);
        std::set<稳定编码> 登记任务;
        std::map<稳定编码, 新任务信息> 需求任务;
        for (const auto& 字段 : 全局基础数据集.查询字段(
                任务集合根节点, 任务成员字段)) {
            const auto* 节点 = std::get_if<稳定编码>(&字段.内容);
            if (!节点 || !登记任务.insert(*节点).second) return 结果;
            auto 任务 = 获取任务(*节点);
            if (!任务 || !需求映射.contains(任务->需求节点)) return 结果;
            const auto 需求节点 = 任务->需求节点;
            if (!需求任务.emplace(需求节点, std::move(*任务)).second) {
                结果.状态 = 新任务操作状态::需求已有多个任务;
                return 结果;
            }
        }
        std::set<稳定编码> 类型任务;
        for (const auto& 字段 : 全局基础数据集.查询目标字段(
                任务节点类型, 节点类型字段)) {
            if (!节点类型为(字段.所属节点, 任务节点类型)
                || !类型任务.insert(字段.所属节点).second) return 结果;
        }
        if (类型任务 != 登记任务) return 结果;

        std::vector<新需求任务对应项> 候选;
        候选.reserve(列表.需求组.size());
        for (const auto& 列表项 : 列表.列表项组) {
            for (const auto 需求节点 : 列表项.需求节点组) {
                const auto 反向 = 全局基础数据集.查询目标字段(需求节点, 任务需求字段);
                const auto 任务 = 需求任务.find(需求节点);
                std::optional<新任务信息> 对应任务;
                if (任务 == 需求任务.end()) {
                    if (!反向.empty()) return 结果;
                } else {
                    if (反向.size() != 1 || 反向.front().所属节点 != 任务->second.节点)
                        return 结果;
                    const auto 正向 = 全局基础数据集.查询字段(任务->second.节点, 任务需求字段);
                    if (正向.size() != 1 || 正向.front().编码 != 反向.front().编码)
                        return 结果;
                    对应任务 = 任务->second;
                }
                候选.push_back({列表项.节点, *需求映射.at(需求节点), std::move(对应任务)});
            }
        }
        结果.对应项组 = std::move(候选);
        结果.状态 = 新任务操作状态::已找到;
        return 结果;
    } catch (...) {
        结果.状态 = 新任务操作状态::资源失败;
        结果.对应项组.clear();
        return 结果;
    }
}

新任务列表结果 新_任务类::查询全部任务() const noexcept {
    新任务列表结果 结果;
    try {
        std::lock_guard 锁(新任务互斥);
        if (!结构仍存在()) {
            结果.状态 = 新任务操作状态::尚未初始化;
            return 结果;
        }
        const auto 全集 = 读取任务登记全集();
        if (!全集) {
            结果.状态 = 新任务操作状态::结构不一致;
            return 结果;
        }
        for (const auto 任务 : *全集) {
            if (!读取任务信息(任务, 特征服务_)) {
                结果.状态 = 新任务操作状态::结构不一致;
                结果.任务节点组.clear();
                return 结果;
            }
        }
        结果.任务节点组 = *全集;
        结果.状态 = 新任务操作状态::已找到;
        return 结果;
    } catch (...) {
        结果.状态 = 新任务操作状态::资源失败;
        结果.任务节点组.clear();
        return 结果;
    }
}

新任务调度材料项 新_任务类::读取任务调度项已加锁(
    稳定编码 任务节点, bool& 资源失败) const {
    新任务调度材料项 项;
    项.定位.任务节点 = 任务节点;
    项.任务 = 读取任务信息(任务节点, 特征服务_);
    if (!项.任务) return 项;
    const auto& 任务 = *项.任务;
    项.定位.需求节点 = 任务.需求节点;
    项.定位.任务状态 = 任务.当前状态;
    项.定位.候选材料节点 = 任务.当前候选方法材料节点;
    项.定位.方法实例节点 = 任务.方法实例节点;
    项.定位.方法执行组织节点=任务.方法执行组织节点;
    项.定位.最近执行实例节点 = 任务.最近执行实例节点;
    const auto 反向 = 全局基础数据集.查询目标字段(任务.需求节点, 任务需求字段);
    if (反向.size() != 1 || 反向.front().所属节点 != 任务节点) return 项;
    项.需求 = 需求服务_.获取需求(任务.需求节点);
    if (!项.需求) return 项;
    auto 祖先 = 项.需求;
    std::set<稳定编码> 已见需求;
    while (祖先) {
        if (!已见需求.insert(祖先->节点).second) return 项;
        if (!祖先->父需求节点) break;
        if (祖先->根角色) return 项;
        祖先 = 需求服务_.获取需求(*祖先->父需求节点);
    }
    if (!祖先 || !祖先->根角色
        || 需求服务_.查询根需求(*祖先->根角色) != 祖先->节点) return 项;

    if (任务.当前候选方法材料节点) {
        项.候选材料 = 读取候选方法材料信息(*任务.当前候选方法材料节点);
        if (!项.候选材料 || 项.候选材料->任务节点 != 任务节点
            || 项.候选材料->需求节点 != 任务.需求节点
            || 项.候选材料->需求目标 != 项.需求->目标
            || !项.候选材料->查询完整
            || 项.候选材料->查询状态 != 新方法操作状态::已找到) return 项;
    }
    if (任务.最近执行实例节点) {
        项.执行 = 读取执行信息(*任务.最近执行实例节点,
            状态服务_, 特征服务_, 特征概念服务_);
        if (!项.执行 || 项.执行->任务节点 != 任务节点) return 项;
        项.定位.影响评估节点 = 项.执行->影响评估节点;
        if (项.执行->影响评估节点) {
            项.影响评估 = 读取评估信息(*项.执行->影响评估节点);
            if (!项.影响评估 || 项.影响评估->任务节点 != 任务节点
                || 项.影响评估->需求节点 != 任务.需求节点
                || 项.影响评估->执行实例节点 != 项.执行->节点
                || 项.影响评估->方法节点 != 项.执行->方法节点
                || std::count(任务.影响评估节点组.begin(),
                    任务.影响评估节点组.end(), 项.影响评估->节点) != 1) return 项;
        }
    }
    if (!项.定位.完整() || 方法实例采用待核查_.contains(任务节点)) return 项;
    if (任务.完成结算节点) {
        const auto 结算 = 读取完成结算信息(*任务.完成结算节点, 状态服务_);
        if (!结算 || 结算->任务节点 != 任务节点
            || 结算->需求节点 != 任务.需求节点) return 项;
    }
    // 终结/结果/后继保持原管线，不因扫描获得W1资格。
    if (任务.当前状态 == 新任务状态::已失败
        || 任务.当前状态 == 新任务状态::已取消
        || 任务.当前状态 == 新任务状态::已结算) {
        if (任务.当前状态 == 新任务状态::已结算 && !任务.完成结算节点) return 项;
        项.状态 = 新任务调度项状态::终结;
        项.等待原因 = 新任务等待原因::无;
        return 项;
    }
    if (任务.当前状态 == 新任务状态::已完成) {
        if (!项.执行 || 项.执行->状态 != 新任务执行状态::已评估
            || !项.影响评估) return 项;
        项.状态 = 任务.完成结算节点 ? 新任务调度项状态::终结
            : 新任务调度项状态::完成待结算;
        项.等待原因 = 新任务等待原因::无;
        return 项;
    }
    if (任务.服务结算来源) {
        const auto 来源 = 读取任务虚拟存在引用(任务.服务结算来源->来源任务);
        if (!服务结算配置_ || 服务结算配置_->结算任务节点 != 任务节点
            || 服务结算配置_->结算需求 != 任务.需求节点 || !来源
            || 来源->当前状态 != static_cast<std::int64_t>(新任务状态::已完成)
            || 任务.服务结算来源->冻结前值 != 服务结算配置_->冻结前值
            || 任务.服务结算来源->目标值域概念节点
                != 服务结算配置_->目标值域概念节点) return 项;
    }
    if (任务.方法实例节点&&!任务.方法执行组织节点) {
        const auto 实例 = 方法服务_.获取混合方法实例(*任务.方法实例节点);
        if (!实例) return 项;
        std::vector<稳定编码> 待查 = 实例->候选方法实例根组;
        std::set<稳定编码> 已查;
        while (!待查.empty()) {
            const auto 方法节点 = 待查.back();
            待查.pop_back();
            if (!已查.insert(方法节点).second) continue;
            const auto 方法 = 方法服务_.获取方法(方法节点);
            if (!方法 || 方法->实例根节点 != 任务.方法实例节点
                || !方法->来源方法节点) return 项;
            for (const auto& 步骤 : 方法->组合步骤组)
                待查.push_back(步骤.调用方法节点);
        }
    }
    if (任务.当前状态 == 新任务状态::执行中
        || 任务.当前状态 == 新任务状态::待影响评估) {
        if (!项.执行) return 项;
        项.状态 = 新任务调度项状态::旧执行待结果;
        项.等待原因 = 新任务等待原因::无;
        return 项;
    }
    if (任务.当前状态 == 新任务状态::等待自我决议
        || 任务.当前状态 == 新任务状态::待执行) {
        if (任务.当前状态 == 新任务状态::等待自我决议
            && (!项.执行 || 项.执行->状态 != 新任务执行状态::已评估
                || !项.影响评估)) return 项;
        项.状态 = 新任务调度项状态::后继待决议;
        项.等待原因 = 新任务等待原因::无;
        return 项;
    }
    if (项.执行 && 项.执行->状态 != 新任务执行状态::已评估) {
        项.状态 = 新任务调度项状态::旧执行待结果;
        项.等待原因 = 新任务等待原因::无;
        return 项;
    }
    新方法候选查询 查询;
    查询.涉及特征类型概念组.push_back(项.需求->目标.目标特征类型概念节点);
    const auto 候选 = 方法服务_.查询候选方法(查询);
    if (候选.状态 != 新方法操作状态::已找到) {
        资源失败 = 候选.状态 == 新方法操作状态::资源失败;
        return 项;
    }
    std::vector<稳定编码> 待查 = 候选.方法节点组;
    std::map<稳定编码, 新方法信息> 闭包;
    while (!待查.empty()) {
        const auto 方法节点 = 待查.back();
        待查.pop_back();
        if (闭包.contains(方法节点)) continue;
        auto 方法 = 方法服务_.获取方法(方法节点);
        if (!方法 || 方法->来源方法节点 || 方法->实例根节点) return 项;
        for (const auto& 步骤 : 方法->组合步骤组) 待查.push_back(步骤.调用方法节点);
        闭包.emplace(方法节点, std::move(*方法));
    }
    项.当前候选方法闭包.reserve(闭包.size());
    for (auto& [节点, 方法] : 闭包) 项.当前候选方法闭包.push_back(std::move(方法));

    if (任务.当前状态 == 新任务状态::待检查条件) {
        // 这里只读核验，绝不进入会覆盖实例条件的选择/筹办入口。
        项.条件核验 = 核验任务条件和参数(任务节点);
        if (项.条件核验->状态 != 新任务治理条件筹办状态::已完成
            || !项.条件核验->候选已完整核验) {
            资源失败 = 项.条件核验->状态 == 新任务治理条件筹办状态::资源失败;
            return 项;
        }
        for (const auto& 方法项 : 项.条件核验->方法筹办组) {
            if (方法项.状态 == 新任务候选方法筹办状态::资源失败) {
                资源失败 = true;
                return 项;
            }
            for (const auto& 配对 : 方法项.条件结果组) {
                if (配对.状态 == 新任务方法条件项核验状态::资源失败) {
                    资源失败 = true;
                    return 项;
                }
            }
        }
    }
    项.状态 = 任务.当前状态 == 新任务状态::等待条件
            || 项.需求->活动状态 != 新需求活动状态::活动
        ? 新任务调度项状态::业务等待 : 新任务调度项状态::普通可筹办;
    项.等待原因 = 项.状态 == 新任务调度项状态::业务等待
        ? 新任务等待原因::未分类 : 新任务等待原因::无;
    // 零候选是当前描述，不根据旧X反推本次等待来源。
    if (项.状态 == 新任务调度项状态::业务等待 && 项.候选材料
        && 项.候选材料->候选方法节点组.empty())
        项.等待原因 = 新任务等待原因::尚无方法;
    return 项;
}

新任务调度材料读取结果 新_任务类::读取任务调度材料() const noexcept {
    新任务调度材料读取结果 结果;
    try {
        std::lock_guard 锁(新任务互斥);
        if (!治理已初始化_ || !结构仍存在()) {
            结果.状态 = 新任务调度读取状态::尚未初始化;
            return 结果;
        }
        const auto 全集 = 读取任务登记全集();
        if (!全集) return 结果;
        结果.项组.reserve(全集->size());
        bool 资源失败 = false;
        for (const auto 任务节点 : *全集) {
            auto 项 = 读取任务调度项已加锁(任务节点, 资源失败);
            if (资源失败) {
                结果.状态 = 新任务调度读取状态::资源失败;
                结果.项组.clear();
                return 结果;
            }
            结果.项组.push_back(std::move(项));
        }
        结果.状态 = 新任务调度读取状态::已读取;
    } catch (...) {
        结果.状态 = 新任务调度读取状态::资源失败;
        结果.项组.clear();
    }
    return 结果;
}

新任务调度材料读取结果 新_任务类::读取任务调度材料(
    稳定编码 任务节点) const noexcept {
    新任务调度材料读取结果 结果;
    try {
        std::lock_guard 锁(新任务互斥);
        if (!治理已初始化_ || !结构仍存在()) {
            结果.状态 = 新任务调度读取状态::尚未初始化;
            return 结果;
        }
        const auto 登记 = 全局基础数据集.查询目标字段(任务节点, 任务成员字段);
        if (!有效(任务节点) || (!节点仍存在(任务节点) && 登记.empty())) {
            结果.状态 = 新任务调度读取状态::任务不存在;
            return 结果;
        }
        if (登记.size() != 1 || 登记.front().所属节点 != 任务集合根节点
            || !节点类型为(任务节点, 任务节点类型)) return 结果;
        bool 资源失败 = false;
        auto 项 = 读取任务调度项已加锁(任务节点, 资源失败);
        if (资源失败) {
            结果.状态 = 新任务调度读取状态::资源失败;
            return 结果;
        }
        结果.项组.push_back(std::move(项));
        结果.状态 = 新任务调度读取状态::已读取;
    } catch (...) {
        结果.状态 = 新任务调度读取状态::资源失败;
        结果.项组.clear();
    }
    return 结果;
}

新任务操作状态 新_任务类::准备等待任务重新筹办(
    const 新任务调度定位& 预期定位) noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        if (!预期定位.完整()) return 新任务操作状态::入口拒绝;
        if (预期定位.任务状态 != 新任务状态::已建立
            && 预期定位.任务状态 != 新任务状态::等待条件
            && 预期定位.任务状态 != 新任务状态::待检查条件)
            return 新任务操作状态::任务状态不允许;
        const auto 读取 = 读取任务调度材料(预期定位.任务节点);
        if (读取.状态 != 新任务调度读取状态::已读取) {
            if (读取.状态 == 新任务调度读取状态::资源失败) return 新任务操作状态::资源失败;
            if (读取.状态 == 新任务调度读取状态::尚未初始化) return 新任务操作状态::尚未初始化;
            if (读取.状态 == 新任务调度读取状态::任务不存在) return 新任务操作状态::任务不存在;
            return 新任务操作状态::结构不一致;
        }
        if (读取.项组.size() != 1) return 新任务操作状态::结构不一致;
        const auto& 项 = 读取.项组.front();
        if (项.状态 == 新任务调度项状态::结构不一致 || !项.任务 || !项.需求)
            return 新任务操作状态::结构不一致;
        auto 迁移后定位 = 预期定位;
        迁移后定位.任务状态 = 新任务状态::待查找方法;
        const bool 已迁移 = 项.定位 == 迁移后定位;
        if (项.定位 != 预期定位 && !已迁移) return 新任务操作状态::任务状态不允许;
        // 同许可交接重试只确认已经生效的迁移，不再次产生筹办或动作许可。
        if (已迁移) return 新任务操作状态::无变化;
        if (项.需求->活动状态 != 新需求活动状态::活动
            || 方法实例采用待核查_.contains(预期定位.任务节点))
            return 新任务操作状态::任务状态不允许;
        if (项.执行 && (项.执行->状态 != 新任务执行状态::已评估
                || !项.执行->结束时间 || !项.影响评估))
            return 新任务操作状态::任务状态不允许;
        const auto 当前 = 需求服务_.复核当前满足(项.定位.需求节点);
        if (当前.状态 == 新需求当前满足状态::资源失败) return 新任务操作状态::资源失败;
        if (项.需求->目标.目标形式 == 新需求目标形式::变化方向) {
            if (当前.状态 != 新需求当前满足状态::需要执行前后比较)
                return 新任务操作状态::任务状态不允许;
            if (项.影响评估
                && 项.影响评估->目标需求结果.状态 == 新需求当前满足状态::已满足)
                return 新任务操作状态::任务状态不允许;
            if (项.执行 && 项.影响评估->目标需求结果.状态 != 新需求当前满足状态::需增加
                && 项.影响评估->目标需求结果.状态 != 新需求当前满足状态::需减少)
                return 新任务操作状态::状态材料不足;
        } else if (当前.状态 != 新需求当前满足状态::需增加
            && 当前.状态 != 新需求当前满足状态::需减少
            && 当前.状态 != 新需求当前满足状态::未满足但无有向差值) {
            return 新任务操作状态::任务状态不允许;
        }
        // 长期结算T必须回到既有来源/冻结值门，不能借普通恢复旁路。
        const bool 是结算任务 = 服务结算配置_
            && 服务结算配置_->结算任务节点 == 预期定位.任务节点;
        if (项.任务->服务结算来源 && !是结算任务) return 新任务操作状态::结构不一致;
        const auto 写入 = 是结算任务
            ? 准备服务结算筹办(预期定位.任务节点)
            : 修改任务状态(预期定位.任务节点, 新任务状态::待查找方法);
        if (写入 != 新任务操作状态::已修改 && 写入 != 新任务操作状态::无变化) return 写入;
        const auto 读回 = 获取任务(预期定位.任务节点);
        auto 预期任务 = *项.任务;
        预期任务.当前状态 = 新任务状态::待查找方法;
        return 读回 && *读回 == 预期任务 ? 写入 : 新任务操作状态::结构不一致;
    } catch (...) { return 新任务操作状态::资源失败; }
}

新任务列表结果 新_任务类::查询受方法学习影响任务(稳定编码 方法节点) const noexcept {
    新任务列表结果 结果;
    try {
        std::lock_guard 锁(新任务互斥);
        const auto 方法 = 方法服务_.获取方法(方法节点);
        if (!方法 || 方法->来源方法节点 || 方法->实例根节点
            || 方法->启用状态 != 新方法启用状态::启用 || 方法->条件结果组.empty()) {
            结果.状态 = 新任务操作状态::方法不存在; return 结果;
        }
        const auto 全部 = 查询全部任务();
        if (全部.状态 != 新任务操作状态::已找到) return 全部;
        for (const auto 节点 : 全部.任务节点组) {
            const auto t = 获取任务(节点);
            if (!t) { 结果.状态 = 新任务操作状态::结构不一致; 结果.任务节点组.clear(); return 结果; }
            if (t->当前状态 != 新任务状态::已建立 && t->当前状态 != 新任务状态::待查找方法
                && t->当前状态 != 新任务状态::待检查条件 && t->当前状态 != 新任务状态::等待条件) continue;
            if (方法实例采用待核查_.contains(节点)) continue;
            if (t->最近执行实例节点) {
                const auto x = 获取执行实例(*t->最近执行实例节点);
                if (!x || x->任务节点 != 节点) { 结果.状态 = 新任务操作状态::结构不一致; 结果.任务节点组.clear(); return 结果; }
                if (x->状态 != 新任务执行状态::已评估) continue;
            }
            const auto d = 需求服务_.获取需求(t->需求节点);
            if (!d) { 结果.状态 = 新任务操作状态::结构不一致; 结果.任务节点组.clear(); return 结果; }
            if (d->活动状态 != 新需求活动状态::活动) continue;
            auto 祖先 = d;
            std::set<稳定编码> 已见;
            while (祖先) {
                if (!已见.insert(祖先->节点).second) break;
                if (!祖先->父需求节点) break;
                if (祖先->根角色) { 祖先.reset(); break; }
                祖先 = 需求服务_.获取需求(*祖先->父需求节点);
            }
            if (!祖先 || 祖先->父需求节点 || !祖先->根角色
                || 需求服务_.查询根需求(*祖先->根角色) != 祖先->节点) {
                结果.状态 = 新任务操作状态::结构不一致; 结果.任务节点组.clear(); return 结果;
            }
            bool 相关 = false;
            const auto 检查类型 = [&](稳定编码 类型) {
                const auto 根 = 取得特征概念类型根(特征概念服务_, 类型);
                return 根 && *根 == d->目标.目标特征类型概念节点;
            };
            for (const auto 类型 : 方法->方法头.涉及特征类型概念组) 相关 = 相关 || 检查类型(类型);
            for (const auto& 对 : 方法->条件结果组) for (const auto& r : 对.结果.结果项组)
                if (r.特征类型概念节点) 相关 = 相关 || 检查类型(*r.特征类型概念节点);
            if (t->当前候选方法材料节点) {
                const auto 材料 = 获取候选方法材料(*t->当前候选方法材料节点);
                if (!材料 || 材料->任务节点 != 节点 || 材料->需求节点 != d->节点) {
                    结果.状态 = 新任务操作状态::结构不一致; 结果.任务节点组.clear(); return 结果;
                }
                相关 = 相关 || std::find(材料->候选方法节点组.begin(), 材料->候选方法节点组.end(), 方法节点)
                    != 材料->候选方法节点组.end();
            }
            if(t->方法执行组织节点){
                const auto 组织=读取任务方法执行组织(节点);if(!组织.组织)throw std::logic_error("学习唤醒组织读取失败");
                for(const auto& 域:组织.组织->方法作用域组)相关=相关||域.来源方法节点==方法节点;
            }else if (t->方法实例节点) {
                const auto 图 = 方法服务_.获取混合方法实例(*t->方法实例节点);
                if (!图) { 结果.状态 = 新任务操作状态::结构不一致; 结果.任务节点组.clear(); return 结果; }
                std::vector<稳定编码> 待查 = 图->候选方法实例根组;
                std::set<稳定编码> 已查;
                while (!待查.empty()) {
                    const auto m = 待查.back(); 待查.pop_back();
                    if (!已查.insert(m).second) continue;
                    const auto 副本 = 方法服务_.获取方法(m);
                    if (!副本 || 副本->实例根节点 != t->方法实例节点 || !副本->来源方法节点) {
                        结果.状态 = 新任务操作状态::结构不一致; 结果.任务节点组.clear(); return 结果;
                    }
                    相关 = 相关 || 副本->来源方法节点 == 方法节点;
                    for (const auto& 步 : 副本->组合步骤组) 待查.push_back(步.调用方法节点);
                }
            }
            if (相关) 结果.任务节点组.push_back(节点);
        }
        结果.状态 = 新任务操作状态::已找到;
    } catch (...) { 结果.状态 = 新任务操作状态::资源失败; 结果.任务节点组.clear(); }
    return 结果;
}

新任务操作状态 新_任务类::准备学习后重新筹办(稳定编码 任务节点, 稳定编码 方法节点) noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        const auto 相关 = 查询受方法学习影响任务(方法节点);
        if (相关.状态 != 新任务操作状态::已找到) return 相关.状态;
        if (std::find(相关.任务节点组.begin(), 相关.任务节点组.end(), 任务节点) == 相关.任务节点组.end())
            return 新任务操作状态::任务状态不允许;
        const auto 写入 = 修改任务状态(任务节点, 新任务状态::待查找方法);
        if (写入 != 新任务操作状态::已修改 && 写入 != 新任务操作状态::无变化) return 写入;
        const auto 读回 = 获取任务(任务节点);
        return 读回 && 读回->当前状态 == 新任务状态::待查找方法 ? 写入 : 新任务操作状态::结构不一致;
    } catch (...) { return 新任务操作状态::资源失败; }
}

新任务列表结果 新_任务类::查询活动任务() const noexcept {
    auto 结果 = 查询全部任务();
    if (结果.状态 != 新任务操作状态::已找到) return 结果;
    try {
        std::lock_guard 锁(新任务互斥);
        std::erase_if(结果.任务节点组, [&](稳定编码 任务) {
            const auto 信息 = 读取任务信息(任务, 特征服务_);
            return !信息 || 是终结状态(信息->当前状态);
        });
        return 结果;
    } catch (...) {
        结果.状态 = 新任务操作状态::资源失败;
        结果.任务节点组.clear();
        return 结果;
    }
}

新任务列表结果 新_任务类::查询指定状态任务(
    新任务状态 状态) const noexcept {
    新任务列表结果 结果;
    if (!是任务状态值(static_cast<std::int64_t>(状态))) return 结果;
    结果 = 查询全部任务();
    if (结果.状态 != 新任务操作状态::已找到) return 结果;
    try {
        std::lock_guard 锁(新任务互斥);
        std::erase_if(结果.任务节点组, [&](稳定编码 任务) {
            const auto 信息 = 读取任务信息(任务, 特征服务_);
            return !信息 || 信息->当前状态 != 状态;
        });
        return 结果;
    } catch (...) {
        结果.状态 = 新任务操作状态::资源失败;
        结果.任务节点组.clear();
        return 结果;
    }
}

新任务操作状态 新_任务类::修改任务状态(
    稳定编码 任务节点,
    新任务状态 新状态) noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        if (!结构仍存在()) return 新任务操作状态::尚未初始化;
        if (!是任务状态值(static_cast<std::int64_t>(新状态))) {
            return 新任务操作状态::入口拒绝;
        }
        const auto 虚拟存在 = 读取任务虚拟存在信息(任务节点, 特征服务_);
        if (!虚拟存在) return 新任务操作状态::任务不存在;
        return 写入任务状态(*虚拟存在, 新状态, 特征服务_);
    } catch (...) {
        return 新任务操作状态::资源失败;
    }
}

bool 新_任务类::装配本能动作读取(新_动态类& 动态服务) noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        if (本能动作读取_) return 本能动作读取_ == &动态服务;
        本能动作读取_ = &动态服务;
        return true;
    } catch (...) { return false; }
}

std::optional<稳定编码> 新_任务类::获取任务已结算动作定义() const noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        if (!结构仍存在()) return std::nullopt;
        const auto 定义 = 读取唯一节点字段(任务集合根节点, 任务内部动作定义字段);
        if (!定义 || !节点类型为(*定义, 任务已结算动作定义类型)) return std::nullopt;
        const auto 操作 = 读取唯一I64字段记录(*定义, 内部动作操作码字段);
        return 操作 && 操作->值 == 1 ? 定义 : std::nullopt;
    } catch (...) { return std::nullopt; }
}

新任务操作状态 新_任务类::配置服务结算任务(
    const 新任务服务结算配置& 配置) noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        if (!结构仍存在()) return 新任务操作状态::尚未初始化;
        if (配置.冻结前值 < 0) return 新任务操作状态::入口拒绝;
        const auto 任务 = 获取任务(配置.结算任务节点);
        const auto 需求 = 需求服务_.获取需求(配置.结算需求);
        const auto 根 = 需求服务_.获取需求(配置.服务根需求);
        const auto 方法 = 方法服务_.获取方法(配置.登记方法校验节点);
        const auto 域 = 特征概念服务_.获取特征概念(配置.目标值域概念节点);
        const auto* 区间 = 域 ? std::get_if<std::vector<特征概念I64闭区间>>(&域->值域) : nullptr;
        constexpr auto 最大值 = (std::numeric_limits<std::int64_t>::max)();
        const auto 目标值 = 配置.冻结前值 == 最大值 ? 最大值 : 配置.冻结前值 + 1;
        if (!任务 || !需求 || !根 || !方法 || !域 || !区间 || 区间->size() != 1
            || 区间->front().下界 != 目标值 || 区间->front().上界 != 目标值
            || 域->材料类型 != 特征概念材料物理类型::I64标量
            || 任务->需求节点 != 配置.结算需求
            || 需求->父需求节点 != 配置.服务根需求
            || 根->根角色 != 新需求根角色::服务
            || 根->目标.目标形式 != 新需求目标形式::变化方向
            || 根->目标.目标变化方向 != 新需求变化方向::增加
            || 需求->目标存在节点 != 根->目标存在节点
            || 需求->目标.目标特征类型概念节点 != 根->目标.目标特征类型概念节点
            || 需求->目标.目标形式 != 新需求目标形式::值
            || 需求->目标.目标值域概念节点 != 配置.目标值域概念节点
            || 取得特征概念类型根(特征概念服务_, 配置.目标值域概念节点)
                != 需求->目标.目标特征类型概念节点
            || 方法->作用域 != 新方法作用域::普通世界
            || 方法->实现方式 != 新方法实现方式::本能
            || 方法->本能函数登记名 != std::optional<std::string>{"服务值结算"}
            || std::find(方法->方法头.涉及特征类型概念组.begin(),
                方法->方法头.涉及特征类型概念组.end(), 需求->目标.目标特征类型概念节点)
                == 方法->方法头.涉及特征类型概念组.end()) return 新任务操作状态::结构不一致;
        if (服务结算配置_)
            return *服务结算配置_ == 配置 ? 新任务操作状态::精确重复 : 新任务操作状态::结构不一致;
        if (任务->当前状态 != 新任务状态::已建立 && 任务->当前状态 != 新任务状态::等待条件)
            return 新任务操作状态::任务状态不允许;
        // 仅保存不可变装配定位；不发布候选或最近方法，采用仍由W1/W2/self决定。
        服务结算配置_ = 配置;
        return 新任务操作状态::已修改;
    } catch (...) { return 新任务操作状态::资源失败; }
}

std::optional<新任务服务结算来源绑定> 新_任务类::读取服务结算来源(
    稳定编码 结算任务节点) const noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        const auto 任务 = 获取任务(结算任务节点);
        return 任务 ? 任务->服务结算来源 : std::nullopt;
    } catch (...) { return std::nullopt; }
}

新任务操作状态 新_任务类::接纳服务结算来源(
    稳定编码 结算任务节点, const 新任务服务结算来源绑定& 绑定) noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        if (!服务结算配置_ || 服务结算配置_->结算任务节点 != 结算任务节点
            || 结算任务节点 == 绑定.来源任务.任务节点
            || 绑定.冻结前值 != 服务结算配置_->冻结前值
            || 绑定.目标值域概念节点 != 服务结算配置_->目标值域概念节点)
            return 新任务操作状态::入口拒绝;
        const auto 任务 = 获取任务(结算任务节点);
        if (!任务) return 新任务操作状态::任务不存在;
        if (任务->服务结算来源)
            return *任务->服务结算来源 == 绑定 ? 新任务操作状态::精确重复 : 新任务操作状态::结构不一致;
        if (任务->当前状态 != 新任务状态::已建立 && 任务->当前状态 != 新任务状态::等待条件
            && 任务->当前状态 != 新任务状态::待查找方法 && 任务->当前状态 != 新任务状态::待检查条件)
            return 新任务操作状态::任务状态不允许;
        const auto 来源 = 读取任务虚拟存在引用(绑定.来源任务);
        if (!来源 || 来源->当前状态 != static_cast<std::int64_t>(新任务状态::已完成))
            return 新任务操作状态::任务状态不允许;
        const auto 当前 = 需求服务_.复核当前满足(任务->需求节点);
        if (当前.状态 != 新需求当前满足状态::需增加 || !当前.实际特征节点)
            return 当前.状态 == 新需求当前满足状态::资源失败
                ? 新任务操作状态::资源失败 : 新任务操作状态::任务状态不允许;
        const auto 特征 = 特征服务_.获取特征(*当前.实际特征节点);
        if (!特征 || 特征->当前值 != std::optional<新特征准确值>{新特征准确值{绑定.冻结前值}})
            return 新任务操作状态::结构不一致;
        std::vector<稳定编码> 已写;
        已写.reserve(5);
        if (!写入服务结算来源字段(结算任务节点, 绑定, 已写)) {
            回滚字段(已写);
            return 新任务操作状态::资源失败;
        }
        std::optional<新任务服务结算来源绑定> 读回;
        if (!读取服务结算来源字段(结算任务节点, 读回) || 读回 != 绑定) {
            回滚字段(已写);
            return 新任务操作状态::结构不一致;
        }
        return 新任务操作状态::已修改;
    } catch (...) { return 新任务操作状态::资源失败; }
}

新任务操作状态 新_任务类::准备服务结算筹办(稳定编码 结算任务节点) noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        if (!治理已初始化_ || !服务结算配置_
            || 服务结算配置_->结算任务节点 != 结算任务节点) return 新任务操作状态::入口拒绝;
        const auto 任务 = 获取任务(结算任务节点);
        if (!任务) return 新任务操作状态::任务不存在;
        if (任务->当前状态 != 新任务状态::已建立 && 任务->当前状态 != 新任务状态::等待条件
            && 任务->当前状态 != 新任务状态::待查找方法 && 任务->当前状态 != 新任务状态::待检查条件)
            return 新任务操作状态::任务状态不允许;
        // 来源是执行条件，不是查找方法和筹办的前置许可。
        if (任务->服务结算来源) {
            const auto 来源 = 读取任务虚拟存在引用(任务->服务结算来源->来源任务);
            if (!来源 || 来源->当前状态 != static_cast<std::int64_t>(新任务状态::已完成))
                return 新任务操作状态::任务状态不允许;
        }
        const auto 当前 = 需求服务_.复核当前满足(任务->需求节点);
        const auto 特征 = 当前.实际特征节点 ? 特征服务_.获取特征(*当前.实际特征节点) : std::nullopt;
        if (当前.状态 != 新需求当前满足状态::需增加 || !特征
            || 特征->当前值 != std::optional<新特征准确值>{新特征准确值{服务结算配置_->冻结前值}})
            return 新任务操作状态::任务状态不允许;
        if (任务->当前状态 == 新任务状态::待查找方法) return 新任务操作状态::无变化;
        if (任务->当前状态 == 新任务状态::已建立) return 准备任务查找方法(结算任务节点);
        return 修改任务状态(结算任务节点, 新任务状态::待查找方法);
    } catch (...) { return 新任务操作状态::资源失败; }
}

std::optional<稳定编码> 新_任务类::获取任务状态特征类型() const noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        if (!结构仍存在()
            || !特征概念服务_.获取特征概念(任务状态特征概念)) return std::nullopt;
        return 任务状态特征概念;
    } catch (...) { return std::nullopt; }
}

std::optional<新任务虚拟存在读回> 新_任务类::读取任务虚拟存在引用(
    const 新任务虚拟存在引用& 引用) const noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        const auto 任务 = 读取任务信息(引用.任务节点, 特征服务_);
        const auto 虚拟 = 读取任务虚拟存在信息(引用.任务节点, 特征服务_);
        if (!任务 || !虚拟 || 虚拟->节点 != 引用.虚拟存在节点
            || 虚拟->任务状态特征节点 != 引用.状态特征节点
            || 任务->任务虚拟存在节点 != 引用.虚拟存在节点) return std::nullopt;
        const auto 特征 = 特征服务_.获取特征(引用.状态特征节点);
        if (!特征 || 取得特征概念类型根(特征概念服务_, 特征->特征概念节点)
                != 任务状态特征概念) return std::nullopt;
        return 新任务虚拟存在读回{引用, 任务状态特征概念,
            static_cast<std::int64_t>(虚拟->当前状态)};
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取任务虚拟存在引用", L"任务虚拟存在读取异常。");
    }
}

bool 新_任务类::装配内部治理世界(新任务治理世界协作接口& 世界) noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        if (内部治理世界_) return 内部治理世界_ == &世界;
        if (已开始过执行_) return false;
        内部治理世界_ = &世界;
        return true;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"装配内部治理世界", L"任务治理协作接口装配异常。");
    }
}

void 新_任务类::确保任务虚拟存在语义(稳定编码 任务节点) noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        const auto 虚拟 = 读取任务虚拟存在信息(任务节点, 特征服务_);
        if (!虚拟) throw std::runtime_error("task virtual existence missing");
        const auto 特征 = 特征服务_.获取特征(虚拟->任务状态特征节点);
        if (!特征) throw std::runtime_error("task state feature missing");
        const auto 字段 = 全局基础数据集.查询字段(虚拟->节点, 虚拟存在专属概念字段);
        if (字段.empty()) {
            const auto 建立 = 存在概念服务_.建立专属存在概念(虚拟->节点, {特征->特征概念节点});
            if (建立.状态 != 存在概念操作状态::已建立 || !建立.概念节点)
                throw std::runtime_error("task concept creation failed");
            if (!有效(全局基础数据集.添加字段节点(虚拟->节点, 虚拟存在专属概念字段, *建立.概念节点))) {
                // 仅撤销本次尚未绑定的专属概念；不能破坏已有治理引用。
                const auto 绑定 = 全局基础数据集.查询字段(虚拟->节点, 虚拟存在专属概念字段);
                if (绑定.empty() && !存在概念服务_.撤销未绑定专属存在概念(*建立.概念节点, 虚拟->节点))
                    throw std::runtime_error("unbound task concept rollback failed");
                throw std::runtime_error("task concept binding failed");
            }
        } else {
            const auto 概念节点 = 读取唯一节点字段(虚拟->节点, 虚拟存在专属概念字段);
            const auto 概念 = 概念节点 ? 存在概念服务_.获取存在概念(*概念节点) : std::nullopt;
            if (!概念 || 概念->种类 != 存在概念种类::专属 || 概念->对应存在节点 != 虚拟->节点)
                throw std::runtime_error("task concept identity mismatch");
            const auto 更新 = 存在概念服务_.更新专属存在概念(*概念节点, {特征->特征概念节点});
            if (更新 != 存在概念操作状态::已更新 && 更新 != 存在概念操作状态::无变化)
                throw std::runtime_error("task concept update failed");
        }
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"确保任务虚拟存在语义", L"任务专属概念发布或读回异常。");
    }
}

std::optional<新任务虚拟存在语义读回> 新_任务类::读取任务虚拟存在语义(
    const 新任务虚拟存在引用& 引用) const noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        const auto 状态 = 读取任务虚拟存在引用(引用);
        if (!状态) return std::nullopt;
        const auto 字段 = 全局基础数据集.查询字段(引用.虚拟存在节点, 虚拟存在专属概念字段);
        if (字段.empty()) return std::nullopt;
        const auto 概念节点 = 读取唯一节点字段(引用.虚拟存在节点, 虚拟存在专属概念字段);
        const auto 概念 = 概念节点 ? 存在概念服务_.获取存在概念(*概念节点) : std::nullopt;
        const auto 特征 = 特征服务_.获取特征(引用.状态特征节点);
        if (!概念 || !特征 || 概念->种类 != 存在概念种类::专属
            || 概念->对应存在节点 != 引用.虚拟存在节点 || 概念->特征组.size() != 1
            || 概念->特征组.front().具体特征概念 != 特征->特征概念节点)
            throw std::runtime_error("task semantic projection is inconsistent");
        return 新任务虚拟存在语义读回{*状态, *概念节点};
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取任务虚拟存在语义", L"已登记任务语义结构读取异常。");
    }
}

新任务已结算结果 新_任务类::标记任务已结算(
    const 新任务已结算请求& 请求, 新_动态类& 动态服务) noexcept {
    新任务已结算结果 结果;
    try {
        if (!有效(请求.来源任务节点) || !有效(请求.执行主体节点)
            || !节点仍存在(请求.内部动作定义节点) || !强时间有效(请求.内部动作时间)
            || !存在服务_.是存在节点(请求.执行主体节点)
            || !场景服务_.存在属于场景范围(请求.现实来源场景节点, 请求.执行主体节点)) return 结果;
        const auto 普通动作 = 动态服务.获取动态(请求.已确认普通动作节点);
        if (!普通动作 || 普通动作->种类 != 新动态种类::动作
            || 普通动作->动作目标种类 != 新动作目标种类::普通存在
            || 普通动作->动作来源节点 != 请求.来源任务节点
            || 普通动作->主题存在节点 != 请求.执行主体节点 || !普通动作->动作时间
            || 普通动作->动作时间->纳秒 >= 请求.内部动作时间.纳秒) return 结果;

        // task为状态唯一写入者；下层不得在自己的锁内反调task。
        std::lock_guard 锁(新任务互斥);
        if (!内部治理世界_) throw std::runtime_error("governance world is not assembled");
        if (获取任务已结算动作定义() != 请求.内部动作定义节点) return 结果;
        const auto 虚拟 = 读取任务虚拟存在信息(请求.来源任务节点, 特征服务_);
        if (!结构仍存在() || !虚拟) throw std::runtime_error("settlement task structure missing");
        const 新任务虚拟存在引用 目标{请求.来源任务节点, 虚拟->节点, 虚拟->任务状态特征节点};
        结果.任务读回 = 读取任务虚拟存在引用(目标);
        if (!结果.任务读回) throw std::runtime_error("settlement target readback failed");
        const auto 既有 = 动态服务.按来源查询动作(请求.已确认普通动作节点);
        if (既有.状态 != 新动态操作状态::已完成) throw std::runtime_error("action source lookup failed");
        if (虚拟->当前状态 == 新任务状态::已结算) {
            结果.治理变化 = 内部治理世界_->读取任务状态变化(目标, 请求.已确认普通动作节点);
            if (!结果.治理变化 || !既有.动作
                || 既有.动作->主题存在节点 != 请求.执行主体节点
                || 既有.动作->动作定义节点 != 请求.内部动作定义节点
                || 既有.动作->动作时间 != 请求.内部动作时间)
                throw std::runtime_error("settled task has incomplete governance evidence");
            结果.内部动作节点 = 结果.治理变化->动作节点;
            结果.状态 = 新任务已结算状态::已是已结算;
            return 结果;
        }
        if (既有.动作) throw std::runtime_error("partial governance action must not be replayed");
        if (虚拟->当前状态 != 新任务状态::已完成) {
            结果.状态 = 新任务已结算状态::来源任务不具备;
            return 结果;
        }
        确保任务虚拟存在语义(请求.来源任务节点);
        const auto 前 = 状态服务_.建立状态(目标.虚拟存在节点, 目标.状态特征节点, 请求.内部动作时间);
        if (前.状态 != 新状态操作状态::已完成 || !前.状态节点)
            throw std::runtime_error("governance before-state publication failed");
        const auto 动作 = 内部治理世界_->建立任务状态动作({目标, 请求.执行主体节点,
            请求.现实来源场景节点, 请求.内部动作定义节点, 请求.已确认普通动作节点,
            *前.状态节点, 请求.内部动作时间});
        if (动作.状态 != 新治理动作登记状态::已建立 || !动作.动作节点)
            throw std::runtime_error("governance action publication failed");
        结果.内部动作节点 = 动作.动作节点;
        (void)写入已核验任务状态值(*虚拟, 新任务状态::已结算, 特征服务_);
        结果.任务读回 = 读取任务虚拟存在引用(目标);
        if (!结果.任务读回 || 结果.任务读回->当前状态 != static_cast<std::int64_t>(新任务状态::已结算))
            throw std::runtime_error("settled task state write/readback failed");
        const auto 后时间 = 当前实例UTC强时间();
        if (!后时间 || 后时间->纳秒 <= 请求.内部动作时间.纳秒)
            throw std::runtime_error("governance result clock must advance");
        const auto 后 = 状态服务_.建立状态(目标.虚拟存在节点, 目标.状态特征节点, *后时间);
        if (后.状态 != 新状态操作状态::已完成 || !后.状态节点)
            throw std::runtime_error("governance after-state publication failed");
        确保任务虚拟存在语义(请求.来源任务节点);
        结果.治理变化 = 内部治理世界_->完成任务状态变化(*动作.动作节点, *前.状态节点, *后.状态节点);
        结果.状态 = 新任务已结算状态::已标记;
        return 结果;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"标记任务已结算",
            L"内部结算动作或状态发生工程异常；保留已发生材料，禁止自动重放。");
    }
}

新任务操作状态 新_任务类::重新启用任务(
    稳定编码 任务节点) noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        if (!结构仍存在()) return 新任务操作状态::尚未初始化;
        const auto 虚拟存在 = 读取任务虚拟存在信息(任务节点, 特征服务_);
        if (!虚拟存在) return 新任务操作状态::任务不存在;
        if (!是终结状态(虚拟存在->当前状态)
            || 虚拟存在->当前状态 == 新任务状态::已结算) {
            return 新任务操作状态::任务状态不允许;
        }

        const auto 清除候选 = 清除候选方法材料(任务节点);
        if (清除候选 != 新任务操作状态::已修改
            && 清除候选 != 新任务操作状态::无变化) return 清除候选;

        // 重新启用是终结任务的专用入口，不能经通用迁移谓词放宽。
        const auto 写入 = 特征服务_.添加特征值(
            虚拟存在->任务状态特征节点,
            新特征准确值{static_cast<std::int64_t>(新任务状态::待查找方法)});
        if (写入.状态 == 新特征写入状态::已完成) {
            return 新任务操作状态::已修改;
        }
        if (写入.状态 == 新特征写入状态::无变化) {
            return 新任务操作状态::无变化;
        }
        if (写入.状态 == 新特征写入状态::结构不一致
            || 写入.状态 == 新特征写入状态::值已更新但聚合未完成) {
            const auto 已写状态 = 读取任务状态(
                虚拟存在->任务状态特征节点, 特征服务_);
            return 已写状态 && *已写状态 == 新任务状态::待查找方法
                ? 新任务操作状态::已修改
                : 新任务操作状态::结构不一致;
        }
        return 新任务操作状态::资源失败;
    } catch (...) {
        return 新任务操作状态::资源失败;
    }
}

新任务候选方法发布结果 新_任务类::发布候选方法材料(
    新任务候选方法发布请求 请求) noexcept {
    新任务候选方法发布结果 结果;
    try {
        std::lock_guard 锁(新任务互斥);
        if (!结构仍存在()) {
            结果.状态 = 新任务操作状态::尚未初始化;
            return 结果;
        }
        if (!收口待回收候选材料(请求.任务节点)) {
            结果.状态 = 新任务操作状态::结构不一致;
            return 结果;
        }
        排序去重(请求.查询.作用存在概念组);
        排序去重(请求.查询.涉及特征类型概念组);
        排序去重(请求.查询.可能结果概念组);
        排序去重(请求.候选方法节点组);
        const auto 任务 = 读取任务信息(请求.任务节点, 特征服务_);
        if (!任务) {
            结果.状态 = 新任务操作状态::任务不存在;
            return 结果;
        }
        if (任务->当前状态 != 新任务状态::待查找方法) {
            结果.状态 = 新任务操作状态::任务状态不允许;
            return 结果;
        }
        if (任务->需求节点 != 请求.需求节点) {
            结果.状态 = 新任务操作状态::结构不一致;
            return 结果;
        }
        const auto 需求 = 需求服务_.获取需求(请求.需求节点);
        if (!需求 || 需求->目标 != 请求.需求目标) {
            结果.状态 = 需求 ? 新任务操作状态::结构不一致
                              : 新任务操作状态::需求不存在;
            return 结果;
        }
        if (请求.查询状态 != 新方法操作状态::已找到 || !请求.查询完整
            || 请求.查询.作用域 || 请求.查询.实现方式
            || !请求.查询.作用存在概念组.empty()
            || !请求.查询.可能结果概念组.empty()
            || 请求.查询.涉及特征类型概念组.size() != 1
            || 请求.查询.涉及特征类型概念组.front()
                != 请求.需求目标.目标特征类型概念节点) return 结果;

        const auto 权威候选 = 方法服务_.查询候选方法(请求.查询);
        if (权威候选.状态 != 新方法操作状态::已找到) {
            结果.状态 = 权威候选.状态 == 新方法操作状态::资源失败
                ? 新任务操作状态::资源失败
                : 新任务操作状态::结构不一致;
            return 结果;
        }
        if (权威候选.方法节点组 != 请求.候选方法节点组) {
            结果.状态 = 新任务操作状态::结构不一致;
            return 结果;
        }

        std::vector<稳定编码> 必须存在{
            请求.需求目标.目标特征类型概念节点};
        if (请求.需求目标.目标值域概念节点) {
            必须存在.push_back(*请求.需求目标.目标值域概念节点);
        }
        if (请求.需求目标.目标比较关系概念节点) {
            必须存在.push_back(*请求.需求目标.目标比较关系概念节点);
        }
        必须存在.insert(必须存在.end(), 请求.查询.作用存在概念组.begin(),
            请求.查询.作用存在概念组.end());
        必须存在.insert(必须存在.end(), 请求.查询.涉及特征类型概念组.begin(),
            请求.查询.涉及特征类型概念组.end());
        必须存在.insert(必须存在.end(), 请求.查询.可能结果概念组.begin(),
            请求.查询.可能结果概念组.end());
        必须存在.insert(必须存在.end(), 请求.候选方法节点组.begin(),
            请求.候选方法节点组.end());
        if (std::any_of(必须存在.begin(), 必须存在.end(),
            [](稳定编码 节点) { return !节点仍存在(节点); })) return 结果;

        std::optional<新任务候选方法材料> 旧材料;
        if (任务->当前候选方法材料节点) {
            旧材料 = 读取候选方法材料信息(*任务->当前候选方法材料节点);
            if (!旧材料) {
                结果.状态 = 新任务操作状态::结构不一致;
                return 结果;
            }
            if (候选材料内容相同(*旧材料, 请求)) {
                结果.状态 = 新任务操作状态::精确重复;
                结果.材料节点 = 旧材料->节点;
                return 结果;
            }
        }

        const auto 材料节点 = 全局基础数据集.新建节点();
        if (!有效(材料节点)) {
            结果.状态 = 新任务操作状态::资源失败;
            return 结果;
        }
        std::vector<稳定编码> 已写字段;
        bool 完成 = 写入节点字段(材料节点, 节点类型字段,
                任务候选方法材料节点类型, 已写字段)
            && 写入节点字段(材料节点, 候选所属任务字段,
                请求.任务节点, 已写字段)
            && 写入节点字段(材料节点, 候选需求字段,
                请求.需求节点, 已写字段)
            && 写入节点字段(材料节点, 候选目标特征类型字段,
                请求.需求目标.目标特征类型概念节点, 已写字段)
            && 写入I64字段(材料节点, 候选目标形式字段,
                static_cast<std::int64_t>(请求.需求目标.目标形式),
                已写字段)
            && 写入I64字段(材料节点, 候选查询状态字段,
                static_cast<std::int64_t>(请求.查询状态), 已写字段)
            && 写入I64字段(材料节点, 候选查询完整字段, 1, 已写字段);
        if (完成 && 请求.需求目标.目标值域概念节点) {
            完成 = 写入节点字段(材料节点, 候选目标值域字段,
                *请求.需求目标.目标值域概念节点, 已写字段);
        }
        if (完成 && 请求.需求目标.目标比较关系概念节点) {
            完成 = 写入节点字段(材料节点, 候选目标比较关系字段,
                *请求.需求目标.目标比较关系概念节点, 已写字段);
        }
        if (完成 && 请求.需求目标.目标变化方向) {
            完成 = 写入I64字段(材料节点, 候选目标变化方向字段,
                static_cast<std::int64_t>(
                    *请求.需求目标.目标变化方向), 已写字段);
        }
        if (完成 && 请求.查询.作用域) {
            完成 = 写入I64字段(材料节点, 候选查询作用域字段,
                static_cast<std::int64_t>(*请求.查询.作用域), 已写字段);
        }
        if (完成 && 请求.查询.实现方式) {
            完成 = 写入I64字段(材料节点, 候选查询实现方式字段,
                static_cast<std::int64_t>(*请求.查询.实现方式), 已写字段);
        }
        const auto 写入组 = [&](稳定编码 字段,
            const std::vector<稳定编码>& 节点组) {
            for (const auto 节点 : 节点组) {
                if (!写入节点字段(材料节点, 字段, 节点, 已写字段)) return false;
            }
            return true;
        };
        if (完成) 完成 = 写入组(候选查询作用存在字段,
            请求.查询.作用存在概念组);
        if (完成) 完成 = 写入组(候选查询特征类型字段,
            请求.查询.涉及特征类型概念组);
        if (完成) 完成 = 写入组(候选查询可能结果字段,
            请求.查询.可能结果概念组);
        if (完成) 完成 = 写入组(候选方法字段, 请求.候选方法节点组);
        if (!完成 || !读取候选方法材料信息(材料节点)) {
            回滚字段(已写字段);
            (void)全局基础数据集.删除节点(材料节点);
            结果.状态 = 完成 ? 新任务操作状态::结构不一致
                             : 新任务操作状态::资源失败;
            return 结果;
        }

        const auto 当前字段组 = 全局基础数据集.查询字段(
            请求.任务节点, 任务候选方法材料字段);
        if (当前字段组.size() > 1) {
            回滚字段(已写字段);
            (void)全局基础数据集.删除节点(材料节点);
            结果.状态 = 新任务操作状态::结构不一致;
            return 结果;
        }
        if (旧材料
            && !登记待回收候选材料(请求.任务节点, 旧材料->节点)) {
            回滚字段(已写字段);
            (void)全局基础数据集.删除节点(材料节点);
            结果.状态 = 新任务操作状态::结构不一致;
            return 结果;
        }
        bool 已发布 = false;
        if (当前字段组.empty()) {
            已发布 = 有效(全局基础数据集.添加字段节点(
                请求.任务节点, 任务候选方法材料字段, 材料节点));
        } else {
            已发布 = 全局基础数据集.修改字段节点(
                当前字段组.front().编码, 材料节点);
        }
        if (!已发布) {
            if (旧材料) (void)收口待回收候选材料(请求.任务节点);
            回滚字段(已写字段);
            (void)全局基础数据集.删除节点(材料节点);
            结果.状态 = 新任务操作状态::资源失败;
            return 结果;
        }
        const auto 读回 = 读取任务信息(请求.任务节点, 特征服务_);
        if (!读回 || 读回->当前候选方法材料节点 != 材料节点) {
            结果.状态 = 新任务操作状态::结构不一致;
            结果.材料节点 = 材料节点;
            return 结果;
        }
        if (旧材料 && !收口待回收候选材料(请求.任务节点)) {
            结果.状态 = 新任务操作状态::结构不一致;
            结果.材料节点 = 材料节点;
            return 结果;
        }
        结果.状态 = 旧材料 ? 新任务操作状态::已修改
                            : 新任务操作状态::已建立;
        结果.材料节点 = 材料节点;
        return 结果;
    } catch (...) {
        结果.状态 = 新任务操作状态::资源失败;
        return 结果;
    }
}

新任务操作状态 新_任务类::清除候选方法材料(
    稳定编码 任务节点) noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        if (!收口待回收候选材料(任务节点)) {
            return 新任务操作状态::结构不一致;
        }
        const auto 任务 = 读取任务信息(任务节点, 特征服务_);
        if (!任务) return 新任务操作状态::任务不存在;
        if (!任务->当前候选方法材料节点) return 新任务操作状态::无变化;
        const auto 字段组 = 全局基础数据集.查询字段(
            任务节点, 任务候选方法材料字段);
        if (字段组.size() != 1
            || !登记待回收候选材料(
                任务节点, *任务->当前候选方法材料节点)) {
            return 新任务操作状态::结构不一致;
        }
        if (!全局基础数据集.删除字段(字段组.front().编码)) {
            (void)收口待回收候选材料(任务节点);
            return 新任务操作状态::结构不一致;
        }
        return 收口待回收候选材料(任务节点)
            ? 新任务操作状态::已修改 : 新任务操作状态::结构不一致;
    } catch (...) {
        return 新任务操作状态::资源失败;
    }
}

新任务操作状态 新_任务类::绑定方法实例(
    稳定编码 任务节点, 稳定编码 方法实例根节点) noexcept {
    稳定编码 新字段;
    std::unique_lock<std::recursive_mutex> 锁;
    const auto 撤销 = [&]() noexcept {
        return !有效(新字段) || 全局基础数据集.删除字段(新字段);
    };
    try {
        锁 = std::unique_lock<std::recursive_mutex>{新任务互斥};
        if (!有效(任务节点) || !有效(方法实例根节点)) return 新任务操作状态::入口拒绝;
        if (!结构仍存在()) return 新任务操作状态::尚未初始化;
        const auto 任务 = 获取任务(任务节点);
        if (!任务) return 新任务操作状态::任务不存在;
        if (!节点仍存在(方法实例根节点)) return 新任务操作状态::方法不存在;
        if (!方法服务_.获取混合方法实例(方法实例根节点)) {
            return 新任务操作状态::方法实例不合法;
        }
        if (任务->方法实例节点 && 任务->方法实例节点 != 方法实例根节点) {
            return 新任务操作状态::任务已有方法实例;
        }
        const auto 已有引用 = 全局基础数据集.查询目标字段(
            方法实例根节点, 任务方法实例字段);
        if (已有引用.size() > 1) return 新任务操作状态::结构不一致;
        if (!已有引用.empty() && 已有引用.front().所属节点 != 任务节点) {
            return 新任务操作状态::方法实例已被占用;
        }
        if (任务->方法实例节点) return 新任务操作状态::精确重复;
        if (!已有引用.empty()) return 新任务操作状态::结构不一致;
        if (任务->当前状态 != 新任务状态::已建立
            && 任务->当前状态 != 新任务状态::待查找方法
            && 任务->当前状态 != 新任务状态::待检查条件
            && 任务->当前状态 != 新任务状态::等待条件) return 新任务操作状态::任务状态不允许;
        新字段 = 全局基础数据集.添加字段节点(
            任务节点, 任务方法实例字段, 方法实例根节点);
        if (!有效(新字段)) return 新任务操作状态::资源失败;
        const auto 读回 = 获取任务(任务节点);
        if (!读回 || 读回->方法实例节点 != 方法实例根节点) {
            (void)撤销();
            return 新任务操作状态::结构不一致;
        }
        return 新任务操作状态::已修改;
    } catch (...) {
        return 撤销() ? 新任务操作状态::资源失败 : 新任务操作状态::结构不一致;
    }
}

新任务执行结果 新_任务类::开始方法执行(
    const 新任务执行开始请求& 请求) noexcept {
    新任务执行结果 结果;
    try {
        std::lock_guard 锁(新任务互斥);
        if (!结构仍存在()) {
            结果.状态 = 新任务操作状态::尚未初始化;
            return 结果;
        }
        const auto 任务 = 读取任务信息(请求.任务节点, 特征服务_);
        if (!任务) {
            结果.状态 = 新任务操作状态::任务不存在;
            return 结果;
        }
        if (任务->当前状态 != 新任务状态::待执行) {
            结果.状态 = 新任务操作状态::任务状态不允许;
            return 结果;
        }
        const auto 来源守卫 = 需求服务_.复核需求服务要求语义(任务->需求节点);
        if (来源守卫.状态 != 新服务要求执行守卫状态::不涉及
            && 来源守卫.状态 != 新服务要求执行守卫状态::当前有效) {
            结果.状态 = 来源守卫.状态 == 新服务要求执行守卫状态::需重新确认
                ? 新任务操作状态::入口拒绝
                : 来源守卫.状态 == 新服务要求执行守卫状态::资源失败
                    ? 新任务操作状态::资源失败 : 新任务操作状态::结构不一致;
            return 结果;
        }
        const auto 方法 = 方法服务_.获取方法(请求.方法节点);
        if (!方法) {
            结果.状态 = 新任务操作状态::方法不存在;
            return 结果;
        }
        if(!请求.本能采用||请求.本能采用->来源方法节点!=请求.方法节点
            ||方法->来源方法节点||方法->实例根节点||方法->实现方式!=新方法实现方式::本能){结果.状态=新任务操作状态::入口拒绝;return 结果;}
        const auto 采用复核=核验任务条件和参数内部(请求.任务节点,&*请求.本能采用,false);
        if(采用复核.状态!=新任务治理条件筹办状态::已完成||采用复核.方法筹办组.size()!=1
            ||采用复核.方法筹办组.front().状态!=新任务候选方法筹办状态::条件具备){结果.状态=新任务操作状态::入口拒绝;return 结果;}
        if (!强时间有效(请求.开始时间)
            || !预计变化定义有效(请求.预计变化阶段组)
            || !请求.结果获取方案.完整()) {
            结果.状态 = 新任务操作状态::方法影响定义不合法;
            return 结果;
        }
        for (const auto& 目标 : 请求.结果获取方案.目标组) {
            if (!节点仍存在(目标.作用对象节点)
                || !特征服务_.是特征节点(目标.实际特征节点)
                || !特征概念服务_.是特征概念节点(
                    目标.特征类型根概念节点)) {
                结果.状态 = 新任务操作状态::方法影响定义不合法;
                return 结果;
            }
        }
        for (const auto& 目标 : 请求.结果获取方案.禁止结果组) {
            if (!节点仍存在(目标.作用对象节点)
                || !特征服务_.是特征节点(目标.实际特征节点)
                || !特征概念服务_.是特征概念节点(
                    目标.特征类型根概念节点)
                || (目标.禁止值域概念节点
                    && !特征概念服务_.是特征概念节点(
                        *目标.禁止值域概念节点))
                || (目标.禁止状态或动态概念节点
                    && !节点仍存在(*目标.禁止状态或动态概念节点))) {
                结果.状态 = 新任务操作状态::方法影响定义不合法;
                return 结果;
            }
        }

        const auto 核验直接域 = [&](稳定编码 类型, 稳定编码 实际特征,
            const std::optional<特征概念I64闭区间>& 域) {
            if (!域) return 新任务操作状态::已找到;
            const auto 输入 = 取得任务值域输入(特征概念服务_, 类型, {}, 域);
            const auto 特征 = 特征服务_.获取特征(实际特征);
            if (!输入 || !特征 || 输入->单位 != 特征->实例单位
                || 取得特征概念类型根(特征概念服务_, 特征->特征概念节点)
                    != 取得特征概念类型根(特征概念服务_, 类型)) return 新任务操作状态::方法影响定义不合法;
            const auto 比较 = 特征服务_.比较特征值域(*输入, *输入);
            if (比较.状态 == 新特征比较状态::资源失败) return 新任务操作状态::资源失败;
            return 比较.状态 == 新特征比较状态::已完成 && 比较.关系编码
                ? 新任务操作状态::已找到 : 新任务操作状态::方法影响定义不合法;
        };
        for (const auto& 阶段 : 请求.预计变化阶段组) for (const auto& 变化 : 阶段.变化组) {
            if (!变化.预期精确数值域) continue;
            const auto 类型根 = 取得特征概念类型根(特征概念服务_, 变化.特征类型概念节点);
            const auto 目标 = std::find_if(请求.结果获取方案.目标组.begin(), 请求.结果获取方案.目标组.end(),
                [&](const auto& x) { return x.作用对象节点 == 变化.作用对象节点 && 类型根 == x.特征类型根概念节点; });
            if (目标 == 请求.结果获取方案.目标组.end()) {
                结果.状态 = 新任务操作状态::方法影响定义不合法; return 结果;
            }
            const auto 校验 = 核验直接域(变化.特征类型概念节点, 目标->实际特征节点, 变化.预期精确数值域);
            if (校验 != 新任务操作状态::已找到) { 结果.状态 = 校验; return 结果; }
        }
        for (const auto& 目标 : 请求.结果获取方案.禁止结果组) {
            const auto 校验 = 核验直接域(目标.特征类型根概念节点, 目标.实际特征节点, 目标.禁止精确数值域);
            if (校验 != 新任务操作状态::已找到) { 结果.状态 = 校验; return 结果; }
        }

        if (服务结算配置_ && 服务结算配置_->结算任务节点 == 请求.任务节点) {
            if (!任务->服务结算来源) {
                结果.状态 = 新任务操作状态::方法影响定义不合法;
                return 结果;
            }
            const auto 来源 = 读取任务虚拟存在引用(任务->服务结算来源->来源任务);
            const auto 当前 = 需求服务_.复核当前满足(任务->需求节点);
            const auto 特征 = 当前.实际特征节点
                ? 特征服务_.获取特征(*当前.实际特征节点) : std::nullopt;
            if (!来源 || 来源->当前状态 != static_cast<std::int64_t>(新任务状态::已完成)
                || 当前.状态 != 新需求当前满足状态::需增加 || !特征
                || 特征->当前值 != std::optional<新特征准确值>{新特征准确值{任务->服务结算来源->冻结前值}}
                || std::none_of(请求.结果获取方案.目标组.begin(), 请求.结果获取方案.目标组.end(),
                    [&](const auto& 目标) {
                        return 目标.实际特征节点 == *当前.实际特征节点
                            && 目标.动作前准确值 == 新特征准确值{任务->服务结算来源->冻结前值};
                    })) {
                结果.状态 = 新任务操作状态::方法影响定义不合法;
                return 结果;
            }
        }
        const auto 执行节点 = 全局基础数据集.新建节点();
        if (!有效(执行节点)) {
            结果.状态 = 新任务操作状态::资源失败;
            return 结果;
        }
        std::vector<稳定编码> 执行字段;
        执行字段.reserve(16);
        已建预计结构 预计结构;
        已建结果获取结构 结果获取结构;
        const auto 清理新执行 = [&]() noexcept {
            try{任务执行组织存储::清理执行采用已加锁(执行节点);}
            catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"清理构造中X采用",L"采用结构核验失败");}
            清理结果获取结构(结果获取结构);
            清理预计结构(预计结构);
            回滚字段(执行字段);
            (void)全局基础数据集.删除节点(执行节点);
        };
        const bool 执行基础完整 =
            写入节点字段(执行节点, 节点类型字段,
                任务执行实例节点类型, 执行字段)
            && 写入节点字段(执行节点, 执行所属任务字段,
                请求.任务节点, 执行字段)
            && 写入节点字段(执行节点, 执行方法字段,
                请求.方法节点, 执行字段)
            && 写入I64字段(执行节点, 执行状态字段,
                static_cast<std::int64_t>(新任务执行状态::执行中),
                执行字段)
            && 写入I64字段(执行节点, 执行开始时间语义字段,
                static_cast<std::int64_t>(请求.开始时间.语义), 执行字段)
            && 写入I64字段(执行节点, 执行开始时间字段,
                请求.开始时间.纳秒, 执行字段);
        if (!执行基础完整
            || (任务->服务结算来源 && !写入服务结算来源字段(
                执行节点, *任务->服务结算来源, 执行字段))
            || !建立预计结构(执行节点, 请求.预计变化阶段组, 预计结构)
            || !建立结果获取结构(
                执行节点, 请求.结果获取方案, 结果获取结构)) {
            清理新执行();
            结果.状态 = 新任务操作状态::资源失败;
            return 结果;
        }

        任务执行组织存储::建立执行采用已加锁(执行节点,*请求.本能采用);
        const auto 构造读回=读取执行信息(执行节点,状态服务_,特征服务_,特征概念服务_);
        if(!构造读回||构造读回->本能采用!=请求.本能采用)throw std::logic_error("X采用完整发布读回失败");
        const auto 原方法 = 读取唯一节点字段记录(
            请求.任务节点, 任务最近方法字段);
        const auto 原执行 = 读取唯一节点字段记录(
            请求.任务节点, 任务最近执行字段);
        const auto 方法字段组 = 全局基础数据集.查询字段(
            请求.任务节点, 任务最近方法字段);
        const auto 执行字段组 = 全局基础数据集.查询字段(
            请求.任务节点, 任务最近执行字段);
        if (方法字段组.size() > 1 || 执行字段组.size() > 1) {
            清理新执行();
            结果.状态 = 新任务操作状态::结构不一致;
            return 结果;
        }
        std::optional<稳定编码> 新方法字段;
        std::optional<稳定编码> 新执行字段;
        bool 方法已设置 = false;
        bool 执行已设置 = false;
        if (原方法) {
            方法已设置 = 全局基础数据集.修改字段节点(
                原方法->关系, 请求.方法节点);
        } else {
            const auto 字段 = 全局基础数据集.添加字段节点(
                请求.任务节点, 任务最近方法字段, 请求.方法节点);
            方法已设置 = 有效(字段);
            if (方法已设置) 新方法字段 = 字段;
        }
        if (方法已设置) {
            if (原执行) {
                执行已设置 = 全局基础数据集.修改字段节点(
                    原执行->关系, 执行节点);
            } else {
                const auto 字段 = 全局基础数据集.添加字段节点(
                    请求.任务节点, 任务最近执行字段, 执行节点);
                执行已设置 = 有效(字段);
                if (执行已设置) 新执行字段 = 字段;
            }
        }
        auto 恢复最近字段 = [&]() noexcept {
            if (新执行字段) {
                (void)全局基础数据集.删除字段(*新执行字段);
            } else if (原执行) {
                (void)全局基础数据集.修改字段节点(
                    原执行->关系, 原执行->目标);
            }
            if (新方法字段) {
                (void)全局基础数据集.删除字段(*新方法字段);
            } else if (原方法) {
                (void)全局基础数据集.修改字段节点(
                    原方法->关系, 原方法->目标);
            }
        };
        if (!方法已设置 || !执行已设置) {
            恢复最近字段();
            清理新执行();
            结果.状态 = 新任务操作状态::资源失败;
            return 结果;
        }

        const auto 虚拟存在 = 读取任务虚拟存在信息(
            请求.任务节点, 特征服务_);
        if (!虚拟存在) {
            恢复最近字段();
            清理新执行();
            结果.状态 = 新任务操作状态::任务虚拟存在缺失;
            return 结果;
        }
        const auto 状态结果 = 写入任务状态(
            *虚拟存在, 新任务状态::执行中, 特征服务_);
        if (状态结果 != 新任务操作状态::已修改) {
            const auto 当前虚拟存在 = 读取任务虚拟存在信息(请求.任务节点, 特征服务_);
            if (!当前虚拟存在 || 当前虚拟存在->当前状态 == 新任务状态::执行中) {
                // 当前状态可能已经发布，不得删除其正式X和采用证据。
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"开始方法执行", L"任务状态发布结果不确定，保留执行证据");
            }
            恢复最近字段();
            清理新执行();
            结果.状态 = 状态结果;
            return 结果;
        }
        已开始过执行_ = true;
        const auto 读回 = 读取执行信息(执行节点, 状态服务_, 特征服务_, 特征概念服务_);
        if (!读回 || 读回->任务节点 != 请求.任务节点
            || 读回->方法节点 != 请求.方法节点
            || 读回->状态 != 新任务执行状态::执行中
            || 读回->结果获取方案 != 请求.结果获取方案
            || 读回->服务结算来源 != 任务->服务结算来源) {
            结果.状态 = 新任务操作状态::结构不一致;
            结果.执行实例节点 = 执行节点;
            return 结果;
        }
        结果.状态 = 新任务操作状态::已开始执行;
        结果.执行实例节点 = 执行节点;
        return 结果;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"开始方法执行", L"执行采用发布工程故障，保留已发布事实");
    }
}

namespace {
bool 核验执行本能动作(const 新任务执行信息& 执行, const 新任务信息& 任务,
    const 新动态信息& 动作, 稳定编码 自我, const 新_存在类& 存在, const 新_场景类& 场景) {
    if (执行.任务节点 != 任务.节点 || 动作.种类 != 新动态种类::动作
        || 动作.动作来源节点 != 执行.节点 || 动作.动作定义节点 != 执行.方法节点
        || 动作.主题存在节点 != 自我 || !存在.是存在节点(自我)
        || 动作.动作目标种类 != 新动作目标种类::普通存在 || !动作.动作目标存在节点
        || !动作.动作时间 || 动作.动作时间->语义 != 执行.开始时间.语义
        || 动作.动作时间->纳秒 < 执行.开始时间.纳秒
        || (执行.结束时间 && 动作.动作时间->纳秒 > 执行.结束时间->纳秒)) return false;
    const auto 目标 = 存在.获取存在(*动作.动作目标存在节点);
    if (!目标 || (动作.动作目标特征节点 && std::find(目标->特征节点组.begin(),
        目标->特征节点组.end(), *动作.动作目标特征节点) == 目标->特征节点组.end())) return false;
    auto 位置 = 场景.查询所属场景(自我);
    std::set<稳定编码> 已查;
    while (位置) {
        if (!已查.insert(*位置).second) return false;
        const auto 列表 = 场景.查询动态列表(*位置);
        if (std::find(列表.begin(), 列表.end(), 动作.节点) != 列表.end())
            return 场景.存在属于场景范围(*位置, *动作.动作目标存在节点);
        位置 = 场景.查询父场景(*位置);
    }
    return false;
}
}

新任务操作状态 新_任务类::记录本能动作身份(
    稳定编码 任务节点, 稳定编码 执行实例节点,
    稳定编码 动作动态节点, 新_动态类& 动态服务) noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        if (!本能动作读取_ || 本能动作读取_ != &动态服务) return 新任务操作状态::依赖未实现;
        const auto 任务 = 获取任务(任务节点);
        const auto 执行 = 获取执行实例(执行实例节点);
        const auto 动作 = 动态服务.获取动态(动作动态节点);
        const auto 需求 = 任务 ? 需求服务_.获取需求(任务->需求节点) : std::nullopt;
        if (!任务 || !执行 || !需求 || !动作 || 执行->任务节点 != 任务节点)
            return 新任务操作状态::结构不一致;
        if (执行->服务结算来源) {
        if (执行->任务节点 != 任务节点 || 任务->服务结算来源 != 执行->服务结算来源
            || 动作->种类 != 新动态种类::动作
            || 动作->动作目标种类 != 新动作目标种类::普通存在
            || 动作->动作来源节点 != 执行->服务结算来源->来源任务.任务节点
            || 动作->动作定义节点 != 执行->方法节点
            || 动作->主题存在节点 != 需求->目标存在节点
            || 动作->动作目标存在节点 != 需求->目标存在节点
            || !动作->动作目标特征节点 || !动作->动作时间
            || 动作->动作时间->语义 != 执行->开始时间.语义
            || 动作->动作时间->纳秒 < 执行->开始时间.纳秒) return 新任务操作状态::结构不一致;
        const auto 目标 = std::find_if(执行->结果获取方案.目标组.begin(),
            执行->结果获取方案.目标组.end(), [&](const auto& 项) {
                return 项.作用对象节点 == 动作->主题存在节点
                    && 项.实际特征节点 == *动作->动作目标特征节点
                    && 项.特征类型根概念节点 == 需求->目标.目标特征类型概念节点;
            });
        const auto 场景 = 场景服务_.查询所属场景(需求->目标存在节点);
        if (目标 == 执行->结果获取方案.目标组.end() || !场景) return 新任务操作状态::结构不一致;
        const auto 动作列表 = 场景服务_.查询动态列表(*场景);
        if (std::find(动作列表.begin(), 动作列表.end(), 动作动态节点) == 动作列表.end())
            return 新任务操作状态::结构不一致;
        } else {
            const auto 安全根 = 需求服务_.获取需求(安全根需求节点);
            const auto 服务根 = 需求服务_.获取需求(服务根需求节点);
            if (!安全根 || !服务根 || 安全根->目标存在节点 != 服务根->目标存在节点
                || !核验执行本能动作(*执行, *任务, *动作, 服务根->目标存在节点, 存在服务_, 场景服务_))
                return 新任务操作状态::结构不一致;
        }
        if (执行->普通动作节点)
            return *执行->普通动作节点 == 动作动态节点 ? 新任务操作状态::精确重复 : 新任务操作状态::结构不一致;
        if (任务->当前状态 != 新任务状态::执行中 || 执行->状态 != 新任务执行状态::执行中
            || 任务->最近执行实例节点 != 执行实例节点) return 新任务操作状态::任务状态不允许;
        if (动态服务.增加引用(动作动态节点) != 新动态操作状态::已完成)
            return 新任务操作状态::资源失败;
        const auto 字段 = 全局基础数据集.添加字段节点(执行实例节点, 执行普通动作字段, 动作动态节点);
        if (!有效(字段)) {
            return 动态服务.撤销增加引用(动作动态节点) == 新动态操作状态::已完成
                ? 新任务操作状态::资源失败 : 新任务操作状态::结构不一致;
        }
        const auto 读回 = 获取执行实例(执行实例节点);
        if (读回 && 读回->普通动作节点 == 动作动态节点) return 新任务操作状态::已修改;
        if (!全局基础数据集.删除字段(字段)) return 新任务操作状态::结构不一致;
        (void)动态服务.撤销增加引用(动作动态节点);
        return 新任务操作状态::结构不一致;
    } catch (...) { return 新任务操作状态::资源失败; }
}

新任务操作状态 新_任务类::记录方法动作输出(
    稳定编码 任务节点,
    稳定编码 执行实例节点,
    const std::vector<稳定编码>& 状态节点组) noexcept {
    if (!有效(任务节点) || !有效(执行实例节点) || 状态节点组.empty()) {
        return 新任务操作状态::入口拒绝;
    }
    try {
        std::lock_guard 锁(新任务互斥);
        if (!结构仍存在()) return 新任务操作状态::尚未初始化;
        const auto 任务 = 读取任务信息(任务节点, 特征服务_);
        const auto 执行 = 读取执行信息(执行实例节点, 状态服务_, 特征服务_, 特征概念服务_);
        if (!任务) return 新任务操作状态::任务不存在;
        if (!执行) return 新任务操作状态::执行实例不存在;
        if (执行->任务节点 != 任务节点) {
            return 新任务操作状态::执行实例不属于任务;
        }

        auto 请求状态 = 状态节点组;
        std::sort(请求状态.begin(), 请求状态.end());
        if (std::adjacent_find(请求状态.begin(), 请求状态.end())
            != 请求状态.end()) {
            return 新任务操作状态::入口拒绝;
        }
        if (!执行->动作输出状态节点组.empty()) {
            return 执行->动作输出状态节点组 == 请求状态
                ? 新任务操作状态::精确重复
                : 新任务操作状态::结构不一致;
        }
        if (任务->当前状态 != 新任务状态::执行中
            || 执行->状态 != 新任务执行状态::执行中
            || !任务->最近执行实例节点
            || *任务->最近执行实例节点 != 执行实例节点) {
            return 新任务操作状态::任务状态不允许;
        }

        for (const auto 状态节点 : 请求状态) {
            const auto 状态 = 状态服务_.获取状态(状态节点);
            if (!状态) return 新任务操作状态::状态不存在;
            if (状态->强时间.语义 != 执行->开始时间.语义
                || 状态->强时间.纳秒 < 执行->开始时间.纳秒) {
                return 新任务操作状态::状态时间不合法;
            }
            const auto 目标位置 = std::find_if(
                执行->结果获取方案.目标组.begin(),
                执行->结果获取方案.目标组.end(),
                [&](const 新任务结果获取目标& 目标) {
                    return 目标.作用对象节点 == 状态->被描述存在节点
                        && 目标.实际特征节点 == 状态->来源实例特征节点;
                });
            if (目标位置 == 执行->结果获取方案.目标组.end()) {
                return 新任务操作状态::方法影响定义不合法;
            }
            const auto 特征 = 特征服务_.获取特征(目标位置->实际特征节点);
            if (!特征
                || 特征->特征概念节点 != 状态->特征类型概念节点) {
                return 新任务操作状态::结构不一致;
            }
        }

        std::vector<稳定编码> 已增加状态引用;
        const auto 引用结果 = 增加状态引用组(请求状态, 已增加状态引用);
        if (引用结果 != 新任务操作状态::已修改) return 引用结果;

        std::vector<稳定编码> 已写字段;
        for (const auto 状态节点 : 请求状态) {
            if (!写入节点字段(执行实例节点, 执行动作输出状态字段,
                    状态节点, 已写字段)) {
                回滚字段(已写字段);
                return 撤销增加状态引用组(已增加状态引用)
                    ? 新任务操作状态::资源失败
                    : 新任务操作状态::结构不一致;
            }
        }
        const auto 读回 = 读取执行信息(执行实例节点, 状态服务_, 特征服务_, 特征概念服务_);
        if (!读回 || 读回->动作输出状态节点组 != 请求状态) {
            回滚字段(已写字段);
            (void)撤销增加状态引用组(已增加状态引用);
            return 新任务操作状态::结构不一致;
        }
        return 新任务操作状态::已修改;
    } catch (...) {
        return 新任务操作状态::资源失败;
    }
}

新任务操作状态 新_任务类::撤销未进入动作执行(
    const 新任务未进入动作撤销请求& 请求) noexcept {
    if (!有效(请求.任务节点) || !有效(请求.执行实例节点)
        || 请求.原最近方法节点.has_value()
            != 请求.原最近执行实例节点.has_value()) {
        return 新任务操作状态::入口拒绝;
    }
    try {
        std::lock_guard 锁(新任务互斥);
        if (!结构仍存在()) return 新任务操作状态::尚未初始化;
        const auto 任务 = 读取任务信息(请求.任务节点, 特征服务_);
        if (!任务) return 新任务操作状态::任务不存在;

        if (!全局基础数据集.查询节点(请求.执行实例节点)) {
            return 任务->当前状态 == 新任务状态::待执行
                    && 任务->最近方法节点 == 请求.原最近方法节点
                    && 任务->最近执行实例节点
                        == 请求.原最近执行实例节点
                ? 新任务操作状态::精确重复
                : 新任务操作状态::执行实例不存在;
        }

        const auto 执行 = 读取执行信息(请求.执行实例节点, 状态服务_, 特征服务_, 特征概念服务_);
        if (!执行) return 新任务操作状态::执行实例不存在;
        if (执行->任务节点 != 请求.任务节点) {
            return 新任务操作状态::执行实例不属于任务;
        }
        if (任务->当前状态 != 新任务状态::执行中
            || 任务->最近方法节点 != 执行->方法节点
            || 任务->最近执行实例节点 != 请求.执行实例节点
            || 执行->状态 != 新任务执行状态::执行中
            || 执行->结束时间 || !执行->实际状态节点组.empty()
            || !执行->动作输出状态节点组.empty() || 执行->普通动作节点
            || 执行->影响评估节点) {
            return 新任务操作状态::任务状态不允许;
        }

        if (请求.原最近方法节点) {
            const auto 原执行 = 读取执行信息(
                *请求.原最近执行实例节点, 状态服务_, 特征服务_, 特征概念服务_);
            if (!节点仍存在(*请求.原最近方法节点)
                || !原执行 || 原执行->任务节点 != 请求.任务节点
                || 原执行->方法节点 != *请求.原最近方法节点) {
                return 新任务操作状态::结构不一致;
            }
        }
        const auto 阶段组 = 读取预计阶段物理组(请求.执行实例节点);
        const auto 虚拟存在 = 读取任务虚拟存在信息(
            请求.任务节点, 特征服务_);
        const auto 当前方法字段 = 读取唯一节点字段记录(
            请求.任务节点, 任务最近方法字段);
        const auto 当前执行字段 = 读取唯一节点字段记录(
            请求.任务节点, 任务最近执行字段);
        if (!阶段组 || !虚拟存在 || !当前方法字段 || !当前执行字段
            || 当前方法字段->目标 != 执行->方法节点
            || 当前执行字段->目标 != 请求.执行实例节点) {
            return 新任务操作状态::结构不一致;
        }

        bool 定位已恢复 = false;
        if (请求.原最近方法节点) {
            if (!全局基础数据集.修改字段节点(
                    当前方法字段->关系, *请求.原最近方法节点)) {
                return 新任务操作状态::资源失败;
            }
            if (!全局基础数据集.修改字段节点(
                    当前执行字段->关系, *请求.原最近执行实例节点)) {
                (void)全局基础数据集.修改字段节点(
                    当前方法字段->关系, 执行->方法节点);
                return 新任务操作状态::资源失败;
            }
            定位已恢复 = true;
        } else {
            if (!全局基础数据集.删除字段(当前执行字段->关系)) {
                return 新任务操作状态::资源失败;
            }
            if (!全局基础数据集.删除字段(当前方法字段->关系)) {
                const auto 恢复执行 = 全局基础数据集.添加字段节点(
                    请求.任务节点, 任务最近执行字段,
                    请求.执行实例节点);
                return 有效(恢复执行)
                    ? 新任务操作状态::资源失败
                    : 新任务操作状态::结构不一致;
            }
            定位已恢复 = true;
        }

        const auto 状态回退 = 写入已核验任务状态值(
            *虚拟存在, 新任务状态::待执行, 特征服务_);
        if (状态回退 != 新任务操作状态::已修改
            && 状态回退 != 新任务操作状态::无变化) {
            if (定位已恢复) {
                if (请求.原最近方法节点) {
                    (void)全局基础数据集.修改字段节点(
                        当前方法字段->关系, 执行->方法节点);
                    (void)全局基础数据集.修改字段节点(
                        当前执行字段->关系, 请求.执行实例节点);
                } else {
                    (void)全局基础数据集.添加字段节点(
                        请求.任务节点, 任务最近方法字段, 执行->方法节点);
                    (void)全局基础数据集.添加字段节点(
                        请求.任务节点, 任务最近执行字段,
                        请求.执行实例节点);
                }
            }
            return 状态回退;
        }

        if (!删除未进入动作执行结构(
                请求.执行实例节点, *阶段组, 本能动作读取_)) {
            return 新任务操作状态::结构不一致;
        }
        const auto 读回 = 读取任务信息(请求.任务节点, 特征服务_);
        if (!读回 || 读回->当前状态 != 新任务状态::待执行
            || 读回->最近方法节点 != 请求.原最近方法节点
            || 读回->最近执行实例节点
                != 请求.原最近执行实例节点
            || 全局基础数据集.查询节点(请求.执行实例节点)) {
            return 新任务操作状态::结构不一致;
        }
        return 新任务操作状态::已修改;
    } catch (...) {
        return 新任务操作状态::资源失败;
    }
}

新任务操作状态 新_任务类::结束方法执行(
    const 新任务执行结束请求& 请求) noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        if (!结构仍存在()) return 新任务操作状态::尚未初始化;
        const auto 任务 = 读取任务信息(请求.任务节点, 特征服务_);
        if (!任务) return 新任务操作状态::任务不存在;
        const auto 执行 = 读取执行信息(请求.执行实例节点, 状态服务_, 特征服务_, 特征概念服务_);
        if (!执行) return 新任务操作状态::执行实例不存在;
        if (执行->任务节点 != 请求.任务节点) {
            return 新任务操作状态::执行实例不属于任务;
        }
        if (!强时间有效(请求.结束时间)
            || 请求.结束时间.纳秒 < 执行->开始时间.纳秒) {
            return 新任务操作状态::状态时间不合法;
        }

        std::vector<std::pair<新状态信息, 稳定编码>> 状态组;
        std::set<std::uint64_t> 已见状态;
        for (const auto 状态节点 : 请求.状态节点组) {
            if (!已见状态.insert(状态节点.值).second) {
                return 新任务操作状态::入口拒绝;
            }
            const auto 状态 = 状态服务_.获取状态(状态节点);
            if (!状态) return 新任务操作状态::状态不存在;
            if (!强时间有效(状态->强时间)
                || 状态->强时间.纳秒 < 执行->开始时间.纳秒
                || 状态->强时间.纳秒 > 请求.结束时间.纳秒) {
                return 新任务操作状态::状态时间不合法;
            }
            const bool 属于结果方案 = std::any_of(
                执行->结果获取方案.目标组.begin(),
                执行->结果获取方案.目标组.end(),
                [&](const 新任务结果获取目标& 目标) {
                    if (目标.作用对象节点 != 状态->被描述存在节点
                        || 目标.实际特征节点
                            != 状态->来源实例特征节点) return false;
                    const auto 特征 = 特征服务_.获取特征(
                        目标.实际特征节点);
                    return 特征 && 特征->特征概念节点
                        == 状态->特征类型概念节点;
                });
            if (!属于结果方案) return 新任务操作状态::入口拒绝;
            状态组.push_back({*状态, 状态节点});
        }
        std::sort(状态组.begin(), 状态组.end(), [](const auto& 左, const auto& 右) {
            if (左.first.强时间.纳秒 != 右.first.强时间.纳秒) {
                return 左.first.强时间.纳秒 < 右.first.强时间.纳秒;
            }
            return 左.second < 右.second;
        });

        if (执行->状态 != 新任务执行状态::执行中) {
            std::vector<稳定编码> 已有 = 执行->实际状态节点组;
            std::vector<稳定编码> 请求状态;
            for (const auto& 项 : 状态组) 请求状态.push_back(项.second);
            if (执行->结束时间 && *执行->结束时间 == 请求.结束时间
                && 已有 == 请求状态) return 新任务操作状态::精确重复;
            return 新任务操作状态::任务状态不允许;
        }
        if (任务->当前状态 != 新任务状态::执行中) {
            return 新任务操作状态::任务状态不允许;
        }

        std::vector<稳定编码> 状态引用节点组;
        状态引用节点组.reserve(状态组.size());
        for (const auto& [状态, 状态节点] : 状态组) {
            (void)状态;
            状态引用节点组.push_back(状态节点);
        }
        std::vector<稳定编码> 已增加状态引用;
        const auto 引用结果 = 增加状态引用组(
            状态引用节点组, 已增加状态引用);
        if (引用结果 != 新任务操作状态::已修改) return 引用结果;

        std::vector<稳定编码> 已写字段;
        std::vector<稳定编码> 已建项节点;
        const auto 回滚未发布实际状态 = [&]() noexcept {
            回滚字段(已写字段);
            for (auto 位置 = 已建项节点.rbegin();
                 位置 != 已建项节点.rend(); ++位置) {
                (void)全局基础数据集.删除节点(*位置);
            }
            return 撤销增加状态引用组(已增加状态引用);
        };
        for (std::size_t i = 0; i < 状态组.size(); ++i) {
            const auto 项节点 = 全局基础数据集.新建节点();
            if (!有效(项节点)) {
                return 回滚未发布实际状态()
                    ? 新任务操作状态::资源失败
                    : 新任务操作状态::结构不一致;
            }
            已建项节点.push_back(项节点);
            const bool 完整 =
                写入节点字段(项节点, 节点类型字段,
                    任务实际状态项节点类型, 已写字段)
                && 写入I64字段(项节点, 实际状态顺序字段,
                    static_cast<std::int64_t>(i), 已写字段)
                && 写入节点字段(项节点, 实际状态节点字段,
                    状态组[i].second, 已写字段)
                && 写入节点字段(请求.执行实例节点,
                    执行实际状态项字段, 项节点, 已写字段);
            if (!完整) {
                return 回滚未发布实际状态()
                    ? 新任务操作状态::资源失败
                    : 新任务操作状态::结构不一致;
            }
        }
        const bool 结束字段完整 =
            写入I64字段(请求.执行实例节点, 执行结束时间语义字段,
                static_cast<std::int64_t>(请求.结束时间.语义), 已写字段)
            && 写入I64字段(请求.执行实例节点, 执行结束时间字段,
                请求.结束时间.纳秒, 已写字段);
        const auto 执行状态 = 读取唯一I64字段记录(
            请求.执行实例节点, 执行状态字段);
        if (!结束字段完整 || !执行状态
            || !全局基础数据集.修改字段值(
                执行状态->关系,
                基础值{static_cast<std::int64_t>(
                    新任务执行状态::已结束待评估)})) {
            return 回滚未发布实际状态()
                ? 新任务操作状态::资源失败
                : 新任务操作状态::结构不一致;
        }

        const auto 虚拟存在 = 读取任务虚拟存在信息(
            请求.任务节点, 特征服务_);
        if (!虚拟存在) {
            (void)全局基础数据集.修改字段值(
                执行状态->关系,
                基础值{static_cast<std::int64_t>(新任务执行状态::执行中)});
            return 回滚未发布实际状态()
                ? 新任务操作状态::任务虚拟存在缺失
                : 新任务操作状态::结构不一致;
        }
        const auto 状态结果 = 写入任务状态(
            *虚拟存在, 新任务状态::待影响评估, 特征服务_);
        if (状态结果 != 新任务操作状态::已修改) {
            (void)全局基础数据集.修改字段值(
                执行状态->关系,
                基础值{static_cast<std::int64_t>(新任务执行状态::执行中)});
            return 回滚未发布实际状态()
                ? 状态结果 : 新任务操作状态::结构不一致;
        }
        const auto 读回 = 读取执行信息(请求.执行实例节点, 状态服务_, 特征服务_, 特征概念服务_);
        if (!读回 || 读回->状态 != 新任务执行状态::已结束待评估) {
            return 新任务操作状态::结构不一致;
        }
        return 新任务操作状态::已结束执行;
    } catch (...) {
        return 新任务操作状态::资源失败;
    }
}

新任务影响评估结果 新_任务类::评估方法执行影响(
    稳定编码 任务节点,
    稳定编码 执行实例节点) noexcept {
    新任务影响评估结果 结果;
    try {
        std::lock_guard 锁(新任务互斥);
        if (!结构仍存在()) {
            结果.状态 = 新任务操作状态::尚未初始化;
            return 结果;
        }
        const auto 任务 = 读取任务信息(任务节点, 特征服务_);
        if (!任务) {
            结果.状态 = 新任务操作状态::任务不存在;
            return 结果;
        }
        const auto 执行 = 读取执行信息(执行实例节点, 状态服务_, 特征服务_, 特征概念服务_);
        if (!执行) {
            结果.状态 = 新任务操作状态::执行实例不存在;
            return 结果;
        }
        if (执行->任务节点 != 任务节点) {
            结果.状态 = 新任务操作状态::执行实例不属于任务;
            return 结果;
        }
        const auto 方法 = 方法服务_.获取方法(执行->方法节点);
        if (!方法) {
            结果.状态 = 新任务操作状态::结构不一致;
            return 结果;
        }
        新方法条件结果信息 冻结配对;
        if (执行->本能采用) {
            冻结配对.条件 = 执行->本能采用->需要条件;
            冻结配对.结果 = 执行->本能采用->结果;
        }
        const auto 私有域检查 = 执行->本能采用
            ? (含私有数值域(冻结配对)
                ? 方法私有域闭包检查状态::含私有域
                : 方法私有域闭包检查状态::未含私有域)
            : 检查方法私有域闭包(方法服务_, *方法);
        if (私有域检查 == 方法私有域闭包检查状态::方法不可读
            || 私有域检查 == 方法私有域闭包检查状态::资源失败) {
            结果.状态 = 私有域检查 == 方法私有域闭包检查状态::资源失败
                ? 新任务操作状态::资源失败 : 新任务操作状态::结构不一致;
            return 结果;
        }
        if (执行->状态 == 新任务执行状态::已评估
            && 执行->影响评估节点) {
            const auto 已有 = 读取评估信息(*执行->影响评估节点);
            if (!已有) {
                结果.状态 = 新任务操作状态::结构不一致;
                return 结果;
            }
            结果.状态 = 新任务操作状态::精确重复;
            结果.评估 = *已有;
            return 结果;
        }
        if (执行->状态 != 新任务执行状态::已结束待评估
            || 任务->当前状态 != 新任务状态::待影响评估) {
            结果.状态 = 新任务操作状态::任务状态不允许;
            return 结果;
        }
        const auto 阶段组 = 读取预计阶段物理组(执行实例节点);
        const auto 实际组 = 读取实际状态物理组(执行实例节点, 状态服务_);
        if (!阶段组 || !实际组) {
            结果.状态 = 新任务操作状态::结构不一致;
            return 结果;
        }
        std::set<std::uint64_t> 实际状态身份;
        for (const auto& 实际 : *实际组) {
            实际状态身份.insert(实际.状态节点.值);
        }
        if (std::any_of(执行->动作输出状态节点组.begin(),
                执行->动作输出状态节点组.end(), [&](稳定编码 状态节点) {
                    return !实际状态身份.contains(状态节点.值);
                })) {
            结果.状态 = 新任务操作状态::结构不一致;
            return 结果;
        }

        std::vector<bool> 已使用(实际组->size(), false);
        std::vector<稳定编码> 命中状态;
        std::vector<稳定编码> 必需命中状态;
        std::vector<稳定编码> 缺失项;
        std::vector<std::pair<新任务预计特征变化, 新状态信息>> 命中细节;
        const auto 概念属于类型根 = [&](稳定编码 概念,
                                         稳定编码 类型根)
            -> std::optional<bool> {
            const auto 关系 = 特征概念服务_.比较单特征概念(概念, 类型根);
            if (关系.状态 != 特征概念规则状态::已完成 || !关系.关系) {
                return std::nullopt;
            }
            return *关系.关系 == 单特征概念关系::同一
                || *关系.关系 == 单特征概念关系::右为上位;
        };
        std::int64_t 最早允许时间 = 执行->开始时间.纳秒;
        for (const auto& 阶段 : *阶段组) {
            std::int64_t 本阶段最晚时间 = 最早允许时间;
            bool 本阶段已有命中 = false;
            for (const auto& 预计 : 阶段.变化组) {
                std::optional<std::size_t> 最佳;
                for (std::size_t i = 0; i < 实际组->size(); ++i) {
                    if (已使用[i]) continue;
                    const auto& 实际 = (*实际组)[i].状态;
                    const auto 方案目标 = std::find_if(
                        执行->结果获取方案.目标组.begin(),
                        执行->结果获取方案.目标组.end(),
                        [&](const 新任务结果获取目标& 目标) {
                            return 目标.作用对象节点
                                    == 实际.被描述存在节点
                                && 目标.实际特征节点
                                    == 实际.来源实例特征节点;
                        });
                    if (方案目标 == 执行->结果获取方案.目标组.end()) {
                        continue;
                    }
                    const auto 预计类型正确 = 概念属于类型根(
                        预计.变化.特征类型概念节点,
                        方案目标->特征类型根概念节点);
                    const auto 实际类型正确 = 概念属于类型根(
                        实际.特征类型概念节点,
                        方案目标->特征类型根概念节点);
                    if (!预计类型正确 || !实际类型正确) {
                        结果.状态 = 新任务操作状态::结构不一致;
                        return 结果;
                    }
                    if (实际.强时间.纳秒 < 最早允许时间
                        || 实际.被描述存在节点 != 预计.变化.作用对象节点
                        || !*预计类型正确 || !*实际类型正确) continue;
                    if (!最佳
                        || 实际.强时间.纳秒
                            < (*实际组)[*最佳].状态.强时间.纳秒
                        || (实际.强时间.纳秒
                                == (*实际组)[*最佳].状态.强时间.纳秒
                            && (*实际组)[i].状态节点
                                < (*实际组)[*最佳].状态节点)) {
                        最佳 = i;
                    }
                }
                if (最佳) {
                    已使用[*最佳] = true;
                    命中状态.push_back((*实际组)[*最佳].状态节点);
                    if (预计.变化.必须出现) {
                        必需命中状态.push_back((*实际组)[*最佳].状态节点);
                    }
                    命中细节.push_back({
                        预计.变化, (*实际组)[*最佳].状态});
                    本阶段最晚时间 = (std::max)(本阶段最晚时间,
                        (*实际组)[*最佳].状态.强时间.纳秒);
                    本阶段已有命中 = true;
                } else if (预计.变化.必须出现) {
                    缺失项.push_back(预计.节点);
                }
            }
            if (本阶段已有命中) 最早允许时间 = 本阶段最晚时间;
        }
        std::vector<稳定编码> 计划外状态;
        for (std::size_t i = 0; i < 实际组->size(); ++i) {
            if (!已使用[i]) 计划外状态.push_back((*实际组)[i].状态节点);
        }

        const bool 有方向要求 = std::any_of(
            阶段组->begin(), 阶段组->end(), [](const auto& 阶段) {
                return std::any_of(阶段.变化组.begin(), 阶段.变化组.end(),
                    [](const auto& 项) {
                        return 项.变化.预期方向.has_value() || 项.变化.预期变化关系节点.has_value();
                    });
            });
        const bool 有值域要求 = std::any_of(
            阶段组->begin(), 阶段组->end(), [](const auto& 阶段) {
                return std::any_of(阶段.变化组.begin(), 阶段.变化组.end(),
                    [](const auto& 项) {
                        return 项.变化.预期值域概念节点.has_value() || 项.变化.预期精确数值域.has_value();
                    });
            });
        auto 合并验证 = [](新任务影响验证状态 当前,
            新任务影响验证状态 新值) noexcept {
            if (当前 == 新任务影响验证状态::冲突
                || 新值 == 新任务影响验证状态::冲突) {
                return 新任务影响验证状态::冲突;
            }
            if (当前 == 新任务影响验证状态::已验证不通过
                || 新值 == 新任务影响验证状态::已验证不通过) {
                return 新任务影响验证状态::已验证不通过;
            }
            if (当前 == 新任务影响验证状态::未验证
                || 新值 == 新任务影响验证状态::未验证) {
                return 新任务影响验证状态::未验证;
            }
            if (当前 == 新任务影响验证状态::已验证通过
                || 新值 == 新任务影响验证状态::已验证通过) {
                return 新任务影响验证状态::已验证通过;
            }
            return 新任务影响验证状态::不适用;
        };

        auto 查找命中 = [&](const 新任务预计特征变化& 变化)
            -> const 新状态信息* {
            const auto 位置 = std::find_if(
                命中细节.begin(), 命中细节.end(), [&](const auto& 项) {
                    return 项.first == 变化;
                });
            return 位置 == 命中细节.end() ? nullptr : &位置->second;
        };
        auto 查找目标 = [&](const 新状态信息& 状态)
            -> const 新任务结果获取目标* {
            const auto 位置 = std::find_if(
                执行->结果获取方案.目标组.begin(),
                执行->结果获取方案.目标组.end(),
                [&](const 新任务结果获取目标& 目标) {
                    return 目标.作用对象节点 == 状态.被描述存在节点
                        && 目标.实际特征节点 == 状态.来源实例特征节点;
                });
            return 位置 == 执行->结果获取方案.目标组.end()
                ? nullptr : &*位置;
        };

        auto 方向验证 = 有方向要求
            ? 新任务影响验证状态::已验证通过
            : 新任务影响验证状态::不适用;
        auto 值域验证 = 有值域要求
            ? 新任务影响验证状态::已验证通过
            : 新任务影响验证状态::不适用;
        for (const auto& 阶段 : *阶段组) {
            for (const auto& 预计 : 阶段.变化组) {
                const auto* 实际 = 查找命中(预计.变化);
                if (预计.变化.预期变化关系节点 || 预计.变化.预期方向) {
                    新任务影响验证状态 单项 =
                        新任务影响验证状态::未验证;
                    if (!实际) {
                        单项 = 预计.变化.必须出现
                            ? 新任务影响验证状态::已验证不通过
                            : 新任务影响验证状态::不适用;
                    } else if (预计.变化.预期方向) {
                        const auto* 目标 = 查找目标(*实际);
                        if (目标) {
                            const auto 比较 = 特征服务_.比较特征值(
                                目标->实际特征节点, 实际->固定准确值,
                                目标->动作前准确值);
                            if (比较.状态 == 新特征比较状态::已完成
                                && 比较.数值类型
                                    == 新特征比较数值类型::有向差值
                                && 比较.比较值) {
                                const auto 差值 = *比较.比较值;
                                const bool 符合 =
                                    (*预计.变化.预期方向
                                            == 新任务预期变化方向::增加
                                        && 差值 > 0)
                                    || (*预计.变化.预期方向
                                            == 新任务预期变化方向::减少
                                        && 差值 < 0)
                                    || (*预计.变化.预期方向
                                            == 新任务预期变化方向::不变
                                        && 差值 == 0);
                                单项 = 符合
                                    ? 新任务影响验证状态::已验证通过
                                    : 新任务影响验证状态::已验证不通过;
                            }
                        }
                    }
                    方向验证 = 合并验证(方向验证, 单项);
                }

                if (预计.变化.预期值域概念节点 || 预计.变化.预期精确数值域) {
                    新任务影响验证状态 单项 =
                        新任务影响验证状态::未验证;
                    if (!实际) {
                        单项 = 预计.变化.必须出现
                            ? 新任务影响验证状态::已验证不通过
                            : 新任务影响验证状态::不适用;
                    } else {
                        const auto* 目标 = 查找目标(*实际);
                        const auto 实例特征 = 目标
                            ? 特征服务_.获取特征(目标->实际特征节点)
                            : std::optional<新特征信息>{};
                        if (目标 && 实例特征 && std::holds_alternative<std::int64_t>(实际->固定准确值)) {
                            const auto 命中 = 准确I64落在任务值域(特征服务_, 特征概念服务_,
                                目标->特征类型根概念节点, 实例特征->实例单位,
                                std::get<std::int64_t>(实际->固定准确值),
                                预计.变化.预期值域概念节点, 预计.变化.预期精确数值域);
                            if (命中) 单项 = *命中 ? 新任务影响验证状态::已验证通过
                                : 新任务影响验证状态::已验证不通过;
                        } else if (目标 && 实例特征 && 预计.变化.预期值域概念节点) {
                            const auto 域 = 特征概念服务_.获取特征概念(*预计.变化.预期值域概念节点);
                            const auto* 集合 = 域 ? std::get_if<特征概念任务集合值域>(&域->值域) : nullptr;
                            if (集合) {
                                if (取得特征概念类型根(特征概念服务_, 域->节点)
                                        != 目标->特征类型根概念节点
                                    || 域->单位 || 实例特征->实例单位) {
                                    值域验证 = 合并验证(值域验证, 单项);
                                    continue;
                                }
                                const auto 比较 = 特征服务_.比较任务集合条件(
                                    目标->特征类型根概念节点, 实际->固定准确值, *集合);
                                if (比较.状态 == 特征概念规则状态::已完成 && 比较.比较值)
                                    单项 = *比较.比较值 == 1 ? 新任务影响验证状态::已验证通过
                                        : 新任务影响验证状态::已验证不通过;
                            } else {
                            const auto 按值查找 =
                                特征概念服务_.查找值对应的最具体概念(
                                    目标->特征类型根概念节点,
                                    实际->固定准确值,
                                    实例特征->实例单位,
                                    特征值服务_);
                            if (按值查找.状态 == 特征概念查找状态::已找到
                                && 按值查找.概念节点) {
                                const auto 关系 =
                                    特征概念服务_.比较单特征概念(
                                        *按值查找.概念节点,
                                        *预计.变化.预期值域概念节点);
                                if (关系.状态
                                        == 特征概念规则状态::已完成
                                    && 关系.关系) {
                                    const bool 包含 = *关系.关系
                                            == 单特征概念关系::同一
                                        || *关系.关系
                                            == 单特征概念关系::右为上位;
                                    单项 = 包含
                                        ? 新任务影响验证状态::已验证通过
                                        : 新任务影响验证状态::已验证不通过;
                                }
                            }
                            }
                        }
                    }
                    值域验证 = 合并验证(值域验证, 单项);
                }
            }
        }
        // 当前方法结果没有独立幅度规格；显式值域是现阶段唯一可验证的
        // 幅度边界。没有值域规格时幅度不适用，不能凭方向命中推导幅度。
        const auto 幅度验证 = 有值域要求
            ? 值域验证 : 新任务影响验证状态::不适用;
        const auto 多渠道验证 =
            执行->结果获取方案.渠道方案组.size() == 1
                && 执行->结果获取方案.多渠道关系
                    == 新任务多渠道关系::任一满足
            ? 新任务影响验证状态::不适用
            : 新任务影响验证状态::未验证;

        enum class 禁止项判定 : std::uint8_t {
            未命中 = 1, 已命中, 未验证, 冲突
        };
        const auto 判定禁止项 = [&](const 新任务禁止结果目标& 目标) {
            std::vector<const 新状态信息*> 候选状态;
            for (const auto& 实际 : *实际组) {
                if (实际.状态.被描述存在节点 == 目标.作用对象节点
                    && 实际.状态.来源实例特征节点 == 目标.实际特征节点) {
                    候选状态.push_back(&实际.状态);
                }
            }
            if (候选状态.empty()) return 禁止项判定::未验证;
            for (std::size_t i = 0; i < 候选状态.size(); ++i) {
                for (std::size_t j = i + 1; j < 候选状态.size(); ++j) {
                    if (候选状态[i]->强时间 == 候选状态[j]->强时间
                        && 候选状态[i]->固定准确值
                            != 候选状态[j]->固定准确值) {
                        return 禁止项判定::冲突;
                    }
                }
            }

            bool 有未验证 = false;
            for (const auto* 状态 : 候选状态) {
                bool 已确定不命中 = false;
                bool 本状态未知 = false;
                if (目标.禁止方向) {
                    const auto 比较 = 特征服务_.比较特征值(
                        目标.实际特征节点, 状态->固定准确值,
                        目标.动作前准确值);
                    if (比较.状态 != 新特征比较状态::已完成
                        || 比较.数值类型
                            != 新特征比较数值类型::有向差值
                        || !比较.比较值) {
                        本状态未知 = true;
                    } else {
                        const auto 差值 = *比较.比较值;
                        const bool 方向命中 =
                            (*目标.禁止方向 == 新任务预期变化方向::增加
                                    && 差值 > 0)
                            || (*目标.禁止方向 == 新任务预期变化方向::减少
                                    && 差值 < 0)
                            || (*目标.禁止方向 == 新任务预期变化方向::不变
                                    && 差值 == 0);
                        已确定不命中 = !方向命中;
                    }
                }
                if ((目标.禁止值域概念节点 || 目标.禁止精确数值域) && !已确定不命中
                    && std::holds_alternative<std::int64_t>(状态->固定准确值)) {
                    const auto 特征 = 特征服务_.获取特征(目标.实际特征节点);
                    const auto 命中 = 特征 ? 准确I64落在任务值域(特征服务_, 特征概念服务_,
                        目标.特征类型根概念节点, 特征->实例单位,
                        std::get<std::int64_t>(状态->固定准确值),
                        目标.禁止值域概念节点, 目标.禁止精确数值域) : std::nullopt;
                    if (!命中) 本状态未知 = true;
                    else 已确定不命中 = !*命中;
                } else if (目标.禁止精确数值域 && !已确定不命中) {
                    本状态未知 = true;
                } else if (目标.禁止值域概念节点 && !已确定不命中) {
                    const auto 域 = 特征概念服务_.获取特征概念(*目标.禁止值域概念节点);
                    const auto* 集合 = 域 ? std::get_if<特征概念任务集合值域>(&域->值域) : nullptr;
                    if (集合) {
                        const auto 特征 = 特征服务_.获取特征(目标.实际特征节点);
                        if (!特征 || 特征->实例单位 || 域->单位
                            || 取得特征概念类型根(特征概念服务_, 域->节点)
                                != 目标.特征类型根概念节点) {
                            本状态未知 = true;
                        } else {
                        const auto 比较 = 特征服务_.比较任务集合条件(
                            目标.特征类型根概念节点, 状态->固定准确值, *集合);
                        if (比较.状态 != 特征概念规则状态::已完成 || !比较.比较值)
                            本状态未知 = true;
                        else 已确定不命中 = *比较.比较值 == 0;
                        }
                    } else {
                    const auto 关系 = 特征概念服务_.比较单特征概念(
                        状态->特征类型概念节点,
                        *目标.禁止值域概念节点);
                    if (关系.状态 != 特征概念规则状态::已完成
                        || !关系.关系) {
                        本状态未知 = true;
                    } else {
                        const bool 值域命中 = *关系.关系
                                == 单特征概念关系::同一
                            || *关系.关系 == 单特征概念关系::右为上位;
                        已确定不命中 = !值域命中;
                    }
                    }
                }
                if (目标.禁止状态或动态概念节点) 本状态未知 = true;
                if (!已确定不命中 && !本状态未知) {
                    return 禁止项判定::已命中;
                }
                if (!已确定不命中 && 本状态未知) 有未验证 = true;
            }
            return 有未验证 ? 禁止项判定::未验证
                              : 禁止项判定::未命中;
        };

        新任务影响验证状态 禁止结果验证 =
            执行->结果获取方案.禁止结果组.empty()
            ? 新任务影响验证状态::不适用
            : 新任务影响验证状态::未验证;
        if (!执行->结果获取方案.禁止结果组.empty()) {
            std::vector<禁止项判定> 判定组;
            for (const auto& 目标 : 执行->结果获取方案.禁止结果组) {
                判定组.push_back(判定禁止项(目标));
            }
            const auto 有 = [&](禁止项判定 值) {
                return std::find(判定组.begin(), 判定组.end(), 值)
                    != 判定组.end();
            };
            const auto 全部 = [&](禁止项判定 值) {
                return std::all_of(判定组.begin(), 判定组.end(),
                    [&](禁止项判定 当前) { return 当前 == 值; });
            };
            if (执行->结果获取方案.禁止结果关系规则
                    == 新方法项关系规则::单项
                || 执行->结果获取方案.禁止结果关系规则
                    == 新方法项关系规则::任一成立) {
                禁止结果验证 = 有(禁止项判定::已命中)
                    ? 新任务影响验证状态::已验证不通过
                    : (有(禁止项判定::冲突)
                        ? 新任务影响验证状态::冲突
                        : (有(禁止项判定::未验证)
                            ? 新任务影响验证状态::未验证
                            : 新任务影响验证状态::已验证通过));
            } else if (执行->结果获取方案.禁止结果关系规则
                    == 新方法项关系规则::全部成立) {
                禁止结果验证 = 全部(禁止项判定::已命中)
                    ? 新任务影响验证状态::已验证不通过
                    : (有(禁止项判定::未命中)
                        ? 新任务影响验证状态::已验证通过
                        : (有(禁止项判定::冲突)
                            ? 新任务影响验证状态::冲突
                            : 新任务影响验证状态::未验证));
            }
        }
        const auto 副作用验证 = 计划外状态.empty()
            ? 新任务影响验证状态::已验证通过
            : 新任务影响验证状态::已验证不通过;
        const auto 必需变化数量 = std::accumulate(
            阶段组->begin(), 阶段组->end(), std::size_t{0},
            [](std::size_t 当前, const auto& 阶段) {
                return 当前 + static_cast<std::size_t>(std::count_if(
                    阶段.变化组.begin(), 阶段.变化组.end(),
                    [](const auto& 项) { return 项.变化.必须出现; }));
            });
        新任务影响验证状态 归因验证 = 新任务影响验证状态::不适用;
        if (必需变化数量 != 0) {
            if (必需命中状态.size() != 必需变化数量) {
                归因验证 = 新任务影响验证状态::已验证不通过;
            } else if (执行->动作输出状态节点组.empty()) {
                归因验证 = 新任务影响验证状态::未验证;
            } else {
                const bool 全部由本次动作输出 = std::all_of(
                    必需命中状态.begin(), 必需命中状态.end(),
                    [&](稳定编码 状态节点) {
                        return std::binary_search(
                            执行->动作输出状态节点组.begin(),
                            执行->动作输出状态节点组.end(), 状态节点);
                    });
                归因验证 = 全部由本次动作输出
                    ? 新任务影响验证状态::已验证通过
                    : 新任务影响验证状态::未验证;
            }
        }

        auto 需求结果 = 需求服务_.复核当前满足(任务->需求节点);
        const auto 需求定义 = 需求服务_.获取需求(任务->需求节点);
        if (!需求定义) {
            结果.状态 = 新任务操作状态::结构不一致;
            return 结果;
        }
        if (需求定义->目标.目标形式 == 新需求目标形式::变化方向) {
            if (需求结果.状态 == 新需求当前满足状态::资源失败) {
                结果.状态 = 新任务操作状态::资源失败;
                return 结果;
            }
            if (需求结果.状态
                    != 新需求当前满足状态::需要执行前后比较
                || !需求定义->目标.目标变化方向
                || !需求结果.实际特征节点) {
                结果.状态 = 新任务操作状态::结构不一致;
                return 结果;
            }
            // 需求方向与方法预测方向是两件事。只使用同一次执行冻结的
            // 目标前值和该目标最新观察后值，不能复用预测阶段的命中结果。
            const auto 目标位置 = std::find_if(
                执行->结果获取方案.目标组.begin(),
                执行->结果获取方案.目标组.end(),
                [&](const 新任务结果获取目标& 目标) {
                    return 目标.作用对象节点 == 需求定义->目标存在节点
                        && 目标.实际特征节点 == *需求结果.实际特征节点;
                });
            if (目标位置 != 执行->结果获取方案.目标组.end()) {
                const 新状态信息* 最新状态 = nullptr;
                for (const auto& 实际 : *实际组) {
                    const auto& 状态 = 实际.状态;
                    if (状态.被描述存在节点 != 目标位置->作用对象节点
                        || 状态.来源实例特征节点 != 目标位置->实际特征节点) {
                        continue;
                    }
                    if (!执行->结束时间
                        || 状态.强时间.语义 != 执行->开始时间.语义
                        || 状态.强时间.纳秒 < 执行->开始时间.纳秒
                        || 状态.强时间.纳秒 > 执行->结束时间->纳秒) {
                        结果.状态 = 新任务操作状态::结构不一致;
                        return 结果;
                    }
                    if (!最新状态
                        || 状态.强时间.纳秒 > 最新状态->强时间.纳秒) {
                        最新状态 = &状态;
                    }
                }
                if (最新状态) {
                    for (const auto& 实际 : *实际组) {
                        const auto& 状态 = 实际.状态;
                        if (状态.被描述存在节点 == 目标位置->作用对象节点
                            && 状态.来源实例特征节点 == 目标位置->实际特征节点
                            && 状态.强时间 == 最新状态->强时间
                            && 状态.固定准确值 != 最新状态->固定准确值) {
                            结果.状态 = 新任务操作状态::结构不一致;
                            return 结果;
                        }
                    }
                    const auto 比较 = 特征服务_.比较特征值(
                        目标位置->实际特征节点, 最新状态->固定准确值,
                        目标位置->动作前准确值);
                    if (比较.状态 == 新特征比较状态::资源失败) {
                        结果.状态 = 新任务操作状态::资源失败;
                        return 结果;
                    }
                    if (比较.状态 == 新特征比较状态::已完成
                        && 比较.数值类型 == 新特征比较数值类型::有向差值
                        && 比较.比较值) {
                        const bool 需要增加 = *需求定义->目标.目标变化方向
                            == 新需求变化方向::增加;
                        const bool 满足 = 需要增加
                            ? *比较.比较值 > 0 : *比较.比较值 < 0;
                        需求结果.状态 = 满足 ? 新需求当前满足状态::已满足
                            : (需要增加 ? 新需求当前满足状态::需增加
                                        : 新需求当前满足状态::需减少);
                    } else {
                        需求结果.状态 = 新需求当前满足状态::比较关系不适用;
                    }
                }
            }
        }
        std::vector<稳定编码> 评估状态引用 = 命中状态;
        评估状态引用.insert(评估状态引用.end(),
            计划外状态.begin(), 计划外状态.end());
        std::sort(评估状态引用.begin(), 评估状态引用.end());
        if (std::adjacent_find(评估状态引用.begin(),
                评估状态引用.end()) != 评估状态引用.end()) {
            结果.状态 = 新任务操作状态::结构不一致;
            return 结果;
        }
        std::vector<稳定编码> 已增加状态引用;
        const auto 引用结果 = 增加状态引用组(
            评估状态引用, 已增加状态引用);
        if (引用结果 != 新任务操作状态::已修改) {
            结果.状态 = 引用结果;
            return 结果;
        }

        const auto 评估节点 = 全局基础数据集.新建节点();
        if (!有效(评估节点)) {
            结果.状态 = 撤销增加状态引用组(已增加状态引用)
                ? 新任务操作状态::资源失败
                : 新任务操作状态::结构不一致;
            return 结果;
        }
        std::vector<稳定编码> 已写字段;
        const auto 回滚未发布评估 = [&]() noexcept {
            回滚字段(已写字段);
            (void)全局基础数据集.删除节点(评估节点);
            return 撤销增加状态引用组(已增加状态引用);
        };
        bool 完整 =
            写入节点字段(评估节点, 节点类型字段,
                任务影响评估节点类型, 已写字段)
            && 写入节点字段(评估节点, 评估所属任务字段,
                任务节点, 已写字段)
            && 写入节点字段(评估节点, 评估需求字段,
                任务->需求节点, 已写字段)
            && 写入节点字段(评估节点, 评估执行实例字段,
                执行实例节点, 已写字段)
            && 写入节点字段(评估节点, 评估方法字段,
                执行->方法节点, 已写字段)
            && 写入I64字段(评估节点, 评估需求结果状态字段,
                static_cast<std::int64_t>(需求结果.状态), 已写字段)
            && 写入I64字段(评估节点, 评估方向验证字段,
                static_cast<std::int64_t>(方向验证), 已写字段)
            && 写入I64字段(评估节点, 评估幅度验证字段,
                static_cast<std::int64_t>(幅度验证), 已写字段)
            && 写入I64字段(评估节点, 评估值域验证字段,
                static_cast<std::int64_t>(值域验证), 已写字段)
            && 写入I64字段(评估节点, 评估多渠道验证字段,
                static_cast<std::int64_t>(多渠道验证), 已写字段)
            && 写入I64字段(评估节点, 评估禁止结果验证字段,
                static_cast<std::int64_t>(禁止结果验证), 已写字段)
            && 写入I64字段(评估节点, 评估副作用验证字段,
                static_cast<std::int64_t>(副作用验证), 已写字段)
            && 写入I64字段(评估节点, 评估归因验证字段,
                static_cast<std::int64_t>(归因验证), 已写字段);
        if (完整 && 需求结果.准确差异) {
            完整 = 写入I64字段(评估节点, 评估需求准确差异字段,
                *需求结果.准确差异, 已写字段);
        }
        if (完整 && 需求结果.实际特征节点) {
            完整 = 写入节点字段(评估节点, 评估需求实际特征字段,
                *需求结果.实际特征节点, 已写字段);
        }
        for (const auto 状态 : 命中状态) {
            if (完整) 完整 = 写入节点字段(评估节点,
                评估符合预测状态字段, 状态, 已写字段);
        }
        for (const auto 项 : 缺失项) {
            if (完整) 完整 = 写入节点字段(评估节点,
                评估缺失变化项字段, 项, 已写字段);
        }
        for (const auto 状态 : 计划外状态) {
            if (完整) 完整 = 写入节点字段(评估节点,
                评估计划外状态字段, 状态, 已写字段);
        }
        if (完整) 完整 = 写入节点字段(执行实例节点,
            执行评估字段, 评估节点, 已写字段);
        if (完整) 完整 = 写入节点字段(任务节点,
            任务影响评估字段, 评估节点, 已写字段);
        const auto 执行状态 = 读取唯一I64字段记录(
            执行实例节点, 执行状态字段);
        if (!完整 || !执行状态
            || !全局基础数据集.修改字段值(
                执行状态->关系,
                基础值{static_cast<std::int64_t>(新任务执行状态::已评估)})) {
            结果.状态 = 回滚未发布评估()
                ? 新任务操作状态::资源失败
                : 新任务操作状态::结构不一致;
            return 结果;
        }

        const auto 虚拟存在 = 读取任务虚拟存在信息(任务节点, 特征服务_);
        if (!虚拟存在) {
            (void)全局基础数据集.修改字段值(
                执行状态->关系,
                基础值{static_cast<std::int64_t>(
                    新任务执行状态::已结束待评估)});
            结果.状态 = 回滚未发布评估()
                ? 新任务操作状态::任务虚拟存在缺失
                : 新任务操作状态::结构不一致;
            return 结果;
        }
        const auto 状态结果 = 写入任务状态(
            *虚拟存在, 新任务状态::等待自我决议, 特征服务_);
        if (状态结果 != 新任务操作状态::已修改) {
            (void)全局基础数据集.修改字段值(
                执行状态->关系,
                基础值{static_cast<std::int64_t>(
                    新任务执行状态::已结束待评估)});
            结果.状态 = 回滚未发布评估()
                ? 状态结果 : 新任务操作状态::结构不一致;
            return 结果;
        }

        const auto 读回 = 读取评估信息(评估节点);
        if (!读回) {
            结果.状态 = 新任务操作状态::结构不一致;
            return 结果;
        }
        结果.状态 = 新任务操作状态::已完成评估;
        结果.评估 = *读回;
        return 结果;
    } catch (...) {
        结果.状态 = 新任务操作状态::资源失败;
        return 结果;
    }
}

新任务完成结算结果 新_任务类::结算已完成任务(
    稳定编码 任务节点,
    稳定编码 执行实例节点,
    稳定编码 影响评估节点) noexcept {
    新任务完成结算结果 结果;
    if (!有效(任务节点) || !有效(执行实例节点)
        || !有效(影响评估节点)) return 结果;
    try {
        std::lock_guard 锁(新任务互斥);
        if (!结构仍存在()) {
            结果.状态 = 新任务操作状态::尚未初始化;
            return 结果;
        }
        const auto 任务 = 读取任务信息(任务节点, 特征服务_);
        const auto 执行 = 读取执行信息(执行实例节点, 状态服务_, 特征服务_, 特征概念服务_);
        const auto 评估 = 读取评估信息(影响评估节点);
        if (!任务) {
            结果.状态 = 新任务操作状态::任务不存在;
            return 结果;
        }
        if (!执行) {
            结果.状态 = 新任务操作状态::执行实例不存在;
            return 结果;
        }
        if (!评估 || 执行->任务节点 != 任务节点
            || 评估->任务节点 != 任务节点
            || 评估->执行实例节点 != 执行实例节点
            || !执行->影响评估节点
            || *执行->影响评估节点 != 影响评估节点
            || !任务->最近执行实例节点
            || *任务->最近执行实例节点 != 执行实例节点
            || !评估->允许形成完成决议()
            || 任务->当前状态 != 新任务状态::已完成) {
            结果.状态 = 新任务操作状态::任务状态不允许;
            return 结果;
        }
        auto 结果闭包 = 执行->实际状态节点组;
        std::sort(结果闭包.begin(), 结果闭包.end(), [](稳定编码 左, 稳定编码 右) {
            return 左.值 < 右.值;
        });
        if (结果闭包.empty()
            || std::adjacent_find(结果闭包.begin(), 结果闭包.end())
                != 结果闭包.end()
            || std::any_of(结果闭包.begin(), 结果闭包.end(),
                [&](稳定编码 状态节点) {
                    return !状态服务_.获取状态(状态节点).has_value();
                })) {
            结果.状态 = 新任务操作状态::结构不一致;
            return 结果;
        }
        if (任务->完成结算节点) {
            const auto 已有 = 读取完成结算信息(
                *任务->完成结算节点, 状态服务_);
            if (!已有) {
                结果.状态 = 新任务操作状态::结构不一致;
                return 结果;
            }
            if (已有->任务节点 != 任务节点
                || 已有->需求节点 != 任务->需求节点
                || 已有->执行实例节点 != 执行实例节点
                || 已有->影响评估节点 != 影响评估节点
                || 已有->完成来源 != 新任务完成来源::自我后继决议
                || 已有->结果状态节点组 != 结果闭包) {
                结果.状态 = 新任务操作状态::结构不一致;
                return 结果;
            }
            结果.状态 = 新任务操作状态::精确重复;
            结果.结算 = *已有;
            return 结果;
        }

        std::vector<稳定编码> 已增加状态引用;
        const auto 引用结果 = 增加状态引用组(
            结果闭包, 已增加状态引用);
        if (引用结果 != 新任务操作状态::已修改) {
            结果.状态 = 引用结果;
            return 结果;
        }

        const auto 结算节点 = 全局基础数据集.新建节点();
        if (!有效(结算节点)) {
            结果.状态 = 撤销增加状态引用组(已增加状态引用)
                ? 新任务操作状态::资源失败
                : 新任务操作状态::结构不一致;
            return 结果;
        }
        std::vector<稳定编码> 已写字段;
        const bool 完整 =
            写入节点字段(结算节点, 节点类型字段,
                任务完成结算节点类型, 已写字段)
            && 写入节点字段(结算节点, 结算所属任务字段,
                任务节点, 已写字段)
            && 写入节点字段(结算节点, 结算需求字段,
                任务->需求节点, 已写字段)
            && 写入节点字段(结算节点, 结算执行实例字段,
                执行实例节点, 已写字段)
            && 写入节点字段(结算节点, 结算影响评估字段,
                影响评估节点, 已写字段)
            && 写入I64字段(结算节点, 结算完成来源字段,
                static_cast<std::int64_t>(新任务完成来源::自我后继决议),
                已写字段)
            && std::all_of(结果闭包.begin(), 结果闭包.end(),
                [&](稳定编码 状态节点) {
                    return 写入节点字段(结算节点, 结算结果状态字段,
                        状态节点, 已写字段);
                })
            && 写入节点字段(任务节点, 任务完成结算字段,
                结算节点, 已写字段);
        if (!完整) {
            回滚字段(已写字段);
            (void)全局基础数据集.删除节点(结算节点);
            结果.状态 = 撤销增加状态引用组(已增加状态引用)
                ? 新任务操作状态::资源失败
                : 新任务操作状态::结构不一致;
            return 结果;
        }
        const auto 读回 = 读取完成结算信息(结算节点, 状态服务_);
        const auto 任务读回 = 读取任务信息(任务节点, 特征服务_);
        if (!读回 || !任务读回 || !任务读回->完成结算节点
            || *任务读回->完成结算节点 != 结算节点) {
            结果.状态 = 新任务操作状态::结构不一致;
            return 结果;
        }
        结果.状态 = 新任务操作状态::已建立;
        结果.结算 = *读回;
        return 结果;
    } catch (...) {
        结果.状态 = 新任务操作状态::资源失败;
        return 结果;
    }
}

新需求当前满足结果 新_任务类::复核任务需求(
    稳定编码 任务节点) const noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        const auto 任务 = 读取任务信息(任务节点, 特征服务_);
        if (!任务) return {};
        return 需求服务_.复核当前满足(任务->需求节点);
    } catch (...) {
        新需求当前满足结果 结果;
        结果.状态 = 新需求当前满足状态::资源失败;
        return 结果;
    }
}

bool 新_任务类::初始化治理() noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        if (!有效(需求树根节点)) return false;
        治理已初始化_ = 初始化();
        return 治理已初始化_;
    } catch (...) { return false; }
}

新任务列表结果 新_任务类::查询可查找方法任务() const noexcept {
    新任务列表结果 结果;
    try {
        std::lock_guard 锁(新任务互斥);
        if (!治理已初始化_) {
            结果.状态 = 新任务操作状态::尚未初始化;
            return 结果;
        }
        结果 = 查询活动任务();
        if (结果.状态 != 新任务操作状态::已找到) return 结果;
        std::vector<稳定编码> 可查找;
        for (const auto 任务节点 : 结果.任务节点组) {
            const auto 任务 = 获取任务(任务节点);
            if (!任务) {
                结果.状态 = 新任务操作状态::结构不一致;
                结果.任务节点组.clear();
                return 结果;
            }
            if (任务->当前状态 == 新任务状态::已建立
                || 任务->当前状态 == 新任务状态::待查找方法) {
                可查找.push_back(任务节点);
            }
        }
        结果.任务节点组 = std::move(可查找);
        return 结果;
    } catch (...) {
        结果.状态 = 新任务操作状态::资源失败;
        结果.任务节点组.clear();
        return 结果;
    }
}

新任务列表结果 新_任务类::查询可检查条件任务() const noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        if (!治理已初始化_) {
            新任务列表结果 结果;
            结果.状态 = 新任务操作状态::尚未初始化;
            return 结果;
        }
        return 查询指定状态任务(新任务状态::待检查条件);
    } catch (...) {
        新任务列表结果 结果;
        结果.状态 = 新任务操作状态::资源失败;
        return 结果;
    }
}

std::optional<std::vector<新任务治理执行读回结果>>
新_任务类::查询等待结果任务执行() const noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        if (!治理已初始化_) return std::nullopt;
        const auto 任务组 = 查询全部任务();
        if (任务组.状态 != 新任务操作状态::已找到) {
            return std::nullopt;
        }

        std::vector<新任务治理执行读回结果> 结果组;
        结果组.reserve(任务组.任务节点组.size());
        for (const auto 任务节点 : 任务组.任务节点组) {
            const auto 任务 = 获取任务(任务节点);
            if (任务 && 任务->当前状态 == 新任务状态::已完成
                && 任务->完成结算节点) {
                continue;
            }
            if (任务 && 任务->当前状态 != 新任务状态::执行中
                && 任务->当前状态 != 新任务状态::待影响评估
                && 任务->当前状态 != 新任务状态::等待自我决议
                && 任务->当前状态 != 新任务状态::已完成) {
                continue;
            }
            if (!任务 || !任务->最近执行实例节点) return std::nullopt;
            const auto 执行 = 获取执行实例(
                *任务->最近执行实例节点);
            if (!执行 || 执行->任务节点 != 任务节点) {
                return std::nullopt;
            }
            新任务治理执行读回结果 读回;
            读回.任务 = *任务;
            读回.执行 = *执行;
            if (任务->当前状态 == 新任务状态::执行中
                && 执行->状态 == 新任务执行状态::执行中
                && !执行->结束时间 && !执行->影响评估节点) {
                读回.状态 = 新任务治理执行读回状态::已读回等待观察;
            } else if (任务->当前状态 == 新任务状态::待影响评估
                && 执行->状态 == 新任务执行状态::已结束待评估
                && 执行->结束时间 && !执行->影响评估节点) {
                读回.状态 = 新任务治理执行读回状态::已读回等待评估;
            } else if ((任务->当前状态 == 新任务状态::等待自我决议
                    || (任务->当前状态 == 新任务状态::已完成
                        && !任务->完成结算节点))
                && 执行->状态 == 新任务执行状态::已评估
                && 执行->结束时间 && 执行->影响评估节点) {
                读回.影响评估 = 获取影响评估(
                    *执行->影响评估节点);
                if (!读回.影响评估
                    || 读回.影响评估->任务节点 != 任务节点
                    || 读回.影响评估->执行实例节点 != 执行->节点) {
                    return std::nullopt;
                }
                读回.状态 = 新任务治理执行读回状态::已读回影响评估;
            } else {
                return std::nullopt;
            }
            结果组.push_back(std::move(读回));
        }
        return 结果组;
    } catch (...) {
        return std::nullopt;
    }
}

新任务操作状态 新_任务类::准备任务查找方法(
    稳定编码 任务节点) noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        if (!治理已初始化_) {
            return 新任务操作状态::尚未初始化;
        }
        const auto 任务 = 获取任务(任务节点);
        if (!任务) return 新任务操作状态::任务不存在;
        if (任务->当前状态 == 新任务状态::待查找方法) {
            return 新任务操作状态::无变化;
        }
        if (任务->当前状态 != 新任务状态::已建立) {
            return 新任务操作状态::任务状态不允许;
        }
        return 修改任务状态(
            任务节点, 新任务状态::待查找方法);
    } catch (...) {
        return 新任务操作状态::资源失败;
    }
}

namespace {

新任务前置方法状态 核验前置筹办任务状态(
    const 新_任务类& 服务, const 新任务信息& 任务) {
    switch (任务.当前状态) {
    case 新任务状态::已建立:
    case 新任务状态::待查找方法:
    case 新任务状态::待检查条件:
    case 新任务状态::等待条件: break;
    default: return 新任务前置方法状态::任务状态不允许;
    }
    if (任务.最近执行实例节点) {
        const auto 执行 = 服务.获取执行实例(*任务.最近执行实例节点);
        if (!执行 || 执行->任务节点 != 任务.节点)
            return 新任务前置方法状态::结构不一致;
        if (执行->状态 != 新任务执行状态::已评估)
            return 新任务前置方法状态::任务状态不允许;
    }
    return 新任务前置方法状态::已读回;
}

// 只核验引用及目标形状，不推导前置目标足以推动原需求，也不批准动作。
新任务前置方法状态 核验前置方法目标(
    const 新任务前置方法目标& 目标, const 新任务信息& 任务,
    const 新_需求类& 需求服务, const 新_存在类& 存在服务,
    const 概念_特征类& 特征概念, const 概念_二次特征类& 二次概念) {
    if (!有效(目标.任务节点) || !有效(目标.需求节点)
        || !有效(目标.原需求目标存在节点) || !有效(目标.前置目标存在节点))
        return 新任务前置方法状态::入口拒绝;
    if (任务.节点 != 目标.任务节点 || 任务.需求节点 != 目标.需求节点)
        return 新任务前置方法状态::原需求已变化;
    const auto 需求 = 需求服务.获取需求(目标.需求节点);
    if (!需求) return 新任务前置方法状态::需求不存在;
    if (需求->目标存在节点 != 目标.原需求目标存在节点
        || 需求->目标 != 目标.原需求目标)
        return 新任务前置方法状态::原需求已变化;
    const auto& 前置 = 目标.前置目标;
    if (!存在服务.是存在节点(目标.前置目标存在节点)
        || !有效(前置.目标特征类型概念节点)
        || 取得特征概念类型根(特征概念, 前置.目标特征类型概念节点)
            != 前置.目标特征类型概念节点)
        return 新任务前置方法状态::入口拒绝;
    if (前置.目标形式 == 新需求目标形式::变化方向) {
        if (前置.目标值域概念节点 || 前置.目标比较关系概念节点
            || !前置.目标变化方向
            || (*前置.目标变化方向 != 新需求变化方向::增加
                && *前置.目标变化方向 != 新需求变化方向::减少))
            return 新任务前置方法状态::入口拒绝;
    } else if (前置.目标形式 == 新需求目标形式::值) {
        if (前置.目标变化方向 || !前置.目标值域概念节点
            || !前置.目标比较关系概念节点
            || !有效(*前置.目标值域概念节点)
            || !有效(*前置.目标比较关系概念节点)
            || 取得特征概念类型根(特征概念, *前置.目标值域概念节点)
                != 前置.目标特征类型概念节点
            || !二次概念.是二次特征概念(*前置.目标比较关系概念节点))
            return 新任务前置方法状态::入口拒绝;
    } else return 新任务前置方法状态::入口拒绝;
    return 新任务前置方法状态::已读回;
}

// 方法头可召回不等于条件结果已形成；此处不筛除零配对的方法。
新任务前置方法状态 查询前置方法完整候选(
    const 新_方法类& 方法服务, 稳定编码 特征类型,
    std::vector<稳定编码>& 候选) {
    新方法候选查询 查询;
    查询.涉及特征类型概念组 = {特征类型};
    auto 结果 = 方法服务.查询候选方法(查询);
    if (结果.状态 != 新方法操作状态::已找到) {
        if (结果.状态 == 新方法操作状态::资源失败)
            return 新任务前置方法状态::资源失败;
        if (结果.状态 == 新方法操作状态::结构不一致)
            return 新任务前置方法状态::结构不一致;
        return 新任务前置方法状态::方法查询未完成;
    }
    std::optional<稳定编码> 上一个;
    for (const auto 节点 : 结果.方法节点组) {
        if (!有效(节点) || (上一个 && !(*上一个 < 节点)))
            return 新任务前置方法状态::结构不一致;
        上一个 = 节点;
        const auto 方法 = 方法服务.获取方法(节点);
        // optional 读取不区分已删除与读取失败；不可将不可解释的空值当可刷新候选。
        if (!方法 || 方法->节点 != 节点 || 方法->来源方法节点 || 方法->实例根节点)
            return 新任务前置方法状态::结构不一致;
        if (方法->启用状态 != 新方法启用状态::启用
            || std::find(方法->方法头.涉及特征类型概念组.begin(),
                方法->方法头.涉及特征类型概念组.end(), 特征类型)
                == 方法->方法头.涉及特征类型概念组.end())
            return 新任务前置方法状态::候选已变化;
    }
    候选 = std::move(结果.方法节点组);
    return 新任务前置方法状态::已读回;
}

} // namespace

新任务前置方法结果 新_任务类::查询并暂持任务前置方法(
    const 新任务前置方法请求& 请求) noexcept {
    using 状态 = 新任务前置方法状态;
    try {
        std::lock_guard 锁(新任务互斥);
        if (!治理已初始化_ || !结构仍存在()) return {状态::尚未初始化, {}};
        const auto 任务 = 获取任务(请求.目标.任务节点);
        if (!任务) return {状态::任务不存在, {}};
        auto 核验 = 核验前置筹办任务状态(*this, *任务);
        if (核验 != 状态::已读回) return {核验, {}};
        核验 = 核验前置方法目标(请求.目标, *任务, 需求服务_, 存在服务_,
            特征概念服务_, 二次特征概念服务_);
        if (核验 != 状态::已读回) return {核验, {}};
        const auto 槽位 = 前置方法筹办材料组_.find(任务->节点);
        if (槽位 != 前置方法筹办材料组_.end() && !槽位->second)
            return {状态::结构不一致, {}};
        if (槽位 == 前置方法筹办材料组_.end() ? 请求.预期现有材料.has_value()
            : (!请求.预期现有材料 || *槽位->second != *请求.预期现有材料))
            return {状态::槽位已变化, {}};

        新任务前置方法材料 材料{请求.目标, {}};
        核验 = 查询前置方法完整候选(方法服务_,
            请求.目标.前置目标.目标特征类型概念节点, 材料.候选方法节点组);
        if (核验 != 状态::已读回) return {核验, {}};
        // 需求owner有自己的锁；查询后再次核对，不能以早先读值盲写。
        const auto 当前任务 = 获取任务(请求.目标.任务节点);
        if (!当前任务) return {状态::任务不存在, {}};
        核验 = 核验前置筹办任务状态(*this, *当前任务);
        if (核验 != 状态::已读回) return {核验, {}};
        核验 = 核验前置方法目标(请求.目标, *当前任务, 需求服务_, 存在服务_,
            特征概念服务_, 二次特征概念服务_);
        if (核验 != 状态::已读回) return {核验, {}};
        if (槽位 != 前置方法筹办材料组_.end() && *槽位->second == 材料)
            return {状态::精确重复, std::move(材料)};

        // 所有可能分配先完成；已有槽仅交换不可变指针，新槽插入有强异常保证。
        新任务前置方法结果 结果{状态::已暂持, 材料};
        auto 新材料 = std::make_shared<const 新任务前置方法材料>(std::move(材料));
        if (槽位 == 前置方法筹办材料组_.end())
            前置方法筹办材料组_.emplace(任务->节点, std::move(新材料));
        else 槽位->second = std::move(新材料);
        return 结果;
    } catch (const std::bad_alloc&) { return {状态::资源失败, {}};
    } catch (...) { return {状态::结构不一致, {}}; }
}

新任务前置方法结果 新_任务类::读取任务前置方法(稳定编码 任务节点) const noexcept {
    using 状态 = 新任务前置方法状态;
    try {
        std::lock_guard 锁(新任务互斥);
        if (!治理已初始化_ || !结构仍存在()) return {状态::尚未初始化, {}};
        const auto 任务 = 获取任务(任务节点);
        if (!任务) return {状态::任务不存在, {}};
        const auto 槽位 = 前置方法筹办材料组_.find(任务节点);
        if (槽位 == 前置方法筹办材料组_.end()) return {状态::未暂持, {}};
        if (!槽位->second) return {状态::结构不一致, {}};
        auto 核验 = 核验前置筹办任务状态(*this, *任务);
        if (核验 != 状态::已读回) return {核验, {}};
        核验 = 核验前置方法目标(槽位->second->目标, *任务, 需求服务_, 存在服务_,
            特征概念服务_, 二次特征概念服务_);
        if (核验 == 状态::原需求已变化) return {核验, *槽位->second};
        if (核验 != 状态::已读回) return {核验, {}};
        std::vector<稳定编码> 候选;
        核验 = 查询前置方法完整候选(方法服务_,
            槽位->second->目标.前置目标.目标特征类型概念节点, 候选);
        if (核验 == 状态::候选已变化) return {核验, *槽位->second};
        if (核验 != 状态::已读回) return {核验, {}};
        if (候选 != 槽位->second->候选方法节点组)
            return {状态::候选已变化, *槽位->second};
        // 查询结果只是此时投影；实际采用必须重新读回方法条件结果。
        return {状态::已读回, *槽位->second};
    } catch (const std::bad_alloc&) { return {状态::资源失败, {}};
    } catch (...) { return {状态::结构不一致, {}}; }
}

新任务前置方法结果 新_任务类::清除任务前置方法(
    稳定编码 任务节点, const 新任务前置方法材料& 预期材料) noexcept {
    using 状态 = 新任务前置方法状态;
    try {
        std::lock_guard 锁(新任务互斥);
        if (!治理已初始化_ || !结构仍存在()) return {状态::尚未初始化, {}};
        const auto 任务 = 获取任务(任务节点);
        if (!任务) return {状态::任务不存在, {}};
        const auto 核验 = 核验前置筹办任务状态(*this, *任务);
        if (核验 != 状态::已读回) return {核验, {}};
        if (预期材料.目标.任务节点 != 任务节点) return {状态::入口拒绝, {}};
        const auto 槽位 = 前置方法筹办材料组_.find(任务节点);
        if (槽位 == 前置方法筹办材料组_.end()) return {状态::未暂持, {}};
        if (!槽位->second) return {状态::结构不一致, {}};
        if (*槽位->second != 预期材料) return {状态::槽位已变化, {}};
        前置方法筹办材料组_.erase(槽位);
        return {状态::已清除, {}};
    } catch (const std::bad_alloc&) { return {状态::资源失败, {}};
    } catch (...) { return {状态::结构不一致, {}}; }
}

新任务治理方法召回结果 新_任务类::查询任务方法候选并发布(
    稳定编码 任务节点) noexcept {
    新任务治理方法召回结果 结果;
    结果.任务节点 = 任务节点;
    try {
        std::lock_guard 锁(新任务互斥);
        if (!治理已初始化_ || !有效(新方法组织根节点)) {
            结果.状态 = 新任务治理方法召回状态::尚未初始化;
            return 结果;
        }
        const auto 任务 = 获取任务(任务节点);
        if (!任务) {
            结果.状态 = 新任务治理方法召回状态::任务不存在;
            return 结果;
        }
        结果.需求节点 = 任务->需求节点;
        if (任务->当前状态 != 新任务状态::待查找方法) {
            结果.状态 = 新任务治理方法召回状态::任务状态已变化;
            return 结果;
        }
        const auto 需求 = 需求服务_.获取需求(任务->需求节点);
        if (!需求) {
            结果.状态 = 新任务治理方法召回状态::需求不存在;
            return 结果;
        }

        新方法候选查询 查询;
        查询.涉及特征类型概念组.push_back(
            需求->目标.目标特征类型概念节点);
        结果.方法查询 = 方法服务_.查询候选方法(查询);
        if (结果.方法查询->状态 != 新方法操作状态::已找到) {
            结果.状态 = 结果.方法查询->状态 == 新方法操作状态::资源失败
                ? 新任务治理方法召回状态::资源失败
                : (结果.方法查询->状态 == 新方法操作状态::结构不一致
                    ? 新任务治理方法召回状态::结构不一致
                    : 新任务治理方法召回状态::方法查询未完成);
            return 结果;
        }
        for (const auto 方法节点 : 结果.方法查询->方法节点组) {
            if (!方法服务_.是方法节点(方法节点)) {
                结果.状态 = 新任务治理方法召回状态::结构不一致;
                return 结果;
            }
        }

        新任务候选方法发布请求 发布;
        发布.任务节点 = 任务节点;
        发布.需求节点 = 任务->需求节点;
        发布.需求目标 = 需求->目标;
        发布.查询 = 查询;
        发布.查询状态 = 结果.方法查询->状态;
        发布.查询完整 = true;
        发布.候选方法节点组 = 结果.方法查询->方法节点组;
        结果.发布已尝试 = true;
        结果.发布结果 = 发布候选方法材料(std::move(发布));
        if ((结果.发布结果->状态 != 新任务操作状态::已建立
                && 结果.发布结果->状态 != 新任务操作状态::已修改
                && 结果.发布结果->状态 != 新任务操作状态::精确重复)
            || !结果.发布结果->材料节点) {
            结果.状态 = 结果.发布结果->状态 == 新任务操作状态::资源失败
                ? 新任务治理方法召回状态::资源失败
                : (结果.发布结果->状态 == 新任务操作状态::任务状态不允许
                    ? 新任务治理方法召回状态::任务状态已变化
                    : 新任务治理方法召回状态::候选发布未完成);
            return 结果;
        }
        结果.候选材料节点 = 结果.发布结果->材料节点;
        结果.状态 = 结果.方法查询->方法节点组.empty()
            ? 新任务治理方法召回状态::已发布完整无候选
            : 新任务治理方法召回状态::已发布有候选;
        return 结果;
    } catch (...) {
        结果.状态 = 新任务治理方法召回状态::资源失败;
        return 结果;
    }
}

新任务治理候选读回结果 新_任务类::读回任务方法候选(
    稳定编码 任务节点, 稳定编码 材料节点) const noexcept {
    新任务治理候选读回结果 结果;
    结果.任务节点 = 任务节点;
    结果.材料节点 = 材料节点;
    try {
        std::lock_guard 锁(新任务互斥);
        if (!治理已初始化_) {
            结果.状态 = 新任务治理候选读回状态::尚未初始化;
            return 结果;
        }
        结果.任务 = 获取任务(任务节点);
        if (!结果.任务) {
            结果.状态 = 新任务治理候选读回状态::任务不存在;
            return 结果;
        }
        if (结果.任务->当前状态 != 新任务状态::待查找方法) {
            结果.状态 = 新任务治理候选读回状态::任务状态已变化;
            return 结果;
        }
        if (结果.任务->当前候选方法材料节点 != 材料节点) {
            结果.状态 = 新任务治理候选读回状态::材料不存在;
            return 结果;
        }
        结果.材料 = 获取候选方法材料(材料节点);
        if (!结果.材料) {
            结果.状态 = 新任务治理候选读回状态::材料不存在;
            return 结果;
        }
        const auto 需求 = 需求服务_.获取需求(结果.任务->需求节点);
        if (!需求 || 结果.材料->任务节点 != 任务节点
            || 结果.材料->需求节点 != 结果.任务->需求节点
            || 结果.材料->需求目标 != 需求->目标
            || 结果.材料->查询状态 != 新方法操作状态::已找到
            || !结果.材料->查询完整
            || 结果.材料->查询.作用域
            || 结果.材料->查询.实现方式
            || !结果.材料->查询.作用存在概念组.empty()
            || !结果.材料->查询.可能结果概念组.empty()
            || 结果.材料->查询.涉及特征类型概念组
                != std::vector<稳定编码>{需求->目标.目标特征类型概念节点}) {
            结果.状态 = 新任务治理候选读回状态::结构不一致;
            return 结果;
        }
        for (const auto 方法节点 : 结果.材料->候选方法节点组) {
            if (!方法服务_.是方法节点(方法节点)) {
                结果.状态 = 新任务治理候选读回状态::结构不一致;
                return 结果;
            }
        }
        结果.状态 = 结果.材料->候选方法节点组.empty()
            ? 新任务治理候选读回状态::已读回完整无候选
            : 新任务治理候选读回状态::已读回有候选;
        return 结果;
    } catch (...) {
        结果.状态 = 新任务治理候选读回状态::资源失败;
        return 结果;
    }
}

新任务治理候选消费结果 新_任务类::消费任务方法候选(
    稳定编码 任务节点, 稳定编码 材料节点) noexcept {
    新任务治理候选消费结果 结果;
    try {
        std::lock_guard 锁(新任务互斥);
        结果.读回 = 读回任务方法候选(任务节点, 材料节点);
        if (结果.读回.状态 == 新任务治理候选读回状态::任务状态已变化) {
            结果.状态 = 新任务治理候选消费状态::任务状态已变化;
            return 结果;
        }
        const bool 有候选 = 结果.读回.状态
            == 新任务治理候选读回状态::已读回有候选;
        const bool 完整无候选 = 结果.读回.状态
            == 新任务治理候选读回状态::已读回完整无候选;
        if (!有候选 && !完整无候选) {
            结果.状态 = 结果.读回.状态 == 新任务治理候选读回状态::尚未初始化
                ? 新任务治理候选消费状态::尚未初始化
                : (结果.读回.状态 == 新任务治理候选读回状态::资源失败
                    ? 新任务治理候选消费状态::资源失败
                    : 新任务治理候选消费状态::读回失败);
            return 结果;
        }
        const auto 目标状态 = 有候选
            ? 新任务状态::待检查条件 : 新任务状态::等待条件;
        结果.状态写入 = 修改任务状态(任务节点, 目标状态);
        if (*结果.状态写入 != 新任务操作状态::已修改
            && *结果.状态写入 != 新任务操作状态::无变化) {
            结果.状态 = *结果.状态写入 == 新任务操作状态::资源失败
                ? 新任务治理候选消费状态::资源失败
                : (*结果.状态写入 == 新任务操作状态::任务状态不允许
                    ? 新任务治理候选消费状态::任务状态已变化
                    : 新任务治理候选消费状态::状态写入失败);
            return 结果;
        }
        const auto 写后 = 获取任务(任务节点);
        if (!写后 || 写后->当前状态 != 目标状态) {
            结果.状态 = 新任务治理候选消费状态::状态写入失败;
            return 结果;
        }
        结果.状态 = 有候选
            ? 新任务治理候选消费状态::已进入条件检查
            : 新任务治理候选消费状态::已进入等待条件;
        return 结果;
    } catch (...) {
        结果.状态 = 新任务治理候选消费状态::资源失败;
        return 结果;
    }
}

新_方法类::新方法实例交付状态 新_任务类::切换学习方法实例(
    稳定编码 任务节点, 稳定编码 旧根, 稳定编码 新根) noexcept {
    using 状态 = 新_方法类::新方法实例交付状态;
    bool 尝试写入 = false;
    try {
        std::lock_guard 锁(新任务互斥);
        const auto t = 获取任务(任务节点);
        if (!t || t->方法实例节点 != 旧根 || t->当前状态 != 新任务状态::待检查条件
            || 方法实例采用待核查_.contains(任务节点)) return 状态::未采用;
        if (t->最近执行实例节点) {
            const auto x = 获取执行实例(*t->最近执行实例节点);
            if (!x || x->任务节点 != 任务节点 || x->状态 != 新任务执行状态::已评估) return 状态::未采用;
        }
        const auto 旧引用 = 全局基础数据集.查询目标字段(旧根, 任务方法实例字段);
        const auto 字段 = 全局基础数据集.查询字段(任务节点, 任务方法实例字段);
        if (旧引用.size() != 1 || 旧引用.front().所属节点 != 任务节点
            || 字段.size() != 1 || !std::holds_alternative<稳定编码>(字段.front().内容)
            || std::get<稳定编码>(字段.front().内容) != 旧根
            || !全局基础数据集.查询目标字段(新根, 任务方法实例字段).empty()
            || !方法服务_.获取混合方法实例(新根)) return 状态::未采用;
        // 先分配未知结果的保留位置；发布之后不再依赖一次分配成功。
        方法实例采用待核查_.emplace(任务节点, 新根);
        尝试写入 = true;
        const bool 已写 = 全局基础数据集.修改字段节点(字段.front().编码, 新根);
        const auto 读回 = 全局基础数据集.查询字段(任务节点, 任务方法实例字段);
        if (读回.size() == 1) {
            const auto* 根 = std::get_if<稳定编码>(&读回.front().内容);
            if (根 && *根 == 新根) { 方法实例采用待核查_.erase(任务节点); return 状态::已采用; }
            if (根 && *根 == 旧根 && !已写) { 方法实例采用待核查_.erase(任务节点); return 状态::未采用; }
        }
        return 状态::采用状态不明;
    } catch (...) { return 尝试写入 ? 状态::采用状态不明 : 状态::未采用; }
}

新任务执行组织结果 新_任务类::交付候选方法执行组织(稳定编码 任务节点) noexcept {
    try{
        std::optional<新任务信息> 原任务;std::optional<新任务候选方法材料> 材料;
        {
            std::lock_guard 锁(新任务互斥);原任务=获取任务(任务节点);
            if(!原任务)return {新任务执行组织状态::任务不存在,{}};
            if(原任务->当前状态!=新任务状态::待检查条件)return {新任务执行组织状态::任务状态不允许,{}};
            if(!原任务->当前候选方法材料节点)return {新任务执行组织状态::入口拒绝,{}};
            材料=获取候选方法材料(*原任务->当前候选方法材料节点);
            if(!材料||材料->任务节点!=任务节点||材料->需求节点!=原任务->需求节点
                ||!材料->查询完整||材料->查询状态!=新方法操作状态::已找到||材料->候选方法节点组.empty())throw std::logic_error("候选组织交付材料读取失败");
            const auto 需求=需求服务_.获取需求(原任务->需求节点);
            if(!需求||需求->目标!=材料->需求目标) return {新任务执行组织状态::任务状态不允许,{}};
        }
        auto 交付=增补任务方法执行组织(任务节点,材料->候选方法节点组);
        std::lock_guard 锁(新任务互斥);const auto 当前=获取任务(任务节点);
        if(!当前)throw std::logic_error("交付后的任务读取失败");
        const auto 当前材料 = 当前->当前候选方法材料节点 ? 获取候选方法材料(*当前->当前候选方法材料节点) : std::nullopt;
        const auto 需求=需求服务_.获取需求(当前->需求节点);
        if(当前->当前状态!=原任务->当前状态||当前->需求节点!=原任务->需求节点
            ||当前->当前候选方法材料节点!=原任务->当前候选方法材料节点||当前材料!=材料
            ||!需求||需求->目标!=材料->需求目标)return {新任务执行组织状态::任务状态不允许,交付.组织};
        return 交付;
    }catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"交付候选方法执行组织",L"任务组织交付工程故障");}
}
新任务操作状态 新_任务类::实例化候选方法(稳定编码 任务节点) noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        if (!治理已初始化_) return 新任务操作状态::尚未初始化;
        const auto 任务 = 获取任务(任务节点);
        if (!任务) return 新任务操作状态::任务不存在;
        if (任务->当前状态 != 新任务状态::待检查条件) return 新任务操作状态::任务状态不允许;
        if (方法实例采用待核查_.contains(任务节点)) return 新任务操作状态::结构不一致;
        if (!任务->当前候选方法材料节点) return 新任务操作状态::候选材料不存在;
        const auto 材料 = 获取候选方法材料(*任务->当前候选方法材料节点);
        const auto 需求 = 需求服务_.获取需求(任务->需求节点);
        if (!需求) return 新任务操作状态::需求不存在;
        if (!材料 || 材料->任务节点 != 任务节点 || 材料->需求节点 != 任务->需求节点
            || 材料->需求目标 != 需求->目标 || !材料->查询完整
            || 材料->查询状态 != 新方法操作状态::已找到 || 材料->候选方法节点组.empty())
            return 新任务操作状态::结构不一致;
        if (任务->方法实例节点) {
            const auto 旧根 = *任务->方法实例节点;
            const auto 增补 = 方法服务_.扩充并交付混合方法实例(旧根, 材料->候选方法节点组,
                [&](稳定编码 新根) noexcept { return 切换学习方法实例(任务节点, 旧根, 新根); });
            using 状态 = 新_方法类::新方法实例增补状态;
            if (增补.状态 == 状态::已扩充 && 增补.实例根节点) return 新任务操作状态::已修改;
            if (增补.状态 == 状态::精确重复 && 增补.实例根节点 == 旧根) return 新任务操作状态::精确重复;
            if (增补.状态 == 状态::资源失败) return 新任务操作状态::资源失败;
            return 新任务操作状态::结构不一致;
        }
        新任务操作状态 绑定状态 = 新任务操作状态::结构不一致;
        const auto 复制 = 方法服务_.建立并交付混合方法实例(材料->候选方法节点组,
            [&](稳定编码 根) noexcept {
                using 交付状态 = 新_方法类::新方法实例交付状态;
                try {
                    绑定状态 = 绑定方法实例(任务节点, 根);
                    const auto 字段 = 全局基础数据集.查询字段(任务节点, 任务方法实例字段);
                    if (字段.size() == 1) {
                        const auto* 引用 = std::get_if<稳定编码>(&字段.front().内容);
                        if (引用 && *引用 == 根) return 交付状态::已采用;
                    }
                    if (字段.empty() && 绑定状态 != 新任务操作状态::已修改
                        && 绑定状态 != 新任务操作状态::精确重复) return 交付状态::未采用;
                    return 交付状态::采用状态不明;
                } catch (...) { return 交付状态::采用状态不明; }
            });
        if (复制.状态 == 新方法复制状态::已复制 && 复制.实例根节点) return 新任务操作状态::已修改;
        if (复制.状态 == 新方法复制状态::资源失败) return 新任务操作状态::资源失败;
        return 新任务操作状态::结构不一致;
    } catch (...) { return 新任务操作状态::资源失败; }
}

新任务治理条件筹办结果 新_任务类::筹办任务条件和参数(
    稳定编码 任务节点) const noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        const auto 任务 = 获取任务(任务节点);
        if (!治理已初始化_ || !任务 || !任务->方法执行组织节点
            || 任务->当前状态 != 新任务状态::待检查条件) return 核验任务条件和参数(任务节点);
        const auto 选择 = 选择任务方法结果(任务节点);
        auto 结果 = 选择.本能采用 ? 核验任务条件和参数内部(任务节点,&*选择.本能采用,false) : 核验任务条件和参数(任务节点);
        结果.方法选择状态 = 选择.状态;
        // 等待条件只有携带完整选定配对时才完成候选核验。
        const bool 已定位 = 选择.方法节点 && 选择.配对节点 && 选择.条件节点 && 选择.结果节点;
        if (选择.状态 != 新任务方法选择状态::已选定
            && !(选择.状态 == 新任务方法选择状态::等待条件 && 已定位)) {
            结果.候选已完整核验 = false;
            if (选择.状态 == 新任务方法选择状态::资源失败) 结果.状态 = 新任务治理条件筹办状态::资源失败;
            else if (选择.状态 == 新任务方法选择状态::结构不一致) 结果.状态 = 新任务治理条件筹办状态::结构不一致;
            else if (选择.状态 == 新任务方法选择状态::任务状态已变化) 结果.状态 = 新任务治理条件筹办状态::任务状态已变化;
        }
        return 结果;
    } catch (...) {
        新任务治理条件筹办结果 结果;
        结果.任务节点 = 任务节点;
        结果.状态 = 新任务治理条件筹办状态::资源失败;
        return 结果;
    }
}

新任务治理条件筹办结果 新_任务类::核验任务条件和参数(
    稳定编码 任务节点) const noexcept {
    return 核验任务条件和参数内部(任务节点,nullptr,false);
}
新任务治理条件筹办结果 新_任务类::核验任务条件和参数内部(
    稳定编码 任务节点,const 新任务本能采用材料* 固定采用,bool 允许当前执行中) const noexcept {
    新任务治理条件筹办结果 结果;
    结果.任务节点 = 任务节点;
    try {
        std::lock_guard 锁(新任务互斥);
        if (!治理已初始化_) {
            结果.状态 = 新任务治理条件筹办状态::尚未初始化;
            return 结果;
        }
        const auto 任务 = 获取任务(任务节点);
        if (!任务) {
            结果.状态 = 新任务治理条件筹办状态::任务不存在;
            return 结果;
        }
        if (任务->当前状态 != 新任务状态::待检查条件
            && !(固定采用&&(任务->当前状态==新任务状态::待执行
                ||(允许当前执行中&&任务->当前状态==新任务状态::执行中)))) {
            结果.状态 = 新任务治理条件筹办状态::任务状态已变化;
            return 结果;
        }
        if (!固定采用&&!任务->当前候选方法材料节点) {
            结果.状态 = 新任务治理条件筹办状态::候选材料不存在;
            return 结果;
        }
        结果.候选材料节点 = 任务->当前候选方法材料节点;
        const auto 材料 = 任务->当前候选方法材料节点 ? 获取候选方法材料(
            *任务->当前候选方法材料节点):std::nullopt;
        const auto 需求 = 需求服务_.获取需求(任务->需求节点);
        if (!需求||(!固定采用&&(!材料 || 材料->任务节点 != 任务节点
            || 材料->需求节点 != 任务->需求节点
            || 材料->需求目标 != 需求->目标
            || 材料->查询状态 != 新方法操作状态::已找到
            || !材料->查询完整 || 材料->候选方法节点组.empty()))) {
            结果.状态 = 新任务治理条件筹办状态::结构不一致;
            return 结果;
        }

        const auto 组织结果=读取任务方法执行组织(任务节点);
        if(!组织结果.组织){结果.状态=新任务治理条件筹办状态::结构不一致;return 结果;}
        const auto& 组织=*组织结果.组织;
        std::vector<稳定编码> 筹办方法组;
        if(固定采用){
            任务执行组织存储::核对采用定义(组织,*固定采用);
            const auto 需要=任务执行组织存储::计算采用需要条件(*固定采用,*需求,特征概念服务_,二次特征概念服务_,特征服务_);
            if(!需要||*需要!=固定采用->需要条件){结果.状态=新任务治理条件筹办状态::任务状态已变化;return 结果;}
            筹办方法组.push_back(固定采用->来源方法节点);
        }
        else for(const auto& 域:组织.方法作用域组)
            if(std::find(组织.根方法作用域节点组.begin(),组织.根方法作用域节点组.end(),域.节点)!=组织.根方法作用域节点组.end())筹办方法组.push_back(域.来源方法节点);

        const auto 存在概念兼容 = [&](稳定编码 实际概念,
                                      稳定编码 要求概念) noexcept {
            if (实际概念 == 要求概念) return true;
            std::vector<稳定编码> 待查{实际概念};
            std::set<稳定编码> 已查;
            while (!待查.empty()) {
                const auto 当前 = 待查.back();
                待查.pop_back();
                if (!已查.insert(当前).second) continue;
                for (const auto 上位 : 存在概念服务_.查询上位存在概念(当前)) {
                    if (上位 == 要求概念) return true;
                    待查.push_back(上位);
                }
            }
            return false;
        };
        bool 概念比较结构失败 = false;
        const auto 特征概念兼容 = [&](稳定编码 实际概念,
                                      稳定编码 要求概念) {
            const auto 比较 = 特征概念服务_.比较单特征概念(
                实际概念, 要求概念);
            if (比较.状态 == 特征概念规则状态::资源失败) throw std::bad_alloc{};
            if (比较.状态 != 特征概念规则状态::已完成 || !比较.关系) {
                概念比较结构失败 = true;
                return false; // 本次核验末尾转工程结构失败，不交付业务缺口。
            }
            return 比较.状态 == 特征概念规则状态::已完成
                && 比较.关系
                && (*比较.关系 == 单特征概念关系::同一
                    || *比较.关系 == 单特征概念关系::右为上位);
        };

        enum class 值域核验 : std::uint8_t {
            命中, 未命中, 材料未到, 结构缺口
        };
        const auto 当前I64命中值域 = [&](const 新特征信息& 特征,
                                          const 新特征值域比较输入& 要求) {
            if (!特征.当前值) return 值域核验::材料未到;
            const auto* I64 = std::get_if<std::int64_t>(&*特征.当前值);
            if (!I64) return 值域核验::结构缺口;
            const auto 比较 = 特征服务_.比较特征值域(
                {特征.特征概念节点, 特征.实例单位, 新特征准确I64比较材料{*I64}}, 要求);
            if (比较.状态 == 新特征比较状态::资源失败) throw std::bad_alloc{};
            if (比较.状态 != 新特征比较状态::已完成 || !比较.关系编码)
                return 值域核验::结构缺口;
            return *比较.关系编码 == static_cast<std::int64_t>(特征概念值域关系::相等)
                    || *比较.关系编码 == static_cast<std::int64_t>(特征概念值域关系::右包含左)
                ? 值域核验::命中 : 值域核验::未命中;
        };
        const auto 当前值命中概念 = [&](const 新特征信息& 特征,
                                        稳定编码 要求概念) {
            if (!特征.当前值) return 值域核验::材料未到;
            const auto 目标域 = 特征概念服务_.获取特征概念(要求概念);
            if (!目标域) return 值域核验::结构缺口;
            if (const auto* 集合 = std::get_if<特征概念任务集合值域>(&目标域->值域)) {
                const auto 实际根 = 取得特征概念类型根(特征概念服务_, 特征.特征概念节点);
                if (!实际根 || 实际根 != 取得特征概念类型根(特征概念服务_, 要求概念))
                    return 值域核验::结构缺口;
                if (特征.实例单位 || 目标域->单位) return 值域核验::结构缺口;
                const auto 比较 = 特征服务_.比较任务集合条件(
                    *实际根, *特征.当前值, *集合);
                if (比较.状态 == 特征概念规则状态::资源失败) throw std::bad_alloc{};
                if (比较.状态 != 特征概念规则状态::已完成 || !比较.比较值)
                    return 值域核验::结构缺口;
                return *比较.比较值 == 1 ? 值域核验::命中 : 值域核验::未命中;
            }
            // 概念域和直接域共用准确值落域入口，不靠实例概念的祖先路径猜测。
            if (std::holds_alternative<std::int64_t>(*特征.当前值)) {
                const auto 域 = 特征概念服务_.获取特征概念(要求概念);
                if (!域) return 值域核验::结构缺口;
                return 当前I64命中值域(特征,
                    {要求概念, 域->单位, 新特征概念值域比较材料{要求概念}});
            }
            const auto 关系 = 特征概念服务_.比较单特征概念(
                特征.特征概念节点, 要求概念);
            if (关系.状态 != 特征概念规则状态::已完成
                || !关系.特征类型根) return 值域核验::结构缺口;
            const auto 查找 = 特征概念服务_.查找值对应的最具体概念(
                *关系.特征类型根, *特征.当前值, 特征.实例单位,
                特征值服务_);
            if (查找.状态 == 特征概念查找状态::未找到) {
                return 值域核验::未命中;
            }
            if (查找.状态 != 特征概念查找状态::已找到
                || !查找.概念节点) return 值域核验::结构缺口;
            return 特征概念兼容(*查找.概念节点, 要求概念)
                ? 值域核验::命中 : 值域核验::未命中;
        };

        struct 对象材料 final {
            稳定编码 节点;
            稳定编码 专属概念;
            std::vector<稳定编码> 特征组;
            std::optional<稳定编码> 直接所属场景;
        };
        const auto 读取对象 = [&](稳定编码 节点)
            -> std::optional<对象材料> {
            if (场景服务_.是场景节点(节点)) {
                const auto 场景 = 场景服务_.获取场景(节点);
                if (!场景) return std::nullopt;
                return 对象材料{节点, 场景->专属存在概念节点,
                    场景->特征节点组, 节点};
            }
            const auto 存在 = 存在服务_.获取存在(节点);
            if (!存在) return std::nullopt;
            return 对象材料{节点, 存在->专属存在概念节点,
                存在->特征节点组, 场景服务_.查询所属场景(节点)};
        };
        const auto 查找所属场景 = [&](稳定编码 节点)
            -> std::optional<稳定编码> {
            if (场景服务_.是场景节点(节点)) return 节点;
            std::set<稳定编码> 已查;
            auto 当前 = 节点;
            while (有效(当前) && 已查.insert(当前).second) {
                if (const auto 所属 = 场景服务_.查询所属场景(当前)) {
                    return 所属;
                }
                const auto 存在 = 存在服务_.获取存在(当前);
                if (!存在 || !存在->父存在节点) return std::nullopt;
                当前 = *存在->父存在节点;
            }
            return std::nullopt;
        };
        const auto 收集场景范围对象 = [&](稳定编码 根场景) {
            std::vector<稳定编码> 对象组;
            std::vector<稳定编码> 待查场景{根场景};
            std::vector<稳定编码> 待查存在;
            std::set<稳定编码> 已查场景;
            std::set<稳定编码> 已查存在;
            while (!待查场景.empty()) {
                const auto 场景 = 待查场景.back();
                待查场景.pop_back();
                if (!已查场景.insert(场景).second) continue;
                对象组.push_back(场景);
                const auto 子场景 = 场景服务_.查询直接子场景(场景);
                待查场景.insert(待查场景.end(), 子场景.begin(), 子场景.end());
                const auto 成员 = 场景服务_.查询直接成员存在(场景);
                待查存在.insert(待查存在.end(), 成员.begin(), 成员.end());
            }
            while (!待查存在.empty()) {
                const auto 存在 = 待查存在.back();
                待查存在.pop_back();
                if (!已查存在.insert(存在).second) continue;
                对象组.push_back(存在);
                const auto 子存在 = 存在服务_.查询直接子存在(存在);
                待查存在.insert(待查存在.end(), 子存在.begin(), 子存在.end());
            }
            std::sort(对象组.begin(), 对象组.end());
            对象组.erase(std::unique(对象组.begin(), 对象组.end()), 对象组.end());
            return 对象组;
        };

        const auto 目标场景 = 查找所属场景(需求->目标存在节点);
        if (!目标场景 || !场景服务_.是场景节点(*目标场景)) {
            结果.状态 = 新任务治理条件筹办状态::结构不一致;
            return 结果;
        }
        const auto 范围对象组 = 收集场景范围对象(*目标场景);

        enum class 对象匹配状态 : std::uint8_t {
            匹配, 不匹配, 材料未到, 结构缺口
        };
        for (const auto 方法节点 : 筹办方法组) {
            新任务候选方法筹办项 方法项;
            方法项.方法节点 = 方法节点;
            const auto 方法 = 任务执行组织存储::读取来源视图(组织,方法节点,方法服务_,固定采用);
            if (!方法) {
                结果.状态 = 新任务治理条件筹办状态::结构不一致;
                return 结果;
            }
            方法项.方法作用域节点=方法->作用域节点;
            if(方法->启用状态!=新方法启用状态::启用){
                方法项.状态=新任务候选方法筹办状态::条件不具备;
                for(const auto& 对:方法->条件结果组)方法项.条件结果组.push_back({对.配对节点,对.条件节点,对.结果节点,新任务方法条件项核验状态::不具备,{}});
                结果.方法筹办组.push_back(std::move(方法项));continue;
            }
            if(方法->实现方式!=新方法实现方式::本能){
                方法项.状态=新任务候选方法筹办状态::结构缺口;
                结果.方法筹办组.push_back(std::move(方法项));
                continue;
            }

            bool 参数材料未到 = false;
            bool 参数结构缺口 = false;
            bool 参数不唯一 = false;
            const auto 记录缺口 = [&](const 新方法参数规格& 规格,
                新任务方法条件缺口种类 种类,
                std::vector<新方法参数角色绑定> 已知绑定组,
                const 新方法条件结果信息* 当前配对 = nullptr) {
                新任务方法条件缺口 缺口{种类, 规格, std::move(已知绑定组), {}};
                if (当前配对) 缺口.相关条件结果组.push_back(*当前配对);
                else for (const auto& 配对 : 方法->条件结果组) {
                    if (std::any_of(配对.条件.条件项组.begin(), 配对.条件.条件项组.end(),
                            [&](const auto& 项) { return 项.参数角色节点 == 规格.参数角色节点; }))
                        缺口.相关条件结果组.push_back(配对);
                }
                if (std::find(方法项.条件缺口组.begin(), 方法项.条件缺口组.end(), 缺口)
                        == 方法项.条件缺口组.end())
                    方法项.条件缺口组.push_back(std::move(缺口));
            };
            for (const auto& 规格 : 方法->参数规格组) {
                const 新方法参数角色绑定* 固定绑定=nullptr;
                if(固定采用){
                    const auto 位置=std::find_if(固定采用->参数绑定组.begin(),固定采用->参数绑定组.end(),[&](const auto& 项){return 项.参数角色节点==规格.参数角色节点;});
                    if(位置==固定采用->参数绑定组.end())throw std::logic_error("固定采用缺少声明角色");
                    固定绑定=&*位置;
                }
                if (规格.对象种类 == 新方法参数对象种类::任务虚拟存在) {
                    if (规格.存在概念节点 || 规格.要求场景 || 规格.特征规格组.size() != 1
                        || !规格.特征规格组.front().二次特征概念组.empty()) {
                        参数结构缺口 = true;
                        break;
                    }
                    if (!任务->服务结算来源) {
                        参数材料未到 = 参数不唯一 = true;
                        记录缺口(规格, 新任务方法条件缺口种类::参数对象未取得, {});
                        continue;
                    }
                    const auto 来源 = 读取任务虚拟存在引用(任务->服务结算来源->来源任务);
                    if(固定绑定&&(!来源||固定绑定->任务虚拟存在引用!=来源->引用)){
                        参数不唯一=true;记录缺口(规格,新任务方法条件缺口种类::条件值不满足,{*固定绑定});continue;
                    }
                    if (!来源 || 规格.存在概念节点 || 规格.要求场景
                        || 规格.特征规格组.size() != 1) {
                        参数结构缺口 = true;
                        break;
                    }
                    const auto& 特征规格 = 规格.特征规格组.front();
                    const auto 特征 = 特征服务_.获取特征(来源->引用.状态特征节点);
                    if (!特征 || !特征规格.二次特征概念组.empty()
                        || !特征概念兼容(特征->特征概念节点, 特征规格.特征类型概念节点)) {
                        参数结构缺口 = true;
                        break;
                    }
                    新方法参数角色绑定 绑定;
                    绑定.参数角色节点 = 规格.参数角色节点;
                    绑定.任务虚拟存在引用 = 来源->引用;
                    绑定.实际特征组.push_back({来源->引用.状态特征节点});
                    if (特征规格.值域概念节点) {
                        const auto 命中 = 当前值命中概念(*特征, *特征规格.值域概念节点);
                        if (命中 == 值域核验::结构缺口) { 参数结构缺口 = true; break; }
                        if (命中 != 值域核验::命中) {
                            参数不唯一 = true;
                            参数材料未到 = 参数材料未到 || 命中 == 值域核验::材料未到;
                            记录缺口(规格, 命中 == 值域核验::材料未到
                                ? 新任务方法条件缺口种类::参数特征值未取得
                                : 新任务方法条件缺口种类::条件值不满足, {绑定});
                            continue;
                        }
                    }
                    方法项.参数绑定组.push_back(std::move(绑定));
                    continue;
                }
                if (std::any_of(规格.特征规格组.begin(), 规格.特征规格组.end(),
                        [](const auto& 特征规格) {
                            return !特征规格.二次特征概念组.empty();
                        })) {
                    参数结构缺口 = true;
                    break;
                }
                std::vector<新方法参数角色绑定> 命中绑定;
                std::vector<新方法参数角色绑定> 已知未满足绑定;
                bool 本角色材料未到 = false;
                const auto 当前对象组=固定绑定
                    ? (固定绑定->实际存在节点 ? std::vector<稳定编码>{*固定绑定->实际存在节点} : std::vector<稳定编码>{})
                    :范围对象组;
                for (const auto 对象节点 : 当前对象组) {
                    if(std::find(范围对象组.begin(),范围对象组.end(),对象节点)==范围对象组.end())continue;
                    const auto 对象 = 读取对象(对象节点);
                    if (!对象) {
                        参数结构缺口 = true;
                        break;
                    }
                    if (规格.存在概念节点
                        && !存在概念兼容(
                            对象->专属概念, *规格.存在概念节点)) continue;

                    对象匹配状态 对象状态 = 对象匹配状态::匹配;
                    std::vector<新方法实际特征绑定> 特征绑定组;
                    std::vector<新方法实际特征绑定> 已知特征绑定组;
                    for (const auto& 特征规格 : 规格.特征规格组) {
                        std::vector<稳定编码> 特征命中;
                        bool 当前值缺失 = false;
                        for (const auto 特征节点 : 对象->特征组) {
                            if(固定绑定&&std::none_of(固定绑定->实际特征组.begin(),固定绑定->实际特征组.end(),[&](const auto& 项){return 项.特征节点==特征节点;}))continue;
                            const auto 特征 = 特征服务_.获取特征(特征节点);
                            if (!特征) {
                                对象状态 = 对象匹配状态::结构缺口;
                                break;
                            }
                            if (!特征概念兼容(特征->特征概念节点,
                                    特征规格.特征类型概念节点)) continue;
                            已知特征绑定组.push_back({特征节点});
                            if (特征规格.值域概念节点) {
                                const auto 命中 = 当前值命中概念(
                                    *特征, *特征规格.值域概念节点);
                                if (命中 == 值域核验::材料未到) {
                                    当前值缺失 = true;
                                    continue;
                                }
                                if (命中 == 值域核验::结构缺口) {
                                    对象状态 = 对象匹配状态::结构缺口;
                                    break;
                                }
                                if (命中 != 值域核验::命中) continue;
                            }
                            特征命中.push_back(特征节点);
                        }
                        if (对象状态 == 对象匹配状态::结构缺口) break;
                        if (特征命中.empty()) {
                            if (当前值缺失) 对象状态 = 对象匹配状态::材料未到;
                            else if (对象状态 == 对象匹配状态::匹配)
                                对象状态 = 对象匹配状态::不匹配;
                            // 业务缺失不能截断剩余规格，继续保留其它真实特征引用。
                            continue;
                        }
                        if (特征命中.size() != 1) {
                            对象状态 = 对象匹配状态::结构缺口;
                            break;
                        }
                        特征绑定组.push_back({特征命中.front()});
                    }
                    if (对象状态 == 对象匹配状态::结构缺口) {
                        参数结构缺口 = true;
                        break;
                    }
                    新方法参数角色绑定 绑定;
                    绑定.参数角色节点 = 规格.参数角色节点;
                    绑定.实际存在节点 = 对象节点;
                    绑定.实际特征组 = 对象状态 == 对象匹配状态::匹配
                        ? std::move(特征绑定组) : std::move(已知特征绑定组);
                    if (对象->直接所属场景
                        && 场景服务_.存在属于场景范围(
                            *目标场景, 对象节点)) {
                        绑定.世界场景节点 = *目标场景;
                    }
                    if (规格.要求场景 && !绑定.世界场景节点) continue;
                    if (对象状态 != 对象匹配状态::匹配) {
                        本角色材料未到 = 本角色材料未到 || 对象状态 == 对象匹配状态::材料未到;
                        // 无类型匹配且没有概念约束的普通场景成员并非已知参数。
                        if (规格.存在概念节点 || !绑定.实际特征组.empty())
                            已知未满足绑定.push_back(std::move(绑定));
                        continue;
                    }
                    命中绑定.push_back(std::move(绑定));
                }
                if (参数结构缺口) break;
                if (命中绑定.size() != 1) {
                    参数不唯一 = true;
                    参数材料未到 = 参数材料未到 || 本角色材料未到;
                    记录缺口(规格, 命中绑定.size() > 1
                        ? 新任务方法条件缺口种类::参数匹配不唯一
                        : 本角色材料未到 ? 新任务方法条件缺口种类::参数特征值未取得
                            : 已知未满足绑定.empty() ? 新任务方法条件缺口种类::参数对象未取得
                                : 新任务方法条件缺口种类::条件值不满足,
                        命中绑定.empty() ? std::move(已知未满足绑定) : std::move(命中绑定));
                    continue;
                }
                方法项.参数绑定组.push_back(std::move(命中绑定.front()));
            }
            if (!参数结构缺口) for (const auto& 规格 : 方法->参数规格组) {
                if (!规格.集合成员约束) continue;
                const auto& 约束 = *规格.集合成员约束;
                const auto 来源 = std::find_if(方法项.参数绑定组.begin(), 方法项.参数绑定组.end(),
                    [&](const auto& b) { return b.参数角色节点 == 规格.参数角色节点; });
                const auto 集合角色 = std::find_if(方法项.参数绑定组.begin(), 方法项.参数绑定组.end(),
                    [&](const auto& b) { return b.参数角色节点 == 约束.集合参数角色节点; });
                // 对象或值域缺口已在该角色核验中保留，不用另一个任务补齐。
                if (来源 == 方法项.参数绑定组.end() || 集合角色 == 方法项.参数绑定组.end()) continue;
                if (!来源->任务虚拟存在引用) { 参数结构缺口 = true; break; }
                std::optional<新特征信息> 集合;
                for (const auto& b : 集合角色->实际特征组) {
                    const auto f = 特征服务_.获取特征(b.特征节点);
                    if (!f) { 参数结构缺口 = true; break; }
                    if (特征概念兼容(f->特征概念节点, 约束.集合特征类型概念节点)) {
                        if (集合) { 参数结构缺口 = true; break; }
                        集合 = f;
                    }
                }
                if (参数结构缺口) break;
                if (!集合) { 参数结构缺口 = true; break; }
                if (!集合->当前值) {
                    参数不唯一 = 参数材料未到 = true;
                    记录缺口(规格, 新任务方法条件缺口种类::参数特征值未取得, {*来源, *集合角色});
                    continue;
                }
                特征概念任务集合值域 成员要求;
                成员要求.必含任务节点组 = {来源->任务虚拟存在引用->任务节点};
                const auto 比较 = 特征服务_.比较任务集合条件(
                    约束.集合特征类型概念节点, *集合->当前值, 成员要求);
                if (比较.状态 == 特征概念规则状态::资源失败) throw std::bad_alloc{};
                if (比较.状态 != 特征概念规则状态::已完成 || !比较.比较值) { 参数结构缺口 = true; break; }
                if (*比较.比较值 != 1) {
                    参数不唯一 = true;
                    记录缺口(规格, 新任务方法条件缺口种类::条件值不满足, {*来源, *集合角色});
                }
            }
            参数结构缺口 = 参数结构缺口 || 概念比较结构失败;
            if (参数结构缺口) {
                // 工程/声明结构异常不携带可供self采用的业务缺口。
                方法项.条件缺口组.clear();
                方法项.状态 = 新任务候选方法筹办状态::结构缺口;
                结果.方法筹办组.push_back(std::move(方法项));
                continue;
            }
            if (参数不唯一) {
                方法项.状态 = 参数材料未到
                    ? 新任务候选方法筹办状态::材料未到
                    : 新任务候选方法筹办状态::参数缺失;
                结果.方法筹办组.push_back(std::move(方法项));
                continue;
            }
            std::sort(方法项.参数绑定组.begin(), 方法项.参数绑定组.end(),
                [](const auto& 左, const auto& 右) {
                    return 左.参数角色节点 < 右.参数角色节点;
                });
            for(auto& 绑定:方法项.参数绑定组)std::sort(绑定.实际特征组.begin(),绑定.实际特征组.end(),[](const auto& 左,const auto& 右){return 左.特征节点<右.特征节点;});
            if(固定采用&&方法项.参数绑定组!=固定采用->参数绑定组){方法项.状态=新任务候选方法筹办状态::条件不具备;结果.方法筹办组.push_back(std::move(方法项));continue;}
            const auto 参数场景 = 方法服务_.建立参数场景(
                方法节点, 方法项.参数绑定组, this);
            if (参数场景.状态 != 新方法操作状态::已建立
                || !参数场景.参数场景) {
                方法项.状态 = 参数场景.状态 == 新方法操作状态::资源失败
                    ? 新任务候选方法筹办状态::资源失败
                    : 新任务候选方法筹办状态::结构缺口;
                结果.方法筹办组.push_back(std::move(方法项));
                continue;
            }

            const auto 查找角色对象 = [&](稳定编码 角色)
                -> std::optional<对象材料> {
                const auto 位置 = std::find_if(
                    方法项.参数绑定组.begin(), 方法项.参数绑定组.end(),
                    [&](const auto& 绑定) {
                        return 绑定.参数角色节点 == 角色;
                    });
                if (位置 != 方法项.参数绑定组.end() && 位置->任务虚拟存在引用) {
                    const auto 来源 = 读取任务虚拟存在引用(*位置->任务虚拟存在引用);
                    if (!来源) return std::nullopt;
                    // 只提供Vt本身的状态F；不调用世界存在/场景入口伪造世界成员。
                    return 对象材料{来源->引用.虚拟存在节点, {},
                        {来源->引用.状态特征节点}, std::nullopt};
                }
                if (位置 == 方法项.参数绑定组.end() || !位置->实际存在节点)
                    return std::nullopt;
                auto 对象 = 读取对象(*位置->实际存在节点);
                if (对象 && 固定采用) {
                    std::erase_if(对象->特征组, [&](稳定编码 特征节点) {
                        return std::none_of(位置->实际特征组.begin(), 位置->实际特征组.end(),
                            [&](const auto& 绑定) { return 绑定.特征节点 == 特征节点; });
                    });
                }
                return 对象;
            };
            bool 任一条件具备 = false;
            bool 有材料未到 = false;
            bool 有结构缺口 = false;
            for (const auto& 配对 : 方法->条件结果组) {
                新任务方法条件结果核验 配对结果{
                    配对.配对节点, 配对.条件节点, 配对.结果节点,
                    新任务方法条件项核验状态::结构缺口};
                const auto 域=std::find_if(组织.方法作用域组.begin(),组织.方法作用域组.end(),[&](const auto& 项){return 项.节点==方法->作用域节点;});
                const auto 要求=std::find_if(域->配对要求组.begin(),域->配对要求组.end(),[&](const auto& 项){return 项.来源配对节点==配对.配对节点;});
                if(要求==域->配对要求组.end())throw std::logic_error("筹办来源配对无任务要求");
                配对结果.执行要求定位=新任务本能采用定位{组织.节点,域->节点,要求->节点};
                std::vector<新任务方法条件项核验状态> 项状态组;
                for (const auto& 条件项 : 配对.条件.条件项组) {
                    if (条件项.二次特征概念节点
                        || 条件项.状态或动态概念节点
                        || (条件项.精确数值域
                            && (!条件项.特征类型概念节点 || 条件项.特征值域概念节点))) {
                        项状态组.push_back(
                            新任务方法条件项核验状态::结构缺口);
                        continue;
                    }
                    const auto 对象 = 查找角色对象(条件项.参数角色节点);
                    if (!对象) {
                        项状态组.push_back(
                            新任务方法条件项核验状态::结构缺口);
                        continue;
                    }
                    if (条件项.存在概念节点
                        && !存在概念兼容(
                            对象->专属概念, *条件项.存在概念节点)) {
                        项状态组.push_back(
                            新任务方法条件项核验状态::不具备);
                        continue;
                    }
                    if (!条件项.特征类型概念节点
                        && !条件项.特征值域概念节点 && !条件项.精确数值域) {
                        项状态组.push_back(
                            新任务方法条件项核验状态::具备);
                        continue;
                    }
                    bool 命中特征 = false;
                    bool 缺当前值 = false;
                    bool 特征结构错误 = false;
                    for (const auto 特征节点 : 对象->特征组) {
                        const auto 特征 = 特征服务_.获取特征(特征节点);
                        if (!特征) {
                            特征结构错误 = true;
                            break;
                        }
                        if (条件项.特征类型概念节点
                            && !特征概念兼容(特征->特征概念节点,
                                *条件项.特征类型概念节点)) continue;
                        if (条件项.特征值域概念节点 || 条件项.精确数值域) {
                            auto 命中 = 值域核验::结构缺口;
                            if (条件项.精确数值域) {
                                const auto 类型 = 特征概念服务_.获取特征概念(
                                    *条件项.特征类型概念节点);
                                if (类型) 命中 = 当前I64命中值域(*特征,
                                    {类型->节点, 类型->单位,
                                        新特征直接I64值域比较材料{*条件项.精确数值域}});
                            } else {
                                命中 = 当前值命中概念(*特征, *条件项.特征值域概念节点);
                            }
                            if (命中 == 值域核验::材料未到) {
                                缺当前值 = true;
                                continue;
                            }
                            if (命中 == 值域核验::结构缺口) {
                                特征结构错误 = true;
                                break;
                            }
                            if (命中 != 值域核验::命中) continue;
                        }
                        命中特征 = true;
                        break;
                    }
                    项状态组.push_back(特征结构错误
                        ? 新任务方法条件项核验状态::结构缺口
                        : (命中特征
                            ? 新任务方法条件项核验状态::具备
                            : (缺当前值
                                ? 新任务方法条件项核验状态::材料未到
                                : 新任务方法条件项核验状态::不具备)));
                }
                const auto 有 = [&](新任务方法条件项核验状态 状态) {
                    return std::find(项状态组.begin(), 项状态组.end(), 状态)
                        != 项状态组.end();
                };
                for (std::size_t i = 0; !有(新任务方法条件项核验状态::结构缺口)
                        && i < 项状态组.size(); ++i) {
                    if (项状态组[i] != 新任务方法条件项核验状态::不具备
                        && 项状态组[i] != 新任务方法条件项核验状态::材料未到) continue;
                    const auto 角色 = 配对.条件.条件项组[i].参数角色节点;
                    const auto 规格 = std::find_if(方法->参数规格组.begin(), 方法->参数规格组.end(),
                        [&](const auto& 项) { return 项.参数角色节点 == 角色; });
                    if (规格 == 方法->参数规格组.end()) {
                        结果.状态 = 新任务治理条件筹办状态::结构不一致;
                        结果.方法筹办组.clear();
                        return 结果;
                    }
                    std::vector<新方法参数角色绑定> 绑定组;
                    for (const auto& 绑定 : 方法项.参数绑定组)
                        if (绑定.参数角色节点 == 角色) 绑定组.push_back(绑定);
                    记录缺口(*规格, 项状态组[i] == 新任务方法条件项核验状态::材料未到
                        ? 新任务方法条件缺口种类::参数特征值未取得
                        : 新任务方法条件缺口种类::条件值不满足, std::move(绑定组), &配对);
                }
                switch (配对.条件.关系规则) {
                case 新方法项关系规则::单项:
                    if (项状态组.size() == 1) 配对结果.状态 = 项状态组.front();
                    break;
                case 新方法项关系规则::全部成立:
                    配对结果.状态 = 有(新任务方法条件项核验状态::不具备)
                        ? 新任务方法条件项核验状态::不具备
                        : (有(新任务方法条件项核验状态::结构缺口)
                            ? 新任务方法条件项核验状态::结构缺口
                            : (有(新任务方法条件项核验状态::材料未到)
                                ? 新任务方法条件项核验状态::材料未到
                                : 新任务方法条件项核验状态::具备));
                    break;
                case 新方法项关系规则::任一成立:
                    配对结果.状态 = 有(新任务方法条件项核验状态::具备)
                        ? 新任务方法条件项核验状态::具备
                        : (有(新任务方法条件项核验状态::结构缺口)
                            ? 新任务方法条件项核验状态::结构缺口
                            : (有(新任务方法条件项核验状态::材料未到)
                                ? 新任务方法条件项核验状态::材料未到
                                : 新任务方法条件项核验状态::不具备));
                    break;
                case 新方法项关系规则::有序成立:
                case 新方法项关系规则::条件选择:
                case 新方法项关系规则::主要结果及允许副作用:
                    配对结果.状态 =
                        新任务方法条件项核验状态::结构缺口;
                    break;
                }
                if (有(新任务方法条件项核验状态::结构缺口))
                    配对结果.状态 = 新任务方法条件项核验状态::结构缺口;
                任一条件具备 = 任一条件具备
                    || 配对结果.状态 == 新任务方法条件项核验状态::具备;
                有材料未到 = 有材料未到
                    || 配对结果.状态 == 新任务方法条件项核验状态::材料未到;
                有结构缺口 = 有结构缺口
                    || 配对结果.状态 == 新任务方法条件项核验状态::结构缺口;
                方法项.条件结果组.push_back(std::move(配对结果));
            }
            if (有结构缺口) 方法项.条件缺口组.clear();
            方法项.状态 = 任一条件具备
                ? 新任务候选方法筹办状态::条件具备
                : (有结构缺口
                    ? 新任务候选方法筹办状态::结构缺口
                    : (有材料未到
                        ? 新任务候选方法筹办状态::材料未到
                        : 新任务候选方法筹办状态::条件不具备));
            结果.方法筹办组.push_back(std::move(方法项));
        }
        if (概念比较结构失败) {
            结果.状态 = 新任务治理条件筹办状态::结构不一致;
            结果.方法筹办组.clear();
            return 结果;
        }
        结果.候选已完整核验 =
            结果.方法筹办组.size() == 筹办方法组.size();
        结果.状态 = 结果.候选已完整核验
            ? 新任务治理条件筹办状态::已完成
            : 新任务治理条件筹办状态::结构不一致;
        return 结果;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"核验任务条件和参数", L"任务执行组织或采用核验工程故障");
    }
}

新任务方法选择结果 新_任务类::选择任务方法结果(稳定编码 任务节点) const noexcept {
    新任务方法选择结果 结果;
    结果.任务节点 = 任务节点;
    try {
        std::lock_guard 锁(新任务互斥);
        const auto 任务 = 获取任务(任务节点);
        if (!治理已初始化_ || !任务) return 结果;
        const auto 来源守卫 = 需求服务_.复核需求服务要求语义(任务->需求节点);
        if (来源守卫.状态 != 新服务要求执行守卫状态::不涉及
            && 来源守卫.状态 != 新服务要求执行守卫状态::当前有效) {
            结果.状态 = 来源守卫.状态 == 新服务要求执行守卫状态::需重新确认
                ? 新任务方法选择状态::等待条件
                : 来源守卫.状态 == 新服务要求执行守卫状态::资源失败
                    ? 新任务方法选择状态::资源失败 : 新任务方法选择状态::结构不一致;
            return 结果;
        }
        if (任务->当前状态 != 新任务状态::待检查条件) {
            结果.状态 = 新任务方法选择状态::任务状态已变化; return 结果;
        }
        const auto 组织结果=读取任务方法执行组织(任务节点);
        if (!组织结果.组织) {
            结果.状态 = 新任务方法选择状态::实例未建立; return 结果;
        }
        const auto& 组织=*组织结果.组织;
        for(const auto& 域:组织.方法作用域组)
            if(std::find(组织.根方法作用域节点组.begin(),组织.根方法作用域节点组.end(),域.节点)!=组织.根方法作用域节点组.end()
                &&域.实现方式!=新方法实现方式::本能){结果.状态=新任务方法选择状态::结果定义暂不支持;return 结果;}
        const auto 需求 = 需求服务_.获取需求(任务->需求节点);
        if (!需求) return 结果;
        if (需求->活动状态 != 新需求活动状态::活动) {
            结果.状态 = 新任务方法选择状态::无适用结果; return 结果;
        }
        const auto 当前 = 需求服务_.复核当前满足(任务->需求节点);
        if (当前.状态 == 新需求当前满足状态::已满足) {
            结果.状态 = 新任务方法选择状态::目标已满足; return 结果;
        }
        if (当前.状态 == 新需求当前满足状态::资源失败) {
            结果.状态 = 新任务方法选择状态::资源失败; return 结果;
        }
        if (!当前.实际特征节点) {
            结果.状态 = 新任务方法选择状态::等待条件; return 结果;
        }
        const auto 特征 = 特征服务_.获取特征(*当前.实际特征节点);
        if (!特征 || !特征->当前值) {
            结果.状态 = 新任务方法选择状态::等待条件; return 结果;
        }
        const auto* 当前I64 = std::get_if<std::int64_t>(&*特征->当前值);
        auto 所需方向 = 预计变化方向::不明确;
        if (需求->目标.目标形式 == 新需求目标形式::变化方向) {
            所需方向 = 需求->目标.目标变化方向 == 新需求变化方向::增加
                ? 预计变化方向::增加 : 预计变化方向::减少;
        } else if (当前.状态 == 新需求当前满足状态::需增加) 所需方向 = 预计变化方向::增加;
        else if (当前.状态 == 新需求当前满足状态::需减少) 所需方向 = 预计变化方向::减少;
        else if (当前.状态 != 新需求当前满足状态::未满足但无有向差值) return 结果;

        const auto 筹办 = 核验任务条件和参数(任务节点);
        if (筹办.状态 != 新任务治理条件筹办状态::已完成 || !筹办.候选已完整核验) {
            结果.状态 = 筹办.状态 == 新任务治理条件筹办状态::资源失败
                ? 新任务方法选择状态::资源失败 : 新任务方法选择状态::结构不一致;
            return 结果;
        }
        struct 来源配对视图 {稳定编码 方法节点;新方法条件结果信息 配对;新任务本能采用定位 定位;};
        std::vector<来源配对视图> 配对组;
        for(const auto& 域:组织.方法作用域组){
            if(std::find(组织.根方法作用域节点组.begin(),组织.根方法作用域节点组.end(),域.节点)==组织.根方法作用域节点组.end())continue;
            for(const auto& 对:域.配对要求组){
                新方法条件结果信息 投影;投影.配对节点=对.来源配对节点;投影.条件节点=对.来源条件节点;投影.结果节点=对.来源结果节点;
                投影.条件=对.条件;投影.结果=对.结果;投影.线性关系组=对.线性关系组;
                配对组.push_back({域.来源方法节点,std::move(投影),{组织.节点,域.节点,对.节点}});
            }
        }
        struct 适用项 {
            const 来源配对视图* 配对;
            const 新任务候选方法筹办项* 筹办;
            bool 保证;
            std::optional<std::int64_t> 端点;
            std::vector<特征概念I64闭区间> 所需条件域组;
        };
        std::vector<适用项> 适用组;
        bool 有未知幅度 = false, 有未支持 = false;
        for (const auto& 项 : 配对组) {
            const auto 方法筹办 = std::find_if(筹办.方法筹办组.begin(), 筹办.方法筹办组.end(),
                [&](const auto& x) { return x.方法节点 == 项.方法节点; });
            if (方法筹办 == 筹办.方法筹办组.end()) return 结果;
            if (方法筹办->状态 == 新任务候选方法筹办状态::参数缺失
                || 方法筹办->状态 == 新任务候选方法筹办状态::材料未到) {
                // 无法绑定的候选并不等于能力不适用，不偷偷遗漏后选次优。
                结果.状态 = 新任务方法选择状态::等待条件; return 结果;
            }
            if (方法筹办->状态 == 新任务候选方法筹办状态::资源失败) {
                结果.状态 = 新任务方法选择状态::资源失败; return 结果;
            }
            if (方法筹办->状态 == 新任务候选方法筹办状态::结构缺口) return 结果;
            const auto& 定义 = 项.配对.结果;
            if (定义.关系规则 != 新方法项关系规则::单项
                && 定义.关系规则 != 新方法项关系规则::全部成立
                && 定义.关系规则 != 新方法项关系规则::主要结果及允许副作用) {
                有未支持 = true; continue;
            }
            const 新方法结果项定义* 目标项 = nullptr;
            for (const auto& 预期 : 定义.结果项组) {
                if (定义.关系规则 == 新方法项关系规则::主要结果及允许副作用 && !预期.主要结果) continue;
                if (!预期.特征类型概念节点) { 有未支持 = true; continue; }
                if (取得特征概念类型根(特征概念服务_, *预期.特征类型概念节点)
                    != std::optional<稳定编码>{需求->目标.目标特征类型概念节点}) continue;
                const auto 绑定 = std::find_if(方法筹办->参数绑定组.begin(), 方法筹办->参数绑定组.end(),
                    [&](const auto& x) { return x.参数角色节点 == 预期.参数角色节点; });
                if (绑定 == 方法筹办->参数绑定组.end()) return 结果;
                if (绑定->实际存在节点 != std::optional<稳定编码>{需求->目标存在节点}) continue;
                if (目标项) { 有未支持 = true; 目标项 = nullptr; break; }
                目标项 = &预期;
            }
            if (!目标项) continue;
            if (目标项->状态或动态概念节点) { 有未支持 = true; continue; }
            const auto 声明方向 = 目标项->变化方向概念节点
                ? 读取预计变化方向(二次特征概念服务_, *目标项->变化方向概念节点)
                : 预计变化方向::不明确;
            if (!目标项->特征值域概念节点 && !目标项->精确数值域) {
                if (声明方向 == 所需方向 && 所需方向 != 预计变化方向::不明确) 有未知幅度 = true;
                else if (声明方向 == 预计变化方向::不明确) 有未支持 = true;
                continue;
            }
            if (目标项->精确数值域 && 目标项->特征值域概念节点) return 结果;
            const auto 类型 = 特征概念服务_.获取特征概念(*目标项->特征类型概念节点);
            if (!类型) return 结果;
            新特征值域比较输入 结果域输入{类型->节点, 类型->单位, {}};
            std::optional<特征概念信息> 域;
            std::vector<特征概念I64闭区间> 直接区间组;
            const std::vector<特征概念I64闭区间>* 区间组 = nullptr;
            if (目标项->精确数值域) {
                结果域输入.材料 = 新特征直接I64值域比较材料{*目标项->精确数值域};
                直接区间组.push_back(*目标项->精确数值域);
                区间组 = &直接区间组;
            } else {
                域 = 特征概念服务_.获取特征概念(*目标项->特征值域概念节点);
                if (!域 || 取得特征概念类型根(特征概念服务_, 域->节点)
                    != std::optional<稳定编码>{需求->目标.目标特征类型概念节点}) return 结果;
                if (域->单位 != 类型->单位) { 有未支持 = true; continue; }
                结果域输入.材料 = 新特征概念值域比较材料{域->节点};
                区间组 = std::get_if<std::vector<特征概念I64闭区间>>(&域->值域);
            }
            if (类型->单位 != 特征->实例单位) { 有未支持 = true; continue; }
            std::optional<bool> 保证;
            std::vector<特征概念I64闭区间> 所需条件域组;
            if (类型->材料类型 == 特征概念材料物理类型::I64标量
                && 是条件包含目标(需求->目标, 特征概念服务_, 二次特征概念服务_)) {
                const auto 比较 = 特征服务_.比较特征值域(结果域输入,
                    {需求->目标.目标特征类型概念节点, 特征->实例单位,
                        新特征概念值域比较材料{*需求->目标.目标值域概念节点}});
                if (比较.状态 == 新特征比较状态::资源失败) {
                    结果.状态 = 新任务方法选择状态::资源失败; return 结果;
                }
                if (比较.状态 != 新特征比较状态::已完成 || !比较.关系编码) {
                    有未支持 = true; continue;
                }
                const auto 关系 = *比较.关系编码;
                if (关系 == static_cast<std::int64_t>(特征概念值域关系::左包含右)
                    || 关系 == static_cast<std::int64_t>(特征概念值域关系::相交)) {
                    const auto 目标域 = 特征概念服务_.获取特征概念(*需求->目标.目标值域概念节点);
                    const auto* 目标区间 = 目标域
                        ? std::get_if<std::vector<特征概念I64闭区间>>(&目标域->值域) : nullptr;
                    if (!区间组 || 区间组->size() != 1 || !目标区间 || 目标区间->size() != 1
                        || 目标域->单位 != 类型->单位 || 项.配对.线性关系组.empty()
                        || (项.配对.条件.关系规则 != 新方法项关系规则::单项
                            && 项.配对.条件.关系规则 != 新方法项关系规则::全部成立)) {
                        有未支持 = true; continue;
                    }
                    const auto 目标序号 = static_cast<std::size_t>(目标项 - 定义.结果项组.data());
                    新任务线性条件输入请求 请求;
                    请求.条件域组.resize(项.配对.条件.条件项组.size());
                    std::vector<bool> 覆盖(请求.条件域组.size(), false);
                    请求.结果约束组.push_back({目标序号, 区间组->front(), 目标区间->front()});
                    for (const auto& 映射 : 项.配对.线性关系组) {
                        if (映射.结果序号 != 目标序号) continue;
                        if (映射.条件序号 >= 请求.条件域组.size()
                            || 映射.原结果域 != 区间组->front()) return 结果;
                        请求.条件域组[映射.条件序号] = 映射.原条件域;
                        覆盖[映射.条件序号] = true;
                        新任务条件结果线性方向 方向;
                        switch (映射.方向) {
                        case 新方法条件结果线性方向::同向: 方向 = 新任务条件结果线性方向::同向; break;
                        case 新方法条件结果线性方向::反向: 方向 = 新任务条件结果线性方向::反向; break;
                        case 新方法条件结果线性方向::不变: 方向 = 新任务条件结果线性方向::不变; break;
                        default: return 结果;
                        }
                        请求.关系组.push_back({映射.条件序号, 目标序号, 方向});
                    }
                    if (覆盖.empty() || std::any_of(覆盖.begin(), 覆盖.end(), [](bool x) { return !x; })) {
                        有未支持 = true; continue;
                    }
                    const auto 反求 = 按线性关系计算任务条件输入(请求);
                    if (反求.状态 == 新任务线性条件输入状态::目标无交集) continue;
                    if (反求.状态 == 新任务线性条件输入状态::资源失败) {
                        结果.状态 = 新任务方法选择状态::资源失败; return 结果;
                    }
                    if (反求.状态 == 新任务线性条件输入状态::不支持无穷域
                        || 反求.状态 == 新任务线性条件输入状态::关系材料不足) {
                        有未支持 = true; continue;
                    }
                    if (反求.状态 != 新任务线性条件输入状态::已算出
                        || 反求.条件输入组.size() != 请求.条件域组.size()) return 结果;
                    for (std::size_t i = 0; i < 反求.条件输入组.size(); ++i) {
                        if (反求.条件输入组[i].条件序号 != i) return 结果;
                        所需条件域组.push_back(反求.条件输入组[i].需要值域);
                    }
                    保证 = true;
                } else {
                    保证 = 关系 == static_cast<std::int64_t>(特征概念值域关系::相等)
                        || 关系 == static_cast<std::int64_t>(特征概念值域关系::右包含左);
                }
            } else if (域) {
                // 其它比较关系保留正式概念路径，不把“大于/不包含”等解释为落域。
                保证 = 结果域保证满足需求(特征概念服务_, 二次特征概念服务_, 需求->目标, *域);
            }
            if (!保证) { 有未支持 = true; continue; }
            auto 域方向 = 预计变化方向::不明确;
            if (当前I64 && 区间组 && !区间组->empty()) {
                if (区间组->front().下界 > *当前I64) 域方向 = 预计变化方向::增加;
                else if (区间组->back().上界 < *当前I64) 域方向 = 预计变化方向::减少;
                else if (区间组->front().下界 == *当前I64 && 区间组->back().上界 == *当前I64)
                    域方向 = 预计变化方向::不变;
            }
            if (声明方向 != 预计变化方向::不明确 && 域方向 != 预计变化方向::不明确
                && 声明方向 != 域方向) return 结果;
            if (*保证) {
                适用组.push_back({&项, &*方法筹办, true, std::nullopt, std::move(所需条件域组)});
                continue;
            }
            if (所需方向 == 预计变化方向::不明确) continue;
            if (域方向 != 所需方向) {
                if (声明方向 == 所需方向) 有未支持 = true;
                continue;
            }
            if (!当前I64 || !区间组 || 区间组->empty()) { 有未知幅度 = true; continue; }
            const auto 端点 = 所需方向 == 预计变化方向::增加 ? 区间组->back().上界 : 区间组->front().下界;
            if (端点 == (std::numeric_limits<std::int64_t>::max)()
                || 端点 == (std::numeric_limits<std::int64_t>::min)()) {
                有未知幅度 = true; continue;
            }
            适用组.push_back({&项, &*方法筹办, false, 端点, {}});
        }
        if (有未支持) { 结果.状态 = 新任务方法选择状态::结果定义暂不支持; return 结果; }
        const bool 有保证 = std::any_of(适用组.begin(), 适用组.end(), [](const auto& x) { return x.保证; });
        if (!有保证 && 有未知幅度) { 结果.状态 = 新任务方法选择状态::幅度材料不足; return 结果; }
        if (有保证) std::erase_if(适用组, [](const auto& x) { return !x.保证; });
        else if (!适用组.empty()) {
            auto 最优端点 = *适用组.front().端点;
            for (const auto& 项 : 适用组) {
                if (所需方向 == 预计变化方向::增加 ? *项.端点 > 最优端点 : *项.端点 < 最优端点)
                    最优端点 = *项.端点;
            }
            std::erase_if(适用组, [&](const auto& x) { return x.端点 != 最优端点; });
        }
        if (适用组.empty()) { 结果.状态 = 新任务方法选择状态::无适用结果; return 结果; }
        if (适用组.size() != 1) { 结果.状态 = 新任务方法选择状态::匹配不唯一; return 结果; }
        const auto& 最优 = 适用组.front();
        auto 配对 = 最优.配对->配对;
        auto 采用=任务执行组织存储::取得采用定义(组织,最优.配对->定位);
        采用.参数绑定组=最优.筹办->参数绑定组;
        if (!最优.所需条件域组.empty()) {
            if(最优.所需条件域组.size()!=采用.需要条件.条件项组.size())throw std::logic_error("反求条件项数异义");
            for(std::size_t 序号=0;序号<最优.所需条件域组.size();++序号){
                auto& 项=采用.需要条件.条件项组[序号];项.特征值域概念节点.reset();项.精确数值域=最优.所需条件域组[序号];
            }
            配对.条件=采用.需要条件;
        }
        const auto 有效条件=任务执行组织存储::计算采用需要条件(采用,*需求,特征概念服务_,二次特征概念服务_,特征服务_);
        if(!有效条件){结果.状态=新任务方法选择状态::结果定义暂不支持;return 结果;}
        采用.需要条件=*有效条件;配对.条件=采用.需要条件;
        // 根据固定同D输入重新读取，不复用选择前的条件结论。
        const auto 新筹办 = 核验任务条件和参数内部(任务节点,&采用,false);
        if (新筹办.状态 != 新任务治理条件筹办状态::已完成 || !新筹办.候选已完整核验) {
            结果.状态 = 新筹办.状态 == 新任务治理条件筹办状态::资源失败
                ? 新任务方法选择状态::资源失败 : 新任务方法选择状态::结构不一致;
            return 结果;
        }
        const auto 新方法筹办 = std::find_if(新筹办.方法筹办组.begin(), 新筹办.方法筹办组.end(),
            [&](const auto& x) { return x.方法节点 == 最优.配对->方法节点; });
        if (新方法筹办 == 新筹办.方法筹办组.end()) return 结果;
        const auto 条件 = std::find_if(新方法筹办->条件结果组.begin(), 新方法筹办->条件结果组.end(),
            [&](const auto& x) { return x.配对节点 == 配对.配对节点 && x.条件节点 == 配对.条件节点
                && x.结果节点 == 配对.结果节点; });
        if (条件 == 新方法筹办->条件结果组.end() || 条件->状态 == 新任务方法条件项核验状态::结构缺口) return 结果;
        if (条件->状态 == 新任务方法条件项核验状态::资源失败) {
            结果.状态 = 新任务方法选择状态::资源失败; return 结果;
        }
        结果.参数绑定组 = 新方法筹办->参数绑定组;
        采用.参数绑定组=结果.参数绑定组;结果.本能采用=std::move(采用);
        结果.条件核验组 = {*条件};
        结果.方法节点 = 最优.配对->方法节点;
        结果.配对节点 = 配对.配对节点;
        结果.条件节点 = 配对.条件节点;
        结果.结果节点 = 配对.结果节点;
        结果.保证满足目标 = 最优.保证;
        结果.有利结果端点 = 最优.端点;
        结果.状态 = 条件->状态 == 新任务方法条件项核验状态::具备
            ? 新任务方法选择状态::已选定 : 新任务方法选择状态::等待条件;
        return 结果;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"选择任务方法结果", L"任务执行组织选择工程故障");
    }
}

新任务条件子需求提议 新_任务类::提议任务条件子需求(
    稳定编码 任务节点) const noexcept {
    新任务条件子需求提议 结果;
    结果.任务节点 = 任务节点;
    const auto 拒绝 = [&](新任务条件子需求提议状态 状态) {
        结果.状态 = 状态;
        结果.全部条件项组.clear();
        结果.子需求项组.clear();
        return std::move(结果);
    };
    try {
        std::lock_guard 锁(新任务互斥);
        const auto 任务 = 获取任务(任务节点);
        if (!治理已初始化_ || !任务) return 结果;
        if (任务->当前状态 != 新任务状态::待检查条件)
            return 拒绝(新任务条件子需求提议状态::任务状态已变化);
        const auto 需求 = 需求服务_.获取需求(任务->需求节点);
        if (!需求 || 查询需求任务(需求->节点) != 任务节点) return 结果;
        结果.父需求节点 = 需求->节点;
        结果.父需求目标存在节点 = 需求->目标存在节点;
        结果.父需求目标 = 需求->目标;
        const auto 选择 = 选择任务方法结果(任务节点);
        switch (选择.状态) {
        case 新任务方法选择状态::已选定:
        case 新任务方法选择状态::等待条件: break;
        case 新任务方法选择状态::任务状态已变化:
            return 拒绝(新任务条件子需求提议状态::任务状态已变化);
        case 新任务方法选择状态::资源失败:
            return 拒绝(新任务条件子需求提议状态::资源失败);
        case 新任务方法选择状态::实例未建立:
        case 新任务方法选择状态::幅度材料不足:
            return 拒绝(新任务条件子需求提议状态::材料未到);
        case 新任务方法选择状态::匹配不唯一:
        case 新任务方法选择状态::无适用结果:
        case 新任务方法选择状态::结果定义暂不支持:
            return 拒绝(新任务条件子需求提议状态::暂不支持);
        case 新任务方法选择状态::目标已满足:
            return 拒绝(新任务条件子需求提议状态::来源已变化);
        default: return 结果;
        }
        if(!选择.本能采用||!选择.方法节点||!选择.配对节点||!选择.条件节点||!选择.结果节点)
            return 拒绝(新任务条件子需求提议状态::材料未到);
        const auto 组织=读取任务方法执行组织(任务节点);
        if(!组织.组织)return 拒绝(新任务条件子需求提议状态::来源已变化);
        任务执行组织存储::核对采用定义(*组织.组织,*选择.本能采用);
        const auto 方法=任务执行组织存储::读取来源视图(*组织.组织,*选择.方法节点,方法服务_,&*选择.本能采用);
        if(!方法||方法->条件结果组.size()!=1)return 拒绝(新任务条件子需求提议状态::来源已变化);
        const auto* 配对=&方法->条件结果组.front();
        结果.方法节点=*选择.方法节点;结果.配对节点=*选择.配对节点;
        结果.条件结果=*配对;结果.参数绑定组=选择.参数绑定组;结果.本能采用=选择.本能采用;
        const auto& 条件 = 配对->条件;
        if (条件.关系规则 != 新方法项关系规则::单项
            && 条件.关系规则 != 新方法项关系规则::全部成立)
            return 拒绝(新任务条件子需求提议状态::暂不支持);
        if (条件.条件项组.empty()
            || (条件.关系规则 == 新方法项关系规则::单项 && 条件.条件项组.size() != 1)) return 结果;
        for (const auto& 条件项 : 条件.条件项组) {
            if (条件项.二次特征概念节点 || 条件项.状态或动态概念节点)
                return 拒绝(新任务条件子需求提议状态::暂不支持);
        }
        const auto 存在概念兼容 = [&](稳定编码 实际, 稳定编码 要求) {
            std::vector<稳定编码> 待查{实际};
            std::set<稳定编码> 已查;
            while (!待查.empty()) {
                const auto 当前 = 待查.back();
                待查.pop_back();
                if (!已查.insert(当前).second) continue;
                if (当前 == 要求) return true;
                const auto 上位组 = 存在概念服务_.查询上位存在概念(当前);
                待查.insert(待查.end(), 上位组.begin(), 上位组.end());
            }
            return false;
        };
        const auto 同目标语义 = [](const 新任务条件子需求项& 左,
                                      const 新任务条件子需求项& 右) {
            return 左.目标存在节点 == 右.目标存在节点
                && 左.目标特征类型概念节点 == 右.目标特征类型概念节点
                && 左.目标区间组 == 右.目标区间组 && 左.单位 == 右.单位
                && 左.集合目标 == 右.集合目标;
        };
        for (std::size_t 序号 = 0; 序号 < 条件.条件项组.size(); ++序号) {
            const auto& 条件项 = 条件.条件项组[序号];
            const 新方法参数角色绑定* 绑定 = nullptr;
            for (const auto& 候选 : 选择.参数绑定组) {
                if (候选.参数角色节点 != 条件项.参数角色节点) continue;
                if (绑定) return 拒绝(新任务条件子需求提议状态::结构不一致);
                绑定 = &候选;
            }
            if (!绑定 || !绑定->实际存在节点)
                return 拒绝(新任务条件子需求提议状态::材料未到);
            std::vector<稳定编码> 特征节点组;
            稳定编码 专属概念;
            if (场景服务_.是场景节点(*绑定->实际存在节点)) {
                const auto 场景 = 场景服务_.获取场景(*绑定->实际存在节点);
                if (!场景) return 拒绝(新任务条件子需求提议状态::结构不一致);
                特征节点组 = 场景->特征节点组;
                专属概念 = 场景->专属存在概念节点;
            } else {
                const auto 存在 = 存在服务_.获取存在(*绑定->实际存在节点);
                if (!存在) return 拒绝(新任务条件子需求提议状态::结构不一致);
                特征节点组 = 存在->特征节点组;
                专属概念 = 存在->专属存在概念节点;
            }
            if (条件项.存在概念节点 && !存在概念兼容(专属概念, *条件项.存在概念节点))
                return 拒绝(新任务条件子需求提议状态::暂不支持);
            if (!条件项.特征类型概念节点 && !条件项.特征值域概念节点 && !条件项.精确数值域) continue;
            auto 目标域节点 = 条件项.特征值域概念节点;
            if (条件项.精确数值域) {
                if (目标域节点 || !条件项.特征类型概念节点 || !精确域有效(*条件项.精确数值域))
                    return 拒绝(新任务条件子需求提议状态::结构不一致);
                const auto 根 = 取得特征概念类型根(特征概念服务_, *条件项.特征类型概念节点);
                const auto 类型 = 根 ? 特征概念服务_.获取特征概念(*根) : std::nullopt;
                if (!类型 || 类型->材料类型 != 特征概念材料物理类型::I64标量)
                    return 拒绝(新任务条件子需求提议状态::结构不一致);
                const std::vector<特征概念I64闭区间> 域组{*条件项.精确数值域};
                const 特征概念定义 定义{类型->材料类型, 域组, 类型->单位,
                    类型->比较规则, 类型->聚合规则, {}};
                const auto 节点 = 特征概念服务_.建立或取得特征概念(定义, 特征值服务_, *根);
                if (!有效(节点)) return 拒绝(新任务条件子需求提议状态::资源失败);
                const auto 读回 = 特征概念服务_.获取特征概念(节点);
                if (!读回 || 读回->值域 != 特征概念值域{域组} || 读回->单位 != 类型->单位
                    || 取得特征概念类型根(特征概念服务_, 节点) != 根)
                    return 拒绝(新任务条件子需求提议状态::结构不一致);
                目标域节点 = 节点;
            }
            if (!目标域节点)
                return 拒绝(新任务条件子需求提议状态::暂不支持);
            const auto 域 = 特征概念服务_.获取特征概念(*目标域节点);
            const auto 类型根 = 取得特征概念类型根(特征概念服务_, *目标域节点);
            if (!域 || !类型根) return 拒绝(新任务条件子需求提议状态::结构不一致);
            const auto* 集合域 = std::get_if<特征概念任务集合值域>(&域->值域);
            if (!集合域 && 域->材料类型 != 特征概念材料物理类型::I64标量)
                return 拒绝(新任务条件子需求提议状态::暂不支持);
            const auto* 区间组 = std::get_if<std::vector<特征概念I64闭区间>>(&域->值域);
            if ((!集合域 && (!区间组 || !是规范条件I64区间组(*区间组)))
                || (条件项.特征类型概念节点
                    && 取得特征概念类型根(特征概念服务_, *条件项.特征类型概念节点) != 类型根))
                return 拒绝(新任务条件子需求提议状态::结构不一致);
            std::optional<新特征信息> 实际特征;
            for (const auto 特征节点 : 特征节点组) {
                const auto 特征 = 特征服务_.获取特征(特征节点);
                if (!特征) return 拒绝(新任务条件子需求提议状态::结构不一致);
                const auto 实际根 = 取得特征概念类型根(特征概念服务_, 特征->特征概念节点);
                if (!实际根) return 拒绝(新任务条件子需求提议状态::结构不一致);
                if (实际根 != 类型根) continue;
                if (实际特征) return 拒绝(新任务条件子需求提议状态::结构不一致);
                实际特征 = *特征;
            }
            if (!实际特征 || !实际特征->当前值)
                return 拒绝(新任务条件子需求提议状态::材料未到);
            const auto* 当前值 = std::get_if<std::int64_t>(&*实际特征->当前值);
            if (!集合域 && !当前值) return 拒绝(新任务条件子需求提议状态::暂不支持);
            if (域->单位 != 实际特征->实例单位)
                return 拒绝(新任务条件子需求提议状态::结构不一致);
            新任务条件子需求项 项{static_cast<std::uint64_t>(序号),
                *绑定->实际存在节点, 实际特征->节点, *类型根, 域->节点,
                区间组 ? *区间组 : std::vector<特征概念I64闭区间>{}, 域->单位,
                集合域 ? std::optional<特征概念任务集合值域>{*集合域} : std::nullopt};
            结果.全部条件项组.push_back(项);
            bool 已满足 = false;
            if (集合域) {
                const auto 比较 = 特征服务_.比较任务集合条件(*类型根, *实际特征->当前值, *集合域);
                if (比较.状态 != 特征概念规则状态::已完成 || !比较.比较值)
                    return 拒绝(比较.状态 == 特征概念规则状态::资源失败
                        ? 新任务条件子需求提议状态::资源失败
                        : 比较.状态 == 特征概念规则状态::读取接口未装配
                            ? 新任务条件子需求提议状态::材料未到 : 新任务条件子需求提议状态::结构不一致);
                已满足 = *比较.比较值 == 1;
            } else 已满足 = std::any_of(区间组->begin(), 区间组->end(),
                [&](const auto& 区间) { return 区间.下界 <= *当前值 && *当前值 <= 区间.上界; });
            if (!已满足 && std::none_of(结果.子需求项组.begin(), 结果.子需求项组.end(),
                    [&](const auto& 既有) { return 同目标语义(既有, 项); }))
                结果.子需求项组.push_back(std::move(项));
        }
        // 全部成立必须在每个对象/FT上有共同可达的值域；不把矛盾条件发布为子D。
        for (std::size_t 序号 = 0; 序号 < 结果.全部条件项组.size(); ++序号) {
            const auto& 起项 = 结果.全部条件项组[序号];
            if (起项.集合目标) continue;
            auto 交集 = 起项.目标区间组;
            for (std::size_t 后序号 = 序号 + 1; 后序号 < 结果.全部条件项组.size(); ++后序号) {
                const auto& 后项 = 结果.全部条件项组[后序号];
                if (起项.目标存在节点 != 后项.目标存在节点
                    || 起项.目标特征类型概念节点 != 后项.目标特征类型概念节点) continue;
                if (起项.单位 != 后项.单位) return 拒绝(新任务条件子需求提议状态::结构不一致);
                if (后项.集合目标) return 拒绝(新任务条件子需求提议状态::结构不一致);
                std::vector<特征概念I64闭区间> 新交集;
                for (const auto& 左 : 交集) for (const auto& 右 : 后项.目标区间组) {
                    const auto 下界 = (std::max)(左.下界, 右.下界);
                    const auto 上界 = (std::min)(左.上界, 右.上界);
                    if (下界 <= 上界) 新交集.push_back({下界, 上界});
                }
                if (新交集.empty()) return 拒绝(新任务条件子需求提议状态::暂不支持);
                交集 = std::move(新交集);
            }
        }
        // 保留全部条件项组作为稳定来源投影；同对象/FT的集合目标以AND合并，
        // 包括当前已经满足的部分，不能只保留本次缺少的成员。
        std::erase_if(结果.子需求项组, [](const auto& 项) { return 项.集合目标.has_value(); });
        std::set<std::pair<稳定编码, 稳定编码>> 已合并集合;
        for (const auto& 首项 : 结果.全部条件项组) {
            if (!首项.集合目标 || !已合并集合.insert(
                    {首项.目标存在节点, 首项.目标特征类型概念节点}).second) continue;
            auto 合并 = *首项.集合目标;
            for (const auto& 其他 : 结果.全部条件项组) {
                if (其他.目标存在节点 != 首项.目标存在节点
                    || 其他.目标特征类型概念节点 != 首项.目标特征类型概念节点) continue;
                if (!其他.集合目标 || 其他.单位 || 其他.实际特征节点 != 首项.实际特征节点)
                    return 拒绝(新任务条件子需求提议状态::结构不一致);
                合并.至少一个任务 = 合并.至少一个任务 || 其他.集合目标->至少一个任务;
                合并.必含任务节点组.insert(合并.必含任务节点组.end(),
                    其他.集合目标->必含任务节点组.begin(), 其他.集合目标->必含任务节点组.end());
                合并.必含需求任务组.insert(合并.必含需求任务组.end(),
                    其他.集合目标->必含需求任务组.begin(), 其他.集合目标->必含需求任务组.end());
                合并.必含服务要求任务组.insert(合并.必含服务要求任务组.end(),
                    其他.集合目标->必含服务要求任务组.begin(), 其他.集合目标->必含服务要求任务组.end());
            }
            const auto 类型 = 特征概念服务_.获取特征概念(首项.目标特征类型概念节点);
            if (!类型) return 拒绝(新任务条件子需求提议状态::结构不一致);
            const 特征概念定义 定义{类型->材料类型, 合并, 类型->单位,
                类型->比较规则, 类型->聚合规则, {}};
            const auto 根比较 = 特征概念服务_.比较值域(类型->材料类型, 类型->值域, 合并);
            if (根比较.状态 != 特征概念规则状态::已完成 || !根比较.关系)
                return 拒绝(根比较.状态 == 特征概念规则状态::资源失败
                    ? 新任务条件子需求提议状态::资源失败 : 新任务条件子需求提议状态::结构不一致);
            const auto 合并节点 = 根比较.关系 == 特征概念值域关系::相等 ? 类型->节点
                : 特征概念服务_.建立或取得特征概念(
                    定义, 特征值服务_, 首项.目标特征类型概念节点);
            if (!有效(合并节点)) return 拒绝(新任务条件子需求提议状态::资源失败);
            const auto 域 = 特征概念服务_.获取特征概念(合并节点);
            const auto* 规范域 = 域 ? std::get_if<特征概念任务集合值域>(&域->值域) : nullptr;
            if (!规范域) return 拒绝(新任务条件子需求提议状态::结构不一致);
            const auto 特征 = 特征服务_.获取特征(首项.实际特征节点);
            if (!特征 || !特征->当前值) return 拒绝(新任务条件子需求提议状态::材料未到);
            const auto 比较 = 特征服务_.比较任务集合条件(
                首项.目标特征类型概念节点, *特征->当前值, *规范域);
            if (比较.状态 != 特征概念规则状态::已完成 || !比较.比较值)
                return 拒绝(比较.状态 == 特征概念规则状态::资源失败
                    ? 新任务条件子需求提议状态::资源失败
                    : 比较.状态 == 特征概念规则状态::读取接口未装配
                        ? 新任务条件子需求提议状态::材料未到 : 新任务条件子需求提议状态::结构不一致);
            if (*比较.比较值 == 0) {
                auto 项 = 首项;
                项.目标值域概念节点 = 合并节点;
                项.集合目标 = *规范域;
                结果.子需求项组.push_back(std::move(项));
            }
        }
        std::set<稳定编码> 已查祖先;
        auto 祖先节点 = std::optional<稳定编码>{需求->节点};
        while (祖先节点) {
            if (!已查祖先.insert(*祖先节点).second)
                return 拒绝(新任务条件子需求提议状态::结构不一致);
            const auto 祖先 = 需求服务_.获取需求(*祖先节点);
            if (!祖先) return 拒绝(新任务条件子需求提议状态::结构不一致);
            if (祖先->目标.目标形式 == 新需求目标形式::值) {
                if (!祖先->目标.目标值域概念节点 || !祖先->目标.目标比较关系概念节点)
                    return 拒绝(新任务条件子需求提议状态::结构不一致);
                const auto 域 = 特征概念服务_.获取特征概念(*祖先->目标.目标值域概念节点);
                const auto 比较 = 二次特征概念服务_.获取二次特征概念(*祖先->目标.目标比较关系概念节点);
                if (!域 || !比较) return 拒绝(新任务条件子需求提议状态::结构不一致);
                if (是条件包含目标(祖先->目标, 特征概念服务_, 二次特征概念服务_)) {
                    for (const auto& 项 : 结果.子需求项组) {
                        if (祖先->目标存在节点 == 项.目标存在节点
                            && 祖先->目标.目标特征类型概念节点 == 项.目标特征类型概念节点) {
                            bool 等价 = 条件投影一致(项, *域);
                            if (项.集合目标 && std::holds_alternative<特征概念任务集合值域>(域->值域)) {
                                const auto 比较 = 特征概念服务_.比较值域(
                                    域->材料类型, 域->值域, 特征概念值域{*项.集合目标});
                                if (比较.状态 != 特征概念规则状态::已完成 || !比较.关系)
                                    return 拒绝(比较.状态 == 特征概念规则状态::资源失败
                                        ? 新任务条件子需求提议状态::资源失败
                                        : 新任务条件子需求提议状态::结构不一致);
                                等价 = 域->单位 == 项.单位 && *比较.关系 == 特征概念值域关系::相等;
                            }
                            if (等价) return 拒绝(新任务条件子需求提议状态::形成循环依赖);
                        }
                    }
                }
            }
            if (!祖先->父需求节点 && !祖先->根角色)
                return 拒绝(新任务条件子需求提议状态::结构不一致);
            if (祖先->根角色 && 需求服务_.查询根需求(*祖先->根角色) != 祖先->节点)
                return 拒绝(新任务条件子需求提议状态::结构不一致);
            祖先节点 = 祖先->父需求节点;
        }
        结果.状态 = 结果.子需求项组.empty()
            ? 新任务条件子需求提议状态::无需子需求 : 新任务条件子需求提议状态::已形成;
        return 结果;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"提议任务条件子需求", L"任务采用条件读取工程故障");
    }
}

新任务条件子需求就绪结果 新_任务类::复核条件子需求就绪(
    稳定编码 任务节点,
    const std::vector<新任务已确认条件子需求>& 子需求组) const noexcept {
    新任务条件子需求就绪结果 结果;
    try {
        std::lock_guard 锁(新任务互斥);
        const auto 任务 = 获取任务(任务节点);
        if (!治理已初始化_ || !任务) return 结果;
        if (任务->当前状态 != 新任务状态::待检查条件) {
            结果.状态 = 新任务条件子需求就绪状态::任务状态已变化;
            return 结果;
        }
        const auto 父需求 = 需求服务_.获取需求(任务->需求节点);
        if (!父需求 || 查询需求任务(父需求->节点) != 任务节点) return 结果;
        bool 全部满足 = true;
        std::set<稳定编码> 已查需求;
        for (const auto& 关联 : 子需求组) {
            const auto& 项 = 关联.原提议;
            const auto 子需求 = 需求服务_.获取需求(关联.需求节点);
            if (!已查需求.insert(关联.需求节点).second || !子需求
                || 子需求->父需求节点 != 父需求->节点
                || 子需求->来源类型 != 新需求来源类型::任务子目标
                || 子需求->来源承接节点 != 任务节点
                || 子需求->目标存在节点 != 项.目标存在节点
                || 子需求->目标.目标特征类型概念节点 != 项.目标特征类型概念节点
                || 子需求->目标.目标值域概念节点 != 项.目标值域概念节点
                || !是条件包含目标(子需求->目标, 特征概念服务_, 二次特征概念服务_)) return 结果;
            const auto 域 = 特征概念服务_.获取特征概念(项.目标值域概念节点);
            if (!域 || !条件投影一致(项, *域)) {
                结果.状态 = 新任务条件子需求就绪状态::来源已变化;
                return 结果;
            }
            const auto 当前 = 需求服务_.复核当前满足(关联.需求节点);
            if (当前.实际特征节点 && 当前.实际特征节点 != 项.实际特征节点) {
                结果.状态 = 新任务条件子需求就绪状态::来源已变化;
                return 结果;
            }
            switch (当前.状态) {
            case 新需求当前满足状态::已满足: break;
            case 新需求当前满足状态::需增加:
            case 新需求当前满足状态::需减少:
            case 新需求当前满足状态::未满足但无有向差值: 全部满足 = false; break;
            case 新需求当前满足状态::目标特征尚未形成:
            case 新需求当前满足状态::当前值不存在:
                结果.状态 = 新任务条件子需求就绪状态::材料未到; return 结果;
            case 新需求当前满足状态::资源失败:
                结果.状态 = 新任务条件子需求就绪状态::资源失败; return 结果;
            default: return 结果;
            }
            if (子需求->活动状态 == 新需求活动状态::失活) 全部满足 = false;
        }
        结果.当前选择 = 选择任务方法结果(任务节点);
        switch (结果.当前选择->状态) {
        case 新任务方法选择状态::已选定:
            结果.全部需求已满足 = 全部满足;
            结果.状态 = 全部满足 ? 新任务条件子需求就绪状态::已就绪
                : 新任务条件子需求就绪状态::等待条件;
            return 结果;
        case 新任务方法选择状态::等待条件:
        case 新任务方法选择状态::匹配不唯一:
        case 新任务方法选择状态::无适用结果:
        case 新任务方法选择状态::结果定义暂不支持:
            结果.全部需求已满足 = 全部满足;
            结果.状态 = 新任务条件子需求就绪状态::等待条件; return 结果;
        case 新任务方法选择状态::资源失败:
            结果.状态 = 新任务条件子需求就绪状态::资源失败; return 结果;
        case 新任务方法选择状态::实例未建立:
        case 新任务方法选择状态::幅度材料不足:
            结果.状态 = 新任务条件子需求就绪状态::材料未到; return 结果;
        case 新任务方法选择状态::任务状态已变化:
            结果.状态 = 新任务条件子需求就绪状态::任务状态已变化; return 结果;
        case 新任务方法选择状态::目标已满足:
            结果.状态 = 新任务条件子需求就绪状态::来源已变化; return 结果;
        default: return 结果;
        }
    } catch (...) {
        结果.状态 = 新任务条件子需求就绪状态::资源失败;
        结果.全部需求已满足 = false;
        结果.当前选择.reset();
        return 结果;
    }
}

新需求方向一致性结果 新_任务类::判断父子需求方向一致(
    稳定编码 父需求节点,
    稳定编码 子需求节点) const noexcept {
    新需求方向一致性结果 结果;
    结果.父需求节点 = 父需求节点;
    结果.子需求节点 = 子需求节点;
    try {
        std::lock_guard 锁(新任务互斥);
        const auto 父需求 = 需求服务_.获取需求(父需求节点);
        const auto 子需求 = 需求服务_.获取需求(子需求节点);
        if (!父需求 || !子需求 || 子需求->父需求节点 != 父需求节点) {
            结果.状态 = 新需求方向一致性状态::结构不一致;
            return 结果;
        }
        if (父需求->活动状态 == 新需求活动状态::已满足) {
            结果.状态 = 新需求方向一致性状态::父需求已满足;
            return 结果;
        }
        if (父需求->活动状态 == 新需求活动状态::失活) {
            结果.状态 = 新需求方向一致性状态::父需求失活;
            return 结果;
        }
        if (子需求->活动状态 == 新需求活动状态::已满足) {
            结果.状态 = 新需求方向一致性状态::子需求已满足;
            return 结果;
        }
        if (子需求->活动状态 == 新需求活动状态::失活) {
            结果.状态 = 新需求方向一致性状态::子需求失活;
            return 结果;
        }
        const auto 父当前 = 需求服务_.复核当前满足(父需求节点);
        if (父当前.状态 == 新需求当前满足状态::已满足) {
            结果.状态 = 新需求方向一致性状态::父需求已满足;
            return 结果;
        }
        if (父当前.状态 == 新需求当前满足状态::资源失败) {
            结果.状态 = 新需求方向一致性状态::资源失败;
            return 结果;
        }

        const auto 当前结果可用于方向判断 = [](新需求当前满足状态 状态) {
            return 状态 == 新需求当前满足状态::需增加
                || 状态 == 新需求当前满足状态::需减少
                || 状态 == 新需求当前满足状态::未满足但无有向差值
                || 状态 == 新需求当前满足状态::需要执行前后比较;
        };
        if (!当前结果可用于方向判断(父当前.状态)) {
            结果.状态 = 父当前.状态 == 新需求当前满足状态::目标特征尚未形成
                    || 父当前.状态 == 新需求当前满足状态::当前值不存在
                ? 新需求方向一致性状态::父方法材料未到
                : 新需求方向一致性状态::结构不一致;
            return 结果;
        }

        const auto 查询唯一任务 = [&](稳定编码 需求节点,
                                        bool& 多任务,
                                        std::optional<新需求方向一致性状态>& 查询失败)
            -> std::optional<稳定编码> {
            多任务 = false;
            查询失败.reset();
            const auto 全部任务 = 查询全部任务();
            if (全部任务.状态 != 新任务操作状态::已找到) {
                查询失败 = 全部任务.状态 == 新任务操作状态::资源失败
                    ? 新需求方向一致性状态::资源失败
                    : 新需求方向一致性状态::结构不一致;
                return std::nullopt;
            }
            std::optional<稳定编码> 命中;
            for (const auto 任务节点 : 全部任务.任务节点组) {
                const auto 任务 = 获取任务(任务节点);
                if (!任务) {
                    多任务 = true;
                    return std::nullopt;
                }
                if (任务->需求节点 != 需求节点) continue;
                if (命中) {
                    多任务 = true;
                    return std::nullopt;
                }
                命中 = 任务节点;
            }
            return 命中;
        };
        bool 父多任务 = false;
        std::optional<新需求方向一致性状态> 父任务查询失败;
        const auto 父任务节点 = 查询唯一任务(
            父需求节点, 父多任务, 父任务查询失败);
        if (!父任务节点) {
            结果.状态 = 父任务查询失败 ? *父任务查询失败 : (父多任务
                ? 新需求方向一致性状态::结构不一致
                : 新需求方向一致性状态::父任务不存在);
            return 结果;
        }
        结果.来源任务节点 = *父任务节点;
        if (子需求->来源类型 != 新需求来源类型::任务子目标
            || 子需求->来源承接节点 != 父任务节点) {
            结果.状态 = 新需求方向一致性状态::来源任务不一致;
            return 结果;
        }
        bool 子多任务 = false;
        std::optional<新需求方向一致性状态> 子任务查询失败;
        const auto 子任务节点 = 查询唯一任务(
            子需求节点, 子多任务, 子任务查询失败);
        if (!子任务节点) {
            结果.状态 = 子任务查询失败 ? *子任务查询失败 : (子多任务
                ? 新需求方向一致性状态::结构不一致
                : 新需求方向一致性状态::子任务不存在);
            return 结果;
        }
        结果.子任务节点 = *子任务节点;

        const auto 映射筹办状态 = [](新任务治理条件筹办状态 状态,
                                      bool 父侧) {
            if (状态 == 新任务治理条件筹办状态::资源失败) {
                return 新需求方向一致性状态::资源失败;
            }
            if (状态 == 新任务治理条件筹办状态::任务状态已变化
                || 状态 == 新任务治理条件筹办状态::候选材料不存在) {
                return 父侧 ? 新需求方向一致性状态::父方法材料未到
                    : 新需求方向一致性状态::子方法材料未到;
            }
            return 新需求方向一致性状态::结构不一致;
        };
        const auto 父筹办 = 筹办任务条件和参数(*父任务节点);
        if (父筹办.状态 != 新任务治理条件筹办状态::已完成
            || !父筹办.候选已完整核验) {
            结果.状态 = 映射筹办状态(父筹办.状态, true);
            return 结果;
        }
        const auto 子筹办 = 筹办任务条件和参数(*子任务节点);
        if (子筹办.状态 != 新任务治理条件筹办状态::已完成
            || !子筹办.候选已完整核验) {
            结果.状态 = 映射筹办状态(子筹办.状态, false);
            return 结果;
        }
        const auto 核验筹办项完整 = [](const 新任务治理条件筹办结果& 筹办) {
            bool 等待 = false;
            for (const auto& 方法项 : 筹办.方法筹办组) {
                if (方法项.状态 == 新任务候选方法筹办状态::资源失败) return -2;
                if (方法项.状态 == 新任务候选方法筹办状态::结构缺口) return -1;
                等待 = 等待
                    || 方法项.状态 == 新任务候选方法筹办状态::材料未到
                    || 方法项.状态 == 新任务候选方法筹办状态::参数缺失;
            }
            return 等待 ? 0 : 1;
        };
        const auto 父完整 = 核验筹办项完整(父筹办);
        const auto 子完整 = 核验筹办项完整(子筹办);
        if (父完整 < 0 || 子完整 < 0) {
            结果.状态 = 父完整 == -2 || 子完整 == -2
                ? 新需求方向一致性状态::资源失败
                : 新需求方向一致性状态::结构不一致;
            return 结果;
        }
        if (父完整 == 0 || 子完整 == 0) {
            结果.状态 = 父完整 == 0
                ? 新需求方向一致性状态::父方法材料未到
                : 新需求方向一致性状态::子方法材料未到;
            return 结果;
        }

        const auto 查绑定对象 = [](const 新任务候选方法筹办项& 筹办,
                                     稳定编码 角色)
            -> std::optional<稳定编码> {
            const auto 位置 = std::find_if(筹办.参数绑定组.begin(),
                筹办.参数绑定组.end(), [&](const auto& 绑定) {
                    return 绑定.参数角色节点 == 角色;
                });
            return 位置 != 筹办.参数绑定组.end()
                ? 位置->实际存在节点 : std::nullopt;
        };
        const auto 存在概念兼容 = [&](稳定编码 实际对象,
                                      稳定编码 要求概念) {
            std::optional<稳定编码> 实际概念;
            if (const auto 场景 = 场景服务_.获取场景(实际对象)) {
                实际概念 = 场景->专属存在概念节点;
            } else if (const auto 存在 = 存在服务_.获取存在(实际对象)) {
                实际概念 = 存在->专属存在概念节点;
            }
            if (!实际概念) return false;
            if (*实际概念 == 要求概念) return true;
            std::vector<稳定编码> 待查{*实际概念};
            std::set<稳定编码> 已查;
            while (!待查.empty()) {
                const auto 当前 = 待查.back();
                待查.pop_back();
                if (!已查.insert(当前).second) continue;
                for (const auto 上位 : 存在概念服务_.查询上位存在概念(当前)) {
                    if (上位 == 要求概念) return true;
                    待查.push_back(上位);
                }
            }
            return false;
        };
        const auto 结果项推动需求 = [&](const 新方法结果项定义& 项,
                                          const 新任务候选方法筹办项& 筹办,
                                          const 新需求信息& 需求,
                                          const 新需求当前满足结果& 当前) {
            const auto 对象 = 查绑定对象(筹办, 项.参数角色节点);
            if (!对象 || *对象 != 需求.目标存在节点
                || !项.特征类型概念节点) return 0;
            const auto 项类型根 = 取得特征概念类型根(
                特征概念服务_, *项.特征类型概念节点);
            const auto 需求类型根 = 取得特征概念类型根(
                特征概念服务_, 需求.目标.目标特征类型概念节点);
            if (!项类型根 || !需求类型根 || 项类型根 != 需求类型根) return 0;
            bool 值域结构有效 = true;
            const bool 值域保证 = 需求.目标.目标形式 == 新需求目标形式::值
                && 需求.目标.目标值域概念节点
                && (项.特征值域概念节点 || 项.精确数值域)
                && 任务值域包含(特征服务_, 特征概念服务_, *项类型根,
                    需求.目标.目标值域概念节点, {}, 项.特征值域概念节点,
                    项.精确数值域, 值域结构有效);
            if (!值域结构有效) return -2;
            auto 方向 = 项.变化方向概念节点
                ? 读取预计变化方向(
                    二次特征概念服务_, *项.变化方向概念节点)
                : 预计变化方向::不明确;
            if (!项.变化方向概念节点 && 当前.实际特征节点) {
                const auto 特征 = 特征服务_.获取特征(*当前.实际特征节点);
                if (特征) 方向 = 从任务值域推导方向(特征服务_, 特征概念服务_, *项类型根,
                    项.特征值域概念节点, 项.精确数值域, *特征);
            }
            std::optional<预计变化方向> 所需方向;
            if (需求.目标.目标形式 == 新需求目标形式::变化方向
                && 需求.目标.目标变化方向) {
                所需方向 = *需求.目标.目标变化方向
                        == 新需求变化方向::增加
                    ? 预计变化方向::增加 : 预计变化方向::减少;
            } else if (当前.状态 == 新需求当前满足状态::需增加) {
                所需方向 = 预计变化方向::增加;
            } else if (当前.状态 == 新需求当前满足状态::需减少) {
                所需方向 = 预计变化方向::减少;
            }
            const bool 方向相反 = 所需方向
                && ((方向 == 预计变化方向::增加
                    && *所需方向 == 预计变化方向::减少)
                || (方向 == 预计变化方向::减少
                    && *所需方向 == 预计变化方向::增加));
            if (值域保证 && 方向相反) return -2;
            if (值域保证) return 2;
            if (!所需方向) return 0;
            if (方向 == *所需方向) return 1;
            if (方向相反) return -1;
            return 0;
        };
        const auto 条件项对应子需求 = [&](const 新方法条件项定义& 项,
                                            const 新任务候选方法筹办项& 筹办) {
            const auto 对象 = 查绑定对象(筹办, 项.参数角色节点);
            if (!对象 || *对象 != 子需求->目标存在节点
                || !项.特征类型概念节点
                || (!项.特征值域概念节点 && !项.精确数值域)
                || 子需求->目标.目标形式 != 新需求目标形式::值
                || !子需求->目标.目标值域概念节点
                || 项.二次特征概念节点 || 项.状态或动态概念节点) return false;
            if (项.存在概念节点
                && !存在概念兼容(*对象, *项.存在概念节点)) return false;
            const auto 条件根 = 取得特征概念类型根(
                特征概念服务_, *项.特征类型概念节点);
            const auto 子根 = 取得特征概念类型根(
                特征概念服务_, 子需求->目标.目标特征类型概念节点);
            if (!条件根 || !子根 || 条件根 != 子根) return false;
            bool 结构有效 = true;
            return 任务值域包含(特征服务_, 特征概念服务_, *条件根,
                项.特征值域概念节点, 项.精确数值域,
                子需求->目标.目标值域概念节点, {}, 结构有效)
                && 结构有效;
        };

        struct 父匹配 final {
            稳定编码 方法;
            稳定编码 配对;
            稳定编码 条件;
            稳定编码 结果;
            int 程度;
        };
        std::vector<父匹配> 父匹配组;
        bool 父有相反结果 = false;
        bool 已找到推动父需求的结果 = false;
        for (const auto& 筹办 : 父筹办.方法筹办组) {
            const auto 组织=读取任务方法执行组织(*父任务节点);
            const auto 选择=选择任务方法结果(*父任务节点);
            const auto 方法 = 组织.组织 ? 任务执行组织存储::读取来源视图(*组织.组织,筹办.方法节点,方法服务_,
                选择.本能采用&&选择.本能采用->来源方法节点==筹办.方法节点 ? &*选择.本能采用 : nullptr) : std::nullopt;
            const auto 私有域检查 = 方法
                ? 检查方法私有域闭包(方法服务_, *方法)
                : 方法私有域闭包检查状态::方法不可读;
            if (私有域检查 == 方法私有域闭包检查状态::方法不可读
            || 私有域检查 == 方法私有域闭包检查状态::资源失败) {
                结果.状态 = 私有域检查 == 方法私有域闭包检查状态::资源失败
                    ? 新需求方向一致性状态::资源失败 : 新需求方向一致性状态::结构不一致;
                return 结果;
            }
            for (const auto& 配对 : 方法->条件结果组) {
                if (配对.结果.关系规则 != 新方法项关系规则::单项
                    && 配对.结果.关系规则 != 新方法项关系规则::全部成立
                    && 配对.结果.关系规则
                        != 新方法项关系规则::主要结果及允许副作用) {
                    结果.状态 = 新需求方向一致性状态::结构不一致;
                    return 结果;
                }
                int 最佳推动 = 0;
                bool 本配对有相反结果 = false;
                for (const auto& 项 : 配对.结果.结果项组) {
                    const auto 推动 = 结果项推动需求(项, 筹办, *父需求, 父当前);
                    if (推动 == -2) {
                        结果.状态 = 新需求方向一致性状态::结构不一致;
                        return 结果;
                    }
                    本配对有相反结果 = 本配对有相反结果 || 推动 == -1;
                    if ((配对.结果.关系规则
                            != 新方法项关系规则::主要结果及允许副作用
                        || 项.主要结果) && 推动 > 最佳推动) {
                        最佳推动 = 推动;
                    }
                }
                父有相反结果 = 父有相反结果 || 本配对有相反结果;
                if (最佳推动 <= 0 || 本配对有相反结果) continue;
                已找到推动父需求的结果 = true;
                const auto 配对核验 = std::find_if(
                    筹办.条件结果组.begin(), 筹办.条件结果组.end(),
                    [&](const auto& 核验) {
                        return 核验.配对节点 == 配对.配对节点
                            && 核验.条件节点 == 配对.条件节点
                            && 核验.结果节点 == 配对.结果节点;
                    });
                if (配对核验 == 筹办.条件结果组.end()) {
                    结果.状态 = 新需求方向一致性状态::结构不一致;
                    return 结果;
                }
                if (配对核验->状态 == 新任务方法条件项核验状态::资源失败) {
                    结果.状态 = 新需求方向一致性状态::资源失败;
                    return 结果;
                }
                if (配对核验->状态 == 新任务方法条件项核验状态::结构缺口) {
                    结果.状态 = 新需求方向一致性状态::结构不一致;
                    return 结果;
                }
                if (配对核验->状态 == 新任务方法条件项核验状态::材料未到) {
                    结果.状态 = 新需求方向一致性状态::父方法材料未到;
                    return 结果;
                }
                if (配对.条件.关系规则 != 新方法项关系规则::单项
                    && 配对.条件.关系规则 != 新方法项关系规则::全部成立
                    && 配对.条件.关系规则 != 新方法项关系规则::任一成立) {
                    结果.状态 = 新需求方向一致性状态::结构不一致;
                    return 结果;
                }
                for (const auto& 条件项 : 配对.条件.条件项组) {
                    if (条件项对应子需求(条件项, 筹办)) {
                        父匹配组.push_back({筹办.方法节点, 配对.配对节点,
                            配对.条件节点, 配对.结果节点, 最佳推动});
                    }
                }
            }
        }
        if (父匹配组.empty()) {
            结果.状态 = 已找到推动父需求的结果
                ? 新需求方向一致性状态::未找到对应条件
                : (父有相反结果
                    ? 新需求方向一致性状态::方向相反
                    : 新需求方向一致性状态::未找到父结果);
            return 结果;
        }
        std::sort(父匹配组.begin(), 父匹配组.end(), [](const auto& 左, const auto& 右) {
            return std::tie(左.方法, 左.配对, 左.条件, 左.结果, 左.程度)
                < std::tie(右.方法, 右.配对, 右.条件, 右.结果, 右.程度);
        });
        父匹配组.erase(std::unique(父匹配组.begin(), 父匹配组.end(),
            [](const auto& 左, const auto& 右) {
                return 左.方法 == 右.方法 && 左.配对 == 右.配对
                    && 左.条件 == 右.条件 && 左.结果 == 右.结果
                    && 左.程度 == 右.程度;
            }), 父匹配组.end());
        const auto 父最佳程度 = std::max_element(父匹配组.begin(),
            父匹配组.end(), [](const auto& 左, const auto& 右) {
                return 左.程度 < 右.程度;
            })->程度;
        父匹配组.erase(std::remove_if(父匹配组.begin(), 父匹配组.end(),
            [&](const auto& 匹配) { return 匹配.程度 < 父最佳程度; }),
            父匹配组.end());
        if (父匹配组.size() != 1) {
            结果.状态 = 新需求方向一致性状态::匹配不唯一;
            return 结果;
        }
        结果.父方法节点 = 父匹配组.front().方法;
        结果.父配对节点 = 父匹配组.front().配对;
        结果.父条件节点 = 父匹配组.front().条件;
        结果.父结果节点 = 父匹配组.front().结果;

        const auto 子当前 = 需求服务_.复核当前满足(子需求节点);
        if (子当前.状态 == 新需求当前满足状态::已满足) {
            结果.状态 = 新需求方向一致性状态::子需求已满足;
            return 结果;
        }
        if (子当前.状态 == 新需求当前满足状态::资源失败) {
            结果.状态 = 新需求方向一致性状态::资源失败;
            return 结果;
        }
        if (!当前结果可用于方向判断(子当前.状态)) {
            结果.状态 = 子当前.状态 == 新需求当前满足状态::目标特征尚未形成
                    || 子当前.状态 == 新需求当前满足状态::当前值不存在
                ? 新需求方向一致性状态::子方法材料未到
                : 新需求方向一致性状态::结构不一致;
            return 结果;
        }
        struct 子匹配 final { 稳定编码 方法; 稳定编码 结果; int 程度; };
        std::vector<子匹配> 子匹配组;
        bool 子有相反结果 = false;
        for (const auto& 筹办 : 子筹办.方法筹办组) {
            const auto 组织=读取任务方法执行组织(*子任务节点);
            const auto 选择=选择任务方法结果(*子任务节点);
            const auto 方法 = 组织.组织 ? 任务执行组织存储::读取来源视图(*组织.组织,筹办.方法节点,方法服务_,
                选择.本能采用&&选择.本能采用->来源方法节点==筹办.方法节点 ? &*选择.本能采用 : nullptr) : std::nullopt;
            const auto 私有域检查 = 方法
                ? 检查方法私有域闭包(方法服务_, *方法)
                : 方法私有域闭包检查状态::方法不可读;
            if (私有域检查 == 方法私有域闭包检查状态::方法不可读
            || 私有域检查 == 方法私有域闭包检查状态::资源失败) {
                结果.状态 = 私有域检查 == 方法私有域闭包检查状态::资源失败
                    ? 新需求方向一致性状态::资源失败 : 新需求方向一致性状态::结构不一致;
                return 结果;
            }
            for (const auto& 配对 : 方法->条件结果组) {
                if (配对.结果.关系规则 != 新方法项关系规则::单项
                    && 配对.结果.关系规则 != 新方法项关系规则::全部成立
                    && 配对.结果.关系规则
                        != 新方法项关系规则::主要结果及允许副作用) {
                    结果.状态 = 新需求方向一致性状态::结构不一致;
                    return 结果;
                }
                int 最佳 = 0;
                bool 本配对有相反结果 = false;
                for (const auto& 项 : 配对.结果.结果项组) {
                    const auto 推动 = 结果项推动需求(项, 筹办, *子需求, 子当前);
                    if (推动 == -2) {
                        结果.状态 = 新需求方向一致性状态::结构不一致;
                        return 结果;
                    }
                    本配对有相反结果 = 本配对有相反结果 || 推动 == -1;
                    if ((配对.结果.关系规则
                            != 新方法项关系规则::主要结果及允许副作用
                        || 项.主要结果) && 推动 > 最佳) {
                        最佳 = 推动;
                    }
                }
                子有相反结果 = 子有相反结果 || 本配对有相反结果;
                if (最佳 <= 0 || 本配对有相反结果) continue;
                const auto 配对核验 = std::find_if(
                    筹办.条件结果组.begin(), 筹办.条件结果组.end(),
                    [&](const auto& 核验) {
                        return 核验.配对节点 == 配对.配对节点
                            && 核验.条件节点 == 配对.条件节点
                            && 核验.结果节点 == 配对.结果节点;
                    });
                if (配对核验 == 筹办.条件结果组.end()) {
                    结果.状态 = 新需求方向一致性状态::结构不一致;
                    return 结果;
                }
                if (配对核验->状态 == 新任务方法条件项核验状态::资源失败) {
                    结果.状态 = 新需求方向一致性状态::资源失败;
                    return 结果;
                }
                if (配对核验->状态 == 新任务方法条件项核验状态::结构缺口) {
                    结果.状态 = 新需求方向一致性状态::结构不一致;
                    return 结果;
                }
                if (配对核验->状态 == 新任务方法条件项核验状态::材料未到) {
                    结果.状态 = 新需求方向一致性状态::子方法材料未到;
                    return 结果;
                }
                子匹配组.push_back({
                    筹办.方法节点, 配对.结果节点, 最佳});
            }
        }
        if (子匹配组.empty()) {
            结果.状态 = 子有相反结果
                ? 新需求方向一致性状态::方向相反
                : 新需求方向一致性状态::子结果不满足条件;
            return 结果;
        }
        std::sort(子匹配组.begin(), 子匹配组.end(), [](const auto& 左, const auto& 右) {
            return std::tie(左.方法, 左.结果, 左.程度)
                < std::tie(右.方法, 右.结果, 右.程度);
        });
        子匹配组.erase(std::unique(子匹配组.begin(), 子匹配组.end(),
            [](const auto& 左, const auto& 右) {
                return 左.方法 == 右.方法 && 左.结果 == 右.结果
                    && 左.程度 == 右.程度;
            }), 子匹配组.end());
        const auto 子最佳程度 = std::max_element(子匹配组.begin(),
            子匹配组.end(), [](const auto& 左, const auto& 右) {
                return 左.程度 < 右.程度;
            })->程度;
        子匹配组.erase(std::remove_if(子匹配组.begin(), 子匹配组.end(),
            [&](const auto& 匹配) { return 匹配.程度 < 子最佳程度; }),
            子匹配组.end());
        if (子匹配组.size() != 1) {
            结果.状态 = 新需求方向一致性状态::匹配不唯一;
            return 结果;
        }
        结果.子方法节点 = 子匹配组.front().方法;
        结果.子结果节点 = 子匹配组.front().结果;
        结果.状态 = 父匹配组.front().程度 == 2
                && 子匹配组.front().程度 == 2
            ? 新需求方向一致性状态::一致
            : 新需求方向一致性状态::方向推进但不保证满足;
        return 结果;
    } catch (const std::bad_alloc&) {
        结果.状态 = 新需求方向一致性状态::资源失败;
        return 结果;
    } catch (...) {
        结果.状态 = 新需求方向一致性状态::结构不一致;
        return 结果;
    }
}

新需求根链方向一致性结果 新_任务类::判断需求根链方向一致(
    稳定编码 叶需求节点) const noexcept {
    新需求根链方向一致性结果 结果;
    try {
        std::lock_guard 锁(新任务互斥);
        std::set<稳定编码> 已见;
        auto 当前节点 = 叶需求节点;
        while (true) {
            if (!已见.insert(当前节点).second) {
                结果.状态 = 新需求方向一致性状态::形成环;
                return 结果;
            }
            const auto 当前 = 需求服务_.获取需求(当前节点);
            if (!当前) {
                结果.状态 = 新需求方向一致性状态::链断裂;
                return 结果;
            }
            结果.根到叶需求节点组.push_back(当前节点);
            if (!当前->父需求节点) {
                if (!当前->根角色
                    || 需求服务_.查询根需求(*当前->根角色) != 当前节点) {
                    结果.状态 = 新需求方向一致性状态::链断裂;
                    return 结果;
                }
                break;
            }
            当前节点 = *当前->父需求节点;
        }
        std::reverse(结果.根到叶需求节点组.begin(),
            结果.根到叶需求节点组.end());
        if (结果.根到叶需求节点组.size() == 1) {
            结果.状态 = 新需求方向一致性状态::一致;
            return 结果;
        }
        bool 有弱推进 = false;
        for (std::size_t i = 1; i < 结果.根到叶需求节点组.size(); ++i) {
            const auto 局部 = 判断父子需求方向一致(
                结果.根到叶需求节点组[i - 1],
                结果.根到叶需求节点组[i]);
            if (局部.状态 != 新需求方向一致性状态::一致
                && 局部.状态
                    != 新需求方向一致性状态::方向推进但不保证满足) {
                结果.状态 = 局部.状态;
                结果.失败父需求节点 = 局部.父需求节点;
                结果.失败子需求节点 = 局部.子需求节点;
                结果.局部结果 = 局部;
                return 结果;
            }
            有弱推进 = 有弱推进
                || 局部.状态
                    == 新需求方向一致性状态::方向推进但不保证满足;
        }
        结果.状态 = 有弱推进
            ? 新需求方向一致性状态::方向推进但不保证满足
            : 新需求方向一致性状态::一致;
        return 结果;
    } catch (const std::bad_alloc&) {
        结果.状态 = 新需求方向一致性状态::资源失败;
        return 结果;
    } catch (...) {
        结果.状态 = 新需求方向一致性状态::结构不一致;
        return 结果;
    }
}

新冲突需求查找结果 新_任务类::查找冲突需求(
    稳定编码 任务节点,
    const std::vector<新任务预计变化阶段>& 预计变化阶段组) const noexcept {
    新冲突需求查找结果 结果;
    结果.任务节点 = 任务节点;
    try {
        std::lock_guard 锁(新任务互斥);
        const auto 任务 = 获取任务(任务节点);
        if (!任务) {
            结果.状态 = 新冲突需求查找状态::任务不存在;
            return 结果;
        }
        if (!预计变化定义有效(预计变化阶段组)) {
            结果.状态 = 新冲突需求查找状态::结构不一致;
            return 结果;
        }
        const auto 全部需求 = 需求服务_.查询全部需求();
        if (全部需求.状态 == 新需求操作状态::资源失败) {
            结果.状态 = 新冲突需求查找状态::资源失败;
            return 结果;
        }
        if (全部需求.状态 != 新需求操作状态::已找到) {
            结果.状态 = 新冲突需求查找状态::结构不一致;
            return 结果;
        }

        for (const auto 需求节点 : 全部需求.需求节点组) {
            if (需求节点 == 任务->需求节点) continue;
            const auto 需求 = 需求服务_.获取需求(需求节点);
            if (!需求) {
                结果.状态 = 新冲突需求查找状态::结构不一致;
                return 结果;
            }
            if (需求->活动状态 == 新需求活动状态::已满足
                || 需求->活动状态 == 新需求活动状态::失活) {
                continue;
            }
            if (需求->活动状态 != 新需求活动状态::活动
                && 需求->活动状态 != 新需求活动状态::等待) {
                结果.状态 = 新冲突需求查找状态::结构不一致;
                return 结果;
            }
            const auto 需求类型根 = 取得特征概念类型根(
                特征概念服务_, 需求->目标.目标特征类型概念节点);
            if (!需求类型根) {
                结果.状态 = 新冲突需求查找状态::结构不一致;
                return 结果;
            }

            bool 有相关预计变化 = false;
            for (const auto& 阶段 : 预计变化阶段组) {
                for (const auto& 预计 : 阶段.变化组) {
                    if (预计.作用对象节点 != 需求->目标存在节点) continue;
                    const auto 预计类型根 = 取得特征概念类型根(
                        特征概念服务_, 预计.特征类型概念节点);
                    if (!预计类型根) {
                        结果.状态 = 新冲突需求查找状态::结构不一致;
                        return 结果;
                    }
                    有相关预计变化 = 有相关预计变化
                        || *预计类型根 == *需求类型根;
                }
            }
            if (!有相关预计变化) continue;

            std::optional<新需求变化方向> 需求方向;
            if (需求->目标.目标形式 == 新需求目标形式::变化方向) {
                if (!需求->目标.目标变化方向
                    || 需求->目标.目标值域概念节点) {
                    结果.状态 = 新冲突需求查找状态::结构不一致;
                    return 结果;
                }
                需求方向 = 需求->目标.目标变化方向;
            } else if (需求->目标.目标形式 == 新需求目标形式::值) {
                if (!需求->目标.目标值域概念节点
                    || 需求->目标.目标变化方向) {
                    结果.状态 = 新冲突需求查找状态::结构不一致;
                    return 结果;
                }
                const auto 当前 = 需求服务_.复核当前满足(需求节点);
                if (当前.状态 == 新需求当前满足状态::资源失败) {
                    结果.状态 = 新冲突需求查找状态::资源失败;
                    return 结果;
                }
                if (当前.状态 == 新需求当前满足状态::已满足) continue;
                if (当前.状态 == 新需求当前满足状态::需增加) {
                    需求方向 = 新需求变化方向::增加;
                } else if (当前.状态 == 新需求当前满足状态::需减少) {
                    需求方向 = 新需求变化方向::减少;
                } else if (当前.状态 == 新需求当前满足状态::未满足但无有向差值) {
                    // 集合等非有向目标使用下方值域保证关系，不虚构数值方向。
                } else if (当前.状态
                        == 新需求当前满足状态::目标特征尚未形成
                    || 当前.状态 == 新需求当前满足状态::当前值不存在) {
                    结果.等待材料需求节点组.push_back(需求节点);
                    continue;
                } else {
                    结果.状态 = 新冲突需求查找状态::结构不一致;
                    return 结果;
                }
            } else {
                结果.状态 = 新冲突需求查找状态::结构不一致;
                return 结果;
            }

            for (const auto& 阶段 : 预计变化阶段组) {
                for (const auto& 预计 : 阶段.变化组) {
                    if (预计.作用对象节点 != 需求->目标存在节点) continue;
                    const auto 预计类型根 = 取得特征概念类型根(
                        特征概念服务_, 预计.特征类型概念节点);
                    if (!预计类型根) {
                        结果.状态 = 新冲突需求查找状态::结构不一致;
                        return 结果;
                    }
                    if (*预计类型根 != *需求类型根) continue;

                    bool 值域结构有效 = true;
                    const bool 值域保证 =
                        需求->目标.目标形式 == 新需求目标形式::值
                        && (预计.预期值域概念节点 || 预计.预期精确数值域)
                        && 任务值域包含(
                            特征服务_, 特征概念服务_, *预计类型根,
                            需求->目标.目标值域概念节点, {},
                            预计.预期值域概念节点, 预计.预期精确数值域,
                            值域结构有效);
                    if (!值域结构有效) {
                        结果.状态 = 新冲突需求查找状态::结构不一致;
                        return 结果;
                    }
                    auto 预期方向 = 预计.预期方向;
                    if (!预期方向) {
                        const auto 当前 = 需求服务_.复核当前满足(需求节点);
                        if (当前.状态 == 新需求当前满足状态::资源失败) {
                            结果.状态 = 新冲突需求查找状态::资源失败; return 结果;
                        }
                        const auto 特征 = 当前.实际特征节点 ? 特征服务_.获取特征(*当前.实际特征节点) : std::nullopt;
                        if (特征) {
                            const auto 推导 = 从任务值域推导方向(特征服务_, 特征概念服务_,
                                *预计类型根, 预计.预期值域概念节点, 预计.预期精确数值域, *特征);
                            if (推导 == 预计变化方向::增加) 预期方向 = 新任务预期变化方向::增加;
                            else if (推导 == 预计变化方向::减少) 预期方向 = 新任务预期变化方向::减少;
                            else if (推导 == 预计变化方向::不变) 预期方向 = 新任务预期变化方向::不变;
                        }
                    }
                    const bool 方向相反 = 需求方向 && 预期方向
                        && ((*需求方向 == 新需求变化方向::增加
                                && *预期方向
                                    == 新任务预期变化方向::减少)
                            || (*需求方向 == 新需求变化方向::减少
                                && *预期方向
                                    == 新任务预期变化方向::增加));
                    if (值域保证) {
                        if (方向相反) {
                            结果.状态 = 新冲突需求查找状态::结构不一致;
                            return 结果;
                        }
                        continue;
                    }
                    if (!需求方向 || !预期方向) {
                        结果.等待材料需求节点组.push_back(需求节点);
                        continue;
                    }
                    if (方向相反) {
                        结果.冲突需求组.push_back({
                            需求节点,
                            需求->目标存在节点,
                            *需求类型根,
                            阶段.顺序,
                            *需求方向,
                            *预期方向,
                            预计.预期值域概念节点,
                            预计.必须出现, 预计.预期精确数值域});
                    }
                }
            }
        }

        std::sort(结果.冲突需求组.begin(), 结果.冲突需求组.end(),
            [](const auto& 左, const auto& 右) {
                return std::tie(左.需求节点.值, 左.阶段顺序,
                           左.作用对象节点.值, 左.特征类型根概念节点.值,
                           左.预计方向, 左.必须出现)
                    < std::tie(右.需求节点.值, 右.阶段顺序,
                           右.作用对象节点.值, 右.特征类型根概念节点.值,
                           右.预计方向, 右.必须出现);
            });
        结果.冲突需求组.erase(std::unique(
            结果.冲突需求组.begin(), 结果.冲突需求组.end()),
            结果.冲突需求组.end());
        std::sort(结果.等待材料需求节点组.begin(),
            结果.等待材料需求节点组.end());
        结果.等待材料需求节点组.erase(std::unique(
            结果.等待材料需求节点组.begin(),
            结果.等待材料需求节点组.end()),
            结果.等待材料需求节点组.end());
        结果.状态 = !结果.冲突需求组.empty()
            ? 新冲突需求查找状态::发现冲突
            : !结果.等待材料需求节点组.empty()
                ? 新冲突需求查找状态::等待材料
                : 新冲突需求查找状态::无冲突;
        return 结果;
    } catch (const std::bad_alloc&) {
        结果.状态 = 新冲突需求查找状态::资源失败;
        结果.冲突需求组.clear();
        结果.等待材料需求节点组.clear();
        return 结果;
    } catch (...) {
        结果.状态 = 新冲突需求查找状态::结构不一致;
        结果.冲突需求组.clear();
        结果.等待材料需求节点组.clear();
        return 结果;
    }
}

新任务治理执行方向核验结果 新_任务类::核验任务执行方向(
    稳定编码 任务节点,
    const 新任务本能采用材料& 本能采用,
    稳定编码 自我存在节点,
    稳定编码 安全需求节点,
    稳定编码 服务需求节点) const noexcept {
    const auto 方法节点=本能采用.来源方法节点,配对节点=本能采用.来源配对节点,结果节点=本能采用.来源结果节点;
    新任务治理执行方向核验结果 结果;
    结果.本能采用=本能采用;
    结果.任务节点 = 任务节点;
    结果.方法节点 = 方法节点;
    结果.配对节点 = 配对节点;
    结果.结果节点 = 结果节点;
    try {
        std::lock_guard 锁(新任务互斥);
        if (!治理已初始化_) {
            结果.状态 = 新任务治理执行方向核验状态::等待材料;
            return 结果;
        }
        const auto 任务 = 获取任务(任务节点);
        const auto 组织=读取任务方法执行组织(任务节点);
        const auto 方法 = 组织.组织 ? 任务执行组织存储::读取来源视图(*组织.组织,方法节点,方法服务_,&本能采用) : std::nullopt;
        if (!任务 || !方法 || 方法->启用状态 != 新方法启用状态::启用) {
            结果.状态 = 新任务治理执行方向核验状态::定位已变化;
            return 结果;
        }
        const auto 来源守卫 = 需求服务_.复核需求服务要求语义(任务->需求节点);
        if (来源守卫.状态 != 新服务要求执行守卫状态::不涉及
            && 来源守卫.状态 != 新服务要求执行守卫状态::当前有效) {
            结果.状态 = 来源守卫.状态 == 新服务要求执行守卫状态::需重新确认
                ? 新任务治理执行方向核验状态::等待材料
                : 来源守卫.状态 == 新服务要求执行守卫状态::资源失败
                    ? 新任务治理执行方向核验状态::资源失败 : 新任务治理执行方向核验状态::结构不一致;
            return 结果;
        }
        if (任务->当前状态 != 新任务状态::待检查条件) {
            结果.状态 = 新任务治理执行方向核验状态::任务状态已变化;
            return 结果;
        }
        if (方法->实现方式 != 新方法实现方式::本能) {
            结果.状态 = 新任务治理执行方向核验状态::结果定义暂不支持;
            return 结果;
        }

        const auto 当前筹办 = 核验任务条件和参数内部(任务节点,&本能采用,false);
        if (当前筹办.状态 == 新任务治理条件筹办状态::任务状态已变化) {
            结果.状态 = 新任务治理执行方向核验状态::任务状态已变化;
            return 结果;
        }
        if (当前筹办.状态 == 新任务治理条件筹办状态::资源失败) {
            结果.状态 = 新任务治理执行方向核验状态::资源失败;
            return 结果;
        }
        if (当前筹办.状态 != 新任务治理条件筹办状态::已完成
            || !当前筹办.候选已完整核验) {
            结果.状态 = 新任务治理执行方向核验状态::等待材料;
            return 结果;
        }

        const 新任务候选方法筹办项* 唯一方法 = nullptr;
        for (const auto& 方法项 : 当前筹办.方法筹办组) {
            if (方法项.方法节点 == 方法节点) 唯一方法 = &方法项;
        }
        if (!唯一方法 || 唯一方法->参数绑定组 != 本能采用.参数绑定组
            ||唯一方法->状态!=新任务候选方法筹办状态::条件具备) {
            结果.状态 = 新任务治理执行方向核验状态::定位已变化;
            return 结果;
        }
        const auto 配对位置 = std::find_if(
            方法->条件结果组.begin(), 方法->条件结果组.end(),
            [&](const 新方法条件结果信息& 候选) {
                return 候选.配对节点 == 配对节点
                    && 候选.结果节点 == 结果节点;
            });
        if (配对位置 == 方法->条件结果组.end()) {
            结果.状态 = 新任务治理执行方向核验状态::定位已变化;
            return 结果;
        }
        if (配对位置->结果.关系规则 != 新方法项关系规则::单项
            && 配对位置->结果.关系规则 != 新方法项关系规则::全部成立
            && 配对位置->结果.关系规则
                != 新方法项关系规则::主要结果及允许副作用) {
            结果.状态 = 新任务治理执行方向核验状态::结果定义暂不支持;
            return 结果;
        }

        struct 已绑定预计变化 final {
            新任务预计特征变化 变化;
            稳定编码 特征类型根;
            预计变化方向 方向 = 预计变化方向::不明确;
        };
        std::vector<已绑定预计变化> 已绑定组;
        新任务预计变化阶段 阶段;
        阶段.顺序 = 0;
        for (const auto& 项 : 配对位置->结果.结果项组) {
            if (!项.特征类型概念节点
                || 项.状态或动态概念节点
                || (!项.特征值域概念节点 && !项.精确数值域 && !项.变化方向概念节点)) {
                结果.状态 = 新任务治理执行方向核验状态::结果定义暂不支持;
                return 结果;
            }
            const auto 绑定位置 = std::find_if(
                唯一方法->参数绑定组.begin(), 唯一方法->参数绑定组.end(),
                [&](const 新方法参数角色绑定& 绑定) {
                    return 绑定.参数角色节点 == 项.参数角色节点;
                });
            if (绑定位置 == 唯一方法->参数绑定组.end()
                || !绑定位置->实际存在节点) {
                结果.状态 = 新任务治理执行方向核验状态::等待材料;
                return 结果;
            }
            const auto 类型根 = 取得特征概念类型根(
                特征概念服务_, *项.特征类型概念节点);
            if (!类型根) {
                结果.状态 = 新任务治理执行方向核验状态::结构不一致;
                return 结果;
            }
            if (项.变化方向概念节点) {
                const auto 方向概念 = 二次特征概念服务_.获取二次特征概念(
                    *项.变化方向概念节点);
                if (!方向概念
                    || 方向概念->算法 != 二次特征算法::有向特征差值
                    || 方向概念->参照特征概念节点
                    || 方向概念->输入定义组.empty()) {
                    结果.状态 = 新任务治理执行方向核验状态::结构不一致;
                    return 结果;
                }
                for (const auto& 输入 : 方向概念->输入定义组) {
                    if (输入.来源种类
                            != 二次特征输入来源种类::特征准确值
                        || !输入.特征类型概念节点
                        || 取得特征概念类型根(
                                特征概念服务_, *输入.特征类型概念节点)
                            != 类型根) {
                        结果.状态 = 新任务治理执行方向核验状态::结构不一致;
                        return 结果;
                    }
                }
            }
            新任务预计特征变化 变化;
            变化.作用对象节点 = *绑定位置->实际存在节点;
            变化.特征类型概念节点 = *项.特征类型概念节点;
            变化.预期变化关系节点 = 项.变化方向概念节点;
            const auto 方向 = 项.变化方向概念节点
                ? 读取预计变化方向(
                    二次特征概念服务_, *项.变化方向概念节点)
                : 预计变化方向::不明确;
            if (方向 == 预计变化方向::增加) {
                变化.预期方向 = 新任务预期变化方向::增加;
            } else if (方向 == 预计变化方向::减少) {
                变化.预期方向 = 新任务预期变化方向::减少;
            } else if (方向 == 预计变化方向::不变) {
                变化.预期方向 = 新任务预期变化方向::不变;
            }
            变化.预期值域概念节点 = 项.特征值域概念节点;
            变化.预期精确数值域 = 项.精确数值域;
            变化.必须出现 = 配对位置->结果.关系规则
                    != 新方法项关系规则::主要结果及允许副作用
                || 项.主要结果;
            阶段.变化组.push_back(变化);
            已绑定组.push_back({变化, *类型根, 方向});
        }
        if (阶段.变化组.empty()) {
            结果.状态 = 新任务治理执行方向核验状态::结果定义暂不支持;
            return 结果;
        }
        结果.预计变化阶段组.push_back(std::move(阶段));

        新任务结果获取方案 获取方案;
        const auto& 确认要求 = 配对位置->结果.确认要求;
        // 当前生产链只接通了状态owner正式通知这一条直接事实渠道。方法若
        // 要求共同/备用多渠道、固定等待上限或具名等待条件，不能伪装可执行。
        if (确认要求.多证据关系 != 新方法多证据关系::任一满足
            || 确认要求.最大等待纳秒
            || 确认要求.等待条件概念节点) {
            结果.状态 = 新任务治理执行方向核验状态::结果定义暂不支持;
            return 结果;
        }
        if (!确认要求.禁止结果项组.empty()
            && 确认要求.禁止结果关系规则 != 新方法项关系规则::单项
            && 确认要求.禁止结果关系规则 != 新方法项关系规则::任一成立
            && 确认要求.禁止结果关系规则 != 新方法项关系规则::全部成立) {
            结果.状态 = 新任务治理执行方向核验状态::结果定义暂不支持;
            return 结果;
        }
        获取方案.多渠道关系 = 确认要求.多证据关系;
        获取方案.渠道方案组.push_back({
            新任务结果获取渠道::世界树状态通知,
            0,
            新任务必要证据质量::正式直接事实,
            std::nullopt,
            std::nullopt});
        获取方案.禁止结果关系规则 = 确认要求.禁止结果关系规则;
        for (auto& 预计 : 已绑定组) {
            std::vector<稳定编码> 特征节点组;
            if (const auto 存在 = 存在服务_.获取存在(
                    预计.变化.作用对象节点)) {
                特征节点组 = 存在->特征节点组;
            } else if (const auto 场景 = 场景服务_.获取场景(
                    预计.变化.作用对象节点)) {
                特征节点组 = 场景->特征节点组;
            } else {
                结果.状态 = 新任务治理执行方向核验状态::结果定义暂不支持;
                return 结果;
            }
            std::optional<稳定编码> 实际特征;
            for (const auto 特征节点 : 特征节点组) {
                const auto 特征 = 特征服务_.获取特征(特征节点);
                if (!特征) {
                    结果.状态 = 新任务治理执行方向核验状态::结构不一致;
                    return 结果;
                }
                const auto 类型根 = 取得特征概念类型根(
                    特征概念服务_, 特征->特征概念节点);
                if (!类型根) {
                    结果.状态 = 新任务治理执行方向核验状态::结构不一致;
                    return 结果;
                }
                if (*类型根 != 预计.特征类型根) continue;
                if (实际特征) {
                    结果.状态 = 新任务治理执行方向核验状态::结构不一致;
                    return 结果;
                }
                实际特征 = 特征节点;
            }
            if (!实际特征) {
                // 没有可由世界树状态通知读回的实际特征，当前执行没有
                // 明确可实现的结果获取途径，必须保持零动作。
                结果.状态 = 新任务治理执行方向核验状态::结果定义暂不支持;
                return 结果;
            }
            const auto 特征 = 特征服务_.获取特征(*实际特征);
            if (!特征 || !特征->当前值) {
                结果.状态 = 新任务治理执行方向核验状态::等待材料;
                return 结果;
            }
            if (预计.变化.预期值域概念节点 || 预计.变化.预期精确数值域) {
                const auto 域 = 取得任务值域输入(特征概念服务_, 预计.特征类型根,
                    预计.变化.预期值域概念节点, 预计.变化.预期精确数值域);
                if (!域 || 域->单位 != 特征->实例单位) {
                    结果.状态 = 新任务治理执行方向核验状态::结构不一致; return 结果;
                }
                const auto 自比 = 特征服务_.比较特征值域(*域, *域);
                if (自比.状态 != 新特征比较状态::已完成) {
                    结果.状态 = 自比.状态 == 新特征比较状态::资源失败
                        ? 新任务治理执行方向核验状态::资源失败 : 新任务治理执行方向核验状态::结构不一致;
                    return 结果;
                }
            }
            if (!预计.变化.预期变化关系节点) {
                预计.方向 = 从任务值域推导方向(特征服务_, 特征概念服务_, 预计.特征类型根,
                    预计.变化.预期值域概念节点, 预计.变化.预期精确数值域, *特征);
                if (预计.方向 == 预计变化方向::增加) 预计.变化.预期方向 = 新任务预期变化方向::增加;
                else if (预计.方向 == 预计变化方向::减少) 预计.变化.预期方向 = 新任务预期变化方向::减少;
                else if (预计.方向 == 预计变化方向::不变) 预计.变化.预期方向 = 新任务预期变化方向::不变;
            }
            获取方案.目标组.push_back({预计.变化.作用对象节点,
                *实际特征, 预计.特征类型根, *特征->当前值,
                预计.变化.必须出现});
        }
        结果.预计变化阶段组.front().变化组.clear();
        for (const auto& 预计 : 已绑定组) 结果.预计变化阶段组.front().变化组.push_back(预计.变化);
        for (const auto& 禁止项 : 确认要求.禁止结果项组) {
            if (!禁止项.特征类型概念节点
                || 禁止项.状态或动态概念节点
                || (!禁止项.特征值域概念节点
                    && !禁止项.精确数值域
                    && !禁止项.变化方向概念节点)) {
                结果.状态 = 新任务治理执行方向核验状态::结果定义暂不支持;
                return 结果;
            }
            const auto 绑定位置 = std::find_if(
                唯一方法->参数绑定组.begin(), 唯一方法->参数绑定组.end(),
                [&](const 新方法参数角色绑定& 绑定) {
                    return 绑定.参数角色节点 == 禁止项.参数角色节点;
                });
            if (绑定位置 == 唯一方法->参数绑定组.end()
                || !绑定位置->实际存在节点) {
                结果.状态 = 新任务治理执行方向核验状态::等待材料;
                return 结果;
            }
            const auto 类型根 = 取得特征概念类型根(
                特征概念服务_, *禁止项.特征类型概念节点);
            if (!类型根) {
                结果.状态 = 新任务治理执行方向核验状态::结构不一致;
                return 结果;
            }
            std::optional<新任务预期变化方向> 禁止方向;
            if (禁止项.变化方向概念节点) {
                const auto 方向 = 读取预计变化方向(
                    二次特征概念服务_, *禁止项.变化方向概念节点);
                if (方向 == 预计变化方向::增加) {
                    禁止方向 = 新任务预期变化方向::增加;
                } else if (方向 == 预计变化方向::减少) {
                    禁止方向 = 新任务预期变化方向::减少;
                } else if (方向 == 预计变化方向::不变) {
                    禁止方向 = 新任务预期变化方向::不变;
                } else {
                    结果.状态 =
                        新任务治理执行方向核验状态::结果定义暂不支持;
                    return 结果;
                }
            }

            const auto 目标位置 = std::find_if(
                获取方案.目标组.begin(), 获取方案.目标组.end(),
                [&](const 新任务结果获取目标& 目标) {
                    return 目标.作用对象节点 == *绑定位置->实际存在节点
                        && 目标.特征类型根概念节点 == *类型根;
                });
            if (目标位置 == 获取方案.目标组.end()) {
                // 单一“状态通知”渠道只能在同一必需结果状态上同时核验禁止
                // 条件；其它特征没有变化通知时无法证明禁止结果未发生。
                结果.状态 = 新任务治理执行方向核验状态::结果定义暂不支持;
                return 结果;
            }
            目标位置->必须出现 = true;
            获取方案.禁止结果组.push_back({
                目标位置->作用对象节点,
                目标位置->实际特征节点,
                目标位置->特征类型根概念节点,
                目标位置->动作前准确值,
                禁止方向,
                禁止项.特征值域概念节点,
                std::nullopt, 禁止项.精确数值域});
        }
        if (!获取方案.完整()) {
            结果.状态 = 新任务治理执行方向核验状态::结果定义暂不支持;
            return 结果;
        }
        结果.结果获取方案 = std::move(获取方案);

        const auto 需求 = 需求服务_.获取需求(任务->需求节点);
        const auto 安全需求 = 需求服务_.获取需求(安全需求节点);
        const auto 服务需求 = 需求服务_.获取需求(服务需求节点);
        const auto 自我 = 存在服务_.获取存在(自我存在节点);
        if (!需求 || !安全需求 || !服务需求 || !自我
            || 安全需求->根角色 != 新需求根角色::安全
            || 服务需求->根角色 != 新需求根角色::服务
            || 安全需求->目标存在节点 != 自我存在节点
            || 服务需求->目标存在节点 != 自我存在节点
            || 安全需求->目标.目标形式 != 新需求目标形式::值
            || 服务需求->目标.目标形式 != 新需求目标形式::变化方向
            || 服务需求->目标.目标变化方向 != 新需求变化方向::增加
            || 服务需求->目标.目标值域概念节点
            || 服务需求->目标.目标比较关系概念节点) {
            结果.状态 = 新任务治理执行方向核验状态::结构不一致;
            return 结果;
        }
        结果.任务目标当前结果 = 需求服务_.复核当前满足(任务->需求节点);
        结果.安全当前结果 = 需求服务_.复核当前满足(安全需求节点);
        结果.服务当前结果 = 需求服务_.复核当前满足(服务需求节点);
        const auto 当前结果可用 = [](const 新需求当前满足结果& 当前) {
            return 当前.状态 == 新需求当前满足状态::已满足
                || 当前.状态 == 新需求当前满足状态::需增加
                || 当前.状态 == 新需求当前满足状态::需减少
                || 当前.状态
                    == 新需求当前满足状态::需要执行前后比较;
        };
        const auto 当前结果等待 = [](const 新需求当前满足结果& 当前) {
            return 当前.状态 == 新需求当前满足状态::目标特征尚未形成
                || 当前.状态 == 新需求当前满足状态::当前值不存在;
        };
        if (结果.任务目标当前结果->状态 == 新需求当前满足状态::资源失败
            || 结果.安全当前结果->状态 == 新需求当前满足状态::资源失败
            || 结果.服务当前结果->状态 == 新需求当前满足状态::资源失败) {
            结果.状态 = 新任务治理执行方向核验状态::资源失败;
            return 结果;
        }
        if (当前结果等待(*结果.任务目标当前结果)
            || 当前结果等待(*结果.安全当前结果)
            || 当前结果等待(*结果.服务当前结果)) {
            结果.状态 = 新任务治理执行方向核验状态::等待材料;
            return 结果;
        }
        // 只复核本次冻结采用的目标结果；不重新枚举或排序候选。
        bool 采用保证满足目标 = false;
        bool 采用方向推进 = false;
        for (const auto& 项 : 本能采用.结果.结果项组) {
            if (!项.特征类型概念节点
                || (本能采用.结果.关系规则 == 新方法项关系规则::主要结果及允许副作用 && !项.主要结果)
                || 取得特征概念类型根(特征概念服务_, *项.特征类型概念节点)
                    != std::optional<稳定编码>{需求->目标.目标特征类型概念节点}) continue;
            const auto 绑定 = std::find_if(本能采用.参数绑定组.begin(), 本能采用.参数绑定组.end(),
                [&](const auto& 值) { return 值.参数角色节点 == 项.参数角色节点; });
            if (绑定 == 本能采用.参数绑定组.end() || 绑定->实际存在节点 != 需求->目标存在节点) continue;
            const auto 输入 = 取得任务值域输入(特征概念服务_, *项.特征类型概念节点,
                项.特征值域概念节点, 项.精确数值域);
            if (!输入) continue;
            const auto 类型 = 特征概念服务_.获取特征概念(*项.特征类型概念节点);
            if (!类型) throw std::logic_error("冻结采用结果类型读取失败");
            if (类型->材料类型 == 特征概念材料物理类型::I64标量
                && 是条件包含目标(需求->目标, 特征概念服务_, 二次特征概念服务_)
                && 需求->目标.目标值域概念节点) {
                const auto 比较 = 特征服务_.比较特征值域(*输入,
                    {需求->目标.目标特征类型概念节点, 输入->单位,
                        新特征概念值域比较材料{*需求->目标.目标值域概念节点}});
                if (比较.状态 != 新特征比较状态::已完成 || !比较.关系编码)
                    throw std::logic_error("冻结采用目标域复核失败");
                const auto 关系 = static_cast<特征概念值域关系>(*比较.关系编码);
                采用保证满足目标 = 关系 == 特征概念值域关系::相等
                    || 关系 == 特征概念值域关系::右包含左
                    || ((关系 == 特征概念值域关系::左包含右 || 关系 == 特征概念值域关系::相交)
                        && !本能采用.线性关系组.empty());
            } else if (项.特征值域概念节点) {
                const auto 域 = 特征概念服务_.获取特征概念(*项.特征值域概念节点);
                if (!域) throw std::logic_error("冻结采用结果域读取失败");
                const auto 保证 = 结果域保证满足需求(特征概念服务_, 二次特征概念服务_, 需求->目标, *域);
                if (!保证) {
                    结果.状态 = 新任务治理执行方向核验状态::结果定义暂不支持;
                    return 结果;
                }
                采用保证满足目标 = *保证;
            } else {
                结果.状态 = 新任务治理执行方向核验状态::结果定义暂不支持;
                return 结果;
            }
            std::optional<特征概念I64闭区间> 区间 = 项.精确数值域;
            if (!区间 && 项.特征值域概念节点) {
                const auto 域 = 特征概念服务_.获取特征概念(*项.特征值域概念节点);
                if (!域) throw std::logic_error("冻结采用结果域读取失败");
                if (const auto* 组 = std::get_if<std::vector<特征概念I64闭区间>>(&域->值域); 组 && !组->empty())
                    区间 = 特征概念I64闭区间{组->front().下界, 组->back().上界};
            }
            const auto 当前特征 = 结果.任务目标当前结果->实际特征节点
                ? 特征服务_.获取特征(*结果.任务目标当前结果->实际特征节点) : std::nullopt;
            const auto* 当前值 = 当前特征 && 当前特征->当前值
                ? std::get_if<std::int64_t>(&*当前特征->当前值) : nullptr;
            const bool 需要增加 = 需求->目标.目标形式 == 新需求目标形式::变化方向
                ? 需求->目标.目标变化方向 == 新需求变化方向::增加
                : 结果.任务目标当前结果->状态 == 新需求当前满足状态::需增加;
            const bool 需要方向 = 需求->目标.目标形式 == 新需求目标形式::变化方向
                || 结果.任务目标当前结果->状态 == 新需求当前满足状态::需增加
                || 结果.任务目标当前结果->状态 == 新需求当前满足状态::需减少;
            if (区间 && 当前值 && 需要方向) {
                const auto 端点 = 需要增加 ? 区间->上界 : 区间->下界;
                采用方向推进 = (需要增加 ? 区间->下界 > *当前值 : 区间->上界 < *当前值)
                    && 端点 != (std::numeric_limits<std::int64_t>::min)()
                    && 端点 != (std::numeric_limits<std::int64_t>::max)();
            }
            break;
        }
        const bool 无向目标已获保证 = 采用保证满足目标
            && 结果.任务目标当前结果->状态 == 新需求当前满足状态::未满足但无有向差值;
        if ((!当前结果可用(*结果.任务目标当前结果) && !无向目标已获保证)
            || !当前结果可用(*结果.安全当前结果)
            || !当前结果可用(*结果.服务当前结果)) {
            结果.状态 = 新任务治理执行方向核验状态::结构不一致;
            return 结果;
        }
        if (结果.任务目标当前结果->状态 == 新需求当前满足状态::已满足) {
            结果.状态 = 新任务治理执行方向核验状态::任务目标已满足;
            return 结果;
        }

        const bool 是结算任务 = 服务结算配置_
            && 服务结算配置_->结算任务节点 == 任务节点;
        if (是结算任务) {
            const auto 来源 = 任务->服务结算来源
                ? 读取任务虚拟存在引用(任务->服务结算来源->来源任务) : std::nullopt;
            const auto 特征 = 结果.任务目标当前结果->实际特征节点
                ? 特征服务_.获取特征(*结果.任务目标当前结果->实际特征节点) : std::nullopt;
            if (!来源 || 来源->当前状态 != static_cast<std::int64_t>(新任务状态::已完成)
                || !特征 || 特征->当前值
                    != std::optional<新特征准确值>{新特征准确值{服务结算配置_->冻结前值}}
                || 需求->目标.目标值域概念节点 != 服务结算配置_->目标值域概念节点) {
                结果.状态 = 新任务治理执行方向核验状态::定位已变化;
                return 结果;
            }
        }

        const auto 任务目标对象 = 需求->目标存在节点;
        bool 找到任务目标 = false, 任务目标推进 = false;
        bool 任务目标相反 = false, 任务目标不明确 = false;
        for (const auto& 预计 : 已绑定组) {
            if (!预计.变化.必须出现
                || 预计.变化.作用对象节点 != 任务目标对象
                || 预计.特征类型根 != 需求->目标.目标特征类型概念节点) {
                continue;
            }
            找到任务目标 = true;
            // 已按冻结采用和同一D复核正式目标比较关系。
            const bool 值域保证 = 采用保证满足目标;
            if (是结算任务 && !值域保证) {
                结果.状态 = 新任务治理执行方向核验状态::任务目标方向不一致;
                return 结果;
            }
            const bool 需要增加 = 需求->目标.目标形式
                    == 新需求目标形式::变化方向
                ? 需求->目标.目标变化方向 == 新需求变化方向::增加
                : 结果.任务目标当前结果->状态
                    == 新需求当前满足状态::需增加;
            const bool 有目标方向 = 需求->目标.目标形式 == 新需求目标形式::变化方向
                || 结果.任务目标当前结果->状态 == 新需求当前满足状态::需增加
                || 结果.任务目标当前结果->状态 == 新需求当前满足状态::需减少;
            const bool 方向推进 = 采用方向推进;
            const bool 方向相反 = 有目标方向 && (需要增加
                ? 预计.方向 == 预计变化方向::减少
                : 预计.方向 == 预计变化方向::增加);
            if (值域保证 && 方向相反) {
                结果.状态 = 新任务治理执行方向核验状态::结构不一致;
                return 结果;
            }
            if (是结算任务 && 预计.方向 == 预计变化方向::不变) {
                // A0 已在执行前读回，固定目标 B>A0，不能同时声明服务值不变。
                结果.状态 = 新任务治理执行方向核验状态::结构不一致;
                return 结果;
            }
            任务目标推进 = 任务目标推进 || 值域保证 || 方向推进;
            任务目标相反 = 任务目标相反 || 方向相反;
            任务目标不明确 = 任务目标不明确
                || (!值域保证 && !方向推进 && !方向相反
                    && 预计.方向 != 预计变化方向::不变);
        }
        if (任务目标不明确) {
            结果.状态 = 新任务治理执行方向核验状态::结果定义暂不支持;
            return 结果;
        }
        if (!找到任务目标 || 任务目标相反 || !任务目标推进) {
            结果.状态 = 新任务治理执行方向核验状态::任务目标方向不一致;
            return 结果;
        }

        结果.冲突需求查找 = 查找冲突需求(
            任务节点, 结果.预计变化阶段组);
        switch (结果.冲突需求查找->状态) {
        case 新冲突需求查找状态::无冲突:
            break;
        case 新冲突需求查找状态::发现冲突:
            结果.状态 = 新任务治理执行方向核验状态::存在冲突需求;
            return 结果;
        case 新冲突需求查找状态::等待材料:
            结果.状态 = 新任务治理执行方向核验状态::等待材料;
            return 结果;
        case 新冲突需求查找状态::任务不存在:
            结果.状态 = 新任务治理执行方向核验状态::定位已变化;
            return 结果;
        case 新冲突需求查找状态::资源失败:
            结果.状态 = 新任务治理执行方向核验状态::资源失败;
            return 结果;
        case 新冲突需求查找状态::结构不一致:
        default:
            结果.状态 = 新任务治理执行方向核验状态::结构不一致;
            return 结果;
        }

        const auto 核验根影响 = [&](const 新需求信息& 根需求,
                                    const 新需求当前满足结果& 当前) {
            for (const auto& 预计 : 已绑定组) {
                if (预计.变化.作用对象节点 != 自我存在节点
                    || 预计.特征类型根
                        != 根需求.目标.目标特征类型概念节点) continue;
                if (根需求.目标.目标形式 == 新需求目标形式::变化方向) {
                    if (当前.状态 != 新需求当前满足状态::需要执行前后比较
                        || !根需求.目标.目标变化方向) return 2;
                    if (预计.方向 == 预计变化方向::不明确) return 3;
                    const auto 相反 = *根需求.目标.目标变化方向 == 新需求变化方向::增加
                        ? 预计变化方向::减少 : 预计变化方向::增加;
                    if (预计.方向 == 相反) return 1;
                    continue;
                }
                bool 值域结构有效 = true;
                const bool 值域保证 = 根需求.目标.目标值域概念节点
                    && (预计.变化.预期值域概念节点 || 预计.变化.预期精确数值域)
                    && 任务值域包含(特征服务_, 特征概念服务_, 预计.特征类型根,
                        根需求.目标.目标值域概念节点, {},
                        预计.变化.预期值域概念节点, 预计.变化.预期精确数值域, 值域结构有效);
                if (!值域结构有效) return 2;
                if (值域保证) continue;
                if (当前.状态 == 新需求当前满足状态::需增加) {
                    if (预计.方向 == 预计变化方向::减少) return 1;
                    if (预计.方向 == 预计变化方向::不明确) return 3;
                    continue;
                }
                if (当前.状态 == 新需求当前满足状态::需减少) {
                    if (预计.方向 == 预计变化方向::增加) return 1;
                    if (预计.方向 == 预计变化方向::不明确) return 3;
                    continue;
                }
                // 根已经位于INT64_MAX目标时，只接受明确不变或仍保证落在目标值域。
                if (预计.方向 != 预计变化方向::不变) return 1;
            }
            return 0;
        };
        const auto 安全核验 = 核验根影响(*安全需求, *结果.安全当前结果);
        if (安全核验) {
            结果.状态 = 安全核验 == 1
                ? 新任务治理执行方向核验状态::安全方向不一致
                : 安全核验 == 3
                    ? 新任务治理执行方向核验状态::结果定义暂不支持
                    : 新任务治理执行方向核验状态::结构不一致;
            return 结果;
        }
        const auto 服务核验 = 核验根影响(*服务需求, *结果.服务当前结果);
        if (服务核验) {
            结果.状态 = 服务核验 == 1
                ? 新任务治理执行方向核验状态::服务方向不一致
                : 服务核验 == 3
                    ? 新任务治理执行方向核验状态::结果定义暂不支持
                    : 新任务治理执行方向核验状态::结构不一致;
            return 结果;
        }
        结果.状态 = 新任务治理执行方向核验状态::已核验可执行;
        return 结果;
    } catch (const std::bad_alloc&) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"核验任务执行方向", L"执行采用核验资源故障");
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"核验任务执行方向", L"执行采用核验工程故障");
    }
}

新任务治理待执行确认状态 新_任务类::确认任务进入待执行(
    稳定编码 任务节点,
    const 新任务本能采用材料& 本能采用) noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        if (!治理已初始化_) {
            return 新任务治理待执行确认状态::结构不一致;
        }
        const auto 任务 = 获取任务(任务节点);
        if (!任务) return 新任务治理待执行确认状态::结构不一致;
        const auto 来源守卫 = 需求服务_.复核需求服务要求语义(任务->需求节点);
        if (来源守卫.状态 != 新服务要求执行守卫状态::不涉及
            && 来源守卫.状态 != 新服务要求执行守卫状态::当前有效) {
            return 来源守卫.状态 == 新服务要求执行守卫状态::需重新确认
                ? 新任务治理待执行确认状态::定位已变化
                : 来源守卫.状态 == 新服务要求执行守卫状态::资源失败
                    ? 新任务治理待执行确认状态::资源失败 : 新任务治理待执行确认状态::结构不一致;
        }
        if (任务->当前状态 != 新任务状态::待检查条件) {
            return 新任务治理待执行确认状态::任务状态已变化;
        }
        const auto 当前筹办=核验任务条件和参数内部(任务节点,&本能采用,false);
        if(当前筹办.状态!=新任务治理条件筹办状态::已完成
            ||当前筹办.方法筹办组.size()!=1
            ||当前筹办.方法筹办组.front().状态!=新任务候选方法筹办状态::条件具备
            ||当前筹办.方法筹办组.front().参数绑定组!=本能采用.参数绑定组)
            return 新任务治理待执行确认状态::定位已变化;
        const auto 修改 = 修改任务状态(
            任务节点, 新任务状态::待执行);
        if (修改 == 新任务操作状态::已修改) {
            return 新任务治理待执行确认状态::已确认;
        }
        if (修改 == 新任务操作状态::资源失败) {
            return 新任务治理待执行确认状态::资源失败;
        }
        return 修改 == 新任务操作状态::任务状态不允许
            ? 新任务治理待执行确认状态::任务状态已变化
            : 新任务治理待执行确认状态::结构不一致;
    } catch (const std::bad_alloc&) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"确认任务进入待执行", L"执行采用复核资源故障");
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"确认任务进入待执行", L"执行采用复核工程故障");
    }
}

新任务治理本能执行结果 新_任务类::执行任务本能方法(
    const 新任务治理本能执行请求& 请求) noexcept {
    新任务治理本能执行结果 结果;
    结果.原请求 = 请求;
    if (!请求.完整()) return 结果;

    std::shared_ptr<const 方法参数场景> 参数场景;
    std::optional<新任务信息> 开始前任务;
    bool 是服务结算执行 = false;
    try {
        {
            std::lock_guard 锁(新任务互斥);
            是服务结算执行 = 服务结算配置_
                && 服务结算配置_->结算任务节点 == 请求.任务节点;
            if (!治理已初始化_) {
                结果.状态 = 新任务治理本能执行状态::结构不一致;
                return 结果;
            }

            结果.临近方向核验 = 核验任务执行方向(
                请求.任务节点, *请求.本能采用, 请求.自我存在节点,
                请求.安全需求节点, 请求.服务需求节点);
            if (结果.临近方向核验->状态
                    != 新任务治理执行方向核验状态::已核验可执行
                || 结果.临近方向核验->预计变化阶段组
                    != 请求.预计变化阶段组
                || !结果.临近方向核验->结果获取方案
                || *结果.临近方向核验->结果获取方案
                    != 请求.结果获取方案) {
                switch (结果.临近方向核验->状态) {
                case 新任务治理执行方向核验状态::任务状态已变化:
                    结果.状态 = 新任务治理本能执行状态::任务状态已变化;
                    break;
                case 新任务治理执行方向核验状态::资源失败:
                    结果.状态 = 新任务治理本能执行状态::资源失败;
                    break;
                case 新任务治理执行方向核验状态::定位已变化:
                case 新任务治理执行方向核验状态::任务目标已满足:
                case 新任务治理执行方向核验状态::任务目标方向不一致:
                case 新任务治理执行方向核验状态::存在冲突需求:
                case 新任务治理执行方向核验状态::安全方向不一致:
                case 新任务治理执行方向核验状态::服务方向不一致:
                case 新任务治理执行方向核验状态::等待材料:
                    结果.状态 = 新任务治理本能执行状态::定位已变化;
                    break;
                default:
                    结果.状态 = 新任务治理本能执行状态::结构不一致;
                    break;
                }
                return 结果;
            }

            const auto 当前筹办 = 核验任务条件和参数内部(请求.任务节点,&*请求.本能采用,false);
            const auto 方法位置 = std::find_if(
                当前筹办.方法筹办组.begin(), 当前筹办.方法筹办组.end(),
                [&](const 新任务候选方法筹办项& 项) {
                    return 项.方法节点 == 请求.方法节点
                        && 项.状态 == 新任务候选方法筹办状态::条件具备;
                });
            if (当前筹办.状态 != 新任务治理条件筹办状态::已完成
                || !当前筹办.候选已完整核验
                || 方法位置 == 当前筹办.方法筹办组.end()
                || 方法位置->参数绑定组 != 请求.参数绑定组) {
                结果.状态 = 当前筹办.状态
                        == 新任务治理条件筹办状态::资源失败
                    ? 新任务治理本能执行状态::资源失败
                    : 新任务治理本能执行状态::定位已变化;
                return 结果;
            }

            const auto 方法 = 方法服务_.获取方法(请求.方法节点);
            if (!方法 || 方法->实现方式 != 新方法实现方式::本能
                || !方法->本能函数登记名) {
                结果.状态 = 新任务治理本能执行状态::定位已变化;
                return 结果;
            }
            const auto 可用 = 新_本能函数管理类::检查可用(
                *方法->本能函数登记名);
            if (可用 != 新本能函数可用状态::可调用) {
                结果.状态 = 可用 == 新本能函数可用状态::依赖未装配
                        || 可用 == 新本能函数可用状态::未登记
                    ? 新任务治理本能执行状态::执行依赖未就绪
                    : 新任务治理本能执行状态::结构不一致;
                return 结果;
            }

            const auto 场景结果 = 方法服务_.建立参数场景(
                请求.方法节点, 请求.参数绑定组, this);
            if (场景结果.状态 != 新方法操作状态::已建立
                || !场景结果.参数场景) {
                结果.状态 = 场景结果.状态 == 新方法操作状态::资源失败
                    ? 新任务治理本能执行状态::资源失败
                    : 新任务治理本能执行状态::参数场景失效;
                return 结果;
            }
            参数场景 = 场景结果.参数场景;

            const auto 开始时间 = 当前实例UTC强时间();
            if (!开始时间) {
                结果.状态 = 是服务结算执行 ? 新任务治理本能执行状态::工程停止
                    : 新任务治理本能执行状态::资源失败;
                return 结果;
            }
            const auto 确认 = 确认任务进入待执行(
                请求.任务节点, *请求.本能采用);
            if (确认 != 新任务治理待执行确认状态::已确认) {
                结果.状态 = 确认 == 新任务治理待执行确认状态::资源失败
                    ? 新任务治理本能执行状态::资源失败
                    : 确认 == 新任务治理待执行确认状态::结构不一致
                        ? 新任务治理本能执行状态::结构不一致
                    : (确认 == 新任务治理待执行确认状态::任务状态已变化
                        ? 新任务治理本能执行状态::任务状态已变化
                        : 新任务治理本能执行状态::定位已变化);
                return 结果;
            }

            开始前任务 = 获取任务(请求.任务节点);
            if (!开始前任务
                || 开始前任务->当前状态 != 新任务状态::待执行) {
                结果.状态 = 新任务治理本能执行状态::结构不一致;
                return 结果;
            }
            结果.执行开始 = 开始方法执行({
                请求.任务节点, 请求.方法节点,
                *开始时间, 请求.预计变化阶段组,
                请求.结果获取方案,请求.本能采用});
            if (结果.执行开始->状态 != 新任务操作状态::已开始执行
                || !结果.执行开始->执行实例节点) {
                if (结果.执行开始->执行实例节点) {
                    结果.状态 = 新任务治理本能执行状态::已开始但无法收束;
                } else if (结果.执行开始->状态 == 新任务操作状态::入口拒绝) {
                    // 开始入口的确认失效尚未建立X，不得被结算的工程失败映射覆盖。
                    const auto 当前任务 = 获取任务(请求.任务节点);
                    if (当前任务 && 当前任务->当前状态 == 新任务状态::待执行
                        && 当前任务->最近方法节点 == 开始前任务->最近方法节点
                        && 当前任务->最近执行实例节点 == 开始前任务->最近执行实例节点) {
                        const auto 回退 = 修改任务状态(请求.任务节点, 新任务状态::待查找方法);
                        const auto 读回 = 获取任务(请求.任务节点);
                        if (回退 != 新任务操作状态::已修改 || !读回
                            || 读回->当前状态 != 新任务状态::待查找方法) {
                            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"执行任务本能方法", L"零动作条件变化回筹办失败");
                        }
                    }
                    结果.状态 = 新任务治理本能执行状态::定位已变化;
                    return 结果;
                } else if (结果.执行开始->状态
                    == 新任务操作状态::任务状态不允许) {
                    结果.状态 = 新任务治理本能执行状态::任务状态已变化;
                } else if (结果.执行开始->状态 == 新任务操作状态::资源失败) {
                    结果.状态 = 新任务治理本能执行状态::资源失败;
                } else {
                    结果.状态 = 新任务治理本能执行状态::执行开始失败;
                }
                if (是服务结算执行) 结果.状态 = 新任务治理本能执行状态::工程停止;
                return 结果;
            }
        }

        准备执行问题材料(请求, *结果.执行开始->执行实例节点);
        const auto 撤销未进入动作 = [&]() noexcept {
            if (!开始前任务 || !结果.执行开始
                || !结果.执行开始->执行实例节点) return false;
            结果.未进入动作撤销状态 =
                撤销未进入动作执行({
                    请求.任务节点,
                    *结果.执行开始->执行实例节点,
                    开始前任务->最近方法节点,
                    开始前任务->最近执行实例节点});
            const bool 已撤销 = *结果.未进入动作撤销状态
                    == 新任务操作状态::已修改
                || *结果.未进入动作撤销状态
                    == 新任务操作状态::精确重复;
            if (已撤销) {
                std::lock_guard 锁(新任务互斥);
                方法问题暂存组_.erase(结果.执行开始->执行实例节点->值);
            }
            return 已撤销;
        };

        // 场景采集可能耗时。不得沿用筹办或X建立前的确认，也不持领域锁调用动作。
        // 这是临近调用的fresh检查，不是与概念变更跨owner原子互斥的许可。
        const auto 来源守卫 = 需求服务_.复核需求服务要求语义(开始前任务->需求节点);
        if (来源守卫.状态 != 新服务要求执行守卫状态::不涉及
            && 来源守卫.状态 != 新服务要求执行守卫状态::当前有效) {
            const bool 已撤销 = 撤销未进入动作();
            结果.状态 = !已撤销 ? 新任务治理本能执行状态::已开始但无法收束
                : 来源守卫.状态 == 新服务要求执行守卫状态::需重新确认
                    ? 新任务治理本能执行状态::动作未进入
                    : 来源守卫.状态 == 新服务要求执行守卫状态::资源失败
                        ? 新任务治理本能执行状态::资源失败 : 新任务治理本能执行状态::结构不一致;
            return 结果;
        }
        // 外部动作入口可能阻塞或回调其它宿主；不得持有任务治理互斥。
        if(复核执行本能输入(*结果.执行开始->执行实例节点,*参数场景)!=新任务本能输入复核状态::仍具备){
            结果.状态 = 撤销未进入动作() ? 新任务治理本能执行状态::动作未进入 : 新任务治理本能执行状态::已开始但无法收束;
            return 结果;
        }
        {
            新_本能函数管理类::本能执行守卫 调用守卫(
                *参数场景, *结果.执行开始->执行实例节点);
            结果.方法调用 = 方法服务_.调用本能函数(
                请求.方法节点, *参数场景);
        }

        const bool 已调用 = 结果.方法调用->状态
                == 新方法本能调用状态::已调用
            && 结果.方法调用->本次条件结果对.has_value();
        if (!已调用) {
            const bool 已撤销 = 撤销未进入动作();
            结果.状态 = 是服务结算执行 ? 新任务治理本能执行状态::工程停止
                : 已撤销
                ? 新任务治理本能执行状态::动作未进入
                : 新任务治理本能执行状态::已开始但无法收束;
            return 结果;
        }
        const auto& 条件结果对 = *结果.方法调用->本次条件结果对;
        保存执行问题运行对(请求.任务节点,
            *结果.执行开始->执行实例节点, 条件结果对);
        const auto& 函数结果 = 条件结果对.结果;
        if (函数结果.动作动态节点) {
            const auto 记录 = 本能动作读取_
                ? 记录本能动作身份(请求.任务节点, *结果.执行开始->执行实例节点,
                    *函数结果.动作动态节点, *本能动作读取_)
                : 新任务操作状态::依赖未实现;
            if (记录 != 新任务操作状态::已修改 && 记录 != 新任务操作状态::精确重复) {
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"执行任务本能方法",
                    L"本能方法已返回明确动作，无法关联本次执行实例。");
            }
        }
        if (函数结果.状态 == 新本能函数执行状态::工程停止) {
            if (是服务结算执行 && 函数结果.动作动态节点
                && !函数结果.已形成状态节点组.empty()) {
                结果.动作输出记录状态 = 记录方法动作输出(请求.任务节点,
                    *结果.执行开始->执行实例节点, 函数结果.已形成状态节点组);
            } else if (函数结果.动作阶段 == 新本能函数动作阶段::未进入动作
                && !函数结果.动作动态节点 && 函数结果.已形成状态节点组.empty()
                && 函数结果.已形成动态节点组.empty()) {
                (void)撤销未进入动作();
            }
            结果.状态 = 新任务治理本能执行状态::工程停止;
            return 结果;
        }
        if (是服务结算执行 && 函数结果.动作阶段 != 新本能函数动作阶段::未进入动作
            && (!函数结果.动作动态节点
                || 函数结果.状态 != 新本能函数执行状态::已按合同返回
                || 函数结果.结果取得状态 != 新本能函数结果取得状态::已取得
                || 函数结果.已形成状态节点组.empty())) {
            结果.状态 = 新任务治理本能执行状态::工程停止;
            return 结果;
        }
        if (函数结果.动作阶段 == 新本能函数动作阶段::未进入动作) {
            if (函数结果.动作动态节点
                || !函数结果.已形成状态节点组.empty()
                || !函数结果.已形成动态节点组.empty()
                || !函数结果.已形成二次特征结果组.empty()
                || !撤销未进入动作()) {
                结果.状态 = 是服务结算执行 ? 新任务治理本能执行状态::工程停止
                    : 新任务治理本能执行状态::已开始但无法收束;
                return 结果;
            }
            if (是服务结算执行 && 函数结果.状态 == 新本能函数执行状态::参数不匹配
                && 函数结果.结果取得状态 == 新本能函数结果取得状态::未取得) {
                const auto 回筹办 = 修改任务状态(请求.任务节点, 新任务状态::待查找方法);
                结果.状态 = 回筹办 == 新任务操作状态::已修改
                    ? 新任务治理本能执行状态::动作未进入 : 新任务治理本能执行状态::工程停止;
                return 结果;
            }
            结果.状态 = 是服务结算执行 ? 新任务治理本能执行状态::工程停止
                : 函数结果.状态
                    == 新本能函数执行状态::依赖未装配
                ? 新任务治理本能执行状态::执行依赖未就绪
                : 新任务治理本能执行状态::动作未进入;
            return 结果;
        }
        if (!函数结果.已形成状态节点组.empty()) {
            结果.动作输出记录状态 = 记录方法动作输出(
                请求.任务节点, *结果.执行开始->执行实例节点,
                函数结果.已形成状态节点组);
            if (*结果.动作输出记录状态 != 新任务操作状态::已修改
                && *结果.动作输出记录状态 != 新任务操作状态::精确重复) {
                结果.状态 = 是服务结算执行 ? 新任务治理本能执行状态::工程停止
                    : *结果.动作输出记录状态
                        == 新任务操作状态::资源失败
                    ? 新任务治理本能执行状态::资源失败
                    : 新任务治理本能执行状态::已开始但无法收束;
                return 结果;
            }
        }
        结果.状态 = 函数结果.结果取得状态
                    == 新本能函数结果取得状态::已取得
                && !函数结果.已形成状态节点组.empty()
            ? 新任务治理本能执行状态::动作已发出且结果材料已具备
            : 新任务治理本能执行状态::动作已发出等待结果;
        return 结果;
    } catch (const std::bad_alloc&) {
        结果.状态 = 是服务结算执行 ? 新任务治理本能执行状态::工程停止
            : 新任务治理本能执行状态::资源失败;
    } catch (...) {
        结果.状态 = 是服务结算执行 ? 新任务治理本能执行状态::工程停止
            : 新任务治理本能执行状态::结构不一致;
    }
    return 结果;
}

新任务治理结果材料核验结果 新_任务类::核验任务结果材料(
    稳定编码 任务节点,
    稳定编码 执行实例节点,
    const std::vector<稳定编码>& 状态节点组) const noexcept {
    新任务治理结果材料核验结果 结果;
    if (!有效(任务节点) || !有效(执行实例节点)
        || 状态节点组.empty()
        || std::any_of(状态节点组.begin(), 状态节点组.end(),
            [](稳定编码 状态节点) { return !有效(状态节点); })) return 结果;
    try {
        std::lock_guard 锁(新任务互斥);
        if (!治理已初始化_) return 结果;
        const auto 读回 = 读回任务执行(任务节点, 执行实例节点);
        if (读回.状态 != 新任务治理执行读回状态::已读回等待观察) {
            结果.状态 = 读回.状态 == 新任务治理执行读回状态::资源失败
                ? 新任务治理结果材料核验状态::资源失败
                : 读回.状态 == 新任务治理执行读回状态::结构不一致
                    ? 新任务治理结果材料核验状态::结构不一致
                    : 新任务治理结果材料核验状态::任务或执行已变化;
            return 结果;
        }
        const auto& 执行 = 读回.执行;

        struct 已读状态 final {
            稳定编码 节点;
            新状态信息 信息;
            稳定编码 特征类型根;
        };
        std::vector<已读状态> 已读组;
        std::set<std::uint64_t> 已见;
        for (const auto 状态节点 : 状态节点组) {
            if (!有效(状态节点) || !已见.insert(状态节点.值).second) continue;
            const auto 状态 = 状态服务_.获取状态(状态节点);
            if (!状态 && 全局基础数据集.查询节点(状态节点)) {
                结果.状态 = 新任务治理结果材料核验状态::结构不一致;
                return 结果;
            }
            if (!状态 || 状态->强时间.语义 != 执行->开始时间.语义
                || 状态->强时间.纳秒 < 执行->开始时间.纳秒) continue;
            const auto 类型根 = 取得特征概念类型根(
                特征概念服务_, 状态->特征类型概念节点);
            if (!类型根) {
                结果.状态 = 新任务治理结果材料核验状态::结构不一致;
                return 结果;
            }
            bool 相关 = false;
            for (const auto& 目标 : 执行->结果获取方案.目标组) {
                const auto 实际特征 = 特征服务_.获取特征(
                    目标.实际特征节点);
                if (!实际特征) {
                    结果.状态 = 新任务治理结果材料核验状态::结构不一致;
                    return 结果;
                }
                if (目标.作用对象节点 == 状态->被描述存在节点
                    && 目标.实际特征节点
                        == 状态->来源实例特征节点
                    && 目标.特征类型根概念节点 == *类型根
                    && 实际特征->特征概念节点
                        == 状态->特征类型概念节点) {
                    相关 = true;
                    break;
                }
            }
            if (相关) 已读组.push_back({状态节点, *状态, *类型根});
        }
        if (已读组.empty()) {
            结果.状态 = 新任务治理结果材料核验状态::与本次执行无关;
            return 结果;
        }
        for (const auto& 状态 : 已读组) {
            结果.已接受状态节点组.push_back(状态.节点);
        }
        std::sort(结果.已接受状态节点组.begin(),
            结果.已接受状态节点组.end());

        for (const auto& 目标 : 执行->结果获取方案.目标组) {
            if (!目标.必须出现) continue;
            const auto 实际特征 = 特征服务_.获取特征(
                目标.实际特征节点);
            if (!实际特征) {
                结果.状态 = 新任务治理结果材料核验状态::结构不一致;
                return 结果;
            }
            const bool 已具备 = std::any_of(
                已读组.begin(), 已读组.end(), [&](const 已读状态& 状态) {
                    return 状态.信息.被描述存在节点 == 目标.作用对象节点
                        && 状态.信息.来源实例特征节点
                            == 目标.实际特征节点
                        && 状态.特征类型根
                            == 目标.特征类型根概念节点
                        && 状态.信息.特征类型概念节点
                            == 实际特征->特征概念节点;
                });
            if (!已具备) {
                结果.状态 = 新任务治理结果材料核验状态::已接受但仍不足;
                return 结果;
            }
        }
        结果.状态 = 新任务治理结果材料核验状态::材料已足够;
        return 结果;
    } catch (const std::bad_alloc&) {
        结果.状态 = 新任务治理结果材料核验状态::资源失败;
    } catch (...) {
        结果.状态 = 新任务治理结果材料核验状态::结构不一致;
    }
    return 结果;
}

新任务治理结果材料核验结果 新_任务类::查找任务结果材料(
    稳定编码 任务节点,
    稳定编码 执行实例节点) const noexcept {
    新任务治理结果材料核验结果 结果;
    if (!有效(任务节点) || !有效(执行实例节点)) return 结果;
    try {
        std::lock_guard 锁(新任务互斥);
        if (!治理已初始化_) return 结果;
        const auto 读回 = 读回任务执行(任务节点, 执行实例节点);
        if (读回.状态 != 新任务治理执行读回状态::已读回等待观察) {
            结果.状态 = 读回.状态 == 新任务治理执行读回状态::资源失败
                ? 新任务治理结果材料核验状态::资源失败
                : 读回.状态 == 新任务治理执行读回状态::结构不一致
                    ? 新任务治理结果材料核验状态::结构不一致
                    : 新任务治理结果材料核验状态::任务或执行已变化;
            return 结果;
        }
        const auto& 执行 = 读回.执行;

        std::set<std::uint64_t> 已见状态;
        std::vector<稳定编码> 候选状态组;
        for (const auto& 目标 : 执行->结果获取方案.目标组) {
            for (const auto 状态节点 :
                    状态服务_.按实例特征查询状态(目标.实际特征节点)) {
                if (有效(状态节点) && 已见状态.insert(状态节点.值).second) {
                    候选状态组.push_back(状态节点);
                }
            }
        }
        if (候选状态组.empty()) {
            结果.状态 = 新任务治理结果材料核验状态::与本次执行无关;
            return 结果;
        }
        return 核验任务结果材料(
            任务节点, 执行实例节点, 候选状态组);
    } catch (const std::bad_alloc&) {
        结果.状态 = 新任务治理结果材料核验状态::资源失败;
    } catch (...) {
        结果.状态 = 新任务治理结果材料核验状态::结构不一致;
    }
    return 结果;
}

新任务治理结果获取结果 新_任务类::获取任务本能结果(
    const 新任务治理结果获取请求& 请求) noexcept {
    新任务治理结果获取结果 结果;
    结果.原请求 = 请求;
    if (!请求.完整()) {
        结果.状态 = 新任务治理结果获取状态::结构不一致;
        return 结果;
    }
    try {
        const auto 当前结束时间 = 当前实例UTC强时间();
        std::unique_lock 锁(新任务互斥);
        if (!治理已初始化_) return 结果;
        const auto 读回 = 读回任务执行(请求.任务节点, 请求.执行实例节点);
        if (读回.状态 != 新任务治理执行读回状态::已读回等待观察
            && 读回.状态 != 新任务治理执行读回状态::已读回等待评估) {
            结果.状态 = 读回.状态 == 新任务治理执行读回状态::资源失败
                ? 新任务治理结果获取状态::资源失败
                : 读回.状态 == 新任务治理执行读回状态::结构不一致
                    ? 新任务治理结果获取状态::结构不一致
                    : 新任务治理结果获取状态::任务状态已变化;
            return 结果;
        }
        const auto& 执行 = 读回.执行;
        if (执行->状态 == 新任务执行状态::执行中) {
            const auto 材料核验 = 核验任务结果材料(
                请求.任务节点, 请求.执行实例节点, 请求.状态节点组);
            if (材料核验.状态
                    != 新任务治理结果材料核验状态::材料已足够) {
                结果.状态 = 材料核验.状态
                        == 新任务治理结果材料核验状态::资源失败
                    ? 新任务治理结果获取状态::资源失败
                    : (材料核验.状态
                            == 新任务治理结果材料核验状态::任务或执行已变化
                        ? 新任务治理结果获取状态::任务状态已变化
                        : (材料核验.状态
                                == 新任务治理结果材料核验状态::结构不一致
                            ? 新任务治理结果获取状态::结构不一致
                            : 新任务治理结果获取状态::等待结果材料));
                return 结果;
            }
            auto 已接受状态 = 材料核验.已接受状态节点组;
            auto 请求状态 = 请求.状态节点组;
            std::sort(已接受状态.begin(), 已接受状态.end());
            std::sort(请求状态.begin(), 请求状态.end());
            if (已接受状态 != 请求状态) {
                结果.状态 = 新任务治理结果获取状态::结构不一致;
                return 结果;
            }
            if (!当前结束时间) {
                结果.状态 = 新任务治理结果获取状态::资源失败;
                return 结果;
            }
            if (当前结束时间->纳秒 < 执行->开始时间.纳秒) {
                结果.状态 = 新任务治理结果获取状态::结构不一致;
                return 结果;
            }
            结果.执行结束状态 = 结束方法执行({
                请求.任务节点, 请求.执行实例节点,
                *当前结束时间, 请求.状态节点组});
        } else if (执行->状态 == 新任务执行状态::已结束待评估) {
            auto 已有状态 = 执行->实际状态节点组;
            auto 请求状态 = 请求.状态节点组;
            std::sort(已有状态.begin(), 已有状态.end());
            std::sort(请求状态.begin(), 请求状态.end());
            结果.执行结束状态 = 已有状态 == 请求状态
                ? 新任务操作状态::精确重复
                : 新任务操作状态::任务状态不允许;
        } else {
            结果.执行结束状态 = 新任务操作状态::任务状态不允许;
        }
        if (*结果.执行结束状态 != 新任务操作状态::已结束执行
            && *结果.执行结束状态 != 新任务操作状态::精确重复) {
            结果.状态 = *结果.执行结束状态
                    == 新任务操作状态::任务状态不允许
                ? 新任务治理结果获取状态::任务状态已变化
                : (*结果.执行结束状态 == 新任务操作状态::资源失败
                    ? 新任务治理结果获取状态::资源失败
                    : 新任务治理结果获取状态::执行结束失败);
            return 结果;
        }
        结果.影响评估 = 评估方法执行影响(
            请求.任务节点, 请求.执行实例节点);
        if (结果.影响评估->状态 != 新任务操作状态::已完成评估
            && 结果.影响评估->状态 != 新任务操作状态::精确重复) {
            结果.状态 = 结果.影响评估->状态
                    == 新任务操作状态::资源失败
                ? 新任务治理结果获取状态::资源失败
                : 新任务治理结果获取状态::影响评估失败;
            return 结果;
        }
        结果.状态 = 新任务治理结果获取状态::已完成影响评估;
        锁.unlock();
        if (结果.影响评估->评估) {
            完成执行问题采集(请求.任务节点, 请求.执行实例节点,
                *结果.影响评估->评估);
        }
        return 结果;
    } catch (const std::bad_alloc&) {
        结果.状态 = 新任务治理结果获取状态::资源失败;
    } catch (...) {
        结果.状态 = 新任务治理结果获取状态::结构不一致;
    }
    return 结果;
}

新任务治理执行读回结果 新_任务类::读回任务执行(
    稳定编码 任务节点,
    稳定编码 执行实例节点) const noexcept {
    新任务治理执行读回结果 结果;
    try {
        std::lock_guard 锁(新任务互斥);
        if (!治理已初始化_) return 结果;
        结果.任务 = 获取任务(任务节点);
        结果.执行 = 获取执行实例(执行实例节点);
        if (!结果.任务 || !结果.执行) {
            // 不可读的现存节点不是“已撤销/不存在”。
            if ((!结果.任务 && 全局基础数据集.查询节点(任务节点))
                || (!结果.执行 && 全局基础数据集.查询节点(执行实例节点))) return 结果;
            结果.状态 = 新任务治理执行读回状态::任务或执行不存在;
            return 结果;
        }
        if (结果.执行->任务节点 != 任务节点) return 结果;
        const bool 在结果处理范围 = 结果.任务->当前状态 == 新任务状态::执行中
            || 结果.任务->当前状态 == 新任务状态::待影响评估
            || 结果.任务->当前状态 == 新任务状态::等待自我决议
            || (结果.任务->当前状态 == 新任务状态::已完成 && !结果.任务->完成结算节点);
        if (!结果.任务->最近执行实例节点 || !结果.任务->最近方法节点) {
            if (!在结果处理范围 && !结果.任务->最近执行实例节点 && !结果.任务->最近方法节点)
                结果.状态 = 新任务治理执行读回状态::执行不在当前位;
            return 结果;
        }
        if (*结果.任务->最近执行实例节点 != 执行实例节点) {
            const auto 当前读回 = 读回任务执行(任务节点, *结果.任务->最近执行实例节点);
            if (当前读回.状态 == 新任务治理执行读回状态::资源失败) {
                结果.状态 = 新任务治理执行读回状态::资源失败;
            } else if (当前读回.状态 == 新任务治理执行读回状态::已读回等待观察
                || 当前读回.状态 == 新任务治理执行读回状态::已读回等待评估
                || 当前读回.状态 == 新任务治理执行读回状态::已读回影响评估
                || 当前读回.状态 == 新任务治理执行读回状态::执行不在当前位) {
                结果.状态 = 新任务治理执行读回状态::执行不在当前位;
            }
            return 结果;
        }
        if (*结果.任务->最近方法节点 != 结果.执行->方法节点) return 结果;
        if (结果.执行->状态 == 新任务执行状态::执行中
            && 结果.任务->当前状态 == 新任务状态::执行中
            && !结果.执行->影响评估节点) {
            结果.状态 = 新任务治理执行读回状态::已读回等待观察;
            return 结果;
        }
        if (结果.执行->状态 == 新任务执行状态::已结束待评估
            && 结果.任务->当前状态 == 新任务状态::待影响评估
            && !结果.执行->影响评估节点) {
            结果.状态 = 新任务治理执行读回状态::已读回等待评估;
            return 结果;
        }
        if (结果.执行->状态 == 新任务执行状态::已评估
            && (结果.任务->当前状态 == 新任务状态::等待自我决议
                || (结果.任务->当前状态 == 新任务状态::已完成
                    && !结果.任务->完成结算节点))
            && 结果.执行->影响评估节点) {
            结果.影响评估 = 获取影响评估(
                *结果.执行->影响评估节点);
            if (结果.影响评估
                && 结果.影响评估->任务节点 == 任务节点
                && 结果.影响评估->执行实例节点 == 执行实例节点) {
                结果.状态 = 新任务治理执行读回状态::已读回影响评估;
            }
            return 结果;
        }
        if (!在结果处理范围)
            结果.状态 = 新任务治理执行读回状态::执行不在当前位;
        return 结果;
    } catch (const std::bad_alloc&) {
        结果.状态 = 新任务治理执行读回状态::资源失败;
    } catch (...) {
        结果.状态 = 新任务治理执行读回状态::结构不一致;
    }
    return 结果;
}

新需求当前满足结果 新_任务类::复核任务当前需求(
    稳定编码 任务节点) const noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        if (!治理已初始化_) return {};
        return 复核任务需求(任务节点);
    } catch (...) {
        新需求当前满足结果 结果;
        结果.状态 = 新需求当前满足状态::资源失败;
        return 结果;
    }
}

新任务治理后继发布结果 新_任务类::发布自我任务后继决议(
    const 新自我任务后继决议& 决议) noexcept {
    新任务治理后继发布结果 结果;
    结果.决议 = 决议;
    if (!有效(决议.任务节点) || !有效(决议.执行实例节点)
        || !有效(决议.影响评估节点)) return 结果;
    try {
        std::lock_guard 锁(新任务互斥);
        if (!治理已初始化_) return 结果;
        const auto 任务 = 获取任务(决议.任务节点);
        const auto 执行 = 获取执行实例(决议.执行实例节点);
        const auto 评估 = 获取影响评估(决议.影响评估节点);
        if (!任务 || !执行 || !评估) {
            结果.状态 = 新任务治理后继发布状态::定位已变化;
            return 结果;
        }
        if (执行->任务节点 != 决议.任务节点
            || 评估->任务节点 != 决议.任务节点
            || 评估->执行实例节点 != 决议.执行实例节点
            || !执行->影响评估节点
            || *执行->影响评估节点 != 决议.影响评估节点
            || !任务->最近执行实例节点
            || *任务->最近执行实例节点 != 决议.执行实例节点) {
            结果.状态 = 新任务治理后继发布状态::定位已变化;
            return 结果;
        }
        if (决议.类型 == 新自我任务后继类型::完成
            && !评估->允许形成完成决议()) {
            // manager不能只凭self传来的枚举发布完成；必须重新读回同一A并
            // 证明完整结果闭包允许完成。
            结果.状态 = 新任务治理后继发布状态::状态发布失败;
            return 结果;
        }
        新任务状态 目标状态 = 新任务状态::等待条件;
        switch (决议.类型) {
        case 新自我任务后继类型::完成:
            目标状态 = 新任务状态::已完成; break;
        case 新自我任务后继类型::继续:
        case 新自我任务后继类型::重试:
        case 新自我任务后继类型::重新筹办:
            目标状态 = 新任务状态::待查找方法; break;
        case 新自我任务后继类型::等待:
            目标状态 = 新任务状态::等待条件; break;
        case 新自我任务后继类型::失败:
            目标状态 = 新任务状态::已失败; break;
        case 新自我任务后继类型::取消:
            目标状态 = 新任务状态::已取消; break;
        }
        if (任务->当前状态 == 目标状态) {
            结果.状态 = 新任务治理后继发布状态::精确重复;
            结果.任务 = 任务;
            return 结果;
        }
        if (任务->当前状态 != 新任务状态::等待自我决议
            || 执行->状态 != 新任务执行状态::已评估) {
            结果.状态 = 新任务治理后继发布状态::任务状态已变化;
            return 结果;
        }
        const auto 写入 = 修改任务状态(
            决议.任务节点, 目标状态);
        if (写入 != 新任务操作状态::已修改
            && 写入 != 新任务操作状态::无变化) {
            结果.状态 = 写入 == 新任务操作状态::资源失败
                ? 新任务治理后继发布状态::资源失败
                : 新任务治理后继发布状态::状态发布失败;
            return 结果;
        }
        结果.任务 = 获取任务(决议.任务节点);
        if (!结果.任务 || 结果.任务->当前状态 != 目标状态) {
            结果.状态 = 新任务治理后继发布状态::结构不一致;
            return 结果;
        }
        结果.状态 = 写入 == 新任务操作状态::无变化
            ? 新任务治理后继发布状态::精确重复
            : 新任务治理后继发布状态::已发布并读回;
        return 结果;
    } catch (const std::bad_alloc&) {
        结果.状态 = 新任务治理后继发布状态::资源失败;
    } catch (...) {
        结果.状态 = 新任务治理后继发布状态::结构不一致;
    }
    return 结果;
}

新任务完成结算结果 新_任务类::建立任务完成结算(
    稳定编码 任务节点,
    稳定编码 执行实例节点,
    稳定编码 影响评估节点) noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        if (!治理已初始化_) return {};
        return 结算已完成任务(
            任务节点, 执行实例节点, 影响评估节点);
    } catch (...) {
        新任务完成结算结果 结果;
        结果.状态 = 新任务操作状态::资源失败;
        return 结果;
    }
}

std::optional<新任务完成结算信息>
新_任务类::读取任务完成结算(稳定编码 任务节点) const noexcept {
    try {
        std::lock_guard 锁(新任务互斥);
        if (!治理已初始化_) return std::nullopt;
        const auto 任务 = 获取任务(任务节点);
        if (!任务 || !任务->完成结算节点) return std::nullopt;
        const auto 结算 = 获取完成结算(*任务->完成结算节点);
        if (!结算 || 结算->任务节点 != 任务节点
            || 结算->需求节点 != 任务->需求节点
            || 结算->完成来源 != 新任务完成来源::自我后继决议) {
            return std::nullopt;
        }
        const auto 执行 = 获取执行实例(结算->执行实例节点);
        const auto 评估 = 获取影响评估(结算->影响评估节点);
        if (!执行 || !评估 || 执行->任务节点 != 任务节点
            || 评估->任务节点 != 任务节点
            || 评估->执行实例节点 != 执行->节点
            || !执行->影响评估节点
            || *执行->影响评估节点 != 评估->节点
            || !评估->允许形成完成决议()) {
            return std::nullopt;
        }
        auto 结果闭包 = 执行->实际状态节点组;
        std::sort(结果闭包.begin(), 结果闭包.end(),
            [](稳定编码 左, 稳定编码 右) { return 左.值 < 右.值; });
        if (结果闭包.empty() || 结果闭包 != 结算->结果状态节点组) {
            return std::nullopt;
        }
        return 结算;
    } catch (...) {
        return std::nullopt;
    }
}

新任务治理承接结果 新_任务类::读回需求任务(
    稳定编码 需求节点, 稳定编码 任务节点) const noexcept {
    新任务治理承接结果 结果;
    结果.需求节点 = 需求节点;
    try {
        std::lock_guard 锁(新任务互斥);
        if (!治理已初始化_) {
            结果.状态 = 新任务治理承接状态::尚未初始化;
            return 结果;
        }
        if (!需求服务_.获取需求(需求节点)) {
            结果.状态 = 新任务治理承接状态::需求不存在;
            return 结果;
        }
        结果.任务 = 获取任务(任务节点);
        结果.虚拟存在 = 获取任务虚拟存在(任务节点);
        const auto 集合 = 查询全部任务();
        if (集合.状态 == 新任务操作状态::资源失败) {
            结果.状态 = 新任务治理承接状态::资源失败;
            return 结果;
        }
        if (集合.状态 != 新任务操作状态::已找到 || !结果.任务 || !结果.虚拟存在
            || 查询需求任务(需求节点) != 任务节点
            || 结果.任务->节点 != 任务节点 || 结果.任务->需求节点 != 需求节点
            || 结果.任务->任务虚拟存在节点 != 结果.虚拟存在->节点
            || 结果.虚拟存在->任务节点 != 任务节点
            || 结果.任务->当前状态 != 结果.虚拟存在->当前状态
            || std::find(集合.任务节点组.begin(), 集合.任务节点组.end(), 任务节点)
                == 集合.任务节点组.end()) return 结果;
        const auto 状态 = 结果.任务->当前状态;
        结果.状态 = 是终结状态(状态)
            ? 新任务治理承接状态::既有终结任务 : 新任务治理承接状态::已读回;
        return 结果;
    } catch (...) {
        结果.状态 = 新任务治理承接状态::资源失败;
        return 结果;
    }
}

新任务治理承接结果 新_任务类::承接需求任务(
    稳定编码 需求节点,
    bool 自我新鲜自动决议) noexcept {
    新任务治理承接结果 结果;
    结果.需求节点 = 需求节点;
    try {
        std::lock_guard 锁(新任务互斥);
        if (!治理已初始化_) {
            结果.状态 = 新任务治理承接状态::尚未初始化;
            return 结果;
        }
        if (!需求服务_.获取需求(需求节点)) {
            结果.状态 = 新任务治理承接状态::需求不存在;
            return 结果;
        }
        if (自我新鲜自动决议) {
            结果.当前复核 = 需求服务_.复核当前满足(需求节点);
            if (结果.当前复核->状态 == 新需求当前满足状态::已满足) {
                结果.状态 = 新任务治理承接状态::需求当前已满足;
                return 结果;
            }
            if (结果.当前复核->状态 != 新需求当前满足状态::需增加) {
                结果.状态 = 新任务治理承接状态::任务操作未完成;
                return 结果;
            }
        }
        结果.写入已尝试 = true;
        结果.建立结果 = 建立或取得任务(需求节点);
        const auto& 建立 = *结果.建立结果;
        if ((建立.状态 != 新任务操作状态::已建立 && 建立.状态 != 新任务操作状态::已复用)
            || !建立.任务节点 || !建立.任务虚拟存在节点) {
            结果.状态 = 新任务治理承接状态::任务操作未完成;
            return 结果;
        }
        auto 读回 = 读回需求任务(需求节点, *建立.任务节点);
        读回.写入已尝试 = true;
        读回.建立结果 = 建立;
        读回.当前复核 = 结果.当前复核;
        if (读回.虚拟存在 && 读回.虚拟存在->节点 != *建立.任务虚拟存在节点)
            读回.状态 = 新任务治理承接状态::读回不一致;
        if (自我新鲜自动决议
            && 读回.状态 == 新任务治理承接状态::已读回
            && 读回.任务
            && (读回.任务->当前状态 == 新任务状态::等待条件
                || 读回.任务->当前状态 == 新任务状态::已建立)) {
            // fresh承接在同一owner串行区发布待筹办状态；随后到达的扫描
            // 只补D-T关联，不能再把已获准筹办的任务降回等待条件。
            读回.重新启用结果 = 修改任务状态(
                *建立.任务节点, 新任务状态::待查找方法);
            if (*读回.重新启用结果 != 新任务操作状态::已修改) {
                读回.状态 = 新任务治理承接状态::任务操作未完成;
                return 读回;
            }
            auto 已唤醒 = 读回需求任务(需求节点, *建立.任务节点);
            已唤醒.写入已尝试 = true;
            已唤醒.建立结果 = 建立;
            已唤醒.当前复核 = 读回.当前复核;
            已唤醒.重新启用结果 = 读回.重新启用结果;
            if (已唤醒.状态 != 新任务治理承接状态::已读回
                || !已唤醒.任务
                || 已唤醒.任务->当前状态 != 新任务状态::待查找方法) {
                已唤醒.状态 = 新任务治理承接状态::读回不一致;
                return 已唤醒;
            }
            已唤醒.状态 = 新任务治理承接状态::已重新启用;
            return 已唤醒;
        }
        if (读回.状态 != 新任务治理承接状态::既有终结任务
            || !自我新鲜自动决议) {
            return 读回;
        }

        // 旧消息本身不能复活任务。即使上层声明这是新鲜决议，仍在同一
        // 在任务 owner 串行区内重新读取当前需求，只接受根需求的正向缺口。
        读回.当前复核 = 需求服务_.复核当前满足(需求节点);
        if (读回.当前复核->状态 == 新需求当前满足状态::已满足) {
            读回.状态 = 新任务治理承接状态::需求当前已满足;
            return 读回;
        }
        if (读回.当前复核->状态 != 新需求当前满足状态::需增加) {
            读回.状态 = 新任务治理承接状态::任务操作未完成;
            return 读回;
        }
        读回.重新启用结果 = 重新启用任务(*建立.任务节点);
        if (*读回.重新启用结果 != 新任务操作状态::已修改) {
            读回.状态 = 新任务治理承接状态::任务操作未完成;
            return 读回;
        }
        auto 已重新启用 = 读回需求任务(需求节点, *建立.任务节点);
        已重新启用.写入已尝试 = true;
        已重新启用.建立结果 = 建立;
        已重新启用.当前复核 = 读回.当前复核;
        已重新启用.重新启用结果 = 读回.重新启用结果;
        if (已重新启用.状态 != 新任务治理承接状态::已读回
            || !已重新启用.任务
            || 已重新启用.任务->当前状态 != 新任务状态::待查找方法) {
            已重新启用.状态 = 新任务治理承接状态::读回不一致;
            return 已重新启用;
        }
        已重新启用.状态 = 新任务治理承接状态::已重新启用;
        return 已重新启用;
    } catch (...) {
        结果.状态 = 新任务治理承接状态::资源失败;
        return 结果;
    }
}

新任务治理承接结果 新_任务类::承接已确认条件子需求(
    稳定编码 父任务节点, 稳定编码 子需求节点) noexcept {
    新任务治理承接结果 结果;
    结果.需求节点 = 子需求节点;
    try {
        std::lock_guard 锁(新任务互斥);
        if (!治理已初始化_) {
            结果.状态 = 新任务治理承接状态::尚未初始化;
            return 结果;
        }
        const auto 父任务 = 获取任务(父任务节点);
        const auto 子需求 = 需求服务_.获取需求(子需求节点);
        if (!子需求) {
            结果.状态 = 新任务治理承接状态::需求不存在;
            return 结果;
        }
        if (!父任务 || 是终结状态(父任务->当前状态)
            || 查询需求任务(父任务->需求节点) != 父任务节点
            || 子需求->父需求节点 != 父任务->需求节点
            || 子需求->来源类型 != 新需求来源类型::任务子目标
            || 子需求->来源承接节点 != 父任务节点
            || !是条件包含目标(子需求->目标, 特征概念服务_, 二次特征概念服务_)) return 结果;
        if (子需求->活动状态 == 新需求活动状态::失活) {
            结果.状态 = 新任务治理承接状态::任务操作未完成;
            return 结果;
        }
        结果.当前复核 = 需求服务_.复核当前满足(子需求节点);
        const auto 当前状态 = 结果.当前复核->状态;
        if (当前状态 == 新需求当前满足状态::资源失败) {
            结果.状态 = 新任务治理承接状态::资源失败;
            return 结果;
        }
        if (当前状态 != 新需求当前满足状态::已满足
            && 当前状态 != 新需求当前满足状态::需增加
            && 当前状态 != 新需求当前满足状态::需减少
            && 当前状态 != 新需求当前满足状态::未满足但无有向差值) {
            结果.状态 = 新任务治理承接状态::任务操作未完成;
            return 结果;
        }
        if (!结果.当前复核->实际特征节点) return 结果;
        const auto 当前特征 = 特征服务_.获取特征(*结果.当前复核->实际特征节点);
        const auto 域 = 特征概念服务_.获取特征概念(*子需求->目标.目标值域概念节点);
        if (!当前特征 || !当前特征->当前值 || !域
            || 当前特征->实例单位 != 域->单位) return 结果;
        if (const auto* 集合 = std::get_if<特征概念任务集合值域>(&域->值域)) {
            const auto 比较 = 特征服务_.比较任务集合条件(
                子需求->目标.目标特征类型概念节点, *当前特征->当前值, *集合);
            if (比较.状态 != 特征概念规则状态::已完成 || !比较.比较值) {
                结果.状态 = 比较.状态 == 特征概念规则状态::资源失败
                    ? 新任务治理承接状态::资源失败 : 新任务治理承接状态::任务操作未完成;
                return 结果;
            }
        } else if (!std::holds_alternative<std::int64_t>(*当前特征->当前值)) return 结果;
        auto 关联 = 建立需求任务关联(子需求节点);
        关联.当前复核 = 结果.当前复核;
        if (关联.状态 != 新任务治理承接状态::已读回
            && 关联.状态 != 新任务治理承接状态::既有终结任务) return 关联;
        // 已满足只补关联；终结任务只读回，不把旧采用回执当作重新启用决议。
        if (关联.状态 == 新任务治理承接状态::既有终结任务) return 关联;
        if (当前状态 == 新需求当前满足状态::已满足) {
            关联.状态 = 新任务治理承接状态::需求当前已满足;
            return 关联;
        }
        if (!关联.任务) return 结果;
        if (关联.任务->当前状态 != 新任务状态::已建立
            && 关联.任务->当前状态 != 新任务状态::等待条件) return 关联;
        关联.重新启用结果 = 修改任务状态(关联.任务->节点, 新任务状态::待查找方法);
        if (*关联.重新启用结果 != 新任务操作状态::已修改
            && *关联.重新启用结果 != 新任务操作状态::无变化) {
            关联.状态 = 新任务治理承接状态::任务操作未完成;
            return 关联;
        }
        auto 已唤醒 = 读回需求任务(子需求节点, 关联.任务->节点);
        已唤醒.写入已尝试 = true;
        已唤醒.建立结果 = 关联.建立结果;
        已唤醒.当前复核 = 关联.当前复核;
        已唤醒.重新启用结果 = 关联.重新启用结果;
        if (已唤醒.状态 != 新任务治理承接状态::已读回 || !已唤醒.任务
            || 已唤醒.任务->当前状态 != 新任务状态::待查找方法) {
            已唤醒.状态 = 新任务治理承接状态::读回不一致;
            return 已唤醒;
        }
        已唤醒.状态 = 新任务治理承接状态::已重新启用;
        return 已唤醒;
    } catch (...) {
        结果.状态 = 新任务治理承接状态::资源失败;
        return 结果;
    }
}

新任务治理承接结果 新_任务类::建立需求任务关联(
    稳定编码 需求节点) noexcept {
    新任务治理承接结果 结果;
    结果.需求节点 = 需求节点;
    try {
        std::lock_guard 锁(新任务互斥);
        if (!治理已初始化_) {
            结果.状态 = 新任务治理承接状态::尚未初始化;
            return 结果;
        }
        const auto 需求 = 需求服务_.获取需求(需求节点);
        if (!需求) {
            结果.状态 = 新任务治理承接状态::需求不存在;
            return 结果;
        }
        结果.写入已尝试 = true;
        结果.建立结果 = 建立或取得任务(需求节点);
        if (!结果.建立结果->任务节点
            || (结果.建立结果->状态 != 新任务操作状态::已建立
                && 结果.建立结果->状态 != 新任务操作状态::已复用)) {
            结果.状态 = 新任务治理承接状态::任务操作未完成;
            return 结果;
        }
        const auto 任务节点 = *结果.建立结果->任务节点;
        auto 读回 = 读回需求任务(需求节点, 任务节点);
        读回.写入已尝试 = true;
        读回.建立结果 = 结果.建立结果;
        if (读回.状态 != 新任务治理承接状态::已读回
            && 读回.状态 != 新任务治理承接状态::既有终结任务) {
            return 读回;
        }
        // 扫描补齐只建立唯一D-T关联，不携带self对该需求仍需治理的
        // fresh决议。无论本次新建还是复用并发入口刚建立的T，都先收束到
        // 等待条件；后续只能由self fresh决议经manager复核后唤醒。
        if (读回.任务 && 读回.任务->当前状态 == 新任务状态::已建立) {
            const auto 等待 = 修改任务状态(任务节点, 新任务状态::等待条件);
            if (等待 != 新任务操作状态::已修改
                && 等待 != 新任务操作状态::无变化) {
                读回.状态 = 等待 == 新任务操作状态::资源失败
                    ? 新任务治理承接状态::资源失败
                    : 新任务治理承接状态::任务操作未完成;
                return 读回;
            }
            auto 等待读回 = 读回需求任务(需求节点, 任务节点);
            等待读回.写入已尝试 = true;
            等待读回.建立结果 = 结果.建立结果;
            return 等待读回;
        }
        return 读回;
    } catch (...) {
        结果.状态 = 新任务治理承接状态::资源失败;
        return 结果;
    }
}

} // namespace 海中鱼巣
