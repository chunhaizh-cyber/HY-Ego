---
name: hai-zhong-yu-chao-execute
description: Execute or resume the D:\海中鱼巣 plan-index workflow. Use when the current window is explicitly an execution window and the user says "执行", "继续", "按照计划索引执行", "继续执行计划", "持续执行", or when the user asks Codex to consume the waiting executable queue from `项目记忆/Codex任务队列.md` and `计划/计划索引.md` until the queue is empty, a real blocker appears, or the user stops it. Do not use bare "继续" as an execution trigger in a design window or read-only monitoring window.
---

# 海中鱼巣执行

## Core Meaning

Treat execution as:

```text
读取 AGENTS.md 和项目规则
-> 读取计划索引与 Codex 任务队列
-> 按队列检查依赖门控，只消费依赖已满足的待执行项
-> 执行前复核前置正式产物和实际接口
-> 执行最小闭合切片
-> 更新执行专属断点 / 实施记录；中央路由由设计角色同步
-> 验证、自动本地提交并自动非强制推送
-> 继续下一项直到没有依赖就绪项或遇到真实阻塞
```

Do not execute work that has no plan, exceeds a plan's allowed files, or lacks required upstream design and validation boundaries.

## Window-Type Gate

Before treating `继续` as execution, identify the current window type:

```text
设计窗口：`继续` means continue design / rules / plans / information data and queue registration. Do not consume the executable queue.
执行窗口：`继续` means continue consuming registered waiting queue items.
只读监控 / 复核窗口：`继续` means continue read-only fact checking and reporting. Do not edit files.
未声明窗口类型：infer from the latest explicit user declaration, thread title, and task object; if still unclear, do not execute from bare `继续`.
```

When the user explicitly enables the same-physical-window serial dual-role mode, use the role recorded in the plan index, queue, worktree registry, and interaction record as the current window type. Bare `继续` stays in that role and does not switch by itself. The design role must first record that the ownership switch takes effect after a successful push, validate, commit, and automatically push that record. The execution role may start only after the push succeeds and this task has re-read Git, the plan index, queue, worktree registry, target plan, breakpoint, and actual interfaces.

The same-window mode removes only a task message sent back to the same physical window. It does not remove the design/execution/integration permission split, interaction record, plan/dependency gates, allowed/forbidden files, actual-interface review, per-worktree single writer ownership, validation, commit, push, or re-read gates. It remains active until the user revokes it or the interaction record transfers write ownership.

## Preflight

1. Confirm cwd is either the registered `D:\海中鱼巣` main integration worktree or the exact task worktree assigned to this plan. Reject unregistered paths, branches, detached HEAD, or a branch checked out in the wrong worktree.
2. Read `AGENTS.md`.
3. Read:

```text
计划/计划索引.md
规范/000_项目规则总纲.md
规范/001_规则迁移清单.md
规范/多工作树并发与集成规范.md
项目记忆/Codex任务队列.md
项目记忆/当前状态.md
项目记忆/并行工作树登记表.md
```

4. If executing a named plan, read that plan file and relevant breakpoint records.
5. Check `git status --short`; never stage unrelated dirty files.
6. If the plan creates code files or changes `入口.cpp`, read `规范/代码文件建立归属与模块命名规范.md` and verify file classification, `功能.分类`, real-module registration, production / self-test separation, and the entry no-worsening boundary before editing.

## Routing Rules

- Plans must be in `项目记忆/Codex任务队列.md` as waiting execution before execution.
- In parallel mode, the current task worktree must have exactly one assigned plan, branch, frozen base, owner, and file set in `项目记忆/并行工作树登记表.md`. Global queue uniqueness is replaced only by per-worktree assignment uniqueness; undeclared parallel candidates are not executable.
- If a plan is marked `依赖门控待执行`, verify every prerequisite plan / queue item has completed and produced the formal artifacts named by the plan. Unmet dependencies are skipped without marking a real blocker.
- After dependencies complete, compare the actual files, symbols, signatures, and behavior named by the plan with its assumed interface contract before any code edit. If they differ, do not edit code; record that plan revision is required and continue with another dependency-ready item.
- A genuinely blocked prerequisite does not unlock dependent execution. The dependent item remains gated unless a revised plan supplies an explicit alternative basis.
- A discussion conclusion, flowchart, or detailed design alone is not executable; a complete plan is required.
- Before executing a plan, verify that its generation record links the corresponding flowchart and detailed design, or explicitly records why no flowchart/detailed-design linkage applies. If the linked flowchart or detailed design is missing, withdrawn, has a question without a default ruling, or falls outside the plan scope, stop before execution and record the blocker in `项目记忆/待确认问题.md`.
- If a queue item is document governance, read-only scan, breakpoint creation, project-memory sync, or plan-index maintenance, execute directly.
- If a queue item changes C++ code, old capability migration, fields, functions, or machine structure, require an implementation slice with allowed files, forbidden files, and validation commands.
- If implementation discovers new decisions, write them to `项目记忆/待确认问题.md`; do not expand scope.
- If an older plan permits a new non-module implementation file or full self-test body in `入口.cpp`, treat that as specification drift: do not edit code, return the plan to the design window, and continue with another dependency-ready item.

## Cross-Role Handoff

Use `规范/设计执行双窗口交互规范.md` and `项目记忆/窗口交互记录.md` for interaction with the design role, whether that role is in another physical window or the current one.

When execution finds interface drift or a forbidden-file conflict:

1. Stop before code edits or fully withdraw the unverified draft without reverting other work.
2. Write only the execution-owned pre-implementation breakpoint or return record; do not update the plan index, queue, central project memory, worktree registry, decision records, or interaction record.
3. Validate, automatically commit the return record locally, and automatically perform a non-force push to the currently registered task branch.
4. The integration role brings a pure return-record commit into `main`; the design role then updates central routing. Send the fixed return summary only after the task-branch push succeeds.
5. If the push fails, keep the local commit, record the real unpublished state, and do not claim that the design or integration role can take over.
6. Do not consume another plan in the same task worktree unless the registry and batch explicitly reassign it after central synchronization.

When a design revision is published, or an external design-revision message arrives:

1. Verify the stated commit exists on the registered branch or is contained in the registered frozen base.
2. Re-read Git status, worktree path, branch, frozen base, plan index, queue, worktree registry, target plan, flowchart, detailed design, breakpoint, and interaction record.
3. Resume only when the queue says `重新待执行` or equivalent and actual interfaces match the revised contract.
4. On completion, write only execution-owned implementation records, validate, automatically commit locally, and automatically perform a non-force push to the registered task branch. Report `分支完成待集成`; do not update central completion or switch to design until the integration and design roles publish the corresponding main state.

Task messages are notifications, not execution authority.

In the same-physical-window mode, a role switch is also not authority by itself: the pushed commit, plan index, queue, target plan, breakpoint, and interaction record must all agree before the new role acts.

## Plan Directories

```text
新生成且等待执行的计划：计划/
完成、验证或收口后的计划：计划/已完成计划/
```

When moving plan files, update `计划/计划索引.md`, `项目记忆/*`, and related breakpoint records.

## Git Worktree Protection

- Execution roles do not edit shared governance files. Before any task edit, verify the exact worktree id, absolute path, branch, frozen base, batch, plan version, owner, and file set in the registry.
- After editing, inspect targeted diffs and stage only the current slice's files.
- If another window has already changed the same target lines and the merge is ambiguous, stop and record the blocker instead of overwriting.
- Do not merge, rebase, cherry-pick, or pull `main` into a task branch. Frozen-base drift returns to design.
- Builds and runs may proceed concurrently only when worktrees have independent output paths and the batch validation matrix explicitly allows it; otherwise serialize them.

## Validation And Sync

For document-only slices:

```powershell
git diff --check
if (Test-Path .\tools\check_specs.py) { python .\tools\check_specs.py }
```

For code slices, use the implementation plan’s validation commands. Default build command when allowed:

```powershell
msbuild .\海中鱼巣.vcxproj /p:Configuration=Debug /p:Platform=x64 /m
.\x64\Debug\海中鱼巣.exe
git diff --check
```

After every completed task round that changed files, stage only that round's files and commit locally. Do not create empty commits, and do not include unrelated dirty files:

```powershell
git status --short
git diff --cached --check
git commit
```

After the automatic non-force push required by the project rules, verify the current task branch against its same-name remote branch. Do not compare or push task HEAD directly to `origin/main`. If the target is unclear, the remote has diverged, HEAD is detached, the registry differs, unrelated changes cannot be isolated, or validation failed, stop and report; never force, merge, or rebase as part of a task push.

## Stop Conditions

Stop only when:

- the executable queue is empty;
- every remaining queue item is dependency-gated and no dependency-ready item exists;
- a plan says hard stop / wait for user confirmation;
- a higher-level rule conflicts with the requested action;
- required runtime, hardware, credentials, or evidence are unavailable;
- the user explicitly pauses or redirects.

Do not stop merely because one stage, one commit, or one validation command succeeded.

## Forbidden Claims

Do not declare:

```text
鱼巢旧能力已迁移完成
自我循环已完成
自我苏醒完成
初步成熟完成
完整业务操作控制面板已完成
数据库恢复已完成
旧控制面板或旧数据库能力已迁移完成
外设 / 体素已接入
```
