# ENTRY-ONLY 薄入口模式路由与初始化宿主分层验证记录

## 记录身份

- 计划：`#380 / DQ-272 / ENTRY-ONLY`
- 类型：`DESIGN-DRIFT-WIP` 安全断点验证
- 原 plan blob：`bdd98b3af026b1608bb3e923405c44c9eb977272`
- 计划段起点：`6041bd1bc9766e5404359dba317366eea63bc8f6`
- 本记录不证明计划完成。

## 已通过

1. `git diff --check`
   - 结果：通过；仅出现 Git 的 LF/CRLF 提示，无空白错误。
2. `python .\tools\check_specs.py --strict`
   - 结果：通过，正式规范目录 `99/99`。
3. 白名单比对
   - 结果：记录写入前的 14 个代码/工程 dirty 文件与计划第 4 节精确一致，无范围外文件。
4. 入口静态拒绝扫描
   - `入口.cpp` 的领域、UI、SQL、自检关键字扫描：零命中。
   - 入口相关五个文件的 `std::cout/std::cerr/std::wcerr/printf` 扫描：零命中。
5. `msbuild .\海中鱼巣.slnx /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /m:1 /v:minimal`
   - 结果：通过，生成 `x64\Debug\海中鱼巣.exe`。
6. 关键退出码
   - `--database-self-test-exit`：`0`。
   - `--runtime-context`：`0`。
   - `--d455-real-sample-exit`：`2`，默认构建能力未启用。
   - `--unknown`：`2`。
   - 重复 `--self-test-exit`：`2`。
   - `--self-test-exit --database-self-test-exit`：`2`。
7. 诊断宏构建与 F27/F29 定位
   - `EnableEntryInitializationSelfTestDebugLog=true` 的 Debug x64 构建通过。
   - F27 九项均通过。
   - F29 第二批次通过；第一批次的 14 个编号和三项初始零计数通过，地址集合大小为 1。

## 未通过与具名原因

1. `.\x64\Debug\海中鱼巣.exe --self-test-exit`
   - 结果：`exit 1`。
   - 原因：F29/V21 要求顺序析构上下文的裸地址跨生命周期两两互异；实测合法地址复用使第一批次失败。
   - 归属：已确认设计缺口，不是代码实现错误。
2. 两次并行 fresh rebuild
   - 命令分别为直接 `.vcxproj /m` 和 `.slnx /m`。
   - 结果：MSVC 模块依赖扫描阶段失败，出现不完整 `.module.json`、`CL.exe` 退出和级联“找不到模块”。
   - 单节点 `/m:1` fresh rebuild 随后通过，证明当前源码可以完整编译；并行扫描故障不作为计划完成证据。

## 未执行或未闭合

- 无参数 UI 受控启动和关闭。
- `--headless` 的 SIGINT/SIGTERM/SIGBREAK 受控收口。
- 六个单 flag、六个重复 flag、15 个两两组合正反顺序的完整参数矩阵。
- 关系仓库性能专项完成验证。
- 五轮日志宏 fresh-build 矩阵。
- 完整覆盖对比、普通 SQL 不可用的 UI/headless 产品子进程验证和最终完成验收。

## 安全断点结论

当前代码可由单节点 fresh rebuild 完整生成，静态范围与规范检查通过，关键路由退出码可复现，设计漂移可由 `--self-test-exit` 稳定暴露。该断点可提交为 `DESIGN-DRIFT-WIP`，但不得宣称 #380 或 ENTRY-ONLY 完成。

## v0.2 续段验证

### 身份

- plan blob：`ec6075e756b2a415806cc18a96bdc396099eeb89`。
- 计划段起点：`f6a88fab026b62c3b86ad25f9612f4bff5f96c21`。
- 计划索引 blob：`e710b9876d89743cd091468da2bf8a5e1e343534`。
- 结果类型：`DESIGN-DRIFT-WIP`，不证明计划完成。

### 已通过

1. `msbuild .\海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /m:1 /v:minimal`
   - 结果：通过，生成 `x64\Debug\海中鱼巣.exe`。
2. v0.2 F29 修订
   - `--self-test-exit`：`0`。
   - 14 个固定编号、回调期间地址非零、三项初始计数全零及强制 S08 失败继续合同均由结构化批次通过。
   - 代码不再建立历史裸地址集合或比较跨生命周期地址。
3. 基础路由
   - `--database-self-test-exit`：`0`。
   - `--runtime-context`：`0`。
   - `--d455-real-sample-exit`：`2`，默认构建能力未启用。
   - 未知参数：`2`。
4. `git diff --check`
   - 结果：通过；只有 LF/CRLF 提示，无空白错误。

### 未通过或未完成

1. F27/V19 局部替身生命周期
   - 当前模块级 `信号替身调用序列` 和 `信号替身调用数量` 能观察 `SIGINT`、`SIGTERM`、回滚 `SIGINT` 三次调用，但违反详细设计“替身和计数不得逃逸函数”。
   - 冻结的无上下文函数指针不能访问 F27 局部计数；执行侧无法在不修改合同或引入全局/静态/thread_local 状态的前提下机械满足两项要求。
   - 归属：设计合同缺口，已请求修订，不是以当前实现可宣称完成的代码项。
2. 发现缺口后停止，尚未执行：
   - 六个单 flag、六个重复 flag、15 个两两组合正反顺序及全部未知参数矩阵；
   - 无参数 UI 和无效 SQL UI 子进程受控关闭；
   - headless 的 SIGINT/SIGTERM/SIGBREAK 收口；
   - 性能专项完成验证；
   - 五轮独立日志宏 fresh-build 矩阵；
   - 覆盖对比、完整静态扫描和最终完成验收。

### 续段安全断点结论

v0.2 的 V21/F29 修订和精确日志路径已形成可编译、可运行断点；F27/V19 新设计缺口有明确代码位置和语言约束证据。该断点可验证但不满足 #380 完成合同。

## v0.3 续段验证

### 身份

- plan blob：`024c51c780705425c70009ba03a1c2404361b7e3`。
- 计划段起点：`e63a8def085b0676b56af12bba5e88efc0691ebe`。
- 计划索引 blob：`d5bcf2f500a342d4007d4c03b4a7e74747fbe519`。
- 结果类型：`ENVIRONMENT-BLOCKED` WIP，不证明计划完成。

### 构建与静态验证

1. `msbuild .\海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /m`
   - 四日志属性显式为 false 的默认轮通过，0 error、0 warning。
2. 四个独立日志属性各自单开的 fresh build
   - 均以可靠单节点 `/m:1` 完整构建通过。
   - 入口宏：`--self-test-exit=0`，只产生入口初始化自检日志。
   - 数据库宏：`--database-self-test-exit=0`，`数据库专项自检.log` 从不存在变为 120 字节，其它切片无新增。
   - 性能宏：完整性能专项 exit 0，`关系仓库性能专项自检.log` 从不存在变为 135 字节，其它切片无新增。
   - D455 日志宏：D455 能力未启用，显式模式 exit 2，不产生假专项日志。
   - 恢复四宏全关后 self/database/runtime 分别 exit 0、D455 exit 2，四份调试日志长度均不变。
3. `python .\tools\check_specs.py --strict`
   - 结果：99 份目录项全部通过。
4. 静态扫描
   - `入口.cpp` 领域/UI/SQL/自检关键字：零命中。
   - 五个入口相关文件 stdout/stderr/printf：零命中。
   - 非自检文件的自检 import：只命中 `启动.应用程序.ixx` 对入口初始化自检和中央运行器的两项设计内导入；无其它非法命中。
   - F27 的信号序列和计数只在函数局部，零模块/static/global/thread_local 测试证据。

### 参数、专项和宿主

1. 参数矩阵
   - 6 个重复 flag：全部 exit 2。
   - 15 个两两组合的正反顺序，共 30 项：全部 exit 2。
   - 2 个未知参数：全部 exit 2。
2. 单模式
   - `--self-test-exit=0`。
   - `--database-self-test-exit=0`。
   - `--runtime-context=0`。
   - `--d455-real-sample-exit=2`，能力未启用。
   - `--warehouse-performance-self-test-exit=0`；三档规模与 12 项验收全部通过。
3. UI
   - 无参数：取得非零 `MainWindowHandle`，`CloseMainWindow()` 返回 true，exit 0。
   - `HY_EGO_SQL_SERVER=tcp:127.0.0.1,1`：取得非零窗口句柄，关闭成功，exit 0；`finally` 清理环境变量。
4. headless
   - Windows 新进程组 `CTRL_BREAK_EVENT` 映射 SIGBREAK：exit 0。
   - 独立新控制台 `CTRL_C_EVENT` 映射 SIGINT：exit 0。
   - SIGTERM：Windows 无外部控制台事件映射；对精确子进程新线程调用 UCRT `raise(SIGTERM)` 得到 exit 3，未形成优雅收口证据。强制终止未被当作通过。

### 结论

代码合同、自检、构建、参数、UI、SIGINT/SIGBREAK、专项和静态验证均已取得通过证据；唯独当前 Windows 环境不能从外部把 SIGTERM 投递到已安装处理器所在的正常执行上下文。依据计划第 8 节，结果为 `ENVIRONMENT-BLOCKED` WIP，是否请求设计包修订：否。

## v0.3 第二次续段验证

### 身份

- plan blob：`024c51c780705425c70009ba03a1c2404361b7e3`。
- 计划索引 blob：`d5bcf2f500a342d4007d4c03b4a7e74747fbe519`。
- 计划段起点：`177f2d8014cbe0f10b16a47b1d73e4f4d339510e`。
- 结果类型：`ENVIRONMENT-BLOCKED` WIP，不证明计划完成。

### 已通过

1. S0
   - S0 时 `main == origin/main`、divergence `0/0`、index clean。
   - #380 是唯一可执行候选，plan / index blob 未变。
   - `a48a70b2..HEAD` 的 14 个目标代码 / 工程文件与 C01—C42 提供接口零变化。
2. 构建与静态
   - 最终 `msbuild .\海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /m`：通过。
   - 单节点 fresh Rebuild：通过。
   - `python .\tools\check_specs.py --strict`：正式规范目录 `100/100`。
   - `git diff --check`：通过。
   - 入口领域关键字与 stdout/stderr/printf：零命中。
   - 自检 import 只有设计内 `启动.应用程序.ixx` 两项。
3. 参数与快速路由
   - 6 个重复、30 个两两组合正反顺序、2 个未知参数，以及 runtime/self/database/D455 四个单模式，共 42 项：全部符合预期。
   - `--runtime-context=0`、`--self-test-exit=0`、`--database-self-test-exit=0`、D455 未启用 `exit 2`。
4. UI 与 SIGTERM
   - 普通 UI：非零窗口句柄、关闭成功、`exit 0`。
   - Debug CRT 精确主线程 `ucrtbased!raise(SIGTERM)`：headless 正常收口，`exit 0`。
5. 日志宏矩阵
   - 四宏全关最终 fresh build 通过。
   - 入口、数据库、关系仓库性能、D455 四个属性分别单开 fresh build 通过。
   - 入口模式 `exit 0` 并只形成入口日志；数据库模式 `exit 0` 并只形成数据库日志；性能宏单开运行 `exit 0` 并形成性能日志；D455 未启用 `exit 2` 且不形成假日志。

### 未闭合

1. V14 固定无效 SQL UI
   - 两轮均设置 `HY_EGO_SQL_SERVER=tcp:127.0.0.1,1`。
   - 两轮均取得非零窗口句柄且 `CloseMainWindow()` 返回 true。
   - 分别等待 90 秒和 180 秒仍未退出，随后终止精确子进程。
   - 未取得计划要求的受控 `exit 0`。
2. 关系仓库性能专项稳定性
   - 四宏全关构建：约 240 秒后 `exit 1`。
   - 性能宏单开 fresh build：约 215 秒后 `exit 0`。
   - 两轮结构化退出码不一致，不能以日志或单次通过覆盖失败。
3. 首轮 `/m` 和首次单节点 Rebuild 曾在模块扫描阶段产生级联缺模块错误；随后单节点和最终 `/m` fresh Rebuild 均通过。该瞬时构建器故障不再是当前阻断，但已保留为环境证据。

### 结论

SIGTERM 的旧验证夹具错误已纠正，产品在 Debug CRT 主线程上下文收到 SIGTERM 后能正常收口。当前剩余阻断是固定无效 SQL UI 无法受控退出，以及性能专项结果不稳定；两项来源均不允许由 #380 修改。结果为 `ENVIRONMENT-BLOCKED`，是否请求设计包修订：否。
