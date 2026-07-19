# TASK-RESULT-STABILITY-S2 任务结果结算独立单轮脚本可续接逐子条件证据诊断 Codex 断点清单

日期：2026-07-19

计划：#313 / DQ-205 / TASK-RESULT-STABILITY-S2 v0.4

批次：WB-DIAG-06

任务工作树：WT-313-R4

状态：具名退回：第 43 轮产品完成后原子状态替换异常，诊断完成待设计裁决 / 分支完成待集成

## 1. 冻结身份与 S0

```text
worktree=C:\Users\zhchh\.codex\worktrees\313-task-result-subcondition-r4\海中鱼巢
branch=codex/313-task-result-subcondition-r4
HEAD=0890464414a8ffd5dfc71dcfc80641d9e4f18181
唯一父=1d95aa5975057965fcfa179c56521ad69e5b5b6a
冻结自检blob=74734d038e3baa0be12a46ee1f275273305b7bf5
历史有效轮次=1-42
历史摘要SHA256=4F5C5D7BE94E547527E30361F40F1C5F9D8767076BD5CC6FE41E427B71EA2EDE
下一轮=43
```

S0 已核对 Git 身份、clean、index、upstream、远端、冻结源码、v0.1-v0.3 正式记录、A14 十四项、A18 十一项、完成组和结算组各 100 条结果、四个公开字段、独立单轮脚本合同及最终单记录范围，全部一致。

## 2. S1 临时观察补丁与单轮脚本

临时补丁只修改 `海中鱼巣/线程/自检.任务结果结算.ixx`，在两组线程全部 join、正式读回和原 `验收[13]` / `验收[17]` 计算后增加 A14 十四项、A18 十一项、完成组 100 条、结算组 100 条及异常观察输出。原验收表达式、线程数、请求、夹具、阶段 610 和顺序未修改。

非 Git 单轮脚本：

```text
D:\海中鱼巣\日志\诊断\TASK-RESULT-STABILITY-S2-R4\run-one-round.ps1
SHA-256=095AAF9EC0E3370C4EE09C3F0F7099BD5C969DD6108A53C6E331B326A97370AC
```

脚本只封装一轮六键状态核对、一个同步产品进程、完整日志、TASK / B3C / B0 解析、失败证据门禁、摘要、日志哈希和原子状态替换。脚本不含跨轮执行、后台启动、递归或后继轮自行调用。

## 3. S2-S3 门禁

Debug x64 Rebuild：

```text
exit=0
warning=0
error=0
完整日志=D:\海中鱼巣\日志\诊断\TASK-RESULT-STABILITY-S2-R4\debug-rebuild.log
日志SHA-256=7621278AA6ECA42833313F192198AD20798EEE64758B20FE276209B86881B512
```

产品启动前门禁：

```text
PowerShell AST错误数=0
脚本冻结哈希匹配=是
LF六键样本=通过
CRLF六键样本=通过
同解析器逐键一致=是
同脚本 -ValidateOnly -Round 43 exit=0
ValidateOnly六键数量=6
ValidateOnly下一轮=43
ValidateOnly终止状态=未终止
ValidateOnly产品启动数=0
ValidateOnly结束后产品进程数=0
```

预检结束时没有 `resume-round-*.log`，没有 `resume-summary.tsv`，Git 仍只有允许的临时自检差异。

## 4. S4 第 43 轮首异常

第 43 轮使用唯一固定外层命令：

```powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "D:/海中鱼巣/日志/诊断/TASK-RESULT-STABILITY-S2-R4/run-one-round.ps1" -Round 43
```

产品进程已自然结束，脚本先形成完整日志和摘要。直接观察结果为：

```text
产品退出码=0
TASK=18行/失败0
B3C=12行/失败0
B0=20行/失败0
A14失败证据行=0
A18失败证据行=0
完成组证据行=0
结算组证据行=0
异常观察行=0
轮次日志SHA-256=D05A9BE01C9A3CD06D850599B2516C2EB513881D8ABDC05FD977946914F6BF62
摘要SHA-256=688574E15D8A73807D1CC36781025AA1F1E435D4F07CB535D7B1FDC6CF27A466
```

摘要原始单行：

```text
43<TAB>0<TAB>18<TAB>0<TAB>12<TAB>0<TAB>20<TAB>0<TAB>0<TAB>0<TAB>0<TAB>0<TAB>0<TAB>D05A9BE01C9A3CD06D850599B2516C2EB513881D8ABDC05FD977946914F6BF62
```

产品输出判定完成后，脚本已生成六键完整的临时状态，内容为下一轮 44、终止状态未终止；随后执行：

```powershell
[System.IO.File]::Replace($TemporaryPath, $StatePath, $null)
```

该调用抛出：

```text
Exception calling "Replace" with "3" argument(s): "The path is not of a legal form."
CategoryInfo=MethodInvocationException
FullyQualifiedErrorId=ArgumentException,run-one-round.ps1
```

因此单轮脚本最终退出 1，正式状态未被替换：

```text
正式状态SHA-256=80554F08716A457D5D029D9E7079F4D56AAC484CCF77524A66D076367B9C8DAE
正式下一轮=43
正式终止状态=未终止
临时状态SHA-256=40796DB9F48E94B68CB70A20582197BC9AD90021B834271A9CBA402230B149BC
临时下一轮=44
临时终止状态=未终止
```

按 v0.4 合同，只有脚本退出 0 且正式游标原子推进的轮次才有效。第 43 轮虽观察到产品退出 0 和 18 / 12 / 20 失败 0，但脚本退出 1、正式游标未推进，因此不计为新有效轮。历史有效轮仍仅为 1-42。第 43 轮未补跑，第 44-60 轮未启动。

本记录只把首异常归为“产品完成后原子状态替换调用异常”，不把它解释为产品失败、TASK-RESULT-S1 失败或确定根因。

## 5. S5 释放与恢复

首异常后已立即停止并向设计窗口明确释放 `D:\海中鱼巣\日志\事件段\自检`。释放时和收口复核时 `海中鱼巣.exe` 进程数均为 0。

临时观察补丁已用反向 `apply_patch` 撤回：

```text
恢复后源码blob=74734d038e3baa0be12a46ee1f275273305b7bf5
目标冻结blob=74734d038e3baa0be12a46ee1f275273305b7bf5
```

脚本、脚本哈希、状态、临时状态、样本、构建日志、轮次日志和摘要全部保留在非 Git R4 诊断目录，不进入任务提交。

## 6. 完成边界

本次只形成原子状态替换异常的诊断证据，不修复单轮脚本，不补跑第 43 轮，不运行第 44-60 轮，不放行 #310，不完成 #305，不解除 #301 / #306，不消费后继，也不声明 TASK-RESULT-S1 已稳定或任何迁移能力完成。

后继是否修订原子替换实现并重新派发，必须由设计窗口根据本记录和非 Git 证据裁决。
