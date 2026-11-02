#include "概念_存在类.h"
#include "../核心/容错检查.h"
#include "../核心/日志系统.h"
#include <sstream>

#include <algorithm>
#include <functional>
#include <new>
#include <array>
#include <limits>
#include <map>
#include <mutex>
#include <new>
#include <set>
#include <stdexcept>
#include <functional>

namespace 海中鱼巣 {

稳定编码 存在概念图根节点{};

namespace 领域同步内部 {
extern std::recursive_mutex 存在特征互斥;
}

namespace {

// 归类专属结构由本概念 owner 持有；不进入普通完整定义的值域/实例链。
稳定编码 归类未知类型{},归类上位字段{},归类词条字段{},归类需求字段{},归类回答字段{},归类登记字段{},归类名称字段{};
auto 语言归类字段组() {return std::array{&归类未知类型,&归类上位字段,&归类词条字段,&归类需求字段,&归类回答字段,&归类登记字段,&归类名称字段};}
bool 归类同快照(const 基础联合节点快照&a,const 基础联合节点快照&b){return a.结构.节点==b.结构.节点&&a.结构.字段组==b.结构.字段组&&a.入向字段组==b.入向字段组&&a.入向外部关系组==b.入向外部关系组&&a.出向外部关系组==b.出向外部关系组;}
bool 归类并保护(std::vector<基础联合节点快照>&a,const std::vector<基础联合节点快照>&b){for(const auto&n:b){const auto it=std::find_if(a.begin(),a.end(),[&](const auto&x){return x.结构.节点.编码==n.结构.节点.编码;});if(it==a.end())a.push_back(n);else if(!归类同快照(*it,n))return false;}return true;}
std::optional<基础联合节点快照> 归类取快照(稳定编码 n){const auto r=全局基础数据集.查询节点组完整结构({n});if(r.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};if(r.状态!=基础结构操作状态::已完成||r.节点组.size()!=1)throw std::logic_error("classification identity snapshot failed");return r.节点组.front().快照;}
bool 归类保护未变(const std::vector<基础联合节点快照>&p){for(const auto&n:p){const auto s=归类取快照(n.结构.节点.编码);if(!s||!归类同快照(n,*s))return false;}return true;}
std::vector<稳定编码> 归类引用组(const 基础节点结构&s,稳定编码 key){std::vector<稳定编码>r;for(const auto&f:s.字段组)if(f.字段节点==key){const auto*v=std::get_if<稳定编码>(&f.内容);if(!v||!有效(*v))throw std::logic_error("classification identity reference field corrupt");r.push_back(*v);}return r;}
稳定编码 归类单引用(const 基础节点结构&s,稳定编码 key){const auto r=归类引用组(s,key);if(r.size()!=1)throw std::logic_error("classification identity cardinality corrupt");return r.front();}

extern 稳定编码 节点类型字段;
std::vector<稳定编码> 归类直接子投影(稳定编码 root,稳定编码 parent,std::vector<基础联合节点快照>&protect){
    const auto r=归类取快照(root);if(!r)throw std::logic_error("classification registry root absent");if(!归类并保护(protect,{*r}))throw 归类概念读取状态::前提已变化;
    std::vector<稳定编码> children;
    for(const auto C:归类引用组(r->结构,归类登记字段)){
        const auto c=归类取快照(C);if(!c||归类单引用(c->结构,节点类型字段)!=归类未知类型)throw std::logic_error("classification registered identity corrupt");
        if(归类单引用(c->结构,归类上位字段)!=parent)continue;
        if(!归类并保护(protect,{*c}))throw 归类概念读取状态::前提已变化;
        children.push_back(C);
    }
    std::sort(children.begin(),children.end());if(std::adjacent_find(children.begin(),children.end())!=children.end())throw std::logic_error("classification child registered twice");return children;
}


std::recursive_mutex& 存在概念图互斥 = 领域同步内部::存在特征互斥;

稳定编码 节点类型字段{};
稳定编码 专属存在概念节点类型{};
稳定编码 抽象存在概念节点类型{};
稳定编码 对应存在字段{};
稳定编码 特征项字段{};
稳定编码 特征项节点类型{};
稳定编码 特征类型概念字段{};
稳定编码 具体特征概念字段{};
稳定编码 名称关系字段{};
稳定编码 存在语义根字段{};
bool 是登记存在根(稳定编码 节点);

constexpr std::int64_t 组织归属关系角色 = 1;
constexpr std::int64_t 概念上下位关系角色 = 2;

bool 节点仍存在(稳定编码 节点) noexcept {
    return 有效(节点) && 全局基础数据集.查询节点(节点).has_value();
}

bool 结构已初始化() noexcept {
    return 有效(存在概念图根节点)
        && 有效(节点类型字段)
        && 有效(专属存在概念节点类型)
        && 有效(抽象存在概念节点类型)
        && 有效(对应存在字段)
        && 有效(特征项字段)
        && 有效(特征项节点类型)
        && 有效(特征类型概念字段)
        && 有效(具体特征概念字段)
        && 有效(名称关系字段) && 有效(存在语义根字段);
}

bool 结构仍存在() noexcept {
    return 结构已初始化()
        && 节点仍存在(存在概念图根节点)
        && 节点仍存在(节点类型字段)
        && 节点仍存在(专属存在概念节点类型)
        && 节点仍存在(抽象存在概念节点类型)
        && 节点仍存在(对应存在字段)
        && 节点仍存在(特征项字段)
        && 节点仍存在(特征项节点类型)
        && 节点仍存在(特征类型概念字段)
        && 节点仍存在(具体特征概念字段)
        && 节点仍存在(名称关系字段) && 节点仍存在(存在语义根字段);
}

std::optional<稳定编码> 读取唯一节点字段(
    稳定编码 节点, 稳定编码 字段) {
    const auto 字段组 = 全局基础数据集.查询字段(节点, 字段);
    if (字段组.size() != 1) return std::nullopt;
    const auto* 目标 = std::get_if<稳定编码>(&字段组.front().内容);
    return 目标 ? std::optional<稳定编码>{*目标} : std::nullopt;
}

std::optional<存在概念种类> 读取概念种类(稳定编码 节点) {
    const auto 类型 = 读取唯一节点字段(节点, 节点类型字段);
    if (!类型) return std::nullopt;
    if (*类型 == 专属存在概念节点类型) return 存在概念种类::专属;
    if (*类型 == 抽象存在概念节点类型) return 存在概念种类::抽象;
    return std::nullopt;
}

void 排序去重(std::vector<稳定编码>& 节点组) {
    std::sort(节点组.begin(), 节点组.end());
    节点组.erase(std::unique(节点组.begin(), 节点组.end()), 节点组.end());
}

void 排序特征项(std::vector<存在概念特征项>& 特征组) {
    std::sort(特征组.begin(), 特征组.end(), [](const auto& 左, const auto& 右) {
        return 左.特征类型概念 < 右.特征类型概念
            || (左.特征类型概念 == 右.特征类型概念
                && 左.具体特征概念 < 右.具体特征概念);
    });
}

std::optional<std::vector<存在概念特征项>> 规范化特征组(
    const std::vector<稳定编码>& 具体特征概念组,
    概念_特征类& 特征概念服务) {
    std::vector<存在概念特征项> 结果;
    for (const auto 具体概念 : 具体特征概念组) {
        if (!特征概念服务.是特征概念节点(具体概念)) return std::nullopt;
        const auto 定位 = 特征概念服务.比较单特征概念(
            具体概念, 具体概念);
        if (定位.状态 != 特征概念规则状态::已完成
            || !定位.特征类型根) {
            return std::nullopt;
        }
        结果.push_back({*定位.特征类型根, 具体概念});
    }
    排序特征项(结果);
    for (std::size_t i = 1; i < 结果.size(); ++i) {
        if (结果[i - 1].特征类型概念 == 结果[i].特征类型概念) {
            return std::nullopt;
        }
    }
    return 结果;
}

std::optional<std::vector<存在概念特征项>> 读取特征组(
    稳定编码 概念节点,
    概念_特征类& 特征概念服务) {
    std::vector<存在概念特征项> 结果;
    for (const auto& 持有字段 : 全局基础数据集.查询字段(
        概念节点, 特征项字段)) {
        const auto* 特征项节点 = std::get_if<稳定编码>(&持有字段.内容);
        if (!特征项节点 || !节点仍存在(*特征项节点)) return std::nullopt;
        const auto 项类型 = 读取唯一节点字段(*特征项节点, 节点类型字段);
        const auto 特征类型 = 读取唯一节点字段(
            *特征项节点, 特征类型概念字段);
        const auto 具体概念 = 读取唯一节点字段(
            *特征项节点, 具体特征概念字段);
        if (!项类型 || *项类型 != 特征项节点类型
            || !特征类型 || !具体概念
            || !特征概念服务.是特征概念节点(*具体概念)) {
            return std::nullopt;
        }
        const auto 定位 = 特征概念服务.比较单特征概念(
            *具体概念, *具体概念);
        if (定位.状态 != 特征概念规则状态::已完成
            || !定位.特征类型根 || *定位.特征类型根 != *特征类型) {
            return std::nullopt;
        }
        结果.push_back({*特征类型, *具体概念});
    }
    排序特征项(结果);
    for (std::size_t i = 1; i < 结果.size(); ++i) {
        if (结果[i - 1].特征类型概念 == 结果[i].特征类型概念) {
            return std::nullopt;
        }
    }
    return 结果;
}

struct 已建特征项 final {
    稳定编码 节点;
    std::vector<稳定编码> 字段组;
    std::optional<稳定编码> 持有字段;
};

std::optional<std::vector<已建特征项>> 建立特征项节点组(
    const std::vector<存在概念特征项>& 特征组) {
    std::vector<已建特征项> 结果;
    const auto 清理 = [&]() noexcept {
        for (auto 位置 = 结果.rbegin(); 位置 != 结果.rend(); ++位置) {
            for (auto 字段 = 位置->字段组.rbegin();
                字段 != 位置->字段组.rend(); ++字段) {
                (void)全局基础数据集.删除字段(*字段);
            }
            (void)全局基础数据集.删除节点(位置->节点);
        }
    };
    for (const auto& 特征 : 特征组) {
        已建特征项 已建;
        已建.节点 = 全局基础数据集.新建节点();
        if (!有效(已建.节点)) {
            清理();
            return std::nullopt;
        }
        const auto 类型字段 = 全局基础数据集.添加字段节点(
            已建.节点, 节点类型字段, 特征项节点类型);
        if (有效(类型字段)) 已建.字段组.push_back(类型字段);
        const auto 类型概念字段 = 有效(类型字段)
            ? 全局基础数据集.添加字段节点(
                已建.节点, 特征类型概念字段, 特征.特征类型概念)
            : 稳定编码{};
        if (有效(类型概念字段)) 已建.字段组.push_back(类型概念字段);
        const auto 具体概念字段 = 有效(类型概念字段)
            ? 全局基础数据集.添加字段节点(
                已建.节点, 具体特征概念字段, 特征.具体特征概念)
            : 稳定编码{};
        if (有效(具体概念字段)) 已建.字段组.push_back(具体概念字段);
        结果.push_back(std::move(已建));
        if (!有效(具体概念字段)) {
            清理();
            return std::nullopt;
        }
    }
    return 结果;
}

void 删除特征项节点组(std::vector<已建特征项>& 特征项组) noexcept {
    for (auto 位置 = 特征项组.rbegin(); 位置 != 特征项组.rend(); ++位置) {
        if (位置->持有字段) {
            (void)全局基础数据集.删除字段(*位置->持有字段);
        }
        for (auto 字段 = 位置->字段组.rbegin(); 字段 != 位置->字段组.rend(); ++字段) {
            (void)全局基础数据集.删除字段(*字段);
        }
        (void)全局基础数据集.删除节点(位置->节点);
    }
}

bool 挂接特征项节点组(
    稳定编码 概念节点,
    std::vector<已建特征项>& 特征项组) noexcept {
    for (auto& 特征项 : 特征项组) {
        const auto 持有字段 = 全局基础数据集.添加字段节点(
            概念节点, 特征项字段, 特征项.节点);
        if (!有效(持有字段)) return false;
        特征项.持有字段 = 持有字段;
    }
    return true;
}

void 删除既有特征项(稳定编码 概念节点) noexcept {
    for (const auto& 持有字段 : 全局基础数据集.查询字段(
        概念节点, 特征项字段)) {
        const auto* 特征项节点 = std::get_if<稳定编码>(&持有字段.内容);
        (void)全局基础数据集.删除字段(持有字段.编码);
        if (!特征项节点) continue;
        for (const auto 字段 : {
            节点类型字段, 特征类型概念字段, 具体特征概念字段}) {
            for (const auto& 关系 : 全局基础数据集.查询字段(
                *特征项节点, 字段)) {
                (void)全局基础数据集.删除字段(关系.编码);
            }
        }
        (void)全局基础数据集.删除节点(*特征项节点);
    }
}

std::vector<基础外部关系> 查询概念关系(
    稳定编码 节点, bool 查询上位) {
    std::vector<基础外部关系> 结果;
    const auto 关系组 = 查询上位
        ? 全局基础数据集.查询目标关系(节点, 基础外部关系类型::父子)
        : 全局基础数据集.查询源关系(节点, 基础外部关系类型::父子);
    for (const auto& 关系 : 关系组) {
        if (关系.角色或顺序 == 概念上下位关系角色) 结果.push_back(关系);
    }
    return 结果;
}

bool 可由上位包含(
    const std::vector<存在概念特征项>& 上位,
    const std::vector<存在概念特征项>& 下位,
    概念_特征类& 特征概念服务) {
    if (上位.size() > 下位.size()) return false;
    for (const auto& 上位项 : 上位) {
        const auto 下位位置 = std::find_if(下位.begin(), 下位.end(),
            [&](const auto& 项) {
                return 项.特征类型概念 == 上位项.特征类型概念;
            });
        if (下位位置 == 下位.end()) return false;
        const auto 关系 = 特征概念服务.比较单特征概念(
            上位项.具体特征概念, 下位位置->具体特征概念);
        if (关系.状态 != 特征概念规则状态::已完成 || !关系.关系) {
            return false;
        }
        if (*关系.关系 != 单特征概念关系::同一
            && *关系.关系 != 单特征概念关系::左为上位) {
            return false;
        }
    }
    return true;
}

bool 向下可达(稳定编码 起点, 稳定编码 目标) {
    std::vector<稳定编码> 待访问{起点};
    std::set<std::uint64_t> 已访问;
    while (!待访问.empty()) {
        const auto 当前 = 待访问.back();
        待访问.pop_back();
        if (当前 == 目标) return true;
        if (!已访问.insert(当前.值).second) continue;
        for (const auto& 关系 : 查询概念关系(当前, false)) {
            待访问.push_back(关系.目标节点);
        }
    }
    return false;
}

std::optional<稳定编码> 共同上位特征概念(
    稳定编码 左,
    稳定编码 右,
    概念_特征类& 特征概念服务) {
    const auto 比较 = 特征概念服务.比较单特征概念(左, 右);
    if (比较.状态 != 特征概念规则状态::已完成 || !比较.关系) {
        return std::nullopt;
    }
    switch (*比较.关系) {
    case 单特征概念关系::同一:
    case 单特征概念关系::左为上位:
        return 左;
    case 单特征概念关系::右为上位:
        return 右;
    case 单特征概念关系::具有共同上位:
        return 比较.最近共同上位;
    case 单特征概念关系::无可比关系:
        return std::nullopt;
    }
    return std::nullopt;
}

} // namespace

专属存在概念身份保护结果 概念_存在类::读取专属存在概念身份保护(
    稳定编码 ec, 稳定编码 e, const 基础联合节点快照& 首读) const noexcept {
    using S = 专属存在概念身份读取状态;
    try {
        if (!有效(ec) || !有效(e) || ec == e || 首读.结构.节点.编码 != e) return {};
        if (!结构已初始化()) return {S::提供者未就绪, {}, {}, {}};
        // 此窄入口只读初始化后不变的schema及L1，不取得任何业务owner锁。
        const auto b = 全局基础数据集.查询节点组完整结构({e, ec});
        if (b.状态 == 基础结构操作状态::资源失败) return {S::资源失败, {}, {}, {}};
        if (b.状态 == 基础结构操作状态::节点不存在) return {S::节点不存在, {}, {}, {}};
        if (b.状态 != 基础结构操作状态::已完成 || b.节点组.size() != 2)
            return {S::结构不一致, {}, {}, {}};
        const 基础联合节点快照* es = nullptr;
        const 基础联合节点快照* cs = nullptr;
        for (const auto& n : b.节点组) {
            if (!n.快照) return {S::节点不存在, {}, {}, {}};
            if (n.查询节点 == e && !es) es = &*n.快照;
            else if (n.查询节点 == ec && !cs) cs = &*n.快照;
            else return {S::结构不一致, {}, {}, {}};
        }
        if (!es || !cs || cs->结构.节点.编码 != ec || es->结构.节点.编码 != e)
            return {S::结构不一致, {}, {}, {}};
        if (es->结构.字段组 != 首读.结构.字段组
            || es->入向字段组 != 首读.入向字段组
            || es->入向外部关系组 != 首读.入向外部关系组
            || es->出向外部关系组 != 首读.出向外部关系组)
            return {S::前提已变化, {}, {}, {}};
        unsigned 类型数 = 0, 归属数 = 0;
        for (const auto& f : cs->结构.字段组) {
            if (f.字段节点 != 节点类型字段 && f.字段节点 != 对应存在字段) continue;
            const auto* v = std::get_if<稳定编码>(&f.内容);
            if (!v) return {S::结构不一致, {}, {}, {}};
            if (f.字段节点 == 节点类型字段) {
                ++类型数;
                if (*v != 专属存在概念节点类型) return {S::结构不一致, {}, {}, {}};
            } else {
                ++归属数;
                if (*v != e) return {S::结构不一致, {}, {}, {}};
            }
        }
        if (类型数 != 1 || 归属数 != 1) return {S::结构不一致, {}, {}, {}};
        return {S::已完成, ec, e, {*es, *cs}};
    } catch (const std::bad_alloc&) { return {S::资源失败, {}, {}, {}}; }
    catch (...) { return {S::结构不一致, {}, {}, {}}; }
}

概念_存在类::概念_存在类(概念_特征类& 特征概念服务) noexcept
    : 特征概念服务_(特征概念服务) {}

bool 概念_存在类::初始化() noexcept {
    try {
        std::lock_guard 锁(存在概念图互斥);
        if (结构已初始化()) return 结构仍存在();
        auto 建立 = [](稳定编码& 节点) {
            if (有效(节点)) return 节点仍存在(节点);
            节点 = 全局基础数据集.新建节点();
            return 有效(节点);
        };
        for(auto* key:语言归类字段组())if(!建立(*key))return false;

        return 建立(存在概念图根节点)
            && 建立(节点类型字段)
            && 建立(专属存在概念节点类型)
            && 建立(抽象存在概念节点类型)
            && 建立(对应存在字段)
            && 建立(特征项字段)
            && 建立(特征项节点类型)
            && 建立(特征类型概念字段)
            && 建立(具体特征概念字段)
            && 建立(名称关系字段) && 建立(存在语义根字段);
    } catch (...) {
        return false;
    }
}

存在概念建立结果 概念_存在类::建立专属存在概念(
    稳定编码 实际存在节点,
    const std::vector<稳定编码>& 具体特征概念组) noexcept {
    存在概念建立结果 结果;
    try {
        std::lock_guard 锁(存在概念图互斥);
        if (!结构仍存在() || !节点仍存在(实际存在节点)) return 结果;
        for (const auto& 组织关系 : 全局基础数据集.查询源关系(
            存在概念图根节点, 基础外部关系类型::父子)) {
            if (组织关系.角色或顺序 != 组织归属关系角色
                || !读取概念种类(组织关系.目标节点)
                || 读取概念种类(组织关系.目标节点)
                    != 存在概念种类::专属) {
                continue;
            }
            const auto 已有对应存在 = 读取唯一节点字段(
                组织关系.目标节点, 对应存在字段);
            if (已有对应存在 && *已有对应存在 == 实际存在节点) {
                结果.状态 = 存在概念操作状态::结构不一致;
                return 结果;
            }
        }
        const auto 特征组 = 规范化特征组(
            具体特征概念组, 特征概念服务_);
        if (!特征组) {
            结果.状态 = 存在概念操作状态::特征概念不一致;
            return 结果;
        }

        const auto 节点 = 全局基础数据集.新建节点();
        if (!有效(节点)) {
            结果.状态 = 存在概念操作状态::资源失败;
            return 结果;
        }
        std::vector<稳定编码> 已写字段;
        const auto 类型字段 = 全局基础数据集.添加字段节点(
            节点, 节点类型字段, 专属存在概念节点类型);
        if (有效(类型字段)) 已写字段.push_back(类型字段);
        const auto 存在字段 = 有效(类型字段)
            ? 全局基础数据集.添加字段节点(
                节点, 对应存在字段, 实际存在节点)
            : 稳定编码{};
        if (有效(存在字段)) 已写字段.push_back(存在字段);
        auto 特征项组 = 有效(存在字段)
            ? 建立特征项节点组(*特征组) : std::nullopt;
        const bool 特征完成 = 特征项组
            && 挂接特征项节点组(节点, *特征项组);
        const auto 组织关系 = 特征完成
            ? 全局基础数据集.添加关系(
                存在概念图根节点, 节点,
                基础外部关系类型::父子, 组织归属关系角色)
            : 稳定编码{};
        if (有效(组织关系)) {
            结果.状态 = 存在概念操作状态::已建立;
            结果.概念节点 = 节点;
            return 结果;
        }
        if (特征项组) 删除特征项节点组(*特征项组);
        for (auto 位置 = 已写字段.rbegin(); 位置 != 已写字段.rend(); ++位置) {
            (void)全局基础数据集.删除字段(*位置);
        }
        (void)全局基础数据集.删除节点(节点);
        结果.状态 = 存在概念操作状态::资源失败;
        return 结果;
    } catch (...) {
        结果.状态 = 存在概念操作状态::资源失败;
        return 结果;
    }
}

存在概念操作状态 概念_存在类::更新专属存在概念(
    稳定编码 专属概念节点,
    const std::vector<稳定编码>& 具体特征概念组) noexcept {
    try {
        std::lock_guard 锁(存在概念图互斥);
        if (!是专属存在概念节点(专属概念节点)) {
            return 存在概念操作状态::概念不存在;
        }
        const auto 新特征组 = 规范化特征组(
            具体特征概念组, 特征概念服务_);
        const auto 原特征组 = 读取特征组(
            专属概念节点, 特征概念服务_);
        if (!新特征组 || !原特征组) {
            return 存在概念操作状态::特征概念不一致;
        }
        if (*新特征组 == *原特征组) return 存在概念操作状态::无变化;

        auto 新特征项组 = 建立特征项节点组(*新特征组);
        if (!新特征项组
            || !挂接特征项节点组(专属概念节点, *新特征项组)) {
            if (新特征项组) 删除特征项节点组(*新特征项组);
            return 存在概念操作状态::资源失败;
        }

        const auto 全部持有字段 = 全局基础数据集.查询字段(
            专属概念节点, 特征项字段);
        std::set<std::uint64_t> 新字段身份;
        for (const auto& 项 : *新特征项组) {
            if (项.持有字段) 新字段身份.insert(项.持有字段->值);
        }
        for (const auto& 字段 : 全部持有字段) {
            if (新字段身份.contains(字段.编码.值)) continue;
            const auto* 旧项节点 = std::get_if<稳定编码>(&字段.内容);
            (void)全局基础数据集.删除字段(字段.编码);
            if (!旧项节点) continue;
            for (const auto 项字段 : {
                节点类型字段, 特征类型概念字段, 具体特征概念字段}) {
                for (const auto& 关系 : 全局基础数据集.查询字段(
                    *旧项节点, 项字段)) {
                    (void)全局基础数据集.删除字段(关系.编码);
                }
            }
            (void)全局基础数据集.删除节点(*旧项节点);
        }

        for (const auto& 上位关系 : 查询概念关系(专属概念节点, true)) {
            const auto 上位特征组 = 读取特征组(
                上位关系.源节点, 特征概念服务_);
            if (!上位特征组
                || !可由上位包含(*上位特征组, *新特征组, 特征概念服务_)) {
                (void)全局基础数据集.删除关系(上位关系.编码);
            }
        }
        return 存在概念操作状态::已更新;
    } catch (...) {
        return 存在概念操作状态::资源失败;
    }
}

void 概念_存在类::核验初建单特征专属概念可回收(
    稳定编码 专属概念节点, 稳定编码 对应存在节点,
    const 存在概念特征项& 预期特征项,
    const std::vector<稳定编码>& 允许外部入向字段关系组) const noexcept {
    try {
        std::lock_guard 锁(存在概念图互斥);
        if (!结构仍存在() || !节点仍存在(对应存在节点))
            throw std::runtime_error("initial private existence concept identity invalid");
        const auto 概念 = 获取存在概念(专属概念节点);
        if (!概念 || 概念->种类 != 存在概念种类::专属
            || 概念->对应存在节点 != 对应存在节点
            || 概念->特征组 != std::vector<存在概念特征项>{预期特征项}
            || !概念->名称关系.empty() || !概念->上位概念组.empty()
            || !概念->下位概念组.empty())
            throw std::runtime_error("private existence concept is not an unchanged single-item projection");
        const auto 特征项 = 读取唯一节点字段(专属概念节点, 特征项字段);
        if (!特征项 || *特征项 == 专属概念节点)
            throw std::runtime_error("private existence concept item identity invalid");
        const std::vector<稳定编码> 节点组{专属概念节点, *特征项};
        const auto 完整组 = 全局基础数据集.查询节点组完整结构(节点组);
        if (完整组.状态 != 基础结构操作状态::已完成
            || 完整组.节点组.size() != 节点组.size())
            throw std::runtime_error("private existence concept complete structure unavailable");
        std::vector<稳定编码> 自有字段;
        std::vector<稳定编码> 结构关系;
        const auto 核对字段 = [&](const 基础联合节点快照& 快照,
            const std::vector<基础节点初始字段>& 预期) {
            if (快照.结构.字段组.size() != 预期.size())
                throw std::runtime_error("private existence concept has extra fields");
            for (const auto& 项 : 预期) {
                const 基础字段关系* 命中 = nullptr;
                for (const auto& 字段 : 快照.结构.字段组) {
                    if (字段.字段节点 != 项.字段节点) continue;
                    if (命中 || 字段.所属节点 != 快照.结构.节点.编码 || 字段.内容 != 项.内容)
                        throw std::runtime_error("private existence concept field content/cardinality changed");
                    命中 = &字段;
                }
                if (!命中) throw std::runtime_error("private existence concept required field missing");
                自有字段.push_back(命中->编码);
            }
        };
        for (const auto& 项 : 完整组.节点组) {
            if (!项.快照) throw std::runtime_error("private existence concept private node missing");
            const auto& 快照 = *项.快照;
            const auto 节点 = 快照.结构.节点.编码;
            if (节点 == 专属概念节点) {
                核对字段(快照, {{节点类型字段, 专属存在概念节点类型},
                    {对应存在字段, 对应存在节点}, {特征项字段, *特征项}});
                if (快照.入向外部关系组.size() != 1 || !快照.出向外部关系组.empty())
                    throw std::runtime_error("private existence concept organization is not exclusive");
                const auto& 关系 = 快照.入向外部关系组.front();
                if (关系.源节点 != 存在概念图根节点 || 关系.目标节点 != 专属概念节点
                    || 关系.类型 != 基础外部关系类型::父子
                    || 关系.角色或顺序 != 组织归属关系角色)
                    throw std::runtime_error("private existence concept organization edge invalid");
                结构关系.push_back(关系.编码);
            } else if (节点 == *特征项) {
                核对字段(快照, {{节点类型字段, 特征项节点类型},
                    {特征类型概念字段, 预期特征项.特征类型概念},
                    {具体特征概念字段, 预期特征项.具体特征概念}});
                if (!快照.入向外部关系组.empty() || !快照.出向外部关系组.empty())
                    throw std::runtime_error("private existence concept item has external relation");
            } else throw std::runtime_error("private existence concept query returned unexpected identity");
        }
        // 允许组仅指向EC根，不能豁免其私有特征项的任何外部消费。
        for (const auto 编码 : 允许外部入向字段关系组) {
            const auto 字段 = 全局基础数据集.查询字段关系(编码);
            const auto* 目标 = 字段 ? std::get_if<稳定编码>(&字段->内容) : nullptr;
            if (!目标 || *目标 != 专属概念节点)
                throw std::runtime_error("private existence concept allowed reference targets private item");
        }
        if (全局基础数据集.检查节点结构引用闭包(
                节点组, 自有字段, 结构关系, 允许外部入向字段关系组)
            != 基础结构操作状态::已完成)
            throw std::runtime_error("private existence concept closure is consumed or changed");
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"核验初建单特征专属概念可回收",
            L"初建专属概念及私有特征项不能完整证明未消费；未执行删除。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"核验初建单特征专属概念可回收",
            L"完整回收预检发生未知工程异常；未执行删除。");
    }
}

bool 概念_存在类::撤销未绑定专属存在概念(
    稳定编码 专属概念节点,
    稳定编码 实际存在节点) noexcept {
    try {
        std::lock_guard 锁(存在概念图互斥);
        if (!有效(专属概念节点) || !有效(实际存在节点)) return false;
        const auto 入口读 = 全局基础数据集.查询节点组完整结构({专属概念节点});
        if (入口读.状态 != 基础结构操作状态::已完成 || 入口读.节点组.size()!=1)
            throw std::logic_error("exclusive concept entry read failed");
        if (!入口读.节点组[0].快照) return false;
        std::optional<稳定编码> 入口类型;
        bool 有所属 = false;
        for (const auto& f : 入口读.节点组[0].快照->结构.字段组) {
            if (f.字段节点 == 对应存在字段) 有所属 = true;
            if (f.字段节点 != 节点类型字段) continue;
            const auto* n = std::get_if<稳定编码>(&f.内容);
            if (!n || !有效(*n) || 入口类型) throw std::logic_error("exclusive concept type damaged");
            入口类型 = *n;
        }
        if (有所属 && 入口类型 != 专属存在概念节点类型)
            throw std::logic_error("exclusive concept owner contradicts type");
        if (入口类型 != 专属存在概念节点类型) return false;
        if (!查询概念关系(专属概念节点, true).empty()
            || !查询概念关系(专属概念节点, false).empty()
            || !全局基础数据集.查询字段(
                专属概念节点, 名称关系字段).empty()) {
            return false;
        }
        const auto 对应存在 = 读取唯一节点字段(
            专属概念节点, 对应存在字段);
        if (!对应存在) throw std::logic_error("exclusive concept owner field missing or duplicate");
        if (*对应存在 != 实际存在节点) return false;

        const auto 组织关系组 = 全局基础数据集.查询目标关系(
            专属概念节点, 基础外部关系类型::父子);
        std::optional<稳定编码> 组织关系;
        for (const auto& 关系 : 组织关系组) {
            if (关系.源节点 == 存在概念图根节点
                && 关系.角色或顺序 == 组织归属关系角色) {
                if (组织关系) throw std::logic_error("duplicate exclusive concept organization");
                组织关系 = 关系.编码;
            }
        }
        if (!组织关系 || 组织关系组.size() != 1)
            throw std::logic_error("exclusive concept organization damaged");
        std::vector<稳定编码> 节点组{专属概念节点}, 字段组;
        for (const auto& f : 全局基础数据集.查询字段(专属概念节点, 特征项字段)) {
            const auto* n = std::get_if<稳定编码>(&f.内容);
            if (!n || !有效(*n) || std::find(节点组.begin(), 节点组.end(), *n) != 节点组.end())
                throw std::logic_error("exclusive concept item identity damaged");
            节点组.push_back(*n);
        }
        const auto 读 = 全局基础数据集.查询节点组完整结构(节点组);
        if (读.状态 != 基础结构操作状态::已完成 || 读.节点组.size() != 节点组.size())
            throw std::logic_error("exclusive concept cleanup snapshot unavailable");
        for (const auto& x : 读.节点组) {
            if (!x.快照) throw std::logic_error("exclusive concept cleanup node missing");
            const bool 主 = x.查询节点 == 专属概念节点;
            const auto& s = *x.快照;
            if (!s.出向外部关系组.empty()
                || (主 ? s.入向外部关系组 != 组织关系组 : !s.入向外部关系组.empty()))
                throw std::logic_error("exclusive concept cleanup unexpected edges");
            unsigned 类型数=0, 一数=0, 二数=0;
            for (const auto& f : s.结构.字段组) {
                const auto* n = std::get_if<稳定编码>(&f.内容);
                if (!n || !有效(*n)) throw std::logic_error("exclusive concept cleanup invalid field");
                if (f.字段节点 == 节点类型字段) {
                    ++类型数;
                    if (*n != (主 ? 专属存在概念节点类型 : 特征项节点类型))
                        throw std::logic_error("exclusive concept cleanup wrong type");
                } else if (f.字段节点 == (主 ? 对应存在字段 : 特征类型概念字段)) {
                    ++一数;
                    if (主 && *n != 实际存在节点) throw std::logic_error("exclusive concept owner changed");
                } else if (f.字段节点 == (主 ? 特征项字段 : 具体特征概念字段)) {
                    ++二数;
                    if (主 && std::find(节点组.begin()+1, 节点组.end(), *n) == 节点组.end())
                        throw std::logic_error("exclusive concept item changed");
                } else throw std::logic_error("exclusive concept cleanup unknown field");
                字段组.push_back(f.编码);
            }
            if (类型数 != 1 || 一数 != 1 || (主 ? 二数 != 节点组.size()-1 : 二数 != 1))
                throw std::logic_error("exclusive concept cleanup field multiplicity");
        }
        const auto s = 全局基础数据集.原子删除节点结构(节点组, 字段组, {*组织关系});
        if (s == 基础结构操作状态::已完成) return true;
        if (s == 基础结构操作状态::引用仍存在) return false;
        throw std::logic_error("exclusive concept atomic cleanup failed");
    } catch (const std::exception& e) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"撤销未绑定专属存在概念", L"原子撤销结构异常，未将故障作为业务否定。", e.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"撤销未绑定专属存在概念", L"未知工程异常。");
    }
}

存在概念抽象结果 概念_存在类::抽象共同存在概念(
    const std::vector<稳定编码>& 来源概念组) noexcept {
    存在概念抽象结果 结果;
    try {
        std::lock_guard 锁(存在概念图互斥);
        if (!结构仍存在()) return 结果;
        auto 来源 = 来源概念组;
        排序去重(来源);
        if (来源.size() < 2) {
            结果.状态 = 存在概念操作状态::来源不足;
            return 结果;
        }
        std::vector<std::vector<存在概念特征项>> 来源特征组;
        for (const auto 来源概念 : 来源) {
            if (!是存在概念节点(来源概念)) {
                结果.状态 = 存在概念操作状态::概念不存在;
                return 结果;
            }
            const auto 特征组 = 读取特征组(来源概念, 特征概念服务_);
            if (!特征组) {
                结果.状态 = 存在概念操作状态::结构不一致;
                return 结果;
            }
            来源特征组.push_back(*特征组);
        }

        for (const auto& 首项 : 来源特征组.front()) {
            auto 共同概念 = std::optional<稳定编码>{首项.具体特征概念};
            for (std::size_t i = 1; i < 来源特征组.size() && 共同概念; ++i) {
                const auto 位置 = std::find_if(
                    来源特征组[i].begin(), 来源特征组[i].end(),
                    [&](const auto& 项) {
                        return 项.特征类型概念 == 首项.特征类型概念;
                    });
                if (位置 == 来源特征组[i].end()) {
                    共同概念.reset();
                    break;
                }
                共同概念 = 共同上位特征概念(
                    *共同概念, 位置->具体特征概念, 特征概念服务_);
            }
            if (共同概念) {
                结果.共同特征组.push_back({首项.特征类型概念, *共同概念});
            }
        }
        排序特征项(结果.共同特征组);
        if (结果.共同特征组.empty()) {
            结果.状态 = 存在概念操作状态::无共同点;
            return 结果;
        }

        std::optional<稳定编码> 抽象节点;
        for (const auto& 组织关系 : 全局基础数据集.查询源关系(
            存在概念图根节点, 基础外部关系类型::父子)) {
            if (组织关系.角色或顺序 != 组织归属关系角色
                || !是抽象存在概念节点(组织关系.目标节点)) {
                continue;
            }
            const auto 已有特征组 = 读取特征组(
                组织关系.目标节点, 特征概念服务_);
            if (已有特征组 && *已有特征组 == 结果.共同特征组) {
                if (抽象节点) {
                    结果.状态 = 存在概念操作状态::结构不一致;
                    return 结果;
                }
                抽象节点 = 组织关系.目标节点;
            }
        }

        bool 新建 = false;
        if (!抽象节点) {
            const auto 节点 = 全局基础数据集.新建节点();
            if (!有效(节点)) {
                结果.状态 = 存在概念操作状态::资源失败;
                return 结果;
            }
            const auto 类型字段 = 全局基础数据集.添加字段节点(
                节点, 节点类型字段, 抽象存在概念节点类型);
            auto 特征项组 = 有效(类型字段)
                ? 建立特征项节点组(结果.共同特征组) : std::nullopt;
            const bool 特征完成 = 特征项组
                && 挂接特征项节点组(节点, *特征项组);
            const auto 组织关系 = 特征完成
                ? 全局基础数据集.添加关系(
                    存在概念图根节点, 节点,
                    基础外部关系类型::父子, 组织归属关系角色)
                : 稳定编码{};
            if (!有效(组织关系)) {
                if (特征项组) 删除特征项节点组(*特征项组);
                if (有效(类型字段)) (void)全局基础数据集.删除字段(类型字段);
                (void)全局基础数据集.删除节点(节点);
                结果.状态 = 存在概念操作状态::资源失败;
                return 结果;
            }
            抽象节点 = 节点;
            新建 = true;
        }

        for (const auto 来源概念 : 来源) {
            if (来源概念 == *抽象节点) continue;
            bool 已存在 = false;
            for (const auto& 关系 : 查询概念关系(*抽象节点, false)) {
                if (关系.目标节点 == 来源概念) {
                    已存在 = true;
                    break;
                }
            }
            if (已存在) continue;
            const auto 来源特征 = 读取特征组(来源概念, 特征概念服务_);
            if (!来源特征
                || !可由上位包含(
                    结果.共同特征组, *来源特征, 特征概念服务_)
                || 向下可达(来源概念, *抽象节点)) {
                结果.状态 = 向下可达(来源概念, *抽象节点)
                    ? 存在概念操作状态::会形成环
                    : 存在概念操作状态::层级不成立;
                return 结果;
            }
            const auto 关系 = 全局基础数据集.添加关系(
                *抽象节点, 来源概念,
                基础外部关系类型::父子, 概念上下位关系角色);
            if (!有效(关系)) {
                结果.状态 = 存在概念操作状态::资源失败;
                return 结果;
            }
        }
        结果.状态 = 新建
            ? 存在概念操作状态::已建立
            : 存在概念操作状态::已复用;
        结果.抽象概念节点 = *抽象节点;
        return 结果;
    } catch (...) {
        结果.状态 = 存在概念操作状态::资源失败;
        return 结果;
    }
}

存在概念建立结果 概念_存在类::建立人工抽象存在概念(
    const std::vector<稳定编码>& 直接下位概念组) noexcept {
    存在概念建立结果 结果;
    稳定编码 节点;
    稳定编码 类型字段;
    稳定编码 组织关系;
    std::vector<稳定编码> 下位关系组;
    const auto 回滚 = [&]() noexcept {
        for (auto 位置 = 下位关系组.rbegin();
            位置 != 下位关系组.rend(); ++位置) {
            (void)全局基础数据集.删除关系(*位置);
        }
        if (有效(组织关系)) (void)全局基础数据集.删除关系(组织关系);
        if (有效(类型字段)) (void)全局基础数据集.删除字段(类型字段);
        if (有效(节点)) (void)全局基础数据集.删除节点(节点);
    };
    try {
        std::lock_guard 锁(存在概念图互斥);
        if (!结构仍存在()) return 结果;
        auto 下位概念组 = 直接下位概念组;
        排序去重(下位概念组);
        if (下位概念组.empty()) {
            结果.状态 = 存在概念操作状态::来源不足;
            return 结果;
        }
        for (const auto 下位概念 : 下位概念组) {
            if (!是存在概念节点(下位概念)) {
                结果.状态 = 存在概念操作状态::概念不存在;
                return 结果;
            }
        }

        节点 = 全局基础数据集.新建节点();
        if (!有效(节点)) {
            结果.状态 = 存在概念操作状态::资源失败;
            return 结果;
        }
        类型字段 = 全局基础数据集.添加字段节点(
            节点, 节点类型字段, 抽象存在概念节点类型);
        组织关系 = 有效(类型字段)
            ? 全局基础数据集.添加关系(
                存在概念图根节点, 节点,
                基础外部关系类型::父子, 组织归属关系角色)
            : 稳定编码{};
        if (有效(组织关系)) {
            for (const auto 下位概念 : 下位概念组) {
                const auto 关系 = 全局基础数据集.添加关系(
                    节点, 下位概念,
                    基础外部关系类型::父子, 概念上下位关系角色);
                if (!有效(关系)) break;
                下位关系组.push_back(关系);
            }
        }

        if (!有效(组织关系) || 下位关系组.size() != 下位概念组.size()) {
            回滚();
            结果.状态 = 存在概念操作状态::资源失败;
            return 结果;
        }

        const auto 读回 = 获取存在概念(节点);
        if (!读回 || 读回->种类 != 存在概念种类::抽象
            || !读回->特征组.empty()
            || 读回->下位概念组 != 下位概念组
            || !读回->上位概念组.empty()) {
            回滚();
            结果.状态 = 存在概念操作状态::结构不一致;
            return 结果;
        }
        结果.状态 = 存在概念操作状态::已建立;
        结果.概念节点 = 节点;
        return 结果;
    } catch (...) {
        回滚();
        结果.状态 = 存在概念操作状态::资源失败;
        return 结果;
    }
}

std::optional<存在概念信息> 概念_存在类::获取存在概念(
    稳定编码 概念节点) const noexcept {
    try {
        std::lock_guard 锁(存在概念图互斥);
        if (!结构仍存在() || !节点仍存在(概念节点)) return std::nullopt;
        const auto 种类 = 读取概念种类(概念节点);
        const auto 特征组 = 读取特征组(概念节点, 特征概念服务_);
        if (!种类 || !特征组) return std::nullopt;
        // 完整普通定义只消费原普通分类边；未知定义身份由分类公开投影另行表达。
        std::vector<稳定编码> 上位概念组,下位概念组;
        for(const auto&e:查询概念关系(概念节点,true))上位概念组.push_back(e.源节点);
        for(const auto&e:查询概念关系(概念节点,false))下位概念组.push_back(e.目标节点);
        排序去重(上位概念组);排序去重(下位概念组);
        // 自动抽象概念由共同特征定义；人工分类概念可以没有共同特征，
        // 但必须已经持有至少一个正式下位概念，不能成为孤立空概念。
        if (*种类 == 存在概念种类::抽象
            && 特征组->empty() && 下位概念组.empty() && !是登记存在根(概念节点)) {
            return std::nullopt;
        }
        存在概念信息 结果;
        结果.节点 = 概念节点;
        结果.种类 = *种类;
        if (*种类 == 存在概念种类::专属) {
            结果.对应存在节点 = 读取唯一节点字段(
                概念节点, 对应存在字段);
            if (!结果.对应存在节点) return std::nullopt;
        } else if (!全局基础数据集.查询字段(
            概念节点, 对应存在字段).empty()) {
            return std::nullopt;
        }
        结果.特征组 = *特征组;
        for (const auto& 字段 : 全局基础数据集.查询字段(
            概念节点, 名称关系字段)) {
            const auto* 关系节点 = std::get_if<稳定编码>(&字段.内容);
            if (!关系节点 || !节点仍存在(*关系节点)) return std::nullopt;
            结果.名称关系.push_back(*关系节点);
        }
        排序去重(结果.名称关系);
        if (结果.名称关系.size() != 全局基础数据集.查询字段(
            概念节点, 名称关系字段).size()) {
            return std::nullopt;
        }
        结果.上位概念组 = 上位概念组;
        结果.下位概念组 = 下位概念组;
        return 结果;
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"获取存在概念", L"概念读取异常。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"获取存在概念", L"概念读取未知异常。");
    }
}

bool 概念_存在类::绑定名称关系(
    稳定编码 概念节点, 稳定编码 名称关系节点) noexcept {
    const auto learning=读取语言归类概念身份保护(概念节点);
    if(learning.信息&&learning.信息->定义未知)return 更新语言归类名称关系(概念节点,名称关系节点,true);

    try {
        std::lock_guard 锁(存在概念图互斥);
        if (!结构仍存在() || !是存在概念节点(概念节点)
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

bool 概念_存在类::解除名称关系(
    稳定编码 概念节点, 稳定编码 名称关系节点) noexcept {
    const auto learning=读取语言归类概念身份保护(概念节点);
    if(learning.信息&&learning.信息->定义未知)return 更新语言归类名称关系(概念节点,名称关系节点,false);

    try {
        std::lock_guard 锁(存在概念图互斥);
        if (!结构仍存在() || !是存在概念节点(概念节点)) return false;
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

bool 概念_存在类::是存在概念节点(稳定编码 节点) const noexcept {
    if(有效(归类未知类型)&&读取唯一节点字段(节点,节点类型字段)==归类未知类型){const auto r=读取语言归类概念身份保护(节点);return r.状态==归类概念读取状态::已读取&&r.信息.has_value();}

    try {
        std::lock_guard 锁(存在概念图互斥);
        return 结构仍存在() && 节点仍存在(节点)
            && 读取概念种类(节点).has_value();
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"概念_存在类::是存在概念节点",
            L"概念身份读取发生C++异常，未形成业务结果。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"概念_存在类::是存在概念节点",
            L"概念身份读取发生未知C++异常，未形成业务结果。");
    }
}

bool 概念_存在类::是专属存在概念节点(稳定编码 节点) const noexcept {
    try {
        std::lock_guard 锁(存在概念图互斥);
        const auto 种类 = 结构仍存在() ? 读取概念种类(节点) : std::nullopt;
        return 种类 && *种类 == 存在概念种类::专属;
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"概念_存在类::是专属存在概念节点",
            L"专属概念身份读取发生C++异常，未形成业务结果。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"概念_存在类::是专属存在概念节点",
            L"专属概念身份读取发生未知C++异常，未形成业务结果。");
    }
}

bool 概念_存在类::是抽象存在概念节点(稳定编码 节点) const noexcept {
    try {
        std::lock_guard 锁(存在概念图互斥);
        const auto 种类 = 结构仍存在() ? 读取概念种类(节点) : std::nullopt;
        return 种类 && *种类 == 存在概念种类::抽象;
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"概念_存在类::是抽象存在概念节点",
            L"抽象概念身份读取发生C++异常，未形成业务结果。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"概念_存在类::是抽象存在概念节点",
            L"抽象概念身份读取发生未知C++异常，未形成业务结果。");
    }
}

std::vector<稳定编码> 概念_存在类::查询上位存在概念(
    稳定编码 概念节点) const noexcept {
    if(有效(归类未知类型)){const auto r=读取存在分类关系保护(概念节点);if(r.信息)return r.信息->直接上位概念组;}

    try {
        std::lock_guard 锁(存在概念图互斥);
        std::vector<稳定编码> 结果;
        if (!是存在概念节点(概念节点)) return 结果;
        for (const auto& 关系 : 查询概念关系(概念节点, true)) {
            if (!是抽象存在概念节点(关系.源节点))throw std::logic_error("ordinary classification parent is not abstract");
            结果.push_back(关系.源节点);
        }
        排序去重(结果);
        return 结果;
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"概念_存在类::查询上位存在概念",
            L"上位概念读取发生C++异常，未形成业务结果。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"概念_存在类::查询上位存在概念",
            L"上位概念读取发生未知C++异常，未形成业务结果。");
    }
}

存在概念实例候选读取结果 概念_存在类::读取下位专属实例候选完整(
    稳定编码 概念节点) const noexcept {
    using 状态 = 存在概念实例候选读取状态;
    存在概念实例候选读取结果 结果;
    try {
        std::lock_guard 锁(存在概念图互斥);
        if (!有效(概念节点)) return 结果;
        if (!结构仍存在()) { 结果.状态 = 状态::概念不可读; return 结果; }
        const auto 是下位关系 = [](const 基础外部关系& 关系) {
            return 关系.类型 == 基础外部关系类型::父子
                && 关系.角色或顺序 == 概念上下位关系角色;
        };
        const auto 快照相同 = [](const 基础联合节点快照& 左, const 基础联合节点快照& 右) {
            return 左.结构.节点 == 右.结构.节点 && 左.结构.字段组 == 右.结构.字段组
                && 左.入向字段组 == 右.入向字段组
                && 左.入向外部关系组 == 右.入向外部关系组
                && 左.出向外部关系组 == 右.出向外部关系组;
        };
        std::map<稳定编码, 基础联合节点快照> 首读组;
        const auto 读取 = [&](稳定编码 节点) -> const 基础联合节点快照* {
            const auto 已有 = 首读组.find(节点);
            if (已有 != 首读组.end()) return &已有->second;
            const auto 单读 = 全局基础数据集.查询节点组完整结构({节点});
            if (单读.状态 == 基础结构操作状态::资源失败) { 结果.状态 = 状态::资源失败; return nullptr; }
            if (单读.状态 == 基础结构操作状态::内部不一致) { 结果.状态 = 状态::结构不一致; return nullptr; }
            if (单读.状态 != 基础结构操作状态::已完成 || 单读.节点组.size() != 1
                || 单读.节点组.front().查询节点 != 节点)
                throw std::logic_error("concept candidate singleton read mismatch");
            if (!单读.节点组.front().快照) { 结果.状态 = 状态::概念不可读; return nullptr; }
            return &首读组.emplace(节点, *单读.节点组.front().快照).first->second;
        };
        const auto* 图根 = 读取(存在概念图根节点);
        if (!图根) return 结果;
        struct 遍历项 { 稳定编码 节点; bool 退出; };
        std::vector<遍历项> 待查{{概念节点, false}};
        std::set<稳定编码> 活动, 完成;
        std::vector<存在概念专属实例候选> 候选;
        while (!待查.empty()) {
            const auto 当前 = 待查.back(); 待查.pop_back();
            if (当前.退出) { 活动.erase(当前.节点); 完成.insert(当前.节点); continue; }
            if (完成.contains(当前.节点)) continue; // DAG汇合不是环。
            if (!活动.insert(当前.节点).second) { 结果.状态 = 状态::结构不一致; return 结果; }
            const auto* 快照 = 读取(当前.节点);
            if (!快照) return 结果;
            std::optional<稳定编码> 类型, 对应存在;
            for (const auto& 字段 : 快照->结构.字段组) {
                if (!有效(字段.编码) || 字段.所属节点 != 当前.节点 || !有效(字段.字段节点)) {
                    结果.状态 = 状态::结构不一致; return 结果;
                }
                if (字段.字段节点 != 节点类型字段 && 字段.字段节点 != 对应存在字段) continue;
                const auto* 引用 = std::get_if<稳定编码>(&字段.内容);
                auto& 槽 = 字段.字段节点 == 节点类型字段 ? 类型 : 对应存在;
                if (!引用 || !有效(*引用) || 槽) { 结果.状态 = 状态::结构不一致; return 结果; }
                槽 = *引用;
            }
            if (!类型 || (*类型 != 专属存在概念节点类型 && *类型 != 抽象存在概念节点类型)) {
                结果.状态 = 当前.节点 == 概念节点 ? 状态::概念不可读 : 状态::结构不一致; return 结果;
            }
            std::size_t 组织归属数 = 0;
            for (const auto& 关系 : 快照->入向外部关系组) {
                if (关系.类型 != 基础外部关系类型::父子 || 关系.角色或顺序 != 组织归属关系角色) continue;
                ++组织归属数;
                if (!有效(关系.编码) || 关系.源节点 != 存在概念图根节点 || 关系.目标节点 != 当前.节点
                    || std::count(图根->出向外部关系组.begin(), 图根->出向外部关系组.end(), 关系) != 1) {
                    结果.状态 = 状态::结构不一致; return 结果;
                }
            }
            if (组织归属数 != 1 || ((*类型 == 专属存在概念节点类型) != 对应存在.has_value())) {
                结果.状态 = 状态::结构不一致; return 结果;
            }
            if (对应存在) {
                if (!读取(*对应存在)) return 结果;
                候选.push_back({当前.节点, *对应存在});
            }
            待查.push_back({当前.节点, true});
            std::set<稳定编码> 直接下位;
            for (const auto& 关系 : 快照->出向外部关系组) {
                if (!是下位关系(关系)) continue;
                if (!有效(关系.编码) || 关系.源节点 != 当前.节点 || !有效(关系.目标节点)
                    || *类型 != 抽象存在概念节点类型 || !直接下位.insert(关系.目标节点).second) {
                    结果.状态 = 状态::结构不一致; return 结果;
                }
                const auto* 下位 = 读取(关系.目标节点);
                if (!下位) return 结果;
                if (std::count(下位->入向外部关系组.begin(), 下位->入向外部关系组.end(), 关系) != 1) {
                    结果.状态 = 状态::结构不一致; return 结果;
                }
                待查.push_back({关系.目标节点, false});
            }
        }
        std::vector<稳定编码> 节点组;
        for (const auto& [节点, _] : 首读组) 节点组.push_back(节点);
        const auto 终读 = 全局基础数据集.查询节点组完整结构(节点组);
        if (终读.状态 == 基础结构操作状态::资源失败) { 结果.状态 = 状态::资源失败; return 结果; }
        if (终读.状态 == 基础结构操作状态::内部不一致) { 结果.状态 = 状态::结构不一致; return 结果; }
        if (终读.状态 != 基础结构操作状态::已完成 || 终读.节点组.size() != 节点组.size())
            throw std::logic_error("concept candidate union read mismatch");
        for (std::size_t i = 0; i < 节点组.size(); ++i) {
            const auto& 项 = 终读.节点组[i];
            if (项.查询节点 != 节点组[i]) throw std::logic_error("concept candidate union order mismatch");
            if (!项.快照 || !快照相同(首读组.at(项.查询节点), *项.快照)) {
                结果.状态 = 状态::读取已变化; return 结果;
            }
        }
        std::sort(候选.begin(), 候选.end(), [](const auto& 左, const auto& 右) {
            return 左.专属概念节点 < 右.专属概念节点;
        });
        结果.状态 = 状态::已完成;
        结果.起始概念节点 = 概念节点;
        结果.候选组 = std::move(候选);
        return 结果;
    } catch (const std::bad_alloc&) { 结果.状态 = 状态::资源失败; return 结果;
    } catch (const std::length_error&) { 结果.状态 = 状态::资源失败; return 结果;
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取下位专属实例候选完整", L"概念候选读取异常。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取下位专属实例候选完整", L"概念候选读取未知异常。");
    }
}

std::vector<稳定编码> 概念_存在类::查询下位存在概念(
    稳定编码 概念节点) const noexcept {
    if(有效(归类未知类型)){const auto r=读取存在分类关系保护(概念节点);if(r.信息)return r.信息->直接下位概念组;}

    try {
        std::lock_guard 锁(存在概念图互斥);
        std::vector<稳定编码> 结果;
        if (!是存在概念节点(概念节点)) return 结果;
        for (const auto& 关系 : 查询概念关系(概念节点, false)) {
            if (!是存在概念节点(关系.目标节点))throw std::logic_error("ordinary classification child has wrong type");
            结果.push_back(关系.目标节点);
        }
        排序去重(结果);
        return 结果;
    } catch(const std::exception&e) {
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"查询下位存在概念",L"普通分类边读取失败。",e.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"查询下位存在概念",L"普通分类边读取未知异常。");
    }
}

namespace {

// 顺序与 INIT-PRONOUN-DEFINITION-PROVIDER v0.1 的十三个私有键一致。
std::array<稳定编码, 13> 指代键{};
std::array<稳定编码, 13> 指代待核验键{};
enum 指代键位 : std::size_t {
    定义类型, 规则类型, 约束类型, 范围键, 参数键, 规则项键, 约束项键,
    规则种类键, 约束角色键, FT根键, 允许区间键, FT快照键, FT单位键
};

enum class 指代内部状态 {
    完成, 输入拒绝, 依赖缺失, 依赖已变化, 结构不一致,
    资源失败, 读取未完成, 读取漂移, 未找到
};

指代内部状态 映射基础状态(基础结构操作状态 状态) {
    switch (状态) {
    case 基础结构操作状态::已完成: return 指代内部状态::完成;
    case 基础结构操作状态::节点不存在:
    case 基础结构操作状态::引用不存在: return 指代内部状态::依赖缺失;
    case 基础结构操作状态::资源失败: return 指代内部状态::资源失败;
    case 基础结构操作状态::入口拒绝: return 指代内部状态::读取未完成;
    default: return 指代内部状态::结构不一致;
    }
}

指代概念查询状态 查询状态(指代内部状态 状态) {
    switch (状态) {
    case 指代内部状态::完成: return 指代概念查询状态::已找到;
    case 指代内部状态::未找到: return 指代概念查询状态::未找到;
    case 指代内部状态::输入拒绝: return 指代概念查询状态::输入拒绝;
    case 指代内部状态::依赖缺失: return 指代概念查询状态::依赖缺失;
    case 指代内部状态::依赖已变化: return 指代概念查询状态::依赖已变化;
    case 指代内部状态::结构不一致: return 指代概念查询状态::结构不一致;
    case 指代内部状态::资源失败: return 指代概念查询状态::资源失败;
    case 指代内部状态::读取漂移: return 指代概念查询状态::读取漂移;
    default: return 指代概念查询状态::读取未完成;
    }
}

指代概念写入状态 写入状态(指代内部状态 状态) {
    switch (状态) {
    case 指代内部状态::输入拒绝: return 指代概念写入状态::输入拒绝;
    case 指代内部状态::依赖缺失: return 指代概念写入状态::依赖缺失;
    case 指代内部状态::依赖已变化: return 指代概念写入状态::依赖已变化;
    case 指代内部状态::结构不一致: return 指代概念写入状态::结构不一致;
    case 指代内部状态::资源失败: return 指代概念写入状态::资源失败;
    case 指代内部状态::读取漂移: return 指代概念写入状态::读取漂移;
    default: return 指代概念写入状态::读取未完成;
    }
}

bool 规范化区间(std::vector<特征概念I64闭区间>& 区间) {
    if (区间.empty()) return false;
    for (const auto& 项 : 区间) if (项.下界 > 项.上界) return false;
    std::sort(区间.begin(), 区间.end(), [](const auto& 左, const auto& 右) {
        return 左.下界 < 右.下界
            || (左.下界 == 右.下界 && 左.上界 < 右.上界);
    });
    std::vector<特征概念I64闭区间> 合并;
    for (const auto& 项 : 区间) {
        if (合并.empty() || (合并.back().上界 < 项.下界
            && (合并.back().上界 == std::numeric_limits<std::int64_t>::max()
                || 合并.back().上界 + 1 < 项.下界))) {
            合并.push_back(项);
        } else if (项.上界 > 合并.back().上界) {
            合并.back().上界 = 项.上界;
        }
    }
    区间 = std::move(合并);
    return true;
}

bool 规范化形状(指代概念定义& 定义) {
    if (定义.目标范围 != 指代目标范围::单个已登记强类型信息节点) return false;
    std::vector<指代参数定义> 预期;
    switch (定义.规则) {
    case 指代声明规则::作用域说话者:
    case 指代声明规则::作用域受话者:
    case 指代声明规则::作用域第三方:
        预期 = {{指代参数角色::目标, 指代参数类型::强类型信息节点},
            {指代参数角色::作用域, 指代参数类型::交互作用域}};
        break;
    case 指代声明规则::相对近指:
    case 指代声明规则::相对远指:
        预期 = {{指代参数角色::目标, 指代参数类型::强类型信息节点},
            {指代参数角色::作用域, 指代参数类型::交互作用域},
            {指代参数角色::参照中心, 指代参数类型::参照信息节点},
            {指代参数角色::参照维度规则, 指代参数类型::正式参照规则}};
        break;
    case 指代声明规则::受限身份变量:
        预期 = {{指代参数角色::目标, 指代参数类型::身份变量},
            {指代参数角色::作用域, 指代参数类型::交互作用域},
            {指代参数角色::主体类型限制, 指代参数类型::正式主体类型限制}};
        break;
    default: return false;
    }
    std::sort(定义.参数组.begin(), 定义.参数组.end(), [](const auto& 左, const auto& 右) {
        return 左.角色 < 右.角色;
    });
    if (定义.参数组 != 预期) return false;
    for (auto& 约束 : 定义.特征约束组) {
        const auto 参数 = std::find_if(定义.参数组.begin(), 定义.参数组.end(),
            [&](const auto& 项) { return 项.角色 == 约束.被约束角色; });
        if (参数 == 定义.参数组.end()
            || (参数->类型 != 指代参数类型::强类型信息节点
                && 参数->类型 != 指代参数类型::参照信息节点)
            || !有效(约束.特征类型根) || !规范化区间(约束.允许值域)
            || 约束.特征类型快照.材料类型 != 特征概念材料物理类型::I64标量
            || 约束.特征类型快照.比较规则 != 特征概念比较规则::I64数值
            || (约束.特征类型快照.聚合规则 != 特征概念聚合规则::不自动聚合
                && 约束.特征类型快照.聚合规则 != 特征概念聚合规则::I64连续值归组)
            || !约束.特征类型快照.名称关系.empty()
            || (约束.特征类型快照.单位 && !有效(*约束.特征类型快照.单位))) return false;
        auto* 根域 = std::get_if<std::vector<特征概念I64闭区间>>(
            &约束.特征类型快照.值域);
        if (!根域 || !规范化区间(*根域)) return false;
    }
    std::sort(定义.特征约束组.begin(), 定义.特征约束组.end(),
        [](const auto& 左, const auto& 右) {
            if (左.被约束角色 != 右.被约束角色)
                return 左.被约束角色 < 右.被约束角色;
            return 左.特征类型根 < 右.特征类型根;
        });
    for (std::size_t i = 1; i < 定义.特征约束组.size(); ++i) {
        const auto& 左 = 定义.特征约束组[i - 1];
        const auto& 右 = 定义.特征约束组[i];
        if (左.被约束角色 == 右.被约束角色
            && 左.特征类型根 == 右.特征类型根) return false;
    }
    return true;
}

指代内部状态 结构键状态() {
    if (std::all_of(指代键.begin(), 指代键.end(), [](auto 值) { return !有效(值); })) {
        return std::all_of(指代待核验键.begin(), 指代待核验键.end(),
            [](auto 值) { return !有效(值); })
            ? 指代内部状态::依赖缺失 : 指代内部状态::结构不一致;
    }
    std::set<稳定编码> 身份;
    for (const auto 编码 : 指代键) {
        if (!有效(编码) || 编码 == 节点类型字段 || !身份.insert(编码).second)
            return 指代内部状态::结构不一致;
        const auto 结构 = 全局基础数据集.查询完整节点结构(编码);
        if (结构.状态 != 基础结构操作状态::已完成)
            return 映射基础状态(结构.状态) == 指代内部状态::资源失败
                ? 指代内部状态::资源失败 : 指代内部状态::结构不一致;
        if (!结构.结构 || !结构.结构->字段组.empty())
            return 指代内部状态::结构不一致;
    }
    return 指代内部状态::完成;
}

std::optional<std::int64_t> 唯一I64(const 基础节点结构& 结构, 稳定编码 键) {
    std::optional<std::int64_t> 值;
    for (const auto& 字段 : 结构.字段组) if (字段.字段节点 == 键) {
        if (值) return std::nullopt;
        const auto* 原始 = std::get_if<基础值>(&字段.内容);
        const auto* 数 = 原始 ? std::get_if<std::int64_t>(&原始->材料) : nullptr;
        if (!数) return std::nullopt;
        值 = *数;
    }
    return 值;
}

std::optional<稳定编码> 唯一引用(const 基础节点结构& 结构, 稳定编码 键) {
    std::optional<稳定编码> 值;
    for (const auto& 字段 : 结构.字段组) if (字段.字段节点 == 键) {
        if (值) return std::nullopt;
        const auto* 引用 = std::get_if<稳定编码>(&字段.内容);
        if (!引用 || !有效(*引用)) return std::nullopt;
        值 = *引用;
    }
    return 值;
}

std::optional<std::vector<std::int64_t>> 唯一数组(
    const 基础节点结构& 结构, 稳定编码 键) {
    std::optional<std::vector<std::int64_t>> 值;
    for (const auto& 字段 : 结构.字段组) if (字段.字段节点 == 键) {
        if (值) return std::nullopt;
        const auto* 原始 = std::get_if<基础值>(&字段.内容);
        const auto* 数 = 原始 ? std::get_if<std::vector<std::int64_t>>(&原始->材料) : nullptr;
        if (!数) return std::nullopt;
        值 = *数;
    }
    return 值;
}

bool 字段闭合(const 基础节点结构& 结构, std::initializer_list<稳定编码> 允许) {
    for (const auto& 字段 : 结构.字段组)
        if (std::find(允许.begin(), 允许.end(), 字段.字段节点) == 允许.end()) return false;
    return true;
}

std::optional<std::vector<特征概念I64闭区间>> 解码区间(
    const std::vector<std::int64_t>& 数组, std::size_t 前缀) {
    if (数组.size() <= 前缀 || 数组[前缀] <= 0) return std::nullopt;
    const auto 数量 = static_cast<std::uint64_t>(数组[前缀]);
    if (数量 > (std::numeric_limits<std::size_t>::max() - 前缀 - 1) / 2
        || 数组.size() != 前缀 + 1 + 2 * static_cast<std::size_t>(数量)) return std::nullopt;
    std::vector<特征概念I64闭区间> 结果;
    for (std::size_t i = 前缀 + 1; i < 数组.size(); i += 2)
        结果.push_back({数组[i], 数组[i + 1]});
    auto 原样 = 结果;
    if (!规范化区间(结果) || 结果 != 原样) return std::nullopt;
    return 结果;
}

std::vector<std::int64_t> 编码区间(const std::vector<特征概念I64闭区间>& 区间) {
    std::vector<std::int64_t> 数组{static_cast<std::int64_t>(区间.size())};
    for (const auto& 项 : 区间) { 数组.push_back(项.下界); 数组.push_back(项.上界); }
    return 数组;
}

指代内部状态 读取结构(稳定编码 节点, 基础节点结构& 输出) {
    const auto 读取 = 全局基础数据集.查询完整节点结构(节点);
    if (读取.状态 != 基础结构操作状态::已完成)
        return 映射基础状态(读取.状态);
    if (!读取.结构) return 指代内部状态::读取未完成;
    输出 = *读取.结构;
    return 指代内部状态::完成;
}

bool 唯一持有(稳定编码 子项, 稳定编码 所属, 稳定编码 字段键) {
    const auto 反向 = 全局基础数据集.查询目标字段(子项, 字段键);
    return 反向.size() == 1 && 反向.front().所属节点 == 所属
        && std::get_if<稳定编码>(&反向.front().内容)
        && *std::get_if<稳定编码>(&反向.front().内容) == 子项;
}

struct 指代候选快照 final {
    std::vector<基础字段关系> 召回;
    std::vector<基础节点结构> 结构;
    std::vector<指代概念信息> 信息;
};

bool 同一候选快照(const 指代候选快照& 左, const 指代候选快照& 右) {
    if (左.召回 != 右.召回 || 左.信息 != 右.信息
        || 左.结构.size() != 右.结构.size()) return false;
    for (std::size_t i = 0; i < 左.结构.size(); ++i)
        if (左.结构[i].节点 != 右.结构[i].节点
            || 左.结构[i].字段组 != 右.结构[i].字段组) return false;
    return true;
}

指代内部状态 读取指代定义(稳定编码 节点, 指代候选快照& 快照) {
    基础节点结构 主;
    const auto 首读 = 全局基础数据集.查询节点组完整结构({节点});
    if (首读.状态 != 基础结构操作状态::已完成)
        return 映射基础状态(首读.状态);
    if (首读.节点组.size() != 1 || !首读.节点组.front().快照)
        return 指代内部状态::依赖缺失;
    主 = 首读.节点组.front().快照->结构;
    std::vector<稳定编码> 名称;
    for (const auto& 字段 : 主.字段组) if (字段.字段节点 == 名称关系字段) {
        const auto* 引用 = std::get_if<稳定编码>(&字段.内容);
        if (!引用 || !有效(*引用)
            || std::find(名称.begin(), 名称.end(), *引用) != 名称.end())
            return 指代内部状态::结构不一致;
        名称.push_back(*引用);
    }
    std::vector<稳定编码> 联合节点{节点};
    联合节点.insert(联合节点.end(), 名称.begin(), 名称.end());
    const auto 联合 = 全局基础数据集.查询节点组完整结构(联合节点);
    if (联合.状态 != 基础结构操作状态::已完成)
        return 映射基础状态(联合.状态);
    if (联合.节点组.size() != 联合节点.size())
        return 指代内部状态::结构不一致;
    bool 主已核对 = false;
    for (const auto& 项 : 联合.节点组) {
        if (!项.快照) return 指代内部状态::结构不一致;
        if (项.查询节点 == 节点) {
            const auto& 前 = *首读.节点组.front().快照;
            const auto& 后 = *项.快照;
            if (前.结构.节点 != 后.结构.节点
                || 前.结构.字段组 != 后.结构.字段组
                || 前.入向外部关系组 != 后.入向外部关系组
                || 前.出向外部关系组 != 后.出向外部关系组
                || 前.入向字段组 != 后.入向字段组)
                return 指代内部状态::读取漂移;
            主已核对 = true;
        }
    }
    if (!主已核对) return 指代内部状态::结构不一致;
    std::sort(名称.begin(), 名称.end());
    auto 状态 = 指代内部状态::完成;
    const auto 类型 = 唯一引用(主, 节点类型字段);
    if (!类型 || *类型 != 指代键[定义类型]
        || !字段闭合(主, {节点类型字段, 指代键[范围键], 指代键[参数键],
            指代键[规则项键], 指代键[约束项键], 名称关系字段}))
        return 指代内部状态::结构不一致;
    const auto 范围 = 唯一I64(主, 指代键[范围键]);
    const auto 参数数组 = 唯一数组(主, 指代键[参数键]);
    const auto 规则节点 = 唯一引用(主, 指代键[规则项键]);
    if (!范围 || *范围 != 1 || !参数数组 || !规则节点
        || 参数数组->size() % 2 != 0)
        return 指代内部状态::结构不一致;
    指代概念定义 定义;
    定义.目标范围 = static_cast<指代目标范围>(*范围);
    for (std::size_t i = 0; i < 参数数组->size(); i += 2) {
        if ((*参数数组)[i] < 1 || (*参数数组)[i] > 5
            || (*参数数组)[i + 1] < 1 || (*参数数组)[i + 1] > 6)
            return 指代内部状态::结构不一致;
        定义.参数组.push_back({static_cast<指代参数角色>((*参数数组)[i]),
            static_cast<指代参数类型>((*参数数组)[i + 1])});
    }
    快照.结构.push_back(主);

    基础节点结构 规则;
    状态 = 读取结构(*规则节点, 规则);
    if (状态 != 指代内部状态::完成) return 状态;
    const auto 规则类型标记 = 唯一引用(规则, 节点类型字段);
    const auto 规则种类值 = 唯一I64(规则, 指代键[规则种类键]);
    if (!规则类型标记 || *规则类型标记 != 指代键[规则类型] || !规则种类值
        || *规则种类值 < 1 || *规则种类值 > 6
        || !字段闭合(规则, {节点类型字段, 指代键[规则种类键]})
        || !唯一持有(*规则节点, 节点, 指代键[规则项键]))
        return 指代内部状态::结构不一致;
    定义.规则 = static_cast<指代声明规则>(*规则种类值);
    快照.结构.push_back(规则);

    std::set<稳定编码> 项身份;
    for (const auto& 字段 : 主.字段组) {
        if (字段.字段节点 != 指代键[约束项键]) continue;
        const auto* 项节点 = std::get_if<稳定编码>(&字段.内容);
        if (!项节点 || !有效(*项节点) || !项身份.insert(*项节点).second
            || !唯一持有(*项节点, 节点, 指代键[约束项键]))
            return 指代内部状态::结构不一致;
        基础节点结构 项;
        状态 = 读取结构(*项节点, 项);
        if (状态 != 指代内部状态::完成) return 状态;
        const auto 项类型标记 = 唯一引用(项, 节点类型字段);
        const auto 角色 = 唯一I64(项, 指代键[约束角色键]);
        const auto 根 = 唯一引用(项, 指代键[FT根键]);
        const auto 允许数组 = 唯一数组(项, 指代键[允许区间键]);
        const auto 快照数组 = 唯一数组(项, 指代键[FT快照键]);
        if (!项类型标记 || *项类型标记 != 指代键[约束类型] || !角色
            || *角色 < 1 || *角色 > 5
            || !根 || !允许数组 || !快照数组 || 快照数组->size() < 7
            || (*快照数组)[0] != 1 || (*快照数组)[1] != 1
            || (*快照数组)[2] != 1
            || (*快照数组)[3] < 1 || (*快照数组)[3] > 2
            || !字段闭合(项,
                {节点类型字段, 指代键[约束角色键], 指代键[FT根键],
                指代键[允许区间键], 指代键[FT快照键], 指代键[FT单位键]}))
            return 指代内部状态::结构不一致;
        const auto 允许域 = 解码区间(*允许数组, 0);
        const auto 根域 = 解码区间(*快照数组, 4);
        if (!允许域 || !根域) return 指代内部状态::结构不一致;
        指代I64特征约束 约束;
        约束.被约束角色 = static_cast<指代参数角色>(*角色);
        约束.特征类型根 = *根;
        约束.允许值域 = *允许域;
        约束.特征类型快照.材料类型 = static_cast<特征概念材料物理类型>((*快照数组)[1]);
        约束.特征类型快照.比较规则 = static_cast<特征概念比较规则>((*快照数组)[2]);
        约束.特征类型快照.聚合规则 = static_cast<特征概念聚合规则>((*快照数组)[3]);
        约束.特征类型快照.值域 = *根域;
        std::size_t 单位数 = 0;
        for (const auto& 子字段 : 项.字段组) if (子字段.字段节点 == 指代键[FT单位键]) {
            ++单位数;
            const auto* 单位 = std::get_if<稳定编码>(&子字段.内容);
            if (!单位 || !有效(*单位)) return 指代内部状态::结构不一致;
            约束.特征类型快照.单位 = *单位;
        }
        if (单位数 > 1) return 指代内部状态::结构不一致;
        定义.特征约束组.push_back(std::move(约束));
        快照.结构.push_back(std::move(项));
    }
    auto 规范定义 = 定义;
    if (!规范化形状(规范定义) || 规范定义 != 定义)
        return 指代内部状态::结构不一致;
    快照.信息.push_back({节点, std::move(定义), std::move(名称)});
    return 指代内部状态::完成;
}

指代内部状态 读取候选全集(指代候选快照& 快照) {
    快照.召回 = 全局基础数据集.查询目标字段(指代键[定义类型], 节点类型字段);
    std::set<稳定编码> 节点身份;
    for (const auto& 字段 : 快照.召回) {
        const auto* 目标 = std::get_if<稳定编码>(&字段.内容);
        if (!目标 || *目标 != 指代键[定义类型]
            || !节点身份.insert(字段.所属节点).second)
            return 指代内部状态::结构不一致;
        const auto 状态 = 读取指代定义(字段.所属节点, 快照);
        if (状态 != 指代内部状态::完成) return 状态;
    }
    for (std::size_t i = 0; i < 快照.信息.size(); ++i)
        for (std::size_t j = i + 1; j < 快照.信息.size(); ++j)
            if (快照.信息[i].定义 == 快照.信息[j].定义)
                return 指代内部状态::结构不一致;
    return 指代内部状态::完成;
}

std::vector<稳定编码> 收集FT根(const 指代候选快照& 快照,
    const 指代概念定义* 请求 = nullptr) {
    std::vector<稳定编码> 结果;
    if (请求) for (const auto& 项 : 请求->特征约束组) 结果.push_back(项.特征类型根);
    for (const auto& 信息 : 快照.信息)
        for (const auto& 项 : 信息.定义.特征约束组) 结果.push_back(项.特征类型根);
    排序去重(结果);
    return 结果;
}

指代内部状态 映射FT状态(特征概念规则状态 状态) {
    switch (状态) {
    case 特征概念规则状态::已完成: return 指代内部状态::完成;
    case 特征概念规则状态::入口拒绝:
    case 特征概念规则状态::读取接口未装配: return 指代内部状态::读取未完成;
    case 特征概念规则状态::值不存在: return 指代内部状态::依赖缺失;
    case 特征概念规则状态::资源失败: return 指代内部状态::资源失败;
    default: return 指代内部状态::结构不一致;
    }
}

struct 指代FT探针 final {
    稳定编码 根;
    特征概念定义 正式;
    基础节点结构 基础;
    std::vector<基础外部关系> 入向;
};

指代内部状态 正式读取FT(概念_特征类& 服务,
    稳定编码 根, 特征概念定义& 定义) {
    const auto 自比较 = 服务.比较单特征概念(根, 根);
    const auto 状态 = 映射FT状态(自比较.状态);
    if (状态 != 指代内部状态::完成) return 状态;
    if (!自比较.关系 || *自比较.关系 != 单特征概念关系::同一
        || !自比较.特征类型根 || !有效(*自比较.特征类型根))
        return 指代内部状态::结构不一致;
    if (*自比较.特征类型根 != 根) return 指代内部状态::输入拒绝;
    const auto 读取 = 服务.获取特征概念(根);
    if (!读取) return 指代内部状态::读取未完成;
    定义 = {读取->材料类型, 读取->值域, 读取->单位,
        读取->比较规则, 读取->聚合规则, {}};
    if (定义.材料类型 != 特征概念材料物理类型::I64标量
        || 定义.比较规则 != 特征概念比较规则::I64数值
        || (定义.单位 && !有效(*定义.单位))) return 指代内部状态::输入拒绝;
    auto* 域 = std::get_if<std::vector<特征概念I64闭区间>>(&定义.值域);
    if (!域 || !规范化区间(*域)) return 指代内部状态::输入拒绝;
    return 指代内部状态::完成;
}

指代内部状态 验证约束组(概念_特征类& 服务,
    const 指代概念定义& 定义, const std::vector<指代FT探针>& 探针,
    bool 请求) {
    for (const auto& 项 : 定义.特征约束组) {
        const auto 位置 = std::find_if(探针.begin(), 探针.end(),
            [&](const auto& 值) { return 值.根 == 项.特征类型根; });
        if (位置 == 探针.end()) return 指代内部状态::结构不一致;
        if (项.特征类型快照 != 位置->正式)
            return 指代内部状态::依赖已变化;
        const auto 比较 = 服务.比较值域(特征概念材料物理类型::I64标量,
            特征概念值域{项.允许值域}, 位置->正式.值域);
        const auto 状态 = 映射FT状态(比较.状态);
        if (状态 != 指代内部状态::完成) return 状态;
        if (!比较.关系) return 指代内部状态::结构不一致;
        if (*比较.关系 != 特征概念值域关系::相等
            && *比较.关系 != 特征概念值域关系::右包含左)
            return 请求 ? 指代内部状态::输入拒绝 : 指代内部状态::结构不一致;
    }
    return 指代内部状态::完成;
}

struct 指代准备 final {
    指代候选快照 候选;
    std::vector<指代FT探针> FT;
};

指代内部状态 准备指代操作(概念_特征类& 服务,
    const 指代概念定义* 请求, 指代准备& 准备) {
    {
        std::lock_guard 锁(存在概念图互斥);
        const auto 结构 = 结构键状态();
        if (结构 != 指代内部状态::完成) return 结构;
        const auto 状态 = 读取候选全集(准备.候选);
        if (状态 != 指代内部状态::完成) return 状态;
    }
    for (const auto 根 : 收集FT根(准备.候选, 请求)) {
        指代FT探针 探针;
        探针.根 = 根;
        auto 状态 = 正式读取FT(服务, 根, 探针.正式);
        if (状态 != 指代内部状态::完成) return 状态;
        状态 = 读取结构(根, 探针.基础);
        if (状态 != 指代内部状态::完成) return 状态;
        探针.入向 = 全局基础数据集.查询目标关系(根, 基础外部关系类型::父子);
        特征概念定义 再读;
        状态 = 正式读取FT(服务, 根, 再读);
        if (状态 != 指代内部状态::完成) return 状态;
        if (再读 != 探针.正式) return 指代内部状态::依赖已变化;
        准备.FT.push_back(std::move(探针));
    }
    if (请求) {
        const auto 状态 = 验证约束组(服务, *请求, 准备.FT, true);
        if (状态 != 指代内部状态::完成) return 状态;
    }
    for (const auto& 信息 : 准备.候选.信息) {
        const auto 状态 = 验证约束组(服务, 信息.定义, 准备.FT, false);
        if (状态 != 指代内部状态::完成) return 状态;
    }
    return 指代内部状态::完成;
}

指代内部状态 复核基础准备(const 指代准备& 准备) {
    const auto 结构状态 = 结构键状态();
    if (结构状态 != 指代内部状态::完成) return 结构状态;
    指代候选快照 现在;
    auto 状态 = 读取候选全集(现在);
    if (状态 == 指代内部状态::结构不一致
        || 状态 == 指代内部状态::依赖缺失)
        return 指代内部状态::读取漂移;
    if (状态 != 指代内部状态::完成) return 状态;
    if (!同一候选快照(现在, 准备.候选)) return 指代内部状态::读取漂移;
    for (const auto& 项 : 准备.FT) {
        基础节点结构 当前;
        状态 = 读取结构(项.根, 当前);
        if (状态 == 指代内部状态::依赖缺失)
            return 指代内部状态::读取漂移;
        if (状态 != 指代内部状态::完成) return 状态;
        if (当前.节点 != 项.基础.节点 || 当前.字段组 != 项.基础.字段组
            || 全局基础数据集.查询目标关系(项.根,
                基础外部关系类型::父子) != 项.入向)
            return 指代内部状态::读取漂移;
    }
    return 指代内部状态::完成;
}

指代内部状态 发布后FT复核(概念_特征类& 服务,
    const 指代概念定义& 定义) {
    for (const auto& 项 : 定义.特征约束组) {
        特征概念定义 现在;
        const auto 状态 = 正式读取FT(服务, 项.特征类型根, 现在);
        if (状态 == 指代内部状态::输入拒绝)
            return 指代内部状态::依赖已变化;
        if (状态 != 指代内部状态::完成) return 状态;
        if (现在 != 项.特征类型快照) return 指代内部状态::依赖已变化;
    }
    return 指代内部状态::完成;
}

std::vector<基础组内新节点> 构造指代节点组(const 指代概念定义& 定义) {
    std::vector<基础组内新节点> 组(2 + 定义.特征约束组.size());
    const auto 加 = [&](std::size_t 索引, 稳定编码 键, 基础组内字段内容 内容) {
        组[索引].字段组.push_back({键, std::move(内容)});
    };
    const auto 加数 = [&](std::size_t 索引, 稳定编码 键, auto 值) {
        加(索引, 键, 基础值{static_cast<std::int64_t>(值)});
    };
    加(0, 节点类型字段, 指代键[定义类型]);
    加数(0, 指代键[范围键], 定义.目标范围);
    std::vector<std::int64_t> 参数;
    for (const auto& 项 : 定义.参数组) {
        参数.push_back(static_cast<std::int64_t>(项.角色));
        参数.push_back(static_cast<std::int64_t>(项.类型));
    }
    加(0, 指代键[参数键], 基础值{std::move(参数)});
    加(0, 指代键[规则项键], 基础组内节点引用{1});
    加(1, 节点类型字段, 指代键[规则类型]);
    加数(1, 指代键[规则种类键], 定义.规则);
    for (std::size_t i = 0; i < 定义.特征约束组.size(); ++i) {
        const auto& 项 = 定义.特征约束组[i];
        const auto 索引 = i + 2;
        加(0, 指代键[约束项键], 基础组内节点引用{索引});
        加(索引, 节点类型字段, 指代键[约束类型]);
        加数(索引, 指代键[约束角色键], 项.被约束角色);
        加(索引, 指代键[FT根键], 项.特征类型根);
        加(索引, 指代键[允许区间键], 基础值{编码区间(项.允许值域)});
        const auto& 根域 = std::get<std::vector<特征概念I64闭区间>>(
            项.特征类型快照.值域);
        auto 快照数组 = 编码区间(根域);
        快照数组.insert(快照数组.begin(), {
            1, static_cast<std::int64_t>(项.特征类型快照.材料类型),
            static_cast<std::int64_t>(项.特征类型快照.比较规则),
            static_cast<std::int64_t>(项.特征类型快照.聚合规则)});
        加(索引, 指代键[FT快照键], 基础值{std::move(快照数组)});
        if (项.特征类型快照.单位)
            加(索引, 指代键[FT单位键], *项.特征类型快照.单位);
    }
    return 组;
}

} // namespace

指代定义结构状态 概念_存在类::初始化指代定义结构() {
    std::lock_guard 锁(存在概念图互斥);
    if (!结构已初始化()) return 指代定义结构状态::入口拒绝;
    for (const auto 编码 : {存在概念图根节点, 节点类型字段,
        专属存在概念节点类型, 抽象存在概念节点类型, 对应存在字段,
        特征项字段, 特征项节点类型, 特征类型概念字段,
        具体特征概念字段, 名称关系字段}) {
        const auto 读取 = 全局基础数据集.查询完整节点结构(编码);
        if (读取.状态 == 基础结构操作状态::资源失败)
            return 指代定义结构状态::资源失败;
        if (读取.状态 != 基础结构操作状态::已完成 || !读取.结构)
            return 指代定义结构状态::内部不一致;
    }
    const auto 既有状态 = 结构键状态();
    if (既有状态 == 指代内部状态::完成)
        return 指代定义结构状态::已就绪;
    if (std::any_of(指代键.begin(), 指代键.end(), [](auto 值) { return 有效(值); }))
        return 既有状态 == 指代内部状态::资源失败
            ? 指代定义结构状态::资源失败 : 指代定义结构状态::内部不一致;
    auto& 待核验 = 指代待核验键;
    if (std::all_of(待核验.begin(), 待核验.end(), [](auto 值) { return !有效(值); })) {
        std::vector<基础组内新节点> 请求(13);
        const auto 建立 = 全局基础数据集.原子建立节点组(请求);
        if (建立.状态 == 基础结构操作状态::资源失败)
            return 指代定义结构状态::资源失败;
        if (建立.状态 != 基础结构操作状态::已完成)
            return 指代定义结构状态::内部不一致;
        for (std::size_t i = 0; i < std::min(待核验.size(), 建立.节点组.size()); ++i)
            待核验[i] = 建立.节点组[i];
        if (建立.节点组.size() != 待核验.size())
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__,
                L"初始化指代定义结构", L"L1 已发布技术节点，但返回身份数量违反原子组合同。");
    }
    std::set<稳定编码> 身份;
    for (const auto 编码 : 待核验) {
        if (!有效(编码) || 编码 == 节点类型字段 || !身份.insert(编码).second)
            return 指代定义结构状态::内部不一致;
        const auto 读取 = 全局基础数据集.查询完整节点结构(编码);
        if (读取.状态 == 基础结构操作状态::资源失败)
            return 指代定义结构状态::资源失败;
        if (读取.状态 != 基础结构操作状态::已完成 || !读取.结构
            || !读取.结构->字段组.empty()) return 指代定义结构状态::内部不一致;
    }
    指代键 = 待核验;
    return 指代定义结构状态::已就绪;
}

指代概念查询结果 概念_存在类::查询指代概念(
    const 指代概念定义& 请求) const {
    指代概念查询结果 结果;
    auto 规范请求 = 请求;
    if (!规范化形状(规范请求)) return 结果;
    指代准备 准备;
    auto 状态 = 准备指代操作(特征概念服务_, &规范请求, 准备);
    if (状态 != 指代内部状态::完成) {
        结果.状态 = 查询状态(状态);
        return 结果;
    }
    std::optional<指代概念信息> 命中;
    {
        std::lock_guard 锁(存在概念图互斥);
        状态 = 复核基础准备(准备);
        if (状态 != 指代内部状态::完成) {
            结果.状态 = 查询状态(状态);
            return 结果;
        }
        for (const auto& 候选 : 准备.候选.信息) if (候选.定义 == 规范请求) {
            if (命中) {
                结果.状态 = 指代概念查询状态::结构不一致;
                return 结果;
            }
            命中 = 候选;
        }
    }
    if (!命中) { 结果.状态 = 指代概念查询状态::未找到; return 结果; }
    状态 = 发布后FT复核(特征概念服务_, 命中->定义);
    if (状态 != 指代内部状态::完成) {
        结果.状态 = 查询状态(状态);
        return 结果;
    }
    结果.状态 = 指代概念查询状态::已找到;
    结果.信息 = std::move(命中);
    return 结果;
}

指代概念查询结果 概念_存在类::获取指代概念(稳定编码 概念节点) const {
    指代概念查询结果 结果;
    if (!有效(概念节点)) return 结果;
    {
        std::lock_guard 锁(存在概念图互斥);
        const auto 结构状态 = 结构键状态();
        if (结构状态 != 指代内部状态::完成) {
            结果.状态 = 查询状态(结构状态);
            return 结果;
        }
        基础节点结构 目标;
        const auto 读取状态 = 读取结构(概念节点, 目标);
        if (读取状态 == 指代内部状态::依赖缺失) {
            结果.状态 = 指代概念查询状态::未找到;
            return 结果;
        }
        if (读取状态 != 指代内部状态::完成) {
            结果.状态 = 查询状态(读取状态);
            return 结果;
        }
        std::size_t 类型数量 = 0;
        std::optional<稳定编码> 类型;
        for (const auto& 字段 : 目标.字段组) if (字段.字段节点 == 节点类型字段) {
            ++类型数量;
            if (const auto* 引用 = std::get_if<稳定编码>(&字段.内容)) 类型 = *引用;
        }
        if (类型数量 > 1 || (类型数量 == 1 && (!类型 || !有效(*类型)))) {
            结果.状态 = 指代概念查询状态::结构不一致;
            return 结果;
        }
        if (!类型 || *类型 != 指代键[定义类型]) {
            结果.状态 = 指代概念查询状态::未找到;
            return 结果;
        }
    }
    指代准备 准备;
    auto 状态 = 准备指代操作(特征概念服务_, nullptr, 准备);
    if (状态 != 指代内部状态::完成) {
        结果.状态 = 查询状态(状态);
        return 结果;
    }
    if (std::none_of(准备.候选.信息.begin(), 准备.候选.信息.end(),
        [&](const auto& 项) { return 项.节点 == 概念节点; })) {
        结果.状态 = 指代概念查询状态::结构不一致;
        return 结果;
    }
    std::optional<指代概念信息> 命中;
    {
        std::lock_guard 锁(存在概念图互斥);
        状态 = 复核基础准备(准备);
        if (状态 != 指代内部状态::完成) {
            结果.状态 = 查询状态(状态);
            return 结果;
        }
        for (const auto& 候选 : 准备.候选.信息) if (候选.节点 == 概念节点) {
            命中 = 候选;
            break;
        }
    }
    if (!命中) { 结果.状态 = 指代概念查询状态::未找到; return 结果; }
    状态 = 发布后FT复核(特征概念服务_, 命中->定义);
    if (状态 != 指代内部状态::完成) {
        结果.状态 = 查询状态(状态);
        return 结果;
    }
    结果.状态 = 指代概念查询状态::已找到;
    结果.信息 = std::move(命中);
    return 结果;
}

指代概念写入结果 概念_存在类::建立或取得指代概念(
    const 指代概念定义& 请求) {
    指代概念写入结果 结果;
    try {
        auto 规范请求 = 请求;
        if (!规范化形状(规范请求)) return 结果;
        指代准备 准备;
        auto 状态 = 准备指代操作(特征概念服务_, &规范请求, 准备);
        if (状态 != 指代内部状态::完成) {
            结果.状态 = 写入状态(状态);
            return 结果;
        }
        bool 新建 = false;
        std::optional<指代概念信息> 读回信息;
        {
            std::lock_guard 锁(存在概念图互斥);
            状态 = 复核基础准备(准备);
            if (状态 != 指代内部状态::完成) {
                结果.状态 = 写入状态(状态);
                return 结果;
            }
            for (const auto& 候选 : 准备.候选.信息) if (候选.定义 == 规范请求) {
                if (结果.已发生身份) {
                    结果.状态 = 指代概念写入状态::结构不一致;
                    return 结果;
                }
                结果.已发生身份 = 候选.节点;
                结果.阶段 = 指代概念发布阶段::已发布待核验;
            }
            if (!结果.已发生身份) {
                const auto 建立 = 全局基础数据集.原子建立节点组(
                    构造指代节点组(规范请求));
                if (建立.状态 != 基础结构操作状态::已完成) {
                    结果.状态 = 写入状态(映射基础状态(建立.状态));
                    return 结果;
                }
                if (建立.节点组.empty() || !有效(建立.节点组.front()))
                    工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__,
                        L"建立或取得指代概念", L"L1 已完成发布但未返回真实定义身份。");
                // 先保存真实身份，再核验返回的剩余组形状。
                结果.已发生身份 = 建立.节点组.front();
                结果.阶段 = 指代概念发布阶段::已发布待核验;
                if (建立.节点组.size() != 2 + 规范请求.特征约束组.size()) {
                    结果.状态 = 指代概念写入状态::结构不一致;
                    return 结果;
                }
                // L1 已发布：任何随后读回、复制或诊断异常都保留真实身份。
                新建 = true;
            }
            指代候选快照 读回;
            状态 = 读取指代定义(*结果.已发生身份, 读回);
            if (状态 != 指代内部状态::完成) {
                结果.状态 = 写入状态(状态);
                return 结果;
            }
            if (读回.信息.size() != 1 || 读回.信息.front().定义 != 规范请求) {
                结果.状态 = 指代概念写入状态::结构不一致;
                return 结果;
            }
            读回信息 = std::move(读回.信息.front());
        }
        状态 = 发布后FT复核(特征概念服务_, 规范请求);
        if (状态 != 指代内部状态::完成) {
            结果.状态 = 写入状态(状态);
            return 结果;
        }
        // 身份与完整信息同时核验后才宣称本次建立或复用成功。
        结果.信息 = std::move(读回信息);
        结果.阶段 = 指代概念发布阶段::已完整读回;
        结果.状态 = 新建 ? 指代概念写入状态::已建立
            : 指代概念写入状态::已复用;
        return 结果;
    } catch (...) {
        if (!结果.已发生身份) throw;
        结果.信息.reset();
        结果.阶段 = 指代概念发布阶段::已发布待核验;
        结果.状态 = 指代概念写入状态::资源失败;
        结果.发布后异常 = std::current_exception();
        return 结果;
    }
}

namespace {
bool 同一联合快照(const 基础联合节点快照& 左,
    const 基础联合节点快照& 右) {
    return 左.结构.节点 == 右.结构.节点
        && 左.结构.字段组 == 右.结构.字段组
        && 左.入向外部关系组 == 右.入向外部关系组
        && 左.出向外部关系组 == 右.出向外部关系组
        && 左.入向字段组 == 右.入向字段组;
}

基础互引节点操作状态 合并名称预期(
    const std::vector<基础联合节点快照>& 概念侧,
    const std::vector<基础联合节点快照>& 名称侧,
    std::vector<基础联合节点快照>& 输出) {
    using 状态 = 基础互引节点操作状态;
    std::map<稳定编码, 基础联合节点快照> 合并;
    for (const auto* 来源 : {&概念侧, &名称侧}) {
        for (const auto& 项 : *来源) {
            const auto 编码 = 项.结构.节点.编码;
            if (!有效(编码)) return 状态::入口拒绝;
            const auto [位置, 新增] = 合并.emplace(编码, 项);
            if (!新增 && !同一联合快照(位置->second, 项))
                return 状态::预期不符;
        }
    }
    for (auto& [_, 项] : 合并) 输出.push_back(std::move(项));
    return 状态::已完成;
}
} // namespace

指代名称联合建立请求结果 指代名称反向参与规格::形成建立请求(
    const 基础节点建立请求& 名称节点请求,
    const std::vector<基础联合节点快照>& 名称侧预期) const {
    using 状态 = 基础互引节点操作状态;
    指代名称联合建立请求结果 结果;
    if (!有效(概念节点_) || !有效(名称字段节点_)
        || !名称节点请求.上级节点 || !有效(*名称节点请求.上级节点))
        return 结果;
    基础互引节点建立请求 请求;
    请求.新节点 = 名称节点请求;
    请求.反向引用所属节点 = 概念节点_;
    请求.反向引用字段节点 = 名称字段节点_;
    结果.状态 = 合并名称预期(预期节点组_, 名称侧预期, 请求.预期节点组);
    if (结果.状态 == 状态::已完成) 结果.请求 = std::move(请求);
    return 结果;
}

指代名称联合删除请求结果 指代名称反向参与规格::形成删除请求(
    稳定编码 名称节点, 稳定编码 名称上级节点,
    const std::vector<基础联合节点快照>& 名称侧预期) const {
    using 状态 = 基础互引节点操作状态;
    指代名称联合删除请求结果 结果;
    if (!有效(名称节点) || !有效(名称上级节点)) return 结果;
    std::optional<稳定编码> 反向身份;
    for (const auto& 字段 : 反向名称字段组_) {
        const auto* 目标 = std::get_if<稳定编码>(&字段.内容);
        if (目标 && *目标 == 名称节点) {
            if (反向身份) return 结果;
            反向身份 = 字段.编码;
        }
    }
    if (!反向身份) return 结果;
    基础互引节点删除请求 请求;
    请求.节点 = 名称节点;
    请求.上级节点 = 名称上级节点;
    请求.反向字段关系 = *反向身份;
    结果.状态 = 合并名称预期(预期节点组_, 名称侧预期, 请求.预期节点组);
    if (结果.状态 == 状态::已完成) 结果.请求 = std::move(请求);
    return 结果;
}

指代概念保护结果 概念_存在类::读取指代概念保护(稳定编码 概念节点) const {
    using S = 指代概念查询状态;
    try {
        const auto 正式 = 获取指代概念(概念节点);
        if (正式.状态 != S::已找到) {
            if (正式.状态 == S::结构不一致)
                throw std::logic_error("pronoun concept structure corrupt");
            return {正式.状态, {}, {}};
        }
        if (!正式.信息) throw std::logic_error("pronoun concept success has no material");
        // 复用原纯读取准备/复核；不创建反向写规格或建立/删除请求。
        指代准备 准备;
        auto 状态 = 准备指代操作(特征概念服务_, &正式.信息->定义, 准备);
        if (状态 != 指代内部状态::完成) {
            if (状态 == 指代内部状态::结构不一致)
                throw std::logic_error("pronoun concept preparation corrupt");
            return {查询状态(状态), {}, {}};
        }
        std::vector<基础联合节点快照> 特征保护;
        for (const auto& FT : 准备.FT) {
            const auto 读 = 特征概念服务_.读取特征概念定义保护(FT.根);
            if (读.前提已变化) return {S::读取漂移, {}, {}};
            if (读.状态 == 基础结构操作状态::资源失败) return {S::资源失败, {}, {}};
            if (读.状态 != 基础结构操作状态::已完成 || !读.信息)
                throw std::logic_error("pronoun constraint feature definition unavailable");
            const auto& 信息 = 读.信息->定义;
            const 特征概念定义 定义{信息.材料类型, 信息.值域, 信息.单位,
                信息.比较规则, 信息.聚合规则, {}};
            if (定义 != FT.正式 || 读.信息->类型根 != FT.根)
                return {S::依赖已变化, {}, {}};
            特征保护.insert(特征保护.end(), 读.预期节点组.begin(), 读.预期节点组.end());
        }
        std::lock_guard 锁(存在概念图互斥);
        状态 = 复核基础准备(准备);
        if (状态 != 指代内部状态::完成) {
            if (状态 == 指代内部状态::结构不一致)
                throw std::logic_error("pronoun concept recheck corrupt");
            return {查询状态(状态), {}, {}};
        }
        const auto 命中 = std::find_if(准备.候选.信息.begin(), 准备.候选.信息.end(),
            [&](const auto& 项) { return 项.节点 == 概念节点; });
        if (命中 == 准备.候选.信息.end() || *命中 != *正式.信息)
            return {S::读取漂移, {}, {}};
        std::set<稳定编码> 身份{概念节点, 名称关系字段};
        身份.insert(指代键.begin(), 指代键.end());
        身份.insert(存在概念图根节点);
        身份.insert(节点类型字段);
        for (const auto& 项 : 准备.候选.结构) 身份.insert(项.节点.编码);
        for (const auto& 项 : 特征保护) 身份.insert(项.结构.节点.编码);
        for (const auto& 项 : 准备.候选.信息)
            身份.insert(项.名称关系.begin(), 项.名称关系.end());
        const auto 联合 = 全局基础数据集.查询节点组完整结构({身份.begin(), 身份.end()});
        if (联合.状态 == 基础结构操作状态::资源失败) return {S::资源失败, {}, {}};
        if (联合.状态 != 基础结构操作状态::已完成 || 联合.节点组.size() != 身份.size())
            throw std::logic_error("pronoun protection joint read failed");
        std::map<稳定编码, const 基础联合节点快照*> 当前;
        指代概念保护结果 结果{S::已找到, *命中, {}};
        for (const auto& 项 : 联合.节点组) {
            if (!项.快照) return {S::读取漂移, {}, {}};
            当前.emplace(项.查询节点, &*项.快照);
            结果.完整保护.push_back(*项.快照);
        }
        for (const auto& 前 : 准备.候选.结构) {
            const auto& 后 = *当前.at(前.节点.编码);
            if (前.节点 != 后.结构.节点 || 前.字段组 != 后.结构.字段组)
                return {S::读取漂移, {}, {}};
        }
        for (const auto& 前 : 特征保护) {
            const auto& 后 = *当前.at(前.结构.节点.编码);
            if (前.结构.节点 != 后.结构.节点 || 前.结构.字段组 != 后.结构.字段组
                || 前.入向字段组 != 后.入向字段组
                || 前.入向外部关系组 != 后.入向外部关系组
                || 前.出向外部关系组 != 后.出向外部关系组)
                return {S::读取漂移, {}, {}};
        }
        return 结果;
    } catch (const std::bad_alloc&) { return {S::资源失败, {}, {}}; }
    catch (const std::length_error&) { return {S::资源失败, {}, {}}; }
}

指代名称反向参与结果 概念_存在类::准备指代名称反向参与(
    稳定编码 概念节点, const 指代概念定义& 预期定义) const {
    指代名称反向参与结果 结果;
    auto 规范定义 = 预期定义;
    if (!有效(概念节点) || !规范化形状(规范定义)) return 结果;
    const auto 正式 = 获取指代概念(概念节点);
    if (正式.状态 != 指代概念查询状态::已找到) {
        结果.状态 = 正式.状态;
        return 结果;
    }
    if (!正式.信息) {
        结果.状态 = 指代概念查询状态::结构不一致;
        return 结果;
    }
    if (正式.信息->定义 != 规范定义) {
        结果.状态 = 指代概念查询状态::依赖已变化;
        return 结果;
    }
    指代准备 准备;
    auto 状态 = 准备指代操作(特征概念服务_, &规范定义, 准备);
    if (状态 != 指代内部状态::完成) {
        结果.状态 = 查询状态(状态);
        return 结果;
    }
    std::lock_guard 锁(存在概念图互斥);
    状态 = 复核基础准备(准备);
    if (状态 != 指代内部状态::完成) {
        结果.状态 = 查询状态(状态);
        return 结果;
    }
    const auto 命中 = std::find_if(准备.候选.信息.begin(), 准备.候选.信息.end(),
        [&](const auto& 项) { return 项.节点 == 概念节点; });
    if (命中 == 准备.候选.信息.end() || *命中 != *正式.信息) {
        结果.状态 = 指代概念查询状态::读取漂移;
        return 结果;
    }
    std::set<稳定编码> 身份{概念节点, 名称关系字段};
    for (const auto& 项 : 准备.候选.结构) 身份.insert(项.节点.编码);
    for (const auto& 项 : 准备.FT) 身份.insert(项.根);
    for (const auto 名称 : 命中->名称关系) 身份.insert(名称);
    const std::vector<稳定编码> 查询身份(身份.begin(), 身份.end());
    const auto 联合 = 全局基础数据集.查询节点组完整结构(查询身份);
    if (联合.状态 != 基础结构操作状态::已完成) {
        结果.状态 = 查询状态(映射基础状态(联合.状态));
        return 结果;
    }
    指代名称反向参与规格 规格;
    规格.概念节点_ = 概念节点;
    规格.名称字段节点_ = 名称关系字段;
    if (联合.节点组.size() != 身份.size()) {
        结果.状态 = 指代概念查询状态::结构不一致;
        return 结果;
    }
    for (const auto& 前 : 准备.候选.结构) {
        const auto 位置 = std::find_if(联合.节点组.begin(), 联合.节点组.end(),
            [&](const auto& 项) { return 项.查询节点 == 前.节点.编码; });
        if (位置 == 联合.节点组.end() || !位置->快照
            || 位置->快照->结构.节点 != 前.节点
            || 位置->快照->结构.字段组 != 前.字段组) {
            结果.状态 = 指代概念查询状态::读取漂移;
            return 结果;
        }
    }
    for (const auto& 前 : 准备.FT) {
        const auto 位置 = std::find_if(联合.节点组.begin(), 联合.节点组.end(),
            [&](const auto& 项) { return 项.查询节点 == 前.根; });
        if (位置 == 联合.节点组.end() || !位置->快照
            || 位置->快照->结构.节点 != 前.基础.节点
            || 位置->快照->结构.字段组 != 前.基础.字段组
            || 位置->快照->入向外部关系组 != 前.入向) {
            结果.状态 = 指代概念查询状态::读取漂移;
            return 结果;
        }
    }
    for (const auto& 项 : 联合.节点组) {
        if (!项.快照) {
            结果.状态 = 指代概念查询状态::结构不一致;
            return 结果;
        }
        if (项.查询节点 == 概念节点) {
            for (const auto& 字段 : 项.快照->结构.字段组)
                if (字段.字段节点 == 名称关系字段)
                    规格.反向名称字段组_.push_back(字段);
            std::vector<稳定编码> 当前名称;
            for (const auto& 字段 : 规格.反向名称字段组_) {
                const auto* 引用 = std::get_if<稳定编码>(&字段.内容);
                if (!引用 || !有效(*引用)) {
                    结果.状态 = 指代概念查询状态::结构不一致;
                    return 结果;
                }
                当前名称.push_back(*引用);
            }
            std::sort(当前名称.begin(), 当前名称.end());
            if (当前名称 != 命中->名称关系) {
                结果.状态 = 指代概念查询状态::读取漂移;
                return 结果;
            }
        }
        规格.预期节点组_.push_back(*项.快照);
    }
    结果.状态 = 指代概念查询状态::已找到;
    结果.规格 = std::move(规格);
    return 结果;
}

存在概念上位保护结果 概念_存在类::读取存在概念上位保护(稳定编码 节点) const noexcept {
    try {
        std::lock_guard 锁(存在概念图互斥);
        const auto 读 = 全局基础数据集.查询节点组完整结构({节点});
        if (读.状态 != 基础结构操作状态::已完成) return {读.状态, {}, {}, false};
        if (读.节点组.size() != 1 || !读.节点组.front().快照)
            return {基础结构操作状态::节点不存在, {}, {}, false};
        const auto& s = *读.节点组.front().快照;
        std::optional<稳定编码> 类型;
        for (const auto& f : s.结构.字段组) if (f.字段节点 == 节点类型字段) {
            const auto* n = std::get_if<稳定编码>(&f.内容);
            if (!n || 类型) return {基础结构操作状态::内部不一致, {}, {}, false};
            类型 = *n;
        }
        if (类型 != 专属存在概念节点类型 && 类型 != 抽象存在概念节点类型)
            return {基础结构操作状态::内部不一致, {}, {}, false};
        存在概念上位保护信息 v{节点, {}};
        for (const auto& r : s.入向外部关系组)
            if (r.类型 == 基础外部关系类型::父子 && r.角色或顺序 == 概念上下位关系角色)
                v.上位概念组.push_back(r.源节点);
        std::sort(v.上位概念组.begin(), v.上位概念组.end());
        if (std::adjacent_find(v.上位概念组.begin(), v.上位概念组.end()) != v.上位概念组.end())
            return {基础结构操作状态::内部不一致, {}, {}, false};
        return {基础结构操作状态::已完成, std::move(v), {s}, false};
    } catch (...) { return {基础结构操作状态::资源失败, {}, {}, false}; }
}

} // namespace 海中鱼巣

namespace 海中鱼巣 {
namespace {
using 正式状态=正式结构操作状态;
enum 正式定义键序 : std::size_t {
    基础类型键,约束类型键,成员类型键,结构类型键,项目类型键,收窄类型键,路径类型键,登记类型键,
    原语键,整数键,引用键,成员概念键,最少成员键,最多成员键,有序键,全部键,
    上位键,项目键,收窄键,约束键,序号键,项目FT键,内容概念键,路径键,
    槽号键,槽目标键,登记键,正式键数
};
std::array<稳定编码,正式键数> 正式键{};
[[noreturn]] void 正式定义故障(const char* message) {
    工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"先天信息概念提供者",
        L"定义读取或发布工程错误，不作为业务否定。",message);
}
bool 正式schema就绪() {
    return 结构已初始化()&&std::all_of(正式键.begin(),正式键.end(),[](auto n){return 有效(n);});
}
bool 初始化正式schema() {
    std::lock_guard lock(存在概念图互斥);
    if (!结构仍存在()) return false;
    if (正式schema就绪()) {
        for(auto n:正式键) if(!节点仍存在(n)) 正式定义故障("schema endpoint missing");
        return true;
    }
    if(std::any_of(正式键.begin(),正式键.end(),[](auto n){return 有效(n);})) 正式定义故障("partial schema");
    auto r=全局基础数据集.原子建立节点组(std::vector<基础组内新节点>(正式键数));
    if(r.状态!=基础结构操作状态::已完成||r.节点组.size()!=正式键数) 正式定义故障("schema publication failed");
    std::copy(r.节点组.begin(),r.节点组.end(),正式键.begin());
    return true;
}
bool 正式快照相同(const 基础联合节点快照&a,const 基础联合节点快照&b) {
    const bool same=a.结构.节点==b.结构.节点&&a.结构.字段组==b.结构.字段组&&a.入向字段组==b.入向字段组
        &&a.入向外部关系组==b.入向外部关系组&&a.出向外部关系组==b.出向外部关系组;
#ifdef HY_EGO_DEBUG_LOG_RECEIVED_DEMAND_PARSE
    // 只观察本次比较已有的两份快照，禁止为诊断补读或刷新保护。
    if(!same)try {
        std::wostringstream text;text<<L"节点="<<a.结构.节点.编码.值
            <<L"；字段差异="<<(a.结构.字段组!=b.结构.字段组)
            <<L"；入字段差异="<<(a.入向字段组!=b.入向字段组)
            <<L"；入关系差异="<<(a.入向外部关系组!=b.入向外部关系组)
            <<L"；出关系差异="<<(a.出向外部关系组!=b.出向外部关系组);
        std::size_t count=0;
        const auto fields=[&](const wchar_t* label,const auto& x,const auto& y) {
            for(const auto& f:x)if(std::find(y.begin(),y.end(),f)==y.end()&&count++<16) {
                text<<L"；"<<label<<L"[关系="<<f.编码.值<<L",owner="<<f.所属节点.值<<L",字段="<<f.字段节点.值;
                if(const auto* ref=std::get_if<稳定编码>(&f.内容))text<<L",引用="<<ref->值;
                text<<L"]";
            }
        };
        fields(L"原字段",a.结构.字段组,b.结构.字段组);fields(L"对照字段",b.结构.字段组,a.结构.字段组);
        fields(L"原入字段",a.入向字段组,b.入向字段组);fields(L"对照入字段",b.入向字段组,a.入向字段组);
        (void)记录调试日志(L"ASK_BACK_INFORMATION_GUARD",L"概念owner",L"正式快照比较差异",text.str());
    }catch(...){}
#endif
    return same;
}
基础联合节点快照 正式快照(稳定编码 n) {
    auto r=全局基础数据集.查询节点组完整结构({n});
    if(r.状态!=基础结构操作状态::已完成||r.节点组.size()!=1||!r.节点组[0].快照)
        正式定义故障("formal endpoint unreadable");
    return *r.节点组[0].快照;
}
// 仅属于一次正式读取调用；不缓存末尾真实复核，也不跨线程/调用存活。
struct 正式读取上下文 final {
    std::map<稳定编码,基础联合节点快照> 快照组;
    std::map<稳定编码,正式结构概念信息> 已完成定义组;
    const 基础联合节点快照& 读取(稳定编码 n) {
        const auto old=快照组.find(n);
        if(old!=快照组.end())return old->second;
        const auto it=快照组.emplace(n,正式快照(n)).first;
        return it->second;
    }
};
void 并保护(std::vector<基础联合节点快照>&all,const std::vector<基础联合节点快照>&add) {
    for(const auto&s:add) {
        auto it=std::find_if(all.begin(),all.end(),[&](const auto&t){return s.结构.节点==t.结构.节点;});
        if(it==all.end()) all.push_back(s);
        else if(!正式快照相同(*it,s)) throw 正式状态::前提已变化;
    }
}
bool 正式保护未变(const std::vector<基础联合节点快照>&all) {
    if(all.empty()) return true;
    std::set<稳定编码> 身份;
    for(const auto&s:all) {
        if(!有效(s.结构.节点.编码)) 正式定义故障("formal endpoint unreadable");
        身份.insert(s.结构.节点.编码);
    }
    const std::vector<稳定编码> 节点组(身份.begin(),身份.end());
    const auto 联合=全局基础数据集.查询节点组完整结构(节点组);
    if(联合.状态!=基础结构操作状态::已完成||联合.节点组.size()!=节点组.size())
        正式定义故障("formal endpoint unreadable");
    std::map<稳定编码,const 基础联合节点快照*> 当前;
    for(std::size_t i=0;i<节点组.size();++i) {
        const auto&项=联合.节点组[i];
        if(项.查询节点!=节点组[i]||!项.快照||项.快照->结构.节点.编码!=节点组[i])
            正式定义故障("formal endpoint unreadable");
        当前.emplace(项.查询节点,&*项.快照);
    }
    bool 相同=true;
    for(const auto&s:all)
        if(!正式快照相同(s,*当前.at(s.结构.节点.编码))) 相同=false;
    return 相同;
}
std::vector<稳定编码> 正式引用组(const 基础节点结构&s,稳定编码 key) {
    std::vector<稳定编码> out;
    for(const auto&f:s.字段组) if(f.字段节点==key) {
        auto p=std::get_if<稳定编码>(&f.内容);
        if(!p||!有效(*p)) 正式定义故障("invalid reference field");
        out.push_back(*p);
    }
    return out;
}
稳定编码 正式单引用(const 基础节点结构&s,稳定编码 key) {
    auto a=正式引用组(s,key); if(a.size()!=1) 正式定义故障("reference cardinality"); return a[0];
}
std::vector<std::int64_t> 正式整数组(const 基础节点结构&s,稳定编码 key) {
    const std::vector<std::int64_t>*value=nullptr;
    for(const auto&f:s.字段组) if(f.字段节点==key) {
        auto b=std::get_if<基础值>(&f.内容);
        if(value||!b||!(value=std::get_if<std::vector<std::int64_t>>(&b->材料))) 正式定义故障("integer vector field");
    }
    if(!value) 正式定义故障("missing integer vector"); return *value;
}
std::int64_t 正式整数(const 基础节点结构&s,稳定编码 key) {
    const std::int64_t*value=nullptr;
    for(const auto&f:s.字段组) if(f.字段节点==key) {
        auto b=std::get_if<基础值>(&f.内容);
        if(value||!b||!(value=std::get_if<std::int64_t>(&b->材料))) 正式定义故障("integer field");
    }
    if(!value) 正式定义故障("missing integer"); return *value;
}
std::uint32_t 正式序号(const 基础节点结构&s,稳定编码 key) {
    auto x=正式整数(s,key); if(x<0||x>UINT32_MAX) 正式定义故障("index out of range"); return static_cast<std::uint32_t>(x);
}
std::map<std::uint32_t,稳定编码> 读取正式登记(std::vector<基础联合节点快照>&protect) {
    auto root=正式快照(存在概念图根节点); 并保护(protect,{root});
    std::map<std::uint32_t,稳定编码> out;
    for(auto n:正式引用组(root.结构,正式键[登记键])) {
        auto s=正式快照(n); 并保护(protect,{s});
        if(正式单引用(s.结构,节点类型字段)!=正式键[登记类型键]) 正式定义故障("slot record type");
        auto slot=正式序号(s.结构,正式键[槽号键]);
        if(!out.emplace(slot,正式单引用(s.结构,正式键[槽目标键])).second) 正式定义故障("duplicate definition slot");
    }
    return out;
}
std::optional<std::size_t> 正式形状(const 基础节点结构&s) {
    auto types=正式引用组(s,节点类型字段);
    if(types.size()!=1) {
        for(auto t:types)for(std::size_t i=基础类型键;i<=结构类型键;++i)
            if(t==正式键[i])正式定义故障("multiple formal type tags");
        return {};
    }
    for(std::size_t i=基础类型键;i<=结构类型键;++i) if(types[0]==正式键[i]) return i;
    return {};
}
void 核正式字段(const 基础节点结构&结构,std::initializer_list<稳定编码>允许字段) {
    for(const auto&字段:结构.字段组)
        if(std::find(允许字段.begin(),允许字段.end(),字段.字段节点)==允许字段.end())
            正式定义故障("undeclared formal definition field");
}
std::optional<稳定编码> 读取根登记(std::vector<基础联合节点快照>&保护) {
    auto s=正式快照(存在概念图根节点);并保护(保护,{s,正式快照(存在语义根字段)});
    auto refs=正式引用组(s.结构,存在语义根字段);
    if(refs.size()>1)正式定义故障("multiple existence semantic roots");
    if(refs.empty())return {};
    return refs.front();
}
void 核普通分类端点(const 基础联合节点快照&s) {
    const auto t=正式单引用(s.结构,节点类型字段);
    if(t!=抽象存在概念节点类型&&t!=专属存在概念节点类型)正式定义故障("ordinary classification endpoint type");
    unsigned count=0;
    for(const auto&r:s.入向外部关系组)if(r.类型==基础外部关系类型::父子&&r.角色或顺序==组织归属关系角色){
        if(r.源节点!=存在概念图根节点)正式定义故障("foreign ordinary classification membership");++count;}
    if(count!=1)正式定义故障("ordinary classification membership");
}
void 核存在根证明(稳定编码 n,std::vector<基础联合节点快照>&保护,bool 允许未登记=false) {
    const auto root=读取根登记(保护);
    if((!root&&!允许未登记)||(root&&*root!=n))正式定义故障("unregistered existence root");
    auto s=正式快照(n);并保护(保护,{s,正式快照(节点类型字段),正式快照(抽象存在概念节点类型)});
    核普通分类端点(s);核正式字段(s.结构,{节点类型字段,名称关系字段});
    if(正式单引用(s.结构,节点类型字段)!=抽象存在概念节点类型)正式定义故障("semantic root is not abstract");
    for(const auto&r:s.入向外部关系组)if(r.类型==基础外部关系类型::父子&&r.角色或顺序==概念上下位关系角色)
        正式定义故障("semantic root has parent");
    for(auto x:正式引用组(s.结构,名称关系字段))并保护(保护,{正式快照(x)});
    std::set<稳定编码>active,done;
    std::function<void(稳定编码)>visit=[&](稳定编码 x){
        if(done.count(x))return;if(!active.insert(x).second)正式定义故障("ordinary classification cycle");
        auto q=正式快照(x);并保护(保护,{q});核普通分类端点(q);
        std::set<稳定编码>children;
        for(const auto&r:q.出向外部关系组)if(r.类型==基础外部关系类型::父子&&r.角色或顺序==概念上下位关系角色){
            if(!children.insert(r.目标节点).second)正式定义故障("duplicate ordinary child");visit(r.目标节点);}
        active.erase(x);done.insert(x);
    };visit(n);
}
bool 是登记存在根(稳定编码 n) {
    std::vector<基础联合节点快照>p;auto r=读取根登记(p);if(!r||*r!=n)return false;
    核存在根证明(n,p);return 正式保护未变(p);
}
void 核基础定义(const 基础范围概念定义&d) {
    const auto p=static_cast<unsigned>(d.原语);
    if(p<1||p>10) 正式定义故障("unknown primitive");
    if(p<=6) {if(!d.整数参数.empty()||!d.引用参数.empty()) 正式定义故障("primitive arguments");}
    else if(d.原语==基础概念原语::I64闭区间) {
        if(d.整数参数.size()!=2||d.整数参数[0]>d.整数参数[1]||!d.引用参数.empty()) 正式定义故障("interval arguments");
    } else if(d.原语==基础概念原语::I64有限枚举) {
        if(d.整数参数.empty()||!std::is_sorted(d.整数参数.begin(),d.整数参数.end())
            ||std::adjacent_find(d.整数参数.begin(),d.整数参数.end())!=d.整数参数.end()||!d.引用参数.empty()) 正式定义故障("enumeration arguments");
    } else if(d.原语==基础概念原语::具名领域身份) {
        if(d.整数参数.size()!=1||d.整数参数[0]<1||d.整数参数[0]>15||!d.引用参数.empty()) 正式定义故障("named identity arguments");
    } else if(!d.整数参数.empty()||d.引用参数.size()!=1) 正式定义故障("unique existence arguments");
}
// 解码只读当前owner/L1；跨owner身份复核在调用者持本地锁之前进行。
正式结构概念信息 解码正式定义(稳定编码 n,std::vector<基础联合节点快照>&protect,
    std::set<稳定编码>&visited,bool 展开引用=true,
    const std::map<稳定编码,const 基础联合节点快照*>*已读组=nullptr,
    正式读取上下文* 上下文=nullptr) {
    const auto 读取=[&](稳定编码 x) {
        if(!已读组) return 上下文 ? 上下文->读取(x) : 正式快照(x);
        const auto it=已读组->find(x);
        if(it==已读组->end()||!it->second||it->second->结构.节点.编码!=x)
            正式定义故障("formal captured endpoint missing");
        return *it->second;
    };
    auto s=读取(n); 并保护(protect,{s});
    auto shape=正式形状(s.结构); if(!shape) 正式定义故障("not a formal definition");
    正式结构概念信息 info; info.节点=n;
    info.名称关系=正式引用组(s.结构,名称关系字段);
    auto names=info.名称关系; 排序去重(names);
    if(names.size()!=info.名称关系.size()) 正式定义故障("duplicate name relationship");
    for(auto name:names) 并保护(protect,{读取(name)});
    const auto rootCount=std::count_if(s.入向外部关系组.begin(),s.入向外部关系组.end(),[](const auto&r){
        return r.源节点==存在概念图根节点&&r.类型==基础外部关系类型::父子&&r.角色或顺序==组织归属关系角色;});
    if(rootCount!=1) 正式定义故障("formal definition root membership");
    auto record=[&](稳定编码 x,正式定义键序 type){auto t=读取(x);并保护(protect,{t});
        if(正式单引用(t.结构,节点类型字段)!=正式键[type]) 正式定义故障("owned record type");return t.结构;};
    if(*shape==基础类型键) {
        核正式字段(s.结构,{节点类型字段,名称关系字段,正式键[原语键],正式键[整数键],正式键[引用键]});
        基础范围概念定义 d{static_cast<基础概念原语>(正式整数(s.结构,正式键[原语键])),正式整数组(s.结构,正式键[整数键]),正式引用组(s.结构,正式键[引用键])};
        核基础定义(d); info.定义=std::move(d);
    } else if(*shape==约束类型键) {
        核正式字段(s.结构,{节点类型字段,名称关系字段,正式键[原语键],正式键[整数键],正式键[引用键],正式键[项目FT键]});
        结构约束定义 d{static_cast<结构约束原语>(正式整数(s.结构,正式键[原语键])),
            正式引用组(s.结构,正式键[项目FT键]),正式整数组(s.结构,正式键[整数键]),正式引用组(s.结构,正式键[引用键])};
        if(static_cast<unsigned>(d.原语)<1||static_cast<unsigned>(d.原语)>10) 正式定义故障("unknown constraint");
        info.定义=std::move(d);
    } else if(*shape==成员类型键) {
        核正式字段(s.结构,{节点类型字段,名称关系字段,正式键[成员概念键],正式键[最少成员键],正式键[最多成员键],正式键[有序键],正式键[全部键]});
        auto min=正式序号(s.结构,正式键[最少成员键]);auto max=正式整数(s.结构,正式键[最多成员键]);
        auto ordered=正式整数(s.结构,正式键[有序键]),all=正式整数(s.结构,正式键[全部键]);
        if(max< -1||max>UINT32_MAX||(max>=0&&min>max)||(ordered!=0&&ordered!=1)||(all!=0&&all!=1)) 正式定义故障("member organization");
        info.定义=成员组织概念定义{正式单引用(s.结构,正式键[成员概念键]),min,
            max<0?std::nullopt:std::optional<std::uint32_t>{static_cast<std::uint32_t>(max)},ordered!=0,all!=0};
    } else {
        核正式字段(s.结构,{节点类型字段,名称关系字段,正式键[上位键],正式键[项目键],正式键[收窄键],正式键[约束键]});
        信息结构概念定义 d;d.上位结构概念组=正式引用组(s.结构,正式键[上位键]);
        for(auto x:正式引用组(s.结构,正式键[项目键])) {
            auto r=record(x,项目类型键);结构概念内容项 item{正式序号(r,正式键[序号键]),正式单引用(r,正式键[项目FT键]),正式单引用(r,正式键[内容概念键]),{}};
            核正式字段(r,{节点类型字段,正式键[序号键],正式键[项目FT键],正式键[内容概念键],正式键[路径键]});
            std::map<std::uint32_t,稳定编码> path;
            for(auto p:正式引用组(r,正式键[路径键])) {auto pr=record(p,路径类型键);
                核正式字段(pr,{节点类型字段,正式键[序号键],正式键[项目FT键]});
                if(!path.emplace(正式序号(pr,正式键[序号键]),正式单引用(pr,正式键[项目FT键])).second) 正式定义故障("duplicate path position");}
            std::uint32_t pos=1;for(auto [i,v]:path){if(i!=pos++)正式定义故障("path sequence");item.读取路径.push_back(v);}
            d.新增项组.push_back(std::move(item));
        }
        std::sort(d.新增项组.begin(),d.新增项组.end(),[](const auto&a,const auto&b){return a.序号<b.序号;});
        for(std::size_t i=0;i<d.新增项组.size();++i) if(d.新增项组[i].序号!=i+1) 正式定义故障("item sequence");
        for(auto x:正式引用组(s.结构,正式键[收窄键])) {auto r=record(x,收窄类型键);
            核正式字段(r,{节点类型字段,正式键[项目FT键],正式键[内容概念键]});
            d.收窄组.push_back({正式单引用(r,正式键[项目FT键]),正式单引用(r,正式键[内容概念键])});}
        d.约束概念组=正式引用组(s.结构,正式键[约束键]);info.定义=std::move(d);
    }
    if(visited.insert(n).second&&展开引用) {
        auto follow=[&](稳定编码 x){auto t=读取(x);并保护(protect,{t});
            if(正式形状(t.结构)&&!visited.count(x)) (void)解码正式定义(x,protect,visited,true,已读组,上下文);};
        std::visit([&](const auto&d){using D=std::decay_t<decltype(d)>;
            if constexpr(std::is_same_v<D,基础范围概念定义>){for(auto x:d.引用参数)follow(x);}
            else if constexpr(std::is_same_v<D,结构约束定义>){for(auto x:d.项目FT组)follow(x);for(auto x:d.引用参数)follow(x);}
            else if constexpr(std::is_same_v<D,成员组织概念定义>)follow(d.成员概念);
            else {for(auto x:d.上位结构概念组){auto p=读取(x);if(正式形状(p.结构))follow(x);else 核存在根证明(x,protect);}for(const auto&i:d.新增项组){follow(i.特征类型);follow(i.内容概念);for(auto x:i.读取路径)follow(x);}
                for(auto [f,c]:d.收窄组){follow(f);follow(c);}for(auto x:d.约束概念组)follow(x);}
        },info.定义);
    }
    if(上下文) {
        const auto [it,added]=上下文->已完成定义组.emplace(n,info);
        if(!added&&it->second!=info)正式定义故障("formal decoded definition disagrees within call");
    }
    return info;
}
struct 正式结构图有效定义 final {
    std::map<稳定编码,结构概念内容项> 内容项组;
    std::map<稳定编码,正式结构有效约束> 约束组;
};
using 正式结构图展开结果=std::map<稳定编码,正式结构图有效定义>;
正式结构图展开结果 核正式结构图(const std::vector<基础联合节点快照>&snapshots,
    const 正式读取上下文* 上下文=nullptr) {
    std::map<稳定编码,const 基础联合节点快照*> 已读组;
    for(const auto&s:snapshots) {
        if(!有效(s.结构.节点.编码)) 正式定义故障("formal captured endpoint invalid");
        const auto[it,新增]=已读组.emplace(s.结构.节点.编码,&s);
        if(!新增&&!正式快照相同(*it->second,s)) throw 正式状态::前提已变化;
    }
    std::map<稳定编码,正式结构定义> definitions;
    for(const auto&s:snapshots)if(正式形状(s.结构)) {
        if(上下文) {
            const auto old=上下文->已完成定义组.find(s.结构.节点.编码);
            if(old!=上下文->已完成定义组.end()) {
                const auto source=上下文->快照组.find(s.结构.节点.编码);
                if(source==上下文->快照组.end()||old->second.节点!=s.结构.节点.编码
                    ||!正式快照相同(source->second,s))
                    正式定义故障("formal decoded definition snapshot missing or differs");
                definitions.emplace(s.结构.节点.编码,old->second.定义);
                continue;
            }
        }
        std::vector<基础联合节点快照>p;std::set<稳定编码>v;
        definitions.emplace(s.结构.节点.编码,解码正式定义(s.结构.节点.编码,p,v,false,&已读组).定义);
    }
    auto basic=[&](稳定编码 n)->const 基础范围概念定义* {
        auto it=definitions.find(n);return it==definitions.end()?nullptr:std::get_if<基础范围概念定义>(&it->second);};
    std::function<bool(稳定编码,稳定编码,std::set<稳定编码>&)> narrower;
    narrower=[&](稳定编码 child,稳定编码 parent,std::set<稳定编码>&visited) {
        if(child==parent)return true;
        auto a=basic(child),b=basic(parent);
        if(a&&b) {
            if(a->原语==基础概念原语::唯一存在&&b->原语==基础概念原语::合法存在)return true;
            if(a->原语==基础概念原语::I64有限枚举&&b->原语==基础概念原语::I64有限枚举)
                return std::includes(b->整数参数.begin(),b->整数参数.end(),a->整数参数.begin(),a->整数参数.end());
            if(a->原语==基础概念原语::I64闭区间&&b->原语==基础概念原语::I64闭区间)
                return a->整数参数[0]>=b->整数参数[0]&&a->整数参数[1]<=b->整数参数[1];
            return false;
        }
        if(!visited.insert(child).second)return false;
        auto it=definitions.find(child);
        if(it!=definitions.end())if(auto d=std::get_if<信息结构概念定义>(&it->second))
            for(auto p:d->上位结构概念组)if(narrower(p,parent,visited))return true;
        return false;
    };
    auto isNarrow=[&](稳定编码 c,稳定编码 p){std::set<稳定编码>v;return narrower(c,p,v);};
    正式结构图展开结果 effective;
    std::set<稳定编码>active;
    std::function<const 正式结构图有效定义&(稳定编码)> expand=
        [&](稳定编码 n)->const 正式结构图有效定义& {
        if(effective.count(n))return effective.at(n);
        if(!active.insert(n).second)正式定义故障("classification cycle");
        auto it=definitions.find(n);
        if(it==definitions.end()) {
            std::vector<基础联合节点快照> p;核存在根证明(n,p);
            for(const auto&s:p){const auto captured=已读组.find(s.结构.节点.编码);
                if(captured==已读组.end()||!正式快照相同(*captured->second,s))throw 正式状态::前提已变化;}
            active.erase(n);return effective.emplace(n,正式结构图有效定义{}).first->second;
        }
        if(!std::holds_alternative<信息结构概念定义>(it->second))正式定义故障("parent is not structural concept");
        const auto&d=std::get<信息结构概念定义>(it->second);
        正式结构图有效定义 当前;
        auto&merged=当前.内容项组;
        std::set<稳定编码>parents;
        for(auto p:d.上位结构概念组) {
            if(!parents.insert(p).second)正式定义故障("duplicate parent");
            const auto&上位=expand(p);
            for(auto [f,item]:上位.内容项组) {
                auto [pos,inserted]=merged.emplace(f,item);
                if(!inserted&&pos->second!=item) {
                    if(pos->second.读取路径!=item.读取路径)正式定义故障("conflicting inherited read path");
                    if(isNarrow(item.内容概念,pos->second.内容概念))pos->second=item;
                    else if(!isNarrow(pos->second.内容概念,item.内容概念))正式定义故障("conflicting inherited content");
                }
            }
            // 与项目合并共用同一次继承遍历；只继承结构上位的约束。
            for(const auto&[约束节点,约束]:上位.约束组) {
                auto [位置,新增]=当前.约束组.emplace(约束节点,约束);
                if(!新增)位置->second.声明结构概念组.insert(
                    位置->second.声明结构概念组.end(),
                    约束.声明结构概念组.begin(),约束.声明结构概念组.end());
            }
        }
        for(const auto&i:d.新增项组)if(!merged.emplace(i.特征类型,i).second)正式定义故障("redeclared inherited field");
        std::set<稳定编码>narrowed;
        for(auto[f,c]:d.收窄组) {
            auto pos=merged.find(f);
            if(pos==merged.end()||!narrowed.insert(f).second
                ||std::any_of(d.新增项组.begin(),d.新增项组.end(),[&](const auto&i){return i.特征类型==f;})
                ||!isNarrow(c,pos->second.内容概念))正式定义故障("invalid inherited narrowing");
            pos->second.内容概念=c;
        }
        for(auto c:d.约束概念组) {
            auto ci=definitions.find(c);
            if(ci==definitions.end()||!std::holds_alternative<结构约束定义>(ci->second))正式定义故障("constraint identity");
            const auto&r=std::get<结构约束定义>(ci->second);
            if(r.原语!=结构约束原语::禁止项目)for(auto f:r.项目FT组)if(!merged.count(f))正式定义故障("constraint refers undeclared field");
            auto [位置,新增]=当前.约束组.emplace(c,正式结构有效约束{c,r,{}});
            位置->second.声明结构概念组.push_back(n);
        }
        for(auto&[约束节点,约束]:当前.约束组)排序去重(约束.声明结构概念组);
        active.erase(n);
        return effective.emplace(n,std::move(当前)).first->second;
    };
    for(const auto&[n,d]:definitions) {
        if(std::holds_alternative<信息结构概念定义>(d)) (void)expand(n);
        if(auto r=std::get_if<结构约束定义>(&d)) {
            auto count=r->项目FT组.size();bool valid=false;
            switch(r->原语) {
            case 结构约束原语::禁止项目:valid=count>0&&r->整数参数.empty()&&r->引用参数.empty();break;
            case 结构约束原语::域属于FT:case 结构约束原语::数值不大于:case 结构约束原语::同所属父链无环:
                valid=count==2&&r->整数参数.empty()&&r->引用参数.empty();break;
            case 结构约束原语::比较适用于目标:valid=count==3&&r->整数参数.empty()&&r->引用参数.empty();break;
            case 结构约束原语::连续有序:case 结构约束原语::需求父链合法:valid=count==1&&r->整数参数.empty()&&r->引用参数.size()==1;break;
            case 结构约束原语::允许未知当前值:valid=count==1&&r->整数参数.empty()&&r->引用参数.empty();break;
            case 结构约束原语::固定配对基数:valid=count==1&&r->引用参数.empty()&&r->整数参数.size()==2
                &&r->整数参数[0]>=0&&(r->整数参数[1]==-1||r->整数参数[1]>=r->整数参数[0]);break;
            case 结构约束原语::同一对象恰一结构分支: {
                std::set<稳定编码> 判别(r->项目FT组.begin(),r->项目FT组.end());
                std::set<稳定编码> 分支(r->引用参数.begin(),r->引用参数.end());
                valid=count>0&&判别.size()==count&&分支.size()>=2&&分支.size()==r->引用参数.size()&&r->整数参数.empty();
                if(valid)for(const auto 分支节点:分支) {
                    const auto 定义=definitions.find(分支节点);
                    if(定义==definitions.end()||!std::holds_alternative<信息结构概念定义>(定义->second)){valid=false;break;}
                    const auto& 有效分支=expand(分支节点);
                    for(const auto FT:判别)if(!有效分支.内容项组.contains(FT))valid=false;
                }
                break;
            }
            default:break;
            }
            if(!valid)正式定义故障("constraint primitive argument contract");
        }
    }
    return effective;
}
struct 正式结构工作读取结果 final {
    正式状态 状态=正式状态::未找到;
    std::optional<正式结构概念信息> 信息;
    std::vector<基础联合节点快照> 完整保护组;
    正式结构图展开结果 有效定义组;
};
// 单次读取的捕获、核验、跨FT保护及最终联合重核，两个公开投影共用。
正式结构工作读取结果 读取正式结构工作保护(稳定编码 n,
    const 概念_特征类& 特征概念服务) {
    if(!正式schema就绪())return {正式状态::依赖未就绪,{},{},{}};
    if(!有效(n)||!节点仍存在(n))return {};
    正式读取上下文 上下文;
    auto first=上下文.读取(n);if(!正式形状(first.结构))return {};
    std::vector<基础联合节点快照> protect;std::set<稳定编码>visited;
    auto info=解码正式定义(n,protect,visited,true,nullptr,&上下文);
    auto 有效定义组=核正式结构图(protect,&上下文);
    // 普通FT完整保护在锁外读取，不能持本owner锁回调。
    const auto captured=protect;
    for(const auto&s:captured) {
        auto id=s.结构.节点.编码;
        if(正式形状(s.结构))continue;
        if(特征概念服务.是特征概念节点(id)) {
            auto r=特征概念服务.读取正式概念身份保护(id);
            if(r.状态==正式概念读取状态::前提已变化)throw 正式状态::前提已变化;
            if(!r.身份||r.身份->概念节点!=id)正式定义故障("referenced FT protection unavailable");
            并保护(protect,r.身份->完整保护组);
        }
    }
    if(!正式保护未变(protect))return {正式状态::前提已变化,{},{},{}};
    return {正式状态::已读取,std::move(info),std::move(protect),std::move(有效定义组)};
}
} // namespace

普通存在概念定义保护结果 概念_存在类::读取普通存在概念定义保护(稳定编码 n) const noexcept {
    using S=正式概念读取状态;
    try {
        if(!结构已初始化())return {S::依赖未就绪,{},{}};
        if(!有效(n)||!节点仍存在(n))return {};
        const auto first=正式快照(n);
        if(!读取概念种类(n))return {};
        存在概念信息 value;value.节点=n;value.种类=*读取概念种类(n);
        if(value.种类==存在概念种类::专属)value.对应存在节点=正式单引用(first.结构,对应存在字段);
        else if(!正式引用组(first.结构,对应存在字段).empty())正式定义故障("abstract definition has concrete existence");
        value.名称关系=正式引用组(first.结构,名称关系字段);
        std::vector<基础联合节点快照> protect{first};
        for(auto x:正式引用组(first.结构,特征项字段)) {
            auto item=正式快照(x);并保护(protect,{item});
            if(正式单引用(item.结构,节点类型字段)!=特征项节点类型)正式定义故障("ordinary feature item type");
            value.特征组.push_back({正式单引用(item.结构,特征类型概念字段),正式单引用(item.结构,具体特征概念字段)});
        }
        for(const auto&r:first.入向外部关系组){并保护(protect,{正式快照(r.源节点)});
            if(r.类型==基础外部关系类型::父子&&r.角色或顺序==概念上下位关系角色)value.上位概念组.push_back(r.源节点);}
        for(const auto&r:first.出向外部关系组){并保护(protect,{正式快照(r.目标节点)});
            if(r.类型==基础外部关系类型::父子&&r.角色或顺序==概念上下位关系角色)value.下位概念组.push_back(r.目标节点);}
        if(value.种类==存在概念种类::抽象&&value.特征组.empty()&&value.下位概念组.empty())核存在根证明(n,protect);
        {auto r=读取根登记(protect);if(r&&*r==n)核存在根证明(n,protect);}
        if(value.对应存在节点)并保护(protect,{正式快照(*value.对应存在节点)});
        排序特征项(value.特征组);
        for(std::size_t i=1;i<value.特征组.size();++i)if(value.特征组[i-1].特征类型概念==value.特征组[i].特征类型概念)正式定义故障("duplicate ordinary feature type");
        for(const auto&i:value.特征组) for(auto c:{i.特征类型概念,i.具体特征概念}) {
            auto r=特征概念服务_.读取正式概念身份保护(c);
            if(r.状态==S::前提已变化)return {S::前提已变化,{},{}};
            if(!r.身份||r.身份->概念节点!=c)正式定义故障("ordinary FT identity unreadable");
            并保护(protect,r.身份->完整保护组);
        }
        for(const auto&i:value.特征组){auto p=特征概念服务_.读取特征概念定义保护(i.具体特征概念);
            if(p.前提已变化)return {S::前提已变化,{},{}};
            if(!p.信息||p.信息->类型根!=i.特征类型概念)正式定义故障("ordinary concrete range type");并保护(protect,p.预期节点组);}
        for(auto x:value.名称关系)并保护(protect,{正式快照(x)});
        auto uniqueNames=value.名称关系;排序去重(uniqueNames);if(uniqueNames.size()!=value.名称关系.size())正式定义故障("ordinary duplicate names");
        排序去重(value.上位概念组);排序去重(value.下位概念组);value.名称关系=std::move(uniqueNames);
        if(!正式保护未变(protect))return {S::前提已变化,{},{}};
        return {S::已读取,std::move(value),std::move(protect)};
    }catch(正式状态){return {S::前提已变化,{},{}};}
    catch(const std::exception&e){正式定义故障(e.what());}catch(...){正式定义故障("ordinary protection exception");}
}
存在范围语义保护结果 概念_存在类::读取存在范围语义保护(稳定编码 n) const noexcept {
    using S=正式概念读取状态;
    try {
        if(!结构已初始化())return {S::依赖未就绪,{},{}};
        if(!有效(n)||!节点仍存在(n))return {};
        if(!结构仍存在())正式定义故障("semantic existence schema unavailable");
        // 捕获结构只用于解码；最终准入仅核下面明确生成的语义selector。
        std::map<稳定编码,基础联合节点快照> snapshots;
        std::map<稳定编码,基础节点> identities;
        基础只读预期 guard;
        const auto identity=[&](稳定编码 id) {
            if(!有效(id))正式定义故障("semantic existence null endpoint");
            if(identities.contains(id))return;
            const auto node=全局基础数据集.查询节点(id);
            if(!node)throw 正式状态::前提已变化;
            identities.emplace(id,*node);
        };
        const auto capture=[&](稳定编码 id)->const 基础联合节点快照& {
            identity(id);
            if(const auto it=snapshots.find(id);it!=snapshots.end())return it->second;
            const auto read=全局基础数据集.查询节点组完整结构({id});
            if(read.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
            if(read.状态!=基础结构操作状态::已完成||read.节点组.size()!=1)
                正式定义故障("semantic existence capture unavailable");
            if(!read.节点组.front().快照)throw 正式状态::前提已变化;
            return snapshots.emplace(id,*read.节点组.front().快照).first->second;
        };
        const auto fields=[&](稳定编码 id,std::optional<稳定编码> key,
            std::vector<稳定编码> excluded=std::vector<稳定编码>{}) {
            const auto& s=capture(id);
            基础只读字段集合预期 p{id,key,std::move(excluded),{}};
            if(key)identity(*key);
            for(const auto k:p.排除字段键组)identity(k);
            for(const auto& f:s.结构.字段组) {
                if((key&&f.字段节点!=*key)
                    ||std::find(p.排除字段键组.begin(),p.排除字段键组.end(),f.字段节点)!=p.排除字段键组.end())continue;
                identity(f.字段节点);
                if(const auto* ref=std::get_if<稳定编码>(&f.内容))identity(*ref);
                p.字段组.push_back(f);
            }
            std::sort(p.字段组.begin(),p.字段组.end(),[](const auto& a,const auto& b){return a.编码<b.编码;});
            guard.字段集合组.push_back(std::move(p));
        };
        const auto relations=[&](稳定编码 id,bool outgoing,std::int64_t role,
            std::optional<稳定编码> peer=std::nullopt) {
            const auto& s=capture(id);
            基础只读外部关系集合预期 p{id,outgoing,基础外部关系类型::父子,role,peer,{}};
            if(peer)identity(*peer);
            const auto& group = outgoing ? s.出向外部关系组 : s.入向外部关系组;
            for(const auto& r:group)if(r.类型==p.类型&&r.角色或顺序==role
                &&(!peer||(outgoing ? r.目标节点 : r.源节点)==*peer)) {
                identity(r.源节点);identity(r.目标节点);p.关系组.push_back(r);
            }
            std::sort(p.关系组.begin(),p.关系组.end(),[](const auto& a,const auto& b){return a.编码<b.编码;});
            auto result=p.关系组;guard.外部关系集合组.push_back(std::move(p));return result;
        };
        const auto ordinaryType=[&](稳定编码 id) {
            const auto& s=capture(id);
            const auto type=正式单引用(s.结构,节点类型字段);
            if(type!=专属存在概念节点类型&&type!=抽象存在概念节点类型)
                正式定义故障("semantic classification endpoint is not ordinary existence concept");
            fields(id,节点类型字段);return type;
        };
        const auto& first=capture(n);
        const auto tags=正式引用组(first.结构,节点类型字段);
        const bool ordinary=std::any_of(tags.begin(),tags.end(),[](const auto t){
            return t==专属存在概念节点类型||t==抽象存在概念节点类型;});
        if(!ordinary)return {};
        if(tags.size()!=1)正式定义故障("semantic existence type cardinality");
        核正式字段(first.结构,{节点类型字段,对应存在字段,特征项字段,名称关系字段});
        fields(n,std::nullopt,{名称关系字段});
        存在范围语义信息 info{n,tags.front()==专属存在概念节点类型
            ? 存在概念种类::专属 : 存在概念种类::抽象,{},{},{}};
        if(info.种类==存在概念种类::专属) {
            info.对应存在节点=正式单引用(first.结构,对应存在字段);identity(*info.对应存在节点);
        }else if(!正式引用组(first.结构,对应存在字段).empty())
            正式定义故障("semantic abstract concept has concrete existence");
        auto items=正式引用组(first.结构,特征项字段);
        std::set<稳定编码> uniqueItems,uniqueFT;
        for(const auto item:items) {
            if(!uniqueItems.insert(item).second)正式定义故障("duplicate semantic existence feature item");
            const auto& s=capture(item);
            核正式字段(s.结构,{节点类型字段,特征类型概念字段,具体特征概念字段});
            if(正式单引用(s.结构,节点类型字段)!=特征项节点类型)
                正式定义故障("semantic feature item type");
            const auto ft=正式单引用(s.结构,特征类型概念字段);
            const auto fc=正式单引用(s.结构,具体特征概念字段);
            if(!uniqueFT.insert(ft).second)正式定义故障("duplicate semantic existence feature type");
            fields(item,std::nullopt);
            // FT/FC的完整定义保护仍由既有FT消费者承担，不扩成本owner的full读集。
            const auto relation=特征概念服务_.比较单特征概念(fc,fc);
            if(relation.状态==特征概念规则状态::前提已变化)throw 正式状态::前提已变化;
            if(relation.状态!=特征概念规则状态::已完成||relation.特征类型根!=std::optional{ft})
                正式定义故障("semantic feature item range type mismatch");
            info.特征组.push_back({ft,fc});
        }
        排序特征项(info.特征组);
        const auto membership=relations(n,false,组织归属关系角色);
        if(membership.size()!=1||membership.front().源节点!=存在概念图根节点)
            正式定义故障("semantic existence organization membership");
        const auto parents=relations(n,false,概念上下位关系角色);
        std::set<稳定编码> uniqueParents;
        for(const auto& edge:parents) {
            if(edge.源节点==n||!uniqueParents.insert(edge.源节点).second
                ||ordinaryType(edge.源节点)!=抽象存在概念节点类型)
                正式定义故障("semantic existence parent is invalid or duplicated");
            info.上位概念组.push_back(edge.源节点);
        }
        排序去重(info.上位概念组);
        if(info.种类==存在概念种类::抽象&&info.特征组.empty()) {
            const auto& owner=capture(存在概念图根节点);
            const auto root=正式引用组(owner.结构,存在语义根字段);
            if(root.size()>1)正式定义故障("semantic existence root registration cardinality");
            if(root.size()==1&&root.front()==n) {
                fields(存在概念图根节点,存在语义根字段);
                if(!info.上位概念组.empty())正式定义故障("semantic root has classification parent");
            }else {
                // 这里只消费一条确定的合法性见证，不冻结整个无关下位集合。
                std::vector<基础外部关系> witnesses;
                for(const auto& edge:first.出向外部关系组)
                    if(edge.类型==基础外部关系类型::父子&&edge.角色或顺序==概念上下位关系角色)
                        witnesses.push_back(edge);
                std::sort(witnesses.begin(),witnesses.end(),[](const auto& a,const auto& b){return a.编码<b.编码;});
                if(witnesses.empty())正式定义故障("empty semantic abstract concept has no ordinary child witness");
                const auto& witness=witnesses.front();
                if(witness.目标节点==n)正式定义故障("semantic ordinary child self cycle");
                (void)ordinaryType(witness.目标节点);
                const auto forward=relations(n,true,概念上下位关系角色,witness.目标节点);
                const auto reverse=relations(witness.目标节点,false,概念上下位关系角色,n);
                if(forward.size()!=1||reverse!=forward)
                    正式定义故障("semantic ordinary witness duplicate or backlink mismatch");
            }
        }
        for(const auto key:{节点类型字段,专属存在概念节点类型,抽象存在概念节点类型,
            对应存在字段,特征项字段,特征项节点类型,特征类型概念字段,具体特征概念字段,
            名称关系字段,存在概念图根节点})identity(key);
        for(const auto& [id,node]:identities)guard.节点组.push_back(node);
        const auto status=全局基础数据集.核验只读预期(guard);
        if(status==基础节点组追加状态::预期不符||status==基础节点组追加状态::节点不存在
            ||status==基础节点组追加状态::引用不存在)return {S::前提已变化,{},{}};
        if(status!=基础节点组追加状态::已完成)正式定义故障("semantic existence readset validation failed");
        return {S::已读取,std::move(info),std::move(guard)};
    }catch(正式状态 s) {
        if(s==正式状态::前提已变化)return {S::前提已变化,{},{}};
        正式定义故障("semantic existence unexpected control status");
    }catch(const std::exception& e){正式定义故障(e.what());}
    catch(...){正式定义故障("semantic existence read exception");}
}
正式结构概念保护结果 概念_存在类::读取正式结构概念保护(稳定编码 n) const noexcept {
    try {
        auto r=读取正式结构工作保护(n,特征概念服务_);
        return {r.状态,std::move(r.信息),std::move(r.完整保护组)};
    }catch(正式状态 s){return {s,{},{}};}
    catch(const std::exception&e){正式定义故障(e.what());}catch(...){正式定义故障("formal protection exception");}
}
正式结构有效内容读取结果 概念_存在类::读取正式结构有效内容保护(稳定编码 n) const noexcept {
    using 状态=正式结构有效内容读取状态;
    try {
        if(!有效(n))return {};
        auto r=读取正式结构工作保护(n,特征概念服务_);
        switch(r.状态) {
        case 正式状态::未找到:return {状态::未找到,{},{}};
        case 正式状态::依赖未就绪:return {状态::依赖未就绪,{},{}};
        case 正式状态::前提已变化:return {状态::前提已变化,{},{}};
        case 正式状态::已读取:break;
        default:正式定义故障("effective structure read unexpected status");
        }
        if(!r.信息||r.信息->节点!=n||r.完整保护组.empty())
            正式定义故障("effective structure read incomplete protection");
        if(!std::holds_alternative<信息结构概念定义>(r.信息->定义))
            return {状态::非信息结构,{},{}};
        const auto 当前=r.有效定义组.find(n);
        if(当前==r.有效定义组.end())正式定义故障("effective structure definition missing");
        正式结构有效内容 输出{n,{},{}};
        // map顺序仅作确定传输组织，不保留/推定跨父的业务序号。
        for(const auto&[特征类型,项目]:当前->second.内容项组)
            输出.内容项组.push_back({特征类型,项目.内容概念,项目.读取路径});
        for(const auto&[约束节点,约束]:当前->second.约束组)输出.约束组.push_back(约束);
        return {状态::已读取,std::move(输出),std::move(r.完整保护组)};
    }catch(正式状态 s) {
        if(s==正式状态::前提已变化)return {状态::前提已变化,{},{}};
        正式定义故障("effective structure read unexpected control status");
    }catch(const std::bad_alloc&) {return {状态::资源失败,{},{}};}
    catch(const std::exception&e){正式定义故障(e.what());}
    catch(...){正式定义故障("effective structure protection exception");}
}
正式内容符合结果 概念_存在类::比较正式内容范围(稳定编码 C,const 正式内容值& 值,
    const 正式引用身份核验器& 身份核验) const noexcept {
    using S=正式内容符合状态;
    try {
        if(值.状态==正式内容值状态::当前未知)return {S::当前未知,{}};
        if(值.状态==正式内容值状态::不适用)return {S::不适用,{}};
        if(值.状态!=正式内容值状态::已知||!值.值)正式定义故障("known typed content lacks value");
        const bool 数值=值.种类==正式内容值种类::I64数值;
        if(数值!=std::holds_alternative<std::int64_t>(*值.值))正式定义故障("typed content physical mismatch");
        auto 定义=读取正式结构概念保护(C);
        if(定义.状态==正式状态::前提已变化)return {S::前提已变化,{}};
        if(定义.状态!=正式状态::已读取||!定义.信息)return {S::未支持,{}};
        const auto* 基础=std::get_if<基础范围概念定义>(&定义.信息->定义);
        if(!基础)return {S::未支持,{}};
        auto 结果= S::未支持;
        if(基础->原语==基础概念原语::I64闭区间||基础->原语==基础概念原语::I64有限枚举) {
            if(!数值)return {S::不符合,std::move(定义.完整保护组)};
            const auto 实际=std::get<std::int64_t>(*值.值);
            const bool 命中=基础->原语==基础概念原语::I64闭区间
 ? 基础->整数参数[0]<=实际&&实际<=基础->整数参数[1]
                :std::binary_search(基础->整数参数.begin(),基础->整数参数.end(),实际);
            结果=命中 ? S::符合 : S::不符合;
        } else if(基础->原语==基础概念原语::唯一存在) {
            结果=!数值&&值.种类==正式内容值种类::存在引用
                &&std::get<稳定编码>(*值.值)==基础->引用参数.front() ? S::符合 : S::不符合;
        } else if(身份核验) {
            auto 核验=身份核验(基础->原语,基础->整数参数,基础->引用参数,值);
            if(核验.状态==S::资源失败||核验.状态==S::前提已变化)return {核验.状态,{}};
            并保护(定义.完整保护组,核验.完整保护组);结果=核验.状态;
        }
        if(!正式保护未变(定义.完整保护组))return {S::前提已变化,{}};
        return {结果,std::move(定义.完整保护组)};
    }catch(正式状态){return {S::前提已变化,{}};}
    catch(const std::bad_alloc&){return {S::资源失败,{}};}
    catch(const std::exception& 错误){正式定义故障(错误.what());}
    catch(...){正式定义故障("formal content range comparison exception");}
}
正式结构内容判定结果 概念_存在类::判定正式结构内容(稳定编码 C,稳定编码 实例,
    const 正式直接内容读取器& 读取内容,const 正式引用身份核验器& 身份核验,
    const 正式关联约束核验器& 关联核验) const noexcept {
    using S=正式内容符合状态;
    try {
        if(!读取内容||!有效(实例))return {};
        auto 定义=读取正式结构有效内容保护(C);
        if(定义.状态==正式结构有效内容读取状态::前提已变化)return {S::前提已变化,{},{},{}};
        if(定义.状态==正式结构有效内容读取状态::资源失败)return {S::资源失败,{},{},{}};
        if(定义.状态!=正式结构有效内容读取状态::已读取||!定义.信息)return {};
        正式结构内容判定结果 输出{S::符合,{},{},std::move(定义.完整保护组)};
        std::map<std::pair<稳定编码,稳定编码>,正式结构有效内容项> 必需;
        std::map<稳定编码,正式结构有效约束> 约束;
        for(const auto& 项:定义.信息->内容项组)必需.emplace(std::pair{项.特征类型,项.内容概念},项);
        for(const auto& 项:定义.信息->约束组)约束.emplace(项.约束概念节点,项);
        const auto 比较=[&](稳定编码 FT,稳定编码 要求) {
            auto 结果=比较正式内容范围(要求,读取内容(实例,FT),身份核验);
            if(结果.状态==S::前提已变化||结果.状态==S::资源失败)throw 结果.状态;
            并保护(输出.完整保护组,结果.完整保护组);return 结果.状态;
        };
        std::set<稳定编码> 待定判别;
        const auto 初始约束=约束;
        for(const auto& [节点,项]:初始约束)if(项.定义.原语==结构约束原语::同一对象恰一结构分支) {
            std::optional<正式结构有效内容> 所选;
            bool 未知=false;std::size_t 命中=0;
            for(const auto 分支:项.定义.引用参数) {
                auto 内容=读取正式结构有效内容保护(分支);
                if(内容.状态==正式结构有效内容读取状态::前提已变化)throw S::前提已变化;
                if(内容.状态==正式结构有效内容读取状态::资源失败)throw S::资源失败;
                if(!内容.信息)正式定义故障("structural branch definition unavailable");
                并保护(输出.完整保护组,内容.完整保护组);
                bool 全部=true,分支未知=false;
                for(const auto FT:项.定义.项目FT组) {
                    const auto 位置=std::find_if(内容.信息->内容项组.begin(),内容.信息->内容项组.end(),[&](const auto& v){return v.特征类型==FT;});
                    if(位置==内容.信息->内容项组.end()||!位置->读取路径.empty())正式定义故障("branch discriminator missing direct range");
                    const auto 状态=比较(FT,位置->内容概念);
                    if(状态==S::未支持)return {S::未支持,{},{},{}};
                    if(状态==S::当前未知)分支未知=true;
                    else if(状态!=S::符合)全部=false;
                }
                if(全部&&分支未知)未知=true;
                if(全部&&!分支未知){++命中;所选=std::move(内容.信息);}
            }
            if(未知) {
                输出.分支组.push_back({节点,{},S::当前未知});
                待定判别.insert(项.定义.项目FT组.begin(),项.定义.项目FT组.end());
            } else if(命中!=1) {
                输出.分支组.push_back({节点,{},S::不符合});输出.状态=S::不符合;
            } else {
                输出.分支组.push_back({节点,所选->概念节点,S::符合});
                // 共同项与所选分支始终合取，不能用分支的宽范围覆盖共同收窄。
                for(const auto& 内容:所选->内容项组)必需.emplace(std::pair{内容.特征类型,内容.内容概念},内容);
                for(const auto& 限制:所选->约束组)约束.emplace(限制.约束概念节点,限制);
            }
        }
        for(const auto& [键,项]:必需) {
            const auto FT=项.特征类型;
            if(!待定判别.empty()&&!待定判别.contains(FT))continue;
            if(!项.读取路径.empty())return {S::未支持,{},{},{}};
            auto 状态=比较(FT,项.内容概念);
            if(状态==S::不适用)状态=S::不符合;
            输出.项目组.push_back({FT,项.内容概念,状态});
            if(状态==S::未支持)输出.状态=S::未支持;
            else if(状态==S::不符合&&输出.状态!=S::未支持)输出.状态=S::不符合;
            else if(状态==S::当前未知&&输出.状态==S::符合)输出.状态=S::当前未知;
        }
        if(!待定判别.empty()&&输出.状态!=S::未支持)输出.状态=S::当前未知;
        if(待定判别.empty())for(const auto& [节点,项]:约束) {
            if(项.定义.原语==结构约束原语::同一对象恰一结构分支) {
                if(!初始约束.contains(节点))return {S::未支持,{},{},{}};
                continue;
            }
            if(!关联核验)return {S::未支持,{},{},{}};
            auto 核验=关联核验(实例,项.定义);
            if(核验.状态==S::前提已变化||核验.状态==S::资源失败)throw 核验.状态;
            并保护(输出.完整保护组,核验.完整保护组);
            if(核验.状态==S::未支持)输出.状态=S::未支持;
            else if(核验.状态==S::不符合&&输出.状态!=S::未支持)输出.状态=S::不符合;
            else if(核验.状态==S::当前未知&&输出.状态==S::符合)输出.状态=S::当前未知;
        }
        if(!正式保护未变(输出.完整保护组))return {S::前提已变化,{},{},{}};
        return 输出;
    }catch(正式内容符合状态 状态){return {状态,{},{},{}};}
    catch(正式状态){return {S::前提已变化,{},{},{}};}
    catch(const std::bad_alloc&){return {S::资源失败,{},{},{}};}
    catch(const std::exception& 错误){正式定义故障(错误.what());}
    catch(...){正式定义故障("formal structural content evaluation exception");}
}
正式结构概念结果 概念_存在类::读取正式结构概念(稳定编码 n) const noexcept {
    auto r=读取正式结构概念保护(n);return {r.状态,std::move(r.信息)};
}
正式概念身份读取结果 概念_存在类::读取正式概念身份保护(稳定编码 n) const noexcept {
    if(有效(归类未知类型)&&读取唯一节点字段(n,节点类型字段)==归类未知类型){const auto r=读取语言归类概念身份保护(n);if(r.状态==归类概念读取状态::已读取)return {正式概念读取状态::已读取,正式概念身份保护{n,r.完整保护}};if(r.状态==归类概念读取状态::前提已变化)return {正式概念读取状态::前提已变化,{}};if(r.状态==归类概念读取状态::依赖未就绪)return {正式概念读取状态::依赖未就绪,{}};return {}; }

    using S=正式概念读取状态;
    auto normal=读取普通存在概念定义保护(n);
    if(normal.状态==S::已读取)return {S::已读取,正式概念身份保护{n,std::move(normal.完整保护组)}};
    if(normal.状态!=S::非本提供者概念)return {normal.状态,{}};
    auto r=读取正式结构概念保护(n);
    if(r.状态==正式状态::已读取)return {S::已读取,正式概念身份保护{n,std::move(r.完整保护组)}};
    if(r.状态==正式状态::前提已变化)return {S::前提已变化,{}};
    if(r.状态==正式状态::依赖未就绪)return {S::依赖未就绪,{}};
    return {};
}
结构项概念保护结果 概念_存在类::读取结构项概念保护(稳定编码 f,稳定编码 c,
    std::span<const 正式概念身份读取接口* const> readers) const noexcept {
    using S=结构项概念保护状态;
    try {
        if(!有效(f)||!有效(c))return {};
        auto fi=特征概念服务_.获取特征概念(f);
        if(!fi)return {};
        auto loc=特征概念服务_.比较单特征概念(f,f);
        if(loc.状态==特征概念规则状态::前提已变化)return {S::前提已变化,{},{},{}};
        if(!loc.特征类型根||*loc.特征类型根!=f)return {};
        std::vector<基础联合节点快照> protect;
        auto fp=特征概念服务_.读取正式概念身份保护(f);
        if(fp.状态==正式概念读取状态::前提已变化)return {S::前提已变化,{},{},{}};
        if(!fp.身份)return {S::依赖未就绪,{},{},{}};
        并保护(protect,fp.身份->完整保护组);
        bool found=false,missing=false;
        for(auto reader:readers) {
            if(!reader){missing=true;continue;}
            auto r=reader->读取正式概念身份保护(c);
            if(r.状态==正式概念读取状态::前提已变化)return {S::前提已变化,{},{},{}};
            if(r.状态==正式概念读取状态::依赖未就绪){missing=true;continue;}
            if(r.状态!=正式概念读取状态::已读取)continue;
            if(!r.身份||r.身份->概念节点!=c||r.身份->完整保护组.empty())正式定义故障("identity reader returned wrong endpoint");
            并保护(protect,r.身份->完整保护组);found=true;
        }
        if(!found)return {missing ? S::依赖未就绪 : S::未取得正式概念证明,{},{},{}};
        if(!正式保护未变(protect))return {S::前提已变化,{},{},{}};
        return {S::已读取,f,c,std::move(protect)};
    }catch(正式状态){return {S::前提已变化,{},{},{}};}
    catch(const std::exception&e){正式定义故障(e.what());}catch(...){正式定义故障("item identity exception");}
}

普通存在概念定义保护结果 概念_存在类::读取存在根概念保护() const noexcept {
    using S=正式概念读取状态;
    try {
        if(!结构已初始化())return {S::依赖未就绪,{},{}};
        std::vector<基础联合节点快照>p;auto root=读取根登记(p);if(!root)return {};
        核存在根证明(*root,p);auto r=读取普通存在概念定义保护(*root);
        if(r.状态!=S::已读取)return r;并保护(r.完整保护组,p);
        if(!正式保护未变(r.完整保护组))return {S::前提已变化,{},{}};return r;
    }catch(正式状态){return {S::前提已变化,{},{}};}
    catch(const std::exception&e){正式定义故障(e.what());}catch(...){正式定义故障("existence root read exception");}
}
存在概念建立结果 概念_存在类::建立或取得存在根概念(const 存在根建立请求&输入) noexcept {
    using S=存在概念操作状态;
    try {
        if(!结构仍存在())return {};
        std::vector<基础联合节点快照>p;auto root=读取根登记(p);
        if(root&&输入.既有根候选&&*root!=*输入.既有根候选)return {S::结构不一致,{}};
        const auto candidate=root?root:输入.既有根候选;
        if(candidate){
            if(!有效(*candidate)||!节点仍存在(*candidate))return {S::入口拒绝,{}};
            if(!root){
                const auto s=正式快照(*candidate);并保护(p,{s});
                const auto tags=正式引用组(s.结构,节点类型字段);
                if(tags.size()!=1)正式定义故障("candidate has invalid type cardinality");
                if(tags.front()!=抽象存在概念节点类型&&tags.front()!=专属存在概念节点类型)return {S::入口拒绝,{}};
                核普通分类端点(s);核正式字段(s.结构,{节点类型字段,名称关系字段,对应存在字段,特征项字段});
                auto ordinary=读取普通存在概念定义保护(*candidate);
                if(ordinary.状态==正式概念读取状态::前提已变化)return {S::结构不一致,{}};
                if(!ordinary.信息)正式定义故障("ordinary candidate full definition unreadable");
                并保护(p,ordinary.完整保护组);
                if(ordinary.信息->种类!=存在概念种类::抽象||ordinary.信息->对应存在节点
                    ||!ordinary.信息->特征组.empty()||!ordinary.信息->上位概念组.empty())return {S::结构不一致,{}};
            }
            核存在根证明(*candidate,p,!root);
        }
        auto children=输入.必要直接下位普通概念组;排序去重(children);
        for(auto c:children){if(!有效(c)||(candidate&&c==*candidate))return {S::入口拒绝,{}};
            auto r=读取普通存在概念定义保护(c);if(!r.信息)return {S::结构不一致,{}};并保护(p,r.完整保护组);
            if(candidate&&向下可达(c,*candidate))return {S::结构不一致,{}};}
        for(auto key:{节点类型字段,抽象存在概念节点类型,专属存在概念节点类型,对应存在字段,特征项字段,特征项节点类型,特征类型概念字段,具体特征概念字段,名称关系字段})并保护(p,{正式快照(key)});
        基础节点组追加请求 request;request.预期联合节点组=p;
        基础组内字段内容 target;
        if(candidate)target=*candidate;
        else {request.新节点组.push_back({{{节点类型字段,抽象存在概念节点类型}}});target=基础组内节点引用{0};
            request.追加外部关系组.push_back({存在概念图根节点,基础组内节点引用{0},基础外部关系类型::父子,组织归属关系角色});}
        if(!root)request.追加字段组.push_back({存在概念图根节点,存在语义根字段,target});
        for(auto c:children){bool found=false;
            if(candidate)for(const auto&r:查询概念关系(*candidate,false))if(r.目标节点==c){if(found)正式定义故障("duplicate requested root child");found=true;}
            if(!found){std::variant<稳定编码,基础组内节点引用> source=candidate?std::variant<稳定编码,基础组内节点引用>{*candidate}:std::variant<稳定编码,基础组内节点引用>{基础组内节点引用{0}};
                request.追加外部关系组.push_back({source,c,基础外部关系类型::父子,概念上下位关系角色});}}
        const bool changed=!request.新节点组.empty()||!request.追加字段组.empty()||!request.追加外部关系组.empty();
        if(changed){std::lock_guard lock(存在概念图互斥);auto r=全局基础数据集.原子追加节点组(request);
            if(r.状态==基础节点组追加状态::预期不符)return {S::结构不一致,{}};
            if(r.状态!=基础节点组追加状态::已完成)正式定义故障("semantic root CAS failed");}
        auto back=读取存在根概念保护();if(!back.信息)正式定义故障("semantic root publication readback failed");
        for(auto c:children)if(std::find(back.信息->下位概念组.begin(),back.信息->下位概念组.end(),c)==back.信息->下位概念组.end())正式定义故障("semantic root child readback failed");
        return {candidate?(changed?S::已更新:S::已复用):S::已建立,back.信息->节点};
    }catch(正式状态){return {S::结构不一致,{}};}
    catch(const std::exception&e){正式定义故障(e.what());}catch(...){正式定义故障("existence root creation exception");}
}
存在分类关系保护结果 概念_存在类::读取存在分类关系保护(稳定编码 n) const noexcept {
    if(有效(归类未知类型)&&读取唯一节点字段(n,节点类型字段)==归类未知类型){
        const auto r=读取语言归类概念身份保护(n);if(r.状态==归类概念读取状态::前提已变化)return {正式结构操作状态::前提已变化,{},{}};
        if(r.状态!=归类概念读取状态::已读取||!r.信息)return {正式结构操作状态::输入拒绝,{},{}};
        auto p=r.完整保护;存在分类关系信息 info{n,存在分类概念形状::语言归类定义未知,{*r.信息->直接上位},{}};
        try{info.直接下位概念组=归类直接子投影(存在概念图根节点,n,p);if(!归类保护未变(p))return {正式结构操作状态::前提已变化,{},{}};return {正式结构操作状态::已读取,std::move(info),std::move(p)};}catch(...){return {正式结构操作状态::前提已变化,{},{}};}
    }

    try {
        if(!结构已初始化())return {正式状态::依赖未就绪,{},{}};
        if(!有效(n)||!节点仍存在(n))return {正式状态::未找到,{},{}};
        std::vector<基础联合节点快照>p;std::map<稳定编码,存在分类关系信息> cache;std::set<稳定编码>active,done;
        std::function<void(稳定编码)>visit=[&](稳定编码 x){
            if(done.count(x))return;if(!active.insert(x).second)正式定义故障("shared classification parent cycle");
            auto s=正式快照(x);并保护(p,{s});存在分类关系信息 info;info.概念节点=x;
            const auto kind=读取概念种类(x);const auto shape=正式schema就绪()?正式形状(s.结构):std::optional<std::size_t>{};
            if(kind){auto r=读取普通存在概念定义保护(x);
                if(r.状态==正式概念读取状态::前提已变化)throw 正式状态::前提已变化;
                if(!r.信息)throw 正式状态::输入拒绝;并保护(p,r.完整保护组);
                核普通分类端点(s);
                if(*kind==存在概念种类::专属)info.形状=存在分类概念形状::普通专属;
                else info.形状=存在分类概念形状::普通抽象;
                if(正式schema就绪()&&!正式引用组(s.结构,正式键[上位键]).empty())正式定义故障("ordinary concept has structural parents");
                for(const auto&r:s.入向外部关系组)if(r.类型==基础外部关系类型::父子&&r.角色或顺序==概念上下位关系角色)info.直接上位概念组.push_back(r.源节点);
            }else if(shape&&*shape==结构类型键){std::set<稳定编码>v;auto def=解码正式定义(x,p,v);核正式结构图(p);
                info.形状=存在分类概念形状::信息结构;info.直接上位概念组=std::get<信息结构概念定义>(def.定义).上位结构概念组;
                for(const auto&r:s.入向外部关系组)if(r.类型==基础外部关系类型::父子&&r.角色或顺序==概念上下位关系角色)正式定义故障("structural concept has ordinary parents");
            }else throw 正式状态::输入拒绝;
            for(const auto&r:s.出向外部关系组)if(r.类型==基础外部关系类型::父子&&r.角色或顺序==概念上下位关系角色){auto child=正式快照(r.目标节点);核普通分类端点(child);并保护(p,{child});
                if(正式schema就绪()&&!正式引用组(child.结构,正式键[上位键]).empty())
                    正式定义故障("ordinary classification child has structural parents");
                if(std::count(child.入向外部关系组.begin(),child.入向外部关系组.end(),r)!=1)正式定义故障("ordinary classification backlink mismatch");
                auto d=读取普通存在概念定义保护(r.目标节点);if(!d.信息)throw 正式状态::前提已变化;并保护(p,d.完整保护组);info.直接下位概念组.push_back(r.目标节点);}
            if(正式schema就绪())for(const auto&f:s.入向字段组)if(f.字段节点==正式键[上位键]){auto child=正式快照(f.所属节点);并保护(p,{child});
                if(正式形状(child.结构)!=结构类型键||std::count(child.结构.字段组.begin(),child.结构.字段组.end(),f)!=1)正式定义故障("classification backlink type");
                for(const auto&r:child.入向外部关系组)
                    if(r.类型==基础外部关系类型::父子&&r.角色或顺序==概念上下位关系角色)
                        正式定义故障("structural classification child has ordinary parents");
                std::set<稳定编码>v;(void)解码正式定义(f.所属节点,p,v);核正式结构图(p);info.直接下位概念组.push_back(f.所属节点);}
            {const auto added=归类直接子投影(存在概念图根节点,x,p);info.直接下位概念组.insert(info.直接下位概念组.end(),added.begin(),added.end());}
            for(auto*group:{&info.直接上位概念组,&info.直接下位概念组}){auto unique=*group;排序去重(unique);if(unique.size()!=group->size())正式定义故障("duplicate classification edge");*group=std::move(unique);}
            cache.emplace(x,info);for(auto parent:info.直接上位概念组)visit(parent);active.erase(x);done.insert(x);
        };visit(n);if(!正式保护未变(p))return {正式状态::前提已变化,{},{}};
        return {正式状态::已读取,cache.at(n),std::move(p)};
    }catch(正式状态 s){return {s,{},{}};}
    catch(const std::exception&e){正式定义故障(e.what());}catch(归类概念读取状态){return {正式结构操作状态::前提已变化,{},{}};}
    catch(...){正式定义故障("classification read exception");}
}

namespace {
// 编译期槽引用与真实节点引用分账；槽号只在本次构造使用，不伪装成稳定编码。
struct 定义模板 {
    std::uint32_t 槽;
    std::size_t 形状;
    基础范围概念定义 基础{};
    结构约束定义 约束{};
    std::uint32_t 成员槽=0,最少=0;
    bool 有序=false,全部=false;
    std::vector<std::uint32_t> 上位,约束槽;
    std::vector<稳定编码> 普通上位引用组;
    struct 项 {稳定编码 FT;std::uint32_t 内容槽;std::vector<稳定编码> 路径;};
    std::vector<项> 项目;
    std::vector<std::pair<稳定编码,std::uint32_t>> 收窄;
};
using 登记映射=std::map<std::uint32_t,稳定编码>;
正式结构定义 模板定义(const 定义模板&t,const 登记映射&ids) {
    if(t.形状==基础类型键)return t.基础;
    if(t.形状==约束类型键)return t.约束;
    if(t.形状==成员类型键)return 成员组织概念定义{ids.at(t.成员槽),t.最少,{},t.有序,t.全部};
    信息结构概念定义 d;
    for(auto x:t.上位)d.上位结构概念组.push_back(ids.at(x));
    for(auto x:t.普通上位引用组){if(std::find(d.上位结构概念组.begin(),d.上位结构概念组.end(),x)!=d.上位结构概念组.end())正式定义故障("duplicate template parent");d.上位结构概念组.push_back(x);}
    for(const auto&i:t.项目)d.新增项组.push_back({static_cast<std::uint32_t>(d.新增项组.size()+1),i.FT,ids.at(i.内容槽),i.路径});
    for(auto[f,c]:t.收窄)d.收窄组.push_back({f,ids.at(c)});
    for(auto c:t.约束槽)d.约束概念组.push_back(ids.at(c));
    return d;
}
void 编译模板(const 定义模板&t,std::size_t index,基础节点组追加请求&request,
    const 登记映射&old,const std::map<std::uint32_t,std::size_t>&fresh) {
    auto ref=[&](std::uint32_t slot)->基础组内字段内容 {
        auto it=old.find(slot);return it!=old.end()?基础组内字段内容{it->second}:基础组内字段内容{基础组内节点引用{fresh.at(slot)}};
    };
    auto field=[&](std::size_t i,正式定义键序 key,基础组内字段内容 value){request.新节点组[i].字段组.push_back({正式键[key],std::move(value)});};
    auto number=[&](std::size_t i,正式定义键序 key,std::int64_t x){field(i,key,基础值{x});};
    auto record=[&](正式定义键序 type){auto i=request.新节点组.size();request.新节点组.emplace_back();
        request.新节点组[i].字段组.push_back({节点类型字段,正式键[type]});return i;};
    request.新节点组[index].字段组.push_back({节点类型字段,正式键[t.形状]});
    if(t.形状==基础类型键) {
        number(index,原语键,static_cast<std::int64_t>(t.基础.原语));field(index,整数键,基础值{t.基础.整数参数});
        for(auto x:t.基础.引用参数)field(index,引用键,x);
    } else if(t.形状==约束类型键) {
        number(index,原语键,static_cast<std::int64_t>(t.约束.原语));field(index,整数键,基础值{t.约束.整数参数});
        for(auto x:t.约束.项目FT组)field(index,项目FT键,x);for(auto x:t.约束.引用参数)field(index,引用键,x);
    } else if(t.形状==成员类型键) {
        field(index,成员概念键,ref(t.成员槽));number(index,最少成员键,t.最少);number(index,最多成员键,-1);
        number(index,有序键,t.有序);number(index,全部键,t.全部);
    } else {
        for(auto x:t.上位)field(index,上位键,ref(x));
        for(auto x:t.普通上位引用组)field(index,上位键,x);
        for(std::size_t p=0;p<t.项目.size();++p) {
            const auto&i=t.项目[p];auto r=record(项目类型键);field(index,项目键,基础组内节点引用{r});
            number(r,序号键,p+1);field(r,项目FT键,i.FT);field(r,内容概念键,ref(i.内容槽));
            for(std::size_t k=0;k<i.路径.size();++k){auto pr=record(路径类型键);field(r,路径键,基础组内节点引用{pr});
                number(pr,序号键,k+1);field(pr,项目FT键,i.路径[k]);}
        }
        for(auto[f,c]:t.收窄){auto r=record(收窄类型键);field(index,收窄键,基础组内节点引用{r});field(r,项目FT键,f);field(r,内容概念键,ref(c));}
        for(auto x:t.约束槽)field(index,约束键,ref(x));
    }
}
// 调用者先在锁外prepare所有外部身份；此处只读本owner/L1及CAS。
正式状态 准备定义批(const std::vector<定义模板>&templates,
    std::vector<基础联合节点快照> external,基础节点组追加请求&request,
    std::vector<std::pair<std::uint32_t,std::size_t>>&published,登记映射&ids) {
    ids=读取正式登记(external);
    std::map<std::uint32_t,std::size_t> fresh;
    for(const auto&t:templates)if(!ids.count(t.槽)){fresh.emplace(t.槽,request.新节点组.size());request.新节点组.emplace_back();}
    // 已有槽完整同义才复用；名称不进签名，循环定义等全部身份已有后可以直接核。
    for(const auto&t:templates) if(ids.count(t.槽)) {
        std::vector<基础联合节点快照> p;std::set<稳定编码>visited;
        auto info=解码正式定义(ids.at(t.槽),p,visited);
        核正式结构图(p);
        if(std::any_of(templates.begin(),templates.end(),[&](const auto&x){return !ids.count(x.槽);})) {
            // 阶段内循环组必须同批存在；独立基础定义允许逐项已完成。
            if(t.形状!=基础类型键) 正式定义故障("partial recursive definition group");
        }
        if(info.定义!=模板定义(t,ids))return 正式状态::定义冲突;
        并保护(external,p);
    }
    for(const auto&t:templates)if(auto it=fresh.find(t.槽);it!=fresh.end()) {
        编译模板(t,it->second,request,ids,fresh);
        request.追加外部关系组.push_back({存在概念图根节点,基础组内节点引用{it->second},基础外部关系类型::父子,组织归属关系角色});
        auto r=request.新节点组.size();request.新节点组.push_back({{{节点类型字段,正式键[登记类型键]},
            {正式键[槽号键],基础值{static_cast<std::int64_t>(t.槽)}},{正式键[槽目标键],基础组内节点引用{it->second}}}});
        request.追加字段组.push_back({存在概念图根节点,正式键[登记键],基础组内节点引用{r}});
        published.push_back({t.槽,it->second});
    }
    request.预期联合节点组=std::move(external);
    return fresh.empty()?正式状态::已复用:正式状态::已建立;
}
正式状态 迁移任务状态先天枚举() {
    std::lock_guard lock(存在概念图互斥);
    std::vector<基础联合节点快照> protect;
    const auto ids=读取正式登记(protect);
    const auto found=ids.find(416);
    if(found==ids.end())return 正式状态::已复用;
    std::set<稳定编码> visited;
    const auto info=解码正式定义(found->second,protect,visited);
    (void)核正式结构图(protect);
    std::vector<std::int64_t> oldValues,newValues;
    for(std::int64_t x=1;x<=13;++x) {
        newValues.push_back(x);
        if(x<=12)oldValues.push_back(x);
    }
    const auto* definition=std::get_if<基础范围概念定义>(&info.定义);
    if(!definition||definition->原语!=基础概念原语::I64有限枚举
        ||!definition->引用参数.empty())return 正式状态::定义冲突;
    if(definition->整数参数==newValues) {
        return 正式保护未变(protect)?正式状态::已复用:正式状态::前提已变化;
    }
    if(definition->整数参数!=oldValues)return 正式状态::定义冲突;
    稳定编码 field;
    for(const auto& snapshot:protect)if(snapshot.结构.节点.编码==found->second)
        for(const auto& item:snapshot.结构.字段组)if(item.字段节点==正式键[整数键]) {
            if(有效(field))正式定义故障("duplicate task state enumeration field");
            field=item.编码;
        }
    if(!有效(field))正式定义故障("missing task state enumeration field");
    基础节点组追加请求 request;
    request.预期联合节点组=protect;
    request.替换字段组.push_back({field,基础值{newValues}});
    const auto result=全局基础数据集.原子追加节点组(request);
    if(result.状态==基础节点组追加状态::预期不符)return 正式状态::前提已变化;
    if(result.状态!=基础节点组追加状态::已完成)
        正式定义故障("task state enumeration migration publication failed");
    std::vector<基础联合节点快照> readback;
    const auto current=读取正式登记(readback);
    const auto slot=current.find(416);
    if(slot==current.end()||slot->second!=found->second)
        正式定义故障("task state enumeration migration identity changed");
    visited.clear();
    const auto after=解码正式定义(slot->second,readback,visited);
    (void)核正式结构图(readback);
    const 正式结构定义 expected=基础范围概念定义{
        基础概念原语::I64有限枚举,newValues,{}};
    if(after.定义!=expected)正式定义故障("task state enumeration migration readback mismatch");
    if(!正式保护未变(readback))return 正式状态::前提已变化;
    return 正式状态::已建立;
}
std::vector<定义模板> 基础模板() {
    std::vector<定义模板> out;
    auto add=[&](std::uint32_t s,基础概念原语 p,std::vector<std::int64_t> ints={}){
        定义模板 t{};t.槽=s;t.形状=基础类型键;t.基础={p,std::move(ints),{}};核基础定义(t.基础);out.push_back(std::move(t));};
    for(std::uint32_t s=401;s<=406;++s)add(s,static_cast<基础概念原语>(s-400));
    add(407,基础概念原语::I64闭区间,{INT64_MIN,INT64_MAX});add(408,基础概念原语::I64闭区间,{0,INT64_MAX});
    add(409,基础概念原语::I64闭区间,{0,10000});add(410,基础概念原语::I64闭区间,{1,UINT32_MAX});
    for(std::uint32_t s=411;s<=420;++s) {
        std::vector<std::int64_t> values;
        if(s==413)values={1,2,3,4};else if(s==416)for(int x=1;x<=13;++x)values.push_back(x);
        else if(s==420)values={0,1};else values={1,2};
        add(s,基础概念原语::I64有限枚举,std::move(values));
    }
    for(std::uint32_t n=1;n<=13;++n)add(500+n,基础概念原语::具名领域身份,{n});
    // 具名收窄基础定义保留独立槽，不能将不同语义合并成同码。
    for(auto [slot,value]:std::initializer_list<std::pair<std::uint32_t,std::int64_t>>{{810,12},{821,1},{822,2}})
        add(slot,基础概念原语::I64有限枚举,{value});
    return out;
}
} // namespace

正式结构操作状态 概念_存在类::发布先天定义批(const 基础节点组追加请求&request,
    const std::vector<std::pair<std::uint32_t,std::size_t>>&slots) noexcept {
    try {
        if(request.新节点组.empty())return 正式状态::已复用;
        const auto r=全局基础数据集.原子追加节点组(request);
        if(r.状态==基础节点组追加状态::预期不符)return 正式状态::前提已变化;
        if(r.状态!=基础节点组追加状态::已完成||r.新节点组.size()!=request.新节点组.size())正式定义故障("definition batch publication failed");
        for(auto[slot,index]:slots)if(index>=r.新节点组.size()||!有效(r.新节点组[index]))正式定义故障("published batch identity count");
        return 正式状态::已建立;
    }catch(const std::exception&e){正式定义故障(e.what());}catch(...){正式定义故障("definition batch exception");}
}

正式结构概念保护结果 概念_存在类::读取已确认消息连接概念保护() const noexcept {
    try {
        if(!正式schema就绪())return {正式状态::依赖未就绪,{},{}};
        std::vector<基础联合节点快照> protect;
        登记映射 ids;
        {
            std::lock_guard lock(存在概念图互斥);
            ids=读取正式登记(protect);
        }
        const auto found=ids.find(514);
        if(found==ids.end()) {
            if(!正式保护未变(protect))return {正式状态::前提已变化,{},{}};
            return {正式状态::未找到,{},{}};
        }
        for(const auto& snapshot:protect) {
            const auto& fields=snapshot.结构.字段组;
            const bool slot=std::any_of(fields.begin(),fields.end(),[](const auto& field){return field.字段节点==正式键[槽号键];});
            if(slot&&正式序号(snapshot.结构,正式键[槽号键])==514)
                核正式字段(snapshot.结构,{节点类型字段,正式键[槽号键],正式键[槽目标键]});
        }
        // 只证明正式定义身份；连接实例的成立性仍由原连接owner核验。
        auto result=读取正式结构概念保护(found->second);
        if(result.状态==正式状态::前提已变化)return result;
        if(result.状态!=正式状态::已读取||!result.信息||result.信息->节点!=found->second)
            正式定义故障("confirmed message connection slot target unreadable");
        const 正式结构定义 expected=基础范围概念定义{基础概念原语::具名领域身份,{14},{}};
        if(result.信息->定义!=expected)正式定义故障("confirmed message connection slot definition conflict");
        并保护(protect,result.完整保护组);
        if(!正式保护未变(protect))return {正式状态::前提已变化,{},{}};
        result.完整保护组=std::move(protect);
        return result;
    }catch(正式状态 s){return {s,{},{}};}
    catch(const std::exception&e){正式定义故障(e.what());}catch(...){正式定义故障("confirmed message connection protection exception");}
}

正式结构概念结果 概念_存在类::建立或取得已确认消息连接概念() noexcept {
    try {
        if(!初始化正式schema())return {正式状态::依赖未就绪,{}};
        定义模板 definition{};
        definition.槽=514;definition.形状=基础类型键;
        definition.基础={基础概念原语::具名领域身份,{14},{}};
        核基础定义(definition.基础);
        正式状态 status;
        {
            // 判定14无跨owner端点；槽、完整定义及组织关系同批发表。
            std::lock_guard lock(存在概念图互斥);
            基础节点组追加请求 request;
            std::vector<std::pair<std::uint32_t,std::size_t>> slots;
            登记映射 ids;
            status=准备定义批({definition},{},request,slots,ids);
            if(status==正式状态::定义冲突)return {status,{}};
            if(status!=正式状态::已建立&&status!=正式状态::已复用)
                正式定义故障("confirmed message connection prepare unexpected status");
            const auto published=发布先天定义批(request,slots);
            if(published==正式状态::前提已变化)return {published,{}};
            if(published!=正式状态::已建立&&published!=正式状态::已复用)
                正式定义故障("confirmed message connection publish unexpected status");
        }
        const auto read=读取已确认消息连接概念保护();
        if(read.状态!=正式状态::已读取||!read.信息)
            正式定义故障("confirmed message connection published definition readback incomplete; preserve published data");
        return {status,read.信息};
    }catch(正式状态 s){return {s,{}};}
    catch(const std::exception&e){正式定义故障(e.what());}catch(...){正式定义故障("confirmed message connection ensure exception");}
}

namespace {
std::vector<定义模板> 三类定义模板(const 登记映射&f,const 先天信息类型依赖&dep,稳定编码 root={}) {
    using namespace 先天特征类型槽;
    std::vector<定义模板> out;
    auto structure=[&](std::uint32_t s,std::vector<std::uint32_t> parents,
        std::vector<std::pair<std::uint32_t,std::uint32_t>> items,
        std::vector<std::pair<std::uint32_t,std::uint32_t>> narrow={},std::vector<std::uint32_t> constraints={}) {
        定义模板 t{};t.槽=s;t.形状=结构类型键;t.上位=std::move(parents);t.约束槽=std::move(constraints);
        for(auto [ft,content]:items)t.项目.push_back({f.at(ft),content,{}});
        for(auto [ft,content]:narrow)t.收窄.push_back({f.at(ft),content});
        out.push_back(std::move(t));
    };
    structure(1,{},{{需求主体,401},{需求执行域,411},{需求目标存在,401},{需求目标FT,402},{需求目标形式,412},{需求权重,408},{需求活动,413}});
    structure(202,{1},{},{{需求执行域,821}});
    structure(204,{1},{{目标域,403},{目标比较,404}},{{需求目标形式,821}},{801,802});
    structure(205,{1},{{目标方向,414}},{{需求目标形式,822}});
    structure(208,{1},{},{{需求执行域,822}});
    structure(2,{},{{对应需求,202},{任务虚拟存在,301}});
    out.back().项目.push_back({dep.任务状态FT,416,{f.at(任务虚拟存在),dep.任务状态FT}});
    out.back().项目.push_back({dep.任务服务值FT,407,{f.at(任务虚拟存在),dep.任务服务值FT}});
    structure(211,{2},{{采用方法,3}});structure(212,{2},{{执行组织,504}});structure(213,{2},{{执行实例,505}});
    structure(214,{2},{{评估依据,506}});structure(215,{2},{{结算依据,507}});
    out.back().收窄.push_back({dep.任务状态FT,810});
    structure(3,{},{{方法头,303},{方法作用域,417},{方法实现方式,418},{方法行为,419},
        {动作定义,508},{实现绑定,509},{方法虚拟存在,302},{共同约束,701},{条件结果知识,702}});
    structure(221,{3},{{本能登记,510}},{{方法实现方式,821}});
    structure(222,{3},{{执行组织,504}},{{方法实现方式,822}});
    structure(301,{},{});out.back().项目={{dep.任务状态FT,416,{}},{dep.任务服务值FT,407,{}}};
    structure(302,{},{{有序参数规格,706}},{},{804,805,809});
    out.back().项目.insert(out.back().项目.begin(),{dep.方法启用FT,420,{}});
    out.back().项目.push_back({dep.方法运行次数FT,408,{}});out.back().项目.push_back({dep.方法成功次数FT,408,{}});
    out.back().项目.push_back({dep.方法可靠度FT,409,{}});
    structure(303,{},{{作用存在概念,703},{涉及FT概念,704},{可能结果概念,705}});
    structure(304,{},{{参数序号,410},{参数信息类型,406},{参数范围,406}});
    structure(305,{},{{所属方法,3},{条件项,707},{配对结果,708}});
    structure(306,{305},{{父条件,305}},{},{806});
    structure(307,{},{{所属方法,3},{结果项,709}});
    structure(308,{305},{},{},{807});structure(309,{305},{},{},{808});
    const std::pair<std::uint32_t,std::uint32_t> members[]={{701,511},{702,305},{703,406},{704,402},{705,406},{706,304},{707,512},{708,307},{709,513}};
    for(auto[s,c]:members){定义模板 t{};t.槽=s;t.形状=成员类型键;t.成员槽=c;t.最少=s==709?1:0;t.有序=s==706;t.全部=s==707;out.push_back(t);}
    auto constraint=[&](std::uint32_t slot,结构约束原语 p,std::vector<稳定编码> fs,
        std::vector<std::int64_t> ints={},std::vector<稳定编码> refs={}) {
        定义模板 t{};t.槽=slot;t.形状=约束类型键;t.约束={p,std::move(fs),std::move(ints),std::move(refs)};out.push_back(t);};
    constraint(801,结构约束原语::域属于FT,{f.at(目标域),f.at(需求目标FT)});
    constraint(802,结构约束原语::比较适用于目标,{f.at(目标比较),f.at(需求目标FT),f.at(目标域)});
    constraint(803,结构约束原语::禁止项目,{f.at(父需求),f.at(需求来源)});
    constraint(804,结构约束原语::数值不大于,{dep.方法成功次数FT,dep.方法运行次数FT});
    constraint(805,结构约束原语::连续有序,{f.at(有序参数规格)},{},{f.at(参数序号)});
    constraint(806,结构约束原语::同所属父链无环,{f.at(所属方法),f.at(父条件)});
    constraint(807,结构约束原语::固定配对基数,{f.at(配对结果)},{1,1});
    constraint(808,结构约束原语::固定配对基数,{f.at(配对结果)},{2,-1});
    constraint(809,结构约束原语::允许未知当前值,{dep.方法可靠度FT});
    constraint(811,结构约束原语::需求父链合法,{f.at(父需求)},{},{f.at(需求根角色)});
    if(有效(root))for(auto&t:out)if(t.槽==1||t.槽==2||t.槽==3)t.普通上位引用组.push_back(root);
    return out;
}
正式状态 迁移三类存在父(const 登记映射&f,const 先天信息类型依赖&dep,稳定编码 root) {
    std::lock_guard lock(存在概念图互斥);
    std::vector<基础联合节点快照>p;核存在根证明(root,p);auto ids=读取正式登记(p);
    unsigned count=0;for(auto s:{1u,2u,3u})count+=ids.count(s)?1:0;
    if(!count)return 正式状态::已复用;if(count!=3)return 正式状态::定义冲突;
    auto old=三类定义模板(f,dep);auto current=三类定义模板(f,dep,root);
    bool allOld=true,allNew=true;
    for(std::size_t i=0;i<old.size();++i){auto it=ids.find(old[i].槽);if(it==ids.end())return 正式状态::定义冲突;
        std::set<稳定编码>v;auto info=解码正式定义(it->second,p,v);allOld=allOld&&(info.定义==模板定义(old[i],ids));allNew=allNew&&(info.定义==模板定义(current[i],ids));}
    核正式结构图(p);
    if(allNew)return 正式保护未变(p)?正式状态::已复用:正式状态::前提已变化;
    if(!allOld)return 正式状态::定义冲突;
    基础节点组追加请求 request;request.预期联合节点组=std::move(p);
    for(auto s:{1u,2u,3u})request.追加字段组.push_back({ids.at(s),正式键[上位键],root});
    auto r=全局基础数据集.原子追加节点组(request);
    if(r.状态==基础节点组追加状态::预期不符)return 正式状态::前提已变化;
    if(r.状态!=基础节点组追加状态::已完成)正式定义故障("DTM semantic root migration CAS failed");
    std::vector<基础联合节点快照>after;for(auto s:{1u,2u,3u}){std::set<稳定编码>v;auto info=解码正式定义(ids.at(s),after,v);
        if(std::get<信息结构概念定义>(info.定义).上位结构概念组!=std::vector<稳定编码>{root})正式定义故障("DTM semantic parent readback failed");}
    核正式结构图(after);if(!正式保护未变(after))return 正式状态::前提已变化;
    return 正式状态::已建立;
}
std::vector<std::uint32_t> 所需FT槽() {
    using namespace 先天特征类型槽;
    return {需求主体,需求执行域,需求目标存在,需求目标FT,需求目标形式,需求权重,需求活动,
        目标域,目标比较,目标方向,父需求,需求来源,需求根角色,理解来源,内容确认来源,
        对应需求,任务虚拟存在,采用方法,执行组织,执行实例,评估依据,结算依据,
        方法头,方法作用域,方法实现方式,方法行为,动作定义,实现绑定,方法虚拟存在,共同约束,条件结果知识,
        本能登记,作用存在概念,涉及FT概念,可能结果概念,有序参数规格,方法运行次数,方法成功次数,方法可靠度,
        参数序号,参数信息类型,参数范围,所属方法,条件项,父条件,配对结果,结果项};
}
特征概念定义 先天FT定义(std::uint32_t slot,const 登记映射&basic) {
    using namespace 先天特征类型槽;
    特征概念定义 d;
    auto interval=[&](std::int64_t a,std::int64_t b){d.值域=std::vector<特征概念I64闭区间>{{a,b}};};
    if(slot==需求执行域||slot==需求目标形式||slot==目标方向||slot==需求根角色
        ||slot==方法作用域||slot==方法实现方式||slot==方法行为) {interval(1,2);return d;}
    if(slot==需求活动){interval(1,4);return d;}
    if(slot==需求权重||slot==方法运行次数||slot==方法成功次数){interval(0,INT64_MAX);return d;}
    if(slot==方法可靠度){interval(0,10000);return d;}
    if(slot==参数序号){interval(1,UINT32_MAX);return d;}
    const bool isConceptReference = slot==需求目标FT||slot==目标域||slot==目标比较||slot==参数信息类型||slot==参数范围;
    d.材料类型=特征概念材料物理类型::信息节点引用;
    d.比较规则=特征概念比较规则::信息引用身份精确;
    d.值域=特征概念信息引用值域{isConceptReference ? 信息引用语义::概念定义 : 信息引用语义::信息对象,basic.at(isConceptReference ? 406 : 405)};
    return d;
}
先天信息概念组 概念组投影(const 登记映射&ids) {
    先天信息概念组 out{ids.at(1),ids.at(2),ids.at(3),{}};
    for(auto[s,n]:ids)if(s!=1&&s!=2&&s!=3)out.子类与支持定义.push_back({s,n});return out;
}
void 核对模板读回(const std::vector<定义模板>&templates,const 登记映射&ids,
    const 概念_存在类&owner) {
    for(const auto&t:templates) {
        auto r=owner.读取正式结构概念(ids.at(t.槽));
        if(r.状态==正式状态::前提已变化)throw 正式状态::前提已变化;
        if(!r.信息||r.信息->定义!=模板定义(t,ids))正式定义故障("published template differs from complete readback");
    }
}
void 核I64依赖(const 特征概念信息&i,std::int64_t low,std::int64_t high) {
    auto domain=std::get_if<std::vector<特征概念I64闭区间>>(&i.值域);
    if(i.材料类型!=特征概念材料物理类型::I64标量||i.比较规则!=特征概念比较规则::I64数值
        ||i.聚合规则!=特征概念聚合规则::不自动聚合||i.单位||!domain
        ||*domain!=std::vector<特征概念I64闭区间>{{low,high}})正式定义故障("existing business FT definition mismatch");
}
} // namespace

先天信息概念结果 概念_存在类::建立或取得先天需求任务方法概念(const 先天信息类型依赖&dep,
    const 新_特征值类&materials,std::span<const 正式概念身份读取接口* const> readers) noexcept {
    try {
        if(!初始化正式schema())return {正式状态::依赖未就绪,{}};
        // 独立基础定义先行；这一步只依赖本owner schema。
        const auto migration=迁移任务状态先天枚举();
        if(migration!=正式状态::已建立&&migration!=正式状态::已复用)return {migration,{}};
        auto bases=基础模板();登记映射 ids;
        {
            基础节点组追加请求 request;std::vector<std::pair<std::uint32_t,std::size_t>> slots;
            std::lock_guard lock(存在概念图互斥);
            auto s=准备定义批(bases,{},request,slots,ids);
            if(s==正式状态::定义冲突)return {s,{}};
            s=发布先天定义批(request,slots);if(s==正式状态::前提已变化)return {s,{}};
        }
        {std::vector<基础联合节点快照>p;ids=读取正式登记(p);}
        核对模板读回(bases,ids,*this);
        // 现有业务类型与新槽在本owner锁外完整prepare。
        std::vector<基础联合节点快照> protect;登记映射 f;
        for(auto x:{dep.任务状态FT,dep.任务服务值FT,dep.方法启用FT,dep.方法运行次数FT,dep.方法成功次数FT,dep.方法可靠度FT}) {
            if(!有效(x))return {正式状态::依赖未就绪,{}};
            auto p=特征概念服务_.读取特征概念定义保护(x);
            if(p.前提已变化)return {正式状态::前提已变化,{}};
            if(p.状态!=基础结构操作状态::已完成||!p.信息||p.信息->类型根!=x)正式定义故障("business FT root unavailable");
            const auto&value=p.信息->定义;
            if(x==dep.任务状态FT)核I64依赖(value,1,13);
            else if(x==dep.任务服务值FT)核I64依赖(value,INT64_MIN,INT64_MAX);
            else if(x==dep.方法启用FT)核I64依赖(value,0,1);
            else if(x==dep.方法可靠度FT)核I64依赖(value,0,10000);
            else 核I64依赖(value,0,INT64_MAX);
        }
        for(auto slot:所需FT槽()) {
            auto r=特征概念服务_.建立或取得先天特征类型槽({slot,先天FT定义(slot,ids)},materials,readers);
            if(r.状态==特征概念规则状态::前提已变化)return {正式状态::前提已变化,{}};
            if(r.状态==特征概念规则状态::读取接口未装配)return {正式状态::依赖未就绪,{}};
            if(!r.类型根)return {正式状态::定义冲突,{}};
            f.emplace(slot,*r.类型根);
            // 不保留先行建槽时的入向旧快照，统一在全部槽完成后重读。
        }
        for(auto slot:所需FT槽()) {auto r=特征概念服务_.读取先天特征类型槽(slot);
            if(!r.类型根||*r.类型根!=f.at(slot))正式定义故障("FT slot readback mismatch");并保护(protect,r.完整保护组);}
        for(auto x:{dep.任务状态FT,dep.任务服务值FT,dep.方法启用FT,dep.方法运行次数FT,dep.方法成功次数FT,dep.方法可靠度FT}) {
            auto p=特征概念服务_.读取特征概念定义保护(x);
            if(p.前提已变化)return {正式状态::前提已变化,{}};
            if(p.状态!=基础结构操作状态::已完成||!p.信息)正式定义故障("business FT final prepare");
            并保护(protect,p.预期节点组);
        }
        if(f.at(先天特征类型槽::方法运行次数)!=dep.方法运行次数FT
            ||f.at(先天特征类型槽::方法成功次数)!=dep.方法成功次数FT
            ||f.at(先天特征类型槽::方法可靠度)!=dep.方法可靠度FT)return {正式状态::定义冲突,{}};
        // 前面FT增加基础概念入向引用，基础prepare必须在全部FT后重采。
        for(const auto&t:bases){auto p=读取正式结构概念保护(ids.at(t.槽));if(!p.信息)return {p.状态,{}};并保护(protect,p.完整保护组);}
        const auto root=读取存在根概念保护();
        if(!root.信息) {
            if(root.状态==正式概念读取状态::前提已变化)return {正式状态::前提已变化,{}};
            return {正式状态::依赖未就绪,{}};
        }
        const auto semantic=root.信息->节点;
        const auto classification=迁移三类存在父(f,dep,semantic);
        if(classification!=正式状态::已建立&&classification!=正式状态::已复用)return {classification,{}};
        // 迁移增加父字段及入向引用；旧prepare不能跨发布继续使用，重采完整定义闭包。
        protect.clear();
        for(auto slot:所需FT槽()){auto r=特征概念服务_.读取先天特征类型槽(slot);if(!r.类型根)return {正式状态::依赖未就绪,{}};并保护(protect,r.完整保护组);}
        for(auto x:{dep.任务状态FT,dep.任务服务值FT,dep.方法启用FT,dep.方法运行次数FT,dep.方法成功次数FT,dep.方法可靠度FT}){auto r=特征概念服务_.读取特征概念定义保护(x);if(r.前提已变化)return {正式状态::前提已变化,{}};if(!r.信息)正式定义故障("DTM migration FT reprepare failed");并保护(protect,r.预期节点组);}
        for(const auto&t:bases){auto r=读取正式结构概念保护(ids.at(t.槽));if(!r.信息)return {r.状态,{}};并保护(protect,r.完整保护组);}
        核存在根证明(semantic,protect);
        auto templates=三类定义模板(f,dep,semantic);
        正式状态 status;
        {
            std::lock_guard lock(存在概念图互斥);基础节点组追加请求 request;std::vector<std::pair<std::uint32_t,std::size_t>>slots;
            auto s=准备定义批(templates,std::move(protect),request,slots,ids);if(s==正式状态::定义冲突)return {s,{}};
            status=发布先天定义批(request,slots);if(status==正式状态::前提已变化)return {status,{}};
        }
        auto back=读取先天需求任务方法概念();if(!back.信息)正式定义故障("innate batch readback failed");
        {std::vector<基础联合节点快照>p;ids=读取正式登记(p);}
        核对模板读回(templates,ids,*this);
        return {status,std::move(back.信息)};
    }catch(正式状态 s){return {s,{}};}
    catch(const std::exception&e){正式定义故障(e.what());}catch(...){正式定义故障("innate initialization exception");}
}

先天信息概念结果 概念_存在类::读取先天需求任务方法概念() const noexcept {
    try {
        if(!正式schema就绪())return {正式状态::依赖未就绪,{}};
        std::vector<基础联合节点快照>protect;auto ids=读取正式登记(protect);
        if(!ids.count(1)&&!ids.count(2)&&!ids.count(3))return {正式状态::未找到,{}};
        if(!ids.count(1)||!ids.count(2)||!ids.count(3))正式定义故障("partial DTM root registration");
        // 整组共享本次捕获和解码闭包，不对每个登记重复核验同一依赖图。
        正式读取上下文 上下文;std::set<稳定编码>visited;
        for(const auto&[s,n]:ids) {
            if(!visited.count(n))解码正式定义(n,protect,visited,true,nullptr,&上下文);
            const auto info=上下文.已完成定义组.find(n);
            if(info==上下文.已完成定义组.end()||info->second.节点!=n)
                正式定义故障("registered definition unreadable");
        }
        核正式结构图(protect,&上下文);
        // 与单节点读口相同的FT保护规则；共享保护组使重复依赖只读取一次。
        const auto captured=protect;
        for(const auto&s:captured) {
            const auto id=s.结构.节点.编码;
            if(正式形状(s.结构))continue;
            if(特征概念服务_.是特征概念节点(id)) {
                auto r=特征概念服务_.读取正式概念身份保护(id);
                if(r.状态==正式概念读取状态::前提已变化)throw 正式状态::前提已变化;
                if(!r.身份||r.身份->概念节点!=id)正式定义故障("referenced FT protection unavailable");
                并保护(protect,r.身份->完整保护组);
            }
        }
        if(!正式保护未变(protect))return {正式状态::前提已变化,{}};
        return {正式状态::已读取,概念组投影(ids)};
    }catch(正式状态 s){return {s,{}};}
    catch(const std::exception&e){正式定义故障(e.what());}catch(...){正式定义故障("innate group read exception");}
}
先天信息概念结果 概念_存在类::建立或取得自我需求子概念(稳定编码 self,
    const 特征存在成员读取接口&reader,std::span<const 正式概念身份读取接口* const> readers) noexcept {
    try {
        if(!有效(self))return {};
        auto ep=reader.读取存在集合成员身份(self);
        if(ep.状态==特征概念规则状态::读取接口未装配)return {正式状态::依赖未就绪,{}};
        if(ep.状态==特征概念规则状态::前提已变化)return {正式状态::前提已变化,{}};
        if(ep.状态!=特征概念规则状态::已完成||!ep.存在节点||*ep.存在节点!=self||ep.身份保护.empty())
            正式定义故障("successful initialization self identity is not readable");
        auto prior=读取先天需求任务方法概念();if(!prior.信息)return {prior.状态,{}};
        std::vector<基础联合节点快照>protect=ep.身份保护;登记映射 f;
        for(auto slot:所需FT槽()){auto r=特征概念服务_.读取先天特征类型槽(slot);
            if(!r.类型根)return {正式状态::依赖未就绪,{}};f.emplace(slot,*r.类型根);并保护(protect,r.完整保护组);}
        // 栈借用的正式reader只在外部prepare调用，不存入定义或本owner。
        for(auto [slot,n]:prior.信息->子类与支持定义) {
            auto r=读取正式结构概念保护(n);if(!r.信息)return {r.状态,{}};并保护(protect,r.完整保护组);
        }
        (void)readers;
        using namespace 先天特征类型槽;
        std::vector<定义模板>templates;
        定义模板 unique{};unique.槽=601;unique.形状=基础类型键;
        unique.基础={基础概念原语::唯一存在,{}, {self}};templates.push_back(unique);
        auto add=[&](std::uint32_t s,std::vector<std::uint32_t> parents){定义模板 t{};t.槽=s;t.形状=结构类型键;t.上位=std::move(parents);templates.push_back(t);};
        add(201,{1});templates.back().收窄={{f.at(需求主体),601}};
        add(203,{201,202});
        add(206,{203});templates.back().项目={{f.at(父需求),1,{}},{f.at(需求来源),501,{}}};templates.back().约束槽={811};
        add(207,{203});templates.back().项目={{f.at(需求根角色),415,{}}};templates.back().约束槽={803};
        正式状态 status;
        {
            std::lock_guard lock(存在概念图互斥);基础节点组追加请求 request;登记映射 ids;
            std::vector<std::pair<std::uint32_t,std::size_t>>slots;
            auto s=准备定义批(templates,std::move(protect),request,slots,ids);if(s==正式状态::定义冲突)return {s,{}};
            status=发布先天定义批(request,slots);if(status==正式状态::前提已变化)return {status,{}};
        }
        auto back=读取先天需求任务方法概念();if(!back.信息)正式定义故障("self concepts readback failed");
        {std::vector<基础联合节点快照>p;auto ids=读取正式登记(p);核对模板读回(templates,ids,*this);}
        return {status,std::move(back.信息)};
    }catch(正式状态 s){return {s,{}};}
    catch(const std::exception&e){正式定义故障(e.what());}catch(...){正式定义故障("self concepts exception");}
}
引用内容筹办概念结果 概念_存在类::读取引用内容筹办概念() const noexcept {
    try {
        if(!正式schema就绪())return {正式状态::依赖未就绪,{},{}};
        std::vector<基础联合节点快照> 保护;
        const auto ids=读取正式登记(保护);
        const std::uint32_t 槽组[]{209,231,232,812};
        std::size_t 已有=0;for(const auto 槽:槽组)if(ids.contains(槽))++已有;
        const auto 结构已有=static_cast<int>(ids.contains(209))+static_cast<int>(ids.contains(231))+static_cast<int>(ids.contains(232));
        if(!结构已有)return {正式状态::依赖未就绪,{},{}};
        if(结构已有!=3||已有!=4)正式定义故障("partial content planning structure batch");
        for(const auto 槽:槽组) {
            auto 读取=读取正式结构概念保护(ids.at(槽));
            if(读取.状态==正式状态::前提已变化)return {正式状态::前提已变化,{},{}};
            if(!读取.信息)正式定义故障("content planning definition unreadable");
            并保护(保护,读取.完整保护组);
        }
        if(!正式保护未变(保护))return {正式状态::前提已变化,{},{}};
        return {正式状态::已读取,引用内容筹办概念组{ids.at(209),ids.at(231),ids.at(232),ids.at(812)},std::move(保护)};
    }catch(正式状态 s){return {s,{},{}};}
    catch(const std::exception& 错误){正式定义故障(错误.what());}
    catch(...){正式定义故障("content planning definitions read exception");}
}
引用内容筹办概念结果 概念_存在类::建立或取得引用内容筹办概念(
    const 新_特征值类& 材料,std::span<const 正式概念身份读取接口* const> 提供者组) noexcept {
    try {
        bool 有新建 = false;
        if(!正式schema就绪())return {正式状态::依赖未就绪,{},{}};
        auto 先天=读取先天需求任务方法概念();
        if(!先天.信息)return {先天.状态,{},{}};
        const auto 发表批=[&](const std::vector<定义模板>& 模板组) {
            std::vector<基础联合节点快照> 保护;
            const auto 现有=读取正式登记(保护);
            // 只捕获此批需要的既有定义；新批绝不并入旧非基础模板组。
            std::set<稳定编码> 需要;
            for(const auto& 项:模板组) {
                for(const auto 槽:项.上位)if(现有.contains(槽))需要.insert(现有.at(槽));
                for(const auto& 内容:项.项目)if(现有.contains(内容.内容槽))需要.insert(现有.at(内容.内容槽));
                for(const auto& 内容:项.收窄)if(现有.contains(内容.second))需要.insert(现有.at(内容.second));
                for(const auto 槽:项.约束槽)if(现有.contains(槽))需要.insert(现有.at(槽));
                需要.insert(项.约束.引用参数.begin(),项.约束.引用参数.end());
                for(const auto& 内容:项.项目) {
                    auto FT=特征概念服务_.读取正式概念身份保护(内容.FT);
                    if(FT.状态==正式概念读取状态::前提已变化)throw 正式状态::前提已变化;
                    if(!FT.身份)正式定义故障("content planning FT unreadable");
                    并保护(保护,FT.身份->完整保护组);
                }
                for(const auto& 收窄:项.收窄) {
                    auto FT=特征概念服务_.读取正式概念身份保护(收窄.first);
                    if(!FT.身份)正式定义故障("content planning narrowed FT unreadable");
                    并保护(保护,FT.身份->完整保护组);
                }
            }
            for(const auto 节点:需要) {
                auto 读取=读取正式结构概念保护(节点);
                if(读取.状态==正式状态::前提已变化)throw 正式状态::前提已变化;
                if(!读取.信息)正式定义故障("content planning support definition unreadable");
                并保护(保护,读取.完整保护组);
            }
            基础节点组追加请求 请求;登记映射 ids;
            std::vector<std::pair<std::uint32_t,std::size_t>> 槽;
            std::lock_guard 锁(存在概念图互斥);
            const auto 状态=准备定义批(模板组,std::move(保护),请求,槽,ids);
            if(状态==正式状态::定义冲突)throw 状态;
            const auto 发表=发布先天定义批(请求,槽);
            if(发表==正式状态::前提已变化)throw 发表;
            有新建 = 有新建 || 发表 == 正式状态::已建立;
            return ids;
        };
        定义模板 基础{};基础.槽=515;基础.形状=基础类型键;基础.基础={基础概念原语::具名领域身份,{15},{}};
        (void)发表批({基础});
        std::vector<基础联合节点快照> 登记保护;auto ids=读取正式登记(登记保护);
        for(const auto 槽:{先天特征类型槽::引用内容需求,先天特征类型槽::引用内容项}) {
            特征概念定义 定义;
            if(槽==先天特征类型槽::引用内容需求) {
                定义.材料类型=特征概念材料物理类型::需求节点引用;
                定义.比较规则=特征概念比较规则::需求身份精确;
                定义.值域=特征概念全材料值域{};
            } else {
                定义.材料类型=特征概念材料物理类型::信息节点引用;
                定义.比较规则=特征概念比较规则::信息引用身份精确;
                定义.值域=特征概念信息引用值域{信息引用语义::概念定义,ids.at(402)};
            }
            auto 结果=特征概念服务_.建立或取得先天特征类型槽({槽,定义},材料,提供者组);
            if(结果.状态==特征概念规则状态::前提已变化)return {正式状态::前提已变化,{},{}};
            if(!结果.类型根)return {正式状态::依赖未就绪,{},{}};
        }
        const auto 形式=特征概念服务_.读取先天特征类型槽(先天特征类型槽::需求目标形式);
        const auto 域=特征概念服务_.读取先天特征类型槽(先天特征类型槽::目标域);
        const auto D=特征概念服务_.读取先天特征类型槽(先天特征类型槽::引用内容需求);
        const auto 路径=特征概念服务_.读取先天特征类型槽(先天特征类型槽::引用内容项);
        if(!形式.类型根||!域.类型根||!D.类型根||!路径.类型根)return {正式状态::依赖未就绪,{},{}};
        定义模板 约束{};约束.槽=812;约束.形状=约束类型键;
        约束.约束={结构约束原语::同一对象恰一结构分支,{*形式.类型根},{},{ids.at(204),ids.at(205)}};
        (void)发表批({约束});
        定义模板 整体{};整体.槽=231;整体.形状=结构类型键;整体.上位={202};
        整体.收窄={{*形式.类型根,821}};整体.项目={{*域.类型根,406,{}},{*D.类型根,515,{}}};
        定义模板 单项{};单项.槽=232;单项.形状=结构类型键;单项.上位={231};单项.项目={{*路径.类型根,402,{}}};
        定义模板 完整{};完整.槽=209;完整.形状=结构类型键;完整.上位={208};完整.约束槽={812};
        (void)发表批({整体,单项,完整});
        登记保护.clear();ids=读取正式登记(登记保护);
        核对模板读回({基础,约束,整体,单项,完整},ids,*this);
        auto 读回 = 读取引用内容筹办概念();
        if(读回.状态 == 正式状态::已读取 && 读回.信息)
            读回.状态 = 有新建 ? 正式状态::已建立 : 正式状态::已复用;
        return 读回;
    }catch(正式状态 s){return {s,{},{}};}
    catch(const std::exception& 错误){正式定义故障(错误.what());}
    catch(...){正式定义故障("content planning definitions ensure exception");}
}
bool 概念_存在类::绑定信息定义名称关系(稳定编码 n,稳定编码 name) noexcept {
    try {
        auto prepared=读取正式结构概念保护(n);
        if(!prepared.信息||!有效(name)||!节点仍存在(name))return false;
        if(std::find(prepared.信息->名称关系.begin(),prepared.信息->名称关系.end(),name)!=prepared.信息->名称关系.end())return true;
        auto np=正式快照(name);并保护(prepared.完整保护组,{np});
        std::lock_guard lock(存在概念图互斥);
        基础节点组追加请求 request;request.预期联合节点组=std::move(prepared.完整保护组);
        request.追加字段组.push_back({n,名称关系字段,name});
        auto r=全局基础数据集.原子追加节点组(request);
        if(r.状态==基础节点组追加状态::预期不符)return false;
        if(r.状态!=基础节点组追加状态::已完成)正式定义故障("name backlink publication failed");
        return true;
    }catch(正式状态){return false;}
    catch(const std::exception&e){正式定义故障(e.what());}catch(...){正式定义故障("name bind exception");}
}
bool 概念_存在类::解除信息定义名称关系(稳定编码 n,稳定编码 name) noexcept {
    try {
        auto prepared=读取正式结构概念保护(n);if(!prepared.信息)return false;
        auto source=std::find_if(prepared.完整保护组.begin(),prepared.完整保护组.end(),[&](const auto&s){return s.结构.节点.编码==n;});
        if(source==prepared.完整保护组.end())正式定义故障("missing name parent snapshot");
        std::optional<稳定编码>erase;
        for(const auto&f:source->结构.字段组)if(f.字段节点==名称关系字段) {
            auto p=std::get_if<稳定编码>(&f.内容);if(!p)正式定义故障("name backlink value");
            if(*p==name){if(erase)正式定义故障("duplicate name backlink");erase=f.编码;}
        }
        if(!erase)return true;
        std::lock_guard lock(存在概念图互斥);基础节点组追加请求 request;
        request.预期联合节点组=std::move(prepared.完整保护组);request.删除字段关系组.push_back(*erase);
        auto r=全局基础数据集.原子追加节点组(request);
        if(r.状态==基础节点组追加状态::预期不符)return false;
        if(r.状态!=基础节点组追加状态::已完成)正式定义故障("name backlink removal failed");return true;
    }catch(正式状态){return false;}
    catch(const std::exception&e){正式定义故障(e.what());}catch(...){正式定义故障("name unbind exception");}
}
} // namespace 海中鱼巣

namespace 海中鱼巣 {

归类概念身份读取结果 概念_存在类::读取语言归类概念身份保护(稳定编码 n) const noexcept {
    using S=归类概念读取状态;
    try {
        std::lock_guard lock(存在概念图互斥);
        if(!结构已初始化())return {S::依赖未就绪,{},{}};
        if(!有效(n))return {};
        const auto first=归类取快照(n);if(!first)return {};
        const auto kind=归类引用组(first->结构,节点类型字段);
        if(kind.size()!=1)return {};
        if(kind.front()!=归类未知类型){
            const auto root=读取存在根概念保护();
            if(root.状态==正式概念读取状态::前提已变化)return {S::前提已变化,{},{}};
            if(!root.信息)return {S::依赖未就绪,{},{}};
            std::vector<基础联合节点快照> p=root.完整保护组;
            std::set<稳定编码> visited;bool found=false;
            std::function<bool(稳定编码)> walk=[&](稳定编码 x){if(x==root.信息->节点){found=true;return true;}if(!visited.insert(x).second)return true;const auto r=读取存在分类关系保护(x);if(r.状态==正式结构操作状态::前提已变化)return false;if(!r.信息)return false;if(!归类并保护(p,r.完整保护组))return false;for(const auto y:r.信息->直接上位概念组)if(!walk(y))return false;return true;};
            if(!walk(n)||!found)return {};
            auto normal=获取存在概念(n);std::vector<稳定编码> names;
            if(normal)names=normal->名称关系;else {const auto r=读取正式结构概念保护(n);if(!r.信息)return {};names=r.信息->名称关系;if(!归类并保护(p,r.完整保护组))return {S::前提已变化,{},{}};}
            if(!归类并保护(p,{*first})||!归类保护未变(p))return {S::前提已变化,{},{}};
            归类概念身份信息 info;info.节点=n;info.所属根=root.信息->节点;info.名称关系=std::move(names);
            return {S::已读取,std::move(info),std::move(p)};
        }
        static thread_local std::set<稳定编码> active;
        if(!active.insert(n).second)return {S::结构不一致,{},{}};
        struct 退出{std::set<稳定编码>&s;稳定编码 n;~退出(){s.erase(n);}}leave{active,n};
        const auto root=归类取快照(存在概念图根节点);if(!root)return {S::依赖未就绪,{},{}};
        const auto registered=归类引用组(root->结构,归类登记字段);
        if(std::count(registered.begin(),registered.end(),n)!=1)return {S::结构不一致,{},{}};
        归类概念身份信息 info;info.节点=n;info.定义未知=true;
        info.直接上位=归类单引用(first->结构,归类上位字段);
        info.目标词条=归类单引用(first->结构,归类词条字段);
        info.归类需求=归类单引用(first->结构,归类需求字段);
        info.回答消息=归类单引用(first->结构,归类回答字段);
        info.名称关系=归类引用组(first->结构,归类名称字段);
        if(std::set<稳定编码>(info.名称关系.begin(),info.名称关系.end()).size()!=info.名称关系.size())return {S::结构不一致,{},{}};
        const auto parent=读取语言归类概念身份保护(*info.直接上位);
        if(parent.状态!=S::已读取||!parent.信息)return parent;
        info.所属根=parent.信息->所属根;
        auto protect=parent.完整保护;
        if(!归类并保护(protect,{*first,*root}))return {S::前提已变化,{},{}};
        for(const auto id:{*info.目标词条,*info.归类需求,*info.回答消息}){
            const auto p=归类取快照(id);if(!p)return {S::结构不一致,{},{}};
            if(!归类并保护(protect,{*p}))return {S::前提已变化,{},{}};
        }
        for(const auto id:info.名称关系){const auto p=归类取快照(id);if(!p)return {S::结构不一致,{},{}};if(!归类并保护(protect,{*p}))return {S::前提已变化,{},{}};}
        if(!归类保护未变(protect))return {S::前提已变化,{},{}};
        return {S::已读取,std::move(info),std::move(protect)};
    }catch(const std::bad_alloc&){return {S::资源失败,{},{}};}
    catch(...){return {S::结构不一致,{},{}};}
}
归类概念建立结果 概念_存在类::建立或取得语言归类概念(const 归类概念建立请求&q) noexcept {
    using S=归类概念读取状态;
    try{
        std::lock_guard lock(存在概念图互斥);
        if(!结构已初始化()||!有效(归类未知类型))return {S::依赖未就绪,{},{}};
        if(!有效(q.目标词条)||!有效(q.归类需求)||!有效(q.回答消息)||q.来源保护.empty())return {};
        const auto parent=读取语言归类概念身份保护(q.上位概念);
        if(parent.状态!=S::已读取||!parent.信息)return {parent.状态,{},{}};
        auto protect=q.来源保护;if(!归类并保护(protect,parent.完整保护))return {S::前提已变化,{},{}};
        const auto root=归类取快照(存在概念图根节点);if(!root)return {S::依赖未就绪,{},{}};
        if(!归类并保护(protect,{*root}))return {S::前提已变化,{},{}};
        for(const auto n:归类引用组(root->结构,归类登记字段)){
            const auto old=读取语言归类概念身份保护(n);
            if(old.状态!=S::已读取||!old.信息)return {old.状态,{},{}};
            const auto&i=*old.信息;
            if((i.归类需求==q.归类需求&&i.回答消息==q.回答消息)||(i.目标词条==q.目标词条&&i.直接上位==q.上位概念)){
                if(i.目标词条!=q.目标词条||i.直接上位!=q.上位概念)return {S::结构不一致,n,true};
                if(!归类并保护(protect,old.完整保护)||!归类保护未变(protect))return {S::前提已变化,n,true};
                return {S::已读取,n,true};
            }
        }
        if(!归类保护未变(protect))return {S::前提已变化,{},{}};
        基础节点组追加请求 add;add.预期联合节点组=std::move(protect);
        add.新节点组.push_back({{{节点类型字段,归类未知类型},{归类上位字段,q.上位概念},{归类词条字段,q.目标词条},{归类需求字段,q.归类需求},{归类回答字段,q.回答消息}}});
        add.追加字段组.push_back({存在概念图根节点,归类登记字段,基础组内节点引用{0}});
        const auto done=全局基础数据集.原子追加节点组(add);
        if(done.状态==基础节点组追加状态::预期不符)return {S::前提已变化,{},{}};
        if(done.状态==基础节点组追加状态::资源失败)return {S::资源失败,{},{}};
        if(done.状态!=基础节点组追加状态::已完成||done.新节点组.size()!=1)return {S::结构不一致,{},{}};
        const auto C=done.新节点组.front();const auto back=读取语言归类概念身份保护(C);
        return {back.状态,C,false}; // 读回非成功仍保留已发布身份，供原来源重试。
    }catch(const std::bad_alloc&){return {S::资源失败,{},{}};}
    catch(...){return {S::结构不一致,{},{}};}
}
bool 概念_存在类::更新语言归类名称关系(稳定编码 C,稳定编码 name,bool bind,const std::vector<基础联合节点快照>& source) noexcept {
    try{
        std::lock_guard lock(存在概念图互斥);
        const auto info=读取语言归类概念身份保护(C);if(info.状态!=归类概念读取状态::已读取||!info.信息)return false;
        const auto snapshot=归类取快照(C);if(!snapshot||!有效(name))return false;
        const auto key=info.信息->定义未知 ? 归类名称字段 : 名称关系字段;
        const auto fields=归类引用组(snapshot->结构,key);
        if(bind&&std::find(fields.begin(),fields.end(),name)!=fields.end())return true;
        基础节点组追加请求 q;q.预期联合节点组=info.完整保护;
        if(!归类并保护(q.预期联合节点组,source))return false;
        if(bind)q.追加字段组.push_back({C,key,name});
        else for(const auto&f:snapshot->结构.字段组)if(f.字段节点==key&&std::get_if<稳定编码>(&f.内容)&&std::get<稳定编码>(f.内容)==name)q.删除字段关系组.push_back(f.编码);
        return 全局基础数据集.原子追加节点组(q).状态==基础节点组追加状态::已完成;
    }catch(...){return false;}
}

}
