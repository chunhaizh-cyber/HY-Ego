---
name: hai-zhong-yu-chao-plan
description: Use in D:\海中鱼巢 when the user says "制定计划", "生成计划", "拆分计划", "修订计划", "更新计划索引", "生成下一个计划", "加入设计计划队列", or asks Codex to turn specs, detailed designs, logic migration packages, or S0 facts into a plan-layer artifact. Complete plans go directly under `计划/` and are registered as waiting execution.
---

# 海中鱼巣制定计划

## Core Meaning

Treat planning as:

```text
规范 / 详细设计 / 逻辑迁移包 / S0 事实
-> 服务级计划或实施切片
-> 计划索引登记
-> Codex 任务队列待执行
```

Do not create a code plan from chat memory alone. Do not treat old functions as migration units. Use:

```text
函数 = 证据采集单位
服务逻辑包 = 迁移计划单位
领域服务 / 结构闭环 = 代码实施单位
```

## Window Role Gate

Planning actions require the design role in the `main` integration worktree. The default route uses a dedicated design window. When the user explicitly enables the same-physical-window serial dual-role mode, the previous execution role must first record that the ownership switch takes effect after a successful push, validate, commit, and automatically push that record. The current task may act as the design role only after the push succeeds and it has re-read Git, the plan index, the queue, the worktree registry, the target plan, and the breakpoint. Bare `继续` stays in the currently registered role and never switches roles by itself.

The same-window mode removes only a task message sent back to the same physical window. It does not remove the design/execution/integration/read-only permission split, interaction record, dependency gates, allowed/forbidden files, per-worktree single writer ownership, validation, commit, push, or re-read gates. The mode remains active until the user revokes it or the interaction record transfers write ownership.

The design role owns end-to-end migration orchestration. After every archive or return, recompute dependencies and either dispatch the unique ready execution, integration, or review task, or record the exact gate. A reserved worktree, named target, or written next step is not a dispatch.

## Preflight

1. Confirm cwd is `D:\海中鱼巢`.
2. Read `AGENTS.md`.
3. Read:

```text
计划/计划索引.md
规范/000_项目规则总纲.md
规范/001_规则迁移清单.md
规范/多工作树并发与集成规范.md
项目记忆/Codex任务队列.md
项目记忆/并行工作树登记表.md
项目记忆/待确认问题.md
```

4. Read the relevant formal source:

```text
规范/
规范/详细设计/
实施记录/*逻辑迁移包归并表.md
实施记录/*S0当前代码事实扫描*_Codex断点清单.md
计划/
```

If no relevant design, migration package, or S0 fact exists, generate that prerequisite first if it is a document-governance action; otherwise stop and state what is missing.

If a plan creates or renames code files, read `规范/代码文件建立归属与模块命名规范.md`. The plan must classify every new file as a data-structure pair or a true module, use `功能.分类`, state the project item type and `include` / `import` path, separate production and self-test modules, and keep full test bodies out of `入口.cpp`.

If a plan preserves, deletes, reimplements, constrains, or newly creates an ability that can be confused with old-project migration, read `规范/迁移路线权力分层规范.md` and `实施记录/20260711_旧能力迁移与新内核建设逐能力台账.md`. The plan must state its source nature, old evidence id, target structure / writer, and whether completion counts toward old-capability migration.

Once a relevant detailed design has been generated, create the plan directly, write it under `计划/`, and register it in the plan index and Codex task queue as waiting execution. No separate user confirmation step remains.

Do not mark plans as parallel merely because their names differ. A pre-authorized parallel batch also requires a common frozen base, disjoint allowed-file sets, disjoint structure/interface ownership, a single owner for shared integration files, a fixed integration order, and an integration validation matrix. Register each task worktree, branch, plan version, base commit, owner, and file set in `项目记忆/并行工作树登记表.md` before any task worktree writes.

Do not wait for a prerequisite implementation merely to generate a dependent plan. Use:

```text
先生成依赖门控计划
-> 登记为依赖门控待执行
-> 前置计划完成后由执行角色复核实际接口
-> 接口一致才执行；接口漂移则退回修订
```

A dependency-gated plan must separate verified current facts from assumed interface contracts. It must list prerequisite plan / queue ids, expected formal artifacts, execution-time interface checks, and the revision path for drift. Never describe an expected artifact as current code fact.

When registering a plan, synchronously verify its corresponding flowchart and detailed design. The plan, plan index, project memory, and generation breakpoint must list the flowchart path, detailed-design path, and plan path, and must state that their scopes agree. If the flowchart or detailed design is missing, withdrawn, still has a question without a default ruling, or does not cover the plan scope, do not register the queue item. For pure rule, read-only scan, information-data, breakpoint, or project-memory plans with no flowchart/detailed-design source, explicitly record why the linkage is not applicable and what formal source replaces it.

## Output Location

- New complete plans go to `计划/` and are registered as waiting execution.
- Completed plans move to `计划/已完成计划/`.

Recommended filenames:

```text
计划/YYYYMMDD_FSxx_主题专项_v0.1.md
计划/YYYYMMDD_FSxx_主题代码实施切片_v0.1.md
```

## Required Plan Sections

Include these sections when applicable:

```text
状态行
依据
实施范围
明确排除项
目标服务 / 目标结构
允许文件
禁止文件
实施步骤 S0/S1/...
验收方式
完成声明边界
默认裁决 / 已知风险
```

For code implementation slices, allowed files, forbidden files, and validation commands are mandatory.

For slices that create code files, a new-file classification table is mandatory. A pre-existing plan that conflicts with the current code-file specification must be revised before execution; do not rely on its age as an exception.

For dependency-gated code slices, these sections are also mandatory:

```text
依赖门控状态
已验证当前事实
假定接口契约
前置计划 / 队列编号
预期正式产物
执行前接口复核
接口漂移退回规则
```

## Service-Level Migration Rules

- Do not write `旧函数 A -> 新函数 A`.
- Use old functions only as evidence anchors.
- Use service logic packages, service plans, or service implementation slices as the migration unit.
- Exclude SQL/control-panel/display mirror, D455, voxel, peripherals, old linked-list containers, and old main-info fields unless a later registered plan explicitly allows them.

## Plan Index And Queue

- Editing `计划/计划索引.md` does not require `计划/.计划索引.lock`.
- Protect concurrent work through Git facts: check `git status --short` before editing, inspect targeted diffs after editing, stage only the current slice's files, and never overwrite unrelated dirty changes.
- When a plan is generated, keep it under `计划/`, update references, and register the next waiting executable item in `项目记忆/Codex任务队列.md`.
- A plan whose prerequisites are incomplete is registered as `依赖门控待执行`; plan generation does not claim that its dependencies or assumed interfaces already exist.
- Before queue registration, verify the plan's corresponding flowchart and detailed design paths, scope, and default rulings, or explicitly state why the linkage is not applicable.
- When the plan is completed, move it to `计划/已完成计划/` and update references.

## Cross-Role Handoff

Use `规范/设计执行双窗口交互规范.md` when an execution role returns a plan for design revision.

After revising a returned plan or preparing an execution, integration, or read-only handoff:

1. Update the flowchart, detailed design, plan, plan index, task queue, project memory, and `项目记忆/窗口交互记录.md`.
2. Keep the original queue id unless the revision creates a genuinely independent prerequisite plan.
3. Validate, commit, and push before sending any task message.
4. If the target role is in another physical window and Codex task tools are available, locate the unique same-repo task by task id, title, cwd, registered worktree, and role; send a message containing the queue / integration id, commit, authoritative paths, allowed actions, forbidden actions, validation, ownership release, and remaining gates.
5. If the target execution role is in the current physical window, do not send a message to self. After the interaction record containing the target role has been committed and pushed in step 3, switch from that published record and re-read Git, the plan index, queue, target plan, breakpoint, and actual interfaces before execution.
6. Treat any task message as a wake-up signal only. The target role must re-read repository facts before action.
7. After sending, read back the target task state. Only a uniquely delivered task may be marked `已派发待执行回执` or `已派发待集成回执`; a reservation remains pending.
8. If an external target task is missing, ambiguous, or messaging fails, leave the handoff as pending in `项目记忆/窗口交互记录.md`; do not broaden authority or claim execution / integration has started.

For a task-worktree handoff, also record the worktree id, absolute path, `codex/*` branch, frozen base, batch id, file ownership, and integration order. A task branch completion is `分支完成待集成`, not plan completion.

For an integration handoff, inspect the target path and branch before publication. If either already exists with a different HEAD, parent, owner, start commit, or reflog, mark the old candidate invalid and allocate a new id, path, and branch. Never instruct the integration role to reset, delete, retarget, or reuse the mismatched candidate. Once the integration role enters `集成中`, the design role must keep WT-MAIN clean and must not commit or push until integration releases the main publication lease.

For a read-only review handoff, specify the exact commit, worktree, review object, questions, and evidence sources. The reviewer only returns facts, differences, evidence, risks, and suggestions; the design role owns persistence and final disposition.

## Git Worktree Protection

- Planning and central governance edits occur only in the registered `main` integration worktree.
- Read `项目记忆/并行工作树登记表.md` before creating, assigning, invalidating, integrating, or reclaiming a task worktree.
- Before editing shared governance files, check `git status --short` and identify unrelated dirty or staged files.
- After editing, inspect targeted diffs and stage only the current slice's files.
- If another window has already changed the same target lines and the merge is ambiguous, stop and record the blocker instead of overwriting.

## Verification

For planning slices:

```powershell
git diff --check
if (Test-Path .\tools\check_specs.py) { python .\tools\check_specs.py }
```

Do not run C++ build unless the planning slice also includes code changes.
