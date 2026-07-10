# DB-PANEL-S1 SQL Server 结构审计投影与控制面板查看代码实施切片 v0.1

日期：2026-07-10

状态：已完成 / 已验证 / 已归档计划 / 代码已实现 / 数据库审计投影不裁决运行期事实

## 1. 依据

```text
用户明确指令：本地 SQL Server 已启动，实现数据写入，并从控制面板查看。
流程图/20260710_SQLServer结构审计投影与控制面板查看流程图_v0.1.md
流程图/20260710_SQLServer结构审计投影与控制面板查看流程图_v0.1.html
规范/详细设计/SQLServer结构审计投影与控制面板查看详细设计.md
规范/详细设计/事件日志持久化恢复详细设计.md
规范/详细设计/仓库快照格式与恢复拒绝矩阵详细设计.md
计划/已完成计划/20260710_PANEL-RUN-S1_控制面板原生窗口可运行代码实施切片_v0.1.md
```

## 2. 目标

实现第一轮 SQL Server 结构统计审计投影：程序核心自检完成后将当前节点、关系、索引数量写入本地 SQL Server，并由原生控制面板只读显示最近记录。

## 3. 允许文件

```text
海中鱼巣/适配/SQL数据库适配.h
海中鱼巣/适配/SQL数据库适配.cpp
海中鱼巣/界面/控制面板窗口.h
海中鱼巣/界面/控制面板窗口.cpp
海中鱼巣/入口.cpp
海中鱼巣.vcxproj
计划/计划索引.md
项目记忆/Codex任务队列.md
项目记忆/当前状态.md
项目记忆/决策记录.md
项目记忆/实施记录.md
项目记忆/待确认问题.md
项目记忆/验收记录.md
实施记录/20260710_DB-PANEL-S1_SQLServer结构审计投影与控制面板查看代码实施_Codex断点清单.md
```

## 4. 禁止文件与能力

```text
禁止修改海中鱼巣/核心/** 和海中鱼巣/领域/**。
禁止数据库直接写核心仓库或裁决机器事实。
禁止控制面板调用数据库写入入口。
禁止实现仓库恢复、回放、旧表迁移或旧 ADO 函数体复制。
禁止接 WebView、HTML 数据通道、HTTP、本机端口、D455、体素或外设。
```

## 5. 实施步骤

### S1 SQL Server 适配

新增 ODBC 适配器，使用 Windows 集成认证连接 `.\SQLEXPRESS`，初始化数据库和审计表，提供参数化写入与最近记录查询。

### S2 入口写入

核心自检通过后读取可持久化结构统计快照，写入一条审计记录并立即读回。数据库失败只产生局部结果和日志，不改变核心通过状态。

### S3 控制面板查询

控制面板新增数据库状态和审计列表，启动和刷新时只读查询最近 50 条记录。

### S4 验证与收口

执行 DB-PANEL-A1 至 A10，生成实施记录，归档计划并同步计划索引和项目记忆。

## 6. 验证命令

```powershell
Get-Service MSSQL$SQLEXPRESS
sqlcmd -S '.\SQLEXPRESS' -E -l 3 -Q "SELECT @@SERVERNAME"
msbuild .\海中鱼巣.vcxproj /p:Configuration=Debug /p:Platform=x64 /m
.\x64\Debug\海中鱼巣.exe --database-self-test-exit
sqlcmd -S '.\SQLEXPRESS' -E -d '海中鱼巣' -Q "SELECT TOP (5) * FROM dbo.结构统计审计 ORDER BY 审计编号 DESC"
git diff --check
python .\tools\check_specs.py
```

真实窗口验收必须确认数据库审计列表可见，并通过 `WM_CLOSE` 正常退出。

## 7. 完成声明边界

最多声明 SQL Server 结构统计审计投影写入、读回和控制面板只读查看完成；不得声明完整仓库持久化、数据库恢复、旧数据库迁移或数据库成为运行期事实源。
