# 20260707 ChatGPT 同步能力迁移 Codex 断点清单

状态：已完成 / 技能治理 / 文档治理 / 未改 C++ / 未迁移旧函数

## 1. 输入入口

用户要求：

```text
将鱼巣中用于和chatgpt同步的能力迁移过来
检查代码现状和已执行计划,并比较是否一致
修正
```

## 2. 已读证据

旧项目证据：

```text
D:\鱼巢\AGENTS.md
D:\鱼巢\.codex\skills\chatgpt-yu-chao-theory\SKILL.md
D:\鱼巢\.codex\skills\chatgpt-yu-chao-theory\agents\openai.yaml
```

当前项目证据：

```text
AGENTS.md
计划/计划索引.md
规范/000_项目规则总纲.md
规范/001_规则迁移清单.md
项目记忆/当前状态.md
项目记忆/Codex任务队列.md
实施记录/20260707_海中鱼巣本地技能转换_Codex断点清单.md
```

## 3. 修正内容

| 文件 | 变更 |
| --- | --- |
| `.codex/skills/chatgpt-hai-zhong-yu-chao-theory/SKILL.md` | 新增海中鱼巣 ChatGPT 侧理论探讨同步技能。 |
| `.codex/skills/chatgpt-hai-zhong-yu-chao-theory/agents/openai.yaml` | 新增 ChatGPT 侧 agent 入口。 |
| `AGENTS.md` | 新增 ChatGPT 理论探讨同步与状态回写规则。 |
| `计划/计划索引.md` | 登记 ChatGPT 同步能力已迁移。 |
| `项目记忆/当前状态.md` | 新增 ChatGPT 理论探讨同步技能状态，并修正 #100-#104 已完成后的过期下一步描述。 |
| `项目记忆/Codex任务队列.md` | 登记本次修正为 #105 已完成。 |
| `项目记忆/决策记录.md` | 记录 ChatGPT 同步采用项目记忆层的决策。 |
| `项目记忆/验收记录.md` | 记录本次技能治理验收边界。 |

## 4. 关键迁移口径

```text
旧鱼巢 docs/current-state.md -> 海中鱼巣 项目记忆/当前状态.md
旧鱼巢 docs/decision-log.md -> 海中鱼巣 项目记忆/决策记录.md
ChatGPT 输出只作为审查、修正、映射、同步摘要或草稿。
不得把 ChatGPT 理论讨论、同步摘要或草稿直接当作当前仓库事实。
不得宣称未发生的本地执行、构建、运行、提交、推送或旧能力迁移。
```

## 5. 状态一致性修正

修正前：

```text
项目记忆/当前状态.md 仍称 FS-10、FS-03 第二轮、FS-02 后续、基础信息服务分层完整增强和旧能力迁移服务包状态复核需由执行窗口继续执行。
```

修正后：

```text
可执行队列 #100-#104 已完成。
当前可执行队列清空。
FS-04 至 FS-08 后续 S1-S4 代码实施切片和 FS-09 后续详细设计 / 快照计划仍在 计划/待确认计划/，确认前不得执行。
```

## 6. 验证

预期验证：

```text
git diff --check
轻量技能校验：SKILL.md、frontmatter、name、description、agents/openai.yaml interface
```

本切片未修改 C++，不运行构建，不运行默认入口。

## 7. 禁止扩大声明

本次只证明 ChatGPT 理论探讨同步能力已作为技能和协作规则迁移到海中鱼巣，并修正项目记忆的过期状态。

不得宣称：

```text
鱼巢旧能力已迁移完成
ChatGPT 同步摘要可替代当前代码事实
代码已编译
入口前置拒绝通过
最小闭环通过
旧能力迁移通过
真实外设样本通过
```
