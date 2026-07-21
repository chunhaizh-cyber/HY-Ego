---
name: hai-zhong-yu-chao-execute
description: Execute or resume the D:\海中鱼巢 plan-index workflow. Use only when the current window is explicitly and formally an execution window and the user says "执行", "继续执行", "按照计划索引执行", "继续执行计划", "持续执行", or explicitly asks Codex to consume this task worktree's assigned executable plan. Do not trigger this skill from bare "继续"; route context-dependent continuation through hai-zhong-yu-chao-continue first.
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
-> 返回设计角色核对并派发独立集成；只有正式预授权执行批次才继续本 worktree 的下一项
```

Do not execute work that has no plan, exceeds a plan's allowed files, or lacks required upstream design and validation boundaries.

## Window-Type Gate

Do not classify bare `继续` inside this skill. Use `hai-zhong-yu-chao-continue` first. Enter this execution workflow only after the current window has already been identified and formally registered as an execution window.

Use these boundaries when checking an explicit execution request:

```text
设计窗口：不进入本技能；继续设计并把执行派发到独立窗口。
执行窗口：只消费当前任务 worktree 正式分配的计划。
集成窗口：不进入本技能；只按登记批次执行集成工作。
只读监控 / 复核窗口：不进入本技能；只读事实检查和报告。
未声明窗口类型：不进入本技能；保持只读并先识别窗口。
```

When the user explicitly enables the same-physical-window serial dual-role mode, bare `继续` still stays in the current role and never activates this skill by itself. A switch to execution additionally requires an explicit user request to switch this physical window, a pushed ownership record, and a fresh read of Git, the plan index, queue, worktree registry, target plan, breakpoint, and actual interfaces. A design window whose next step names an execution or integration role must dispatch that work to a separate window unless the user explicitly orders the formal same-window switch.

The same-window mode removes only a task message sent back to the same physical window. It does not remove the design/execution/integration permission split, interaction record, plan/dependency gates, allowed/forbidden files, actual-interface review, per-worktree single writer ownership, validation, commit, push, or re-read gates. It remains active until the user revokes it or the interaction record transfers write ownership.

Follow `规范/0600_顶层任务树与子智能体协作治理规范_20260721.md`. The top-level task and all recursive subagents form one task tree and may hold only one write role at a time. An execution tree never uses a child as a design or integration writer. When two or more plan-authorized implementation, evidence, or verification slices are safely isolatable, use same-role or read-only subagents in parallel by default; otherwise record the named reason. Each dispatch must state the inherited role, allowed slice, forbidden actions, Git / lease permissions, and return condition. The top-level task verifies all results and serializes Git closeout.

## Preflight

1. Confirm cwd is either the registered `D:\海中鱼巢` main integration worktree or the exact task worktree assigned to this plan. Reject unregistered paths, branches, detached HEAD, or a branch checked out in the wrong worktree.
2. Read `AGENTS.md`.
3. Read:

```text
计划/计划索引.md
规范/规范目录.md
规范/0050_项目通用机器逻辑与禁止性规则总纲_20260721.md
规范/0100_编号规则与重排预留说明_20260720.md
规范/0300_规范冲突与前后矛盾清单_20260720.md
规范/0600_顶层任务树与子智能体协作治理规范_20260721.md
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
5. A task branch completion does not authorize this execution role to create or reuse an integration worktree. The design role must verify the task commit and perform a separate, real integration dispatch.

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
if (Test-Path .\tools\check_specs.py) { python .\tools\check_specs.py --strict }
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
