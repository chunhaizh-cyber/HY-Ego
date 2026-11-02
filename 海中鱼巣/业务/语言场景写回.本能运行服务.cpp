#include "语言场景写回.本能运行服务.h"
#include "新_世界树类.h"
#include "新_外设世界树类.h"
#include "../领域/新_自然语言世界树类.h"
#include "../领域/新_方法类.h"
#include "../领域/新_本能函数管理类.h"
#include "../领域/新_方法参数场景.h"
#include "../适配/适配器.UTC时钟.h"
#include "../核心/容错检查.h"
#include <algorithm>
#include <map>
#include <mutex>
#include <set>
#include <stdexcept>
namespace 海中鱼巣 {
namespace {
bool 同快照(const 基础联合节点快照& a,const 基础联合节点快照& b){return a.结构.节点==b.结构.节点&&a.结构.字段组==b.结构.字段组&&a.入向字段组==b.入向字段组&&a.入向外部关系组==b.入向外部关系组&&a.出向外部关系组==b.出向外部关系组;}
bool 并保护(std::vector<基础联合节点快照>& a,const std::vector<基础联合节点快照>& b){for(const auto& x:b){const auto i=std::find_if(a.begin(),a.end(),[&](const auto& v){return v.结构.节点.编码==x.结构.节点.编码;});if(i==a.end())a.push_back(x);else if(!同快照(*i,x))return false;}return true;}
void 应用自身追加(std::vector<基础联合节点快照>& all,const 新事实追加保护增量& d){
    const auto add=[](auto& v,const auto& x){const auto i=std::find_if(v.begin(),v.end(),[&](const auto& y){return y.编码==x.编码;});if(i==v.end())v.push_back(x);else if(*i!=x)throw std::logic_error("claim own append receipt differs");std::sort(v.begin(),v.end(),[](const auto& a,const auto& b){return a.编码<b.编码;});};
    for(auto& s:all){const auto n=s.结构.节点.编码;for(const auto& f:d.新增字段组){if(f.所属节点==n)add(s.结构.字段组,f);const auto* t=std::get_if<稳定编码>(&f.内容);if(t&&*t==n)add(s.入向字段组,f);}for(const auto& e:d.新增外部关系组){if(e.源节点==n)add(s.出向外部关系组,e);if(e.目标节点==n)add(s.入向外部关系组,e);}for(const auto& f:d.字段变化组){if(f.修改前.所属节点!=n)continue;const auto i=std::find(s.结构.字段组.begin(),s.结构.字段组.end(),f.修改前);if(i==s.结构.字段组.end())throw std::logic_error("claim own replacement receipt differs");*i=f.修改后;}}
}
新本能函数本次条件结果对 返回(新语言场景主张状态 s){新本能函数本次条件结果对 r;r.结果.语言场景写回=新语言场景主张结果{s,{}};return r;}
bool 整数规范(const std::string& s){if(s.empty())return false;const std::size_t start=s[0]=='-'?1:0;if(start==s.size()||(s[start]=='0'&&(start||s.size()!=1)))return false;return std::all_of(s.begin()+static_cast<std::ptrdiff_t>(start),s.end(),[](char c){return c>='0'&&c<='9';});}
}
std::optional<新语言场景主张内容> 投影语言场景主张(const 语言场景基础含义& m,std::vector<新语言场景主张对象> objects){
    新语言场景主张内容 out;out.对象组=std::move(objects);
    const auto conceptOf=[&](std::int64_t id)->std::optional<稳定编码>{for(const auto& x:m.解释.出现组)if(const auto* n=std::get_if<语言场景基础命名出现>(&x);n&&n->局部出现编号==id)return n->概念节点;return {};};
    if(const auto* c=std::get_if<语言场景特征概念主张>(&m.解释.主张)){out.种类=新语言场景主张种类::特征概念;out.主体或部分出现编号=c->主体出现编号;out.特征类型概念=conceptOf(c->特征类型出现编号);out.特征值概念=conceptOf(c->特征概念出现编号);if(!out.特征类型概念||!out.特征值概念)return {};}
    else if(const auto* integer=std::get_if<语言场景整数特征主张>(&m.解释.主张)){out.种类=新语言场景主张种类::整数特征;out.主体或部分出现编号=integer->主体出现编号;out.特征类型概念=conceptOf(integer->特征类型出现编号);for(const auto& x:m.解释.出现组)if(const auto* n=std::get_if<语言场景基础整数出现>(&x);n&&n->局部出现编号==integer->整数出现编号)out.规范整数=n->规范十进制;if(!out.特征类型概念||!out.规范整数)return {};}
    else {const auto& composition=std::get<语言场景组成主张>(m.解释.主张);out.种类=新语言场景主张种类::组成;out.主体或部分出现编号=composition.部分出现编号;out.整体出现编号=composition.整体出现编号;}
    // 只核对已经解释出的对象角色；不复制语言解析或用语言实例冒充真实E。
    const auto required=out.种类==新语言场景主张种类::组成 ? 2u : 1u;
    if(out.对象组.size()!=required)return {};
    std::set<std::int64_t> seen;
    for(const auto& binding:out.对象组){
        if(!seen.insert(binding.出现编号).second)return {};
        const 语言场景基础命名出现* occurrence=nullptr;
        for(const auto& entry:m.解释.出现组){
            const auto* named=std::get_if<语言场景基础命名出现>(&entry);
            if(!named||named->局部出现编号!=binding.出现编号)continue;
            if(occurrence)return {};occurrence=named;
        }
        if(!occurrence||occurrence->概念节点!=binding.来源概念)return {};
        if(out.种类==新语言场景主张种类::组成){
            const bool part=binding.出现编号==out.主体或部分出现编号&&occurrence->角色==语言场景基础角色::部分;
            const bool whole=binding.出现编号==out.整体出现编号&&occurrence->角色==语言场景基础角色::整体;
            if(!part&&!whole)return {};
        }else if(binding.出现编号!=out.主体或部分出现编号||occurrence->角色!=语言场景基础角色::主体)return {};
    }
    if(!seen.contains(out.主体或部分出现编号)||(out.种类==新语言场景主张种类::组成&&!seen.contains(out.整体出现编号)))return {};
    return out;
}
struct 语言场景写回本能运行服务::实现 final {
    新世界事实服务引用 f;新_外设世界树类& 外设;新_自然语言世界树类& 语言世界;新_方法类& 方法;
    稳定编码 通用概念,登记方法{};std::recursive_mutex 锁;bool 已关闭=false;
    实现(新_世界树类& w,新_外设世界树类& p,新_自然语言世界树类& l,新_方法类& m,稳定编码 c):f(w.获取事实服务引用()),外设(p),语言世界(l),方法(m),通用概念(c){}
};
语言场景写回本能运行服务::语言场景写回本能运行服务(新_世界树类& w,新_外设世界树类& p,新_自然语言世界树类& l,新_方法类& m,稳定编码 c):实现_(std::make_unique<实现>(w,p,l,m,c)){}
语言场景写回本能运行服务::~语言场景写回本能运行服务()=default;
void 语言场景写回本能运行服务::初始化(){std::lock_guard lock(实现_->锁);实现_->f.场景服务.初始化语言主张存储();(void)声明();}
void 语言场景写回本能运行服务::关闭() noexcept {try{std::lock_guard lock(实现_->锁);实现_->已关闭=true;}catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"关闭语言场景写回",L"运行端口关闭异常。");}}
新本能函数本次条件结果对 语言场景写回本能运行服务::声明(){
    auto& s=*实现_;std::lock_guard lock(s.锁);if(s.已关闭)throw std::logic_error("claim runtime closed");
    新方法知识建立请求 q;q.作用域=新方法作用域::普通世界;q.实现方式=新方法实现方式::本能;q.行为=新方法行为::改变;q.方法头.作用存在概念组={s.通用概念};q.方法头.可能结果概念组={s.通用概念};
    const auto m=s.方法.确保本能方法登记(std::string(新本能函数登记名_写回语言场景主张),q);if(!m.方法节点)throw std::logic_error("claim method registration failed");s.登记方法=*m.方法节点;
    新本能函数本次条件结果对 r;r.结果.状态=新本能函数执行状态::已按合同返回;return r;
}
新本能函数本次条件结果对 语言场景写回本能运行服务::调用(const 新语言场景写回调用上下文& c){
    稳定编码 method;{std::lock_guard lock(实现_->锁);if(实现_->已关闭)return 返回(新语言场景主张状态::待材料);method=实现_->登记方法;}
    const auto p=实现_->方法.建立参数场景(method,{});if(!p.参数场景)return 返回(新语言场景主张状态::前提已变化);
    语言场景写回上下文守卫 guard(*p.参数场景,c);const auto r=实现_->方法.调用本能函数(method,*p.参数场景);
    if(!r.本次条件结果对)throw std::logic_error("claim registered call returned no material");return *r.本次条件结果对;
}
新本能函数本次条件结果对 语言场景写回本能运行服务::执行(const 方法参数场景& scene,const 新语言场景写回调用上下文& c){
    auto& s=*实现_;std::lock_guard lock(s.锁);const auto& q=c.主张;
    if(s.已关闭||新_本能函数管理类::本次停止已请求()||!c.已取得唯一主张||c.候选保护.empty()
        ||scene.用途()!=新方法参数场景用途::执行||scene.方法节点()!=s.登记方法||!scene.全部角色绑定().empty()
        ||!有效(c.执行者)||!有效(q.目标场景)||!有效(q.当前场景)||!有效(q.来源语言场景)||!有效(q.说话者)||!有效(q.语言)||q.固定原文.empty())return 返回(新语言场景主张状态::待材料);
    auto protect=c.候选保护;
    const auto covered=[&](稳定编码 n){return std::any_of(protect.begin(),protect.end(),[&](const auto& x){return x.结构.节点.编码==n;});};
    if(!covered(q.来源语言场景)||!covered(q.来源.接收消息节点))return 返回(新语言场景主张状态::待材料);
    const auto message=s.外设.读取消息保护(q.来源.接收消息节点);
    if(message.状态!=新外设事实状态::已完成||!message.消息||message.消息->来源.方向!=新外设消息方向::接收||message.消息->UTF8正文!=q.固定原文)return 返回(新语言场景主张状态::待材料);
    std::uint64_t scalarCount=0;for(const char raw:q.固定原文){const auto byte=static_cast<unsigned char>(raw);if((byte&0xC0)!=0x80)++scalarCount;}
    if(q.来源.原文起点!=0||q.来源.原文终点!=scalarCount)return 返回(新语言场景主张状态::待材料);
    const auto languageScene=s.语言世界.读取接收来源场景(q.来源.接收消息节点);
    if(!languageScene.信息||languageScene.信息->场景节点!=q.来源语言场景)return 返回(新语言场景主张状态::待材料);
    if(!并保护(protect,message.完整保护)||!并保护(protect,languageScene.完整保护))return 返回(新语言场景主张状态::前提已变化);
    const auto actionScene=s.f.场景服务.获取根场景();
    if(!actionScene)return 返回(新语言场景主张状态::待材料);
    // 当前场景由真实执行者的位置约束，caller不能填写别的场景绕过绑定门。
    const bool actorIsScene=s.f.场景服务.是场景节点(c.执行者);
    const auto actualCurrent=actorIsScene ? std::optional<稳定编码>{c.执行者}
        : s.f.场景服务.查询所属场景(c.执行者);
    if(!actualCurrent||*actualCurrent!=q.当前场景)return 返回(新语言场景主张状态::待材料);
    const auto current=s.f.场景服务.读取指定场景范围保护(*actualCurrent);
    if(!actorIsScene&&std::find(current.范围.范围成员存在组.begin(),current.范围.范围成员存在组.end(),c.执行者)==current.范围.范围成员存在组.end())return 返回(新语言场景主张状态::待材料);
    if(current.范围.状态!=新场景范围成员读取状态::已完成||!s.f.场景服务.是场景节点(q.目标场景)
        ||!s.f.场景服务.存在属于场景范围(*actionScene,q.目标场景)||!s.f.场景服务.存在属于场景范围(*actionScene,c.执行者))return 返回(新语言场景主张状态::待材料);
    if(!并保护(protect,current.完整保护))return 返回(新语言场景主张状态::前提已变化);
    const auto speaker=s.f.存在服务.读取存在身份保护(q.说话者);
    if(!speaker.信息||speaker.前提已变化||!并保护(protect,speaker.预期节点组))return 返回(新语言场景主张状态::前提已变化);
    const auto actor=s.f.存在服务.读取存在身份保护(c.执行者);
    if(!actor.信息||actor.前提已变化||!并保护(protect,actor.预期节点组))return 返回(新语言场景主张状态::前提已变化);
    std::set<std::int64_t> ids;
    for(const auto& b:q.内容.对象组){
        if(!有效(b.来源概念)||!covered(b.来源概念)||!ids.insert(b.出现编号).second)return 返回(新语言场景主张状态::待材料);
        if(q.目标场景==q.当前场景&&!b.真实存在)return 返回(新语言场景主张状态::待材料);
        if(!b.真实存在)continue;
        const auto identity=s.f.存在服务.读取存在身份保护(*b.真实存在);
        if(!identity.信息||identity.前提已变化||identity.信息->专属存在概念节点!=b.来源概念)return 返回(新语言场景主张状态::待材料);
        const auto i=std::find_if(identity.预期节点组.begin(),identity.预期节点组.end(),[&](const auto& x){return x.结构.节点.编码==*b.真实存在;});if(i==identity.预期节点组.end())throw std::logic_error("claim identity proof missing");
        const auto reverse=s.f.存在概念服务.读取专属存在概念身份保护(b.来源概念,*b.真实存在,*i);
        if(reverse.状态!=专属存在概念身份读取状态::已完成)return 返回(新语言场景主张状态::待材料);
        if(q.目标场景==q.当前场景&&std::find(current.范围.范围成员存在组.begin(),current.范围.范围成员存在组.end(),*b.真实存在)==current.范围.范围成员存在组.end())return 返回(新语言场景主张状态::待材料);
        if(!并保护(protect,identity.预期节点组)||!并保护(protect,reverse.身份保护))return 返回(新语言场景主张状态::前提已变化);
    }
    const auto& t=q.内容;
    if(t.对象组.size()!=(t.种类==新语言场景主张种类::组成 ? 2u : 1u)
        ||(t.特征类型概念&&(!有效(*t.特征类型概念)||!covered(*t.特征类型概念)))
        ||(t.特征值概念&&(!有效(*t.特征值概念)||!covered(*t.特征值概念))))return 返回(新语言场景主张状态::待材料);
    if(!ids.contains(t.主体或部分出现编号)||(t.种类==新语言场景主张种类::组成
        ? !ids.contains(t.整体出现编号)||t.特征类型概念||t.特征值概念||t.规范整数
        : !t.特征类型概念||(t.种类==新语言场景主张种类::整数特征 ? !t.规范整数||!整数规范(*t.规范整数)||t.特征值概念 : t.种类!=新语言场景主张种类::特征概念||!t.特征值概念||t.规范整数)))return 返回(新语言场景主张状态::待材料);
    const auto old=s.f.场景服务.查询语言主张(q);if(old.状态!=新语言场景主张状态::待材料){auto r=返回(old.状态);r.结果.语言场景写回=old;if(old.记录){r.结果.状态=新本能函数执行状态::已按合同返回;r.结果.结果取得状态=新本能函数结果取得状态::已取得;}return r;}
    // 材料不足的分支到此全部结束。后续已开始的动作保留，工程问题不伪装成待材料。
    基础节点组追加请求 pre;pre.预期联合节点组=protect;const auto check=全局基础数据集.原子追加节点组(pre);
    if(check.状态==基础节点组追加状态::预期不符)return 返回(新语言场景主张状态::前提已变化);if(check.状态!=基础节点组追加状态::已完成)throw std::logic_error("claim precondition proof unavailable");
    const auto call=s.方法.建立直接调用身份(scene.方法节点(),{},q.当前场景,message.消息->场景节点);
    const auto callData=全局基础数据集.查询完整节点结构(call.节点);if(!callData.结构)throw std::logic_error("claim call identity unreadable");
    新事实追加保护增量 callDelta;callDelta.新增字段组=callData.结构->字段组;应用自身追加(protect,callDelta);
    const auto now=UTC时钟适配器{}.读取当前UTC时间证据_v1({});if(!now.成功())throw std::runtime_error("claim action clock unavailable");
    新事实追加保护增量 actionDelta,sceneDelta;
    const auto action=s.f.动态服务.建立动作动态({c.执行者,scene.方法节点(),call.节点,q.目标场景,{},
        {新状态时间语义::实例绝对UTC,now.证据->UTC纳秒}},actionDelta);
    if(action.状态!=新动态操作状态::已完成||!action.动态节点)throw std::logic_error("claim action publication failed");
    const auto linked=s.f.场景服务.添加动态(*actionScene,*action.动态节点,sceneDelta);
    if(linked!=新场景操作状态::已完成&&linked!=新场景操作状态::无变化)throw std::logic_error("claim action scene publication failed");
    应用自身追加(protect,actionDelta);应用自身追加(protect,sceneDelta);s.方法.绑定直接调用动作(call.节点,*action.动态节点);
    const auto record=s.f.场景服务.记录语言主张(q,*action.动态节点,protect);
    if(record.状态!=新语言场景主张状态::已记录||!record.记录)throw std::logic_error("claim changed after action started; action retained");
    s.方法.收束直接调用身份(call.节点);
    auto r=返回(record.状态);r.结果.语言场景写回=record;r.结果.动作动态节点=*action.动态节点;r.结果.动作阶段=新本能函数动作阶段::已按函数合同返回;
    r.结果.状态=新本能函数执行状态::已按合同返回;r.结果.结果取得状态=新本能函数结果取得状态::已取得;return r;
}
} // namespace 海中鱼巣
