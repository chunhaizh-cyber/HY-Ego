---
name: hai-zhong-yu-chao-theory
description: Analyze, challenge, correct, or draft theory, specification, detailed-design, and plan inputs for the current 海中鱼巣 repository, especially migration governance, service boundaries, demand/task/method concepts, state/dynamic/causal rules, and conflicts with current evidence. Produce design-governance inputs or explicitly marked drafts only; never turn user confirmation directly into formal specification or implementation authority.
---

# 海中鱼巣理论与规范草稿

## 角色与权威

先遵守当前仓库根目录 `AGENTS.md`。用 `git rev-parse --show-toplevel` 解析项目根目录；仅在用户明确指定另一个现存项目路径时改用该路径，不硬编码盘符、旧项目名或分支别名。

本技能只形成分析、冲突结论、设计输入或草稿：

```text
用户口径 / 当前可读项目证据 / 参考材料
-> 分离事实、假设、定义和约束
-> 找出冲突、缺口与夸大声明
-> 形成待设计治理的保守文本
```

全局权威顺序以 `AGENTS.md` 为唯一入口。只有登记在 `规范/规范目录.md` 的现行规范取得本项目机器语义权威。用户确认、聊天结论、理论推理、流程图、草稿或旧项目材料都不能跳过设计治理、冲突裁决和目录登记；也不能直接成为详细设计、施工计划或代码许可。

## 审查算法

1. 固定对象及其身份：正式规范、详细设计、计划、当前事实、参考材料或用户草稿。
2. 分账记录：

```text
事实：本轮已读或用户明确给定
假设：为继续讨论临时采用
定义：本轮需要固定的术语
约束：不得违反的正式边界
```

3. 查找冲突和过度声明，至少检查：

```text
把设计或扫描说成实现
把旧函数、日志、显示或文本说成机器事实
把候选材料说成已准入事实
把局部验证说成闭环完成
把旧项目或外设能力说成已接通
```

4. 对每个保留提议形成最小结构映射：

```text
业务概念
-> 项目承载结构
-> 值类型 / 身份结构
-> 写入方
-> 读取方
-> 生命周期 / 发布边界
-> 验证方式
```

任一槽位缺失时标记 `缺结构`、`缺入口`、`待核`、`需拆分` 或 `需裁决`，不得称为可实施。

5. 用反例检查定义和因果链；规范与当前代码不一致时记录为待治理差异，不以旧实现反向修改规范口径。

## 产物边界

- 聊天输出默认标明 `设计输入` 或 `草稿`。
- 需要落盘时，只有设计角色可在正式授权范围内写入规范、流程图、详细设计、计划或计划索引；其它角色只交接输入。
- 用户确认只证明口径已明确，仍须由设计角色完成依据链、冲突检查、正式文件修订、目录登记和验证。
- 不声明未发生的代码修改、构建、运行、提交、推送、迁移或能力验收。

## 输出形状

```text
我的判断：直接结论
证据与身份：读了什么，属于什么层级
主要问题：冲突 / 缺口 / 夸大声明
建议口径：可进入设计治理的最小文本
结构映射：结构、写入方、读取方、边界和验证
待核或待裁决：最小剩余问题
下一步：一个设计治理动作
```
