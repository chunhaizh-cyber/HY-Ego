# ARCH-L4-INSTINCT-ROOT-DEMAND-PROVIDER 安全 WIP 验证记录

- `git diff --check`：三处计划生产路径通过；暂存区施工前为空。
- 根工程 Debug x64 与 Release x64 各在`D:\TEMP\海中鱼巣\ARCH-L4-INSTINCT-ROOT-DEMAND-PROVIDER\20260920-s0\{Debug-x64,Release-x64}`独占 Rebuild，均生成`bin\海中鱼巣.exe`且无失败标记；同目录 Build读回退出码均为0。
- 静态检索确认普通应用只登记/构造/取得需求服务，不调用根 provider。

未覆盖：计划第7节规定的需求结构和双根 provider 运行矩阵、重启/未知发布、资源故障、Release运行、启动与集成验收。因此不得将本WIP描述为根材料 provider完成。

## 续行 v0.2

- `registry_probe.cpp`（仓外隔离）：首次需求结构登记后以结果事实代次重放，返回`精确重复`且交付相等；错误合同版本返回`入口拒绝`。修复前同一重放返回`幂等冲突`。
- `root_provider_probe.cpp`（仓外隔离）：安全根首次和按当前截止恢复成功；同角色换`F`返回`幂等冲突`；未找到、已退出和属性类型`F`分别返回具名拒绝；服务根以不同`F`独立形成，`T/D/L`不与安全根交叉。该驱动的输出同时记录实例特征登记`版本=1`且`success=0`的计划外阻断事实。
- 根工程 Debug x64 和 Release x64 均在本任务的`D:\TEMP\...\20260920-s0\{Debug-x64,Release-x64}`完成独占Rebuild，均为0警告、0错误。两个生成EXE以默认启动路径返回1；当前`启动.应用程序.cpp`固定在未实现阶段21返回失败，且普通应用此前会被上述实例特征登记成功谓词阻断，因此不作为根provider运行失败结论。
- `git diff --check`通过；静态检索仍确认普通应用未调用`建立或读取本能根材料`。

仍未覆盖：原身份可能发布/重启读回、写后漂移、资源失败、根不完整/污染/重复成员的故障注入，以及普通应用真实装配、阶段21、启动和集成验收。计划外实例特征结果版本缺陷未修复前，本计划不得标记完成。

## 续行 v0.3：新持久仓双根恢复

- 专项源和全部输出位于`D:\TEMP\海中鱼巣\ARCH-L4-INSTINCT-ROOT-DEMAND-PROVIDER\20260920-s0\root-provider-persistence-probe\`，以MSVC 19.51编译`root_provider_persistence_probe.cpp`及既有`数据服务.特征类.cpp`；该专项未进入Git或工程文件。
- 新`data-v4`持久仓的首次进程`seed`退出0，输出`seed pass G=10 T=27 D=28 L=29 serviceT=34`；证明首次结构登记和安全、服务双根形成后，根材料读回截止为新写入H。
- 同一`data-v4`第二进程`recover`退出0，输出`recover pass G=11 T=27 D=28 L=29 serviceT=34`；安全、服务均为`已恢复`，安全根与服务根的`T/D/L`未交叉，且结果与材料读回截止为当前G。`recover-instance`退出0，输出`recover-instance pass G=11 version=2 firstH=7`。
- 已有`data-v3`第二进程`recover`亦退出0，输出同为`recover pass G=11 T=27 D=28 L=29 serviceT=34`。
- 根工程在独占目录`D:\TEMP\海中鱼巣\ARCH-L4-INSTINCT-ROOT-DEMAND-PROVIDER\20260920-s0\{Debug-x64,Release-x64}\`以MSBuild 18.5 `/t:Rebuild /m:1`完成；两个配置均清除`unsuccessfulbuild`标记并生成`bin\海中鱼巣.exe`。
- `git diff --check`通过；本次没有暂存其它所有者的`Debug/`、`.obj`、PDB或其它遗留输出。

本轮证明冻结范围内的首次/持久恢复读回边界和根工程可构建；仍未覆盖计划第7节的原身份可能发布、写后漂移、资源失败、根不完整/污染/重复成员故障注入，以及普通应用启动、阶段21、线程、连续运行和正式集成验收，不能将本计划或上位集成目标标记完成。

## 续行 v0.4：故障矩阵的合法注入边界

- 以当前`7ca1f17b7`生产头重新编译仓外`root_provider_probe.cpp`，双根首次/精确重复、同角色异F、F未找到/已退出/错型、服务根隔离均通过，输出`root provider probe pass; instance_registration_success=1 instance_version=2`，退出0。
- 为验证“根材料已退出”，夹具尝试在根形成后以同一需求owner退出完整`T/D/L/DF/DT/LD/V`集合。需求服务已独占持有写端口；夹具无法在服务存活期间取得有效的第二端口，失效端口提交返回L1`内部不一致(11)`、`G=0`。这不是根provider对已退出材料的返回，不能作为该分支的通过或失败证据。
- 当前L1公开服务未发现故障注入、资源关闭或发布未知模拟入口。计划v0.6第7节要求原身份可能发布、资源失败、写后漂移、根不完整/污染/重复成员等分支，却没有冻结合法注入方式、测试友元/夹具边界或可使用的测试端口。执行者不能通过私有访问、伪造L1结果或改生产合同来补造这些验证。

因此，已覆盖的正常、F拒绝和持久恢复证据保持有效；故障矩阵的未覆盖项需要计划支撑明确合法验证设计后才能继续，不将本记录解释为根provider错误或计划完成。

## 续行 v0.5：v0.7 退出与闭包矩阵

- 当前生产头与`数据服务.特征类.cpp`在仓外唯一目录`D:\TEMP\海中鱼巣\ARCH-L4-INSTINCT-ROOT-DEMAND-PROVIDER\20260920-v07-root-exit-closure\`编译`root_provider_probe.cpp`，退出0。每个模式均新建运行包：`exit`（完整安全根`T/D/L/DF/DT/LD/V`公共退出后返回`根材料已退出`）、`duplicate`（额外`L->D`返回`根材料未闭合`）、`pollution`（普通关系污染返回`根材料未闭合`）和`stale`（写前旧`G`返回`事实代次漂移`）均退出0、零材料失败；夹具先销毁需求服务、同 owner 重取端口、回放结构登记，再以公共写集写入退出或污染并重建服务。
- 同目录以新持久仓编译并运行`root_provider_persistence_probe.cpp`：`seed`输出`G=10 T=27 D=28 L=29 serviceT=34`，第二进程`recover`输出`G=11 T=27 D=28 L=29 serviceT=34`，`recover-instance`输出`G=11 version=2 firstH=7`；三项退出0。该结果证明本次引用闭包检查未破坏双根原身份跨进程读回与实例恢复，不证明阶段21或启动。
- `missing`直接运行未登记为PASS：完整退出后调用既有公共`执行L1物理清理`会使底层读取返回`历史材料已清理`，按现行L1合同属于独立技术非成功，根入口正确返回`内部不一致(15)`，不是计划中“未找到”。生产与公共L1均未提供可在不伪造状态、不修改快照或不改L1合同的前提下制造根自有`未找到`的入口；该精确运行分支为`NOT_RUN（合法注入缺口）`。源码逐行复核仍确认直接`未找到`映射为`根材料未闭合`。
- 根工程以 MSBuild 18.8、`/t:Rebuild /m:1`在该目录的`Debug-x64`和`Release-x64`隔离`OutDir/IntDir`完成；两个配置均生成`bin\海中鱼巣.exe`且最终没有`obj\海中鱼巣.tlog\unsuccessfulbuild`标记。构建不等于运行、恢复或集成验收。
- `git diff --check`通过；静态检索仍确认装配层不调用`建立或读取本能根材料`，本轮没有暂存异主遗留。

仍为`NOT_RUN（合法注入缺口）`：原身份发布未知和运行期资源失败；现行生产合同禁止为此新增故障注入、友元、测试 DTO、私有状态出口或第二写端口。阶段21、普通应用真实装配、启动、线程、连续运行和正式集成验收仍未覆盖。本次结果提交：`f57e3513ad1fddee674101e81cf6bd2263e165a0`。
