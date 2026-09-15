# METHOD-REGISTRY-ROOT-PRODUCTION-INIT 验证记录

计划：`METHOD-REGISTRY-ROOT-PRODUCTION-INIT` v0.4。基线：`471c7dc1530e3c6e70bdd9307784517e259940d1`。

## 已执行

| 检查 | 命令 / 材料 | 结果 |
| --- | --- | --- |
| Debug 构建 | `msbuild .\海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /p:PlatformToolset=v143 /m:1 /v:minimal` | 通过，链接 `x64\Debug\海中鱼巣.exe` |
| Release 构建 | `msbuild .\海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Release /p:Platform=x64 /p:PlatformToolset=v143 /m:1 /v:minimal` | 通过，链接 `x64\Release\海中鱼巣.exe` |
| 隔离内存 L1 专项 | `方法登记根隔离验证.cpp` 与新增两对源文件，以 `cl /std:c++20` 编译并运行 | 退出码 0；第一次 provider 初始化为 `已初始化`，第二次为 `精确重复`，稳定编码相同 |

构建与专项临时材料位于 `D:\TEMP\海中鱼巣\METHOD-REGISTRY-ROOT-PRODUCTION-INIT\build-1\`。

## 结论边界

验证证明当前白名单源码可在 Debug/Release x64 编译链接，且隔离内存 L1 可完成一根的首次建立及同编码重复读回。它不证明真实持久仓跨进程恢复、普通应用默认数据目录、阶段21、本能根、线程、任务治理、运行宿主或业务闭环。
