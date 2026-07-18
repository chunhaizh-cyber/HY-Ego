---
name: hai-zhong-yu-chao-current-fact-scan
description: Use in D:\海中鱼巣 when the user says "按当前代码检查", "按当前 worktree 检查", "先扫描本地事实", "先确认当前入口和调用点", "S0 当前代码事实扫描", or asks Codex to confirm current local code facts before planning or editing.
---

# 海中鱼巣当前事实扫描

## Core Meaning

Treat this as a read-only S0/local-fact scan:

```text
当前目标
-> 当前 worktree 与 dirty state
-> 当前规则 / 计划 / 队列
-> 当前真实代码入口、调用点、结构承载
-> 缺口、风险、待确认问题
```

Do not edit files, run builds, or claim implementation completion unless the user explicitly expands scope.

## Preflight

1. Resolve the current Git top level and confirm it is either the registered main integration worktree or a registered task / integration worktree.
2. Read `AGENTS.md`.
3. Read:

```text
计划/计划索引.md
规范/000_项目规则总纲.md
规范/001_规则迁移清单.md
项目记忆/当前状态.md
项目记忆/Codex任务队列.md
项目记忆/并行工作树登记表.md
```

4. Read current Git evidence:

```powershell
git status --short --branch --untracked-files=no
git diff --name-only
git rev-parse --show-toplevel
git branch --show-current
git worktree list --porcelain
```

Use scoped `rg` and short file reads.

## Authority Order

```text
用户本轮明确要求
-> AGENTS.md
-> 规范
-> 详细设计
-> 计划索引 / 任务队列
-> 当前本地代码事实
-> 当前 dirty diff
-> 实施记录 / 断点清单
-> 说明书 / README / 日志 / 总结
-> 旧对话记忆
```

## Output Shape

When relevant, return:

```text
当前扫描对象：
当前分支：
当前 worktree 编号 / 类型：
登记基线与任务分配：
worktree 是否 dirty：
已读证据：
本地真实入口：
本地调用点：
本地承载结构：
缺函数 / 缺入口 / 缺证据：
受未提交改动影响：
待确认问题：
禁止直接宣称：
下一步建议：
```

If used as an executable S0 slice, write a breakpoint file under:

```text
实施记录/YYYYMMDD_<主题>S0当前代码事实扫描_Codex断点清单.md
```

and update project memory when the plan requires it.

## Boundaries

- Do not modify code, plans, specs, or docs unless the S0 slice explicitly asks to write the breakpoint/project-memory record.
- Do not combine facts from different worktrees without naming each path, branch, commit, dirty state, and registry status.
- Do not infer cloud state from local state.
- Do not treat logs, console output, or display text as machine facts.
- Do not say `已实现` without current code path, structure carrying path, and validation evidence.
