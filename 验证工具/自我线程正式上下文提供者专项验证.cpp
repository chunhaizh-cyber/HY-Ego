#include "../海中鱼巣/装配.普通应用.cpp"

#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>

namespace {

using namespace 海中鱼巣;

[[noreturn]] void 失败(const std::string_view 消息) {
  throw std::runtime_error(std::string(消息));
}

void 检查(const bool 条件, const std::string_view 消息) {
  if (!条件) 失败(消息);
  std::cout << "PASS " << 消息 << '\n';
}

普通应用配置 建立配置(const std::filesystem::path& 根) {
  普通应用配置 配置;
  配置.L1事实基座持久存储 = {L1事实基座持久恢复合同版本_v1, 根};
  配置.自我线程邮箱容量 = 8;
  配置.自我线程进入停门等待毫秒 = 10000;
  配置.自我线程停止回收诊断等待毫秒 = 10000;
  return 配置;
}

普通应用装配内部::普通应用上下文& 取得上下文() {
  if (!普通应用装配内部::上下文) 失败("ordinary context is absent");
  return *普通应用装配内部::上下文;
}

bool 正式上下文成功(const 自我线程正式上下文结果_v2& 结果) {
  return (结果.状态==自我线程外部调用状态::成功||
          结果.状态==自我线程外部调用状态::精确重复)&&
      结果.合同版本==自我线程正式上下文合同版本_v2&&
      结果.投影&&结果.投影->完整()&&!结果.写业务事实;
}

自我线程正式上下文请求_v2 建立上下文请求() {
  auto& 上下文 = 取得上下文();
  if (!上下文.本能根运行锚点 || !上下文.本能根运行自我投影 ||
      !上下文.本能根运行根场景)
    失败("runtime anchor cache is incomplete");
  const auto& 锚点 = *上下文.本能根运行锚点;
  const auto& 自我 = *上下文.本能根运行自我投影;
  if (!自我.位置.直接结构父) 失败("self direct parent is absent");

  自我线程正式上下文请求_v2 请求;
  请求.锁定世界 = {{上下文.本能根运行根场景->值},
      {上下文.本能根运行根场景->值}, 自我.Gread};
  请求.锁定自我 = {{自我.E.值}, {自我.世界根.值},
      {自我.位置.直接结构父->父.值}, 自我.Gread};
  请求.锁定本能根.自我 = {锚点.自我.编码.值};
  请求.锁定本能根.安全根 = {{锚点.安全根.需求.值},
      {锚点.安全根.列表项.值}, {锚点.安全根.实际特征.编码.值},
      {锚点.安全根.目标合同.值}};
  请求.锁定本能根.服务根 = {{锚点.服务根.需求.值},
      {锚点.服务根.列表项.值}, {锚点.服务根.实际特征.编码.值},
      {锚点.服务根.目标合同.值}};
  请求.锁定本能根.Gread = 锚点.事实截止代次;
  if (!请求.完整()) 失败("formal context request is incomplete");
  return 请求;
}

std::uint64_t 当前代次() {
  auto& 上下文 = 取得上下文();
  const auto 根 = 上下文.世界树->读取当前现实世界根_v3(
      {世界树现实根当前完整读取合同版本_v3});
  if (根.版本!=世界树现实根当前完整读取合同版本_v3||
      根.状态!=世界树现实根当前完整读取状态_v3::已读取||!根.树)
    失败("current world generation unavailable");
  return 根.Gread;
}

void 验证缺根纯读(const std::filesystem::path& 根) {
  检查(构造普通应用上下文(建立配置(根)).成功(), "missing-root context assembled");
  auto& 上下文 = 取得上下文();
  检查(上下文.自我线程正式上下文 != nullptr,
      "ordinary context owns exactly one formal-context provider");
  const auto 前 = 当前代次();
  const 本能根材料请求 请求{本能根材料合同版本, 前,
      本能根角色::安全, {1}};
  const auto 结果 = 上下文.需求->读取本能根材料(请求);
  检查(结果.状态 == 本能根材料状态::根材料未闭合 && !结果.材料,
      "missing first root material is a pure-read closure miss");
  检查(当前代次() == 前 && 上下文.需求->核验当前事实代次(前).成功(),
      "missing-root pure read writes no business fact");
}

void 验证方法门未就绪(const std::filesystem::path& 根) {
  检查(构造普通应用上下文(建立配置(根)).成功(), "dependency context assembled");
  const auto 自我 = 初始化普通应用自我();
  检查(自我.请求回显 && 自我.成功(*自我.请求回显), "formal self ready before dependency test");
  auto& 上下文 = 取得上下文();
  const auto 根读 = 上下文.世界树->读取当前现实世界根_v3(
      {世界树现实根当前完整读取合同版本_v3});
  检查(根读.状态==世界树现实根当前完整读取状态_v3::已读取&&根读.树,
      "world root ready before dependency test");
  const 真实自我当前完整读取请求_v2 自我请求{
      真实自我当前完整读取合同版本_v2,根读.Gread,根读.树->根场景,
      上下文.角色结构.项目角色};
  const auto 当前自我 = 上下文.自我->读取当前自我_v2(自我请求);
  检查(当前自我.状态==真实自我形成状态::已读取&&当前自我.投影 &&
      当前自我.投影->位置.直接结构父, "fresh self ready before dependency test");
  自我线程正式上下文请求_v2 请求;
  请求.锁定世界 = {{根读.树->根场景.值}, {根读.树->根场景.值}, 根读.Gread};
  请求.锁定自我 = {{当前自我.投影->E.值}, {当前自我.投影->世界根.值},
      {当前自我.投影->位置.直接结构父->父.值}, 当前自我.投影->Gread};
  请求.锁定本能根.自我 = {当前自我.投影->E.值};
  请求.锁定本能根.安全根 = {{11}, {12}, {13}, {14}};
  请求.锁定本能根.服务根 = {{21}, {22}, {23}, {24}};
  请求.锁定本能根.Gread = 根读.Gread;
  检查(请求.完整(), "dependency request is structurally complete");
  const auto 前 = 当前代次();
  const auto 结果 = 上下文.自我线程正式上下文->读取正式上下文(请求);
  检查(结果.状态 == 自我线程外部调用状态::依赖未就绪 &&
      !结果.投影 && !结果.写业务事实,
      "missing method root maps to dependency-not-ready");
  检查(当前代次() == 前, "dependency failure writes no business fact");
}

void 验证完整链(const std::filesystem::path& 根, const bool 期望恢复) {
  检查(构造普通应用上下文(建立配置(根)).成功(), "provider context assembled");
  auto 自我 = 初始化普通应用自我();
  检查(自我.请求回显 && 自我.成功(*自我.请求回显), "formal self initialized");
  const auto 方法 = 初始化普通应用方法登记根();
  检查(方法.成功(), "method registry gate initialized");
  const auto 锚点 = 初始化普通应用本能根运行锚点(方法);
  检查(锚点.成功() && 锚点.锚点, "instinct roots initialized");
  if (期望恢复)
    检查(锚点.状态 == 本能根运行初始化状态_v1::已恢复,
        "persistent instinct roots recovered");
  else
    检查(锚点.状态 == 本能根运行初始化状态_v1::已形成,
        "persistent instinct roots formed");

  auto& 上下文 = 取得上下文();
  检查(上下文.自我线程正式上下文 != nullptr,
      "production assembly retains the provider");
  const auto 请求 = 建立上下文请求();
  const auto 前 = 当前代次();
  const auto 结果 = 上下文.自我线程正式上下文->读取正式上下文(请求);
  检查(正式上下文成功(结果),
      "fresh formal context read succeeds");
  检查(结果.投影->Gread == 前 &&
      结果.投影->世界 == 请求.锁定世界.世界 &&
      结果.投影->自我 == 请求.锁定自我.自我 &&
      结果.投影->安全根 == 请求.锁定本能根.安全根 &&
      结果.投影->服务根 == 请求.锁定本能根.服务根,
      "formal context projection preserves locked identities at one cutoff");
  检查(当前代次() == 前 && !结果.写业务事实,
      "successful formal context read writes no business fact");

  auto 无效=请求;
  无效.合同版本=1;
  const auto 拒绝=上下文.自我线程正式上下文->读取正式上下文(无效);
  检查(拒绝.状态==自我线程外部调用状态::入口拒绝&&
      !拒绝.投影&&!拒绝.写业务事实,
      "legacy formal-context version is rejected");
  检查(当前代次() == 前, "version rejection writes no business fact");

  const 本能根材料请求 安全读取请求{
      本能根材料合同版本, 前, 本能根角色::安全,
      {请求.锁定本能根.安全根.实际特征.值}};
  const auto 安全读取 = 上下文.需求->读取本能根材料(安全读取请求);
  检查(安全读取.状态 == 本能根材料状态::已读取 &&
      安全读取.成功(安全读取请求) && 安全读取.材料,
      "existing safety-root material is read without write fallback");
  const 本能根材料请求 服务读取请求{
      本能根材料合同版本, 前, 本能根角色::服务,
      {请求.锁定本能根.服务根.实际特征.值}};
  const auto 服务读取 = 上下文.需求->读取本能根材料(服务读取请求);
  检查(服务读取.状态 == 本能根材料状态::已读取 &&
      服务读取.成功(服务读取请求) && 服务读取.材料,
      "existing service-root material is independently read");
  auto 过期根请求 = 安全读取请求;
  --过期根请求.期望事实代次;
  检查(过期根请求.期望事实代次 != 0 &&
      上下文.需求->读取本能根材料(过期根请求).状态 ==
          本能根材料状态::事实代次漂移,
      "stale root-material cutoff maps to generation drift");
  auto 异义根请求 = 安全读取请求;
  异义根请求.实际特征 = {请求.锁定自我.自我.值};
  检查(上下文.需求->读取本能根材料(异义根请求).状态 ==
          本能根材料状态::幂等冲突,
      "mismatched root feature maps to persistent-request conflict");
  auto 非法角色请求 = 安全读取请求;
  非法角色请求.角色 = static_cast<本能根角色>(255);
  检查(上下文.需求->读取本能根材料(非法角色请求).状态 ==
          本能根材料状态::入口拒绝,
      "unknown root role is rejected instead of aliasing the service root");

  const auto 检查引用冲突 = [&](自我线程正式上下文请求_v2 异义,
                                  const std::string_view 消息) {
    检查(异义.完整(), "mutated reference request remains structurally complete");
    const auto 冲突 = 上下文.自我线程正式上下文->读取正式上下文(异义);
    检查(冲突.状态 == 自我线程外部调用状态::引用冲突 &&
        !冲突.投影 && !冲突.写业务事实, 消息);
  };
  auto 世界异义 = 请求;
  世界异义.锁定世界.世界 = {请求.锁定自我.自我.值};
  世界异义.锁定世界.根场景 = {请求.锁定自我.自我.值};
  世界异义.锁定自我.世界 = {请求.锁定自我.自我.值};
  世界异义.锁定自我.所在场景 = {请求.锁定自我.自我.值};
  检查引用冲突(世界异义, "locked-world mismatch returns an empty reference conflict");
  auto 自我异义 = 请求;
  自我异义.锁定自我.自我 = {请求.锁定世界.世界.值};
  自我异义.锁定本能根.自我 = {请求.锁定世界.世界.值};
  检查引用冲突(自我异义, "locked-self mismatch returns an empty reference conflict");
  auto 根身份异义 = 请求;
  根身份异义.锁定本能根.安全根.需求 = {请求.锁定自我.自我.值};
  检查引用冲突(根身份异义,
      "locked root identity mismatch returns an empty reference conflict");
  auto 根特征异义 = 请求;
  根特征异义.锁定本能根.安全根.实际特征 = {请求.锁定自我.自我.值};
  检查引用冲突(根特征异义,
      "locked root feature mismatch returns an empty reference conflict");
  检查(当前代次() == 前,
      "root and provider failure-mapping probes write no business fact");

  const auto 创建 = 创建并停门普通应用自我线程();
  const auto 快照 = 上下文.自我线程对象.读取诊断快照();
  检查(创建.成功() && 快照.生命周期 == 自我线程生命周期状态::已停门 &&
      !快照.治理运行门开启 && 快照.成功治理批次数量 == 0,
      "provider ownership does not open the self-governance gate");
  检查(停止并回收普通应用自我线程().成功(), "self thread stops after ownership test");
}

void 验证移动后当前读回(const std::filesystem::path& 根) {
  检查(构造普通应用上下文(建立配置(根)).成功(), "move context assembled");
  const auto 自我形成 = 初始化普通应用自我();
  检查(自我形成.请求回显 && 自我形成.成功(*自我形成.请求回显) &&
      自我形成.投影, "strict first-birth self projection succeeds before move");
  const auto 初始 = *自我形成.投影;
  const auto 方法 = 初始化普通应用方法登记根();
  检查(方法.成功(), "method registry ready before move");
  const auto 锚点 = 初始化普通应用本能根运行锚点(方法);
  检查(锚点.成功() && 锚点.锚点, "instinct roots ready before move");

  auto& 上下文 = 取得上下文();
  const auto 世界根 = 初始.世界根;
  const 概念树预算 概念预算{256, 256, 0, 0, 64, 256, 0, 0};
  世界树场景创建请求_v4 场景请求{
      4,
      当前代次(),
      世界根,
      通用存在概念定义{1, 通用存在定义规则::不预设特征},
      概念初始组织指定::显式顶层,
      {},
      {0x53434601},
      存在场景概念创建键_v2{
          {0x53434602}, {0x53434603}, {0x53434604}, {0x53434605}},
      std::nullopt,
      {64, 256, 64},
      概念预算};
  const auto 新场景 = 上下文.世界树->创建场景并纳入现实世界(场景请求);
  检查(新场景.成功(场景请求) && 新场景.投影,
      "new parent scene is created under the world root");

  const 世界树成员移动请求 移动请求{
      2, 当前代次(), {0x53434606}, 世界根, 新场景.投影->E,
      初始.E, {64, 256}};
  const auto 移动 = 上下文.世界树->移动现实世界场景(移动请求);
  检查(移动.成功(移动请求), "self scene is legally moved below the new scene");

  const auto Gread = 当前代次();
  const 真实自我读取请求 读取请求{
      1, Gread, 世界根, 上下文.角色结构.项目角色,
      {64, 256, 64}, 概念预算};
  const auto 当前自我 = 上下文.自我->读取当前自我(读取请求);
  检查(当前自我.成功(读取请求) && 当前自我.投影 &&
      当前自我.投影->位置.直接结构父,
      "direct fresh self read succeeds after legal movement");
  const auto& 当前 = *当前自我.投影;
  检查(当前.E == 初始.E && 当前.世界根 == 初始.世界根 &&
      当前.角色.角色 == 初始.角色.角色 && 当前.角色.E == 初始.角色.E &&
      当前.角色.登记关系 == 初始.角色.登记关系 &&
      当前.角色.目标关系 == 初始.角色.目标关系 &&
      当前.角色.存在身份.身份 == 初始.角色.存在身份.身份 &&
      当前.概念.概念 == 初始.概念.概念 &&
      当前.使用.关系 == 初始.使用.关系 && 当前.使用.E == 初始.使用.E &&
      当前.使用.EC == 初始.使用.EC,
      "movement preserves E world role EC and concept-use identities");
  bool 四根不变 = true;
  for (std::size_t i = 0; i < 当前.场景.场景角色.四根.size(); ++i) {
    const auto& 左 = 当前.场景.场景角色.四根[i];
    const auto& 右 = 初始.场景.场景角色.四根[i];
    四根不变 = 四根不变 && 左.角色 == 右.角色 &&
        左.根.编码 == 右.根.编码 && 左.绑定.编码 == 右.绑定.编码;
  }
  检查(四根不变 &&
      当前.场景.场景角色.场景角色登记边.编码 ==
          初始.场景.场景角色.场景角色登记边.编码,
      "movement preserves scene role and all four roots");
  检查(当前.位置.直接结构父->父 == 新场景.投影->E &&
      当前.位置.上行路径.size() > 1,
      "fresh self position reports the new parent and a multi-edge path");
  检查(当前.位置.直接结构父->创建事实代次 !=
          当前.角色.存在身份.节点创建事实代次,
      "current parent-edge generation is independent from self birth generation");

  const auto 上下文请求 = 建立上下文请求();
  const auto 正式上下文 =
      上下文.自我线程正式上下文->读取正式上下文(上下文请求);
  检查(正式上下文成功(正式上下文) &&
      正式上下文.投影->自我所在场景.值 == 新场景.投影->E.值 &&
      正式上下文.投影->安全根 == 上下文请求.锁定本能根.安全根 &&
      正式上下文.投影->服务根 == 上下文请求.锁定本能根.服务根,
      "formal-context provider returns the same fresh parent and locked roots");
}

} // namespace

int main(int argc, char** argv) {
  try {
    std::cout << std::unitbuf;
    if (argc != 3)
      失败("usage: <missing|dependency|first|recover|move> <absolute-root>");
    const std::string_view 模式 = argv[1];
    const auto 根 = std::filesystem::absolute(argv[2]);
    if (模式 == "missing") 验证缺根纯读(根);
    else if (模式 == "dependency") 验证方法门未就绪(根);
    else if (模式 == "first") 验证完整链(根, false);
    else if (模式 == "recover") 验证完整链(根, true);
    else if (模式 == "move") 验证移动后当前读回(根);
    else 失败("unknown mode");
    return 0;
  } catch (const std::exception& 异常) {
    std::cerr << "FAIL " << 异常.what() << '\n';
    return 1;
  } catch (...) {
    std::cerr << "FAIL unexpected exception\n";
    return 2;
  }
}
