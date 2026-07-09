# Codex 跨机同步清单

更新时间：2026-07-09

## 1. 结论

当前 `D:\海中鱼巣` 项目内需要通过 Git 云端同步的项目资料已同步到 `origin/main`。

本次核对结果：

```text
git status --short --branch：main 与 origin/main 对齐，工作区无普通未跟踪文件。
git ls-files -o --exclude-standard：无输出，说明没有未跟踪且未忽略的项目文件。
git ls-files -o -i --exclude-standard：仅命中本机缓存、构建产物、运行日志和个人配置。
git rev-list --left-right --count main...origin/main：0 0。
```

## 2. 已随项目仓库同步的资料

另一台电脑克隆 `https://github.com/chunhaizh-cyber/HY-Ego.git` 的 `main` 分支后，可获得以下项目资料：

```text
AGENTS.md
.codex/skills/*
规范/*
规范/详细设计/*
规范/验收/*
计划/*
计划/已完成计划/*
计划/待确认计划/ 目录状态
项目记忆/*
实施记录/*
流程图/*
说明书/*
tools/check_specs.py
海中鱼巣/*.h / *.cpp 等当前项目代码
海中鱼巣.vcxproj
海中鱼巣.vcxproj.filters
海中鱼巣.slnx
历史对话记录/*
```

## 3. 当前没有发现需要补加到 Git 的项目资料

以下核对项未发现遗漏：

```text
项目内 Codex 技能：已跟踪 `.codex/skills/*/SKILL.md` 与 `agents/openai.yaml`。
项目记忆：已跟踪当前状态、决策记录、Codex 任务队列、待确认问题、验收记录和实施记录索引。
计划材料：已跟踪根计划、已完成计划、当前已确认计划和计划索引。
规范 / 详细设计 / 流程图 / 实施记录：已跟踪。
历史对话记录：当前已有文件已跟踪。
普通未跟踪文件：无。
```

## 4. 明确不随项目仓库同步的本机资料

以下内容存在于本机，但不应直接提交到项目仓库：

| 本机内容 | 当前状态 | 不同步原因 |
| --- | --- | --- |
| `C:\Users\zhchh\.codex\auth.json` | 未同步 | 认证信息，属于敏感材料。 |
| `C:\Users\zhchh\.codex\config.toml` 和备份 | 未同步 | 个人 Codex 客户端配置，可能包含本机路径、模型或服务偏好。 |
| `C:\Users\zhchh\.codex\sessions`、`archived_sessions`、`session_index.jsonl` | 未同步 | 本机会话历史和索引，不是项目正式事实来源。 |
| `C:\Users\zhchh\.codex\memories`、`memories_1.sqlite*` | 未同步 | Codex 全局记忆和派生摘要，可能混合多个项目；项目内长期事实已落到 `项目记忆/`。 |
| `C:\Users\zhchh\.codex\logs_2.sqlite*`、`state_5.sqlite*`、`goals_1.sqlite*` | 未同步 | 本机运行状态、日志和目标数据库，不应作为项目事实。 |
| `C:\Users\zhchh\.codex\plugins`、`cache`、`vendor_imports`、`node_repl` | 未同步 | 插件 / 运行缓存，可由目标机器环境重新安装或生成。 |
| `C:\Users\zhchh\.codex\attachments`、`generated_images`、`tmp`、`.tmp` | 未同步 | 附件、生成物和临时缓存，需按具体文件另行判断。 |
| `D:\海中鱼巣\.vs` | Git 忽略 | Visual Studio 本机缓存。 |
| `D:\海中鱼巣\x64`、`D:\海中鱼巣\海中鱼巣\x64` | Git 忽略 | 构建输出，可重建。 |
| `D:\海中鱼巣\海中鱼巣.vcxproj.user` | Git 忽略 | Visual Studio 用户本机配置。 |
| `D:\海中鱼巣\日志\*.log` | Git 忽略 | 运行日志 / 事件日志，只做人读诊断，不承载机器事实。 |

## 5. 另一台电脑恢复建议

```text
1. 安装 Codex / Git / Visual Studio / Python 等基础环境。
2. 克隆 `https://github.com/chunhaizh-cyber/HY-Ego.git`。
3. 打开仓库后先读取 AGENTS.md、计划/计划索引.md、规范/000_项目规则总纲.md、规范/001_规则迁移清单.md。
4. 使用项目内 `.codex/skills/` 作为海中鱼巣专用技能来源。
5. 不从本机全局 Codex 会话或记忆数据库推断项目事实；以仓库内 `项目记忆/`、`计划/`、`规范/`、`实施记录/` 和当前代码为准。
6. 如确实需要迁移全局 Codex 认证、配置、历史会话或全局记忆，必须通过安全的个人配置迁移流程单独处理，不提交到本项目仓库。
```

## 6. 完成声明边界

本清单只能证明：

```text
项目仓库内可同步资料已核对，当前没有未跟踪且应纳入 Git 的项目资料。
本机 Codex 全局状态、认证、会话、插件缓存和运行数据库已明确列为不随项目仓库同步。
```

不得声明：

```text
另一台电脑已经完成 Codex 登录。
全局 Codex 会话和记忆已迁移。
插件、浏览器状态、自动化状态或本机缓存已恢复。
C++ 已重新构建或运行。
```
