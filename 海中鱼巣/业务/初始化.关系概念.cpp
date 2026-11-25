#include "初始化.关系概念.h"

#include <algorithm>
#include <functional>
#include <sstream>
#include <stdexcept>

namespace 海中鱼巣 {
namespace {

std::int64_t 含义操作码(const std::string& s) {
    if (s == "组成描述")
        return static_cast<std::int64_t>(新语言知识含义操作::组成描述);
    if (s == "组织隶属")
        return static_cast<std::int64_t>(新语言知识含义操作::组织隶属);
    if (s == "概念归类")
        return static_cast<std::int64_t>(新语言知识含义操作::概念归类);
    if (s == "特征描述")
        return static_cast<std::int64_t>(新语言知识含义操作::特征描述);
    if (s == "需求描述")
        return static_cast<std::int64_t>(新语言知识含义操作::需求描述);
    return 0;
}

std::int64_t 参数角色码(const std::string& s) {
    if (s == "整体") return static_cast<std::int64_t>(新语言知识参数角色::整体);
    if (s == "部分") return static_cast<std::int64_t>(新语言知识参数角色::部分);
    if (s == "上级") return static_cast<std::int64_t>(新语言知识参数角色::上级);
    if (s == "下级") return static_cast<std::int64_t>(新语言知识参数角色::下级);
    if (s == "主体") return static_cast<std::int64_t>(新语言知识参数角色::主体);
    if (s == "目标存在") return static_cast<std::int64_t>(新语言知识参数角色::目标存在);
    return 0;
}

struct 种子包 final {
    关系概念定义 定义;
    std::vector<std::string> 名称词面组;
};

关系连接描述 解析连接行(
    const std::vector<std::string>& t,
    std::size_t 种类下标,
    const std::function<void(bool)>& 要求) {
    要求(t.size() > 种类下标);
    if (t[种类下标] == "外部边") {
        关系外部边连接 e;
        for (std::size_t i = 种类下标 + 1; i < t.size(); ++i) {
            const auto p = t[i].find('=');
            要求(p != std::string::npos);
            const auto k = t[i].substr(0, p);
            const auto v = t[i].substr(p + 1);
            if (k == "owner") e.owner标识 = v;
            else if (k == "边") {
                要求(v == "父子" || v == "兄弟");
                e.边类型 = v == "父子" ? 基础外部关系类型::父子
                                      : 基础外部关系类型::兄弟;
            } else if (k == "角色") e.角色或顺序 = std::stoll(v);
            else if (k == "源") e.源角色键 = v;
            else if (k == "目标") e.目标角色键 = v;
            else 要求(false);
        }
        要求(!e.owner标识.empty() && !e.源角色键.empty() && !e.目标角色键.empty());
        return e;
    }
    要求(t[种类下标] == "字段引用");
    关系字段引用连接 f;
    for (std::size_t i = 种类下标 + 1; i < t.size(); ++i) {
        const auto p = t[i].find('=');
        要求(p != std::string::npos);
        const auto k = t[i].substr(0, p);
        const auto v = t[i].substr(p + 1);
        if (k == "owner") f.owner标识 = v;
        else if (k == "字段") f.字段键名 = v;
        else if (k == "宿主") f.宿主角色键 = v;
        else if (k == "目标") f.目标角色键 = v;
        else 要求(false);
    }
    要求(!f.owner标识.empty() && !f.字段键名.empty()
        && !f.宿主角色键.empty() && !f.目标角色键.empty());
    return f;
}

std::vector<种子包> 解码种子包(
    const std::string& 文本,
    稳定编码 中文,
    std::optional<稳定编码> 英文) {
    std::istringstream 流(文本);
    std::string 行;
    std::vector<std::vector<std::string>> 行组;
    while (std::getline(流, 行)) {
        if (!行.empty() && 行.back() == '\r') 行.pop_back();
        if (行组.empty() && 行.starts_with("\xEF\xBB\xBF")) 行.erase(0, 3);
        if (行.empty() || 行[0] == '#') continue;
        std::istringstream 行流(行);
        std::vector<std::string> 词;
        std::string w;
        while (行流 >> w) 词.push_back(std::move(w));
        if (!词.empty()) 行组.push_back(std::move(词));
    }
    const auto 要求 = [](bool ok) {
        if (!ok) throw std::logic_error("世界关系概念种子语法拒绝");
    };
    std::vector<种子包> out;
    std::optional<种子包> 当前;
    for (const auto& t : 行组) {
        if (t[0] == "概念") {
            要求(t.size() == 2 && !当前);
            当前.emplace();
            当前->定义.稳定种子键 = t[1];
            continue;
        }
        要求(当前.has_value());
        auto& d = 当前->定义;
        if (t[0] == "端点") {
            要求(t.size() == 3);
            关系端点约束 e;
            e.角色键 = t[1];
            if (t[2] == "存在") e.对象种类 = 关系端点对象种类::存在;
            else {
                要求(t[2] == "需求");
                e.对象种类 = 关系端点对象种类::需求;
            }
            d.端点组.push_back(std::move(e));
        } else if (t[0] == "连接") {
            要求(t.size() >= 2);
            d.连接 = 解析连接行(t, 1, 要求);
        } else if (t[0] == "附加连接") {
            要求(t.size() >= 2);
            d.附加连接组.push_back(解析连接行(t, 1, 要求));
        } else if (t[0] == "名称") {
            要求(t.size() == 3);
            关系概念语言名称 n;
            if (t[1] == "zh") {
                要求(有效(中文));
                n.语言节点 = 中文;
            } else {
                要求(t[1] == "en" && 英文 && 有效(*英文));
                n.语言节点 = *英文;
            }
            n.预置 = true;
            d.名称组.push_back(std::move(n));
            当前->名称词面组.push_back(t[2]);
        } else if (t[0] == "表达") {
            要求(t.size() >= 3);
            关系概念表达关联 a;
            if (t[1] == "zh") {
                要求(有效(中文));
                a.语言节点 = 中文;
            } else {
                要求(t[1] == "en" && 英文 && 有效(*英文));
                a.语言节点 = *英文;
            }
            // 「组合」= 多陈述段入口；含义操作由各陈述段提供。
            if (t[2] == "组合") a.含义操作码 = 0;
            else {
                a.含义操作码 = 含义操作码(t[2]);
                要求(a.含义操作码 != 0);
            }
            a.预置 = t.size() < 4 || t[3] == "预置";
            d.表达关联组.push_back(std::move(a));
        } else if (t[0] == "陈述段") {
            要求(!d.表达关联组.empty());
            要求(t.size() >= 2);
            关系表达陈述段 段;
            段.含义操作码 = 含义操作码(t[1]);
            要求(段.含义操作码 != 0);
            d.表达关联组.back().陈述段组.push_back(std::move(段));
        } else if (t[0] == "知识选") {
            要求(!d.表达关联组.empty());
            要求(t.size() >= 2);
            auto& 表 = d.表达关联组.back();
            std::string* 尾词目标 = nullptr;
            if (!表.陈述段组.empty()) 尾词目标 = &表.陈述段组.back().预置知识尾词;
            else 尾词目标 = &表.预置知识尾词;
            for (std::size_t i = 1; i < t.size(); ++i) {
                const auto p = t[i].find('=');
                要求(p != std::string::npos);
                const auto k = t[i].substr(0, p);
                const auto v = t[i].substr(p + 1);
                if (k == "尾词") {
                    要求(!v.empty());
                    *尾词目标 = v;
                } else 要求(false);
            }
            要求(!尾词目标->empty());
        } else if (t[0] == "槽") {
            要求(!d.表达关联组.empty());
            要求(t.size() >= 2);
            关系表达角色槽 槽;
            槽.端点角色键 = t[1];
            for (std::size_t i = 2; i < t.size(); ++i) {
                const auto p = t[i].find('=');
                要求(p != std::string::npos);
                const auto k = t[i].substr(0, p);
                const auto v = t[i].substr(p + 1);
                if (k == "参数") {
                    槽.参数角色码 = 参数角色码(v);
                    要求(槽.参数角色码 != 0);
                } else if (k == "投影") {
                    if (v == "端点") 槽.投影 = 关系表达取材投影::端点身份;
                    else if (v == "需求目标存在")
                        槽.投影 = 关系表达取材投影::需求目标存在;
                    else {
                        要求(v == "需求主体存在");
                        槽.投影 = 关系表达取材投影::需求主体存在;
                    }
                } else 要求(false);
            }
            要求(槽.参数角色码 != 0 && !槽.端点角色键.empty());
            auto& 表 = d.表达关联组.back();
            if (!表.陈述段组.empty()) 表.陈述段组.back().角色槽组.push_back(std::move(槽));
            else 表.角色槽组.push_back(std::move(槽));
        } else if (t[0] == "结束") {
            要求(t.size() == 1);
            要求(!d.稳定种子键.empty() && d.端点组.size() >= 2
                && !d.连接.valueless_by_exception());
            要求(d.名称组.size() == 当前->名称词面组.size());
            for (const auto& a : d.表达关联组) {
                if (!a.陈述段组.empty()) {
                    要求(a.含义操作码 == 0);
                    for (const auto& 段 : a.陈述段组) {
                        要求(段.含义操作码 != 0);
                        要求(段.角色槽组.size() >= 2);
                        要求(!段.预置知识尾词.empty() || !段.表达知识节点组.empty());
                    }
                } else {
                    要求(a.含义操作码 != 0);
                    要求(a.角色槽组.size() >= 2);
                    要求(!a.预置知识尾词.empty() || !a.表达知识节点组.empty());
                }
            }
            out.push_back(std::move(*当前));
            当前.reset();
        } else 要求(false);
    }
    要求(!当前.has_value());
    要求(!out.empty());
    return out;
}

} // namespace

初始化关系概念结果 初始化世界关系概念(
    const 初始化关系概念请求& 请求,
    关系概念类& 关系,
    新_词类& 词) {
    初始化关系概念结果 out;
    if (!有效(请求.中文语言节点) || 请求.种子文本.empty()) {
        out.状态 = 初始化关系概念状态::前置拒绝;
        out.诊断 = "缺少中文语言或种子文本";
        return out;
    }
    if (!关系.初始化()) {
        out.状态 = 初始化关系概念状态::前置拒绝;
        out.诊断 = "关系概念登记未就绪";
        return out;
    }
    std::vector<种子包> 包;
    try {
        包 = 解码种子包(请求.种子文本, 请求.中文语言节点, 请求.英文语言节点);
    } catch (const std::exception& e) {
        out.状态 = 初始化关系概念状态::语法拒绝;
        out.诊断 = e.what();
        return out;
    }
    for (auto& p : 包) {
        for (std::size_t i = 0; i < p.定义.名称组.size(); ++i) {
            const auto 词条 = 词.建立或取得词条({
                p.定义.名称组[i].语言节点,
                严格UTF8字节精确规范化合同节点,
                p.名称词面组[i],
                std::nullopt});
            if (!词条.词条节点) {
                out.状态 = 初始化关系概念状态::写入失败;
                out.诊断 = "预置名称词条建立失败:" + p.名称词面组[i];
                return out;
            }
            p.定义.名称组[i].词条节点 = *词条.词条节点;
        }
        const auto 建立 = 关系.建立或取得关系概念(p.定义);
        if (建立.状态 != 关系概念操作状态::已建立
            && 建立.状态 != 关系概念操作状态::已复用) {
            out.状态 = 初始化关系概念状态::写入失败;
            out.诊断 = "建立关系概念失败:" + p.定义.稳定种子键
                + " status=" + std::to_string(static_cast<int>(建立.状态));
            return out;
        }
        if (!建立.信息) {
            out.状态 = 初始化关系概念状态::读回失败;
            return out;
        }
        out.项组.push_back(*建立.信息);
    }
    out.状态 = 初始化关系概念状态::已完成;
    return out;
}

初始化关系概念结果 读取世界关系概念种子(
    const 初始化关系概念请求& 请求,
    const 关系概念类& 关系,
    const 新_词类& 词) {
    初始化关系概念结果 out;
    (void)词;
    if (!关系.已装配()) {
        out.状态 = 初始化关系概念状态::前置拒绝;
        return out;
    }
    std::vector<种子包> 包;
    try {
        包 = 解码种子包(请求.种子文本, 请求.中文语言节点, 请求.英文语言节点);
    } catch (const std::exception& e) {
        out.状态 = 初始化关系概念状态::语法拒绝;
        out.诊断 = e.what();
        return out;
    }
    for (const auto& p : 包) {
        const auto 读 = 关系.按种子键读取(p.定义.稳定种子键);
        if (读.状态 != 关系概念操作状态::已读取 || !读.信息) {
            out.状态 = 初始化关系概念状态::读回失败;
            out.诊断 = "缺关系概念:" + p.定义.稳定种子键;
            return out;
        }
        if (读.信息->定义.端点组 != p.定义.端点组
            || 读.信息->定义.连接 != p.定义.连接
            || 读.信息->定义.附加连接组 != p.定义.附加连接组) {
            out.状态 = 初始化关系概念状态::读回失败;
            out.诊断 = "关系概念形状漂移:" + p.定义.稳定种子键;
            return out;
        }
        for (const auto& 期望 : p.定义.表达关联组) {
            const auto it = std::find_if(
                读.信息->定义.表达关联组.begin(), 读.信息->定义.表达关联组.end(),
                [&](const 关系概念表达关联& a) {
                    return a.语言节点 == 期望.语言节点;
                });
            if (it == 读.信息->定义.表达关联组.end()
                || it->含义操作码 != 期望.含义操作码
                || it->角色槽组 != 期望.角色槽组
                || it->预置知识尾词 != 期望.预置知识尾词
                || it->陈述段组 != 期望.陈述段组) {
                out.状态 = 初始化关系概念状态::读回失败;
                out.诊断 = "表达槽合同漂移:" + p.定义.稳定种子键;
                return out;
            }
        }
        out.项组.push_back(*读.信息);
    }
    out.状态 = 初始化关系概念状态::已完成;
    return out;
}

} // namespace 海中鱼巣
