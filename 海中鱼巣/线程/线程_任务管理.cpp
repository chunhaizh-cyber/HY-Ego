#include "线程_任务管理.h"

#include <chrono>
#include <condition_variable>
#include <deque>
#include <limits>
#include <mutex>
#include <new>
#include <thread>

namespace 海中鱼巣 {
namespace {
std::chrono::milliseconds 等待时长(std::uint64_t v) noexcept{
  using R=std::chrono::milliseconds::rep;
  constexpr auto m=(std::numeric_limits<R>::max)();
  return std::chrono::milliseconds{v>static_cast<std::uint64_t>(m)?m:static_cast<R>(v)};
}
bool 包完整(const 不可变本能根任务初始化包_v1&p) noexcept{
  return p.原请求.合同版本==1&&有效(p.原请求.意图.值)&&p.原请求.期望事实代次&&
      (p.原请求.来源.根角色==本能根角色::安全||p.原请求.来源.根角色==本能根角色::服务)&&
      有效(p.原请求.来源.D.值)&&有效(p.原请求.来源.L)&&有效(p.原请求.来源.根形成F)&&
      有效(p.预留记录.值)&&p.预留序号&&有效(p.控制幂等身份)&&
      有效(p.任务核心建立幂等身份)&&有效(p.P1建立幂等身份)&&
      有效(p.Vt首迁移幂等身份)&&p.预留形成事实代次;
}
bool 消息完整(const 自我到任务管理本能根承接消息_v1&m) noexcept{
  return m.合同版本==1&&m.消息.完整()&&m.来源复核消息.完整()&&
      m.原请求.完整()&&包完整(m.初始化包);
}
bool 可恢复(本能根任务承接总状态_v1 s) noexcept{
  return s==本能根任务承接总状态_v1::事实代次漂移||
      s==本能根任务承接总状态_v1::已可能发布||
      s==本能根任务承接总状态_v1::资源失败;
}
bool 同轮立即恢复(本能根任务承接总状态_v1 s) noexcept{
  return s==本能根任务承接总状态_v1::事实代次漂移||
      s==本能根任务承接总状态_v1::已可能发布;
}
bool 承接成功(const 本能根任务承接结果_v1&r,
              const 不可变本能根任务初始化包_v1&p) noexcept{
  return (r.状态==本能根任务承接总状态_v1::已完成||
          r.状态==本能根任务承接总状态_v1::精确重复||
          r.状态==本能根任务承接总状态_v1::已承接到当前任务)&&
      r.合同版本==1&&r.原包==p&&r.事实代次&&r.核心;
}
} // namespace

struct 任务管理线程::实现 final{
  mutable std::mutex 互斥;std::condition_variable 变化;std::mutex 加入互斥;
  std::deque<自我到任务管理本能根承接消息_v1> 邮箱;
  std::optional<自我到任务管理本能根承接消息_v1> 当前;
  std::optional<任务管理线程创建请求_v1> 锁定请求;
  本能根任务核心端口_v1* owner=nullptr;std::thread 线程;
  任务管理线程生命周期_v1 生命周期=任务管理线程生命周期_v1::未创建;
  bool 创建已尝试=false,线程已进入=false,停止=false,线程已完成=false,内部错误=false;
  std::uint64_t 已接收=0,已完成=0,失败=0,停机未完成=0,重试=0;
  本能根任务阶段状态_v1 最近=本能根任务阶段状态_v1::未执行;

  bool 核验读回(const 本能根任务承接结果_v1&r) noexcept{
    if(!r.核心||!owner)return false;
    const 本能根任务身份读取请求_v1 cq{1,r.事实代次,r.核心->T};
    const auto core=owner->按任务读取核心(cq);
    if(!core.成功(cq)||!core.核心||*core.核心!=*r.核心||
       core.核心->Vt状态!=本能根任务Vt状态_v1::待找方法||!core.核心->P1)return false;
    const 本能根任务目标投影读取请求_v1 tq{1,r.事实代次,r.核心->T};
    const auto target=owner->按任务读取目标投影(tq);
    return r.核心->首次来源==r.原包.原请求.来源&&
        r.核心->D==r.原包.原请求.来源.D&&
        r.核心->L==r.原包.原请求.来源.L&&
        target.成功(tq)&&target.投影&&target.投影->D==r.核心->D&&
        target.投影->L==r.核心->L&&target.投影->根角色==r.核心->首次来源.根角色&&
        target.投影->根角色==r.原包.原请求.来源.根角色&&
        r.核心->首次来源.根形成F==r.原包.原请求.来源.根形成F;
  }
  enum class 处理结果{成功,可恢复,失败};
  处理结果 处理(const 自我到任务管理本能根承接消息_v1&m) noexcept{
    if(!owner||!消息完整(m))return 处理结果::失败;
    auto result=owner->恢复任务初始化(m.初始化包);
    if(同轮立即恢复(result.状态))result=owner->恢复任务初始化(m.初始化包);
    {
      std::lock_guard lock{互斥};
      if(result.核心阶段!=本能根任务阶段状态_v1::未执行)
        最近=result.核心阶段;
      else if(result.状态==本能根任务承接总状态_v1::事实代次漂移)
        最近=本能根任务阶段状态_v1::事实代次漂移;
      else if(result.状态==本能根任务承接总状态_v1::已可能发布)
        最近=本能根任务阶段状态_v1::已可能发布;
      else if(result.状态==本能根任务承接总状态_v1::资源失败)
        最近=本能根任务阶段状态_v1::资源失败;
      else if(result.状态==本能根任务承接总状态_v1::内部不一致)
        最近=本能根任务阶段状态_v1::内部不一致;
      else 最近=本能根任务阶段状态_v1::未执行;
    }
    if(承接成功(result,m.初始化包)&&核验读回(result))return 处理结果::成功;
    if(可恢复(result.状态))return 处理结果::可恢复;
    return 处理结果::失败;
  }
  void 运行() noexcept{
    try{
      std::unique_lock lock{互斥};线程已进入=true;生命周期=任务管理线程生命周期_v1::接收中;变化.notify_all();
      while(true){
        变化.wait(lock,[&]{return 停止||!邮箱.empty();});
        if(!当前&&邮箱.empty()&&停止)break;
        if(!当前&&!邮箱.empty()){当前=std::move(邮箱.front());邮箱.pop_front();}
        if(!当前)continue;
        const auto message=*当前;
        lock.unlock();const auto outcome=处理(message);lock.lock();
        if(outcome==处理结果::成功){++已完成;当前.reset();变化.notify_all();continue;}
        if(outcome==处理结果::失败){++失败;当前.reset();变化.notify_all();continue;}
        if(停止){++停机未完成;当前.reset();变化.notify_all();continue;}
        ++重试;
        变化.wait_for(lock,等待时长(根治理可恢复失败重试间隔毫秒_v1),[&]{return 停止;});
      }
      生命周期=任务管理线程生命周期_v1::已停止;线程已完成=true;变化.notify_all();
    }catch(...){std::lock_guard lock{互斥};内部错误=true;停止=true;线程已完成=true;
      生命周期=任务管理线程生命周期_v1::内部错误;变化.notify_all();}
  }
  bool 加入() noexcept{std::lock_guard lock{加入互斥};if(!线程.joinable())return false;
    try{线程.join();return true;}catch(...){return false;}}
};

任务管理线程::任务管理线程() noexcept:实现_(new(std::nothrow)实现{}){}
任务管理线程::~任务管理线程() noexcept{if(!实现_)return;请求停止();(void)实现_->加入();delete 实现_;实现_=nullptr;}

任务管理线程操作结果_v1 任务管理线程::创建并等待就绪(
    const 任务管理线程创建请求_v1&r,本能根任务核心端口_v1& owner,
    std::uint64_t wait) noexcept{
  if(!实现_||r.合同版本!=1||!r.邮箱容量||!wait)return {};
  try{
    std::unique_lock lock{实现_->互斥};bool creator=false;
    if(实现_->创建已尝试){
      if(!实现_->锁定请求||*实现_->锁定请求!=r||实现_->owner!=&owner)
        return {任务管理线程操作状态_v1::选择冲突,1,实现_->生命周期};
      if(实现_->线程已进入&&!实现_->线程已完成)
        return {任务管理线程操作状态_v1::精确重复,1,实现_->生命周期};
      return {};
    }
    实现_->创建已尝试=true;实现_->锁定请求=r;实现_->owner=&owner;
    实现_->生命周期=任务管理线程生命周期_v1::启动中;creator=true;
    lock.unlock();
    try{实现_->线程=std::thread([p=实现_]{p->运行();});}
    catch(...){lock.lock();实现_->生命周期=任务管理线程生命周期_v1::内部错误;
      实现_->内部错误=true;实现_->线程已完成=true;return {任务管理线程操作状态_v1::资源失败,1,实现_->生命周期};}
    lock.lock();
    if(!实现_->变化.wait_for(lock,等待时长(wait),[&]{return 实现_->线程已进入||实现_->线程已完成;}))
      return {任务管理线程操作状态_v1::等待超时,1,实现_->生命周期};
    if(creator&&实现_->线程已进入&&实现_->生命周期==任务管理线程生命周期_v1::接收中)
      return {任务管理线程操作状态_v1::已创建,1,实现_->生命周期};
  }catch(...){
    const auto life=实现_ ? 实现_->生命周期 : 任务管理线程生命周期_v1::内部错误;
    return {任务管理线程操作状态_v1::内部不一致,1,life};
  }
  return {};
}
bool 任务管理线程::已就绪() const noexcept{if(!实现_)return false;std::lock_guard l{实现_->互斥};
  return 实现_->生命周期==任务管理线程生命周期_v1::接收中&&!实现_->停止&&!实现_->内部错误;}
根治理意图投递结果_v1 任务管理线程::提交(
    const 自我到任务管理本能根承接消息_v1&m) noexcept{
  根治理意图投递结果_v1 out;out.消息=m.消息;
  if(!实现_||!消息完整(m))return out;
  try{std::lock_guard lock{实现_->互斥};
    if(实现_->停止){out.状态=根治理意图投递状态_v1::正在停止;return out;}
    if(实现_->生命周期!=任务管理线程生命周期_v1::接收中){out.状态=根治理意图投递状态_v1::尚未就绪;return out;}
    const auto check=[&](const auto&x){return x.消息==m.消息;};
    if(实现_->当前&&check(*实现_->当前)){
      out.状态=*实现_->当前==m
          ? 根治理意图投递状态_v1::精确重复
          : 根治理意图投递状态_v1::消息冲突;
      if(out.状态==根治理意图投递状态_v1::精确重复)实现_->变化.notify_all();
      return out;
    }
    for(const auto&x:实现_->邮箱)if(check(x)){
      out.状态=x==m
          ? 根治理意图投递状态_v1::精确重复
          : 根治理意图投递状态_v1::消息冲突;
      return out;
    }
    if(实现_->邮箱.size()>=实现_->锁定请求->邮箱容量){out.状态=根治理意图投递状态_v1::队列已满;return out;}
    实现_->邮箱.push_back(m);++实现_->已接收;out.状态=根治理意图投递状态_v1::已接收;实现_->变化.notify_all();
  }catch(const std::bad_alloc&){out.状态=根治理意图投递状态_v1::资源失败;}
   catch(...){out.状态=根治理意图投递状态_v1::内部不一致;}
  return out;
}
任务管理线程操作结果_v1 任务管理线程::请求停止() noexcept{if(!实现_)return {};
  std::lock_guard l{实现_->互斥};if(!实现_->创建已尝试)return {};
  实现_->停止=true;if(!实现_->线程已完成)实现_->生命周期=任务管理线程生命周期_v1::停止排空中;
  实现_->变化.notify_all();return {任务管理线程操作状态_v1::已请求停止,1,实现_->生命周期};}
任务管理线程操作结果_v1 任务管理线程::等待停止(std::uint64_t wait) noexcept{
  if(!实现_||!wait)return {};
  std::unique_lock l{实现_->互斥};
  if(!实现_->变化.wait_for(l,等待时长(wait),[&]{return 实现_->线程已完成;}))
    return {任务管理线程操作状态_v1::等待超时,1,实现_->生命周期};
  l.unlock();if(!实现_->加入())return {任务管理线程操作状态_v1::内部不一致,1,实现_->生命周期};
  return {任务管理线程操作状态_v1::已停止,1,任务管理线程生命周期_v1::已停止};
}
任务管理线程诊断快照_v1 任务管理线程::读取诊断快照() const noexcept{
  任务管理线程诊断快照_v1 o;if(!实现_){o.生命周期=任务管理线程生命周期_v1::内部错误;o.内部错误锁存=true;return o;}
  std::lock_guard l{实现_->互斥};o.生命周期=实现_->生命周期;o.线程已进入=实现_->线程已进入;
  o.停止已请求=实现_->停止;o.线程已完成=实现_->线程已完成;o.邮箱数量=实现_->邮箱.size();
  o.邮箱容量=实现_->锁定请求 ? 实现_->锁定请求->邮箱容量 : 0;
  if(实现_->当前)o.当前消息=实现_->当前->消息;
  o.已接收数量=实现_->已接收;o.已完成承接数量=实现_->已完成;o.具名失败数量=实现_->失败;
  o.停机未完成数量=实现_->停机未完成;o.自动重试数量=实现_->重试;o.最近owner状态=实现_->最近;o.内部错误锁存=实现_->内部错误;return o;
}

} // namespace 海中鱼巣
