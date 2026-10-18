#include "新_场景类.h"
#include "../核心/容错检查.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <map>
#include <mutex>
#include <new>
#include <set>
#include <stdexcept>
#include <utility>

namespace 海中鱼巣 {

struct 新任务治理场景发布准备::内容 final {
    const 新_场景类* 所有者;
    稳定编码 场景, 专属概念;
    新任务虚拟存在引用 目标;
    基础封存追加片段 片段;
    std::vector<基础联合节点快照> 撤销预期;
};
新任务治理场景发布准备::新任务治理场景发布准备(std::shared_ptr<const 内容> 值) noexcept
    : 内容_(std::move(值)) {}
稳定编码 新任务治理场景发布准备::候选场景节点() const noexcept { return 内容_->场景; }
const 基础封存追加片段& 新任务治理场景发布准备::场景片段() const noexcept { return 内容_->片段; }

struct 新方法治理场景发布准备::内容 final {
    const 新_场景类* 所有者;
    稳定编码 场景, 专属概念;
    新方法虚拟存在引用 目标;
    基础封存追加片段 片段;
    std::vector<基础联合节点快照> 撤销预期;
};
新方法治理场景发布准备::新方法治理场景发布准备(std::shared_ptr<const 内容> 值) noexcept
    : 内容_(std::move(值)) {}
稳定编码 新方法治理场景发布准备::候选场景节点() const noexcept { return 内容_->场景; }
const 基础封存追加片段& 新方法治理场景发布准备::场景片段() const noexcept { return 内容_->片段; }

namespace {

std::recursive_mutex 新场景互斥;

稳定编码 场景身份字段{};
稳定编码 场景节点类型{};
稳定编码 成员存在字段{};
稳定编码 边界存在字段{};
稳定编码 状态列表字段{};
稳定编码 动态列表字段{};
稳定编码 根用途字段{}, 治理引用类型字段{}, 治理引用类型{}, 所属治理场景字段{};
稳定编码 治理引用种类字段{}, 治理参与节点字段{}, 治理任务字段{};
稳定编码 治理虚拟存在字段{}, 治理状态特征字段{}, 治理现实来源场景字段{};
稳定编码 治理所属任务字段{}, 治理所属虚拟存在字段{}, 治理所属状态特征字段{};
稳定编码 治理所属方法字段{}, 治理所属方法虚拟存在字段{}, 治理所属方法启用特征字段{};
稳定编码 治理方法字段{}, 治理方法虚拟存在字段{}, 治理方法启用特征字段{};

[[noreturn]] void 治理场景工程失败(const char* 原因) {
    throw std::runtime_error(原因);
}

[[noreturn]] void 报告治理场景工程异常() noexcept {
    try { throw; }
    catch (const std::exception& e) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"场景治理组织",
            L"治理场景引用、用途或状态动态组织发生工程异常；保留事实并终止。", e.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"场景治理组织",
            L"治理场景组织发生未知工程异常；保留事实并终止。");
    }
}

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
        && 有效(动态列表字段) && 有效(根用途字段)
        && 有效(治理引用类型字段) && 有效(治理引用类型) && 有效(所属治理场景字段)
        && 有效(治理引用种类字段) && 有效(治理参与节点字段) && 有效(治理任务字段)
        && 有效(治理虚拟存在字段) && 有效(治理状态特征字段) && 有效(治理现实来源场景字段)
        && 有效(治理所属任务字段) && 有效(治理所属虚拟存在字段) && 有效(治理所属状态特征字段)
        && 有效(治理所属方法字段) && 有效(治理所属方法虚拟存在字段) && 有效(治理所属方法启用特征字段)
        && 有效(治理方法字段) && 有效(治理方法虚拟存在字段) && 有效(治理方法启用特征字段);
}

bool 结构仍存在() noexcept {
    return 结构已初始化()
        && 节点仍存在(场景身份字段)
        && 节点仍存在(场景节点类型)
        && 节点仍存在(成员存在字段)
        && 节点仍存在(边界存在字段)
        && 节点仍存在(状态列表字段)
        && 节点仍存在(动态列表字段) && 节点仍存在(根用途字段)
        && 节点仍存在(治理引用类型字段) && 节点仍存在(治理引用类型)
        && 节点仍存在(所属治理场景字段) && 节点仍存在(治理引用种类字段)
        && 节点仍存在(治理参与节点字段) && 节点仍存在(治理任务字段)
        && 节点仍存在(治理虚拟存在字段) && 节点仍存在(治理状态特征字段)
        && 节点仍存在(治理现实来源场景字段) && 节点仍存在(治理所属任务字段)
        && 节点仍存在(治理所属虚拟存在字段) && 节点仍存在(治理所属状态特征字段)
        && 节点仍存在(治理所属方法字段) && 节点仍存在(治理所属方法虚拟存在字段)
        && 节点仍存在(治理所属方法启用特征字段) && 节点仍存在(治理方法字段)
        && 节点仍存在(治理方法虚拟存在字段) && 节点仍存在(治理方法启用特征字段);
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

void 读取场景写集预期(基础节点组追加请求& 请求, std::vector<稳定编码> 节点组) {
    排序去重(节点组);
    const auto 读取 = 全局基础数据集.查询节点组完整结构(节点组);
    if (读取.状态 != 基础结构操作状态::已完成 || 读取.节点组.size() != 节点组.size())
        治理场景工程失败("scene composition snapshot unavailable");
    for (const auto& 项 : 读取.节点组) {
        if (!项.快照) 治理场景工程失败("scene composition endpoint missing");
        请求.预期联合节点组.push_back(*项.快照);
    }
}

void 核验场景候选未被消费(const std::vector<基础联合节点快照>& 预期) {
    if (预期.empty()) 治理场景工程失败("candidate rollback has no complete snapshot");
    std::vector<稳定编码> 节点组;
    for (const auto& 项 : 预期) 节点组.push_back(项.结构.节点.编码);
    const auto 当前 = 全局基础数据集.查询节点组完整结构(节点组);
    if (当前.状态 != 基础结构操作状态::已完成 || 当前.节点组.size() != 预期.size())
        治理场景工程失败("candidate rollback snapshot unavailable");
    for (const auto& 旧 : 预期) {
        const auto it = std::find_if(当前.节点组.begin(), 当前.节点组.end(),
            [&](const auto& 项) { return 项.查询节点 == 旧.结构.节点.编码; });
        if (it == 当前.节点组.end() || !it->快照 || it->快照->结构.节点 != 旧.结构.节点
            || it->快照->结构.字段组 != 旧.结构.字段组
            || it->快照->入向字段组 != 旧.入向字段组
            || it->快照->入向外部关系组 != 旧.入向外部关系组
            || it->快照->出向外部关系组 != 旧.出向外部关系组)
            治理场景工程失败("candidate was changed or consumed; preserve it before any cleanup");
    }
}

[[noreturn]] void 报告场景范围成员读取工程异常() noexcept {
    try { throw; }
    catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取本树范围成员完整",
            L"场景范围完整读取发生工程异常；未形成完整候选。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取本树范围成员完整",
            L"场景范围完整读取发生未知工程异常；未形成完整候选。");
    }
}

基础联合节点查询结果 读取场景范围单节点(稳定编码 节点) {
    auto 读回 = 全局基础数据集.查询节点组完整结构({节点});
    if (读回.状态 == 基础结构操作状态::已完成
        && (读回.节点组.size() != 1 || 读回.节点组.front().查询节点 != 节点)) {
        throw std::logic_error("scene range L1 singleton result shape mismatch");
    }
    return 读回;
}

std::vector<基础外部关系> 场景层级关系组(
    const std::vector<基础外部关系>& 关系组) {
    std::vector<基础外部关系> 结果;
    for (const auto& 关系 : 关系组) {
        if (关系.类型 == 基础外部关系类型::父子
            && 关系.角色或顺序 == 场景组成子场景关系角色) {
            结果.push_back(关系);
        }
    }
    return 结果;
}

bool 场景范围快照一致(const 基础联合节点快照& 初读,
    const 基础联合节点快照& 末读) {
    return 初读.结构.节点 == 末读.结构.节点
        && 初读.结构.字段组 == 末读.结构.字段组
        && 场景层级关系组(初读.入向外部关系组)
            == 场景层级关系组(末读.入向外部关系组)
        && 场景层级关系组(初读.出向外部关系组)
            == 场景层级关系组(末读.出向外部关系组);
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

新场景树用途 读取根用途(稳定编码 根) {
    const auto 字段 = 全局基础数据集.查询字段(根, 根用途字段);
    if (字段.empty()) return 新场景树用途::普通;
    if (字段.size() != 1) 治理场景工程失败("duplicate scene root purpose");
    const auto* 值 = std::get_if<基础值>(&字段.front().内容);
    const auto* 数值 = 值 ? std::get_if<std::int64_t>(&值->材料) : nullptr;
    if (!数值 || (*数值 != 1 && *数值 != 2)) 治理场景工程失败("invalid scene root purpose");
    return static_cast<新场景树用途>(*数值);
}

// 只解析正式场景组织，不依赖当前视图的根；供跨世界拒绝分类使用。
std::optional<稳定编码> 读取组织根(稳定编码 节点) {
    const auto 标记 = 全局基础数据集.查询字段(节点, 场景身份字段);
    if (标记.empty()) return std::nullopt;
    std::set<稳定编码> 已访问;
    auto 当前 = 节点;
    for (;;) {
        if (!已访问.insert(当前).second) 治理场景工程失败("scene parent cycle");
        if (读取唯一节点字段(当前, 场景身份字段) != 场景节点类型)
            治理场景工程失败("invalid scene identity on root chain");
        const auto 父 = 查询父场景关系(当前);
        if (父.empty()) return 当前;
        if (父.size() != 1 || !全局基础数据集.查询字段(当前, 根用途字段).empty())
            治理场景工程失败("invalid scene root or parent cardinality");
        当前 = 父.front().源节点;
    }
}

struct 治理引用记录 final {
    稳定编码 节点, 场景, 参与节点;
    新治理参与者种类 种类;
    std::optional<新任务虚拟存在引用> 任务;
    std::optional<稳定编码> 来源场景;
    std::optional<新方法虚拟存在引用> 方法;
    friend bool operator==(const 治理引用记录&, const 治理引用记录&) = default;
};

治理引用记录 解析治理引用(const 基础节点结构& 结构) {
    const auto 取字段 = [&](稳定编码 键) -> const 基础字段关系* {
        const 基础字段关系* 结果 = nullptr;
        for (const auto& 字段 : 结构.字段组) if (字段.字段节点 == 键) {
            if (结果) 治理场景工程失败("duplicate governance reference field");
            结果 = &字段;
        }
        return 结果;
    };
    const auto 取引用 = [&](稳定编码 键) {
        const auto* 字段 = 取字段(键);
        const auto* 值 = 字段 ? std::get_if<稳定编码>(&字段->内容) : nullptr;
        if (!值 || !节点仍存在(*值)) 治理场景工程失败("missing governance reference endpoint");
        return *值;
    };
    if (取引用(治理引用类型字段) != 治理引用类型)
        治理场景工程失败("invalid governance reference type");
    治理引用记录 结果{结构.节点.编码, 取引用(所属治理场景字段),
        取引用(治理参与节点字段), 新治理参与者种类::任务虚拟存在, {}, {}};
    const auto* 种类字段 = 取字段(治理引用种类字段);
    const auto* 值 = 种类字段 ? std::get_if<基础值>(&种类字段->内容) : nullptr;
    const auto* 种类 = 值 ? std::get_if<std::int64_t>(&值->材料) : nullptr;
    if (!种类 || (*种类 != 1 && *种类 != 2 && *种类 != 3)) 治理场景工程失败("invalid governance participant kind");
    结果.种类 = static_cast<新治理参与者种类>(*种类);
    if (结果.种类 == 新治理参与者种类::任务虚拟存在) {
        if (结构.字段组.size() != 7 || 取字段(治理现实来源场景字段)
            || 取字段(治理方法字段) || 取字段(治理方法虚拟存在字段) || 取字段(治理方法启用特征字段))
            治理场景工程失败("mixed task and real participant reference");
        结果.任务 = 新任务虚拟存在引用{取引用(治理任务字段),
            取引用(治理虚拟存在字段), 取引用(治理状态特征字段)};
        if (结果.任务->虚拟存在节点 != 结果.参与节点)
            治理场景工程失败("governance task participant identity mismatch");
    } else if (结果.种类 == 新治理参与者种类::方法虚拟存在) {
        if (结构.字段组.size() != 7 || 取字段(治理现实来源场景字段)
            || 取字段(治理任务字段) || 取字段(治理虚拟存在字段) || 取字段(治理状态特征字段))
            治理场景工程失败("mixed method participant reference");
        结果.方法 = 新方法虚拟存在引用{取引用(治理方法字段),
            取引用(治理方法虚拟存在字段), 取引用(治理方法启用特征字段)};
        if (结果.方法->虚拟存在节点 != 结果.参与节点)
            治理场景工程失败("governance method participant identity mismatch");
    } else {
        if (结构.字段组.size() != 5 || 取字段(治理任务字段)
            || 取字段(治理虚拟存在字段) || 取字段(治理状态特征字段)
            || 取字段(治理方法字段) || 取字段(治理方法虚拟存在字段) || 取字段(治理方法启用特征字段))
            治理场景工程失败("mixed real and task participant reference");
        结果.来源场景 = 取引用(治理现实来源场景字段);
    }
    const auto 根 = 读取组织根(结果.场景);
    if (!根 || 读取根用途(*根) != 新场景树用途::内部治理)
        治理场景工程失败("governance reference outside governance tree");
    return 结果;
}

std::vector<治理引用记录> 读取治理引用组(稳定编码 场景) {
    const auto 读回 = 全局基础数据集.查询字段引用节点结构(场景, 所属治理场景字段);
    if (读回.状态 != 基础结构操作状态::已完成)
        治理场景工程失败("governance reference collection read failed");
    std::vector<治理引用记录> 结果;
    std::set<稳定编码> 已有参与者;
    const auto 所属任务 = 全局基础数据集.查询字段(场景, 治理所属任务字段);
    const auto 所属虚拟 = 全局基础数据集.查询字段(场景, 治理所属虚拟存在字段);
    const auto 所属特征 = 全局基础数据集.查询字段(场景, 治理所属状态特征字段);
    const auto 所属方法 = 全局基础数据集.查询字段(场景, 治理所属方法字段);
    const auto 方法虚拟 = 全局基础数据集.查询字段(场景, 治理所属方法虚拟存在字段);
    const auto 方法特征 = 全局基础数据集.查询字段(场景, 治理所属方法启用特征字段);
    const bool 有方法归属 = !所属方法.empty() || !方法虚拟.empty() || !方法特征.empty();
    if (!所属任务.empty() || !所属虚拟.empty() || !所属特征.empty()) {
        if (有方法归属) 治理场景工程失败("scene has mixed task and method ownership");
        const auto 任务 = 读取唯一节点字段(场景, 治理所属任务字段);
        const auto 虚拟 = 读取唯一节点字段(场景, 治理所属虚拟存在字段);
        const auto 特征 = 读取唯一节点字段(场景, 治理所属状态特征字段);
        const auto 根 = 读取组织根(场景);
        if (!任务 || !虚拟 || !特征 || !节点仍存在(*任务) || !节点仍存在(*虚拟)
            || !节点仍存在(*特征) || !根 || *根 == 场景
            || 读取根用途(*根) != 新场景树用途::内部治理)
            治理场景工程失败("task scene ownership fields inconsistent");
        结果.push_back({{}, 场景, *虚拟, 新治理参与者种类::任务虚拟存在,
            新任务虚拟存在引用{*任务, *虚拟, *特征}, {}});
        已有参与者.insert(*虚拟);
    }
    if (有方法归属) {
        const auto 方法 = 读取唯一节点字段(场景, 治理所属方法字段);
        const auto 虚拟 = 读取唯一节点字段(场景, 治理所属方法虚拟存在字段);
        const auto 特征 = 读取唯一节点字段(场景, 治理所属方法启用特征字段);
        const auto 根 = 读取组织根(场景);
        const auto 父 = 查询父场景关系(场景);
        if (!方法 || !虚拟 || !特征 || !节点仍存在(*方法) || !节点仍存在(*虚拟)
            || !节点仍存在(*特征) || !根 || *根 == 场景 || 父.size() != 1
            || 父.front().源节点 != *根 || 读取根用途(*根) != 新场景树用途::内部治理
            || 全局基础数据集.查询目标字段(*方法, 治理所属方法字段).size() != 1)
            治理场景工程失败("method scene ownership fields inconsistent");
        结果.push_back({{}, 场景, *虚拟, 新治理参与者种类::方法虚拟存在,
            {}, {}, 新方法虚拟存在引用{*方法, *虚拟, *特征}});
        已有参与者.insert(*虚拟);
    }
    for (const auto& 结构 : 读回.来源节点组) {
        auto 项 = 解析治理引用(结构);
        if (项.场景 != 场景 || !已有参与者.insert(项.参与节点).second)
            治理场景工程失败("ambiguous governance participant");
        结果.push_back(std::move(项));
    }
    return 结果;
}

void 发布治理引用(const 治理引用记录& 记录) {
    if (static_cast<int>(记录.任务.has_value()) + static_cast<int>(记录.来源场景.has_value())
        + static_cast<int>(记录.方法.has_value()) != 1)
        治理场景工程失败("governance reference branch is not unique");
    基础组内新节点 节点;
    节点.字段组 = {{治理引用类型字段, 治理引用类型}, {所属治理场景字段, 记录.场景},
        {治理参与节点字段, 记录.参与节点},
        {治理引用种类字段, 基础值{static_cast<std::int64_t>(记录.种类)}}};
    if (记录.任务) {
        节点.字段组.push_back({治理任务字段, 记录.任务->任务节点});
        节点.字段组.push_back({治理虚拟存在字段, 记录.任务->虚拟存在节点});
        节点.字段组.push_back({治理状态特征字段, 记录.任务->状态特征节点});
    } else if (记录.方法) {
        节点.字段组.push_back({治理方法字段, 记录.方法->方法节点});
        节点.字段组.push_back({治理方法虚拟存在字段, 记录.方法->虚拟存在节点});
        节点.字段组.push_back({治理方法启用特征字段, 记录.方法->启用特征节点});
    } else if (记录.来源场景) 节点.字段组.push_back({治理现实来源场景字段, *记录.来源场景});
    else 治理场景工程失败("governance reference missing branch");
    const auto 结果 = 全局基础数据集.原子建立节点组({节点});
    if (结果.状态 != 基础结构操作状态::已完成 || 结果.节点组.size() != 1)
        治理场景工程失败("atomic governance reference publication failed");
    const auto 读回 = 全局基础数据集.查询完整节点结构(结果.节点组.front());
    if (读回.状态 != 基础结构操作状态::已完成 || !读回.结构)
        治理场景工程失败("governance reference publication readback failed");
    const auto 已有 = 解析治理引用(*读回.结构);
    if (已有.场景 != 记录.场景 || 已有.参与节点 != 记录.参与节点
        || 已有.种类 != 记录.种类 || 已有.任务 != 记录.任务 || 已有.来源场景 != 记录.来源场景
        || 已有.方法 != 记录.方法)
        治理场景工程失败("governance reference publication mismatch");
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
    新_动态类& 动态服务, 新场景树用途 用途) noexcept
    : 场景树根节点_(场景树根节点),
      用途_(用途),
      存在服务_(存在服务),
      状态服务_(状态服务),
      动态服务_(动态服务) {}

bool 新_场景类::初始化() noexcept {
    try {
        std::lock_guard 锁(新场景互斥);
        if (用途_ != 新场景树用途::普通 && 用途_ != 新场景树用途::内部治理)
            治理场景工程失败("invalid configured scene purpose");
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
            && 建立(动态列表字段) && 建立(根用途字段)
            && 建立(治理引用类型字段) && 建立(治理引用类型) && 建立(所属治理场景字段)
            && 建立(治理引用种类字段) && 建立(治理参与节点字段) && 建立(治理任务字段)
            && 建立(治理虚拟存在字段) && 建立(治理状态特征字段) && 建立(治理现实来源场景字段)
            && 建立(治理所属任务字段) && 建立(治理所属虚拟存在字段) && 建立(治理所属状态特征字段)
            // 独立扩展键；保留全部旧键与已有场景根，不重建旧 schema。
            && 建立(治理所属方法字段) && 建立(治理所属方法虚拟存在字段) && 建立(治理所属方法启用特征字段)
            && 建立(治理方法字段) && 建立(治理方法虚拟存在字段) && 建立(治理方法启用特征字段);
    } catch (...) {
        报告治理场景工程异常();
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
        const auto 用途字段 = 全局基础数据集.添加字段值(节点, 根用途字段,
            基础值{static_cast<std::int64_t>(用途_)});
        if (!有效(用途字段)) 治理场景工程失败("scene root purpose publication failed");
        const auto 存在结果 = 存在服务_.初始化存在节点(
            节点, 新存在建立请求{}, true, std::nullopt);
        if (存在结果.状态 != 新存在操作状态::已完成
            || !存在结果.专属存在概念节点) {
            (void)全局基础数据集.删除字段(用途字段);
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
        报告治理场景工程异常();
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
        if (!全局基础数据集.查询字段(场景节点, 治理所属方法字段).empty()
            && 新父场景节点 != 场景树根节点_)
            return 新场景操作状态::父场景不合法;
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
            || !状态字段组.empty() || !动态字段组.empty()
            || !读取治理引用组(场景节点).empty()) {
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
        报告治理场景工程异常();
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
        if (用途_ == 新场景树用途::内部治理) {
            const auto 子关系 = 查询子场景关系(场景节点);
            if (子关系.size() != 结果.直接子场景组.size())
                治理场景工程失败("governance child scene list is malformed or duplicated");
            for (const auto& 关系 : 子关系) {
                if (!是场景节点(关系.目标节点)
                    || std::find(结果.直接子场景组.begin(), 结果.直接子场景组.end(), 关系.目标节点)
                        == 结果.直接子场景组.end())
                    治理场景工程失败("governance child scene cannot be read completely");
                const auto 子父关系 = 查询父场景关系(关系.目标节点);
                if (子父关系.size() != 1 || 子父关系.front().编码 != 关系.编码)
                    治理场景工程失败("governance child scene has no unique parent");
            }
            // 通用列表读取会去重/返回空；迁移必须辨别完整空表与损坏，不能漏迁。
            const auto 核验唯一列表 = [&](稳定编码 字段, const std::vector<稳定编码>& 节点组) {
                const auto 列表 = 全局基础数据集.查询字段(场景节点, 字段);
                if (列表.size() != 节点组.size()) 治理场景工程失败("governance fact list is malformed or duplicated");
                for (const auto& 项 : 列表) {
                    const auto* 节点 = std::get_if<稳定编码>(&项.内容);
                    if (!节点 || std::find(节点组.begin(), 节点组.end(), *节点) == 节点组.end())
                        治理场景工程失败("governance fact list cannot be read completely");
                    const auto 归属 = 全局基础数据集.查询目标字段(*节点, 字段);
                    if (归属.size() != 1 || 归属.front().编码 != 项.编码)
                        治理场景工程失败("governance fact has multiple scene memberships");
                }
            };
            核验唯一列表(状态列表字段, 结果.状态节点组);
            核验唯一列表(动态列表字段, 结果.动态节点组);
        }
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
        if (读取根用途(场景树根节点_) != 用途_)
            治理场景工程失败("scene view purpose does not match its root");

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
        报告治理场景工程异常();
    }
}

std::optional<新任务治理场景发布准备> 新_场景类::准备任务治理场景(
    稳定编码 父场景, const 新任务虚拟存在引用& 引用) noexcept {
    稳定编码 候选;
    bool 已初始化存在 = false;
    std::vector<基础联合节点快照> 撤销预期;
    try {
        std::lock_guard 锁(新场景互斥);
        if (用途_ != 新场景树用途::内部治理 || 获取根场景() != 父场景
            || 读取任务治理场景(引用.任务节点) || !节点仍存在(引用.任务节点)
            || !节点仍存在(引用.虚拟存在节点) || !节点仍存在(引用.状态特征节点))
            治理场景工程失败("invalid task scene preparation");
        候选 = 全局基础数据集.新建节点();
        if (!有效(候选)) return std::nullopt;
        const auto 存在 = 存在服务_.初始化存在节点(候选, {}, true, std::nullopt);
        if (存在.状态 != 新存在操作状态::已完成 || !存在.专属存在概念节点)
            治理场景工程失败("candidate scene existence preparation failed");
        已初始化存在 = true;
        基础节点组追加请求 清理快照;
        读取场景写集预期(清理快照, {候选, *存在.专属存在概念节点});
        撤销预期 = std::move(清理快照.预期联合节点组);
        基础节点组追加请求 请求;
        请求.追加字段组 = {{候选, 场景身份字段, 场景节点类型},
            {候选, 治理所属任务字段, 引用.任务节点},
            {候选, 治理所属虚拟存在字段, 引用.虚拟存在节点},
            {候选, 治理所属状态特征字段, 引用.状态特征节点}};
        请求.追加外部关系组 = {{父场景, 候选, 基础外部关系类型::父子, 场景组成子场景关系角色}};
        读取场景写集预期(请求, {父场景, 候选, 场景节点类型,
            引用.任务节点, 引用.虚拟存在节点, 引用.状态特征节点});
        const auto 封存 = 全局基础数据集.封存原子追加片段(请求);
        if (封存.状态 != 基础节点组追加状态::已完成 || !封存.片段)
            治理场景工程失败("candidate scene seal failed");
        return 新任务治理场景发布准备{std::make_shared<const 新任务治理场景发布准备::内容>(
            新任务治理场景发布准备::内容{this, 候选, *存在.专属存在概念节点, 引用, *封存.片段, 撤销预期})};
    } catch (...) {
        // 本入口从不发表场景身份；只撤销它自己准备的未挂树存在。
        if (有效(候选)) {
            // 没有取得完整快照时不能推断初始化没有留下内容；先保留现场。
            if (!已初始化存在 || 撤销预期.empty())
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"准备任务治理场景", L"候选初始化状态无法完整还原，保留现场。");
            try { 核验场景候选未被消费(撤销预期); }
            catch (...) { 报告治理场景工程异常(); }
            if (已初始化存在 && 存在服务_.清理存在节点内容(候选) != 新存在操作状态::已完成)
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"准备任务治理场景", L"候选存在清理失败。");
            if (!全局基础数据集.删除节点(候选))
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"准备任务治理场景", L"候选节点清理失败。");
        }
        return std::nullopt;
    }
}

void 新_场景类::撤销未发布任务治理场景(const 新任务治理场景发布准备& 准备) noexcept {
    try {
        std::lock_guard 锁(新场景互斥);
        if (!准备.内容_ || 准备.内容_->所有者 != this) 治理场景工程失败("foreign scene preparation");
        const auto 场景 = 准备.内容_->场景;
        if (!节点仍存在(场景)) return;
        核验场景候选未被消费(准备.内容_->撤销预期);
        if (!全局基础数据集.查询字段(场景, 场景身份字段).empty()
            || !查询父场景关系(场景).empty()
            || !全局基础数据集.查询字段(场景, 治理所属任务字段).empty())
            治理场景工程失败("published scene cannot be rolled back");
        const auto 存在 = 存在服务_.获取存在(场景);
        if (!存在 || 存在->专属存在概念节点 != 准备.内容_->专属概念
            || 存在服务_.清理存在节点内容(场景) != 新存在操作状态::已完成
            || !全局基础数据集.删除节点(场景))
            治理场景工程失败("candidate scene rollback failed");
    } catch (...) { 报告治理场景工程异常(); }
}

std::optional<新任务治理场景信息> 新_场景类::读取任务治理场景(稳定编码 任务) const noexcept {
    try {
        std::lock_guard 锁(新场景互斥);
        if (用途_ != 新场景树用途::内部治理 || !获取根场景())
            治理场景工程失败("task scene reader has no governance root");
        const auto 字段组 = 全局基础数据集.查询目标字段(任务, 治理所属任务字段);
        if (字段组.empty()) return std::nullopt;
        if (字段组.size() != 1) 治理场景工程失败("task has multiple governance scenes");
        const auto 场景 = 字段组.front().所属节点;
        const auto 信息 = 获取场景(场景);
        if (!信息 || 信息->父场景节点 != 场景树根节点_ || 场景 == 场景树根节点_)
            治理场景工程失败("task governance scene parent/identity mismatch");
        const auto 归属任务 = 读取唯一节点字段(场景, 治理所属任务字段);
        const auto 虚拟 = 读取唯一节点字段(场景, 治理所属虚拟存在字段);
        const auto 特征 = 读取唯一节点字段(场景, 治理所属状态特征字段);
        if (归属任务 != 任务 || !虚拟 || !特征 || !节点仍存在(*虚拟) || !节点仍存在(*特征))
            治理场景工程失败("task governance scene ownership incomplete");
        (void)读取治理引用组(场景);
        return 新任务治理场景信息{场景树根节点_, 场景, {任务, *虚拟, *特征}};
    } catch (...) { 报告治理场景工程异常(); }
}

std::optional<新方法治理场景发布准备> 新_场景类::准备方法治理场景(
    稳定编码 父场景, const 新方法虚拟存在引用& 引用) noexcept {
    稳定编码 候选;
    std::vector<基础联合节点快照> 撤销预期;
    try {
        std::lock_guard 锁(新场景互斥);
        if (用途_ != 新场景树用途::内部治理 || 获取根场景() != 父场景
            || 读取方法治理场景(引用.方法节点) || !节点仍存在(引用.方法节点)
            || !节点仍存在(引用.虚拟存在节点) || !节点仍存在(引用.启用特征节点))
            治理场景工程失败("invalid method scene preparation");
        候选 = 全局基础数据集.新建节点();
        if (!有效(候选)) return std::nullopt;
        const auto 存在 = 存在服务_.初始化存在节点(候选, {}, true, std::nullopt);
        if (存在.状态 != 新存在操作状态::已完成 || !存在.专属存在概念节点)
            治理场景工程失败("candidate method scene existence preparation failed");
        基础节点组追加请求 清理快照;
        读取场景写集预期(清理快照, {候选, *存在.专属存在概念节点});
        撤销预期 = std::move(清理快照.预期联合节点组);
        基础节点组追加请求 请求;
        请求.追加字段组 = {{候选, 场景身份字段, 场景节点类型},
            {候选, 治理所属方法字段, 引用.方法节点},
            {候选, 治理所属方法虚拟存在字段, 引用.虚拟存在节点},
            {候选, 治理所属方法启用特征字段, 引用.启用特征节点}};
        请求.追加外部关系组 = {{父场景, 候选, 基础外部关系类型::父子, 场景组成子场景关系角色}};
        读取场景写集预期(请求, {父场景, 候选, 场景节点类型,
            引用.方法节点, 引用.虚拟存在节点, 引用.启用特征节点});
        const auto 封存 = 全局基础数据集.封存原子追加片段(请求);
        if (封存.状态 != 基础节点组追加状态::已完成 || !封存.片段)
            治理场景工程失败("candidate method scene seal failed");
        return 新方法治理场景发布准备{std::make_shared<const 新方法治理场景发布准备::内容>(
            新方法治理场景发布准备::内容{this, 候选, *存在.专属存在概念节点, 引用, *封存.片段, 撤销预期})};
    } catch (...) {
        // 只清理拥有完整快照且从未公开的候选；工程异常不会转为业务空结果。
        if (有效(候选) && !撤销预期.empty()) {
            try {
                核验场景候选未被消费(撤销预期);
                if (存在服务_.清理存在节点内容(候选) != 新存在操作状态::已完成
                    || !全局基础数据集.删除节点(候选))
                    治理场景工程失败("candidate method scene cleanup failed");
            } catch (...) { 报告治理场景工程异常(); }
        }
        报告治理场景工程异常();
    }
}

void 新_场景类::撤销未发布方法治理场景(const 新方法治理场景发布准备& 准备) noexcept {
    try {
        std::lock_guard 锁(新场景互斥);
        if (!准备.内容_ || 准备.内容_->所有者 != this)
            治理场景工程失败("foreign method scene preparation");
        const auto 场景 = 准备.内容_->场景;
        if (!节点仍存在(场景)) return;
        核验场景候选未被消费(准备.内容_->撤销预期);
        if (!全局基础数据集.查询字段(场景, 场景身份字段).empty()
            || !查询父场景关系(场景).empty()
            || !全局基础数据集.查询字段(场景, 治理所属方法字段).empty())
            治理场景工程失败("published method scene cannot be rolled back as candidate");
        const auto 存在 = 存在服务_.获取存在(场景);
        if (!存在 || 存在->专属存在概念节点 != 准备.内容_->专属概念
            || 存在服务_.清理存在节点内容(场景) != 新存在操作状态::已完成
            || !全局基础数据集.删除节点(场景))
            治理场景工程失败("candidate method scene rollback failed");
    } catch (...) { 报告治理场景工程异常(); }
}

std::optional<新方法治理场景信息> 新_场景类::读取方法治理场景(稳定编码 方法) const noexcept {
    try {
        std::lock_guard 锁(新场景互斥);
        if (用途_ != 新场景树用途::内部治理 || !获取根场景())
            治理场景工程失败("method scene reader has no governance root");
        const auto 字段组 = 全局基础数据集.查询目标字段(方法, 治理所属方法字段);
        if (字段组.empty()) return std::nullopt;
        if (字段组.size() != 1) 治理场景工程失败("method has multiple governance scenes");
        const auto 场景 = 字段组.front().所属节点;
        const auto 信息 = 获取场景(场景);
        if (!信息 || 信息->父场景节点 != 场景树根节点_ || 场景 == 场景树根节点_)
            治理场景工程失败("method governance scene parent/identity mismatch");
        const auto 归属方法 = 读取唯一节点字段(场景, 治理所属方法字段);
        const auto 虚拟 = 读取唯一节点字段(场景, 治理所属方法虚拟存在字段);
        const auto 特征字段组 = 全局基础数据集.查询字段(场景, 治理所属方法启用特征字段);
        const auto* 特征 = 特征字段组.size() == 1
            ? std::get_if<稳定编码>(&特征字段组.front().内容) : nullptr;
        if (归属方法 != 方法 || !虚拟 || !特征 || !节点仍存在(方法)
            || !节点仍存在(*虚拟) || !节点仍存在(*特征))
            治理场景工程失败("method governance scene ownership incomplete");
        (void)读取治理引用组(场景);
        return 新方法治理场景信息{场景树根节点_, 场景,
            {方法, *虚拟, *特征}, 特征字段组.front().编码};
    } catch (...) { 报告治理场景工程异常(); }
}

新场景操作状态 新_场景类::添加治理方法引用(稳定编码 场景节点,
    const 新方法虚拟存在引用& 引用, const 新方法治理对象读取接口& 读取器) noexcept {
    try {
        if (用途_ != 新场景树用途::内部治理 || !是场景节点(场景节点))
            return 新场景操作状态::入口拒绝;
        // 不持场景锁进入方法 owner，避免方法发布/回收方向的锁倒置。
        const auto 方法 = 读取器.读取方法虚拟存在引用(引用);
        if (!方法 || 方法->引用 != 引用 || !节点仍存在(方法->专属存在概念节点))
            return 新场景操作状态::入口拒绝;
        std::lock_guard 锁(新场景互斥);
        if (!是场景节点(场景节点)) return 新场景操作状态::场景不存在;
        const auto 归属 = 读取方法治理场景(引用.方法节点);
        if (!归属) return 新场景操作状态::存在不存在;
        if (归属->方法引用 != 引用) 治理场景工程失败("method reference ownership mismatch");
        for (const auto& 已有 : 读取治理引用组(场景节点)) {
            if (已有.参与节点 != 引用.虚拟存在节点) continue;
            if (已有.种类 != 新治理参与者种类::方法虚拟存在 || 已有.方法 != 引用)
                治理场景工程失败("method governance reference identity conflict");
            return 新场景操作状态::无变化;
        }
        发布治理引用({{}, 场景节点, 引用.虚拟存在节点,
            新治理参与者种类::方法虚拟存在, {}, {}, 引用});
        return 新场景操作状态::已完成;
    } catch (...) { 报告治理场景工程异常(); }
}

std::optional<新治理场景参与者> 新_场景类::读取治理参与者(稳定编码 场景节点,
    稳定编码 参与节点, const 新方法治理对象读取接口& 读取器) const noexcept {
    try {
        std::optional<治理引用记录> 记录;
        {
            std::lock_guard 锁(新场景互斥);
            if (用途_ != 新场景树用途::内部治理 || !是场景节点(场景节点)) return {};
            for (const auto& 项 : 读取治理引用组(场景节点))
                if (项.参与节点 == 参与节点) 记录 = 项;
        }
        if (!记录 || !记录->方法) return {};
        const auto 方法 = 读取器.读取方法虚拟存在引用(*记录->方法);
        std::lock_guard 锁(新场景互斥);
        // 方法回收可能在读取器等待方法锁时完成；失效快照不是损坏证据。
        if (!是场景节点(场景节点)) return {};
        bool 一致 = false;
        for (const auto& 项 : 读取治理引用组(场景节点)) if (项 == *记录) 一致 = true;
        if (!一致) return {};
        if (!方法 || 方法->引用 != *记录->方法 || !节点仍存在(方法->专属存在概念节点))
            治理场景工程失败("registered method semantic projection unavailable");
        const auto 归属 = 读取方法治理场景(方法->引用.方法节点);
        if (!归属 || 归属->方法引用 != 方法->引用)
            治理场景工程失败("registered method participant has no complete own scene");
        return 新治理场景参与者{场景节点, 参与节点, 方法->专属存在概念节点,
            新治理参与者种类::方法虚拟存在, {}, {}, 方法->引用};
    } catch (...) { 报告治理场景工程异常(); }
}

新场景操作状态 新_场景类::添加治理状态(稳定编码 场景节点, 稳定编码 状态节点,
    const 新方法治理对象读取接口& 读取器, const 概念_特征类& 概念服务) noexcept {
    try {
        if (用途_ != 新场景树用途::内部治理 || !是场景节点(场景节点))
            return 新场景操作状态::入口拒绝;
        const auto 状态 = 状态服务_.获取状态(状态节点);
        if (!状态) return 新场景操作状态::状态不存在;
        const auto 参与者 = 读取治理参与者(场景节点, 状态->被描述存在节点, 读取器);
        if (!参与者 || !参与者->方法引用) return 新场景操作状态::状态与场景不一致;
        const auto 方法 = 读取器.读取方法虚拟存在引用(*参与者->方法引用);
        const auto* 值 = std::get_if<std::int64_t>(&状态->固定准确值);
        if (!方法 || 方法->引用 != *参与者->方法引用
            || 状态->来源实例特征节点 != 方法->引用.启用特征节点
            || !值 || (*值 != 0 && *值 != 1)
            || 状态->强时间.语义 != 新状态时间语义::实例绝对UTC)
            return 新场景操作状态::状态与场景不一致;
        // S保存形成时的FC，不能拿历史子FC和当前FT根直接比较编码。
        const auto 关系 = 概念服务.比较单特征概念(状态->特征类型概念节点, 状态->特征类型概念节点);
        if (关系.状态 != 特征概念规则状态::已完成)
            治理场景工程失败("method state historical feature concept read failed");
        if (关系.关系 != 单特征概念关系::同一 || 关系.特征类型根 != 方法->启用特征类型概念节点)
            return 新场景操作状态::状态与场景不一致;
        const auto 结果 = 添加已核验状态(场景节点, 状态节点, true, {}, 方法->引用);
        if (结果 == 新场景操作状态::结构不一致 || 结果 == 新场景操作状态::资源失败)
            治理场景工程失败("method governance state attachment failed");
        return 结果;
    } catch (...) { 报告治理场景工程异常(); }
}

新场景操作状态 新_场景类::添加治理动态(稳定编码 场景节点, 稳定编码 动态节点,
    const 新方法治理对象读取接口& 读取器, const 概念_特征类& 概念服务) noexcept {
    try {
        if (用途_ != 新场景树用途::内部治理 || !是场景节点(场景节点))
            return 新场景操作状态::入口拒绝;
        const auto 动态 = 动态服务_.获取动态(动态节点);
        if (!动态) return 新场景操作状态::动态不存在;
        if (动态->种类 != 新动态种类::原子 || 动态->状态节点组.size() != 2)
            return 新场景操作状态::入口拒绝;
        const auto 参与者 = 读取治理参与者(场景节点, 动态->主题存在节点, 读取器);
        if (!参与者 || !参与者->方法引用) return 新场景操作状态::动态与场景不一致;
        const auto 方法 = 读取器.读取方法虚拟存在引用(*参与者->方法引用);
        if (!方法 || 方法->引用 != *参与者->方法引用)
            return 新场景操作状态::动态与场景不一致;
        const auto 前 = 状态服务_.获取状态(动态->状态节点组[0]);
        const auto 后 = 状态服务_.获取状态(动态->状态节点组[1]);
        if (!前 || !后 || 前->固定准确值 == 后->固定准确值
            || 前->强时间.纳秒 >= 后->强时间.纳秒)
            return 新场景操作状态::动态与场景不一致;
        for (const auto* 状态 : {&*前, &*后}) {
            const auto* 值 = std::get_if<std::int64_t>(&状态->固定准确值);
            if (状态->被描述存在节点 != 方法->引用.虚拟存在节点
                || 状态->来源实例特征节点 != 方法->引用.启用特征节点
                || 状态->强时间.语义 != 新状态时间语义::实例绝对UTC
                || !值 || (*值 != 0 && *值 != 1))
                return 新场景操作状态::动态与场景不一致;
            const auto 关系 = 概念服务.比较单特征概念(状态->特征类型概念节点, 状态->特征类型概念节点);
            if (关系.状态 != 特征概念规则状态::已完成)
                治理场景工程失败("method dynamic historical feature concept read failed");
            if (关系.关系 != 单特征概念关系::同一 || 关系.特征类型根 != 方法->启用特征类型概念节点)
                return 新场景操作状态::动态与场景不一致;
        }
        const auto 结果 = 添加已核验动态(场景节点, 动态节点, true, {}, 方法->引用);
        if (结果 == 新场景操作状态::结构不一致 || 结果 == 新场景操作状态::资源失败)
            治理场景工程失败("method governance dynamic attachment failed");
        return 结果;
    } catch (...) { 报告治理场景工程异常(); }
}

void 新_场景类::回收未消费方法治理场景(const 新方法虚拟存在引用& 引用,
    const 新方法治理对象读取接口& 读取器) noexcept {
    try {
        const auto 方法 = 读取器.读取方法虚拟存在引用(引用);
        if (!方法 || 方法->引用 != 引用) 治理场景工程失败("method scene disposal owner unavailable");
        std::lock_guard 锁(新场景互斥);
        const auto 归属 = 读取方法治理场景(引用.方法节点);
        if (!归属 || 归属->方法引用 != 引用) 治理场景工程失败("method scene disposal ownership mismatch");
        const auto 信息 = 获取场景(归属->场景节点);
        const auto 参与者 = 读取治理引用组(归属->场景节点);
        if (!信息 || !信息->直接子场景组.empty() || !信息->直接成员存在组.empty()
            || !信息->边界存在组.empty() || !信息->状态节点组.empty() || !信息->动态节点组.empty()
            || !信息->特征节点组.empty() || 参与者.size() != 1 || 参与者.front().方法 != 引用
            || 有效(参与者.front().节点))
            治理场景工程失败("method scene was consumed and cannot be disposed");
        for (const auto 字段 : {治理方法字段, 治理方法虚拟存在字段, 治理方法启用特征字段}) {
            const auto 目标 = 字段 == 治理方法字段 ? 引用.方法节点
                : 字段 == 治理方法虚拟存在字段 ? 引用.虚拟存在节点 : 引用.启用特征节点;
            if (!全局基础数据集.查询目标字段(目标, 字段).empty())
                治理场景工程失败("method has external scene participants; preserve disposal batch");
        }
        基础节点组追加请求 请求;
        读取场景写集预期(请求, {归属->根场景节点, 归属->场景节点, 信息->专属存在概念节点,
            引用.方法节点, 引用.虚拟存在节点, 引用.启用特征节点});
        const auto 场景快照 = std::find_if(请求.预期联合节点组.begin(), 请求.预期联合节点组.end(),
            [&](const auto& 项) { return 项.结构.节点.编码 == 归属->场景节点; });
        if (场景快照 == 请求.预期联合节点组.end() || 场景快照->入向字段组.size() != 1
            || 场景快照->入向字段组.front().所属节点 != 信息->专属存在概念节点
            || std::get_if<稳定编码>(&场景快照->入向字段组.front().内容) == nullptr
            || std::get<稳定编码>(场景快照->入向字段组.front().内容) != 归属->场景节点
            || 场景快照->入向外部关系组.size() != 1 || !场景快照->出向外部关系组.empty())
            治理场景工程失败("method scene disposal has external references");
        const auto 概念快照 = std::find_if(请求.预期联合节点组.begin(), 请求.预期联合节点组.end(),
            [&](const auto& 项) { return 项.结构.节点.编码 == 信息->专属存在概念节点; });
        if (概念快照 == 请求.预期联合节点组.end() || 概念快照->入向字段组.size() != 1
            || 概念快照->入向字段组.front().所属节点 != 归属->场景节点
            || 概念快照->入向外部关系组.size() != 1 || !概念快照->出向外部关系组.empty())
            治理场景工程失败("method scene dedicated concept has external consumers");
        const auto& 父 = 场景快照->入向外部关系组.front();
        if (父.源节点 != 归属->根场景节点 || 父.目标节点 != 归属->场景节点
            || 父.类型 != 基础外部关系类型::父子 || 父.角色或顺序 != 场景组成子场景关系角色)
            治理场景工程失败("method scene disposal parent mismatch");
        for (const auto 字段 : {治理所属方法字段, 治理所属方法虚拟存在字段, 治理所属方法启用特征字段}) {
            const auto 字段组 = 全局基础数据集.查询字段(归属->场景节点, 字段);
            if (字段组.size() != 1) 治理场景工程失败("method scene disposal ownership changed");
            请求.删除字段关系组.push_back(字段组.front().编码);
        }
        if (全局基础数据集.原子追加节点组(请求).状态 != 基础节点组追加状态::已完成)
            治理场景工程失败("method scene disposal ownership CAS failed");
        // 归属仍在时空场景删除会拒绝；移除归属后失败必须保留现场并终止。
        if (删除空场景(归属->场景节点) != 新场景操作状态::已完成)
            治理场景工程失败("published unconsumed method scene disposal failed");
    } catch (...) { 报告治理场景工程异常(); }
}

std::vector<稳定编码> 新_场景类::查询治理参与节点(稳定编码 场景) const noexcept {
    try {
        std::lock_guard 锁(新场景互斥);
        if (用途_ != 新场景树用途::内部治理 || !是场景节点(场景))
            治理场景工程失败("invalid governance participant scope");
        std::vector<稳定编码> 结果;
        for (const auto& 引用 : 读取治理引用组(场景)) 结果.push_back(引用.参与节点);
        std::sort(结果.begin(), 结果.end());
        return 结果;
    } catch (...) { 报告治理场景工程异常(); }
}

新场景操作状态 新_场景类::添加治理任务引用(稳定编码 场景节点,
    const 新任务虚拟存在引用& 引用, const 新内部治理对象读取接口& 读取器) noexcept {
    try {
        if (用途_ != 新场景树用途::内部治理 || !是场景节点(场景节点))
            return 新场景操作状态::入口拒绝;
        const auto 语义 = 读取器.读取任务虚拟存在语义(引用);
        if (!语义 || 语义->状态.引用 != 引用 || !节点仍存在(语义->专属存在概念节点))
            return 新场景操作状态::入口拒绝;
        std::lock_guard 锁(新场景互斥);
        if (!是场景节点(场景节点)) return 新场景操作状态::场景不存在;
        const auto 所属任务 = 读取唯一节点字段(场景节点, 治理所属任务字段);
        if (所属任务 && *所属任务 != 引用.任务节点)
            治理场景工程失败("task scene rejects a foreign task owner");
        for (const auto& 已有 : 读取治理引用组(场景节点)) {
            if (已有.参与节点 != 引用.虚拟存在节点) continue;
            if (已有.种类 != 新治理参与者种类::任务虚拟存在 || 已有.任务 != 引用)
                治理场景工程失败("task governance reference identity conflict");
            return 新场景操作状态::无变化;
        }
        发布治理引用({{}, 场景节点, 引用.虚拟存在节点,
            新治理参与者种类::任务虚拟存在, 引用, {}});
        return 新场景操作状态::已完成;
    } catch (...) { 报告治理场景工程异常(); }
}

新场景操作状态 新_场景类::原子迁移治理状态动态(稳定编码 原场景, 稳定编码 目标场景,
    const std::vector<稳定编码>& 状态组, const std::vector<稳定编码>& 动态组,
    const 新内部治理对象读取接口& 读取器) noexcept {
    try {
        std::optional<新任务治理场景信息> 归属;
        {
            std::lock_guard 锁(新场景互斥);
            const auto 任务 = 读取唯一节点字段(目标场景, 治理所属任务字段);
            if (!任务 || 获取根场景() != 原场景 || 原场景 == 目标场景)
                治理场景工程失败("invalid governance migration scope");
            归属 = 读取任务治理场景(*任务);
            if (!归属 || 归属->场景节点 != 目标场景) 治理场景工程失败("migration target mismatch");
        }
        // 所有task回调均在场景锁外；后续持锁发布只消费值式核验材料。
        const auto 客体 = 读取治理参与者(目标场景, 归属->任务引用.虚拟存在节点, 读取器);
        if (!客体 || 客体->任务引用 != 归属->任务引用)
            治理场景工程失败("migration task participant unavailable");
        std::map<稳定编码, 新状态信息> 状态材料;
        std::map<稳定编码, 新动态信息> 动态材料;
        for (const auto 节点 : 状态组) {
            const auto 状态 = 状态服务_.获取状态(节点);
            if (!状态 || 状态->被描述存在节点 != 归属->任务引用.虚拟存在节点
                || 状态->来源实例特征节点 != 归属->任务引用.状态特征节点
                || !状态材料.emplace(节点, *状态).second)
                治理场景工程失败("migration state identity mismatch");
        }
        for (const auto 节点 : 动态组) {
            const auto 动态 = 动态服务_.获取动态(节点);
            if (!动态 || !动态材料.emplace(节点, *动态).second)
                治理场景工程失败("migration dynamic missing/duplicate");
            if (动态->种类 == 新动态种类::动作) {
                const auto 主体 = 读取治理参与者(目标场景, 动态->主题存在节点, 读取器);
                if (动态->动作目标种类 != 新动作目标种类::任务虚拟存在
                    || 动态->动作目标任务虚拟存在 != 归属->任务引用 || !主体
                    || 主体->种类 != 新治理参与者种类::现实执行主体引用
                    || 动态->动作结果组.size() != 1
                    || std::find(动态组.begin(), 动态组.end(), 动态->动作结果组.front().结果动态节点) == 动态组.end())
                    治理场景工程失败("migration action closure mismatch");
            } else if (动态->种类 != 新动态种类::原子
                || 动态->主题存在节点 != 归属->任务引用.虚拟存在节点 || 动态->状态节点组.size() != 2
                || !状态材料.contains(动态->状态节点组[0]) || !状态材料.contains(动态->状态节点组[1]))
                治理场景工程失败("migration atomic dynamic closure mismatch");
        }
        if (状态组.empty() || 动态组.empty()) 治理场景工程失败("empty migration package");
        std::lock_guard 锁(新场景互斥);
        if (读取任务治理场景(归属->任务引用.任务节点) != 归属)
            治理场景工程失败("migration ownership changed");
        基础节点组追加请求 请求;
        std::vector<稳定编码> 预期节点{原场景, 目标场景};
        std::optional<bool> 已在目标;
        const auto 收集 = [&](const std::vector<稳定编码>& 节点组, 稳定编码 列表) {
            for (const auto 节点 : 节点组) {
                const auto 挂接 = 全局基础数据集.查询目标字段(节点, 列表);
                if (挂接.size() != 1 || (挂接.front().所属节点 != 原场景 && 挂接.front().所属节点 != 目标场景))
                    治理场景工程失败("migration fact has no unique source owner");
                const bool 在目标 = 挂接.front().所属节点 == 目标场景;
                if (已在目标 && *已在目标 != 在目标) 治理场景工程失败("partly moved migration package");
                已在目标 = 在目标;
                预期节点.push_back(节点);
                if (!在目标) {
                    请求.删除字段关系组.push_back(挂接.front().编码);
                    请求.追加字段组.push_back({目标场景, 列表, 节点});
                }
            }
        };
        收集(状态组, 状态列表字段);
        收集(动态组, 动态列表字段);
        for (const auto& [节点, 信息] : 状态材料)
            if (状态服务_.获取状态(节点) != 信息) 治理场景工程失败("migration state changed before publication");
        for (const auto& [节点, 信息] : 动态材料)
            if (动态服务_.获取动态(节点) != 信息) 治理场景工程失败("migration dynamic changed before publication");
        if (*已在目标) return 新场景操作状态::无变化;
        读取场景写集预期(请求, std::move(预期节点));
        const auto 发布 = 全局基础数据集.原子追加节点组(请求);
        if (发布.状态 != 基础节点组追加状态::已完成)
            治理场景工程失败("atomic governance migration failed; no partial move permitted");
        for (const auto& [节点, 信息] : 状态材料) {
            const auto 挂接 = 全局基础数据集.查询目标字段(节点, 状态列表字段);
            if (挂接.size() != 1 || 挂接.front().所属节点 != 目标场景 || 状态服务_.获取状态(节点) != 信息)
                治理场景工程失败("migration state/count readback mismatch");
        }
        for (const auto& [节点, 信息] : 动态材料) {
            const auto 挂接 = 全局基础数据集.查询目标字段(节点, 动态列表字段);
            if (挂接.size() != 1 || 挂接.front().所属节点 != 目标场景 || 动态服务_.获取动态(节点) != 信息)
                治理场景工程失败("migration dynamic/count readback mismatch");
        }
        return 新场景操作状态::已完成;
    } catch (...) { 报告治理场景工程异常(); }
}

新场景操作状态 新_场景类::添加治理执行主体引用(稳定编码 场景节点,
    稳定编码 执行主体节点, 稳定编码 现实场景节点, const 新_场景类& 现实场景) noexcept {
    try {
        if (用途_ != 新场景树用途::内部治理 || !是场景节点(场景节点)
            || 现实场景.用途_ != 新场景树用途::普通
            || !现实场景.存在属于场景范围(现实场景节点, 执行主体节点)
            || 现实场景.是场景节点(执行主体节点)) return 新场景操作状态::入口拒绝;
        const auto 主体 = 存在服务_.获取存在(执行主体节点);
        if (!主体) return 新场景操作状态::入口拒绝;
        std::lock_guard 锁(新场景互斥);
        if (!是场景节点(场景节点)) return 新场景操作状态::场景不存在;
        for (const auto& 已有 : 读取治理引用组(场景节点)) {
            if (已有.参与节点 != 执行主体节点) continue;
            if (已有.种类 != 新治理参与者种类::现实执行主体引用 || 已有.来源场景 != 现实场景节点)
                治理场景工程失败("real actor governance reference identity conflict");
            return 新场景操作状态::无变化;
        }
        发布治理引用({{}, 场景节点, 执行主体节点,
            新治理参与者种类::现实执行主体引用, {}, 现实场景节点});
        return 新场景操作状态::已完成;
    } catch (...) { 报告治理场景工程异常(); }
}

std::optional<新治理场景参与者> 新_场景类::读取治理参与者(稳定编码 场景节点,
    稳定编码 参与节点, const 新内部治理对象读取接口& 读取器) const noexcept {
    try {
        std::optional<治理引用记录> 记录;
        {
            std::lock_guard 锁(新场景互斥);
            if (用途_ != 新场景树用途::内部治理 || !是场景节点(场景节点)) return {};
            for (const auto& 项 : 读取治理引用组(场景节点))
                if (项.参与节点 == 参与节点) 记录 = 项;
        }
        if (!记录 || 记录->方法) return {};
        稳定编码 概念;
        if (记录->任务) {
            const auto 语义 = 读取器.读取任务虚拟存在语义(*记录->任务);
            if (!语义 || 语义->状态.引用 != *记录->任务 || !节点仍存在(语义->专属存在概念节点))
                治理场景工程失败("registered task semantic projection unavailable");
            概念 = 语义->专属存在概念节点;
        } else {
            const auto 主体 = 存在服务_.获取存在(参与节点);
            if (!主体) 治理场景工程失败("registered real actor unavailable");
            概念 = 主体->专属存在概念节点;
        }
        // 回调之后只重读正式引用，不在场景锁内重入任务owner。
        std::lock_guard 锁(新场景互斥);
        bool 一致 = false;
        for (const auto& 项 : 读取治理引用组(场景节点)) if (项 == *记录) 一致 = true;
        if (!一致) 治理场景工程失败("governance participant changed during read");
        return 新治理场景参与者{场景节点, 参与节点, 概念, 记录->种类,
            记录->任务, 记录->来源场景};
    } catch (...) { 报告治理场景工程异常(); }
}

新场景操作状态 新_场景类::添加治理状态(稳定编码 场景节点, 稳定编码 状态节点,
    const 新内部治理对象读取接口& 读取器) noexcept {
    try {
        if (用途_ != 新场景树用途::内部治理 || !是场景节点(场景节点)) return 新场景操作状态::入口拒绝;
        const auto 状态 = 状态服务_.获取状态(状态节点);
        if (!状态) return 新场景操作状态::状态不存在;
        const auto 参与者 = 读取治理参与者(场景节点, 状态->被描述存在节点, 读取器);
        if (!参与者 || !参与者->任务引用)
            return 新场景操作状态::状态与场景不一致;
        // 回调任务owner在场景锁外；锁内只使用值式身份复核自身组织。
        const auto 特征组 = 读取器.读取任务虚拟存在特征组(*参与者->任务引用);
        if (!特征组 || 特征组->引用 != *参与者->任务引用
            || (状态->来源实例特征节点 != 特征组->状态.特征节点
                && (!特征组->服务值 || 状态->来源实例特征节点 != 特征组->服务值->特征节点)))
            return 新场景操作状态::状态与场景不一致;
        const auto 结果 = 添加已核验状态(场景节点, 状态节点, true,
            新任务虚拟存在特征引用{特征组->引用, 状态->来源实例特征节点});
        if (结果 == 新场景操作状态::结构不一致 || 结果 == 新场景操作状态::资源失败)
            治理场景工程失败("governance state attachment failed");
        return 结果;
    } catch (...) { 报告治理场景工程异常(); }
}

新场景操作状态 新_场景类::添加治理动态(稳定编码 场景节点, 稳定编码 动态节点,
    const 新内部治理对象读取接口& 读取器) noexcept {
    try {
        if (用途_ != 新场景树用途::内部治理 || !是场景节点(场景节点)) return 新场景操作状态::入口拒绝;
        const auto 动态 = 动态服务_.获取动态(动态节点);
        if (!动态) return 新场景操作状态::动态不存在;
        std::optional<新任务虚拟存在特征引用> 治理特征;
        if (动态->种类 == 新动态种类::动作) {
            if (动态->动作目标种类 != 新动作目标种类::任务虚拟存在
                || !动态->动作目标任务虚拟存在 || !动态->动作目标特征节点)
                return 新场景操作状态::动态与场景不一致;
            const auto 主体 = 读取治理参与者(场景节点, 动态->主题存在节点, 读取器);
            const auto 目标 = 读取治理参与者(场景节点, 动态->动作目标任务虚拟存在->虚拟存在节点, 读取器);
            if (!主体 || 主体->种类 != 新治理参与者种类::现实执行主体引用 || !目标
                || 目标->任务引用 != 动态->动作目标任务虚拟存在)
                return 新场景操作状态::动态与场景不一致;
            // 实际目标F可为状态或服务值；任务三字段引用中的stateF保持原义。
            const auto 特征组 = 读取器.读取任务虚拟存在特征组(*目标->任务引用);
            if (!特征组 || 特征组->引用 != *目标->任务引用
                || (*动态->动作目标特征节点 != 特征组->状态.特征节点
                    && (!特征组->服务值 || *动态->动作目标特征节点 != 特征组->服务值->特征节点)))
                return 新场景操作状态::动态与场景不一致;
            治理特征 = 新任务虚拟存在特征引用{特征组->引用, *动态->动作目标特征节点};
        } else {
            if (动态->种类 != 新动态种类::原子) return 新场景操作状态::入口拒绝;
            const auto 目标 = 读取治理参与者(场景节点, 动态->主题存在节点, 读取器);
            if (!目标 || !目标->任务引用) return 新场景操作状态::动态与场景不一致;
            const auto 特征组 = 读取器.读取任务虚拟存在特征组(*目标->任务引用);
            if (!特征组 || 特征组->引用 != *目标->任务引用 || 动态->状态节点组.size() != 2)
                return 新场景操作状态::动态与场景不一致;
            const auto 前 = 状态服务_.获取状态(动态->状态节点组[0]);
            const auto 后 = 状态服务_.获取状态(动态->状态节点组[1]);
            if (!前 || !后 || 前->被描述存在节点 != 特征组->引用.虚拟存在节点
                || 后->被描述存在节点 != 前->被描述存在节点
                || 后->来源实例特征节点 != 前->来源实例特征节点
                || (前->来源实例特征节点 != 特征组->状态.特征节点
                    && (!特征组->服务值 || 前->来源实例特征节点 != 特征组->服务值->特征节点)))
                return 新场景操作状态::动态与场景不一致;
            治理特征 = 新任务虚拟存在特征引用{特征组->引用, 前->来源实例特征节点};
        }
        const auto 结果 = 添加已核验动态(场景节点, 动态节点, true, 治理特征);
        if (结果 == 新场景操作状态::结构不一致 || 结果 == 新场景操作状态::资源失败)
            治理场景工程失败("governance dynamic attachment failed");
        return 结果;
    } catch (...) { 报告治理场景工程异常(); }
}

bool 新_场景类::属于内部治理组织(稳定编码 节点) const noexcept {
    try {
        std::lock_guard 锁(新场景互斥);
        if (!节点仍存在(节点)) return false;
        if (!结构仍存在()) 治理场景工程失败("scene classification schema unavailable");
        const auto 是治理场景 = [&](稳定编码 候选) {
            const auto 根 = 读取组织根(候选);
            return 根 && 读取根用途(*根) == 新场景树用途::内部治理;
        };
        if (是治理场景(节点)) return true;
        for (const auto 字段 : {治理所属虚拟存在字段, 治理所属状态特征字段}) {
            for (const auto& 引用 : 全局基础数据集.查询目标字段(节点, 字段)) {
                bool 已核验 = false;
                for (const auto& 参与者 : 读取治理引用组(引用.所属节点))
                    if (参与者.任务 && !有效(参与者.节点)
                        && (参与者.任务->虚拟存在节点 == 节点 || 参与者.任务->状态特征节点 == 节点)) 已核验 = true;
                if (!已核验)
                    治理场景工程失败("governance owner classification mismatch");
                return true;
            }
        }
        for (const auto 字段 : {治理所属方法虚拟存在字段, 治理所属方法启用特征字段}) {
            for (const auto& 引用 : 全局基础数据集.查询目标字段(节点, 字段)) {
                bool 已核验 = false;
                for (const auto& 参与者 : 读取治理引用组(引用.所属节点))
                    if (参与者.方法 && !有效(参与者.节点)
                        && (参与者.方法->虚拟存在节点 == 节点 || 参与者.方法->启用特征节点 == 节点)) 已核验 = true;
                if (!已核验) 治理场景工程失败("method owner classification mismatch");
                return true;
            }
        }
        for (const auto 字段 : {治理虚拟存在字段, 治理状态特征字段}) {
            const auto 来源 = 全局基础数据集.查询字段引用节点结构(节点, 字段);
            if (来源.状态 != 基础结构操作状态::已完成)
                治理场景工程失败("governance classification reference read failed");
            for (const auto& 结构 : 来源.来源节点组) {
                const auto 引用 = 解析治理引用(结构);
                if (!引用.任务) 治理场景工程失败("invalid governance task classification reference");
                return true;
            }
        }
        for (const auto 字段 : {治理方法虚拟存在字段, 治理方法启用特征字段}) {
            const auto 来源 = 全局基础数据集.查询字段引用节点结构(节点, 字段);
            if (来源.状态 != 基础结构操作状态::已完成)
                治理场景工程失败("method classification reference read failed");
            for (const auto& 结构 : 来源.来源节点组) {
                const auto 引用 = 解析治理引用(结构);
                if (!引用.方法) 治理场景工程失败("invalid governance method classification reference");
                return true;
            }
        }
        for (const auto 列表 : {状态列表字段, 动态列表字段}) {
            const auto 挂接 = 全局基础数据集.查询目标字段(节点, 列表);
            if (挂接.size() > 1) 治理场景工程失败("state or dynamic has multiple scene owners");
            if (!挂接.empty() && 是治理场景(挂接.front().所属节点)) return true;
        }
        // 场景自身的特征仍由存在owner持有；不把共享概念或执行主体归入治理实例。
        for (const auto& 字段 : 全局基础数据集.查询目标字段(场景节点类型, 场景身份字段)) {
            if (!是治理场景(字段.所属节点)) continue;
            const auto 信息 = 存在服务_.获取存在(字段.所属节点);
            if (!信息) 治理场景工程失败("governance scene feature holder unavailable");
            if (std::find(信息->特征节点组.begin(), 信息->特征节点组.end(), 节点) != 信息->特征节点组.end()) return true;
        }
        return false;
    } catch (...) { 报告治理场景工程异常(); }
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
        if (用途_ == 新场景树用途::普通 && 属于内部治理组织(存在节点))
            return 新场景操作状态::入口拒绝;
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

新场景范围成员读取结果 新_场景类::读取本树范围成员完整() const noexcept {
    using 状态 = 新场景范围成员读取状态;
    新场景范围成员读取结果 结果;
    try {
        std::lock_guard 锁(新场景互斥);
        const auto 根 = 场景树根节点_;
        if (!有效(根) || !有效(场景身份字段) || !有效(场景节点类型)
            || !有效(成员存在字段) || !有效(根用途字段)
            || !节点仍存在(场景身份字段) || !节点仍存在(场景节点类型)
            || !节点仍存在(成员存在字段) || !节点仍存在(根用途字段)) {
            return 结果;
        }
        if (用途_ != 新场景树用途::普通 && 用途_ != 新场景树用途::内部治理) {
            结果.状态 = 状态::结构不一致;
            return 结果;
        }

        struct 待访问场景 final {
            稳定编码 节点;
            std::optional<基础外部关系> 预期父关系;
        };
        struct 场景发现项 final {
            基础联合节点快照 首读;
            std::vector<稳定编码> 直接成员;
        };
        struct 成员发现项 final {
            稳定编码 所属场景, 成员字段关系;
            基础联合节点快照 首读;
        };
        const auto 否定复核状态 = [&](稳定编码 节点,
            const 基础联合节点快照& 首读, bool 是场景) {
            const auto 复核 = 读取场景范围单节点(节点);
            if (复核.状态 == 基础结构操作状态::资源失败) return 状态::资源失败;
            if (复核.状态 == 基础结构操作状态::内部不一致)
                return 状态::结构不一致;
            if (复核.状态 != 基础结构操作状态::已完成)
                throw std::logic_error("scene range L1 recheck rejected valid node");
            if (!复核.节点组.front().快照) return 状态::读取已变化;
            const auto& 末读 = *复核.节点组.front().快照;
            if (是场景 ? !场景范围快照一致(首读, 末读)
                : 首读.结构.字段组 != 末读.结构.字段组) {
                return 状态::读取已变化;
            }
            return 状态::结构不一致;
        };

        std::map<稳定编码, 场景发现项> 场景组;
        std::map<稳定编码, 成员发现项> 成员组;
        std::set<稳定编码> 已发现场景{根};
        std::vector<待访问场景> 待访问{{根, std::nullopt}};
        while (!待访问.empty()) {
            const auto 当前 = std::move(待访问.back());
            待访问.pop_back();
            auto 场景读回 = 读取场景范围单节点(当前.节点);
            if (场景读回.状态 == 基础结构操作状态::资源失败) {
                结果.状态 = 状态::资源失败;
                return 结果;
            }
            if (场景读回.状态 == 基础结构操作状态::内部不一致) {
                结果.状态 = 状态::结构不一致;
                return 结果;
            }
            if (场景读回.状态 != 基础结构操作状态::已完成)
                throw std::logic_error("scene range L1 rejected valid scene request");
            if (!场景读回.节点组.front().快照) {
                结果.状态 = 当前.预期父关系 ? 状态::结构不一致 : 状态::根不可读;
                return 结果;
            }
            auto 场景快照 = std::move(*场景读回.节点组.front().快照);
            if (场景快照.结构.节点.编码 != 当前.节点) {
                结果.状态 = 状态::结构不一致;
                return 结果;
            }
            const auto 入向父关系 = 场景层级关系组(场景快照.入向外部关系组);
            if (当前.预期父关系
                ? 入向父关系.size() != 1 || 入向父关系.front() != *当前.预期父关系
                : !入向父关系.empty()) {
                结果.状态 = 状态::结构不一致;
                return 结果;
            }

            std::size_t 身份数 = 0, 用途数 = 0;
            for (const auto& 字段 : 场景快照.结构.字段组) {
                if (!有效(字段.编码) || !有效(字段.字段节点)
                    || 字段.所属节点 != 当前.节点) {
                    结果.状态 = 状态::结构不一致;
                    return 结果;
                }
                if (字段.字段节点 == 场景身份字段) {
                    ++身份数;
                    const auto* 类型 = std::get_if<稳定编码>(&字段.内容);
                    if (!类型 || *类型 != 场景节点类型) {
                        结果.状态 = 状态::结构不一致;
                        return 结果;
                    }
                }
                if (字段.字段节点 == 根用途字段) {
                    ++用途数;
                    const auto* 值 = std::get_if<基础值>(&字段.内容);
                    const auto* 数值 = 值 ? std::get_if<std::int64_t>(&值->材料) : nullptr;
                    if (!数值 || (*数值 != 1 && *数值 != 2)
                        || (当前.节点 == 根
                            && *数值 != static_cast<std::int64_t>(用途_))) {
                        结果.状态 = 状态::结构不一致;
                        return 结果;
                    }
                }
            }
            if (身份数 != 1 || (当前.节点 == 根
                    ? 用途数 > 1 || (用途数 == 0 && 用途_ != 新场景树用途::普通)
                    : 用途数 != 0)) {
                结果.状态 = 状态::结构不一致;
                return 结果;
            }
            if (!存在服务_.是存在节点(当前.节点)) {
                结果.状态 = 否定复核状态(当前.节点, 场景快照, true);
                return 结果;
            }

            std::vector<稳定编码> 直接成员;
            for (const auto& 字段 : 场景快照.结构.字段组) {
                if (字段.字段节点 != 成员存在字段) continue;
                const auto* 成员 = std::get_if<稳定编码>(&字段.内容);
                if (!成员 || !有效(*成员) || 成员组.contains(*成员)) {
                    结果.状态 = 状态::结构不一致;
                    return 结果;
                }
                auto 成员读回 = 读取场景范围单节点(*成员);
                if (成员读回.状态 == 基础结构操作状态::资源失败) {
                    结果.状态 = 状态::资源失败;
                    return 结果;
                }
                if (成员读回.状态 == 基础结构操作状态::内部不一致) {
                    结果.状态 = 状态::结构不一致;
                    return 结果;
                }
                if (成员读回.状态 != 基础结构操作状态::已完成)
                    throw std::logic_error("scene range L1 rejected valid member request");
                if (!成员读回.节点组.front().快照) {
                    结果.状态 = 状态::结构不一致;
                    return 结果;
                }
                auto 成员快照 = std::move(*成员读回.节点组.front().快照);
                if (成员快照.结构.节点.编码 != *成员
                    || std::any_of(成员快照.结构.字段组.begin(),
                        成员快照.结构.字段组.end(), [&](const auto& 当前字段) {
                            return 当前字段.字段节点 == 场景身份字段;
                        })) {
                    结果.状态 = 状态::结构不一致;
                    return 结果;
                }
                if (!存在服务_.是存在节点(*成员)) {
                    结果.状态 = 否定复核状态(*成员, 成员快照, false);
                    return 结果;
                }
                成员组.emplace(*成员, 成员发现项{
                    当前.节点, 字段.编码, std::move(成员快照)});
                直接成员.push_back(*成员);
            }
            std::sort(直接成员.begin(), 直接成员.end());
            const auto 出向子关系 = 场景层级关系组(场景快照.出向外部关系组);
            for (const auto& 关系 : 出向子关系) {
                if (!有效(关系.编码) || 关系.源节点 != 当前.节点
                    || !有效(关系.目标节点)
                    || !已发现场景.insert(关系.目标节点).second) {
                    结果.状态 = 状态::结构不一致;
                    return 结果;
                }
                待访问.push_back({关系.目标节点, 关系});
            }
            场景组.emplace(当前.节点,
                场景发现项{std::move(场景快照), std::move(直接成员)});
        }

        std::vector<稳定编码> 联合节点;
        联合节点.reserve(场景组.size() + 成员组.size());
        for (const auto& [节点, _] : 场景组) 联合节点.push_back(节点);
        for (const auto& [节点, _] : 成员组) 联合节点.push_back(节点);
        std::sort(联合节点.begin(), 联合节点.end());
        if (std::adjacent_find(联合节点.begin(), 联合节点.end()) != 联合节点.end()) {
            结果.状态 = 状态::结构不一致;
            return 结果;
        }
        const auto 联合读回 = 全局基础数据集.查询节点组完整结构(联合节点);
        if (联合读回.状态 == 基础结构操作状态::资源失败) {
            结果.状态 = 状态::资源失败;
            return 结果;
        }
        if (联合读回.状态 == 基础结构操作状态::内部不一致) {
            结果.状态 = 状态::结构不一致;
            return 结果;
        }
        if (联合读回.状态 != 基础结构操作状态::已完成
            || 联合读回.节点组.size() != 联合节点.size()) {
            throw std::logic_error("scene range L1 rejected valid union request");
        }
        for (std::size_t i = 0; i < 联合节点.size(); ++i) {
            const auto& 项 = 联合读回.节点组[i];
            if (项.查询节点 != 联合节点[i])
                throw std::logic_error("scene range L1 union order mismatch");
            if (!项.快照) {
                结果.状态 = 状态::读取已变化;
                return 结果;
            }
            const auto 场景 = 场景组.find(项.查询节点);
            if (场景 != 场景组.end()) {
                if (!场景范围快照一致(场景->second.首读, *项.快照)) {
                    结果.状态 = 状态::读取已变化;
                    return 结果;
                }
                continue;
            }
            const auto 成员 = 成员组.find(项.查询节点);
            if (成员 == 成员组.end())
                throw std::logic_error("scene range union contains unknown node");
            if (std::any_of(项.快照->结构.字段组.begin(),
                    项.快照->结构.字段组.end(), [&](const auto& 字段) {
                        return 字段.字段节点 == 场景身份字段;
                    })) {
                结果.状态 = 状态::读取已变化;
                return 结果;
            }
            std::size_t 归属字段数 = 0;
            bool 归属互证 = false;
            for (const auto& 字段 : 项.快照->入向字段组) {
                if (字段.字段节点 != 成员存在字段) continue;
                ++归属字段数;
                const auto* 目标 = std::get_if<稳定编码>(&字段.内容);
                if (有效(字段.编码) && 字段.编码 == 成员->second.成员字段关系
                    && 字段.所属节点 == 成员->second.所属场景
                    && 目标 && *目标 == 项.查询节点) 归属互证 = true;
            }
            if (归属字段数 != 1 || !归属互证) {
                结果.状态 = 状态::结构不一致;
                return 结果;
            }
            if (!存在服务_.是存在节点(项.查询节点)) {
                结果.状态 = 否定复核状态(项.查询节点, *项.快照, false);
                return 结果;
            }
        }

        新场景范围成员读取结果 完成;
        完成.状态 = 状态::已完成;
        完成.根场景节点 = 根;
        完成.场景成员组.reserve(场景组.size());
        for (const auto& [节点, 场景] : 场景组)
            完成.场景成员组.push_back({节点, 场景.直接成员});
        完成.范围成员存在组.reserve(成员组.size());
        for (const auto& [节点, _] : 成员组)
            完成.范围成员存在组.push_back(节点);
        return 完成;
    } catch (const std::bad_alloc&) {
        结果.状态 = 状态::资源失败;
        return 结果;
    } catch (const std::length_error&) {
        结果.状态 = 状态::资源失败;
        return 结果;
    } catch (...) {
        报告场景范围成员读取工程异常();
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
        if (用途_ == 新场景树用途::普通 && 属于内部治理组织(边界存在节点))
            return 新场景操作状态::入口拒绝;
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
    return 添加已核验状态(场景节点, 状态节点, false);
}

新场景操作状态 新_场景类::添加已核验状态(
    稳定编码 场景节点, 稳定编码 状态节点, bool 治理,
    std::optional<新任务虚拟存在特征引用> 治理特征,
    std::optional<新方法虚拟存在引用> 治理方法) noexcept {
    try {
        std::lock_guard 锁(新场景互斥);
        if (!是场景节点(场景节点)) return 新场景操作状态::场景不存在;
        const auto 状态信息 = 状态服务_.获取状态(状态节点);
        if (!状态信息) return 新场景操作状态::状态不存在;
        if (治理 != (用途_ == 新场景树用途::内部治理)) return 新场景操作状态::入口拒绝;
        if ((治理特征 && 治理方法) || 治理 != (治理特征.has_value() || 治理方法.has_value()))
            return 新场景操作状态::入口拒绝;
        if (治理 && !治理方法 && !全局基础数据集.查询字段(场景节点, 治理所属方法字段).empty())
            return 新场景操作状态::状态与场景不一致;
        bool 发生于本场景 = false;
        if (治理方法) {
            const auto 所属 = 读取方法治理场景(治理方法->方法节点);
            if (!所属 || 所属->场景节点 != 场景节点 || 所属->方法引用 != 治理方法)
                return 新场景操作状态::状态与场景不一致;
            for (const auto& 引用 : 读取治理引用组(场景节点))
                if (引用.方法 == 治理方法 && 引用.参与节点 == 状态信息->被描述存在节点
                    && 治理方法->启用特征节点 == 状态信息->来源实例特征节点) 发生于本场景 = true;
        } else if (治理) {
            const auto 所属 = 读取任务治理场景(治理特征->任务引用.任务节点);
            if (!所属 || 所属->场景节点 != 场景节点 || 所属->任务引用 != 治理特征->任务引用)
                return 新场景操作状态::状态与场景不一致;
            for (const auto& 引用 : 读取治理引用组(场景节点))
                if (引用.任务 == 治理特征->任务引用
                    && 引用.参与节点 == 状态信息->被描述存在节点
                    && 治理特征->特征节点 == 状态信息->来源实例特征节点) 发生于本场景 = true;
        } else {
            if (属于内部治理组织(状态信息->被描述存在节点)
                || 属于内部治理组织(状态信息->来源实例特征节点)
                || 属于内部治理组织(状态节点)) return 新场景操作状态::状态与场景不一致;
            发生于本场景 = 状态信息->被描述存在节点 == 场景节点;
            if (!发生于本场景) {
                const auto 所属场景 = 查询所属场景(状态信息->被描述存在节点);
                发生于本场景 = 所属场景 && *所属场景 == 场景节点;
            }
        }
        if (!发生于本场景) return 新场景操作状态::状态与场景不一致;

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
            if (治理) 治理场景工程失败("published governance state reference increment failed; preserve facts");
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
                if (治理) 治理场景工程失败("published governance state list is malformed; preserve facts");
                (void)全局基础数据集.删除字段(新字段);
                (void)状态服务_.减少引用(状态节点);
                return 新场景操作状态::结构不一致;
            }
            if (*已有状态 == 状态节点) ++命中数;
        }
        if (命中数 != 1) {
            if (治理) 治理场景工程失败("published governance state readback mismatch; preserve facts");
            (void)全局基础数据集.删除字段(新字段);
            (void)状态服务_.减少引用(状态节点);
            return 新场景操作状态::结构不一致;
        }
        return 新场景操作状态::已完成;
    } catch (...) {
        报告治理场景工程异常();
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
    return 添加已核验动态(场景节点, 动态节点, false);
}

新场景操作状态 新_场景类::添加已核验动态(
    稳定编码 场景节点, 稳定编码 动态节点, bool 治理,
    std::optional<新任务虚拟存在特征引用> 治理特征,
    std::optional<新方法虚拟存在引用> 治理方法) noexcept {
    try {
        std::lock_guard 锁(新场景互斥);
        if (!是场景节点(场景节点)) return 新场景操作状态::场景不存在;
        const auto 动态信息 = 动态服务_.获取动态(动态节点);
        if (!动态信息) return 新场景操作状态::动态不存在;
        if (治理 != (用途_ == 新场景树用途::内部治理)) return 新场景操作状态::入口拒绝;
        if ((治理特征 && 治理方法) || 治理 != (治理特征.has_value() || 治理方法.has_value()))
            return 新场景操作状态::入口拒绝;
        if (治理 && !治理方法 && !全局基础数据集.查询字段(场景节点, 治理所属方法字段).empty())
            return 新场景操作状态::动态与场景不一致;
        if (治理方法) {
            const auto 所属 = 读取方法治理场景(治理方法->方法节点);
            if (!所属 || 所属->场景节点 != 场景节点 || 所属->方法引用 != 治理方法)
                return 新场景操作状态::动态与场景不一致;
        }
        // 所属种类必须先于既有挂接早退核验，旧治理动作不得借重放进入普通场景。
        if (动态信息->种类 == 新动态种类::动作) {
            if (治理) {
                if (治理方法 || 动态信息->动作目标种类 != 新动作目标种类::任务虚拟存在
                    || !动态信息->动作目标任务虚拟存在 || 动态信息->动作目标存在节点
                    || !治理特征 || 动态信息->动作目标任务虚拟存在 != 治理特征->任务引用
                    || 动态信息->动作目标特征节点 != 治理特征->特征节点)
                    return 新场景操作状态::动态与场景不一致;
                bool 主体合法 = false, 目标合法 = false;
                for (const auto& 引用 : 读取治理引用组(场景节点)) {
                    if (引用.种类 == 新治理参与者种类::现实执行主体引用
                        && 引用.参与节点 == 动态信息->主题存在节点) 主体合法 = true;
                    if (引用.任务 && 引用.任务 == 动态信息->动作目标任务虚拟存在) 目标合法 = true;
                }
                if (!主体合法 || !目标合法) return 新场景操作状态::动态与场景不一致;
            } else if (动态信息->动作目标种类 != 新动作目标种类::普通存在
                || !动态信息->动作目标存在节点
                || (动态信息->动作目标特征节点 && 属于内部治理组织(*动态信息->动作目标特征节点))
                || !存在属于场景范围(场景节点, *动态信息->动作目标存在节点)
                || !存在属于场景范围(场景节点, 动态信息->主题存在节点)) {
                return 新场景操作状态::动态与场景不一致;
            }
        } else {
            if (!治理 && (属于内部治理组织(动态信息->主题存在节点) || 属于内部治理组织(动态节点)))
                return 新场景操作状态::动态与场景不一致;
            if (治理 && 动态信息->种类 != 新动态种类::原子) return 新场景操作状态::入口拒绝;
            if (治理) {
                const auto 虚拟 = 治理方法 ? 治理方法->虚拟存在节点 : 治理特征->任务引用.虚拟存在节点;
                const auto 特征 = 治理方法 ? 治理方法->启用特征节点 : 治理特征->特征节点;
                if (动态信息->主题存在节点 != 虚拟
                    || 动态信息->状态节点组.size() != 2)
                    return 新场景操作状态::动态与场景不一致;
                bool 目标合法 = false;
                for (const auto& 引用 : 读取治理引用组(场景节点))
                    if (治理方法 ? 引用.方法 == 治理方法 : 引用.任务 == 治理特征->任务引用) 目标合法 = true;
                if (!目标合法) return 新场景操作状态::动态与场景不一致;
                for (const auto s : 动态信息->状态节点组) {
                    const auto 状态 = 状态服务_.获取状态(s);
                    if (!状态 || 状态->被描述存在节点 != 虚拟 || 状态->来源实例特征节点 != 特征)
                        return 新场景操作状态::动态与场景不一致;
                }
            }
            const auto 场景状态 = 查询状态列表(场景节点);
            for (const auto 状态节点 : 动态信息->状态节点组)
                if (std::find(场景状态.begin(), 场景状态.end(), 状态节点) == 场景状态.end())
                    return 新场景操作状态::动态与场景不一致;
        }

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
            if (治理) 治理场景工程失败("published governance dynamic reference increment failed; preserve facts");
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
                if (治理) 治理场景工程失败("published governance dynamic list is malformed; preserve facts");
                (void)全局基础数据集.删除字段(新字段);
                (void)动态服务_.减少引用(动态节点);
                return 新场景操作状态::结构不一致;
            }
            if (*已有动态 == 动态节点) ++命中数;
        }
        if (命中数 != 1) {
            if (治理) 治理场景工程失败("published governance dynamic readback mismatch; preserve facts");
            (void)全局基础数据集.删除字段(新字段);
            (void)动态服务_.减少引用(动态节点);
            return 新场景操作状态::结构不一致;
        }
        return 新场景操作状态::已完成;
    } catch (...) {
        报告治理场景工程异常();
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
