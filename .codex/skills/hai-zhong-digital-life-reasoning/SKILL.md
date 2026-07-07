---
name: hai-zhong-digital-life-reasoning
description: Use when the user says "推理", "逻辑推理", "请推理", "梳理逻辑", or asks to reason about digital-life concepts as they should map into the D:\海中鱼巣 rebuild, including self/need/task/method, service boundaries, state/dynamic/causal chains, and implementation constraints.
---

# 海中鱼巣数字生命推理

## Core Rule

Reason from theory into Sea Nest project structures:

```text
概念 / 机制 / 因果链
-> 前提、定义、约束
-> 可验证推理链
-> 海中鱼巣结构映射
-> 规范 / 详细设计 / 计划落点
```

Do not turn theory conclusions into implementation facts without code and validation evidence.

## Source Priority

```text
1. User-stated rules in the current conversation.
2. D:\海中鱼巣 AGENTS.md, 规范, 计划, 项目记忆.
3. Existing files under D:\数字生命思路 when explicitly relevant.
4. Read-only reference material under D:\鱼巢.
5. External web only for current public facts.
```

## Reasoning Workflow

1. Define the object:

```text
对象 = 概念 / 机制 / 因果链 / 实施方案 / 边界 / 反例 / 评价标准
```

2. Separate:

```text
事实
假设
定义
约束
```

3. Build the chain:

```text
前提 P
约束 C
=> 中间结论 M
=> 可验证后果 V
=> 结论 K
```

4. Map to Sea Nest:

```text
概念 -> 节点/关系/索引/特征/状态/动态/需求/任务/方法/因果引用 -> 写入方 -> 读取方 -> 验证方式
```

5. Test counterexamples.

## Durable Output

If the user asks to save the aligned reasoning, write under the current project directory unless they specify another path:

```text
推理_<主题>_YYYYMMDD.md
方案_<主题>_YYYYMMDD.md
```

For Sea Nest project-governance conclusions, prefer landing as draft material and then route through:

```text
规范 -> 详细设计 -> 计划 -> 实施记录 -> 验证
```

## Boundaries

- Do not modify `D:\鱼巢` or `D:\数字生命思路` unless explicitly asked.
- Do not claim old capability migration or self-loop completion from reasoning.
- Do not use text, logs, or display as machine logic.
- Do not bypass service-level migration confirmation.
