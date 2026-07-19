# TASK-RESULT-STABILITY-S2 JY-470 原子状态替换异常具名退回与独立集成准备

日期：2026-07-19

状态：正式设计记录 / 具名退回已接受 / 分支完成待独立文档集成

## 1. 任务提交

```text
任务：#313 / DQ-205 v0.4 / WB-DIAG-06 / WT-313-R4
提交：464a6f2c1d5879bed8a65a159ad25ea48dde1d7e
唯一父：0890464414a8ffd5dfc71dcfc80641d9e4f18181
远端：origin/codex/313-task-result-subcondition-r4 同哈希，0/0
范围：精确一份 R4 专属诊断记录
状态：worktree / index clean；源码恢复冻结 blob
```

## 2. 接受的异常事实

第 43 轮产品进程自然退出 0，TASK / B3C / B0 精确 18 / 12 / 20 且失败 0。脚本已生成下一轮 44、未终止的临时六键状态和摘要，但随后执行：

```powershell
[System.IO.File]::Replace($TemporaryPath, $StatePath, $null)
```

抛出 `ArgumentException: The path is not of a legal form.`。脚本最终退出 1，正式状态仍为下一轮 43、未终止。因此第 43 轮不计新有效轮，历史有效轮仍只有 1—42；未补跑 43，未启动 44—60。

该异常只归类为“产品完成后原子状态替换调用异常”，不是产品失败，不是 TASK-RESULT-S1 失败，也不构成确定根因。

## 3. 资源与恢复

共享事件段目录已释放，`海中鱼巣.exe` 进程数为 0。临时观察补丁已恢复到 blob `74734d038e3baa0be12a46ee1f275273305b7bf5`。非 Git脚本、状态、样本、构建日志、轮日志和摘要未进入提交。

## 4. 独立集成

```text
集成批次：INT-WB-DIAG-06-R1
路径：C:\Users\zhchh\.codex\worktrees\integration-wb-diag-06-r1\海中鱼巢
分支：codex/integration-wb-diag-06-r1
起点：E215 发布后的 origin/main 精确提交
输入：origin/codex/313-task-result-subcondition-r4@464a6f2c1d5879bed8a65a159ad25ea48dde1d7e
```

集成只允许无冲突汇入任务记录并新增唯一集成记录；不构建、不运行、不占共享目录，不修改 C++、工程、入口或中央治理。E215 发布后才发送正式集成消息并读回接管。

## 5. 边界

本记录不修复 `File.Replace`，不补跑第 43 轮，不生成 v0.5，不放行 #310 / #305 / #301 / #306。只有诊断记录进入 main 并由设计窗口归档后，才能裁决无备份原子替换的正式 API 与新 worktree。
