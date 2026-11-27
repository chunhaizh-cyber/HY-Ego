#include "新_自然语言世界树类.h"
#include "世界挂接.全局根.h"
#include "../核心/容错检查.h"

#include "概念_存在类.h"
#include "概念_二次特征类.h"
#include "概念_特征类.h"
#include "概念_名称关系类.h"
#include "新_词类.h"
#include "新_语言类.h"
#include "新_特征值类.h"

#include <algorithm>
#include <functional>
#include <limits>
#include <mutex>
#include <map>
#include <set>
#include <thread>
#include <stdexcept>
#include <unordered_set>

namespace 海中鱼巣 {

稳定编码 自然语言世界树根节点{};

namespace {

std::recursive_mutex 自然语言世界树互斥;
稳定编码 自然语言概念来源字段{};
struct 语言复制前提变化 final {};

bool 节点仍存在(稳定编码 节点) noexcept {
    return 有效(节点) && 全局基础数据集.查询节点(节点).has_value();
}

std::optional<稳定编码> 读取唯一节点字段(
    稳定编码 节点, 稳定编码 字段) {
    const auto 字段组 = 全局基础数据集.查询字段(节点, 字段);
    if (字段组.size() != 1) return std::nullopt;
    const auto* 目标 = std::get_if<稳定编码>(&字段组.front().内容);
    return 目标 && 节点仍存在(*目标)
        ? std::optional<稳定编码>{*目标} : std::nullopt;
}

bool 区间特征与概念一致(
    稳定编码 特征节点,
    稳定编码 概念节点,
    const 新_特征类& 特征服务,
    const 概念_特征类& 特征概念服务) {
    const auto 概念 = 特征概念服务.获取特征概念(概念节点);
    const auto 特征 = 特征服务.获取特征(特征节点);
    return 概念 && 特征
        && 特征->特征概念节点 == 概念节点
        && 特征->实例区间值 && *特征->实例区间值 == 概念->值域
        && !特征->实例历史聚合值域
        && !特征->当前值 && !特征->标准值
        && 特征->历史不同值.empty()
        && 特征->历史值命中信息.empty();
}

bool 实例投影与来源概念一致(
    const 新存在信息& 存在,
    稳定编码 来源概念,
    新自然语言概念实例种类 种类,
    const 新_特征类& 特征服务,
    const 概念_特征类& 特征概念服务,
    const 概念_存在类& 存在概念服务) {
    if (种类 == 新自然语言概念实例种类::特征概念) {
        return 存在.特征节点组.size() == 1
            && 区间特征与概念一致(
                存在.特征节点组.front(), 来源概念,
                特征服务, 特征概念服务);
    }

    const auto 概念 = 存在概念服务.获取存在概念(来源概念);
    if (!概念 || 存在.特征节点组.size() != 概念->特征组.size()) {
        return false;
    }
    std::vector<bool> 已匹配(概念->特征组.size(), false);
    for (const auto 特征节点 : 存在.特征节点组) {
        const auto 特征 = 特征服务.获取特征(特征节点);
        if (!特征) return false;
        bool 找到 = false;
        for (std::size_t i = 0; i < 概念->特征组.size(); ++i) {
            if (已匹配[i]
                || 概念->特征组[i].具体特征概念
                    != 特征->特征概念节点) {
                continue;
            }
            if (!区间特征与概念一致(
                    特征节点, 特征->特征概念节点,
                    特征服务, 特征概念服务)) {
                return false;
            }
            已匹配[i] = true;
            找到 = true;
            break;
        }
        if (!找到) return false;
    }
    return true;
}

std::optional<新自然语言概念实例信息> 读取概念实例(
    稳定编码 节点,
    const 新_场景类& 场景服务,
    const 新_存在类& 存在服务,
    const 新_特征类& 特征服务,
    const 概念_特征类& 特征概念服务,
    const 概念_存在类& 存在概念服务) {
    const auto 所属场景 = 场景服务.查询所属场景(节点);
    const auto 存在 = 存在服务.获取存在(节点);
    const auto 来源概念 = 读取唯一节点字段(
        节点, 自然语言概念来源字段);
    if (!所属场景 || !存在 || !来源概念
        || !存在->子存在节点组.empty()) {
        return std::nullopt;
    }

    const bool 是特征概念 = 特征概念服务.是特征概念节点(*来源概念);
    const bool 是存在概念 = 存在概念服务.是存在概念节点(*来源概念);
    if (是特征概念 == 是存在概念) return std::nullopt;
    const auto 种类 = 是特征概念
        ? 新自然语言概念实例种类::特征概念
        : 新自然语言概念实例种类::存在概念;
    if (!实例投影与来源概念一致(
            *存在, *来源概念, 种类,
            特征服务, 特征概念服务, 存在概念服务)) {
        return std::nullopt;
    }
    return 新自然语言概念实例信息{
        节点, *所属场景, *来源概念, 种类};
}

std::optional<std::vector<新自然语言概念实例信息>> 读取场景全部概念实例(
    稳定编码 场景节点,
    const 新_场景类& 场景服务,
    const 新_存在类& 存在服务,
    const 新_特征类& 特征服务,
    const 概念_特征类& 特征概念服务,
    const 概念_存在类& 存在概念服务) {
    if (!场景服务.是场景节点(场景节点)) return std::nullopt;
    std::vector<新自然语言概念实例信息> 结果;
    for (const auto 节点 : 场景服务.查询直接成员存在(场景节点)) {
        const auto 实例 = 读取概念实例(
            节点, 场景服务, 存在服务, 特征服务,
            特征概念服务, 存在概念服务);
        if (!实例 || 实例->所属场景节点 != 场景节点) {
            return std::nullopt;
        }
        结果.push_back(*实例);
    }
    return 结果;
}

bool 结构仍存在(const 新_场景类& 场景服务) {
    if (!有效(自然语言世界树根节点)
        || !节点仍存在(自然语言世界树根节点)
        || !有效(自然语言概念来源字段)
        || !节点仍存在(自然语言概念来源字段)
        || !场景服务.是场景节点(自然语言世界树根节点)) {
        return false;
    }
    const auto 根 = 场景服务.获取场景(自然语言世界树根节点);
    return 根 && !根->父场景节点;
}

新自然语言场景操作状态 映射场景建立状态(新场景操作状态 状态) {
    switch (状态) {
    case 新场景操作状态::已完成:
        return 新自然语言场景操作状态::已建立;
    case 新场景操作状态::资源失败:
        return 新自然语言场景操作状态::资源失败;
    case 新场景操作状态::父场景不合法:
    case 新场景操作状态::场景不存在:
        return 新自然语言场景操作状态::未初始化;
    case 新场景操作状态::入口拒绝:
        return 新自然语言场景操作状态::入口拒绝;
    default:
        return 新自然语言场景操作状态::结构不一致;
    }
}

enum class 概念配对检查状态 : std::uint8_t {
    可配对 = 1,
    不可配对 = 2,
    结构不一致 = 3,
    资源失败 = 4
};

std::optional<bool> 是上位存在概念(
    稳定编码 上位概念,
    稳定编码 下位概念,
    const 概念_存在类& 存在概念服务) {
    std::vector<稳定编码> 待查{下位概念};
    std::unordered_set<std::uint64_t> 已查;
    while (!待查.empty()) {
        const auto 当前 = 待查.back();
        待查.pop_back();
        if (!已查.insert(当前.值).second) continue;
        const auto 信息 = 存在概念服务.获取存在概念(当前);
        if (!信息) return std::nullopt;
        for (const auto 父概念 : 信息->上位概念组) {
            if (父概念 == 上位概念) return true;
            待查.push_back(父概念);
        }
    }
    return false;
}

概念配对检查状态 检查概念可配对(
    const 新自然语言概念实例信息& 左,
    const 新自然语言概念实例信息& 右,
    const 概念_特征类& 特征概念服务,
    const 概念_存在类& 存在概念服务) {
    if (左.种类 != 右.种类) return 概念配对检查状态::不可配对;
    if (左.概念节点 == 右.概念节点) return 概念配对检查状态::可配对;
    if (左.种类 == 新自然语言概念实例种类::特征概念) {
        const auto 关系 = 特征概念服务.比较单特征概念(
            左.概念节点, 右.概念节点);
        if (关系.状态 == 特征概念规则状态::资源失败) {
            return 概念配对检查状态::资源失败;
        }
        if (关系.状态 != 特征概念规则状态::已完成 || !关系.关系) {
            return 概念配对检查状态::结构不一致;
        }
        return *关系.关系 == 单特征概念关系::左为上位
                || *关系.关系 == 单特征概念关系::右为上位
            ? 概念配对检查状态::可配对
            : 概念配对检查状态::不可配对;
    }

    if (!存在概念服务.是存在概念节点(左.概念节点)
        || !存在概念服务.是存在概念节点(右.概念节点)) {
        return 概念配对检查状态::结构不一致;
    }
    const auto 左包含右 = 是上位存在概念(
        左.概念节点, 右.概念节点, 存在概念服务);
    const auto 右包含左 = 是上位存在概念(
        右.概念节点, 左.概念节点, 存在概念服务);
    if (!左包含右 || !右包含左) {
        return 概念配对检查状态::结构不一致;
    }
    return *左包含右 || *右包含左
        ? 概念配对检查状态::可配对
        : 概念配对检查状态::不可配对;
}

std::int64_t 饱和场景差异(
    std::size_t 左数量,
    std::size_t 右数量,
    std::size_t 相同配对数量,
    std::size_t 上下位配对数量) noexcept {
    const auto 最大 = static_cast<std::uint64_t>(
        (std::numeric_limits<std::int64_t>::max)());
    const auto 左未配对 = 左数量 - 相同配对数量 - 上下位配对数量;
    const auto 右未配对 = 右数量 - 相同配对数量 - 上下位配对数量;
    std::uint64_t 差异 = static_cast<std::uint64_t>(上下位配对数量);
    const auto 增加 = [&差异, 最大](std::size_t 数量) noexcept {
        const auto 值 = static_cast<std::uint64_t>(数量);
        差异 = 值 > 最大 - 差异 ? 最大 : 差异 + 值;
    };
    增加(左未配对);
    增加(右未配对);
    return static_cast<std::int64_t>(差异);
}

} // namespace

新_自然语言世界树类::新_自然语言世界树类(
    概念_特征类& 特征概念服务,
    概念_存在类& 存在概念服务,
    新_特征类& 特征服务,
    新_存在类& 存在服务,
    新_状态类& 状态服务,
    新_动态类& 动态服务) noexcept
    : 特征概念服务_(特征概念服务),
      存在概念服务_(存在概念服务),
      特征服务_(特征服务),
      存在服务_(存在服务),
      状态服务_(状态服务), 动态服务_(动态服务),
      场景服务_(
          自然语言世界树根节点, 存在服务, 状态服务, 动态服务) {}

bool 新_自然语言世界树类::接入词条概念挂接(
    新_词类& 词, 概念_名称关系类& 名称, 新_特征值类& 值) {
    std::unique_lock 锁(自然语言世界树互斥);
    if (!已初始化()) return false;
    const auto 范围 = 存在概念服务_.建立或取得正式概念身份范围();
    if (!范围.信息) return false;
    特征概念定义 定义;
    定义.材料类型 = 特征概念材料物理类型::信息节点引用;
    定义.比较规则 = 特征概念比较规则::信息引用身份精确;
    定义.聚合规则 = 特征概念聚合规则::不自动聚合;
    定义.值域 = 特征概念信息引用值域{信息引用语义::概念定义, 范围.信息->节点};
    const 正式概念身份读取接口* 提供者[] = {&存在概念服务_};
    先天特征类型槽结果 类型;
    for (;;) {
        类型 = 特征概念服务_.建立或取得先天特征类型槽(
            {先天特征类型槽::概念名称挂接, 定义}, 值, 提供者);
        if (类型.状态 != 特征概念规则状态::前提已变化) break;
        锁.unlock();
        std::this_thread::yield();
        锁.lock();
    }
    if ((类型.状态 != 特征概念规则状态::已完成 && 类型.状态 != 特征概念规则状态::无变化) || !类型.类型根) return false;
    auto 绑定 = 场景服务_.取得场景状态参与绑定(自然语言世界树根节点);
    词.装配概念挂接(特征服务_, 值, 状态服务_, 动态服务_, *类型.类型根, 绑定);
    锁.unlock();
    for (const auto W : 词.读取启动词条组()) {
        for (;;) {
            if (!场景服务_.是场景节点(自然语言世界树根节点)
                || !全局基础数据集.查询节点(*类型.类型根)) return false;
            const auto 当前词 = 词.获取词条(W);
            if (当前词.状态 == 新词条查询状态::未找到) break;
            if (当前词.状态 != 新词条查询状态::已找到) return false;
            基础节点组追加状态 状态;
            try {
                const auto 概念组 = 名称.读取词条当前概念组(W);
                状态 = 词.补齐词条概念挂接(W, 概念组);
            } catch (基础节点组追加状态 变化) { 状态 = 变化; }
            if (状态 == 基础节点组追加状态::已完成) break;
            if (状态 != 基础节点组追加状态::预期不符
                && 状态 != 基础节点组追加状态::节点不存在
                && 状态 != 基础节点组追加状态::引用不存在) return false;
            std::this_thread::yield();
        }
    }
    锁.lock();
    词条场景绑定_ = std::move(绑定);
    return true;
}

新场景操作状态 新_自然语言世界树类::挂接已发生词条变化动态(
    稳定编码 所属语言场景, 稳定编码 动态) noexcept {
    // 场景owner负责根归属、两态挂接及保持；失败不删除已发生的动态。
    return 场景服务_.添加动态(所属语言场景, 动态);
}

新自然语言词条场景记录结果 新_自然语言世界树类::读取词条场景记录() const {
    std::lock_guard 锁(自然语言世界树互斥);
    if (!词条场景绑定_) return {};
    auto 记录 = 场景服务_.读取词条场景记录(*词条场景绑定_);
    switch (记录.状态) {
    case 新词条场景记录读取状态::已读取:
        return {新自然语言场景结构比较状态::已完成, std::move(记录)};
    case 新词条场景记录读取状态::场景不存在:
        return {新自然语言场景结构比较状态::场景不存在, {}};
    case 新词条场景记录读取状态::资源失败:
        return {新自然语言场景结构比较状态::资源失败, {}};
    }
    throw std::logic_error("word scene record returned unknown status");
}

bool 新自然语言世界初始化结果::成功() const noexcept {
    return (状态 == 新自然语言世界初始化状态::已完成
            || 状态 == 新自然语言世界初始化状态::无变化)
        && 根节点 && 有效(*根节点);
}
新自然语言场景复制结果 新_自然语言世界树类::读取复制场景(稳定编码 n) const {
    using S=新自然语言场景结构比较状态;
    std::lock_guard lock(自然语言世界树互斥);
    try {
    if(!有效(自然语言世界树根节点)||!有效(自然语言概念来源字段))return {S::未初始化};
    if(!结构仍存在(场景服务_))throw std::runtime_error("initialized language world structure corrupt");
    const auto scope=场景服务_.读取指定场景范围完整(n);
    if(scope.状态==新场景范围成员读取状态::资源失败)return {S::资源失败};
    if(scope.状态==新场景范围成员读取状态::起点不可读||scope.状态==新场景范围成员读取状态::不属本树)return {};
    if(scope.状态==新场景范围成员读取状态::读取已变化)return {S::前提已变化};
    if(scope.状态!=新场景范围成员读取状态::已完成)throw std::runtime_error("language source scene range read failed");
    if(n==自然语言世界树根节点||scope.直接父场景节点!=std::optional{自然语言世界树根节点})return {};
    const auto scene=场景服务_.获取场景(n);
    if(!scene)throw std::runtime_error("language scene source identity unreadable");
    if(!scene->直接子场景组.empty()||!scene->状态节点组.empty()||!scene->动态节点组.empty())
        return {S::复制内容未支持};
    新自然语言场景复制信息 info{*scene};
    std::map<稳定编码,新存在信息> members;
    std::map<稳定编码,基础联合节点快照> definitionProtection;
    const auto protectOne=[&](const 基础联合节点快照& x) {
        const auto [it,added]=definitionProtection.emplace(x.结构.节点.编码,x);
        if(!added&&(it->second.结构.字段组!=x.结构.字段组||it->second.入向字段组!=x.入向字段组
            ||it->second.入向外部关系组!=x.入向外部关系组||it->second.出向外部关系组!=x.出向外部关系组))
            throw 语言复制前提变化{};
    };
    const auto protect=[&](const auto& group) {
        for(const auto& x:group)protectOne(x);
    };
    const auto protectConcept=[&](稳定编码 C) {
        const auto existence=存在概念服务_.读取普通存在概念定义保护(C);
        if(existence.状态==正式概念读取状态::已读取){protect(existence.完整保护组);return;}
        if(existence.状态==正式概念读取状态::前提已变化)throw 语言复制前提变化{};
        if(existence.状态!=正式概念读取状态::非本提供者概念)throw std::runtime_error("language source existence concept read failed");
        const auto feature=特征概念服务_.读取特征概念定义保护(C);
        if(feature.前提已变化)throw 语言复制前提变化{};
        if(feature.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
        if(feature.状态!=基础结构操作状态::已完成||!feature.信息)throw std::runtime_error("language source feature concept read failed");
        protect(feature.预期节点组);
    };
    protectConcept(scene->专属存在概念节点);
    std::set<稳定编码> ids{n,自然语言世界树根节点,自然语言概念来源字段,scene->专属存在概念节点};
    ids.insert(scene->特征节点组.begin(),scene->特征节点组.end());
    for(const auto E:scene->直接成员存在组) {
        const auto e=存在服务_.获取存在(E);
        if(!e)throw std::runtime_error("language scene member has no existence identity");
        members.emplace(E,*e);protectConcept(e->专属存在概念节点);
        ids.insert(E);ids.insert(e->专属存在概念节点);
        const auto fields=全局基础数据集.查询字段(E,自然语言概念来源字段);
        if(fields.empty())info.普通成员存在组.push_back(E);
        else {
            const auto instance=读取概念实例(E,场景服务_,存在服务_,特征服务_,特征概念服务_,存在概念服务_);
            if(!instance||instance->所属场景节点!=n)throw std::runtime_error("language concept instance source corrupt");
            info.概念实例组.push_back(*instance);ids.insert(instance->概念节点);
            protectConcept(instance->概念节点);
        }
        ids.insert(e->特征节点组.begin(),e->特征节点组.end());
    }
    for(const auto E:scene->边界存在组) {
        const auto e=存在服务_.读取存在身份保护(E);
        if(e.状态==基础结构操作状态::资源失败)return {S::资源失败};
        if(e.前提已变化)return {S::前提已变化};
        if(e.状态!=基础结构操作状态::已完成||!e.信息)throw std::runtime_error("language scene boundary identity missing");
        ids.insert(E);ids.insert(e.信息->专属存在概念节点);
        protect(e.预期节点组);protectConcept(e.信息->专属存在概念节点);
    }
    auto snapshots=全局基础数据集.查询节点组完整结构({ids.begin(),ids.end()});
    if(snapshots.状态==基础结构操作状态::资源失败)return {S::资源失败};
    if(snapshots.状态!=基础结构操作状态::已完成)throw std::runtime_error("language source snapshot failed");
    新自然语言场景复制结果 result{S::已完成,info};
    for(const auto& x:snapshots.节点组){if(!x.快照)throw std::runtime_error("language source dependency disappeared");protectOne(*x.快照);}
    if(场景服务_.获取场景(n)!=scene)return {S::前提已变化};
    for(const auto& [E,e]:members)if(存在服务_.获取存在(E)!=std::optional{e})return {S::前提已变化};
    for(const auto& e:info.概念实例组)if(读取概念实例(e.节点,场景服务_,存在服务_,特征服务_,特征概念服务_,存在概念服务_)!=std::optional{e})return {S::前提已变化};
    ids.clear();for(auto& [id,p]:definitionProtection){ids.insert(id);result.完整保护.push_back(std::move(p));}
    const auto fresh=全局基础数据集.查询节点组完整结构({ids.begin(),ids.end()});
    if(fresh.状态==基础结构操作状态::资源失败)return {S::资源失败};
    if(fresh.状态!=基础结构操作状态::已完成||fresh.节点组.size()!=result.完整保护.size())
        throw std::runtime_error("language source recheck failed");
    for(std::size_t i=0;i<fresh.节点组.size();++i) {
        const auto& a=result.完整保护[i];const auto& b=fresh.节点组[i].快照;
        if(!b||a.结构.字段组!=b->结构.字段组||a.入向字段组!=b->入向字段组
            ||a.入向外部关系组!=b->入向外部关系组||a.出向外部关系组!=b->出向外部关系组)
            return {S::前提已变化};
    }return result;
    }catch(const 语言复制前提变化&){return {S::前提已变化};}
    catch(const std::bad_alloc&){return {S::资源失败};}
}

bool 新自然语言概念实例建立结果::成功() const noexcept {
    return 状态 == 新自然语言概念实例操作状态::已建立
        && 实例 && 有效(实例->节点) && 有效(实例->所属场景节点)
        && 有效(实例->概念节点);
}

新自然语言世界初始化结果 新_自然语言世界树类::初始化() noexcept {
    新自然语言世界初始化结果 结果;
    try {
        std::lock_guard 锁(自然语言世界树互斥);
        const bool 原根已存在 = 有效(自然语言世界树根节点);
        const bool 原来源字段已存在 = 有效(自然语言概念来源字段);
        if (!场景服务_.初始化()) {
            结果.状态 = 原根已存在 || 原来源字段已存在
                ? 新自然语言世界初始化状态::结构不一致
                : 新自然语言世界初始化状态::资源失败;
            return 结果;
        }
        const auto 根 = 场景服务_.建立根场景();
        if ((根.状态 != 新场景操作状态::已完成
                && 根.状态 != 新场景操作状态::无变化)
            || !根.场景节点) {
            结果.状态 = 根.状态 == 新场景操作状态::资源失败
                ? 新自然语言世界初始化状态::资源失败
                : 新自然语言世界初始化状态::结构不一致;
            return 结果;
        }
        for (const auto 全局根 : {新_全局根类::初始化(),
                新_全局根类::挂接世界根(世界类型登记键::自然语言, *根.场景节点)}) {
            if (全局根 != 全局根操作状态::已完成 && 全局根 != 全局根操作状态::无变化) {
                结果.状态 = 全局根 == 全局根操作状态::资源失败
                    ? 新自然语言世界初始化状态::资源失败
                    : 新自然语言世界初始化状态::结构不一致;
                return 结果;
            }
        }
        if (!有效(自然语言概念来源字段)) {
            自然语言概念来源字段 = 全局基础数据集.新建节点();
            if (!有效(自然语言概念来源字段)) {
                结果.状态 = 新自然语言世界初始化状态::资源失败;
                return 结果;
            }
        }
        if (!结构仍存在(场景服务_)) {
            结果.状态 = 新自然语言世界初始化状态::结构不一致;
            return 结果;
        }
        结果.状态 = 原根已存在 && 原来源字段已存在
            ? 新自然语言世界初始化状态::无变化
            : 新自然语言世界初始化状态::已完成;
        结果.根节点 = 自然语言世界树根节点;
        return 结果;
    } catch (...) {
        结果.状态 = 新自然语言世界初始化状态::资源失败;
        return 结果;
    }
}

bool 新_自然语言世界树类::已初始化() const noexcept {
    try {
        std::lock_guard 锁(自然语言世界树互斥);
        return 结构仍存在(场景服务_);
    } catch (...) {
        return false;
    }
}

std::optional<稳定编码> 新_自然语言世界树类::获取根节点() const noexcept {
    try {
        std::lock_guard 锁(自然语言世界树互斥);
        return 结构仍存在(场景服务_)
            ? std::optional<稳定编码>{自然语言世界树根节点}
            : std::nullopt;
    } catch (...) {
        return std::nullopt;
    }
}

新来源子场景结果 新_自然语言世界树类::建立或取得出站来源场景(稳定编码 来源) noexcept {
    try {
        std::lock_guard 锁(自然语言世界树互斥);
        return 场景服务_.建立或取得来源子场景(自然语言世界树根节点,来源);
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"建立或取得出站来源场景",L"出站来源场景工程错误。");
    }
}
新接收语言解释场景结果 新_自然语言世界树类::读取出站表达解释场景(稳定编码 来源) const {
    std::lock_guard 锁(自然语言世界树互斥);
    return 场景服务_.读取接收语言解释场景(场景服务_.读取来源子场景(自然语言世界树根节点, 来源));
}

新来源子场景结果 新_自然语言世界树类::建立或取得自我写入来源场景(稳定编码 写入来源) noexcept {
    try {
        std::lock_guard 锁(自然语言世界树互斥);
        return 场景服务_.建立或取得来源子场景(自然语言世界树根节点,写入来源);
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"建立或取得自我写入来源场景",L"自我写入来源场景工程错误。");
    }
}
新来源子场景结果 新_自然语言世界树类::建立或取得接收来源场景(稳定编码 消息) noexcept {
    std::lock_guard 锁(自然语言世界树互斥);
    return 场景服务_.建立或取得来源子场景(自然语言世界树根节点, 消息);
}

新语言分界调用结果<新语言材料范围信息> 新_自然语言世界树类::建立或取得接收原文范围(
    新_语言类& 语言, const 新接收认知来源键& 来源, 稳定编码 前置来源节点, 稳定编码 语言节点) {
    using S = 新语言分界工程状态;
    using 场景状态 = 新来源子场景状态;
    const auto 场景 = 建立或取得接收来源场景(来源.接收消息节点);
    switch (场景.状态) {
    case 场景状态::资源失败: return {S::资源失败, {}, {}};
    case 场景状态::前提已变化: return {S::前提已变化, {}, {}};
    case 场景状态::未建立: return {S::材料未就绪, {}, {}};
    case 场景状态::输入不合法: return {S::输入拒绝, {}, {}};
    case 场景状态::已建立:
    case 场景状态::已存在:
        if (!场景.信息) throw std::logic_error("received source scene result is empty");
        return 语言.建立或取得材料范围({前置来源节点, 来源.原文起点, 来源.原文终点,
            语言节点, 严格UTF8字节精确规范化合同节点, 场景.信息->场景节点}, 场景服务_);
    }
    throw std::logic_error("unknown received source scene state");
}

新语言分界调用结果<稳定编码> 新_自然语言世界树类::采用接收分词候选(
    新_语言类& 语言, 稳定编码 R, 稳定编码 前置来源,
    const 新_特征值类& 值, const 新_词类& 词条, std::stop_token 停止) {
    return 语言.采用接收分词候选(R, 前置来源, 值, 词条, 场景服务_, 状态服务_, 停止);
}

新语言分界调用结果<新语言分界材料信息> 新_自然语言世界树类::采用接收词条准入(
    新_语言类& 语言, 稳定编码 R, 稳定编码 前置来源,
    const 新_特征值类& 值, 新_词类& 词条, std::stop_token 停止) {
    return 语言.采用接收词条准入(R, 前置来源, 值, 词条, 场景服务_, 状态服务_, 停止);
}

新语言分界调用结果<新语言分界采用回执> 新_自然语言世界树类::采用语言分界材料(
    新_语言类& 语言, const 新语言分界采用请求& 请求) {
    return 语言.采用分界材料(请求, 场景服务_, 状态服务_);
}

新语言分界调用结果<新语言分界依据采用回执> 新_自然语言世界树类::采用已形成分界依据(
    新_语言类& 语言, const 新语言分界依据采用请求& 请求, std::stop_token 停止) {
    return 语言.采用已形成分界依据(请求, 场景服务_, 状态服务_, 停止);
}

新来源子场景结果 新_自然语言世界树类::建立或取得词典来源场景(稳定编码 材料Q) noexcept {
    std::lock_guard 锁(自然语言世界树互斥);
    return 场景服务_.建立或取得来源子场景(自然语言世界树根节点, 材料Q);
}

新来源子场景结果 新_自然语言世界树类::读取接收来源场景(稳定编码 消息) const noexcept {
    std::lock_guard 锁(自然语言世界树互斥);
    return 场景服务_.读取来源子场景(自然语言世界树根节点, 消息);
}

新接收语言解释场景结果 新_自然语言世界树类::读取接收语言解释场景(
    稳定编码 消息节点) const {
    return 场景服务_.读取接收语言解释场景(读取接收来源场景(消息节点));
}

新接收局部对象结果 新_自然语言世界树类::建立或取得接收局部对象(
    const 新语言前置来源信息& 来源, std::uint64_t 起始, std::uint64_t 结束, 稳定编码 构式) {
    if (!有效(来源.节点) || 起始 < 来源.来源.原文起点 || 结束 > 来源.来源.原文终点 || 起始 >= 结束)
        return {};
    std::lock_guard 锁(自然语言世界树互斥);
    return 场景服务_.建立或取得接收局部对象(读取接收来源场景(来源.来源.接收消息节点),
        {来源.节点, 构式, 起始, 结束});
}

新接收局部对象组结果 新_自然语言世界树类::读取接收局部对象组(稳定编码 消息节点) const {
    return 场景服务_.读取接收局部对象组(读取接收来源场景(消息节点));
}

新局部组成结果 新_自然语言世界树类::采用接收局部组成(
    稳定编码 对象来源消息, 稳定编码 父, 稳定编码 子,
    const 新局部具名特征来源& 来源, 基础只读预期 预期, const 新状态强时间& 时间) {
    return 场景服务_.采用接收局部组成(读取接收来源场景(对象来源消息),
        父, 子, 来源, std::move(预期), 时间, 特征服务_);
}

std::optional<新局部组成信息> 新_自然语言世界树类::读取接收局部组成(稳定编码 子) const {
    return 场景服务_.读取接收局部组成(子, 特征服务_);
}

新特征建立结果 新_自然语言世界树类::采用接收局部具名特征(
    稳定编码 对象来源消息, 稳定编码 E, 稳定编码 C,
    const 新局部具名特征来源& 来源, 基础只读预期 预期) {
    return 场景服务_.采用接收局部具名特征(读取接收来源场景(对象来源消息),
        E, C, 来源, std::move(预期), 特征服务_);
}

std::optional<新局部具名特征信息> 新_自然语言世界树类::读取接收局部具名特征(稳定编码 E) const {
    return 场景服务_.读取接收局部具名特征(E, 特征服务_);
}

namespace {
struct 有序比较角色 final {
    稳定编码 主体, 参照, 特征域;
};
std::optional<有序比较角色> 取得有序高于角色(const 接收语言句段解释& 句) {
    using O = 新语言知识含义操作;
    using P = 新语言知识参数角色;
    using C = 新语言知识角色类别;
    if (!句.静态结构 || 句.静态结构->操作 != O::特征有序大于
        || !句.缺口组.empty() || 句.静态结构->角色组.size() != 3) return {};
    const auto& 组 = 句.静态结构->角色组;
    const auto 取 = [&](P 参数, C 类别) -> std::optional<稳定编码> {
        const auto 项 = std::ranges::find(组, 参数, &接收语言已解释角色::参数);
        const auto* 值 = 项 == 组.end() || !项->值
            ? nullptr : std::get_if<语言图引用值>(&*项->值);
        return 值 && 值->类别 == 类别 ? std::optional{值->节点} : std::nullopt;
    };
    const auto 主 = 取(P::主体, C::存在);
    const auto 参 = 取(P::参照对象, C::存在);
    const auto 域 = 取(P::特征类型, C::I64标量特征类型);
    if (!主 || !参 || !域 || *主 == *参) return {};
    return 有序比较角色{*主, *参, *域};
}

bool 具备合适温度方向定义(const 二次特征定义保护结果& 定义, 稳定编码 类型) {
    const auto& 信息 = 定义.信息;
    return 定义.状态 == 基础结构操作状态::已完成 && !定义.前提已变化
        && 信息 && 信息->节点 == 类型 && 信息->二次特征类型根节点 == 类型
        && 信息->算法 == 二次特征算法::语言温度比较方向
        && 信息->语言适用范围 && 信息->输入定义组.size() == 2
        && 信息->输入定义组.front().特征类型概念节点 && !定义.预期节点组.empty();
}
}

bool 新_自然语言世界树类::接入语言温度方向类型(
    const 概念_二次特征类& 概念, 稳定编码 类型) noexcept {
    try {
        std::lock_guard 锁(自然语言世界树互斥);
        if (!具备合适温度方向定义(概念.读取二次特征定义保护(类型), 类型)) return false;
        if (二次概念读取接口_ && (二次概念读取接口_ != &概念 || 语言温度方向类型_ != 类型))
            return false;
        二次概念读取接口_ = &概念;
        语言温度方向类型_ = 类型;
        return true;
    } catch (...) { return false; }
}

基础节点组追加状态 新_自然语言世界树类::采用接收语言场景方向(
    const 新接收认知来源键& 键, const 新接收语言解释信息& 解释,
    const 新_语言类& 语言, const 新语言分界消息读取接口& 消息读取) {
    using A = 基础节点组追加状态;
    using S = 基础结构操作状态;
    using L = 新语言分界工程状态;
    using W = 新来源子场景状态;
    try {
        if (!键.根出现路径.empty()) return A::已完成;
        if (std::ranges::none_of(解释.内容.句段组, [](const auto& 句) {
                return 取得有序高于角色(句).has_value();
            })) return A::已完成;

        const auto 正式 = 语言.读取接收语言解释定义来源(解释.解释定义节点, *this, 消息读取);
        if (正式.状态 == L::资源失败) return A::资源失败;
        if (正式.状态 == L::前提已变化 || 正式.状态 == L::未找到) return A::预期不符;
        if (正式.状态 != L::已完成 || !正式.信息) return A::内部不一致;
        const auto& 材料 = *正式.信息;
        if (材料.前置来源.来源 != 键 || 材料.接收消息.消息节点 != 键.接收消息节点
            || 材料.内容 != 解释.内容) return A::预期不符;
        const auto 场景 = 材料.内容.来源场景;
        const auto 场景来源 = 读取接收来源场景(键.接收消息节点);
        if (场景来源.状态 == W::资源失败) return A::资源失败;
        if (场景来源.状态 != W::已存在 || !场景来源.信息
            || 场景来源.信息->场景节点 != 场景) return A::预期不符;
        const auto 直接成员 = 场景服务_.读取接收局部对象组(场景来源);
        if (直接成员.状态 == W::资源失败) return A::资源失败;
        if (直接成员.状态 != W::已存在) return A::预期不符;
        if (直接成员.对象组.size() < 2) return A::已完成;

        if (!二次概念读取接口_) return A::内部不一致;
        const auto 类型 = 二次概念读取接口_->读取二次特征定义保护(语言温度方向类型_);
        if (类型.状态 == S::资源失败) return A::资源失败;
        if (类型.前提已变化) return A::预期不符;
        if (!具备合适温度方向定义(类型, 语言温度方向类型_)) return A::内部不一致;
        const auto& 域 = 类型.信息->输出特征概念.值域;
        const auto* 值组 = std::get_if<std::vector<稳定编码>>(&域);
        if (!值组) return A::内部不一致;
        稳定编码 正向;
        for (const auto V : *值组) {
            const auto 实料 = 特征服务_.特征值服务_.获取特征值(V);
            if (实料 && 实料->材料 == 新特征值材料{std::string{">0"}}) 正向 = V;
        }
        if (!有效(正向)) return A::内部不一致;
        auto 值保护 = 特征服务_.特征值服务_.读取特征值保护(正向);
        if (值保护.状态 == 新特征值保护读取状态::资源失败) return A::资源失败;
        if (值保护.状态 != 新特征值保护读取状态::已读取 || !值保护.信息
            || 值保护.信息->材料 != 新特征值材料{std::string{">0"}}) return A::内部不一致;

        std::map<稳定编码, 基础联合节点快照> 快照;
        for (const auto& 项 : 场景来源.完整保护) 快照.emplace(项.结构.节点.编码, 项);
        for (const auto& 项 : 类型.预期节点组) 快照.emplace(项.结构.节点.编码, 项);
        const auto 来源节点 = 全局基础数据集.查询节点组完整结构(
            {材料.记录节点, 材料.解释定义节点, 材料.前置来源.节点});
        if (来源节点.状态 == S::资源失败) return A::资源失败;
        if (来源节点.状态 != S::已完成) return A::预期不符;
        for (const auto& 项 : 来源节点.节点组) {
            if (!项.快照) return A::预期不符;
            快照.emplace(项.查询节点, *项.快照);
        }

        std::vector<新语言场景方向实例> 项组;
        std::set<std::pair<稳定编码, 稳定编码>> 已键;
        std::set<稳定编码> 无直接父端点;
        for (const auto& 句 : 材料.内容.句段组) {
            const auto 角色 = 取得有序高于角色(句);
            if (!角色 || 角色->特征域 != 类型.信息->输入定义组.front().特征类型概念节点) continue;
            const auto 是同源在先直成员 = [&](稳定编码 E) {
                const auto 项 = std::ranges::find(直接成员.对象组, E, &新接收局部对象信息::存在);
                return 项 != 直接成员.对象组.end() && 项->场景 == 场景
                    && 项->当前概念 == 类型.信息->语言适用范围->适用存在根
                    && 项->声明.前置来源 == 材料.前置来源.节点
                    && 项->声明.结束标量 <= 句.起始标量;
            };
            if (!是同源在先直成员(角色->主体) || !是同源在先直成员(角色->参照)) continue;
            bool 有可能父存在 = false;
            for (const auto E : {角色->主体, 角色->参照}) {
                const auto 身份 = 存在服务_.读取存在身份保护(E);
                if (身份.状态 == S::资源失败) return A::资源失败;
                if (身份.前提已变化 || 身份.状态 == S::节点不存在) return A::预期不符;
                if (身份.状态 != S::已完成 || !身份.信息 || 身份.预期节点组.size() != 1)
                    return A::内部不一致;
                if (身份.信息->专属存在概念节点 != 类型.信息->语言适用范围->适用存在根)
                    return A::预期不符;
                const auto& 节点 = 身份.预期节点组.front();
                有可能父存在 |= std::ranges::any_of(节点.入向外部关系组, [](const auto& 边) {
                    return 边.类型 == 基础外部关系类型::父子;
                });
                快照.emplace(E, 节点);
            }
            if (有可能父存在 || !已键.emplace(角色->主体, 角色->参照).second) continue;
            无直接父端点.insert(角色->主体);
            无直接父端点.insert(角色->参照);
            项组.push_back({场景, 语言温度方向类型_, 角色->主体, 角色->参照, 正向, 键.接收消息节点,
                材料.解释定义节点, 句.静态结构->所用构式, 句.起始标量, 句.结束标量});
        }
        if (项组.empty()) return A::已完成;
        std::vector<基础联合节点快照> 证明;
        证明.reserve(快照.size());
        for (auto& 项 : 快照) 证明.push_back(std::move(项.second));
        for (const auto E : 无直接父端点)
            值保护.窄预期.外部关系集合组.push_back({E, false, 基础外部关系类型::父子, {}, {}, {}});
        return 特征服务_.建立首批语言场景方向实例(场景, 项组, std::move(证明), std::move(值保护.窄预期));
    } catch (const std::bad_alloc&) { return A::资源失败; }
      catch (const std::length_error&) { return A::资源失败; }
}

新语言场景方向实例读取 新_自然语言世界树类::读取接收语言场景方向(
    稳定编码 M, const 新_语言类& 语言, const 新语言分界消息读取接口& 消息读取) const {
    using S = 基础结构操作状态;
    using L = 新语言分界工程状态;
    try {
        const auto 场景 = 读取接收来源场景(M);
        if (场景.状态 == 新来源子场景状态::资源失败) return {S::资源失败};
        if (场景.状态 != 新来源子场景状态::已存在 || !场景.信息) return {S::节点不存在};
        auto 结果 = 特征服务_.读取语言场景方向实例(场景.信息->场景节点);
        if (结果.状态 != S::已完成 || 结果.实例组.empty()) return 结果;
        if (!二次概念读取接口_) return {S::内部不一致};
        const auto 类型 = 二次概念读取接口_->读取二次特征定义保护(语言温度方向类型_);
        if (类型.状态 == S::资源失败) return {S::资源失败};
        if (!具备合适温度方向定义(类型, 语言温度方向类型_)) return {S::内部不一致};
        const auto* 值组 = std::get_if<std::vector<稳定编码>>(&类型.信息->输出特征概念.值域);
        if (!值组) return {S::内部不一致};
        for (const auto& 项 : 结果.实例组) {
            if (项.消息 != M || 项.类型 != 语言温度方向类型_ || std::ranges::find(*值组, 项.值) == 值组->end())
                return {S::内部不一致};
            const auto 值 = 特征服务_.特征值服务_.获取特征值(项.值);
            if (!值 || 值->材料 != 新特征值材料{std::string{">0"}}) return {S::内部不一致};
            const auto 原 = 语言.读取接收语言解释句段来源(
                项.解释定义, 项.起始标量, 项.结束标量, *this, 消息读取);
            if (原.状态 == L::资源失败) return {S::资源失败};
            if (原.状态 != L::已完成 || !原.信息) return {S::内部不一致};
            const auto& 来源 = *原.信息;
            const auto 角色 = 取得有序高于角色(来源.句段);
            if (!角色 || 来源.接收消息.消息节点 != M || 来源.来源场景 != 项.场景
                || 来源.句段.静态结构->所用构式 != 项.构式 || 角色->主体 != 项.主体
                || 角色->参照 != 项.参照 || 角色->特征域 != 类型.信息->输入定义组.front().特征类型概念节点)
                return {S::内部不一致};
        }
        return 结果;
    } catch (const std::bad_alloc&) { return {S::资源失败}; }
      catch (const std::length_error&) { return {S::资源失败}; }
}

新语言分界调用结果<稳定编码> 新_自然语言世界树类::同步接收指代实例(
    新_语言类& 语言, const 新接收认知来源键& 来源, const 新语言分界消息读取接口& 消息读取) {
    return 语言.同步接收指代实例(来源, *this, 消息读取, 场景服务_, 状态服务_);
}

新场景状态参与绑定 新_自然语言世界树类::取得消息解释状态参与绑定(
    const 新接收语言解释场景绑定& 绑定) {
    return 场景服务_.取得场景状态参与绑定(绑定.场景());
}

基础节点组追加状态 新_自然语言世界树类::封存消息解释指代状态挂接(
    const 新场景状态参与绑定& 绑定, 新指代关系初建准备& 准备) const {
    return 场景服务_.封存指代关系状态组挂接(绑定, 准备);
}

bool 新_自然语言世界树类::合并接收指代实例挂接(
    const 新接收语言解释场景绑定& 绑定, 基础节点组追加请求& 请求,
    基础组内节点引用 实例) const {
    return 场景服务_.合并接收指代实例挂接(绑定, 请求, 实例);
}

bool 新_自然语言世界树类::合并接收语言解释首次挂接(
    const 新接收语言解释场景绑定& 绑定, 基础节点组追加请求& 请求,
    基础组内节点引用 记录) const {
    return 场景服务_.合并接收语言解释首次挂接(绑定, 请求, 记录);
}

新自然语言场景建立结果 新_自然语言世界树类::建立空场景() noexcept {
    新自然语言场景建立结果 结果;
    try {
        std::lock_guard 锁(自然语言世界树互斥);
        if (!有效(自然语言世界树根节点)
            || !有效(自然语言概念来源字段)) {
            结果.状态 = 新自然语言场景操作状态::未初始化;
            return 结果;
        }
        if (!结构仍存在(场景服务_)) {
            结果.状态 = 新自然语言场景操作状态::结构不一致;
            return 结果;
        }
        const auto 建立 = 场景服务_.建立场景(自然语言世界树根节点);
        结果.状态 = 映射场景建立状态(建立.状态);
        结果.场景节点 = 建立.场景节点;
        return 结果;
    } catch (...) {
        结果.状态 = 新自然语言场景操作状态::资源失败;
        return 结果;
    }
}

std::optional<新自然语言场景信息> 新_自然语言世界树类::获取场景(
    稳定编码 场景节点) const noexcept {
    try {
        std::lock_guard 锁(自然语言世界树互斥);
        if (!结构仍存在(场景服务_)
            || 场景节点 == 自然语言世界树根节点
            || 场景服务_.查询父场景(场景节点)
                != std::optional<稳定编码>{自然语言世界树根节点}) {
            return std::nullopt;
        }
        return 场景服务_.获取场景(场景节点)
            ? std::optional<新自然语言场景信息>{{场景节点}}
            : std::nullopt;
    } catch (...) {
        return std::nullopt;
    }
}

std::vector<稳定编码> 新_自然语言世界树类::查询全部场景() const noexcept {
    try {
        std::lock_guard 锁(自然语言世界树互斥);
        return 结构仍存在(场景服务_)
            ? 场景服务_.查询直接子场景(自然语言世界树根节点)
            : std::vector<稳定编码>{};
    } catch (...) {
        return {};
    }
}

bool 新_自然语言世界树类::是自然语言场景节点(稳定编码 节点) const noexcept {
    return 获取场景(节点).has_value();
}

bool 新_自然语言世界树类::是自然语言场景存在(稳定编码 节点) const noexcept {
    return 场景服务_.是场景节点(节点) || 场景服务_.查询所属场景(节点).has_value();
}

新自然语言概念实例建立结果 新_自然语言世界树类::实例化概念(
    稳定编码 场景节点,
    稳定编码 概念节点) noexcept {
    新自然语言概念实例建立结果 结果;
    稳定编码 已建实例节点{};
    稳定编码 已建来源字段{};
    try {
        std::lock_guard 锁(自然语言世界树互斥);
        if (!有效(自然语言世界树根节点)
            || !有效(自然语言概念来源字段)) {
            结果.状态 = 新自然语言概念实例操作状态::未初始化;
            return 结果;
        }
        if (!结构仍存在(场景服务_)) {
            结果.状态 = 新自然语言概念实例操作状态::结构不一致;
            return 结果;
        }
        if (!获取场景(场景节点)) {
            结果.状态 = 新自然语言概念实例操作状态::场景不存在;
            return 结果;
        }

        const bool 是特征概念 = 特征概念服务_.是特征概念节点(概念节点);
        const bool 是存在概念 = 存在概念服务_.是存在概念节点(概念节点);
        if (!是特征概念 && !是存在概念) {
            结果.状态 = 新自然语言概念实例操作状态::概念不存在;
            return 结果;
        }
        if (是特征概念 && 是存在概念) {
            结果.状态 = 新自然语言概念实例操作状态::结构不一致;
            return 结果;
        }

        新场景区间成员建立请求 请求;
        if (是特征概念) {
            const auto 概念 = 特征概念服务_.获取特征概念(概念节点);
            if (!概念) {
                结果.状态 = 新自然语言概念实例操作状态::结构不一致;
                return 结果;
            }
            请求.初始特征组.push_back({概念节点, 概念->值域});
        } else {
            const auto 概念 = 存在概念服务_.获取存在概念(概念节点);
            if (!概念) {
                结果.状态 = 新自然语言概念实例操作状态::结构不一致;
                return 结果;
            }
            for (const auto& 特征项 : 概念->特征组) {
                const auto 特征概念 = 特征概念服务_.获取特征概念(
                    特征项.具体特征概念);
                if (!特征概念) {
                    结果.状态 = 新自然语言概念实例操作状态::结构不一致;
                    return 结果;
                }
                请求.初始特征组.push_back({
                    特征项.具体特征概念, 特征概念->值域});
            }
        }

        const auto 建立 = 场景服务_.建立区间成员存在(场景节点, 请求);
        if (建立.状态 != 新场景操作状态::已完成 || !建立.存在节点) {
            结果.状态 = 建立.状态 == 新场景操作状态::资源失败
                ? 新自然语言概念实例操作状态::资源失败
                : 新自然语言概念实例操作状态::结构不一致;
            return 结果;
        }
        const auto 实例节点 = *建立.存在节点;
        已建实例节点 = 实例节点;
        已建来源字段 = 全局基础数据集.添加字段节点(
            实例节点, 自然语言概念来源字段, 概念节点);
        if (!有效(已建来源字段)) {
            (void)场景服务_.删除成员存在(场景节点, 实例节点);
            已建实例节点 = {};
            结果.状态 = 新自然语言概念实例操作状态::资源失败;
            return 结果;
        }
        const auto 读回 = 读取概念实例(
            实例节点, 场景服务_, 存在服务_, 特征服务_,
            特征概念服务_, 存在概念服务_);
        if (!读回 || 读回->所属场景节点 != 场景节点
            || 读回->概念节点 != 概念节点) {
            (void)全局基础数据集.删除字段(已建来源字段);
            (void)场景服务_.删除成员存在(场景节点, 实例节点);
            已建来源字段 = {};
            已建实例节点 = {};
            结果.状态 = 新自然语言概念实例操作状态::结构不一致;
            return 结果;
        }
        结果.状态 = 新自然语言概念实例操作状态::已建立;
        结果.实例 = *读回;
        return 结果;
    } catch (...) {
        if (有效(已建来源字段)) {
            (void)全局基础数据集.删除字段(已建来源字段);
        }
        if (有效(已建实例节点)) {
            (void)场景服务_.删除成员存在(场景节点, 已建实例节点);
        }
        结果.状态 = 新自然语言概念实例操作状态::资源失败;
        return 结果;
    }
}

std::optional<新自然语言概念实例信息>
新_自然语言世界树类::获取概念实例(稳定编码 实例节点) const noexcept {
    try {
        std::lock_guard 锁(自然语言世界树互斥);
        if (!结构仍存在(场景服务_)) return std::nullopt;
        const auto 实例 = 读取概念实例(
            实例节点, 场景服务_, 存在服务_, 特征服务_,
            特征概念服务_, 存在概念服务_);
        return 实例 && 获取场景(实例->所属场景节点)
            ? 实例 : std::nullopt;
    } catch (...) {
        return std::nullopt;
    }
}

std::vector<稳定编码> 新_自然语言世界树类::查询场景概念实例(
    稳定编码 场景节点) const noexcept {
    try {
        std::lock_guard 锁(自然语言世界树互斥);
        std::vector<稳定编码> 结果;
        if (!结构仍存在(场景服务_) || !获取场景(场景节点)) return 结果;
        const auto 实例组 = 读取场景全部概念实例(
            场景节点, 场景服务_, 存在服务_, 特征服务_,
            特征概念服务_, 存在概念服务_);
        if (!实例组) return {};
        for (const auto& 实例 : *实例组) 结果.push_back(实例.节点);
        return 结果;
    } catch (...) {
        return {};
    }
}

bool 新_自然语言世界树类::是自然语言概念实例节点(
    稳定编码 节点) const noexcept {
    return 获取概念实例(节点).has_value();
}

新自然语言场景结构比较结果 新_自然语言世界树类::比较场景结构(
    稳定编码 左场景节点,
    稳定编码 右场景节点) const noexcept {
    新自然语言场景结构比较结果 结果;
    try {
        std::lock_guard 锁(自然语言世界树互斥);
        if (!有效(自然语言世界树根节点)
            || !有效(自然语言概念来源字段)) {
            结果.状态 = 新自然语言场景结构比较状态::未初始化;
            return 结果;
        }
        if (!结构仍存在(场景服务_)) {
            结果.状态 = 新自然语言场景结构比较状态::结构不一致;
            return 结果;
        }
        if (!获取场景(左场景节点) || !获取场景(右场景节点)) {
            结果.状态 = 新自然语言场景结构比较状态::场景不存在;
            return 结果;
        }
        const auto 左实例组 = 读取场景全部概念实例(
            左场景节点, 场景服务_, 存在服务_, 特征服务_,
            特征概念服务_, 存在概念服务_);
        const auto 右实例组 = 读取场景全部概念实例(
            右场景节点, 场景服务_, 存在服务_, 特征服务_,
            特征概念服务_, 存在概念服务_);
        if (!左实例组 || !右实例组) {
            结果.状态 = 新自然语言场景结构比较状态::结构不一致;
            return 结果;
        }

        std::vector<bool> 左已配对(左实例组->size(), false);
        std::vector<bool> 右已配对(右实例组->size(), false);
        std::size_t 相同配对数量 = 0;
        for (std::size_t 左序号 = 0; 左序号 < 左实例组->size(); ++左序号) {
            for (std::size_t 右序号 = 0; 右序号 < 右实例组->size(); ++右序号) {
                if (右已配对[右序号]
                    || (*左实例组)[左序号].种类 != (*右实例组)[右序号].种类
                    || (*左实例组)[左序号].概念节点
                        != (*右实例组)[右序号].概念节点) {
                    continue;
                }
                左已配对[左序号] = true;
                右已配对[右序号] = true;
                ++相同配对数量;
                break;
            }
        }

        std::vector<std::size_t> 左剩余;
        std::vector<std::size_t> 右剩余;
        for (std::size_t 序号 = 0; 序号 < 左已配对.size(); ++序号) {
            if (!左已配对[序号]) 左剩余.push_back(序号);
        }
        for (std::size_t 序号 = 0; 序号 < 右已配对.size(); ++序号) {
            if (!右已配对[序号]) 右剩余.push_back(序号);
        }

        std::vector<std::vector<std::size_t>> 可配对右序号组(左剩余.size());
        for (std::size_t 左序号 = 0; 左序号 < 左剩余.size(); ++左序号) {
            for (std::size_t 右序号 = 0; 右序号 < 右剩余.size(); ++右序号) {
                const auto 检查 = 检查概念可配对(
                    (*左实例组)[左剩余[左序号]],
                    (*右实例组)[右剩余[右序号]],
                    特征概念服务_, 存在概念服务_);
                if (检查 == 概念配对检查状态::资源失败) {
                    结果.状态 = 新自然语言场景结构比较状态::资源失败;
                    return 结果;
                }
                if (检查 == 概念配对检查状态::结构不一致) {
                    结果.状态 = 新自然语言场景结构比较状态::结构不一致;
                    return 结果;
                }
                if (检查 == 概念配对检查状态::可配对) {
                    可配对右序号组[左序号].push_back(右序号);
                }
            }
        }

        std::vector<std::optional<std::size_t>> 右匹配左(右剩余.size());
        std::function<bool(std::size_t, std::vector<bool>&)> 尝试匹配 =
            [&](std::size_t 左序号, std::vector<bool>& 本轮已查右) {
                for (const auto 右序号 : 可配对右序号组[左序号]) {
                    if (本轮已查右[右序号]) continue;
                    本轮已查右[右序号] = true;
                    if (!右匹配左[右序号]
                        || 尝试匹配(*右匹配左[右序号], 本轮已查右)) {
                        右匹配左[右序号] = 左序号;
                        return true;
                    }
                }
                return false;
            };
        std::size_t 上下位配对数量 = 0;
        for (std::size_t 左序号 = 0; 左序号 < 左剩余.size(); ++左序号) {
            std::vector<bool> 本轮已查右(右剩余.size(), false);
            if (尝试匹配(左序号, 本轮已查右)) ++上下位配对数量;
        }

        结果.状态 = 新自然语言场景结构比较状态::已完成;
        结果.差异 = 饱和场景差异(
            左实例组->size(), 右实例组->size(),
            相同配对数量, 上下位配对数量);
        return 结果;
    } catch (...) {
        结果.状态 = 新自然语言场景结构比较状态::资源失败;
        结果.差异.reset();
        return 结果;
    }
}

} // namespace 海中鱼巣
