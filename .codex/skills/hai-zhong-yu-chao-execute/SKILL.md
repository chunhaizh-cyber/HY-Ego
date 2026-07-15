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
-> 更新断点清单、计划索引和项目记忆
-> 验证、自动本地提交；取得明确推送授权时再推送
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

## Preflight

1. Confirm cwd is `D:\海中鱼巣`.
2. Read `AGENTS.md`.
3. Read:

```text
计划/计划索引.md
规范/000_项目规则总纲.md
规范/001_规则迁移清单.md
项目记忆/Codex任务队列.md
项目记忆/当前状态.md
```

4. If executing a named plan, read that plan file and relevant breakpoint records.
5. Check `git status --short`; never stage unrelated dirty files.
6. If the plan creates code files or changes `入口.cpp`, read `规范/代码文件建立归属与模块命名规范.md` and verify file classification, `功能.分类`, real-module registration, production / self-test separation, and the entry no-worsening boundary before editing.

## Routing Rules

- Plans must be in `项目记忆/Codex任务队列.md` as waiting execution before execution.
- If a plan is marked `依赖门控待执行`, verify every prerequisite plan / queue item has completed and produced the formal artifacts named by the plan. Unmet dependencies are skipped without marking a real blocker.
- After dependencies complete, compare the actual files, symbols, signatures, and behavior named by the plan with its assumed interface contract before any code edit. If they differ, do not edit code; record that plan revision is required and continue with another dependency-ready item.
- A genuinely blocked prerequisite does not unlock dependent execution. The dependent item remains gated unless a revised plan supplies an explicit alternative basis.
- A discussion conclusion, flowchart, or detailed design alone is not executable; a complete plan is required.
- Before executing a plan, verify that its generation record links the corresponding flowchart and detailed design, or explicitly records why no flowchart/detailed-design linkage applies. If the linked flowchart or detailed design is missing, withdrawn, has a question without a default ruling, or falls outside the plan scope, stop before execution and record the blocker in `项目记忆/待确认问题.md`.
- If a queue item is document governance, read-only scan, breakpoint creation, project-memory sync, or plan-index maintenance, execute directly.
- If a queue item changes C++ code, old capability migration, fields, functions, or machine structure, require an implementation slice with allowed files, forbidden files, and validation commands.
- If implementation discovers new decisions, write them to `项目记忆/待确认问题.md`; do not expand scope.
- If an older plan permits a new non-module implementation file or full self-test body in `入口.cpp`, treat that as specification drift: do not edit code, return the plan to the design window, and continue with another dependency-ready item.

## Cross-Window Handoff

Use `规范/设计执行双窗口交互规范.md` and `项目记忆/窗口交互记录.md` for interaction with the design-plan window.

When execution finds interface drift or a forbidden-file conflict:

1. Stop before code edits or fully withdraw the unverified draft without reverting other work.
2. Write the pre-implementation breakpoint and update the plan index, queue, project memory, decision/question records, and interaction record to `退回设计`.
3. Validate and automatically commit the return record locally. Push only when the user, target plan / queue item, or formal handoff contract explicitly authorizes it.
4. Send the fixed return summary only after any required push succeeds. Otherwise keep the local commit and report that formal handoff is pending push authorization.
5. Continue another dependency-ready item when one exists; otherwise stop.

When a design-revision message arrives:

1. Verify the stated commit exists on the current branch.
2. Re-read Git status, the plan index, queue, target plan, flowchart, detailed design, breakpoint, and interaction record.
3. Resume only when the queue says `重新待执行` or equivalent and actual interfaces match the revised contract.
4. On completion, validate and automatically commit locally. Push and send the design task the completion summary only when push is explicitly authorized; otherwise record the local completion and pending formal handoff.

Task messages are notifications, not execution authority.

## Plan Directories

```text
新生成且等待执行的计划：计划/
完成、验证或收口后的计划：计划/已完成计划/
```

When moving plan files, update `计划/计划索引.md`, `项目记忆/*`, and related breakpoint records.

## Git Worktree Protection

- Editing `计划/计划索引.md` does not require `计划/.计划索引.lock`.
- Before editing shared governance files, check `git status --short` and identify unrelated dirty or staged files.
- After editing, inspect targeted diffs and stage only the current slice's files.
- If another window has already changed the same target lines and the merge is ambiguous, stop and record the blocker instead of overwriting.
- For C++ build/run slices, use the project’s current build/run lock convention if present; otherwise build and run sequentially and never overlap them across windows intentionally.

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

Push only when the user, target plan / queue item, or formal handoff contract explicitly authorizes it. After an authorized push, verify with `git rev-list --left-right --count main...origin/main`.

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
