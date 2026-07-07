---
name: hai-zhong-yu-chao-root-cause
description: Trace popup, Debug Error, abort, runtime anomaly, logic-error log, failed validation, or unexpected state root causes in D:\海中鱼巣. Use when the user asks to 追根因, 定位弹窗根因, 分析逻辑错误, 追查运行异常, or explain and fix why an error appears instead of hiding it.
---

# 海中鱼巣根因追踪

## Core Meaning

Treat every popup, logic-error log, failed validation, abnormal state, or crash-adjacent symptom as:

```text
现象
-> 直接生成点
-> 入口拒绝 / 服务边界 / 写入结构
-> 上游输入与生命周期
-> 违反规范或计划的第一个点
-> 最小修复
-> 复验同一现象消失且无同类错误
```

Do not solve by hiding popups, downgrading logs, swallowing failures, or adding silent fallbacks.

## Preflight

1. Confirm cwd is `D:\海中鱼巣`.
2. Read `AGENTS.md`.
3. Read:

```text
计划/计划索引.md
规范/000_项目规则总纲.md
规范/001_规则迁移清单.md
项目记忆/当前状态.md
```

4. If code edits may be needed, also read the confirmed implementation slice and target file headers.
5. Check `git status --short`; keep unrelated dirty files out of the slice.

## Layered Search

Use layers:

```text
L0 现象层：弹窗、日志、退出码、验收输出、用户可见异常。
L1 直接生成层：弹窗 / 日志 / 验收失败调用点。
L2 本地事实层：入口拒绝、服务写入、仓库返回、结构读取。
L3 上游请求层：调用者、输入包、任务 / 方法 / 动态 / 状态来源。
L4 生命周期层：节点、关系、状态、任务、方法、动作、因果的创建和失效顺序。
L5 规范契约层：AGENTS、规则总纲、详细设计、计划切片。
```

Record same-layer candidates before drilling deeper.

## Evidence Rules

- Prefer `rg`, scoped file reads, path-limited diffs, and current logs.
- Logs and popups are evidence of a symptom, not machine facts.
- Write expected behavior before running validation.
- Use real build/runtime evidence. Do not fabricate tests or samples.
- If a guard seems too strict, prove upstream state is valid before relaxing it.

## Root-Cause Record

For nontrivial root-cause work, create or update:

```text
实施记录/YYYYMMDD_<主题>根因追踪_Codex断点清单.md
```

Record:

```text
当前现象
直接生成点
当前层级
同层候选
选中分支
已确认事实
已排除
已改文件
已运行命令
下一步
验收证据
禁止宣称
```

If the fix requires new scope, write the issue to `项目记忆/待确认问题.md` instead of expanding implementation.

## Fix And Verify

1. State files/functions to touch before editing.
2. Edit minimally with `apply_patch`.
3. Build and run only when the plan allows it.
4. Validate the original symptom:
   - same popup/log absent;
   - failed validation now passes;
   - predicted structure/state matches actual output;
   - no unrelated new failure appears.
5. Update breakpoint, plan index, and project memory.
6. Commit and push only this slice’s files.

## Hard Boundaries

- Do not bypass domain services.
- Do not write bad structures then rely on later validation.
- Do not use logs, console output, comments, or display text as machine logic.
- Do not move into SQL/control-panel/D455/voxel/peripheral work unless a confirmed plan allows it.
- Do not declare old capability migration or system completion from a bug fix.
