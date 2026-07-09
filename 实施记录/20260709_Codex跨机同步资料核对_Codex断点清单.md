# Codex 跨机同步资料核对 Codex 断点清单

日期：2026-07-09

状态：已完成文档核对 / 已同步项目内清单 / 未改 C++ / 未构建 / 未迁移旧函数

## 1. 输入入口

用户询问：若要将本机 Codex 所有信息和项目数据通过云端同步到另一台电脑，当前还差哪些信息没同步到云端；没有的请添加并同步一次。

## 2. 核对范围

```text
D:\海中鱼巣 项目仓库 Git 状态
项目内 .codex/skills
项目记忆、计划、规范、流程图、实施记录、说明书、历史对话记录
.gitignore 与被忽略文件
C:\Users\zhchh\.codex 顶层目录名称
C:\Users\zhchh\.codex\memories 顶层目录名称
```

本次只读取全局 Codex 目录的顶层清单，不读取认证内容、会话内容或数据库内容。

## 3. 核对结果

```text
git status --short --branch：main 与 origin/main 对齐。
git ls-files -o --exclude-standard：无输出。
git ls-files -o -i --exclude-standard：仅命中 .vs、x64、*.user、*.log、构建中间文件和运行日志。
git ls-files '.codex/**' '历史对话记录/**'：项目内 Codex 技能和历史对话记录已跟踪。
git rev-list --left-right --count main...origin/main：0 0。
```

## 4. 输出变化

新增：

```text
项目记忆/Codex跨机同步清单.md
```

同步：

```text
项目记忆/当前状态.md
项目记忆/决策记录.md
项目记忆/验收记录.md
实施记录/20260709_Codex跨机同步资料核对_Codex断点清单.md
```

## 5. 未同步但不应提交的内容

```text
C:\Users\zhchh\.codex\auth.json
C:\Users\zhchh\.codex\config.toml
C:\Users\zhchh\.codex\sessions
C:\Users\zhchh\.codex\archived_sessions
C:\Users\zhchh\.codex\memories
C:\Users\zhchh\.codex\*.sqlite*
C:\Users\zhchh\.codex\plugins
C:\Users\zhchh\.codex\cache
D:\海中鱼巣\.vs
D:\海中鱼巣\x64
D:\海中鱼巣\海中鱼巣\x64
D:\海中鱼巣\海中鱼巣.vcxproj.user
D:\海中鱼巣\日志\*.log
```

以上内容属于认证、个人配置、全局会话 / 记忆、运行状态、插件缓存、本机 IDE 缓存、构建产物或运行日志，不作为项目正式事实同步到 Git 云端。

## 6. 验证命令

```powershell
git diff --check
python .\tools\check_specs.py
git status --short --branch
git rev-list --left-right --count main...origin/main
```

## 7. 完成声明边界

本次只能声明：

```text
项目内需要随仓库云端同步的 Codex 技能、项目记忆、计划、规范、流程图、实施记录、说明书、历史对话记录和代码资料已核对并记录；当前没有发现应纳入 Git 但未跟踪的项目资料。
```

不得声明：

```text
全局 Codex 认证、会话、插件缓存、运行数据库或个人配置已迁移。
另一台电脑已完成 Codex 登录或插件恢复。
C++ 已重新构建或运行。
```
