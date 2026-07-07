---
name: hai-zhong-yu-chao-discuss
description: Use in D:\海中鱼巣 when the user says "讨论", "请讨论", "一起讨论", "挑剔", "专家审查", "帮我找问题", or asks Codex to review a spec, plan, detailed design, migration package, service boundary, or implementation direction without immediately editing code.
---

# 海中鱼巣讨论

## Core Meaning

Treat discussion as expert review, not implementation authorization:

```text
用户观点
-> 拆成判断、前提、待裁决问题
-> 对照 AGENTS、规范、详细设计、计划索引和当前代码事实
-> 标出可保留、需修改、需拆分、需确认、应排除
-> 给出最小可落地口径
```

## Context

For `D:\海中鱼巣`, read as needed:

```text
AGENTS.md
计划/计划索引.md
规范/000_项目规则总纲.md
规范/001_规则迁移清单.md
项目记忆/当前状态.md
项目记忆/待确认问题.md
```

Use current evidence labels:

```text
已读证据
代码事实
规范事实
计划事实
用户口径
合理推断
待核
```

## Review Standard

Check:

```text
概念是否已定义
能否映射到节点 / 主信息 / 关系 / 索引 / 特征 / 特征值 / 需求 / 任务 / 方法 / 状态 / 动态 / 因果引用
写入方、读取方、生命周期是否明确
是否绕过领域服务
是否把日志 / 控制台 / 显示 / 注释当机器逻辑
是否把旧函数当迁移单位
是否需要用户确认
```

## Output Shape

Prefer:

```text
我的判断：
关键拆分：
建议口径：
结构映射：
还缺什么：
```

For findings:

```text
原观点：
问题：
依据：
落地改法：
结构映射：
反例：
验证方式：
```

## Hard Rules

- Do not implement during discussion unless the user explicitly asks to modify files.
- Do not override formal specs silently.
- Do not expand project scope to make an idea work.
- Do not say "可以" unless writer, reader, structure, and validation path are clear.
- Do not declare self-loop, awakening, old capability migration, SQL/control-panel/peripheral integration, or full system completion from design discussion.
