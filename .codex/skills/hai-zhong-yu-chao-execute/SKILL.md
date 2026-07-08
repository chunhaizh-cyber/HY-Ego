---
name: hai-zhong-yu-chao-execute
description: Execute or resume the D:\海中鱼巣 plan-index workflow. Use when the current window is explicitly an execution window and the user says "执行", "继续", "按照计划索引执行", "继续执行计划", "持续执行", or when the user asks Codex to consume the confirmed executable queue from `项目记忆/Codex任务队列.md` and `计划/计划索引.md` until the queue is empty, a real blocker appears, or the user stops it. Do not use bare "继续" as an execution trigger in a design-confirmation window or read-only monitoring window.
---

# 海中鱼巣执行

## Core Meaning

Treat execution as:

```text
读取 AGENTS.md 和项目规则
-> 读取计划索引与 Codex 任务队列
-> 只消费已确认、已登记的可执行项
-> 执行最小闭合切片
-> 更新断点清单、计划索引和项目记忆
-> 验证、提交、推送
-> 继续下一项直到队列清空或遇到真实阻塞
```

Do not self-authorize unconfirmed design, code implementation, old-function migration, field landing, SQL/control-panel/D455/voxel/peripheral work.

## Window-Type Gate

Before treating `继续` as execution, identify the current window type:

```text
设计确认窗口：`继续` means continue design / rules / plans / information data / confirmation material. Do not consume the executable queue.
执行窗口：`继续` means continue consuming confirmed queue items.
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

## Routing Rules

- Confirmed plans must be in `项目记忆/Codex任务队列.md` before execution.
- A confirmation statement alone is not executable.
- If a queue item is document governance, read-only scan, breakpoint creation, project-memory sync, or plan-index maintenance, execute directly.
- If a queue item changes C++ code, old capability migration, fields, functions, or machine structure, require an implementation slice with allowed files, forbidden files, and validation commands.
- If implementation discovers new decisions, write them to `项目记忆/待确认问题.md`; do not expand scope.

## Plan Directories

```text
新生成未确认计划：计划/待确认计划/
确认后仍在承接的计划：计划/
完成、验证或收口后的计划：计划/已完成计划/
```

When moving plan files, update `计划/计划索引.md`, `项目记忆/*`, and related breakpoint records.

## Locks

- Before editing `计划/计划索引.md`, create `计划/.计划索引.lock`; remove it after the index edit is complete.
- If a fresh lock exists, do not edit the index. Record the blocker.
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

After each completed slice, stage only this slice’s files, commit, push, and verify:

```powershell
git rev-list --left-right --count main...origin/main
```

## Stop Conditions

Stop only when:

- the executable queue is empty;
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
外设 / 体素 / 控制面板已接入
```
