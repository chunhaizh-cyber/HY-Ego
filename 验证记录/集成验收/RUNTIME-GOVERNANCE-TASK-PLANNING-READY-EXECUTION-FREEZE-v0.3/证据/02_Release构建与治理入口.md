# Release 构建与治理入口

## 隔离根

```text
D:\TEMP\海中鱼巣\RUNTIME-GOVERNANCE-TASK-PLANNING-READY-EXECUTION-FREEZE\20260823-acceptance-e7970edf-01
```

该目录由`git archive e7970edfba0283cad9934ee152dd9488015a51cc`展开；未复制当前工作区 WIP。

## Release Rebuild

命令：

```powershell
& 'C:\Program Files\Microsoft Visual Studio\18\Professional\MSBuild\Current\Bin\MSBuild.exe' `
  '.\海中鱼巣.slnx' /t:Rebuild /p:Configuration=Release /p:Platform=x64 /m
```

结果：

```text
exit=0
已成功生成。
0 个警告
0 个错误
已用时间 00:00:52.49
```

链接输入明确包含：

```text
端到端测试.需求服务.ixx.obj
端到端测试.自我治理一致事实.ixx.obj
```

因此“能够构建”成立，但“正式构建隔离”失败。

## 既有治理入口

命令：

```powershell
.\x64\Release\海中鱼巣.exe --test-治理
```

首次执行超过 60 秒工具时限；确认该精确路径没有残留进程后重新独立执行。第二次结果：

```text
exit=0
elapsed=89.7s
[治理测试] 全部通过
PROCESS_EXIT_CODE=0
```

该运行输出同时明确保留多项`NOT_RUN`。它只证明既有自检在该 Release 构建中运行并退出 `0`，不证明本服务的正式成功路径、正式读回或正式构建隔离。

## 临时根清理分账

按 CODE-FILE-01 尝试删除本轮精确隔离根时，执行环境策略拒绝递归删除命令；未使用跨 shell 或其它规避方式。只读复核结果：

```text
RUNNING_PROCESS_COUNT=0
RESIDUAL_FILE_COUNT=16280
RESIDUAL_BYTES=784990477
```

残留只含候选源码副本、独立 `.git` 元数据和 Release 构建产物，不含凭据、个人信息或业务材料，不占用进程、端口、数据库或外设。清理所有者为本验收切片；该普通残留不改变已取得的构建与运行证据。
