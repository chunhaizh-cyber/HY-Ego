#include "关系陈述.语言服务.h"
#include "../领域/关系概念类.h"
#include "../领域/新_存在类.h"
#include "../领域/新_需求类.h"
#include "../领域/新_自然语言世界树类.h"
#include "../领域/语言含义图.数据.h"
#include "解释.基础语言.h"

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

// 统一构图：含义操作与角色槽全部来自关系概念表达关联/陈述段知识，无每关系专用构图函数。
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

// 提问构图：信息请求(填项) 包一层关系操作；待询槽为查询变量，其余槽为已绑定存在。
// 槽材料里 稳定编码{} 表示该槽待询（不按关系种子写专用问句函数）。
语言图含义材料 按表达槽构造提问含义图(
    稳定编码 语言,
    新语言知识含义操作 操作,
    const std::vector<std::pair<参数, 稳定编码>>& 槽材料,
    稳定编码 待询内容范围概念) {
    语言图含义材料 图;
    图.语言节点 = 语言;
    新语言图含义节点 根;
    根.局部编号 = 1;
    根.操作 = 新语言知识含义操作::信息请求;
    根.作用域 = 新语言知识作用域::提问范围;
    根.确认范围 = 新语言知识确认范围::填项内容;
    根.角色组.push_back(角色定义(0, 种类::子含义引用, 类别::子含义));
    根.参数组.push_back({参数::被询内容, 0});
    根.子含义组.push_back({0, 2, 新语言子含义用途::被询内容, {}});

    新语言图含义节点 子;
    子.局部编号 = 2;
    子.操作 = 操作;
    子.作用域 = 新语言知识作用域::提问范围;
    语言图含义出现 子出现;
    子出现.出现编号 = 1;
    子出现.定义含义编号 = 2;
    子出现.有效作用域 = 新语言知识作用域::提问范围;
    for (std::size_t i = 0; i < 槽材料.size(); ++i) {
        const auto 编号 = static_cast<std::int64_t>(i);
        子.参数组.push_back({槽材料[i].first, 编号});
        if (!有效(槽材料[i].second)) {
            子.角色组.push_back(角色定义(编号, 种类::查询变量, 类别::查询存在));
            // 基础成文要求待求内容范围概念有效（查询变量词面「什么」）。
            子出现.角色组.push_back({编号, 语言图待求值{0, 待询内容范围概念}});
        } else {
            子.角色组.push_back(角色定义(编号, 种类::领域对象引用, 类别::存在));
            子出现.角色组.push_back(
                {编号, 语言图引用值{类别::存在, 槽材料[i].second, {}, {}}});
        }
    }

    语言图含义出现 根出现;
    根出现.出现编号 = 0;
    根出现.定义含义编号 = 1;
    根出现.有效作用域 = 新语言知识作用域::提问范围;
    根出现.角色组.push_back({0, 语言图子出现引用{1}});

    图.定义图.根含义编号 = 1;
    图.定义图.节点组.push_back(std::move(根));
    图.定义图.节点组.push_back(std::move(子));
    图.根出现编号 = 0;
    图.出现组.push_back(std::move(根出现));
    图.出现组.push_back(std::move(子出现));
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

// 根含义参数签名：须与陈述段角色槽参数集一致，避免「需要」同时命中目标存在句与单值扩展句。
std::optional<std::set<std::int64_t>> 知识根参数签名(
    const 新语言知识信息& 知识,
    新语言知识含义操作 操作) {
    std::set<std::int64_t> 码;
    if (知识.定义.目标.含义图) {
        for (const auto& 节点 : 知识.定义.目标.含义图->节点组) {
            if (节点.局部编号 != 知识.定义.目标.含义图->根含义编号) continue;
            if (节点.操作 != 操作) return std::nullopt;
            for (const auto& p : 节点.参数组)
                码.insert(static_cast<std::int64_t>(p.参数));
            return 码;
        }
        return std::nullopt;
    }
    if (知识.定义.目标.含义组.size() == 1
        && 知识.定义.目标.含义组.front().操作 == 操作) {
        for (const auto& p : 知识.定义.目标.含义组.front().参数组)
            码.insert(static_cast<std::int64_t>(p.参数));
        return 码;
    }
    return std::nullopt;
}

// 提问知识：根为信息请求，被询子含义/子图节点携带关系操作与槽参数签名。
std::optional<std::set<std::int64_t>> 提问知识槽参数签名(
    const 新语言知识信息& 知识,
    新语言知识含义操作 操作) {
    std::set<std::int64_t> 码;
    if (知识.定义.目标.含义图) {
        const auto& 图 = *知识.定义.目标.含义图;
        const 新语言图含义节点* 根 = nullptr;
        for (const auto& 节点 : 图.节点组) {
            if (节点.局部编号 == 图.根含义编号) {
                根 = &节点;
                break;
            }
        }
        if (!根 || 根->操作 != 新语言知识含义操作::信息请求
            || 根->子含义组.size() != 1)
            return std::nullopt;
        const auto 子号 = 根->子含义组.front().子含义编号;
        for (const auto& 节点 : 图.节点组) {
            if (节点.局部编号 != 子号) continue;
            if (节点.操作 != 操作) return std::nullopt;
            for (const auto& p : 节点.参数组)
                码.insert(static_cast<std::int64_t>(p.参数));
            return 码;
        }
        return std::nullopt;
    }
    for (const auto& 项 : 知识.定义.目标.含义组) {
        if (项.操作 != 新语言知识含义操作::信息请求 || !项.被询含义)
            continue;
        if (项.被询含义->操作 != 操作) return std::nullopt;
        for (const auto& p : 项.被询含义->参数组)
            码.insert(static_cast<std::int64_t>(p.参数));
        return 码;
    }
    return std::nullopt;
}

std::optional<std::set<std::int64_t>> 按目的取知识槽签名(
    const 新语言知识信息& 知识,
    新语言知识含义操作 操作,
    新语言知识表达目的 目的) {
    if (目的 == 新语言知识表达目的::提问)
        return 提问知识槽参数签名(知识, 操作);
    return 知识根参数签名(知识, 操作);
}

// 解析一段要消费的根知识：后天节点优先；否则按预置尾词+含义操作+槽参数签名解析。
// 已挂表达知识节点组时不得静默回退预置尾词/其它模式。
std::vector<稳定编码> 解析段根知识(
    const std::vector<稳定编码>& 表达知识节点组,
    const std::string& 预置知识尾词,
    新语言知识含义操作 操作,
    const std::set<std::int64_t>& 期望参数码,
    新语言知识表达目的 目的,
    稳定编码 语言节点,
    const 新_语言类& 语言,
    const 新_词类& 词,
    const 概念_存在类& 存在概念,
    std::string& 诊断) {
    if (!表达知识节点组.empty()) {
        for (const auto n : 表达知识节点组) {
            if (!有效(n)) {
                诊断 = "表达知识节点无效";
                return {};
            }
            const auto 读 = 语言.获取语言知识(n, 词, 存在概念);
            if (读.状态 != 新语言知识状态::已完成 || !读.信息) {
                诊断 = "表达知识节点不可读";
                return {};
            }
            if (读.信息->定义.目标.目的 != 目的) {
                诊断 = "表达知识目的与出站用途不一致（不把陈述当问句）";
                return {};
            }
            const auto 签名 = 按目的取知识槽签名(*读.信息, 操作, 目的);
            if (!签名 || *签名 != 期望参数码) {
                诊断 = "表达知识含义操作/参数与陈述段不一致（不静默换模式）";
                return {};
            }
        }
        return 表达知识节点组;
    }
    if (预置知识尾词.empty()) {
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
        if (信息.定义.目标.目的 != 目的) continue;
        const auto 签名 = 按目的取知识槽签名(信息, 操作, 目的);
        if (!签名 || *签名 != 期望参数码) continue;
        if (知识含固定尾词(信息, 预置知识尾词, 词)) 命中.push_back(信息.节点);
    }
    if (命中.empty()) {
        诊断 = "预置尾词未命中语言知识:" + 预置知识尾词;
        return {};
    }
    if (命中.size() > 1) {
        诊断 = "预置尾词命中多条语言知识:" + 预置知识尾词;
        return {};
    }
    return 命中;
}

关系陈述结果 完成单段成文(
    const 关系陈述请求& 请求,
    bool 预置,
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
    out.所用表达为预置 = 预置;
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
    case 语言场景基础输出状态::输入拒绝:
        out.状态 = 关系陈述状态::输入拒绝;
        out.诊断 = "统一成文输入拒绝";
        return out;
    default:
        out.状态 = 关系陈述状态::范围未支持;
        out.诊断 = "统一成文状态=" + std::to_string(static_cast<int>(输出.状态));
        return out;
    }
}

关系陈述结果 成文一段(
    const 关系陈述请求& 请求,
    const 关系表达陈述段& 段,
    bool 预置,
    const std::map<std::string, 关系端点对象种类>& 端点种类表,
    const 新_语言类& 语言,
    const 新_词类& 词,
    const 概念_名称关系类& 名称,
    const 概念_存在类& 存在概念,
    const 概念_特征类& 特征概念,
    const 新_存在类& 存在,
    const 新_需求类& 需求,
    const 新_自然语言世界树类& 语言世界,
    关系陈述结果 out) {
    if (段.含义操作码 == 0 || 段.角色槽组.size() < 2) {
        out.状态 = 关系陈述状态::缺语言知识;
        out.诊断 = "陈述段缺含义操作或角色槽";
        return out;
    }
    const auto 操作 = static_cast<新语言知识含义操作>(段.含义操作码);

    std::set<std::string> 已用端点;
    std::set<std::int64_t> 已用参数;
    std::set<std::int64_t> 期望参数码;
    std::vector<std::pair<参数, 稳定编码>> 槽材料;
    槽材料.reserve(段.角色槽组.size());
    for (const auto& 槽 : 段.角色槽组) {
        if (槽.端点角色键.empty() || 槽.参数角色码 == 0
            || !已用端点.insert(槽.端点角色键).second
            || !已用参数.insert(槽.参数角色码).second) {
            out.状态 = 关系陈述状态::缺语言知识;
            out.诊断 = "表达角色槽重复或空键";
            return out;
        }
        期望参数码.insert(槽.参数角色码);
        const auto 种类It = 端点种类表.find(槽.端点角色键);
        if (种类It == 端点种类表.end()) {
            out.状态 = 关系陈述状态::端点不合法;
            out.诊断 = "槽引用未知端点角色:" + 槽.端点角色键;
            return out;
        }
        const bool 本槽待询 = 请求.表达目的 == 新语言知识表达目的::提问
            && 请求.待询端点角色
            && *请求.待询端点角色 == 槽.端点角色键;
        if (本槽待询) {
            // 待询端点不取材、不绑定；槽位以无效编码进入提问构图。
            槽材料.push_back({static_cast<参数>(槽.参数角色码), {}});
            continue;
        }
        const auto 绑定It = 请求.端点绑定.find(槽.端点角色键);
        if (绑定It == 请求.端点绑定.end()) {
            out.状态 = 关系陈述状态::端点不合法;
            out.诊断 = "槽引用未知端点角色:" + 槽.端点角色键;
            return out;
        }
        auto 存在节点 = 取材存在节点(
            槽, 绑定It->second, 种类It->second, 存在, 需求, out.诊断);
        if (!存在节点) {
            out.状态 = (槽.投影 != 关系表达取材投影::端点身份
                            && 种类It->second == 关系端点对象种类::需求)
                ? 关系陈述状态::缺名称
                : 关系陈述状态::端点不合法;
            return out;
        }
        槽材料.push_back({static_cast<参数>(槽.参数角色码), *存在节点});
    }

    const auto 目的 = 请求.表达目的;
    if (目的 != 新语言知识表达目的::陈述
        && 目的 != 新语言知识表达目的::提问) {
        out.状态 = 关系陈述状态::范围未支持;
        out.诊断 = "关系出站仅支持陈述或提问目的";
        return out;
    }
    auto 允许根知识 = 解析段根知识(
        段.表达知识节点组, 段.预置知识尾词, 操作, 期望参数码, 目的,
        请求.目标语言节点, 语言, 词, 存在概念, out.诊断);
    if (允许根知识.empty()) {
        out.状态 = 关系陈述状态::缺语言知识;
        return out;
    }

    auto 图 = 目的 == 新语言知识表达目的::提问
        ? 按表达槽构造提问含义图(
            请求.目标语言节点, 操作, 槽材料, 请求.待询内容范围概念)
        : 按表达槽构造含义图(请求.目标语言节点, 操作, 槽材料);
    return 完成单段成文(
        请求, 预置, std::move(图), std::move(允许根知识),
        语言, 词, 名称, 存在概念, 特征概念, 存在, 语言世界,
        std::move(out));
}

} // namespace

关系陈述语言服务::关系陈述语言服务(
    关系概念类& 关系,
    const 新_语言类& 语言,
    const 新_词类& 词,
    const 概念_名称关系类& 名称,
    const 概念_存在类& 存在概念,
    const 概念_特征类& 特征概念,
    const 新_存在类& 存在,
    const 新_需求类& 需求,
    新_自然语言世界树类& 语言世界) noexcept
    : 关系_(关系), 语言_(语言), 词_(词), 名称_(名称), 存在概念_(存在概念),
      特征概念_(特征概念), 存在_(存在), 需求_(需求), 语言世界_(语言世界) {}

namespace {
bool 连接引用端点角色(const 关系连接描述& 连接, const std::string& 角色) {
    if (const auto* 边 = std::get_if<关系外部边连接>(&连接))
        return 边->源角色键 == 角色 || 边->目标角色键 == 角色;
    if (const auto* 字段 = std::get_if<关系字段引用连接>(&连接))
        return 字段->宿主角色键 == 角色 || 字段->目标角色键 == 角色;
    return false;
}
} // namespace

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
    if (请求.表达目的 != 新语言知识表达目的::陈述
        && 请求.表达目的 != 新语言知识表达目的::提问) {
        out.状态 = 关系陈述状态::范围未支持;
        out.诊断 = "关系出站仅支持陈述或提问目的";
        return out;
    }
    if (请求.表达目的 == 新语言知识表达目的::提问) {
        if (!请求.待询端点角色 || 请求.待询端点角色->empty()) {
            out.状态 = 关系陈述状态::输入拒绝;
            out.诊断 = "提问须指定待询端点角色";
            return out;
        }
        if (请求.端点绑定.contains(*请求.待询端点角色)) {
            out.状态 = 关系陈述状态::输入拒绝;
            out.诊断 = "待询端点不得同时绑定正式节点";
            return out;
        }
        if (!有效(请求.待询内容范围概念)) {
            out.状态 = 关系陈述状态::输入拒绝;
            out.诊断 = "提问须提供待询内容范围概念";
            return out;
        }
    } else if (请求.待询端点角色) {
        out.状态 = 关系陈述状态::输入拒绝;
        out.诊断 = "陈述不得携带待询端点";
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
        const bool 本端待询 = 请求.表达目的 == 新语言知识表达目的::提问
            && 请求.待询端点角色 && *请求.待询端点角色 == 端.角色键;
        if (本端待询) continue;
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
    if (请求.表达目的 == 新语言知识表达目的::提问
        && !端点种类表.contains(*请求.待询端点角色)) {
        out.状态 = 关系陈述状态::端点不合法;
        out.诊断 = "待询端点不是关系概念角色:" + *请求.待询端点角色;
        return out;
    }

    // 主连接=世界事实；附加连接=适用条件（须成立，默认不进入陈述段成句）。
    // 提问：跳过引用待询端点的连接核验（未知项不成事实）。
    std::vector<关系连接描述> 待核验;
    待核验.push_back(定义.连接);
    for (const auto& a : 定义.附加连接组) 待核验.push_back(a);

    for (std::size_t i = 0; i < 待核验.size(); ++i) {
        if (请求.表达目的 == 新语言知识表达目的::提问
            && 请求.待询端点角色
            && 连接引用端点角色(待核验[i], *请求.待询端点角色))
            continue;
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
    if (!表达) {
        out.状态 = 关系陈述状态::缺语言知识;
        out.诊断 = "目标语言无关系表达关联（不暗退其它语言）";
        return out;
    }

    std::vector<关系表达陈述段> 段组;
    if (!表达->陈述段组.empty()) {
        段组 = 表达->陈述段组;
    } else {
        if (表达->含义操作码 == 0 || 表达->角色槽组.size() < 2) {
            out.状态 = 关系陈述状态::缺语言知识;
            out.诊断 = "表达关联缺含义操作或角色槽（须由知识数据提供，无专用构图回退）";
            return out;
        }
        关系表达陈述段 单;
        单.含义操作码 = 表达->含义操作码;
        单.预置知识尾词 = 表达->预置知识尾词;
        单.表达知识节点组 = 表达->表达知识节点组;
        单.角色槽组 = 表达->角色槽组;
        段组.push_back(std::move(单));
    }

    std::string 组合正文;
    std::vector<稳定编码> 组合知识;
    for (std::size_t i = 0; i < 段组.size(); ++i) {
        auto 段结果 = 成文一段(
            请求, 段组[i], 表达->预置, 端点种类表,
            语言_, 词_, 名称_, 存在概念_, 特征概念_, 存在_, 需求_, 语言世界_,
            out);
        if (段结果.状态 != 关系陈述状态::已生成且唯一回读) {
            if (段组.size() > 1 && 段结果.诊断.empty())
                段结果.诊断 = "组合陈述第" + std::to_string(i + 1) + "段失败";
            else if (段组.size() > 1)
                段结果.诊断 = "组合陈述第" + std::to_string(i + 1) + "段:"
                    + 段结果.诊断;
            return 段结果;
        }
        if (!组合正文.empty()) 组合正文 += "，";
        组合正文 += 段结果.UTF8正文;
        for (const auto k : 段结果.所用知识节点组) 组合知识.push_back(k);
    }

    out.状态 = 关系陈述状态::已生成且唯一回读;
    out.UTF8正文 = std::move(组合正文);
    out.所用知识节点组 = std::move(组合知识);
    out.所用表达为预置 = 表达->预置;
    out.诊断.clear();
    return out;
}

namespace {

struct 入站匹配段 final {
    std::string 种子键;
    关系表达陈述段 段;
    bool 预置 = true;
    // 该种子表达关联展开后的陈述段总数；>1 表示组合种子中的一段。
    std::size_t 种子陈述段数 = 1;
};

// 用与出站相同的表达知识解析，把解释命中的知识节点反查到关系种子+陈述段。
std::vector<入站匹配段> 按知识匹配关系段(
    稳定编码 知识节点,
    稳定编码 语言节点,
    const 关系概念类& 关系,
    const 新_语言类& 语言,
    const 新_词类& 词,
    const 概念_存在类& 存在概念,
    std::string& 诊断) {
    std::vector<入站匹配段> 命中;
    const auto 列表 = 关系.列出全部();
    if (列表.状态 != 关系概念操作状态::已读取) {
        诊断 = "关系概念列表不可读";
        return {};
    }
    for (const auto& 信息 : 列表.信息组) {
        const 关系概念表达关联* 表达 = nullptr;
        for (const auto& a : 信息.定义.表达关联组) {
            if (a.语言节点 == 语言节点) {
                表达 = &a;
                break;
            }
        }
        if (!表达) continue;
        std::vector<关系表达陈述段> 段组;
        if (!表达->陈述段组.empty()) 段组 = 表达->陈述段组;
        else {
            关系表达陈述段 单;
            单.含义操作码 = 表达->含义操作码;
            单.预置知识尾词 = 表达->预置知识尾词;
            单.表达知识节点组 = 表达->表达知识节点组;
            单.角色槽组 = 表达->角色槽组;
            段组.push_back(std::move(单));
        }
        const auto 段数 = 段组.size();
        for (const auto& 段 : 段组) {
            if (段.含义操作码 == 0 || 段.角色槽组.size() < 2) continue;
            std::set<std::int64_t> 期望;
            for (const auto& 槽 : 段.角色槽组) 期望.insert(槽.参数角色码);
            std::string 段诊;
            auto 根 = 解析段根知识(
                段.表达知识节点组, 段.预置知识尾词,
                static_cast<新语言知识含义操作>(段.含义操作码), 期望,
                新语言知识表达目的::陈述,
                语言节点, 语言, 词, 存在概念, 段诊);
            if (根.empty()) continue;
            if (std::find(根.begin(), 根.end(), 知识节点) == 根.end()) continue;
            命中.push_back({信息.定义.稳定种子键, 段, 表达->预置, 段数});
        }
    }
    return 命中;
}

std::optional<稳定编码> 图角色存在节点(
    const 语言图含义材料& 图, std::int64_t 参数角色码) {
    const auto 根 = std::find_if(
        图.出现组.begin(), 图.出现组.end(),
        [&](const auto& a) { return a.出现编号 == 图.根出现编号; });
    if (根 == 图.出现组.end()) return std::nullopt;
    const auto 定义 = std::find_if(
        图.定义图.节点组.begin(), 图.定义图.节点组.end(),
        [&](const auto& n) { return n.局部编号 == 根->定义含义编号; });
    if (定义 == 图.定义图.节点组.end()) return std::nullopt;
    std::optional<std::int64_t> 角色编号;
    for (const auto& p : 定义->参数组) {
        if (static_cast<std::int64_t>(p.参数) == 参数角色码) {
            角色编号 = p.规则角色;
            break;
        }
    }
    if (!角色编号) return std::nullopt;
    for (const auto& b : 根->角色组) {
        if (b.角色编号 != *角色编号) continue;
        const auto* 引用 = std::get_if<语言图引用值>(&b.值);
        if (!引用) return std::nullopt;
        if (引用->类别 == 新语言知识角色类别::存在 && 有效(引用->节点))
            return 引用->节点;
        return std::nullopt;
    }
    return std::nullopt;
}

std::optional<稳定编码> 图角色来源概念(
    const 语言图含义材料& 图, std::int64_t 参数角色码) {
    const auto 根 = std::find_if(
        图.出现组.begin(), 图.出现组.end(),
        [&](const auto& a) { return a.出现编号 == 图.根出现编号; });
    if (根 == 图.出现组.end()) return std::nullopt;
    const auto 定义 = std::find_if(
        图.定义图.节点组.begin(), 图.定义图.节点组.end(),
        [&](const auto& n) { return n.局部编号 == 根->定义含义编号; });
    if (定义 == 图.定义图.节点组.end()) return std::nullopt;
    std::optional<std::int64_t> 角色编号;
    for (const auto& p : 定义->参数组) {
        if (static_cast<std::int64_t>(p.参数) == 参数角色码) {
            角色编号 = p.规则角色;
            break;
        }
    }
    if (!角色编号) return std::nullopt;
    for (const auto& b : 根->角色组) {
        if (b.角色编号 != *角色编号) continue;
        const auto* 引用 = std::get_if<语言图引用值>(&b.值);
        if (!引用) return std::nullopt;
        if (引用->类别 == 新语言知识角色类别::存在概念 && 有效(引用->节点))
            return 引用->节点;
        if (引用->来源概念 && 有效(*引用->来源概念)) return *引用->来源概念;
        return std::nullopt;
    }
    return std::nullopt;
}

} // namespace

关系陈述理解结果 关系陈述语言服务::理解关系陈述(
    const 关系陈述理解请求& 请求) {
    关系陈述理解结果 out;
    if (请求.UTF8正文.empty() || !有效(请求.目标语言节点)) {
        out.状态 = 关系陈述理解状态::输入拒绝;
        out.诊断 = "缺正文或目标语言";
        return out;
    }
    if (!语言_.是语言节点(请求.目标语言节点)) {
        out.状态 = 关系陈述理解状态::输入拒绝;
        out.诊断 = "目标语言不是语言节点";
        return out;
    }

    语言图解释请求 图请求;
    图请求.语言节点 = 请求.目标语言节点;
    图请求.固定原文 = 请求.UTF8正文;
    图请求.对象绑定组 = 请求.对象绑定组;
    图请求.专名绑定读取 = 请求.专名绑定读取;
    图请求.交互上下文 = 请求.交互上下文;
    图请求.读取模式 = 语言解释读取模式::显式额度;

    const auto 图解释 = 解释单句含义图(
        图请求, 语言_, 词_, 名称_, 存在概念_, 特征概念_, 存在_);
    if (图解释.状态 == 基础语言输入状态::概念多义待解
        || 图解释.状态 == 基础语言输入状态::多规则待解
        || 图解释.完整候选组.size() > 1) {
        out.状态 = 关系陈述理解状态::多义待解;
        out.诊断 = "解释多义，不任取候选";
        return out;
    }
    if (图解释.状态 == 基础语言输入状态::缺名称) {
        out.状态 = 关系陈述理解状态::缺名称;
        return out;
    }
    if (图解释.状态 == 基础语言输入状态::读取漂移) {
        out.状态 = 关系陈述理解状态::前提已变化;
        out.诊断 = "解释读取漂移";
        return out;
    }
    if (图解释.状态 == 基础语言输入状态::内部不一致
        || 图解释.状态 == 基础语言输入状态::引用不可读
        || 图解释.状态 == 基础语言输入状态::资源失败
        || 图解释.状态 == 基础语言输入状态::工程未完成) {
        out.状态 = 关系陈述理解状态::范围未支持;
        out.诊断 = "解释工程状态="
            + std::to_string(static_cast<int>(图解释.状态));
        return out;
    }
    if (图解释.状态 == 基础语言输入状态::缺规则
        || 图解释.完整候选组.empty()) {
        out.状态 = 关系陈述理解状态::缺语言知识;
        out.诊断 = "含义图未形成唯一候选";
        return out;
    }
    if (图解释.状态 != 基础语言输入状态::已形成唯一含义
        && 图解释.状态 != 基础语言输入状态::主体待绑定) {
        out.状态 = 关系陈述理解状态::范围未支持;
        out.诊断 = "解释状态=" + std::to_string(static_cast<int>(图解释.状态));
        return out;
    }
    const auto& 图 = 图解释.完整候选组.front();
    const auto 根 = std::find_if(
        图.出现组.begin(), 图.出现组.end(),
        [&](const auto& a) { return a.出现编号 == 图.根出现编号; });
    if (根 == 图.出现组.end() || !有效(根->所用知识节点)) {
        out.状态 = 关系陈述理解状态::缺语言知识;
        out.诊断 = "根出现缺所用知识";
        return out;
    }

    auto 匹配 = 按知识匹配关系段(
        根->所用知识节点, 请求.目标语言节点, 关系_, 语言_, 词_, 存在概念_,
        out.诊断);
    if (匹配.empty()) {
        out.状态 = 关系陈述理解状态::缺语言知识;
        if (out.诊断.empty()) out.诊断 = "所用知识未匹配任何关系表达段";
        return out;
    }
    // 同知识命中多种子：先收窄到「表达仅含该命中段」的单段种子，
    // 避免组合种子（如 part_under_whole…）因共享「一部分/要」段而误占单句入站。
    std::set<std::string> 种子集;
    for (const auto& m : 匹配) 种子集.insert(m.种子键);
    if (种子集.size() > 1) {
        std::vector<入站匹配段> 单段;
        for (const auto& m : 匹配) {
            if (m.种子陈述段数 == 1) 单段.push_back(m);
        }
        if (!单段.empty()) {
            匹配 = std::move(单段);
            种子集.clear();
            for (const auto& m : 匹配) 种子集.insert(m.种子键);
        }
    }
    if (种子集.size() > 1) {
        out.状态 = 关系陈述理解状态::多义待解;
        out.诊断 = "同一知识命中多个关系种子，不任取";
        return out;
    }
    const auto& 选 = 匹配.front();

    关系陈述入站材料 材料;
    材料.关系种子键 = 选.种子键;
    材料.固定原文 = 请求.UTF8正文;
    材料.交互场景 = 请求.交互场景;
    材料.所用知识节点组 = {根->所用知识节点};

    for (const auto& 槽 : 选.段.角色槽组) {
        关系陈述入站角色材料 角色;
        角色.端点角色键 = 槽.端点角色键;
        角色.所指存在 = 图角色存在节点(图, 槽.参数角色码);
        角色.来源概念 = 图角色来源概念(图, 槽.参数角色码);
        if (!角色.所指存在 && !角色.来源概念) {
            角色.未确定 = true;
        }
        // 入站忠实记录所说角色；不按世界边「纠正」方向。
        材料.角色组.push_back(std::move(角色));
    }

    const auto 场景 = 语言世界_.建立空场景();
    if (场景.状态 != 新自然语言场景操作状态::已建立 || !场景.场景节点) {
        out.状态 = 关系陈述理解状态::场景建立未完成;
        out.诊断 = "自然语言空场景未建立";
        return out;
    }
    材料.自然语言场景 = *场景.场景节点;

    for (auto& 角色 : 材料.角色组) {
        稳定编码 概念{};
        if (角色.来源概念) 概念 = *角色.来源概念;
        else if (角色.所指存在) {
            const auto 专 = 存在_.查询专属存在概念(*角色.所指存在);
            if (专) 概念 = *专;
        }
        if (!有效(概念)) continue;
        if (!角色.来源概念) 角色.来源概念 = 概念;
        const auto 实例 = 语言世界_.实例化概念(材料.自然语言场景, 概念);
        if (实例.状态 != 新自然语言概念实例操作状态::已建立 || !实例.实例) {
            // 专属夹具概念（如需求主体存在）可能带不可实例化特征；
            // 入站材料仍保留所指存在/来源概念与空场景键，不因单角色实例化失败整单作废。
            continue;
        }
        角色.场景实例 = 实例.实例->节点;
    }

    const auto 保存 = 关系_.建立入站材料(材料);
    if (保存.状态 != 关系概念操作状态::已建立
        && 保存.状态 != 关系概念操作状态::已复用) {
        out.状态 = 关系陈述理解状态::材料保存失败;
        out.诊断 = "入站材料保存状态="
            + std::to_string(static_cast<int>(保存.状态));
        return out;
    }
    if (!保存.信息) {
        out.状态 = 关系陈述理解状态::材料保存失败;
        out.诊断 = "入站材料保存无信息";
        return out;
    }
    out.材料 = std::move(*保存.信息);
    out.状态 = 关系陈述理解状态::已形成场景材料;
    out.诊断.clear();
    return out;
}

} // namespace 海中鱼巣
