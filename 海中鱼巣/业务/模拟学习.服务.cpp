#include "模拟学习.服务.h"
#include "模拟学习.算法.h"
#include "模拟任务需求来源.服务.h"
#include "../领域/新_模拟世界类.h"
#include "../领域/新_方法类.h"
#include "../领域/新_本能函数管理类.h"
#include "../领域/概念_特征类.h"
#include "../领域/新_存在类.h"
#include "../核心/容错检查.h"
#include <algorithm>
#include <limits>
#include <memory>
#include <map>
#include <new>
#include <thread>

namespace 海中鱼巣 {
新本能函数本次条件结果对 新_模拟世界类::执行贡献治理场景(const 方法参数场景&) noexcept {
    工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"模拟贡献治理",
        L"贡献治理尚无隔离模拟合同，禁止访问真实任务或治理世界。");
}
新本能函数本次条件结果对 新_模拟世界类::消费最终来源(const 方法参数场景&) noexcept {
    工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"模拟最终贡献消费",
        L"Q结算尚无模拟合同，禁止进入真实消费或借用旧加一样例。");
}
模拟学习服务::模拟学习服务(新_方法类& 方法, 概念_特征类& 概念,
    const 新服务值结算本能端口& 登记,const 模拟任务需求来源读取接口& 来源) noexcept
    : 方法_(方法), 概念_(概念), 登记读取_(登记),业务来源_(来源) {}

新学习运行异常::新学习运行异常(稳定编码 方法节点, 新学习运行异常原因 原因) noexcept
    : 记录_{方法节点,原因} {}
新学习运行异常::新学习运行异常(稳定编码 方法节点, 新学习运行异常原因 原因, std::string 诊断)
    : 记录_{方法节点, 原因, std::move(诊断)} {}
const 新学习运行异常记录& 新学习运行异常::记录() const noexcept { return 记录_; }
const char* 新学习运行异常::what() const noexcept {
    if (!记录_.诊断.empty()) return 记录_.诊断.c_str();
    switch (记录_.原因) {
    case 新学习运行异常原因::规格未支持: return "unsupported simulation specification";
    case 新学习运行异常原因::停止取消: return "learning operation cancelled";
    case 新学习运行异常原因::输入不合法: return "invalid learning input";
    case 新学习运行异常原因::资源不足: return "learning resource exhausted";
    case 新学习运行异常原因::结构不一致: return "learning structure inconsistent";
    case 新学习运行异常原因::本能运行异常: return "instinct execution abnormal";
    case 新学习运行异常原因::发布结果未知: return "learning publication indeterminate";
    default: return "learning internal error";
    }
}

bool 模拟学习服务::可接收学习(稳定编码 方法节点) const {
    return !方法_.是历史服务结算方法(方法节点);
}

新学习结束通知 模拟学习服务::学习(稳定编码 方法节点, const std::function<bool()>& 停止检查) {
    using 原因 = 新学习运行异常原因;
    try {
        if (停止检查 && 停止检查())
            throw 新学习运行异常(方法节点, 原因::停止取消);
        if (!有效(方法节点))
            throw 新学习运行异常(方法节点, 原因::输入不合法);
        const auto 协作停止 = [&] {
            return (停止检查 && 停止检查()) || !可接收学习(方法节点);
        };
        if (协作停止()) throw 新学习运行异常(方法节点, 原因::停止取消);
        const auto 方法 = 方法_.获取方法(方法节点);
        if (!方法)
            throw 新学习运行异常(方法节点, 原因::结构不一致);
        if (方法->来源方法节点 || 方法->实例根节点
            || 方法->实现方式 != 新方法实现方式::本能
            || 方法->启用状态 != 新方法启用状态::启用)
            throw 新学习运行异常(方法节点, 原因::输入不合法);
        if (!方法->本能函数登记名 || 方法->本能函数登记名->empty())
            throw 新学习运行异常(方法节点, 原因::结构不一致);
        for (std::size_t i = 0; i < 方法->参数规格组.size(); ++i) {
            if (!方法->参数规格组[i].有序规格)
                throw 新学习运行异常(方法节点, 原因::规格未支持,
                    "simulation parameter " + std::to_string(i + 1)
                    + ": legacy role specification requires migration to ordered parameters");
        }
        // 只有通用模拟流程实际返回，才能交付正常学习结束通知。
        学习有序本能(*方法, 协作停止);
        if (协作停止()) throw 新学习运行异常(方法节点, 原因::停止取消);
        return {方法节点};
    } catch (const 新学习运行异常&) {
        throw;
    } catch (const std::bad_alloc&) {
        throw 新学习运行异常(方法节点, 原因::资源不足);
    } catch (const std::exception& e) {
        throw 新学习运行异常(方法节点, 原因::内部错误, e.what());
    } catch (...) {
        throw 新学习运行异常(方法节点, 原因::内部错误);
    }
}
} // namespace 海中鱼巣

namespace 海中鱼巣 {

void 模拟学习服务::学习有序本能(const 新方法信息& m,const std::function<bool()>& 停止检查) {
    const auto 停止=[&] { if(停止检查&&停止检查())throw 新学习运行异常(m.节点,新学习运行异常原因::停止取消); };
    const auto 不支持 = [&](std::uint32_t 位置, const char* 原因) -> void {
        throw 新学习运行异常(m.节点, 新学习运行异常原因::规格未支持,
            "simulation parameter " + std::to_string(位置) + ": " + 原因);
    };
    std::vector<新模拟有序参数输入> base;
    std::vector<std::optional<新特征匹配定义快照>> defs;
    for(const auto& spec:m.参数规格组) {
        if(!spec.有序规格) 不支持(0,"mixed ordered and role parameters");
        base.push_back({*spec.有序规格});
        defs.push_back(spec.有序规格->信息类型==新方法参数信息类型::特征
            ?方法_.读取模拟特征范围(spec.有序规格->范围概念节点):std::nullopt);
        if(spec.有序规格->信息类型==新方法参数信息类型::特征&&!defs.back())
            throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,
                "parameter feature range unavailable: position=" + std::to_string(spec.有序规格->序号)
                + " concept=" + std::to_string(spec.有序规格->范围概念节点.值));
    }
    const auto N=base.size();
    // 参数各自的概念范围决定取样点；合并后的范围只负责合法性约束。
    const auto 原始参数定义 = defs;
    // 同一F的参数限定和存在原型均取正式值域交集，不丢弃任一限定。
    const auto 收窄特征范围 = [&](std::size_t k,const 新特征匹配定义快照& d) {
        if(defs[k]->特征类型根!=d.特征类型根||defs[k]->材料类型!=d.材料类型
            ||defs[k]->比较规则!=d.比较规则)
            throw 新学习运行异常(m.节点,新学习运行异常原因::输入不合法,"aliased feature definitions disagree");
        if(std::holds_alternative<特征概念全材料值域>(defs[k]->值域)) {
                defs[k]->值域=d.值域;
            } else if(std::holds_alternative<特征概念全材料值域>(d.值域)) {
                // 另一侧为全集，保留本侧已有约束。
            } else if(const auto* a=std::get_if<std::vector<特征概念I64闭区间>>(&defs[k]->值域)) {
                const auto* b=std::get_if<std::vector<特征概念I64闭区间>>(&d.值域);
                if(!b) 不支持(static_cast<std::uint32_t>(k+1),"prototype range intersection shape");
                std::vector<特征概念I64闭区间> intersection;
                for(const auto& x:*a)for(const auto& y:*b) {
                    const auto l=(std::max)(x.下界,y.下界),h=(std::min)(x.上界,y.上界);
                    if(l<=h)intersection.push_back({l,h});
                }
                defs[k]->值域=std::move(intersection);
            } else if (auto* a = std::get_if<特征概念结构化I64值域>(&defs[k]->值域)) {
                const auto* b = std::get_if<特征概念结构化I64值域>(&d.值域);
                if (!b || a->分量.size() != b->分量.size())
                    不支持(static_cast<std::uint32_t>(k+1),"prototype structured range shape");
                for (std::size_t n=0;n<a->分量.size();++n) {
                    auto& 左=a->分量[n]; const auto& 右=b->分量[n];
                    if (左.角色!=右.角色 || 左.单位!=右.单位)
                        不支持(static_cast<std::uint32_t>(k+1),"prototype component roles or units differ");
                    std::vector<特征概念I64闭区间> 交集;
                    for (const auto& x:左.值域) for (const auto& y:右.值域) {
                        const auto l=(std::max)(x.下界,y.下界),h=(std::min)(x.上界,y.上界);
                        if(l<=h)交集.push_back({l,h});
                    }
                    左.值域=std::move(交集);
                }
            } else if (defs[k]->值域 != d.值域) {
                const auto 比较=概念_.比较值域(defs[k]->特征类型根,defs[k]->值域,d.值域);
                if(比较.状态==特征概念规则状态::资源失败)throw 新学习运行异常(m.节点,新学习运行异常原因::资源不足);
                if(比较.状态!=特征概念规则状态::已完成||!比较.关系)
                    不支持(static_cast<std::uint32_t>(k+1),"prototype domain comparison is unsupported");
                if(*比较.关系==特征概念值域关系::左包含右)defs[k]->值域=d.值域;
                else if(*比较.关系==特征概念值域关系::相等||*比较.关系==特征概念值域关系::右包含左){}
                else if(*比较.关系==特征概念值域关系::分离)defs[k]->值域=std::vector<稳定编码>{};
                else {
                    const auto* 有限=std::get_if<std::vector<稳定编码>>(&defs[k]->值域);
                    const auto* 另一有限=std::get_if<std::vector<稳定编码>>(&d.值域);
                    if(!有限&&!另一有限)不支持(static_cast<std::uint32_t>(k+1),"nonfinite intersecting prototype domains");
                    const auto& 取样=有限 ? *有限 : *另一有限;
                    const auto& 范围=有限 ? d.值域 : defs[k]->值域;
                    std::vector<稳定编码> 交集;
                    for(const auto v:取样) {
                        const auto r=概念_.比较值域(defs[k]->特征类型根,特征概念值域{std::vector<稳定编码>{v}},范围);
                        if(r.状态!=特征概念规则状态::已完成||!r.关系)
                            不支持(static_cast<std::uint32_t>(k+1),"finite prototype element comparison");
                        if(*r.关系==特征概念值域关系::相等||*r.关系==特征概念值域关系::右包含左)交集.push_back(v);
                    }
                    defs[k]->值域=std::move(交集);
                }
            }
    };
    std::vector<std::size_t> 代表(N);
    for(std::size_t i=0;i<N;++i)代表[i]=i;
    const auto 根=[&](std::size_t i){while(代表[i]!=i)i=代表[i];return i;};
    for(const auto& r:m.初始模拟关系组) {
        if(!r.左参数序号||r.左参数序号>N||!r.右参数序号||r.右参数序号>N)
            throw 新学习运行异常(m.节点,新学习运行异常原因::输入不合法,"relation parameter out of range");
        if(r.种类==新方法参数关系种类::同一) {
            const auto a=根(r.左参数序号-1),b=根(r.右参数序号-1);
            if(base[a].规格.信息类型!=base[b].规格.信息类型)
                throw 新学习运行异常(m.节点,新学习运行异常原因::输入不合法,"alias information type mismatch");
            代表[(std::max)(a,b)]=(std::min)(a,b);
        }
    }
    for(std::size_t i=0;i<N;++i){代表[i]=根(i);if(代表[i]!=i)base[i].同一参数=static_cast<std::uint32_t>(代表[i]+1);}
    for(std::size_t i=0;i<N;++i)if(代表[i]!=i&&defs[i])
        收窄特征范围(代表[i],*defs[i]);
    for(const auto& r:m.初始模拟关系组) {
        const auto a=代表[r.左参数序号-1],b=代表[r.右参数序号-1];
        if(r.种类==新方法参数关系种类::不同&&a==b)
            throw 新学习运行异常(m.节点,新学习运行异常原因::输入不合法,"conflicting same/different relations");
        if(r.种类==新方法参数关系种类::特征所属存在) {
            auto& p=base[a]; const auto owner=static_cast<std::uint32_t>(b+1);
            if(p.所属存在参数&&p.所属存在参数!=owner)
                throw 新学习运行异常(m.节点,新学习运行异常原因::输入不合法,"conflicting feature owners");
            p.所属存在参数=owner;
        }
        if(r.种类==新方法参数关系种类::集合包含存在)
            base[a].集合成员参数组.push_back(static_cast<std::uint32_t>(b+1));
    }
    for(std::size_t i=0;i<N;++i)if(代表[i]!=i) {
        base[i].所属存在参数=base[代表[i]].所属存在参数;
        base[i].集合成员参数组=base[代表[i]].集合成员参数组;
    }
    // 存在范围中的正式特征成为原型的一部分，不凭位置邻接给参数添关系。
    for(std::size_t i=0;i<base.size();++i) {
        if(defs[i])if(const auto* 引用=std::get_if<特征概念存在引用值域>(&defs[i]->值域)) {
            if(std::none_of(base.begin(),base.end(),[&](const auto& p) {
                return p.规格.信息类型==新方法参数信息类型::存在&&p.规格.范围概念节点==引用->存在概念节点;
            })) {
                // 类型范围缺少原型时追加私有存在；已有同范围原型复用，递归引用由此闭环。
                base.push_back({{0,新方法参数信息类型::存在,引用->存在概念节点}});
                defs.push_back(std::nullopt);
            }
        }
        if(base[i].规格.信息类型!=新方法参数信息类型::存在)continue;
        const auto owner=static_cast<std::uint32_t>((i<N ? 代表[i] : i)+1);
        const auto ec=方法_.读取模拟存在范围(base[i].规格.范围概念节点);
        if(!ec)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,
            "existence range unavailable: position=" + std::to_string(i + 1)
            + " concept=" + std::to_string(base[i].规格.范围概念节点.值));
        for(const auto& required:ec->特征组) {
            const auto d=方法_.读取模拟特征范围(required.具体特征概念);
            if(!d)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,
                "existence required feature range unavailable: concept=" + std::to_string(required.具体特征概念.值));
            auto k=base.size();
            for(std::size_t j=0;j<base.size();++j)
                if(base[j].所属存在参数==owner&&defs[j]&&defs[j]->特征类型根==d->特征类型根) {k=j;break;}
            if(k==base.size()) {
                base.push_back({{0,新方法参数信息类型::特征,required.具体特征概念},owner});
                defs.push_back(d);
            } else { 收窄特征范围(k,*d); }
        }
    }
    for(std::size_t i=0;i<N;++i)if(代表[i]!=i&&defs[i])defs[i]=defs[代表[i]];
    const auto 数值候选 = [&](std::size_t 位置, std::optional<std::size_t> 分量) {
        const auto 读取区间 = [&](const 新特征匹配定义快照& 定义)
            -> const std::vector<特征概念I64闭区间>* {
            if (!分量) return std::get_if<std::vector<特征概念I64闭区间>>(&定义.值域);
            const auto* 结构 = std::get_if<特征概念结构化I64值域>(&定义.值域);
            return 结构 && *分量 < 结构->分量.size() ? &结构->分量[*分量].值域 : nullptr;
        };
        const auto* 有效范围 = 读取区间(*defs[位置]);
        if (!有效范围) 不支持(static_cast<std::uint32_t>(位置 + 1), "numeric candidate range unavailable");
        // 私有原型没有独立调用参数，其正式范围直接作为取样依据。
        const auto* 初始范围 = 位置 < N && 原始参数定义[位置]
            ? 读取区间(*原始参数定义[位置]) : 有效范围;
        if (!初始范围) {
            if (位置 < N && std::holds_alternative<特征概念全材料值域>(原始参数定义[位置]->值域))
                初始范围 = 有效范围; // 全材料声明本身没有有限点，由正式原型限定提供。
            else 不支持(static_cast<std::uint32_t>(位置 + 1), "numeric parameter sample shape unavailable");
        }
        auto 候选 = 生成通用模拟整数候选(*初始范围);
        std::erase_if(候选, [&](const auto& 项) {
            const auto 值 = std::get<std::int64_t>(项);
            return std::none_of(有效范围->begin(), 有效范围->end(), [&](const auto& 区间) {
                return 区间.下界 <= 值 && 值 <= 区间.上界;
            });
        });
        for (std::size_t j = 0; j < N && 位置 < N; ++j) {
            if (j == 位置 || 代表[j] != 代表[位置] || !原始参数定义[j]) continue;
            if (std::holds_alternative<特征概念全材料值域>(原始参数定义[j]->值域)) continue;
            const auto* 范围 = 读取区间(*原始参数定义[j]);
            if (!范围) 不支持(static_cast<std::uint32_t>(j + 1), "aliased numeric sample shape unavailable");
            const auto 其它候选 = 生成通用模拟整数候选(*范围);
            std::erase_if(候选, [&](const auto& 值) {
                return std::find(其它候选.begin(), 其它候选.end(), 值) == 其它候选.end();
            });
        }
        return 候选;
    };
    // 来源只由正式有限材料明确给出，不从现实实例中任取首项。
    // 这里只投影执行所需的E/F/当前材料闭包；实例身份全部转换为私有输入位置。
    const auto 原型数量=base.size();
    // 正式任务/需求成员先交各自owner读取；可读值投影不冒充可执行本地图。
    std::string 业务缺口;
    std::map<稳定编码,新模拟任务来源投影> 任务来源;
    std::vector<新模拟任务治理输入> 任务治理组;
    struct 来源采集已漂移 final {};
    for (;;) {
        停止();
        任务来源.clear();
        任务治理组.clear();
        业务缺口.clear();
        try
        {
            for (std::size_t i = 0; i < 原型数量; ++i)
                if (defs[i] && (defs[i]->比较规则 == 特征概念比较规则::服务任务集合身份判等 ||
                                defs[i]->比较规则 == 特征概念比较规则::需求集合身份判等))
                {
                    停止();
                    const auto *vs = std::get_if<std::vector<稳定编码>>(&defs[i]->值域);
                    if (!vs)
                        不支持(static_cast<std::uint32_t>(i + 1),
                               "task/demand collection has no explicitly known finite source material");
                    for (const auto V : *vs)
                    {
                        const auto material = 方法_.读取模拟候选材料(V);
                        const auto *members =
                            material ? std::get_if<std::vector<std::uint64_t>>(&material->材料) : nullptr;
                        if (!members)
                            throw 新学习运行异常(m.节点, 新学习运行异常原因::结构不一致,
                                                 "typed source collection material unavailable");
                        for (const auto n : *members)
                        {
                            停止();
                            const auto prefix = " M=" + std::to_string(m.节点.值) +
                                                " parameter=" + std::to_string(i + 1) + " V=" + std::to_string(V.值) +
                                                " member=" + std::to_string(n);
                            const auto consume = [&](const auto &read, const char *kind) {
                                if (read.状态 == 新模拟业务来源状态::来源已变化)
                                    throw 来源采集已漂移{};
                                if (read.状态 != 新模拟业务来源状态::已读取 || !read.信息)
                                    throw 新学习运行异常(m.节点, 新学习运行异常原因::结构不一致,
                                                         prefix + " source=" + kind + " status=" +
                                                             std::to_string(static_cast<unsigned>(read.状态)) + " " +
                                                             read.诊断);
                                业务缺口 += prefix + " source=" + kind + " status=read; missing local typed reader";
                                for (const auto &edge : read.信息->实例引用组)
                                    业务缺口 += " [category=" + std::to_string(static_cast<unsigned>(edge.类别)) +
                                                " field=" + edge.字段角色 +
                                                " reference=" + std::to_string(edge.来源身份.值) + "]";
                                业务缺口 += '\n';
                            };
                            if (defs[i]->比较规则 == 特征概念比较规则::服务任务集合身份判等)
                            {
                                const auto read = 业务来源_.读取任务来源(稳定编码{n});
                                if (read.状态 == 新模拟业务来源状态::来源已变化)
                                    throw 来源采集已漂移{};
                                if (read.状态 != 新模拟业务来源状态::已读取 || !read.信息)
                                    throw 新学习运行异常(m.节点, 新学习运行异常原因::结构不一致,
                                                         prefix + " " + read.诊断);
                                const auto [it, added] = 任务来源.emplace(稳定编码{n}, *read.信息);
                                if (!added &&
                                    (it->second.任务 != read.信息->任务 || it->second.虚拟存在 != read.信息->虚拟存在 ||
                                     it->second.治理特征 != read.信息->治理特征))
                                    throw 来源采集已漂移{};
                            }
                            else
                                consume(业务来源_.读取需求来源(稳定编码{n}), "demand");
                        }
                    }
                }
            if (!业务缺口.empty())
                throw 新学习运行异常(m.节点, 新学习运行异常原因::规格未支持, std::move(业务缺口));
            for (const auto &[T, source] : 任务来源)
            {
                停止();
                const auto fresh = 业务来源_.读取任务来源(T);
                if (fresh.状态 == 新模拟业务来源状态::来源已变化)
                    throw 来源采集已漂移{};
                if (!fresh.信息 || fresh.状态 != 新模拟业务来源状态::已读取)
                    throw 新学习运行异常(m.节点, 新学习运行异常原因::结构不一致,
                                         "task governance source changed during capture");
                if (fresh.信息->任务 != source.任务 || fresh.信息->虚拟存在 != source.虚拟存在 ||
                    fresh.信息->治理特征 != source.治理特征)
                    throw 来源采集已漂移{};
                任务治理组.push_back({source.治理特征});
            }
            break;
        }
        catch (const 来源采集已漂移 &)
        {
            // 还未建立模拟对象或发表知识；丢弃整批纯值副本，不拼接两次来源。
            停止();
            std::this_thread::yield();
        }
    }
    std::map<稳定编码, std::size_t> 来源存在位置;
    std::map<稳定编码, 新存在信息> 来源存在快照;
    std::map<稳定编码, 新特征匹配实例快照> 来源特征快照;
    std::vector<稳定编码> 待复制存在;
    const auto 加入来源存在 = [&](稳定编码 E) -> std::size_t {
        if(const auto it=来源存在位置.find(E);it!=来源存在位置.end())
            return it->second;
        const auto e=方法_.读取模拟来源存在(E);
        if(!e)
            throw 新学习运行异常(m.节点, 新学习运行异常原因::结构不一致, "copy source existence unavailable");
        const auto pos=base.size();
        新模拟有序参数输入 p{{0,新方法参数信息类型::存在,e->专属存在概念节点}};
        p.来源存在=E;
        base.push_back(std::move(p));
        defs.push_back(std::nullopt);
        来源存在位置.emplace(E,pos);
        来源存在快照.emplace(E, *e);
        待复制存在.push_back(E);
        return pos;
    };
    for(std::size_t i=0;i<原型数量;++i)if(defs[i]
        &&defs[i]->比较规则==特征概念比较规则::存在集合身份判等) {
        if(const auto* vs=std::get_if<std::vector<稳定编码>>(&defs[i]->值域)) {
            // 加入来源可能扩容defs，遍历材料编码的值副本，不保留容器内指针。
            const auto 材料组=*vs;
            for(const auto V:材料组) {
                const auto v=方法_.读取模拟候选材料(V);
                const auto* a=v?std::get_if<std::vector<std::uint64_t>>(&v->材料):nullptr;
                if(!a)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"copy source collection material unavailable");
                for(const auto E:*a)加入来源存在(稳定编码{E});
            }
        }
    }
    for(std::size_t next=0;next<待复制存在.size();++next) {
        停止();
        const auto E=待复制存在[next];
        const auto owner=static_cast<std::uint32_t>(来源存在位置.at(E)+1);
        const auto e=来源存在快照.at(E);
        for(const auto F:e.特征节点组) {
            if(来源特征快照.contains(F))
                throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"copy source feature has multiple owners");
            const auto f=方法_.读取模拟来源特征(F);
            if(!f)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"copy source feature unavailable");
            if(f->上级特征节点)不支持(0,"copy source subfeature organization is unsupported");
            if(f->实例单位&&f->实例单位!=f->定义.单位)
                不支持(0,"copy source instance unit differs from its formal definition");
            if(!f->当前值)不支持(0,"copy source feature current value is unknown");
            新模拟有序参数输入 p{{0,新方法参数信息类型::特征,f->定义.概念节点},owner};
            p.复制上下文=true;
            if(f->定义.比较规则==特征概念比较规则::存在身份精确) {
                const auto* ref=std::get_if<稳定编码>(&*f->当前值);
                if(!ref)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"copy source existence reference shape");
                p.引用存在参数=static_cast<std::uint32_t>(加入来源存在(*ref)+1);
            } else if(const auto* n=std::get_if<std::int64_t>(&*f->当前值))p.值=*n;
            else {
                if(!f->当前完整材料)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"copy source full material unavailable");
                p.材料=f->当前完整材料->材料;
                if(f->定义.比较规则==特征概念比较规则::存在集合身份判等) {
                    const auto* a=std::get_if<std::vector<std::uint64_t>>(&*p.材料);
                    if(!a)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"copy source existence collection shape");
                    const auto members=*a;
                    for(const auto ref:members)p.集合成员参数组.push_back(static_cast<std::uint32_t>(加入来源存在(稳定编码{ref})+1));
                    p.材料=新特征值材料{std::vector<std::uint64_t>{}};
                } else if(f->定义.比较规则==特征概念比较规则::服务任务集合身份判等
                    ||f->定义.比较规则==特征概念比较规则::需求集合身份判等) {
                    不支持(0,"copy source task/demand references need their isolated owner");
                }
            }
            来源特征快照.emplace(F,*f);
            base.push_back(std::move(p));defs.push_back(f->定义);
        }
    }
    // owner逐项采集不是跨世界原子快照。读回检测采集期间变化；之后只用值副本。
    for(const auto& [E,e]:来源存在快照)if(方法_.读取模拟来源存在(E)!=e)
        throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"copy source existence changed during capture");
    for(const auto& [F,f]:来源特征快照)if(方法_.读取模拟来源特征(F)!=f)
        throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"copy source feature changed during capture");
    std::vector<通用模拟候选轴> axes;
    std::vector<std::vector<std::size_t>> axisFor(base.size());
    std::vector<bool> 集合选择(base.size(),false),引用选择(base.size(),false);
    for(std::size_t i=0;i<原型数量;++i) if(defs[i]) {
        if(i<N&&代表[i]!=i){axisFor[i]=axisFor[代表[i]];集合选择[i]=集合选择[代表[i]];引用选择[i]=引用选择[代表[i]];continue;}
        if (const auto* 结构 = std::get_if<特征概念结构化I64值域>(&defs[i]->值域)) {
            if (defs[i]->材料类型 != 特征概念材料物理类型::I64数组)
                不支持(static_cast<std::uint32_t>(i+1),"structured non-array material needs payload shape constraints");
            if (结构->分量.empty())
                throw 新学习运行异常(m.节点,新学习运行异常原因::输入不合法,"empty structured integer domain");
            for (std::size_t 分量 = 0; 分量 < 结构->分量.size(); ++分量) {
                axisFor[i].push_back(axes.size());
                axes.push_back({static_cast<std::uint32_t>(axes.size()+1),数值候选(i, 分量)});
            }
            continue;
        }
        std::vector<通用模拟候选值> values;
        if(const auto* 引用=std::get_if<特征概念存在引用值域>(&defs[i]->值域)) {
            if(i<N)不支持(static_cast<std::uint32_t>(i+1),"explicit reference C/R needs value-position representation");
            引用选择[i]=true;
            for(std::size_t j=0;j<base.size();++j)
                if(base[j].规格.信息类型==新方法参数信息类型::存在
                    &&base[j].规格.范围概念节点==引用->存在概念节点&&(j>=N||代表[j]==j))
                    values.emplace_back(static_cast<std::int64_t>(j+1));
            if(values.empty())不支持(static_cast<std::uint32_t>(i+1),"existence-reference prototype not found");
            values=生成通用模拟枚举候选(std::move(values),m.节点.值+i);
        }
        else if(std::holds_alternative<std::vector<特征概念I64闭区间>>(defs[i]->值域))
            values=数值候选(i, std::nullopt);
        else if(const auto* r=std::get_if<std::vector<稳定编码>>(&defs[i]->值域)) {
            for(auto v:*r)values.emplace_back(v);
            values=生成通用模拟枚举候选(std::move(values),m.节点.值+i);
        } else if(const auto* r=std::get_if<特征概念存在集合值域>(&defs[i]->值域)) {
            if(r->至少一个存在||!r->必含存在节点组.empty()) 不支持(static_cast<std::uint32_t>(i+1),"referenced existence collection needs isolated binding");
            values.emplace_back(std::int64_t{0}); // 空集合原型的本地选择号，不是特征的数值。
        } else { 不支持(static_cast<std::uint32_t>(i+1),"range has no finite candidate declaration"); }
        axisFor[i].push_back(axes.size());
        axes.push_back({static_cast<std::uint32_t>(axes.size()+1),std::move(values)});
    }
    std::vector<std::size_t> eventAxes;
    for(const auto pos:m.初始模拟事件参数组) {
        if(!pos||pos>N||!defs[pos-1])throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,
            "simulation event parameter position invalid");
        // 每个事件按其声明参数的原始范围取样；别名共享F身份，不要求先后事件值相同。
        const auto& 事件定义 = std::holds_alternative<特征概念全材料值域>(原始参数定义[pos-1]->值域)
            ? *defs[pos-1] : *原始参数定义[pos-1];
        const auto* domain=std::get_if<std::vector<特征概念I64闭区间>>(&事件定义.值域);
        if(!domain) 不支持(pos,"event parameter is not an integer domain");
        eventAxes.push_back(axes.size());
        axes.push_back({static_cast<std::uint32_t>(axes.size()+1),生成通用模拟整数候选(*domain)});
    }
    通用模拟组合游标 cursor(std::move(axes));
    while(!cursor.已结束()) {
        停止();
        const auto values=cursor.读取当前组合();
        auto input=base;
        for(std::size_t i=0;i<原型数量;++i) if(defs[i]) {
            if (std::holds_alternative<特征概念结构化I64值域>(defs[i]->值域)) {
                std::vector<std::int64_t> 分量;
                分量.reserve(axisFor[i].size());
                for (const auto 轴 : axisFor[i]) 分量.push_back(std::get<std::int64_t>(values[轴]));
                input[i].材料 = 新特征值材料{std::move(分量)};
                continue;
            }
            const auto& x=values[axisFor[i].front()];
            if(引用选择[i])input[i].引用存在参数=static_cast<std::uint32_t>(std::get<std::int64_t>(x));
            else if(defs[i]->材料类型==特征概念材料物理类型::I64标量)input[i].值=std::get<std::int64_t>(x);
            else if(集合选择[i]) {
                input[i].材料=新特征值材料{std::vector<std::uint64_t>{}};
                input[i].集合成员参数组.push_back(static_cast<std::uint32_t>(std::get<std::int64_t>(x)));
            }
            else if(const auto* v=std::get_if<稳定编码>(&x)) {
                const auto material=方法_.读取模拟候选材料(*v);
                if(!material)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,
                    "selected candidate material unavailable");
                // 源成员只通过本地参数位置进入样例；数组中的源编码不进入模拟图。
                if(defs[i]->比较规则==特征概念比较规则::存在集合身份判等) {
                    const auto* a=std::get_if<std::vector<std::uint64_t>>(&material->材料);
                    if(!a)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,
                        "selected existence collection material is not U64 array");
                    for(const auto E:*a)input[i].集合成员参数组.push_back(static_cast<std::uint32_t>(来源存在位置.at(稳定编码{E})+1));
                    input[i].材料=新特征值材料{std::vector<std::uint64_t>{}};
                    continue;
                }
                if(defs[i]->比较规则==特征概念比较规则::服务任务集合身份判等) {
                    const auto* a=std::get_if<std::vector<std::uint64_t>>(&material->材料);
                    if(!a)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,
                        "selected task collection material is not U64 array");
                    for(const auto T:*a)input[i].来源任务成员组.push_back(稳定编码{T});
                    input[i].材料=新特征值材料{std::vector<std::uint64_t>{}};
                    continue;
                }
                input[i].材料=material->材料;
            } else input[i].材料=新特征值材料{std::vector<std::uint64_t>{}};
        }
        std::vector<新模拟输入事件> events;
        for(std::size_t i=0;i<eventAxes.size();++i)
            events.push_back({m.初始模拟事件参数组[i],std::get<std::int64_t>(values[eventAxes[i]])});
        auto world=std::make_shared<新_模拟世界类>(方法_);
        const auto created=world->建立有序样例(m.节点,input,events,任务治理组);
        if(created.状态!=新模拟世界状态::已完成)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,
            "ordered sample construction rejected: state=" + std::to_string(static_cast<int>(created.状态)));
        const auto bindings=world->读取参数绑定();
        if(!bindings)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,
            "ordered sample parameter bindings unavailable");
        const auto 集合正式材料 = [&](const 新特征值信息& value,特征概念比较规则 rule) -> std::optional<新特征值信息> {
            const auto* members=std::get_if<std::vector<std::uint64_t>>(&value.材料);
            if(!members)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,
                "knowledge collection material is not U64 array");
            std::vector<std::uint64_t> sources;
            bool 全为来源=true;
            for(const auto n:*members) {
                const auto source=rule==特征概念比较规则::服务任务集合身份判等
                    ?world->读取模拟任务来源(稳定编码{n}):world->读取模拟存在来源(稳定编码{n});
                if(source)sources.push_back(source->值);else 全为来源=false;
            }
            if(全为来源) {
                std::sort(sources.begin(),sources.end());
                sources.erase(std::unique(sources.begin(),sources.end()),sources.end());
                auto result=value;result.节点={};result.材料=std::move(sources);
                return result; // 只发表知识材料，不向任何源存在写入模拟结果。
            }
            if(rule==特征概念比较规则::服务任务集合身份判等)
                不支持(0,"task collection contains an unmapped member; existence-position relations cannot represent tasks");
            for(const auto n:*members) {
                const auto visible=std::any_of(bindings->begin(),bindings->end(),[&](const auto& b) {
                    return b.有序引用&&b.有序引用->信息类型==新方法参数信息类型::存在
                        &&b.有序引用->信息节点.值==n;
                });
                if(!visible)不支持(0,"mixed copied and synthetic collection cannot be fully expressed by current C/R positions");
            }
            return {}; // 每个成员均可通过正式有序参数关系表达。
        };
        auto context=方法_.建立模拟参数场景(m.节点,*bindings,world);
        if(!context.参数场景)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,
            "simulation parameter context rejected: state=" + std::to_string(static_cast<int>(context.状态))
            + " builtin=" + m.本能函数登记名.value_or("")
            + " parameters=" + std::to_string(m.参数规格组.size()));
        for(std::size_t i=0;i<N;++i)if(defs[i]&&defs[i]->比较规则==特征概念比较规则::服务任务集合身份判等) {
            const auto F=(*bindings)[i].有序引用->信息节点;
            const auto f=context.参数场景->读取绑定特征(F);
            const auto* V = (f && f->当前值) ? std::get_if<稳定编码>(&*f->当前值) : nullptr;
            const auto v=V?world->读取模拟特征值材料(*V):std::nullopt;
            const auto* members=v?std::get_if<std::vector<std::uint64_t>>(&v->材料):nullptr;
            if(!members)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,
                "local task collection readback is not U64 array");
            for(const auto T:*members) {
                const auto source=world->读取模拟任务来源(稳定编码{T});
                const auto ref=world->查找任务虚拟存在引用(稳定编码{T});
                const auto state=ref?world->读取任务虚拟存在引用(*ref):std::nullopt;
                const auto local=context.参数场景->读取绑定集合任务特征组(static_cast<std::uint32_t>(i+1),稳定编码{T});
                if(!source||!local||!state||state->引用!=local->引用
                    ||state->当前状态!=local->状态.当前值)
                    throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"local task governance readback missing");
                const auto& original=任务来源.at(*source).治理特征;
                if(local->引用.任务节点.值!=T||local->引用.任务节点==*source
                    ||local->引用.虚拟存在节点==original.引用.虚拟存在节点
                    ||local->状态.当前值!=original.状态.当前值
                    ||local->服务值.has_value()!=original.服务值.has_value()
                    ||(local->服务值&&local->服务值->当前值!=original.服务值->当前值))
                    throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"local task governance readback disagrees");
            }
        }
        std::vector<std::optional<新特征信息>> before(N);
        std::vector<std::optional<新特征值信息>> beforeMaterial(N);
        for(std::size_t i=0;i<N;++i) if(defs[i]) {
            before[i]=context.参数场景->读取绑定特征((*bindings)[i].有序引用->信息节点);
            if(!before[i]||!before[i]->当前值)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,
                "simulation before feature current value unavailable: position=" + std::to_string(i + 1));
            if(const auto* v=std::get_if<稳定编码>(&*before[i]->当前值))
                beforeMaterial[i]=world->读取模拟特征值材料(*v);
        }
        停止();
        world->开始记录复制上下文读取();
        const auto call=方法_.调用本能函数(m.节点,*context.参数场景);
        const auto 有隐藏依赖=world->结束记录复制上下文读取();
        const auto 成员读取组 = world->读取任务成员记录();
        const auto 方法写回组 = world->读取本次写回特征组();
        const auto 是本次方法写回 = [&](稳定编码 特征) {
            return std::find(方法写回组.begin(),方法写回组.end(),特征) != 方法写回组.end();
        };
        if (成员读取组.size() > 1) 不支持(0,"multiple task members or collection paths are not supported");
        if(有隐藏依赖&&(!来源存在位置.empty()||!任务治理组.empty()))
            不支持(0,"copied context was read but its conditions have no formal C/R parameter positions");
        for (const auto 特征 : 方法写回组) {
            bool 可定位 = false;
            for (std::size_t i=0;i<N;++i)
                if (defs[i] && (*bindings)[i].有序引用->信息节点 == 特征) 可定位 = true;
            for (const auto& 路径 : 成员读取组)
                if (路径.动作前.状态.特征节点 == 特征
                    || (路径.动作前.服务值 && 路径.动作前.服务值->特征节点 == 特征)) 可定位 = true;
            if (!可定位) 不支持(0,"method wrote a feature without a formal C/R parameter or member path");
        }
        if(call.状态!=新方法本能调用状态::已调用||!call.本次条件结果对)
            throw 新学习运行异常(m.节点,新学习运行异常原因::本能运行异常);
        const auto& run=*call.本次条件结果对;
        std::optional<新方法条件结果定义> 本组已发表;
        if(run.结果.状态==新本能函数执行状态::已按合同返回
            &&run.结果.结果取得状态==新本能函数结果取得状态::已取得) {
            新方法条件结果定义 pair;
            pair.条件.关系规则=新方法项关系规则::全部成立;
            pair.条件.参数关系组=m.初始模拟关系组;
            pair.结果.关系规则=新方法项关系规则::全部成立;
            for(std::size_t i=0;i<N;++i) {
                新方法条件项定义 c;c.参数序号=static_cast<std::uint32_t>(i+1);
                if(!defs[i])c.存在概念节点=base[i].规格.范围概念节点;
                else {
                    c.特征类型概念节点=defs[i]->特征类型根;
                    if(const auto* v=std::get_if<std::int64_t>(&*before[i]->当前值))c.精确数值域=特征概念I64闭区间{*v,*v};
                    else {
                        if(!beforeMaterial[i])throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致);
                        if(defs[i]->比较规则==特征概念比较规则::存在集合身份判等
                            ||defs[i]->比较规则==特征概念比较规则::服务任务集合身份判等) {
                            const auto* members=std::get_if<std::vector<std::uint64_t>>(&beforeMaterial[i]->材料);
                            if(!members)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致);
                            if (!成员读取组.empty() && 成员读取组.front().集合参数序号 == i + 1)
                                c.特征值域概念节点 = base[i].规格.范围概念节点;
                            else if(const auto formal=集合正式材料(*beforeMaterial[i],defs[i]->比较规则))
                                c.特征值域概念节点=方法_.保存模拟材料值域(base[i].规格.范围概念节点,*formal);
                            else {
                                c.特征值域概念节点=base[i].规格.范围概念节点;
                                for(std::size_t j=0;j<N;++j)if(!defs[j]
                                    &&std::find(members->begin(),members->end(),(*bindings)[j].有序引用->信息节点.值)!=members->end()) {
                                    const 新方法参数关系 relation{新方法参数关系种类::集合包含存在,
                                        static_cast<std::uint32_t>(i+1),static_cast<std::uint32_t>(j+1)};
                                    if(std::find(pair.条件.参数关系组.begin(),pair.条件.参数关系组.end(),relation)==pair.条件.参数关系组.end())
                                        pair.条件.参数关系组.push_back(relation);
                                }
                            }
                        } else c.特征值域概念节点=方法_.保存模拟材料值域(base[i].规格.范围概念节点,*beforeMaterial[i]);
                    }
                }
                pair.条件.条件项组.push_back(std::move(c));
                if(!defs[i])continue;
                const auto id=(*bindings)[i].有序引用->信息节点;
                const auto f=context.参数场景->读取绑定特征(id);
                if(!f||!f->当前值)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致);
                if (!成员读取组.empty() && 成员读取组.front().集合参数序号 == i + 1) {
                    const auto* V = std::get_if<稳定编码>(&*f->当前值);
                    const auto material = V ? world->读取模拟特征值材料(*V) : std::nullopt;
                    if (!material || !beforeMaterial[i] || material->材料 != beforeMaterial[i]->材料)
                        不支持(static_cast<std::uint32_t>(i + 1),"changed member collection cannot be expressed without source identity");
                    continue; // 未改变集合，不把本轮来源T身份固化成结果。
                }
                if (!是本次方法写回(id)) continue;
                新方法结果项定义 r;r.参数序号=static_cast<std::uint32_t>(i+1);r.特征类型概念节点=defs[i]->特征类型根;
                if(const auto* v=std::get_if<std::int64_t>(&*f->当前值))r.精确数值域=特征概念I64闭区间{*v,*v};
                else {
                    const auto material=world->读取模拟特征值材料(std::get<稳定编码>(*f->当前值));
                    if(!material)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致);
                    if(defs[i]->比较规则==特征概念比较规则::存在集合身份判等
                        ||defs[i]->比较规则==特征概念比较规则::服务任务集合身份判等) {
                        const auto* members=std::get_if<std::vector<std::uint64_t>>(&material->材料);
                        if(!members)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致);
                        if(const auto formal=集合正式材料(*material,defs[i]->比较规则))
                            r.特征值域概念节点=方法_.保存模拟材料值域(base[i].规格.范围概念节点,*formal);
                        else for(std::size_t j=0;j<N;++j) if(!defs[j]
                            &&std::find(members->begin(),members->end(),(*bindings)[j].有序引用->信息节点.值)!=members->end())
                            pair.结果.参数关系组.push_back({新方法参数关系种类::集合包含存在,static_cast<std::uint32_t>(i+1),static_cast<std::uint32_t>(j+1)});
                    } else r.特征值域概念节点=方法_.保存模拟材料值域(base[i].规格.范围概念节点,*material);
                }
                pair.结果.结果项组.push_back(std::move(r));
            }
            if (!成员读取组.empty()) {
                const auto& 路径 = 成员读取组.front();
                const auto after = world->读取任务虚拟存在特征组(路径.动作前.引用);
                if (!after || after->引用 != 路径.动作前.引用
                    || after->服务值.has_value() != 路径.动作前.服务值.has_value())
                    throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"member governance shape changed");
                const auto 增加项 = [&](const 新任务虚拟存在特征读回& 前, const 新任务虚拟存在特征读回& 后) {
                    if (前.特征节点 != 后.特征节点 || 前.特征类型概念节点 != 后.特征类型概念节点)
                        throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致,"member feature identity changed");
                    新方法条件项定义 c;
                    c.参数序号 = 路径.集合参数序号; c.任务成员变量序号 = 1;
                    c.特征类型概念节点 = 前.特征类型概念节点;
                    c.精确数值域 = 特征概念I64闭区间{前.当前值,前.当前值};
                    pair.条件.条件项组.push_back(c);
                    if (是本次方法写回(后.特征节点)) {
                        新方法结果项定义 r;
                        r.参数序号 = 路径.集合参数序号; r.任务成员变量序号 = 1;
                        r.特征类型概念节点 = 后.特征类型概念节点;
                        r.精确数值域 = 特征概念I64闭区间{后.当前值,后.当前值};
                        pair.结果.结果项组.push_back(r);
                    }
                };
                增加项(路径.动作前.状态,after->状态);
                if (after->服务值) 增加项(*路径.动作前.服务值,*after->服务值);
            }
            if(!pair.结果.结果项组.empty()||!pair.结果.参数关系组.empty()) {
                const auto published=方法_.追加模拟条件结果(m.节点,{pair});
                if(published.状态!=新方法学习发布状态::已发布&&published.状态!=新方法学习发布状态::精确重复
                    &&published.状态!=新方法学习发布状态::发布结果未知)
                    throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致);
                const auto fresh=方法_.获取方法(m.节点);
                if(!fresh||!std::any_of(fresh->条件结果组.begin(),fresh->条件结果组.end(),[&](const auto& p) {
                    return p.条件==pair.条件&&p.结果==pair.结果&&p.命中次数==0
                        &&p.知识来源==新方法知识来源种类::模拟;
                }))工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"模拟学习发布读回",
                    L"完整条件结果发布无法读回确认；保留当前样例，禁止析构释放后继续业务。");
                本组已发表=pair;
            }
        }
        context.参数场景.reset();
        world->释放有序样例();
        if (本组已发表) {
            const auto 释放后=方法_.获取方法(m.节点);
            if(!释放后||!std::any_of(释放后->条件结果组.begin(),释放后->条件结果组.end(),[&](const auto& p) {
                return p.条件==本组已发表->条件&&p.结果==本组已发表->结果&&p.命中次数==0
                    &&p.知识来源==新方法知识来源种类::模拟;
            }))throw 新学习运行异常(m.节点,新学习运行异常原因::发布结果未知,"knowledge invalid after sample release");
        }
        cursor.推进();
    }
}

}
