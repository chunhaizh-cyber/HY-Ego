#include "本能字段询问.运行接口.h"
#include "新_方法参数场景.h"
#include <algorithm>
#include <stdexcept>
#include <utility>

namespace 海中鱼巣 {
namespace {
void 要求(bool ok,const char* text) { if (!ok) throw std::logic_error(text); }
bool 正式读取成功(const 正式信息参数读取结果& r) {
    if (r.状态==正式信息参数读取状态::前提已变化 || r.状态==正式信息参数读取状态::信息不存在) return false;
    要求(r.状态==正式信息参数读取状态::已读取 && r.信息 && !r.完整保护组.empty(),"formal field question reader unavailable");return true;
}
}
新字段询问材料读回 读取通用询问材料(const 方法参数场景& scene,
    const 新字段询问登记配置& c,const 正式信息参数读取接口& reader,const 新字段询问外设窗口对应& 对应外设窗口) {
    using R=新字段询问材料状态;
    if (scene.全部角色绑定().size()!=5) return {R::定位未支持,{}};
    for (std::uint32_t i=1;i<=5;++i) {
        const auto p=scene.查询参数(i);
        if (!p || p->信息类型!=(i==5 ? 新方法参数信息类型::特征 : 新方法参数信息类型::存在)) return {R::定位未支持,{}};
    }
    // 参数4为现实交互场景中的窗口副本，参数5为其外设场景标识特征（值为外设场景稳定编码）。
    const auto ident=scene.读取存在参数特征(4,c.外设场景标识类型);
    if (ident.状态==新方法参数特征读取状态::定义前提已变化 || ident.状态==新方法参数特征读取状态::特征未取得
        || ident.状态==新方法参数特征读取状态::当前值未取得)
        return {R::前提已变化,{}};
    if (ident.状态==新方法参数特征读取状态::特征匹配不唯一) return {R::定位未支持,{}};
    要求(ident.状态==新方法参数特征读取状态::已读取 && ident.特征.has_value(),"declared peripheral scene identifier unreadable");
    if (ident.特征->节点!=scene.查询参数(5)->信息节点) return {R::定位未支持,{}};
    const auto* 标识值=ident.特征->当前值 ? std::get_if<std::int64_t>(&*ident.特征->当前值) : nullptr;
    if (!标识值 || *标识值<=0) return {R::前提已变化,{}};
    const 稳定编码 外设场景{static_cast<std::uint64_t>(*标识值)};
    if (!对应外设窗口) return {R::定位未支持,{}};
    const auto 外设窗口=对应外设窗口(外设场景,scene.查询参数(4)->信息节点);
    if (!外设窗口) return {R::前提已变化,{}};
    const auto q=scene.读取正式信息参数保护(1);
    if (!正式读取成功(q)) return {R::前提已变化,{}};
    if (!q.信息->引用概念语义 || !q.信息->引用概念语义->引用内容定位) return {R::定位未支持,{}};
    要求(q.信息->信息节点==scene.查询参数(1)->信息节点 && q.信息->类别概念==c.需求根概念,"formal locator projection identity mismatch");
    const auto loc=*q.信息->引用概念语义->引用内容定位;
    if (loc.正式内容路径.size()!=1 || std::find(c.支持字段类型组.begin(),c.支持字段类型组.end(),loc.正式内容路径.front())==c.支持字段类型组.end()) return {R::定位未支持,{}};
    const auto d=reader.读取正式信息参数保护(loc.信息节点,c.需求根概念);
    if (!正式读取成功(d)) return {R::前提已变化,{}};
    要求(d.信息->信息节点==loc.信息节点 && d.信息->类别概念==c.需求根概念,"formal target projection identity mismatch");
    新字段询问材料 m;m.定位=loc;m.定位需求=q;m.待完善需求=d;
    m.自我=scene.查询参数(2)->信息节点;m.受询者=scene.查询参数(3)->信息节点;
    m.窗口副本=scene.查询参数(4)->信息节点;m.标识特征=scene.查询参数(5)->信息节点;
    m.窗口=外设窗口->first;m.输出特征=外设窗口->second;
    const auto& roles=scene.全部角色绑定();
    if (m.自我==m.受询者 || !roles[1].世界场景节点 || !roles[3].世界场景节点) return {R::定位未支持,{}};
    m.执行场景=*roles[1].世界场景节点;m.外设场景=外设场景;
    const 正式内容值* value=nullptr;const 正式内容值* subject=nullptr;
    for (const auto& item:d.信息->直接内容组) {
        if (item.特征类型==loc.正式内容路径.front()) { 要求(!value,"formal slot repeated");value=&item.内容; }
        if (item.特征类型==c.需求主体类型) { 要求(!subject,"formal subject repeated");subject=&item.内容; }
    }
    要求(value && subject,"formal field question projection missing required slots");
    if (subject->状态!=正式内容值状态::已知 || !subject->值
        || !std::holds_alternative<稳定编码>(*subject->值) || std::get<稳定编码>(*subject->值)!=m.受询者) return {R::前提已变化,{}};
    m.字段内容=*value;
    m.表达上下文={loc,c.需求根概念,c.语言节点,m.自我,m.受询者,m.窗口,m.外设场景,c.第三方指代概念,c.第三方别名关系};
    if (value->状态==正式内容值状态::不适用) return {R::字段不适用,std::move(m)};
    if (value->状态==正式内容值状态::已知 && value->值) return {R::字段已有值,std::move(m)};
    要求(value->状态==正式内容值状态::当前未知 && !value->值,"formal unknown slot has contradictory payload");
    return {R::已取得,std::move(m)};
}
} // namespace 海中鱼巣
