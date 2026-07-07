---
name: hai-zhong-yu-chao-pre-implementation-review
description: Use in D:\海中鱼巣 when the user says "实施前审查", "先审查再改", "改之前先检查", "先确认能不能改", "实现前 review", or asks whether a planned code/document change is grounded in current facts, confirmed scope, allowed files, forbidden files, and validation rules.
---

# 海中鱼巣实施前审查

## Core Meaning

Use this as the gate before edits:

```text
用户目标
-> 当前本地事实
-> 规范 / 详细设计 / 已确认计划
-> 允许文件 / 禁止文件 / 验证方式
-> 是否可以实施
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
项目记忆/待确认问题.md
```

4. Read the target plan or implementation slice.
5. If current facts are stale or missing, require S0 fact scan first.

## Review Questions

Answer before permitting edits:

1. Is the target grounded in current local code/document facts?
2. Is there a formal spec, detailed design, or confirmed plan?
3. Is the requested action inside the current slice boundary?
4. Are allowed files and forbidden files explicit?
5. Will dirty worktree state contaminate the slice?
6. Are validation commands and completion boundaries explicit?
7. Does the action require user confirmation before machine-structure changes?

## Output States

```text
可执行
缺事实
缺规范
缺详细设计
缺实施切片
越界风险
脏树风险
待用户确认
```

## Required Output

```text
审查对象：
当前事实是否足够：
权威依据：
允许修改范围：
禁止修改范围：
现有可复用入口：
主要风险：
审查结论：
进入实现前的下一步：
```

Do not implement during this skill.

## Hard Boundaries

- Do not allow old function body migration without a confirmed service implementation slice.
- Do not allow old main-info field landing without confirmed field mapping.
- Do not allow SQL/control-panel/D455/voxel/peripheral work in first-round migration slices.
- Do not use logs, README, console output, or comments as machine logic.
