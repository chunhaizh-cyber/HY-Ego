# FOUR-OWNER-ATOMIC-FEATURE-BIRTH 验证记录 v0.2

日期：2026-09-16
基线：`5de0a83833aece60ff2ac13d1dd65143c6970797`；未提交、未推送。

## 验证矩阵

| 检查 | 命令或依据 | 结果 |
| --- | --- | --- |
| Debug x64 隔离重建 | `MSBuild.exe 海中鱼巣.vcxproj /t:Rebuild /m:1 /p:Configuration=Debug /p:Platform=x64`，`BaseIntermediateOutputPath`、`IntDir`、`OutDir` 均在 `D:/TEMP/海中鱼巣/FOUR-OWNER-ATOMIC-FEATURE-BIRTH/20260916-front-build/Debug-x64` | 退出码 0；0 warnings、0 errors；耗时 00:02:41.71 |
| Release x64 隔离重建 | 同上，配置改为 `Release`，隔离目录为 `.../Release-x64` | 退出码 0；0 warnings、0 errors；耗时 00:02:25.39 |
| 新 L3 旧链静态检索 | 针对两个新 L3 头/源检索旧观察、旧定位、`指定F` 与两种观察分类符号 | 零命中 |
| 限定差异格式 | 已跟踪代码/工程路径运行 `git diff --check --`，两份未跟踪记录分别运行 `git diff --no-index --check -- NUL <记录>` | 通过 |
| 暂存区格式 | `git diff --cached --check` | 通过；本轮未暂存 |

构建日志与 binlog：

- `D:/TEMP/海中鱼巣/FOUR-OWNER-ATOMIC-FEATURE-BIRTH/20260916-front-build/Debug-x64/Debug-x64-rerun4.log`
- `D:/TEMP/海中鱼巣/FOUR-OWNER-ATOMIC-FEATURE-BIRTH/20260916-front-build/Debug-x64/Debug-x64-rerun4.binlog`
- `D:/TEMP/海中鱼巣/FOUR-OWNER-ATOMIC-FEATURE-BIRTH/20260916-front-build/Release-x64/Release-x64.log`
- `D:/TEMP/海中鱼巣/FOUR-OWNER-ATOMIC-FEATURE-BIRTH/20260916-front-build/Release-x64/Release-x64.binlog`

## 已知工具边界

`python .\tools\check_specs.py --strict` 未作为通过项：当前工具仍硬编码读取已删除的 `海中鱼巣/核心/服务.L1事实基座.ixx`，因而在文件读取处失败。该路径及工具不属于本计划允许范围；未为取得通过而恢复已删除 ixx 或修改工具。

## 未验证范围

- 未运行 Debug 或 Release 产物。
- 未执行专项验证程序、数据库恢复、持久库读回或并发/幂等运行验证。
- 未完成正式集成验收，不能将本记录升级为四 owner 运行正确、恢复正确或业务闭环完成。
