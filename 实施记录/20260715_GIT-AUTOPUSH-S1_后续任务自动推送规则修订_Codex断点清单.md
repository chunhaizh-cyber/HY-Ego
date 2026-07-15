# GIT-AUTOPUSH-S1 后续任务自动推送规则修订断点清单

日期：2026-07-15

状态：JY-343 / 用户持续授权已落实 / 纯治理任务 / 未修改生产 C++ / 完成后自动提交并自动非强制推送

## 1. 输入入口

用户明确指令：`后续自动推送`。

前置规则为 JY-339：每轮任务完成并验证后自动本地提交，但推送逐轮授权。本轮以用户新指令覆盖其推送门禁，保留自动提交的范围隔离和验证要求。

## 2. 核心处理

1. 每轮有文件改动的任务只有在授权范围完成并通过要求验证后，才自动收尾。
2. 只暂存本轮文件，自动本地提交，随后自动执行非强制推送。
3. 正式迁移任务未另行指定目标时，默认快进发布到 `origin/main`。
4. 任务分支只有在 `origin/main` 是当前提交祖先时，才允许使用 `git push origin HEAD:main`。
5. 目标不明、远端分叉、detached HEAD、无关改动无法隔离、验证失败或推送失败时停止并报告。
6. 持续授权不包含 force、自动合并、自动变基、自动删除分支、自动创建 PR 或自动发布制品。

## 3. 结构承载

- `AGENTS.md`
- `规范/000_项目规则总纲.md`
- `规范/001_规则迁移清单.md`
- `规范/设计执行双窗口交互规范.md`
- `.codex/skills/hai-zhong-yu-chao-execute/SKILL.md`
- `计划/计划索引.md`
- `项目记忆/Codex任务队列.md`
- `项目记忆/当前状态.md`
- `项目记忆/决策记录.md`
- `项目记忆/验收记录.md`
- `项目记忆/窗口交互记录.md`

## 4. 输出变化

后续项目任务不再因缺少逐轮“推送”指令停在本地提交。自动推送成功后才能发送正式跨窗口交接消息；自动推送失败时保留本地提交并报告真实远端状态。

本轮规则提交完成后，把 E51-E55 的本地提交作为同一快进链发布到 `origin/main`，使 #271 / DQ-163 成为远端正式待执行项。

## 5. 验证方式

```powershell
python .\tools\check_specs.py --strict
git diff --check
git diff --cached --check
git merge-base --is-ancestor origin/main HEAD
git push origin HEAD:main
git rev-list --left-right --count origin/main...HEAD
git status --short --branch
```

## 6. 禁止扩大声明

本轮只修订 Git 收尾和跨窗口发布规则，不修改 C++、工程、业务计划范围或队列执行状态，不证明 #271、#224、#225 已实施，也不证明项目迁移完成。
