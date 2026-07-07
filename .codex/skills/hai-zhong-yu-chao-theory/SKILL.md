---
name: hai-zhong-yu-chao-theory
description: Use when analyzing, correcting, or drafting theory/spec/plan text for the 海中鱼巣 rebuild, especially around migration governance, project口径, service boundaries, demand/task/method concepts, state/dynamic/causal rules, and contradictions between user theory and current local/cloud code evidence.
---

# 海中鱼巣理论与规范草稿

## Core Meaning

Use this for reasoning and drafting, not execution:

```text
用户口径 / 上传材料 / 当前项目证据
-> 分离事实、假设、定义、约束
-> 找冲突和缺口
-> 形成可写入规范 / 详细设计 / 计划的保守草稿
```

Do not claim code, build, runtime validation, cloud sync, or migration completion unless that evidence is explicitly available.

## Authority Order

```text
用户本轮明确硬规则
-> AGENTS.md
-> 规范
-> 详细设计
-> 计划 / 计划索引
-> 当前代码事实 / commit facts
-> 实施记录 / 断点清单
-> README / 说明书 / 日志 / UI / 总结
-> 推断 / 建议
```

## Hard Project Rules

Always preserve:

```text
函数名、变量名、类内业务成员名必须用中文。
机器逻辑必须落到节点、主信息、关系、索引、特征、特征值、需求、任务、方法、状态、动态或因果引用。
日志、控制台、注释、说明文档、临时文本不得承载机器逻辑。
业务写入必须经领域服务。
特征值服务只由特征服务直接访问。
需求目标是目标状态，不是 I64。
线程不是动作来源。
函数事实是证据采集单位，服务逻辑包是迁移确认单位。
第一版不接控制面板 / SQL / D455 / 体素 / 外设。
```

## Review Workflow

1. Fix the object:

```text
对象 = 规范 / 详细设计 / 计划 / 逻辑迁移包 / 当前状态 / 用户口径 / 冲突集合
```

2. Split:

```text
事实
假设
定义
约束
```

3. Find stale or inflated claims:

```text
把设计说成实现
把 S0 扫描说成代码完成
把旧函数编号当迁移单位
把日志/显示/说明文字当机器事实
把外设/SQL/控制面板提前接入
```

4. For any surviving proposal, provide:

```text
业务概念 -> 项目承载结构 -> 值类型 / 身份结构 -> 写入方 -> 读取方 -> 验证方式
```

If any slot is missing, mark as `缺结构`、`缺入口`、`待核` or `需确认`.

## Output Style

```text
我的判断：
主要问题：
建议口径：
结构映射：
待确认：
下一步：
```

When drafting document text, mark it as draft unless the user explicitly confirms it into the project.
