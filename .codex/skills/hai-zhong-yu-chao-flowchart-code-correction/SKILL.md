---
name: hai-zhong-yu-chao-flowchart-code-correction
description: Use in D:\海中鱼巣 when the user asks to 根据代码画流程图, 检查流程图, 根据流程图优化代码, 流程图驱动代码纠偏, 生成修订计划, or 将修订计划加入计划索引. This skill turns current code into a current-state flowchart, contract-context review, non-success return classification, deviation list, and a registered correction plan.
---

# 海中鱼巣流程图驱动代码纠偏

## Core Meaning

Treat flowcharts as a code-review and correction tool:

```text
当前代码
-> 现状流程图
-> 逐行代码映射表
-> 输入契约 / 调用语境表
-> 非成功返回二分审查
-> 偏差清单
-> 修订计划
-> 计划索引和 Codex 任务队列登记
```

This skill does not directly edit C++ unless the user separately asks to execute a registered code plan.

## Required Context

Before modifying project files, read:

```text
计划/计划索引.md
规范/000_项目规则总纲.md
规范/001_规则迁移清单.md
规范/现状流程图与施工流程图区分规范.md
```

When deriving from code, read the relevant source files and their file-head rules. Do not rely on chat memory for code facts.

## Output Package

For each target, create or verify:

```text
流程图/YYYYMMDD_<主题>现状流程图_v0.1.md
流程图/YYYYMMDD_<主题>现状流程图_v0.1.html
实施记录/YYYYMMDD_<主题>逐行代码映射表.md
实施记录/YYYYMMDD_<主题>输入契约与调用语境表.md
实施记录/YYYYMMDD_<主题>非成功返回二分审查表.md
实施记录/YYYYMMDD_<主题>现状施工偏差清单.md
计划/YYYYMMDD_<主题>代码纠偏或文档修订计划_v0.1.md
```

If the target only needs documentation correction, the plan may be a document-governance plan. If it changes C++ or machine structures, the plan must include allowed files, forbidden files, entry rejection rules, failure closure, and validation commands.

## Workflow

1. Confirm the target scope: files, functions, old construction flowchart, detailed design, and completed plan records.
2. Generate the current-state flowchart from current code, not from intended design.
3. Generate the line-by-line code mapping table. Cover every non-empty effective line; pure braces may be included in adjacent ranges.
4. Generate the input contract / call-context table.
5. Review every mid-flow non-success return using the contract table.
6. Generate the deviation list, splitting findings into:

```text
修图
修详细设计
修代码
补规范或补契约
```

7. Generate a correction plan.
8. If the plan is complete and executable, add it to `计划/计划索引.md` and `项目记忆/Codex任务队列.md`. If dependencies are missing, register it as dependency-gated. If it is only an upstream document package and the user explicitly says not to index it, record that boundary.
9. Run mechanical validation:

```powershell
py -3 .\tools\check_flowchart_correction.py --strict `
  --flowchart-md <md> `
  --flowchart-html <html> `
  --mapping <mapping> `
  --contract <contract> `
  --non-success <non_success> `
  --deviation <deviation> `
  --plan <plan> `
  --require-plan-registration
```

Use `--help` for optional flags.

## Contract Review Rule

Never classify a non-success return as `逻辑内返回` only because the structure did not change.

For each non-success return, ask:

```text
Is the input external/test/candidate material?
Is the input guaranteed valid by an upstream formal flow?
Has the flow entered creation, binding, relation write, state write, dynamic recording, settlement, readback, or structure carriage?
Is this return explicitly allowed by the design?
```

Classification:

```text
逻辑内返回:
  External/test/candidate/request material is rejected or empty, the design allows the branch, and no readable partial structure is produced.

追根因解决:
  The upstream formal flow already promised the value was valid, or internal structure carriage/readback has begun and the result is not as expected.
```

If one code branch can be reached by both contexts, split the diagram node and table row into:

```text
外部材料语境：逻辑内返回
正式流程契约语境：追根因解决
```

## Required Tables

Input contract / call-context table:

```text
入口
调用方
输入字段
输入来源
上游是否保证有效
允许逻辑内返回
追根因触发条件
结构变化
验证方式
```

Non-success return review table:

```text
节点
代码位置
返回条件
输入语境
是否上游保证有效
结构是否变化
设计是否允许
二分口径
追根因对象或逻辑返回理由
计划动作
```

Deviation list:

```text
编号
现状代码事实
流程图 / 详细设计 / 计划描述
偏差类型
纠偏方向
是否需要代码计划
```

## Registration Rule

When a correction plan changes code, machine structure, or formal project state, it must be:

```text
1. Written under 计划/
2. Referenced in 计划/计划索引.md
3. Registered in 项目记忆/Codex任务队列.md
4. Linked back to the current-state flowchart, contract table, non-success review, and deviation list
```

Do not register a plan if the flowchart, contract table, non-success review, or deviation list is missing or fails mechanical validation.

## Boundaries

Do not claim:

```text
代码已纠偏
全部流程图已校正
真实闭环、自我循环、自我苏醒完成
旧项目能力已迁移
SQL / 控制面板 / D455 / 体素 / 真实外设已接入
```

unless a later executed code plan proves it with the required validation level.
