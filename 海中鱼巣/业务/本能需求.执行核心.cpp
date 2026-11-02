#include "本能需求.执行核心.h"

#include <stdexcept>
#include <utility>

namespace 海中鱼巣 {
namespace {
void 设置动作前返回(新本能函数本次条件结果对& out, 新本能需求材料状态 state) {
    switch(state) {
    case 新本能需求材料状态::材料待齐:
    case 新本能需求材料状态::需重新确认:
        out.结果.结果取得状态=新本能函数结果取得状态::待取得;
        return;
    case 新本能需求材料状态::参数不合法:
        out.结果.状态=新本能函数执行状态::参数不匹配;
        return;
    case 新本能需求材料状态::前提已变化:return;
    default:throw std::logic_error("unexpected demand material rejection state");
    }
}
}

新本能函数本次条件结果对 执行服务对象新需求核心(
    const 方法参数场景& scene, 新本能需求运行接口& service) {
    新本能函数本次条件结果对 out;
    const auto p1=scene.查询参数(1), p2=scene.查询参数(2), p3=scene.查询参数(3);
    if(scene.全部角色绑定().size()!=3 || !p1 || !p2 || !p3
        || p1->信息类型!=新方法参数信息类型::存在
        || p2->信息类型!=新方法参数信息类型::特征
        || p3->信息类型!=新方法参数信息类型::存在) {
        out.结果.状态=新本能函数执行状态::参数不匹配;
        return out;
    }
    const auto read=service.读取新需求材料(scene);
    if(read.状态!=新本能需求材料状态::已读取) {
        设置动作前返回(out,read.状态);
        return out;
    }
    if(!read.材料)throw std::logic_error("demand material success without content");
    const auto& material=*read.材料;
    if(material.服务对象!=p1->信息节点 || material.需求特征!=p2->信息节点
        || material.需求节点!=p3->信息节点
        || material.动作前需求.节点!=p3->信息节点
        || material.动作前需求.需求主体存在节点!=p1->信息节点
        || material.动作前需求.执行域!=新需求执行域::被认知
        || material.动作前需求特征.节点!=p2->信息节点)
        throw std::logic_error("demand material differs from supplied parameters");
    out.条件.执行分支节点=scene.执行分支节点();
    for(std::uint32_t n=1;n<=3;++n) {
        新本能函数条件角色材料 item;
        item.有序引用=*scene.查询参数(n);
        if(n==2)item.实际特征组.push_back({material.需求特征,material.动作前需求特征.当前值});
        out.条件.参数角色组.push_back(std::move(item));
    }
    const auto checked=service.复核新需求材料(scene,material);
    if(checked!=新本能需求材料状态::已读取) {
        设置动作前返回(out,checked);
        return out;
    }
    const auto action=service.开始本次需求动作(scene);
    if(!有效(action))throw std::logic_error("demand operation has no action record");
    out.结果.动作动态节点=action;
    out.结果.动作阶段=新本能函数动作阶段::已进入动作;
    out.结果.状态=新本能函数执行状态::动作结果未知;
    out.结果.结果取得状态=新本能函数结果取得状态::部分取得;
    const auto published=service.发布新需求(scene,material,action);
    out.结果.已形成状态节点组=published.结果状态节点组;
    switch(published.状态) {
    case 新本能需求发布状态::已落实并发布:
    case 新本能需求发布状态::已复用并发布:
        if(!published.需求节点 || !有效(*published.需求节点)||!published.已发布需求)
            throw std::logic_error("demand publication success without actual demand");
        {
            const auto& result=*published.已发布需求;
            if(result.需求节点!=*published.需求节点||result.需求节点!=p3->信息节点||result.主体存在节点!=p1->信息节点
                ||result.需求特征节点!=p2->信息节点||result.本次动作节点!=action
                ||!有效(result.结果场景节点)||!有效(result.首次发布动作节点))
                throw std::logic_error("published demand identity differs from supplied parameters/action");
            out.结果.已发布需求=result;
        }
        out.结果.状态=新本能函数执行状态::已按合同返回;
        out.结果.动作阶段=新本能函数动作阶段::已按函数合同返回;
        out.结果.结果取得状态=新本能函数结果取得状态::已取得;
        break;
    case 新本能需求发布状态::前提已变化:
    case 新本能需求发布状态::需重新确认:
    case 新本能需求发布状态::已建需求尚未发布:
    case 新本能需求发布状态::已发布但当前读回变化:
        if(published.已发布需求)throw std::logic_error("unpublished demand carried as a completed result");
        break;
    default:throw std::logic_error("unknown demand publication state");
    }
    return out;
}
} // namespace 海中鱼巣
