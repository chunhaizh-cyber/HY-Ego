# REAL-GENERATION-LOOP-S1 JY-544 唯一执行代理可达登记与 S0-A 派发准备

日期：2026-07-20

角色：WT-MAIN 设计窗口

对象：#315 / DQ-207 / WT-315-R8 / REAL-GENERATION-LOOP-S1 v0.7

## 1. 前置发布

```text
E287 设计包：484f999a21558cf7b25f503e715e5b258802c4b0
E288 R8创建登记：99e773e4242dedc4e1547fce9f945b69ddcad186
main == origin/main@99e773e4242dedc4e1547fce9f945b69ddcad186
```

## 2. 唯一目标窗口

E288 发布后，设计窗口创建新的独立候选：

```text
canonical task name：/root/wt315_r8_execution
```

代理只返回：当前尚未接管 WT-315-R8，等待设计窗口在正式仓库登记并推送后再派发 S0-A。代理未调用工具、未读取文件/worktree、未运行 Git/构建/产品、未修改/暂存/提交/推送、未开始 S0、未消费 #315。

该代理是 R8 后继的唯一目标。旧 R7 代理继续只读失效，不得取得 R8 写入权。

## 3. R8 读后事实

```text
路径：C:/Users/zhchh/.codex/worktrees/315-real-generation-loop-s1-r8/海中鱼巢
分支：codex/315-real-generation-loop-s1-r8
HEAD / 冻结基线：484f999a21558cf7b25f503e715e5b258802c4b0
worktree / index：clean
upstream：无
远端同名分支：不存在
相对 origin/main@99e773e：0 / 1；唯一落后为 E288 中央创建登记
固定源树：未恢复
代码：零修改
```

## 4. 资源读后事实

JY-543 时的无参数产品 PID40560 已自行结束，设计窗口未终止。当前共享目录项目数仍为 0，剩余进程为 Visual Studio 拥有或派生的：

```text
MSBuild.exe PID36188，父进程 devenv.exe，nodeReuse
mspdbsrv.exe PID27780
vctip.exe PID30756
海中鱼巣.exe：0
```

设计窗口不裁决这些进程是否满足执行计划的资源门禁，也不终止非本窗口进程。S0-A 必须重新读取进程父链、命令行、共享目录和 R8 身份，并按 v0.7 明确返回 PASS 或 `资源DRIFT`；若 DRIFT，停止，不进入 S0-B/S1，不写记录或代码。

## 5. 派发门禁

E289 发布前不向代理发送 S0-A。E289 推送并从 `origin/main` 读回后，只发送 S0-A 身份与资源检查：

1. 核对 R8 路径、分支、HEAD、冻结基线、clean/index、无 upstream、远端不存在、相对 origin/main 仅落后 E288/E289 中央治理。
2. 核对产品、活动构建、MSBuild/mspdbsrv/vctip 的父链与命令行以及共享目录。
3. 只返回 PASS/DRIFT 和精确证据；不读取 S0-B/C/D，不恢复源树，不修改、构建、运行、暂存、提交或推送。

设计窗口读回并另行裁决前不得进入下一步。

## 6. 当前状态

```text
R8：已创建待派发
唯一目标：/root/wt315_r8_execution
S0-A：未派发
写入者：无
#316：继续依赖门控
```

## 7. 完成声明边界

JY-544 只证明唯一新执行代理可达并已正式登记为待命目标。它不证明 S0-A 已派发/通过、资源门禁通过、源树恢复、代码修复、验证、任务提交、集成或 #315 完成。
