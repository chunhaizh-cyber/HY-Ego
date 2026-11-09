#include "基础数据集.h"
#include "容错检查.h"

#include <algorithm>
#include <limits>
#include <mutex>
#include <set>
#include <new>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <unordered_map>

#ifndef HY_EGO_CORE_INDEX_AUDIT
#define HY_EGO_CORE_INDEX_AUDIT 0
#endif

namespace 海中鱼巣 {

基础数据集 全局基础数据集;

struct 基础封存追加片段::内容 final {
    std::shared_ptr<const int> 数据集身份;
    基础节点组追加请求 请求;
};

struct 基础片段发布回执::内容 final {
    基础封存追加片段 来源;
    std::vector<稳定编码> 新节点, 追加字段, 追加外部关系;
    std::vector<std::vector<稳定编码>> 新节点初始字段;
    内容(const 基础封存追加片段& 片段, const std::vector<基础组内新节点>& 节点组,
        std::size_t 字段数, std::size_t 关系数)
        : 来源(片段), 新节点(节点组.size()), 追加字段(字段数), 追加外部关系(关系数),
          新节点初始字段(节点组.size()) {
        for (std::size_t i = 0; i < 节点组.size(); ++i)
            新节点初始字段[i].resize(节点组[i].字段组.size());
    }
};

基础封存追加片段::基础封存追加片段(std::shared_ptr<const 内容> 值) noexcept
    : 内容_(std::move(值)) {}

基础片段发布回执::基础片段发布回执(std::shared_ptr<const 内容> 值) noexcept
    : 内容_(std::move(值)) {}

bool 基础片段发布回执::对应片段(const 基础封存追加片段& 片段) const noexcept {
    return 内容_ && 片段.内容_ && 内容_->来源.内容_ == 片段.内容_;
}

const std::vector<稳定编码>& 基础片段发布回执::新节点组() const noexcept {
    static const std::vector<稳定编码> 空;
    return 内容_ ? 内容_->新节点 : 空;
}
const std::vector<稳定编码>& 基础片段发布回执::追加字段关系组() const noexcept {
    static const std::vector<稳定编码> 空;
    return 内容_ ? 内容_->追加字段 : 空;
}
const std::vector<稳定编码>& 基础片段发布回执::追加外部关系组() const noexcept {
    static const std::vector<稳定编码> 空;
    return 内容_ ? 内容_->追加外部关系 : 空;
}
const std::vector<std::vector<稳定编码>>& 基础片段发布回执::新节点初始字段关系组() const noexcept {
    static const std::vector<std::vector<稳定编码>> 空;
    return 内容_ ? 内容_->新节点初始字段 : 空;
}

namespace {

bool 有效(基础外部关系类型 类型) noexcept {
    return 类型 == 基础外部关系类型::父子
        || 类型 == 基础外部关系类型::兄弟;
}

bool 联合快照相等(const 基础联合节点快照& 左,
    const 基础联合节点快照& 右) {
    return 左.结构.节点 == 右.结构.节点
        && 左.结构.字段组 == 右.结构.字段组
        && 左.入向外部关系组 == 右.入向外部关系组
        && 左.出向外部关系组 == 右.出向外部关系组
        && 左.入向字段组 == 右.入向字段组;
}

// 容许变化：本体字段相同；原外部关系全部仍在，新增关系另一端不在读取组；原入向字段中来源属于读取组者全部仍在。
bool 联合快照容许新增入向(const 基础联合节点快照& 当前,
    const 基础联合节点快照& 预期, const std::set<稳定编码>& 读取组节点) {
    const auto 按编码 = [](auto 组) {
        std::sort(组.begin(), 组.end(), [](const auto& 左, const auto& 右) { return 左.编码 < 右.编码; });
        return 组;
    };
    if (当前.结构.节点 != 预期.结构.节点 || 按编码(当前.结构.字段组) != 按编码(预期.结构.字段组))
        return false;
    // 外部关系：原关系（含类型、角色或顺序）须全部仍在——删除或改变即变化（③）；新增关系只在另一端不属于
    // 读取组时容许（新成员、新下位，①），读取组内节点之间新增关系视为结构变化。
    const auto 关系仍在 = [&](const std::vector<基础外部关系>& 原组, const std::vector<基础外部关系>& 今组, bool 入向) {
        for (const auto& 原 : 原组)
            if (std::find(今组.begin(), 今组.end(), 原) == 今组.end()) return false;
        for (const auto& 今 : 今组) {
            if (std::find(原组.begin(), 原组.end(), 今) != 原组.end()) continue;
            if (读取组节点.contains(入向 ? 今.源节点 : 今.目标节点)) return false;
        }
        return true;
    };
    if (!关系仍在(预期.入向外部关系组, 当前.入向外部关系组, true)
        || !关系仍在(预期.出向外部关系组, 当前.出向外部关系组, false))
        return false;
    for (const auto& 原 : 预期.入向字段组)
        if (读取组节点.contains(原.所属节点)
            && std::find(当前.入向字段组.begin(), 当前.入向字段组.end(), 原) == 当前.入向字段组.end())
            return false;
    return true;
}

bool 封存字段内容合法(const 基础字段内容& 内容) noexcept {
    if (内容.valueless_by_exception()) return false;
    if (const auto* 引用 = std::get_if<稳定编码>(&内容)) return 有效(*引用);
    return !std::get<基础值>(内容).材料.valueless_by_exception();
}

template<class 项>
bool 按编码规范排序(std::vector<项>& 组) {
    std::sort(组.begin(), 组.end(), [](const auto& 左, const auto& 右) {
        return 左.编码 < 右.编码;
    });
    稳定编码 前{};
    for (const auto& 值 : 组) {
        if (!有效(值.编码) || 值.编码 == 前) return false;
        前 = 值.编码;
    }
    return true;
}

bool 只读预期为空(const 基础只读预期& 预期) noexcept {
    return 预期.节点组.empty() && 预期.字段集合组.empty() && 预期.外部关系集合组.empty();
}

// selector结果是完整集合；相同selector只能同义合并，重叠selector保留AND。
基础节点组追加状态 规范化只读预期(基础只读预期& 预期) {
    using 状态 = 基础节点组追加状态;
    if (只读预期为空(预期)) return 状态::入口拒绝;
    std::sort(预期.节点组.begin(), 预期.节点组.end(), [](const auto& a, const auto& b) { return a.编码 < b.编码; });
    std::set<稳定编码> 身份;
    for (const auto& n : 预期.节点组) {
        if (!有效(n.编码)) return 状态::入口拒绝;
        身份.insert(n.编码);
    }
    预期.节点组.erase(std::unique(预期.节点组.begin(), 预期.节点组.end()), 预期.节点组.end());
    using 字段键 = std::tuple<稳定编码, std::optional<稳定编码>, std::vector<稳定编码>>;
    std::map<字段键, std::vector<基础字段关系>> 字段集合;
    for (auto& s : 预期.字段集合组) {
        if (!身份.contains(s.节点) || (s.字段节点 && (!身份.contains(*s.字段节点) || !s.排除字段键组.empty())))
            return 状态::入口拒绝;
        std::sort(s.排除字段键组.begin(), s.排除字段键组.end());
        s.排除字段键组.erase(std::unique(s.排除字段键组.begin(), s.排除字段键组.end()), s.排除字段键组.end());
        for (const auto key : s.排除字段键组) if (!身份.contains(key)) return 状态::入口拒绝;
        if (!按编码规范排序(s.字段组)) return 状态::入口拒绝;
        for (const auto& f : s.字段组) {
            if (f.所属节点 != s.节点 || !有效(f.字段节点) || !封存字段内容合法(f.内容)
                || (s.字段节点 && f.字段节点 != *s.字段节点)
                || std::binary_search(s.排除字段键组.begin(), s.排除字段键组.end(), f.字段节点))
                return 状态::入口拒绝;
        }
        const auto [it, added] = 字段集合.emplace(字段键{s.节点, s.字段节点, s.排除字段键组}, s.字段组);
        if (!added && it->second != s.字段组) return 状态::预期不符;
    }
    预期.字段集合组.clear();
    for (auto& [key, fields] : 字段集合)
        预期.字段集合组.push_back({std::get<0>(key), std::get<1>(key), std::get<2>(key), std::move(fields)});
    using 关系键 = std::tuple<稳定编码, bool, 基础外部关系类型,
        std::optional<std::int64_t>, std::optional<稳定编码>>;
    std::map<关系键, std::vector<基础外部关系>> 关系集合;
    for (auto& s : 预期.外部关系集合组) {
        if (!身份.contains(s.节点) || !有效(s.类型) || (s.对端节点 && !身份.contains(*s.对端节点))
            || !按编码规范排序(s.关系组)) return 状态::入口拒绝;
        for (const auto& r : s.关系组) {
            if (!有效(r.源节点) || !有效(r.目标节点) || r.类型 != s.类型
                || (s.出向 ? r.源节点 : r.目标节点) != s.节点
                || (s.角色或顺序 && r.角色或顺序 != *s.角色或顺序)
                || (s.对端节点 && (s.出向 ? r.目标节点 : r.源节点) != *s.对端节点))
                return 状态::入口拒绝;
        }
        const auto [it, added] = 关系集合.emplace(关系键{s.节点, s.出向, s.类型, s.角色或顺序, s.对端节点}, s.关系组);
        if (!added && it->second != s.关系组) return 状态::预期不符;
    }
    预期.外部关系集合组.clear();
    for (auto& [key, relations] : 关系集合)
        预期.外部关系集合组.push_back({std::get<0>(key), std::get<1>(key), std::get<2>(key),
            std::get<3>(key), std::get<4>(key), std::move(relations)});
    return 状态::已完成;
}

// 仅核验值式结构，不访问或修改数据集；fresh相等仍由唯一原子提交点核验。
基础节点组追加状态 规范化封存请求(基础节点组追加请求& 请求) {
    using 状态 = 基础节点组追加状态;
    // 无写入的片段只在携带联合预期或只读预期时合法：它是与其它片段（如顶层特征更新）
    // 组合发表的定位保护，单独不产生事实；完全空的请求仍拒绝。
    if (!请求.预期既有节点组.empty()
        || (请求.新节点组.empty() && 请求.追加字段组.empty()
            && 请求.替换字段组.empty() && 请求.追加外部关系组.empty()
            && 请求.删除字段关系组.empty()
            && 请求.预期联合节点组.empty() && 请求.容许新增入向预期组.empty()
            && 只读预期为空(请求.只读预期))) return 状态::入口拒绝;
    // 容许新增入向预期按与联合预期相同的形状规范化（排序、归属、引用目标、关系端点、重复身份）；
    // 同节点多份容许预期保留并逐份核验（AND）。
    for (auto& 快照 : 请求.容许新增入向预期组) {
        const auto 宿主 = 快照.结构.节点.编码;
        if (!有效(宿主) || !按编码规范排序(快照.结构.字段组)
            || !按编码规范排序(快照.入向字段组)
            || !按编码规范排序(快照.入向外部关系组)
            || !按编码规范排序(快照.出向外部关系组)) return 状态::入口拒绝;
        for (const auto& 字段 : 快照.结构.字段组)
            if (字段.所属节点 != 宿主 || !有效(字段.字段节点)
                || !封存字段内容合法(字段.内容)) return 状态::入口拒绝;
        for (const auto& 字段 : 快照.入向字段组) {
            const auto* 目标 = std::get_if<稳定编码>(&字段.内容);
            if (!有效(字段.所属节点) || !有效(字段.字段节点)
                || !目标 || *目标 != 宿主) return 状态::入口拒绝;
        }
        for (const auto& 关系 : 快照.入向外部关系组)
            if (!有效(关系.类型) || !有效(关系.源节点)
                || 关系.目标节点 != 宿主) return 状态::入口拒绝;
        for (const auto& 关系 : 快照.出向外部关系组)
            if (!有效(关系.类型) || !有效(关系.目标节点)
                || 关系.源节点 != 宿主) return 状态::入口拒绝;
    }
    if (!只读预期为空(请求.只读预期)) {
        const auto 状态值 = 规范化只读预期(请求.只读预期);
        if (状态值 != 状态::已完成) return 状态值;
    }
    for (auto& 快照 : 请求.预期联合节点组) {
        const auto 宿主 = 快照.结构.节点.编码;
        if (!有效(宿主) || !按编码规范排序(快照.结构.字段组)
            || !按编码规范排序(快照.入向字段组)
            || !按编码规范排序(快照.入向外部关系组)
            || !按编码规范排序(快照.出向外部关系组)) return 状态::入口拒绝;
        for (const auto& 字段 : 快照.结构.字段组)
            if (字段.所属节点 != 宿主 || !有效(字段.字段节点)
                || !封存字段内容合法(字段.内容)) return 状态::入口拒绝;
        for (const auto& 字段 : 快照.入向字段组) {
            const auto* 目标 = std::get_if<稳定编码>(&字段.内容);
            if (!有效(字段.所属节点) || !有效(字段.字段节点)
                || !目标 || *目标 != 宿主) return 状态::入口拒绝;
        }
        for (const auto& 关系 : 快照.入向外部关系组)
            if (!有效(关系.类型) || !有效(关系.源节点)
                || 关系.目标节点 != 宿主) return 状态::入口拒绝;
        for (const auto& 关系 : 快照.出向外部关系组)
            if (!有效(关系.类型) || !有效(关系.目标节点)
                || 关系.源节点 != 宿主) return 状态::入口拒绝;
    }
    std::sort(请求.预期联合节点组.begin(), 请求.预期联合节点组.end(),
        [](const auto& 左, const auto& 右) { return 左.结构.节点.编码 < 右.结构.节点.编码; });
    for (std::size_t i = 1; i < 请求.预期联合节点组.size(); ++i)
        if (请求.预期联合节点组[i - 1].结构.节点.编码 == 请求.预期联合节点组[i].结构.节点.编码
            && !联合快照相等(请求.预期联合节点组[i - 1], 请求.预期联合节点组[i]))
            return 状态::预期不符;
    请求.预期联合节点组.erase(std::unique(请求.预期联合节点组.begin(), 请求.预期联合节点组.end(),
        [](const auto& 左, const auto& 右) { return 左.结构.节点.编码 == 右.结构.节点.编码; }),
        请求.预期联合节点组.end());
    std::set<稳定编码> 宿主组;
    std::map<稳定编码, const 基础字段关系*> 字段映射;
    for (const auto& 快照 : 请求.预期联合节点组) {
        宿主组.insert(快照.结构.节点.编码);
        for (const auto& 字段 : 快照.结构.字段组)
            if (!字段映射.emplace(字段.编码, &字段).second) return 状态::入口拒绝;
    }
    std::set<稳定编码> 只读引用目标;
    for (const auto& n : 请求.只读预期.节点组) 只读引用目标.insert(n.编码);
    const auto 检查内容 = [&](const 基础组内字段内容& 内容) {
        if (内容.valueless_by_exception()) return false;
        if (const auto* 引用 = std::get_if<稳定编码>(&内容))
            return 有效(*引用) && (宿主组.contains(*引用) || 只读引用目标.contains(*引用));
        if (const auto* 内部 = std::get_if<基础组内节点引用>(&内容))
            return 内部->索引 < 请求.新节点组.size();
        return !std::get<基础值>(内容).材料.valueless_by_exception();
    };
    for (const auto& 节点 : 请求.新节点组)
        for (const auto& 字段 : 节点.字段组)
            if (!有效(字段.字段节点) || !检查内容(字段.内容)) return 状态::入口拒绝;
    for (const auto& 字段 : 请求.追加字段组)
        if (!宿主组.contains(字段.所属节点) || !有效(字段.字段节点)
            || !检查内容(字段.内容)) return 状态::入口拒绝;
    std::set<稳定编码> 已写字段;
    const auto 检查旧字段 = [&](稳定编码 编码) {
        const auto 位置 = 字段映射.find(编码);
        if (!有效(编码) || 位置 == 字段映射.end() || !已写字段.insert(编码).second)
            return false;
        const auto* 引用 = std::get_if<稳定编码>(&位置->second->内容);
        return !引用 || 宿主组.contains(*引用);
    };
    for (const auto 编码 : 请求.删除字段关系组)
        if (!检查旧字段(编码)) return 状态::入口拒绝;
    for (const auto& 替换 : 请求.替换字段组) {
        if (!检查旧字段(替换.字段关系) || !封存字段内容合法(替换.新内容))
            return 状态::入口拒绝;
        if (const auto* 引用 = std::get_if<稳定编码>(&替换.新内容);
            引用 && !宿主组.contains(*引用)) return 状态::入口拒绝;
    }
    const auto 检查端点 = [&](const std::variant<稳定编码, 基础组内节点引用>& 端点) {
        if (端点.valueless_by_exception()) return false;
        if (const auto* 引用 = std::get_if<稳定编码>(&端点))
            return 有效(*引用) && 宿主组.contains(*引用);
        return std::get<基础组内节点引用>(端点).索引 < 请求.新节点组.size();
    };
    for (const auto& 关系 : 请求.追加外部关系组)
        if (!有效(关系.类型) || !检查端点(关系.源节点) || !检查端点(关系.目标节点))
            return 状态::入口拒绝;
    return 状态::已完成;
}

} // namespace

bool 联合快照容许变化判定(const 基础联合节点快照& 当前, const 基础联合节点快照& 预期,
    const std::set<稳定编码>& 读取组节点) {
    return 联合快照容许新增入向(当前, 预期, 读取组节点);
}
std::set<稳定编码> 联合快照组节点(const std::vector<基础联合节点快照>& 组) {
    std::set<稳定编码> 结果;
    for (const auto& 快照 : 组) 结果.insert(快照.结构.节点.编码);
    return 结果;
}

namespace {
// 索引只存正式编码；所有插入先完成分配，失败只撤销本次成功插入的键。
void 加字段索引(const 基础字段关系& 字段, auto& 所属, auto& 反向, auto& 入向, auto& 字段键) {
    bool a=false,b=false,c=false,d=false;
    const auto* 目标=std::get_if<稳定编码>(&字段.内容);
    try {
        a=所属.emplace(字段.所属节点.值,字段.字段节点.值,字段.编码.值).second;
        if(!a)throw std::logic_error("forward field index collision");
        d=字段键.emplace(字段.字段节点.值,字段.编码.值).second;
        if(!d)throw std::logic_error("field-key index collision");
        if(目标) {
            b=反向.emplace(字段.字段节点.值,目标->值,字段.编码.值).second;
            if(!b)throw std::logic_error("reverse field index collision");
            c=入向.emplace(目标->值,字段.编码.值).second;
            if(!c)throw std::logic_error("target-first field index collision");
        }
    } catch(...) {
        if(c)入向.erase({目标->值,字段.编码.值});
        if(b)反向.erase({字段.字段节点.值,目标->值,字段.编码.值});
        if(d)字段键.erase({字段.字段节点.值,字段.编码.值});
        if(a)所属.erase({字段.所属节点.值,字段.字段节点.值,字段.编码.值});
        throw;
    }
}
void 删字段索引(const 基础字段关系& 字段, auto& 所属, auto& 反向, auto& 入向, auto& 字段键) noexcept {
    所属.erase({字段.所属节点.值,字段.字段节点.值,字段.编码.值});
    字段键.erase({字段.字段节点.值,字段.编码.值});
    if(const auto* 目标=std::get_if<稳定编码>(&字段.内容)) {
        反向.erase({字段.字段节点.值,目标->值,字段.编码.值});
        入向.erase({目标->值,字段.编码.值});
    }
}
void 加关系索引(const 基础外部关系& 关系, auto& 源, auto& 目标) {
    const auto 类型=static_cast<std::uint64_t>(关系.类型);
    bool 已加源=false;
    try {
        已加源=源.emplace(关系.源节点.值,类型,关系.编码.值).second;
        if(!已加源)throw std::logic_error("source edge index collision");
        if(!目标.emplace(关系.目标节点.值,类型,关系.编码.值).second)
            throw std::logic_error("target edge index collision");
    } catch(...) {
        if(已加源)源.erase({关系.源节点.值,类型,关系.编码.值});
        throw;
    }
}
void 删关系索引(const 基础外部关系& 关系, auto& 源, auto& 目标) noexcept {
    const auto 类型=static_cast<std::uint64_t>(关系.类型);
    源.erase({关系.源节点.值,类型,关系.编码.值});
    目标.erase({关系.目标节点.值,类型,关系.编码.值});
}
void 收集三元节点索引(const auto& 索引,std::uint64_t 节点,std::set<std::uint64_t>& 输出) {
    for(auto it=索引.lower_bound({节点,0,0});it!=索引.end()&&std::get<0>(*it)==节点;++it)
        输出.insert(std::get<2>(*it));
}
void 收集二元节点索引(const auto& 索引,std::uint64_t 节点,std::set<std::uint64_t>& 输出) {
    for(auto it=索引.lower_bound({节点,0});it!=索引.end()&&it->first==节点;++it)
        输出.insert(it->second);
}
} // namespace

稳定编码 基础数据集::分配编码() noexcept {
    if (下一个编码_ == 0
        || 下一个编码_ == (std::numeric_limits<std::uint64_t>::max)()) {
        return {};
    }
    return {下一个编码_++};
}

bool 基础数据集::节点存在(稳定编码 节点) const noexcept {
    return 有效(节点) && 节点_.contains(节点.值);
}

void 基础数据集::插入字段已加锁(基础字段关系 字段) {
    const auto [位置, 已插入] = 字段关系_.emplace(字段.编码.值, std::move(字段));
    if (!已插入) throw std::logic_error("field identity collision");
    try { 加字段索引(位置->second,所属字段索引_,目标字段索引_,入向字段索引_,字段节点索引_); }
    catch(...) { 字段关系_.erase(位置);throw; }
}
void 基础数据集::插入关系已加锁(基础外部关系 关系) {
    const auto [位置, 已插入] = 外部关系_.emplace(关系.编码.值,关系);
    if (!已插入) throw std::logic_error("edge identity collision");
    try { 加关系索引(关系,源关系索引_,目标关系索引_); }
    catch(...) { 外部关系_.erase(位置);throw; }
}
void 基础数据集::删除关系已加锁(const 基础外部关系& 关系) noexcept {
    删关系索引(关系,源关系索引_,目标关系索引_);
    外部关系_.erase(关系.编码.值);
}
void 基础数据集::核验字段索引已加锁(const 基础字段关系& 字段) const {
#if HY_EGO_CORE_INDEX_AUDIT
    if (!所属字段索引_.contains({字段.所属节点.值,字段.字段节点.值,字段.编码.值})
        || !字段节点索引_.contains({字段.字段节点.值,字段.编码.值}))
        throw std::logic_error("field index missing");
    if (const auto* 引用=std::get_if<稳定编码>(&字段.内容);引用
        && (!目标字段索引_.contains({字段.字段节点.值,引用->值,字段.编码.值})
            || !入向字段索引_.contains({引用->值,字段.编码.值})))
        throw std::logic_error("reverse field index missing");
#else
    (void)字段;
#endif
}
void 基础数据集::删除字段已加锁(const 基础字段关系& 字段) noexcept {
    删字段索引(字段,所属字段索引_,目标字段索引_,入向字段索引_,字段节点索引_);
    字段关系_.erase(字段.编码.值);
}
void 基础数据集::读取节点关系已加锁(稳定编码 节点,bool 出向,
    std::optional<基础外部关系类型> 类型,std::vector<基础外部关系>& 结果) const {
    const auto& 索引=出向 ? 源关系索引_ : 目标关系索引_;
    const auto 类型值=类型 ? static_cast<std::uint64_t>(*类型) : 0;
    for(auto it=索引.lower_bound({节点.值,类型值,0});
        it!=索引.end()&&std::get<0>(*it)==节点.值&&(!类型||std::get<1>(*it)==类型值);++it)
        结果.push_back(外部关系_.at(std::get<2>(*it)));
    // 跨关系类型仍维持正式编码顺序，自环在入出两组分别保留。
    if(!类型)std::sort(结果.begin(),结果.end(),[](const auto& a,const auto& b){return a.编码.值<b.编码.值;});
}

void 基础数据集::读取节点字段已加锁(
    稳定编码 节点, std::vector<基础字段关系>& 结果) const {
    for (auto 位置 = 所属字段索引_.lower_bound({节点.值, 0, 0});
        位置 != 所属字段索引_.end() && std::get<0>(*位置) == 节点.值; ++位置)
        结果.push_back(字段关系_.at(std::get<2>(*位置)));
    // 跨字段类型的索引顺序不是旧表的编码顺序；对外仍保持原顺序。
    std::sort(结果.begin(), 结果.end(), [](const auto& 左, const auto& 右) {
        return 左.编码.值 < 右.编码.值;
    });
}

基础联合节点快照 基础数据集::读取联合快照已加锁(稳定编码 节点) const {
    基础联合节点快照 结果;
    结果.结构.节点 = 节点_.at(节点.值);
    读取节点字段已加锁(节点,结果.结构.字段组);
    读取节点关系已加锁(节点,false,std::nullopt,结果.入向外部关系组);
    读取节点关系已加锁(节点,true,std::nullopt,结果.出向外部关系组);
    for(auto it=入向字段索引_.lower_bound({节点.值,0});it!=入向字段索引_.end()&&it->first==节点.值;++it)
        结果.入向字段组.push_back(字段关系_.at(it->second));
    return 结果;
}

基础互引节点操作状态 基础数据集::复核联合预期已加锁(
    const std::vector<基础联合节点快照>& 预期,
    const std::set<std::uint64_t>& 必需) const {
    using 状态 = 基础互引节点操作状态;
    std::set<std::uint64_t> 已提供;
    for (const auto& 项 : 预期) {
        const auto 编码 = 项.结构.节点.编码;
        if (!有效(编码) || !已提供.insert(编码.值).second) return 状态::入口拒绝;
    }
    if (!std::includes(已提供.begin(), 已提供.end(), 必需.begin(), 必需.end()))
        return 状态::入口拒绝;
    for (const auto& 项 : 预期) {
        const auto 编码 = 项.结构.节点.编码;
        if (!节点存在(编码)) return 状态::节点不存在;
        if (!联合快照相等(读取联合快照已加锁(编码), 项))
            return 状态::预期不符;
    }
    return 状态::已完成;
}

bool 基础数据集::节点被引用(稳定编码 节点) const noexcept {
    const auto 关联=[&](const auto& 索引){const auto it=索引.lower_bound({节点.值,0,0});return it!=索引.end()&&std::get<0>(*it)==节点.值;};
    const auto 二元关联=[&](const auto& 索引){const auto it=索引.lower_bound({节点.值,0});return it!=索引.end()&&it->first==节点.值;};
    return 关联(源关系索引_)||关联(目标关系索引_)||关联(所属字段索引_)
        ||二元关联(入向字段索引_)||二元关联(字段节点索引_);
}

bool 基础数据集::有入向字段引用(稳定编码 节点) const noexcept {
    return 有入向字段引用(节点, {});
}

bool 基础数据集::有入向字段引用(稳定编码 节点, 稳定编码 忽略所属节点) const noexcept {
    try {
        std::shared_lock 锁(互斥_);
        if (!节点存在(节点)) throw std::logic_error("reference target missing");
        for(auto it=入向字段索引_.lower_bound({节点.值,0});it!=入向字段索引_.end()&&it->first==节点.值;++it) {
            const auto& 字段=字段关系_.at(it->second);
            if(字段.所属节点!=节点&&字段.所属节点!=忽略所属节点)return true;
        }
        return false;
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取入向字段引用",
            L"引用检查失败，禁止解释为无引用。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取入向字段引用", L"未知工程异常。");
    }
}

基础结构操作状态 基础数据集::检查节点结构引用闭包已加锁(
    const std::vector<稳定编码>& 节点组,
    const std::vector<稳定编码>& 自有字段关系组,
    const std::vector<稳定编码>& 结构外部关系组,
    const std::vector<稳定编码>& 允许外部入向字段关系组,
    bool 拒绝全部字段键消费) const {
    using 状态 = 基础结构操作状态;
    if (节点组.empty()) return 状态::入口拒绝;
    const auto 合法集合 = [](const auto& 组, auto& 集合) {
        for (const auto 编码 : 组)
            if (!有效(编码) || !集合.insert(编码.值).second) return false;
        return true;
    };
    std::set<std::uint64_t> 节点集, 字段集, 关系集, 允许字段集;
    if (!合法集合(节点组, 节点集) || !合法集合(自有字段关系组, 字段集)
        || !合法集合(结构外部关系组, 关系集)
        || !合法集合(允许外部入向字段关系组, 允许字段集)) return 状态::入口拒绝;
    for (const auto 编码 : 节点集)
        if (!节点_.contains(编码)) return 状态::节点不存在;
    for (const auto 编码 : 字段集) {
        const auto 位置 = 字段关系_.find(编码);
        if (位置 == 字段关系_.end()) return 状态::引用不存在;
        if (!节点集.contains(位置->second.所属节点.值)) return 状态::入口拒绝;
#if HY_EGO_CORE_INDEX_AUDIT
        核验字段索引已加锁(位置->second);
#endif
    }
    for (const auto 编码 : 关系集) {
        const auto 位置 = 外部关系_.find(编码);
        if (位置 == 外部关系_.end()) return 状态::引用不存在;
        if (!节点集.contains(位置->second.源节点.值)
            && !节点集.contains(位置->second.目标节点.值)) return 状态::入口拒绝;
    }
    for (const auto 编码 : 允许字段集) {
        if (字段集.contains(编码)) return 状态::入口拒绝;
        const auto 位置 = 字段关系_.find(编码);
        if (位置 == 字段关系_.end()) return 状态::引用不存在;
        const auto& 字段 = 位置->second;
        const auto* 目标 = std::get_if<稳定编码>(&字段.内容);
        if (节点集.contains(字段.所属节点.值) || !目标
            || !节点集.contains(目标->值)) return 状态::入口拒绝;
#if HY_EGO_CORE_INDEX_AUDIT
        核验字段索引已加锁(字段);
#endif
    }
    std::set<std::uint64_t> 相关字段,相关关系;
    for(const auto 节点:节点集) {
        收集三元节点索引(所属字段索引_,节点,相关字段);
        收集二元节点索引(字段节点索引_,节点,相关字段);
        收集二元节点索引(入向字段索引_,节点,相关字段);
        收集三元节点索引(源关系索引_,节点,相关关系);
        收集三元节点索引(目标关系索引_,节点,相关关系);
    }
    for (const auto 编码 : 相关字段) {
        const auto& 字段=字段关系_.at(编码);
        // 新只读入口连自有字段的字段键消费也拒绝；旧删除保留完整集内随同删除语义。
        if (拒绝全部字段键消费 && 节点集.contains(字段.字段节点.值))
            return 状态::引用仍存在;
        if (字段集.contains(编码)) continue;
        if (节点集.contains(字段.所属节点.值)) return 状态::内部不一致;
        if (节点集.contains(字段.字段节点.值)) return 状态::引用仍存在;
        const auto* 目标 = std::get_if<稳定编码>(&字段.内容);
        if (目标 && 节点集.contains(目标->值) && !允许字段集.contains(编码))
            return 状态::引用仍存在;
    }
    for (const auto 编码 : 相关关系) {
        const auto& 关系=外部关系_.at(编码);
        if (!关系集.contains(编码) && 节点集.contains(关系.源节点.值)
            && 节点集.contains(关系.目标节点.值)) return 状态::内部不一致;
        if (!关系集.contains(编码) && (节点集.contains(关系.源节点.值)
            || 节点集.contains(关系.目标节点.值))) return 状态::引用仍存在;
    }
    return 状态::已完成;
}

基础结构操作状态 基础数据集::检查节点结构引用闭包(
    const std::vector<稳定编码>& 节点组,
    const std::vector<稳定编码>& 自有字段关系组,
    const std::vector<稳定编码>& 结构外部关系组,
    const std::vector<稳定编码>& 允许外部入向字段关系组) const noexcept {
    try {
        std::shared_lock 锁(互斥_);
        return 检查节点结构引用闭包已加锁(节点组, 自有字段关系组,
            结构外部关系组, 允许外部入向字段关系组, true);
    } catch (const std::bad_alloc&) { return 基础结构操作状态::资源失败;
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"检查节点结构引用闭包",
            L"只读引用闭包或字段索引核验异常，未形成业务结果。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"检查节点结构引用闭包",
            L"只读引用闭包核验发生未知工程异常。");
    }
}

基础结构操作状态 基础数据集::原子删除节点结构(
    const std::vector<稳定编码>& 节点组,
    const std::vector<稳定编码>& 字段关系组,
    const std::vector<稳定编码>& 外部关系组) noexcept {
    using 状态 = 基础结构操作状态;
    try {
        std::unique_lock 锁(互斥_);
        const auto 核验 = 检查节点结构引用闭包已加锁(
            节点组, 字段关系组, 外部关系组, {}, false);
        if (核验 != 状态::已完成) return 核验;
        // 此后只在已核验的标准map/set中删除，不再分配；无外部调用。
        for (const auto 编码 : 字段关系组) 删除字段已加锁(字段关系_.at(编码.值));
        for (const auto 编码 : 外部关系组) 删除关系已加锁(外部关系_.at(编码.值));
        for (const auto 编码 : 节点组) 节点_.erase(节点_.find(编码.值));
        return 状态::已完成;
    } catch (const std::bad_alloc&) { return 状态::资源失败;
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"原子删除节点结构", L"字段结构与索引删除异常。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"原子删除节点结构", L"字段结构与索引删除未知异常。");
    }
}

稳定编码 基础数据集::新建节点() noexcept {
    try {
        std::unique_lock 锁(互斥_);
        const auto 编码 = 分配编码();
        if (!有效(编码)) return {};
        节点_.emplace(编码.值, 基础节点{编码});
        return 编码;
    } catch (...) {
        return {};
    }
}

稳定编码 基础数据集::新建节点(稳定编码 上级节点) noexcept {
    try {
        std::unique_lock 锁(互斥_);
        if (!节点存在(上级节点)) return {};

        const auto 节点编码 = 分配编码();
        const auto 关系编码 = 分配编码();
        if (!有效(节点编码) || !有效(关系编码)) return {};

        const auto [节点位置, 节点已插入] =
            节点_.emplace(节点编码.值, 基础节点{节点编码});
        if (!节点已插入) return {};
        try {
            插入关系已加锁(基础外部关系{关系编码,上级节点,节点编码,基础外部关系类型::父子,0});
        } catch (...) {
            节点_.erase(节点位置);
            throw;
        }
        return 节点编码;
    } catch (...) {
        return {};
    }
}

bool 基础数据集::删除节点(稳定编码 节点) noexcept {
    try {
        std::unique_lock 锁(互斥_);
        if (!节点存在(节点) || 节点被引用(节点)) return false;
        return 节点_.erase(节点.值) == 1;
    } catch (...) {
        return false;
    }
}

std::optional<基础节点> 基础数据集::查询节点(稳定编码 节点) const noexcept {
    try {
        std::shared_lock 锁(互斥_);
        const auto 位置 = 节点_.find(节点.值);
        return 位置 == 节点_.end()
            ? std::nullopt : std::optional<基础节点>{位置->second};
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"基础数据集::查询节点",
            L"节点读取发生C++异常，未形成业务结果。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"基础数据集::查询节点",
            L"节点读取发生未知C++异常，未形成业务结果。");
    }
}

基础节点建立结果 基础数据集::原子建立节点(
    const 基础节点建立请求& 请求) noexcept {
    using 状态 = 基础结构操作状态;
    try {
        // 单节点入口只适配值式请求，发布仍经同一个组构建实现。
        std::vector<基础组内新节点> 组请求(1);
        组请求.front().字段组.reserve(请求.字段组.size());
        for (const auto& 字段 : 请求.字段组) {
            if (const auto* 值 = std::get_if<基础值>(&字段.内容))
                组请求.front().字段组.push_back({字段.字段节点, *值});
            else if (const auto* 引用 = std::get_if<稳定编码>(&字段.内容))
                组请求.front().字段组.push_back({字段.字段节点, *引用});
            else return {状态::入口拒绝, {}};
        }
        std::unique_lock 锁(互斥_);
        const auto 结果 = 原子建立节点组已加锁(组请求, 请求.上级节点);
        if (结果.状态 != 状态::已完成) return {结果.状态, {}};
        return {状态::已完成, 结果.节点组.front()};
    } catch (const std::bad_alloc&) { return {状态::资源失败, {}};
    } catch (const std::length_error&) { return {状态::资源失败, {}};
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"原子建立节点", L"字段与索引候选建立异常。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"原子建立节点", L"字段与索引候选建立未知异常。");
    }
}

基础节点组建立结果 基础数据集::原子建立节点组(
    const std::vector<基础组内新节点>& 请求) noexcept {
    using 状态 = 基础结构操作状态;
    try {
        std::unique_lock 锁(互斥_);
        return 原子建立节点组已加锁(请求, std::nullopt);
    } catch (const std::bad_alloc&) { return {状态::资源失败, {}};
    } catch (const std::length_error&) { return {状态::资源失败, {}};
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"原子建立节点组", L"字段与索引候选建立异常。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"原子建立节点组", L"字段与索引候选建立未知异常。");
    }
}

基础节点组建立结果 基础数据集::原子建立节点组已加锁(
    const std::vector<基础组内新节点>& 请求,
    std::optional<稳定编码> 上级节点) {
    using 状态 = 基础结构操作状态;
    if (请求.empty() || (上级节点 && 请求.size() != 1))
        return {状态::入口拒绝, {}};
    if (上级节点 && !有效(*上级节点)) return {状态::入口拒绝, {}};
    if (上级节点 && !节点存在(*上级节点)) return {状态::引用不存在, {}};
    for (const auto& 节点 : 请求) {
        for (const auto& 字段 : 节点.字段组) {
            if (!有效(字段.字段节点) || 字段.内容.valueless_by_exception())
                return {状态::入口拒绝, {}};
            if (!节点存在(字段.字段节点)) return {状态::引用不存在, {}};
            if (const auto* 引用 = std::get_if<稳定编码>(&字段.内容)) {
                if (!有效(*引用)) return {状态::入口拒绝, {}};
                if (!节点存在(*引用)) return {状态::引用不存在, {}};
            } else if (const auto* 组内 = std::get_if<基础组内节点引用>(&字段.内容)) {
                if (组内->索引 >= 请求.size()) return {状态::入口拒绝, {}};
            } else if (std::get<基础值>(字段.内容).材料.valueless_by_exception()) {
                return {状态::入口拒绝, {}};
            }
        }
    }

    // 独占锁内构造隔离候选；分配水位属于同一分配器，失败也不回退。
    // 这里不调用任何上层服务、回调、I/O或本对象公开入口。
    auto 候选节点 = 节点_;
    auto 候选字段 = 字段关系_;
    auto 候选关系 = 外部关系_;
    auto 候选源关系索引 = 源关系索引_;
    auto 候选目标关系索引 = 目标关系索引_;
    auto 候选所属索引 = 所属字段索引_;
    auto 候选目标索引 = 目标字段索引_;
    auto 候选入向索引 = 入向字段索引_;
    auto 候选字段键索引 = 字段节点索引_;
    基础节点组建立结果 结果{状态::已完成, {}};
    结果.节点组.reserve(请求.size());
    const auto 编码已占用 = [&](稳定编码 编码) {
        return 候选节点.contains(编码.值) || 候选字段.contains(编码.值)
            || 候选关系.contains(编码.值);
    };
    for (std::size_t 索引 = 0; 索引 < 请求.size(); ++索引) {
        const auto 编码 = 分配编码();
        if (!有效(编码)) return {状态::资源失败, {}};
        if (编码已占用(编码)) return {状态::内部不一致, {}};
        候选节点.emplace(编码.值, 基础节点{编码});
        结果.节点组.push_back(编码);
    }
    for (std::size_t 索引 = 0; 索引 < 请求.size(); ++索引) {
        for (const auto& 字段 : 请求[索引].字段组) {
            基础字段内容 内容;
            if (const auto* 值 = std::get_if<基础值>(&字段.内容)) 内容 = *值;
            else if (const auto* 引用 = std::get_if<稳定编码>(&字段.内容)) 内容 = *引用;
            else 内容 = 结果.节点组[std::get<基础组内节点引用>(字段.内容).索引];
            const auto 编码 = 分配编码();
            if (!有效(编码)) return {状态::资源失败, {}};
            if (编码已占用(编码)) return {状态::内部不一致, {}};
            候选字段.emplace(编码.值, 基础字段关系{
                编码, 结果.节点组[索引], 字段.字段节点, std::move(内容)});
            const auto& 新字段 = 候选字段.at(编码.值);
            加字段索引(新字段,候选所属索引,候选目标索引,候选入向索引,候选字段键索引);
        }
    }
    if (上级节点) {
        const auto 编码 = 分配编码();
        if (!有效(编码)) return {状态::资源失败, {}};
        if (编码已占用(编码)) return {状态::内部不一致, {}};
        const 基础外部关系 边{编码,*上级节点,结果.节点组.front(),基础外部关系类型::父子,0};
        候选关系.emplace(编码.值,边);
        加关系索引(边,候选源关系索引,候选目标关系索引);
    }
    // map使用标准分配器和无状态比较器，交换不分配、不抛异常。
    // 读者只能在本次独占锁释放后观察到全部候选。
    static_assert(std::is_nothrow_swappable_v<decltype(源关系索引_)>);
    static_assert(std::is_nothrow_swappable_v<decltype(目标关系索引_)>);
    static_assert(std::is_nothrow_swappable_v<decltype(入向字段索引_)>);
    static_assert(std::is_nothrow_swappable_v<decltype(字段节点索引_)>);
    节点_.swap(候选节点);
    字段关系_.swap(候选字段);
    外部关系_.swap(候选关系);
    源关系索引_.swap(候选源关系索引);
    目标关系索引_.swap(候选目标关系索引);
    所属字段索引_.swap(候选所属索引);
    目标字段索引_.swap(候选目标索引);
    入向字段索引_.swap(候选入向索引);
    字段节点索引_.swap(候选字段键索引);
    return 结果;
}

基础节点组追加状态 基础数据集::核验只读预期已加锁(const 基础只读预期& 预期) const {
    using 状态 = 基础节点组追加状态;
    auto 正规 = 预期;
    const auto 形状 = 规范化只读预期(正规);
    if (形状 != 状态::已完成) return 形状;
    std::set<稳定编码> 宿主, 显式引用;
    for (const auto& s : 正规.字段集合组) {
        宿主.insert(s.节点);
        if (s.字段节点) 显式引用.insert(*s.字段节点);
        显式引用.insert(s.排除字段键组.begin(), s.排除字段键组.end());
    }
    for (const auto& s : 正规.外部关系集合组) {
        宿主.insert(s.节点);
        if (s.对端节点) 显式引用.insert(*s.对端节点);
    }
    for (const auto& n : 正规.节点组) if (!节点存在(n.编码)) {
        if (显式引用.contains(n.编码) && !宿主.contains(n.编码)) return 状态::引用不存在;
        return 状态::节点不存在;
    }
    const auto 检查预期材料 = [&](const 基础字段关系& f) {
        if (!节点存在(f.字段节点)) return false;
        const auto* ref = std::get_if<稳定编码>(&f.内容);
        return !ref || 节点存在(*ref);
    };
    for (const auto& s : 正规.字段集合组) {
        for (const auto& f : s.字段组) if (!检查预期材料(f)) return 状态::引用不存在;
        const auto 键命中 = [&](稳定编码 key) {
            return (!s.字段节点 || key == *s.字段节点)
                && !std::binary_search(s.排除字段键组.begin(), s.排除字段键组.end(), key);
        };
        std::vector<基础字段关系> 当前;
        读取节点字段已加锁(s.节点, 当前);
        std::erase_if(当前, [&](const auto& f) { return !键命中(f.字段节点); });
        // 保留会阻止过期材料发布的真实前提检查；索引一致性由写端维护。
        for (const auto& f : 当前) {
            if (!有效(f.编码) || !有效(f.字段节点)
                || !封存字段内容合法(f.内容) || !检查预期材料(f))
                throw std::logic_error("readonly field selector current material damaged");
        }
        if (当前 != s.字段组) return 状态::预期不符;
    }
    for (const auto& s : 正规.外部关系集合组) {
        for (const auto& r : s.关系组)
            if (!节点存在(r.源节点) || !节点存在(r.目标节点)) return 状态::引用不存在;
        std::vector<基础外部关系> 当前;
        std::vector<基础外部关系> 关联;
        读取节点关系已加锁(s.节点,s.出向,s.类型,关联);
        for (const auto& r : 关联) {
            if ((s.角色或顺序 && r.角色或顺序 != *s.角色或顺序)
                || (s.对端节点 && (s.出向 ? r.目标节点 : r.源节点) != *s.对端节点)) continue;
            if (!有效(r.编码) || !节点存在(r.源节点) || !节点存在(r.目标节点))
                throw std::logic_error("readonly relation selector current material damaged");
            当前.push_back(r);
        }
        if (当前 != s.关系组) return 状态::预期不符;
    }
    return 状态::已完成;
}

基础节点组追加状态 基础数据集::核验只读预期(const 基础只读预期& 预期) const noexcept {
    using 状态 = 基础节点组追加状态;
    try {
        std::shared_lock 锁(互斥_);
        return 核验只读预期已加锁(预期);
    } catch (const std::bad_alloc&) { return 状态::资源失败;
    } catch (const std::length_error&) { return 状态::资源失败;
    } catch (const std::exception& e) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"核验只读预期", L"只读集合或当前索引损坏。", e.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"核验只读预期", L"未知只读集合核验异常。");
    }
}

基础节点组追加结果 基础数据集::原子追加节点组(
    const 基础节点组追加请求& 请求) noexcept {
    using 状态 = 基础节点组追加状态;
    try {
        std::unique_lock 锁(互斥_);
        if (请求.新节点组.empty() && 请求.追加字段组.empty()
            && 请求.替换字段组.empty() && 请求.追加外部关系组.empty()
            && 请求.删除字段关系组.empty())
            return {状态::入口拒绝, {}, {}};
        if (!只读预期为空(请求.只读预期)) {
            const auto 校验 = 核验只读预期已加锁(请求.只读预期);
            if (校验 != 状态::已完成) return {校验, {}, {}};
        }
        const auto 检查字段 = [&](稳定编码 键, const 基础组内字段内容& 内容) {
            if (!有效(键) || 内容.valueless_by_exception()) return 状态::入口拒绝;
            if (!节点存在(键)) return 状态::引用不存在;
            if (const auto* 引用 = std::get_if<稳定编码>(&内容)) {
                if (!有效(*引用)) return 状态::入口拒绝;
                if (!节点存在(*引用)) return 状态::引用不存在;
            } else if (const auto* 内部 = std::get_if<基础组内节点引用>(&内容)) {
                if (内部->索引 >= 请求.新节点组.size()) return 状态::入口拒绝;
            } else if (std::get<基础值>(内容).材料.valueless_by_exception()) return 状态::入口拒绝;
            return 状态::已完成;
        };
        std::set<稳定编码> 宿主组;
        for (const auto& 结构 : 请求.预期既有节点组) {
            const auto 宿主 = 结构.节点.编码;
            if (!有效(宿主) || !宿主组.insert(宿主).second) return {状态::入口拒绝, {}, {}};
            if (!节点存在(宿主)) return {状态::节点不存在, {}, {}};
            auto 预期字段 = 结构.字段组;
            std::sort(预期字段.begin(), 预期字段.end(), [](const auto& 左, const auto& 右) {
                return 左.编码 < 右.编码;
            });
            稳定编码 上项;
            for (const auto& 项 : 预期字段) {
                if (!有效(项.编码) || 项.编码 == 上项 || 项.所属节点 != 宿主
                    || 项.内容.valueless_by_exception()) return {状态::入口拒绝, {}, {}};
                上项 = 项.编码;
                const auto 内容 = std::visit([](const auto& 值) -> 基础组内字段内容 { return 值; }, 项.内容);
                const auto 检查 = 检查字段(项.字段节点, 内容);
                if (检查 != 状态::已完成) return {检查, {}, {}};
            }
            std::vector<基础字段关系> 当前字段;
            读取节点字段已加锁(宿主, 当前字段);
            if (当前字段 != 预期字段) return {状态::预期不符, {}, {}};
        }
        std::set<稳定编码> 联合宿主组;
        for (const auto& 快照 : 请求.预期联合节点组) {
            const auto 宿主 = 快照.结构.节点.编码;
            if (!有效(宿主) || !联合宿主组.insert(宿主).second)
                return {状态::入口拒绝, {}, {}};
            if (!节点存在(宿主)) return {状态::节点不存在, {}, {}};
            if (!联合快照相等(读取联合快照已加锁(宿主), 快照))
                return {状态::预期不符, {}, {}};
            宿主组.insert(宿主);
        }
        // 读取组为本请求读取并预期的全部节点：容许预期组与严格联合预期组。
        std::set<稳定编码> 容许组节点;
        for (const auto& 快照 : 请求.容许新增入向预期组) 容许组节点.insert(快照.结构.节点.编码);
        for (const auto& 快照 : 请求.预期联合节点组) 容许组节点.insert(快照.结构.节点.编码);
        for (const auto& 快照 : 请求.容许新增入向预期组) {
            const auto 宿主 = 快照.结构.节点.编码;
            if (!有效(宿主)) return {状态::入口拒绝, {}, {}};
            // 入向字段身份不得重复（重复会让同一当前字段被两次命中）。
            for (std::size_t i = 1; i < 快照.入向字段组.size(); ++i)
                for (std::size_t j = 0; j < i; ++j)
                    if (快照.入向字段组[j].编码 == 快照.入向字段组[i].编码) return {状态::入口拒绝, {}, {}};
            if (!节点存在(宿主)) return {状态::节点不存在, {}, {}};
            if (!联合快照容许新增入向(读取联合快照已加锁(宿主), 快照, 容许组节点))
                return {状态::预期不符, {}, {}};
        }
        std::set<稳定编码> 删除字段码;
        for (const auto 编码 : 请求.删除字段关系组) {
            if (!有效(编码) || !删除字段码.insert(编码).second)
                return {状态::入口拒绝, {}, {}};
            const auto 位置 = 字段关系_.find(编码.值);
            if (位置 == 字段关系_.end()
                || !联合宿主组.contains(位置->second.所属节点))
                return {状态::入口拒绝, {}, {}};
            // 所属完整快照已相等；节点引用删除还须保护旧目标的完整结构。
            const auto& 字段 = 位置->second;
            if (字段.内容.valueless_by_exception() || !节点存在(字段.字段节点))
                throw std::logic_error("追加删除字段的当前内容或字段节点损坏");
            if (const auto* 目标 = std::get_if<稳定编码>(&字段.内容)) {
                if (!节点存在(*目标))
                    throw std::logic_error("追加删除字段的当前引用目标缺失");
                if (!联合宿主组.contains(*目标)) return {状态::入口拒绝, {}, {}};
            } else if (std::get<基础值>(字段.内容).材料.valueless_by_exception()) {
                throw std::logic_error("追加删除字段的当前直接值损坏");
            }
#if HY_EGO_CORE_INDEX_AUDIT
            核验字段索引已加锁(字段);
#endif
        }
        std::set<稳定编码> 替换字段码;
        for (const auto& 项 : 请求.替换字段组) {
            if (!有效(项.字段关系) || !替换字段码.insert(项.字段关系).second
                || 删除字段码.contains(项.字段关系)
                || 项.新内容.valueless_by_exception()) return {状态::入口拒绝, {}, {}};
            const auto 位置 = 字段关系_.find(项.字段关系.值);
            if (位置 == 字段关系_.end() || !联合宿主组.contains(位置->second.所属节点))
                return {状态::入口拒绝, {}, {}};
            const auto 内容 = std::visit([](const auto& 值) -> 基础组内字段内容 { return 值; }, 项.新内容);
            const auto 检查 = 检查字段(位置->second.字段节点, 内容);
            if (检查 != 状态::已完成) return {检查, {}, {}};
            const auto& 旧内容 = std::as_const(位置->second.内容);
            for (const auto* 引用 : {std::get_if<稳定编码>(&旧内容),
                    std::get_if<稳定编码>(&项.新内容)})
                if (引用 && !联合宿主组.contains(*引用)) return {状态::入口拒绝, {}, {}};
#if HY_EGO_CORE_INDEX_AUDIT
            核验字段索引已加锁(位置->second);
#endif
        }
        const auto 检查端点 = [&](const std::variant<稳定编码, 基础组内节点引用>& 端点) {
            if (端点.valueless_by_exception()) return 状态::入口拒绝;
            if (const auto* 编码 = std::get_if<稳定编码>(&端点)) {
                if (!有效(*编码) || !联合宿主组.contains(*编码)) return 状态::入口拒绝;
                return 节点存在(*编码) ? 状态::已完成 : 状态::引用不存在;
            }
            return std::get<基础组内节点引用>(端点).索引 < 请求.新节点组.size()
                ? 状态::已完成 : 状态::入口拒绝;
        };
        for (const auto& 项 : 请求.追加外部关系组) {
            if (!有效(项.类型)) return {状态::入口拒绝, {}, {}};
            const auto 源检查 = 检查端点(项.源节点), 目标检查 = 检查端点(项.目标节点);
            if (源检查 != 状态::已完成) return {源检查, {}, {}};
            if (目标检查 != 状态::已完成) return {目标检查, {}, {}};
        }
        for (const auto& 节点 : 请求.新节点组) for (const auto& 项 : 节点.字段组) {
            const auto 检查 = 检查字段(项.字段节点, 项.内容);
            if (检查 != 状态::已完成) return {检查, {}, {}};
        }
        for (const auto& 项 : 请求.追加字段组) {
            if (!有效(项.所属节点) || !宿主组.contains(项.所属节点)) return {状态::入口拒绝, {}, {}};
            if ((!请求.替换字段组.empty() || !请求.追加外部关系组.empty()
                    || !请求.删除字段关系组.empty())
                && !联合宿主组.contains(项.所属节点)) return {状态::入口拒绝, {}, {}};
            const auto 检查 = 检查字段(项.字段节点, 项.内容);
            if (检查 != 状态::已完成) return {检查, {}, {}};
        }
        auto 候选节点 = 节点_;
        auto 候选字段 = 字段关系_;
        auto 候选所属索引 = 所属字段索引_;
        auto 候选目标索引 = 目标字段索引_;
        auto 候选入向索引 = 入向字段索引_;
        auto 候选字段键索引 = 字段节点索引_;
        // 只有新增外部边时才复制关系表及两份关系索引。
        std::optional<decltype(外部关系_)> 候选关系;
        std::optional<decltype(源关系索引_)> 候选源关系索引,候选目标关系索引;
        if (!请求.追加外部关系组.empty()) {
            候选关系 = 外部关系_;
            候选源关系索引 = 源关系索引_;
            候选目标关系索引 = 目标关系索引_;
        }
        基础节点组追加结果 结果{状态::已完成, {}, {}};
        结果.新节点组.reserve(请求.新节点组.size());
        结果.追加字段关系组.reserve(请求.追加字段组.size());
        结果.追加外部关系组.reserve(请求.追加外部关系组.size());
        结果.新节点初始字段关系组.resize(请求.新节点组.size());
        for (std::size_t i = 0; i < 请求.新节点组.size(); ++i)
            结果.新节点初始字段关系组[i].resize(请求.新节点组[i].字段组.size());
        const auto 已占用 = [&](稳定编码 编码) {
            return 候选节点.contains(编码.值) || 候选字段.contains(编码.值)
                || (候选关系 ? 候选关系->contains(编码.值) : 外部关系_.contains(编码.值));
        };
        for (std::size_t i = 0; i < 请求.新节点组.size(); ++i) {
            const auto 编码 = 分配编码();
            if (!有效(编码)) return {状态::资源失败, {}, {}};
            if (已占用(编码)) throw std::logic_error("append node identity collision");
            候选节点.emplace(编码.值, 基础节点{编码});
            结果.新节点组.push_back(编码);
        }
        for (const auto 编码 : 请求.删除字段关系组) {
            const auto 位置 = 候选字段.find(编码.值);
            if (位置 == 候选字段.end())
                throw std::logic_error("追加删除字段的候选记录缺失");
            const auto& 字段 = 位置->second;
            删字段索引(字段,候选所属索引,候选目标索引,候选入向索引,候选字段键索引);
            候选字段.erase(位置);
        }
        for (const auto& 项 : 请求.替换字段组) {
            auto& 字段 = 候选字段.at(项.字段关系.值);
            删字段索引(字段,候选所属索引,候选目标索引,候选入向索引,候选字段键索引);
            字段.内容 = 项.新内容;
            加字段索引(字段,候选所属索引,候选目标索引,候选入向索引,候选字段键索引);
        }
        const auto 加字段 = [&](稳定编码 宿主, 稳定编码 键, const 基础组内字段内容& 源) {
            基础字段内容 内容;
            if (const auto* 值 = std::get_if<基础值>(&源)) 内容 = *值;
            else if (const auto* 引用 = std::get_if<稳定编码>(&源)) 内容 = *引用;
            else 内容 = 结果.新节点组.at(std::get<基础组内节点引用>(源).索引);
            const auto 编码 = 分配编码();
            if (!有效(编码)) return 编码;
            if (已占用(编码)) throw std::logic_error("append field identity collision");
            候选字段.emplace(编码.值, 基础字段关系{编码, 宿主, 键, std::move(内容)});
            加字段索引(候选字段.at(编码.值),候选所属索引,候选目标索引,候选入向索引,候选字段键索引);
            return 编码;
        };
        for (std::size_t i = 0; i < 请求.新节点组.size(); ++i) {
            for (std::size_t j = 0; j < 请求.新节点组[i].字段组.size(); ++j) {
                const auto& 项 = 请求.新节点组[i].字段组[j];
                const auto 字段码 = 加字段(结果.新节点组[i], 项.字段节点, 项.内容);
                if (!有效(字段码)) return {状态::资源失败, {}, {}};
                结果.新节点初始字段关系组[i][j] = 字段码;
            }
        }
        for (const auto& 项 : 请求.追加字段组) {
            const auto 字段码 = 加字段(项.所属节点, 项.字段节点, 项.内容);
            if (!有效(字段码)) return {状态::资源失败, {}, {}};
            结果.追加字段关系组.push_back(字段码);
        }
        const auto 解析端点 = [&](const std::variant<稳定编码, 基础组内节点引用>& 端点) {
            if (const auto* 编码 = std::get_if<稳定编码>(&端点)) return *编码;
            return 结果.新节点组.at(std::get<基础组内节点引用>(端点).索引);
        };
        for (const auto& 项 : 请求.追加外部关系组) {
            const auto 编码 = 分配编码();
            if (!有效(编码)) return {状态::资源失败, {}, {}};
            if (已占用(编码)) throw std::logic_error("append relation identity collision");
            const 基础外部关系 边{编码,解析端点(项.源节点),解析端点(项.目标节点),项.类型,项.角色或顺序};
            候选关系->emplace(编码.值,边);
            加关系索引(边,*候选源关系索引,*候选目标关系索引);
            结果.追加外部关系组.push_back(编码);
        }
        // 全部候选与索引已闭合；以下交换不再分配。
        节点_.swap(候选节点); 字段关系_.swap(候选字段);
        所属字段索引_.swap(候选所属索引); 目标字段索引_.swap(候选目标索引);
        入向字段索引_.swap(候选入向索引);
        字段节点索引_.swap(候选字段键索引);
        if (候选关系) {
            外部关系_.swap(*候选关系);
            源关系索引_.swap(*候选源关系索引);
            目标关系索引_.swap(*候选目标关系索引);
        }
        return 结果;
    } catch (const std::bad_alloc&) { return {状态::资源失败, {}, {}};
    } catch (const std::length_error&) { return {状态::资源失败, {}, {}};
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"原子追加节点组", L"追加结构或索引异常", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"原子追加节点组", L"未知追加异常");
    }
}

基础片段封存结果 基础数据集::封存原子追加片段(
    const 基础节点组追加请求& 请求) noexcept {
    using 状态 = 基础节点组追加状态;
    try {
        auto 材料 = std::make_shared<基础封存追加片段::内容>();
        材料->请求 = 请求;
        const auto 检查 = 规范化封存请求(材料->请求);
        if (检查 != 状态::已完成) return {检查, {}};
        {
            std::unique_lock 锁(互斥_);
            if (!封存实例身份_) 封存实例身份_ = std::make_shared<const int>(0);
            材料->数据集身份 = 封存实例身份_;
        }
        return {状态::已完成, 基础封存追加片段{std::move(材料)}};
    } catch (const std::bad_alloc&) { return {状态::资源失败, {}};
    } catch (const std::length_error&) { return {状态::资源失败, {}};
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"封存原子追加片段", L"封存结构异常", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"封存原子追加片段", L"未知封存异常");
    }
}

基础片段组合结果 基础数据集::原子追加封存片段组(
    const std::vector<基础封存追加片段>& 片段组) noexcept {
    using 状态 = 基础节点组追加状态;
    bool 已发布 = false;
    try {
        if (片段组.empty()) return {状态::入口拒绝, {}};
        std::shared_ptr<const int> 本数据集;
        {
            std::shared_lock 锁(互斥_);
            本数据集 = 封存实例身份_;
        }
        基础节点组追加请求 联合;
        std::set<const 基础封存追加片段::内容*> 已用片段;
        std::map<稳定编码, const 基础联合节点快照*> 联合预期;
        std::map<稳定编码, const 基础字段关系*> 旧字段;
        std::map<稳定编码, std::size_t> 字段写者;
        std::map<std::pair<稳定编码, 稳定编码>, std::size_t> 字段键写者;
        // 布尔标记区别既有正式编码和本次组内索引，避免两个命名空间碰撞。
        using 端点键 = std::pair<bool, std::uint64_t>;
        using 边键 = std::tuple<端点键, 端点键, 基础外部关系类型, std::int64_t>;
        std::map<边键, std::size_t> 边写者;
        std::vector<std::shared_ptr<基础片段发布回执::内容>> 回执内容;
        基础片段组合结果 结果{状态::已完成, {}};
        回执内容.reserve(片段组.size());
        结果.片段回执组.reserve(片段组.size());
        const auto 检查容量 = [](std::size_t 当前, std::size_t 增量) {
            if (增量 > (std::numeric_limits<std::size_t>::max)() - 当前)
                throw std::length_error("sealed composition size overflow");
        };
        for (std::size_t i = 0; i < 片段组.size(); ++i) {
            const auto& 片段 = 片段组[i];
            if (!片段.内容_ || !本数据集 || 片段.内容_->数据集身份 != 本数据集
                || !已用片段.insert(片段.内容_.get()).second)
                return {状态::入口拒绝, {}};
            const auto& 请求 = 片段.内容_->请求;
            联合.只读预期.节点组.insert(联合.只读预期.节点组.end(), 请求.只读预期.节点组.begin(), 请求.只读预期.节点组.end());
            联合.只读预期.字段集合组.insert(联合.只读预期.字段集合组.end(), 请求.只读预期.字段集合组.begin(), 请求.只读预期.字段集合组.end());
            联合.只读预期.外部关系集合组.insert(联合.只读预期.外部关系集合组.end(), 请求.只读预期.外部关系集合组.begin(), 请求.只读预期.外部关系集合组.end());
            联合.容许新增入向预期组.insert(联合.容许新增入向预期组.end(),
                请求.容许新增入向预期组.begin(), 请求.容许新增入向预期组.end());
            for (const auto& 快照 : 请求.预期联合节点组) {
                const auto [位置, 新增] = 联合预期.emplace(快照.结构.节点.编码, &快照);
                if (!新增 && !联合快照相等(*位置->second, 快照))
                    return {状态::预期不符, {}};
                if (新增) {
                    联合.预期联合节点组.push_back(快照);
                    for (const auto& 字段 : 快照.结构.字段组) {
                        const auto [字段位置, 新字段] = 旧字段.emplace(字段.编码, &字段);
                        if (!新字段 && *字段位置->second != 字段)
                            return {状态::预期不符, {}};
                    }
                }
            }
            const auto 登记键 = [&](稳定编码 宿主, 稳定编码 键) {
                const auto [位置, 新增] = 字段键写者.emplace(std::make_pair(宿主, 键), i);
                return 新增 || 位置->second == i;
            };
            const auto 登记旧字段 = [&](稳定编码 编码) {
                const auto 位置 = 旧字段.find(编码);
                if (位置 == 旧字段.end()) throw std::logic_error("sealed field snapshot absent");
                return 字段写者.emplace(编码, i).second
                    && 登记键(位置->second->所属节点, 位置->second->字段节点);
            };
            for (const auto 编码 : 请求.删除字段关系组)
                if (!登记旧字段(编码)) return {状态::入口拒绝, {}};
            for (const auto& 替换 : 请求.替换字段组)
                if (!登记旧字段(替换.字段关系)) return {状态::入口拒绝, {}};
            for (const auto& 字段 : 请求.追加字段组)
                if (!登记键(字段.所属节点, 字段.字段节点)) return {状态::入口拒绝, {}};

            const auto 节点偏移 = 联合.新节点组.size();
            检查容量(节点偏移, 请求.新节点组.size());
            检查容量(联合.追加字段组.size(), 请求.追加字段组.size());
            检查容量(联合.追加外部关系组.size(), 请求.追加外部关系组.size());
            检查容量(联合.替换字段组.size(), 请求.替换字段组.size());
            检查容量(联合.删除字段关系组.size(), 请求.删除字段关系组.size());
            const auto 重定位 = [&](auto& 引用) {
                if (auto* 局部 = std::get_if<基础组内节点引用>(&引用))
                    局部->索引 += 节点偏移; // 封存已限定本片段范围，前缀加法已检查。
            };
            for (const auto& 节点 : 请求.新节点组) {
                auto 副本 = 节点;
                for (auto& 字段 : 副本.字段组) 重定位(字段.内容);
                联合.新节点组.push_back(std::move(副本));
            }
            for (const auto& 字段 : 请求.追加字段组) {
                auto 副本 = 字段;
                重定位(副本.内容);
                联合.追加字段组.push_back(std::move(副本));
            }
            const auto 取端点键 = [](const auto& 端点) -> 端点键 {
                if (const auto* 编码 = std::get_if<稳定编码>(&端点)) return {false, 编码->值};
                return {true, std::get<基础组内节点引用>(端点).索引};
            };
            for (const auto& 关系 : 请求.追加外部关系组) {
                auto 副本 = 关系;
                重定位(副本.源节点); 重定位(副本.目标节点);
                const 边键 键{取端点键(副本.源节点), 取端点键(副本.目标节点), 副本.类型, 副本.角色或顺序};
                const auto [位置, 新增] = 边写者.emplace(键, i);
                if (!新增 && 位置->second != i) return {状态::入口拒绝, {}};
                联合.追加外部关系组.push_back(std::move(副本));
            }
            联合.替换字段组.insert(联合.替换字段组.end(), 请求.替换字段组.begin(), 请求.替换字段组.end());
            联合.删除字段关系组.insert(联合.删除字段关系组.end(), 请求.删除字段关系组.begin(), 请求.删除字段关系组.end());
            auto 回执 = std::make_shared<基础片段发布回执::内容>(片段,
                请求.新节点组, 请求.追加字段组.size(), 请求.追加外部关系组.size());
            结果.片段回执组.push_back(基础片段发布回执{回执});
            回执内容.push_back(std::move(回执));
        }
        // 不持有外层数据集锁；所有回执和映射已分配，仅此处调用一次原事务。
        if (!只读预期为空(联合.只读预期)) {
            const auto 规范化 = 规范化只读预期(联合.只读预期);
            if (规范化 != 状态::已完成) return {规范化, {}};
        }
        const auto 发布 = 原子追加节点组(联合);
        if (发布.状态 != 状态::已完成) return {发布.状态, {}};
        已发布 = true;
        if (发布.新节点组.size() != 联合.新节点组.size()
            || 发布.新节点初始字段关系组.size() != 联合.新节点组.size()
            || 发布.追加字段关系组.size() != 联合.追加字段组.size()
            || 发布.追加外部关系组.size() != 联合.追加外部关系组.size())
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"原子追加封存片段组", L"发布结果尺寸与封存请求不一致");
        std::size_t 节点位置 = 0, 字段位置 = 0, 关系位置 = 0;
        for (auto& 回执 : 回执内容) {
            for (std::size_t i = 0; i < 回执->新节点.size(); ++i) {
                auto& 初始字段 = 回执->新节点初始字段[i];
                const auto& 已发布字段 = 发布.新节点初始字段关系组[节点位置];
                if (初始字段.size() != 已发布字段.size())
                    工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"原子追加封存片段组", L"新节点初始字段回执尺寸不一致");
                std::copy(已发布字段.begin(), 已发布字段.end(), 初始字段.begin());
                回执->新节点[i] = 发布.新节点组[节点位置++];
            }
            for (auto& 编码 : 回执->追加字段) 编码 = 发布.追加字段关系组[字段位置++];
            for (auto& 编码 : 回执->追加外部关系) 编码 = 发布.追加外部关系组[关系位置++];
        }
        return 结果;
    } catch (const std::bad_alloc&) {
        if (已发布) 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"原子追加封存片段组", L"发布后发生资源异常");
        return {状态::资源失败, {}};
    } catch (const std::length_error&) {
        if (已发布) 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"原子追加封存片段组", L"发布后发生容量异常");
        return {状态::资源失败, {}};
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"原子追加封存片段组", L"组合结构或回执异常", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"原子追加封存片段组", L"未知组合异常");
    }
}

基础节点结构结果 基础数据集::查询完整节点结构(稳定编码 节点) const noexcept {
    using 状态 = 基础结构操作状态;
    try {
        std::shared_lock 锁(互斥_);
        const auto 位置 = 节点_.find(节点.值);
        if (位置 == 节点_.end()) return {状态::节点不存在, {}};
        基础节点结构 结构{位置->second, {}};
        读取节点字段已加锁(节点, 结构.字段组);
        return {状态::已完成, std::move(结构)};
    } catch (const std::bad_alloc&) { return {状态::资源失败, {}};
    } catch (const std::length_error&) { return {状态::资源失败, {}};
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"查询完整节点结构", L"字段索引读取异常。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"查询完整节点结构", L"字段索引读取未知异常。");
    }
}

基础引用节点结构结果 基础数据集::查询字段引用节点结构(
    稳定编码 目标节点, 稳定编码 字段节点) const noexcept {
    using 状态 = 基础结构操作状态;
    try {
        std::shared_lock 锁(互斥_);
        if (!节点存在(目标节点) || !节点存在(字段节点))
            return {状态::节点不存在, {}, {}};
        基础引用节点结构结果 结果{状态::已完成, {}, {}};
        std::map<std::uint64_t, 基础节点结构> 来源组;
        for (auto 位置 = 目标字段索引_.lower_bound({字段节点.值, 目标节点.值, 0});
            位置 != 目标字段索引_.end() && std::get<0>(*位置) == 字段节点.值
                && std::get<1>(*位置) == 目标节点.值; ++位置) {
            const auto& 字段 = 字段关系_.at(std::get<2>(*位置));
            const auto 来源 = 节点_.find(字段.所属节点.值);
            if (来源 == 节点_.end()) return {状态::内部不一致, {}, {}};
            结果.引用字段组.push_back(字段);
            来源组.try_emplace(来源->first, 基础节点结构{来源->second, {}});
        }
        结果.来源节点组.reserve(来源组.size());
        for (auto& [编码, 来源] : 来源组) {
            读取节点字段已加锁(来源.节点.编码, 来源.字段组);
            结果.来源节点组.push_back(std::move(来源));
        }
        return 结果;
    } catch (const std::bad_alloc&) { return {状态::资源失败, {}, {}};
    } catch (const std::length_error&) { return {状态::资源失败, {}, {}};
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"查询字段引用节点结构", L"反向字段索引读取异常。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"查询字段引用节点结构", L"反向字段索引读取未知异常。");
    }
}

基础联合节点查询结果 基础数据集::查询节点组完整结构(
    const std::vector<稳定编码>& 节点组) const noexcept {
    using 状态 = 基础结构操作状态;
    try {
        std::shared_lock 锁(互斥_);
        if (节点组.empty()) return {状态::入口拒绝, {}};
        std::set<std::uint64_t> 身份;
        for (const auto 编码 : 节点组)
            if (!有效(编码) || !身份.insert(编码.值).second)
                return {状态::入口拒绝, {}};
        基础联合节点查询结果 结果{状态::已完成, {}};
        结果.节点组.reserve(身份.size());
        for (const auto 编码 : 身份) {
            const 稳定编码 节点{编码};
            if (节点存在(节点)) 结果.节点组.push_back({节点,读取联合快照已加锁(节点)});
            else {
                if(节点被引用(节点))return {状态::内部不一致,{}};
                结果.节点组.push_back({节点,std::nullopt});
            }
        }
        return 结果;
    } catch (const std::bad_alloc&) { return {状态::资源失败, {}};
    } catch (const std::length_error&) { return {状态::资源失败, {}};
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"查询节点组完整结构",
            L"联合结构读取异常。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"查询节点组完整结构",
            L"联合结构读取未知异常。");
    }
}

基础字段组合变化结果 基础数据集::原子比较变更字段组(
    const 基础字段组合变化请求& 请求) noexcept {
    using 状态 = 基础字段组合变化状态;
    try {
        std::unique_lock 锁(互斥_);
        if (请求.预期节点组.empty()
            || (请求.删除字段关系组.empty() && 请求.替换字段组.empty()
                && 请求.追加字段组.empty())) return {状态::入口拒绝, {}};

        const auto 内容合法 = [](const 基础字段内容& 内容) noexcept {
            if (内容.valueless_by_exception()) return false;
            if (const auto* 引用 = std::get_if<稳定编码>(&内容)) return 有效(*引用);
            return !std::get<基础值>(内容).材料.valueless_by_exception();
        };
        const auto 引用目标 = [](const 基础字段内容& 内容) noexcept {
            return std::get_if<稳定编码>(&内容);
        };
        std::set<std::uint64_t> 预期节点;
        std::map<std::uint64_t, const 基础字段关系*> 预期字段;
        for (const auto& 快照 : 请求.预期节点组) {
            const auto 节点 = 快照.结构.节点.编码;
            if (!有效(节点) || !预期节点.insert(节点.值).second)
                return {状态::入口拒绝, {}};
            for (const auto& 字段 : 快照.结构.字段组) {
                if (!有效(字段.编码) || !有效(字段.字段节点)
                    || 字段.所属节点 != 节点 || !内容合法(字段.内容)
                    || !预期字段.emplace(字段.编码.值, &字段).second)
                    return {状态::入口拒绝, {}};
            }
        }
        std::set<std::uint64_t> 已触碰字段;
        for (const auto 编码 : 请求.删除字段关系组) {
            if (!有效(编码) || !已触碰字段.insert(编码.值).second)
                return {状态::入口拒绝, {}};
            const auto 位置 = 预期字段.find(编码.值);
            if (位置 == 预期字段.end()) return {状态::入口拒绝, {}};
            if (const auto* 目标 = 引用目标(位置->second->内容);
                目标 && !预期节点.contains(目标->值))
                return {状态::入口拒绝, {}};
        }
        for (const auto& 替换 : 请求.替换字段组) {
            if (!有效(替换.字段关系) || !内容合法(替换.新内容)
                || !已触碰字段.insert(替换.字段关系.值).second)
                return {状态::入口拒绝, {}};
            const auto 位置 = 预期字段.find(替换.字段关系.值);
            if (位置 == 预期字段.end()) return {状态::入口拒绝, {}};
            if (const auto* 目标 = 引用目标(位置->second->内容);
                目标 && !预期节点.contains(目标->值))
                return {状态::入口拒绝, {}};
            if (const auto* 目标 = 引用目标(替换.新内容);
                目标 && !预期节点.contains(目标->值))
                return {状态::入口拒绝, {}};
        }
        for (const auto& 追加 : 请求.追加字段组) {
            if (!有效(追加.所属节点) || !预期节点.contains(追加.所属节点.值)
                || !有效(追加.字段节点) || !内容合法(追加.内容))
                return {状态::入口拒绝, {}};
            if (const auto* 目标 = 引用目标(追加.内容);
                目标 && !预期节点.contains(目标->值))
                return {状态::入口拒绝, {}};
        }
        for (const auto& 快照 : 请求.预期节点组)
            if (!节点存在(快照.结构.节点.编码)) return {状态::节点不存在, {}};
        for (const auto& 快照 : 请求.预期节点组) {
            if (!联合快照相等(
                    读取联合快照已加锁(快照.结构.节点.编码), 快照))
                return {状态::预期不符, {}};
            // 已证明是当前字段，端点失效才属于底座不变量破损。
            for (const auto& 字段 : 快照.结构.字段组) {
                if (!节点存在(字段.字段节点))
                    throw std::logic_error("current field key node missing");
                if (const auto* 目标 = 引用目标(字段.内容);
                    目标 && !节点存在(*目标))
                    throw std::logic_error("current field target node missing");
            }
        }
        for (const auto& 追加 : 请求.追加字段组)
            if (!节点存在(追加.字段节点)) return {状态::引用不存在, {}};
        for (const auto 编码 : 已触碰字段) {
            const auto 位置 = 字段关系_.find(编码);
            if (位置 == 字段关系_.end()
                || 位置->second != *预期字段.at(编码))
                throw std::logic_error("current touched field disagrees with complete snapshot");
#if HY_EGO_CORE_INDEX_AUDIT
            核验字段索引已加锁(位置->second);
#endif
        }

        auto 候选字段 = 字段关系_;
        auto 候选所属索引 = 所属字段索引_;
        auto 候选目标索引 = 目标字段索引_;
        auto 候选入向索引 = 入向字段索引_;
        auto 候选字段键索引 = 字段节点索引_;
        基础字段组合变化结果 结果{状态::已完成, {}};
        结果.追加字段关系组.reserve(请求.追加字段组.size());
        const auto 移除候选字段 = [&](稳定编码 编码) {
            const auto 位置 = 候选字段.find(编码.值);
            if (位置 == 候选字段.end())
                throw std::logic_error("candidate field to remove missing");
            const auto& 字段 = 位置->second;
            删字段索引(字段,候选所属索引,候选目标索引,候选入向索引,候选字段键索引);
            候选字段.erase(位置);
        };
        for (const auto 编码 : 请求.删除字段关系组) 移除候选字段(编码);
        for (const auto& 替换 : 请求.替换字段组) {
            const auto 位置 = 候选字段.find(替换.字段关系.值);
            if (位置 == 候选字段.end())
                throw std::logic_error("candidate field to replace missing");
            const auto& 字段 = 位置->second;
            删字段索引(字段,候选所属索引,候选目标索引,候选入向索引,候选字段键索引);
            位置->second.内容 = 替换.新内容;
            加字段索引(位置->second,候选所属索引,候选目标索引,候选入向索引,候选字段键索引);
        }
        for (const auto& 追加 : 请求.追加字段组) {
            const auto 编码 = 分配编码();
            if (!有效(编码)) return {状态::编码耗尽, {}};
            if (节点_.contains(编码.值) || 外部关系_.contains(编码.值)
                || 候选字段.contains(编码.值))
                throw std::logic_error("allocated field identity collision");
            const auto [位置, 已插入] = 候选字段.emplace(编码.值,
                基础字段关系{编码, 追加.所属节点, 追加.字段节点, 追加.内容});
            if (!已插入) throw std::logic_error("candidate field identity collision");
            加字段索引(位置->second,候选所属索引,候选目标索引,候选入向索引,候选字段键索引);
            结果.追加字段关系组.push_back(编码);
        }

        static_assert(std::is_nothrow_swappable_v<decltype(字段关系_)>);
        static_assert(std::is_nothrow_swappable_v<decltype(所属字段索引_)>);
        static_assert(std::is_nothrow_swappable_v<decltype(目标字段索引_)>);
        static_assert(std::is_nothrow_move_constructible_v<基础字段组合变化结果>);
        字段关系_.swap(候选字段);
        所属字段索引_.swap(候选所属索引);
        目标字段索引_.swap(候选目标索引);
        入向字段索引_.swap(候选入向索引);
        字段节点索引_.swap(候选字段键索引);
        return 结果;
    } catch (const std::bad_alloc&) { return {状态::资源失败, {}};
    } catch (const std::length_error&) { return {状态::资源失败, {}};
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"原子比较变更字段组",
            L"完整预期字段组合变化发生工程异常，未形成业务否定。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"原子比较变更字段组",
            L"完整预期字段组合变化发生未知工程异常，未形成业务否定。");
    }
}

基础互引节点建立结果 基础数据集::原子建立互引节点(
    const 基础互引节点建立请求& 请求) noexcept {
    using 状态 = 基础互引节点操作状态;
    try {
        std::unique_lock 锁(互斥_);
        if (!请求.新节点.上级节点 || !有效(*请求.新节点.上级节点)
            || !有效(请求.反向引用所属节点)
            || !有效(请求.反向引用字段节点)) return {状态::入口拒绝, {}, {}, {}};
        std::set<std::uint64_t> 必需{
            请求.新节点.上级节点->值, 请求.反向引用所属节点.值,
            请求.反向引用字段节点.值};
        for (const auto& 字段 : 请求.新节点.字段组) {
            if (!有效(字段.字段节点) || 字段.内容.valueless_by_exception())
                return {状态::入口拒绝, {}, {}, {}};
            必需.insert(字段.字段节点.值);
            if (const auto* 引用 = std::get_if<稳定编码>(&字段.内容)) {
                if (!有效(*引用)) return {状态::入口拒绝, {}, {}, {}};
                必需.insert(引用->值);
            } else if (std::get<基础值>(字段.内容).材料.valueless_by_exception())
                return {状态::入口拒绝, {}, {}, {}};
        }
        const auto 预期状态 = 复核联合预期已加锁(请求.预期节点组, 必需);
        if (预期状态 != 状态::已完成) return {预期状态, {}, {}, {}};
        auto 候选节点 = 节点_;
        auto 候选字段 = 字段关系_;
        auto 候选关系 = 外部关系_;
        auto 候选源关系索引 = 源关系索引_;
        auto 候选目标关系索引 = 目标关系索引_;
        auto 候选所属索引 = 所属字段索引_;
        auto 候选目标索引 = 目标字段索引_;
        auto 候选入向索引 = 入向字段索引_;
        auto 候选字段键索引 = 字段节点索引_;
        const auto 编码已占用 = [&](稳定编码 编码) {
            return 候选节点.contains(编码.值) || 候选字段.contains(编码.值)
                || 候选关系.contains(编码.值);
        };
        const auto 节点 = 分配编码();
        if (!有效(节点)) return {状态::资源失败, {}, {}, {}};
        if (编码已占用(节点)) return {状态::内部不一致, {}, {}, {}};
        候选节点.emplace(节点.值, 基础节点{节点});
        const auto 加字段 = [&](稳定编码 所属, 稳定编码 键, 基础字段内容 内容)
            -> std::optional<稳定编码> {
            const auto 编码 = 分配编码();
            if (!有效(编码)) return std::nullopt;
            if (编码已占用(编码)) throw std::logic_error("mutual field identity collision");
            候选字段.emplace(编码.值, 基础字段关系{编码, 所属, 键, std::move(内容)});
            const auto& 字段 = 候选字段.at(编码.值);
            加字段索引(字段,候选所属索引,候选目标索引,候选入向索引,候选字段键索引);
            return 编码;
        };
        for (const auto& 字段 : 请求.新节点.字段组)
            if (!加字段(节点, 字段.字段节点, 字段.内容))
                return {状态::资源失败, {}, {}, {}};
        const auto 反向 = 加字段(请求.反向引用所属节点,
            请求.反向引用字段节点, 节点);
        if (!反向) return {状态::资源失败, {}, {}, {}};
        const auto 关系 = 分配编码();
        if (!有效(关系)) return {状态::资源失败, {}, {}, {}};
        if (编码已占用(关系)) return {状态::内部不一致, {}, {}, {}};
        const 基础外部关系 边{关系,*请求.新节点.上级节点,节点,基础外部关系类型::父子,0};
        候选关系.emplace(关系.值,边);
        加关系索引(边,候选源关系索引,候选目标关系索引);
        基础互引节点建立结果 结果{状态::已完成, 节点, *反向, 关系};
        节点_.swap(候选节点);
        字段关系_.swap(候选字段);
        外部关系_.swap(候选关系);
        源关系索引_.swap(候选源关系索引);
        目标关系索引_.swap(候选目标关系索引);
        所属字段索引_.swap(候选所属索引);
        目标字段索引_.swap(候选目标索引);
        入向字段索引_.swap(候选入向索引);
        字段节点索引_.swap(候选字段键索引);
        return 结果;
    } catch (const std::bad_alloc&) { return {状态::资源失败, {}, {}, {}};
    } catch (const std::length_error&) { return {状态::资源失败, {}, {}, {}};
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"原子建立互引节点",
            L"联合建立候选与索引异常。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"原子建立互引节点",
            L"联合建立未知异常。");
    }
}

基础互引节点操作状态 基础数据集::原子删除互引节点(
    const 基础互引节点删除请求& 请求) noexcept {
    using 状态 = 基础互引节点操作状态;
    try {
        std::unique_lock 锁(互斥_);
        if (!有效(请求.节点) || !有效(请求.上级节点)
            || !有效(请求.反向字段关系)) return 状态::入口拒绝;
        if (请求.节点 == 请求.上级节点) return 状态::入口拒绝;
        if (!节点存在(请求.节点)) return 状态::节点不存在;
        const auto 反向位置 = 字段关系_.find(请求.反向字段关系.值);
        if (反向位置 == 字段关系_.end()) return 状态::引用不存在;
        const auto& 反向 = 反向位置->second;
        const auto* 目标 = std::get_if<稳定编码>(&反向.内容);
        if (!目标 || *目标 != 请求.节点 || 反向.所属节点 == 请求.节点)
            return 状态::入口拒绝;
#if HY_EGO_CORE_INDEX_AUDIT
        核验字段索引已加锁(反向);
#endif
        std::set<std::uint64_t> 必需{
            请求.节点.值, 请求.上级节点.值,
            反向.所属节点.值, 反向.字段节点.值};
        std::vector<std::uint64_t> 待删字段;
        std::set<std::uint64_t> 相关字段,相关关系;
        收集三元节点索引(所属字段索引_,请求.节点.值,相关字段);
        收集二元节点索引(字段节点索引_,请求.节点.值,相关字段);
        收集二元节点索引(入向字段索引_,请求.节点.值,相关字段);
        收集三元节点索引(源关系索引_,请求.节点.值,相关关系);
        收集三元节点索引(目标关系索引_,请求.节点.值,相关关系);
        for (const auto 编码 : 相关字段) {
            const auto& 字段=字段关系_.at(编码);
            if (字段.字段节点 == 请求.节点) return 状态::引用仍存在;
            if (字段.所属节点 == 请求.节点) {
#if HY_EGO_CORE_INDEX_AUDIT
                核验字段索引已加锁(字段);
#endif
                if (const auto* 引用 = std::get_if<稳定编码>(&字段.内容);
                    引用 && *引用 == 请求.节点)
                    return 状态::引用仍存在;
                待删字段.push_back(编码);
                必需.insert(字段.字段节点.值);
                if (const auto* 引用 = std::get_if<稳定编码>(&字段.内容))
                    必需.insert(引用->值);
                continue;
            }
            if (编码 == 请求.反向字段关系.值) continue;
            if (const auto* 引用 = std::get_if<稳定编码>(&字段.内容);
                引用 && *引用 == 请求.节点) return 状态::引用仍存在;
        }
        std::optional<std::uint64_t> 父边;
        for (const auto 编码 : 相关关系) {
            const auto& 关系=外部关系_.at(编码);
            if (关系.目标节点 == 请求.节点
                && 关系.源节点 == 请求.上级节点
                && 关系.类型 == 基础外部关系类型::父子
                && 关系.角色或顺序 == 0) {
                if (父边) return 状态::内部不一致;
                父边 = 编码;
            } else if (关系.源节点 == 请求.节点 || 关系.目标节点 == 请求.节点) {
                return 状态::引用仍存在;
            }
        }
        if (!父边) return 状态::引用不存在;
        const auto 预期状态 = 复核联合预期已加锁(请求.预期节点组, 必需);
        if (预期状态 != 状态::已完成) return 预期状态;
        auto 候选节点 = 节点_;
        auto 候选字段 = 字段关系_;
        auto 候选关系 = 外部关系_;
        auto 候选源关系索引 = 源关系索引_;
        auto 候选目标关系索引 = 目标关系索引_;
        auto 候选所属索引 = 所属字段索引_;
        auto 候选目标索引 = 目标字段索引_;
        auto 候选入向索引 = 入向字段索引_;
        auto 候选字段键索引 = 字段节点索引_;
        const auto 删字段 = [&](const 基础字段关系& 字段) {
            删字段索引(字段,候选所属索引,候选目标索引,候选入向索引,候选字段键索引);
            if (候选字段.erase(字段.编码.值) != 1)
                throw std::logic_error("union delete field missing");
        };
        for (const auto 编码 : 待删字段) 删字段(字段关系_.at(编码));
        删字段(反向);
        删关系索引(外部关系_.at(*父边),候选源关系索引,候选目标关系索引);
        if (候选关系.erase(*父边) != 1
            || 候选节点.erase(请求.节点.值) != 1)
            throw std::logic_error("union delete node or edge missing");
        节点_.swap(候选节点);
        字段关系_.swap(候选字段);
        外部关系_.swap(候选关系);
        源关系索引_.swap(候选源关系索引);
        目标关系索引_.swap(候选目标关系索引);
        所属字段索引_.swap(候选所属索引);
        目标字段索引_.swap(候选目标索引);
        入向字段索引_.swap(候选入向索引);
        字段节点索引_.swap(候选字段键索引);
        return 状态::已完成;
    } catch (const std::bad_alloc&) { return 状态::资源失败;
    } catch (const std::length_error&) { return 状态::资源失败;
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"原子删除互引节点",
            L"联合删除候选与索引异常。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"原子删除互引节点",
            L"联合删除未知异常。");
    }
}

稳定编码 基础数据集::添加关系(稳定编码 源节点, 稳定编码 目标节点,
    基础外部关系类型 类型, std::int64_t 角色或顺序) noexcept {
    try {
        std::unique_lock 锁(互斥_);
        if (!节点存在(源节点) || !节点存在(目标节点) || !有效(类型)) return {};
        const auto 编码 = 分配编码();
        if (!有效(编码)) return {};
        插入关系已加锁(基础外部关系{编码,源节点,目标节点,类型,角色或顺序});
        return 编码;
    } catch (...) {
        return {};
    }
}

bool 基础数据集::修改关系(稳定编码 关系, 稳定编码 源节点,
    稳定编码 目标节点, 基础外部关系类型 类型,
    std::int64_t 角色或顺序) noexcept {
    try {
        std::unique_lock 锁(互斥_);
        if (!节点存在(源节点) || !节点存在(目标节点) || !有效(类型)) return false;
        const auto 位置 = 外部关系_.find(关系.值);
        if (位置 == 外部关系_.end()) return false;
        const auto 旧=位置->second;
        const 字段索引键 旧源{旧.源节点.值,static_cast<std::uint64_t>(旧.类型),关系.值};
        const 字段索引键 旧目标{旧.目标节点.值,static_cast<std::uint64_t>(旧.类型),关系.值};
        const 字段索引键 新源{源节点.值,static_cast<std::uint64_t>(类型),关系.值};
        const 字段索引键 新目标{目标节点.值,static_cast<std::uint64_t>(类型),关系.值};
        bool 已加源=false,已加目标=false;
        try {
            if(新源!=旧源) {已加源=源关系索引_.insert(新源).second;if(!已加源)throw std::logic_error("modified source edge index collision");}
            if(新目标!=旧目标) {已加目标=目标关系索引_.insert(新目标).second;if(!已加目标)throw std::logic_error("modified target edge index collision");}
        } catch(...) {
            if(已加目标)目标关系索引_.erase(新目标);
            if(已加源)源关系索引_.erase(新源);
            throw;
        }
        static_assert(std::is_nothrow_copy_assignable_v<基础外部关系>);
        位置->second = {关系, 源节点, 目标节点, 类型, 角色或顺序};
        if(新源!=旧源)源关系索引_.erase(旧源);
        if(新目标!=旧目标)目标关系索引_.erase(旧目标);
        return true;
    } catch (...) {
        return false;
    }
}

bool 基础数据集::删除关系(稳定编码 关系) noexcept {
    try {
        std::unique_lock 锁(互斥_);
        if(!有效(关系))return false;
        const auto it=外部关系_.find(关系.值);
        if(it==外部关系_.end())return false;
        删除关系已加锁(it->second);
        return true;
    } catch (...) {
        return false;
    }
}

std::optional<基础外部关系> 基础数据集::查询关系(稳定编码 关系) const noexcept {
    try {
        std::shared_lock 锁(互斥_);
        const auto 位置 = 外部关系_.find(关系.值);
        return 位置 == 外部关系_.end()
            ? std::nullopt : std::optional<基础外部关系>{位置->second};
    } catch (...) {
        return std::nullopt;
    }
}

std::vector<基础外部关系> 基础数据集::查询源关系(
    稳定编码 源节点, 基础外部关系类型 类型) const {
    std::shared_lock 锁(互斥_);
    std::vector<基础外部关系> 结果;
    if (!节点存在(源节点) || !有效(类型)) return 结果;
    读取节点关系已加锁(源节点,true,类型,结果);
    return 结果;
}

std::vector<基础外部关系> 基础数据集::查询目标关系(
    稳定编码 目标节点, 基础外部关系类型 类型) const {
    std::shared_lock 锁(互斥_);
    std::vector<基础外部关系> 结果;
    if (!节点存在(目标节点) || !有效(类型)) return 结果;
    读取节点关系已加锁(目标节点,false,类型,结果);
    return 结果;
}

稳定编码 基础数据集::添加字段值(
    稳定编码 节点, 稳定编码 字段节点, 基础值 值) noexcept {
    try {
        std::unique_lock 锁(互斥_);
        if (!节点存在(节点) || !节点存在(字段节点)) return {};
        const auto 编码 = 分配编码();
        if (!有效(编码)) return {};
        插入字段已加锁(基础字段关系{编码, 节点, 字段节点, std::move(值)});
        return 编码;
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"添加字段值", L"字段及索引插入异常，已撤销本次插项。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"添加字段值", L"字段及索引插入未知异常。");
    }
}

稳定编码 基础数据集::添加字段节点(
    稳定编码 节点, 稳定编码 字段节点, 稳定编码 目标节点) noexcept {
    try {
        std::unique_lock 锁(互斥_);
        if (!节点存在(节点) || !节点存在(字段节点) || !节点存在(目标节点)) return {};
        const auto 编码 = 分配编码();
        if (!有效(编码)) return {};
        插入字段已加锁(基础字段关系{编码, 节点, 字段节点, 目标节点});
        return 编码;
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"添加字段节点", L"引用字段及索引插入异常，已撤销本次插项。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"添加字段节点", L"引用字段及索引插入未知异常。");
    }
}

bool 基础数据集::修改字段值(稳定编码 字段关系, 基础值 值) noexcept {
    try {
        std::unique_lock 锁(互斥_);
        const auto 位置 = 字段关系_.find(字段关系.值);
        if (位置 == 字段关系_.end()) return false;
#if HY_EGO_CORE_INDEX_AUDIT
        核验字段索引已加锁(位置->second);
#endif
        static_assert(std::is_nothrow_swappable_v<基础字段内容>);
        基础字段内容 候选{std::move(值)};
        位置->second.内容.swap(候选);
        if (const auto* 旧引用 = std::get_if<稳定编码>(&候选)) {
            目标字段索引_.erase({位置->second.字段节点.值,旧引用->值,字段关系.值});
            入向字段索引_.erase({旧引用->值,字段关系.值});
        }
        return true;
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"修改字段值", L"字段内容及索引更新异常。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"修改字段值", L"字段内容及索引更新未知异常。");
    }
}

bool 基础数据集::修改字段节点(
    稳定编码 字段关系, 稳定编码 目标节点) noexcept {
    try {
        std::unique_lock 锁(互斥_);
        if (!节点存在(目标节点)) return false;
        const auto 位置 = 字段关系_.find(字段关系.值);
        if (位置 == 字段关系_.end()) return false;
#if HY_EGO_CORE_INDEX_AUDIT
        核验字段索引已加锁(位置->second);
#endif
        const auto& 字段 = 位置->second;
        const auto* 旧引用 = std::get_if<稳定编码>(&字段.内容);
        if (旧引用 && *旧引用 == 目标节点) return true;
        const std::optional<字段索引键> 旧键 = 旧引用
            ? std::optional<字段索引键>{{字段.字段节点.值, 旧引用->值, 字段关系.值}} : std::nullopt;
        基础字段内容 候选{目标节点};
        bool 已加反向=false,已加入向=false;
        try {
            已加反向=目标字段索引_.emplace(字段.字段节点.值,目标节点.值,字段关系.值).second;
            if(!已加反向)throw std::logic_error("modified reverse field index collision");
            已加入向=入向字段索引_.emplace(目标节点.值,字段关系.值).second;
            if(!已加入向)throw std::logic_error("modified target-first field index collision");
        } catch(...) {
            if(已加入向)入向字段索引_.erase({目标节点.值,字段关系.值});
            if(已加反向)目标字段索引_.erase({字段.字段节点.值,目标节点.值,字段关系.值});
            throw;
        }
        static_assert(std::is_nothrow_swappable_v<基础字段内容>);
        位置->second.内容.swap(候选);
        if (旧键) {
            目标字段索引_.erase(*旧键);
            入向字段索引_.erase({std::get<1>(*旧键),字段关系.值});
        }
        return true;
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"修改字段节点", L"引用目标及索引更新异常。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"修改字段节点", L"引用目标及索引更新未知异常。");
    }
}

bool 基础数据集::删除字段(稳定编码 字段关系) noexcept {
    try {
        std::unique_lock 锁(互斥_);
        if (!有效(字段关系)) return false;
        const auto 位置 = 字段关系_.find(字段关系.值);
        if (位置 == 字段关系_.end()) return false;
#if HY_EGO_CORE_INDEX_AUDIT
        核验字段索引已加锁(位置->second);
#endif
        删除字段已加锁(位置->second);
        return true;
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"删除字段", L"字段及索引删除异常。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"删除字段", L"字段及索引删除未知异常。");
    }
}

std::optional<基础字段关系> 基础数据集::查询字段关系(
    稳定编码 字段关系) const noexcept {
    try {
        std::shared_lock 锁(互斥_);
        const auto 位置 = 字段关系_.find(字段关系.值);
        return 位置 == 字段关系_.end()
            ? std::nullopt : std::optional<基础字段关系>{位置->second};
    } catch (...) {
        return std::nullopt;
    }
}

std::vector<基础字段关系> 基础数据集::查询字段(
    稳定编码 节点, 稳定编码 字段节点) const {
    std::shared_lock 锁(互斥_);
    std::vector<基础字段关系> 结果;
    if (!节点存在(节点) || !节点存在(字段节点)) return 结果;
    for (auto 位置 = 所属字段索引_.lower_bound({节点.值, 字段节点.值, 0});
        位置 != 所属字段索引_.end() && std::get<0>(*位置) == 节点.值
            && std::get<1>(*位置) == 字段节点.值; ++位置)
        结果.push_back(字段关系_.at(std::get<2>(*位置)));
    return 结果;
}

std::vector<基础字段关系> 基础数据集::查询目标字段(
    稳定编码 目标节点, 稳定编码 字段节点) const {
    std::shared_lock 锁(互斥_);
    std::vector<基础字段关系> 结果;
    if (!节点存在(目标节点) || !节点存在(字段节点)) return 结果;
    for (auto 位置 = 目标字段索引_.lower_bound({字段节点.值, 目标节点.值, 0});
        位置 != 目标字段索引_.end() && std::get<0>(*位置) == 字段节点.值
            && std::get<1>(*位置) == 目标节点.值; ++位置)
        结果.push_back(字段关系_.at(std::get<2>(*位置)));
    return 结果;
}

} // namespace 海中鱼巣
