# SYSTEM-WORLD-TREE-ROOT-PUBLISH-I1 验证记录

日期：2026-08-13

## 已运行

| 验证 | 命令 | 结果 |
| --- | --- | --- |
| 根工程 Debug fresh Rebuild | `MSBuild.exe .\海中鱼巣.slnx /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /m /nologo /v:minimal` | PASS，生成 `x64/Debug/海中鱼巣.exe` |
| 根工程 Release fresh Rebuild | `MSBuild.exe .\海中鱼巣.slnx /t:Rebuild /p:Configuration=Release /p:Platform=x64 /m /nologo /v:minimal` | PASS，生成 `x64/Release/海中鱼巣.exe` |
| I1 专项 Debug | `powershell -ExecutionPolicy Bypass -File .\验证工具\运行系统世界树根建立与正式读回发布参数验证.ps1 -配置 Debug` | PASS，10 项，失败 0 |
| I1 专项 Release | `powershell -ExecutionPolicy Bypass -File .\验证工具\运行系统世界树根建立与正式读回发布参数验证.ps1 -配置 Release` | PASS，10 项，失败 0 |
| 正式规范目录 | `python .\tools\check_specs.py --strict` | PASS，113 / 113 |
| XML | PowerShell 解析三个工程 XML | PASS |
| diff | `git diff --check -- <九条白名单>` | PASS |

专项双配置实际覆盖：普通应用装配；A1 与普通应用 R1 同址；首次调用前无发布；无效版本和身份写前拒绝；首次根建立、正式登记 / 整树读回与发布形状；上下文发布一致；已发布重复零根写且重新正式读取；值式 getter 隔离；四线程同请求串行并保持重复形状。

源代码门禁确认：独立 I1 实现文件和工程登记零命中；生产代码无验证实体、故障钩子、sleep、计数器或阶段 15；启动、程序结果和阶段文件零 diff；根工程 / filters 只唯一登记 DTO 头。

## NOT_RUN

- 真实分配失败；
- 私有损坏注入和提交后故障；
- 持久化、崩溃、跨进程恢复和长时生产；
- 正式启动对 I1 的调用、阶段 14、I1 成功交给 I2、阶段 16—18；
- I2、真实自我、概念维度根；
- 独立集成验收。

因此本记录不证明正式入口可达、阶段接线、自我初始化或系统闭环。
