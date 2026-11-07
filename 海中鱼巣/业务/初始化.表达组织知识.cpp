#include "初始化.表达组织知识.h"
#include <sstream>
#include <stdexcept>

namespace 海中鱼巣 {
namespace {
// 首批种子语法有限：逐行解码，未知选项和重复声明拒绝，不忽略多余材料。
std::vector<新表达组织知识定义> 解码组织知识(const std::string& 文本) {
    std::istringstream 流(文本);std::string 行;std::vector<std::vector<std::string>> 行组;
    while(std::getline(流,行)) {
        if(!行.empty()&&行.back()=='\r')行.pop_back();
        if(行组.empty()&&行.starts_with("\xEF\xBB\xBF"))行.erase(0,3);
        std::istringstream 行流(行);std::vector<std::string> 词组;std::string 词;
        while(行流>>词)词组.push_back(词);
        if(!词组.empty())行组.push_back(std::move(词组));
    }
    const auto 要求=[](bool ok){if(!ok)throw std::logic_error("表达组织知识种子语法或静态签名不支持");};
    const auto 编号=[&](const std::string& t){
        要求(!t.empty()&&t.find_first_not_of("0123456789")==std::string::npos);
        std::size_t end=0;const auto n=std::stoll(t,&end);要求(end==t.size()&&n>0);return static_cast<std::int64_t>(n);
    };
    const auto 用途=[&](const std::string& t){
        if(t=="陈述")return 新表达用途::陈述;if(t=="疑问")return 新表达用途::疑问;
        要求(t=="祈使");return 新表达用途::祈使;
    };
    const auto 角色=[&](const std::string& t){
        if(t=="需求D")return 新组织绑定角色::需求D;if(t=="字段特征类型")return 新组织绑定角色::字段特征类型;
        if(t=="说话者")return 新组织绑定角色::说话者;要求(t=="受话者");return 新组织绑定角色::受话者;
    };
    std::vector<新表达组织知识定义> out;std::optional<新表达组织知识定义> 当前;
    bool 有适用=false,有缺材=false;std::set<std::string> 标题;
    std::map<std::int64_t,新组织绑定角色> 步骤;
    for(const auto& t:行组){
        if(t[0]=="组织"){
            要求(t.size()==2&&!当前&&标题.insert(t[1]).second);当前.emplace();有适用=false;有缺材=false;步骤.clear();continue;
        }
        要求(当前.has_value());auto& d=*当前;
        if(t[0]=="适用"){
            要求(t.size()==3&&!有适用&&t[2]=="需求字段");d.适用类型={用途(t[1]),0};有适用=true;
        } else if(t[0]=="条件"){
            要求(t==std::vector<std::string>{"条件","需求字段内容状态","等于","当前未知"});
            要求(d.适用条件组.empty());d.适用条件组.push_back(新组织取材算子::需求字段内容状态);
        } else if(t[0]=="取材"){
            要求(t.size()>=4);新组织取材步骤 step;step.编号=编号(t[1]);
            if(t[2]=="绑定角色"){
                要求(t.size()==4);step.算子=新组织取材算子::绑定角色;step.输出角色=角色(t[3]);
            } else {
                要求(t.size()==6&&t[2]=="读具名字段"&&t[3]=="需求字段值域概念"&&t[4]=="需求D"&&t[5]=="字段特征类型");
                step.算子=新组织取材算子::需求字段值域概念;step.输出角色=新组织绑定角色::字段值域概念;
            }
            要求(步骤.emplace(step.编号,step.输出角色).second);d.取材步骤组.push_back(step);
        } else if(t[0]=="模板"){
            要求(t.size()>=5);
            if(t[1]=="顶层"){
                要求(t.size()==5&&t[3]=="内容"&&d.含义网模板.顶层项组.empty());
                d.含义网模板.顶层项组.push_back({std::nullopt,用途(t[2]),编号(t[4])});
            } else if(t[2]=="态度"){
                要求(t.size()==9&&t[3]=="询问"&&t[6]=="确认范围=填项内容"&&t[7]=="内容");
                新内容算子 q;q.种类=新内容算子种类::态度;q.态度种类=新态度种类::询问;
                q.确认范围=新确认范围::填项内容;q.内容引用组={编号(t[8])};
                for(std::size_t i=4;i<6;++i){const auto pos=t[i].find('=');要求(pos!=std::string::npos);
                    const auto name=t[i].substr(0,pos);要求(name=="询问者"||name=="受询者");
                    q.角色组.push_back({name,新规则角色变量{角色(t[i].substr(pos+1))}});
                }
                d.含义网模板.内容项组.push_back({编号(t[1]),q,{},{},{}});
            } else {
                要求(t.size()==8&&t[2]=="静态"&&t[4]=="谓项=特征值"&&t[6]=="值=疑问代词实例");
                const auto 取材引用=[&](const std::string& text,const std::string& prefix){
                    要求(text.starts_with(prefix));const auto n=编号(text.substr(prefix.size()));要求(步骤.contains(n));return n;
                };
                新静态含义单元 unit;unit.主体=新规则角色变量{步骤.at(取材引用(t[3],"主体=取材"))};
                unit.谓项=新特征值谓项{新规则角色变量{步骤.at(取材引用(t[5],"FT=取材"))},
                    新表达指称{新疑问代词实例{0,{},0,取材引用(t[7],"类型=取材")}}};
                d.含义网模板.内容项组.push_back({编号(t[1]),unit,{},{},{}});
            }
        } else if(t[0]=="缺材"){
            要求(t==std::vector<std::string>{"缺材","必需不足"}&&!有缺材);有缺材=true;
        } else {
            要求(t==std::vector<std::string>{"结束"}&&有适用&&有缺材);
            for(auto& i:d.含义网模板.内容项组){
                auto* unit=std::get_if<新静态含义单元>(&i.内容);if(!unit)continue;
                auto& value=std::get<新表达指称>(std::get<新特征值谓项>(unit->谓项).值);
                auto& q=std::get<新疑问代词实例>(value);std::size_t count=0;
                for(const auto& parent:d.含义网模板.内容项组){const auto* op=std::get_if<新内容算子>(&parent.内容);
                    if(op&&std::find(op->内容引用组.begin(),op->内容引用组.end(),i.编号)!=op->内容引用组.end()){q.询问内容=parent.编号;++count;}}
                要求(count==1);
            }
            std::sort(d.含义网模板.内容项组.begin(),d.含义网模板.内容项组.end(),[](const auto& x,const auto& y){return x.编号<y.编号;});
            核验含义网结构(d.含义网模板,true);out.push_back(d);当前.reset();
        }
    }
    要求(!当前&&!out.empty());return out;
}
}
void 初始化表达组织知识(const std::string& 文本,新_语言类& 语言) {
    const auto 定义组=解码组织知识(文本);
    语言.初始化表达组织知识结构();
    for(const auto& d:定义组){
        const auto r=语言.建立或取得表达组织知识(d);
        const auto b=语言.读取表达组织知识(r.节点);
        if(b.节点!=r.节点||b.定义!=d)throw std::logic_error("表达组织知识初始化读回不一致");
    }
}
void 读取表达组织知识种子(const std::string& 文本,const 新_语言类& 语言) {
    for(const auto& d:解码组织知识(文本)) {
        const auto 候选=语言.查询适用表达组织知识(d.适用类型);
        const auto 数=std::count_if(候选.候选组.begin(),候选.候选组.end(),[&](const auto& i){return i.定义==d;});
        if(数!=1)throw std::logic_error("表达组织知识只读重载缺失或重复");
    }
}
} // namespace 海中鱼巣
