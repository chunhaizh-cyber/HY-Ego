#include "../海中鱼巣/装配.普通应用.cpp"

#include <chrono>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string_view>
#include <thread>

namespace {
using namespace 海中鱼巣;

[[noreturn]] void 失败(std::string_view 文本){throw std::runtime_error(std::string(文本));}
void 检查(bool 条件,std::string_view 文本){if(!条件)失败(文本);std::cout<<"PASS "<<文本<<'\n';}

普通应用配置 配置(const std::filesystem::path& 根){
  普通应用配置 c;
  c.L1事实基座持久存储={L1事实基座持久恢复合同版本_v1,根};
  c.自我线程邮箱容量=8;
  c.自我线程进入停门等待毫秒=10000;
  c.自我线程停止回收诊断等待毫秒=10000;
  return c;
}

void 初始化并消费(const std::filesystem::path& 根,const bool 恢复轮){
  const auto assembled=构造普通应用上下文(配置(根));
  if(!assembled.成功()){
    std::cerr<<"assembly_status="<<static_cast<unsigned>(assembled.状态);
    if(assembled.持久恢复)
      std::cerr<<" recovery_status="<<static_cast<unsigned>(assembled.持久恢复->状态);
    const bool 有见证=assembled.持久恢复&&assembled.持久恢复->恢复见证.has_value();
    std::cerr<<" root_generation="<<assembled.根事实代次
        <<" witness="<<static_cast<unsigned>(有见证);
    std::cerr<<'\n';
  }
  检查(assembled.成功(),"ordinary application assembled");
  const auto self=初始化普通应用自我();
  检查(self.请求回显&&self.成功(*self.请求回显),"formal self ready");
  const auto method=初始化普通应用方法登记根();
  检查(method.成功(),"method root ready");
  const auto roots=初始化普通应用本能根运行锚点(method);
  if(!roots.成功())std::cerr<<"roots_status="<<static_cast<unsigned>(roots.状态)<<'\n';
  检查(roots.成功()&&roots.锚点,"instinct roots ready");
  const auto created=创建并停门普通应用自我线程();
  if(!created.成功())std::cerr<<"create_status="<<static_cast<unsigned>(created.状态)
      <<" lifecycle="<<static_cast<unsigned>(created.生命周期)<<'\n';
  检查(created.成功(),"self thread stopped at governance gate");
  检查(启动并开放普通应用首个根治理批次().成功(),"first root-governance batch opened the gate");

  auto& context=*普通应用装配内部::上下文;
  const auto selfSnapshot=context.自我线程对象.读取诊断快照();
  检查(selfSnapshot.治理运行门开启&&
      selfSnapshot.生命周期==自我线程生命周期状态::治理中&&
      selfSnapshot.成功治理批次数量>=1,"self published governance-loop witness");

  const auto deadline=std::chrono::steady_clock::now()+std::chrono::seconds(10);
  任务管理线程诊断快照_v1 manager;
  do{
    manager=context.任务管理线程对象.读取诊断快照();
    if(manager.已完成承接数量>=2||manager.具名失败数量||manager.内部错误锁存)break;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }while(std::chrono::steady_clock::now()<deadline);
  检查(manager.已完成承接数量>=2&&manager.具名失败数量==0&&
      !manager.内部错误锁存,"manager completed both root task initializations");

  const auto 核验意图=[&](本能根任务初始化意图身份_v1 意图,
                          本能根角色 角色,const char* 名称){
    const auto package=context.本能根任务核心->按初始化意图读取不可变包({1,意图});
    检查(package.状态==本能根任务阶段状态_v1::已读取&&package.包,
        名称);
    const 本能根任务锚点读取请求_v1 coreRequest{
        1,package.Gread,package.包->原请求.来源.L};
    const auto core=context.本能根任务核心->按查询锚点读取当前任务(coreRequest);
    检查(core.成功(coreRequest)&&
        core.核心&&core.核心->首次来源.根角色==角色&&
        core.核心->Vt状态==本能根任务Vt状态_v1::待找方法&&core.核心->P1,
        "task core independently read back at pending-method state");
    const auto target=context.本能根任务核心->按任务读取目标投影(
        {1,package.Gread,core.核心->T});
    检查(target.状态==本能根任务阶段状态_v1::已读取&&target.投影&&
        target.投影->根角色==角色&&
        target.投影->D==package.包->原请求.来源.D&&
        target.投影->L==package.包->原请求.来源.L,
        "task target independently read back");
  };
  核验意图(普通应用装配内部::首次安全根任务意图,本能根角色::安全,
      "safety package recovered by stable intent");
  核验意图(普通应用装配内部::首次服务根任务意图,本能根角色::服务,
      "service package recovered by stable intent");

  检查(停止并回收普通应用治理线程().成功(),"self then manager stopped and joined");
  if(恢复轮)std::cout<<"PASS recover process converged\n";
  else std::cout<<"PASS seed process converged\n";
}
}

int main(int argc,char** argv){
  try{
    std::cout<<std::unitbuf;
    if(argc!=3)失败("usage: <seed|recover> <absolute-root>");
    const std::string_view mode=argv[1];
    if(mode!="seed"&&mode!="recover")失败("unknown mode");
    初始化并消费(std::filesystem::absolute(argv[2]),mode=="recover");
    return 0;
  }catch(const std::exception& e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}
  catch(...){std::cerr<<"FAIL unexpected exception\n";return 2;}
}
