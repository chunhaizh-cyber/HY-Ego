#include "关系陈述.语言服务.h"
#include "../领域/关系概念类.h"
#include "../领域/新_存在类.h"
#include "../领域/新_需求类.h"
#include "../领域/语言含义图.数据.h"

#include <algorithm>
#include <limits>
#include <set>

namespace 海中鱼巣 {
namespace {

using 种类 = 新语言知识角色种类;
using 类别 = 新语言知识角色类别;
using 参数 = 新语言知识参数角色;

新语言知识角色定义 角色定义(std::int64_t 编号, 种类 k, 类别 c) {
    return {编号, k, 新语言知识角色数量::单项, {{编号, {c}}}};
}

// 统一构图：含义操作与角色槽全部来自关系概念表达关联知识，无每关系专用构图函数。
语言图含义材料 按表达槽构造含义图(
    稳定编码 语言,
    新语言知识含义操作 操作,
    const std::vector<std::pair<参数, 稳定编码>>& 槽材料) {
    语言图含义材料 图;
    图.语言节点 = 语言;
    新语言图含义节点 节点;
    节点.局部编号 = 1;
    节点.操作 = 操作;
    节点.作用域 = 新语言知识作用域::普通陈述;
    语言图含义出现 出现;
    出现.出现编号 = 0;
    出现.定义含义编号 = 1;
    出现.有效作用域 = 新语言知识作用域::普通陈述;
    for (std::size_t i = 0; i < 槽材料.size(); ++i) {
        const auto 编号 = static_cast<std::int64_t>(i);
        节点.角色组.push_back(角色定义(编号, 种类::领域对象引用, 类别::存在));
        节点.参数组.push_back({槽材料[i].first, 编号});
        出现.角色组.push_back(
            {编号, 语言图引用值{类别::存在, 槽材料[i].second, {}, {}}});
    }
    图.定义图.根含义编号 = 1;
    图.定义图.节点组.push_back(std::move(节点));
    图.根出现编号 = 0;
    图.出现组.push_back(std::move(出现));
    return 图;
}

bool 存在组成边成立(const 新_存在类& 存在, 稳定编码 整体, 稳定编码 部分) {
    const auto 子组 = 存在.查询直接子存在(整体);
    return std::find(子组.begin(), 子组.end(), 部分) != 子组.end();
}

bool 需求父子边成立(const 新_需求类& 需求, 稳定编码 父, 稳定编码 子) {
    const auto 子组 = 需求.查询直接子需求(父);
    return std::find(子组.begin(), 子组.end(), 子) != 子组.end();
}

// owner 正式读边/字段：领域能力分派，不是按关系种子键写专用成句。
bool 核验外部边(
    const 关系外部边连接& 边,
    稳定编码 源,
    稳定编码 目标,
    const 新_存在类& 存在,
    const 新_需求类& 需求,
    bool& 正向成立,
    bool& 反向成立,
    std::string& 诊断) {
    正向成立 = false;
    反向成立 = false;
    if (边.owner标识 == "新_存在类"
        && 边.边类型 == 基础外部关系类型::父子
        && 边.角色或顺序 == 2) {
        正向成立 = 存在组成边成立(存在, 源, 目标);
        反向成立 = 存在组成边成立(存在, 目标, 源);
        return true;
    }
    if (边.owner标识 == "新_需求类"
        && 边.边类型 == 基础外部关系类型::父子
        && 边.角色或顺序 == 1) {
        正向成立 = 需求父子边成立(需求, 源, 目标);
        反向成立 = 需求父子边成立(需求, 目标, 源);
        return true;
    }
    诊断 = "未接线的 owner/角色组合:" + 边.owner标识;
    return false;
}

bool 核验字段引用(
    const 关系字段引用连接& 字段,
    稳定编码 宿主,
    稳定编码 目标,
    const 新_需求类& 需求,
    bool& 成立,
    std::string& 诊断) {
    成立 = false;
    if (字段.owner标识 == "新_需求类" && 字段.字段键名 == "目标存在") {
        const auto 信息 = 需求.获取需求(宿主);
        if (!信息) {
            诊断 = "字段宿主需求不可读";
            return true; // 已接线；成立=false
        }
        成立 = 信息->目标存在节点 == 目标;
        if (!成立) 诊断 = "需求目标存在与绑定目标不一致";
        return true;
    }
    诊断 = "未接线的字段引用:" + 字段.owner标识 + "/" + 字段.字段键名;
    return false;
}

bool 核验单连接(
    const 关系连接描述& 连接,
    const std::map<std::string, 稳定编码>& 绑定,
    const 新_存在类& 存在,
    const 新_需求类& 需求,
    bool& 正向成立,
    bool& 反向成立,
    std::string& 诊断) {
    正向成立 = false;
    反向成立 = false;
    if (const auto* 字段 = std::get_if<关系字段引用连接>(&连接)) {
        const auto 宿主It = 绑定.find(字段->宿主角色键);
        const auto 目标It = 绑定.find(字段->目标角色键);
        if (宿主It == 绑定.end() || 目标It == 绑定.end()) {
            诊断 = "字段连接缺端点绑定";
            return false;
        }
        bool 成立 = false;
        if (!核验字段引用(*字段, 宿主It->second, 目标It->second, 需求, 成立, 诊断))
            return false;
        正向成立 = 成立;
        return true;
    }
    const auto* 边 = std::get_if<关系外部边连接>(&连接);
    if (!边) return false;
    const auto 源It = 绑定.find(边->源角色键);
    const auto 目标It = 绑定.find(边->目标角色键);
    if (源It == 绑定.end() || 目标It == 绑定.end()) {
        诊断 = "边连接缺端点绑定";
        return false;
    }
    return 核验外部边(
        *边, 源It->second, 目标It->second, 存在, 需求, 正向成立, 反向成立, 诊断);
}

std::optional<稳定编码> 取材存在节点(
    const 关系表达角色槽& 槽,
    稳定编码 端点,
    关系端点对象种类 端点种类,
    const 新_存在类& 存在,
    const 新_需求类& 需求,
    std::string& 诊断) {
    if (槽.投影 == 关系表达取材投影::端点身份) {
        if (端点种类 != 关系端点对象种类::存在 || !存在.获取存在(端点)) {
            诊断 = "投影=端点但端点不是可读存在:" + 槽.端点角色键;
            return std::nullopt;
        }
        return 端点;
    }
    if (端点种类 != 关系端点对象种类::需求) {
        诊断 = "需求投影但端点不是需求:" + 槽.端点角色键;
        return std::nullopt;
    }
    const auto 信息 = 需求.获取需求(端点);
    if (!信息) {
        诊断 = "需求端点读回失败:" + 槽.端点角色键;
        return std::nullopt;
    }
    if (槽.投影 == 关系表达取材投影::需求目标存在) {
        if (!有效(信息->目标存在节点) || !存在.获取存在(信息->目标存在节点)) {
            诊断 = "需求目标存在不可读:" + 槽.端点角色键;
            return std::nullopt;
        }
        return 信息->目标存在节点;
    }
    if (槽.投影 == 关系表达取材投影::需求主体存在) {
        if (!有效(信息->需求主体存在节点)
            || !存在.获取存在(信息->需求主体存在节点)) {
            诊断 = "需求主体存在不可读:" + 槽.端点角色键;
            return std::nullopt;
        }
        return 信息->需求主体存在节点;
    }
    诊断 = "未知取材投影";
    return std::nullopt;
}

bool 知识含指代表达(const 新语言知识定义& 定义) {
    for (const auto& 项 : 定义.结构.有序项组) {
        if (const auto* 图角色 = std::get_if<新语言知识图角色表达项>(&项.载荷)) {
            if (图角色->方式 == 新语言知识表达方式::已消解指代) return true;
        }
        if (const auto* 旧角色 = std::get_if<新语言知识角色表达项>(&项.载荷)) {
            if (旧角色->方式 == 新语言知识表达方式::已消解指代) return true;
        }
    }
    return false;
}

bool 知识含固定尾词(
    const 新语言知识信息& 知识,
    const std::string& 尾词,
    const 新_词类& 词) {
    if (尾词.empty()) return false;
    // 排除带「我/你」指代的同尾词变体，保证预置尾词唯一命中。
    if (知识含指代表达(知识.定义)) return false;
    for (const auto& 项 : 知识.定义.结构.有序项组) {
        const auto* 固定 = std::get_if<新语言知识固定词项>(&项.载荷);
        if (!固定) continue;
        const auto 读 = 词.获取词条(固定->词条节点);
        if (读.状态 == 新词条查询状态::已找到 && 读.信息
            && 读.信息->规范化词面 == 尾词)
            return true;
    }
    return false;
}

// 解析表达关联要消费的根知识：后天节点优先；否则按预置尾词+含义操作在语言知识中解析。
std::vector<稳定编码> 解析表达根知识(
    const 关系概念表达关联& 表达,
    新语言知识含义操作 操作,
    稳定编码 语言节点,
    const 新_语言类& 语言,
    const 新_词类& 词,
    const 概念_存在类& 存在概念,
    std::string& 诊断) {
    if (!表达.表达知识节点组.empty()) {
        for (const auto n : 表达.表达知识节点组) {
            if (!有效(n)) {
                诊断 = "表达知识节点无效";
                return {};
            }
            const auto 读 = 语言.获取语言知识(n, 词, 存在概念);
            if (读.状态 != 新语言知识状态::已完成 || !读.信息) {
                诊断 = "表达知识节点不可读";
                return {};
            }
        }
        return 表达.表达知识节点组;
    }
    if (表达.预置知识尾词.empty()) {
        诊断 = "表达关联无知识节点且无预置尾词（须真消费，不暗匹配全库）";
        return {};
    }
    const auto 候选 = 语言.枚举语言知识候选(
        语言节点, (std::numeric_limits<std::size_t>::max)(), 词, 存在概念);
    if (候选.状态 != 新语言知识状态::已完成) {
        诊断 = "枚举语言知识失败";
        return {};
    }
    std::vector<稳定编码> 命中;
    for (const auto& 信息 : 候选.信息组) {
        if (信息.定义.目标.目的 != 新语言知识表达目的::陈述) continue;
        bool 操作匹配 = false;
        if (信息.定义.目标.含义图) {
            for (const auto& 节点 : 信息.定义.目标.含义图->节点组) {
                if (节点.局部编号 == 信息.定义.目标.含义图->根含义编号
                    && 节点.操作 == 操作)
                    操作匹配 = true;
            }
        } else if (信息.定义.目标.含义组.size() == 1
            && 信息.定义.目标.含义组.front().操作 == 操作) {
            操作匹配 = true;
        }
        if (!操作匹配) continue;
        if (知识含固定尾词(信息, 表达.预置知识尾词, 词)) 命中.push_back(信息.节点);
    }
    if (命中.empty()) {
        诊断 = "预置尾词未命中语言知识:" + 表达.预置知识尾词;
        return {};
    }
    if (命中.size() > 1) {
        诊断 = "预置尾词命中多条语言知识:" + 表达.预置知识尾词;
        return {};
    }
    return 命中;
}

关系陈述结果 完成成文(
    const 关系陈述请求& 请求,
    const 关系概念表达关联& 表达,
    语言图含义材料 图,
    std::vector<稳定编码> 允许根知识,
    const 新_语言类& 语言,
    const 新_词类& 词,
    const 概念_名称关系类& 名称,
    const 概念_存在类& 存在概念,
    const 概念_特征类& 特征概念,
    const 新_存在类& 存在,
    const 新_自然语言世界树类& 语言世界,
    关系陈述结果 out) {
    out.所用表达为预置 = 表达.预置;
    const auto 无限 = (std::numeric_limits<std::size_t>::max)();
    语言基础统一输出请求 成文{
        请求.目标语言节点, 图, 无限, 无限, 无限, 无限, 无限, 无限,
        请求.说话者, 请求.受话者, std::move(允许根知识)};
    const auto 输出 = 从基础信息生成可回读单句(
        成文, 语言世界, 语言, 词, 名称, 存在概念, 特征概念, 存在, nullptr);
    switch (输出.状态) {
    case 语言场景基础输出状态::已生成且唯一回读:
        out.状态 = 关系陈述状态::已生成且唯一回读;
        out.UTF8正文 = 输出.文本;
        out.所用知识节点组 = 输出.所用知识节点组;
        return out;
    case 语言场景基础输出状态::缺规则:
    case 语言场景基础输出状态::结构未支持:
    case 语言场景基础输出状态::回读含义不一致:
        out.状态 = 关系陈述状态::缺语言知识;
        out.诊断 = "成文缺规则或回读不一致";
        return out;
    case 语言场景基础输出状态::缺名称:
        out.状态 = 关系陈述状态::缺名称;
        return out;
    case 语言场景基础输出状态::名称待选:
    case 语言场景基础输出状态::回读多义:
        out.状态 = 关系陈述状态::多义;
        return out;
    case 语言场景基础输出状态::读取漂移:
    case 语言场景基础输出状态::引用不可读:
        out.状态 = 关系陈述状态::前提已变化;
        return out;
    default:
        out.状态 = 关系陈述状态::范围未支持;
        out.诊断 = "统一成文状态=" + std::to_string(static_cast<int>(输出.状态));
        return out;
    }
}

} // namespace

关系陈述语言服务::关系陈述语言服务(
    const 关系概念类& 关系,
    const 新_语言类& 语言,
    const 新_词类& 词,
    const 概念_名称关系类& 名称,
    const 概念_存在类& 存在概念,
    const 概念_特征类& 特征概念,
    const 新_存在类& 存在,
    const 新_需求类& 需求,
    const 新_自然语言世界树类& 语言世界) noexcept
    : 关系_(关系), 语言_(语言), 词_(词), 名称_(名称), 存在概念_(存在概念),
      特征概念_(特征概念), 存在_(存在), 需求_(需求), 语言世界_(语言世界) {}

关系陈述结果 关系陈述语言服务::生成关系陈述(const 关系陈述请求& 请求) const {
    关系陈述结果 out;
    if (请求.关系种子键.empty() || !有效(请求.目标语言节点)) {
        out.状态 = 关系陈述状态::输入拒绝;
        out.诊断 = "缺种子键或目标语言";
        return out;
    }
    if (!语言_.是语言节点(请求.目标语言节点)) {
        out.状态 = 关系陈述状态::输入拒绝;
        out.诊断 = "目标语言不是语言节点";
        return out;
    }
    const auto 概念读 = 关系_.按种子键读取(请求.关系种子键);
    if (概念读.状态 == 关系概念操作状态::未找到) {
        out.状态 = 关系陈述状态::缺关系概念;
        return out;
    }
    if (概念读.状态 != 关系概念操作状态::已读取 || !概念读.信息) {
        out.状态 = 关系陈述状态::前提已变化;
        out.诊断 = "关系概念读取未完成";
        return out;
    }
    out.所用关系概念 = *概念读.信息;
    const auto& 定义 = 概念读.信息->定义;

    std::map<std::string, 关系端点对象种类> 端点种类表;
    for (const auto& 端 : 定义.端点组) {
        端点种类表.emplace(端.角色键, 端.对象种类);
        const auto it = 请求.端点绑定.find(端.角色键);
        if (it == 请求.端点绑定.end() || !有效(it->second)) {
            out.状态 = 关系陈述状态::端点不合法;
            out.诊断 = "缺端点:" + 端.角色键;
            return out;
        }
        if (端.对象种类 == 关系端点对象种类::存在) {
            if (!存在_.获取存在(it->second)) {
                out.状态 = 关系陈述状态::端点不合法;
                out.诊断 = "端点不是存在:" + 端.角色键;
                return out;
            }
        } else if (!需求_.获取需求(it->second)) {
            out.状态 = 关系陈述状态::端点不合法;
            out.诊断 = "端点不是需求:" + 端.角色键;
            return out;
        }
    }

    std::vector<关系连接描述> 待核验;
    待核验.push_back(定义.连接);
    for (const auto& a : 定义.附加连接组) 待核验.push_back(a);

    for (std::size_t i = 0; i < 待核验.size(); ++i) {
        bool 正向 = false, 反向 = false;
        if (!核验单连接(
                待核验[i], 请求.端点绑定, 存在_, 需求_, 正向, 反向, out.诊断)) {
            out.状态 = 关系陈述状态::范围未支持;
            if (i > 0) out.诊断 = "附加连接未接线:" + out.诊断;
            return out;
        }
        if (!正向) {
            out.状态 = (i == 0 && 反向) ? 关系陈述状态::方向或角色错误
                                        : 关系陈述状态::关系不成立;
            if (i > 0) out.诊断 = "附加连接不成立:" + out.诊断;
            else if (反向) out.诊断 = "端点方向与关系概念源/目标相反";
            else if (out.诊断.empty()) out.诊断 = "正式读未找到匹配边/字段";
            return out;
        }
    }

    const 关系概念表达关联* 表达 = nullptr;
    for (const auto& a : 定义.表达关联组) {
        if (a.语言节点 == 请求.目标语言节点) {
            表达 = &a;
            break;
        }
    }
    if (!表达 || 表达->含义操作码 == 0) {
        out.状态 = 关系陈述状态::缺语言知识;
        out.诊断 = "目标语言无关系表达关联（不暗退其它语言）";
        return out;
    }
    if (表达->角色槽组.size() < 2) {
        out.状态 = 关系陈述状态::缺语言知识;
        out.诊断 = "表达关联缺角色槽（须由知识数据提供，无专用构图回退）";
        return out;
    }

    const auto 操作 = static_cast<新语言知识含义操作>(表达->含义操作码);
    auto 允许根知识 = 解析表达根知识(
        *表达, 操作, 请求.目标语言节点, 语言_, 词_, 存在概念_, out.诊断);
    if (允许根知识.empty()) {
        out.状态 = 关系陈述状态::缺语言知识;
        return out;
    }

    std::set<std::string> 已用端点;
    std::set<std::int64_t> 已用参数;
    std::vector<std::pair<参数, 稳定编码>> 槽材料;
    槽材料.reserve(表达->角色槽组.size());
    for (const auto& 槽 : 表达->角色槽组) {
        if (槽.端点角色键.empty() || 槽.参数角色码 == 0
            || !已用端点.insert(槽.端点角色键).second
            || !已用参数.insert(槽.参数角色码).second) {
            out.状态 = 关系陈述状态::缺语言知识;
            out.诊断 = "表达角色槽重复或空键";
            return out;
        }
        const auto 种类It = 端点种类表.find(槽.端点角色键);
        const auto 绑定It = 请求.端点绑定.find(槽.端点角色键);
        if (种类It == 端点种类表.end() || 绑定It == 请求.端点绑定.end()) {
            out.状态 = 关系陈述状态::端点不合法;
            out.诊断 = "槽引用未知端点角色:" + 槽.端点角色键;
            return out;
        }
        auto 存在节点 = 取材存在节点(
            槽, 绑定It->second, 种类It->second, 存在_, 需求_, out.诊断);
        if (!存在节点) {
            out.状态 = (槽.投影 != 关系表达取材投影::端点身份
                            && 种类It->second == 关系端点对象种类::需求)
                ? 关系陈述状态::缺名称
                : 关系陈述状态::端点不合法;
            return out;
        }
        槽材料.push_back({static_cast<参数>(槽.参数角色码), *存在节点});
    }

    return 完成成文(
        请求, *表达,
        按表达槽构造含义图(请求.目标语言节点, 操作, 槽材料),
        std::move(允许根知识),
        语言_, 词_, 名称_, 存在概念_, 特征概念_, 存在_, 语言世界_,
        std::move(out));
}

} // namespace 海中鱼巣
