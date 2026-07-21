---
name: hai-zhong-yu-chao-current-fact-scan
description: Use in the 海中鱼巣 repository when the user says "按当前代码检查", "按当前 worktree 检查", "先扫描本地事实", "先确认当前入口和调用点", "S0 当前代码事实扫描", or asks Codex to confirm current local code facts before planning or editing. This skill is strictly read-only.
---

# 海中鱼巣当前事实扫描

## 角色与入口

严格服从仓库根目录 `AGENTS.md`。用 `git rev-parse --show-toplevel` 解析仓库根目录，并从正式 worktree 登记确认当前路径、分支、角色、基线和所有者；不得硬编码仓库路径、主分支名或工作树身份。身份无法确认时停止在只读事实报告，不推测补齐。

本技能只完成 S0 / 当前本地事实扫描：

```text
当前目标
-> 当前 worktree 与 dirty state
-> 当前规则 / 计划 / 队列
-> 当前真实代码入口、调用点、结构承载
-> 缺口、风险、待确认问题
```

## 专用步骤

1. 解析 Git 顶层，读取 `AGENTS.md`。
2. 按扫描对象限载读取：

```text
计划/计划索引.md
规范/规范目录.md
项目记忆/当前状态.md
项目记忆/Codex任务队列.md
项目记忆/并行工作树登记表.md
目标正式规范、有效设计、现行计划、任务文件头部规则
```

3. 读取当前 Git 证据：

```powershell
git status --short --branch --untracked-files=no
git diff --name-only
git rev-parse --show-toplevel
git branch --show-current
git worktree list --porcelain
```

4. 用范围受限的 `rg` 和短文件读取定位真实入口、调用者、结构承载、写入方、读取方、拒绝、返回和验证证据。
5. 核对 dirty diff 是否影响所读事实；不得把不同 worktree 的事实混合为同一结论。
6. 将结果分为 `事实 / 差异 / 证据 / 风险 / 建议`；不能由当前证据证明的内容标记为待核，不用计划目标替代代码事实。

## 输出

按需返回：

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

正式只读复核派发还必须具名：

```text
目标提交与 worktree
复核对象与问题清单
证据入口
报告接收的设计任务
```

## 停止边界

- 本技能没有写断点或项目记忆的例外：不得修改任何文件、构建、运行程序、暂存、提交、推送、入队、解除依赖、裁决完成或取得执行 / 发布占用。
- 用户要求持久化扫描结果时，将事实报告交给正式设计角色或另行登记的纯文档任务；不得在本技能内切换角色。
- 不从本地状态推断云端状态，不把日志、控制台或显示文本当机器事实。
- 没有当前调用路径、结构承载和验证证据时，不得宣称 `已实现`、`已接通` 或 `已完成`。
