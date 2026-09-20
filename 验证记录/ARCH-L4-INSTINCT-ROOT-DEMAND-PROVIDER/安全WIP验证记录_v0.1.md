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
