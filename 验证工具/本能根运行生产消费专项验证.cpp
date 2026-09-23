#include "../海中鱼巣/启动.应用程序.h"
#include "../海中鱼巣/启动.程序运行宿主.h"
#include "../海中鱼巣/装配.普通应用.h"

#include <atomic>
#include <chrono>
#include <csignal>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>

namespace {

using namespace 海中鱼巣;

[[noreturn]] void 失败(const std::string_view message) {
  throw std::runtime_error(std::string(message));
}

void 检查(const bool value,const std::string_view message) {
  if(!value)失败(message);
  std::cout<<"PASS "<<message<<'\n';
}

普通应用配置 建立配置(const std::filesystem::path&root) {
  普通应用配置 config;
  config.L1事实基座持久存储={
      L1事实基座持久恢复合同版本_v1,root};
  config.自我线程邮箱容量=8;
  config.自我线程进入停门等待毫秒=10000;
  config.自我线程停止回收诊断等待毫秒=10000;
  return config;
}

void 验证配置(const std::filesystem::path&root) {
  检查(普通应用默认自我线程邮箱容量!=0&&
      普通应用默认自我线程进入停门等待毫秒!=0&&
      普通应用默认自我线程停止回收诊断等待毫秒!=0,
      "named defaults are nonzero");
  const auto valid=建立配置(root);
  检查(valid==valid,"configuration equality covers identical values");
  auto changed=valid;
  ++changed.自我线程邮箱容量;
  检查(!(valid==changed),"configuration equality observes mailbox capacity");
  changed=valid;
  ++changed.自我线程进入停门等待毫秒;
  检查(!(valid==changed),"configuration equality observes gate wait");
  changed=valid;
  ++changed.自我线程停止回收诊断等待毫秒;
  检查(!(valid==changed),"configuration equality observes recovery wait");

  auto bad=valid;
  bad.自我线程邮箱容量=0;
  检查(构造普通应用上下文(bad).状态==普通应用装配状态::入口拒绝,
      "zero mailbox rejected");
  bad=valid;
  bad.自我线程进入停门等待毫秒=0;
  检查(构造普通应用上下文(bad).状态==普通应用装配状态::入口拒绝,
      "zero gate wait rejected");
  bad=valid;
  bad.自我线程停止回收诊断等待毫秒=0;
  检查(构造普通应用上下文(bad).状态==普通应用装配状态::入口拒绝,
      "zero recovery wait rejected");
}

void 验证阶段链(const std::filesystem::path&root,const bool expectedRecovery) {
  const auto assembly=构造普通应用上下文(建立配置(root));
  检查(assembly.成功(),"ordinary context assembled");
  const auto self=初始化普通应用自我();
  检查(self.请求回显&&self.成功(*self.请求回显),"formal self ready");
  const auto method=初始化普通应用方法登记根();
  检查(method.成功()&&method.最终Gread!=0,"stage 19 ready");
  const auto anchor=初始化普通应用本能根运行锚点(method);
  检查(anchor.成功()&&anchor.锚点&&anchor.锚点->完整(),
      "stage 21 returns complete anchor");
  if(expectedRecovery)
    检查(anchor.状态==本能根运行初始化状态_v1::已恢复,
        "stage 21 recovers persistent roots");
  const auto created=创建并停门普通应用自我线程();
  检查(created.成功()&&created.见证&&created.见证->完整(),
      "stage 20 creates one self at closed gate");
  const auto repeated=创建并停门普通应用自我线程();
  检查(repeated.状态==自我线程操作状态::精确重复&&
      repeated.见证==created.见证,"stage 20 exact replay is stable");
  const auto refusedAnchor=初始化普通应用本能根运行锚点(method);
  检查(refusedAnchor.状态==本能根运行初始化状态_v1::入口拒绝,
      "stage 21 refuses while self lease is active");
  const auto stopped=停止并回收普通应用自我线程();
  检查(stopped.成功(),"self stops and joins");
  检查(创建并停门普通应用自我线程().状态==
      自我线程操作状态::入口拒绝,
      "failed stage 21 clears cached anchor");
  const auto stoppedAgain=停止并回收普通应用自我线程();
  检查(stoppedAgain.状态==自我线程操作状态::精确重复,
      "self stop replay is stable");
}

void 验证控制面板(const std::filesystem::path&root) {
  检查(构造普通应用上下文(建立配置(root)).成功(),
      "control-panel context assembled");
  const auto result=运行海中鱼巣({启动模式::普通控制面板});
  检查(result.模式==启动模式::普通控制面板&&
      result.状态==程序运行状态::宿主失败&&
      result.失败阶段==程序失败阶段::控制面板窗口,
      "control panel safely returns stage 12");
}

void 验证无窗口(const std::filesystem::path&root) {
  检查(构造普通应用上下文(建立配置(root)).成功(),
      "headless context assembled");
  std::atomic<bool> finished=false;
  std::thread signaler([&finished] {
    while(!finished.load(std::memory_order_relaxed)) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      接收程序停止信号(SIGINT);
    }
  });
  const auto result=运行海中鱼巣({启动模式::无窗口常驻});
  finished.store(true,std::memory_order_relaxed);
  signaler.join();
  检查(result.模式==启动模式::无窗口常驻&&result.成功()&&
      result.失败阶段==程序失败阶段::无,
      "headless host exits on signal after safe self recovery");
}

} // namespace

int main(int argc,char**argv) {
  try {
    std::cout<<std::unitbuf;
    if(argc!=3)失败("usage: <config|stage-first|stage-recover|ordinary|headless> <absolute-root>");
    const std::string_view mode=argv[1];
    const std::filesystem::path root=std::filesystem::absolute(argv[2]);
    if(mode=="config")验证配置(root);
    else if(mode=="stage-first")验证阶段链(root,false);
    else if(mode=="stage-recover")验证阶段链(root,true);
    else if(mode=="ordinary")验证控制面板(root);
    else if(mode=="headless")验证无窗口(root);
    else 失败("unknown mode");
    return 0;
  } catch(const std::exception&e) {
    std::cerr<<"FAIL "<<e.what()<<'\n';
    return 1;
  } catch(...) {
    std::cerr<<"FAIL unexpected exception\n";
    return 2;
  }
}
