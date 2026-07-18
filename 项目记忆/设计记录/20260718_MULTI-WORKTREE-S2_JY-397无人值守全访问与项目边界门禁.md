# MULTI-WORKTREE-S2 JY-397 无人值守全访问与项目边界门禁

日期：2026-07-18

关联计划：#296 / DQ-188 / WB-296-01

状态：平台规则已修订 / 等待 Codex 桌面重启 / #296 断点仍未提交

## 1. 当前事实

1. `unelevated` 默认沙箱已解决 WT-296 的 1312，常规读取和 S0 复核正常。
2. 多 worktree 的 Git 元数据位于主仓库 `.git/worktrees`，默认沙箱不能创建任务 worktree 的 `index.lock`。
3. `approval_policy = "on-request"` 与 `approvals_reviewer = "auto_review"` 没有形成无人值守：自动审查拒绝 Git 收口，Codex 桌面仍向用户显示 Command approval 弹窗。
4. 用户明确拒绝重复人工授权。WT-296 仍只有一份未跟踪执行断点，C++、工程、暂存区、提交和远端任务分支均无新增变化。

## 2. 裁决

1. 当前 Windows 多 worktree 无人值守执行改用 `sandbox_mode = "danger-full-access"` 与 `approval_policy = "never"`；撤销 JY-396 的 `auto_review` 路径。
2. Full access 只取消操作系统沙箱和人工审批，不合并项目角色、不扩大计划允许文件、不改变任务分支与 `origin/main` 权限。
3. 每个任务仍必须先登记 worktree、分支、冻结基线、计划、文件 / 结构 / 接口所有权和验证矩阵；执行前后机械核对 `git status`、允许范围和 staged diff。
4. 删除、移动、force / force-with-lease、任务窗口写主工作树、任务分支推送 `origin/main`、未登记合并 / 变基 / cherry-pick 继续禁止。
5. 重启 Codex 桌面后，WT-296 只完成现有断点的精确暂存、提交和同名任务分支推送，不继续代码实施。

## 3. 完成边界

本轮只修订多 worktree 平台权限口径。不得声明 #296 断点已发布、接口漂移已修订、代码已实施、阶段 890 已登记或任何恢复能力完成。
