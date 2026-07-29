# ENTRY-ONLY 薄入口模式路由与初始化宿主分层施工记录

## 记录身份

- 计划：`#380 / DQ-272 / ENTRY-ONLY`
- 记录类型：`DESIGN-DRIFT-WIP`
- 原计划版本：`v0.1`
- 原 plan blob：`bdd98b3af026b1608bb3e923405c44c9eb977272`
- 计划段起点：`6041bd1bc9766e5404359dba317366eea63bc8f6`
- 分支：`main`
- 完成边界：本记录只固定可构建、可复现设计漂移的代码断点，不声明 #380 完成。

## S0 与 Git

- S0 时 `main == origin/main == 6041bd1bc9766e5404359dba317366eea63bc8f6`，divergence `0/0`，工作区和 index clean。
- 计划索引 blob：`fe63781db3cabfa08fdd620fcf91f60524cf8043`。
- #380 当时为唯一可选计划；#352、#359 未同时施工。
- 当前 WIP 仅含本计划允许的代码、工程、自检和两份专属记录。

## 已变更文件

修改：

- `海中鱼巣.vcxproj`
- `海中鱼巣.vcxproj.filters`
- `海中鱼巣/入口.cpp`
- `海中鱼巣/核心/日志系统.h`

新建：

- `海中鱼巣/启动选项.数据.h`
- `海中鱼巣/程序运行结果.数据.h`
- `海中鱼巣/启动.程序入口.ixx`
- `海中鱼巣/启动.应用程序.ixx`
- `海中鱼巣/装配.普通应用.ixx`
- `海中鱼巣/领域/初始化.系统.ixx`
- `海中鱼巣/界面/投影.控制面板启动.ixx`
- `海中鱼巣/适配/审计.数据库启动.ixx`
- `海中鱼巣/启动.程序运行宿主.ixx`
- `海中鱼巣/自检.入口初始化.ixx`

## DESIGN-DRIFT

F18/F29/V21 同时要求：

- S01-S14 各自构造并独占一个 `unique_ptr<普通应用上下文>`；
- 每项函数返回后完整析构，不共享仓库、全局夹具或上下文；
- 14 个顺序、生命周期不重叠的上下文裸地址必须两两互异。

诊断宏开启时实测 14 个固定编号各观察一次、初始节点/关系/索引计数全部为 0，但地址集合大小为 1。C++ 分配器允许复用已经释放的地址，因此“跨不重叠生命周期裸地址两两互异”不能由现行生命周期合同机械保证。不得用泄漏、全局或 `thread_local` 保留、特殊分配策略或延长上下文生命周期绕过。

配对计划支撑已复核该设计修订请求成立。待设计包删除跨生命周期地址互异断言后，执行侧重新 S0 并继续。

## 断点边界

- 已形成薄入口、模式解析与路由、普通应用装配、系统初始化、控制面板投影、数据库启动审计、停止信号与宿主、自检隔离的 WIP 实现。
- 已形成 F27 九项端口合同验证；本轮诊断证据为九项全部通过。
- F29 第二批次强制 S08 失败并继续执行的合同通过；第一批次仅因 V21 地址互异断言失败。
- 未完成计划第 7 节全验证矩阵，不得声明 ENTRY-ONLY 已实现并验证。
- 未执行或未闭合：完整参数矩阵、UI/headless 受控收口、关系仓库性能专项完成、五轮日志宏 fresh-build 矩阵、覆盖对比和最终完成验收。

## v0.2 续段 DESIGN-DRIFT-WIP

- 新计划版本：`v0.2`。
- 新 plan blob：`ec6075e756b2a415806cc18a96bdc396099eeb89`。
- 新计划段起点：`f6a88fab026b62c3b86ad25f9612f4bff5f96c21`。
- 新计划索引 blob：`e710b9876d89743cd091468da2bf8a5e1e343534`。
- S0 时 `main == origin/main == f6a88fab026b62c3b86ad25f9612f4bff5f96c21`，divergence `0/0`，工作区和 index clean。
- 已按 v0.2 删除 F29 的历史裸地址集合和跨生命周期互异断言，改为精确验证 S01-S14 固定编号、回调期间地址非零及三项初始计数全零。
- 已把数据库、关系仓库性能和 D455 三个专项调试日志文件名修正为 v0.2 冻结的精确路径。

### 新具名设计缺口

详细设计第 6.3 节、F27 和 V19 同时要求：

- F27 的信号安装替身及调用计数不得逃逸函数；
- 第二次安装在 `SIGTERM` 失败后，必须机械观察第三次 `SIGINT` 恢复调用；
- F12 的注入端口保持 `程序信号处理函数 (*)(int, 程序信号处理函数)`。

该函数指针没有调用方上下文参数。非捕获函数指针回调不能访问 F27 自动存储期局部计数，捕获 lambda 不能转换为该函数指针；回调返回后若要读取第三次调用证据，只能引入 `static`、`global`、`thread_local` 状态，或修订冻结签名/端口合同。当前 WIP 中的模块级 `信号替身调用序列` 和 `信号替身调用数量` 因而不符合“替身和计数不得逃逸函数”，不得作为完成实现。

执行侧已停止扩大范围并请求配对计划支撑在原目标内修订；不自行选择修改 F12 签名，亦不以全局或线程局部测试夹具绕过。

### v0.2 安全断点边界

- 本续段实际修改：`海中鱼巣/启动.应用程序.ixx`、`海中鱼巣/核心/日志系统.h` 和两份计划专属记录。
- 已通过单节点 fresh Debug Rebuild，当前源码可完整编译。
- 已通过 `--self-test-exit=0`、`--database-self-test-exit=0`、`--runtime-context=0`；默认未启用的 D455 和未知参数均为 exit 2。
- 未完成：完整参数矩阵、UI/headless 信号收口、五轮日志宏 fresh-build、覆盖与最终静态收口。
- 本断点只保存 v0.2 已闭合修订及新设计缺口，不声明 #380 完成。

## v0.3 续段 ENVIRONMENT-BLOCKED

- 计划版本：`v0.3`。
- plan blob：`024c51c780705425c70009ba03a1c2404361b7e3`。
- 计划索引 blob：`d5bcf2f500a342d4007d4c03b4a7e74747fbe519`。
- 计划段起点：`e63a8def085b0676b56af12bba5e88efc0691ebe`。
- S0 时 `main == origin/main == e63a8def085b0676b56af12bba5e88efc0691ebe`，divergence `0/0`，工作区和 index clean；#380 是唯一可执行候选。

### 机械实施

- `海中鱼巣/启动.程序运行宿主.ixx`
  - `程序信号安装函数` 改为冻结的可捕获 `std::function`。
  - F12 按值取得端口并移动到成功租约，失败恢复和成功析构均使用租约拥有的同一端口。
  - 生产默认端口显式绑定 `std::signal`。
- `海中鱼巣/启动.应用程序.ixx`
  - 删除模块级 `信号替身调用序列` 和 `信号替身调用数量`。
  - F27 在自动存储期建立局部三槽序列、局部计数和引用捕获替身，验证 `SIGINT, SIGTERM, SIGINT`。

### 已闭合验证

- 严格规范检查 `99/99`，`git diff --check` 通过。
- 计划要求的默认并行 fresh Debug Rebuild `/m` 通过，0 error、0 warning。
- 参数拒绝矩阵：6 个重复、15 对组合的正反顺序共 30 项、2 个未知参数全部 exit 2。
- 默认路由：self/database/runtime exit 0；D455 未启用 exit 2；关系仓库性能专项完整执行 exit 0。
- 普通 UI 和无效 SQL UI 均取得非零窗口句柄，`CloseMainWindow()` 成功并 exit 0。
- headless 的 SIGINT 和 Windows SIGBREAK 均受控优雅 exit 0。
- 五轮 fresh-build 配置均已构建；入口、数据库、关系仓库性能三个可运行调试切片只更新对应日志，D455 能力未启用时显式模式 exit 2 且不伪造日志；四宏全关时四份日志均无新增。
- 入口领域关键字和 stdout/stderr/printf 旁路扫描零命中；自检 import 扫描只命中设计明确拥有 F15/F28 的 `启动.应用程序.ixx` 两行。

### 环境阻断

Windows 控制台事件可机械发送 SIGINT 和 SIGBREAK，但没有映射为 C 运行库 SIGTERM 的外部控制台事件。为避免用强制终止冒充信号收口，本轮只对自行创建的精确 headless 子进程尝试调用其已加载 UCRT 的 `raise(SIGTERM)`；该调用发生在注入的新线程，走 C 运行库默认终止并得到 exit 3，未触发主执行线程已安装处理器，不能作为优雅 exit 0 证据。

因此计划第 7.3 节的 headless SIGTERM 外部产品验证在当前 Windows 环境未闭合，按第 8 节以 `ENVIRONMENT-BLOCKED` 收口。是否请求设计包修订：否。不得据此声明 #380 完成。

## v0.3 第二次续段 ENVIRONMENT-BLOCKED

- plan blob：`024c51c780705425c70009ba03a1c2404361b7e3`。
- 计划索引 blob：`d5bcf2f500a342d4007d4c03b4a7e74747fbe519`。
- 计划段起点：`177f2d8014cbe0f10b16a47b1d73e4f4d339510e`。
- S0 时 `main == origin/main == 177f2d8014cbe0f10b16a47b1d73e4f4d339510e`，divergence `0/0`，index clean；#380 仍为唯一可执行候选。
- 从上一 v0.3 WIP 提交 `a48a70b2d678dea64dd8228adb4f26f0badd9506` 到本轮 S0，14 个目标代码 / 工程文件与 C01—C42 提供接口零变化；本轮未修改产品代码。

### 新闭合的 SIGTERM 证据

上一轮验证夹具调用了 Release CRT 的 `ucrtbase!raise`，而 Debug 产品实际通过 `ucrtbased.dll` 注册信号处理器，因此得到默认终止，不能证明产品合同。

本轮以 `CreateProcess` 返回的精确主线程句柄暂停主线程，在该线程上下文调用目标进程 `ucrtbased!raise(SIGTERM)`，随后恢复原指令上下文。headless 由已安装处理器设置停止请求并正常返回 `exit 0`。该夹具只存在于执行命令进程，不新增仓库文件、产品测试后门或模块级证据。

### 当前环境阻断

1. 固定无效 SQL 的普通 UI 两轮均取得非零 `MainWindowHandle`，`CloseMainWindow()` 均返回 true，但分别等待 90 秒和 180 秒后仍未退出，只能终止精确子进程。该结果未满足 V14 的受控关闭和 `exit 0`。
2. 关系仓库性能专项在四宏全关构建中一次运行约 240 秒后 `exit 1`；性能日志宏单开 fresh build 后复跑约 215 秒并 `exit 0`，结果不稳定，不能把单次通过当作稳定完成证据。
3. SQL 适配器、关系仓库性能自检及其阈值均不在 #380 白名单；执行侧不得修改来源或降低验收。

### 本轮收口

- 已通过最终 Debug x64 `/m` fresh Rebuild、单节点 fresh Rebuild、strict `100/100`、`git diff --check`、入口静态门禁、42 个快速参数用例、self/database/runtime/D455 路由、普通 UI，以及入口 / 数据库 / 性能 / D455 四个日志宏单开 fresh build。
- 已保留上一 WIP 中 SIGINT、SIGBREAK、F27/F29、日志隔离和覆盖事实；关联代码从 `a48a70b2` 至本轮零变化。
- 未闭合 V14 无效 SQL UI 收口和稳定性能专项结果，不声明 #380 完成。
- 结果：`ENVIRONMENT-BLOCKED`。
- 是否请求设计包修订：否。

## v0.3 第三次续段 ENVIRONMENT-BLOCKED

- plan blob：`024c51c780705425c70009ba03a1c2404361b7e3`。
- 计划索引 blob：`d5bcf2f500a342d4007d4c03b4a7e74747fbe519`。
- 计划段起点：`42129cd1ddccfafabd69331325124afbd59434b8`。
- S0 时 `main == origin/main == 42129cd1ddccfafabd69331325124afbd59434b8`，divergence `0/0`，index clean；#380 仍为唯一可执行候选。
- 从最近 #380 WIP `2934596b9f673d01a3b72d468942f6912bf4cda4` 到 S0，14 个目标代码 / 工程文件与两份专属记录零变化；本续段未修改产品代码。
- 验证期间 `main/origin/main` 前移到 `2c38ec4017e38b2e541b93c8c9ae038856f849c7`；新提交只修改流程图维护材料，未触碰 #380 允许文件、计划、接口或两份专属记录。发布前按新 HEAD 重新复核本切片。

### V14 后续机械复验

两轮均只在精确子进程环境设置 `HY_EGO_SQL_SERVER=tcp:127.0.0.1,1`，等待取得非零窗口句柄，调用 `CloseMainWindow()`，等待自然退出，并在夹具收口时清理环境变量：

1. 第 1 轮约 16.441 秒结束：窗口约 16.262 秒出现，`CloseMainWindow()` 返回 true，关闭后约 173 毫秒 `exit 0`。
2. 第 2 轮约 16.361 秒结束：窗口约 16.113 秒出现，`CloseMainWindow()` 返回 true，关闭后约 241 毫秒 `exit 0`。

当轮两次均在 180 秒边界内受控 `exit 0`。旧“等待 90 / 180 秒仍不退出”保留为历史高延迟 / 高方差证据，不再作为本续段 V14 阻断。

### 关系仓库性能专项稳定性

四宏全关 fresh Debug x64 Rebuild 通过，0 warning、0 error。对同一构建、同一 `--warehouse-performance-self-test-exit` 命令连续运行两轮：

1. 第 1 轮约 266.443 秒，`exit 1`，stderr 为空。
2. 第 2 轮约 296.350 秒，`exit 0`，stderr 为空。

同一构建和命令得到不一致退出码，稳定性门禁未闭合。性能专项来源、阈值和相关代码均不在 #380 白名单，本续段未修改来源、未降低门限，也不以第二轮通过覆盖第一轮失败。

### 本轮收口

- `git diff --check` 通过；严格规范检查 `101/101`。
- 四宏全关 Debug x64 fresh Rebuild 通过，0 warning、0 error。
- 入口领域关键字和 stdout/stderr/printf 旁路扫描零命中；自检 import 只有设计允许的 `启动.应用程序.ixx` 两项。
- V14 固定无效 SQL UI 当轮两次受控 `exit 0`，旧 V14 阻断已由当轮机械证据推翻。
- 关系仓库性能专项同一构建连续得到 `exit 1 / exit 0`，属于验证结果不稳定；任一验证失败不得提交“完成”。
- 结果：`ENVIRONMENT-BLOCKED`。
- 是否请求设计包修订：否。

## v0.3 第四次续段 PERFORMANCE-VALIDATION-FAILED

- plan blob：`024c51c780705425c70009ba03a1c2404361b7e3`。
- 计划索引 blob：`1282d6dd96304499b2299fe99d73c909933c62f3`。
- 计划段起点：`473620aabbdeca2ee0b783604c77dd5c586b692d`。
- S0 时 `main == origin/main`、divergence `0/0`、index clean；#377 v0.7
  已有 `SOURCE-CANDIDATE-COMPLETE` 结果提交，#379、#352、#359 均为
  `待激活`，故 #380 是继续门禁中的唯一剩余可执行候选。
- 最近 #380 记录断点 `48a8a0a730f6fc9aef307b95abeee4f38a75826e`
  到本轮 S0，14 个允许代码 / 工程文件与 C01—C42 零变化；本轮未修改
  产品代码。工作区流程图维护智能体的具名未跟踪 Markdown 保持原样，
  未清理、未暂存、未提交。

### 当轮机械复核

1. 四日志宏全关 Debug x64 `/m` Rebuild 通过，生成当前
   `x64/Debug/海中鱼巣.exe`。
2. 对同一构建连续两次运行
   `--warehouse-performance-self-test-exit`：
   - 第 1 轮在约 398—427 秒之间自然结束，`exit 1`，stdout / stderr
     均为空；
   - 第 2 轮在第 311 秒轮询时已自然结束，`exit 1`，stdout / stderr
     均为空。
3. 两轮均未强制终止、未改变超时、未打开性能日志宏、未修改性能来源
   或降低门限。

### 收口

旧的同构建 `exit 1 / exit 0` 波动本轮没有重现；当前机械事实是全关
Debug 构建连续两轮稳定 `exit 1`。这闭合了“是否仍不稳定”的诊断，但
没有闭合计划第 7 节性能专项必须通过的验收。性能专项实现、门限和失败
归因来源均在 #380 白名单外，执行侧不得修改。

结果：`PERFORMANCE-VALIDATION-FAILED-OUTSIDE-SCOPE`。不声明 #380
完成；是否请求 #380 设计包修订：否。请求计划支撑把 #380 置为等待
外部性能专项来源修复 / 归因结果的状态，不以重复执行或单次偶然成功
覆盖连续失败。
