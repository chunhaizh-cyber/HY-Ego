#include "语言补全.运行服务.h"
#include "接收需求认知.服务.h"
#include "解释.基础语言.h"
#include "表达.基础语言.h"
#include "正式表达需求.h"
#include <limits>
#include "词条归类.事实服务.h"
#include "新_外设世界树类.h"
#include "../领域/新_自然语言世界树类.h"
#include "../领域/新_方法类.h"
#include "../领域/新_方法参数场景.h"
#include "../领域/新_本能函数管理类.h"
#include "../领域/概念_二次特征类.h"
#include "../领域/概念_动态类.h"
#include "../领域/概念_名称关系类.h"
#include "../领域/新_词类.h"
#include "../领域/新_特征值类.h"
#include "../适配/适配器.UTC时钟.h"
#include "../核心/容错检查.h"
#include <algorithm>
#include <array>
#include <functional>
#include <map>
#include <mutex>
#include <set>
#include <stdexcept>
namespace 海中鱼巣 {
namespace {
using S=新语言补全状态;
thread_local const std::function<新本能函数本次条件结果对(const 方法参数场景&)>* 本次补全操作=nullptr;
新本能函数本次条件结果对 补全入口(const 方法参数场景& scene) noexcept {
    try {
        if(!本次补全操作)throw std::logic_error("language completion builtin requires a typed operation");
        return (*本次补全操作)(scene);
    } catch(const std::exception& e) {
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"语言补全本能",L"语言补全工程故障，保留已经发布的前缀。",e.what());
    } catch(...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"语言补全本能",L"语言补全未知工程故障。");
    }
}
constexpr std::array<std::array<std::string_view,3>,2> 登记名{{
    {"builtin.language.lemma.ask.v1","builtin.language.lemma.send.v1","builtin.language.lemma.answer.v1"},
    {"builtin.language.pronoun.ask.v1","builtin.language.pronoun.send.v1","builtin.language.pronoun.answer.v1"}}};
std::size_t 种类下标(新语言补全种类 k){return k==新语言补全种类::词条归类 ? 0 : 1;}
新状态强时间 当前时间() {
    const auto r=UTC时钟适配器{}.读取当前UTC时间证据_v1({});
    if(!r.成功())throw std::logic_error("language completion UTC unavailable");
    return {新状态时间语义::实例绝对UTC,r.证据->UTC纳秒};
}
void 并入(std::vector<基础联合节点快照>& a,const std::vector<基础联合节点快照>& b) {
    for(const auto& item : b){const auto it=std::find_if(a.begin(),a.end(),[&](const auto& old){return old.结构.节点==item.结构.节点;});
        if(it==a.end())a.push_back(item);else if(it->结构.字段组!=item.结构.字段组||it->入向字段组!=item.入向字段组
            ||it->入向外部关系组!=item.入向外部关系组||it->出向外部关系组!=item.出向外部关系组)throw std::logic_error("completion proof contains contradictory snapshots");}
}
std::string 标量片段(const std::string& text,std::uint64_t first,std::uint64_t last) {
    std::size_t b=0,begin=std::string::npos,end=std::string::npos;std::uint64_t n=0;
    while(b<text.size()) {if(n==first)begin=b;if(n==last){end=b;break;}
        const auto c=static_cast<unsigned char>(text[b]);b+=c<128 ? 1 : c<224 ? 2 : c<240 ? 3 : 4;++n;}
    if(n==last)end=b;if(begin==std::string::npos||end==std::string::npos||end<begin)return {};return text.substr(begin,end-begin);
}
std::string 去末标点(std::string text) {
    while(!text.empty()&&(text.back()==' '||text.back()=='\r'||text.back()=='\n'||text.back()=='\t'))text.pop_back();
    for(std::string_view ending : {"。","！","!","？","?","."})if(text.ends_with(ending)){text.resize(text.size()-ending.size());break;}
    return text;
}

}
struct 语言补全运行服务::实现 final {
    新_外设世界树类& 外设;新_自然语言世界树类& 世界;新_语言类& 语言;新_词类& 词;
    概念_名称关系类& 名称;概念_存在类& 存在概念;概念_特征类& 特征概念;概念_动态类& 动态概念;概念_二次特征类& 二次;
    新_存在类& 存在;新_特征值类& 值;新_特征类& 特征;新_场景类& 场景;新_需求类& 需求;新_方法类& 方法;新_动态类& 动态;接收需求认知服务& 接收;
    词条归类事实服务 归类;
    mutable std::mutex 锁;bool 关闭=false;std::optional<新字段询问登记配置> 配置;新语言指代配置 指代;
    std::array<std::array<稳定编码,3>,2> 方法组{};
    struct 发送记录 {新语言补全目标 目标;新需求字段追问来源 来源;稳定编码 消息;bool 确认=false;};
    std::vector<发送记录> 已发送;
    std::set<稳定编码> 待发送调用;
    实现(新_外设世界树类& a,新_自然语言世界树类& b,新_语言类& c,新_词类& d,概念_名称关系类& e,
        概念_存在类& f,概念_特征类& g,概念_动态类& h,概念_二次特征类& i,新_存在类& j,新_特征值类& k,
        新_特征类& l,新_场景类& m,新_需求类& n,新_方法类& o,新_动态类& p,接收需求认知服务& q)
         : 外设(a),世界(b),语言(c),词(d),名称(e),存在概念(f),特征概念(g),动态概念(h),二次(i),存在(j),值(k),特征(l),场景(m),需求(n),方法(o),动态(p),接收(q),归类(d,e,f,g,h){}
    bool 活跃() const {std::lock_guard lock(锁);return !关闭&&配置.has_value();}
    std::vector<语言图含义材料> 理解材料(const std::string& text,新语言知识含义操作 op) const {
        语言图解释请求 q;q.语言节点=配置->语言节点;q.固定原文=text;
        q.读取模式=语言解释读取模式::本次正式有限集完整读取;
        const auto parsed=解释单句含义图(q,语言,词,名称,存在概念,特征概念,存在);
        std::vector<语言图含义材料> result;
        for(const auto& g : parsed.完整候选组)if(g.定义图.节点组.size()==1&&g.定义图.节点组.front().操作==op)result.push_back(g);
        return result;
    }
    std::optional<新语言知识信息> 材料知识(新语言知识含义操作 op) const {
        const auto all=语言.枚举语言知识候选(配置->语言节点,(std::numeric_limits<std::size_t>::max)(),词,存在概念);
        if(all.状态!=新语言知识状态::已完成)return {};
        std::optional<新语言知识信息> result;
        for(const auto& item : all.信息组)if(item.定义.目标.含义图&&item.定义.目标.含义图->节点组.size()==1
            &&item.定义.目标.含义图->节点组.front().操作==op){if(result)return {};result=item;}
        return result;
    }
    std::optional<新语言补全候选> 生成材料(const 正式表达需求& expression,新语言知识含义操作 op,const std::string& X,const 新语言补全目标& goal,
        const 新需求字段追问来源& source) const {
        if(expression.目的!=新语言知识表达目的::提问||expression.语言节点!=配置->语言节点
            ||expression.说话者!=source.请求.受请求者节点||expression.受话者!=source.请求.来源人类节点
            ||expression.交互窗口!=source.窗口||expression.含义操作!=op)return {};
        const auto knowledge=材料知识(op);if(!knowledge)return {};
        语言图含义材料 g;g.语言节点=配置->语言节点;g.定义图=*knowledge->定义.目标.含义图;g.根出现编号=0;
        const auto& node=g.定义图.节点组.front();语言图含义出现 occurrence;
        occurrence.出现编号=0;occurrence.所用知识节点=knowledge->节点;occurrence.定义含义编号=node.局部编号;occurrence.有效作用域=node.作用域;
        for(const auto& role : node.角色组)occurrence.角色组.push_back({role.局部编号,语言图材料值{X}});
        g.出现组.push_back(occurrence);
        基础表达请求 q{};q.语言节点=配置->语言节点;q.目的=新语言知识表达目的::提问;q.操作=op;q.作用域=新语言知识作用域::提问范围;
        q.最大知识条目数=(std::numeric_limits<std::size_t>::max)();q.最大名称条目数=q.最大知识条目数;q.最大输出标量数=q.最大知识条目数;
        q.受询者=source.请求.来源人类节点;q.含义图=g;
        const auto generated=生成基础语言表达(q,语言,词,名称,存在概念,特征概念,存在);
        if(generated.状态!=基础表达状态::已生成)return {};
        const auto back=理解材料(generated.文本,op);
        if(back.size()!=1||back.front().出现组.size()!=1||back.front().出现组.front().角色组.size()!=1)return {};
        const auto* raw=std::get_if<语言图材料值>(&back.front().出现组.front().角色组.front().值);
        if(!raw||raw->原文!=X)return {};
        新语言补全候选 out;out.目标=goal;out.UTF8正文=generated.文本;out.原句确认=op==新语言知识含义操作::原文重述确认;out.所用知识节点组=generated.已用知识节点组;
        std::vector<稳定编码> nodes=out.所用知识节点组;nodes.push_back(knowledge->节点);
        for(const auto& item : knowledge->定义.结构.有序项组)if(const auto* w=std::get_if<新语言知识固定词项>(&item.载荷))nodes.push_back(w->词条节点);
        const auto proof=全局基础数据集.查询节点组完整结构(nodes);if(proof.状态!=基础结构操作状态::已完成)return {};
        for(const auto& n : proof.节点组){if(!n.快照)return {};out.完整保护.push_back(*n.快照);}
        return out;
    }
    bool 复核来源(const 新需求字段追问来源& source) const {
        if(!活跃())return false;
        const auto fresh=接收.读取字段追问来源(source.请求.被认知需求节点,source.请求.受请求者节点,source.自我场景,source.父需求);
        return fresh&&*fresh==source;
    }
    std::optional<稳定编码> 核五参(const 方法参数场景& scene,const 新语言补全目标& goal,const 新需求字段追问来源& source) const {
        if(!复核来源(source))return {};
        const auto& bindings=scene.全部角色绑定();if(bindings.size()!=5)return {};
        std::array<稳定编码,5> ids{};
        for(const auto& b : bindings){if(!b.有序引用||b.有序引用->序号<1||b.有序引用->序号>5)return {};
            const auto slot=b.有序引用->序号-1;if(有效(ids[slot]))return {};ids[slot]=b.有序引用->信息节点;
            if(b.有序引用->信息类型!=(slot==4 ? 新方法参数信息类型::特征 : 新方法参数信息类型::存在))return {};}
        if(ids[1]!=source.请求.受请求者节点||ids[2]!=source.请求.来源人类节点||ids[3]!=source.窗口||ids[4]!=source.输出特征)return {};
        const auto D=需求.读取需求治理信息(ids[0]);if(!D||!D->完整目标)return {};
        if(goal.种类==新语言补全种类::词条归类) {if(!goal.词条||D->完整目标->目标.种类!=新需求种类::词条归类||D->完整目标->目标.词条归类!=goal.词条)return {};}
        else if(!goal.指代||*D->完整目标!=*goal.指代)return {};
        const auto window=外设.读取对话框来源(source.窗口);
        if(!window||window->公开宿主会话!=source.公开宿主会话||window->对话框标识!=source.对话框标识
            ||window->定位.场景节点!=source.外设场景||window->定位.输出队列特征!=std::optional{source.输出特征})return {};
        // 完整目标相同并不证明子需求仍有效；复用同一公开三层读取和末核。
        if(来源保护(source,goal,ids[0]).empty())return {};
        return ids[0];
    }
    std::vector<基础联合节点快照> 来源保护(const 新需求字段追问来源& source,const 新语言补全目标& goal,稳定编码 D) const {
        const auto child=需求.读取需求治理信息(D);
        if(!child||!child->父需求节点)return {};
        const auto info=需求.读取需求治理信息(*child->父需求节点);
        const auto root=需求.读取需求治理信息(source.父需求);
        const auto valid=[&](const 新需求治理信息& demand) {
            return demand.活动状态!=新需求活动状态::失活&&demand.执行域==新需求执行域::自我执行
                &&demand.需求主体存在节点==source.请求.受请求者节点&&demand.完整目标.has_value();
        };
        if(!info||!root||!valid(*child)||!valid(*info)||!valid(*root)
            ||child->来源类型!=新需求来源类型::任务子目标||!child->来源承接节点||!有效(*child->来源承接节点)
            ||info->父需求节点!=std::optional{source.父需求}||info->来源类型!=新需求来源类型::服务对象需求信息)return {};
        const auto& parentTarget=*info->完整目标;
        if(parentTarget.目标.种类!=新需求种类::普通||parentTarget.目标存在节点!=source.请求.来源人类节点
            ||!parentTarget.目标.引用内容定位
            ||parentTarget.目标.引用内容定位->引用需求节点!=source.请求.被认知需求节点
            ||!parentTarget.目标.引用内容定位->正式内容路径.empty())return {};
        if(goal.种类==新语言补全种类::词条归类) {
            if(!goal.词条||child->完整目标->目标.种类!=新需求种类::词条归类
                ||child->完整目标->目标.词条归类!=goal.词条)return {};
        }else if(!goal.指代||*child->完整目标!=*goal.指代)return {};
        const auto sourceProof=需求.读取字段追问来源保护(source);
        const auto adopted=语言.读取采用含义(source.请求.需求来源.采用含义节点,特征);
        const auto M=外设.读取消息保护(source.请求.需求来源.键.接收消息节点);
        if(sourceProof.状态!=新需求操作状态::已找到||!adopted.信息||!M.消息)return {};
        std::vector<基础联合节点快照> out=sourceProof.完整保护;
        // 来源正常变化返回零动作，不将相互矛盾的公开快照伪装成工程错误或成功。
        const auto merge=[&](const std::vector<基础联合节点快照>& incoming) {
            for(const auto& item : incoming) {
                const auto old=std::find_if(out.begin(),out.end(),[&](const auto& row){return row.结构.节点==item.结构.节点;});
                if(old==out.end())out.push_back(item);
                else if(old->结构.字段组!=item.结构.字段组||old->入向字段组!=item.入向字段组
                    ||old->入向外部关系组!=item.入向外部关系组||old->出向外部关系组!=item.出向外部关系组)return false;
            }
            return true;
        };
        if(!merge(adopted.完整保护)||!merge(M.完整保护))return {};
        // 正式公开投影保护实际治理F及其当前值，不能只保护D外壳而漏掉活动态。
        for(const auto demandId : {D,info->节点,source.父需求}) {
            const auto formal=需求.读取正式信息参数保护(demandId,配置->需求根概念);
            if(formal.状态!=正式信息参数读取状态::已读取||!formal.信息||!merge(formal.完整保护组))return {};
        }
        std::vector<稳定编码> nodes{D,info->节点,source.父需求,source.请求.被认知需求节点,source.窗口,source.输出特征};
        if(goal.词条)nodes.push_back(goal.词条->目标词条);
        if(goal.指代) {
            const auto r=语言.读取交互指代(goal.指代->目标存在节点,指代,场景,存在,特征);
            if(!r.已读回||!merge(r.完整保护))return {};nodes.push_back(goal.指代->目标存在节点);
        }
        const auto proof=全局基础数据集.查询节点组完整结构(nodes);if(proof.状态!=基础结构操作状态::已完成)return {};
        for(const auto& n : proof.节点组){if(!n.快照||!merge({*n.快照}))return {};}
        // 捕获后重新公开读值，避免先按旧活动态判断、却保护了新失活状态。
        if(需求.读取需求治理信息(D)!=child||需求.读取需求治理信息(info->节点)!=info
            ||需求.读取需求治理信息(source.父需求)!=root||!复核来源(source))return {};
        基础节点组追加请求 check;check.预期联合节点组=out;
        const auto current=全局基础数据集.原子追加节点组(check);
        if(current.状态==基础节点组追加状态::预期不符)return {};
        if(current.状态==基础节点组追加状态::资源失败)throw std::bad_alloc{};
        if(current.状态!=基础节点组追加状态::已完成)throw std::logic_error("language demand source joint proof failed");
        return out;
    }
    新本能函数本次条件结果对 调用(const 方法参数场景& scene,新语言补全种类 kind,新语言补全动作 action,
        const std::function<新本能函数本次条件结果对(const 方法参数场景&)>& operation) {
        if(本次补全操作)throw std::logic_error("nested language completion operation");
        struct Guard {~Guard(){本次补全操作=nullptr;}} guard;本次补全操作=&operation;
        return 新_本能函数管理类::执行(登记名[种类下标(kind)][static_cast<std::size_t>(action)-1],scene);
    }
    新本能函数本次条件结果对 成功(std::optional<稳定编码> action={}) const {
        新本能函数本次条件结果对 r;r.结果.状态=新本能函数执行状态::已按合同返回;
        r.结果.动作阶段=新本能函数动作阶段::已按函数合同返回;r.结果.结果取得状态=新本能函数结果取得状态::已取得;r.结果.动作动态节点=action;return r;
    }
};
语言补全运行服务::语言补全运行服务(新_外设世界树类& a,新_自然语言世界树类& b,新_语言类& c,新_词类& d,
    概念_名称关系类& e,概念_存在类& f,概念_特征类& g,概念_动态类& h,概念_二次特征类& i,新_存在类& j,
    新_特征值类& k,新_特征类& l,新_场景类& m,新_需求类& n,新_方法类& o,新_动态类& p,接收需求认知服务& q)
     : 实现_(std::make_unique<实现>(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q)){}
语言补全运行服务::~语言补全运行服务()=default;
void 语言补全运行服务::关闭() noexcept {
    std::set<稳定编码> calls;{std::lock_guard lock(实现_->锁);实现_->关闭=true;calls.swap(实现_->待发送调用);}
    for(const auto id : calls)实现_->方法.收束直接调用身份(id);
}
bool 语言补全运行服务::初始化(const 新字段询问登记配置& cfg) {
    auto& s=*实现_;if(s.配置)return false;
    特征概念定义 def;def.值域=std::vector<特征概念I64闭区间>{{0,1}};
    const auto FT=s.特征概念.建立或取得特征概念(def,s.值);if(!有效(FT))return false;
    def.值域=std::vector<特征概念I64闭区间>{{1,1}};
    const auto domain=s.特征概念.建立或取得特征概念(def,s.值,FT);if(!有效(domain))return false;
    const auto compare=s.二次.建立或取得默认二次特征概念(二次特征算法::有向特征差值,
        {{1,二次特征输入角色::左输入,二次特征输入来源种类::特征准确值,FT},
         {2,二次特征输入角色::右输入,二次特征输入来源种类::特征准确值,FT}},domain);
    if(!compare.概念节点)return false;
    const auto C=s.二次.建立或取得参照结果域概念(*compare.概念节点,二次特征结果域种类::包含于参照值域);
    if(!C.概念节点)return false;
    s.指代={FT,domain,*C.概念节点};s.配置=cfg;
    for(std::size_t k=0;k<2;++k)for(std::size_t a=0;a<3;++a) {
        const auto registered=新_本能函数管理类::登记(登记名[k][a],&补全入口);
        if(registered!=新本能函数登记状态::已登记&&registered!=新本能函数登记状态::精确重复)return false;
        新方法知识建立请求 req;req.作用域=新方法作用域::普通世界;req.实现方式=新方法实现方式::本能;req.行为=新方法行为::改变;
        req.方法头.作用存在概念组={a==1 ? cfg.通用存在概念 : cfg.需求根概念};
        req.方法头.涉及特征类型概念组={a==1 ? cfg.输出队列类型 : FT};req.方法头.可能结果概念组=req.方法头.涉及特征类型概念组;
        for(const auto spec : std::array<新方法有序参数规格,5>{{{1,新方法参数信息类型::存在,cfg.需求根概念},{2,新方法参数信息类型::存在,cfg.自我范围概念},
            {3,新方法参数信息类型::存在,cfg.人类概念},{4,新方法参数信息类型::存在,cfg.通用存在概念},{5,新方法参数信息类型::特征,cfg.输出队列类型}}}) {
            新方法参数规格 p;p.有序规格=spec;req.参数规格组.push_back(std::move(p));}
        req.初始模拟关系组={{新方法参数关系种类::不同,2,3},{新方法参数关系种类::特征所属存在,5,4}};
        const auto method=s.方法.确保本能方法登记(std::string(登记名[k][a]),req);if(!method.方法节点)return false;s.方法组[k][a]=*method.方法节点;
    }
    return true;
}
std::optional<稳定编码> 语言补全运行服务::读取方法(新语言补全种类 kind,新语言补全动作 action) const {
    const auto& s=*实现_;if(!s.活跃())return {};const auto id=s.方法组[种类下标(kind)][static_cast<std::size_t>(action)-1];return 有效(id) ? std::optional{id} : std::nullopt;
}
新语言指代结果 语言补全运行服务::消解交互指代(const 新语言指代请求& q) {
    auto& s=*实现_;if(!s.活跃())return {};
    const auto definition=s.存在概念.获取指代概念(q.代词概念);
    if(!definition.信息||definition.信息->定义.目标范围!=指代目标范围::单个已登记强类型信息节点)return {};
    auto checked=q;
    if(q.存在目标) {
        const auto E=s.存在.获取存在(*q.存在目标);if(!E)return {};
        for(const auto& constraint : definition.信息->定义.特征约束组) {
            if(constraint.被约束角色!=指代参数角色::目标){checked.存在目标.reset();break;}
            std::optional<std::int64_t> value;
            for(const auto F : E->特征节点组) {
                const auto head=s.特征.读取特征头保护(F);if(!head.信息||head.信息->特征类型根!=constraint.特征类型根)continue;
                const auto current=s.特征.获取特征(F);if(value||!current||!current->当前值){checked.存在目标.reset();break;}
                if(const auto* n=std::get_if<std::int64_t>(&*current->当前值))value=*n;
            }
            if(!value||std::none_of(constraint.允许值域.begin(),constraint.允许值域.end(),[&](const auto& r){return *value>=r.下界&&*value<=r.上界;}))checked.存在目标.reset();
        }
    }
    return s.语言.建立或取得交互指代(checked,s.指代,s.场景,s.存在,s.特征);
}

bool 语言补全运行服务::记录采用指代(稳定编码 U,稳定编码 H,稳定编码 E) {
    auto& s=*实现_;if(!s.活跃())return false;
    const auto adopted=s.语言.读取采用含义(U,s.特征);if(!adopted.信息)return false;
    const auto& a=*adopted.信息;
    const auto source=s.外设.读取消息保护(a.来源.接收消息节点);
    if(source.状态!=新外设事实状态::已完成||!source.消息)return false;
    const auto pieces=s.语言.按已登记词条最长匹配(a.含义.语言节点,source.消息->UTF8正文,s.词);
    if(pieces.状态!=新语言查询状态::已找到)return false;
    // 固定我/你和完整单词第三人称先按正式定义形成出现。
    // 近远指短语未有完整构式解析时保留原文，不截中心词制造缺约束的实例。
    for(const auto& piece : pieces.片段组)if(piece.词条节点) {
        const auto names=s.名称.读取词条名称候选保护(*piece.词条节点);
        std::optional<指代名称关系信息> pronoun;
        if(names.状态==概念名称关系查询状态::未找到)continue;
        if(names.状态!=概念名称关系查询状态::已找到)return false;
        for(const auto& name : names.候选组)if(const auto* p=std::get_if<指代名称关系信息>(&name)) {
            if(pronoun&&pronoun->概念.节点!=p->概念.节点)return false;pronoun=*p;
        }
        if(!pronoun)continue;
        const auto rule=pronoun->概念.定义.规则;
        if(rule!=指代声明规则::作用域说话者&&rule!=指代声明规则::作用域受话者&&rule!=指代声明规则::作用域第三方)continue;
        新语言指代请求 q;q.自然语言场景=a.场景节点;q.来源消息=a.来源.接收消息节点;
        q.原文材料=a.原文材料节点;q.作用域=U;q.代词概念=pronoun->概念.节点;
        q.原文起点=piece.原文码点起点;q.原文终点=q.原文起点+piece.原文码点数量;
        if(rule==指代声明规则::作用域说话者)q.存在目标=H;
        else if(rule==指代声明规则::作用域受话者)q.存在目标=E;
        const auto r=消解交互指代(q);if(!r.已读回)return false;
    }
    return true;
}
新语言补全缺口结果 语言补全运行服务::读取缺口(const 新需求字段追问来源& source) const {
    auto& s=*实现_;新语言补全缺口结果 out;
    if(!s.复核来源(source)){out.状态=S::来源已变化;return out;}
    const auto adopted=s.语言.读取采用含义(source.请求.需求来源.采用含义节点,s.特征);
    const auto message=s.外设.读取消息保护(source.请求.需求来源.键.接收消息节点);
    if(!adopted.信息||message.状态!=新外设事实状态::已完成||!message.消息)return out;
    并入(out.完整保护,adopted.完整保护);并入(out.完整保护,message.完整保护);
    std::set<稳定编码> targets;bool rawUnknown=false;
    const auto pieces=s.语言.按已登记词条最长匹配(s.配置->语言节点,message.消息->UTF8正文,s.词);
    if(pieces.状态!=新语言查询状态::已找到)return out;
    // 语法固定项是功能材料，不能因无概念关系被送进三根概念树。
    std::set<稳定编码> grammarWords;
    const auto knowledge=s.语言.枚举语言知识候选(s.配置->语言节点,(std::numeric_limits<std::size_t>::max)(),s.词,s.存在概念);
    if(knowledge.状态!=新语言知识状态::已完成)return out;
    for(const auto& k : knowledge.信息组)for(const auto& item : k.定义.结构.有序项组)
        if(const auto* fixed=std::get_if<新语言知识固定词项>(&item.载荷))grammarWords.insert(fixed->词条节点);
    for(const auto& p : pieces.片段组) {
        if(!p.词条节点){if(!去末标点(p.原文).empty())rawUnknown=true;continue;}
        const auto names=s.名称.读取词条名称候选保护(*p.词条节点);
        if(names.状态==概念名称关系查询状态::资源失败){out.状态=S::资源失败;return out;}
        const bool isPronoun=std::any_of(names.候选组.begin(),names.候选组.end(),[](const auto& n){return std::holds_alternative<指代名称关系信息>(n);});
        if(isPronoun||grammarWords.contains(*p.词条节点))continue;
        const auto current=s.归类.读取词条归类目标({*p.词条节点});
        并入(out.完整保护,current.完整保护);
        if(current.状态==新词条归类读取状态::未满足&&targets.insert(*p.词条节点).second)
            out.目标组.push_back({新语言补全种类::词条归类,新词条归类目标{*p.词条节点},{}});
        else if(current.状态!=新词条归类读取状态::已满足&&current.状态!=新词条归类读取状态::未满足){out.状态=S::等待材料;return out;}
    }
    // 回答中的已登记新词沿同一筹办口提出同级归类目标；不创建词界，不越过所属根。
    std::vector<实现::发送记录> sentRecords;
    {std::lock_guard lock(s.锁);sentRecords=s.已发送;}
    const auto incoming=s.外设.完整读取仍保留的接收记录();
    if(incoming.状态!=新外设事实状态::已完成)return out;
    std::map<稳定编码,std::set<稳定编码>> edges;
    for(const auto& sent : sentRecords)if(sent.来源==source&&sent.目标.词条) {
        const auto outgoing=s.外设.读取消息保护(sent.消息);if(!outgoing.消息)continue;
        const auto X=s.词.获取词条(sent.目标.词条->目标词条);if(!X.信息)continue;
        for(const auto& m : incoming.消息组) {
            if(m.窗口节点!=source.窗口||m.来源.方向!=新外设消息方向::接收||m.来源.公开宿主会话!=source.公开宿主会话
                ||m.来源.对话框标识!=source.对话框标识||m.正式记录时间.纳秒<=outgoing.消息->正式记录时间.纳秒)continue;
            const auto parsed=s.理解材料(去末标点(m.UTF8正文),新语言知识含义操作::词条归类回答);if(parsed.size()!=1||parsed.front().出现组.size()!=1)continue;
            std::optional<std::string> left,right;
            for(const auto& param : parsed.front().定义图.节点组.front().参数组)
                for(const auto& b : parsed.front().出现组.front().角色组)if(param.规则角色==b.角色编号)
                    if(const auto* raw=std::get_if<语言图材料值>(&b.值)) {
                        if(param.参数==新语言知识参数角色::主体)left=raw->原文;else if(param.参数==新语言知识参数角色::目标内容)right=raw->原文;
                    }
            if(!left||!right||*left!=X.信息->规范化词面)continue;
            const auto Y=s.词.按规范化词面查询(s.配置->语言节点,严格UTF8字节精确规范化合同节点,*right);if(!Y.信息)continue;
            edges[X.信息->节点].insert(Y.信息->节点);
            const auto current=s.归类.读取词条归类目标({Y.信息->节点});
            if(current.状态!=新词条归类读取状态::未满足||Y.信息->节点==X.信息->节点)continue;
            std::set<稳定编码> visited;
            const auto cycles=[&](auto&& self,稳定编码 node)->bool {
                if(node==X.信息->节点)return true;if(!visited.insert(node).second)return false;
                for(auto next : edges[node])if(self(self,next))return true;return false;
            };
            if(cycles(cycles,Y.信息->节点))continue;
            if(targets.insert(Y.信息->节点).second) {
                const auto answerProof=s.外设.读取消息保护(m.消息节点);if(!answerProof.消息)continue;
                并入(out.完整保护,answerProof.完整保护);并入(out.完整保护,current.完整保护);
                out.目标组.push_back({新语言补全种类::词条归类,新词条归类目标{Y.信息->节点},{}});
            }
        }
    }
    for(const auto E : s.语言.查询消息指代实例(message.消息->消息节点)) {
        const auto r=s.语言.读取交互指代(E,s.指代,s.场景,s.存在,s.特征);
        if(!r.已读回)return out;并入(out.完整保护,r.完整保护);
        if(!r.已消解) {
            新需求完整目标 goal;goal.目标存在节点=E;goal.目标.目标特征类型概念节点=s.指代.状态类型;
            goal.目标.目标形式=新需求目标形式::值;goal.目标.目标值域概念节点=s.指代.已消解值域;goal.目标.目标比较关系概念节点=s.指代.比较关系;
            out.目标组.push_back({新语言补全种类::指代澄清,{},goal});
        }
    }
    out.状态=out.目标组.empty() ? (rawUnknown ? S::等待材料 : S::无需补全) : S::已完成;return out;
}
新语言补全询问结果 语言补全运行服务::询问(const 方法参数场景& scene,const 新语言补全目标& goal,const 新需求字段追问来源& source) {
    auto& s=*实现_;新语言补全询问结果 out;
    s.调用(scene,goal.种类,新语言补全动作::询问,[&](const 方法参数场景& actual){
        const auto D=s.核五参(actual,goal,source);if(!D)return 新本能函数本次条件结果对{};
        std::string text;
        if(goal.种类==新语言补全种类::词条归类) {
            if(!goal.词条)return 新本能函数本次条件结果对{};const auto word=s.词.获取词条(goal.词条->目标词条);
            if(!word.信息)return 新本能函数本次条件结果对{};text=word.信息->规范化词面;
        } else {
            if(!goal.指代)return 新本能函数本次条件结果对{};
            const auto r=s.语言.读取交互指代(goal.指代->目标存在节点,s.指代,s.场景,s.存在,s.特征);
            if(!r.已读回||!r.信息||r.已消解)return 新本能函数本次条件结果对{};
            const auto m=s.外设.读取消息保护(r.信息->来源.来源消息);if(!m.消息)return 新本能函数本次条件结果对{};
            text=标量片段(m.消息->UTF8正文,r.信息->来源.原文起点,r.信息->来源.原文终点);
        }
        const auto op=goal.种类==新语言补全种类::词条归类 ? 新语言知识含义操作::词条归类询问 : 新语言知识含义操作::指代澄清询问;
        正式表达需求 expression;expression.目的=新语言知识表达目的::提问;expression.含义操作=op;
        expression.对象种类=goal.种类==新语言补全种类::词条归类 ? 正式表达对象种类::词条归类 : 正式表达对象种类::指代澄清;
        expression.语言节点=s.配置->语言节点;expression.说话者=source.请求.受请求者节点;expression.受话者=source.请求.来源人类节点;
        expression.交互窗口=source.窗口;expression.交互作用域=source.请求.需求来源.采用含义节点;
        expression.触发需求节点=*D;expression.触发方法节点=actual.方法节点();expression.原文来源=source.请求.需求来源;
        if(goal.词条)expression.目标词条组.push_back(goal.词条->目标词条);
        if(goal.指代)expression.指代实例=goal.指代->目标存在节点;
        const auto sendMethod=s.方法组[种类下标(goal.种类)][1];
        const auto prepared=s.方法.建立参数场景(sendMethod,actual.全部角色绑定());
        if(!prepared.参数场景)return 新本能函数本次条件结果对{};
        const auto sendCall=s.方法.建立直接调用身份(sendMethod,prepared.参数场景->全部角色绑定(),source.自我场景,source.外设场景);
        // 先准备本次发送自有调用引用，再冻结完整来源读集；实际动作仍只在发送入口发生。
        out.候选=s.生成材料(expression,op,text,goal,source);
        if(out.候选){
            const auto protection=s.来源保护(source,goal,*D);
            if(protection.empty()){s.方法.收束直接调用身份(sendCall.节点);out.候选.reset();return 新本能函数本次条件结果对{};}
            并入(out.候选->完整保护,protection);out.候选->发送调用身份=sendCall.节点;
            {std::lock_guard lock(s.锁);s.待发送调用.insert(sendCall.节点);}
            out.状态=S::已完成;return s.成功();
        }
        s.方法.收束直接调用身份(sendCall.节点);return 新本能函数本次条件结果对{};
    });return out;
}
新语言补全发送结果 语言补全运行服务::发送(const 方法参数场景& scene,const 新语言补全候选& candidate,const 新需求字段追问来源& source) {
    auto& s=*实现_;新语言补全发送结果 out;
    s.调用(scene,candidate.目标.种类,新语言补全动作::发送,[&](const 方法参数场景& actual){
        if(!s.核五参(actual,candidate.目标,source)||candidate.完整保护.empty())return 新本能函数本次条件结果对{};
        const auto window=s.外设.读取对话框来源(source.窗口);if(!window)return 新本能函数本次条件结果对{};
        if(!candidate.发送调用身份)return 新本能函数本次条件结果对{};
        const auto call=s.方法.读取直接调用身份(*candidate.发送调用身份);
        if(!call||call->已收束||call->方法节点!=actual.方法节点()||call->现实场景!=source.自我场景||call->外设场景!=source.外设场景)
            return 新本能函数本次条件结果对{};
        for(const auto& b : actual.全部角色绑定())if(!b.有序引用
            ||std::find(call->参数引用组.begin(),call->参数引用组.end(),*b.有序引用)==call->参数引用组.end())return 新本能函数本次条件结果对{};
        新外设原子发送请求 request;request.定位=window->定位;request.执行场景=source.外设场景;request.执行主体=source.请求.受请求者节点;
        request.动作定义=actual.方法节点();request.动作来源=call->节点;request.UTF8正文=candidate.UTF8正文;request.完整保护组=candidate.完整保护;
        const auto sent=s.外设.原子提交发送(request,[&]{return s.复核来源(source);});
        s.方法.收束直接调用身份(call->节点);
        {std::lock_guard lock(s.锁);s.待发送调用.erase(call->节点);}
        if((sent.输出.状态!=新外设输出队列状态::已入列&&sent.输出.状态!=新外设输出队列状态::已复用)||!sent.输出.消息)return 新本能函数本次条件结果对{};
        {std::lock_guard lock(s.锁);s.已发送.push_back({candidate.目标,source,sent.输出.消息->消息节点,candidate.原句确认});}
        out.状态=S::已完成;out.入列=新字段询问入列读回{sent.输出.消息->消息节点,sent.输出.入列状态.value_or(稳定编码{}),sent.输出.入列动态.value_or(稳定编码{}),sent.输出.消息->来源.来源序号};
        return s.成功(sent.动作节点);
    });return out;
}

新语言补全回答写回结果 语言补全运行服务::回答写回(const 方法参数场景& scene,const 新语言补全目标& goal,const 新需求字段追问来源& source) {
    auto& s=*实现_;新语言补全回答写回结果 out;
    // 选择与解析发生在动作前。动作体只消费这一份已解析载荷及其保护，不重新挑回答。
    const auto D=s.核五参(scene,goal,source);if(!D)return out;
    std::optional<稳定编码> sent;bool confirmation=false;
    {std::lock_guard lock(s.锁);for(const auto& r : s.已发送)if(r.目标==goal&&r.来源==source){sent=r.消息;confirmation=r.确认;}}
    if(!sent)return out;
    const auto outgoing=s.外设.读取消息保护(*sent);if(!outgoing.消息)return out;
    const auto incoming=s.外设.完整读取仍保留的接收记录();if(incoming.状态!=新外设事实状态::已完成)return out;
    if(confirmation) {
        std::optional<新外设消息信息> accepted;bool rejected=false;
        for(const auto& m : incoming.消息组)if(m.来源.方向==新外设消息方向::接收&&m.窗口节点==source.窗口
            &&m.来源.公开宿主会话==source.公开宿主会话&&m.来源.对话框标识==source.对话框标识
            &&m.正式记录时间.纳秒>outgoing.消息->正式记录时间.纳秒) {
            const auto yes=s.理解材料(去末标点(m.UTF8正文),新语言知识含义操作::原文确认接受);
            const auto no=s.理解材料(去末标点(m.UTF8正文),新语言知识含义操作::原文确认拒绝);
            if(!no.empty())rejected=true;
            if(yes.size()==1){if(accepted)return out;accepted=m;}
        }
        if(!accepted||rejected)return out;
        const auto proof=s.外设.读取消息保护(accepted->消息节点);if(!proof.消息)return out;
        s.调用(scene,goal.种类,新语言补全动作::回答写回,[&](const 方法参数场景& actual){
            if(!s.核五参(actual,goal,source))return 新本能函数本次条件结果对{};
            const auto fresh=s.归类.读取词条归类目标(*goal.词条);
            if(fresh.状态!=新词条归类读取状态::已满足)return 新本能函数本次条件结果对{};
            out.状态=S::已完成;out.目标当前已满足=true;out.完整保护=fresh.完整保护;并入(out.完整保护,proof.完整保护);
            return s.成功();
        });return out;
    }
    struct Answer {新外设消息信息 M;std::vector<基础联合节点快照> 保护;std::optional<稳定编码> 上位,目标;};
    std::optional<Answer> answer;
    for(const auto& m : incoming.消息组) {
        if(m.来源.方向!=新外设消息方向::接收||m.窗口节点!=source.窗口||m.场景节点!=source.外设场景
            ||m.来源.公开宿主会话!=source.公开宿主会话||m.来源.对话框标识!=source.对话框标识
            ||m.正式记录时间.语义!=新状态时间语义::实例绝对UTC||m.正式记录时间.纳秒<=outgoing.消息->正式记录时间.纳秒)continue;
        Answer a;a.M=m;const auto proof=s.外设.读取消息保护(m.消息节点);if(!proof.消息)return out;a.保护=proof.完整保护;
        if(goal.种类==新语言补全种类::词条归类) {
            if(!goal.词条)return out;const auto word=s.词.获取词条(goal.词条->目标词条);if(!word.信息)return out;
            const auto parsed=s.理解材料(去末标点(m.UTF8正文),新语言知识含义操作::词条归类回答);
            if(parsed.size()!=1||parsed.front().出现组.size()!=1)continue;
            const auto& o=parsed.front().出现组.front();if(o.角色组.size()!=2)continue;
            std::optional<std::string> X,Y;
            const auto& node=parsed.front().定义图.节点组.front();
            for(const auto& param : node.参数组)for(const auto& b : o.角色组)if(param.规则角色==b.角色编号)
                if(const auto* raw=std::get_if<语言图材料值>(&b.值)) {if(param.参数==新语言知识参数角色::主体)X=raw->原文;else if(param.参数==新语言知识参数角色::目标内容)Y=raw->原文;}
            if(!X||!Y||*X!=word.信息->规范化词面||*X==*Y)continue;
            const auto parent=s.词.按规范化词面查询(s.配置->语言节点,严格UTF8字节精确规范化合同节点,*Y);
            if(!parent.信息)continue; // 未确认词界保持真实原文，不制造Y词条。
            const auto names=s.名称.读取词条名称候选保护(parent.信息->节点);并入(a.保护,names.完整保护);
            std::set<稳定编码> parents;
            for(const auto& n : names.候选组)if(const auto* c=std::get_if<概念名称关系信息>(&n))parents.insert(c->概念节点);
            if(parents.size()!=1)continue;a.上位=*parents.begin();
        } else {
            if(!goal.指代)return out;
            const auto word=s.词.按规范化词面查询(s.配置->语言节点,严格UTF8字节精确规范化合同节点,去末标点(m.UTF8正文));
            if(!word.信息)continue;
            const auto names=s.名称.读取词条名称候选保护(word.信息->节点);并入(a.保护,names.完整保护);
            const auto adopted=s.语言.读取采用含义(source.请求.需求来源.采用含义节点,s.特征);
            if(!adopted.信息||adopted.信息->来源!=source.请求.需求来源.键)return out;
            并入(a.保护,adopted.完整保护);
            std::set<稳定编码> mentioned;
            for(const auto& o : adopted.信息->含义.出现组)for(const auto& b : o.角色组)
                if(const auto* E=std::get_if<语言图引用值>(&b.值);E&&E->类别==新语言知识角色类别::存在)mentioned.insert(E->节点);
            for(const auto instance : s.语言.查询消息指代实例(source.请求.需求来源.键.接收消息节点)) {
                const auto ref=s.语言.读取交互指代(instance,s.指代,s.场景,s.存在,s.特征);
                if(ref.已消解&&ref.信息&&ref.信息->当前存在目标
                    &&ref.信息->来源.自然语言场景==adopted.信息->场景节点&&ref.信息->来源.作用域==adopted.信息->节点) {
                    mentioned.insert(*ref.信息->当前存在目标);并入(a.保护,ref.完整保护);
                }
            }
            std::set<稳定编码> selected;
            for(const auto target : mentioned) {
                const auto E=s.存在.获取存在(target);if(!E)continue;
                for(const auto& n : names.候选组)if(const auto* c=std::get_if<概念名称关系信息>(&n);c&&c->概念节点==E->专属存在概念节点)selected.insert(target);
            }
            if(selected.size()!=1)continue;a.目标=*selected.begin();
        }
        if(answer)return out;answer=std::move(a);
    }
    if(!answer)return out;
    s.调用(scene,goal.种类,新语言补全动作::回答写回,[&](const 方法参数场景& actual){
        if(!s.核五参(actual,goal,source))return 新本能函数本次条件结果对{};
        基础节点组追加请求 check;check.预期联合节点组=answer->保护;
        const auto verified=全局基础数据集.原子追加节点组(check);
        if(verified.状态==基础节点组追加状态::预期不符)return 新本能函数本次条件结果对{};
        if(verified.状态!=基础节点组追加状态::已完成)throw std::logic_error("language answer source proof failed");
        const auto call=s.方法.建立直接调用身份(actual.方法节点(),actual.全部角色绑定(),source.自我场景,source.外设场景);
        const auto action=s.动态.建立动作动态({source.请求.受请求者节点,actual.方法节点(),call.节点,source.外设场景,{},当前时间()});
        const auto demandProof=全局基础数据集.查询节点组完整结构({*D});
        if(demandProof.状态!=基础结构操作状态::已完成||demandProof.节点组.size()!=1||!demandProof.节点组.front().快照)
            throw std::logic_error("classification source demand readback unavailable");
        并入(answer->保护,{*demandProof.节点组.front().快照});
        if(!action.动态节点)throw std::logic_error("language answer action publication failed");
        s.方法.绑定直接调用动作(call.节点,*action.动态节点);
        if(!s.核五参(actual,goal,source)){s.方法.收束直接调用身份(call.节点);return 新本能函数本次条件结果对{};}
        if(goal.种类==新语言补全种类::词条归类) {
            const auto written=s.归类.写回词条归类({goal.词条->目标词条,*answer->上位,*D,answer->M.消息节点,answer->保护});
            if(written.状态==新词条归类概念写回状态::已写入且读回) {
                const auto fresh=s.归类.读取词条归类目标(*goal.词条);
                out.目标当前已满足=fresh.状态==新词条归类读取状态::已满足;out.完整保护=fresh.完整保护;
                const auto adopted=s.材料知识(新语言知识含义操作::词条归类回答);
                if(!adopted||!written.概念节点||!s.语言.记录词条归类学习(goal.词条->目标词条,*written.概念节点,answer->M.消息节点,adopted->节点))out.目标当前已满足=false;
                // 词条事实已成立不等于整句已理解。当前服务尚未取得可证明完整理解的
                // 重述图及其独立回读；明确保留后续待办，绝不以原文引述代替重组。
                out.仍待理解重述=true;
            }
        } else {
            const auto old=s.语言.读取交互指代(goal.指代->目标存在节点,s.指代,s.场景,s.存在,s.特征);
            if(old.已读回&&old.信息) {
                auto q=old.信息->来源;q.存在目标=answer->目标;
                const auto written=消解交互指代(q);out.目标当前已满足=written.已读回&&written.已消解;out.完整保护=written.完整保护;
            }
        }
        s.方法.收束直接调用身份(call.节点);
        if(out.目标当前已满足){out.状态=out.仍待理解重述 ? S::等待材料 : S::已完成;return s.成功(action.动态节点);}
        return 新本能函数本次条件结果对{};
    });return out;
}
} // namespace 海中鱼巣
