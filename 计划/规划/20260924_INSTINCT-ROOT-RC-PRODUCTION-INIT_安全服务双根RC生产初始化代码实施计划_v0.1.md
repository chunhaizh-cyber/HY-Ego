# INSTINCT-ROOT-RC-PRODUCTION-INIT 安全服务双根RC生产初始化代码实施计划

版本：v0.2

日期：2026-09-24

状态：由 `计划/计划索引.md` 维护；本文件发布并登记精确 blob 后方可被选择

代码事实基线：`98b3718acd29ae4059f9c56759c7ef0ac68d7560`

详细设计：`规范/详细设计/20260924_INSTINCT-ROOT-RC-PRODUCTION-INIT_安全服务双根RC生产初始化与稳定定位详细设计_v0.1.md`（内部版本 v0.2）

## 1. 目标与完成条件

实现普通应用构造期的安全/服务双根 RC 生产初始化器：消费已发布双 FT/完整域 FC，创建或恢复通用 EC、双目标判断 K 和双 `D_POS` RC，在共同截止完整读回并形成按本能值角色区分的值式交付；接入普通应用生产构造和跨进程恢复。

完成必须同时满足：

1. 新 provider、DTO、状态和成功谓词按详细设计实现；
2. 通用 EC、双 K、双 RC 只经既有 DATA-L2 owner 发布；
3. 普通应用在世界树实例/SELF-FORM 前取得完整交付；
4. SELF-FORM 复用同一通用 EC；
5. 首次、重复、部分恢复、跨进程恢复和全部具名非成功由专项验证覆盖；
6. 根工程与专项 Debug/Release 构建通过；
7. 施工/验证记录准确限定完成边界；
8. 仅本计划允许文件被提交并普通推送。

## 2. 正式依据

- 6130 v0.17：双根 FT、量化三身份、`M-current`、合法结果域；
- 4180、4181、4182 v0.3：I64 比较、K 唯一路由及固定身份/当前路由读取预算；K 建立与原请求重放保持现行无预算写入口；
- 7170、7180、4241、4242：RC 定义、owner、幂等、读回与生命周期；
- 8120 v0.17：普通应用初始化与 SELF-FORM 查询/复用通用 EC；
- 8121 v0.6：阶段21禁止创建二次概念；
- 对应详细设计 v0.2。

## 3. 唯一所有权和施工范围

计划所有者：执行本计划的唯一执行智能体。

起点：执行前重新 S0，以计划索引登记的精确版本/blob 和当时 `main` 为准。

允许修改：

- `海中鱼巣/业务/初始化.本能双根二次关系概念.h`
- `海中鱼巣/业务/初始化.本能双根二次关系概念.cpp`
- `海中鱼巣/装配.普通应用.h`
- `海中鱼巣/装配.普通应用.cpp`
- `海中鱼巣.vcxproj`
- `海中鱼巣.vcxproj.filters`
- `验证工具/本能双根二次关系概念初始化专项验证.cpp`
- `验证工具/本能双根二次关系概念初始化专项验证.vcxproj`
- `验证工具/世界树根启动专项验证.vcxproj`
- `验证工具/本能根运行生产消费专项验证.vcxproj`
- `实施记录/20260924_INSTINCT-ROOT-RC-PRODUCTION-INIT_安全服务双根RC生产初始化实施记录_v0.1.md`
- `验证记录/20260924_INSTINCT-ROOT-RC-PRODUCTION-INIT_安全服务双根RC生产初始化验证记录_v0.1.md`

明确禁止：

- 修改任何规则、正式规范、详细设计、目标、流程图或计划索引；
- 修改阶段21根材料、需求结构、锚点 DTO、线程 DTO、自我线程、任务管理或启动失败阶段编号；
- 新建 RC/K/概念 owner、恢复账、字符串索引、固定节点编码或兼容入口；
- 实现 RC 求值、目标常量来源适配、G4-A、sink 或治理门；
- 触碰当前三份异主流程图 WIP。

## 4. 目标接口和物理位置

严格实现详细设计第4至第7节的：

- `本能双根二次关系概念初始化状态`
- `本能双根二次关系概念初始化请求`
- `本能根二次关系概念交付`
- `本能双根二次关系概念初始化结果`
- `本能双根二次关系概念初始化提供者`
- `读取普通应用本能双根二次关系概念初始化()`

固定定义、固定 key、恢复顺序、共同截止读回和状态映射不得由执行者临场修改。

## 5. 实施步骤

1. S0：核对计划 blob、HEAD、index、异主 WIP、现有 DTO/函数和知识条目；冻结本计划切片。
2. 新建初始化头源，实现值式完整性、状态成功谓词、构造核验和请求入口核验。
3. 实现通用 EC 的查询→首次材料恢复→创建→正式读回，不复制 pure owner 逻辑。
4. 实现每根 K 的当前路由读取→定义核验→固定键建立→发布未知按完整原请求重放收敛→身份和当前路由双读回；K 读取预算及扫描上限只进入两个读取入口，建立与完整原请求重放调用现行无预算写入口，禁止补预算字段，并核验每次写调用返回结果的读取用量为零。
5. 实现每根 RC 的完整定义查找→固定键建立→原请求收敛→身份完整读回。
6. 冻结最终 G，以纯概念预算重读共同 EC 与双 FC，以 K 读取预算重读双 K 身份并以同一预算和扫描上限重读当前路由，以 RC 预算重读双 RC，同时重读两根 FT，形成角色化交付。
7. 按详细设计冻结的数值15、原因字段和分入口预算口径接入普通应用上下文：pure 公开调用使用纯概念预算，RC 公开调用使用 RC 预算，两个 K 读取入口使用 K 读取预算且当前路由读取另带扫描上限，K 写入口无预算且每次写调用返回结果的读取用量为零；完成重复装配核验和只读访问器，不修改启动阶段顺序。
8. 登记新代码到根工程和两个直接编译普通装配源的既有专项工程；建立独立专项验证工程。
9. 完成最低验证矩阵，按真实结果修复本计划范围内代码问题。
10. 写实施/验证记录；实施记录包含函数级代码知识变更清单。
11. 精确暂存允许文件，复核 staged diff，普通提交并推送。

## 6. 非成功与停工条件

范围内的编译、代码和专项验证错误由执行者修复。出现以下任一情况只停止受影响切片并退回计划支撑：

- 当前规范或接口不再支持详细设计冻结的 K 量化、D_POS、A/B方向或构造期位置；
- 必须修改根材料、锚点、线程 DTO、启动阶段编号或现行 owner 才能继续；
- K/RC 发布未知无法按现有公开入口安全收敛；
- 需要新增公开机器语义、事务、owner 或持久恢复账；
- 允许文件与异主 WIP 出现真实重叠。

不得以空函数、固定成功、测试伪数据、裸 I64 或进程缓存绕过。

## 7. 验证命令与成功条件

执行者应使用本机已配置的真实 MSBuild。每个工程、配置的 `OutDir` 与 `IntDir` 必须分别位于 `D:\TEMP\HY-Ego\INSTINCT-ROOT-RC-PRODUCTION-INIT\<工程>\<配置>\out\` 和 `...\obj\`，不得共享。至少执行：

```powershell
msbuild 验证工具/本能双根二次关系概念初始化专项验证.vcxproj /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /p:TargetName=InstinctRootRcInitTests /p:OutDir=D:\TEMP\HY-Ego\INSTINCT-ROOT-RC-PRODUCTION-INIT\rc-init\Debug\out\ /p:IntDir=D:\TEMP\HY-Ego\INSTINCT-ROOT-RC-PRODUCTION-INIT\rc-init\Debug\obj\
msbuild 验证工具/本能双根二次关系概念初始化专项验证.vcxproj /t:Rebuild /p:Configuration=Release /p:Platform=x64 /p:TargetName=InstinctRootRcInitTests /p:OutDir=D:\TEMP\HY-Ego\INSTINCT-ROOT-RC-PRODUCTION-INIT\rc-init\Release\out\ /p:IntDir=D:\TEMP\HY-Ego\INSTINCT-ROOT-RC-PRODUCTION-INIT\rc-init\Release\obj\
& 'D:\TEMP\HY-Ego\INSTINCT-ROOT-RC-PRODUCTION-INIT\rc-init\Debug\out\InstinctRootRcInitTests.exe'
& 'D:\TEMP\HY-Ego\INSTINCT-ROOT-RC-PRODUCTION-INIT\rc-init\Release\out\InstinctRootRcInitTests.exe'
msbuild 验证工具/世界树根启动专项验证.vcxproj /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /p:TargetName=WorldTreeRootStartupTests /p:OutDir=D:\TEMP\HY-Ego\INSTINCT-ROOT-RC-PRODUCTION-INIT\world-root\Debug\out\ /p:IntDir=D:\TEMP\HY-Ego\INSTINCT-ROOT-RC-PRODUCTION-INIT\world-root\Debug\obj\
msbuild 验证工具/世界树根启动专项验证.vcxproj /t:Rebuild /p:Configuration=Release /p:Platform=x64 /p:TargetName=WorldTreeRootStartupTests /p:OutDir=D:\TEMP\HY-Ego\INSTINCT-ROOT-RC-PRODUCTION-INIT\world-root\Release\out\ /p:IntDir=D:\TEMP\HY-Ego\INSTINCT-ROOT-RC-PRODUCTION-INIT\world-root\Release\obj\
& 'D:\TEMP\HY-Ego\INSTINCT-ROOT-RC-PRODUCTION-INIT\world-root\Debug\out\WorldTreeRootStartupTests.exe'
& 'D:\TEMP\HY-Ego\INSTINCT-ROOT-RC-PRODUCTION-INIT\world-root\Release\out\WorldTreeRootStartupTests.exe'
msbuild 验证工具/本能根运行生产消费专项验证.vcxproj /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /p:TargetName=InstinctRootProductionConsumerTests /p:OutDir=D:\TEMP\HY-Ego\INSTINCT-ROOT-RC-PRODUCTION-INIT\instinct-consumer\Debug\out\ /p:IntDir=D:\TEMP\HY-Ego\INSTINCT-ROOT-RC-PRODUCTION-INIT\instinct-consumer\Debug\obj\
msbuild 验证工具/本能根运行生产消费专项验证.vcxproj /t:Rebuild /p:Configuration=Release /p:Platform=x64 /p:TargetName=InstinctRootProductionConsumerTests /p:OutDir=D:\TEMP\HY-Ego\INSTINCT-ROOT-RC-PRODUCTION-INIT\instinct-consumer\Release\out\ /p:IntDir=D:\TEMP\HY-Ego\INSTINCT-ROOT-RC-PRODUCTION-INIT\instinct-consumer\Release\obj\
& 'D:\TEMP\HY-Ego\INSTINCT-ROOT-RC-PRODUCTION-INIT\instinct-consumer\Debug\out\InstinctRootProductionConsumerTests.exe'
& 'D:\TEMP\HY-Ego\INSTINCT-ROOT-RC-PRODUCTION-INIT\instinct-consumer\Release\out\InstinctRootProductionConsumerTests.exe'
msbuild 海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /p:OutDir=D:\TEMP\HY-Ego\INSTINCT-ROOT-RC-PRODUCTION-INIT\root\Debug\out\ /p:IntDir=D:\TEMP\HY-Ego\INSTINCT-ROOT-RC-PRODUCTION-INIT\root\Debug\obj\
msbuild 海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Release /p:Platform=x64 /p:OutDir=D:\TEMP\HY-Ego\INSTINCT-ROOT-RC-PRODUCTION-INIT\root\Release\out\ /p:IntDir=D:\TEMP\HY-Ego\INSTINCT-ROOT-RC-PRODUCTION-INIT\root\Release\obj\
git diff --check
git diff --cached --check
```

专项必须逐项报告详细设计第9节的十二类验证，其中必须证明 pure 与 RC 各公开调用使用对应预算、K 读取预算及扫描上限只进入两个 K 读取入口、K 建立与完整原请求重放没有读取预算字段且每次写调用返回结果的读取用量为零；未实际覆盖发布未知或跨进程时不得写为 PASS，也不得以构建替代。
两个受影响既有专项也必须完成 Debug/Release 构建与运行；任一项未实际执行都不得声明本计划完成。

## 8. 发布和完成声明

只提交本计划允许文件，三份流程图 WIP 必须继续保持未暂存。发布后确认 `HEAD==origin/main`、ahead/behind 为 `0/0`、index 为空并列出剩余 dirty。

完成只能声明双根 RC 初始化与普通应用交付已经闭合；RC 求值、目标常量来源、G4-A、sink、治理门、消息循环和自我治理总目标仍未完成。

## 9. 修订记录

| 日期 | 版本 | 修订内容 |
| --- | --- | --- |
| 2026-09-24 | v0.2 | 按 4182 v0.3 和现行 K owner 合同关闭 `DESIGN-DRIFT-K-WRITE-BUDGET`：K 读取预算只用于固定身份与当前路由读取，K 建立和完整原请求重放保持无预算写入口且每次写调用返回结果的读取用量为零；同步修订实施步骤和验证要求，白名单与其它机器语义不变。 |
| 2026-09-24 | v0.1 | 首次形成安全/服务双根 RC 生产初始化代码实施计划。 |
