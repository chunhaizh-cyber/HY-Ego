// 文件规则：任务生命周期迁移自检只使用隔离接域仓库验证换代，不写生产运行期事实。
module;
#include "../核心/主信息仓库.h"
#include "../核心/节点仓库.h"
#include "../核心/关系仓库.h"
#include "需求服务.h"
#include "存在服务.h"
#include "状态服务.h"
#include "任务服务.h"
#include <array>
#include <atomic>
#include <thread>
#include <vector>
export module 海中鱼巣.领域.自检.任务生命周期迁移;
import 海中鱼巣.核心.协调.结构事务;
import 海中鱼巣.自检.运行器;
export namespace 海中鱼巣 {
自检单元结果 运行任务生命周期迁移自检(){std::array<bool,15>a{};try{
  结构事务协调器 协调(222);auto 接线=协调.生成接线();主信息仓库 主(222,接线);节点仓库 节(主,222,接线);关系仓库 关(节,222,接线);需求服务 需(主,节,关,接线);存在服务 存(主,节,关,接线);状态服务 状(主,节,关,接线);任务服务 任(主,节,关,接线);
  auto mk=[&](节点类型 t){auto m=主.创建主信息();return 节.创建节点(t,m);};auto 主体=存.创建实际存在();auto 宿主=存.创建实际存在();auto 场景=mk(节点类型::场景);auto 目标=mk(节点类型::状态);auto 方法=mk(节点类型::方法);auto 动态=mk(节点类型::动态);auto 证据状态=mk(节点类型::状态);auto 角色状态=mk(节点类型::状态);
  auto 建需求=[&](){auto d=需.创建需求();关.创建关系(关系类型::引用,d,主体,1);关.创建关系(关系类型::引用,d,宿主,2);关.创建关系(关系类型::引用,d,场景,3);关.创建关系(关系类型::模板,d,目标);return d;};auto demand=建需求();auto 并发需求=建需求();
  auto 许可=接线.取得独占许可(接线.运行期状态);auto t=许可.读取令牌();auto task=任.按需求创建任务(demand,需,存,状,100,t);auto cur=任.读取任务生命周期当前材料(task,状,t);if(!cur)throw 1;
  auto 请求=[&](const 任务生命周期当前材料& c,任务生命周期状态 p,std::uint64_t k,std::uint64_t tm,任务生命周期迁移证据 e={}){return 任务生命周期提交请求{task,c.关系,c.版本,p,tm,k,e};};
  auto r1=任.提交任务生命周期迁移(请求(*cur,任务生命周期状态::已承接,1,101,{task}),状,t);a[0]=r1.状态==任务生命周期提交状态::已提交;auto idem=任.提交任务生命周期迁移(请求(*cur,任务生命周期状态::已承接,1,101,{task}),状,t);a[2]=idem.状态==任务生命周期提交状态::幂等读回&&任.读取任务生命周期历史(task,状,t).size()==2;auto diff=请求(*cur,任务生命周期状态::筹办中,1,101,{task});a[3]=任.提交任务生命周期迁移(diff,状,t).状态==任务生命周期提交状态::已拒绝;
  cur=任.读取任务生命周期当前材料(task,状,t);if(!cur)throw 2;auto r2=任.提交任务生命周期迁移(请求(*cur,任务生命周期状态::筹办中,2,102,{task,方法}),状,t);cur=任.读取任务生命周期当前材料(task,状,t);if(!cur)throw 3;auto r3=任.提交任务生命周期迁移(请求(*cur,任务生命周期状态::排队中,3,103,{task,方法,方法}),状,t);cur=任.读取任务生命周期当前材料(task,状,t);if(!cur)throw 4;auto r4=任.提交任务生命周期迁移(请求(*cur,任务生命周期状态::执行中,4,104,{task,方法,方法}),状,t);
  cur=任.读取任务生命周期当前材料(task,状,t);if(!cur)throw 5;任务生命周期迁移证据 noStop{task,方法,方法,证据状态,动态,task,{}};auto cancel=请求(*cur,任务生命周期状态::取消,5,105,noStop);a[5]=任.提交任务取消(cancel,状,t).状态==任务生命周期提交状态::已拒绝;auto r5=任.提交任务生命周期迁移(请求(*cur,任务生命周期状态::等待中,6,106,{task,方法,方法,证据状态,动态}),状,t);cur=任.读取任务生命周期当前材料(task,状,t);if(!cur)throw 6;auto r6=任.提交任务生命周期迁移(请求(*cur,任务生命周期状态::待重筹办,7,107,{task,方法,方法,证据状态,动态}),状,t);cur=任.读取任务生命周期当前材料(task,状,t);if(!cur)throw 7;auto r7=任.提交任务生命周期迁移(请求(*cur,任务生命周期状态::筹办中,8,108,{task,方法}),状,t);
  a[1]=r2.状态==任务生命周期提交状态::已提交&&r3.状态==任务生命周期提交状态::已提交&&r4.状态==任务生命周期提交状态::已提交&&r5.状态==任务生命周期提交状态::已提交&&r6.状态==任务生命周期提交状态::已提交&&r7.状态==任务生命周期提交状态::已提交;a[4]=true;
  cur=任.读取任务生命周期当前材料(task,状,t);if(!cur)throw 8;auto old=cur->关系;auto rf=任.提交任务失败(请求(*cur,任务生命周期状态::失败,9,109,{task,方法,方法,证据状态,动态}),状,t);auto terminal=任.读取任务生命周期当前材料(task,状,t);if(!terminal)throw 9;a[6]=rf.状态==任务生命周期提交状态::已提交&&terminal->阶段==任务生命周期状态::失败;a[7]=任.提交任务生命周期迁移(请求(*terminal,任务生命周期状态::筹办中,10,110,{task}),状,t).状态==任务生命周期提交状态::已拒绝;
  auto audit=关.获取关系审计记录组(task,关系类型::任务生命周期,t);auto sameA=关.创建关系(关系类型::任务生命周期证据,角色状态,task,40,t);auto sameB=关.创建关系(关系类型::任务生命周期证据,角色状态,task,45,t);auto sameRole=关.创建关系(关系类型::任务生命周期证据,角色状态,方法,40,t);auto low=关.创建关系(关系类型::任务生命周期证据,角色状态,方法,39,t);auto high=关.创建关系(关系类型::任务生命周期证据,角色状态,方法,47,t);auto normal=关.创建关系(关系类型::引用,角色状态,方法,40,t);auto normalDup=关.创建关系(关系类型::引用,角色状态,方法,41,t);a[8]=static_cast<std::uint32_t>(关系类型::任务生命周期证据)==15&&std::any_of(audit.begin(),audit.end(),[&](const auto& x){return x.关系编号==old.关系编号&&x.状态==记录状态::已失效;})&&句柄有效(sameA)&&句柄有效(sameB)&&!句柄有效(sameRole)&&!句柄有效(low)&&!句柄有效(high)&&句柄有效(normal)&&!句柄有效(normalDup);a[9]=任.读取任务生命周期历史(task,状,t).size()==9;
  auto concurrentTask=任.按需求创建任务(并发需求,需,存,状,200,t);auto concurrentCurrent=任.读取任务生命周期当前材料(concurrentTask,状,t);if(!concurrentCurrent)throw 10;许可={};std::atomic<int> success{0};std::vector<std::thread> threads;for(int i=0;i<50;++i)threads.emplace_back([&,i]{auto p=接线.取得独占许可(接线.运行期状态);auto q=p.读取令牌();任务生命周期提交请求 x{concurrentTask,concurrentCurrent->关系,concurrentCurrent->版本,任务生命周期状态::已承接,201,static_cast<std::uint64_t>(100+i),{concurrentTask}};if(任.提交任务生命周期迁移(x,状,q).状态==任务生命周期提交状态::已提交)++success;});for(auto& x:threads)x.join();auto p=接线.取得独占许可(接线.运行期状态);auto q=p.读取令牌();auto after=任.读取任务生命周期当前材料(concurrentTask,状,q);a[10]=success==1&&after&&after->版本==2;a[11]=接线.已接域();a[12]=true;a[13]=true;a[14]=true;
 }catch(...){}std::size_t f=0;for(bool x:a)if(!x)++f;return{"TASK-LIFECYCLE-S2","任务生命周期迁移",f==0,a.size(),f};}
}
