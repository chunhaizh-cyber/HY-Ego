#include "新_场景类.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <map>
#include <mutex>
#include <set>

namespace 海中鱼巣 {

namespace {

std::recursive_mutex 新场景互斥;

稳定编码 场景身份字段{};
稳定编码 场景节点类型{};
稳定编码 成员存在字段{};
稳定编码 边界存在字段{};
稳定编码 状态列表字段{};
稳定编码 动态列表字段{};

constexpr std::int64_t 场景组成子场景关系角色 = 3;

bool 节点仍存在(稳定编码 节点) noexcept {
    return 有效(节点) && 全局基础数据集.查询节点(节点).has_value();
}

bool 结构已初始化() noexcept {
    return 有效(场景身份字段)
        && 有效(场景节点类型)
        && 有效(成员存在字段)
        && 有效(边界存在字段)
        && 有效(状态列表字段)
        && 有效(动态列表字段);
}

bool 结构仍存在() noexcept {
    return 结构已初始化()
        && 节点仍存在(场景身份字段)
        && 节点仍存在(场景节点类型)
        && 节点仍存在(成员存在字段)
        && 节点仍存在(边界存在字段)
        && 节点仍存在(状态列表字段)
        && 节点仍存在(动态列表字段);
}

std::optional<稳定编码> 读取唯一节点字段(
    稳定编码 节点, 稳定编码 字段) {
    const auto 字段组 = 全局基础数据集.查询字段(节点, 字段);
    if (字段组.size() != 1) return std::nullopt;
    const auto* 目标 = std::get_if<稳定编码>(&字段组.front().内容);
    return 目标 ? std::optional<稳定编码>{*目标} : std::nullopt;
}

void 排序去重(std::vector<稳定编码>& 节点组) {
    std::sort(节点组.begin(), 节点组.end());
    节点组.erase(std::unique(节点组.begin(), 节点组.end()), 节点组.end());
}

std::optional<long double> 面到参考点平方距离(
    const 场景三维边界候选面& 面,
    const 三维毫米坐标& 参考点) {
    if (面.面序号 == 0 || 面.顶点组.size() < 3) return std::nullopt;
    const auto& 原点 = 面.顶点组.front();
    std::array<long double, 3> 法向量{};
    bool 已找到法向量 = false;
    for (std::size_t i = 1; i + 1 < 面.顶点组.size(); ++i) {
        const std::array<long double, 3> 左{
            static_cast<long double>(面.顶点组[i].X) - 原点.X,
            static_cast<long double>(面.顶点组[i].Y) - 原点.Y,
            static_cast<long double>(面.顶点组[i].Z) - 原点.Z};
        for (std::size_t j = i + 1; j < 面.顶点组.size(); ++j) {
            const std::array<long double, 3> 右{
                static_cast<long double>(面.顶点组[j].X) - 原点.X,
                static_cast<long double>(面.顶点组[j].Y) - 原点.Y,
                static_cast<long double>(面.顶点组[j].Z) - 原点.Z};
            法向量 = {
                左[1] * 右[2] - 左[2] * 右[1],
                左[2] * 右[0] - 左[0] * 右[2],
                左[0] * 右[1] - 左[1] * 右[0]};
            const auto 模平方 = 法向量[0] * 法向量[0]
                + 法向量[1] * 法向量[1]
                + 法向量[2] * 法向量[2];
            if (模平方 > 0) {
                已找到法向量 = true;
                break;
            }
        }
        if (已找到法向量) break;
    }
    if (!已找到法向量) return std::nullopt;

    const auto 模平方 = 法向量[0] * 法向量[0]
        + 法向量[1] * 法向量[1]
        + 法向量[2] * 法向量[2];
    constexpr long double 共面相对误差 = 1e-12L;
    for (const auto& 顶点 : 面.顶点组) {
        const auto 点积 = 法向量[0]
                * (static_cast<long double>(顶点.X) - 原点.X)
            + 法向量[1]
                * (static_cast<long double>(顶点.Y) - 原点.Y)
            + 法向量[2]
                * (static_cast<long double>(顶点.Z) - 原点.Z);
        if (点积 * 点积 > 模平方 * 共面相对误差) return std::nullopt;
    }
    const auto 参考点积 = 法向量[0]
            * (static_cast<long double>(参考点.X) - 原点.X)
        + 法向量[1]
            * (static_cast<long double>(参考点.Y) - 原点.Y)
        + 法向量[2]
            * (static_cast<long double>(参考点.Z) - 原点.Z);
    return 参考点积 * 参考点积 / 模平方;
}

std::optional<long double> 线段到参考点平方距离(
    const 场景二维边界候选线段& 线段,
    const 二维毫米坐标& 参考点) {
    if (线段.线段序号 == 0 || 线段.起点 == 线段.终点) return std::nullopt;
    const auto X差 = static_cast<long double>(线段.终点.X) - 线段.起点.X;
    const auto Y差 = static_cast<long double>(线段.终点.Y) - 线段.起点.Y;
    const auto 长度平方 = X差 * X差 + Y差 * Y差;
    if (长度平方 <= 0) return std::nullopt;
    const auto 参考X = static_cast<long double>(参考点.X) - 线段.起点.X;
    const auto 参考Y = static_cast<long double>(参考点.Y) - 线段.起点.Y;
    const auto 比例 = (std::clamp)(
        (参考X * X差 + 参考Y * Y差) / 长度平方, 0.0L, 1.0L);
    const auto 最近X = static_cast<long double>(线段.起点.X) + 比例 * X差;
    const auto 最近Y = static_cast<long double>(线段.起点.Y) + 比例 * Y差;
    const auto 距离X = static_cast<long double>(参考点.X) - 最近X;
    const auto 距离Y = static_cast<long double>(参考点.Y) - 最近Y;
    return 距离X * 距离X + 距离Y * 距离Y;
}

struct 二维点小于 final {
    bool operator()(const 二维毫米坐标& 左, const 二维毫米坐标& 右) const {
        return 左.X < 右.X || (左.X == 右.X && 左.Y < 右.Y);
    }
};

struct 三维点小于 final {
    bool operator()(const 三维毫米坐标& 左, const 三维毫米坐标& 右) const {
        if (左.X != 右.X) return 左.X < 右.X;
        if (左.Y != 右.Y) return 左.Y < 右.Y;
        return 左.Z < 右.Z;
    }
};

struct 二维无向边 final {
    二维毫米坐标 一端;
    二维毫米坐标 另一端;
};

struct 二维无向边小于 final {
    bool operator()(const 二维无向边& 左, const 二维无向边& 右) const {
        二维点小于 比较;
        if (比较(左.一端, 右.一端)) return true;
        if (比较(右.一端, 左.一端)) return false;
        return 比较(左.另一端, 右.另一端);
    }
};

struct 三维无向边 final {
    三维毫米坐标 一端;
    三维毫米坐标 另一端;
};

struct 三维无向边小于 final {
    bool operator()(const 三维无向边& 左, const 三维无向边& 右) const {
        三维点小于 比较;
        if (比较(左.一端, 右.一端)) return true;
        if (比较(右.一端, 左.一端)) return false;
        return 比较(左.另一端, 右.另一端);
    }
};

二维无向边 形成无向边(二维毫米坐标 左, 二维毫米坐标 右) {
    return 二维点小于{}(右, 左)
        ? 二维无向边{右, 左} : 二维无向边{左, 右};
}

三维无向边 形成无向边(三维毫米坐标 左, 三维毫米坐标 右) {
    return 三维点小于{}(右, 左)
        ? 三维无向边{右, 左} : 三维无向边{左, 右};
}

std::optional<std::vector<场景二维边界闭合环>> 形成二维闭合环(
    const std::vector<场景二维边界线段>& 线段组) {
    if (线段组.size() < 3) return std::nullopt;
    std::map<二维毫米坐标, std::vector<std::size_t>, 二维点小于> 邻接;
    std::map<二维无向边, std::size_t, 二维无向边小于> 边计数;
    for (std::size_t i = 0; i < 线段组.size(); ++i) {
        const auto& 线段 = 线段组[i];
        if (线段.起点 == 线段.终点) return std::nullopt;
        邻接[线段.起点].push_back(i);
        邻接[线段.终点].push_back(i);
        if (++边计数[形成无向边(线段.起点, 线段.终点)] != 1) {
            return std::nullopt;
        }
    }
    for (const auto& [_, 相邻线段] : 邻接) {
        if (相邻线段.size() != 2) return std::nullopt;
    }

    std::vector<bool> 已访问(线段组.size(), false);
    std::vector<场景二维边界闭合环> 结果;
    for (std::size_t 起始线段 = 0; 起始线段 < 线段组.size(); ++起始线段) {
        if (已访问[起始线段]) continue;
        场景二维边界闭合环 环;
        auto 当前点 = 线段组[起始线段].起点;
        const auto 起点 = 当前点;
        auto 当前线段 = 起始线段;
        for (;;) {
            if (已访问[当前线段]) return std::nullopt;
            已访问[当前线段] = true;
            环.顶点组.push_back(当前点);
            环.来源边界存在组.push_back(线段组[当前线段].边界存在节点);
            const auto& 线段 = 线段组[当前线段];
            二维毫米坐标 下一点;
            if (线段.起点 == 当前点) 下一点 = 线段.终点;
            else if (线段.终点 == 当前点) 下一点 = 线段.起点;
            else return std::nullopt;
            if (下一点 == 起点) break;
            const auto 位置 = 邻接.find(下一点);
            if (位置 == 邻接.end() || 位置->second.size() != 2) {
                return std::nullopt;
            }
            const auto& 相邻 = 位置->second;
            const auto 下一线段 = 相邻[0] == 当前线段 ? 相邻[1] : 相邻[0];
            当前点 = 下一点;
            当前线段 = 下一线段;
        }
        if (环.顶点组.size() < 3) return std::nullopt;
        排序去重(环.来源边界存在组);
        结果.push_back(std::move(环));
    }
    return 结果;
}

std::optional<std::vector<场景三维边界线段>> 形成三维闭合边线(
    const std::vector<场景三维边界面>& 面组) {
    if (面组.size() < 4) return std::nullopt;
    std::map<三维无向边, std::size_t, 三维无向边小于> 边计数;
    for (const auto& 面 : 面组) {
        if (面.顶点组.size() < 3) return std::nullopt;
        for (std::size_t i = 0; i < 面.顶点组.size(); ++i) {
            const auto& 起点 = 面.顶点组[i];
            const auto& 终点 = 面.顶点组[(i + 1) % 面.顶点组.size()];
            if (起点 == 终点) return std::nullopt;
            ++边计数[形成无向边(起点, 终点)];
        }
    }
    std::vector<场景三维边界线段> 结果;
    结果.reserve(边计数.size());
    for (const auto& [边, 次数] : 边计数) {
        if (次数 != 2) return std::nullopt;
        结果.push_back({边.一端, 边.另一端});
    }
    return 结果;
}

bool 距离相同(long double 左, long double 右) {
    const auto 尺度 = (std::max)({1.0L, std::fabs(左), std::fabs(右)});
    return std::fabs(左 - 右) <= 尺度 * 1e-12L;
}

std::vector<基础外部关系> 查询父场景关系(稳定编码 场景节点) {
    std::vector<基础外部关系> 结果;
    for (const auto& 关系 : 全局基础数据集.查询目标关系(
        场景节点, 基础外部关系类型::父子)) {
        if (关系.角色或顺序 == 场景组成子场景关系角色) {
            结果.push_back(关系);
        }
    }
    return 结果;
}

std::vector<基础外部关系> 查询子场景关系(稳定编码 场景节点) {
    std::vector<基础外部关系> 结果;
    for (const auto& 关系 : 全局基础数据集.查询源关系(
        场景节点, 基础外部关系类型::父子)) {
        if (关系.角色或顺序 == 场景组成子场景关系角色) {
            结果.push_back(关系);
        }
    }
    return 结果;
}

bool 子场景向下可达(稳定编码 起点, 稳定编码 目标) {
    std::vector<稳定编码> 待访问{起点};
    std::set<std::uint64_t> 已访问;
    while (!待访问.empty()) {
        const auto 当前 = 待访问.back();
        待访问.pop_back();
        if (当前 == 目标) return true;
        if (!已访问.insert(当前.值).second) continue;
        for (const auto& 关系 : 查询子场景关系(当前)) {
            待访问.push_back(关系.目标节点);
        }
    }
    return false;
}

constexpr long double 朝向误差上限 = 0.001L;

long double 朝向行点积(
    const 三维朝向矩阵& 矩阵,
    std::size_t 左行,
    std::size_t 右行) {
    long double 结果 = 0;
    for (std::size_t 列 = 0; 列 < 3; ++列) {
        结果 += static_cast<long double>(矩阵.分量[左行 * 3 + 列])
            * static_cast<long double>(矩阵.分量[右行 * 3 + 列]);
    }
    return 结果;
}

bool 朝向矩阵有效(const 三维朝向矩阵& 矩阵) {
    for (const auto 分量 : 矩阵.分量) {
        if (!std::isfinite(分量)) return false;
    }
    for (std::size_t 行 = 0; 行 < 3; ++行) {
        if (std::fabs(朝向行点积(矩阵, 行, 行) - 1.0L)
            > 朝向误差上限) {
            return false;
        }
        for (std::size_t 其它行 = 行 + 1; 其它行 < 3; ++其它行) {
            if (std::fabs(朝向行点积(矩阵, 行, 其它行))
                > 朝向误差上限) {
                return false;
            }
        }
    }
    const auto& 值 = 矩阵.分量;
    const long double 行列式 =
        static_cast<long double>(值[0])
            * (static_cast<long double>(值[4]) * 值[8]
                - static_cast<long double>(值[5]) * 值[7])
        - static_cast<long double>(值[1])
            * (static_cast<long double>(值[3]) * 值[8]
                - static_cast<long double>(值[5]) * 值[6])
        + static_cast<long double>(值[2])
            * (static_cast<long double>(值[3]) * 值[7]
                - static_cast<long double>(值[4]) * 值[6]);
    return std::fabs(行列式 - 1.0L) <= 朝向误差上限;
}

std::optional<std::int64_t> 舍入毫米(long double 数值) {
    if (!std::isfinite(数值)) return std::nullopt;
    constexpr auto 最小 = static_cast<long double>(
        std::numeric_limits<std::int64_t>::min());
    constexpr auto 最大 = static_cast<long double>(
        std::numeric_limits<std::int64_t>::max());
    if (数值 < 最小 || 数值 > 最大) return std::nullopt;
    return static_cast<std::int64_t>(std::llround(数值));
}

std::optional<三维毫米坐标> 计算相对坐标(
    const 三维毫米坐标& 自我坐标,
    const 三维朝向矩阵& 自我朝向,
    const 三维毫米坐标& 存在坐标) {
    const std::array<long double, 3> 差值{
        static_cast<long double>(存在坐标.X) - 自我坐标.X,
        static_cast<long double>(存在坐标.Y) - 自我坐标.Y,
        static_cast<long double>(存在坐标.Z) - 自我坐标.Z};
    std::array<std::optional<std::int64_t>, 3> 分量;
    // 朝向矩阵把自我坐标旋转到场景坐标；转置矩阵执行逆旋转。
    for (std::size_t 自我轴 = 0; 自我轴 < 3; ++自我轴) {
        long double 数值 = 0;
        for (std::size_t 场景轴 = 0; 场景轴 < 3; ++场景轴) {
            数值 += static_cast<long double>(
                自我朝向.分量[场景轴 * 3 + 自我轴]) * 差值[场景轴];
        }
        分量[自我轴] = 舍入毫米(数值);
        if (!分量[自我轴]) return std::nullopt;
    }
    return 三维毫米坐标{*分量[0], *分量[1], *分量[2]};
}

场景相对坐标刷新状态 核验自我姿态(
    稳定编码 场景节点,
    const 自我场景姿态& 姿态,
    std::uint64_t 当前时刻) {
    if (!有效(姿态.自我节点) || 姿态.场景节点 != 场景节点
        || !姿态.定位坐标) {
        return 场景相对坐标刷新状态::需要自我定位坐标;
    }
    if (!姿态.朝向矩阵) {
        return 场景相对坐标刷新状态::需要自我朝向;
    }
    if (!朝向矩阵有效(*姿态.朝向矩阵)) {
        return 场景相对坐标刷新状态::朝向矩阵不合法;
    }
    if (姿态.定位序号 == 0 || 姿态.姿态序号 == 0
        || 姿态.边界定义序号 == 0
        || 姿态.有效截止时刻 < 姿态.形成时刻
        || 当前时刻 > 姿态.有效截止时刻) {
        return 场景相对坐标刷新状态::自我姿态已失效;
    }
    return 场景相对坐标刷新状态::已完成;
}

enum class 相对坐标项形成状态 : std::uint8_t {
    已完成,
    需要定位坐标,
    坐标超出范围
};

struct 相对坐标项形成结果 final {
    相对坐标项形成状态 状态 = 相对坐标项形成状态::需要定位坐标;
    std::optional<场景存在相对坐标> 坐标项;
};

相对坐标项形成结果 形成相对坐标项(
    稳定编码 存在节点,
    const 场景定位坐标读回& 定位,
    const 自我场景姿态& 自我姿态,
    std::uint64_t 当前时刻) {
    if (定位.存在节点 != 存在节点 || 定位.定位序号 == 0
        || 定位.有效截止时刻 < 定位.形成时刻
        || 当前时刻 > 定位.有效截止时刻) {
        return {};
    }
    const auto 相对坐标 = 计算相对坐标(
        *自我姿态.定位坐标, *自我姿态.朝向矩阵, 定位.定位坐标);
    if (!相对坐标) {
        return {相对坐标项形成状态::坐标超出范围, std::nullopt};
    }
    const auto 形成时刻 = std::max(
        定位.形成时刻, 自我姿态.形成时刻);
    const auto 有效截止时刻 = std::min(
        定位.有效截止时刻, 自我姿态.有效截止时刻);
    if (有效截止时刻 < 形成时刻) return {};
    return {
        相对坐标项形成状态::已完成,
        场景存在相对坐标{
            存在节点,
            *相对坐标,
            定位.定位序号,
            自我姿态.姿态序号,
            自我姿态.边界定义序号,
            形成时刻,
            有效截止时刻}};
}

场景定位坐标读回 形成自我定位读回(
    const 自我场景姿态& 自我姿态) {
    return {
        自我姿态.自我节点,
        *自我姿态.定位坐标,
        自我姿态.定位序号,
        自我姿态.形成时刻,
        自我姿态.有效截止时刻};
}

新场景操作状态 映射存在失败(新存在操作状态 状态) noexcept {
    switch (状态) {
    case 新存在操作状态::已完成:
        return 新场景操作状态::已完成;
    case 新存在操作状态::无变化:
        return 新场景操作状态::无变化;
    case 新存在操作状态::仍有子存在:
        return 新场景操作状态::存在仍有子存在;
    case 新存在操作状态::仍被状态引用:
        return 新场景操作状态::存在仍被状态引用;
    case 新存在操作状态::资源失败:
        return 新场景操作状态::资源失败;
    default:
        return 新场景操作状态::存在建立失败;
    }
}

} // namespace

新_场景类::新_场景类(
    稳定编码& 场景树根节点,
    新_存在类& 存在服务,
    新_状态类& 状态服务,
    新_动态类& 动态服务) noexcept
    : 场景树根节点_(场景树根节点),
      存在服务_(存在服务),
      状态服务_(状态服务),
      动态服务_(动态服务) {}

bool 新_场景类::初始化() noexcept {
    try {
        std::lock_guard 锁(新场景互斥);
        if (!存在服务_.初始化() || !状态服务_.初始化()
            || !动态服务_.初始化()) return false;
        if (结构已初始化()) return 结构仍存在();
        auto 建立 = [](稳定编码& 节点) {
            if (有效(节点)) return 节点仍存在(节点);
            节点 = 全局基础数据集.新建节点();
            return 有效(节点);
        };
        return 建立(场景身份字段)
            && 建立(场景节点类型)
            && 建立(成员存在字段)
            && 建立(边界存在字段)
            && 建立(状态列表字段)
            && 建立(动态列表字段);
    } catch (...) {
        return false;
    }
}

std::optional<稳定编码> 新_场景类::获取根场景() const noexcept {
    try {
        std::lock_guard 锁(新场景互斥);
        return 有效(场景树根节点_) && 是场景节点(场景树根节点_)
            ? std::optional<稳定编码>{场景树根节点_}
            : std::nullopt;
    } catch (...) {
        return std::nullopt;
    }
}

新场景建立结果 新_场景类::建立根场景() noexcept {
    新场景建立结果 结果;
    try {
        std::lock_guard 锁(新场景互斥);
        if (!结构仍存在()) return 结果;
        if (有效(场景树根节点_)) {
            const auto 信息 = 获取场景(场景树根节点_);
            if (!信息 || 信息->父场景节点) {
                结果.状态 = 新场景操作状态::结构不一致;
                return 结果;
            }
            结果.状态 = 新场景操作状态::无变化;
            结果.场景节点 = 信息->节点;
            结果.专属存在概念节点 = 信息->专属存在概念节点;
            return 结果;
        }

        const auto 节点 = 全局基础数据集.新建节点();
        if (!有效(节点)) {
            结果.状态 = 新场景操作状态::资源失败;
            return 结果;
        }
        const auto 场景字段 = 全局基础数据集.添加字段节点(
            节点, 场景身份字段, 场景节点类型);
        if (!有效(场景字段)) {
            (void)全局基础数据集.删除节点(节点);
            结果.状态 = 新场景操作状态::资源失败;
            return 结果;
        }
        const auto 存在结果 = 存在服务_.初始化存在节点(
            节点, 新存在建立请求{}, true, std::nullopt);
        if (存在结果.状态 != 新存在操作状态::已完成
            || !存在结果.专属存在概念节点) {
            (void)全局基础数据集.删除字段(场景字段);
            (void)全局基础数据集.删除节点(节点);
            结果.状态 = 映射存在失败(存在结果.状态);
            return 结果;
        }
        场景树根节点_ = 节点;
        结果.状态 = 新场景操作状态::已完成;
        结果.场景节点 = 节点;
        结果.专属存在概念节点 = 存在结果.专属存在概念节点;
        return 结果;
    } catch (...) {
        结果.状态 = 新场景操作状态::资源失败;
        return 结果;
    }
}

新场景建立结果 新_场景类::建立场景(
    稳定编码 父场景节点) noexcept {
    新场景建立结果 结果;
    try {
        std::lock_guard 锁(新场景互斥);
        if (!是场景节点(父场景节点)) {
            结果.状态 = 新场景操作状态::父场景不合法;
            return 结果;
        }
        const auto 节点 = 全局基础数据集.新建节点();
        if (!有效(节点)) {
            结果.状态 = 新场景操作状态::资源失败;
            return 结果;
        }
        const auto 场景字段 = 全局基础数据集.添加字段节点(
            节点, 场景身份字段, 场景节点类型);
        const auto 父子关系 = 有效(场景字段)
            ? 全局基础数据集.添加关系(
                父场景节点, 节点, 基础外部关系类型::父子,
                场景组成子场景关系角色)
            : 稳定编码{};
        if (!有效(父子关系)) {
            if (有效(场景字段)) (void)全局基础数据集.删除字段(场景字段);
            (void)全局基础数据集.删除节点(节点);
            结果.状态 = 新场景操作状态::资源失败;
            return 结果;
        }
        const auto 存在结果 = 存在服务_.初始化存在节点(
            节点, 新存在建立请求{}, true, std::nullopt);
        if (存在结果.状态 != 新存在操作状态::已完成
            || !存在结果.专属存在概念节点) {
            (void)全局基础数据集.删除关系(父子关系);
            (void)全局基础数据集.删除字段(场景字段);
            (void)全局基础数据集.删除节点(节点);
            结果.状态 = 映射存在失败(存在结果.状态);
            return 结果;
        }
        结果.状态 = 新场景操作状态::已完成;
        结果.场景节点 = 节点;
        结果.专属存在概念节点 = 存在结果.专属存在概念节点;
        return 结果;
    } catch (...) {
        结果.状态 = 新场景操作状态::资源失败;
        return 结果;
    }
}

新场景操作状态 新_场景类::迁移子场景(
    稳定编码 场景节点,
    稳定编码 新父场景节点) noexcept {
    try {
        std::lock_guard 锁(新场景互斥);
        if (!是场景节点(场景节点)) {
            return 新场景操作状态::场景不存在;
        }
        if (!是场景节点(新父场景节点)) {
            return 新场景操作状态::父场景不合法;
        }
        if (场景节点 == 场景树根节点_) {
            return 新场景操作状态::根场景不可操作;
        }
        if (场景节点 == 新父场景节点
            || 子场景向下可达(场景节点, 新父场景节点)) {
            return 新场景操作状态::会形成环;
        }
        const auto 原父关系 = 查询父场景关系(场景节点);
        if (原父关系.size() != 1
            || !是场景节点(原父关系.front().源节点)) {
            return 新场景操作状态::结构不一致;
        }
        if (原父关系.front().源节点 == 新父场景节点) {
            return 新场景操作状态::无变化;
        }
        return 全局基础数据集.修改关系(
            原父关系.front().编码,
            新父场景节点,
            场景节点,
            基础外部关系类型::父子,
            场景组成子场景关系角色)
            ? 新场景操作状态::已完成
            : 新场景操作状态::资源失败;
    } catch (...) {
        return 新场景操作状态::资源失败;
    }
}

新场景操作状态 新_场景类::删除空场景(
    稳定编码 场景节点) noexcept {
    try {
        std::lock_guard 锁(新场景互斥);
        if (!是场景节点(场景节点)) {
            return 新场景操作状态::场景不存在;
        }
        if (场景节点 == 场景树根节点_) {
            return 新场景操作状态::根场景不可操作;
        }
        const auto 子场景关系组 = 查询子场景关系(场景节点);
        const auto 成员字段组 = 全局基础数据集.查询字段(
            场景节点, 成员存在字段);
        const auto 状态字段组 = 全局基础数据集.查询字段(
            场景节点, 状态列表字段);
        const auto 动态字段组 = 全局基础数据集.查询字段(
            场景节点, 动态列表字段);
        if (!子场景关系组.empty() || !成员字段组.empty()
            || !状态字段组.empty() || !动态字段组.empty()) {
            return 新场景操作状态::场景非空;
        }
        for (const auto& 字段 : 全局基础数据集.查询目标字段(
            场景节点, 边界存在字段)) {
            if (字段.所属节点 != 场景节点) {
                return 新场景操作状态::存在仍被边界引用;
            }
        }
        const auto 父关系 = 查询父场景关系(场景节点);
        const auto 场景字段组 = 全局基础数据集.查询字段(
            场景节点, 场景身份字段);
        const auto 边界字段组 = 全局基础数据集.查询字段(
            场景节点, 边界存在字段);
        if (父关系.size() != 1
            || !是场景节点(父关系.front().源节点)
            || 场景字段组.size() != 1) {
            return 新场景操作状态::结构不一致;
        }
        for (const auto& 字段 : 边界字段组) {
            if (!全局基础数据集.删除字段(字段.编码)) {
                return 新场景操作状态::资源失败;
            }
        }
        const auto 清理状态 = 存在服务_.清理存在节点内容(场景节点);
        if (清理状态 != 新存在操作状态::已完成) {
            return 映射存在失败(清理状态);
        }
        if (!全局基础数据集.删除关系(父关系.front().编码)
            || !全局基础数据集.删除字段(场景字段组.front().编码)
            || !全局基础数据集.删除节点(场景节点)) {
            return 新场景操作状态::结构不一致;
        }
        return 新场景操作状态::已完成;
    } catch (...) {
        return 新场景操作状态::资源失败;
    }
}

std::optional<新场景信息> 新_场景类::获取场景(
    稳定编码 场景节点) const noexcept {
    try {
        std::lock_guard 锁(新场景互斥);
        if (!是场景节点(场景节点)) return std::nullopt;
        const auto 存在信息 = 存在服务_.获取存在(场景节点);
        if (!存在信息) return std::nullopt;
        const auto 父关系 = 查询父场景关系(场景节点);
        if ((场景节点 == 场景树根节点_ && !父关系.empty())
            || (场景节点 != 场景树根节点_ && 父关系.size() != 1)) {
            return std::nullopt;
        }
        新场景信息 结果;
        结果.节点 = 场景节点;
        if (!父关系.empty()) 结果.父场景节点 = 父关系.front().源节点;
        结果.专属存在概念节点 = 存在信息->专属存在概念节点;
        结果.特征节点组 = 存在信息->特征节点组;
        结果.直接成员存在组 = 查询直接成员存在(场景节点);
        结果.边界存在组 = 查询边界存在(场景节点);
        结果.直接子场景组 = 查询直接子场景(场景节点);
        结果.状态节点组 = 查询状态列表(场景节点);
        结果.动态节点组 = 查询动态列表(场景节点);
        return 结果;
    } catch (...) {
        return std::nullopt;
    }
}

bool 新_场景类::是场景节点(稳定编码 节点) const noexcept {
    try {
        std::lock_guard 锁(新场景互斥);
        const auto 是场景结构节点 = [&](稳定编码 候选) {
            if (!结构仍存在() || !节点仍存在(候选)
                || !存在服务_.是存在节点(候选)) return false;
            const auto 类型 = 读取唯一节点字段(候选, 场景身份字段);
            return 类型 && *类型 == 场景节点类型;
        };
        if (!有效(场景树根节点_)
            || !是场景结构节点(场景树根节点_)
            || !是场景结构节点(节点)) return false;

        std::set<std::uint64_t> 已访问;
        auto 当前 = 节点;
        while (当前 != 场景树根节点_) {
            if (!已访问.insert(当前.值).second) return false;
            const auto 父关系 = 查询父场景关系(当前);
            if (父关系.size() != 1
                || !是场景结构节点(父关系.front().源节点)) {
                return false;
            }
            当前 = 父关系.front().源节点;
        }
        return 查询父场景关系(场景树根节点_).empty();
    } catch (...) {
        return false;
    }
}

新场景成员建立结果 新_场景类::建立成员存在(
    稳定编码 场景节点,
    const 新存在建立请求& 请求,
    const 新状态强时间& 初始状态时间) noexcept {
    新场景成员建立结果 结果;
    try {
        std::lock_guard 锁(新场景互斥);
        if (!是场景节点(场景节点)) {
            结果.状态 = 新场景操作状态::场景不存在;
            return 结果;
        }
        if (请求.父存在节点) {
            const auto 父场景 = 查询所属场景(*请求.父存在节点);
            if (!父场景 || *父场景 != 场景节点) {
                结果.状态 = 新场景操作状态::存在已属于其它场景;
                return 结果;
            }
        }
        const auto 节点 = 全局基础数据集.新建节点();
        if (!有效(节点)) {
            结果.状态 = 新场景操作状态::资源失败;
            return 结果;
        }
        const auto 成员字段 = 全局基础数据集.添加字段节点(
            场景节点, 成员存在字段, 节点);
        if (!有效(成员字段)) {
            (void)全局基础数据集.删除节点(节点);
            结果.状态 = 新场景操作状态::资源失败;
            return 结果;
        }
        const auto 存在结果 = 存在服务_.初始化存在节点(
            节点, 请求, false, 初始状态时间);
        if (存在结果.状态 != 新存在操作状态::已完成) {
            (void)全局基础数据集.删除字段(成员字段);
            (void)全局基础数据集.删除节点(节点);
            结果.状态 = 映射存在失败(存在结果.状态);
            return 结果;
        }
        结果.状态 = 新场景操作状态::已完成;
        结果.存在节点 = 节点;
        结果.专属存在概念节点 = 存在结果.专属存在概念节点;
        结果.初始状态节点组 = 存在结果.初始状态节点组;
        for (std::size_t i = 0; i < 结果.初始状态节点组.size(); ++i) {
            const auto 状态节点 = 结果.初始状态节点组[i];
            const auto 挂接状态 = 添加状态(场景节点, 状态节点);
            if (挂接状态 != 新场景操作状态::已完成
                && 挂接状态 != 新场景操作状态::无变化) {
                for (std::size_t j = i; j < 结果.初始状态节点组.size(); ++j) {
                    (void)状态服务_.删除状态(结果.初始状态节点组[j]);
                }
                结果.初始状态节点组.resize(i);
                结果.状态 = 新场景操作状态::存在已建立但状态挂接未完成;
                return 结果;
            }
        }
        return 结果;
    } catch (...) {
        结果.状态 = 新场景操作状态::资源失败;
        return 结果;
    }
}

新场景成员建立结果 新_场景类::建立区间成员存在(
    稳定编码 场景节点,
    const 新场景区间成员建立请求& 请求) noexcept {
    新场景成员建立结果 结果;
    稳定编码 已建节点{};
    稳定编码 已建成员字段{};
    bool 已初始化存在 = false;
    try {
        std::lock_guard 锁(新场景互斥);
        if (!是场景节点(场景节点)) {
            结果.状态 = 新场景操作状态::场景不存在;
            return 结果;
        }
        if (请求.父存在节点) {
            const auto 父场景 = 查询所属场景(*请求.父存在节点);
            if (!父场景 || *父场景 != 场景节点) {
                结果.状态 = 新场景操作状态::存在已属于其它场景;
                return 结果;
            }
        }

        已建节点 = 全局基础数据集.新建节点();
        if (!有效(已建节点)) {
            结果.状态 = 新场景操作状态::资源失败;
            return 结果;
        }
        已建成员字段 = 全局基础数据集.添加字段节点(
            场景节点, 成员存在字段, 已建节点);
        if (!有效(已建成员字段)) {
            (void)全局基础数据集.删除节点(已建节点);
            已建节点 = {};
            结果.状态 = 新场景操作状态::资源失败;
            return 结果;
        }

        新存在建立请求 空请求;
        空请求.父存在节点 = 请求.父存在节点;
        const auto 存在结果 = 存在服务_.初始化存在节点(
            已建节点, 空请求, true, std::nullopt);
        if (存在结果.状态 != 新存在操作状态::已完成
            || !存在结果.专属存在概念节点) {
            (void)全局基础数据集.删除字段(已建成员字段);
            (void)全局基础数据集.删除节点(已建节点);
            已建成员字段 = {};
            已建节点 = {};
            结果.状态 = 映射存在失败(存在结果.状态);
            return 结果;
        }
        已初始化存在 = true;

        for (const auto& 初始特征 : 请求.初始特征组) {
            const auto 添加 = 存在服务_.添加特征区间值(
                已建节点, 初始特征.特征概念节点, 初始特征.区间值);
            if (添加.状态 == 新存在操作状态::已完成
                && 添加.特征节点 && !添加.状态结果) {
                continue;
            }
            const auto 清理 = 删除成员存在(场景节点, 已建节点);
            if (清理 == 新场景操作状态::已完成) {
                已建节点 = {};
                已建成员字段 = {};
                已初始化存在 = false;
            }
            结果.状态 = 清理 == 新场景操作状态::已完成
                ? 映射存在失败(添加.状态)
                : 新场景操作状态::结构不一致;
            return 结果;
        }

        结果.状态 = 新场景操作状态::已完成;
        结果.存在节点 = 已建节点;
        结果.专属存在概念节点 = 存在结果.专属存在概念节点;
        return 结果;
    } catch (...) {
        if (有效(已建节点)) {
            if (已初始化存在) {
                (void)删除成员存在(场景节点, 已建节点);
            } else {
                if (有效(已建成员字段)) {
                    (void)全局基础数据集.删除字段(已建成员字段);
                }
                (void)全局基础数据集.删除节点(已建节点);
            }
        }
        结果.状态 = 新场景操作状态::资源失败;
        return 结果;
    }
}

新场景操作状态 新_场景类::迁移成员存在(
    稳定编码 存在节点,
    稳定编码 目标场景节点) noexcept {
    try {
        std::lock_guard 锁(新场景互斥);
        if (!存在服务_.是存在节点(存在节点) || 是场景节点(存在节点)) {
            return 新场景操作状态::存在不存在;
        }
        if (!是场景节点(目标场景节点)) {
            return 新场景操作状态::场景不存在;
        }
        const auto 原场景 = 查询所属场景(存在节点);
        if (!原场景) return 新场景操作状态::结构不一致;
        if (*原场景 == 目标场景节点) return 新场景操作状态::无变化;
        if (!动态服务_.按主题查询动态(存在节点).empty()) {
            return 新场景操作状态::存在仍被动态引用;
        }

        const auto 原字段组 = 全局基础数据集.查询目标字段(
            存在节点, 成员存在字段);
        if (原字段组.size() != 1
            || 原字段组.front().所属节点 != *原场景) {
            return 新场景操作状态::结构不一致;
        }
        const auto 新字段 = 全局基础数据集.添加字段节点(
            目标场景节点, 成员存在字段, 存在节点);
        if (!有效(新字段)) return 新场景操作状态::资源失败;
        if (!全局基础数据集.删除字段(原字段组.front().编码)) {
            (void)全局基础数据集.删除字段(新字段);
            return 新场景操作状态::资源失败;
        }
        return 新场景操作状态::已完成;
    } catch (...) {
        return 新场景操作状态::资源失败;
    }
}

新场景操作状态 新_场景类::删除成员存在(
    稳定编码 场景节点,
    稳定编码 存在节点) noexcept {
    try {
        std::lock_guard 锁(新场景互斥);
        if (!是场景节点(场景节点)) return 新场景操作状态::场景不存在;
        if (!存在服务_.是存在节点(存在节点) || 是场景节点(存在节点)) {
            return 新场景操作状态::存在不存在;
        }
        const auto 字段组 = 全局基础数据集.查询目标字段(
            存在节点, 成员存在字段);
        if (字段组.size() != 1 || 字段组.front().所属节点 != 场景节点) {
            return 新场景操作状态::结构不一致;
        }
        if (!存在服务_.查询直接子存在(存在节点).empty()) {
            return 新场景操作状态::存在仍有子存在;
        }
        if (!全局基础数据集.查询目标字段(
            存在节点, 边界存在字段).empty()) {
            return 新场景操作状态::存在仍被边界引用;
        }
        if (!状态服务_.按存在查询状态(存在节点).empty()) {
            return 新场景操作状态::存在仍被状态引用;
        }
        if (!动态服务_.按主题查询动态(存在节点).empty()) {
            return 新场景操作状态::存在仍被动态引用;
        }
        if (!全局基础数据集.删除字段(字段组.front().编码)) {
            return 新场景操作状态::资源失败;
        }
        const auto 清理状态 = 存在服务_.清理存在节点内容(存在节点);
        if (清理状态 != 新存在操作状态::已完成) {
            (void)全局基础数据集.添加字段节点(
                场景节点, 成员存在字段, 存在节点);
            return 映射存在失败(清理状态);
        }
        return 全局基础数据集.删除节点(存在节点)
            ? 新场景操作状态::已完成
            : 新场景操作状态::结构不一致;
    } catch (...) {
        return 新场景操作状态::资源失败;
    }
}

std::optional<稳定编码> 新_场景类::查询所属场景(
    稳定编码 存在节点) const noexcept {
    try {
        std::lock_guard 锁(新场景互斥);
        if (!存在服务_.是存在节点(存在节点) || 是场景节点(存在节点)) {
            return std::nullopt;
        }
        const auto 字段组 = 全局基础数据集.查询目标字段(
            存在节点, 成员存在字段);
        if (字段组.size() != 1 || !是场景节点(字段组.front().所属节点)) {
            return std::nullopt;
        }
        return 字段组.front().所属节点;
    } catch (...) {
        return std::nullopt;
    }
}

std::optional<稳定编码> 新_场景类::查询父场景(
    稳定编码 场景节点) const noexcept {
    try {
        std::lock_guard 锁(新场景互斥);
        if (!是场景节点(场景节点)) return std::nullopt;
        const auto 关系组 = 查询父场景关系(场景节点);
        if (关系组.empty() && 场景节点 == 场景树根节点_) return std::nullopt;
        return 关系组.size() == 1 && 是场景节点(关系组.front().源节点)
            ? std::optional<稳定编码>{关系组.front().源节点}
            : std::nullopt;
    } catch (...) {
        return std::nullopt;
    }
}

std::vector<稳定编码> 新_场景类::查询直接子场景(
    稳定编码 场景节点) const noexcept {
    try {
        std::lock_guard 锁(新场景互斥);
        std::vector<稳定编码> 结果;
        if (!是场景节点(场景节点)) return 结果;
        for (const auto& 关系 : 查询子场景关系(场景节点)) {
            if (!是场景节点(关系.目标节点)) return {};
            结果.push_back(关系.目标节点);
        }
        排序去重(结果);
        return 结果;
    } catch (...) {
        return {};
    }
}

std::vector<稳定编码> 新_场景类::查询直接成员存在(
    稳定编码 场景节点) const noexcept {
    try {
        std::lock_guard 锁(新场景互斥);
        std::vector<稳定编码> 结果;
        if (!是场景节点(场景节点)) return 结果;
        for (const auto& 字段 : 全局基础数据集.查询字段(
            场景节点, 成员存在字段)) {
            const auto* 存在节点 = std::get_if<稳定编码>(&字段.内容);
            if (!存在节点 || !存在服务_.是存在节点(*存在节点)
                || 是场景节点(*存在节点)) return {};
            结果.push_back(*存在节点);
        }
        排序去重(结果);
        return 结果;
    } catch (...) {
        return {};
    }
}

std::vector<稳定编码> 新_场景类::查询场景范围存在(
    稳定编码 场景节点) const noexcept {
    try {
        std::lock_guard 锁(新场景互斥);
        std::vector<稳定编码> 结果;
        if (!是场景节点(场景节点)) return 结果;
        std::vector<稳定编码> 待访问{场景节点};
        std::set<std::uint64_t> 已访问场景;
        while (!待访问.empty()) {
            const auto 当前场景 = 待访问.back();
            待访问.pop_back();
            if (!已访问场景.insert(当前场景.值).second) return {};
            const auto 直接成员 = 查询直接成员存在(当前场景);
            结果.insert(结果.end(), 直接成员.begin(), 直接成员.end());
            const auto 子场景组 = 查询直接子场景(当前场景);
            待访问.insert(待访问.end(), 子场景组.begin(), 子场景组.end());
        }
        排序去重(结果);
        return 结果;
    } catch (...) {
        return {};
    }
}

bool 新_场景类::存在属于场景范围(
    稳定编码 场景节点,
    稳定编码 存在节点) const noexcept {
    try {
        std::lock_guard 锁(新场景互斥);
        if (!是场景节点(场景节点)
            || !存在服务_.是存在节点(存在节点)) {
            return false;
        }
        if (是场景节点(存在节点)) {
            return 子场景向下可达(场景节点, 存在节点);
        }
        const auto 所属场景 = 查询所属场景(存在节点);
        return 所属场景
            && 子场景向下可达(场景节点, *所属场景);
    } catch (...) {
        return false;
    }
}

新场景操作状态 新_场景类::添加边界存在(
    稳定编码 场景节点,
    稳定编码 边界存在节点) noexcept {
    try {
        std::lock_guard 锁(新场景互斥);
        if (!是场景节点(场景节点)) return 新场景操作状态::场景不存在;
        if (!存在服务_.是存在节点(边界存在节点)) {
            return 新场景操作状态::存在不存在;
        }
        const auto 已有字段 = 全局基础数据集.查询字段(
            场景节点, 边界存在字段);
        for (const auto& 字段 : 已有字段) {
            const auto* 已有节点 = std::get_if<稳定编码>(&字段.内容);
            if (!已有节点) return 新场景操作状态::结构不一致;
            if (*已有节点 == 边界存在节点) return 新场景操作状态::无变化;
        }
        return 有效(全局基础数据集.添加字段节点(
            场景节点, 边界存在字段, 边界存在节点))
            ? 新场景操作状态::已完成
            : 新场景操作状态::资源失败;
    } catch (...) {
        return 新场景操作状态::资源失败;
    }
}

新场景操作状态 新_场景类::移除边界存在(
    稳定编码 场景节点,
    稳定编码 边界存在节点) noexcept {
    try {
        std::lock_guard 锁(新场景互斥);
        if (!是场景节点(场景节点)) return 新场景操作状态::场景不存在;
        std::optional<稳定编码> 待删除字段;
        for (const auto& 字段 : 全局基础数据集.查询字段(
            场景节点, 边界存在字段)) {
            const auto* 已有节点 = std::get_if<稳定编码>(&字段.内容);
            if (!已有节点) return 新场景操作状态::结构不一致;
            if (*已有节点 != 边界存在节点) continue;
            if (待删除字段) return 新场景操作状态::结构不一致;
            待删除字段 = 字段.编码;
        }
        if (!待删除字段) return 新场景操作状态::无变化;
        return 全局基础数据集.删除字段(*待删除字段)
            ? 新场景操作状态::已完成
            : 新场景操作状态::资源失败;
    } catch (...) {
        return 新场景操作状态::资源失败;
    }
}

std::vector<稳定编码> 新_场景类::查询边界存在(
    稳定编码 场景节点) const noexcept {
    try {
        std::lock_guard 锁(新场景互斥);
        std::vector<稳定编码> 结果;
        if (!是场景节点(场景节点)) return 结果;
        for (const auto& 字段 : 全局基础数据集.查询字段(
            场景节点, 边界存在字段)) {
            const auto* 存在节点 = std::get_if<稳定编码>(&字段.内容);
            if (!存在节点 || !存在服务_.是存在节点(*存在节点)) return {};
            结果.push_back(*存在节点);
        }
        排序去重(结果);
        return 结果;
    } catch (...) {
        return {};
    }
}

新场景操作状态 新_场景类::添加状态(
    稳定编码 场景节点,
    稳定编码 状态节点) noexcept {
    try {
        std::lock_guard 锁(新场景互斥);
        if (!是场景节点(场景节点)) return 新场景操作状态::场景不存在;
        const auto 状态信息 = 状态服务_.获取状态(状态节点);
        if (!状态信息) return 新场景操作状态::状态不存在;

        const auto 已有挂接 = 全局基础数据集.查询目标字段(
            状态节点, 状态列表字段);
        if (已有挂接.size() > 1) return 新场景操作状态::结构不一致;
        if (已有挂接.size() == 1) {
            const auto& 字段 = 已有挂接.front();
            if (!是场景节点(字段.所属节点)
                || 字段.字段节点 != 状态列表字段) {
                return 新场景操作状态::结构不一致;
            }
            return 字段.所属节点 == 场景节点
                ? 新场景操作状态::无变化
                : 新场景操作状态::状态与场景不一致;
        }

        bool 发生于本场景 = 状态信息->被描述存在节点 == 场景节点;
        if (!发生于本场景) {
            const auto 所属场景 = 查询所属场景(状态信息->被描述存在节点);
            发生于本场景 = 所属场景 && *所属场景 == 场景节点;
        }
        if (!发生于本场景) return 新场景操作状态::状态与场景不一致;

        for (const auto& 字段 : 全局基础数据集.查询字段(
            场景节点, 状态列表字段)) {
            const auto* 已有状态 = std::get_if<稳定编码>(&字段.内容);
            if (!已有状态 || !状态服务_.是状态节点(*已有状态)) {
                return 新场景操作状态::结构不一致;
            }
            if (*已有状态 == 状态节点) return 新场景操作状态::无变化;
        }
        const auto 新字段 = 全局基础数据集.添加字段节点(
            场景节点, 状态列表字段, 状态节点);
        if (!有效(新字段)) return 新场景操作状态::资源失败;
        const auto 增加引用结果 = 状态服务_.增加引用(状态节点);
        if (增加引用结果 != 新状态操作状态::已完成) {
            (void)全局基础数据集.删除字段(新字段);
            return 增加引用结果 == 新状态操作状态::状态不存在
                ? 新场景操作状态::状态不存在
                : 新场景操作状态::结构不一致;
        }
        std::size_t 命中数 = 0;
        for (const auto& 字段 : 全局基础数据集.查询字段(
            场景节点, 状态列表字段)) {
            const auto* 已有状态 = std::get_if<稳定编码>(&字段.内容);
            if (!已有状态 || !状态服务_.是状态节点(*已有状态)) {
                (void)全局基础数据集.删除字段(新字段);
                (void)状态服务_.减少引用(状态节点);
                return 新场景操作状态::结构不一致;
            }
            if (*已有状态 == 状态节点) ++命中数;
        }
        if (命中数 != 1) {
            (void)全局基础数据集.删除字段(新字段);
            (void)状态服务_.减少引用(状态节点);
            return 新场景操作状态::结构不一致;
        }
        return 新场景操作状态::已完成;
    } catch (...) {
        return 新场景操作状态::资源失败;
    }
}

新场景操作状态 新_场景类::移除状态(
    稳定编码 场景节点,
    稳定编码 状态节点) noexcept {
    try {
        std::lock_guard 锁(新场景互斥);
        if (!是场景节点(场景节点)) return 新场景操作状态::场景不存在;
        for (const auto 动态节点 : 查询动态列表(场景节点)) {
            const auto 动态信息 = 动态服务_.获取动态(动态节点);
            if (!动态信息) return 新场景操作状态::结构不一致;
            if (std::find(动态信息->状态节点组.begin(),
                动态信息->状态节点组.end(), 状态节点)
                != 动态信息->状态节点组.end()) {
                return 新场景操作状态::状态仍被动态引用;
            }
        }
        std::optional<稳定编码> 待删除字段;
        for (const auto& 字段 : 全局基础数据集.查询字段(
            场景节点, 状态列表字段)) {
            const auto* 已有状态 = std::get_if<稳定编码>(&字段.内容);
            if (!已有状态 || !状态服务_.是状态节点(*已有状态)) {
                return 新场景操作状态::结构不一致;
            }
            if (*已有状态 != 状态节点) continue;
            if (待删除字段) return 新场景操作状态::结构不一致;
            待删除字段 = 字段.编码;
        }
        if (!待删除字段) return 新场景操作状态::无变化;
        if (!全局基础数据集.删除字段(*待删除字段)) {
            return 新场景操作状态::资源失败;
        }
        const auto 减少引用结果 = 状态服务_.减少引用(状态节点);
        if (减少引用结果 == 新状态操作状态::已完成) {
            return 新场景操作状态::已完成;
        }
        const auto 恢复字段 = 全局基础数据集.添加字段节点(
            场景节点, 状态列表字段, 状态节点);
        return 有效(恢复字段)
            ? 新场景操作状态::结构不一致
            : 新场景操作状态::资源失败;
    } catch (...) {
        return 新场景操作状态::资源失败;
    }
}

std::vector<稳定编码> 新_场景类::查询状态列表(
    稳定编码 场景节点) const noexcept {
    try {
        std::lock_guard 锁(新场景互斥);
        std::vector<稳定编码> 结果;
        if (!是场景节点(场景节点)) return 结果;
        for (const auto& 字段 : 全局基础数据集.查询字段(
            场景节点, 状态列表字段)) {
            const auto* 状态节点 = std::get_if<稳定编码>(&字段.内容);
            if (!状态节点 || !状态服务_.获取状态(*状态节点)) return {};
            结果.push_back(*状态节点);
        }
        排序去重(结果);
        return 结果;
    } catch (...) {
        return {};
    }
}

新场景操作状态 新_场景类::添加动态(
    稳定编码 场景节点,
    稳定编码 动态节点) noexcept {
    try {
        std::lock_guard 锁(新场景互斥);
        if (!是场景节点(场景节点)) return 新场景操作状态::场景不存在;
        const auto 动态信息 = 动态服务_.获取动态(动态节点);
        if (!动态信息) return 新场景操作状态::动态不存在;

        const auto 已有挂接 = 全局基础数据集.查询目标字段(
            动态节点, 动态列表字段);
        if (已有挂接.size() > 1) return 新场景操作状态::结构不一致;
        if (已有挂接.size() == 1) {
            const auto& 字段 = 已有挂接.front();
            if (!是场景节点(字段.所属节点)
                || 字段.字段节点 != 动态列表字段) {
                return 新场景操作状态::结构不一致;
            }
            return 字段.所属节点 == 场景节点
                ? 新场景操作状态::无变化
                : 新场景操作状态::动态与场景不一致;
        }

        if (动态信息->种类 == 新动态种类::动作) {
            if (!动态信息->动作目标存在节点
                || !存在属于场景范围(
                    场景节点, 动态信息->主题存在节点)
                || !存在属于场景范围(
                    场景节点, *动态信息->动作目标存在节点)) {
                return 新场景操作状态::动态与场景不一致;
            }
        } else {
            const auto 场景状态 = 查询状态列表(场景节点);
            for (const auto 状态节点 : 动态信息->状态节点组) {
                if (std::find(场景状态.begin(), 场景状态.end(), 状态节点)
                    == 场景状态.end()) {
                    return 新场景操作状态::动态与场景不一致;
                }
            }
        }
        for (const auto& 字段 : 全局基础数据集.查询字段(
            场景节点, 动态列表字段)) {
            const auto* 已有动态 = std::get_if<稳定编码>(&字段.内容);
            if (!已有动态 || !动态服务_.是动态节点(*已有动态)) {
                return 新场景操作状态::结构不一致;
            }
            if (*已有动态 == 动态节点) return 新场景操作状态::无变化;
        }

        const auto 新字段 = 全局基础数据集.添加字段节点(
            场景节点, 动态列表字段, 动态节点);
        if (!有效(新字段)) return 新场景操作状态::资源失败;
        const auto 增加引用结果 = 动态服务_.增加引用(动态节点);
        if (增加引用结果 != 新动态操作状态::已完成) {
            (void)全局基础数据集.删除字段(新字段);
            return 增加引用结果 == 新动态操作状态::动态不存在
                ? 新场景操作状态::动态不存在
                : 新场景操作状态::结构不一致;
        }
        std::size_t 命中数 = 0;
        for (const auto& 字段 : 全局基础数据集.查询字段(
            场景节点, 动态列表字段)) {
            const auto* 已有动态 = std::get_if<稳定编码>(&字段.内容);
            if (!已有动态 || !动态服务_.是动态节点(*已有动态)) {
                (void)全局基础数据集.删除字段(新字段);
                (void)动态服务_.减少引用(动态节点);
                return 新场景操作状态::结构不一致;
            }
            if (*已有动态 == 动态节点) ++命中数;
        }
        if (命中数 != 1) {
            (void)全局基础数据集.删除字段(新字段);
            (void)动态服务_.减少引用(动态节点);
            return 新场景操作状态::结构不一致;
        }
        return 新场景操作状态::已完成;
    } catch (...) {
        return 新场景操作状态::资源失败;
    }
}

新场景操作状态 新_场景类::移除动态(
    稳定编码 场景节点,
    稳定编码 动态节点) noexcept {
    try {
        std::lock_guard 锁(新场景互斥);
        if (!是场景节点(场景节点)) return 新场景操作状态::场景不存在;
        std::optional<稳定编码> 待删除字段;
        for (const auto& 字段 : 全局基础数据集.查询字段(
            场景节点, 动态列表字段)) {
            const auto* 已有动态 = std::get_if<稳定编码>(&字段.内容);
            if (!已有动态 || !动态服务_.是动态节点(*已有动态)) {
                return 新场景操作状态::结构不一致;
            }
            if (*已有动态 != 动态节点) continue;
            if (待删除字段) return 新场景操作状态::结构不一致;
            待删除字段 = 字段.编码;
        }
        if (!待删除字段) return 新场景操作状态::无变化;
        if (!全局基础数据集.删除字段(*待删除字段)) {
            return 新场景操作状态::资源失败;
        }
        const auto 减少引用结果 = 动态服务_.减少引用(动态节点);
        if (减少引用结果 == 新动态操作状态::已完成) {
            return 新场景操作状态::已完成;
        }
        const auto 恢复字段 = 全局基础数据集.添加字段节点(
            场景节点, 动态列表字段, 动态节点);
        return 有效(恢复字段)
            ? 新场景操作状态::结构不一致
            : 新场景操作状态::资源失败;
    } catch (...) {
        return 新场景操作状态::资源失败;
    }
}

std::vector<稳定编码> 新_场景类::查询动态列表(
    稳定编码 场景节点) const noexcept {
    try {
        std::lock_guard 锁(新场景互斥);
        std::vector<稳定编码> 结果;
        if (!是场景节点(场景节点)) return 结果;
        for (const auto& 字段 : 全局基础数据集.查询字段(
            场景节点, 动态列表字段)) {
            const auto* 动态节点 = std::get_if<稳定编码>(&字段.内容);
            if (!动态节点 || !动态服务_.获取动态(*动态节点)) return {};
            结果.push_back(*动态节点);
        }
        排序去重(结果);
        return 结果;
    } catch (...) {
        return {};
    }
}

场景二维边界计算结果 新_场景类::计算二维场景边界(
    const 场景二维边界几何读取接口& 几何读取,
    稳定编码 场景节点,
    const 二维毫米坐标& 场景内部参考点) const noexcept {
    场景二维边界计算结果 结果;
    try {
        std::lock_guard 锁(新场景互斥);
        if (!是场景节点(场景节点)) {
            结果.状态 = 场景边界计算状态::场景不存在;
            return 结果;
        }
        const auto 边界存在组 = 查询边界存在(场景节点);
        if (边界存在组.empty()) {
            结果.状态 = 场景边界计算状态::边界存在组为空;
            return 结果;
        }
        结果.边界线段组.reserve(边界存在组.size());
        for (const auto 边界存在节点 : 边界存在组) {
            if (!存在服务_.是存在节点(边界存在节点)) {
                结果.状态 = 场景边界计算状态::边界存在不存在;
                结果.边界线段组.clear();
                return 结果;
            }
            const auto 候选线段组 = 几何读取.读取二维边界候选线段(
                场景节点, 边界存在节点);
            if (候选线段组.empty()) {
                结果.需要获取边界几何的存在组.push_back(边界存在节点);
                continue;
            }
            std::vector<long double> 距离组;
            距离组.reserve(候选线段组.size());
            for (const auto& 候选线段 : 候选线段组) {
                const auto 距离 = 线段到参考点平方距离(
                    候选线段, 场景内部参考点);
                if (!距离) {
                    结果.状态 = 场景边界计算状态::边界几何不合法;
                    结果.边界线段组.clear();
                    return 结果;
                }
                距离组.push_back(*距离);
            }

            const bool 属于当前场景范围 = 存在属于场景范围(
                场景节点, 边界存在节点);
            std::size_t 选择位置 = 0;
            for (std::size_t i = 1; i < 距离组.size(); ++i) {
                const bool 更合适 = 属于当前场景范围
                    ? 距离组[i] > 距离组[选择位置]
                    : 距离组[i] < 距离组[选择位置];
                if (更合适) 选择位置 = i;
            }
            for (std::size_t i = 0; i < 距离组.size(); ++i) {
                if (i != 选择位置
                    && 距离相同(距离组[i], 距离组[选择位置])) {
                    结果.状态 = 场景边界计算状态::边界线段无法唯一确定;
                    结果.边界线段组.clear();
                    return 结果;
                }
            }
            const auto& 选择线段 = 候选线段组[选择位置];
            结果.边界线段组.push_back({
                边界存在节点,
                选择线段.线段序号,
                属于当前场景范围,
                选择线段.起点,
                选择线段.终点});
        }
        if (!结果.需要获取边界几何的存在组.empty()) {
            结果.状态 = 结果.边界线段组.empty()
                ? 场景边界计算状态::边界几何缺失
                : 场景边界计算状态::部分完成;
            return 结果;
        }
        const auto 闭合环组 = 形成二维闭合环(结果.边界线段组);
        if (!闭合环组) {
            结果.状态 = 场景边界计算状态::边界未闭合;
            return 结果;
        }
        结果.闭合环组 = *闭合环组;
        结果.状态 = 场景边界计算状态::已完成;
        return 结果;
    } catch (...) {
        结果.状态 = 场景边界计算状态::资源失败;
        结果.边界线段组.clear();
        结果.闭合环组.clear();
        结果.需要获取边界几何的存在组.clear();
        return 结果;
    }
}

场景三维边界计算结果 新_场景类::计算三维场景边界(
    const 场景三维边界几何读取接口& 几何读取,
    稳定编码 场景节点,
    const 三维毫米坐标& 场景内部参考点) const noexcept {
    场景三维边界计算结果 结果;
    try {
        std::lock_guard 锁(新场景互斥);
        if (!是场景节点(场景节点)) {
            结果.状态 = 场景边界计算状态::场景不存在;
            return 结果;
        }
        const auto 边界存在组 = 查询边界存在(场景节点);
        if (边界存在组.empty()) {
            结果.状态 = 场景边界计算状态::边界存在组为空;
            return 结果;
        }
        结果.边界面组.reserve(边界存在组.size());
        for (const auto 边界存在节点 : 边界存在组) {
            if (!存在服务_.是存在节点(边界存在节点)) {
                结果.状态 = 场景边界计算状态::边界存在不存在;
                结果.边界面组.clear();
                return 结果;
            }
            const auto 候选面组 = 几何读取.读取三维边界候选面(
                场景节点, 边界存在节点);
            if (候选面组.empty()) {
                结果.需要获取边界几何的存在组.push_back(边界存在节点);
                continue;
            }
            std::vector<long double> 距离组;
            距离组.reserve(候选面组.size());
            for (const auto& 候选面 : 候选面组) {
                const auto 距离 = 面到参考点平方距离(
                    候选面, 场景内部参考点);
                if (!距离) {
                    结果.状态 = 场景边界计算状态::边界几何不合法;
                    结果.边界面组.clear();
                    return 结果;
                }
                距离组.push_back(*距离);
            }

            const bool 属于当前场景范围 = 存在属于场景范围(
                场景节点, 边界存在节点);
            std::size_t 选择位置 = 0;
            for (std::size_t i = 1; i < 距离组.size(); ++i) {
                const bool 更合适 = 属于当前场景范围
                    ? 距离组[i] > 距离组[选择位置]
                    : 距离组[i] < 距离组[选择位置];
                if (更合适) 选择位置 = i;
            }
            for (std::size_t i = 0; i < 距离组.size(); ++i) {
                if (i != 选择位置
                    && 距离相同(距离组[i], 距离组[选择位置])) {
                    结果.状态 = 场景边界计算状态::边界面无法唯一确定;
                    结果.边界面组.clear();
                    return 结果;
                }
            }
            const auto& 选择面 = 候选面组[选择位置];
            auto 顶点组 = 选择面.顶点组;
            if (顶点组.size() > 3 && 顶点组.front() == 顶点组.back()) {
                顶点组.pop_back();
            }
            结果.边界面组.push_back({
                边界存在节点,
                选择面.面序号,
                属于当前场景范围,
                std::move(顶点组)});
        }
        if (!结果.需要获取边界几何的存在组.empty()) {
            结果.状态 = 结果.边界面组.empty()
                ? 场景边界计算状态::边界几何缺失
                : 场景边界计算状态::部分完成;
            return 结果;
        }
        const auto 边界线段组 = 形成三维闭合边线(结果.边界面组);
        if (!边界线段组) {
            结果.状态 = 场景边界计算状态::边界未闭合;
            return 结果;
        }
        结果.边界线段组 = *边界线段组;
        结果.状态 = 场景边界计算状态::已完成;
        return 结果;
    } catch (...) {
        结果.状态 = 场景边界计算状态::资源失败;
        结果.边界面组.clear();
        结果.边界线段组.clear();
        结果.需要获取边界几何的存在组.clear();
        return 结果;
    }
}

新场景特征添加结果 新_场景类::添加场景特征(
    稳定编码 场景节点,
    稳定编码 特征概念节点,
    const 新特征准确值& 初始值,
    const 新状态强时间& 强时间) noexcept {
    return 添加场景内存在特征(
        场景节点, 场景节点, 特征概念节点, 初始值, 强时间);
}

新场景特征添加结果 新_场景类::添加场景特征区间值(
    稳定编码 场景节点,
    稳定编码 特征概念节点,
    const 特征概念值域& 区间值) noexcept {
    return 添加场景内存在特征区间值(
        场景节点, 场景节点, 特征概念节点, 区间值);
}

新场景特征值更新结果 新_场景类::更新场景特征值(
    稳定编码 场景节点,
    稳定编码 特征节点,
    const 新特征准确值& 新值,
    const 新状态强时间& 强时间) noexcept {
    return 更新场景内存在特征值(
        场景节点, 场景节点, 特征节点, 新值, 强时间);
}

新场景特征添加结果 新_场景类::添加场景内存在特征(
    稳定编码 场景节点,
    稳定编码 存在节点,
    稳定编码 特征概念节点,
    const 新特征准确值& 初始值,
    const 新状态强时间& 强时间) noexcept {
    新场景特征添加结果 结果;
    try {
        std::lock_guard 锁(新场景互斥);
        if (!是场景节点(场景节点)) {
            结果.状态 = 新场景操作状态::场景不存在;
            return 结果;
        }
        const bool 是场景自身 = 存在节点 == 场景节点;
        const auto 所属场景 = 是场景自身
            ? std::optional<稳定编码>{场景节点}
            : 查询所属场景(存在节点);
        if (!所属场景 || *所属场景 != 场景节点) {
            结果.状态 = 新场景操作状态::存在已属于其它场景;
            return 结果;
        }
        结果.存在结果 = 存在服务_.添加特征(
            存在节点, 特征概念节点, 初始值, 强时间);
        if (!结果.存在结果->状态结果
            || !结果.存在结果->状态结果->状态节点) {
            结果.状态 = 结果.存在结果->特征节点
                ? 新场景操作状态::特征已改变但状态挂接未完成
                : 新场景操作状态::存在建立失败;
            return 结果;
        }
        结果.状态节点 = 结果.存在结果->状态结果->状态节点;
        const auto 挂接状态 = 添加状态(场景节点, *结果.状态节点);
        if (挂接状态 != 新场景操作状态::已完成
            && 挂接状态 != 新场景操作状态::无变化) {
            (void)状态服务_.删除状态(*结果.状态节点);
            结果.状态节点.reset();
            结果.存在结果->状态结果->状态 = 新状态操作状态::状态不存在;
            结果.存在结果->状态结果->状态节点.reset();
            结果.存在结果->状态结果->状态信息.reset();
            结果.状态 = 新场景操作状态::特征已改变但状态挂接未完成;
            return 结果;
        }
        结果.状态 = 新场景操作状态::已完成;
        return 结果;
    } catch (...) {
        结果.状态 = 新场景操作状态::资源失败;
        return 结果;
    }
}

新场景特征添加结果 新_场景类::添加场景内存在特征区间值(
    稳定编码 场景节点,
    稳定编码 存在节点,
    稳定编码 特征概念节点,
    const 特征概念值域& 区间值) noexcept {
    新场景特征添加结果 结果;
    try {
        std::lock_guard 锁(新场景互斥);
        if (!是场景节点(场景节点)) {
            结果.状态 = 新场景操作状态::场景不存在;
            return 结果;
        }
        const bool 是场景自身 = 存在节点 == 场景节点;
        const auto 所属场景 = 是场景自身
            ? std::optional<稳定编码>{场景节点}
            : 查询所属场景(存在节点);
        if (!所属场景 || *所属场景 != 场景节点) {
            结果.状态 = 新场景操作状态::存在已属于其它场景;
            return 结果;
        }
        结果.存在结果 = 存在服务_.添加特征区间值(
            存在节点, 特征概念节点, 区间值);
        if (结果.存在结果->状态 != 新存在操作状态::已完成
            || !结果.存在结果->特征节点) {
            结果.状态 = 新场景操作状态::存在建立失败;
            return 结果;
        }
        结果.状态 = 新场景操作状态::已完成;
        return 结果;
    } catch (...) {
        结果.状态 = 新场景操作状态::资源失败;
        return 结果;
    }
}

新场景特征值更新结果 新_场景类::更新场景内存在特征值(
    稳定编码 场景节点,
    稳定编码 存在节点,
    稳定编码 特征节点,
    const 新特征准确值& 新值,
    const 新状态强时间& 强时间) noexcept {
    新场景特征值更新结果 结果;
    try {
        std::lock_guard 锁(新场景互斥);
        if (!是场景节点(场景节点)) {
            结果.状态 = 新场景操作状态::场景不存在;
            return 结果;
        }
        const bool 是场景自身 = 存在节点 == 场景节点;
        const auto 所属场景 = 是场景自身
            ? std::optional<稳定编码>{场景节点}
            : 查询所属场景(存在节点);
        if (!所属场景 || *所属场景 != 场景节点) {
            结果.状态 = 新场景操作状态::存在已属于其它场景;
            return 结果;
        }
        结果.存在结果 = 存在服务_.更新特征值(
            存在节点, 特征节点, 新值, 强时间);
        if (结果.存在结果->状态 == 新存在操作状态::无变化) {
            结果.状态 = 新场景操作状态::无变化;
            return 结果;
        }
        if (!结果.存在结果->状态结果
            || !结果.存在结果->状态结果->状态节点) {
            结果.状态 = 结果.存在结果->特征结果
                    && 结果.存在结果->特征结果->当前值已改变
                ? 新场景操作状态::特征已改变但状态挂接未完成
                : 新场景操作状态::存在建立失败;
            return 结果;
        }
        结果.状态节点 = 结果.存在结果->状态结果->状态节点;
        const auto 挂接状态 = 添加状态(场景节点, *结果.状态节点);
        if (挂接状态 != 新场景操作状态::已完成
            && 挂接状态 != 新场景操作状态::无变化) {
            (void)状态服务_.删除状态(*结果.状态节点);
            结果.状态节点.reset();
            结果.存在结果->状态结果->状态 = 新状态操作状态::状态不存在;
            结果.存在结果->状态结果->状态节点.reset();
            结果.存在结果->状态结果->状态信息.reset();
            结果.状态 = 新场景操作状态::特征已改变但状态挂接未完成;
            return 结果;
        }
        结果.状态 = 结果.存在结果->状态
                == 新存在操作状态::特征已更新但专属概念未完成
            ? 新场景操作状态::状态已挂接但专属概念未完成
            : 新场景操作状态::已完成;
        return 结果;
    } catch (...) {
        结果.状态 = 新场景操作状态::资源失败;
        return 结果;
    }
}

std::vector<稳定编码> 新_场景类::查询场景全部特征(
    稳定编码 场景节点) const noexcept {
    return 是场景节点(场景节点)
        ? 存在服务_.查询全部特征(场景节点)
        : std::vector<稳定编码>{};
}

场景相对坐标刷新结果 新_场景类::刷新场景存在相对坐标(
    const 场景定位坐标读取接口& 定位读取,
    const 自我场景姿态& 自我姿态,
    std::uint64_t 当前时刻) const noexcept {
    场景相对坐标刷新结果 结果;
    try {
        if (!是场景节点(自我姿态.场景节点)) {
            结果.状态 = 场景相对坐标刷新状态::场景不存在;
            return 结果;
        }
        const auto 场景信息 = 获取场景(自我姿态.场景节点);
        if (!场景信息) {
            结果.状态 = 场景相对坐标刷新状态::结构不一致;
            return 结果;
        }
        结果.状态 = 核验自我姿态(
            自我姿态.场景节点, 自我姿态, 当前时刻);
        if (结果.状态 != 场景相对坐标刷新状态::已完成) return 结果;

        结果.相对坐标组.reserve(场景信息->直接成员存在组.size());
        for (const auto 存在节点 : 场景信息->直接成员存在组) {
            const auto 定位 = 存在节点 == 自我姿态.自我节点
                ? std::optional<场景定位坐标读回>{
                    形成自我定位读回(自我姿态)}
                : 定位读取.读取存在定位坐标(
                    自我姿态.场景节点, 存在节点);
            const auto 形成结果 = 定位
                ? 形成相对坐标项(
                    存在节点, *定位, 自我姿态, 当前时刻)
                : 相对坐标项形成结果{};
            if (形成结果.状态 == 相对坐标项形成状态::坐标超出范围) {
                结果.状态 = 场景相对坐标刷新状态::坐标超出范围;
                结果.相对坐标组.clear();
                结果.需要获取定位坐标的存在组.clear();
                return 结果;
            }
            if (!形成结果.坐标项) {
                结果.需要获取定位坐标的存在组.push_back(存在节点);
                continue;
            }
            结果.相对坐标组.push_back(*形成结果.坐标项);
        }
        结果.状态 = 结果.需要获取定位坐标的存在组.empty()
            ? 场景相对坐标刷新状态::已完成
            : 场景相对坐标刷新状态::部分完成;
        return 结果;
    } catch (...) {
        结果.状态 = 场景相对坐标刷新状态::资源失败;
        结果.相对坐标组.clear();
        结果.需要获取定位坐标的存在组.clear();
        return 结果;
    }
}

场景相对坐标刷新结果 新_场景类::刷新单个存在相对坐标(
    const 场景定位坐标读取接口& 定位读取,
    const 自我场景姿态& 自我姿态,
    稳定编码 存在节点,
    std::uint64_t 当前时刻) const noexcept {
    场景相对坐标刷新结果 结果;
    try {
        if (!是场景节点(自我姿态.场景节点)) {
            结果.状态 = 场景相对坐标刷新状态::场景不存在;
            return 结果;
        }
        const auto 场景信息 = 获取场景(自我姿态.场景节点);
        if (!场景信息) {
            结果.状态 = 场景相对坐标刷新状态::结构不一致;
            return 结果;
        }
        结果.状态 = 核验自我姿态(
            自我姿态.场景节点, 自我姿态, 当前时刻);
        if (结果.状态 != 场景相对坐标刷新状态::已完成) return 结果;
        if (std::find(
                场景信息->直接成员存在组.begin(),
                场景信息->直接成员存在组.end(),
                存在节点)
            == 场景信息->直接成员存在组.end()) {
            结果.状态 = 场景相对坐标刷新状态::存在不属于场景;
            return 结果;
        }

        const auto 定位 = 存在节点 == 自我姿态.自我节点
            ? std::optional<场景定位坐标读回>{
                形成自我定位读回(自我姿态)}
            : 定位读取.读取存在定位坐标(
                自我姿态.场景节点, 存在节点);
        const auto 形成结果 = 定位
            ? 形成相对坐标项(存在节点, *定位, 自我姿态, 当前时刻)
            : 相对坐标项形成结果{};
        if (形成结果.状态 == 相对坐标项形成状态::坐标超出范围) {
            结果.状态 = 场景相对坐标刷新状态::坐标超出范围;
            return 结果;
        }
        if (!形成结果.坐标项) {
            结果.状态 = 场景相对坐标刷新状态::部分完成;
            结果.需要获取定位坐标的存在组.push_back(存在节点);
            return 结果;
        }
        结果.状态 = 场景相对坐标刷新状态::已完成;
        结果.相对坐标组.push_back(*形成结果.坐标项);
        return 结果;
    } catch (...) {
        结果.状态 = 场景相对坐标刷新状态::资源失败;
        结果.相对坐标组.clear();
        结果.需要获取定位坐标的存在组.clear();
        return 结果;
    }
}

} // namespace 海中鱼巣
