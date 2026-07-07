---
name: hai-zhong-yu-chao-plan
description: Use in D:\海中鱼巣 when the user says "制定计划", "生成计划", "拆分计划", "修订计划", "更新计划索引", "生成下一个计划", "加入设计计划队列", or asks Codex to turn confirmed specs, detailed designs, logic migration packages, or S0 facts into a plan-layer artifact. Generated but unconfirmed plans must go under `计划/待确认计划/`.
---

# 海中鱼巣制定计划

## Core Meaning

Treat planning as:

```text
规范 / 详细设计 / 逻辑迁移包 / S0 事实
-> 服务级计划或实施切片
-> 计划索引登记
-> 待确认或可执行队列
```

Do not create a code plan from chat memory alone. Do not treat old functions as migration units. Use:

```text
函数 = 证据采集单位
服务逻辑包 = 迁移确认单位
领域服务 / 结构闭环 = 代码实施单位
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

4. Read the relevant formal source:

```text
规范/
规范/详细设计/
实施记录/*逻辑迁移包归并表.md
实施记录/*S0当前代码事实扫描*_Codex断点清单.md
计划/
计划/待确认计划/
```

If no relevant design, migration package, or S0 fact exists, generate that prerequisite first if it is a document-governance action; otherwise stop and state what is missing.

## Output Location

- New unconfirmed plans go to `计划/待确认计划/`.
- Confirmed and still-active plans live in `计划/`.
- Completed plans move to `计划/已完成计划/`.

Recommended filenames:

```text
计划/待确认计划/YYYYMMDD_FSxx_主题专项_v0.1.md
计划/待确认计划/YYYYMMDD_FSxx_主题代码实施切片_v0.1.md
```

## Required Plan Sections

Include these sections when applicable:

```text
状态行
依据
确认范围
明确排除项
目标服务 / 目标结构
允许文件
禁止文件
实施步骤 S0/S1/...
验收方式
完成声明边界
待确认问题
```

For code implementation slices, allowed files, forbidden files, and validation commands are mandatory.

## Service-Level Migration Rules

- Do not write `旧函数 A -> 新函数 A`.
- Use old functions only as evidence anchors.
- Confirm service logic packages, service plans, or service implementation slices.
- Exclude SQL/control-panel/display mirror, D455, voxel, peripherals, old linked-list containers, and old main-info fields unless a later confirmed plan explicitly allows them.

## Plan Index And Queue

- Editing `计划/计划索引.md` requires `计划/.计划索引.lock`.
- If a plan is generated but not confirmed, register it as 待确认 and keep it under `计划/待确认计划/`.
- After user confirmation, move it to `计划/`, update references, and register the confirmed next executable item in `项目记忆/Codex任务队列.md`.
- When the plan is completed, move it to `计划/已完成计划/` and update references.

## Verification

For planning slices:

```powershell
git diff --check
if (Test-Path .\tools\check_specs.py) { python .\tools\check_specs.py }
```

Do not run C++ build unless the planning slice also includes code changes.
