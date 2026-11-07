#pragma once

#include <algorithm>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>
#include "../核心/基础数据集.h"

namespace 海中鱼巣 {
enum class 新表达用途 : std::uint8_t { 陈述=1, 疑问, 祈使 };
enum class 新指称类别 : std::uint8_t { 存在=1, 场景, 特征, 状态, 动态, 关系, 因果, 需求, 任务, 方法, 概念, 特征类型 };
struct 新绑定指称 final {
    新指称类别 类别=新指称类别::存在; 稳定编码 节点{},范围概念{};
    friend bool operator==(const 新绑定指称&,const 新绑定指称&)=default;
};
struct 新指代实例引用 final {
    std::optional<稳定编码> 节点; std::optional<std::int64_t> 网内编号;
    friend bool operator==(const 新指代实例引用&,const 新指代实例引用&)=default;
};
struct 新疑问代词实例 final {
    std::int64_t 网内编号=0; 稳定编码 待求类型概念{}; std::int64_t 询问内容=0;
    std::optional<std::int64_t> 类型取材步骤;
    friend bool operator==(const 新疑问代词实例&,const 新疑问代词实例&)=default;
};
struct 新名称描述指称 final {
    std::string 名称; std::optional<std::int64_t> 描述内容;
    friend bool operator==(const 新名称描述指称&,const 新名称描述指称&)=default;
};
struct 新类型化表达值 final {
    稳定编码 类型{},单位{}; std::variant<std::int64_t,std::string> 准确值;
    friend bool operator==(const 新类型化表达值&,const 新类型化表达值&)=default;
};
struct 新匿名表达对象 final {
    std::int64_t 网内编号=0; 稳定编码 范围概念{};
    friend bool operator==(const 新匿名表达对象&,const 新匿名表达对象&)=default;
};
struct 新明确未知 final { friend bool operator==(const 新明确未知&,const 新明确未知&)=default; };
enum class 新组织绑定角色 : std::uint8_t { 需求D=1, 字段特征类型, 说话者, 受话者, 字段值域概念 };
struct 新规则角色变量 final {
    新组织绑定角色 角色=新组织绑定角色::需求D;
    friend bool operator==(const 新规则角色变量&,const 新规则角色变量&)=default;
};
using 新表达指称=std::variant<新绑定指称,新指代实例引用,新疑问代词实例,新名称描述指称,
    新类型化表达值,新匿名表达对象,新明确未知,新规则角色变量>;
enum class 新表达方向 : std::uint8_t { 增加=1, 减少 };
struct 新表达值域 final {
    稳定编码 值域概念{};
    friend bool operator==(const 新表达值域&,const 新表达值域&)=default;
};
struct 新表达概念条件 final {
    稳定编码 概念{};
    friend bool operator==(const 新表达概念条件&,const 新表达概念条件&)=default;
};
using 新表达特征值=std::variant<新表达指称,新类型化表达值,新表达方向,新表达概念条件,新表达值域>;
struct 新特征值谓项 final {
    新表达指称 特征类型; 新表达特征值 值;
    friend bool operator==(const 新特征值谓项&,const 新特征值谓项&)=default;
};
struct 新概念归属谓项 final {
    新表达指称 概念;
    friend bool operator==(const 新概念归属谓项&,const 新概念归属谓项&)=default;
};
struct 新同一指认谓项 final {
    新表达指称 另一指称;
    friend bool operator==(const 新同一指认谓项&,const 新同一指认谓项&)=default;
};
struct 新静态含义单元 final {
    新表达指称 主体; std::variant<新特征值谓项,新概念归属谓项,新同一指认谓项> 谓项;
    std::optional<新表达指称> 时间,范围;
    friend bool operator==(const 新静态含义单元&,const 新静态含义单元&)=default;
};
struct 新含义具名角色 final {
    std::string 名称; 新表达指称 指称;
    friend bool operator==(const 新含义具名角色&,const 新含义具名角色&)=default;
};
enum class 新动作表达方式 : std::uint8_t { 尝试=1, 完成 };
struct 新动作发生含义 final {
    std::optional<新表达指称> 主体; 稳定编码 动作概念{};
    std::vector<新含义具名角色> 对象角色组; 新动作表达方式 方式=新动作表达方式::尝试;
    friend bool operator==(const 新动作发生含义&,const 新动作发生含义&)=default;
};
struct 新状态变化含义 final {
    新表达指称 对象; 稳定编码 特征类型{};
    std::optional<新表达特征值> 前值,后值或方向; std::optional<新表达指称> 时间;
    friend bool operator==(const 新状态变化含义&,const 新状态变化含义&)=default;
};
struct 新关联主张含义 final {
    std::int64_t 动作发生引用=0,状态变化引用=0;
    friend bool operator==(const 新关联主张含义&,const 新关联主张含义&)=default;
};
struct 新事件含义单元 final {
    std::optional<新动作发生含义> 动作发生; std::optional<新状态变化含义> 状态变化;
    std::optional<新关联主张含义> 关联主张;
    friend bool operator==(const 新事件含义单元&,const 新事件含义单元&)=default;
};
enum class 新内容算子种类 : std::uint8_t { 否定=1, 条件, 引述, 态度, 量化 };
enum class 新态度种类 : std::uint8_t { 需要=1, 期望, 请求执行, 请求达成, 询问 };
enum class 新确认范围 : std::uint8_t { 真值=1, 填项内容 };
enum class 新量化方式 : std::uint8_t { 全称=1, 存在, 任指 };
struct 新共享角色映射 final {
    std::int64_t 源角色=0,目标角色=0;
    friend bool operator==(const 新共享角色映射&,const 新共享角色映射&)=default;
};
struct 新内容算子 final {
    新内容算子种类 种类=新内容算子种类::否定;
    std::vector<新含义具名角色> 角色组; std::vector<std::int64_t> 内容引用组;
    std::vector<新共享角色映射> 共享角色映射;
    std::optional<新态度种类> 态度种类; std::optional<新确认范围> 确认范围;
    std::optional<新量化方式> 量化方式; std::optional<新表达概念条件> 数量值域;
    friend bool operator==(const 新内容算子&,const 新内容算子&)=default;
};
enum class 新篇章组合种类 : std::uint8_t { 文本顺序=1, 时间顺序, 并列, 转折, 因果, 目的, 说明 };
struct 新篇章组合 final {
    新篇章组合种类 种类=新篇章组合种类::文本顺序; std::vector<std::int64_t> 有序子项组;
    friend bool operator==(const 新篇章组合&,const 新篇章组合&)=default;
};
enum class 新跨场景引用种类 : std::uint8_t { 回应=1, 引用 };
struct 新跨场景含义引用 final {
    新跨场景引用种类 种类=新跨场景引用种类::回应; 稳定编码 目标语言场景{};
    std::variant<std::int64_t,稳定编码> 目标项或指代实例;
    friend bool operator==(const 新跨场景含义引用&,const 新跨场景含义引用&)=default;
};
struct 新含义原文范围 final {
    std::int64_t 起始=0,长度=0;
    friend bool operator==(const 新含义原文范围&,const 新含义原文范围&)=default;
};
struct 新含义内容项 final {
    std::int64_t 编号=0;
    std::variant<新静态含义单元,新事件含义单元,新内容算子,新篇章组合,新跨场景含义引用> 内容;
    std::vector<新含义原文范围> 原文范围组; std::vector<稳定编码> 所用知识组;
    std::vector<新含义具名角色> 角色映射;
    friend bool operator==(const 新含义内容项&,const 新含义内容项&)=default;
};
struct 新含义顶层 final {
    std::optional<std::int64_t> 语言出现编号; 新表达用途 表达用途=新表达用途::陈述; std::int64_t 内容引用=0;
    friend bool operator==(const 新含义顶层&,const 新含义顶层&)=default;
};
struct 新语言含义网 final {
    std::optional<稳定编码> 身份,所属语言场景;
    std::vector<新含义顶层> 顶层项组; std::vector<新含义内容项> 内容项组;
    friend bool operator==(const 新语言含义网&,const 新语言含义网&)=default;
};

// 数据结构自身的不变式，header-only，供模板预检与具体实例共用；不读写owner。
inline void 核验含义网结构(const 新语言含义网& 网,bool 模板=false) {
    const auto 要求=[](bool 条件){if(!条件)throw std::logic_error("含义网结构不合法");};
    要求(网.顶层项组.size()==1);
    要求(网.顶层项组.front().表达用途>=新表达用途::陈述&&网.顶层项组.front().表达用途<=新表达用途::祈使);
    if(网.身份)要求(有效(*网.身份));if(网.所属语言场景)要求(有效(*网.所属语言场景));
    std::map<std::int64_t,const 新含义内容项*> 项;
    for(const auto& i:网.内容项组)要求(i.编号>=0&&项.emplace(i.编号,&i).second);
    要求(项.contains(网.顶层项组.front().内容引用));
    std::map<std::int64_t,int> 路径;
    std::vector<std::int64_t> 查询作用域;
    std::function<void(std::int64_t,bool)> 检查;
    std::function<void(const 新表达指称&,bool)> 指称;
    指称=[&](const 新表达指称& v,bool 询问){std::visit([&](const auto& x){
        using T=std::decay_t<decltype(x)>;
        if constexpr(std::is_same_v<T,新绑定指称>)要求(有效(x.节点)&&x.类别>=新指称类别::存在&&x.类别<=新指称类别::特征类型);
        else if constexpr(std::is_same_v<T,新指代实例引用>){
            要求(x.节点.has_value()!=x.网内编号.has_value());
            if(x.节点)要求(有效(*x.节点));else 要求(*x.网内编号>=0);
        }
        else if constexpr(std::is_same_v<T,新疑问代词实例>){
            要求(询问 && x.网内编号>=0 && std::find(查询作用域.begin(),查询作用域.end(),x.询问内容)!=查询作用域.end());
            if(模板)要求(x.类型取材步骤.has_value());
            else 要求(有效(x.待求类型概念) && !x.类型取材步骤);
        }
        else if constexpr(std::is_same_v<T,新规则角色变量>)要求(模板);
        else if constexpr(std::is_same_v<T,新名称描述指称>){要求(!x.名称.empty()||x.描述内容);if(x.描述内容)检查(*x.描述内容,询问);}
        else if constexpr(std::is_same_v<T,新匿名表达对象>)要求(x.网内编号>=0&&有效(x.范围概念));
        else if constexpr(std::is_same_v<T,新类型化表达值>)要求(有效(x.类型));
    },v);};
    const auto 值=[&](const 新表达特征值& v,bool q){std::visit([&](const auto& x){
        using T=std::decay_t<decltype(x)>;
        if constexpr(std::is_same_v<T,新表达指称>)指称(x,q);
        else if constexpr(std::is_same_v<T,新类型化表达值>)要求(有效(x.类型));
        else if constexpr(std::is_same_v<T,新表达概念条件>)要求(有效(x.概念));
        else if constexpr(std::is_same_v<T,新表达值域>)要求(有效(x.值域概念));
    },v);};
    检查=[&](std::int64_t id,bool 询问){
        要求(项.contains(id)&&路径[id]!=1);路径[id]=1;
        const auto& i=*项.at(id);
        for(const auto& r:i.角色映射)指称(r.指称,询问);
        std::visit([&](const auto& x){using T=std::decay_t<decltype(x)>;
            if constexpr(std::is_same_v<T,新静态含义单元>){
                指称(x.主体,询问);if(x.时间)指称(*x.时间,询问);if(x.范围)指称(*x.范围,询问);
                std::visit([&](const auto& p){using P=std::decay_t<decltype(p)>;
                    if constexpr(std::is_same_v<P,新特征值谓项>){指称(p.特征类型,询问);值(p.值,询问);}
                    else if constexpr(std::is_same_v<P,新概念归属谓项>)指称(p.概念,询问);
                    else 指称(p.另一指称,询问);
                },x.谓项);
            } else if constexpr(std::is_same_v<T,新内容算子>){
                要求(x.种类>=新内容算子种类::否定&&x.种类<=新内容算子种类::量化);
                const bool q=x.种类==新内容算子种类::态度&&x.态度种类==新态度种类::询问;
                std::set<std::string> 角色;
                for(const auto& r:x.角色组){要求(角色.insert(r.名称).second);指称(r.指称,询问||q);}
                const auto 有角色=[&](const char* n){要求(角色.contains(n));};
                if(x.种类==新内容算子种类::条件)要求(x.内容引用组.size()==2);
                else 要求(x.内容引用组.size()==1);
                if(x.种类==新内容算子种类::引述)有角色("原说话者");
                if(x.种类==新内容算子种类::态度){
                    要求(x.态度种类.has_value()&&*x.态度种类>=新态度种类::需要&&*x.态度种类<=新态度种类::询问);
                    if(q){有角色("询问者");有角色("受询者");要求(x.确认范围==新确认范围::真值||x.确认范围==新确认范围::填项内容);}
                    else if(x.态度种类==新态度种类::需要||x.态度种类==新态度种类::期望)有角色("持有者");
                    else {有角色("提出者");有角色("受请求者");}
                }
                if(x.种类==新内容算子种类::量化){要求(x.量化方式.has_value());有角色("变量");有角色("限制域");有角色("绑定范围");}
                if(q)查询作用域.push_back(id);
                for(auto c:x.内容引用组)检查(c,询问||q);
                if(q)查询作用域.pop_back();
            } else if constexpr(std::is_same_v<T,新篇章组合>){
                要求(x.种类>=新篇章组合种类::文本顺序&&x.种类<=新篇章组合种类::说明&&x.有序子项组.size()>=2);
                if(x.种类>=新篇章组合种类::转折)要求(x.有序子项组.size()==2);
                for(auto c:x.有序子项组)检查(c,询问);
            } else if constexpr(std::is_same_v<T,新事件含义单元>){
                要求(x.动作发生||x.状态变化||x.关联主张);
                if(x.动作发生){要求(有效(x.动作发生->动作概念));if(x.动作发生->主体)指称(*x.动作发生->主体,询问);for(const auto& r:x.动作发生->对象角色组)指称(r.指称,询问);}
                if(x.状态变化){const auto& s=*x.状态变化;指称(s.对象,询问);要求(有效(s.特征类型));if(s.前值)值(*s.前值,询问);if(s.后值或方向)值(*s.后值或方向,询问);if(s.时间)指称(*s.时间,询问);}
                if(x.关联主张){检查(x.关联主张->动作发生引用,询问);检查(x.关联主张->状态变化引用,询问);}
            } else {
                要求(有效(x.目标语言场景)&&(x.种类==新跨场景引用种类::回应||x.种类==新跨场景引用种类::引用));
                std::visit([&](const auto& target){using V=std::decay_t<decltype(target)>;
                    if constexpr(std::is_same_v<V,稳定编码>)要求(有效(target));else 要求(target>=0);
                },x.目标项或指代实例);
            }
        },i.内容);
        路径[id]=2;
    };
    检查(网.顶层项组.front().内容引用,false);
    for(const auto& [id,i]:项){(void)i;要求(路径[id]==2);}
}

// 首批三个静态签名的取材算子；知识保存签名，不保存代码或字段反射路径。
enum class 新组织取材算子 : std::uint8_t { 绑定角色=1, 需求字段内容状态, 需求字段值域概念 };
struct 新组织取材步骤 final {
    std::int64_t 编号=0; 新组织取材算子 算子=新组织取材算子::绑定角色;
    新组织绑定角色 输出角色=新组织绑定角色::需求D;
    friend bool operator==(const 新组织取材步骤&,const 新组织取材步骤&)=default;
};
struct 新表达需求类型 final {
    新表达用途 用途=新表达用途::疑问;
    // 表达对象的variant索引；当前唯一合法对象是需求字段。
    std::int64_t 对象种类=0;
    friend bool operator==(const 新表达需求类型&,const 新表达需求类型&)=default;
};
enum class 新组织缺材处理 : std::uint8_t { 必需不足=1, 可选略去, 明确表达未知 };
struct 新表达组织知识定义 final {
    新表达需求类型 适用类型;
    std::vector<新组织取材算子> 适用条件组;
    std::vector<新组织取材步骤> 取材步骤组;
    新语言含义网 含义网模板;
    新组织缺材处理 缺材处理=新组织缺材处理::必需不足;
    friend bool operator==(const 新表达组织知识定义&,const 新表达组织知识定义&)=default;
};
struct 新表达组织知识信息 final {
    稳定编码 节点{}; 新表达组织知识定义 定义;
    std::vector<基础联合节点快照> 完整保护;
};
struct 新表达组织知识候选 final {
    std::vector<新表达组织知识信息> 候选组;
    std::vector<基础联合节点快照> 完整保护;
};
struct 新出站具体表达信息 final {
    稳定编码 节点{},场景{},所用组织知识{},来源{}; 新语言含义网 含义网; std::string 候选话语原文;
};
} // namespace 海中鱼巣
