# 双根目标筹办与子需求入树施工与编译记录

日期：2026-11-03。身份：`DUAL-ROOT-DEMAND-PREPARATION-GROWTH` v0.1。代码所有者：root/language_source_code；Git/main/目标及后续运行所有者：root。

## 1. 输入、S0与边界

- 实际工作树：`D:/worktree/demand-tree-growth-20261103/海中鱼巣`；分支：`codex/dual-root-demand-preparation-growth-20261103`。
- 施工HEAD：`d1db2a3de12828a5db83676fae3870dfb800caad`；S0时origin/main相同，index及工作树clean。
- 计划：`计划/20261103_DUAL-ROOT-DEMAND-PREPARATION-GROWTH_双根目标筹办与子需求入树实施计划_v0.1.md`，blob `80a7d07cdeead81fabf77ea5aed5394e1f4b3aec`；详设blob `fc9dbaf2de2dc04dacf00ea3c0db00092654c88c`；索引blob `b349b49fac60fe0fb64140bbfecf15a645d880ab`登记可执行。
- 相对生产冻结基线`31227f7af4fad2b1f736f07f8754deed4fa6bcf9`只有本计划、详设及索引前进；root后续main `0c5ea6d9a1b75d64cb1dd0fecb852759684d20b9`仅四份目标文档前进，不改变本次生产输入。
- 已读取执行技能、完整计划与详设、索引、现行相关头源及生产调用链。AGENTS、串行/工作树/代码知识规则及知识库三入口与前次已读版本无差异；用途卡/完整现行调用图没有可代替当前源码的证据，按当前代码和本叶冻结复用裁决实施。
- 唯一允许范围为下列六生产文件及本记录；不更改公开DTO、持久字段、工程入口、根目标/结算或现实执行硬门。旧语言工作树六份WIP继续原地停写，未复制、暂存或带入本叶。
- 构建前没有MSBuild/cl/link/应用进程；本轮输出目录不存在，独占资源由root交给本代码切片。未取得本叶程序运行资源，不启动程序/浏览器。

## 2. 实际变更与静态复核

1. self分别按双根fresh `需增加`结果安排原自动承接；外部需求承接只允许初始化的两个正式根。
2. task删除服务根关联捷径与普通调度/候选/条件/重新筹办/执行的身份型禁令。两种调度读取都准备、读取并核同完整场景批。保留资格判定本体，所有残留调用仅用于前U父域/结果核验。
3. 前U父关联在task同一串行区直接建立/取得并读回唯一T；只有本次全新T从初始已建立收束为等待，既有T任何状态与普通材料零写。
4. manager共同投池先处理同号重复/异义，再处理同T活动、普通/特殊提交中及贡献占用；前U另核执行/反馈/学习/等待复核和服务结算保护。三口均以活动数＋普通提交中数＋特殊提交中数预留，投池前完整复制请求/返回材料，投池后仅赋future并nothrow移动到活动槽，最后释放提交中。
5. 前U外层全量核同来源之后才查同T占用；返回请求在投池前复制，避免已接受后再分配。ACK保持先worker确认后移除manager活动项，成功释放后触发普通调度。
6. 两个特殊投池口保持原来源、领取、ACK及结算业务；新增预分配资源失败保留原请求记录，提交中仍false；已接受后先安装future和活动项，再清提交中。
7. 控制GET不再按采集中/8192字节隐藏已有不可变缓存；正文、状态、序号、年龄、采集中、陈旧及内部完整性标志原样保留，不在HTTP中查询业务。
8. 本轮直接修改task函数中原先将任意异常降为资源状态的最终catch，现区分bad_alloc/length_error资源结果与工程终止；新增最终诊断保留`std::exception::what()`及未知异常兜底。未扩大到白名单之外的provider/全仓异常整治。

静态检查已执行：`git diff --check`、`git diff --cached --check`均无诊断。生产改动恰为六文件。`是前置语言材料父任务`残留为自身、前U父关联、task读前/读后、manager接纳/完成/读取；worker已无此身份排除。三处真实`活动工作.push_back`均为`std::move(待安装)`；三个nothrow编译期断言保留。

按代码核对交错：普通fresh先建立时，前U取得既有T零状态写；前U先新建时，同owner锁内初始化等待，再由后到fresh普通承接激活；前U/普通提交通过同一manager互斥及提交中保护；ready未ACK及worker ACK后manager删除前仍有manager活动项；三投池的活动数与在途提交数在安装前后总和不增加，预留覆盖其它两口。以上只是静态复核，不是并发测试证明。

## 3. 第一轮冻结生产输入

以下均为仓库相对路径，Git blob按`git hash-object`读取；构建消费此HEAD与这六份WIP，不消费其它工作树。

| 文件 | blob |
| --- | --- |
| 海中鱼巣/领域/新_任务类.h | `0959487a9773a40ebef7030a356b8dc0414c470a` |
| 海中鱼巣/领域/新_任务类.cpp | `164098fa7939fd30194a4da8751456eab3fc1dfc` |
| 海中鱼巣/线程/新_任务管理线程类.cpp | `21a95f9fccf34f7c867c3565058f76adf966ce7a` |
| 海中鱼巣/线程/新_自我线程类.cpp | `8d5c86e251f4e35d250c2287c8b501e24c77ac55` |
| 海中鱼巣/线程/新_任务工作线程类.cpp | `2f30d2a83b983de55b320b93a1b3a5c9e7cb3124` |
| 海中鱼巣/界面/宿主.网页对话.cpp | `c679945611006cef9846a5f11d98478a98fdda34` |

## 4. 实际项目编译

实际项目为本树根`海中鱼巣.vcxproj`；配置Debug/x64；工具`C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/MSBuild.exe`。没有复制源码、新建临时工程或专项测试程序。

```powershell
& 'C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/MSBuild.exe' './海中鱼巣.vcxproj' /t:Build /p:Configuration=Debug /p:Platform=x64 /m:2 /nologo /v:minimal /fl '/p:OutDir=D:/TEMP/海中鱼巣/DUAL-ROOT-DEMAND-PREPARATION-GROWTH/build-20261103-code-01/bin/' '/p:IntDir=D:/TEMP/海中鱼巣/DUAL-ROOT-DEMAND-PREPARATION-GROWTH/build-20261103-code-01/obj/' '/flp:logfile=D:/TEMP/海中鱼巣/DUAL-ROOT-DEMAND-PREPARATION-GROWTH/build-20261103-code-01/build.log;verbosity=normal;encoding=UTF-8'
```

开始：`2026-11-03T22:58:59.1338012+08:00`；结束：`2026-11-03T23:04:28.4534872+08:00`。实际进程exit `0`；MSBuild记录成功生成，`0`警告、`0`错误，用时`00:05:28.85`。首轮编译链接通过，无编译修复或输入增量；完成后复核六文件blob与第3节一致。

产物：`D:/TEMP/海中鱼巣/DUAL-ROOT-DEMAND-PREPARATION-GROWTH/build-20261103-code-01/bin/海中鱼巣.exe`；SHA256：`23359E674687E6895F63032C82D1A867E8827A25E70D3202B1978BFD86055FE3`。完整日志：同轮目录`build.log`。结束后无MSBuild/cl/link/应用进程；构建资源可交root后续运行使用。结论仅为“已编译，未运行测试”。

## 5. 代码知识变更清单

无新增/退出函数、公开签名或DTO。以下均为修改现函数，函数身份与可见性沿现头源；共享知识卡由root交维护owner，不在本叶改知识索引。结果提交待root发布。

| 模块/文件 | 完整函数身份与可见性 | 用途、合同及调用边变化 |
| --- | --- | --- |
| self cpp | `void 新_自我线程类::实现::安排自动承接(const 新自我本能需求复核结果&) noexcept`，私有实现 | 原复核→原两槽自动承接增加服务根；只消费fresh需增加，零领域写入；双根各自去重/原消息号不变。 |
| self cpp | `新自我消息接收结果 新_自我线程类::提交消息(const 新自我治理消息&) noexcept`，public | 外部需求承接仅允许正式双根；原接纳future/停止/邮箱及其他消息形状不变。 |
| task cpp | `新任务治理承接结果 新_任务类::承接需求任务(稳定编码,bool) noexcept`，public | manager→原fresh根目标承接不再把服务根降为扫描关联；原D/T唯一、状态及终态规则不变；资源与实现异常分开，诊断保留原因。 |
| task cpp | `新任务治理承接结果 新_任务类::建立前置语言材料父任务(稳定编码,稳定编码) noexcept`，public | self前U→建立/取得T→读回；删除扫描关联调用边。task同锁仅新T初始化等待，既有T零写；保留真实前缀和原因诊断。 |
| task cpp | `新任务调度材料项 新_任务类::读取任务调度项已加锁(稳定编码,bool&,const 新任务本次场景核验批&) const`，private | 两调度读取→原目标/候选/条件分类，不因前U资格返回仅材料治理；异常向正式边界送出。 |
| task cpp | `新任务调度材料读取结果 新_任务类::读取任务调度材料() const noexcept`，public | worker W0→全量任务完整场景批读取/核同，服务根不跳过；资源不能生成部分已读取全集，工程异常保留原因后终止。 |
| task cpp | `新任务调度材料读取结果 新_任务类::读取任务调度材料(稳定编码) const noexcept`，public | manager独立单T读取同样走完整场景批，无服务根特例；同上工程分账。 |
| task cpp | `新任务操作状态 新_任务类::重新启用任务(稳定编码) noexcept`，public | 删除服务根身份拒绝；终态、已结算及候选清理限制不变；实现异常不再伪装资源。 |
| task cpp | `新任务操作状态 新_任务类::准备任务查找方法(稳定编码) noexcept`，public | 原self/manager准备不排除服务根；只接受原合法状态，资源/实现异常分账。 |
| task cpp | `新任务候选方法发布结果 新_任务类::发布候选方法材料(新任务候选方法发布请求) noexcept`，public | W1/内部发布移除服务根身份拒绝；完整来源、目标、候选及原子发布/读回不变。 |
| task cpp | `新任务治理方法召回结果 新_任务类::查询任务方法候选并发布(稳定编码) noexcept`，public | worker W1走原真实查询及发布，不返回仅材料治理；完整空候选与资源/工程异常分账。 |
| task cpp | `新任务治理条件筹办结果 新_任务类::筹办任务条件和参数(稳定编码) const noexcept`，public | W2不按服务根身份排除；仍按原候选/所选分支与真实材料核验；新增最终工程原因链。 |
| task cpp | `新任务治理条件筹办结果 新_任务类::核验任务条件和参数(稳定编码) const noexcept`，private | 内部复核只删身份禁令，场景批/采用/条件合同及原最终工程出口不变。 |
| task cpp | `新任务执行结果 新_任务类::开始方法执行(const 新任务执行开始请求&) noexcept`，public | 删除身份禁令；原因果许可、场景批、执行冻结和X建立硬门不变。 |
| task cpp | `新任务治理本能执行结果 新_任务类::执行任务本能方法(const 新任务治理本能执行请求&) noexcept`，public | 删除身份型仅材料治理；真实参数、结果途径、授权、安全、停止、零动作和动作后不重做合同全保留。 |
| task h | `bool 新_任务类::是前置语言材料父任务(稳定编码) const noexcept`，public | 仅补资格不是排他类型的注释，函数实现/ABI未改；保留前U父域验证调用。 |
| manager cpp | `新任务工作接收结果 新_任务管理线程类::实现::提交普通工作(const 新任务工作请求&)`，私有实现 | 原派发→原worker；增同号优先/同T活动及在途门、前U保护、总容量/池前副本及预留、池后无抛安装；无新账，最终故障向原责任边界送出。 |
| manager cpp | `void 新_任务管理线程类::实现::推进等待复核() noexcept`，私有实现 | 删除服务根资格终结；原定位/决议/占用和状态复核不变。 |
| manager cpp | `筹办派发状态 新_任务管理线程类::实现::尝试派发执行工作() noexcept`，私有实现 | 删除服务根提前返回；原self决议定位、完整请求、执行保护不变。 |
| manager cpp | `筹办派发状态 新_任务管理线程类::实现::尝试派发筹办工作() noexcept`，私有实现 | 原重试/就绪扫描不跳过服务根；原W1/W2状态、来源及结果消费不变。 |
| manager cpp | `void 新_任务管理线程类::实现::推进学习事件() noexcept`，私有实现 | 删除服务根身份终结；原真实方法知识/事件定位、复核与等待释放不变，无自动假配对。 |
| manager cpp | `新自我等待复核接收状态 新_任务管理线程类::提交等待复核决议(const 新自我等待复核决议&) noexcept`，public | 不因服务根返回定位变化，原fresh目标/资格/占用/决议核同不变。 |
| manager cpp | `新自我学习复核接收状态 新_任务管理线程类::提交学习复核决议(const 新自我学习复核决议&) noexcept`，public | 同上，原学习事件身份与fresh复核保持。 |
| manager cpp | `新前置语言筹办接收结果 新_任务管理线程类::提交前置语言输入筹办(const 新任务前置语言输入请求&) noexcept`，public | self前U→共同口，先全部同来源核同再busy；请求返回值池前复制；接纳仍只读q，不写业务。 |
| manager cpp | `新任务工作操作状态 新_任务管理线程类::确认前置语言输入筹办已读回(const 新任务工作请求&) noexcept`，public | self ACK→worker ACK→manager移除占用→触发普通调度；活动项覆盖两级ACK中隙。 |
| manager cpp | `新贡献治理工作接收结果 新_任务管理线程类::提交任务贡献治理工作(const void*,const 新任务贡献反馈定位&) noexcept`，public | 仅共享活动vector接纳增加同T/在途、容量和池前预分配，池后无抛安装再清提交中；原贡献来源/领取/结算不变。 |
| manager cpp | `新最终服务工作接收结果 新_任务管理线程类::提交最终服务消费工作(const void*,const 新服务值最终来源定位&) noexcept`，public | 同上，原K/R来源/最终领取及消费不变；预分配资源不足保留原工作重试。 |
| worker cpp | `新任务工作接收结果 新_任务工作线程类::提交工作(const 新任务工作请求&) noexcept`，public | manager入池仅移除服务根身份门；工种载荷互斥、同T/同号、容量、停止、future及前U处理首分支不变。 |
| host cpp | `bool 处理请求(SOCKET,std::string_view,std::string_view,std::string_view,const std::string&,bool&,const 网页对话消息接收函数&,const 网页服务输入回调组&,const 网页对话框事实回调组&,const 网页控制面板回调组&)`，文件内部 | 正常宿主→控制GET返回既有非空不可变缓存；不改HTTP schema、采集、发送或业务owner调用；异常向原宿主最终出口送出。 |

以上所有被修改函数用途卡均需由维护owner按最终已发布代码建立/更新或复核；本记录不代表共享知识已完成更新。事实owner保持需求D、任务T/Vt/候选及方法C/R各自原owner，线程仅交接实际材料，HTTP仅投影。

## 6. 测试、发布与未完成范围

本代码owner未运行程序、浏览器、自检、模拟、性能或任何测试脚本。双根正常生长测试已由用户具名授权，但运行资源与实际发布基线由root掌握；本轮root测试前均为`NOT_RUN`。静态可达不证明实际根T/子D/子T、必要条件提议或学习已发生。

未证明：目标/条件分解实际命中、同义复用/循环/满足停止、并发容量/停止交错、无配对学习实际建对、真实方法执行、正常退出或上位目标完成。HTTP大正文复制、同步发送及单线程公平性风险未处理，不宣称彻底无阻塞。

无本叶Git/index写入、提交或推送；代码与本记录待root审查后发布。旧语言WIP和历史证据未动。构建输出保留给root后续实际运行，归本轮资源交接，不删除其它任务产物。
