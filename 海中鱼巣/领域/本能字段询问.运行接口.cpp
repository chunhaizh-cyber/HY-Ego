#include "本能字段询问.运行接口.h"
#include "新_方法参数场景.h"
#include <algorithm>
#include <stdexcept>
#include <utility>

namespace 海中鱼巣 {
thread_local int 字段询问材料诊断行=0;
namespace {
void 要求(bool ok,const char* text) { if (!ok) throw std::logic_error(text); }
bool 正式读取成功(const 正式信息参数读取结果& r) {
    if (r.状态==正式信息参数读取状态::前提已变化 || r.状态==正式信息参数读取状态::信息不存在) return false;
    要求(r.状态==正式信息参数读取状态::已读取 && r.信息 && !r.完整保护组.empty(),"formal field question reader unavailable");return true;
}
}
新字段询问问题建立读回 新本能字段询问运行接口::建立出站问题(
    const 方法参数场景&,const 新字段询问材料&,稳定编码) {
    throw std::logic_error("field question runtime does not establish outbound questions");
}
新字段询问材料读回 读取通用询问材料(const 方法参数场景& scene,
    const 新字段询问登记配置& c,const 正式信息参数读取接口& reader,const 新字段询问外设窗口对应& 对应外设窗口,
    const std::function<std::optional<稳定编码>(稳定编码)>& 读取当前交互者) {
    using R=新字段询问材料状态;
    字段询问材料诊断行=0;
    // 结构类“定位未支持”（5380 §3.6）：真实执行中参数形状、Q引用D与单个受支持字段由补问叶子召回和参数1正式指定保证，
    // 运行依赖由装配保证，不满足即工程错误；模拟学习按采样实参运行、无外设窗口对应，没有这一保证，仍按定位未支持返回（不进入动作）。
    // ASK-READ-SCENE v0.4 §3.1 起询问本能在模拟用途直接按仅条件样例返回，不再调用本读取；模拟分支仅为其它模拟运行接口保留。
    要求(有效(c.需求目标存在类型)&&有效(c.需求目标特征类型),"field question subject mapping slots missing");
    const auto 未支持=[&](const char* text)->新字段询问材料读回 {
        要求(scene.用途()==新方法参数场景用途::模拟,text);return {R::定位未支持,{}};
    };
    // CONV-INSTINCT §6.6：4参数——1补问需求Q、2自我、3窗口副本、4外设场景标识特征/3。
    if (scene.全部角色绑定().size()!=4) return 未支持("field question formal parameter count mismatch");
    for (std::uint32_t i=1;i<=4;++i) {
        const auto p=scene.查询参数(i);
        if (!p || p->信息类型!=(i==4 ? 新方法参数信息类型::特征 : 新方法参数信息类型::存在)) return 未支持("field question formal parameter kind mismatch");
    }
    // 参数3为现实交互场景中的窗口副本，参数4为其外设场景标识特征（值为外设场景稳定编码）。
    const auto ident=scene.读取存在参数特征(3,c.外设场景标识类型);
    if (ident.状态==新方法参数特征读取状态::定义前提已变化 || ident.状态==新方法参数特征读取状态::特征未取得
        || ident.状态==新方法参数特征读取状态::当前值未取得)
        { 字段询问材料诊断行=__LINE__; return {R::前提已变化,{}}; }
    if (ident.状态==新方法参数特征读取状态::特征匹配不唯一) return 未支持("window copy peripheral scene identifier not unique");
    要求(ident.状态==新方法参数特征读取状态::已读取 && ident.特征.has_value(),"declared peripheral scene identifier unreadable");
    if (ident.特征->节点!=scene.查询参数(4)->信息节点) return 未支持("parameter 4 is not the identifier feature of parameter 3");
    const auto* 标识值=ident.特征->当前值 ? std::get_if<std::int64_t>(&*ident.特征->当前值) : nullptr;
    if (!标识值 || *标识值<=0) { 字段询问材料诊断行=__LINE__; return {R::前提已变化,{}}; }
    const 稳定编码 外设场景{static_cast<std::uint64_t>(*标识值)};
    if (!对应外设窗口) return 未支持("field question peripheral window mapping not supplied");
    const auto 外设窗口=对应外设窗口(外设场景,scene.查询参数(3)->信息节点);
    if (!外设窗口) { 字段询问材料诊断行=__LINE__; return {R::前提已变化,{}}; }
    const auto q=scene.读取正式信息参数保护(1);
    if (!正式读取成功(q)) { 字段询问材料诊断行=__LINE__; return {R::前提已变化,{}}; }
    if (!q.信息->引用概念语义 || !q.信息->引用概念语义->引用内容定位) return 未支持("question demand has no reference content locator");
    要求(q.信息->信息节点==scene.查询参数(1)->信息节点 && q.信息->类别概念==c.补问需求概念,"formal locator projection identity mismatch");
    const auto loc=*q.信息->引用概念语义->引用内容定位;
    if (loc.正式内容路径.size()!=1 || std::find(c.支持字段类型组.begin(),c.支持字段类型组.end(),loc.正式内容路径.front())==c.支持字段类型组.end()) return 未支持("question demand locator path is not a single supported field");
    const auto d=reader.读取正式信息参数保护(loc.信息节点,c.需求根概念);
    if (!正式读取成功(d)) { 字段询问材料诊断行=__LINE__; return {R::前提已变化,{}}; }
    要求(d.信息->信息节点==loc.信息节点 && d.信息->类别概念==c.需求根概念,"formal target projection identity mismatch");
    新字段询问材料 m;m.定位=loc;m.定位需求=q;m.待完善需求=d;
    m.自我=scene.查询参数(2)->信息节点;
    m.窗口副本=scene.查询参数(3)->信息节点;m.标识特征=scene.查询参数(4)->信息节点;
    m.窗口=外设窗口->first;m.输出特征=外设窗口->second;
    // 交互者（受话者）＝参数2的“交互者”特征当前值；读取失败或无当前值按复核不通过。
    if (!读取当前交互者) return 未支持("field question interactor reader not supplied");
    const auto 交互者=读取当前交互者(m.自我);
    if (!交互者) { 字段询问材料诊断行=__LINE__; return {R::前提已变化,{}}; }
    m.交互者=*交互者;
    // 交互者可以是自我（自问自答），不作断言。
    const auto& roles=scene.全部角色绑定();
    if (!roles[1].世界场景节点 || !roles[2].世界场景节点) return 未支持("field question binding world scene missing");
    m.执行场景=*roles[1].世界场景节点;m.外设场景=外设场景;
    const 正式内容值* value=nullptr;
    for (const auto& item:d.信息->直接内容组)
        if (item.特征类型==loc.正式内容路径.front()) { 要求(!value,"formal slot repeated");value=&item.内容; }
    要求(value,"formal field question projection missing required slot");
    // §6.6：不再核对“D需求主体＝受询者”；D主体是否为交互者只影响成文（“你”或第三方指代）。
    m.字段内容=*value;
    m.表达上下文={loc,c.需求根概念,c.语言节点,m.自我,m.交互者,m.窗口,m.外设场景,c.第三方指代概念,c.第三方别名关系};
    if (value->状态==正式内容值状态::不适用) return {R::字段不适用,std::move(m)};
    if (value->状态==正式内容值状态::已知 && value->值) return {R::字段已有值,std::move(m)};
    要求(value->状态==正式内容值状态::当前未知 && !value->值,"formal unknown slot has contradictory payload");
    // 被问结构（7240 §5.5；顶层裁决 2026-11-12）：目标形式及载荷类字段统一映射为
    // （主体＝D的目标存在，被问栏＝D的目标特征类型）；其它字段或映射所需D字段未知时无法映射，按工程断言。
    const auto 字段=loc.正式内容路径.front();
    if (字段==c.需求目标存在类型 || 字段==c.需求目标特征类型)
        return 未支持("field question slot has no (subject, asked field) mapping");
    const auto 读引用=[&](稳定编码 类型)->std::optional<稳定编码> {
        for (const auto& item:d.信息->直接内容组) if (item.特征类型==类型) {
            if (item.内容.状态!=正式内容值状态::已知 || !item.内容.值) return std::nullopt;
            const auto* v=std::get_if<稳定编码>(&*item.内容.值);
            return v && 有效(*v) ? std::optional{*v} : std::nullopt;
        }
        return std::nullopt;
    };
    const auto 主体=读引用(c.需求目标存在类型),被问栏=读引用(c.需求目标特征类型);
    if (!主体 || !被问栏) return 未支持("field question subject mapping requires known target existence and feature type");
    m.问题主体=*主体;m.被问栏=*被问栏;
    return {R::已取得,std::move(m)};
}
} // namespace 海中鱼巣
