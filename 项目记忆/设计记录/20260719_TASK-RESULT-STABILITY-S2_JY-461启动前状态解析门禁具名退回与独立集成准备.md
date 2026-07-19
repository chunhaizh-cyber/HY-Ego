# TASK-RESULT-STABILITY-S2 JY-461 启动前状态解析门禁具名退回与独立集成准备

日期：2026-07-19

状态：正式设计记录 / v0.2 具名退回 / 分支完成待独立文档集成

## 1. 正式任务证据

```text
任务提交：17855611b0b0b7d157d06fec1a01df59e74d2f25
唯一父：0890464414a8ffd5dfc71dcfc80641d9e4f18181
范围：精确一份 v0.2 专属诊断记录
远端：origin/codex/313-task-result-subcondition-r2 同哈希，0/0
任务树：worktree / index clean
冻结源码：74734d038e3baa0be12a46ee1f275273305b7bf5
```

临时补丁静态门禁通过，Debug x64 Rebuild 为 0 警告、0 错误。共享事件段目录已释放，`海中鱼巣.exe` 进程数为 0。

## 2. 具名异常裁决

`resume-state.tsv` 由 Windows PowerShell 写成 CRLF。第 43 轮启动前门禁使用 `(?m)^下一轮=43$` 和 `(?m)^终止状态=未终止$`，其 `$` 前保留 `\r`，因此在执行产品进程语句前抛出状态不一致。

该事实只属于诊断编排不兼容：

```text
没有启动海中鱼巣.exe
没有 resume-round-43.log
没有产品退出码
resume-summary.tsv 仅表头
游标仍为 43
没有新的有效诊断轮
```

按照 v0.2 首异常即停、不得同轮补跑合同，接受具名退回。不得把它解释为 TASK-RESULT-S1 产品失败、候选稳定性证据或 #310 因果。

## 3. 独立集成预留

```text
编号：INT-WB-DIAG-04-R1
路径：C:\Users\zhchh\.codex\worktrees\integration-wb-diag-04-r1\海中鱼巢
分支：codex/integration-wb-diag-04-r1
输入：17855611b0b0b7d157d06fec1a01df59e74d2f25
起点：E206 发布提交，由派发消息给出完整哈希
允许：任务专属记录 + 唯一独立集成记录
验证：纯文档单父、单文件、blob、remote、strict、diff 和禁止范围
```

创建前目标路径、本地分支、远端同名分支和 worktree 登记均不存在。E206 发布后才发送正式集成消息；接管通过后取得主线发布占用。

## 4. 后继边界

集成发布并释放主线占用后，设计窗口可归档 v0.2 并修订 v0.3：状态文件解析必须对 CRLF / LF 无关，且在启动产品前用静态样本验证两种行尾。不得在原 R2 分支修改后补跑；应另立 WT-313-R3 并重新经历预留、创建、S0 和共享时隙门禁。
