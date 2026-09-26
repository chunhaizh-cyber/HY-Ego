#include "../海中鱼巣/装配.普通应用.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string_view>

namespace {
using namespace 海中鱼巣;

[[noreturn]] void 失败(std::string_view s){throw std::runtime_error(std::string(s));}
void 检查(bool ok,std::string_view s){if(!ok)失败(s);std::cout<<"PASS "<<s<<'\n';}

普通应用配置 配置(const std::filesystem::path& root){
  普通应用配置 c;c.L1事实基座持久存储={L1事实基座持久恢复合同版本_v1,root};
  c.自我线程邮箱容量=8;c.自我线程进入停门等待毫秒=10000;
  c.自我线程停止回收诊断等待毫秒=10000;return c;
}

本能根运行锚点_v1 初始化(const std::filesystem::path& root){
  检查(构造普通应用上下文(配置(root)).成功(),"ordinary application assembled");
  const auto self=初始化普通应用自我();检查(self.请求回显&&self.成功(*self.请求回显),"self ready");
  const auto method=初始化普通应用方法登记根();检查(method.成功(),"method root ready");
  const auto anchor=初始化普通应用本能根运行锚点(method);
  检查(anchor.成功()&&anchor.锚点&&anchor.锚点->完整(),"instinct roots ready");
  return *anchor.锚点;
}

本能根任务初始化语义请求_v1 请求(const 本能根运行单根锚点_v1& root,
    本能根角色 role,std::uint64_t H,std::uint64_t intent){
  本能根任务初始化语义请求_v1 r;
  r.意图={{intent}};r.期望事实代次=H;
  r.来源={role,{root.需求},root.列表项,root.实际特征};
  return r;
}

void 保存包(const std::filesystem::path& path,const 不可变本能根任务初始化包_v1& p){
  std::ofstream o(path,std::ios::trunc);if(!o)失败("package file open");
  o<<p.原请求.意图.值.值<<' '<<p.原请求.期望事实代次<<' '
   <<static_cast<unsigned>(p.原请求.来源.根角色)<<' '<<p.原请求.来源.D.值.值<<' '
   <<p.原请求.来源.L.值<<' '<<p.原请求.来源.根形成F.编码.值<<' '
   <<p.预留记录.值.值<<' '<<p.预留序号<<' '
   <<p.控制幂等身份.值<<' '<<p.任务核心建立幂等身份.值<<' '
   <<p.P1建立幂等身份.值<<' '<<p.Vt首迁移幂等身份.值<<' '
   <<p.预留形成事实代次<<'\n';
}
std::filesystem::path 包路径(const std::filesystem::path& root){
  return root.parent_path()/(root.filename().wstring()+L".task-package.txt");
}

不可变本能根任务初始化包_v1 读取包(const std::filesystem::path& path){
  std::ifstream i(path);if(!i)失败("package file read");
  不可变本能根任务初始化包_v1 p;unsigned role=0;
  i>>p.原请求.意图.值.值>>p.原请求.期望事实代次>>role
   >>p.原请求.来源.D.值.值>>p.原请求.来源.L.值
   >>p.原请求.来源.根形成F.编码.值>>p.预留记录.值.值>>p.预留序号
   >>p.控制幂等身份.值>>p.任务核心建立幂等身份.值
   >>p.P1建立幂等身份.值>>p.Vt首迁移幂等身份.值>>p.预留形成事实代次;
  if(!i||role<1||role>2)失败("package file malformed");
  p.原请求.来源.根角色=static_cast<本能根角色>(role);return p;
}

void 验证首跑(const std::filesystem::path& root){
  const auto anchor=初始化(root);auto& service=本能根任务核心();
  const auto req=请求(anchor.安全根,本能根角色::安全,anchor.事实截止代次,0x544F300000000001ULL);
  const auto package=service.签发或恢复不可变初始化包(req);
  检查(package.成功(req)&&package.包,"initialization package issued");
  const auto byIntent=service.按初始化意图读取不可变包({1,req.意图});
  检查(byIntent.状态==本能根任务阶段状态_v1::已读取&&
      byIntent.包==package.包,"package read back by stable intent");
  const auto sameRootReq=请求(anchor.安全根,本能根角色::安全,package.事实代次,
      0x544F300000000002ULL);
  const auto sameRootPackage=service.签发或恢复不可变初始化包(sameRootReq);
  检查(sameRootPackage.成功(sameRootReq)&&sameRootPackage.包,
      "second same-root package issued before task formation");
  const auto formed=service.承接或建立任务(*package.包);
  检查(formed.成功(*package.包)&&formed.状态==本能根任务承接总状态_v1::已完成,
      "three-stage task formed");
  const auto attached=service.承接或建立任务(*sameRootPackage.包);
  检查(attached.成功(*sameRootPackage.包)
      &&attached.状态==本能根任务承接总状态_v1::已承接到当前任务
      &&attached.核心&&formed.核心&&attached.核心->T==formed.核心->T,
      "second intent attached to existing current task");
  const auto attachedReplay=service.恢复任务初始化(*sameRootPackage.包);
  检查(attachedReplay.成功(*sameRootPackage.包)
      &&attachedReplay.状态==本能根任务承接总状态_v1::精确重复
      &&attachedReplay.核心==attached.核心,"existing-task receipt replay stable");
  const auto replay=service.恢复任务初始化(*package.包);
  检查(replay.成功(*package.包)&&replay.状态==本能根任务承接总状态_v1::精确重复
      &&replay.核心==formed.核心,"three-stage replay stable");
  const 本能根任务目标投影读取请求_v1 tr{1,replay.事实代次,replay.核心->T};
  const auto target=service.按任务读取目标投影(tr);
  检查(target.成功(tr)&&target.投影->D==req.来源.D&&target.投影->L==req.来源.L,
      "current root target reconstructed");
  const 本能根任务当前资格退出请求_v1 er{1,replay.事实代次,replay.核心->T,
      replay.核心->L,本能根任务生命周期_v1::当前可治理,
      本能根任务生命周期_v1::已退出当前资格,{0x544F300000000101ULL}};
  const auto exited=service.退出任务当前资格(er);
  检查(exited.成功(er),"current qualification exited");
  检查(service.退出任务当前资格(er).成功(er),"exit replay stable");
  const auto serviceReq=请求(anchor.服务根,本能根角色::服务,exited.事实代次,
      0x544F300000000003ULL);
  const auto servicePackage=service.签发或恢复不可变初始化包(serviceReq);
  检查(servicePackage.成功(serviceReq)&&servicePackage.包,"service-root package issued");
  const auto serviceTask=service.承接或建立任务(*servicePackage.包);
  检查(serviceTask.成功(*servicePackage.包)
      &&serviceTask.状态==本能根任务承接总状态_v1::已完成,
      "service-root three-stage task formed");
  保存包(包路径(root),*package.包);
}

void 验证恢复(const std::filesystem::path& root){
  (void)初始化(root);auto& service=本能根任务核心();const auto package=读取包(包路径(root));
  const auto issued=service.签发或恢复不可变初始化包(package.原请求);
  检查(issued.成功(package.原请求)&&issued.包==package,"package recovered across process");
  const auto byIntent=service.按初始化意图读取不可变包({1,package.原请求.意图});
  检查(byIntent.状态==本能根任务阶段状态_v1::当前任务不可复用&&
      byIntent.包==package,"finished task reported by stable intent");
  const auto task=service.恢复任务初始化(package);
  检查(task.成功(package)&&task.状态==本能根任务承接总状态_v1::精确重复,
      "task recovered across process");
}
}

int main(int argc,char** argv){
  try{
    std::cout<<std::unitbuf;if(argc!=3)失败("usage: <seed|recover> <root>");
    const auto root=std::filesystem::absolute(argv[2]);
    if(std::string_view(argv[1])=="seed")验证首跑(root);
    else if(std::string_view(argv[1])=="recover")验证恢复(root);else 失败("unknown mode");
    return 0;
  }catch(const std::exception& e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}
  catch(...){std::cerr<<"FAIL unexpected exception\n";return 2;}
}
