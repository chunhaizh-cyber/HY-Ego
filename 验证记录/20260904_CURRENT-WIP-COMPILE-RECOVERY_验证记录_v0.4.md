# CURRENT-WIP-COMPILE-RECOVERY 验证记录

计划身份：`CURRENT-WIP-COMPILE-RECOVERY`

计划版本 / blob：v0.4 / `b9c58b80f77c4ac29683fa263d597e0770e79c48`

验证基线：`ef5e176afceef1b7e90aefea173e8002a173d164` 加本计划两份记录

验证日期：2026-09-04

## 1. 静态合同

| 验证项 | 命令 / 方法 | 结果 |
| --- | --- | --- |
| 普通应用归零 | 对 `海中鱼巣` 的 `*.ixx` / `*.cpp` 搜索 `海中鱼巣.装配.普通应用` 与 `装配.普通应用.ixx` | 0 命中；源码文件不存在 |
| 旧筹办消费者归零 | 对 `海中鱼巣` 的 `*.ixx` / `*.cpp` 搜索旧 module、`需求初次筹办准备提供者`、`需求初次筹办准备结果` | 0 命中；未编译的旧 DTO 头定义不冒充消费者 |
| contract 登记 | 在 `海中鱼巣.vcxproj` 与 `.filters` 精确搜索 `海中鱼巣\业务\合同.自我世界树根消费.ixx` | 工程 1 条，filters 1 条 |
| contract 消费 | 精确搜索 `import 海中鱼巣.业务.合同.自我世界树根消费;` | 真实自我 provider 与自我线程创建模块各 1 条 |
| 真实自我空壳 | 审查 `真实自我形成提供者::形成` | 只返回 `请求无效`；无 L1/L2 调用和写入 |
| 普通模式显式失败 | 审查 `运行普通程序` | 唯一返回 `初始化失败 / 普通应用装配` |
| UTF-8 工程选项 | 搜索 `<AdditionalOptions>/utf-8` | Win32/x64 的 Debug/Release 共 4 条 |

## 2. 隔离构建

使用项目要求的 Visual Studio 18 / MSBuild 18.4.0，均执行 `/m:1 /t:Rebuild /p:Platform=x64`：

```powershell
& 'C:\Program Files\Microsoft Visual Studio\18\Professional\MSBuild\Current\Bin\MSBuild.exe' '.\海中鱼巣.vcxproj' /m:1 /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /p:IntDir='D:\TEMP\海中鱼巣\CURRENT-WIP-COMPILE-RECOVERY-v0.4\Debug\obj\' /p:OutDir='D:\TEMP\海中鱼巣\CURRENT-WIP-COMPILE-RECOVERY-v0.4\Debug\bin\'

& 'C:\Program Files\Microsoft Visual Studio\18\Professional\MSBuild\Current\Bin\MSBuild.exe' '.\海中鱼巣.vcxproj' /m:1 /t:Rebuild /p:Configuration=Release /p:Platform=x64 /p:IntDir='D:\TEMP\海中鱼巣\CURRENT-WIP-COMPILE-RECOVERY-v0.4\Release\obj\' /p:OutDir='D:\TEMP\海中鱼巣\CURRENT-WIP-COMPILE-RECOVERY-v0.4\Release\bin\'
```

| 配置 | 结果 | 警告 / 错误 | 日志 |
| --- | --- | --- | --- |
| Debug x64 | 退出码 0 | 0 / 0 | `D:\TEMP\海中鱼巣\CURRENT-WIP-COMPILE-RECOVERY-v0.4\Debug\msbuild-v18.log` |
| Release x64 | 退出码 0 | 0 / 0 | `D:\TEMP\海中鱼巣\CURRENT-WIP-COMPILE-RECOVERY-v0.4\Release\msbuild-v18.log` |

首次误用 VS 2022 MSBuild 的 Debug 尝试在进入编译前以 MSB8020 退出：该安装没有项目要求的 v145 工具集。证据保存在 `D:\TEMP\海中鱼巣\CURRENT-WIP-COMPILE-RECOVERY-v0.4\Debug\msbuild.log`；改用本机 Visual Studio 18 后，同一配置成功，因此归为工具链选择问题，不是源码或设计漂移。

## 3. 仓库检查

- `git diff --check`：通过。
- `git diff --cached --check`：在精确暂存两份记录后执行。
- `python .\tools\check_specs.py --strict`：退出码 0，97 份目录项全部通过。

## 4. 结论与未验证范围

结论：当前 canonical 工程的 Debug x64 与 Release x64 均完成编译；普通应用 module/path、旧筹办消费者和 E2E 未恢复，真实自我与普通模式保持明确非成功、零副作用形状。

本结论不证明链接以外的启动、运行、端到端测试、业务正确性、服务验收、事实恢复、跨进程行为或旧 ABI 等价。构建产出位于仓库外隔离目录，不进入 Git。
