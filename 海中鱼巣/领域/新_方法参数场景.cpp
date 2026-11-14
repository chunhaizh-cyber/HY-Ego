#include "新_方法参数场景.h"
#include "新_方法类.h"
#include "新_存在类.h"
#include "../核心/容错检查.h"

#include <utility>
#include <algorithm>
#include <thread>
#include <stdexcept>

namespace 海中鱼巣 {
新模拟需求读回 方法参数场景::读取模拟需求参数(std::uint32_t pos) const {
    if(用途_!=新方法参数场景用途::模拟||!模拟运行_||!特征概念读取_)
        return {新模拟需求状态::输入不合法};
    const auto* p=查询参数(pos);
    if(!p||p->信息类型!=新方法参数信息类型::特征)return {新模拟需求状态::输入不合法};
    const auto f=模拟运行_->读取模拟特征(p->信息节点);
    if(!f)return {};
    const auto d=特征概念读取_->获取特征概念(f->特征概念节点);
    if(!d||d->材料类型!=特征概念材料物理类型::需求节点引用
        ||d->比较规则!=特征概念比较规则::需求身份精确)return {新模拟需求状态::输入不合法};
    if(!f->当前值)return {};
    const auto* D=std::get_if<稳定编码>(&*f->当前值);
    if(!D)throw std::logic_error("local demand feature has an invalid current reference");
    return 模拟运行_->读取模拟需求(*D);
}
新本能交互运行接口* 方法参数场景::获取交互运行接口() const noexcept {
    return 用途_==新方法参数场景用途::模拟&&模拟运行_ ? 模拟运行_->获取交互运行接口() : nullptr;
}
新本能需求运行接口* 方法参数场景::获取需求运行接口() const noexcept {
    return 需求运行_ ? 需求运行_.get()
        : 模拟运行_ ? 模拟运行_->获取需求运行接口() : nullptr;
}
新本能字段询问运行接口* 方法参数场景::获取字段询问运行接口() const noexcept {
    if (用途_==新方法参数场景用途::模拟)
        return 模拟运行_ ? 模拟运行_->获取字段询问运行接口() : nullptr;
    return 字段询问运行_.get();
}
namespace {

struct 参数材料需要重读 {};

std::optional<新词条特征信息> 读取可选词条挂接(const 方法参数场景& 场景,std::uint32_t 位置) noexcept {
    auto 当前=场景.读取词条参数挂接(位置);
    using S=新方法词条参数读取状态;
    switch(当前.状态) {
    case S::已读取:
        if(当前.信息)return std::move(当前.信息);
        break;
    case S::参数已失效: case S::未声明: case S::参数未绑定: case S::信息类型不符: case S::能力未支持:
        return {};
    case S::依赖未就绪:
        if(场景.用途()!=新方法参数场景用途::执行)return {};
        break;
    case S::资源失败: break;
    }
    工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取词条绑定特征",L"词条读取资源、执行装配或成功载荷发生工程错误。");
}


void 核验有序参数绑定组(const std::vector<新方法参数角色绑定>& 绑定组) noexcept {
    const bool 含有序引用 = std::any_of(绑定组.begin(), 绑定组.end(),
        [](const 新方法参数角色绑定& 绑定) noexcept { return 绑定.有序引用.has_value(); });
    if (!含有序引用) return;
    for (std::size_t 位置 = 0; 位置 < 绑定组.size(); ++位置) {
        const auto& 绑定 = 绑定组[位置];
        if (!绑定.有序引用 || 绑定.有序引用->序号 != 位置 + 1
            || !有效(绑定.有序引用->信息节点)
            || (绑定.有序引用->信息类型 != 新方法参数信息类型::存在
                && 绑定.有序引用->信息类型 != 新方法参数信息类型::特征
                && 绑定.有序引用->信息类型 != 新方法参数信息类型::词条)
            || 绑定.参数角色节点.值 != 0 || 绑定.实际存在节点 || !绑定.实际特征组.empty()
            || 绑定.任务虚拟存在引用 || 绑定.服务要求分项引用 || 绑定.服务要求确认引用
            || (绑定.世界场景节点 && !有效(*绑定.世界场景节点)))
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"方法参数场景", L"有序参数绑定次序、类型或新旧格式不一致。");
    }
}

} // namespace

方法参数场景::方法参数场景(
    稳定编码 方法节点,
    std::vector<新方法参数角色绑定> 角色绑定组,
    const 新_特征类& 特征服务,
    const 新_特征值类& 特征值服务,
    新方法参数场景用途 用途,
    const 新内部治理对象读取接口* 治理对象读取,
    std::shared_ptr<新方法模拟运行接口> 模拟运行,
    const 新服务要求材料读取接口* 服务要求读取,
    std::optional<新任务贡献治理参数> 任务贡献治理参数,
    std::optional<新方法任务成员绑定> 任务成员绑定,
    std::optional<稳定编码> 执行分支,const 新_方法类* 方法读取,
    const 新_存在类* 存在读取,const 概念_特征类* 特征概念读取,
    std::shared_ptr<新本能需求运行接口> 需求运行,
    const 正式信息参数读取接口* 正式信息读取,
    std::shared_ptr<新本能字段询问运行接口> 字段询问运行,
    const 新_词类* 词条读取) noexcept
    : 方法节点_(方法节点),
      正式信息读取_(正式信息读取),
      方法读取_(方法读取),存在读取_(存在读取),特征概念读取_(特征概念读取),
      词条读取_(词条读取),
      角色绑定组_(std::move(角色绑定组)),
      特征服务_(&特征服务),
      特征值服务_(&特征值服务),
      用途_(用途),
      治理对象读取_(治理对象读取),
      模拟运行_(std::move(模拟运行)),
      需求运行_(std::move(需求运行)),
      字段询问运行_(std::move(字段询问运行)),
      服务要求读取_(服务要求读取),
      任务贡献治理参数_(std::move(任务贡献治理参数)),
      任务成员绑定_(std::move(任务成员绑定)),执行分支节点_(执行分支) {
    核验有序参数绑定组(角色绑定组_);
    if (字段询问运行_ && 模拟运行_ && 模拟运行_->获取字段询问运行接口()!=字段询问运行_.get())
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"方法参数场景",L"字段询问提供者与模拟owner不一致。");
    if(需求运行_ && 模拟运行_ && 模拟运行_->获取需求运行接口()!=需求运行_.get())
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"方法参数场景",L"需求操作提供者与场景owner不一致。");
}

std::optional<稳定编码> 方法参数场景::执行分支节点() const noexcept { return 执行分支节点_; }

新方法词条参数读取结果 方法参数场景::读取词条参数挂接(std::uint32_t 位置) const noexcept {
    using S = 新方法词条参数读取状态;
    try {
        if (用途_ == 新方法参数场景用途::登记) return {S::未声明, {}};
        const auto* 参数 = 查询参数(位置);
        if (!参数) return {S::参数未绑定, {}};
        if (参数->信息类型 != 新方法参数信息类型::词条) return {S::信息类型不符, {}};
        if (用途_ == 新方法参数场景用途::模拟) return {S::能力未支持, {}};
        if (用途_ != 新方法参数场景用途::执行) return {S::未声明, {}};
        if (!词条读取_) return {S::依赖未就绪, {}};
        auto 当前 = 词条读取_->读取词条特征信息(参数->信息节点);
        if (当前.状态 == 新词条查询状态::未找到) return {S::参数已失效, {}};
        if (当前.状态 == 新词条查询状态::资源失败) return {S::资源失败, {}};
        if (当前.状态 != 新词条查询状态::已找到 || !当前.信息)
            throw std::logic_error("word parameter read failed");
        return {S::已读取, std::move(当前.信息)};
    } catch (const std::bad_alloc&) { return {S::资源失败, {}}; }
    catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取词条参数", L"词条参数当前材料读取发生工程错误。");
    }
}

正式信息参数读取结果 方法参数场景::读取正式信息参数保护(std::uint32_t 位置) const noexcept {
    for (;;) {
        try {
            const auto* p=查询参数(位置);
            if(!p||p->信息类型!=新方法参数信息类型::存在||!方法读取_)
                throw std::logic_error("formal parameter is not bound or has an incompatible type");
            const auto m=方法读取_->获取方法(方法节点_);
            if(!m)return {正式信息参数读取状态::信息不存在,{},{}};
            if(位置==0||位置>m->参数规格组.size()||!m->参数规格组[位置-1].有序规格)
                return {正式信息参数读取状态::范围未支持,{},{}};
            const auto& spec=*m->参数规格组[位置-1].有序规格;
            if(spec.序号!=位置||spec.信息类型!=p->信息类型)
                return {正式信息参数读取状态::范围未支持,{},{}};
            const auto definition=方法读取_->读取模拟正式结构(spec.范围概念节点);
            if(definition.状态==正式结构操作状态::前提已变化)throw 参数材料需要重读{};
            if(definition.状态==正式结构操作状态::依赖未就绪)
                return {正式信息参数读取状态::依赖未就绪,{},{}};
            if(definition.状态==正式结构操作状态::未找到||definition.状态==正式结构操作状态::输入拒绝)return {};
            if(definition.状态!=正式结构操作状态::已读取||!definition.信息)
                throw std::logic_error("formal parameter range definition unavailable");
            if(!std::holds_alternative<信息结构概念定义>(definition.信息->定义))return {};
            正式信息参数读取结果 result;
            if(用途_==新方法参数场景用途::模拟) {
                if(!模拟运行_||!模拟运行_->核验模拟参数(方法节点_,角色绑定组_))
                    throw std::logic_error("formal parameter simulation owner differs");
                result=模拟运行_->读取正式信息参数保护(p->信息节点,spec.范围概念节点);
            } else if(用途_!=新方法参数场景用途::执行||!正式信息读取_) {
                return {正式信息参数读取状态::依赖未就绪,{},{}};
            } else result=正式信息读取_->读取正式信息参数保护(p->信息节点,spec.范围概念节点);
            if(result.状态==正式信息参数读取状态::前提已变化)throw 参数材料需要重读{};
            // reader交付本轮材料；不在上层回扫底座或比较整份旧方法、定义与内容快照。
            return result;
        } catch(const 参数材料需要重读&) {
        } catch(const std::bad_alloc&) {
            return {正式信息参数读取状态::资源失败,{},{}};
        } catch(...) {
            工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取正式信息参数",L"正式参数或方法声明读取发生工程错误。");
        }
        std::this_thread::yield();
    }
}

新方法参数特征读取结果 方法参数场景::读取存在参数特征(
    std::uint32_t 位置,稳定编码 FT) const noexcept {
    using S=新方法参数特征读取状态;
    for (;;) {
        try {
            if(用途_!=新方法参数场景用途::执行&&用途_!=新方法参数场景用途::模拟)
                return {S::未声明,{},{}};
            const auto* p=查询参数(位置);
            if(!p)return {S::参数未绑定,{},{}};
            if(p->信息类型!=新方法参数信息类型::存在 && p->信息类型!=新方法参数信息类型::词条)
                return {S::信息类型不符,{},{}};
            if(p->信息类型==新方法参数信息类型::词条 && 用途_==新方法参数场景用途::模拟)
                return {S::能力未支持,{},{}};
            if(!方法读取_||!特征概念读取_)throw std::logic_error("existence feature query definition owners unavailable");
            const auto 声明=方法读取_->读取存在参数特征声明(方法节点_,执行分支节点_,位置,FT);
            if(声明.状态==S::定义前提已变化)throw 参数材料需要重读{};
            if(声明.状态!=S::已读取)return {声明.状态,{},{}};
            if(!声明.特征定义||声明.特征定义->特征类型根!=FT)
                throw std::logic_error("existence feature declaration projection inconsistent");
            if(p->信息类型==新方法参数信息类型::词条) {
                const auto 当前=读取词条参数挂接(位置);
                using W=新方法词条参数读取状态;
                if(当前.状态==W::参数已失效)return {S::特征未取得,{},{}};
                if(当前.状态==W::资源失败)throw std::bad_alloc{};
                if(当前.状态!=W::已读取||!当前.信息||!特征服务_)
                    throw std::logic_error("word feature reader unavailable");
                const auto 成员 = std::find_if(当前.信息->特征组.begin(), 当前.信息->特征组.end(),
                    [&](const auto& 项) { return 项.特征类型根 == FT; });
                if (成员 == 当前.信息->特征组.end()) return {S::特征未取得,{},{}};
                const auto f=特征服务_->捕获特征匹配实例(成员->特征节点);
                if(!f) {
                    const auto 最新=读取词条参数挂接(位置);
                    if(最新.状态==W::参数已失效)return {S::特征未取得,{},{}};
                    if(最新.状态==W::资源失败)throw std::bad_alloc{};
                    if(最新.状态!=W::已读取||!最新.信息)
                        throw std::logic_error("word feature reader unavailable");
                    if(std::none_of(最新.信息->特征组.begin(), 最新.信息->特征组.end(),
                        [&](const auto& 项) { return 项 == *成员; }))
                        throw 参数材料需要重读{};
                    throw std::logic_error("word references an unavailable name feature");
                }
                if(f->定义.特征类型根!=FT)return {S::特征未取得,{},{}};
                新特征信息 信息;
                信息.节点=f->特征节点;信息.特征概念节点=f->定义.概念节点;
                信息.实例单位=f->实例单位;信息.当前值=f->当前值;
                return {f->当前值 ? S::已读取 : S::当前值未取得,std::move(信息),f->当前完整材料};
            }
            if(用途_==新方法参数场景用途::模拟) {
                if(!模拟运行_||!模拟运行_->核验模拟参数(方法节点_,角色绑定组_))
                    throw std::logic_error("simulation existence feature query owner differs");
                return 模拟运行_->读取模拟存在参数特征(位置,FT);
            }
            if(!存在读取_||!特征服务_||!特征值服务_)
                throw std::logic_error("existence feature instance owners unavailable");
            const auto E=存在读取_->获取存在(p->信息节点);
            if(!E)return {S::特征未取得,{},{}};
            std::vector<新特征匹配实例快照> 命中;
            for(const auto F:E->特征节点组) {
                auto f=特征服务_->捕获特征匹配实例(F);
                if(!f) {
                    const auto 最新=存在读取_->获取存在(p->信息节点);
                    if(!最新)return {S::特征未取得,{},{}};
                    if(std::find(最新->特征节点组.begin(),最新->特征节点组.end(),F)==最新->特征节点组.end())
                        throw 参数材料需要重读{};
                    throw std::logic_error("existence references an unavailable feature");
                }
                if(f->定义.特征类型根==FT)命中.push_back(std::move(*f));
            }
            新方法参数特征读取结果 out;
            out.状态=命中.empty() ? S::特征未取得 : S::特征匹配不唯一;
            if(命中.size()==1) {
                const auto& f=命中.front();
                新特征信息 投影;
                投影.节点=f.特征节点;投影.特征概念节点=f.定义.概念节点;
                投影.实例单位=f.实例单位;投影.当前值=f.当前值;
                out.特征=std::move(投影);out.当前完整材料=f.当前完整材料;
                out.状态=f.当前值 ? S::已读取 : S::当前值未取得;
            }
            // 当前值及材料来自本轮捕获；不再次逐特征核对，也不要求世界在读取后保持静止。
            return out;
        } catch(const 参数材料需要重读&) {
        } catch(...) {
            工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取存在参数特征",L"声明或所属特征当前材料发生工程读取错误。");
        }
        std::this_thread::yield();
    }
}

稳定编码 方法参数场景::方法节点() const noexcept {
    return 方法节点_;
}

新方法参数场景用途 方法参数场景::用途() const noexcept {
    return 用途_;
}

const std::optional<新任务贡献治理参数>& 方法参数场景::读取任务贡献治理参数() const noexcept {
    return 任务贡献治理参数_;
}

bool 方法参数场景::使用治理对象读取接口(const 新内部治理对象读取接口& 读取) const noexcept {
    return 治理对象读取_ == &读取;
}


const std::vector<新方法参数角色绑定>&
方法参数场景::全部角色绑定() const noexcept {
    return 角色绑定组_;
}

const 新方法参数角色绑定* 方法参数场景::查询角色(
    稳定编码 参数角色节点) const noexcept {
    for (const auto& 绑定 : 角色绑定组_) {
        if (!绑定.有序引用 && 绑定.参数角色节点 == 参数角色节点) return &绑定;
    }
    return nullptr;
}

const 新方法有序参数引用* 方法参数场景::查询参数(std::uint32_t 序号) const noexcept {
    if (序号 == 0 || 序号 > 角色绑定组_.size()) return nullptr;
    const auto& 绑定 = 角色绑定组_[序号 - 1];
    return 绑定.有序引用 ? &*绑定.有序引用 : nullptr;
}

std::optional<新任务虚拟存在读回> 方法参数场景::读取绑定任务虚拟存在(
    稳定编码 参数角色节点) const noexcept {
    const auto* 绑定 = 查询角色(参数角色节点);
    const 新内部治理对象读取接口* 读取 = 治理对象读取_;
    if (用途_ == 新方法参数场景用途::模拟) {
        if (!模拟运行_ || !模拟运行_->核验模拟参数(方法节点_, 角色绑定组_))
            return std::nullopt;
        读取 = 模拟运行_.get();
    }
    if ((用途_ != 新方法参数场景用途::执行 && 用途_ != 新方法参数场景用途::模拟) || !读取
        || !绑定 || !绑定->任务虚拟存在引用
        || 绑定->服务要求分项引用 || 绑定->服务要求确认引用
        || 绑定->世界场景节点 || 绑定->实际存在节点
        || 绑定->实际特征组.empty()) return std::nullopt;
    if (绑定->实际特征组.size() != 1) {
        const auto 组 = 读取绑定任务特征组(参数角色节点);
        if (!组) return {};
        return 新任务虚拟存在读回{组->引用, 组->状态.特征类型概念节点, 组->状态.当前值};
    }
    if (绑定->实际特征组.front().特征节点 != 绑定->任务虚拟存在引用->状态特征节点) return {};
    const auto 读回 = 读取->读取任务虚拟存在引用(
        *绑定->任务虚拟存在引用);
    return 读回 && 读回->引用 == *绑定->任务虚拟存在引用
        ? 读回 : std::nullopt;
}

std::optional<新任务虚拟存在特征组> 方法参数场景::读取绑定任务特征组(
    稳定编码 参数角色节点) const noexcept {
    const auto* 绑定 = 查询角色(参数角色节点);
    const 新内部治理对象读取接口* 读取 = 治理对象读取_;
    if (用途_ == 新方法参数场景用途::模拟) {
        if (!模拟运行_ || !模拟运行_->核验模拟参数(方法节点_, 角色绑定组_)) return {};
        读取 = 模拟运行_.get();
    }
    if ((用途_ != 新方法参数场景用途::执行 && 用途_ != 新方法参数场景用途::模拟)
        || !读取 || !绑定 || !绑定->任务虚拟存在引用 || 绑定->世界场景节点
        || 绑定->实际存在节点 || 绑定->服务要求分项引用 || 绑定->服务要求确认引用
        || 绑定->实际特征组.empty() || 绑定->实际特征组.size() > 2) return {};
    const auto 组 = 读取->读取任务虚拟存在特征组(*绑定->任务虚拟存在引用);
    if (!组 || 组->引用 != *绑定->任务虚拟存在引用
        || 组->状态.特征节点 != 组->引用.状态特征节点) return {};
    std::size_t 状态数 = 0, Q数 = 0;
    for (const auto& f : 绑定->实际特征组) {
        if (f.特征节点 == 组->状态.特征节点) ++状态数;
        else if (组->服务值 && f.特征节点 == 组->服务值->特征节点) ++Q数;
        else return {};
    }
    if (状态数 != 1 || Q数 > 1 || (绑定->实际特征组.size() == 2 && Q数 != 1)) return {};
    return 组;
}

std::optional<新任务虚拟存在特征组> 方法参数场景::读取绑定集合任务特征组(
    std::uint32_t 位置,稳定编码 T) const noexcept {
    if (任务成员绑定_ && (任务成员绑定_->集合参数序号 != 位置
        || 任务成员绑定_->任务引用.任务节点 != T)) return {};
    const auto* p=查询参数(位置);
    if(!p||p->信息类型!=新方法参数信息类型::特征)return {};
    const auto f=读取绑定特征(p->信息节点);
    if(!f||!f->当前值||!特征服务_)return {};
    const auto d=特征服务_->捕获特征匹配定义(f->特征概念节点);
    if(!d||d->比较规则!=特征概念比较规则::服务任务集合身份判等)return {};
    const auto* V=std::get_if<稳定编码>(&*f->当前值);
    const auto v=V?读取绑定特征值材料(*V):std::nullopt;
    const auto* a=v?std::get_if<std::vector<std::uint64_t>>(&v->材料):nullptr;
    if(!a||std::find(a->begin(),a->end(),T.值)==a->end())return {};
    const 新内部治理对象读取接口* owner=nullptr;
    if(用途_==新方法参数场景用途::模拟) {
        if(!模拟运行_||!模拟运行_->核验模拟参数(方法节点_,角色绑定组_))return {};
        owner=模拟运行_.get();
    }
    else if(用途_==新方法参数场景用途::执行)owner=治理对象读取_;
    if(!owner)return {};
    if (用途_ == 新方法参数场景用途::模拟) {
        const auto result = 模拟运行_->读取模拟集合任务治理(位置, T);
        if (!result || result->引用.任务节点 != T) return {};
        if (任务成员绑定_ && (result->引用 != 任务成员绑定_->任务引用
            || 任务成员绑定_->服务值特征节点 != (result->服务值
                ? std::optional<稳定编码>{result->服务值->特征节点} : std::nullopt))) return {};
        return result;
    }
    const auto ref=owner->查找任务虚拟存在引用(T);
    if(!ref||ref->任务节点!=T)return {};
    const auto result=owner->读取任务虚拟存在特征组(*ref);
    if (!result || result->引用 != *ref || result->状态.特征节点 != ref->状态特征节点) return {};
    if (任务成员绑定_ && (任务成员绑定_->任务引用 != *ref
        || 任务成员绑定_->服务值特征节点 != (result->服务值
            ? std::optional<稳定编码>{result->服务值->特征节点} : std::nullopt))) return {};
    return result;
}

std::optional<新方法任务成员绑定> 方法参数场景::读取绑定任务成员(
    std::uint32_t 变量序号) const noexcept {
    return 变量序号 == 1 ? 任务成员绑定_ : std::nullopt;
}

bool 方法参数场景::使用服务要求读取接口(
    const 新服务要求材料读取接口& 读取) const noexcept {
    return 用途_ == 新方法参数场景用途::执行 && 服务要求读取_ == &读取;
}

新服务要求分项读取结果 方法参数场景::读取绑定服务要求分项(
    稳定编码 参数角色节点) const noexcept {
    using S = 新服务要求材料读取状态;
    const auto* 绑定 = 查询角色(参数角色节点);
    if (用途_ != 新方法参数场景用途::执行 || !服务要求读取_)
        return {S::工程故障, {}};
    if (!绑定 || !绑定->服务要求分项引用) return {S::材料不存在, {}};
    if (绑定->服务要求确认引用 || 绑定->任务虚拟存在引用
        || 绑定->世界场景节点 || 绑定->实际存在节点 || !绑定->实际特征组.empty())
        return {S::工程故障, {}};
    const auto& 引用 = *绑定->服务要求分项引用;
    const auto 读回 = 服务要求读取_->读取服务要求分项(引用);
    if (读回.状态 != S::已读取) return 读回;
    if (!读回.材料 || 读回.材料->引用 != 引用) return {S::工程故障, {}};
    switch (引用.分项) {
    case 新服务要求分项种类::来源人类:
    case 新服务要求分项种类::目标存在:
    case 新服务要求分项种类::目标特征类型: {
        const auto* 编码 = std::get_if<稳定编码>(&读回.材料->值);
        if (!编码 || !有效(*编码)) return {S::工程故障, {}};
        break;
    }
    case 新服务要求分项种类::目标定义:
        if (!std::holds_alternative<新服务要求目标分项>(读回.材料->值))
            return {S::工程故障, {}};
        break;
    default: return {S::工程故障, {}};
    }
    return 读回;
}

新服务要求确认读取结果 方法参数场景::读取绑定服务要求确认身份(
    稳定编码 参数角色节点) const noexcept {
    using S = 新服务要求确认状态;
    const auto* 绑定 = 查询角色(参数角色节点);
    if (用途_ != 新方法参数场景用途::执行 || !服务要求读取_)
        return {S::未装配, {}};
    if (!绑定 || !绑定->服务要求确认引用) return {S::确认不存在, {}};
    if (绑定->服务要求分项引用 || 绑定->任务虚拟存在引用
        || 绑定->世界场景节点 || 绑定->实际存在节点 || !绑定->实际特征组.empty())
        return {S::结构不一致, {}};
    const auto 读回 = 服务要求读取_->读取服务要求确认身份(绑定->服务要求确认引用->确认节点);
    if (读回.状态 == S::已读取 && (!读回.确认
        || 读回.确认->确认节点 != 绑定->服务要求确认引用->确认节点))
        return {S::结构不一致, {}};
    return 读回;
}

新服务要求确认读取结果 方法参数场景::复核绑定服务要求确认语义(
    稳定编码 参数角色节点) const noexcept {
    using S = 新服务要求确认状态;
    const auto 身份 = 读取绑定服务要求确认身份(参数角色节点);
    if (身份.状态 != S::已读取) return 身份;
    const auto 读回 = 服务要求读取_->复核服务要求确认语义(身份.确认->确认节点);
    if ((读回.状态 == S::当前有效 || 读回.状态 == S::需重新确认)
        && (!读回.确认 || *读回.确认 != *身份.确认))
        return {S::结构不一致, {}};
    return 读回;
}

std::optional<新特征信息> 方法参数场景::读取绑定特征(
    稳定编码 特征节点) const noexcept {
    if (!有效(特征节点)) return std::nullopt;
    if (用途_ == 新方法参数场景用途::模拟) {
        if (!模拟运行_ || !模拟运行_->核验模拟参数(方法节点_, 角色绑定组_))
            return std::nullopt;
    } else if (!特征服务_) return std::nullopt;
    for (const auto& 角色 : 角色绑定组_) {
        if (角色.有序引用) {
            if(角色.有序引用->信息类型==新方法参数信息类型::词条) {
                const auto 当前=读取可选词条挂接(*this,角色.有序引用->序号);
                if(!当前)continue;
                const auto 成员 = std::find_if(当前->特征组.begin(), 当前->特征组.end(),
                    [&](const auto& 项) { return 项.特征节点 == 特征节点; });
                if (成员 == 当前->特征组.end()) continue;
                return 读取存在参数特征(角色.有序引用->序号,成员->特征类型根).特征;
            }
            if (角色.有序引用->信息类型 != 新方法参数信息类型::特征
                || 角色.有序引用->信息节点 != 特征节点) continue;
            return 用途_ == 新方法参数场景用途::模拟
                ? 模拟运行_->读取模拟特征(特征节点)
                : 特征服务_->获取特征(特征节点);
        }
        for (const auto& 特征 : 角色.实际特征组) {
            if (特征.特征节点 == 特征节点) {
                if (角色.任务虚拟存在引用
                    && !读取绑定任务虚拟存在(角色.参数角色节点)) {
                    return std::nullopt;
                }
                return 用途_ == 新方法参数场景用途::模拟
                    ? 模拟运行_->读取模拟特征(特征节点)
                    : 特征服务_->获取特征(特征节点);
            }
        }
    }
    return std::nullopt;
}

std::optional<新特征值信息> 方法参数场景::读取绑定特征值材料(
    稳定编码 特征值节点) const noexcept {
    if (!有效(特征值节点)) return std::nullopt;
    if (用途_ == 新方法参数场景用途::模拟) {
        if (!模拟运行_ || !模拟运行_->核验模拟参数(方法节点_, 角色绑定组_))
            return std::nullopt;
    } else if (!特征值服务_) return std::nullopt;
    for (const auto& 角色 : 角色绑定组_) {
        if(角色.有序引用&&角色.有序引用->信息类型==新方法参数信息类型::词条) {
            const auto 当前=读取可选词条挂接(*this,角色.有序引用->序号);
            if(!当前)continue;
            for (const auto& 成员 : 当前->特征组) {
                const auto 特征=读取存在参数特征(角色.有序引用->序号,成员.特征类型根);
                if(特征.当前完整材料&&特征.特征&&特征.特征->当前值) {
                    const auto* V=std::get_if<稳定编码>(&*特征.特征->当前值);
                    if(V&&*V==特征值节点)return 特征.当前完整材料;
                }
            }
            continue;
        }
        if (角色.有序引用 && 角色.有序引用->信息类型 != 新方法参数信息类型::特征) continue;
        const std::size_t 特征数 = 角色.有序引用 ? 1 : 角色.实际特征组.size();
        for (std::size_t 位置 = 0; 位置 < 特征数; ++位置) {
            const auto 特征节点 = 角色.有序引用
                ? 角色.有序引用->信息节点 : 角色.实际特征组[位置].特征节点;
            const auto 特征 = 读取绑定特征(特征节点);
            if (!特征 || !特征->当前值) continue;
            const auto* 材料节点 = std::get_if<稳定编码>(&*特征->当前值);
            if (材料节点 && *材料节点 == 特征值节点) {
                return 用途_ == 新方法参数场景用途::模拟
                    ? 模拟运行_->读取模拟特征值材料(特征值节点)
                    : 特征值服务_->获取特征值(特征值节点);
            }
        }
    }
    return std::nullopt;
}

} // namespace 海中鱼巣

namespace 海中鱼巣 {
新方法模拟运行接口* 方法参数场景::获取模拟运行接口() const noexcept {
    return 用途_==新方法参数场景用途::模拟 && 模拟运行_
        && 模拟运行_->核验模拟参数(方法节点_,角色绑定组_) ? 模拟运行_.get() : nullptr;
}
}
