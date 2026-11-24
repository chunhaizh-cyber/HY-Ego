#include "关系概念类.h"
#include "新_语言类.h"

#include <algorithm>
#include <map>
#include <new>
#include <set>
#include <stdexcept>

namespace 海中鱼巣 {
namespace {

稳定编码 节点类型字段{};
稳定编码 关系概念根类型{};
稳定编码 关系概念节点类型{};
稳定编码 关系概念登记根{};
稳定编码 种子键字段{};
稳定编码 连接种类字段{};
稳定编码 边类型字段{};
稳定编码 边角色字段{};
稳定编码 owner字段{};
稳定编码 源角色字段{};
稳定编码 目标角色字段{};
稳定编码 字段键名字段{};
稳定编码 宿主角色字段{};
稳定编码 端点角色键字段{};
稳定编码 端点对象种类字段{};
稳定编码 端点序号字段{};
稳定编码 名称语言字段{};
稳定编码 名称词条字段{};
稳定编码 名称预置字段{};
稳定编码 表达语言字段{};
稳定编码 表达含义操作字段{};
稳定编码 表达预置字段{};
稳定编码 表达知识字段{};
稳定编码 槽序号字段{};
稳定编码 槽端点键字段{};
稳定编码 槽参数角色字段{};
稳定编码 槽取材投影字段{};
稳定编码 表达知识尾词字段{};
稳定编码 附加连接序号字段{};

bool 节点仍存在(稳定编码 n) noexcept {
    return 有效(n) && 全局基础数据集.查询节点(n).has_value();
}

bool 有唯一父(稳定编码 子, 稳定编码 父) {
    const auto 组 = 全局基础数据集.查询目标关系(子, 基础外部关系类型::父子);
    return 组.size() == 1 && 组.front().源节点 == 父 && 组.front().角色或顺序 == 0;
}

bool 类型为(稳定编码 节点, 稳定编码 类型) {
    const auto 组 = 全局基础数据集.查询字段(节点, 节点类型字段);
    if (组.size() != 1) return false;
    const auto* v = std::get_if<稳定编码>(&组.front().内容);
    return v && *v == 类型;
}

std::optional<稳定编码> 读唯一引用(稳定编码 节点, 稳定编码 字段) {
    const auto 组 = 全局基础数据集.查询字段(节点, 字段);
    if (组.size() != 1) return std::nullopt;
    const auto* v = std::get_if<稳定编码>(&组.front().内容);
    return v ? std::optional{*v} : std::nullopt;
}

std::optional<std::int64_t> 读唯一整数(稳定编码 节点, 稳定编码 字段) {
    const auto 组 = 全局基础数据集.查询字段(节点, 字段);
    if (组.size() != 1) return std::nullopt;
    const auto* 值 = std::get_if<基础值>(&组.front().内容);
    if (!值) return std::nullopt;
    const auto* n = std::get_if<std::int64_t>(&值->材料);
    return n ? std::optional{*n} : std::nullopt;
}

std::optional<std::string> 读唯一字符串(稳定编码 节点, 稳定编码 字段) {
    const auto 组 = 全局基础数据集.查询字段(节点, 字段);
    if (组.size() != 1) return std::nullopt;
    const auto* 值 = std::get_if<基础值>(&组.front().内容);
    if (!值) return std::nullopt;
    const auto* s = std::get_if<std::string>(&值->材料);
    return s ? std::optional{*s} : std::nullopt;
}

bool 写引用(稳定编码 节点, 稳定编码 字段, 稳定编码 目标) {
    return 有效(全局基础数据集.添加字段节点(节点, 字段, 目标));
}

bool 写整数(稳定编码 节点, 稳定编码 字段, std::int64_t v) {
    return 有效(全局基础数据集.添加字段值(节点, 字段, 基础值{v}));
}

bool 写字符串(稳定编码 节点, 稳定编码 字段, std::string v) {
    return 有效(全局基础数据集.添加字段值(节点, 字段, 基础值{std::move(v)}));
}

bool 结构句柄就绪() {
    return 有效(节点类型字段) && 有效(关系概念根类型) && 有效(关系概念节点类型)
        && 有效(种子键字段) && 有效(连接种类字段) && 有效(边类型字段)
        && 有效(边角色字段) && 有效(owner字段) && 有效(源角色字段)
        && 有效(目标角色字段) && 有效(字段键名字段) && 有效(宿主角色字段)
        && 有效(端点角色键字段) && 有效(端点对象种类字段) && 有效(端点序号字段)
        && 有效(名称语言字段) && 有效(名称词条字段) && 有效(名称预置字段)
        && 有效(表达语言字段) && 有效(表达含义操作字段) && 有效(表达预置字段)
        && 有效(表达知识字段) && 有效(槽序号字段) && 有效(槽端点键字段)
        && 有效(槽参数角色字段) && 有效(槽取材投影字段)
        && 有效(表达知识尾词字段) && 有效(附加连接序号字段);
}

bool 结构仍存在() {
    return 结构句柄就绪() && 节点仍存在(关系概念登记根)
        && 类型为(关系概念登记根, 关系概念根类型)
        && 有唯一父(关系概念登记根, 自然语言组织根节点);
}

std::optional<关系连接描述> 解码连接字段(稳定编码 节点) {
    const auto 种类 = 读唯一整数(节点, 连接种类字段);
    if (!种类) return std::nullopt;
    if (*种类 == static_cast<std::int64_t>(关系连接种类::外部边)) {
        关系外部边连接 e;
        const auto owner = 读唯一字符串(节点, owner字段);
        const auto 边 = 读唯一整数(节点, 边类型字段);
        const auto 角色 = 读唯一整数(节点, 边角色字段);
        const auto 源 = 读唯一字符串(节点, 源角色字段);
        const auto 目标 = 读唯一字符串(节点, 目标角色字段);
        if (!owner || !边 || !角色 || !源 || !目标) return std::nullopt;
        e.owner标识 = *owner;
        e.边类型 = static_cast<基础外部关系类型>(*边);
        e.角色或顺序 = *角色;
        e.源角色键 = *源;
        e.目标角色键 = *目标;
        return e;
    }
    if (*种类 == static_cast<std::int64_t>(关系连接种类::字段引用)) {
        关系字段引用连接 f;
        const auto owner = 读唯一字符串(节点, owner字段);
        const auto 字段 = 读唯一字符串(节点, 字段键名字段);
        const auto 宿主 = 读唯一字符串(节点, 宿主角色字段);
        const auto 目标 = 读唯一字符串(节点, 目标角色字段);
        if (!owner || !字段 || !宿主 || !目标) return std::nullopt;
        f.owner标识 = *owner;
        f.字段键名 = *字段;
        f.宿主角色键 = *宿主;
        f.目标角色键 = *目标;
        return f;
    }
    return std::nullopt;
}

bool 写连接字段(稳定编码 节点, const 关系连接描述& 连接) {
    if (const auto* 边 = std::get_if<关系外部边连接>(&连接)) {
        return 写整数(节点, 连接种类字段, static_cast<std::int64_t>(关系连接种类::外部边))
            && 写字符串(节点, owner字段, 边->owner标识)
            && 写整数(节点, 边类型字段, static_cast<std::int64_t>(边->边类型))
            && 写整数(节点, 边角色字段, 边->角色或顺序)
            && 写字符串(节点, 源角色字段, 边->源角色键)
            && 写字符串(节点, 目标角色字段, 边->目标角色键);
    }
    const auto& 字段 = std::get<关系字段引用连接>(连接);
    return 写整数(节点, 连接种类字段, static_cast<std::int64_t>(关系连接种类::字段引用))
        && 写字符串(节点, owner字段, 字段.owner标识)
        && 写字符串(节点, 字段键名字段, 字段.字段键名)
        && 写字符串(节点, 宿主角色字段, 字段.宿主角色键)
        && 写字符串(节点, 目标角色字段, 字段.目标角色键);
}

关系概念读取结果 解码节点(稳定编码 节点) {
    using S = 关系概念操作状态;
    if (!有效(节点) || !节点仍存在(节点) || !类型为(节点, 关系概念节点类型)
        || !有唯一父(节点, 关系概念登记根))
        return {S::未找到, {}};
    const auto 键 = 读唯一字符串(节点, 种子键字段);
    if (!键 || 键->empty()) return {S::结构不一致, {}};
    关系概念定义 d;
    d.稳定种子键 = *键;
    auto 主连接 = 解码连接字段(节点);
    if (!主连接) return {S::结构不一致, {}};
    d.连接 = std::move(*主连接);

    std::map<std::int64_t, 关系连接描述> 附加序;
    for (const auto& 边 : 全局基础数据集.查询源关系(节点, 基础外部关系类型::父子)) {
        if (边.角色或顺序 != 4) continue; // 4=附加连接
        const auto 序 = 读唯一整数(边.目标节点, 附加连接序号字段);
        auto 连 = 解码连接字段(边.目标节点);
        if (!序 || !连 || !附加序.emplace(*序, std::move(*连)).second)
            return {S::结构不一致, {}};
    }
    for (const auto& [_, c] : 附加序) d.附加连接组.push_back(c);

    std::map<std::int64_t, 关系端点约束> 端点序;
    for (const auto& 边 : 全局基础数据集.查询源关系(节点, 基础外部关系类型::父子)) {
        if (边.角色或顺序 != 1) continue; // 1=端点子节点
        const auto 子 = 边.目标节点;
        const auto 序 = 读唯一整数(子, 端点序号字段);
        const auto 角色键 = 读唯一字符串(子, 端点角色键字段);
        const auto 种类码 = 读唯一整数(子, 端点对象种类字段);
        if (!序 || !角色键 || !种类码) return {S::结构不一致, {}};
        关系端点约束 c;
        c.角色键 = *角色键;
        c.对象种类 = static_cast<关系端点对象种类>(*种类码);
        if (!端点序.emplace(*序, std::move(c)).second) return {S::结构不一致, {}};
    }
    for (const auto& [_, c] : 端点序) d.端点组.push_back(c);

    for (const auto& 边 : 全局基础数据集.查询源关系(节点, 基础外部关系类型::父子)) {
        if (边.角色或顺序 != 2) continue; // 2=名称子节点
        const auto 子 = 边.目标节点;
        const auto 语言 = 读唯一引用(子, 名称语言字段);
        const auto 词条 = 读唯一引用(子, 名称词条字段);
        const auto 预置 = 读唯一整数(子, 名称预置字段);
        if (!语言 || !词条 || !预置) return {S::结构不一致, {}};
        d.名称组.push_back({*语言, *词条, *预置 != 0});
    }
    for (const auto& 边 : 全局基础数据集.查询源关系(节点, 基础外部关系类型::父子)) {
        if (边.角色或顺序 != 3) continue; // 3=表达关联子节点
        const auto 子 = 边.目标节点;
        const auto 语言 = 读唯一引用(子, 表达语言字段);
        const auto 操作 = 读唯一整数(子, 表达含义操作字段);
        const auto 预置 = 读唯一整数(子, 表达预置字段);
        if (!语言 || !操作 || !预置) return {S::结构不一致, {}};
        关系概念表达关联 a;
        a.语言节点 = *语言;
        a.含义操作码 = *操作;
        a.预置 = *预置 != 0;
        if (const auto 尾 = 读唯一字符串(子, 表达知识尾词字段)) a.预置知识尾词 = *尾;
        for (const auto& k : 全局基础数据集.查询字段(子, 表达知识字段)) {
            const auto* n = std::get_if<稳定编码>(&k.内容);
            if (!n) return {S::结构不一致, {}};
            a.表达知识节点组.push_back(*n);
        }
        std::map<std::int64_t, 关系表达角色槽> 槽序;
        for (const auto& 槽边 : 全局基础数据集.查询源关系(子, 基础外部关系类型::父子)) {
            if (槽边.角色或顺序 != 0) continue;
            const auto 槽节点 = 槽边.目标节点;
            const auto 序 = 读唯一整数(槽节点, 槽序号字段);
            const auto 端点键 = 读唯一字符串(槽节点, 槽端点键字段);
            const auto 参数 = 读唯一整数(槽节点, 槽参数角色字段);
            const auto 投影 = 读唯一整数(槽节点, 槽取材投影字段);
            if (!序 || !端点键 || !参数 || !投影) return {S::结构不一致, {}};
            关系表达角色槽 槽;
            槽.端点角色键 = *端点键;
            槽.参数角色码 = *参数;
            槽.投影 = static_cast<关系表达取材投影>(*投影);
            if (!槽序.emplace(*序, std::move(槽)).second) return {S::结构不一致, {}};
        }
        for (const auto& [_, 槽] : 槽序) a.角色槽组.push_back(槽);
        d.表达关联组.push_back(std::move(a));
    }
    return {S::已读取, 关系概念信息{节点, std::move(d)}};
}

bool 形状兼容(const 关系概念定义& 既有, const 关系概念定义& 请求) {
    if (既有.稳定种子键 != 请求.稳定种子键) return false;
    if (既有.端点组 != 请求.端点组) return false;
    if (既有.连接 != 请求.连接) return false;
    return 既有.附加连接组 == 请求.附加连接组;
}

} // namespace

bool 关系概念类::初始化() noexcept {
    try {
        std::lock_guard 锁(互斥_);
        if (已初始化_ && 结构仍存在()) return true;
        if (!有效(自然语言组织根节点) || !节点仍存在(自然语言组织根节点))
            return false;
        auto 独立 = [](稳定编码& n) {
            if (有效(n)) return 节点仍存在(n);
            n = 全局基础数据集.新建节点();
            return 有效(n);
        };
        if (!独立(节点类型字段) || !独立(关系概念根类型) || !独立(关系概念节点类型)
            || !独立(种子键字段) || !独立(连接种类字段) || !独立(边类型字段)
            || !独立(边角色字段) || !独立(owner字段) || !独立(源角色字段)
            || !独立(目标角色字段) || !独立(字段键名字段) || !独立(宿主角色字段)
            || !独立(端点角色键字段) || !独立(端点对象种类字段) || !独立(端点序号字段)
            || !独立(名称语言字段) || !独立(名称词条字段) || !独立(名称预置字段)
            || !独立(表达语言字段) || !独立(表达含义操作字段) || !独立(表达预置字段)
            || !独立(表达知识字段) || !独立(槽序号字段) || !独立(槽端点键字段)
            || !独立(槽参数角色字段) || !独立(槽取材投影字段)
            || !独立(表达知识尾词字段) || !独立(附加连接序号字段))
            return false;
        if (!有效(关系概念登记根)) {
            关系概念登记根 = 全局基础数据集.新建节点(自然语言组织根节点);
            if (!有效(关系概念登记根)) return false;
        } else if (!节点仍存在(关系概念登记根)
            || !有唯一父(关系概念登记根, 自然语言组织根节点)) {
            return false;
        }
        const auto 类型组 = 全局基础数据集.查询字段(关系概念登记根, 节点类型字段);
        if (类型组.empty()) {
            if (!写引用(关系概念登记根, 节点类型字段, 关系概念根类型)) return false;
        } else if (类型组.size() != 1 || !类型为(关系概念登记根, 关系概念根类型)) {
            return false;
        }
        if (!结构仍存在()) return false;
        已初始化_ = true;
        return true;
    } catch (...) {
        return false;
    }
}

bool 关系概念类::已装配() const noexcept {
    std::lock_guard 锁(互斥_);
    return 已初始化_ && 结构仍存在();
}

关系概念建立结果 关系概念类::建立或取得关系概念(const 关系概念定义& 请求) noexcept {
    using S = 关系概念操作状态;
    try {
        std::lock_guard 锁(互斥_);
        if (!已初始化_ || !结构仍存在()) return {S::依赖未就绪, {}};
        if (请求.稳定种子键.empty() || 请求.端点组.size() < 2) return {S::输入不合法, {}};
        std::set<std::string> 角色;
        for (const auto& e : 请求.端点组) {
            if (e.角色键.empty() || !角色.insert(e.角色键).second) return {S::输入不合法, {}};
        }
        const auto 连接角色合法 = [&](const 关系连接描述& 连) -> bool {
            if (const auto* 边 = std::get_if<关系外部边连接>(&连)) {
                return !边->owner标识.empty() && !边->源角色键.empty()
                    && !边->目标角色键.empty() && 角色.contains(边->源角色键)
                    && 角色.contains(边->目标角色键);
            }
            if (const auto* 字段 = std::get_if<关系字段引用连接>(&连)) {
                return !字段->owner标识.empty() && !字段->字段键名.empty()
                    && !字段->宿主角色键.empty() && !字段->目标角色键.empty()
                    && 角色.contains(字段->宿主角色键)
                    && 角色.contains(字段->目标角色键);
            }
            return false;
        };
        if (!连接角色合法(请求.连接)) return {S::输入不合法, {}};
        for (const auto& 附加 : 请求.附加连接组)
            if (!连接角色合法(附加)) return {S::输入不合法, {}};

        for (const auto& 边 : 全局基础数据集.查询源关系(
            关系概念登记根, 基础外部关系类型::父子)) {
            if (边.角色或顺序 != 0) continue;
            auto 既有 = 解码节点(边.目标节点);
            if (既有.状态 == S::未找到) continue;
            if (既有.状态 != S::已读取 || !既有.信息) return {S::结构不一致, {}};
            if (既有.信息->定义.稳定种子键 != 请求.稳定种子键) continue;
            if (!形状兼容(既有.信息->定义, 请求)) return {S::定义冲突, {}};
            // 复用：保留既有名称与表达关联（含后天知识），不覆盖。
            // 幂等补槽：仅当某语言表达已有但角色槽为空、且请求提供≥2槽时写入子节点；
            // 不改含义操作/预置/后天知识，不删除已有槽。
            for (const auto& 表 : 请求.表达关联组) {
                if (!有效(表.语言节点) || 表.角色槽组.size() < 2) continue;
                for (const auto& 表达边 : 全局基础数据集.查询源关系(
                    边.目标节点, 基础外部关系类型::父子)) {
                    if (表达边.角色或顺序 != 3) continue;
                    const auto 语言 = 读唯一引用(表达边.目标节点, 表达语言字段);
                    if (!语言 || *语言 != 表.语言节点) continue;
                    bool 已有槽 = false;
                    for (const auto& 槽边 : 全局基础数据集.查询源关系(
                        表达边.目标节点, 基础外部关系类型::父子)) {
                        if (槽边.角色或顺序 == 0) {
                            已有槽 = true;
                            break;
                        }
                    }
                    if (已有槽) break;
                    for (std::size_t i = 0; i < 表.角色槽组.size(); ++i) {
                        const auto& 槽 = 表.角色槽组[i];
                        if (槽.端点角色键.empty() || 槽.参数角色码 == 0)
                            return {S::输入不合法, {}};
                        const auto 槽节点 = 全局基础数据集.新建节点();
                        if (!有效(槽节点)
                            || !有效(全局基础数据集.添加关系(
                                表达边.目标节点, 槽节点, 基础外部关系类型::父子, 0))
                            || !写整数(槽节点, 槽序号字段, static_cast<std::int64_t>(i))
                            || !写字符串(槽节点, 槽端点键字段, 槽.端点角色键)
                            || !写整数(槽节点, 槽参数角色字段, 槽.参数角色码)
                            || !写整数(槽节点, 槽取材投影字段,
                                static_cast<std::int64_t>(槽.投影)))
                            return {S::资源失败, {}};
                    }
                    break;
                }
            }
            auto 读回 = 解码节点(边.目标节点);
            if (读回.状态 != S::已读取 || !读回.信息) return {S::结构不一致, {}};
            return {S::已复用, std::move(读回.信息)};
        }

        const auto 节点 = 全局基础数据集.新建节点(关系概念登记根);
        if (!有效(节点)) return {S::资源失败, {}};
        if (!写引用(节点, 节点类型字段, 关系概念节点类型)
            || !写字符串(节点, 种子键字段, 请求.稳定种子键)
            || !写连接字段(节点, 请求.连接))
            return {S::资源失败, {}};
        for (std::size_t i = 0; i < 请求.附加连接组.size(); ++i) {
            const auto 子 = 全局基础数据集.新建节点();
            if (!有效(子)
                || !有效(全局基础数据集.添加关系(节点, 子, 基础外部关系类型::父子, 4))
                || !写整数(子, 附加连接序号字段, static_cast<std::int64_t>(i))
                || !写连接字段(子, 请求.附加连接组[i]))
                return {S::资源失败, {}};
        }
        for (std::size_t i = 0; i < 请求.端点组.size(); ++i) {
            const auto 子 = 全局基础数据集.新建节点();
            if (!有效(子)
                || !有效(全局基础数据集.添加关系(节点, 子, 基础外部关系类型::父子, 1))
                || !写整数(子, 端点序号字段, static_cast<std::int64_t>(i))
                || !写字符串(子, 端点角色键字段, 请求.端点组[i].角色键)
                || !写整数(子, 端点对象种类字段,
                    static_cast<std::int64_t>(请求.端点组[i].对象种类)))
                return {S::资源失败, {}};
        }
        for (const auto& 名 : 请求.名称组) {
            if (!有效(名.语言节点) || !有效(名.词条节点)) return {S::输入不合法, {}};
            const auto 子 = 全局基础数据集.新建节点();
            if (!有效(子)
                || !有效(全局基础数据集.添加关系(节点, 子, 基础外部关系类型::父子, 2))
                || !写引用(子, 名称语言字段, 名.语言节点)
                || !写引用(子, 名称词条字段, 名.词条节点)
                || !写整数(子, 名称预置字段, 名.预置 ? 1 : 0))
                return {S::资源失败, {}};
        }
        for (const auto& 表 : 请求.表达关联组) {
            if (!有效(表.语言节点) || 表.角色槽组.size() < 2) return {S::输入不合法, {}};
            const auto 子 = 全局基础数据集.新建节点();
            if (!有效(子)
                || !有效(全局基础数据集.添加关系(节点, 子, 基础外部关系类型::父子, 3))
                || !写引用(子, 表达语言字段, 表.语言节点)
                || !写整数(子, 表达含义操作字段, 表.含义操作码)
                || !写整数(子, 表达预置字段, 表.预置 ? 1 : 0)
                || (!表.预置知识尾词.empty()
                    && !写字符串(子, 表达知识尾词字段, 表.预置知识尾词)))
                return {S::资源失败, {}};
            for (const auto k : 表.表达知识节点组) {
                if (!有效(k) || !写引用(子, 表达知识字段, k)) return {S::资源失败, {}};
            }
            for (std::size_t i = 0; i < 表.角色槽组.size(); ++i) {
                const auto& 槽 = 表.角色槽组[i];
                if (槽.端点角色键.empty() || 槽.参数角色码 == 0) return {S::输入不合法, {}};
                const auto 槽节点 = 全局基础数据集.新建节点();
                if (!有效(槽节点)
                    || !有效(全局基础数据集.添加关系(子, 槽节点, 基础外部关系类型::父子, 0))
                    || !写整数(槽节点, 槽序号字段, static_cast<std::int64_t>(i))
                    || !写字符串(槽节点, 槽端点键字段, 槽.端点角色键)
                    || !写整数(槽节点, 槽参数角色字段, 槽.参数角色码)
                    || !写整数(槽节点, 槽取材投影字段,
                        static_cast<std::int64_t>(槽.投影)))
                    return {S::资源失败, {}};
            }
        }
        auto 读回 = 解码节点(节点);
        if (读回.状态 != S::已读取 || !读回.信息) return {S::结构不一致, {}};
        return {S::已建立, std::move(读回.信息)};
    } catch (const std::bad_alloc&) {
        return {S::资源失败, {}};
    } catch (...) {
        return {S::资源失败, {}};
    }
}

关系概念读取结果 关系概念类::按种子键读取(const std::string& 键) const noexcept {
    using S = 关系概念操作状态;
    try {
        std::lock_guard 锁(互斥_);
        if (!已初始化_ || !结构仍存在()) return {S::依赖未就绪, {}};
        if (键.empty()) return {S::输入不合法, {}};
        for (const auto& 边 : 全局基础数据集.查询源关系(
            关系概念登记根, 基础外部关系类型::父子)) {
            if (边.角色或顺序 != 0) continue;
            auto r = 解码节点(边.目标节点);
            if (r.状态 == S::未找到) continue;
            if (r.状态 != S::已读取 || !r.信息) return {S::结构不一致, {}};
            if (r.信息->定义.稳定种子键 == 键) return r;
        }
        return {S::未找到, {}};
    } catch (...) {
        return {S::资源失败, {}};
    }
}

关系概念读取结果 关系概念类::按节点读取(稳定编码 节点) const noexcept {
    using S = 关系概念操作状态;
    try {
        std::lock_guard 锁(互斥_);
        if (!已初始化_ || !结构仍存在()) return {S::依赖未就绪, {}};
        return 解码节点(节点);
    } catch (...) {
        return {S::资源失败, {}};
    }
}

关系概念列表结果 关系概念类::列出全部() const noexcept {
    using S = 关系概念操作状态;
    try {
        std::lock_guard 锁(互斥_);
        if (!已初始化_ || !结构仍存在()) return {S::依赖未就绪, {}};
        关系概念列表结果 out;
        out.状态 = S::已读取;
        for (const auto& 边 : 全局基础数据集.查询源关系(
            关系概念登记根, 基础外部关系类型::父子)) {
            if (边.角色或顺序 != 0) continue;
            auto r = 解码节点(边.目标节点);
            if (r.状态 != S::已读取 || !r.信息) return {S::结构不一致, {}};
            out.信息组.push_back(std::move(*r.信息));
        }
        return out;
    } catch (...) {
        return {S::资源失败, {}};
    }
}

关系概念操作状态 关系概念类::追加表达知识(
    稳定编码 关系概念节点, 稳定编码 语言节点, 稳定编码 知识节点) noexcept {
    using S = 关系概念操作状态;
    try {
        std::lock_guard 锁(互斥_);
        if (!已初始化_ || !结构仍存在()) return S::依赖未就绪;
        if (!有效(关系概念节点) || !有效(语言节点) || !有效(知识节点)) return S::输入不合法;
        auto 读 = 解码节点(关系概念节点);
        if (读.状态 != S::已读取 || !读.信息) return 读.状态;
        for (const auto& 边 : 全局基础数据集.查询源关系(
            关系概念节点, 基础外部关系类型::父子)) {
            if (边.角色或顺序 != 3) continue;
            const auto 语言 = 读唯一引用(边.目标节点, 表达语言字段);
            if (!语言 || *语言 != 语言节点) continue;
            for (const auto& k : 全局基础数据集.查询字段(边.目标节点, 表达知识字段)) {
                const auto* n = std::get_if<稳定编码>(&k.内容);
                if (n && *n == 知识节点) return S::已复用;
            }
            return 写引用(边.目标节点, 表达知识字段, 知识节点) ? S::已建立 : S::资源失败;
        }
        // 后天只向已有目标语言表达关联追加知识节点；不新建含义操作码=0 的空关联，
        // 以免读回时抢先匹配导致成文路径误判缺语言知识。
        return S::未找到;
    } catch (...) {
        return S::资源失败;
    }
}

} // namespace 海中鱼巣
