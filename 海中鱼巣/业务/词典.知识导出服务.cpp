#include "词典.知识导出服务.h"
#include "../领域/新_本能函数集.h"
#include "../领域/概念_存在类.h"
#include "../领域/概念_特征类.h"
#include "../领域/新_特征值类.h"
#include "../领域/新_方法类.h"
#include "../领域/信息内容.数据.h"
#include "../领域/本能交互.运行接口.h"

#include <algorithm>
#include <array>
#include <limits>
#include <new>
#include <stdexcept>
#include <thread>
#include <variant>

namespace 海中鱼巣 {
namespace {
void 要求词典执行证据(bool 成立, const char* 诊断) {
    if (!成立) throw std::logic_error(诊断);
}
稳定编码 读取词典特征类型(概念_特征类& 概念, std::uint32_t 槽) {
    const auto r = 概念.读取当前先天特征类型槽(槽);
    if (r.状态 == 特征概念规则状态::资源失败) throw std::bad_alloc{};
    要求词典执行证据(r.状态 == 特征概念规则状态::已完成 && r.类型根.has_value(),
        "dictionary knowledge feature type unavailable");
    return *r.类型根;
}
}

新词典知识导出结果 导出词典条件结果(
    const 新方法条件定义& 基础条件, const 新本能函数本次条件& 动作前条件,
    const 新词典本能输入& 输入, 稳定编码 正式W,
    const 新本能词典读取回执& 终态, 概念_存在类& 存在概念,
    概念_特征类& 特征概念, const 新_特征值类& 值) {
    // 下列判断仅关联同一次执行的值式证据，不证明后来世界仍未变化。
    要求词典执行证据(终态.查阅结局 == 新词典查阅结局::命中
        && 终态.采用阶段 == 新词典材料采用阶段::已采用 && 终态.材料Q
        && 有效(终态.动作) && 终态.词条 == 输入.词条 && 终态.材料特征 == 输入.材料特征
        && 终态.变化阶段 && *终态.变化阶段 != 新词典变化处理阶段::待处理,
        "dictionary knowledge requires an adopted terminal receipt of this invocation");
    要求词典执行证据(!输入.批次摘要.empty() && 输入.外设场景标识 > 0
        && 输入.外设场景标识 <= static_cast<std::uint64_t>((std::numeric_limits<std::int64_t>::max)()),
        "dictionary knowledge source input malformed");
    const std::array 节点{输入.词条, 输入.材料特征, 输入.来源存在,
        输入.执行主体, 输入.批次特征, 输入.定位特征};
    const std::array 类型{新方法参数信息类型::词条, 新方法参数信息类型::特征,
        新方法参数信息类型::存在, 新方法参数信息类型::存在,
        新方法参数信息类型::特征, 新方法参数信息类型::特征};
    要求词典执行证据(动作前条件.参数角色组.size() == 节点.size(),
        "dictionary knowledge requires six actual parameters");
    for (std::size_t i = 0; i < 节点.size(); ++i) {
        const auto matches = std::count_if(动作前条件.参数角色组.begin(), 动作前条件.参数角色组.end(),
            [&](const auto& row) { return row.有序引用 && row.有序引用->序号 == i + 1
                && row.有序引用->信息类型 == 类型[i] && row.有序引用->信息节点 == 节点[i]; });
        要求词典执行证据(matches == 1, "dictionary knowledge actual parameter identity mismatch");
    }

    const auto 同键 = 存在概念.建立或取得词条同键范围(正式W);
    if (同键.状态 == 新词条同键操作状态::未找到)
        return {新词典知识导出状态::正式来源已退出, {}};
    if (同键.状态 == 新词条同键操作状态::资源失败) throw std::bad_alloc{};
    要求词典执行证据((同键.状态 == 新词条同键操作状态::已建立
        || 同键.状态 == 新词条同键操作状态::已复用) && 同键.信息,
        "dictionary knowledge same-key concept unavailable");
    要求词典执行证据(同键.信息->词条语言 == 输入.键.词条语言
        && 同键.信息->规范化合同 == 输入.键.规范化合同
        && 同键.信息->原规范化词面 == 输入.键.原规范化词面,
        "dictionary knowledge formal word differs from actual immutable query key");
    const auto 材料FT = 读取词典特征类型(特征概念, 先天特征类型槽::词典材料);
    const auto 批次FT = 读取词典特征类型(特征概念, 先天特征类型槽::词典来源批次);

    // 原登记合同在位置3只声明批次与来源标识两种F；不借本地F反查正式owner。
    std::optional<稳定编码> 定位FT;
    for (const auto& c : 基础条件.条件项组) {
        if (c.参数序号 != 3 || !c.特征类型概念节点 || *c.特征类型概念节点 == 批次FT) continue;
        要求词典执行证据(!定位FT || *定位FT == *c.特征类型概念节点,
            "dictionary knowledge source declaration is ambiguous");
        定位FT = c.特征类型概念节点;
    }
    要求词典执行证据(定位FT.has_value(), "dictionary knowledge source declaration missing");
    // 模拟动作前V可能是localV；用本次固定正文定位原本已存在的正式共享V。
    const auto 批次值 = 值.查询特征值(材料物理类型::UTF8字符串, 输入.批次摘要);
    要求词典执行证据(批次值.size() == 1, "dictionary knowledge shared batch material unavailable");
    稳定编码 批次范围;
    for (;;) {
        const auto 批次根 = 特征概念.获取特征概念(批次FT);
        要求词典执行证据(批次根.has_value(), "dictionary knowledge batch type unavailable");
        特征概念定义 批次域;
        批次域.材料类型 = 批次根->材料类型; 批次域.单位 = 批次根->单位;
        批次域.比较规则 = 批次根->比较规则; 批次域.聚合规则 = 批次根->聚合规则;
        批次域.值域 = 批次值;
        const auto 批次C = 特征概念.建立或取得特征概念强类型(批次域, 值, 批次FT);
        if (批次C.状态 == 特征概念规则状态::前提已变化) {
            std::this_thread::yield(); continue;
        }
        if (批次C.状态 == 特征概念规则状态::资源失败) throw std::bad_alloc{};
        要求词典执行证据((批次C.状态 == 特征概念规则状态::已完成
            || 批次C.状态 == 特征概念规则状态::无变化) && 批次C.概念节点,
            "dictionary knowledge exact batch domain unavailable");
        批次范围 = *批次C.概念节点;
        break;
    }
    const auto Q范围 = 存在概念.读取词典材料身份范围概念();
    const auto 材料根 = 特征概念.获取特征概念(材料FT);
    要求词典执行证据(Q范围.状态 == 正式结构操作状态::已读取 && Q范围.信息 && 材料根,
        "dictionary knowledge material identity domain unavailable");
    const auto* Q域 = std::get_if<特征概念信息引用值域>(&材料根->值域);
    要求词典执行证据(Q域 && Q域->语义 == 信息引用语义::信息对象
        && Q域->内容概念 == Q范围.信息->节点, "dictionary knowledge material domain is not Q");

    新方法条件结果定义 pair;
    pair.条件 = 基础条件;
    要求词典执行证据(pair.条件.关系规则 == 新方法项关系规则::全部成立
        || pair.条件.关系规则 == 新方法项关系规则::单项,
        "dictionary knowledge requires a conjunctive execution branch");
    pair.条件.关系规则 = 新方法项关系规则::全部成立;
    bool 已收窄W = false;
    for (auto& c : pair.条件.条件项组) {
        if (c.参数序号 == 1 && c.存在概念节点) {
            c.存在概念节点 = 同键.信息->概念节点; 已收窄W = true;
        }
    }
    if (!已收窄W) {
        新方法条件项定义 c; c.参数序号 = 1; c.存在概念节点 = 同键.信息->概念节点;
        pair.条件.条件项组.push_back(std::move(c));
    }
    新方法条件项定义 batch;
    batch.参数序号 = 5; batch.特征类型概念节点 = 批次FT; batch.特征值域概念节点 = 批次范围;
    if (std::find(pair.条件.条件项组.begin(), pair.条件.条件项组.end(), batch) == pair.条件.条件项组.end())
        pair.条件.条件项组.push_back(std::move(batch));
    新方法条件项定义 location;
    location.参数序号 = 6; location.特征类型概念节点 = 定位FT;
    const auto number = static_cast<std::int64_t>(输入.外设场景标识);
    location.精确数值域 = 特征概念I64闭区间{number, number};
    if (std::find(pair.条件.条件项组.begin(), pair.条件.条件项组.end(), location) == pair.条件.条件项组.end())
        pair.条件.条件项组.push_back(std::move(location));
    新方法结果项定义 result;
    result.参数序号 = 2; result.特征类型概念节点 = 材料FT; result.特征值域概念节点 = 材料FT;
    pair.结果.结果项组.push_back(std::move(result));
    if (动作前条件.执行分支节点) pair.来源执行分支节点组.push_back(*动作前条件.执行分支节点);
    return {新词典知识导出状态::已形成, std::move(pair)};
}
static std::optional<新归类概念知识片段> 构造归类概念知识片段(
    const 新本能词条归类回执& 回执,const std::vector<正式信息参数投影>& 本次材料读取组,
    const std::vector<稳定编码>& 本次观察概念组,
    const 新词典归类信息概念组& 范围,稳定编码 挂接特征类型,
    概念_存在类& 存在概念,新_方法类& 方法,
    std::span<const 正式概念身份读取接口* const> 提供者组) {
    const auto C=回执.实际输入.采用概念;
    // 此处检查的是同次已发生动作与其观察结果，不比较当前世界与旧快照。
    要求词典执行证据(有效(回执.动作) && 有效(C) && 有效(回执.实际输入.词条)
        && 有效(回执.实际输入.挂接特征) && 有效(回执.实际输入.归类材料)
        && 有效(范围.目标词条FT) && 有效(范围.采用概念FT) && 有效(挂接特征类型),
        "classification knowledge receipt incomplete");
    if(本次材料读取组.empty())return {};
    bool 已找到实际输入=false;
    for(const auto& 本次材料读取:本次材料读取组) {
        if(本次材料读取.信息节点!=回执.实际输入.归类材料)continue;
        const auto 读取引用=[&](稳定编码 FT,正式内容值种类 种类)->std::optional<稳定编码> {
            const 正式直接内容项* found=nullptr;
            for(const auto& item:本次材料读取.直接内容组)if(item.特征类型==FT) {
                if(found)return {};found=&item;
            }
            if(!found || found->内容.种类!=种类 || found->内容.状态!=正式内容值状态::已知
                || !found->内容.值)return {};
            const auto* value=std::get_if<稳定编码>(&*found->内容.值);
            return value ? std::optional{*value} : std::nullopt;
        };
        // 每次提交前读取都属于本次证据；只要有一次不能由同一参数关系表达，就不形成片段。
        if(读取引用(范围.目标词条FT,正式内容值种类::词条引用)!=std::optional{回执.实际输入.词条}
            || 读取引用(范围.采用概念FT,正式内容值种类::概念引用)!=std::optional{C})return {};
        已找到实际输入=true;
    }
    if(!已找到实际输入)return {};
    if(std::find(本次观察概念组.begin(),本次观察概念组.end(),C)==本次观察概念组.end())
        return {};
    const auto 单点=存在概念.建立或取得正式概念单点范围(C,提供者组);
    if(单点.状态==正式结构操作状态::输入拒绝 || 单点.状态==正式结构操作状态::未找到)return {};
    要求词典执行证据((单点.状态==正式结构操作状态::已建立 || 单点.状态==正式结构操作状态::已复用)
        && 单点.信息,"classification formal concept singleton unavailable");
    const auto 域=方法.保存模拟概念引用值域(挂接特征类型,单点.信息->节点);
    新归类概念知识片段 片段;
    片段.采用概念条件.参数序号=3;
    片段.采用概念条件.引用内容条件=新方法引用内容条件{{范围.采用概念FT},单点.信息->节点,
        新方法内容作用点::参数自身,false};
    片段.目标词条关系={新方法参数关系种类::内容引用参数,3,1,{范围.目标词条FT}};
    片段.挂接结果.参数序号=2;
    片段.挂接结果.特征类型概念节点=挂接特征类型;
    片段.挂接结果.特征值域概念节点=域;
    片段.挂接结果.取值位置=特征取值位置::历史不同值;
    return 片段;
}
std::vector<新归类有序读取> 整理归类读取顺序(const 新隔离调用证据& 证据) {
    std::vector<新归类有序读取> 读取流;
    读取流.reserve(证据.归类材料读取组.size()+证据.归类特征读取组.size()
        +证据.归类值读取组.size()+证据.归类其他读取组.size());
    for(const auto& 读取:证据.归类材料读取组)读取流.push_back({读取.读取序号,&读取});
    for(const auto& 读取:证据.归类特征读取组)读取流.push_back({读取.读取序号,&读取});
    for(const auto& 读取:证据.归类值读取组)读取流.push_back({读取.读取序号,&读取});
    for(const auto& 读取:证据.归类其他读取组)读取流.push_back({读取.读取序号,&读取});
    std::sort(读取流.begin(),读取流.end(),[](const auto& a,const auto& b){return a.序号<b.序号;});
    return 读取流;
}
新归类正式材料结果 取得归类正式概念材料(const 新特征值信息& 材料,
    std::span<const 稳定编码> 本地身份,std::span<const 正式概念身份读取接口* const> 提供者组,
    const std::function<bool()>& 停止检查) {
    using S=新归类正式材料状态;
    const auto* 概念组=std::get_if<std::vector<std::uint64_t>>(&材料.材料);
    if(材料.物理类型!=材料物理类型::U64数组 || !概念组)return {S::材料不适用,{}};
    for(const auto raw:*概念组) {
        const 稳定编码 C{raw};
        if(std::find(本地身份.begin(),本地身份.end(),C)!=本地身份.end())return {S::含本地身份,{}};
        for(;;) {
            if(停止检查 && 停止检查())return {S::已停止,{}};
            bool 找到=false,需重读=false;
            for(const auto* 提供者:提供者组) {
                if(!提供者)continue;
                const auto 身份=提供者->读取正式概念身份保护(C);
                if(身份.状态==正式概念读取状态::前提已变化) {需重读=true;break;}
                if(身份.状态==正式概念读取状态::已读取 && 身份.身份 && 身份.身份->概念节点==C) {
                    找到=true;break;
                }
            }
            if(需重读) {std::this_thread::yield();continue;}
            if(!找到)return {S::正式身份未取得,{}};
            break;
        }
    }
    return {S::已取得,*概念组};
}
新归类正式名称读取结果 转换归类名称读取(const 新隔离调用证据& 证据,
    std::span<const 稳定编码> 本地身份,std::span<const 正式概念身份读取接口* const> 提供者组,
    const std::function<bool()>& 停止检查) {
    using S=新归类正式材料状态;
    if(证据.已请求取消)return {S::已停止,{}};
    if(!证据.词条归类)return {S::材料不适用,{}};
    新归类正式名称读取结果 结果;结果.状态=S::已取得;
    for(const auto& 读取:证据.归类名称读取组) {
        if(停止检查 && 停止检查())return {S::已停止,{}};
        if(读取.词条!=证据.词条归类->实际输入.词条)continue;
        const auto 转换组=[&](const std::vector<稳定编码>& 概念组) {
            std::vector<std::uint64_t> 值;值.reserve(概念组.size());
            for(const auto 概念:概念组)值.push_back(概念.值);
            return 取得归类正式概念材料(新特征值信息{{},材料物理类型::U64数组,std::move(值)},
                本地身份,提供者组,停止检查);
        };
        auto 普通=转换组(读取.普通名称概念组);
        if(普通.状态!=S::已取得)return {普通.状态,{}};
        auto 全部=转换组(读取.全部名称概念组);
        if(全部.状态!=S::已取得)return {全部.状态,{}};
        结果.读取组.push_back({std::move(普通.概念组),std::move(全部.概念组),读取.前序内容读取数,
            读取.动作前,读取.归类结果已提交});
    }
    return 结果;
}
std::optional<新归类概念知识片段> 从归类关闭证据构造概念片段(
    const 新隔离调用证据& 证据,const 新词典归类信息概念组& 范围,稳定编码 挂接特征类型,
    概念_存在类& 存在概念,新_方法类& 方法,
    std::span<const 正式概念身份读取接口* const> 提供者组) {
    if(!证据.词条归类 || 证据.已请求取消)return {};
    const auto& 回执=*证据.词条归类;
    const auto 读取流=整理归类读取顺序(证据);
    std::vector<正式信息参数投影> 材料读取;
    std::optional<std::size_t> 最后特征位置;
    for(std::size_t i=0;i<读取流.size();++i) {
        const auto& 项=读取流[i];
        if(const auto p=std::get_if<const 新隔离归类材料读取*>(&项.读取)) {
            const auto& 读取=**p;
            // 本片只抽象动作提交前实际使用的归类材料；提交后的读取留作观察。
            if(读取.归类结果已提交)continue;
            // 同次调用可读取其他参数；这里只收集实际归类材料参数。
            if(读取.内容.信息节点!=回执.实际输入.归类材料
                || std::find(读取.参数位置组.begin(),读取.参数位置组.end(),3)==读取.参数位置组.end())continue;
            材料读取.push_back(读取.内容);
        } else if(const auto p=std::get_if<const 新隔离归类特征读取*>(&项.读取)) {
            const auto& 读取=**p;
            if(读取.归类结果已提交 && 读取.内容.节点==回执.实际输入.挂接特征)最后特征位置=i;
        }
        // 其他投影由完整条件导出处理，本函数只构造概念片段。
    }
    // 节点编码只定位材料；必须有该次特征读取之后取得的材料内容。
    if(!最后特征位置)return {};
    const auto* 观察=std::get_if<const 新隔离归类特征读取*>(&读取流[*最后特征位置].读取);
    if(!观察 || !*观察)return {};
    const auto C=回执.实际输入.采用概念;
    const auto& 历史=(*观察)->内容.历史不同值;
    if(std::find(历史.begin(),历史.end(),新特征准确值{C})==历史.end())return {};
    const std::vector<稳定编码> 概念组{C};
    return 构造归类概念知识片段(回执,材料读取,概念组,范围,挂接特征类型,
        存在概念,方法,提供者组);
}
std::optional<新方法条件结果定义> 构造归类学习候选(
    const 新方法初始必要条件定义& 必要条件,const 新本能函数本次条件& 实际条件,
    const 新隔离调用证据& 证据,std::span<const 新方法初始场景事实限定> 本次场景事实,
    const 新词典归类信息概念组& 范围,稳定编码 挂接特征类型,
    概念_存在类& 存在概念,新_方法类& 方法,
    std::span<const 正式概念身份读取接口* const> 提供者组) {
    if(!证据.词条归类 || 证据.已请求取消 || !证据.身份.主动作
        || *证据.身份.主动作!=证据.词条归类->动作)return {};
    const auto& 输入=证据.词条归类->实际输入;
    const std::array 节点{输入.词条,输入.挂接特征,输入.归类材料,输入.执行主体};
    const std::array 类型{新方法参数信息类型::词条,新方法参数信息类型::特征,
        新方法参数信息类型::存在,新方法参数信息类型::存在};
    for(std::size_t i=0;i<节点.size();++i) {
        const auto 找到=std::find_if(实际条件.参数角色组.begin(),实际条件.参数角色组.end(),[&](const auto& row) {
            return row.有序引用 && row.有序引用->序号==i+1
                && row.有序引用->信息类型==类型[i] && row.有序引用->信息节点==节点[i];
        });
        if(找到==实际条件.参数角色组.end())return {};
    }
    const auto 已观察=[&](const auto& fact) {
        return std::find(本次场景事实.begin(),本次场景事实.end(),fact)!=本次场景事实.end();
    };
    for(const auto& fact:必要条件.场景事实限定组)if(!已观察(fact))return {};
    for(const auto& fact:必要条件.条件.场景事实限定组)if(!已观察(fact))return {};
    if(必要条件.条件.关系规则!=新方法项关系规则::全部成立
        && 必要条件.条件.关系规则!=新方法项关系规则::单项)return {};
    const auto 片段=从归类关闭证据构造概念片段(证据,范围,挂接特征类型,存在概念,方法,提供者组);
    if(!片段)return {};
    新方法条件结果定义 候选;
    候选.条件=必要条件.条件;候选.条件.关系规则=新方法项关系规则::全部成立;
    const auto 添加唯一=[](auto& group,const auto& item) {
        if(std::find(group.begin(),group.end(),item)==group.end())group.push_back(item);
    };
    for(const auto& fact:必要条件.场景事实限定组)添加唯一(候选.条件.场景事实限定组,fact);
    添加唯一(候选.条件.条件项组,片段->采用概念条件);
    添加唯一(候选.条件.参数关系组,片段->目标词条关系);
    候选.结果.关系规则=新方法项关系规则::全部成立;
    候选.结果.结果项组.push_back(片段->挂接结果);
    if(实际条件.执行分支节点)候选.来源执行分支节点组.push_back(*实际条件.执行分支节点);
    return 候选;
}
} // namespace 海中鱼巣
