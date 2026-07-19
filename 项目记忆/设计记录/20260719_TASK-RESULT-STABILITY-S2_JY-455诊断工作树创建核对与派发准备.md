# TASK-RESULT-STABILITY-S2 JY-455 诊断工作树创建核对与派发准备

日期：2026-07-19

状态：正式设计记录 / WT-313-R1 已创建待派发

## 1. 前置

JY-454 / E199 已发布 #313 / DQ-205 / WB-DIAG-03，并冻结：

```text
候选：0890464414a8ffd5dfc71dcfc80641d9e4f18181
父提交：1d95aa5975057965fcfa179c56521ad69e5b5b6a
自检 blob：74734d038e3baa0be12a46ee1f275273305b7bf5
路径：C:\Users\zhchh\.codex\worktrees\313-task-result-subcondition-r1\海中鱼巢
分支：codex/313-task-result-subcondition-r1
```

## 2. 创建前后核对

创建前，目标路径、本地分支、远端同名分支和 worktree 登记均不存在。创建后：

```text
HEAD：0890464414a8ffd5dfc71dcfc80641d9e4f18181
唯一父：1d95aa5975057965fcfa179c56521ad69e5b5b6a
分支：codex/313-task-result-subcondition-r1
worktree / index：clean
upstream：无
远端同名：不存在
自检 blob：74734d038e3baa0be12a46ee1f275273305b7bf5
```

身份与 #313 v0.1、登记表 11.8 一致，不需要重建、重置或复用其它 worktree。

## 3. 派发边界

目标任务固定为 `019f73ee-7d09-7a20-ad9f-1788ea62de23`。E200 推送后先只派发 S0：重读中央正式状态，核对 A14 / A18 字段、作用域、失败后输出位置、反向补丁恢复和最终单记录范围。

S0 读回一致前，不得修改自检、创建诊断目录、构建或运行。S0 通过也不自动取得固定事件段目录；设计窗口必须另行核对进程和其它 worktree 暂停，再明确授予共享时隙。

本记录不放行 #310，不修改 #305，不解锁 #301 / #306，不创建 WB-P0B-02 集成。
