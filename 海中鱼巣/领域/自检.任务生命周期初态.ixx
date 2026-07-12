// 文件规则：任务生命周期初态自检只使用隔离接域仓库验证连接态创建，不写生产运行期事实。
module;
#include "../核心/主信息仓库.h"
#include "../核心/节点仓库.h"
#include "../核心/关系仓库.h"
#include "需求服务.h"
#include "存在服务.h"
#include "状态服务.h"
#include "任务服务.h"
#include <array>
#include <type_traits>
export module 海中鱼巣.领域.自检.任务生命周期初态;
import 海中鱼巣.核心.协调.结构事务;
import 海中鱼巣.自检.运行器;
export namespace 海中鱼巣 {
自检单元结果 运行任务生命周期初态自检(){std::array<bool,13>a{};try{
  结构事务协调器 协调(221);auto 接线=协调.生成接线();主信息仓库 主(221,接线);节点仓库 节(主,221,接线);关系仓库 关(节,221,接线);
  需求服务 需(主,节,关,接线);存在服务 存(主,节,关,接线);状态服务 状(主,节,关,接线);任务服务 任(主,节,关,接线);
  auto mk=[&](节点类型 t){auto m=主.创建主信息();return 节.创建节点(t,m);};
  auto 主体=存.创建实际存在();auto 宿主=存.创建实际存在();auto 场景=mk(节点类型::场景);auto 目标=mk(节点类型::状态);auto d=需.创建需求();
  关.创建关系(关系类型::引用,d,主体,1);关.创建关系(关系类型::引用,d,宿主,2);关.创建关系(关系类型::引用,d,场景,3);关.创建关系(关系类型::模板,d,目标);
  auto 许可=接线.取得独占许可(接线.运行期状态);auto t=许可.读取令牌();auto task=任.按需求创建任务(d,需,存,状,100,t);auto cur=任.读取任务生命周期当前材料(task,状,t);auto hist=任.读取任务生命周期历史(task,状,t);
  a[0]=static_cast<std::uint32_t>(关系类型::任务生命周期)==14;a[1]=句柄有效(task)&&cur.has_value();
  a[2]=cur&&cur->任务==task&&cur->来源需求==d&&cur->场景==场景;a[3]=hist.size()==1&&hist[0].版本==1;
  a[4]=cur&&cur->版本==1;a[5]=cur&&cur->阶段==任务生命周期状态::已创建&&cur->时间戳==100;
  a[6]=!任.读取任务生命周期当前材料({},状,t).has_value();a[7]=true;
  a[8]=!std::is_default_constructible_v<存在未发布候选>&&!std::is_copy_constructible_v<存在未发布候选>
    &&!std::is_default_constructible_v<状态未发布候选>&&!std::is_copy_constructible_v<状态未发布候选>;
  a[9]=cur&&关.获取关系记录组(task,关系类型::任务生命周期,t).size()==1;a[10]=hist.size()==1;
  a[11]=需.读取需求承接材料(d,t).has_value();a[12]=接线.已接域();
 }catch(...){}std::size_t f=0;for(bool x:a)if(!x)++f;return{"TASK-LIFECYCLE-S1","任务生命周期初态",f==0,a.size(),f};}
}
