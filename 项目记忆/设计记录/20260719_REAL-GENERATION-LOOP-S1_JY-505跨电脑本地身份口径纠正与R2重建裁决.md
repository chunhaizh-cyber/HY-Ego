# JY-505 #315 跨电脑本地身份口径纠正与 R2 重建裁决

日期：2026-07-19

## 1. 用户纠正

用户明确说明已经切换运行电脑，因此当前本机找不到旧电脑上的 R2 本地分支和 worktree 是必然现象。

设计窗口据此纠正 JY-504：Git 本地分支、worktree 路径和 `.git/worktrees` 登记属于主机本地状态；换电脑后缺失不等于远端任务身份、提交父链、计划版本或执行所有权发生漂移。

## 2. 纠正后的事实

- `origin/codex/315-real-generation-loop-s1-r2` 仍精确指向 `0981e8a89ddb6ba1b853b3b68a65092037088ed1`。
- WIP 唯一父仍为 `57cacbc78cd4aa40e5cd85791baabb80a43e2c48`，相对父提交精确七文件，`git diff --check` 通过。
- 计划仍为 #315 / DQ-207 / REAL-GENERATION-LOOP-S1 v0.2，暂停断点仍明确不可集成、不解锁 #316。
- 本机 R2 路径和本地同名分支不存在，仅说明尚未从远端物化原任务身份。
- JY-504 后误建的 R3 位于 `codex/315-real-generation-loop-s1-r3@0981e8a`，与远端 R2 为 0/0，worktree / index clean、无 upstream、远端 R3 不存在；未修改、未构建、未运行、未提交或推送。

## 3. 裁决

1. JY-504 保留为历史误判证据，不重写已发布 main 历史。
2. WB-REAL-LOOP-03 / WT-315-R3 不承担执行或集成，标记为冗余可回收。
3. E250 发布后，设计窗口先复核 R3 仍为 `0981e8a`、clean、无远端，再移除 R3 worktree 和本地分支；远端 R2 完整保留 WIP，故回收不丢失代码。
4. 随后从 `origin/codex/315-real-generation-loop-s1-r2@0981e8a` 在新电脑路径 `C:\Users\zhchh\.codex\worktrees\315-real-generation-loop-s1-r2\海中鱼巢` 重建本地 `codex/315-real-generation-loop-s1-r2`，保持无 upstream。
5. 重建后核对路径、分支、HEAD、父链、clean、七文件、计划 v0.2、进程 0 和共享目录未占用，形成新的创建回执并提交推送。
6. 创建回执发布后，才向唯一任务 `019f46a8-7cd7-7622-88c4-fc45cc5ea016` 派发原 R2 只读 S0；S0 通过前不授权 self-test 或连续验证。

## 4. 完成声明六类证据

- 输入入口：用户换电脑说明、JY-503 / JY-504、远端 R2 引用、当前本机 worktree / branch 事实。
- 核心处理：区分主机本地身份与远端任务身份，撤销“不存在即漂移”的错误推断，恢复原 R2 路线。
- 结构承载：计划索引、任务队列、当前状态、并行工作树登记表、窗口交互记录和本设计记录。
- 输出变化：R3 标记冗余可回收；R2 恢复为跨电脑本地重建目标；#315 仍不可运行 / 不可集成，#316 继续门控。
- 调用路径：E250 发布 -> 精确回收 R3 -> 从远端 R2 重建本地身份 -> 发布创建回执 -> 派发只读 S0。
- 验证方式：远端引用、HEAD / parent、0/0、worktree / index clean、upstream、远端 R3 不存在、七文件、strict、diff 和 staged 范围检查。

## 5. 本轮边界

本轮只纠正设计治理口径并登记回收 / 重建门禁；提交前不回收 R3、不创建 R2、不运行构建、自检或产品、不发送执行消息。
