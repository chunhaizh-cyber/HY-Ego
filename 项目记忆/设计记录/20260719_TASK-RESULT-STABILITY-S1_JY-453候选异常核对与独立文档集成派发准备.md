# TASK-RESULT-STABILITY-S1 JY-453 候选异常核对与独立文档集成派发准备

日期：2026-07-19

状态：正式设计记录 / #312 分支完成待独立文档集成 / #310 继续暂停

## 1. 输入事实

```text
任务提交：a3be89ed4b5c5132dd693c8d92c8b1b8e5d7be88
唯一父提交：0890464414a8ffd5dfc71dcfc80641d9e4f18181
任务分支：origin/codex/312-task-result-candidate-r1
任务范围：精确一份 #312 专属诊断记录
任务状态：worktree / index clean，无 upstream，本地与远端哈希一致
```

Debug x64 Rebuild 为 0 警告、0 错误。候选完整自检第 1—16 轮均为 exit 0，TASK / B3C / B0 分别为 18 / 12 / 20 行且失败数均为 0；第 17 轮 exit 1，TASK 仍为 18 行，其中 `TASK-RESULT-S1-A14 完成重筹办结算三类百轮并发单提交` 与 `TASK-RESULT-S1-A18 上游回归与完整结果治理闭环` 失败，B3C 12 行和 B0 20 行的失败数均为 0。执行角色随后立即停止，第 18 轮未启动，共享目录已释放，进程为 0。

第 17 轮完整日志 SHA-256 为 `638A2623F691993ACE67FE2B9762AFE8B76F8469FC52431F130FE5C38B6FACEA`；摘要 SHA-256 为 `D3D647529A06CAB37DE66F5ED6548463714551026D93BA9B43EE0480905B3CC6`；构建日志 SHA-256 为 `259E2E96D5F62899AAEB17ED9E2A878F1E9D380C1AF22D02C39F9BE8E3DB3FB6`。

A14 与 A18 都是第 17 轮直接观察到的失败叶子。A18 内含多项治理闭环条件，现有输出没有逐子条件值，因此不得把 A18 简化为 A14 的派生失败，也不得从“基线本次未复现、候选本次复现”直接声明 #310 的确定因果。

## 2. 集成必要性

#312 的正式诊断记录进入 `main` 前，不得归档 #312、生成根因修复或恢复 #310 正式验证。任务提交只包含诊断记录，不是 WB-P0B-02 的代码集成输入，但仍须由独立集成角色完成范围核对、blob 核对和主线发布；设计角色不得直接 cherry-pick。

## 3. 独立集成冻结

```text
集成编号：INT-WB-DIAG-02-R1
目标路径：C:\Users\zhchh\.codex\worktrees\integration-wb-diag-02-r1\海中鱼巢
目标分支：codex/integration-wb-diag-02-r1
正式起点：JY-453 / E198 本记录发布后的 origin/main 精确提交
唯一输入：origin/codex/312-task-result-candidate-r1@a3be89ed4b5c5132dd693c8d92c8b1b8e5d7be88
输入父提交：0890464414a8ffd5dfc71dcfc80641d9e4f18181
目标任务：019f73ee-7d09-7a20-ad9f-1788ea62de23
```

创建前核对结果：目标路径、本地分支、远端同名分支和 worktree 登记均不存在。E198 推送后由集成角色从消息给出的精确 `origin/main` 创建并核对身份；消息送达前不占用主线发布权。

允许写入：

```text
实施记录/20260719_TASK-RESULT-STABILITY-S1_任务结果结算间歇失败候选对照诊断_Codex断点清单.md
实施记录/20260719_TASK-RESULT-STABILITY-S1_任务结果结算间歇失败候选对照诊断独立集成记录.md
```

禁止修改：

```text
海中鱼巣/**
海中鱼巣.vcxproj
海中鱼巣.vcxproj.filters
AGENTS.md
规范/**
流程图/**
计划/**
项目记忆/**
#310 / #305 任务分支
```

## 4. 集成步骤与门禁

1. 从 E198 消息给出的精确 `origin/main` 创建登记路径和分支；核对 WT-MAIN clean 且 `main == origin/main == 登记起点`。
2. 核对输入提交唯一父、远端哈希、精确单文件范围和任务记录内容；不把非 Git 原始日志复制进仓库。
3. 无冲突 cherry-pick `a3be89e`；逐 blob 核对任务记录与输入一致。
4. 只新增本次独立集成记录，写明纯文档不构建 / 不运行的原因、范围证据、诊断边界和主线发布结果。
5. 运行 `git diff --check` 和严格规范检查；确认 C++、工程、入口、中央治理零差异。
6. 推送同名集成分支；通过后让 WT-MAIN `main` 非强制快进并推送 `origin/main`，明确释放主线发布占用。

身份冲突、远端 main 前进、cherry-pick 冲突、超范围、任务记录 blob 不一致或静态检查失败时立即退回，不修改业务语义，不 reset、delete、rebase 或 force。

本纯文档集成不运行 C++ 构建或完整自检；#312 的运行证据已经在任务分支形成，集成不改变任何编译输入或运行行为。

## 5. 后继边界

集成发布只把 #312 诊断证据送入 `main`。设计窗口读取正式集成记录后，才能归档 #312，并根据 A14、A18 的实际判定式生成逐子条件诊断计划。当前不放行 #310，不创建 WB-P0B-02 集成，不解锁 #305、#301 或 #306。
