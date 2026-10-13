#include "服务要求.结构化.h"
#include "../领域/概念_存在类.h"
#include "../领域/新_语言类.h"
#include <algorithm>
#include <array>
#include <limits>
#include <map>
#include <set>
#include <utility>

namespace 海中鱼巣 {
namespace {
using 状态 = 结构化服务要求状态;
struct 材料失败 { 状态 原因; };
[[noreturn]] void 拒绝(状态 原因 = 状态::结构不一致) { throw 材料失败{原因}; }
void 必须(bool 条件, 状态 原因 = 状态::结构不一致) { if (!条件) 拒绝(原因); }
void 检查基础(基础结构操作状态 结果) {
    if (结果 == 基础结构操作状态::资源失败) 拒绝(状态::资源失败);
    必须(结果 == 基础结构操作状态::已完成);
}
enum class 字段 : std::size_t {
    类型, 场景, 来源, 目标人, 会话, 特征类型, 形式, 域, 比较, 方向, 固定语义,
    对应理解, 类型快照, 域快照, 比较快照, 来源概念, 材料型, 单位, 比较规则,
    特征快照, 域种类, 区间, 材料, 分量, 类型根, 算法, 输出域, 参照, 输入,
    结果种类, 顺序, 角色, 输入来源, 数量
};
enum class 种类 : std::size_t { 理解, 确认, 目标语义, 特征语义, 值域语义, 比较语义, 输入项, 分量, 数量 };
constexpr std::size_t 下标(字段 项) noexcept { return static_cast<std::size_t>(项); }
constexpr std::size_t 下标(种类 项) noexcept { return static_cast<std::size_t>(项); }

void 检查区间(const std::vector<特征概念I64闭区间>& 域) {
    必须(!域.empty());
    for (std::size_t i = 0; i < 域.size(); ++i) {
        必须(域[i].下界 <= 域[i].上界);
        if (i) {
            必须(域[i - 1].上界 < 域[i].下界);
            必须(域[i - 1].上界 != (std::numeric_limits<std::int64_t>::max)()
                && 域[i - 1].上界 + 1 < 域[i].下界);
        }
    }
}
std::vector<std::int64_t> 编码区间(const std::vector<特征概念I64闭区间>& 域) {
    检查区间(域);
    std::vector<std::int64_t> 结果;
    for (const auto& 项 : 域) { 结果.push_back(项.下界); 结果.push_back(项.上界); }
    return 结果;
}
std::vector<特征概念I64闭区间> 解码区间(const std::vector<std::int64_t>& 材料) {
    必须(材料.size() % 2 == 0);
    std::vector<特征概念I64闭区间> 结果;
    for (std::size_t i = 0; i < 材料.size(); i += 2) 结果.push_back({材料[i], 材料[i + 1]});
    检查区间(结果);
    return 结果;
}
void 检查形状(const 新需求目标定义& 目标) {
    必须(有效(目标.目标特征类型概念节点), 状态::目标不完整);
    if (目标.目标形式 == 新需求目标形式::值) {
        必须(!目标.目标变化方向, 状态::目标不合法);
        必须(目标.目标值域概念节点 && 有效(*目标.目标值域概念节点)
            && 目标.目标比较关系概念节点 && 有效(*目标.目标比较关系概念节点), 状态::目标不完整);
    } else if (目标.目标形式 == 新需求目标形式::变化方向) {
        必须(!目标.目标值域概念节点 && !目标.目标比较关系概念节点, 状态::目标不合法);
        必须(目标.目标变化方向.has_value(), 状态::目标不完整);
        必须(*目标.目标变化方向 == 新需求变化方向::增加
            || *目标.目标变化方向 == 新需求变化方向::减少, 状态::目标不合法);
    } else 拒绝(状态::目标不合法);
}
bool 同一理解(const 服务要求理解信息& 左, const 服务要求理解信息& 右) {
    return 左.来源人类节点 == 右.来源人类节点 && 左.公开会话标识 == 右.公开会话标识
        && 左.目标 == 右.目标 && 左.固定语义 == 右.固定语义;
}
} // namespace

struct 结构化服务要求服务::实现 {
    explicit 实现(结构化服务要求服务& 所有者) noexcept : 服务(所有者) {}
    结构化服务要求服务& 服务;
    稳定编码 人类;
    稳定编码 场景;
    std::array<稳定编码, 下标(字段::数量)> 字段锚{};
    std::array<稳定编码, 下标(种类::数量)> 类型锚{};
    bool 就绪 = false;

    void 检查人类(稳定编码 来源) const {
        必须(有效(来源), 状态::来源不合法);
        const auto 存在 = 服务.存在_.获取存在(来源);
        必须(存在 && 存在->节点 == 来源);
        // 三色迭代DFS允许共享祖先，但拒绝真正的回边，不按预算截断。
        std::map<稳定编码, unsigned> 颜色;
        struct 帧 { 稳定编码 节点; bool 退出; };
        std::vector<帧> 栈{{存在->专属存在概念节点, false}};
        bool 找到 = false;
        while (!栈.empty()) {
            const auto 当前 = 栈.back(); 栈.pop_back();
            if (当前.退出) { 颜色[当前.节点] = 2; continue; }
            const auto 色 = 颜色[当前.节点];
            必须(色 != 1);
            if (色 == 2) continue;
            const auto 概念 = 服务.存在概念_.获取存在概念(当前.节点);
            必须(概念 && 概念->节点 == 当前.节点);
            if (当前.节点 == 人类) 找到 = true;
            颜色[当前.节点] = 1;
            栈.push_back({当前.节点, true});
            for (auto i = 概念->上位概念组.rbegin(); i != 概念->上位概念组.rend(); ++i)
                栈.push_back({*i, false});
        }
        必须(找到, 状态::来源不合法);
    }
    稳定编码 类型根(稳定编码 概念) const {
        const auto 比较 = 服务.特征概念_.比较单特征概念(概念, 概念);
        if (比较.状态 == 特征概念规则状态::资源失败) 拒绝(状态::资源失败);
        必须(比较.状态 == 特征概念规则状态::已完成 && 比较.特征类型根);
        return *比较.特征类型根;
    }
    服务要求特征语义快照 捕获特征(const 特征概念信息& 信息) const {
        必须(信息.材料类型 != 特征概念材料物理类型::存在节点引用, 状态::语义快照暂不支持);
        return {信息.节点, 信息.材料类型, 信息.单位, 信息.比较规则};
    }
    void 检查域(const 服务要求值域语义快照& 信息) const {
        const auto 材料型 = 信息.特征语义.材料类型;
        必须(材料型 != 特征概念材料物理类型::存在节点引用
            && !std::holds_alternative<特征概念存在引用值域>(信息.值域), 状态::语义快照暂不支持);
        if (const auto* 区间 = std::get_if<std::vector<特征概念I64闭区间>>(&信息.值域)) {
            必须(材料型 == 特征概念材料物理类型::I64标量); 检查区间(*区间);
        } else if (const auto* 材料 = std::get_if<std::vector<稳定编码>>(&信息.值域)) {
            必须(材料型 != 特征概念材料物理类型::I64标量 && !材料->empty());
            for (std::size_t i = 0; i < 材料->size(); ++i) {
                必须(有效((*材料)[i]) && (!i || (*材料)[i - 1] < (*材料)[i]));
                必须(服务.特征值_.获取特征值((*材料)[i]).has_value());
            }
        } else if (const auto* 结构 = std::get_if<特征概念结构化I64值域>(&信息.值域)) {
            必须(材料型 == 特征概念材料物理类型::I64数组 && !结构->分量.empty());
            for (const auto& 项 : 结构->分量) { 必须(有效(项.角色)); 检查区间(项.值域); }
        } else 必须(材料型 != 特征概念材料物理类型::I64标量);
    }
    服务要求值域语义快照 捕获域(const 特征概念信息& 信息) const {
        服务要求值域语义快照 结果{捕获特征(信息), 信息.值域};
        检查域(结果); return 结果;
    }
    服务要求目标语义快照 捕获目标(const 新需求目标定义& 目标) const {
        检查形状(目标);
        const auto FT = 目标.目标特征类型概念节点;
        必须(类型根(FT) == FT, 状态::目标不合法);
        const auto 类型 = 服务.特征概念_.获取特征概念(FT);
        必须(类型 && 类型->节点 == FT);
        服务要求目标语义快照 结果{捕获特征(*类型), {}, {}};
        if (目标.目标形式 == 新需求目标形式::变化方向) return 结果;
        必须(类型根(*目标.目标值域概念节点) == FT, 状态::目标不合法);
        const auto 域 = 服务.特征概念_.获取特征概念(*目标.目标值域概念节点);
        必须(域 && 域->节点 == *目标.目标值域概念节点);
        结果.目标域 = 捕获域(*域);
        const auto 比较 = 服务.二次概念_.获取二次特征概念(*目标.目标比较关系概念节点);
        必须(比较 && 比较->节点 == *目标.目标比较关系概念节点);
        必须(比较->参照特征概念节点 == 目标.目标值域概念节点, 状态::目标不合法);
        bool 有准确值 = false;
        for (const auto& 输入 : 比较->输入定义组) {
            if (输入.来源种类 != 二次特征输入来源种类::特征准确值) continue;
            必须(输入.特征类型概念节点.has_value(), 状态::目标不合法);
            必须(类型根(*输入.特征类型概念节点) == FT, 状态::目标不合法);
            有准确值 = true;
        }
        必须(有准确值, 状态::目标不合法);
        结果.比较 = 服务要求比较语义快照{比较->节点, 比较->二次特征类型根节点,
            比较->算法, 比较->输入定义组, 比较->参照特征概念节点,
            捕获域(比较->输出特征概念), 比较->结果域种类组};
        return 结果;
    }
    bool 语义仍相同(const 服务要求理解信息& 理解) const {
        try { return 捕获目标(理解.目标) == 理解.固定语义; }
        catch (const 材料失败& 错误) {
            if (错误.原因 == 状态::目标不合法 || 错误.原因 == 状态::语义快照暂不支持) return false;
            throw;
        }
    }

    struct 编码器 {
        const 实现& owner;
        std::vector<基础组内新节点> 组;
        std::size_t 新节点(种类 类型) {
            const auto i = 组.size(); 组.emplace_back();
            写(i, 字段::类型, owner.类型锚[下标(类型)]);
            写(i, 字段::场景, owner.场景); return i;
        }
        void 写(std::size_t i, 字段 键, 基础组内字段内容 值) {
            组.at(i).字段组.push_back({owner.字段锚[下标(键)], std::move(值)});
        }
        void 数(std::size_t i, 字段 键, std::int64_t 值) { 写(i, 键, 基础值{值}); }
        void 链(std::size_t i, 字段 键, std::size_t j) { 写(i, 键, 基础组内节点引用{j}); }
        std::size_t 特征(const 服务要求特征语义快照& 值) {
            const auto i = 新节点(种类::特征语义);
            写(i, 字段::来源概念, 值.概念节点); 数(i, 字段::材料型, static_cast<std::int64_t>(值.材料类型));
            数(i, 字段::比较规则, static_cast<std::int64_t>(值.比较规则));
            if (值.单位) 写(i, 字段::单位, *值.单位); return i;
        }
        std::size_t 域(const 服务要求值域语义快照& 值) {
            const auto i = 新节点(种类::值域语义); 链(i, 字段::特征快照, 特征(值.特征语义));
            数(i, 字段::域种类, static_cast<std::int64_t>(值.值域.index() + 1));
            if (const auto* v = std::get_if<std::vector<特征概念I64闭区间>>(&值.值域)) 写(i, 字段::区间, 基础值{编码区间(*v)});
            else if (const auto* v = std::get_if<std::vector<稳定编码>>(&值.值域)) {
                for (const auto 项 : *v) 写(i, 字段::材料, 项);
            } else if (const auto* v = std::get_if<特征概念结构化I64值域>(&值.值域)) {
                std::int64_t 顺序 = 0;
                for (const auto& 项 : v->分量) {
                    const auto j = 新节点(种类::分量); 数(j, 字段::顺序, 顺序++);
                    写(j, 字段::角色, 项.角色); 写(j, 字段::区间, 基础值{编码区间(项.值域)});
                    if (项.单位) 写(j, 字段::单位, *项.单位);
                    链(i, 字段::分量, j);
                }
            } else 必须(std::holds_alternative<特征概念全材料值域>(值.值域));
            return i;
        }
        std::size_t 比较(const 服务要求比较语义快照& 值) {
            const auto i = 新节点(种类::比较语义);
            写(i, 字段::来源概念, 值.比较概念节点); 写(i, 字段::类型根, 值.类型根节点);
            数(i, 字段::算法, static_cast<std::int64_t>(值.算法)); 链(i, 字段::输出域, 域(值.输出语义));
            if (值.参照概念节点) 写(i, 字段::参照, *值.参照概念节点);
            for (const auto& 项 : 值.输入定义组) {
                const auto j = 新节点(种类::输入项); 数(j, 字段::顺序, 项.顺序);
                数(j, 字段::角色, static_cast<std::int64_t>(项.角色));
                数(j, 字段::输入来源, static_cast<std::int64_t>(项.来源种类));
                if (项.特征类型概念节点) 写(j, 字段::特征类型, *项.特征类型概念节点);
                链(i, 字段::输入, j);
            }
            std::vector<std::int64_t> 枚举;
            for (const auto 项 : 值.结果域种类组) 枚举.push_back(static_cast<std::int64_t>(项));
            写(i, 字段::结果种类, 基础值{std::move(枚举)}); return i;
        }
        std::size_t 目标(const 服务要求目标语义快照& 值) {
            const auto i = 新节点(种类::目标语义); 链(i, 字段::类型快照, 特征(值.目标类型));
            if (值.目标域) 链(i, 字段::域快照, 域(*值.目标域));
            if (值.比较) 链(i, 字段::比较快照, 比较(*值.比较)); return i;
        }
        std::size_t 理解(const 服务要求理解信息& 值) {
            const auto i = 新节点(种类::理解);
            写(i, 字段::来源, 值.来源人类节点); 写(i, 字段::目标人, 值.目标存在节点);
            写(i, 字段::会话, 基础值{值.公开会话标识}); 写(i, 字段::特征类型, 值.目标.目标特征类型概念节点);
            数(i, 字段::形式, static_cast<std::int64_t>(值.目标.目标形式));
            if (值.目标.目标值域概念节点) 写(i, 字段::域, *值.目标.目标值域概念节点);
            if (值.目标.目标比较关系概念节点) 写(i, 字段::比较, *值.目标.目标比较关系概念节点);
            if (值.目标.目标变化方向) 数(i, 字段::方向, static_cast<std::int64_t>(*值.目标.目标变化方向));
            链(i, 字段::固定语义, 目标(值.固定语义)); return i;
        }
    };

    // 所有解码只使用同一次反向查询返回的完整图，不能逐字段fresh拼装。
    struct 解码器 {
        const 实现& owner;
        std::map<稳定编码, const 基础节点结构*> 图;
        struct 游标 {
            const 实现& owner;
            const 基础节点结构& 节点;
            std::set<稳定编码> 已读;
            游标(const 实现& o, const 基础节点结构& n, 种类 类型) : owner(o), 节点(n) {
                必须(引用(字段::类型) == owner.类型锚[下标(类型)]);
                必须(引用(字段::场景) == owner.场景);
            }
            std::vector<const 基础字段内容*> 全部(字段 键) {
                必须(已读.insert(owner.字段锚[下标(键)]).second);
                std::vector<const 基础字段内容*> 值;
                for (const auto& 项 : 节点.字段组) {
                    必须(项.所属节点 == 节点.节点.编码);
                    if (项.字段节点 == owner.字段锚[下标(键)]) 值.push_back(&项.内容);
                }
                return 值;
            }
            static 稳定编码 解引用(const 基础字段内容& 值) {
                const auto* p = std::get_if<稳定编码>(&值); 必须(p && 有效(*p)); return *p;
            }
            std::vector<稳定编码> 引用组(字段 键) {
                std::vector<稳定编码> 值; std::set<稳定编码> 唯一;
                for (const auto* 项 : 全部(键)) {
                    const auto 编码 = 解引用(*项); 必须(唯一.insert(编码).second); 值.push_back(编码);
                }
                return 值;
            }
            std::optional<稳定编码> 可选引用(字段 键) {
                const auto 值 = 全部(键); 必须(值.size() <= 1);
                if (值.empty()) return {}; return 解引用(*值.front());
            }
            稳定编码 引用(字段 键) { const auto v = 可选引用(键); 必须(v.has_value()); return *v; }
            template<class T> std::optional<T> 可选原始(字段 键) {
                const auto 值 = 全部(键); 必须(值.size() <= 1); if (值.empty()) return {};
                const auto* 基础 = std::get_if<基础值>(值.front()); 必须(基础 != nullptr);
                const auto* p = std::get_if<T>(&基础->材料); 必须(p != nullptr); return *p;
            }
            template<class T> T 原始(字段 键) {
                auto v = 可选原始<T>(键); 必须(v.has_value()); return std::move(*v);
            }
            std::int64_t 数(字段 键) { return 原始<std::int64_t>(键); }
            void 完成() const {
                for (const auto& 项 : 节点.字段组) 必须(已读.contains(项.字段节点));
            }
        };
        解码器(const 实现& o, const 基础引用节点结构结果& 材料) : owner(o) {
            for (const auto& 节点 : 材料.来源节点组)
                必须(图.emplace(节点.节点.编码, &节点).second);
            // 每个owner节点必须只有一条归属字段，反向关系与完整节点一致。
            必须(材料.引用字段组.size() == 图.size());
        }
        const 基础节点结构& 节点(稳定编码 编码) const {
            const auto i = 图.find(编码); 必须(i != 图.end()); return *i->second;
        }
        种类 类型(稳定编码 编码) const {
            const auto& n = 节点(编码); std::optional<种类> 结果;
            for (const auto& f : n.字段组) if (f.字段节点 == owner.字段锚[下标(字段::类型)]) {
                必须(!结果); const auto r = 游标::解引用(f.内容);
                const auto i = std::find(owner.类型锚.begin(), owner.类型锚.end(), r);
                必须(i != owner.类型锚.end()); 结果 = static_cast<种类>(i - owner.类型锚.begin());
            }
            必须(结果.has_value()); return *结果;
        }
        服务要求特征语义快照 特征(稳定编码 编码) const {
            游标 r(owner, 节点(编码), 种类::特征语义);
            服务要求特征语义快照 值; 值.概念节点 = r.引用(字段::来源概念);
            const auto 材料型 = r.数(字段::材料型); const auto 规则 = r.数(字段::比较规则);
            必须(材料型 >= 1 && 材料型 <= 6 && 规则 >= 1 && 规则 <= 4);
            值.材料类型 = static_cast<特征概念材料物理类型>(材料型);
            值.比较规则 = static_cast<特征概念比较规则>(规则);
            值.单位 = r.可选引用(字段::单位); r.完成(); return 值;
        }
        std::pair<std::int64_t, 特征概念I64分量定义> 分量(稳定编码 编码) const {
            游标 r(owner, 节点(编码), 种类::分量);
            const auto 顺序 = r.数(字段::顺序); 必须(顺序 >= 0);
            特征概念I64分量定义 v; v.角色 = r.引用(字段::角色); v.单位 = r.可选引用(字段::单位);
            v.值域 = 解码区间(r.原始<std::vector<std::int64_t>>(字段::区间)); r.完成(); return {顺序, std::move(v)};
        }
        服务要求值域语义快照 域(稳定编码 编码) const {
            游标 r(owner, 节点(编码), 种类::值域语义);
            服务要求值域语义快照 v; v.特征语义 = 特征(r.引用(字段::特征快照));
            switch (r.数(字段::域种类)) {
            case 1: v.值域 = 解码区间(r.原始<std::vector<std::int64_t>>(字段::区间)); break;
            case 2: v.值域 = 特征概念全材料值域{}; break;
            case 3: {
                auto 项 = r.引用组(字段::材料); std::sort(项.begin(), 项.end()); v.值域 = std::move(项); break;
            }
            case 4: {
                std::map<std::int64_t, 特征概念I64分量定义> 项;
                for (const auto 引用 : r.引用组(字段::分量)) {
                    auto x = 分量(引用); 必须(项.emplace(x.first, std::move(x.second)).second);
                }
                特征概念结构化I64值域 d;
                for (auto& [顺序, 定义] : 项) {
                    必须(顺序 == static_cast<std::int64_t>(d.分量.size())); d.分量.push_back(std::move(定义));
                }
                v.值域 = std::move(d); break;
            }
            default: 拒绝();
            }
            r.完成(); owner.检查域(v); return v;
        }
        二次特征概念输入定义 输入(稳定编码 编码) const {
            游标 r(owner, 节点(编码), 种类::输入项);
            二次特征概念输入定义 v; v.顺序 = r.数(字段::顺序); 必须(v.顺序 >= 0);
            const auto 角色 = r.数(字段::角色); const auto 来源 = r.数(字段::输入来源);
            必须(角色 >= 1 && 角色 <= 4 && 来源 >= 1 && 来源 <= 7);
            v.角色 = static_cast<二次特征输入角色>(角色); v.来源种类 = static_cast<二次特征输入来源种类>(来源);
            v.特征类型概念节点 = r.可选引用(字段::特征类型);
            if (v.来源种类 == 二次特征输入来源种类::特征准确值
                || v.来源种类 == 二次特征输入来源种类::特征变化
                || v.来源种类 == 二次特征输入来源种类::状态出现时间) 必须(v.特征类型概念节点.has_value());
            else 必须(!v.特征类型概念节点);
            r.完成(); return v;
        }
        服务要求比较语义快照 比较(稳定编码 编码) const {
            游标 r(owner, 节点(编码), 种类::比较语义);
            服务要求比较语义快照 v;
            v.比较概念节点 = r.引用(字段::来源概念); v.类型根节点 = r.引用(字段::类型根);
            const auto 算法 = r.数(字段::算法); 必须(算法 >= 1 && 算法 <= 8);
            v.算法 = static_cast<二次特征算法>(算法); v.参照概念节点 = r.可选引用(字段::参照);
            v.输出语义 = 域(r.引用(字段::输出域));
            for (const auto 项 : r.引用组(字段::输入)) v.输入定义组.push_back(输入(项));
            std::sort(v.输入定义组.begin(), v.输入定义组.end(), [](const auto& a, const auto& b) { return a.顺序 < b.顺序; });
            必须(!v.输入定义组.empty());
            for (std::size_t i = 0; i < v.输入定义组.size(); ++i)
                必须(v.输入定义组[i].顺序 == static_cast<std::int64_t>(i));
            // 手工细化的合法结果子域可以尚未绑定关系枚举；空数组不等于缺字段。
            const auto 枚举 = r.原始<std::vector<std::int64_t>>(字段::结果种类);
            for (std::size_t i = 0; i < 枚举.size(); ++i) {
                必须(枚举[i] >= 1 && 枚举[i] <= 10 && (!i || 枚举[i - 1] < 枚举[i]));
                v.结果域种类组.push_back(static_cast<二次特征结果域种类>(枚举[i]));
            }
            r.完成(); return v;
        }
        服务要求目标语义快照 目标(稳定编码 编码) const {
            游标 r(owner, 节点(编码), 种类::目标语义);
            服务要求目标语义快照 v; v.目标类型 = 特征(r.引用(字段::类型快照));
            const auto d = r.可选引用(字段::域快照); const auto c = r.可选引用(字段::比较快照);
            必须(d.has_value() == c.has_value());
            if (d) { v.目标域 = 域(*d); v.比较 = 比较(*c); }
            r.完成(); return v;
        }
        服务要求理解信息 理解(稳定编码 编码) const {
            游标 r(owner, 节点(编码), 种类::理解);
            服务要求理解信息 v; v.节点 = 编码; v.语言场景节点 = owner.场景;
            v.来源人类节点 = r.引用(字段::来源); v.目标存在节点 = r.引用(字段::目标人);
            必须(v.来源人类节点 == v.目标存在节点);
            v.公开会话标识 = r.原始<std::string>(字段::会话);
            必须(!v.公开会话标识.empty() && 新_语言类::严格UTF8字节串有效(v.公开会话标识));
            v.目标.目标特征类型概念节点 = r.引用(字段::特征类型);
            v.目标.目标形式 = static_cast<新需求目标形式>(r.数(字段::形式));
            v.目标.目标值域概念节点 = r.可选引用(字段::域);
            v.目标.目标比较关系概念节点 = r.可选引用(字段::比较);
            const auto 方向 = r.可选原始<std::int64_t>(字段::方向);
            if (方向) v.目标.目标变化方向 = static_cast<新需求变化方向>(*方向);
            try { 检查形状(v.目标); } catch (const 材料失败&) { 拒绝(); }
            v.固定语义 = 目标(r.引用(字段::固定语义));
            必须(v.固定语义.目标类型.概念节点 == v.目标.目标特征类型概念节点);
            if (v.目标.目标形式 == 新需求目标形式::值) {
                必须(v.固定语义.目标域 && v.固定语义.比较);
                必须(v.固定语义.目标域->特征语义.概念节点 == *v.目标.目标值域概念节点
                    && v.固定语义.比较->比较概念节点 == *v.目标.目标比较关系概念节点
                    && v.固定语义.比较->参照概念节点 == v.目标.目标值域概念节点);
            } else 必须(!v.固定语义.目标域 && !v.固定语义.比较);
            r.完成(); return v;
        }
        服务要求确认信息 确认(稳定编码 编码) const {
            游标 r(owner, 节点(编码), 种类::确认);
            服务要求确认信息 v; v.节点 = 编码; v.理解 = 理解(r.引用(字段::对应理解));
            必须(v.理解.来源人类节点 == r.引用(字段::来源)
                && v.理解.公开会话标识 == r.原始<std::string>(字段::会话)); r.完成(); return v;
        }
    };
    struct 完整材料 {
        std::vector<服务要求理解信息> 理解组;
        std::vector<服务要求确认信息> 确认组;
    };
    完整材料 读取全部() const {
        必须(就绪, 状态::未初始化);
        const auto 快照 = 全局基础数据集.查询字段引用节点结构(场景, 字段锚[下标(字段::场景)]);
        检查基础(快照.状态); 解码器 解码(*this, 快照); 完整材料 结果;
        for (const auto& [编码, p] : 解码.图) {
            (void)p;
            switch (解码.类型(编码)) {
            case 种类::理解: {
                auto v = 解码.理解(编码);
                for (const auto& 已有 : 结果.理解组) 必须(!同一理解(v, 已有));
                结果.理解组.push_back(std::move(v)); break;
            }
            case 种类::确认: {
                auto v = 解码.确认(编码);
                for (const auto& 已有 : 结果.确认组) 必须(v.理解.节点 != 已有.理解.节点);
                结果.确认组.push_back(std::move(v)); break;
            }
            case 种类::目标语义: (void)解码.目标(编码); break;
            case 种类::特征语义: (void)解码.特征(编码); break;
            case 种类::值域语义: (void)解码.域(编码); break;
            case 种类::比较语义: (void)解码.比较(编码); break;
            case 种类::输入项: (void)解码.输入(编码); break;
            case 种类::分量: (void)解码.分量(编码); break;
            default: 拒绝();
            }
        }
        return 结果;
    }
};

结构化服务要求服务::结构化服务要求服务(新_自然语言世界树类& 语言,
    新_存在类& 存在, 新_特征值类& 特征值, 概念_存在类& 存在概念,
    概念_特征类& 特征概念, 概念_二次特征类& 二次概念) noexcept
    : 语言_(语言), 存在_(存在), 特征值_(特征值), 存在概念_(存在概念),
      特征概念_(特征概念), 二次概念_(二次概念) {}
结构化服务要求服务::~结构化服务要求服务() = default;

结构化服务要求状态 结构化服务要求服务::初始化(稳定编码 人类概念节点) noexcept {
    try {
        std::lock_guard 锁(互斥_);
        必须(有效(人类概念节点), 状态::来源不合法);
        const auto 概念 = 存在概念_.获取存在概念(人类概念节点);
        必须(概念 && 概念->节点 == 人类概念节点);
        if (!实现_) 实现_ = std::make_unique<实现>(*this);
        auto& s = *实现_;
        必须(!有效(s.人类) || s.人类 == 人类概念节点, 状态::来源不合法);
        if (s.就绪) return 状态::已复用;
        s.人类 = 人类概念节点;
        for (auto& 锚 : s.字段锚) if (!有效(锚)) {
            const auto r = 全局基础数据集.原子建立节点({}); 检查基础(r.状态);
            必须(r.节点.has_value()); 锚 = *r.节点;
        }
        for (auto& 锚 : s.类型锚) if (!有效(锚)) {
            const auto r = 全局基础数据集.原子建立节点({}); 检查基础(r.状态);
            必须(r.节点.has_value()); 锚 = *r.节点;
        }
        if (!有效(s.场景)) {
            const auto r = 语言_.建立空场景();
            if (r.状态 == 新自然语言场景操作状态::资源失败) 拒绝(状态::资源失败);
            必须(r.状态 == 新自然语言场景操作状态::已建立 && r.场景节点);
            s.场景 = *r.场景节点;
        }
        s.就绪 = true; return 状态::已建立;
    } catch (const 材料失败& e) { return e.原因; }
    catch (...) { return 状态::资源失败; }
}

服务要求理解结果 结构化服务要求服务::建立或取得理解(const 服务要求理解请求& 请求) noexcept {
    try {
        std::lock_guard 锁(互斥_);
        必须(实现_ && 实现_->就绪, 状态::未初始化); auto& s = *实现_;
        必须(!请求.公开会话标识.empty(), 状态::会话未绑定);
        必须(新_语言类::严格UTF8字节串有效(请求.公开会话标识), 状态::目标不合法);
        s.检查人类(请求.来源人类节点);
        服务要求理解信息 值{{}, s.场景, 请求.公开会话标识, 请求.来源人类节点,
            请求.来源人类节点, 请求.目标, s.捕获目标(请求.目标)};
        const auto 材料 = s.读取全部();
        for (const auto& 已有 : 材料.理解组) if (同一理解(值, 已有)) {
            必须(s.语义仍相同(已有), 状态::需重新确认); return {状态::已复用, 已有};
        }
        实现::编码器 编码{s, {}}; const auto 索引 = 编码.理解(值);
        const auto 发布 = 全局基础数据集.原子建立节点组(编码.组); 检查基础(发布.状态);
        必须(发布.节点组.size() == 编码.组.size()); 值.节点 = 发布.节点组.at(索引);
        必须(s.语义仍相同(值), 状态::需重新确认);
        return {状态::已建立, std::move(值)};
    } catch (const 材料失败& e) { return {e.原因, {}}; }
    catch (...) { return {状态::资源失败, {}}; }
}
服务要求理解结果 结构化服务要求服务::读取理解(稳定编码 理解节点) const noexcept {
    try {
        std::lock_guard 锁(互斥_); 必须(实现_ && 实现_->就绪, 状态::未初始化);
        auto 材料 = 实现_->读取全部();
        for (auto& 值 : 材料.理解组) if (值.节点 == 理解节点) return {状态::已读取, std::move(值)};
        return {状态::理解不存在, {}};
    } catch (const 材料失败& e) { return {e.原因, {}}; }
    catch (...) { return {状态::资源失败, {}}; }
}
服务要求确认结果 结构化服务要求服务::确认理解(std::string_view 会话,
    稳定编码 来源, 稳定编码 理解节点) noexcept {
    try {
        std::lock_guard 锁(互斥_); 必须(实现_ && 实现_->就绪, 状态::未初始化); auto& s = *实现_;
        必须(!会话.empty(), 状态::会话未绑定); s.检查人类(来源);
        const auto 材料 = s.读取全部();
        const auto i = std::find_if(材料.理解组.begin(), 材料.理解组.end(),
            [理解节点](const auto& v) { return v.节点 == 理解节点; });
        必须(i != 材料.理解组.end(), 状态::理解不存在);
        必须(i->来源人类节点 == 来源 && i->公开会话标识 == 会话, 状态::确认对象不匹配);
        必须(s.语义仍相同(*i), 状态::需重新确认);
        for (const auto& v : 材料.确认组) if (v.理解.节点 == 理解节点) return {状态::已复用, v};
        // 返回载荷先分配完成；原子发布后只填稳定编码，不再构造可抛材料。
        服务要求确认结果 结果{状态::已建立, 服务要求确认信息{{}, *i, false}};
        基础节点建立请求 请求;
        请求.字段组 = {
            {s.字段锚[下标(字段::类型)], s.类型锚[下标(种类::确认)]},
            {s.字段锚[下标(字段::场景)], s.场景},
            {s.字段锚[下标(字段::来源)], 来源},
            {s.字段锚[下标(字段::会话)], 基础值{std::string(会话)}},
            {s.字段锚[下标(字段::对应理解)], 理解节点}};
        const auto 发布 = 全局基础数据集.原子建立节点(请求); 检查基础(发布.状态);
        必须(发布.节点.has_value()); 结果.确认->节点 = *发布.节点; return 结果;
    } catch (const 材料失败& e) { return {e.原因, {}}; }
    catch (...) { return {状态::资源失败, {}}; }
}
服务要求确认查询结果 结构化服务要求服务::查询已确认要求(稳定编码 来源) const noexcept {
    try {
        std::lock_guard 锁(互斥_); 必须(实现_ && 实现_->就绪, 状态::未初始化); const auto& s = *实现_;
        s.检查人类(来源); auto 材料 = s.读取全部(); 服务要求确认查询结果 结果{状态::已读取, {}};
        for (auto& 项 : 材料.确认组) if (项.理解.来源人类节点 == 来源) {
            项.需重新确认 = !s.语义仍相同(项.理解); 结果.确认组.push_back(std::move(项));
        }
        return 结果;
    } catch (const 材料失败& e) { return {e.原因, {}}; }
    catch (...) { return {状态::资源失败, {}}; }
}

} // namespace 海中鱼巣
