# PERCEPTION-CUTOVER 真实 D455 体素生产装配、恢复与端到端验收代码实施切片 v0.2

编号：PERCEPTION-CUTOVER；路由：#373 / DQ-265；合同：PER-C13 / ABI 2；日期：2026-07-24

设计状态：终审 PASS。当前状态、版本和 plan blob 唯一读取 `计划/计划索引.md`。本版本保留真实依赖门控，未满足时不得选择。

## 1. 依赖与目标

真实依赖：#352、#359、#361—#372 各自固定结果提交全部存在，目标模块 / ABI / 依赖方向与本计划一致，并能在同一集成候选父链上汇合。#360 已退出，只作设计依据，不是实现依赖。

目标：实现 `D455生产运行期ABI版本 = 2`，唯一接通真实设备占用、连续采样、PER-C1—C12 提供者、单队列任务承接、四方法独立回合、世界结构、体素、停止 / 断流 / 恢复和跨进程真实硬件验收。

精确依据：`规范/详细设计/真实D455生产装配恢复与验收详细设计.md` v0.2；`流程图/20260724_PERCEPTION-RUNTIME_真实D455生产装配恢复验收施工流程图_v0.2.md`；`规范/详细设计/函数结构知识图谱/20260724_PERCEPTION-D0_D455观察体素生产闭环函数结构知识图谱_v0.2.md`。

## 2. 精确文件所有权

只读：

- `海中鱼巣/适配/协议.D455采样材料.ixx`
- `海中鱼巣/线程/外设采样材料线程.ixx`（模拟 / 空材料旧线程，禁止借用为真实 D455）
- #352、#361—#372 的全部固定实现文件
- #359 固定结果提交中除下述“共享接线移交集”之外的私有 provider、路由、任务请求和治理实现

允许受控修改 / 新建：

- `海中鱼巣/生产运行期配置.数据.h`
- `海中鱼巣/适配/采集器.D455相机.ixx`
- `海中鱼巣/线程/缓存.D455帧材料.ixx`
- `海中鱼巣/线程/上行桥.D455采样材料.ixx`
- `海中鱼巣/线程/D455生产采样线程.ixx`
- `海中鱼巣/线程/运行宿主线程.ixx`
- `海中鱼巣/线程/自我线程.ixx`
- `海中鱼巣/线程/任务管理线程.ixx`
- `海中鱼巣/线程/任务工作线程.ixx`
- `海中鱼巣/装配.运行期业务.ixx`
- `海中鱼巣/启动.生产运行期.ixx`
- `海中鱼巣/启动.运行期上下文.ixx`
- `海中鱼巣/线程/协议.D455生产运行期.ixx`
- `海中鱼巣/装配.D455生产运行期.ixx`
- `海中鱼巣/装配.D455恢复.ixx`
- `海中鱼巣/适配/自检.D455生产闭环.ixx`
- `海中鱼巣/自检.运行器.ixx`
- `海中鱼巣/入口.cpp`
- `海中鱼巣.vcxproj`
- `海中鱼巣.vcxproj.filters`
- `实施记录/20260724_PERCEPTION-CUTOVER_PER-C13真实D455闭环接线验收_Codex断点清单.md`

禁止：表外文件、任何规范 / 流程图 / 详细设计 / 知识图谱 / 计划 / 索引、修改 #361—#372 私有实现、控制面板和 SQL。

共享接线移交集为：`海中鱼巣/线程/运行宿主线程.ixx`、`海中鱼巣/线程/自我线程.ixx`、`海中鱼巣/线程/任务管理线程.ixx`、`海中鱼巣/线程/任务工作线程.ixx`、`海中鱼巣/装配.运行期业务.ixx`、`海中鱼巣/启动.生产运行期.ixx`、`海中鱼巣/启动.运行期上下文.ixx`、`海中鱼巣/自检.运行器.ixx`、`海中鱼巣/入口.cpp`、`海中鱼巣.vcxproj`、`海中鱼巣.vcxproj.filters`。只有 #359 固定结果提交存在、执行占用释放且 S0 逐文件记录该提交中的起始 blob 后，后继 D455 接线修改权才转给 #373；#359 固定提交保持不可变，其余文件继续只读。

## 3. 接口与生命周期

- 精确 DTO、稳定枚举、`D455运行规格`、四恢复端口和六个公开函数唯一读取生产装配详细设计 v0.2。
- `D455生产运行期装配` 构造注入真实帧来源工厂、C1—C12 固定提供者、运行期上下文宿主、运行宿主线程、自我线程、任务管理线程和任务工作线程。
- 恢复确认顺序：权威结构 -> 报告治理 -> 任务筹办 -> 体素；撤销逆序；四域完整读回后才发布运行期上下文。
- 断流先封闭旧生产代次；重连必须建立新代次。停止先封闭生产 / 消费，再形成恢复候选并释放设备。

## 4. S0—S7

1. S0：核对计划版本 / blob、执行通道 clean、全部固定结果提交、现行文件内容、设备身份、SDK / 工程环境、零占用进程和白名单。任一缺失即 DRIFT。
2. S1：建立 C13 DTO / ABI、固定生产配置、设备占用、运行代次和停止 / 断流状态。
3. S2：装配 C1—C12 固定提供者、真实生产采样线程、任务消费与四方法回流；旧模拟线程保持只读。
4. S3：实现四恢复提供者编排、逆序撤销、索引 / 空队列重建和完整上下文最后发布。
5. S4：接入运行宿主、自我 / 任务线程、入口和统一运行器；工程只登记固定模块一次。
6. S5：Debug x64 当轮重建与完整自检；Release x64 当轮重建与完整自检。
7. S6：真实 D455 连续场景、断流重连、正常停止和跨进程两轮验收；每轮清理精确 PID 和设备占用。
8. S7：白名单、工程登记、import、ABI 和禁止旧线程借用检查通过后，精确暂存白名单，运行 cached diff，提交结果；再按 `$计划段起点..HEAD` 核对 diff 与精确文件集，确认 worktree / index clean 后推送执行分支。

## 5. 验收矩阵

- 连续运行至少 60 秒、完整同步批次至少 300、稳定报告至少 10、四产品至少各 1。
- 同一报告重复承接 0；观察提交、状态 / 动态提交和体素发布均有真实读回。
- 断流后 30 秒内重连；停止 10 秒内完成；跨进程重启 2 轮；残留进程和设备占用 0。
- 四产品若无自然前置，必须建立合法任务、扫描基准和跟踪目标，不得以“未触发”通过。

## 6. 机械验证命令

S0 记录 `$计划段起点 = (git rev-parse HEAD).Trim()`。S5—S7 依次运行；Debug / Release 自检各限时 180 秒，两次真实硬件验收各限时 240 秒。所有程序必须由 `Start-Process -PassThru` 取得精确 PID，超时只终止该 PID，并在继续前确认没有 `海中鱼巣.exe` 残留：

```powershell
function 运行受限程序 {
  param(
    [Parameter(Mandatory=$true)][string]$程序,
    [Parameter(Mandatory=$true)][string[]]$参数,
    [Parameter(Mandatory=$true)][int]$限时秒
  )
  $进程 = Start-Process -FilePath $程序 -ArgumentList $参数 -PassThru -WindowStyle Hidden
  $进程编号 = $进程.Id
  try {
    if (-not $进程.WaitForExit($限时秒 * 1000)) {
      Stop-Process -Id $进程编号 -Force -ErrorAction SilentlyContinue
      Wait-Process -Id $进程编号 -Timeout 10 -ErrorAction SilentlyContinue
      throw "程序超时：PID=$进程编号"
    }
    $进程.Refresh()
    if ($进程.ExitCode -ne 0) { throw "程序失败：PID=$进程编号 ExitCode=$($进程.ExitCode)" }
  }
  finally {
    if (Get-Process -Id $进程编号 -ErrorAction SilentlyContinue) {
      Stop-Process -Id $进程编号 -Force -ErrorAction SilentlyContinue
      Wait-Process -Id $进程编号 -Timeout 10 -ErrorAction SilentlyContinue
    }
  }
  if (Get-Process '海中鱼巣' -ErrorAction SilentlyContinue) {
    throw "存在海中鱼巣.exe残留进程"
  }
}

msbuild .\海中鱼巣.vcxproj /p:Configuration=Debug /p:Platform=x64 /m
if ($LASTEXITCODE -ne 0) { exit 1 }
运行受限程序 -程序 '.\x64\Debug\海中鱼巣.exe' -参数 @('--self-test-exit') -限时秒 180
msbuild .\海中鱼巣.vcxproj /p:Configuration=Release /p:Platform=x64 /m
if ($LASTEXITCODE -ne 0) { exit 1 }
运行受限程序 -程序 '.\x64\Release\海中鱼巣.exe' -参数 @('--self-test-exit') -限时秒 180
运行受限程序 -程序 '.\x64\Release\海中鱼巣.exe' -参数 @('--d455-production-e2e-exit') -限时秒 240
运行受限程序 -程序 '.\x64\Release\海中鱼巣.exe' -参数 @('--d455-production-e2e-exit') -限时秒 240
$期望 = @(
  '海中鱼巣/生产运行期配置.数据.h',
  '海中鱼巣/适配/采集器.D455相机.ixx',
  '海中鱼巣/线程/缓存.D455帧材料.ixx',
  '海中鱼巣/线程/上行桥.D455采样材料.ixx',
  '海中鱼巣/线程/D455生产采样线程.ixx',
  '海中鱼巣/线程/运行宿主线程.ixx',
  '海中鱼巣/线程/自我线程.ixx',
  '海中鱼巣/线程/任务管理线程.ixx',
  '海中鱼巣/线程/任务工作线程.ixx',
  '海中鱼巣/装配.运行期业务.ixx',
  '海中鱼巣/启动.生产运行期.ixx',
  '海中鱼巣/启动.运行期上下文.ixx',
  '海中鱼巣/线程/协议.D455生产运行期.ixx',
  '海中鱼巣/装配.D455生产运行期.ixx',
  '海中鱼巣/装配.D455恢复.ixx',
  '海中鱼巣/适配/自检.D455生产闭环.ixx',
  '海中鱼巣/自检.运行器.ixx',
  '海中鱼巣/入口.cpp',
  '海中鱼巣.vcxproj',
  '海中鱼巣.vcxproj.filters',
  '实施记录/20260724_PERCEPTION-CUTOVER_PER-C13真实D455闭环接线验收_Codex断点清单.md'
) | Sort-Object
$分支 = (git branch --show-current).Trim()
if (-not $分支.StartsWith('codex/')) { exit 1 }
git diff --check
if ($LASTEXITCODE -ne 0) { exit 1 }
git add -- $期望
if ($LASTEXITCODE -ne 0) { exit 1 }
git diff --cached --check
if ($LASTEXITCODE -ne 0) { exit 1 }
$已暂存 = @(git -c core.quotepath=false diff --cached --name-only) | Sort-Object
if (Compare-Object $期望 $已暂存) { exit 1 }
git commit -m '实施：完成PER-C13真实D455生产闭环候选'
if ($LASTEXITCODE -ne 0) { exit 1 }
git diff --check "$计划段起点..HEAD"
if ($LASTEXITCODE -ne 0) { exit 1 }
$实际 = @(git -c core.quotepath=false diff --name-only "$计划段起点..HEAD") | Sort-Object
if (Compare-Object $期望 $实际) { exit 1 }
if (git status --porcelain) { exit 1 }
git push origin "HEAD:refs/heads/$分支"
if ($LASTEXITCODE -ne 0) { exit 1 }
if (git status --porcelain) { exit 1 }
```

两次真实验收各自须满足第 5 节全部计数，退出 0，且每次退出后残留进程 / 设备占用为 0。`Compare-Object` 必须零输出；只读和表外文件命中数必须为 0。

## 7. DRIFT、失败与完成边界

- `DRIFT-PER-FIXED-CANDIDATE`：依赖提交、ABI、模块或父链不符。
- `DRIFT-PER-HARDWARE-IDENTITY`：设备、标定、SDK 或占用事实不符。
- `DRIFT-PER-RECOVERY-CONTRACT`：四提供者、恢复字段或发布顺序漂移。
- `DRIFT-PER-FILE-OWNERSHIP`：需要表外文件或修改私有候选。
- `DRIFT-PER-IMPORT-CYCLE`：固定提供者形成反向依赖；`DRIFT-PER-CURRENT-INTERFACE`：现行运行宿主、线程、入口或工程无法按合同适配。
- 半上下文可见、设备释放失败、停止后仍发布、服务版本矛盾为 `FAIL-PER-INTERNAL-READBACK`；白名单 / ABI / 静态验证失败为 `FAIL-PER-VALIDATION`；构建 / 自检 / 硬件验收失败为 `FAIL-PER-E2E`。
- 合法 WIP 只允许在白名单内形成具名 clean 断点提交；不得跳过真实硬件矩阵。
- 完成提交仍只证明执行分支候选；独立集成进入 main 且全部真实验收通过后，才可声明真实 D455 观察体素生产闭环完成。
