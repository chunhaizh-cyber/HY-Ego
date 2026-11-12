#include "表达.含义网投影.h"

#include <algorithm>
#include <initializer_list>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>

namespace 海中鱼巣 {
namespace {
using 种类=新语言知识角色种类;
using 类别=新语言知识角色类别;
using 参数=新语言知识参数角色;
using 域=新语言知识作用域;

struct 投影出现 final { std::int64_t 节点=0,出现=0; };
struct 对象值 final { 类别 类=类别::存在; 语言图引用值 值; };

新语言知识角色定义 角色(std::int64_t 编号,种类 k,std::vector<类别> 类) {
    return {编号,k,新语言知识角色数量::单项,{{编号,std::move(类)}}};
}

class 投影器 final {
public:
    投影器(const 新语言含义网& 网,const 新含义网投影上下文& 上下文):网_(网),上下文_(上下文) {
        for(const auto& 项:网.内容项组)项表_.emplace(项.编号,&项);
    }
    新含义网投影结果 运行() {
        if(网_.身份||网_.所属语言场景||网_.顶层项组.size()!=1||项表_.size()!=网_.内容项组.size())return {};
        const auto& 顶=网_.顶层项组.front();
        if(顶.语言出现编号||!项表_.contains(顶.内容引用))return {};
        const auto* 根=项表_.at(顶.内容引用);
        const auto* 算子=std::get_if<新内容算子>(&根->内容);
        const auto 根态度=算子&&算子->种类==新内容算子种类::态度 ? 算子->态度种类 : std::nullopt;
        域 根域=域::普通陈述;
        switch(顶.表达用途) {
        case 新表达用途::陈述: if(根态度==新态度种类::询问||根态度==新态度种类::请求达成)return {}; break;
        case 新表达用途::疑问: if(根态度!=新态度种类::询问)return {}; 根域=域::提问范围; break;
        case 新表达用途::祈使: if(根态度!=新态度种类::请求达成)return {}; 根域=域::请求范围; break;
        // 招呼（CONV-EXPR-TEXT §3.5）：顶层内容只能是招呼单元，投影为零角色招呼节点。
        case 新表达用途::招呼: if(!std::holds_alternative<新招呼含义单元>(根->内容))return {}; break;
        default: return {};
        }
        图_.语言节点=上下文_.语言节点;
        const auto 根出现=投影项(顶.内容引用,根域);
        if(!根出现||已用_.size()!=项表_.size())return {};
        图_.定义图.根含义编号=根出现->节点;
        图_.根出现编号=根出现->出现;
        if(用到正式信息_) {
            if(!上下文_.正式指称作用域)return {};
            图_.正式指称作用域=上下文_.正式指称作用域;
        }
        return {新含义网投影状态::已投影,std::move(图_)};
    }
private:
    const 新语言含义网& 网_;
    const 新含义网投影上下文& 上下文_;
    std::map<std::int64_t,const 新含义内容项*> 项表_;
    std::set<std::int64_t> 已用_;
    语言图含义材料 图_;
    std::int64_t 下一节点_=1,下一出现_=0;
    bool 用到正式信息_=false;
    std::vector<std::int64_t> 询问栈_; // 当前所在询问算子内容项编号，用于核对疑问代词的绑定范围
    std::int64_t 当前项_=0;

    // 先序编号：父节点先于子节点取得编号与位置，保持与原手工图一致的节点、出现顺序。
    struct 占位 final { std::size_t 节点位=0,出现位=0; 投影出现 编号; };
    占位 预留() {
        占位 p{图_.定义图.节点组.size(),图_.出现组.size(),{下一节点_++,下一出现_++}};
        图_.定义图.节点组.emplace_back();图_.出现组.emplace_back();
        return p;
    }
    void 填入(const 占位& p,新语言图含义节点 节点,域 作用域,std::vector<语言图角色绑定> 绑定) {
        节点.局部编号=p.编号.节点;节点.作用域=作用域;
        图_.定义图.节点组[p.节点位]=std::move(节点);
        图_.出现组[p.出现位]={p.编号.出现,{},p.编号.节点,作用域,{},std::move(绑定),{}};
    }

    std::optional<对象值> 对象引用(const 新表达指称& v) {
        const auto* b=std::get_if<新绑定指称>(&v);
        if(!b||!有效(b->节点))return std::nullopt;
        const auto 来源=有效(b->范围概念) ? std::optional<稳定编码>{b->范围概念} : std::nullopt;
        switch(b->类别) {
        case 新指称类别::存在: return 对象值{类别::存在,{类别::存在,b->节点,来源,{}}};
        case 新指称类别::需求: case 新指称类别::任务: case 新指称类别::方法:
            if(!来源)return std::nullopt;
            用到正式信息_=true;
            return 对象值{类别::正式信息节点,{类别::正式信息节点,b->节点,来源,{}}};
        default: return std::nullopt;
        }
    }
    static std::optional<语言图引用值> 特征类型引用(const 新表达指称& v,类别 类) {
        const auto* b=std::get_if<新绑定指称>(&v);
        if(!b||b->类别!=新指称类别::特征类型||!有效(b->节点))return std::nullopt;
        return 语言图引用值{类,b->节点,有效(b->范围概念)?std::optional<稳定编码>{b->范围概念}:std::nullopt,{}};
    }
    static std::optional<语言图引用值> 概念引用(const 新表达指称& v) {
        const auto* b=std::get_if<新绑定指称>(&v);
        if(!b||b->类别!=新指称类别::概念||!有效(b->节点))return std::nullopt;
        return 语言图引用值{类别::存在概念,b->节点,{},{}};
    }

    std::optional<投影出现> 投影招呼(const 新招呼含义单元& g,std::int64_t 编号,域 作用域) {
        if(编号!=网_.顶层项组.front().内容引用||作用域!=域::普通陈述)return std::nullopt;
        新语言图含义节点 节点;
        switch(g.种类) {
        case 新招呼种类::发起: 节点.操作=新语言知识含义操作::招呼发起; break;
        case 新招呼种类::维持: 节点.操作=新语言知识含义操作::招呼维持; break;
        case 新招呼种类::结束: 节点.操作=新语言知识含义操作::招呼结束; break;
        default: throw std::logic_error("含义网招呼种类非法");
        }
        const auto p=预留();
        填入(p,std::move(节点),作用域,{});
        return p.编号;
    }

    std::optional<投影出现> 投影项(std::int64_t 编号,域 作用域) {
        if(!项表_.contains(编号)||!已用_.insert(编号).second)return std::nullopt;
        const auto& 项=*项表_.at(编号);
        if(!项.角色映射.empty())return std::nullopt;
        当前项_=编号;
        if(const auto* s=std::get_if<新静态含义单元>(&项.内容))return 投影静态(*s,作用域);
        if(const auto* a=std::get_if<新内容算子>(&项.内容))return 投影态度(*a,作用域);
        if(const auto* g=std::get_if<新招呼含义单元>(&项.内容))return 投影招呼(*g,编号,作用域);
        return std::nullopt; // 事件、篇章组合、跨场景引用：CONV-INPUT/CONV-EVENT-LOGIC 承接
    }

    std::optional<投影出现> 投影静态(const 新静态含义单元& s,域 作用域) {
        if(s.时间||s.范围)return std::nullopt;
        if(const auto* p=std::get_if<新特征值谓项>(&s.谓项))return 投影特征值(s.主体,*p,作用域);
        if(const auto* p=std::get_if<新概念归属谓项>(&s.谓项))return 投影概念归属(s.主体,*p,作用域);
        return std::nullopt; // 同一指认待描述结构扩展
    }

    std::optional<投影出现> 投影特征值(const 新表达指称& 主体,const 新特征值谓项& p,域 作用域) {
        const auto 对象=对象引用(主体);
        if(!对象)return std::nullopt;
        const bool 正式=对象->类==类别::正式信息节点;
        新语言图含义节点 节点;
        std::vector<语言图角色绑定> 绑定{{0,对象->值}};
        节点.角色组={角色(0,种类::领域对象引用,{对象->类})};
        节点.参数组={{参数::主体,0},{参数::特征类型,1}};
        const auto 类型角色=[&](类别 类)->bool {
            const auto ft=特征类型引用(p.特征类型,类);
            if(!ft)return false;
            节点.角色组.push_back(角色(1,种类::特征类型引用,{类}));绑定.push_back({1,*ft});
            return true;
        };
        if(const auto* v=std::get_if<新表达指称>(&p.值)) {
            const auto* q=std::get_if<新疑问代词实例>(v);
            // 现有描述结构只有正式字段填项问（S10）；普通存在的特征询问无模板，整体不支持。
            if(!q||!正式||!有效(q->待求类型概念)||q->类型取材步骤||询问栈_.empty()||q->询问内容!=询问栈_.back())return std::nullopt;
            if(!类型角色(类别::正式特征类型))return std::nullopt;
            节点.操作=新语言知识含义操作::特征描述;
            节点.角色组.push_back(角色(2,种类::查询变量,{类别::查询特征值}));
            节点.参数组.push_back({参数::特征值,2});
            绑定.push_back({2,语言图待求值{q->网内编号,q->待求类型概念}});
            if(正式) {
                const auto* d=std::get_if<新绑定指称>(&主体);const auto* ft=std::get_if<新绑定指称>(&p.特征类型);
                图_.被询正式内容=正式内容引用定位{d->节点,{ft->节点}};
            }
        } else if(const auto* t=std::get_if<新类型化表达值>(&p.值)) {
            // 旧图无值类型位：只接受值类型即该特征类型的整数，避免不同类型压成同一材料。
            const auto* n=std::get_if<std::int64_t>(&t->准确值);
            const auto* ft=std::get_if<新绑定指称>(&p.特征类型);
            if(!n||正式||!ft||t->类型!=ft->节点||!类型角色(类别::I64标量特征类型))return std::nullopt;
            节点.操作=新语言知识含义操作::特征描述;节点.特征值分支=新语言知识特征值分支::准确值;
            节点.角色组.push_back(角色(2,种类::准确值,{类别::十进制整数}));
            节点.参数组.push_back({参数::特征值,2});
            绑定.push_back({2,语言图整数值{std::to_string(*n),有效(t->单位)?std::optional<稳定编码>{t->单位}:std::nullopt}});
        } else if(const auto* d=std::get_if<新表达方向>(&p.值)) {
            if(正式||!类型角色(类别::I64标量特征类型))return std::nullopt;
            节点.操作=新语言知识含义操作::特征方向描述;
            switch(*d) {
            case 新表达方向::增加: 节点.方向=新语言知识特征方向::增加; break;
            case 新表达方向::减少: 节点.方向=新语言知识特征方向::减少; break;
            default: throw std::logic_error("含义网方向枚举非法");
            }
        } else if(const auto* c=std::get_if<新表达概念条件>(&p.值)) {
            if(!有效(c->概念)||正式||!类型角色(类别::正式特征类型))return std::nullopt;
            节点.操作=新语言知识含义操作::特征描述;节点.特征值分支=新语言知识特征值分支::概念条件;
            节点.角色组.push_back(角色(2,种类::概念引用,{类别::特征概念}));
            节点.参数组.push_back({参数::特征值,2});
            绑定.push_back({2,语言图引用值{类别::特征概念,c->概念,{},{}}});
        } else return std::nullopt; // 值域需读取端点，随区间成文另行接入
        const auto 位=预留();
        填入(位,std::move(节点),作用域,std::move(绑定));
        return 位.编号;
    }

    std::optional<投影出现> 投影概念归属(const 新表达指称& 主体,const 新概念归属谓项& p,域 作用域) {
        const auto 归类=概念引用(p.概念);
        if(!归类)return std::nullopt;
        新语言图含义节点 节点;节点.操作=新语言知识含义操作::概念归类;
        std::vector<语言图角色绑定> 绑定;
        // 概念作主体（S02）须由概念owner区分存在概念与特征概念，投影不读owner，暂整体不支持。
        const auto* 主体指称=std::get_if<新绑定指称>(&主体);
        if(!主体指称||主体指称->类别!=新指称类别::存在)return std::nullopt;
        const auto 对象=对象引用(主体);
        if(!对象)return std::nullopt;
        节点.角色组={角色(0,种类::领域对象引用,{类别::存在}),角色(1,种类::概念引用,{类别::存在概念})};
        绑定={{0,对象->值},{1,*归类}};
        节点.参数组={{参数::主体,0},{参数::归类概念,1}};
        const auto 位=预留();
        填入(位,std::move(节点),作用域,std::move(绑定));
        return 位.编号;
    }

    // 最小需求含义“持有者需要目标存在”（R120/R121）：投影为直接带目标存在、不带目标子含义的需求描述节点。
    // 目标存在为询问范围内的疑问代词时为待求（目标栏问题“Y需要什么？”），为已绑定存在时为陈述目标。
    std::optional<投影出现> 投影目标存在需要(const 新内容算子& a,域 作用域) {
        std::map<std::string,const 新表达指称*> 角色表;
        for(const auto& r:a.角色组)if(!角色表.emplace(r.名称,&r.指称).second)return std::nullopt;
        if(a.确认范围||a.量化方式||a.数量值域||!a.共享角色映射.empty()||角色表.size()!=2
            ||!角色表.contains("持有者")||!角色表.contains("目标存在"))return std::nullopt;
        const auto 持有者=对象引用(*角色表.at("持有者"));
        if(!持有者||持有者->类!=类别::存在)return std::nullopt;
        新语言图含义节点 节点;节点.操作=新语言知识含义操作::需求描述;
        节点.参数组={{参数::主体,0},{参数::目标存在,1}};
        std::vector<语言图角色绑定> 绑定{{0,持有者->值}};
        const auto& 目标=*角色表.at("目标存在");
        if(const auto* q=std::get_if<新疑问代词实例>(&目标)) {
            if(作用域!=域::提问范围||!有效(q->待求类型概念)||q->类型取材步骤||询问栈_.empty()||q->询问内容!=询问栈_.back())return std::nullopt;
            节点.角色组={角色(0,种类::领域对象引用,{类别::存在}),角色(1,种类::查询变量,{类别::查询存在})};
            绑定.push_back({1,语言图待求值{q->网内编号,q->待求类型概念}});
        } else {
            const auto 目标值=对象引用(目标);
            if(作用域!=域::普通陈述||!目标值||目标值->类!=类别::存在)return std::nullopt;
            节点.角色组={角色(0,种类::领域对象引用,{类别::存在}),角色(1,种类::领域对象引用,{类别::存在})};
            绑定.push_back({1,目标值->值});
        }
        const auto 位=预留();
        填入(位,std::move(节点),作用域,std::move(绑定));
        return 位.编号;
    }

    std::optional<投影出现> 投影态度(const 新内容算子& a,域 作用域) {
        if(a.种类==新内容算子种类::态度&&a.态度种类==新态度种类::需要&&a.内容引用组.empty())return 投影目标存在需要(a,作用域);
        if(a.种类!=新内容算子种类::态度||!a.态度种类||a.内容引用组.size()!=1||!a.共享角色映射.empty()
            ||a.量化方式||a.数量值域)return std::nullopt;
        std::map<std::string,const 新表达指称*> 角色表;
        for(const auto& r:a.角色组)if(!角色表.emplace(r.名称,&r.指称).second)return std::nullopt;
        const auto 只含=[&](std::initializer_list<const char*> 允许) {
            for(const auto& 项:角色表)if(std::find_if(允许.begin(),允许.end(),[&](const char* x){return 项.first==x;})==允许.end())return false;
            return true;
        };
        const auto 内容=a.内容引用组.front();
        switch(*a.态度种类) {
        case 新态度种类::询问: {
            // 旧图无询问参与者位：参与者必须恰为本次成文的说话者与受话者，否则整体不支持。
            if(!a.确认范围||角色表.size()!=2||!角色表.contains("询问者")||!角色表.contains("受询者")
                ||!上下文_.说话者||!上下文_.受话者
                ||*角色表.at("询问者")!=新表达指称{*上下文_.说话者}||*角色表.at("受询者")!=新表达指称{*上下文_.受话者})return std::nullopt;
            const auto 位=预留();
            询问栈_.push_back(当前项_);
            const auto 子=投影项(内容,域::提问范围);
            询问栈_.pop_back();
            if(!子)return std::nullopt;
            新语言图含义节点 节点;节点.操作=新语言知识含义操作::信息请求;
            节点.角色组={角色(0,种类::子含义引用,{类别::子含义})};
            节点.参数组={{参数::被询内容,0}};
            节点.确认范围=*a.确认范围==新确认范围::填项内容 ? 新语言知识确认范围::填项内容 : 新语言知识确认范围::主张成立与否;
            节点.子含义组={{0,子->节点,新语言子含义用途::被询内容,{}}};
            填入(位,std::move(节点),作用域,{{0,语言图子出现引用{子->出现}}});
            return 位.编号;
        }
        case 新态度种类::需要: {
            if(a.确认范围||角色表.size()!=1||!角色表.contains("持有者"))return std::nullopt;
            const auto 持有者=对象引用(*角色表.at("持有者"));
            if(!持有者||持有者->类!=类别::存在)return std::nullopt;
            const auto 位=预留();
            const auto 子=投影项(内容,域::需求目标范围);if(!子)return std::nullopt;
            填入(位,需求描述节点(子->节点),作用域,{{0,持有者->值},{1,语言图子出现引用{子->出现}}});
            return 位.编号;
        }
        case 新态度种类::请求达成: {
            if(a.确认范围||角色表.size()!=2||!角色表.contains("受请求者")||!角色表.contains("提出者"))return std::nullopt;
            const auto 受请求者=对象引用(*角色表.at("受请求者"));const auto 提出者=对象引用(*角色表.at("提出者"));
            if(!受请求者||!提出者||受请求者->类!=类别::存在||提出者->类!=类别::存在)return std::nullopt;
            const auto 外=预留();const auto 内=预留();
            const auto 子=投影项(内容,域::需求目标范围);if(!子)return std::nullopt;
            填入(内,需求描述节点(子->节点),域::需求目标范围,{{0,提出者->值},{1,语言图子出现引用{子->出现}}});
            新语言图含义节点 节点;节点.操作=新语言知识含义操作::行动请求;
            节点.角色组={角色(0,种类::领域对象引用,{类别::存在}),角色(1,种类::子含义引用,{类别::子含义})};
            节点.参数组={{参数::受请求者,0},{参数::目标内容,1}};
            节点.子含义组={{1,内.编号.节点,新语言子含义用途::期望目标,{}}};
            填入(外,std::move(节点),作用域,{{0,受请求者->值},{1,语言图子出现引用{内.编号.出现}}});
            return 外.编号;
        }
        default: return std::nullopt; // 期望、请求执行：随事件单元成文接入
        }
    }
    static 新语言图含义节点 需求描述节点(std::int64_t 子节点) {
        新语言图含义节点 节点;节点.操作=新语言知识含义操作::需求描述;
        节点.角色组={角色(0,种类::领域对象引用,{类别::存在}),角色(1,种类::子含义引用,{类别::子含义})};
        节点.参数组={{参数::主体,0},{参数::目标内容,1}};
        节点.子含义组={{1,子节点,新语言子含义用途::需求目标,{}}};
        return 节点;
    }
};
} // namespace

新含义网投影结果 含义网投影旧图(const 新语言含义网& 网,const 新含义网投影上下文& 上下文) {
    核验含义网结构(网);
    return 投影器(网,上下文).运行();
}
} // namespace 海中鱼巣
